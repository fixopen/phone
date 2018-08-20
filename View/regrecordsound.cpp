// regrecordsound.cpp : implementation file
//

#include "stdafx.h"
#include "regrecordsound.h"
#include "../Data/LanguageResource.h"
#include "../Data/SkinStyle.h"
#include "../MultimediaPhoneDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRegRecordSound dialog


#define SECONDBYTES8           2048
#define SECONDBYTES2           850
#define IDC_SLIDER             10042
#define IDC_EDIT_SOUND_MIN     10043
#define IDC_EDIT_SOUND_MAX     10044
#define NM_RELEASEDCAPTURE      (NM_FIRST-16)
extern DWORD PreFreeSize;
extern DWORD sdPreFreeSize;

/////////////////////////////////////////////////////////////////////////////
// CRecordSoundDlg dialog

long HexToDem(unsigned long HexNum)
{
	long DemNum=0;
	char string[8];
	sprintf(string,"%x",HexNum);
	DemNum=atol(string);
	return DemNum;
}

DWORD volumeA[]={0x10001000,0x44004400,0x88008800,0xcc00cc00,0xff00ff00};
CRegRecordSound::CRegRecordSound(CWnd* pParent /*=NULL*/)
	: CCEDialog(CRegRecordSound::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRecordSoundDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}
void CRegRecordSound::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRecordSoundDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRegRecordSound, CCEDialog)
	//{{AFX_MSG_MAP(CRecordSoundDlg)
	ON_MESSAGE(MM_WIM_OPEN,OnMM_WIM_OPEN)
	ON_MESSAGE(MM_WIM_DATA,OnMM_WIM_DATA)
	ON_MESSAGE(MM_WIM_CLOSE,OnMM_WIM_CLOSE)
    ON_MESSAGE(WM_CHANGELANGUAGE,OnChangeLanguage)
	ON_MESSAGE(WM_HSCROLL,OnHScroll)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_RECORDDLG_CLOSE, OnButtonSoundClose)
	ON_BN_CLICKED(IDC_BUTTON_RECORDDLG_CLEAR, OnButtonSoundClear)
	ON_BN_CLICKED(IDC_BUTTON_RECORDDLG_PLAY, OnButtonSoundPlay)
	ON_BN_CLICKED(IDC_BUTTON_RECORDDLG_RECORD, OnButtonSoundRecord)
    ON_EN_KILLFOCUS(IDC_EDIT_SOUND_MAX,SetSliderMaxVaule)
	ON_EN_KILLFOCUS(IDC_EDIT_SOUND_MIN,SetSliderMinVaule)
	ON_MESSAGE(WM_PROCESS_POS, OnPlaySeek)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRecordSoundDlg message handlers

BOOL CRegRecordSound::OnInitDialog() 
{   

	CCEDialog::OnInitDialog();
	// TODO: Add extra initialization here
	std::string strTemp;
	CString str;
    
	m_sticRegSound.Create(CString("调节录音质量"), WS_CHILD|WS_VISIBLE, CRect(62, 80*VHEIGHT, 150, 96*VHEIGHT), this);
    m_sticRegSound.SetColor(RGB(0, 0, 0), Data::g_partFrameInRectBackRGB[Data::g_skinstyle]);
    
	m_sticMin.Create(CString("最小值:"), WS_CHILD|WS_VISIBLE, CRect(150, 80*VHEIGHT, 200, 96*VHEIGHT), this);
	m_sticMin.SetColor(RGB(0, 0, 0), Data::g_partFrameInRectBackRGB[Data::g_skinstyle]);
    
	m_editMin.Create(WS_CHILD|WS_VISIBLE, CRect(200, 80*VHEIGHT, 250, 96*VHEIGHT), this, IDC_EDIT_SOUND_MIN);
	m_editMin.SetWindowText(L"");
	m_editMin.SetLimitText(15);
   
	m_sticMax.Create(CString("最大值:"), WS_CHILD|WS_VISIBLE, CRect(250, 80*VHEIGHT, 300, 96*VHEIGHT), this);
	m_sticMax.SetColor(RGB(0, 0, 0), Data::g_partFrameInRectBackRGB[Data::g_skinstyle]);
   
	m_editMax.Create(WS_CHILD|WS_VISIBLE, CRect(300, 80*VHEIGHT, 350, 96*VHEIGHT), this, IDC_EDIT_SOUND_MAX);
	m_editMax.SetWindowText(L"");
	m_editMax.SetLimitText(15);
    
	m_slider.Create(TBS_HORZ|TBS_ENABLESELRANGE,CRect(62, 96*VHEIGHT, 360, 120*VHEIGHT),this,IDC_SLIDER);
	m_slider.ShowWindow(TRUE);
	m_slider.SetPos(0);
	m_slider.SetRangeMax(0x4a,TRUE);
	m_slider.SetRangeMin(0x3a,TRUE);

	m_sticSoundName.Create(str, WS_CHILD|WS_VISIBLE, CRect(62, 125*VHEIGHT, 230, 141*VHEIGHT), this);
	m_sticSoundName.SetColor(RGB(0, 0, 0), Data::g_partFrameInRectBackRGB[Data::g_skinstyle]);
	
	m_sticSoundDuration.Create(str, WS_CHILD|WS_VISIBLE, CRect(232, 125*VHEIGHT, 338, 141*VHEIGHT), this);
	m_sticSoundDuration.SetColor(RGB(0, 0, 0), Data::g_partFrameInRectBackRGB[Data::g_skinstyle]);
	m_sticSoundDuration.SetAlign(SS_RIGHT);	

	m_procbarSound.Create(WS_CHILD|WS_VISIBLE, CRect(62, 141*VHEIGHT, 338, 151*VHEIGHT), this, IDC_PBAR_RECORDDLG_PROCESS);
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
     
	m_sticCurrent.Create(CString("当前值"), WS_CHILD|WS_VISIBLE, CRect(62, 153*VHEIGHT, 112, 168*VHEIGHT), this);
    m_sticCurrent.SetColor(RGB(0, 0, 0), Data::g_partFrameInRectBackRGB[Data::g_skinstyle]);
    
	m_sticCurrentValue.Create(_T("0"), WS_CHILD|WS_VISIBLE, CRect(112, 153*VHEIGHT, 150, 168*VHEIGHT), this);
    m_sticCurrentValue.SetColor(RGB(0, 0, 0), Data::g_partFrameInRectBackRGB[Data::g_skinstyle]);

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
	waveOutSetVolume(NULL,volumeA[m_nSoundindex]);
	m_IsRecord=false;
	return TRUE;  
}
std::string CRegRecordSound::GetSoundPath(void)
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
	return path;
}

