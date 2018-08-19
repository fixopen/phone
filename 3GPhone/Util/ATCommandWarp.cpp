#include "ATCommandWarp.h"
#include "fsm.h"
#include "Log.h"

namespace Util {
    struct CREGResponseParser { //register
        //+CREG:state[,"locationAreaCode","cellId"]
        CREGResponseParser(std::string const& content);
        enum State {
            sUnregisterNotSearch = 0,
            wRegisterHome = 1,
            sUnregisterAndSearch = 2,
            sRegisterDenied = 3,
            sUnknown = 4,
            sRegisterRoaming = 5,
        };
        State state;
        std::string locationAreaCode;
        std::string cellId;
    };

    struct CMERResponseParser { //Mobile termination Event Report
        //+CIEV:indication, value //Indicator EVent report
        CMERResponseParser(std::string const& content);
        enum Indication {
            iBatteryChargeLevel = 1, //0-5::0 20 40 60 80 100
            iSignalQulity = 2, //0-5
            iServiceLevel = 3, //0 - 1 Service availability
            iSounderActivity = 4, //0 tone stop, 1 incoming call ringing, waiting call tone, call process tone, busy tone
            iUnreadedShortMessage = 5, //0 1
            iCallInProgress = 6, //0 1
            iRoamingStatus = 7, //0 1
            iSMSStorageFull = 8, //0 available 1 full
        };
        Indication indication;
        int value;
    };

    struct CFUNParser { //set device FUNctionaliity
        //AT+CFUN=funLevel[,reset]
        CFUNParser(std::string const& content);
        enum FunctionLevel {
            fMinimum = 0, //power off, deregisteration
            fFull = 1, //power on, registeration
            fFly = 4, //disable transmit and receice RF circuits
            fSimUsimActivate = 5,
            fSimUsimDeactivate = 6,
        };
        enum Reset {
            rImmediately = 0, //default
            rManual = 1,
        };
        FunctionLevel functionLevel;
        Reset reset;
    };

    struct COPSParser { //set operator selection mode
        //AT+COPS=[mode[,format[,operator[,accessTech]]]]
        enum Mode {
            mAutomatic = 0, //operator ignored
            mManual = 1, //operator must has
            mManualDeregister = 2,
            mSetFormat4ReadCommandPlusCOPSQuest = 3,
            mIfManualFailSwitchAutomatic = 4,
        };
        enum Format {
            fLongAlphaNumeric = 0,
            fShortAlphaNumeric = 1,
            fNumeric = 2,
        };
        enum State {
            sUnknown = 0,
            sAvailable = 1,
            sCurrent = 2,
            sForbidden = 3,
        };
        enum AccessTech {
            aGSM = 0,
            aGSMCompact = 1,
            aUTRAN  = 2,
            aEGPRS = 3,
            aGPRSwithDTM = 16,
            aEGPRSwithDTM = 19,
        };
    };

    struct DSQParser { //Signal Quality
        //AT^DSQ=[mode]
        enum Mode {
            mDisableIntermediateResult = 0,
            mEnableIntermediateResult = 1,
            mQueryCurrentSignalQuality = 2, //^DSQ:value //0-91
        };
        Mode mode;
    };

    struct CRCParser { //Cellular Result Code
        //AT+CRC=[mode]
        enum Mode {
            mDisableExtend = 0, //RING
            mEnableExtend = 1, //+CRING
        };
        Mode mode;
    };


    ATCommand::ATCommand(unsigned int const port) {
        Init(port);
    }

    void ATCommand::Init3GModule() {
        //std::string ATV = "ATV1\x0D"; //set response mode
        std::string DUSBDEG = "AT^DUSBDEG=1,1\x0D"; //setup the serial port
        Write(DUSBDEG);
        std::string DGPIOSM = "AT^DGPIOSM=1\x0D"; //set wakeup mode
        Write(DGPIOSM);
        std::string DSQ = "AT^DSQ\x0D"; //startup network signal indicator
        Write(DSQ);
        std::string CRC = "AT+CRC=1\x0D"; //incoming call extend indicator
        Write(CRC);
        std::string DSCI = "AT^DSCI=1\x0D"; //progress indicator
        Write(DSCI);
        std::string DCPI = "AT^DCPI=1\x0D"; //call progress indication
        Write(DCPI);
    }

