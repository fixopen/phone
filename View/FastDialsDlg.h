#if !defined(AFX_FASTDIALSDLG_H__6F1EB099_1F0C_487C_AAED_221F5FB6088B__INCLUDED_)
#define AFX_FASTDIALSDLG_H__6F1EB099_1F0C_487C_AAED_221F5FB6088B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// fastdialsdlg.h : header file
//
#include "../Control/CELineEdit.h"
#include "../Control/CEStatic.h"
#include "../Control/CeBtnST.h"
#include "../Control/CEDialog.h"

/////////////////////////////////////////////////////////////////////////////
// CFastDialsDlg dialog

class CFastDialsDlg : public CCEDialog
{
// Construction
public:
	CFastDialsDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CFastDialsDlg)
	enum { IDD = IDD_DIALOG_FASTDIALS };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFastDialsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFastDialsDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	afx_msg void OnButtonFastDialsOk();
	afx_msg void OnButtonFastDialsCancel();
	DECLARE_MESSAGE_MAP()
		
private:
	CCEStatic m_sticTitle;
	CCEStatic m_sticNumber01;
	CCEStatic m_sticNumber02;
	CCEStatic m_sticNumber03;
	CCEStatic m_sticNumber04;
	CCEStatic m_sticNumber05;
	CCEStatic m_sticNumber06;
	CCEStatic m_sticNumber07;
	CCEStatic m_sticNumber08;
	CCEStatic m_sticNumber09;
	CCEStatic m_sticNumber10;
	CCEStatic m_sticNumber11;
	CCEStatic m_sticNumber12;
	CCEStatic m_sticBackground;

	CCELineEdit m_edtNumber01;
	CCELineEdit m_edtNumber02;
	CCELineEdit m_edtNumber03;
	CCELineEdit m_edtNumber04;
	CCELineEdit m_edtNumber05;
	CCELineEdit m_edtNumber06;
	CCELineEdit m_edtNumber07;
	CCELineEdit m_edtNumber08;
	CCELineEdit m_edtNumber09;
	CCELineEdit m_edtNumber10;
	CCELineEdit m_edtNumber11;
	CCELineEdit m_edtNumber12;

	CCEButtonST m_btnOk;
	CCEButtonST m_btnCancel;

	void SetStaticDefaultColor(CCEStatic* cestatic);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FASTDIALSDLG_H__6F1EB099_1F0C_487C_AAED_221F5FB6088B__INCLUDED_)
