// RecordPhoneDlg.h : header file
//

#pragma once

#include "ImageButton.h"

// CRecordPhoneDlg dialog
class CRecordPhoneDlg : public CDialog {
// Construction
public:
	CRecordPhoneDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_RECORDPHONE_DIALOG };


	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

// Implementation
protected:
	HICON m_hIcon;
	UINT_PTR clock_;
	UINT_PTR timerId_;
    static size_t const pageSize = 10;
	ImageButton callButton_;
	ImageButton voiceMessagesButton_;

	ImageButton contactsButton_;
	ImageButton recordsButton_;
	ImageButton settingButton_;

	// Generated message map functions
	virtual BOOL OnInitDialog();
#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
	afx_msg void OnSize(UINT /*nType*/, int /*cx*/, int /*cy*/);
#endif
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedButtonContacts();
    afx_msg void OnBnClickedButtonPhoneRecords();
    afx_msg void OnBnClickedButtonSetting();
    afx_msg void OnBnClickedButtonVoiceMessages();
    afx_msg void OnBnClickedButtonCall();
    afx_msg void OnStnClickedStaticDateTime();
    afx_msg void OnDestroy();
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
protected:
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
