#include "CallId.h"

#include <string.h> //for strlen

#if 0
//FSK 包类型
//int const CALLID_TYPE_NONE = 0x00;
int const CALLID_TYPE_DTMF = 0x01;
int const CALLID_TYPE_FSK_SIMPLE = 0x04; // YDN069-1997 标准规定
int const CALLID_TYPE_FSK_COMPLEX = 0x80; // YDN069-1997 标准规定

//复合格式参数类型（用于解码）
int const CALLID_PARAM_TIME = 0x01; // YDN069-1997 标准规定
int const CALLID_PARAM_NUMBER = 0x02; // YDN069-1997 标准规定
int const CALLID_PARAM_NONUMBER = 0x04; // YDN069-1997 标准规定
int const CALLID_PARAM_NAME = 0x07; // YDN069-1997 标准规定
int const CALLID_PARAM_NONAME = 0x08; // YDN069-1997 标准规定

//不能显示来电原因（用于应用）
int const CALLID_OK = 0;
int const CALLID_NONE = 1;
int const CALLID_FORBID = 2;
int const CALLID_NOTGET = 3;
int const CALLID_ERROR = 4;
#endif
namespace Telephone {
    namespace Logical {
        //decode call id package
        int const TEL_DTMFEND = 0x8E; // DTMF 来电结束
        int const TEL_FSKEND = 0x8F; // FSK 来电结束

        int const DTMF_CHANNEL_STRAT = 0x30; // DTMF 来电通道开始
        int const DTMF_CHANNEL_END = 0x3F; // DTMF 来电通道结束

        int const FSK_CHANNEL_STRAT = 0x40; // FSK 来电通道开始
        int const FSK_CHANNEL_END = 0x4F; // FSK 来电通道结束

        char const * const DTMFNUMBER = "D1234567890*#ABC";

        //复合格式参数类型（用于解码）
        enum ComplexParamType {
            CALLID_PARAM_TIME = 0x01, // YDN069-1997 标准规定
            CALLID_PARAM_NUMBER = 0x02, // YDN069-1997 标准规定
            CALLID_PARAM_NONUMBER = 0x04, // YDN069-1997 标准规定
            CALLID_PARAM_NAME = 0x07, // YDN069-1997 标准规定
            CALLID_PARAM_NONAME = 0x08, // YDN069-1997 标准规定
        };

        CallId const CallId::Parse(unsigned char const * const buffer) {
            CallId result;
            bool flag = false;

            if (*buffer >= DTMF_CHANNEL_STRAT && *buffer <= DTMF_CHANNEL_END) {
                flag = result.decodeDTMFCallIDPackage_(buffer);
            } else if (*buffer >= FSK_CHANNEL_STRAT && *buffer <= FSK_CHANNEL_END) {
                  flag = result.decodeFSKCallIDPackage_(buffer);
            }

            if (flag) {
                // 过滤无效号码字符
                int numberLength = strlen((char*)(result.number));

                int filteredNumberIndex = 0;
                for (int orgNumberIndex = 0; orgNumberIndex < numberLength; ++orgNumberIndex) {
                    if ((result.number[orgNumberIndex] >= '0') && (result.number[orgNumberIndex] <= '9')) {
                        result.number[filteredNumberIndex] = result.number[orgNumberIndex];
                        ++filteredNumberIndex;
                    }
                }
                result.number[filteredNumberIndex] = '\0';

                if (filteredNumberIndex == 0) {
                    //result->numberStatus = CALLID_NONE;
                    flag = false;
                }
            } else {
                result.number[0] = '\0';
                result.numberStatus = CALLID_ERROR;
            }
            return result;
        }

        CallId::CallId(unsigned char const * const buffer) {
            if (buffer) {
                bool flag = false;

                if (*buffer >= DTMF_CHANNEL_STRAT && *buffer <= DTMF_CHANNEL_END) {
                    flag = decodeDTMFCallIDPackage_(buffer);
                } else if (*buffer >= FSK_CHANNEL_STRAT && *buffer <= FSK_CHANNEL_END) {
                      flag = decodeFSKCallIDPackage_(buffer);
                }

                if (flag) {
                    // 过滤无效号码字符
                    int numberLength = strlen((char*)(number));

                    int filteredNumberIndex = 0;
                    for (int orgNumberIndex = 0; orgNumberIndex < numberLength; ++orgNumberIndex) {
                        if ((number[orgNumberIndex] >= '0') && (number[orgNumberIndex] <= '9')) {
                            number[filteredNumberIndex] = number[orgNumberIndex];
                            ++filteredNumberIndex;
                        }
                    }
                    number[filteredNumberIndex] = '\0';

                    if (filteredNumberIndex == 0) {
                        //numberStatus = CALLID_NONE;
                        flag = false;
                    }
                } else {
                    number[0] = '\0';
                    numberStatus = CALLID_ERROR;
                }
                //return flag;
            }
        }

