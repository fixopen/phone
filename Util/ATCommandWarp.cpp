#include "stdafx.h"
#include "ATCommandWarp.h"

#include "../resource.h"
#include "../Multimediaphone.h"

using Util::ATCommandWarp;

bool ATCommandWarp::bInit = false;

/************************************************************************/
/*        add by qi                                                      */
/************************************************************************/
struct Converter {
	Converter() {
		memset(convertBuffer, 0, 7);
	}
	unsigned char convertBuffer[7];
	unsigned char sevenBit0() {
		return convertBuffer[0] >> 1;
	}
	unsigned char sevenBit1() {
		return ((convertBuffer[0] & 0x1) << 6) | (convertBuffer[1] >> 2);
	}
	unsigned char sevenBit2() {
		return ((convertBuffer[1] & 0x3) << 5) | (convertBuffer[2] >> 3);
	}
	unsigned char sevenBit3() {
		return ((convertBuffer[2] & 0x7) << 4) | (convertBuffer[3] >> 4);
	}
	unsigned char sevenBit4() {
		return ((convertBuffer[3] & 0xF) << 3) | (convertBuffer[4] >> 5);
	}
	unsigned char sevenBit5() {
		return ((convertBuffer[4] & 0x1F) << 2) | (convertBuffer[5] >> 6);
	}
	unsigned char sevenBit6() {
		return ((convertBuffer[5] & 0x3F) << 1) | (convertBuffer[6] >> 7);
	}
	unsigned char sevenBit7() {
		return ((convertBuffer[6] & 0x7F) << 0);
	}
	void sevenBit0(unsigned char ch) {
		ch &= 0x7F;
		convertBuffer[0] = ch << 1;
	}
	void sevenBit1(unsigned char ch) {
		ch &= 0x7F;
		convertBuffer[0] |= (ch & 0x40) >> 6;
		convertBuffer[1] = (ch & 0x3F) << 2;
	}
	void sevenBit2(unsigned char ch) {
		convertBuffer[1] |= (ch & 0x60) >> 5;
		convertBuffer[2] = (ch & 0x1F) << 3;
	}
	void sevenBit3(unsigned char ch) {
		convertBuffer[2] |= (ch & 0x70) >> 4;
		convertBuffer[3] = (ch & 0xF) << 4;
	}
	void sevenBit4(unsigned char ch) {
		convertBuffer[3] |= (ch & 0x78) >> 3;
		convertBuffer[4] = (ch & 0x7) << 5;
	}
	void sevenBit5(unsigned char ch) {
		convertBuffer[4] |= (ch & 0x7C) >> 2;
		convertBuffer[5] = (ch & 0x3) << 6;
	}
	void sevenBit6(unsigned char ch) {
		convertBuffer[5] |= (ch & 0x7E) >> 1;
		convertBuffer[6] = (ch & 0x1) << 7;
	}
	void sevenBit7(unsigned char ch) {
		convertBuffer[6] |= (ch & 0x7F) >> 0;
	}
};
// add by qi up 

ATCommandWarp::ATCommandWarp()
{
	m_pCom = NULL;
	InitializeCriticalSection(&m_csAT);   
}

ATCommandWarp::~ATCommandWarp()
{
	if (bInit)
	{
		Off();
	}
	DeleteCriticalSection(&m_csAT); 
}

ATCommandWarp::ATCommandWarp(const ATCommandWarp& ATWarp)
{

}

//ATCommandWarp* ATCommandWarp::GetATCommandWarp()
//{
//	static ATCommandWarp* at = 0;
//	if (at == 0)
//	{
//		at = new ATCommandWarp();
//	}
//	return at;
//}

void ATCommandWarp::Bind(Util::ComWarp* com)
{
	m_pCom = com;
}
int ATCommandWarp::Connect(char* pin)
{
	if (!bInit)
	{
		int i = Init();
		if (i == 1)
		{
			int o = On(pin);
			if (o == 1)
			{
				bInit = true;
			}
			return o;
		}
			
		return 0;
	}	
	return 1;
}

bool ATCommandWarp::Command(char* cmd, int len, char* out, int count)
{
	if(!m_pCom)
		return false;

	char ans[1024] = {0};      // Ӧ�� 
	int ansLen = 1024;
	bool result = false;
// 	m_pCom->WriteComm(cmd, len);   
// 	m_pCom->ReadComm(ans, 1024);   
//	Transaction(cmd, len, ans, ansLen);
	for (int i = 0; i < count; ++i)
	{
		if (strstr(ans, "OK") == NULL)
		{
			m_pCom->WriteComm(cmd, len);  
		//	if(count != 1)              //lxz 20090825
		//	    Sleep(i * 1000);
			int len = m_pCom->ReadComm(ans, 1024);     
			
			if (out != NULL)
			{
				memcpy(out, ans, len);     
				out[len] = '\0';
			}
// 			Sleep(1000);
// 			Transaction(cmd, len, ans, ansLen);
		}
		else
		{
			result = true;
			break;
		}
	}
	return result;
}

bool ATCommandWarp::Command_1(char* cmd, int len)
{
	if(!m_pCom)
		return false;
	
	bool result = true;
	m_pCom->WriteComm(cmd, len);  
	return result;
}

int ATCommandWarp::Transaction(char* req, int reqLen, char* res, int resLen)
{
	if(!m_pCom)
		return false;
	
	EnterCriticalSection(&m_csAT);
	int dwNumWrite = 0;
	while (dwNumWrite != reqLen)
	{
		dwNumWrite += m_pCom->WriteComm(req + dwNumWrite, reqLen - dwNumWrite);
	}

	int dwNumRead = 0;
	int nnCount = 0;
	while (true)
	{
		int count = m_pCom->ReadComm(res + dwNumRead, resLen - dwNumRead);
		dwNumRead += count;
		if ((nnCount > 10) ||((dwNumRead > 0) && (count == 0)) || (strstr(res, "OK") != NULL) || (strstr(res, "ERROR") != NULL) || (strstr(res, "\r\n") != NULL))    //
		{
			if(nnCount > 10)
				Dprintf("CountOut %d\r", nnCount);
			break;
		}
		nnCount++;
	}
	LeaveCriticalSection(&m_csAT);  
	return dwNumRead;
}

