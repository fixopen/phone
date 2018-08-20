#include "../multimediaphone.h"
#include "../multimediaphoneDlg.h"
#include "Telephone.h"
#include "../resource.h"

static unsigned char g_tel_code[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '*', '#', 'A', 'B', 'C', 'D'};
const char *const DTMFNUMBER = "D1234567890*#ABC";

Phone::Phone()
{
	//ring tone
	pGraph = NULL;   
	pMediaControl = NULL; 
	ipBasAudio       =   NULL;
	pEvent = NULL;
	g_RingSound = -8000; //-(MAX_SOUND-MIN_SOUND)/2; 
	gRingFilename[0] = '\0';
	//end ring tone

	writestatus_ = FALSE;		// 1 正常状态， 0 取消
	writecount_ = 0;		// 需要写总的个数，
	writecurrentcount_ = 0; // 已写的个数
	memset(dialcode_, 0, DIAL_CODE_MAX);
	HANDLE  hPhoneReadThread;
	HANDLE  hPhoneWriteThread;
	
//	printf("Test Phone IO Ctrl Functions\r\n");
	
	hPort = CreateFile (_T("SCL1:"), // Pointer to the name of the port
		GENERIC_READ | GENERIC_WRITE, // Access (read-write) mode
		0,            // Share mode
		NULL,         // Pointer to the security attribute
		OPEN_EXISTING,// How to open the serial port
		0,            // Port attributes
		NULL);
	
	if (hPort != INVALID_HANDLE_VALUE)
	{
//		printf("hPort is ok------------------!\r\n");
		hPhoneReadThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ReadPhonePrc, NULL, 0, NULL);
		if (hPhoneReadThread == NULL )
		{
//			printf("Phone: Create Read Thread Error!\r\n");
			CloseHandle(hPort);
		}
		hPhoneWriteThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)WritePhonePrc, NULL, 0, NULL);
		if (hPhoneWriteThread == NULL )
		{
//			printf("Phone: Create Read Thread Error!\r\n");
			CloseHandle(hPort);
		}
	}

	InitRing();
}

Phone::~Phone()
{
	CloseHandle(hPort);
}

const unsigned char gTelVersion[] = {0x50, 0x51, 0x50, 0x50, 0x50, 0x51, 0x50, 0x50, 0x52, 0x50, 0x50, 0x53, 0x50, 0x58, 0x50, 0x55, 0x50, 0x51};
BOOL Phone::DeteTelVersion(unsigned char c)
{
	CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	if(c >= 0x50 && c <= 0x5F)
	{
		if(strlen(main->phone_->m_chTelVersion) > 23)
			memset(main->phone_->m_chTelVersion, 0, 24);
		main->phone_->m_chTelVersion[strlen(main->phone_->m_chTelVersion)] = c;
	}
	else if(c == 0x8C)
	{
		static char gTelVersion[24];
		strcpy(gTelVersion, main->phone_->m_chTelVersion);
		if(memcmp(main->phone_->m_chTelVersion, gTelVersion, strlen(main->phone_->m_chTelVersion)) == 0)
		{
			PostMessage(theApp.m_pMainWnd->m_hWnd, WM_DETECTTEL_VERSION, 1, (LPARAM)gTelVersion);
		}
		else
		{
			PostMessage(theApp.m_pMainWnd->m_hWnd, WM_DETECTTEL_VERSION, 0, (LPARAM)gTelVersion);
		}
		memset(main->phone_->m_chTelVersion, 0, 24);
	}
	return TRUE;
}

