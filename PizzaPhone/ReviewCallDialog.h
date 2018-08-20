#pragma once


// ReviewCallDialog dialog

class ReviewCallDialog : public CDialog
{
	DECLARE_DYNAMIC(ReviewCallDialog)

public:
	ReviewCallDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~ReviewCallDialog();

// Dialog Data
	enum { IDD = IDD_DIALOG_REVIEW_CALL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
