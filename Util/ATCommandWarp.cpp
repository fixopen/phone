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

	char ans[1024] = {0};      // 应答串 
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
	char AT[] = "AT\r";// 测试GSM-MODEM的存在性
	char ATZ[] = "ATZ\r";//复位
	char ATE[] = "ATE0\r";// ECHO OFF  
	char CGMR[] = "AT+CGMR\r";
	char CMGF[] = "AT+CMGF=0\r";// PDU模式
	char CSCS[] = "AT+CSCS=\"UCS2\"\r";
	char CPMS[] = "AT+CPMS=\"SM\",\"SM\",\"SM\"\r";
	char CMNI[] = "AT+CNMI=2,1,0,0,0\r";
	char ATV[] = "ATV1\r";//1 <CR><LF><verbose code><CR><LF> 0 <numeric code><CR>	"ATV0\r";返回“0\r”（ATV指令决定返回result code是数字格式还是字符格式，ATV0 返回为数字格式，其中0 表示OK，1 表示CONNECT，2 表示RING，3 表示NO CARRIER，4 表示ERROR
	char DUSBDEG1[] = "AT^DUSBDEG=1,1\r";//返回“0\r”,（此指令为开启EP1 通信口，第一个参数从1-4 对应EP1-EP4，第二个参数1 表示open，0 表示close；在启用某	个通信口做业务之前需通过此指令开启对应通信口）
	char DUSBDEG2[] = "AT^DUSBDEG=2,1\r";//返回“0\r”,（此指令为开启EP1 通信口，第一个参数从1-4 对应EP1-EP4，第二个参数1 表示open，0 表示close；在启用某	个通信口做业务之前需通过此指令开启对应通信口）
	char DUSBDEG3[] = "AT^DUSBDEG=3,1\r";//返回“0\r”,（此指令为开启EP1 通信口，第一个参数从1-4 对应EP1-EP4，第二个参数1 表示open，0 表示close；在启用某	个通信口做业务之前需通过此指令开启对应通信口）
	char DUSBDEG4[] = "AT^DUSBDEG=4,1\r";//返回“0\r”,（此指令为开启EP1 通信口，第一个参数从1-4 对应EP1-EP4，第二个参数1 表示open，0 表示close；在启用某	个通信口做业务之前需通过此指令开启对应通信口）
	char DGPIOSM[] = "AT^DGPIOSM=1\r";//r”，返回“0\r”（, 此指令为APP与模块唤醒模式选择）
	char DSLP[] = "AT^DSLP=0,0\r";
	char DSQ[] = "AT^DSQ=1\r";//返回“0\r”,（此指令为开启网络信号分贝值的上报）；
	char CRC[] = "AT+CRC=1\r";//返回“0\r”,（此指令为开启来电后的扩展指示，可选	输入指令）；
	char DSCI[] = "AT^DSCI=1\r";//，返回“0\r”,（此指令为开启进度指示，可选输入指令）；
	char DCPI[] = "AT^DCPI=1\r";//返回“0\r”, （此指令为开启呼叫状态指示，以拨打语	音电话10086 为例，开启此上报指示后，其后跟随DCPI 的上报指示，
	//“^DCPI:1,0,0,1”为建立连接指示；“^DCPI:1,3,0,1” 为正在呼叫指示；
	//“^DCPI:1,4,0,1” 为信道分配指示；“^DCPI:1,5,0,1” 为描述进度指示；
	//“^DCPI:1,2,0,1”为被叫振铃指示；“^DCPI:1,6,0,1”为呼叫建立指示，可选输入指令）；

	char DVTTYPE[] = "AT^DVTTYPE=3\r";
	char DVTABLE[] = "AT^DVTABLE=1,1,0\r";

	char DAUDSCE[] = "AT^DAUDSCE=?\r";
	char DAUDSCE0[] = "AT^DAUDSCE=0\r";
	char DAUDSCE1[] = "AT^DAUDSCE=1\r";
	char DAUDSCS[] = "AT^DAUDSCS?\r";

	bool result = true;
	char ans[1024] = {0};      // 应答串    

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

		//视频电话
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
	char CREG1[] = "AT+CREG=1\r";//返回“0\r”（此指令为开启网络上报，可选输入指令）；
	char CREG0[] = "AT+CREG=0\r";//返回“0\r”（此指令为开启网络上报，可选输入指令）；
	char CMER[] = "AT+CMER=2,0,0,2\r";//r”，返回“0\r”（此指令为开启高层协议栈事件CIEV：	<ind>,<value>上报）；
	char CFUN5[] = "AT+CFUN=5\r";//返回“0\r”，（此指令为激活USIM 卡）；
	char CFUN1[] = "AT+CFUN=1\r";//，返回“0\r”，（此指令为激活协议栈）；
	char COPS[] = "AT+COPS=0\r";//返回“0\r”，(此指令为搜网指令),此后LC6311 会返
	//回很多数据，直到返回“+CREG：1”表示开机成功（“^DACTI:2”表示当前接入技术是TD; “+CIEV：2,2”表示信号等级为2 级；“+CIEV：8,0”表示短
	//消息存储未满；“+CREG：2”表示搜网中；“+CREG：1”表示搜网成功并注
	//册成功。【说明：“+CREG:n”其中“n”的值可为0[未注册]，1[注册成功]，2[搜网]，3[网络拒绝注册]，4[网络注册状态未知]，5[漫游]，开机后只有当CREG
	//返回的是1 或5 时，才能做LC6311 所支持的业务】；鉴于对CREG：4 的协议
	//理解，建议当+CREG：4 时，APP 处理器在UI 显示为可提供服务状态，即等
	//同于+CREG：1或+CREG:5】）。

	char CMGF[] = "AT+CMGF=0\r";// PDU模式
	char CSCS[] = "AT+CSCS=\"GSM\"\r";//UCS2//
	char CPMS[] = "AT+CPMS=\"SM\",\"SM\",\"SM\"\r";
	char CMNI[] = "AT+CNMI=2,1,0,0,0\r";

	char CPIN[] = "AT+CPIN?\r"; //PIN 鉴定 此处用来判断是否插入SIM卡
	char CNUM[] = "AT+CNUM\r";//读手机号码
	char CSCA[] = "AT+CSCA?\r";//读短信中心号码


	bool result = true;
	char ans[1024] = {0};      // 应答串    
	
	if (!Command(CREG1, strlen(CREG1)))
	{
		return 0;
	}

