#ifndef __TELEPHONE_LOGICAL_TELEPHONE_H__
#define __TELEPHONE_LOGICAL_TELEPHONE_H__

#include "../../Util/SerialPort.h"
#include "../../Util/Fsm.h"
#include "../../Util/Audio/WaveRecorder.h"
#include "../Data/CallInfo.h"
#include "../View/CallDialog.h"
#include "../../Contact/Data/Contact.h"
#include "../../Contact/Data/SoundSegment.h"
#include "CallId.h"

namespace Telephone {
    namespace Logical {
        class PstnTelephone : public Util::FSM<PstnTelephone> {
            friend void ParseTelephoneData(unsigned char const * const data, unsigned int const length);
            friend void DialThread();
        public:
            enum State {
                sIdle,
                sPickuping,
                sPickupTimeout,
                sDialing,
                sWaitingConnect,
                sConnecting,
                sSubSeqPickuping,
                sRecording,
                sRinging,
                sCallId,
                sWaitingReceive,
                sAnswerTimeout,
                sDisconnecting,
                sError,
            };
            enum Event {
                ePickup,
                ePickupTimeout,
                eDial,
                eDialTimeout,
                eWaitConnectTimeout, //eRemotePickup
                eHangup,
                eRemoteRing,
                eRemoteRingTimeout,
                eRemoteCallId,
                eRemoteCallIdTimeout,
                eRemoteWaitReceiveTimeout,
                eAutoAnswer,
                eAnswerTimeout,
                eRemoteHangup,
                eStartRecord,
                eStopRecord,
                eStorageFull,
                eRecordMaxDurationOver,
                                //eSoftDial,
                //eRemoteBusy,
                //eRemotePickup,
                //eTimeout,
                //eLeaveWord,
                //eForceRecord,
                eSubSeqPickup,
                eSubSeqHangup,
            };
            enum Timer {
                tNull,
                tPickup,
                tDial,
                tWaitConnect,
                tRing,
                tCallId,
                tWaitReceive,
                tAnswer,
                                //tConnect,
                tRecord,
                tDisconnect,
                                //tError,
            };
            enum ConnectionType {
                ctHandset,
                ctHandsfree,
            };
            static PstnTelephone* const Instance();
            void RingControl(bool const isOpen = true);
            void BackLightControl(bool const isOpen = true);
            bool const HasCall() const {
                return hasCall_;
            }
            void SetCall() {
                hasCall_ = true;
            }
            void ClearCall() {
                hasCall_ = false;
            }
            void Mute();
            void SetTimeout(Timer const timer, size_t const timeout) {
                timers_.insert(std::make_pair(timer, timeout));
            }
            //void SetPickupTimeout(size_t const pickupTimeout) {
            //    timers_.insert(std::make_pair(tPickup, pickupTimeout));
            //}
            //void SetDialTimeout(size_t const dialTimeout) {
            //    timers_.insert(std::make_pair(tDial, dialTimeout));
            //}
            //void SetWaitConnectTimeout(size_t const waitConnectTimeout) {
            //    timers_.insert(std::make_pair(tWaitConnect, waitConnectTimeout));
            //}
            //void SetRingTimeout(size_t const ringTimeout) {
            //    timers_.insert(std::make_pair(tRing, ringTimeout));
            //}
            //void SetCallIdTimeout(size_t const callIdTimeout) {
            //    timers_.insert(std::make_pair(tCallId, callIdTimeout));
            //}
            //void SetWaitReceiveTimeout(size_t const waitReceiveTimeout) {
            //    timers_.insert(std::make_pair(tWaitReceive, waitReceiveTimeout));
            //}
            //void SetAnswerTimeout(size_t const answerTimeout) {
            //    timers_.insert(std::make_pair(tAnswer, answerTimeout));
            //}
            //void SetRecordTimeout(size_t const recordTimeout) {
            //    timers_.insert(std::make_pair(tRecord, recordTimeout));
            //}
            //void SetDisconnectTimeout(size_t const disconnectTimeout) {
            //    timers_.insert(std::make_pair(tDisconnect, disconnectTimeout));
            //}
            void SetForceRecord(bool const isForceRecord) {
            }
            BOOL DetectVersion(unsigned char c);
            int DetectStatus(unsigned char c);
            enum SoundType {
                stPstn,
                stAudioCard,
            };
            void SwitchSound(SoundType const soundType);
            void SetContact(Contact::Data::Contact* contact) {
                contact_ = contact;
            }
            std::wstring const FindSpeedDialNumber(int const serialNo) const;
            std::wstring const LastNumber() const {
                return lastNumber_;
            }
            ConnectionType const GetConnectionType() const {
                return connectionType_;
            }
            void SetConnectionType(ConnectionType const type) {
                connectionType_ = type;
            }
            void AppendCallIdCode(unsigned char const c) {
                callIdBuffer_[callIdLength_] = c;
                ++callIdLength_;
            }
            CALLID_INFO& DecodeCallId() {
                memset(&callId_, 0, sizeof(CALLID_INFO));
                //DecodeCallIDPackage(callIdBuffer_, &callId_);
                callId_ = CallId::Parse(callIdBuffer_);
                callIdLength_ = 0;
                return callId_;
            }
            void ForceHangup();
            void SoftHangup();
            void SoftPickup();
            void SoftDial(std::wstring const & number);
        protected:
            static void onTimer_(HWND window, UINT message, UINT timerId, DWORD tickCount) {
                window; //is null
                message; //is WM_TIMER
                tickCount; //::GetTickCount();
                ::KillTimer(0, timerId); //the timerId is currentTimerId_
                switch (PstnTelephone::Instance()->currentTimer_) {
                case tPickup:
                    PstnTelephone::Instance()->fireEvent(PstnTelephone::ePickupTimeout, 0);
                    break;
                case tDial:
                    PstnTelephone::Instance()->fireEvent(PstnTelephone::eDialTimeout, 0);
                    break;
                case tWaitConnect:
                    PstnTelephone::Instance()->fireEvent(PstnTelephone::eWaitConnectTimeout, 0);
                    break;
                case tRing:
                    PstnTelephone::Instance()->fireEvent(PstnTelephone::eRemoteRingTimeout, 0);
                    break;
                case tCallId:
                    PstnTelephone::Instance()->fireEvent(PstnTelephone::eRemoteCallIdTimeout, 0);
                    break;
                case tWaitReceive:
                    PstnTelephone::Instance()->fireEvent(PstnTelephone::eRemoteWaitReceiveTimeout, 0);
                    break;
                case tAnswer:
                    PstnTelephone::Instance()->fireEvent(PstnTelephone::eAnswerTimeout, 0);
                    break;
                default:
                    break;
                }
                PstnTelephone::Instance()->currentTimer_ = tNull;
            }
            //void pickup_(void* param);
            //void ringAndShow_(void* param) {
            //    ring_(param);
            //    show_(param);
            //}
            //void recordCallIdAndShow_(void* param) {
            //    recordCallId_(param);
            //    show_(param);
            //}
            //void softPickupAndSoftDial_(void* param) {
            //    softPickup_(param);
            //    softDial_(param);
            //}
            //void hide_(void* param);
            //void dial_(void* param);
            //void pickupTimeout_(void* param);
            //void softDial_(void* param);
            //void recordCallId_(void* param);
            //void connect_(void* param);
            //void connectAndStartRecord_(void* param) {
            //    connect_(param);
            //    startRecord_(param);
            //}
            //void recordCallInfoAndReject_(void* param) {
            //    recordCallInfo_(param);
            //    reject_(param);
            //}
            //void recordCallInfoAndCallinTimeout_(void* param) {
            //    recordCallInfo_(param);
            //    callinTimeout_(param);
            //}
            //void startRecord_(void* param);
            //void recordCallInfoAndRemoteHangup_(void* param) {
            //    recordCallInfo_(param);
            //    remoteHangup_(param);
            //}
            //void recordCallInfoAndHangup_(void* param) {
            //    recordCallInfo_(param);
            //    hangup_(param);
            //}
            //void ring_(void* param);
            //void recordCallInfoAndCallIdTimeout_(void* param) {
            //    recordCallInfo_(param);
            //    callIdTimeout_(param);
            //}
            //void connectAndPlayPromptAndStartRecord_(void* param) {
            //    connect_(param);
            //    playPrompt_(param);
            //    startRecord_(param);
            //}
            //void recordCallInfoAndConnectTimeout_(void* param) {
            //    recordCallInfo_(param);
            //    connectTimeout_(param);
            //}
            //void recordCallInfoAndRemoteBusy_(void* param) {
            //    recordCallInfo_(param);
            //    remoteBusy_(param);
            //}
            //void subDial_(void* param);
            //void recordCallInfoAndDialTimeout_(void* param) {
            //    recordCallInfo_(param);
            //    dialTimeout_(param);
            //}
            //void stopRecord_(void* param);
            //void stopRecordAndRecordCallInfoAndRemoteHangup_(void* param) {
            //    stopRecord_(param);
            //    recordCallInfo_(param);
            //    remoteHangup_(param);
            //}
            //void stopRecordAndRecordCallInfoAndHangup_(void* param) {
            //    stopRecord_(param);
            //    recordCallInfo_(param);
            //    hangup_(param);
            //}
            //void stopRecordAndRecordTimeout_(void* param) {
            //    stopRecord_(param);
            //    recordTimeout_(param);
            //}
        private:
            //void show_(void* param);
            //void softPickup_(void* param);
            //void recordCallInfo_(void* param);
            //void reject_(void* param);
            //void callinTimeout_(void* param);
            //void remoteHangup_(void* param);
            //void hangup_(void* param);
            //void callIdTimeout_(void* param);
            //void playPrompt_(void* param);
            //void connectTimeout_(void* param);
            //void remoteBusy_(void* param);
            //void dialTimeout_(void* param);
            //void recordTimeout_(void* param);

