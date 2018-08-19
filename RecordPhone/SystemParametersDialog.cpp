// SystemParametersDialog.cpp : implementation file
//

#include "stdafx.h"
#include "RecordPhone.h"
#include "SystemParametersDialog.h"
#include "CallDialog.h"
#include "PlayingRecordsDialog.h"
#include "Telephone.h"
#include "BackgroundImage.h"
#include "Util/ImageOp.h"
#include "Util/Properties.h"

// SystemParametersDialog dialog

IMPLEMENT_DYNAMIC(SystemParametersDialog, CDialog)

SystemParametersDialog::SystemParametersDialog(CWnd* pParent /*=NULL*/)
: CDialog(SystemParametersDialog::IDD, pParent) {
}

SystemParametersDialog::~SystemParametersDialog() {
}

void SystemParametersDialog::DoDataExchange(CDataExchange* pDX) {
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(SystemParametersDialog, CDialog)
    ON_BN_CLICKED(IDC_BUTTON_CLOSE, &SystemParametersDialog::OnBnClickedButtonClose)
    ON_BN_CLICKED(IDC_BUTTON_RESTORE, &SystemParametersDialog::OnBnClickedButtonRestore)
    ON_BN_CLICKED(IDC_BUTTON_SCREEN_CALIBRATION, &SystemParametersDialog::OnBnClickedButtonScreenCalibration)
    ON_WM_CTLCOLOR()
    ON_BN_CLICKED(IDC_BUTTON_CALL, &SystemParametersDialog::OnBnClickedButtonCall)
    ON_BN_CLICKED(IDC_BUTTON_VOICE_MESSAGES, &SystemParametersDialog::OnBnClickedButtonVoiceMessages)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// SystemParametersDialog message handlers

void SystemParametersDialog::OnBnClickedButtonClose() {
    // TODO: Add your control notification handler code here
    CWnd* control = GetDlgItem(IDC_EDIT_RECORD_DATA_FOLDER);
    CString value;
    control->GetWindowTextW(value);
    //StoragePath::SetCallRecordPath(static_cast<LPCTSTR>(value));

    control = GetDlgItem(IDC_EDIT_RINGTONE_VOLUME);
    control->GetWindowTextW(value);
    Util::Properties::Set(L"ringtoneVolume", static_cast<LPCTSTR>(value));

    control = GetDlgItem(IDC_EDIT_MASTER_VOLUME);
    control->GetWindowTextW(value);
    Util::Properties::Set(L"masterVolume", static_cast<LPCTSTR>(value));

    CDateTimeCtrl* date = static_cast<CDateTimeCtrl*>(GetDlgItem(IDC_DATETIMEPICKER_DATE));
    SYSTEMTIME sysDate;
    date->GetTime(&sysDate);
    CDateTimeCtrl* time = static_cast<CDateTimeCtrl*>(GetDlgItem(IDC_DATETIMEPICKER_TIME));
    SYSTEMTIME sysTime;
    date->GetTime(&sysTime);
    SYSTEMTIME sysDateTime = sysDate;
    sysDateTime.wHour = sysTime.wHour;
    sysDateTime.wMinute = sysTime.wMinute;
    sysDateTime.wSecond = sysTime.wSecond;
    SetSystemTime(&sysDateTime);

    ShowWindow(SW_HIDE);
}

void SystemParametersDialog::OnBnClickedButtonRestore() {
    // TODO: Add your control notification handler code here
    //restore default setting
}

void SystemParametersDialog::OnBnClickedButtonScreenCalibration() {
    // TODO: Add your control notification handler code here
    //start screen calibration
	HINSTANCE hinstance = LoadLibrary(L"coredll.dll");
    if (hinstance == NULL) {
        AfxMessageBox (L"instance == NULL");
        return;
    }
    typedef BOOL (WINAPI *_TouchCalibrate)();
    _TouchCalibrate TouchCalibrate = NULL;
    TouchCalibrate = (_TouchCalibrate)GetProcAddress(hinstance, L"TouchCalibrate");
    if (TouchCalibrate == NULL) {
        AfxMessageBox(L"TouchCalibrate == NULL");
        return;
    }
	//extern BOOL g_bAdjustPanel;
	//g_bAdjustPanel = TRUE;
    TouchCalibrate();
	//g_bAdjustPanel = FALSE;
    FreeLibrary(hinstance);
	TCHAR szTemp[256] = {0};
	_tcscpy(szTemp, TEXT("HARDWARE\\DEVICEMAP\\TOUCH"));
	HKEY hKey = NULL;
	LONG hRes = RegOpenKeyEx(HKEY_LOCAL_MACHINE, szTemp, 0, 0, &hKey);
	if (ERROR_SUCCESS == hRes) {
	}
	LONG l = RegFlushKey(HKEY_LOCAL_MACHINE);
	RegCloseKey(hKey);
}

HBRUSH SystemParametersDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) {
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

void SystemParametersDialog::OnBnClickedButtonCall() {
    callForm->SetListener(this);
    callForm->ShowWindow(SW_SHOW);
}

void SystemParametersDialog::OnBnClickedButtonVoiceMessages() {
    std::vector<Util::shared_ptr<CallInfo> > callInfos = CallInfo::GetAllNotPlayedLeaveWord();
    if (!callInfos.empty()) {
        playingRecordsForm->SetCallInfos(callInfos);
        playingRecordsForm->ShowWindow(SW_SHOW);
    }
}

BOOL SystemParametersDialog::OnInitDialog() {
    CDialog::OnInitDialog();

    // TODO:  Add extra initialization here
	SIZE buttonSize;
	buttonSize.cx = 48;
	buttonSize.cy = 48;
	std::wstring maskFilename = L"/FlashDrv/debug/buttonMask.bmp";
	closeButton_.SetImage(L"/FlashDrv/debug/close.jpg", buttonSize, maskFilename);
	closeButton_.SubclassDlgItem(IDC_BUTTON_CLOSE, this);
	restoreButton_.SetImage(L"/FlashDrv/debug/restore.jpg", buttonSize, maskFilename);
	restoreButton_.SubclassDlgItem(IDC_BUTTON_RESTORE, this);
	callButton_.SetImage(L"/FlashDrv/debug/call.jpg", buttonSize, maskFilename);
	callButton_.SubclassDlgItem(IDC_BUTTON_CALL, this);
	voiceMessagesButton_.SetImage(L"/FlashDrv/debug/message.jpg", buttonSize, maskFilename);
	voiceMessagesButton_.SubclassDlgItem(IDC_BUTTON_VOICE_MESSAGES, this);

	SIZE screenCalibrationSize;
	screenCalibrationSize.cx = 43;
	screenCalibrationSize.cy = 43;
	maskFilename = L"/FlashDrv/debug/touchPanelMask.bmp";
	screenCalibrationButton_.SetImage(L"/FlashDrv/debug/touchPanel.jpg", buttonSize, maskFilename);
	screenCalibrationButton_.SubclassDlgItem(IDC_BUTTON_SCREEN_CALIBRATION, this);

	//font_.CreateFont(20, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"ËÎÌו");
	font_.CreatePointFont(Screen::fontPoint, L"ËÎÌו");
	GetDlgItem(IDC_EDIT_RECORD_DATA_FOLDER)->SetFont(&font_, FALSE);
	GetDlgItem(IDC_EDIT_RINGTONE_VOLUME)->SetFont(&font_, FALSE);
	GetDlgItem(IDC_EDIT_MASTER_VOLUME)->SetFont(&font_, FALSE);
	GetDlgItem(IDC_DATETIMEPICKER_DATE)->SetFont(&font_, FALSE);
	GetDlgItem(IDC_DATETIMEPICKER_TIME)->SetFont(&font_, FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void SystemParametersDialog::ShowConfig() {
    bool hasCall = Telephone::Instance()->HasCall();
    if (hasCall) {
        CWnd* control = GetDlgItem(IDC_BUTTON_CALL);
        control->ShowWindow(SW_SHOW);
    }

    CWnd* control = GetDlgItem(IDC_EDIT_RECORD_DATA_FOLDER);
    control->SetWindowTextW(StoragePath::GetCallRecordPath().c_str());

    control = GetDlgItem(IDC_EDIT_RINGTONE_VOLUME);
    std::wstring ringtoneVolume = Util::Properties::Get(L"ringtoneVolume");
    control->SetWindowTextW(ringtoneVolume.c_str());

    CSpinButtonCtrl* ringtone = static_cast<CSpinButtonCtrl*>(GetDlgItem(IDC_SPIN_RINGTONE_VOLUME));
    ringtone->SetRange(1, 100);
    ringtone->SetPos(Util::StringOp::ToInt(ringtoneVolume));
    ringtone->SetBuddy(control);

    control = GetDlgItem(IDC_EDIT_MASTER_VOLUME);
    std::wstring masterVolume = Util::Properties::Get(L"masterVolume");
    control->SetWindowTextW(masterVolume.c_str());

    CSpinButtonCtrl* master = static_cast<CSpinButtonCtrl*>(GetDlgItem(IDC_SPIN_MASTER_VOLUME));
    master->SetRange(1, 100);
    master->SetPos(Util::StringOp::ToInt(masterVolume));
    master->SetBuddy(control);
}

LRESULT SystemParametersDialog::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) {
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

BOOL SystemParametersDialog::OnEraseBkgnd(CDC* pDC) {
	BOOL r = BackgroundImage::drawBackground(m_hWnd);
	if (!r) {
        DWORD errorCode = GetLastError();
        DWORD ec = errorCode;
	}
	//return CDialog::OnEraseBkgnd(pDC);
	return TRUE;
}
