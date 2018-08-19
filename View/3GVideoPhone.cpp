// 3GVideoPhone.cpp : implementation file
//

#include "stdafx.h"
#include "../multimediaphone.h"
#include "../MultimediaPhoneDlg.h"
#include "3GVideoPhone.h"
#include "../waveHeader.h"
#include "../inc/SABTD.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define SECONDBYTES8           2048
#define SECONDBYTES2           850

extern HINSTANCE m_hSABTD;
extern SabtdInit SabtdInit_;
extern SabtdUnInit SabtdUnInit_;
extern SabtdSetParam SabtdSetParam_;
extern SabtdGetParam SabtdGetParam_;
extern SabtdReset SabtdReset_;
extern SabtdProcess SabtdProcess_;

static void* m_sabtd_struct;
static std::queue<Util::Audio::OggCodec::PcmData*> CheckQueue;
static HANDLE hCheckThread;
static volatile bool bVoiceStop = true;
extern CRITICAL_SECTION csVoice;

extern short* pVoice;
extern int nVoiceLength;
extern CString telNumber;
/////////////////////////////////////////////////////////////////////////////
// C3GVideoPhone dialog

C3GVideoPhone::C3GVideoPhone(CWnd* pParent /*=NULL*/)
: CDialog(C3GVideoPhone::IDD, pParent) {
    //{{AFX_DATA_INIT(C3GVideoPhone)
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
    m_SoundSize = SOUND_DEFAULT_VOLUME;     //声音大小需要读取
    m_IsSound = TRUE;
}

void C3GVideoPhone::DoDataExchange(CDataExchange* pDX) {
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(C3GVideoPhone)
    // NOTE: the ClassWizard will add DDX and DDV calls here
    //}}AFX_DATA_MAP

}

BEGIN_MESSAGE_MAP(C3GVideoPhone, CDialog)
    //{{AFX_MSG_MAP(C3GVideoPhone)
    ON_MESSAGE(WM_CLICKMJPG_TOAPP, OnClickMJPG)
    //}}AFX_MSG_MAP
    ON_BN_CLICKED(IDC_BUTTON_TELEPHONE_RECORD, OnButtonTelephoneRecord)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// C3GVideoPhone message handlers

DWORD WINAPI VoiceCheckThreadPrc(LPVOID lpParameter);

