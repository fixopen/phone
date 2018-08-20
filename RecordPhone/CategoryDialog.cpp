// CategoryDialog.cpp : implementation file
//

#include "stdafx.h"

#include <sip.h>

#include "RecordPhone.h"
#include "CategoryDialog.h"
#include "SoundSegment.h"
#include "CallDialog.h"
#include "PlayingRecordsDialog.h"
#include "Telephone.h"
#include "BackgroundImage.h"
#include "Util/ImageOp.h"
#include "Util/Properties.h"
#include "Util/ScanDir.h"

CategoryDialog* categoryForm = 0;

// CategoryDialog dialog

IMPLEMENT_DYNAMIC(CategoryDialog, CDialog)

CategoryDialog::CategoryDialog(CWnd* pParent /*=NULL*/)
: CDialog(CategoryDialog::IDD, pParent)
, listener_(0) {
}

CategoryDialog::~CategoryDialog() {
}

void CategoryDialog::DoDataExchange(CDataExchange* pDX) {
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CategoryDialog, CDialog)
    ON_WM_DESTROY()
    ON_BN_CLICKED(IDC_BUTTON_CLOSE, &CategoryDialog::OnBnClickedButtonClose)
    ON_BN_CLICKED(IDC_BUTTON_DELETE, &CategoryDialog::OnBnClickedButtonDelete)
    ON_BN_CLICKED(IDC_BUTTON_PLAY_FAVORITE_RINGTONE, &CategoryDialog::OnBnClickedButtonPlayFavoriteRingtone)
    ON_BN_CLICKED(IDC_BUTTON_PLAY_PROMPT_MESSAGE, &CategoryDialog::OnBnClickedButtonPlayPromptMessage)
    ON_BN_CLICKED(IDC_BUTTON_PAUSE_FAVORITE_RINGTONE, &CategoryDialog::OnBnClickedButtonPauseFavoriteRingtone)
    ON_BN_CLICKED(IDC_BUTTON_RESUME_FAVORITE_RINGTONE, &CategoryDialog::OnBnClickedButtonResumeFavoriteRingtone)
    ON_BN_CLICKED(IDC_BUTTON_BREAK_FAVORITE_RINGTONE, &CategoryDialog::OnBnClickedButtonBreakFavoriteRingtone)
    ON_BN_CLICKED(IDC_BUTTON_PAUSE_PROMPT_MESSAGE, &CategoryDialog::OnBnClickedButtonPausePromptMessage)
    ON_BN_CLICKED(IDC_BUTTON_RESUME_PROMPT_MESSAGE, &CategoryDialog::OnBnClickedButtonResumePromptMessage)
    ON_BN_CLICKED(IDC_BUTTON_BREAK_PROMPT_MESSAGE, &CategoryDialog::OnBnClickedButtonBreakPromptMessage)
    ON_WM_CTLCOLOR()
    ON_BN_CLICKED(IDC_BUTTON_CALL, &CategoryDialog::OnBnClickedButtonCall)
    ON_BN_CLICKED(IDC_BUTTON_VOICE_MESSAGES, &CategoryDialog::OnBnClickedButtonVoiceMessages)
    ON_EN_SETFOCUS(IDC_EDIT_NAME, &CategoryDialog::OnEnSetfocusEditName)
    ON_EN_KILLFOCUS(IDC_EDIT_NAME, &CategoryDialog::OnEnKillfocusEditName)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CategoryDialog message handlers

void CategoryDialog::SetCategory(Util::shared_ptr<ContactCategory> const& category) {
    bool hasCall = Telephone::Instance()->HasCall();
    if (hasCall) {
        CWnd* control = GetDlgItem(IDC_BUTTON_CALL);
        control->ShowWindow(SW_SHOW);
    }

    category_ = category;
    CWnd* control = GetDlgItem(IDC_EDIT_NAME);
    control->SetWindowTextW(category_->name().c_str());

    control = GetDlgItem(IDC_COMBO_FAVORITE_RING);
    control->SetWindowTextW(category_->ringtoneName().c_str());

    control = GetDlgItem(IDC_COMBO_PROMPT_MESSAGE);
    control->SetWindowTextW(category_->promptName().c_str());
}

