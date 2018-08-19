#if !defined(CSELECTNUMBERDLG_)
#define CSELECTNUMBERDLG_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// deletetipdlg.h : header file
//
#include "../Control/CEDialog.h"
#include <string>

#include "../control/MJPGStatic.h"

/////////////////////////////////////////////////////////////////////////////
// CDeleteTipDlg dialog

class CSelectNumberDlg : public CCEDialog
{
    // Construction
public:
    CSelectNumberDlg(CWnd* pParent = NULL);   // standard constructor
    int       m_ntype;

    // Dialog Data
    //{{AFX_DATA(CDeleteTipDlg)
    enum { IDD = IDD_DIALOG_DELETETIP };
    // NOTE: the ClassWizard will add data members here
    //}}AFX_DATA


    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDeleteTipDlg)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

    // Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CDeleteTipDlg)
    virtual BOOL OnInitDialog();
    afx_msg void OnTimer(UINT nIDEvent);
    //}}AFX_MSG
    afx_msg void OnButtonOk();
    afx_msg void OnButtonCancel();
    afx_msg LRESULT OnClickMJPG(WPARAM w, LPARAM l);
    DECLARE_MESSAGE_MAP()
private:

    CMJPGStatic	 m_MJPGList;
    HWND		 m_handle;
    int			 m_iMaxPos ; 
    int			 m_nNumber ;

public:

    void SetNumber(std::vector<CString> vNumber);
    void SetHWnd(HWND handle);
    void ClickBtn(int item);

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DELETETIPDLG_H__F5EF9874_51FB_4A20_AE47_D27787F7717B__INCLUDED_)
