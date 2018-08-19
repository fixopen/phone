#include <afxwin.h>
#include "TelephoneWarp.h"

namespace Telephone {
    namespace Logical {
        TelephoneWarp::TelephoneWarp(Util::ATCommand* atChannel)
        : Util::FSM<TelephoneWarp>()
        , at_(atChannel) {
            registerRule(sIdle, ePickup, sPickuping, &TelephoneWarp::showDialpad);
            registerRule(sIdle, eRemoteCall, sIncalling, &TelephoneWarp::showRemoteCallAndRecordCallId);
            registerRule(sPickuping, eDial, sDialing, &TelephoneWarp::waitRemoteAnswer);
            registerRule(sPickuping, eHangup, sIdle, &TelephoneWarp::hideDialpad);
            registerRule(sDialing, eRemotePickup, sConnecting, &TelephoneWarp::hideDialpadAndShowCallController);
            registerRule(sDialing, eHangup, sIdle, &TelephoneWarp::finishWaitRemoteAnswer);
            registerRule(sDialing, eRemoteReject, sRemoteBusy, &TelephoneWarp::finishWaitRemoteAnswer);
            registerRule(sConnecting, eStartRecord, sRecording, &TelephoneWarp::startRecord);
            registerRule(sConnecting, eHangup, sIdle, &TelephoneWarp::hideCallControllerAndRecordHistory);
            registerRule(sConnecting, eRemoteHangup, sRemoteBusy, &TelephoneWarp::recordHistory);
            registerRule(sRecording, eStopRecord, sConnecting, &TelephoneWarp::stopRecordAndSaveFile);
            registerRule(sRecording, eHangup, sIdle, &TelephoneWarp::stopRecordAndSaveFileAndHideCallControllerAndRecordHistory);
            registerRule(sIncalling, eHangup, sConnecting, &TelephoneWarp::showCallController);
            registerRule(sIncalling, eLeaveWord, sRecording, &TelephoneWarp::startRecord);
            registerRule(sIncalling, eRemoteHangup, sIdle, &TelephoneWarp::recordHistory);
            registerRule(sIncalling, eReject, sIdle, &TelephoneWarp::hideRemoteCallAndRecordHistory);
            registerRule(sRemoteBusy, eHangup, sIdle, &TelephoneWarp::hideCallController);
        }

        void TelephoneWarp::Dial(std::string const& number) {
            std::string ATD = "ATD" + number + "\x0D";
            //Util::ATCommand::Write(ATD);
            at_->Write(ATD);
            fireEvent(eDial, 0);
        }

	    void TelephoneWarp::Hangup() {
	        std::string ATH = "ATH\x0D";
            //Util::ATCommand::Write(ATH);
            at_->Write(ATH);
            fireEvent(eHangup, 0);
        }

	    void TelephoneWarp::Answer() {
	        std::string ATA = "ATA\x0D";
            //Util::ATCommand::Write(ATA);
            at_->Write(ATA);
            fireEvent(ePickup, 0);
        }

        void TelephoneWarp::Reject() {
        }

	    void TelephoneWarp::Redial() {
	        std::string ATDL = "ATDL;\x0D";
            //Util::ATCommand::Write(ATDL);
            at_->Write(ATDL);
            fireEvent(eDial, 0);
        }

	    void TelephoneWarp::SubDial(char const number) {
            std::string cmd = "AT+VTS=";
            cmd += number;
            cmd += "\x0D";
            //Util::ATCommand::Write(cmd);
            at_->Write(cmd);
            fireEvent(eDial, 0);
        }

        void TelephoneWarp::showDialpad(void* p) {
            showDialpad_(p);
		}

        void TelephoneWarp::waitRemoteAnswer(void* p) {
            waitRemoteAnswer_(p);
		}

        void TelephoneWarp::hideDialpadAndShowCallController(void* p) {
            hideDialpad_(p);
            showCallController_(p);
		}

        void TelephoneWarp::startRecord(void* p) {
            startRecord_(p);
		}

        void TelephoneWarp::stopRecordAndSaveFile(void* p) {
            stopRecord_(p);
            saveFile_(p);
		}

        void TelephoneWarp::hideCallControllerAndRecordHistory(void* p) {
            hideCallController_(p);
            recordHistory_(p);
		}

        void TelephoneWarp::showRemoteCallAndRecordCallId(void* p) {
            showRemoteCall_(p);
            recordCallId_(p);
		}

