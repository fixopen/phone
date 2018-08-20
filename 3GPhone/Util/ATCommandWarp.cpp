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
	char AT[] = "AT\x0D";// ����GSM-MODEM�Ĵ�����    
	char ATE[] = "ATE0\x0D";// ECHO OFF    
	char CMGF[] = "AT+CMGF=0\x0D";// PDUģʽ
	char ATV[] = "ATV1\x0D";//1 <CR><LF><verbose code><CR><LF> 0 <numeric code><CR>	"ATV0\x0D";���ء�0\x0D����ATVָ���������result code�����ָ�ʽ�����ַ���ʽ��ATV0 ����Ϊ���ָ�ʽ������0 ��ʾOK��1 ��ʾCONNECT��2 ��ʾRING��3 ��ʾNO CARRIER��4 ��ʾERROR
	char DUSBDEG[] = "AT^DUSBDEG=1,1\x0D";//���ء�0\x0D��,����ָ��Ϊ����EP1 ͨ�ſڣ���һ��������1-4 ��ӦEP1-EP4���ڶ�������1 ��ʾopen��0 ��ʾclose��������ĳ	��ͨ�ſ���ҵ��֮ǰ��ͨ����ָ����Ӧͨ�ſڣ�
	char DGPIOSM[] = "AT^DGPIOSM=1\x0D";//r�������ء�0\x0D����, ��ָ��ΪAPP��ģ�黽��ģʽѡ��
	char DSQ[] = "AT^DSQ\x0D";//���ء�0\x0D��,����ָ��Ϊ���������źŷֱ�ֵ���ϱ�����
	char CRC[] = "AT+CRC=1\x0D";//���ء�0\x0D��,����ָ��Ϊ������������չָʾ����ѡ	����ָ���
	char DSCI[] = "AT^DSCI=1\x0D";//�����ء�0\x0D��,����ָ��Ϊ�������ָʾ����ѡ����ָ���
	char DCPI[] = "AT^DCPI=1\x0D";//���ء�0\x0D��, ����ָ��Ϊ������״ָ̬ʾ���Բ�����	��绰10086 Ϊ����������ϱ�ָʾ��������DCPI ���ϱ�ָʾ��
	//��^DCPI:1,0,0,1��Ϊ��������ָʾ����^DCPI:1,3,0,1�� Ϊ���ں��ָʾ��
	//��^DCPI:1,4,0,1�� Ϊ�ŵ�����ָʾ����^DCPI:1,5,0,1�� Ϊ��������ָʾ��
	//��^DCPI:1,2,0,1��Ϊ��������ָʾ����^DCPI:1,6,0,1��Ϊ��н���ָʾ����ѡ����ָ���

	bool result = true;
	char ans[128];      // Ӧ��    

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
	char CREG[] = "AT+CREG=1\x0D";//���ء�0\x0D������ָ��Ϊ���������ϱ�����ѡ����ָ���
	char CMER[] = "AT+CMER =2,0,0,2\x0D";//r�������ء�0\x0D������ָ��Ϊ����߲�Э��ջ�¼�CIEV��	<ind>,<value>�ϱ�����
	char CFUN5[] = "AT+CFUN=5\x0D";//���ء�0\x0D��������ָ��Ϊ����USIM ������
	char CFUN1[] = "AT+CFUN=1\x0D";//�����ء�0\x0D��������ָ��Ϊ����Э��ջ����
	char COPS[] = "AT+COPS=0\x0D";//���ء�0\x0D����(��ָ��Ϊ����ָ��),�˺�LC6311 �᷵
	//�غܶ����ݣ�ֱ�����ء�+CREG��1����ʾ�����ɹ�����^DACTI:2����ʾ��ǰ���뼼����TD; ��+CIEV��2,2����ʾ�źŵȼ�Ϊ2 ������+CIEV��8,0����ʾ��
	//��Ϣ�洢δ������+CREG��2����ʾ�����У���+CREG��1����ʾ�����ɹ���ע
	//��ɹ�����˵������+CREG:n�����С�n����ֵ��Ϊ0[δע��]��1[ע��ɹ�]��2[����]��3[����ܾ�ע��]��4[����ע��״̬δ֪]��5[����]��������ֻ�е�CREG
	//���ص���1 ��5 ʱ��������LC6311 ��֧�ֵ�ҵ�񡿣����ڶ�CREG��4 ��Э��
	//��⣬���鵱+CREG��4 ʱ��APP ��������UI ��ʾΪ���ṩ����״̬������
	//ͬ��+CREG��1��+CREG:5������

	bool result = true;
	//char ans[128];      // Ӧ��    

	//m_pCom->WriteComm(CREG, strlen(CREG));
	//m_pCom->ReadComm(ans, 128);
	//m_pCom->WriteComm(CMER, strlen(CMER));
	//m_pCom->ReadComm(ans, 128);
	//m_pCom->WriteComm(CFUN5, strlen(CFUN5));
	//m_pCom->ReadComm(ans, 128);
	//if (strstr(ans, "+CME ERROR:11") != NULL)//�����ÿ���PIN ��ʱ��APP�����롰AT+CFUN=5��������USIM �������صġ���CME		ERROR��11����ʾ��SIM PIN Required������ʱAPP �˻ᵯ����ʾ����pin�����ʾ��		APP������PIN �����ִ�м���Э��ջ��ָ�
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
	char COPS[] = "AT+COPS=2\x0D";//���ء�+CREG: 0\x0D\n0\x0D������ָ��Ϊȥ�������磩��
	char CFUN0[] = "AT+CFUN=0\x0D";//���ء�0\x0D������ָ��Ϊȥ����Э��ջ����
	char CFUN6[] = "AT+CFUN=6\x0D";//���ء�0\x0D������ָ��Ϊȥ����USIM ������
	char DSOFF[] = "AT^DSOFF\x0D";//�����ء�0\x0D������ָ��Ϊģ����ػ�����

	bool result = true;
	char ans[128];      // Ӧ��  

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
	//char ans[128];      // Ӧ��    
	m_pCom->WriteComm(CMD, strlen(CMD));
	//	m_pCom->ReadComm(ans, 128);
	//	result = *ans;
	return result;	
}

