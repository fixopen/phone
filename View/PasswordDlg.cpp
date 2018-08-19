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


CPasswordDlg::CPasswordDlg(CWnd* pParent /*=NULL*/)
: CCEDialog(CPasswordDlg::IDD, pParent) {
    //{{AFX_DATA_INIT(CPasswordDlg)
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
    m_passwordType = SETTINGPLAY_PASSWORD;
    m_password = "";
    m_tempPassword = "";
    m_nStep = 0;
    m_Owner = NULL;
}

void CPasswordDlg::DoDataExchange(CDataExchange* pDX) {
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CPasswordDlg)
    // NOTE: the ClassWizard will add DDX and DDV calls here
    //}}AFX_DATA_MAP

}


BEGIN_MESSAGE_MAP(CPasswordDlg, CCEDialog)
    //{{AFX_MSG_MAP(CPasswordDlg)
    ON_WM_TIMER()
    //}}AFX_MSG_MAP
    ON_BN_CLICKED(IDC_BUTTON_PASSWORD_OK, OnButtonPasswordOk)
    ON_BN_CLICKED(IDC_BUTTON_PASSWORD_CANCEL, OnButtonPasswordCancel)
    ON_MESSAGE(WM_CLICKMJPG_TOAPP, OnClickMJPG)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPasswordDlg message handlers