    void ATCommand::Startup() {
        std::string CREG = "AT+CREG=1\x0D";
        Write(CREG);
        std::string CMER = "AT+CMER=2,0,0,2\x0D";
        Write(CMER);
        std::string CFUN5 = "AT+CFUN=5\x0D";
        Write(CFUN5);
        std::string CFUN1 = "AT+CFUN=1\x0D";
        Write(CFUN1);
        std::string COPS = "AT+COPS=0\x0D";
        Write(COPS);
    }

    void ATCommand::StartupWithPin(std::string const& pin) {
        //std::string CREG = "AT+CREG=1\x0D";
        //std::string CMER = "AT+CMER =2,0,0,2\x0D";
        //std::string CFUN5 = "AT+CFUN=5\x0D";
        //std::string CPIN = "AT+CPIN=\"" + pin + "\"";
        //std::string CFUN1 = "AT+CFUN=1\x0D";
        //std::string COPS = "AT+COPS=0\x0D";
    }

    void ATCommand::ShutDown() {
        std::string COPS = "AT+COPS=2\x0D";
        Write(COPS);
        std::string CFUN0 = "AT+CFUN=0\x0D";
        Write(CFUN0);
        std::string CFUN6 = "AT+CFUN=6\x0D";
        Write(CFUN6);
        std::string DSOFF = "AT^DSOFF\x0D";
        Write(DSOFF);
    }

    void ATCommand::disptch_(unsigned char const* const data, unsigned int const length) {
        Log::Log(reinterpret_cast<char const* const>(data));
        //std::string d(reinterpret_cast<char const*>(data), length);
        //for (int i = 0; i < observers_.size(); ++i) {
        //    observers_[i](d);
        //}
        //if (state_ == sRequest) {
        //    if ((d.find("OK") != std::string::npos)
        //        || (d.find("ERROR") != std::string::npos)) {
        //        state_ = sNull;
        //        //state = sNull;
        //    }
        //}
    }

    void ATCommand::Init(unsigned int portNo, unsigned int baud, unsigned int parity, unsigned int databits, unsigned int stopbits) {
        serialPort_ = new Util::RS232();
        serialPort_->OpenPort(portNo, baud, parity, databits, stopbits);
        //serialPort_->SetReadFunc(disptch_);
    }

    void ATCommand::Finally() {
        serialPort_->ClosePort();
        delete serialPort_;
    }

    void ATCommand::RegisterObserver(Observer ob) {
        observers_.push_back(ob);
    }

    void ATCommand::Write(std::string const& data) {
        //while (state_ != sNull) {
        //    ::Sleep(1000);
        //}
        //if (state_ == sNull) {
            serialPort_->WritePort(reinterpret_cast<unsigned char const* const>(data.data()), data.length());
            state_ = sRequest;
            Util::Log::Log(data);
        //}
    }

    std::string const ATCommand::Read() {
        std::string result;
        int const readBufferLength = 1024;
        char readBuffer[readBufferLength] = {0};
        unsigned long actualReadLen = 0;
        ReadFile(serialPort_->handle_, readBuffer, readBufferLength, &actualReadLen, 0);
        if (actualReadLen > 0) {
            char buffer[1248] = {0};
            sprintf(buffer, "receive data is %s", readBuffer);
            Log::Log(buffer);
            result = readBuffer;
        }
        return result;
    }

    namespace {
        void parseSerialData(std::string const& data) {
        }
    }

