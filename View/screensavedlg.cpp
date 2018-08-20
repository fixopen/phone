// ScreenSaveDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ScreenSaveDlg.h"
#include "../Data/LanguageResource.h"
#include "../Logical/LunarderDate.h"
#include "../resource.h"
#include "../multimediaphone.h"
#include "../Data/LanguageResource.h"
#include "../MultimediaPhoneDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

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
	//关闭输入法
	KeyBoardOff();
	
	m_passwndEdit.SetWindowText(_T(""));
	m_static.SetWindowText(_T(""));
	
	m_passwndEdit.ShowWindow(SW_HIDE);
	
	if(type > 1 || type < 0)
		type = 1;
	type_ = type;

 		SetTimer(IDT_SCREENSAVE_TIME, SCREENSAVE_TIMER, NULL);
 		OnTimer(IDT_SCREENSAVE_TIME);

	if(Redraw)
		Invalidate();
}

void CScreenSaveDlg::SetSaveScreenPhoto_()
{
//	playerDlg_->SetImageListForScreenSave("/flashdrv/my_photo/screensave.pls");
}

BEGIN_MESSAGE_MAP(CScreenSaveDlg, CDialog)
	//{{AFX_MSG_MAP(CScreenSaveDlg)
//	ON_WM_ERASEBKGND()
	ON_EN_CHANGE(IDC_EDIT_SCREENSAVE, OnChangeEdit1)
	ON_WM_LBUTTONDOWN()
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_MESSAGE(WM_OUTEVENT, OnOutEvent)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScreenSaveDlg message handlers

BOOL CScreenSaveDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	IsShowScreenSaveDlg=false;
	
	// TODO: Add extra initialization here
		//播放器
// 	playerDlg_ = new CPlayerDlg();
// 	playerDlg_->Create(CPlayerDlg::IDD, this);
	m_static.Create(L"", /*WS_VISIBLE|*/WS_CHILD, CRect(0, 0, 150, 64*VHEIGHT), this);
	m_static.SetFontSize(64);
	m_static1.Create(L"", /*WS_VISIBLE|*/WS_CHILD, CRect(0, 0, 150, 24*VHEIGHT), this);
	m_static1.SetFontSize(18);
	m_static.SetColor(RGB(0, 0, 0), RGB(0, 0, 0));
	m_static1.SetColor(RGB(0, 0, 0), RGB(0, 0, 0));
	m_passwndEdit.Create(WS_CHILD|/*WS_VISIBLE|*/ES_PASSWORD|ES_AUTOHSCROLL, CRect(200, 100, 290, 120), this, IDC_EDIT_SCREENSAVE);
	m_passwndEdit.SetLimitText(6);
	m_passwndEdit.ShowWindow(SW_HIDE);

// 	//defect 128
	m_passwndEdit.SetFocus();
	m_passwndEdit.ShowCaret();
// 	//=================
	SetTimer(IDT_SCREENSAVE_TIME, SCREENSAVE_TIMER, NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CScreenSaveDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	OnExit();
	CDialog::OnLButtonDown(nFlags, point);
}

