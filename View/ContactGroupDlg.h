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
#include "..\Util\OggCodec.h"

/////////////////////////////////////////////////////////////////////////////
// CContactGroupDlg dialog

class CContactGroupDlg : public CDialog
{
// Construction
public:
	CContactGroupDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CContactGroupDlg)
	enum { IDD = IDD_DIALOG_CONTACTGROUP };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


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
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	afx_msg void OnButtonContactGroupOk();
	afx_msg void OnButtonContactGroupCancel();
	afx_msg void OnButtonContactGroupDelete();
	afx_msg void OnButtonPlayRing();
	afx_msg void OnButtonPlayLeaveWord();
private:
public:
	CCEStatic m_sticTitle;
	CCEStatic m_sticName;
	CCEStatic m_sticRing;
	CCEStatic m_sticSoudTip;
	CCEBmpButton m_btnPlayRing;
	CCEBmpButton m_btnPlayLeaveWord;
	CCEStatic m_sticOperation;
	CCEStatic m_sticSepLine2;
	CCERectEdit m_edtName;

	CCEComboBox_ m_cmbRing;
	CCEComboBox_ m_cmbSoundTip;

	CCEBmpButton m_btnOk;
	CCEBmpButton m_btnCancel;
	CCEBmpButton m_btnDelete;
	CCEFrameStatic m_sticBackground;
	
	void SetButtonDefaultColor(CCEButtonST* button);
	void SetButtonSelectedColor(CCEButtonST* button);

	boost::shared_ptr<Data::ContactGroup> m_pContactGroup;
	OggCodec* m_pOggCodec;
	bool m_bPlayPause;
	bool m_bPlaying;
	bool m_iSBell;
	int m_iTotalSeconds;
	int m_iCurrentSecond;
	void SetButtoName();

	void AddRingTipItems(void);
	void ChangeLanguage();
public:
	void SetContactGroup(boost::shared_ptr<Data::ContactGroup> contactgroup);

	std::vector<CString > m_vGroupTipsDir;//Ŀ¼
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONTACTGROUPDLG_H__995E2317_44FA_497B_A5D8_7D1C6592073F__INCLUDED_)