int ATCommandWarp::Init(void)
{
	char AT[] = "AT\r";// ����GSM-MODEM�Ĵ�����
	char ATZ[] = "ATZ\r";//��λ
	char ATE[] = "ATE0\r";// ECHO OFF  
	char CGMR[] = "AT+CGMR\r";
	char CMGF[] = "AT+CMGF=0\r";// PDUģʽ
	char CSCS[] = "AT+CSCS=\"UCS2\"\r";
	char CPMS[] = "AT+CPMS=\"SM\",\"SM\",\"SM\"\r";
	char CMNI[] = "AT+CNMI=2,1,0,0,0\r";
	char ATV[] = "ATV1\r";//1 <CR><LF><verbose code><CR><LF> 0 <numeric code><CR>	"ATV0\r";���ء�0\r����ATVָ���������result code�����ָ�ʽ�����ַ���ʽ��ATV0 ����Ϊ���ָ�ʽ������0 ��ʾOK��1 ��ʾCONNECT��2 ��ʾRING��3 ��ʾNO CARRIER��4 ��ʾERROR
	char DUSBDEG1[] = "AT^DUSBDEG=1,1\r";//���ء�0\r��,����ָ��Ϊ����EP1 ͨ�ſڣ���һ��������1-4 ��ӦEP1-EP4���ڶ�������1 ��ʾopen��0 ��ʾclose��������ĳ	��ͨ�ſ���ҵ��֮ǰ��ͨ����ָ�����Ӧͨ�ſڣ�
	char DUSBDEG2[] = "AT^DUSBDEG=2,1\r";//���ء�0\r��,����ָ��Ϊ����EP1 ͨ�ſڣ���һ��������1-4 ��ӦEP1-EP4���ڶ�������1 ��ʾopen��0 ��ʾclose��������ĳ	��ͨ�ſ���ҵ��֮ǰ��ͨ����ָ�����Ӧͨ�ſڣ�
	char DUSBDEG3[] = "AT^DUSBDEG=3,1\r";//���ء�0\r��,����ָ��Ϊ����EP1 ͨ�ſڣ���һ��������1-4 ��ӦEP1-EP4���ڶ�������1 ��ʾopen��0 ��ʾclose��������ĳ	��ͨ�ſ���ҵ��֮ǰ��ͨ����ָ�����Ӧͨ�ſڣ�
	char DUSBDEG4[] = "AT^DUSBDEG=4,1\r";//���ء�0\r��,����ָ��Ϊ����EP1 ͨ�ſڣ���һ��������1-4 ��ӦEP1-EP4���ڶ�������1 ��ʾopen��0 ��ʾclose��������ĳ	��ͨ�ſ���ҵ��֮ǰ��ͨ����ָ�����Ӧͨ�ſڣ�
	char DGPIOSM[] = "AT^DGPIOSM=1\r";//r�������ء�0\r����, ��ָ��ΪAPP��ģ�黽��ģʽѡ��
	char DSLP[] = "AT^DSLP=0,0\r";
	char DSQ[] = "AT^DSQ=1\r";//���ء�0\r��,����ָ��Ϊ���������źŷֱ�ֵ���ϱ�����
	char CRC[] = "AT+CRC=1\r";//���ء�0\r��,����ָ��Ϊ������������չָʾ����ѡ	����ָ���
	char DSCI[] = "AT^DSCI=1\r";//�����ء�0\r��,����ָ��Ϊ��������ָʾ����ѡ����ָ���
	char DCPI[] = "AT^DCPI=1\r";//���ء�0\r��, ����ָ��Ϊ��������״ָ̬ʾ���Բ�����	���绰10086 Ϊ�����������ϱ�ָʾ��������DCPI ���ϱ�ָʾ��
	//��^DCPI:1,0,0,1��Ϊ��������ָʾ����^DCPI:1,3,0,1�� Ϊ���ں���ָʾ��
	//��^DCPI:1,4,0,1�� Ϊ�ŵ�����ָʾ����^DCPI:1,5,0,1�� Ϊ��������ָʾ��
	//��^DCPI:1,2,0,1��Ϊ��������ָʾ����^DCPI:1,6,0,1��Ϊ���н���ָʾ����ѡ����ָ���

	char DVTTYPE[] = "AT^DVTTYPE=3\r";
	char DVTABLE[] = "AT^DVTABLE=1,1,0\r";

	char DAUDSCE[] = "AT^DAUDSCE=?\r";
	char DAUDSCE0[] = "AT^DAUDSCE=0\r";
	char DAUDSCE1[] = "AT^DAUDSCE=1\r";
	char DAUDSCS[] = "AT^DAUDSCS?\r";

	bool result = true;
	char ans[1024] = {0};      // Ӧ��    

 	if (!Command(AT, strlen(AT)))
	{
		return 0;
	}

	Command(ATZ, strlen(ATZ));
	
	if (Command(ATE, strlen(ATE))
		&& Command(CGMR, strlen(CGMR))
		&& Command(DUSBDEG1, strlen(DUSBDEG1))
		&& Command(DUSBDEG2, strlen(DUSBDEG2))
		&& Command(DUSBDEG3, strlen(DUSBDEG3))
		&& Command(DUSBDEG4, strlen(DUSBDEG4))
		&& Command(DGPIOSM, strlen(DGPIOSM))

//		&& Command(DSLP, strlen(DSLP))
//		&& Command(DSQ, strlen(DSQ))

		//��Ƶ�绰
		&& Command(CRC, strlen(CRC))
		&& Command(DSCI, strlen(DSCI))
		&& Command(DCPI, strlen(DCPI))
//		&& Command(DVTTYPE, strlen(DVTTYPE))
//		&& Command(DVTABLE, strlen(DVTABLE))

//		&& Command(DSQ, strlen(DSQ))
// 		&& Command(CMGF, strlen(CMGF))
// 		&& Command(CSCS, strlen(CSCS))
// 		&& Command(CPMS, strlen(CPMS))
// 		&& Command(CMNI, strlen(CMNI))
//		&& Command(DSLP, strlen(DSLP))
//		&& Command(DAUDSCE, strlen(DAUDSCE))
// 		&& Command(DAUDSCE0, strlen(DAUDSCE0))
//		&& Command(DAUDSCE1, strlen(DAUDSCE1))
//		&& Command(DAUDSCS, strlen(DAUDSCS))
		)
	{
		return 1;
	}
	return 0;
}

