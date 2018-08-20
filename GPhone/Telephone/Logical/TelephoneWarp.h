#ifndef __TELEPHONE_LOGICAL_TELEPHONEWARP_H__
#define __TELEPHONE_LOGICAL_TELEPHONEWARP_H__

#include "../../Util/Fsm.h"
#include "../../Util/Observer.h"

namespace Telephone {
    namespace Logical {
        class TelephoneWarp : public Util::FSM<TelephoneWarp>, public Util::Observer {
        public:
            enum State {
                sIdle,
                sPickuping,
                sDialing,
                sWaitConnecting,
                sConnecting,
                sVideoConnecting,
                sRecording,
                sRemoteBusy,
                sIncalling,
                sError,
            };
            enum Event {
                ePickup,
                eHangup,
                eDial,
                eVideoDial,
                eSendNumber,
                eStartRecord,
                eStopRecord,
                eLeaveWord,
                eReject,
                eTimeout,
                eStorageLow,
                eError,
                eRemotePickup,
                eRemoteHangup,
                eRemoteCall,
                eRemoteReject,
                eRemoteBusy,
                eRemoteVideoStart,
                eRemoteVideoStop,
                eRemoteVideoCodecConfig,
                eRemoteVideoData,
            };
            static TelephoneWarp* const Instance() {
                TelephoneWarp* result = new TelephoneWarp();
                return result;
            }
            TelephoneWarp(/*Util::Observable* atChannel, Util::Observable* keypad*/);
            virtual void Update(std::string const& data);
            void Dial(std::string const& number);
            void VideoDial(std::string const& number);
            void VideoChannelSetup();
		    void Hangup();
		    void Answer();
            void Reject();
		    void Redial();
		    void SubDial(char const number);
        protected:
            void showDialpad_(void* p);
            void recordDialCode_(void* p);
            void waitRemoteAnswer_(void* p);
            void hideDialpadAndShowCallController_(void* p);
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
        private:
            static void telephoneParser(std::string const& data);
            void saveFile_(void* p);
            void showRemoteCall_(void* p);
            void recordCallId_(void* p);
            void hideRemoteCall_(void* p);
        };
    }
}

#endif // __TELEPHONE_LOGICAL_TELEPHONEWARP_H__