BOOL C3GVideoPhone::OnInitDialog() {
    CDialog::OnInitDialog();

    // TODO: Add extra initialization here
    m_MJPGList.Create(L"", WS_VISIBLE | WS_CHILD, CRect(0, 0, 800, 423), this);
    m_MJPGList.SetCurrentLinkFile(".\\adv\\mjpg\\k5\\中文\\3g_语音电话.xml");
    MoveWindow(CRect(0, 57, 800, 480)); 
    ChangeVolume(m_SoundSize + 50);
    m_MJPGList.SetUnitText(12, telNumber, TRUE);

    if (!InitData()) {
        MessageBox(_T("Telephone Initialize error!\n"), _T("Error"));
    }

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

bool C3GVideoPhone::InitData(void) {
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
    //  m_bAutoRecord = FALSE;
    m_bHasCallID = FALSE;
    m_bPlayingLeaveWord = FALSE;
    m_bFirwall = FALSE;
    m_bRingPlayed = FALSE;  
    //  m_spSimAddr = boost::shared_ptr<Data::SimAddr>(new Data::SimAddr);

    m_spContactInfo = boost::shared_ptr<Data::ContactInfo>();
    m_spSoundSegment = boost::shared_ptr<Data::SoundSegment>();

    m_bEnding = FALSE;
    m_bPlaying = FALSE;
    m_bRecording = FALSE;
    m_dwDataLength = 0;

    m_pOggCodec = new Util::Audio::OggCodec();
    pVoice = (short*)malloc(nVoiceLength * sizeof(short));
    InitializeCriticalSection(&csVoice);

    m_waveform.wFormatTag = WAVE_FORMAT_PCM;
    m_waveform.nChannels = CHANNELS;
    m_waveform.nSamplesPerSec = SAMPLESPERSEC;
    m_waveform.wBitsPerSample = BITSPERSAMPLE;
    m_waveform.nBlockAlign = m_waveform.nChannels * m_waveform.wBitsPerSample / 8;
    m_waveform.nAvgBytesPerSec = m_waveform.nBlockAlign * m_waveform.nSamplesPerSec;
    m_waveform.cbSize = 0;  

    //  m_pWaveHdr1 = reinterpret_cast<PWAVEHDR>(malloc(sizeof(WAVEHDR)));
    //  m_pWaveHdr2 = reinterpret_cast<PWAVEHDR>(malloc(sizeof(WAVEHDR)));
    //  m_pBuffer1=(PBYTE)malloc(INP_BUFFER_SIZE);
    //  m_pBuffer2=(PBYTE)malloc(INP_BUFFER_SIZE);
    //  
    //  if (!m_pBuffer1 || !m_pBuffer2)
    //  {
    //      if (m_pBuffer1) free(m_pBuffer1);
    //      if (m_pBuffer2) free(m_pBuffer2);
    //      Dprintf("buffer malloc error!\n");
    //  }

    for (int i = 0; i < WAVE_BUFFER_COUNT; ++i) {
        m_pBuffer[i] = (PBYTE)malloc(INP_BUFFER_SIZE);
        m_pWaveHdr[i] = reinterpret_cast<PWAVEHDR>(malloc(sizeof(WAVEHDR)));
    }

    m_hSABTD = NULL;
    m_hSABTD = LoadLibrary(SABTD_dir); 
    if (m_hSABTD == NULL) {
        SabtdInit_ = NULL;
        SabtdUnInit_ = NULL;
        SabtdSetParam_ = NULL;
        SabtdGetParam_ = NULL;
        SabtdReset_ = NULL;
        SabtdProcess_ = NULL;
        //return false;
    } else {
          SabtdInit_ = (SabtdInit)GetProcAddress(m_hSABTD, _T("SABTD_Init"));
          SabtdUnInit_ = (SabtdUnInit)GetProcAddress(m_hSABTD, _T("SABTD_UnInit"));
          SabtdSetParam_ = (SabtdSetParam)GetProcAddress(m_hSABTD, _T("SABTD_SetParam"));
          SabtdGetParam_ = (SabtdGetParam)GetProcAddress(m_hSABTD, _T("SABTD_GetParam"));
          SabtdReset_ = (SabtdReset)GetProcAddress(m_hSABTD, _T("SABTD_Reset"));
          SabtdProcess_ = (SabtdProcess)GetProcAddress(m_hSABTD, _T("SABTD_Process"));

        if (SabtdInit_ && SabtdUnInit_ && SabtdSetParam_ && SabtdGetParam_ && SabtdReset_ && SabtdProcess_) {
            int initResult = 0;
            initResult = SabtdInit_(&m_sabtd_struct, SAMPLESPERSEC, sa_licen_file);
            if ((initResult == -1000) || (initResult < 0 || m_sabtd_struct == NULL)) {
                Dprintf("SABTD_CEDLL Initialize error!\n");
                SabtdInit_ = NULL;
                SabtdUnInit_ = NULL;
                SabtdSetParam_ = NULL;
                SabtdGetParam_ = NULL;
                SabtdReset_ = NULL;
                SabtdProcess_ = NULL;
            } else {
                if (initResult == SARTC_NORMAL) {
                    SabtdSetParam_(m_sabtd_struct, SABTD_PARAMTYPE_ENERGY, 1.0);
                    SabtdSetParam_(m_sabtd_struct, SABTD_PARAMTYPE_FRONT_NOTTONE, 0);
                    SabtdSetParam_(m_sabtd_struct, SABTD_PARAMTYPE_BUSYTONE_NUM, 3);
                    SabtdSetParam_(m_sabtd_struct, SABTD_PARAMTYPE_SILENCE_LENGTH, 5);
                }
            }
        }
    }

    //((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->SetSoundRingVal(-500);
    ((CMultimediaPhoneDlg *)(theApp.m_pMainWnd))->phone_->OpenTelRing(FALSE);

    //  m_spSimAddr->Import("/NandFlash/resource/mobile.txt");

    return true;
}

LRESULT C3GVideoPhone::OnClickMJPG(WPARAM w, LPARAM l) {
    LRESULT result = 0;
    CMultimediaPhoneDlg* pMainDlg = ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd));
    switch (w) {
    case 0:
        //切换
        //          m_MJPGList.SetUnitIsDownStatus(w, TRUE);
        break;
    case 1:
        //挂机
        break;
    case 2:
        //开启视频
        //          m_MJPGList.SetUnitIsDownStatus(w, TRUE);
        break;
    case 3:
        //隐藏本地视频
        //          m_MJPGList.SetUnitIsDownStatus(w, TRUE);
        break;
    case 5:
        //录音
        OnButtonTelephoneRecord();
        break;
    case 6:
        //便签
        break;
    case 7:
        //通讯录
        pMainDlg->m_MJPGList.SetUnitBitmap(0, ".\\ADV\\MJPG\\k5\\common\\通讯录\\通讯录图标.bmp", "", TRUE);
        pMainDlg->m_pContactDlg->ShowWindow(SW_SHOW);
        break;
    case 8:
        //通话记录
        pMainDlg->m_MJPGList.SetUnitBitmap(0, ".\\ADV\\MJPG\\k5\\common\\通话记录\\通话记录图标.bmp", "", TRUE);
        pMainDlg->m_pContactInfoDlg->ShowWindow(SW_SHOW);
        break;
    case 9:
        //二次拨号
        break;
    case 50:
        //声音喇叭
        Mute();
        break;
    case 51:
        //音量按钮
    case 52:
    case 53:
    case 54:
    case 55:
    case 56:
    case 57:
    case 58:
    case 59:
    case 60:
        if (m_IsSound) {
            m_SoundSize = w - 50;
            ChangeVolume(w);
            pMainDlg->phone_->Volume(m_SoundSize);
        }
        break;
    case 1000:
        //返回
        ShowWindow(SW_HIDE);
        break;
    case 1001:
        //设置
        break;
    default:
        break;
    }
    return result;
}