int ATCommandWarp::On(char* pin)
{
	char CREG1[] = "AT+CREG=1\r";//���ء�0\r������ָ��Ϊ���������ϱ�����ѡ����ָ���
	char CREG0[] = "AT+CREG=0\r";//���ء�0\r������ָ��Ϊ���������ϱ�����ѡ����ָ���
	char CMER[] = "AT+CMER=2,0,0,2\r";//r�������ء�0\r������ָ��Ϊ�����߲�Э��ջ�¼�CIEV��	<ind>,<value>�ϱ�����
	char CFUN5[] = "AT+CFUN=5\r";//���ء�0\r��������ָ��Ϊ����USIM ������
	char CFUN1[] = "AT+CFUN=1\r";//�����ء�0\r��������ָ��Ϊ����Э��ջ����
	char COPS[] = "AT+COPS=0\r";//���ء�0\r����(��ָ��Ϊ����ָ��),�˺�LC6311 �᷵
	//�غܶ����ݣ�ֱ�����ء�+CREG��1����ʾ�����ɹ�����^DACTI:2����ʾ��ǰ���뼼����TD; ��+CIEV��2,2����ʾ�źŵȼ�Ϊ2 ������+CIEV��8,0����ʾ��
	//��Ϣ�洢δ������+CREG��2����ʾ�����У���+CREG��1����ʾ�����ɹ���ע
	//��ɹ�����˵������+CREG:n�����С�n����ֵ��Ϊ0[δע��]��1[ע��ɹ�]��2[����]��3[����ܾ�ע��]��4[����ע��״̬δ֪]��5[����]��������ֻ�е�CREG
	//���ص���1 ��5 ʱ��������LC6311 ��֧�ֵ�ҵ�񡿣����ڶ�CREG��4 ��Э��
	//��⣬���鵱+CREG��4 ʱ��APP ��������UI ��ʾΪ���ṩ����״̬������
	//ͬ��+CREG��1��+CREG:5������

	char CMGF[] = "AT+CMGF=0\r";// PDUģʽ
	char CSCS[] = "AT+CSCS=\"GSM\"\r";//UCS2//
	char CPMS[] = "AT+CPMS=\"SM\",\"SM\",\"SM\"\r";
	char CMNI[] = "AT+CNMI=2,1,0,0,0\r";

	char CPIN[] = "AT+CPIN?\r"; //PIN ���� �˴������ж��Ƿ����SIM��
	char CNUM[] = "AT+CNUM\r";//���ֻ�����
	char CSCA[] = "AT+CSCA?\r";//���������ĺ���


	bool result = true;
	char ans[1024] = {0};      // Ӧ��    
	
	if (!Command(CREG1, strlen(CREG1)))
	{
		return 0;
	}

//	Command(CMER, strlen(CMER));
	memset(ans, 0, 1024);

//	if (!Command(CFUN5, strlen(CFUN5), ans)) // old
	if (!CommandFun5(CFUN5, strlen(CFUN5), ans))//add by qi 2009_08_28
	{	
		if (strstr(ans, "+CME ERROR: 11") != NULL || strstr(ans, "+CME ERROR: 12") != NULL )//�����ÿ���PIN ��ʱ��APP�����롰AT+CFUN=5��������USIM �������صġ���CME		ERROR��11����ʾ��SIM PIN Required������ʱAPP �˻ᵯ����ʾ����pin�����ʾ��		APP������PIN �����ִ�м���Э��ջ��ָ�
		{
			CheckPin();
			//	test
			//	TestCommand();

			/*	char cmd[128] = {0};
				sprintf(cmd, "AT+CPIN=\"%s\"\r", pin);
				if (!Command(cmd, strlen(cmd)))
				{
					return 2;
				}
			*/

		}
		else
		{
			return 3;
		}
	}
	
	// 6311  lxz 20090807
#define TD6311 1
#if(TD6311 == 1)
	if(!(Command(CFUN1, strlen(CFUN1))
		&& Command(COPS, strlen(COPS))))
	{
		return 0;
	}
#else
	if(!(Command(CFUN1, strlen(CFUN1))))
	{
		return 0;
	}
#endif
	memset(ans, 0, 1024);
	m_pCom->ReadComm(ans, 1024);
 	while ((strstr(ans, "+CREG: 1") == NULL) && (strstr(ans, "+CREG: 5") == NULL))
	{
		m_pCom->ReadComm(ans, 1024);
	}

	PhoneVolume(3);

	if (!(Command(CREG0, strlen(CREG0))
		&& Command(CMGF, strlen(CMGF))
		&& Command(CSCS, strlen(CSCS))
		&& Command(CPMS, strlen(CPMS))
		&& Command(CMNI, strlen(CMNI))))
	{
		return 0;
	}
	memset(ans, 0, 1024);
	if (Command(CSCA, strlen(CSCA), ans))
	{
		std::string addr = ans;
		unsigned int pos1 = addr.find("\"");
		if (pos1 != std::string::npos)
		{
			unsigned int pos2 = addr.find("\"", pos1 + 1);
			if (pos2 != std::string::npos)
			{
				pos1 += 1;
				if (pos2 - pos1 > 0)
				{
					m_strSmsCentreAddress = addr.substr(pos1, pos2 - pos1);
				}
			}
		}	
	}

	memset(ans, 0, 1024);
	if (Command(CNUM, strlen(CNUM), ans))
	{
		std::string number = ans;
		unsigned int pos1 = number.find(",");
		if (pos1 != std::string::npos)
		{
			unsigned int pos2 = number.find(",", pos1 + 1);
			if (pos2 != std::string::npos)
			{
				pos1 += 2;
				pos2 -= 1;
				if (pos2 - pos1 > 0)
				{
					m_strPhoneNumber = number.substr(pos1, pos2 - pos1);
				}
			}
		}
	}
	return 1;
}

bool ATCommandWarp::Off(void)
{
	char COPS[] = "AT+COPS=2\r";//���ء�+CREG: 0\r\n0\r������ָ��Ϊȥ�������磩��
	char CFUN0[] = "AT+CFUN=0\r";//���ء�0\r������ָ��Ϊȥ����Э��ջ����
	char CFUN6[] = "AT+CFUN=6\r";//���ء�0\r������ָ��Ϊȥ����USIM ������
	char DSOFF[] = "AT^DSOFF\r";//�����ء�0\r������ָ��Ϊģ����ػ�����

	bool result = true;
	char ans[1024] = {0};      // Ӧ��  

	if (Command(COPS, strlen(COPS))
		&& Command(CFUN0, strlen(CFUN0))
		&& Command(CFUN6, strlen(CFUN6))
		&& Command(DSOFF, strlen(DSOFF))
		)
	{
		return true;
	}
	return false;
}

bool ATCommandWarp::PhoneDial(char * number)
{
	char ATD[] = "ATD";
	char CMD[1024];
	strcpy(CMD, ATD);
	strcat(CMD, number);
	strcat(CMD, ";\r");
	bool result = true;
	char ans[1024] = {0};      // Ӧ�� 
	int ansLen = 1024;
// 	m_pCom->WriteComm(CMD, strlen(CMD));
// 	//	m_pCom->ReadComm(ans, 1024);
// 	//	result = *ans;
	Transaction(CMD, strlen(CMD), ans, ansLen);
	return result;	
}

bool ATCommandWarp::PhoneRedial(void)
{
	char ATDL[] = "ATDL;\r";
	bool result = true;
	char ans[1024] = {0};      // Ӧ�� 
	int ansLen = 1024;
// 	m_pCom->WriteComm(ATDL, strlen(ATDL));
// 	m_pCom->ReadComm(ans, 1024);
	Transaction(ATDL, strlen(ATDL), ans, ansLen);
	return result;	
}

int ATCommandWarp::PhoneState(void)
{
	char CLCC[] = "AT+CLCC\r";
	char result = -1;
	char ans[1024] = {0};      // Ӧ�� 
	int ansLen = 1024;
// 	m_pCom->WriteComm(CLCC, strlen(CLCC));
// 	m_pCom->ReadComm(ans, 1024);
	Transaction(CLCC, strlen(CLCC), ans, ansLen);

	if (strstr(ans, "NO DIALTONE") != NULL)
	{
		return 10;//TRACE("NO DIALTONE\n");
	}
	else if (strstr(ans, "BUSY") != NULL)
	{
		return 11;//TRACE("BUSY\n");
	}
	else if (strstr(ans, "NO ANSWER") != NULL)
	{
		return 12;//TRACE("NO ANSWER\n");
	}
	else if (strstr(ans, "NO CARRIER") != NULL)
	{
		return 13;//TRACE("NO CARRIER\n");
	}
	else if (strstr(ans, "CONNECT") != NULL)
	{
		return 14;//TRACE("CONNECT\n");
	}
	else if (strstr(ans, "NW CONGESTION") != NULL)
	{
		return 15;//TRACE("CONNECT\n");
	}
	else if (strstr(ans, "ERROR ODB") != NULL)
	{
		return 16;//TRACE("ERROR ODB\n");
	}
	else
	{
		//<idx>: integer type; call identification number as described in 3GPP TS	22.030 subclause 4.5.5.1; this number can be used in +CHLD	command operations
		//<dir>:
		//  0 mobile originated (MO) call
		//	1 mobile terminated (MT) call
		//<stat> (state of the call):
		//  0 active
		//	1 held
		//	2 dialing (MO call)
		//	3 alerting (MO call)
		//	4 incoming (MT call)
		//	5 waiting (MT call)
		//<mode> (bearer/teleservice):
		//  0 voice
		//	1 data
		//	2 fax
		//	3 voice followed by data, voice mode
		//	4 alternating voice/data, voice mode
		//	5 alternating voice/fax, voice mode
		//	6 voice followed by data, data mode
		//	7 alternating voice/data, data mode
		//	8 alternating voice/fax, fax mode
		//	9 unknown
		//<mpty>:
		//  0 call is not one of multiparty (conference) call parties
		//	1 call is one of multiparty (conference) call parties
		char* p = strstr(ans, "+CLCC:");
		if (p != NULL)
		{
			p += 9;//+CLCC: 1,0,2,0,0,"10086",129\r\n
			if ((strncmp(p, "0,0", 3) == 0) || (strncmp(p, "1,0", 3) == 0))
			{
				result = 0;//active (connect)
			}
			else if ((strncmp(p, "0,1", 3) == 0) || (strncmp(p, "1,1", 3) == 0))
			{
				result = 1;//held
			}
			else if (strncmp(p, "0,2", 3) == 0)
			{
				result = 2;//dialing
			}
			else if (strncmp(p, "0,3", 3) == 0)
			{
				result = 3;//alerting
			}
			else if (strncmp(p, "1,4", 3) == 0)
			{
				result = 4;//incoming
			}		
			else if (strncmp(p, "1,5", 3) == 0)
			{
				result = 5;//waiting
			}
		}
		else if (strcmp(ans, "\r\nOK\r\n") == 0)//memcmp(ans, "\r\nOK\r\n", strlen("\r\nOK\r\n"))
		{
			//result = 6;
		}
	}
	return result;	
}