char *gDetectString  = "1****1";
char *gDetectString1 = "1####1";
int Phone::DetectTestStatus(unsigned char c)
{
	char c1;
	if(c >= 0xB0 && c <= 0xBC)
		c1 = g_tel_code[c-0xB0];
	CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	if(c1 != '1' && c1 != '*' && c1 != '#')
	{
		memset(main->phone_->m_chDetectCode, 0, 24);
	}
	else
	{
		main->phone_->m_chDetectCode[strlen(main->phone_->m_chDetectCode)] = c1;
	}
	if(memcmp(main->phone_->m_chDetectCode, gDetectString, strlen(gDetectString)) == 0)
	{
		memset(main->phone_->m_chDetectCode, 0, 24);
		return 0;
	}
	else if(memcmp(main->phone_->m_chDetectCode, gDetectString1, strlen(gDetectString1)) == 0)
	{
		memset(main->phone_->m_chDetectCode, 0, 24);
		return 1;
	}
	else if(strlen(main->phone_->m_chDetectCode) >= strlen(gDetectString))
	{
		memset(main->phone_->m_chDetectCode, 0, 24);
	}
	return -1;
}

void Phone::ReadPhonePrc(void)
{
	CHAR phoneData[256];
	int i;
	static BOOL isRingTelCode = FALSE;
	CALLID_INFO	CallID;
	UINT8   CallIDbuff[128];
	int     CallIDLen = 0;
	
	while(1)
	{
		//CCheckDlg* main = dynamic_cast<CCheckDlg*>(theApp.m_pMainWnd);
		CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
		int ncount = main->phone_->ReadPhoneData(phoneData, 256);
		for (i=0; i<ncount; i++)
		{
			if(CallIDLen > 128)
				CallIDLen = 0;
			unsigned char c = phoneData[i];

			int rr = DetectTestStatus(c);
			if(rr == 0)
			{
				PostMessage(theApp.m_pMainWnd->m_hWnd, WM_INTOSETTESTDLG, 0, 0);
			}
			else if(rr == 1)
			{
				PostMessage(theApp.m_pMainWnd->m_hWnd, WM_INTODETECTDLG, 0, 0);
			}
			DeteTelVersion(c);
			switch(c)
			{
				case RING_VALUE:
					//isRingTelCode = TRUE;
					PostMessage(theApp.m_pMainWnd->m_hWnd, WM_TEL_RING, 0, 0);
					//theApp.m_pMainWnd->SendMessage(WM_TEL_RING, 0, 0);
					break;
				case HUNGON_VALUE:
					isRingTelCode = FALSE;
					PostMessage(theApp.m_pMainWnd->m_hWnd, WM_TEL_HUNGON, 0, 0);
					//theApp.m_pMainWnd->SendMessage(WM_TEL_HUNGON, 0, 0);
					break;
				case HUNGOFF_VALUE:
					PostMessage(theApp.m_pMainWnd->m_hWnd, WM_TEL_HUNGOFF, 0, 0);
					//theApp.m_pMainWnd->SendMessage(WM_TEL_HUNGOFF, 0, 0);
					break;
				case FSKEND_VALUE:
					isRingTelCode = FALSE;
					CallIDbuff[CallIDLen++] = c;
					main->phone_->DecodeCallIDPackage(CallIDbuff, &CallID);
					PostMessage(theApp.m_pMainWnd->m_hWnd, WM_TEL_CALLIDEND, (WPARAM)&CallID, 0);
					CallIDLen = 0;
					//theApp.m_pMainWnd->SendMessage(WM_TEL_CALLIDEND, 0, 0);
					break;
				case DTMFEND_VALUE:
					isRingTelCode = FALSE;
					CallIDbuff[CallIDLen++] = c;
					main->phone_->DecodeCallIDPackage(CallIDbuff, &CallID);
					PostMessage(theApp.m_pMainWnd->m_hWnd, WM_TEL_CALLIDEND, (WPARAM)&CallID, 0);
					CallIDLen = 0;
					//theApp.m_pMainWnd->SendMessage(WM_TEL_CALLIDEND, 0, 0);
					break;
				case PAGEUP_VALUE:
					keybd_event('U', 0, 0, 0);
					break;	
				case PAGEDOWN_VALUE:
					keybd_event('D', 0, 0, 0);
					break;

				case 0x30:
				case 0x31:
				case 0x32:
				case 0x33:
				case 0x34:
				case 0x35:
				case 0x36:
				case 0x37:
				case 0x38:
				case 0x39:
				case 0x3A:
				case 0x3B:
				case 0x3C:
				case 0x3D:
				case 0x3E:
				case 0x3F:
					isRingTelCode = TRUE;
					CallIDbuff[CallIDLen++] = c;
						//PostMessage(theApp.m_pMainWnd->m_hWnd, WM_TEL_TELCODE, c, 0);
					//theApp.m_pMainWnd->SendMessage(WM_TEL_TELCODE, g_tel_code[c-0x30], 0);
					break;
				case 0x40:
				case 0x41:
				case 0x42:
				case 0x43:
				case 0x44:
				case 0x45:
				case 0x46:
				case 0x47:
				case 0x48:
				case 0x49:
				case 0x4A:
				case 0x4B:
				case 0x4C:
				case 0x4D:
				case 0x4E:
				case 0x4F:
					isRingTelCode = TRUE;
					CallIDbuff[CallIDLen++] = c;
					//PostMessage(theApp.m_pMainWnd->m_hWnd, WM_TEL_TELCODE, c, 0);
					//theApp.m_pMainWnd->SendMessage(WM_TEL_TELCODE, g_tel_code[c-0x40], 0);
					break;
				case 0xB0:
				case 0xB1:
				case 0xB2:
				case 0xB3:
				case 0xB4:
				case 0xB5:
				case 0xB6:
				case 0xB7:
				case 0xB8:
				case 0xB9:
				case 0xBA:
				case 0xBB:
				case 0xBC:
// 				case 0xBD:
// 				case 0xBE:
// 				case 0xBF: 
					PostMessage(theApp.m_pMainWnd->m_hWnd, WM_TEL_KEYCODE, g_tel_code[c-0xB0], 0);
					//theApp.m_pMainWnd->SendMessage(WM_TEL_KEYCODE, g_tel_code[c-0xB0], 0);
					break;
				case 0xBD:
					PostMessage(theApp.m_pMainWnd->m_hWnd, WM_REDIAL, 0, 0);
					break;
				case 0xBE:
					PostMessage(theApp.m_pMainWnd->m_hWnd, WM_MUTE, 0, 0);
					break;
				case 0xBF:
					PostMessage(theApp.m_pMainWnd->m_hWnd, WM_RKEY, 0, 0);
					break;
				case 1:
				case 2:
				case 3:
				case 4:
				case 5:
				case 6:
				case 7:
				case 8:
				case 9:
				case 10:
				case 11:
				case 12:
				case 13:
					PostMessage(theApp.m_pMainWnd->m_hWnd, WM_SPEEDDIAL, c, 0);
					break;
				case 0x94:
					PostMessage(theApp.m_pMainWnd->m_hWnd, WM_HUNGONINRECORD, 0, 0);
					break;
				default:
					break;
			}
			
			/*
			unsigned char base = 0xB0;
			if(c == UP_VALUE)
			{
				theApp.m_pMainWnd->SendMessage(WM_USER + 100, UP, 0);
				keybd_event(VK_LEFT, 0, 0, 0);
			}
			else if(c == DOWN_VALUE)
			{
				theApp.m_pMainWnd->SendMessage(WM_USER + 100, DOWN, 0);
				keybd_event(VK_RIGHT, 0, 0, 0);
			}
			else if(c == BS_VALUE)
			{
				//theApp.m_pMainWnd->SendMessage(WM_USER + 100, BS, 0);
				keybd_event(VK_BACK, 0, 0, 0);
			}
			
#if (TEL_VERSION_LDJ == 0)
			else if (c == 0xBA)
			{
				//theApp.m_pMainWnd->SendMessage(WM_USER + 100, 0x30, 0);
				keybd_event(VK_NUMPAD0, 0, 0, 0);
			}
			else if (c >= 0xB1 && c <= 0xB9)
			{
				//theApp.m_pMainWnd->SendMessage(WM_USER + 100, 0x30 + (c - base), 0);
				keybd_event(VK_NUMPAD0 + (c - base), 0, 0, 0);
			}
#else
			else if (c >= 0x30 && c <= 0x39)
			{
				//theApp.m_pMainWnd->SendMessage(WM_USER + 100, 0x30 + (c - base), 0);
				keybd_event(VK_NUMPAD0 + (c - 0x30), 0, 0, 0);
			}
#endif
			*/
		}
	}
}

