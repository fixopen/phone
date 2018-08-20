// CallDialog.cpp : implementation file
//

#include "stdafx.h"
#include "../../GPhone.h"
#include "CallDialog.h"
#include "../Logical/Telephone.h"
#include "../../Contact/Data/Contact.h"
#include "../../Contact/View/ContactDialog.h"
#include "../../Util/ImageOp.h"
#include "../../Util/TimeOp.h"
#include "../../Util/Properties.h"
#include "../../Util/Screen.h"
#include "../../Util/BackgroundImage.h"
//#include "PlayingRecordsDialog.h"

namespace Telephone {
    namespace View {
        CallDialog* callForm = 0;
        // CCallDialog dialog

        IMPLEMENT_DYNAMIC(CallDialog, CDialog)

            CallDialog::CallDialog(CWnd* pParent /*=NULL*/)
            : CDialog(CallDialog::IDD, pParent)
            , listener_(0)
            , hasCallId_(false) {
                Logical::Telephone::Instance(); //init the telephone machine
        }

        CallDialog::~CallDialog() {
        }

        void CallDialog::DoDataExchange(CDataExchange* pDX) {
            CDialog::DoDataExchange(pDX);
        }


        BEGIN_MESSAGE_MAP(CallDialog, CDialog)
            //ON_BN_CLICKED(IDC_BUTTON_CLOSE, &CallDialog::OnBnClickedButtonClose)
            //ON_BN_CLICKED(IDC_BUTTON_REJECT, &CallDialog::OnBnClickedButtonReject)
            //ON_BN_CLICKED(IDC_BUTTON_LISTEN, &CallDialog::OnBnClickedButtonListen)
            //ON_BN_CLICKED(IDC_BUTTON_HANGUP, &CallDialog::OnBnClickedButtonHangup)
            //ON_BN_CLICKED(IDC_BUTTON_HIDDEN, &CallDialog::OnBnClickedButtonHidden)
            //ON_BN_CLICKED(IDC_BUTTON_NEW_CONTACT, &CallDialog::OnBnClickedButtonNewContact)
            //ON_BN_CLICKED(IDC_BUTTON_MUTE, &CallDialog::OnBnClickedButtonMute)
            //ON_BN_CLICKED(IDC_BUTTON_START_RECORD, &CallDialog::OnBnClickedButtonStartRecord)
            //ON_BN_CLICKED(IDC_BUTTON_STOP_RECORD, &CallDialog::OnBnClickedButtonStopRecord)
            //ON_BN_CLICKED(IDC_BUTTON_VOICE_MESSAGES, &CallDialog::OnBnClickedButtonVoiceMessages)
            ON_WM_DESTROY()
            ON_WM_CTLCOLOR()
            ON_WM_TIMER()
            ON_WM_SHOWWINDOW()
            ON_WM_ERASEBKGND()
        END_MESSAGE_MAP()


        // CallDialog message handlers

        void CallDialog::SetNumber(std::wstring const& number) {
            //CWnd* control = GetDlgItem(IDC_EDIT_NUMBER);
            //control->SetWindowTextW(number.c_str());
        }

        void CallDialog::SetName(std::wstring const& name) {
            //CWnd* control = GetDlgItem(IDC_EDIT_NAME);
            //control->SetWindowTextW(name.c_str());
        }

        void CallDialog::SetAddress(std::wstring const& address) {
            //CWnd* control = GetDlgItem(IDC_EDIT_ADDRESS);
            //control->SetWindowTextW(address.c_str());
        }

        void CallDialog::SetStartTime(Util::Timestamp const& time) {
            //CWnd* control = GetDlgItem(IDC_STATIC_TIME);
            //control->SetWindowTextW(Util::StringOp::FromTimestamp(time).c_str());
        }

        void CallDialog::SetRecordStartTime(Util::Timestamp const& time) {
            //CWnd* control = GetDlgItem(IDC_STATIC_RECORD_START_TIME);
            //control->SetWindowTextW(Util::StringOp::FromTimestamp(time).c_str());
        }

