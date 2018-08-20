#include "stdafx.h"
#include "TelephoneWarp.h"
#include "../multimediaphone.h"
#include "../multimediaphoneDlg.h"
#include "../resource.h"

#define HUNGON_VALUE	0x82		//0x82	 ժ��
#define HUNGOFF_VALUE	0x83		//0x83   �һ�
#define RING_VALUE		0x84		//0x84   �����ź�
//#define WM_TEL_CALLIDEND                8023

#define DEFUALT_SOUND_VALUME   -1500

using namespace Telephone;

typedef enum
{   
	stBeginRest,                // ��ʼ��Ϣ/��ʱ    
	stContinueRest,             // ������Ϣ/��ʱ    
	stSendPhoneRequest,         // ����绰   
	stSendPhoneResponse,        // ��ȡ����Ӧ�𵽻�����    
	//stSendPhoneWaitIdle,      // ���򲻳ɹ����ȴ�GSM����    
	stReadPhoneRequest,         // ���Ͷ�ȡ�����б������    
	stReadPhoneResponse,        // ��ȡ�����б�������    
	//stDeletePhoneRequest,     // ɾ����¼    
	//stDeletePhoneResponse,    // ɾ����¼Ӧ��    
	//stDeletePhoneWaitIdle,    // ɾ�����ɹ����ȴ�GSM����  
	atRedial,
	stAnswer,
	stHangupPhone,			//�һ�
	stExitThread                // �˳�    
}TelephoneState;               // ������̵�״̬ 

