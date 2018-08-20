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
	: CCEDialog(CTelephoneDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTelephoneDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bRing = FALSE;
	m_strTelStatus = "";
	m_nTelStatus = -1;
	m_bTelUsing = FALSE;
	m_bFirstConnect = TRUE;

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

BEGIN_MESSAGE_MAP(CTelephoneDlg, CCEDialog)
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
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

void CTelephoneDlg::OnTelStatus(WPARAM w, LPARAM l)
{	
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	char *chTel_status[] = {"对方忙!", "通话中...", "对方已接听!", "通话保持!", "正在呼叫...", "对方振铃...", "等待...", "无拨号音!", "无应答!", "呼叫结束!","网络拥塞!", "对方挂机", "呼叫受限", "来电呼入..."};
	switch(w)
	{
	case TEL_FROMBUY: //对方忙
		SetTimer(IDT_TEL_EXIT, 4000, NULL);
		break;

	case TEL_FROMCONNECTED:  //通话联通状态
		if(!m_bFirstConnect)
		{	
			main->m_phoneLine[(int)l-1].pFSM->fireEvent(CMultimediaPhoneDlg::p3geConnected,NULL);
			m_nTelStatus = w;
			return;
		}
		m_bFirstConnect = FALSE;
		::KillTimer((theApp.m_pMainWnd)->m_hWnd, ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_uiKeyTimer);
		::SetTimer((theApp.m_pMainWnd)->m_hWnd, ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_uiKeyTimer, 50, 0);
		break;

	case TEL_FROMCONNECT: //对方接听
		{
			main->m_phoneLine[(int)l-1].pFSM->fireEvent(CMultimediaPhoneDlg::p3geConnected,NULL);
		}
		break;

	case TEL_FROMHELD:       //保持状态
		main->m_phoneLine[(int)l-1].pFSM->fireEvent(CMultimediaPhoneDlg::p3geHold,NULL);
		break;

	case TEL_FROMDIALING:    //正在拨打状态 
		main->m_phoneLine[(int)l-1].pFSM->fireEvent(CMultimediaPhoneDlg::p3geDialing,NULL);
		break;

	case TEL_FROMALERTING:   //对方振铃状态
		main->m_phoneLine[(int)l-1].TelStatus = w;
		break;

	case TEL_FROMWAITING: //对方呼入
		break;

	case TEL_FROMNODIALTONE: 
		SetTimer(IDT_TEL_EXIT, 4000, NULL);
		break;

	case TEL_FROMNOANSWER:   //无应答
		SetTimer(IDT_TEL_EXIT, 4000, NULL);
		break;

	case TEL_FROMNOCARRIER:  //连接不能被建立
		SetTimer(IDT_TEL_EXIT, 4000, NULL);
		break;

	case TEL_FROMCONGESTION:  //网络拥塞
		SetTimer(IDT_TEL_EXIT, 4000, NULL);
		break;

	case TEL_FROMOPPHUNGUP:   //对方挂机
		SetTimer(IDT_TEL_EXIT, 4000, NULL);
		break;

	case TEL_FROMODB:         //呼叫限制
		SetTimer(IDT_TEL_EXIT, 4000, NULL);
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
	
	if ( 1 == l || 0 == l)//一路
	{
		m_MJPGList.SetUnitText(400, m_strTelStatus, TRUE);
	}
	else if ( 2 == l)//二路
	{

	}
	else if( 3 == l)//
	{
		
	}
	
	KillTimer(IDT_TEL_STATUS);
	SetTimer(IDT_TEL_STATUS, 800, NULL);
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
		std::vector<std::pair<std::string, std::string> > ms = pMainDlg->m_pSettingDlg->m_pTempSetting->speedDials();
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
			icon = Allicon[2];
			pMainDlg->AddIcon(icon);

			break;

		case 13://通话记录
			pMainDlg->m_pContactInfoDlg->ShowRightBtn(false);
			pMainDlg->m_pContactInfoDlg->ShowWindow_();
			icon = Allicon[3];
			pMainDlg->AddIcon(icon);
			break;

		case 14://二次拨号
			break;
			
		case 20://隐藏
			OnButtonTelephoneHide();
			pMainDlg->PopbackIcon();
			break;

		case 21://切换
			break;

		case 22://挂机
			HandleOff();
			break;
		
		case 23:
			break;

		case 40:
		case 41:
		case 42:
		case 43:
		case 44:
		case 45:
			SetVolume(w-40);
			break;

		case 50://设置声音
			VolumeSwitch();
			break;
	
		case 1000:
			ShowWindow(SW_HIDE);		
//			pMainDlg->m_pTelephoneDlg->HangOff_(NULL);
			pMainDlg->PopbackIcon();
			HandleOff();
			break;

		default:
		break;
	}

}
BOOL CTelephoneDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_uiIgnoreRingCount = 1;
	m_uiIPstngnoreRingCount = 1;
	std::string strTemp;
	CString str;
	
	m_MJPGList.Create(L"", WS_VISIBLE|WS_CHILD, CRect(0, 0, 800, 423), this);
	m_MJPGList.SetCurrentLinkFile(".\\adv\\mjpg\\k5\\中文\\电话显示.xml");
	m_MJPGList.SetMJPGRect(CRect(0, 0, 800, 423));
	
	MoveWindow(0,57,800,423);

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
	m_uiTelephoneTimer = 1;
	m_uiTelephoneSecond = 0;
	m_uTelSecondOff = 0;
	m_uiRecordTimer = 2;
	m_uiRecordSecond = 0;
	m_uiRingCount = 0;
	m_uiTipTimer = 3;
	m_uiInNoCount = 0;
	m_uiRecordCound = 0;
	m_bHasCallID = FALSE;
	m_bPlayingLeaveWord = FALSE;
	m_bFirwall = FALSE;
	m_bRingPlayed = FALSE;	
	
	//pstn
	m_uiPstnRingCount = 0 ;
	m_uiPstnTelephoneTimer = 100;
	m_uPstnTelSecondOff = 0;

	m_bPstnFirwall = FALSE;
	m_bPstnHasCallID = FALSE;
	m_bPstnRingPlayed = FALSE;

	m_spContactInfo = boost::shared_ptr<Data::ContactInfo>();
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

	if (main->m_pTelphoneDialDlg->IsWindowVisible())
	{
		main->m_pTelphoneDialDlg->ShowWindow(SW_HIDE);	
		main->PopbackIcon();
	}

	ShowWindow_(FALSE);
	if(!m_bTelUsing)
	{
		m_bTelUsing = FALSE;
		m_nTelStatus = -1;
		m_bFirstConnect = TRUE;
		m_strTelStatus = "";
		KillTimer(IDT_TEL_STATUS);
		KillTimer(IDT_TEL_EXIT);
//		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pFSM->fireEvent(CMultimediaPhoneDlg::teHangOff, NULL);
		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_phoneLine[0].pFSM->fireEvent(CMultimediaPhoneDlg::p3geHangOff, NULL);
	
	}
}

void CTelephoneDlg::OnButtonTelephoneNote()
{
//	((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pNotebookDlg->m_bIsOPenTel = TRUE;
//	((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pNotebookDlg->ShowWindow(SW_SHOW);
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	main->m_pMainDlg->m_mainLunarderDlg_->OnClickMJPG(7,0); 
	main->m_pMainDlg->m_mainLunarderDlg_->ShowWindow(SW_SHOW);

}

