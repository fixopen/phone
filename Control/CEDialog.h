#if !defined(AFX_CEDIALOG_H__19873EBA_758E_448D_BDC5_A389CF1330B8__INCLUDED_)
#define AFX_CEDIALOG_H__19873EBA_758E_448D_BDC5_A389CF1330B8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CEDialog.h : header file
//

//#include "../resource.h"
/////////////////////////////////////////////////////////////////////////////
// CCEDialog dialog

class CCEDialog : public CDialog
{
// Construction
public:
	CCEDialog(UINT nIDTemplate, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCEDialog)
	enum { IDD = 0 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCEDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCEDialog)
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CEDIALOG_H__19873EBA_758E_448D_BDC5_A389CF1330B8__INCLUDED_)
