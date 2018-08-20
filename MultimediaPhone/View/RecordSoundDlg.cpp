// RecordSoundDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RecordSoundDlg.h"
#include "../MultimediaPhone.h"
#include "../MultimediaPhoneDlg.h"
#include "../Data/LanguageResource.h"
#include "../Data/SkinStyle.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace View {
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
    END_MESSAGE_MAP()

    /////////////////////////////////////////////////////////////////////////////
    // CRecordSoundDlg message handlers
    LRESULT CRecordSoundDlg::OnClickMJPG(WPARAM w, LPARAM l)
    {
        LRESULT result = 0;
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
        return result;
    }

    BOOL CRecordSoundDlg::OnInitDialog() 
    {
        CCEDialog::OnInitDialog();

        // TODO: Add extra initialization here
        std::string strTemp;
        CString str;

		m_MJPGList.Create(L"", WS_VISIBLE|WS_CHILD, CRect(54*X_XISHU, 62*Y_XISHU, 746*X_XISHU, 358*Y_XISHU), this);
        m_MJPGList.SetCurrentLinkFile(_T(".\\adv\\mjpg\\k1\\中文\\本地录音.xml"));
        m_MJPGList.SetMJPGRect(CRect(54*X_XISHU, 62*Y_XISHU, 746*X_XISHU, 358*Y_XISHU));

        m_sticSoundName.Create(str, WS_CHILD|WS_VISIBLE, CRect((48 )*X_XISHU, (95 )*Y_XISHU, (280  - 8)*X_XISHU, (125 )*Y_XISHU), &m_MJPGList);
        m_sticSoundName.SetColor(RGB(0, 0, 0), RGB(203, 203, 203));//Data::g_partFrameInRectBackRGB[Data::g_skinstyle]);

        m_sticSoundDuration.Create(str, WS_CHILD/*|WS_VISIBLE*/, CRect((280  - 8)*X_XISHU, (95 )*Y_XISHU, (526 )*X_XISHU, (125 )*Y_XISHU), &m_MJPGList);
        m_sticSoundDuration.SetColor(RGB(0, 0, 0), RGB(203, 203, 203));//Data::g_partFrameInRectBackRGB[Data::g_skinstyle]);
        m_sticSoundDuration.SetAlign(SS_RIGHT);	

        m_procbarSound.Create(WS_CHILD|WS_VISIBLE, CRect((55 )*X_XISHU, (161 )*Y_XISHU, (520 )*X_XISHU, (188 )*Y_XISHU), &m_MJPGList, IDC_PBAR_RECORDDLG_PROCESS);
        m_procbarSound.SetParam(0, 0, 50, 1);
        m_procbarSound.SetPos(0);
        /*
        strTemp = Data::LanguageResource::Get(Data::RI_SOUND_PLAY_CLOSE);
        str = strTemp.c_str();
        m_btnClose.Create(str, Data::g_buttonArcBMPPARTDILOAGID[0][Data::g_skinstyle], Data::g_buttonArcBMPPARTDILOAGID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(378, 78, 434, 98), this, IDC_BUTTON_RECORDDLG_CLOSE);
        m_btnClose.SetBackRGB(Data::g_partFrameMainBackRGB[Data::g_skinstyle]);

        strTemp = Data::LanguageResource::Get(Data::RI_VEDIO_DELETEALLBTN);
        str = strTemp.c_str();
        m_btnClear.Create(str, Data::g_buttonArcBMPPARTDILOAGID[0][Data::g_skinstyle], Data::g_buttonArcBMPPARTDILOAGID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(378, 103, 434, 123), this, IDC_BUTTON_RECORDDLG_CLEAR);
        m_btnClear.SetBackRGB(Data::g_partFrameMainBackRGB[Data::g_skinstyle]);

        strTemp = Data::LanguageResource::Get(Data::RI_SOUND_PLAY_PLAY);
        str = strTemp.c_str();
        m_btnPlay.Create(str, Data::g_buttonArcBMPPARTDILOAGID[0][Data::g_skinstyle], Data::g_buttonArcBMPPARTDILOAGID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(378, 128, 434, 148), this, IDC_BUTTON_RECORDDLG_PLAY);
        m_btnPlay.SetBackRGB(Data::g_partFrameMainBackRGB[Data::g_skinstyle]);

        strTemp = Data::LanguageResource::Get(Data::RI_TEL_RECORDEBTN);
        str = strTemp.c_str();
        m_btnRecord.Create(str, Data::g_buttonArcBMPPARTDILOAGID[0][Data::g_skinstyle], Data::g_buttonArcBMPPARTDILOAGID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(378, 153, 434, 173), this, IDC_BUTTON_RECORDDLG_RECORD);
        m_btnRecord.SetBackRGB(Data::g_partFrameMainBackRGB[Data::g_skinstyle]);
        */
        //m_btnRecordTip.Create(_T(""),IDB_BITMAP31, IDB_BITMAP31, WS_CHILD|WS_VISIBLE, CRect(328 , 223 , 370 , 250 ), this, IDC_BUTTON_TELEPHONE_RECORDTIP);
        //m_btnRecordTip.SetBackRGB(Data::g_partFrameMainBackRGB[Data::g_skinstyle]);

        strTemp = Data::LanguageResource::Get(Data::RI_TEL_RECORDING);
        str = strTemp.c_str();
        str = _T("");
        m_sticRecordCaption.Create(str, WS_CHILD|WS_VISIBLE, CRect((372 )*X_XISHU, (216 )*Y_XISHU, (430 )*X_XISHU, (246 )*Y_XISHU), &m_MJPGList);
        m_sticRecordCaption.SetColor(RGB(0, 0, 0), RGB(203, 203, 203));//Data::g_partFrameInRectBackRGB[Data::g_skinstyle]);

        m_sticRecord.Create(_T(""), WS_CHILD|WS_VISIBLE, CRect((430 )*X_XISHU, (216 )*Y_XISHU, (528 )*X_XISHU, (246 )*Y_XISHU), &m_MJPGList);
        m_sticRecord.SetColor(RGB(0, 0, 0), RGB(203, 203, 203));//Data::g_partFrameInRectBackRGB[Data::g_skinstyle]);

        m_sticDurationMax.Create(_T(""), WS_CHILD/*|WS_VISIBLE*/, CRect((48 )*X_XISHU, (216 )*Y_XISHU, (330 )*X_XISHU, (246 )*Y_XISHU), &m_MJPGList);
        m_sticDurationMax.SetColor(RGB(0, 0, 0), RGB(203, 203, 203));//Data::g_partFrameInRectBackRGB[Data::g_skinstyle]);


        /*
        m_sticBackground.Create(CRect(34, 40, 448, 184), this, 1);

        TextStruct ts[1];
        memset(ts, 0, sizeof(TextStruct) * 1);

        ts[0].txtRect = CRect(8, 0, 100, 20);
        ts[0].txtFontSize = 16;
        ts[0].sAlign = DT_LEFT | DT_BOTTOM;
        memcpy(ts[0].sTxt, Data::LanguageResource::Get(Data::RI_SOUND_LOCALRECORD).c_str(), Data::LanguageResource::Get(Data::RI_SOUND_LOCALRECORD).length());

        m_sticBackground.SetTextStruct(ts, 1);
        */

        // 	m_btnClear.EnableWindow(FALSE);
        // 	m_btnPlay.EnableWindow(FALSE);

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
        m_spSoundSegment = Util::shared_ptr<Data::SoundSegment>();

        m_waveform.wFormatTag=WAVE_FORMAT_PCM;
        m_waveform.nChannels=CHANNELS;
        m_waveform.nSamplesPerSec=SAMPLESPERSEC;
        m_waveform.wBitsPerSample=BITSPERSAMPLE;//8;
        m_waveform.nBlockAlign=m_waveform.nChannels * m_waveform.wBitsPerSample / 8;
        m_waveform.nAvgBytesPerSec=m_waveform.nBlockAlign * m_waveform.nSamplesPerSec;
        m_waveform.cbSize=0;	

        // 	m_pWaveHdr1 = reinterpret_cast<PWAVEHDR>(malloc(sizeof(WAVEHDR)));
        // 	m_pWaveHdr2 = reinterpret_cast<PWAVEHDR>(malloc(sizeof(WAVEHDR)));
        // 	m_pBuffer1=(PBYTE)malloc(INP_BUFFER_SIZE);
        // 	m_pBuffer2=(PBYTE)malloc(INP_BUFFER_SIZE);
        // 	
        // 	if (!m_pBuffer1 || !m_pBuffer2)
        // 	{
        // 		if (m_pBuffer1) free(m_pBuffer1);
        // 		if (m_pBuffer2) free(m_pBuffer2);
        // 		Dprintf("buffer malloc error!\n");
        // 	}

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

        // 	if (!m_spSoundSegment)
        // 	{
        // 		if (m_sstType == Data::sstTips)
        // 		{
        // 			path = "\\FlashDrv\\MY_RECORD\\";
        // 		}
        // 		else
        // 		{
        path = ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pSetting->soundPath();

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
        //		}


        ULARGE_INTEGER freeBytes;
        ULARGE_INTEGER totalBytes;
        GetDiskFreeSpaceEx(Util::StringOp::ToCString(path), &freeBytes, &totalBytes, NULL);

        if (path == ssFlashRecordPath)
        {
            if (freeBytes.QuadPart < totalBytes.QuadPart * 0.1)
                //if (totalBytes.QuadPart < MINFREEBYTES)
            {
                ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pWarningNoFlashDlg->ShowWindow_(SW_SHOW);
                return false;
            }

            //m_uiRemainRecordSecond = (totalBytes.QuadPart - MINFREEBYTES) / SECONDBYTES + 1;
            m_uiRemainRecordSecond = (freeBytes.QuadPart - (totalBytes.QuadPart * 0.1)) / SECONDBYTES + 1;
        }
        else
        {
            if (freeBytes.QuadPart < SECONDBYTES)
            {
                ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pWarningNoFlashDlg->ShowWindow_(SW_SHOW);
                return false;
            }
            m_uiRemainRecordSecond = (UINT)freeBytes.QuadPart / SECONDBYTES + 1;
        }

        CString sMax;
        sMax.Format(_T("录音最大时长: %02d:%02d:%02d"), m_uiRemainRecordSecond / 3600, m_uiRemainRecordSecond % 3600 / 60, m_uiRemainRecordSecond % 60);
        m_sticDurationMax.SetWindowText(sMax);
        m_sticDurationMax.ShowWindow(TRUE);

        m_spSoundSegment = Util::shared_ptr<Data::SoundSegment>(new Data::SoundSegment);

        CTime t;
        t = CTime::GetCurrentTime();
        CString filename;
        filename.Format(_T("%02d%02d%02d.spx"), t.GetHour(), t.GetMinute(), t.GetSecond());
        std::string mf = path + Util::StringOp::FromCString(filename);
        m_spSoundSegment->filename(mf);
        m_spSoundSegment->type(Data::sstLocalRecord);
        //		m_spSoundSegment->type(m_sstType);
        m_sticSoundName.SetWindowText(filename);
        m_spSoundSegment->startTime(CTime::GetCurrentTime());
        m_sticSoundDuration.SetWindowText(_T("00:00"));
        m_sticSoundName.ShowWindow(TRUE);
        m_sticSoundDuration.ShowWindow(TRUE);
        //	}

        if (waveInOpen(&m_hWaveIn,WAVE_MAPPER,&m_waveform,(DWORD)this->m_hWnd,NULL,CALLBACK_WINDOW))
        {
            return false;
        }

        waveInMessage(m_hWaveIn,0x30000002,0,0);

        // 	m_pWaveHdr1->lpData=(char*)m_pBuffer1;
        // 	m_pWaveHdr1->dwBufferLength=INP_BUFFER_SIZE;
        // 	m_pWaveHdr1->dwBytesRecorded=0;
        // 	m_pWaveHdr1->dwUser=0;
        // 	m_pWaveHdr1->dwFlags=0;
        // 	m_pWaveHdr1->dwLoops=1;
        // 	m_pWaveHdr1->lpNext=NULL;
        // 	m_pWaveHdr1->reserved=0;
        // 	
        // 	MMRESULT r = waveInPrepareHeader(m_hWaveIn,m_pWaveHdr1,sizeof(WAVEHDR));
        // 	
        // 	m_pWaveHdr2->lpData=(char*)m_pBuffer2;
        // 	m_pWaveHdr2->dwBufferLength=INP_BUFFER_SIZE;
        // 	m_pWaveHdr2->dwBytesRecorded=0;
        // 	m_pWaveHdr2->dwUser=0;
        // 	m_pWaveHdr2->dwFlags=0;
        // 	m_pWaveHdr2->dwLoops=1;
        // 	m_pWaveHdr2->lpNext=NULL;
        // 	m_pWaveHdr2->reserved=0;
        // 	
        // 	r = waveInPrepareHeader(m_hWaveIn,m_pWaveHdr2,sizeof(WAVEHDR));
        // 	
        // 	waveInAddBuffer (m_hWaveIn, m_pWaveHdr1, sizeof (WAVEHDR)) ;
        // 	waveInAddBuffer (m_hWaveIn, m_pWaveHdr2, sizeof (WAVEHDR)) ;// 

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

        m_pOggCodec->EncodeFile(m_spSoundSegment->filename());
        if (!m_pOggCodec->StartEncode())
        {
            return false;
        }
        m_bRecordOver = FALSE;

        //Dprintf("RecStart\n");
        return true;
    }

    bool CRecordSoundDlg::RecStop(void)
    {
        m_sticDurationMax.SetWindowText(L"");
        //Dprintf("RecStop\n");
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
            m_pOggCodec->DecodeFile(m_spSoundSegment->filename());
            return m_pOggCodec->StartDecode();	
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

        // 	m_btnClear.EnableWindow(FALSE);
        // 	m_btnPlay.EnableWindow(FALSE);
        m_MJPGList.SetUnitIsShow(1, FALSE);
        m_MJPGList.SetUnitIsShow(2, FALSE);
        m_MJPGList.SetUnitIsShow(3, TRUE);
        m_MJPGList.SetUnitIsShow(5, FALSE);
        m_MJPGList.SetUnitIsShow(4, FALSE);

        m_sticSoundName.SetWindowText(_T(""));
        m_sticSoundDuration.SetWindowText(_T(""));
        m_sticSoundName.ShowWindow(FALSE);
        m_sticSoundDuration.ShowWindow(FALSE);
        m_sticDurationMax.ShowWindow(FALSE);
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
            Data::SoundSegment::RemoveDataById(m_spSoundSegment->id());
            ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pSoundDlg->ShowItemsInList(2);
        }
        catch (CFileException* pEx)
        {
            pEx->Delete();
        }

        m_bClear = TRUE;
        m_sticSoundName.SetWindowText(_T(""));
        m_sticRecord.SetWindowText(_T("00:00"));
        m_sticSoundDuration.ShowWindow(FALSE);
        m_uiCurrentSecond = 0;
        m_uiTotalSecond = 0;
        ShowSeconds();
        m_procbarSound.SetPos(0);
    }

    void CRecordSoundDlg::OnButtonSoundPlay()
    {
        if (m_bPlaying)
        {
            // 		std::string s = Data::LanguageResource::Get(Data::RI_SOUND_PLAY_PLAY);
            // 		m_btnPlay.SetWindowText(CString(s.c_str()));
            m_bPlaying = FALSE;

            // 		m_btnClear.EnableWindow(TRUE);
            // 		m_btnRecord.EnableWindow(TRUE);

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
            waveOutSetVolume(NULL, ((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->phone_->g_RingSound);

            if (PlayStart())
            {
                // 			std::string s = Data::LanguageResource::Get(Data::RI_SOUND_RECORD_STOPPLAY);
                // 			m_btnPlay.SetWindowText(CString(s.c_str()));
                m_bPlaying = TRUE;
                // 			m_btnClear.EnableWindow(FALSE);
                // 			m_btnRecord.EnableWindow(FALSE);
                m_MJPGList.SetUnitIsShow(1, FALSE);
                m_MJPGList.SetUnitIsShow(2, FALSE);
                m_MJPGList.SetUnitIsShow(3, TRUE);//FALSE);
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
            // 		std::string s = Data::LanguageResource::Get(Data::RI_TEL_RECORDEBTN);
            // 		m_btnRecord.SetWindowText(CString(s.c_str()));
            // 		m_bISRecording = FALSE;
            // 		m_btnClear.EnableWindow(TRUE);
            // 		m_btnPlay.EnableWindow(TRUE);
            // 		KillTimer(1);
            // 		m_uiTotalSecond = m_uiCurrentSecond;
            // 		m_uiCurrentSecond = 0;
            // 		ShowSeconds();
            // 		m_btnRecordTip.ShowWindow(FALSE);
            // 		m_sticRecordCaption.ShowWindow(FALSE);
            // 		m_sticRecord.ShowWindow(FALSE);
            // 		m_sticRecord.SetWindowText(_T("00:00"));
            m_procbarSound.SetDisable(FALSE);
            RecStop();
        }
        else
        {
            m_procbarSound.SetDisable(TRUE);

            if (RecStart())
            {
                // 			std::string s = Data::LanguageResource::Get(Data::RI_TEL_STOPRECORDEBTN);
                // 			m_btnRecord.SetWindowText(CString(s.c_str()));
                m_bISRecording = TRUE;
                // 			m_btnClear.EnableWindow(FALSE);
                // 			m_btnPlay.EnableWindow(FALSE);

                m_MJPGList.SetUnitIsShow(1, FALSE);
                m_MJPGList.SetUnitIsShow(2, FALSE);
                m_MJPGList.SetUnitIsShow(3, FALSE);
                m_MJPGList.SetUnitIsShow(4, FALSE);
                m_MJPGList.SetUnitIsShow(5, TRUE);

                m_bClear = FALSE;
                m_uiCurrentSecond = 0;
                m_uiTotalSecond = 0;
                ShowSeconds();
                //m_btnRecordTip.ShowWindow(TRUE);
                m_sticRecordCaption.ShowWindow(TRUE);
                m_sticRecord.ShowWindow(TRUE);
                m_sticRecordCaption.SetWindowText(_T("时长:"));
                m_sticRecord.SetWindowText(_T("00:00"));
                m_sticSoundDuration.ShowWindow(FALSE);
                SetTimer(1, 1000, NULL);
            }
        }
    }

    LRESULT CRecordSoundDlg::OnMM_WIM_OPEN(UINT wParam, LONG lParam) 
    {
        LRESULT result = 0;
        // TODO: Add your message handler code here and/or call default
        //Dprintf("OnMM_WIM_OPEN\n");
        return result;
    }

    LRESULT CRecordSoundDlg::OnMM_WIM_DATA(UINT wParam, LONG lParam) 
    {
        LRESULT result = 0;
        //Dprintf("OnMM_WIM_DATA\n");		
        //Dprintf("OnMM_WIM_DATA = %d\n", ((PWAVEHDR) lParam)->dwBytesRecorded);

        // 	OggCodec::PcmData * pd = new OggCodec::PcmData;
        // 	pd->dataLength = ((PWAVEHDR) lParam)->dwBytesRecorded;
        // 	pd->data = (short*)malloc(((PWAVEHDR) lParam)->dwBytesRecorded * 2);
        // 
        // 	for (int i = 0; i < ((PWAVEHDR) lParam)->dwBytesRecorded; ++i)
        // 	{
        // 		pcmReadSample_M16M08((unsigned char *)(((PWAVEHDR) lParam)->lpData) + i, (unsigned short *)(pd->data) + i);
        // 	}
        // 	m_pOggCodec->PushPcmData(pd);

        OggCodec::PcmData * pd = new OggCodec::PcmData;
        pd->dataLength = ((PWAVEHDR) lParam)->dwBytesRecorded / sizeof(short);
        pd->data = (short*)malloc(((PWAVEHDR) lParam)->dwBytesRecorded);
        memcpy(pd->data , ((PWAVEHDR) lParam)->lpData, ((PWAVEHDR) lParam)->dwBytesRecorded);
        m_pOggCodec->PushPcmData(pd);

        if (m_bEnding)
        {
            //Dprintf("waveInClose\n");
            //waveInReset(m_hWaveIn);
            waveInClose(m_hWaveIn) ;
            return result;
        }

        // Send out a new buffer	
        waveInAddBuffer (m_hWaveIn, (PWAVEHDR) lParam, sizeof (WAVEHDR));
        return result;
    }

    LRESULT CRecordSoundDlg::OnMM_WIM_CLOSE(UINT wParam, LONG lParam) 
    {
        LRESULT result = 0;
        // TODO: Add your message handler code here and/or call default
        //Dprintf("OnMM_WIM_CLOSE\n");

        // 	waveInUnprepareHeader (m_hWaveIn, m_pWaveHdr1, sizeof (WAVEHDR)) ;
        // 	waveInUnprepareHeader (m_hWaveIn, m_pWaveHdr2, sizeof (WAVEHDR)) ;

        for (int i = 0; i < WAVE_BUFFER_COUNT; ++i)
        {
            waveInUnprepareHeader (m_hWaveIn, m_pWaveHdr[i], sizeof (WAVEHDR));
        }

        //Dprintf("OnMM_WIM_CLOSE1\n");

        m_pOggCodec->StopEncode(true);
        m_bRecordOver = TRUE;

        //Dprintf("OnMM_WIM_CLOSE2\n");
        // 	std::string s = Data::LanguageResource::Get(Data::RI_TEL_RECORDEBTN);
        // 	m_btnRecord.SetWindowText(CString(s.c_str()));
        m_bISRecording = FALSE;
        // 	m_btnClear.EnableWindow(TRUE);
        // 	m_btnPlay.EnableWindow(TRUE);
        m_MJPGList.SetUnitIsShow(1, TRUE);
        m_MJPGList.SetUnitIsShow(2, TRUE);
        m_MJPGList.SetUnitIsShow(3, TRUE);
        m_MJPGList.SetUnitIsShow(4, FALSE);
        m_MJPGList.SetUnitIsShow(5, FALSE);

        KillTimer(1);
        m_uiTotalSecond = m_uiCurrentSecond;
        m_uiCurrentSecond = 0;
        ShowSeconds();
        //m_btnRecordTip.ShowWindow(FALSE);
        // 	m_sticRecordCaption.ShowWindow(FALSE);
        // 	m_sticRecord.ShowWindow(FALSE);
        m_sticRecordCaption.SetWindowText(_T(""));
        m_sticRecord.SetWindowText(_T(""));
        m_sticSoundDuration.ShowWindow(TRUE);

        // 	if (m_uiTotalSecond == 0)
        // 	{
        // 		m_uiTotalSecond = 1;
        // 	}
        // 	m_spSoundSegment->duration(Util::TimeSpan(m_uiTotalSecond));
        // 	m_spSoundSegment->Insert();
        // 	((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pSoundDlg->ShowItemsInList();

        if (m_uiTotalSecond > 1)
        {
            m_spSoundSegment->duration(m_uiTotalSecond);
            //m_spSoundSegment->Insert();
            ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSoundDlg->SaveSoundSegment(m_spSoundSegment);
            ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pSoundDlg->ShowItemsInList(2);
            //		m_uiTotalSecond = 0;
        }
        else
        {
            CFile::Remove(Util::StringOp::ToCString(m_spSoundSegment->filename()));
        }
        Sleep(500);
        return result;
    }

    LRESULT CRecordSoundDlg::OnPlaySeek(WPARAM w, LPARAM l)
    {
        LRESULT result = 0;
        if ((int)l == IDC_PBAR_RECORDDLG_PROCESS)
        {
            if (m_bPlaying)
            {
                if (m_pOggCodec->DecodeSeek((float)w/(float)m_uiTotalSecond))
                {
                    m_uiCurrentSecond = (int)w;
                }
            }
        }
        return result;
    }

    void CRecordSoundDlg::OnTimer(UINT nIDEvent) 
    {
        // TODO: Add your message handler code here and/or call default
        if (nIDEvent == 1)
        {
            //	path = ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pSetting->soundPath();

            extern BOOL DetectDIR(TCHAR *sDir);
            if (path == ssStorageCardRecordPath)
            {
                if( !DetectDIR(_T("/storagecard")))
                {
                    m_sticSoundName.SetWindowText(L"SD卡被拔出");
                    OnButtonSoundRecord();
                    CCEDialog::OnTimer(nIDEvent);
                    return;
                }
            }

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

    // void CRecordSoundDlg::SetSoundSegmentType(Data::SoundSegmentType type)
    // {
    // 	m_sstType = type;
    // }
}