    void ATCommand::RegisterNetwork(std::string const& pin) {
        Init(1, 9600);
        RegisterObserver(parseSerialData);
        std::string AT = "AT\x0D";
        Write(AT);
        std::string ATE = "ATE0\x0D"; //set echo off
        Write(ATE);
        std::string CMGF = "AT+CMGF=0\x0D"; //set sms format to pdu, 1 is text
        Write(CMGF);
    }

    void ATCommand::UnregisterNetwork() {
        std::string COPS = "AT+COPS=2\x0D";
        Write(COPS);
        std::string CFUN0 = "AT+CFUN=0\x0D";
        Write(CFUN0);
        std::string CFUN6 = "AT+CFUN=6\x0D";
        Write(CFUN6);
        std::string DSOFF = "AT^DSOFF\x0D";
        Write(DSOFF);
        Finally();
    }
}

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
	char AT[] = "AT\x0D";// 测试GSM-MODEM的存在性    
	char ATE[] = "ATE0\x0D";// ECHO OFF    
	char CMGF[] = "AT+CMGF=0\x0D";// PDU模式
	char ATV[] = "ATV1\x0D";//1 <CR><LF><verbose code><CR><LF> 0 <numeric code><CR>	"ATV0\x0D";返回“0\x0D”（ATV指令决定返回result code是数字格式还是字符格式，ATV0 返回为数字格式，其中0 表示OK，1 表示CONNECT，2 表示RING，3 表示NO CARRIER，4 表示ERROR
	char DUSBDEG[] = "AT^DUSBDEG=1,1\x0D";//返回“0\x0D”,（此指令为开启EP1 通信口，第一个参数从1-4 对应EP1-EP4，第二个参数1 表示open，0 表示close；在启用某	个通信口做业务之前需通过此指令开启对应通信口）
	char DGPIOSM[] = "AT^DGPIOSM=1\x0D";//r”，返回“0\x0D”（, 此指令为APP与模块唤醒模式选择）
	char DSQ[] = "AT^DSQ\x0D";//返回“0\x0D”,（此指令为开启网络信号分贝值的上报）；
	char CRC[] = "AT+CRC=1\x0D";//返回“0\x0D”,（此指令为开启来电后的扩展指示，可选	输入指令）；
	char DSCI[] = "AT^DSCI=1\x0D";//，返回“0\x0D”,（此指令为开启进度指示，可选输入指令）；
	char DCPI[] = "AT^DCPI=1\x0D";//返回“0\x0D”, （此指令为开启呼叫状态指示，以拨打语	音电话10086 为例，开启此上报指示后，其后跟随DCPI 的上报指示，
	//“^DCPI:1,0,0,1”为建立连接指示；“^DCPI:1,3,0,1” 为正在呼叫指示；
	//“^DCPI:1,4,0,1” 为信道分配指示；“^DCPI:1,5,0,1” 为描述进度指示；
	//“^DCPI:1,2,0,1”为被叫振铃指示；“^DCPI:1,6,0,1”为呼叫建立指示，可选输入指令）；

	bool result = true;
	char ans[128];      // 应答串    

	m_pCom->WriteComm(AT, strlen(AT));   
	m_pCom->ReadComm(ans, 128);   
	if (strstr(ans, "OK") == NULL)  
		return false;   

	m_pCom->WriteComm(ATE, strlen(ATE));   
	m_pCom->ReadComm(ans, 128);       
	m_pCom->WriteComm(CMGF, strlen(CMGF));   
	m_pCom->ReadComm(ans, 128); 
	//m_pCom->WriteComm(ATV, strlen(ATV));
	//m_pCom->ReadComm(ans, 128);
	//m_pCom->WriteComm(DUSBDEG, strlen(DUSBDEG));
	//m_pCom->ReadComm(ans, 128);
	//m_pCom->WriteComm(DGPIOSM, strlen(DGPIOSM));
	//m_pCom->ReadComm(ans, 128);
	//m_pCom->WriteComm(DSQ, strlen(DSQ));
	//m_pCom->ReadComm(ans, 128);
	//m_pCom->WriteComm(CRC, strlen(CRC));
	//m_pCom->ReadComm(ans, 128);
	//m_pCom->WriteComm(DSCI, strlen(DSCI));
	//m_pCom->ReadComm(ans, 128);
	//m_pCom->WriteComm(DCPI, strlen(DCPI));
	//m_pCom->ReadComm(ans, 128);
	if (strstr(ans, "OK\x0D\n") == NULL)
	{
		result = false;
	}
	return result;
}

