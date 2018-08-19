#if !defined(AFX_INFORMATIONDLG_H__BC1151CF_86F9_4B01_B80A_D28DB7948C38__INCLUDED_)
#define AFX_INFORMATIONDLG_H__BC1151CF_86F9_4B01_B80A_D28DB7948C38__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InformationDlg.h : header file
//
#include "../Control/CEStatic.h"
#include "../Control/CeBtnST.h"
#include "../Control/CEDialog.h"

/////////////////////////////////////////////////////////////////////////////
// CInformationDlg dialog

class CInformationDlg : public CDialog
{
// Construction
public:
	CInformationDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CInformationDlg)
	enum { IDD = IDD_DIALOG_INFORMATION };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInformationDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CInformationDlg)
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nId);
	//}}AFX_MSG
	afx_msg void OnButtonNoteClose();
	DECLARE_MESSAGE_MAP()
private:

	CCEBmpButton m_btnClose;

	CCEStatic m_sticOperation;
	CCEStatic m_sticPanel;
	CCEStatic m_sticBack;
 	CCEFrameStatic m_sticBackground;

	//stocks
	PROCESS_INFORMATION   m_stocksprocessinfo; 
	BOOL				  m_bStockIsRun;

	void stopstocks();
	void runstocks();

public:
	void ShowWindow_(int nCmdShow);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INFORMATIONDLG_H__BC1151CF_86F9_4B01_B80A_D28DB7948C38__INCLUDED_)