        void CallDialog::ClearDuration() {
            //CWnd* control = GetDlgItem(IDC_STATIC_DURATION);
            //control->SetWindowTextW(L"0");
            //control = GetDlgItem(IDC_STATIC_RECORD_DURATION);
            //control->SetWindowTextW(L"0");
        }

        void CallDialog::OnDestroy() {
            CDialog::OnDestroy();
            MessageBox(L"CallDialog::OnDestroy");
        }

        void CallDialog::OnBnClickedButtonClose() {
            if (listener_) {
                //listener_->PostMessageW(UM_CALLINFO_UPDATE_FINALLY, 0, 0);
                listener_ = 0;
            }
            ClearDuration();
            Logical::Telephone::Instance()->SoftHangup();
            Logical::Telephone::Instance()->ClearCall();
            ShowWindow(SW_HIDE);
        }

        LRESULT CallDialog::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) {
            // TODO: Add your specialized code here and/or call the base class
            Util::shared_ptr<Contact::Data::Contact> contact;
            CWnd* control;
            switch (message) {
                //case UM_CONTACT_UPDATE_FINALLY:
                //    contact = contactForm->GetContact();
                //    contact->Insert();
                //    control = GetDlgItem(IDC_EDIT_NAME);
                //    control->SetWindowTextW(contact->name().c_str());
                //    break;
                case UM_TEL_DIAL:
                    dial_(static_cast<int>(lParam));
                    break;
                case UM_TEL_PICKUP_TIMEOUT:
                    pickupTimeout_();
                    break;
                case UM_TEL_CALLID:
                    callId_(reinterpret_cast<Logical::CALLID_INFO*>(lParam));
                    break;
                case UM_TEL_CONNECT:
                    connect_();
                    break;
                case UM_TEL_START_RECORD:
                    startRecord_();
                    break;
                case UM_TEL_RING:
                    ring_();
                    break;
                case UM_TEL_CALLID_TIMEOUT:
                    callIdTimeout_();
                    break;
                case UM_TEL_SUB_DIAL:
                    subDial_(static_cast<wchar_t>(lParam));
                    break;
                case UM_TEL_STOP_RECORD:
                    stopRecord_();
                    break;
                case UM_TEL_REMOTE_HANGUP:
                    remoteHangup_();
                    break;
                case UM_TEL_CONNECT_TIMEOUT:
                    connectTimeout_();
                    break;
                case UM_TEL_REMOTE_BUSY:
                    remoteBusy_();
                    break;
                case UM_TEL_CALLIN_TIMEOUT:
                    callinTimeout_();
                    break;
                case UM_TEL_DIAL_TIMEOUT:
                    dialTimeout_();
                    break;
                case UM_TEL_RECORD_TIMEOUT:
                    recordTimeout_();
                    break;
                case UM_TEL_SOFTDIAL:
                    softDial_(*reinterpret_cast<std::wstring*>(lParam));
                    delete reinterpret_cast<std::wstring*>(lParam);
                    break;
                case UM_TEL_PICKUP:
                    pickup_();
                    break;
                case UM_TEL_HANGUP:
                    hangup_();
                    break;
                default:
                    break;
            }
            return CDialog::WindowProc(message, wParam, lParam);
        }

        void CallDialog::OnBnClickedButtonReject() {
            Logical::Telephone::Instance()->SoftHangup();
            //Logical::Telephone::Instance()->fireEvent(Telephone::eHangup, 0);
            Logical::Telephone::Instance()->ClearCall();
        }

        void CallDialog::OnBnClickedButtonListen() {
            Logical::Telephone::Instance()->SoftPickup();
            //Logical::Telephone::Instance()->fireEvent(Telephone::ePickup, 0);
            Logical::Telephone::Instance()->SetConnectionType(Logical::Telephone::ctHandsfree);
        }

