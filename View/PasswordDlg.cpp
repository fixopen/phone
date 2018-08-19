// passworddlg.cpp : implementation file
//

#include "stdafx.h"
#include "..\multimediaphone.h"
#include "passworddlg.h"
#include "../Data/LanguageResource.h"
#include "../MultimediaPhoneDlg.h"
#include "../Data/SkinStyle.h"
#include "../Util/ATCommandWarp.h"
#include "sip.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPasswordDlg dialog
std::string  g_tempPassword = "";

CPasswordDlg::CPasswordDlg(CWnd* pParent /*=NULL*/)
	: CCEDialog(CPasswordDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPasswordDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_passwordType = SETTINGPLAY_PASSWORD;
	m_password = "";
	m_nStep = 0;
	m_Owner = NULL;
	m_nCtrl = 0;
}

void CPasswordDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPasswordDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPasswordDlg, CCEDialog)
	//{{AFX_MSG_MAP(CPasswordDlg)
	ON_WM_PAINT()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_PASSWORD_CANCEL, OnButtonPasswordCancel)
	ON_MESSAGE(WM_CLICKMJPG_TOAPP, OnClickMJPG)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPasswordDlg message handlers
void CPasswordDlg::OnClickMJPG(WPARAM w, LPARAM l)
{
	switch(w)
	{
	case 1001:
		KillTimer(4);
		SettingOK();
		break;
	case 1000:
		KillTimer(4);
		OnButtonPasswordCancel();
		break;
	}
}
BOOL CPasswordDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	//int xtop = 140 ;
	int xtop = 190 ;
	int ytop = 105 ;
	int xbegin = 185 ;
	int ybegin = 94 ;
	int width = 200, height = 36;
	int internal = 19;
	
	m_edtOldPassword.Create(WS_CHILD|ES_PASSWORD, CRect(190 +xtop, 120 + ytop, 401+xtop, ytop + 149), this, IDC_SETTING_PASSWORD1);
	m_edtOldPassword.SetLimitText(6);
	m_edtOldPassword.SetLimitDiagital();

	m_edtNewPassword1.Create(WS_CHILD|ES_PASSWORD, CRect(190+xtop, ytop+156, 401+xtop, ytop+185), this, IDC_SETTING_PASSWORD2);
	m_edtNewPassword1.SetLimitText(6);
	m_edtNewPassword1.SetLimitDiagital();

	m_edtNewPassword2.Create(WS_CHILD|ES_PASSWORD, CRect(190+xtop, ytop+192, 401+xtop, ytop+221), this, IDC_SETTING_PASSWORD3);
	m_edtNewPassword2.SetLimitText(6);
	m_edtNewPassword2.SetLimitDiagital();
		
	m_MJPGList.Create(L"", WS_VISIBLE|WS_CHILD, CRect(xtop, ytop, xtop+440, ytop+270), this);
	m_MJPGList.SetCurrentLinkFile(".\\adv\\mjpg\\k5\\中文\\密码验证.xml");
	m_MJPGList.SetMJPGRect(CRect(xtop, ytop, xtop+440, ytop+270));
	
	m_MJPGList.SetUnitFont(0, font_20);
	m_MJPGList.SetUnitFont(1, font_18);
	m_MJPGList.SetUnitFont(2, font_18);
	m_MJPGList.SetUnitFont(3, font_18);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPasswordDlg::SetStaticDefaultColor(CCEStatic* cestatic)
{
	// 	cestatic->SetColor(RGB(0, 0, 0), RGB(194,235,149));
}

