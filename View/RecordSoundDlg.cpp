// RecordSoundDlg.cpp : implementation file
//

#include "stdafx.h"
#include "..\multimediaphone.h"
#include "RecordSoundDlg.h"
#include "../Data/LanguageResource.h"
#include "../Data/SkinStyle.h"
#include "../MultimediaPhoneDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define SECONDBYTES8           2048
#define SECONDBYTES2           850
extern DWORD PreFreeSize;
extern DWORD sdPreFreeSize;
extern int g_iRecordCount;

/////////////////////////////////////////////////////////////////////////////
// CRecordSoundDlg dialog
DWORD volumet[]={0x10001000,0x44004400,0x88008800,0xcc00cc00,0xff00ff00};
CRecordSoundDlg::CRecordSoundDlg(CWnd* pParent /*=NULL*/)
	: CCEDialog(CRecordSoundDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRecordSoundDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CRecordSoundDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRecordSoundDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CRecordSoundDlg, CCEDialog)
	//{{AFX_MSG_MAP(CRecordSoundDlg)
	ON_MESSAGE(MM_WIM_OPEN,OnMM_WIM_OPEN)
	ON_MESSAGE(MM_WIM_DATA,OnMM_WIM_DATA)
	ON_MESSAGE(MM_WIM_CLOSE,OnMM_WIM_CLOSE)
    ON_MESSAGE(WM_CHANGELANGUAGE,OnChangeLanguage)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_RECORDDLG_CLOSE, OnButtonSoundClose)
	ON_BN_CLICKED(IDC_BUTTON_RECORDDLG_CLEAR, OnButtonSoundClear)
	ON_BN_CLICKED(IDC_BUTTON_RECORDDLG_PLAY, OnButtonSoundPlay)
	ON_BN_CLICKED(IDC_BUTTON_RECORDDLG_RECORD, OnButtonSoundRecord)
	ON_MESSAGE(WM_PROCESS_POS, OnPlaySeek)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRecordSoundDlg message handlers

