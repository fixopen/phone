// TipDlg.cpp : implementation file
//

#include "stdafx.h"
#include "../multimediaphone.h"
#include "../MultimediaPhoneDlg.h"
#include "TipDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTipDlg dialog


CTipDlg::CTipDlg(CWnd* pParent /*=NULL*/)
: CDialog(CTipDlg::IDD, pParent) {
    //{{AFX_DATA_INIT(CTipDlg)
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT

}


void CTipDlg::DoDataExchange(CDataExchange* pDX) {
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CTipDlg)
    // NOTE: the ClassWizard will add DDX and DDV calls here
    //}}AFX_DATA_MAP

}


BEGIN_MESSAGE_MAP(CTipDlg, CDialog)
    //{{AFX_MSG_MAP(CTipDlg)
    ON_BN_CLICKED(IDC_BTN_OK, OnClickOK)
    //}}AFX_MSG_MAP
    ON_MESSAGE(WM_CLICKMJPG_TOAPP, OnClickMJPG)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTipDlg message handlers

BOOL CTipDlg::OnInitDialog() {
    CDialog::OnInitDialog();
    // TODO: Add extra initialization here

    m_MJPGList.Create(L"", WS_VISIBLE | WS_CHILD, CRect(0, 0, 440, 270), this);
    m_MJPGList.SetCurrentLinkFile(L"./adv/mjpg/k5/中文/提示语.xml");

    MoveWindow(100, 100, 440, 270);

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

LRESULT CTipDlg::OnClickMJPG(WPARAM w, LPARAM l) {
    LRESULT result = 0;
    switch (w) {
    case 1000:
        ShowWindow(SW_HIDE);
        break;
    case 1001:
        OnClickOK();
        break;
    }
    return result;
}

void CTipDlg::OnClickOK() {
    ((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->m_pSettingDlg->UpdateTips();
    ShowWindow(SW_HIDE);
}