void CPasswordDlg::SettingOK()
{
	CString s;
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	if(m_passwordType == SETTINGPLAY_PASSWORD || m_passwordType == SETTINGSUPPER_PASSWORD || m_passwordType == SETTINGSCREEN_PASSWORD)
	{
		CString s1, s2, s3;
		GetDlgItemText(IDC_SETTING_PASSWORD1, s1);
		GetDlgItemText(IDC_SETTING_PASSWORD2, s2);
		GetDlgItemText(IDC_SETTING_PASSWORD3, s3);
		if(m_password != s1)
		{
			main->m_pWarningNoFlashDlg->SetTitle(L"旧密码输入错误!");
			main->m_pWarningNoFlashDlg->SetHWnd(this->GetSafeHwnd());
			main->m_pWarningNoFlashDlg->ShowWindow_(TRUE);
			m_edtOldPassword.SetWindowText(L"");
		}
		else if(s2 != s3)
		{
			main->m_pWarningNoFlashDlg->SetTitle(L"新密码输入错误!");
			main->m_pWarningNoFlashDlg->SetHWnd(this->GetSafeHwnd());
			main->m_pWarningNoFlashDlg->ShowWindow_(TRUE);
			m_edtNewPassword1.SetWindowText(L"");
			m_edtNewPassword2.SetWindowText(L"");
		}
		else
		{
			SipShowIM(SIPF_OFF);
			//给主窗口发送新的密码
			ShowWindow_(FALSE);
			if(m_Owner)
			{
				g_tempPassword = Util::StringOp::FromCString(s2);
				main->m_pDeleteTipDlg->SetTitle(L"确定要更改密码吗？", 0);
				main->m_pDeleteTipDlg->SetHWnd(this->GetSafeHwnd());
				main->m_pDeleteTipDlg->ShowWindow_(TRUE);
				return;
			}
		}
	}
	else if(m_passwordType == SETTINGPIN_PASSWORD)			//设置PIN码
	{
		CString s1, s2, s3;
		GetDlgItemText(IDC_SETTING_PASSWORD1, s1);
		GetDlgItemText(IDC_SETTING_PASSWORD2, s2);
		GetDlgItemText(IDC_SETTING_PASSWORD3, s3);
		if(s1 == "")
		{
			main->m_pWarningNoFlashDlg->SetTitle(L"旧密码输入错误!");
			main->m_pWarningNoFlashDlg->SetHWnd(this->GetSafeHwnd());
			main->m_pWarningNoFlashDlg->ShowWindow_(TRUE);
			m_edtOldPassword.SetWindowText(L"");
		}
		else if(s2 != s3)
		{
			main->m_pWarningNoFlashDlg->SetTitle(L"新密码输入错误!");
			main->m_pWarningNoFlashDlg->SetHWnd(this->GetSafeHwnd());
			main->m_pWarningNoFlashDlg->ShowWindow_(TRUE);
			m_edtNewPassword1.SetWindowText(L"");
			m_edtNewPassword2.SetWindowText(L"");
		}
		else
		{
			char oldpin[10], newpin[10];
			std::string s1_, s2_;
			s1_ = Util::StringOp::FromCString(s1);
			s2_ = Util::StringOp::FromCString(s2);
			sprintf(oldpin, "%s", s1_.c_str());
			sprintf(newpin, "%s", s2_.c_str());
			extern Util::ATCommandWarp* GetATCommandWarp();
			Util::ATCommandWarp *pATCommanWarp = GetATCommandWarp();
			BOOL ret = pATCommanWarp->ChangePin(oldpin, newpin);	
			{
				if(!ret)
				{
					main->m_pWarningNoFlashDlg->SetTitle(L"旧密码输入错误!");
					main->m_pWarningNoFlashDlg->SetHWnd(this->GetSafeHwnd());
					main->m_pWarningNoFlashDlg->ShowWindow_(TRUE);
					m_edtOldPassword.SetWindowText(L"");
				}
				else
				{
					SipShowIM(SIPF_OFF);
					ShowWindow_(FALSE);   //lxz 20090906
				}
			}
		}
	}
	else if(m_passwordType == CHECK_LOCKPINPASSWORD)		//输入pin码
	{
		CString s1;
		GetDlgItemText(IDC_SETTING_PASSWORD2, s1);
		
		if(s1 == "")
		{
			main->m_pWarningNoFlashDlg->SetTitle(L"密码输入错误!");
			main->m_pWarningNoFlashDlg->SetHWnd(this->GetSafeHwnd());
			main->m_pWarningNoFlashDlg->ShowWindow_(TRUE);
			m_edtNewPassword1.SetWindowText(L"");
		}
		else
		{
			char oldpin[10];
			std::string s1_;
			s1_ = Util::StringOp::FromCString(s1);
			sprintf(oldpin, "%s", s1_.c_str());
			extern Util::ATCommandWarp* GetATCommandWarp();
			Util::ATCommandWarp *pATCommanWarp = GetATCommandWarp();
			BOOL ret = pATCommanWarp->LockSim(oldpin);	
			if(!ret)
			{
				main->m_pWarningNoFlashDlg->SetTitle(L"密码输入错误!");
				main->m_pWarningNoFlashDlg->SetHWnd(this->GetSafeHwnd());
				main->m_pWarningNoFlashDlg->ShowWindow_(TRUE);
				m_edtNewPassword1.SetWindowText(L"");
			}
			else
			{
				SipShowIM(SIPF_OFF);
				ShowWindow_(FALSE);
				::SendMessage(m_Owner, WM_CHECKPASSWORD, 0, m_nCtrl);
			}
		}
	}
	else if(m_passwordType == CHECK_UNLOCKPINPASSWORD)		//输入pin码
	{
		CString s1;
		GetDlgItemText(IDC_SETTING_PASSWORD2, s1);
		
		if(s1 == "")
		{
			main->m_pWarningNoFlashDlg->SetTitle(L"密码输入错误!");
			main->m_pWarningNoFlashDlg->SetHWnd(this->GetSafeHwnd());
			main->m_pWarningNoFlashDlg->ShowWindow_(TRUE);
			m_edtNewPassword1.SetWindowText(L"");
		}
		else
		{
			char oldpin[10];
			std::string s1_;
			s1_ = Util::StringOp::FromCString(s1);
			sprintf(oldpin, "%s", s1_.c_str());
			extern Util::ATCommandWarp* GetATCommandWarp();
			Util::ATCommandWarp *pATCommanWarp = GetATCommandWarp();
			BOOL ret = pATCommanWarp->UnlockSim(oldpin);	
			if(!ret)
			{
				main->m_pWarningNoFlashDlg->SetTitle(L"密码输入错误!");
				main->m_pWarningNoFlashDlg->SetHWnd(this->GetSafeHwnd());
				main->m_pWarningNoFlashDlg->ShowWindow_(TRUE);
				m_edtNewPassword1.SetWindowText(L"");
			}
			else
			{
				SipShowIM(SIPF_OFF);
				ShowWindow_(FALSE);
				::SendMessage(m_Owner, WM_CHECKPASSWORD, 0, m_nCtrl);
			}
		}
	}
	else if(m_passwordType == CHECK_PINPASSWORD)			//输入解锁PIN
	{
		CString s1;
		GetDlgItemText(IDC_SETTING_PASSWORD2, s1);

		//add by qi 20100802
		//判断PIN码的长度
		if (s1.GetLength() < 4)
		{	
			main->m_pWarningNoFlashDlg->SetTitle(L"PIN码应至少4位");
			main->m_pWarningNoFlashDlg->SetHWnd(this->GetSafeHwnd());
			main->m_pWarningNoFlashDlg->ShowWindow_(TRUE);
			return;
		}
		//

		if(s1 == "")
		{
			main->m_pWarningNoFlashDlg->SetTitle(L"密码输入错误!");
			main->m_pWarningNoFlashDlg->SetHWnd(this->GetSafeHwnd());
			main->m_pWarningNoFlashDlg->ShowWindow_(TRUE);
			m_edtNewPassword1.SetWindowText(L"");
		}
		else
		{
			char oldpin[10];
			std::string s1_;
			s1_ = Util::StringOp::FromCString(s1);
			sprintf(oldpin, "%s", s1_.c_str());
			extern Util::ATCommandWarp* GetATCommandWarp();
			Util::ATCommandWarp *pATCommanWarp = GetATCommandWarp();
			pATCommanWarp->InputPin(oldpin);	
			
			SipShowIM(SIPF_OFF);
			ShowWindow_(FALSE);
		}
	}
	else if(m_passwordType == CHECK_PUKPASSWORD)			//输入解锁PUK
	{
		CString s1;
		GetDlgItemText(IDC_SETTING_PASSWORD2, s1);
		
		if(s1 == "")
		{
			main->m_pWarningNoFlashDlg->SetTitle(L"密码输入错误!");
			main->m_pWarningNoFlashDlg->SetHWnd(this->GetSafeHwnd());
			main->m_pWarningNoFlashDlg->ShowWindow_(TRUE);
			m_edtNewPassword1.SetWindowText(L"");
		}
		else
		{
			char oldpin[10];
			std::string s1_;
			s1_ = Util::StringOp::FromCString(s1);
			sprintf(oldpin, "%s", s1_.c_str());
			extern Util::ATCommandWarp* GetATCommandWarp();
			Util::ATCommandWarp *pATCommanWarp = GetATCommandWarp();
			pATCommanWarp->InputPuk(oldpin, "1234");	
			
			SipShowIM(SIPF_OFF);
			ShowWindow_(FALSE);
		}
	}
	else 
	{
		CString s1;
		GetDlgItemText(IDC_SETTING_PASSWORD2, s1);
	
		if(m_password != s1)
		{
			main->m_pWarningNoFlashDlg->SetTitle(L"密码输入错误!");
			main->m_pWarningNoFlashDlg->SetHWnd(this->GetSafeHwnd());
			main->m_pWarningNoFlashDlg->ShowWindow_(TRUE);
			m_edtNewPassword1.SetWindowText(L"");
		}
		else
		{
			SipShowIM(SIPF_OFF);
			ShowWindow_(SW_HIDE);
			if(m_Owner)
			{
				if(m_passwordType == CHECK_PLAYPASSWORD)
				{
					::SendMessage(m_Owner, CHECK_PLAYPASSWORD, 1, m_nCtrl);
				}
				else if(m_passwordType == CHECK_SUPPERPASSWORD)
				{
					::SendMessage(m_Owner, CHECK_SUPPERPASSWORD, 1, m_nCtrl);
				}
				else if(m_passwordType == CHECK_SCREENPASSWORD)
				{
					main->m_pMainDlg->m_mainScreenSaveDlg_->WindowProc(CHECK_SCREENPASSWORD, 1, m_nCtrl);
				}
				else if(m_passwordType == CHECK_CLEARPASSWORD)
				{
					::SendMessage(m_Owner, CHECK_CLEARPASSWORD, 1, m_nCtrl);
				}
			}
		}
	}
}