        void CallDialog::OnBnClickedButtonHangup() {
            Logical::Telephone::Instance()->SoftHangup();
            //Logical::Telephone::Instance()->fireEvent(Logical::Telephone::eHangup, 0);
            Logical::Telephone::Instance()->ClearCall();
        }

        void CallDialog::OnBnClickedButtonHidden() {
            if (listener_) {
                //listener_->PostMessageW(UM_SHOW_TELEPHONE, 0, 0);
            }
            ShowWindow(SW_HIDE);
        }

        void CallDialog::OnBnClickedButtonNewContact() {
            //Util::shared_ptr<Contact::Data::Contact> newContact(new Contact::Data::Contact());
            //CWnd* control = GetDlgItem(IDC_EDIT_NUMBER);
            //CString value;
            //control->GetWindowTextW(value);
            //newContact->telephone(static_cast<LPCTSTR>(value));
            //contactForm->SetContact(newContact);
            //contactForm->SetListener(this);
            //contactForm->ShowWindow(SW_SHOW);
        }

        void CallDialog::OnBnClickedButtonMute() {
            Logical::Telephone::Instance()->Mute();
        }

        void CallDialog::OnBnClickedButtonStartRecord() {
            Logical::Telephone::Instance()->fireEvent(Logical::Telephone::eStartRecord, 0);
        }

        void CallDialog::OnBnClickedButtonStopRecord() {
            Logical::Telephone::Instance()->fireEvent(Logical::Telephone::eStopRecord, 0);
        }

        void CallDialog::dial_(wchar_t const keyCode) {
            //KillTimer(timers_[Logical::Telephone::tDial]);
            //timers_.erase(Logical::Telephone::tDial);
            //CString value;
            //CWnd* control = GetDlgItem(IDC_EDIT_NUMBER);
            //control->GetWindowTextW(value);
            //value += keyCode;
            //control->SetWindowTextW(value);
            //std::wstring filter = L"[telephone] = '";
            //filter += static_cast<LPCTSTR>(value);
            //filter += L"' OR [telephone2] = '";
            //filter += static_cast<LPCTSTR>(value);
            //filter += L"' OR [mobile] = '";
            //filter += static_cast<LPCTSTR>(value);
            //filter += L"' OR [mobile2] = '";
            //filter += static_cast<LPCTSTR>(value);
            //filter += L"'";
            //std::vector<Util::shared_ptr<Contact::Data::Contact> > contacts = Contact::Data::Contact::Select(filter);
            //if (!contacts.empty()) {
            //    control = GetDlgItem(IDC_EDIT_NAME);
            //    control->SetWindowTextW(contacts[0]->name().c_str());
            //    Logical::Telephone::Instance()->SetContact(contacts[0]);
            //}
            //timers_.insert(std::make_pair(Logical::Telephone::tDial, SetTimer(Logical::Telephone::tDial, Logical::Telephone::tDial, 0)));
        }

        void CallDialog::pickupTimeout_() {
        }

        void CallDialog::callId_(Logical::CALLID_INFO* callId) {
            //KillTimer(timers_[Logical::Telephone::tCallId]);
            //timers_.erase(Logical::Telephone::tCallId);
            //SetNumber(Util::StringOp::ToUnicode(callId->number));
            //timers_.insert(std::make_pair(Logical::Telephone::tCallId, SetTimer(Logical::Telephone::tCallId, Logical::Telephone::tCallId, 0)));
            //CWnd* control = GetDlgItem(IDC_STATIC_COMMUNICATION_STATUS);
            //control->SetWindowTextW(L"CallerID!");
            ////SetNumber(L"");
            ////SetName(L"");
            //ClearDuration();
            //hasCallId_ = true;
            //ShowWindow(SW_SHOW);
        }

