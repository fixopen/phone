// MainPhotoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MainPhotoDlg.h"
#include "../resource.h"
#include "../Data/LanguageResource.h"
#include "../Data/SkinStyle.h"
#include "../MultimediaPhoneDlg.h"
#include "../MultimediaPhone.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#include<algorithm>

extern int g_isAutoPlay;
static BOOL gbPhotoPauseCount = 0;
extern BOOL DetectDIR(TCHAR* sDir);
/////////////////////////////////////////////////////////////////////////////
// CMainPhotoDlg dialog

CMainPhotoDlg::CMainPhotoDlg(CWnd* pParent /*=NULL*/)
: CDialog(CMainPhotoDlg::IDD, pParent) {
    //{{AFX_DATA_INIT(CMainPhotoDlg)
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
    memset(m_chDir, 0, 128 * 2);
    m_nPhotoType = local_type;
}


void CMainPhotoDlg::DoDataExchange(CDataExchange* pDX) {
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CMainPhotoDlg)
    // NOTE: the ClassWizard will add DDX and DDV calls here
    //}}AFX_DATA_MAP

}

BEGIN_MESSAGE_MAP(CMainPhotoDlg, CDialog)
    //{{AFX_MSG_MAP(CMainPhotoDlg)
    ON_BN_CLICKED(IDC_BUTTON_PHOTOEXIT, OnExit)
    ON_BN_CLICKED(IDC_BUTTON_PHOTOPLAY, OnPlayerFirst)
    ON_BN_CLICKED(IDC_BUTTON_PHOTOPRE, OnPre)
    ON_BN_CLICKED(IDC_BUTTON_PHOTOBACK, OnBack)
    ON_BN_CLICKED(IDC_BUTTON_PHOTOOPEN, OnOpenFile)
    ON_BN_CLICKED(IDC_BUTTON_PHOTOPLAYALL, OnPlayerAll)
    ON_BN_CLICKED(IDC_BUTTON_PHOTOSCREENSAVE, OnScreenSave)
    ON_WM_ACTIVATE()
    ON_MESSAGE(WM_MJPGTOGGLE, OnClickMJPG)
    ON_MESSAGE(WM_CLICKMJPG_TOAPP, OnClickMJPG)
    ON_MESSAGE(WM_OUTEVENT, OnOutEvent)
    ON_WM_TIMER()
    ON_WM_KEYDOWN()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CMainPhotoDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
    if (nChar == 'U') {
        OnPre();
    } else if (nChar == 'D') {
        OnBack();
    }
}

void CMainPhotoDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) {
    if (nState & WA_ACTIVE) {
        CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
    }
}

/////////////////////////////////////////////////////////////////////////////
// CMainPhotoDlg message handlers

