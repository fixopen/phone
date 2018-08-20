// MainDlg.cpp : implementation file
//

#include "stdafx.h"
#include "../MultimediaPhone.h"
#include "../MultimediaPhoneDlg.h"
#include "../Data/LanguageResource.h"
#include "../Data/SkinStyle.h"
#include "MainDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainDlg dialog

#define TIMER_MAINDLG		0xFFF3

CMainDlg::CMainDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMainDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMainDlg)
	m_mainmenuDlg_ = NULL;
	//}}AFX_DATA_INIT
}

CMainDlg::~CMainDlg()
{
	if(m_mainmenuDlg_)
		delete m_mainmenuDlg_;
	m_mainmenuDlg_ = NULL;
}

void CMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMainDlg)
	/*
	DDX_Control(pDX, IDC_STATIC_LINE, m_LineStatic);
	DDX_Control(pDX, IDC_STATIC_TODAY, m_TodayStatic);
	DDX_Control(pDX, IDC_STATIC_TELSTATUS, m_TelStatusStatic);
	DDX_Control(pDX, IDC_STATIC_CALLWALL, m_CallWallStatic);
	DDX_Control(pDX, IDC_STATIC_TIME, m_TimeStatic);
	*/
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMainDlg, CDialog)
	//{{AFX_MSG_MAP(CMainDlg)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_MESSAGE(WM_CHANGEWINDOW, OnChangeWindow)
	ON_MESSAGE(WM_PLAYVIDEO, OnPlayVideo)
	ON_MESSAGE(WM_PLAYPHOTO, OnPlayPhoto)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainDlg message handlers
void CMainDlg::SetCtrlEnable(BOOL flag)
{
//	m_LineStatic.EnableWindow(flag);
//	m_TodayStatic.EnableWindow(flag);
	m_TelStatusStatic.EnableWindow(flag);
	m_CallWallStatic.EnableWindow(flag);
	m_TimeStatic.EnableWindow(flag);
}

BOOL CMainDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_mainmenuDlg_ = new CMainMenuDlg;
	m_mainmenuDlg_->Create(CMainMenuDlg::IDD, this);
	m_mainmenuDlg_->ShowWindow(SW_SHOW);		

	m_mainVideoDlg_ = new CMainVideoDlg;
	m_mainVideoDlg_->Create(CMainVideoDlg::IDD, this);
	m_mainVideoDlg_->ShowWindow(SW_HIDE);

	m_mainPhotoDlg_ = new CMainPhotoDlg;
	m_mainPhotoDlg_->Create(CMainPhotoDlg::IDD, this);
	m_mainPhotoDlg_->ShowWindow(SW_HIDE);

	m_mainMp3Dlg_ = new CMainMp3Dlg;
	m_mainMp3Dlg_->Create(CMainMp3Dlg::IDD, this);
	m_mainMp3Dlg_->ShowWindow(SW_HIDE);

	m_mainLunarderDlg_ = new CLunarderDlg;
	m_mainLunarderDlg_->Create(CLunarderDlg::IDD, this);
	m_mainLunarderDlg_->ShowWindow(SW_HIDE);
		
	m_mainScreenSaveDlg_ = new CScreenSaveDlg;
	m_mainScreenSaveDlg_->Create(CScreenSaveDlg::IDD, this);
	m_mainScreenSaveDlg_->ShowWindow(SW_HIDE);

	m_mainCalucaterDlg_ = new CCalculaterDlg;
	m_mainCalucaterDlg_->Create(CCalculaterDlg::IDD, this);
	m_mainCalucaterDlg_->ShowWindow(SW_HIDE);

//	m_LineStatic.Create(L"", WS_CHILD|WS_VISIBLE, CRect(247-MAIN_MARGIN_OFFSET,9, 248-MAIN_MARGIN_OFFSET, 195), this, IDC_STATIC_LINE);
//	m_LineStatic.SetColor(RGB(0, 0, 0), DIVIDEDLINE_RGB);
	
