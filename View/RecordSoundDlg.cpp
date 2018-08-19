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

LPBYTE _stdcall pcmReadSample_M16M08(LPBYTE pb, LPWORD pdw)
{
    *(LPWORD)pdw = (WORD)(((BYTE UNALIGNED *)pb)[0] ^ (BYTE)0x80) << 8;
	
    return ((LPBYTE)&((BYTE UNALIGNED *)pb)[1]);
} // pcmReadSample_M16M08()


/////////////////////////////////////////////////////////////////////////////
// CRecordSoundDlg dialog

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

void CRecordSoundDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if(!m_bPlaying)
		return;
	if(nChar == 'D')
	{
		m_nSoundindex++;
		if(m_nSoundindex > 4)
			m_nSoundindex = 4;
	}
	else if(nChar == 'U')
	{
		m_nSoundindex--;
		if(m_nSoundindex  < 0)
			m_nSoundindex = 0;
	}
	DWORD volume[] = {0xFF00FF00, 0xcc00cc00, 0x88008800, 0x44004400, 0x10001000}; 
	waveOutSetVolume(NULL, volume[m_nSoundindex]);
}

BEGIN_MESSAGE_MAP(CRecordSoundDlg, CCEDialog)
	//{{AFX_MSG_MAP(CRecordSoundDlg)
	ON_MESSAGE(MM_WIM_OPEN,OnMM_WIM_OPEN)
	ON_MESSAGE(MM_WIM_DATA,OnMM_WIM_DATA)
	ON_MESSAGE(MM_WIM_CLOSE,OnMM_WIM_CLOSE)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_RECORDDLG_CLOSE, OnButtonSoundClose)
	ON_BN_CLICKED(IDC_BUTTON_RECORDDLG_CLEAR, OnButtonSoundClear)
	ON_BN_CLICKED(IDC_BUTTON_RECORDDLG_PLAY, OnButtonSoundPlay)
	ON_BN_CLICKED(IDC_BUTTON_RECORDDLG_RECORD, OnButtonSoundRecord)
	ON_MESSAGE(WM_PROCESS_POS, OnPlaySeek)
	ON_MESSAGE(WM_CLICKMJPG_TOAPP, OnClickMJPG)
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRecordSoundDlg message handlers
void CRecordSoundDlg::OnClickMJPG(WPARAM w, LPARAM l)
{

	CMultimediaPhoneDlg *pWnd = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	switch(w)
	{
	case 1:
		OnButtonSoundClose();
		pWnd->m_pMainDlg->m_mainMp3Dlg_->SendMessage(WM_OUTEVENT, 0, 1);
		break;
	case 2:
		OnButtonSoundClear();
		break;
	case 3:
	case 5:
		pWnd->m_pMainDlg->m_mainMp3Dlg_->OnTimer(1002); //SendMessage(WM_OUTEVENT, 0, 0);
		OnButtonSoundPlay();
		break;
	case 4:
	case 6:
		pWnd->m_pMainDlg->m_mainMp3Dlg_->OnTimer(1002);//SendMessage(WM_OUTEVENT, 0, 0);
		OnButtonSoundRecord();
		break;		
	}
}

