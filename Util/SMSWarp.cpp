#include "stdafx.h"
#include "SMSWarp.h"
#include "../Data/Message.h"
#include "../Data/MmsMessage.h"
#include "../Data/OtaMessage.h"
#include "../Multimediaphone.h"
#include "../MultimediaPhoneDlg.h"
#undef atoi
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

	//	AfxBeginThread(SmThread, /*this*/sms, THREAD_PRIORITY_NORMAL);   
	}
	return sms;
}
void SMSWarp::Bind_(Util::ATCommandWarp* at)
{
	m_pAT = at;
}
void SMSWarp::Bind(Util::ATCommandWarp* at)
{
	m_pAT = at;
	// 启动子线程    
	AfxBeginThread(SmThread, this, THREAD_PRIORITY_NORMAL);   
}

void SMSWarp::SetOTANumber(std::string number)
{
	m_strOTANumber = number;
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
		if(nChar == 0)   
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
	WCHAR wchar[256] = {0}; // UNICODE串缓冲区
	// 字符串-->UNICODE串
	nDstLength = ::MultiByteToWideChar(CP_ACP, 0, pSrc, nSrcLength, wchar, 256);
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

int Findlinefonrbuff(char *buf, int maxLineCount, int *pLineCount)
{
	int i = 0;
	unsigned char *ptr = (unsigned char *)buf;
	int ncount = 0;
	BOOL isHanzi = FALSE;
	int jcount = 0;
	pLineCount[i++] = ncount;
	while(*ptr != '\0')
	{
		if(*ptr >= 0xA0)
		{
			ptr++;
			ncount++;
			isHanzi = TRUE;
		}
		else
		{

		}
		jcount++;
		ptr++;
		ncount++;
		if(jcount == maxLineCount)
		{
			jcount = 0;
			pLineCount[i] = ncount;
			i++;
		}
	}
	if(jcount != 0)
	{
	//	jcount = 0;
		pLineCount[i] = ncount;
		i++;
	}

	if(i == 3 && jcount == 3)
		i = 2;
	return (i-1);
}

// PDU编码，用于编制、发送短消息
// pSrc: 源PDU参数指针
// pDst: 目标PDU串指针
// 返回: 目标PDU串长度
int SMSWarp::gsmEncodePdu_(const SM_PARAM* pSrc, SMS_BUFF* pDst_) {
	int nLength; // 内部用的串长度
	int nDstLength; // 目标PDU串长度
	unsigned char buf[256]; // 内部用的缓冲区

	char s_Mulit[512];
    char *ptr1 = (char *)pSrc->TP_UD;
	int nCount = 0;
	char pDst[512];

	int lineCount[24];
	int nTotal = Findlinefonrbuff((char *)pSrc->TP_UD, 67, &lineCount[0]);

	if(nTotal > MAX_SM_SEND)
		nTotal = MAX_SM_SEND;

	unsigned char head[6];
	head[0] = 0x05;
	head[1] = 0x00;
	head[2] = 0x03;
	head[3] = rand()%255;

	for(int i = 0; i < nTotal; i++)
	{
		memset(s_Mulit, 0, 512);

		// SMSC地址信息段
		nLength = strlen(pSrc->SCA); // SMSC地址字符串的长度
		buf[0] = (char)((nLength & 1) == 0 ? nLength : nLength + 1) / 2 + 1;// SMSC地址信息长度
		buf[1] = 0x91; // 固定: 用国际格式号码
		nDstLength = gsmBytes2String(buf, pDst, 2); // 转换2个字节到目标PDU串
		// 转换SMSC到目标PDU串
		nDstLength += gsmInvertNumbers(pSrc->SCA, &pDst[nDstLength], nLength);
		// TPDU段基本参数、目标地址等
		nLength = strlen(pSrc->TPA); // TP-DA地址字符串的长度
		unsigned char report = 0;
		if(nTotal == 1)
		{
			strcpy(s_Mulit, ptr1);
		
			buf[0] = 0x11+report; // 是发送短信(TP-MTI=01)，TP-VP用相对格式(TP-VPF=10)
		}
		else
		{
			memcpy(s_Mulit, ptr1+lineCount[i], lineCount[i+1] - lineCount[i]);
		
			buf[0] = 0x51+report; // 是发送短信(TP-MTI=01)，TP-VP用相对格式(TP-VPF=10)
			
			head[4] = nTotal;
			head[5] = i+1;
		
		}
		buf[1] = 0; // TP-MR=0
		buf[2] = (char)nLength; // 目标地址数字个数(TP-DA地址字符串真实长度)
		if ((pSrc->TPA[0] == '8') && (pSrc->TPA[1] == '6'))
		{
			buf[3] = 0x91; // 固定: 用国际格式号码
		}
		else
		{
			buf[3] = 0xA1; // 用国内格式号码
		}
		nDstLength += gsmBytes2String(buf, &pDst[nDstLength], 4);//转换4个字节到目标PDU串
		// 转换TP-DA到目标PDU串
		nDstLength += gsmInvertNumbers(pSrc->TPA, &pDst[nDstLength], nLength);
		// TPDU段协议标识、编码方式、用户信息等
		
		nLength = strlen(s_Mulit); // 用户信息字符串的长度
		buf[0] = pSrc->TP_PID; // 协议标识(TP-PID)
		buf[1] = pSrc->TP_DCS; // 用户信息编码方式(TP-DCS)
		buf[2] = 0xA7; // 有效期(TP-VP) 0为5分钟 90为12小时 A7为24小时
		if(pSrc->TP_DCS == GSM_7BIT) {
			// 7-bit编码方式
			buf[3] = nLength; // 编码前长度
			// 转换TP-DA到目标PDU串
			nLength = gsmEncode7bit(s_Mulit, &buf[4], nLength+1) + 4;
		}
		else if (pSrc->TP_DCS == GSM_UCS2) {
			// UCS2编码方式
			// 转换TP-DA到目标PDU串
			buf[3] = gsmEncodeUcs2(s_Mulit, &buf[4], nLength);
			nLength = buf[3] + 4; // nLength等于该段数据长度
		}
		else {
			// 8-bit编码方式
			// 转换TP-DA到目标PDU串
			buf[3] = gsmEncode8bit(s_Mulit, &buf[4], nLength);
			nLength = buf[3] + 4; // nLength等于该段数据长度
		}
		// 转换该段数据到目标PDU串
		// 转换该段数据到目标PDU串
		//nDstLength += gsmBytes2String(buf, &pDst[nDstLength], nLength);
		//return nDstLength; // 返回目标字符串长度

		if(nTotal > 1)
		{
			buf[3] = buf[3]+6;				//长短信长度加上6

			char head_[25];
			nDstLength += gsmBytes2String(buf, &pDst[nDstLength], 4);
			sprintf(head_, "%02x%02x%02x%02x%02x%02x", head[0]&0xFF, head[1]&0xFF,head[2]&0xFF,head[3]&0xFF,head[4]&0xFF,head[5]&0xFF);
			strcpy(&pDst[nDstLength], head_);
			nDstLength += 12;
			nDstLength += gsmBytes2String(buf+4, &pDst[nDstLength], nLength-4);
			strcpy(pDst_[nCount].buff, pDst);
			pDst_[nCount].len = nDstLength;
		}
		else
		{
			nDstLength += gsmBytes2String(buf, &pDst[nDstLength], nLength);
			pDst_[nCount].len = nDstLength;
			strcpy(pDst_[nCount].buff, pDst);
		}
		nCount++;
	}

	return nCount; // 返回目标字符串长度
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
	if ((pSrc->TPA[0] == '8') && (pSrc->TPA[1] == '6'))
	{
		buf[3] = 0x91; // 固定: 用国际格式号码
	}
	else
	{
		buf[3] = 0xA1; // 用国内格式号码
	}
	nDstLength += gsmBytes2String(buf, &pDst[nDstLength], 4);//转换4个字节到目标PDU串
	// 转换TP-DA到目标PDU串
	nDstLength += gsmInvertNumbers(pSrc->TPA, &pDst[nDstLength], nLength);
	// TPDU段协议标识、编码方式、用户信息等
	nLength = strlen(pSrc->TP_UD); // 用户信息字符串的长度
	buf[0] = pSrc->TP_PID; // 协议标识(TP-PID)
	buf[1] = pSrc->TP_DCS; // 用户信息编码方式(TP-DCS)
	buf[2] = 0xA7; // 有效期(TP-VP) 0为5分钟 90为12小时 A7为24小时
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
	unsigned char udhi = 0; //是否有数据头
	unsigned char udh[32]; //数据头内容

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

	pDst->Total = 1;
	pDst->Number = 1;
	if (udhi == 0x40)
	{
		unsigned char hl;
		gsmString2Bytes(pSrc, &hl, 2);
		if (hl > 0)
		{
			gsmString2Bytes(pSrc, udh, (hl+1) * 2);
			pSrc += 2;
			pSrc += hl * 2;
			tmp -= (hl + 1);

			if (udh[2] == 0x03)
			{
				gsmBytes2String(&udh[3], pDst->Serial , 1);
				pDst->Total = udh[4];
				pDst->Number = udh[5];
			}
			else if (udh[2] == 0x04)
			{
				gsmBytes2String(&udh[3], pDst->Serial, 2);
				pDst->Total = udh[5];
				pDst->Number = udh[6];
			}
		}
	}
	pDst->Type = mtSMS;
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

				//pDst->pMMS_Notify = new MMS::MMSWarp::MMS_NotificationRequest;
				//MMS::MMSWarp::GetMMSWarp()->DecodeNotificationRequest((const char*)pBuf, nDstLength, pDst->pMMS_Notify);

				int length = 0;

				while (length < nDstLength)
				{
					unsigned char flag = pBuf[length++];
					if (flag == 0x8C)
					{
						pDst->MMS_Type = pBuf[length++];
					}
					else if (flag == 0x98)
					{
						pDst->MMS_TransactionID = std::string((char*)pBuf + length);
						length += pDst->MMS_TransactionID.size() + 1;
					}
					else if (flag == 0x8D)
					{
						pDst->MMS_Version = pBuf[length++];
					}
					else if (flag == 0x83)
					{
						//pDst->MessageReference = string(pBuf + length);
						//length += pDst->MessageReference.size() + 1;
						std::string url = std::string((char*)pBuf + length);
						gsmDecode8bit((const unsigned char *)url.c_str(), pDst->TP_UD, url.size());
						length += url.size() + 1;
					}
					else if (flag == 0x88)
					{
						unsigned char len = pBuf[length++];//0x05 0x81  //Expiry + Length + Relative-token + Length + Delta-secs
						unsigned int n = 0;
						unsigned char token = pBuf[length];
						if (token == 0x81)
						{
							length += 1;
							unsigned char len = pBuf[length];
							length += 1;
							for (int i = 0; i < len; ++i)
							{
								unsigned char t = pBuf[length];
								n = ((n << 8) | t);
								length += 1;
							}
						}
						else
						{
							length += 1;
							unsigned char len = pBuf[length];
							length += 1;
							char buf[128] = {0};
							unsigned int l = gsmSerializeNumbers((const char*)pBuf + length, buf, len);
							std::string t(buf, l);
							CTime expiry(atoi(t.substr(0, 2).c_str()) + 2000, atoi(t.substr(2, 2).c_str()), atoi(t.substr(4, 2).c_str()), atoi(t.substr(6, 2).c_str()), atoi(t.substr(8, 2).c_str()), atoi(t.substr(10, 2).c_str()));
							CTime start(1970, 1, 1, 8, 0, 0);
							CTimeSpan ts = expiry - start;
							n = ts.GetTotalSeconds();
							length += len;
						}
						pDst->MMS_TimeOfExpiry = n;
					}
					else if (flag == 0x89)
					{
						unsigned char len = pBuf[length++];
						length += 1;//0x80
						pDst->MMS_SenderAddress = std::string((char*)pBuf + length);
						length += pDst->MMS_SenderAddress.size() + 1;
					}
					else if (flag == 0x8A)
					{
						pDst->MMS_Class = pBuf[length++];
					}
					else if (flag == 0x8E)
					{
						unsigned char len = pBuf[length++];
						unsigned int n = 0;
						for (int i = 0; i < len; ++i)
						{
							unsigned char s = pBuf[length];
							n = ((n << 8) | s);
							length += 1;
						}
						pDst->MMS_Size = n;
					}
				}
				pDst->Type = mtMMS;
			}
			else if ((tmp == 1) && (*pSrc == 0xAE)) //si
			{
				pDst->Type = mtSI;
			}
		}
		else //sms
		{
			unsigned int otaType = 0;
			for (int i = 0; i < 4; ++i)
			{
				otaType = ((otaType << 8) | buf[i]);
			}
			//1065805044
			if ((std::string(pDst->TPA) == m_strOTANumber))// || (otaType == tOTAMenu) || (otaType == tOTAdMediaFile) || (otaType == tOTAInterface) || (otaType == tOTAWeather))
			{
				gsmDecode8bit(buf + 4, pDst->TP_UD, nDstLength - 4);
				//pDst->pOTA_Notify = new OTA_NotificationRequest();
				//pDst->pOTA_Notify->Type = (OTA_TYPE)otaType;
				pDst->OTAType = (OTA_TYPE)otaType;
				pDst->Type = mtOTA;
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
int SMSWarp::gsmSendMessage(SM_PARAM* pSrc, SM_BUFF* pBuff)   
{  
/*	
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
		pBuff->len = m_pAT->SmsSend(pdu, strlen(pdu), pBuff->data, 16384);
	}
	
	*/
	
	int nPduLength;     // PDU串长度    
	unsigned char nSmscLength;  // SMSC串长度    
	//int nLength;        // 串口收到的数据长度    
	//char cmd[16];       // 命令串    
	SMS_BUFF pdu[MAX_SM_SEND];      // PDU串    
	//char ans[128];      // 应答串    
	
	int count = gsmEncodePdu_(pSrc, pdu);   // 根据PDU参数，编码PDU串    

	for(int i = 0; i < count; i++)
	{
		nPduLength = pdu[i].len;
		strcat(pdu[i].buff, "\x01a");       // 以Ctrl-Z结束    
		gsmString2Bytes(pdu[i].buff, &nSmscLength, 2);  // 取PDU串中的SMSC信息长度    
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
			pBuff->len = m_pAT->SmsSend(pdu[i].buff, strlen(pdu[i].buff), pBuff->data, 16384);
		}
	}
	
	return 0;   
}   

// 读取短消息，仅发送命令，不读取应答    
// 用+CMGL代替+CMGR，可一次性读出全部短消息    
int SMSWarp::gsmReadMessageList(SM_BUFF* pBuff)   
{   
//	return WriteComm("AT+CMGL=0\r", 10);   
//	return m_pAT->SmsReadList();
	pBuff->len = m_pAT->SmsReadList(pBuff->data, 16384);
	Dprintf("pBuff->len %d\r", pBuff->len);
	Dprintf("pBuff->data: %x %s\r", pBuff->data, (char *)pBuff->data);
	pBuff->len = strlen((char *)pBuff->data);
	return pBuff->len;
}   

// 删除短消息，仅发送命令，不读取应答    
// 输入: index - 短消息序号，1-255    
int SMSWarp::gsmDeleteMessage(int index, SM_BUFF* pBuff)   
{   
	//char cmd[16];       // 命令串    
	//sprintf(cmd, "AT+CMGD=%d\r", index);    // 生成命令    
	//// 输出命令串    
	//return WriteComm(cmd, strlen(cmd)); 
//	return m_pAT->SmsDelete(index);
	pBuff->len = m_pAT->SmsDelete(index, pBuff->data, 16384);
	return pBuff->len;
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
	
//0526	
// 	nLength = m_pAT->SmsReadResponse(&pBuff->data[pBuff->len], 16384 - pBuff->len);
// 	pBuff->len += nLength;   

	// 确定GSM MODEM的应答状态    
	nState = GSM_WAIT;   
	if (/*(nLength > 0) && */(pBuff->len >= 4))   //0526
	{   
// 		if (strncmp(&pBuff->data[pBuff->len - 4], "OK\r\n", 4) == 0)
		if (strstr(pBuff->data, "\r\nOK\r\n") != NULL)
		{
			nState = GSM_OK;   
		}
		else if (strstr(pBuff->data, "+CMS ERROR") != NULL)
		{
			nState = GSM_ERR;   
		}
		else
		{
			nState = GSM_OK; 
		}
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
	buff.data = (char*)malloc(16384);
	SM_PARAM param[MAX_SM_SEND];    // 发送/接收短消息缓冲区    
	enum {   
		stBeginRest,                // 开始休息/延时    
		stContinueRest,             // 继续休息/延时    
		stSendMessageRequest,       // 发送短消息    
		stSendMessageResponse,      // 读取发送应答到缓冲区    
		stReadMessageRequest,       // 发送读取短消息列表的命令    
		stReadMessageResponse,      // 读取短消息列表到缓冲区    
		stDeleteMessageRequest,     // 删除短消息    
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
			TRACE(L"SMS State=stBeginRest\n");    
			Sleep(2500);
			nState = stContinueRest; 
			EnterCriticalSection(&csCom);
			TRACE(L"SMS Enter\n");
			break;   

		case stContinueRest:   
			TRACE(L"SMS State=stContinueRest\n");    
			if (p->GetSendMessage(param))   
			{   
				nState = stSendMessageRequest;  // 有待发短消息，就不休息了    
			}   
			else  
			{   
				nState = stReadMessageRequest;  // 转到读取短消息状态    
			}   
			break;   

		case stSendMessageRequest:   
			TRACE(L"SMS State=stSendMessageRequest\n");    
			buff.len = 0;
			memset(buff.data, 0, 16384);   
			::SendMessage(theApp.m_pMainWnd->m_hWnd, WM_SMS_SENDPRO, (WPARAM)&param, (LPARAM)0);
			p->gsmSendMessage(param, &buff);   
			nState = stSendMessageResponse;   
			break;   

		case stSendMessageResponse:   
			TRACE(L"SMS State=stSendMessageResponse\n");    
			switch (p->gsmGetResponse(&buff))   
			{   
			case GSM_OK:    
				TRACE(L"Send  GSM_OK \n");    //需要通知APP，方式待定
				p->SendCallback(true);
				break;   
			case GSM_ERR:   
				TRACE(L"Send  GSM_ERR \n");    
				p->SendCallback(false);
				break;   
			default:   
				if (strstr(buff.data, "+CMGS:") != NULL)//+CMGS:
				{
					TRACE(L"Send  GSM_WAIT \n"); 
					p->SendCallback(true);
				}
				else  
				{   
					TRACE(L"Send  Timeout!\n");    
					p->SendCallback(false);
				}   
			} 
			LeaveCriticalSection(&csCom);
			TRACE(L"SMS Leave\n");
			nState = stBeginRest;  
			break;   

		case stReadMessageRequest:   
			
			TRACE(L"SMS State=stReadMessageRequest\n");    
			buff.len = 0;
			memset(buff.data, 0, 16384);   
			p->gsmReadMessageList(&buff);   
			nState = stReadMessageResponse;   
			break;   

		case stReadMessageResponse:   
			TRACE(L"SMS State=stReadMessageResponse\n");  
			
			Sleep(1000);   
			switch (p->gsmGetResponse(&buff))   
			{   
			case GSM_OK:    
				nMsg = p->gsmParseMessageList(param, &buff);   
				TRACE(L"Read  GSM_OK %d\n", nMsg);    
				if (nMsg > 0)   
				{   
					for (int i = 0; i < nMsg; ++i)
					{
						p->MapMessage(&param[i]);
					}
					nDelete = 0;   
					nState = stDeleteMessageRequest;   
				}   
				else   
				{   
					LeaveCriticalSection(&csCom);
					TRACE(L"SMS Leave\n");
					nState = stBeginRest;  
				}   
				break;   
			default:   
				TRACE(L"Read  GSM_ERR \n");    
				LeaveCriticalSection(&csCom);
				TRACE(L"SMS Leave\n");
				nState = stBeginRest;   
				break;   
			}   
			break;   

		case stDeleteMessageRequest:   
			TRACE(L"SMS State=stDeleteMessageRequest\n");    
			buff.len = 0;
			memset(buff.data, 0, 16384);   
   
			while (nDelete < nMsg)
			{
				p->gsmDeleteMessage(param[nDelete].index, &buff);   
				nDelete++;   
				Sleep(300);
			}

			LeaveCriticalSection(&csCom);
			TRACE(L"SMS Leave\n");
			nState = stBeginRest;   
			break;     
		}   

		// 检测是否有关闭本线程的信号    
		DWORD dwEvent = WaitForSingleObject(p->m_hKillThreadEvent, 20);   
		if (dwEvent == WAIT_OBJECT_0)  nState = stExitThread;   
	}   

	free(buff.data);
	// 置该线程结束标志    
	SetEvent(p->m_hThreadKilledEvent);   

	return 0;   
}   

//////////////////////////////////////////////////////////////////////////

void SMSWarp::Send(std::string number, std::string content)
{
	SMS::SMSWarp::SM_PARAM SmParam;   
	memset(&SmParam, 0, sizeof(SMS::SMSWarp::SM_PARAM));   
	std::string sca = m_pAT->SmsCenterAddress();
	if (sca.empty())
	{
		SetCenterAddress();
		sca = m_strCenterAddress;
	}

	// 去掉号码前的"+"	// 在号码前加"86"    
	if(sca[0] == '+')  sca = sca.substr(1);   
	if(sca.substr(0, 2) != "86")  sca = "86" + sca;   
 
	// 填充短消息结构    
	strcpy(SmParam.SCA, sca.c_str());   
	strcpy(SmParam.TP_UD, content.c_str());   
	SmParam.TP_PID = 0;   
	SmParam.TP_DCS = GSM_UCS2;


	std::string number_;
	do{
		    BOOL flag = FALSE; 
			int nIndex = number.find(",");
			if(nIndex < 0)
				nIndex = number.find(";");
			if(nIndex < 0)
			{
				flag = TRUE;
				number_ = number;
			}
			else
			{
				number_ = number.substr(0, nIndex);
				number = number.substr(nIndex+1, number.length() - (nIndex+1));
			}

			if(number_ != "")
			{
				if (number_.substr(0, 2) != "10")
				{
					if(number_[0] == '+')  number_ = number_.substr(1); 
					if(number_.substr(0, 2) != "86")  number_ = "86" + number_;   
				}
				strcpy(SmParam.TPA, number_.c_str());   
				PutSendMessage(&SmParam);
			}
			
			if(flag)
				break;
	}while(true);
}

void SMSWarp::SetCenterAddress()
{
	m_strCenterAddress = ((CMultimediaPhoneDlg *)(theApp.m_pMainWnd))->m_pSettingDlg->m_pSetting->speCode12_;
//	m_pAT->SmsCenterAddress(address);
}

std::wstring SMSWarp::ToUnicode(std::string content)
{
	size_t wideContentLength = MultiByteToWideChar(CP_ACP, 0, content.c_str(), content.length(), 0, 0);
	wchar_t* wideContent = new wchar_t[wideContentLength + 2];
	memset(wideContent, 0, sizeof(wchar_t) * (wideContentLength + 2));
	size_t convLength = MultiByteToWideChar(CP_ACP, 0, content.c_str(), content.length(), wideContent, wideContentLength);
	std::wstring result(wideContent);
	delete[] wideContent;
	return result;
}
void SMSWarp::MapMessage(SM_PARAM* sms)
{
	void* pMsg = NULL;
	if (sms->Type == mtSMS)
	{
		Data::Message* msg = new Data::Message;
		pMsg = msg;
		std::string sca(sms->SCA);
		msg->smsc.address = sca;
		msg->smsc.addressLength = sca.size();
		msg->smsc.addressType.typeOfNumber = Data::AddressType::tInternationNumber;

		msg->flag.type = Data::Flag::tDeliver;

		std::string num(sms->TPA);
		msg->remote.address = num;
		msg->remote.addressLength = num.size();
		msg->remote.addressType.typeOfNumber = Data::AddressType::tNationNumber;

		msg->encoding.encoding = Data::Encoding::UnicodeCodeSet;
		msg->timestamp.year = atoi(std::string(&sms->TP_SCTS[0],2).c_str());
		msg->timestamp.month = atoi(std::string(&sms->TP_SCTS[2],2).c_str());
		msg->timestamp.day = atoi(std::string(&sms->TP_SCTS[4],2).c_str());
		msg->timestamp.hour = atoi(std::string(&sms->TP_SCTS[6],2).c_str());
		msg->timestamp.minite = atoi(std::string(&sms->TP_SCTS[8],2).c_str());
		msg->timestamp.second = atoi(std::string(&sms->TP_SCTS[10],2).c_str());


// 		std::string content(sms->TP_UD);
// 		size_t wideContentLength = MultiByteToWideChar(CP_ACP, 0, content.c_str(), content.length(), 0, 0);
// 		wchar_t* wideContent = new wchar_t[wideContentLength + 2];
// 		memset(wideContent, 0, sizeof(wchar_t) * (wideContentLength + 2));
// 		size_t convLength = MultiByteToWideChar(CP_ACP, 0, content.c_str(), content.length(), wideContent, wideContentLength);
// 		std::wstring result(wideContent);
// 		delete[] wideContent;

		msg->unicodeData = sms->TP_UD;
		msg->state = Data::Message::sNoRead;
		msg->group = Data::Message::gReceive;
		msg->total = sms->Total;
		if (sms->Total > 1)
		{
 			msg->transactionId = sms->Serial;
			msg->total = sms->Total;
 			msg->no = sms->Number;
		}
	}
	else if (sms->Type == mtMMS)
	{
		Data::MmsMessage* msg = new Data::MmsMessage;
		pMsg = msg;
		std::string sca(sms->SCA);
		msg->smsc.address = sca;
		msg->smsc.addressLength = sca.size();
		msg->smsc.addressType.typeOfNumber = Data::AddressType::tInternationNumber;
		
		msg->flag.type = Data::Flag::tDeliver;
		
		std::string num(sms->TPA);
		msg->remote.address = num;
		msg->remote.addressLength = num.size();
		msg->remote.addressType.typeOfNumber = Data::AddressType::tNationNumber;
		
		msg->encoding.encoding = Data::Encoding::UnicodeCodeSet;
		
		msg->timestamp.year = atoi(std::string(&sms->TP_SCTS[0],2).c_str());
		msg->timestamp.month = atoi(std::string(&sms->TP_SCTS[2],2).c_str());
		msg->timestamp.day = atoi(std::string(&sms->TP_SCTS[4],2).c_str());
		msg->timestamp.hour = atoi(std::string(&sms->TP_SCTS[6],2).c_str());
		msg->timestamp.minite = atoi(std::string(&sms->TP_SCTS[8],2).c_str());
		msg->timestamp.second = atoi(std::string(&sms->TP_SCTS[10],2).c_str());


//		std::string content(sms->TP_UD);
// 		size_t wideContentLength = MultiByteToWideChar(CP_ACP, 0, content.c_str(), content.length(), 0, 0);
// 		wchar_t* wideContent = new wchar_t[wideContentLength + 2];
// 		memset(wideContent, 0, sizeof(wchar_t) * (wideContentLength + 2));
// 		size_t convLength = MultiByteToWideChar(CP_ACP, 0, content.c_str(), content.length(), wideContent, wideContentLength);
// 		std::wstring result(wideContent);
// 		delete[] wideContent;

		msg->state = Data::MmsMessage::stUnDownload;
		msg->transactionId = sms->MMS_TransactionID;
		msg->messageClass = sms->MMS_Class;
		msg->messageSize = sms->MMS_Size;
		msg->timeOfExpiry = sms->MMS_TimeOfExpiry;
		msg->url = sms->TP_UD;
	}
	else if (sms->Type == mtOTA)
	{
		Data::OtaMessage* msg = new Data::OtaMessage;
		pMsg = msg;
		std::string sca(sms->SCA);
		msg->smsc.address = sca;
		msg->smsc.addressLength = sca.size();
		msg->smsc.addressType.typeOfNumber = Data::AddressType::tInternationNumber;
		
		msg->flag.type = Data::Flag::tDeliver;
		
		std::string num(sms->TPA);
		msg->remote.address = num;
		msg->remote.addressLength = num.size();
		msg->remote.addressType.typeOfNumber = Data::AddressType::tNationNumber;
		
		msg->encoding.encoding = Data::Encoding::UnicodeCodeSet;
		
		msg->timestamp.year = atoi(std::string(&sms->TP_SCTS[0],2).c_str());
		msg->timestamp.month = atoi(std::string(&sms->TP_SCTS[2],2).c_str());
		msg->timestamp.day = atoi(std::string(&sms->TP_SCTS[4],2).c_str());
		msg->timestamp.hour = atoi(std::string(&sms->TP_SCTS[6],2).c_str());
		msg->timestamp.minite = atoi(std::string(&sms->TP_SCTS[8],2).c_str());
		msg->timestamp.second = atoi(std::string(&sms->TP_SCTS[10],2).c_str());

// 		std::string content(sms->TP_UD);
// 		size_t wideContentLength = MultiByteToWideChar(CP_ACP, 0, content.c_str(), content.length(), 0, 0);
// 		wchar_t* wideContent = new wchar_t[wideContentLength + 2];
// 		memset(wideContent, 0, sizeof(wchar_t) * (wideContentLength + 2));
// 		size_t convLength = MultiByteToWideChar(CP_ACP, 0, content.c_str(), content.length(), wideContent, wideContentLength);
// 		std::wstring result(wideContent);
// 		delete[] wideContent;

		msg->state = Data::OtaMessage::stUnDownload;
		msg->instruction = Data::Type((int)sms->OTAType);
		msg->url = sms->TP_UD;
	}
	ReceiveCallback(sms->Type, pMsg);
}
void SMSWarp::ReceiveCallback(SMS_TYPE type, void* msg)
{
	switch (type)
	{
	case mtSMS:
		PostMessage(theApp.m_pMainWnd->m_hWnd, WM_SMS_STATUS, WM_SMS_SMS, (LPARAM)msg);
		break;
	case mtMMS:
		PostMessage(theApp.m_pMainWnd->m_hWnd, WM_SMS_STATUS, WM_SMS_MMS, (LPARAM)msg);
		break;
	case mtOTA:
		PostMessage(theApp.m_pMainWnd->m_hWnd, WM_SMS_STATUS, WM_SMS_OTA, (LPARAM)msg);
		break;
	}
}
void SMSWarp::SendCallback(bool send)
{
	PostMessage(theApp.m_pMainWnd->m_hWnd, WM_SMS_STATUS, WM_SMS_SENDRESULT, int(send));
}