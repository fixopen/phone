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

#include "../control/MJPGStatic.h"


/////////////////////////////////////////////////////////////////////////////
// CPasswordDlg dialog
enum PASSWORD_TYPE{CHECK_PLAYPASSWORD, CHECK_SUPPERPASSWORD, CHECK_CLEARPASSWORD,CHECK_PINPASSWORD,CHECK_PUKPASSWORD, CHECK_LOCKPINPASSWORD, CHECK_UNLOCKPINPASSWORD, SETTINGPLAY_PASSWORD, SETTINGSUPPER_PASSWORD, SETTINGPIN_PASSWORD, PASSWORD_CHECKERROR, PASSWORD_OLDERROR, PASSWORD_NEWERROR};

class CPasswordDlg : public CCEDialog
{
// Construction
public:
	int m_nCtrl;
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
	afx_msg void OnClickMJPG(WPARAM w, LPARAM l);
	DECLARE_MESSAGE_MAP()
private:
	CMJPGStatic		m_MJPGList;
/*
	CCEStatic m_sticTitle;
	CCEStatic m_sticTip;
	CCEStatic m_sticOldPassword;
	CCEStatic m_sticNewPassword1;
	CCEStatic m_sticNewPassword2;

	CCERectEdit m_edtOldPassword;
	CCERectEdit m_edtNewPassword1;
	CCERectEdit m_edtNewPassword2;
	
	CCEBmpButton m_btnOk;
	CCEBmpButton m_btnCancel;
	CCEFramePartStatic m_sticBackground;
*/

	CCERectEdit m_edtOldPassword;
	CCERectEdit m_edtNewPassword1;
	CCERectEdit m_edtNewPassword2;
	
	PASSWORD_TYPE m_passwordType;
	CString		  m_password;
	int			  m_nStep;
	HWND		  m_Owner;

	void SetStaticDefaultColor(CCEStatic* cestatic);
public:
	void	SetType(PASSWORD_TYPE nType, int nCtrl = 0);
	void    SetOldPassWord(char *pPassWord);
	void    SetHWnd(HWND handle){m_Owner = handle;}
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PASSWORDDLG_H__D8935697_5CC9_4844_A12D_E203B5F8A9F5__INCLUDED_)
