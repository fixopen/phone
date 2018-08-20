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

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

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

#define  INP_BUFFER_SIZE 23040 * 4
#define  BITSPERSAMPLE   16
#define  SAMPLESPERSEC   44100
#define  CHANNELS        1

#define  SECONDBYTES     10 * 1024
#define  MINFREEBYTES    100 * 1024
#define  IGNORERINGCOUNT 2

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

struct PcmData 
{
	short * data;
	long dataLength;
};

void* m_sabtd_struct;
std::queue<PcmData*> checkqueue;
HANDLE hCheckThread;

config_t config;
std::queue<PcmData*> mp3queue;
HANDLE hMp3Thread;
volatile bool mp3Stop = true;

DWORD WINAPI EncodeMp3ThreadPrc(LPVOID lpParameter)
{
	while (true)
	{
		if (!mp3queue.empty())
		{
			PcmData* m = mp3queue.front();
			config.wave.pcm = m->data;
			config.wave.pcm_total_length = m->dataLength;
			config.wave.pcm_current_position = 0;
			L3_compress(&config);
			free(m->data);
			delete m;
			mp3queue.pop();
			continue;
		}
		else if (mp3Stop)
		{
			break;
		}
		Sleep(10);
	}
	fflush(config.mpeg.file);
	fclose(config.mpeg.file);
	CloseHandle(hMp3Thread);
	hMp3Thread = NULL;
	printf("\nEncode over!!!!!!\n");
	return 0;
}

int  write_mp3(long bytes, void *buffer, void *config_in)
{
    config_t *config=(config_t *)config_in;
    return fwrite(buffer, sizeof(unsigned char), bytes, config->mpeg.file);  
}

