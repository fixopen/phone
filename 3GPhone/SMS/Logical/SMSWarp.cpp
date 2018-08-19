#include <afxwin.h>
#include <vector>
#include "SMSWarp.h"
#include "SMSParser.h"

namespace SMS {
    namespace Logical {
        void SMSWarp::processMessage(Data::Message const& message) {
            //message.Insert(); //insert modify the message's id
            state_ = sNotifier;
            if (message.remote.address == "spec code") {
                SMSParser::MessageType mt = SMSParser::getMessageType(message);
                //switch (mt) {
                //case SMSParser::tRegister:
                //    //no this case
                //    break;
                //case SMSParser::tRegisterReply:
                //    //no this case
                //    break;
                //case SMSParser::tWeather:
                //    transport_->Get(message.binaryData.substr(4), localFilename);
                //    state_ = sContentCompleted;
                //    notifyWeather("new weather info arravied", localFilename);
                //    break;
                //case SMSParser::tSoftwareDownload:
                //    //not process
                //    break;
                //case SMSParser::tMultimediaDownload:
                //    transport_->Get(message.binaryData.substr(4), localFilename);
                //    state_ = sContentCompleted;
                //    notifyMultimedia("new multimedia info arravied", localFilename);
                //    break;
                //case SMSParser::tMenuManager:
                //    transport_->Get(message.binaryData.substr(4), localFilename);
                //    state_ = sContentCompleted;
                //    notifyMenu("new menu info arravied", localFilename);
                //    break;
                //default:
                //    break;
                //}
            } else {
                //if (confirm("you have a message, do you want see it?") == yes) {
                //    showMessageDetail(message); //view same as message history message detail
                //} else {
                //    unreadedMessages_.push_back(message);
                //    indicationView("new message received");
                //}
            }
        }
    }
}

using namespace SMS;

SMSWarp::SMSWarp()
{
    m_nSendIn = 0;
    m_nSendOut = 0;
    m_nRecvIn = 0;
    m_nRecvOut = 0;

    m_hKillThreadEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    m_hThreadKilledEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

    InitializeCriticalSection(&m_csSend);
    InitializeCriticalSection(&m_csRecv);
}

SMSWarp::~SMSWarp()
{
    SetEvent(m_hKillThreadEvent);           // �����ر����̵߳��ź�
    WaitForSingleObject(m_hThreadKilledEvent, INFINITE);    // �ȴ����̹߳ر�

    DeleteCriticalSection(&m_csSend);
    DeleteCriticalSection(&m_csRecv);

    CloseHandle(m_hKillThreadEvent);
    CloseHandle(m_hThreadKilledEvent);
}

SMSWarp::SMSWarp(const SMSWarp& smsWarp)
{
}

SMSWarp* SMSWarp::GetSMSWarp()
{
    static SMSWarp* sms = 0;
    if (sms == 0)
    {
        sms = new SMSWarp();
    }
    return sms;
}

void SMSWarp::Bind(Util::ATCommandWarp* at)
{
    m_pAT = at;
    // �������߳�
    AfxBeginThread(SmThread, this, THREAD_PRIORITY_NORMAL);
}

// �ɴ�ӡ�ַ���ת��Ϊ�ֽ�����    
// �磺"C8329BFD0E01" --> {0xC8, 0x32, 0x9B, 0xFD, 0x0E, 0x01}    
// ����: pSrc - Դ�ַ���ָ��    
//       nSrcLength - Դ�ַ�������    
// ���: pDst - Ŀ������ָ��    
// ����: Ŀ�����ݳ���    
int SMSWarp::gsmString2Bytes(const char* pSrc, unsigned char* pDst, int nSrcLength)
{
    for (int i = 0; i < nSrcLength; i += 2)
    {
        // �����4λ
        if ((*pSrc >= '0') && (*pSrc <= '9'))
        {
            *pDst = (*pSrc - '0') << 4;
        }
        else
        {
            *pDst = (*pSrc - 'A' + 10) << 4;
        }

        pSrc++;

        // �����4λ
        if ((*pSrc>='0') && (*pSrc<='9'))
        {
            *pDst |= *pSrc - '0';
        }
        else
        {
            *pDst |= *pSrc - 'A' + 10;
        }

        pSrc++;
        pDst++;
    }   

    // ����Ŀ�����ݳ���
    return (nSrcLength / 2);
}   

// �ֽ�����ת��Ϊ�ɴ�ӡ�ַ���    
// �磺{0xC8, 0x32, 0x9B, 0xFD, 0x0E, 0x01} --> "C8329BFD0E01"     
// ����: pSrc - Դ����ָ��    
//       nSrcLength - Դ���ݳ���    
// ���: pDst - Ŀ���ַ���ָ��    
// ����: Ŀ���ַ�������    
int SMSWarp::gsmBytes2String(const unsigned char* pSrc, char* pDst, int nSrcLength)
{
    const char tab[]="0123456789ABCDEF";    // 0x0-0xf���ַ����ұ�

    for (int i = 0; i < nSrcLength; i++)
    {
        *pDst++ = tab[*pSrc >> 4];        // �����4λ
        *pDst++ = tab[*pSrc & 0x0f];    // �����4λ
        pSrc++;
    }

    // ����ַ����Ӹ�������
    *pDst = '\0';

    // ����Ŀ���ַ�������
    return (nSrcLength * 2);
}   

