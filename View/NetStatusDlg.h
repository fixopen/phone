#if !defined(AFX_ALARMSHOWDLG_H__12A8AE91_FA94_4D46_862D_22225F439F9969__INCLUDED_)
#define AFX_ALARMSHOWDLG_H__12A8AE91_FA94_4D46_862D_22225F439F9969__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AlarmShowDlg.h : header file
//
#include "../Control/CEDialog.h"
#include "../control/MJPGStatic.h"

enum DIALTYPE{CMWAP, CMNET};
/////////////////////////////////////////////////////////////////////////////
// CNetStatusDlg dialog
class CNetStatusDlg : public CCEDialog
{
public:
	static bool ADSLInit();
	//type = 0 cmwap 1 cmnet
	static bool ADSLDial(char *dialnumber, char *username, char *password, CWnd *pMsgWnd, DIALTYPE type);
	static bool ADSLHungUp();
	static int CheckADSLStatus(int &nStatusCode);

// Construction
public:
	BOOL m_bADSLISConnnect;
	int  m_nADSLTimeCount;
	CMJPGStatic		m_MJPGList;
	int m_type;    // 0 æ÷”ÚÕ¯ 1 ADSL
	CNetStatusDlg(CWnd* pParent = NULL);   // standard constructor
	void SetType(int type, BOOL IsConenctStatus);
	void ShowWindow_(int cmdshow);

// Dialog Data
	//{{AFX_DATA(CNetStatusDlg)
	enum { IDD = IDD_DIALOG_NETSTATUS };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNetStatusDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CNetStatusDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnClickMJPG(WPARAM w, LPARAM l);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnRasErrorCode(WPARAM w, LPARAM l);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ALARMSHOWDLG_H__12A8AE91_FA94_4D46_862D_22225F439F99__INCLUDED_)
