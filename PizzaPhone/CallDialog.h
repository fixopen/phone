#pragma once


// CallDialog dialog

class CallDialog : public CDialog
{
	DECLARE_DYNAMIC(CallDialog)

public:
	CallDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CallDialog();

// Dialog Data
	enum { IDD = IDD_DIALOG_CALL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