// 7bit����    
// ����: pSrc - Դ�ַ���ָ��    
//       nSrcLength - Դ�ַ�������    
// ���: pDst - Ŀ����봮ָ��    
// ����: Ŀ����봮����    
int SMSWarp::gsmEncode7bit(const char* pSrc, unsigned char* pDst, int nSrcLength)
{
    int nSrc;       // Դ�ַ����ļ���ֵ
    int nDst;       // Ŀ����봮�ļ���ֵ
    int nChar;      // ��ǰ���ڴ���������ַ��ֽڵ���ţ���Χ��0-7
    unsigned char nLeft;    // ��һ�ֽڲ��������

    // ����ֵ��ʼ��
    nSrc = 0;
    nDst = 0;

    // ��Դ��ÿ8���ֽڷ�Ϊһ�飬ѹ����7���ֽ�
    // ѭ���ô�����̣�ֱ��Դ����������
    // ������鲻��8�ֽڣ�Ҳ����ȷ����
    while (nSrc < nSrcLength)
    {
        // ȡԴ�ַ����ļ���ֵ�����3λ
        nChar = nSrc & 7;

        // ����Դ����ÿ���ֽ�
        if (nChar == 0)
        {
            // ���ڵ�һ���ֽڣ�ֻ�Ǳ�����������������һ���ֽ�ʱʹ��
            nLeft = *pSrc;
        }
        else
        {
            // ���������ֽڣ������ұ߲��������������ӣ��õ�һ��Ŀ������ֽ�
            *pDst = (*pSrc << (8-nChar)) | nLeft;

            // �����ֽ�ʣ�µ���߲��֣���Ϊ�������ݱ�������
            nLeft = *pSrc >> nChar;

            // �޸�Ŀ�괮��ָ��ͼ���ֵ
            pDst++;
            nDst++;
        }

        // �޸�Դ����ָ��ͼ���ֵ
        pSrc++;
        nSrc++;
    }   

    // ����Ŀ�괮����
    return nDst;
}

// 7bit����    
// ����: pSrc - Դ���봮ָ��    
//       nSrcLength - Դ���봮����    
// ���: pDst - Ŀ���ַ���ָ��    
// ����: Ŀ���ַ�������    
int SMSWarp::gsmDecode7bit(const unsigned char* pSrc, char* pDst, int nSrcLength) {
    int nSrc; // Դ�ַ����ļ���ֵ
    int nDst; // Ŀ����봮�ļ���ֵ
    int nByte; // ��ǰ���ڴ���������ֽڵ���ţ���Χ��0-6
    unsigned char nLeft; // ��һ�ֽڲ��������
    // ����ֵ��ʼ��
    nSrc = 0;
    nDst = 0;
    // �����ֽ���źͲ������ݳ�ʼ��
    nByte = 0;
    nLeft = 0;
    // ��Դ����ÿ7���ֽڷ�Ϊһ�飬��ѹ����8���ֽ�
    // ѭ���ô�����̣�ֱ��Դ���ݱ�������
    // ������鲻��7�ֽڣ�Ҳ����ȷ����
    while(nSrc<nSrcLength) {
        // ��Դ�ֽ��ұ߲��������������ӣ�ȥ�����λ���õ�һ��Ŀ������ֽ�
        *pDst = ((*pSrc << nByte) | nLeft) & 0x7f;
        // �����ֽ�ʣ�µ���߲��֣���Ϊ�������ݱ�������
        nLeft = *pSrc >> (7-nByte);
        // �޸�Ŀ�괮��ָ��ͼ���ֵ
        pDst++;
        nDst++;
        // �޸��ֽڼ���ֵ
        nByte++;
        // ����һ������һ���ֽ�
        if(nByte == 7) {
            // ����õ�һ��Ŀ������ֽ�
            *pDst = nLeft;
            // �޸�Ŀ�괮��ָ��ͼ���ֵ
            pDst++;
            nDst++;
            // �����ֽ���źͲ������ݳ�ʼ��
            nByte = 0;
            nLeft = 0;
        }
        // �޸�Դ����ָ��ͼ���ֵ
        pSrc++;
        nSrc++;
    }
    *pDst = 0;
    // ����Ŀ�괮����
    return nDst;
}

// 8bit����    
// ����: pSrc - Դ�ַ���ָ��    
//       nSrcLength - Դ�ַ�������    
// ���: pDst - Ŀ����봮ָ��    
// ����: Ŀ����봮����    
int SMSWarp::gsmEncode8bit(const char* pSrc, unsigned char* pDst, int nSrcLength)   
{   
    // �򵥸���    
    memcpy(pDst, pSrc, nSrcLength);   

    return nSrcLength;   
}   

// 8bit����    
// ����: pSrc - Դ���봮ָ��    
//       nSrcLength -  Դ���봮����    
// ���: pDst -  Ŀ���ַ���ָ��    
// ����: Ŀ���ַ�������    
int SMSWarp::gsmDecode8bit(const unsigned char* pSrc, char* pDst, int nSrcLength)   
{   
    // �򵥸���    
    memcpy(pDst, pSrc, nSrcLength);   

    // ����ַ����Ӹ�������    
    pDst[nSrcLength] = '\0';   

    return nSrcLength;   
}   