bool ATCommandWarp::PhoneRing(char * number, int * type)
{
	char CLCC[] = "AT+CLCC\r";//ע�⣺������Ϣ�������ϱ��ģ����ﻹ��Ҫ�޸ġ� +CRING: VOICE\r\n+CLIP:"10086",161,"",,"",0\r\n
	bool result = false;
	char ans[1024] = {0};      // Ӧ�� 
	int ansLen = 1024;
// 	m_pCom->WriteComm(CLCC, strlen(CLCC));
// 	m_pCom->ReadComm(ans, 1024);
	Transaction(CLCC, strlen(CLCC), ans, ansLen);

	char* p = strstr(ans, "+CLIP:");
	if ((strstr(ans, "RING") != NULL) && (p != NULL))
	{
		p += 8;//�ƶ���+CLIP:"֮��
		while (*p != '\"')
		{
			*number = *p;
			number++;
			p++;
		}
		*number = '\0';

		p += 2;
		*type = atoi(p);
		result = true;
	}
	else
	{
		char* p = strstr(ans, "+CLCC:");
		if (p != NULL)
		{
			p += 9;//+CLCC: 1,0,2,0,0,"10086",129\r\n
			if (strncmp(p, "1,4", 3) == 0)
			{
				p += 9;
				while (*p != '\"')
				{
					*number = *p;
					number++;
					p++;
				}
				*number = '\0';

				p += 2;
				*type = atoi(p);
				result = true;
			}
		}
	}


	//Execution command returns the activity status <pas> of the MT. It can	be used tointerrogate the MT before requesting action from the phone.
	//char CPAS[] = "AT+CPAS\r";
	//char result = 0;
	//char ans[1024];      // Ӧ��    
	//m_pCom->WriteComm(CPAS, strlen(CPAS));
	//m_pCom->ReadComm(ans, 1024);
	//if (strstr(ans, "+CPAS:3") != NULL)
	//{
	//}
	return result;
}

bool ATCommandWarp::PhoneHangup(void)
{
	char ATH[] = "ATH\r";//���ڹҶ�ATָ�AT��CHUP���Ҷϵ�ǰ����CS��·����AT��CHLD Ҳ����Ϊ��ѡATָ������Ҷϣ���LC6311 ���APP �ϱ���3\r����3 ��ʾ��NO CARRIER��,Ϊ�Զ������Ҷ�
	bool result = true;
	char ans[1024] = {0};      // Ӧ�� 
	int ansLen = 1024;
// 	m_pCom->WriteComm(ATH, strlen(ATH));
// 	m_pCom->ReadComm(ans, 1024);
	Transaction(ATH, strlen(ATH), ans, ansLen);
	return result;	
}

bool ATCommandWarp::PhoneAnswer(void)
{
	char ATA[] = "ATA\r";//���ڹҶ�ATָ�AT��CHUP���Ҷϵ�ǰ����CS��·����AT��CHLD Ҳ����Ϊ��ѡATָ������Ҷϣ���LC6311 ���APP �ϱ���3\r����3 ��ʾ��NO CARRIER��,Ϊ�Զ������Ҷ�
	bool result = true;
	char ans[1024] = {0};      // Ӧ�� 
	int ansLen = 1024;
// 	m_pCom->WriteComm(ATA, strlen(ATA));
// 	m_pCom->ReadComm(ans, 1024);
	Transaction(ATA, strlen(ATA), ans, ansLen);
	return result;	
}
bool ATCommandWarp::PhoneSubDial(char number)
{
	char cmd[16];       // ���    
	sprintf(cmd, "AT+VTS=%c\r", number); // ��������    
	char ans[1024] = {0};      // Ӧ�� 
	int ansLen = 1024;
// 	m_pCom->WriteComm(cmd, strlen(cmd));
// 	//m_pCom->ReadComm(ans, 1024);
	Transaction(cmd, strlen(cmd), ans, ansLen);
	//if(strstr(ans, "OK\r\n") != NULL)   
	{
		return true;
	}
	return false;
}


//�绰����
bool ATCommandWarp::PhoneSetTrans(PhoneCallTRans calltrans)     //���ú���ת��
{
	return true;
}

bool ATCommandWarp::PhoneGetTrans(PhoneCallTRans &calltrans)	 //��ȡ����ת������
{
	char cmd[16];       // ���    
	for(int i = 0; i < 4; i++)
	{
		sprintf(cmd, "AT+CCFC=%d,2?\r", i); // ��������    
		char ans[1024] = {0};      // Ӧ�� 
		int ansLen = 1024;

		extern Util::ATCommandWarp* GetATCommandWarp();
		ATCommandWarp *pATCommanWarp = GetATCommandWarp();
		pATCommanWarp->Transaction(cmd, strlen(cmd), ans, ansLen);
		if(strstr(ans, "OK\r\n") != NULL)   
		{
			;
		}

	}
	
	return false;
}

bool ATCommandWarp::PhoneSetCallLimit(PhoneCallLimit calllimit)    //���ú�������
{
	return true;
}

bool ATCommandWarp::PhoneGetCallLimit(PhoneCallLimit &calllimit)    //��ȡ������������
{
	return true;
}

bool ATCommandWarp::PhoneSetCallWaiting(PhoneCallWaiting callwaiting)		//���ú��еȴ�
{
	return true;
}

bool ATCommandWarp::PhoneGetCallWaiting(PhoneCallWaiting &callwaiting)		//��ȡ���еȴ�����
{
	return true;
}

bool ATCommandWarp::PhoneVolume(unsigned int level)
{
	char cmd1[16];       // ���    
	char cmd2[16];       // ���    
	char cmd3[16];       // ���    
	sprintf(cmd1, "AT^DAUDO=1,%d\r", level); // �������� 
	sprintf(cmd2, "AT^DAUDO=2,%d\r", level); // �������� 
	sprintf(cmd3, "AT^DAUDO=3,%d\r", level); // �������� 
	
	char ans[1024] = {0};      // Ӧ�� 
	int ansLen = 1024;
	// 	m_pCom->WriteComm(cmd, strlen(cmd));
	// 	//m_pCom->ReadComm(ans, 1024);
	Transaction(cmd1, strlen(cmd1), ans, ansLen);
	Transaction(cmd2, strlen(cmd2), ans, ansLen);
	//if(strstr(ans, "OK\r\n") != NULL)   
	{
		return true;
	}
	return false;
}
bool ATCommandWarp::PhoneMute(bool isMute)
{
	int mute = 0;
	if (isMute)
	{
		mute = 1;
	}
	else
	{
		mute = 0;
	}
	char cmd[16];       // ���    
	sprintf(cmd, "AT+CMUT=%d\r", mute); // ��������    
	char ans[1024] = {0};      // Ӧ�� 
	int ansLen = 1024;
	// 	m_pCom->WriteComm(cmd, strlen(cmd));
	// 	//m_pCom->ReadComm(ans, 1024);
	Transaction(cmd, strlen(cmd), ans, ansLen);
	//if(strstr(ans, "OK\r\n") != NULL)   
	{
		return true;
	}
	return false;
}
bool ATCommandWarp::PhoneAutoAnswer(unsigned int second)
{
	char cmd[16];       // ���    
	sprintf(cmd, "ATS0=%d\r", second); // ��������    
	char ans[1024] = {0};      // Ӧ�� 
	int ansLen = 1024;
	// 	m_pCom->WriteComm(cmd, strlen(cmd));
	// 	//m_pCom->ReadComm(ans, 1024);
	Transaction(cmd, strlen(cmd), ans, ansLen);
	//if(strstr(ans, "OK\r\n") != NULL)   
	{
		return true;
	}
	return false;
}