int Phone::ReadPhoneData(char *pData, int num)
{
	DWORD dwSize,i;
	
	ReadFile(hPort,pData, num, &dwSize, NULL);
	
//	Dprintf("\r\n Data Received:%d \r\n",dwSize);
	
	for (i=0;i<dwSize;i++)
		Dprintf(" %x ",pData[i]&0xFF);
		
	
	return dwSize;
}

void Phone::WritePhonePrc(void)
{
	CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;

	while(1)
	{
		if(main->phone_->writestatus_)
		{
			main->phone_->WritePhoneData();
		}
		else
		{
			Sleep(500);
		}
		Sleep(0);
	}
}

void Phone::OpenTelRing(BOOL Flag)
{
	char value;
	if(Flag)
		value = OPENRING_VALUE; 
	else
		value = CLOSERING_VALUE;
	int ncount = 0;
	DWORD dwSize = 0;
	while(dwSize == 0 && ncount++ < 5)
	{
		WriteFile(hPort, &value, 1, &dwSize,NULL);
		Sleep(50);
	}
}

void Phone::DialNumber(char* telcode, int dial_tyle)
{
	memset(dialcode_, 0, DIAL_CODE_MAX);
	if(dial_tyle == 0) //免提
	{
// 		dialcode_[0] = FREEHAND_VALUE;
// 		dialcode_[1] = HUNGON_VALUE;
// 		strcpy(dialcode_+2, telcode);
// 		writecount_ = strlen(telcode)+2;
		strcpy(dialcode_, telcode);
		writecount_ = strlen(telcode);
	}
	else if(dial_tyle == 1)	//手柄
	{
		strcpy(dialcode_, telcode);
		writecount_ = strlen(dialcode_);
	}
	
	writecurrentcount_ = 0;
	writestatus_ = TRUE;
}