// UCS2����    
// ����: pSrc - Դ�ַ���ָ��    
//       nSrcLength - Դ�ַ�������    
// ���: pDst - Ŀ����봮ָ��    
// ����: Ŀ����봮����    
int SMSWarp::gsmEncodeUcs2(const char* pSrc, unsigned char* pDst, int nSrcLength) {
    int nDstLength; // UNICODE���ַ���Ŀ
    WCHAR wchar[128]; // UNICODE��������
    // �ַ���-->UNICODE��
    nDstLength = ::MultiByteToWideChar(CP_ACP, 0, pSrc, nSrcLength, wchar, 128);
    // �ߵ��ֽڶԵ������
    for(int i=0; i<nDstLength; i++) {
        // �������λ�ֽ�
        *pDst++ = wchar[i] >> 8;
        // �������λ�ֽ�
        *pDst++ = wchar[i] & 0xff;
    }
    // ����Ŀ����봮����
    return nDstLength * 2;
}

// UCS2����    
// ����: pSrc - Դ���봮ָ��    
//       nSrcLength -  Դ���봮����    
// ���: pDst -  Ŀ���ַ���ָ��    
// ����: Ŀ���ַ�������    
int SMSWarp::gsmDecodeUcs2(const unsigned char* pSrc, char* pDst, int nSrcLength) {
    int nDstLength;
    // UNICODE���ַ���Ŀ
    WCHAR wchar[128]; // UNICODE��������
    // �ߵ��ֽڶԵ���ƴ��UNICODE
    for(int i=0; i<nSrcLength/2; i++) {
        // �ȸ�λ�ֽ�
        wchar[i] = *pSrc++ << 8;
        // ���λ�ֽ�
        wchar[i] |= *pSrc++;
    }
    // UNICODE��-->�ַ���
    nDstLength = ::WideCharToMultiByte(CP_ACP, 0, wchar, nSrcLength/2, pDst, 160,
        NULL, NULL);
    // ����ַ����Ӹ�������
    pDst[nDstLength] = '\0';
    // ����Ŀ���ַ�������
    return nDstLength;
}

// ����˳����ַ���ת��Ϊ�����ߵ����ַ�����������Ϊ��������'F'�ճ�ż��    
// �磺"8613851872468" --> "683158812764F8"    
// ����: pSrc - Դ�ַ���ָ��    
//       nSrcLength - Դ�ַ�������    
// ���: pDst - Ŀ���ַ���ָ��    
// ����: Ŀ���ַ�������        	
int SMSWarp::gsmInvertNumbers(const char* pSrc, char* pDst, int nSrcLength) 
{
    int nDstLength; // Ŀ���ַ�������
    char ch; // ���ڱ���һ���ַ�
    // ���ƴ�����
    nDstLength = nSrcLength;
    // �����ߵ�
    for(int i=0; i<nSrcLength;i+=2) {
        ch = *pSrc++; // �����ȳ��ֵ��ַ�
        *pDst++ = *pSrc++; // ���ƺ���ֵ��ַ�
        *pDst++ = ch; // �����ȳ��ֵ��ַ�
    }
    // Դ��������������
    if(nSrcLength & 1) {
        *(pDst-2) = 'F'; // ��'F'
        nDstLength++; // Ŀ�괮���ȼ�1
    }
    *pDst = '\0'; // ����ַ����Ӹ�������
    return nDstLength; // ����Ŀ���ַ�������
}

// �����ߵ����ַ���ת��Ϊ����˳����ַ���
// �磺"683158812764F8" --> "8613851872468"   	
// ����: pSrc - Դ�ַ���ָ��    
//       nSrcLength - Դ�ַ�������    
// ���: pDst - Ŀ���ַ���ָ��    
// ����: Ŀ���ַ�������    
int SMSWarp::gsmSerializeNumbers(const char* pSrc, char* pDst, int nSrcLength) {
    int nDstLength; // Ŀ���ַ�������
    char ch; // ���ڱ���һ���ַ�
    // ���ƴ�����
    nDstLength = nSrcLength;
    // �����ߵ�
    for(int i=0; i<nSrcLength;i+=2) {
        ch = *pSrc++; // �����ȳ��ֵ��ַ�
        *pDst++ = *pSrc++; // ���ƺ���ֵ��ַ�
        *pDst++ = ch; // �����ȳ��ֵ��ַ�
    }
    // �����ַ���'F'��
    if(*(pDst-1) == 'F') {
        pDst--;
        nDstLength--; // Ŀ���ַ������ȼ�1
    }
    *pDst = '\0'; // ����ַ����Ӹ�������
    return nDstLength; // ����Ŀ���ַ�������
}

