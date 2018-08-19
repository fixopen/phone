// fastdialsdlg.cpp : implementation file
//

#include "stdafx.h"
#include "..\multimediaphone.h"
#include "PinSetDlg.h"
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
// CPinSetDlg dialog


CPinSetDlg::CPinSetDlg(CWnd* pParent /*=NULL*/)
: CCEDialog(CPinSetDlg::IDD, pParent) {
    //{{AFX_DATA_INIT(CPinSetDlg)
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT

}


void CPinSetDlg::DoDataExchange(CDataExchange* pDX) {
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CPinSetDlg)
    // NOTE: the ClassWizard will add DDX and DDV calls here
    //}}AFX_DATA_MAP

}


BEGIN_MESSAGE_MAP(CPinSetDlg, CCEDialog)
    //{{AFX_MSG_MAP(CPinSetDlg)
    //}}AFX_MSG_MAP
    ON_BN_CLICKED(IDC_BUTTON_FASTDIALS_OK, OnButtonFastDialsOk)
    ON_BN_CLICKED(IDC_BUTTON_FASTDIALS_CANCEL, OnButtonFastDialsCancel)
    ON_MESSAGE(WM_CLICKMJPG_TOAPP, OnClickMJPG)
    ON_MESSAGE(WM_CHECKPASSWORD, OnCheckPIN)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPinSetDlg message handlers
LRESULT CPinSetDlg::OnClickMJPG(WPARAM w, LPARAM l) {
    LRESULT result = 0;
    switch (w) {
    case 41:
    case 42:
        if (41 == w) {
            m_MJPGList.SetUnitIsDownStatus(w + 1, FALSE);
            m_MJPGList.SetUnitIsShow(w + 1, TRUE);
            m_pPasswordDlg->SettingType(CHECK_LOCKPINPASSWORD);
        } else if (42 == w) {
            m_MJPGList.SetUnitIsDownStatus(w - 1, FALSE);
            m_MJPGList.SetUnitIsShow(w - 1, TRUE);
            m_pPasswordDlg->SettingType(CHECK_UNLOCKPINPASSWORD);
        }
        m_MJPGList.SetUnitIsDownStatus(w, TRUE);
        m_MJPGList.SetUnitIsShow(w, TRUE);
        m_pPasswordDlg->SetHWnd(this->m_hWnd);
        m_pPasswordDlg->ShowWindow_(SW_SHOW);
        break;
    case 43:
        if (m_bPin) {
            m_pPasswordDlg->SettingType(SETTINGPIN_PASSWORD);
            m_pPasswordDlg->SetHWnd(this->m_hWnd);
            m_pPasswordDlg->ShowWindow_(SW_SHOW);
        }
        break;
    case 44:
        break;
    case 1001:
        OnButtonFastDialsOk();
        break;
    case 1000:
        OnButtonFastDialsCancel();
        break;
    }
    return result;
}

LRESULT CPinSetDlg::OnCheckPIN(WPARAM w, LPARAM l) {
    LRESULT result = 0;
    SipShowIM(SIPF_OFF);
    ShowWindow_(FALSE);
    SipShowIM(SIPF_OFF);
    return result;
}

void CPinSetDlg::SetButtonDefaultColor(CCEButtonST* button) {
    button->SetColor(CCEButtonST::BTNST_COLOR_BK_IN, RGB(206, 206, 206));
    button->SetColor(CCEButtonST::BTNST_COLOR_BK_OUT, RGB(206, 206, 206));
    button->SetColor(CCEButtonST::BTNST_COLOR_BK_FOCUS, RGB(206, 206, 206));
}

BOOL CPinSetDlg::OnInitDialog() {
    CDialog::OnInitDialog();

    // TODO: Add extra initialization here
    m_edtPinNumber.Create(WS_CHILD | WS_VISIBLE, CRect(138, 128 + 57, 138 + 212, 128 + 30 + 57), this, IDC_EDIT_PINNUMBER);
    m_edtPinNumber.SetLimitText(15);
    m_edtPinNumber.SetLimitDiagital();

    m_MJPGList.Create(L"", WS_VISIBLE | WS_CHILD, CRect(100, 137, 540, 407), this);
    m_MJPGList.SetCurrentLinkFile(".\\adv\\mjpg\\k5\\ÖÐÎÄ\\ÉèÖÃPINÂë.xml");
    m_MJPGList.SetMJPGRect(CRect(100, 137, 540, 407));

    m_pPasswordDlg = new CPasswordDlg(this);
    m_pPasswordDlg->Create(CPasswordDlg::IDD);

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CPinSetDlg::OnButtonFastDialsOk() {
    SipShowIM(SIPF_OFF);
    ShowWindow_(FALSE);
    SipShowIM(SIPF_OFF);
}

void CPinSetDlg::OnButtonFastDialsCancel() {
    SipShowIM(SIPF_OFF);
    ShowWindow_(FALSE);
    SipShowIM(SIPF_OFF);
}

void CPinSetDlg::SetCallSetParam() {
    int nStatus;

    extern Util::ATCommandWarp * GetATCommandWarp();
    Util::ATCommandWarp* pATCommanWarp = GetATCommandWarp();
    pATCommanWarp->GetSimStatus(nStatus);
    m_bPin = (BOOL)nStatus;

    if (m_bPin) {
        m_MJPGList.SetUnitIsDownStatus(41, TRUE);
        m_MJPGList.SetUnitIsDownStatus(42, FALSE);
    } else {
        m_MJPGList.SetUnitIsDownStatus(41, FALSE);
        m_MJPGList.SetUnitIsDownStatus(42, TRUE);
    }
}