//	m_TodayStatic.Create(L"", WS_CHILD|WS_VISIBLE, CRect(252-MAIN_MARGIN_OFFSET,9, 471-MAIN_MARGIN_OFFSET, 29), this, IDC_STATIC_TODAY);
//	m_TodayStatic.SetColor(RGB(0, 0, 0), STATIC_RGB1);
//	CString s = Data::LanguageResource::Get(Data::RI_MAIN_TODAYSTC).c_str();
//	m_TodayStatic.SetWindowText(s);
	
	m_TelStatusStatic.Create(L"", WS_CHILD|WS_VISIBLE, CRect(263, 111, 263+203-1, 111+23-1), this, IDC_STATIC_TELSTATUS);
	m_TelStatusStatic.SetColor(Data::g_mainTxtColor[0][Data::g_skinstyle], RGB(0, 0, 0));
//	m_TelStatusStatic.SetUnderLine(TRUE);
	m_TelStatusStatic.SetBitmapID(Data::g_mainTxtBmpID[Data::g_skinstyle]);
	m_TelStatusStatic.SetAlign(DT_CENTER);
	char txt[64] = {0};
	sprintf(txt, "%d%s%s(%d%s%s)", GetMissedTelInfo(), Data::LanguageResource::Get(Data::RI_COMN_ENTRIESSTC).c_str(), Data::LanguageResource::Get(Data::RI_COMN_MISSTELSTC).c_str(),\
		GetRecordedTelInfo(), Data::LanguageResource::Get(Data::RI_COMN_ENTRIESSTC).c_str(), Data::LanguageResource::Get(Data::RI_COMN_RECORDETELSTC).c_str());
	CString s = txt;
	m_TelStatusStatic.SetWindowText(s);
	m_TelStatusStatic.SetClicked(TRUE);

	m_CallWallStatic.Create(L"", WS_CHILD|WS_VISIBLE, CRect(263, 141, 263+203-1, 141+23-1), this, IDC_STATIC_CALLWALL);
	m_CallWallStatic.SetColor(Data::g_mainTxtColor[1][Data::g_skinstyle], RGB(0, 0, 0));
//	m_CallWallStatic.SetUnderLine(TRUE);
	m_CallWallStatic.SetBitmapID(Data::g_mainTxtBmpID[Data::g_skinstyle]);
	m_CallWallStatic.SetAlign(DT_CENTER);
	if(GetCallWallInfo())
	{
		sprintf(txt, "%s", Data::LanguageResource::Get(Data::RI_MAIN_PROTECTEDWALL_OPEN_STC).c_str());
	}
	else
	{
		sprintf(txt, "%s", Data::LanguageResource::Get(Data::RI_MAIN_PROTECTEDWALL_CLOSE_STC).c_str());
	}
	s = txt;
	m_CallWallStatic.SetWindowText(s);
	m_CallWallStatic.SetClicked(TRUE);

	
	m_TodayNoteStatic.Create(L"", WS_CHILD|WS_VISIBLE, CRect(263, 170, 263+203-1, 170+23-1), this, IDC_STATIC_TELSTATUS);
	m_TodayNoteStatic.SetBitmapID(Data::g_mainTxtBmpID[Data::g_skinstyle]);
	m_TodayNoteStatic.SetAlign(DT_CENTER);
	m_TodayNoteStatic.SetColor(Data::g_mainTxtColor[2][Data::g_skinstyle], RGB(0, 0, 0));

	//todo 添加今天的记事显示
	sprintf(txt, "%s", "今天无记事");
	s = txt;
	m_TodayNoteStatic.SetWindowText(s);
	m_TodayNoteStatic.SetClicked(TRUE);


	m_TimeStatic.Create(L"", WS_CHILD|WS_VISIBLE, CRect(262, 25, 262+194, 25+84), this, IDC_STATIC_TIME);
	m_TimeStatic.SetBackRGB(Data::g_allFramBackRGB[Data::g_skinstyle]);

	m_FrameStatic.Create(CRect(0, 0, 480, 204), this, 0);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//add function
//获取未接电话个数
int	CMainDlg::GetMissedTelInfo()
{
	//todo :: Missed finish!
	return 3;
}
//获取留言电话个数
int	CMainDlg::GetRecordedTelInfo()
{
	//todo :: Missed finish!
	return 2;
}

//获取是否设置启动防火墙
BOOL CMainDlg::GetCallWallInfo()
{
	//todo :: Missed finish!
	return TRUE;
}

void CMainDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here

	// Do not call CDialog::OnPaint() for painting message
}