BOOL CMainPhotoDlg::OnInitDialog() {
    CDialog::OnInitDialog();

    // TODO: Add extra initialization here

    //播放器
    CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
    playerDlg_ = new CPlayerDlg(main->playerimage_);
    playerDlg_->Create(CPlayerDlg::IDD, this);
    playerDlg_->ReSetWindowsRect(CRect(110, 88, 552, 385));

    //wangzhenxing20091011
    m_MJPGList.Create(L"", WS_CHILD | WS_VISIBLE, CRect(0, 0, 800, 423), this);
    m_MJPGList.SetCurrentLinkFile(L".\\ADV\\MJPG\\k5\\中文\\图片播放.xml");

    m_MJPGList.SetUnitFont(30, font_16);
    m_MJPGList.SetUnitColor(30, font_white, TRUE);
    m_MJPGList.SetUnitFont(31, font_16);
    m_MJPGList.SetUnitColor(31, font_white, TRUE);

    MoveWindow(0, 57, 800, 423);

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CMainPhotoDlg::OnExit() {
    KillTimer(0x100);
    playerDlg_->StopTimer();
    playerDlg_->ShowWindow(SW_HIDE);
    playerDlg_->player_->ExitPlayer(TRUE);

    GetParent()->SendMessage(WM_CHANGEWINDOW, (WPARAM)this, (LPARAM)SW_HIDE);
}

//播放
void CMainPhotoDlg::OnPlayer(int index) {
    if (g_isAutoPlay)   ////
    {
        playerDlg_->StopTimer();
        g_isAutoPlay = FALSE;
        m_MJPGList.SetUnitIsDownStatus(3, FALSE);
    } else {
        playerDlg_->SetResumeTimer();
        g_isAutoPlay = TRUE;
        m_MJPGList.SetUnitIsDownStatus(3, TRUE);
    }
    m_MJPGList.SetUnitIsShow(3, TRUE);
}

//前一条
void CMainPhotoDlg::OnPre() {
    gbPhotoPauseCount = 0;
    playerDlg_->OnKeyDown('U', 0, 0);
    if (m_selectCurrentFile == 1) {
        m_selectCurrentFile = m_selectFileCount;
    } else {
        m_selectCurrentFile--;
    }

    SetCurrentFile();
}
//后一条
void CMainPhotoDlg::OnBack() {
    gbPhotoPauseCount = 0;
    playerDlg_->OnKeyDown('D', 0, 0);
    if (m_selectCurrentFile == m_selectFileCount) {
        m_selectCurrentFile = 1;
    } else {
        m_selectCurrentFile++;
    }

    SetCurrentFile();
}

void CMainPhotoDlg::OnFirst() {
    CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
    main->playerimage_->First();
    m_selectCurrentFile = 1;
    SetCurrentFile();
}

void CMainPhotoDlg::OnLast() {
    CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
    main->playerimage_->Last();
    m_selectCurrentFile = m_selectFileCount;
    SetCurrentFile();
}

void CMainPhotoDlg::NarrowPicture() {
    CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
    main->playerimage_->NarrowPlay(m_PhotoList[m_selectCurrentFile - 1]);
}

void CMainPhotoDlg::ZoomPicture() {
    CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
    main->playerimage_->ZoomPlay(m_PhotoList[m_selectCurrentFile - 1]);
}

void CMainPhotoDlg::RotatePicture() {
    CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
    main->playerimage_->RotatePicture(m_PhotoList[m_selectCurrentFile - 1]);
}

void CMainPhotoDlg::SetCurrentFile() {
    CString s;
    s.Format(L"%d", m_selectCurrentFile);
    m_MJPGList.SetUnitText(30, s, TRUE);
}

//打开播放列表
void CMainPhotoDlg::OnOpenFile() {
    playerDlg_->player_->ExitPlayer(TRUE);

    playerDlg_->StopTimer();
    playerDlg_->ShowWindow(SW_HIDE);
}

//全屏播放
void CMainPhotoDlg::OnPlayerAll() {
    //  GetParent()->SendMessage(WM_PLAYPHOTO, 2);  //全屏

    SetAllScreenPlayer(TRUE);
    playerDlg_->SetActiveWindow();

    ::SetCursorPos(799, 479);
}

void CMainPhotoDlg::OnScreenSave() {
    //wangzhenxing20091011
    int nFileSelected = 0;
    int ncount = m_PhotoList.size();
    FILE* file = NULL;
    for (int i = 0; i < ncount; i++) {
        //      int val = m_PhotoList[i];
        //      if(val == 1)
        //      {
        nFileSelected++;
        if (nFileSelected == 1) {
            fopen_s(&file, my_photo_dir_list, "w+b");
            fwrite(m_PhotoList[i], sizeof(char), m_PhotoList[i].GetLength(), file);
        } else {
              fwrite(m_PhotoList[i], sizeof(char), m_PhotoList[i].GetLength(), file);
        }
        //      }
    }
    if (file) {
        fclose(file);
    }
}

void CMainPhotoDlg::SetCtrlEnable(BOOL flag) {
}

void CMainPhotoDlg::SetAllScreenPlayer(BOOL flag) {
    if (flag) {
        CRect rt = CRect(0, 0, 800, 480);
        playerDlg_->MoveWindow(&rt);
    } else {
        CRect rt = CRect(110, 88, 552, 385);
        playerDlg_->MoveWindow(&rt);
    }

    playerDlg_->player_->SetAllScreenPlayer(flag);
    Invalidate();   //lxz 2007 11 30
}

LRESULT CMainPhotoDlg::OnClickMJPG(WPARAM w, LPARAM l) {
    LRESULT result = 0;
    switch (w) {
    case 1:
        //第一张
        OnFirst();
        break;
    case 2:
        //上一条
        OnPre();
        break;
    case 3:
        //播放
        OnPlayer();
        break;      
    case 4:
        //下一条
        OnBack();
        break;
    case 5:
        //最后一张
        OnLast();
        break;
    case 6:
        //全屏
        OnPlayerAll();
        break;
    case 11:
        //保存至屏保
        OnScreenSave();
        break;
    case 20:
        //放大
        ZoomPicture();
        break;
    case 21:
        //缩小
        NarrowPicture();
        break;
    case 22:
        //旋转
        RotatePicture();
        break;
    case 30:
        //输入要查看第几张
        break;
    case 50:
        //图片上拉
        break;
    case 60:
        //图片左拉
        break;
    case 70:
        //图片下拉
        break;
    case 80:
        //图片右拉
        break;
    case 1000:
        //退出
        OnExit();
        break;
    }
    return result;
}

void CMainPhotoDlg::OnTimer(UINT nIDEvent) {
    // TODO: Add your message handler code here and/or call default
    CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;

    if (nIDEvent == 1 && IsWindowVisible() && playerDlg_->IsWindowVisible()) {
        main->playerimage_->Down();
    } else if (nIDEvent == 0x100) {
        KillTimer(0x100);
        if (IsWindowVisible()) {
            OnPlayer(-1);
        }
    } else if (nIDEvent == 100) {
        KillTimer(100);
        if (memcmp(m_chDir, ptchLocalPhoto, wcslen(m_chDir) * 2) == 0) {
            main->m_pMainDlg->m_p3GHomePicDlg->SetPlayList(ptchLocalPhoto);
        }
    }

    CDialog::OnTimer(nIDEvent);
}

LRESULT CMainPhotoDlg::OnOutEvent(WPARAM w, LPARAM l) {
    LRESULT result = 0;
    if (IsWindowVisible()) {
        CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
        if (l == 0 && main->playerimage_->isPlaying_)  //暂停播放
        {
            gbPhotoPauseCount = 1;
            playerDlg_->StopTimer();
            playerDlg_->ShowWindow(SW_HIDE);
            playerDlg_->player_->ExitPlayer(TRUE);
        } else if (l == 1) //恢复播放
        {
              //if(gnPhotoPauseCount <= 0)    //不能解决冲突问题.   //20090223
            if (gbPhotoPauseCount && !main->m_AlarmShowDlg->IsWindowVisible() && !main->m_pTelephoneDlg->IsWindowVisible()) {
                gbPhotoPauseCount = 0;
                SetTimer(0x100, 5, NULL);
            }
        }
    }
    return result;
}

LRESULT CMainPhotoDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_DELETESELITEM:
        {
        }
        break;
    default:
        break;
    }
    return CDialog::WindowProc(message, wParam, lParam);
}