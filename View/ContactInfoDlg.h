#if !defined(AFX_CONTACTINFODLG_H__C49FEE49_5250_4FFF_955B_50F8FF8D5BD7__INCLUDED_)
#define AFX_CONTACTINFODLG_H__C49FEE49_5250_4FFF_955B_50F8FF8D5BD7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ContactInfoDlg.h : header file
//
#include "../Control/CEListCtrl.h"
#include "../Control/CEStatic.h"
#include "../Control/CeBtnST.h"
#include "../Data/ContactData.h"
#include "../Util/SmartPtr.h"
#include "PlaySoundDlg.h"
#include "ContactInfoSearchDlg.h"
#include "../Data/ContactInfo.h"
//#include "noteDlg.h"
#include "PasswordDlg.h"

#include "../control/MJPGStatic.h"

/////////////////////////////////////////////////////////////////////////////
// CContactInfoDlg dialog

class CContactInfoDlg : public CDialog
{
// Construction
public:
	CContactInfoDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CContactInfoDlg)
	enum { IDD = IDD_DIALOG_CONTACTINFO };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CContactInfoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CContactInfoDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	afx_msg void OnClickListType(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickListList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonDial();
	afx_msg void OnButtonSound();
	afx_msg void OnButtonNote();
	afx_msg void OnButtonNew();
	afx_msg void OnButtonSearch();
	afx_msg void OnButtonDelete();
	afx_msg void OnButtonClose();
	afx_msg void OnClickMJPG(WPARAM w, LPARAM l);
	afx_msg void OnListCltrlClick(WPARAM w, LPARAM l);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	DECLARE_MESSAGE_MAP()

private:
	
	CMJPGStatic		m_MJPGList;
	CFont m_NewListFont;
	/*
	CCEStatic	m_sticTypeTitle;
	CCEStatic	m_sticNameNO;
	CCEStatic	m_sticTime;
	CCEStatic	m_sticDuration;
	CCEStatic	m_sticSound;
	CCEStatic	m_sticOperation;
	CCEStatic	m_sticSepLine1;
	CCEStatic	m_sticSepLine2;


	CCEListCtrl	m_lsType;
	*/
	CCEListCtrl	m_lsList;

	CImageList* m_pImageList;
	CCEBmpButton m_btnNewContact;
	/*
	CCEBmpButton m_btnDial;
	CCEBmpButton m_btnSound;
	CCEBmpButton m_btnNote;
	
	CCEBmpButton m_btnSearch;
	CCEBmpButton m_btnDelete;
	CCEBmpButton m_btnClose;
	CCEStatic  m_sticPanel;
	CCEFrameStatic m_sticBackground;
	*/
	void SetButtonDefaultColor(CCEButtonST* button);
	void SetButtonSelectedColor(CCEButtonST* button);

	CContactInfoSearchDlg* m_pSearchContactInfoDlg;
// 	CNoteDlg* m_pNoteDlg;
	CPasswordDlg*  m_pPasswordDlg;
	bool m_bTelephoneInUse;

	const int ContactInfoTotal;
	const int PageSize;
	unsigned int m_uiSelectIndex;
	unsigned int m_uiType;

	std::string m_sListFilter;
	std::string m_sListSearchFilter;
	std::vector<boost::shared_ptr<Data::ContactInfo> > m_vCurrentResult;
	void ShowArrayInList(std::vector<boost::shared_ptr<Data::ContactInfo> > array);
	std::string GetName(std::string number);
 	void DeleteSelectedItem(void);
	void ScrollItemsInList(int step, int nPos);

public:
	CPlaySoundDlg* m_pPlaySoundDlg;
	void ShowItemsInList(int type);
	void SaveContactInfo(boost::shared_ptr<Data::ContactInfo> ci);
	void Search(std::string filter);
	void ShowUnconnectItems(void);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONTACTINFODLG_H__C49FEE49_5250_4FFF_955B_50F8FF8D5BD7__INCLUDED_)
