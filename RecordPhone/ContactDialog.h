#pragma once
#include "Contact.h"
#include "CallDialog.h"
#include "ImageButton.h"

// ContactDialog dialog

class ContactDialog : public CDialog {
	DECLARE_DYNAMIC(ContactDialog)

public:
	ContactDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~ContactDialog();

// Dialog Data
	enum { IDD = IDD_DIALOG_CONTACT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
    bool isSuppressSuggest_;
    Util::shared_ptr<Contact> contact_;
    CWnd* listener_;
	CFont font_;
	ImageButton closeButton_;
	ImageButton callButton_;
	ImageButton voiceMessagesButton_;
public:
    void SetContact(Util::shared_ptr<Contact> const& contact);
    void SyncContact();
    void SetListener(CWnd* listener) {
        listener_ = listener;
    }
    void SuppressSuggest() {
        isSuppressSuggest_ = true;
    }
    Util::shared_ptr<Contact> const GetContact() const;
    afx_msg void OnBnClickedButtonClose();
    virtual BOOL OnInitDialog();
    afx_msg void OnBnClickedButtonCallPhone();
    afx_msg void OnBnClickedButtonCallPhone2();
    afx_msg void OnBnClickedButtonCallMobile();
    afx_msg void OnBnClickedButtonCallMobile2();
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    afx_msg void OnBnClickedButtonCall();
    afx_msg void OnBnClickedButtonVoiceMessages();
    afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
    afx_msg void OnEnSetfocusEditName();
    afx_msg void OnEnKillfocusEditName();
    afx_msg void OnEnSetfocusEditPhone();
    afx_msg void OnEnKillfocusEditPhone();
    afx_msg void OnEnSetfocusEditPhone2();
    afx_msg void OnEnKillfocusEditPhone2();
    afx_msg void OnEnSetfocusEditMobile();
    afx_msg void OnEnKillfocusEditMobile();
    afx_msg void OnEnSetfocusEditMobile2();
    afx_msg void OnEnKillfocusEditMobile2();
    afx_msg void OnEnSetfocusEditEmail();
    afx_msg void OnEnKillfocusEditEmail();
    afx_msg void OnEnSetfocusEditAddress();
    afx_msg void OnEnKillfocusEditAddress();
    afx_msg void OnEnSetfocusEditCompany();
    afx_msg void OnEnKillfocusEditCompany();
    afx_msg void OnEnSetfocusEditDepartment();
    afx_msg void OnEnKillfocusEditDepartment();
    afx_msg void OnEnSetfocusEditDuty();
    afx_msg void OnEnKillfocusEditDuty();
    afx_msg void OnEnSetfocusEditRemarks();
    afx_msg void OnEnKillfocusEditRemarks();
protected:
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnCbnSelchangeComboCategory();
    afx_msg void OnEnChangeEditName();
    afx_msg void OnLbnSelchangeListSuggestName();
    afx_msg void OnLbnDblclkListSuggestName();
	afx_msg void OnEnSetfocusEditAddress2();
	afx_msg void OnEnKillfocusEditAddress2();
};

extern ContactDialog* contactForm;