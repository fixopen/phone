// ContactsDialog.cpp : implementation file
//

#include "stdafx.h"

#include <sip.h>

#include "RecordPhone.h"
#include "ContactsDialog.h"
#include "CallDialog.h"
#include "PlayingRecordsDialog.h"
#include "Telephone.h"
#include "BackgroundImage.h"

#include "Util/ImageOp.h"

ContactsDialog* contactsForm = 0;

// ContactsDialog dialog

IMPLEMENT_DYNAMIC(ContactsDialog, CDialog)

ContactsDialog::ContactsDialog(CWnd* pParent /*=NULL*/)
: CDialog(ContactsDialog::IDD, pParent)
, offset_(0) {
}

ContactsDialog::~ContactsDialog() {
}

void ContactsDialog::DoDataExchange(CDataExchange* pDX) {
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(ContactsDialog, CDialog)
    ON_BN_CLICKED(IDC_BUTTON_CLOSE, &ContactsDialog::OnBnClickedButtonClose)
    ON_BN_CLICKED(IDC_BUTTON_NEW_CONTACT, &ContactsDialog::OnBnClickedButtonNewContact)
    ON_BN_CLICKED(IDC_BUTTON_NEW_CATEGORY, &ContactsDialog::OnBnClickedButtonNewCategory)
    ON_BN_CLICKED(IDC_BUTTON_CLEAR, &ContactsDialog::OnBnClickedButtonClear)
    ON_BN_CLICKED(IDC_BUTTON_PAGE_UP, &ContactsDialog::OnBnClickedButtonPageUp)
    ON_BN_CLICKED(IDC_BUTTON_PAGE_DOWN, &ContactsDialog::OnBnClickedButtonPageDown)
    ON_STN_CLICKED(IDC_STATIC_CATEGORY_0, &ContactsDialog::OnStnClickedStaticCategory0)
    ON_STN_CLICKED(IDC_STATIC_DELETE_0, &ContactsDialog::OnStnClickedStaticDelete0)
    ON_STN_CLICKED(IDC_STATIC_NAME_0, &ContactsDialog::OnStnClickedStaticName0)
    ON_STN_CLICKED(IDC_STATIC_TELEPHONE_0, &ContactsDialog::OnStnClickedStaticTelephone0)
    ON_STN_CLICKED(IDC_STATIC_ADDRESS_0, &ContactsDialog::OnStnClickedStaticAddress0)
    ON_STN_CLICKED(IDC_STATIC_TYPE_0, &ContactsDialog::OnStnClickedStaticType0)
    ON_STN_CLICKED(IDC_STATIC_CATEGORY_1, &ContactsDialog::OnStnClickedStaticCategory1)
    ON_STN_CLICKED(IDC_STATIC_DELETE_1, &ContactsDialog::OnStnClickedStaticDelete1)
    ON_STN_CLICKED(IDC_STATIC_NAME_1, &ContactsDialog::OnStnClickedStaticName1)
    ON_STN_CLICKED(IDC_STATIC_TELEPHONE_1, &ContactsDialog::OnStnClickedStaticTelephone1)
    ON_STN_CLICKED(IDC_STATIC_ADDRESS_1, &ContactsDialog::OnStnClickedStaticAddress1)
    ON_STN_CLICKED(IDC_STATIC_TYPE_1, &ContactsDialog::OnStnClickedStaticType1)
    ON_STN_CLICKED(IDC_STATIC_CATEGORY_2, &ContactsDialog::OnStnClickedStaticCategory2)
    ON_STN_CLICKED(IDC_STATIC_DELETE_2, &ContactsDialog::OnStnClickedStaticDelete2)
    ON_STN_CLICKED(IDC_STATIC_NAME_2, &ContactsDialog::OnStnClickedStaticName2)
    ON_STN_CLICKED(IDC_STATIC_TELEPHONE_2, &ContactsDialog::OnStnClickedStaticTelephone2)
    ON_STN_CLICKED(IDC_STATIC_ADDRESS_2, &ContactsDialog::OnStnClickedStaticAddress2)
    ON_STN_CLICKED(IDC_STATIC_TYPE_2, &ContactsDialog::OnStnClickedStaticType2)
    ON_STN_CLICKED(IDC_STATIC_CATEGORY_3, &ContactsDialog::OnStnClickedStaticCategory3)
    ON_STN_CLICKED(IDC_STATIC_DELETE_3, &ContactsDialog::OnStnClickedStaticDelete3)
    ON_STN_CLICKED(IDC_STATIC_NAME_3, &ContactsDialog::OnStnClickedStaticName3)
    ON_STN_CLICKED(IDC_STATIC_TELEPHONE_3, &ContactsDialog::OnStnClickedStaticTelephone3)
    ON_STN_CLICKED(IDC_STATIC_ADDRESS_3, &ContactsDialog::OnStnClickedStaticAddress3)
    ON_STN_CLICKED(IDC_STATIC_TYPE_3, &ContactsDialog::OnStnClickedStaticType3)
    ON_STN_CLICKED(IDC_STATIC_CATEGORY_4, &ContactsDialog::OnStnClickedStaticCategory4)
    ON_STN_CLICKED(IDC_STATIC_DELETE_4, &ContactsDialog::OnStnClickedStaticDelete4)
    ON_STN_CLICKED(IDC_STATIC_NAME_4, &ContactsDialog::OnStnClickedStaticName4)
    ON_STN_CLICKED(IDC_STATIC_TELEPHONE_4, &ContactsDialog::OnStnClickedStaticTelephone4)
    ON_STN_CLICKED(IDC_STATIC_ADDRESS_4, &ContactsDialog::OnStnClickedStaticAddress4)
    ON_STN_CLICKED(IDC_STATIC_TYPE_4, &ContactsDialog::OnStnClickedStaticType4)
    ON_STN_CLICKED(IDC_STATIC_CATEGORY_5, &ContactsDialog::OnStnClickedStaticCategory5)
    ON_STN_CLICKED(IDC_STATIC_DELETE_5, &ContactsDialog::OnStnClickedStaticDelete5)
    ON_STN_CLICKED(IDC_STATIC_NAME_5, &ContactsDialog::OnStnClickedStaticName5)
    ON_STN_CLICKED(IDC_STATIC_TELEPHONE_5, &ContactsDialog::OnStnClickedStaticTelephone5)
    ON_STN_CLICKED(IDC_STATIC_ADDRESS_5, &ContactsDialog::OnStnClickedStaticAddress5)
    ON_STN_CLICKED(IDC_STATIC_TYPE_5, &ContactsDialog::OnStnClickedStaticType5)
    ON_STN_CLICKED(IDC_STATIC_CATEGORY_6, &ContactsDialog::OnStnClickedStaticCategory6)
    ON_STN_CLICKED(IDC_STATIC_DELETE_6, &ContactsDialog::OnStnClickedStaticDelete6)
    ON_STN_CLICKED(IDC_STATIC_NAME_6, &ContactsDialog::OnStnClickedStaticName6)
    ON_STN_CLICKED(IDC_STATIC_TELEPHONE_6, &ContactsDialog::OnStnClickedStaticTelephone6)
    ON_STN_CLICKED(IDC_STATIC_ADDRESS_6, &ContactsDialog::OnStnClickedStaticAddress6)
    ON_STN_CLICKED(IDC_STATIC_TYPE_6, &ContactsDialog::OnStnClickedStaticType6)
    ON_STN_CLICKED(IDC_STATIC_CATEGORY_7, &ContactsDialog::OnStnClickedStaticCategory7)
    ON_STN_CLICKED(IDC_STATIC_DELETE_7, &ContactsDialog::OnStnClickedStaticDelete7)
    ON_STN_CLICKED(IDC_STATIC_NAME_7, &ContactsDialog::OnStnClickedStaticName7)
    ON_STN_CLICKED(IDC_STATIC_TELEPHONE_7, &ContactsDialog::OnStnClickedStaticTelephone7)
    ON_STN_CLICKED(IDC_STATIC_ADDRESS_7, &ContactsDialog::OnStnClickedStaticAddress7)
    ON_STN_CLICKED(IDC_STATIC_TYPE_7, &ContactsDialog::OnStnClickedStaticType7)
    ON_STN_CLICKED(IDC_STATIC_CATEGORY_8, &ContactsDialog::OnStnClickedStaticCategory8)
    ON_STN_CLICKED(IDC_STATIC_DELETE_8, &ContactsDialog::OnStnClickedStaticDelete8)
    ON_STN_CLICKED(IDC_STATIC_NAME_8, &ContactsDialog::OnStnClickedStaticName8)
    ON_STN_CLICKED(IDC_STATIC_TELEPHONE_8, &ContactsDialog::OnStnClickedStaticTelephone8)
    ON_STN_CLICKED(IDC_STATIC_ADDRESS_8, &ContactsDialog::OnStnClickedStaticAddress8)
    ON_STN_CLICKED(IDC_STATIC_TYPE_8, &ContactsDialog::OnStnClickedStaticType8)
    ON_STN_CLICKED(IDC_STATIC_CATEGORY_9, &ContactsDialog::OnStnClickedStaticCategory9)
    ON_STN_CLICKED(IDC_STATIC_DELETE_9, &ContactsDialog::OnStnClickedStaticDelete9)
    ON_STN_CLICKED(IDC_STATIC_NAME_9, &ContactsDialog::OnStnClickedStaticName9)
    ON_STN_CLICKED(IDC_STATIC_TELEPHONE_9, &ContactsDialog::OnStnClickedStaticTelephone9)
    ON_STN_CLICKED(IDC_STATIC_ADDRESS_9, &ContactsDialog::OnStnClickedStaticAddress9)
    ON_STN_CLICKED(IDC_STATIC_TYPE_9, &ContactsDialog::OnStnClickedStaticType9)
    ON_WM_CTLCOLOR()
    ON_BN_CLICKED(IDC_BUTTON_CALL, &ContactsDialog::OnBnClickedButtonCall)
    ON_BN_CLICKED(IDC_BUTTON_VOICE_MESSAGES, &ContactsDialog::OnBnClickedButtonVoiceMessages)
    ON_BN_CLICKED(IDC_BUTTON_FIND, &ContactsDialog::OnBnClickedButtonFind)
    ON_EN_SETFOCUS(IDC_EDIT_NAME, &ContactsDialog::OnEnSetfocusEditName)
    ON_EN_KILLFOCUS(IDC_EDIT_NAME, &ContactsDialog::OnEnKillfocusEditName)
    ON_EN_SETFOCUS(IDC_EDIT_TELEPHONE, &ContactsDialog::OnEnSetfocusEditTelephone)
    ON_EN_KILLFOCUS(IDC_EDIT_TELEPHONE, &ContactsDialog::OnEnKillfocusEditTelephone)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// ContactsDialog message handlers

void ContactsDialog::OnBnClickedButtonClose() {
    // TODO: Add your control notification handler code here
    ShowWindow(SW_HIDE);
    //contactForm->DestroyWindow();
    //delete contactForm;
    //OnOK();
    //DestroyWindow();
}

void ContactsDialog::SetContacts(std::vector<Util::shared_ptr<Contact> > const& contacts) {
    bool hasCall = Telephone::Instance()->HasCall();
    if (hasCall) {
        CWnd* control = GetDlgItem(IDC_BUTTON_CALL);
        control->ShowWindow(SW_SHOW);
    }

    CComboBox* categoriesControl = static_cast<CComboBox*>(GetDlgItem(IDC_COMBO_CATEGORY));
	//categoriesControl->Clear();
    categoriesControl->GetCount();
    while (categoriesControl->DeleteString(0))
        ;
    categoriesControl->AddString(L"All");
    std::vector<Util::shared_ptr<ContactCategory> > categories = ContactCategory::All();
    for (size_t i = 0; i < categories.size(); ++i) {
        categoriesControl->AddString(categories[i]->name().c_str());
    }
    categoriesControl->SetWindowTextW(L"All");
    contacts_ = contacts;
    CWnd* control = GetDlgItem(IDC_STATIC_CATEGORY_0);
    control->ShowWindow(SW_HIDE);
    control = GetDlgItem(IDC_STATIC_NAME_0);
    control->ShowWindow(SW_HIDE);
    control = GetDlgItem(IDC_STATIC_TELEPHONE_0);
    control->ShowWindow(SW_HIDE);
    control = GetDlgItem(IDC_STATIC_ADDRESS_0);
    control->ShowWindow(SW_HIDE);
    control = GetDlgItem(IDC_STATIC_TYPE_0);
    control->ShowWindow(SW_HIDE);
    control = GetDlgItem(IDC_STATIC_DELETE_0);
    control->ShowWindow(SW_HIDE);
    control = GetDlgItem(IDC_STATIC_CATEGORY_1);
    control->ShowWindow(SW_HIDE);
    control = GetDlgItem(IDC_STATIC_NAME_1);
    control->ShowWindow(SW_HIDE);
    control = GetDlgItem(IDC_STATIC_TELEPHONE_1);
    control->ShowWindow(SW_HIDE);
    control = GetDlgItem(IDC_STATIC_ADDRESS_1);
    control->ShowWindow(SW_HIDE);
    control = GetDlgItem(IDC_STATIC_TYPE_1);
    control->ShowWindow(SW_HIDE);
    control = GetDlgItem(IDC_STATIC_DELETE_1);
    control->ShowWindow(SW_HIDE);
    control = GetDlgItem(IDC_STATIC_CATEGORY_2);
    control->ShowWindow(SW_HIDE);
    control = GetDlgItem(IDC_STATIC_NAME_2);
    control->ShowWindow(SW_HIDE);
    control = GetDlgItem(IDC_STATIC_TELEPHONE_2);
    control->ShowWindow(SW_HIDE);
    control = GetDlgItem(IDC_STATIC_ADDRESS_2);
    control->ShowWindow(SW_HIDE);
    control = GetDlgItem(IDC_STATIC_TYPE_2);
    control->ShowWindow(SW_HIDE);
    control = GetDlgItem(IDC_STATIC_DELETE_2);
    control->ShowWindow(SW_HIDE);
    control = GetDlgItem(IDC_STATIC_CATEGORY_3);
    control->ShowWindow(SW_HIDE);
    control = GetDlgItem(IDC_STATIC_NAME_3);
    control->ShowWindow(SW_HIDE);
    control = GetDlgItem(IDC_STATIC_TELEPHONE_3);
    control->ShowWindow(SW_HIDE);
    control = GetDlgItem(IDC_STATIC_ADDRESS_3);
    control->ShowWindow(SW_HIDE);
    control = GetDlgItem(IDC_STATIC_TYPE_3);
    control->ShowWindow(SW_HIDE);
    control = GetDlgItem(IDC_STATIC_DELETE_3);
    control->ShowWindow(SW_HIDE);
    control = GetDlgItem(IDC_STATIC_CATEGORY_4);
    control->ShowWindow(SW_HIDE);
    control = GetDlgItem(IDC_STATIC_NAME_4);
    control->ShowWindow(SW_HIDE);
    control = GetDlgItem(IDC_STATIC_TELEPHONE_4);
    control->ShowWindow(SW_HIDE);
    control = GetDlgItem(IDC_STATIC_ADDRESS_4);
    control->ShowWindow(SW_HIDE);
    control = GetDlgItem(IDC_STATIC_TYPE_4);
    control->ShowWindow(SW_HIDE);
    control = GetDlgItem(IDC_STATIC_DELETE_4);
    control->ShowWindow(SW_HIDE);
    control = GetDlgItem(IDC_STATIC_CATEGORY_5);
    control->ShowWindow(SW_HIDE);
    control = GetDlgItem(IDC_STATIC_NAME_5);
    control->ShowWindow(SW_HIDE);
    control = GetDlgItem(IDC_STATIC_TELEPHONE_5);
    control->ShowWindow(SW_HIDE);
    control = GetDlgItem(IDC_STATIC_ADDRESS_5);
    control->ShowWindow(SW_HIDE);
    control = GetDlgItem(IDC_STATIC_TYPE_5);
    control->ShowWindow(SW_HIDE);
    control = GetDlgItem(IDC_STATIC_DELETE_5);
    control->ShowWindow(SW_HIDE);
    control = GetDlgItem(IDC_STATIC_CATEGORY_6);
    control->ShowWindow(SW_HIDE);
    control = GetDlgItem(IDC_STATIC_NAME_6);
    control->ShowWindow(SW_HIDE);
    control = GetDlgItem(IDC_STATIC_TELEPHONE_6);
    control->ShowWindow(SW_HIDE);
    control = GetDlgItem(IDC_STATIC_ADDRESS_6);
    control->ShowWindow(SW_HIDE);
    control = GetDlgItem(IDC_STATIC_TYPE_6);
    control->ShowWindow(SW_HIDE);
    control = GetDlgItem(IDC_STATIC_DELETE_6);
    control->ShowWindow(SW_HIDE);
    control = GetDlgItem(IDC_STATIC_CATEGORY_7);
    control->ShowWindow(SW_HIDE);
    control = GetDlgItem(IDC_STATIC_NAME_7);
    control->ShowWindow(SW_HIDE);
    control = GetDlgItem(IDC_STATIC_TELEPHONE_7);
    control->ShowWindow(SW_HIDE);
    control = GetDlgItem(IDC_STATIC_ADDRESS_7);
    control->ShowWindow(SW_HIDE);
    control = GetDlgItem(IDC_STATIC_TYPE_7);
    control->ShowWindow(SW_HIDE);
    control = GetDlgItem(IDC_STATIC_DELETE_7);
    control->ShowWindow(SW_HIDE);
    control = GetDlgItem(IDC_STATIC_CATEGORY_8);
    control->ShowWindow(SW_HIDE);
    control = GetDlgItem(IDC_STATIC_NAME_8);
    control->ShowWindow(SW_HIDE);
    control = GetDlgItem(IDC_STATIC_TELEPHONE_8);
    control->ShowWindow(SW_HIDE);
    control = GetDlgItem(IDC_STATIC_ADDRESS_8);
    control->ShowWindow(SW_HIDE);
    control = GetDlgItem(IDC_STATIC_TYPE_8);
    control->ShowWindow(SW_HIDE);
    control = GetDlgItem(IDC_STATIC_DELETE_8);
    control->ShowWindow(SW_HIDE);
    control = GetDlgItem(IDC_STATIC_CATEGORY_9);
    control->ShowWindow(SW_HIDE);
    control = GetDlgItem(IDC_STATIC_NAME_9);
    control->ShowWindow(SW_HIDE);
    control = GetDlgItem(IDC_STATIC_TELEPHONE_9);
    control->ShowWindow(SW_HIDE);
    control = GetDlgItem(IDC_STATIC_ADDRESS_9);
    control->ShowWindow(SW_HIDE);
    control = GetDlgItem(IDC_STATIC_TYPE_9);
    control->ShowWindow(SW_HIDE);
    control = GetDlgItem(IDC_STATIC_DELETE_9);
    control->ShowWindow(SW_HIDE);
    size_t count = contacts_.size();
    size_t current = 0;
    if (current < count) {
        control = GetDlgItem(IDC_STATIC_CATEGORY_0);
        std::wstring categoryName = contacts_[current]->GetCategoryName();
        if (categoryName != L"") {
            control->SetWindowTextW(categoryName.c_str());
            control->ShowWindow(SW_SHOW);
        } else {
            control->SetWindowTextW(L"No Category");
        }
        control = GetDlgItem(IDC_STATIC_NAME_0);
        control->SetWindowTextW(contacts_[current]->name().c_str());
        control->ShowWindow(SW_SHOW);
        control = GetDlgItem(IDC_STATIC_TELEPHONE_0);
        control->SetWindowTextW(contacts_[current]->telephone().c_str());
        control->ShowWindow(SW_SHOW);
        control = GetDlgItem(IDC_STATIC_ADDRESS_0);
        control->SetWindowTextW(contacts_[current]->address().c_str());
        control->ShowWindow(SW_SHOW);
        control = GetDlgItem(IDC_STATIC_TYPE_0);
        control->SetWindowTextW(ContactTypeToString(contacts_[current]->type()).c_str());
        control->ShowWindow(SW_SHOW);
        control = GetDlgItem(IDC_STATIC_DELETE_0);
        control->ShowWindow(SW_SHOW);
        ++current;
    }
    if (current < count) {
        control = GetDlgItem(IDC_STATIC_CATEGORY_1);
        std::wstring categoryName = contacts_[current]->GetCategoryName();
        if (categoryName != L"") {
            control->SetWindowTextW(categoryName.c_str());
            control->ShowWindow(SW_SHOW);
        } else {
            control->SetWindowTextW(L"No Category");
        }
        control = GetDlgItem(IDC_STATIC_NAME_1);
        control->SetWindowTextW(contacts_[current]->name().c_str());
        control->ShowWindow(SW_SHOW);
        control = GetDlgItem(IDC_STATIC_TELEPHONE_1);
        control->SetWindowTextW(contacts_[current]->telephone().c_str());
        control->ShowWindow(SW_SHOW);
        control = GetDlgItem(IDC_STATIC_ADDRESS_1);
        control->SetWindowTextW(contacts_[current]->address().c_str());
        control->ShowWindow(SW_SHOW);
        control = GetDlgItem(IDC_STATIC_TYPE_1);
        control->SetWindowTextW(ContactTypeToString(contacts_[current]->type()).c_str());
        control->ShowWindow(SW_SHOW);
        control = GetDlgItem(IDC_STATIC_DELETE_1);
        control->ShowWindow(SW_SHOW);
        ++current;
    }
    if (current < count) {
        control = GetDlgItem(IDC_STATIC_CATEGORY_2);
        std::wstring categoryName = contacts_[current]->GetCategoryName();
        if (categoryName != L"") {
            control->SetWindowTextW(categoryName.c_str());
            control->ShowWindow(SW_SHOW);
        } else {
            control->SetWindowTextW(L"No Category");
        }
        control = GetDlgItem(IDC_STATIC_NAME_2);
        control->SetWindowTextW(contacts_[current]->name().c_str());
        control->ShowWindow(SW_SHOW);
        control = GetDlgItem(IDC_STATIC_TELEPHONE_2);
        control->SetWindowTextW(contacts_[current]->telephone().c_str());
        control->ShowWindow(SW_SHOW);
        control = GetDlgItem(IDC_STATIC_ADDRESS_2);
        control->SetWindowTextW(contacts_[current]->address().c_str());
        control->ShowWindow(SW_SHOW);
        control = GetDlgItem(IDC_STATIC_TYPE_2);
        control->SetWindowTextW(ContactTypeToString(contacts_[current]->type()).c_str());
        control->ShowWindow(SW_SHOW);
        control = GetDlgItem(IDC_STATIC_DELETE_2);
        control->ShowWindow(SW_SHOW);
        ++current;
    }
    if (current < count) {
        control = GetDlgItem(IDC_STATIC_CATEGORY_3);
        std::wstring categoryName = contacts_[current]->GetCategoryName();
        if (categoryName != L"") {
            control->SetWindowTextW(categoryName.c_str());
            control->ShowWindow(SW_SHOW);
        } else {
            control->SetWindowTextW(L"No Category");
        }
        control = GetDlgItem(IDC_STATIC_NAME_3);
        control->SetWindowTextW(contacts_[current]->name().c_str());
        control->ShowWindow(SW_SHOW);
        control = GetDlgItem(IDC_STATIC_TELEPHONE_3);
        control->SetWindowTextW(contacts_[current]->telephone().c_str());
        control->ShowWindow(SW_SHOW);
        control = GetDlgItem(IDC_STATIC_ADDRESS_3);
        control->SetWindowTextW(contacts_[current]->address().c_str());
        control->ShowWindow(SW_SHOW);
        control = GetDlgItem(IDC_STATIC_TYPE_3);
        control->SetWindowTextW(ContactTypeToString(contacts_[current]->type()).c_str());
        control->ShowWindow(SW_SHOW);
        control = GetDlgItem(IDC_STATIC_DELETE_3);
        control->ShowWindow(SW_SHOW);
        ++current;
    }
    if (current < count) {
        control = GetDlgItem(IDC_STATIC_CATEGORY_4);
        std::wstring categoryName = contacts_[current]->GetCategoryName();
        if (categoryName != L"") {
            control->SetWindowTextW(categoryName.c_str());
            control->ShowWindow(SW_SHOW);
        } else {
            control->SetWindowTextW(L"No Category");
        }
        control = GetDlgItem(IDC_STATIC_NAME_4);
        control->SetWindowTextW(contacts_[current]->name().c_str());
        control->ShowWindow(SW_SHOW);
        control = GetDlgItem(IDC_STATIC_TELEPHONE_4);
        control->SetWindowTextW(contacts_[current]->telephone().c_str());
        control->ShowWindow(SW_SHOW);
        control = GetDlgItem(IDC_STATIC_ADDRESS_4);
        control->SetWindowTextW(contacts_[current]->address().c_str());
        control->ShowWindow(SW_SHOW);
        control = GetDlgItem(IDC_STATIC_TYPE_4);
        control->SetWindowTextW(ContactTypeToString(contacts_[current]->type()).c_str());
        control->ShowWindow(SW_SHOW);
        control = GetDlgItem(IDC_STATIC_DELETE_4);
        control->ShowWindow(SW_SHOW);
        ++current;
    }
    if (current < count) {
        control = GetDlgItem(IDC_STATIC_CATEGORY_5);
        std::wstring categoryName = contacts_[current]->GetCategoryName();
        if (categoryName != L"") {
            control->SetWindowTextW(categoryName.c_str());
            control->ShowWindow(SW_SHOW);
        } else {
            control->SetWindowTextW(L"No Category");
        }
        control = GetDlgItem(IDC_STATIC_NAME_5);
        control->SetWindowTextW(contacts_[current]->name().c_str());
        control->ShowWindow(SW_SHOW);
        control = GetDlgItem(IDC_STATIC_TELEPHONE_5);
        control->SetWindowTextW(contacts_[current]->telephone().c_str());
        control->ShowWindow(SW_SHOW);
        control = GetDlgItem(IDC_STATIC_ADDRESS_5);
        control->SetWindowTextW(contacts_[current]->address().c_str());
        control->ShowWindow(SW_SHOW);
        control = GetDlgItem(IDC_STATIC_TYPE_5);
        control->SetWindowTextW(ContactTypeToString(contacts_[current]->type()).c_str());
        control->ShowWindow(SW_SHOW);
        control = GetDlgItem(IDC_STATIC_DELETE_5);
        control->ShowWindow(SW_SHOW);
        ++current;
    }
    if (current < count) {
        control = GetDlgItem(IDC_STATIC_CATEGORY_6);
        std::wstring categoryName = contacts_[current]->GetCategoryName();
        if (categoryName != L"") {
            control->SetWindowTextW(categoryName.c_str());
            control->ShowWindow(SW_SHOW);
        } else {
            control->SetWindowTextW(L"No Category");
        }
        control = GetDlgItem(IDC_STATIC_NAME_6);
        control->SetWindowTextW(contacts_[current]->name().c_str());
        control->ShowWindow(SW_SHOW);
        control = GetDlgItem(IDC_STATIC_TELEPHONE_6);
        control->SetWindowTextW(contacts_[current]->telephone().c_str());
        control->ShowWindow(SW_SHOW);
        control = GetDlgItem(IDC_STATIC_ADDRESS_6);
        control->SetWindowTextW(contacts_[current]->address().c_str());
        control->ShowWindow(SW_SHOW);
        control = GetDlgItem(IDC_STATIC_TYPE_6);
        control->SetWindowTextW(ContactTypeToString(contacts_[current]->type()).c_str());
        control->ShowWindow(SW_SHOW);
        control = GetDlgItem(IDC_STATIC_DELETE_6);
        control->ShowWindow(SW_SHOW);
        ++current;
    }
    if (current < count) {
        control = GetDlgItem(IDC_STATIC_CATEGORY_7);
        std::wstring categoryName = contacts_[current]->GetCategoryName();
        if (categoryName != L"") {
            control->SetWindowTextW(categoryName.c_str());
            control->ShowWindow(SW_SHOW);
        } else {
            control->SetWindowTextW(L"No Category");
        }
        control = GetDlgItem(IDC_STATIC_NAME_7);
        control->SetWindowTextW(contacts_[current]->name().c_str());
        control->ShowWindow(SW_SHOW);
        control = GetDlgItem(IDC_STATIC_TELEPHONE_7);
        control->SetWindowTextW(contacts_[current]->telephone().c_str());
        control->ShowWindow(SW_SHOW);
        control = GetDlgItem(IDC_STATIC_ADDRESS_7);
        control->SetWindowTextW(contacts_[current]->address().c_str());
        control->ShowWindow(SW_SHOW);
        control = GetDlgItem(IDC_STATIC_TYPE_7);
        control->SetWindowTextW(ContactTypeToString(contacts_[current]->type()).c_str());
        control->ShowWindow(SW_SHOW);
        control = GetDlgItem(IDC_STATIC_DELETE_7);
        control->ShowWindow(SW_SHOW);
        ++current;
    }
    if (current < count) {
        control = GetDlgItem(IDC_STATIC_CATEGORY_8);
        std::wstring categoryName = contacts_[current]->GetCategoryName();
        if (categoryName != L"") {
            control->SetWindowTextW(categoryName.c_str());
            control->ShowWindow(SW_SHOW);
        } else {
            control->SetWindowTextW(L"No Category");
        }
        control = GetDlgItem(IDC_STATIC_NAME_8);
        control->SetWindowTextW(contacts_[current]->name().c_str());
        control->ShowWindow(SW_SHOW);
        control = GetDlgItem(IDC_STATIC_TELEPHONE_8);
        control->SetWindowTextW(contacts_[current]->telephone().c_str());
        control->ShowWindow(SW_SHOW);
        control = GetDlgItem(IDC_STATIC_ADDRESS_8);
        control->SetWindowTextW(contacts_[current]->address().c_str());
        control->ShowWindow(SW_SHOW);
        control = GetDlgItem(IDC_STATIC_TYPE_8);
        control->SetWindowTextW(ContactTypeToString(contacts_[current]->type()).c_str());
        control->ShowWindow(SW_SHOW);
        control = GetDlgItem(IDC_STATIC_DELETE_8);
        control->ShowWindow(SW_SHOW);
        ++current;
    }
    if (current < count) {
        control = GetDlgItem(IDC_STATIC_CATEGORY_9);
        std::wstring categoryName = contacts_[current]->GetCategoryName();
        if (categoryName != L"") {
            control->SetWindowTextW(categoryName.c_str());
            control->ShowWindow(SW_SHOW);
        } else {
            control->SetWindowTextW(L"No Category");
        }
        control = GetDlgItem(IDC_STATIC_NAME_9);
        control->SetWindowTextW(contacts_[current]->name().c_str());
        control->ShowWindow(SW_SHOW);
        control = GetDlgItem(IDC_STATIC_TELEPHONE_9);
        control->SetWindowTextW(contacts_[current]->telephone().c_str());
        control->ShowWindow(SW_SHOW);
        control = GetDlgItem(IDC_STATIC_ADDRESS_9);
        control->SetWindowTextW(contacts_[current]->address().c_str());
        control->ShowWindow(SW_SHOW);
        control = GetDlgItem(IDC_STATIC_TYPE_9);
        control->SetWindowTextW(ContactTypeToString(contacts_[current]->type()).c_str());
        control->ShowWindow(SW_SHOW);
        control = GetDlgItem(IDC_STATIC_DELETE_9);
        control->ShowWindow(SW_SHOW);
        ++current;
    }
}

void ContactsDialog::drawContact_(Util::shared_ptr<Contact> const& contact, int const index) {
    //MessageBox(L"Draw Contact Item");
}

void ContactsDialog::OnBnClickedButtonNewContact() {
    // TODO: Add your control notification handler code here
    isNewContact_ = true;
    Util::shared_ptr<Contact> newContact(new Contact());
    contactForm->SetListener(this);
    contactForm->SetContact(newContact);
    contactForm->ShowWindow(SW_SHOW);
    filter_ = L""; //clear search condition
    CWnd* control = GetDlgItem(IDC_EDIT_NAME);
    control->SetWindowTextW(L"");
    control = GetDlgItem(IDC_EDIT_TELEPHONE);
    control->SetWindowTextW(L"");
    control = GetDlgItem(IDC_COMBO_CATEGORY);
    control->SetWindowTextW(L"All");
    control = GetDlgItem(IDC_COMBO_TYPE);
    control->SetWindowTextW(L"All");
}

BOOL ContactsDialog::OnInitDialog() {
    CDialog::OnInitDialog();
    // TODO:  Add extra initialization here
    CComboBox* control = static_cast<CComboBox*>(GetDlgItem(IDC_COMBO_CATEGORY));
    control->AddString(L"All");
    std::vector<Util::shared_ptr<ContactCategory> > categories = ContactCategory::All();
    for (size_t i = 0; i < categories.size(); ++i) {
        control->AddString(categories[i]->name().c_str());
    }
    control->SetWindowTextW(L"All");

    control = static_cast<CComboBox*>(GetDlgItem(IDC_COMBO_TYPE));
    control->AddString(L"All");
    control->AddString(L"Normal");
    control->AddString(L"VIP");
    control->AddString(L"Blacklist");
    control->SetWindowTextW(L"All");

	SIZE buttonSize;
	buttonSize.cx = 48;
	buttonSize.cy = 48;
	std::wstring maskFilename = L"/FlashDrv/debug/buttonMask.bmp";
	closeButton_.SetImage(L"/FlashDrv/debug/close.jpg", buttonSize, maskFilename);
	closeButton_.SubclassDlgItem(IDC_BUTTON_CLOSE, this);
	pageDownButton_.SetImage(L"/FlashDrv/debug/pagedown.jpg", buttonSize, maskFilename);
	pageDownButton_.SubclassDlgItem(IDC_BUTTON_PAGE_DOWN, this);
	pageUpButton_.SetImage(L"/FlashDrv/debug/pageup.jpg", buttonSize, maskFilename);
	pageUpButton_.SubclassDlgItem(IDC_BUTTON_PAGE_UP, this);
	clearButton_.SetImage(L"/FlashDrv/debug/clear.jpg", buttonSize, maskFilename);
	clearButton_.SubclassDlgItem(IDC_BUTTON_CLEAR, this);
	newContactButton_.SetImage(L"/FlashDrv/debug/newContact.jpg", buttonSize, maskFilename);
	newContactButton_.SubclassDlgItem(IDC_BUTTON_NEW_CONTACT, this);
	newCategoryButton_.SetImage(L"/FlashDrv/debug/newCategory.jpg", buttonSize, maskFilename);
	newCategoryButton_.SubclassDlgItem(IDC_BUTTON_NEW_CATEGORY, this);
	callButton_.SetImage(L"/FlashDrv/debug/call.jpg", buttonSize, maskFilename);
	callButton_.SubclassDlgItem(IDC_BUTTON_CALL, this);
	voiceMessagesButton_.SetImage(L"/FlashDrv/debug/message.jpg", buttonSize, maskFilename);
	voiceMessagesButton_.SubclassDlgItem(IDC_BUTTON_VOICE_MESSAGES, this);
	findButton_.SetImage(L"/FlashDrv/debug/find.jpg", buttonSize, maskFilename);
	findButton_.SubclassDlgItem(IDC_BUTTON_FIND, this);

	//font_.CreateFont(20, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"ËÎÌו");
	font_.CreatePointFont(Screen::fontPoint, L"ËÎÌו");
	GetDlgItem(IDC_EDIT_NAME)->SetFont(&font_, FALSE);
	GetDlgItem(IDC_EDIT_TELEPHONE)->SetFont(&font_, FALSE);
	GetDlgItem(IDC_COMBO_CATEGORY)->SetFont(&font_, FALSE);
	GetDlgItem(IDC_COMBO_TYPE)->SetFont(&font_, FALSE);

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

LRESULT ContactsDialog::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) {
    // TODO: Add your specialized code here and/or call the base class
    Util::shared_ptr<Contact> contact;
    std::vector<Util::shared_ptr<Contact> > contacts;
    Util::shared_ptr<ContactCategory> contactCategory;
    CWnd* control = 0;
    switch (message) {
        case UM_SHOW_TELEPHONE:
            control = GetDlgItem(IDC_BUTTON_CALL);
            control->ShowWindow(SW_SHOW);
            break;
        case UM_CONTACT_UPDATE_FINALLY:
            contactForm->SyncContact();
            contact = contactForm->GetContact();
            if (isNewContact_) {
                contact->Insert();
            } else {
                contact->Update();
            }
            contacts = Contact::Select(filter_, L"id", Util::BaseData::srDesc, offset_, pageSize);
            SetContacts(contacts);
            break;
        case UM_CATEGORY_UPDATE_FINALLY:
            categoryForm->SyncCategory();
            contactCategory = categoryForm->GetCategory();
            if (isNewCategory_) {
                contactCategory->Insert();
                ContactCategory::All().push_back(contactCategory);
            } else {
                contactCategory->Update();
            }
            refreshList_();
            break;
        case UM_CATEGORY_DELETE_FINALLY:
            refreshList_();
            break;
        default:
            break;
    }
    return CDialog::WindowProc(message, wParam, lParam);
}

void ContactsDialog::refreshList_() {
    //redraw contact list
}

void ContactsDialog::OnBnClickedButtonNewCategory() {
    // TODO: Add your control notification handler code here
    isNewCategory_ = true;
    Util::shared_ptr<ContactCategory> newContactCategory(new ContactCategory());
    categoryForm->SetCategory(newContactCategory);
    categoryForm->ShowWindow(SW_SHOW);
}

void ContactsDialog::OnBnClickedButtonClear() {
    //if (you are sure)
    Contact::Remove(L"");
    std::vector<Util::shared_ptr<Contact> > newContacts;
    SetContacts(newContacts);
}

void ContactsDialog::OnBnClickedButtonPageUp() {
    offset_ -= pageSize;
    if (offset_ < 0) {
        offset_ = 0;
    }
    std::vector<Util::shared_ptr<Contact> > newContacts = Contact::Select(filter_, L"id", Util::BaseData::srDesc, offset_, pageSize);
    SetContacts(newContacts);
}

void ContactsDialog::OnBnClickedButtonPageDown() {
    size_t maxRecord = Contact::GetDataCount(L"");
    offset_ += pageSize;
    if (offset_ > maxRecord) {
        offset_ = maxRecord / pageSize * pageSize;
    }
    std::vector<Util::shared_ptr<Contact> > newContacts = Contact::Select(filter_, L"id", Util::BaseData::srDesc, offset_, pageSize);
    SetContacts(newContacts);
}

void ContactsDialog::CategoryEdit(int const controlId, int const no) {
    isNewCategory_ = false;
    CWnd* control = GetDlgItem(controlId);
    CString value;
    control->GetWindowTextW(value);
    if (value != L"No Category") {
        categoryForm->SetCategory(Util::shared_ptr<ContactCategory>(const_cast<ContactCategory*>(contacts_[no]->category())));
        categoryForm->SetListener(this);
        categoryForm->ShowWindow(SW_SHOW);
    }
}

void ContactsDialog::Delete(int const no) {
    contacts_[no]->Remove();
    std::vector<Util::shared_ptr<Contact> > newContacts = Contact::Select(filter_, L"id", Util::BaseData::srDesc, offset_, pageSize);
    SetContacts(newContacts);
}

void ContactsDialog::ShowDetail(int const no) {
    isNewContact_ = false;
    contactForm->SetContact(contacts_[no]);
    contactForm->SetListener(this);
    contactForm->ShowWindow(SW_SHOW);
}

void ContactsDialog::OnStnClickedStaticCategory0() {
    CategoryEdit(IDC_STATIC_CATEGORY_0, 0);
}

void ContactsDialog::OnStnClickedStaticDelete0() {
    Delete(0);
}

void ContactsDialog::OnStnClickedStaticName0() {
    ShowDetail(0);
}

void ContactsDialog::OnStnClickedStaticTelephone0() {
    ShowDetail(0);
}

void ContactsDialog::OnStnClickedStaticAddress0() {
    ShowDetail(0);
}

void ContactsDialog::OnStnClickedStaticType0() {
    ShowDetail(0);
}

void ContactsDialog::OnStnClickedStaticCategory1() {
    CategoryEdit(IDC_STATIC_CATEGORY_1, 1);
}

void ContactsDialog::OnStnClickedStaticDelete1() {
    Delete(1);
}

void ContactsDialog::OnStnClickedStaticName1() {
    ShowDetail(1);
}

void ContactsDialog::OnStnClickedStaticTelephone1() {
    ShowDetail(1);
}

void ContactsDialog::OnStnClickedStaticAddress1() {
    ShowDetail(1);
}

void ContactsDialog::OnStnClickedStaticType1() {
    ShowDetail(1);
}

void ContactsDialog::OnStnClickedStaticCategory2() {
    CategoryEdit(IDC_STATIC_CATEGORY_2, 2);
}

void ContactsDialog::OnStnClickedStaticDelete2() {
    Delete(2);
}

void ContactsDialog::OnStnClickedStaticName2() {
    ShowDetail(2);
}

void ContactsDialog::OnStnClickedStaticTelephone2() {
    ShowDetail(2);
}

void ContactsDialog::OnStnClickedStaticAddress2() {
    ShowDetail(2);
}

void ContactsDialog::OnStnClickedStaticType2() {
    ShowDetail(2);
}

void ContactsDialog::OnStnClickedStaticCategory3() {
    CategoryEdit(IDC_STATIC_CATEGORY_3, 3);
}

void ContactsDialog::OnStnClickedStaticDelete3() {
    Delete(3);
}

void ContactsDialog::OnStnClickedStaticName3() {
    ShowDetail(3);
}

void ContactsDialog::OnStnClickedStaticTelephone3() {
    ShowDetail(3);
}

void ContactsDialog::OnStnClickedStaticAddress3() {
    ShowDetail(3);
}

void ContactsDialog::OnStnClickedStaticType3() {
    ShowDetail(3);
}

void ContactsDialog::OnStnClickedStaticCategory4() {
    CategoryEdit(IDC_STATIC_CATEGORY_4, 4);
}

void ContactsDialog::OnStnClickedStaticDelete4() {
    Delete(4);
}

void ContactsDialog::OnStnClickedStaticName4() {
    ShowDetail(4);
}

void ContactsDialog::OnStnClickedStaticTelephone4() {
    ShowDetail(4);
}

void ContactsDialog::OnStnClickedStaticAddress4() {
    ShowDetail(4);
}

void ContactsDialog::OnStnClickedStaticType4() {
    ShowDetail(4);
}

void ContactsDialog::OnStnClickedStaticCategory5() {
    CategoryEdit(IDC_STATIC_CATEGORY_5, 5);
}

void ContactsDialog::OnStnClickedStaticDelete5() {
    Delete(5);
}

void ContactsDialog::OnStnClickedStaticName5() {
    ShowDetail(5);
}

void ContactsDialog::OnStnClickedStaticTelephone5() {
    ShowDetail(5);
}

void ContactsDialog::OnStnClickedStaticAddress5() {
    ShowDetail(5);
}

void ContactsDialog::OnStnClickedStaticType5() {
    ShowDetail(5);
}

void ContactsDialog::OnStnClickedStaticCategory6() {
    CategoryEdit(IDC_STATIC_CATEGORY_6, 6);
}

void ContactsDialog::OnStnClickedStaticDelete6() {
    Delete(6);
}

void ContactsDialog::OnStnClickedStaticName6() {
    ShowDetail(6);
}

void ContactsDialog::OnStnClickedStaticTelephone6() {
    ShowDetail(6);
}

void ContactsDialog::OnStnClickedStaticAddress6() {
    ShowDetail(6);
}

void ContactsDialog::OnStnClickedStaticType6() {
    ShowDetail(6);
}

void ContactsDialog::OnStnClickedStaticCategory7() {
    CategoryEdit(IDC_STATIC_CATEGORY_7, 7);
}

void ContactsDialog::OnStnClickedStaticDelete7() {
    Delete(7);
}

void ContactsDialog::OnStnClickedStaticName7() {
    ShowDetail(7);
}

void ContactsDialog::OnStnClickedStaticTelephone7() {
    ShowDetail(7);
}

void ContactsDialog::OnStnClickedStaticAddress7() {
    ShowDetail(7);
}

void ContactsDialog::OnStnClickedStaticType7() {
    ShowDetail(7);
}

void ContactsDialog::OnStnClickedStaticCategory8() {
    CategoryEdit(IDC_STATIC_CATEGORY_8, 8);
}

void ContactsDialog::OnStnClickedStaticDelete8() {
    Delete(8);
}

void ContactsDialog::OnStnClickedStaticName8() {
    ShowDetail(8);
}

void ContactsDialog::OnStnClickedStaticTelephone8() {
    ShowDetail(8);
}

void ContactsDialog::OnStnClickedStaticAddress8() {
    ShowDetail(8);
}

void ContactsDialog::OnStnClickedStaticType8() {
    ShowDetail(8);
}

void ContactsDialog::OnStnClickedStaticCategory9() {
    CategoryEdit(IDC_STATIC_CATEGORY_9, 9);
}

void ContactsDialog::OnStnClickedStaticDelete9() {
    Delete(9);
}

void ContactsDialog::OnStnClickedStaticName9() {
    ShowDetail(9);
}

void ContactsDialog::OnStnClickedStaticTelephone9() {
    ShowDetail(9);
}

void ContactsDialog::OnStnClickedStaticAddress9() {
    ShowDetail(9);
}

void ContactsDialog::OnStnClickedStaticType9() {
    ShowDetail(9);
}

HBRUSH ContactsDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) {
    HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	RECT bounds;
	pWnd->GetWindowRect(&bounds);
	std::wstring backgroundImageFilename = L"/FlashDrv/debug/NULL.JPG";
	SIZE backgroundImageSize;
	backgroundImageSize.cx = 800;
	backgroundImageSize.cy = 480;
	CFont font;
	//font.CreateFont(height, width, escapement, orientation, weight, italic, underline, strikerOut, charset, outPrecision, clipPrecision, quality, pitchAndFamily, faceName);
	font.CreateFont(20, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial");
    switch (nCtlColor) {
        case CTLCOLOR_BTN:
            break;
        case CTLCOLOR_STATIC:
			hbr = BackgroundImage::setStaticProperties(pDC->m_hDC, bounds, Util::ImageOp::GetImage(backgroundImageFilename, backgroundImageSize), font, RGB(255, 255, 255));
            break;
        case CTLCOLOR_EDIT:
			//hbr = BackgroundImage::setStaticProperties(pDC->m_hDC, bounds, Util::ImageOp::GetImage(backgroundImageFilename, backgroundImageSize), font, RGB(255, 0, 0));
            break;
        //case CTLCOLOR_SCROLLBAR:
        //    break;
        case CTLCOLOR_LISTBOX:
            break;
        case CTLCOLOR_MSGBOX:
            break;
        //case CTLCOLOR_DLG:
        //    break;
        default:
            break;
    }

    // TODO:  Return a different brush if the default is not desired
    return hbr;
}

void ContactsDialog::OnBnClickedButtonCall() {
    callForm->SetListener(this);
    callForm->ShowWindow(SW_SHOW);
}

void ContactsDialog::OnBnClickedButtonVoiceMessages() {
    std::vector<Util::shared_ptr<CallInfo> > callInfos = CallInfo::GetAllNotPlayedLeaveWord();
    if (!callInfos.empty()) {
        playingRecordsForm->SetCallInfos(callInfos);
        playingRecordsForm->ShowWindow(SW_SHOW);
    }
}

void ContactsDialog::OnBnClickedButtonFind() {
    CString filter;
    CWnd* control = GetDlgItem(IDC_EDIT_NAME);
    CString value;
    control->GetWindowTextW(value);
    if (value != L"") {
        filter += L"[name] LIKE '" + value + L"%'";
    }
    control = GetDlgItem(IDC_EDIT_TELEPHONE);
    control->GetWindowTextW(value);
    if (value != L"") {
        if (filter != L"") {
            filter += " AND ";
        }
        filter += L"([telephone] LIKE '" + value + L"%'";
        filter += L" OR [telephone2] LIKE '" + value + L"%'";
        filter += L" OR [mobile] LIKE '" + value + L"%'";
        filter += L" OR [mobile2] LIKE '" + value + L"%')";
    }
    control = GetDlgItem(IDC_COMBO_CATEGORY);
    control->GetWindowTextW(value);
    if (value != L"All") {
        if (filter != L"") {
            filter += " AND ";
        }
        filter += L"[categoryId] = " + ContactCategory::GetIdByName(static_cast<LPCTSTR>(value));
    }
    control = GetDlgItem(IDC_COMBO_TYPE);
    control->GetWindowTextW(value);
    if (value != L"All") {
        if (filter != L"") {
            filter += " AND ";
        }
        filter += L"[type] = ";
        filter += Util::StringOp::FromInt(StringToContactType(static_cast<LPCTSTR>(value))).c_str();
    }
    filter_ = static_cast<LPCTSTR>(filter);
    std::vector<Util::shared_ptr<Contact> > contacts = Contact::Select(filter_, L"id", Util::BaseData::srDesc, 0, pageSize);
    SetContacts(contacts);
}

void ContactsDialog::OnEnSetfocusEditName() {
    SipShowIM(SIPF_ON);
}

void ContactsDialog::OnEnKillfocusEditName() {
    SipShowIM(SIPF_OFF);
}

void ContactsDialog::OnEnSetfocusEditTelephone() {
    SipShowIM(SIPF_ON);
}

void ContactsDialog::OnEnKillfocusEditTelephone() {
    SipShowIM(SIPF_OFF);
}

BOOL ContactsDialog::OnEraseBkgnd(CDC* pDC) {
	BOOL r = BackgroundImage::drawBackground(m_hWnd);
	if (!r) {
        DWORD errorCode = GetLastError();
        DWORD ec = errorCode;
	}
	//return CDialog::OnEraseBkgnd(pDC);
	return TRUE;
}