bool ATCommandWarp::On(const char* pin)
{
	char CREG[] = "AT+CREG=1\x0D";//返回“0\x0D”（此指令为开启网络上报，可选输入指令）；
	char CMER[] = "AT+CMER =2,0,0,2\x0D";//r”，返回“0\x0D”（此指令为开启高层协议栈事件CIEV：	<ind>,<value>上报）；
	char CFUN5[] = "AT+CFUN=5\x0D";//返回“0\x0D”，（此指令为激活USIM 卡）；
	char CFUN1[] = "AT+CFUN=1\x0D";//，返回“0\x0D”，（此指令为激活协议栈）；
	char COPS[] = "AT+COPS=0\x0D";//返回“0\x0D”，(此指令为搜网指令),此后LC6311 会返
	//回很多数据，直到返回“+CREG：1”表示开机成功（“^DACTI:2”表示当前接入技术是TD; “+CIEV：2,2”表示信号等级为2 级；“+CIEV：8,0”表示短
	//消息存储未满；“+CREG：2”表示搜网中；“+CREG：1”表示搜网成功并注
	//册成功。【说明：“+CREG:n”其中“n”的值可为0[未注册]，1[注册成功]，2[搜网]，3[网络拒绝注册]，4[网络注册状态未知]，5[漫游]，开机后只有当CREG
	//返回的是1 或5 时，才能做LC6311 所支持的业务】；鉴于对CREG：4 的协议
	//理解，建议当+CREG：4 时，APP 处理器在UI 显示为可提供服务状态，即等
	//同于+CREG：1或+CREG:5】）。

	bool result = true;
	//char ans[128];      // 应答串    

	//m_pCom->WriteComm(CREG, strlen(CREG));
	//m_pCom->ReadComm(ans, 128);
	//m_pCom->WriteComm(CMER, strlen(CMER));
	//m_pCom->ReadComm(ans, 128);
	//m_pCom->WriteComm(CFUN5, strlen(CFUN5));
	//m_pCom->ReadComm(ans, 128);
	//if (strstr(ans, "+CME ERROR:11") != NULL)//在设置开机PIN 码时，APP端输入“AT+CFUN=5”，激活USIM 卡，返回的“＋CME		ERROR：11”表示“SIM PIN Required”；此时APP 端会弹出提示输入pin码的提示，		APP端输入PIN 码后，再执行激活协议栈的指令。
	//{
	//	m_pCom->WriteComm((void*)pin, strlen(pin));
	//	m_pCom->ReadComm(ans, 128);
	//	if (strstr(ans, "OK\x0D\n") == NULL)
	//	{
	//		result = false;
	//	}
	//}
	//m_pCom->WriteComm(CFUN1, strlen(CFUN1));
	//m_pCom->ReadComm(ans, 128);
	//m_pCom->WriteComm(COPS, strlen(COPS));
	//m_pCom->ReadComm(ans, 128);
	return result;
}

bool ATCommandWarp::Off(void)
{
	char COPS[] = "AT+COPS=2\x0D";//返回“+CREG: 0\x0D\n0\x0D”（此指令为去激活网络）；
	char CFUN0[] = "AT+CFUN=0\x0D";//返回“0\x0D”（此指令为去激活协议栈）；
	char CFUN6[] = "AT+CFUN=6\x0D";//返回“0\x0D”（此指令为去激活USIM 卡）；
	char DSOFF[] = "AT^DSOFF\x0D";//，返回“0\x0D”（此指令为模块软关机）；

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
	if (strstr(ans, "OK\x0D\n") == NULL)
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
	strcat(CMD, ";\x0D");
	bool result = true;
	//char ans[128];      // 应答串    
	m_pCom->WriteComm(CMD, strlen(CMD));
	//	m_pCom->ReadComm(ans, 128);
	//	result = *ans;
	return result;	
}