std::string CTelephoneDlg::GetSoundPath(void)
{
	path = ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pTempSetting->soundPath();
	
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
// 	CString caption;
// 	m_btnRecord.GetWindowText(caption);
// 	if (caption == m_strRecord)
	if (!m_bRecording)
	{
		GetSoundPath();

		ULARGE_INTEGER freeBytes;
		ULARGE_INTEGER totalBytes;
		GetDiskFreeSpaceEx(Util::StringOp::ToCString(path), &freeBytes, &totalBytes, NULL);

		int secondBytes = SECONDBYTES8;
		if (((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pSettingDlg->m_pTempSetting->isDeleteProtect())
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
				((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pWarningNoFlashDlg->ShowWindow_(SW_SHOW);
				return;
			}
			
			//m_uiRemainRecordSecond = (totalBytes.QuadPart - MINFREEBYTES) / SECONDBYTES + 1;
			m_uiRemainRecordSecond = (freeBytes.QuadPart - (totalBytes.QuadPart * 0.1)) / secondBytes + 1;
		}
		else
		{
			if (freeBytes.QuadPart < secondBytes*2)
			{
				((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pWarningNoFlashDlg->ShowWindow_(SW_SHOW);
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
			
			if (((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pSettingDlg->m_pTempSetting->isDeleteProtect())
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
			m_spSoundSegment->contactInfoId(m_spContactInfo->id());
			m_spSoundSegment->telephoneNumber(m_spContactInfo->telephoneNumber());
			m_spSoundSegment->name(m_spContactInfo->name());
			if (m_spContactInfo->type() == Data::citInNo)
			{
				m_spSoundSegment->type(Data::sstLeaveWord);
			}
			else
			{
				m_spSoundSegment->type(Data::sstTeleRecord);
			}
			m_spSoundSegment->Insert();
			m_spContactInfo->isSound(true);
			m_spContactInfo->Update();

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
				
				if (((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pTempSetting->isMustRecord())
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
//	((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->HungOff();
	m_nTelStatus = -1;
	m_bFirstConnect = TRUE;
	m_strTelStatus = "";
	KillTimer(IDT_TEL_STATUS);
	KillTimer(IDT_TEL_EXIT);

	KillTimer(5);
	KillTimer(IDC_TELDLGSHOW);
	m_uiIgnoreRingCount = 1;
	
	m_uiIPstngnoreRingCount = 1;

 	((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->StopRing(true);
	waveInMessage(0, WAV_SPEAKER_ONLY, 0, 0); //20090324

	KillTimer(m_uiTipTimer);
	KillTimer(m_uiTelephoneTimer);
	KillTimer(m_uiPstnTelephoneTimer);

	if (m_bRecording)
	{
		OnButtonTelephoneRecord();
	}
	
	if (m_spContactInfo)
	{
		m_spContactInfo->duration(m_uiTelephoneSecond);
		if (m_spContactInfo->type() == Data::citInNo)
		{
			m_spContactInfo->played(false);
		}
		
		((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pContactInfoDlg->SaveContactInfo(m_spContactInfo);
		
		if (!m_vSoundSegment.empty())
		{
			for (std::vector<boost::shared_ptr<Data::SoundSegment> >::iterator iter = m_vSoundSegment.begin(); iter != m_vSoundSegment.end(); ++iter)
			{
//				if ((*iter)->duration() > 1)
				{
					(*iter)->telephoneNumber(m_spContactInfo->telephoneNumber());
					(*iter)->name(m_spContactInfo->name());
					(*iter)->contactInfoId(m_spContactInfo->id());
					(*iter)->contactId(m_spContactInfo->contactId());

					if (m_spContactInfo->type() == Data::citInNo)
					{
						(*iter)->type(Data::sstLeaveWord);
						(*iter)->played(false);

						//lxz 20090325
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
		}
		m_vSoundSegment.clear();

		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactInfoDlg->ResetTypeInfo();
		if (m_spContactInfo->type() == Data::citInNo)
		{
			::SendMessage(((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pMainDlg->GetSafeHwnd(), WM_TELNOTIFY, 1, 1);
		}

	}

	m_spContactInfo = boost::shared_ptr<Data::ContactInfo>();
	
	m_uiTelephoneSecond = 0;
	m_uiRingCount = 0;

	m_uiPstnRingCount = 0;

	m_bHasCallID = FALSE;
	m_bPlayingLeaveWord = FALSE;
	m_bFirwall = FALSE;
	m_sTipFilename = "";
	m_sRingFilename = "";
	m_bRecording = FALSE;
	m_bRingPlayed = FALSE;
	
	m_bPstnRingPlayed = FALSE;

	m_MJPGList.SetUnitText(100, "", FALSE);
	m_MJPGList.SetUnitText(101, "", FALSE);
	m_MJPGList.SetUnitText(102, "", FALSE);
	m_MJPGList.SetUnitText(103, "", FALSE);

	m_MJPGList.SetUnitText(107, "", FALSE);

	Dprintf("TelDlg hide\n");
	m_MJPGList.Invalidate();
	SetTimer(IDC_TELDLGHIDE, 10, NULL);
	
	((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->PhoneDialTone(FALSE, NULL);
    
	//lxz 20090207
	((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pMainDlg->SendMessage(WM_PLAYVIDEO, 1, 0);//恢复视频
	((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pMainDlg->m_mainMp3Dlg_->SendMessage(WM_OUTEVENT, 0, 1);
	
	((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->Hand(0);
	((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->Free(0);
	
	Telephone::TelephoneWarp::GetTelephoneWarp()->PSTNHangoff();


	// add by qi 2009_11_05
	CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)(theApp.m_pMainWnd);
	CString icon ;
	main->m_MJPGList.SetUnitIsShow(15,false,true);
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

	KillTimer(SHOW_10CONTACTINFO_TIMER);//
	main->m_pTelphoneDialDlg->KillTimer(IDT_AUTO_DIAL);
	
	main->m_pTelphoneRingDlg->ClearData(100);
	main->m_pTelphoneRingDlg->ClearData(200);

}

void CTelephoneDlg::HangOn_(void* param)
{
	if(((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSoundDlg->m_pRecordSoundDlg->IsWindowVisible())
		((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSoundDlg->m_pRecordSoundDlg->CloseSound();
	if(((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSoundDlg->m_pPlaySoundDlg->IsWindowVisible())
		((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSoundDlg->m_pPlaySoundDlg->CloseSound();

	Dprintf("TelDlg show1\n");
		
	SipShowIM(SIPF_OFF);
	SetTimer(IDC_TELDLGSHOW, 10, NULL);
	
	//是软件启动还是硬件启动
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd ;
	if (!main->m_pTelphoneDialDlg->m_bSoftware)
	{
		((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->PhoneDialTone(TRUE, "dialing");	
	}

	//lxz 20100108
	extern BOOL gIsHandSet; 
	if(gIsHandSet)
	{
		extern void GMute(BOOL isOn);
		GMute(TRUE);
	}
	
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

	number = m_MJPGList.GetUnitText(100);
	number += char(param);
	if (number.GetLength() > 20)
	{
		m_MJPGList.SetUnitFont(100, font_16);
		m_MJPGList.SetUnitText(100, number, flag);			
	}
	else
	{
		m_MJPGList.SetUnitFont(100, font_30);
		m_MJPGList.SetUnitText(100, number, flag);
	}

	m_MJPGList.SetUnitIsShow(19, TRUE, flag);  //拨打

 	if (!m_spContactInfo)
 	{
 		m_spContactInfo = boost::shared_ptr<Data::ContactInfo>(new Data::ContactInfo);
		m_spContactInfo->type(Data::citOut);
		m_spContactInfo->startTime(CTime::GetCurrentTime());
		m_spContactInfo->Insert();
	}
	m_spContactInfo->telephoneNumber(Util::StringOp::FromCString(number));
	m_sTelephoneNumber = Util::StringOp::FromCString(number);
	

//	if(m_nTelStatus == TEL_FROMCONNECTED) //正在通话
	if(main->m_phoneLine[0].pFSM->getCurrentState() 
		== CMultimediaPhoneDlg::p3gsConnected)
	{
		char code[2] = {0};
		code[0] = char(param);
    	((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->SubDial(char(param));
		((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->PhoneDialTone(TRUE, code);
		return;
	}

	char code[2] = {0};
	code[0] = char(param);
	((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->PhoneDialTone(TRUE, code);
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
	if (number.GetLength() > 20)
	{
		m_MJPGList.SetUnitFont(100, font_16);
		m_MJPGList.SetUnitText(100, number, flag);			
	}
	else
	{
		m_MJPGList.SetUnitFont(100, font_30);
		m_MJPGList.SetUnitText(100, number, flag);
	}
	
//	if(m_nTelStatus == TEL_FROMCONNECTED) //正在通话
	if(main->m_phoneLine[0].pFSM->getCurrentState() 
		== CMultimediaPhoneDlg::p3gsConnected)
	{
		char code[2] = {0};
		code[0] = char(param);
    	((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->SubDial(char(param));
		((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->PhoneDialTone(TRUE, code);
		
		extern void GMute(BOOL isOn);
		GMute(FALSE);
		
	}

}

void CTelephoneDlg::Connect_(void* param)
{
	((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->HungOn(TRUE);    //lxz 20091110

	m_MJPGList.SetUnitIsShow(9, FALSE);
	m_MJPGList.SetUnitIsShow(19, FALSE);
	m_MJPGList.SetUnitIsShow(3, TRUE);
	
	if (m_bPlayingLeaveWord)
	{
		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_phoneLine[0].pFSM->
			setStartState(CMultimediaPhoneDlg::p3gsRing);
		return;
	}

	((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->StopRing(true);
	extern void GMute(BOOL isOn);
	if (!((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->Hand())
	{
		GMute(FALSE);
	}


	KillTimer(IDT_STOPAUTORECORDE_TIME);

	if (((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pTempSetting->isMustRecord())
	{
		if (!m_bRecording)
		{

		}
		m_MJPGList.SetUnitIsShow(8, FALSE);
		m_MJPGList.SetUnitIsShow(2, FALSE);
	}
	else
	{
		if (m_bRecording)
		{
			OnButtonTelephoneRecord();
		}
		else
		{
			m_MJPGList.SetUnitIsShow(8, FALSE);
			m_MJPGList.SetUnitIsShow(2, TRUE);
		}	
	}


	m_MJPGList.SetUnitIsShow(7, TRUE);
	m_MJPGList.SetUnitIsShow(5, FALSE);
	m_MJPGList.SetUnitIsShow(6, FALSE);
	m_MJPGList.SetUnitIsShow(4, FALSE);

	m_MJPGList.SetUnitText(107, "", FALSE);


	if (m_spContactInfo->type() == Data::citOut)
	{
		m_spContactInfo->telephoneNumber(m_sTelephoneNumber);
		m_spContactInfo->startTime(CTime::GetCurrentTime());
		if (m_sTelephoneNumber.size() >= 3)
		{
			//std::vector<boost::shared_ptr<Data::Contact> > result = FindCallContact(Util::StringOp::FromCString(number));
			std::vector<boost::shared_ptr<Data::Contact> > result = FindCallContact(m_sTelephoneNumber);
			if (!result.empty())
			{
				//ShowContact(result[0], Util::StringOp::FromCString(number));
				ShowContact(result[0], m_sTelephoneNumber);
			}
		}
	}
	else
	{	
		if (m_bRing && !IsWindowVisible())
		{
			CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd ;
			main->m_pTelphoneRingDlg->ShowWindow_(SW_HIDE);
			std::string type = "(3G来电)";
			m_MJPGList.SetUnitText(100, Util::StringOp::ToCString(m_sTel+m_sCity+type), TRUE);
			std::vector<boost::shared_ptr<Data::Contact> > result = FindCallContact(m_sTel);
			if (!result.empty())
			{
				ShowContact(result[0], m_sTel);
			}

			ShowWindow_(SW_SHOW);
		}
		m_spContactInfo->type(Data::citInOk);

	}

	SetTimer(m_uiTelephoneTimer, 1000, 0);//lxz test
	CTimeSpan ts;
	ts = CTime::GetCurrentTime() - m_spContactInfo->startTime();
	m_uTelSecondOff = ts.GetTotalSeconds();

}

void CTelephoneDlg::Ring_(void* param)
{	
	CMultimediaPhoneDlg * main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;

	m_bTelUsing = TRUE;
	main->m_pSoundDlg->m_pRecordSoundDlg->CloseSound();
	main->m_pSoundDlg->m_pPlaySoundDlg->OnButtonClose();
	
	extern BOOL _test_call;
	if(_test_call)
	{
		HandleOn();
		Sleep(1000);
		OnButtonTelephoneRecord();
		SetTimer(10, 30000, NULL);

	}
	
	m_bRing = TRUE;

	if (!m_spContactInfo)
	{
		m_spContactInfo = boost::shared_ptr<Data::ContactInfo>(new Data::ContactInfo);
		m_spContactInfo->startTime(CTime::GetCurrentTime());
		m_spContactInfo->type(Data::citInNo);
		m_spContactInfo->Insert();
	}	
	
	if (!main->m_pTelphoneRingDlg->IsWindowVisible() && (m_uiRingCount == 0))
	{
		//恢复黑屏 lxz 20090304
		((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->ReStoreBackLight();
						
		SipShowIM(SIPF_OFF);
				
		//add by qi 2009_11_05
		main->m_pTelphoneRingDlg->ShowWindow_(SW_SHOW);
		
		CString icon = Allicon[0];
		main->AddIcon(icon);

		KillTimer(IDC_TELDLGHIDE);
		
		main->m_pMainDlg->m_mainMp3Dlg_->OnTimer(1002);//重复发送了暂停消息
		main->m_pTelphoneRingDlg->InitData();

	}
	
	//两路同时来电
	if (main->m_phoneLine[2].pFSM->getCurrentState() == CMultimediaPhoneDlg::pstnsRing)
	{
		if (main->m_pTelphoneRingDlg->m_MJPGList.GetUnitIsShow(100))
		{
			main->m_pTelphoneRingDlg->m_MJPGList.SetUnitText(5,L"PSTN来电...",true);	
		}
		else if (main->m_pTelphoneRingDlg->m_MJPGList.GetUnitIsShow(200))
		{
			main->m_pTelphoneRingDlg->m_MJPGList.SetUnitText(5,L"3G来电...",true);		
		}
	}
	

	if (m_bFirwall)
	{
		return;
	}

	++m_uiRingCount;
	
	if ((!m_bHasCallID) && (m_uiRingCount == (m_uiIgnoreRingCount + 1)))
	{
		if (main->m_pSettingDlg->m_pTempSetting->isFirewall())
		{
			int firetype = main->m_pSettingDlg->m_pTempSetting->firewallType();
			if (firetype == 1 || firetype == 2 || firetype == 3)
			{
				m_bFirwall = TRUE;
				return;
			}
			
		}			
		
		if (((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pTempSetting->isUseSpecRing())
		{
			std::string ringname = main->m_pSettingDlg->m_pTempSetting->defaultRingFilename();
			CFileStatus   status;
			if (ringname != "/flashdrv/my_ring/" && CFile::GetStatus(LPCTSTR(Util::StringOp::ToCString(ringname)),status))
			{
				m_sRingFilename = ringname;
			}

		}

	}

	if (!m_bRingPlayed)
	{
		if (m_bHasCallID || (m_uiRingCount > m_uiIgnoreRingCount))
		{
			m_bRingPlayed = TRUE;
		
			if (m_sRingFilename != "")
			{					
				main->SMSSpecRing(2, m_sRingFilename.c_str());				
			}
			else
			{
				main->phone_->OpenTelRing();
			}
			
		}
	}

	if (main->m_pSettingDlg->m_pTempSetting->isAutoReply())
	{
		if (m_uiRingCount - m_uiIgnoreRingCount ==  main->m_pSettingDlg->m_pTempSetting->autoReplyRingCount())
		{
			if (m_sTipFilename == "")
			{
				m_sTipFilename = main->m_pSettingDlg->m_pTempSetting->defaultTipFilename();
			}
			CFileStatus   status;
			if(m_sTipFilename == "" || m_sTipFilename == "默认" || (!CFile::GetStatus(LPCTSTR(Util::StringOp::ToCString(m_sTipFilename)),status)))
				m_sTipFilename = "\\hive\\tip1.wav";
			if (CFile::GetStatus(LPCTSTR(Util::StringOp::ToCString(m_sTipFilename)),status))
			{
				CString s = m_sTipFilename.c_str();
				if(s.Find(L".wav") > 0)				
				{
					::KillTimer((theApp.m_pMainWnd)->m_hWnd, ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_uiRingTimer);
					((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->StopRing(true);
					((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->OpenTelRing(FALSE);
					m_bPlayingLeaveWord = TRUE;
					HangOnToRecord();
					((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->SetMsgWnd(NULL);
					((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->StartRing((LPTSTR)(LPCTSTR)Util::StringOp::ToCString(m_sTipFilename), 1);
					SetTimer(m_uiTipTimer, 5 * 1000, 0);
				}
				else if(s.Find(L".spx") > 0)
				{
					::KillTimer((theApp.m_pMainWnd)->m_hWnd, ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_uiRingTimer);
					((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->StopRing(true);
					((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->OpenTelRing(FALSE);
					m_bPlayingLeaveWord = TRUE;
					HangOnToRecord();
					((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->SetMsgWnd(NULL);
					std::vector<boost::shared_ptr<Data::SoundSegment> > result = Data::SoundSegment::GetFromDatabase("type = 2 AND isTip = 1 AND filename = '" + m_sTipFilename + "'");	
					if (result.size() > 0)
					{
						int seconds = result[0]->duration();
						seconds += 2;
					
						m_pOggCodec->StartDecode(m_sTipFilename);
						SetTimer(m_uiTipTimer, seconds * 1000, 0);
					}
					else
					{
						SetTimer(m_uiTipTimer, 2 * 1000, 0);
					}
				}
			}
		}		
	}

}

void CTelephoneDlg::pstnRing_(void* param)
{
	((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSoundDlg->m_pRecordSoundDlg->CloseSound();
	((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSoundDlg->m_pPlaySoundDlg->OnButtonClose();
	
	m_bPstnRing = TRUE ;

	if (!m_spPstnContactInfo)
	{
		m_spPstnContactInfo = boost::shared_ptr<Data::ContactInfo>(new Data::ContactInfo);
		m_spPstnContactInfo->startTime(CTime::GetCurrentTime());
		m_spPstnContactInfo->type(Data::citInNo);
		m_spPstnContactInfo->Insert();
	}	
	
	CMultimediaPhoneDlg * main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	if (!main->m_pTelphoneRingDlg->IsWindowVisible() && (m_uiPstnRingCount == 0))
	{
		//恢复黑屏 lxz 20090304
		((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->ReStoreBackLight();						
		SipShowIM(SIPF_OFF);				
		main->m_pTelphoneRingDlg->ShowWindow_(SW_SHOW);
		
		CString icon = Allicon[0];
		main->AddIcon(icon);

		KillTimer(IDC_TELDLGHIDE);
		main->m_pMainDlg->m_mainMp3Dlg_->OnTimer(1002);//重复发送了暂停消息
		main->m_pTelphoneRingDlg->InitData();
	}
	
	//两路同时来电
	if (main->m_phoneLine[0].pFSM->getCurrentState() == CMultimediaPhoneDlg::p3gsRing)
	{
		if (main->m_pTelphoneRingDlg->m_MJPGList.GetUnitIsShow(100))
		{
			main->m_pTelphoneRingDlg->m_MJPGList.SetUnitText(5,L"PSTN来电...",true);	
		}
		else if (main->m_pTelphoneRingDlg->m_MJPGList.GetUnitIsShow(200))
		{
			main->m_pTelphoneRingDlg->m_MJPGList.SetUnitText(5,L"3G来电...",true);		
		}

	}

//	pstn 来电
// 	if (m_sPstnTel.empty())
// 	{
// 		main->m_pTelphoneRingDlg->m_MJPGList.SetUnitText(201,L"(PSTN来电)",true);	
// 	}

	if (m_bPstnFirwall)
	{
		return;
	}

	++m_uiPstnRingCount;
	
	//没有callid
	if ((!m_bPstnHasCallID) && (m_uiPstnRingCount == (m_uiIPstngnoreRingCount + 1)))
	{
		if (main->m_pSettingDlg->m_pTempSetting->isFirewall())
		{
			int firetype = main->m_pSettingDlg->m_pTempSetting->firewallType();
			if (firetype == 1 || firetype == 2 || firetype == 3)
			{
				m_bPstnFirwall = TRUE;
				return;
			}			
		}			
		
		if (((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pTempSetting->isUseSpecRing())
		{
			std::string ringname = main->m_pSettingDlg->m_pTempSetting->defaultRingFilename();
			CFileStatus   status;
			if (ringname != "/flashdrv/my_ring/" && CFile::GetStatus(LPCTSTR(Util::StringOp::ToCString(ringname)),status))
			{
				m_sPstnRingFilename = ringname;
			}

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
				main->phone_->OpenTelRing();
			}
			
		}

	}

// 	if (main->m_pSettingDlg->m_pTempSetting->isAutoReply())
// 	{
// 		if (m_uiPstnRingCount - m_uiIPstngnoreRingCount ==  main->m_pSettingDlg->m_pTempSetting->autoReplyRingCount())
// 		{
// 			if (m_sTipFilename == "")
// 			{
// 				m_sTipFilename = main->m_pSettingDlg->m_pTempSetting->defaultTipFilename();
// 			}
// 			CFileStatus   status;
// 			if(m_sTipFilename == "" || m_sTipFilename == "默认" || (!CFile::GetStatus(LPCTSTR(Util::StringOp::ToCString(m_sTipFilename)),status)))
// 				m_sTipFilename = "\\hive\\tip1.wav";
// 			if (CFile::GetStatus(LPCTSTR(Util::StringOp::ToCString(m_sTipFilename)),status))
// 			{
// 				CString s = m_sTipFilename.c_str();
// 				if(s.Find(L".wav") > 0)				
// 				{
// 					::KillTimer((theApp.m_pMainWnd)->m_hWnd, ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_uiPSTNRingTimer);
// 					((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->StopRing(true);
// 					((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->OpenTelRing(FALSE);
// 					m_bPlayingLeaveWord = TRUE;
// 					HangOnToRecord();
// 					((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->SetMsgWnd(NULL);
// 					((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->StartRing((LPTSTR)(LPCTSTR)Util::StringOp::ToCString(m_sTipFilename), 1);
// 					SetTimer(m_uiTipTimer, 5 * 1000, 0);
// 				}
// 				else if(s.Find(L".spx") > 0)
// 				{
// 					::KillTimer((theApp.m_pMainWnd)->m_hWnd, ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_uiPSTNRingTimer);
// 					((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->StopRing(true);
// 					((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->OpenTelRing(FALSE);
// 					m_bPlayingLeaveWord = TRUE;
// 					HangOnToRecord();
// 					((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->SetMsgWnd(NULL);
// 					std::vector<boost::shared_ptr<Data::SoundSegment> > result = Data::SoundSegment::GetFromDatabase("type = 2 AND isTip = 1 AND filename = '" + m_sTipFilename + "'");	
// 					if (result.size() > 0)
// 					{
// 						int seconds = result[0]->duration();
// 						seconds += 2;
// 					
// 						m_pOggCodec->StartDecode(m_sTipFilename);
// 						SetTimer(m_uiTipTimer, seconds * 1000, 0);
// 					}
// 					else
// 					{
// 						SetTimer(m_uiTipTimer, 2 * 1000, 0);
// 					}
// 				}
// 			}
// 		}		
// 	}

}

void CTelephoneDlg::CallID_(void* param)
{	
	CMultimediaPhoneDlg * main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	
	m_bTelUsing = TRUE;
	((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSoundDlg->m_pRecordSoundDlg->CloseSound();
	((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSoundDlg->m_pPlaySoundDlg->OnButtonClose();
	
	if (main->m_phoneLine[2].pFSM->getCurrentState() == CMultimediaPhoneDlg::pstnsHangOff)
	{
		main->m_pTelphoneRingDlg->HideContact(200);
	}

	//来callID
	if(m_bHasCallID)
	{
		HangOff_(NULL);
	}
	
	m_bHasCallID = TRUE;
	m_bRing = TRUE;
	
	if (!m_spContactInfo)
	{
		m_spContactInfo = boost::shared_ptr<Data::ContactInfo>(new Data::ContactInfo);
		m_spContactInfo->startTime(CTime::GetCurrentTime());
		m_spContactInfo->type(Data::citInNo);
		m_spContactInfo->Insert();
	}
	
	CALLID_INFO* info = (CALLID_INFO*)param;
	std::string tel = info->number;
	Dprintf("Call ID telephone number:\n");
	Dprintf(tel.c_str());
	std::string mobile;
	std::string city;
	std::string code;
	if (((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pTempSetting->thisTelephoneNumber().number().length() >= 2)
	{
		code = ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pTempSetting->thisTelephoneNumber().number();
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
		
	std::string type = "(3G来电)";
//	m_MJPGList.SetUnitText(100, Util::StringOp::ToCString(tel+city+type), TRUE);
	m_sTel	= tel ;
	m_sCity	= city; 
	if (tel.substr(0,2) == "13")
	{
		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pTelphoneRingDlg->
			m_MJPGList.SetUnitText(101, Util::StringOp::ToCString(tel+city+type), TRUE);
		
	}
	else
	{
		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pTelphoneRingDlg->
			m_MJPGList.SetUnitText(102, Util::StringOp::ToCString(tel+city+type), TRUE);
		
	}
	
	if (tel == "")
	{
		if (((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pTempSetting->isFirewall())
		{
			int firetype = ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pTempSetting->firewallType();
			if (firetype == 1 || firetype == 2 || firetype == 3)
			{
				m_bFirwall = TRUE;
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
			((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pTelphoneRingDlg->ShowContact(result[0], tel);

			if (((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pTempSetting->isFirewall())
			{
				int firetype = ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pTempSetting->firewallType();
				Data::ContactType type = result[0]->type();
				
				if (((firetype == 0) && (type == Data::ctBlacklist))
					|| ((firetype == 1) && (type == Data::ctBlacklist))
					|| ((firetype == 2) && (type != Data::ctVip))
					|| (firetype == 3))
				{
					m_bFirwall = TRUE;
				}
			}
			
			if (!m_bFirwall)
			{
				if (result[0]->group())
				{
					std::string tipname = result[0]->group()->tipName();
					if (tipname == Data::LanguageResource::Get(Data::RI_CARD_DEFAULT))
					{
						tipname = ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pTempSetting->defaultTipFilename();
					}
					
					CFileStatus   status;
					if (CFile::GetStatus(LPCTSTR(Util::StringOp::ToCString(tipname)),status))
					{
						m_sTipFilename = tipname;
					}
					
					std::string ringname = result[0]->group()->ringName();
					if (ringname == Data::LanguageResource::Get(Data::RI_CARD_DEFAULT))
					{
						ringname = ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pTempSetting->defaultRingFilename();
					}
					
					if (ringname != "/flashdrv/my_ring/" && CFile::GetStatus(LPCTSTR(Util::StringOp::ToCString(ringname)),status))
					{
						m_sRingFilename = ringname;
					}
				}
				else			//lxz 20090330
				{
					std::string ringname = "";
					CFileStatus   status;
					if(((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pTempSetting->isUseSpecRing())
					{
						ringname = ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pTempSetting->defaultRingFilename();
					}
					
					if (ringname != "/flashdrv/my_ring/" && CFile::GetStatus(LPCTSTR(Util::StringOp::ToCString(ringname)), status))
					{
						m_sRingFilename = ringname;
					}

				}
			}
			
		}
		else
		{
			if (((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pTempSetting->isFirewall())
			{
				int firetype = ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pTempSetting->firewallType();
				if (firetype == 1 || firetype == 2 || firetype == 3)
				{
					m_bFirwall = TRUE;
				}
			}

			//lxz 20090330
			std::string ringname = "";
			CFileStatus   status;
			if(((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pTempSetting->isUseSpecRing())
			{
				ringname = ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pTempSetting->defaultRingFilename();
			}
			
			if (ringname != "/flashdrv/my_ring/" && CFile::GetStatus(LPCTSTR(Util::StringOp::ToCString(ringname)), status))
			{
				m_sRingFilename = ringname;
			}
		}
	}

	m_spContactInfo->telephoneNumber(Data::TelephoneNumber(tel));
	m_spContactInfo->Update();
	if (m_uiRingCount > 0)
	{
		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_phoneLine[0].pFSM->
			fireEvent(CMultimediaPhoneDlg::p3geRing, NULL);
		m_uiIgnoreRingCount = 2;

	}
}

void CTelephoneDlg::pstnCallID_(void* param)
{	
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	m_bTelUsing = TRUE;
	((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSoundDlg->m_pRecordSoundDlg->CloseSound();
	((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSoundDlg->m_pPlaySoundDlg->OnButtonClose();
	
	int unitNo = 200;
	if (main->m_phoneLine[0].pFSM->getCurrentState() < CMultimediaPhoneDlg::p3gsDialing)
	{
		main->m_pTelphoneRingDlg->HideContact(100);
	}

	//来callID
	if(m_bHasCallID)
	{
		HangOff_(NULL);
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
	if (((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pTempSetting->thisTelephoneNumber().number().length() >= 2)
	{
		code = ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pTempSetting->thisTelephoneNumber().number();
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
	
	std::string type = "(PSTN来电)";
//	m_MJPGList.SetUnitText(100, Util::StringOp::ToCString(tel+city+type), TRUE);
	
	m_sPstnTel	 = tel ;
	m_sPstnCity  = city ;		
	if (tel.substr(0,2) == "13")
	{
		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pTelphoneRingDlg->
			m_MJPGList.SetUnitText(unitNo+1, Util::StringOp::ToCString(tel+city+type), TRUE);

	}
	else
	{
		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pTelphoneRingDlg->
			m_MJPGList.SetUnitText(unitNo+2, Util::StringOp::ToCString(tel+city+type), TRUE);

	}
	
	if (tel == "")
	{
		if (((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pTempSetting->isFirewall())
		{
			int firetype = ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pTempSetting->firewallType();
			if (firetype == 1 || firetype == 2 || firetype == 3)
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
	//		ShowContact(result[0], tel);
			
			//add by qi 2009_11_04
			((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pTelphoneRingDlg->ShowContact(result[0], tel,unitNo);

			if (((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pTempSetting->isFirewall())
			{
				int firetype = ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pTempSetting->firewallType();
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
						tipname = ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pTempSetting->defaultTipFilename();
					}
					
					CFileStatus   status;
					if (CFile::GetStatus(LPCTSTR(Util::StringOp::ToCString(tipname)),status))
					{
						m_sTipFilename = tipname;
					}
					
					std::string ringname = result[0]->group()->ringName();
					if (ringname == Data::LanguageResource::Get(Data::RI_CARD_DEFAULT))
					{
						ringname = ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pTempSetting->defaultRingFilename();
					}
					
					if (ringname != "/flashdrv/my_ring/" && CFile::GetStatus(LPCTSTR(Util::StringOp::ToCString(ringname)),status))
					{
						m_sPstnRingFilename = ringname;
					}

				}
				else			//lxz 20090330
				{
					std::string ringname = "";
					CFileStatus   status;
					if(((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pTempSetting->isUseSpecRing())
					{
						ringname = ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pTempSetting->defaultRingFilename();
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
			if (((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pTempSetting->isFirewall())
			{
				int firetype = ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pTempSetting->firewallType();
				if (firetype == 1 || firetype == 2 || firetype == 3)
				{
					m_bPstnFirwall = TRUE;
				}
			}

			//lxz 20090330
			std::string	  ringname = "";
			CFileStatus   status;
			if(((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pTempSetting->isUseSpecRing())
			{
				ringname = ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pTempSetting->defaultRingFilename();
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
	
	((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->StopRing(true);
	extern void GMute(BOOL isOn);
	if (!((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->Hand())
	{
		GMute(FALSE);
	}

	KillTimer(IDT_STOPAUTORECORDE_TIME);
	
	if (((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pTempSetting->isMustRecord())
	{

	}
	else
	{
		if (m_bRecording)
		{
			OnButtonTelephoneRecord();
		}
		else
		{
			m_MJPGList.SetUnitIsShow(8, FALSE);
			m_MJPGList.SetUnitIsShow(2, TRUE);
		}	
	}
	
	m_MJPGList.SetUnitText(107, "", FALSE);
				
	if (m_bPstnRing && !IsWindowVisible())
	{
		
		CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd ;
		main->m_pTelphoneRingDlg->ShowWindow_(SW_HIDE);
	
		std::string type = "(PSTN来电)";
		m_MJPGList.SetUnitText(100, Util::StringOp::ToCString(m_sPstnTel+m_sPstnCity+type), TRUE);
		if (!m_sPstnTel.empty())
		{
			std::vector<boost::shared_ptr<Data::Contact> > result = FindCallContact(m_sPstnTel);
			if (!result.empty())
			{
				ShowContact(result[0], m_sPstnTel,200);
			}
		}
		ShowWindow_(SW_SHOW);

	}
	m_spPstnContactInfo->type(Data::citInOk);
	
	SetTimer(m_uiPstnTelephoneTimer, 1000, 0);
	CTimeSpan ts;
	ts = CTime::GetCurrentTime() - m_spPstnContactInfo->startTime();
	m_uPstnTelSecondOff = ts.GetTotalSeconds();

}

void CTelephoneDlg::PstnHangOff_(void* param)
{	
	m_uiIPstngnoreRingCount = 1;
	KillTimer(m_uiPstnTelephoneTimer);

	if (m_spPstnContactInfo)
	{
		m_spPstnContactInfo->duration(m_uiPstnTelephoneSecond);
		if (m_spPstnContactInfo->type() == Data::citInNo)
		{
			m_spPstnContactInfo->played(false);
		}
		
		((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pContactInfoDlg->SaveContactInfo(m_spPstnContactInfo);
		
		if (!m_vSoundSegment.empty())
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

		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactInfoDlg->ResetTypeInfo();
		if (m_spPstnContactInfo->type() == Data::citInNo)
		{
			::SendMessage(((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pMainDlg->GetSafeHwnd(), WM_TELNOTIFY, 1, 1);
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
	
	m_sPstnRingFilename ="";
	m_sPstnTel	= "";
	m_sPstnCity = "";
	
	((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pTelphoneRingDlg->ClearData(200);
	Dprintf("Pstn TelDlg hide\n");
		
}

void CTelephoneDlg::G3HangOff()
{
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;

	KillTimer(m_uiTipTimer);
	KillTimer(m_uiTelephoneTimer);
	
	if (m_bRecording)
	{
		OnButtonTelephoneRecord();
	}

	if (m_spContactInfo)
	{
		m_spContactInfo->duration(m_uiTelephoneSecond);
		if (m_spContactInfo->type() == Data::citInNo)
		{
			m_spContactInfo->played(false);
		}
		
		((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pContactInfoDlg->SaveContactInfo(m_spContactInfo);
		
		if (!m_vSoundSegment.empty())
		{
			for (std::vector<boost::shared_ptr<Data::SoundSegment> >::iterator iter = m_vSoundSegment.begin(); iter != m_vSoundSegment.end(); ++iter)
			{				
				(*iter)->telephoneNumber(m_spContactInfo->telephoneNumber());
				(*iter)->name(m_spContactInfo->name());
				(*iter)->contactInfoId(m_spContactInfo->id());
				(*iter)->contactId(m_spContactInfo->contactId());
				
				if (m_spContactInfo->type() == Data::citInNo)
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
		
		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactInfoDlg->ResetTypeInfo();
		if (m_spContactInfo->type() == Data::citInNo)
		{
			::SendMessage(((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pMainDlg->GetSafeHwnd(), WM_TELNOTIFY, 1, 1);
		}
		
	}
	m_spContactInfo = boost::shared_ptr<Data::ContactInfo>();
	
	m_uiTelephoneSecond = 0;
	m_uiRingCount = 0;	
	m_bHasCallID = FALSE;
	m_bPlayingLeaveWord = FALSE;
	m_bFirwall = FALSE;
	m_sTipFilename = "";
	m_sRingFilename = "";
	m_bRecording = FALSE;
	m_bRingPlayed = FALSE;
		
	main->m_pTelphoneRingDlg->ClearData(100);
	m_MJPGList.Invalidate();
	
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

//	if (((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pFSM->getCurrentState() == CMultimediaPhoneDlg::tsHangOff)
	if (((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_phoneLine[0].pFSM->getCurrentState() == CMultimediaPhoneDlg::p3gsHangOff)
	return;

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
	//if (!m_bAutoRecord)
	Dprintf("Hang On IN Record 1\n");
	if (m_spContactInfo && (m_spContactInfo->type() == Data::citOut))
//	if ((m_spContactInfo->id() == 0) && (m_spContactInfo->type() == Data::citOut))
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

//	if (m_spContactInfo)
//	{
		m_spContactInfo->type(Data::citInOk);
//	}

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
	CString str ;
	if ( 100 == uintNo )
	{
		m_spContactInfo->contactId(contact->id());
		m_spContactInfo->name(contact->name());
		str = CString(contact->name().c_str());
		str += _T("(");
		str += CString(number.c_str());
		str += _T(")");
		if (m_spContactInfo->type() != Data::citInOk)
		{
			str += _T("(3G来电)");
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

	m_MJPGList.SetUnitText(100, str, TRUE);
	m_MJPGList.SetUnitText(101, CString(contact->company().c_str()), TRUE);
	m_MJPGList.SetUnitText(102, CString(contact->department().c_str()), TRUE);
	m_MJPGList.SetUnitText(103, CString(contact->duty().c_str()), TRUE);

}

void CTelephoneDlg::DialContact(Data::TelephoneNumber telephoneNumber, int contactId)
{
	//if ((((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pFSM->getCurrentState() != CMultimediaPhoneDlg::tsHangOff)
	//	&& (((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pFSM->getCurrentState() != CMultimediaPhoneDlg::tsHangOn))
	if ((((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_phoneLine[0].pFSM->getCurrentState() != CMultimediaPhoneDlg::p3gsHangOff)
		&& (((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_phoneLine[0].pFSM->getCurrentState() != CMultimediaPhoneDlg::p3gsHangOn))
	return;

	BOOL flag = TRUE;
	if (!IsWindowVisible())
	{
		flag = FALSE;

		
		m_MJPGList.SetUnitIsShow(9, FALSE);
		m_MJPGList.SetUnitIsShow(19, FALSE);
		m_MJPGList.SetUnitIsShow(3, TRUE);
		
		ShowWindow_(TRUE);
		//SetTimer(IDC_TELDLGSHOW, 10, NULL);
	}

	Dprintf("DialContact:\n");
	Dprintf(telephoneNumber.number().c_str());

	m_sTelephoneNumber = telephoneNumber.number();
	std::string number = m_sTelephoneNumber;

//	if (((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pFSM->getCurrentState() == CMultimediaPhoneDlg::tsHangOff)
	if (((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_phoneLine[0].pFSM->getCurrentState() == CMultimediaPhoneDlg::p3gsHangOff)
	{
		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->phone_->HungOn();
		Sleep(500);
	}

	if (((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pTempSetting->thisTelephoneNumber().number().length() >= 2)
	{
		std::string code = ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pTempSetting->thisTelephoneNumber().number();
		if (code[0] != '0')
		{
			code = "0" + code;
		}
		int codeLen = code.length();
		if (number.substr(0, codeLen) == code)
		{
			number = number.substr(codeLen);
		}
		else
		{
			if ((number.length() > 11) && (number.substr(0, 3) == "861"))
			{
				number = number.substr(2);
			}
			std::string mobile = number;
			if (number[0] == '0')
			{
				mobile = number.substr(1);
			}
			mobile = mobile.substr(0, 7);
				
			FILE* file = fopen("\\hive\\res\\mobile.txt" , "rb");
			if (file != NULL)
			{
				std::string addr = StartSearch(file, mobile, 0, 93935);
				if (addr != "")
				{
					if (addr.substr(23, code.length()) == code)
					{
						if (number[0] == '0')
						{
							number = number.substr(1);
						}
					}
					else
					{
						if (number[0] != '0')
						{
							number = "0" + number;
						}
					}
					//city = addr.substr(8, 14);
				}
				fclose(file);
			}
		}
	}

	m_sDialNumber = number;

	if (((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pTempSetting->isAppendIpPrefix())
	{
		if (number[0] == '0')
		{
			m_sDialNumber = ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pTempSetting->ipPrefix() + number;
		}
	}

	if (((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pTempSetting->isAutoAppendOutlinePrefix())
	{
		m_sOutLine = ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pTempSetting->outlinePrefix();
// 		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->phone_->DialNumber((char*)out.c_str());
// 		Sleep(1500);
		if(m_sDialNumber.length() >= 5)
			m_MJPGList.SetUnitText(100, CString(std::string(m_sOutLine + m_sDialNumber).c_str()), flag);
		else 
		{
			int nindex = m_sDialNumber.find('1');
			if(nindex == 0)
				m_MJPGList.SetUnitText(100, CString(std::string(m_sOutLine + m_sDialNumber).c_str()), flag);
			else    //直接拨号
				m_MJPGList.SetUnitText(100, CString(std::string(m_sDialNumber).c_str()), flag);

		}

	}
	else
		m_MJPGList.SetUnitText(100, CString(std::string(m_sDialNumber).c_str()), flag);
	SetTimer(4, 10, NULL);

	((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->PhoneDialTone(TRUE, (char *)number.c_str());

	//((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->phone_->DialNumber((char*)number.c_str());
	
	if (!m_spContactInfo)
	{
		m_spContactInfo = boost::shared_ptr<Data::ContactInfo>(new Data::ContactInfo);
		m_spContactInfo->type(Data::citOut);
		m_spContactInfo->startTime(CTime::GetCurrentTime());
		m_spContactInfo->telephoneNumber(m_sTelephoneNumber);
		m_spContactInfo->Insert();
	}

	//((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pFSM->setStartState(CMultimediaPhoneDlg::tsKey);
	//((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pFSM->fireEvent(CMultimediaPhoneDlg::teConnect, NULL);
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
		CCEDialog::ShowWindow_(cmdshow);
		
		::PostMessage(((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pMainDlg->m_firewalDlg_->m_cmbTime.m_Combo.m_hWnd, CB_SHOWDROPDOWN,0,0);  //如果commbox打开，让commobox隐藏
		::PostMessage(((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pContactGroupDlg->m_cmbSoundTip.m_Combo.m_hWnd, CB_SHOWDROPDOWN,0,0);
		
		::PostMessage(((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pContactNewDlg->m_cmbType.m_Combo.m_hWnd, CB_SHOWDROPDOWN,0,0);
		::PostMessage(((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pContactNewDlg->m_cmbGroup.m_Combo.m_hWnd, CB_SHOWDROPDOWN,0,0);
		::PostMessage(((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pMainDlg->m_mainLunarderDlg_->m_cmbRing.m_Combo.m_hWnd, CB_SHOWDROPDOWN,0,0);
//		::PostMessage(((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pContactDlg->m_pSimImportDlg->m_cmbGroup.m_Combo.m_hWnd, CB_SHOWDROPDOWN,0,0);
		::PostMessage(((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pMainDlg->m_pWebDialog->m_cmbURL.m_Combo.m_hWnd, CB_SHOWDROPDOWN,0,0);
		
//		::PostMessage(((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_cmbRing.m_Combo.m_hWnd, CB_SHOWDROPDOWN,0,0);
		::PostMessage(((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_cmbRingTimes.m_Combo.m_hWnd, CB_SHOWDROPDOWN,0,0);
		::PostMessage(((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_cmbAutoRecoedeTimes.m_Combo.m_hWnd, CB_SHOWDROPDOWN,0,0);
		::PostMessage(((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_cmbWaitTime.m_Combo.m_hWnd, CB_SHOWDROPDOWN,0,0);
//		::PostMessage(((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_cmbSoundSavePath.m_Combo.m_hWnd, CB_SHOWDROPDOWN,0,0);
		
//		::PostMessage(((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_cmbRingVolume.m_Combo.m_hWnd, CB_SHOWDROPDOWN,0,0);
//		::PostMessage(((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_cmbSystemVolume.m_Combo.m_hWnd, CB_SHOWDROPDOWN,0,0);
		::PostMessage(((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_cmbBlackLightWaitTime.m_Combo.m_hWnd, CB_SHOWDROPDOWN,0,0);
	}
	else
	{
		CCEDialog::ShowWindow_(cmdshow);
		((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pMainDlg->m_pWebDialog->SendMessage(WM_KILLWEBSHOW, 0, 0);

//		((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pMainDlg->SendMessage(WM_PLAYVIDEO, 1, 0);			//恢复视频
//		((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pMainDlg->m_mainMp3Dlg_->SendMessage(WM_OUTEVENT, 0, 1);
	}
}

void CTelephoneDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	CString icon ;
	if(nIDEvent == IDT_TEL_STATUS)
	{
	//	if (((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pFSM->getCurrentState() == CMultimediaPhoneDlg::tsHangOff )
		if (((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_phoneLine[0].pFSM->getCurrentState() == CMultimediaPhoneDlg::p3gsHangOff )
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
	else if(nIDEvent == IDT_DIAL_PRESS)
	{	
		KillTimer(IDT_DIAL_PRESS);
		m_MJPGList.SetUnitIsShow(9, FALSE);
		m_MJPGList.SetUnitIsShow(19, FALSE);
		m_MJPGList.SetUnitIsShow(3, TRUE);
//		m_MJPGList.Invalidate();
	}
	else if(nIDEvent == IDT_TEL_EXIT)
	{
		KillTimer(IDT_TEL_EXIT);
		HandleOff();
	}
	else if (nIDEvent == m_uiTelephoneTimer)
	{
		CTimeSpan ts;
		ts = CTime::GetCurrentTime() - m_spContactInfo->startTime() - m_uTelSecondOff;
		m_uiTelephoneSecond = ts.GetTotalSeconds();

		CString duration;
		if (m_uiTelephoneSecond > 3600)
		{
			duration.Format(_T("%d:%02d:%02d"), ts.GetHours(), ts.GetMinutes(),ts.GetSeconds());
		}
		else
		{
			duration.Format(_T("%02d:%02d"), ts.GetMinutes(),ts.GetSeconds());
		}

		if(IsWindowVisible() && !((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pNotebookDlg->m_bIsOPenTel)
		{
			m_MJPGList.SetUnitText(107, duration, TRUE, TRUE);
		}

		if ((m_uiTelephoneSecond % 60) == 0)
		{
			m_spContactInfo->duration(m_uiTelephoneSecond);
			m_spContactInfo->Update();
		}	
		
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
			m_MJPGList.SetUnitText(107, duration, TRUE, TRUE);
		}

		if ((m_uiPstnTelephoneSecond % 60) == 0)
		{
			m_spPstnContactInfo->duration(m_uiTelephoneSecond);
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
				
				CCEDialog::OnTimer(nIDEvent);
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
		int nTimers = ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pTempSetting->maxSoundUseSize();
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
		if(m_bRing || ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->GetPhoneHungOn())
		{
			//change by qi 2009_11_04				
			if (!((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pTelphoneDialDlg->IsWindowVisible())
			{	

				((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pTelphoneDialDlg->ShowWindow_(SW_SHOW);
				
				//启动10条通话记录
				SetTimer(SHOW_10CONTACTINFO_TIMER,10,NULL);
				
				//加载图标
				icon = Allicon[0];
				((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->AddIcon(icon);

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
			HandleOff();	
			//lxz 20090207
			/*
			ShowWindow_(FALSE);    //Mp3被挂起,不能继续播放
			((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pMainDlg->SendMessage(WM_PLAYVIDEO, 1, 0);			//恢复视频
			((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pMainDlg->m_mainMp3Dlg_->SendMessage(WM_OUTEVENT, 0, 1);
			*/

		}
	}
	else if(nIDEvent == IDC_TELDLGHIDE)
	{
		Dprintf("TelDlg hide3\n");
		m_bRing = FALSE;
		KillTimer(IDC_TELDLGHIDE);
		ShowWindow_(FALSE);

		m_MJPGList.SetUnitIsShow(9, FALSE);
		m_MJPGList.SetUnitIsShow(19, FALSE);
		m_MJPGList.SetUnitIsShow(3, FALSE);
	}
	else if(nIDEvent == 4)
	{
		KillTimer(4);
		memset(m_chDialNumber, 0, 64);
		if (((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pTempSetting->isAutoAppendOutlinePrefix())
		{

			if((m_sDialNumber.length() >= 5))
			{
				m_sOutLine = ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pTempSetting->outlinePrefix();
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
					m_sOutLine = ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pTempSetting->outlinePrefix();
					strcpy(m_chDialNumber, (char*)m_sOutLine.c_str());
					strcat(m_chDialNumber, ",");
					//((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->phone_->DialNumber((char*)m_sOutLine.c_str());
					//Sleep(2000);
				}
			}

// 			m_sOutLine = ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pSetting->outlinePrefix();
// 			((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->phone_->DialNumber((char*)m_sOutLine.c_str());
// 			Sleep(1500);
		}
		memcpy(m_chDialNumber+strlen(m_chDialNumber), (char*)m_sDialNumber.c_str(), strlen((char*)m_sDialNumber.c_str()));
		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->phone_->DialNumber(m_chDialNumber);
		//((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->phone_->DialNumber((char*)m_sDialNumber.c_str());
	//	((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pFSM->setStartState(CMultimediaPhoneDlg::tsKey);
		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_phoneLine[0].pFSM->setStartState(CMultimediaPhoneDlg::p3gsKey);
	//	Sleep(4000);   //lxz20090204
	//	SetTimer(5, 6000, NULL);
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

	extern BOOL _test_call;
	if(_test_call)
	{
		if (nIDEvent == 10)
		{
			KillTimer(10);
			HandleOff();
		}
	}
	
	CCEDialog::OnTimer(nIDEvent);
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

void CTelephoneDlg::FromTelDial(boost::shared_ptr<Data::ContactInfo> pContactInfo,std::string tel)
{	
	m_spContactInfo		= pContactInfo ;
	m_sTelephoneNumber	= tel	;
	m_bTelUsing			= TRUE	;
	
	CString s = Util::StringOp::ToCString(m_sTelephoneNumber);
	m_MJPGList.SetUnitText(100,s,true);
	
}
void CTelephoneDlg::VolumeSwitch()
{	
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	static bool on = false;

	if (!on)
	{
		m_MJPGList.SetUnitIsDownStatus(50,true);
		on = true;
		main->phone_->Mute(true);
//		main->phone_->Volume(0);
	}
	else
	{	
		m_MJPGList.SetUnitIsDownStatus(50,false);
		on = false ;
		main->phone_->Mute(false);
//		main->phone_->Volume(2);
	}

	m_MJPGList.SetUnitIsShow(50,true,true);

// 	for (int i = 0 ; i < 5 ;i++)
// 	{
// 		m_MJPGList.SetUnitIsDownStatus(40+i,false);
// 	}
// 	m_MJPGList.Invalidate();

}
void CTelephoneDlg::SetVolume(unsigned int level)
{
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd ;
	main->phone_->Volume(level);
//	main->phone_->Volume(5);
	
	if (m_MJPGList.GetUnitIsDownStatus(40+level))
	{
		level++;
	}

	for (int i = 0 ; i < 6; i++)
	{
		m_MJPGList.SetUnitIsDownStatus(40+i,true);
	}
	
	m_Volume = level;
	
	for (i = 0 ; i < level;i++)
	{
		m_MJPGList.SetUnitIsDownStatus(40+i,false);	
	}
	m_MJPGList.Invalidate();

}

void CTelephoneDlg::HandleAudio(bool bt)
{		
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd ;

	if (bt)
	{
		m_Volume++;
		if (m_Volume > 6)
		{
			m_Volume = 6;
			return ;
		}
	}
	else
	{	m_Volume--;
		if (m_Volume < 0)
		{
			m_Volume = 0 ;
			return ;
		}
	}
	
	main->phone_->Volume(m_Volume);

	for (int i = 0 ; i < 6; i++)
	{
		m_MJPGList.SetUnitIsDownStatus(40+i,true);
	}
	
	for (i = 0 ; i < m_Volume;i++)
	{
		m_MJPGList.SetUnitIsDownStatus(40+i,false);	
	}
	m_MJPGList.Invalidate();
	
}

void CTelephoneDlg::Dialback(std::string telnum)
{
	CMultimediaPhoneDlg *pMainDlg = (CMultimediaPhoneDlg*)theApp.m_pMainWnd ;
	CString icon ;
	//拨号
	if(!telnum.empty())
	{	
		//添加图标
		icon = Allicon[0];
		pMainDlg->AddIcon(icon);
		
		Data::TelephoneNumber telphonenumber ;
		telphonenumber.number(telnum);
		DialContact(telphonenumber);
	}

}

