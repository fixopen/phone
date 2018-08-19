// RecordPhoneDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RecordPhone.h"
#include "RecordPhoneDlg.h"
#include "ContactsDialog.h"
#include "PhoneRecordsDialog.h"
#include "SettingDialog.h"
#include "CallDialog.h"
#include "CategoryDialog.h"
#include "CategoryDialog.h"
#include "PlayingRecordsDialog.h"
#include "Screen.h"
#include "SoundSegment.h"
#include "Telephone.h"
#include "BackgroundImage.h"

#include "Util/Log.h"
#include "Util/ImageOp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CRecordPhoneDlg dialog

CRecordPhoneDlg::CRecordPhoneDlg(CWnd* pParent /*=NULL*/)
: CDialog(CRecordPhoneDlg::IDD, pParent) {
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CRecordPhoneDlg::DoDataExchange(CDataExchange* pDX) {
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CRecordPhoneDlg, CDialog)
#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
	ON_WM_SIZE()
#endif
	//}}AFX_MSG_MAP
    ON_BN_CLICKED(IDC_BUTTON_CONTACTS, &CRecordPhoneDlg::OnBnClickedButtonContacts)
    ON_BN_CLICKED(IDC_BUTTON_PHONE_RECORDS, &CRecordPhoneDlg::OnBnClickedButtonPhoneRecords)
    ON_BN_CLICKED(IDC_BUTTON_SETTING, &CRecordPhoneDlg::OnBnClickedButtonSetting)
    ON_BN_CLICKED(IDC_BUTTON_VOICE_MESSAGES, &CRecordPhoneDlg::OnBnClickedButtonVoiceMessages)
    ON_BN_CLICKED(IDC_BUTTON_CALL, &CRecordPhoneDlg::OnBnClickedButtonCall)
    ON_STN_CLICKED(IDC_STATIC_DATE_TIME, &CRecordPhoneDlg::OnStnClickedStaticDateTime)
    ON_WM_DESTROY()
    ON_WM_CTLCOLOR()
    ON_WM_SHOWWINDOW()
	ON_WM_ERASEBKGND()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CRecordPhoneDlg message handlers

BOOL CRecordPhoneDlg::OnInitDialog() {
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	//setup log
    Util::Log::Sink* processor = new Util::Log::Sink();
    Util::Log::Destination* dst = new Util::Log::FileDestination("log.log");
    processor->SetDestination(dst);
    Util::Log::Formatter* fmt = new Util::Log::Formatter();
    processor->SetFormatter(fmt);
    Util::Log::AddProcess(processor, -1);
    //setup database
    Util::BaseData::InitDatabase(L"/FlashDrv/debug/recordPhone.sqlite");
    //init sound base variable
    SoundSegment::Init();
    //create all form
    contactsForm = new ContactsDialog(this);
    contactsForm->Create(ContactsDialog::IDD, this);
    contactForm = new ContactDialog(contactsForm);
    contactForm->Create(ContactDialog::IDD, contactsForm);
    categoryForm = new CategoryDialog(contactsForm);
    categoryForm->Create(CategoryDialog::IDD, contactsForm);
    phoneRecordsForm = new PhoneRecordsDialog(this);
    phoneRecordsForm->Create(PhoneRecordsDialog::IDD, this);
    playingRecordsForm = new PlayingRecordsDialog(phoneRecordsForm);
    playingRecordsForm->Create(PlayingRecordsDialog::IDD, phoneRecordsForm);
    settingForm = new SettingDialog(this);
    //settingForm->SetBackgroundDC(dc);
    settingForm->Create(SettingDialog::IDD, this);
    callForm = new CallDialog(this);
    callForm->Create(CallDialog::IDD, this);

	SIZE buttonSize;
	buttonSize.cx = 48;
	buttonSize.cy = 48;
	std::wstring maskFilename = L"/FlashDrv/debug/buttonMask.bmp";
	callButton_.SetImage(L"/FlashDrv/debug/call.jpg", buttonSize, maskFilename);
	callButton_.SubclassDlgItem(IDC_BUTTON_CALL, this);
	voiceMessagesButton_.SetImage(L"/FlashDrv/debug/message.jpg", buttonSize, maskFilename);
	voiceMessagesButton_.SubclassDlgItem(IDC_BUTTON_VOICE_MESSAGES, this);
	SIZE bigButtonSize;
	bigButtonSize.cx = 130;
	bigButtonSize.cy = 130;
	maskFilename = L"/FlashDrv/debug/bigButtonMask.bmp";
	contactsButton_.SetImage(L"/FlashDrv/debug/contacts.jpg", bigButtonSize, maskFilename);
	contactsButton_.SubclassDlgItem(IDC_BUTTON_CONTACTS, this);
	recordsButton_.SetImage(L"/FlashDrv/debug/records.jpg", bigButtonSize, maskFilename);
	recordsButton_.SubclassDlgItem(IDC_BUTTON_PHONE_RECORDS, this);
	settingButton_.SetImage(L"/FlashDrv/debug/setting.jpg", bigButtonSize, maskFilename);
	settingButton_.SubclassDlgItem(IDC_BUTTON_SETTING, this);
	
	SetWindowPos(NULL, 0, 0, 800, 480, SWP_NOREDRAW);
	
	CTime t = CTime::GetCurrentTime();
	CString time = t.Format(L"%A, %B %d, %Y %H:%M");
	GetDlgItem(IDC_STATIC_DATE_TIME)->SetWindowTextW(time);
	timerId_ = 44;
	clock_ = SetTimer(timerId_, 1000 * 60, NULL);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
void CRecordPhoneDlg::OnSize(UINT /*nType*/, int /*cx*/, int /*cy*/) {
	if (AfxIsDRAEnabled()) {
		DRA::RelayoutDialog(
			AfxGetResourceHandle(), 
			this->m_hWnd, 
			DRA::GetDisplayMode() != DRA::Portrait ? MAKEINTRESOURCE(IDD_RECORDPHONE_DIALOG_WIDE) : MAKEINTRESOURCE(IDD_RECORDPHONE_DIALOG));
	}
}
#endif

void CRecordPhoneDlg::OnBnClickedButtonContacts() {
    std::vector<Util::shared_ptr<Contact> > contacts = Contact::Select(L"", L"id", Util::BaseData::srDesc, 0, pageSize);
    contactsForm->SetContacts(contacts);
    contactsForm->ShowWindow(SW_SHOW);
}

void CRecordPhoneDlg::OnBnClickedButtonPhoneRecords() {
    std::vector<Util::shared_ptr<CallInfo> > history = CallInfo::Select(L"", L"id", Util::BaseData::srDesc, 0, pageSize);
    phoneRecordsForm->SetCallInfos(history);
    phoneRecordsForm->ShowWindow(SW_SHOW);
}

void CRecordPhoneDlg::OnBnClickedButtonSetting() {
    settingForm->ShowWindow(SW_SHOW);
}

void CRecordPhoneDlg::OnBnClickedButtonVoiceMessages() {
    std::vector<Util::shared_ptr<CallInfo> > callInfos = CallInfo::GetAllNotPlayedLeaveWord();
    if (!callInfos.empty()) {
        playingRecordsForm->SetCallInfos(callInfos);
        playingRecordsForm->ShowWindow(SW_SHOW);
    }
}

void CRecordPhoneDlg::OnBnClickedButtonCall() {
    callForm->SetListener(this);
    callForm->ShowWindow(SW_SHOW);
}

void CRecordPhoneDlg::OnStnClickedStaticDateTime() {
    //CalendarDialog* form = new CalendarDialog(this);
    //form->Create(CalendarDialog::IDD, this);
    //form->ShowWindow(SW_SHOW);
}

void CRecordPhoneDlg::OnDestroy() {
    CDialog::OnDestroy();

    // TODO: Add your message handler code here
    Util::BaseData::UninitDatabase();
}

HBRUSH CRecordPhoneDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) {
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
			hbr = BackgroundImage::setStaticProperties(pDC->m_hDC, bounds, Util::ImageOp::GetImage(backgroundImageFilename, backgroundImageSize), font, RGB(255, 255, 255));
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

void CRecordPhoneDlg::OnShowWindow(BOOL bShow, UINT nStatus) {
    CDialog::OnShowWindow(bShow, nStatus);

    // TODO: Add your message handler code here
    if (bShow) {
        bool hasCall = Telephone::Instance()->HasCall();
        if (hasCall) {
            CWnd* control = GetDlgItem(IDC_BUTTON_CALL);
            control->ShowWindow(SW_SHOW);
        }
    }
}

LRESULT CRecordPhoneDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) {
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

BOOL CRecordPhoneDlg::OnEraseBkgnd(CDC* pDC) {
	BOOL r = BackgroundImage::drawBackground(m_hWnd);
	if (!r) {
        DWORD errorCode = GetLastError();
        DWORD ec = errorCode;
	}
	//return CDialog::OnEraseBkgnd(pDC);
	return TRUE;
}

void CRecordPhoneDlg::OnTimer(UINT_PTR nIDEvent) {
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent == timerId_) {
		CTime t = CTime::GetCurrentTime();
		CString time = t.Format(L"%A, %B %d, %Y %H:%M");
		GetDlgItem(IDC_STATIC_DATE_TIME)->SetWindowTextW(time);
	}
	CDialog::OnTimer(nIDEvent);
}
