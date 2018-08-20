#include "stdafx.h"
#include "ATCommandWarp.h"

using Util::ATCommandWarp;

bool ATCommandWarp::bInit = false;
ATCommandWarp::ATCommandWarp()
{
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

int ATCommandWarp::Transaction(char* req, int reqLen, char* res, int resLen)
{
	EnterCriticalSection(&m_csAT);
	int dwNumWrite = 0;
	while (dwNumWrite != reqLen)
	{
		dwNumWrite += m_pCom->WriteComm(req + dwNumWrite, reqLen - dwNumWrite);
	}

	int dwNumRead = 0;
	while (true)
	{
		int count = m_pCom->ReadComm(res + dwNumRead, resLen - dwNumRead);
		dwNumRead += count;
		if (((dwNumRead > 0) && (count == 0)) || (strstr(res, "OK") != NULL) || (strstr(res, "ERROR") != NULL) || (strstr(res, "\r\n> ") != NULL))
		{
			break;
		}
	}
	LeaveCriticalSection(&m_csAT);  
	return dwNumRead;
}

int ATCommandWarp::Init(void)
{
	char AT[] = "AT\r";// ����GSM-MODEM�Ĵ�����    
//	char CPIN[] = "AT+CPIN?\r"; //PIN ���� �˴������ж��Ƿ����SIM��
	char ATE[] = "ATE0\r";// ECHO OFF    
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
//	char CNUM[] = "AT+CNUM\r";//���ֻ�����

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

// 	if (!Command(CPIN, strlen(CPIN)))
// 	{
// 		return 2;
// 	}
// 
// 	if (Command(CNUM, strlen(CNUM), ans))
// 	{
// 		std::string number = ans;
// 		unsigned int pos1 = number.find(",");
// 		if (pos1 != std::string::npos)
// 		{
// 			unsigned int pos2 = number.find(",", pos1 + 1);
// 			if (pos2 != std::string::npos)
// 			{
// 				pos1 += 2;
// 				pos2 -= 1;
// 				if (pos2 - pos1 > 0)
// 				{
// 					m_strPhoneNumber = number.substr(pos1, pos2 - pos1);
// 				}
// 			}
// 		}
// 	}

	if (Command(ATE, strlen(ATE))
		&& Command(DUSBDEG1, strlen(DUSBDEG1))
		&& Command(DUSBDEG2, strlen(DUSBDEG2))
		&& Command(DUSBDEG3, strlen(DUSBDEG3))
		&& Command(DUSBDEG4, strlen(DUSBDEG4))
		&& Command(DGPIOSM, strlen(DGPIOSM))
//		&& Command(DSQ, strlen(DSQ))
		&& Command(CMGF, strlen(CMGF))
		&& Command(CSCS, strlen(CSCS))
		&& Command(CPMS, strlen(CPMS))
		&& Command(CMNI, strlen(CMNI))
		&& Command(DSLP, strlen(DSLP))
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

	char CPIN[] = "AT+CPIN?\r"; //PIN ���� �˴������ж��Ƿ����SIM��
	char CNUM[] = "AT+CNUM\r";//���ֻ�����

	bool result = true;
	char ans[1024] = {0};      // Ӧ��    

	if (!Command(CREG1, strlen(CREG1)))
	{
		return 0;
	}

	if (!Command(CFUN5, strlen(CFUN5), ans))
	{
		if (strstr(ans, "+CME ERROR:11") != NULL)//�����ÿ���PIN ��ʱ��APP�����롰AT+CFUN=5��������USIM �������صġ���CME		ERROR��11����ʾ��SIM PIN Required������ʱAPP �˻ᵯ����ʾ����pin�����ʾ��		APP������PIN �����ִ�м���Э��ջ��ָ�
		{
			if (!Command(pin, strlen(pin)))
			{
				return 2;
			}
		}
		else if (strstr(ans, "+CME ERROR:10") != NULL)
		{
			return 3;
		}
		else
		{
			return 0;
		}
	}
	
	if(!(Command(CFUN1, strlen(CFUN1))
		&&Command(COPS, strlen(COPS))))
	{
		return 0;
	}

	m_pCom->ReadComm(ans, 1024);
 	while ((strstr(ans, "+CREG: 1") == NULL) && (strstr(ans, "+CREG: 5") == NULL))
	{
		m_pCom->ReadComm(ans, 1024);
	}
	Command(CREG0, strlen(CREG0));

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
/*
std::string ATCommandWarp::SmsCenterAddress(void)
{
	char CSCA[] = "CSCA?\r";       // ���    
	m_pCom->WriteComm(CSCA, strlen(CSCA));
	std::string sca;
	char ans[1024] = {0};      // Ӧ��    
	m_pCom->ReadComm(ans, 1024);
	if(strstr(ans, "+CSCA: ") != NULL)
	{
		std::string tmp = ans;
		int pos1 = tmp.find(" ");
		int pos2 = tmp.find(",");
		if ((pos1 != std::string::npos) && (pos2 != std::string::npos))
		{
			sca = tmp.substr(pos1 + 1, pos2 - pos1 - 1);
		}
	}
	return sca;
}
bool ATCommandWarp::SmsCenterAddress(std::string sca)
{
	char cmd[16];       // ���    
	sprintf(cmd, "CSCA=%s\r",sca.c_str()); // ��������    
	m_pCom->WriteComm(cmd, strlen(cmd));
	char ans[1024];      // Ӧ��    
	m_pCom->ReadComm(ans, 1024);
	if(strstr(ans, "OK\r\n ") != NULL)   
	{
		return true;
	}
	return false;
}
*/