            void setPickupTimerAndShowClearCallForm_(void* param) {
                param;
                setTimer_(tPickup);
                View::callForm()->SetNumber(L"");
                View::callForm()->SetAddress(L"");
                View::callForm()->ShowWindow(SW_SHOW);
            }
            void setRingTimerAndNewAndSetTypeAndSetStartTimeAndShowClearCallForm_(void* param) {
                param;
                setTimer_(tRing);
                newCallInfo_();
                setCallInfoType_(Data::citDialed);
                setCallInfoStartTime_(Util::Timestamp::GetCurrentTime());
                showCallForm_();
            }
            void setCallIdTimerAndNewAndSetTypeAndSetStartTimeAndRecordCallIdAndShowCallForm_(void* param) {
                setTimer_(tCallId);
                newCallInfo_();
                setCallInfoType_(Data::citReceived);
                setCallInfoStartTime_(Util::Timestamp::GetCurrentTime());
                recordCallId_(param);
                showCallForm_();
            }
            void clearPickupTimerAndSetDialTimerAndNewAndSetTypeAndSetStartTimeAndRecordFirstDialKeyAndNotifyCallFormNumber_(void* param) {
                clearTimer_();
                setTimer_(tDial);
                newCallInfo_();
                setCallInfoType_(Data::citReceived);
                setCallInfoStartTime_(Util::Timestamp::GetCurrentTime());
                appendNumberCode_(reinterpret_cast<wchar_t>(param));
                showCallForm_();
            }
            void notifyCallFormPickupTimeout_(void* param) {
                View::callForm()->PostMessageW(View::UM_TEL_PICKUP_TIMEOUT, 0, 0);
            }
            void clearDialTimerAndSetTypeAndDurationAndInsertCallInfoAndHangupAndHideCallForm_(void* param) {
                clearTimer_();
                setCallInfoType_(Data::citMissed);
                setCallInfoDuration_(Util::Timestamp::GetCurrentTime() - callInfo_->startTime());
                insertCallInfo_(param);
                //hangup
                hideCallForm_();
            }
            void resetDialTimerAndRecordDialKeyAndNotifyCallFormNumber_(void* param) {
                clearTimer_();
                setTimer_(tDial);
                appendNumberCode_(reinterpret_cast<wchar_t>(param));
            }
            void setWaitConnectTimerAndStopRecordDialKeyAndDisableCallFormNumberChange_(void* param) {
                setTimer_(tWaitConnect);
                View::callForm()->PostMessageW(View::UM_TEL_DIAL_END, 0, 0);
            }
            void setDurationAndInsertCallInfoAndHideCallForm_(void* param) {
                setCallInfoDuration_(Util::Timestamp::GetCurrentTime() - callInfo_->startTime());
                insertCallInfo_(param);
                hideCallForm_();
            }
            void clearWaitConnectTimerAndUpdateStartTimeAndNotifyCallFormConnected_(void* param) {
                clearTimer_();
                setCallInfoStartTime_(Util::Timestamp::GetCurrentTime());
                View::callForm()->PostMessageW(View::UM_TEL_CONNECT, 0, 0);
            }
            void recordCallIdAndNotifyCallFormNumber_(void* param) {
                recordCallId_(param);
                View::callForm()->PostMessageW(View::UM_TEL_CALLID, 0, reinterpret_cast<LPARAM>(param));
            }
            void clearWaitConnectTimerAndSetDurationAndInsertCallInfoAndHideCallForm_(void* param) {
                clearTimer_();
                setCallInfoDuration_(Util::Timestamp::GetCurrentTime() - callInfo_->startTime());
                insertCallInfo_(param);
                hideCallForm_();
            }
            void subDial_(void* param) {
                LPARAM keyCode = reinterpret_cast<int>(param);
                //send sub-dial message to call form
                View::callForm()->PostMessage(View::UM_TEL_SUB_DIAL, 0, keyCode);
            }
            void setRecordTimerAndNewRecordAndSetRecordStartTimeAndStartRecordAndNotifyCallFormStartRecord_(void* param) {
                setTimer_(tRecord);
                newRecord_();
                setRecordStartTime_(Util::Timestamp::GetCurrentTime());
                View::callForm()->PostMessageW(View::UM_TEL_START_RECORD, 0, 0);
            }
            void setDisconnectTimerAndSetDurationAndInsertCallInfoAndNotifyCallFormRemoteHangup_(void* param) {
                setTimer_(tDisconnect);
                setCallInfoDuration_(Util::Timestamp::GetCurrentTime() - callInfo_->startTime());
                insertCallInfo_(param);
                View::callForm()->PostMessageW(View::UM_TEL_REMOTE_HANGUP, 0, 0);
            }
            void clearRingTimerAndSetCallIdTimerAndRecordCallIdAndNotifyCallFormNumber_(void* param) {
                clearTimer_();
                setTimer_(tCallId);
                recordCallId_(param);
                std::wstring number = *static_cast<std::wstring*>(param);
                View::callForm()->SetNumber(number);
            }
            void setRecordDurationAndInsertRecordAndStopRecordAndNotifyCallFormStopRecord_(void* param) {
                //setRecordDuration_(Util::Timestamp::GetCurrentTime() - record_->startTime());
                insertRecord_();
                callInfo_->StopRecord();
                View::callForm()->PostMessageW(View::UM_TEL_STOP_RECORD, 0, 0);
            }
            void setDisconnectTimerAndStopRecordAndInsertCallInfoAndNotifyCallFormRemoteHangup_(void* param) {
                setTimer_(tDisconnect);
                callInfo_->StopRecord();
                insertCallInfo_(param);
                View::callForm()->PostMessageW(View::UM_TEL_REMOTE_HANGUP, 0, 0);
            }
            void stopRecordAndInsertCallInfoAndHideCallForm_(void* param) {
                callInfo_->StopRecord();
                insertCallInfo_(param);
                hideCallForm_();
            }
            void stopRecordAndNotifyCallFormStorageFull_(void* param) {
                callInfo_->StopRecord();
                View::callForm()->PostMessageW(View::UM_TEL_STORAGE_FULL, 0, 0);
            }
            void stopRecordAndNotifyCallFormMaxDurationOver_(void* param) {
                callInfo_->StopRecord();
                View::callForm()->PostMessageW(View::UM_TEL_MAX_DURATION_OVER, 0, 0);
            }
            void clearRingTimerAndNotifyCallFormConnected_(void* param) {
                clearTimer_();
                View::callForm()->PostMessageW(View::UM_TEL_CONNECT, 0, 0);
            }
            void clearRingTimerAndInsertCallInfoAndHideCallForm_(void* param) {
                clearTimer_();
                insertCallInfo_(param);
                hideCallForm_();
            }
            void clearRingTimerAndInsertCallInfoAndNotifyCallFormRemoteHangup_(void* param) {
                clearTimer_();
                insertCallInfo_(param);
                View::callForm()->PostMessageW(View::UM_TEL_REMOTE_HANGUP, 0, 0);
            }
            void setAnswerTimerAndInsertCallInfoAndNotifyCallFormCallinTimeout_(void* param) {
                setTimer_(tAnswer);
                insertCallInfo_(param);
                View::callForm()->PostMessageW(View::UM_TEL_CALLIN_TIMEOUT, 0, 0);
            }
            void clearRingTimerAndPlayPromptAndNotifyCallFormConnected_(void* param) {
                clearTimer_();
                playPrompt_(param);
                View::callForm()->PostMessageW(View::UM_TEL_CONNECT, 0, 0);
            }
            void clearCallIdTimerAndSetWaitReceiveTimerAndPlayPromptAndNotifyCallFormRing_(void* param) {
                clearTimer_();
                setTimer_(tWaitReceive);
                View::callForm()->PostMessageW(View::UM_TEL_RING, 0, 0);
            }
            void clearCallIdTimerAndNotifyCallFormConnected_(void* param) {
                clearTimer_();
                View::callForm()->PostMessageW(View::UM_TEL_CONNECT, 0, 0);
            }
            void clearCallIdTimerAndInsertCallInfoAndHideCallForm_(void* param) {
                clearTimer_();
                insertCallInfo_(param);
                hideCallForm_();
            }
            void clearCallIdTimerAndInsertCallInfoAndNotifyCallFormRemoteHangup_(void* param) {
                clearTimer_();
                insertCallInfo_(param);
                View::callForm()->PostMessageW(View::UM_TEL_REMOTE_HANGUP, 0, 0);
            }
            void clearCallIdTimerAndSetWaitReceiveTimerAndNotifyCallFormRing_(void* param) {
                clearTimer_();
                setTimer_(tWaitReceive);
                View::callForm()->PostMessageW(View::UM_TEL_RING, 0, 0);
            }
            void setAnswerTimerAndInsertCallInfoAndNotifyCallFormCallIdTimeout_(void* param) {
                setTimer_(tAnswer);
                insertCallInfo_(param);
                View::callForm()->PostMessageW(View::UM_TEL_CALLID_TIMEOUT, 0, 0);
            }
            void clearCallIdTimerAndPlayPromptAndNotifyCallFormConnected_(void* param) {
                clearTimer_();
                playPrompt_(param);
                View::callForm()->PostMessageW(View::UM_TEL_CONNECT, 0, 0);
            }
            void clearWaitReceiveTimerAndPlayPromptAndNotifyCallFormConnected_(void* param) {
                clearTimer_();
                playPrompt_(param);
                View::callForm()->PostMessageW(View::UM_TEL_CONNECT, 0, 0);
            }
            void clearWaitReceiveTimerAndNotifyCallFormConnected_(void* param) {
                clearTimer_();
                View::callForm()->PostMessageW(View::UM_TEL_CONNECT, 0, 0);
            }
            void clearWaitReceiveTimerAndInsertCallInfoAndNotifyCallFormRemoteHangup_(void* param) {
                clearTimer_();
                insertCallInfo_(param);
                View::callForm()->PostMessageW(View::UM_TEL_REMOTE_HANGUP, 0, 0);
            }