void Phone::HungOff()
{
	char value = HUNGOFF_VALUE; 
	int ncount = 0;
	DWORD dwSize = 0;
	while(dwSize == 0 && ncount++ < 5)
	{
		WriteFile(hPort, &value, 1, &dwSize,NULL);
		Sleep(50);
	}
}

void Phone::HungOn()
{
	char value[2] = {FREEHAND_VALUE, HUNGON_VALUE}; 
	int ncount = 0;
	DWORD dwSize = 0;
	while(dwSize == 0 && ncount++ < 5)
	{
		WriteFile(hPort, &value, 1, &dwSize, NULL);
		Sleep(50);
		WriteFile(hPort, &value[1], 1, &dwSize, NULL);
		Sleep(50);
	}
}

void Phone::Mute()
{
	char value = MUTE_VALUE; 
	int ncount = 0;
	DWORD dwSize = 0;
	while(dwSize == 0 && ncount++ < 5)
	{
		WriteFile(hPort, &value, 1, &dwSize,NULL);
		Sleep(50);
	}
}

void Phone::HungOnToRecord()
{
	char value = HUNGONTORECORD_VALUE; 
	int ncount = 0;
	DWORD dwSize = 0;
	while(dwSize == 0 && ncount++ < 5)
	{
		WriteFile(hPort, &value, 1, &dwSize,NULL);
		Sleep(50);
	}
}

void Phone::WritePhoneData()
{	
	DWORD dwSize;
	while(writestatus_ && (writecurrentcount_<writecount_))
	{
		if(dialcode_[writecurrentcount_] != ',')
		{
			char c;
			Num2Dialnum(&c, &dialcode_[writecurrentcount_], 1);
			WriteFile(hPort, &c, 1, &dwSize,NULL);
			Sleep(300);
		}
		else
		{
			Sleep(1500);
		}
		writecurrentcount_++;
	}
	if(writestatus_ == 0)
		HungOff();
	else
		writestatus_ = 0;
}

