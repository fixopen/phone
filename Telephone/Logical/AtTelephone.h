#ifndef __TELEPHONE_LOGICAL_TELEPHONEWARP_H__
#define __TELEPHONE_LOGICAL_TELEPHONEWARP_H__

#include "../../Util/Fsm.h"
#include "../../Util/Observer.h"
#include "../../Util/SerialPort.h"

namespace Telephone {
    namespace Logical {
        class AtTelephone : public Util::FSM<AtTelephone>, public Util::Observer {
        public:
            enum TransferType {
                ttUnconditional,
                ttBusy,
                ttNotReply,
                ttNotReach,
            };

            enum State {
                sIdle,
                sPickuping,
                sDialing,
                sWaitConnecting,
				sVideoWaitConnecting,
				sMediaStarting,
				sMediaConsultation,
				sMediaStoping,
                sConnecting,
                sVideoConnecting,
                sRecording,
                sHolding,
                sRemoteDisconnecting,
                sIncalling,
                sError,
            };
            enum Event {
                ePickup,
                eHangup,
                eDial,
                eVideoDial,
                eSendNumber,
				eVideoStart,
				eVideoStop,
				eVideoConsultation,
				eConnect,
                eStartRecord,
                eStopRecord,
                eLeaveWord,
                eHold,
                eUnhold,
                eReject,
                eTimeout,
                eStorageLow,
                eError,
                eRemoteHangup,
                eRemoteCall,
                eRemoteReject,
                eRemoteRing,
				eRemoteReceived,
                eRemoteVideoStart,
                eRemoteVideoStop,
                eRemoteVideoConsultation,
                eRemoteVideoData,
            };
			enum Action {
			};
            static AtTelephone* const Instance() {
                static AtTelephone* result = new AtTelephone();
                return result;
            }
            AtTelephone(/*Util::Observable* atChannel, Util::Observable* keypad*/);
            ~AtTelephone();
            virtual void Update(std::string const& data);
            void Dial(std::string const& number);
            void CallLimit();
            void ReportSCE();
            void DectectNetworkType();
            void CallWaiting(bool const isTrue);
            void CallSwitch();
            void AutoAnswer(int const secondCount);
            void Handsfree();
            void SetVideoChannel(int const channelNumber = 3);
            void VideoDial(std::string const& number);
            void VideoChannelSetup();
            void VideoChannelTeardown();
            void VideoChannelConsultation();
            void VideoDataControl(bool const isSend);
            void VideoHangup();
		    void Hangup();
		    void Answer();
            void Reject();
            void Volume();
            void Mute(bool const isTrue);
			void SetAudioScenes(int scenesNo);
            void SetTransfer(TransferType const type, bool const isEnable);
		    void Redial();
		    void SubDial(char const number);
        protected:
            void showDialpad_(void* p);
            void recordDialCode_(void* p);
            void waitRemoteAnswer_(void* p);
            void hideDialpadAndShowCallController_(void* p);
			void hideDialpadAndShowMediaController_(void* p);
            void startRecord_(void* p);
            void stopRecord_(void* p);
            void stopRecordAndSaveFile_(void* p);
            void hideCallControllerAndRecordHistory_(void* p);
            void showRemoteCallAndRecordCallId_(void* p);
            void showCallController_(void* p);
            void recordHistory_(void* p);
            void hideCallController_(void* p);
            void hideDialpad_(void* p);
            void finishWaitRemoteAnswer_(void* p);
            void hideRemoteCallAndRecordHistory_(void* p);
            void stopRecordAndSaveFileAndHideCallControllerAndRecordHistory_(void* p);
			void startMedia_(void* p);
			void stopMedia_(void* p);
			void consultationMedia_(void* p);
        private:
            static void telephoneParser(std::string const& data);
            void saveFile_(void* p);
            void showRemoteCall_(void* p);
            void recordCallId_(void* p);
            void hideRemoteCall_(void* p);
            Util::RS232* videoPort_;
        };
    }
}

#endif // __TELEPHONE_LOGICAL_TELEPHONEWARP_H__
