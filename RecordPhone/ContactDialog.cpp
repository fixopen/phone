// ContactDialog.cpp : implementation file
//

#include "stdafx.h"

#include <sip.h>

#include "RecordPhone.h"
#include "ContactDialog.h"
#include "CallDialog.h"
#include "PlayingRecordsDialog.h"
#include "Telephone.h"
#include "BackgroundImage.h"
#include "Util/ImageOp.h"

ContactDialog* contactForm = 0;

// ContactDialog dialog

IMPLEMENT_DYNAMIC(ContactDialog, CDialog)

ContactDialog::ContactDialog(CWnd* pParent /*=NULL*/)
: CDialog(ContactDialog::IDD, pParent) {
}

ContactDialog::~ContactDialog() {
}

void ContactDialog::DoDataExchange(CDataExchange* pDX) {
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(ContactDialog, CDialog)
    ON_BN_CLICKED(IDC_BUTTON_CLOSE, &ContactDialog::OnBnClickedButtonClose)
    ON_BN_CLICKED(IDC_BUTTON_CALL_PHONE, &ContactDialog::OnBnClickedButtonCallPhone)
    ON_BN_CLICKED(IDC_BUTTON_CALL_PHONE2, &ContactDialog::OnBnClickedButtonCallPhone2)
    ON_BN_CLICKED(IDC_BUTTON_CALL_MOBILE, &ContactDialog::OnBnClickedButtonCallMobile)
    ON_BN_CLICKED(IDC_BUTTON_CALL_MOBILE2, &ContactDialog::OnBnClickedButtonCallMobile2)
    ON_WM_CTLCOLOR()
    ON_BN_CLICKED(IDC_BUTTON_CALL, &ContactDialog::OnBnClickedButtonCall)
    ON_BN_CLICKED(IDC_BUTTON_VOICE_MESSAGES, &ContactDialog::OnBnClickedButtonVoiceMessages)
    ON_WM_SHOWWINDOW()
    ON_EN_SETFOCUS(IDC_EDIT_NAME, &ContactDialog::OnEnSetfocusEditName)
    ON_EN_KILLFOCUS(IDC_EDIT_NAME, &ContactDialog::OnEnKillfocusEditName)
    ON_EN_SETFOCUS(IDC_EDIT_PHONE, &ContactDialog::OnEnSetfocusEditPhone)
    ON_EN_KILLFOCUS(IDC_EDIT_PHONE, &ContactDialog::OnEnKillfocusEditPhone)
    ON_EN_SETFOCUS(IDC_EDIT_PHONE2, &ContactDialog::OnEnSetfocusEditPhone2)
    ON_EN_KILLFOCUS(IDC_EDIT_PHONE2, &ContactDialog::OnEnKillfocusEditPhone2)
    ON_EN_SETFOCUS(IDC_EDIT_MOBILE, &ContactDialog::OnEnSetfocusEditMobile)
    ON_EN_KILLFOCUS(IDC_EDIT_MOBILE, &ContactDialog::OnEnKillfocusEditMobile)
    ON_EN_SETFOCUS(IDC_EDIT_MOBILE2, &ContactDialog::OnEnSetfocusEditMobile2)
    ON_EN_KILLFOCUS(IDC_EDIT_MOBILE2, &ContactDialog::OnEnKillfocusEditMobile2)
    ON_EN_SETFOCUS(IDC_EDIT_EMAIL, &ContactDialog::OnEnSetfocusEditEmail)
    ON_EN_KILLFOCUS(IDC_EDIT_EMAIL, &ContactDialog::OnEnKillfocusEditEmail)
    ON_EN_SETFOCUS(IDC_EDIT_ADDRESS, &ContactDialog::OnEnSetfocusEditAddress)
    ON_EN_KILLFOCUS(IDC_EDIT_ADDRESS, &ContactDialog::OnEnKillfocusEditAddress)
    ON_EN_SETFOCUS(IDC_EDIT_COMPANY, &ContactDialog::OnEnSetfocusEditCompany)
    ON_EN_KILLFOCUS(IDC_EDIT_COMPANY, &ContactDialog::OnEnKillfocusEditCompany)
    ON_EN_SETFOCUS(IDC_EDIT_DEPARTMENT, &ContactDialog::OnEnSetfocusEditDepartment)
    ON_EN_KILLFOCUS(IDC_EDIT_DEPARTMENT, &ContactDialog::OnEnKillfocusEditDepartment)
    ON_EN_SETFOCUS(IDC_EDIT_DUTY, &ContactDialog::OnEnSetfocusEditDuty)
    ON_EN_KILLFOCUS(IDC_EDIT_DUTY, &ContactDialog::OnEnKillfocusEditDuty)
    ON_EN_SETFOCUS(IDC_EDIT_REMARKS, &ContactDialog::OnEnSetfocusEditRemarks)
    ON_EN_KILLFOCUS(IDC_EDIT_REMARKS, &ContactDialog::OnEnKillfocusEditRemarks)
	ON_WM_ERASEBKGND()
    ON_CBN_SELCHANGE(IDC_COMBO_CATEGORY, &ContactDialog::OnCbnSelchangeComboCategory)
END_MESSAGE_MAP()


// ContactDialog message handlers

void ContactDialog::SetContact(Util::shared_ptr<Contact> const& contact) {
    CComboBox* categoriesControl = static_cast<CComboBox*>(GetDlgItem(IDC_COMBO_CATEGORY));
	categoriesControl->Clear();
    std::vector<Util::shared_ptr<ContactCategory> > categories = ContactCategory::All();
    for (size_t i = 0; i < categories.size(); ++i) {
        categoriesControl->AddString(categories[i]->name().c_str());
    }
    categoriesControl->SetWindowTextW(L"");

    contact_ = contact;
    CWnd* control = GetDlgItem(IDC_EDIT_NAME);
    control->SetWindowTextW(contact_->name().c_str());

    control = GetDlgItem(IDC_EDIT_PHONE);
    control->SetWindowTextW(contact_->telephone().c_str());

    control = GetDlgItem(IDC_EDIT_PHONE2);
    control->SetWindowTextW(contact_->telephone2().c_str());

    control = GetDlgItem(IDC_EDIT_MOBILE);
    control->SetWindowTextW(contact_->mobile().c_str());

    control = GetDlgItem(IDC_EDIT_MOBILE2);
    control->SetWindowTextW(contact_->mobile2().c_str());

    control = GetDlgItem(IDC_EDIT_EMAIL);
    control->SetWindowTextW(contact_->email().c_str());

    control = GetDlgItem(IDC_COMBO_CATEGORY);
    control->SetWindowTextW(contact_->GetCategoryName().c_str());

    control = GetDlgItem(IDC_COMBO_TYPE);
    control->SetWindowTextW(ContactTypeToString(contact_->type()).c_str());

    control = GetDlgItem(IDC_EDIT_ADDRESS);
    control->SetWindowTextW(contact_->address().c_str());

    control = GetDlgItem(IDC_EDIT_COMPANY);
    control->SetWindowTextW(contact_->company().c_str());

    control = GetDlgItem(IDC_EDIT_DEPARTMENT);
    control->SetWindowTextW(contact_->department().c_str());

    control = GetDlgItem(IDC_EDIT_DUTY);
    control->SetWindowTextW(contact_->duty().c_str());

    control = GetDlgItem(IDC_EDIT_REMARKS);
    control->SetWindowTextW(contact_->remarks().c_str());
}

void ContactDialog::SyncContact() {
    CString value;
    CWnd* control = GetDlgItem(IDC_EDIT_NAME);
    control->GetWindowTextW(value);
    contact_->name(static_cast<LPCTSTR>(value));

    control = GetDlgItem(IDC_EDIT_PHONE);
    control->GetWindowTextW(value);
    contact_->telephone(static_cast<LPCTSTR>(value));

    control = GetDlgItem(IDC_EDIT_PHONE2);
    control->GetWindowTextW(value);
    contact_->telephone2(static_cast<LPCTSTR>(value));

    control = GetDlgItem(IDC_EDIT_MOBILE);
    control->GetWindowTextW(value);
    contact_->mobile(static_cast<LPCTSTR>(value));

    control = GetDlgItem(IDC_EDIT_MOBILE2);
    control->GetWindowTextW(value);
    contact_->mobile2(static_cast<LPCTSTR>(value));

    control = GetDlgItem(IDC_EDIT_EMAIL);
    control->GetWindowTextW(value);
    contact_->email(static_cast<LPCTSTR>(value));

    control = GetDlgItem(IDC_COMBO_CATEGORY);
    control->GetWindowTextW(value);
    contact_->categoryId(ContactCategory::GetIdByName(static_cast<LPCTSTR>(value)));

    control = GetDlgItem(IDC_COMBO_TYPE);
    control->GetWindowTextW(value);
    contact_->type(StringToContactType(static_cast<LPCTSTR>(value)));

    control = GetDlgItem(IDC_EDIT_ADDRESS);
    control->GetWindowTextW(value);
    contact_->address(static_cast<LPCTSTR>(value));

    control = GetDlgItem(IDC_EDIT_COMPANY);
    control->GetWindowTextW(value);
    contact_->company(static_cast<LPCTSTR>(value));

    control = GetDlgItem(IDC_EDIT_DEPARTMENT);
    control->GetWindowTextW(value);
    contact_->department(static_cast<LPCTSTR>(value));

    control = GetDlgItem(IDC_EDIT_DUTY);
    control->GetWindowTextW(value);
    contact_->duty(static_cast<LPCTSTR>(value));

    control = GetDlgItem(IDC_EDIT_REMARKS);
    control->GetWindowTextW(value);
    contact_->remarks(static_cast<LPCTSTR>(value));
}

Util::shared_ptr<Contact> const ContactDialog::GetContact() const {
    return contact_;
}

void ContactDialog::OnBnClickedButtonClose() {
    // TODO: Add your control notification handler code here
    SyncContact();
    if (listener_) {
        listener_->PostMessageW(UM_CONTACT_UPDATE_FINALLY);
    }
    ShowWindow(SW_HIDE);
    //OnOK();
    //DestroyWindow();
}

BOOL ContactDialog::OnInitDialog() {
    CDialog::OnInitDialog();

    // TODO:  Add extra initialization here
    //setup type
    CComboBox* control = static_cast<CComboBox*>(GetDlgItem(IDC_COMBO_TYPE));
    //control->AddString(L"All");
    control->AddString(L"Normal");
    control->AddString(L"VIP");
    control->AddString(L"Blacklist");

	SIZE buttonSize;
	buttonSize.cx = 48;
	buttonSize.cy = 48;
	std::wstring maskFilename = L"/FlashDrv/debug/buttonMask.bmp";
	closeButton_.SetImage(L"/FlashDrv/debug/close.jpg", buttonSize, maskFilename);
	closeButton_.SubclassDlgItem(IDC_BUTTON_CLOSE, this);
	callButton_.SetImage(L"/FlashDrv/debug/call.jpg", buttonSize, maskFilename);
	callButton_.SubclassDlgItem(IDC_BUTTON_CALL, this);
	voiceMessagesButton_.SetImage(L"/FlashDrv/debug/message.jpg", buttonSize, maskFilename);
	voiceMessagesButton_.SubclassDlgItem(IDC_BUTTON_VOICE_MESSAGES, this);

	//font_.CreateFont(20, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"ËÎÌו");
	font_.CreatePointFont(Screen::fontPoint, L"ËÎÌו");
	GetDlgItem(IDC_EDIT_NAME)->SetFont(&font_, FALSE);
	GetDlgItem(IDC_EDIT_PHONE)->SetFont(&font_, FALSE);
	GetDlgItem(IDC_EDIT_PHONE2)->SetFont(&font_, FALSE);
	GetDlgItem(IDC_EDIT_MOBILE)->SetFont(&font_, FALSE);
	GetDlgItem(IDC_EDIT_MOBILE2)->SetFont(&font_, FALSE);
	GetDlgItem(IDC_EDIT_EMAIL)->SetFont(&font_, FALSE);
	GetDlgItem(IDC_COMBO_CATEGORY)->SetFont(&font_, FALSE);
	GetDlgItem(IDC_COMBO_TYPE)->SetFont(&font_, FALSE);
	GetDlgItem(IDC_EDIT_ADDRESS)->SetFont(&font_, FALSE);
	GetDlgItem(IDC_EDIT_COMPANY)->SetFont(&font_, FALSE);
	GetDlgItem(IDC_EDIT_DEPARTMENT)->SetFont(&font_, FALSE);
	GetDlgItem(IDC_EDIT_DUTY)->SetFont(&font_, FALSE);
	GetDlgItem(IDC_EDIT_REMARKS)->SetFont(&font_, FALSE);

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void ContactDialog::OnBnClickedButtonCallPhone() {
    CWnd* control = GetDlgItem(IDC_EDIT_PHONE);
    CString value;
    control->GetWindowTextW(value);
    if (value != L"") {
		Telephone::Instance()->SetContact(contact_);
        std::wstring* number = new std::wstring(static_cast<LPCTSTR>(value));
        Telephone::Instance()->fireEvent(Telephone::eSoftDial, reinterpret_cast<void*>(number));
    }
}

void ContactDialog::OnBnClickedButtonCallPhone2() {
    CWnd* control = GetDlgItem(IDC_EDIT_PHONE2);
    CString value;
    control->GetWindowTextW(value);
    if (value != L"") {
		Telephone::Instance()->SetContact(contact_);
        std::wstring* number = new std::wstring(static_cast<LPCTSTR>(value));
        Telephone::Instance()->fireEvent(Telephone::eSoftDial, reinterpret_cast<void*>(number));
    }
}

void ContactDialog::OnBnClickedButtonCallMobile() {
    CWnd* control = GetDlgItem(IDC_EDIT_MOBILE);
    CString value;
    control->GetWindowTextW(value);
    if (value != L"") {
		Telephone::Instance()->SetContact(contact_);
        std::wstring* number = new std::wstring(static_cast<LPCTSTR>(value));
        Telephone::Instance()->fireEvent(Telephone::eSoftDial, reinterpret_cast<void*>(number));
    }
}

void ContactDialog::OnBnClickedButtonCallMobile2() {
    CWnd* control = GetDlgItem(IDC_EDIT_MOBILE);
    CString value;
    control->GetWindowTextW(value);
    if (value != L"") {
		Telephone::Instance()->SetContact(contact_);
        std::wstring* number = new std::wstring(static_cast<LPCTSTR>(value));
        Telephone::Instance()->fireEvent(Telephone::eSoftDial, reinterpret_cast<void*>(number));
    }
}

HBRUSH ContactDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) {
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
			//hbr = BackgroundImage::setStaticProperties(pDC->m_hDC, bounds, Util::ImageOp::GetImage(backgroundImageFilename, backgroundImageSize), font, RGB(255, 255, 255));
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

    return hbr;
}

void ContactDialog::OnBnClickedButtonCall() {
    callForm->SetListener(this);
    callForm->ShowWindow(SW_SHOW);
}

void ContactDialog::OnBnClickedButtonVoiceMessages() {
    std::vector<Util::shared_ptr<CallInfo> > callInfos = CallInfo::GetAllNotPlayedLeaveWord();
    if (!callInfos.empty()) {
        playingRecordsForm->SetCallInfos(callInfos);
        playingRecordsForm->ShowWindow(SW_SHOW);
    }
}

void ContactDialog::OnShowWindow(BOOL bShow, UINT nStatus) {
    CDialog::OnShowWindow(bShow, nStatus);

    // TODO: Add your message handler code here
    if (bShow) {
        bool hasCall = Telephone::Instance()->HasCall();
        if (hasCall) {
            CWnd* control = GetDlgItem(IDC_BUTTON_CALL);
            control->ShowWindow(SW_SHOW);
        }
        //setup category
        CComboBox* control = static_cast<CComboBox*>(GetDlgItem(IDC_COMBO_CATEGORY));
        //control->AddString(L"All");
        std::vector<Util::shared_ptr<ContactCategory> > categories = ContactCategory::Select(L"");
        for (size_t i = 0; i < categories.size(); ++i) {
            control->AddString(categories[i]->name().c_str());
        }
        //CComboBox* control = static_cast<CComboBox*>(GetDlgItem(IDC_COMBO_CATEGORY));
        //std::vector<Util::shared_ptr<ContactCategory> > categories = ContactCategory::All();
        //for (size_t i = 0; i < categories.size(); ++i) {
        //    control->AddString(categories[i]->name().c_str());
        //}
    }
}

void ContactDialog::OnEnSetfocusEditName() {
    SipShowIM(SIPF_ON);
}

void ContactDialog::OnEnKillfocusEditName() {
    SipShowIM(SIPF_OFF);
}

void ContactDialog::OnEnSetfocusEditPhone() {
    SipShowIM(SIPF_ON);
}

void ContactDialog::OnEnKillfocusEditPhone() {
    SipShowIM(SIPF_OFF);
}

void ContactDialog::OnEnSetfocusEditPhone2() {
    SipShowIM(SIPF_ON);
}

void ContactDialog::OnEnKillfocusEditPhone2() {
    SipShowIM(SIPF_OFF);
}

void ContactDialog::OnEnSetfocusEditMobile() {
    SipShowIM(SIPF_ON);
}

void ContactDialog::OnEnKillfocusEditMobile() {
    SipShowIM(SIPF_OFF);
}

void ContactDialog::OnEnSetfocusEditMobile2() {
    SipShowIM(SIPF_ON);
}

void ContactDialog::OnEnKillfocusEditMobile2() {
    SipShowIM(SIPF_OFF);
}

void ContactDialog::OnEnSetfocusEditEmail() {
    SipShowIM(SIPF_ON);
}

void ContactDialog::OnEnKillfocusEditEmail() {
    SipShowIM(SIPF_OFF);
}

void ContactDialog::OnEnSetfocusEditAddress() {
    SipShowIM(SIPF_ON);
}

void ContactDialog::OnEnKillfocusEditAddress() {
    SipShowIM(SIPF_OFF);
}

void ContactDialog::OnEnSetfocusEditCompany() {
    SipShowIM(SIPF_ON);
}

void ContactDialog::OnEnKillfocusEditCompany() {
    SipShowIM(SIPF_OFF);
}

void ContactDialog::OnEnSetfocusEditDepartment() {
    SipShowIM(SIPF_ON);
}

void ContactDialog::OnEnKillfocusEditDepartment() {
    SipShowIM(SIPF_OFF);
}

void ContactDialog::OnEnSetfocusEditDuty() {
    SipShowIM(SIPF_ON);
}

void ContactDialog::OnEnKillfocusEditDuty() {
    SipShowIM(SIPF_OFF);
}

void ContactDialog::OnEnSetfocusEditRemarks() {
    SipShowIM(SIPF_ON);
}

void ContactDialog::OnEnKillfocusEditRemarks() {
    SipShowIM(SIPF_OFF);
}

LRESULT ContactDialog::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) {
    // TODO: Add your specialized code here and/or call the base class
    CWnd* control = 0;
    switch (message) {
        case UM_SHOW_TELEPHONE:
            control = GetDlgItem(IDC_BUTTON_CALL);
            control->ShowWindow(SW_SHOW);
            break;
        default:
            break;
    }
    return CDialog::WindowProc(message, wParam, lParam);
}

BOOL ContactDialog::OnEraseBkgnd(CDC* pDC) {
	BOOL r = BackgroundImage::drawBackground(m_hWnd);
	if (!r) {
        DWORD errorCode = GetLastError();
        DWORD ec = errorCode;
	}
	//return CDialog::OnEraseBkgnd(pDC);
	return TRUE;
}

void ContactDialog::OnCbnSelchangeComboCategory()
{
    // TODO: Add your control notification handler code here
}
