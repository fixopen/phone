#pragma once


// ContactDialog dialog

class ContactDialog : public CDialog
{
	DECLARE_DYNAMIC(ContactDialog)

public:
	ContactDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~ContactDialog();

// Dialog Data
	enum { IDD = IDD_DIALOG_CONTACT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