void CategoryDialog::SyncCategory() {
    CString value;
    CWnd* control = GetDlgItem(IDC_EDIT_NAME);
    control->GetWindowTextW(value);
    category_->name(static_cast<LPCTSTR>(value));

    control = GetDlgItem(IDC_COMBO_FAVORITE_RING);
    control->GetWindowTextW(value);
    category_->ringtoneName(static_cast<LPCTSTR>(value));

    control = GetDlgItem(IDC_COMBO_PROMPT_MESSAGE);
    control->GetWindowTextW(value);
    category_->promptName(static_cast<LPCTSTR>(value));
}

Util::shared_ptr<ContactCategory> const CategoryDialog::GetCategory() const {
    return category_;
}

void CategoryDialog::OnDestroy() {
    CDialog::OnDestroy();
    MessageBox(L"CategoryDialog::Destroy");
}

void CategoryDialog::OnBnClickedButtonClose() {
    // TODO: Add your control notification handler code here
    SyncCategory();
    m_pParentWnd->PostMessage(UM_CATEGORY_UPDATE_FINALLY);
    ShowWindow(SW_HIDE);
    //OnOK();
    //DestroyWindow();
}

void CategoryDialog::OnBnClickedButtonDelete() {
    // TODO: Add your control notification handler code here
    category_->Remove();
    std::vector<Util::shared_ptr<ContactCategory> > categories = ContactCategory::All();
    categories.erase(std::remove(categories.begin(), categories.end(), category_), categories.end());
    //category_.reset();
    m_pParentWnd->PostMessage(UM_CATEGORY_DELETE_FINALLY);
    ShowWindow(SW_HIDE);
    //rebind();
}

void CategoryDialog::OnBnClickedButtonPlayFavoriteRingtone() {
    // TODO: Add your control notification handler code here
    CWnd* control = GetDlgItem(IDC_COMBO_FAVORITE_RING);
    CString filename;
    control->GetWindowTextW(filename);
    SoundSegment(sstRingtone, static_cast<LPCTSTR>(filename)).Play();
}

void CategoryDialog::OnBnClickedButtonPauseFavoriteRingtone() {
    // TODO: Add your control notification handler code here
    CWnd* control = GetDlgItem(IDC_COMBO_FAVORITE_RING);
    CString filename;
    control->GetWindowTextW(filename);
    SoundSegment(sstRingtone, static_cast<LPCTSTR>(filename)).PausePlay();
}

void CategoryDialog::OnBnClickedButtonResumeFavoriteRingtone() {
    // TODO: Add your control notification handler code here
    CWnd* control = GetDlgItem(IDC_COMBO_FAVORITE_RING);
    CString filename;
    control->GetWindowTextW(filename);
    SoundSegment(sstRingtone, static_cast<LPCTSTR>(filename)).ResumePlay();
}

void CategoryDialog::OnBnClickedButtonBreakFavoriteRingtone() {
    // TODO: Add your control notification handler code here
    CWnd* control = GetDlgItem(IDC_COMBO_FAVORITE_RING);
    CString filename;
    control->GetWindowTextW(filename);
    SoundSegment(sstRingtone, static_cast<LPCTSTR>(filename)).BreakPlay();
}

void CategoryDialog::OnBnClickedButtonPlayPromptMessage() {
    // TODO: Add your control notification handler code here
    CWnd* control = GetDlgItem(IDC_COMBO_PROMPT_MESSAGE);
    CString filename;
    control->GetWindowTextW(filename);
    SoundSegment(sstRingtone, static_cast<LPCTSTR>(filename)).Play();
}

void CategoryDialog::OnBnClickedButtonPausePromptMessage() {
    // TODO: Add your control notification handler code here
    CWnd* control = GetDlgItem(IDC_COMBO_PROMPT_MESSAGE);
    CString filename;
    control->GetWindowTextW(filename);
    SoundSegment(sstRingtone, static_cast<LPCTSTR>(filename)).PausePlay();
}