        bool CallId::decodeDTMFCallIDPackage_(unsigned char const * const buffer) {
            int i;
            for (i = 0; (*(buffer + i) != TEL_DTMFEND) && (i < CALLID_NUM_LEN - 1); ++i) {
                number[i] = DTMFNUMBER[*(buffer + i) - DTMF_CHANNEL_STRAT];
            }
            number[i] = '\0';
            numberStatus = CALLID_OK;
            type = CALLID_TYPE_DTMF;
            return true;
        }

        void CallId::decodeFSKSimpleParam_(unsigned char const * const data, int const dataLength) {
            //Decode time
            month = (*(data + 0) - '0') * 10 + (*(data + 1) - '0');
            day = (*(data + 2) - '0') * 10 + (*(data + 3) - '0');
            hour = (*(data + 4) - '0') * 10 + (*(data + 5) - '0');
            minute = (*(data + 6) - '0') * 10 + (*(data + 7) - '0');
            timeStatus = CALLID_OK;

            //Decode number
            unsigned char* const numberPointer = const_cast<unsigned char* const>(data + 8);
            int numberLength = dataLength - 8;

            //去掉奇偶校验信息
            for (int i = 0; i < numberLength; ++i) {
                *(numberPointer + i) &= 0x7F;
            }

            if (*numberPointer == 'O') {
                numberStatus = CALLID_NOTGET;
            } else if (*numberPointer == 'P') {
                numberStatus = CALLID_FORBID;
            } else {
                for (int i = 0; i < numberLength; ++i) {
                    *(numberPointer + i) &= 0x0F;
                }
                for (int i = 0; i < numberLength; ++i) {
                    *(numberPointer + i) |= 0x30;
                }
                int i;
                for (i = 0; (i < numberLength) && (i < CALLID_NUM_LEN - 1); ++i) {
                    number[i] = *(numberPointer + i);
                }
                  number[i] = '\0';
                  numberStatus = CALLID_OK;
            }
        }

        void CallId::decodeFSKComplexParam_(unsigned char const * const parameter) {
            // parameter: pointer_parameter, pd: pointer_data
            int dataLength = *(parameter + 1);
            unsigned char* const data = const_cast<unsigned char* const>(parameter + 2);

            //odd-even check error
            //just cast off check bit temporarily
            int i;
            for (i = 0; i < dataLength; ++i) {
                *(data + i) &= 0x7F;
            }

            switch (*parameter) {
            case CALLID_PARAM_TIME:
                //Decode date time
                month = (*(data + 0) - '0') * 10 + (*(data + 1) - '0');
                day = (*(data + 2) - '0') * 10 + (*(data + 3) - '0');
                hour = (*(data + 4) - '0') * 10 + (*(data + 5) - '0');
                minute = (*(data + 6) - '0') * 10 + (*(data + 7) - '0');
                timeStatus = CALLID_OK;
                break;

            case CALLID_PARAM_NUMBER:
                //Decode number
                for (i = 0; i < dataLength; ++i) {
                    *(data + i) &= 0x0F;
                }
                for (i = 0; i < dataLength; ++i) {
                    *(data + i) |= 0x30;
                }
                for (i = 0; (i < dataLength) && (i < CALLID_NUM_LEN - 1); ++i) {
                    number[i] = *(data + i);
                }
                number[i] = '\0';
                numberStatus = CALLID_OK;
                break;

            case CALLID_PARAM_NONUMBER:
                //Set 'no number'
                if (*data == 'O') {
                    numberStatus = CALLID_NOTGET;
                } else if (*data == 'P') {
                    numberStatus = CALLID_FORBID;
                } else {
                    numberStatus = CALLID_ERROR;
                }
                break;

            case CALLID_PARAM_NAME:
                //Decode name
                for (i = 0; (i < dataLength) && (i < CALLID_NAME_LEN - 1); ++i) {
                    name[i] = *(data + i);
                }
                name[i] = '\0';
                nameStatus = CALLID_OK;
                break;

            case CALLID_PARAM_NONAME:
                //Set 'no name'
                if (*data == 'O') {
                    nameStatus = CALLID_NOTGET;
                } else if (*data == 'P') {
                    nameStatus = CALLID_FORBID;
                } else {
                    nameStatus = CALLID_FORBID;
                }
                break;
            }
        }

