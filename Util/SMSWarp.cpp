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
	// �������߳�    
	AfxBeginThread(SmThread, this, THREAD_PRIORITY_NORMAL);   
}

void SMSWarp::SetOTANumber(std::string number)
{
	m_strOTANumber = number;
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
		if(nChar == 0)   
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
	WCHAR wchar[256] = {0}; // UNICODE��������
	// �ַ���-->UNICODE��
	nDstLength = ::MultiByteToWideChar(CP_ACP, 0, pSrc, nSrcLength, wchar, 256);
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

// PDU���룬���ڱ��ơ����Ͷ���Ϣ
// pSrc: ԴPDU����ָ��
// pDst: Ŀ��PDU��ָ��
// ����: Ŀ��PDU������
int SMSWarp::gsmEncodePdu_(const SM_PARAM* pSrc, SMS_BUFF* pDst_) {
	int nLength; // �ڲ��õĴ�����
	int nDstLength; // Ŀ��PDU������
	unsigned char buf[256]; // �ڲ��õĻ�����

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

		// SMSC��ַ��Ϣ��
		nLength = strlen(pSrc->SCA); // SMSC��ַ�ַ����ĳ���
		buf[0] = (char)((nLength & 1) == 0 ? nLength : nLength + 1) / 2 + 1;// SMSC��ַ��Ϣ����
		buf[1] = 0x91; // �̶�: �ù��ʸ�ʽ����
		nDstLength = gsmBytes2String(buf, pDst, 2); // ת��2���ֽڵ�Ŀ��PDU��
		// ת��SMSC��Ŀ��PDU��
		nDstLength += gsmInvertNumbers(pSrc->SCA, &pDst[nDstLength], nLength);
		// TPDU�λ���������Ŀ���ַ��
		nLength = strlen(pSrc->TPA); // TP-DA��ַ�ַ����ĳ���
		unsigned char report = 0;
		if(nTotal == 1)
		{
			strcpy(s_Mulit, ptr1);
		
			buf[0] = 0x11+report; // �Ƿ��Ͷ���(TP-MTI=01)��TP-VP����Ը�ʽ(TP-VPF=10)
		}
		else
		{
			memcpy(s_Mulit, ptr1+lineCount[i], lineCount[i+1] - lineCount[i]);
		
			buf[0] = 0x51+report; // �Ƿ��Ͷ���(TP-MTI=01)��TP-VP����Ը�ʽ(TP-VPF=10)
			
			head[4] = nTotal;
			head[5] = i+1;
		
		}
		buf[1] = 0; // TP-MR=0
		buf[2] = (char)nLength; // Ŀ���ַ���ָ���(TP-DA��ַ�ַ�����ʵ����)
		if ((pSrc->TPA[0] == '8') && (pSrc->TPA[1] == '6'))
		{
			buf[3] = 0x91; // �̶�: �ù��ʸ�ʽ����
		}
		else
		{
			buf[3] = 0xA1; // �ù��ڸ�ʽ����
		}
		nDstLength += gsmBytes2String(buf, &pDst[nDstLength], 4);//ת��4���ֽڵ�Ŀ��PDU��
		// ת��TP-DA��Ŀ��PDU��
		nDstLength += gsmInvertNumbers(pSrc->TPA, &pDst[nDstLength], nLength);
		// TPDU��Э���ʶ�����뷽ʽ���û���Ϣ��
		
		nLength = strlen(s_Mulit); // �û���Ϣ�ַ����ĳ���
		buf[0] = pSrc->TP_PID; // Э���ʶ(TP-PID)
		buf[1] = pSrc->TP_DCS; // �û���Ϣ���뷽ʽ(TP-DCS)
		buf[2] = 0xA7; // ��Ч��(TP-VP) 0Ϊ5���� 90Ϊ12Сʱ A7Ϊ24Сʱ
		if(pSrc->TP_DCS == GSM_7BIT) {
			// 7-bit���뷽ʽ
			buf[3] = nLength; // ����ǰ����
			// ת��TP-DA��Ŀ��PDU��
			nLength = gsmEncode7bit(s_Mulit, &buf[4], nLength+1) + 4;
		}
		else if (pSrc->TP_DCS == GSM_UCS2) {
			// UCS2���뷽ʽ
			// ת��TP-DA��Ŀ��PDU��
			buf[3] = gsmEncodeUcs2(s_Mulit, &buf[4], nLength);
			nLength = buf[3] + 4; // nLength���ڸö����ݳ���
		}
		else {
			// 8-bit���뷽ʽ
			// ת��TP-DA��Ŀ��PDU��
			buf[3] = gsmEncode8bit(s_Mulit, &buf[4], nLength);
			nLength = buf[3] + 4; // nLength���ڸö����ݳ���
		}
		// ת���ö����ݵ�Ŀ��PDU��
		// ת���ö����ݵ�Ŀ��PDU��
		//nDstLength += gsmBytes2String(buf, &pDst[nDstLength], nLength);
		//return nDstLength; // ����Ŀ���ַ�������

		if(nTotal > 1)
		{
			buf[3] = buf[3]+6;				//�����ų��ȼ���6

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

	return nCount; // ����Ŀ���ַ�������
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
	if ((pSrc->TPA[0] == '8') && (pSrc->TPA[1] == '6'))
	{
		buf[3] = 0x91; // �̶�: �ù��ʸ�ʽ����
	}
	else
	{
		buf[3] = 0xA1; // �ù��ڸ�ʽ����
	}
	nDstLength += gsmBytes2String(buf, &pDst[nDstLength], 4);//ת��4���ֽڵ�Ŀ��PDU��
	// ת��TP-DA��Ŀ��PDU��
	nDstLength += gsmInvertNumbers(pSrc->TPA, &pDst[nDstLength], nLength);
	// TPDU��Э���ʶ�����뷽ʽ���û���Ϣ��
	nLength = strlen(pSrc->TP_UD); // �û���Ϣ�ַ����ĳ���
	buf[0] = pSrc->TP_PID; // Э���ʶ(TP-PID)
	buf[1] = pSrc->TP_DCS; // �û���Ϣ���뷽ʽ(TP-DCS)
	buf[2] = 0xA7; // ��Ч��(TP-VP) 0Ϊ5���� 90Ϊ12Сʱ A7Ϊ24Сʱ
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
	unsigned char udhi = 0; //�Ƿ�������ͷ
	unsigned char udh[32]; //����ͷ����

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
int SMSWarp::gsmSendMessage(SM_PARAM* pSrc, SM_BUFF* pBuff)   
{  
/*	
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
		pBuff->len = m_pAT->SmsSend(pdu, strlen(pdu), pBuff->data, 16384);
	}
	
	*/
	
	int nPduLength;     // PDU������    
	unsigned char nSmscLength;  // SMSC������    
	//int nLength;        // �����յ������ݳ���    
	//char cmd[16];       // ���    
	SMS_BUFF pdu[MAX_SM_SEND];      // PDU��    
	//char ans[128];      // Ӧ��    
	
	int count = gsmEncodePdu_(pSrc, pdu);   // ����PDU����������PDU��    

	for(int i = 0; i < count; i++)
	{
		nPduLength = pdu[i].len;
		strcat(pdu[i].buff, "\x01a");       // ��Ctrl-Z����    
		gsmString2Bytes(pdu[i].buff, &nSmscLength, 2);  // ȡPDU���е�SMSC��Ϣ����    
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
			pBuff->len = m_pAT->SmsSend(pdu[i].buff, strlen(pdu[i].buff), pBuff->data, 16384);
		}
	}
	
	return 0;   
}   

// ��ȡ����Ϣ���������������ȡӦ��    
// ��+CMGL����+CMGR����һ���Զ���ȫ������Ϣ    
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

// ɾ������Ϣ���������������ȡӦ��    
// ����: index - ����Ϣ��ţ�1-255    
int SMSWarp::gsmDeleteMessage(int index, SM_BUFF* pBuff)   
{   
	//char cmd[16];       // ���    
	//sprintf(cmd, "AT+CMGD=%d\r", index);    // ��������    
	//// ������    
	//return WriteComm(cmd, strlen(cmd)); 
//	return m_pAT->SmsDelete(index);
	pBuff->len = m_pAT->SmsDelete(index, pBuff->data, 16384);
	return pBuff->len;
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
	
//0526	
// 	nLength = m_pAT->SmsReadResponse(&pBuff->data[pBuff->len], 16384 - pBuff->len);
// 	pBuff->len += nLength;   

	// ȷ��GSM MODEM��Ӧ��״̬    
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
	buff.data = (char*)malloc(16384);
	SM_PARAM param[MAX_SM_SEND];    // ����/���ն���Ϣ������    
	enum {   
		stBeginRest,                // ��ʼ��Ϣ/��ʱ    
		stContinueRest,             // ������Ϣ/��ʱ    
		stSendMessageRequest,       // ���Ͷ���Ϣ    
		stSendMessageResponse,      // ��ȡ����Ӧ�𵽻�����    
		stReadMessageRequest,       // ���Ͷ�ȡ����Ϣ�б������    
		stReadMessageResponse,      // ��ȡ����Ϣ�б�������    
		stDeleteMessageRequest,     // ɾ������Ϣ    
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
				nState = stSendMessageRequest;  // �д�������Ϣ���Ͳ���Ϣ��    
			}   
			else  
			{   
				nState = stReadMessageRequest;  // ת����ȡ����Ϣ״̬    
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
				TRACE(L"Send  GSM_OK \n");    //��Ҫ֪ͨAPP����ʽ����
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

		// ����Ƿ��йرձ��̵߳��ź�    
		DWORD dwEvent = WaitForSingleObject(p->m_hKillThreadEvent, 20);   
		if (dwEvent == WAIT_OBJECT_0)  nState = stExitThread;   
	}   

	free(buff.data);
	// �ø��߳̽�����־    
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

	// ȥ������ǰ��"+"	// �ں���ǰ��"86"    
	if(sca[0] == '+')  sca = sca.substr(1);   
	if(sca.substr(0, 2) != "86")  sca = "86" + sca;   
 
	// ������Ϣ�ṹ    
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