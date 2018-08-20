#if !defined(AFX_INFORMATIONDLG_H__BC1151CF_86F9_4B01_B80A_D28DB7948C38__INCLUDED_)
#define AFX_INFORMATIONDLG_H__BC1151CF_86F9_4B01_B80A_D28DB7948C38__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InformationDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CInformationDlg dialog

class CInformationDlg : public CDialog
{
// Construction
public:
	CInformationDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CInformationDlg)
	enum { IDD = IDD_DIALOG_INFORMATION };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInformationDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CInformationDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INFORMATIONDLG_H__BC1151CF_86F9_4B01_B80A_D28DB7948C38__INCLUDED_)