bool ATCommandWarp::PhoneRedial(void)
{
	char ATDL[] = "ATDL;\x0D";
	bool result = true;
	char ans[128];      // Ӧ��    
	m_pCom->WriteComm(ATDL, strlen(ATDL));
	m_pCom->ReadComm(ans, 128);
	return result;	
}

int ATCommandWarp::PhoneState(void)
{
	char CLCC[] = "AT+CLCC\x0D";
	char result = -1;
	char ans[128];      // Ӧ��    
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
	char CLCC[] = "AT+CLCC\x0D";//ע�⣺������Ϣ�������ϱ��ģ����ﻹ��Ҫ�޸ġ� +CRING: VOICE\x0D\n+CLIP:"10086",161,"",,"",0\x0D\n
	bool result = false;
	char ans[128];      // Ӧ��    
	m_pCom->WriteComm(CLCC, strlen(CLCC));
	m_pCom->ReadComm(ans, 128);

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
	//char ans[128];      // Ӧ��    
	//m_pCom->WriteComm(CPAS, strlen(CPAS));
	//m_pCom->ReadComm(ans, 128);
	//if (strstr(ans, "+CPAS:3") != NULL)
	//{
	//}
	return result;
}

bool ATCommandWarp::PhoneHangup(void)
{
	char ATH[] = "ATH\x0D";//���ڹҶ�ATָ�AT��CHUP���Ҷϵ�ǰ����CS��·����AT��CHLD Ҳ����Ϊ��ѡATָ������Ҷϣ���LC6311 ���APP �ϱ���3\x0D����3 ��ʾ��NO CARRIER��,Ϊ�Զ������Ҷ�
	bool result = true;
	char ans[128];      // Ӧ��    
	m_pCom->WriteComm(ATH, strlen(ATH));
	m_pCom->ReadComm(ans, 128);
	return result;	
}

bool ATCommandWarp::PhoneAnswer(void)
{
	char ATA[] = "ATA\x0D";//���ڹҶ�ATָ�AT��CHUP���Ҷϵ�ǰ����CS��·����AT��CHLD Ҳ����Ϊ��ѡATָ������Ҷϣ���LC6311 ���APP �ϱ���3\x0D����3 ��ʾ��NO CARRIER��,Ϊ�Զ������Ҷ�
	bool result = true;
	char ans[128];      // Ӧ��    
	m_pCom->WriteComm(ATA, strlen(ATA));
	m_pCom->ReadComm(ans, 128);
	return result;	
}

bool ATCommandWarp::PhoneSubDial(char number)
{
	char cmd[16];       // ���    
	sprintf(cmd, "AT+VTS=%c\x0D", number); // ��������    
	m_pCom->WriteComm(cmd, strlen(cmd));
	char ans[128];      // Ӧ��    
	m_pCom->ReadComm(ans, 128);
	if(strstr(ans, "OK\x0D\n> ") != NULL)   
	{
		return true;
	}
	return false;
}

bool ATCommandWarp::SmsSend(int dataLength)
{
	char cmd[16];       // ���    
	sprintf(cmd, "AT+CMGS=%d\x0D", dataLength); // ��������    
	m_pCom->WriteComm(cmd, strlen(cmd));
	int nLength;        // �����յ������ݳ���    
	char ans[128];      // Ӧ��    
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
	char cmd[16];       // ���    
	sprintf(cmd, "AT+CMGD=%d\x0D", index);    // ��������    
	// ������    
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
