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

void CMainMenuDlg::OnTimer(UINT nIDTimer)
{
// 	if(nIDTimer == IDC_SHOWPLYERDLG_TIMER)
// 	{
// 		KillTimer(IDC_SHOWPLYERDLG_TIMER);
// 		playerDlg_->ShowWindow(SW_SHOW);
// 	}
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
void CMainMenuDlg::OnAppSoftKey(WPARAM wParam, LPARAM lParam)
{
	if(wParam == 1)
	{
		OnVideoBtn();
	}
	else if(wParam == 2)
	{
		OnPhotoBtn();
	}
	else if(wParam == 3)
	{
		OnMp3Btn();
	}
	else if(wParam == 4)
	{
		OnLunarderBtn();
	}
	else if(wParam == 5)
	{
		OnCalculBtn();
	}
	else if(wParam == 6)
	{
		OnScreenSaveBtn();
	}
}

void CMainMenuDlg::OnVideoBtn() 
{
	// TODO: Add your control notification handler code here
	CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	main->m_pMainDlg->m_mainVideoDlg_->SetPlayList(_T("/flashdrv/my_video/"), 0);
	GetParent()->SendMessage(WM_CHANGEWINDOW, (WPARAM)0, SW_SHOW);   //切换到视频
//	main->m_mainVideoDlg_->ShowWindow(SW_SHOW);
}
void CMainMenuDlg::OnPhotoBtn() 
{
	// TODO: Add your control notification handler code here
	CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	main->m_pMainDlg->m_mainPhotoDlg_->SetPlayList(_T("/flashdrv/my_photo/"), 0);
	GetParent()->SendMessage(WM_CHANGEWINDOW, (WPARAM)1, SW_SHOW);   //切换到Photo

}

void CMainMenuDlg::OnMp3Btn()
{
	CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	main->m_pMainDlg->m_mainMp3Dlg_->SetPlayList(_T("/flashdrv/my_music/"), 0);
	GetParent()->SendMessage(WM_CHANGEWINDOW, (WPARAM)2, SW_SHOW);   //切换到Mp3
}

void CMainMenuDlg::SetVideo()
{
}

void CMainMenuDlg ::OnScreenSaveBtn()
{
	CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	int type = (int)main->m_pSettingDlg->m_pSetting->screenSaverContent();
	((CMainDlg *)(main->GetPanel(IDC_BUTTON_MAIN)))->m_mainScreenSaveDlg_->SetSaveScreenType(type, TRUE);	  //1 图片屏保   0  时间屏保
	GetParent()->SendMessage(WM_CHANGEWINDOW, (WPARAM)5, SW_SHOW);   //切换到屏保
}

void CMainMenuDlg::OnCalculBtn()
{
	CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	GetParent()->SendMessage(WM_CHANGEWINDOW, (WPARAM)4, SW_SHOW);   //切换到计算器
}

void CMainMenuDlg::OnLunarderBtn()
{
	SYSTEMTIME curtime;
	GetLocalTime(&curtime);
	CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	((CMainDlg *)(main->GetPanel(IDC_BUTTON_MAIN)))->m_mainLunarderDlg_->SetData(curtime.wYear, curtime.wMonth, curtime.wDay);
	GetParent()->SendMessage(WM_CHANGEWINDOW, (WPARAM)3, SW_SHOW);   //切换到日历
}

BOOL CMainMenuDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	std::string str;
	str=Data::LanguageResource::Get(Data::RI_REC_MODE);
	m_sticRecordedMode.Create(str.c_str(), WS_CHILD|WS_VISIBLE, CRect(24, 20, 230, 38), this);
	m_sticRecordedMode.SetColor(RGB(0, 0, 0), Data::g_allFramBackRGB[Data::g_skinstyle]);

	std::string str;
	str=Data::LanguageResource::Get(Data::RI_RECORD_QUALITY);
	m_sticRecordedQuality.Create(RI_RECORD_QUALITY, WS_CHILD|WS_VISIBLE, CRect(24, 40, 230, 58), this);
	m_sticRecordedQuality.SetColor(RGB(0, 0, 0), Data::g_allFramBackRGB[Data::g_skinstyle]);

	std::string str;
	str=Data::LanguageResource::Get(Data::RI_TOTAL_CAPACITY);
	m_sticDiskVolume.Create(RI_TOTAL_CAPACITY, WS_CHILD|WS_VISIBLE, CRect(24, 60, 230, 78), this);
	m_sticDiskVolume.SetColor(RGB(0, 0, 0), Data::g_allFramBackRGB[Data::g_skinstyle]);

	m_sticUnUsedDiskVolume.Create(_T("磁盘可用空间:"), WS_CHILD|WS_VISIBLE, CRect(24, 80, 230, 98), this);
	m_sticUnUsedDiskVolume.SetColor(RGB(0, 0, 0), Data::g_allFramBackRGB[Data::g_skinstyle]);
	
	m_procbarDiskUse.Create(WS_CHILD|WS_VISIBLE, CRect(24, 100, 230, 120), this, 0xFFFF);
	m_procbarDiskUse.SetParam(0, 0, 100, 1);
	m_procbarDiskUse.SetPos(0);
	m_procbarDiskUse.SetDisable(true);

	ICONKEYSTRUCT softkeyval_[3];

	softkeyval_[0].sKeyRect = CRect(0, 22, 69, 63);
	softkeyval_[0].nKeyValue = 4;
	softkeyval_[0].nTextId = Data::RI_MAIN_CALENDARBTN;
	softkeyval_[0].nBitmapId = IDB_BITMAP_MAINCAND;
	softkeyval_[0].nAlign = IMAGE_TOP;
	softkeyval_[0].bToggle = FALSE;

	softkeyval_[1].sKeyRect = CRect(70, 22, 139, 63);
	softkeyval_[1].nKeyValue = 5;
	softkeyval_[1].nTextId = Data::RI_MAIN_CALCULATORBTN;
	softkeyval_[1].nBitmapId = IDB_BITMAP_MAINCALU;
	softkeyval_[1].nAlign = IMAGE_TOP;
	softkeyval_[1].bToggle = FALSE;

	softkeyval_[2].sKeyRect = CRect(140, 22, 209, 63);
	softkeyval_[2].nKeyValue = 6;
	softkeyval_[2].nTextId = Data::RI_MAIN_SCREENSAVERBTN;
	softkeyval_[2].nBitmapId = IDB_BITMAP_MAINSAVE;
	softkeyval_[2].nAlign = IMAGE_TOP;
	softkeyval_[2].bToggle = FALSE;

	m_AppSoftKey.Create(0, 0, -1, CPoint(0, 0), 3, softkeyval_, CRect(15, 120, 15+210-1, 120+84-1), this, WS_VISIBLE|WS_CHILD, IDC_BUTTON1);
	SetSkinStyle();

// 	playerDlg_ = new CPlayerDlg();
// 	playerDlg_->Create(CPlayerDlg::IDD, this);
// 	playerDlg_->MoveWindow(CRect(13, 20, 228, 102), FALSE);

	/*
	//video
	playerDlg_->SetParam( "/flashdrv/my_video/apple192.avi");
	playerDlg_->ReSetWindowsRect(CRect(8, 8, 240, 120));
	playerDlg_->ShowWindow(SW_SHOW);
	*/

	//pic
// 	SetVideo();
// 	playerDlg_->ShowWindow(SW_SHOW);

	m_backGroudStatic.Create(CRect(0, 0, 240, 204), this);
//	m_backGroudStatic.SetInRect(CRect(11, 18, 229, 103));

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMainMenuDlg::SetSkinStyle()
{
	m_AppSoftKey.m_pKeys[0].nBitmapId = Data::g_mainAppVideoBmpID[Data::g_skinstyle];
	m_AppSoftKey.m_pKeys[1].nBitmapId = Data::g_mainAppPhotoBmpID[Data::g_skinstyle];
	m_AppSoftKey.m_pKeys[2].nBitmapId = Data::g_mainAppMp3BmpID[Data::g_skinstyle];
	m_AppSoftKey.m_pKeys[3].nBitmapId = Data::g_mainAppCandBmpID[Data::g_skinstyle];
	m_AppSoftKey.m_pKeys[4].nBitmapId = Data::g_mainAppCaluBmpID [Data::g_skinstyle];
	m_AppSoftKey.m_pKeys[5].nBitmapId = Data::g_mainAppSaveBmpID[Data::g_skinstyle];

	m_AppSoftKey.m_clrBack = Data::g_allFramBackRGB[Data::g_skinstyle];
}

void CMainMenuDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	
	// Do not call CDialog::OnPaint() for painting messages
}

