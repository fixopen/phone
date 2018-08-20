#include "StdAfx.h"
#include "../multimediaphone.h"
#include "../multimediaphoneDlg.h"
#include "Telephone.h"
#include "../resource.h"
#include "afxwin.h"
#include "afxwin.h"
#define DEFUALT_SOUND_VALUME   -1500

typedef struct{
	DWORD  bkCmd;			//控制命令
	DWORD  bkLevel;			//背光控制级别			
}BACKLIGHTCMD;


//method
#define METHOD_BUFFERED 0
#define METHOD_IN_DIRECT 1
#define METHOD_OUT_DIRECT 2
#define METHOD_NEITHER 3

//access
#define FILE_ANY_ACCESS 0
#define FILE_READ_ACCESS (0x0001)
#define FILE_WRITE_ACCESS (0x0002)

#define SCL_IOCODE_BASE				2050

#define SCL_SYSTEM_RESET		CTL_CODE(FILE_DEVICE_STREAMS,SCL_IOCODE_BASE+0x1,METHOD_IN_DIRECT,FILE_ANY_ACCESS) //reset system
#define SCL_ENABLE_WATCHDOG		CTL_CODE(FILE_DEVICE_STREAMS,SCL_IOCODE_BASE+0x2,METHOD_IN_DIRECT,FILE_ANY_ACCESS) //enable watch dog
#define	SCL_ADJUST_BACKLIGHT		CTL_CODE(FILE_DEVICE_STREAMS,SCL_IOCODE_BASE+0x5,METHOD_IN_DIRECT,FILE_ANY_ACCESS)	   //LCD 灯控制	
#define SCL_GET_PRODUCT_INFO		CTL_CODE(FILE_DEVICE_STREAMS,SCL_IOCODE_BASE+0x7,METHOD_IN_DIRECT,FILE_ANY_ACCESS)	   //产品信息
#define SCL_ADSL_POWERCTL		CTL_CODE(FILE_DEVICE_STREAMS,SCL_IOCODE_BASE+0xA,METHOD_IN_DIRECT,FILE_ANY_ACCESS) //ADSL 电源控制
#define SCL_ENCRYGPT_VERIFY		CTL_CODE(FILE_DEVICE_STREAMS,SCL_IOCODE_BASE+0xE,METHOD_IN_DIRECT,FILE_ANY_ACCESS)	
#define CTL_CODE(DeviceType, Function, Method, Access) (((DeviceType) << 16) | ((Access) << 14) | ((Function) << 2) | (Method))
#define FILE_DEVICE_STREAMS 0x0000001e

//method
#define METHOD_BUFFERED 0
#define METHOD_IN_DIRECT 1
#define METHOD_OUT_DIRECT 2
#define METHOD_NEITHER 3

//access
#define FILE_ANY_ACCESS 0
#define FILE_READ_ACCESS (0x0001)
#define FILE_WRITE_ACCESS (0x0002)

#define SCL_IOCODE_BASE				2050

#define SCL_SYSTEM_RESET		CTL_CODE(FILE_DEVICE_STREAMS,SCL_IOCODE_BASE+0x1,METHOD_IN_DIRECT,FILE_ANY_ACCESS) //reset system
#define SCL_ENABLE_WATCHDOG		CTL_CODE(FILE_DEVICE_STREAMS,SCL_IOCODE_BASE+0x2,METHOD_IN_DIRECT,FILE_ANY_ACCESS) //enable watch dog
#define SCL_SYSTEM_RESET_RIGHTNOW   CTL_CODE(FILE_DEVICE_STREAMS,SCL_IOCODE_BASE+0x22,METHOD_IN_DIRECT,FILE_ANY_ACCESS)
#define	SCL_ADJUST_BACKLIGHT		CTL_CODE(FILE_DEVICE_STREAMS,SCL_IOCODE_BASE+0x5,METHOD_IN_DIRECT,FILE_ANY_ACCESS)	   //LCD 灯控制	
#define SCL_GET_PRODUCT_INFO		CTL_CODE(FILE_DEVICE_STREAMS,SCL_IOCODE_BASE+0x7,METHOD_IN_DIRECT,FILE_ANY_ACCESS)	   //产品信息
#define SCL_ADSL_POWERCTL		CTL_CODE(FILE_DEVICE_STREAMS,SCL_IOCODE_BASE+0xA,METHOD_IN_DIRECT,FILE_ANY_ACCESS) //ADSL 电源控制
#define SCL_ENCRYGPT_VERIFY		CTL_CODE(FILE_DEVICE_STREAMS,SCL_IOCODE_BASE+0xE,METHOD_IN_DIRECT,FILE_ANY_ACCESS)	