void CPasswordDlg::OnButtonPasswordCancel()
{
	if( m_passwordType == CHECK_PINPASSWORD)
	{
		SettingType(CHECK_PINPASSWORD);
	}
	else if(m_passwordType == CHECK_PUKPASSWORD)
	{
		SettingType(CHECK_PUKPASSWORD);
	}
	else
	{
		if(m_passwordType == CHECK_SCREENPASSWORD)
		{
			((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pMainDlg->m_mainScreenSaveDlg_->WindowProc(CHECK_SCREENPASSWORD, 0, m_nCtrl);
		}
		SipShowIM(SIPF_OFF);
		SetTimer(1000, 10, NULL);
		//ShowWindow_(SW_HIDE); 
	}
}

void CPasswordDlg::SetOldPassWord(char *pPassWord)
{
	std::string s = pPassWord;
	m_password = Util::StringOp::ToCString(s);
	if(m_password == L"")
	{
		m_edtOldPassword.EnableWindow(FALSE);
	}
	else
	{
		m_edtOldPassword.EnableWindow(TRUE);
	}
}

void CPasswordDlg::SetErrorCount(int nCount)
{
	if(nCount > 0)
	{
		m_nErrorCount = nCount;
		char temp[24]; 
		sprintf(temp, "剩余%d次", nCount);
		CString s = temp;
		m_MJPGList.SetUnitIsShow(30, TRUE, FALSE);
		m_MJPGList.SetUnitText(30, s, FALSE);
	}
}

void CPasswordDlg::SettingType(PASSWORD_TYPE nType, int nCtrl)
{
	if(nCtrl > 0)
	{
		m_nCtrl = nCtrl;
	}
	m_passwordType = nType;
	m_edtNewPassword1.SetWindowText(L"");
	m_edtNewPassword2.SetWindowText(L"");
	m_edtOldPassword.SetWindowText(L"");
	m_edtNewPassword1.SetLimitDiagital();

	m_edtOldPassword.SetLimitText(6);
	m_edtNewPassword1.SetLimitText(6);
	m_edtNewPassword2.SetLimitText(6);

	m_MJPGList.SetUnitText(1, L"", FALSE);
	m_MJPGList.SetUnitText(2, L"", FALSE);
	m_MJPGList.SetUnitText(3, L"", FALSE);

	if(m_passwordType == SETTINGPLAY_PASSWORD || m_passwordType == SETTINGSUPPER_PASSWORD || m_passwordType == SETTINGPIN_PASSWORD || m_passwordType == SETTINGSCREEN_PASSWORD)
	{
		m_MJPGList.SetUnitText(1, L"请输入旧密码", FALSE);
		m_MJPGList.SetUnitText(2, L"请输入新密码", FALSE);
		m_MJPGList.SetUnitText(3, L"请再次输入新密码", FALSE);

		if(m_passwordType == SETTINGPLAY_PASSWORD)
		{
			m_MJPGList.SetUnitText(0, L"播放密码设置", FALSE);
		}
		else if(m_passwordType == SETTINGPIN_PASSWORD)   //lxz 20090906
		{
			m_edtNewPassword1.SetLimitDiagital();
			m_edtNewPassword1.SetLimitText(8);
			m_edtNewPassword2.SetLimitDiagital();
			m_edtNewPassword2.SetLimitText(8);
			m_edtOldPassword.SetLimitDiagital();
			m_edtOldPassword.SetLimitText(8);	

			m_MJPGList.SetUnitText(0, L"PIN码设置", FALSE);
		}
		else if(m_passwordType == SETTINGSUPPER_PASSWORD)
		{
			m_MJPGList.SetUnitText(0, L"超级密码设置", FALSE);
		}
		else if(m_passwordType == SETTINGSCREEN_PASSWORD)
		{
			m_MJPGList.SetUnitText(0, L"屏保密码设置", FALSE);
		}
		SetTimer(1, 200, NULL);
	}
	else
	{
		m_MJPGList.SetUnitText(2, L"请输入密码", FALSE);
		if(m_passwordType == CHECK_PLAYPASSWORD)
		{
			m_MJPGList.SetUnitText(0, L"播放密码验证", FALSE);
		}
		else if(m_passwordType == CHECK_SUPPERPASSWORD)
		{
			m_MJPGList.SetUnitText(0, L"超级密码验证", FALSE);
		}
		else if(m_passwordType == CHECK_CLEARPASSWORD)
		{
			m_edtNewPassword1.SetLimitDiagital(FALSE);
			m_edtNewPassword1.SetLimitText(8);
			m_MJPGList.SetUnitText(0, L"清空密码验证", FALSE);
		}
		else if(m_passwordType == CHECK_LOCKPINPASSWORD || m_passwordType == CHECK_UNLOCKPINPASSWORD || m_passwordType == CHECK_PINPASSWORD)    //lxz 20090906
		{
			m_MJPGList.SetUnitText(2, L"请输入PIN码", FALSE);
			m_MJPGList.SetUnitText(0, L"PIN码验证", FALSE);
			m_edtNewPassword1.SetLimitText(8);
		}
		else if(m_passwordType == CHECK_PUKPASSWORD )
		{
			m_MJPGList.SetUnitText(0, L"PUK码验证", FALSE);
			m_edtNewPassword1.SetLimitText(8);
		}
		else if(m_passwordType == CHECK_SCREENPASSWORD)
		{
			m_MJPGList.SetUnitText(0, L"屏保码验证", FALSE);
		}
		SetTimer(3, 200, NULL);
	}
	SetTimer(4, 20*1000, NULL);
	m_MJPGList.Invalidate();
}

void CPasswordDlg::OnTimer(UINT nIDEvent)
{
	if(1 == nIDEvent)
	{
		m_edtNewPassword1.ShowWindow(SW_SHOW);
		m_edtNewPassword2.ShowWindow(SW_SHOW);
		m_edtOldPassword.ShowWindow(SW_SHOW);
	}
	else if(2 == nIDEvent)
	{
		m_edtNewPassword1.ShowWindow(SW_HIDE);
		m_edtNewPassword2.ShowWindow(SW_HIDE);
		m_edtOldPassword.ShowWindow(SW_HIDE);
	}
	else if(3 == nIDEvent)
	{
		m_edtNewPassword1.ShowWindow(SW_SHOW);
		m_edtNewPassword2.ShowWindow(SW_HIDE);
		m_edtOldPassword.ShowWindow(SW_HIDE);
	}
	else if(4 == nIDEvent)
	{
		KillTimer(4);
		OnButtonPasswordCancel();
	}
	else if(1000 == nIDEvent)
	{
		KillTimer(4);
		KillTimer(1000);
		ShowWindow_(SW_HIDE);
	}
}

LRESULT CPasswordDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
	case WM_DELETESELITEM:
		::SendMessage(m_Owner, WM_SETTINGPASSWORD, 0, m_passwordType);
		break;
	}
	return CDialog::WindowProc(message, wParam, lParam);
}

void CPasswordDlg ::OnPaint()
{
	if(m_passwordType == CHECK_SCREENPASSWORD)
	{
		CRect rt;
		GetWindowRect(&rt);
		HDC hdc = ::GetDC(m_hWnd);
		CBrush bBr = RGB(0, 0, 0); 
		rt = CRect(0, 0, rt.Width(), rt.Height());
		::FillRect(hdc, rt, (HBRUSH)bBr.m_hObject);
		::ReleaseDC(m_hWnd, hdc);
		CDialog::OnPaint();
	}
	else
		CCEDialog::OnPaint();
}