// PDU���룬���ڱ��ơ����Ͷ���Ϣ
// pSrc: ԴPDU����ָ��
// pDst: Ŀ��PDU��ָ��
// ����: Ŀ��PDU������
int SMSWarp::gsmEncodePdu(const SM_PARAM* pSrc, char* pDst) {
    int nLength; // �ڲ��õĴ�����
    int nDstLength; // Ŀ��PDU������
    unsigned char buf[256]; // �ڲ��õĻ�����
    // SMSC��ַ��Ϣ��
    nLength = strlen(pSrc->SCA); // SMSC��ַ�ַ����ĳ���
    buf[0] = (char)((nLength & 1) == 0 ? nLength : nLength + 1) / 2 + 1;// SMSC��ַ��Ϣ����
    buf[1] = 0x91; // �̶�: �ù��ʸ�ʽ����
    nDstLength = gsmBytes2String(buf, pDst, 2); // ת��2���ֽڵ�Ŀ��PDU��
    // ת��SMSC��Ŀ��PDU��
    nDstLength += gsmInvertNumbers(pSrc->SCA, &pDst[nDstLength], nLength);
    // TPDU�λ���������Ŀ���ַ��
    nLength = strlen(pSrc->TPA); // TP-DA��ַ�ַ����ĳ���
    buf[0] = 0x11; // �Ƿ��Ͷ���(TP-MTI=01)��TP-VP����Ը�ʽ(TP-VPF=10)
    buf[1] = 0; // TP-MR=0
    buf[2] = (char)nLength; // Ŀ���ַ���ָ���(TP-DA��ַ�ַ�����ʵ����)
    buf[3] = 0x91; // �̶�: �ù��ʸ�ʽ����
    nDstLength += gsmBytes2String(buf, &pDst[nDstLength], 4);//ת��4���ֽڵ�Ŀ��PDU��
    // ת��TP-DA��Ŀ��PDU��
    nDstLength += gsmInvertNumbers(pSrc->TPA, &pDst[nDstLength], nLength);
    // TPDU��Э���ʶ�����뷽ʽ���û���Ϣ��
    nLength = strlen(pSrc->TP_UD); // �û���Ϣ�ַ����ĳ���
    buf[0] = pSrc->TP_PID; // Э���ʶ(TP-PID)
    buf[1] = pSrc->TP_DCS; // �û���Ϣ���뷽ʽ(TP-DCS)
    buf[2] = 0; // ��Ч��(TP-VP)Ϊ5����
    if(pSrc->TP_DCS == GSM_7BIT) {
        // 7-bit���뷽ʽ
        buf[3] = nLength; // ����ǰ����
        // ת��TP-DA��Ŀ��PDU��
        nLength = gsmEncode7bit(pSrc->TP_UD, &buf[4], nLength+1) + 4;
    }
    else if (pSrc->TP_DCS == GSM_UCS2) {
        // UCS2���뷽ʽ
        // ת��TP-DA��Ŀ��PDU��
        buf[3] = gsmEncodeUcs2(pSrc->TP_UD, &buf[4], nLength);
        nLength = buf[3] + 4; // nLength���ڸö����ݳ���
    }
    else {
        // 8-bit���뷽ʽ
        // ת��TP-DA��Ŀ��PDU��
        buf[3] = gsmEncode8bit(pSrc->TP_UD, &buf[4], nLength);
        nLength = buf[3] + 4; // nLength���ڸö����ݳ���
    }
    // ת���ö����ݵ�Ŀ��PDU��
    nDstLength += gsmBytes2String(buf, &pDst[nDstLength], nLength);
    return nDstLength; // ����Ŀ���ַ�������
}