#define CTL_CODE(DeviceType, Function, Method, Access) (((DeviceType) << 16) | ((Access) << 14) | ((Function) << 2) | (Method))
#define FILE_DEVICE_STREAMS 0x0000001e

#define RS232_INVERT		0
#define BKCMD_MIS_CALL      0x3
#define BKCMD_MSG_LIGHT     0x4
static unsigned char g_tel_code[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '*', '#', 'A', 'B', 'C', 'D'};
const char *const DTMFNUMBER = "D1234567890*#ABC";
const unsigned char gTelVersion[] = {0x50, 0x51, 0x50, 0x50, 0x50, 0x51, 0x50, 0x50, 0x52, 0x50, 0x50, 0x53, 0x50, 0x58, 0x50, 0x55, 0x50, 0x51};
extern int counter;
extern BOOL g_bBattery;

extern BOOL Writelog(std::string filePath,unsigned char * ERRORContent, int len,std::string ERRORTYPE);

BOOL Writelog(std::string filePath,std::string conttent,std::string ERRORTYPE);

BOOL DeteTelVersion(unsigned char c)
{
	return TRUE;
}
char *gDetectString  = "1*0#*0#*0#*0#1";
char *gDetectString1 = "*#1579#";
char *gSetUserIDString = "*#357#"; //"3*0#*0#*0#*0#3";
static char gTelVersion1_[128];
int Phone::DetectTestStatus(unsigned char c)
{
	char c1;

	if(c >= 0xB1 && c <= 0xBC)
	{
		c1 = g_tel_code[c-0xB0];
	}

	CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;

	if((c1 != '1' &&  c1 != '5' && c1 != '*' && c1 != '#' && c1 != '9' && c1 != '3'  && c1 != '7') && !main->phone_->m_bInputtingUserID)
	{
		memset(main->phone_->m_chDetectCode, 0, 24);

		main->phone_->m_bInputtingUserID = FALSE;
	}
	else
	{
		if(main->phone_->m_bInputtingUserID && c1 == '*')
		{
			main->phone_->m_chDetectCode[strlen(main->phone_->m_chDetectCode)] = '.';

		}else{

			main->phone_->m_chDetectCode[strlen(main->phone_->m_chDetectCode)] = c1;
		}
	}
	
	//判断是否已经输入完成
	if(main->phone_->m_bInputtingUserID)
	{
		if(main->phone_->m_chDetectCode[strlen(main->phone_->m_chDetectCode)-1] == '#' && main->phone_->m_chDetectCode[strlen(main->phone_->m_chDetectCode)-2] == '.')
		{
			int ret ;
		
			if(strlen(main->phone_->m_chDetectCode) >= 4)
			{
				strcpy(gTelVersion1_, (const char *)main->phone_->m_chDetectCode);
			
				gTelVersion1_[strlen(gTelVersion1_) - 1] = '\0';
			
				ret = 3;
			}
			else{

				ret = 4;
			}
		
			memset(main->phone_->m_chDetectCode, 0, 24);
			
			return ret;
		}
	}
	
	if(memcmp(main->phone_->m_chDetectCode, gDetectString, strlen(gDetectString)) == 0)
	{
		memset(main->phone_->m_chDetectCode, 0, 24);

		main->phone_->m_bInputtingUserID = FALSE;

		return 5;
	}
	else if(memcmp(main->phone_->m_chDetectCode, gDetectString1, strlen(gDetectString1)) == 0)
	{
		memset(main->phone_->m_chDetectCode, 0, 24);

		main->phone_->m_bInputtingUserID = FALSE;

		return 1;
	}
	else if(memcmp(main->phone_->m_chDetectCode, gSetUserIDString, strlen(gSetUserIDString)) == 0)
	{
		//main->phone_->m_bInputtingUserID = TRUE;

		memset(main->phone_->m_chDetectCode, 0, 24);

		return 0;

	}else if(strlen(main->phone_->m_chDetectCode) >= strlen(gDetectString))
	{
		main->phone_->m_bInputtingUserID = FALSE;

		memset(main->phone_->m_chDetectCode, 0, 24);
	}

	return -1;
}