void CategoryDialog::OnBnClickedButtonResumePromptMessage() {
    // TODO: Add your control notification handler code here
    CWnd* control = GetDlgItem(IDC_COMBO_PROMPT_MESSAGE);
    CString filename;
    control->GetWindowTextW(filename);
    SoundSegment(sstRingtone, static_cast<LPCTSTR>(filename)).ResumePlay();
}

void CategoryDialog::OnBnClickedButtonBreakPromptMessage() {
    // TODO: Add your control notification handler code here
    CWnd* control = GetDlgItem(IDC_COMBO_PROMPT_MESSAGE);
    CString filename;
    control->GetWindowTextW(filename);
    SoundSegment(sstRingtone, static_cast<LPCTSTR>(filename)).BreakPlay();
}

HBRUSH CategoryDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) {
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

void CategoryDialog::OnBnClickedButtonCall() {
    callForm->SetListener(this);
    callForm->ShowWindow(SW_SHOW);
}

void CategoryDialog::OnBnClickedButtonVoiceMessages() {
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

BOOL CategoryDialog::OnInitDialog() {
    CDialog::OnInitDialog();

    // TODO:  Add extra initialization here
    CComboBox* control = static_cast<CComboBox*>(GetDlgItem(IDC_COMBO_FAVORITE_RING));
    std::wstring ringtone = StoragePath::GetFlashDrvPath() +  L"/" + StoragePath::GetRingtonePath();
    Util::ProcessFiles(L"*.*", FileLister(ringtone));
    for (size_t i = 0; i < filelist.size(); ++i) {
        control->AddString(filelist[i].c_str());
    }
    control->SetWindowTextW(Util::Properties::Get(L"defaultRingtone").c_str());

    control = static_cast<CComboBox*>(GetDlgItem(IDC_COMBO_PROMPT_MESSAGE));
    std::wstring prompt = StoragePath::GetFlashDrvPath() +  L"/" + StoragePath::GetPromptPath();
    Util::ProcessFiles(L"*.*", FileLister(prompt));
    for (size_t i = 0; i < filelist.size(); ++i) {
        control->AddString(filelist[i].c_str());
    }
    control->SetWindowTextW(Util::Properties::Get(L"defaultPrompt").c_str());

	SIZE buttonSize;
	buttonSize.cx = 48;
	buttonSize.cy = 48;
	std::wstring maskFilename = L"/FlashDrv/debug/buttonMask.bmp";
	closeButton_.SetImage(L"/FlashDrv/debug/close.jpg", buttonSize, maskFilename);
	closeButton_.SubclassDlgItem(IDC_BUTTON_CLOSE, this);
	deleteButton_.SetImage(L"/FlashDrv/debug/clear.jpg", buttonSize, maskFilename);
	deleteButton_.SubclassDlgItem(IDC_BUTTON_DELETE, this);
	callButton_.SetImage(L"/FlashDrv/debug/call.jpg", buttonSize, maskFilename);
	callButton_.SubclassDlgItem(IDC_BUTTON_CALL, this);
	voiceMessagesButton_.SetImage(L"/FlashDrv/debug/message.jpg", buttonSize, maskFilename);
	voiceMessagesButton_.SubclassDlgItem(IDC_BUTTON_VOICE_MESSAGES, this);

	//font_.CreateFont(20, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"ËÎÌו");
	font_.CreatePointFont(Screen::fontPoint, L"ËÎÌו");
	GetDlgItem(IDC_EDIT_NAME)->SetFont(&font_, FALSE);
	GetDlgItem(IDC_COMBO_FAVORITE_RING)->SetFont(&font_, FALSE);
	GetDlgItem(IDC_COMBO_PROMPT_MESSAGE)->SetFont(&font_, FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CategoryDialog::OnEnSetfocusEditName() {
    SipShowIM(SIPF_ON);
}

void CategoryDialog::OnEnKillfocusEditName() {
    SipShowIM(SIPF_OFF);
}

LRESULT CategoryDialog::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) {
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

BOOL CategoryDialog::OnEraseBkgnd(CDC* pDC) {
	BOOL r = BackgroundImage::drawBackground(m_hWnd);
	if (!r) {
        DWORD errorCode = GetLastError();
        DWORD ec = errorCode;
	}
	//return CDialog::OnEraseBkgnd(pDC);
	return TRUE;
}
