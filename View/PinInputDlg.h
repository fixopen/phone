#if !defined(CPININPUTDLG_)
#define CPININPUTDLG_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// fastdialsdlg.h : header file
//

#include "../Control/CEStatic.h"
#include "../Control/CEDialog.h"
#include "../control/MJPGStatic.h"
#include "../Control/CELineEdit.h"


/////////////////////////////////////////////////////////////////////////////
// CPinSetDlg dialog

class CPinInputDlg : public CCEDialog
{
// Construction
public:
	BOOL m_bPin;
	int  m_nType;

	int m_nErrorCount;
	void SetErrorCount(int nCount);

	CCERectEdit m_edt1Number;
	CCERectEdit m_edt2Number;
	CCERectEdit m_edt3Number;

	CPinInputDlg(CWnd* pParent = NULL);   // standard constructor
	
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
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPinSetDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	afx_msg void OnClickMJPG(WPARAM w, LPARAM l);
	DECLARE_MESSAGE_MAP()
		
public:
	CMJPGStatic		m_MJPGList;

public:
	void OnbtnOK();//确定
	void ShowPinLockView();//PIN码开解锁界面
	void ShowPinInputView();//PIN码输入界面
	void ShowPukInputView();//PUK输入的界面,
	void ShowPin2InputView();//PIN2码输入界面
	void ShowPuk2InputView();//PUK输入的界面
	void ChangePinView();//修改PIN码
	void ChangePin2View();//修改PIN2码
	void EditClear();

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FASTDIALSDLG_H__6F1EB099_1F0C_487C_AAED_221F5FB6088B__INCLUDED_)