bool ATCommandWarp::PhoneRedial(void)
{
	char ATDL[] = "ATDL;\x0D";
	bool result = true;
	char ans[128];      // 应答串    
	m_pCom->WriteComm(ATDL, strlen(ATDL));
	m_pCom->ReadComm(ans, 128);
	return result;	
}

int ATCommandWarp::PhoneState(void)
{
	char CLCC[] = "AT+CLCC\x0D";
	char result = -1;
	char ans[128];      // 应答串    
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
			p += 9;//+CLCC: 1,0,2,0,0,"10086",129\x0D\n
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
		else if (strstr(ans, "OK\x0D\n") != NULL)
		{
			result = 6;
		}
	}
	return result;	
}

bool ATCommandWarp::PhoneRing(char * number, int * type)
{
	char CLCC[] = "AT+CLCC\x0D";//注意：来电消息是主动上报的，这里还需要修改。 +CRING: VOICE\x0D\n+CLIP:"10086",161,"",,"",0\x0D\n
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
			p += 9;//+CLCC: 1,0,2,0,0,"10086",129\x0D\n
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
	//char CPAS[] = "AT+CPAS\x0D";
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
	char ATH[] = "ATH\x0D";//对于挂断AT指令，AT＋CHUP（挂断当前激活CS链路）和AT＋CHLD 也可作为可选AT指令。被动挂断，则LC6311 会给APP 上报“3\x0D”，3 表示“NO CARRIER”,为对端主动挂断
	bool result = true;
	char ans[128];      // 应答串    
	m_pCom->WriteComm(ATH, strlen(ATH));
	m_pCom->ReadComm(ans, 128);
	return result;	
}

bool ATCommandWarp::PhoneAnswer(void)
{
	char ATA[] = "ATA\x0D";//对于挂断AT指令，AT＋CHUP（挂断当前激活CS链路）和AT＋CHLD 也可作为可选AT指令。被动挂断，则LC6311 会给APP 上报“3\x0D”，3 表示“NO CARRIER”,为对端主动挂断
	bool result = true;
	char ans[128];      // 应答串    
	m_pCom->WriteComm(ATA, strlen(ATA));
	m_pCom->ReadComm(ans, 128);
	return result;	
}

bool ATCommandWarp::PhoneSubDial(char number)
{
	char cmd[16];       // 命令串    
	sprintf(cmd, "AT+VTS=%c\x0D", number); // 生成命令    
	m_pCom->WriteComm(cmd, strlen(cmd));
	char ans[128];      // 应答串    
	m_pCom->ReadComm(ans, 128);
	if(strstr(ans, "OK\x0D\n> ") != NULL)   
	{
		return true;
	}
	return false;
}

bool ATCommandWarp::SmsSend(int dataLength)
{
	char cmd[16];       // 命令串    
	sprintf(cmd, "AT+CMGS=%d\x0D", dataLength); // 生成命令    
	m_pCom->WriteComm(cmd, strlen(cmd));
	int nLength;        // 串口收到的数据长度    
	char ans[128];      // 应答串    
	nLength = m_pCom->ReadComm(ans, 128);
	if(nLength == 4 && strncmp(ans, "\x0D\n> ", 4) == 0)   
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
	sprintf(cmd, "AT+CMGD=%d\x0D", index);    // 生成命令    
	// 输出命令串    
	return m_pCom->WriteComm(cmd, strlen(cmd)); 
}

int ATCommandWarp::SmsReadList(void)
{
	char cmd[] = "AT+CMGL=0\x0D";
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
