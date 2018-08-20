#if !defined(AFX_CONTACTDLG_H__8E8BDFEC_AA75_4628_8820_CDAA92906516__INCLUDED_)
#define AFX_CONTACTDLG_H__8E8BDFEC_AA75_4628_8820_CDAA92906516__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ContactDlg.h : header file
//
#include "../Control/CEListCtrl.h"
#include "../Control/CEStatic.h"
#include "../Control/CeBtnST.h"
#include "../Control/SoftKey.h"
#include "../Data/Contact.h"
#include "ContactSearchDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CContactDlg dialog

class CContactDlg : public CDialog
{
// Construction
public:
	CContactDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CContactDlg)
	enum { IDD = IDD_DIALOG_CONTACT };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CContactDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CContactDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	afx_msg void OnClickListType(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickListList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonContactNew();
	afx_msg void OnButtonContactNewtype();
	afx_msg void OnButtonContactEdittype();
	afx_msg void OnButtonContactSearch();
	afx_msg LRESULT OnSoftKeyMessage(WPARAM wParam, LPARAM lParam);   
	DECLARE_MESSAGE_MAP()
private:
	CCEStatic	m_sticTypeTitle;
	CCEStatic	m_sticName;
	CCEStatic	m_sticCompany;
	CCEStatic	m_sticDepartment;
	CCEStatic	m_sticDuty;
	CCEStatic	m_sticOperation;
	CCEStatic	m_sticSepLine1;
	CCEStatic	m_sticSepLine2;
	CCEStatic   m_sticPinYin;

	CCEListCtrl	m_lsType;
	CCEListCtrl	m_lsList;
	CSoftKey    m_skSoftKey;
	CImageList* m_pImageList;

	CCEButtonST m_btnNewContact;
	CCEButtonST m_btnNewType;
	CCEButtonST m_btnEditType;
	CCEButtonST m_btnSearch;
	
	void SetButtonDefaultColor(CCEButtonST* button);
	void SetButtonSelectedColor(CCEButtonST* button);
	
	std::vector<boost::shared_ptr<Data::Contact> > m_vSearchResult;
	void ShowArrayInList(std::vector<boost::shared_ptr<Data::Contact> > array);
	bool ShowItemsInListByAlias(void);
	void ShowItemsInListByAlias(std::string alias);

	CContactSearchDlg* m_pContactSearchDlg;

public:
	void ShowTypeItems(void);
	void ShowItemsInList(void);
	int GetTypeListSelected(void);
	void SetTypeListSelected(int index);
	void ShowSearchResultInList(std::vector<boost::shared_ptr<Data::Contact> > array);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONTACTDLG_H__8E8BDFEC_AA75_4628_8820_CDAA92906516__INCLUDED_)