// PDU���룬���ڽ��ա��Ķ�����Ϣ
// pSrc: ԴPDU��ָ��
// pDst: Ŀ��PDU����ָ��
// ����: �û���Ϣ������
int SMSWarp::gsmDecodePdu(const char* pSrc, SM_PARAM* pDst) {
    int nDstLength; // Ŀ��PDU������
    unsigned char tmp; // �ڲ��õ���ʱ�ֽڱ���
    unsigned char buf[256]; // �ڲ��õĻ�����
    unsigned char udhi = 0;
    // SMSC��ַ��Ϣ��
    gsmString2Bytes(pSrc, &tmp, 2); // ȡ����
    tmp = (tmp - 1) * 2; // SMSC���봮����
    pSrc += 4; // ָ�����
    gsmSerializeNumbers(pSrc, pDst->SCA, tmp); // ת��SMSC���뵽Ŀ��PDU��
    pSrc += tmp; // ָ�����
    // TPDU�λ����������ظ���ַ��
    gsmString2Bytes(pSrc, &tmp, 2); // ȡ��������
    pSrc += 2; // ָ�����
    udhi = tmp & 0x40;//�û�����ͷ��ʶ
    //	 if(tmp & 0x80) 
    {
        // �����ظ���ַ��ȡ�ظ���ַ��Ϣ
        gsmString2Bytes(pSrc, &tmp, 2); // ȡ����
        if(tmp & 1) tmp += 1; // ������ż��
        pSrc += 4; // ָ�����
        gsmSerializeNumbers(pSrc, pDst->TPA, tmp); // ȡTP-RA����
        pSrc += tmp; // ָ�����
    }
    // TPDU��Э���ʶ�����뷽ʽ���û���Ϣ��
    gsmString2Bytes(pSrc, (unsigned char*)&pDst->TP_PID, 2); // ȡЭ���ʶ(TP-PID)
    pSrc += 2; // ָ�����
    gsmString2Bytes(pSrc, (unsigned char*)&pDst->TP_DCS, 2);
    // ȡ���뷽ʽ(TP-DCS)
    pSrc += 2; // ָ�����
    gsmSerializeNumbers(pSrc, pDst->TP_SCTS, 14); // ����ʱ����ַ���(TP_SCTS)
    pSrc += 14; // ָ�����
    gsmString2Bytes(pSrc, &tmp, 2); // �û���Ϣ����(TP-UDL)
    pSrc += 2; // ָ�����
    if (udhi == 0x40)
    {
        unsigned char hl;
        gsmString2Bytes(pSrc, &hl, 2);
        pSrc += 2;
        pSrc += hl * 2;
        tmp -= (hl + 1);
    }
    if(pDst->TP_DCS == GSM_7BIT) 
    {
        // 7-bit����
        nDstLength = gsmString2Bytes(pSrc, buf, tmp & 7 ? (int)tmp*7 / 4 + 2 : (int)tmp*7 / 4);
        // ��ʽת��
        gsmDecode7bit(buf, pDst->TP_UD, nDstLength); // ת����TP-DU
        nDstLength = tmp;
    }
    else if(pDst->TP_DCS == GSM_UCS2) 
    {
        // UCS2����
        nDstLength = gsmString2Bytes(pSrc, buf, tmp * 2);
        // ��ʽת��
        nDstLength = gsmDecodeUcs2(buf, pDst->TP_UD, nDstLength); // ת����TP-DU
    }
    else 
    {
        // 8-bit����
        nDstLength = gsmString2Bytes(pSrc, buf, tmp * 2);
        unsigned char* pBuf = &buf[0];
        //		 char udh[] = {0x05, 0x04, 0x0B, 0x84, 0x23, 0xF0};// ֪ͨ
        //		 if (memcmp(pBuf + 1, udh, 6) == 0)
        if ((pBuf[1] == 0x06) && (memcmp(pBuf + 3, "application/vnd.wap", strlen("application/vnd.wap")) == 0))
        {
            //pBuf += 1; //0x06
            //tmp = *pBuf; //head 1 ����
            //pBuf += 1;
            //pBuf += tmp;
            pBuf += 2; // Transaction ID ,PDU Type: Push 0x06
            tmp = *pBuf; //ͷ2����
            pBuf += 1;

            char* mms = "application/vnd.wap.mms-message";
            if (memcmp(pBuf, mms, strlen(mms)) == 0)
            {
                pBuf += tmp;

                nDstLength -= (3 + tmp);//Transaction ID ,PDU Type,ͷ2����,ͷ����

                pBuf += 2; //0x82 0x8C X-Mms-Message-Type: m-notification-ind 
                pBuf += 1; //0x98 Transaction-ID
                while (*pBuf != 0x00)
                {
                    pBuf += 1;
                }
                pBuf += 1; //0x00
                pBuf += 3; //0x8D 0x90  MMS-Version: 1.0  //0x83 Content-Location
                const unsigned char * content = (const unsigned char *)pBuf;
                int len = 0;
                while (*pBuf != 0x00)
                {
                    pBuf += 1;
                    len += 1;
                }
                gsmDecode8bit(content, pDst->TP_UD, len);
                pBuf += 1; //0x00
                pBuf += 3; //0x88 0x05 0x81  //Expiry + Length + Relative-token + Length + Delta-secs
                tmp = *pBuf;
                pBuf += 1;
                unsigned int n = 0;

                for (int i = 0; i < tmp; ++i)
                {
                    n = ((n << 8) | *pBuf);
                    pBuf += 1;
                }
                pDst->MMS_Expiry = n;

                pBuf += 1; //0x89 From + Length + Address-Present-token
                tmp = *pBuf;
                pBuf += 1;
                pBuf += 1; //0x80
                gsmDecode8bit((const unsigned char *)pBuf, pDst->TPA, tmp - 1);
                pBuf += (tmp - 1);
                pBuf += 2; //0x8A 0x82
                pBuf += 1; //0x8E
                tmp = *pBuf;
                pBuf += 1;

                n = 0;
                for (int i = 0; i < tmp; ++i)
                {
                    n = ((n << 8) | *pBuf);
                    pBuf += 1;
                }
                pDst->MMS_Size = n;
                pDst->Type = mtMMS;
            }
            else if ((tmp == 1) && (*pSrc == 0xAE)) //si
            {
                pDst->Type = mtSI;
            }
        }
        else //sms
        {
            unsigned int otaType = *((int*)pSrc);
            if ((otaType == mtOTAMenu) || (otaType == mtOTAdMediaFile) || (otaType == mtOTAInterface) || (otaType == mtOTAWeather))
            {
                gsmDecode8bit(buf + 4, pDst->TP_UD, nDstLength - 4);
                pDst->Type = otaType;
            }
            else
            {
                // ��ʽת��
                nDstLength = gsmDecode8bit(buf, pDst->TP_UD, nDstLength); // ת����TP-DU
                pDst->Type = mtSMS;
            }
        }
    }

    // ����Ŀ���ַ�������
    return nDstLength;
}

