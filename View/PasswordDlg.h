#if !defined(AFX_PASSWORDDLG_H__D8935697_5CC9_4844_A12D_E203B5F8A9F5__INCLUDED_)
#define AFX_PASSWORDDLG_H__D8935697_5CC9_4844_A12D_E203B5F8A9F5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// passworddlg.h : header file
//
#include "../Control/CEStatic.h"
#include "../Control/CeBtnST.h"
#include "../Control/CELineEdit.h"
#include "../Control/CEDialog.h"


/////////////////////////////////////////////////////////////////////////////
// CPasswordDlg dialog

class CPasswordDlg : public CCEDialog
{
// Construction
public:
	CPasswordDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPasswordDlg)
	enum { IDD = IDD_DIALOG_PASSWORD };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPasswordDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPasswordDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	afx_msg void OnButtonPasswordOk();
	afx_msg void OnButtonPasswordCancel();
	DECLARE_MESSAGE_MAP()
private:
	CCEStatic m_sticBackground;
	CCEStatic m_sticTitle;
	CCEStatic m_sticOldPassword;
	CCEStatic m_sticNewPassword1;
	CCEStatic m_sticNewPassword2;

	CCELineEdit m_edtOldPassword;
	CCELineEdit m_edtNewPassword1;
	CCELineEdit m_edtNewPassword2;
	
	CCEButtonST m_btnOk;
	CCEButtonST m_btnCancel;

	void SetStaticDefaultColor(CCEStatic* cestatic);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PASSWORDDLG_H__D8935697_5CC9_4844_A12D_E203B5F8A9F5__INCLUDED_)
