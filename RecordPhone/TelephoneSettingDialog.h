#pragma once

#include <string>

#include "Contact.h" //for FirewallType
#include "ImageButton.h"

// TelephoneSettingDialog dialog

class TelephoneSettingDialog : public CDialog {
	DECLARE_DYNAMIC(TelephoneSettingDialog)

public:
	TelephoneSettingDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~TelephoneSettingDialog();
    void ShowConfig();

// Dialog Data
	enum { IDD = IDD_DIALOG_TELEPHONE_SETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    void ClearAllFirewall();
	CFont font_;
    FirewallType firewallType_;
	ImageButton closeButton_;
	ImageButton restoreButton_;
	ImageButton callButton_;
	ImageButton voiceMessagesButton_;

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedButtonRestore();
    afx_msg void OnBnClickedButtonClose();
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    afx_msg void OnBnClickedButtonCall();
    afx_msg void OnBnClickedButtonVoiceMessages();
    virtual BOOL OnInitDialog();
    afx_msg void OnEnSetfocusEditThisTelephoneNumber();
    afx_msg void OnEnKillfocusEditThisTelephoneNumber();
    afx_msg void OnEnSetfocusEditLocalAreaCode();
    afx_msg void OnEnKillfocusEditLocalAreaCode();
    afx_msg void OnEnSetfocusEditOutlinePrefix();
    afx_msg void OnEnKillfocusEditOutlinePrefix();
    afx_msg void OnEnSetfocusEditIpPrefix();
    afx_msg void OnEnKillfocusEditIpPrefix();
    afx_msg void OnEnSetfocusEditDuration();
    afx_msg void OnEnKillfocusEditDuration();
    afx_msg void OnEnSetfocusEditSpeedDial1();
    afx_msg void OnEnKillfocusEditSpeedDial1();
    afx_msg void OnEnSetfocusEditSpeedDial2();
    afx_msg void OnEnKillfocusEditSpeedDial2();
    afx_msg void OnEnSetfocusEditSpeedDial3();
    afx_msg void OnEnKillfocusEditSpeedDial3();
    afx_msg void OnEnSetfocusEditSpeedDial4();
    afx_msg void OnEnKillfocusEditSpeedDial4();
    afx_msg void OnEnSetfocusEditSpeedDial5();
    afx_msg void OnEnKillfocusEditSpeedDial5();
    afx_msg void OnEnSetfocusEditSpeedDial6();
    afx_msg void OnEnKillfocusEditSpeedDial6();
    afx_msg void OnEnSetfocusEditSpeedDial7();
    afx_msg void OnEnKillfocusEditSpeedDial7();
    afx_msg void OnEnSetfocusEditSpeedDial8();
    afx_msg void OnEnKillfocusEditSpeedDial8();
    afx_msg void OnEnSetfocusEditSpeedDial9();
    afx_msg void OnEnKillfocusEditSpeedDial9();
    afx_msg void OnEnSetfocusEditSpeedDial10();
    afx_msg void OnEnKillfocusEditSpeedDial10();
    afx_msg void OnEnSetfocusEditSpeedDial11();
    afx_msg void OnEnKillfocusEditSpeedDial11();
    afx_msg void OnEnSetfocusEditSpeedDial12();
    afx_msg void OnEnKillfocusEditSpeedDial12();
    afx_msg void OnBnClickedRadioNull();
    afx_msg void OnBnClickedRadioBlacklist();
    afx_msg void OnBnClickedRadioNotInContacts();
    afx_msg void OnBnClickedRadioNotVip();
    afx_msg void OnBnClickedRadioAll();
protected:
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};
