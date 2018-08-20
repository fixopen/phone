#if !defined(AFX_DELETETIPDLG_H__F5EF9874_51FB_4A20_AE47_D27787F7717B__INCLUDED_)
#define AFX_DELETETIPDLG_H__F5EF9874_51FB_4A20_AE47_D27787F7717B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// deletetipdlg.h : header file
//
#include "../Control/CEStatic.h"
#include "../Control/CeBtnST.h"
#include "../Control/CELineEdit.h"
#include "../Control/CEDialog.h"
#include <string>
//#define WM_CHANGELANGUAGE (WM_USER+300)
/////////////////////////////////////////////////////////////////////////////
// CDeleteTipDlg dialog

class CDeleteTipDlg : public CCEDialog
{
// Construction
public:
	CDeleteTipDlg(CWnd* pParent = NULL);   // standard constructor
    void ChangeLanguage();
// Dialog Data
	//{{AFX_DATA(CDeleteTipDlg)
	enum { IDD = IDD_DIALOG_DELETETIP };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDeleteTipDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDeleteTipDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg LRESULT OnChangeLanguage(WPARAM wParam,LPARAM lParam);
	//}}AFX_MSG
	afx_msg void OnButtonDeleteTipOk();
	afx_msg void OnButtonDeleteTipCancel();
	DECLARE_MESSAGE_MAP()
private:
	CCEStatic m_sticTitle;
	CCEStatic m_sticTip;
	CCEStatic m_sticPassword;
	CCEStatic m_sticError;
	CCERectEdit m_edtPassword;
	CCEBmpButton m_btnOk;
	CCEBmpButton m_btnCancel;
	CCEFramePartStatic m_sticBackground;
	HWND m_handle;
	bool m_bIsPasswordModel;
	bool m_bIsNewContact;
	CString m_sPassword;
public:
	void SetHWnd(HWND handle);
	void SetPasswordModel(bool b);
	void SetPassword(CString password);
	void SetDelTip(CString tips);
	void SetTitle(std::string title);
	void setM_bIsNewContact(bool flag);
	CCEBmpButton * getM_btnOK();
	CCEBmpButton * getM_btnCancel();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DELETETIPDLG_H__F5EF9874_51FB_4A20_AE47_D27787F7717B__INCLUDED_)
