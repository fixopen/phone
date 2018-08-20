#ifndef __TELEPHONE_LOGICAL_TELEPHONEWARP_H__
#define __TELEPHONE_LOGICAL_TELEPHONEWARP_H__

#include "../../Util/Fsm.h" //for FSM
#include "../../Util/Observer.h" //for Observer
#include "../../Util/SerialPort.h" //for RS232
#include "../../Util/Video/Display.h" //for Display
#include "../../Util/Video/VideoCodec.h" //for VideoEncoder VideoDecoder
//#include "../../Util/Video/Camera6410.h" //for Camera6410
//#include "../../Util/Video/LocalCamera6410.h" //for LocalCamera6410
#include "../../Util/Video/Camera/LocalCamera6410.h" //for LocalCamera6410

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

            enum TransferAction {
                taDisable,
                taEnable,
                taQuery,
                taRegistration,
                taErasure,
            };

            enum State {
                sIdle,
                sPickuping,
                sDialing,
                sWaitConnecting,
                sWaitVideoConnecting,
                sMediaConsultation,
                sRemoteDisconnecting,
                sRemoteCalling,
                sRemoteVideoCalling,
                sVideoAnswered,
                sRemoteVideoDisconnecting,
                sConnecting,
                sVideoConnecting,
                sRecording,
                sVideoRecording,
                sHolding,
                sError,
            };

            enum Event {
                eHangup,
                ePickup,
                eDial,
                eSendCall,
                eSendVideoCall,
                eRemoteAnswer,
                eRemoteReject,
                                //eRemoteVideoAnswer,
                eRemoteVideoReject,
                eRemoteVideoHangup,
                eRemoteHangup,
                eRemoteCall,
                eRemoteVideoCall,
                eAnswer,
                eReject,
                eVideoAnswer,
                eVideoReject,
                eVideoHangup,
                eRemoteMediaStart,
                eRemoteMediaConsultation,
                eRemoteMediaStop,
                eHold,
                eUnhold,
                eStartRecord,
                eStopRecord,
                eStartVideoRecord,
                eStopVideoRecord,
                eStorageLow,

                                //eVideoStart,
                //eVideoStop,
                //eVideoConsultation,
                //eConnect,
                //eLeaveWord,

                //eReject,
                //eTimeout,
                //eError,
                //eRemoteHangup,
                //eRemoteReject,
                //eRemoteRing,
                //eRemoteReceived,
                //eRemoteVideoData,
            };

            enum Action {
            };

            static AtTelephone* const Instance() {
                static AtTelephone* result = new AtTelephone();
                return result;
            }

            AtTelephone(/*Util::Observable* atChannel, Util::Observable* keypad*/);
            ~AtTelephone();
            virtual void Update(std::string const & data);
            void Dial(std::string const & number);
            void Answer();
            void Reject();
            void Hangup();
            void VideoDial(std::string const & number);
            void VideoAnswer();
            void VideoReject();
            void VideoHangup();

            void SetVideoChannel(int const channelNumber = 3);
            void SetDisplayWindow(HWND const local, HWND const remote);
            void InitCamera();
            void SendVideoData(unsigned char const * const data, size_t const length);

            void CallLimit();
            void ReportScenes();
            void DectectNetworkType();
            void CallWaiting(bool const isTrue);
            void CallSwitch();
            void AutoAnswer(int const secondCount);
            void Handsfree();
            void VideoChannelSetup();
            void VideoChannelTeardown();
            void VideoChannelConsultation();
            void VideoDataControl(bool const isSend);
            void Volume();
            void Mute(bool const isTrue);
            void SetAudioScenes(int scenesNo);
            void SetTransfer(TransferType const type, TransferAction const action, std::string const number, size_t const timeout = 20);
            void Redial();
            void SubDial(char const number);
        protected:
            void showDialpad_(void* param);
            void recordDialCode_(void* param);
            void waitRemoteAnswer_(void* param);
            void waitRemoteVideoAnswer_(void* param);
            void showRemoteCallAndRecordCallId_(void* param);
            void showRemoteVideoCallAndRecordCallId_(void* param);
            void hideDialpad_(void* param);
            void finishWaitRemoteAnswer_(void* param);
            void hideDialpadAndShowCallController_(void* param);
            void startMedia_(void* param);
            void autoRedialAudio_(void* param);
            void finishWaitRemoteVideoAnswer_(void* param);
            void startRecord_(void* param);
            void hold_(void* param);
            void hideCallControllerAndRecordHistory_(void* param);
            void recordHistory_(void* param);
            void consultationMedia_(void* param);
            void stopMedia_(void* param);
            void hideRemoteCallAndRecordHistory_(void* param);
            void answer_(void* param);
            void stopMediaAndRedial_(void* param);
            void hideCallController_(void* param);
            void stopRecordAndSaveFile_(void* param);
            void stopRecordAndSaveFileAndHideCallControllerAndRecordHistory_(void* param);
            void stopRecord_(void* param);
            void showCallController_(void* param);
        private:
            static void remoteVideoDataProcess_(unsigned char const * const data, unsigned int const length);
            static void cameraDataProcess_(unsigned char const * const data, unsigned int const length);
        private:
            Util::RS232* videoPort_;
            Util::Video::VideoDecoder* decoder_;
            bool isDecoderInited_;
            Util::Video::VideoEncoder* encoder_;
            bool isEncoderInited_;
            Util::Video::Display* localDisplay_;
            Util::Video::Display* remoteDisplay_;
            //Util::Video::Camera6410* camera_;
            Util::Video::LocalCamera6410* camera_;
            std::string number_;
        public:
            //@@stat.
            std::vector<int> ticks_;
            std::vector<int> cameraDataSize_;
            std::vector<int> receiveDataSize_;
            std::vector<int> receiveTicks_;
        };
    }
}

#endif // __TELEPHONE_LOGICAL_TELEPHONEWARP_H__
