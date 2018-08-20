#pragma once


// ContactsDialog dialog

class ContactsDialog : public CDialog
{
	DECLARE_DYNAMIC(ContactsDialog)

public:
	ContactsDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~ContactsDialog();

// Dialog Data
	enum { IDD = IDD_DIALOG_CONTACTS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
