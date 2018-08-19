// fastdialsdlg.cpp : implementation file
//

#include "stdafx.h"
#include "..\multimediaphone.h"
#include "SpecCodedlg.h"
#include "../Data/LanguageResource.h"
#include "../Data/SkinStyle.h"
#include "sip.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSpecCodeDlg dialog


CSpecCodeDlg::CSpecCodeDlg(CWnd* pParent /*=NULL*/)
: CCEDialog(CSpecCodeDlg::IDD, pParent) {
    //{{AFX_DATA_INIT(CSpecCodeDlg)
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT

}


void CSpecCodeDlg::DoDataExchange(CDataExchange* pDX) {
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CSpecCodeDlg)
    // NOTE: the ClassWizard will add DDX and DDV calls here
    //}}AFX_DATA_MAP

}


BEGIN_MESSAGE_MAP(CSpecCodeDlg, CCEDialog)
    //{{AFX_MSG_MAP(CSpecCodeDlg)
    //}}AFX_MSG_MAP
    ON_BN_CLICKED(IDC_BUTTON_FASTDIALS_OK, OnButtonFastDialsOk)
    ON_BN_CLICKED(IDC_BUTTON_FASTDIALS_CANCEL, OnButtonFastDialsCancel)
    ON_MESSAGE(WM_CLICKMJPG_TOAPP, OnClickMJPG)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSpecCodeDlg message handlers
LRESULT CSpecCodeDlg::OnClickMJPG(WPARAM w, LPARAM l) {
    LRESULT result = 0;
    switch (w) {
    case 100:
        OnButtonFastDialsOk();
        break;
    case 101:
        OnButtonFastDialsCancel();
        break;
    }
    return result;
}

