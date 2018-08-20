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
#include "../Control/CEDialog.h"

//#define WM_CHANGELANGUAGE (WM_USER+300)
/////////////////////////////////////////////////////////////////////////////
// CContactDlg dialog

class CContactDlg : public CDialog
{
// Construction
public:
	CContactDlg(CWnd* pParent = NULL);   // standard constructor
   	CContactSearchDlg* m_pContactSearchDlg;
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
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL
// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CContactDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnTimer(UINT nIDEvent);
//	afx_msg LRESULT OnChangeLanguage();
	//}}AFX_MSG
	afx_msg void OnClickListType(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickListList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnListCltrlClick(WPARAM w, LPARAM l);
	afx_msg void OnButtonContactNew();
	afx_msg void OnButtonContactNewtype();
	afx_msg void OnButtonContactEdittype();
	afx_msg void OnButtonContactSearch();
	afx_msg void OnButtonContactClose();
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
	CCEStatic   m_sticPanel;
	CCEFrameStatic m_sticBackground;
	CCEListCtrl	m_lsType;
	CCEListCtrl	m_lsList;
	CSoftKey    m_skSoftKey;
	CImageList* m_pImageList;
	CCEBmpButton m_btnNewContact;
	CCEBmpButton m_btnNewType;
	CCEBmpButton m_btnEditType;
	CCEBmpButton m_btnSearch;
	CCEBmpButton m_btnClose;
	void SetButtonDefaultColor(CCEButtonST* button);
	void SetButtonSelectedColor(CCEButtonST* button);
public:
	const int ContactTotal;
	const int PageSize;
	int iSel;
    boost::shared_ptr<Data::Contact> m_pContt;
private:
	std::string m_sListFilter;
	std::string m_sListSearchFilter;
	std::string findfilter;
	std::vector<boost::shared_ptr<Data::Contact> > m_vCurrentResult;
	void ShowArrayInList(std::vector<boost::shared_ptr<Data::Contact> > array);
	std::string GetPYIndex(void);
	void ScrollItemsInList(int step);
public:
	void ShowItemsInList(void);
	void ShowTypeItems(void);
	void Search(std::string filter);
	void ChangeLanguage();
//	void OnBTNew();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONTACTDLG_H__8E8BDFEC_AA75_4628_8820_CDAA92906516__INCLUDED_)