unsigned int ATCommandWarp::PhoneNettype(void)			//LC6211
{
	char cmd[] = "AT^DACTI?\r";       // ���    
	unsigned int result = 0;
	char ans[1024] = {0};      // Ӧ�� 
	int ansLen = 1024;
	// 	m_pCom->WriteComm(cmd, strlen(cmd));
	// 	m_pCom->ReadComm(ans, 1024);
	Transaction(cmd, strlen(cmd), ans, ansLen);
	if(strstr(ans, "OK\r\n") != NULL)   
	{
		char* p = strstr(ans, "^DACTI: ");//
		if (p != NULL)
		{
			p += 8;

			char *p1 = strstr(ans, ",");
			char i[4] = {0};
			if(p1 != NULL)
			{
				memcpy(i, p1+1, 1);
				result = atoi(i);
			}
			else
				result = atoi(p);
		}
	}
	return result;
}

void ATCommandWarp::PhoneDialTone(BOOL isOn, char *tone)
{
	char AT[64] = {0};
	bool result = true;
	char ans[1024] = {0};     
	int ansLen = 1024;
	if(!isOn)
	{
		strcpy(AT, "AT^DAUDCTRL=0\r");
		Command_1(AT, strlen(AT));
	}
	else
	{
	//	if(strlen(tone) < 2)
		{
			PhoneDialTone(0, NULL);
		}
		strcpy(AT, "AT^DAUDCTRL=1,\"");
		strcat(AT, tone);
		strcat(AT, "\"\r");
		Command_1(AT, strlen(AT));
	}
}

unsigned int ATCommandWarp::PhoneSignalQuality(void)
{
	char cmd[] = "AT+CSQ\r";       // ���    
	unsigned int result = 0;
	char ans[1024] = {0};      // Ӧ�� 
	int ansLen = 1024;
// 	m_pCom->WriteComm(cmd, strlen(cmd));
// 	m_pCom->ReadComm(ans, 1024);
	Transaction(cmd, strlen(cmd), ans, ansLen);
	if(strstr(ans, "OK\r\n") != NULL)   
	{
		char* p = strstr(ans, "+CSQ: ");//+CSQ: 129,99
		if (p != NULL)
		{
			p += 6;
			result = atoi(p);
			if (result == 99)
			{
				result = 0;
			}
			else
			{
				result = result - 100 - 116 + 113;
				result = (result + 1) / 2;
				if (result < 8)
				{
					result = 1;
				}
				else
				{
					result = (result - 8) / 6 + 2;
				}
				if (result > 5)
				{
					result = 5;
				}
			}
		}
	}
	return result;
}

bool ATCommandWarp::PhoneHandFree(bool isHandFree)
{
	int hf = 0;
	if (isHandFree)
	{
		hf = 2;
	}
	else
	{
		hf = 0;
	}
	char cmd[16];       // ���    
	sprintf(cmd, "AT^DAUDSCS=%d\r", hf); // ��������    
	char ans[1024] = {0};      // Ӧ�� 
	int ansLen = 1024;
// 	m_pCom->WriteComm(cmd, strlen(cmd));
// 	//m_pCom->ReadComm(ans, 1024);
	Transaction(cmd, strlen(cmd), ans, ansLen);
	//if(strstr(ans, "OK\r\n") != NULL)   
	{
		return true;
	}
	return false;
}

std::string ATCommandWarp::PhoneNumber(void)
{
	return m_strPhoneNumber;
}

bool ATCommandWarp::SmsSend(int dataLength)
{
	char cmd[16];       // ���    
	sprintf(cmd, "AT+CMGS=%d\r", dataLength); // ��������    
	int nLength;        // �����յ������ݳ���    
	char ans[1024] = {0};      // Ӧ�� 
	int ansLen = 1024;
// 	m_pCom->WriteComm(cmd, strlen(cmd));
// 	nLength = m_pCom->ReadComm(ans, 1024);
	nLength = Transaction(cmd, strlen(cmd), ans, ansLen);
// 	if(nLength == 4 && strncmp(ans, "\r\n> ", 4) == 0)   
//	if ((strncmp(ans, "\r\n> ", 4) == 0) || (strstr(ans, "OK\r\n") != NULL))
	{
		return true;
	}
	return false;
}
int ATCommandWarp::SmsSend(char* pdu, int pduLength, char* ans, int ansLength)
{
	//return m_pCom->WriteComm(pdu, pduLength);
	return Transaction(pdu, pduLength, ans, ansLength);
}
int ATCommandWarp::SmsDelete(int index, char* ans, int ansLength)
{
	char cmd[16];       // ���    
	sprintf(cmd, "AT+CMGD=%d\r", index);    // ��������    
//	return m_pCom->WriteComm(cmd, strlen(cmd)); 
	return Transaction(cmd, strlen(cmd), ans, ansLength);
}
int ATCommandWarp::SmsReadList(char* ans, int ansLength)
{
	char cmd[] = "AT+CMGL=4\r";
//	return m_pCom->WriteComm(cmd, strlen(cmd));
	return Transaction(cmd, strlen(cmd), ans, ansLength);
}
int ATCommandWarp::SmsReadResponse(void* pData, int nLength)
{
	return m_pCom->ReadComm(pData, nLength);
}
int ATCommandWarp::SmsWriteRequest(void* pData, int nLength)
{
	return m_pCom->WriteComm(pData, nLength);
}

std::string ATCommandWarp::SmsCenterAddress(void)
{
	return m_strSmsCentreAddress;
}
bool ATCommandWarp::SmsCenterAddress(std::string sca)
{
	char cmd[64] = {0};
	sprintf(cmd, "AT+CSCA=\"%s\",145\r", sca.c_str());
	char ans[1024] = {0};      // Ӧ�� 
	int ansLen = 1024;
	int nLength = Transaction(cmd, strlen(cmd), ans, ansLen);
	return true;
}