bool CRegRecordSound::RecStart(void)
{
	// TODO: Add your control notification handler code here
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	std::string path = GetSoundPath();

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

	if (path == ssFlashRecordPath)
	{		
		if (freeBytes.QuadPart < PreFreeSize)
		{
			main->m_pDeleteTipDlg->SetPasswordModel(false);
			main->m_pDeleteTipDlg->SetTitle(strTemp);
			main->m_pDeleteTipDlg->SetDelTip(Util::StringOp::ToCString(Data::LanguageResource::Get(Data::RI_SYSTEM_DISK)));
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
			main->m_pDeleteTipDlg->SetDelTip(Util::StringOp::ToCString(Data::LanguageResource::Get(Data::RI_SYSTEM_DISK)));
			main->m_pDeleteTipDlg->ShowWindow_(SW_SHOW);
			return false;
		}

		m_uiRemainRecordSecond = (freeBytes.QuadPart - sdPreFreeSize) / secondBytes + 1;
	}
		
	m_spSoundSegment = boost::shared_ptr<Data::SoundSegment>(new Data::SoundSegment);

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
	m_spSoundSegment->Insert();
	m_sticSoundName.SetWindowText(filename);
	m_sticSoundDuration.SetWindowText(_T("00:00"));	
	
	if (waveInOpen(&m_hWaveIn,0,&m_waveform,(DWORD)this->m_hWnd,NULL,CALLBACK_WINDOW))
	{
		return false;
	}
	int currentvalue= m_slider.GetPos();
	DWORD dwHex = strtoul(	Util::StringOp::FromInt(currentvalue).c_str(), NULL, 10);
	waveInMessage(m_hWaveIn,0x40000001,dwHex,0);
 
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

	waveInStart (m_hWaveIn) ;

	std::string comment = m_spSoundSegment->name() + "\'" + m_spSoundSegment->telephoneNumber().number() + "\'" + Util::StringOp::FromInt(m_spSoundSegment->type()) + "\'" + Util::StringOp::FromTimestamp(m_spSoundSegment->startTime()) + "\'";

	m_pOggCodec->StartEncode(m_spSoundSegment->filename(), comment);

	m_IsRecord=true;
	return true;
}

CString CRegRecordSound::GenerateRecordName(BOOL isHighQuality, BOOL isProtect)
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
		filename += L"现场录音";
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
		filename += L"现场录音";
		if(isProtect)
			filename += L"ST.spd";
		else
			filename += L"ST.spx";
	}

	return filename;
}

bool CRegRecordSound::RecStop(void)
{
	m_bEnding=TRUE;	

 	waveInReset(m_hWaveIn);	

	return true;
}

bool CRegRecordSound::PlayStart(void)
{
	CFileStatus   status;

	if (CFile::GetStatus(LPCTSTR(Util::StringOp::ToCString(m_spSoundSegment->filename())),status))
	{
		return m_pOggCodec->StartDecode(m_spSoundSegment->filename());	
	}
	return false;
}

bool CRegRecordSound::PlayStop(void)
{
	m_pOggCodec->StopDecode(true);

	return true;
}

