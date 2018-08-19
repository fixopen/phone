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
    SetEvent(m_hKillThreadEvent);           // 发出关闭子线程的信号
    WaitForSingleObject(m_hThreadKilledEvent, INFINITE);    // 等待子线程关闭

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
    // 启动子线程
    AfxBeginThread(SmThread, this, THREAD_PRIORITY_NORMAL);
}

// 可打印字符串转换为字节数据    
// 如："C8329BFD0E01" --> {0xC8, 0x32, 0x9B, 0xFD, 0x0E, 0x01}    
// 输入: pSrc - 源字符串指针    
//       nSrcLength - 源字符串长度    
// 输出: pDst - 目标数据指针    
// 返回: 目标数据长度    
int SMSWarp::gsmString2Bytes(const char* pSrc, unsigned char* pDst, int nSrcLength)
{
    for (int i = 0; i < nSrcLength; i += 2)
    {
        // 输出高4位
        if ((*pSrc >= '0') && (*pSrc <= '9'))
        {
            *pDst = (*pSrc - '0') << 4;
        }
        else
        {
            *pDst = (*pSrc - 'A' + 10) << 4;
        }

        pSrc++;

        // 输出低4位
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

    // 返回目标数据长度
    return (nSrcLength / 2);
}   

// 字节数据转换为可打印字符串    
// 如：{0xC8, 0x32, 0x9B, 0xFD, 0x0E, 0x01} --> "C8329BFD0E01"     
// 输入: pSrc - 源数据指针    
//       nSrcLength - 源数据长度    
// 输出: pDst - 目标字符串指针    
// 返回: 目标字符串长度    
int SMSWarp::gsmBytes2String(const unsigned char* pSrc, char* pDst, int nSrcLength)
{
    const char tab[]="0123456789ABCDEF";    // 0x0-0xf的字符查找表

    for (int i = 0; i < nSrcLength; i++)
    {
        *pDst++ = tab[*pSrc >> 4];        // 输出高4位
        *pDst++ = tab[*pSrc & 0x0f];    // 输出低4位
        pSrc++;
    }

    // 输出字符串加个结束符
    *pDst = '\0';

    // 返回目标字符串长度
    return (nSrcLength * 2);
}   

// 7bit编码    
// 输入: pSrc - 源字符串指针    
//       nSrcLength - 源字符串长度    
// 输出: pDst - 目标编码串指针    
// 返回: 目标编码串长度    
int SMSWarp::gsmEncode7bit(const char* pSrc, unsigned char* pDst, int nSrcLength)
{
    int nSrc;       // 源字符串的计数值
    int nDst;       // 目标编码串的计数值
    int nChar;      // 当前正在处理的组内字符字节的序号，范围是0-7
    unsigned char nLeft;    // 上一字节残余的数据

    // 计数值初始化
    nSrc = 0;
    nDst = 0;

    // 将源串每8个字节分为一组，压缩成7个字节
    // 循环该处理过程，直至源串被处理完
    // 如果分组不到8字节，也能正确处理
    while (nSrc < nSrcLength)
    {
        // 取源字符串的计数值的最低3位
        nChar = nSrc & 7;

        // 处理源串的每个字节
        if (nChar == 0)
        {
            // 组内第一个字节，只是保存起来，待处理下一个字节时使用
            nLeft = *pSrc;
        }
        else
        {
            // 组内其它字节，将其右边部分与残余数据相加，得到一个目标编码字节
            *pDst = (*pSrc << (8-nChar)) | nLeft;

            // 将该字节剩下的左边部分，作为残余数据保存起来
            nLeft = *pSrc >> nChar;

            // 修改目标串的指针和计数值
            pDst++;
            nDst++;
        }

        // 修改源串的指针和计数值
        pSrc++;
        nSrc++;
    }   

    // 返回目标串长度
    return nDst;
}

// 7bit解码    
// 输入: pSrc - 源编码串指针    
//       nSrcLength - 源编码串长度    
// 输出: pDst - 目标字符串指针    
// 返回: 目标字符串长度    
int SMSWarp::gsmDecode7bit(const unsigned char* pSrc, char* pDst, int nSrcLength) {
    int nSrc; // 源字符串的计数值
    int nDst; // 目标解码串的计数值
    int nByte; // 当前正在处理的组内字节的序号，范围是0-6
    unsigned char nLeft; // 上一字节残余的数据
    // 计数值初始化
    nSrc = 0;
    nDst = 0;
    // 组内字节序号和残余数据初始化
    nByte = 0;
    nLeft = 0;
    // 将源数据每7个字节分为一组，解压缩成8个字节
    // 循环该处理过程，直至源数据被处理完
    // 如果分组不到7字节，也能正确处理
    while(nSrc<nSrcLength) {
        // 将源字节右边部分与残余数据相加，去掉最高位，得到一个目标解码字节
        *pDst = ((*pSrc << nByte) | nLeft) & 0x7f;
        // 将该字节剩下的左边部分，作为残余数据保存起来
        nLeft = *pSrc >> (7-nByte);
        // 修改目标串的指针和计数值
        pDst++;
        nDst++;
        // 修改字节计数值
        nByte++;
        // 到了一组的最后一个字节
        if(nByte == 7) {
            // 额外得到一个目标解码字节
            *pDst = nLeft;
            // 修改目标串的指针和计数值
            pDst++;
            nDst++;
            // 组内字节序号和残余数据初始化
            nByte = 0;
            nLeft = 0;
        }
        // 修改源串的指针和计数值
        pSrc++;
        nSrc++;
    }
    *pDst = 0;
    // 返回目标串长度
    return nDst;
}

// 8bit编码    
// 输入: pSrc - 源字符串指针    
//       nSrcLength - 源字符串长度    
// 输出: pDst - 目标编码串指针    
// 返回: 目标编码串长度    
int SMSWarp::gsmEncode8bit(const char* pSrc, unsigned char* pDst, int nSrcLength)   
{   
    // 简单复制    
    memcpy(pDst, pSrc, nSrcLength);   

    return nSrcLength;   
}   

// 8bit解码    
// 输入: pSrc - 源编码串指针    
//       nSrcLength -  源编码串长度    
// 输出: pDst -  目标字符串指针    
// 返回: 目标字符串长度    
int SMSWarp::gsmDecode8bit(const unsigned char* pSrc, char* pDst, int nSrcLength)   
{   
    // 简单复制    
    memcpy(pDst, pSrc, nSrcLength);   

    // 输出字符串加个结束符    
    pDst[nSrcLength] = '\0';   

    return nSrcLength;   
}   

// UCS2编码    
// 输入: pSrc - 源字符串指针    
//       nSrcLength - 源字符串长度    
// 输出: pDst - 目标编码串指针    
// 返回: 目标编码串长度    
int SMSWarp::gsmEncodeUcs2(const char* pSrc, unsigned char* pDst, int nSrcLength) {
    int nDstLength; // UNICODE宽字符数目
    WCHAR wchar[128]; // UNICODE串缓冲区
    // 字符串-->UNICODE串
    nDstLength = ::MultiByteToWideChar(CP_ACP, 0, pSrc, nSrcLength, wchar, 128);
    // 高低字节对调，输出
    for(int i=0; i<nDstLength; i++) {
        // 先输出高位字节
        *pDst++ = wchar[i] >> 8;
        // 后输出低位字节
        *pDst++ = wchar[i] & 0xff;
    }
    // 返回目标编码串长度
    return nDstLength * 2;
}

// UCS2解码    
// 输入: pSrc - 源编码串指针    
//       nSrcLength -  源编码串长度    
// 输出: pDst -  目标字符串指针    
// 返回: 目标字符串长度    
int SMSWarp::gsmDecodeUcs2(const unsigned char* pSrc, char* pDst, int nSrcLength) {
    int nDstLength;
    // UNICODE宽字符数目
    WCHAR wchar[128]; // UNICODE串缓冲区
    // 高低字节对调，拼成UNICODE
    for(int i=0; i<nSrcLength/2; i++) {
        // 先高位字节
        wchar[i] = *pSrc++ << 8;
        // 后低位字节
        wchar[i] |= *pSrc++;
    }
    // UNICODE串-->字符串
    nDstLength = ::WideCharToMultiByte(CP_ACP, 0, wchar, nSrcLength/2, pDst, 160,
        NULL, NULL);
    // 输出字符串加个结束符
    pDst[nDstLength] = '\0';
    // 返回目标字符串长度
    return nDstLength;
}

// 正常顺序的字符串转换为两两颠倒的字符串，若长度为奇数，补'F'凑成偶数    
// 如："8613851872468" --> "683158812764F8"    
// 输入: pSrc - 源字符串指针    
//       nSrcLength - 源字符串长度    
// 输出: pDst - 目标字符串指针    
// 返回: 目标字符串长度        	
int SMSWarp::gsmInvertNumbers(const char* pSrc, char* pDst, int nSrcLength) 
{
    int nDstLength; // 目标字符串长度
    char ch; // 用于保存一个字符
    // 复制串长度
    nDstLength = nSrcLength;
    // 两两颠倒
    for(int i=0; i<nSrcLength;i+=2) {
        ch = *pSrc++; // 保存先出现的字符
        *pDst++ = *pSrc++; // 复制后出现的字符
        *pDst++ = ch; // 复制先出现的字符
    }
    // 源串长度是奇数吗？
    if(nSrcLength & 1) {
        *(pDst-2) = 'F'; // 补'F'
        nDstLength++; // 目标串长度加1
    }
    *pDst = '\0'; // 输出字符串加个结束符
    return nDstLength; // 返回目标字符串长度
}

// 两两颠倒的字符串转换为正常顺序的字符串
// 如："683158812764F8" --> "8613851872468"   	
// 输入: pSrc - 源字符串指针    
//       nSrcLength - 源字符串长度    
// 输出: pDst - 目标字符串指针    
// 返回: 目标字符串长度    
int SMSWarp::gsmSerializeNumbers(const char* pSrc, char* pDst, int nSrcLength) {
    int nDstLength; // 目标字符串长度
    char ch; // 用于保存一个字符
    // 复制串长度
    nDstLength = nSrcLength;
    // 两两颠倒
    for(int i=0; i<nSrcLength;i+=2) {
        ch = *pSrc++; // 保存先出现的字符
        *pDst++ = *pSrc++; // 复制后出现的字符
        *pDst++ = ch; // 复制先出现的字符
    }
    // 最后的字符是'F'吗？
    if(*(pDst-1) == 'F') {
        pDst--;
        nDstLength--; // 目标字符串长度减1
    }
    *pDst = '\0'; // 输出字符串加个结束符
    return nDstLength; // 返回目标字符串长度
}

// PDU编码，用于编制、发送短消息
// pSrc: 源PDU参数指针
// pDst: 目标PDU串指针
// 返回: 目标PDU串长度
int SMSWarp::gsmEncodePdu(const SM_PARAM* pSrc, char* pDst) {
    int nLength; // 内部用的串长度
    int nDstLength; // 目标PDU串长度
    unsigned char buf[256]; // 内部用的缓冲区
    // SMSC地址信息段
    nLength = strlen(pSrc->SCA); // SMSC地址字符串的长度
    buf[0] = (char)((nLength & 1) == 0 ? nLength : nLength + 1) / 2 + 1;// SMSC地址信息长度
    buf[1] = 0x91; // 固定: 用国际格式号码
    nDstLength = gsmBytes2String(buf, pDst, 2); // 转换2个字节到目标PDU串
    // 转换SMSC到目标PDU串
    nDstLength += gsmInvertNumbers(pSrc->SCA, &pDst[nDstLength], nLength);
    // TPDU段基本参数、目标地址等
    nLength = strlen(pSrc->TPA); // TP-DA地址字符串的长度
    buf[0] = 0x11; // 是发送短信(TP-MTI=01)，TP-VP用相对格式(TP-VPF=10)
    buf[1] = 0; // TP-MR=0
    buf[2] = (char)nLength; // 目标地址数字个数(TP-DA地址字符串真实长度)
    buf[3] = 0x91; // 固定: 用国际格式号码
    nDstLength += gsmBytes2String(buf, &pDst[nDstLength], 4);//转换4个字节到目标PDU串
    // 转换TP-DA到目标PDU串
    nDstLength += gsmInvertNumbers(pSrc->TPA, &pDst[nDstLength], nLength);
    // TPDU段协议标识、编码方式、用户信息等
    nLength = strlen(pSrc->TP_UD); // 用户信息字符串的长度
    buf[0] = pSrc->TP_PID; // 协议标识(TP-PID)
    buf[1] = pSrc->TP_DCS; // 用户信息编码方式(TP-DCS)
    buf[2] = 0; // 有效期(TP-VP)为5分钟
    if(pSrc->TP_DCS == GSM_7BIT) {
        // 7-bit编码方式
        buf[3] = nLength; // 编码前长度
        // 转换TP-DA到目标PDU串
        nLength = gsmEncode7bit(pSrc->TP_UD, &buf[4], nLength+1) + 4;
    }
    else if (pSrc->TP_DCS == GSM_UCS2) {
        // UCS2编码方式
        // 转换TP-DA到目标PDU串
        buf[3] = gsmEncodeUcs2(pSrc->TP_UD, &buf[4], nLength);
        nLength = buf[3] + 4; // nLength等于该段数据长度
    }
    else {
        // 8-bit编码方式
        // 转换TP-DA到目标PDU串
        buf[3] = gsmEncode8bit(pSrc->TP_UD, &buf[4], nLength);
        nLength = buf[3] + 4; // nLength等于该段数据长度
    }
    // 转换该段数据到目标PDU串
    nDstLength += gsmBytes2String(buf, &pDst[nDstLength], nLength);
    return nDstLength; // 返回目标字符串长度
}

// PDU解码，用于接收、阅读短消息
// pSrc: 源PDU串指针
// pDst: 目标PDU参数指针
// 返回: 用户信息串长度
int SMSWarp::gsmDecodePdu(const char* pSrc, SM_PARAM* pDst) {
    int nDstLength; // 目标PDU串长度
    unsigned char tmp; // 内部用的临时字节变量
    unsigned char buf[256]; // 内部用的缓冲区
    unsigned char udhi = 0;
    // SMSC地址信息段
    gsmString2Bytes(pSrc, &tmp, 2); // 取长度
    tmp = (tmp - 1) * 2; // SMSC号码串长度
    pSrc += 4; // 指针后移
    gsmSerializeNumbers(pSrc, pDst->SCA, tmp); // 转换SMSC号码到目标PDU串
    pSrc += tmp; // 指针后移
    // TPDU段基本参数、回复地址等
    gsmString2Bytes(pSrc, &tmp, 2); // 取基本参数
    pSrc += 2; // 指针后移
    udhi = tmp & 0x40;//用户数据头标识
    //	 if(tmp & 0x80) 
    {
        // 包含回复地址，取回复地址信息
        gsmString2Bytes(pSrc, &tmp, 2); // 取长度
        if(tmp & 1) tmp += 1; // 调整奇偶性
        pSrc += 4; // 指针后移
        gsmSerializeNumbers(pSrc, pDst->TPA, tmp); // 取TP-RA号码
        pSrc += tmp; // 指针后移
    }
    // TPDU段协议标识、编码方式、用户信息等
    gsmString2Bytes(pSrc, (unsigned char*)&pDst->TP_PID, 2); // 取协议标识(TP-PID)
    pSrc += 2; // 指针后移
    gsmString2Bytes(pSrc, (unsigned char*)&pDst->TP_DCS, 2);
    // 取编码方式(TP-DCS)
    pSrc += 2; // 指针后移
    gsmSerializeNumbers(pSrc, pDst->TP_SCTS, 14); // 服务时间戳字符串(TP_SCTS)
    pSrc += 14; // 指针后移
    gsmString2Bytes(pSrc, &tmp, 2); // 用户信息长度(TP-UDL)
    pSrc += 2; // 指针后移
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
        // 7-bit解码
        nDstLength = gsmString2Bytes(pSrc, buf, tmp & 7 ? (int)tmp*7 / 4 + 2 : (int)tmp*7 / 4);
        // 格式转换
        gsmDecode7bit(buf, pDst->TP_UD, nDstLength); // 转换到TP-DU
        nDstLength = tmp;
    }
    else if(pDst->TP_DCS == GSM_UCS2) 
    {
        // UCS2解码
        nDstLength = gsmString2Bytes(pSrc, buf, tmp * 2);
        // 格式转换
        nDstLength = gsmDecodeUcs2(buf, pDst->TP_UD, nDstLength); // 转换到TP-DU
    }
    else 
    {
        // 8-bit解码
        nDstLength = gsmString2Bytes(pSrc, buf, tmp * 2);
        unsigned char* pBuf = &buf[0];
        //		 char udh[] = {0x05, 0x04, 0x0B, 0x84, 0x23, 0xF0};// 通知
        //		 if (memcmp(pBuf + 1, udh, 6) == 0)
        if ((pBuf[1] == 0x06) && (memcmp(pBuf + 3, "application/vnd.wap", strlen("application/vnd.wap")) == 0))
        {
            //pBuf += 1; //0x06
            //tmp = *pBuf; //head 1 长度
            //pBuf += 1;
            //pBuf += tmp;
            pBuf += 2; // Transaction ID ,PDU Type: Push 0x06
            tmp = *pBuf; //头2长度
            pBuf += 1;

            char* mms = "application/vnd.wap.mms-message";
            if (memcmp(pBuf, mms, strlen(mms)) == 0)
            {
                pBuf += tmp;

                nDstLength -= (3 + tmp);//Transaction ID ,PDU Type,头2长度,头内容

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
                // 格式转换
                nDstLength = gsmDecode8bit(buf, pDst->TP_UD, nDstLength); // 转换到TP-DU
                pDst->Type = mtSMS;
            }
        }
    }

    // 返回目标字符串长度
    return nDstLength;
}