LRESULT CPasswordDlg::OnClickMJPG(WPARAM w, LPARAM l) {
    LRESULT result = 0;
    switch (w) {
    case 1001:
        //      OnButtonPasswordOk();
        SettingOK();
        break;
    case 1000:
        OnButtonPasswordCancel();
        break;
    }
    return result;
}
BOOL CPasswordDlg::OnInitDialog() {
    CDialog::OnInitDialog();

    // TODO: Add extra initialization here

    m_edtOldPassword.Create(WS_CHILD | ES_PASSWORD, CRect(190 + 100, 120 + 137, 401 + 100, 137 + 149), this, IDC_SETTING_PASSWORD1);
    m_edtOldPassword.SetLimitText(6);
    m_edtOldPassword.SetLimitDiagital();

    m_edtNewPassword1.Create(WS_CHILD | ES_PASSWORD, CRect(190 + 100, 137 + 156, 401 + 100, 137 + 185), this, IDC_SETTING_PASSWORD2);
    m_edtNewPassword1.SetLimitText(6);
    m_edtNewPassword1.SetLimitDiagital();


    m_edtNewPassword2.Create(WS_CHILD | ES_PASSWORD, CRect(190 + 100, 137 + 192, 401 + 100, 137 + 221), this, IDC_SETTING_PASSWORD3);
    m_edtNewPassword2.SetLimitText(6);
    m_edtNewPassword2.SetLimitDiagital();

    m_MJPGList.Create(L"", WS_VISIBLE | WS_CHILD, CRect(100, 137, 540, 407), this);
    m_MJPGList.SetCurrentLinkFile(".\\adv\\mjpg\\k5\\ÖÐÎÄ\\ÃÜÂëÑéÖ¤.xml");
    m_MJPGList.SetMJPGRect(CRect(100, 137, 540, 407));
    m_MJPGList.SetUnitFont(0, font_20);
    m_MJPGList.SetUnitFont(1, font_18);
    m_MJPGList.SetUnitFont(2, font_18);
    m_MJPGList.SetUnitFont(3, font_18);

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CPasswordDlg::SetStaticDefaultColor(CCEStatic* cestatic) {
    //  cestatic->SetColor(RGB(0, 0, 0), RGB(194,235,149));
}

void CPasswordDlg::OnButtonPasswordOk() {
    CString s;
    BOOL isError = FALSE;
    if (PASSWORD_CHECKERROR == m_passwordType || PASSWORD_OLDERROR == m_passwordType || PASSWORD_NEWERROR == m_passwordType) {
        if (PASSWORD_CHECKERROR == m_passwordType && m_passwordType == CHECK_PINPASSWORD) {
            SetType(CHECK_PINPASSWORD);
        }
        if (PASSWORD_CHECKERROR == m_passwordType && m_passwordType == CHECK_PUKPASSWORD) {
            SetType(CHECK_PUKPASSWORD);
        } else {
            SipShowIM(SIPF_OFF);
            ShowWindow_(FALSE);

            if (m_Owner && (PASSWORD_CHECKERROR == m_passwordType)) {
                ::SendMessage(m_Owner, WM_CHECKPASSWORD, 0, m_nCtrl);
            }
        }
    } else if (m_passwordType == SETTINGPLAY_PASSWORD || m_passwordType == SETTINGSUPPER_PASSWORD) {
        CString s1, s2, s3;
          GetDlgItemText(IDC_SETTING_PASSWORD1, s1);
          GetDlgItemText(IDC_SETTING_PASSWORD2, s2);
          GetDlgItemText(IDC_SETTING_PASSWORD3, s3);
        if (m_password != s1) {
            SetType(PASSWORD_OLDERROR, m_nCtrl);
        } else if (s2 != s3) {
            SetType(PASSWORD_NEWERROR, m_nCtrl);
        } else {
              SipShowIM(SIPF_OFF);
              //¸øÖ÷´°¿Ú·¢ËÍÐÂµÄÃÜÂë
              ShowWindow_(FALSE);
              //          theApp.inputDlg_->ShowWindow(FALSE);
            if (m_Owner) {
                std::string strtemp = Util::StringOp::FromCString(s2);
                const char* temp = strtemp.c_str();
                ::SendMessage(m_Owner, WM_SETTINGPASSWORD, (WPARAM)temp, /*strlen(temp)*/m_passwordType);
                return;
            }
        }
    } else if (m_passwordType == SETTINGPIN_PASSWORD)           //ÉèÖÃPINÂë
    {
        CString s1, s2, s3;
        GetDlgItemText(IDC_SETTING_PASSWORD1, s1);
        GetDlgItemText(IDC_SETTING_PASSWORD2, s2);
        GetDlgItemText(IDC_SETTING_PASSWORD3, s3);
        if (s1 == "" || s2 == "") {
            SetType(PASSWORD_NEWERROR, m_nCtrl);    //lxz 20090906
        } else if (s2 != s3) {
            SetType(PASSWORD_CHECKERROR, m_nCtrl);
        } else {
            char oldpin[6], newpin[6];
            std::string s1_, s2_;
            s1_ = Util::StringOp::FromCString(s1);
            s2_ = Util::StringOp::FromCString(s2);
            sprintf_s(oldpin, 6, "%s", s1_.c_str());
            sprintf_s(newpin, 6, "%s", s2_.c_str());
            extern Util::ATCommandWarp * GetATCommandWarp();
            Util::ATCommandWarp* pATCommanWarp = GetATCommandWarp();
            BOOL ret = pATCommanWarp->ChangePin(oldpin, newpin);
            {
                if (!ret) {
                    SetType(PASSWORD_CHECKERROR, m_nCtrl);   //lxz 20090906
                } else {
                    SipShowIM(SIPF_OFF);
                    ShowWindow_(FALSE);   //lxz 20090906
                }
            }
        }
    } else if (m_passwordType == CHECK_LOCKPINPASSWORD)     //ÊäÈëpinÂë
    {
        CString s1;
        GetDlgItemText(IDC_SETTING_PASSWORD2, s1);

        if (s1 == "") {
            SetType(PASSWORD_CHECKERROR, m_nCtrl);
        } else {
            char oldpin[6] = {
                0
            };
            std::string s1_;
            s1_ = Util::StringOp::FromCString(s1);
            sprintf_s(oldpin, 6, "%s", s1_.c_str());
            extern Util::ATCommandWarp * GetATCommandWarp();
            Util::ATCommandWarp* pATCommanWarp = GetATCommandWarp();
            BOOL ret = pATCommanWarp->LockSim(oldpin);  
            if (!ret) {
                SetType(PASSWORD_CHECKERROR, m_nCtrl);
            } else {
                SipShowIM(SIPF_OFF);
                ShowWindow_(FALSE);
                ::SendMessage(m_Owner, WM_CHECKPASSWORD, 0, m_nCtrl);
            }
        }
    } else if (m_passwordType == CHECK_UNLOCKPINPASSWORD)       //ÊäÈëpinÂë
    {
        CString s1;
        GetDlgItemText(IDC_SETTING_PASSWORD2, s1);

        if (s1 == "") {
            SetType(PASSWORD_CHECKERROR, m_nCtrl);
        } else {
            char oldpin[6] = {
                0
            };
            std::string s1_;
            s1_ = Util::StringOp::FromCString(s1);
            sprintf_s(oldpin, 6, "%s", s1_.c_str());
            extern Util::ATCommandWarp * GetATCommandWarp();
            Util::ATCommandWarp* pATCommanWarp = GetATCommandWarp();
            BOOL ret = pATCommanWarp->UnlockSim(oldpin);    
            if (!ret) {
                SetType(PASSWORD_CHECKERROR, m_nCtrl);
            } else {
                SipShowIM(SIPF_OFF);
                ShowWindow_(FALSE);
                ::SendMessage(m_Owner, WM_CHECKPASSWORD, 0, m_nCtrl);
            }
        }
    } else if (m_passwordType == CHECK_PINPASSWORD)         //ÊäÈë½âËøPIN
    {
        CString s1;
        GetDlgItemText(IDC_SETTING_PASSWORD2, s1);

        if (s1 == "") {
            SetType(PASSWORD_CHECKERROR, m_nCtrl);
        } else {
            char oldpin[6] = {
                0
            };
            std::string s1_;
            s1_ = Util::StringOp::FromCString(s1);
            sprintf_s(oldpin, 6, "%s", s1_.c_str());
            extern Util::ATCommandWarp * GetATCommandWarp();
            Util::ATCommandWarp* pATCommanWarp = GetATCommandWarp();
            pATCommanWarp->InputPin(oldpin);    

            SipShowIM(SIPF_OFF);
            ShowWindow_(FALSE);
        }
    } else if (m_passwordType == CHECK_PUKPASSWORD)         //ÊäÈë½âËøPUK
    {
        CString s1;
        GetDlgItemText(IDC_SETTING_PASSWORD2, s1);

        if (s1 == "") {
            SetType(PASSWORD_CHECKERROR, m_nCtrl);
        } else {
            char oldpin[6] = {
                0
            };
            std::string s1_;
            s1_ = Util::StringOp::FromCString(s1);
            sprintf_s(oldpin, 6, "%s", s1_.c_str());
            extern Util::ATCommandWarp * GetATCommandWarp();
            Util::ATCommandWarp* pATCommanWarp = GetATCommandWarp();
            pATCommanWarp->InputPuk(oldpin, "1234");    

            SipShowIM(SIPF_OFF);
            ShowWindow_(FALSE);
        }
    } else {
        CString s1;
        GetDlgItemText(IDC_SETTING_PASSWORD2, s1);

        if (m_password != s1) {
            SetType(PASSWORD_CHECKERROR, m_nCtrl);
        } else {
            SipShowIM(SIPF_OFF);
            ShowWindow_(FALSE);
            if (m_Owner) {
                ::SendMessage(m_Owner, WM_CHECKPASSWORD, 1, m_nCtrl);
            }
        }
    }
}

void CPasswordDlg::SettingOK() {
    CString s;
    if (m_passwordType == SETTINGPLAY_PASSWORD || m_passwordType == SETTINGSUPPER_PASSWORD || m_passwordType == SETTINGSCREEN_PASSWORD) {
        CString s1, s2, s3;
        GetDlgItemText(IDC_SETTING_PASSWORD1, s1);
        GetDlgItemText(IDC_SETTING_PASSWORD2, s2);
        GetDlgItemText(IDC_SETTING_PASSWORD3, s3);
        if (m_password != s1) {
            m_MJPGList.SetUnitText(1, _T("¾ÉÃÜÂëÊäÈë´íÎó£¡"), TRUE);
            m_edtOldPassword.SetWindowText(L"");
        } else if (s2 != s3) {
            m_MJPGList.SetUnitText(2, L"ÐÂÃÜÂëÊäÈë´íÎó£¡", TRUE);
            m_edtNewPassword1.SetWindowText(L"");
            m_edtNewPassword2.SetWindowText(L"");
        } else {
              SipShowIM(SIPF_OFF);
              //¸øÖ÷´°¿Ú·¢ËÍÐÂµÄÃÜÂë
              ShowWindow_(FALSE);
            if (m_Owner) {
                std::string strtemp = Util::StringOp::FromCString(s2);
                m_tempPassword = strtemp.c_str();
                ((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetTitle(L"È·¶¨Òª¸ü¸ÄÃÜÂëÂð£¿", 0);
                ((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetHWnd(this->GetSafeHwnd());
                ((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->m_pDeleteTipDlg->ShowWindow_(TRUE);
                return;
            }
        }
    } else if (m_passwordType == SETTINGPIN_PASSWORD)           //ÉèÖÃPINÂë
    {
        CString s1, s2, s3;
        GetDlgItemText(IDC_SETTING_PASSWORD1, s1);
        GetDlgItemText(IDC_SETTING_PASSWORD2, s2);
        GetDlgItemText(IDC_SETTING_PASSWORD3, s3);
        if (s1 == "") {
            m_MJPGList.SetUnitText(1, L"¾ÉÃÜÂëÊäÈë´íÎó£¡", TRUE);
            m_edtOldPassword.SetWindowText(L"");
        } else if (s2 != s3) {
            m_MJPGList.SetUnitText(2, L"ÐÂÃÜÂëÊäÈë´íÎó£¡", TRUE);
            m_edtNewPassword1.SetWindowText(L"");
            m_edtNewPassword2.SetWindowText(L"");
        } else {
            char oldpin[6] = {
                 0
                 }, newpin[6] = {0};
            std::string s1_, s2_;
            s1_ = Util::StringOp::FromCString(s1);
            s2_ = Util::StringOp::FromCString(s2);
            sprintf_s(oldpin, 6, "%s", s1_.c_str());
            sprintf_s(newpin, 6, "%s", s2_.c_str());
            extern Util::ATCommandWarp * GetATCommandWarp();
            Util::ATCommandWarp* pATCommanWarp = GetATCommandWarp();
            BOOL ret = pATCommanWarp->ChangePin(oldpin, newpin);
            {
                if (!ret) {
                    m_MJPGList.SetUnitText(1, L"¾ÉÃÜÂëÊäÈë´íÎó£¡", TRUE);
                    m_edtOldPassword.SetWindowText(L"");
                } else {
                    SipShowIM(SIPF_OFF);
                    ShowWindow_(FALSE);   //lxz 20090906
                }
            }
        }
    } else if (m_passwordType == CHECK_LOCKPINPASSWORD)     //ÊäÈëpinÂë
    {
        CString s1;
        GetDlgItemText(IDC_SETTING_PASSWORD2, s1);

        if (s1 == "") {
            m_MJPGList.SetUnitText(2, L"ÃÜÂëÊäÈë´íÎó£¡", TRUE);
            m_edtNewPassword1.SetWindowText(L"");
        } else {
            char oldpin[6] = {
                0
            };
            std::string s1_;
            s1_ = Util::StringOp::FromCString(s1);
            sprintf_s(oldpin, 6, "%s", s1_.c_str());
            extern Util::ATCommandWarp * GetATCommandWarp();
            Util::ATCommandWarp* pATCommanWarp = GetATCommandWarp();
            BOOL ret = pATCommanWarp->LockSim(oldpin);  
            if (!ret) {
                m_MJPGList.SetUnitText(2, L"ÃÜÂëÊäÈë´íÎó£¡", TRUE);
                m_edtNewPassword1.SetWindowText(L"");
            } else {
                SipShowIM(SIPF_OFF);
                ShowWindow_(FALSE);
                ::SendMessage(m_Owner, WM_CHECKPASSWORD, 0, m_nCtrl);
            }
        }
    } else if (m_passwordType == CHECK_UNLOCKPINPASSWORD)       //ÊäÈëpinÂë
    {
        CString s1;
        GetDlgItemText(IDC_SETTING_PASSWORD2, s1);

        if (s1 == "") {
            m_MJPGList.SetUnitText(2, L"ÃÜÂëÊäÈë´íÎó£¡", TRUE);
            m_edtNewPassword1.SetWindowText(L"");
        } else {
            char oldpin[6] = {
                0
            };
            std::string s1_;
            s1_ = Util::StringOp::FromCString(s1);
            sprintf_s(oldpin, 6, "%s", s1_.c_str());
            extern Util::ATCommandWarp * GetATCommandWarp();
            Util::ATCommandWarp* pATCommanWarp = GetATCommandWarp();
            BOOL ret = pATCommanWarp->UnlockSim(oldpin);    
            if (!ret) {
                m_MJPGList.SetUnitText(2, L"ÃÜÂëÊäÈë´íÎó£¡", TRUE);
                m_edtNewPassword1.SetWindowText(L"");
            } else {
                SipShowIM(SIPF_OFF);
                ShowWindow_(FALSE);
                ::SendMessage(m_Owner, WM_CHECKPASSWORD, 0, m_nCtrl);
            }
        }
    } else if (m_passwordType == CHECK_PINPASSWORD)         //ÊäÈë½âËøPIN
    {
        CString s1;
        GetDlgItemText(IDC_SETTING_PASSWORD2, s1);

        if (s1 == "") {
            m_MJPGList.SetUnitText(2, L"ÃÜÂëÊäÈë´íÎó£¡", TRUE);
            m_edtNewPassword1.SetWindowText(L"");
        } else {
            char oldpin[6] = {
                0
            };
            std::string s1_;
            s1_ = Util::StringOp::FromCString(s1);
            sprintf_s(oldpin, 6, "%s", s1_.c_str());
            extern Util::ATCommandWarp * GetATCommandWarp();
            Util::ATCommandWarp* pATCommanWarp = GetATCommandWarp();
            pATCommanWarp->InputPin(oldpin);    

            SipShowIM(SIPF_OFF);
            ShowWindow_(FALSE);
        }
    } else if (m_passwordType == CHECK_PUKPASSWORD)         //ÊäÈë½âËøPUK
    {
        CString s1;
        GetDlgItemText(IDC_SETTING_PASSWORD2, s1);

        if (s1 == "") {
            m_MJPGList.SetUnitText(2, L"ÃÜÂëÊäÈë´íÎó£¡", TRUE);
            m_edtNewPassword1.SetWindowText(L"");
        } else {
            char oldpin[6] = {
                0
            };
            std::string s1_;
            s1_ = Util::StringOp::FromCString(s1);
            sprintf_s(oldpin, 6, "%s", s1_.c_str());
            extern Util::ATCommandWarp * GetATCommandWarp();
            Util::ATCommandWarp* pATCommanWarp = GetATCommandWarp();
            pATCommanWarp->InputPuk(oldpin, "1234");    

            SipShowIM(SIPF_OFF);
            ShowWindow_(FALSE);
        }
    } else {
        CString s1;
        GetDlgItemText(IDC_SETTING_PASSWORD2, s1);

        if (m_password != s1) {
            m_MJPGList.SetUnitText(1, L"¾ÉÃÜÂëÊäÈë´íÎó£¡", TRUE);
            m_edtOldPassword.SetWindowText(L"");
        } else {
            SipShowIM(SIPF_OFF);
            ShowWindow_(FALSE);
            if (m_Owner) {
                ::SendMessage(m_Owner, WM_CHECKPASSWORD, 1, m_nCtrl);
            }
        }
    }
}

void CPasswordDlg::OnButtonPasswordCancel() {
    if (m_passwordType == CHECK_PINPASSWORD) {
        SetType(CHECK_PINPASSWORD);
    } else if (m_passwordType == CHECK_PUKPASSWORD) {
        SetType(CHECK_PUKPASSWORD);
    } else {
        if (m_Owner && (CHECK_CLEARPASSWORD >= m_passwordType)) {
            ::SendMessage(m_Owner, WM_CHECKPASSWORD, 0, m_nCtrl);
        }

        SipShowIM(SIPF_OFF);
        ShowWindow_(FALSE);
    }
}

void CPasswordDlg::SetOldPassWord(char* pPassWord) {
    m_password = pPassWord;
    if (m_password == "" /* && m_passwordType >= SETTINGPLAY_PASSWORD  && m_passwordType <= SETTINGSUPPER_PASSWORD*/) {
        //  m_MJPGList.SetUnitBitmap(2, L"", L"",  TRUE);
        m_edtOldPassword.ShowWindow(SW_HIDE);
    }
}

void CPasswordDlg::SetType(PASSWORD_TYPE nType, int nCtrl) {
    m_nCtrl = nCtrl;
    m_passwordType = nType;
    m_edtNewPassword1.SetWindowText(L"");
    m_edtNewPassword2.SetWindowText(L"");
    m_edtOldPassword.SetWindowText(L"");
    m_edtNewPassword1.SetLimitDiagital();
    m_edtNewPassword1.SetLimitText(6);

    if (m_passwordType == SETTINGPLAY_PASSWORD || m_passwordType == SETTINGSUPPER_PASSWORD || m_passwordType == SETTINGPIN_PASSWORD) {
        m_MJPGList.SetUnitBitmap(3, L".\\adv\\mjpg\\k1\\common\\input_null.bmp", L"", TRUE);
        m_MJPGList.SetUnitBitmap(5, L".\\adv\\mjpg\\k1\\common\\input_null.bmp", L"", TRUE);
        m_MJPGList.SetUnitBitmap(7, L".\\adv\\mjpg\\k1\\common\\input_null.bmp", L"", TRUE);

        if (m_passwordType == SETTINGPLAY_PASSWORD) {
            m_MJPGList.SetUnitBitmap(2, L".\\adv\\mjpg\\k1\\common\\password_playold.bmp", L"", TRUE);
        } else if (m_passwordType == SETTINGPIN_PASSWORD)   //lxz 20090906
        {
              m_edtNewPassword1.SetLimitDiagital();
              m_edtNewPassword1.SetLimitText(4);
              m_edtNewPassword2.SetLimitDiagital();
              m_edtNewPassword2.SetLimitText(4);
              m_edtOldPassword.SetLimitDiagital();
              m_edtOldPassword.SetLimitText(4);   
              m_MJPGList.SetUnitBitmap(2, L".\\adv\\mjpg\\k1\\common\\password_pin.bmp", L"", TRUE);
        } else {
              m_MJPGList.SetUnitBitmap(2, L".\\adv\\mjpg\\k1\\common\\password_supperold.bmp", L"", TRUE);
        }



        m_MJPGList.SetUnitBitmap(4, L".\\adv\\mjpg\\k1\\common\\password_new.bmp", L"", TRUE);
        m_MJPGList.SetUnitBitmap(6, L".\\adv\\mjpg\\k1\\common\\password_new1.bmp", L"", TRUE);

        m_edtNewPassword1.ShowWindow(SW_SHOW);
        m_edtNewPassword2.ShowWindow(SW_SHOW);
        m_edtOldPassword.ShowWindow(SW_SHOW);
    } else if (m_passwordType == PASSWORD_CHECKERROR) {
          m_MJPGList.SetUnitBitmap(3, L"", L"", TRUE);
          m_MJPGList.SetUnitBitmap(7, L"", L"", TRUE);

          m_MJPGList.SetUnitBitmap(2, L"", L"", TRUE);
          m_MJPGList.SetUnitBitmap(6, L"", L"", TRUE);

          m_MJPGList.SetUnitBitmap(4, L".\\adv\\mjpg\\k1\\common\\password_checkerror.bmp", L"", TRUE);
          m_MJPGList.SetUnitBitmap(5, L"", L"", TRUE);

          m_edtNewPassword1.ShowWindow(SW_HIDE);
          m_edtNewPassword2.ShowWindow(SW_HIDE);
          m_edtOldPassword.ShowWindow(SW_HIDE);
    } else if (m_passwordType == PASSWORD_OLDERROR) {
          m_MJPGList.SetUnitBitmap(3, L"", L"", TRUE);
          m_MJPGList.SetUnitBitmap(7, L"", L"", TRUE);

          m_MJPGList.SetUnitBitmap(2, L"", L"", TRUE);
          m_MJPGList.SetUnitBitmap(6, L"", L"", TRUE);

          m_MJPGList.SetUnitBitmap(4, L".\\adv\\mjpg\\k1\\common\\password_olderror.bmp", L"", TRUE);
          m_MJPGList.SetUnitBitmap(5, L"", L"", TRUE);

          m_edtNewPassword1.ShowWindow(SW_HIDE);
          m_edtNewPassword2.ShowWindow(SW_HIDE);
          m_edtOldPassword.ShowWindow(SW_HIDE);
    } else if (m_passwordType == PASSWORD_NEWERROR) {
          m_MJPGList.SetUnitBitmap(3, L"", L"", TRUE);
          m_MJPGList.SetUnitBitmap(7, L"", L"", TRUE);

          m_MJPGList.SetUnitBitmap(2, L"", L"", TRUE);
          m_MJPGList.SetUnitBitmap(6, L"", L"", TRUE);

          m_MJPGList.SetUnitBitmap(4, L".\\adv\\mjpg\\k1\\common\\password_newerror.bmp", L"", TRUE);
          m_MJPGList.SetUnitBitmap(5, L"", L"", TRUE);

          m_edtNewPassword1.ShowWindow(SW_HIDE);
          m_edtNewPassword2.ShowWindow(SW_HIDE);
          m_edtOldPassword.ShowWindow(SW_HIDE);
    } else {
          m_MJPGList.SetUnitBitmap(3, L"", L"", TRUE);
          m_MJPGList.SetUnitBitmap(7, L"", L"", TRUE);

          m_MJPGList.SetUnitBitmap(2, L"", L"", TRUE);
          m_MJPGList.SetUnitBitmap(6, L"", L"", TRUE);

        if (m_passwordType == CHECK_PLAYPASSWORD) {
            m_MJPGList.SetUnitBitmap(4, L".\\adv\\mjpg\\k1\\common\\password_play.bmp", L"", TRUE);
            m_MJPGList.SetUnitBitmap(5, L".\\adv\\mjpg\\k1\\common\\input_null.bmp", L"", TRUE);
        } else if (m_passwordType == CHECK_SUPPERPASSWORD) {
              m_MJPGList.SetUnitBitmap(4, L".\\adv\\mjpg\\k1\\common\\password_supper.bmp", L"", TRUE);
              m_MJPGList.SetUnitBitmap(5, L".\\adv\\mjpg\\k1\\common\\input_null.bmp", L"", TRUE);
        } else if (m_passwordType == CHECK_CLEARPASSWORD) {
              m_edtNewPassword1.SetLimitDiagital(FALSE);
              m_edtNewPassword1.SetLimitText(8);
              m_MJPGList.SetUnitBitmap(4, L".\\adv\\mjpg\\k1\\common\\password_clear.bmp", L"", TRUE);
              m_MJPGList.SetUnitBitmap(5, L".\\adv\\mjpg\\k1\\common\\input_null.bmp", L"", TRUE);
        } else if (m_passwordType == CHECK_LOCKPINPASSWORD || m_passwordType == CHECK_UNLOCKPINPASSWORD || m_passwordType == CHECK_PINPASSWORD)    //lxz 20090906
        {
              m_edtNewPassword1.SetLimitText(4);
              m_MJPGList.SetUnitBitmap(4, L".\\adv\\mjpg\\k1\\common\\password_pin.bmp", L"", TRUE);
              m_MJPGList.SetUnitBitmap(5, L".\\adv\\mjpg\\k1\\common\\input_null.bmp", L"", TRUE);
        } else if (m_passwordType == CHECK_PUKPASSWORD) {
              m_edtNewPassword1.SetLimitText(8);
              m_MJPGList.SetUnitBitmap(4, L".\\adv\\mjpg\\k1\\common\\password_puk.bmp", L"", TRUE);
              m_MJPGList.SetUnitBitmap(5, L".\\adv\\mjpg\\k1\\common\\input_null.bmp", L"", TRUE);
        }
          m_edtNewPassword1.ShowWindow(SW_SHOW);
          m_edtNewPassword2.ShowWindow(SW_HIDE);
          m_edtOldPassword.ShowWindow(SW_HIDE);
    }
}

void CPasswordDlg::SettingType(PASSWORD_TYPE nType, int nCtrl) {
    m_nCtrl = nCtrl;
    m_passwordType = nType;
    m_edtNewPassword1.SetWindowText(L"");
    m_edtNewPassword2.SetWindowText(L"");
    m_edtOldPassword.SetWindowText(L"");
    m_edtNewPassword1.SetLimitDiagital();
    m_edtNewPassword1.SetLimitText(6);

    m_MJPGList.SetUnitText(1, L"", FALSE);
    m_MJPGList.SetUnitText(2, L"", FALSE);
    m_MJPGList.SetUnitText(3, L"", FALSE);

    if (m_passwordType == SETTINGPLAY_PASSWORD || m_passwordType == SETTINGSUPPER_PASSWORD || m_passwordType == SETTINGPIN_PASSWORD || m_passwordType == SETTINGSCREEN_PASSWORD) {
        m_MJPGList.SetUnitText(1, L"ÇëÊäÈë¾ÉÃÜÂë", FALSE);
        m_MJPGList.SetUnitText(2, L"ÇëÊäÈëÐÂÃÜÂë", FALSE);
        m_MJPGList.SetUnitText(3, L"ÇëÔÙ´ÎÊäÈëÐÂÃÜÂë", FALSE);

        if (m_passwordType == SETTINGPLAY_PASSWORD) {
            m_MJPGList.SetUnitText(0, L"²¥·ÅÃÜÂëÉèÖÃ", FALSE);
        } else if (m_passwordType == SETTINGPIN_PASSWORD)   //lxz 20090906
        {
            m_edtNewPassword1.SetLimitDiagital();
            m_edtNewPassword1.SetLimitText(4);
            m_edtNewPassword2.SetLimitDiagital();
            m_edtNewPassword2.SetLimitText(4);
            m_edtOldPassword.SetLimitDiagital();
            m_edtOldPassword.SetLimitText(4);   

            m_MJPGList.SetUnitText(0, L"PINÂëÉèÖÃ", FALSE);
        } else if (m_passwordType == SETTINGSUPPER_PASSWORD) {
            m_MJPGList.SetUnitText(0, L"³¬¼¶ÃÜÂëÉèÖÃ", FALSE);
        } else if (m_passwordType == SETTINGSCREEN_PASSWORD) {
            m_MJPGList.SetUnitText(0, L"ÆÁ±£ÃÜÂëÉèÖÃ", FALSE);
        }
        SetTimer(1, 200, NULL);
    } else {
          m_MJPGList.SetUnitText(2, L"ÇëÊäÈëÃÜÂë", FALSE);
        if (m_passwordType == CHECK_PLAYPASSWORD) {
            m_MJPGList.SetUnitText(0, L"²¥·ÅÃÜÂëÑéÖ¤", FALSE);
        } else if (m_passwordType == CHECK_SUPPERPASSWORD) {
            m_MJPGList.SetUnitText(0, L"³¬¼¶ÃÜÂëÑéÖ¤", FALSE);
        } else if (m_passwordType == CHECK_CLEARPASSWORD) {
              m_edtNewPassword1.SetLimitDiagital(FALSE);
              m_edtNewPassword1.SetLimitText(8);
              m_MJPGList.SetUnitText(0, L"Çå¿ÕÃÜÂëÑéÖ¤", FALSE);
        } else if (m_passwordType == CHECK_LOCKPINPASSWORD || m_passwordType == CHECK_UNLOCKPINPASSWORD || m_passwordType == CHECK_PINPASSWORD)    //lxz 20090906
        {
            m_MJPGList.SetUnitText(0, L"PINÂëÑéÖ¤", FALSE);
            m_edtNewPassword1.SetLimitText(4);
        } else if (m_passwordType == CHECK_PUKPASSWORD) {
            m_MJPGList.SetUnitText(0, L"PUKÂëÑéÖ¤", FALSE);
            m_edtNewPassword1.SetLimitText(8);
        } else if (m_passwordType == CHECK_SCREENPASSWORD) {
            m_MJPGList.SetUnitText(0, L"ÆÁ±£ÂëÑéÖ¤", FALSE);
        }
          SetTimer(3, 200, NULL);
    }
    m_MJPGList.Invalidate();
}

void CPasswordDlg::OnTimer(UINT nIDEvent) {
    if (1 == nIDEvent) {
        m_edtNewPassword1.ShowWindow(SW_SHOW);
        m_edtNewPassword2.ShowWindow(SW_SHOW);
        m_edtOldPassword.ShowWindow(SW_SHOW);
    } else if (2 == nIDEvent) {
        m_edtNewPassword1.ShowWindow(SW_HIDE);
        m_edtNewPassword2.ShowWindow(SW_HIDE);
        m_edtOldPassword.ShowWindow(SW_HIDE);
    } else if (3 == nIDEvent) {
        m_edtNewPassword1.ShowWindow(SW_SHOW);
        m_edtNewPassword2.ShowWindow(SW_HIDE);
        m_edtOldPassword.ShowWindow(SW_HIDE);
    }
}

LRESULT CPasswordDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_DELETESELITEM:
        ::SendMessage(m_Owner, WM_SETTINGPASSWORD, (WPARAM)m_tempPassword, m_passwordType);
        break;
    }
    return CDialog::WindowProc(message, wParam, lParam);
}
