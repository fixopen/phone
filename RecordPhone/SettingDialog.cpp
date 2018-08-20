// SettingDialog.cpp : implementation file
//

#include "stdafx.h"
#include "RecordPhone.h"
#include "SettingDialog.h"
#include "CallDialog.h"
#include "PlayingRecordsDialog.h"
#include "Telephone.h"
#include "BackgroundImage.h"
#include "Util/ImageOp.h"

SettingDialog* settingForm = 0;

// SettingDialog dialog

IMPLEMENT_DYNAMIC(SettingDialog, CDialog)

SettingDialog::SettingDialog(CWnd* pParent /*=NULL*/)
: CDialog(SettingDialog::IDD, pParent) {
}

SettingDialog::~SettingDialog() {
}

void SettingDialog::DoDataExchange(CDataExchange* pDX) {
    CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(SettingDialog, CDialog)
    ON_BN_CLICKED(IDC_BUTTON_CLOSE, &SettingDialog::OnBnClickedButtonClose)
    ON_WM_DESTROY()
    ON_BN_CLICKED(IDC_BUTTON_TELEPHONE, &SettingDialog::OnBnClickedButtonTelephone)
    ON_BN_CLICKED(IDC_BUTTON_SYSTEM_PARAMETERS, &SettingDialog::OnBnClickedButtonSystemParameters)
    ON_BN_CLICKED(IDC_BUTTON_SYSTEM_INFO, &SettingDialog::OnBnClickedButtonSystemInfo)
    ON_WM_CTLCOLOR()
    ON_WM_ERASEBKGND()
    ON_BN_CLICKED(IDC_BUTTON_CALL, &SettingDialog::OnBnClickedButtonCall)
    ON_BN_CLICKED(IDC_BUTTON_VOICE_MESSAGES, &SettingDialog::OnBnClickedButtonVoiceMessages)
    //ON_WM_PAINT()
    ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_BUTTON1, &SettingDialog::OnBnClickedButton1)
END_MESSAGE_MAP()


// SettingDialog message handlers

void SettingDialog::OnBnClickedButtonClose() {
    // TODO: Add your control notification handler code here
    ShowWindow(SW_HIDE);
    //OnOK();
    //DestroyWindow();
}

BOOL SettingDialog::OnInitDialog() {
    CDialog::OnInitDialog();

    // TODO:  Add extra initialization here
    bool hasCall = Telephone::Instance()->HasCall();
    if (hasCall) {
        CWnd* control = GetDlgItem(IDC_BUTTON_CALL);
        control->ShowWindow(SW_SHOW);
    }
    //setup the children form : telephone, system params, system info
    telephoneSettingForm_ = new TelephoneSettingDialog(this);
    telephoneSettingForm_->Create(TelephoneSettingDialog::IDD, this);

    systemParametersForm_ = new SystemParametersDialog(this);
    systemParametersForm_->Create(SystemParametersDialog::IDD, this);

    systemInfoForm_ = new SystemInfoDialog(this);
    systemInfoForm_->Create(SystemInfoDialog::IDD, this);

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
	SIZE bigButtonSize;
	bigButtonSize.cx = 130;
	bigButtonSize.cy = 130;
	maskFilename = L"/FlashDrv/debug/bigButtonMask.bmp";
	telephoneButton_.SetImage(L"/FlashDrv/debug/telephoneSetting.jpg", bigButtonSize, maskFilename);
	telephoneButton_.SubclassDlgItem(IDC_BUTTON_TELEPHONE, this);
	parametersButton_.SetImage(L"/FlashDrv/debug/sysParameters.jpg", bigButtonSize, maskFilename);
	parametersButton_.SubclassDlgItem(IDC_BUTTON_SYSTEM_PARAMETERS, this);
	infoButton_.SetImage(L"/FlashDrv/debug/sysInfo.jpg", bigButtonSize, maskFilename);
	infoButton_.SubclassDlgItem(IDC_BUTTON_SYSTEM_INFO, this);

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void SettingDialog::OnDestroy() {
    CDialog::OnDestroy();

    // TODO: Add your message handler code here
    telephoneSettingForm_->DestroyWindow();
    delete telephoneSettingForm_;

    systemParametersForm_->DestroyWindow();
    delete systemParametersForm_;

    systemInfoForm_->DestroyWindow();
    delete systemInfoForm_;
}

void SettingDialog::OnBnClickedButtonTelephone() {
    // TODO: Add your control notification handler code here
    telephoneSettingForm_->ShowConfig();
    telephoneSettingForm_->ShowWindow(SW_SHOW);
    systemParametersForm_->ShowWindow(SW_HIDE);
    systemInfoForm_->ShowWindow(SW_HIDE);
	::SetFocus(m_hWnd);
}

void SettingDialog::OnBnClickedButtonSystemParameters() {
    // TODO: Add your control notification handler code here
    telephoneSettingForm_->ShowWindow(SW_HIDE);
    systemParametersForm_->ShowConfig();
    systemParametersForm_->ShowWindow(SW_SHOW);
    systemInfoForm_->ShowWindow(SW_HIDE);
	::SetFocus(m_hWnd);
}

void SettingDialog::OnBnClickedButtonSystemInfo() {
    // TODO: Add your control notification handler code here
    telephoneSettingForm_->ShowWindow(SW_HIDE);
    systemParametersForm_->ShowWindow(SW_HIDE);
    systemInfoForm_->ShowConfig();
    systemInfoForm_->ShowWindow(SW_SHOW);
	::SetFocus(m_hWnd);
}

HBRUSH SettingDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) {
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

BOOL SettingDialog::OnEraseBkgnd(CDC* pDC) {
    //SIZE imageSize;
    //imageSize.cx = 800;
    //imageSize.cy = 480;
	BOOL r = BackgroundImage::drawBackground(m_hWnd);
	if (!r) {
        DWORD errorCode = GetLastError();
        DWORD ec = errorCode;
	}
    return TRUE;
}

void SettingDialog::OnBnClickedButtonCall() {
    callForm->SetListener(this);
    callForm->ShowWindow(SW_SHOW);
}

void SettingDialog::OnBnClickedButtonVoiceMessages() {
    std::vector<Util::shared_ptr<CallInfo> > callInfos = CallInfo::GetAllNotPlayedLeaveWord();
    if (!callInfos.empty()) {
        playingRecordsForm->SetCallInfos(callInfos);
        playingRecordsForm->ShowWindow(SW_SHOW);
    }
}

//void SettingDialog::OnPaint() {
//    CPaintDC dc(this); // device context for painting
//    // TODO: Add your message handler code here
//    // Do not call CDialog::OnPaint() for painting messages
//    drawBackground_();
//}

void SettingDialog::OnShowWindow(BOOL bShow, UINT nStatus) {
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

LRESULT SettingDialog::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) {
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

void SettingDialog::OnBnClickedButton1() {
	// TODO: Add your control notification handler code here
	ShowWindow(SW_HIDE);
	ShowWindow(SW_SHOW);
}