//// 初始化GSM状态    
//BOOL SMSWarp::gsmInit()   
//{   
//	char ans[128];      // 应答串    
//
//	// 测试GSM-MODEM的存在性    
//	WriteComm("AT\r", 3);   
//	ReadComm(ans, 128);   
//	if (strstr(ans, "OK") == NULL)  return FALSE;   
//
//	// ECHO OFF    
//	WriteComm("ATE0\r", 5);   
//	ReadComm(ans, 128);   
//
//	// PDU模式    
//	WriteComm("AT+CMGF=0\r", 10);   
//	ReadComm(ans, 128);   
//
//	return TRUE;   
//}   

// 发送短消息，仅发送命令，不读取应答    
// 输入: pSrc - 源PDU参数指针    
int SMSWarp::gsmSendMessage(SM_PARAM* pSrc)   
{   
    int nPduLength;     // PDU串长度    
    unsigned char nSmscLength;  // SMSC串长度    
    //int nLength;        // 串口收到的数据长度    
    //char cmd[16];       // 命令串    
    char pdu[512];      // PDU串    
    //char ans[128];      // 应答串    

    nPduLength = gsmEncodePdu(pSrc, pdu);   // 根据PDU参数，编码PDU串    
    strcat(pdu, "\x01a");       // 以Ctrl-Z结束    

    gsmString2Bytes(pdu, &nSmscLength, 2);  // 取PDU串中的SMSC信息长度    
    nSmscLength++;      // 加上长度字节本身    

    //// 命令中的长度，不包括SMSC信息长度，以数据字节计    
    //sprintf(cmd, "AT+CMGS=%d\r", nPduLength / 2 - nSmscLength); // 生成命令    

    //  TRACE("%s", cmd);    
    //  TRACE("%s\n", pdu);    

    //WriteComm(cmd, strlen(cmd));    // 先输出命令串    

    //nLength = ReadComm(ans, 128);   // 读应答数据    

    //// 根据能否找到"\r\n> "决定成功与否    
    //if(nLength == 4 && strncmp(ans, "\r\n> ", 4) == 0)   
    //{   
    //	return WriteComm(pdu, strlen(pdu));     // 得到肯定回答，继续输出PDU串    
    //}   
    if (m_pAT->SmsSend(nPduLength / 2 - nSmscLength))
    {
        m_pAT->SmsSend(pdu, strlen(pdu));
    }

    return 0;   
}   