void CMainMenuDlg::ShowWindow_(int nCmdShow, int type)
{
	CString text;
	if (((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pSetting->isMustRecord())
	{
		text = _T("录音模式: 强制录音");
	}
	else
	{
		text = _T("录音模式: 手动录音");
	}

	m_sticRecordedMode.SetWindowText(text);

	if (((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pSetting->isDeleteProtect())
	{
		text = _T("录音质量: 高质量");
	}
	else
	{
		text = _T("录音质量: 标准");
	}

	m_sticRecordedQuality.SetWindowText(text);

	ULARGE_INTEGER freeBytes;
	ULARGE_INTEGER totalBytes;
	GetDiskFreeSpaceEx(_T("\\Flashdrv"), &freeBytes, &totalBytes, NULL);

	char total[128];
	int tol = totalBytes.QuadPart / (1024*1024);
	sprintf(total, "%d", tol);
	CString to= total;

	m_sticDiskVolume.SetWindowText(_T("磁盘总容量: ") + to + _T(" M"));


	int s = freeBytes.QuadPart - PreFreeSize;
	if (s < 0)
	{
		s = 0;
	}
	s = int (s /(1024*1024));
	char remain[128];
	sprintf(remain, "%d", s);
	CString re = remain;
	m_sticUnUsedDiskVolume.SetWindowText(_T("磁盘可用空间: ") + re + _T(" M"));

	int p = (tol - s) * 100 / tol;
	m_procbarDiskUse.SetPos(p);


	if(nCmdShow == SW_SHOW)
	{
//		playerDlg_->SetParam( "", 0, TRUE);
	}
	//解决刷新左右两边不同步显示
	if(type == 1)
	{
		ShowWindow(nCmdShow);
//		SetTimer(IDC_SHOWPLYERDLG_TIMER, 10, NULL);

	//	playerDlg_->ShowWindow(nCmdShow);   //移到Timer中处理	
	}
	else
	{
//		playerDlg_->ShowWindow(nCmdShow);	
		ShowWindow(nCmdShow);
	}
}