BOOL CSpecCodeDlg::OnInitDialog() {
    CDialog::OnInitDialog();

    // TODO: Add extra initialization here
    std::string strTemp;
    CString str;
    /*  
    strTemp = Data::LanguageResource::Get(Data::RI_FASTDIALS_NUMBER01);
    str = strTemp.c_str();
    m_sticNumber01.Create(str, WS_CHILD|WS_VISIBLE, CRect(52, 72, 102, 88), this);
    m_sticNumber01.SetColor(RGB(0, 0, 0), Data::g_partFrameInRectBackRGB[Data::g_skinstyle]);
    m_sticNumber01.SetAlign(SS_CENTER);
    strTemp = Data::LanguageResource::Get(Data::RI_FASTDIALS_NUMBER02);
    str = strTemp.c_str();
    m_sticNumber02.Create(str, WS_CHILD|WS_VISIBLE, CRect(52, 89, 102, 105), this);
    m_sticNumber02.SetColor(RGB(0, 0, 0), Data::g_partFrameInRectBackRGB[Data::g_skinstyle]);
    m_sticNumber02.SetAlign(SS_CENTER);
    strTemp = Data::LanguageResource::Get(Data::RI_FASTDIALS_NUMBER03);
    str = strTemp.c_str();
    m_sticNumber03.Create(str, WS_CHILD|WS_VISIBLE, CRect(52, 106, 102, 122), this);
    m_sticNumber03.SetColor(RGB(0, 0, 0), Data::g_partFrameInRectBackRGB[Data::g_skinstyle]);
    m_sticNumber03.SetAlign(SS_CENTER);
    strTemp = Data::LanguageResource::Get(Data::RI_FASTDIALS_NUMBER04);
    str = strTemp.c_str();
    m_sticNumber04.Create(str, WS_CHILD|WS_VISIBLE, CRect(52, 123, 102, 139), this);
    m_sticNumber04.SetColor(RGB(0, 0, 0), Data::g_partFrameInRectBackRGB[Data::g_skinstyle]);
    m_sticNumber04.SetAlign(SS_CENTER);
    strTemp = Data::LanguageResource::Get(Data::RI_FASTDIALS_NUMBER05);
    str = strTemp.c_str();
    m_sticNumber05.Create(str, WS_CHILD|WS_VISIBLE, CRect(52, 140, 102, 156), this);
    m_sticNumber05.SetColor(RGB(0, 0, 0), Data::g_partFrameInRectBackRGB[Data::g_skinstyle]);
    m_sticNumber05.SetAlign(SS_CENTER);
    strTemp = Data::LanguageResource::Get(Data::RI_FASTDIALS_NUMBER06);
    str = strTemp.c_str();
    m_sticNumber06.Create(str, WS_CHILD|WS_VISIBLE, CRect(52, 157, 102, 173), this);
    m_sticNumber06.SetColor(RGB(0, 0, 0), Data::g_partFrameInRectBackRGB[Data::g_skinstyle]);
    m_sticNumber06.SetAlign(SS_CENTER);
    strTemp = Data::LanguageResource::Get(Data::RI_FASTDIALS_NUMBER07);
    str = strTemp.c_str();
    m_sticNumber07.Create(str, WS_CHILD|WS_VISIBLE, CRect(206, 72, 258, 88), this);
    m_sticNumber07.SetColor(RGB(0, 0, 0), Data::g_partFrameInRectBackRGB[Data::g_skinstyle]);
    m_sticNumber07.SetAlign(SS_CENTER);
    strTemp = Data::LanguageResource::Get(Data::RI_FASTDIALS_NUMBER08);
    str = strTemp.c_str();
    m_sticNumber08.Create(str, WS_CHILD|WS_VISIBLE, CRect(206, 89, 258, 105), this);
    m_sticNumber08.SetColor(RGB(0, 0, 0), Data::g_partFrameInRectBackRGB[Data::g_skinstyle]);
    m_sticNumber08.SetAlign(SS_CENTER);
    strTemp = Data::LanguageResource::Get(Data::RI_FASTDIALS_NUMBER09);
    str = strTemp.c_str();
    m_sticNumber09.Create(str, WS_CHILD|WS_VISIBLE, CRect(206, 106, 258, 122), this);
    m_sticNumber09.SetColor(RGB(0, 0, 0), Data::g_partFrameInRectBackRGB[Data::g_skinstyle]);
    m_sticNumber09.SetAlign(SS_CENTER);
    strTemp = Data::LanguageResource::Get(Data::RI_FASTDIALS_NUMBER10);
    str = strTemp.c_str();
    m_sticNumber10.Create(str, WS_CHILD|WS_VISIBLE, CRect(206, 123, 258, 139), this);
    m_sticNumber10.SetColor(RGB(0, 0, 0), Data::g_partFrameInRectBackRGB[Data::g_skinstyle]);
    m_sticNumber10.SetAlign(SS_CENTER);
    strTemp = Data::LanguageResource::Get(Data::RI_FASTDIALS_NUMBER11);
    str = strTemp.c_str();
    m_sticNumber11.Create(str, WS_CHILD|WS_VISIBLE, CRect(206, 140, 258, 156), this);
    m_sticNumber11.SetColor(RGB(0, 0, 0), Data::g_partFrameInRectBackRGB[Data::g_skinstyle]);
    m_sticNumber11.SetAlign(SS_CENTER);
    strTemp = Data::LanguageResource::Get(Data::RI_FASTDIALS_NUMBER12);
    str = strTemp.c_str();
    m_sticNumber12.Create(str, WS_CHILD|WS_VISIBLE, CRect(206, 157, 258, 173), this);
    m_sticNumber12.SetColor(RGB(0, 0, 0), Data::g_partFrameInRectBackRGB[Data::g_skinstyle]);
    m_sticNumber12.SetAlign(SS_CENTER);
    */  
    for (int i = 0; i < 12; ++i) {
        if (i < 6) {
            m_edtNumber[i].Create(WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, CRect(107 + 54, 78 + i * 32 + 62, 286 + 54, 104 + i * 32 + 62), this, IDC_EDIT_FASTDIALS_NUMBER01);
            m_edtNumber[i].SetLimitText(15);
            m_edtNumber[i].SetLimitDiagital();
        } else {
              m_edtNumber[i].Create(WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, CRect(366 + 54, 78 + (i - 6) * 32 + 62, 544 + 54, 104 + (i - 6) * 32 + 62), this, IDC_EDIT_FASTDIALS_NUMBER01);
              m_edtNumber[i].SetLimitText(15);
            if (i != 11) {
                m_edtNumber[i].SetLimitDiagital();
            }
        }
    }
    /*
    m_edtNumber[0].Create(WS_CHILD|WS_VISIBLE, CRect(108, 78, 282, 102), this, IDC_EDIT_FASTDIALS_NUMBER01);
    m_edtNumber[0].SetLimitText(15);
    m_edtNumber[1].Create(WS_CHILD|WS_VISIBLE, CRect(104, 110, 204, 105), this, IDC_EDIT_FASTDIALS_NUMBER02);
    m_edtNumber[1].SetLimitText(15);
    m_edtNumber[2].Create(WS_CHILD|WS_VISIBLE, CRect(104, 106, 204, 122), this, IDC_EDIT_FASTDIALS_NUMBER03);
    m_edtNumber[2].SetLimitText(15);
    m_edtNumber[3].Create(WS_CHILD|WS_VISIBLE, CRect(104, 123, 204, 139), this, IDC_EDIT_FASTDIALS_NUMBER04);
    m_edtNumber[3].SetLimitText(15);
    m_edtNumber[4].Create(WS_CHILD|WS_VISIBLE, CRect(104, 140, 204, 156), this, IDC_EDIT_FASTDIALS_NUMBER05);
    m_edtNumber[4].SetLimitText(15);
    m_edtNumber[5].Create(WS_CHILD|WS_VISIBLE, CRect(104, 157, 204, 173), this, IDC_EDIT_FASTDIALS_NUMBER06);
    m_edtNumber[5].SetLimitText(15);
    m_edtNumber[6].Create(WS_CHILD|WS_VISIBLE, CRect(260, 72, 360, 88), this, IDC_EDIT_FASTDIALS_NUMBER07);
    m_edtNumber[6].SetLimitText(15);
    m_edtNumber[7].Create(WS_CHILD|WS_VISIBLE, CRect(260, 89, 360, 105), this, IDC_EDIT_FASTDIALS_NUMBER08);
    m_edtNumber[7].SetLimitText(15);
    m_edtNumber[8].Create(WS_CHILD|WS_VISIBLE, CRect(260, 106, 360, 122), this, IDC_EDIT_FASTDIALS_NUMBER09);
    m_edtNumber[8].SetLimitText(15);
    m_edtNumber[9].Create(WS_CHILD|WS_VISIBLE, CRect(260, 123, 360, 139), this, IDC_EDIT_FASTDIALS_NUMBER10);
    m_edtNumber[9].SetLimitText(15);
    m_edtNumber[10].Create(WS_CHILD|WS_VISIBLE, CRect(260, 140, 360, 156), this, IDC_EDIT_FASTDIALS_NUMBER11);
    m_edtNumber[10].SetLimitText(15);
    m_edtNumber[11].Create(WS_CHILD|WS_VISIBLE, CRect(260, 157, 360, 173), this, IDC_EDIT_FASTDIALS_NUMBER12);
    m_edtNumber[11].SetLimitText(15);
    strTemp = Data::LanguageResource::Get(Data::RI_COMN_OKBTN);
    str = strTemp.c_str();
    m_btnOk.Create(str, Data::g_buttonArcBMPPARTDILOAGID[0][Data::g_skinstyle], Data::g_buttonArcBMPPARTDILOAGID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(378, 78, 434, 98), this, IDC_BUTTON_FASTDIALS_OK);
    m_btnOk.SetBackRGB(Data::g_partFrameMainBackRGB[Data::g_skinstyle]);
    strTemp = Data::LanguageResource::Get(Data::RI_COMN_CANCELBTN);
    str = strTemp.c_str();
    m_btnCancel.Create(str, Data::g_buttonArcBMPPARTDILOAGID[0][Data::g_skinstyle], Data::g_buttonArcBMPPARTDILOAGID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(378, 103, 434, 123), this, IDC_BUTTON_FASTDIALS_CANCEL);
    m_btnCancel.SetBackRGB(Data::g_partFrameMainBackRGB[Data::g_skinstyle]);
    m_sticBackground.Create(CRect(34, 40, 448, 184), this, 1);
    TextStruct ts[1];
    memset(ts, 0, sizeof(TextStruct) * 1);
    ts[0].txtRect = CRect(8, 0, 100, 20);
    ts[0].txtFontSize = 16;
    ts[0].sAlign = DT_LEFT | DT_BOTTOM;
    memcpy(ts[0].sTxt, Data::LanguageResource::Get(Data::RI_FASTDIALS_TITLE).c_str(), Data::LanguageResource::Get(Data::RI_FASTDIALS_TITLE).length());
    m_sticBackground.SetTextStruct(ts, 1);
    */
    m_MJPGList.Create(L"", WS_VISIBLE | WS_CHILD, CRect(54, 62, 746, 358), this);
    m_MJPGList.SetCurrentLinkFile(".\\adv\\mjpg\\k1\\中文\\特服号.xml");
    m_MJPGList.SetMJPGRect(CRect(54, 62, 746, 358));

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CSpecCodeDlg::OnButtonFastDialsOk() {
    if (m_Data) {
        int i = 0;
        CString s;
        m_edtNumber[i++].GetWindowText(s);
        m_Data->speCode1_ = Util::StringOp::FromCString(s);
        m_edtNumber[i++].GetWindowText(s);
        m_Data->speCode2_ = Util::StringOp::FromCString(s);
        m_edtNumber[i++].GetWindowText(s);
        m_Data->speCode3_ = Util::StringOp::FromCString(s);
        m_edtNumber[i++].GetWindowText(s);
        m_Data->speCode4_ = Util::StringOp::FromCString(s);
        m_edtNumber[i++].GetWindowText(s);
        m_Data->speCode5_ = Util::StringOp::FromCString(s);
        m_edtNumber[i++].GetWindowText(s);
        m_Data->speCode6_ = Util::StringOp::FromCString(s);
        m_edtNumber[i++].GetWindowText(s);
        m_Data->speCode7_ = Util::StringOp::FromCString(s);
        m_edtNumber[i++].GetWindowText(s);
        m_Data->speCode8_ = Util::StringOp::FromCString(s);
        m_edtNumber[i++].GetWindowText(s);
        m_Data->speCode9_ = Util::StringOp::FromCString(s);
        m_edtNumber[i++].GetWindowText(s);
        m_Data->speCode10_ = Util::StringOp::FromCString(s);
        m_edtNumber[i++].GetWindowText(s);
        m_Data->speCode11_ = Util::StringOp::FromCString(s);
        m_edtNumber[i++].GetWindowText(s);
        m_Data->speCode12_ = Util::StringOp::FromCString(s);
        m_Data->Update();
    }

    SipShowIM(SIPF_OFF);

    ShowWindow_(FALSE);
    SipShowIM(SIPF_OFF);
}

void CSpecCodeDlg::OnButtonFastDialsCancel() {
    SipShowIM(SIPF_OFF);
    ShowWindow_(FALSE);
    SipShowIM(SIPF_OFF);
}

void CSpecCodeDlg::SetSpecCodeParam(boost::shared_ptr<Data::Setting> data) {
    m_Data = data;
    int idx = 0;
    std::string str = m_Data->speCode1_;
    m_edtNumber[idx++].SetWindowText(Util::StringOp::ToCString(str));
    str = m_Data->speCode2_;
    m_edtNumber[idx++].SetWindowText(Util::StringOp::ToCString(str));
    str = m_Data->speCode3_;
    m_edtNumber[idx++].SetWindowText(Util::StringOp::ToCString(str));
    str = m_Data->speCode4_;
    m_edtNumber[idx++].SetWindowText(Util::StringOp::ToCString(str));
    str = m_Data->speCode5_;
    m_edtNumber[idx++].SetWindowText(Util::StringOp::ToCString(str));
    str = m_Data->speCode6_;
    m_edtNumber[idx++].SetWindowText(Util::StringOp::ToCString(str));
    str = m_Data->speCode7_;
    m_edtNumber[idx++].SetWindowText(Util::StringOp::ToCString(str));
    str = m_Data->speCode8_;
    m_edtNumber[idx++].SetWindowText(Util::StringOp::ToCString(str));
    str = m_Data->speCode9_;
    m_edtNumber[idx++].SetWindowText(Util::StringOp::ToCString(str));
    str = m_Data->speCode10_;
    m_edtNumber[idx++].SetWindowText(Util::StringOp::ToCString(str));
    str = m_Data->speCode11_;
    m_edtNumber[idx++].SetWindowText(Util::StringOp::ToCString(str));
    str = m_Data->speCode12_;
    m_edtNumber[idx++].SetWindowText(Util::StringOp::ToCString(str));
}