// 读取短消息，仅发送命令，不读取应答    
// 用+CMGL代替+CMGR，可一次性读出全部短消息    
int SMSWarp::gsmReadMessageList()   
{   
    //	return WriteComm("AT+CMGL=0\r", 10);   
    return m_pAT->SmsReadList();
}   

// 删除短消息，仅发送命令，不读取应答    
// 输入: index - 短消息序号，1-255    
int SMSWarp::gsmDeleteMessage(int index)   
{   
    //char cmd[16];       // 命令串    
    //sprintf(cmd, "AT+CMGD=%d\r", index);    // 生成命令    
    //// 输出命令串    
    //return WriteComm(cmd, strlen(cmd)); 
    return m_pAT->SmsDelete(index);
}   

// 读取GSM MODEM的应答，可能是一部分    
// 输出: pBuff - 接收应答缓冲区    
// 返回: GSM MODEM的应答状态, GSM_WAIT/GSM_OK/GSM_ERR    
// 备注: 可能需要多次调用才能完成读取一次应答，首次调用时应将pBuff初始化    
int SMSWarp::gsmGetResponse(SM_BUFF* pBuff)   
{   
    int nLength;        // 串口收到的数据长度    
    int nState;   

    // 从串口读数据，追加到缓冲区尾部    
    //	nLength = ReadComm(&pBuff->data[pBuff->len], 128);       
    nLength = m_pAT->SmsReadResponse(&pBuff->data[pBuff->len], 128);
    pBuff->len += nLength;   

    // 确定GSM MODEM的应答状态    
    nState = GSM_WAIT;   
    if ((nLength > 0) && (pBuff->len >= 4))   
    {   
        if (strncmp(&pBuff->data[pBuff->len - 4], "OK\r\n", 4) == 0)  nState = GSM_OK;   
        else if (strstr(pBuff->data, "+CMS ERROR") != NULL) nState = GSM_ERR;   
    }   

    return nState;   
}   