BOOL CRecordSoundDlg::OnInitDialog() 
{
	CCEDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	std::string strTemp;
	CString str;

	m_sticSoundName.Create(str, WS_CHILD|WS_VISIBLE, CRect(62, 76*VHEIGHT, 340, 92*VHEIGHT), this);
	m_sticSoundName.SetColor(RGB(0, 0, 0), Data::g_partFrameInRectBackRGB[Data::g_skinstyle]);
	
	m_sticSoundDuration.Create(str, WS_CHILD|WS_VISIBLE, CRect(232, 98*VHEIGHT, 338, 114*VHEIGHT), this);
	m_sticSoundDuration.SetColor(RGB(0, 0, 0), Data::g_partFrameInRectBackRGB[Data::g_skinstyle]);
	m_sticSoundDuration.SetAlign(SS_RIGHT);	

	m_procbarSound.Create(WS_CHILD|WS_VISIBLE, CRect(62, 123*VHEIGHT, 338, 139*VHEIGHT), this, IDC_PBAR_RECORDDLG_PROCESS);
	m_procbarSound.SetParam(0, 0, 50, 1);
	m_procbarSound.SetPos(0);
	m_procbarSound.SetDisable(true);

	strTemp = Data::LanguageResource::Get(Data::RI_SOUND_PLAY_CLOSE);
	str = strTemp.c_str();
	m_btnClose.Create(str, Data::g_buttonArcBMPPARTDILOAGID[0][Data::g_skinstyle], Data::g_buttonArcBMPPARTDILOAGID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(378, 78*VHEIGHT, 434, 98*VHEIGHT), this, IDC_BUTTON_RECORDDLG_CLOSE);
	m_btnClose.SetBackRGB(Data::g_partFrameMainBackRGB[Data::g_skinstyle]);

	strTemp = Data::LanguageResource::Get(Data::RI_VEDIO_DELETEALLBTN);
	str = strTemp.c_str();
	m_btnClear.Create(str, Data::g_buttonArcBMPPARTDILOAGID[0][Data::g_skinstyle], Data::g_buttonArcBMPPARTDILOAGID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(378, 103*VHEIGHT, 434, 123*VHEIGHT), this, IDC_BUTTON_RECORDDLG_CLEAR);
	m_btnClear.SetBackRGB(Data::g_partFrameMainBackRGB[Data::g_skinstyle]);
	
	strTemp = Data::LanguageResource::Get(Data::RI_SOUND_PLAY_PLAY);
	str = strTemp.c_str();
	m_btnPlay.Create(str, Data::g_buttonArcBMPPARTDILOAGID[0][Data::g_skinstyle], Data::g_buttonArcBMPPARTDILOAGID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(378, 128*VHEIGHT, 434, 148*VHEIGHT-1), this, IDC_BUTTON_RECORDDLG_PLAY);
	m_btnPlay.SetBackRGB(Data::g_partFrameMainBackRGB[Data::g_skinstyle]);

	strTemp = Data::LanguageResource::Get(Data::RI_TEL_RECORDEBTN);
	str = strTemp.c_str();
	m_btnRecord.Create(str, Data::g_buttonArcBMPPARTDILOAGID[0][Data::g_skinstyle], Data::g_buttonArcBMPPARTDILOAGID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(378, 153*VHEIGHT, 434, 173*VHEIGHT), this, IDC_BUTTON_RECORDDLG_RECORD);
	m_btnRecord.SetBackRGB(Data::g_partFrameMainBackRGB[Data::g_skinstyle]);

	m_btnRecordTip.Create(_T(""),IDB_BITMAP31, IDB_BITMAP31, WS_CHILD, CRect(230, 153*VHEIGHT, 246, 168*VHEIGHT), this, IDC_BUTTON_TELEPHONE_RECORDTIP);
	m_btnRecordTip.SetBackRGB(Data::g_partFrameMainBackRGB[Data::g_skinstyle]);

	strTemp = Data::LanguageResource::Get(Data::RI_TEL_RECORDING);
	str = strTemp.c_str();
	m_sticRecordCaption.Create(str, WS_CHILD, CRect(250, 153*VHEIGHT, 300, 168*VHEIGHT), this);
	m_sticRecordCaption.SetColor(RGB(0, 0, 0), Data::g_partFrameInRectBackRGB[Data::g_skinstyle]);
	
	m_sticRecord.Create(_T(""), WS_CHILD, CRect(302, 153*VHEIGHT, 360, 168*VHEIGHT), this);
	m_sticRecord.SetColor(RGB(0, 0, 0), Data::g_partFrameInRectBackRGB[Data::g_skinstyle]);

	m_sticBackground.Create(CRect(34, 40*VHEIGHT, 448, 184*VHEIGHT), this, 1);
	
	TextStruct ts[1];
	memset(ts, 0, sizeof(TextStruct) * 1);
	
	ts[0].txtRect = CRect(8, 0, 100, 20*VHEIGHT);
	ts[0].txtFontSize = 16;
	ts[0].sAlign = DT_LEFT | DT_BOTTOM;
	memcpy(ts[0].sTxt, Data::LanguageResource::Get(Data::RI_SOUND_LOCALRECORD).c_str(), Data::LanguageResource::Get(Data::RI_SOUND_LOCALRECORD).length());
	
	m_sticBackground.SetTextStruct(ts, 1);
	
	m_bISRecording = FALSE;
	m_bPlaying = FALSE;
	m_bClear = FALSE;
	m_uiCurrentSecond = 0;
	m_uiTotalSecond = 0;
	m_btnClear.EnableWindow(FALSE);
	m_btnPlay.EnableWindow(FALSE);
	m_spSoundSegment = boost::shared_ptr<Data::SoundSegment>();
	m_pOggCodec = new OggCodec();
		
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
	
	m_nSoundindex = ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pSettingDlg->m_pSetting->soundVolume();
	waveOutSetVolume(NULL,volumet[m_nSoundindex]);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

std::string CRecordSoundDlg::GetSoundPath(void)
{
	std::string path = ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pSetting->soundPath();
	
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
			path = ssFlashRecordPath;
			if (!DetectDIR((LPTSTR)(LPCTSTR)csFlashRecordPath))
			{
				CreateDirectory((LPTSTR)(LPCTSTR)csFlashRecordPath, NULL);
			}
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
	}

	return path;
}
extern HANDLE g_hThreadExit;
bool CRecordSoundDlg::RecStart(void)
{
	// TODO: Add your control notification handler code here
	WaitForSingleObject(g_hThreadExit, 4000);

	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	if(g_iRecordCount >= 30000)
	{
		main->m_pDeleteTipDlg->SetPasswordModel(false);
		main->m_pDeleteTipDlg->SetTitle(Data::LanguageResource::Get(Data::RI_ERROR));
		main->m_pDeleteTipDlg->SetDelTip(Util::StringOp::ToCString(Data::LanguageResource::Get(Data::RI_SYSTEM_SOUND_TIP)));
		main->m_pDeleteTipDlg->ShowWindow_(SW_SHOW);
		return false;
	}
	
	//change by qi 20110707
	//std::string path = GetSoundPath();
	std::string path = main->m_pTelephoneDlg->GetSoundPath();
	//

	ULARGE_INTEGER freeBytes;
	ULARGE_INTEGER totalBytes;
	GetDiskFreeSpaceEx(Util::StringOp::ToCString(path), &freeBytes, &totalBytes, NULL);
	int secondBytes = SECONDBYTES8;
	std::string strTemp=Data::LanguageResource::Get(Data::RI_ERROR);
	if (main->m_pSettingDlg->m_pSetting->isDeleteProtect())
	{
		secondBytes = SECONDBYTES8;
	}
	else
	{
		secondBytes = SECONDBYTES2;
	}

//	change by qi 20110707	
//	if (path == ssFlashRecordPath)
	if (path.find(ssFlashRecordPath) != std::string::npos)
	{		
		if (freeBytes.QuadPart < PreFreeSize)
		{
			OnButtonSoundClose();
			main->m_pDeleteTipDlg->SetPasswordModel(false);
			main->m_pDeleteTipDlg->SetTitle(strTemp);
			main->m_pDeleteTipDlg->SetDelTip(\
				Util::StringOp::ToCString(Data::LanguageResource::Get(Data::RI_SYSTEM_DISK)));
			main->m_pDeleteTipDlg->ShowWindow_(SW_SHOW);
			return false;
		}
		
		m_uiRemainRecordSecond = (freeBytes.QuadPart - PreFreeSize) / secondBytes + 1;
	}
	else
	{
		if (freeBytes.QuadPart < sdPreFreeSize)
		{
			main->m_pDeleteTipDlg->SetPasswordModel(false);
			main->m_pDeleteTipDlg->SetTitle(strTemp);
			main->m_pDeleteTipDlg->SetDelTip(\
				Util::StringOp::ToCString(Data::LanguageResource::Get(Data::RI_SYSTEM_DISK)));
			main->m_pDeleteTipDlg->ShowWindow_(SW_SHOW);
			return false;
		}
		m_uiRemainRecordSecond = (freeBytes.QuadPart - sdPreFreeSize) / secondBytes + 1;
	}
		
	m_spSoundSegment = boost::shared_ptr<Data::SoundSegment>(new Data::SoundSegment);

	CTime t = CTime::GetCurrentTime();
	CString filename;

	if (main->m_pSettingDlg->m_pSetting->isDeleteProtect())
	{
		m_pOggCodec->SetQuality(8);

		if(main->m_pSettingDlg->m_pSetting->isPlayProtect())
		{
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

	std::string mf = path + Util::StringOp::FromCString(filename);
	m_spSoundSegment->filename(mf);
	m_spSoundSegment->type(Data::sstLocalRecord);
	m_spSoundSegment->startTime(CTime::GetCurrentTime());
	m_sticSoundName.SetWindowText(filename);
	m_sticSoundDuration.SetWindowText(_T("00:00"));	
	
	if (waveInOpen(&m_hWaveIn,0,&m_waveform,(DWORD)this->m_hWnd,NULL,CALLBACK_WINDOW))
	{
		return false;
	}
	waveInMessage(m_hWaveIn,0x30000002,0,0);
	
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
		
	m_bEnding = FALSE;
	waveInStart(m_hWaveIn);

	std::string comment = m_spSoundSegment->name() + "\'" + m_spSoundSegment->telephoneNumber().number() + "\'" + Util::StringOp::FromInt(m_spSoundSegment->type()) + "\'" + Util::StringOp::FromTimestamp(m_spSoundSegment->startTime()) + "\'";
	if(m_pOggCodec->StartEncode(m_spSoundSegment->filename(), comment))
	{
		m_spSoundSegment->Insert();
	}

	return true;
}

CString CRecordSoundDlg::GenerateRecordName(BOOL isHighQuality, BOOL isProtect)
{
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	CString filename(L"");
	CTime t = CTime::GetCurrentTime();

	if(isHighQuality)
	{
		filename.Format(_T("%04d%02d%02d%02d%02d%02d"), t.GetYear(), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute(), t.GetSecond());
		filename += L"_";
		if(main->m_pSettingDlg->m_pSetting->LocalNumber() != "")
		{
			filename += Util::StringOp::ToCString(main->m_pSettingDlg->m_pSetting->LocalNumber());
		}
		else
		{
			filename += L"0";
		}

 		filename += L"_";
 		filename += L"0";
		filename += L"_";

		if (gLangItem == 1)
			filename += L"现场录音";
		else
			filename += L"LocalRecord";

		if(isProtect)
			filename += L"HQ.spd";
		else
			filename += L"HQ.spx";
	}
	else
	{
		filename.Format(_T("%04d%02d%02d%02d%02d%02d"), t.GetYear(), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute(), t.GetSecond());
		filename += L"_";
		if(main->m_pSettingDlg->m_pSetting->LocalNumber() != "")
		{
			filename += Util::StringOp::ToCString(main->m_pSettingDlg->m_pSetting->LocalNumber());
		}
		else
		{
			filename += L"0";
		}

 		filename += L"_";
 		filename += L"0";
 		filename += L"_";

		if (gLangItem == 1)
			filename += L"现场录音";
		else
			filename += L"LocalRecord";

		if(isProtect)
			filename += L"ST.spd";
		else
			filename += L"ST.spx";
	}

	return filename;
}

bool CRecordSoundDlg::RecStop(void)
{
	m_bEnding=TRUE;	

 	waveInReset(m_hWaveIn);	
	return true;
}

bool CRecordSoundDlg::PlayStart(void)
{
	CFileStatus   status;

	if (CFile::GetStatus(LPCTSTR(Util::StringOp::ToCString(m_spSoundSegment->filename())),status))
	{
		return m_pOggCodec->StartDecode(m_spSoundSegment->filename());	
	}
	return false;
}

bool CRecordSoundDlg::PlayStop(void)
{
	m_pOggCodec->StopDecode(true);

	return true;
}

void CRecordSoundDlg::OnButtonSoundClose()
{
	if (m_bPlaying)
	{
		OnButtonSoundPlay();
	}

	if (m_bISRecording)
	{
		OnButtonSoundRecord();
	}

	m_btnClear.EnableWindow(FALSE);

	m_btnPlay.EnableWindow(FALSE);
	
	m_sticSoundName.SetWindowText(_T(""));

	m_sticRecord.SetWindowText(_T("00:00"));

	m_uiCurrentSecond = 0;

	m_uiTotalSecond = 0;

	ShowSeconds();

	m_procbarSound.SetPos(0);

	ShowWindow_(FALSE);
}

void CRecordSoundDlg::OnButtonSoundClear()
{
	try
	{
		CFileStatus   status;

		if (CFile::GetStatus(LPCTSTR(Util::StringOp::ToCString(m_spSoundSegment->filename())),status))
		{
			CFile::Remove(LPCTSTR(Util::StringOp::ToCString(m_spSoundSegment->filename())));
		}
		m_spSoundSegment->Remove();

		((CSoundDlg*)m_pParentWnd)->ShowItemsInList();
	}
	catch (CFileException* pEx)
	{
 		pEx->Delete();
 	}

	m_bClear = TRUE;
	m_sticSoundName.SetWindowText(_T(""));
	m_sticRecord.SetWindowText(_T("00:00"));
	m_uiCurrentSecond = 0;
	m_uiTotalSecond = 0;
	ShowSeconds();
	m_procbarSound.SetPos(0);
	m_btnPlay.EnableWindow(FALSE);
}
void CRecordSoundDlg::OnBtRecordplay()
{
 OnButtonSoundPlay();
}
void CRecordSoundDlg::OnBtRecord()
{
	OnButtonSoundRecord();
}
void CRecordSoundDlg::OnButtonSoundPlay()
{
	if (m_bPlaying)
	{
		KillTimer(2);
		PlayStop();
		std::string s = Data::LanguageResource::Get(Data::RI_SOUND_PLAY_PLAY);
		m_btnPlay.SetWindowText(CString(s.c_str()));
		m_bPlaying = FALSE;
		m_btnClear.EnableWindow(TRUE);
		m_btnRecord.EnableWindow(TRUE);
		m_uiCurrentSecond = 0;
		ShowSeconds();
		m_procbarSound.SetPos(0);
		m_procbarSound.SetDisable(true);
	}
	else
	{
		if (PlayStart())
		{
			std::string s = Data::LanguageResource::Get(Data::RI_SOUND_RECORD_STOPPLAY);

			m_btnPlay.SetWindowText(CString(s.c_str()));

			m_bPlaying = TRUE;

			m_btnClear.EnableWindow(FALSE);

			m_btnRecord.EnableWindow(FALSE);

			m_uiCurrentSecond = 0;

			m_procbarSound.SetParam(0, 0, m_uiTotalSecond, 1);

			m_procbarSound.SetDisable(false);

			SetTimer(2, 1000, NULL);
		}
	}
}

void CRecordSoundDlg::OnButtonSoundRecord()
{
	if (m_bISRecording)
	{
		KillTimer(1);

		RecStop();

		if (m_uiCurrentSecond == 0)
		{
			m_uiCurrentSecond = 1;
		}
		m_uiTotalSecond = m_uiCurrentSecond;
		m_uiCurrentSecond = 0;
		m_sticSoundDuration.ShowWindow(TRUE);
		ShowSeconds();
		m_spSoundSegment->duration(m_uiTotalSecond);

		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pSoundDlg->SetTypeListSelected(2);
        
        ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pSoundDlg->SaveSoundSegment(m_spSoundSegment);	
	}
	else
	{
		if (RecStart())
		{   
			std::string s = Data::LanguageResource::Get(Data::RI_TEL_STOPRECORDEBTN);
			m_btnRecord.SetWindowText(CString(s.c_str()));
			m_bISRecording = TRUE;
			m_btnClear.EnableWindow(FALSE);
			m_btnPlay.EnableWindow(FALSE);
			m_bClear = FALSE;
			m_uiCurrentSecond = 0;
			m_uiTotalSecond = 0;
			//ShowSeconds();  //wzx20101222
			m_btnRecordTip.ShowWindow(TRUE);
			m_sticRecordCaption.ShowWindow(TRUE);
			m_sticSoundDuration.ShowWindow(FALSE);
			m_sticRecord.ShowWindow(TRUE);

			SetTimer(1, 1000, NULL);
		}
	}
}

void CRecordSoundDlg::OnMM_WIM_OPEN(UINT wParam, LONG lParam) 
{
}

void CRecordSoundDlg::OnMM_WIM_DATA(UINT wParam, LONG lParam) 
{
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
	if (m_bEnding)
	{
		waveInClose(m_hWaveIn) ;
		return;
	}
	
	waveInAddBuffer (m_hWaveIn, (PWAVEHDR) lParam, sizeof (WAVEHDR));
}

void CRecordSoundDlg::OnMM_WIM_CLOSE(UINT wParam, LONG lParam) 
{
	// TODO: Add your message handler code here and/or call default
	for (int i = 0; i < WAVE_BUFFER_COUNT; ++i)
	{
		waveInUnprepareHeader (m_hWaveIn, m_pWaveHdr[i], sizeof (WAVEHDR));
	}
	
	Sleep(200);
	m_pOggCodec->StopEncode(true);
	//m_pOggCodec->ReSetOggCodec();  //线程同步时有问题
	m_bRecordOver = TRUE;
	std::string s = Data::LanguageResource::Get(Data::RI_TEL_RECORDEBTN);
	m_btnRecord.SetWindowText(CString(s.c_str()));
	m_bISRecording = FALSE;
	m_btnClear.EnableWindow(TRUE);
	m_btnPlay.EnableWindow(TRUE);
	m_btnRecordTip.ShowWindow(FALSE);
	m_sticRecordCaption.ShowWindow(FALSE);
	m_sticRecord.ShowWindow(FALSE);
	m_sticRecord.SetWindowText(_T("00:00"));
}

void CRecordSoundDlg::OnPlaySeek(WPARAM w, LPARAM l)
{
	if ((int)l == IDC_PBAR_RECORDDLG_PROCESS)
	{
		if (m_bPlaying)
		{
			CString filename;

			m_sticSoundName.GetWindowText(filename);

			std::string fn = Util::StringOp::FromCString(filename);

			if (fn.find("HQ") != std::string::npos)
			{
				w -= (w % 2);
			}
			else
			{
				w -= (w % 5);
			}
			if (m_pOggCodec->DecodeSeek(w))
			{
				m_uiCurrentSecond = w;
			}
			else
			{
				OnButtonSoundPlay();
			}
		}
	}
}

void CRecordSoundDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent == 1)
	{
		++m_uiCurrentSecond;
		CString duration;
		if (m_uiCurrentSecond > 3600)
		{
			duration.Format(_T("%02d:%02d:%02d"), m_uiCurrentSecond / 3600, m_uiCurrentSecond % 3600 / 60, m_uiCurrentSecond % 60);
		}
		else
		{
			duration.Format(_T("%02d:%02d"), m_uiCurrentSecond / 60, m_uiCurrentSecond % 60);
		}
		m_sticRecord.SetWindowText(duration);
		if ((m_uiCurrentSecond % 60) == 0)
		{
			m_spSoundSegment->duration(m_uiCurrentSecond);
			m_spSoundSegment->Update();
		}

		if (m_uiCurrentSecond >= m_uiRemainRecordSecond)
		{
			OnButtonSoundRecord();
		}
	}
	else if (nIDEvent == 2)
	{
		++m_uiCurrentSecond;
		if (m_uiCurrentSecond > m_uiTotalSecond)
		{
			KillTimer(2);
			OnButtonSoundPlay();
		}
		else
		{
			ShowSeconds();
			m_procbarSound.SetPos(m_uiCurrentSecond);
		}
	}
	CCEDialog::OnTimer(nIDEvent);
}