        void TelephoneWarp::showCallController(void* p) {
            showCallController_(p);
		}

        void TelephoneWarp::recordHistory(void* p) {
            recordHistory_(p);
		}

        void TelephoneWarp::hideCallController(void* p) {
            hideCallController_(p);
		}

        void TelephoneWarp::hideDialpad(void* p) {
            hideDialpad_(p);
		}

        void TelephoneWarp::finishWaitRemoteAnswer(void* p) {
            finishWaitRemoteAnswer_(p);
		}

        void TelephoneWarp::hideRemoteCallAndRecordHistory(void* p) {
            hideRemoteCall_(p);
            recordHistory_(p);
		}

        void TelephoneWarp::stopRecordAndSaveFileAndHideCallControllerAndRecordHistory(void* p) {
            stopRecord_(p);
            saveFile_(p);
            hideCallController_(0);
            recordHistory_(p);
		}

        void TelephoneWarp::showDialpad_(void* p) {
		}

        void TelephoneWarp::waitRemoteAnswer_(void* p) {
		}

        void TelephoneWarp::hideDialpad_(void* p) {
		}

        void TelephoneWarp::showCallController_(void* p) {
		}

        void TelephoneWarp::startRecord_(void* p) {
		}

        void TelephoneWarp::stopRecord_(void* p) {
		}

        void TelephoneWarp::saveFile_(void* p) {
		}

        void TelephoneWarp::hideCallController_(void* p) {
		}

        void TelephoneWarp::recordHistory_(void* p) {
		}

        void TelephoneWarp::showRemoteCall_(void* p) {
		}

        void TelephoneWarp::recordCallId_(void* p) {
	        std::string CLCC = "AT+CLCC\x0D";
            //Util::ATCommand::Write(CLCC);
            at_->Write(CLCC);
            fireEvent(ePickup, 0);
		}

        void TelephoneWarp::finishWaitRemoteAnswer_(void* p) {
		}

        void TelephoneWarp::hideRemoteCall_(void* p) {
		}

        //OK 0
        //CONNECT 1
        //RING 2
        //NO CARRIER 3
        //ERROR 4
        //BUSY 7
        //+CME ERROR
        //NO DIAL TONE
        //NO DIALTONE

        enum NumberType {
            tUnknown = 129, //IDSN
            tNational = 161, //IDSN
            tInternational = 145, //ISDN
            tNetworkSpecific = 177, //ISDN
        };

        enum Direction {
            dOriginated = 0, //主叫
            dTerminated = 1, //被叫
        };

        enum State {
            sActive = 0,
            sHeld = 1,
            sDialing4O = 2,
            sAlerting4O = 3,
            sIncoming4T = 4,
            sWaiting4T = 5,
            sTerminated = 6,
        };

        enum Mode {
            mVoice = 0,
            mData = 1,
            mFax = 2,
            mVoiceAndData1 = 3,
            mVoiceOrData1 = 4,
            mVoiceOrFax1 = 5,
            mVoiceAndData2 = 6,
            mVoiceOrData2 = 7,
            mVoiceOrFax2 = 8,
            mUnknown = 9,
        };

        enum Conference {
            cNoMultiparty = 0,
            cMultiparty = 1,
        };

        enum ServiceType {
            tAsync, //ASYNC
            tSync, //SYNC
            tRelAsync, //REL ASYNC
            tRelSync, //REL SYNC
            tFax, //FAX
            tVoice, //VOICE
            tGprs, //GPRS PDP_type, PDP_addr // GPRS "IP","x.y.z.w"
        };

        struct CLCCResponseParser { //list current call
            //+CLCC:index,direct,state,mode,conference[,"number",type]<CR><LF>
            CLCCResponseParser(std::string const& content);
            int index; //=1, normal
            Direction direct;
            State state;
            Mode mode;
            Conference conference;
            std::string number;
            NumberType type;
        };

        struct IncomingParser { //ring
            //+CRING: type
            //+CRING: VOICE\x0D\x0A+CLIP:"number",type,"subaddr",subAddrType,"alphaId",CLIValidity\x0D\x0A
            IncomingParser(std::string const& content);
            enum AlphaId {
                //TODO: not understand it
            };
            enum CLIValidity {
                vValid = 0,
                OriginatorWithheld = 1,
                NotAvailable = 2,
            };
            ServiceType serviceType;
            std::string number;
            NumberType type;
            std::string subAddr; //default null
            NumberType subAddrType; //default null
            AlphaId alphaId; //default null
            CLIValidity cliValidity;
        };

