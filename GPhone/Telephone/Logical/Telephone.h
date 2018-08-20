#ifndef __TELEPHONE_LOGICAL_TELEPHONE_H__
#define __TELEPHONE_LOGICAL_TELEPHONE_H__

#include "../../Util/SerialPort.h"
#include "../../Util/Fsm.h"
#include "../../Util/WaveRecorder.h"
#include "../Data/CallInfo.h"
#include "../../Contact/Data/Contact.h"
#include "CallId.h"

namespace Telephone {
    namespace Logical {
        class Telephone : public Util::FSM<Telephone> {
            friend void ParseTelephoneData(unsigned char const* const data, unsigned int const length);
            friend void DialThread();
        public:
            enum State {
                sIdle,
                sSubSeqPickuping,
                sPickuping,
                sDialing,
                sWaitingConnect,
                sConnecting,
                sRinging,
                sCallId,
                sWaitingReceive,
                sDisconnecting,
                sRecording,
                sError,
            };
            enum Event {
                eHangup,
                ePickup,
                eDial,
                eSoftDial,
                eRemotePickup,
                eRemoteRing,
                eRemoteCallId,
                eRemoteHangup,
                eRemoteBusy,
                eTimeout,
                eLeaveWord,
                eForceRecord,
                eStartRecord,
                eStopRecord,
                eSubSeqPickup,
                eSubSeqHangup,
            };
            enum Timer {
                tPickup = 5000,
                tDial = 3000,
                tWaitConnect = 3100,
                tConnect = 1000,
                tRing,
                tCallId = 8000,
                tWaitReceive = 3500,
                tDisconnect,
                tRecord,
                tError,
            };
            enum ConnectionType {
                ctHandset,
                ctHandsfree,
            };
            static Telephone* const Instance();
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
            BOOL DetectVersion(unsigned char c);
            int DetectStatus(unsigned char c);
            void SetContact(Util::shared_ptr<Contact::Data::Contact>& contact) {
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
            void SoftHangup();
            void SoftPickup();
        protected:
            void pickup_(void* param);
            void ringAndShow_(void* param) {
                ring_(param);
                show_(param);
            }
            void recordCallIdAndShow_(void* param) {
                recordCallId_(param);
                show_(param);
            }
            void softPickupAndSoftDial_(void* param) {
                softPickup_(param);
                softDial_(param);
            }
            void hide_(void* param);
            void dial_(void* param);
            void pickupTimeout_(void* param);
            void softDial_(void* param);
            void recordCallId_(void* param);
            void connect_(void* param);
            void connectAndStartRecord_(void* param) {
                connect_(param);
                startRecord_(param);
            }
            void recordCallInfoAndReject_(void* param) {
                recordCallInfo_(param);
                reject_(param);
            }
            void recordCallInfoAndCallinTimeout_(void* param) {
                recordCallInfo_(param);
                callinTimeout_(param);
            }
            void startRecord_(void* param);
            void recordCallInfoAndRemoteHangup_(void* param) {
                recordCallInfo_(param);
                remoteHangup_(param);
            }
            void recordCallInfoAndHangup_(void* param) {
                recordCallInfo_(param);
                hangup_(param);
            }
            void ring_(void* param);
            void recordCallInfoAndCallIdTimeout_(void* param) {
                recordCallInfo_(param);
                callIdTimeout_(param);
            }
            void connectAndPlayPromptAndStartRecord_(void* param) {
                connect_(param);
                playPrompt_(param);
                startRecord_(param);
            }
            void recordCallInfoAndConnectTimeout_(void* param) {
                recordCallInfo_(param);
                connectTimeout_(param);
            }
            void recordCallInfoAndRemoteBusy_(void* param) {
                recordCallInfo_(param);
                remoteBusy_(param);
            }
            void subDial_(void* param);
            void recordCallInfoAndDialTimeout_(void* param) {
                recordCallInfo_(param);
                dialTimeout_(param);
            }
            void stopRecord_(void* param);
            void stopRecordAndRecordCallInfoAndRemoteHangup_(void* param) {
                stopRecord_(param);
                recordCallInfo_(param);
                remoteHangup_(param);
            }
            void stopRecordAndRecordCallInfoAndHangup_(void* param) {
                stopRecord_(param);
                recordCallInfo_(param);
                hangup_(param);
            }
            void stopRecordAndRecordTimeout_(void* param) {
                stopRecord_(param);
                recordTimeout_(param);
            }
        private:
            void show_(void* param);
            void softPickup_(void* param);
            void recordCallInfo_(void* param);
            void reject_(void* param);
            void callinTimeout_(void* param);
            void remoteHangup_(void* param);
            void hangup_(void* param);
            void callIdTimeout_(void* param);
            void playPrompt_(void* param);
            void connectTimeout_(void* param);
            void remoteBusy_(void* param);
            void dialTimeout_(void* param);
            void recordTimeout_(void* param);
        private:
            Telephone();
            ~Telephone();
            //void Hangup();
            //void ForceHangup();
            //void HandsfreePickup();
            //void PickupToRecord();
            //void Dial(char* nuber, int dialType = 0); //dialType: 0 免提，1 摘机
            void numberToDialNumber_(char* const dst, std::wstring const& src) {
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
            void writePort_(unsigned char const* const data, int const dataLength) {
                for (int i = 0; i < dataLength; ++i) {
                    unsigned char c = data[i];
#if (RS232_INVERT == 1)
                    c = invertBit_(c);
#endif
                    serialPort_->Write((const unsigned char * const)&c, 1);
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
            Util::shared_ptr<Contact::Data::Contact> contact_;
            Util::shared_ptr<Data::CallInfo> callInfo_;
            ConnectionType connectionType_;
            unsigned char callIdBuffer_[128];
            int callIdLength_;
            CALLID_INFO callId_;
        };
    }
}

#endif //__TELEPHONE_LOGICAL_TELEPHONE_H__