            void showCallForm_() {
                View::callForm()->ShowWindow(SW_SHOW);
            }
            void hideCallForm_(void* param) {
                param;
                hideCallForm_();
            }
            void hideCallForm_() {
                View::callForm()->ShowWindow(SW_HIDE);
            }
            void clearPickupTimerAndHideCallForm_(void* param) {
                param;
                clearTimer_();
                View::callForm()->ShowWindow(SW_HIDE);
            }
            void appendCallFormNumberCode_(wchar_t const key) {
            }
            void setCallFormStartTime_(Util::Timestamp const & time) {
            }

            void newCallInfo_() {
                callInfo_ = new Data::CallInfo();
            }
            void setCallInfoType_(Data::CallInfoType const type) {
                callInfo_->type(type);
            }
            void setCallInfoStartTime_(Util::Timestamp const & time) {
                callInfo_->startTime(time);
            }
            void setCallInfoNumber_(std::wstring const & number) {
                callInfo_->telephoneNumber(number);
            }
            void appendNumberCode_(wchar_t const key) {
                callInfo_->telephoneNumber(callInfo_->telephoneNumber() + key);
            }
            void setCallInfoDuration_(Util::TimeSpan const & duration) {
                callInfo_->duration(duration);
            }
            void insertCallInfo_(void* param) {
                callInfo_->Insert();
                delete callInfo_;
                callInfo_ = 0;
            }

