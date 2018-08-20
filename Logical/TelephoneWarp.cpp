#include "stdafx.h"
#include "TelephoneWarp.h"
#include "../multimediaphone.h"
#include "../multimediaphoneDlg.h"
#include "../resource.h"

#define HUNGON_VALUE	0x82		//0x82	 摘机
#define HUNGOFF_VALUE	0x83		//0x83   挂机
#define RING_VALUE		0x84		//0x84   振铃信号
//#define WM_TEL_CALLIDEND                8023

#define DEFUALT_SOUND_VALUME   -1500

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
	stRedial,
	stAnswer,
	stHangupPhone,			//挂机
	stExitThread                // 退出    
}TelephoneState;               // 处理过程的状态 

static unsigned char g_tel_code[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '*', '#', 'A', 'B', 'C', 'D'};

char *gDetectString  = "1*0#*0#*0#*0#1";
char *gDetectString1 = "*#1579#";
char *gSetUserIDString = "*#357#"; //"3*0#*0#*0#*0#3";
static char gTelVersion1_[128];
int gBatteryLevel[] = {830, 805, 780, 755, 720};
int gBatteryOffset = 110;
int gBatteryBase = 720;
int TelephoneWarp::DetectTestStatus(unsigned char c)
{
	int isBatteryStatus = 0;
	CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;

	if(c == 'A')
	{
		main->phone_->m_bInputtingUserID = TRUE;
		memset(main->phone_->m_chDetectCode, 0, 64);
		main->phone_->m_chDetectCode[strlen(main->phone_->m_chDetectCode)] = c;
	}
	else if(main->phone_->m_bInputtingUserID)
	{
		if(strlen(main->phone_->m_chDetectCode) < 63)
			main->phone_->m_chDetectCode[strlen(main->phone_->m_chDetectCode)] = c;
	}
	if(c == '\x0d')
	{
		main->phone_->m_bInputtingUserID = FALSE;
		char *ptr;
		if((ptr = strstr(main->phone_->m_chDetectCode, "AT=")) > 0)
		{
			char *p = strstr(ptr+3, ";");
			if(!p)
			{
				p = strstr(ptr+3, "'");
			}
			if(p)
			{
				char txt[4] = {0};
				memcpy(txt, ptr+3, p-ptr-3);
				CString s = txt;
				int n = Util::StringOp::ToInt(txt, 10);

				isBatteryStatus = 1;
				if(strstr(ptr, "CHG=2"))
				{
					main->phone_->m_BatteryStatus.isCharge = 0;
					isBatteryStatus = 2;
					int off = n - gBatteryBase;
					int index = 1;
					for(int i = 3; i >= 0; i--)
					{
						gBatteryLevel[i] = gBatteryBase+ (gBatteryOffset*25*index)/off;
						index++;
					}
				}

				for(int i = 0; i < 5; i++)
				{
					if(n >= gBatteryLevel[i])
					{
						i++;
						break;
					}
				}
				main->phone_->m_BatteryStatus.batteryProccess = i;
			}
		}
		if((ptr = strstr(main->phone_->m_chDetectCode, "CHG=")) > 0)
		{
			char *p = strstr(ptr+4, ";");
			if(!p)
			{
				p = strstr(ptr+4, "'");
			}
			if(p)
			{
				isBatteryStatus = 1;
				char txt[4] = {0};
				memcpy(txt, ptr+4, p-ptr-4);
				if(strstr(txt,"1"))
				{
					main->phone_->m_BatteryStatus.isCharge = 1;
					main->phone_->m_BatteryStatus.batteryType = BATTERY_DC;
				}
				else if(strstr(txt,"0"))
				{
					main->phone_->m_BatteryStatus.isCharge = 0;
				}
			}
		}
		if((ptr = strstr(main->phone_->m_chDetectCode, "SRC=")) > 0)
		{
			char *p = strstr(ptr+4, ";");
			if(!p)
			{
				p = strstr(ptr+4, "'");
			}
			if(p)
			{
				isBatteryStatus = 1;
				char txt[4] = {0};
				memcpy(txt, ptr+4, p-ptr-4);
				if(strstr(txt,"1"))
				{
					main->phone_->m_BatteryStatus.batteryType = BATTERY_DC;
				}
				else
				{
					main->phone_->m_BatteryStatus.batteryType = BATTERY_1;
				}
			}
		}
	}
	return isBatteryStatus;
}

