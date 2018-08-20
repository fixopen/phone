// passworddlg.cpp : implementation file
//

#include "stdafx.h"
#include "PasswordDlg.h"
#include "../MultimediaPhone.h"
#include "../Data/LanguageResource.h"
#include "../Data/SkinStyle.h"
//#include "sip.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPasswordDlg dialog

namespace View {
    CPasswordDlg::CPasswordDlg(CWnd* pParent /*=NULL*/)
        : CCEDialog(CPasswordDlg::IDD, pParent)
    {
        //{{AFX_DATA_INIT(CPasswordDlg)
        // NOTE: the ClassWizard will add member initialization here
        //}}AFX_DATA_INIT
        m_passwordType = SETTING_PASSWORD;
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
    LRESULT CPasswordDlg::OnClickMJPG(WPARAM w, LPARAM l)
    {
        LRESULT result = 0;
        switch(w)
        {
        case 1:
            OnButtonPasswordOk();
            break;
        case 2:
            OnButtonPasswordCancel();
            break;
        }
        return result;
    }
    BOOL CPasswordDlg::OnInitDialog() 
    {
        CDialog::OnInitDialog();

        // TODO: Add extra initialization here

        std::string strTemp;
        CString str;
        /*
        strTemp = Data::LanguageResource::Get(Data::RI_PASSWORD_TIP);
        str = strTemp.c_str();
        m_sticTip.Create(str, WS_CHILD|WS_VISIBLE, CRect(62, 76, 340, 94), this);
        m_sticTip.SetColor(RGB(0, 0, 0), Data::g_partFrameInRectBackRGB[Data::g_skinstyle]);

        strTemp = Data::LanguageResource::Get(Data::RI_PASSWORD_OLDPASSWORD);
        str = strTemp.c_str();
        m_sticOldPassword.Create(str, WS_CHILD|WS_VISIBLE, CRect(50, 98, 144, 116), this);
        m_sticOldPassword.SetColor(RGB(0, 0, 0), Data::g_partFrameInRectBackRGB[Data::g_skinstyle]);
        m_sticOldPassword.SetAlign(SS_RIGHT);	
        */
        m_edtOldPassword.Create(WS_CHILD|WS_VISIBLE|ES_PASSWORD, CRect(166 + 54, 144 + 62, 524 + 54, 175 + 62), this, IDC_SETTING_PASSWORD1);
        m_edtOldPassword.SetLimitText(15);
        m_edtOldPassword.SetLimitDiagital();
        /*
        strTemp = Data::LanguageResource::Get(Data::RI_PASSWORD_NEWPASSWORD1);
        str = strTemp.c_str();
        m_sticNewPassword1.Create(str, WS_CHILD|WS_VISIBLE, CRect(50, 123, 144, 141), this);
        m_sticNewPassword1.SetColor(RGB(0, 0, 0), Data::g_partFrameInRectBackRGB[Data::g_skinstyle]);
        m_sticNewPassword1.SetAlign(SS_RIGHT);	

        m_edtNewPassword1.Create(WS_CHILD|WS_VISIBLE|ES_PASSWORD, CRect(148, 123, 338, 141), this, IDC_SETTING_PASSWORD2);
        m_edtNewPassword1.SetLimitText(15);

        strTemp = Data::LanguageResource::Get(Data::RI_PASSWORD_NEWPASSWORD2);
        str = strTemp.c_str();
        m_sticNewPassword2.Create(str, WS_CHILD|WS_VISIBLE, CRect(50, 148, 144, 166), this);
        m_sticNewPassword2.SetColor(RGB(0, 0, 0), Data::g_partFrameInRectBackRGB[Data::g_skinstyle]);
        m_sticNewPassword2.SetAlign(SS_RIGHT);	

        m_edtNewPassword2.Create(WS_CHILD|WS_VISIBLE|ES_PASSWORD, CRect(148, 148, 338, 166), this, IDC_SETTING_PASSWORD3);
        m_edtNewPassword2.SetLimitText(15);

        strTemp = Data::LanguageResource::Get(Data::RI_COMN_OKBTN);
        str = strTemp.c_str();
        m_btnOk.Create(str, Data::g_buttonArcBMPPARTDILOAGID[0][Data::g_skinstyle], Data::g_buttonArcBMPPARTDILOAGID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(378, 78, 434, 98), this, IDC_BUTTON_PASSWORD_OK);
        m_btnOk.SetBackRGB(Data::g_partFrameMainBackRGB[Data::g_skinstyle]);

        strTemp = Data::LanguageResource::Get(Data::RI_COMN_CANCELBTN);
        str = strTemp.c_str();
        m_btnCancel.Create(str, Data::g_buttonArcBMPPARTDILOAGID[0][Data::g_skinstyle], Data::g_buttonArcBMPPARTDILOAGID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(378, 103, 434, 123), this, IDC_BUTTON_PASSWORD_CANCEL);
        m_btnCancel.SetBackRGB(Data::g_partFrameMainBackRGB[Data::g_skinstyle]);

        m_sticBackground.Create(CRect(34, 40, 448, 184), this, 1);

        TextStruct ts[1];
        memset(ts, 0, sizeof(TextStruct) * 1);

        ts[0].txtRect = CRect(8, 0, 100, 20);
        ts[0].txtFontSize = 16;
        ts[0].sAlign = DT_LEFT | DT_BOTTOM;
        memcpy(ts[0].sTxt, Data::LanguageResource::Get(Data::RI_PASSWORD_TITLE).c_str(), Data::LanguageResource::Get(Data::RI_PASSWORD_TITLE).length());

        m_sticBackground.SetTextStruct(ts, 1);
        */
        m_MJPGList.Create(L"", WS_VISIBLE|WS_CHILD, CRect(54, 62, 746, 358), this);
        m_MJPGList.SetCurrentLinkFile(_T(".\\adv\\mjpg\\k1\\中文\\密码验证.xml"));
        m_MJPGList.SetMJPGRect(CRect(54, 62, 746, 358));

        return TRUE;  // return TRUE unless you set the focus to a control
        // EXCEPTION: OCX Property Pages should return FALSE
    }

    void CPasswordDlg::SetStaticDefaultColor(CCEStatic* cestatic)
    {
        // 	cestatic->SetColor(RGB(0, 0, 0), RGB(194,235,149));
        cestatic->SetColor(RGB(0, 0, 0), RGB(255,255,255));
    }

    void CPasswordDlg::OnButtonPasswordOk()
    {
        CString s;
        BOOL isError = FALSE;
        if(m_passwordType == SETTING_PASSWORD)
        {
            CString s1, s2, s3;
            GetDlgItemText(IDC_SETTING_PASSWORD1, s1);
            GetDlgItemText(IDC_SETTING_PASSWORD2, s2);
            GetDlgItemText(IDC_SETTING_PASSWORD3, s3);
            if(m_password != s1)
            {
                std::string strTemp;
                strTemp = Data::LanguageResource::Get(Data::RI_PASSWORD_TIP1);
                s = strTemp.c_str();
                SetDlgItemText(IDC_SETTING_PASSWORD1, L"");
                isError = TRUE;
            }
            else if(s2 != s3)
            {
                std::string strTemp;
                strTemp = Data::LanguageResource::Get(Data::RI_PASSWORD_TIP2);
                s = strTemp.c_str();
                SetDlgItemText(IDC_SETTING_PASSWORD2, L"");
                SetDlgItemText(IDC_SETTING_PASSWORD3, L"");
                isError = TRUE;
            }
            else
            {
                //SipShowIM(SIPF_OFF);
                //给主窗口发送新的密码
                ShowWindow_(FALSE);
                //			theApp.inputDlg_->ShowWindow(FALSE);
                if(m_Owner)
                {
                    std::string strtemp = Util::StringOp::FromCString(s2);
                    const char *temp = strtemp.c_str();
                    ::SendMessage(m_Owner, WM_SETTINGPASSWORD, (WPARAM)temp, strlen(temp));
                    return;
                }
            }
        }
        else 
        {
            CString s1;
            GetDlgItemText(IDC_SETTING_PASSWORD1, s1);

            if(m_password != s1)
            {
                std::string strTemp;
                strTemp = Data::LanguageResource::Get(Data::RI_PASSWORD_TIP3);
                s = strTemp.c_str();
                SetDlgItemText(IDC_SETTING_PASSWORD1, L"");
                isError = TRUE;
            }
            else
            {
                //SipShowIM(SIPF_OFF);
                ShowWindow_(FALSE);
                //	theApp.inputDlg_->ShowWindow(FALSE);
                if(m_Owner)
                    ::SendMessage(m_Owner, WM_CHECKPASSWORD, 0, 0);
            }
        }
        if(isError)
        {
            m_sticTip.SetWindowText(s);
        }
    }

    void CPasswordDlg::OnButtonPasswordCancel()
    {
        //SipShowIM(SIPF_OFF);
        ShowWindow_(FALSE); 
    }

    void CPasswordDlg::SetOldPassWord(char *pPassWord)
    {
        m_password = pPassWord;
    }

    void CPasswordDlg::SetType(PASSWORD_TYPE nType)
    {
        std::string strTemp = Data::LanguageResource::Get(Data::RI_PASSWORD_TIP);
        CString str = Util::StringOp::ToCString(strTemp.c_str());
        m_sticTip.SetWindowText(str);
        m_edtNewPassword1.SetWindowText(L"");
        m_edtNewPassword2.SetWindowText(L"");
        m_edtOldPassword.SetWindowText(L"");

        m_passwordType = nType;
        TextStruct ts[1];
        memset(ts, 0, sizeof(TextStruct) * 1);

        ts[0].txtRect = CRect(8, 0, 100, 20);
        ts[0].txtFontSize = 16;
        ts[0].sAlign = DT_LEFT | DT_BOTTOM;
        if(m_passwordType == SETTING_PASSWORD)
        {
            memcpy(ts[0].sTxt, Data::LanguageResource::Get(Data::RI_PASSWORD_TITLE).c_str(), Data::LanguageResource::Get(Data::RI_PASSWORD_TITLE).length());
            m_sticNewPassword1.ShowWindow(SW_SHOW);
            m_sticNewPassword2.ShowWindow(SW_SHOW);
            m_edtNewPassword1.ShowWindow(SW_SHOW);
            m_edtNewPassword2.ShowWindow(SW_SHOW);
            m_sticOldPassword.ShowWindow(SW_SHOW);
            m_edtOldPassword.ShowWindow(SW_SHOW);
        }
        else
        {
            m_sticNewPassword1.ShowWindow(SW_HIDE);
            m_sticNewPassword2.ShowWindow(SW_HIDE);
            m_edtNewPassword1.ShowWindow(SW_HIDE);
            m_edtNewPassword2.ShowWindow(SW_HIDE);
            m_sticOldPassword.ShowWindow(SW_HIDE);
            m_edtOldPassword.ShowWindow(SW_SHOW);
            memcpy(ts[0].sTxt, Data::LanguageResource::Get(Data::RI_PASSWORD_TITLE1).c_str(), Data::LanguageResource::Get(Data::RI_PASSWORD_TITLE).length());
        }
        m_sticBackground.SetTextStruct(ts, 1);
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
}
