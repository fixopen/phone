#pragma once
#include "ContactInfo.h"
#include "ImageButton.h"

// PlayingRecordsDialog dialog

class PlayingRecordsDialog : public CDialog {
	DECLARE_DYNAMIC(PlayingRecordsDialog)

public:
	PlayingRecordsDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~PlayingRecordsDialog();

// Dialog Data
	enum { IDD = IDD_DIALOG_PLAYING_RECORDS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
    std::vector<Util::shared_ptr<CallInfo> > callInfos_;
    std::vector<Util::shared_ptr<CallInfo> >::iterator currentCallInfo_;
    std::vector<Util::shared_ptr<SoundSegment> >::iterator currentSoundSegment_;
	CFont font_;
	ImageButton closeButton_;
	ImageButton callButton_;
	ImageButton voiceMessagesButton_;
    ImageButton previousButton_;
    ImageButton nextButton_;
    ImageButton playButton_;
    ImageButton pauseButton_;
public:
    void SetCallInfos(std::vector<Util::shared_ptr<CallInfo> > const& callInfos);
    void SetCallInfo(Util::shared_ptr<CallInfo> const& callInfo);
    void PlayNext();
    afx_msg void OnBnClickedButtonClose();
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    afx_msg void OnBnClickedButtonCall();
    afx_msg void OnBnClickedButtonVoiceMessages();
    virtual BOOL OnInitDialog();
protected:
    void next_();
    void previous_();
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnBnClickedButtonDeleteCurrent();
    afx_msg void OnBnClickedButtonResume();
    afx_msg void OnBnClickedButtonPause();
    afx_msg void OnBnClickedButtonPrevious();
    afx_msg void OnBnClickedButtonNext();
};

extern PlayingRecordsDialog* playingRecordsForm;