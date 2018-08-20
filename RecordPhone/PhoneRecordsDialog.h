#pragma once
#include "CallInfo.h"
#include "ImageButton.h"

// PhoneRecordsDialog dialog

class PhoneRecordsDialog : public CDialog {
	DECLARE_DYNAMIC(PhoneRecordsDialog)

public:
	PhoneRecordsDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~PhoneRecordsDialog();

// Dialog Data
	enum { IDD = IDD_DIALOG_PHONE_RECORDS };
    static size_t const pageSize = 10;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
    std::vector<Util::shared_ptr<CallInfo> > callInfos_;
    std::wstring filter_;
    size_t offset_;
	CFont font_;
	ImageButton closeButton_;
	ImageButton pageDownButton_;
	ImageButton pageUpButton_;
	ImageButton clearButton_;
	ImageButton playMessagesButton_;
	ImageButton callButton_;
	ImageButton voiceMessagesButton_;
	ImageButton findButton_;
	int deleteIndex_;
    void PlayRecord(int const no);
    void Call(int const no);
    void Delete(int const no);
    std::wstring const FormatTime(Util::Timestamp const& time) const;
	void hideCallInfo_(int const typeId, int const nameId, int const numberId, int const durationId, int const startTimeId, int const hasRecordId, int const callId, int const deleteId);
	void showCallInfo_(int const no, int const typeId, int const nameId, int const numberId, int const durationId, int const startTimeId, int const hasRecordId, int const callId, int const deleteId);
	void showContactByName_(int const nameId);
public:
    void SetCallInfos(std::vector<Util::shared_ptr<CallInfo> > const& callInfos);
    afx_msg void OnBnClickedButtonClose();
    afx_msg void OnBnClickedButtonPageDown();
    afx_msg void OnBnClickedButtonPageUp();
    afx_msg void OnBnClickedButtonPlayMessages();
    afx_msg void OnBnClickedButtonClear();
    afx_msg void OnBnClickedButtonFind();
    afx_msg void OnStnClickedStaticHasRecord0();
    afx_msg void OnStnClickedStaticCall0();
    afx_msg void OnStnClickedStaticDelete0();
    afx_msg void OnStnClickedStaticHasRecord1();
    afx_msg void OnStnClickedStaticCall1();
    afx_msg void OnStnClickedStaticDelete1();
    afx_msg void OnStnClickedStaticHasRecord2();
    afx_msg void OnStnClickedStaticCall2();
    afx_msg void OnStnClickedStaticDelete2();
    afx_msg void OnStnClickedStaticHasRecord3();
    afx_msg void OnStnClickedStaticCall3();
    afx_msg void OnStnClickedStaticDelete3();
    afx_msg void OnStnClickedStaticHasRecord4();
    afx_msg void OnStnClickedStaticCall4();
    afx_msg void OnStnClickedStaticDelete4();
    afx_msg void OnStnClickedStaticHasRecord5();
    afx_msg void OnStnClickedStaticCall5();
    afx_msg void OnStnClickedStaticDelete5();
    afx_msg void OnStnClickedStaticHasRecord6();
    afx_msg void OnStnClickedStaticCall6();
    afx_msg void OnStnClickedStaticDelete6();
    afx_msg void OnStnClickedStaticHasRecord7();
    afx_msg void OnStnClickedStaticCall7();
    afx_msg void OnStnClickedStaticDelete7();
    afx_msg void OnStnClickedStaticHasRecord8();
    afx_msg void OnStnClickedStaticCall8();
    afx_msg void OnStnClickedStaticDelete8();
    afx_msg void OnStnClickedStaticHasRecord9();
    afx_msg void OnStnClickedStaticCall9();
    afx_msg void OnStnClickedStaticDelete9();
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    afx_msg void OnBnClickedButtonCall();
    afx_msg void OnBnClickedButtonVoiceMessages();
    virtual BOOL OnInitDialog();
    afx_msg void OnEnSetfocusEditName();
    afx_msg void OnEnKillfocusEditName();
    afx_msg void OnEnSetfocusEditNumber();
    afx_msg void OnEnKillfocusEditNumber();
protected:
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnEnUpdateEditName();
	afx_msg void OnEnUpdateEditNumber();
	afx_msg void OnEnUpdateEditStartTime();
	afx_msg void OnEnUpdateEditEndTime();
	afx_msg void OnStnClickedStaticCallName0();
	afx_msg void OnStnClickedStaticCallName1();
	afx_msg void OnStnClickedStaticCallName2();
	afx_msg void OnStnClickedStaticCallName3();
	afx_msg void OnStnClickedStaticCallName4();
	afx_msg void OnStnClickedStaticCallName5();
	afx_msg void OnStnClickedStaticCallName6();
	afx_msg void OnStnClickedStaticCallName7();
	afx_msg void OnStnClickedStaticCallName8();
	afx_msg void OnStnClickedStaticCallName9();
	afx_msg void OnEnSetfocusEditStartTime();
	afx_msg void OnEnSetfocusEditEndTime();
	afx_msg void OnEnKillfocusEditStartTime();
	afx_msg void OnEnKillfocusEditEndTime();
};

extern PhoneRecordsDialog* phoneRecordsForm;
