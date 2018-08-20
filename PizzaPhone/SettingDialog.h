#pragma once


// SettingDialog dialog

class SettingDialog : public CDialog
{
	DECLARE_DYNAMIC(SettingDialog)

public:
	SettingDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~SettingDialog();

// Dialog Data
	enum { IDD = IDD_DIALOG_SETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
