#if !defined(AFX_CONTACTGROUPDLG_H__995E2317_44FA_497B_A5D8_7D1C6592073F__INCLUDED_)
#define AFX_CONTACTGROUPDLG_H__995E2317_44FA_497B_A5D8_7D1C6592073F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// contactgroupdlg.h : header file
//
#include "../Control/CELineEdit.h"
#include "../Control/CEStatic.h"
#include "../Control/CeBtnST.h"
#include "../Control/CECombobox_.h"
#include "../Control/CEListCtrl.h"
#include "../Data/ContactGroup.h"

#include "../control/MJPGStatic.h"

/////////////////////////////////////////////////////////////////////////////
// CContactGroupDlg dialog

class CContactGroupDlg : public CDialog
{
// Construction
public:
	CCEComboBox_ m_cmbSoundTip;
	CContactGroupDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CContactGroupDlg)
	enum { IDD = IDD_DIALOG_CONTACTGROUP };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
	afx_msg void OnStopTryRing(WPARAM w, LPARAM l);
	void StopTryRing();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CContactGroupDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CContactGroupDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	afx_msg void OnButtonContactGroupOk();
	afx_msg void OnButtonContactGroupCancel();
	afx_msg void OnButtonContactGroupDelete();

	afx_msg void OnRingLst(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRingSelect();
	afx_msg void OnClickMJPG(WPARAM w, LPARAM l);
	afx_msg void OnListCltrlClick(WPARAM w, LPARAM l);
	DECLARE_MESSAGE_MAP()
private:
	CMJPGStatic		m_MJPGList;

	/*
	CCEStatic m_sticTitle;
	CCEStatic m_sticName;
	CCEStatic m_sticRing;
	CCEStatic m_sticSoudTip;
	CCEStatic m_sticOperation;
	CCEStatic m_sticSepLine2;
	*/
	CCERectEdit m_edtName;

//	CCEComboBox_ m_cmbRing;
	//铃声选择由Combox改成ListCtrl
	CImageList *m_pImageList1;
	CCERectEdit m_ringEdit;
	CCEBmpButton m_ringButton;
	CCEListCtrl m_ringList;
	CCEStatic m_ringStatic;
	TCHAR	  m_ringDir[128];
	void SetRingLst(TCHAR *dir);

	/*
	CCEBmpButton m_btnOk;
	CCEBmpButton m_btnCancel;
	CCEBmpButton m_btnDelete;
	
	CCEFrameStatic m_sticBackground;
	*/
	
	void SetButtonDefaultColor(CCEButtonST* button);
	void SetButtonSelectedColor(CCEButtonST* button);

	boost::shared_ptr<Data::ContactGroup> m_pContactGroup;

	void AddRingTipItems(void);
public:
	void SetContactGroup(boost::shared_ptr<Data::ContactGroup> contactgroup);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONTACTGROUPDLG_H__995E2317_44FA_497B_A5D8_7D1C6592073F__INCLUDED_)