void ParseTelephoneData(unsigned char const* const data, unsigned int const length)
{
	Sleep(10);
	int i;
	static BOOL isRingTelCode = FALSE;
	static CALLID_INFO	CallID;
	static UINT8   CallIDbuff[128];
	static int     CallIDLen = 0;

	for (i=0; i<length; i++)
		{
		
			unsigned char c = data[i];
			Dprintf("%c", c);
			if(c == '\n' || c == '\r')
				Dprintf("\r\n");
			
			CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
			int ret = main->phone_->DetectTestStatus(c);
			if(ret == 1)
			{
				PostMessage(theApp.m_pMainWnd->m_hWnd, WM_CHANGE_BATTERY, 0, 0);
			}
			else if(ret == 2)
			{
				PostMessage(theApp.m_pMainWnd->m_hWnd, WM_CHANGE_BATTERYOK, 0, 0);
			}
		
			switch(c)
			{
				case HUNGON_VALUE:
					PostMessage(theApp.m_pMainWnd->m_hWnd, WM_TEL_HUNGON, 0, 0);
					break;
				case HUNGOFF_VALUE:
 					PostMessage(theApp.m_pMainWnd->m_hWnd, WM_TEL_HUNGOFF, 0, 0);
					if(((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_nTELRigster >= TELRIGSTER_TD)
						Telephone::TelephoneWarp::GetTelephoneWarp()->HandFree(false);
					break;				
				case PAGEUP_VALUE:
					PostMessage(theApp.m_pMainWnd->m_hWnd, WM_KEYDOWN, 'U', 0);
					break;	
				case PAGEDOWN_VALUE:
					PostMessage(theApp.m_pMainWnd->m_hWnd, WM_KEYDOWN, 'D', 0);
					break;				
				case 0xB0: case 0xB1: case 0xB2: case 0xB3: case 0xB4: case 0xB5: case 0xB6:
				case 0xB7: case 0xB8: case 0xB9: case 0xBA: case 0xBB: case 0xBC:
 					PostMessage(theApp.m_pMainWnd->m_hWnd, WM_TEL_KEYCODE, g_tel_code[c-0xB0], 0);
					break;
				case 0xC8:
					if(((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_nTELRigster >= TELRIGSTER_TD )
						Telephone::TelephoneWarp::GetTelephoneWarp()->HandFree(false);
					break;
				case 0xC9:
					if(((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_nTELRigster >= TELRIGSTER_TD )
						Telephone::TelephoneWarp::GetTelephoneWarp()->HandFree(true);
					break;
				case 0xCA:
					if(((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_nTELRigster >= TELRIGSTER_TD )
						Telephone::TelephoneWarp::GetTelephoneWarp()->HandFree(true);
					break;
				case 0xCB:
					if(((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_nTELRigster >= TELRIGSTER_TD )
						Telephone::TelephoneWarp::GetTelephoneWarp()->HandFree(false);
					break;
				case 1: case 2: case 3: case 4: case 5: case 6:	
				case 7: case 8: case 9: case 10: case 11: case 12:
					PostMessage(theApp.m_pMainWnd->m_hWnd, WM_SPEEDDIAL, c, 0);
					/*
					{
						CPoint pt1 ;
						::GetCursorPos(&pt1);
						CPoint pt2 = CPoint(2, 120);
						if(pt1 == pt2)
							pt2.x += 2;
						::SetCursorPos(pt2.x, pt2.y);
						mouse_event(MOUSEEVENTF_LEFTDOWN|MOUSEEVENTF_LEFTUP,0,0,0,0);
					}
					if( c == 1)
					{
						keybd_event(VK_NUMPAD4, 0, 0, 0);
					//	PostMessage(theApp.m_pMainWnd->m_hWnd, WM_KEYDOWN, CANCEL_KEY, 0);	
					}
					else if(c == 2)
					{
						keybd_event(VK_NUMPAD0, 0, 0, 0);
						//PostMessage(theApp.m_pMainWnd->m_hWnd, WM_KEYDOWN, UP_KEY, 0);
					}
					else if(c == 3)
					{
						keybd_event(VK_NUMPAD5, 0, 0, 0);
						//PostMessage(theApp.m_pMainWnd->m_hWnd, WM_KEYDOWN, OK_KEY, 0);
					}
					else if(c == 4)
					{
						keybd_event(VK_NUMPAD2, 0, 0, 0);
						//PostMessage(theApp.m_pMainWnd->m_hWnd, WM_KEYDOWN, LEFT_KEY, 0);
					}
					else if(c == 6)
					{
						keybd_event(VK_NUMPAD3, 0, 0, 0);
						//PostMessage(theApp.m_pMainWnd->m_hWnd, WM_KEYDOWN, RIGHT_KEY, 0);
					}
					else if(c == 8)
					{
						keybd_event(VK_NUMPAD1, 0, 0, 0);
						//PostMessage(theApp.m_pMainWnd->m_hWnd, WM_KEYDOWN, DOWN_KEY, 0);
					}
					*/
					break;
				
				case 0xBD:
					PostMessage(theApp.m_pMainWnd->m_hWnd, WM_REDIAL, 0, 0);
					break;
				/*	case 0xBF:
					PostMessage(theApp.m_pMainWnd->m_hWnd, WM_RKEY, 0, 0);
					break;
				case 1: case 2: case 3: case 4: case 5: case 6:	
				case 7: case 8: case 9: case 10: case 11: case 12:
					PostMessage(theApp.m_pMainWnd->m_hWnd, WM_SPEEDDIAL, c, 0);
					break;
				case 0x94:
					PostMessage(theApp.m_pMainWnd->m_hWnd, WM_HUNGONINRECORD, 0, 0);
					break;
				case 0xa2:
					PostMessage(theApp.m_pMainWnd->m_hWnd, WM_HUNGONTORECORD, 0, 0);
					break;
				case 0x9D:   //并机摘机
					PostMessage(theApp.m_pMainWnd->m_hWnd, WM_BINGJIZHAIJI, 0, 0);
					*/
				default:
					break;
			}
		}
}

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

	//ring tone
	pGraph = NULL;   
	pMediaControl = NULL; 
	pMediaPosition = NULL;
	ipBasAudio       =   NULL;
	pEvent = NULL;
	g_RingSound = -1500; //-(MAX_SOUND-MIN_SOUND)/2; 
	gRingFilename[0] = '\0';
	
	m_chTelVersion[0] = '\0';
	m_chDetectCode[0] = '\0';
	m_bInputtingUserID = FALSE;
	m_bStartRing = FALSE;
	
	m_pMsgWnd = NULL;

	m_BatteryStatus.batteryProccess = 100;
	m_BatteryStatus.batteryType = BATTERY_1;
	m_BatteryStatus.isCharge = 0;

	m_pRS232 = new Util::RS232();
	m_pRS232->OpenPort();
	m_pRS232->SetReadFunc(ParseTelephoneData);

	//end ring tone
	::InitializeCriticalSection(&m_ringSetion_);
	InitRing();
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

void TelephoneWarp::Bind_(Util::ATCommandWarp* at)
{
	m_pAT = at;
}

void TelephoneWarp::Bind(Util::ATCommandWarp* at)
{
//	m_pAT = at;
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
	char buff[1024];           // 接收电话列表的缓冲区   来电号码 
	TEL_NUM param[256];    // 发送/接收电话缓冲区    
	char* state = NULL;
	unsigned int signalOrg = 0;
	unsigned int signalNow = 0;
	int netType = -1;
	int netTypeOrg = -1;

	// 发送和接收处理的状态循环    
	while (nState != stExitThread)   
	{   
		switch(nState)   
		{   
		case stBeginRest:   
			TRACE(L"State=stBeginRest\n");    
			Sleep(1000);
			nState = stContinueRest;   
			EnterCriticalSection(&csCom);
			TRACE(L"Tel Enter\n");
			break;   

		case stContinueRest:   
			TRACE(L"State=stContinueRest\n");    
			if (p->GetSendMessage(param))   
			{   
				nState = stSendPhoneRequest;  // 有待拨打电话，就不休息了    
			}   
			else  
			{   
				nState = stReadPhoneRequest;  // 转到读来电状态    
			}   
			break;   

		case stSendPhoneRequest:   
			TRACE(L"State=stSendPhoneRequest\n");    
			p->Dial_(param[0]);   
			memset(&buff, 0, sizeof(buff));   
			nState = stSendPhoneResponse;   
			break;   

		case stSendPhoneResponse:   
			TRACE(L"State=stSendPhoneResponse\n");    
			Sleep(2000);   
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
				Sleep(1000);
				
				netType = p->PhoneNettype_();
			//	if(netType != netTypeOrg)
				{
					if(netType != 0xF)
					{
						netTypeOrg = netType;
						p->PhoneNettype(netType);
					}
				}
				//TRACE(L"Net %d\r", r);
				signalNow = p->SignalQuality_();
				if (signalNow != 0xFF && signalNow != signalOrg)
				{
					signalOrg = signalNow;
					p->SignalQuality(signalNow);
				}
				LeaveCriticalSection(&csCom);
				TRACE(L"Tel Leave\n");
			   
				nState = stBeginRest;   
			}
				//test lxz
			break;   

		case stReadPhoneResponse:   
			TRACE(L"State=stReadPhoneResponse\n");    
			Sleep(2000);   
			p->State_();
			break;   

		case stRedial:
			TRACE(L"State=stRedial\n");    
			p->Redial_();
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
			LeaveCriticalSection(&csCom);
			TRACE(L"Tel Leave\n");
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
	return m_pAT->PhoneDial(num.NUM, (BOOL)num.TYPE);
}

unsigned char TelephoneWarp::State_(void)
{
	int state = m_pAT->PhoneState();
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
unsigned int TelephoneWarp::SignalQuality_(void)
{
	return m_pAT->PhoneSignalQuality();
}
unsigned int TelephoneWarp::PhoneNettype_(void)
{
	return m_pAT->PhoneNettype();
}

void TelephoneWarp::PhoneDialTone(BOOL isOn, char *tone)
{
	m_pAT->PhoneDialTone(isOn, tone);
}
//////////////////////////////////////////////////////////////////////////
bool TelephoneWarp::Dial(char* number, BOOL isVideo)
{
	TEL_NUM num;
	memset(&num, 0, sizeof(TEL_NUM));
	strcpy(num.NUM, number);
	num.TYPE = isVideo;
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
	nState = stRedial;
	return true;
}
bool TelephoneWarp::SubDial(char number)
{
	return m_pAT->PhoneSubDial(number);
}
bool TelephoneWarp::Volume(unsigned int level)
{
	return m_pAT->PhoneVolume(level);
}
bool TelephoneWarp::Mute(bool isMute)
{
	return m_pAT->PhoneMute(isMute);
}
bool TelephoneWarp::AutoAnswer(unsigned int second)
{
	return m_pAT->PhoneAutoAnswer(second);
}
bool TelephoneWarp::HandFree(bool isHandFree)
{
	return m_pAT->PhoneHandFree(isHandFree);
}
std::string TelephoneWarp::GetNumber(void)
{
	return m_pAT->PhoneNumber();
}
//////////////////////////////////////////////////////////////////////////
void TelephoneWarp::Incoming(char* num)
{
	static CALLID_INFO CallID;
	memset(&CallID, 0, sizeof(CALLID_INFO));
	strcpy(CallID.number, num);
	PostMessage(theApp.m_pMainWnd->m_hWnd, WM_TEL_CALLIDEND, (WPARAM)&CallID, 0);
//	TRACE(CallID);
}
void TelephoneWarp::Ring(void)//来电振铃
{
	PostMessage(theApp.m_pMainWnd->m_hWnd, WM_TEL_RING, 0, 0);
	PostMessage(theApp.m_pMainWnd->m_hWnd, WM_TEL_STATUS, TEL_FROMRING, 0);
}
void TelephoneWarp::Busy(void)//对方忙
{
	PostMessage(theApp.m_pMainWnd->m_hWnd, WM_TEL_STATUS, TEL_FROMBUY, 0);
}
void TelephoneWarp::Connected(void)//通话联通状态
{
	PostMessage(theApp.m_pMainWnd->m_hWnd, WM_TEL_STATUS, TEL_FROMCONNECTED, 0);
}
void TelephoneWarp::Connect(void)//对方接听 一次
{
	PostMessage(theApp.m_pMainWnd->m_hWnd, WM_TEL_STATUS, TEL_FROMCONNECT, 0);
}
void TelephoneWarp::Held(void)//保持状态
{
	PostMessage(theApp.m_pMainWnd->m_hWnd, WM_TEL_STATUS, TEL_FROMHELD, 0);
}
void TelephoneWarp::Dialing(void)//正在拨打状态
{
	PostMessage(theApp.m_pMainWnd->m_hWnd, WM_TEL_STATUS, TEL_FROMDIALING, 0);
}
void TelephoneWarp::Alerting(void)//对方振铃状态
{
	PhoneDialTone(0, NULL);
	PostMessage(theApp.m_pMainWnd->m_hWnd, WM_TEL_STATUS, TEL_FROMALERTING, 0);
}
void TelephoneWarp::Waiting(void)
{
	PostMessage(theApp.m_pMainWnd->m_hWnd, WM_TEL_STATUS, TEL_FROMWAITING, 0);
}
void TelephoneWarp::NoDialTone(void)
{
	PostMessage(theApp.m_pMainWnd->m_hWnd, WM_TEL_STATUS, TEL_FROMNODIALTONE, 0);
}
void TelephoneWarp::NoAnswer(void)//无应答
{
	PhoneDialTone(1, "hangup");
	PostMessage(theApp.m_pMainWnd->m_hWnd, WM_TEL_STATUS, TEL_FROMNOANSWER, 0);
}
void TelephoneWarp::NoCarrier(void)//连接不能被建立
{
	PhoneDialTone(1, "hangup");
	nState = stHangupPhone;
	PostMessage(theApp.m_pMainWnd->m_hWnd, WM_TEL_STATUS, TEL_FROMNOCARRIER, 0);
}
void TelephoneWarp::Congestion(void)//网络拥塞
{
	PostMessage(theApp.m_pMainWnd->m_hWnd, WM_TEL_STATUS, TEL_FROMCONGESTION, 0);
}
void TelephoneWarp::OppHangup(void)//对方挂机
{
	PhoneDialTone(1, "hangup");
	nState = stHangupPhone;
	PostMessage(theApp.m_pMainWnd->m_hWnd, WM_TEL_STATUS, TEL_FROMOPPHUNGUP, 0);
}
void TelephoneWarp::Odb(void)//呼叫限制
{
	PhoneDialTone(1, "hangup");
	PostMessage(theApp.m_pMainWnd->m_hWnd, WM_TEL_STATUS, TEL_FROMODB, 0);
}
void TelephoneWarp::SignalQuality(int level)
{
	PostMessage(theApp.m_pMainWnd->m_hWnd, WM_TEL_STATUS, TEL_SIGNALQUALITY, level);
}
void TelephoneWarp::PhoneNettype(int type)
{
	PostMessage(theApp.m_pMainWnd->m_hWnd, WM_TEL_STATUS, TEL_NETTYPE, type);
}
//拨打电话
void TelephoneWarp::DialNumber(char* telcode, int dial_tyle)   //int dial_type 0 语音电话， 1 视频电话
{
//	if(dial_tyle == 0)
//		HandFree(true);
//	else
//		HandFree(false);
	Dial(telcode, dial_tyle);
}
//挂机
void TelephoneWarp::HungOff()
{
	Hangup();
	PostMessage(theApp.m_pMainWnd->m_hWnd, WM_TEL_HUNGOFF, 0, 0);
}
//摘机
void TelephoneWarp::HungOn(BOOL isRing)
{
	PostMessage(theApp.m_pMainWnd->m_hWnd, WM_TEL_HUNGON, 0, 0);
	if(isRing)
		Answer();   
}

//打开/关闭振铃
void TelephoneWarp::OpenTelRing(BOOL Flag )
{
}
//静音
void TelephoneWarp::Mute()
{
}
//录音
void TelephoneWarp::HungOnToRecord()
{
}

//ring tone    
#pragma   comment   (lib,"Ole32.lib") 
#pragma   comment   (lib,"Strmiids.lib") 

void TelephoneWarp::PlayRingPrc(void)
{
	CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;

	BOOL bRun = TRUE;
	while(bRun)
	{
		Sleep(10);
		long evCode;
		if(main->phone_->pMediaControl &&  main->phone_->pGraph && main->phone_->ipBasAudio  && main->phone_->pEvent && main->phone_->gRingFilename[0] != '\0' && main->phone_->nRingCount ) 	//循环播放
		{
			main->phone_->pEvent->WaitForCompletion(5, &evCode);
		//	Dprintf("evCode = %x\n", evCode);
			if(evCode != 0)	
			{
				main->phone_->nRingCount--;
				if(main->phone_->nRingCount)
				{
					main->phone_->pMediaPosition->put_CurrentPosition(0);
					main->phone_->pMediaControl->Run();
				}
				else
				{
// 					if(main->phone_->m_pMsgWnd)
// 						main->phone_->m_pMsgWnd->SendMessage(WM_STOPTRYRING);
//					main->phone_->StartRing(L"");
					main->phone_->StopRing();
				}
			//	main->phone_->StartRing(main->phone_->gRingFilename);
			}
		}		
	}
}

void TelephoneWarp::InitRing()
{
	CoInitialize(NULL); 
	hPlayRingThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)PlayRingPrc, NULL, 0, NULL);
}

void TelephoneWarp::ReleaseRing()
{
	CloseHandle(hPlayRingThread);
}

void TelephoneWarp::InitRingSrc()
{
	//
	//  以下程序每次播放MP3(或其他格式)文件时调用        
	//  创建一个Filter   Graph   manager      
	CoCreateInstance(CLSID_FilterGraph,   NULL,   CLSCTX_INPROC,   IID_IGraphBuilder,   (void   **)&pGraph); 
	pGraph->QueryInterface(IID_IMediaControl,   (void   **)&pMediaControl); 
	pGraph->QueryInterface(IID_IMediaPosition,(LPVOID*)&pMediaPosition);

	//得到音频相关接口 
    pGraph->QueryInterface(IID_IBasicAudio,   (void   **)&ipBasAudio); 
	pGraph->QueryInterface(IID_IMediaEvent, (void **)&pEvent);
}

void TelephoneWarp::RelaseRingSrc()
{
	//  释放对象 
	if(pMediaControl)
	{
		pMediaControl->Stop();
	}
	if(pEvent)
	{
		pEvent->Release();
		pEvent = NULL;
	}
	if(ipBasAudio)
	{
		ipBasAudio->Release();
		ipBasAudio = NULL;
	}
	if(pMediaPosition)
	{
		pMediaPosition->Release();
		pMediaPosition = NULL;
	}
	if(pMediaControl)
	{
		pMediaControl->Release(); 
		pMediaControl = NULL;
	}
	if(pGraph)
	{
		pGraph->Release();  
		pGraph = NULL;
	}
}

void TelephoneWarp::StartRing(TCHAR *filename, int ncount)
{
	/*
	if(wcslen(filename) != 0)
		sndPlaySound(filename, SND_ASYNC|SND_LOOP);
	else
		sndPlaySound(NULL, 0);
	return;
	*/

	if(wcslen(filename) == 0)
	{
		memset(gRingFilename, 0, sizeof(TCHAR)*64);
		StopRing();
		return;
	}

// 	HWND hWnd = ::FindWindow(L"csplayer_win1", L"csplayer window1"); //
// 	if(hWnd)
// 		AfxMessageBox(L"csplayer_win1");

	::Sleep(50);

	::EnterCriticalSection(&m_ringSetion_);
	nRingCount = ncount;
//	memset(gRingFilename, 0, 64*2);
	memcpy(gRingFilename, filename, wcslen(filename)*2);	
	gRingFilename[wcslen(filename)] = '\0';
	RelaseRingSrc();
	InitRingSrc();
	::LeaveCriticalSection(&m_ringSetion_);
	if(pGraph && pMediaControl)
	{
		//  连接播放指定文件所需的filter    
		pGraph->RenderFile(filename,   NULL); 
		//  开始播放指定的文件    
		pMediaControl->Run(); 
		if(ipBasAudio)
		{
			Dprintf("g_RingSound = %x\n", g_RingSound);

			waveOutSetVolume(NULL, g_RingSound);
			
		//	ipBasAudio->put_Volume(g_RingSound); 
		}
	}
	m_bStartRing = TRUE;
}

void TelephoneWarp::StopRing(bool releaseSrc)
{
	/*
	sndPlaySound(NULL, 0);
	return;
	*/

	::EnterCriticalSection(&m_ringSetion_);
	if(m_bStartRing)
	{
		gRingFilename[0] = '\0';
		if(releaseSrc)
		{
			RelaseRingSrc();
		}
		
		if(m_pMsgWnd)
		{
			m_pMsgWnd->SendMessage(WM_STOPTRYRING);
		}
		m_bStartRing = FALSE;
	}
	m_pMsgWnd = NULL;
	::LeaveCriticalSection(&m_ringSetion_);
}

void TelephoneWarp::PauseRing()
{
	if(pMediaControl)
		pMediaControl->Pause();
}

void TelephoneWarp::ResumeRing()
{
	if(pMediaControl)
		pMediaControl->Run();
}

//-8000  ~ 0
void TelephoneWarp::SetSoundRingVal(int val)
{
	if(val <= -8000)
		val = -8000;
	else if(val > 0)
		val = 0;
	g_RingSound = val;
	if(ipBasAudio)
	{
		ipBasAudio->put_Volume(g_RingSound);
	}
}

void TelephoneWarp::ReduceSoundRing(int offset)
{
	if(ipBasAudio)
	{
		g_RingSound -= offset;
		if(g_RingSound < MIN_SOUND)
			g_RingSound = MIN_SOUND;
		ipBasAudio->put_Volume(g_RingSound); 
	}
}

void TelephoneWarp::IncreaseSoundRing(int offset)
{
	if(ipBasAudio)
	{
		g_RingSound += offset;
		if(g_RingSound > MAX_SOUND)
			g_RingSound = MAX_SOUND;
		ipBasAudio->put_Volume(g_RingSound); 
	}
}

void TelephoneWarp::MinSoundRing()
{
	if(ipBasAudio)
	{
		g_RingSound = MIN_SOUND;
		ipBasAudio->put_Volume(g_RingSound); 
	}
}

void TelephoneWarp::MaxSoundRing()
{
	if(ipBasAudio)
	{
		g_RingSound = MAX_SOUND;
		ipBasAudio->put_Volume(g_RingSound); 
	}
}

void TelephoneWarp::ResumeSoundValueRing()
{
	g_RingSound = DEFUALT_SOUND_VALUME;
	if(ipBasAudio)
	{
		ipBasAudio->put_Volume(g_RingSound); 
	}
}
