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
	m_isScreenPlay = FALSE;
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
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	SipShowIM(SIPF_OFF);
	m_passwndEdit.ShowWindow(SW_HIDE);

	::PostMessage(main->m_pMainDlg->m_firewalDlg_->m_cmbTime.m_pCombo->m_hWnd, CB_SHOWDROPDOWN,0,0);  //如果commbox打开，让commobox隐藏
	::PostMessage(main->m_pContactGroupDlg->m_cmbSoundTip.m_pCombo->m_hWnd, CB_SHOWDROPDOWN,0,0);
	
	::PostMessage(main->m_pContactNewDlg->m_cmbType.m_pCombo->m_hWnd, CB_SHOWDROPDOWN,0,0);
	::PostMessage(main->m_pContactNewDlg->m_cmbGroup.m_pCombo->m_hWnd, CB_SHOWDROPDOWN,0,0);
	::PostMessage(main->m_pMainDlg->m_mainLunarderDlg_->m_cmbRing.m_pCombo->m_hWnd, CB_SHOWDROPDOWN,0,0);
	::PostMessage(main->m_pMainDlg->m_pWebDialog->m_cmbURL.m_pCombo->m_hWnd, CB_SHOWDROPDOWN,0,0);
	
	::PostMessage(main->m_pSettingDlg->m_cmbRingTimes.m_pCombo->m_hWnd, CB_SHOWDROPDOWN,0,0);
	::PostMessage(main->m_pSettingDlg->m_cmbAutoRecoedeTimes.m_pCombo->m_hWnd, CB_SHOWDROPDOWN,0,0);
	::PostMessage(main->m_pSettingDlg->m_cmbWaitTime.m_pCombo->m_hWnd, CB_SHOWDROPDOWN,0,0);
	::PostMessage(main->m_pSettingDlg->m_cmbBlackLightWaitTime.m_pCombo->m_hWnd, CB_SHOWDROPDOWN,0,0);

	if(type > 1 || type < 0)
		type = 1;
	type_ = type;
	playerDlg_->ReSetWindowsRect(CRect(0, 0, 800, 480));
	if(type_)   //图片
	{
		m_isScreenPlay = TRUE;
		::SetCursorPos(799, 479);
		main->m_pMainDlg->m_p3GHomePicDlg->SetScreenPicture();
		playerDlg_->SetParam("", mtImage, TRUE);
		SetSaveScreenPhoto_();
		playerDlg_->ShowWindow(SW_SHOW);
		playerDlg_->player_->PlayerImage();
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
	bool ret = ((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->m_pSettingDlg->m_pSetting->isUseScreenSaverPassword();
	return ret;
}

BOOL IsShowSreenSaveInput()
{
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg *)theApp.m_pMainWnd;
	if(main->m_pMainDlg->m_mainScreenSaveDlg_->IsHaveScreenPwnd())
	{
		if(!main->m_pPasswordDlg->IsWindowVisible() && main->m_pMainDlg->m_mainScreenSaveDlg_->m_isScreenPlay)
		{
			std::string strTemp = main->m_pSettingDlg->m_pSetting->screenSaverPassword();
			main->m_pPasswordDlg->SetOldPassWord((char *)strTemp.c_str());
			main->m_pPasswordDlg->SetHWnd(main->m_pMainDlg->m_mainScreenSaveDlg_->GetSafeHwnd());
 			main->m_pPasswordDlg->ShowWindow_(SW_SHOW);	
			return TRUE;
		}
		
	}
	return FALSE;
}

void CScreenSaveDlg:: OnExit()
{
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg *)theApp.m_pMainWnd;
	GetPassWord();
	main->playerimage_->isActiveMode_ = 1;  //退出屏保
	if(!IsHaveScreenPwnd())
	{
		m_passwndEdit.ShowWindow(SW_HIDE);
		m_isScreenPlay = FALSE;
		ShowWindow(SW_HIDE);
	}
	else
	{
		m_passwndEdit.ShowWindow(SW_HIDE);
		
		main->m_pPasswordDlg->SettingType(CHECK_SCREENPASSWORD);
		std::string strTemp = main->m_pSettingDlg->m_pSetting->screenSaverPassword();
		main->m_pPasswordDlg->SetOldPassWord((char *)strTemp.c_str());
		main->m_pPasswordDlg->SetHWnd(this->GetSafeHwnd());
		main->m_pPasswordDlg->ShowWindow_(SW_SHOW);
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
		}
	}
	else if(IDT_SCREENSAVE_INPUTTIME == nIDEvent)
	{
		SipShowIM(SIPF_OFF);
		KillTimer(IDT_SCREENSAVE_INPUTTIME);
		SetSaveScreenType(type_, TRUE);
	}
	else if(1001 == nIDEvent)		//lxz 20100703 屏保
	{
		KillTimer(1001);
		playerDlg_->player_->ExitPlayer(TRUE);
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
			
			SetTimer(1001, 10, NULL);		//lxz 20100703 屏保
			//playerDlg_->player_->ExitPlayer(TRUE);
			
			OnExit();
		}
		else if(IsWindowVisible())
			OnExit();
	}
}

LRESULT CScreenSaveDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg *)theApp.m_pMainWnd;
	switch(message)
	{
	case CHECK_SCREENPASSWORD:
		if(1 == wParam)
		{
			ShowWindow(SW_HIDE);
			m_isScreenPlay = FALSE;
		}
		else if(0 == wParam)
		{
			playerDlg_->SetParam("", mtImage, TRUE);
			playerDlg_->ShowWindow(SW_SHOW);
			playerDlg_->player_->PlayerImage();
			m_isScreenPlay = TRUE;
		}
		break;
	default:
		break;
	}
	return CDialog::WindowProc(message, wParam, lParam);
}