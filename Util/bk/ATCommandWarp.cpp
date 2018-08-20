#include "stdafx.h"
#include "ATCommandWarp.h"

using Util::ATCommandWarp;

bool ATCommandWarp::bInit = false;
ATCommandWarp::ATCommandWarp()
{

}

ATCommandWarp::~ATCommandWarp()
{
	if (bInit)
	{
		Off();
	}
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
bool ATCommandWarp::Connect(std::string pin)
{
	if (!bInit)
	{
		if (Init() && On(pin.c_str()))
		{
			bInit = true;
		}
	}	
	return bInit;
}
bool ATCommandWarp::Init(void)
{
	char AT[] = "AT\r";// 测试GSM-MODEM的存在性    
	char ATE[] = "ATE0\r";// ECHO OFF    
	char CMGF[] = "AT+CMGF=0\r";// PDU模式
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



	bool result = true;
	char ans[128];      // 应答串    

	m_pCom->WriteComm(AT, strlen(AT));  
	Sleep(1000);
	m_pCom->ReadComm(ans, 128);
	m_pCom->WriteComm(AT, strlen(AT));  
	m_pCom->ReadComm(ans, 128);
	if (strstr(ans, "OK") == NULL)  
		return false;   

	m_pCom->WriteComm(ATE, strlen(ATE));   
	m_pCom->ReadComm(ans, 128);       
	m_pCom->WriteComm(CMGF, strlen(CMGF));   
	m_pCom->ReadComm(ans, 128); 
	Sleep(500);
	m_pCom->WriteComm(CPMS, strlen(CPMS));   
	m_pCom->ReadComm(ans, 128); 
	Sleep(500);
	m_pCom->WriteComm(CMNI, strlen(CMNI));   
	m_pCom->ReadComm(ans, 128); 
	//m_pCom->WriteComm(ATV, strlen(ATV));
	//m_pCom->ReadComm(ans, 128);

	m_pCom->WriteComm(DUSBDEG1, strlen(DUSBDEG1));
	m_pCom->ReadComm(ans, 128);
// 	m_pCom->WriteComm(DUSBDEG2, strlen(DUSBDEG2));
// 	m_pCom->ReadComm(ans, 128);
// 	m_pCom->WriteComm(DUSBDEG3, strlen(DUSBDEG3));
// 	m_pCom->ReadComm(ans, 128);
// 	m_pCom->WriteComm(DUSBDEG4, strlen(DUSBDEG4));
// 	m_pCom->ReadComm(ans, 128);

	//m_pCom->WriteComm(DGPIOSM, strlen(DGPIOSM));
	//m_pCom->ReadComm(ans, 128);
	m_pCom->WriteComm(DSLP, strlen(DSLP));
	m_pCom->ReadComm(ans, 128);
 	m_pCom->WriteComm(DSQ, strlen(DSQ));
 	m_pCom->ReadComm(ans, 128);
	//m_pCom->WriteComm(CRC, strlen(CRC));
	//m_pCom->ReadComm(ans, 128);
	//m_pCom->WriteComm(DSCI, strlen(DSCI));
	//m_pCom->ReadComm(ans, 128);
	//m_pCom->WriteComm(DCPI, strlen(DCPI));
	//m_pCom->ReadComm(ans, 128);

	if (strstr(ans, "OK\r\n") == NULL)
	{
		result = false;
	}

	return result;
}

bool ATCommandWarp::On(const char* pin)
{
	char CREG[] = "AT+CREG=1\r";//返回“0\r”（此指令为开启网络上报，可选输入指令）；
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

	bool result = true;
	char ans[128];      // 应答串    

	m_pCom->WriteComm(CREG, strlen(CREG));
	m_pCom->ReadComm(ans, 128);
// 	m_pCom->WriteComm(DSLP, strlen(DSLP));
// 	m_pCom->ReadComm(ans, 128);
// 	m_pCom->WriteComm(DSQ, strlen(DSQ));
// 	m_pCom->ReadComm(ans, 128);
	//m_pCom->WriteComm(CMER, strlen(CMER));
	//m_pCom->ReadComm(ans, 128);
	m_pCom->WriteComm(CFUN5, strlen(CFUN5));
	m_pCom->ReadComm(ans, 128);
	if (strstr(ans, "+CME ERROR:11") != NULL)//在设置开机PIN 码时，APP端输入“AT+CFUN=5”，激活USIM 卡，返回的“＋CME		ERROR：11”表示“SIM PIN Required”；此时APP 端会弹出提示输入pin码的提示，		APP端输入PIN 码后，再执行激活协议栈的指令。
	{
		m_pCom->WriteComm((void*)pin, strlen(pin));
		m_pCom->ReadComm(ans, 128);
		if (strstr(ans, "OK\r\n") == NULL)
		{
			result = false;
		}
	}
	Sleep(500);
	m_pCom->WriteComm(CFUN1, strlen(CFUN1));
	m_pCom->ReadComm(ans, 128);
	m_pCom->WriteComm(COPS, strlen(COPS));
	m_pCom->ReadComm(ans, 128);
	while ((strstr(ans, "+CREG: 1") == NULL) && (strstr(ans, "+CREG: 5") == NULL))
	{
		m_pCom->ReadComm(ans, 128);
	}

	return result;
}

bool ATCommandWarp::Off(void)
{
	char COPS[] = "AT+COPS=2\r";//返回“+CREG: 0\r\n0\r”（此指令为去激活网络）；
	char CFUN0[] = "AT+CFUN=0\r";//返回“0\r”（此指令为去激活协议栈）；
	char CFUN6[] = "AT+CFUN=6\r";//返回“0\r”（此指令为去激活USIM 卡）；
	char DSOFF[] = "AT^DSOFF\r";//，返回“0\r”（此指令为模块软关机）；

	bool result = true;
	char ans[128];      // 应答串  

	m_pCom->WriteComm(COPS, strlen(COPS));
	m_pCom->ReadComm(ans, 128);
	m_pCom->WriteComm(CFUN0, strlen(CFUN0));
	m_pCom->ReadComm(ans, 128);
	m_pCom->WriteComm(CFUN6, strlen(CFUN6));
	m_pCom->ReadComm(ans, 128);
	m_pCom->WriteComm(DSOFF, strlen(DSOFF));
	m_pCom->ReadComm(ans, 128);
	if (strstr(ans, "OK\r\n") == NULL)
	{
		result = false;
	}
	return result;
}

bool ATCommandWarp::PhoneDial(char * number)
{
	char ATD[] = "ATD";
	char CMD[128];
	strcpy(CMD, ATD);
	strcat(CMD, number);
	strcat(CMD, "\r");
	bool result = true;
	//char ans[128];      // 应答串    
	m_pCom->WriteComm(CMD, strlen(CMD));
	//	m_pCom->ReadComm(ans, 128);
	//	result = *ans;
	return result;	
}

bool ATCommandWarp::PhoneRedial(void)
{
	char ATDL[] = "ATDL;\r";
	bool result = true;
	char ans[128];      // 应答串    
	m_pCom->WriteComm(ATDL, strlen(ATDL));
	m_pCom->ReadComm(ans, 128);
	return result;	
}

int ATCommandWarp::PhoneState(void)
{
	char CLCC[] = "AT+CLCC\r";
	char result = -1;
	char ans[128] = {0};      // 应答串    
	m_pCom->WriteComm(CLCC, strlen(CLCC));
	m_pCom->ReadComm(ans, 128);

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
	char ans[128];      // 应答串    
	m_pCom->WriteComm(CLCC, strlen(CLCC));
	m_pCom->ReadComm(ans, 128);

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
	//char ans[128];      // 应答串    
	//m_pCom->WriteComm(CPAS, strlen(CPAS));
	//m_pCom->ReadComm(ans, 128);
	//if (strstr(ans, "+CPAS:3") != NULL)
	//{
	//}
	return result;
}

bool ATCommandWarp::PhoneHangup(void)
{
	char ATH[] = "ATH\r";//对于挂断AT指令，AT＋CHUP（挂断当前激活CS链路）和AT＋CHLD 也可作为可选AT指令。被动挂断，则LC6311 会给APP 上报“3\r”，3 表示“NO CARRIER”,为对端主动挂断
	bool result = true;
	char ans[128];      // 应答串    
	m_pCom->WriteComm(ATH, strlen(ATH));
	m_pCom->ReadComm(ans, 128);
	return result;	
}

bool ATCommandWarp::PhoneAnswer(void)
{
	char ATA[] = "ATA\r";//对于挂断AT指令，AT＋CHUP（挂断当前激活CS链路）和AT＋CHLD 也可作为可选AT指令。被动挂断，则LC6311 会给APP 上报“3\r”，3 表示“NO CARRIER”,为对端主动挂断
	bool result = true;
	char ans[128];      // 应答串    
	m_pCom->WriteComm(ATA, strlen(ATA));
	m_pCom->ReadComm(ans, 128);
	return result;	
}
bool ATCommandWarp::PhoneSubDial(char number)
{
	char cmd[16];       // 命令串    
	sprintf(cmd, "AT+VTS=%c\r", number); // 生成命令    
	m_pCom->WriteComm(cmd, strlen(cmd));
	char ans[128];      // 应答串    
	//m_pCom->ReadComm(ans, 128);
	//if(strstr(ans, "OK\r\n") != NULL)   
	{
		return true;
	}
	return false;
}
bool ATCommandWarp::PhoneVolume(unsigned int level)
{
	char cmd[16];       // 命令串    
	sprintf(cmd, "AT+CLVL=%d\r", level); // 生成命令    
	m_pCom->WriteComm(cmd, strlen(cmd));
	char ans[128];      // 应答串    
	//m_pCom->ReadComm(ans, 128);
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
	m_pCom->WriteComm(cmd, strlen(cmd));
	char ans[128];      // 应答串    
	//m_pCom->ReadComm(ans, 128);
	//if(strstr(ans, "OK\r\n") != NULL)   
	{
		return true;
	}
	return false;
}
bool ATCommandWarp::SmsSend(int dataLength)
{
	char cmd[16];       // 命令串    
	sprintf(cmd, "AT+CMGS=%d\r", dataLength); // 生成命令    
	m_pCom->WriteComm(cmd, strlen(cmd));
	int nLength;        // 串口收到的数据长度    
	char ans[128];      // 应答串   
	nLength = m_pCom->ReadComm(ans, 128);
// 	if(nLength == 4 && strncmp(ans, "\r\n> ", 4) == 0)   
	if (strstr(ans, "\r\nOK\r\n") != NULL)
	{
		while (strstr(ans, "\r\n> ") == NULL)
		{
			m_pCom->WriteComm(cmd, strlen(cmd));
			m_pCom->ReadComm(ans, 128);
		}
		return true;
	}
	else if (strstr(ans, "\r\n> ") != NULL)
	{
		return true;
	}
	return false;
}
int ATCommandWarp::SmsSend(char* pdu, int pduLength)
{
	return m_pCom->WriteComm(pdu, pduLength);
}
int ATCommandWarp::SmsDelete(int index)
{
	char cmd[16];       // 命令串    
	sprintf(cmd, "AT+CMGD=%d\r", index);    // 生成命令    
	// 输出命令串    
	return m_pCom->WriteComm(cmd, strlen(cmd)); 
}
int ATCommandWarp::SmsReadList(void)
{
	char cmd[] = "AT+CMGL=4\r";
	return m_pCom->WriteComm(cmd, strlen(cmd));
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
	char CSCA[] = "CSCA?\r";       // 命令串    
	m_pCom->WriteComm(CSCA, strlen(CSCA));
	std::string sca;
	char ans[128] = {0};      // 应答串    
	m_pCom->ReadComm(ans, 128);
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
	char cmd[16];       // 命令串    
	sprintf(cmd, "CSCA=%s\r",sca.c_str()); // 生成命令    
	m_pCom->WriteComm(cmd, strlen(cmd));
	char ans[128];      // 应答串    
	m_pCom->ReadComm(ans, 128);
	if(strstr(ans, "OK\r\n ") != NULL)   
	{
		return true;
	}
	return false;
}
*/