void ParseTelephoneData(unsigned char const* const data, unsigned int const length)
{
	int i;
	static BOOL isRingTelCode = FALSE;
	static CALLID_INFO	CallID[10];
	static int			curCallID = 0;
	static UINT8   CallIDbuff[128];
	static int     CallIDLen = 0;
	static unsigned char hungOn[2] = {0};
	extern BOOL _test_call;

	for (i=0; i<length; i++)
		{
			if(CallIDLen >= 128)
				CallIDLen = 0;

			unsigned char c = data[i];
			if(c == 0xC9 || c == 0xC8)  //wzx 20101227
			{
				hungOn[0] = c;
			}
			else if(c == 0x82)
			{
				if(hungOn[0] == 0xC9 || hungOn[0] == 0xC8)
					hungOn[1] = c;
			}
			else
			{
				memset(hungOn, 0, 2);
			}
			TRACE(L"%x\n", c);
			CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
			int rr = main->phone_->DetectTestStatus(c);
			if(rr == 1)
			{
		      	PostMessage(theApp.m_pMainWnd->m_hWnd, WM_CLEARPWD, 0, 0);
			}
			else if(rr == 0)
			{
	     	  //	PostMessage(theApp.m_pMainWnd->m_hWnd, WM_FORMATDATA, 0, 0);
			}

#if (RS232_INVERT==1)
			unsigned char d = 0;
			d |= (((c >> 0) & 0x01) << 7);
			d |= (((c >> 1) & 0x01) << 6);
			d |= (((c >> 2) & 0x01) << 5);
			d |= (((c >> 3) & 0x01) << 4);
			d |= (((c >> 4) & 0x01) << 3);
			d |= (((c >> 5) & 0x01) << 2);
			d |= (((c >> 6) & 0x01) << 1);
			d |= (((c >> 7) & 0x01) << 0);
			c = d;
#endif
			switch(c)
			{ 
				
        #if outputstring_data
 
			case EREADOUTPUTSTRING_DATA:
				break;

        #endif
              
				case RING_VALUE:
					Writelog("\\FlashDrv\\Log.txt",\
						Util::StringOp::FromInt(RING_VALUE),"recive ring signal:");				
					PostMessage(theApp.m_pMainWnd->m_hWnd, WM_TEL_RING, 0, 0);
					break;
				case HUNGON_VALUE:
					if(hungOn[0] == 0xC9 && hungOn[1] == 0x82 && g_bBattery)
					{
						PostMessage(theApp.m_pMainWnd->m_hWnd, WM_BATTERY_HUNGON, 0, 0);
					}
					else
					{
						isRingTelCode = FALSE;
						counter=0;
						theApp.IsHangON=true;
						
						if (!theApp.IsCheckTakingState)
						{ 
							((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pTelephoneDlg->setCheckTakingstate(true);
							::KillTimer(((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pTelephoneDlg->m_hWnd,CHECKLINESTATE);
						}
						
						PostMessage(theApp.m_pMainWnd->m_hWnd, WM_TEL_HUNGON, 0, 0);
					}
					break;
				case HUNGOFF_VALUE:
					{
						theApp.IsHangON=false;
						theApp.IsCheckTakingState=false;
						PostMessage(theApp.m_pMainWnd->m_hWnd, WM_TEL_HUNGOFF, 0, 0);
						CallIDLen = 0;
					}
					break;
				case FORCEHANGON:
					PostMessage(theApp.m_pMainWnd->m_hWnd,WM_CHECKFOCRHANGON,0,0);
					break;
				case FORCEHANGOFF:
					PostMessage(theApp.m_pMainWnd->m_hWnd,WM_CHECKFOCRHANGOFF,0,0);
					break;
				case FSKEND_VALUE:
					{
						isRingTelCode = FALSE;
					
						CallIDbuff[CallIDLen++] = c;
                        
						Writelog("\\FlashDrv\\Log.txt",CallIDbuff,CallIDLen,"recive callid fsk signal:");
                        
						memset(&CallID[curCallID], 0, sizeof(CALLID_INFO));
						
						DecodeCallIDPackage(CallIDbuff, &CallID[curCallID]);
					
						std::string tel=CallID[curCallID].number;
					
						Writelog("\\FlashDrv\\Log.txt",tel,"recive fsk number:");
					
						PostMessage(theApp.m_pMainWnd->m_hWnd,WM_TEL_CALLIDEND, (WPARAM)&CallID[curCallID], 0);
						curCallID = (curCallID + 1) % (sizeof(CallID) / sizeof(CallID[0]));

						memset(CallIDbuff,'\0',128);
					    
						CallIDLen = 0;
					}
					break;

				case DTMFEND_VALUE:
					{
						isRingTelCode = FALSE;
						CallIDbuff[CallIDLen++] = c;
						Writelog("\\FlashDrv\\Log.txt",CallIDbuff,CallIDLen,"recive callid dtm signal:");
					
						memset(&CallID[curCallID], 0, sizeof(CALLID_INFO));
						DecodeCallIDPackage(CallIDbuff, &CallID[curCallID]);
					
						std::string tel=CallID[curCallID].number;				
					    Writelog("\\FlashDrv\\Log.txt",tel,"recive dtm number:");
						
						if(strlen(CallID[curCallID].number) > 0)
						{
							PostMessage(theApp.m_pMainWnd->m_hWnd,\
								WM_TEL_CALLIDEND, (WPARAM)&CallID[curCallID], 0);
							curCallID = (curCallID + 1) % (sizeof(CallID) / sizeof(CallID[0]));
						}

						memset(CallIDbuff,'\0',128);				   
						CallIDLen = 0;
					}
					break;
				case PAGEUP_VALUE:
					PostMessage(theApp.m_pMainWnd->m_hWnd, WM_KEYDOWN, 'U', 0);
					break;	
				case PAGEDOWN_VALUE:
					PostMessage(theApp.m_pMainWnd->m_hWnd, WM_KEYDOWN, 'D', 0);
					break;
			    case 0x30: case 0x31: case 0x32: case 0x33: case 0x34: case 0x35: case 0x36: case 0x37:
				case 0x38: case 0x39: case 0x3A: case 0x3B: case 0x3C: case 0x3D: case 0x3E: case 0x3F:
					isRingTelCode = TRUE;
					CallIDbuff[CallIDLen++] = c;
					break;
				case 0x40: case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: case 0x46: case 0x47:
				case 0x48: case 0x49: case 0x4A: case 0x4B: case 0x4C: case 0x4D: case 0x4E: case 0x4F:
					isRingTelCode = TRUE;
					CallIDbuff[CallIDLen++] = c;
					break;
				case 0xB1: case 0xB2: case 0xB3: case 0xB4: case 0xB5: case 0xB6: case 0xB7:
				case 0xB8: case 0xB9: case 0xBA: case 0xBB: case 0xBC:
 					PostMessage(theApp.m_pMainWnd->m_hWnd, WM_TEL_KEYCODE, g_tel_code[c-0xB0], 0);
					//暂时disable 屏保密码、计算器输入框的硬键按键输入支持
					//所有相关defect 均有defect 142 跟踪
					//defect 142
//					keybd_event(g_tel_code[c-0xB0],   0,   0,   0);  
//					keybd_event(g_tel_code[c-0xB0],   0,   KEYEVENTF_KEYUP,   0);
					//==========
					//Sleep(50);
					//theApp.m_pMainWnd->SendMessage(WM_TEL_KEYCODE, g_tel_code[c-0xB0], 0);
					break;
				case 0xBD:
					PostMessage(theApp.m_pMainWnd->m_hWnd, WM_REDIAL, 0, 0);
					break;
				case 0xBF:
					PostMessage(theApp.m_pMainWnd->m_hWnd, WM_RKEY, 0, 0);
					break;
                case 1: case 2: case 3: case 4: case 5: case 6:	/*case 1:case 6:*/
				case 7:/* case 8: case 9: case 10: case 11: case 12:*/
				PostMessage(theApp.m_pMainWnd->m_hWnd, WM_SPEEDDIAL, c, 0);
				break;
                case 0XA9:  //确认键
 					PostMessage(theApp.m_pMainWnd->m_hWnd,WM_OK,0,0);
					break;
				case 0XA0: //退出键
					PostMessage(theApp.m_pMainWnd->m_hWnd,WM_CANCEL,0,0);
					break;
				case 0XA4:
					PostMessage(theApp.m_pMainWnd->m_hWnd,WM_DELETE,0,0);
					break;
				case 0XAB:
					PostMessage(theApp.m_pMainWnd->m_hWnd,WM_ITYPE,0,0);
					break;
				case 0XA1:
					PostMessage(theApp.m_pMainWnd->m_hWnd,WM_SEARCH,0,0);
			 		break;
				case 0XA2:
					PostMessage(theApp.m_pMainWnd->m_hWnd,WM_LOCALRECORD,0,0);
					break;
				case 0XA3:
					PostMessage(theApp.m_pMainWnd->m_hWnd, WM_PLAYLEAVEWORD, 0, 0);
					break;
				case 0XAA:
					PostMessage(theApp.m_pMainWnd->m_hWnd,WM_NEWCONTACT,0,0);
					break;
				case 0XA8:
					PostMessage(theApp.m_pMainWnd->m_hWnd,WM_BTLEFT,0,0);
					break;
				case 0XA7:
					PostMessage(theApp.m_pMainWnd->m_hWnd,WM_BTRIGHT,0,0);
					break;
				case 0x94:
					PostMessage(theApp.m_pMainWnd->m_hWnd, WM_HUNGONINRECORD, 0, 0);
					break;
				case 0x9D:   //并机摘机
					PostMessage(theApp.m_pMainWnd->m_hWnd, WM_BINGJIZHAIJI, 0, 0);
					break;
				case 0xDA:
					PostMessage(theApp.m_pMainWnd->m_hWnd, WM_BATTERYTODC, 0, 0);
					break;
				case 0xDB:
					PostMessage(theApp.m_pMainWnd->m_hWnd, WM_DCTOBATTERY, 0, 0);
					break;
				default:
					break;
			}
		}
}

Phone::Phone()
{
	//ring tone
	pGraph = NULL; 
	
	pMediaControl = NULL; 

	pMediaPosition = NULL;

	ipBasAudio       =   NULL;

	pEvent = NULL;

	g_RingSound = -1500; //-(MAX_SOUND-MIN_SOUND)/2;
	m_ringSound = -1500;

	gRingFilename[0] = '\0';

	m_chTelVersion[0] = '\0';

	m_chDetectCode[0] = '\0';

	memset(m_chDetectCode,'\0',24);

	m_bInputtingUserID = FALSE;

	m_bStartRing = FALSE;

	flag=true;
	
	m_pMsgWnd = NULL;

	//end ring tone
	isBackLight_ = FALSE;

	isleveaMsgLight=FALSE;

	iscallLight_=FALSE;

	m_pRS232 = new Util::RS232();

	m_pRS232->OpenPort();

	m_pRS232->SetReadFunc(ParseTelephoneData);
	
	::InitializeCriticalSection(&m_ringSetion_);

	m_hPort = CreateFile (_T("SCL1:"), // Pointer to the name of the port
		GENERIC_READ | GENERIC_WRITE, // Access (read-write) mode
		0,          
		NULL,         
		CREATE_ALWAYS,
		0,            
		NULL);
	
	InitRing();
}

Phone::~Phone()
{
	m_pRS232->ClosePort();
	delete m_pRS232;
	m_pRS232 = NULL;
}

void Phone::WritePort(const unsigned char * const data, int dataLength)
{
	for (int i=0; i < dataLength; i++)
	{
		unsigned char c = data[i];

#if (RS232_INVERT == 1)
		unsigned char d = 0;
		d |= (((c >> 0) & 0x01) << 7);
		d |= (((c >> 1) & 0x01) << 6);
		d |= (((c >> 2) & 0x01) << 5);
		d |= (((c >> 3) & 0x01) << 4);
		d |= (((c >> 4) & 0x01) << 3);
		d |= (((c >> 5) & 0x01) << 2);
		d |= (((c >> 6) & 0x01) << 1);
		d |= (((c >> 7) & 0x01) << 0);
		c = d;
#endif
		m_pRS232->WritePort((const unsigned char * const)&c, 1);

		Sleep(10);
	}
}

void Phone::OpenTelRing(BOOL Flag)
{
	char value;

	if(Flag)
		value = OPENRING_VALUE; 
	else
		value = CLOSERING_VALUE;
		
	WritePort((const unsigned char * const)&value, 1);
}


void DialThread()
{
	CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
    
	for(int i = 0; i < main->phone_->m_nDialNumber; i++)
	{
		if(!main->phone_->m_bDial)
		{
			break;
		}

		if(main->phone_->m_chDialNumber[i] < 0x30 || main->phone_->m_chDialNumber[i] > 0x40)
		{
			Sleep(2500);
		}
		else
		{   

			main->phone_->WritePort((const unsigned char * const)&main->phone_->m_chDialNumber[i], 1);

			Sleep(80);
		}
	}
}

void Phone::DialNumber(char* telcode, int dial_tyle)
{  
	CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;

	m_bDial = TRUE;

	m_nDialNumber = strlen(telcode) < 128 ? strlen(telcode) : 128;
	
	Num2Dialnum(m_chDialNumber, telcode, m_nDialNumber);

	HANDLE hDialThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)DialThread, NULL, 0, NULL);

	SetThreadPriority(hDialThread, THREAD_PRIORITY_LOWEST);
}

void Phone::ForceHungOff()
{
	char value = FORCEHUNGOFF_VALUE;
	
	WritePort((const unsigned char * const)&value, 1);
}

void Phone::CheckForceHungOff()
{
  char value=FORCEHANGON;

  WritePort((const unsigned char * const)&value, 1);
}

#if outputstring_data

void Phone::getOutPutStringData()
{
	char value=SREADOUTPUTSTRING_DATA;

	WritePort((const unsigned char * const)&value,1);
 
}

#endif
//
void Phone::HungOff()
{
	ForceHungOff();
	m_bDial = FALSE;
	char value = HUNGOFF_VALUE; 

	WritePort((const unsigned char * const)&value, 1);
}

void Phone::HungOn()
{
	char value[2] = {FREEHAND_VALUE, HUNGON_VALUE}; 

	WritePort((const unsigned char * const)&value, 2);

	::SetTimer(((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pTelephoneDlg->m_hWnd,CHECKLINESTATE,200,NULL);
}

void Phone::Mute()
{
	char value = MUTE_VALUE; 
	WritePort((const unsigned char * const)&value, 1);
}

void Phone::HungOnToRecord()
{
	 char value;
		
	if(((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pSettingDlg->m_IsSpeakerOut.GetCheck() == BST_CHECKED)
	{
		 value=HUNGONTORECORD_VALUE;
	}else
	{
		 value=HUNCONTORECORD_NO_VALUE; 
	}

	WritePort((const unsigned char * const)&value, 1);
}

void Phone::OpenBacklight(BOOL flag)
{
	if(isBackLight_ != flag)
	{
		unsigned long size;
		BACKLIGHTCMD blightcmd;
		blightcmd.bkCmd = 0x01;
		if(flag)
			blightcmd.bkLevel = 0;
		else
			blightcmd.bkLevel = 7;
		UINT value;
		if(DeviceIoControl(m_hPort, SCL_ADJUST_BACKLIGHT, &blightcmd, sizeof(BACKLIGHTCMD), &value, sizeof(UINT), &size, NULL))
		{
			isBackLight_ = flag;
			return;
		}
	}
}
void Phone::OpenCallLight(BOOL flag) 
{
	if(iscallLight_ != flag)
	{
		unsigned long size;
		BACKLIGHTCMD blightcmd;
		blightcmd.bkCmd = 0x03;
		if(flag)
			blightcmd.bkLevel = 1;
		else
			blightcmd.bkLevel = 0;
		
		UINT value;
		
		if(DeviceIoControl(m_hPort, SCL_ADJUST_BACKLIGHT, &blightcmd, sizeof(BACKLIGHTCMD), &value, sizeof(UINT), &size, NULL))
		{
			iscallLight_ = flag;
			return;
		}
	}
}
void Phone::OpenLeveaLight(BOOL flag)
{
	if(isleveaMsgLight != flag)
	{
		unsigned long size;

		BACKLIGHTCMD blightcmd;

		blightcmd.bkCmd = 0x04;

		if(flag)

			blightcmd.bkLevel = 1;

		else

			blightcmd.bkLevel = 0;
		
		UINT value;
		
		if(DeviceIoControl(m_hPort, SCL_ADJUST_BACKLIGHT, &blightcmd, sizeof(BACKLIGHTCMD), &value, sizeof(UINT), &size, NULL))
		{
			isleveaMsgLight = flag;

			return;
		}
	}
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
		if(main->phone_->pMediaControl &&  main->phone_->pGraph && main->phone_->ipBasAudio  && main->phone_->pEvent && main->phone_->gRingFilename[0] != '\0' && main->phone_->nRingCount ) 	//循环播放
		{
			main->phone_->pEvent->WaitForCompletion(5, &evCode);
			
			if(evCode != 0)	
			{
				main->phone_->nRingCount--;
				if(main->phone_->nRingCount)
				{
					main->phone_->pMediaPosition->put_CurrentPosition(0);
					main->phone_->pMediaControl->Run();

				}else{

					main->phone_->StopRing();
				}

			}
		}		
	}
}

void Phone::InitRing()
{
	CoInitialize(NULL); 

	hPlayRingThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)PlayRingPrc, NULL, 0, NULL);
}

void Phone::ReleaseRing()
{
	CloseHandle(hPlayRingThread);
}

void Phone::InitRingSrc()
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

void Phone::RelaseRingSrc()
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

void Phone::StartRing(TCHAR *filename, int ncount)
{

	if(wcslen(filename) == 0)
	{
		memset(gRingFilename, 0, sizeof(TCHAR)*64);
		StopRing();
		return;
	}
	::Sleep(50);
	::EnterCriticalSection(&m_ringSetion_);
	nRingCount = ncount;
	memcpy(gRingFilename, filename, wcslen(filename)*2);	
	gRingFilename[wcslen(filename)] = '\0';
	RelaseRingSrc();
	InitRingSrc();
	::LeaveCriticalSection(&m_ringSetion_);
	if(pGraph && pMediaControl)
	{
		//  连接播放指定文件所需的filter    
	    pGraph->RenderFile(filename, NULL); 
		//  开始播放指定的文件    
		pMediaControl->Run(); 
		if(ipBasAudio)
		{
			Dprintf("g_RingSound = %x\n", g_RingSound);

			//waveOutSetVolume(NULL, g_RingSound);
			
			ipBasAudio->put_Volume(g_RingSound); 
		}
	}
	m_bStartRing = TRUE;
}

void Phone::StopRing(bool releaseSrc)
{
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
			m_pMsgWnd->PostMessage(WM_STOPTRYRING);
		}
		m_bStartRing = FALSE;
	}
	m_pMsgWnd = NULL;
	::LeaveCriticalSection(&m_ringSetion_);
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
///////////////zzw///////////////////
void Phone::RestorRing(int index)
{
	
	DWORD volume[]={0x10001000,0x44004400,0x88008800,0xcc00cc00,0xff00ff00};
	waveOutSetVolume(NULL, volume[index]);
	int volume2[] = {-2400, -1800, -1200, -600, -1};
	SetSoundRingVal(volume2[index]);	
}

BOOL Phone::SetWatchDog(void)
{
	if (m_hPort != INVALID_HANDLE_VALUE)
	{
		unsigned long size;
		if(flag){
			if(DeviceIoControl(m_hPort, SCL_ENABLE_WATCHDOG, NULL, 0, NULL, 0, &size, NULL))
			{  
				return TRUE;
			}
		}else
		{
			if(DeviceIoControl(m_hPort, SCL_SYSTEM_RESET_RIGHTNOW, NULL, 0, NULL, 0, &size, NULL))
			{  
				return TRUE;
			}

		}
	}
	return FALSE;
}
BOOL Phone::ResetSystem(void)
{
	if (m_hPort != INVALID_HANDLE_VALUE)
	{
		unsigned long size;
		
		if(DeviceIoControl(m_hPort, SCL_SYSTEM_RESET_RIGHTNOW, NULL, 0, NULL, 0, &size, NULL))
		{  
             
			flag=false;

			return TRUE;
		}
	}
	return FALSE;
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
	g_RingSound = DEFUALT_SOUND_VALUME;
	if(ipBasAudio)
	{
		ipBasAudio->put_Volume(g_RingSound); 
	}
}
BOOL Phone::GetManuInfo(char *pBuffer, int length)
{
	if (m_hPort != INVALID_HANDLE_VALUE)
    {
        //Get Information;
        unsigned long size;

        UINT nIn;

        if(DeviceIoControl(m_hPort, SCL_GET_PRODUCT_INFO, &nIn, 0, pBuffer, 512, &size, NULL))
        { 
            return TRUE;
        }
    }

    return FALSE;

}
bool DecodeCallIDPackage(BYTE *buf, CALLID_INFO *pcallid)
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
	return flag;
}

//////////////////////////////////////////
//
//	解 DTMF 来电包
//
bool DecodeDTMFCallIDPackage(BYTE *buf, CALLID_INFO *pcallid)
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
bool DecodeFSKCallIDPackage(BYTE *buf, CALLID_INFO *pcallid)
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
	return true;
}