            void recordCallId_(void* param) {
                CALLID_INFO* callId = reinterpret_cast<CALLID_INFO*>(param);
                lastNumber_ = Util::Text::StringOp::FromUTF8(callId->number);
                View::callForm()->PostMessage(View::UM_TEL_CALLID, 0, reinterpret_cast<LPARAM>(callId));
            }

            void newRecord_() {
                /*record_ = */callInfo_->Record(); //new Data::Record();
            }
            void setRecordStartTime_(Util::Timestamp const & time) {
                //record_->startTime(time);
            }
            void setRecordDuration_(Util::TimeSpan const & duration) {
                //record_->duration(duration);
            }
            void insertRecord_() {
                //record_->Insert();
                //delete record_;
                //record_ = 0;
            }

            void playPrompt_(void* param) {
                std::wstring ringtone = Contact::Data::ContactCategory::GetRingtone(lastNumber_);
                if (ringtone != L"") {
                    //Util::RingtonePlayer::Instance()->Start(ringtone);
                }
            }

            void clearTimer_() {
                if (currentTimerId_) {
                    ::KillTimer(0, currentTimerId_);
                    currentTimerId_ = 0;
                }
            }

            void setTimer_(Timer const timer) {
                currentTimerId_ = ::SetTimer(0, timer, timers_[timer], onTimer_);
            }

