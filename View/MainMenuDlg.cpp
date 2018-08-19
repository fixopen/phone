// MainMenuDlg.cpp : implementation file
//

#include "stdafx.h"
#include "../MultimediaPhone.h"
#include "../Data/LanguageResource.h"
#include "../Data/SkinStyle.h"
#include "MainMenuDlg.h"
#include "../MultimediaPhoneDlg.h"
#include "../MultimediaPhone.h"
#include "MainDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainMenuDlg dialog
//#define WM_CHANGEWINDOW   8001

CMainMenuDlg::CMainMenuDlg(CWnd* pParent /*=NULL*/)
: CDialog(CMainMenuDlg::IDD, pParent) {
    //{{AFX_DATA_INIT(CMainMenuDlg)
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT

}

CMainMenuDlg:: ~CMainMenuDlg() {
}

void CMainMenuDlg::DoDataExchange(CDataExchange* pDX) {
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CMainMenuDlg)
    /*
    DDX_Control(pDX, IDC_BUTTON6, m_ScreenSaveBtn);
    DDX_Control(pDX, IDC_BUTTON5, m_CalcuBtn);
    DDX_Control(pDX, IDC_BUTTON4, m_CalenBtn);
    DDX_Control(pDX, IDC_BUTTON3, m_MusicBtn);
    DDX_Control(pDX, IDC_BUTTON2, m_PhotoBtn);
    DDX_Control(pDX, IDC_BUTTON1, m_VideoBtn);
    */
    //}}AFX_DATA_MAP

}

void CMainMenuDlg::OnTimer(UINT nIDTimer) {
    if (nIDTimer == IDC_SHOWPLYERDLG_TIMER) {
        KillTimer(IDC_SHOWPLYERDLG_TIMER);
        //      playerDlg_->ShowWindow(SW_SHOW);
    }
    CDialog::OnTimer(nIDTimer);
}

BEGIN_MESSAGE_MAP(CMainMenuDlg, CDialog)
    //{{AFX_MSG_MAP(CMainMenuDlg)
    ON_WM_PAINT()
    ON_WM_TIMER()
    ON_MESSAGE(WM_SOFTKEY, OnAppSoftKey)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainMenuDlg message handlers
LRESULT CMainMenuDlg::OnAppSoftKey(WPARAM wParam, LPARAM lParam) {
    LRESULT result = 0;
    if (wParam == 1) {
        OnVideoBtn();
    } else if (wParam == 2) {
        OnPhotoBtn();
    } else if (wParam == 3) {
        OnMp3Btn();
    } else if (wParam == 4) {
        OnLunarderBtn();
    } else if (wParam == 5) {
        OnCalculBtn();
    } else if (wParam == 6) {
        OnScreenSaveBtn();
    }
    return result;
}

void CMainMenuDlg::OnVideoBtn() {
    // TODO: Add your control notification handler code here
    CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
    GetParent()->SendMessage(WM_CHANGEWINDOW, (WPARAM)0, SW_SHOW);   //切换到视频
}
void CMainMenuDlg::OnPhotoBtn(int type) {
    // TODO: Add your control notification handler code here
    CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
    main->m_pMainDlg->m_mainPhotoDlg_->m_nPhotoType = (PhotoType)(!type);
    GetParent()->SendMessage(WM_CHANGEWINDOW, (WPARAM)1, SW_SHOW);   //切换到Photo

    //  main->m_mainVideoDlg_->ShowWindow(SW_SHOW);
}

void CMainMenuDlg::OnMp3Btn() {
    CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
    GetParent()->SendMessage(WM_CHANGEWINDOW, (WPARAM)2, SW_SHOW);   //切换到Mp3
}

void CMainMenuDlg::SetVideo() {
}

void CMainMenuDlg ::OnScreenSaveBtn() {
    CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
    int type = (int)main->m_pSettingDlg->m_pSetting->screenSaverContent();
    ((CMainDlg *)(main->GetPanel(IDC_BUTTON_MAIN)))->m_mainScreenSaveDlg_->SetSaveScreenType(type, TRUE);     //1 图片屏保   0  时间屏保
    GetParent()->SendMessage(WM_CHANGEWINDOW, (WPARAM)5, SW_SHOW);   //切换到互联网
}

void CMainMenuDlg::OnCalculBtn() {
    CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
    //  ((CMainDlg *)(main->GetPanel(IDC_BUTTON_MAIN)))->m_mainCalucaterDlg_->ClearData();
    GetParent()->SendMessage(WM_CHANGEWINDOW, (WPARAM)4, SW_SHOW);   //切换到计算器
}

void CMainMenuDlg::OnLunarderBtn() {
    SYSTEMTIME curtime;
    GetLocalTime(&curtime);
    CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
    ((CMainDlg *)(main->GetPanel(IDC_BUTTON_MAIN)))->m_mainLunarderDlg_->SetData(curtime.wYear, curtime.wMonth, curtime.wDay);
    GetParent()->SendMessage(WM_CHANGEWINDOW, (WPARAM)3, SW_SHOW);   //切换到日历
}

