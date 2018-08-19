#pragma once
#include "ContactCategory.h"
#include "ImageButton.h"

// CCategoryDialog dialog

class CategoryDialog : public CDialog {
	DECLARE_DYNAMIC(CategoryDialog)

public:
	CategoryDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CategoryDialog();
    void SetListener(CWnd* listener) {
        listener_ = listener;
    }

// Dialog Data
	enum { IDD = IDD_DIALOG_CATEGORY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
    Util::shared_ptr<ContactCategory> category_;
    CWnd* listener_;
	CFont font_;
    //Util::shared_ptr<SoundSegment> currentSound_;
	ImageButton closeButton_;
	ImageButton deleteButton_;
	ImageButton callButton_;
	ImageButton voiceMessagesButton_;
public:
    void SetCategory(Util::shared_ptr<ContactCategory> const& category);
    void SyncCategory();
    Util::shared_ptr<ContactCategory> const GetCategory() const;
    afx_msg void OnDestroy();
    afx_msg void OnBnClickedButtonClose();
    afx_msg void OnBnClickedButtonDelete();
    afx_msg void OnBnClickedButtonPlayFavoriteRingtone();
    afx_msg void OnBnClickedButtonPauseFavoriteRingtone();
    afx_msg void OnBnClickedButtonResumeFavoriteRingtone();
    afx_msg void OnBnClickedButtonBreakFavoriteRingtone();
    afx_msg void OnBnClickedButtonPlayPromptMessage();
    afx_msg void OnBnClickedButtonPausePromptMessage();
    afx_msg void OnBnClickedButtonResumePromptMessage();
    afx_msg void OnBnClickedButtonBreakPromptMessage();
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    afx_msg void OnBnClickedButtonCall();
    afx_msg void OnBnClickedButtonVoiceMessages();
    virtual BOOL OnInitDialog();
    afx_msg void OnEnSetfocusEditName();
    afx_msg void OnEnKillfocusEditName();
protected:
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};

extern CategoryDialog* categoryForm;