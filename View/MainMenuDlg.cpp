// MainMenuDlg.cpp : implementation file
//

#include "stdafx.h"
#include "../MultimediaPhone.h"
#include "../Data/LanguageResource.h"
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
//#define WM_CHANGEWINDOW	8001

CMainMenuDlg::CMainMenuDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMainMenuDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMainMenuDlg)
		// NOTE: the ClassWizard will add member initialization here

	//}}AFX_DATA_INIT
}

CMainMenuDlg:: ~CMainMenuDlg()
{
	
}

void CMainMenuDlg::DoDataExchange(CDataExchange* pDX)
{
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


BEGIN_MESSAGE_MAP(CMainMenuDlg, CDialog)
	//{{AFX_MSG_MAP(CMainMenuDlg)
	ON_BN_CLICKED(IDC_BUTTON1, OnVideoBtn)
	ON_BN_CLICKED(IDC_BUTTON2, OnPhotoBtn)
	ON_BN_CLICKED(IDC_BUTTON3, OnMp3Btn)
	ON_BN_CLICKED(IDC_BUTTON4, OnLunarderBtn)
	ON_BN_CLICKED(IDC_BUTTON5, OnCalculBtn)
	ON_BN_CLICKED(IDC_BUTTON6, OnScreenSaveBtn)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainMenuDlg message handlers

void CMainMenuDlg::OnVideoBtn() 
{
	// TODO: Add your control notification handler code here
	CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	GetParent()->SendMessage(WM_CHANGEWINDOW, (WPARAM)0, SW_SHOW);   //切换到视频
//	main->m_mainVideoDlg_->ShowWindow(SW_SHOW);
}
void CMainMenuDlg::OnPhotoBtn() 
{
	// TODO: Add your control notification handler code here
	CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	GetParent()->SendMessage(WM_CHANGEWINDOW, (WPARAM)1, SW_SHOW);   //切换到Photo
//	main->m_mainVideoDlg_->ShowWindow(SW_SHOW);
}

void CMainMenuDlg::OnMp3Btn()
{
	CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	GetParent()->SendMessage(WM_CHANGEWINDOW, (WPARAM)2, SW_SHOW);   //切换到Mp3
}

void CMainMenuDlg::SetVideo()
{
	playerDlg_->SetParam( "/flashdrv/apple192.avi", FALSE, TRUE);
	playerDlg_->SetImageList("/flashdrv/my_photo/");
	playerDlg_->ReSetWindowsRect(CRect(8, 8, 240, 120));
}

void CMainMenuDlg ::OnScreenSaveBtn()
{
	CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	((CMainDlg *)(main->GetPanel(IDC_BUTTON_MAIN)))->m_mainScreenSaveDlg_->SetSaveScreenType(0, TRUE);	
	GetParent()->SendMessage(WM_CHANGEWINDOW, (WPARAM)5, SW_SHOW);   //切换到屏保
}

void CMainMenuDlg::OnCalculBtn()
{
	CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	//((CMainDlg *)(main->GetPanel(IDC_BUTTON_MAIN)))->m_mainCalucaterDlg_->ClearData();
	GetParent()->SendMessage(WM_CHANGEWINDOW, (WPARAM)4, SW_SHOW);   //切换到计算器
}

void CMainMenuDlg::OnLunarderBtn()
{
	SYSTEMTIME curtime;
	GetLocalTime(&curtime);
	CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	((CMainDlg *)(main->GetPanel(IDC_BUTTON_MAIN)))->m_mainLunarderDlg_->m_lunarder.SetData(curtime.wYear, curtime.wMonth, curtime.wDay, TRUE);
	GetParent()->SendMessage(WM_CHANGEWINDOW, (WPARAM)3, SW_SHOW);   //切换到日历
}

BOOL CMainMenuDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	//CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	
	playerDlg_ = new CPlayerDlg();
	playerDlg_->Create(CPlayerDlg::IDD, this);
	
	/*
	//video
	playerDlg_->SetParam( "/flashdrv/my_video/apple192.avi");
	playerDlg_->ReSetWindowsRect(CRect(8, 8, 240, 120));
	playerDlg_->ShowWindow(SW_SHOW);
	*/
	//pic
	SetVideo();
	playerDlg_->ShowWindow(SW_SHOW);

	CString s;
	s = Data::LanguageResource::Get(Data::RI_MAIN_VIDEOBTN).c_str();
	m_VideoBtn.Create(s, WS_CHILD|WS_VISIBLE, CRect(21, 122, 94, 142), this, IDC_BUTTON1);
	m_VideoBtn.SetIcon(IDI_ICON_VIDEO, CSize(16, 16));
	s = Data::LanguageResource::Get(Data::RI_MAIN_POTOBTN).c_str();
	m_PhotoBtn.Create(s, WS_CHILD|WS_VISIBLE, CRect(112, 122, 185, 142), this, IDC_BUTTON2);
	m_PhotoBtn.SetIcon(IDI_ICON_PHOTO, CSize(16, 16));
	s = Data::LanguageResource::Get(Data::RI_MAIN_MP3BTN).c_str();
	m_MusicBtn.Create(s, WS_CHILD|WS_VISIBLE, CRect(21, 147, 94, 167), this, IDC_BUTTON3);
	m_MusicBtn.SetIcon(IDI_ICON_MP3, CSize(16, 16));
	s = Data::LanguageResource::Get(Data::RI_MAIN_CALENDARBTN).c_str();
	m_CalenBtn.Create(s, WS_CHILD|WS_VISIBLE, CRect(112, 147, 185, 167), this, IDC_BUTTON4);
	m_CalenBtn.SetIcon(IDI_ICON_CANL, CSize(16, 16));
	s = Data::LanguageResource::Get(Data::RI_MAIN_CALCULATORBTN).c_str();
	m_CalcuBtn.Create(s, WS_CHILD|WS_VISIBLE, CRect(21, 172, 94, 192), this, IDC_BUTTON5);
	m_CalcuBtn.SetIcon(IDI_ICON_CALU, CSize(16, 16));
	s = Data::LanguageResource::Get(Data::RI_MAIN_SCREENSAVERBTN).c_str();
	m_ScreenSaveBtn.Create(s, WS_CHILD|WS_VISIBLE, CRect(112, 172, 185, 192), this, IDC_BUTTON6);
	m_ScreenSaveBtn.SetIcon(IDI_ICON_SAVESCREEN, CSize(16, 16));

	m_backGroudStatic.Create(CRect(0, 0, 240, 204), this);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMainMenuDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	
	// Do not call CDialog::OnPaint() for painting messages
}
