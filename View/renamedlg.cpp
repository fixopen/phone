// RenameDlg.cpp : implementation file
//

#include "stdafx.h"
#include "../MultimediaPhone.h"
#include "RenameDlg.h"
#include "../resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRenameDlg dialog


CRenameDlg::CRenameDlg(CWnd* pParent /*=NULL*/)
: CCEDialog(CRenameDlg::IDD, pParent) {
    //{{AFX_DATA_INIT(CRenameDlg)
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT

}


void CRenameDlg::DoDataExchange(CDataExchange* pDX) {
    CCEDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CRenameDlg)
    // NOTE: the ClassWizard will add DDX and DDV calls here
    //}}AFX_DATA_MAP

}

void CRenameDlg::SetWndParam(CWnd* pWnd, int index, CString sName) {
    m_pOwner = pWnd;
    m_oldName = sName;
    m_index = index;

    m_edtName.SetWindowText(sName);
    m_edtNewName.SetWindowText(L"");
}

LRESULT CRenameDlg::OnClickMJPG(WPARAM w, LPARAM l) {
    LRESULT result = 0;
    switch (w) {
    case 1001:
        {
            CString s;
            m_edtNewName.GetWindowText(s);
            if (s != L"" && s != m_oldName && m_pOwner) {
                m_pOwner->SendMessage(WM_RENAME, m_index, 0);
            }
            ShowWindow_(SW_HIDE);
        }
        break;
    case 1000:
        ShowWindow_(SW_HIDE);
        break;
    }
    return result;
}

BEGIN_MESSAGE_MAP(CRenameDlg, CCEDialog)
    //{{AFX_MSG_MAP(CRenameDlg)
    ON_MESSAGE(WM_CLICKMJPG_TOAPP, OnClickMJPG)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRenameDlg message handlers

BOOL CRenameDlg::OnInitDialog() {
    CCEDialog::OnInitDialog();

    // TODO: Add extra initialization here
    m_edtName.Create(WS_CHILD | WS_VISIBLE, CRect(136 + 100, 125 + 60, 136 + 100 + 270, 125 + 60 + 35), this, IDC_EDIT_CONTACTGROUP_NAME);
    m_edtName.SetLimitText(15);

    m_edtNewName.Create(WS_CHILD | WS_VISIBLE, CRect(136 + 100, 175 + 60, 136 + 100 + 270, 175 + 60 + 35), this, IDC_EDIT_NAME);
    m_edtNewName.SetLimitText(15);

    m_MJPGList.Create(L"", WS_VISIBLE | WS_CHILD, CRect(100, 60, 540, 330), this);
    m_MJPGList.SetCurrentLinkFile(".\\adv\\mjpg\\k5\\中文\\修改文件名.xml");
    m_MJPGList.SetMJPGRect(CRect(100, 60, 540, 330));

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}