        struct CPASResponseParser { //phone activity status of MT
            //+CPAS:state
            CPASResponseParser(std::string const& content);
            enum State {
                sReady = 0,
                sUnavailable = 1,
                sUnknown = 2,
                sRinging = 3,
                sProgressOrHold = 4,
                sSleep = 5,
            };
            State state;
        };

        struct DSCIResponseParser { //State of Call Indicator
            //^DSCI:id,direction,state,mode,conference,"number",type,bs_type,cause,transactionId,"subaddr",subAddrType
            DSCIResponseParser(std::string const& content);
            int id;
            Direction direction;
            State state;
            Mode mode;
            Conference conference;
            std::string number;
            NumberType type;
            ServiceType bs_type; //when mode is 3-8, this field valid
            enum Cause {
                cUnassignedNumber = 0, //unallocated number
                cNoRouteToDestination = 3,
                cChannelUnacceptable = 6,
                cOperatorDeterminedBarring = 8,
                cNormalCallClearing = 16,
                cUserBusy = 17,
                cNoUserResponding = 18,
                cUserAlertingNotAnswer = 19,
                cCallRejected = 21,
                cNumberChanged = 22,
                cPreemption = 25,
                cNonSelectedUserClearing = 26,
                cDestinationOutOfOrder = 27,
                cInvalidNumberFormat = 28, //incomplete number
                cFacilityRejected = 29,
                cResponseToStatusEnquiry = 30,
                cNormalUnspecified = 31,
                cNoCircuitChannelAvailable = 34,
                cNetworkOutOfOrder = 38,
                cTemporaryFailure = 41,
                cSwitchingEquipmentCongestion = 42,
                cAccessInformagtionDiscarded = 43,
                cRequestedCircuitChannelUnavailable = 44,
                cResourceUnavailableUnspecified = 47,
                cQualityOfServiceUnavailable = 49,
                cRequestedFacilityNotSubscribed = 50,
                cIncomingCallsBarredWithinTheCUG = 55,
                cBearerCapabilityNotAuthorized = 57,
                cBearerCapabilityNotPresentlyAvailable = 58,
                cServiceOrOptionNotAvailableUnspecified = 63,
                cBearerServiceNotImplemented = 65,
                cACMEqualToOrGreaterThanACMmax = 68,
                cRequestedFacilityNotImplementesd = 69,
                cOnlyRestrictedDigitalInfoBearerCapabilityIsAvailable = 70,
                cServiceOrOptionNotImplementedUnspecified = 79,
                cInvalidTransactionIdentifierValue = 81,
                cUserNotMemberOfCUG = 87,
                cIncompatibleDestination = 88,
                cInvalidTransitNetworkSelection = 91,
                cSemanticallyIncorrectMessage = 95,
                cInvalidMandatoryInformation = 96,
                cMessageTypeNonexistentOrNotImplemented = 97,
                cMessageTypeNotCompatibleWithProtocolState = 98,
                cInformationElementNonexistentOrNotImplemented = 99,
                cConditionalIEError = 100,
                cMessageNotCompatibleWithProtocolState = 101,
                cRecoveryOnTimerExpiry = 102,
                cProtocolErrorUnspecified = 111,
                cInterworkingUnspecified = 127,
            };
            Cause cause; //only disconnected valid
            int transactionId;
            std::string subAddr;
            NumberType subAddrType;
        };

        struct DCPIResponseParser { //call progress indication
            //^DCPI:id,type,bandTone,TCH
            DCPIResponseParser(std::string const& content);
            enum Type {
                tSetup = 0, //sent out
                tDisconnect = 1,
                tAlerting = 2,
                tCallProceeding = 3,
                tChannelAssignment = 4,
                tProgressDescription = 5,
                tConnect = 6,
            };
            enum BandTone {
                bNoInBandTone = 0,
                bInBandTone = 1,
            };
            enum TCH {
                cNotAssigned = 0,
                cAssigned = 1,
            };
            int id;
            Type type;
            BandTone bandTone;
            TCH tch;
        };

        struct ErrorParser {
            //+CME ERROR:error
            ErrorParser(std::string const& content);
            enum Error {
            };
            Error error;
        };

