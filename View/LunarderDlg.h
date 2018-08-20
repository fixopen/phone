#if !defined(AFX_LUNARDERDLG_H__4F197ACD_544D_4716_884B_E30681C957BA__INCLUDED_)
#define AFX_LUNARDERDLG_H__4F197ACD_544D_4716_884B_E30681C957BA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LunarderDlg.h : header file
//


#include "../control/CeBtnST.h"
#include "../control/CEStatic.h"
#include "../control/CELunarder.h"
#include "../resource.h"

/////////////////////////////////////////////////////////////////////////////
// CLunarderDlg dialog

class CLunarderDlg : public CDialog
{
public:
	CCEStatic	m_stcTitle;
	CCEButtonST	m_btnExit;
	CCELunarder m_lunarder;
	
// Construction
public:
	CLunarderDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CLunarderDlg)
	enum { IDD = IDD_DIALOG_LUNARDER };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLunarderDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnExit();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLunarderDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LUNARDERDLG_H__4F197ACD_544D_4716_884B_E30681C957BA__INCLUDED_)
