// MainMp3Dlg.cpp : implementation file
//
#include "stdafx.h"
#include "MainMp3Dlg.h"
#include "../Data/LanguageResource.h"
#include "../Data/SkinStyle.h"
#include "../MultimediaPhoneDlg.h"
#include "../MultimediaPhone.h"
#include "player.h"
#include "../Pblmember.h"
#include <Afxctl.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static int gMp3PuaseCount = 0;
size_t gPlayIndex = 0;
/////////////////////////////////////////////////////////////////////////////
// CMainMp3Dlg dialog

CMainMp3Dlg::CMainMp3Dlg(CWnd* pParent /*=NULL*/)
: CDialog(CMainMp3Dlg::IDD, pParent) {
    //{{AFX_DATA_INIT(CMainMp3Dlg)
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
    memset(m_chDir, 0, 128 * 2);
    m_ClickType = 0;
    m_IsPlay = 0;
    m_IsSound = TRUE;
    m_Volume = SOUND_DEFAULT_VOLUME;
    m_bIsPausebyEvent = FALSE;
    m_pageSize = 5;
    m_PreOrBack = 0;
}


void CMainMp3Dlg::DoDataExchange(CDataExchange* pDX) {
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CMainMp3Dlg)
    // NOTE: the ClassWizard will add DDX and DDV calls here
    //}}AFX_DATA_MAP

}


BEGIN_MESSAGE_MAP(CMainMp3Dlg, CDialog)
    //{{AFX_MSG_MAP(CMainMp3Dlg)
    ON_BN_CLICKED(IDC_BUTTON_MP3EXIT, OnExit)
    ON_BN_CLICKED(IDC_BUTTON_MP3PLAY, OnPlayer)
    //  ON_BN_CLICKED(IDC_BUTTON_MP3STOP, OnStop)
    ON_BN_CLICKED(IDC_BUTTON_MP3PRE, OnPre)
    ON_BN_CLICKED(IDC_BUTTON_MP3BACK, OnBack)
    ON_BN_CLICKED(IDC_BUTTON_MP3OPEN, OnOpenFile)
    ON_BN_CLICKED(IDC_BUTTON_MP3MUTE, OnMute)
    ON_MESSAGE(WM_PROCESS_POS, OnPregress)
    ON_WM_TIMER()
    ON_WM_ACTIVATE()
    ON_MESSAGE(WM_OUTEVENT, OnOutEvent)
    ON_MESSAGE(WM_MJPGTOGGLE, OnClickMJPG)
    ON_MESSAGE(WM_CLICKMJPG_TOAPP, OnClickMJPG)
    ON_WM_KEYDOWN()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CMainMp3Dlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
    if (nChar == 'U') {
        OnPre();
    } else if (nChar == 'D') {
        OnBack();
    }
}

void CMainMp3Dlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) {
    if (nState & WA_ACTIVE) {
    }
}


