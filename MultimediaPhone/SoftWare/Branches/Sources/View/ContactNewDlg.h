#if !defined(AFX_CONTACTNEWDLG_H__96578402_C2C4_43A0_9ABA_90B878DE7D1D__INCLUDED_)
#define AFX_CONTACTNEWDLG_H__96578402_C2C4_43A0_9ABA_90B878DE7D1D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ContactNewDlg.h : header file
//
#include "../Control/CELineEdit.h"
#include "../Control/CEStatic.h"
#include "../Control/CeBtnST.h"
#include "../Control/CECombobox_.h"
#include "../Data/Contact.h"

/////////////////////////////////////////////////////////////////////////////
// CContactNewDlg dialog

class CContactNewDlg : public CDialog
{
// Construction
public:
	CContactNewDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CContactNewDlg)
	enum { IDD = IDD_DIALOG_CONTACTNEW };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CContactNewDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CContactNewDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	afx_msg void OnButtonContactNewOk();
	afx_msg void OnButtonContactNewCancel();
	afx_msg void OnButtonContactNewDelete();
	afx_msg void OnButtonContactNewDialMobile1();
	afx_msg void OnButtonContactNewDialMobile2();
	afx_msg void OnButtonContactNewDialTelephone1();
	afx_msg void OnButtonContactNewDialTelephone2();
	DECLARE_MESSAGE_MAP()
private:
	CCEStatic m_sticTitle;
	CCEStatic m_sticName;
	CCEStatic m_sticMobile1;
	CCEStatic m_sticMobile2;
	CCEStatic m_sticTelephone1;
	CCEStatic m_sticTelephone2;
	CCEStatic m_sticEmail;
	CCEStatic m_sticSite;
	CCEStatic m_sticGroup;
	CCEStatic m_sticCompany;
	CCEStatic m_sticCompanyAddress;
	CCEStatic m_sticPostCode;
	CCEStatic m_sticDepartment;
	CCEStatic m_sticDuty;
	CCEStatic m_sticMemo;
	CCEStatic m_sticOperation;
	CCEStatic m_sticSepLine1;
	CCEStatic m_sticSepLine2;

	CCERectEdit m_edtName;
	CCERectEdit m_edtMobile1;
	CCERectEdit m_edtMobile2;
	CCERectEdit m_edtTelephone1;
	CCERectEdit m_edtTelephone2;
	CCERectEdit m_edtEmail;
	CCERectEdit m_edtSite;
	CCERectEdit m_edtCompany;
	CCERectEdit m_edtCompanyAddress;
	CCERectEdit m_edtPostCode;
	CCERectEdit m_edtDepartment;
	CCERectEdit m_edtDuty;
	CCERectEdit m_edtMemo;

	CCEComboBox_ m_cmbType;
	CCEComboBox_ m_cmbGroup;
	
	CCEBmpButton m_btnDialMobile1;
	CCEBmpButton m_btnDialMobile2;
	CCEBmpButton m_btnDialTelephone1;
	CCEBmpButton m_btnDialTelephone2;

	CCEBmpButton m_btnOk;
	CCEBmpButton m_btnCancel;
	CCEBmpButton m_btnDelete;

//	CCEFrameStatic m_backGround;

	CImageList* m_pImageList;
	CCEFrameStatic m_sticBackground;
	
	void SetButtonDefaultColor(CCEButtonST* button);
	void SetButtonSelectedColor(CCEButtonST* button);

	boost::shared_ptr<Data::Contact> m_pContact;

	void AddGroupItems(void);
	char* LoadPinYinIndex(void);
	char* m_pPinYinIndex;
	std::string GetAlias(std::string name);

public:
	void SetContact(boost::shared_ptr<Data::Contact> contact);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONTACTNEWDLG_H__96578402_C2C4_43A0_9ABA_90B878DE7D1D__INCLUDED_)