DWORD WINAPI VoiceCheckThreadPrc(LPVOID lpParameter)
{
	while (true)
	{
		if (!checkqueue.empty())
		{
			PcmData* m = checkqueue.front();
			int toneDetected = 0;
			if (SabtdProcess_(m_sabtd_struct, &toneDetected, m->data, m->dataLength) < 0)
			{
				printf("SABTD_Process Process Error!\n");
			}
			
			if ((toneDetected == SABTD_TONE_DETECTED) || (toneDetected == SABTD_SILENCE_DETECTED))   // 检测到忙音
			{
				printf("SABTD_Process Process OK! %d\n", toneDetected);
				SabtdReset_(m_sabtd_struct);
				((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->HungOff();	
			}

// 			if (SABTD_Process(m_sabtd_struct, &toneDetected, m->data, m->dataLength) < 0)
// 			{
// 				printf("SABTD_Process Process Error!\n");
// 			}
// 			
// 			if ((toneDetected == SABTD_TONE_DETECTED) || (toneDetected == SABTD_SILENCE_DETECTED))   // 检测到忙音
// 			{
// 				printf("SABTD_Process Process OK! %d\n", toneDetected);
// 				SABTD_Reset(m_sabtd_struct);
// 				((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->HungOff();	
// 			}
			free(m->data);
			delete m;
			checkqueue.pop();
			continue;
		}
		Sleep(10);
	}
	return 0;
}


/////////////////////////////////////////////////////////////////////////////
// CTelephoneDlg dialog


CTelephoneDlg::CTelephoneDlg(CWnd* pParent /*=NULL*/)
	: CCEDialog(CTelephoneDlg::IDD, pParent)
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

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTelephoneDlg message handlers

BOOL CTelephoneDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CenterWindow(GetDesktopWindow());	// center to the hpc screen
	std::string strTemp;
	CString str;
	
	m_sticNameNo.Create(str, WS_CHILD|WS_VISIBLE, CRect(62, 78, 360, 108), this, IDC_STATIC_TELEPHONE_NAMENO);
	m_sticNameNo.SetColor(RGB(0, 0, 0), Data::g_partFrameInRectBackRGB[Data::g_skinstyle]);
	m_sticNameNo.SetFontSize(30);   

	m_sticCompany.Create(str, WS_CHILD|WS_VISIBLE, CRect(62, 113, 230, 129), this, IDC_STATIC_TELEPHONE_COMPANY);
	m_sticCompany.SetColor(RGB(0, 0, 0), Data::g_partFrameInRectBackRGB[Data::g_skinstyle]);

	m_sticDepartment.Create(str, WS_CHILD|WS_VISIBLE, CRect(62, 133, 230, 149), this, IDC_STATIC_TELEPHONE_DEPARTMENT);
	m_sticDepartment.SetColor(RGB(0, 0, 0), Data::g_partFrameInRectBackRGB[Data::g_skinstyle]);

	m_sticDuty.Create(str, WS_CHILD|WS_VISIBLE, CRect(62, 153, 230, 169), this, IDC_STATIC_TELEPHONE_DUTY);
	m_sticDuty.SetColor(RGB(0, 0, 0), Data::g_partFrameInRectBackRGB[Data::g_skinstyle]);


	strTemp = Data::LanguageResource::Get(Data::RI_TEL_RECORDING);
	str = strTemp.c_str();
	m_sticRecordCaption.Create(str, WS_CHILD, CRect(250, 128, 300, 144), this);
	m_sticRecordCaption.SetColor(RGB(0, 0, 0), Data::g_partFrameInRectBackRGB[Data::g_skinstyle]);

	m_sticRecord.Create(_T(""), WS_CHILD, CRect(302, 128, 360, 144), this);
	m_sticRecord.SetColor(RGB(0, 0, 0), Data::g_partFrameInRectBackRGB[Data::g_skinstyle]);

	strTemp = Data::LanguageResource::Get(Data::RI_TEL_DURATION);
	str = strTemp.c_str();
	m_sticDurationCaption.Create(str, WS_CHILD, CRect(250, 153, 300, 168), this);
	m_sticDurationCaption.SetColor(RGB(0, 0, 0), Data::g_partFrameInRectBackRGB[Data::g_skinstyle]);

	m_sticDuration.Create(_T(""), WS_CHILD, CRect(302, 153, 360, 168), this);
	m_sticDuration.SetColor(RGB(0, 0, 0), Data::g_partFrameInRectBackRGB[Data::g_skinstyle]);


	strTemp = Data::LanguageResource::Get(Data::RI_TEL_HIDEBTN);
	str = strTemp.c_str();
	m_btnHide.Create(str, Data::g_buttonArcBMPPARTDILOAGID[0][Data::g_skinstyle], Data::g_buttonArcBMPPARTDILOAGID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(378, 78, 434, 98), this, IDC_BUTTON_TELEPHONE_HIDE);
	m_btnHide.SetBackRGB(Data::g_partFrameMainBackRGB[Data::g_skinstyle]);

	strTemp = Data::LanguageResource::Get(Data::RI_TEL_NOTEBTN);
	str = strTemp.c_str();
	m_btnNote.Create(str, Data::g_buttonArcBMPPARTDILOAGID[0][Data::g_skinstyle], Data::g_buttonArcBMPPARTDILOAGID[1][Data::g_skinstyle], WS_CHILD, CRect(378, 103, 434, 123), this, IDC_BUTTON_TELEPHONE_NOTE);
	m_btnNote.SetBackRGB(Data::g_partFrameMainBackRGB[Data::g_skinstyle]);

	strTemp = Data::LanguageResource::Get(Data::RI_TEL_RECORDEBTN);
	str = strTemp.c_str();
	m_btnRecord.Create(str, Data::g_buttonArcBMPPARTDILOAGID[0][Data::g_skinstyle], Data::g_buttonArcBMPPARTDILOAGID[1][Data::g_skinstyle], WS_CHILD, CRect(378, 128, 434, 148), this, IDC_BUTTON_TELEPHONE_RECORD);
	m_btnRecord.SetBackRGB(Data::g_partFrameMainBackRGB[Data::g_skinstyle]);

	strTemp = Data::LanguageResource::Get(Data::RI_TEL_ACCEPTBTN);
	str = strTemp.c_str();
	m_btnHandle.Create(str, Data::g_buttonArcBMPPARTDILOAGID[0][Data::g_skinstyle], Data::g_buttonArcBMPPARTDILOAGID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(378, 153, 434, 173), this, IDC_BUTTON_TELEPHONE_HANDLE);
	m_btnHandle.SetBackRGB(Data::g_partFrameMainBackRGB[Data::g_skinstyle]);

	m_btnRecordTip.Create(_T(""),IDB_BITMAP31, IDB_BITMAP31, WS_CHILD, CRect(230, 128, 246, 144), this, IDC_BUTTON_TELEPHONE_RECORDTIP);
	m_btnRecordTip.SetBackRGB(Data::g_partFrameMainBackRGB[Data::g_skinstyle]);
	
	m_sticBackground.Create(CRect(34, 40, 448, 184), this, 1);
	
	TextStruct ts[1];
	memset(ts, 0, sizeof(TextStruct) * 1);
	
	ts[0].txtRect = CRect(8, 0, 100, 20);
	ts[0].txtFontSize = 16;
	ts[0].sAlign = DT_LEFT | DT_BOTTOM;
	memcpy(ts[0].sTxt, Data::LanguageResource::Get(Data::RI_TEL_NEWTELSTC).c_str(), Data::LanguageResource::Get(Data::RI_TEL_NEWTELSTC).length());
	
	m_sticBackground.SetTextStruct(ts, 1);
	
	if (!InitData())
	{
		MessageBox(_T("Telephone Initialize error!\n"));
	}

// 	ULARGE_INTEGER freeBytes;
// 	ULARGE_INTEGER totalBytes;
// 	ULARGE_INTEGER totalFreeBytes;
// 	GetDiskFreeSpaceEx(_T("/Flashdrv"), &freeBytes, &totalBytes, &totalFreeBytes);
// 	printf("\nFlashdrv:%d-%d-%d\n", freeBytes.QuadPart, totalBytes, totalFreeBytes.QuadPart);
// 	GetDiskFreeSpaceEx(_T("/Flashdrv/my_record"), &freeBytes, &totalBytes, &totalFreeBytes);
// 	printf("\my_record:%d-%d-%d\n", freeBytes.QuadPart, totalBytes.QuadPart, totalFreeBytes.QuadPart);
// 	GetDiskFreeSpaceEx(_T("/StorageCard"), &freeBytes, &totalBytes, NULL);
// 	printf("\nStorageCard:%d-%d\n", freeBytes, totalBytes);
// 	GetDiskFreeSpaceEx(_T("/UsbDisk"), &freeBytes, &totalBytes, NULL);
// 	printf("\nUsbDisk:%d-%d\n", freeBytes, totalBytes);
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
	m_uiRecordTimer = 2;
	m_uiRecordSecond = 0;
	m_uiRingCount = 0;
	m_uiTipTimer = 3;
	m_uiInNoCount = 0;
	m_uiRecordCound = 0;
	m_bAutoRecord = FALSE;
	m_bISRecording = FALSE;
	m_iAdjustTimes = -1;
	m_uiIgnoreRing = 0;
	m_bHasCallID = FALSE;
	
	m_spContactInfo = boost::shared_ptr<Data::ContactInfo>();
	m_spSoundSegment = boost::shared_ptr<Data::SoundSegment>();
	
	m_bEnding = FALSE;
	m_bPlaying = FALSE;
	m_dwDataLength = 0;
	m_pWaveHdr1 = reinterpret_cast<PWAVEHDR>(malloc(sizeof(WAVEHDR)));
	m_pWaveHdr2 = reinterpret_cast<PWAVEHDR>(malloc(sizeof(WAVEHDR)));
	
	m_bCheckingTone = FALSE;
	m_bPlayingLeaveWord = FALSE;
	
	m_waveform.wFormatTag=WAVE_FORMAT_PCM;
	m_waveform.nChannels=CHANNELS;
	m_waveform.nSamplesPerSec=SAMPLESPERSEC;
	m_waveform.wBitsPerSample=BITSPERSAMPLE;
	m_waveform.nBlockAlign=m_waveform.nChannels * m_waveform.wBitsPerSample / 8;
	m_waveform.nAvgBytesPerSec=m_waveform.nBlockAlign * m_waveform.nSamplesPerSec;
	m_waveform.cbSize=0;	

	m_pBuffer1=(PBYTE)malloc(INP_BUFFER_SIZE);
	m_pBuffer2=(PBYTE)malloc(INP_BUFFER_SIZE);
	
	if (!m_pBuffer1 || !m_pBuffer2)
	{
		if (m_pBuffer1) free(m_pBuffer1);
		if (m_pBuffer2) free(m_pBuffer2);
		return false;
	}

	m_hSABTD = NULL;
	m_hSABTD = LoadLibrary(_T("\\FlashDrv\\RES_DAT\\SABTD_CEDLL.dll")); 
	if (m_hSABTD == NULL)
	{
		return false;
	}
	else
	{
// 		SabtdInit_ = (SabtdInit)GetProcAddress(m_hSABTD, MAKEINTRESOURCE(2));
// 		SabtdUnInit_ = (SabtdUnInit)GetProcAddress(m_hSABTD, MAKEINTRESOURCE(6));
// 		SabtdSetParam_ = (SabtdSetParam)GetProcAddress(m_hSABTD, MAKEINTRESOURCE(5));
// 		SabtdGetParam_ = (SabtdGetParam)GetProcAddress(m_hSABTD, MAKEINTRESOURCE(1));
// 		SabtdReset_ = (SabtdReset)GetProcAddress(m_hSABTD, MAKEINTRESOURCE(4));
// 		SabtdProcess_ = (SabtdProcess)GetProcAddress(m_hSABTD, MAKEINTRESOURCE(3));
		SabtdInit_ = (SabtdInit)GetProcAddress(m_hSABTD, _T("SABTD_Init"));
		SabtdUnInit_ = (SabtdUnInit)GetProcAddress(m_hSABTD, _T("SABTD_UnInit"));
		SabtdSetParam_ = (SabtdSetParam)GetProcAddress(m_hSABTD, _T("SABTD_SetParam"));
		SabtdGetParam_ = (SabtdGetParam)GetProcAddress(m_hSABTD, _T("SABTD_GetParam"));
		SabtdReset_ = (SabtdReset)GetProcAddress(m_hSABTD, _T("SABTD_Reset"));
		SabtdProcess_ = (SabtdProcess)GetProcAddress(m_hSABTD, _T("SABTD_Process"));
	}

	int initResult = -1;
	initResult = SabtdInit_(&m_sabtd_struct, SAMPLESPERSEC, "\\FlashDrv\\RES_DAT\\sa_license.dat");
	if (initResult==-1000)  // 授权文件出错
	{
		printf("Product license error! Please contract SoftAcoustic Inc.\n");
		return false;
	}
	else if (initResult<0 || m_sabtd_struct==NULL)    // 初始化出错
	{
		printf("Initialize error!\n");
		return false;		
	}

// 	int initResult = SABTD_Init(&m_sabtd_struct,SAMPLESPERSEC);
// 	if (initResult==-1000)  // 授权文件出错
// 	{
// 		printf("Product license error! Please contract SoftAcoustic Inc.\n");
// 		return false;
// 	}
// 	else if (initResult<0 || m_sabtd_struct==NULL)    // 初始化出错
// 	{
// 		printf("Initialize error!\n");
// 		return false;
// 	}
//  
	hCheckThread = NULL;
	hCheckThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)VoiceCheckThreadPrc, NULL, 0, NULL);
	if (hCheckThread == INVALID_HANDLE_VALUE)
	{
		printf("Create Check Thread error!\n");
		return false;
	}
	
	((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->SetSoundRingVal(-500);
	((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->OpenTelRing(FALSE);

	return true;
}

void CTelephoneDlg::OnButtonTelephoneHide()
{
	ShowWindow_(FALSE);
}

void CTelephoneDlg::OnButtonTelephoneNote()
{
	ShowWindow_(FALSE);
	((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->SwitchPanel_(IDC_BUTTON_INFORMATION);
}

void CTelephoneDlg::OnButtonTelephoneRecord()
{
	CString caption;
	m_btnRecord.GetWindowText(caption);
	if (caption == m_strRecord)
	{
 		if (mp3queue.empty() && mp3Stop)
 		{
			std::string path = ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pSetting->soundPath();

			BOOL DetectDIR(TCHAR *sDir);
			
			BOOL sd = DetectDIR((LPTSTR)(LPCTSTR)Util::StringOp::ToCString(path));
			if (!sd)
			{
				path = "/flashdrv/MY_RECORD/";
			}

			ULARGE_INTEGER freeBytes;
			ULARGE_INTEGER totalBytes;
			GetDiskFreeSpaceEx(Util::StringOp::ToCString(path), &freeBytes, &totalBytes, NULL);
			if (totalBytes.QuadPart < MINFREEBYTES)
			{
				MessageBox(Util::StringOp::ToCString(Data::LanguageResource::Get(Data::RI_SYSTEM_DISK)));
				return;
			}
			
			m_uiRemainRecordSecond = totalBytes.QuadPart / SECONDBYTES + 1;

			m_spSoundSegment = boost::shared_ptr<Data::SoundSegment>(new Data::SoundSegment);
			m_vSoundSegment.push_back(m_spSoundSegment);
			CTime t;
			t = CTime::GetCurrentTime();
			CString filename;
			filename.Format(_T("%02d%02d%02d.wav"), t.GetHour(), t.GetMinute(), t.GetSecond());
			m_sRecordFileName = path + Util::StringOp::FromCString(filename);
			std::string mf = m_sRecordFileName.substr(0, m_sRecordFileName.length() - 4);
			mf += ".mp3";
			m_spSoundSegment->filename(mf);
			// 	m_spSoundSegment->filename(m_sRecordFileName);
			m_spSoundSegment->startTime(CTime::GetCurrentTime());
			if (m_bAutoRecord)
			{
				m_spSoundSegment->type(Data::sstLeaveWord);
			}
			else
			{
				m_spSoundSegment->type(Data::sstTeleRecord);
			}
			m_spContactInfo->isSound(true);

			//time(&config.start_time);
			
			L3_set_config_mpeg_defaults(&config.mpeg);
			
			config.mpeg.mode = MODE_MONO; //reset this value if needed
			config.mpeg.bitr = 64;
			config.infile  = (char*)m_sRecordFileName.c_str();
			config.outfile = (char*)mf.c_str();	
			config.wave.type          = WAVE_RIFF_PCM;
			config.wave.channels      = CHANNELS;
			config.wave.samplerate    = SAMPLESPERSEC;
			config.wave.bits          = BITSPERSAMPLE;
			
			//Open the input file and fill the config wave_t header
			//wave_open(&config);
			
			//Set the MP3 sample rate index plus see if it's valid
			config.mpeg.samplerate_index = L3_find_samplerate_index(config.wave.samplerate);
			if(config.mpeg.samplerate_index < 0) 
				return;
			
			// Set the MP3 bit rate index plus see if it's valid
			config.mpeg.bitrate_index  = L3_find_bitrate_index(config.mpeg.bitr);      
			if(config.mpeg.bitrate_index < 0) 
				return;
			
			// open the output file
			if ((config.mpeg.file = fopen(config.outfile, "wb")) == NULL) {
				return;
			}
			
			// set up the read PCM stream and write MP3 stream functions
			config.get_pcm=&wave_get;
			config.write_mp3=&write_mp3;
			
			while (!mp3queue.empty())
			{
				PcmData* m = mp3queue.front();
				free(m->data);
				delete m;
				mp3queue.pop();
			}
			
			mp3Stop = false;
			hMp3Thread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)EncodeMp3ThreadPrc, NULL, 0, NULL);
			if (hMp3Thread == INVALID_HANDLE_VALUE)
			{
				mp3Stop = true;
				return ;
			}

			m_iAdjustTimes = 2;
			m_bISRecording = TRUE;
			SetTimer(m_uiRecordTimer, 1000, 0);

			//if (RecStart())
			{
				m_btnRecordTip.ShowWindow(TRUE);
				m_sticRecordCaption.ShowWindow(TRUE);
				m_sticRecord.ShowWindow(TRUE);
				m_btnRecord.SetWindowText(m_strStopRecord);			
			}			
		}
	}
	else
	{
		//if (RecStop())
		{
			KillTimer(m_uiRecordTimer);
			m_iAdjustTimes = 2;
			m_bISRecording = FALSE;

			m_btnRecordTip.ShowWindow(FALSE);
			m_sticRecordCaption.ShowWindow(FALSE);
			m_sticRecord.ShowWindow(FALSE);
			m_sticRecord.SetWindowText(_T(""));
			m_btnRecord.SetWindowText(m_strRecord);

			m_spSoundSegment->duration(Util::TimeSpan(m_uiRecordSecond));
			m_spSoundSegment = boost::shared_ptr<Data::SoundSegment>();
			m_uiRecordSecond = 0;
		}
	}
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
	}	
}

void CTelephoneDlg::HangOff_(void* param)
{
 	((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->StopRing();

	KillTimer(m_uiTipTimer);
	KillTimer(m_uiTelephoneTimer);

	RecStop();

	CString caption;
	m_btnRecord.GetWindowText(caption);
	if (caption == m_strStopRecord)
	{
		OnButtonTelephoneRecord();
	}

	if (m_spContactInfo)// != boost::shared_ptr<Data::ContactInfo>()
	{
		m_spContactInfo->duration(Util::TimeSpan(m_uiTelephoneSecond));
		
		if (m_spContactInfo->type() == Data::citInNo)
		{
			++m_uiInNoCount;
			if (!m_vSoundSegment.empty())
			{
				++m_uiRecordCound;
			}
			::SendMessage(((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pMainDlg->GetSafeHwnd(), WM_TELNOTIFY, 0, 0);
		}

		((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pContactInfoDlg->SaveContactInfo(m_spContactInfo);
	}



	if (!m_vSoundSegment.empty())
	{
		for (std::vector<boost::shared_ptr<Data::SoundSegment> >::iterator iter = m_vSoundSegment.begin(); iter != m_vSoundSegment.end(); ++iter)
		{
			(*iter)->telephoneNumber(m_spContactInfo->telephoneNumber());
			(*iter)->contactInfoId(m_spContactInfo->id());
			(*iter)->contactId(m_spContactInfo->contactId());

			((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSoundDlg->SaveSoundSegment(*iter);
		}
	}
	m_vSoundSegment.clear();

	m_spContactInfo = boost::shared_ptr<Data::ContactInfo>();
	m_uiTelephoneSecond = 0;
	m_spSoundSegment = boost::shared_ptr<Data::SoundSegment>();
	m_uiRecordSecond = 0;
	m_uiRingCount = 0;
	m_bAutoRecord = FALSE;
	m_uiRingCount = 0;

	m_uiIgnoreRing = 0;
	m_bHasCallID = FALSE;
	m_bCheckingTone = FALSE;
	m_bPlayingLeaveWord = FALSE;
	m_iAdjustTimes = -1;

	ShowWindow_(FALSE);
	m_sticDurationCaption.ShowWindow(FALSE);
	m_sticDuration.SetWindowText(_T(""));
	m_sticDuration.ShowWindow(FALSE);
	m_btnNote.ShowWindow(FALSE);
 	m_btnRecord.ShowWindow(FALSE);
	m_btnHandle.SetWindowText(m_strHangOn);
	m_btnHandle.ShowWindow(FALSE);
	m_sticNameNo.SetWindowText(_T(""));
	m_sticCompany.SetWindowText(_T(""));
	m_sticDepartment.SetWindowText(_T(""));
	m_sticDuty.SetWindowText(_T(""));

	((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->OpenTelRing(FALSE);
}

void CTelephoneDlg::HangOn_(void* param)
{
	TextStruct ts[1];
	memset(ts, 0, sizeof(TextStruct) * 1);
	
	ts[0].txtRect = CRect(8, 0, 100, 20);
	ts[0].txtFontSize = 16;
	ts[0].sAlign = DT_LEFT | DT_BOTTOM;
	memcpy(ts[0].sTxt, Data::LanguageResource::Get(Data::RI_TEL_DIALTELSTC).c_str(), Data::LanguageResource::Get(Data::RI_TEL_DIALTELSTC).length());
	
	m_sticBackground.SetTextStruct(ts, 1);

	m_btnHandle.SetWindowText(m_strHangOff);
	m_btnHandle.ShowWindow(TRUE);
	ShowWindow_(TRUE);
}

void CTelephoneDlg::Key_(void* param)
{
	CString number;
	m_sticNameNo.GetWindowText(number);
	number += char(param);
	if (number.GetLength() > 20)
	{
		m_sticNameNo.SetWindowText(number.Right(20));
	}
	else
	{
		m_sticNameNo.SetWindowText(number);
	}
	
	if (!m_spContactInfo)
	{
		m_spContactInfo = boost::shared_ptr<Data::ContactInfo>(new Data::ContactInfo);
		m_spContactInfo->type(Data::citOut);
		m_spContactInfo->startTime(CTime::GetCurrentTime());
	}
	m_spContactInfo->telephoneNumber(Util::StringOp::FromCString(number));
	m_sTelephoneNumber = Util::StringOp::FromCString(number);
	if (!IsWindowVisible())
	{
		ShowWindow_(true);
	}
}

void CTelephoneDlg::SubKey_(void* param)
{
	CString number;
	m_sticNameNo.GetWindowText(number);
	number += char(param);

	if (number.GetLength() > 20)
	{
		m_sticNameNo.SetWindowText(number.Right(number.GetLength() - 20));
	}
	else
	{
		m_sticNameNo.SetWindowText(number);
	}
	
	if (!IsWindowVisible())
	{
		ShowWindow_(true);
	}
}

void CTelephoneDlg::Connect_(void* param)
{
	if (!m_bCheckingTone)
	{		
		RecStart();
//		OnButtonTelephoneRecord();
	}

	if (m_bPlayingLeaveWord)
	{
		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pFSM->setStartState(CMultimediaPhoneDlg::tsRing);
		return;
	}

	((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->StopRing();
	
	TextStruct ts[1];
	memset(ts, 0, sizeof(TextStruct) * 1);
	
	ts[0].txtRect = CRect(8, 0, 100, 20);
	ts[0].txtFontSize = 16;
	ts[0].sAlign = DT_LEFT | DT_BOTTOM;
	memcpy(ts[0].sTxt, Data::LanguageResource::Get(Data::RI_TEL_TELINGSTC).c_str(), Data::LanguageResource::Get(Data::RI_TEL_TELINGSTC).length());
	
	m_sticBackground.SetTextStruct(ts, 1);

	m_sticDurationCaption.ShowWindow(TRUE);
	m_sticDuration.ShowWindow(TRUE);
	m_btnNote.ShowWindow(TRUE);
	m_btnRecord.ShowWindow(TRUE);
	m_btnHandle.SetWindowText(m_strHangOff);
	m_btnHandle.ShowWindow(TRUE);
	m_sticDuration.ShowWindow(TRUE);
	m_sticDuration.SetWindowText(_T(""));
	if (!IsWindowVisible())
	{
		ShowWindow_(true);
	}

	if (m_spContactInfo->type() == Data::citOut)
	{
		CString number;
		m_sticNameNo.GetWindowText(number);
		m_spContactInfo->telephoneNumber(Util::StringOp::FromCString(number));
		m_spContactInfo->startTime(CTime::GetCurrentTime());
		std::vector<boost::shared_ptr<Data::Contact> > result = FindCallContact(Util::StringOp::FromCString(number));
		if (!result.empty())
		{
			ShowContact(result[0]);
		}
	}
	else if (!m_bAutoRecord)
	{
		m_spContactInfo->type(Data::citInOk);	
	}

	SetTimer(m_uiTelephoneTimer, 1000, 0);
}

void CTelephoneDlg::Ring_(void* param)
{
	if (!m_spContactInfo)
	{
		m_spContactInfo = boost::shared_ptr<Data::ContactInfo>(new Data::ContactInfo);
		m_spContactInfo->startTime(CTime::GetCurrentTime());
		m_spContactInfo->type(Data::citInNo);
	}	

	++m_uiIgnoreRing;
	if ((m_uiIgnoreRing == (IGNORERINGCOUNT + 1)) && !m_bHasCallID)
	{
		if (!IsWindowVisible())
		{
			TextStruct ts[1];
			memset(ts, 0, sizeof(TextStruct) * 1);
			
			ts[0].txtRect = CRect(8, 0, 100, 20);
			ts[0].txtFontSize = 16;
			ts[0].sAlign = DT_LEFT | DT_BOTTOM;
			memcpy(ts[0].sTxt, Data::LanguageResource::Get(Data::RI_TEL_NEWTELSTC).c_str(), Data::LanguageResource::Get(Data::RI_TEL_NEWTELSTC).length());
			
			m_sticBackground.SetTextStruct(ts, 1);

			m_btnHandle.SetWindowText(m_strHangOn);
			m_btnHandle.ShowWindow(TRUE);
			ShowWindow_(TRUE);
			m_bAutoRecord = FALSE;
		}


		if (((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pSetting->isUseSpecRing())
		{
			std::string ring = ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pSetting->defaultRingFilename();
			((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->StartRing((LPTSTR)(LPCTSTR)Util::StringOp::ToCString(ring));
		}
		else
		{
			if (((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pSetting->isFirewall())
			{
				int firetype = ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pSetting->firewallType();
				if (firetype != 4)
				{
					((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->OpenTelRing();
				}
			}
			else
			{
				((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->OpenTelRing();
			}
		}
	}

	if (((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pSetting->isAutoReply())
	{
		++m_uiRingCount;
		if (m_uiRingCount - IGNORERINGCOUNT == ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pSetting->autoReplyRingCount())
		{
			::KillTimer((theApp.m_pMainWnd)->m_hWnd, ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_uiRingTimer);
			m_bAutoRecord = TRUE;
			std::string tip = ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pSetting->defaultRingFilename();
//			((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->StartRing((LPTSTR)(LPCTSTR)Util::StringOp::ToCString(tip));

			HangOnToRecord();
			((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->StartRing(_T("\\FlashDrv\\hj.mp3"), 1);
			SetTimer(m_uiTipTimer, 10 * 1000, 0);
		}		
	}
}

void CTelephoneDlg::CallID_(void* param)
{
	m_bHasCallID = TRUE;
	if (!IsWindowVisible())
	{
		TextStruct ts[1];
		memset(ts, 0, sizeof(TextStruct) * 1);
		
		ts[0].txtRect = CRect(8, 0, 100, 20);
		ts[0].txtFontSize = 16;
		ts[0].sAlign = DT_LEFT | DT_BOTTOM;
		memcpy(ts[0].sTxt, Data::LanguageResource::Get(Data::RI_TEL_NEWTELSTC).c_str(), Data::LanguageResource::Get(Data::RI_TEL_NEWTELSTC).length());
		
		m_sticBackground.SetTextStruct(ts, 1);

		m_btnHandle.SetWindowText(m_strHangOn);
		m_btnHandle.ShowWindow(TRUE);
		ShowWindow_(TRUE);
	}
	
	if (!m_spContactInfo)
	{
		m_spContactInfo = boost::shared_ptr<Data::ContactInfo>(new Data::ContactInfo);
		m_spContactInfo->startTime(CTime::GetCurrentTime());
		m_spContactInfo->type(Data::citInNo);
	}

	CALLID_INFO* info = (CALLID_INFO*)param;
	std::string tel = info->number;

	if (((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pSetting->thisTelephoneNumber().number().length() >= 2)
	{
		std::string code = ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pSetting->thisTelephoneNumber().number();
		if (code[0] != '0')
		{
			code = "0" + code;
		}

		int codeLen = code.length();
		if (tel.substr(0, codeLen) == code)
		{
			tel = tel.substr(codeLen);
		}
	}

	m_spContactInfo->telephoneNumber(Data::TelephoneNumber(tel));

//	CString number = info->number;
// 	CString number = Util::StringOp::ToCString(tel);
// 	m_sticNameNo.SetWindowText(number);
	m_sticNameNo.SetWindowText(Util::StringOp::ToCString(tel));
// 	if (!number.IsEmpty())
	if (tel != "")
	{	
//		std::vector<boost::shared_ptr<Data::Contact> > result = FindCallContact(Util::StringOp::FromCString(number));
		std::vector<boost::shared_ptr<Data::Contact> > result = FindCallContact(tel);
		if (!result.empty())
		{
			ShowContact(result[0]);
			
			if (((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pSetting->isFirewall())
			{
				int firetype = ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pSetting->firewallType();
				Data::ContactType type = result[0]->type();

				if (firetype == 1 && type == Data::ctBlacklist)
					return;

				if (firetype == 2 && (type == Data::ctBlacklist))//!(type == Data::ctNormal || type == Data::ctVip)
					return;

				if (firetype == 3 && type != Data::ctVip)
					return;

				if (firetype == 4)
					return;
			}

			if (result[0]->group())// && result[0]->group()->ring()
			{
				std::string ringname = result[0]->group()->ringName();
				if (ringname == Data::LanguageResource::Get(Data::RI_CARD_DEFAULT))
				{
					ringname = ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pSetting->defaultRingFilename();
				}
				CFileStatus   status;
				if (CFile::GetStatus(LPCTSTR(Util::StringOp::ToCString(ringname)),status))
				{
					((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->StartRing((LPTSTR)(LPCTSTR)Util::StringOp::ToCString(ringname));
					return;
				}
			}
		}
		else
		{
			if (((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pSetting->isFirewall())
			{
				int firetype = ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pSetting->firewallType();
				if (firetype == 2 || firetype == 3 || firetype == 4)
					return;
			}
		}
	}

	if (((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pSetting->isFirewall())
	{
		int firetype = ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pSetting->firewallType();
		if (firetype == 4)
			return;
	}

	if (((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pSetting->isUseSpecRing())
	{
		std::string ringname = ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pSetting->defaultRingFilename();
		CFileStatus   status;
		if (CFile::GetStatus(LPCTSTR(Util::StringOp::ToCString(ringname)),status))
		{
			((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->StartRing((LPTSTR)(LPCTSTR)Util::StringOp::ToCString(ringname));
			return;
		}
	}

	((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->OpenTelRing();
}

void CTelephoneDlg::Mute(void)
{
	printf("Mute\n");
	((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->Mute();
}

void CTelephoneDlg::Redial(void)
{
	if (m_sTelephoneNumber != "")
	{
		DialContact(m_sTelephoneNumber);
	}
}

void CTelephoneDlg::HangOnInRecord(void)
{
	printf("Hang On IN Record\n");
	if (m_spSoundSegment)
	{
		m_spSoundSegment->type(Data::sstTeleRecord);
	}
	if (m_spContactInfo)
	{
		m_spContactInfo->type(Data::citInOk);
	}
	m_bAutoRecord = FALSE;
	m_bPlayingLeaveWord = FALSE;
	((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pFSM->fireEvent(CMultimediaPhoneDlg::teHangOn, NULL);
}

void CTelephoneDlg::HangOnToRecord(void)
{
	printf("Hang On To Record\n");
	m_bPlayingLeaveWord = TRUE;
	((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->HungOnToRecord();	

	if (waveInOpen(&m_hWaveIn,0,&m_waveform,(DWORD)this->m_hWnd,NULL,CALLBACK_WINDOW))
	{
		printf("waveInOpen error!\n");
		return ;
	}	
	waveInMessage(m_hWaveIn,WAV_SPEAKER_LINEOUT,0,0);

	m_pWaveHdr1->lpData=(char*)m_pBuffer1;
	m_pWaveHdr1->dwBufferLength=INP_BUFFER_SIZE;
	m_pWaveHdr1->dwBytesRecorded=0;
	m_pWaveHdr1->dwUser=0;
	m_pWaveHdr1->dwFlags=0;
	m_pWaveHdr1->dwLoops=1;
	m_pWaveHdr1->lpNext=NULL;
	m_pWaveHdr1->reserved=0;
	
	MMRESULT r = waveInPrepareHeader(m_hWaveIn,m_pWaveHdr1,sizeof(WAVEHDR));
	
	m_pWaveHdr2->lpData=(char*)m_pBuffer2;
	m_pWaveHdr2->dwBufferLength=INP_BUFFER_SIZE;
	m_pWaveHdr2->dwBytesRecorded=0;
	m_pWaveHdr2->dwUser=0;
	m_pWaveHdr2->dwFlags=0;
	m_pWaveHdr2->dwLoops=1;
	m_pWaveHdr2->lpNext=NULL;
	m_pWaveHdr2->reserved=0;
	
	r = waveInPrepareHeader(m_hWaveIn,m_pWaveHdr2,sizeof(WAVEHDR));
	waveInAddBuffer (m_hWaveIn, m_pWaveHdr1, sizeof (WAVEHDR)) ;
	waveInAddBuffer (m_hWaveIn, m_pWaveHdr2, sizeof (WAVEHDR)) ;
	waveInReset(m_hWaveIn);
	waveInClose(m_hWaveIn);
}

std::vector<boost::shared_ptr<Data::Contact> > CTelephoneDlg::FindCallContact(std::string number)
{
	std::string filter;
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
	
	std::vector<boost::shared_ptr<Data::Contact> > result = Data::Contact::GetFromDatabase(filter);
	return result;
}

void CTelephoneDlg::ShowContact(boost::shared_ptr<Data::Contact> contact)
{
  	m_spContactInfo->contactId(contact->id());
	m_spContactInfo->name(contact->name());
	m_sticNameNo.SetWindowText(CString(contact->name().c_str()));
	m_sticCompany.SetWindowText(CString(contact->company().c_str()));
	m_sticDepartment.SetWindowText(CString(contact->department().c_str()));
	m_sticDuty.SetWindowText(CString(contact->duty().c_str()));
}

void CTelephoneDlg::DialContact(Data::TelephoneNumber telephoneNumber, int contactId)
{
	if ((((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pFSM->getCurrentState() != CMultimediaPhoneDlg::tsHangOff)
		&& (((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pFSM->getCurrentState() != CMultimediaPhoneDlg::tsHangOn))
		return;

	 m_sTelephoneNumber = telephoneNumber.number();
	std::string number = m_sTelephoneNumber;
	if (((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pFSM->getCurrentState() == CMultimediaPhoneDlg::tsHangOff)
	{
		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->phone_->HungOn();
		Sleep(1000);
	}

	if (((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pSetting->isAutoAppendOutlinePrefix())
	{
		std::string out = ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pSetting->outlinePrefix();
		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->phone_->DialNumber((char*)out.c_str());
		Sleep(2000);
	}

	if (((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pSetting->isAppendIpPrefix())
	{
		if (number[0] == '0')
		{
			number = ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pSetting->ipPrefix() + number;
		}
	}

	((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->phone_->DialNumber((char*)number.c_str());
	m_sticNameNo.SetWindowText(CString(number.c_str()));
	if (!IsWindowVisible())
	{
		TextStruct ts[1];
		memset(ts, 0, sizeof(TextStruct) * 1);
		
		ts[0].txtRect = CRect(8, 0, 100, 20);
		ts[0].txtFontSize = 16;
		ts[0].sAlign = DT_LEFT | DT_BOTTOM;
		memcpy(ts[0].sTxt, Data::LanguageResource::Get(Data::RI_TEL_NEWTELSTC).c_str(), Data::LanguageResource::Get(Data::RI_TEL_NEWTELSTC).length());
		
		m_sticBackground.SetTextStruct(ts, 1);

		m_btnHandle.SetWindowText(m_strHangOn);
		m_btnHandle.ShowWindow(TRUE);
		ShowWindow_(TRUE);
		((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->EnableLine(TRUE);
	}
	
	if (!m_spContactInfo)
	{
		m_spContactInfo = boost::shared_ptr<Data::ContactInfo>(new Data::ContactInfo);
		m_spContactInfo->type(Data::citOut);
		m_spContactInfo->startTime(CTime::GetCurrentTime());
		m_spContactInfo->telephoneNumber(number);
	}

	((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pFSM->setStartState(CMultimediaPhoneDlg::tsKey);
	((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pFSM->fireEvent(CMultimediaPhoneDlg::teConnect, NULL);
}

UINT CTelephoneDlg::GetUnconnectCount(void)
{
	return m_uiInNoCount;
}

void CTelephoneDlg::ClearUnconnectCount(void)
{
	m_uiInNoCount = 0;
	::SendMessage(((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pMainDlg->GetSafeHwnd(), WM_TELNOTIFY, 0, 0);
}

UINT CTelephoneDlg::GetRecordCount(void)
{
	return m_uiRecordCound;
}

void CTelephoneDlg::ClearRecordCount(void)
{
	m_uiRecordCound = 0;
	::SendMessage(((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pMainDlg->GetSafeHwnd(), WM_TELNOTIFY, 0, 0);
}

void CTelephoneDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent == m_uiTelephoneTimer)
	{
		++m_uiTelephoneSecond;
		CTimeSpan ts(m_uiTelephoneSecond);
		CString duration;
		if (m_uiTelephoneSecond > 3600)
		{
			duration.Format(_T("%d:%02d:%02d"), ts.GetHours(), ts.GetMinutes(),ts.GetSeconds());
		}
		else
		{
			duration.Format(_T("%d:%02d"), ts.GetMinutes(),ts.GetSeconds());
		}
		m_sticDuration.SetWindowText(duration);
	}
	else if (nIDEvent == m_uiRecordTimer)
	{
		++m_uiRecordSecond;
		CTimeSpan ts(m_uiRecordSecond);
		CString duration;
		if (m_uiRecordSecond > 3600)
		{
			duration.Format(_T("%d:%02d:%02d"), ts.GetHours(), ts.GetMinutes(),ts.GetSeconds());
		}
		else
		{
			duration.Format(_T("%d:%02d"), ts.GetMinutes(),ts.GetSeconds());
		}
		m_sticRecord.SetWindowText(duration);

		if (m_uiRecordSecond >= m_uiRemainRecordSecond)
		{
			OnButtonTelephoneRecord();
		}
	}
	else if (nIDEvent == m_uiTipTimer)
	{
		KillTimer(m_uiTipTimer);
		((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->StopRing();
		OnButtonTelephoneRecord();
	}
	
	CCEDialog::OnTimer(nIDEvent);
}

bool CTelephoneDlg::RecStart(void)
{
	// TODO: Add your control notification handler code here
	
// 	SaveWavHead();
// 	
// 	if (m_fWav.Open(LPCTSTR(Util::StringOp::ToCString(m_sRecordFileName)), CFile::modeWrite) == 0)
// 		return false;
// 	
// 	m_fWav.SeekToEnd();
// 	printf("SaveWavHead\n");
	
	/////////////////////////////////////////////////////////////////////////////////////////////////////

	if (waveInOpen(&m_hWaveIn,0,&m_waveform,(DWORD)this->m_hWnd,NULL,CALLBACK_WINDOW))
	{
//		m_fWav.Close();
		return false;
	}

  	waveInMessage(m_hWaveIn,WAV_LINEIN_MIC,0,0);

	m_pWaveHdr1->lpData=(char*)m_pBuffer1;
	m_pWaveHdr1->dwBufferLength=INP_BUFFER_SIZE;
	m_pWaveHdr1->dwBytesRecorded=0;
	m_pWaveHdr1->dwUser=0;
	m_pWaveHdr1->dwFlags=0;
	m_pWaveHdr1->dwLoops=1;
	m_pWaveHdr1->lpNext=NULL;
	m_pWaveHdr1->reserved=0;
	
	MMRESULT r = waveInPrepareHeader(m_hWaveIn,m_pWaveHdr1,sizeof(WAVEHDR));
	
	m_pWaveHdr2->lpData=(char*)m_pBuffer2;
	m_pWaveHdr2->dwBufferLength=INP_BUFFER_SIZE;
	m_pWaveHdr2->dwBytesRecorded=0;
	m_pWaveHdr2->dwUser=0;
	m_pWaveHdr2->dwFlags=0;
	m_pWaveHdr2->dwLoops=1;
	m_pWaveHdr2->lpNext=NULL;
	m_pWaveHdr2->reserved=0;
	
	r = waveInPrepareHeader(m_hWaveIn,m_pWaveHdr2,sizeof(WAVEHDR));
		
	waveInAddBuffer (m_hWaveIn, m_pWaveHdr1, sizeof (WAVEHDR)) ;
	waveInAddBuffer (m_hWaveIn, m_pWaveHdr2, sizeof (WAVEHDR)) ;
	
	// Begin sampling

	m_bEnding = FALSE ;
	m_dwDataLength = 0 ;
	m_bCheckingTone = TRUE;
	waveInStart (m_hWaveIn) ;
	printf("RecStart\n");
	return true;
}

bool CTelephoneDlg::RecStop(void)
{
	m_bEnding=TRUE;		
	waveInReset(m_hWaveIn);
	waveInClose(m_hWaveIn);

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
	printf("OnMM_WIM_OPEN\n");
}

void CTelephoneDlg::OnMM_WIM_DATA(UINT wParam, LONG lParam) 
{
	printf("OnMM_WIM_DATA\n");

//  m_fWav.Write(((PWAVEHDR) lParam)->lpData, ((PWAVEHDR) lParam)->dwBytesRecorded);
 	m_dwDataLength += ((PWAVEHDR) lParam)->dwBytesRecorded;
	
	if (m_bISRecording)
	{
		--m_iAdjustTimes;
		if (m_iAdjustTimes <= 0)
		{
			PcmData * pd = new PcmData;
			pd->dataLength = ((PWAVEHDR) lParam)->dwBytesRecorded / sizeof(short);
			pd->data = (short*)malloc(((PWAVEHDR) lParam)->dwBytesRecorded);
			memcpy(pd->data , ((PWAVEHDR) lParam)->lpData, ((PWAVEHDR) lParam)->dwBytesRecorded);
			mp3queue.push(pd);
		}
	}
	else if (m_iAdjustTimes >= 0)
	{
		--m_iAdjustTimes;
		if (m_iAdjustTimes >= 0)
		{
			PcmData * pd = new PcmData;
			pd->dataLength = ((PWAVEHDR) lParam)->dwBytesRecorded / sizeof(short);
			pd->data = (short*)malloc(((PWAVEHDR) lParam)->dwBytesRecorded);
			memcpy(pd->data , ((PWAVEHDR) lParam)->lpData, ((PWAVEHDR) lParam)->dwBytesRecorded);
			mp3queue.push(pd);
		}
		else
		{
			mp3Stop = true;
		}
	}

	PcmData * pd = new PcmData;
	pd->dataLength = ((PWAVEHDR) lParam)->dwBytesRecorded / sizeof(short);
	pd->data = (short*)malloc(((PWAVEHDR) lParam)->dwBytesRecorded);
	memcpy(pd->data , ((PWAVEHDR) lParam)->lpData, ((PWAVEHDR) lParam)->dwBytesRecorded);
	checkqueue.push(pd);

	if (m_bEnding)
	{
		waveInReset(m_hWaveIn);
		waveInClose(m_hWaveIn);
		return;
	}
	
	// Send out a new buffer	
	waveInAddBuffer (m_hWaveIn, (PWAVEHDR) lParam, sizeof (WAVEHDR));
}

void CTelephoneDlg::OnMM_WIM_CLOSE(UINT wParam, LONG lParam) 
{
	// TODO: Add your message handler code here and/or call default
	printf("OnMM_WIM_CLOSE\n");
	
	waveInUnprepareHeader (m_hWaveIn, m_pWaveHdr1, sizeof (WAVEHDR)) ;
	waveInUnprepareHeader (m_hWaveIn, m_pWaveHdr2, sizeof (WAVEHDR)) ;

// 	m_fWav.Close();
// 	SaveWavHead();

	mp3Stop = true;
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

 	waveOutMessage(m_hWaveOut,WAV_SPEAKER_LINEOUT,0,0);
	return true;
}

bool CTelephoneDlg::PlayStop(void)
{
	if (!m_bPlaying)
	{
		return true;
	}
	waveOutReset(m_hWaveOut);
	return true;
}

void CTelephoneDlg::OnMM_WOM_OPEN(UINT wParam,LONG lParam)
{
	printf("OnMM_WOM_OPEN\n");
// 	DWORD dwLength = 0;
// 	CFileStatus   status;
// 	if (CFile::GetStatus(LPCTSTR(Util::StringOp::ToCString(m_sPlayFileName)),status))
// 	{
// 		CFile file;
// 		file.Open(LPCTSTR(Util::StringOp::ToCString(m_sPlayFileName)), CFile::modeRead);
// 		int size = file.GetLength();
// 		m_pOutBuffer = (PBYTE)malloc(size + 1);
// 		if (m_pOutBuffer == NULL)
// 		{
// 			file.Close();
// 			waveOutReset(m_hWaveOut);
// 			return;
// 		}
// 		file.Read(m_pOutBuffer, size);
// 		file.Close();
// 	}
// 	
// 	
// 	m_pWaveHdr1->lpData          = (char*)m_pOutBuffer ;
// 	m_pWaveHdr1->dwBufferLength  = dwLength ;
// 	m_pWaveHdr1->dwBytesRecorded = 0 ;
// 	m_pWaveHdr1->dwUser          = 0 ;
// 	m_pWaveHdr1->dwFlags         = WHDR_BEGINLOOP | WHDR_ENDLOOP ;
// 	m_pWaveHdr1->dwLoops         = 1 ;
// 	m_pWaveHdr1->lpNext          = NULL ;
// 	m_pWaveHdr1->reserved        = 0 ;
// 	
// 	// Prepare and write
// 	
// 	waveOutPrepareHeader (m_hWaveOut, m_pWaveHdr1, sizeof (WAVEHDR)) ;
// 	waveOutWrite (m_hWaveOut, m_pWaveHdr1, sizeof (WAVEHDR)) ;
// 	
// 	m_bPlaying = TRUE ;
}

void CTelephoneDlg::OnMM_WOM_DONE(UINT wParam,LONG lParam)
{
	printf("OnMM_WOM_DONE\n");
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
	printf("OnMM_WOM_CLOSE\n");
// 	m_bPlaying = FALSE ;	
// 	if (m_pOutBuffer != NULL)
// 	{
// 		free(m_pOutBuffer);
// 		m_pOutBuffer = NULL;
// 	}
}