////////////////////////////////
//
//	解 FSK 来电简单格式参数包
//
void DecodeFSKSimpleParam(BYTE *pd, int datalen, CALLID_INFO *pcallid)
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
void DecodeFSKComplexParam(BYTE *pparm, CALLID_INFO *pcallid)
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


/////////////SIMDevice/////////////////
BYTE gSIMRS232[1024];
int gSIMLength;

void ParseSIMData(unsigned char const* const data, unsigned int const length)
{
	CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	
	int i;
	for (i=0; i < length; i++)
	{
		if(data[i] == 0x9F)
			main->simdevice_->mLength = data[i+1];
	}
	
	memcpy(main->simdevice_->m_chReadBuff, data, length);
	SendMessage(main->simdevice_->m_pOwnerCwd->m_hWnd, WM_SIMDATA_OK, (WPARAM)length, (LPARAM)(main->simdevice_->m_chReadBuff));
	memset(main->simdevice_->m_chReadBuff, 0, 1024);
	main->simdevice_->m_nReadCount = 0;
}


void ReadThreadPrc(void)
{
	CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	int gIndex = 0;
	
	while(!main->simdevice_->m_bExit && gIndex++ < 3)
	{
		if(main->simdevice_->m_bRecvFlag)
		{
			DWORD size;
			DeviceIoControl(main->simdevice_->m_hPort, SIM_WRITE, main->simdevice_->m_chSendBuff, main->simdevice_->m_nSendLength, gSIMRS232 , main->simdevice_->m_nRecvLen, &size, NULL);
			if(size == main->simdevice_->m_nRecvLen)
			{
				ParseSIMData(gSIMRS232, size);

				main->simdevice_->m_bRecvFlag = FALSE;
				main->simdevice_->m_bExit = TRUE;
			}
			else if( size != main->simdevice_->m_nRecvLen)
			{
				SendMessage(main->simdevice_->m_pOwnerCwd->m_hWnd, WM_SIMDATA_ERROR, 0, 0);
				main->simdevice_->m_bRecvFlag = FALSE;
				main->simdevice_->m_bExit = TRUE;
			}
			::Sleep(10);
		}
		else
			::Sleep(10);
		
	}
}