void Phone::Num2Dialnum(CHAR *dst, CHAR *src, DWORD num)
{
	CHAR *pDst=NULL,*pSrc=NULL;
	DWORD i;
	
	pDst = dst;
	pSrc = src;
	
	for (i = 0 ;i < num ;i++)
	{
		switch (*pSrc)
		{
#if (TEL_VERSION_LDJ == 0)
		case '0':
			*pDst = 0x3A; 
			break;
		case '1':
			*pDst = 0x31;
			break;
		case '2':
			*pDst = 0x32;
			break;
		case '3':
			*pDst = 0x33;
			break;
		case '4':
			*pDst = 0x34;
			break;
		case '5':
			*pDst = 0x35;
			break;
		case '6':
			*pDst = 0x36;
			break;
		case '7':
			*pDst = 0x37;
			break;
		case '8':
			*pDst = 0x38;
			break;
		case '9':
			*pDst = 0x39;
			break;
		case '*':
			*pDst = 0x3B;
			break;
		case '#':
			*pDst = 0x3C;
			break;
		case 'A':
		case 'a':
			*pDst = 0x3D;
			break;
		case 'B':
		case 'b':
			*pDst = 0x3E;
			break;
		case 'C':
		case 'c':
			*pDst = 0x3F;
			break;
		case 'D':
		case 'd':
			*pDst = 0x30;
			break;
#else
		case '0':
			*pDst = 0x30; 
			break;
		case '1':
			*pDst = 0x31;
			break;
		case '2':
			*pDst = 0x32;
			break;
		case '3':
			*pDst = 0x33;
			break;
		case '4':
			*pDst = 0x34;
			break;
		case '5':
			*pDst = 0x35;
			break;
		case '6':
			*pDst = 0x36;
			break;
		case '7':
			*pDst = 0x37;
			break;
		case '8':
			*pDst = 0x38;
			break;
		case '9':
			*pDst = 0x39;
			break;
		case '*':
			*pDst = 0x3A;
			break;
		case '#':
			*pDst = 0x3B;
			break;
		case 'A':
		case 'a':
			*pDst = 0x3C;
			break;
		case 'B':
		case 'b':
			*pDst = 0x3D;
			break;
		case 'C':
		case 'c':
			*pDst = 0x3E;
			break;
		case 'D':
		case 'd':
			*pDst = 0x3F;
			break;
#endif
		default:
			*pDst = *pSrc;
			break;
		}
		pDst++;
		pSrc++;
	}
}

//ring tone    

#pragma   comment   (lib,"Ole32.lib") 
#pragma   comment   (lib,"Strmiids.lib") 

void Phone::PlayRingPrc(void)
{
	CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;

	BOOL bRun = TRUE;
	while(bRun)
	{
		Sleep(10);
		long evCode;
		if(main->phone_->pMediaControl &&  main->phone_->pGraph && main->phone_->ipBasAudio  && main->phone_->pEvent && main->phone_->gRingFilename[0] != '\0' && main->phone_->nRingCount == 0xFF) 	//循环播放
		{
			main->phone_->pEvent->WaitForCompletion(5, &evCode);
			//printf("evCode = %x\n", evCode);
			if(evCode != 0)	
				main->phone_->StartRing(main->phone_->gRingFilename);
		}		
	}
}

void Phone::InitRing()
{

	hPlayRingThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)PlayRingPrc, NULL, 0, NULL);
}

void Phone::ReleaseRing()
{
	CloseHandle(hPlayRingThread);
}

void Phone::InitRingSrc()
{
	CoInitialize(NULL); 
	//
	//  以下程序每次播放MP3(或其他格式)文件时调用        
	//  创建一个Filter   Graph   manager      
	CoCreateInstance(CLSID_FilterGraph,   NULL,   CLSCTX_INPROC,   IID_IGraphBuilder,   (void   **)&pGraph); 
	pGraph->QueryInterface(IID_IMediaControl,   (void   **)&pMediaControl); 
	//得到音频相关接口 
    pGraph->QueryInterface(IID_IBasicAudio,   (void   **)&ipBasAudio); 
	pGraph->QueryInterface(IID_IMediaEvent, (void **)&pEvent);
}