// add by qi 2009_07_22
void ATCommandWarp::CheckPin()
{
	char PIN[] = "AT+CPIN?\r" ;//��ѯsim��״̬
	char answer[1024];
	memset(answer,0,1024);
	if (Command(PIN,strlen(PIN),answer))
	{
		if (strstr(answer,"+CPIN: SIM PIN") !=NULL)//��Ҫpin1��
		{	
			HandlePin();
		}
		else if (strstr(answer,"+CPIN: SIM PUK") !=NULL)//��ҪPUK1��
		{	
			HandlePuk();
		}
	}
}
void ATCommandWarp::HandlePin()
{
	char ans[1024];//Ӧ����Ϣ
	memset(ans,0,1024);
	PostMessage(theApp.m_pMainWnd->m_hWnd,WM_PIN_INPUT,0,0);
	while (strstr(ans,"+CME ERROR: 12") == NULL)//��������pin���������sim������ס
	{	
		
		while(strstr(ans,"OK") == NULL && strstr(ans,"ERROR") == NULL )//�ȴ��û�����pin��
		{	
			Sleep(100);
			m_pCom->ReadComm(ans,1024);	
		}		
		if (strstr(ans,"OK") != NULL)//����pin����ȷ
		{	
//			PostMessage(theApp.m_pMainWnd->m_hWnd,WM_PIN_INPUT,PIN_OK,0);
			return ;	
		}
		
		if ( strstr(ans,"ERROR") != NULL)//����pin�����
		{	
			if (strstr(ans,"+CME ERROR: 12") == NULL)
			{
				PostMessage(theApp.m_pMainWnd->m_hWnd,WM_PIN_INPUT,PIN_ERROR,0);
				memset(ans, 0, 1024);
			}
		}
		
	}
	
	if (strstr(ans,"+CME ERROR: 12")!= NULL)//��Ҫ����PUK��
	{
		HandlePuk();
	}		
	
}
void ATCommandWarp::HandlePuk()
{
	int num = 0 ;
	PostMessage(theApp.m_pMainWnd->m_hWnd,WM_PIN_INPUT,PIN_PUK,0);	
	while ( num != 10)//������PUK���ۼ�10�Σ�����SIM������������ס��SIM���ϣ���Ҫ���»�����
	{			
		char ans[1024];
		memset(ans,0,1024);
		while(strstr(ans,"OK") == NULL && strstr(ans,"ERROR")== NULL )//�ȴ��û�����pin��
		{	
			Sleep(100);
			m_pCom->ReadComm(ans,1024);	
		}		
		if (strstr(ans,"OK") != NULL)//����PUK����ȷ
		{	
		//	PostMessage(theApp.m_pMainWnd->m_hWnd,WM_PIN_INPUT,PIN_OK,0);
			return ;	
		}		
		if ( strstr(ans,"ERROR") != NULL)//����PUK�����
		{
			if (num > 0 && num < 9)
			{	
				num++;
				PostMessage(theApp.m_pMainWnd->m_hWnd,WM_PIN_INPUT,PIN_FUK_ERROR,0);
				memset(ans, 0, 1024);
			}
		}		
	}
	
	if (10 == num)//10���������
	{			
		PostMessage(theApp.m_pMainWnd->m_hWnd,WM_PIN_INPUT,PIN_FUK_LOCKED,0);		
	}

}
bool ATCommandWarp::CommandFun5(char* cmd, int len, char* out /* = NULL */, int count /* = 5 */)
{
	char ans[1024] = {0};// Ӧ�� 
	int ansLen = 1024;
	bool result = false;

	for (int i = 0; i < count; ++i)
	{
		m_pCom->WriteComm(cmd, len);  
		Sleep(i * 1000);
		int len = m_pCom->ReadComm(ans, 1024);     
		
		if (out != NULL)
		{
			memcpy(out, ans, len);     
			out[len] = '\0';
		}

		if (strstr(ans,"+CME ERROR: 11") != NULL && strstr(ans,"OK") != NULL)//��ҪPIN��
		{
			result = false ;
			break;
		}
		else if (strstr(ans,"+CME ERROR: 12") != NULL && strstr(ans,"OK") != NULL)//��Ҫ����PUK��
		{
			result = false ;
			break;
		}
		else if (strstr(ans,"OK") != NULL)
		{
			result = true;
			break;
		}
	}
	return result;
}

bool ATCommandWarp::UnconditionalTransfer(const char *destphone,bool able)
{
	char CCFC[100];
	char ans[1024];
	memset(CCFC,0,100);
	if (able)
	{
		sprintf(CCFC,"AT+CCFC=0,%d\r",1);
	}
	else
	{
		sprintf(CCFC,"AT+CCFC=0,%d\r",0);
	}
	Transaction(CCFC,strlen(CCFC),ans,1024);
	if (strstr(ans,"OK") != NULL)
	{
		return true ;
	}
	return false ;
}
bool ATCommandWarp::BusyTransfer(const char *destphone,bool able)
{
	char CCFC[100];
	char ans[1024];
	memset(CCFC,0,100);
	if (able)
	{
		sprintf(CCFC,"AT+CCFC=1,%d\r",1);
	}
	else
	{
		sprintf(CCFC,"AT+CCFC=1,%d\r",0);
	}
	Transaction(CCFC,strlen(CCFC),ans,1024);
	if (strstr(ans,"OK") != NULL)
	{
		return true ;
	}
	return false ;
}
bool ATCommandWarp::NoReplyTransfer(const char *destphone,bool able)
{
	char CCFC[100];
	char ans[1024];
	memset(CCFC,0,100);
	if (able)
	{
		sprintf(CCFC,"AT+CCFC=2,%d\r",1);
	}
	else
	{
		sprintf(CCFC,"AT+CCFC=2,%d\r",0);
	}
	Transaction(CCFC,strlen(CCFC),ans,1024);
	if (strstr(ans,"OK") != NULL)
	{
		return true ;
	}
	return false ;
}
bool ATCommandWarp::NoReachTranfer(const char *destphone,bool able)
{
	char CCFC[100];
	char ans[1024];
	memset(CCFC,0,100);
	if (able)
	{
		sprintf(CCFC,"AT+CCFC=3,%d\r",1);
	}
	else
	{
		sprintf(CCFC,"AT+CCFC=3,%d\r",0);
	}
	Transaction(CCFC,strlen(CCFC),ans,1024);
	if (strstr(ans,"OK") != NULL)
	{
		return true ;
	}
	return false ;
}
void ATCommandWarp::InputPin(const char *pin)
{
	char PIN[100];
	memset(PIN,0,100);
	sprintf(PIN,"AT+CPIN=\"%s\"\r",pin);
	m_pCom->WriteComm(PIN,strlen(PIN));//����pin��
}

