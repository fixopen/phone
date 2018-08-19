#if !defined(AFX_FASTDIALSDLG_H__6F1EB099_1F0C_487C_AAED_221F5FB6088B0101_03_INCLUDED_)
#define AFX_FASTDIALSDLG_H__6F1EB099_1F0C_487C_AAED_221F5FB6088B0101_03_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// fastdialsdlg.h : header file
//
#include "../Control/CEListCtrl.h"
#include "../Control/CEStatic.h"
#include "../Control/CeBtnST.h"
#include "../Control/CELineEdit.h"
#include "../Control/CECombobox_.h"
#include "../Control/CERadioButton.h"
#include "../Control/CEDialog.h"

#include "../Data/Setting.h"

#include "../control/MJPGStatic.h"
#include "../Util/ATCommandWarp.h"

/////////////////////////////////////////////////////////////////////////////
// CSTKDlg dialog

struct STKMenu
{
	int cmdid;
	CString sName;
};

class CSTKDlg : public CCEDialog
{
// Construction
public:
	int   m_nCurrentCount;
	int   m_nCmdidFather[18];
	BOOL SetCmdID(int id, int nCount, BOOL isFindFatherCmd);
	void ResetCmdID();
	void ParseSTK(const char *src, BOOL isACK = TRUE);
	void OnATSend(char *src, char *dsc, char *ack, char *ack1 = NULL);
	void ShowMenu();
	void GenResPonse(char *src, BOOL isOK);

	void doOK();
	void doCancel();
	void doSelectMenu(int w);

	int m_nMenuCount;
	STKMenu m_STKMenu[18];

	CSTKDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSTKDlg)
	enum { IDD = IDD_DIALOG_FASTDIALS };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSTKDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSTKDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	afx_msg void OnButtonFastDialsOk();
	afx_msg void OnButtonFastDialsCancel();
	afx_msg void OnClickMJPG(WPARAM w, LPARAM l);
	afx_msg void OnTimer(UINT nIDEvent);
	DECLARE_MESSAGE_MAP()
		
private:
	CMJPGStatic		m_MJPGList;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FASTDIALSDLG_H__6F1EB099_1F0C_487C_AAED_221F5FB6088B__INCLUDED_)