        void CallDialog::connect_() {
            //CWnd* control = GetDlgItem(IDC_STATIC_COMMUNICATION_STATUS);
            //control->SetWindowTextW(L"Connected!");
            //SetStartTime(Util::Timestamp::GetCurrentTime());
            //timers_.insert(std::make_pair(Logical::Telephone::tConnect, SetTimer(Logical::Telephone::tConnect, Logical::Telephone::tConnect, 0)));
            //std::wstring v = Util::Properties::Get(L"autoRecord");
            //if (v == L"1") {
            //    Logical::Telephone::Instance()->fireEvent(Logical::Telephone::eStartRecord, 0);
            //}
        }

        void CallDialog::startRecord_() {
            //CWnd* control = GetDlgItem(IDC_STATIC_RECORD_START_TIME);
            //control->SetWindowTextW(Util::StringOp::FromTimestamp(Util::Timestamp::GetCurrentTime()).c_str());

            //timers_.insert(std::make_pair(Logical::Telephone::tRecord, SetTimer(Logical::Telephone::tRecord, 1000, 0)));
        }

        void CallDialog::ring_() {
            //CWnd* control = GetDlgItem(IDC_STATIC_COMMUNICATION_STATUS);
            //control->SetWindowTextW(L"Ring!");
            //if (!hasCallId_) {
            //    SetNumber(L"");
            //    SetName(L"");
            //    ClearDuration();
            //}
            //ShowWindow(SW_SHOW);
        }

        void CallDialog::callIdTimeout_() {
        }

        void CallDialog::subDial_(wchar_t const keyCode) {
        }

        void CallDialog::stopRecord_() {
            //KillTimer(timers_[Logical::Telephone::tRecord]);
            //timers_.erase(Logical::Telephone::tRecord);
        }

        void CallDialog::remoteHangup_() {
            //CWnd* control = GetDlgItem(IDC_STATIC_COMMUNICATION_STATUS);
            //control->SetWindowTextW(L"Remote Hangup!");
            //KillTimer(timers_[Logical::Telephone::tConnect]);
            //timers_.erase(Logical::Telephone::tConnect);
            //KillTimer(timers_[Logical::Telephone::tRecord]);
            //timers_.erase(Logical::Telephone::tRecord);
            //hasCallId_ = false;
        }

        void CallDialog::connectTimeout_() {
        }

        void CallDialog::remoteBusy_() {
        }

        void CallDialog::callinTimeout_() {
            ShowWindow(SW_HIDE);
        }

        void CallDialog::dialTimeout_() {
        }

        void CallDialog::recordTimeout_() {
        }

        void CallDialog::softDial_(std::wstring const& number) {
            KillTimer(timers_[Logical::Telephone::tWaitConnect]);
            timers_.erase(Logical::Telephone::tWaitConnect);
            SetNumber(number);
            timers_.insert(std::make_pair(Logical::Telephone::tWaitConnect, SetTimer(Logical::Telephone::tWaitConnect, Logical::Telephone::tWaitConnect, 0)));
        }

        void CallDialog::pickup_() {
            SetNumber(L"");
            SetName(L"");
            ClearDuration();
            ShowWindow(SW_SHOW);
        }

        void CallDialog::hangup_() {
            KillTimer(timers_[Logical::Telephone::tConnect]);
            timers_.erase(Logical::Telephone::tConnect);
            KillTimer(timers_[Logical::Telephone::tRecord]);
            timers_.erase(Logical::Telephone::tRecord);
            ShowWindow(SW_HIDE);
            hasCallId_ = false;
        }