SIMDevice::SIMDevice(CWnd *pWnd)
{
	mLength = 0;				//response lenth
	m_nReadCount = 0;			//datalen
	mRecordCount = 0;

	m_pOwnerCwd = pWnd;
	m_bRecvFlag = FALSE;
	m_bExit = TRUE;

	m_hPort = CreateFile (_T("SIM1:"), // Pointer to the name of the port
		GENERIC_READ | GENERIC_WRITE, // Access (read-write) mode
		0,            // Share mode
		NULL,         // Pointer to the security attribute
		CREATE_ALWAYS,// Create New File as any case
		0,            // Port attributes
		NULL);
	if(m_hPort == INVALID_HANDLE_VALUE)
	{
		Dprintf("SIM1 Error\n");
	}
}

SIMDevice::~SIMDevice()
{

}

void SIMDevice::WritePort(const unsigned char * const data, int dataLength, int recvLen)
{
	if(m_hPort != INVALID_HANDLE_VALUE)
	{
		if(m_bExit)		//创建线程
		{
			m_bExit = FALSE;
			HANDLE hSimReadThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ReadThreadPrc, NULL, 0, NULL);
		}

		memcpy(m_chSendBuff, data, dataLength);
	    m_nSendLength = dataLength;
		m_nRecvLen = recvLen;
		m_bRecvFlag = TRUE;
	}
}

BOOL SIMDevice::Reset()
{
	if(m_hPort != INVALID_HANDLE_VALUE)
	{
		DWORD size;
		BOOL Reset_OK = FALSE;
		int nCount = 0;
		while(!Reset_OK && nCount++ < 2)
		{
			DeviceIoControl(m_hPort, SIM_RESET, NULL, 0, &Reset_OK , sizeof(BOOL), &size, NULL);

			Sleep(5);
		}
		return Reset_OK;
	}
	return FALSE;
}