void CRecordSoundDlg::ShowSeconds(void)
{
	if (m_uiCurrentSecond > m_uiTotalSecond)
	{
		m_uiCurrentSecond = m_uiTotalSecond;
	}
	CString strTime;
	if (m_uiTotalSecond < 3600)
	{
		strTime.Format(_T("%02d:%02d/%02d:%02d"), m_uiCurrentSecond / 60, m_uiCurrentSecond % 60, m_uiTotalSecond / 60, m_uiTotalSecond % 60);
	}
	else
	{
		strTime.Format(_T("%02d:%02d:%02d/%02d:%02d:%02d"), m_uiCurrentSecond / 3600, m_uiCurrentSecond % 3600 / 60, m_uiCurrentSecond % 60, m_uiTotalSecond / 3600, m_uiTotalSecond % 3600 / 60, m_uiTotalSecond % 60);
	}

	m_sticSoundDuration.SetWindowText(strTime);
}


void CRecordSoundDlg::CloseSound(void)
{
	OnButtonSoundClose();
}

void CRecordSoundDlg::StartRecord(void)
{
	OnButtonSoundRecord();
}
LRESULT CRecordSoundDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_PLAYLEAVEWORD:
        if (((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pSoundDlg->m_pRecordSoundDlg->IsWindowVisible())
		{
			if(m_bISRecording)
			{
				return 0;
 			}
			else
			{
			  CString str;
              m_sticSoundName.GetWindowText(str);
			  if (str.IsEmpty())
			  {
				  return 0;
			  }
			  else
			  {
              OnButtonSoundPlay();
			  }
			}	
 		}
        break;
	case WM_CANCEL:
		if (((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pSoundDlg->m_pRecordSoundDlg->IsWindowVisible())
		{
			OnButtonSoundClose();
		}
		break;
	case WM_LOCALRECORD:
		OnButtonSoundRecord();
 		break;
	case WM_RECORD_FULL:
		if(m_spSoundSegment)
		{
			KillTimer(1);
			RecStop();
			m_spSoundSegment->Remove();
			m_spSoundSegment = boost::shared_ptr<Data::SoundSegment>();
			m_uiCurrentSecond = 0;
			((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pSoundDlg->SetTypeListSelected(2);
		}
		break;
	case WM_BTLEFT:
		if(m_bPlaying)
		{
			m_nSoundindex--;
			if(m_nSoundindex  < 0)
				m_nSoundindex = 0;
		}
		waveOutSetVolume(NULL, volumet[m_nSoundindex]);
		break;
	case WM_BTRIGHT:
		if(m_bPlaying)
		{
			m_nSoundindex++;
			if(m_nSoundindex > 4)
				m_nSoundindex = 4;
		}
		waveOutSetVolume(NULL, volumet[m_nSoundindex]);
		break;
	}
		return CDialog::WindowProc(message, wParam, lParam);
}
LRESULT CRecordSoundDlg::OnChangeLanguage(WPARAM wParam,LPARAM lParam)
{
ChangeLanguage();
return 0;
}
void CRecordSoundDlg::ChangeLanguage()
{
	std::string strTemp;
	CString str; 
    strTemp = Data::LanguageResource::Get(Data::RI_SOUND_PLAY_CLOSE);
	str = strTemp.c_str();
	m_btnClose.SetWindowText(str);

	strTemp = Data::LanguageResource::Get(Data::RI_VEDIO_DELETEALLBTN);
	str = strTemp.c_str();
	m_btnClear.SetWindowText(str);

	strTemp = Data::LanguageResource::Get(Data::RI_SOUND_PLAY_PLAY);
	str = strTemp.c_str();
	m_btnPlay.SetWindowText(str);

	strTemp = Data::LanguageResource::Get(Data::RI_TEL_RECORDEBTN);
	str = strTemp.c_str();
	m_btnRecord.SetWindowText(str);

	strTemp = Data::LanguageResource::Get(Data::RI_TEL_RECORDING);
	str = strTemp.c_str();
	m_sticRecordCaption.SetWindowText(str);

    TextStruct ts[1];
	memset(ts, 0, sizeof(TextStruct) * 1);
	
	ts[0].txtRect = CRect(8, 0, 100, 20*VHEIGHT);
	ts[0].txtFontSize = 16;
	ts[0].sAlign = DT_LEFT | DT_BOTTOM;
	memcpy(ts[0].sTxt, Data::LanguageResource::Get(Data::RI_SOUND_LOCALRECORD).c_str(), Data::LanguageResource::Get(Data::RI_SOUND_LOCALRECORD).length());
	
	m_sticBackground.SetTextStruct(ts, 1);
}
