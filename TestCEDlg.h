#if !defined(AFX_TESTCEDLG_H__AB228209_5E94_4375_A852_AB15238FAEF9__INCLUDED_)
#define AFX_TESTCEDLG_H__AB228209_5E94_4375_A852_AB15238FAEF9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TestCEDlg.h : header file
//

#include "./control/CEDialog.h"

/////////////////////////////////////////////////////////////////////////////
// CTestCEDlg dialog

class CTestCEDlg : public CCEDialog
{
// Construction
public:
	CTestCEDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTestCEDlg)
	enum { IDD = IDD_DIALOG_TESTCEDLG};
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestCEDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTestCEDlg)
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTCEDLG_H__AB228209_5E94_4375_A852_AB15238FAEF9__INCLUDED_)
