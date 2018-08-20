#if !defined(AFX_TELEPHONEDLG_H__2FAA5271_4CC6_4EE3_B0A8_045FE16A15B1__INCLUDED_)
#define AFX_TELEPHONEDLG_H__2FAA5271_4CC6_4EE3_B0A8_045FE16A15B1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// telephonedlg.h : header file
//
#include "../Control/CEStatic.h"
#include "../Control/CeBtnST.h"
#include "../Control/CEDialog.h"
#include "NoteDlg.h"
#include "../Data/ContactInfo.h"
#include "../Data/SoundSegment.h"

/////////////////////////////////////////////////////////////////////////////
// CTelephoneDlg dialog

class CTelephoneDlg : public CCEDialog
{
// Construction
public:
	CTelephoneDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTelephoneDlg)
	enum { IDD = IDD_DIALOG_TELEPHONE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTelephoneDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTelephoneDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	afx_msg void OnButtonTelephoneHide();
	afx_msg void OnButtonTelephoneNote();
	afx_msg void OnButtonTelephoneRecord();
	afx_msg void OnButtonTelephoneHandle();
	DECLARE_MESSAGE_MAP()

private:
	CCEStatic m_sticTitle;
	CCEStatic m_sticNameNo;
	CCEStatic m_sticCompany;
	CCEStatic m_sticDepartment;
	CCEStatic m_sticDuty;
	CCEStatic m_sticRecordCaption;
	CCEStatic m_sticRecord;
	CCEStatic m_sticDurationCaption;
	CCEStatic m_sticDuration;
	CCEStatic m_sticBackground;
	
	CCEButtonST m_btnHide;
	CCEButtonST m_btnNote;
	CCEButtonST m_btnRecord;
	CCEButtonST m_btnHandle;
	CCEButtonST m_btnRecordTip;

	void SetStaticDefaultColor(CCEStatic* cestatic);

	CNoteDlg* m_pNoteDlg;
	boost::shared_ptr<Data::ContactInfo> m_spContactInfo;
	boost::shared_ptr<Data::SoundSegment> m_spSoundSegment;
	std::vector<boost::shared_ptr<Data::ContactInfoSoundSegmentMap> >m_vCISSMap;
	CString m_strRecord;
	CString m_strStopRecord;
	CString m_strHangOff;
	CString m_strHangOn;
	UINT m_uiTelephoneTimer;
	UINT m_uiTelephoneSecond;
	UINT m_uiRecordTimer;
	UINT m_uiRecordSecond;
	UINT m_uiRingCount;
	UINT m_uiCurrentContactID;
	
public:
	void HangOff_(void* param);
	void HangOn_(void* param);
	void Key_(void* param);
	void SubKey_(void* param);
	void Connect_(void* param);
	void Ring_(void* param);
	void Code_(void* param);
	void CallID_(void* param);
	void ShowContact(boost::shared_ptr<Data::Contact> contact);
	void DialContact(std::string number, int contactId = 0);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TELEPHONEDLG_H__2FAA5271_4CC6_4EE3_B0A8_045FE16A15B1__INCLUDED_)
