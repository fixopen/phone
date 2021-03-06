#if !defined(AFX_LUNARDERDLG_H__4F197ACD_544D_4716_884B_E30681C957BA__INCLUDED_)
#define AFX_LUNARDERDLG_H__4F197ACD_544D_4716_884B_E30681C957BA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LunarderDlg.h : header file
//
#include "../control/CeBtnST.h"
#include "../control/CEStatic.h"
#include "../control/CELunarder.h"
#include "../control/CELineEdit.h"
#include "../Data/SchedulerData.h"

#include "../resource.h"

//#define WM_CHANGELANGUAGE (WM_USER+300)
/////////////////////////////////////////////////////////////////////////////
// CLunarderDlg dialog

class CLunarderDlg : public CDialog
{
public:
	BOOL m_bIsHaveNote;
	int m_type;
	CCEStatic	m_StcYear, m_StcMonth;
	CCEBmpButton m_BtnYleft, m_BtnYright, m_BtnMleft, m_BtnMright, m_BtnUleft, m_BtnDright;
	CCEBmpButton m_btnExit;
	CCEBmpButton m_btnOk;
	CCEBmpButton m_btnCancel;
	CCEBmpButton m_btnDelete;

	CCELunarder m_lunarder;

	CCEStatic	m_Line1Static;
	CCEStatic	m_Line2Static;
	CCEStatic	m_Line3Static;
	CCEStatic	m_Line4Static;
	CCEStatic	m_Line5Static;
	CCEStatic	m_Line6Static;
	CCELineEdit m_Edit;
	CCEStatic   m_titleStatic;

	CCEFrameStatic m_backStatic;

	boost::shared_ptr<Data::Scheduler> m_pScheduler;

	void SetData(int y, int m, int d);
	BOOL m_maindeskIn;
	void OnExit_(){OnExit();}
    void ChangeLanguage();
// Construction
public:
	CLunarderDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CLunarderDlg)
	enum { IDD = IDD_DIALOG_LUNARDER };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLunarderDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnExit();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLunarderDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnYleft();
	afx_msg void OnYright();
	afx_msg void OnMleft();
	afx_msg void OnMright();
	afx_msg void OnUp();
	afx_msg void OnDown();
	afx_msg void OnLunarClick(WPARAM w, LPARAM l);
	afx_msg void OnOk();
	afx_msg void OnCancel();
	afx_msg void OnDelete();
	afx_msg LRESULT OnChangeLanguage(WPARAM wParam,LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LUNARDERDLG_H__4F197ACD_544D_4716_884B_E30681C957BA__INCLUDED_)
