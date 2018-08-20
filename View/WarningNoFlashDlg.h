#if !defined(CWARNNINGN0FLASHDLG)
#define CWARNNINGN0FLASHDLG

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AlarmShowDlg.h : header file
//
#include "../Control/CEDialog.h"
#include "../control/MJPGStatic.h"

/////////////////////////////////////////////////////////////////////////////
// CWarningNoFlashDlg dialog

class CWarningNoFlashDlg : public CCEDialog
{
// Construction
public:
	CMJPGStatic		m_MJPGList;
	HWND			m_handle;
	CWarningNoFlashDlg(CWnd* pParent = NULL);   // standard constructor
	void SetType(int type);
	void SetTitle(CString title);
	void SetHWnd(HWND handle);
	int m_nType;
	CCEProcessBar m_procbarSound;

	static void RemoveThread(void *type);
	afx_msg void OnClickMJPG(WPARAM w, LPARAM l);
	afx_msg void OnClickOK();
	afx_msg void OnClickCancel();

// Dialog Data
	//{{AFX_DATA(CWarningNoFlashDlg)
	enum { IDD = IDD_DIALOG_NOFLASH };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWarningNoFlashDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CWarningNoFlashDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ALARMSHOWDLG_H__12A8AE91_FA94_4D46_862D_22225F439F99__INCLUDED_)