// 从列表中解析出全部短消息    
// 输入: pBuff - 短消息列表缓冲区    
// 输出: pMsg - 短消息缓冲区    
// 返回: 短消息条数    
int SMSWarp::gsmParseMessageList(SM_PARAM* pMsg, SM_BUFF* pBuff)   
{   
    int nMsg;           // 短消息计数值    
    char* ptr;          // 内部用的数据指针    

    nMsg = 0;   
    ptr = pBuff->data;   

    // 循环读取每一条短消息, 以"+CMGL:"开头    
    while((ptr = strstr(ptr, "+CMGL:")) != NULL)   
    {   
        ptr += 6;       // 跳过"+CMGL:", 定位到序号    
        sscanf(ptr, "%d", &pMsg->index); // 读取序号    
        //      TRACE("  index=%d\n",pMsg->index);    

        ptr = strstr(ptr, "\r\n");  // 找下一行    
        if (ptr != NULL)   
        {   
            ptr += 2;       // 跳过"\r\n", 定位到PDU    

            gsmDecodePdu(ptr, pMsg);    // PDU串解码    

            pMsg++;     // 准备读下一条短消息    
            nMsg++;     // 短消息计数加1    
        }   
    }   

    return nMsg;   
}   

// 将一条短消息放入发送队列    
void SMSWarp::PutSendMessage(SM_PARAM* pparam)   
{   
    EnterCriticalSection(&m_csSend);   

    memcpy(&m_SmSend[m_nSendIn], pparam, sizeof(SM_PARAM));   

    m_nSendIn++;   
    if (m_nSendIn >= MAX_SM_SEND)  m_nSendIn = 0;   

    LeaveCriticalSection(&m_csSend);   
}   

