#if !defined(AFX_YHTELDLG_H__C94EBB4B_B3AB_4063_9DEB_23E73E760B44__INCLUDED_)
#define AFX_YHTELDLG_H__C94EBB4B_B3AB_4063_9DEB_23E73E760B44__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// YHTelDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// C3GTelDlg dialog
#include "../control/MJPGStatic.h"
#include "../Logical/LunarderDate.h"
#include "../control/CEStatic.h"
#include "../Data/LanguageResource.h"

class C3GTelDlg : public CDialog {
    // Construction
public:
    CMJPGStatic m_MJPGList;
    C3GTelDlg(CWnd* pParent = NULL);   // standard constructor

    // Dialog Data
    //{{AFX_DATA(C3GTelDlg)
    enum { IDD = IDD_DIALOG_CACULATER };
    // NOTE: the ClassWizard will add data members here
    //}}AFX_DATA


    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(C3GTelDlg)
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

    // Implementation
protected:
    // Generated message map functions
    //{{AFX_MSG(C3GTelDlg)
    virtual BOOL OnInitDialog();
    //  afx_msg void OnTimer(UINT nIDEvent);
    afx_msg LRESULT OnClickMJPG(WPARAM w, LPARAM l);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_YHTELDLG_H__C94EBB4B_B3AB_4063_9DEB_23E73E760B44__INCLUDED_)
