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

#include "../control/MJPGStatic.h"

/////////////////////////////////////////////////////////////////////////////
// CContactNewDlg dialog

class CContactNewDlg : public CDialog
{
// Construction
public:
	CCEComboBox_ m_cmbType;
	CCEComboBox_ m_cmbGroup;
	
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
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual BOOL PreTranslateMessage(MSG* pMsg);

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
	afx_msg void OnClickMJPG(WPARAM w, LPARAM l);
	afx_msg void OnComboSelect(WPARAM w, LPARAM l);

	DECLARE_MESSAGE_MAP()
private:
	CMJPGStatic		m_MJPGList;
/*
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
*/
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
	
//	CCEComboBox_ m_comStorage;//add by qi 2009_09_28,存储来至组别

/*
	CCEBmpButton m_btnDialMobile1;
	CCEBmpButton m_btnDialMobile2;
	CCEBmpButton m_btnDialTelephone1;
	CCEBmpButton m_btnDialTelephone2;

	CCEBmpButton m_btnOk;
	CCEBmpButton m_btnCancel;
	CCEBmpButton m_btnDelete;
*/

//	CCEFrameStatic m_backGround;

	CImageList* m_pImageList;
//	CCEFrameStatic m_sticBackground;
	
	void SetButtonDefaultColor(CCEButtonST* button);
	void SetButtonSelectedColor(CCEButtonST* button);

	boost::shared_ptr<Data::Contact> m_pContact;

	void AddGroupItems(void);
	char* LoadPinYinIndex(void);
	std::string ReplaceSingleQuotation(std::string& field);

private:
	std::vector<int>	m_vSimID;
	bool				m_bSIMGroup;//是否是sim卡组	

public:
	char* m_pPinYinIndex;
	void SetContact(boost::shared_ptr<Data::Contact> contact);
	std::string GetAlias(std::string name);

	// add by qi 2009_09_28
	void ClearEdit();//选中SIM卡组，除了前三个控件可编辑，其它不可编辑
	void SetSim(bool btrue);//设置SIM卡的编辑环境
	void ShowWindow_();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONTACTNEWDLG_H__96578402_C2C4_43A0_9ABA_90B878DE7D1D__INCLUDED_)