void CMainDlg::OnTimer(UINT nIDEvent)
{
	if(TIMER_MAINDLG == nIDEvent)
	{
	
	}

	CDialog::OnTimer(nIDEvent);
}

void CMainDlg::OnChangeWindow(WPARAM w, LPARAM l)
{
	if(l == SW_HIDE)
	{
		if((CWnd *)w != m_mainmenuDlg_)
		{
			((CWnd *)w)->ShowWindow(SW_HIDE);
			m_mainmenuDlg_->SetVideo();
			m_mainmenuDlg_->ShowWindow(SW_SHOW);
		}
	}
	else if(l == SW_SHOW)
	{
		m_mainmenuDlg_->ShowWindow(SW_HIDE);
		if(w == 0)	//显示桌面影院
		{
			m_mainVideoDlg_->SetVideo();
			m_mainVideoDlg_->ShowWindow(SW_SHOW);
		}
		else if(w == 1)	//显示Photo
		{
			m_mainPhotoDlg_->SetPhoto();
			m_mainPhotoDlg_->ShowWindow(SW_SHOW);
		}
		else if(w == 2)	//显示Mp3
		{
			m_mainMp3Dlg_->SetMP3();
			m_mainMp3Dlg_->ShowWindow(SW_SHOW);
		}
		else if(w == 3)		//显示日历
		{
			m_mainLunarderDlg_->ShowWindow(SW_SHOW);
		}
		else if(w == 4)
		{
			m_mainCalucaterDlg_->ShowWindow(SW_SHOW);
		}
		else if(w == 5)	//显示屏保
		{
			m_mainScreenSaveDlg_->ShowWindow(SW_SHOW);
		}
	}
}

void CMainDlg::OnPlayPhoto(WPARAM w, LPARAM l)
{
	CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg *)theApp.m_pMainWnd;
	if(m_mainPhotoDlg_->IsWindowVisible())
	{	
		if(!main->player_->type_)		//图片
		{
			if(w == 2)	//全屏播放
			{
				if(main->player_->isPlaying_)
				{
					m_mainPhotoDlg_->SetAllScreenPlayer(TRUE);
				}
			}
			else if(w == 3) //恢复播放
			{
				if(main->player_->isPlaying_)
				{
					m_mainPhotoDlg_->SetAllScreenPlayer(FALSE);
				}
			}
		}
	}
	else if(m_mainScreenSaveDlg_->IsWindowVisible())
	{
		if(w == 3)	//退出屏保
		{
			if(main->player_->isPlaying_)
			{
				m_mainScreenSaveDlg_->OnExit();
			}
		}
	}
}

void CMainDlg::OnPlayVideo(WPARAM w, LPARAM l)
{
	if(m_mainmenuDlg_->IsWindowVisible() || m_mainVideoDlg_->IsWindowVisible() || m_mainMp3Dlg_->IsWindowVisible())
	{
		CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg *)theApp.m_pMainWnd;
		if(main->player_->type_)		//视频
		{
			if(w == 0)	//暂停视频
			{
				if(main->player_->isPlaying_)	//正在播放
				{
					if(main->player_->owner_ == m_mainMp3Dlg_->playerDlg_ && l == 0 )  //MP3 & (l = 1)不是来电
						return;
					main->player_->ExitPlayer();
				}
			}
			else if(w ==  1)//播放视频
			{
				if(!main->player_->isPlaying_)	
				{
					main->player_->InitPlayer();
				}
			}
			else if(w == 2)	//全屏播放
			{
				SetCtrlEnable(FALSE);
				main->player_->SetAllScreenPlayer(TRUE);
			}
			else if(w == 3)	//退出全屏
			{
				main->player_->SetAllScreenPlayer(FALSE);
				SetCtrlEnable(TRUE);
				m_mainVideoDlg_->SetCtrlEnable(TRUE);
			}
			else if(w == 4) //文件开始播放
			{
				//有问题
				if(m_mainVideoDlg_->IsWindowVisible() )//|| m_mainVideoDlg_->IsWindowVisible())	//视频
				{
					m_mainVideoDlg_->StartOpenNewFile();
				}
				else if(m_mainMp3Dlg_->IsWindowVisible())
				{
					m_mainMp3Dlg_->StartOpenNewFile();
				}
				/*
				else if() //MP3
					;
					*/
			}
		}
	}
}