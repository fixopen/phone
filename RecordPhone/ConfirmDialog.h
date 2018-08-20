#pragma once

#include <string>
#include "ImageButton.h"

// ConfirmDialog dialog

class ConfirmDialog : public CDialog {
	DECLARE_DYNAMIC(ConfirmDialog)

public:
	ConfirmDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~ConfirmDialog();

// Dialog Data
	enum { IDD = IDD_DIALOG_CONFIRM };

    void SetConfirmMessage(std::wstring const& message);

    void SetListener(CWnd* listener) {
        listener_ = listener;
    }
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    bool isOk_;
    CWnd* listener_;
    ImageButton okButton_;
    ImageButton cancelButton_;

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedButtonYes();
    afx_msg void OnBnClickedButtonNo();
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    virtual BOOL OnInitDialog();
};

extern ConfirmDialog* confirmForm;