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
#include "sip.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CScreenSaveDlg dialog
#define SCREENSAVE_TIMER		5*1000

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
	
	//((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->SwitchPanel_(0);   //闹铃等提示框在屏保的后面了

	/*
	if(theApp.inputDlg_->IsWindowVisible())  //hide 关闭输入法
	{
		theApp.inputDlg_->hide();   
	}
	*/
	SipShowIM(SIPF_OFF);
	m_passwndEdit.ShowWindow(SW_HIDE);
//	m_warningstatic.ShowWindow(SW_HIDE);

	::PostMessage(((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pMainDlg->m_firewalDlg_->m_cmbTime.m_Combo.m_hWnd, CB_SHOWDROPDOWN,0,0);  //如果commbox打开，让commobox隐藏
	::PostMessage(((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pContactGroupDlg->m_cmbSoundTip.m_Combo.m_hWnd, CB_SHOWDROPDOWN,0,0);
	
	::PostMessage(((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pContactNewDlg->m_cmbType.m_Combo.m_hWnd, CB_SHOWDROPDOWN,0,0);
	::PostMessage(((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pContactNewDlg->m_cmbGroup.m_Combo.m_hWnd, CB_SHOWDROPDOWN,0,0);
	::PostMessage(((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pMainDlg->m_mainLunarderDlg_->m_cmbRing.m_Combo.m_hWnd, CB_SHOWDROPDOWN,0,0);
//	::PostMessage(((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pContactDlg->m_pSimImportDlg->m_cmbGroup.m_Combo.m_hWnd, CB_SHOWDROPDOWN,0,0);
	::PostMessage(((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pMainDlg->m_pWebDialog->m_cmbURL.m_Combo.m_hWnd, CB_SHOWDROPDOWN,0,0);
	
//	::PostMessage(((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_cmbRing.m_Combo.m_hWnd, CB_SHOWDROPDOWN,0,0);
	::PostMessage(((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_cmbRingTimes.m_Combo.m_hWnd, CB_SHOWDROPDOWN,0,0);
	::PostMessage(((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_cmbAutoRecoedeTimes.m_Combo.m_hWnd, CB_SHOWDROPDOWN,0,0);
	::PostMessage(((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_cmbWaitTime.m_Combo.m_hWnd, CB_SHOWDROPDOWN,0,0);
//	::PostMessage(((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_cmbSoundSavePath.m_Combo.m_hWnd, CB_SHOWDROPDOWN,0,0);
	
//	::PostMessage(((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_cmbRingVolume.m_Combo.m_hWnd, CB_SHOWDROPDOWN,0,0);
//	::PostMessage(((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_cmbSystemVolume.m_Combo.m_hWnd, CB_SHOWDROPDOWN,0,0);
	::PostMessage(((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_cmbBlackLightWaitTime.m_Combo.m_hWnd, CB_SHOWDROPDOWN,0,0);

	if(type > 1 || type < 0)
		type = 1;
	type_ = type;
	playerDlg_->ReSetWindowsRect(CRect(0, 0, 800, 480));
	if(type_)   //图片
	{
		::SetCursorPos(799, 479);
		playerDlg_->SetParam("", mtImage, TRUE);
		SetSaveScreenPhoto_();
		playerDlg_->ShowWindow(SW_SHOW);
	}
	else
	{
		SetTimer(IDT_SCREENSAVE_TIME, SCREENSAVE_TIMER, NULL);
		OnTimer(IDT_SCREENSAVE_TIME);
		playerDlg_->StopTimer();
		playerDlg_->ShowWindow(SW_HIDE);
	}
	if(Redraw)
		Invalidate();
}

void CScreenSaveDlg::SetSaveScreenPhoto_()
{
	if(playerDlg_->SetImageListForScreenSave("/flashdrv/my_photo/screensave.pls"))
	{
		
	}
	else
	{
//		m_warningstatic.ShowWindow(SW_SHOW);
	}
}

BEGIN_MESSAGE_MAP(CScreenSaveDlg, CDialog)
	//{{AFX_MSG_MAP(CScreenSaveDlg)
	ON_WM_ERASEBKGND()
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
	
	// TODO: Add extra initialization here
		//播放器

	CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	playerDlg_ = new CPlayerDlg(main->playerimage_);

	playerDlg_->Create(CPlayerDlg::IDD, this);
	//SetSaveScreenType(0);

// 	m_static.Create(L"", WS_VISIBLE|WS_CHILD, CRect(0, 0, 200, 64), this);
// 	m_static.SetFontSize(64);
// 	m_static1.Create(L"", WS_VISIBLE|WS_CHILD, CRect(0, 0, 200, 32), this);
// 	m_static1.SetFontSize(24);
// 	m_static.SetColor(RGB(0, 0, 0), RGB(0, 0, 0));
// 	m_static1.SetColor(RGB(0, 0, 0), RGB(0, 0, 0));

//	m_warningstatic.Create(L"无屏保图片,请在电子相册中设定屏保图片!", WS_CHILD, CRect(200, 220, 600, 260), this);
//	m_warningstatic.SetFontSize(28);
//	m_warningstatic.SetColor(RGB(255, 255, 255), RGB(128, 128, 128));

	m_passwndEdit.Create(WS_CHILD|WS_VISIBLE|ES_PASSWORD|ES_CENTER, CRect(300, 225, 500, 255), this, IDC_EDIT_SCREENSAVE);
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

void CScreenSaveDlg::GetPassWord()
{
	std::string strTemp = ((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->m_pSettingDlg->m_pSetting->screenSaverPassword();
	m_password = strTemp.c_str();
}

BOOL CScreenSaveDlg::IsHaveScreenPwnd()
{
	//lxz 不使用密码
	return FALSE;

	bool ret = ((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->m_pSettingDlg->m_pSetting->isUseScreenSaverPassword();
	return ret;
}

void CScreenSaveDlg:: OnExit()
{
	GetPassWord();
	((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->playerimage_->isActiveMode_ = 1;  //退出屏保
	if(!IsHaveScreenPwnd())
	{
		m_passwndEdit.ShowWindow(SW_HIDE);
		playerDlg_->StopTimer();
		playerDlg_->ShowWindow(SW_HIDE);
		((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->m_pMainDlg->SendMessage(WM_CHANGEWINDOW, (WPARAM)this, (LPARAM)SW_HIDE);
	//	ShowWindow(SW_HIDE);
	}
	else
	{
		KillTimer(IDT_SCREENSAVE_TIME);
		playerDlg_->StopTimer();
		playerDlg_->ShowWindow(SW_HIDE);

		CRect rt = CRect(220, 225, 290, 255);
		m_static.SetColor(RGB(0xFF, 0, 0), RGB(0, 0, 0));
		m_static.MoveWindow(rt.left, rt.top, rt.Width(), rt.Height(), FALSE);
		m_static.SetFontSize(24);
		CString str = Data::LanguageResource::Get(Data::RI_SCREENSAVE_PASSWORDINPUT).c_str();
		m_static.SetWindowText(L"请输入密码");
		SetDlgItemText(IDC_EDIT_SCREENSAVE, L"");

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
			HDC dc = ::GetDC(m_hWnd);
			::SetBkMode(dc, TRANSPARENT);
			CRect rect_;
			GetClientRect(&rect_);
			CBrush bBr = RGB(0, 0, 0); 
			::FillRect(dc, &rect_, (HBRUSH)bBr.m_hObject);

			SYSTEMTIME curtime;
			GetLocalTime(&curtime);
			char time[24];
			sprintf(time, "%02d:%02d:%02d", curtime.wHour, curtime.wMinute, curtime.wSecond);
			CString s = time;

			int nx = rand();
			int ny = rand();
			int nc = rand();
			
			nx = nx % (800-210);
			ny = ny % (480-96);
			::SetTextColor(dc, nc%0xFFFFFF);
			CRect rt = CRect(nx, ny+26, nx+210, ny+26+64);
			CFont m_Font;
			VERIFY(m_Font.CreateFont(
				64,                        // nHeight
				0,                         // nWidth
				0,                         // nEscapement
				0,                         // nOrientation
				FW_NORMAL,                 // nWeight
				FALSE,                     // bItalic
				FALSE,                     // bUnderline
				0,                         // cStrikeOut
				ANSI_CHARSET,              // nCharSet
				OUT_DEFAULT_PRECIS,        // nOutPrecision
				CLIP_DEFAULT_PRECIS,       // nClipPrecision
				DEFAULT_QUALITY,           // nQuality
				DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
			 _T("宋体")));                 // lpszFacename

			HFONT oldFont = (HFONT)::SelectObject(dc, m_Font.m_hObject);
			::DrawText(dc, s, s.GetLength(), &rt, DT_WORDBREAK | DT_LEFT);
			::SelectObject(dc, oldFont);


			int nWeekDay  = Logical::LunarderDate::WeekDay(curtime.wYear, curtime.wMonth, curtime.wDay);
			sprintf(time, "%02d-%02d-%02d %s", curtime.wYear, curtime.wMonth, curtime.wDay, Data::LanguageResource::Get(Data::RI_COMN_SUNSTC+nWeekDay).c_str());
 			s = time;

			rt = CRect(nx, ny, nx+210, ny+28);
			CFont m_Font1;
			VERIFY(m_Font1.CreateFont(
				24,                        // nHeight
				0,                         // nWidth
				0,                         // nEscapement
				0,                         // nOrientation
				FW_NORMAL,                 // nWeight
				FALSE,                     // bItalic
				FALSE,                     // bUnderline
				0,                         // cStrikeOut
				ANSI_CHARSET,              // nCharSet
				OUT_DEFAULT_PRECIS,        // nOutPrecision
				CLIP_DEFAULT_PRECIS,       // nClipPrecision
				DEFAULT_QUALITY,           // nQuality
				DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
				_T("宋体")));                 // lpszFacename
			
			oldFont = (HFONT)::SelectObject(dc, m_Font1.m_hObject);
			::DrawText(dc, s, s.GetLength(), &rt, DT_WORDBREAK | DT_LEFT);
			::SelectObject(dc, oldFont);

			::ReleaseDC(m_hWnd, dc);

			/*

			SYSTEMTIME curtime;
			GetLocalTime(&curtime);
			char time[24];
			sprintf(time, "%02d:%02d:%02d", curtime.wHour, curtime.wMinute, curtime.wSecond);
			CString s = time;


			int nx = rand();
			int ny = rand();
			int nc = rand();
			
			nx = nx % (800-210);
			ny = ny % (480-96);
			m_static.SetWindowText(s);
			m_static.SetColor(nc%0xFFFFFF, RGB(0, 0, 0));
			m_static.MoveWindow(nx, ny+26, 210, 64, FALSE);
		//	m_static.Invalidate()//OnDraw(CRect(nx, ny+26, nx+210, ny+26+64), s);
			
  			int nWeekDay  = Logical::LunarderDate::WeekDay(curtime.wYear, curtime.wMonth, curtime.wDay);
  			sprintf(time, "%02d-%02d-%02d %s", curtime.wYear, curtime.wMonth, curtime.wDay, Data::LanguageResource::Get(Data::RI_COMN_SUNSTC+nWeekDay).c_str());
 			s = time;
			m_static1.SetWindowText(s);
			m_static1.SetColor(nc%0xFFFFFF, RGB(0, 0, 0));
		//	m_static1.OnDraw(CRect(nx, ny, nx+210, ny+28), s);
			m_static1.MoveWindow(nx, ny, 200, 28, FALSE);
			*/
		}
	}
	else if(IDT_SCREENSAVE_INPUTTIME == nIDEvent)
	{
		SipShowIM(SIPF_OFF);
		KillTimer(IDT_SCREENSAVE_INPUTTIME);
		SetSaveScreenType(type_, TRUE);
	}

	CDialog::OnTimer(nIDEvent);
}

BOOL CScreenSaveDlg::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
// 	CRect rt;
// 	GetClientRect(&rt);
// 	pDC->FillSolidRect(&rt, RGB(0, 0, 0));

	return TRUE;
//	return CDialog::OnEraseBkgnd(pDC);
}


void CScreenSaveDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	if(type_ == 0)
	{
		CRect rt;
		GetClientRect(&rt);
		dc.FillSolidRect(&rt, RGB(0, 0, 0));
	}
	
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
	KillTimer(IDT_SCREENSAVE_INPUTTIME);
	CString sTxt;
	GetDlgItemText(IDC_EDIT_SCREENSAVE, sTxt);
	if(sTxt == m_password || m_password == "")		//验证正确 退出屏保
	{
		SipShowIM(SIPF_OFF);
		m_passwndEdit.ShowWindow(SW_HIDE);
		playerDlg_->StopTimer();
		playerDlg_->ShowWindow(SW_HIDE);
		((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->m_pMainDlg->SendMessage(WM_CHANGEWINDOW, (WPARAM)this, (LPARAM)SW_HIDE);
	//	ShowWindow(SW_HIDE);
		return;
	}
	else				//错误继续，验证
	{
		if(sTxt.GetLength() >= 6)
		{
			SetDlgItemText(IDC_EDIT_SCREENSAVE, L"");
		}
	}
	SetTimer(IDT_SCREENSAVE_INPUTTIME, 30*1000, NULL);
}

void CScreenSaveDlg::OnOutEvent(WPARAM w, LPARAM l)
{
	if(l == 0)
	{
		if(playerDlg_->player_->isPlaying_ && playerDlg_->player_->owner_ == playerDlg_)
		{
			((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->SetScreenSaveTimer();
			playerDlg_->StopTimer();
			playerDlg_->ShowWindow(SW_HIDE);
			playerDlg_->player_->ExitPlayer(TRUE);
//			theApp.PostThreadMessage(WM_LBUTTONDOWN, 0, 0);
//			ShowWindow(SW_HIDE);
			OnExit();
		}
		else if(IsWindowVisible())
			OnExit();
	}
}
