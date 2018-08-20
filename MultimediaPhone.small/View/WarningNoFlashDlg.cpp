// AlarmShowDlg.cpp : implementation file
//

#include "stdafx.h"
#include "WarningNoFlashDlg.h"
#include "../MultimediaPhone.h"
#include "../MultimediaPhoneDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace View {
    /////////////////////////////////////////////////////////////////////////////
    // CWarningNoFlashDlg dialog


    CWarningNoFlashDlg::CWarningNoFlashDlg(CWnd* pParent /*=NULL*/)
        : CCEDialog(CWarningNoFlashDlg::IDD, pParent)
    {
        //{{AFX_DATA_INIT(CWarningNoFlashDlg)
        // NOTE: the ClassWizard will add member initialization here
        //}}AFX_DATA_INIT
    }


    void CWarningNoFlashDlg::DoDataExchange(CDataExchange* pDX)
    {
        CCEDialog::DoDataExchange(pDX);
        //{{AFX_DATA_MAP(CWarningNoFlashDlg)
        // NOTE: the ClassWizard will add DDX and DDV calls here
        //}}AFX_DATA_MAP
    }


    BEGIN_MESSAGE_MAP(CWarningNoFlashDlg, CCEDialog)
        //{{AFX_MSG_MAP(CWarningNoFlashDlg)
        ON_MESSAGE(WM_CLICKMJPG_TOAPP, OnClickMJPG)
        //}}AFX_MSG_MAP
    END_MESSAGE_MAP()

    /////////////////////////////////////////////////////////////////////////////
    // CWarningNoFlashDlg message handlers

    BOOL CWarningNoFlashDlg::OnInitDialog() 
    {
        CCEDialog::OnInitDialog();

        // TODO: Add extra initialization here
        m_MJPGList.Create(L"", WS_VISIBLE|WS_CHILD, CRect(54, 62, 746, 358), this);
        m_MJPGList.SetCurrentLinkFile(_T(".\\adv\\mjpg\\k1\\中文\\存贮空间不足.xml"));
        m_MJPGList.SetMJPGRect(CRect(54, 62, 746, 358));

        return TRUE;  // return TRUE unless you set the focus to a control
        // EXCEPTION: OCX Property Pages should return FALSE
    }

    void CWarningNoFlashDlg::SetType(int type)
    {
        if(type == 0)
        {
            m_MJPGList.SetUnitBitmap(1, _T(".\\adv\\mjpg\\k1\\common\\请输入内容.bmp"), _T(".\\adv\\mjpg\\k1\\common\\请输入内容.bmp"), FALSE);
        }
        else
        {
            m_MJPGList.SetUnitBitmap(1, _T(".\\adv\\mjpg\\k1\\common\\存贮空间不足.bmp"), _T(".\\adv\\mjpg\\k1\\common\\存贮空间不足.bmp"), FALSE);
        }

    }
    LRESULT CWarningNoFlashDlg::OnClickMJPG(WPARAM w, LPARAM l)
    {
        LRESULT result = 0;
        ShowWindow_(SW_HIDE);
        SetType(0xFF);
        return result;
    }
}