void C3GVideoPhone::ChangeVolume(int w) {
    for (int i = 1; i <= 10; i++) {
        int Volume = i + 50;
        if (Volume <= w) {
            m_MJPGList.SetUnitIsDownStatus(Volume, TRUE);
        } else {
            m_MJPGList.SetUnitIsDownStatus(Volume, FALSE);
        }
    }
    m_MJPGList.Invalidate();
}

std::string C3GVideoPhone::GetSoundPath(void) {
    path = ((CMultimediaPhoneDlg *)(theApp.m_pMainWnd))->m_pSettingDlg->m_pSetting->soundPath();

    BOOL DetectDIR(TCHAR * sDir);
    if (path == ssStorageCardRecordPath) {
        BOOL sd = DetectDIR((LPTSTR)(LPCTSTR)csStarageCard);
        if (sd) {
            if (!DetectDIR((LPTSTR)(LPCTSTR)csStorageCardRecordPath)) {
                CreateDirectory((LPTSTR)(LPCTSTR)csStorageCardRecordPath, NULL);
            }
        } else {
            path = ssFlashRecordPath;
            if (!DetectDIR((LPTSTR)(LPCTSTR)csFlashRecordPath)) {
                CreateDirectory((LPTSTR)(LPCTSTR)csFlashRecordPath, NULL);
            }
        }
    }
    return path;
}