BOOL CRecordSoundDlg::OnInitDialog() 
{
	CCEDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	std::string strTemp;
	CString str;

	m_procbarSound.Create(WS_CHILD|WS_VISIBLE, CRect(55 + 54, 161 + 62, 520 + 54, 188 + 62), this, IDC_PBAR_RECORDDLG_PROCESS);
	m_procbarSound.SetParam(0, 0, 50, 1);
	m_procbarSound.SetPos(0);
	m_procbarSound.SetDisable(TRUE);
	
	m_MJPGList.Create(L"", WS_VISIBLE|WS_CHILD, CRect(54, 62, 746, 358), this);
	m_MJPGList.SetCurrentLinkFile(".\\adv\\mjpg\\k1\\中文\\本地录音.xml");
	m_MJPGList.SetMJPGRect(CRect(54, 62, 746, 358));

	m_MJPGList.SetUnitIsShow(0, TRUE);
	m_MJPGList.SetUnitIsShow(1, FALSE);
	m_MJPGList.SetUnitIsShow(2, FALSE);
	m_MJPGList.SetUnitIsShow(3, TRUE);
	m_MJPGList.SetUnitIsShow(4, FALSE);
	m_MJPGList.SetUnitIsShow(5, FALSE);

	m_bISRecording = FALSE;
	m_bPlaying = FALSE;
	m_bClear = FALSE;
	m_uiCurrentSecond = 0;
	m_uiTotalSecond = 0;

	m_spSoundSegment = boost::shared_ptr<Data::SoundSegment>();
		
	m_waveform.wFormatTag=WAVE_FORMAT_PCM;
	m_waveform.nChannels=CHANNELS;
	m_waveform.nSamplesPerSec=SAMPLESPERSEC;
	m_waveform.wBitsPerSample=BITSPERSAMPLE;//8;
	m_waveform.nBlockAlign=m_waveform.nChannels * m_waveform.wBitsPerSample / 8;
	m_waveform.nAvgBytesPerSec=m_waveform.nBlockAlign * m_waveform.nSamplesPerSec;
	m_waveform.cbSize=0;	

	for (int i = 0; i < WAVE_BUFFER_COUNT; ++i)
	{
		m_pBuffer[i] = 	(PBYTE)malloc(INP_BUFFER_SIZE);
		m_pWaveHdr[i] = reinterpret_cast<PWAVEHDR>(malloc(sizeof(WAVEHDR)));
	}

	m_pOggCodec = new OggCodec();
	m_bRecordOver = TRUE;
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

bool CRecordSoundDlg::RecStart(void)
{
	// TODO: Add your control notification handler code here
	path = ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pTempSetting->soundPath();
	
	BOOL DetectDIR(TCHAR *sDir);
	if (path == ssStorageCardRecordPath)//"\\StorageCard\\MY_RECORD\\")
	{
		BOOL sd = DetectDIR((LPTSTR)(LPCTSTR)csStarageCard);
		if (sd)
		{
			if (!DetectDIR((LPTSTR)(LPCTSTR)csStorageCardRecordPath))
			{
				CreateDirectory(csStorageCardRecordPath, NULL);
			}
		}
		else
		{
			path = ssFlashRecordPath;
			if (!DetectDIR((LPTSTR)(LPCTSTR)csFlashRecordPath))
			{
				CreateDirectory(csFlashRecordPath, NULL);
			}
		}
	}

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
		{
			((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pWarningNoFlashDlg->ShowWindow_(SW_SHOW);
			return false;
		}
		m_uiRemainRecordSecond = (freeBytes.QuadPart - (totalBytes.QuadPart * 0.1)) / secondBytes + 1;
	}
	else
	{
		if (freeBytes.QuadPart < secondBytes*2)
		{
			((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pWarningNoFlashDlg->ShowWindow_(SW_SHOW);
			return false;
		}
		m_uiRemainRecordSecond = freeBytes.QuadPart / secondBytes + 1;
	}
	
	CString sMax;
	sMax.Format(_T("录音最大时长:%02d小时%02d分"), m_uiRemainRecordSecond / 3600, m_uiRemainRecordSecond % 3600 / 60/*, m_uiRemainRecordSecond % 60*/);
	
	m_MJPGList.SetUnitText(102, sMax, TRUE);
	m_MJPGList.SetUnitIsShow(102, TRUE);
	
	m_spSoundSegment = boost::shared_ptr<Data::SoundSegment>(new Data::SoundSegment);
	
	CTime t;
	t = CTime::GetCurrentTime();
	CString filename;
	
	if (((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pSettingDlg->m_pSetting->isDeleteProtect())
	{
		m_pOggCodec->SetQuality(8);
		filename.Format(_T("%02d%02d%02dHQ.spx"), t.GetHour(), t.GetMinute(), t.GetSecond());
	}
	else
	{
		m_pOggCodec->SetQuality(2);
		filename.Format(_T("%02d%02d%02dST.spx"), t.GetHour(), t.GetMinute(), t.GetSecond());
	}
	
	std::string mf = path + Util::StringOp::FromCString(filename);
	m_spSoundSegment->filename(mf);
	m_spSoundSegment->type(Data::sstLocalRecord);
	m_spSoundSegment->startTime(CTime::GetCurrentTime());
	m_spSoundSegment->Insert();
	
	m_MJPGList.SetUnitText(100, filename, TRUE);
	m_MJPGList.SetUnitText(101, _T(""), TRUE);
	m_MJPGList.SetUnitIsShow(100, TRUE);
	m_MJPGList.SetUnitIsShow(101, FALSE);
	
	m_MJPGList.Invalidate();   //lxz	

	if (waveInOpen(&m_hWaveIn,WAVE_MAPPER,&m_waveform,(DWORD)this->m_hWnd,NULL,CALLBACK_WINDOW))
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
	
	// Begin sampling
	
	m_bEnding = FALSE ;
	waveInStart (m_hWaveIn) ;
	
	std::string comment = m_spSoundSegment->name() + "\'" + m_spSoundSegment->telephoneNumber().number() + "\'" + Util::StringOp::FromInt(m_spSoundSegment->type()) + "\'" + Util::StringOp::FromTimestamp(m_spSoundSegment->startTime()) + "\'";
	if(!m_pOggCodec->StartEncode(m_spSoundSegment->filename(), comment))
	{
		return false;
	}
	m_bRecordOver = FALSE;

	Dprintf("RecStart\n");
	return true;
}

bool CRecordSoundDlg::RecStop(void)
{
	m_MJPGList.SetUnitText(102, L"", TRUE);

	Dprintf("RecStop\n");
	m_bEnding=TRUE;	
	waveInReset(m_hWaveIn);
//	waveInClose(m_hWaveIn);
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

	m_MJPGList.SetUnitIsShow(1, FALSE);
	m_MJPGList.SetUnitIsShow(2, FALSE);
	m_MJPGList.SetUnitIsShow(3, TRUE);
	m_MJPGList.SetUnitIsShow(5, FALSE);
	m_MJPGList.SetUnitIsShow(4, FALSE);

	m_MJPGList.SetUnitText(100, L"", FALSE);
	m_MJPGList.SetUnitText(101, L"", FALSE);
	m_MJPGList.SetUnitIsShow(100, FALSE);
	m_MJPGList.SetUnitIsShow(101, FALSE);
	m_MJPGList.SetUnitIsShow(102, FALSE);
	m_MJPGList.SetUnitText(104, L"", FALSE);

	m_uiCurrentSecond = 0;
	m_uiTotalSecond = 0;

	m_procbarSound.SetPos(0);
	m_procbarSound.SetDisable(TRUE);
	ShowWindow_(FALSE);
	((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->SetScreenSaveTimer();
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
		Data::SoundSegment::RemoveDataById(m_spSoundSegment->id());
		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pSoundDlg->ShowItemsInList(2);
	}
	catch (CFileException* pEx)
	{
 		pEx->Delete();
 	}

	m_bClear = TRUE;

	m_uiCurrentSecond = 0;
	m_uiTotalSecond = 0;
	ShowSeconds();
	
	m_procbarSound.SetDisable(TRUE);
	m_MJPGList.SetUnitText(100, L"", TRUE);
	m_MJPGList.SetUnitText(104, L"", TRUE);
	m_MJPGList.SetUnitText(101, L"", TRUE);
	m_MJPGList.SetUnitIsShow(101, FALSE);

	m_procbarSound.SetPos(0);
}

void CRecordSoundDlg::OnButtonSoundPlay()
{
	if (m_bPlaying)
	{
		m_bPlaying = FALSE;

		m_MJPGList.SetUnitIsShow(1, TRUE);
		m_MJPGList.SetUnitIsShow(2, TRUE);
		m_MJPGList.SetUnitIsShow(3, TRUE);
		m_MJPGList.SetUnitIsShow(4, FALSE);
		m_MJPGList.SetUnitIsShow(5, FALSE);

		PlayStop();
		KillTimer(2);
		m_uiCurrentSecond = 0;
		ShowSeconds();
		m_procbarSound.SetPos(0);
	}
	else
	{
		//如果正在播放铃声,或者自动答录，直接退出
		if(((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->phone_->m_bStartRing)
			return;

		if (PlayStart())
		{
			m_bPlaying = TRUE;
			m_MJPGList.SetUnitIsShow(1, FALSE);
			m_MJPGList.SetUnitIsShow(2, FALSE);
			m_MJPGList.SetUnitIsShow(3, TRUE);
			m_MJPGList.SetUnitIsShow(5, FALSE);
			m_MJPGList.SetUnitIsShow(4, TRUE);
			m_uiCurrentSecond = 0;
			m_procbarSound.SetParam(0, 0, m_uiTotalSecond, 1);
			SetTimer(2, 1000, NULL);
		}
	}
}

void CRecordSoundDlg::OnButtonSoundRecord()
{
	if (m_bPlaying)
		return;

	if (m_bISRecording)
	{
		m_bISRecording = FALSE;
 		KillTimer(1);
 		m_uiTotalSecond = m_uiCurrentSecond;
 		m_uiCurrentSecond = 0;
 		ShowSeconds();
		m_procbarSound.SetDisable(FALSE);
		RecStop(); 
		m_spSoundSegment->duration(m_uiTotalSecond);
		((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSoundDlg->SaveSoundSegment(m_spSoundSegment);
		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pSoundDlg->ShowItemsInList(2);   //移到 close去做
	}
	else
	{
		//如果正在播放铃声,或者自动答录，直接退出
		if(((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->phone_->m_bStartRing  )
			return;

		m_procbarSound.SetDisable(TRUE);

		if (RecStart())   
		{
			m_bISRecording = TRUE;

			m_MJPGList.SetUnitIsShow(1, FALSE);
			m_MJPGList.SetUnitIsShow(2, FALSE);
			m_MJPGList.SetUnitIsShow(3, FALSE);
			m_MJPGList.SetUnitIsShow(4, FALSE);
			m_MJPGList.SetUnitIsShow(5, TRUE);

			m_bClear = FALSE;
			m_uiCurrentSecond = 0;
			m_uiTotalSecond = 0;
			ShowSeconds();

			m_MJPGList.SetUnitIsShow(103, TRUE);
			m_MJPGList.SetUnitIsShow(104, TRUE);
			m_MJPGList.SetUnitText(104, L"00:00", TRUE);
			m_MJPGList.SetUnitText(103, L"时长:", TRUE);
			m_MJPGList.SetUnitIsShow(101, FALSE);
			m_MJPGList.Invalidate(); //lxz

			SetTimer(1, 1000, NULL);  
		}
	}
}

void CRecordSoundDlg::OnMM_WIM_OPEN(UINT wParam, LONG lParam) 
{
	// TODO: Add your message handler code here and/or call default
	Dprintf("OnMM_WIM_OPEN\n");
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
		Dprintf("waveInClose\n");
		waveInClose(m_hWaveIn) ;
		return;
	}
	
	// Send out a new buffer	
	waveInAddBuffer (m_hWaveIn, (PWAVEHDR) lParam, sizeof (WAVEHDR));
}

void CRecordSoundDlg::OnMM_WIM_CLOSE(UINT wParam, LONG lParam) 
{
	// TODO: Add your message handler code here and/or call default
	Dprintf("OnMM_WIM_CLOSE\n");

	for (int i = 0; i < WAVE_BUFFER_COUNT; ++i)
	{
		waveInUnprepareHeader (m_hWaveIn, m_pWaveHdr[i], sizeof (WAVEHDR));
	}
	
	Dprintf("OnMM_WIM_CLOSE1\n");

	m_pOggCodec->StopEncode(true);
	m_bRecordOver = TRUE;

	Dprintf("OnMM_WIM_CLOSE2\n");
	m_bISRecording = FALSE;
	m_MJPGList.SetUnitIsShow(1, TRUE);
	m_MJPGList.SetUnitIsShow(2, TRUE);
	m_MJPGList.SetUnitIsShow(3, TRUE);
	m_MJPGList.SetUnitIsShow(4, FALSE);
	m_MJPGList.SetUnitIsShow(5, FALSE);

	KillTimer(1);
	m_uiCurrentSecond = 0;
	ShowSeconds();

	m_MJPGList.SetUnitText(103, L"", TRUE);
	m_MJPGList.SetUnitText(104, L"", TRUE);
	m_MJPGList.SetUnitIsShow(101, TRUE);
	
	Sleep(500);
	((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pSoundDlg->ShowItemsInList(-1);
}

void CRecordSoundDlg::OnPlaySeek(WPARAM w, LPARAM l)
{
	if ((int)l == IDC_PBAR_RECORDDLG_PROCESS)
	{
		if (m_bPlaying)
		{
			CString filename = m_MJPGList.GetUnitText(100);
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
		extern BOOL DetectDIR(TCHAR *sDir);
		if (path == ssStorageCardRecordPath)
		{
			if( !DetectDIR(_T("/storagecard")))
			{
				m_MJPGList.SetUnitText(100, L"SD卡被拔出", TRUE);
				OnButtonSoundRecord();
				CCEDialog::OnTimer(nIDEvent);
				return;
			}
		}
		
		CTimeSpan ts;
		ts = CTime::GetCurrentTime() - m_spSoundSegment->startTime();
		m_uiCurrentSecond = ts.GetTotalSeconds();

		CString duration;
		if (m_uiCurrentSecond > 3600)
		{
			duration.Format(_T("%02d:%02d:%02d"), m_uiCurrentSecond / 3600, m_uiCurrentSecond % 3600 / 60, m_uiCurrentSecond % 60);
		}
		else
		{
			duration.Format(_T("%02d:%02d"), m_uiCurrentSecond / 60, m_uiCurrentSecond % 60);
		}
		m_MJPGList.SetUnitText(104, duration, TRUE);
		if ((m_uiCurrentSecond % 60) == 0)
		{
			m_spSoundSegment->duration(m_uiCurrentSecond);
			m_spSoundSegment->Update();
		}
		if (m_uiCurrentSecond >= m_uiRemainRecordSecond)
		{
			OnButtonSoundRecord();
		}

		else		//关闭屏保，背光
		{
			CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
			main->CancelBalckLightSaveTimer();
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

			//关闭屏保，背光
			CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
			main->CancelBalckLightSaveTimer();
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
	m_MJPGList.SetUnitText(101, strTime, TRUE);
}


void CRecordSoundDlg::CloseSound(void)
{
	OnButtonSoundClose();
}

void CRecordSoundDlg::StartRecord(void)
{
	OnButtonSoundRecord();
}

void CRecordSoundDlg::ShowWindow_(int cmdshow)
{
	if(cmdshow > 0)
	{
		CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
		m_nSoundindex = main->m_pSettingDlg->m_pSetting->sysVolume();
		DWORD volume[] = {0xFF00FF00, 0xcc00cc00, 0x88008800, 0x44004400, 0x10001000}; 
		waveOutSetVolume(NULL, volume[m_nSoundindex]);
	}
	CCEDialog::ShowWindow_(cmdshow);
}