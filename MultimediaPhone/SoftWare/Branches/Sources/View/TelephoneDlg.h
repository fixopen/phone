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
//#include "NoteDlg.h"
#include "../Data/ContactInfo.h"
#include "../Data/SoundSegment.h"
#include "../inc/SABTD.h"


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
	afx_msg void OnMM_WIM_OPEN(UINT wParam,LONG lParam);
	afx_msg void OnMM_WIM_DATA(UINT wParam,LONG lParam);
	afx_msg void OnMM_WIM_CLOSE(UINT wParam,LONG lParam);
	afx_msg void OnMM_WOM_OPEN(UINT wParam,LONG lParam);
	afx_msg void OnMM_WOM_DONE(UINT wParam,LONG lParam);
	afx_msg void OnMM_WOM_CLOSE(UINT wParam,LONG lParam);
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
	
	CCEBmpButton m_btnHide;
	CCEBmpButton m_btnNote;
	CCEBmpButton m_btnRecord;
	CCEBmpButton m_btnHandle;
	CCEBmpButton m_btnRecordTip;
	CCEFramePartStatic m_sticBackground;

//	CNoteDlg* m_pNoteDlg;
	boost::shared_ptr<Data::ContactInfo> m_spContactInfo;
	boost::shared_ptr<Data::SoundSegment> m_spSoundSegment;
	std::vector<boost::shared_ptr<Data::SoundSegment> >m_vSoundSegment;
	CString m_strRecord;
	CString m_strStopRecord;
	CString m_strHangOff;
	CString m_strHangOn;
	UINT m_uiTelephoneTimer;
	UINT m_uiTelephoneSecond;
	UINT m_uiRecordTimer;
	UINT m_uiRecordSecond;
	UINT m_uiRingCount;
	UINT m_uiTipTimer;
	UINT m_uiInNoCount;
	UINT m_uiRecordCound;
	BOOL m_bAutoRecord;
	BOOL m_bISRecording;
	int m_iAdjustTimes;
	UINT m_uiIgnoreRing;
	BOOL m_bHasCallID;
	UINT m_uiRemainRecordSecond;
	std::string m_sTelephoneNumber;
private:
	volatile BOOL m_bEnding;
	BOOL m_bPlaying;
 	DWORD m_dwDataLength;
	HWAVEIN m_hWaveIn;
	HWAVEOUT m_hWaveOut;
	PBYTE m_pBuffer1;
	PBYTE m_pBuffer2;
	PBYTE m_pOutBuffer;
	PWAVEHDR m_pWaveHdr1;
	PWAVEHDR m_pWaveHdr2;
	CFile m_fWav;	
	WAVEFORMATEX m_waveform;
	std::string m_sRecordFileName;
	std::string m_sPlayFileName;
	bool RecStart(void);
	bool RecStop(void);
	void SaveWavHead(void);
	bool PlayStart(void);
	bool PlayStop(void);

	BOOL m_bCheckingTone;
	BOOL m_bPlayingLeaveWord;
	bool InitData(void);

public:
	void HangOff_(void* param);
	void HangOn_(void* param);
	void Key_(void* param);
	void SubKey_(void* param);
	void Connect_(void* param);
	void Ring_(void* param);
	void CallID_(void* param);
	std::vector<boost::shared_ptr<Data::Contact> > FindCallContact(std::string number);
	void ShowContact(boost::shared_ptr<Data::Contact> contact);
	void DialContact(Data::TelephoneNumber telephoneNumber, int contactId = 0);

	void Mute(void);
	void Redial(void);
	void HangOnInRecord(void);
	void HangOnToRecord(void);

	UINT GetUnconnectCount(void);
	void ClearUnconnectCount(void);
	UINT GetRecordCount(void); 
	void ClearRecordCount(void);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TELEPHONEDLG_H__2FAA5271_4CC6_4EE3_B0A8_045FE16A15B1__INCLUDED_)
