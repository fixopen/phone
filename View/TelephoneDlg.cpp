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
#include "stdlib.h"
#include <string>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#define IDC_TELDLGSHOW		1000//定时器事件
#define DIAL_TIMER  		1001
#define HUNGOFF_TIMER       1002
#define Record_strch        "录音"
#define Record_streg        "Record"
#define hangon_strch        "接听"  
#define hangon_streg        "Receiving"
#define SIMADDRLENGTH        29
#define SIMADDTLENGTH        34

typedef enum __WAVEDEV_MSG__
{
	WAV_SPEAKER_ONLY = 0x10000001,
	WAV_LINEOUT_ONLY = 0x10000002,
	WAV_SPEAKER_LINEOUT = 0x10000003,
	WAV_SNDEFFECT_POP	= 0x20000000,
	WAV_SNDEFFECT_3D	= 0x20000001,
	WAV_SNDEFFECT_CLASSIC	= 0x20000002,
	WAV_SNDEFFECT_JAZZ		= 0x20000003,
	WAV_LINEIN_MIC			= 0x30000001,
	WAV_LOCAL_MIC			= 0x30000002
} WAVDEV_MESSAGE;


#define RIFF_FILE       mmioFOURCC('R','I','F','F')
#define RIFF_WAVE       mmioFOURCC('W','A','V','E')
#define RIFF_FORMAT     mmioFOURCC('f','m','t',' ')
#define RIFF_DATA       mmioFOURCC('d','a','t','a')

#define SECONDBYTES8           2048
#define SECONDBYTES2           850

#define DIR_SOUNDSEGMENT_NUM	500    //一个目录下存储的录音文件数	
#define TEL_SHOW_LENGTH			20	

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
static volatile bool bVoiceStop = true;
static volatile bool bHasTDThread = false;
static CRITICAL_SECTION csVoice;
short * pVoiceBuffer = NULL;
int nVoiceBufferLength = 32000;
int nVoiceBuffetPostion=0;
int nVoiceLength=8000;
std::string g_sOutDialRecordName = "";
static volatile bool bVoiceMeset=true;
extern DWORD PreFreeSize;
extern DWORD sdPreFreeSize;
extern BOOL DetectFile(TCHAR *sDir);
extern int g_iRecordCount;

HANDLE g_hThreadExit = 0;  //判断线程是否退出
extern 	VOID WriteLog_(CHAR* str);

// VOID WriteLog_(char *ptr, int size)
// {
// 	FILE* fp = fopen("\\flashdrv\\log.dat", "w+b");
//     if (fp != NULL) {
// 		fwrite(ptr, sizeof(char), size, fp);
// 	    fclose(fp);
//     }
// }

VOID WriteLog_(CHAR* str)
{	
	return ;

	TCHAR s[256] = {0};
	
    FILE* fp = fopen("\\flashdrv\\log.txt", "a+");
    if (fp != NULL) {
		fwrite(str, strlen(str), 1, fp);
		fwrite("\n", strlen("\n"), 1, fp);
        fclose(fp);
    }
}

