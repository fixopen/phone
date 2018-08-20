#if !defined(AFX_SOUNDDLG_H__641E06F2_DBB9_4EFF_96A3_2B8626006167__INCLUDED_)
#define AFX_SOUNDDLG_H__641E06F2_DBB9_4EFF_96A3_2B8626006167__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SoundDlg.h : header file
//
#include "../Control/CEListCtrl.h"
#include "../Control/CEStatic.h"
#include "../Control/CeBtnST.h"
#include "../Data/SoundSegment.h"
#include "PlaySoundDlg.h"
#include "SoundSearchDlg.h"
#include "passwordDlg.h"
#include "RecordSoundDlg.h"
#include "DetailTip.h"
#include "TIP.h"
//#define WM_CHANGELANGUAGE (WM_USER+300)
/////////////////////////////////////////////////////////////////////////////
// CSoundDlg dialog

class CSoundDlg : public CDialog
{
// Construction
public:
	CSoundDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSoundDlg)
	enum { IDD = IDD_DIALOG_SOUND };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSoundDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSoundDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	afx_msg void OnClickListType(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickListList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnListCltrlClick(WPARAM w, LPARAM l);
	afx_msg void OnButtonSound();
	afx_msg void OnButtonRecord();
	afx_msg void OnButtonSearch();
	afx_msg void OnButtonSetTip();
	afx_msg void OnButtonDelete();
	afx_msg void OnButtonDetailTIP();
	afx_msg void OnButtonClose();
	afx_msg void OnButtonContact();
	DECLARE_MESSAGE_MAP()
private:
	CCEStatic	m_sticTypeTitle;
	CCEStatic	m_sticSound;
	CCEStatic	m_sticNameNO;
	CCEStatic	m_sticTime;
	CCEStatic	m_sticDuration;
	CCEStatic	m_sticOperation;
	CCEStatic	m_sticSepLine1;
	CCEStatic	m_sticSepLine2;
	CCEStatic   m_sticPanel;
	CCEListCtrl	m_lsList;
	CCEListCtrl	m_lsType;
	CImageList* m_pImageList;
	CCEBmpButton m_btnSound;
	CCEBmpButton m_btnRecord;
	CCEBmpButton m_btnSearch;
	CCEBmpButton m_btnSetTip;
	CCEBmpButton m_btnDelete;
	CCEBmpButton m_btnDetail;
	CCEBmpButton m_btnClose;
    CCEBmpButton m_btncontact;
	CCEFrameStatic m_sticBackground;
	bool m_bTelephoneInUse;
	void SetButtonDefaultColor(CCEButtonST* button);
	void SetButtonSelectedColor(CCEButtonST* button);
	void DeleteSelectedItem(void);

	const int SoundTotal;
	const int PageSize;
	int m_iType;
// 	int currentPage;
    int iSel;
	std::string m_sListFilter;
	std::string m_sListSearchFilter;
	std::string findfilter;
	std::vector<boost::shared_ptr<Data::SoundSegment> > m_vResult;
	std::vector<boost::shared_ptr<Data::SoundSegment> > m_vSearchResult;
	std::vector<boost::shared_ptr<Data::SoundSegment> > m_vCurrentResult;
	void ShowSearchResultInList(std::vector<boost::shared_ptr<Data::SoundSegment> > array);
	void ScrollItemsInList(int step);
public:
	CSoundSearchDlg* m_pSoundSearchDlg;
	CPasswordDlg* m_pPasswordDlg;
	CRecordSoundDlg* m_pRecordSoundDlg;
	CPlaySoundDlg* m_pPlaySoundDlg;
	CDetailTip * m_pDetailTip;
	CTIP * m_psoundTipDlg;
	bool m_bPopPasswordDlg;
	void SaveSoundSegment(boost::shared_ptr<Data::SoundSegment> ss);
	void ShowArrayInList(std::vector<boost::shared_ptr<Data::SoundSegment> > array, int type = -1);
	void Search(std::string filter);
	void ShowItemsInList(void);
	void SetTypeListSelected(int index);
	void ChangeLanguage();
	void setButtonName();
};      
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
#endif // !defined(AFX_SOUNDDLG_H__641E06F2_DBB9_4EFF_96A3_2B8626006167__INCLUDED_)