            //void setPickupTimer_() {
            //    currentTimerId_ = ::SetTimer(0, tPickup, timers_[tPickup], onTimer_);
            //}
            //void setDialTimer_() {
            //    currentTimerId_ = ::SetTimer(0, tDial, timers_[tDial], onTimer_);
            //}
            //void resetDialTimer_() {
            //    clearTimer_();
            //    setDialTimer_();
            //}
            //void setWaitConnectTimer_() {
            //    currentTimerId_ = ::SetTimer(0, tWaitConnect, timers_[tWaitConnect], onTimer_);
            //}
            //void setRingTimer_() {
            //    currentTimerId_ = ::SetTimer(0, tRing, timers_[tRing], onTimer_);
            //}
            //void setCallIdTimer_() {
            //    currentTimerId_ = ::SetTimer(0, tCallId, timers_[tCallId], onTimer_);
            //}
            //void setWaitReceiveTimer_() {
            //    currentTimerId_ = ::SetTimer(0, tWaitReceive, timers_[tWaitReceive], onTimer_);
            //}
            //void setAnswerTimer_() {
            //    currentTimerId_ = ::SetTimer(0, tAnswer, timers_[tAnswer], onTimer_);
            //}
            //void setRecordTimer_() {
            //    currentTimerId_ = ::SetTimer(0, tRecord, timers_[tRecord], onTimer_);
            //}
            //void setDisconnectTimer_() {
            //    currentTimerId_ = ::SetTimer(0, tDisconnect, timers_[tDisconnect], onTimer_);
            //}
        private:
            PstnTelephone();
            ~PstnTelephone();
            //void Hangup();
            //void ForceHangup();
            //void HandsfreePickup();
            //void PickupToRecord();
            //void Dial(char* nuber, int dialType = 0); //dialType: 0 免提，1 摘机
            void numberToDialNumber_(char* const dst, std::wstring const & src) {
                char* pDst = const_cast<char*>(dst);
                size_t count = min(src.length(), 128);
                for (size_t i = 0; i < count; ++i) {
                    char c = static_cast<char>(src[i]);
                    *pDst = c;
                    if (mapTable_.find(c) != mapTable_.end()) {
                        *pDst = mapTable_[c];
                    }
                    ++pDst;
                }
                numberLength_ = count;
            }
            void writePort_(unsigned char const * const data, int const dataLength) {
                for (int i = 0; i < dataLength; ++i) {
                    unsigned char c = data[i];
#if (RS232_INVERT == 1)
                    c = invertBit_(c);
#endif
                    serialPort_->Write((const unsigned char * const) & c, 1);
                }
            }
            unsigned char const invertBit_(unsigned char const c) {
                unsigned char r = 0;
                r |= (((c >> 0) & 0x01) << 7);
                r |= (((c >> 1) & 0x01) << 6);
                r |= (((c >> 2) & 0x01) << 5);
                r |= (((c >> 3) & 0x01) << 4);
                r |= (((c >> 4) & 0x01) << 3);
                r |= (((c >> 5) & 0x01) << 2);
                r |= (((c >> 6) & 0x01) << 1);
                r |= (((c >> 7) & 0x01) << 0);
                return r;
            }
        private:
            bool hasCall_;
            Util::RS232* serialPort_;
            bool isBackLightOpened_; //记录背光是否是打开
            bool needUserId_;
            char detectCode_[24];
            bool isDialing_;
            std::map<char, char> mapTable_;
            char number_[128];
            int numberLength_;
            std::wstring lastNumber_;
            //char version_[24];
            Contact::Data::Contact* contact_;
            Data::CallInfo* callInfo_;
            //Data::Record* record_;
            ConnectionType connectionType_;
            unsigned char callIdBuffer_[128];
            int callIdLength_;
            CALLID_INFO callId_;
            //std::vector<TimerStruct> timers_;
            std::map<Timer, size_t> timers_;
            Timer currentTimer_;
            UINT_PTR currentTimerId_;
        };
    }
}

#endif //__TELEPHONE_LOGICAL_TELEPHONE_H__
