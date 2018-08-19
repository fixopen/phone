#if !defined(AFX_TIPDLG_H__E2B28702_894B_4206_9FB8_DBDDA906E30F__INCLUDED_)
#define AFX_TIPDLG_H__E2B28702_894B_4206_9FB8_DBDDA906E30F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TipDlg.h : header file
//

#include "../Control/MJPGStatic.h"
#include "../Control/CEDialog.h"
/////////////////////////////////////////////////////////////////////////////
// CTipDlg dialog

class CTipDlg : public CCEDialog
{
// Construction
public:
	CTipDlg(CWnd* pParent = NULL);   // standard constructor
	CMJPGStatic   m_MJPGList;
	
	void SetTitle(CString title,int isTime = 4000,TEXTSIZE font = font_20);

// Dialog Data
	//{{AFX_DATA(CTipDlg)
	enum { IDD = IDD_DIALOG_DELETETIP };
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTipDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTipDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnClickMJPG(WPARAM w, LPARAM l);
	afx_msg void OnClickOK();
	afx_msg void OnTimer(UINT nIDEvent) ;
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TIPDLG_H__E2B28702_894B_4206_9FB8_DBDDA906E30F__INCLUDED_)
