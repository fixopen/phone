#ifndef __TELEPHONE_BUSYTONEDETECTTHREAD_H__
#define __TELEPHONE_BUSYTONEDETECTTHREAD_H__

#include "../../Util/Thread.h"
#include "../../Util/Audio/OggCodec.h"
#include "inc/sabtd/SABTD.h"
#include <queue>

namespace Telephone {
    namespace Logical {
        class BusyToneDetectThread : public Util::Thread {
        public:
            static BusyToneDetectThread* const Instance() {
                static BusyToneDetectThread* thread = 0;
                if (!thread) {
                    thread = new BusyToneDetectThread();
                    thread->start();
                }
                return thread;
            }
            typedef int (__stdcall *SabtdInit)(void **ppSabtdStatus, int sampleRate, char *license);
            typedef int (__stdcall *SabtdUnInit)(void *pSabtdStatus);
            typedef int (__stdcall *SabtdSetParam)(void *pSabtdStatus, ParamType paramType, float paramValue);
            typedef int (__stdcall *SabtdGetParam)(void *pSabtdStatus, ParamType paramType, float *pParamValue);
            typedef int (__stdcall *SabtdReset)(void *pSabtdStatus);
            typedef int (__stdcall *SabtdProcess)(void *pSabtdStatus, int *result, short *data, int length);
        public:
            BusyToneDetectThread()
            : Thread()
            , quit_(false)
            , active_(false)
            , isVoiceStop_(true) {
                InitializeCriticalSection(&mutex_);
            }
            ~BusyToneDetectThread() {
                DeleteCriticalSection(&mutex_);
            }
            void willStop() {
                quit_ = true;
            }
            void AppendVoiceData(Util::Audio::OggCodec::PcmData* const data) {
                EnterCriticalSection(&mutex_);
                detectQueue_.push(data);
                LeaveCriticalSection(&mutex_);
                isVoiceStop_ = false;
                active_ = true;
            }
            void VoiceStop() {
                isVoiceStop_ = true;
            }
            int const run();
            void prepare_();
            void detect_();
            void finally_();
        private:
            HINSTANCE detectLib_;
            void* detectStruct_;
            SabtdInit btd_init;
            SabtdUnInit btd_uninit;
            SabtdSetParam btd_set_param;
            SabtdGetParam btd_get_param;
            SabtdReset btd_reset;
            SabtdProcess btd_process;
            std::queue<Util::Audio::OggCodec::PcmData*> detectQueue_;
            CRITICAL_SECTION mutex_;

            int voiceLength_;
            static size_t const maxVoiceDataLength = 32000;
            short voiceData_[maxVoiceDataLength];
            volatile bool isVoiceStop_;

            volatile bool quit_;
            volatile bool active_;
        };
    }
}

#endif //__TELEPHONE_BUSYTONEDETECTTHREAD_H__
