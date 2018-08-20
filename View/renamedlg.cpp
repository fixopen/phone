// RenameDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RenameDlg.h"
#include "../MultimediaPhone.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRenameDlg dialog

namespace View {
    CRenameDlg::CRenameDlg(CWnd* pParent /*=NULL*/)
        : CCEDialog(CRenameDlg::IDD, pParent)
    {
        //{{AFX_DATA_INIT(CRenameDlg)
        // NOTE: the ClassWizard will add member initialization here
        //}}AFX_DATA_INIT
    }


    void CRenameDlg::DoDataExchange(CDataExchange* pDX)
    {
        CCEDialog::DoDataExchange(pDX);
        //{{AFX_DATA_MAP(CRenameDlg)
        // NOTE: the ClassWizard will add DDX and DDV calls here
        //}}AFX_DATA_MAP
    }

    void  CRenameDlg::SetWndParam(CWnd *pWnd, int index, CString sName)
    {
        m_pOwner = pWnd;
        m_oldName = sName;
        m_index = index;

        m_edtName.SetWindowText(sName);
    }

    LRESULT CRenameDlg::OnClickMJPG(WPARAM w, LPARAM l)
    {
        LRESULT result = 0;
        switch(w)
        {
        case 1:
            {
                CString s;
                m_edtName.GetWindowText(s);
                if(s != "" && s != m_oldName && m_pOwner)
                    m_pOwner->SendMessage(WM_RENAME, m_index, 0);
                ShowWindow_(SW_HIDE);
            }
            break;
        case 2:
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

    BOOL CRenameDlg::OnInitDialog() 
    {
        CCEDialog::OnInitDialog();

        // TODO: Add extra initialization here
		m_MJPGList.Create(L"", WS_VISIBLE|WS_CHILD, CRect(54*X_XISHU, 62*Y_XISHU, 746*X_XISHU, 358*Y_XISHU), this);
        m_MJPGList.SetCurrentLinkFile(_T(".\\adv\\mjpg\\k1\\����\\�޸��ļ���.xml"));
        m_MJPGList.SetMJPGRect(CRect(54*X_XISHU, 62*Y_XISHU, 746*X_XISHU, 358*Y_XISHU));

		m_edtName.Create(WS_CHILD|WS_VISIBLE, CRect((162-54)X_XISHU, (91-62)*Y_XISHU, (162+253-54)X_XISHU, (91+39-62)*Y_XISHU), &m_MJPGList, IDC_EDIT_CONTACTGROUP_NAME);
        m_edtName.SetLimitText(15);

        return TRUE;  // return TRUE unless you set the focus to a control
        // EXCEPTION: OCX Property Pages should return FALSE
    }
}
