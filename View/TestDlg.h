#if !defined(AFX_TESTDLG_H__18AC1561_3425_48BF_A440_1F40FFE30CE6__INCLUDED_)
#define AFX_TESTDLG_H__18AC1561_3425_48BF_A440_1F40FFE30CE6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TestDlg.h : header file
//
#include "../resource.h"

/////////////////////////////////////////////////////////////////////////////
// CTestDlg dialog

class CTestDlg : public CDialog
{
// Construction
public:
	CTestDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTestDlg)
	enum { IDD = IDD_DIALOG_TEST };
		// NOTE: the ClassWizard will add data members here
	CListBox m_listBox;
	CListBox m_listBoxMem;
	//}}AFX_DATA
	BOOL m_bFlagCall;
	BOOL m_bFlagTestDb;
	BOOL m_bExit;  //ÊÇ·ñÍË³ö

	static DWORD WINAPI ThreadCallProc(LPVOID lParam);
	static DWORD WINAPI ThreadTestDbProc(LPVOID lParam);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTestDlg)
	afx_msg void OnBtnTestCall();
	afx_msg void OnBtnTestDb();
	afx_msg void OnBtnTestExit();
	afx_msg LRESULT OnTelData(WPARAM w, LPARAM l);
	afx_msg LRESULT OnExit(WPARAM w, LPARAM l);
	//}}AFX_MSG
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTDLG_H__18AC1561_3425_48BF_A440_1F40FFE30CE6__INCLUDED_)