//	Command(CMER, strlen(CMER));
	memset(ans, 0, 1024);

//	if (!Command(CFUN5, strlen(CFUN5), ans)) // old
	if (!CommandFun5(CFUN5, strlen(CFUN5), ans))//add by qi 2009_08_28
	{	
		if (strstr(ans, "+CME ERROR: 11") != NULL || strstr(ans, "+CME ERROR: 12") != NULL )//在设置开机PIN 码时，APP端输入“AT+CFUN=5”，激活USIM 卡，返回的“＋CME		ERROR：11”表示“SIM PIN Required”；此时APP 端会弹出提示输入pin码的提示，		APP端输入PIN 码后，再执行激活协议栈的指令。
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
	char COPS[] = "AT+COPS=2\r";//返回“+CREG: 0\r\n0\r”（此指令为去激活网络）；
	char CFUN0[] = "AT+CFUN=0\r";//返回“0\r”（此指令为去激活协议栈）；
	char CFUN6[] = "AT+CFUN=6\r";//返回“0\r”（此指令为去激活USIM 卡）；
	char DSOFF[] = "AT^DSOFF\r";//，返回“0\r”（此指令为模块软关机）；

	bool result = true;
	char ans[1024] = {0};      // 应答串  

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
	char ans[1024] = {0};      // 应答串 
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
	char ans[1024] = {0};      // 应答串 
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
	char ans[1024] = {0};      // 应答串 
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
	char CLCC[] = "AT+CLCC\r";//注意：来电消息是主动上报的，这里还需要修改。 +CRING: VOICE\r\n+CLIP:"10086",161,"",,"",0\r\n
	bool result = false;
	char ans[1024] = {0};      // 应答串 
	int ansLen = 1024;
// 	m_pCom->WriteComm(CLCC, strlen(CLCC));
// 	m_pCom->ReadComm(ans, 1024);
	Transaction(CLCC, strlen(CLCC), ans, ansLen);

	char* p = strstr(ans, "+CLIP:");
	if ((strstr(ans, "RING") != NULL) && (p != NULL))
	{
		p += 8;//移动到+CLIP:"之后
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
	//char ans[1024];      // 应答串    
	//m_pCom->WriteComm(CPAS, strlen(CPAS));
	//m_pCom->ReadComm(ans, 1024);
	//if (strstr(ans, "+CPAS:3") != NULL)
	//{
	//}
	return result;
}

bool ATCommandWarp::PhoneHangup(void)
{
	char ATH[] = "ATH\r";//对于挂断AT指令，AT＋CHUP（挂断当前激活CS链路）和AT＋CHLD 也可作为可选AT指令。被动挂断，则LC6311 会给APP 上报“3\r”，3 表示“NO CARRIER”,为对端主动挂断
	bool result = true;
	char ans[1024] = {0};      // 应答串 
	int ansLen = 1024;
// 	m_pCom->WriteComm(ATH, strlen(ATH));
// 	m_pCom->ReadComm(ans, 1024);
	Transaction(ATH, strlen(ATH), ans, ansLen);
	return result;	
}

bool ATCommandWarp::PhoneAnswer(void)
{
	char ATA[] = "ATA\r";//对于挂断AT指令，AT＋CHUP（挂断当前激活CS链路）和AT＋CHLD 也可作为可选AT指令。被动挂断，则LC6311 会给APP 上报“3\r”，3 表示“NO CARRIER”,为对端主动挂断
	bool result = true;
	char ans[1024] = {0};      // 应答串 
	int ansLen = 1024;
// 	m_pCom->WriteComm(ATA, strlen(ATA));
// 	m_pCom->ReadComm(ans, 1024);
	Transaction(ATA, strlen(ATA), ans, ansLen);
	return result;	
}
bool ATCommandWarp::PhoneSubDial(char number)
{
	char cmd[16];       // 命令串    
	sprintf(cmd, "AT+VTS=%c\r", number); // 生成命令    
	char ans[1024] = {0};      // 应答串 
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


//电话功能
bool ATCommandWarp::PhoneSetTrans(PhoneCallTRans calltrans)     //设置呼叫转移
{
	return true;
}

bool ATCommandWarp::PhoneGetTrans(PhoneCallTRans &calltrans)	 //获取呼叫转移设置
{
	char cmd[16];       // 命令串    
	for(int i = 0; i < 4; i++)
	{
		sprintf(cmd, "AT+CCFC=%d,2?\r", i); // 生成命令    
		char ans[1024] = {0};      // 应答串 
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

bool ATCommandWarp::PhoneSetCallLimit(PhoneCallLimit calllimit)    //设置呼叫受限
{
	return true;
}

bool ATCommandWarp::PhoneGetCallLimit(PhoneCallLimit &calllimit)    //获取呼叫受限设置
{
	return true;
}

bool ATCommandWarp::PhoneSetCallWaiting(PhoneCallWaiting callwaiting)		//设置呼叫等待
{
	return true;
}

bool ATCommandWarp::PhoneGetCallWaiting(PhoneCallWaiting &callwaiting)		//获取呼叫等待设置
{
	return true;
}

bool ATCommandWarp::PhoneVolume(unsigned int level)
{
	char cmd1[16];       // 命令串    
	char cmd2[16];       // 命令串    
	char cmd3[16];       // 命令串    
	sprintf(cmd1, "AT^DAUDO=1,%d\r", level); // 生成命令 
	sprintf(cmd2, "AT^DAUDO=2,%d\r", level); // 生成命令 
	sprintf(cmd3, "AT^DAUDO=3,%d\r", level); // 生成命令 
	
	char ans[1024] = {0};      // 应答串 
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
	char cmd[16];       // 命令串    
	sprintf(cmd, "AT+CMUT=%d\r", mute); // 生成命令    
	char ans[1024] = {0};      // 应答串 
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
	char cmd[16];       // 命令串    
	sprintf(cmd, "ATS0=%d\r", second); // 生成命令    
	char ans[1024] = {0};      // 应答串 
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
	char cmd[] = "AT^DACTI?\r";       // 命令串    
	unsigned int result = 0;
	char ans[1024] = {0};      // 应答串 
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
	char cmd[] = "AT+CSQ\r";       // 命令串    
	unsigned int result = 0;
	char ans[1024] = {0};      // 应答串 
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
	char cmd[16];       // 命令串    
	sprintf(cmd, "AT^DAUDSCS=%d\r", hf); // 生成命令    
	char ans[1024] = {0};      // 应答串 
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
	char cmd[16];       // 命令串    
	sprintf(cmd, "AT+CMGS=%d\r", dataLength); // 生成命令    
	int nLength;        // 串口收到的数据长度    
	char ans[1024] = {0};      // 应答串 
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
	char cmd[16];       // 命令串    
	sprintf(cmd, "AT+CMGD=%d\r", index);    // 生成命令    
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
	char ans[1024] = {0};      // 应答串 
	int ansLen = 1024;
	int nLength = Transaction(cmd, strlen(cmd), ans, ansLen);
	return true;
}

// add by qi 2009_07_22
void ATCommandWarp::CheckPin()
{
	char PIN[] = "AT+CPIN?\r" ;//查询sim卡状态
	char answer[1024];
	memset(answer,0,1024);
	if (Command(PIN,strlen(PIN),answer))
	{
		if (strstr(answer,"+CPIN: SIM PIN") !=NULL)//需要pin1码
		{	
			HandlePin();
		}
		else if (strstr(answer,"+CPIN: SIM PUK") !=NULL)//需要PUK1码
		{	
			HandlePuk();
		}
	}
}
void ATCommandWarp::HandlePin()
{
	char ans[1024];//应答消息
	memset(ans,0,1024);
	PostMessage(theApp.m_pMainWnd->m_hWnd,WM_PIN_INPUT,0,0);
	while (strstr(ans,"+CME ERROR: 12") == NULL)//连续三次pin码输入错误，sim将被锁住
	{	
		
		while(strstr(ans,"OK") == NULL && strstr(ans,"ERROR") == NULL )//等待用户输入pin码
		{	
			Sleep(100);
			m_pCom->ReadComm(ans,1024);	
		}		
		if (strstr(ans,"OK") != NULL)//输入pin码正确
		{	
//			PostMessage(theApp.m_pMainWnd->m_hWnd,WM_PIN_INPUT,PIN_OK,0);
			return ;	
		}
		
		if ( strstr(ans,"ERROR") != NULL)//输入pin码错误
		{	
			if (strstr(ans,"+CME ERROR: 12") == NULL)
			{
				PostMessage(theApp.m_pMainWnd->m_hWnd,WM_PIN_INPUT,PIN_ERROR,0);
				memset(ans, 0, 1024);
			}
		}
		
	}
	
	if (strstr(ans,"+CME ERROR: 12")!= NULL)//需要输入PUK码
	{
		HandlePuk();
	}		
	
}
void ATCommandWarp::HandlePuk()
{
	int num = 0 ;
	PostMessage(theApp.m_pMainWnd->m_hWnd,WM_PIN_INPUT,PIN_PUK,0);	
	while ( num != 10)//如果输错PUK码累计10次，您的SIM卡将被永久锁住（SIM报废）需要重新换卡。
	{			
		char ans[1024];
		memset(ans,0,1024);
		while(strstr(ans,"OK") == NULL && strstr(ans,"ERROR")== NULL )//等待用户输入pin码
		{	
			Sleep(100);
			m_pCom->ReadComm(ans,1024);	
		}		
		if (strstr(ans,"OK") != NULL)//输入PUK码正确
		{	
		//	PostMessage(theApp.m_pMainWnd->m_hWnd,WM_PIN_INPUT,PIN_OK,0);
			return ;	
		}		
		if ( strstr(ans,"ERROR") != NULL)//输入PUK码错误
		{
			if (num > 0 && num < 9)
			{	
				num++;
				PostMessage(theApp.m_pMainWnd->m_hWnd,WM_PIN_INPUT,PIN_FUK_ERROR,0);
				memset(ans, 0, 1024);
			}
		}		
	}
	
	if (10 == num)//10次输入错误
	{			
		PostMessage(theApp.m_pMainWnd->m_hWnd,WM_PIN_INPUT,PIN_FUK_LOCKED,0);		
	}

}
bool ATCommandWarp::CommandFun5(char* cmd, int len, char* out /* = NULL */, int count /* = 5 */)
{
	char ans[1024] = {0};// 应答串 
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

		if (strstr(ans,"+CME ERROR: 11") != NULL && strstr(ans,"OK") != NULL)//需要PIN码
		{
			result = false ;
			break;
		}
		else if (strstr(ans,"+CME ERROR: 12") != NULL && strstr(ans,"OK") != NULL)//需要输入PUK码
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
	m_pCom->WriteComm(PIN,strlen(PIN));//输入pin码
}

void ATCommandWarp::InputPuk(const char *puk,const char *newpin)
{
	char FUK[100];
	memset(FUK,0,100);
	sprintf(FUK,"AT+CPIN=\"%s\",\"%s\"\r",puk,newpin);
	m_pCom->WriteComm(FUK,strlen(FUK));//输入sim卡解锁码和新的pin码
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

// 	char CLCK[100];//SIM上锁
 	char ans[1024];
//	memset(CLCK,0,100);
	memset(ans,0,1024);
//	sprintf(CLCK,"AT+CLCK=\"SC\",1,\"%s\"\r","5678");
//	Command(CLCK,strlen(CLCK),ans);	
	
// 	char CPIN[] = "AT+CPIN?\r"; //PIN 鉴定 此处用来判断是否插入SIM卡
// 	char ans[1024];
// 	memset(ans,0,100);
// 	Command(CPIN,strlen(CPIN),ans);	

// 	char FUK[100];
// 	memset(FUK,0,100);
// 	sprintf(FUK,"AT+CPIN=\"%s\",\"%s\"\r","04002585","5678");
// 	Command(FUK,strlen(FUK),ans);

//	输入pin码
// 	char PIN[100];
// 	memset(PIN,0,100);
// 	memset(ans,0,100);
// 	sprintf(PIN,"AT+CPIN=\"%s\"\r","5678");
// 	Command(PIN,strlen(PIN),ans);

//  char CLCK[100];//sim解锁
//  memset(CLCK,0,100);
// 	memset(ans,0,1024);
//  sprintf(CLCK,"AT+CLCK=\"SC\",0,\"%s\"\r","5678");
//  Command(CLCK,strlen(CLCK),ans);

// 	memset(ans,0,100);
// 	Command(CPIN,strlen(CPIN),ans);	

// 	sprintf(FUK,"AT+CPIN=\"%s\",\"%s\"\r","04002585","5678");
//  Command(FUK,strlen(FUK),ans);

//  修改pin码
//	char PASSWORD[100];
//	memset(ans,0,1024);
//	memset(PASSWORD,0,100);
//	sprintf(PASSWORD,"AT+CPWD=\"SC\",\"%s\",\"%s\"\r","5678","1234");
//	Command(PASSWORD,strlen(PASSWORD),ans);

//	无条件转移，开通无条件转移
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
	char CLCK[100];//SIM上锁
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
	char CLCK[100];//sim解锁
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
	//只有在锁住的情况下才能改变pin
	
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
	char CPOL[] = "AT+CPOL?\r"; //AT+WOPN读取操作员名字
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
	char ans[1024] = {0}; // 应答串
	int ansLen = 1024;
	char cmd1[]	= "AT+CPBS = \"SM\"\r";//选择SIM存贮器
	memset(ans,0,1024);
	Transaction(cmd1, strlen(cmd1), ans, ansLen);
	if (strstr(ans, "OK") == NULL)
	{
		Dprintf("SM NOT FOUND!");
		return false;
	}
	
	int l;//SIM 中已经存在的电话的数目
	char cmd2[]	= "AT+CPBS?\r";//查询SIM状态
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
		l = atoi(s.c_str());//SIM中已经存的数目
	}
	
	char cmd3[16] ;//查询状态
	char *siminfo ;//保存sim卡反馈回所有信息(字符串)
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
		// 分析返回的信息
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

	char ans[1024] = {0}; // 应答串 
	int ansLen = 1024;

	char HEX[] = "AT+CSCS= \"GSM\"\r";//选择编码集GSM,也就是7_bit
	Transaction(HEX, strlen(HEX), ans, ansLen);
	if (strstr(ans, "OK") == NULL)
	{
		return false;
	}

	char cmd2[1024]; //向电话本中添加或删除条目
	int length = 0 ;
	for (int i = 0 ; i < vsim.size(); i++)
	{   
		int  size;
		char *pname = new char[200];//名字这么大空间足够了
		unsigned char *desname = new unsigned char[200];//
		memset(cmd2,0,1024);
		memset(pname,0,200);
		memset(desname,0,200);	

	//	GB2312toUSC2(vsim[i].name.c_str(),pname,strlen(vsim[i].name.c_str()));//将名字转换成USC2格式
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
	//反馈的信息的格式
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
		l = s.find(',');//找电话号码
		phone = s.substr(1,l-2);

		s = s.substr(l+1);
		l = s.find(',');//找姓名
		s = s.substr(l+1);
		l = s.find(',');
		name = s.substr(1,l-2);
		
		//转化GB2312
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

// 可打印字符串转换为字节数据    
// 如："C8329BFD0E01" --> {0xC8, 0x32, 0x9B, 0xFD, 0x0E, 0x01}    
// 输入: pSrc - 源字符串指针    
//       nSrcLength - 源字符串长度    
// 输出: pDst - 目标数据指针    
// 返回: 目标数据长度    
int ATCommandWarp::gsmString2Bytes(const char* pSrc, unsigned char* pDst, int nSrcLength)   
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
int ATCommandWarp::gsmBytes2String(const unsigned char* pSrc, char* pDst, int nSrcLength)   
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

//转化成7_bit   
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
	WCHAR wh[128];//最多128个汉字名字
	unsigned char pdst[256];
	int it = 0  ;
	int num = 0 ;
	memset(wh,0,128*2);
	memset(pdst,0,256);
	nDstlength = ::MultiByteToWideChar(CP_ACP, 0, src, srclength, wh, srclength +1);
	for(int i=0; i<nDstlength; i++) 
	{
		// 先输出高位字节
		pdst[it++] = wh[i] >> 8;
		// 后输出低位字节
		pdst[it++] = wh[i] & 0xff;
	}
	pdst[it] = '\n';//结束标志，自己加的
	
	int i = 0 ;
	while (pdst[i] !='\n')//把值转成相应的字符串
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
	for (int i = 0 ; i < length ;i+=2)//把字符串转化成16进制的值
	{   
		char temp[3];
		temp[0] = src[i];
		temp[1] = src[i+1];
		temp[2] = '\0';
		hex[it++]  = ::strtol(temp,&endpos,16);
	}
	
	hex[it] = '\n' ;//标志,判断该hex是否结束
	it = 0 ;
	int i = 0 ;
	while (hex[i] !='\n')//组成unicode
	{
		wh[it++] = (hex[i]<< 8) | (unsigned char)hex[i+1] ;
		i +=2;
		if (hex[i-1] == '\n')
		{
			break ;
		}
	}
	wh[it] = '\0' ;	
	// UNICODE串-->字符串
	int nDstLength = ::WideCharToMultiByte(CP_ACP, 0, wh, length/4, des, length,
		NULL, NULL);
	// 输出字符串加个结束符
	des[nDstLength] = '\0';	
}