void WriteMemory()
{	
	return;

	CString strMemInfo;
	MEMORYSTATUS memInfo;
	
	memInfo.dwLength = sizeof(memInfo);
	GlobalMemoryStatus(&memInfo);

	strMemInfo.Format(\
		_T("memory load: %u%%, total physical: %u(B),\
		available physical: %u(B)  "), memInfo.dwMemoryLoad,\
		memInfo.dwTotalPhys, memInfo.dwAvailPhys);
	
	CTime time = CTime::GetCurrentTime();
	CString strTime;
	strTime.Format(_T("%02d-%02d-%02d %02d:%02d:%02d"),\
		time.GetYear(), time.GetMonth(), time.GetDay(),\
		time.GetHour(), time.GetMinute(), time.GetSecond());

	strMemInfo += strTime;
	WriteLog_((char *)Util::StringOp::FromCString(strMemInfo).c_str());

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
DWORD WINAPI VoiceCheckThreadPrc(LPVOID lpParameter)
{
	bHasTDThread = true;
	while (true)
	{
		EnterCriticalSection(&csVoice);
		bool b = CheckQueue.empty();
		LeaveCriticalSection(&csVoice);
		if (!b)
		{
			EnterCriticalSection(&csVoice);
			OggCodec::PcmData* m = CheckQueue.front();
			memset(pVoiceBuffer, 0,nVoiceBufferLength*sizeof(short));
			memcpy(pVoiceBuffer, m->data, m->dataLength*sizeof(short));
			nVoiceBufferLength = m->dataLength;
			free(m->data);
			delete m;
			CheckQueue.pop();
			LeaveCriticalSection(&csVoice);
			int toneDetected = 0;
			if (SabtdProcess_(m_sabtd_struct, &toneDetected, pVoiceBuffer, nVoiceBufferLength) < 0)
			{
				break;
			}
			if(toneDetected == SABTD_TONE_DETECTED)
			{
				SabtdReset_(m_sabtd_struct);
				((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->HungOff();	
				bVoiceMeset=true;
			    break;
			}
			Sleep(0);
			continue;
		}
		else if (bVoiceStop)
		{   
			bVoiceMeset=true;
			break;
		}
		Sleep(10);
	}
	SabtdReset_(m_sabtd_struct);
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
	bHasTDThread = false;
	return 0;
}

BOOL Writelog(std::string filePath,unsigned char * ERRORContent, int contentLen,std::string ERRORTYPE)
{
	FILE * file=NULL;
	
	SYSTEMTIME curtime;
	
	GetLocalTime(&curtime);
	
	char time[76];

	char telNumber[3];
	
	std::string space=" ";
	
    std::string TEspace="\r\n";
	
	memset(time,'\0',76);

	memset(telNumber,'\0',3);
	
	sprintf(time, "%04d-%02d-%02d %02d:%02d", curtime.wYear,curtime.wMonth,curtime.wDay,curtime.wHour, curtime.wMinute);
	
	file=fopen(filePath.c_str(),"ab+");
	
	if (file == NULL)
	{
		return FALSE;
	}
    
	int len=fseek(file,0,SEEK_END);
	
	if (len >1024*1024)
	{   	
		fseek(file,0,SEEK_SET);
		
	}else
	{
		fseek(file,0,SEEK_END);	
	}
	
	fwrite(time,sizeof(char),strlen(time),file);
	
	fwrite(space.c_str(),sizeof(char),space.length(),file);
	
	fwrite(ERRORTYPE.c_str(),sizeof(char),ERRORTYPE.length(),file);
	
	for ( int index=0;index<contentLen;index++)
	{
		sprintf(telNumber,"%2x",ERRORContent[index]);

		fwrite(telNumber,sizeof(char),strlen(telNumber),file);

		memset(telNumber,'\0',3);
	}
	
	fwrite(TEspace.c_str(),sizeof(char),TEspace.length(),file);
	
    fflush(file);
	
    fclose(file);
	
	return TRUE;
	
}

BOOL Writelog(std::string filePath,std::string conttent, std::string ERRORTYPE)
{
	FILE * file=NULL;
	
	SYSTEMTIME curtime;
	
	GetLocalTime(&curtime);
	
	char time[76];
	
	std::string space=" ";
	
    std::string TEspace="\r\n";
	
	memset(time,'\0',76);
	
	sprintf(time, "%04d-%02d-%02d %02d:%02d", curtime.wYear,curtime.wMonth,curtime.wDay,curtime.wHour, curtime.wMinute);
	
	file=fopen(filePath.c_str(),"ab+");
	
	if (file == NULL)
	{
		return FALSE;
	}
    
	int len=fseek(file,0,SEEK_END);
	
	if (len >1024*1024)
	{   	
		fseek(file,0,SEEK_SET);
		
	}else
	{
		fseek(file,0,SEEK_END);	
	}
	
	fwrite(time,sizeof(char),strlen(time),file);
	
	fwrite(space.c_str(),sizeof(char),space.length(),file);
	
	fwrite(ERRORTYPE.c_str(),sizeof(char),ERRORTYPE.length(),file);
	
	fwrite(conttent.c_str(),sizeof(char),conttent.length(),file);
	
	fwrite(TEspace.c_str(),sizeof(char),TEspace.length(),file);
	
    fflush(file);
	
    fclose(file);
	
	return TRUE;
	
}

void replaceXing(std::string& number)
{
	for (int i = 0 ; i < number.size() ;i++)
	{
		if (number[i] == '*')
		{
			number[i] = 'a';
		}
	}
}


/////////////////////////////////////////////////////////////////////////////
// CTelephoneDlg dialog


CTelephoneDlg::CTelephoneDlg(CWnd* pParent /*=NULL*/)
	: CCEDialog/*CCEDialog*/(CTelephoneDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTelephoneDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

void CTelephoneDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTelephoneDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTelephoneDlg, CCEDialog/*CCEDialog*/)
	//{{AFX_MSG_MAP(CTelephoneDlg)
	ON_WM_TIMER()
	ON_MESSAGE(MM_WIM_OPEN,OnMM_WIM_OPEN)
	ON_MESSAGE(MM_WIM_DATA,OnMM_WIM_DATA)
	ON_MESSAGE(MM_WIM_CLOSE,OnMM_WIM_CLOSE)
	ON_MESSAGE(MM_WOM_OPEN,OnMM_WOM_OPEN)
	ON_MESSAGE(MM_WOM_DONE,OnMM_WOM_DONE)
	ON_MESSAGE(MM_WOM_CLOSE,OnMM_WOM_CLOSE)
	ON_MESSAGE(WM_CHANGELANGUAGE,OnChangeLanguage)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_TELEPHONE_HIDE, OnButtonTelephoneHide)
	ON_BN_CLICKED(IDC_BUTTON_TELEPHONE_NOTE, OnButtonTelephoneNote)
	ON_BN_CLICKED(IDC_BUTTON_TELEPHONE_RECORD, OnButtonTelephoneRecord)
	ON_BN_CLICKED(IDC_BUTTON_TELEPHONE_HANDLE, OnButtonTelephoneHandle)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTelephoneDlg message handlers

BOOL CTelephoneDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	std::string strTemp;
	CString strtext;
	int yBegin  = 73 ;
	int yHeight = 22 ;
    m_sticNumber.Create(strtext,WS_CHILD|WS_VISIBLE, CRect(50, 70*VHEIGHT, 363, 95*VHEIGHT), this, IDC_STATIC_TELEPHONE_NAMENO);
    m_sticNumber.SetColor(RGB(0, 0, 0), Data::g_partFrameInRectBackRGB[Data::g_skinstyle]);
	m_sticNumber.SetFontSize(34);

	m_sticMcaller.Create(strtext, WS_CHILD|WS_VISIBLE, CRect(55, 95*VHEIGHT, 105, 115*VHEIGHT), this, IDC_STATIC_MCALLER);
	m_sticMcaller.SetColor(RGB(0,0,0),Data::g_partFrameInRectBackRGB[Data::g_skinstyle]);
	m_sticMcaller.SetFontSize(18);
	m_sticDynMcaller.Create(strtext, WS_CHILD|WS_VISIBLE, CRect(105, 95*VHEIGHT, 360, 115*VHEIGHT), this, IDC_STATIC_MCALLER);
	m_sticDynMcaller.SetColor(RGB(0,0,0),Data::g_partFrameInRectBackRGB[Data::g_skinstyle]);
	m_sticDynMcaller.SetFontSize(18);

	m_sticName.Create(strtext, WS_CHILD|WS_VISIBLE, CRect(55, 115*VHEIGHT, 105, 135*VHEIGHT), this, IDC_STATIC_TELEPHONE_NAMENO);
	m_sticName.SetColor(RGB(0, 0, 0), Data::g_partFrameInRectBackRGB[Data::g_skinstyle]);
	m_sticName.SetFontSize(18);
	m_sticDynName.Create(strtext, WS_CHILD|WS_VISIBLE, CRect(105, 115*VHEIGHT, 360, 135*VHEIGHT), this, IDC_STATIC_TELEPHONE_NAMENO);
	m_sticDynName.SetColor(RGB(0, 0, 0), Data::g_partFrameInRectBackRGB[Data::g_skinstyle]);
	m_sticDynName.SetFontSize(18);

	m_sticCompany.Create(strtext, WS_CHILD|WS_VISIBLE, CRect(55, 135*VHEIGHT, 105, 155*VHEIGHT), this, IDC_STATIC_TELEPHONE_COMPANY);
	m_sticCompany.SetColor(RGB(0, 0, 0), Data::g_partFrameInRectBackRGB[Data::g_skinstyle]);
	m_sticCompany.SetFontSize(18);
	m_sticDynCompany.Create(strtext, WS_CHILD|WS_VISIBLE, CRect(105, 135*VHEIGHT, 225, 155*VHEIGHT), this, IDC_STATIC_TELEPHONE_COMPANY);
	m_sticDynCompany.SetColor(RGB(0, 0, 0), Data::g_partFrameInRectBackRGB[Data::g_skinstyle]);
	m_sticDynCompany.SetFontSize(18);

	m_sticDuty.Create(strtext, WS_CHILD|WS_VISIBLE, CRect(55, 155*VHEIGHT, 105, 175*VHEIGHT), this, IDC_STATIC_TELEPHONE_DUTY);
	m_sticDuty.SetColor(RGB(0, 0, 0), Data::g_partFrameInRectBackRGB[Data::g_skinstyle]);
	m_sticDuty.SetFontSize(18);
	m_sticDynDuty.Create(strtext, WS_CHILD|WS_VISIBLE, CRect(105, 155*VHEIGHT, 225, 175*VHEIGHT), this, IDC_STATIC_TELEPHONE_DUTY);
	m_sticDynDuty.SetColor(RGB(0, 0, 0), Data::g_partFrameInRectBackRGB[Data::g_skinstyle]);
	m_sticDynDuty.SetFontSize(18);

	strTemp = Data::LanguageResource::Get(Data::RI_TEL_RECORDING);
	strtext = strTemp.c_str();
	m_sticRecordCaption.Create(strtext, WS_CHILD, CRect(230, 135*VHEIGHT, 300, 155*VHEIGHT), this);
	m_sticRecordCaption.SetColor(RGB(0, 0, 0), Data::g_partFrameInRectBackRGB[Data::g_skinstyle]);
	m_sticRecordCaption.SetFontSize(18);

	m_sticRecord.Create(_T(""), WS_CHILD, CRect(300, 135*VHEIGHT, 360, 155*VHEIGHT), this);
	m_sticRecord.SetColor(RGB(0, 0, 0), Data::g_partFrameInRectBackRGB[Data::g_skinstyle]);
	m_sticRecord.SetFontSize(18);

	strTemp = Data::LanguageResource::Get(Data::RI_TEL_DURATION);
	strtext = strTemp.c_str();
	m_sticDurationCaption.Create(strtext, WS_CHILD, CRect(230, 155*VHEIGHT, 300, 175*VHEIGHT), this);
	m_sticDurationCaption.SetColor(RGB(0, 0, 0), Data::g_partFrameInRectBackRGB[Data::g_skinstyle]);
	m_sticDurationCaption.SetFontSize(18);
	m_sticDuration.Create(_T(""), WS_CHILD, CRect(300, 155*VHEIGHT, 360, 175*VHEIGHT), this);
	m_sticDuration.SetColor(RGB(0, 0, 0), Data::g_partFrameInRectBackRGB[Data::g_skinstyle]);
	m_sticDuration.SetFontSize(18);


	strTemp = Data::LanguageResource::Get(Data::RI_TEL_HIDEBTN);
	strtext = strTemp.c_str();
	m_btnHide.Create(strtext, Data::g_buttonArcBMPPARTDILOAGID[0][Data::g_skinstyle], Data::g_buttonArcBMPPARTDILOAGID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(378, 78*VHEIGHT, 434, 98*VHEIGHT), this, IDC_BUTTON_TELEPHONE_HIDE);
	m_btnHide.SetBackRGB(Data::g_partFrameMainBackRGB[Data::g_skinstyle]);

	strTemp = Data::LanguageResource::Get(Data::RI_TEL_NOTEBTN);
	strtext = strTemp.c_str();
	m_btnNote.Create(strtext, Data::g_buttonArcBMPPARTDILOAGID[0][Data::g_skinstyle], Data::g_buttonArcBMPPARTDILOAGID[1][Data::g_skinstyle], WS_CHILD, CRect(378, 103*VHEIGHT, 434, 123*VHEIGHT), this, IDC_BUTTON_TELEPHONE_NOTE);
	m_btnNote.SetBackRGB(Data::g_partFrameMainBackRGB[Data::g_skinstyle]);

	strTemp = Data::LanguageResource::Get(Data::RI_TEL_RECORDEBTN);
	strtext = strTemp.c_str();
	m_btnRecord.Create(strtext, Data::g_buttonArcBMPPARTDILOAGID[0][Data::g_skinstyle], Data::g_buttonArcBMPPARTDILOAGID[1][Data::g_skinstyle], WS_CHILD, CRect(378, 128*VHEIGHT, 434, 148*VHEIGHT-1), this, IDC_BUTTON_TELEPHONE_RECORD);
	m_btnRecord.SetBackRGB(Data::g_partFrameMainBackRGB[Data::g_skinstyle]);
	strTemp = Data::LanguageResource::Get(Data::RI_TEL_ACCEPTBTN);
	strtext = strTemp.c_str();
	m_btnHandle.Create(strtext, Data::g_buttonArcBMPPARTDILOAGID[0][Data::g_skinstyle], Data::g_buttonArcBMPPARTDILOAGID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(378, 153*VHEIGHT, 434, 173*VHEIGHT), this, IDC_BUTTON_TELEPHONE_HANDLE);
	m_btnHandle.SetBackRGB(Data::g_partFrameMainBackRGB[Data::g_skinstyle]);

	m_btnRecordTip.Create(_T(""),IDB_BITMAP31, IDB_BITMAP31, WS_CHILD, CRect(230, 128*VHEIGHT, 246, 144*VHEIGHT), this, IDC_BUTTON_TELEPHONE_RECORDTIP);
	m_btnRecordTip.SetBackRGB(Data::g_partFrameMainBackRGB[Data::g_skinstyle]);

//	change by qi 20111103
	m_sticBackground.Create(CRect(34, 40*VHEIGHT, 448, 184*VHEIGHT), this, 1);
	
	SetTitle(Data::RI_TEL_NEWTELSTC);
	if (!InitData())
	{

	}
    m_IsRecordNumber=false;
	m_IsTaking=false;

	g_hThreadExit = CreateEvent(NULL, TRUE, TRUE, NULL);

	return TRUE;  
}

bool CTelephoneDlg::InitData(void)
{
	m_strRecord = Util::StringOp::ToCString((Data::LanguageResource::Get(Data::RI_TEL_RECORDEBTN)));
	m_strStopRecord = Util::StringOp::ToCString((Data::LanguageResource::Get(Data::RI_TEL_STOPRECORDEBTN)));
	m_strHangOn = Util::StringOp::ToCString((Data::LanguageResource::Get(Data::RI_TEL_ACCEPTBTN)));
	m_strHangOff = Util::StringOp::ToCString((Data::LanguageResource::Get(Data::RI_TEL_HUNGOFFBTN)));
	m_uiTelephoneTimer = 1;
	m_uiTelephoneSecond = 0;
	m_uiRecordTimer = 2;
	m_uiRecordSecond = 0;
	m_uiRingCount = 0;
	m_uiTipTimer = 3;
	m_uiToneDetect = 11;
	m_bHasTDThread = FALSE;
	m_uiInNoCount = 0;
	m_uiRecordCound = 0;
	m_uiCallIDTimeoutTimer = 4;
	m_uiCallIDTimeoutMS = 2500;

	m_bAddZero = FALSE;
	m_bKeyDial = FALSE;
	m_bHasCallID = FALSE;
	m_bRingPlayed = FALSE;
	m_spContactInfo = boost::shared_ptr<Data::ContactInfo>();
	m_spSoundSegment = boost::shared_ptr<Data::SoundSegment>();
	m_pOggCodec = new OggCodec();
	InitializeCriticalSection(&csVoice);
	pVoiceBuffer = (short*)malloc(nVoiceBufferLength * sizeof(short));
	m_bEnding = FALSE;
	m_bPlaying = FALSE;
	m_bRecording = FALSE;
	m_dwDataLength = 0;
	m_sDeleteFile.clear();

	for (int i = 0; i < WAVE_BUFFER_COUNT; ++i)
	{
		m_pBuffer[i] = 	(PBYTE)malloc(INP_BUFFER_SIZE);
		m_pWaveHdr[i] = reinterpret_cast<PWAVEHDR>(malloc(sizeof(WAVEHDR)));
	}
	
	m_bHasLeaveWord = FALSE;
	m_bPlayingLeaveWord = FALSE;
	m_bFirwall = FALSE;
	m_waveform.wFormatTag=WAVE_FORMAT_PCM;
	m_waveform.nChannels=CHANNELS;
	m_waveform.nSamplesPerSec=SAMPLESPERSEC;
	m_waveform.wBitsPerSample=BITSPERSAMPLE;
	m_waveform.nBlockAlign=m_waveform.nChannels * m_waveform.wBitsPerSample / 8;
	m_waveform.nAvgBytesPerSec=m_waveform.nBlockAlign * m_waveform.nSamplesPerSec;
	m_waveform.cbSize=0;	

	m_hSABTD = LoadLibrary(_T("\\HIVE\\RES_DAT\\SABTD_CEDLL.dll")); 
	Dprintf("\\HIVE\\RES_DAT\\SABTD_CEDLL.dll %x.\n", m_hSABTD);
	if (m_hSABTD != NULL)
	{
		SabtdInit_ = (SabtdInit)GetProcAddress(m_hSABTD, _T("SABTD_Init"));
		SabtdUnInit_ = (SabtdUnInit)GetProcAddress(m_hSABTD, _T("SABTD_UnInit"));
		SabtdSetParam_ = (SabtdSetParam)GetProcAddress(m_hSABTD, _T("SABTD_SetParam"));
		SabtdGetParam_ = (SabtdGetParam)GetProcAddress(m_hSABTD, _T("SABTD_GetParam"));
		SabtdReset_ = (SabtdReset)GetProcAddress(m_hSABTD, _T("SABTD_Reset"));
		SabtdProcess_ = (SabtdProcess)GetProcAddress(m_hSABTD, _T("SABTD_Process"));
		
		int err = SabtdInit_(&m_sabtd_struct, SAMPLESPERSEC, "\\HIVE\\RES_DAT\\sa_license.dat");
		if (err == SARTC_NORMAL)
		{
			SabtdSetParam_(m_sabtd_struct, SABTD_PARAMTYPE_FRONT_NOTTONE, 0);
 			SabtdSetParam_(m_sabtd_struct, SABTD_PARAMTYPE_BUSYTONE_NUM, 3);
			SabtdSetParam_(m_sabtd_struct, SABTD_PARAMTYPE_ENERGY, 0.1f);
			SabtdSetParam_(m_sabtd_struct, SABTD_PARAMTYPE_FREQ_MIN, 300);
			Dprintf("Product license ok.\n");
		}
		else
		{
			Dprintf("Product license error %d.\n", err);
		}
	}
    
	if ((((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_chbEnableRing.GetCheck()==BST_CHECKED)||
		((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pSetting->isFirewall())
	{
		((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->OpenTelRing(FALSE);
		
	}else{
		
		((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->OpenTelRing(TRUE);
	}

	return true;
}

void CTelephoneDlg::OnButtonTelephoneHide()
{
	ShowWindow_(FALSE);
}

void CTelephoneDlg::ShowWindow_(int nCmdShow)
{
	if(nCmdShow > 0)
	{
		if(((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pMainDlg->m_mainScreenSaveDlg_->IsWindowVisible())
			
			return;
	}

//add by qi 20111103
//	ShowWindow(nCmdShow);

	CCEDialog::ShowWindow_(nCmdShow);
}

void CTelephoneDlg::OnButtonTelephoneNote()
{ 
	//defect 143
	ShowWindow_(FALSE);
	//==================
	((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->SwitchPanel_(IDC_BUTTON_INFORMATION);
}

std::string CTelephoneDlg::GetSoundPath(void)
{
	std::string path = ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->\
		m_pSettingDlg->m_pSetting->soundPath();

	BOOL DetectDIR(TCHAR *sDir);

	if (path == ssStorageCardRecordPath)
	{
		BOOL sd = DetectDIR(L"\\StorageCard");
		if (sd)
		{
			if (!DetectDIR((LPTSTR)(LPCTSTR)csStorageCardRecordPath))
			{
				CreateDirectory((LPTSTR)(LPCTSTR)csStorageCardRecordPath, NULL);
			}
		}
		else
		{	
			//change by qi 20110707
			//设置录音存储目录
			SetFlashdrvRecordDir(path);
		}
	}
	else if(path == ssFlashRecordPath)
	{
		ULARGE_INTEGER freeBytes;
		ULARGE_INTEGER totalBytes;
		GetDiskFreeSpaceEx(Util::StringOp::ToCString(path), &freeBytes, &totalBytes, NULL);
		if (freeBytes.QuadPart < PreFreeSize)  //FlashDrv下空间不足自动转存到SD卡
		{
			if(DetectDIR(L"\\StorageCard"))
			{
				GetDiskFreeSpaceEx(L"\\StorageCard", &freeBytes, &totalBytes, NULL);
				if(freeBytes.QuadPart > sdPreFreeSize)  //SD卡中预留5M空间
				{
					path = ssStorageCardRecordPath;
					if (!DetectDIR((LPTSTR)(LPCTSTR)csStorageCardRecordPath))
					{
						CreateDirectory((LPTSTR)(LPCTSTR)csStorageCardRecordPath, NULL);
					}
				}
			}
		}
		else 
		{	
			//change by qi 20110707
			//设置录音存储目录
			SetFlashdrvRecordDir(path);
		}
	}
	
	return path;
}

void CTelephoneDlg::SetFlashdrvRecordDir(std::string &path)
{
	//add by qi 20110707
	//判断录音文件个数
	UINT nSoundSegmentCount   = Data::SoundSegment::GetDataCount("");
	UINT nPage = nSoundSegmentCount/DIR_SOUNDSEGMENT_NUM;
	CString dirName ;//目录名
	dirName.Format(L"my_record%d\\",nPage);//每500个录音放一个目录
	path = ssFlashRecordPath + Util::StringOp::FromCString(dirName);
	
	//创建目录
	CString cDir = Util::StringOp::ToCString(path);
	BOOL DetectDIR(TCHAR *sDir);
	if (!DetectDIR((LPTSTR)(LPCTSTR)cDir))
	{
		CreateDirectory((LPTSTR)(LPCTSTR)cDir, NULL);
	}
}

void CTelephoneDlg::OnButtonTelephoneRecord()
{

	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	CString caption;
	std::string str;
	str = Data::LanguageResource::Get(Data::RI_ERROR);

	m_btnRecord.GetWindowText(caption);
	

	if (caption == m_strRecord)
	{	
		//test
		WriteLog_("telephoneRecord_begin");
		WriteMemory();
		//

		if(g_iRecordCount >= 30000)
		{
			main->m_pDeleteTipDlg->SetPasswordModel(false);
			main->m_pDeleteTipDlg->SetTitle(str);
			main->m_pDeleteTipDlg->SetDelTip(Util::StringOp::ToCString(Data::LanguageResource::Get(Data::RI_SYSTEM_SOUND_TIP)));
			main->m_pDeleteTipDlg->ShowWindow_(SW_SHOW);
			return;
		}

		std::string path = GetSoundPath();
		ULARGE_INTEGER freeBytes;
		ULARGE_INTEGER totalBytes;
	
		GetDiskFreeSpaceEx(Util::StringOp::ToCString(path), &freeBytes, &totalBytes, NULL);

		int secondBytes = SECONDBYTES8;

		if (main->m_pSettingDlg->m_pSetting->isDeleteProtect())
		{
			secondBytes = SECONDBYTES8;
		}
		else
		{
			secondBytes = SECONDBYTES2;
		}
		
		//change by qi 20110707
		//if (path == ssFlashRecordPath)
		if (path.find(ssFlashRecordPath) != std::string::npos)
		{		
			if (freeBytes.QuadPart < PreFreeSize)
			{	
				main->m_pDeleteTipDlg->SetPasswordModel(false);
				main->m_pDeleteTipDlg->SetTitle(str);
				main->m_pDeleteTipDlg->SetDelTip(\
					Util::StringOp::ToCString(Data::LanguageResource::Get(Data::RI_SYSTEM_DISK)));
				main->m_pDeleteTipDlg->ShowWindow_(SW_SHOW);
				return;
			}
			
			m_uiRemainRecordSecond = (freeBytes.QuadPart - PreFreeSize) / secondBytes + 1;
		}
		else  //在SD卡上
		{
			if (freeBytes.QuadPart < sdPreFreeSize)
			{
				main->m_pDeleteTipDlg->SetPasswordModel(false);
				main->m_pDeleteTipDlg->SetTitle(str);
				main->m_pDeleteTipDlg->SetDelTip(Util::StringOp::ToCString(Data::LanguageResource::Get(Data::RI_SYSTEM_DISK)));
				main->m_pDeleteTipDlg->ShowWindow_(SW_SHOW);
				
				return;
			}

			m_uiRemainRecordSecond = (freeBytes.QuadPart - sdPreFreeSize) / secondBytes + 1;
		}
		
		//test
		WriteLog_("telephoneRecord_timer");
		WriteMemory();
		//

		SetTimer(12, 10, NULL);  //wzx20101220
		if (main->m_pSettingDlg->m_pSetting->isMustRecord())
		{
			m_btnRecord.ShowWindow(FALSE);
		}
		if(main->m_pSoundDlg->m_pPlaySoundDlg->IsWindowVisible())
		{
			main->m_pSoundDlg->m_pPlaySoundDlg->PostMessage(WM_CANCEL, 0, 0);
		}

	}
	else
	{
		if (RecStop())
		{
			//test
			WriteLog_("RecStop");
			WriteMemory();
			//

 			KillTimer(m_uiRecordTimer);

			if(main->m_pSettingDlg->m_pSetting->isPlayProtect()) 
			{
				std::string  fName=m_spSoundSegment.get()->filename();
				int index=fName.find('.')+1;
				fName=fName.substr((size_t)0,(size_t)index)+"spd";
				m_spSoundSegment.get()->filename(fName);
			}

			m_spSoundSegment->duration(m_uiRecordSecond);
			m_spSoundSegment->Update();

			//test
			WriteLog_("m_spSoundSegment->Update");
			WriteMemory();
			//
		}
	}
}

CString CTelephoneDlg::GenerateRecordName(BOOL isHighQuality, BOOL isProtect)
{
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	CString filename(L"");
	CTime t = CTime::GetCurrentTime();

	if(isHighQuality)
	{

		filename.Format(_T("%04d%02d%02d%02d%02d%02d"), t.GetYear(), t.GetMonth(),\
			t.GetDay(), t.GetHour(), t.GetMinute(), t.GetSecond());
		filename += L"_";
		if(main->m_pSettingDlg->m_pSetting->LocalNumber() != "")
		{	
			//add by qi 20111021
			//遇到*变成a ;
			std::string localNumber = main->m_pSettingDlg->m_pSetting->LocalNumber();
			replaceXing(localNumber);
			filename += Util::StringOp::ToCString(localNumber);
			
			// change by qi 20111021
			//filename += Util::StringOp::ToCString(\
				main->m_pSettingDlg->m_pSetting->LocalNumber());
		}
		else
		{
			filename += L"0";
		}
		filename += L"_";

		if(m_spContactInfo)
		{
			if(m_spContactInfo->telephoneNumber().number() != "")
			{	
				//add by qi 20111021
				//遇到*变成a ;
				std::string number = m_spContactInfo->telephoneNumber().number();
				replaceXing(number);		
				filename += Util::StringOp::ToCString(number);
				
				// change by qi 20111021
				//filename += Util::StringOp::ToCString(\
					m_spContactInfo->telephoneNumber().number());
			}
			else
			{
				filename += L"0";
			}
			filename += L"_";

			if(m_spContactInfo->type() == Data::citInOk)
			{
				filename += L"In";
			}
			else if(m_spContactInfo->type() == Data::citOut)
			{
				filename += L"Out";
			}
			else if(m_spContactInfo->type() == Data::citInNo)
			{
				if (gLangItem == 1)
					filename += L"留言";
				else
					filename += L"Message";
			}
		}
		else
		{
			filename += L"0";
		}
		
		if(isProtect)
			filename += L"HQ.spd";
		else
			filename += L"HQ.spx";
	}
	else
	{

		filename.Format(_T("%04d%02d%02d"), t.GetYear(), t.GetMonth(),t.GetDay());

		filename.Format(_T("%04d%02d%02d%02d%02d%02d"), t.GetYear(), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute(), t.GetSecond());
		filename += L"_";
		if(main->m_pSettingDlg->m_pSetting->LocalNumber() != "")
		{	
			//add by qi 20111021
			//遇到*变成a ;
			std::string localNumber = main->m_pSettingDlg->m_pSetting->LocalNumber();
			replaceXing(localNumber);		
			filename += Util::StringOp::ToCString(localNumber);
				
			// change by qi 20111021
			//filename += Util::StringOp::ToCString(\
			main->m_pSettingDlg->m_pSetting->LocalNumber());
		}
		else
		{
			filename += L"0";
		}

		filename += L"_";
		if(m_spContactInfo)
		{
			if(m_spContactInfo->telephoneNumber().number() != "")
			{	
				//add by qi 20111021
				//遇到*变成a ;
				std::string number = m_spContactInfo->telephoneNumber().number();
				replaceXing(number);
				filename += Util::StringOp::ToCString(number);
				
				// change by qi 20111021
				//filename +=\
				Util::StringOp::ToCString(m_spContactInfo->telephoneNumber().number());
			}
			else
			{
				filename += L"0";
			}
			filename += L"_";

			if(m_spContactInfo->type() == Data::citInOk)
			{
				filename += L"In";
			}
			else if(m_spContactInfo->type() == Data::citOut)
			{
				filename += L"Out";
			}
			else if(m_spContactInfo->type() == Data::citInNo)
			{
				if (gLangItem == 1)
					filename += L"留言";
				else
					filename += L"Message";
			}
		}
		else
		{
			filename += L"0";
		}

		if(isProtect)
			filename += L"ST.spd";
		else
			filename += L"ST.spx";
	}

	return filename;
}

void CTelephoneDlg::OnButtonTelephoneHandle()
{
	CString caption;

	m_btnHandle.GetWindowText(caption);

	if (caption == m_strHangOn)
	{
		((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->HungOn();
	}
	else 
	{
		((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->HungOff();

		m_pOggCodec->StopDecode(true);
		
	}	
}
///zzw//////////////////////010818
extern BOOL ISSpeedDial;
void CTelephoneDlg::HangOff_(void* param)
{
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	
	//test
	WriteLog_("hangoff_begin");
	WriteMemory();
	//

	theApp.m_bIsHungOnFireWall=FALSE;
    ISSpeedDial=TRUE;
	KillTimer(m_uiRecordTimer);
	KillTimer(IDC_TELDLGSHOW);
	KillTimer(m_uiTipTimer);
	KillTimer(m_uiTelephoneTimer);
	KillTimer(HUNGOFF_TIMER);
	KillTimer(12);
	KillTimer(m_uiToneDetect);

	waveInMessage(0, WAV_LOCAL_MIC, 0, 0);
	waveInMessage(0, WAV_SPEAKER_ONLY, 0, 0);
	main->phone_->StopRing(true);

	CString caption;
	m_btnRecord.GetWindowText(caption);

	if (caption == m_strStopRecord)
	{
		OnButtonTelephoneRecord();
	}
	
	if (m_spContactInfo)
	{
		SaveRecAndContactInfo();
	}

	m_uiRingCount = 0;
	m_bAddZero = FALSE;
	m_bKeyDial = FALSE;
	m_bHasCallID = FALSE;
	m_bPlayingLeaveWord = FALSE;
	m_bHasLeaveWord = FALSE;
	m_bFirwall = FALSE;
	m_sTipFilename = "";
	m_sRingFilename = "";
	m_bRingPlayed = FALSE;
	m_bRecording = FALSE;

	ShowWindow_(FALSE);
	m_sticDurationCaption.ShowWindow(FALSE);
	m_sticDuration.SetWindowText(_T(""));
	m_sticDuration.ShowWindow(FALSE);
	m_btnNote.ShowWindow(FALSE);
 	m_btnRecord.ShowWindow(FALSE);
	m_btnHandle.SetWindowText(m_strHangOn);
	m_sticNumber.SetWindowText(_T(""));
	m_sticName.SetWindowText(_T(""));
	m_sticDynName.SetWindowText(_T(""));
	m_sticMcaller.SetWindowText(_T(""));
	m_sticDynMcaller.SetWindowText(_T(""));
	m_sticCompany.SetWindowText(_T(""));
	m_sticDynCompany.SetWindowText(_T(""));
	m_sticDuty.SetWindowText(_T(""));
	m_sticDynDuty.SetWindowText(_T(""));
	m_bRecordNumber.Empty();
	m_IsRecordNumber=false;

	//////////////////////zzw100823/////////////////////////////////////////////
	if ((main->m_pSettingDlg->m_chbEnableRing.GetCheck()==BST_CHECKED)||
		main->m_pSettingDlg->m_pSetting->isFirewall())
	{
		main->phone_->OpenTelRing(FALSE);
	}
	else
	{
		main->phone_->OpenTelRing(TRUE);
	}
    
	int index=main->m_pSettingDlg->m_pSetting->soundVolume();
	main->phone_->RestorRing(index);

	//test
	WriteLog_("hangoff_end");
	WriteMemory();
	//

}

void CTelephoneDlg::HangOn_(void* param)
{
	//test
	WriteLog_("hang_on_begin");
	WriteMemory();
	//

	SetTitle(Data::RI_TEL_DIALTELSTC);

	m_btnHandle.SetWindowText(m_strHangOff);

	m_btnHandle.ShowWindow(TRUE);

	KeyBoardOff();
	
    m_btnNote.ShowWindow(TRUE);
	
	CreateContactInfo(Data::citOut);
	
	if (!((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pSetting->isMustRecord())
	{
		m_btnRecord.ShowWindow(TRUE);
	}
	else
	{
		OnButtonTelephoneRecord();
	}
	
	//test
	WriteLog_("hang_on_middle");
	WriteMemory();
	//


   /////////////zzw10824///////////////////////////////////////////////////
	if ((((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_chbEnableRing.GetCheck()==BST_CHECKED)||
		((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pSetting->isFirewall())
	{
		((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->OpenTelRing(FALSE);
		
	}else{
		
		((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->OpenTelRing(TRUE);
	}
  
	SetTimer(IDC_TELDLGSHOW, 5, NULL);	
	
	//test
	WriteLog_("hang_on_end");
	WriteMemory();
	//

}

void CTelephoneDlg::Key_(void* param)
{
	m_bKeyDial = TRUE;
	CString number;
	m_sticNumber.GetWindowText(number);
	number += char(param);
	if (number.GetLength() > TEL_SHOW_LENGTH)
	{
		m_sticNumber.SetWindowText(number.Right(TEL_SHOW_LENGTH));
	}
	else
	{
		m_sticNumber.SetWindowText(number);
	}

	if (((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->\
		m_pSettingDlg->m_pSetting->isAutoAppendOutlinePrefix())  //wzx 20101225
	{
		std::string out = ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pSetting->outlinePrefix();
		if(number == Util::StringOp::ToCString(out))
		{
			number = L"";
		}
	}
	
	if(m_spContactInfo)
	{
		m_spContactInfo->telephoneNumber(Util::StringOp::FromCString(number));
		m_spContactInfo->Update();
	}
}
void CTelephoneDlg::SubKey_(void* param)
{
	CString number;
	m_sticNumber.GetWindowText(number);
	number += char(param);
	if (number.GetLength() > TEL_SHOW_LENGTH)
	{
		m_sticNumber.SetWindowText(number.Right(number.GetLength() - TEL_SHOW_LENGTH));
	}
	else
	{
	   m_sticNumber.SetWindowText(number);
	}	
}
void CTelephoneDlg::Connect_(void* param)
{   
	m_bConnected = TRUE;

	if (m_bPlayingLeaveWord)
	{
		m_btnHandle.SetWindowText(m_strHangOff);

		m_btnHandle.ShowWindow(TRUE);

		m_btnNote.ShowWindow(TRUE);

		m_btnRecord.ShowWindow(FALSE);

		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pFSM->setStartState(CMultimediaPhoneDlg::tsRing);
		
		return;
	}

	KillTimer(HUNGOFF_TIMER);
	
	((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->StopRing(true);

	if (((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pSetting->isMustRecord())
	{
		CString caption;

		m_btnRecord.GetWindowText(caption);

		if(m_spContactInfo->type() == Data::citInNo)		////lxz 20101206
			m_spContactInfo->type(Data::citInOk);

		//add by qi 20111102
		//没有录音开始录音
		if (!m_bRecording)
		{
			OnButtonTelephoneRecord();
		}
	}

	SetTitle(Data::RI_TEL_TELINGSTC);

	if (!m_sticDurationCaption.IsWindowVisible())
	{
		m_sticDurationCaption.ShowWindow(TRUE);
		
	    m_sticDuration.ShowWindow(TRUE);
	}

	m_btnNote.ShowWindow(TRUE);

	if (!((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pSetting->isMustRecord())
	{
	   m_btnRecord.ShowWindow(TRUE);
	}

	m_btnHandle.SetWindowText(m_strHangOff);

	m_btnHandle.ShowWindow(TRUE);

	m_sticDuration.ShowWindow(TRUE);

	m_sticDuration.SetWindowText(_T(""));

	if (m_spContactInfo->type() == Data::citOut)
	{
		CString number;
		m_sticNumber.GetWindowText(number);
		theApp.IsCheckTakingState=true;
		KillTimer(CHECKLINESTATE);
	    SetTimer(CHECKLINESTATE,5000,NULL);
		///zzw10824///////////////////////////////////////
		if(((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_chbEnableOutLine.GetCheck() == BST_CHECKED)
		{
			CString outLineNumber;
            ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_edtOutLine.GetWindowText(outLineNumber);
			outLineNumber.TrimLeft();
		    outLineNumber.TrimRight();
			if (!outLineNumber.IsEmpty())
			{
				int len = outLineNumber.GetLength();
				if(number.Left(len) == outLineNumber)
				{
					number=number.Mid(len);
				}
			}
		}
		
		if(((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_chbEnableIPDial.GetCheck() == BST_CHECKED)
		{   
			CString IPDial;
           ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_edtIPDial.GetWindowText(IPDial);
		    IPDial.TrimLeft();
		    IPDial.TrimRight();
			if (!IPDial.IsEmpty())
			{
				int len=IPDial.GetLength();
				int endindex=number.Find(IPDial);
				if(endindex!=-1)
				{ 
					number=number.Mid(len);
				}	
			}  
		}

		if(m_bAddZero)
		{
			if(number.Left(1) == L"0")
			{
				number=number.Mid(1);
			}
		}

		std::string fnumber = Util::StringOp::FromCString(number);
		m_spContactInfo->startTime(CTime::GetCurrentTime());
		std::vector<boost::shared_ptr<Data::Contact> > result = FindCallContact(fnumber);
		Data::Contact * pContact = result.empty() ? NULL : result[0].get();
		UpdateCurContactInfo(pContact, fnumber);
		if(m_bKeyDial)  //wzx 20110113
		{
			m_sCity = "";
			if(fnumber.size() >= 3)
			{
				std::string addr = ParseDialNumber(fnumber);
				if (!addr.empty())
				{
					if(!isMobileNumber(fnumber))
					{
						CString space = Util::StringOp::ToCString(addr);
						space.TrimLeft();  //去掉最左端的空格
						addr = Util::StringOp::FromCString(space);
						int pos = addr.find(' ');
						m_sCity = addr.substr(0, pos);
					}
					else
					{
						addr = addr.substr(8, 24);
						int index = addr.find(' ');
						m_sCity = addr.substr(0, index);
						CString space = Util::StringOp::ToCString(m_sCity);
						space.TrimRight();  //去掉最右端的空格
						m_sCity = Util::StringOp::FromCString(space);
					}
				}
			}
		}
		ShowContact(pContact, fnumber);
	}
	else
	{
		m_spContactInfo->type(Data::citInOk);
	}
     
	SetTimer(m_uiTelephoneTimer, 1000, 0);
}

void CTelephoneDlg::Ring_(void* param)
{  
	//test
	WriteLog_("Ring_begin");
	WriteMemory();
	//

	CreateContactInfo(Data::citInNo);
	m_bKeyDial = FALSE;
	// 之前没弹过框则弹框
	if ((m_uiRingCount == 0) && !m_bHasCallID)
	{
		PopupWindow(Data::RI_TEL_NEWTELSTC, m_strHangOn);
	}

    /////zzw100826/////////////////////////
	theApp.m_bIsHungOnFireWall=TRUE;

	++m_uiRingCount;

	if (m_bHasCallID)
	{
		// 收到Ring消息则表明之前的CallID一定已经结束.
		m_bCallIDFinished = TRUE;
	}

	//test
	WriteLog_("m_uiRingCount == 1");
	WriteMemory();
	//

	if (m_uiRingCount == 1)
	{
		if (!m_bHasCallID)
		{
			UpdateFirewallStatus(FALSE);
		}
		else if(m_bHasCallID ||\
			!((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->\
			m_pSettingDlg->m_pSetting->isUseSpecRing())		 //lxz 20110210
			OpenRing();    //lxz 20110210

		m_bConnected = FALSE;
		return;
	}
	else if(m_uiRingCount == 2)				//lxz 20110210
		OpenRing(); 

	if (m_bFirwall)
	{
		return;
	}
	
	//test
	WriteLog_("isAutoReply");
	WriteMemory();
	//

	if (((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->\
		m_pSetting->isAutoReply())//自动答录
	{  
		std::vector<boost::shared_ptr<Data::SoundSegment> > result;
		

		if (m_uiRingCount - 1 ==  ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pSetting->autoReplyRingCount())
		{	

			if (m_sTipFilename == "")//提示语
			{   
				m_sTipFilename = ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pSetting->defaultTipFilename();
							
				if ((Util::StringOp::ToCString(m_sTipFilename).Right(4).CompareNoCase(_T(".mp3")) != 0)
			         &&(Util::StringOp::ToCString(m_sTipFilename).Right(4).CompareNoCase(_T(".wav")) != 0))				//if (m_sTipFilename.find('.')==-1)
				{	
                    
					//change by qi 20110625
					//int index=m_sTipFilename.rfind("\\")+1;
				    //m_sTipFilename=m_sTipFilename.substr(index);
					//result=Data::SoundSegment::GetFromDatabase("type = 2 AND tipName = '" + m_sTipFilename + "'");	
					
					// 	if (result.size()>0)
					// 						{
					// 	m_sTipFilename=result[0]->filename();
					// 							
					// 	}
					//
					
					//add by qi 20110625
					result=Data::SoundSegment::GetFromDatabase("type = 2\
						AND tipName = '" + Data::LanguageResource::Get(Data::RI_SOUND_LEAVEWORDTIP) + "'");	
				
					if (result.size() == 0)
					{
						if (gLangItem == Data::lEnglish)
						{
							m_sTipFilename = "\\Hive\\my_tip\\tip4.wav"; 
						}
						else
						{
							m_sTipFilename = "\\Hive\\my_tip\\tip9.wav"; 
						}
					}
					//
				}
				
			}
			else
			{	

				if ((Util::StringOp::ToCString(m_sTipFilename).Right(4).CompareNoCase(_T(".mp3")) != 0)
					&&(Util::StringOp::ToCString(m_sTipFilename).Right(4).CompareNoCase(_T(".wav")) != 0))				//if (m_sTipFilename.find('.')==-1)
				{	
					result=Data::SoundSegment::GetFromDatabase("type = 2 AND filename = '" + m_sTipFilename + "'");	
					if (result.size() > 0)
					{
						m_sTipFilename=result[0]->filename();

					}
				}
			}

		    CFileStatus status;
			if (CFile::GetStatus(LPCTSTR(Util::StringOp::ToCString(m_sTipFilename)),status))
			{
				::KillTimer((theApp.m_pMainWnd)->m_hWnd, ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_uiRingTimer);
		
				((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->StopRing(true);

				((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->OpenTelRing(FALSE);

				m_bPlayingLeaveWord = TRUE;
				
				//test
				WriteLog_("HangOnToRecord");
				WriteMemory();
				//

				HangOnToRecord();//是否自动外放

				int second = 9;//提示音5s
				
				if((m_sTipFilename.rfind(".spx") != std::string::npos)||(m_sTipFilename.rfind(".spd") != std::string::npos)) 
				{
					if (result.size() > 0)
					{
						second = result[0]->duration();

						m_pOggCodec->StartDecode(m_sTipFilename);//播放提示语

					}

					SetTimer(m_uiTipTimer, second * 1000, 0);
				}
				else
				{	
					//test
					WriteLog_("StartRing");
					WriteMemory();
					//

					((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->\
						StartRing((LPTSTR)(LPCTSTR)Util::StringOp::ToCString(\
						m_sTipFilename), 1);

					std::string s = m_sTipFilename.substr(m_sTipFilename.length() - 5, 1);//预置的提示语文件在my_tip目录下，文件名最后一位为该提示音时长秒数，如tip5.wav，为5秒时长，方便以后继续添加
					int se = atoi(Util::StringOp::ToCString(s));
					if (se != 0)
					{
						second = se;
					}

					SetTimer(m_uiTipTimer, second * 1000, 0);//忙音检查
				
				}	
			}
		}
	}

	//test
	WriteLog_("Ring_end");
	WriteMemory();
	//
}

////////////////来电//////////////////////
void CTelephoneDlg::CallID_(void* param)
{
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	
	//test
	WriteLog_("callID_begin");
	WriteMemory();
	//

	// 如果已经能够确定之前CallID已经结束(通过Ring消息判断),则认为这是新的来电.
	if (m_bHasCallID && m_bCallIDFinished)
	{
		HangOff_(NULL);
	}

	// 如果之前没有弹过框,则弹框
	if ((m_uiRingCount == 0) && !m_bHasCallID)
	{
		PopupWindow(Data::RI_TEL_NEWTELSTC, m_strHangOn);
	}

	m_bHasCallID = TRUE;
	m_bCallIDFinished = FALSE;

	// 将新的CallID拼到原来的值的后面
	CALLID_INFO* info = (CALLID_INFO*)param;
	std::string curNum;
	if (m_spContactInfo)
	{
		//curNum = m_spContactInfo->telephoneNumber().number();
	}
	std::string tel = curNum + info->number;

	if(tel.length() > 8)
	{
		std::string b = tel.substr(0, 1);
		
		//号码的第一位不是0,1，自动加0
		if(tel[0] != '0' && tel[0] != '1')
		{
			tel.insert(0, '0');
		}
	}
	
	std::string mobile("");
	std::string code("");
	std::string addr("");
	m_sCity = "";
	if (main->m_pSettingDlg->m_pSetting->thisTelephoneNumber().number().length() >= 2)
	{
		code = main->m_pSettingDlg->m_pSetting->thisTelephoneNumber().number();
		if (code[0] != '0')
		{
			//在设置里设置的区号没加0，自动加0
			code = "0" + code;
		}
	}
	
	//test
	WriteLog_("callID_mobile=tel");
	WriteMemory();
	//

	mobile = tel;
	if (!mobile.empty())
	{	
		//change by qi 20111019
		//大于9位号码去检查区号，小于9位当做本地处理
		if (mobile.size() > 9)
		{
			addr = ParseDialNumber(mobile);			
			if (!addr.empty())
			{
				if (!code.empty() && addr.substr(23, code.length()) == code)
				{
					if (tel[0] == '0')
					{
						tel = tel.substr(1);
					}
				}

				if(mobile[0] == '0')
				{
					CString space = Util::StringOp::ToCString(addr);
					space.TrimLeft();  //去掉最左端的空格
					addr = Util::StringOp::FromCString(space);
					int pos = addr.find(' ');
					m_sCity = addr.substr(0, pos);
				}
				else if(mobile[0] == '1')
				{
					addr = addr.substr(8, 24);
					int index = addr.find(' ');
					m_sCity = addr.substr(0, index);
					CString space = Util::StringOp::ToCString(m_sCity);
					space.TrimRight();  //去掉最右端的空格
					m_sCity = Util::StringOp::FromCString(space);
				}
			}

		}
 		else
 		{	
 			//如果号码第一位是0，去掉铃
 			if (tel[0] == '0')
 			{
 				tel = tel.substr(1);
 			}
 			m_sCity = "本地";
 		}

	}
	
	//test
	WriteLog_("callID_FindCallContact");
	WriteMemory();
	//

	std::vector<boost::shared_ptr<Data::Contact> > contacts = FindCallContact(tel);
	
//	printf("tel %s\r\n", tel.c_str());
	
	Data::Contact * pContact = contacts.empty() ? NULL : contacts[0].get();
	CreateContactInfo(Data::citInNo);
	UpdateCurContactInfo(pContact, tel);
	UpdateFirewallStatus(pContact);

	ShowContact(pContact, tel);
	
	//test
	WriteLog_("defaultTipFilename");
	WriteMemory();
	//
	
	m_sTipFilename = main->m_pSettingDlg->m_pSetting->defaultTipFilename();
	m_sRingFilename = main->m_pSettingDlg->m_pSetting->defaultRingFilename();

	CFileStatus status;
	if (!m_bFirwall && pContact && pContact->group())
	{
		std::string tipname = pContact->group()->tipName();
		if (tipname != Data::LanguageResource::Get(Data::RI_CARD_DEFAULT))
		{
			if (CFile::GetStatus(LPCTSTR(Util::StringOp::ToCString(tipname)),status))
			{
				m_sTipFilename = tipname;
			}
		}

		std::string ringname = pContact->group()->ringName();
		if (ringname != Data::LanguageResource::Get(Data::RI_CARD_DEFAULT))
		{
			if (CFile::GetStatus(LPCTSTR(Util::StringOp::ToCString(ringname)),status))
			{
				m_sRingFilename = ringname;
			}
		}
	}

	if (!CFile::GetStatus(LPCTSTR(Util::StringOp::ToCString(m_sTipFilename)),status))
	{	
		if (gLangItem == Data::lEnglish)
		{
			m_sTipFilename = "\\Hive\\my_tip\\tip4.wav"; 
		}
		else
		{
			m_sTipFilename = "\\Hive\\my_tip\\tip9.wav"; 
		}
	}

	//lxz 20110210
	if(m_uiRingCount == 1)
	{
		OpenRing();
	}

	//test
	WriteLog_("callID_end");
	WriteMemory();
	//
	
//	printf("m_sRingFilename %s\r\n", m_sRingFilename.c_str());
}

void CTelephoneDlg::CreateContactInfo(Data::ContatcInfoType type)
{
	if (!m_spContactInfo)
	{	
		m_spContactInfo = boost::shared_ptr<Data::ContactInfo>(new Data::ContactInfo);
		m_spContactInfo->startTime(CTime::GetCurrentTime());
		m_spContactInfo->type(type);
		m_spContactInfo->Insert();
	}
}

// 打开铃声.
void CTelephoneDlg::OpenRing()
{
	if (!m_bFirwall && !m_bRingPlayed && !m_bConnected)
	{
		m_bRingPlayed = TRUE;
		
		if (((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pSetting->isUseSpecRing())
		{
			if (m_sRingFilename.empty())
			{
				m_sRingFilename = ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pSetting->defaultRingFilename();
			}
			CFileStatus status;
			if (CFile::GetStatus(LPCTSTR(Util::StringOp::ToCString(m_sRingFilename)),status))
			{
				((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->StartRing((LPTSTR)(LPCTSTR)Util::StringOp::ToCString(m_sRingFilename));
			}
			else
			{
				((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->OpenTelRing(TRUE);
			}
		}
		else
		{
			((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->OpenTelRing(TRUE);
		}	
	}
}

void CTelephoneDlg::UpdateFirewallStatus(Data::Contact * pContact)
{
//	printf("UpdateFirewallStatus %x\r\n", pContact);

	m_bFirwall = FALSE;
	boost::shared_ptr<Data::Setting> pSetting = 
		((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pSetting;
	if (pSetting->isFirewall())
	{
		int firetype = pSetting->firewallType();

		if (pContact)
		{
		//	printf("firetype pContact->type() %x %x\r\n", firetype, pContact->type());

			if (((firetype == 0) && (pContact->type() == Data::ctBlacklist))
				|| ((firetype == 1) && (pContact->type() == Data::ctBlacklist))
				|| ((firetype == 2) && (pContact->type() != Data::ctVip))
				|| (firetype == 3))
			{
				m_bFirwall = TRUE;
			}
		}
		else if ((firetype == 1) || (firetype == 2) || (firetype == 3))
		{
		//	printf("firetype %x\r\n", firetype);
			m_bFirwall = TRUE;
		}
	}

//	printf("m_bFirwall %x\r\n", m_bFirwall);
}

void CTelephoneDlg::UpdateCurContactInfo(Data::Contact * pContact, std::string telNum)
{
	m_spContactInfo->telephoneNumber(Data::TelephoneNumber(telNum));
	m_spContactInfo->contactId(pContact ? pContact->id() : 0);
	m_spContactInfo->name(pContact ? pContact->name() : "");
}
void CTelephoneDlg::HangOnInRecord(void)
{
	//if (!m_bAutoRecord)
	if (m_spContactInfo && (m_spContactInfo->type() == Data::citOut))
		return;
	if (m_bPlayingLeaveWord)
	{
		KillTimer(m_uiTipTimer);
		m_pOggCodec->StopDecode(true);
		m_bPlayingLeaveWord = FALSE;
	}
	else
	{
		KillTimer(HUNGOFF_TIMER);
		bVoiceStop = true;
	}

	if (m_spSoundSegment)
	{
		m_spSoundSegment->type(Data::sstTeleRecord);
		m_spSoundSegment->played(true);
	}

	if (m_spContactInfo)
	{
		m_spContactInfo->type(Data::citInOk);
	}

	m_bPlayingLeaveWord = FALSE;
	((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pFSM->fireEvent(CMultimediaPhoneDlg::teHangOn, NULL);
}

void CTelephoneDlg::HangOnToRecord(void)
{
	((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->HungOnToRecord();	

	waveInMessage(0,WAV_LINEOUT_ONLY,0,0);
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

void CTelephoneDlg::ShowContact(Data::Contact * pContact, std::string number)
{   
	std::string strANSI;
	CString str;
	if (number.length() > 0)
	{
		strANSI = Data::LanguageResource::Get(Data::DETAIL_NUMBER);
		str = strANSI.c_str();
		str += L":  ";
		str += CString(number.c_str());
		m_sticNumber.SetWindowText(str);
	}
	else
	{
		m_sticNumber.SetWindowText(L"");
	}
	
	if (!m_sCity.empty())
	{
		CString strMcaller = Data::LanguageResource::Get(Data::MCALLER).c_str();
		m_sticMcaller.SetWindowText(strMcaller);
		m_sticDynMcaller.SetWindowText(Util::StringOp::ToCString(m_sCity));
	}
	else
	{
		m_sticMcaller.SetWindowText(L"");
		m_sticDynMcaller.SetWindowText(L"");
	}
	
	if (pContact)
	{
		// 名字
		strANSI = Data::LanguageResource::Get(Data::RI_CARD_NAMESTC);
		str = strANSI.c_str();
		str += L":";
		m_sticName.SetWindowText(str);
		
		str = pContact->name().c_str();
		m_sticDynName.SetWindowText(str);

		// 公司
		strANSI = Data::LanguageResource::Get(Data::RI_CARD_COMPANYSTC);
		str = strANSI.c_str();
		str += L":";
		m_sticCompany.SetWindowText(str);
		
		str = pContact->company().c_str();
		if (str.GetLength() > 12)
		{
			str = str.Left(7) + L"...";
		}
		m_sticDynCompany.SetWindowText(str);
		
		// 称呼
		strANSI = Data::LanguageResource::Get(Data::RI_CARD_TITLESTC);
		str = strANSI.c_str();
		str += L":";
		m_sticDuty.SetWindowText(str);

		str = pContact->duty().c_str();
		if (str.GetLength() > 12)
		{
			str=str.Left(7) + L"...";
		}
		m_sticDynDuty.SetWindowText(str);
	}
	else
	{
		m_sticName.SetWindowText(L"");
		m_sticDynName.SetWindowText(L"");

		m_sticCompany.SetWindowText(L"");
		m_sticDynCompany.SetWindowText(L"");

		m_sticDuty.SetWindowText(L"");
		m_sticDynDuty.SetWindowText(L"");
	}
}
int counter=0;
int Tcounter=0;
void CTelephoneDlg::TakeLine()
{   
	if ((((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pFSM->getCurrentState() != CMultimediaPhoneDlg::tsHangOff)
		&& (((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pFSM->getCurrentState() != CMultimediaPhoneDlg::tsHangOn))
		     return ;
	if (((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pFSM->getCurrentState() == CMultimediaPhoneDlg::tsHangOff)
	{  
		Tcounter++;
		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->phone_->HungOn();
		
		Sleep(1000);
	}
	
}
void CTelephoneDlg::DialTelePhoneNumber(Data::TelephoneNumber telephoneNumber,int contactId /* = 0 */)
{
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	
	//test
	WriteLog_("DialTelePhoneNumber");
	WriteMemory();
	//

	std::string number = telephoneNumber.number();
	setCheckTakingstate(false);
    KillTimer(CHECKLINESTATE);
	counter=0;
	m_sCity = "";

	if (main->m_pSettingDlg->m_pSetting->isAutoAppendOutlinePrefix())
	{
		if(number.size() > 4 || number == "110" || number == "112" || number == "114" || number == "119" || number == "120"
			|| number == "121" || number == "122" || number == "2580" || number == "2585")
		{
			std::string out = main->m_pSettingDlg->m_pSetting->outlinePrefix();
			main->phone_->DialNumber((char*)out.c_str());
			Sleep(2000);
		}
	}

	if(main->m_pSettingDlg->m_pSetting->thisTelephoneNumber().number().length() > 2)
	{
		//wzx 20110104
		std::string ipNumber = main->m_pSettingDlg->m_pSetting->ipPrefix();
		if(number.size() >= ipNumber.size() && number.substr(0, ipNumber.size()) == ipNumber)
		{
			number = number.substr(ipNumber.size());
		}
		BOOL isMobile = isMobileNumber(number);
		std::string addr("");
		std::string code = main->m_pSettingDlg->m_pSetting->thisTelephoneNumber().number();
		if (code[0] != '0')
		{
			code = "0" + code;
		}
		int codeLen = code.length();
		if (number.substr(0, codeLen) == code)
		{
			number = number.substr(codeLen);
			addr = ParseDialNumber(code);
		}
		else
		{
			if (isMobile && number[0] == '0')
			{
				number = number.substr(1);
			}

			addr = ParseDialNumber(number);
			if (!addr.empty() && isMobile)
			{
				if (addr.substr(23, codeLen) == code)
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
						m_bAddZero = TRUE;
					}
				}
			}
		}

		if(!addr.empty())
		{
			if(isMobile)
			{
				addr = addr.substr(8, 24);
				int index = addr.find(' ');
				m_sCity = addr.substr(0, index);
				CString space = Util::StringOp::ToCString(m_sCity);
				space.TrimRight();  //去掉最右端的空格
				m_sCity = Util::StringOp::FromCString(space);
			}
			else
			{
				CString space = Util::StringOp::ToCString(addr);
				space.TrimLeft();  //去掉最左端的空格
				addr = Util::StringOp::FromCString(space);
				int pos = addr.find(' ');
				m_sCity = addr.substr(0, pos);
			}
		}
	}

	if (main->m_pSettingDlg->m_pSetting->isAppendIpPrefix())
	{
		if (number[0] == '0')
		{
			number = main->m_pSettingDlg->m_pSetting->ipPrefix() + number;
		}
	}

	m_sticNumber.SetWindowText(CString(number.c_str()));
	CreateContactInfo(Data::citOut);
	m_spContactInfo->telephoneNumber(number);

	if (!IsWindowVisible())
	{  
		SetTitle(Data::RI_TEL_NEWTELSTC);
		m_btnHandle.SetWindowText(m_strHangOn);
		m_btnHandle.ShowWindow(TRUE);
		ShowWindow_(TRUE);
		main->EnableLine(TRUE);		
	}
	
	if (!main->m_pSettingDlg->m_pSetting->isMustRecord())
	{
		m_btnRecord.ShowWindow(TRUE);
	}
	else
	{	
		//add by qi 20111102
		//没有录音开始录音
		if (!m_bRecording)
		{
			OnButtonTelephoneRecord();
		}
	}
	
	//test
	WriteLog_("DialTele_OnButtonTelephoneRecord");
	WriteMemory();
	//

	//////////////////zzw10824//////////////////////////////////////////////////////////
	if ((main->m_pSettingDlg->m_chbEnableRing.GetCheck()==BST_CHECKED)||
		main->m_pSettingDlg->m_pSetting->isFirewall())
	{
		main->phone_->OpenTelRing(FALSE);
	}
	else
	{
		main->phone_->OpenTelRing(TRUE);
	}

	//test
	WriteLog_("DialTele_OnButtonTelephoneRecord_END");
	WriteMemory();
	//
	SetTimer(DIAL_TIMER, 500, NULL);

}
void CTelephoneDlg::DialContact(Data::TelephoneNumber telephoneNumber, int contactId)
{   
	//test
	WriteLog_("dialContact_Begin");
	WriteMemory();
	//

	/////////////////zzw100825//////////////////////////////////////////////
	if (((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pFSM->getCurrentState()==CMultimediaPhoneDlg::tsCallID||
		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pFSM->getCurrentState()==CMultimediaPhoneDlg::tsConnect||
		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pFSM->getCurrentState()==CMultimediaPhoneDlg::tsRing)
	{
		return;
	}
  ///////////////////////////////////////////////////////////////////////////
	if(counter>3)
	{
	    KillTimer(CHECKLINESTATE);
		KillTimer(DIAL_TIMER);
		counter=0;
		return;
	}

	m_bKeyDial = FALSE;
	TakeLine();

	if(getCheckTakingState())
	{
		DialTelePhoneNumber(telephoneNumber,contactId);
	}
	else
	{ 
		counter++;
		DialContact(telephoneNumber, contactId);
	}

}

UINT CTelephoneDlg::GetUnconnectCount(void)
{	
	//change by qi 20111105
	return	Data::ContactInfo::GetDataCount("type = 0 AND played = 0");

// 	std::vector<boost::shared_ptr<Data::ContactInfo> > result = \
// 		Data::ContactInfo::GetFromDatabase("type = 0 AND played = 0");	
// 	m_uiInNoCount = result.size();
// 	return m_uiInNoCount;
	
}

UINT CTelephoneDlg::GetRecordCount(void)
{	
	//change by qi 20111105
	return	Data::SoundSegment::GetDataCount("type = 0 AND played = 0");

// 	std::vector<boost::shared_ptr<Data::SoundSegment> > result =\
// 		Data::SoundSegment::GetFromDatabase("type = 0 AND played = 0");		
// 	m_uiRecordCound = result.size();
// 	return m_uiRecordCound;

}

void CTelephoneDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent == m_uiTelephoneTimer)
	{
		++m_uiTelephoneSecond;
		CString duration;
		if (m_uiTelephoneSecond > 3600)
		{
			duration.Format(_T("%02d:%02d:%02d"), m_uiTelephoneSecond / 3600, m_uiTelephoneSecond % 3600 / 60, m_uiTelephoneSecond % 60);
		}
		else
		{
			duration.Format(_T("%02d:%02d"), m_uiTelephoneSecond / 60, m_uiTelephoneSecond % 60);
		}
		m_sticDuration.SetWindowText(duration);

		if ((m_uiTelephoneSecond % 60) == 0)
		{
			m_spContactInfo->duration(m_uiTelephoneSecond);
			m_spContactInfo->Update();
		}	
	}
	else if (nIDEvent == m_uiRecordTimer)
	{
		++m_uiRecordSecond;

		CString duration;

		if (m_uiRecordSecond > 3600)
		{
			duration.Format(_T("%02d:%02d:%02d"), m_uiRecordSecond / 3600, m_uiRecordSecond % 3600 / 60, m_uiRecordSecond % 60);
		}
		else
		{
			duration.Format(_T("%02d:%02d"), m_uiRecordSecond / 60, m_uiRecordSecond % 60);
		}

		m_sticRecord.SetWindowText(duration);

		if ((m_uiRecordSecond % 60) == 0)
		{
			m_spSoundSegment->duration(m_uiRecordSecond);
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
		((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->StopRing(true);
		m_pOggCodec->StopDecode(true);
		waveInMessage(0,WAV_LINEOUT_ONLY,0,0);
		((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->\
			StartRing(_T("\\Hive\\tip2.wav"), 1);
		Sleep(1000);
		m_bHasLeaveWord = TRUE;
        OnButtonTelephoneRecord();//开始录音
		int max = ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->\
			m_pSetting->maxSoundUseSize();//答录时长
		if(max == -1)
		{
			max = ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->\
				m_pMainDlg->m_nRdTime/1000;
		}
		
		//test
		WriteLog_("HUNGOFF_TIMER");
		WriteMemory();
		//

		//change by qi 20110624
		SetTimer(HUNGOFF_TIMER, max * 1000, 0);
		//SetTimer(HUNGOFF_TIMER, 10 * 1000, 0);	//wzx test
	}
	else if(nIDEvent == m_uiToneDetect)
	{
		KillTimer(m_uiToneDetect);
		
		//test
		WriteLog_("start_VoiceCheckThreadPrc");
		WriteMemory();
		//

		if (m_hSABTD != NULL)//
		{
			bVoiceStop = false;
			HANDLE hCheckThread = CreateThread(\
				NULL, 0, (LPTHREAD_START_ROUTINE)VoiceCheckThreadPrc, NULL, 0, NULL);
			if (hCheckThread == INVALID_HANDLE_VALUE)
			{
				bVoiceStop = true;
			}
			CloseHandle(hCheckThread);
		}

		//test
		WriteLog_("start_CloseHandle");
		WriteMemory();
		//

	}
	else if(nIDEvent == IDC_TELDLGSHOW)//弹出窗口
	{
		KillTimer(IDC_TELDLGSHOW); 
        ShowWindow_(TRUE);  
	}
	else if (nIDEvent == DIAL_TIMER)//拨打电话
	{
		KillTimer(DIAL_TIMER);

		if(m_spContactInfo)
		{	
			//test
			WriteLog_("DialTele_DIAL_TIMER");
			WriteMemory();
			//

			 std::string number = m_spContactInfo->telephoneNumber().number();
			((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->phone_->\
				DialNumber((char*)number.c_str());
			((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pFSM->\
				setStartState(CMultimediaPhoneDlg::tsKey);
			((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pFSM->\
				fireEvent(CMultimediaPhoneDlg::teConnect, NULL);
			
			//test
			WriteLog_("DialTele_DIAL_TIMER_End");
			WriteMemory();
			//

		}
	}
	else if (nIDEvent == HUNGOFF_TIMER)//挂机事件
	{
		KillTimer(HUNGOFF_TIMER);
		//m_uiRecordSecond = ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pSetting->maxSoundUseSize();
		m_uiRecordSecond = 10;	//wzx test
		((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->HungOff();	

	}else if (nIDEvent==CHECKLINESTATE)
	{
        ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->CheckForceHungOff();
   
	}else if (nIDEvent==CHECKLINESTATEIDlE)
	{ 
	    KillTimer(CHECKLINESTATEIDlE);
		SetTimer(CHECKLINESTATE,200,NULL);
	}
	else if(12 == nIDEvent)
	{
		KillTimer(12);
		CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
		if (RecStart())
		{
			//test
			WriteLog_("telephoneRecord_RecStart");
			WriteMemory();
			//

			m_spSoundSegment = boost::shared_ptr<Data::SoundSegment>(\
				new Data::SoundSegment);
			m_vSoundSegment.push_back(m_spSoundSegment);

			CString filename;
			if (main->m_pSettingDlg->m_pSetting->isDeleteProtect())
			{
				m_pOggCodec->SetQuality(8);

				if(main->m_pSettingDlg->m_pSetting->isPlayProtect())
				{//wzx20101213
					filename = GenerateRecordName(TRUE, TRUE);
				}
				else
				{
					filename = GenerateRecordName(TRUE, FALSE);
				}
			}
			else
			{
				m_pOggCodec->SetQuality(2);
				if(main->m_pSettingDlg->m_pSetting->isPlayProtect())
				{
					filename = GenerateRecordName(FALSE, TRUE);
				}
				else
				{
					filename = GenerateRecordName(FALSE, FALSE);
				}
			}

			m_sRecordFileName = GetSoundPath() + Util::StringOp::FromCString(filename);
			m_spSoundSegment->filename(m_sRecordFileName);
			m_spSoundSegment->startTime(CTime::GetCurrentTime());		
			m_spSoundSegment->contactInfoId(m_spContactInfo->id());
			m_spSoundSegment->telephoneNumber(m_spContactInfo->telephoneNumber());
			m_spSoundSegment->name(m_spContactInfo->name());
			
			//test
			WriteLog_("m_spSoundSegment->name");
			WriteMemory();
			//

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

			std::string comment = m_spSoundSegment->name() + "\'" +\
				m_spSoundSegment->telephoneNumber().number() + "\'" + \
				Util::StringOp::FromInt(m_spSoundSegment->type()) + "\'" +\
				Util::StringOp::FromTimestamp(m_spSoundSegment->startTime()) + "\'";

			if (m_pOggCodec->StartEncode(m_sRecordFileName, ""))
			{	
				//test
				WriteLog_("StartEncode");
				WriteMemory();
				//

				m_uiRecordSecond = 0;
				SetTimer(m_uiRecordTimer, 1000, 0);
				if(!bHasTDThread)
					SetTimer(m_uiToneDetect, 10, NULL);  //设置忙音检测定时器
				m_btnRecordTip.ShowWindow(TRUE);
				m_sticRecordCaption.ShowWindow(TRUE);
				m_sticRecord.ShowWindow(TRUE);
				m_btnRecord.SetWindowText(m_strStopRecord);
			}
		}
		
	}
	else if(14 == nIDEvent)
	{
		KillTimer(14);
		if(!m_sDeleteFile.empty())
		{
			for(std::vector<std::string>::iterator iter=m_sDeleteFile.begin(); iter != m_sDeleteFile.end(); ++iter)
			{
				CString name((*iter).c_str());
				CFileStatus status;
				if (CFile::GetStatus((LPCTSTR)name,status))
				{
					try
					{
						CFile::Remove(name);
					}
					catch (CFileException* pEx)
					{
						pEx->Delete();
					}
				}
			}
			m_sDeleteFile.clear();
		}
	}

	/*CCEDialog*/CCEDialog::OnTimer(nIDEvent);
}

bool CTelephoneDlg::RecStart(void)
{
	// TODO: Add your control notification handler code here
	WaitForSingleObject(g_hThreadExit, 4000);

	if (waveInOpen(&m_hWaveIn,0,&m_waveform,(DWORD)this->m_hWnd,NULL,CALLBACK_WINDOW))
	{
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
		
	// Begin sampling
	m_bEnding = FALSE ;
	m_bRecording = TRUE;
	m_dwDataLength = 0 ;
	waveInStart(m_hWaveIn) ;
	return true;
}

bool CTelephoneDlg::RecStop(void)
{	
	m_bRecording = FALSE;
	m_bEnding=TRUE;
 	waveInReset(m_hWaveIn);
	TRACE(L"RecStop!!!\n");
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
}

void CTelephoneDlg::OnMM_WIM_DATA(UINT wParam, LONG lParam) 
{
	Sleep(0);   //lxz20101220
 	OggCodec::PcmData * pd = new OggCodec::PcmData;
	if (pd != NULL)
	{
		pd->dataLength = ((PWAVEHDR) lParam)->dwBytesRecorded / sizeof(short);
		pd->data = (short*)malloc(((PWAVEHDR) lParam)->dwBytesRecorded);
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

	if (!bVoiceStop)
	{
		OggCodec::PcmData * pd = new OggCodec::PcmData;
		if (pd != NULL)
		{
			pd->dataLength = ((PWAVEHDR) lParam)->dwBytesRecorded / sizeof(short);
			pd->data = (short*)malloc(((PWAVEHDR) lParam)->dwBytesRecorded);
			if (pd->data != NULL)
			{
				memcpy(pd->data, ((PWAVEHDR) lParam)->lpData, ((PWAVEHDR) lParam)->dwBytesRecorded);
				EnterCriticalSection(&csVoice);
				CheckQueue.push(pd);
				LeaveCriticalSection(&csVoice);
			}
			else
			{
				delete pd;
			}
		}
	}

	if (m_bEnding)
	{
		waveInClose(m_hWaveIn);
		return;
	}
	
	// Send out a new buffer	
	waveInAddBuffer (m_hWaveIn, (PWAVEHDR)lParam, sizeof(WAVEHDR));
}

void CTelephoneDlg::OnMM_WIM_CLOSE(UINT wParam, LONG lParam) 
{
	// TODO: Add your message handler code here and/or call default
	TRACE(L"OnMM_WIM_CLOSE\n");
	for (int i = 0; i < WAVE_BUFFER_COUNT; ++i)
	{
		waveInUnprepareHeader (m_hWaveIn, m_pWaveHdr[i], sizeof(WAVEHDR));
	}

	m_bRecording = FALSE;
	bVoiceStop = true;
	m_pOggCodec->StopEncode(true);    //lxz20101220
	m_btnRecordTip.ShowWindow(FALSE);
	m_sticRecordCaption.ShowWindow(FALSE);
	m_sticRecord.ShowWindow(FALSE);
	m_sticRecord.SetWindowText(_T(""));
	m_btnRecord.SetWindowText(m_strRecord);
}

bool CTelephoneDlg::PlayStart(void)
{
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
 	waveOutMessage(m_hWaveOut,WAV_SPEAKER_LINEOUT,0,0);
	return true;
}

bool CTelephoneDlg::PlayStop(void)
{
	waveOutReset(m_hWaveOut);
	waveOutClose(m_hWaveOut);
	return true;
}
void CTelephoneDlg::OnMM_WOM_OPEN(UINT wParam,LONG lParam)
{
}

void CTelephoneDlg::OnMM_WOM_DONE(UINT wParam,LONG lParam)
{
}

void CTelephoneDlg::OnMM_WOM_CLOSE(UINT wParam,LONG lParam)
{
}
LRESULT CTelephoneDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
	case WM_LOCALRECORD:
		OnButtonTelephoneRecord();				
		break;
	case WM_RECORD_FULL:
		if(m_spSoundSegment)
		{
			KillTimer(m_uiRecordTimer);
			RecStop();
			m_spSoundSegment->Remove();
			m_spSoundSegment = boost::shared_ptr<Data::SoundSegment>();
		}
		break;
	}
	return CDialog::WindowProc(message,wParam,lParam);
}
LRESULT CTelephoneDlg::OnChangeLanguage(WPARAM wParam,LPARAM lParam)
{
   ChangeLanguage();
   return 0;
}

void SetModeStr(CString RecordStr_,CString HangStr_)
{

}

void CTelephoneDlg::ChangeLanguage()
{
	std::string strTemp;
	CString str; 
	CString s;

	m_strRecord = Util::StringOp::ToCString((Data::LanguageResource::Get(Data::RI_TEL_RECORDEBTN)));
	m_strStopRecord = Util::StringOp::ToCString((Data::LanguageResource::Get(Data::RI_TEL_STOPRECORDEBTN)));
	m_strHangOn = Util::StringOp::ToCString((Data::LanguageResource::Get(Data::RI_TEL_ACCEPTBTN)));
	m_strHangOff = Util::StringOp::ToCString((Data::LanguageResource::Get(Data::RI_TEL_HUNGOFFBTN)));
	

	strTemp = Data::LanguageResource::Get(Data::RI_TEL_RECORDING);
	str = strTemp.c_str();
	m_sticRecordCaption.SetWindowText(str);

	strTemp = Data::LanguageResource::Get(Data::RI_TEL_DURATION);
	str = strTemp.c_str();
	m_sticDurationCaption.SetWindowText(str);

	strTemp = Data::LanguageResource::Get(Data::RI_TEL_HIDEBTN);
	str = strTemp.c_str();
	m_btnHide.SetWindowText(str);

	strTemp = Data::LanguageResource::Get(Data::RI_TEL_NOTEBTN);
	str = strTemp.c_str();
	m_btnNote.SetWindowText(str);
///================================================
   m_btnRecord.GetWindowText(s);
	//defect 154 
    //defect 172
  if (Record_strch==s||Record_streg==s)
	{
	  strTemp = Data::LanguageResource::Get(Data::RI_TEL_RECORDEBTN);
	}
	else
	{
	 strTemp = Data::LanguageResource::Get(Data::RI_TEL_STOPRECORDEBTN);
	}
 	/////=============================================
	str = strTemp.c_str();

	m_btnRecord.SetWindowText(str);
	
	m_btnHandle.GetWindowText(s);

	if (hangon_strch==s||hangon_streg==s)
	{
		strTemp = Data::LanguageResource::Get(Data::RI_TEL_ACCEPTBTN);
	}
	else
	{   
		strTemp = Data::LanguageResource::Get(Data::RI_TEL_HUNGOFFBTN);
	}
	str = strTemp.c_str();

	m_btnHandle.SetWindowText(str);

	SetTitle(Data::RI_TEL_NEWTELSTC);
}

std::string CTelephoneDlg::StartSearch(FILE* file, const std::string& mobile, int low, int high)
{
	std::string result("");
	fseek(file, 0, SEEK_SET);
	BOOL isFind = FALSE;
	char buff[64];
	memset(buff, 0, 64);
	while(fgets(buff, 64, file) != NULL)
	{
		result = buff;
		memset(buff, 0, 64);
		if(result.substr(28, mobile.size()) == mobile)
		{
			isFind = TRUE;
			break;
		}
	}
	if(!isFind)
	{
		result = "";
	}
	return result;
}

std::string CTelephoneDlg::ParseCode(std::string tel)
{  
    FILE * file=fopen("\\hive\\res_dat\\telephone.txt","rb");
	std::string result;
	
	if (file)
	{
		if(tel[1] > '2')
		{
			result=tel.substr(0,4);
		}
		else
		{   
			result=tel.substr(0,3);
		}
		result=StartSearch(file,result,0,337);
		fclose(file);
	}
	else
	{
		result="";
	}
	
	return result;
}

std::string CTelephoneDlg::StartMobileSearch(FILE* file, const std::string& mobile, int low, int high)
{
	if(low>high)		
	{	
		return "";	
	}	
	else		
	{	
		char buff[64];
		std::string result;
		int mid=(low+high)/2;
		fseek(file, 0, SEEK_SET);
		fseek(file, mid * SIMADDRLENGTH, SEEK_SET);
		fread(buff, 1, SIMADDRLENGTH, file);
		std::string resultTemp = std::string(buff, SIMADDRLENGTH - 2);

		int index=resultTemp.find(' ');
		result=resultTemp.substr(0, index);

		if(result == mobile)		
		{
			CString space = Util::StringOp::ToCString(resultTemp);
			space.TrimRight();  //去掉最右端的空格
			result = Util::StringOp::FromCString(space);
			return result;	
		}	
		else if(result > mobile)
		{	
			return StartMobileSearch(file,mobile,low,mid-1);			
		}		
		else			
		{		
			return StartMobileSearch(file,mobile,mid+1,high);
		}
	}	
}
std::string CTelephoneDlg::ParseMobileCode(std::string mobile)
{
	FILE * file=fopen("\\hive\\res_dat\\mobile.txt","rb");
	std::string result;

	if (file)
	{
		 std::string temp=mobile.substr(0,7);
		 result = StartMobileSearch(file, temp, 0, 185313);
	     fclose(file);
	}
	else
	{
		result="";
	}

	return result;
}

bool CTelephoneDlg::isMobileNumber(std::string dailNumber)
{   
	bool flag=true;
	
	CString tempNumber=Util::StringOp::ToCString(dailNumber);
	
	tempNumber.MakeReverse();//字符串反转
	
	int count=tempNumber.GetLength();
	
	if (count>10)
	{
		tempNumber=tempNumber.Mid(0,11);
		
		if((isdigit(tempNumber[10])&&tempNumber[10]=='1'))
		{
			flag=true;
			
		}else
		{
			flag=false;
		}
		
	}else
	{
		flag=false;
	}
	
	return flag;
}

std::string CTelephoneDlg::ParseDialNumber(std::string DialNumber)
{
    bool flag=isMobileNumber(DialNumber);
	
	std::string result="";
	
	if (flag)
	{
		result=ParseMobileCode(DialNumber);
	}
	else
	{
		result=ParseCode(DialNumber);
	}

	return result;
}

std::string CTelephoneDlg::ParseTelePhoneCode(std::string tel)
{  
	FILE * file=fopen("\\hive\\res_dat\\telephone.txt","rb");
    
	std::string result="";
	
	if (file)
	{
		if (tel.size()>=11)
		{
			if (tel.size()>11)
			{
				result=tel.substr(0,4);
				
			}else
			{
				result=tel.substr(0,3);
			}

			result=StartSearch(file,result,0,336);

			fclose(file);

		}else if (tel.size()>=7)
		{
			result="本地";
		}
	}
	return result;
}

void CTelephoneDlg::setRecordNumber(CString number)
{
	m_bRecordNumber+=number;
	m_IsRecordNumber=true;
}
bool CTelephoneDlg::getCheckTakingState()
{
	return m_IsTaking;
}
void CTelephoneDlg::setCheckTakingstate(bool flag)
{
	m_IsTaking=flag;
}

void CTelephoneDlg::SetTitle(DWORD titleID)
{
	std::string title = Data::LanguageResource::Get(titleID).c_str();

	TextStruct ts[1];	
	memset(ts, 0, sizeof(TextStruct) * 1);	
	ts[0].txtRect = CRect(8, 0, 100, 20 * VHEIGHT);	
	ts[0].txtFontSize = 16;
	ts[0].sAlign = DT_LEFT | DT_BOTTOM;

	memcpy(ts[0].sTxt, title.c_str(), title.length());

//  change by qi 20111103
	m_sticBackground.SetTextStruct(ts, 1);
}

void CTelephoneDlg::PopupWindow(DWORD titleID, CString btnHandleTxt)
{
	KeyBoardOff();
		
	SetTitle(titleID);
	
	m_btnHandle.SetWindowText(btnHandleTxt);
	m_btnHandle.ShowWindow(TRUE);
	
	///zzw10824////////////////////////////////
	SetTimer(IDC_TELDLGSHOW, 5, NULL);//弹出窗口
}

void CTelephoneDlg::SaveRecAndContactInfo()
{
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	
	m_spContactInfo->duration(m_uiTelephoneSecond);
	if(0 == m_uiTelephoneSecond && m_spContactInfo->type() != Data::citInNo)
	{
		if(!m_bHasLeaveWord)
			m_spContactInfo->isSound(false);
	}
	if(g_iRecordCount >= 30000)  //录音记录满时通话无录音
		m_spContactInfo->isSound(false);
	if (m_spContactInfo->type() == Data::citInNo)
	{
		m_spContactInfo->played(false);
	}
	main->m_pContactInfoDlg->SaveContactInfo(m_spContactInfo);
	
	if (!m_vSoundSegment.empty())
	{
		for (std::vector<boost::shared_ptr<Data::SoundSegment> >::iterator iter = \
			m_vSoundSegment.begin(); iter != m_vSoundSegment.end(); ++iter)
		{
			if(0 == m_uiTelephoneSecond && \
				m_spContactInfo->type() != Data::citInNo)//当通话时长为0时，要同时删除录音文件
			{
				if(!m_bHasLeaveWord)
				{
					(*iter)->duration(m_uiTelephoneSecond);
					m_sDeleteFile.push_back((*iter)->filename());
				}
			}

			if(m_uiTelephoneSecond > 0 && (*iter)->duration() == 0)
			{
				(*iter)->duration(1);
			}
			
			(*iter)->telephoneNumber(m_spContactInfo->telephoneNumber());
			(*iter)->name(m_spContactInfo->name());
			(*iter)->contactInfoId(m_spContactInfo->id());
			(*iter)->contactId(m_spContactInfo->contactId());
			
			if((*iter)->duration() > 0 && m_spContactInfo->type() == Data::citOut \
				&& main->m_pSettingDlg->m_pSetting->isMustRecord())
			{
				int pos = m_sRecordFileName.rfind("\\");
				int pos1 = m_sRecordFileName.find("_", pos+1);
				int pos2 = m_sRecordFileName.find("_", pos1+1);
				std::string filename = m_sRecordFileName.substr(0, pos2+1);
				pos = m_sRecordFileName.rfind("_");
				std::string subname = m_sRecordFileName.substr(pos);
				if(m_spContactInfo->telephoneNumber().number() != "")
				{
					//add by qi 20111021
					//遇到*变成a ;
					std::string number = m_spContactInfo->telephoneNumber().number();
					replaceXing(number);				
					filename += number;
				
					// change by qi 20111021
					//filename += m_spContactInfo->telephoneNumber().number();
				}
				else
				{
					filename += "0";
				}

				filename += subname;
				if(filename != m_sRecordFileName)
				{
					CString name(m_sRecordFileName.c_str());
					if(DetectFile((LPTSTR)(LPCTSTR)name))
					{
						CFile::Rename((LPTSTR)(LPCTSTR)name, \
							(LPTSTR)(LPCTSTR)CString(filename.c_str()));
					}
				}

				m_sRecordFileName = filename;
				g_sOutDialRecordName = filename;
				(*iter)->filename(filename);
			
			}
			
			if (m_spContactInfo->type() == Data::citInNo)
			{
				(*iter)->type(Data::sstLeaveWord);
				(*iter)->played(false);
				main->m_pMainDlg->m_nRecordCount++;
			}
			else
			{
				(*iter)->type(Data::sstTeleRecord);
				(*iter)->played(true);
			}
			main->m_pSoundDlg->SaveSoundSegment(*iter); 
			
		}
		
		if(!m_sDeleteFile.empty())
			SetTimer(14, 4000, NULL);
	}
	m_vSoundSegment.clear();

	if (m_spContactInfo->type() == Data::citInNo)
	{
		main->m_pMainDlg->m_nUnconnectCount++;
		::PostMessage(main->m_pMainDlg->GetSafeHwnd(), WM_TELNOTIFY, 0, 0);
	}
	m_spContactInfo = boost::shared_ptr<Data::ContactInfo>();
	m_uiTelephoneSecond = 0;
}
