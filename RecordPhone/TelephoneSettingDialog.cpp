// TelephoneSettingDialog.cpp : implementation file
//

#include "stdafx.h"

#include <sip.h>

#include "RecordPhone.h"
#include "TelephoneSettingDialog.h"
#include "CallDialog.h"
#include "PlayingRecordsDialog.h"
#include "Telephone.h"
#include "BackgroundImage.h"
#include "Util/ImageOp.h"
#include "Util/Properties.h"
#include "Util/ScanDir.h"

// TelephoneSettingDialog dialog

IMPLEMENT_DYNAMIC(TelephoneSettingDialog, CDialog)

TelephoneSettingDialog::TelephoneSettingDialog(CWnd* pParent /*=NULL*/)
: CDialog(TelephoneSettingDialog::IDD, pParent) {
}

TelephoneSettingDialog::~TelephoneSettingDialog() {
}

void TelephoneSettingDialog::ShowConfig() {
    bool hasCall = Telephone::Instance()->HasCall();
    if (hasCall) {
        CWnd* control = GetDlgItem(IDC_BUTTON_CALL);
        control->ShowWindow(SW_SHOW);
    }
    CWnd* control = GetDlgItem(IDC_EDIT_THIS_TELEPHONE_NUMBER);
    control->SetWindowTextW(Util::Properties::Get(L"thisTelephoneNumber").c_str());

    control = GetDlgItem(IDC_EDIT_LOCAL_AREA_CODE);
    control->SetWindowTextW(Util::Properties::Get(L"localAreaCode").c_str());

    control = GetDlgItem(IDC_EDIT_OUTLINE_PREFIX);
    control->SetWindowTextW(Util::Properties::Get(L"outlinePrefix").c_str());

    control = GetDlgItem(IDC_EDIT_IP_PREFIX);
    control->SetWindowTextW(Util::Properties::Get(L"ipPrefix").c_str());

    control = GetDlgItem(IDC_COMBO_DEFAULT_RING);
    control->SetWindowTextW(Util::Properties::Get(L"defaultRingtone").c_str());

    control = GetDlgItem(IDC_EDIT_DELAY_COUNT);
    std::wstring delayCountValue = Util::Properties::Get(L"delayCount");
    control->SetWindowTextW(delayCountValue.c_str());

    CSpinButtonCtrl* delayCount = static_cast<CSpinButtonCtrl*>(GetDlgItem(IDC_SPIN_DELAY_COUNT));
    delayCount->SetRange(1, 5);
    delayCount->SetPos(Util::StringOp::ToInt(delayCountValue));
    delayCount->SetBuddy(control);

    ClearAllFirewall();
    FirewallType ft = static_cast<FirewallType>(Util::StringOp::ToInt(Util::Properties::Get(L"firewallType")));
    CButton* firewall;
    switch (ft) {
        case ftNull:
            //firewall = static_cast<CButton*>(GetDlgItem(IDC_RADIO_NULL));
            //firewall->SetCheck(BST_CHECKED);
            break;
        case ftBlacklist:
            firewall = static_cast<CButton*>(GetDlgItem(IDC_RADIO_BLACKLIST));
            firewall->SetCheck(BST_CHECKED);
            break;
        case ftNotInContacts:
            firewall = static_cast<CButton*>(GetDlgItem(IDC_RADIO_NOT_IN_CONTACTS));
            firewall->SetCheck(BST_CHECKED);
            break;
        case ftNotVIP:
            firewall = static_cast<CButton*>(GetDlgItem(IDC_RADIO_NOT_VIP));
            firewall->SetCheck(BST_CHECKED);
            break;
        case ftAll:
            firewall = static_cast<CButton*>(GetDlgItem(IDC_RADIO_ALL));
            firewall->SetCheck(BST_CHECKED);
            {
                CEdit* duration = static_cast<CEdit*>(GetDlgItem(IDC_EDIT_DURATION));
                duration->SetReadOnly();
                duration->SetWindowTextW(Util::Properties::Get(L"blockDuration").c_str());
            }
            break;
        default:
            break;
    }

    control = GetDlgItem(IDC_CHECK_FORCE_AUTO_RECORD);
    (static_cast<CButton*>(control))->SetCheck(BST_CHECKED);
    std::wstring v = Util::Properties::Get(L"autoRecord");
    if (v == L"0") {
        (static_cast<CButton*>(control))->SetCheck(BST_UNCHECKED);
    }

    control = GetDlgItem(IDC_EDIT_SPEED_DIAL_1);
    control->SetWindowTextW(Util::Properties::Get(L"speedDial1").c_str());

    control = GetDlgItem(IDC_EDIT_SPEED_DIAL_2);
    control->SetWindowTextW(Util::Properties::Get(L"speedDial2").c_str());

    control = GetDlgItem(IDC_EDIT_SPEED_DIAL_3);
    control->SetWindowTextW(Util::Properties::Get(L"speedDial3").c_str());

    control = GetDlgItem(IDC_EDIT_SPEED_DIAL_4);
    control->SetWindowTextW(Util::Properties::Get(L"speedDial4").c_str());

    control = GetDlgItem(IDC_EDIT_SPEED_DIAL_5);
    control->SetWindowTextW(Util::Properties::Get(L"speedDial5").c_str());

    control = GetDlgItem(IDC_EDIT_SPEED_DIAL_6);
    control->SetWindowTextW(Util::Properties::Get(L"speedDial6").c_str());

    control = GetDlgItem(IDC_EDIT_SPEED_DIAL_7);
    control->SetWindowTextW(Util::Properties::Get(L"speedDial7").c_str());

    control = GetDlgItem(IDC_EDIT_SPEED_DIAL_8);
    control->SetWindowTextW(Util::Properties::Get(L"speedDial8").c_str());

    control = GetDlgItem(IDC_EDIT_SPEED_DIAL_9);
    control->SetWindowTextW(Util::Properties::Get(L"speedDial9").c_str());

    control = GetDlgItem(IDC_EDIT_SPEED_DIAL_10);
    control->SetWindowTextW(Util::Properties::Get(L"speedDial10").c_str());

    control = GetDlgItem(IDC_EDIT_SPEED_DIAL_11);
    control->SetWindowTextW(Util::Properties::Get(L"speedDial11").c_str());

    control = GetDlgItem(IDC_EDIT_SPEED_DIAL_12);
    control->SetWindowTextW(Util::Properties::Get(L"speedDial12").c_str());
}