BOOL CMainMenuDlg::OnInitDialog() {
    CDialog::OnInitDialog();

    // TODO: Add extra initialization here
    //CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;

    CString s;
    s = Data::LanguageResource::Get(Data::RI_MAIN_VIDEOBTN).c_str();

    ICONKEYSTRUCT softkeyval_[6];
    softkeyval_[0].sKeyRect = CRect(0, 0, 69, 41);
    softkeyval_[0].nKeyValue = 1;
    softkeyval_[0].nTextId = Data::RI_MAIN_VIDEOBTN;
    softkeyval_[0].nBitmapId = IDB_BITMAP_MAINVIDEO;
    softkeyval_[0].nAlign = IMAGE_TOP;
    softkeyval_[0].bToggle = FALSE;

    softkeyval_[1].sKeyRect = CRect(70, 0, 139, 41);
    softkeyval_[1].nKeyValue = 2;
    softkeyval_[1].nTextId = Data::RI_MAIN_POTOBTN;
    softkeyval_[1].nBitmapId = IDB_BITMAP_MAINPHOTO;
    softkeyval_[1].nAlign = IMAGE_TOP;
    softkeyval_[1].bToggle = FALSE;

    softkeyval_[2].sKeyRect = CRect(140, 0, 209, 41);
    softkeyval_[2].nKeyValue = 3;
    softkeyval_[2].nTextId = Data::RI_MAIN_MP3BTN;
    softkeyval_[2].nBitmapId = IDB_BITMAP_MAINMP3;
    softkeyval_[2].nAlign = IMAGE_TOP;
    softkeyval_[2].bToggle = FALSE;

    softkeyval_[3].sKeyRect = CRect(0, 42, 69, 83);
    softkeyval_[3].nKeyValue = 4;
    softkeyval_[3].nTextId = Data::RI_MAIN_CALENDARBTN;
    softkeyval_[3].nBitmapId = IDB_BITMAP_MAINCAND;
    softkeyval_[3].nAlign = IMAGE_TOP;
    softkeyval_[3].bToggle = FALSE;

    softkeyval_[4].sKeyRect = CRect(70, 42, 139, 83);
    softkeyval_[4].nKeyValue = 5;
    softkeyval_[4].nTextId = Data::RI_MAIN_CALCULATORBTN;
    softkeyval_[4].nBitmapId = IDB_BITMAP_MAINCALU;
    softkeyval_[4].nAlign = IMAGE_TOP;
    softkeyval_[4].bToggle = FALSE;

    softkeyval_[5].sKeyRect = CRect(140, 42, 209, 83);
    softkeyval_[5].nKeyValue = 6;
    softkeyval_[5].nTextId = Data::RI_MAIN_SCREENSAVERBTN;
    softkeyval_[5].nBitmapId = IDB_BITMAP_MAINSAVE;
    softkeyval_[5].nAlign = IMAGE_TOP;
    softkeyval_[5].bToggle = FALSE;

    m_AppSoftKey.Create(0, 0, -1, CPoint(0, 0), 6, softkeyval_, CRect(15, 110, 15 + 210 - 1, 110 + 84 - 1), this, WS_VISIBLE | WS_CHILD, IDC_BUTTON1);
    SetSkinStyle();

    SetVideo();
    //  playerDlg_->ShowWindow(SW_SHOW);

    m_backGroudStatic.Create(CRect(0, 0, 240, 204), this);
    m_backGroudStatic.SetInRect(CRect(11, 18, 229, 103));

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CMainMenuDlg::SetSkinStyle() {
    m_AppSoftKey.m_pKeys[0].nBitmapId = Data::g_mainAppVideoBmpID[Data::g_skinstyle];
    m_AppSoftKey.m_pKeys[1].nBitmapId = Data::g_mainAppPhotoBmpID[Data::g_skinstyle];
    m_AppSoftKey.m_pKeys[2].nBitmapId = Data::g_mainAppMp3BmpID[Data::g_skinstyle];
    m_AppSoftKey.m_pKeys[3].nBitmapId = Data::g_mainAppCandBmpID[Data::g_skinstyle];
    m_AppSoftKey.m_pKeys[4].nBitmapId = Data::g_mainAppCaluBmpID[Data::g_skinstyle];
    m_AppSoftKey.m_pKeys[5].nBitmapId = Data::g_mainAppSaveBmpID[Data::g_skinstyle];

    m_AppSoftKey.m_clrBack = Data::g_allFramBackRGB[Data::g_skinstyle];
}

void CMainMenuDlg::OnPaint() {
    CPaintDC dc(this); // device context for painting
    // TODO: Add your message handler code here

    // Do not call CDialog::OnPaint() for painting messages
}

void CMainMenuDlg::ShowWindow_(int nCmdShow, int type) {
    if (nCmdShow == SW_SHOW) {
        //      playerDlg_->SetParam( "", mtImage, TRUE);
    }
    //解决刷新左右两边不同步显示
    if (type == 1) {
        ShowWindow(nCmdShow);
        SetTimer(IDC_SHOWPLYERDLG_TIMER, 10, NULL);
        //  playerDlg_->ShowWindow(nCmdShow);   //移到Timer中处理   
    } else {
        //  playerDlg_->ShowWindow(nCmdShow);   //lxz 080603    
        ShowWindow(nCmdShow);
    }
}