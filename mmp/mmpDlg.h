// mmpDlg.h : header file
//

#if !defined(AFX_MMPDLG_H__96773027_9007_4083_8833_F8EEBE6FD6B6__INCLUDED_)
#define AFX_MMPDLG_H__96773027_9007_4083_8833_F8EEBE6FD6B6__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// CMmpDlg dialog

class CMmpDlg : public CDialog
{
// Construction
public:
	CMmpDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CMmpDlg)
	enum { IDD = IDD_MMP_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMmpDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CMmpDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft eMbedded Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MMPDLG_H__96773027_9007_4083_8833_F8EEBE6FD6B6__INCLUDED_)
