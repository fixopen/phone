#include "StdAfx.h"
#include "CallId.h"

//decode call id package
#define TEL_DTMFEND             0x8E // DTMF 来电结束
#define TEL_FSKEND              0x8F // FSK 来电结束

#define DTMF_CHANNEL_STRAT      0x30 // DTMF 来电通道开始
#define DTMF_CHANNEL_END        0x3F // DTMF 来电通道结束

#define FSK_CHANNEL_STRAT       0x40 // FSK 来电通道开始
#define FSK_CHANNEL_END         0x4F // FSK 来电通道结束

//FSK 包类型
//#define CALLID_TYPE_NONE        0
#define CALLID_TYPE_DTMF        1
#define CALLID_TYPE_FSK_SIMPLE  0x04 // YDN069-1997 标准规定
#define CALLID_TYPE_FSK_COMPLEX 0x80 // YDN069-1997 标准规定

//复合格式参数类型（用于解码）
#define CALLID_PARAM_TIME       0x01 // YDN069-1997 标准规定
#define CALLID_PARAM_NUMBER     0x02 // YDN069-1997 标准规定
#define CALLID_PARAM_NONUMBER   0x04 // YDN069-1997 标准规定
#define CALLID_PARAM_NAME       0x07 // YDN069-1997 标准规定
#define CALLID_PARAM_NONAME     0x08 // YDN069-1997 标准规定

//不能显示来电原因（用于应用）
#define CALLID_OK               0
#define CALLID_NONE             1
#define CALLID_FORBID           2
#define CALLID_NOTGET           3
#define CALLID_ERROR            4

const char *const DTMFNUMBER = "D1234567890*#ABC";

//CallId::CallId(char const* const buffer) {
//}

bool DecodeDTMFCallIDPackage(BYTE *buf, CALLID_INFO *pcallid) {
    int i;

    for (i = 0; (*(buf + i) != TEL_DTMFEND) && (i < CALLID_NUM_LEN - 1); ++i) {
        pcallid->number[i] = DTMFNUMBER[*(buf + i) - DTMF_CHANNEL_STRAT];
    }
    pcallid->number[i] = '\0';
    pcallid->numberstatus = CALLID_OK;
    pcallid->type = CALLID_TYPE_DTMF;
    return true;
}

void DecodeFSKSimpleParam(BYTE *pd, int datalen, CALLID_INFO *pcallid) {
    /*
    *	Decode time
    */
    pcallid->month = (*(pd + 0) - '0') * 10 + (*(pd + 1) - '0');
    pcallid->day = (*(pd + 2) - '0') * 10 + (*(pd + 3) - '0');
    pcallid->hour = (*(pd + 4) - '0') * 10 + (*(pd + 5) - '0');
    pcallid->minute = (*(pd + 6) - '0') * 10 + (*(pd + 7) - '0');
    pcallid->timestatus = CALLID_OK;

    /*
    *	Decode number
    */
    BYTE	*pnum = pd + 8;
    int		numlen = datalen - 8, i;

    // byw, 2002/8/27, 去掉奇偶校验信息
    for (i = 0; i < numlen; i ++)
    {
        *(pnum + i) &= 0x7F;
    }

    if (*pnum == 'O') {
        pcallid->numberstatus = CALLID_NOTGET;
    } else if (*pnum == 'P') {
        pcallid->numberstatus = CALLID_FORBID;
    } else {
        for (i = 0; i < numlen; i ++) {
            *(pnum + i) &= 0x0F;
        }
        for (i = 0; i < numlen; i ++) {
            *(pnum + i) |= 0x30;
        }

        for (i = 0; (i < numlen) && (i < CALLID_NUM_LEN -1); i ++) {
            pcallid->number[i] = *(pnum + i);
        }
        pcallid->number[i] = '\0';
        pcallid->numberstatus = CALLID_OK;
    }
}