void CRegRecordSound::OnButtonSoundClose()
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

   ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pSettingDlg->m_chbEnabeDeleteProtect.SetCheck(BST_UNCHECKED);
    
   if(m_IsRecord)
   {
	    OnButtonSoundClear();

		m_IsRecord=false;
   }

  ShowWindow_(FALSE);
}

void CRegRecordSound::OnButtonSoundClear()
{
	try
	{
		CFileStatus   status;

		if (CFile::GetStatus(LPCTSTR(Util::StringOp::ToCString(m_spSoundSegment->filename())),status))
		{
			CFile::Remove(LPCTSTR(Util::StringOp::ToCString(m_spSoundSegment->filename())));
		}
		m_spSoundSegment->Remove();
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
void CRegRecordSound::OnBtRecordplay()
{
 OnButtonSoundPlay();
}
void CRegRecordSound::OnBtRecord()
{
	OnButtonSoundRecord();
}
void CRegRecordSound::OnButtonSoundPlay()
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

void CRegRecordSound::OnButtonSoundRecord()
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
			ShowSeconds();
			m_btnRecordTip.ShowWindow(TRUE);
			m_sticRecordCaption.ShowWindow(TRUE);
			m_sticRecord.ShowWindow(TRUE);
			SetTimer(1, 1000, NULL);
		}
	}
}

void CRegRecordSound::OnMM_WIM_OPEN(UINT wParam, LONG lParam) 
{
}

void CRegRecordSound::OnMM_WIM_DATA(UINT wParam, LONG lParam) 
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
//		waveInReset(m_hWaveIn);
		waveInClose(m_hWaveIn) ;
		return;
	}
	
	// Send out a new buffer	
	waveInAddBuffer (m_hWaveIn, (PWAVEHDR) lParam, sizeof (WAVEHDR));
}

void CRegRecordSound::OnMM_WIM_CLOSE(UINT wParam, LONG lParam) 
{
	// TODO: Add your message handler code here and/or call default
	for (int i = 0; i < WAVE_BUFFER_COUNT; ++i)
	{
		waveInUnprepareHeader (m_hWaveIn, m_pWaveHdr[i], sizeof (WAVEHDR));
	}

	Sleep(200);
	m_pOggCodec->StopEncode(true);
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

void CRegRecordSound::OnPlaySeek(WPARAM w, LPARAM l)
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

void CRegRecordSound::OnTimer(UINT nIDEvent) 
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

void CRegRecordSound::ShowSeconds(void)
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


void CRegRecordSound::CloseSound(void)
{
	OnButtonSoundClose();
}

void CRegRecordSound::StartRecord(void)
{
	OnButtonSoundRecord();
}
LRESULT CRegRecordSound::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
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
// 		if (((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pSoundDlg->m_pRecordSoundDlg->IsWindowVisible())
// 		{
			OnButtonSoundRecord();
//		}
 		break;
	case WM_BTLEFT:
		if(m_bPlaying)
		{
			m_nSoundindex--;
			if(m_nSoundindex  < 0)
				m_nSoundindex = 0;
		}
		waveOutSetVolume(NULL, volumeA[m_nSoundindex]);
		break;
	case WM_BTRIGHT:
		if(m_bPlaying)
		{
			m_nSoundindex++;
			if(m_nSoundindex > 4)
				m_nSoundindex = 4;
		}
		waveOutSetVolume(NULL, volumeA[m_nSoundindex]);
		break;
	}
		return CDialog::WindowProc(message, wParam, lParam);
}
LRESULT CRegRecordSound::OnChangeLanguage(WPARAM wParam,LPARAM lParam)
{
    ChangeLanguage();
    return 0;
}
void CRegRecordSound::ChangeLanguage()
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

void CRegRecordSound::SetSliderMaxVaule()
{  
   CString maxCurrent,strHex;;
   m_editMax.GetWindowText(strHex);
   if(!strHex.IsEmpty())
   {   
	   DWORD dwHex = strtoul(Util::StringOp::FromCString(strHex).c_str(), NULL, 16);
	   maxCurrent.Format(_T("%ld"), dwHex);
	   m_slider.SetRangeMax(Util::StringOp::ToInt(maxCurrent),TRUE);
   } 
}

void CRegRecordSound::SetSliderMinVaule()
{
	CString maxCurrent,strHex;
	m_editMin.GetWindowText(strHex);
	if(!strHex.IsEmpty())
	{   
		DWORD dwHex = strtoul(Util::StringOp::FromCString(strHex).c_str(), NULL, 16);
	    maxCurrent.Format(_T("%ld"), dwHex);
        m_slider.SetRangeMin(Util::StringOp::ToInt(maxCurrent),TRUE);
	}
}
void CRegRecordSound::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	int currentvalue= m_slider.GetPos();
	CString CSCrtValue;
	CSCrtValue.Format(_T("%x"),currentvalue);
	m_sticCurrentValue.SetWindowText(_T(""));
    m_sticCurrentValue.SetWindowText(CSCrtValue);
}

