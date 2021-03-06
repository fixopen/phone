#if !defined(AFX_CONTACTSEARCHDLG_H__653CD87C_51BE_47C0_839C_2E3D82A8EC84__INCLUDED_)
#define AFX_CONTACTSEARCHDLG_H__653CD87C_51BE_47C0_839C_2E3D82A8EC84__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// contactsearchdlg.h : header file
//
#include "../Control/CEStatic.h"
#include "../Control/CeBtnST.h"
#include "../Control/CELineEdit.h"
#include "../Control/CEDialog.h"

/////////////////////////////////////////////////////////////////////////////
// CContactSearchDlg dialog

//#define WM_CHANGELANGUAGE (WM_USER+300)
class CContactSearchDlg : public CCEDialog
{
// Construction
public:
	CContactSearchDlg(CWnd* pParent = NULL);   // standard constructor
    void ChangeLanguage();
// Dialog Data
	//{{AFX_DATA(CContactSearchDlg)
	enum { IDD = IDD_DIALOG_CONTACTSEARCH };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CContactSearchDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CContactSearchDlg)
	virtual BOOL OnInitDialog();
	afx_msg LRESULT OnChangeLanguage(WPARAM wParam,LPARAM lParam);
	//}}AFX_MSG
	afx_msg void OnButtonSearchContactCancel();
	afx_msg void OnButtonSearchContactSearch();
	DECLARE_MESSAGE_MAP()
		
private:
	CCEStatic m_sticTitle;
	CCEStatic m_sticTip;
	CCEStatic m_sticName;
	CCEStatic m_sticNumber;
	CCEStatic m_sticCompany;
	CCEStatic m_sticDuty;
	CCERectEdit m_edtName;
	CCERectEdit m_edtNumber;
	CCERectEdit m_edtCompany;
	CCERectEdit m_edtDuty;

	CCEBmpButton m_btnSearch;
	CCEBmpButton m_btnCancel;
	CCEFramePartStatic m_sticBackground;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONTACTSEARCHDLG_H__653CD87C_51BE_47C0_839C_2E3D82A8EC84__INCLUDED_)