//// ��ʼ��GSM״̬    
//BOOL SMSWarp::gsmInit()   
//{   
//	char ans[128];      // Ӧ��    
//
//	// ����GSM-MODEM�Ĵ�����    
//	WriteComm("AT\r", 3);   
//	ReadComm(ans, 128);   
//	if (strstr(ans, "OK") == NULL)  return FALSE;   
//
//	// ECHO OFF    
//	WriteComm("ATE0\r", 5);   
//	ReadComm(ans, 128);   
//
//	// PDUģʽ    
//	WriteComm("AT+CMGF=0\r", 10);   
//	ReadComm(ans, 128);   
//
//	return TRUE;   
//}   

// ���Ͷ���Ϣ���������������ȡӦ��    
// ����: pSrc - ԴPDU����ָ��    
int SMSWarp::gsmSendMessage(SM_PARAM* pSrc)   
{   
    int nPduLength;     // PDU������    
    unsigned char nSmscLength;  // SMSC������    
    //int nLength;        // �����յ������ݳ���    
    //char cmd[16];       // ���    
    char pdu[512];      // PDU��    
    //char ans[128];      // Ӧ��    

    nPduLength = gsmEncodePdu(pSrc, pdu);   // ����PDU����������PDU��    
    strcat(pdu, "\x01a");       // ��Ctrl-Z����    

    gsmString2Bytes(pdu, &nSmscLength, 2);  // ȡPDU���е�SMSC��Ϣ����    
    nSmscLength++;      // ���ϳ����ֽڱ���    

    //// �����еĳ��ȣ�������SMSC��Ϣ���ȣ��������ֽڼ�    
    //sprintf(cmd, "AT+CMGS=%d\r", nPduLength / 2 - nSmscLength); // ��������    

    //  TRACE("%s", cmd);    
    //  TRACE("%s\n", pdu);    

    //WriteComm(cmd, strlen(cmd));    // ��������    

    //nLength = ReadComm(ans, 128);   // ��Ӧ������    

    //// �����ܷ��ҵ�"\r\n> "�����ɹ����    
    //if(nLength == 4 && strncmp(ans, "\r\n> ", 4) == 0)   
    //{   
    //	return WriteComm(pdu, strlen(pdu));     // �õ��϶��ش𣬼������PDU��    
    //}   
    if (m_pAT->SmsSend(nPduLength / 2 - nSmscLength))
    {
        m_pAT->SmsSend(pdu, strlen(pdu));
    }

    return 0;   
}   

// ��ȡ����Ϣ���������������ȡӦ��    
// ��+CMGL����+CMGR����һ���Զ���ȫ������Ϣ    
int SMSWarp::gsmReadMessageList()   
{   
    //	return WriteComm("AT+CMGL=0\r", 10);   
    return m_pAT->SmsReadList();
}   

// ɾ������Ϣ���������������ȡӦ��    
// ����: index - ����Ϣ��ţ�1-255    
int SMSWarp::gsmDeleteMessage(int index)   
{   
    //char cmd[16];       // ���    
    //sprintf(cmd, "AT+CMGD=%d\r", index);    // ��������    
    //// ������    
    //return WriteComm(cmd, strlen(cmd)); 
    return m_pAT->SmsDelete(index);
}   

// ��ȡGSM MODEM��Ӧ�𣬿�����һ����    
// ���: pBuff - ����Ӧ�𻺳���    
// ����: GSM MODEM��Ӧ��״̬, GSM_WAIT/GSM_OK/GSM_ERR    
// ��ע: ������Ҫ��ε��ò�����ɶ�ȡһ��Ӧ���״ε���ʱӦ��pBuff��ʼ��    
int SMSWarp::gsmGetResponse(SM_BUFF* pBuff)   
{   
    int nLength;        // �����յ������ݳ���    
    int nState;   

    // �Ӵ��ڶ����ݣ�׷�ӵ�������β��    
    //	nLength = ReadComm(&pBuff->data[pBuff->len], 128);       
    nLength = m_pAT->SmsReadResponse(&pBuff->data[pBuff->len], 128);
    pBuff->len += nLength;   

    // ȷ��GSM MODEM��Ӧ��״̬    
    nState = GSM_WAIT;   
    if ((nLength > 0) && (pBuff->len >= 4))   
    {   
        if (strncmp(&pBuff->data[pBuff->len - 4], "OK\r\n", 4) == 0)  nState = GSM_OK;   
        else if (strstr(pBuff->data, "+CMS ERROR") != NULL) nState = GSM_ERR;   
    }   

    return nState;   
}   

