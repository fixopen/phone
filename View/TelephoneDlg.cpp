// telephonedlg.cpp : implementation file
//

#include "stdafx.h"
#include "..\multimediaphone.h"
#include "telephonedlg.h"
#include "../Data/LanguageResource.h"
#include "../MultimediaPhoneDlg.h"
#include "../Logical/Telephone.h"
#include "../Data/SkinStyle.h"
#include "../inc/shine.h"
#include <queue>
#include "sip.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define SECONDBYTES8           2048
#define SECONDBYTES2           850

#define IDC_TELDLGSHOW		1000
#define IDC_TELDLGHIDE		1004
#define IDT_STOPAUTORECORDE_TIME	1001
#define IDT_TEL_STATUS		1010
#define IDT_TEL_EXIT		1011
#define IDT_DIAL_PRESS		1012
#define IDT_AUTO_DIAL       1013

//add by qi 2009_11_09
#define SHOW_10CONTACTINFO_TIMER       1014 

typedef enum __WAVEDEV_MSG__
{
	WAV_PLAYBACK_FLAG       = 0x10000000,
		WAV_SPEAKER_ONLY		= 0x10000001,
		WAV_LINEOUT_ONLY		= 0x10000002,
		WAV_HANDLE_ONLY			= 0x10000004,
		
		WAV_SNDEFFECT_POP		= 0x20000000,
		WAV_SNDEFFECT_3D		= 0x20000001,
		WAV_SNDEFFECT_CLASSIC	= 0x20000002,
		WAV_SNDEFFECT_JAZZ		= 0x20000003,
		
		WAV_RECORD_FLAG			= 0x30000000,
		WAV_LINEIN_MIC			= 0x30000001,
		WAV_LOCAL_MIC			= 0x30000002,
		WAV_LOCAL_HANDLE		= 0x30000004,
		
		WAV_SND_SOFTMUTE_ON		= 0x40000003,
		WAV_SND_SOFTMUTE_OFF	= 0x40000004,
		
		WAV_MIC_GAIN   			= 0x40000001
};

#define RIFF_FILE       mmioFOURCC('R','I','F','F')
#define RIFF_WAVE       mmioFOURCC('W','A','V','E')
#define RIFF_FORMAT     mmioFOURCC('f','m','t',' ')
#define RIFF_DATA       mmioFOURCC('d','a','t','a')

//#define  IGNORERINGCOUNT 1

int initResult = -1;

HINSTANCE m_hSABTD;
typedef int (__stdcall *SabtdInit)(void **ppSabtdStatus, int sampleRate, char *license);
typedef int (__stdcall *SabtdUnInit)(void *pSabtdStatus);
typedef int (__stdcall *SabtdSetParam)(void *pSabtdStatus, ParamType paramType, float paramValue);
typedef int (__stdcall *SabtdGetParam)(void *pSabtdStatus, ParamType paramType, float *pParamValue);
typedef int (__stdcall *SabtdReset)(void *pSabtdStatus);
typedef int (__stdcall *SabtdProcess)(void *pSabtdStatus, int *result, short *data, int length);

SabtdInit SabtdInit_;
SabtdUnInit SabtdUnInit_;
SabtdSetParam SabtdSetParam_;
SabtdGetParam SabtdGetParam_;
SabtdReset SabtdReset_;
SabtdProcess SabtdProcess_;

static void* m_sabtd_struct;
static std::queue<OggCodec::PcmData*> CheckQueue;
static HANDLE hCheckThread;
static volatile bool bVoiceStop = true;
CRITICAL_SECTION csVoice;
short * pVoice;
int nVoiceLength = 32000;

BOOL g_isPhoneMute = FALSE;

CString telNumber;
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
DWORD WINAPI VoiceCheckThreadPrc(LPVOID lpParameter)
{
	while (true)
	{
		::Sleep(0);
		EnterCriticalSection(&csVoice);
		bool b = CheckQueue.empty();
		LeaveCriticalSection(&csVoice);
		if (!b)
		{
			EnterCriticalSection(&csVoice);
			OggCodec::PcmData* m = CheckQueue.front();
			memset(pVoice, 32000*sizeof(short), 0);
			memcpy(pVoice, m->data, m->dataLength*sizeof(short));
			nVoiceLength = m->dataLength;
			free(m->data);
			delete m;
			CheckQueue.pop();
			LeaveCriticalSection(&csVoice);
			
			int toneDetected = 0;
			if (SabtdProcess_ != NULL)  
			{
				if (SabtdProcess_(m_sabtd_struct, &toneDetected, pVoice, nVoiceLength) < 0)
				{
					Dprintf("SABTD_Process Process Error!\n");
					if (SabtdReset_ != NULL)
					{
						SabtdReset_(m_sabtd_struct);
					}
//					break;
				}
			}
			
			if ((toneDetected == SABTD_TONE_DETECTED) /*|| (toneDetected == SABTD_SILENCE_DETECTED)*/)   //静音屏蔽
			{
				Dprintf("SABTD_Process Process OK! %d\n", toneDetected);
				if (SabtdReset_ != NULL)
				{
					SabtdReset_(m_sabtd_struct);
				}
				((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->HungOff();	
			}
			continue;

		}
		else if (bVoiceStop)
		{
			break;
		}
		Sleep(10);
	}
	
	if (SabtdReset_ != NULL)
	{
		SabtdReset_(m_sabtd_struct);
	}

	EnterCriticalSection(&csVoice);
	int s = CheckQueue.size();
	for (int i = 0; i < s; ++i)
	{
		OggCodec::PcmData* m = CheckQueue.front();
		free(m->data);
		delete m;
		CheckQueue.pop();
	}
	LeaveCriticalSection(&csVoice);

	CloseHandle(hCheckThread);
	hCheckThread = NULL;
	Dprintf("\nVoice Check over!\n");
	return 0;

}

#define  SIMADDRLENGTH  29
std::string StartSearch(FILE* file, const std::string& mobile, int low, int high)
{
    if(low>high)
    {
        return "";
    }
    else
    {
		char buff[32];
		std::string result;
        int mid=(low+high)/2;
		fseek(file, 0, SEEK_SET);
		fseek(file, mid * SIMADDRLENGTH, SEEK_SET);
		fread(buff, 1, SIMADDRLENGTH, file);
		result = std::string(buff, SIMADDRLENGTH - 2);
        if(result.substr(0, 7) == mobile)
        {
            return result;
        }
        else if(result.substr(0, 7) > mobile)
        {
            return StartSearch(file,mobile,low,mid-1);
        }
        else
        {
            return StartSearch(file,mobile,mid+1,high);
        }
    }
}

/////////////////////////////////////////////////////////////////////////////
// CTelephoneDlg dialog


CTelephoneDlg::CTelephoneDlg(CWnd* pParent /*=NULL*/)
	: CDialog /*CCEDialog*/(CTelephoneDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTelephoneDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
//	m_bRing = FALSE;
	m_strTelStatus = "";
	m_nTelStatus = -1;
	m_bTelUsing = FALSE;
	m_bFirstConnect = TRUE;
	m_bSoftWareOff  = false; 

	m_bSend0x82Win = false;
	m_nAllPhoneNum = 1;

}

void CTelephoneDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTelephoneDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

void CTelephoneDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pNotebookDlg->m_bIsOPenTel = FALSE;
	CDialog::OnLButtonDown(nFlags, point);
}

BEGIN_MESSAGE_MAP(CTelephoneDlg, CDialog /*CCEDialog*/)
	//{{AFX_MSG_MAP(CTelephoneDlg)
	ON_WM_TIMER()
	ON_MESSAGE(MM_WIM_OPEN,OnMM_WIM_OPEN)
	ON_MESSAGE(MM_WIM_DATA,OnMM_WIM_DATA)
	ON_MESSAGE(MM_WIM_CLOSE,OnMM_WIM_CLOSE)
	ON_MESSAGE(MM_WOM_OPEN,OnMM_WOM_OPEN)
	ON_MESSAGE(MM_WOM_DONE,OnMM_WOM_DONE)
	ON_MESSAGE(MM_WOM_CLOSE,OnMM_WOM_CLOSE)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_TELEPHONE_HIDE, OnButtonTelephoneHide)
	ON_BN_CLICKED(IDC_BUTTON_TELEPHONE_NOTE, OnButtonTelephoneNote)
	ON_BN_CLICKED(IDC_BUTTON_TELEPHONE_RECORD, OnButtonTelephoneRecord)
	ON_BN_CLICKED(IDC_BUTTON_TELEPHONE_HANDLE, OnButtonTelephoneHandle)
	ON_MESSAGE(WM_CLICKMJPG_TOAPP, OnClickMJPG)
	ON_MESSAGE(WM_MJPGTOGGLE, OnClickMJPG)
	ON_MESSAGE(WM_TEL_STATUS, OnTelStatus)
	ON_MESSAGE(WM_CALLWAITTING,OnCallWaitting)
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

void CTelephoneDlg::OnTelStatus(WPARAM w, LPARAM l)
{	
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	char *chTel_status[] = {"对方忙!", "通话中...", "对方已接听!", "通话保持!", "正在呼叫...", "对方振铃...", "等待...", "无拨号音!", "无应答!", "呼叫结束!","网络拥塞!", "对方挂机", "呼叫受限", "来电呼入..."};
	
	if (main->m_phoneLine[l-1].b3gReject)//拒接状态
	{
		return;
	}
	
	switch(w)
	{
	case TEL_FROMBUY: //对方忙
		//SetTimer(main->m_phoneLine[l-1].HangoffTimer, 4000, NULL);
		HandleOff(l);
		break;

	case TEL_FROMCONNECTED:  //通话联通状态
	//	if(!m_bFirstConnect)
	//	{	
			main->m_phoneLine[(int)l-1].pFSM->fireEvent(CMultimediaPhoneDlg::p3geConnected,(void*)(l-1));
			m_nTelStatus = w;
		//	return;
		//}
		//m_bFirstConnect = FALSE;
		::KillTimer((theApp.m_pMainWnd)->m_hWnd, ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_uiKeyTimer);
		::SetTimer((theApp.m_pMainWnd)->m_hWnd, ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_uiKeyTimer, 50, 0);
		break;

	case TEL_FROMCONNECT: //对方接听
		{
			main->m_phoneLine[(int)l-1].pFSM->fireEvent(CMultimediaPhoneDlg::p3geConnected,NULL);
		}
		break;

	case TEL_FROMHELD://保持状态
		{	
			main->m_phoneLine[(int)l-1].pFSM->fireEvent(CMultimediaPhoneDlg::p3geHold,NULL);
		}
		break;

	case TEL_FROMDIALING:    //正在拨打状态 
		main->m_phoneLine[(int)l-1].pFSM->fireEvent(CMultimediaPhoneDlg::p3geDialing,NULL);
		break;

	case TEL_FROMALERTING:   //对方振铃状态
		main->phone_->PhoneDialTone(0, NULL);
		main->phone_->Mute(false);//wangzhenxing20100805
		main->m_phoneLine[(int)l-1].pFSM->setStartState(CMultimediaPhoneDlg::p3gsAltering);
		main->m_phoneLine[(int)l-1].TelStatus = w;
		break;

	case TEL_FROMWAITING: //对方呼入
		break;

	case TEL_FROMNODIALTONE: 
		//SetTimer(main->m_phoneLine[l-1].HangoffTimer, 100, NULL);
		HandleOff(l);
		break;

	case TEL_FROMNOANSWER:   //无应答
		//SetTimer(main->m_phoneLine[l-1].HangoffTimer, 100, NULL);
		main->phone_->PhoneDialTone(1, "hangup");
		HandleOff(l);
		break;

	case TEL_FROMNOCARRIER:  //连接不能被建立
		//SetTimer(main->m_phoneLine[l-1].HangoffTimer, 100, NULL);
		HandleOff(l);
		break;

	case TEL_FROMCONGESTION:  //网络拥塞
		//SetTimer(main->m_phoneLine[l-1].HangoffTimer, 100, NULL);
		HandleOff(l);
		break;

	case TEL_FROMOPPHUNGUP:   //对方挂机
		//SetTimer(main->m_phoneLine[l-1].HangoffTimer, 100, NULL);
		main->phone_->PhoneDialTone(1, "hangup");
		HandleOff(l);
		break;

	case TEL_FROMODB:         //呼叫限制
		//SetTimer(main->m_phoneLine[l-1].HangoffTimer, 100, NULL);
		main->phone_->PhoneDialTone(1, "hangup");
		HandleOff(l);
		break;

	case TEL_FROMRING:         //来电
		break;

	case TEL_FROMEND:
		break;

	default:
		break;

	}

	m_nTelStatus = w;
	m_strTelStatus = chTel_status[w];
	
	//在PSTN是连接状态,状态标记不显示在主界面上
	if (main->m_phoneLine[2].pFSM->getCurrentState() == CMultimediaPhoneDlg::pstnsConnected)
	{
		return ;
	}

	//紧急呼叫状态特殊判断
	if (main->m_phoneLine[(int)l-1].telnum.compare("112") == 0)
	{
		if (!(m_nTelStatus == TEL_FROMNODIALTONE || m_nTelStatus == TEL_FROMNOANSWER
			|| m_nTelStatus == TEL_FROMNOCARRIER || m_nTelStatus == TEL_FROMCONGESTION
			|| m_nTelStatus == TEL_FROMOPPHUNGUP || m_nTelStatus == TEL_FROMODB
			|| m_nTelStatus == TEL_FROMBUY))
		{
			m_strTelStatus = L"紧急呼叫...";	
		}
		
	}

	if (l >0 && l < 3)
	{   
		main->m_phoneLine[l-1].lineStatus = Util::StringOp::FromCString(m_strTelStatus);//chTel_status[w];

		if (m_nAllPhoneNum == l)
		{			
			int line = l - 1;
			bool bfresh = false;
			if(main->m_phoneLine[line].pFSM->getCurrentState() == CMultimediaPhoneDlg::p3gsHold &&
				main->m_phoneLine[(line+1)%2].pFSM->getCurrentState() >= CMultimediaPhoneDlg::p3gsDialing &&
				main->m_phoneLine[(line+1)%2].pFSM->getCurrentState() != CMultimediaPhoneDlg::p3gsHold)
			{	
				if (main->m_nline != (line+1)%2)
				{
					bfresh = true ;
				}
				main->m_nline = (line+1)%2;
			}
			else if(main->m_phoneLine[line].pFSM->getCurrentState() >= CMultimediaPhoneDlg::p3gsDialing &&
				main->m_phoneLine[line].pFSM->getCurrentState() != CMultimediaPhoneDlg::p3gsHold &&
				main->m_phoneLine[(line+1)%2].pFSM->getCurrentState() == CMultimediaPhoneDlg::p3gsHold )
			{	
				if (main->m_nline != line)
				{
					bfresh = true ;
				}
				main->m_nline = line;//
			}
			else if(main->m_phoneLine[line].pFSM->getCurrentState() == CMultimediaPhoneDlg::p3gsConnected &&
				(main->m_phoneLine[(line+1)%2].pFSM->getCurrentState() >= CMultimediaPhoneDlg::p3gsDialing &&
				main->m_phoneLine[(line+1)%2].pFSM->getCurrentState() < CMultimediaPhoneDlg::p3gsConnected))
			{	
				if (main->m_nline != line)
				{
					bfresh = true ;
				}
				main->m_nline = line;//
			}
			else if((main->m_phoneLine[line].pFSM->getCurrentState() >= CMultimediaPhoneDlg::p3gsDialing &&
				main->m_phoneLine[line].pFSM->getCurrentState() < CMultimediaPhoneDlg::p3gsConnected )&&
				main->m_phoneLine[(line+1)%2].pFSM->getCurrentState() == CMultimediaPhoneDlg::p3gsConnected )
			{
				if (main->m_nline != (line+1)%2)
				{
					bfresh = true ;
				}
				main->m_nline = (line+1)%2;//
			}
			
			if (m_nAllPhoneNum > 1)
			{
				ShowMainInfo();
			//	if (bfresh)
			//	{
			//		m_MJPGList.Invalidate();
			//	}
				ShowLineStatus(l-1);
				ShowLineStatus(l%2);
			}
			else
			{
				if (main->m_phoneLine[l-1].pFSM->getCurrentState() !=
					CMultimediaPhoneDlg::p3gsHangOff )
				{	
					ShowLineStatus(l-1);
				}
			}

		}

	}
	
}

/////////////////////////////////////////////////////////////////////////////
// CTelephoneDlg message handlers
void CTelephoneDlg::DialSpeedCode(int index)
{
	m_MJPGList.SetUnitText(100, "", TRUE);
	CMultimediaPhoneDlg *pMainDlg = ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd));
	std::string telcode = "";
	if(index < 5&& index >= 0)
	{
		std::vector<std::pair<std::string, std::string> > ms = pMainDlg->m_pSettingDlg->m_pSetting->speedDials();
		telcode = ms[index+1].second;
	}
	else if(index == 5)
	{
		telcode = "110";
	}
	else if(index == 6)
	{
		telcode = "120";
	}
	else if(index == 7)
	{
		telcode = "119";
	}
	if(telcode != "")
	{
		for(int i = 0; i < telcode.length(); i++)
		{
			pMainDlg->SendMessage(WM_TEL_KEYCODE, (WPARAM)telcode[i], 0);
		}
	}

}

void CTelephoneDlg::OnClickMJPG(WPARAM w, LPARAM l)
{
	CMultimediaPhoneDlg *pMainDlg = ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd));
	CWnd *p;
	CString icon;
	switch (w)
	{	
		case 1://设置
			
			break;

		case 10://录音
		//	OnButtonTelephoneRecord();
			break;

		case 11://便签 == 记事
			OnButtonTelephoneNote();
			break;

		case 12://通讯录
			pMainDlg->m_pContactDlg->ShowRightBtn(false);
			pMainDlg->m_pContactDlg->ShowWindow_();
			p = pMainDlg->m_pContactDlg;
			icon = Allicon[2];
			pMainDlg->AddIcon(icon,p,false);
			break;

		case 13://通话记录
			pMainDlg->m_pContactInfoDlg->ShowRightBtn(false);
			pMainDlg->m_pContactInfoDlg->ShowWindow_();
			p = pMainDlg->m_pContactInfoDlg;
			icon = Allicon[3];
			pMainDlg->AddIcon(icon,p,false);
			break;

		case 14://二次拨号
			Hold2Dial();
			break;
			
		case 20://隐藏
			OnButtonTelephoneHide();
			pMainDlg->PopbackIcon();
			break;

		case 21://切换
			PhoneSwitch();
			break;

		case 22://挂机
			{
				//挂断这一路
				int g3num = 0;
				pMainDlg->Find3gLineNum(g3num);
				if (g3num > 1)
				{
					CString toOffline;
					toOffline.Format(L"1%d",pMainDlg->m_nline + 1);
					pMainDlg->m_pATCommandWarp1->CHLD(atoi(toOffline));
					
				}
				else 
				HandleOff();
			}   			
			break;
		
		case 23: //选项
			Option();
			break;

		case 24://三方会议
			pMainDlg->m_pATCommandWarp1->CHLD(3);
			break;

		case 40:
		case 41:
		case 42:
		case 43:
		case 44:
			SetVolume(w-40);
			break;

		case 50://设置声音
			VolumeSwitch();
			break;

		case 60://三方通话
//			pMainDlg->m_pATCommandWarp1->CHLD(3);
			break;
	
		case 1000:
			ShowWindow(SW_HIDE);		
			pMainDlg->PopbackIcon();
			m_bSoftWareOff = true;
			HandleOff();
			break;

		default:
		break;
	}

}
BOOL CTelephoneDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_bClickSubDial = false;
	m_nLevel = 3 ;
