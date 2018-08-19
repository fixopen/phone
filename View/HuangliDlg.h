#if !defined(AFX_HUANGLIDLG_H__8ABC99C3_B72D_4387_B187_8801DDC7B540__INCLUDED_)
#define AFX_HUANGLIDLG_H__8ABC99C3_B72D_4387_B187_8801DDC7B540__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HuangliDlg.h : header file
//

#include "../Control/MJPGStatic.h"
#include "../resource.h"

/////////////////////////////////////////////////////////////////////////////
// CHuangliDlg dialog

class CHuangliDlg : public CDialog
{
public:
	int				m_curIndex;
	CMJPGStatic		m_MJPGList;
	int				m_Year, m_Month, m_Day;
	void SetData(int y, int m, int d, BOOL flag = TRUE);
	void ShowLunderRight(BOOL isDraw);

	void OnYleft();
	void OnYright();
	void OnMleft();
	void OnMright();
	void OnExit();

// Construction
public:
	CHuangliDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CHuangliDlg)
	enum { IDD = IDD_DIALOG_HUANGLI };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHuangliDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CHuangliDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnClickMJPG(WPARAM w, LPARAM l);
	afx_msg void OnMJPGMOVE(WPARAM w, LPARAM l);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HUANGLIDLG_H__8ABC99C3_B72D_4387_B187_8801DDC7B540__INCLUDED_)