void DecodeFSKComplexParam(BYTE *pparm, CALLID_INFO *pcallid) {
    // pparm: pointer_parameter, pd: pointer_data
    int		datalen, i;
    BYTE	*pd;

    datalen = *(pparm + 1);
    pd = pparm + 2;

    /*
    *	odd-even check error
    *	just cast off check bit temporarily
    */
    for (i = 0; i < datalen; i ++)
        *(pd + i) &= 0x7F;

    switch (*pparm){

    case CALLID_PARAM_TIME:
        /*
        *	Decode date time
        */
        pcallid->month = (*(pd + 0) - '0') * 10 + (*(pd + 1) - '0');
        pcallid->day = (*(pd + 2) - '0') * 10 + (*(pd + 3) - '0');
        pcallid->hour = (*(pd + 4) - '0') * 10 + (*(pd + 5) - '0');
        pcallid->minute = (*(pd + 6) - '0') * 10 + (*(pd + 7) - '0');
        pcallid->timestatus = CALLID_OK;
        break;

    case CALLID_PARAM_NUMBER:
        /*
        *	Decode number
        */
        for (i = 0; i < datalen; ++i) {
            *(pd + i) &= 0x0F;
        }
        for (i = 0; i < datalen; ++i) {
            *(pd + i) |= 0x30;
        }
        for (i = 0; (i < datalen) && (i < CALLID_NUM_LEN -1); ++i) {
            pcallid->number[i] = *(pd + i);
        }
        pcallid->number[i] = '\0';
        pcallid->numberstatus = CALLID_OK;
        break;

    case CALLID_PARAM_NONUMBER:
        /*
        *	Set 'no number'
        */
        if (*pd == 'O') {
            pcallid->numberstatus = CALLID_NOTGET;
        } else if (*pd == 'P') {
            pcallid->numberstatus = CALLID_FORBID;
        } else {
            pcallid->numberstatus = CALLID_ERROR;
        }
        break;

    case CALLID_PARAM_NAME:
        /*
        *	Decode name
        */
        for (i = 0; (i < datalen) && (i < CALLID_NAME_LEN -1); ++i) {
            pcallid->name[i] = *(pd + i);
        }
        pcallid->name[i] = '\0';
        pcallid->namestatus = CALLID_OK;
        break;

    case CALLID_PARAM_NONAME:
        /*
        *	Set 'no name'
        */
        if (*pd == 'O') {
            pcallid->namestatus = CALLID_NOTGET;
        } else if (*pd == 'P') {
            pcallid->namestatus = CALLID_FORBID;
        } else {
            pcallid->namestatus = CALLID_FORBID;
        }
        break;
    }
}

bool DecodeFSKCallIDPackage(BYTE *buf, CALLID_INFO *pcallid) {
    int		pklen, datalen, i;
    BYTE	*pparm;		// pparm: pointer_parameter
    BYTE	*pd;		// pd:    pointer_data
    /*
    *	Assemble package
    */
    for (pklen = 0; *(buf + pklen) != TEL_FSKEND; pklen ++){
        if (pklen % 2 == 0) {
            *(buf + pklen / 2) = (*(buf + pklen) << 4) & 0xF0;
        } else {
            *(buf + pklen / 2) |= (*(buf + pklen) & 0x0F);
        }
    }

    /*
    *	Locate pakage start flag error
    */
    pklen /= 2;
    for (i = 0; i < pklen; i ++) {
        if ((*buf == CALLID_TYPE_FSK_SIMPLE)
            || (*buf == CALLID_TYPE_FSK_COMPLEX)) {
            break;
        } else {
            ++buf;
        }
    }
    if (i == pklen) {
        return false;
    } else {
        pklen -= i;
    }

    /*
    *	Package check sum error
    */
    BYTE	sum = 0;
    for (i = 0; i < pklen - 1; ++i)
        sum += *(buf + i);

    /*
    *	Package length error
    */
    datalen = *(buf + 1);

    if (*buf == CALLID_TYPE_FSK_SIMPLE) { //Decode simple format
        pd = buf + 2;
        DecodeFSKSimpleParam(pd, datalen, pcallid);
        pcallid->type = CALLID_TYPE_FSK_SIMPLE;
    } else if (*buf == CALLID_TYPE_FSK_COMPLEX) { //Decode complex format
        pparm = buf + 2;
        while (pparm < buf + 2 + datalen){
            DecodeFSKComplexParam(pparm, pcallid); // Decode every parameter
            pparm += *(pparm + 1) + 2;
        }
        pcallid->type = CALLID_TYPE_FSK_COMPLEX;
    }
    return true;
}

bool DecodeCallIDPackage(BYTE *buf, CALLID_INFO *pcallid) {
    bool flag = false;

    if (*buf >= DTMF_CHANNEL_STRAT && *buf <= DTMF_CHANNEL_END) {
        flag = DecodeDTMFCallIDPackage(buf, pcallid);
    } else if (*buf >= FSK_CHANNEL_STRAT && *buf <= FSK_CHANNEL_END) {
        flag = DecodeFSKCallIDPackage(buf, pcallid);
    }

    if (flag) {
        // 过滤无效号码字符
        int i, j, k;
        i = strlen((char*)(pcallid->number));

        for (j = 0, k = 0; k < i; k ++) {
            if ((pcallid->number[k] >= '0') && (pcallid->number[k] <= '9')) {
                pcallid->number[j] = pcallid->number[k];
                j ++;
            }
        }
        pcallid->number[j] = '\0';

        if (j == 0) {
            //pcallid->numberstatus = CALLID_NONE;
            flag = false;
        }
    } else {
        pcallid->number[0] = '\0';
        pcallid->numberstatus = CALLID_ERROR;   //fjm
    }
    return flag;
}