void CScreenSaveDlg::GetPassWord()
{
	std::string strTemp = ((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->m_pSettingDlg->m_pSetting->screenSaverPassword();
	m_password = Util::StringOp::ToCString(strTemp);
}

BOOL CScreenSaveDlg::IsHaveScreenPwnd()
{
	bool ret = ((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->m_pSettingDlg->m_pSetting->isUseScreenSaverPassword();
	return ret;
}

void CScreenSaveDlg:: OnExit()
{
	KillTimer(IDT_SCREENSAVE_INPUTTIME);
	SetTimer(IDT_SCREENSAVE_INPUTTIME, SCREENSAVE_TIMER, NULL);

	GetPassWord();
	if(!IsHaveScreenPwnd() || (m_password == ""))
	{
		ShowWindow(SW_HIDE);
		m_passwndEdit.ShowWindow(SW_HIDE);
		m_static.ShowWindow(SW_HIDE);
		m_passwndEdit.SetWindowText(_T(""));
		m_static.SetWindowText(_T(""));
		m_static1.SetWindowText(_T(""));
	}
	else
	{
		KillTimer(IDT_SCREENSAVE_TIME);
		CRect rt = CRect(210, 70, 320, 99);
		m_static.SetColor(RGB(0xFF, 0, 0), RGB(0, 0, 0));
		m_static.MoveWindow(rt.left, rt.top, rt.Width(), rt.Height(), FALSE);
		m_static.SetFontSize(16);
		CString str = Util::StringOp::ToCString(Data::LanguageResource::Get(Data::RI_SCREENSAVE_PASSWORDINPUT));
		m_static.SetWindowText(str);
		m_static1.MoveWindow(0, 0, 0, 0, FALSE);
		m_static1.SetColor(RGB(0xFF, 0, 0), RGB(0, 0, 0));
		m_static.ShowWindow(SW_SHOW);
		m_passwndEdit.ShowWindow(SW_SHOW);
		m_passwndEdit.SetWindowText(_T(""));
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

			CTime tRandTime=CTime::GetCurrentTime();   
			unsigned int iRandTime=tRandTime.GetTime();   
			srand((unsigned int)iRandTime); 

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
			m_static.ShowWindow(SW_SHOW);
			m_static1.ShowWindow(SW_SHOW);
		}		
	}
	else if(IDT_SCREENSAVE_INPUTTIME == nIDEvent)
	{
		KillTimer(IDT_SCREENSAVE_INPUTTIME);

		KeyBoardOff();
		
		KillTimer(IDT_SCREENSAVE_TIME);
		SetTimer(IDT_SCREENSAVE_TIME, SCREENSAVE_TIMER, NULL);
		
		m_passwndEdit.ShowWindow(SW_HIDE);
		// 		//defect 128
		m_passwndEdit.SetWindowText(_T(""));
// 		//===============
		m_static.ShowWindow(SW_HIDE);
		m_static1.ShowWindow(SW_HIDE);
	}
	else if(nIDEvent == 100)
	{
		KillTimer(100);

		if((((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pFSM->getCurrentState() != CMultimediaPhoneDlg::tsHangOff))
		{  
			BOOL DetectDIR(TCHAR *sDir);
			((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pTelephoneDlg->ShowWindow_(TRUE);
		}
	}

	CDialog::OnTimer(nIDEvent);
}
BOOL CScreenSaveDlg::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
/*	CRect rt;
	GetClientRect(&rt);
	pDC->FillSolidRect(&rt, RGB(0, 0, 0));*/
//	return CDialog::OnEraseBkgnd(pDC);
	return FALSE;
}


void CScreenSaveDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	CRect rt;

	GetClientRect(&rt);

	dc.FillSolidRect(&rt, RGB(0, 0, 0));	
}

void CScreenSaveDlg::OnChangeEdit1()
{
	KillTimer(IDT_SCREENSAVE_INPUTTIME);

	SetTimer(IDT_SCREENSAVE_INPUTTIME, SCREENSAVE_TIMER, NULL);

	CString sTxt;

	m_passwndEdit.GetWindowText(sTxt);

	if(!sTxt.IsEmpty() && (sTxt == m_password))
	{
		KeyBoardOff();
		
		ShowWindow(SW_HIDE);
		m_passwndEdit.ShowWindow(SW_HIDE);
		// 		//defect 128
		m_passwndEdit.SetWindowText(_T(""));
		// 		//=============
		m_static.ShowWindow(SW_HIDE);

		m_static.SetWindowText(_T(""));

		m_static1.SetWindowText(_T(""));

		SetTimer(100, 10, NULL);

		return;
	}
}

void CScreenSaveDlg::OnOutEvent(WPARAM w, LPARAM l)
{	
	//defect 128
	KillTimer(IDT_SCREENSAVE_INPUTTIME);

	SetTimer(IDT_SCREENSAVE_INPUTTIME, SCREENSAVE_TIMER, NULL);

	GetPassWord();

	if(!IsHaveScreenPwnd() || (m_password == ""))
	{
		ShowWindow(SW_HIDE);

		m_passwndEdit.ShowWindow(SW_HIDE);

		m_static.ShowWindow(SW_HIDE);

		m_passwndEdit.SetWindowText(_T(""));

		m_static.SetWindowText(_T(""));

		m_static1.SetWindowText(_T(""));

		IsShowScreenSaveDlg=true;
	}
	else
	{
		KillTimer(IDT_SCREENSAVE_TIME);

		CRect rt = CRect(210, 70, 320, 99);

		m_static.SetColor(RGB(0xFF, 0, 0), RGB(0, 0, 0));

		m_static.MoveWindow(rt.left, rt.top, rt.Width(), rt.Height(), FALSE);

		m_static.SetFontSize(16);

		CString str = Util::StringOp::ToCString(Data::LanguageResource::Get(Data::RI_SCREENSAVE_PASSWORDINPUT));
	
		m_static.SetWindowText(str);

		m_static1.MoveWindow(0, 0, 0, 0, FALSE);

		m_static1.SetColor(RGB(0xFF, 0, 0), RGB(0, 0, 0));

		m_static.ShowWindow(SW_SHOW);

		m_passwndEdit.ShowWindow(SW_SHOW);

	}
	//======================
}
bool CScreenSaveDlg::getIsShowScreenSaveDlg()
{
   return IsShowScreenSaveDlg;
}

void CScreenSaveDlg::setIsShowScreenSaveDlg(bool flag)
{
   IsShowScreenSaveDlg=flag;
}