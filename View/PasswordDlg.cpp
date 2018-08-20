// passworddlg.cpp : implementation file
//

#include "stdafx.h"
#include "..\multimediaphone.h"
#include "passworddlg.h"
#include "../Data/LanguageResource.h"
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
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_PASSWORD_OK, OnButtonPasswordOk)
	ON_BN_CLICKED(IDC_BUTTON_PASSWORD_CANCEL, OnButtonPasswordCancel)
	ON_MESSAGE(WM_CLICKMJPG_TOAPP, OnClickMJPG)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPasswordDlg message handlers
void CPasswordDlg::OnClickMJPG(WPARAM w, LPARAM l)
{
	switch(w)
	{
	case 100:
		OnButtonPasswordOk();
		break;
	case 101:
		OnButtonPasswordCancel();
		break;
	}
}
BOOL CPasswordDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	m_edtOldPassword.Create(WS_CHILD|ES_PASSWORD, CRect(54 + 267, 62 + 84, 54 + 522, 62 + 124), this, IDC_SETTING_PASSWORD1);
	m_edtOldPassword.SetLimitText(6);
	m_edtOldPassword.SetLimitDiagital();

	m_edtNewPassword1.Create(WS_CHILD|ES_PASSWORD, CRect(54 + 267, 62 + 148, 54 + 522, 62 + 188), this, IDC_SETTING_PASSWORD2);
	m_edtNewPassword1.SetLimitText(6);
	m_edtNewPassword1.SetLimitDiagital();


	m_edtNewPassword2.Create(WS_CHILD|ES_PASSWORD, CRect(54 + 267, 62 + 213, 54 + 522, 62 + 253), this, IDC_SETTING_PASSWORD3);
	m_edtNewPassword2.SetLimitText(6);
	m_edtNewPassword2.SetLimitDiagital();
		
	m_MJPGList.Create(L"", WS_VISIBLE|WS_CHILD, CRect(54, 62, 746, 358), this);
	m_MJPGList.SetCurrentLinkFile(".\\adv\\mjpg\\k1\\中文\\密码验证.xml");
	m_MJPGList.SetMJPGRect(CRect(54, 62, 746, 358));

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPasswordDlg::SetStaticDefaultColor(CCEStatic* cestatic)
{
	// 	cestatic->SetColor(RGB(0, 0, 0), RGB(194,235,149));
}