void ATCommandWarp::InputPuk(const char *puk,const char *newpin)
{
	char FUK[100];
	memset(FUK,0,100);
	sprintf(FUK,"AT+CPIN=\"%s\",\"%s\"\r",puk,newpin);
	m_pCom->WriteComm(FUK,strlen(FUK));//����sim����������µ�pin��
}
bool ATCommandWarp::InputPin2(const char *pin2)
{
	char PIN2[100];
	char ans[1024];
	memset(PIN2,0,100);
	memset(ans,0,1024);
	sprintf(PIN2,"AT+CPIN2=\"%s\"\r",pin2);
	Transaction(PIN2,strlen(PIN2),ans,1024);
	if (strstr(ans,"ERROR") != NULL)
	{
		return false ;
	}
	return true ;
}
bool ATCommandWarp::InputPuk2(const char *puk2,const char *newpin)
{
	char FUK2[100];
	char ans[1024];
	memset(FUK2,0,100);
	memset(ans,0,1024);
	sprintf(FUK2,"AT+CPIN2=\"%s\",\"%s\"\r",puk2,newpin);
	Transaction(FUK2,strlen(FUK2),ans,1024);
	if (strstr(ans,"ERROR") != NULL)
	{
		return false ;
	}
	return true ;
}
void ATCommandWarp::TestCommand()
{

// 	char CLCK[100];//SIM����
 	char ans[1024];
//	memset(CLCK,0,100);
	memset(ans,0,1024);
//	sprintf(CLCK,"AT+CLCK=\"SC\",1,\"%s\"\r","5678");
//	Command(CLCK,strlen(CLCK),ans);	
	
// 	char CPIN[] = "AT+CPIN?\r"; //PIN ���� �˴������ж��Ƿ����SIM��
// 	char ans[1024];
// 	memset(ans,0,100);
// 	Command(CPIN,strlen(CPIN),ans);	

// 	char FUK[100];
// 	memset(FUK,0,100);
// 	sprintf(FUK,"AT+CPIN=\"%s\",\"%s\"\r","04002585","5678");
// 	Command(FUK,strlen(FUK),ans);

//	����pin��
// 	char PIN[100];
// 	memset(PIN,0,100);
// 	memset(ans,0,100);
// 	sprintf(PIN,"AT+CPIN=\"%s\"\r","5678");
// 	Command(PIN,strlen(PIN),ans);

//  char CLCK[100];//sim����
//  memset(CLCK,0,100);
// 	memset(ans,0,1024);
//  sprintf(CLCK,"AT+CLCK=\"SC\",0,\"%s\"\r","5678");
//  Command(CLCK,strlen(CLCK),ans);

// 	memset(ans,0,100);
// 	Command(CPIN,strlen(CPIN),ans);	

// 	sprintf(FUK,"AT+CPIN=\"%s\",\"%s\"\r","04002585","5678");
//  Command(FUK,strlen(FUK),ans);

//  �޸�pin��
//	char PASSWORD[100];
//	memset(ans,0,1024);
//	memset(PASSWORD,0,100);
//	sprintf(PASSWORD,"AT+CPWD=\"SC\",\"%s\",\"%s\"\r","5678","1234");
//	Command(PASSWORD,strlen(PASSWORD),ans);

//	������ת�ƣ���ͨ������ת��
//  	char CCFC[100];
//  	memset(CCFC,0,100);
//  	sprintf(CCFC,"AT+CCFC=0,3,\"%s\",129\r","13601371701");	
//  	Command(CCFC,strlen(CCFC),ans);

	char CCFC[100];
	memset(CCFC,0,100);
	sprintf(CCFC,"AT+CCFC=0,1,\"%s\",129\r","13601371701");	
	Command(CCFC,strlen(CCFC),ans);


//	std::string s;
//	GetOperator(s);

//	char COPN[] = "AT+COPN\r";
//	memset(ans,0,100);
//	Command(COPN,strlen(COPN),ans);


}

bool ATCommandWarp::LockSim(const char *password)
{
	char CLCK[100];//SIM����
	char ans[1024];
	memset(CLCK,0,100);
	memset(ans,0,1024);
	sprintf(CLCK,"AT+CLCK=\"SC\",1,\"%s\"\r",password);
	Transaction(CLCK,strlen(CLCK),ans,1024);
	if (strstr(ans,"ERROR") != NULL)
	{	
		return false ;
	}
	return true ;
}
bool ATCommandWarp::UnlockSim(const char *password)
{
	char CLCK[100];//sim����
	char ans[1024];
	memset(CLCK,0,100);
	memset(ans,0,1024);
	sprintf(CLCK,"AT+CLCK=\"SC\",0,\"%s\"\r",password);
	Transaction(CLCK,strlen(CLCK),ans,1024);
	if (strstr(ans,"ERROR") != NULL)
	{	
		return false ;
	}
	return true ;
}
bool ATCommandWarp::GetSimStatus(int &status)
{
	char CLCK[] = "AT+CLCK=\"SC\",2\r";
	char ans[1024];
	memset(ans,0,1024);
	Transaction(CLCK,strlen(CLCK),ans,1024);
	if (strstr(ans,"OK") != NULL)
	{
		std::string s = ans ;
		int l ;
		l = s.find("+CLCK:");
		s = s.substr(l+7);
		status = atoi(s.c_str());
		return true ;
	}
	return false ;
}
bool ATCommandWarp::ChangePin(const char *pin,const char *newpin)
{	
	//ֻ������ס������²��ܸı�pin
	
	char PASSWORD[100];
	char ans[1024];
	memset(ans,0,1024);
	memset(PASSWORD,0,100);
	sprintf(PASSWORD,"AT+CPWD=\"SC\",\"%s\",\"%s\"\r",pin,newpin);
	Transaction(PASSWORD,strlen(PASSWORD),ans,1024);
	if (strstr(ans,"ERROR") != NULL)
	{
		return false ;
	}
	
	return true ;
}
bool ATCommandWarp::ChangePin2(const char *pin,const char *newpin)
{
	char PASSWORD[100];
	char ans[1024];
	memset(PASSWORD,0,100);
	memset(ans,0,1024);
	sprintf(PASSWORD,"AT+PASSWORD=\"P2\",\"%s\",\"%s\"\r",pin,newpin);
	Transaction(PASSWORD,strlen(PASSWORD),ans,1024);
	if (strstr(ans,"ERROR") != NULL)
	{
		return false ;
	}
	return true ;
}
bool ATCommandWarp::GetOperator(std::string &opeator)
{
	char CPOL[] = "AT+CPOL?\r"; //AT+WOPN��ȡ����Ա����
	char ans[1024];
	Transaction(CPOL,strlen(CPOL),ans,1024);
	if (strstr(ans,"OK") != NULL)
	{
		std::string str = ans ;
		int l ;
		l = str.find(',');
		str.substr(l+1);
		l = str.find(',');
		str.substr(l+2);
		l = str.find('\"');
		opeator = str.substr(0,l);
		return true ;
	}
	return false ;
}


bool ATCommandWarp::ReadSimPhoneInfo(std::vector<SIM_FORMAT> &vsim)
{
	char ans[1024] = {0}; // Ӧ��
	int ansLen = 1024;
	char cmd1[]	= "AT+CPBS = \"SM\"\r";//ѡ��SIM������
	memset(ans,0,1024);
	Transaction(cmd1, strlen(cmd1), ans, ansLen);
	if (strstr(ans, "OK") == NULL)
	{
		Dprintf("SM NOT FOUND!");
		return false;
	}
	
	int l;//SIM ���Ѿ����ڵĵ绰����Ŀ
	char cmd2[]	= "AT+CPBS?\r";//��ѯSIM״̬
	memset(ans,0,1024);
	Transaction(cmd2, strlen(cmd2), ans, ansLen);
	if (strstr(ans, "OK") == NULL)
	{
		Dprintf("SM STATE ERROR!");
		return false;
	}
	else
	{
		std::string s = ans;
		l = s.find("\"SM\"");
		s = s.substr(l);
		l = s.find(',');
		s = s.substr(l+1);
		l = atoi(s.c_str());//SIM���Ѿ������Ŀ
	}
	
	char cmd3[16] ;//��ѯ״̬
	char *siminfo ;//����sim��������������Ϣ(�ַ���)
	if (l == 0)
	{
		siminfo = new char[1024];
	}
	else
	{
		siminfo = new char[200*l];
	}

	sprintf(cmd3,"AT+CPBR=0,%d\r",l); 
	Transaction(cmd3, strlen(cmd3), siminfo, 200*l);
	if (strstr(siminfo, "OK") != NULL)
	{
		// �������ص���Ϣ
		ParseAnswerData(siminfo,vsim);
	}

	if (siminfo !=NULL)
	{
		delete []siminfo ;
	}
	return true;
}


