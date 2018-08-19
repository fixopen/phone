#pragma once
#include "Contact.h"
#include "ContactDialog.h"
#include "CategoryDialog.h"
#include "ImageButton.h"
// ContactsDialog dialog

class ContactsDialog : public CDialog {
	DECLARE_DYNAMIC(ContactsDialog)

public:
	ContactsDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~ContactsDialog();

// Dialog Data
	enum { IDD = IDD_DIALOG_CONTACTS };

    static size_t const pageSize = 10;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
    void refreshList_();
    void drawContact_(Util::shared_ptr<Contact> const& contact, int const index);
    void CategoryEdit(int const controlId, int const no);
    void Delete(int const no);
    void ShowDetail(int const no);
    std::vector<Util::shared_ptr<Contact> > contacts_;
    //Util::shared_ptr<Contact> currentContact_;
    ContactDialog* contactForm_;
    bool isNewContact_;
    bool isNewCategory_;
    CategoryDialog* contactCategoryForm_;
    std::wstring filter_;
    size_t offset_;
	CFont font_;
	ImageButton closeButton_;
	ImageButton pageDownButton_;
	ImageButton pageUpButton_;
    ImageButton importButton_;
	ImageButton clearButton_;
	ImageButton newContactButton_;
	ImageButton newCategoryButton_;
	ImageButton callButton_;
	ImageButton voiceMessagesButton_;
	ImageButton findButton_;
public:
    void SetContactType(bool const isNew) {
        isNewContact_ = isNew;
    }
    void SetContacts(std::vector<Util::shared_ptr<Contact> > const& contacts);
    afx_msg void OnBnClickedButtonClose();
    afx_msg void OnBnClickedButtonNewContact();
    virtual BOOL OnInitDialog();
protected:
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
    afx_msg void OnBnClickedButtonNewCategory();
    afx_msg void OnBnClickedButtonClear();
    afx_msg void OnBnClickedButtonPageUp();
    afx_msg void OnBnClickedButtonPageDown();
    afx_msg void OnStnClickedStaticCategory0();
    afx_msg void OnStnClickedStaticDelete0();
    afx_msg void OnStnClickedStaticName0();
    afx_msg void OnStnClickedStaticTelephone0();
    afx_msg void OnStnClickedStaticAddress0();
    afx_msg void OnStnClickedStaticType0();
    afx_msg void OnStnClickedStaticCategory1();
    afx_msg void OnStnClickedStaticDelete1();
    afx_msg void OnStnClickedStaticName1();
    afx_msg void OnStnClickedStaticTelephone1();
    afx_msg void OnStnClickedStaticAddress1();
    afx_msg void OnStnClickedStaticType1();
    afx_msg void OnStnClickedStaticCategory2();
    afx_msg void OnStnClickedStaticDelete2();
    afx_msg void OnStnClickedStaticName2();
    afx_msg void OnStnClickedStaticTelephone2();
    afx_msg void OnStnClickedStaticAddress2();
    afx_msg void OnStnClickedStaticType2();
    afx_msg void OnStnClickedStaticCategory3();
    afx_msg void OnStnClickedStaticDelete3();
    afx_msg void OnStnClickedStaticName3();
    afx_msg void OnStnClickedStaticTelephone3();
    afx_msg void OnStnClickedStaticAddress3();
    afx_msg void OnStnClickedStaticType3();
    afx_msg void OnStnClickedStaticCategory4();
    afx_msg void OnStnClickedStaticDelete4();
    afx_msg void OnStnClickedStaticName4();
    afx_msg void OnStnClickedStaticTelephone4();
    afx_msg void OnStnClickedStaticAddress4();
    afx_msg void OnStnClickedStaticType4();
    afx_msg void OnStnClickedStaticCategory5();
    afx_msg void OnStnClickedStaticDelete5();
    afx_msg void OnStnClickedStaticName5();
    afx_msg void OnStnClickedStaticTelephone5();
    afx_msg void OnStnClickedStaticAddress5();
    afx_msg void OnStnClickedStaticType5();
    afx_msg void OnStnClickedStaticCategory6();
    afx_msg void OnStnClickedStaticDelete6();
    afx_msg void OnStnClickedStaticName6();
    afx_msg void OnStnClickedStaticTelephone6();
    afx_msg void OnStnClickedStaticAddress6();
    afx_msg void OnStnClickedStaticType6();
    afx_msg void OnStnClickedStaticCategory7();
    afx_msg void OnStnClickedStaticDelete7();
    afx_msg void OnStnClickedStaticName7();
    afx_msg void OnStnClickedStaticTelephone7();
    afx_msg void OnStnClickedStaticAddress7();
    afx_msg void OnStnClickedStaticType7();
    afx_msg void OnStnClickedStaticCategory8();
    afx_msg void OnStnClickedStaticDelete8();
    afx_msg void OnStnClickedStaticName8();
    afx_msg void OnStnClickedStaticTelephone8();
    afx_msg void OnStnClickedStaticAddress8();
    afx_msg void OnStnClickedStaticType8();
    afx_msg void OnStnClickedStaticCategory9();
    afx_msg void OnStnClickedStaticDelete9();
    afx_msg void OnStnClickedStaticName9();
    afx_msg void OnStnClickedStaticTelephone9();
    afx_msg void OnStnClickedStaticAddress9();
    afx_msg void OnStnClickedStaticType9();
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    afx_msg void OnBnClickedButtonCall();
    afx_msg void OnBnClickedButtonVoiceMessages();
    afx_msg void OnBnClickedButtonFind();
    afx_msg void OnEnSetfocusEditName();
    afx_msg void OnEnKillfocusEditName();
    afx_msg void OnEnSetfocusEditTelephone();
    afx_msg void OnEnKillfocusEditTelephone();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnBnClickedButtonImportContact();
};

extern ContactsDialog* contactsForm;