// ���б��н�����ȫ������Ϣ    
// ����: pBuff - ����Ϣ�б�����    
// ���: pMsg - ����Ϣ������    
// ����: ����Ϣ����    
int SMSWarp::gsmParseMessageList(SM_PARAM* pMsg, SM_BUFF* pBuff)   
{   
    int nMsg;           // ����Ϣ����ֵ    
    char* ptr;          // �ڲ��õ�����ָ��    

    nMsg = 0;   
    ptr = pBuff->data;   

    // ѭ����ȡÿһ������Ϣ, ��"+CMGL:"��ͷ    
    while((ptr = strstr(ptr, "+CMGL:")) != NULL)   
    {   
        ptr += 6;       // ����"+CMGL:", ��λ�����    
        sscanf(ptr, "%d", &pMsg->index); // ��ȡ���    
        //      TRACE("  index=%d\n",pMsg->index);    

        ptr = strstr(ptr, "\r\n");  // ����һ��    
        if (ptr != NULL)   
        {   
            ptr += 2;       // ����"\r\n", ��λ��PDU    

            gsmDecodePdu(ptr, pMsg);    // PDU������    

            pMsg++;     // ׼������һ������Ϣ    
            nMsg++;     // ����Ϣ������1    
        }   
    }   

    return nMsg;   
}   

// ��һ������Ϣ���뷢�Ͷ���    
void SMSWarp::PutSendMessage(SM_PARAM* pparam)   
{   
    EnterCriticalSection(&m_csSend);   

    memcpy(&m_SmSend[m_nSendIn], pparam, sizeof(SM_PARAM));   

    m_nSendIn++;   
    if (m_nSendIn >= MAX_SM_SEND)  m_nSendIn = 0;   

    LeaveCriticalSection(&m_csSend);   
}   

// �ӷ��Ͷ�����ȡһ������Ϣ    
BOOL SMSWarp::GetSendMessage(SM_PARAM* pparam)   
{   
    BOOL fSuccess = FALSE;   

    EnterCriticalSection(&m_csSend);   

    if (m_nSendOut != m_nSendIn)   
    {   
        memcpy(pparam, &m_SmSend[m_nSendOut], sizeof(SM_PARAM));   

        m_nSendOut++;   
        if (m_nSendOut >= MAX_SM_SEND)  m_nSendOut = 0;   

        fSuccess = TRUE;   
    }   

    LeaveCriticalSection(&m_csSend);   

    return fSuccess;   
}   

// ������Ϣ������ն���    
void SMSWarp::PutRecvMessage(SM_PARAM* pparam, int nCount)   
{   
    EnterCriticalSection(&m_csRecv);   

    for (int i = 0; i < nCount; i++)   
    {   
        memcpy(&m_SmRecv[m_nRecvIn], pparam, sizeof(SM_PARAM));   

        m_nRecvIn++;   
        if (m_nRecvIn >= MAX_SM_RECV)  m_nRecvIn = 0;   

        pparam++;   
    }   

    LeaveCriticalSection(&m_csRecv);   
}   

// �ӽ��ն�����ȡһ������Ϣ    
BOOL SMSWarp::GetRecvMessage(SM_PARAM* pparam)   
{   
    BOOL fSuccess = FALSE;   

    EnterCriticalSection(&m_csRecv);   

    if (m_nRecvOut != m_nRecvIn)   
    {   
        memcpy(pparam, &m_SmRecv[m_nRecvOut], sizeof(SM_PARAM));   

        m_nRecvOut++;   
        if (m_nRecvOut >= MAX_SM_RECV)  m_nRecvOut = 0;   

        fSuccess = TRUE;   
    }   

    LeaveCriticalSection(&m_csRecv);   

    return fSuccess;   
}   

