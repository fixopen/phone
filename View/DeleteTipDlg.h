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

#include "../control/MJPGStatic.h"

/////////////////////////////////////////////////////////////////////////////
// CDeleteTipDlg dialog

class CDeleteTipDlg : public CCEDialog
{
// Construction
public:
	CDeleteTipDlg(CWnd* pParent = NULL);   // standard constructor
	int       type;

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
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDeleteTipDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	afx_msg void OnButtonDeleteTipOk();
	afx_msg void OnButtonDeleteTipCancel();
	afx_msg LRESULT OnClickMJPG(WPARAM w, LPARAM l);
	DECLARE_MESSAGE_MAP()
private:
	CMJPGStatic		m_MJPGList;
	
//	CCEStatic m_sticTitle;
//	CCEStatic m_sticTip;
//	CCEStatic m_sticPassword;
	CCERectEdit m_edtNewPassword1;
//	CCEBmpButton m_btnOk;
//	CCEBmpButton m_btnCancel;
//	CCEFramePartStatic m_sticBackground;
	HWND m_handle;
	bool m_bIsPasswordModel;
	CString m_sPassword;
	BOOL m_bIsTipModel;
public:
	void SetHWnd(HWND handle);
	void SetPasswordModel(bool b);
	void SetTipModel(BOOL b);
	void SetPassword(CString password);
	void SetDelTip(CString tips);
	void SetTitle(CString title, int isTime = 5000);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DELETETIPDLG_H__F5EF9874_51FB_4A20_AE47_D27787F7717B__INCLUDED_)