void Phone::RelaseRingSrc()
{
	if(pMediaControl && pGraph && ipBasAudio && pEvent)
	{
		//  释放对象 
		pEvent->Release();
		pEvent = NULL;
		ipBasAudio->Release();
		ipBasAudio = NULL;
		pGraph->Release();  
		pGraph = NULL;
		pMediaControl->Release(); 
		pMediaControl = NULL;
	}
	
}

void Phone::StartRing(TCHAR *filename, int ncount)
{
	nRingCount = ncount;
//	memset(gRingFilename, 0, 64*2);
	memcpy(gRingFilename, filename, wcslen(filename)*2);	
	gRingFilename[wcslen(filename)] = '\0';
	RelaseRingSrc();
	InitRingSrc();
	if(pGraph && pMediaControl)
	{
		//  连接播放指定文件所需的filter    
		pGraph->RenderFile(filename,   NULL); 
		//  开始播放指定的文件    
		pMediaControl->Run(); 
		if(ipBasAudio)
		{
			//printf("g_RingSound = %d\n", g_RingSound);
			ipBasAudio->put_Volume(g_RingSound); 
		}
	}
}

void Phone::StopRing()
{
	if(pMediaControl)
	{
		pMediaControl->Stop();
	}
	gRingFilename[0] = '\0';
}

void Phone::PauseRing()
{
	if(pMediaControl)
		pMediaControl->Pause();
}

void Phone::ResumeRing()
{
	if(pMediaControl)
		pMediaControl->Run();
}

//-8000  ~ 0
void Phone::SetSoundRingVal(int val)
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

void Phone::ReduceSoundRing(int offset)
{
	if(ipBasAudio)
	{
		g_RingSound -= offset;
		if(g_RingSound < MIN_SOUND)
			g_RingSound = MIN_SOUND;
		ipBasAudio->put_Volume(g_RingSound); 
	}
}

void Phone::IncreaseSoundRing(int offset)
{
	if(ipBasAudio)
	{
		g_RingSound += offset;
		if(g_RingSound > MAX_SOUND)
			g_RingSound = MAX_SOUND;
		ipBasAudio->put_Volume(g_RingSound); 
	}
}

void Phone::MinSoundRing()
{
	if(ipBasAudio)
	{
		g_RingSound = MIN_SOUND;
		ipBasAudio->put_Volume(g_RingSound); 
	}
}

void Phone::MaxSoundRing()
{
	if(ipBasAudio)
	{
		g_RingSound = MAX_SOUND;
		ipBasAudio->put_Volume(g_RingSound); 
	}
}

void Phone::ResumeSoundValueRing()
{
	g_RingSound = -1500;
	if(ipBasAudio)
	{
		ipBasAudio->put_Volume(g_RingSound); 
	}
}

bool Phone::DecodeCallIDPackage(BYTE *buf, CALLID_INFO *pcallid)
{
	bool	flag = false;

	if (*buf >= DTMF_CHANNEL_STRAT && *buf <= DTMF_CHANNEL_END)
	{
		flag = DecodeDTMFCallIDPackage(buf, pcallid);
	}

	else if (*buf >= FSK_CHANNEL_STRAT && *buf <= FSK_CHANNEL_END)
	{
		flag = DecodeFSKCallIDPackage(buf, pcallid);
	}

	if (flag)
	{
		// 过滤无效号码字符
		int		i, j, k;
		i = strlen((char*)(pcallid->number));

		for (j = 0, k = 0; k < i; k ++)
		{
			if ((pcallid->number[k] >= '0') && (pcallid->number[k] <= '9'))
			{
				pcallid->number[j] = pcallid->number[k];
				j ++;
			}
		}
		pcallid->number[j] = '\0';

		if (j == 0)
		{
	          	//pcallid->numberstatus = CALLID_NONE;
			flag = false;
		}
	}

	else
	{
		pcallid->number[0] = '\0';
                pcallid->numberstatus = CALLID_ERROR;   //fjm
	}
        //printf("Before return ,pcallid->numberstatus is %d\n", pcallid->numberstatus);
	return flag;
}

