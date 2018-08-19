// PhoneRecordsDialog.cpp : implementation file
//

#include "stdafx.h"

#include <sip.h>

#include "RecordPhone.h"
#include "PhoneRecordsDialog.h"
#include "PlayingRecordsDialog.h"
#include "CallDialog.h"
#include "ContactInfo.h"
#include "Telephone.h"
#include "CallDialog.h"
#include "PlayingRecordsDialog.h"
#include "BackgroundImage.h"
#include "Util/ImageOp.h"
#include "Util/TimeOp.h"

PhoneRecordsDialog* phoneRecordsForm = 0;

// PhoneRecordsDialog dialog

IMPLEMENT_DYNAMIC(PhoneRecordsDialog, CDialog)

PhoneRecordsDialog::PhoneRecordsDialog(CWnd* pParent /*=NULL*/)
: CDialog(PhoneRecordsDialog::IDD, pParent) {
}

PhoneRecordsDialog::~PhoneRecordsDialog() {
}

void PhoneRecordsDialog::DoDataExchange(CDataExchange* pDX) {
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(PhoneRecordsDialog, CDialog)
    ON_BN_CLICKED(IDC_BUTTON_CLOSE, &PhoneRecordsDialog::OnBnClickedButtonClose)
    ON_BN_CLICKED(IDC_BUTTON_PAGE_DOWN, &PhoneRecordsDialog::OnBnClickedButtonPageDown)
    ON_BN_CLICKED(IDC_BUTTON_PAGE_UP, &PhoneRecordsDialog::OnBnClickedButtonPageUp)
    ON_BN_CLICKED(IDC_BUTTON_PLAY_MESSAGES, &PhoneRecordsDialog::OnBnClickedButtonPlayMessages)
    ON_BN_CLICKED(IDC_BUTTON_CLEAR, &PhoneRecordsDialog::OnBnClickedButtonClear)
    ON_BN_CLICKED(IDC_BUTTON_FIND, &PhoneRecordsDialog::OnBnClickedButtonFind)
    ON_STN_CLICKED(IDC_STATIC_HAS_RECORD_0, &PhoneRecordsDialog::OnStnClickedStaticHasRecord0)
    ON_STN_CLICKED(IDC_STATIC_CALL_0, &PhoneRecordsDialog::OnStnClickedStaticCall0)
    ON_STN_CLICKED(IDC_STATIC_CALL_DELETE_0, &PhoneRecordsDialog::OnStnClickedStaticDelete0)
    ON_STN_CLICKED(IDC_STATIC_HAS_RECORD_1, &PhoneRecordsDialog::OnStnClickedStaticHasRecord1)
    ON_STN_CLICKED(IDC_STATIC_CALL_1, &PhoneRecordsDialog::OnStnClickedStaticCall1)
    ON_STN_CLICKED(IDC_STATIC_CALL_DELETE_1, &PhoneRecordsDialog::OnStnClickedStaticDelete1)
    ON_STN_CLICKED(IDC_STATIC_HAS_RECORD_2, &PhoneRecordsDialog::OnStnClickedStaticHasRecord2)
    ON_STN_CLICKED(IDC_STATIC_CALL_2, &PhoneRecordsDialog::OnStnClickedStaticCall2)
    ON_STN_CLICKED(IDC_STATIC_CALL_DELETE_2, &PhoneRecordsDialog::OnStnClickedStaticDelete2)
    ON_STN_CLICKED(IDC_STATIC_HAS_RECORD_3, &PhoneRecordsDialog::OnStnClickedStaticHasRecord3)
    ON_STN_CLICKED(IDC_STATIC_CALL_3, &PhoneRecordsDialog::OnStnClickedStaticCall3)
    ON_STN_CLICKED(IDC_STATIC_CALL_DELETE_3, &PhoneRecordsDialog::OnStnClickedStaticDelete3)
    ON_STN_CLICKED(IDC_STATIC_HAS_RECORD_4, &PhoneRecordsDialog::OnStnClickedStaticHasRecord4)
    ON_STN_CLICKED(IDC_STATIC_CALL_4, &PhoneRecordsDialog::OnStnClickedStaticCall4)
    ON_STN_CLICKED(IDC_STATIC_CALL_DELETE_4, &PhoneRecordsDialog::OnStnClickedStaticDelete4)
    ON_STN_CLICKED(IDC_STATIC_HAS_RECORD_5, &PhoneRecordsDialog::OnStnClickedStaticHasRecord5)
    ON_STN_CLICKED(IDC_STATIC_CALL_5, &PhoneRecordsDialog::OnStnClickedStaticCall5)
    ON_STN_CLICKED(IDC_STATIC_CALL_DELETE_5, &PhoneRecordsDialog::OnStnClickedStaticDelete5)
    ON_STN_CLICKED(IDC_STATIC_HAS_RECORD_6, &PhoneRecordsDialog::OnStnClickedStaticHasRecord6)
    ON_STN_CLICKED(IDC_STATIC_CALL_6, &PhoneRecordsDialog::OnStnClickedStaticCall6)
    ON_STN_CLICKED(IDC_STATIC_CALL_DELETE_6, &PhoneRecordsDialog::OnStnClickedStaticDelete6)
    ON_STN_CLICKED(IDC_STATIC_HAS_RECORD_7, &PhoneRecordsDialog::OnStnClickedStaticHasRecord7)
    ON_STN_CLICKED(IDC_STATIC_CALL_7, &PhoneRecordsDialog::OnStnClickedStaticCall7)
    ON_STN_CLICKED(IDC_STATIC_CALL_DELETE_7, &PhoneRecordsDialog::OnStnClickedStaticDelete7)
    ON_STN_CLICKED(IDC_STATIC_HAS_RECORD_8, &PhoneRecordsDialog::OnStnClickedStaticHasRecord8)
    ON_STN_CLICKED(IDC_STATIC_CALL_8, &PhoneRecordsDialog::OnStnClickedStaticCall8)
    ON_STN_CLICKED(IDC_STATIC_CALL_DELETE_8, &PhoneRecordsDialog::OnStnClickedStaticDelete8)
    ON_STN_CLICKED(IDC_STATIC_HAS_RECORD_9, &PhoneRecordsDialog::OnStnClickedStaticHasRecord9)
    ON_STN_CLICKED(IDC_STATIC_CALL_9, &PhoneRecordsDialog::OnStnClickedStaticCall9)
    ON_STN_CLICKED(IDC_STATIC_CALL_DELETE_9, &PhoneRecordsDialog::OnStnClickedStaticDelete9)
    ON_WM_CTLCOLOR()
    ON_BN_CLICKED(IDC_BUTTON_CALL, &PhoneRecordsDialog::OnBnClickedButtonCall)
    ON_BN_CLICKED(IDC_BUTTON_VOICE_MESSAGES, &PhoneRecordsDialog::OnBnClickedButtonVoiceMessages)
    ON_EN_SETFOCUS(IDC_EDIT_NAME, &PhoneRecordsDialog::OnEnSetfocusEditName)
    ON_EN_KILLFOCUS(IDC_EDIT_NAME, &PhoneRecordsDialog::OnEnKillfocusEditName)
    ON_EN_SETFOCUS(IDC_EDIT_NUMBER, &PhoneRecordsDialog::OnEnSetfocusEditNumber)
    ON_EN_KILLFOCUS(IDC_EDIT_NUMBER, &PhoneRecordsDialog::OnEnKillfocusEditNumber)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// PhoneRecordsDialog message handlers

void PhoneRecordsDialog::SetCallInfos(std::vector<Util::shared_ptr<CallInfo> > const& callInfos) {
    bool hasCall = Telephone::Instance()->HasCall();
    if (hasCall) {
        CWnd* control = GetDlgItem(IDC_BUTTON_CALL);
        control->ShowWindow(SW_SHOW);
    }
    callInfos_ = callInfos;
    CWnd* control = GetDlgItem(IDC_STATIC_CALL_TYPE_0);
    control->ShowWindow(SW_HIDE);
    control = GetDlgItem(IDC_STATIC_CALL_NAME_0);
    control->ShowWindow(SW_HIDE);
    control = GetDlgItem(IDC_STATIC_NUMBER_0);
    control->ShowWindow(SW_HIDE);
    control = GetDlgItem(IDC_STATIC_DURATION_0);
    control->ShowWindow(SW_HIDE);
    control = GetDlgItem(IDC_STATIC_START_TIME_0);
    control->ShowWindow(SW_HIDE);
    control = GetDlgItem(IDC_STATIC_HAS_RECORD_0);
    control->ShowWindow(SW_HIDE);
    control = GetDlgItem(IDC_STATIC_CALL_0);
    control->ShowWindow(SW_HIDE);
    control = GetDlgItem(IDC_STATIC_CALL_DELETE_0);
    control->ShowWindow(SW_HIDE);

    control = GetDlgItem(IDC_STATIC_CALL_TYPE_1);
    control->ShowWindow(SW_HIDE);
    control = GetDlgItem(IDC_STATIC_CALL_NAME_1);
    control->ShowWindow(SW_HIDE);
    control = GetDlgItem(IDC_STATIC_NUMBER_1);
    control->ShowWindow(SW_HIDE);
    control = GetDlgItem(IDC_STATIC_DURATION_1);
    control->ShowWindow(SW_HIDE);
    control = GetDlgItem(IDC_STATIC_START_TIME_1);
    control->ShowWindow(SW_HIDE);
    control = GetDlgItem(IDC_STATIC_HAS_RECORD_1);
    control->ShowWindow(SW_HIDE);
    control = GetDlgItem(IDC_STATIC_CALL_1);
    control->ShowWindow(SW_HIDE);
    control = GetDlgItem(IDC_STATIC_CALL_DELETE_1);
    control->ShowWindow(SW_HIDE);

    control = GetDlgItem(IDC_STATIC_CALL_TYPE_2);
    control->ShowWindow(SW_HIDE);
    control = GetDlgItem(IDC_STATIC_CALL_NAME_2);
    control->ShowWindow(SW_HIDE);
    control = GetDlgItem(IDC_STATIC_NUMBER_2);
    control->ShowWindow(SW_HIDE);
    control = GetDlgItem(IDC_STATIC_DURATION_11);
    control->ShowWindow(SW_HIDE);
    control = GetDlgItem(IDC_STATIC_START_TIME_11);
    control->ShowWindow(SW_HIDE);
    control = GetDlgItem(IDC_STATIC_HAS_RECORD_2);
    control->ShowWindow(SW_HIDE);
    control = GetDlgItem(IDC_STATIC_CALL_2);
    control->ShowWindow(SW_HIDE);
    control = GetDlgItem(IDC_STATIC_CALL_DELETE_2);
    control->ShowWindow(SW_HIDE);

    control = GetDlgItem(IDC_STATIC_CALL_TYPE_3);
    control->ShowWindow(SW_HIDE);
    control = GetDlgItem(IDC_STATIC_CALL_NAME_3);
    control->ShowWindow(SW_HIDE);
    control = GetDlgItem(IDC_STATIC_NUMBER_3);
    control->ShowWindow(SW_HIDE);
    control = GetDlgItem(IDC_STATIC_DURATION_3);
    control->ShowWindow(SW_HIDE);
    control = GetDlgItem(IDC_STATIC_START_TIME_3);
    control->ShowWindow(SW_HIDE);
    control = GetDlgItem(IDC_STATIC_HAS_RECORD_3);
    control->ShowWindow(SW_HIDE);
    control = GetDlgItem(IDC_STATIC_CALL_3);
    control->ShowWindow(SW_HIDE);
    control = GetDlgItem(IDC_STATIC_CALL_DELETE_3);
    control->ShowWindow(SW_HIDE);

    control = GetDlgItem(IDC_STATIC_CALL_TYPE_4);
    control->ShowWindow(SW_HIDE);
    control = GetDlgItem(IDC_STATIC_CALL_NAME_4);
    control->ShowWindow(SW_HIDE);
    control = GetDlgItem(IDC_STATIC_NUMBER_4);
    control->ShowWindow(SW_HIDE);
    control = GetDlgItem(IDC_STATIC_DURATION_4);
    control->ShowWindow(SW_HIDE);
    control = GetDlgItem(IDC_STATIC_START_TIME_4);
    control->ShowWindow(SW_HIDE);
    control = GetDlgItem(IDC_STATIC_HAS_RECORD_4);
    control->ShowWindow(SW_HIDE);
    control = GetDlgItem(IDC_STATIC_CALL_4);
    control->ShowWindow(SW_HIDE);
    control = GetDlgItem(IDC_STATIC_CALL_DELETE_4);
    control->ShowWindow(SW_HIDE);

    control = GetDlgItem(IDC_STATIC_CALL_TYPE_5);
    control->ShowWindow(SW_HIDE);
    control = GetDlgItem(IDC_STATIC_CALL_NAME_5);
    control->ShowWindow(SW_HIDE);
    control = GetDlgItem(IDC_STATIC_NUMBER_5);
    control->ShowWindow(SW_HIDE);
    control = GetDlgItem(IDC_STATIC_DURATION_5);
    control->ShowWindow(SW_HIDE);
    control = GetDlgItem(IDC_STATIC_START_TIME_5);
    control->ShowWindow(SW_HIDE);
    control = GetDlgItem(IDC_STATIC_HAS_RECORD_5);
    control->ShowWindow(SW_HIDE);
    control = GetDlgItem(IDC_STATIC_CALL_5);
    control->ShowWindow(SW_HIDE);
    control = GetDlgItem(IDC_STATIC_CALL_DELETE_5);
    control->ShowWindow(SW_HIDE);

    control = GetDlgItem(IDC_STATIC_CALL_TYPE_6);
    control->ShowWindow(SW_HIDE);
    control = GetDlgItem(IDC_STATIC_CALL_NAME_6);
    control->ShowWindow(SW_HIDE);
    control = GetDlgItem(IDC_STATIC_NUMBER_6);
    control->ShowWindow(SW_HIDE);
    control = GetDlgItem(IDC_STATIC_DURATION_6);
    control->ShowWindow(SW_HIDE);
    control = GetDlgItem(IDC_STATIC_START_TIME_6);
    control->ShowWindow(SW_HIDE);
    control = GetDlgItem(IDC_STATIC_HAS_RECORD_6);
    control->ShowWindow(SW_HIDE);
    control = GetDlgItem(IDC_STATIC_CALL_6);
    control->ShowWindow(SW_HIDE);
    control = GetDlgItem(IDC_STATIC_CALL_DELETE_6);
    control->ShowWindow(SW_HIDE);

    control = GetDlgItem(IDC_STATIC_CALL_TYPE_7);
    control->ShowWindow(SW_HIDE);
    control = GetDlgItem(IDC_STATIC_CALL_NAME_7);
    control->ShowWindow(SW_HIDE);
    control = GetDlgItem(IDC_STATIC_NUMBER_7);
    control->ShowWindow(SW_HIDE);
    control = GetDlgItem(IDC_STATIC_DURATION_7);
    control->ShowWindow(SW_HIDE);
    control = GetDlgItem(IDC_STATIC_START_TIME_7);
    control->ShowWindow(SW_HIDE);
    control = GetDlgItem(IDC_STATIC_HAS_RECORD_7);
    control->ShowWindow(SW_HIDE);
    control = GetDlgItem(IDC_STATIC_CALL_7);
    control->ShowWindow(SW_HIDE);
    control = GetDlgItem(IDC_STATIC_CALL_DELETE_7);
    control->ShowWindow(SW_HIDE);

    control = GetDlgItem(IDC_STATIC_CALL_TYPE_8);
    control->ShowWindow(SW_HIDE);
    control = GetDlgItem(IDC_STATIC_CALL_NAME_8);
    control->ShowWindow(SW_HIDE);
    control = GetDlgItem(IDC_STATIC_NUMBER_8);
    control->ShowWindow(SW_HIDE);
    control = GetDlgItem(IDC_STATIC_DURATION_8);
    control->ShowWindow(SW_HIDE);
    control = GetDlgItem(IDC_STATIC_START_TIME_8);
    control->ShowWindow(SW_HIDE);
    control = GetDlgItem(IDC_STATIC_HAS_RECORD_8);
    control->ShowWindow(SW_HIDE);
    control = GetDlgItem(IDC_STATIC_CALL_8);
    control->ShowWindow(SW_HIDE);
    control = GetDlgItem(IDC_STATIC_CALL_DELETE_8);
    control->ShowWindow(SW_HIDE);

    control = GetDlgItem(IDC_STATIC_CALL_TYPE_9);
    control->ShowWindow(SW_HIDE);
    control = GetDlgItem(IDC_STATIC_CALL_NAME_9);
    control->ShowWindow(SW_HIDE);
    control = GetDlgItem(IDC_STATIC_NUMBER_9);
    control->ShowWindow(SW_HIDE);
    control = GetDlgItem(IDC_STATIC_DURATION_9);
    control->ShowWindow(SW_HIDE);
    control = GetDlgItem(IDC_STATIC_START_TIME_9);
    control->ShowWindow(SW_HIDE);
    control = GetDlgItem(IDC_STATIC_HAS_RECORD_9);
    control->ShowWindow(SW_HIDE);
    control = GetDlgItem(IDC_STATIC_CALL_9);
    control->ShowWindow(SW_HIDE);
    control = GetDlgItem(IDC_STATIC_CALL_DELETE_9);
    control->ShowWindow(SW_HIDE);
    size_t count = callInfos_.size();
    size_t current = 0;
    if (current < count) {
        control = GetDlgItem(IDC_STATIC_CALL_TYPE_0);
        control->SetWindowTextW(ContactInfoTypeToString(callInfos_[current]->type()).c_str());
        control->ShowWindow(SW_SHOW);
        control = GetDlgItem(IDC_STATIC_CALL_NAME_0);
        control->SetWindowTextW(callInfos_[current]->name().c_str());
        control->ShowWindow(SW_SHOW);
        control = GetDlgItem(IDC_STATIC_NUMBER_0);
        control->SetWindowTextW(callInfos_[current]->telephoneNumber().c_str());
        control->ShowWindow(SW_SHOW);
        control = GetDlgItem(IDC_STATIC_DURATION_0);
        control->SetWindowTextW(Util::StringOp::FromTimeSpan(callInfos_[current]->duration()).c_str());
        control->ShowWindow(SW_SHOW);
        control = GetDlgItem(IDC_STATIC_START_TIME_0);
        control->SetWindowTextW(Util::StringOp::FromTimestamp(callInfos_[current]->startTime()).c_str());
        control->ShowWindow(SW_SHOW);
        if (callInfos_[current]->hasSound()) {
            CString value = L"sounds";
            control = GetDlgItem(IDC_STATIC_HAS_RECORD_0);
            control->SetWindowTextW(value);
            control->ShowWindow(SW_SHOW);
        }
        control = GetDlgItem(IDC_STATIC_CALL_0);
        control->ShowWindow(SW_SHOW);
        control = GetDlgItem(IDC_STATIC_CALL_DELETE_0);
        control->ShowWindow(SW_SHOW);
        ++current;
    }
    if (current < count) {
        control = GetDlgItem(IDC_STATIC_CALL_TYPE_1);
        control->SetWindowTextW(ContactInfoTypeToString(callInfos_[current]->type()).c_str());
        control->ShowWindow(SW_SHOW);
        control = GetDlgItem(IDC_STATIC_CALL_NAME_1);
        control->SetWindowTextW(callInfos_[current]->name().c_str());
        control->ShowWindow(SW_SHOW);
        control = GetDlgItem(IDC_STATIC_NUMBER_1);
        control->SetWindowTextW(callInfos_[current]->telephoneNumber().c_str());
        control->ShowWindow(SW_SHOW);
        control = GetDlgItem(IDC_STATIC_DURATION_1);
        control->SetWindowTextW(Util::StringOp::FromTimeSpan(callInfos_[current]->duration()).c_str());
        control->ShowWindow(SW_SHOW);
        control = GetDlgItem(IDC_STATIC_START_TIME_1);
        control->SetWindowTextW(Util::StringOp::FromTimestamp(callInfos_[current]->startTime()).c_str());
        control->ShowWindow(SW_SHOW);
        if (callInfos_[current]->hasSound()) {
            CString value = L"sounds";
            control = GetDlgItem(IDC_STATIC_HAS_RECORD_1);
            control->SetWindowTextW(value);
            control->ShowWindow(SW_SHOW);
        }
        control = GetDlgItem(IDC_STATIC_CALL_1);
        control->ShowWindow(SW_SHOW);
        control = GetDlgItem(IDC_STATIC_CALL_DELETE_1);
        control->ShowWindow(SW_SHOW);
        ++current;
    }
    if (current < count) {
        control = GetDlgItem(IDC_STATIC_CALL_TYPE_2);
        control->SetWindowTextW(ContactInfoTypeToString(callInfos_[current]->type()).c_str());
        control->ShowWindow(SW_SHOW);
        control = GetDlgItem(IDC_STATIC_CALL_NAME_2);
        control->SetWindowTextW(callInfos_[current]->name().c_str());
        control->ShowWindow(SW_SHOW);
        control = GetDlgItem(IDC_STATIC_NUMBER_2);
        control->SetWindowTextW(callInfos_[current]->telephoneNumber().c_str());
        control->ShowWindow(SW_SHOW);
        control = GetDlgItem(IDC_STATIC_DURATION_11);
        control->SetWindowTextW(Util::StringOp::FromTimeSpan(callInfos_[current]->duration()).c_str());
        control->ShowWindow(SW_SHOW);
        control = GetDlgItem(IDC_STATIC_START_TIME_11);
        control->SetWindowTextW(Util::StringOp::FromTimestamp(callInfos_[current]->startTime()).c_str());
        control->ShowWindow(SW_SHOW);
        if (callInfos_[current]->hasSound()) {
            CString value = L"sounds";
            control = GetDlgItem(IDC_STATIC_HAS_RECORD_2);
            control->SetWindowTextW(value);
            control->ShowWindow(SW_SHOW);
        }
        control = GetDlgItem(IDC_STATIC_CALL_2);
        control->ShowWindow(SW_SHOW);
        control = GetDlgItem(IDC_STATIC_CALL_DELETE_2);
        control->ShowWindow(SW_SHOW);
        ++current;
    }
    if (current < count) {
        control = GetDlgItem(IDC_STATIC_CALL_TYPE_3);
        control->SetWindowTextW(ContactInfoTypeToString(callInfos_[current]->type()).c_str());
        control->ShowWindow(SW_SHOW);
        control = GetDlgItem(IDC_STATIC_CALL_NAME_3);
        control->SetWindowTextW(callInfos_[current]->name().c_str());
        control->ShowWindow(SW_SHOW);
        control = GetDlgItem(IDC_STATIC_NUMBER_3);
        control->SetWindowTextW(callInfos_[current]->telephoneNumber().c_str());
        control->ShowWindow(SW_SHOW);
        control = GetDlgItem(IDC_STATIC_DURATION_3);
        control->SetWindowTextW(Util::StringOp::FromTimeSpan(callInfos_[current]->duration()).c_str());
        control->ShowWindow(SW_SHOW);
        control = GetDlgItem(IDC_STATIC_START_TIME_3);
        control->SetWindowTextW(Util::StringOp::FromTimestamp(callInfos_[current]->startTime()).c_str());
        control->ShowWindow(SW_SHOW);
        if (callInfos_[current]->hasSound()) {
            CString value = L"sounds";
            control = GetDlgItem(IDC_STATIC_HAS_RECORD_3);
            control->SetWindowTextW(value);
            control->ShowWindow(SW_SHOW);
        }
        control = GetDlgItem(IDC_STATIC_CALL_3);
        control->ShowWindow(SW_SHOW);
        control = GetDlgItem(IDC_STATIC_CALL_DELETE_3);
        control->ShowWindow(SW_SHOW);
        ++current;
    }
    if (current < count) {
        control = GetDlgItem(IDC_STATIC_CALL_TYPE_4);
        control->SetWindowTextW(ContactInfoTypeToString(callInfos_[current]->type()).c_str());
        control->ShowWindow(SW_SHOW);
        control = GetDlgItem(IDC_STATIC_CALL_NAME_4);
        control->SetWindowTextW(callInfos_[current]->name().c_str());
        control->ShowWindow(SW_SHOW);
        control = GetDlgItem(IDC_STATIC_NUMBER_4);
        control->SetWindowTextW(callInfos_[current]->telephoneNumber().c_str());
        control->ShowWindow(SW_SHOW);
        control = GetDlgItem(IDC_STATIC_DURATION_4);
        control->SetWindowTextW(Util::StringOp::FromTimeSpan(callInfos_[current]->duration()).c_str());
        control->ShowWindow(SW_SHOW);
        control = GetDlgItem(IDC_STATIC_START_TIME_4);
        control->SetWindowTextW(Util::StringOp::FromTimestamp(callInfos_[current]->startTime()).c_str());
        control->ShowWindow(SW_SHOW);
        if (callInfos_[current]->hasSound()) {
            CString value = L"sounds";
            control = GetDlgItem(IDC_STATIC_HAS_RECORD_4);
            control->SetWindowTextW(value);
            control->ShowWindow(SW_SHOW);
        }
        control = GetDlgItem(IDC_STATIC_CALL_4);
        control->ShowWindow(SW_SHOW);
        control = GetDlgItem(IDC_STATIC_CALL_DELETE_4);
        control->ShowWindow(SW_SHOW);
        ++current;
    }
    if (current < count) {
        control = GetDlgItem(IDC_STATIC_CALL_TYPE_5);
        control->SetWindowTextW(ContactInfoTypeToString(callInfos_[current]->type()).c_str());
        control->ShowWindow(SW_SHOW);
        control = GetDlgItem(IDC_STATIC_CALL_NAME_5);
        control->SetWindowTextW(callInfos_[current]->name().c_str());
        control->ShowWindow(SW_SHOW);
        control = GetDlgItem(IDC_STATIC_NUMBER_5);
        control->SetWindowTextW(callInfos_[current]->telephoneNumber().c_str());
        control->ShowWindow(SW_SHOW);
        control = GetDlgItem(IDC_STATIC_DURATION_5);
        control->SetWindowTextW(Util::StringOp::FromTimeSpan(callInfos_[current]->duration()).c_str());
        control->ShowWindow(SW_SHOW);
        control = GetDlgItem(IDC_STATIC_START_TIME_5);
        control->SetWindowTextW(Util::StringOp::FromTimestamp(callInfos_[current]->startTime()).c_str());
        control->ShowWindow(SW_SHOW);
        if (callInfos_[current]->hasSound()) {
            CString value = L"sounds";
            control = GetDlgItem(IDC_STATIC_HAS_RECORD_5);
            control->SetWindowTextW(value);
            control->ShowWindow(SW_SHOW);
        }
        control = GetDlgItem(IDC_STATIC_CALL_5);
        control->ShowWindow(SW_SHOW);
        control = GetDlgItem(IDC_STATIC_CALL_DELETE_5);
        control->ShowWindow(SW_SHOW);
        ++current;
    }
    if (current < count) {
        control = GetDlgItem(IDC_STATIC_CALL_TYPE_6);
        control->SetWindowTextW(ContactInfoTypeToString(callInfos_[current]->type()).c_str());
        control->ShowWindow(SW_SHOW);
        control = GetDlgItem(IDC_STATIC_CALL_NAME_6);
        control->SetWindowTextW(callInfos_[current]->name().c_str());
        control->ShowWindow(SW_SHOW);
        control = GetDlgItem(IDC_STATIC_NUMBER_6);
        control->SetWindowTextW(callInfos_[current]->telephoneNumber().c_str());
        control->ShowWindow(SW_SHOW);
        control = GetDlgItem(IDC_STATIC_DURATION_6);
        control->SetWindowTextW(Util::StringOp::FromTimeSpan(callInfos_[current]->duration()).c_str());
        control->ShowWindow(SW_SHOW);
        control = GetDlgItem(IDC_STATIC_START_TIME_6);
        control->SetWindowTextW(Util::StringOp::FromTimestamp(callInfos_[current]->startTime()).c_str());
        control->ShowWindow(SW_SHOW);
        if (callInfos_[current]->hasSound()) {
            CString value = L"sounds";
            control = GetDlgItem(IDC_STATIC_HAS_RECORD_6);
            control->SetWindowTextW(value);
            control->ShowWindow(SW_SHOW);
        }
        control = GetDlgItem(IDC_STATIC_CALL_6);
        control->ShowWindow(SW_SHOW);
        control = GetDlgItem(IDC_STATIC_CALL_DELETE_6);
        control->ShowWindow(SW_SHOW);
        ++current;
    }
    if (current < count) {
        control = GetDlgItem(IDC_STATIC_CALL_TYPE_7);
        control->SetWindowTextW(ContactInfoTypeToString(callInfos_[current]->type()).c_str());
        control->ShowWindow(SW_SHOW);
        control = GetDlgItem(IDC_STATIC_CALL_NAME_7);
        control->SetWindowTextW(callInfos_[current]->name().c_str());
        control->ShowWindow(SW_SHOW);
        control = GetDlgItem(IDC_STATIC_NUMBER_7);
        control->SetWindowTextW(callInfos_[current]->telephoneNumber().c_str());
        control->ShowWindow(SW_SHOW);
        control = GetDlgItem(IDC_STATIC_DURATION_7);
        control->SetWindowTextW(Util::StringOp::FromTimeSpan(callInfos_[current]->duration()).c_str());
        control->ShowWindow(SW_SHOW);
        control = GetDlgItem(IDC_STATIC_START_TIME_7);
        control->SetWindowTextW(Util::StringOp::FromTimestamp(callInfos_[current]->startTime()).c_str());
        control->ShowWindow(SW_SHOW);
        if (callInfos_[current]->hasSound()) {
            CString value = L"sounds";
            control = GetDlgItem(IDC_STATIC_HAS_RECORD_7);
            control->SetWindowTextW(value);
            control->ShowWindow(SW_SHOW);
        }
        control = GetDlgItem(IDC_STATIC_CALL_7);
        control->ShowWindow(SW_SHOW);
        control = GetDlgItem(IDC_STATIC_CALL_DELETE_7);
        control->ShowWindow(SW_SHOW);
        ++current;
    }
    if (current < count) {
        control = GetDlgItem(IDC_STATIC_CALL_TYPE_8);
        control->SetWindowTextW(ContactInfoTypeToString(callInfos_[current]->type()).c_str());
        control->ShowWindow(SW_SHOW);
        control = GetDlgItem(IDC_STATIC_CALL_NAME_8);
        control->SetWindowTextW(callInfos_[current]->name().c_str());
        control->ShowWindow(SW_SHOW);
        control = GetDlgItem(IDC_STATIC_NUMBER_8);
        control->SetWindowTextW(callInfos_[current]->telephoneNumber().c_str());
        control->ShowWindow(SW_SHOW);
        control = GetDlgItem(IDC_STATIC_DURATION_8);
        control->SetWindowTextW(Util::StringOp::FromTimeSpan(callInfos_[current]->duration()).c_str());
        control->ShowWindow(SW_SHOW);
        control = GetDlgItem(IDC_STATIC_START_TIME_8);
        control->SetWindowTextW(Util::StringOp::FromTimestamp(callInfos_[current]->startTime()).c_str());
        control->ShowWindow(SW_SHOW);
        if (callInfos_[current]->hasSound()) {
            CString value = L"sounds";
            control = GetDlgItem(IDC_STATIC_HAS_RECORD_8);
            control->SetWindowTextW(value);
            control->ShowWindow(SW_SHOW);
        }
        control = GetDlgItem(IDC_STATIC_CALL_8);
        control->ShowWindow(SW_SHOW);
        control = GetDlgItem(IDC_STATIC_CALL_DELETE_8);
        control->ShowWindow(SW_SHOW);
        ++current;
    }
    if (current < count) {
        control = GetDlgItem(IDC_STATIC_CALL_TYPE_9);
        control->SetWindowTextW(ContactInfoTypeToString(callInfos_[current]->type()).c_str());
        control->ShowWindow(SW_SHOW);
        control = GetDlgItem(IDC_STATIC_CALL_NAME_9);
        control->SetWindowTextW(callInfos_[current]->name().c_str());
        control->ShowWindow(SW_SHOW);
        control = GetDlgItem(IDC_STATIC_NUMBER_9);
        control->SetWindowTextW(callInfos_[current]->telephoneNumber().c_str());
        control->ShowWindow(SW_SHOW);
        control = GetDlgItem(IDC_STATIC_DURATION_9);
        control->SetWindowTextW(Util::StringOp::FromTimeSpan(callInfos_[current]->duration()).c_str());
        control->ShowWindow(SW_SHOW);
        control = GetDlgItem(IDC_STATIC_START_TIME_9);
        control->SetWindowTextW(Util::StringOp::FromTimestamp(callInfos_[current]->startTime()).c_str());
        control->ShowWindow(SW_SHOW);
        if (callInfos_[current]->hasSound()) {
            CString value = L"sounds";
            control = GetDlgItem(IDC_STATIC_HAS_RECORD_9);
            control->SetWindowTextW(value);
            control->ShowWindow(SW_SHOW);
        }
        control = GetDlgItem(IDC_STATIC_CALL_9);
        control->ShowWindow(SW_SHOW);
        control = GetDlgItem(IDC_STATIC_CALL_DELETE_9);
        control->ShowWindow(SW_SHOW);
        ++current;
    }
}

void PhoneRecordsDialog::OnBnClickedButtonClose() {
    // TODO: Add your control notification handler code here
    ShowWindow(SW_HIDE);
    //OnOK();
    //DestroyWindow();
}

void PhoneRecordsDialog::OnBnClickedButtonPlayMessages() {
    std::vector<Util::shared_ptr<CallInfo> > callInfos = CallInfo::Select(L"[type] = " + Util::StringOp::FromInt(citLeaveWord));
    playingRecordsForm->SetCallInfos(callInfos);
    playingRecordsForm->ShowWindow(SW_SHOW);
}

void PhoneRecordsDialog::OnBnClickedButtonClear() {
    //if (you are sure)
    CallInfo::Remove(L"");
    callInfos_.clear();
    SetCallInfos(callInfos_);
}

void PhoneRecordsDialog::OnBnClickedButtonPageUp() {
    offset_ -= pageSize;
    if (offset_ < 0) {
        offset_ = 0;
    }
    std::vector<Util::shared_ptr<CallInfo> > newCallInfos = CallInfo::Select(filter_, L"id", Util::BaseData::srDesc, offset_, pageSize);
    SetCallInfos(newCallInfos);
}

void PhoneRecordsDialog::OnBnClickedButtonPageDown() {
    size_t maxRecord = CallInfo::GetDataCount(L"");
    offset_ += pageSize;
    if (offset_ > maxRecord) {
        offset_ = maxRecord / pageSize * pageSize;
    }
    std::vector<Util::shared_ptr<CallInfo> > newCallInfos = CallInfo::Select(filter_, L"id", Util::BaseData::srDesc, offset_, pageSize);
    SetCallInfos(newCallInfos);
}

void PhoneRecordsDialog::OnBnClickedButtonFind() {
    CString filter;
    CWnd* control = GetDlgItem(IDC_EDIT_NAME);
    CString value;
    control->GetWindowTextW(value);
    if (value != L"") {
        filter += L"[contactId] = ";
        filter += Util::StringOp::FromInt(Contact::GetIdByName(static_cast<LPCTSTR>(value))).c_str();
    }
    control = GetDlgItem(IDC_EDIT_NUMBER);
    control->GetWindowTextW(value);
    if (value != L"") {
        if (filter != L"") {
            filter += " AND ";
        }
        filter += L"[telephoneNumber] LIKE '" + value + L"%'";
    }
    control = GetDlgItem(IDC_COMBO_TYPE);
    control->GetWindowTextW(value);
    if (value != L"All") {
        if (filter != L"") {
            filter += " AND ";
        }
        filter += L"[type] = ";
        filter += Util::StringOp::FromInt(StringToContactInfoType(static_cast<LPCTSTR>(value))).c_str();
    }
    filter_ = static_cast<LPCTSTR>(filter);
    std::vector<Util::shared_ptr<CallInfo> > callInfos = CallInfo::Select(filter_, L"id", Util::BaseData::srDesc, 0, pageSize);
    SetCallInfos(callInfos);
}

void PhoneRecordsDialog::PlayRecord(int const no) {
    if (callInfos_[no]->hasSound()) {
        playingRecordsForm->SetCallInfo(callInfos_[no]);
    }
}

void PhoneRecordsDialog::Call(int const no) {
	std::wstring* number = new std::wstring(callInfos_[no]->telephoneNumber());
    Telephone::Instance()->fireEvent(Telephone::eSoftDial, const_cast<void*>(reinterpret_cast<void const*>(number)));
}

void PhoneRecordsDialog::Delete(int const no) {
    //CWnd* control = GetDlgItem(startTimeId);
    //CString value;
    //control->GetWindowTextW(value);
    //Util::Timestamp time = Util::TimeOp::FromString(static_cast<LPCTSTR>(value), Util::TimeOp::pDateTime, L"/ :");
    //CallInfo::Remove(L"[startTime] = '" + Util::StringOp::FromTimestamp(time) + L"'");
    callInfos_[no]->Remove();
    std::vector<Util::shared_ptr<CallInfo> > newCallInfos = CallInfo::Select(filter_, L"id", Util::BaseData::srDesc, offset_, pageSize);
    SetCallInfos(newCallInfos);
}

void PhoneRecordsDialog::OnStnClickedStaticHasRecord0() {
    PlayRecord(0);
}

void PhoneRecordsDialog::OnStnClickedStaticCall0() {
    Call(0);
}

void PhoneRecordsDialog::OnStnClickedStaticDelete0() {
    Delete(0);
}

void PhoneRecordsDialog::OnStnClickedStaticHasRecord1() {
    PlayRecord(1);
}

void PhoneRecordsDialog::OnStnClickedStaticCall1() {
    Call(1);
}

void PhoneRecordsDialog::OnStnClickedStaticDelete1() {
    Delete(1);
}

void PhoneRecordsDialog::OnStnClickedStaticHasRecord2() {
    PlayRecord(2);
}

void PhoneRecordsDialog::OnStnClickedStaticCall2() {
    Call(2);
}

void PhoneRecordsDialog::OnStnClickedStaticDelete2() {
    Delete(2);
}

void PhoneRecordsDialog::OnStnClickedStaticHasRecord3() {
    PlayRecord(3);
}

void PhoneRecordsDialog::OnStnClickedStaticCall3() {
    Call(3);
}

void PhoneRecordsDialog::OnStnClickedStaticDelete3() {
    Delete(3);
}

void PhoneRecordsDialog::OnStnClickedStaticHasRecord4() {
    PlayRecord(4);
}

void PhoneRecordsDialog::OnStnClickedStaticCall4() {
    Call(4);
}

void PhoneRecordsDialog::OnStnClickedStaticDelete4() {
    Delete(4);
}

void PhoneRecordsDialog::OnStnClickedStaticHasRecord5() {
    PlayRecord(5);
}

void PhoneRecordsDialog::OnStnClickedStaticCall5() {
    Call(5);
}

void PhoneRecordsDialog::OnStnClickedStaticDelete5() {
    Delete(5);
}

void PhoneRecordsDialog::OnStnClickedStaticHasRecord6() {
    PlayRecord(6);
}

void PhoneRecordsDialog::OnStnClickedStaticCall6() {
    Call(6);
}

void PhoneRecordsDialog::OnStnClickedStaticDelete6() {
    Delete(6);
}

void PhoneRecordsDialog::OnStnClickedStaticHasRecord7() {
    PlayRecord(7);
}

void PhoneRecordsDialog::OnStnClickedStaticCall7() {
    Call(7);
}

void PhoneRecordsDialog::OnStnClickedStaticDelete7() {
    Delete(7);
}

void PhoneRecordsDialog::OnStnClickedStaticHasRecord8() {
    PlayRecord(8);
}

void PhoneRecordsDialog::OnStnClickedStaticCall8() {
    Call(8);
}

void PhoneRecordsDialog::OnStnClickedStaticDelete8() {
    Delete(8);
}

void PhoneRecordsDialog::OnStnClickedStaticHasRecord9() {
    PlayRecord(9);
}

void PhoneRecordsDialog::OnStnClickedStaticCall9() {
    Call(9);
}

void PhoneRecordsDialog::OnStnClickedStaticDelete9() {
    Delete(9);
}

HBRUSH PhoneRecordsDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) {
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

void PhoneRecordsDialog::OnBnClickedButtonCall() {
    callForm->SetListener(this);
    callForm->ShowWindow(SW_SHOW);
}

void PhoneRecordsDialog::OnBnClickedButtonVoiceMessages() {
    std::vector<Util::shared_ptr<CallInfo> > callInfos = CallInfo::GetAllNotPlayedLeaveWord();
    if (!callInfos.empty()) {
        playingRecordsForm->SetCallInfos(callInfos);
        playingRecordsForm->ShowWindow(SW_SHOW);
    }
}

BOOL PhoneRecordsDialog::OnInitDialog() {
    CDialog::OnInitDialog();

    // TODO:  Add extra initialization here
    CComboBox* control = static_cast<CComboBox*>(GetDlgItem(IDC_COMBO_TYPE));
    control->AddString(L"All");
    control->AddString(L"Missed");
    control->AddString(L"Received");
    control->AddString(L"Dialed");
    control->AddString(L"Leave Word");
    control->SetWindowTextW(L"All");

	SIZE buttonSize;
	buttonSize.cx = 48;
	buttonSize.cy = 48;
	std::wstring maskFilename = L"/FlashDrv/debug/buttonMask.bmp";
	closeButton_.SetImage(L"/FlashDrv/debug/close.jpg", buttonSize, maskFilename);
	closeButton_.SubclassDlgItem(IDC_BUTTON_CLOSE, this);
	pageDownButton_.SetImage(L"/FlashDrv/debug/pagedown.jpg", buttonSize, maskFilename);
	pageDownButton_.SubclassDlgItem(IDC_BUTTON_PAGE_DOWN, this);
	pageUpButton_.SetImage(L"/FlashDrv/debug/pageup.jpg", buttonSize, maskFilename);
	pageUpButton_.SubclassDlgItem(IDC_BUTTON_PAGE_UP, this);
	clearButton_.SetImage(L"/FlashDrv/debug/clear.jpg", buttonSize, maskFilename);
	clearButton_.SubclassDlgItem(IDC_BUTTON_CLEAR, this);
	playMessagesButton_.SetImage(L"/FlashDrv/debug/playMessage.jpg", buttonSize, maskFilename);
	playMessagesButton_.SubclassDlgItem(IDC_BUTTON_PLAY_MESSAGES, this);
	callButton_.SetImage(L"/FlashDrv/debug/call.jpg", buttonSize, maskFilename);
	callButton_.SubclassDlgItem(IDC_BUTTON_CALL, this);
	voiceMessagesButton_.SetImage(L"/FlashDrv/debug/message.jpg", buttonSize, maskFilename);
	voiceMessagesButton_.SubclassDlgItem(IDC_BUTTON_VOICE_MESSAGES, this);
	findButton_.SetImage(L"/FlashDrv/debug/find.jpg", buttonSize, maskFilename);
	findButton_.SubclassDlgItem(IDC_BUTTON_FIND, this);

	//font_.CreateFont(20, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"����");
	font_.CreatePointFont(Screen::fontPoint, L"����");
	GetDlgItem(IDC_COMBO_TYPE)->SetFont(&font_, FALSE);
	GetDlgItem(IDC_EDIT_NAME)->SetFont(&font_, FALSE);
	GetDlgItem(IDC_EDIT_NUMBER)->SetFont(&font_, FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void PhoneRecordsDialog::OnEnSetfocusEditName() {
    SipShowIM(SIPF_ON);
}

void PhoneRecordsDialog::OnEnKillfocusEditName() {
    SipShowIM(SIPF_OFF);
}

void PhoneRecordsDialog::OnEnSetfocusEditNumber() {
    SipShowIM(SIPF_ON);
}

void PhoneRecordsDialog::OnEnKillfocusEditNumber() {
    SipShowIM(SIPF_OFF);
}

LRESULT PhoneRecordsDialog::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) {
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

BOOL PhoneRecordsDialog::OnEraseBkgnd(CDC* pDC) {
	BOOL r = BackgroundImage::drawBackground(m_hWnd);
	if (!r) {
        DWORD errorCode = GetLastError();
        DWORD ec = errorCode;
	}
	//return CDialog::OnEraseBkgnd(pDC);
	return TRUE;
}