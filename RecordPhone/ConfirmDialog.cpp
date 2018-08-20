// ConfirmDialog.cpp : implementation file
//

#include "stdafx.h"
#include "RecordPhone.h"
#include "ConfirmDialog.h"
#include "BackgroundImage.h"
#include "Util/ImageOp.h"

ConfirmDialog* confirmForm = 0;
// ConfirmDialog dialog

IMPLEMENT_DYNAMIC(ConfirmDialog, CDialog)

ConfirmDialog::ConfirmDialog(CWnd* pParent /*=NULL*/)
: CDialog(ConfirmDialog::IDD, pParent)
, isOk_(false)
, listener_(0) {
}

ConfirmDialog::~ConfirmDialog() {
}

void ConfirmDialog::DoDataExchange(CDataExchange* pDX) {
	CDialog::DoDataExchange(pDX);
}

void ConfirmDialog::SetConfirmMessage(std::wstring const& message) {
    CWnd* control = GetDlgItem(IDC_STATIC_CONFIRM);
    control->SetWindowTextW(message.c_str());
}

BEGIN_MESSAGE_MAP(ConfirmDialog, CDialog)
    ON_BN_CLICKED(IDC_BUTTON_YES, &ConfirmDialog::OnBnClickedButtonYes)
    ON_BN_CLICKED(IDC_BUTTON_NO, &ConfirmDialog::OnBnClickedButtonNo)
    ON_WM_CTLCOLOR()
    ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// ConfirmDialog message handlers

void ConfirmDialog::OnBnClickedButtonYes() {
    isOk_ = true;
    if (listener_) {
        listener_->PostMessageW(UM_CONFIRM, isOk_, 0);
        listener_ = 0;
    }
    SetConfirmMessage(L"");
    ShowWindow(SW_HIDE);
}

void ConfirmDialog::OnBnClickedButtonNo() {
    isOk_ = false;
    if (listener_) {
        listener_->PostMessageW(UM_CONFIRM, isOk_, 0);
        listener_ = 0;
    }
    SetConfirmMessage(L"");
    ShowWindow(SW_HIDE);
}

HBRUSH ConfirmDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) {
    HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

    // TODO:  Change any attributes of the DC here
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

    // TODO:  Return a different brush if the default is not desired
    return hbr;
}

BOOL ConfirmDialog::OnEraseBkgnd(CDC* pDC) {
	BOOL r = BackgroundImage::drawBackground(m_hWnd);
	if (!r) {
        DWORD errorCode = GetLastError();
        DWORD ec = errorCode;
	}
	//return CDialog::OnEraseBkgnd(pDC);
	return TRUE;
}

BOOL ConfirmDialog::OnInitDialog() {
    CDialog::OnInitDialog();

    // TODO:  Add extra initialization here
	SIZE buttonSize;
	buttonSize.cx = 48;
	buttonSize.cy = 48;
	std::wstring maskFilename = L"/FlashDrv/debug/buttonMask.bmp";
	okButton_.SetImage(L"/FlashDrv/debug/ok.jpg", buttonSize, maskFilename);
	okButton_.SubclassDlgItem(IDC_BUTTON_YES, this);
	cancelButton_.SetImage(L"/FlashDrv/debug/cancel.jpg", buttonSize, maskFilename);
	cancelButton_.SubclassDlgItem(IDC_BUTTON_NO, this);

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}