//	m_uiIgnoreRingCount = 1;
	m_uiIPstngnoreRingCount = 1;
	m_nSelectLine = -1;
	std::string strTemp;
	CString str;
	
	m_MJPGList.Create(L"", WS_VISIBLE|WS_CHILD, CRect(0, 0, 800, 423), this,10086);
	m_MJPGList.SetCurrentLinkFile(".\\adv\\mjpg\\k5\\中文\\电话显示.xml");
	m_MJPGList.SetMJPGRect(CRect(0, 0, 800, 423));
	
	MoveWindow(0,57,800,423);
	
	SetAudioLevel();
	
	m_pTelePhoneOptionDlg = new CTelePhoneOptionDlg();
	m_pTelePhoneOptionDlg->Create(CTelePhoneOptionDlg::IDD,this);
	m_pTelePhoneOptionDlg->ShowWindow_(SW_HIDE);
			
	if (!InitData())
	{
		MessageBox(_T("Telephone Initialize error!\n"), _T("Error"));
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

bool CTelephoneDlg::InitData(void)
{
	m_strRecord = CString(Data::LanguageResource::Get(Data::RI_TEL_RECORDEBTN).c_str());
	m_strStopRecord = CString(Data::LanguageResource::Get(Data::RI_TEL_STOPRECORDEBTN).c_str());
	m_strHangOn = CString(Data::LanguageResource::Get(Data::RI_TEL_ACCEPTBTN).c_str());
	m_strHangOff = CString(Data::LanguageResource::Get(Data::RI_TEL_HUNGOFFBTN).c_str());

	m_uiRecordTimer = 2;
	m_uiRecordSecond = 0;
//	m_uiRingCount = 0;
	m_uiTipTimer = 3;
	m_uiInNoCount = 0;
	m_uiRecordCound = 0;
//	m_bHasCallID = FALSE;
	m_bPlayingLeaveWord = FALSE;
//	m_bFirwall = FALSE;
//	m_bRingPlayed = FALSE;	
	
	//pstn
	m_uiPstnRingCount = 0 ;
	m_uiPstnTelephoneTimer = 100;
	m_uPstnTelSecondOff = 0;
	m_uiPstnTelephoneSecond = 0;

	m_bPstnFirwall = FALSE;
	m_bPstnHasCallID = FALSE;
	m_bPstnRingPlayed = FALSE;
	m_bPstnReject = FALSE;
	m_bPstnFirstRing = FALSE;

	m_spContactInfo[0] = boost::shared_ptr<Data::ContactInfo>();
	m_spContactInfo[1] = boost::shared_ptr<Data::ContactInfo>();

	m_spPstnContactInfo = boost::shared_ptr<Data::ContactInfo>();
	m_spSoundSegment = boost::shared_ptr<Data::SoundSegment>();
	
	m_bEnding = FALSE;
	m_bPlaying = FALSE;
	m_bRecording = FALSE;
	m_dwDataLength = 0;

	m_MJPGList.SetUnitIsShow(104, false,true);
	m_MJPGList.SetUnitIsShow(105, false,true);
	
	m_pOggCodec = new OggCodec();
	pVoice = (short*)malloc(nVoiceLength*sizeof(short));
	InitializeCriticalSection(&csVoice);
	
	m_waveform.wFormatTag=WAVE_FORMAT_PCM;
	m_waveform.nChannels=CHANNELS;
	m_waveform.nSamplesPerSec=SAMPLESPERSEC;
	m_waveform.wBitsPerSample=BITSPERSAMPLE;
	m_waveform.nBlockAlign=m_waveform.nChannels * m_waveform.wBitsPerSample / 8;
	m_waveform.nAvgBytesPerSec=m_waveform.nBlockAlign * m_waveform.nSamplesPerSec;
	m_waveform.cbSize=0;	

	for (int i = 0; i < WAVE_BUFFER_COUNT; ++i)
	{
		m_pBuffer[i] = 	(PBYTE)malloc(INP_BUFFER_SIZE);
		m_pWaveHdr[i] = reinterpret_cast<PWAVEHDR>(malloc(sizeof(WAVEHDR)));
	}

	m_hSABTD = NULL;
	m_hSABTD = LoadLibrary(_T("\\flashdrv\\res_dat\\SABTD_CEDLL.dll")); 
	if (m_hSABTD == NULL)
	{
		SabtdInit_ = NULL;
		SabtdUnInit_ = NULL;
		SabtdSetParam_ = NULL;
		SabtdGetParam_ = NULL;
		SabtdReset_ = NULL;
		SabtdProcess_ = NULL;
	}
	else
	{
		SabtdInit_ = (SabtdInit)GetProcAddress(m_hSABTD, _T("SABTD_Init"));
		SabtdUnInit_ = (SabtdUnInit)GetProcAddress(m_hSABTD, _T("SABTD_UnInit"));
		SabtdSetParam_ = (SabtdSetParam)GetProcAddress(m_hSABTD, _T("SABTD_SetParam"));
		SabtdGetParam_ = (SabtdGetParam)GetProcAddress(m_hSABTD, _T("SABTD_GetParam"));
		SabtdReset_ = (SabtdReset)GetProcAddress(m_hSABTD, _T("SABTD_Reset"));
		SabtdProcess_ = (SabtdProcess)GetProcAddress(m_hSABTD, _T("SABTD_Process"));

		if(SabtdInit_ && SabtdUnInit_ && SabtdSetParam_ && SabtdGetParam_ && SabtdReset_ && SabtdProcess_) 
		{
			int initResult = 0;
			initResult = SabtdInit_(&m_sabtd_struct, SAMPLESPERSEC, "\\flashdrv\\res_dat\\sa_license.dat");
			if ((initResult==-1000) || (initResult<0 || m_sabtd_struct==NULL))
			{
				Dprintf("SABTD_CEDLL Initialize error!\n");
				SabtdInit_ = NULL;
				SabtdUnInit_ = NULL;
				SabtdSetParam_ = NULL;
				SabtdGetParam_ = NULL;
				SabtdReset_ = NULL;
				SabtdProcess_ = NULL;
			}
			else
			{
				if (initResult == SARTC_NORMAL)
				{
					SabtdSetParam_(m_sabtd_struct, SABTD_PARAMTYPE_ENERGY, 1.0);
					SabtdSetParam_(m_sabtd_struct, SABTD_PARAMTYPE_FRONT_NOTTONE, 0);
					SabtdSetParam_(m_sabtd_struct, SABTD_PARAMTYPE_BUSYTONE_NUM, 3);
 					SabtdSetParam_(m_sabtd_struct, SABTD_PARAMTYPE_SILENCE_LENGTH, 5);
				}
			}
		}
	}
	
	((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->OpenTelRing(FALSE);

	return true;
}

void CTelephoneDlg::OnButtonTelephoneHide()
{	
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	main->phone_->StopRing(true);
	main->m_MJPGList.SetUnitIsShow(15,true,true);

	extern void GMute(BOOL isOn);
	if (!main->phone_->Hand())
	{
		GMute(FALSE);
	}	

	if (main->m_pTelphoneDialDlg->IsWindowVisible())
	{
		main->m_pTelphoneDialDlg->ShowWindow(SW_HIDE);	
		main->PopbackIcon();
	}

	ShowWindow_(FALSE);
/*	if(!m_bTelUsing)
	{
		m_bTelUsing = FALSE;
		m_nTelStatus = -1;
		m_bFirstConnect = TRUE;
		m_strTelStatus = "";
		KillTimer(IDT_TEL_STATUS);
		KillTimer(IDT_TEL_EXIT);
		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_phoneLine[0].pFSM->fireEvent(CMultimediaPhoneDlg::p3geHangOff, NULL);
	
	}*/
}

void CTelephoneDlg::OnButtonTelephoneNote()
{
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	main->m_pNotebookDlg->ShowWindow_(SW_SHOW);
	main->AddIcon(Allicon[8],main->m_pNotebookDlg,false);
	main->AddDesktopBtn();

}

std::string CTelephoneDlg::GetSoundPath(void)
{
	path = ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pSetting->soundPath();
	
	BOOL DetectDIR(TCHAR *sDir);
	if (path == ssStorageCardRecordPath)
	{
		BOOL sd = DetectDIR((LPTSTR)(LPCTSTR)csStarageCard);
		if (sd)
		{
			if (!DetectDIR((LPTSTR)(LPCTSTR)csStorageCardRecordPath))
			{
				CreateDirectory((LPTSTR)(LPCTSTR)csStorageCardRecordPath, NULL);
			}
		}
		else
		{
			path = ssFlashRecordPath;
			if (!DetectDIR((LPTSTR)(LPCTSTR)csFlashRecordPath))
			{
				CreateDirectory((LPTSTR)(LPCTSTR)csFlashRecordPath, NULL);
			}
		}
	}
	return path;
}

void CTelephoneDlg::OnButtonTelephoneRecord()
{
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	if (!m_bRecording)
	{
		GetSoundPath();

		ULARGE_INTEGER freeBytes;
		ULARGE_INTEGER totalBytes;
		GetDiskFreeSpaceEx(Util::StringOp::ToCString(path), &freeBytes, &totalBytes, NULL);

		int secondBytes = SECONDBYTES8;
		if (((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pSettingDlg->m_pSetting->isDeleteProtect())
		{
			secondBytes = SECONDBYTES8;
		}
		else
		{
			secondBytes = SECONDBYTES2;
		}
		
		if (path == ssFlashRecordPath)
		{		
			if (freeBytes.QuadPart < totalBytes.QuadPart * 0.1)
				//if (totalBytes.QuadPart < MINFREEBYTES)
			{
				main->m_pWarningNoFlashDlg->ShowWindow_(SW_SHOW);
				return;
			}
			
			//m_uiRemainRecordSecond = (totalBytes.QuadPart - MINFREEBYTES) / SECONDBYTES + 1;
			m_uiRemainRecordSecond = (freeBytes.QuadPart - (totalBytes.QuadPart * 0.1)) / secondBytes + 1;
		}
		else
		{
			if (freeBytes.QuadPart < secondBytes*2)
			{
				main->m_pWarningNoFlashDlg->ShowWindow_(SW_SHOW);
				return;
			}
			m_uiRemainRecordSecond = freeBytes.QuadPart / secondBytes + 1;
		}

		if (RecStart())
		{
			Dprintf("RecStart Ok\r\n");
					
			m_spSoundSegment = boost::shared_ptr<Data::SoundSegment>(new Data::SoundSegment);
			m_vSoundSegment.push_back(m_spSoundSegment);

			CTime t;
			t = CTime::GetCurrentTime();
			CString filename;
			
			if ( main->m_pSettingDlg->m_pSetting->isDeleteProtect() )
			{
				m_pOggCodec->SetQuality(8);
				filename.Format(_T("%02d%02d%02dHQ.spx"), t.GetHour(), t.GetMinute(), t.GetSecond());
			}
			else
			{
				m_pOggCodec->SetQuality(2);
				filename.Format(_T("%02d%02d%02dST.spx"), t.GetHour(), t.GetMinute(), t.GetSecond());
			}

			m_sRecordFileName = path + Util::StringOp::FromCString(filename);
			m_spSoundSegment->filename(m_sRecordFileName);
			m_spSoundSegment->startTime(CTime::GetCurrentTime());
			m_spSoundSegment->contactInfoId(m_spContactInfo[main->m_nline]->id());
			m_spSoundSegment->telephoneNumber(m_spContactInfo[main->m_nline]->telephoneNumber());
			m_spSoundSegment->name(m_spContactInfo[main->m_nline]->name());
			if (m_spContactInfo[main->m_nline]->type() == Data::citInNo)
			{
				m_spSoundSegment->type(Data::sstLeaveWord);
			}
			else
			{
				m_spSoundSegment->type(Data::sstTeleRecord);
			}
			m_spSoundSegment->Insert();
			m_spContactInfo[main->m_nline]->isSound(true);
			m_spContactInfo[main->m_nline]->Update();

			Dprintf("m_pOggCodec->StartEncode 1\r\n");
			std::string comment = m_spSoundSegment->name() + "\'" + m_spSoundSegment->telephoneNumber().number() + "\'" + Util::StringOp::FromInt(m_spSoundSegment->type()) + "\'" + Util::StringOp::FromTimestamp(m_spSoundSegment->startTime()) + "\'";
			if (m_pOggCodec->StartEncode(m_sRecordFileName, comment))
			{
				Dprintf("m_pOggCodec->StartEncode 2\r\n");
				m_uiRecordSecond = 0;
				SetTimer(m_uiRecordTimer, 1000, 0);		     //lxz test		
			
			//	change by qi
			//	m_MJPGList.SetUnitIsShow(104, TRUE);
				m_MJPGList.SetUnitIsShow(106, TRUE);
				
				if (main->m_pSettingDlg->m_pSetting->isMustRecord())
				{
					m_MJPGList.SetUnitIsShow(2, FALSE);
					m_MJPGList.SetUnitIsShow(8, FALSE);
				}
				else
				{
					m_MJPGList.SetUnitIsShow(2, FALSE);
					m_MJPGList.SetUnitIsShow(8, TRUE);
				}
			}
		}			
	}
	else
	{
		if (RecStop())
		{
 			KillTimer(m_uiRecordTimer);

			if (m_uiRecordSecond == 0)
			{
				m_uiRecordSecond = 1;
			}
			Dprintf("record second = %d\n", m_uiRecordSecond);
			m_spSoundSegment->duration(m_uiRecordSecond);
			m_spSoundSegment->Update();
// 			m_spSoundSegment = boost::shared_ptr<Data::SoundSegment>();
 			m_uiRecordSecond = 0;
		}
	}
}

void CTelephoneDlg::HandleOn(void)
{
//	((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->HungOn(TRUE);
	::PostMessage(theApp.m_pMainWnd->m_hWnd, WM_TEL_HUNGON, 0, 0);
}

void CTelephoneDlg::HandleOff(void)
{	
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	m_bTelUsing = FALSE;
	m_nTelStatus = -1;
	m_bFirstConnect = TRUE;
	KillTimer(IDT_TEL_STATUS);
	KillTimer(IDT_TEL_EXIT);
	
//	((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_phoneLine[0].pFSM->fireEvent(CMultimediaPhoneDlg::p3geHangOff, NULL);	
//	main->phone_->HungOff();

	::PostMessage(main->m_hWnd, WM_TEL_HUNGOFF, 0, 0);//目前全部挂断

	m_strTelStatus = "";
	m_MJPGList.SetUnitText(400, "", TRUE);
			
}

void CTelephoneDlg::HandleOff(int const line)
{
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	
	if (main->m_phoneLine[line-1].pFSM->getCurrentState()
		> CMultimediaPhoneDlg::p3gsHangOn)
	{
		Dprintf("TelephoneDlg : HandleOff\r\n");
		main->phone_->StopRing(true);	
		::PostMessage(main->m_hWnd, WM_TEL_HUNGOFF, 0, line);

	}
			
}

void CTelephoneDlg::OnButtonTelephoneHandle()
{
	Dprintf("\nHandle \n");
	CString caption;
//	m_btnHandle.GetWindowText(caption);
	if (caption == m_strHangOn)
	{
		((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->HungOn();
	}
	else 
	{
		((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->HungOff();
	}	

}

void CTelephoneDlg::HangOff_(void* param)
{
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	m_nTelStatus = -1;
	m_bFirstConnect = TRUE;
	m_strTelStatus = "";
	KillTimer(IDT_TEL_STATUS);
	KillTimer(IDT_TEL_EXIT);

	KillTimer(5);
	KillTimer(IDC_TELDLGSHOW);
//	m_uiIgnoreRingCount = 1;
	
	m_uiIPstngnoreRingCount = 1;

 	((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->StopRing(true);

	Dprintf("WAV_SPEAKER_ONLG HangOff_\r");

	waveInMessage(0, WAV_SPEAKER_ONLY, 0, 0); //20090324

	KillTimer(m_uiTipTimer);
	KillTimer(main->m_phoneLine[main->m_nline].timer);
	KillTimer(m_uiPstnTelephoneTimer);

	if (m_bRecording)
	{
		OnButtonTelephoneRecord();
	}
	
	if (m_spContactInfo[main->m_nline])
	{
		m_spContactInfo[main->m_nline]->duration(main->m_phoneLine[main->m_nline].Second);
		if (m_spContactInfo[main->m_nline]->type() == Data::citInNo)
		{
			m_spContactInfo[main->m_nline]->played(false);
		}
		
		main->m_pContactInfoDlg->SaveContactInfo(m_spContactInfo[main->m_nline]);
		
		if (!m_vSoundSegment.empty())
		{
			for (std::vector<boost::shared_ptr<Data::SoundSegment> >::iterator iter = m_vSoundSegment.begin(); iter != m_vSoundSegment.end(); ++iter)
			{
//				if ((*iter)->duration() > 1)
				{
					(*iter)->telephoneNumber(m_spContactInfo[main->m_nline]->telephoneNumber());
					(*iter)->name(m_spContactInfo[main->m_nline]->name());
					(*iter)->contactInfoId(m_spContactInfo[main->m_nline]->id());
					(*iter)->contactId(m_spContactInfo[main->m_nline]->contactId());

					if (m_spContactInfo[main->m_nline]->type() == Data::citInNo)
					{
						(*iter)->type(Data::sstLeaveWord);
						(*iter)->played(false);

						//lxz 20090325
						::SendMessage(main->m_pMainDlg->GetSafeHwnd(), WM_TELNOTIFY, 2, 1);
					}
					else
					{
						(*iter)->type(Data::sstTeleRecord);
						(*iter)->played(true);
					}
					
					main->m_pSoundDlg->SaveSoundSegment(*iter);
				}
			}
		}
		m_vSoundSegment.clear();

		main->m_pContactInfoDlg->ResetTypeInfo();
		if (m_spContactInfo[main->m_nline]->type() == Data::citInNo)
		{
			::SendMessage(main->m_pMainDlg->GetSafeHwnd(), WM_TELNOTIFY, 1, 1);
		}

	}

	if(m_spContactInfo[main->m_nline]->type() != Data::citInNo && m_spContactInfo[main->m_nline]->duration() > 0)
		::PostMessage(main->GetSafeHwnd(), WM_TELCALL_TIME, m_spContactInfo[main->m_nline]->duration(), 0);
	m_spContactInfo[main->m_nline] = boost::shared_ptr<Data::ContactInfo>();
	main->m_phoneLine[main->m_nline].bHasCallID = false;
	main->m_phoneLine[main->m_nline].bFirwall = false;
	main->m_phoneLine[main->m_nline].bRingPlayed = false;
	main->m_phoneLine[main->m_nline].RingCount = 0;
	main->m_phoneLine[main->m_nline].iIgnoreRingCount = 1;

//	m_uiTelephoneSecond = 0;
//	m_uiRingCount = 0;
	m_uiPstnRingCount = 0;	
	//m_bHasCallID = FALSE;
	m_bPlayingLeaveWord = FALSE;
//	m_bFirwall = FALSE;
	m_sTipFilename = "";
//	m_sRingFilename = "";
	m_bRecording = FALSE;
//	m_bRingPlayed = FALSE;
	
	m_bPstnRingPlayed = FALSE;

	m_MJPGList.SetUnitText(100, "", FALSE);
	m_MJPGList.SetUnitText(101, "", FALSE);
	m_MJPGList.SetUnitText(102, "", FALSE);
	m_MJPGList.SetUnitText(103, "", FALSE);
	
	m_MJPGList.SetUnitIsShow(105,false,true);
	m_MJPGList.SetUnitText(107, "", FALSE);


	Dprintf("TelDlg hide\n");
	m_MJPGList.Invalidate();
	SetTimer(IDC_TELDLGHIDE, 10, NULL);
	
	main->phone_->PhoneDialTone(FALSE, NULL);
	
	//是软件挂断，手柄没放下,状态不清,其他情况下清下
	if (!(m_bSoftWareOff && main->phone_->Hand()))
	{
		main->phone_->Hand(0);
		main->phone_->Free(0);
	}
	m_bSoftWareOff = false ;
	Telephone::TelephoneWarp::GetTelephoneWarp()->PSTNHangoff();

	CString icon ;
	main->m_MJPGList.SetUnitIsShow(15,false,true);
	main->m_MJPGList.Invalidate();

	main->m_pTelphoneDialDlg->HangOff_();
	main->m_pTelphoneDialDlg->m_p10ContactInfoDlg->ShowWindow(SW_HIDE);
	
	main->m_pTelphoneRingDlg->HideContact(100,false);
	main->m_pTelphoneRingDlg->HideContact(200,false);
	
	if (main->m_pTelphoneRingDlg->IsWindowVisible())
	{
		main->m_pTelphoneRingDlg->ShowWindow_(SW_HIDE);
		main->PopbackIcon();
	}
	
	if (IsWindowVisible())
	{
		ShowWindow_(FALSE);
		main->PopbackIcon();	
	}

	if (main->m_pTelphoneDialDlg->IsWindowVisible())
	{
		main->m_pTelphoneDialDlg->ShowWindow_(SW_HIDE);
		main->PopbackIcon();	
			
	}
	//wangzhenxing20100629
	main->IsSendMessage(TRUE);

	KillTimer(SHOW_10CONTACTINFO_TIMER);//
	main->m_pTelphoneDialDlg->KillTimer(IDT_AUTO_DIAL);
	
	main->m_pTelphoneRingDlg->ClearData(100);
	main->m_pTelphoneRingDlg->ClearData(200);
	main->m_pTelphoneRingDlg->m_MJPGList.SetUnitText(6,L"",true);
	main->m_pTelphoneRingDlg->m_MJPGList.SetUnitText(7,L"",true);
	Dprintf("Hangoff over\r\n");
}

void CTelephoneDlg::CommonDataInit()
{
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	
	main->phone_->Hangup();
	
	KillTimer(CHECK_0X82_TIMER);
	m_nTelStatus = -1;
	m_bFirstConnect = TRUE;
	m_strTelStatus = "";
	KillTimer(IDT_TEL_STATUS);
	KillTimer(IDT_TEL_EXIT);
	
	KillTimer(5);
	KillTimer(IDC_TELDLGSHOW);
	
	main->phone_->PhoneDialTone(FALSE, NULL);//

	main->phone_->StopRing(true);
	waveInMessage(0, WAV_SPEAKER_ONLY, 0, 0); //20090324
	
	KillTimer(m_uiTipTimer);
	KillTimer(m_uiPstnTelephoneTimer);
	
	if (m_bRecording)
	{
		OnButtonTelephoneRecord();
	}
	
	m_bPlayingLeaveWord = FALSE;
	m_sTipFilename = "";
	m_bSend0x82Win = false;
	
	m_MJPGList.SetUnitText(100, "", FALSE);
	m_MJPGList.SetUnitFont(100, font_30);

	m_MJPGList.SetUnitText(101, "", FALSE);
	m_MJPGList.SetUnitText(102, "", FALSE);
	m_MJPGList.SetUnitText(103, "", FALSE);
	
	m_MJPGList.SetUnitIsShow(105,false,true);
	m_MJPGList.SetUnitText(107, L"", FALSE);
	m_MJPGList.SetUnitText(400,L"",true);

	main->m_pTelphoneRingDlg->RightUnitShow(false);
	
	Dprintf("CommonDataInit\n");
	
	m_MJPGList.Invalidate();
	
	
//	add by qi 20100607
//	extern HANDLE ExpThread;
//	CloseHandle(ExpThread);
	
	main->phone_->Hand(0);
	main->phone_->Free(0);
	
	Telephone::TelephoneWarp::GetTelephoneWarp()->PSTNHangoff();
	main->phone_->PSTNRingMute(false);//恢复

	CString icon ;
	main->m_MJPGList.SetUnitIsShow(15,false,true);
	main->m_MJPGList.Invalidate();
	
	main->m_pTelphoneDialDlg->HangOff_();
	main->m_pTelphoneDialDlg->m_p10ContactInfoDlg->ShowWindow(SW_HIDE);
	
	main->m_pTelephoneDlg->m_pTelePhoneOptionDlg->ShowWindow_(SW_HIDE);
	
	main->m_pTelphoneRingDlg->HideContact(100,false);
	main->m_pTelphoneRingDlg->HideContact(200,false);
	
	if (main->m_pTelphoneRingDlg->IsWindowVisible())
	{
		main->m_pTelphoneRingDlg->ShowWindow_(SW_HIDE);
		main->PopbackIcon();
	}
	
	//只剩一路，剩下的信息初始化
	for ( int i = 0 ; i < 3 ;i++)
	{
		m_MJPGList.SetUnitText(30+i,L"",true,true);
	}
	
	if (IsWindowVisible())
	{
		ShowWindow_(SW_HIDE);
		main->PopbackIcon(this);	
	}
	
	if (main->m_pTelphoneDialDlg->IsWindowVisible())
	{
		main->m_pTelphoneDialDlg->ShowWindow_(SW_HIDE);
		main->PopbackIcon(main->m_pTelphoneDialDlg);				

	}
	
	KillTimer(SHOW_10CONTACTINFO_TIMER);//
	main->m_pTelphoneDialDlg->KillTimer(IDT_AUTO_DIAL);
	
	main->m_nline = 0 ;
	
	main->m_pTelphoneRingDlg->ClearData(100);
//	main->m_pTelphoneRingDlg->ClearData(200);
	main->m_pTelphoneRingDlg->m_MJPGList.SetUnitText(6,L"",true);
	main->m_pTelphoneRingDlg->m_MJPGList.SetUnitText(7,L"",true);
	
	//wangzhenxing 20100629
	main->IsSendMessage(TRUE);//恢复视频
	if(g_isPhoneMute)
	{
		m_MJPGList.SetUnitIsDownStatus(50,false);
		g_isPhoneMute = FALSE ;
		for (int i = 0 ; i < 5; i++)
		{
			m_MJPGList.SetUnitIsShow(40+i,true,true);
			m_MJPGList.SetUnitIsDisable(40+i,false);
		}
		SetAudioLevel();
	}
	//change by qi 20100701

	extern void GNotifyDial(BOOL isDial);
	GNotifyDial(FALSE);

}

void CTelephoneDlg::HangOn_(void* param)
{	
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
// 	if(main->m_pSoundDlg->m_pRecordSoundDlg->IsWindowVisible())
// 		main->m_pSoundDlg->m_pRecordSoundDlg->CloseSound();
// 	if(main->m_pSoundDlg->m_pPlaySoundDlg->IsWindowVisible())
// 		main->m_pSoundDlg->m_pPlaySoundDlg->CloseSound();
	
	extern VOID WriteMyLog_(char *ptr, int size);
	WriteMyLog_("HangOn_",strlen("HangOn_"));
	
	Dprintf("TelDlg show1\n");
		
	SipShowIM(SIPF_OFF);

//  change by qi 20100519
//	SetTimer(IDC_TELDLGSHOW, 10, NULL);
	
	//change  by qi 20100519
	if(main->GetPhoneHungOn())
	{
		if (!((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pTelphoneDialDlg->IsWindowVisible())
		{				
			//加载图标			
			CWnd *p = main->m_pTelphoneDialDlg ;
			main->AddDesktopBtn();
			main->AddIcon(Allicon[0],p,false);			
			main->m_pTelphoneDialDlg->ShowWindow_(SW_SHOW);			
			main->m_pTelphoneDialDlg->m_bMayReturn = true ;	
			
		}		
	}
	
	//lxz 20100108
	extern void GMute(BOOL isOn);
	if(main->phone_->Hand())
	{
		GMute(TRUE);
	}

//  是软件启动还是硬件启动
//	if (!main->m_pTelphoneDialDlg->m_bSoftware)
	{
		Dprintf("dialing\r\n");
		main->phone_->PhoneDialTone(TRUE, "dialing");
	}
	WriteMyLog_("dialing",strlen("dialing"));

	main->m_pTelphoneDialDlg->m_bSoftware = false ;

}	


void CTelephoneDlg::Key_(void* param)
{
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;

	if(param == NULL)
		return;
	BOOL flag = FALSE;
	if (IsWindowVisible())
	{
		flag = TRUE;
	}

	CString number;
	char code[2] = {0};
	code[0] = char(param);
	((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->PhoneDialTone(TRUE, code);

	number = m_MJPGList.GetUnitText(100);
	number += char(param);
	SetFirstItem(number,flag);

	if(!m_MJPGList.GetUnitIsShow(19))
		m_MJPGList.SetUnitIsShow(19, TRUE, flag);  //拨打

 	if (!m_spContactInfo[main->m_nline])
 	{
 		m_spContactInfo[main->m_nline] = boost::shared_ptr<Data::ContactInfo>(new Data::ContactInfo);
		m_spContactInfo[main->m_nline]->type(Data::citOut);
		m_spContactInfo[main->m_nline]->startTime(CTime::GetCurrentTime());
		m_spContactInfo[main->m_nline]->Insert();
	}
	m_spContactInfo[main->m_nline]->telephoneNumber(Util::StringOp::FromCString(number));
	m_sTelephoneNumber = Util::StringOp::FromCString(number);
	

//	if(m_nTelStatus == TEL_FROMCONNECTED) //正在通话
	if(main->m_phoneLine[main->m_nline].pFSM->getCurrentState() 
		== CMultimediaPhoneDlg::p3gsConnected)
	{
		char code[2] = {0};
		code[0] = char(param);
    	((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->SubDial(char(param));
		((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->PhoneDialTone(TRUE, code);
		return;
	}

	KillTimer(IDT_AUTO_DIAL);
	SetTimer(IDT_AUTO_DIAL, 8000, NULL);

}

void CTelephoneDlg::SubKey_(void* param)
{
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;

	BOOL flag = FALSE;
	if (IsWindowVisible())
	{
		flag = TRUE;
	}

	CString number;
	number = m_MJPGList.GetUnitText(100);
	number += char(param);
	SetFirstItem(number,flag);
		
	//	if(m_nTelStatus == TEL_FROMCONNECTED) //正在通话
	if(main->m_phoneLine[main->m_nline].pFSM->getCurrentState() 
		== CMultimediaPhoneDlg::p3gsConnected)
	{
		char code[2] = {0};
		code[0] = char(param);
		
		if (main->phone_->Hand())
		{
			waveInMessage(0, 0x10000002,0,0);//lineout
		}
		else
		{
			waveInMessage(0, 0x10000001,0,0);//speeker
		}
		//拨号音
		extern void GPlayDialTone(char *dialtone);
		GPlayDialTone(code);
		//
		
		main->phone_->SubDial(char(param));
		if (!main->phone_->Hand())
		{
			extern void GMute(BOOL isOn);
			GMute(FALSE);					
		}
	
	}

}

void CTelephoneDlg::Connect_(void* param)
{	
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	int line = (int)param;
	Dprintf("Current_line: %d\r\n",line);
	Dprintf("3g_Connect \r\n");
	
	main->m_pTelphoneDialDlg->m_bSetGainLevel = false;

	if (m_spContactInfo[line])
	{
		if (m_spContactInfo[line]->type() != Data::citOut )
		{	
			int g3num = 0;
			main->Find3gLineNum(g3num);
			if (g3num == 1)
			{
				main->phone_->HungOn(TRUE);
			}
		}
	}
	
	main->phone_->StopRing(true);
	extern void GMute(BOOL isOn);
	if (!main->phone_->Hand())
	{
		GMute(FALSE);
	}

	m_MJPGList.SetUnitText(107, "", FALSE);
	
	if (m_spContactInfo[line])
	{
		if (m_spContactInfo[line]->type() == Data::citOut)
		{
			m_spContactInfo[line]->telephoneNumber(m_sTelephoneNumber);
			m_spContactInfo[line]->startTime(CTime::GetCurrentTime());
			/*if (m_sTelephoneNumber.size() >= 3)
			{
			//	std::string tel = main->m_phoneLine[line].telnum ;
			//	std::vector<boost::shared_ptr<Data::Contact> > result = FindCallContact(tel);
				std::vector<boost::shared_ptr<Data::Contact> > result = FindCallContact(m_sTelephoneNumber);
				if (!result.empty())
				{
			//		ShowContact(result[0], tel);
					ShowContact(result[0], m_sTelephoneNumber);
				}
			}*/
		}
		else
		{	
			if (!IsWindowVisible())
			{						
				main->m_phoneLine[line].b3gFisrtRing = false;
				main->m_pTelphoneRingDlg->ShowWindow_(SW_HIDE);
				main->PopbackIcon();
				std::string type = "(3G来电)";
				//m_MJPGList.SetUnitText(100, Util::StringOp::ToCString(m_sTel+m_sCity+type), TRUE);
				SetFirstItem(Util::StringOp::ToCString(m_sTel+m_sCity+type),true);
				std::vector<boost::shared_ptr<Data::Contact> > result = FindCallContact(m_sTel);
				if (!result.empty())
				{
					ShowContact(result[0], m_sTel);
				}
				
				main->m_pTelphoneRingDlg->ClearData(100);					
				main->AddIcon(Allicon[0],this,false);
				ShowWindow_(SW_SHOW);
				
			}
			else if ((main->m_phoneLine[(line+1)%2].pFSM->getCurrentState() == CMultimediaPhoneDlg::p3gsConnected ||
				main->m_phoneLine[(line+1)%2].pFSM->getCurrentState() == CMultimediaPhoneDlg::p3gsHold)&& 
				main->m_phoneLine[line].pFSM->getCurrentState() == CMultimediaPhoneDlg::p3gsConnected)//
			{
				Dprintf("contect new line\r\n");

				main->m_pTelphoneRingDlg->ShowWindow_(SW_HIDE);
				main->PopbackIcon();
				std::string type = "(3G来电)";
				SetFirstItem(Util::StringOp::ToCString(m_sTel+m_sCity+type),true);
			//	m_MJPGList.SetUnitText(100, Util::StringOp::ToCString(m_sTel+m_sCity+type), TRUE);
				std::vector<boost::shared_ptr<Data::Contact> > result = FindCallContact(m_sTel);
				if (!result.empty())
				{
					ShowContact(result[0], m_sTel);
				}
				
				//线路切换
				main->m_pATCommandWarp1->CHLD(2);
			//	main->m_nline = line;
				
				main->m_pTelphoneRingDlg->ClearData(100);
			}

			m_spContactInfo[line]->type(Data::citInOk);

		}
	}

	//这里让PSTN铃声静音
	main->phone_->PSTNRingMute();
		
	main->m_phoneLine[line].timer = line +200;//先设置线路
	SetTimer(main->m_phoneLine[line].timer, 1000, 0);//lxz test
	CTimeSpan ts;
	if (m_spContactInfo[line])
	{
		ts = CTime::GetCurrentTime() - m_spContactInfo[line]->startTime();
		main->m_phoneLine[line].SecondOff = ts.GetTotalSeconds();
	}


}

void CTelephoneDlg::Ring_(void* param)
{	
	CMultimediaPhoneDlg * main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	
	 // add by qi 
	 Dprintf("telephoneDlg Ring %d\r\n",(int)(param));

	 int line = (int )param ;

 	 if ( !m_bPstnFirstRing && !(main->m_phoneLine[line].b3gFisrtRing))//检查第一次振铃
 	 {
 		main->m_phoneLine[line].b3gFisrtRing = true;	
 	 }

	 if (main->m_phoneLine[line].b3gReject)//来电振铃屏蔽
	 {
		return ;		
	 }
	
	 //
	 main->m_pTelphoneRingDlg->m_MJPGList.SetUnitIsShow(3,true,true);

	m_bTelUsing = TRUE;
//	main->m_pSoundDlg->m_pRecordSoundDlg->CloseSound();
//	main->m_pSoundDlg->m_pPlaySoundDlg->OnButtonClose();
	
	extern BOOL _test_call;
	if(_test_call)
	{
		HandleOn();
		Sleep(1000);
		OnButtonTelephoneRecord();
		SetTimer(10, 30000, NULL);

	}
	
//	m_bRing = TRUE;
	if (!m_spContactInfo[line])
	{
		m_spContactInfo[line] = boost::shared_ptr<Data::ContactInfo>(new Data::ContactInfo);
		m_spContactInfo[line]->startTime(CTime::GetCurrentTime());
		m_spContactInfo[line]->type(Data::citInNo);
		m_spContactInfo[line]->Insert();
	}	
	
	if (!main->m_pTelphoneRingDlg->IsWindowVisible() &&
		( main->m_phoneLine[line].RingCount == 0 ))
	{
		//恢复黑屏 lxz 20090304
		main->ReStoreBackLight();
						
		SipShowIM(SIPF_OFF);
				
		//add by qi 2009_11_05
		main->m_pTelphoneRingDlg->ShowWindow_(SW_SHOW);
		
		CWnd *p = main->m_pTelphoneRingDlg;
		main->AddIcon(Allicon[0],p,false);

//		KillTimer(IDC_TELDLGHIDE);
		
		main->m_pMainDlg->m_mainMp3Dlg_->OnTimer(1002);//重复发送了暂停消息
		main->m_pTelphoneRingDlg->InitData();

	}
	
	if (!(main->m_phoneLine[line].b3gFisrtRing))//pstn先来电,后来的电显示在右边
	{
		main->m_pTelphoneRingDlg->RightUnitShow(true);
		main->m_pTelphoneRingDlg->m_MJPGList.SetUnitText(300,L"(3G来电)",false);
	}
	else 
	{	
		CString type = L"(3G来电)" ;
		CString number = L"(" + Util::StringOp::ToCString(main->m_phoneLine[line].telnum) +L")";
		main->m_pTelphoneRingDlg->m_MJPGList.SetUnitText(6,number+type,false);
	}
	
	Dprintf("Ring bFirwall\r\n");
	if ( main->m_phoneLine[line].bFirwall )
	{
		return;
	}

//	++m_uiRingCount;
	main->m_phoneLine[line].RingCount++;

	if ((!main->m_phoneLine[line].bHasCallID) && 
		(main->m_phoneLine[line].RingCount ==
		(main->m_phoneLine[line].iIgnoreRingCount + 1)))
	{
		if (main->m_pSettingDlg->m_pSetting->isFirewall())
		{
			int firetype = main->m_pSettingDlg->m_pSetting->firewallType();
			if (firetype == 1 || firetype == 2 || firetype == 3)
			{
				main->m_phoneLine[line].bFirwall  = true;
				return;
			}			
		}			
		
		std::string ringname;
		if (main->m_pSettingDlg->m_pSetting->isPhoneCallRing_)
		{
			ringname = main->m_pSettingDlg->m_pSetting->phoneCallRingFilename_;
		}
		else//没有设置就用缺省的
		{
			ringname = main->m_pSettingDlg->m_pSetting->defaultRingFilename();
		}

		CFileStatus status;
		if (ringname != "/flashdrv/my_ring/" && CFile::GetStatus(LPCTSTR(Util::StringOp::ToCString(ringname)),status))
		{
			//m_sRingFilename = ringname;
			main->m_phoneLine[line].RingFilename = ringname ;
		}

	}

	if (!main->m_phoneLine[line].bRingPlayed)
	{
		if (main->m_phoneLine[line].bHasCallID || 
			(main->m_phoneLine[line].RingCount >
			main->m_phoneLine[line].iIgnoreRingCount))
		{
			main->m_phoneLine[line].bRingPlayed = TRUE;
			
			//有活动的路就不想铃声
			if (main->m_phoneLine[(line+1)%2].pFSM->getCurrentState() <= CMultimediaPhoneDlg::p3gsDialing &&
				main->m_phoneLine[2].pFSM->getCurrentState() == CMultimediaPhoneDlg::pstnsHangOff)
			{
				if (main->m_phoneLine[line].RingFilename != "")
				{					
					main->SMSSpecRing(2, main->m_phoneLine[line].RingFilename.c_str());				
				}
				else
				{
					main->phone_->OpenTelRing();
				}			
			}
				
		}
	}
}

void CTelephoneDlg::pstnRing_(void* param)
{
// 	((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSoundDlg->m_pRecordSoundDlg->CloseSound();
// 	((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSoundDlg->m_pPlaySoundDlg->OnButtonClose();
	CMultimediaPhoneDlg * main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	
	if ((main->m_phoneLine[0].pFSM->getCurrentState() == CMultimediaPhoneDlg::p3gsConnected ||
		main->m_phoneLine[0].pFSM->getCurrentState() == CMultimediaPhoneDlg::p3gsHold)&&
		(main->m_phoneLine[1].pFSM->getCurrentState() == CMultimediaPhoneDlg::p3gsConnected||
		main->m_phoneLine[1].pFSM->getCurrentState() == CMultimediaPhoneDlg::p3gsHold))
	{
		return;//已经有两路了，再来PSTN不接
	}

	if (m_bPstnReject)
	{
		return ;
	}
	
	int line = 0;
	for (int i = 0 ;i < 2;i++)//查找3G两路有没有振铃
	{
		if (main->m_phoneLine[i].pFSM->getCurrentState() == CMultimediaPhoneDlg::p3gsRing)
		{
			line = i;
			break;
		}
	}
	
	if (!m_bPstnFirstRing && !(main->m_phoneLine[line].b3gFisrtRing))//记录谁先第一次振铃
	{
		m_bPstnFirstRing = TRUE;
	}

	//在只有PSTN是振铃，而3G路不是活动的，才把拒接按钮去掉
	if (main->m_phoneLine[main->m_nline].pFSM->getCurrentState() == CMultimediaPhoneDlg::p3gsRing ||
		main->m_phoneLine[main->m_nline].pFSM->getCurrentState() == CMultimediaPhoneDlg::p3gsConnected ||
		main->m_phoneLine[main->m_nline].pFSM->getCurrentState() == CMultimediaPhoneDlg::p3gsHold)
	{
		main->m_pTelphoneRingDlg->m_MJPGList.SetUnitIsShow(3,true,true);	
	}
	else
	{
		main->m_pTelphoneRingDlg->m_MJPGList.SetUnitIsShow(3,false,true);	
	}
	//

	m_bPstnRing = TRUE ;

	if (!m_spPstnContactInfo)
	{
		m_spPstnContactInfo = boost::shared_ptr<Data::ContactInfo>(new Data::ContactInfo);
		m_spPstnContactInfo->startTime(CTime::GetCurrentTime());
		m_spPstnContactInfo->type(Data::citInNo);
		m_spPstnContactInfo->Insert();
	}	
	
	if (!main->m_pTelphoneRingDlg->IsWindowVisible() && (m_uiPstnRingCount == 0))
	{
		//恢复黑屏 lxz 20090304
		((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->ReStoreBackLight();						
		SipShowIM(SIPF_OFF);				
		main->m_pTelphoneRingDlg->ShowWindow_(SW_SHOW);
		
		CWnd *p = main->m_pTelphoneRingDlg;
		main->AddIcon(Allicon[0],p,false);

//		KillTimer(IDC_TELDLGHIDE);
		main->m_pMainDlg->m_mainMp3Dlg_->OnTimer(1002);//重复发送了暂停消息
		main->m_pTelphoneRingDlg->InitData();
	}
	
	if (m_bPstnFirstRing)
	{	
		CString type = L"(PSTN来电)" ;
		CString number = L"(" + Util::StringOp::ToCString(m_sPstnTel) +L")";
		main->m_pTelphoneRingDlg->m_MJPGList.SetUnitText(6,number+type,false);
	}
	else
	{	
		main->m_pTelphoneRingDlg->RightUnitShow(true);
		main->m_pTelphoneRingDlg->m_MJPGList.SetUnitText(300,L"(PSTN来电)",false);
	}
 	
	if (m_bPstnFirwall)
	{
		return;
	}

	++m_uiPstnRingCount;
	
	//没有callid
	if ((!m_bPstnHasCallID) && (m_uiPstnRingCount == (m_uiIPstngnoreRingCount + 1)))
	{
		if (main->m_pSettingDlg->m_pSetting->isFirewall())
		{
			int firetype = main->m_pSettingDlg->m_pSetting->firewallType();
			if (firetype == 1 || firetype == 2 || firetype == 3)
			{
				m_bPstnFirwall = TRUE;
				return;
			}			
		}			
		
		std::string ringname ;
		if (main->m_pSettingDlg->m_pSetting->isPhoneCallRing_)
		{
			ringname = main->m_pSettingDlg->m_pSetting->phoneCallRingFilename_;
		}
		else//没有设置就用缺省的
		{
			ringname = main->m_pSettingDlg->m_pSetting->defaultRingFilename();
		}
		
		CFileStatus   status;
		if (ringname != "/flashdrv/my_ring/" && CFile::GetStatus(LPCTSTR(Util::StringOp::ToCString(ringname)),status))
		{
			m_sPstnRingFilename = ringname;
		}
	}

	if (!m_bPstnRingPlayed)
	{
		if (m_bPstnHasCallID || (m_uiPstnRingCount > m_uiIPstngnoreRingCount))
		{
			m_bPstnRingPlayed = TRUE;
		
			if (m_sPstnRingFilename != "")
			{					
			  // main->SMSSpecRing(2, m_sRingFilename.c_str());				
			}
			else
			{
				if (main->m_phoneLine[main->m_nline].pFSM->getCurrentState() <= CMultimediaPhoneDlg::p3gsDialing)
				{
					main->phone_->OpenTelRing();
				}				
			}
			
		}

	}
}

void CTelephoneDlg::CallID_(void* param)
{	
	CMultimediaPhoneDlg * main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	
	m_bTelUsing = TRUE;
// 	main->m_pSoundDlg->m_pRecordSoundDlg->CloseSound();
// 	main->m_pSoundDlg->m_pPlaySoundDlg->OnButtonClose();
	
	CALLID_INFO* info = (CALLID_INFO*)param;
	int line = info->Line -1;//线路
	
	if ( !m_bPstnFirstRing && !(main->m_phoneLine[line].b3gFisrtRing))//检查第一次振铃
	{
		main->m_phoneLine[line].b3gFisrtRing = true;	
	}

	//把拒接的图标显示出来
	main->m_pTelphoneRingDlg->m_MJPGList.SetUnitIsShow(3,true,true);

	//第二路来电发"嘟嘟"的提示音
	if (main->m_phoneLine[(line+1)%2].pFSM->getCurrentState() >= CMultimediaPhoneDlg::p3gsConnected)
	{	
		if (main->phone_->Hand())
		{
			waveInMessage(0, 0x10000002,0,0);//lineout
		}

		//播放嘟嘟的声音
		TCHAR filename[] = L"\\flashdrv\\单音.mp3" ;
		main->phone_->StartRing(filename,1);
	}
	
	//来callID
	if(main->m_phoneLine[line].bHasCallID)
	{
		HangOff_(NULL);
	}
	
	main->m_phoneLine[line].bHasCallID = TRUE;
//	m_bRing = TRUE;
	
	if (!m_spContactInfo[line])
	{
		m_spContactInfo[line] = boost::shared_ptr<Data::ContactInfo>(new Data::ContactInfo);
		m_spContactInfo[line]->startTime(CTime::GetCurrentTime());
		m_spContactInfo[line]->type(Data::citInNo);
		m_spContactInfo[line]->Insert();
	}
	
	std::string tel = info->number;
	Dprintf("Call ID telephone number:\n");
	Dprintf(tel.c_str());
	std::string mobile;
	std::string city;
	std::string code;

	if (main->m_pSettingDlg->m_pSetting->thisTelephoneNumber().number().length() >= 2)
	{
		code = main->m_pSettingDlg->m_pSetting->thisTelephoneNumber().number();
		if (code[0] != '0')
		{
			code = "0" + code;
		}
		
		mobile = tel;
		if (tel[0] == '0')
		{
			int codeLen = code.length();
			if (tel.substr(0, codeLen) == code)
			{
				tel = tel.substr(codeLen);
			}
			else
			{
				mobile = tel.substr(1);
			}
		}
		mobile = mobile.substr(0, 7);
	}
	else
	{
		mobile = tel.substr(0, 7);
	}
		

	if (!mobile.empty() && (mobile[0] == '1'))
	{
		FILE* file = fopen("\\hive\\res\\mobile.txt" , "rb");
		if (file != NULL)
		{
			std::string addr = StartSearch(file, mobile, 0, 93935);
			if (addr != "")
			{
				if (addr.substr(23, code.length()) == code)
				{
					if (tel[0] == '0')
					{
						tel = tel.substr(1);
					}
				}
				city = addr.substr(8, 14);
			}
			fclose(file);
		}
	}
	
	main->m_phoneLine[line].telnum = tel;
	main->m_phoneLine[line].city = city;

//	std::string type = "(3G来电)";
//	m_MJPGList.SetUnitText(100, Util::StringOp::ToCString(tel+city+type), TRUE);
	m_sTel	= tel ;
	m_sCity	= city;
	
	//
	if (main->m_phoneLine[line].b3gFisrtRing)
	{
	/*	if (tel.substr(0,1) == "1")
		{	
			main->m_pTelphoneRingDlg->m_MJPGList.SetUnitText(101, L"", TRUE);
			main->m_pTelphoneRingDlg->
				m_MJPGList.SetUnitText(101, Util::StringOp::ToCString(tel+city), TRUE);
			
		}
		else
		{
			main->m_pTelphoneRingDlg->
				m_MJPGList.SetUnitText(102, Util::StringOp::ToCString(tel+city), TRUE);
			
		}*/
	}
	else
	{
		main->m_pTelphoneRingDlg->
			m_MJPGList.SetUnitText(301, Util::StringOp::ToCString(tel+city), TRUE);		
	}
	
	if (tel == "")
	{
		if ( main->m_pSettingDlg->m_pSetting->isFirewall())
		{
			int firetype = main->m_pSettingDlg->m_pSetting->firewallType();
			if (firetype == 1 || firetype == 2 || firetype == 3)
			{
				main->m_phoneLine[line].bFirwall = true;
			}
		}

	}
	else
	{	
		std::vector<boost::shared_ptr<Data::Contact> > result = FindCallContact(tel);
		if (!result.empty())
		{
			//ShowContact(result[0], tel);
			
			//add by qi 2009_11_04
			if (main->m_phoneLine[line].b3gFisrtRing)
			{
				main->m_pTelphoneRingDlg->ShowContact(result[0], tel);
			}

			if (main->m_pSettingDlg->m_pSetting->isFirewall())
			{
				int firetype = main->m_pSettingDlg->m_pSetting->firewallType();
				Data::ContactType type = result[0]->type();
				
				if (((firetype == 0) && (type == Data::ctBlacklist))
					|| ((firetype == 1) && (type == Data::ctBlacklist))
					|| ((firetype == 2) && (type != Data::ctVip))
					|| (firetype == 3))
				{
					main->m_phoneLine[line].bFirwall = true;
				}


			}
			
			if (!main->m_phoneLine[line].bFirwall)
			{
				if (result[0]->group())
				{
					std::string tipname = result[0]->group()->tipName();
					if (tipname == Data::LanguageResource::Get(Data::RI_CARD_DEFAULT))
					{
						tipname = main->m_pSettingDlg->m_pSetting->defaultTipFilename();
					}
					
					CFileStatus   status;
					if (CFile::GetStatus(LPCTSTR(Util::StringOp::ToCString(tipname)),status))
					{
						m_sTipFilename = tipname;
					}
					
					std::string ringname = result[0]->group()->ringName();
					if (ringname == Data::LanguageResource::Get(Data::RI_CARD_DEFAULT))
					{
						ringname = main->m_pSettingDlg->m_pSetting->phoneCallRingFilename_;
					}
					
					if (ringname != "/flashdrv/my_ring/" && CFile::GetStatus(LPCTSTR(Util::StringOp::ToCString(ringname)),status))
					{
						//	m_sRingFilename = ringname;
						main->m_phoneLine[line].RingFilename = ringname ;
					}
				}
				else//lxz 20090330
				{
					std::string ringname = "";
					CFileStatus   status;
					if (main->m_pSettingDlg->m_pSetting->isPhoneCallRing_)
					{
						ringname = main->m_pSettingDlg->m_pSetting->phoneCallRingFilename_;
					}
					else//没有设置就用缺省的
					{
						ringname = main->m_pSettingDlg->m_pSetting->defaultRingFilename();
					}

					if (ringname != "/flashdrv/my_ring/" && CFile::GetStatus(LPCTSTR(Util::StringOp::ToCString(ringname)), status))
					{
						// m_sRingFilename = ringname;
						main->m_phoneLine[line].RingFilename = ringname ;
					}

				}
			}
			
		}
		else
		{
			if (main->m_pSettingDlg->m_pSetting->isFirewall())
			{
				int firetype = main->m_pSettingDlg->m_pSetting->firewallType();
				if (firetype == 1 || firetype == 2 || firetype == 3)
				{
					main->m_phoneLine[line].bFirwall = true;
				}
			}

			//lxz 20090330
			std::string  ringname = "";
			CFileStatus  status;
			if (main->m_pSettingDlg->m_pSetting->isPhoneCallRing_)
			{
				ringname = main->m_pSettingDlg->m_pSetting->phoneCallRingFilename_;

			}
			else//没有设置就用缺省的
			{
				ringname = main->m_pSettingDlg->m_pSetting->defaultRingFilename();
			}
			
			if (ringname != "/flashdrv/my_ring/" && CFile::GetStatus(LPCTSTR(Util::StringOp::ToCString(ringname)), status))
			{
				//	m_sRingFilename = ringname;
				main->m_phoneLine[line].RingFilename = ringname ;
			}

		}
	}
	
	if (m_spContactInfo[line])
	{
		m_spContactInfo[line]->telephoneNumber(Data::TelephoneNumber(tel));
		m_spContactInfo[line]->Update();
	}

	if (main->m_phoneLine[line].RingCount > 0)
	{
		Dprintf("RingCount \r\n");

		main->m_phoneLine[line].pFSM->
			fireEvent(CMultimediaPhoneDlg::p3geRing, (void *)line);
		main->m_phoneLine[line].iIgnoreRingCount = 2;

	}

}

void CTelephoneDlg::pstnCallID_(void* param)
{	
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	m_bTelUsing = TRUE;
//	((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSoundDlg->m_pRecordSoundDlg->CloseSound();
//	((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSoundDlg->m_pPlaySoundDlg->OnButtonClose();
	
	int unitNo = 100;
// 	if (main->m_phoneLine[0].pFSM->getCurrentState() < CMultimediaPhoneDlg::p3gsDialing)
// 	{
// 		main->m_pTelphoneRingDlg->HideContact(100);
// 	}
	
	if ((main->m_phoneLine[0].pFSM->getCurrentState() == CMultimediaPhoneDlg::p3gsConnected ||
		main->m_phoneLine[0].pFSM->getCurrentState() == CMultimediaPhoneDlg::p3gsHold)&&
		(main->m_phoneLine[1].pFSM->getCurrentState() == CMultimediaPhoneDlg::p3gsConnected||
		main->m_phoneLine[1].pFSM->getCurrentState() == CMultimediaPhoneDlg::p3gsHold))
	{
		return;//已经有两路了，再来PSTN不接
	}

	int line = 0;
	for (int i = 0 ;i < 2;i++)//查找3G两路有没有振铃
	{
		 if (main->m_phoneLine[i].pFSM->getCurrentState() == CMultimediaPhoneDlg::p3gsRing)
		 {
			line = i;
			break;
		 }
	}

	if (!m_bPstnFirstRing && !(main->m_phoneLine[line].b3gFisrtRing))//记录谁先第一次振铃
	{
		m_bPstnFirstRing = TRUE;
	}

	//在只有PSTN是振铃，而3G路不是活动的，才把拒接按钮去掉
	if (main->m_phoneLine[main->m_nline].pFSM->getCurrentState() == CMultimediaPhoneDlg::p3gsRing ||
		main->m_phoneLine[main->m_nline].pFSM->getCurrentState() == CMultimediaPhoneDlg::p3gsConnected ||
		main->m_phoneLine[main->m_nline].pFSM->getCurrentState() == CMultimediaPhoneDlg::p3gsHold)
	{
		main->m_pTelphoneRingDlg->m_MJPGList.SetUnitIsShow(3,true,true);	
	}
	else
	{
		main->m_pTelphoneRingDlg->m_MJPGList.SetUnitIsShow(3,false,true);	
	}
	//

	//来callID
	if(m_bPstnHasCallID)
	{
	//	HangOff_(NULL);
		CommonDataInit();
	}
	
	m_bPstnHasCallID = TRUE;
	m_bPstnRing = TRUE ;
	
	if (!m_spPstnContactInfo)
	{
		m_spPstnContactInfo = boost::shared_ptr<Data::ContactInfo>(new Data::ContactInfo);
		m_spPstnContactInfo->startTime(CTime::GetCurrentTime());
		m_spPstnContactInfo->type(Data::citInNo);
		m_spPstnContactInfo->Insert();
	}
	
	CALLID_INFO* info = (CALLID_INFO*)param;
	std::string tel = info->number;
	Dprintf("PSTN Call ID telephone number:\n");
	Dprintf(tel.c_str());
	std::string mobile;
	std::string city;
	std::string code;
	if (main->m_pSettingDlg->m_pSetting->thisTelephoneNumber().number().length() >= 2)
	{
		code = main->m_pSettingDlg->m_pSetting->thisTelephoneNumber().number();
		if (code[0] != '0')
		{
			code = "0" + code;
		}
		
		mobile = tel;
		if (tel[0] == '0')
		{
			int codeLen = code.length();
			if (tel.substr(0, codeLen) == code)
			{
				tel = tel.substr(codeLen);
			}
			else
			{
				mobile = tel.substr(1);
			}
		}
		mobile = mobile.substr(0, 7);
	}
	else
	{
		mobile = tel.substr(0, 7);
	}
		

	if (!mobile.empty() && (mobile[0] == '1'))
	{
		FILE* file = fopen("\\hive\\res\\mobile.txt" , "rb");
		if (file != NULL)
		{
			std::string addr = StartSearch(file, mobile, 0, 93935);
			if (addr != "")
			{
				if (addr.substr(23, code.length()) == code)
				{
					if (tel[0] == '0')
					{
						tel = tel.substr(1);
					}
				}
				city = addr.substr(8, 14);
			}
			fclose(file);
		}
	}
		
	m_sPstnTel	 = tel ;
	m_sPstnCity  = city ;

	if (m_sPstnTel.size() > 12)
	{
		m_sPstnTel = m_sPstnTel.substr(0,12);
	}
	
	bool bfresh = false;
	if (!m_bPstnFirstRing)
	{
		main->m_pTelphoneRingDlg->
			m_MJPGList.SetUnitText(301, Util::StringOp::ToCString(tel+city), bfresh);
	}
	
	if (tel == "")
	{
		if (main->m_pSettingDlg->m_pSetting->isFirewall())
		{
			int firetype = main->m_pSettingDlg->m_pSetting->firewallType();
			if (firetype == 0 || firetype == 2 || firetype == 3)
			{
				m_bPstnFirwall = TRUE;
			}
		}
	}
	else
	{	
		std::vector<boost::shared_ptr<Data::Contact> > result = FindCallContact(tel);
		if (!result.empty())
		{

			//add by qi 2009_11_04
			if (m_bPstnFirstRing)
			{
				main->m_pTelphoneRingDlg->ShowContact(result[0], tel,unitNo);
			}

			if (main->m_pSettingDlg->m_pSetting->isFirewall())
			{
				int firetype = main->m_pSettingDlg->m_pSetting->firewallType();
				Data::ContactType type = result[0]->type();
				
				if (((firetype == 0) && (type == Data::ctBlacklist))
					|| ((firetype == 1) && (type == Data::ctBlacklist))
					|| ((firetype == 2) && (type != Data::ctVip))
					|| (firetype == 3))
				{
					m_bPstnFirwall = TRUE;
				}
			}
			
			if (!m_bPstnFirwall)
			{
				if (result[0]->group())
				{
					std::string tipname = result[0]->group()->tipName();
					if (tipname == Data::LanguageResource::Get(Data::RI_CARD_DEFAULT))
					{
						tipname = main->m_pSettingDlg->m_pSetting->defaultTipFilename();
					}
					
					CFileStatus   status;
					if (CFile::GetStatus(LPCTSTR(Util::StringOp::ToCString(tipname)),status))
					{
						m_sTipFilename = tipname;
					}
					
					std::string ringname = result[0]->group()->ringName();
					if (ringname == Data::LanguageResource::Get(Data::RI_CARD_DEFAULT))
					{
						ringname = main->m_pSettingDlg->m_pSetting->defaultRingFilename();
					}
					
					if (ringname != "/flashdrv/my_ring/" && CFile::GetStatus(LPCTSTR(Util::StringOp::ToCString(ringname)),status))
					{
						m_sPstnRingFilename = ringname;
					}

				}
				else //lxz 20090330
				{
					std::string ringname = "";
					CFileStatus   status;
					if (main->m_pSettingDlg->m_pSetting->isPhoneCallRing_)
					{
						ringname = main->m_pSettingDlg->m_pSetting->phoneCallRingFilename_;
					}
					else//没有设置就用缺省的
					{
						ringname = main->m_pSettingDlg->m_pSetting->defaultRingFilename();
					}
					
					if (ringname != "/flashdrv/my_ring/" && CFile::GetStatus(LPCTSTR(Util::StringOp::ToCString(ringname)), status))
					{
						m_sPstnRingFilename = ringname;
					}
				}
			}
			
		}
		else
		{
			if (main->m_pSettingDlg->m_pSetting->isFirewall())
			{
				int firetype = main->m_pSettingDlg->m_pSetting->firewallType();
				if (firetype == 1 || firetype == 2 || firetype == 3)
				{
					m_bPstnFirwall = TRUE;
				}
			}

			//lxz 20090330
			std::string	  ringname = "";
			CFileStatus   status;
			if (main->m_pSettingDlg->m_pSetting->isPhoneCallRing_)
			{
				ringname = main->m_pSettingDlg->m_pSetting->phoneCallRingFilename_;
			}
			else//没有设置就用缺省的
			{
				ringname = main->m_pSettingDlg->m_pSetting->defaultRingFilename();
			}
			
			if (ringname != "/flashdrv/my_ring/" && CFile::GetStatus(LPCTSTR(Util::StringOp::ToCString(ringname)), status))
			{
				m_sPstnRingFilename = ringname;
			}
		}
	}

	m_spPstnContactInfo->telephoneNumber(Data::TelephoneNumber(tel));
	m_spPstnContactInfo->Update();

	if (m_uiPstnRingCount > 0)
	{
		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_phoneLine[2].pFSM->
			fireEvent(CMultimediaPhoneDlg::pstneRing, NULL);
		m_uiIPstngnoreRingCount = 2;

	}
	
}

void CTelephoneDlg::PstnContect_(void* param)
{
		
//	if (m_bPlayingLeaveWord)
//	{
	//	((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pFSM->setStartState(CMultimediaPhoneDlg::p3gsRing);
	//	return;
	//}
	
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd ;

	if (main->m_phoneLine[0].pFSM->getCurrentState() == CMultimediaPhoneDlg::p3gsConnected &&
		main->m_phoneLine[1].pFSM->getCurrentState() == CMultimediaPhoneDlg::p3gsConnected)
	{
		return;//已经有两路了，再来PSTN不接
	}

	main->phone_->StopRing(true);
	
	extern void GMute(BOOL isOn);
	if (!main->phone_->Hand())
	{
		GMute(FALSE);
	}
	
	m_MJPGList.SetUnitText(107, "", FALSE);
				
//	if (m_bPstnRing && !IsWindowVisible())
	{	
		m_bPstnFirstRing = false;
		main->m_pTelphoneRingDlg->ShowWindow_(SW_HIDE);
		main->PopbackIcon();
	
		std::string type = "(PSTN来电)";
		SetFirstItem(Util::StringOp::ToCString(m_sPstnTel+m_sPstnCity+type),true);
	//	m_MJPGList.SetUnitText(100, Util::StringOp::ToCString(m_sPstnTel+m_sPstnCity+type), TRUE);
		if (!m_sPstnTel.empty())
		{
			std::vector<boost::shared_ptr<Data::Contact> > result = FindCallContact(m_sPstnTel);
			if (!result.empty())
			{
				ShowContact(result[0], m_sPstnTel,200);
			}
		}
		main->AddIcon(Allicon[0],this,false);
		ShowWindow_(SW_SHOW);
		
	}
	main->m_pTelphoneRingDlg->ClearData(100);
	m_spPstnContactInfo->type(Data::citInOk);
	
	SetTimer(m_uiPstnTelephoneTimer, 1000, 0);
	CTimeSpan ts;
	ts = CTime::GetCurrentTime() - m_spPstnContactInfo->startTime();
	m_uPstnTelSecondOff = ts.GetTotalSeconds();

}

void CTelephoneDlg::PstnHangOff_(void* param)
{	
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	m_uiIPstngnoreRingCount = 1;
	KillTimer(m_uiPstnTelephoneTimer);

	if (m_spPstnContactInfo)
	{
		m_spPstnContactInfo->duration(m_uiPstnTelephoneSecond);
		if (m_spPstnContactInfo->type() == Data::citInNo)
		{
			m_spPstnContactInfo->played(false);
		}
		
		main->m_pContactInfoDlg->SaveContactInfo(m_spPstnContactInfo);
		
	/*	if (!m_vSoundSegment.empty())
		{
			for (std::vector<boost::shared_ptr<Data::SoundSegment> >::iterator iter = m_vSoundSegment.begin(); iter != m_vSoundSegment.end(); ++iter)
			{				
				(*iter)->telephoneNumber(m_spPstnContactInfo->telephoneNumber());
				(*iter)->name(m_spPstnContactInfo->name());
				(*iter)->contactInfoId(m_spPstnContactInfo->id());
				(*iter)->contactId(m_spPstnContactInfo->contactId());

				if (m_spPstnContactInfo->type() == Data::citInNo)
				{
					(*iter)->type(Data::sstLeaveWord);
					(*iter)->played(false);

					::SendMessage(main->m_pMainDlg->GetSafeHwnd(), WM_TELNOTIFY, 2, 1);
				}
				else
				{
					(*iter)->type(Data::sstTeleRecord);
					(*iter)->played(true);
				}
				
				main->m_pSoundDlg->SaveSoundSegment(*iter);
				
			}
		}
		m_vSoundSegment.clear();*/

		main->m_pContactInfoDlg->ResetTypeInfo();
		if (m_spPstnContactInfo->type() == Data::citInNo)
		{
			::SendMessage(main->m_pMainDlg->GetSafeHwnd(), WM_TELNOTIFY, 1, 1);
		}

	}

	m_spPstnContactInfo = boost::shared_ptr<Data::ContactInfo>();
	
	m_uiPstnTelephoneSecond = 0;
	m_uiPstnRingCount = 0;	
	m_uiIPstngnoreRingCount = 0;
	m_uPstnTelSecondOff = 0;

	m_bPstnRingPlayed = FALSE;
	m_bPstnFirwall = FALSE ;
	m_bPstnHasCallID = FALSE;
	m_bPstnReject = FALSE;
	m_bPstnFirstRing = FALSE;
	
	m_sPstnRingFilename ="";
	m_sPstnTel	= "";
	m_sPstnCity = "";
	
	main->m_phoneLine[2].pFSM->setStartState(CMultimediaPhoneDlg::pstnsHangOff);
	main->phone_->PSTNHangoff_();
	main->phone_->PSTNHangoff();
	//main->m_pTelphoneRingDlg->ClearData(100);

	if (!main->FindActiveLine())//没有活动线路，共有信息全部初始化
	{
		CommonDataInit();
	}
	else //线路切换
	{
		//只剩PSTN就不用切换，剩3G_1路的话需要切换
		for (int i = 0 ; i < 2 ; i++)
		{
			if (main->m_phoneLine[i].pFSM->getCurrentState() >= 
				CMultimediaPhoneDlg::p3gsDialing)
			{
				main->m_nline = i ;
				break;
			}
		}

		//Pstn自动挂断
		if (main->m_pTelphoneRingDlg->IsWindowVisible())
		{
			if (main->m_phoneLine[main->m_nline].pFSM->getCurrentState() == CMultimediaPhoneDlg::p3gsRing)//如果3G另一路是振铃状态
			{	
				main->m_phoneLine[main->m_nline].b3gFisrtRing = true ;
				main->m_pTelphoneRingDlg->ClearData(100);
				main->m_pTelphoneRingDlg->RightUnitShow(false);		
				Show3gInfo(main->m_nline);
			}
			else if (main->m_phoneLine[main->m_nline].pFSM->getCurrentState() == CMultimediaPhoneDlg::p3gsConnected)
			{
				//change by qi 20100609
				main->PopIcon(main->m_pTelphoneRingDlg);

// 				ShowWindow_(SW_HIDE);
// 				main->PopbackIcon();
			}
		}


	}

	Dprintf("Pstn HangOff\n");
		
}

void CTelephoneDlg::G3HangOff()
{
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;

	KillTimer(m_uiTipTimer);
	KillTimer(main->m_phoneLine[main->m_nline].timer);
	
	if (m_bRecording)
	{
		OnButtonTelephoneRecord();
	}

	if (m_spContactInfo[main->m_nline])
	{
		m_spContactInfo[main->m_nline]->duration(main->m_phoneLine[main->m_nline].Second);
		if (m_spContactInfo[main->m_nline]->type() == Data::citInNo)
		{
			m_spContactInfo[main->m_nline]->played(false);
		}
		
		main->m_pContactInfoDlg->SaveContactInfo(m_spContactInfo[main->m_nline]);
		
		if (!m_vSoundSegment.empty())
		{
			for (std::vector<boost::shared_ptr<Data::SoundSegment> >::iterator iter = m_vSoundSegment.begin(); iter != m_vSoundSegment.end(); ++iter)
			{				
				(*iter)->telephoneNumber(m_spContactInfo[main->m_nline]->telephoneNumber());
				(*iter)->name(m_spContactInfo[main->m_nline]->name());
				(*iter)->contactInfoId(m_spContactInfo[main->m_nline]->id());
				(*iter)->contactId(m_spContactInfo[main->m_nline]->contactId());
				
				if (m_spContactInfo[main->m_nline]->type() == Data::citInNo)
				{
					(*iter)->type(Data::sstLeaveWord);
					(*iter)->played(false);
					
					::SendMessage(((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pMainDlg->GetSafeHwnd(), WM_TELNOTIFY, 2, 1);
				}
				else
				{
					(*iter)->type(Data::sstTeleRecord);
					(*iter)->played(true);
				}
				
				((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSoundDlg->SaveSoundSegment(*iter);
				
			}

		}
		m_vSoundSegment.clear();
		
		main->m_pContactInfoDlg->ResetTypeInfo();
		if (m_spContactInfo[main->m_nline]->type() == Data::citInNo)
		{
			::SendMessage(((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pMainDlg->GetSafeHwnd(), WM_TELNOTIFY, 1, 1);
		}
		
	}
	m_spContactInfo[main->m_nline] = boost::shared_ptr<Data::ContactInfo>();
	
//	m_uiTelephoneSecond = 0;
	main->m_phoneLine[main->m_nline].Second = 0 ;
	main->m_phoneLine[main->m_nline].RingCount = 0 ;

	main->m_phoneLine[main->m_nline].bHasCallID  = false;
	main->m_phoneLine[main->m_nline].bFirwall	 = false;
	main->m_phoneLine[main->m_nline].bRingPlayed = false;
	
	main->m_phoneLine[main->m_nline].RingFilename = "" ;
	main->m_phoneLine[main->m_nline].iIgnoreRingCount = 1;

//	m_uiRingCount = 0;	
	
	m_bPlayingLeaveWord = FALSE;
	m_sTipFilename = "";
//	m_sRingFilename = "";
	m_bRecording = FALSE;
//	m_bRingPlayed = FALSE;
		
//	main->m_pTelphoneRingDlg->ClearData(100);
	m_MJPGList.Invalidate();
	
}

void CTelephoneDlg::G3HangOff(int const line)
{
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	
	KillTimer(main->m_phoneLine[line].timer);
	KillTimer(main->m_phoneLine[line].LineStatusTimer);

	Dprintf("G3HangeOff:%d\r\n",line);
	
// 	if (main->m_pTelephoneDlg->m_pTelePhoneOptionDlg->IsWindowVisible())
// 	{
// 		main->m_pTelephoneDlg->m_pTelePhoneOptionDlg->ShowWindow_(SW_HIDE);
// 	}

	if (m_spContactInfo[line])
	{
		m_spContactInfo[line]->duration(main->m_phoneLine[line].Second);
		if (m_spContactInfo[line]->type() == Data::citInNo)
		{
			m_spContactInfo[line]->played(false);
		}
		
		main->m_pContactInfoDlg->SaveContactInfo(m_spContactInfo[line]);
		
		//录音文件 暂时可以不要
	/*	if (!m_vSoundSegment.empty())
		{
			for (std::vector<boost::shared_ptr<Data::SoundSegment> >::iterator iter = m_vSoundSegment.begin(); iter != m_vSoundSegment.end(); ++iter)
			{				
				(*iter)->telephoneNumber(m_spContactInfo[line]->telephoneNumber());
				(*iter)->name(m_spContactInfo[line]->name());
				(*iter)->contactInfoId(m_spContactInfo[line]->id());
				(*iter)->contactId(m_spContactInfo[line]->contactId());
				
				if (m_spContactInfo[line]->type() == Data::citInNo)
				{
					(*iter)->type(Data::sstLeaveWord);
					(*iter)->played(false);
					
					::SendMessage(main->m_pMainDlg->GetSafeHwnd(), WM_TELNOTIFY, 2, 1);
				}
				else
				{
					(*iter)->type(Data::sstTeleRecord);
					(*iter)->played(true);
				}
				
				main->m_pSoundDlg->SaveSoundSegment(*iter);
				
			}
			
		}
		m_vSoundSegment.clear();*/
		
		main->m_pContactInfoDlg->ResetTypeInfo();
		if (m_spContactInfo[line]->type() == Data::citInNo)
		{
			::SendMessage(((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pMainDlg->GetSafeHwnd(), WM_TELNOTIFY, 1, 1);
		}
		
		if(m_spContactInfo[line]->type() != Data::citInNo && m_spContactInfo[line]->duration() > 0)
			::PostMessage(main->GetSafeHwnd(), WM_TELCALL_TIME, m_spContactInfo[line]->duration(), 0);
	}
	m_spContactInfo[line] = boost::shared_ptr<Data::ContactInfo>();
	
	main->m_phoneLine[line].Second = 0 ;
	main->m_phoneLine[line].RingCount = 0 ;
	
	main->m_phoneLine[line].bHasCallID  = false;
	main->m_phoneLine[line].bFirwall	= false;
	main->m_phoneLine[line].bRingPlayed = false;
	
	main->m_phoneLine[line].RingFilename = "" ;
	main->m_phoneLine[line].iIgnoreRingCount = 1;
	main->m_phoneLine[line].telnum = "";
	main->m_phoneLine[line].city = "";
	main->m_phoneLine[line].b3gReject = false;
	main->m_phoneLine[line].b3gFisrtRing = false;
	
	main->m_phoneLine[line].pFSM->setStartState(CMultimediaPhoneDlg::p3gsHangOff);
	
	m_MJPGList.Invalidate();

}

void CTelephoneDlg::G3AllHangOff()
{
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;

	Dprintf("G3 ALL HangeOff \r\n");

	for (int j = 0 ; j < 2;j++)
	{	
		int line = j ;
		KillTimer(main->m_phoneLine[line].LineStatusTimer);
		KillTimer(main->m_phoneLine[line].timer);		
		if (m_spContactInfo[line])
		{
			m_spContactInfo[line]->duration(main->m_phoneLine[line].Second);
			if (m_spContactInfo[line]->type() == Data::citInNo)
			{
				m_spContactInfo[line]->played(false);
			}
			
			main->m_pContactInfoDlg->SaveContactInfo(m_spContactInfo[line]);
			
			//录音文件 暂时可以不要
			if (!m_vSoundSegment.empty())
			{
				for (std::vector<boost::shared_ptr<Data::SoundSegment> >::iterator iter = m_vSoundSegment.begin(); iter != m_vSoundSegment.end(); ++iter)
				{				
					(*iter)->telephoneNumber(m_spContactInfo[line]->telephoneNumber());
					(*iter)->name(m_spContactInfo[line]->name());
					(*iter)->contactInfoId(m_spContactInfo[line]->id());
					(*iter)->contactId(m_spContactInfo[line]->contactId());
					
					if (m_spContactInfo[line]->type() == Data::citInNo)
					{
						(*iter)->type(Data::sstLeaveWord);
						(*iter)->played(false);
						
						::SendMessage(main->m_pMainDlg->GetSafeHwnd(), WM_TELNOTIFY, 2, 1);
					}
					else
					{
						(*iter)->type(Data::sstTeleRecord);
						(*iter)->played(true);
					}
					
					main->m_pSoundDlg->SaveSoundSegment(*iter);
					
				}
				
			}
			m_vSoundSegment.clear();
			
			main->m_pContactInfoDlg->ResetTypeInfo();
			if (m_spContactInfo[line]->type() == Data::citInNo)
			{
				::SendMessage(((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pMainDlg->GetSafeHwnd(), WM_TELNOTIFY, 1, 1);
			}

			if(m_spContactInfo[line]->type() != Data::citInNo && m_spContactInfo[line]->duration() > 0)
				::PostMessage(main->GetSafeHwnd(), WM_TELCALL_TIME, m_spContactInfo[line]->duration(), 0);
			
		}
		m_spContactInfo[line] = boost::shared_ptr<Data::ContactInfo>();
		
		main->m_phoneLine[line].Second = 0 ;
		main->m_phoneLine[line].RingCount = 0 ;
		
		main->m_phoneLine[line].bHasCallID  = false;
		main->m_phoneLine[line].bFirwall	= false;
		main->m_phoneLine[line].bRingPlayed = false;
		
		main->m_phoneLine[line].RingFilename = "" ;
		main->m_phoneLine[line].iIgnoreRingCount = 1;
		main->m_phoneLine[line].telnum = "";
		main->m_phoneLine[line].city = "";
		main->m_phoneLine[line].b3gReject = false;
		main->m_phoneLine[line].b3gFisrtRing = false;

		main->m_phoneLine[line].pFSM->setStartState(CMultimediaPhoneDlg::p3gsHangOff);
	}
		
}

void CTelephoneDlg::Mute(void)
{
	Dprintf("Mute\n");
	//去掉mute功能
	((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->Mute();
}

void CTelephoneDlg::Redial(void)
{
	Dprintf("Redial\n");

	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	if (main->m_phoneLine[0].pFSM->getCurrentState() == CMultimediaPhoneDlg::p3gsHangOff)
	return;
	
	main->SendOutEvnet(8021, 0);
	std::vector<boost::shared_ptr<Data::ContactInfo> > vCurrentResult = Data::ContactInfo::GetFromDatabase("type = 2");
	if(vCurrentResult.size() > 0)
	{
		{
			DialContact(vCurrentResult[0]->telephoneNumber());
		}
	}
	//((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->phone_->Redial();
}

void CTelephoneDlg::HangOnInRecord(void)
{
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	Dprintf("Hang On IN Record 1\n");
	if (m_spContactInfo && (m_spContactInfo[main->m_nline]->type() == Data::citOut))
		return;
	Dprintf("Hang On IN Record 2\n");

	if(m_bPlayingLeaveWord)
	{
		KillTimer(m_uiTipTimer);
		((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->StopRing(true);
		m_pOggCodec->StopDecode(true);
		m_bPlayingLeaveWord = FALSE;
	}
	else
	{
		bVoiceStop = true;
	}

	if (m_spSoundSegment)
	{
		m_spSoundSegment->type(Data::sstTeleRecord);
		m_spSoundSegment->played(true);
	}


	m_spContactInfo[main->m_nline]->type(Data::citInOk);

	//m_bAutoRecord = FALSE;
	m_bPlayingLeaveWord = FALSE;
	((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pFSM->fireEvent(CMultimediaPhoneDlg::teHangOn, NULL);
}

void CTelephoneDlg::HangOnToRecord(void)
{
 	Dprintf("Hang On To Record\n");
 	((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->HungOnToRecord();	
	//Mute();

// 	if (waveInOpen(&m_hWaveIn,0,&m_waveform,0,NULL,NULL))
// 	{
// 		Dprintf("waveInOpen error!\n");
// 		return ;
// 	}	
	waveInMessage(0,WAV_LINEOUT_ONLY,0,0);//WAV_SPEAKER_LINEOUT//WAV_LINEOUT_ONLY
// 	waveInReset(m_hWaveIn);
// 	waveInClose(m_hWaveIn);
}

std::vector<boost::shared_ptr<Data::Contact> > CTelephoneDlg::FindCallContact(std::string number)
{
	std::string filter;
	if (number.length() < 7)
	{
		filter += "mobilesTelephone = '";
		filter += (number);
		filter += "'";
		filter += " OR mobilesTelephone2 = '";
		filter += (number);
		filter += "'";
		filter += " OR worksTelephone = '";
		filter += (number);
		filter += "'";
		filter += " OR worksTelephone2 = '";
		filter += (number);
		filter += "'";
	}
	else
	{
		filter += "mobilesTelephone LIKE '%";
		filter += (number);
		filter += "'";
		filter += " OR mobilesTelephone2 LIKE '%";
		filter += (number);
		filter += "'";
		filter += " OR worksTelephone LIKE '%";
		filter += (number);
		filter += "'";
		filter += " OR worksTelephone2 LIKE '%";
		filter += (number);
		filter += "'";
	}
	
	std::vector<boost::shared_ptr<Data::Contact> > result = Data::Contact::GetFromDatabase(filter);
	return result;
}

void CTelephoneDlg::ShowContact(boost::shared_ptr<Data::Contact> contact, std::string number,int uintNo)
{	
	CMultimediaPhoneDlg *main =(CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	bool brefresh = false ;
	for (int i = 0; i < 4;i++)
	{
		m_MJPGList.SetUnitText(100+i,L"",brefresh);
	}

	CString str ;
	if ( 100 == uintNo )
	{
		m_spContactInfo[main->m_nline]->contactId(contact->id());
		m_spContactInfo[main->m_nline]->name(contact->name());
		str = CString(contact->name().c_str());
		str += _T("(");
		str += CString(number.c_str());
		str += _T(")");
		if (m_spContactInfo[main->m_nline]->type() != Data::citOut)
		{
			str += _T("(3G来电)");
		}
		else
		{
			str += _T("(3G拨打)");
		}

	}
	else
	{
		m_spPstnContactInfo->contactId(contact->id());
		m_spPstnContactInfo->name(contact->name());
		str = CString(contact->name().c_str());
		str += _T("(");
		str += CString(number.c_str());
		str += _T(")");
		str += _T("(PSTN来电)");
	}
	
	SetFirstItem(str,brefresh);
//	m_MJPGList.SetUnitText(100, str, brefresh);
	CString temp ;
	temp = Util::StringOp::ToCString(contact->company().c_str());//单位
	m_MJPGList.SetUnitText(101, "单位:"+temp, brefresh);

	temp = Util::StringOp::ToCString(contact->companyAddress().c_str());//单位地址
	m_MJPGList.SetUnitText(102, "单位地址:"+temp, brefresh);

	temp = Util::StringOp::ToCString(contact->mobilesTelephone().number().c_str());//号码
	m_MJPGList.SetUnitText(103, "移动电话1:"+temp, brefresh);


}

void CTelephoneDlg::DialContact(Data::TelephoneNumber telephoneNumber, int contactId)
{
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	if ((main->m_phoneLine[0].pFSM->getCurrentState() != CMultimediaPhoneDlg::p3gsHangOff)
		&& (main->m_phoneLine[0].pFSM->getCurrentState() != CMultimediaPhoneDlg::p3gsHangOn))
	return;

	BOOL flag = TRUE;
	if (!IsWindowVisible())
	{
		flag = FALSE;

		main->m_phoneLine[0].pFSM->setStartState(CMultimediaPhoneDlg::p3gsKey);
		
		if (!main->phone_->Hand())//change by qi 20100508
		{	
			extern void GMute(BOOL isOn);
			GMute(false);

			//add 20100722
			extern void GNotifyDial(BOOL isDial);
			GNotifyDial(1);

			//if(main->m_nTELRigster >= TELRIGSTER_TD)
				Telephone::TelephoneWarp::GetTelephoneWarp()->HandFree(true);

			main->phone_->SWitchPhone3G(true);
			main->phone_->Free(1);
		}

		main->AddIcon(Allicon[0],this,false);
		ShowWindow_(TRUE);
	}

	Dprintf("DialContact:\n");
	Dprintf(telephoneNumber.number().c_str());

	m_sTelephoneNumber = telephoneNumber.number();
	std::string number = m_sTelephoneNumber;

	//add by qi 0708
	main->m_phoneLine[0].telnum = number;

	m_sDialNumber = number;
	if (m_sDialNumber.substr(0,2) == "86")
	{
		m_sDialNumber = m_sDialNumber.substr(2);
	}

	m_sDialNumber = number;

	if (main->m_pSettingDlg->m_pSetting->isAutoAppendOutlinePrefix())
	{
		m_sOutLine = main->m_pSettingDlg->m_pSetting->outlinePrefix();
		if(m_sDialNumber.length() >= 5)
			SetFirstItem(CString(std::string(m_sOutLine + m_sDialNumber).c_str()),flag);
		//	m_MJPGList.SetUnitText(100, CString(std::string(m_sOutLine + m_sDialNumber).c_str()), flag);
		else 
		{
			int nindex = m_sDialNumber.find('1');
			if(nindex == 0)
				SetFirstItem(CString(std::string(m_sOutLine + m_sDialNumber).c_str()),flag);
			//	m_MJPGList.SetUnitText(100, CString(std::string(m_sOutLine + m_sDialNumber).c_str()), flag);
			else    //直接拨号
				SetFirstItem(CString(std::string(m_sDialNumber).c_str()),flag);
			//	m_MJPGList.SetUnitText(100, CString(std::string(m_sDialNumber).c_str()), flag);

		}

	}
	else
		SetFirstItem(CString(std::string(m_sDialNumber).c_str()),flag);
//		m_MJPGList.SetUnitText(100, CString(std::string(m_sDialNumber).c_str()), flag);
	SetTimer(4, 10, NULL);

	main->phone_->PhoneDialTone(TRUE, (char *)number.c_str());

	//((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->phone_->DialNumber((char*)number.c_str());
	
	if (!m_spContactInfo[main->m_nline])
	{
		m_spContactInfo[main->m_nline] = boost::shared_ptr<Data::ContactInfo>(new Data::ContactInfo);
		m_spContactInfo[main->m_nline]->type(Data::citOut);
		m_spContactInfo[main->m_nline]->startTime(CTime::GetCurrentTime());
		m_spContactInfo[main->m_nline]->telephoneNumber(m_sTelephoneNumber);
		m_spContactInfo[main->m_nline]->Insert();
	}

	//add by qi 20100712
	if (contactId > 0)
	{
		std::string filter;
		filter = "id = ";
		filter += Util::StringOp::FromInt(contactId);

		std::vector<boost::shared_ptr<Data::Contact > > result;
		result = Data::Contact::GetFromDatabase(filter);
 		if (!result.empty())
 		{
 			ShowContact(result[0], m_sTelephoneNumber);
 		}	
	}
	else
	{
		if (m_sTelephoneNumber.size() >= 3)
		{
			std::vector<boost::shared_ptr<Data::Contact> > result = FindCallContact(m_sTelephoneNumber);
			if (!result.empty())
			{
				ShowContact(result[0], m_sTelephoneNumber);
			}
		}
	}

}

UINT CTelephoneDlg::GetUnconnectCount(void)
{
	std::vector<boost::shared_ptr<Data::ContactInfo> > result = Data::ContactInfo::GetFromDatabase("type = 0 AND played = 0");	
	m_uiInNoCount = result.size();
	return m_uiInNoCount;
}
// 
// void CTelephoneDlg::ClearUnconnectCount(void)
// {
// 	m_uiInNoCount = 0;
// 	::SendMessage(((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pMainDlg->GetSafeHwnd(), WM_TELNOTIFY, 0, 0);
// }

UINT CTelephoneDlg::GetRecordCount(void)
{
	std::vector<boost::shared_ptr<Data::SoundSegment> > result = Data::SoundSegment::GetFromDatabase("type = 0 AND played = 0");	
	m_uiRecordCound = result.size();
	return m_uiRecordCound;
}
// 
// void CTelephoneDlg::ClearRecordCount(void)
// {
// 	m_uiRecordCound = 0;
// 	::SendMessage(((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pMainDlg->GetSafeHwnd(), WM_TELNOTIFY, 0, 0);
// }

void CTelephoneDlg::ShowWindow_(int cmdshow)
{
	if(cmdshow > 0)
	{
		((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pMainDlg->SendMessage(WM_PLAYVIDEO, 0, 0);    //暂停视频   重复发送了暂停信息

		((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pMainDlg->m_pWebDialog->SendMessage(WM_KILLWEBSHOW, 1, 0);
		::SetWindowPos(m_hWnd, HWND_TOPMOST, 0, 57, 800, 480, 0);
	//	CCEDialog::ShowWindow_(cmdshow);
		ShowWindow(cmdshow);

		::PostMessage(((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pMainDlg->m_firewalDlg_->m_cmbTime.m_pCombo->m_hWnd, CB_SHOWDROPDOWN,0,0);  //如果commbox打开，让commobox隐藏
		::PostMessage(((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pContactGroupDlg->m_cmbSoundTip.m_pCombo->m_hWnd, CB_SHOWDROPDOWN,0,0);
		
		::PostMessage(((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pContactNewDlg->m_cmbType.m_pCombo->m_hWnd, CB_SHOWDROPDOWN,0,0);
		::PostMessage(((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pContactNewDlg->m_cmbGroup.m_pCombo->m_hWnd, CB_SHOWDROPDOWN,0,0);
		::PostMessage(((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pMainDlg->m_mainLunarderDlg_->m_cmbRing.m_pCombo->m_hWnd, CB_SHOWDROPDOWN,0,0);
//		::PostMessage(((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pContactDlg->m_pSimImportDlg->m_cmbGroup.m_pCombo->m_hWnd, CB_SHOWDROPDOWN,0,0);
		::PostMessage(((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pMainDlg->m_pWebDialog->m_cmbURL.m_pCombo->m_hWnd, CB_SHOWDROPDOWN,0,0);
		
//		::PostMessage(((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_cmbRing.m_pCombo->m_hWnd, CB_SHOWDROPDOWN,0,0);
		::PostMessage(((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_cmbRingTimes.m_pCombo->m_hWnd, CB_SHOWDROPDOWN,0,0);
		::PostMessage(((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_cmbAutoRecoedeTimes.m_pCombo->m_hWnd, CB_SHOWDROPDOWN,0,0);
		::PostMessage(((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_cmbWaitTime.m_pCombo->m_hWnd, CB_SHOWDROPDOWN,0,0);
//		::PostMessage(((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_cmbSoundSavePath.m_pCombo->m_hWnd, CB_SHOWDROPDOWN,0,0);
		
//		::PostMessage(((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_cmbRingVolume.m_pCombo->m_hWnd, CB_SHOWDROPDOWN,0,0);
//		::PostMessage(((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_cmbSystemVolume.m_pCombo->m_hWnd, CB_SHOWDROPDOWN,0,0);
		::PostMessage(((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_cmbBlackLightWaitTime.m_pCombo->m_hWnd, CB_SHOWDROPDOWN,0,0);
	}
	else
	{
	//	CCEDialog::ShowWindow_(cmdshow);
		ShowWindow(cmdshow);

		((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pMainDlg->m_pWebDialog->SendMessage(WM_KILLWEBSHOW, 0, 0);

//		((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pMainDlg->SendMessage(WM_PLAYVIDEO, 1, 0);			//恢复视频
//		((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pMainDlg->m_mainMp3Dlg_->SendMessage(WM_OUTEVENT, 0, 1);
	}

	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg *)theApp.m_pMainWnd;
	if(main->m_pMainDlg->m_mainScreenSaveDlg_->m_isScreenPlay)
	{
		main->m_pMainDlg->m_mainScreenSaveDlg_->OnExit();
	}
}

void CTelephoneDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	CString icon ;
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	if(nIDEvent == IDT_TEL_STATUS)
	{
		if (main->m_phoneLine[0].pFSM->getCurrentState() == CMultimediaPhoneDlg::p3gsHangOff )
		{
			m_strTelStatus = "";
			KillTimer(IDT_TEL_STATUS);
		}
		CString s = m_MJPGList.GetUnitText(400);
		if(s != "")
			m_MJPGList.SetUnitText(400, "", TRUE);
		else
			m_MJPGList.SetUnitText(400, m_strTelStatus, TRUE);

	}

	if (nIDEvent == main->m_phoneLine[0].LineStatusTimer)
	{
		LineStatusTimer(0);
	}
	else if (nIDEvent == main->m_phoneLine[1].LineStatusTimer)
	{
		LineStatusTimer(1);
	}
	else if(nIDEvent == IDT_DIAL_PRESS)
	{	
		KillTimer(IDT_DIAL_PRESS);
	}
	else if(nIDEvent == IDT_TEL_EXIT)
	{
		KillTimer(IDT_TEL_EXIT);
		HandleOff();
	}
	else if (nIDEvent == main->m_phoneLine[0].HangoffTimer)
	{
	//	Dprintf("HangoffTimer : 0\r\n");
		KillTimer(main->m_phoneLine[0].HangoffTimer);
		HandleOff(1);
	}
	else if (nIDEvent == main->m_phoneLine[1].HangoffTimer)
	{	
	//	Dprintf("HangoffTimer : 1\r\n");
		KillTimer(main->m_phoneLine[1].HangoffTimer);
		HandleOff(2);
	}
	else if (nIDEvent == main->m_phoneLine[0].timer)
	{
	//	Dprintf("Line0\r\n");

// 		CTimeSpan ts;
// 		ts = CTime::GetCurrentTime() - m_spContactInfo[main->m_nline]->startTime() - m_uTelSecondOff;
// 		m_uiTelephoneSecond = ts.GetTotalSeconds();
// 
// 		CString duration;
// 		if (m_uiTelephoneSecond > 3600)
// 		{
// 			duration.Format(_T("%d:%02d:%02d"), ts.GetHours(), ts.GetMinutes(),ts.GetSeconds());
// 		}
// 		else
// 		{
// 			duration.Format(_T("%02d:%02d"), ts.GetMinutes(),ts.GetSeconds());
// 		}
// 
// 		if(IsWindowVisible() && !(main->m_pNotebookDlg->m_bIsOPenTel))
// 		{
// 			m_MJPGList.SetUnitText(107, duration, TRUE, TRUE);
// 		}
// 
// 		if ((m_uiTelephoneSecond % 60) == 0)
// 		{
// 			m_spContactInfo[main->m_nline]->duration(m_uiTelephoneSecond);
// 			m_spContactInfo[main->m_nline]->Update();
// 		}	
		LineDurTimer(0);

	}
	else if (nIDEvent == main->m_phoneLine[1].timer)
	{	
	//	Dprintf("Line1\r\n");

		LineDurTimer(1);	
	}
	else if (nIDEvent == m_uiPstnTelephoneTimer)
	{
		CTimeSpan ts;
		ts = CTime::GetCurrentTime() - m_spPstnContactInfo->startTime() - m_uPstnTelSecondOff;
		m_uiPstnTelephoneSecond = ts.GetTotalSeconds();
		
		CString duration;
		if (m_uiPstnTelephoneSecond > 3600)
		{
			duration.Format(_T("%d:%02d:%02d"), ts.GetHours(), ts.GetMinutes(),ts.GetSeconds());
		}
		else
		{
			duration.Format(_T("%02d:%02d"), ts.GetMinutes(),ts.GetSeconds());
		}
		
		if(IsWindowVisible() && !((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pNotebookDlg->m_bIsOPenTel)
		{	
			m_MJPGList.SetUnitText(400,L"通话中...",true);
			m_MJPGList.SetUnitIsShow(105,true,true);
			m_MJPGList.SetUnitText(107, duration, TRUE, TRUE);					
		}

		if ((m_uiPstnTelephoneSecond % 60) == 0)
		{
			m_spPstnContactInfo->duration(m_uiPstnTelephoneSecond);
			m_spPstnContactInfo->Update();
		}

	}
	else if (nIDEvent == m_uiRecordTimer)
	{
	//	std::string path = ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pSetting->soundPath();
		
		extern BOOL DetectDIR(TCHAR *sDir);
		if (path == ssStorageCardRecordPath)
		{
			if( !DetectDIR(_T("/storagecard")))
			{
				OnButtonTelephoneRecord();
				
				m_MJPGList.SetUnitText(106, "SD卡不存在", TRUE);
				m_MJPGList.SetUnitIsShow(106, TRUE);
				
				CDialog::OnTimer(nIDEvent);
				//CCEDialog::OnTimer(nIDEvent);
				return;
			}
		}

		CTimeSpan ts;
		ts = CTime::GetCurrentTime() - m_spSoundSegment->startTime();
		m_uiRecordSecond = ts.GetTotalSeconds();

		//++m_uiRecordSecond;
		//CTimeSpan ts(m_uiRecordSecond);

		CString duration;
		if (m_uiRecordSecond > 3600)
		{
			duration.Format(_T("%d:%02d:%02d"), ts.GetHours(), ts.GetMinutes(),ts.GetSeconds());
		}
		else
		{
			duration.Format(_T("%02d:%02d"), ts.GetMinutes(),ts.GetSeconds());
		}
		if(IsWindowVisible() && !((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pNotebookDlg->m_bIsOPenTel)
		{
			BOOL flag = FALSE;
			if(!m_MJPGList.GetUnitIsShow(107))
				flag = TRUE;
			m_MJPGList.SetUnitText(106, duration, flag);
		}
		if ((m_uiRecordSecond % 60) == 0)
		{
			m_spSoundSegment->duration(m_uiRecordSecond);
			m_spSoundSegment->Update();
		}
		if (m_uiRecordSecond >= m_uiRemainRecordSecond)
		{
			OnButtonTelephoneRecord();
		}
	}
	else if (nIDEvent == m_uiTipTimer)
	{
		KillTimer(m_uiTipTimer);
		m_bPlayingLeaveWord = FALSE;
		m_pOggCodec->StopDecode(TRUE);
		waveInMessage(0,WAV_LINEOUT_ONLY,0,0);//WAV_SPEAKER_LINEOUT//WAV_LINEOUT_ONLY
		Dprintf("Wav_LineOut\r\n");
		((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->StartRing(_T("\\hive\\tip2.wav"), 1);
		Sleep(1500);

		OnButtonTelephoneRecord();

		bVoiceStop = false;
		Dprintf("Voice Check start\n");
		hCheckThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)VoiceCheckThreadPrc, NULL, 0, NULL);
		if (hCheckThread == INVALID_HANDLE_VALUE)
		{
			Dprintf("Create Check Thread error!\n");
			bVoiceStop = true;
		}
		int nTimers = ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pSetting->maxSoundUseSize();
		int nTimers_[] = {15, 30, 60, 90};
		SetTimer(IDT_STOPAUTORECORDE_TIME, (nTimers_[nTimers])*1000, NULL);

	}
	else if(nIDEvent == IDT_STOPAUTORECORDE_TIME) //停止自动录音留言
	{
		KillTimer(IDT_STOPAUTORECORDE_TIME);
		((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->HungOff();	

	}
	else if(nIDEvent == IDC_TELDLGSHOW)
	{
		KillTimer(IDC_TELDLGSHOW);
		Dprintf("TelDlg show\n");
		if(/*m_bRing || */((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->GetPhoneHungOn())
		{
			//change by qi 2009_11_04				
			if (!((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pTelphoneDialDlg->IsWindowVisible())
			{	

				//加载图标

				CWnd *p = main->m_pTelphoneDialDlg ;
				icon = Allicon[0];
				main->AddIcon(icon,p,false);
				
				main->m_pTelphoneDialDlg->ShowWindow_(SW_SHOW);
				
				//启动10条通话记录
//				SetTimer(SHOW_10CONTACTINFO_TIMER,10,NULL);
				main->m_pTelphoneDialDlg->m_bMayReturn = true ;
				

			}
			
//			old
// 			if(!IsWindowVisible())
// 			{	
// 				ShowWindow_(TRUE);
// 			}

		}
		else
		{
			Dprintf("hide1111\r\n");
//			HandleOff();

		}
	}
	else if(nIDEvent == IDC_TELDLGHIDE)
	{
//		Dprintf("TelDlg hide3\n");
//		m_bRing = FALSE;
// 		KillTimer(IDC_TELDLGHIDE);
// 		ShowWindow_(FALSE);
// 
// 		m_MJPGList.SetUnitIsShow(9, FALSE);
// 		m_MJPGList.SetUnitIsShow(19, FALSE);
// 		m_MJPGList.SetUnitIsShow(3, FALSE);

	}
	else if(nIDEvent == 4)
	{
		CMultimediaPhoneDlg *main= (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
		KillTimer(4);
		memset(m_chDialNumber, 0, 64);
		if (main->m_pSettingDlg->m_pSetting->isAutoAppendOutlinePrefix())
		{

			if((m_sDialNumber.length() >= 5))
			{
				m_sOutLine = main->m_pSettingDlg->m_pSetting->outlinePrefix();
				//((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->phone_->DialNumber((char*)m_sOutLine.c_str());
				strcpy(m_chDialNumber, (char*)m_sOutLine.c_str());
				strcat(m_chDialNumber, ",");
				//Sleep(2000);
			}
			else 
			{
				int nindex = m_sDialNumber.find('1');
				if( nindex == 0)
				{
					m_sOutLine = main->m_pSettingDlg->m_pSetting->outlinePrefix();
					strcpy(m_chDialNumber, (char*)m_sOutLine.c_str());
					strcat(m_chDialNumber, ",");
					//((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->phone_->DialNumber((char*)m_sOutLine.c_str());
					//Sleep(2000);
				}
			}

		}

		memcpy(m_chDialNumber+strlen(m_chDialNumber), (char*)m_sDialNumber.c_str(), strlen((char*)m_sDialNumber.c_str()));
		main->phone_->DialNumber(m_chDialNumber);
	//	main->m_phoneLine[0].pFSM->setStartState(CMultimediaPhoneDlg::p3gsKey);
	
	}
	else if(nIDEvent == 5)
	{
		KillTimer(5);
//		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pFSM->fireEvent(CMultimediaPhoneDlg::teConnect, NULL);
	}
	else if (SHOW_10CONTACTINFO_TIMER == nIDEvent)
	{	
		KillTimer(SHOW_10CONTACTINFO_TIMER);
		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pTelphoneDialDlg->m_p10ContactInfoDlg->Show10ContactInfo();
		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pTelphoneDialDlg->m_bMayReturn = true ;
	}
	else if ( CHECK_0X82_TIMER == nIDEvent)//检查软件接听pstn是否成功
	{
		if (m_bSend0x82Win)
		{	
			Dprintf("Send0x82Win\r\n");
			KillTimer(CHECK_0X82_TIMER);
			main->m_phoneLine[2].pFSM->fireEvent(CMultimediaPhoneDlg::pstneHangOn,NULL);
			m_bSend0x82Win = false;
		}
		else//重发发送
		{
			main->phone_->PSTNHangOn();	
		}
	}

	extern BOOL _test_call;
	if(_test_call)
	{
		if (nIDEvent == 10)
		{
			KillTimer(10);
			HandleOff();
		}
	}
	
	CDialog::OnTimer(nIDEvent);
//	CCEDialog::OnTimer(nIDEvent);

}

bool CTelephoneDlg::RecStart(void)
{		
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	LRESULT l=waveInOpen(&m_hWaveIn,WAVE_MAPPER,&m_waveform,(DWORD)this->m_hWnd,NULL,CALLBACK_WINDOW);
	if (l)
	{
		Dprintf("waveInOpen error\r\n");
		return false;
	}
	waveInMessage(m_hWaveIn,WAV_LINEIN_MIC,0,0);
	Dprintf("Wav_WAV_LINEIN_MIC\r\n");

	for (int i = 0; i < WAVE_BUFFER_COUNT; ++i)
	{
		m_pWaveHdr[i]->lpData=(char*)m_pBuffer[i];
		m_pWaveHdr[i]->dwBufferLength=INP_BUFFER_SIZE;
		m_pWaveHdr[i]->dwBytesRecorded=0;
		m_pWaveHdr[i]->dwUser=0;
		m_pWaveHdr[i]->dwFlags=0;
		m_pWaveHdr[i]->dwLoops=1;
		m_pWaveHdr[i]->lpNext=NULL;
		m_pWaveHdr[i]->reserved=0;
		
		waveInPrepareHeader(m_hWaveIn,m_pWaveHdr[i],sizeof(WAVEHDR));
		waveInAddBuffer (m_hWaveIn, m_pWaveHdr[i], sizeof (WAVEHDR));
	}
	
	m_bEnding = FALSE ;
	m_bRecording = TRUE;
	m_dwDataLength = 0 ;
	waveInStart (m_hWaveIn) ;

	Dprintf("RecStart\n");
	return true;

}

bool CTelephoneDlg::RecStop(void)
{
	m_bEnding=TRUE;		
	waveInReset(m_hWaveIn);

	return true;
}

void CTelephoneDlg::SaveWavHead(void)
{
	if (m_sRecordFileName == "")
	{
		return;
	}

#pragma pack(2)
	
	struct wave_header
	{
		char riff[4];             /* "RIFF" */
		unsigned long size;       /* length of rest of file = size of rest of header(36) + data length */
		char wave[4];             /* "WAVE" */
		char fmt[4];              /* "fmt " */
		unsigned long fmt_len;    /* length of rest of fmt chunk = 16 */
		unsigned short tag;       /* MS PCM = 1 */
		unsigned short channels;  /* channels, mono = 1, stereo = 2 */
		unsigned long samp_rate;  /* samples per second = 44100 */
		unsigned long byte_rate;  /* bytes per second = samp_rate * byte_samp = 176400 */
		unsigned short byte_samp; /* block align (bytes per sample) = channels * bits_per_sample / 8 = 4 */
		unsigned short bit_samp;  /* bits per sample = 16 for MS PCM (format specific) */
		unsigned short res0;
		char data[4];             /* "data" */
		unsigned long length;     /* data length (bytes) */
	} header;

	DWORD temp = RIFF_FILE;
	memcpy(&header.riff, &temp, 4);
	header.size = 36 + m_dwDataLength;
	temp = RIFF_WAVE;
	memcpy(&header.wave, &temp, 4);
	temp = RIFF_FORMAT;
	memcpy(&header.fmt, &temp, 4);
	header.fmt_len = sizeof(WAVEFORMATEX);
	memcpy(&header.tag, &m_waveform, sizeof(WAVEFORMATEX));
	temp = RIFF_DATA;
	memcpy(&header.data, &temp, 4);
	header.length = m_dwDataLength;
		
	CFileStatus   status;
	if (CFile::GetStatus(LPCTSTR(Util::StringOp::ToCString(m_sRecordFileName)),status))
	{
		m_fWav.Open(LPCTSTR(Util::StringOp::ToCString(m_sRecordFileName)), CFile::modeWrite);
	}
	else
	{
		m_fWav.Open(LPCTSTR(Util::StringOp::ToCString(m_sRecordFileName)), CFile::modeCreate | CFile::modeWrite);
	}
	
	m_fWav.SeekToBegin();
 	m_fWav.Write(&header, sizeof(wave_header));
	m_fWav.Close();
	
}

void CTelephoneDlg::OnMM_WIM_OPEN(UINT wParam, LONG lParam) 
{
	// TODO: Add your message handler code here and/or call default
	Dprintf("OnMM_WIM_OPEN\n");
//	PlayStart();

}

void CTelephoneDlg::OnMM_WIM_DATA(UINT wParam, LONG lParam) 
{
//	Dprintf("OnMM_WIM_DATA\n");
//	Dprintf("OnMM_WIM_DATA = %d\n", ((PWAVEHDR) lParam)->dwBytesRecorded);

//  m_fWav.Write(((PWAVEHDR) lParam)->lpData, ((PWAVEHDR) lParam)->dwBytesRecorded);
// 	m_dwDataLength += ((PWAVEHDR) lParam)->dwBytesRecorded;

 //test mhq	
	OggCodec::PcmData * pd = new OggCodec::PcmData;
	if (pd != NULL)
	{
		pd->dataLength = ((PWAVEHDR) lParam)->dwBytesRecorded / sizeof(short);
		pd->data = (short*)malloc(((PWAVEHDR) lParam)->dwBytesRecorded + 2);
		if (pd->data != NULL)
		{
			memcpy(pd->data , ((PWAVEHDR) lParam)->lpData, ((PWAVEHDR) lParam)->dwBytesRecorded);
			m_pOggCodec->PushPcmData(pd);
		}
		else
		{
			delete pd;
		}
	}

//	Dprintf("OnMM_WIM_DATA 0\r\n");

// 	char * d = (char*)malloc(((PWAVEHDR) lParam)->dwBytesRecorded);
// 	memcpy(d , ((PWAVEHDR) lParam)->lpData, ((PWAVEHDR) lParam)->dwBytesRecorded);
// 	
// 	PWAVEHDR wh = reinterpret_cast<PWAVEHDR>(malloc(sizeof(WAVEHDR)));
// 	wh->lpData          = d;
// 	wh->dwBufferLength  = ((PWAVEHDR) lParam)->dwBytesRecorded / sizeof(short); ;
// 	wh->dwBytesRecorded = 0 ;
// 	wh->dwUser          = 0 ;
// 	wh->dwFlags         = WHDR_BEGINLOOP | WHDR_ENDLOOP ;
// 	wh->dwLoops         = 1 ;
// 	wh->lpNext          = NULL ;
// 	wh->reserved        = 0 ;
// 	
// 	// Prepare and write
// 	
// 	waveOutPrepareHeader (m_hWaveOut, wh, sizeof (WAVEHDR)) ;
// 	waveOutWrite (m_hWaveOut, wh, sizeof (WAVEHDR)) ;
	if (!bVoiceStop)
	{
		OggCodec::PcmData * pd = new OggCodec::PcmData;
		if (pd != NULL)
		{
			pd->dataLength = ((PWAVEHDR) lParam)->dwBytesRecorded / sizeof(short);
			pd->data = (short*)malloc(((PWAVEHDR) lParam)->dwBytesRecorded);
			if (pd->data != NULL)
			{
				memcpy(pd->data , ((PWAVEHDR) lParam)->lpData, ((PWAVEHDR) lParam)->dwBytesRecorded);
//				EnterCriticalSection(&csVoice);
				CheckQueue.push(pd);
//				LeaveCriticalSection(&csVoice);
			}
			else
			{
				delete pd;
			}
		}
	}

//	Dprintf("OnMM_WIM_DATA 2\r\n");

	if (m_bEnding)
	{
		Dprintf("waveInClose\n");
		//waveInReset(m_hWaveIn);
		waveInClose(m_hWaveIn);
		return;
	}
	
	// Send out a new buffer	
	waveInAddBuffer (m_hWaveIn, (PWAVEHDR) lParam, sizeof (WAVEHDR));
//	Dprintf("OnMM_WIM_DATA 3\r\n");
}

void CTelephoneDlg::OnMM_WIM_CLOSE(UINT wParam, LONG lParam) 
{
	// TODO: Add your message handler code here and/or call default
	Dprintf("OnMM_WIM_CLOSE\n");
	
//	waveInUnprepareHeader (m_hWaveIn, m_pWaveHdr1, sizeof (WAVEHDR)) ;
//	waveInUnprepareHeader (m_hWaveIn, m_pWaveHdr2, sizeof (WAVEHDR)) ;
	for (int i = 0; i < WAVE_BUFFER_COUNT; ++i)
	{
		waveInUnprepareHeader (m_hWaveIn, m_pWaveHdr[i], sizeof (WAVEHDR));
	}

// 	m_fWav.Close();
// 	SaveWavHead();

	bVoiceStop = true;
	m_pOggCodec->StopEncode(true);
	m_bRecording = FALSE;


	m_MJPGList.SetUnitIsShow(104, FALSE);
	m_MJPGList.SetUnitIsShow(106, FALSE);
	m_MJPGList.SetUnitText(106, L"", TRUE);

	m_MJPGList.SetUnitIsShow(8, FALSE);
	m_MJPGList.SetUnitIsShow(2, TRUE);
	m_MJPGList.Invalidate();
	
	Dprintf("Wav_WAV_LINEIN_MIC\r\n");
	waveInMessage(0,WAV_LOCAL_MIC,0,0);		//20090324
//	m_btnRecord.SetWindowText(m_strRecord);
	
// 			if (m_uiRecordSecond == 0)
// 			{
// 				m_uiRecordSecond = 1;
// 			}
// 	m_spSoundSegment->duration(Util::TimeSpan(m_uiRecordSecond));
// 	m_spSoundSegment = boost::shared_ptr<Data::SoundSegment>();
// 	m_uiRecordSecond = 0;

// 	if (m_uiRecordSecond <= 1)
// 	{
// 		CFile::Remove(Util::StringOp::ToCString(m_sRecordFileName));
// 	}
//	Sleep(500);

//	PlayStop();
}

bool CTelephoneDlg::PlayStart(void)
{
	if (m_bPlaying)
	{
		waveOutReset(m_hWaveOut);
	}
	m_waveform.wFormatTag=WAVE_FORMAT_PCM;
	m_waveform.nChannels=CHANNELS;
	m_waveform.nSamplesPerSec=SAMPLESPERSEC;
	m_waveform.wBitsPerSample=BITSPERSAMPLE;
	m_waveform.nBlockAlign=m_waveform.nChannels * m_waveform.wBitsPerSample / 8;
	m_waveform.nAvgBytesPerSec=m_waveform.nBlockAlign * m_waveform.nSamplesPerSec;
	m_waveform.cbSize=0;	
	
	if (waveOutOpen(&m_hWaveOut,WAVE_MAPPER,&m_waveform,(DWORD)this->m_hWnd,NULL,CALLBACK_WINDOW))
	{
		return false;
	}
	
	Dprintf("Wav_LineOut\r\n");
 	waveInMessage(0, WAV_LINEOUT_ONLY,0,0);
	return true;
}

bool CTelephoneDlg::PlayStop(void)
{
	if (!m_bPlaying)
	{
		return true;
	}
	waveOutReset(m_hWaveOut);
	waveOutClose(m_hWaveOut);
	return true;
}

void CTelephoneDlg::OnMM_WOM_OPEN(UINT wParam,LONG lParam)
{
	Dprintf("OnMM_WOM_OPEN\n");	
	m_bPlaying = TRUE ;
}

void CTelephoneDlg::OnMM_WOM_DONE(UINT wParam,LONG lParam)
{
 	Dprintf("OnMM_WOM_DONE\n");

	waveOutUnprepareHeader (m_hWaveOut, (PWAVEHDR) lParam, sizeof (WAVEHDR));
	free(((PWAVEHDR) lParam)->lpData);
	free((PWAVEHDR) lParam);

// 	waveOutUnprepareHeader (m_hWaveOut, m_pWaveHdr1, sizeof (WAVEHDR)) ;
// 	waveOutClose (m_hWaveOut) ;
// 	m_bPlaying = FALSE ;	
// 	if (m_pOutBuffer != NULL)
// 	{
// 		free(m_pOutBuffer);
// 		m_pOutBuffer = NULL;
// 	}
}

void CTelephoneDlg::OnMM_WOM_CLOSE(UINT wParam,LONG lParam)
{
 	Dprintf("OnMM_WOM_CLOSE\n");
 	m_bPlaying = FALSE ;	
// 	if (m_pOutBuffer != NULL)
// 	{
// 		free(m_pOutBuffer);
// 		m_pOutBuffer = NULL;
// 	}

}

void CTelephoneDlg::FromTelDial(std::string tel)
{	
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
//	m_spContactInfo[main->m_nline]	= pContactInfo  ;
	m_sTelephoneNumber	= tel			;
	m_bTelUsing			= TRUE			;
	
	main->m_phoneLine[main->m_nline].telnum = tel;

	CString s = Util::StringOp::ToCString(m_sTelephoneNumber);
	SetFirstItem(s,true);
//	m_MJPGList.SetUnitText(100,s,true);

	//add by qi 20100712
	if (m_sTelephoneNumber.size() >= 3)
	{
		std::vector<boost::shared_ptr<Data::Contact> > result = FindCallContact(m_sTelephoneNumber);
		if (!result.empty())
		{
			ShowContact(result[0], m_sTelephoneNumber);
		}
	}
	
}

void CTelephoneDlg::VolumeSwitch()
{	
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;

	if (!g_isPhoneMute)
	{
		m_MJPGList.SetUnitIsDownStatus(50,true);
		g_isPhoneMute = TRUE;
		main->phone_->Mute(true);
		//main->phone_->Volume(0);
		for (int i = 0 ; i < 5; i++)
		{
			m_MJPGList.SetUnitIsDownStatus(40+i,true);
			m_MJPGList.SetUnitIsShow(40+i,true,true);
			m_MJPGList.SetUnitIsDisable(40+i,true);
		}
	}
	else
	{	
		m_MJPGList.SetUnitIsDownStatus(50,false);
		g_isPhoneMute = FALSE ;
		main->phone_->Mute(false);
		main->phone_->Volume(m_nLevel);
		for (int i = 0 ; i < 5; i++)
		{
			m_MJPGList.SetUnitIsShow(40+i,true,true);
			m_MJPGList.SetUnitIsDisable(40+i,false);
		}
		SetAudioLevel();

	}

}

void CTelephoneDlg::SetVolume(unsigned int level)
{
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd ;
	main->phone_->Volume(level);
	
	if (m_MJPGList.GetUnitIsDownStatus(40+level))
	{
		level++;
	}
	m_nLevel = level;
	SetAudioLevel();

}

void CTelephoneDlg::HandleAudio(bool bt)
{		
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd ;

	if (bt)
	{
		m_nLevel++;
		if (m_nLevel > 5)
		{
			m_nLevel = 5;
			return ;
		}
	}
	else
	{	m_nLevel--;
		if (m_nLevel < 0)
		{
			m_nLevel = 0 ;
			return ;
		}
	}
	
	main->phone_->Volume(m_nLevel);
	SetAudioLevel();
	
}

void CTelephoneDlg::SetAudioLevel()
{
	for (int i = 0 ; i < 5; i++)
	{
		m_MJPGList.SetUnitIsDownStatus(40+i,true);
	}
	
	for (i = 0 ; i < m_nLevel;i++)
	{
		m_MJPGList.SetUnitIsDownStatus(40+i,false);	
	}
	m_MJPGList.Invalidate();

}

void CTelephoneDlg::Hold2Dial()//保持一路去拨号
{
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	main->m_pTelphoneDialDlg->SubDialShow();	
}

void CTelephoneDlg::Dialback(std::string telnum)
{
	CMultimediaPhoneDlg *pMainDlg = (CMultimediaPhoneDlg*)theApp.m_pMainWnd ;
	CString icon ;
	//拨号
	if(!telnum.empty())
	{	
		//添加图标
		CWnd *p = this ;
		icon = Allicon[0];
		pMainDlg->AddIcon(icon,p,this);
		
		Data::TelephoneNumber telphonenumber ;
		telphonenumber.number(telnum);
		pMainDlg->m_phoneLine[0].pFSM->setStartState(CMultimediaPhoneDlg::p3gsHangOn);
		DialContact(telphonenumber);
	}

}

void CTelephoneDlg::PhoneSwitch()
{
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	main->m_pATCommandWarp1->CHLD(2);//把当前活动的路保持
//	main->m_nline = (main->m_nline + 1) %2 ;//3G_1 和 3G_2切换
//	ShowMainInfo();
	
}

void CTelephoneDlg::OnCallWaitting(WPARAM w, LPARAM l)
{
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;

}

void CTelephoneDlg::LineDurTimer(int nline)
{	
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;

	if (m_spContactInfo[nline])
	{
		CTimeSpan ts;
		ts = CTime::GetCurrentTime() - m_spContactInfo[nline]->startTime() - main->m_phoneLine[nline].SecondOff;
		main->m_phoneLine[nline].Second = ts.GetTotalSeconds();

		CString duration;
		if (main->m_phoneLine[nline].Second > 3600)
		{
			duration.Format(_T("%d:%02d:%02d"), ts.GetHours(), ts.GetMinutes(),ts.GetSeconds());
		}
		else
		{
			duration.Format(_T("%02d:%02d"), ts.GetMinutes(),ts.GetSeconds());
		}
		
		if ((main->m_phoneLine[nline].Second % 60) == 0)
		{
			m_spContactInfo[nline]->duration(main->m_phoneLine[nline].Second);
			m_spContactInfo[nline]->Update();
		}
		
		if (nline == main->m_nline)
		{		
			if(IsWindowVisible() && !(main->m_pNotebookDlg->m_bIsOPenTel))
			{	
				m_MJPGList.SetUnitIsShow(105,true,true);
				m_MJPGList.SetUnitText(107, duration, TRUE, TRUE);
			}		
		}
		else
		{
			if(IsWindowVisible() && !(main->m_pNotebookDlg->m_bIsOPenTel))
			{	
				//  号码，时间，状态，接听
				//m_MJPGList.SetUnitText(32,duration,true,true);
			}
		}
	}

}

void CTelephoneDlg::LineStatusTimer(int nline)
{	
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
// 	if (main->m_phoneLine[nline].pFSM->getCurrentState() == CMultimediaPhoneDlg::p3gsHangOff )
// 	{
// 		m_strTelStatus = "";
// 		KillTimer(IDT_TEL_STATUS);
// 	}
	
	if (nline == main->m_nline)//主线路
	{
		CString status = m_MJPGList.GetUnitText(400);
		if(status != L"")
			m_MJPGList.SetUnitText(400, L"", TRUE);
		else
			m_MJPGList.SetUnitText(400, status, TRUE);

	}
	else//另一路
	{
		Dprintf("Timer_status\r\n");
		CString status = m_MJPGList.GetUnitText(31);
		if(status != L"")
			m_MJPGList.SetUnitText(31, L"", TRUE);
		else
			m_MJPGList.SetUnitText(31, status, TRUE);
	}

}

void CTelephoneDlg::ShowLineStatus(int nline)
{	
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	
//	Dprintf("ShowLineStatus : %d\r\n" ,nline);
	CString number;
	CString status;
	std::string num = main->m_phoneLine[nline].telnum;
	
	std::vector<boost::shared_ptr<Data::Contact> > result ;
	if ( !num.empty())
	{
		FindCallContact(num);
	}	
	if (!result.empty())
	{
		number = Util::StringOp::ToCString(result[0]->name());
	}
	else
	{
		number = Util::StringOp::ToCString(main->m_phoneLine[nline].telnum);
	}

	status = Util::StringOp::ToCString(main->m_phoneLine[nline].lineStatus);

	if (nline == main->m_nline)//主线路
	{
		m_MJPGList.SetUnitText(400,status,true);
	}
	else if ( nline != main->m_nline && main->m_phoneLine[main->m_nline].pFSM->getCurrentState() 
		>= CMultimediaPhoneDlg::p3gsDialing && main->m_nline != 2)//另一路的状态
	{
		Dprintf("Show_Right\r\n");
		m_MJPGList.SetUnitText(30,number,true);
		m_MJPGList.SetUnitText(31,status, true);
	}
	

//  add by qi
//	KillTimer(main->m_phoneLine[nline].LineStatusTimer);
//	SetTimer(main->m_phoneLine[nline].LineStatusTimer,500,NULL);
	

}

void CTelephoneDlg::SelectNum(int id)
{
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	if (0 == id)//线路一
	{
		m_nSelectLine = 0 ;				
	}
	else if(1 == id)//线路二
	{
		if (main->m_phoneLine[1].pFSM->getCurrentState()
			!= CMultimediaPhoneDlg::p3gsHangOff )
		{
			m_nSelectLine = 1;
		}
		else 
		{
			m_nSelectLine = 2;
		}
	}
	else if (2 == id)//线路三
	{
		m_nSelectLine = 2;
	}

}

void CTelephoneDlg::ShowMainInfo()
{	
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	
// 	for (int i = 0;i < 3;i++)
// 	{
// 		m_MJPGList.SetUnitText(101+i,L"",true);
// 	}

	std::string tel  = main->m_phoneLine[main->m_nline].telnum;
	std::string city = main->m_phoneLine[main->m_nline].city;
	std::string type ;
	
	if (m_spContactInfo[main->m_nline])
	{
		if (m_spContactInfo[main->m_nline]->type() != Data::citOut )
		{
			type = "(3G来电)";	
		}
		else
		{
			type = "(3G拨打)";
		}
	}
	
	SetFirstItem(Util::StringOp::ToCString(tel + city +type),false);
//	m_MJPGList.SetUnitText(100, Util::StringOp::ToCString(tel + city +type), false);
	
	if (!tel.empty())
	{
		std::vector<boost::shared_ptr<Data::Contact> > result = FindCallContact(tel);
		if (!result.empty())
		{
			ShowContact(result[0], tel);
		}
		else 
		{
			for (int i = 0 ; i < 3;i++)
			{
				m_MJPGList.SetUnitText(101+i, L"", true);
			}
		}
	}

}

void CTelephoneDlg::SetFirstItem(CString info,bool bfresh)
{
	if (info.IsEmpty())
	{
		m_MJPGList.SetUnitText(100,L"",bfresh);
	}
	else
	{
		if (info.GetLength() > 25)
		{
			m_MJPGList.SetUnitFont(100, font_22);
			m_MJPGList.SetUnitText(100, info, bfresh);			
		}
		else
		{
			m_MJPGList.SetUnitFont(100, font_30);
			m_MJPGList.SetUnitText(100, info, bfresh);
		}	
	}

}

void CTelephoneDlg::Option()
{
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;

	if (main->m_phoneLine[main->m_nline].pFSM->getCurrentState() == 
		CMultimediaPhoneDlg::p3gsConnected && 
		main->m_phoneLine[(main->m_nline+1)%2].pFSM->getCurrentState() == 
		CMultimediaPhoneDlg::p3gsHold)
	{
		m_pTelePhoneOptionDlg->ShowOption(CTelePhoneOptionDlg::twoPhoneHoldAndActive);

	}
	else if (main->m_phoneLine[main->m_nline].pFSM->getCurrentState() == 
		CMultimediaPhoneDlg::p3gsHold && 
		main->m_phoneLine[(main->m_nline+1)%2].pFSM->getCurrentState() == 
		CMultimediaPhoneDlg::p3gsHold)
	{
		m_pTelePhoneOptionDlg->ShowOption(CTelePhoneOptionDlg::twoPhoneHoldAndHold);
	}
	else if (main->m_phoneLine[main->m_nline].pFSM->getCurrentState() == 
		CMultimediaPhoneDlg::p3gsConnected && 
		main->m_phoneLine[(main->m_nline+1)%2].pFSM->getCurrentState() == 
		CMultimediaPhoneDlg::p3gsConnected)
	{
		m_pTelePhoneOptionDlg->ShowOption(CTelePhoneOptionDlg::threePhone);	
	}
	else if (main->m_phoneLine[2].pFSM->getCurrentState() == CMultimediaPhoneDlg::pstnsConnected
		&& (main->m_phoneLine[0].pFSM->getCurrentState() == CMultimediaPhoneDlg::p3gsConnected ||
		main->m_phoneLine[1].pFSM->getCurrentState() == CMultimediaPhoneDlg::p3gsConnected ))//3g_1跟Pstn切换
	{
		m_pTelePhoneOptionDlg->ShowOption(CTelePhoneOptionDlg::pstnAnd3g);			
	}
	else if (main->m_phoneLine[main->m_nline].pFSM->getCurrentState() == 
		CMultimediaPhoneDlg::p3gsConnected)
	{
		m_pTelePhoneOptionDlg->ShowOption(CTelePhoneOptionDlg::onePhoneActive);	
	}
	else if (main->m_phoneLine[main->m_nline].pFSM->getCurrentState() == 
		CMultimediaPhoneDlg::p3gsHold)
	{
		m_pTelePhoneOptionDlg->ShowOption(CTelePhoneOptionDlg::onePhoneHold);

	}

}

void CTelephoneDlg::SwitchAnother3G()
{	
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	if (!main->FindActiveLine())//没有活动线路，共有信息全部初始化
	{
		CommonDataInit();
	}
	else //线路切换
	{			
		//只剩PSTN就不用切换，剩3G_1路的话需要切换
		for (int i = 0 ; i < 2 ; i++)
		{
			if (main->m_phoneLine[i].pFSM->getCurrentState() != 
				CMultimediaPhoneDlg::p3gsHangOff)
			{
				main->m_nline = i ;
				break;
			}
		}
		
		//3G没有活动的路，如果PSTN的路是活动的，转到PSTN这路
		int g3Num = 0 ;
		main->Find3gLineNum(g3Num);
		if (g3Num != 1 && main->m_phoneLine[2].pFSM->getCurrentState() != CMultimediaPhoneDlg::pstnsHangOff)
		{
			main->m_nline = 2;
		}
// 		if (g3Num != 2 && main->m_phoneLine[2].pFSM->getCurrentState() != CMultimediaPhoneDlg::pstnsHangOff)
// 		{
// 			main->m_nline = 2;
// 		}

		if (main->m_nline != 2)//切换到3G另一路
		{
			// 这路是保持的切换到Active
			if (main->m_phoneLine[main->m_nline].pFSM->getCurrentState() ==
				CMultimediaPhoneDlg::p3gsHold)
			{	
				Dprintf("Active\r\n");
				main->m_pATCommandWarp1->CHLD(2);
			}
			
			extern void GMute(BOOL isOn);
			GMute(false);
			
			Dprintf("Right_Clear\r\n");
			//只剩一路，剩下的信息初始化
			for (int i = 0 ; i < 3 ;i++)
			{
				m_MJPGList.SetUnitText(30+i,L"",true,true);
			}

			Dprintf("ShowMain\r\n");
			ShowMainInfo();//展示主界面信息

		}
		else
		{	
// 			if (m_bPstnReject)//pstn是拒接状态,挂断PSTN
// 			{
// 				PstnHangOff_(NULL);
// 			}
// 			else 
			if (main->m_phoneLine[2].pFSM->getCurrentState() == CMultimediaPhoneDlg::pstnsRing)
			{	
				main->m_nline = 0;
				ActivePstn();
			}
			
		}
		
	}
}

void CTelephoneDlg::ShowPstnInfo()//把PSTN的信息显示在主界面上
{
	
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;

	CString info = L"";
	if (!m_sPstnTel.empty())
	{
		info += L"(" + Util::StringOp::ToCString(m_sPstnTel) + L")";
	}
	/*if (!m_sPstnCity.empty())
	{
		info += L"(" + Util::StringOp::ToCString(m_sPstnCity) + L")";
	}*/
	info += L"(PSTN来电)";

	main->m_pTelphoneRingDlg->m_MJPGList.SetUnitText(6,info,true);

	/*if (m_sPstnTel.substr(0,2) == "1")
	{
		main->m_pTelphoneRingDlg->
			m_MJPGList.SetUnitText(100+1, Util::StringOp::ToCString(m_sPstnTel+m_sPstnCity), TRUE);
		
	}
	else
	{
		main->m_pTelphoneRingDlg->
			m_MJPGList.SetUnitText(100+2, Util::StringOp::ToCString(m_sPstnTel+m_sPstnCity), TRUE);
		
	}*/
	
	if (!m_sPstnTel.empty())
	{
		std::vector<boost::shared_ptr<Data::Contact> > result = FindCallContact(m_sPstnTel);
		if (!result.empty())
		{
			main->m_pTelphoneRingDlg->ShowContact(result[0], m_sPstnTel,100);		
		}
		else
		{
			main->m_pTelphoneRingDlg->ClearData(100);
		}
	}
	else
	{
		main->m_pTelphoneRingDlg->ClearData(100);
	}

}

void CTelephoneDlg::Show3gInfo(int const line)
{	
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;

	std::string city = main->m_phoneLine[line].city;
	std::string tel  = main->m_phoneLine[line].telnum;

	CString info = L"(" + Util::StringOp::ToCString(tel) + L")";
	/*if (!city.empty())
	{
		info += L"("+Util::StringOp::ToCString(city)+L")";
	}*/
	info += L"(3G来电)";
	
	main->m_pTelphoneRingDlg->m_MJPGList.SetUnitText(6,info,true);

/*	if (tel.substr(0,1) == "1")
	{	
		main->m_pTelphoneRingDlg->m_MJPGList.SetUnitText(101, L"", TRUE);
		main->m_pTelphoneRingDlg->
			m_MJPGList.SetUnitText(101, Util::StringOp::ToCString(tel+city), TRUE);
		
	}
	else
	{
		main->m_pTelphoneRingDlg->
			m_MJPGList.SetUnitText(102, Util::StringOp::ToCString(tel+city), TRUE);
		
	}*/

	std::vector<boost::shared_ptr<Data::Contact> > result = FindCallContact(tel);
	if (!result.empty())
	{		
		if (main->m_phoneLine[line].b3gFisrtRing)
		{
			main->m_pTelphoneRingDlg->ShowContact(result[0], tel);
		}
	}
	else
	{
		main->m_pTelphoneRingDlg->ClearData(100);
	}

}

void CTelephoneDlg::ActivePstn()
{	
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	main->phone_->Free(0);
	m_bPstnFirstRing = true;
	main->m_pTelphoneRingDlg->RightUnitShow(false);
	ShowPstnInfo();
	
	ShowWindow(SW_HIDE);
	main->PopbackIcon(this);
	
	main->m_pTelphoneRingDlg->m_MJPGList.SetUnitIsShow(3,false,true);

	main->m_pTelphoneRingDlg->ShowWindow_(SW_SHOW);
	CWnd *p = main->m_pTelphoneRingDlg;
	main->AddIcon(Allicon[0],p,false);
	
	main->phone_->PSTNRingMute(FALSE);//
}

void CTelephoneDlg::SetContactInfoOK(int const index)
{
	m_spContactInfo[index]->type(Data::citInOk);
}