BOOL CMainMp3Dlg::OnInitDialog() {
    CDialog::OnInitDialog();

    // TODO: Add extra initialization here

    m_MoveText = new CCEMoveTxtStatic();
    m_MoveText->Create(L"", RGB(0, 0, 0), WS_CHILD | WS_VISIBLE, CRect(36, 22, 474, 52), this);
    m_MoveText->SetTxt(L"", 22, RGB(0, 0, 0));
    m_MoveText->MoveWindow(CRect(36, 22, 474, 52));

    m_prgPlayTime.Create(WS_CHILD | WS_VISIBLE, CRect(38, 327, 474, 327 + 9), this, IDC_PROGRESS_MP3TIME);
    m_prgPlayTime.SetParam(0, 0, 10, 1, 1, IDB_PROGRESS_TIME1, IDB_PROGRESS_TIME2);
    m_prgPlayTime.SetPos(0, FALSE);
    m_prgPlayTime.SetBackRGB(Data::g_allFramBackRGB[Data::g_skinstyle]);

    //播放器
    CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
    playerDlg_ = new CPlayerDlg(main->playeraudio_);
    playerDlg_->Create(CPlayerDlg::IDD, this);
    playerDlg_->ReSetWindowsRect(CRect(36, 52, 474, 320));

    //wangzhenxing0922
    m_MJPGList.Create(L"", WS_VISIBLE | WS_CHILD, CRect(0, 0, 800, 423), this);
    m_MJPGList.SetCurrentLinkFile(".\\adv\\mjpg\\k5\\中文\\音乐播放.xml");

    MoveWindow(0, 57, 800, 423);
    m_MJPGList.SetUnitColor(40, font_white, TRUE);
    m_MJPGList.SetUnitColor(41, font_white, TRUE);
    m_MJPGList.SetUnitFont(40, font_16);
    m_MJPGList.SetUnitFont(41, font_16);
    for (int i = 21; i <= 25; i++) {
        m_MJPGList.SetUnitFont(i, font_18);
    }
    m_MJPGList.SetUnitIsShow(50, TRUE);
    ChangeVolume(m_Volume + 50);
    ((CMultimediaPhoneDlg *)(theApp.m_pMainWnd))->playeraudio_->SetVolume(m_Volume);

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CMainMp3Dlg::SetMP3(CString filename) {
    char filename_[128] = {
        0
    };
    std::string str = Util::StringOp::FromCString(filename);
    sprintf_s(filename_, 128, "%s", str.c_str());
    playerDlg_->SetParam(filename_, mtAudio);
    playerDlg_->ReSetWindowsRect(CRect(0, 0, 0, 0));

    if ((gPlayIndex > 0) && (0 == gPlayIndex % m_pageSize)) {
        m_MJPGList.SetUnitColor(25, font_blue, TRUE);
    } else {
          m_MJPGList.SetUnitColor((gPlayIndex % m_pageSize) + 20, font_blue, TRUE);
    }
}

void CMainMp3Dlg::OnExit_(BOOL isStopMusic) {
    CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
    if (isStopMusic) {
        main->playeraudio_->ExitPlayer();
        KillTimer(IDT_GETINFO_TIIMER);
        m_MoveText->KillTimer(0xFFF0);
        m_MoveText->m_nCount = 0;
        m_MoveText->SetTxt(L"", 22, RGB(255, 255, 255));
        //wangzhenxing0923
        playerDlg_->ShowWindow(SW_HIDE);
        m_bIsPausebyEvent = FALSE;
        m_IsPlay = 0;
        gPlayIndex = 0;
        m_prgPlayTime.SetPos(0);
        m_MJPGList.SetUnitIsDownStatus(3, FALSE);
        m_MJPGList.SetUnitIsShow(3, TRUE);
        ClearAll();
    }
    if (IsWindowVisible()) {
        GetParent()->SendMessage(WM_CHANGEWINDOW, (WPARAM)this, (LPARAM)SW_HIDE);
    }
}

void CMainMp3Dlg::OnExit() {
    OnExit_(TRUE);
}

//播放
void CMainMp3Dlg::OnPlayer() {
    if (((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->m_pSoundDlg->m_pRecordSoundDlg->m_bISRecording || ((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->m_pTelephoneDlg->m_bRecording) {
        return;
    }

    if (((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->m_pTelephoneDlg->GetIsRecordStatus()) {
        return;
    }

    if (m_IsPlay == 0) {
        char* buffer = new char[1024 * 10];
        memset(buffer, 0, 1024 * 10);
        int nFileSelected = m_MP3List.size();

        char txt[128] = {
            0
        };
        strcpy_s(txt, 128, "[Playlist]\r\n");
        strcpy_s(buffer, 1024 * 10, txt);
        for (size_t i = 0; i < m_MP3List.size(); ++i) {
            std::string str = Util::StringOp::FromCString(m_MP3List[i]);
            sprintf_s(txt, 128, "File%d=%s\r\nLength%d=255\r\n", i, str.c_str(), i);
            if (strlen(buffer) < (1024 * 8)) {
                strcat_s(buffer, 1024 * 10, txt);
            }
        }
        sprintf_s(txt, 128, "NumberOfEntries=%d\r\nVersion=2\r\n", nFileSelected);
        strcat_s(buffer, 1024 * 10, txt);

        if (nFileSelected > 0) {
            FILE* file = 0;
            fopen_s(&file, playlist_pls_file, "w+b");
            if (file) {
                fwrite(buffer, sizeof(char), strlen(buffer), file);
                fclose(file);
            }
            //调用播放窗口
            if (playerDlg_) {
                m_MoveText->KillTimer(0xFFF0);
                m_MoveText->SetTxt(L"", 22, RGB(255, 255, 255));
                m_MoveText->m_nCount = 0;
                SetMP3(m_MP3List[gPlayIndex - 1]);
                playerDlg_->Show();
                m_MoveText->SetTxt(m_ShowList[gPlayIndex - 1], 22, RGB(255, 255, 255));
                m_MoveText->SetTimer(0xFFF0, 100, NULL);
                SetTimer(IDT_GETINFO_TIIMER, 1000, NULL);
            }
            delete[]buffer;
            m_IsPlay = 1;
            m_MJPGList.SetUnitIsDownStatus(3, TRUE);
            m_MJPGList.SetUnitIsShow(3, TRUE);
        }
    } else if (m_IsPlay == 1) {
        m_MJPGList.SetUnitIsDownStatus(3, FALSE);
        m_MJPGList.SetUnitIsShow(3, TRUE);
        CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
        main->playeraudio_->PausePlayer(TRUE);
        m_IsPlay = 2;
        KillTimer(IDT_GETINFO_TIIMER);
    } else if (m_IsPlay == 2) {
        if (m_bIsPausebyEvent) {
            Dprintf("Mp3 resume!\r\n");

            CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
            m_bIsPausebyEvent = FALSE;
            main->playeraudio_->SetOwner(playerDlg_);
            main->playeraudio_->InitPlayer();

            main->playeraudio_->isActiveMode_ = 1;
            main->playeraudio_->ResumePlayer();
            gMp3PuaseCount = 0;
        }

          m_MJPGList.SetUnitIsDownStatus(3, TRUE);
          m_MJPGList.SetUnitIsShow(3, TRUE);
        CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
          main->playeraudio_->PausePlayer(FALSE);
          m_IsPlay = 1;
          SetTimer(IDT_GETINFO_TIIMER, 1000, NULL);
          SetTimer(0x100, 200, NULL);
    }
}

void CMainMp3Dlg::OnStop() {
    CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;

    if (1 == m_IsPlay || 2 == m_IsPlay) {
        m_prgPlayTime.SetPos(0);
        m_IsPlay = 0;
        m_bIsPausebyEvent = TRUE;
        m_MJPGList.SetUnitIsDownStatus(3, FALSE);
        m_MJPGList.SetUnitIsShow(3, TRUE);
        m_MJPGList.SetUnitText(31, L"00:00:00", TRUE);
        main->playeraudio_->StopPlayer();
        KillTimer(IDT_GETINFO_TIIMER);
        m_MoveText->KillTimer(0xFFF0);
        m_MoveText->SetTxt(L"", 22, RGB(255, 255, 255));
        m_MoveText->m_nCount = 0;
    }
}

//前一条
void CMainMp3Dlg::OnPre() {
    if (((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->m_pSoundDlg->m_pRecordSoundDlg->m_bISRecording || ((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->m_pTelephoneDlg->m_bRecording) {
        return;
    }
    //wangzhenxing1013

    KillTimer(IDT_GETINFO_TIIMER);
    m_MoveText->KillTimer(0xFFF0);
    m_MoveText->SetTxt(L"", 22, RGB(255, 255, 255));
    m_MoveText->m_nCount = 0;

    m_MJPGList.SetUnitColor(((gPlayIndex) % m_pageSize) + 20, font_red, TRUE);
    m_MJPGList.SetUnitText(((gPlayIndex) % m_pageSize) + 20, m_ShowList[gPlayIndex - 1], TRUE);

    gPlayIndex--;

    if (0 == gPlayIndex) {
        OnLast();
    } else if (0 == gPlayIndex % m_pageSize) {
        PageUp();
    }
    SetMP3(m_MP3List[gPlayIndex - 1]);
    playerDlg_->Show();
    m_IsPlay = 1;
    m_MoveText->SetTxt(m_ShowList[gPlayIndex - 1], 22, RGB(255, 255, 255));
    SetTimer(IDT_GETINFO_TIIMER, 1000, NULL);
    m_MoveText->SetTimer(0xFFF0, 100, NULL);
}
//后一条
void CMainMp3Dlg::OnBack() {
    if (((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->m_pSoundDlg->m_pRecordSoundDlg->m_bISRecording || ((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->m_pTelephoneDlg->m_bRecording) {
        return;
    }
    //wangzhenxing1013
    KillTimer(IDT_GETINFO_TIIMER);
    m_MoveText->KillTimer(0xFFF0);
    m_MoveText->SetTxt(L"", 22, RGB(255, 255, 255));
    m_MoveText->m_nCount = 0;

    m_MJPGList.SetUnitColor(((gPlayIndex) % m_pageSize) + 20, font_red, TRUE);
    m_MJPGList.SetUnitText(((gPlayIndex) % m_pageSize) + 20, m_ShowList[gPlayIndex - 1], TRUE);
    gPlayIndex++;

    if ((gPlayIndex > 1) && (1 == gPlayIndex % m_pageSize)) {
        PageDown();
    }
    if (m_MP3List.size() < gPlayIndex) {
        OnFirst();
    }

    SetMP3(m_MP3List[gPlayIndex - 1]);
    playerDlg_->Show();
    m_IsPlay = 1;
    m_MoveText->SetTxt(m_ShowList[gPlayIndex - 1], 22, RGB(255, 255, 255));
    SetTimer(IDT_GETINFO_TIIMER, 1000, NULL);
    m_MoveText->SetTimer(0xFFF0, 100, NULL);
}

void CMainMp3Dlg::OnFirst() {
    CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
    KillTimer(IDT_GETINFO_TIIMER);
    m_MoveText->KillTimer(0xFFF0);
    m_MoveText->SetTxt(L"", 22, RGB(255, 255, 255));
    m_MoveText->m_nCount = 0;

    main->playeraudio_->First();
    m_selectCurrentPage = 1;
    CString str;
    str.Format(L"%d", m_selectCurrentPage);
    m_MJPGList.SetUnitText(40, str, TRUE);

    gPlayIndex = 1;
    m_IsPlay = 1;
    m_MoveText->SetTxt(m_ShowList[gPlayIndex - 1], 22, RGB(255, 255, 255));
    m_MJPGList.SetUnitIsDisable(9, TRUE);
    if (m_selectCurrentPage < m_selectPageCount) {
        m_MJPGList.SetUnitIsDisable(10, FALSE);
    }
    ShowArrayInList(m_ShowList);
    SetTimer(IDT_GETINFO_TIIMER, 1000, NULL);
    m_MoveText->SetTimer(0xFFF0, 100, NULL);
}

void CMainMp3Dlg::OnLast() {
    CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;

    KillTimer(IDT_GETINFO_TIIMER);
    m_MoveText->KillTimer(0xFFF0);
    m_MoveText->SetTxt(L"", 22, RGB(255, 255, 255));
    m_MoveText->m_nCount = 0;

    main->playeraudio_->Last();
    m_selectCurrentPage = m_selectPageCount;
    CString str;
    str.Format(L"%d", m_selectCurrentPage);
    m_MJPGList.SetUnitText(40, str, TRUE);

    gPlayIndex = m_MP3List.size();
    m_IsPlay = 1;
    m_MoveText->SetTxt(m_ShowList[gPlayIndex - 1], 22, RGB(255, 255, 255));
    m_MJPGList.SetUnitIsDisable(10, TRUE);
    if (m_selectCurrentPage > 1) {
        m_MJPGList.SetUnitIsDisable(9, FALSE);
    }
    ShowArrayInList(m_ShowList);
    SetTimer(IDT_GETINFO_TIIMER, 1000, NULL);
    m_MoveText->SetTimer(0xFFF0, 100, NULL);
}

//打开播放列表
void CMainMp3Dlg::OnOpenFile() {
    CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;

    m_bIsPausebyEvent = FALSE;
    main->playeraudio_->ExitPlayer();
    m_IsPlay = 0;
    KillTimer(IDT_GETINFO_TIIMER);
    m_prgPlayTime.SetPos(0);
}

//设置声音开关
void CMainMp3Dlg::OnMute() {
    CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
    if (m_IsSound) {
        ChangeVolume(0);
        main->playeraudio_->SetVolume(0);
        m_MJPGList.SetUnitIsDownStatus(50, TRUE);
        m_MJPGList.SetUnitIsShow(50, TRUE);
    } else {
        ChangeVolume(m_Volume + 50);
        main->playeraudio_->SetVolume(m_Volume);
        m_MJPGList.SetUnitIsDownStatus(50, FALSE);
        m_MJPGList.SetUnitIsShow(50, TRUE);
    }
    m_IsSound = !m_IsSound;
}

//
void CMainMp3Dlg::SetVolume() {
    CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
    int index = main->m_pSettingDlg->m_pSetting->sysVolume();
    int Volume[] = {
        10, 8, 6, 4, 2
    };
    main->playeraudio_->SetVolume(Volume[index]);
    m_Volume = Volume[index];
}

void CMainMp3Dlg::ChangeVolume(int w) {
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

LRESULT CMainMp3Dlg::OnPregress(WPARAM w, LPARAM l) {
    LRESULT result = 0;
    CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
    //设置声音
    if (l == IDC_PROGRESS_MP3SOUND) {
        Dprintf("SetSound %d \n", w);
        if (m_IsSound) {
            main->playeraudio_->SetVolume(w);
        }
        m_Volume = w;
    }
    //调整播放时间
    else if (l == IDC_PROGRESS_MP3TIME) {
        int percent = m_prgPlayTime.GetPercent();
        main->playeraudio_->PlayPos(percent);
    }
    return result;
}

/////////////////////////////////////////////////////////////////////////////
// CMainMp3Dlg message handlers
void CMainMp3Dlg::OnTimer(UINT nIDEvent) {
    // TODO: Add your message handler code here and/or call default
    if (0x10 == nIDEvent) {
        KillTimer(0x10);
        for (int i = 21; i <= 25; i++) {
            if (m_MJPGList.GetUnitIsDownStatus(i)) {
                m_MJPGList.SetUnitIsDownStatus(i, FALSE);
                m_MJPGList.SetUnitIsShow(i, TRUE);
            }
        }
    }

    if (IDT_GETINFO_TIIMER == nIDEvent && m_MJPGList.GetUnitIsDownStatus(3)) {
        CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
        MEDIA_INFO info;
        main->playeraudio_->GetVideoFileInfo(&info);

        int Hour, Min, Sec;
        int tick = info.nTick;
        Hour = tick / 3600 / TICKSPERSEC;
        tick -= Hour * 3600 * TICKSPERSEC;
        Min = tick / 60 / TICKSPERSEC;
        tick -= Min * 60 * TICKSPERSEC;
        Sec = tick / TICKSPERSEC;

        int Hour1, Min1, Sec1;
        tick = info.nDuration;
        Hour1 = tick / 3600 / TICKSPERSEC;
        tick -= Hour1 * 3600 * TICKSPERSEC;
        Min1 = tick / 60 / TICKSPERSEC;
        tick -= Min1 * 60 * TICKSPERSEC;
        Sec1 = tick / TICKSPERSEC;

        int n1 = Hour*3600 + Min*60 + Sec;
        int n2 = Hour1*3600 + Min1*60 + Sec1;

        m_prgPlayTime.SetParam(0, 0, n2, 1, 1, IDB_PROGRESS_TIME1, IDB_PROGRESS_TIME2);
        m_prgPlayTime.SetPos(n1);

        char txt[32] = {
            0
        };
        sprintf_s(txt, 32, "%02d:%02d:%02d", Hour, Min, Sec);
        CString s = txt;
        m_MJPGList.SetUnitText(31, s, TRUE);
        sprintf_s(txt, 32, "%02d:%02d:%02d", Hour1, Min1, Sec1);
        s = txt;
        m_MJPGList.SetUnitText(30, s, TRUE);

        if (((n1 >= (n2 - 3)) && n1 != 0) || n2 == 0)       //换到下一首
        {
            if (main->playeraudio_->isPlaying_) {
                m_MJPGList.SetUnitColor(((gPlayIndex) % m_pageSize) + 20, font_red, TRUE);
                m_MJPGList.SetUnitText(((gPlayIndex) % m_pageSize) + 20, m_ShowList[gPlayIndex - 1], TRUE);
                gPlayIndex++;

                if ((0 == gPlayIndex % m_pageSize) && (gPlayIndex > 1)) {
                    PageDown();
                }
                if (gPlayIndex > m_MP3List.size()) {
                    gPlayIndex = 1;
                    if (m_selectCurrentPage == m_selectPageCount) {
                        OnFirst();
                    }
                }
                std::string s = Util::StringOp::FromCString(m_MP3List[gPlayIndex - 1]);
                char filename_[128] = {
                    0
                };
                sprintf_s(filename_, 128, "%s", s.c_str());
                SetMP3(filename_);
                playerDlg_->Show();
                m_MoveText->m_nCount = 0;
                m_MoveText->SetTxt(m_ShowList[gPlayIndex - 1], 22, RGB(255, 255, 255));
            }
        }
    } else if (nIDEvent == 123) {
        int nn = rand();
        nn = nn % 12;
        Dprintf("SetSound %d \n", nn);
        CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
        main->playeraudio_->SetVolume(nn);
        m_Volume = nn;
    } else if (nIDEvent == 1001)   //恢复播放
    {
          gMp3PuaseCount--;
        CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
          Dprintf("main->phone_->m_bStartRing %d\r\n", main->phone_->m_bStartRing);
          KillTimer(1001);
        if (m_bIsPausebyEvent && !main->phone_->m_bStartRing && !main->m_pSoundDlg->m_pRecordSoundDlg->m_bISRecording && !((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->m_pTelephoneDlg->m_bRecording/* && (gMp3PuaseCount<=0)*/) {
            gMp3PuaseCount = 0;
            Dprintf("Mp3 resume!\r\n");
            m_bIsPausebyEvent = FALSE;
            main->playeraudio_->SetOwner(playerDlg_);
            main->playeraudio_->InitPlayer();

            main->playeraudio_->isActiveMode_ = 1;
            main->playeraudio_->ResumePlayer();

            if (m_IsPlay == 2) {
                main->playeraudio_->PausePlayer(TRUE);
            } else {
                SetTimer(IDT_GETINFO_TIIMER, 1000, NULL);
            }
        }
    } else if (nIDEvent == 1002)    //暂停播放
    {
        Dprintf("Mp3 stop\r\n");
        if (gMp3PuaseCount < 0) {
            gMp3PuaseCount = 0;
        }
        gMp3PuaseCount++;
        KillTimer(1002);
        CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
        if (main->playeraudio_->isPlaying_) {
            KillTimer(IDT_GETINFO_TIIMER);
            main->playeraudio_->ExitPlayer(TRUE);
            m_bIsPausebyEvent = TRUE;
        }
    }
    CDialog::OnTimer(nIDEvent);
}

LRESULT CMainMp3Dlg::OnOutEvent(WPARAM w, LPARAM l) {
    LRESULT result = 0;
    CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;

    if (l == 0)  //暂停播放
    {
        KillTimer(1002);
        SetTimer(1002, 10, NULL);
    } else if (l == 1) //恢复播放
    {
        KillTimer(1001);

        SetTimer(1001, 5, NULL);
    }
    return result;
}

LRESULT CMainMp3Dlg::OnClickMJPG(WPARAM w, LPARAM l) {
    LRESULT result = 0;
    CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
    static unsigned int firstClick = 0;
    static unsigned int lastClick = 0;
    CString strFileName;
    switch (w) {
    case 7:
        //全屏
        break;
    case 1:
        //第一首
        m_MJPGList.SetUnitIsDownStatus(3, FALSE);
        m_MJPGList.SetUnitIsShow(3, TRUE);
        OnFirst();
        m_MJPGList.SetUnitIsDownStatus(3, TRUE);
        m_MJPGList.SetUnitIsShow(3, TRUE);
        break;
    case 2:
        //上一首
        m_MJPGList.SetUnitIsDownStatus(3, FALSE);
        m_MJPGList.SetUnitIsShow(3, TRUE);
        OnPre();
        m_MJPGList.SetUnitIsDownStatus(3, TRUE);
        m_MJPGList.SetUnitIsShow(3, TRUE);
        break;
    case 3:
        //播放
        OnPlayer();
        break;
    case 4:
        //停止
        OnStop();
        break;
    case 5:
        //下一首
        m_MJPGList.SetUnitIsDownStatus(3, FALSE);
        m_MJPGList.SetUnitIsShow(3, TRUE);
        OnBack();
        m_MJPGList.SetUnitIsDownStatus(3, TRUE);
        m_MJPGList.SetUnitIsShow(3, TRUE);
        break;
    case 6:
        //最后一首
        m_MJPGList.SetUnitIsDownStatus(3, FALSE);
        m_MJPGList.SetUnitIsShow(3, TRUE);
        OnLast();
        m_MJPGList.SetUnitIsDownStatus(3, TRUE);
        m_MJPGList.SetUnitIsShow(3, TRUE);
        break;
    case 9:
        //上翻
        PageUp();
        break;
    case 10:
        //下翻
        PageDown();
        break;
    case 21:
    case 22:
    case 23:
    case 24:
    case 25:
        if (m_ClickType == w) {
            lastClick = GetTickCount();
            Dprintf("lastClick %d\r\n", lastClick);
            if ((lastClick - firstClick) < 1000) {
                OnDBClickShowList(w);
            } else {
                firstClick = GetTickCount();
            }
            Dprintf("lastClick off %d\r\n", lastClick - firstClick);
        } else {
            m_ClickType = w;
            firstClick = GetTickCount();
            Dprintf("firstClick %d\r\n", firstClick);
            OnClickShowList(w);
        }
        break;
    case 50:
        OnMute();
        break;
    case 51:
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
            m_Volume = w - 50;
            ChangeVolume(w);
            ((CMultimediaPhoneDlg *)(theApp.m_pMainWnd))->playeraudio_->SetVolume(m_Volume);
        }
        break;
    case 100:
        //进度条
        break;
    case 1000:
        //返回
        OnExit();
        break;
    }
    return result;
}

void CMainMp3Dlg::CalculatePage(int dataCount) {
    if (0 == dataCount) {
        m_selectCurrentPage = 0;
    } else {
        m_selectCurrentPage = 1;
    }

    if (0 == dataCount % m_pageSize) {
        m_selectPageCount = dataCount / m_pageSize;
    } else {
        m_selectPageCount = dataCount / m_pageSize + 1;
    }

    CString str;
    str.Format(L"/%d", m_selectPageCount);
    m_MJPGList.SetUnitText(41, str, TRUE);
    str.Empty();
    str.Format(L"%d", m_selectCurrentPage);
    m_MJPGList.SetUnitText(40, str, TRUE);

    m_MJPGList.SetUnitIsDisable(9, TRUE);
    if (m_selectPageCount <= 1)     //当总数小于6时设置下翻按钮不可用
    {
        m_MJPGList.SetUnitIsDisable(10, TRUE);
    } else {
        m_MJPGList.SetUnitIsDisable(10, FALSE);
    }
}

void CMainMp3Dlg::ShowArrayInList(std::vector<CString> fileName) {
    ClearAll();
    if (gPlayIndex <= m_selectCurrentPage * m_pageSize && gPlayIndex >= (m_selectCurrentPage - 1) * m_pageSize) {
        if ((gPlayIndex > 0) && (0 == gPlayIndex % m_pageSize)) {
            m_MJPGList.SetUnitColor(25, font_blue, FALSE);
        } else {
              m_MJPGList.SetUnitColor((gPlayIndex % m_pageSize) + 20, font_blue, FALSE);
        }
    }
    if (fileName.size() > 0) {
        int index = 21;
        for (size_t i = (m_selectCurrentPage - 1) * m_pageSize; i < m_selectCurrentPage* m_pageSize; ++i) {
            if (fileName.begin() + i < fileName.end()) {
                m_MJPGList.SetUnitIsDisable(index, FALSE);
                m_MJPGList.SetUnitText(index, fileName[i], FALSE);
            } else {
                m_MJPGList.SetUnitText(index, L"", FALSE);
                m_MJPGList.SetUnitIsDisable(index, TRUE);
            }

            index++;
        }
        m_MJPGList.Invalidate();
    }
}

void CMainMp3Dlg::PageUp() {
    m_selectCurrentPage--;

    if (m_selectCurrentPage <= 1) {
        m_selectCurrentPage = 1;
        m_MJPGList.SetUnitIsDisable(9, TRUE);
    }
    if (m_selectCurrentPage < m_selectPageCount) {
        m_MJPGList.SetUnitIsDisable(10, FALSE);
    }

    CString str;
    str.Format(L"%d", m_selectCurrentPage);
    m_MJPGList.SetUnitText(40, str, TRUE);
    str.Empty();
    ShowArrayInList(m_ShowList);
}

void CMainMp3Dlg::PageDown() {
    m_selectCurrentPage++;   

    if (m_selectCurrentPage > 1) {
        m_MJPGList.SetUnitIsDisable(9, FALSE);
    }
    if (m_selectCurrentPage >= m_selectPageCount) {
        m_MJPGList.SetUnitIsDisable(10, TRUE);
    }

    CString str;
    str.Format(L"%d", m_selectCurrentPage);
    m_MJPGList.SetUnitText(40, str, TRUE);
    str.Empty();
    ShowArrayInList(m_ShowList);
}

void CMainMp3Dlg::ClearAll() {
    for (int i = 21; i <= 25; i++) {
        m_MJPGList.SetUnitText(i, L"", FALSE);
        m_MJPGList.SetUnitIsDownStatus(i, FALSE);
        m_MJPGList.SetUnitColor(i, font_red, FALSE);
    }
    m_MJPGList.Invalidate();
}

void CMainMp3Dlg::OnClickShowList(int unitNO) {
    for (int i = 21; i <= 25; i++) {
        if (i != unitNO) {
            m_MJPGList.SetUnitIsDownStatus(i, FALSE);
        } else {
            m_MJPGList.SetUnitIsDownStatus(i, TRUE);
        }
    }
    m_MJPGList.Invalidate();
    SetTimer(0x10, 10 * 1000, NULL);
}

void CMainMp3Dlg::OnDBClickShowList(int unitNO) {
    m_ClickType = 0;
    m_MJPGList.SetUnitIsDownStatus(3, FALSE);
    m_MJPGList.SetUnitIsShow(3, TRUE);
    KillTimer(IDT_GETINFO_TIIMER);
    m_MJPGList.SetUnitColor((gPlayIndex % m_pageSize) + 20, font_red, TRUE);
    gPlayIndex = (m_selectCurrentPage - 1) * m_pageSize + unitNO - 20;
    SetMP3(m_MP3List[gPlayIndex - 1]);
    playerDlg_->Show();
    m_IsPlay = 1;
    m_MoveText->m_nCount = 0;
    m_MoveText->SetTxt(m_ShowList[gPlayIndex - 1], 22, RGB(255, 255, 255));
    SetTimer(IDT_GETINFO_TIIMER, 1000, NULL);
    m_MJPGList.SetUnitIsDownStatus(3, TRUE);
    m_MJPGList.SetUnitIsShow(3, TRUE);
}

