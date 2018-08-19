#pragma once

#include "ImageButton.h"

// SystemInfoDialog dialog

class SystemInfoDialog : public CDialog {
	DECLARE_DYNAMIC(SystemInfoDialog)

public:
	SystemInfoDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~SystemInfoDialog();

// Dialog Data
	enum { IDD = IDD_DIALOG_SYSTEM_INFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	ImageButton closeButton_;
	ImageButton callButton_;
	ImageButton voiceMessagesButton_;

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedButtonClose();
    afx_msg void OnBnClickedButtonClearContacts();
    afx_msg void OnBnClickedButtonClearVoiceMessages();
    afx_msg void OnBnClickedButtonClearCallHistory();
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    afx_msg void OnBnClickedButtonCall();
    afx_msg void OnBnClickedButtonVoiceMessages();
    virtual BOOL OnInitDialog();
    void ShowConfig();
protected:
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};