static unsigned char g_tel_code[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '*', '#', 'A', 'B', 'C', 'D'};
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
			Dprintf("%x\n", data[i]);

			CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
		
			switch(c)
			{
				case HUNGON_VALUE:
					PostMessage(theApp.m_pMainWnd->m_hWnd, WM_TEL_HUNGON, 0, 0);
					break;
				case HUNGOFF_VALUE:
 					PostMessage(theApp.m_pMainWnd->m_hWnd, WM_TEL_HUNGOFF, 0, 0);
					break;
				
				case PAGEUP_VALUE:
					PostMessage(theApp.m_pMainWnd->m_hWnd, WM_KEYDOWN, 'U', 0);
					break;	
				case PAGEDOWN_VALUE:
					PostMessage(theApp.m_pMainWnd->m_hWnd, WM_KEYDOWN, 'D', 0);
					break;

				
				case 0xB0: case 0xB1: case 0xB2: case 0xB3: case 0xB4: case 0xB5: case 0xB6: case 0xB7:
				case 0xB8: case 0xB9: case 0xBA: case 0xBB: case 0xBC:
 					PostMessage(theApp.m_pMainWnd->m_hWnd, WM_TEL_KEYCODE, g_tel_code[c-0xB0], 0);

					break;
					/*
				case 0xBD:
					PostMessage(theApp.m_pMainWnd->m_hWnd, WM_REDIAL, 0, 0);
					break;
				case 0xBF:
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
				case 0x9D:   //����ժ��
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


	m_pRS232 = new Util::RS232();
	m_pRS232->OpenPort();
	m_pRS232->SetReadFunc(ParseTelephoneData);

	//end ring tone
	::InitializeCriticalSection(&m_ringSetion_);
	InitRing();
}
TelephoneWarp::~TelephoneWarp()
{
	SetEvent(m_hKillThreadEvent);           // �����ر����̵߳��ź�    
	WaitForSingleObject(m_hThreadKilledEvent, INFINITE);    // �ȴ����̹߳ر�    

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
	// �������߳�    
	AfxBeginThread(TelephoneThread, this, THREAD_PRIORITY_NORMAL);   
}
// ��һ������Ϣ���뷢�Ͷ���    
void TelephoneWarp::PutSendMessage(TEL_NUM* pparam)   
{   
	EnterCriticalSection(&m_csSend);   

	memcpy(&m_PhoneSend[m_nSendIn], pparam, sizeof(TEL_NUM));   

	m_nSendIn++;   
	if (m_nSendIn >= MAX_PHONE_SEND)  m_nSendIn = 0;   

	LeaveCriticalSection(&m_csSend);   
}   

// �ӷ��Ͷ�����ȡһ������Ϣ    
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

// ������Ϣ������ն���    
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

// �ӽ��ն�����ȡһ������Ϣ    
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
	//int nMsg;               // �յ��绰����    
	//int nDelete;            // Ŀǰ����ɾ���ļ�¼���    
	char buff[1024];           // ���յ绰�б�Ļ�����   ������� 
	TEL_NUM param[256];    // ����/���յ绰������    
	char* state = NULL;

	CTime tmOrg, tmNow;     // �ϴκ����ڵ�ʱ�䣬���㳬ʱ��    
	//enum {   
	//	stBeginRest,                // ��ʼ��Ϣ/��ʱ    
	//	stContinueRest,             // ������Ϣ/��ʱ    
	//	stSendPhoneRequest,         // ����绰   
	//	stSendPhoneResponse,        // ��ȡ����Ӧ�𵽻�����    
	//	stSendPhoneWaitIdle,      // ���򲻳ɹ����ȴ�GSM����    
	//	stReadPhoneRequest,       // ���Ͷ�ȡ�����б������    
	//	stReadPhoneResponse,      // ��ȡ�����б�������    
	//	stDeletePhoneRequest,     // ɾ����¼    
	//	stDeletePhoneResponse,    // ɾ����¼Ӧ��    
	//	stDeletePhoneWaitIdle,    // ɾ�����ɹ����ȴ�GSM����    
	//	stExitThread                // �˳�    
	//} nState;               // ������̵�״̬    


	// ��ʼ״̬    
	//nState = stBeginRest;   

	// ���ͺͽ��մ����״̬ѭ��    
	while (nState != stExitThread)   
	{   
		switch(nState)   
		{   
		case stBeginRest:   
			TRACE(L"State=stBeginRest\n");    
			Sleep(2000);
			tmOrg = CTime::GetCurrentTime(); 
			nState = stContinueRest;   
			EnterCriticalSection(&csCom);
			break;   

		case stContinueRest:   
			TRACE(L"State=stContinueRest\n");    
			//Sleep(300);   
			tmNow = CTime::GetCurrentTime();   
			if (p->GetSendMessage(param))   
			{   
				nState = stSendPhoneRequest;  // �д�����绰���Ͳ���Ϣ��    
			}   
			else// if (tmNow - tmOrg >= 2)     // ������绰���пգ���Ϣ2��    
			{   
				nState = stReadPhoneRequest;  // ת��������״̬    
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
				LeaveCriticalSection(&csCom);
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
			LeaveCriticalSection(&csCom);
			break;
		} 
		// ����Ƿ��йرձ��̵߳��ź�    
		DWORD dwEvent = WaitForSingleObject(p->m_hKillThreadEvent, 20);   
		if (dwEvent == WAIT_OBJECT_0)  nState = stExitThread;   
	}   

	// �ø��߳̽�����־    
	SetEvent(p->m_hThreadKilledEvent); 
	return 0;
}

bool TelephoneWarp::Dial_(TEL_NUM num)
{
	return m_pAT->PhoneDial(num.NUM);
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
bool TelephoneWarp::Volume(unsigned int level)
{
	return m_pAT->PhoneVolume(level);
}
bool TelephoneWarp::AutoAnswer(unsigned int second)
{
	return m_pAT->PhoneAutoAnswer(second);
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
void TelephoneWarp::Ring(void)//��������
{
	PostMessage(theApp.m_pMainWnd->m_hWnd, WM_TEL_RING, 0, 0);
	PostMessage(theApp.m_pMainWnd->m_hWnd, WM_TEL_STATUS, TEL_FROMRING, 0);
}
void TelephoneWarp::Busy(void)//�Է�æ
{
	PostMessage(theApp.m_pMainWnd->m_hWnd, WM_TEL_STATUS, TEL_FROMBUY, 0);
}
void TelephoneWarp::Connected(void)//ͨ����ͨ״̬
{
	PostMessage(theApp.m_pMainWnd->m_hWnd, WM_TEL_STATUS, TEL_FROMCONNECTED, 0);
}
void TelephoneWarp::Connect(void)//�Է����� һ��
{
	PostMessage(theApp.m_pMainWnd->m_hWnd, WM_TEL_STATUS, TEL_FROMCONNECT, 0);
}
void TelephoneWarp::Held(void)//����״̬
{
	PostMessage(theApp.m_pMainWnd->m_hWnd, WM_TEL_STATUS, TEL_FROMHELD, 0);
}
void TelephoneWarp::Dialing(void)//���ڲ���״̬
{
	PostMessage(theApp.m_pMainWnd->m_hWnd, WM_TEL_STATUS, TEL_FROMDIALING, 0);
}
void TelephoneWarp::Alerting(void)//�Է�����״̬
{
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
void TelephoneWarp::NoAnswer(void)//��Ӧ��
{
	PostMessage(theApp.m_pMainWnd->m_hWnd, WM_TEL_STATUS, TEL_FROMNOANSWER, 0);
}
void TelephoneWarp::NoCarrier(void)//���Ӳ��ܱ�����
{
	nState = stHangupPhone;
	PostMessage(theApp.m_pMainWnd->m_hWnd, WM_TEL_STATUS, TEL_FROMNOCARRIER, 0);
}
void TelephoneWarp::Congestion(void)//����ӵ��
{
	PostMessage(theApp.m_pMainWnd->m_hWnd, WM_TEL_STATUS, TEL_FROMCONGESTION, 0);
}
void TelephoneWarp::OppHangup(void)//�Է��һ�
{
	nState = stHangupPhone;
	PostMessage(theApp.m_pMainWnd->m_hWnd, WM_TEL_STATUS, TEL_FROMOPPHUNGUP, 0);
}
void TelephoneWarp::Odb(void)//��������
{
	PostMessage(theApp.m_pMainWnd->m_hWnd, WM_TEL_STATUS, TEL_FROMODB, 0);
}

//����绰
void TelephoneWarp::DialNumber(char* telcode, int dial_tyle)   //int dial_type 0 ���ᣬ 1 ժ��
{
	Dial(telcode);
}
//�һ�
void TelephoneWarp::HungOff()
{
	Hangup();
	PostMessage(theApp.m_pMainWnd->m_hWnd, WM_TEL_HUNGOFF, 0, 0);
}
//ժ��
void TelephoneWarp::HungOn(BOOL isRing)
{
	PostMessage(theApp.m_pMainWnd->m_hWnd, WM_TEL_HUNGON, 0, 0);
	if(isRing)
		Answer();   
}

//��/�ر�����
void TelephoneWarp::OpenTelRing(BOOL Flag )
{
}
//����
void TelephoneWarp::Mute()
{
}
//¼��
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
		if(main->phone_->pMediaControl &&  main->phone_->pGraph && main->phone_->ipBasAudio  && main->phone_->pEvent && main->phone_->gRingFilename[0] != '\0' && main->phone_->nRingCount ) 	//ѭ������
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
	//  ���³���ÿ�β���MP3(��������ʽ)�ļ�ʱ����        
	//  ����һ��Filter   Graph   manager      
	CoCreateInstance(CLSID_FilterGraph,   NULL,   CLSCTX_INPROC,   IID_IGraphBuilder,   (void   **)&pGraph); 
	pGraph->QueryInterface(IID_IMediaControl,   (void   **)&pMediaControl); 
	pGraph->QueryInterface(IID_IMediaPosition,(LPVOID*)&pMediaPosition);

	//�õ���Ƶ��ؽӿ� 
    pGraph->QueryInterface(IID_IBasicAudio,   (void   **)&ipBasAudio); 
	pGraph->QueryInterface(IID_IMediaEvent, (void **)&pEvent);
}

void TelephoneWarp::RelaseRingSrc()
{
	//  �ͷŶ��� 
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
		//  ���Ӳ���ָ���ļ������filter    
		pGraph->RenderFile(filename,   NULL); 
		//  ��ʼ����ָ�����ļ�    
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
