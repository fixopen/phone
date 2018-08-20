// deletetipdlg.cpp : implementation file
//

#include "stdafx.h"
#include "DeleteTipDlg.h"
#include "../MultimediaPhone.h"
#include "../Data/LanguageResource.h"
#include "../Data/SkinStyle.h"
//#include "sip.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace View {
    /////////////////////////////////////////////////////////////////////////////
    // CDeleteTipDlg dialog


    CDeleteTipDlg::CDeleteTipDlg(CWnd* pParent /*=NULL*/)
        : CCEDialog(CDeleteTipDlg::IDD, pParent)
    {
        //{{AFX_DATA_INIT(CDeleteTipDlg)
        // NOTE: the ClassWizard will add member initialization here
        //}}AFX_DATA_INIT
    }


    void CDeleteTipDlg::DoDataExchange(CDataExchange* pDX)
    {
        CDialog::DoDataExchange(pDX);
        //{{AFX_DATA_MAP(CDeleteTipDlg)
        // NOTE: the ClassWizard will add DDX and DDV calls here
        //}}AFX_DATA_MAP
    }


    BEGIN_MESSAGE_MAP(CDeleteTipDlg, CCEDialog)
        //{{AFX_MSG_MAP(CDeleteTipDlg)
        ON_WM_TIMER()
        //}}AFX_MSG_MAP
        ON_BN_CLICKED(IDC_BUTTON_DELETETIP_OK, OnButtonDeleteTipOk)
        ON_BN_CLICKED(IDC_BUTTON_DELETETIP_CANCEL, OnButtonDeleteTipCancel)
        ON_MESSAGE(WM_CLICKMJPG_TOAPP, OnClickMJPG)
    END_MESSAGE_MAP()

    /////////////////////////////////////////////////////////////////////////////
    // CDeleteTipDlg message handlers
    LRESULT CDeleteTipDlg::OnClickMJPG(WPARAM w, LPARAM l)
    {
        LRESULT result = 0;
        switch(w)
        {
        case 1:
            OnButtonDeleteTipOk();
            break;
        case 2:
            OnButtonDeleteTipCancel();
            break;
        }
        return 0;
    }

    BOOL CDeleteTipDlg::OnInitDialog() 
    {
        CDialog::OnInitDialog();

        // TODO: Add extra initialization here
        std::string strTemp;
        CString str;

        strTemp = Data::LanguageResource::Get(Data::RI_DELETETIP_TIP);
        str = strTemp.c_str();
        m_sticTip.Create(str, WS_CHILD|WS_VISIBLE, CRect(62+20 , 102 + 62, 580+31 , 132 + 62), this);
        m_sticTip.SetColor(RGB(0, 0, 0), RGB(203, 203, 203));//Data::g_partFrameInRectBackRGB[Data::g_skinstyle]);
        /*
        strTemp = Data::LanguageResource::Get(Data::RI_DELETETIP_PASSWORD);
        str = strTemp.c_str();
        m_sticPassword.Create(str, WS_CHILD, CRect(62, 123, 144, 141), this);
        m_sticPassword.SetColor(RGB(0, 0, 0), Data::g_partFrameInRectBackRGB[Data::g_skinstyle]);
        m_sticPassword.SetAlign(SS_CENTER);

        m_edtPassword.Create(WS_CHILD | ES_PASSWORD, CRect(148, 123, 338, 141), this, IDC_EDIT_DELETETIP_PASSWORD);
        m_edtPassword.SetLimitText(15);

        strTemp = Data::LanguageResource::Get(Data::RI_COMN_OKBTN);	
        str = strTemp.c_str();
        m_btnOk.Create(str, Data::g_buttonArcBMPPARTDILOAGID[0][Data::g_skinstyle], Data::g_buttonArcBMPPARTDILOAGID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(378, 78, 434, 98), this, IDC_BUTTON_DELETETIP_OK);
        m_btnOk.SetBackRGB(Data::g_partFrameMainBackRGB[Data::g_skinstyle]);

        strTemp = Data::LanguageResource::Get(Data::RI_COMN_CANCELBTN);
        str = strTemp.c_str();
        m_btnCancel.Create(str, Data::g_buttonArcBMPPARTDILOAGID[0][Data::g_skinstyle], Data::g_buttonArcBMPPARTDILOAGID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(378, 103, 434, 123), this, IDC_BUTTON_DELETETIP_CANCEL);
        m_btnCancel.SetBackRGB(Data::g_partFrameMainBackRGB[Data::g_skinstyle]);

        m_sticBackground.Create(CRect(34, 40, 448, 184), this, 1);

        TextStruct ts[1];
        memset(ts, 0, sizeof(TextStruct) * 1);

        ts[0].txtRect = CRect(8, 0, 100, 20);
        ts[0].txtFontSize = 16;
        ts[0].sAlign = DT_LEFT | DT_BOTTOM;
        memcpy(ts[0].sTxt, Data::LanguageResource::Get(Data::RI_DELETETIP_TITLE).c_str(), Data::LanguageResource::Get(Data::RI_DELETETIP_TITLE).length());

        m_sticBackground.SetTextStruct(ts, 1);
        */
        m_MJPGList.Create(L"", WS_VISIBLE|WS_CHILD, CRect(54, 62, 746, 358), this);
        m_MJPGList.SetCurrentLinkFile(_T(".\\adv\\mjpg\\k1\\中文\\删除确认.xml"));
        m_MJPGList.SetMJPGRect(CRect(54, 62, 746, 358));

        m_bIsPasswordModel = false;
        m_bIsTipModel = false;
        return TRUE;  // return TRUE unless you set the focus to a control
        // EXCEPTION: OCX Property Pages should return FALSE
    }

    void CDeleteTipDlg::OnButtonDeleteTipOk()
    {
        /*	if (m_bIsPasswordModel)
        {
        CString p;
        m_edtPassword.GetWindowText(p);
        if (m_sPassword == p)
        {
        // 			KillTimer(1);
        // 			SetTimer(1, 30*1000, 0);
        ShowWindow_(FALSE);	
        std::string strTemp = Data::LanguageResource::Get(Data::RI_DELETETIP_PASSWORD);
        CString str = strTemp.c_str();
        m_sticPassword.SetWindowText(str);
        m_edtPassword.SetWindowText(_T(""));
        SetPasswordModel(false);
        //::SendMessage(m_handle, WM_DELETESELITEM, 0, 0);
        ::PostMessage(m_handle, WM_DELETESELITEM, 0, 0);
        }
        else
        {
        std::string strTemp;
        strTemp = Data::LanguageResource::Get(Data::RI_PASSWORD_TIP3);
        SetDelTip(CString(strTemp.c_str()));
        m_edtPassword.SetWindowText(_T(""));
        return;
        }
        }
        else if (m_bIsTipModel)
        {
        ShowWindow_(FALSE);
        SetTipModel(false);
        }
        else*/
        {
            ShowWindow_(FALSE);
            SetPasswordModel(false);
            //::SendMessage(m_handle, WM_DELETESELITEM, 0, 0);
            ::PostMessage(m_handle, WM_DELETESELITEM, 0, 0);
        }
        //SipShowIM(SIPF_OFF);

        // 	TextStruct ts[1];
        // 	memset(ts, 0, sizeof(TextStruct) * 1);
        // 	
        // 	ts[0].txtRect = CRect(8, 0, 100, 20);
        // 	ts[0].txtFontSize = 16;
        // 	ts[0].sAlign = DT_LEFT | DT_BOTTOM;
        // 	memcpy(ts[0].sTxt, Data::LanguageResource::Get(Data::RI_DELETETIP_TITLE).c_str(), Data::LanguageResource::Get(Data::RI_DELETETIP_TITLE).length());
        // 	
        // 	m_sticBackground.SetTextStruct(ts, 1);
    }

    void CDeleteTipDlg::OnButtonDeleteTipCancel()
    {
        ShowWindow_(FALSE);
        //SipShowIM(SIPF_OFF);

        // 	TextStruct ts[1];
        // 	memset(ts, 0, sizeof(TextStruct) * 1);
        // 	
        // 	ts[0].txtRect = CRect(8, 0, 100, 20);
        // 	ts[0].txtFontSize = 16;
        // 	ts[0].sAlign = DT_LEFT | DT_BOTTOM;
        // 	memcpy(ts[0].sTxt, Data::LanguageResource::Get(Data::RI_DELETETIP_TITLE).c_str(), Data::LanguageResource::Get(Data::RI_DELETETIP_TITLE).length());
        // 	
        // 	m_sticBackground.SetTextStruct(ts, 1);
        // 	std::string strTemp = Data::LanguageResource::Get(Data::RI_DELETETIP_PASSWORD);
        // 	CString str = strTemp.c_str();
        // 	m_sticPassword.SetWindowText(str);
        // 	SetPasswordModel(false);
    }

    void CDeleteTipDlg::SetHWnd(HWND handle)
    {
        m_handle = handle;
    }

    void CDeleteTipDlg::OnTimer(UINT nIDEvent) 
    {
        // TODO: Add your message handler code here and/or call default
        KillTimer(nIDEvent);
        m_edtPassword.SetWindowText(_T(""));
        CCEDialog::OnTimer(nIDEvent);
    }

    void CDeleteTipDlg::SetPasswordModel(bool b)
    {
        // 	m_bIsPasswordModel = b;
        // 	m_sticPassword.ShowWindow(b);
        // 	m_edtPassword.ShowWindow(b);
    }

    void CDeleteTipDlg::SetPassword(CString password)
    {
        //	m_sPassword = password;
    }

    void CDeleteTipDlg::SetDelTip(CString tips)
    {
        m_sticTip.SetWindowText(tips);
    }

    void CDeleteTipDlg::SetTitle(std::string title)
    {
        // 	TextStruct ts[1];
        // 	memset(ts, 0, sizeof(TextStruct) * 1);
        // 	
        // 	ts[0].txtRect = CRect(8, 0, 100, 20);
        // 	ts[0].txtFontSize = 16;
        // 	ts[0].sAlign = DT_LEFT | DT_BOTTOM;
        // 	memcpy(ts[0].sTxt, title.c_str(), title.length());
        // 	
        // 	m_sticBackground.SetTextStruct(ts, 1);
    }

    void CDeleteTipDlg::SetTipModel(BOOL b)
    {
        //	m_bIsTipModel = b;
        //	m_btnCancel.ShowWindow(!b);
    }
}