void CPasswordDlg::OnButtonPasswordOk()
{
	CString s;
	BOOL isError = FALSE;
	if(PASSWORD_CHECKERROR == m_passwordType || PASSWORD_OLDERROR == m_passwordType || PASSWORD_NEWERROR == m_passwordType)
	{
		if(PASSWORD_CHECKERROR == m_passwordType && m_passwordType == CHECK_PINPASSWORD)
		{
			SetType(CHECK_PINPASSWORD);
		}
		if(PASSWORD_CHECKERROR == m_passwordType && m_passwordType == CHECK_PUKPASSWORD)
		{
			SetType(CHECK_PUKPASSWORD);
		}
		else
		{
			SipShowIM(SIPF_OFF);
			ShowWindow_(FALSE);
			
			if(m_Owner && (PASSWORD_CHECKERROR == m_passwordType))
				::SendMessage(m_Owner, WM_CHECKPASSWORD, 0, m_nCtrl);
		}	
	}
	else if(m_passwordType == SETTINGPLAY_PASSWORD || m_passwordType == SETTINGSUPPER_PASSWORD)
	{
		CString s1, s2, s3;
		GetDlgItemText(IDC_SETTING_PASSWORD1, s1);
		GetDlgItemText(IDC_SETTING_PASSWORD2, s2);
		GetDlgItemText(IDC_SETTING_PASSWORD3, s3);
		if(m_password != s1)
		{
			SetType(PASSWORD_OLDERROR, m_nCtrl);
		}
		else if(s2 != s3)
		{
			SetType(PASSWORD_NEWERROR, m_nCtrl);
		}
		else
		{
			SipShowIM(SIPF_OFF);
			//给主窗口发送新的密码
			ShowWindow_(FALSE);
//			theApp.inputDlg_->ShowWindow(FALSE);
			if(m_Owner)
			{
				std::string strtemp = Util::StringOp::FromCString(s2);
				const char *temp = strtemp.c_str();
				::SendMessage(m_Owner, WM_SETTINGPASSWORD, (WPARAM)temp, /*strlen(temp)*/m_passwordType);
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
		if(s1 == "" || s2 == "")
		{
			SetType(PASSWORD_NEWERROR, m_nCtrl);	//lxz 20090906
		}
		else if(s2 != s3)
		{
			SetType(PASSWORD_CHECKERROR, m_nCtrl);
		}
		else
		{
			char oldpin[6], newpin[6];
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
					SetType(PASSWORD_CHECKERROR, m_nCtrl);   //lxz 20090906
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
			SetType(PASSWORD_CHECKERROR, m_nCtrl);
		}
		else
		{
			char oldpin[6];
			std::string s1_;
			s1_ = Util::StringOp::FromCString(s1);
			sprintf(oldpin, "%s", s1_.c_str());
			extern Util::ATCommandWarp* GetATCommandWarp();
			Util::ATCommandWarp *pATCommanWarp = GetATCommandWarp();
			BOOL ret = pATCommanWarp->LockSim(oldpin);	
			if(!ret)
			{
				SetType(PASSWORD_CHECKERROR, m_nCtrl);
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
			SetType(PASSWORD_CHECKERROR, m_nCtrl);
		}
		else
		{
			char oldpin[6];
			std::string s1_;
			s1_ = Util::StringOp::FromCString(s1);
			sprintf(oldpin, "%s", s1_.c_str());
			extern Util::ATCommandWarp* GetATCommandWarp();
			Util::ATCommandWarp *pATCommanWarp = GetATCommandWarp();
			BOOL ret = pATCommanWarp->UnlockSim(oldpin);	
			if(!ret)
			{
				SetType(PASSWORD_CHECKERROR, m_nCtrl);
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
		
		if(s1 == "")
		{
			SetType(PASSWORD_CHECKERROR, m_nCtrl);
		}
		else
		{
			char oldpin[6];
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
			SetType(PASSWORD_CHECKERROR, m_nCtrl);
		}
		else
		{
			char oldpin[6];
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
			SetType(PASSWORD_CHECKERROR, m_nCtrl);
		}
		else
		{
			SipShowIM(SIPF_OFF);
			ShowWindow_(FALSE);
			if(m_Owner)
				::SendMessage(m_Owner, WM_CHECKPASSWORD, 1, m_nCtrl);
		}
	}
}

void CPasswordDlg::OnButtonPasswordCancel()
{
	if( m_passwordType == CHECK_PINPASSWORD)
	{
		SetType(CHECK_PINPASSWORD);
	}
	else if(m_passwordType == CHECK_PUKPASSWORD)
	{
		SetType(CHECK_PUKPASSWORD);
	}
	else
	{
		if(m_Owner && (CHECK_CLEARPASSWORD >= m_passwordType))
			::SendMessage(m_Owner, WM_CHECKPASSWORD, 0, m_nCtrl);
		
		SipShowIM(SIPF_OFF);
		ShowWindow_(FALSE); 
	}
}

void CPasswordDlg::SetOldPassWord(char *pPassWord)
{
	m_password = pPassWord;
	if(m_password == "" /* && m_passwordType >= SETTINGPLAY_PASSWORD  && m_passwordType <= SETTINGSUPPER_PASSWORD*/)
	{
	//	m_MJPGList.SetUnitBitmap(2, L"", L"",  TRUE);
		m_edtOldPassword.ShowWindow(SW_HIDE);
	}
}

void CPasswordDlg::SetType(PASSWORD_TYPE nType, int nCtrl)
{
	m_nCtrl = nCtrl;
	m_passwordType = nType;
	m_edtNewPassword1.SetWindowText(L"");
	m_edtNewPassword2.SetWindowText(L"");
	m_edtOldPassword.SetWindowText(L"");
	m_edtNewPassword1.SetLimitDiagital();
	m_edtNewPassword1.SetLimitText(6);

	if(m_passwordType == SETTINGPLAY_PASSWORD || m_passwordType == SETTINGSUPPER_PASSWORD || m_passwordType == SETTINGPIN_PASSWORD)
	{
		m_MJPGList.SetUnitBitmap(3, L".\\adv\\mjpg\\k1\\common\\input_null.bmp", L"",  TRUE);
		m_MJPGList.SetUnitBitmap(5, L".\\adv\\mjpg\\k1\\common\\input_null.bmp", L"",  TRUE);
		m_MJPGList.SetUnitBitmap(7, L".\\adv\\mjpg\\k1\\common\\input_null.bmp", L"",  TRUE);

		if(m_passwordType == SETTINGPLAY_PASSWORD)
			m_MJPGList.SetUnitBitmap(2, L".\\adv\\mjpg\\k1\\common\\password_playold.bmp", L"",  TRUE);
		else if(m_passwordType == SETTINGPIN_PASSWORD)   //lxz 20090906
		{
			m_edtNewPassword1.SetLimitDiagital();
			m_edtNewPassword1.SetLimitText(4);
			m_edtNewPassword2.SetLimitDiagital();
			m_edtNewPassword2.SetLimitText(4);
			m_edtOldPassword.SetLimitDiagital();
			m_edtOldPassword.SetLimitText(4);	
			m_MJPGList.SetUnitBitmap(2, L".\\adv\\mjpg\\k1\\common\\password_pin.bmp", L"",  TRUE);
		}
		else
			m_MJPGList.SetUnitBitmap(2, L".\\adv\\mjpg\\k1\\common\\password_supperold.bmp", L"",  TRUE);

		

		m_MJPGList.SetUnitBitmap(4, L".\\adv\\mjpg\\k1\\common\\password_new.bmp", L"",  TRUE);
		m_MJPGList.SetUnitBitmap(6, L".\\adv\\mjpg\\k1\\common\\password_new1.bmp", L"",  TRUE);
		
		m_edtNewPassword1.ShowWindow(SW_SHOW);
		m_edtNewPassword2.ShowWindow(SW_SHOW);
		m_edtOldPassword.ShowWindow(SW_SHOW);
	}
	else if(m_passwordType == PASSWORD_CHECKERROR)
	{
		
		m_MJPGList.SetUnitBitmap(3, L"", L"",  TRUE);
		m_MJPGList.SetUnitBitmap(7, L"", L"",  TRUE);
		
		m_MJPGList.SetUnitBitmap(2, L"", L"",  TRUE);
		m_MJPGList.SetUnitBitmap(6, L"", L"",  TRUE);
		
		m_MJPGList.SetUnitBitmap(4, L".\\adv\\mjpg\\k1\\common\\password_checkerror.bmp", L"",  TRUE);
		m_MJPGList.SetUnitBitmap(5, L"", L"",  TRUE);
		
		m_edtNewPassword1.ShowWindow(SW_HIDE);
		m_edtNewPassword2.ShowWindow(SW_HIDE);
		m_edtOldPassword.ShowWindow(SW_HIDE);
	}
	else if(m_passwordType == PASSWORD_OLDERROR)
	{
		m_MJPGList.SetUnitBitmap(3, L"", L"",  TRUE);
		m_MJPGList.SetUnitBitmap(7, L"", L"",  TRUE);
		
		m_MJPGList.SetUnitBitmap(2, L"", L"",  TRUE);
		m_MJPGList.SetUnitBitmap(6, L"", L"",  TRUE);
		
		m_MJPGList.SetUnitBitmap(4, L".\\adv\\mjpg\\k1\\common\\password_olderror.bmp", L"",  TRUE);
		m_MJPGList.SetUnitBitmap(5, L"", L"",  TRUE);
		
		m_edtNewPassword1.ShowWindow(SW_HIDE);
		m_edtNewPassword2.ShowWindow(SW_HIDE);
		m_edtOldPassword.ShowWindow(SW_HIDE);
	}
	else if(m_passwordType == PASSWORD_NEWERROR)
	{
		
		m_MJPGList.SetUnitBitmap(3, L"", L"",  TRUE);
		m_MJPGList.SetUnitBitmap(7, L"", L"",  TRUE);
		
		m_MJPGList.SetUnitBitmap(2, L"", L"",  TRUE);
		m_MJPGList.SetUnitBitmap(6, L"", L"",  TRUE);
		
		m_MJPGList.SetUnitBitmap(4, L".\\adv\\mjpg\\k1\\common\\password_newerror.bmp", L"",  TRUE);
		m_MJPGList.SetUnitBitmap(5, L"", L"",  TRUE);
		
		m_edtNewPassword1.ShowWindow(SW_HIDE);
		m_edtNewPassword2.ShowWindow(SW_HIDE);
		m_edtOldPassword.ShowWindow(SW_HIDE);
	}
	else
	{
	
		m_MJPGList.SetUnitBitmap(3, L"", L"",  TRUE);
		m_MJPGList.SetUnitBitmap(7, L"", L"",  TRUE);
		
		m_MJPGList.SetUnitBitmap(2, L"", L"",  TRUE);
		m_MJPGList.SetUnitBitmap(6, L"", L"",  TRUE);

		if(m_passwordType == CHECK_PLAYPASSWORD)
		{
			m_MJPGList.SetUnitBitmap(4, L".\\adv\\mjpg\\k1\\common\\password_play.bmp", L"",  TRUE);
			m_MJPGList.SetUnitBitmap(5, L".\\adv\\mjpg\\k1\\common\\input_null.bmp", L"",  TRUE);
		}
		else if(m_passwordType == CHECK_SUPPERPASSWORD)
		{
			m_MJPGList.SetUnitBitmap(4, L".\\adv\\mjpg\\k1\\common\\password_supper.bmp", L"",  TRUE);
			m_MJPGList.SetUnitBitmap(5, L".\\adv\\mjpg\\k1\\common\\input_null.bmp", L"",  TRUE);
		}
		else if(m_passwordType == CHECK_CLEARPASSWORD)
		{
			m_edtNewPassword1.SetLimitDiagital(FALSE);
			m_edtNewPassword1.SetLimitText(8);
			m_MJPGList.SetUnitBitmap(4, L".\\adv\\mjpg\\k1\\common\\password_clear.bmp", L"",  TRUE);
			m_MJPGList.SetUnitBitmap(5, L".\\adv\\mjpg\\k1\\common\\input_null.bmp", L"",  TRUE);
		}
		else if(m_passwordType == CHECK_LOCKPINPASSWORD || m_passwordType == CHECK_UNLOCKPINPASSWORD || m_passwordType == CHECK_PINPASSWORD)    //lxz 20090906
		{
			m_edtNewPassword1.SetLimitText(4);
			m_MJPGList.SetUnitBitmap(4, L".\\adv\\mjpg\\k1\\common\\password_pin.bmp", L"",  TRUE);
			m_MJPGList.SetUnitBitmap(5, L".\\adv\\mjpg\\k1\\common\\input_null.bmp", L"",  TRUE);
		}
		else if(m_passwordType == CHECK_PUKPASSWORD )
		{
			m_edtNewPassword1.SetLimitText(8);
			m_MJPGList.SetUnitBitmap(4, L".\\adv\\mjpg\\k1\\common\\password_puk.bmp", L"",  TRUE);
			m_MJPGList.SetUnitBitmap(5, L".\\adv\\mjpg\\k1\\common\\input_null.bmp", L"",  TRUE);
		}
		m_edtNewPassword1.ShowWindow(SW_SHOW);
		m_edtNewPassword2.ShowWindow(SW_HIDE);
		m_edtOldPassword.ShowWindow(SW_HIDE);
	}
}

// void CPasswordDlg::SetStep(int nStep)
// {
// // 	if(m_passwordType == SETTING_PASSWORD )
// // 	{
// // 		if(nStep == 1)
// // 		{
// // 			
// // 			m_sticNewPassword1.ShowWindow(SW_SHOW);
// // 			m_sticNewPassword2.ShowWindow(SW_SHOW;
// // 			m_edtNewPassword1.ShowWindow(SW_SHOW);
// // 			m_edtNewPassword2.ShowWindow(SW_SHOW);
// // 			m_sticOldPassword.ShowWindow(SW_HIDE);
// // 			m_edtOldPassword.ShowWindow(SW_HIDE);
// // 
// // 			std::string strTemp = Data::LanguageResource::Get(Data::RI_PASSWORD_TIP);
// // 			CString str = strTemp.c_str();
// // 			m_sticTip.SetWindowText(str);
// // 		}
// // 	}
// }