void C3GVideoPhone::OnButtonTelephoneRecord() {
    //  CString caption;
    //  m_btnRecord.GetWindowText(caption);
    //  if (caption == m_strRecord)
    if (!m_bRecording) {
        GetSoundPath();

        ULARGE_INTEGER freeBytes;
        ULARGE_INTEGER totalBytes;
        GetDiskFreeSpaceEx(Util::StringOp::ToCString(path), &freeBytes, &totalBytes, NULL);

        int secondBytes = SECONDBYTES8;
        if (((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->m_pSettingDlg->m_pSetting->isDeleteProtect()) {
            secondBytes = SECONDBYTES8;
        } else {
            secondBytes = SECONDBYTES2;
        }

        if (path == ssFlashRecordPath) {
            if (freeBytes.QuadPart < totalBytes.QuadPart * 0.1)
                            //if (totalBytes.QuadPart < MINFREEBYTES)
            {
                ((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->m_pWarningNoFlashDlg->ShowWindow_(SW_SHOW);
                return;
            }

            //m_uiRemainRecordSecond = (totalBytes.QuadPart - MINFREEBYTES) / SECONDBYTES + 1;
            m_uiRemainRecordSecond = static_cast<UINT>((freeBytes.QuadPart - (totalBytes.QuadPart * 0.1)) / secondBytes + 1);
        } else {
            if (freeBytes.QuadPart < secondBytes * 2) {
                ((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->m_pWarningNoFlashDlg->ShowWindow_(SW_SHOW);
                return;
            }
              m_uiRemainRecordSecond = static_cast<UINT>(freeBytes.QuadPart / secondBytes + 1);
        }

        if (RecStart()) {
            Dprintf("RecStart Ok\r\n");

            m_spSoundSegment = boost::shared_ptr<Data::SoundSegment>(new Data::SoundSegment);
            m_vSoundSegment.push_back(m_spSoundSegment);

            CTime t;
            t = CTime::GetCurrentTime();
            CString filename;

            if (((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->m_pSettingDlg->m_pSetting->isDeleteProtect()) {
                m_pOggCodec->SetQuality(8);
                filename.Format(_T("%02d%02d%02dHQ.spx"), t.GetHour(), t.GetMinute(), t.GetSecond());
            } else {
                  m_pOggCodec->SetQuality(2);
                  filename.Format(_T("%02d%02d%02dST.spx"), t.GetHour(), t.GetMinute(), t.GetSecond());
            }

            m_sRecordFileName = path + Util::StringOp::FromCString(filename);
            m_spSoundSegment->filename(m_sRecordFileName);
            m_spSoundSegment->startTime(CTime::GetCurrentTime());
            m_spSoundSegment->contactInfoId(m_spContactInfo->id());
            m_spSoundSegment->telephoneNumber(m_spContactInfo->telephoneNumber());
            m_spSoundSegment->name(m_spContactInfo->name());
            if (m_spContactInfo->type() == Data::citInNo) {
                m_spSoundSegment->type(Data::sstLeaveWord);
            } else {
                m_spSoundSegment->type(Data::sstTeleRecord);
            }
            m_spSoundSegment->Insert();
            m_spContactInfo->isSound(true);
            m_spContactInfo->Update();

            Dprintf("m_pUtil::Audio::OggCodec->StartEncode 1\r\n");
            std::string comment = m_spSoundSegment->name() + "\'" + m_spSoundSegment->telephoneNumber().number() + "\'" + Util::StringOp::FromInt(m_spSoundSegment->type()) + "\'" + Util::StringOp::FromTimestamp(m_spSoundSegment->startTime()) + "\'";
            if (m_pOggCodec->StartEncode(m_sRecordFileName, comment)) {
                Dprintf("m_pUtil::Audio::OggCodec->StartEncode 2\r\n");
                m_uiRecordSecond = 0;
                SetTimer(m_uiRecordTimer, 1000, 0);          //lxz test     
                //m_btnRecordTip.ShowWindow(TRUE);
                //m_sticRecordCaption.ShowWindow(TRUE);
                m_MJPGList.SetUnitIsShow(104, TRUE);
                m_MJPGList.SetUnitIsShow(106, TRUE);
                //m_sticRecord.ShowWindow(TRUE);
                //m_btnRecord.SetWindowText(m_strStopRecord);

                if (((CMultimediaPhoneDlg *)(theApp.m_pMainWnd))->m_pSettingDlg->m_pSetting->isMustRecord()) {
                    m_MJPGList.SetUnitIsShow(2, FALSE);
                    m_MJPGList.SetUnitIsShow(8, FALSE);
                } else {
                    m_MJPGList.SetUnitIsShow(2, FALSE);
                    m_MJPGList.SetUnitIsShow(8, TRUE);
                }
            }
        }
    } else {
        if (RecStop()) {
            KillTimer(m_uiRecordTimer);
            //          m_btnRecordTip.ShowWindow(FALSE);
            //          m_sticRecordCaption.ShowWindow(FALSE);
            //          m_sticRecord.ShowWindow(FALSE);
            //          m_sticRecord.SetWindowText(_T(""));
            //          m_btnRecord.SetWindowText(m_strRecord);

            if (m_uiRecordSecond == 0) {
                m_uiRecordSecond = 1;
            }
            Dprintf("record second = %d\n", m_uiRecordSecond);
            m_spSoundSegment->duration(m_uiRecordSecond);
            m_spSoundSegment->Update();
            //          m_spSoundSegment = boost::shared_ptr<Data::SoundSegment>();
            m_uiRecordSecond = 0;
        }
    }
}

bool C3GVideoPhone::RecStart(void) {
    // TODO: Add your control notification handler code here

    //  SaveWavHead();
    //  
    //  if (m_fWav.Open(LPCTSTR(Util::StringOp::ToCString(m_sRecordFileName)), CFile::modeWrite) == 0)
    //      return false;
    //  
    //  m_fWav.SeekToEnd();
    //  Dprintf("SaveWavHead\n");

    /////////////////////////////////////////////////////////////////////////////////////////////////////
    LRESULT l = waveInOpen(&m_hWaveIn, WAVE_MAPPER, &m_waveform, (DWORD)this->m_hWnd, NULL, CALLBACK_WINDOW);
    if (l) {
        Dprintf("waveInOpen error\r\n");
        //      m_fWav.Close();
        return false;
    }
    waveInMessage(m_hWaveIn, WAV_LINEIN_MIC, 0, 0);


    //  m_pWaveHdr1->lpData=(char*)m_pBuffer1;
    //  m_pWaveHdr1->dwBufferLength=INP_BUFFER_SIZE;
    //  m_pWaveHdr1->dwBytesRecorded=0;
    //  m_pWaveHdr1->dwUser=0;
    //  m_pWaveHdr1->dwFlags=0;
    //  m_pWaveHdr1->dwLoops=1;
    //  m_pWaveHdr1->lpNext=NULL;
    //  m_pWaveHdr1->reserved=0;
    //  
    //  MMRESULT r = waveInPrepareHeader(m_hWaveIn,m_pWaveHdr1,sizeof(WAVEHDR));
    //  
    //  m_pWaveHdr2->lpData=(char*)m_pBuffer2;
    //  m_pWaveHdr2->dwBufferLength=INP_BUFFER_SIZE;
    //  m_pWaveHdr2->dwBytesRecorded=0;
    //  m_pWaveHdr2->dwUser=0;
    //  m_pWaveHdr2->dwFlags=0;
    //  m_pWaveHdr2->dwLoops=1;
    //  m_pWaveHdr2->lpNext=NULL;
    //  m_pWaveHdr2->reserved=0;
    //  
    //  r = waveInPrepareHeader(m_hWaveIn,m_pWaveHdr2,sizeof(WAVEHDR));
    //      
    //  waveInAddBuffer (m_hWaveIn, m_pWaveHdr1, sizeof (WAVEHDR)) ;
    //  waveInAddBuffer (m_hWaveIn, m_pWaveHdr2, sizeof (WAVEHDR)) ;

    for (int i = 0; i < WAVE_BUFFER_COUNT; ++i) {
        m_pWaveHdr[i]->lpData = (char*)m_pBuffer[i];
        m_pWaveHdr[i]->dwBufferLength = INP_BUFFER_SIZE;
        m_pWaveHdr[i]->dwBytesRecorded = 0;
        m_pWaveHdr[i]->dwUser = 0;
        m_pWaveHdr[i]->dwFlags = 0;
        m_pWaveHdr[i]->dwLoops = 1;
        m_pWaveHdr[i]->lpNext = NULL;
        m_pWaveHdr[i]->reserved = 0;

        waveInPrepareHeader(m_hWaveIn, m_pWaveHdr[i], sizeof(WAVEHDR));
        waveInAddBuffer(m_hWaveIn, m_pWaveHdr[i], sizeof(WAVEHDR));
    }

    // Begin sampling

    m_bEnding = FALSE ;
    m_bRecording = TRUE;
    m_dwDataLength = 0 ;
    waveInStart(m_hWaveIn) ;

    Dprintf("RecStart\n");
    return true;
}

bool C3GVideoPhone::RecStop(void) {
    m_bEnding = TRUE;       
    waveInReset(m_hWaveIn);
    //  waveInClose(m_hWaveIn);

    return true;
}

void C3GVideoPhone::Mute(void) {
    CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
    if (m_IsSound) {
        ChangeVolume(0);
        main->phone_->Mute(TRUE);
        m_MJPGList.SetUnitIsDownStatus(50, TRUE);
        m_MJPGList.SetUnitIsShow(50, TRUE);
    } else {
        ChangeVolume(m_SoundSize + 50);
        main->phone_->Volume(m_SoundSize);
        m_MJPGList.SetUnitIsDownStatus(50, FALSE);
        m_MJPGList.SetUnitIsShow(50, TRUE);
    }
    m_IsSound = !m_IsSound;
}