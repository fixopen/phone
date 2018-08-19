#pragma once

#include "TelephoneSettingDialog.h"
#include "SystemParametersDialog.h"
#include "SystemInfoDialog.h"
#include "ImageButton.h"

// SettingDialog dialog

class SettingDialog : public CDialog {
	DECLARE_DYNAMIC(SettingDialog)

public:
	SettingDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~SettingDialog();

// Dialog Data
	enum { IDD = IDD_DIALOG_SETTING };

	//"INSERT INTO \"tableName\" ( \"fieldName\" ) VALUES ( CAST('ddd' AS money) )"

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
    TelephoneSettingDialog* telephoneSettingForm_;
    SystemParametersDialog* systemParametersForm_;
    SystemInfoDialog* systemInfoForm_;
	ImageButton closeButton_;
	ImageButton callButton_;
	ImageButton voiceMessagesButton_;
	ImageButton telephoneButton_;
	ImageButton parametersButton_;
	ImageButton infoButton_;
public:
    afx_msg void OnBnClickedButtonClose();
    virtual BOOL OnInitDialog();
    afx_msg void OnDestroy();
    afx_msg void OnBnClickedButtonTelephone();
    afx_msg void OnBnClickedButtonSystemParameters();
    afx_msg void OnBnClickedButtonSystemInfo();
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnBnClickedButtonCall();
    afx_msg void OnBnClickedButtonVoiceMessages();
    //afx_msg void OnPaint();
    afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
protected:
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnBnClickedButton1();
};

extern SettingDialog* settingForm;