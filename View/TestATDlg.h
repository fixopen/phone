#if !defined(AFX_TESTDLG_H__18AC1561_3425_48BF_A440_1F40FFE30CE6_00__INCLUDED_)
#define AFX_TESTDLG_H__18AC1561_3425_48BF_A440_1F40FFE30CE6_00__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TestDlg.h : header file
//
#include "../resource.h"
#include "../Control/CELineEdit.h"
#include "../Control/CEComboBox_.h"
#include "../Control/CERadioButton.h"

/////////////////////////////////////////////////////////////////////////////
// CTestTDlg dialog

class CTestATDlg : public CDialog
{
// Construction
public:
	CButton m_btnSend;
	CButton m_btnStart;
	CButton m_btnEnd;
	CButton m_btnExit;
	CButton m_btnADSLON;
	CButton m_btnADSLOFF;

	CCERadioButton m_Radio1;
	CCERadioButton m_Radio2;
	CCERadioButton m_Radio3;
	CCERadioButton m_Radio4;
	CButton  m_SetDDMT;

	CButton  m_btnSearch;
	CCEComboBox12_ m_cmbSearchBox;
	CButton m_btnReReg;


	CCELineEdit m_edtAT;
	CCEComboBox12_ m_cmbBox;
	CEdit m_lstAT;

	CButton  m_btnCopyLog;

	CTestATDlg(CWnd* pParent = NULL);   // standard constructor
	void ParseSTK(const char *src);
	void ResPonseSTK(char *cmd_indifity, int result);

	void OnATSend(char *src, char *dsc);
	BOOL OnReadATFile();
	
// Dialog Data
	//{{AFX_DATA(CTestTDlg)
	enum { IDD = IDD_DIALOG_TESTAT };
		// NOTE: the ClassWizard will add data members here
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestTDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTestTDlg)
	afx_msg void OnBtnSend();
	afx_msg void OnBtnExit();
	afx_msg void OnATSelect(WPARAM w, LPARAM l);

	afx_msg void OnBtnSetDDTM(WPARAM w, LPARAM l);
	afx_msg void OnBtnReSearchNet(WPARAM w, LPARAM l);
	afx_msg void OnBtnReRist(WPARAM w, LPARAM l);
	afx_msg void OnBtnCopyLog(WPARAM w, LPARAM l);
	afx_msg void OnBtnNewStart(WPARAM w, LPARAM l);
	afx_msg void OnBtnNewEnd(WPARAM w, LPARAM l);
	afx_msg void OnBtnADSL(WPARAM w, LPARAM l);
	afx_msg void OnBtnADSLOFF(WPARAM w, LPARAM l);

	//}}AFX_MSG
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTDLG_H__18AC1561_3425_48BF_A440_1F40FFE30CE6__INCLUDED_)
