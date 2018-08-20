#if !defined(AFX_FASTDIALSDLG_H__6F1EB099_1F0C_487C_AAED_221F5FB6088B01_10__INCLUDED_)
#define AFX_FASTDIALSDLG_H__6F1EB099_1F0C_487C_AAED_221F5FB6088B01_10__INCLUDED_

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
#include "../view/PasswordDlg.h"
#include "../view/PinInputDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CPinSetDlg dialog

class CPinSetDlg : public CCEDialog
{
// Construction
public:
	BOOL m_bPin;
	CCERectEdit m_edtPinNumber;
	
	CPinInputDlg *m_pPinInputDlg;
	void SetCallSetParam();
	void SetPinStatus();
	CPinSetDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPinSetDlg)
	enum { IDD = IDD_DIALOG_FASTDIALS };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPinSetDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam) ;
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPinSetDlg)
	afx_msg void OnClickMJPG(WPARAM w, LPARAM l);
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
		
public:
	CMJPGStatic		m_MJPGList;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FASTDIALSDLG_H__6F1EB099_1F0C_487C_AAED_221F5FB6088B__INCLUDED_)
