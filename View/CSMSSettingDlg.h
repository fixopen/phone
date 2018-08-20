#if !defined(AFX_YHTELDLG_H__C94EBB4B_B3AB_4063_9DEB_23E73E760B44_40__INCLUDED_)
#define AFX_YHTELDLG_H__C94EBB4B_B3AB_4063_9DEB_23E73E760B44__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// YHTelDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// C3GSMSDlg dialog
#include "../control/MJPGStatic.h"
#include "../control/CEStatic.h"
#include "../Control/CELineEdit.h"
#include "../Data/MessageSet.h"
#include "CSMSSettingDlg.h"

class CSMSSettingDlg : public CDialog
{
// Construction
public:

	CMJPGStatic		m_MJPGList;
	CCERectEdit		m_edtSmsc;

	CSMSSettingDlg(CWnd* pParent = NULL);   // standard constructor

	enum VPState{
		normal,
		setvp,
		setcsca,
		restorevp
	};

	VPState m_vpState;

// Dialog Data
	//{{AFX_DATA(C3GSMSDlg)
	enum { IDD = IDD_DIALOG_MMS };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(C3GSMSDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	//}}AFX_VIRTUAL

private:
	SMSMMS_TYPE	 m_smsType ;

public:
	void SetUpBtn(int ID );
	void OnButtonOK();
	void OnButtonCancel();
	void SettingOK();
	void InitData(boost::shared_ptr<Data::MessageSet> const& msgSet);
	void ShowWindow_(int nCmdShow);
protected:
	// Generated message map functions
	//{{AFX_MSG(C3GSMSDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnClickMJPG(WPARAM w, LPARAM l);
	afx_msg void OnbtnCsca();
	afx_msg void OnbtnVp();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	//«ø÷∆ ’Õ¯
	CCERadioButton m_Radio1;
	CCERadioButton m_Radio2;
	CCERadioButton m_Radio3;
	CCERadioButton m_Radio4;

	CButton m_btnSetVP;
	CButton m_btnSetCSCA;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_YHTELDLG_H__C94EBB4B_B3AB_4063_9DEB_23E73E760B44__INCLUDED_)
