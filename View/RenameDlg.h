#if !defined(CRENAMEDLG_)
#define CRENAMEDLG_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RenameDlg.h : header file
//

#include "../Control/CELineEdit.h"
#include "../control/MJPGStatic.h"
#include "../Control/CEDialog.h"

/////////////////////////////////////////////////////////////////////////////
// CRenameDlg dialog

class CRenameDlg : public CCEDialog
{
public:
	CCERectEdit     m_edtName;
	CCERectEdit     m_edtNewName;
	CMJPGStatic		m_MJPGList;
	CWnd			*m_pOwner;
	CString         m_oldName;
	int             m_index;
	void            SetWndParam(CWnd *pWnd, int nIndex, CString sName);

// Construction
public:
	CRenameDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CRenameDlg)
	enum { IDD = IDD_DIALOG_RENAME };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRenameDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRenameDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnClickMJPG(WPARAM w, LPARAM l);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RENAMEDLG_H__0A17544A_9F8E_4C81_ADA6_0A95FF826E02__INCLUDED_)