bool ATCommandWarp::Save2Sim(std::vector<SIM_FORMAT> vsim)
{
	if (vsim.size() == 0)
	{
		return false;
	}

	char ans[1024] = {0}; // Ӧ�� 
	int ansLen = 1024;

	char HEX[] = "AT+CSCS= \"GSM\"\r";//ѡ����뼯GSM,Ҳ����7_bit
	Transaction(HEX, strlen(HEX), ans, ansLen);
	if (strstr(ans, "OK") == NULL)
	{
		return false;
	}

	char cmd2[1024]; //��绰������ӻ�ɾ����Ŀ
	int length = 0 ;
	for (int i = 0 ; i < vsim.size(); i++)
	{   
		int  size;
		char *pname = new char[200];//������ô��ռ��㹻��
		unsigned char *desname = new unsigned char[200];//
		memset(cmd2,0,1024);
		memset(pname,0,200);
		memset(desname,0,200);	

	//	GB2312toUSC2(vsim[i].name.c_str(),pname,strlen(vsim[i].name.c_str()));//������ת����USC2��ʽ
		int dlength = EncodeGsm(vsim[i].name.c_str(),desname,strlen(vsim[i].name.c_str()));
		gsmBytes2String(desname,pname,dlength);
		sprintf(cmd2,"AT+CPBW=%d,\"%s\",129,\"%s\"\r",0,vsim[i].telnum.c_str(),pname);		
	//	sprintf(cmd2,"AT+CPBW=%d\r",13);		
		Transaction(cmd2, strlen(cmd2), ans, ansLen);
		delete[]pname;
		delete[]desname;
		Sleep(100);
	}
	
}
void ATCommandWarp::ParseAnswerData(char *src,std::vector<SIM_FORMAT> &vsim)
{	
	//��������Ϣ�ĸ�ʽ
	/*
		AT+CPBR= 0,2
		+CPBR: 1,"13512509353",129,"502A5DE5",0
		+CPBR: 2,"12452",129,"7F57603B",0
		OK
	*/
	std::string s = src ;
	std::string name ;
	std::string phone ;
	int l = 0 ;
	while ((l = s.find("+CPBR:"))!= -1)
	{	
		SIM_FORMAT sim ;
		s = s.substr(l+1);
		l = s.find(',');
		s = s.substr(l+1);	
		l = s.find(',');//�ҵ绰����
		phone = s.substr(1,l-2);

		s = s.substr(l+1);
		l = s.find(',');//������
		s = s.substr(l+1);
		l = s.find(',');
		name = s.substr(1,l-2);
		
		//ת��GB2312
		char *des = new char[200];
		unsigned char *temp = new unsigned char[200]; 
		memset(temp,0,200);
		memset(des,0,200);
		int tlength = gsmString2Bytes(name.c_str(),temp,strlen(name.c_str()));
		DecodeGsm(temp,des,tlength);
		//USC2toGB2312((char*)name.c_str(),des,strlen(name.c_str()));
		sim.telnum = phone;
		sim.name = des ;
		vsim.push_back(sim);
		delete []des;
		delete []temp; 
	}
}

// �ɴ�ӡ�ַ���ת��Ϊ�ֽ�����    
// �磺"C8329BFD0E01" --> {0xC8, 0x32, 0x9B, 0xFD, 0x0E, 0x01}    
// ����: pSrc - Դ�ַ���ָ��    
//       nSrcLength - Դ�ַ�������    
// ���: pDst - Ŀ������ָ��    
// ����: Ŀ�����ݳ���    
int ATCommandWarp::gsmString2Bytes(const char* pSrc, unsigned char* pDst, int nSrcLength)   
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
int ATCommandWarp::gsmBytes2String(const unsigned char* pSrc, char* pDst, int nSrcLength)   
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

//ת����7_bit   
int ATCommandWarp::EncodeGsm(const char *src,unsigned char *des,int srclength )
{
	Converter c;	
	int num = 0 ;
	int desLength = 0 ;
	unsigned char* temp = des;
	while( num < srclength)
	{	
		memset(c.convertBuffer,0,7);
		for (int j = 0; j < 7; ++j) 
		{
			if (num > srclength)
			{
				break ;
			}
			c.convertBuffer[j] = src[num++] ;
		}
		*des++ = c.sevenBit0();
		*des++ = c.sevenBit1();
		*des++ = c.sevenBit2();
		*des++ = c.sevenBit3();
		*des++ = c.sevenBit4();
		*des++ = c.sevenBit5();
		*des++ = c.sevenBit6();
		*des++ = c.sevenBit7();
		desLength += 8;
	}
	*des = '\0';
	return desLength;
}

//Decode 7_bit
int ATCommandWarp::DecodeGsm(const unsigned char *src,char *des,int srclength)
{	
	Converter c ;
	int num = 0 ;
	int deslength = 0 ;
	while (num < srclength)
	{	
		memset(c.convertBuffer,0,7);
		c.sevenBit0(src[num++]);
		deslength++ ;
		if (num < srclength)
		{
			c.sevenBit1(src[num++]);
			deslength++;
		}
		if (num < srclength)
		{
			c.sevenBit2(src[num++]);
			deslength++;
		}
		if (num < srclength)
		{
			c.sevenBit3(src[num++]);
			deslength++;
		}
		if (num < srclength)
		{
			c.sevenBit4(src[num++]);
			deslength++;
		}
		if (num < srclength)
		{
			c.sevenBit5 (src[num++]);
			deslength++;
		}		
		if (num < srclength)
		{
			c.sevenBit6 (src[num++]);
			deslength++;
		}		
		if (num < srclength)
		{
			c.sevenBit7 (src[num++]);
			deslength++;
		}
		
		for (int i = 0 ; i < 7; i++)
		{
			*des++ = c.convertBuffer[i];
		}
	}
	*des = '\0';
	return deslength;
}
void ATCommandWarp::GB2312toUSC2(const char *src,char *des,int srclength)
{
	int nDstlength ;
	WCHAR wh[128];//���128����������
	unsigned char pdst[256];
	int it = 0  ;
	int num = 0 ;
	memset(wh,0,128*2);
	memset(pdst,0,256);
	nDstlength = ::MultiByteToWideChar(CP_ACP, 0, src, srclength, wh, srclength +1);
	for(int i=0; i<nDstlength; i++) 
	{
		// �������λ�ֽ�
		pdst[it++] = wh[i] >> 8;
		// �������λ�ֽ�
		pdst[it++] = wh[i] & 0xff;
	}
	pdst[it] = '\n';//������־���Լ��ӵ�
	
	int i = 0 ;
	while (pdst[i] !='\n')//��ֵת����Ӧ���ַ���
	{
		sprintf(des+num*2,"%02x",pdst[i]);
		num++;
		i++;
	}
	des[num*2+1] = '\0';	
}
void ATCommandWarp::USC2toGB2312(char *src,char *des,int length)
{
	char *endpos = 0 ;
	char hex[128];
	WCHAR wh[64];
	memset(hex,0,128);
	memset(wh,0,64);
	int it = 0 ;
	for (int i = 0 ; i < length ;i+=2)//���ַ���ת����16���Ƶ�ֵ
	{   
		char temp[3];
		temp[0] = src[i];
		temp[1] = src[i+1];
		temp[2] = '\0';
		hex[it++]  = ::strtol(temp,&endpos,16);
	}
	
	hex[it] = '\n' ;//��־,�жϸ�hex�Ƿ����
	it = 0 ;
	int i = 0 ;
	while (hex[i] !='\n')//���unicode
	{
		wh[it++] = (hex[i]<< 8) | (unsigned char)hex[i+1] ;
		i +=2;
		if (hex[i-1] == '\n')
		{
			break ;
		}
	}
	wh[it] = '\0' ;	
	// UNICODE��-->�ַ���
	int nDstLength = ::WideCharToMultiByte(CP_ACP, 0, wh, length/4, des, length,
		NULL, NULL);
	// ����ַ����Ӹ�������
	des[nDstLength] = '\0';	
}