UINT SMSWarp::SmThread(LPVOID lParam)   
{   
    SMSWarp* p=(SMSWarp *)lParam;   // this    
    int nMsg;               // �յ�����Ϣ����    
    int nDelete;            // Ŀǰ����ɾ���Ķ���Ϣ���    
    SM_BUFF buff;           // ���ն���Ϣ�б�Ļ�����    
    SM_PARAM param[256];    // ����/���ն���Ϣ������    
    CTime tmOrg, tmNow;     // �ϴκ����ڵ�ʱ�䣬���㳬ʱ��    
    enum {   
        stBeginRest,                // ��ʼ��Ϣ/��ʱ    
        stContinueRest,             // ������Ϣ/��ʱ    
        stSendMessageRequest,       // ���Ͷ���Ϣ    
        stSendMessageResponse,      // ��ȡ����Ӧ�𵽻�����    
        stSendMessageWaitIdle,      // ���Ͳ��ɹ����ȴ�GSM����    
        stReadMessageRequest,       // ���Ͷ�ȡ����Ϣ�б������    
        stReadMessageResponse,      // ��ȡ����Ϣ�б�������    
        stDeleteMessageRequest,     // ɾ������Ϣ    
        stDeleteMessageResponse,    // ɾ������Ϣ    
        stDeleteMessageWaitIdle,    // ɾ�����ɹ����ȴ�GSM����    
        stExitThread                // �˳�    
    } nState;               // ������̵�״̬    

    // ��ʼ״̬    
    nState = stBeginRest;   

    // ���ͺͽ��մ����״̬ѭ��    
    while (nState != stExitThread)   
    {   
        switch(nState)   
        {   
        case stBeginRest:   
            //              TRACE("State=stBeginRest\n");    
            tmOrg = CTime::GetCurrentTime();   
            nState = stContinueRest;   
            break;   

        case stContinueRest:   
            //              TRACE("State=stContinueRest\n");    
            Sleep(300);   
            tmNow = CTime::GetCurrentTime();   
            if (p->GetSendMessage(param))   
            {   
                nState = stSendMessageRequest;  // �д�������Ϣ���Ͳ���Ϣ��    
            }   
            else if (tmNow - tmOrg >= 5)     // ��������Ϣ���пգ���Ϣ5��    
            {   
                nState = stReadMessageRequest;  // ת����ȡ����Ϣ״̬    
            }   
            break;   

        case stSendMessageRequest:   
            //              TRACE("State=stSendMessageRequest\n");    
            p->gsmSendMessage(param);   
            memset(&buff, 0, sizeof(buff));   
            tmOrg = CTime::GetCurrentTime();   
            nState = stSendMessageResponse;   
            break;   

        case stSendMessageResponse:   
            //              TRACE("State=stSendMessageResponse\n");    
            Sleep(100);   
            tmNow = CTime::GetCurrentTime();   
            switch (p->gsmGetResponse(&buff))   
            {   
            case GSM_OK:    
                //                      TRACE("  GSM_OK %d\n", tmNow - tmOrg);    //��Ҫ֪ͨAPP����ʽ����
                nState = stBeginRest;   
                break;   
            case GSM_ERR:   
                //                      TRACE("  GSM_ERR %d\n", tmNow - tmOrg);    
                nState = stSendMessageWaitIdle;   
                break;   
            default:   
                //                      TRACE("  GSM_WAIT %d\n", tmNow - tmOrg);    
                if (tmNow - tmOrg >= 10)     // 10�볬ʱ    
                {   
                    //                          TRACE("  Timeout!\n");    
                    nState = stSendMessageWaitIdle;   
                }   
            }   
            break;   
        case stSendMessageWaitIdle:   
            Sleep(500);   
            nState = stSendMessageRequest;      // ֱ�����ͳɹ�Ϊֹ    
            break;   

        case stReadMessageRequest:   
            //              TRACE("State=stReadMessageRequest\n");    
            p->gsmReadMessageList();   
            memset(&buff, 0, sizeof(buff));   
            tmOrg = CTime::GetCurrentTime();   
            nState = stReadMessageResponse;   
            break;   

        case stReadMessageResponse:   
            //              TRACE("State=stReadMessageResponse\n");    
            Sleep(100);   
            tmNow = CTime::GetCurrentTime();   
            switch (p->gsmGetResponse(&buff))   
            {   
            case GSM_OK:    
                //                      TRACE("  GSM_OK %d\n", tmNow - tmOrg);    
                nMsg = p->gsmParseMessageList(param, &buff);   
                if (nMsg > 0)   
                {   
                    p->PutRecvMessage(param, nMsg);   
                    nDelete = 0;   
                    nState = stDeleteMessageRequest;   
                }   
                else   
                {   
                    nState = stBeginRest;   
                }   
                break;   
            case GSM_ERR:   
                //                      TRACE("  GSM_ERR %d\n", tmNow - tmOrg);    
                nState = stBeginRest;   
                break;   
            default:   
                //                      TRACE("  GSM_WAIT %d\n", tmNow - tmOrg);    
                if (tmNow - tmOrg >= 15)     // 15�볬ʱ    
                {   
                    //                          TRACE("  Timeout!\n");    
                    nState = stBeginRest;   
                }   
            }   
            break;   

        case stDeleteMessageRequest:   
            //              TRACE("State=stDeleteMessageRequest\n");    
            if (nDelete < nMsg)   
            {   
                p->gsmDeleteMessage(param[nDelete].index);   
                memset(&buff, 0, sizeof(buff));   
                tmOrg = CTime::GetCurrentTime();   
                nState = stDeleteMessageResponse;   
            }   
            else   
            {   
                nState = stBeginRest;   
            }   
            break;   

        case stDeleteMessageResponse:   
            //              TRACE("State=stDeleteMessageResponse\n");    
            Sleep(100);   
            tmNow = CTime::GetCurrentTime();   
            switch (p->gsmGetResponse(&buff))   
            {   
            case GSM_OK:    
                //                      TRACE("  GSM_OK %d\n", tmNow - tmOrg);    
                nDelete++;   
                nState = stDeleteMessageRequest;   
                break;   
            case GSM_ERR:   
                //                      TRACE("  GSM_ERR %d\n", tmNow - tmOrg);    
                nState = stDeleteMessageWaitIdle;   
                break;   
            default:   
                //                      TRACE("  GSM_WAIT %d\n", tmNow - tmOrg);    
                if (tmNow - tmOrg >= 5)      // 5�볬ʱ    
                {   
                    //                          TRACE("  Timeout!\n");    
                    nState = stBeginRest;   
                }   
            }   
            break;   

        case stDeleteMessageWaitIdle:   
            //              TRACE("State=stDeleteMessageWaitIdle\n");    
            Sleep(500);   
            nState = stDeleteMessageRequest;        // ֱ��ɾ���ɹ�Ϊֹ    
            break;   
        }   

        // ����Ƿ��йرձ��̵߳��ź�    
        DWORD dwEvent = WaitForSingleObject(p->m_hKillThreadEvent, 20);   
        if (dwEvent == WAIT_OBJECT_0)  nState = stExitThread;   
    }   

    // �ø��߳̽�����־    
    SetEvent(p->m_hThreadKilledEvent);   

    return 0;   
}   