void TelephoneSettingDialog::DoDataExchange(CDataExchange* pDX) {
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(TelephoneSettingDialog, CDialog)
    ON_BN_CLICKED(IDC_BUTTON_RESTORE, &TelephoneSettingDialog::OnBnClickedButtonRestore)
    ON_BN_CLICKED(IDC_BUTTON_CLOSE, &TelephoneSettingDialog::OnBnClickedButtonClose)
    ON_WM_CTLCOLOR()
    ON_BN_CLICKED(IDC_BUTTON_CALL, &TelephoneSettingDialog::OnBnClickedButtonCall)
    ON_BN_CLICKED(IDC_BUTTON_VOICE_MESSAGES, &TelephoneSettingDialog::OnBnClickedButtonVoiceMessages)
    ON_EN_SETFOCUS(IDC_EDIT_THIS_TELEPHONE_NUMBER, &TelephoneSettingDialog::OnEnSetfocusEditThisTelephoneNumber)
    ON_EN_KILLFOCUS(IDC_EDIT_THIS_TELEPHONE_NUMBER, &TelephoneSettingDialog::OnEnKillfocusEditThisTelephoneNumber)
    ON_EN_SETFOCUS(IDC_EDIT_LOCAL_AREA_CODE, &TelephoneSettingDialog::OnEnSetfocusEditLocalAreaCode)
    ON_EN_KILLFOCUS(IDC_EDIT_LOCAL_AREA_CODE, &TelephoneSettingDialog::OnEnKillfocusEditLocalAreaCode)
    ON_EN_SETFOCUS(IDC_EDIT_OUTLINE_PREFIX, &TelephoneSettingDialog::OnEnSetfocusEditOutlinePrefix)
    ON_EN_KILLFOCUS(IDC_EDIT_OUTLINE_PREFIX, &TelephoneSettingDialog::OnEnKillfocusEditOutlinePrefix)
    ON_EN_SETFOCUS(IDC_EDIT_IP_PREFIX, &TelephoneSettingDialog::OnEnSetfocusEditIpPrefix)
    ON_EN_KILLFOCUS(IDC_EDIT_IP_PREFIX, &TelephoneSettingDialog::OnEnKillfocusEditIpPrefix)
    ON_EN_SETFOCUS(IDC_EDIT_DURATION, &TelephoneSettingDialog::OnEnSetfocusEditDuration)
    ON_EN_KILLFOCUS(IDC_EDIT_DURATION, &TelephoneSettingDialog::OnEnKillfocusEditDuration)
    ON_EN_SETFOCUS(IDC_EDIT_SPEED_DIAL_1, &TelephoneSettingDialog::OnEnSetfocusEditSpeedDial1)
    ON_EN_KILLFOCUS(IDC_EDIT_SPEED_DIAL_1, &TelephoneSettingDialog::OnEnKillfocusEditSpeedDial1)
    ON_EN_SETFOCUS(IDC_EDIT_SPEED_DIAL_2, &TelephoneSettingDialog::OnEnSetfocusEditSpeedDial2)
    ON_EN_KILLFOCUS(IDC_EDIT_SPEED_DIAL_2, &TelephoneSettingDialog::OnEnKillfocusEditSpeedDial2)
    ON_EN_SETFOCUS(IDC_EDIT_SPEED_DIAL_3, &TelephoneSettingDialog::OnEnSetfocusEditSpeedDial3)
    ON_EN_KILLFOCUS(IDC_EDIT_SPEED_DIAL_3, &TelephoneSettingDialog::OnEnKillfocusEditSpeedDial3)
    ON_EN_SETFOCUS(IDC_EDIT_SPEED_DIAL_4, &TelephoneSettingDialog::OnEnSetfocusEditSpeedDial4)
    ON_EN_KILLFOCUS(IDC_EDIT_SPEED_DIAL_4, &TelephoneSettingDialog::OnEnKillfocusEditSpeedDial4)
    ON_EN_SETFOCUS(IDC_EDIT_SPEED_DIAL_5, &TelephoneSettingDialog::OnEnSetfocusEditSpeedDial5)
    ON_EN_KILLFOCUS(IDC_EDIT_SPEED_DIAL_5, &TelephoneSettingDialog::OnEnKillfocusEditSpeedDial5)
    ON_EN_SETFOCUS(IDC_EDIT_SPEED_DIAL_6, &TelephoneSettingDialog::OnEnSetfocusEditSpeedDial6)
    ON_EN_KILLFOCUS(IDC_EDIT_SPEED_DIAL_6, &TelephoneSettingDialog::OnEnKillfocusEditSpeedDial6)
    ON_EN_SETFOCUS(IDC_EDIT_SPEED_DIAL_7, &TelephoneSettingDialog::OnEnSetfocusEditSpeedDial7)
    ON_EN_KILLFOCUS(IDC_EDIT_SPEED_DIAL_7, &TelephoneSettingDialog::OnEnKillfocusEditSpeedDial7)
    ON_EN_SETFOCUS(IDC_EDIT_SPEED_DIAL_8, &TelephoneSettingDialog::OnEnSetfocusEditSpeedDial8)
    ON_EN_KILLFOCUS(IDC_EDIT_SPEED_DIAL_8, &TelephoneSettingDialog::OnEnKillfocusEditSpeedDial8)
    ON_EN_SETFOCUS(IDC_EDIT_SPEED_DIAL_9, &TelephoneSettingDialog::OnEnSetfocusEditSpeedDial9)
    ON_EN_KILLFOCUS(IDC_EDIT_SPEED_DIAL_9, &TelephoneSettingDialog::OnEnKillfocusEditSpeedDial9)
    ON_EN_SETFOCUS(IDC_EDIT_SPEED_DIAL_10, &TelephoneSettingDialog::OnEnSetfocusEditSpeedDial10)
    ON_EN_KILLFOCUS(IDC_EDIT_SPEED_DIAL_10, &TelephoneSettingDialog::OnEnKillfocusEditSpeedDial10)
    ON_EN_SETFOCUS(IDC_EDIT_SPEED_DIAL_11, &TelephoneSettingDialog::OnEnSetfocusEditSpeedDial11)
    ON_EN_KILLFOCUS(IDC_EDIT_SPEED_DIAL_11, &TelephoneSettingDialog::OnEnKillfocusEditSpeedDial11)
    ON_EN_SETFOCUS(IDC_EDIT_SPEED_DIAL_12, &TelephoneSettingDialog::OnEnSetfocusEditSpeedDial12)
    ON_EN_KILLFOCUS(IDC_EDIT_SPEED_DIAL_12, &TelephoneSettingDialog::OnEnKillfocusEditSpeedDial12)
    ON_BN_CLICKED(IDC_RADIO_NULL, &TelephoneSettingDialog::OnBnClickedRadioNull)
    ON_BN_CLICKED(IDC_RADIO_BLACKLIST, &TelephoneSettingDialog::OnBnClickedRadioBlacklist)
    ON_BN_CLICKED(IDC_RADIO_NOT_IN_CONTACTS, &TelephoneSettingDialog::OnBnClickedRadioNotInContacts)
    ON_BN_CLICKED(IDC_RADIO_NOT_VIP, &TelephoneSettingDialog::OnBnClickedRadioNotVip)
    ON_BN_CLICKED(IDC_RADIO_ALL, &TelephoneSettingDialog::OnBnClickedRadioAll)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// TelephoneSettingDialog message handlers

void TelephoneSettingDialog::OnBnClickedButtonRestore() {
    // TODO: Add your control notification handler code here
    //restore default setting
}

void TelephoneSettingDialog::OnBnClickedButtonClose() {
    // TODO: Add your control notification handler code here
    CWnd* control = GetDlgItem(IDC_EDIT_THIS_TELEPHONE_NUMBER);
    CString value;
    control->GetWindowTextW(value);
    Util::Properties::Set(L"thisTelephoneNumber", static_cast<LPCTSTR>(value));

    control = GetDlgItem(IDC_EDIT_LOCAL_AREA_CODE);
    control->GetWindowTextW(value);
    Util::Properties::Set(L"localAreaCode", static_cast<LPCTSTR>(value));

    control = GetDlgItem(IDC_EDIT_OUTLINE_PREFIX);
    control->GetWindowTextW(value);
    Util::Properties::Set(L"outlinePrefix", static_cast<LPCTSTR>(value));

    control = GetDlgItem(IDC_EDIT_IP_PREFIX);
    control->GetWindowTextW(value);
    Util::Properties::Set(L"ipPrefix", static_cast<LPCTSTR>(value));

    control = GetDlgItem(IDC_COMBO_DEFAULT_RING);
    control->GetWindowTextW(value);
    Util::Properties::Set(L"defaultRingtone", static_cast<LPCTSTR>(value));

    control = GetDlgItem(IDC_EDIT_DELAY_COUNT);
    control->GetWindowTextW(value);
    Util::Properties::Set(L"delayCount", static_cast<LPCTSTR>(value));

    Util::Properties::Set(L"firewallType", Util::StringOp::FromInt(firewallType_));
    if (firewallType_ == ftAll) {
        control = GetDlgItem(IDC_EDIT_DURATION);
        control->GetWindowTextW(value);
        Util::Properties::Set(L"blockDuration", static_cast<LPCTSTR>(value));
    }

    control = GetDlgItem(IDC_CHECK_FORCE_AUTO_RECORD);
    std::wstring v = L"1";
    int checkState = (static_cast<CButton*>(control))->GetCheck();
    switch (checkState) {
        case BST_UNCHECKED:
            v = L"0";
            break;
        case BST_CHECKED:
            break;
        case BST_INDETERMINATE:
            break;
        default:
            break;
    }
    Util::Properties::Set(L"autoRecord", v);

    control = GetDlgItem(IDC_EDIT_SPEED_DIAL_1);
    control->GetWindowTextW(value);
    Util::Properties::Set(L"speedDial1", static_cast<LPCTSTR>(value));

    control = GetDlgItem(IDC_EDIT_SPEED_DIAL_2);
    control->GetWindowTextW(value);
    Util::Properties::Set(L"speedDial2", static_cast<LPCTSTR>(value));

    control = GetDlgItem(IDC_EDIT_SPEED_DIAL_3);
    control->GetWindowTextW(value);
    Util::Properties::Set(L"speedDial3", static_cast<LPCTSTR>(value));

    control = GetDlgItem(IDC_EDIT_SPEED_DIAL_4);
    control->GetWindowTextW(value);
    Util::Properties::Set(L"speedDial4", static_cast<LPCTSTR>(value));

    control = GetDlgItem(IDC_EDIT_SPEED_DIAL_5);
    control->GetWindowTextW(value);
    Util::Properties::Set(L"speedDial5", static_cast<LPCTSTR>(value));

    control = GetDlgItem(IDC_EDIT_SPEED_DIAL_6);
    control->GetWindowTextW(value);
    Util::Properties::Set(L"speedDial6", static_cast<LPCTSTR>(value));

    control = GetDlgItem(IDC_EDIT_SPEED_DIAL_7);
    control->GetWindowTextW(value);
    Util::Properties::Set(L"speedDial7", static_cast<LPCTSTR>(value));

    control = GetDlgItem(IDC_EDIT_SPEED_DIAL_8);
    control->GetWindowTextW(value);
    Util::Properties::Set(L"speedDial8", static_cast<LPCTSTR>(value));

    control = GetDlgItem(IDC_EDIT_SPEED_DIAL_9);
    control->GetWindowTextW(value);
    Util::Properties::Set(L"speedDial9", static_cast<LPCTSTR>(value));

    control = GetDlgItem(IDC_EDIT_SPEED_DIAL_10);
    control->GetWindowTextW(value);
    Util::Properties::Set(L"speedDial10", static_cast<LPCTSTR>(value));

    control = GetDlgItem(IDC_EDIT_SPEED_DIAL_11);
    control->GetWindowTextW(value);
    Util::Properties::Set(L"speedDial11", static_cast<LPCTSTR>(value));

    control = GetDlgItem(IDC_EDIT_SPEED_DIAL_12);
    control->GetWindowTextW(value);
    Util::Properties::Set(L"speedDial12", static_cast<LPCTSTR>(value));

    ShowWindow(SW_HIDE);
}

HBRUSH TelephoneSettingDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) {
	//CEdit* e;
	static bool isFirst = true;
    HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	RECT bounds;
	pWnd->GetWindowRect(&bounds);
	std::wstring backgroundImageFilename = L"/FlashDrv/debug/NULL.JPG";
	SIZE backgroundImageSize;
	backgroundImageSize.cx = 800;
	backgroundImageSize.cy = 480;
	CString boundsInfo;
    switch (nCtlColor) {
        case CTLCOLOR_BTN:
            break;
        case CTLCOLOR_STATIC:
			hbr = BackgroundImage::setStaticProperties(pDC->m_hDC, bounds, Util::ImageOp::GetImage(backgroundImageFilename, backgroundImageSize), font_, RGB(255, 255, 255));
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

void TelephoneSettingDialog::OnBnClickedButtonCall() {
    callForm->SetListener(this);
    callForm->ShowWindow(SW_SHOW);
}

void TelephoneSettingDialog::OnBnClickedButtonVoiceMessages() {
    std::vector<Util::shared_ptr<CallInfo> > callInfos = CallInfo::GetAllNotPlayedLeaveWord();
    if (!callInfos.empty()) {
        playingRecordsForm->SetCallInfos(callInfos);
        playingRecordsForm->ShowWindow(SW_SHOW);
    }
}

namespace {
    std::vector<std::wstring> filelist;
    class FileLister : public Util::ProcessFileProcedure {
    public:
        FileLister(std::wstring const& filepath)
        : ProcessFileProcedure(filepath) {
            filelist.clear();
        }
        void FileLister::operator()(WIN32_FIND_DATA const& FindFileData) {
            filelist.push_back(FindFileData.cFileName);
        }
    };
}

BOOL TelephoneSettingDialog::OnInitDialog() {
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

	//font_.CreateFont(20, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"ËÎÌו");
	font_.CreatePointFont(Screen::fontPoint, L"ËÎÌו");
	GetDlgItem(IDC_EDIT_THIS_TELEPHONE_NUMBER)->SetFont(&font_, FALSE);
	GetDlgItem(IDC_EDIT_LOCAL_AREA_CODE)->SetFont(&font_, FALSE);
	GetDlgItem(IDC_EDIT_OUTLINE_PREFIX)->SetFont(&font_, FALSE);
	GetDlgItem(IDC_EDIT_IP_PREFIX)->SetFont(&font_, FALSE);
	GetDlgItem(IDC_COMBO_DEFAULT_RING)->SetFont(&font_, FALSE);
	GetDlgItem(IDC_EDIT_DELAY_COUNT)->SetFont(&font_, FALSE);
	GetDlgItem(IDC_CHECK_FORCE_AUTO_RECORD)->SetFont(&font_, FALSE);
	GetDlgItem(IDC_RADIO_NULL)->SetFont(&font_, FALSE);
	GetDlgItem(IDC_RADIO_BLACKLIST)->SetFont(&font_, FALSE);
	GetDlgItem(IDC_RADIO_NOT_IN_CONTACTS)->SetFont(&font_, FALSE);
	GetDlgItem(IDC_RADIO_NOT_VIP)->SetFont(&font_, FALSE);
	GetDlgItem(IDC_RADIO_ALL)->SetFont(&font_, FALSE);
	GetDlgItem(IDC_EDIT_SPEED_DIAL_1)->SetFont(&font_, FALSE);
	GetDlgItem(IDC_EDIT_SPEED_DIAL_2)->SetFont(&font_, FALSE);
	GetDlgItem(IDC_EDIT_SPEED_DIAL_3)->SetFont(&font_, FALSE);
	GetDlgItem(IDC_EDIT_SPEED_DIAL_4)->SetFont(&font_, FALSE);
	GetDlgItem(IDC_EDIT_SPEED_DIAL_5)->SetFont(&font_, FALSE);
	GetDlgItem(IDC_EDIT_SPEED_DIAL_6)->SetFont(&font_, FALSE);
	GetDlgItem(IDC_EDIT_SPEED_DIAL_7)->SetFont(&font_, FALSE);
	GetDlgItem(IDC_EDIT_SPEED_DIAL_8)->SetFont(&font_, FALSE);
	GetDlgItem(IDC_EDIT_SPEED_DIAL_9)->SetFont(&font_, FALSE);
	GetDlgItem(IDC_EDIT_SPEED_DIAL_10)->SetFont(&font_, FALSE);
	GetDlgItem(IDC_EDIT_SPEED_DIAL_11)->SetFont(&font_, FALSE);
	GetDlgItem(IDC_EDIT_SPEED_DIAL_12)->SetFont(&font_, FALSE);
	GetDlgItem(IDC_EDIT_SPEED_DIAL_13)->SetFont(&font_, FALSE);
	GetDlgItem(IDC_EDIT_SPEED_DIAL_14)->SetFont(&font_, FALSE);
	GetDlgItem(IDC_EDIT_SPEED_DIAL_15)->SetFont(&font_, FALSE);

	CComboBox* control = static_cast<CComboBox*>(GetDlgItem(IDC_COMBO_DEFAULT_RING));
    std::wstring ringtone = StoragePath::GetFlashDrvPath() +  L"/" + StoragePath::GetRingtonePath();
    Util::ProcessFiles(L"*.*", FileLister(ringtone));
    for (size_t i = 0; i < filelist.size(); ++i) {
        control->AddString(filelist[i].c_str());
    }
    control->SetWindowTextW(Util::Properties::Get(L"defaultRingtone").c_str());

    ShowConfig();

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void TelephoneSettingDialog::OnEnSetfocusEditThisTelephoneNumber() {
    SipShowIM(SIPF_ON);
}

void TelephoneSettingDialog::OnEnKillfocusEditThisTelephoneNumber() {
    SipShowIM(SIPF_OFF);
}

void TelephoneSettingDialog::OnEnSetfocusEditLocalAreaCode() {
    SipShowIM(SIPF_ON);
}

void TelephoneSettingDialog::OnEnKillfocusEditLocalAreaCode() {
    SipShowIM(SIPF_OFF);
}

void TelephoneSettingDialog::OnEnSetfocusEditOutlinePrefix() {
    SipShowIM(SIPF_ON);
}

void TelephoneSettingDialog::OnEnKillfocusEditOutlinePrefix() {
    SipShowIM(SIPF_OFF);
}

void TelephoneSettingDialog::OnEnSetfocusEditIpPrefix() {
    SipShowIM(SIPF_ON);
}

void TelephoneSettingDialog::OnEnKillfocusEditIpPrefix() {
    SipShowIM(SIPF_OFF);
}

void TelephoneSettingDialog::OnEnSetfocusEditDuration() {
    SipShowIM(SIPF_ON);
}

void TelephoneSettingDialog::OnEnKillfocusEditDuration() {
    SipShowIM(SIPF_OFF);
}

void TelephoneSettingDialog::OnEnSetfocusEditSpeedDial1() {
    SipShowIM(SIPF_ON);
}

void TelephoneSettingDialog::OnEnKillfocusEditSpeedDial1() {
    SipShowIM(SIPF_OFF);
}

void TelephoneSettingDialog::OnEnSetfocusEditSpeedDial2() {
    SipShowIM(SIPF_ON);
}

void TelephoneSettingDialog::OnEnKillfocusEditSpeedDial2() {
    SipShowIM(SIPF_OFF);
}

void TelephoneSettingDialog::OnEnSetfocusEditSpeedDial3() {
    SipShowIM(SIPF_ON);
}

void TelephoneSettingDialog::OnEnKillfocusEditSpeedDial3() {
    SipShowIM(SIPF_OFF);
}

void TelephoneSettingDialog::OnEnSetfocusEditSpeedDial4() {
    SipShowIM(SIPF_ON);
}

void TelephoneSettingDialog::OnEnKillfocusEditSpeedDial4() {
    SipShowIM(SIPF_OFF);
}

void TelephoneSettingDialog::OnEnSetfocusEditSpeedDial5() {
    SipShowIM(SIPF_ON);
}

void TelephoneSettingDialog::OnEnKillfocusEditSpeedDial5() {
    SipShowIM(SIPF_OFF);
}

void TelephoneSettingDialog::OnEnSetfocusEditSpeedDial6() {
    SipShowIM(SIPF_ON);
}

void TelephoneSettingDialog::OnEnKillfocusEditSpeedDial6() {
    SipShowIM(SIPF_OFF);
}

void TelephoneSettingDialog::OnEnSetfocusEditSpeedDial7() {
    SipShowIM(SIPF_ON);
}

void TelephoneSettingDialog::OnEnKillfocusEditSpeedDial7() {
    SipShowIM(SIPF_OFF);
}

void TelephoneSettingDialog::OnEnSetfocusEditSpeedDial8() {
    SipShowIM(SIPF_ON);
}

void TelephoneSettingDialog::OnEnKillfocusEditSpeedDial8() {
    SipShowIM(SIPF_OFF);
}

void TelephoneSettingDialog::OnEnSetfocusEditSpeedDial9() {
    SipShowIM(SIPF_ON);
}

void TelephoneSettingDialog::OnEnKillfocusEditSpeedDial9() {
    SipShowIM(SIPF_OFF);
}

void TelephoneSettingDialog::OnEnSetfocusEditSpeedDial10() {
    SipShowIM(SIPF_ON);
}

void TelephoneSettingDialog::OnEnKillfocusEditSpeedDial10() {
    SipShowIM(SIPF_OFF);
}

void TelephoneSettingDialog::OnEnSetfocusEditSpeedDial11() {
    SipShowIM(SIPF_ON);
}

void TelephoneSettingDialog::OnEnKillfocusEditSpeedDial11() {
    SipShowIM(SIPF_OFF);
}

void TelephoneSettingDialog::OnEnSetfocusEditSpeedDial12() {
    SipShowIM(SIPF_ON);
}

void TelephoneSettingDialog::OnEnKillfocusEditSpeedDial12() {
    SipShowIM(SIPF_OFF);
}

void TelephoneSettingDialog::ClearAllFirewall() {
    //CButton* control = static_cast<CButton*>(GetDlgItem(IDC_RADIO_NULL));
    //control->SetCheck(BST_UNCHECKED);
    CButton* control = static_cast<CButton*>(GetDlgItem(IDC_RADIO_BLACKLIST));
    control->SetCheck(BST_UNCHECKED);
    control = static_cast<CButton*>(GetDlgItem(IDC_RADIO_NOT_IN_CONTACTS));
    control->SetCheck(BST_UNCHECKED);
    control = static_cast<CButton*>(GetDlgItem(IDC_RADIO_NOT_VIP));
    control->SetCheck(BST_UNCHECKED);
    control = static_cast<CButton*>(GetDlgItem(IDC_RADIO_ALL));
    control->SetCheck(BST_UNCHECKED);
    CWnd* time = GetDlgItem(IDC_EDIT_DURATION);
    (static_cast<CEdit*>(time))->SetReadOnly(true);
}

void TelephoneSettingDialog::OnBnClickedRadioNull() {
    ClearAllFirewall();
    CButton* control = static_cast<CButton*>(GetDlgItem(IDC_RADIO_NULL));
    control->SetCheck(BST_CHECKED);
    firewallType_ = ftNull;
}

void TelephoneSettingDialog::OnBnClickedRadioBlacklist() {
    ClearAllFirewall();
    CButton* control = static_cast<CButton*>(GetDlgItem(IDC_RADIO_BLACKLIST));
    control->SetCheck(BST_CHECKED);
    firewallType_ = ftBlacklist;
}

void TelephoneSettingDialog::OnBnClickedRadioNotInContacts() {
    ClearAllFirewall();
    CButton* control = static_cast<CButton*>(GetDlgItem(IDC_RADIO_NOT_IN_CONTACTS));
    control->SetCheck(BST_CHECKED);
    firewallType_ = ftNotInContacts;
}

void TelephoneSettingDialog::OnBnClickedRadioNotVip() {
    ClearAllFirewall();
    CButton* control = static_cast<CButton*>(GetDlgItem(IDC_RADIO_NOT_VIP));
    control->SetCheck(BST_CHECKED);
    firewallType_ = ftNotVIP;
}

void TelephoneSettingDialog::OnBnClickedRadioAll() {
    ClearAllFirewall();
    CButton* control = static_cast<CButton*>(GetDlgItem(IDC_RADIO_ALL));
    control->SetCheck(BST_CHECKED);
    firewallType_ = ftAll;
    CWnd* time = GetDlgItem(IDC_EDIT_DURATION);
    (static_cast<CEdit*>(time))->SetReadOnly(false);
    time->SetFocus();
}

LRESULT TelephoneSettingDialog::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) {
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

BOOL TelephoneSettingDialog::OnEraseBkgnd(CDC* pDC) {
	BOOL r = BackgroundImage::drawBackground(m_hWnd);
	if (!r) {
        DWORD errorCode = GetLastError();
        DWORD ec = errorCode;
	}
	//return CDialog::OnEraseBkgnd(pDC);
	return TRUE;
}