//////////////////////////////////////////
//
//	解 DTMF 来电包
//
bool Phone::DecodeDTMFCallIDPackage(BYTE *buf, CALLID_INFO *pcallid)
{
	int		i;

	for (i = 0; (*(buf + i) != TEL_DTMFEND) && (i < CALLID_NUM_LEN - 1); i ++)
	{
		pcallid->number[i] = DTMFNUMBER[*(buf + i) - DTMF_CHANNEL_STRAT];
	}
	pcallid->number[i] = '\0';
	pcallid->numberstatus = CALLID_OK;
	pcallid->type = CALLID_TYPE_DTMF;
	return true;
}


////////////////////////////////
//
//	解 FSK 来电显示包
//
bool Phone::DecodeFSKCallIDPackage(BYTE *buf, CALLID_INFO *pcallid)
{
	int		pklen, datalen, i;
	BYTE	*pparm;		// pparm: pointer_parameter
	BYTE	*pd;		// pd:    pointer_data
	/*
	*	Assemble package
	*/
	for (pklen = 0; *(buf + pklen) != TEL_FSKEND; pklen ++){
		if (pklen % 2 == 0)
			*(buf + pklen / 2) = (*(buf + pklen) << 4) & 0xF0;
		else
			*(buf + pklen / 2) |= (*(buf + pklen) & 0x0F);
	}

	/*
	*	Package frame error
	*/
	//if (pklen % 2 != 0)
	//	return false;

	/*
	*	Locate pakage start flag error
	*/
	pklen /= 2;
	for (i = 0; i < pklen; i ++)
	{
		if ((*buf == CALLID_TYPE_FSK_SIMPLE)
		   ||(*buf == CALLID_TYPE_FSK_COMPLEX))
			break;
		else
			buf ++;
	}
	if (i == pklen)
		return false;
	else
		pklen -= i;

	/*
	*	Package check sum error
	*/
	BYTE	sum = 0;
	for (i = 0; i < pklen - 1; i ++)
		sum += *(buf + i);
	//if ((BYTE)(sum + *(buf + pklen - 1)) != 0)
	//	return false;

	/*
	*	Package length error
	*/
	datalen = *(buf + 1);
	//if (datalen + 3 != pklen)
	//	return false;

	/*
	*	Decode simple format
	*/
	if (*buf == CALLID_TYPE_FSK_SIMPLE){
		pd = buf + 2;
		DecodeFSKSimpleParam(pd, datalen, pcallid);
		pcallid->type = CALLID_TYPE_FSK_SIMPLE;
	}

	/*
	*	Decode complex format
	*/
	else if (*buf == CALLID_TYPE_FSK_COMPLEX){
		pparm = buf + 2;
		while (pparm < buf + 2 + datalen){
			DecodeFSKComplexParam(pparm, pcallid);		// Decode every parameter
			pparm += *(pparm + 1) + 2;
		}
		pcallid->type = CALLID_TYPE_FSK_COMPLEX;
	}
        //printf("Before return to main ,pcallid->numberstatus is %d\n", pcallid->numberstatus);
	return true;
}

////////////////////////////////
//
//	解 FSK 来电简单格式参数包
//
void Phone::DecodeFSKSimpleParam(BYTE *pd, int datalen, CALLID_INFO *pcallid)
{
	/*
	*	Data length error
	*/
	//if (datalen < 8 + 1)
	//	return;

	/*
	*	Decode time
	*/
	pcallid->month = (*(pd + 0) - '0') * 10 + (*(pd + 1) - '0');
	pcallid->day = (*(pd + 2) - '0') * 10 + (*(pd + 3) - '0');
	pcallid->hour = (*(pd + 4) - '0') * 10 + (*(pd + 5) - '0');
	pcallid->minute = (*(pd + 6) - '0') * 10 + (*(pd + 7) - '0');
	pcallid->timestatus = CALLID_OK;

	/*
	*	Decode number
	*/
	BYTE	*pnum = pd + 8;
	int		numlen = datalen - 8, i;

	// byw, 2002/8/27, 去掉奇偶校验信息
	for (i = 0; i < numlen; i ++)
	{
		*(pnum + i) &= 0x7F;
	}

	if (*pnum == 'O')
		pcallid->numberstatus = CALLID_NOTGET;
	else if (*pnum == 'P')
		pcallid->numberstatus = CALLID_FORBID;
	else{
////////////////////////////////////////////////shiyu   2005-07-15
                for (i = 0; i < numlen; i ++)
                {
                        *(pnum + i) &= 0x0F;
                }
                for (i = 0; i < numlen; i ++)
                {
                        *(pnum + i) |= 0x30;
                }
///////////////////////////////////////////////////

		for (i = 0; (i < numlen) && (i < CALLID_NUM_LEN -1); i ++)
			pcallid->number[i] = *(pnum + i);
		pcallid->number[i] = '\0';
		pcallid->numberstatus = CALLID_OK;
	}
}