        bool CallId::decodeFSKCallIDPackage_(unsigned char const * const buffer) {
            //Assemble package
            unsigned char* buf = const_cast<unsigned char*>(buffer);
            int packetLength;
            for (packetLength = 0; *(buf + packetLength) != TEL_FSKEND; ++packetLength) {
                if (packetLength % 2 == 0) {
                    *(buf + packetLength / 2) = (*(buf + packetLength) << 4) & 0xF0;
                } else {
                      *(buf + packetLength / 2) |= (*(buf + packetLength) & 0x0F);
                }
            }

            //Locate pakage start flag error
            packetLength /= 2;
            int i;
            for (i = 0; i < packetLength; ++i) {
                if ((*buf == CALLID_TYPE_FSK_SIMPLE) || (*buf == CALLID_TYPE_FSK_COMPLEX)) {
                    break;
                } else {
                    ++buf;
                }
            }
            if (i == packetLength) {
                return false;
            } else {
                packetLength -= i;
            }

            //Package check sum error
            unsigned char sum = 0;
            for (i = 0; i < packetLength - 1; ++i) {
                sum += *(buf + i);
            }

            //Package length error
            int dataLength = *(buf + 1);

            if (*buf == CALLID_TYPE_FSK_SIMPLE) {
                //Decode simple format
                unsigned char* data = buf + 2;
                decodeFSKSimpleParam_(data, dataLength);
                type = CALLID_TYPE_FSK_SIMPLE;
            } else if (*buf == CALLID_TYPE_FSK_COMPLEX) {
                  //Decode complex format
                unsigned char* parameter = buf + 2;
                while (parameter < buf + 2 + dataLength) {
                    decodeFSKComplexParam_(parameter); // Decode every parameter
                    parameter += *(parameter + 1) + 2;
                }
                  type = CALLID_TYPE_FSK_COMPLEX;
            }
            return true;
        }
#if 0
        bool DecodeDTMFCallIDPackage(unsigned char const* const buf, CALLID_INFO* const pcallid) {
            int i;
            for (i = 0; (*(buf + i) != TEL_DTMFEND) && (i < CALLID_NUM_LEN - 1); ++i) {
                pcallid->number[i] = DTMFNUMBER[*(buf + i) - DTMF_CHANNEL_STRAT];
            }
            pcallid->number[i] = '\0';
            pcallid->numberStatus = CALLID_OK;
            pcallid->type = CALLID_TYPE_DTMF;
            return true;
        }

        void DecodeFSKSimpleParam(unsigned char *pd, int datalen, CALLID_INFO *pcallid) {
            /*
            *   Decode time
            */
            pcallid->month = (*(pd + 0) - '0') * 10 + (*(pd + 1) - '0');
            pcallid->day = (*(pd + 2) - '0') * 10 + (*(pd + 3) - '0');
            pcallid->hour = (*(pd + 4) - '0') * 10 + (*(pd + 5) - '0');
            pcallid->minute = (*(pd + 6) - '0') * 10 + (*(pd + 7) - '0');
            pcallid->timestatus = CALLID_OK;

            /*
            *   Decode number
            */
            unsigned char   *pnum = pd + 8;
            int     numlen = datalen - 8, i;

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

        void DecodeFSKComplexParam(unsigned char *pparm, CALLID_INFO *pcallid) {
            // pparm: pointer_parameter, pd: pointer_data
            int     datalen, i;
            unsigned char   *pd;

            datalen = *(pparm + 1);
            pd = pparm + 2;

            /*
            *   odd-even check error
            *   just cast off check bit temporarily
            */
            for (i = 0; i < datalen; i ++)
                *(pd + i) &= 0x7F;

            switch (*pparm){
            case CALLID_PARAM_TIME:
                /*
                *   Decode date time
                */
                pcallid->month = (*(pd + 0) - '0') * 10 + (*(pd + 1) - '0');
                pcallid->day = (*(pd + 2) - '0') * 10 + (*(pd + 3) - '0');
                pcallid->hour = (*(pd + 4) - '0') * 10 + (*(pd + 5) - '0');
                pcallid->minute = (*(pd + 6) - '0') * 10 + (*(pd + 7) - '0');
                pcallid->timestatus = CALLID_OK;
                break;

            case CALLID_PARAM_NUMBER:
                /*
                *   Decode number
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
                *   Set 'no number'
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
                *   Decode name
                */
                for (i = 0; (i < datalen) && (i < CALLID_NAME_LEN -1); ++i) {
                    pcallid->name[i] = *(pd + i);
                }
                pcallid->name[i] = '\0';
                pcallid->namestatus = CALLID_OK;
                break;

            case CALLID_PARAM_NONAME:
                /*
                *   Set 'no name'
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

        bool DecodeFSKCallIDPackage(unsigned char *buf, CALLID_INFO *pcallid) {
            int     pklen, datalen, i;
            unsigned char   *pparm;     // pparm: pointer_parameter
            unsigned char   *pd;        // pd:    pointer_data
            /*
            *   Assemble package
            */
            for (pklen = 0; *(buf + pklen) != TEL_FSKEND; pklen ++){
                if (pklen % 2 == 0) {
                    *(buf + pklen / 2) = (*(buf + pklen) << 4) & 0xF0;
                } else {
                    *(buf + pklen / 2) |= (*(buf + pklen) & 0x0F);
                }
            }

            /*
            *   Locate pakage start flag error
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
            *   Package check sum error
            */
            unsigned char   sum = 0;
            for (i = 0; i < pklen - 1; ++i)
                sum += *(buf + i);

            /*
            *   Package length error
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

        bool DecodeCallIDPackage(unsigned char *buf, CALLID_INFO *pcallid) {
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
#endif
    }
}