        HBRUSH CallDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) {
            HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
            //RECT bounds;
            //pWnd->GetWindowRect(&bounds);
            //std::wstring backgroundImageFilename = L"/FlashDrv/debug/NULL.JPG";
            //SIZE backgroundImageSize;
            //backgroundImageSize.cx = 800;
            //backgroundImageSize.cy = 480;
            //CFont font;
            ////font.CreateFont(height, width, escapement, orientation, weight, italic, underline, strikerOut, charset, outPrecision, clipPrecision, quality, pitchAndFamily, faceName);
            //font.CreateFont(20, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial");
            //switch (nCtlColor) {
            //    case CTLCOLOR_BTN:
            //        break;
            //    case CTLCOLOR_STATIC:
            //        hbr = Util::BackgroundImage::setStaticProperties(pDC->m_hDC, bounds, Util::ImageOp::GetImage(backgroundImageFilename, backgroundImageSize), font, RGB(255, 255, 255));
            //        break;
            //    case CTLCOLOR_EDIT:
            //        //hbr = Util::BackgroundImage::setStaticProperties(pDC->m_hDC, bounds, Util::ImageOp::GetImage(backgroundImageFilename, backgroundImageSize), font, RGB(255, 255, 255));
            //        break;
            //        //case CTLCOLOR_SCROLLBAR:
            //        //    break;
            //    case CTLCOLOR_LISTBOX:
            //        break;
            //    case CTLCOLOR_MSGBOX:
            //        break;
            //        //case CTLCOLOR_DLG:
            //        //    break;
            //    default:
            //        break;
            //}

            return hbr;
        }

        void CallDialog::OnBnClickedButtonVoiceMessages() {
            std::vector<Util::shared_ptr<Data::CallInfo> > callInfos = Data::CallInfo::GetAllNotPlayedLeaveWord();
            if (!callInfos.empty()) {
                //playingRecordsForm->SetCallInfos(callInfos);
                //playingRecordsForm->ShowWindow(SW_SHOW);
            }
        }

        void CallDialog::OnTimer(UINT_PTR nIDEvent) {
            if (nIDEvent == timers_[Logical::Telephone::tDial]) {
                KillTimer(nIDEvent);
                timers_.erase(Logical::Telephone::tDial);
                if (Util::Properties::Get(L"autoRecord") == L"1") {
                    Logical::Telephone::Instance()->fireEvent(Logical::Telephone::eForceRecord, 0);
                } else {
                    Logical::Telephone::Instance()->fireEvent(Logical::Telephone::eRemotePickup, 0);
                }
            } else if (nIDEvent == timers_[Logical::Telephone::tWaitReceive]) {
                KillTimer(nIDEvent);
                timers_.erase(Logical::Telephone::tWaitReceive);
                if (Util::Properties::Get(L"autoRecord") == L"1") {
                    Logical::Telephone::Instance()->fireEvent(Logical::Telephone::eForceRecord, 0);
                } else {
                    Logical::Telephone::Instance()->fireEvent(Logical::Telephone::eRemotePickup, 0);
                }
            } else if (nIDEvent == timers_[Logical::Telephone::tRing]) {
                KillTimer(nIDEvent);
                timers_.erase(Logical::Telephone::tRing);
                //if (ringCount > Util::Properties::Get(L"delayCount")) {
                //    Logical::Telephone::Instance()->fireEvent(Logical::Telephone::eLeaveWord, 0);
                //} else {
                //    ++ringCount;
                //}
            } else if (nIDEvent == timers_[Logical::Telephone::tCallId]) {
                KillTimer(nIDEvent);
                timers_.erase(Logical::Telephone::tCallId);
                if (Util::Properties::Get(L"autoAnswer") == L"1") {
                    Logical::Telephone::Instance()->fireEvent(Logical::Telephone::eLeaveWord, 0);
                } else {
                    Logical::Telephone::Instance()->fireEvent(Logical::Telephone::eTimeout, 0);
                }
            } else if (nIDEvent == timers_[Logical::Telephone::tWaitConnect]) {
                KillTimer(nIDEvent);
                timers_.erase(Logical::Telephone::tWaitConnect);
                if (Util::Properties::Get(L"autoRecord") == L"1") {
                    Logical::Telephone::Instance()->fireEvent(Logical::Telephone::eForceRecord, 0);
                } else {
                    Logical::Telephone::Instance()->fireEvent(Logical::Telephone::eRemotePickup, 0);
                }
            } else if (nIDEvent == timers_[Logical::Telephone::tConnect]) {
                //CWnd* duration = GetDlgItem(IDC_STATIC_DURATION);
                //CString value;
                //duration->GetWindowTextW(value);
                //int v = Util::StringOp::ToInt(static_cast<LPCTSTR>(value));
                //++v;
                //duration->SetWindowTextW(Util::StringOp::FromInt(v).c_str());
            } else if (nIDEvent == timers_[Logical::Telephone::tRecord]) {
                //CWnd* duration = GetDlgItem(IDC_STATIC_RECORD_DURATION);
                //CString value;
                //duration->GetWindowTextW(value);
                //int v = Util::StringOp::ToInt(static_cast<LPCTSTR>(value));
                //++v;
                //duration->SetWindowTextW(Util::StringOp::FromInt(v).c_str());
            } else {
                KillTimer(nIDEvent);
            }

            CDialog::OnTimer(nIDEvent);
        }

        void CallDialog::OnShowWindow(BOOL bShow, UINT nStatus) {
            CDialog::OnShowWindow(bShow, nStatus);

            if (bShow) {
                SetNumber(L"");
                SetName(L"");
                ClearDuration();
            }
        }

        BOOL CallDialog::OnEraseBkgnd(CDC* pDC) {
            BOOL r = Util::BackgroundImage::drawBackground(m_hWnd);
            if (!r) {
                DWORD errorCode = GetLastError();
                DWORD ec = errorCode;
            }
            //return CDialog::OnEraseBkgnd(pDC);
            return TRUE;
        }

        BOOL CallDialog::OnInitDialog() {
            CDialog::OnInitDialog();

            // TODO:  Add extra initialization here
            //SIZE buttonSize;
            //buttonSize.cx = 48;
            //buttonSize.cy = 48;
            //std::wstring maskFilename = L"/FlashDrv/debug/buttonMask.bmp";
            //closeButton_.SetImage(L"/FlashDrv/debug/close.jpg", buttonSize, maskFilename);
            //closeButton_.SubclassDlgItem(IDC_BUTTON_CLOSE, this);
            //hangupButton_.SetImage(L"/FlashDrv/debug/hangup.jpg", buttonSize, maskFilename);
            //hangupButton_.SubclassDlgItem(IDC_BUTTON_HANGUP, this);
            //hiddenButton_.SetImage(L"/FlashDrv/debug/hidden.jpg", buttonSize, maskFilename);
            //hiddenButton_.SubclassDlgItem(IDC_BUTTON_HIDDEN, this);
            //rejectButton_.SetImage(L"/FlashDrv/debug/hangup.jpg", buttonSize, maskFilename);
            //rejectButton_.SubclassDlgItem(IDC_BUTTON_REJECT, this);
            //listenButton_.SetImage(L"/FlashDrv/debug/answer.jpg", buttonSize, maskFilename);
            //listenButton_.SubclassDlgItem(IDC_BUTTON_LISTEN, this);
            //callButton_.SetImage(L"/FlashDrv/debug/call.jpg", buttonSize, maskFilename);
            //callButton_.SubclassDlgItem(IDC_BUTTON_CALL, this);
            //voiceMessagesButton_.SetImage(L"/FlashDrv/debug/message.jpg", buttonSize, maskFilename);
            //voiceMessagesButton_.SubclassDlgItem(IDC_BUTTON_VOICE_MESSAGES, this);

            ////font_.CreateFont(20, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"ËÎÌו");
            //font_.CreatePointFont(Util::Screen::fontPoint, L"ËÎÌו");
            //GetDlgItem(IDC_EDIT_NAME)->SetFont(&font_, FALSE);
            //GetDlgItem(IDC_EDIT_NUMBER)->SetFont(&font_, FALSE);

            return TRUE;  // return TRUE unless you set the focus to a control
            // EXCEPTION: OCX Property Pages should return FALSE
        }
    }
}