////////////////////////////////
//
//	解 FSK 来电复杂格式参数包
//
void Phone::DecodeFSKComplexParam(BYTE *pparm, CALLID_INFO *pcallid)
{
	// pparm: pointer_parameter, pd: pointer_data
	int		datalen, i;
	BYTE	*pd;

	datalen = *(pparm + 1);
	pd = pparm + 2;

	/*
	*	odd-even check error
	*	just cast off check bit temporarily
	*/
	for (i = 0; i < datalen; i ++)
		*(pd + i) &= 0x7F;

	switch (*pparm){

	case CALLID_PARAM_TIME:

		/*
		*	Data time length error
		*/
		//if (datalen != 8)
		//	return;

		/*
		*	Decode date time
		*/
		pcallid->month = (*(pd + 0) - '0') * 10 + (*(pd + 1) - '0');
		pcallid->day = (*(pd + 2) - '0') * 10 + (*(pd + 3) - '0');
		pcallid->hour = (*(pd + 4) - '0') * 10 + (*(pd + 5) - '0');
		pcallid->minute = (*(pd + 6) - '0') * 10 + (*(pd + 7) - '0');
		pcallid->timestatus = CALLID_OK;
		break;

	case CALLID_PARAM_NUMBER:
		/*
		*	Decode number
		*/
/////////////////////////////////////////////////////////shiyu       2005-07-15
                for (i = 0; i < datalen; i ++)
                {
                        *(pd + i) &= 0x0F;
                }
                for (i = 0; i < datalen; i ++)
                {
                        *(pd + i) |= 0x30;
                }
///////////////////////////////////////////////////////

		for (i = 0; (i < datalen) && (i < CALLID_NUM_LEN -1); i ++)
			pcallid->number[i] = *(pd + i);
		pcallid->number[i] = '\0';
		pcallid->numberstatus = CALLID_OK;
		break;

	case CALLID_PARAM_NONUMBER:
		/*
		*	Set 'no number'
		*/
		if (*pd == 'O')
			pcallid->numberstatus = CALLID_NOTGET;
		else if (*pd == 'P')
                {
			pcallid->numberstatus = CALLID_FORBID;
                        printf("Detect  P,来电保密 PPPPPPPPPPPPPPPPPPPPPPPPPPPPP\n");
                        printf("pcallid->numberstatus is %d\n", pcallid->numberstatus);
                }
		else
			pcallid->numberstatus = CALLID_ERROR; //fjm
		break;

	case CALLID_PARAM_NAME:
		/*
		*	Decode name
		*/
		for (i = 0; (i < datalen) && (i < CALLID_NAME_LEN -1); i ++)
			pcallid->name[i] = *(pd + i);
		pcallid->name[i] = '\0';
		pcallid->namestatus = CALLID_OK;
		break;

	case CALLID_PARAM_NONAME:
		/*
		*	Set 'no name'
		*/
		if (*pd == 'O')
			pcallid->namestatus = CALLID_NOTGET;
		else if (*pd == 'P')
			pcallid->namestatus = CALLID_FORBID;
		else
			pcallid->namestatus = CALLID_FORBID;

		break;
	}
}
