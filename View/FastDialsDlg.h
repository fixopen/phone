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

#include "../Data/Setting.h"

#include "../control/MJPGStatic.h"

/////////////////////////////////////////////////////////////////////////////
// CFastDialsDlg dialog

class CFastDialsDlg : public CCEDialog
{
// Construction
public:
	boost::shared_ptr<Data::Setting>  m_Data;
	void SetFastDialParam(	boost::shared_ptr<Data::Setting> data);
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
	afx_msg LRESULT OnClickMJPG(WPARAM w, LPARAM l);
	DECLARE_MESSAGE_MAP()
		
private:
	CMJPGStatic		m_MJPGList;
/*
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
	CCEFramePartStatic m_sticBackground;
*/
	CCERectEdit m_edtNumber[12];
	
//	CCEBmpButton m_btnOk;
//	CCEBmpButton m_btnCancel;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FASTDIALSDLG_H__6F1EB099_1F0C_487C_AAED_221F5FB6088B__INCLUDED_)
