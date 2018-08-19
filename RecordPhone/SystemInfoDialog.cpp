// SystemInfoDialog.cpp : implementation file
//

#include "stdafx.h"
#include "RecordPhone.h"
#include "SystemInfoDialog.h"
#include "CallDialog.h"
#include "PlayingRecordsDialog.h"
#include "Telephone.h"
#include "BackgroundImage.h"
#include "Util/ImageOp.h"

// SystemInfoDialog dialog

IMPLEMENT_DYNAMIC(SystemInfoDialog, CDialog)

SystemInfoDialog::SystemInfoDialog(CWnd* pParent /*=NULL*/)
: CDialog(SystemInfoDialog::IDD, pParent) {
}

SystemInfoDialog::~SystemInfoDialog() {
}

void SystemInfoDialog::DoDataExchange(CDataExchange* pDX) {
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(SystemInfoDialog, CDialog)
    ON_BN_CLICKED(IDC_BUTTON_CLOSE, &SystemInfoDialog::OnBnClickedButtonClose)
    ON_BN_CLICKED(IDC_BUTTON_CLEAR_CONTACTS, &SystemInfoDialog::OnBnClickedButtonClearContacts)
    ON_BN_CLICKED(IDC_BUTTON_CLEAR_VOICE_MESSAGES, &SystemInfoDialog::OnBnClickedButtonClearVoiceMessages)
    ON_BN_CLICKED(IDC_BUTTON_CLEAR_CALL_HISTORY, &SystemInfoDialog::OnBnClickedButtonClearCallHistory)
    ON_WM_CTLCOLOR()
    ON_BN_CLICKED(IDC_BUTTON_CALL, &SystemInfoDialog::OnBnClickedButtonCall)
    ON_BN_CLICKED(IDC_BUTTON_VOICE_MESSAGES, &SystemInfoDialog::OnBnClickedButtonVoiceMessages)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// SystemInfoDialog message handlers

void SystemInfoDialog::OnBnClickedButtonClose() {
    // TODO: Add your control notification handler code here
    ShowWindow(SW_HIDE);
}

void SystemInfoDialog::OnBnClickedButtonClearContacts() {
    // TODO: Add your control notification handler code here
    Contact::Remove(L"");
    CWnd* control = GetDlgItem(IDC_STATIC_CONTACT_COUNT);
    control->SetWindowTextW(L"0");
}

void SystemInfoDialog::OnBnClickedButtonClearVoiceMessages() {
    // TODO: Add your control notification handler code here
    SoundSegment::Remove(L"[contactInfoId] <> 0");
    CWnd* control = GetDlgItem(IDC_STATIC_VOICE_MESSAGE_COUNT);
    control->SetWindowTextW(L"0");
}

void SystemInfoDialog::OnBnClickedButtonClearCallHistory() {
    // TODO: Add your control notification handler code here
    CallInfo::Remove(L"");
    CWnd* control = GetDlgItem(IDC_STATIC_CALL_HISTORY_COUNT);
    control->SetWindowTextW(L"0");
}

HBRUSH SystemInfoDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) {
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

void SystemInfoDialog::OnBnClickedButtonCall() {
    callForm->SetListener(this);
    callForm->ShowWindow(SW_SHOW);
}

void SystemInfoDialog::OnBnClickedButtonVoiceMessages() {
    std::vector<Util::shared_ptr<CallInfo> > callInfos = CallInfo::GetAllNotPlayedLeaveWord();
    if (!callInfos.empty()) {
        playingRecordsForm->SetCallInfos(callInfos);
        playingRecordsForm->ShowWindow(SW_SHOW);
    }
}

BOOL SystemInfoDialog::OnInitDialog() {
    CDialog::OnInitDialog();

    // TODO:  Add extra initialization here
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

	return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void SystemInfoDialog::ShowConfig() {
    bool hasCall = Telephone::Instance()->HasCall();
    if (hasCall) {
        CWnd* control = GetDlgItem(IDC_BUTTON_CALL);
        control->ShowWindow(SW_SHOW);
    }
    int count = Contact::GetDataCount(L"");
    CWnd* control = GetDlgItem(IDC_STATIC_CONTACT_COUNT);
    control->SetWindowTextW(Util::StringOp::FromInt(count).c_str());
    count = SoundSegment::GetDataCount(L"[contactInfoId] <> 0");
    control = GetDlgItem(IDC_STATIC_VOICE_MESSAGE_COUNT);
    control->SetWindowTextW(Util::StringOp::FromInt(count).c_str());
    count = CallInfo::GetDataCount(L"");
    control = GetDlgItem(IDC_STATIC_CALL_HISTORY_COUNT);
    control->SetWindowTextW(Util::StringOp::FromInt(count).c_str());
}

LRESULT SystemInfoDialog::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) {
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

BOOL SystemInfoDialog::OnEraseBkgnd(CDC* pDC) {
	BOOL r = BackgroundImage::drawBackground(m_hWnd);
	if (!r) {
        DWORD errorCode = GetLastError();
        DWORD ec = errorCode;
	}
	//return CDialog::OnEraseBkgnd(pDC);
	return TRUE;
}
