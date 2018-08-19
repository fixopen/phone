#pragma once

#include "ImageButton.h"

// SystemParametersDialog dialog

class SystemParametersDialog : public CDialog
{
	DECLARE_DYNAMIC(SystemParametersDialog)

public:
	SystemParametersDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~SystemParametersDialog();

// Dialog Data
	enum { IDD = IDD_DIALOG_SYSTEM_PARAMETERS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	CFont font_;
	ImageButton closeButton_;
	ImageButton restoreButton_;
	ImageButton callButton_;
	ImageButton voiceMessagesButton_;

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedButtonClose();
    afx_msg void OnBnClickedButtonRestore();
    afx_msg void OnBnClickedButtonScreenCalibration();
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
