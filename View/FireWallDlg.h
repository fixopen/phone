#if !defined(AFX_FIREWALLDLG_H__37E23B5A_3692_4738_ABD1_37AA4B0386B5__INCLUDED_)
#define AFX_FIREWALLDLG_H__37E23B5A_3692_4738_ABD1_37AA4B0386B5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// firewalldlg.h : header file
//
#include "../Control/CEStatic.h"
#include "../Control/CeBtnST.h"
#include "../Control/CEDialog.h"
#include "../Control/CECombobox_.h"

/////////////////////////////////////////////////////////////////////////////
// CFireWallDlg dialog

class CFireWallDlg : public CCEDialog
{
// Construction
public:
	CFireWallDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CFireWallDlg)
	enum { IDD = IDD_DIALOG_FIREWALL };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFireWallDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFireWallDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	afx_msg void OnButtonFireWallOk();
	afx_msg void OnButtonFireWallCancel();
	DECLARE_MESSAGE_MAP()
private:
	CCEStatic m_sticTitle;
	CCEStatic m_sticTimePart1;
	CCEStatic m_sticTimePart2;
	CCEButtonST m_btnOk;
	CCEButtonST m_btnCancel;
	CCEButtonST m_chbEnableFireWall;
	CCEButtonST m_chbRefuseBlacklist;
	CCEButtonST m_chbContactExceptBlacklist;
	CCEButtonST m_chbOnlyVIP;
	CCEButtonST m_chbRefuseAllByTime;
	CCEComboBox_ m_cmbTime;
	CCEStatic m_sticBackground;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FIREWALLDLG_H__37E23B5A_3692_4738_ABD1_37AA4B0386B5__INCLUDED_)