        void TelephoneWarp::telephoneParser(std::string const& data) {
            TelephoneWarp* thisTelephone = 0;
            //if (data.find("+CRING")) {
            //    IncomingParser* p = new IncomingParser(data);
            //    thisTelephone->fireEvent(eRemoteCall, p);
            //    delete p;
            //} else if (data.find("+CLCC")) {
            //    CLCCResponseParser* p = new CLCCResponseParser(data);
            //    thisTelephone->fireEvent(eRemoteCall, p);
            //    delete p;
            //} else if (data.find("+CPAS")) {
            //    CPASResponseParser* p = new CPASResponseParser(data);
            //    thisTelephone->fireEvent(eRemoteCall, p);
            //    delete p;
            //} else if (data.find("^DSCI")) {
            //    DSCIResponseParser* p = new DSCIResponseParser(data);
            //    //switch (p->cause) {
            //    //    case c
            //    //}
            //    delete p;
            //} else if (data.find("+CME ERROR")) {
            //    ErrorParser* p = new ErrorParser(data);
            //    thisTelephone->fireEvent(eError, p);
            //    delete p;
            //} else {
            //    //error
            //}
        }
    }
}

#define HUNGON_VALUE	0x82		//0x82	 摘机
#define HUNGOFF_VALUE	0x83		//0x83   挂机
#define RING_VALUE		0x84		//0x84   振铃信号
#define WM_TEL_CALLIDEND                8023

using namespace Telephone;

typedef enum
{   
	stBeginRest,                // 开始休息/延时    
	stContinueRest,             // 继续休息/延时    
	stSendPhoneRequest,         // 拨打电话   
	stSendPhoneResponse,        // 读取拨打应答到缓冲区    
	//stSendPhoneWaitIdle,      // 拨打不成功，等待GSM就绪    
	stReadPhoneRequest,         // 发送读取来电列表的命令    
	stReadPhoneResponse,        // 读取来电列表到缓冲区    
	//stDeletePhoneRequest,     // 删除记录    
	//stDeletePhoneResponse,    // 删除记录应答    
	//stDeletePhoneWaitIdle,    // 删除不成功，等待GSM就绪  
	atRedial,
	stAnswer,
	stHangupPhone,			//挂机
	stExitThread                // 退出    
}TelephoneState;               // 处理过程的状态 

static TelephoneState nState = stBeginRest;
TelephoneWarp::TelephoneWarp()
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

TelephoneWarp::~TelephoneWarp()
{
	SetEvent(m_hKillThreadEvent);           // 发出关闭子线程的信号    
	WaitForSingleObject(m_hThreadKilledEvent, INFINITE);    // 等待子线程关闭    

	CloseHandle(m_hKillThreadEvent);   
	CloseHandle(m_hThreadKilledEvent);   
}

TelephoneWarp::TelephoneWarp(const TelephoneWarp& tel)
{
}

TelephoneWarp* TelephoneWarp::GetTelephoneWarp()
{
	static TelephoneWarp* tw = 0;
	if (tw == 0)
	{
		tw = new TelephoneWarp();
	}
	return tw;
}

void TelephoneWarp::Bind(Util::ATCommandWarp* at)
{
	m_pAT = at;
	// 启动子线程    
	AfxBeginThread(TelephoneThread, this, THREAD_PRIORITY_NORMAL);   
}

// 将一条短消息放入发送队列    
void TelephoneWarp::PutSendMessage(TEL_NUM* pparam)   
{   
	EnterCriticalSection(&m_csSend);   

	memcpy(&m_PhoneSend[m_nSendIn], pparam, sizeof(TEL_NUM));   

	m_nSendIn++;   
	if (m_nSendIn >= MAX_PHONE_SEND)  m_nSendIn = 0;   

	LeaveCriticalSection(&m_csSend);   
}   

// 从发送队列中取一条短消息    
BOOL TelephoneWarp::GetSendMessage(TEL_NUM* pparam)   
{   
	BOOL fSuccess = FALSE;   

	EnterCriticalSection(&m_csSend);   

	if (m_nSendOut != m_nSendIn)   
	{   
		memcpy(pparam, &m_PhoneSend[m_nSendOut], sizeof(TEL_NUM));   

		m_nSendOut++;   
		if (m_nSendOut >= MAX_PHONE_SEND)  m_nSendOut = 0;   

		fSuccess = TRUE;   
	}   

	LeaveCriticalSection(&m_csSend);   

	return fSuccess;   
}   

