// ScreenSaveDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ScreenSaveDlg.h"
#include "../Data/LanguageResource.h"
#include "../Logical/LunarderDate.h"
#include "../resource.h"
#include "../multimediaphone.h"
#include "../Data/LanguageResource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CString g_screenpwd = "1234";

/////////////////////////////////////////////////////////////////////////////
// CScreenSaveDlg dialog
#define SCREENSAVE_TIMER		10*1000

CScreenSaveDlg::CScreenSaveDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CScreenSaveDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CScreenSaveDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	type_ = 0;
}


void CScreenSaveDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CScreenSaveDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

void CScreenSaveDlg::SetSaveScreenType(int type, BOOL Redraw)
{
	type_ = type;
	playerDlg_->ReSetWindowsRect(CRect(0, 0, 479, 233));
	if(type_)
	{
		playerDlg_->SetParam( "/flashdrv/apple192.avi", FALSE, TRUE);
		SetSaveScreenPhoto_();
		playerDlg_->ShowWindow(SW_SHOW);
	}
	else
	{
		SetTimer(IDT_SCREENSAVE_TIME, SCREENSAVE_TIMER, NULL);
		OnTimer(IDT_SCREENSAVE_TIME);
		playerDlg_->ShowWindow(SW_HIDE);
	}
	if(Redraw)
		Invalidate();
}

void CScreenSaveDlg::SetSaveScreenPhoto_()
{
	playerDlg_->SetImageListForScreenSave("/flashdrv/my_photo/screensave.pls");
}

BEGIN_MESSAGE_MAP(CScreenSaveDlg, CDialog)
	//{{AFX_MSG_MAP(CScreenSaveDlg)
//	ON_WM_ERASEBKGND()
	ON_EN_CHANGE(IDC_EDIT_SCREENSAVE, OnChangeEdit1)
	ON_WM_LBUTTONDOWN()
	ON_WM_TIMER()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScreenSaveDlg message handlers

BOOL CScreenSaveDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
		//播放器
	playerDlg_ = new CPlayerDlg();
	playerDlg_->Create(CPlayerDlg::IDD, this);
	//SetSaveScreenType(0);
	m_static.Create(L"", WS_VISIBLE|WS_CHILD, CRect(0, 0, 160, 64), this);
	m_static.SetFontSize(64);
	m_static1.Create(L"", WS_VISIBLE|WS_CHILD, CRect(0, 0, 160, 24), this);
	m_static1.SetFontSize(18);
	m_static.SetColor(RGB(0, 0, 0), RGB(0, 0, 0));
	m_static1.SetColor(RGB(0, 0, 0), RGB(0, 0, 0));
	m_passwndEdit.Create(WS_CHILD|WS_VISIBLE|ES_PASSWORD, CRect(220, 100, 290, 120), this, IDC_EDIT_SCREENSAVE);
	m_passwndEdit.ShowWindow(SW_HIDE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CScreenSaveDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	OnExit();
	CDialog::OnLButtonDown(nFlags, point);
}

BOOL IsHaveScreenPwnd()
{
	return TRUE;
}

void CScreenSaveDlg:: OnExit()
{
	if(!IsHaveScreenPwnd())
	{
		m_passwndEdit.ShowWindow(SW_HIDE);
		playerDlg_->ShowWindow(SW_HIDE);
		GetParent()->SendMessage(WM_CHANGEWINDOW, (WPARAM)this, (LPARAM)SW_HIDE);
	}
	else
	{
		KillTimer(IDT_SCREENSAVE_TIME);
		playerDlg_->StopTimer();
		playerDlg_->ShowWindow(SW_HIDE);

		CRect rt = CRect(210, 70, 320, 99);
		m_static.SetColor(RGB(0xFF, 0, 0), RGB(0, 0, 0));
		m_static.MoveWindow(rt.left, rt.top, rt.Width(), rt.Height(), FALSE);
		m_static.SetFontSize(16);
		CString str = Data::LanguageResource::Get(Data::RI_SCREENSAVE_PASSWORDINPUT).c_str();
		m_static.SetWindowText(L"请输入密码:");

		m_static1.MoveWindow(0, 0, 0, 0, FALSE);
		m_static1.SetColor(RGB(0xFF, 0, 0), RGB(0, 0, 0));

		m_passwndEdit.ShowWindow(SW_SHOW);
	}
}
	
void CScreenSaveDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(IDT_SCREENSAVE_TIME == nIDEvent)
	{
		if(type_ == 0)
		{
			SYSTEMTIME curtime;
			GetLocalTime(&curtime);
			char time[24];
			sprintf(time, "%02d:%02d", curtime.wHour, curtime.wMinute);
			CString s = time;

			int nx = rand();
			int ny = rand();
			int nc = rand();
			
			nx = nx % (480-160);
			ny = ny % (234-88);
			m_static.SetFontSize(64);
			m_static.SetWindowText(s);
			m_static.SetColor(nc%0xFFFFFF, RGB(0, 0, 0));
			m_static.MoveWindow(nx, ny+24, 160, 64, FALSE);

			int nWeekDay  = Logical::LunarderDate::WeekDay(curtime.wYear, curtime.wMonth, curtime.wDay);
			sprintf(time, "%02d-%02d-%02d %s", curtime.wYear, curtime.wMonth, curtime.wDay, Data::LanguageResource::Get(Data::RI_COMN_SUNSTC+nWeekDay).c_str());
			s = time;
			m_static1.SetWindowText(s);
			m_static1.SetColor(nc%0xFFFFFF, RGB(0, 0, 0));
			m_static1.MoveWindow(nx, ny, 160, 24, FALSE);
		}		
	}
	CDialog::OnTimer(nIDEvent);
}

BOOL CScreenSaveDlg::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	CRect rt;
	GetClientRect(&rt);
	pDC->FillSolidRect(&rt, RGB(0, 0, 0));
	return CDialog::OnEraseBkgnd(pDC);
}


void CScreenSaveDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CRect rt;
	GetClientRect(&rt);
	dc.FillSolidRect(&rt, RGB(0, 0, 0));

	//CDialog::OnPaint();
	
	// TODO: Add your message handler code here
	/*
	if(type_ == 0)
	{
		
		CRect rt;
		GetClientRect(&rt);
	
		//CDC *pDC = GetDC();
		dc.FillSolidRect(&rt, RGB(0, 0, 0));
		//ReleaseDC(pDC);

		SYSTEMTIME curtime;
		GetLocalTime(&curtime);
		char time[24];
		sprintf(time, "%02d:%02d", curtime.wHour, curtime.wMinute);
		CString s = time;

		int nx = rand();
		int ny = rand();
		int nc = rand();
		
		nx = nx % (480-80);
		ny = ny % (234-32);
		m_static.SetWindowText(s);
		m_static.SetColor(nc%0xFFFFFF, RGB(0, 0, 0));
		m_static.MoveWindow(nx, ny, 80, 32, FALSE);
	}
	*/
	// Do not call CDialog::OnPaint() for painting messages
}

void CScreenSaveDlg::OnChangeEdit1()
{
	CString sTxt;
	GetDlgItemText(IDC_EDIT_SCREENSAVE, sTxt);
	if(sTxt == g_screenpwd)		//验证正确 退出屏保
	{
		
		theApp.inputDlg_->hide();  //hide
	
		SetDlgItemText(IDC_EDIT_SCREENSAVE, L"");
		m_passwndEdit.ShowWindow(SW_HIDE);
		playerDlg_->ShowWindow(SW_HIDE);
		GetParent()->SendMessage(WM_CHANGEWINDOW, (WPARAM)this, (LPARAM)SW_HIDE);
		return;
	}
	else				//错误继续，验证
	{
		if(sTxt.GetLength() >= 6)
		{
			SetDlgItemText(IDC_EDIT_SCREENSAVE, L"");
		}
	}
	//SetTimer(IDT_SCREENSAVE_INPUTTIME, 20)
}