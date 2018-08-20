// PlayingRecordsDialog.cpp : implementation file
//

#include "stdafx.h"
#include "RecordPhone.h"
#include "PlayingRecordsDialog.h"
#include "CallDialog.h"
#include "PlayingRecordsDialog.h"
#include "Telephone.h"
#include "BackgroundImage.h"
#include "WavePlayer.h"
#include "Util/ImageOp.h"

PlayingRecordsDialog* playingRecordsForm = 0;

// PlayingRecordsDialog dialog

IMPLEMENT_DYNAMIC(PlayingRecordsDialog, CDialog)

PlayingRecordsDialog::PlayingRecordsDialog(CWnd* pParent /*=NULL*/)
: CDialog(PlayingRecordsDialog::IDD, pParent)
, isFinally_(false) {
}

PlayingRecordsDialog::~PlayingRecordsDialog() {
}

void PlayingRecordsDialog::DoDataExchange(CDataExchange* pDX) {
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(PlayingRecordsDialog, CDialog)
    ON_BN_CLICKED(IDC_BUTTON_CLOSE, &PlayingRecordsDialog::OnBnClickedButtonClose)
    ON_WM_CTLCOLOR()
    ON_BN_CLICKED(IDC_BUTTON_CALL, &PlayingRecordsDialog::OnBnClickedButtonCall)
    ON_BN_CLICKED(IDC_BUTTON_VOICE_MESSAGES, &PlayingRecordsDialog::OnBnClickedButtonVoiceMessages)
	ON_WM_ERASEBKGND()
    ON_BN_CLICKED(IDC_BUTTON_DELETE_CURRENT, &PlayingRecordsDialog::OnBnClickedButtonDeleteCurrent)
    ON_BN_CLICKED(IDC_BUTTON_RESUME, &PlayingRecordsDialog::OnBnClickedButtonResume)
    ON_BN_CLICKED(IDC_BUTTON_PAUSE, &PlayingRecordsDialog::OnBnClickedButtonPause)
    ON_BN_CLICKED(IDC_BUTTON_PREVIOUS, &PlayingRecordsDialog::OnBnClickedButtonPrevious)
    ON_BN_CLICKED(IDC_BUTTON_NEXT, &PlayingRecordsDialog::OnBnClickedButtonNext)
END_MESSAGE_MAP()

void PlayingRecordsDialog::next_() {
    ++currentIndex_;
    if (currentIndex_ >= soundSegments_.size()) {
        currentIndex_ = 0;
        isFinally_ = true;
    }
}

void PlayingRecordsDialog::previous_() {
    --currentIndex_;
    if (currentIndex_ < 0) {
        currentIndex_ = soundSegments_.size() - 1;
    }
}

void PlayingRecordsDialog::PlayNext() {
    next_();
    if (isFinally_) {
        return;
    }
    WavePlayer::Instance()->Start(soundSegments_[currentIndex_]);
}

namespace {
    void PlayEnd() {
        playingRecordsForm->PlayNext();
    }
}

void PlayingRecordsDialog::StartPlay() {
    if (!soundSegments_.empty()) {
        WavePlayer::Instance()->SetStopNotify(PlayEnd);
        WavePlayer::Instance()->Start(soundSegments_[currentIndex_]);
    }
}

// PlayingRecordsDialog message handlers
void PlayingRecordsDialog::SetCallInfos(std::vector<Util::shared_ptr<CallInfo> > const& callInfos) {
    bool hasCall = Telephone::Instance()->HasCall();
    if (hasCall) {
        CWnd* control = GetDlgItem(IDC_BUTTON_CALL);
        control->ShowWindow(SW_SHOW);
    }

    for (size_t i = 0; i < callInfos.size(); ++i) {
        std::vector<Util::shared_ptr<SoundSegment> > sounds = callInfos[i]->sounds();
        for (size_t j = 0; j < sounds.size(); ++j) {
            soundSegments_.push_back(sounds[j]->filename());
        }
    }
    currentIndex_ = 0;

    StartPlay();
}

void PlayingRecordsDialog::SetCallInfo(Util::shared_ptr<CallInfo> const& callInfo) {
    bool hasCall = Telephone::Instance()->HasCall();
    if (hasCall) {
        CWnd* control = GetDlgItem(IDC_BUTTON_CALL);
        control->ShowWindow(SW_SHOW);
    }

	CWnd* control = GetDlgItem(IDC_EDIT_NAME);
	control->SetWindowTextW(callInfo->name().c_str());
	control = GetDlgItem(IDC_EDIT_NUMBER);
	control->SetWindowTextW(callInfo->telephoneNumber().c_str());
	control = GetDlgItem(IDC_DATETIMEPICKER_DATE);
	static_cast<CDateTimeCtrl*>(control)->SetTime(&callInfo->startTime());
	control = GetDlgItem(IDC_DATETIMEPICKER_TIME);
	static_cast<CDateTimeCtrl*>(control)->SetTime(&callInfo->startTime());
	control = GetDlgItem(IDC_EDIT_DURATION);
	control->SetWindowTextW(Util::StringOp::FromInt(callInfo->duration().GetTotalSeconds()).c_str());

	control = GetDlgItem(IDC_PROGRESS_PLAY);
	static_cast<CProgressCtrl*>(control)->SetPos(5);

    std::vector<Util::shared_ptr<SoundSegment> > sounds = callInfo->sounds();
    for (size_t j = 0; j < sounds.size(); ++j) {
        soundSegments_.push_back(sounds[j]->filename());
    }
    currentIndex_ = 0;

    StartPlay();
}

void PlayingRecordsDialog::OnBnClickedButtonClose() {
    // TODO: Add your control notification handler code here
    ShowWindow(SW_HIDE);
    //OnOK();
    //DestroyWindow();
    //std::vector<Util::shared_ptr<SoundSegment> > sounds = callInfos_[no]->sounds();
    //for (size_t i = 0; i < sounds.size(); ++i) {
    //    sounds[i]->Play();
    //}
}

HBRUSH PlayingRecordsDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) {
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

void PlayingRecordsDialog::OnBnClickedButtonCall() {
    callForm->SetListener(this);
    callForm->ShowWindow(SW_SHOW);
}

void PlayingRecordsDialog::OnBnClickedButtonVoiceMessages() {
    std::vector<Util::shared_ptr<CallInfo> > callInfos = CallInfo::GetAllNotPlayedLeaveWord();
    if (!callInfos.empty()) {
        playingRecordsForm->SetCallInfos(callInfos);
        playingRecordsForm->ShowWindow(SW_SHOW);
    }
}

BOOL PlayingRecordsDialog::OnInitDialog() {
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
	SIZE navButtonSize;
	navButtonSize.cx = 50;
	navButtonSize.cy = 50;
    maskFilename = L"/FlashDrv/debug/navMask.bmp";
	previousButton_.SetImage(L"/FlashDrv/debug/previous.jpg", navButtonSize, maskFilename);
	previousButton_.SubclassDlgItem(IDC_BUTTON_PREVIOUS, this);
	nextButton_.SetImage(L"/FlashDrv/debug/next.jpg", navButtonSize, maskFilename);
	nextButton_.SubclassDlgItem(IDC_BUTTON_NEXT, this);
	SIZE playControlButtonSize;
	playControlButtonSize.cx = 68;
	playControlButtonSize.cy = 68;
    maskFilename = L"/FlashDrv/debug/playMask.bmp";
	playButton_.SetImage(L"/FlashDrv/debug/play.jpg", playControlButtonSize, maskFilename);
    playButton_.SubclassDlgItem(IDC_BUTTON_RESUME, this);
	pauseButton_.SetImage(L"/FlashDrv/debug/pause.jpg", playControlButtonSize, maskFilename);
    pauseButton_.SubclassDlgItem(IDC_BUTTON_PAUSE, this);

	//font_.CreateFont(20, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"ËÎÌו");
	font_.CreatePointFont(Screen::fontPoint, L"ËÎÌו");
	GetDlgItem(IDC_EDIT_NAME)->SetFont(&font_, FALSE);
	GetDlgItem(IDC_EDIT_NUMBER)->SetFont(&font_, FALSE);
	GetDlgItem(IDC_EDIT_DURATION)->SetFont(&font_, FALSE);
	GetDlgItem(IDC_EDIT_CURRENT_AND_TOTAL)->SetFont(&font_, FALSE);
	GetDlgItem(IDC_DATETIMEPICKER_DATE)->SetFont(&font_, FALSE);
	GetDlgItem(IDC_DATETIMEPICKER_TIME)->SetFont(&font_, FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

LRESULT PlayingRecordsDialog::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) {
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

BOOL PlayingRecordsDialog::OnEraseBkgnd(CDC* pDC) {
	BOOL r = BackgroundImage::drawBackground(m_hWnd);
	if (!r) {
        DWORD errorCode = GetLastError();
        DWORD ec = errorCode;
	}
	//return CDialog::OnEraseBkgnd(pDC);
	return TRUE;
}

void PlayingRecordsDialog::OnBnClickedButtonDeleteCurrent() {
    WavePlayer::Instance()->Stop();
    soundSegments_.erase(std::remove(soundSegments_.begin(), soundSegments_.end(), soundSegments_[currentIndex_]), soundSegments_.end());
    next_();
    WavePlayer::Instance()->Start(soundSegments_[currentIndex_]);
}

void PlayingRecordsDialog::OnBnClickedButtonResume() {
    WavePlayer::Instance()->Resume();
    CWnd* control = GetDlgItem(IDC_BUTTON_PAUSE);
    control->ShowWindow(SW_SHOW);
    control = GetDlgItem(IDC_BUTTON_RESUME);
    control->ShowWindow(SW_HIDE);
}

void PlayingRecordsDialog::OnBnClickedButtonPause() {
    WavePlayer::Instance()->Pause();
    CWnd* control = GetDlgItem(IDC_BUTTON_PAUSE);
    control->ShowWindow(SW_HIDE);
    control = GetDlgItem(IDC_BUTTON_RESUME);
    control->ShowWindow(SW_SHOW);
}

void PlayingRecordsDialog::OnBnClickedButtonPrevious() {
    WavePlayer::Instance()->Stop();
    previous_();
    WavePlayer::Instance()->Start(soundSegments_[currentIndex_]);
}

void PlayingRecordsDialog::OnBnClickedButtonNext() {
    WavePlayer::Instance()->Stop();
    next_();
    WavePlayer::Instance()->Start(soundSegments_[currentIndex_]);
}