// 将短消息放入接收队列    
void TelephoneWarp::PutRecvMessage(TEL_NUM* pparam, int nCount)   
{   
	EnterCriticalSection(&m_csRecv);   

	for (int i = 0; i < nCount; i++)   
	{   
		memcpy(&m_PhoneRecv[m_nRecvIn], pparam, sizeof(TEL_NUM));   

		m_nRecvIn++;   
		if (m_nRecvIn >= MAX_PHONE_RECV)  m_nRecvIn = 0;   

		pparam++;   
	}   

	LeaveCriticalSection(&m_csRecv);   
}   

// 从接收队列中取一条短消息    
BOOL TelephoneWarp::GetRecvMessage(TEL_NUM* pparam)   
{   
	BOOL fSuccess = FALSE;   

	EnterCriticalSection(&m_csRecv);   

	if (m_nRecvOut != m_nRecvIn)   
	{   
		memcpy(pparam, &m_PhoneRecv[m_nRecvOut], sizeof(TEL_NUM));   

		m_nRecvOut++;   
		if (m_nRecvOut >= MAX_PHONE_RECV)  m_nRecvOut = 0;   

		fSuccess = TRUE;   
	}   

	LeaveCriticalSection(&m_csRecv);   

	return fSuccess;   
}   

UINT TelephoneWarp::TelephoneThread(LPVOID lParam)
{
	TelephoneWarp* p=(TelephoneWarp *)lParam;   // this    
	//int nMsg;               // 收到电话条数    
	//int nDelete;            // 目前正在删除的记录编号    
	char buff[1024];           // 接收电话列表的缓冲区   来电号码 
	TEL_NUM param[256];    // 发送/接收电话缓冲区    
	char* state = NULL;

	CTime tmOrg, tmNow;     // 上次和现在的时间，计算超时用    
	//enum {   
	//	stBeginRest,                // 开始休息/延时    
	//	stContinueRest,             // 继续休息/延时    
	//	stSendPhoneRequest,         // 拨打电话   
	//	stSendPhoneResponse,        // 读取拨打应答到缓冲区    
	//	stSendPhoneWaitIdle,      // 拨打不成功，等待GSM就绪    
	//	stReadPhoneRequest,       // 发送读取来电列表的命令    
	//	stReadPhoneResponse,      // 读取来电列表到缓冲区    
	//	stDeletePhoneRequest,     // 删除记录    
	//	stDeletePhoneResponse,    // 删除记录应答    
	//	stDeletePhoneWaitIdle,    // 删除不成功，等待GSM就绪    
	//	stExitThread                // 退出    
	//} nState;               // 处理过程的状态    


	// 初始状态    
	//nState = stBeginRest;   

	// 发送和接收处理的状态循环    
	while (nState != stExitThread)   
	{   
		switch(nState)   
		{   
		case stBeginRest:   
			TRACE(L"State=stBeginRest\n");    
			tmOrg = CTime::GetCurrentTime();   
			nState = stContinueRest;   
			break;   

		case stContinueRest:   
			TRACE(L"State=stContinueRest\n");    
			Sleep(300);   
			tmNow = CTime::GetCurrentTime();   
			if (p->GetSendMessage(param))   
			{   
				nState = stSendPhoneRequest;  // 有待拨打电话，就不休息了    
			}   
			else if (tmNow - tmOrg >= 2)     // 待拨打电话队列空，休息2秒    
			{   
				nState = stReadPhoneRequest;  // 转到读来电状态    
			}   
			break;   

		case stSendPhoneRequest:   
			TRACE(L"State=stSendPhoneRequest\n");    
			p->Dial_(param[0]);   
			memset(&buff, 0, sizeof(buff));   
			tmOrg = CTime::GetCurrentTime();   
			nState = stSendPhoneResponse;   
			break;   

		case stSendPhoneResponse:   
			TRACE(L"State=stSendPhoneResponse\n");    
			Sleep(1000);   
			p->State_();
			break;

		case stReadPhoneRequest:   
			TRACE(L"State=stReadPhoneRequest\n");    
			memset(&buff, 0, sizeof(buff));   
			if (p->Incoming_(&param[0]))
			{
				p->Incoming(param[0].NUM);
				nState = stReadPhoneResponse;
			}
			else
			{
				nState = stBeginRest;   
			}
			break;   

		case stReadPhoneResponse:   
			TRACE(L"State=stReadPhoneResponse\n");    
			Sleep(1000);   
			p->State_();
			break;   

		case atRedial:
			TRACE(L"State=atRedial\n");    
			p->Redial_();
			nState = stSendPhoneResponse;  
			break;

		case stAnswer:
			TRACE(L"State=stAnswer\n");    
			p->Answer_();
			nState = stReadPhoneResponse;  
			break;

		case stHangupPhone:
			TRACE(L"State=stHangupPhone\n");    
			p->Hangup_();
			nState = stBeginRest;
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

bool TelephoneWarp::Dial_(TEL_NUM num)
{
	return m_pAT->PhoneDial(num.NUM);
}

unsigned char TelephoneWarp::State_(void)
{
	unsigned char state = m_pAT->PhoneState();
	switch (state)
	{
	case 0:
		TRACE(L"Connected\n");
		Connected();
		break;
	case 1:
		TRACE(L"Held\n");
		Held();
		break;
	case 2:
		TRACE(L"Dialing\n");
		Dialing();
		break;
	case 3:
		TRACE(L"Alerting\n");
		Alerting();
		break;
	case 4://incoming
		TRACE(L"Incoming\n");
		Ring();
		break;
	case 5:
		TRACE(L"Waiting\n");
		Waiting();
		break;
	case 6:
		TRACE(L"OppHangup\n");
		OppHangup();
		break;
	case 10:
		TRACE(L"NO DIALTONE\n");
		NoDialTone();
		break;
	case 11:
		TRACE(L"BUSY\n");
		Busy();
		break;
	case 12:
		TRACE(L"NO ANSWER\n");
		NoAnswer();
		break;
	case 13:
		TRACE(L"NO CARRIER\n");
		NoCarrier();
		break;
	case 14:
		TRACE(L"CONNECT\n");
		Connect();
		break;
	case 15:
		TRACE(L"NW CONGESTION\n");
		Congestion();
		break;
	case 16:
		TRACE(L"ERROR ODB\n");
		Odb();
		break;
	}
	return state;
}

bool TelephoneWarp::Hangup_(void)
{
	return m_pAT->PhoneHangup();
}

bool TelephoneWarp::Answer_(void)
{
	return m_pAT->PhoneAnswer();
}

bool TelephoneWarp::Redial_()
{
	return m_pAT->PhoneRedial();
}

bool TelephoneWarp::Incoming_(TEL_NUM* num)
{
	return m_pAT->PhoneRing(num->NUM, &num->TYPE);
}
//////////////////////////////////////////////////////////////////////////
bool TelephoneWarp::Dial(char* number)
{
	TEL_NUM num;
	memset(&num, 0, sizeof(TEL_NUM));
	strcpy(num.NUM, number);
	PutSendMessage(&num);
	return true;
}
bool TelephoneWarp::Hangup(void)
{
	nState = stHangupPhone;
	return true;
}
bool TelephoneWarp::Answer(void)
{
	nState = stAnswer;
	return true;
}
bool TelephoneWarp::Redial(void)
{
	nState = atRedial;
	return true;
}
bool TelephoneWarp::SubDial(char number)
{
	return m_pAT->PhoneSubDial(number);
}
//////////////////////////////////////////////////////////////////////////
void TelephoneWarp::Incoming(char* num)
{
	static char CallID[128];
	memset(CallID, 0, 128);
	strcpy(CallID, num);
//	PostMessage(theApp.m_pMainWnd->m_hWnd, WM_TEL_CALLIDEND, (WPARAM)CallID, 0);
	//TRACE(CallID);
}
void TelephoneWarp::Ring(void)//来电振铃
{
//	PostMessage(theApp.m_pMainWnd->m_hWnd, WM_TEL_RING, 0, 0);
}
void TelephoneWarp::Connected(void)//通话联通状态
{
}
void TelephoneWarp::Held(void)//保持状态
{
}
void TelephoneWarp::Dialing(void)//正在拨打状态
{
}
void TelephoneWarp::Alerting(void)//对方振铃状态
{
}
void TelephoneWarp::Waiting(void)
{
}
void TelephoneWarp::NoDialTone(void)
{
}
void TelephoneWarp::Busy(void)//对方忙
{
}
void TelephoneWarp::NoAnswer(void)//无应答
{
}
void TelephoneWarp::NoCarrier(void)//连接不能被建立
{
}
void TelephoneWarp::Connect(void)//对方接听
{
}
void TelephoneWarp::Congestion(void)//网络拥塞
{
}
void TelephoneWarp::OppHangup(void)//对方挂机
{
	nState = stBeginRest;
}
void TelephoneWarp::Odb(void)//呼叫限制
{
}