// 从发送队列中取一条短消息    
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

// 将短消息放入接收队列    
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

// 从接收队列中取一条短消息    
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
    int nMsg;               // 收到短消息条数    
    int nDelete;            // 目前正在删除的短消息编号    
    SM_BUFF buff;           // 接收短消息列表的缓冲区    
    SM_PARAM param[256];    // 发送/接收短消息缓冲区    
    CTime tmOrg, tmNow;     // 上次和现在的时间，计算超时用    
    enum {   
        stBeginRest,                // 开始休息/延时    
        stContinueRest,             // 继续休息/延时    
        stSendMessageRequest,       // 发送短消息    
        stSendMessageResponse,      // 读取发送应答到缓冲区    
        stSendMessageWaitIdle,      // 发送不成功，等待GSM就绪    
        stReadMessageRequest,       // 发送读取短消息列表的命令    
        stReadMessageResponse,      // 读取短消息列表到缓冲区    
        stDeleteMessageRequest,     // 删除短消息    
        stDeleteMessageResponse,    // 删除短消息    
        stDeleteMessageWaitIdle,    // 删除不成功，等待GSM就绪    
        stExitThread                // 退出    
    } nState;               // 处理过程的状态    

    // 初始状态    
    nState = stBeginRest;   

    // 发送和接收处理的状态循环    
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
                nState = stSendMessageRequest;  // 有待发短消息，就不休息了    
            }   
            else if (tmNow - tmOrg >= 5)     // 待发短消息队列空，休息5秒    
            {   
                nState = stReadMessageRequest;  // 转到读取短消息状态    
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
                //                      TRACE("  GSM_OK %d\n", tmNow - tmOrg);    //需要通知APP，方式待定
                nState = stBeginRest;   
                break;   
            case GSM_ERR:   
                //                      TRACE("  GSM_ERR %d\n", tmNow - tmOrg);    
                nState = stSendMessageWaitIdle;   
                break;   
            default:   
                //                      TRACE("  GSM_WAIT %d\n", tmNow - tmOrg);    
                if (tmNow - tmOrg >= 10)     // 10秒超时    
                {   
                    //                          TRACE("  Timeout!\n");    
                    nState = stSendMessageWaitIdle;   
                }   
            }   
            break;   
        case stSendMessageWaitIdle:   
            Sleep(500);   
            nState = stSendMessageRequest;      // 直到发送成功为止    
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
                if (tmNow - tmOrg >= 15)     // 15秒超时    
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
                if (tmNow - tmOrg >= 5)      // 5秒超时    
                {   
                    //                          TRACE("  Timeout!\n");    
                    nState = stBeginRest;   
                }   
            }   
            break;   

        case stDeleteMessageWaitIdle:   
            //              TRACE("State=stDeleteMessageWaitIdle\n");    
            Sleep(500);   
            nState = stDeleteMessageRequest;        // 直到删除成功为止    
            break;   
        }   

        // 检测是否有关闭本线程的信号    
        DWORD dwEvent = WaitForSingleObject(p->m_hKillThreadEvent, 20);   
        if (dwEvent == WAIT_OBJECT_0)  nState = stExitThread;   
    }   

    // 置该线程结束标志    
    SetEvent(p->m_hThreadKilledEvent);   

    return 0;   
}   
