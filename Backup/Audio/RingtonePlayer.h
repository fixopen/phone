#ifndef __UTIL_AUDIO_RINGTONEPLAYER_H__
#define __UTIL_AUDIO_RINGTONEPLAYER_H__

#include <dshow.h>
#include <string>
#include "../Thread.h"

namespace Util {
    namespace Audio {
        class RingtonePlayer : public Thread {
        public:
            static int const MIN_SOUND = -7000; //-10000
            static int const MAX_SOUND = 0;
            static int const DEFUALT_SOUND_VALUME = -1500;
            static RingtonePlayer* const Instance();
            void SetStopNotify(void (*stopNotify)()) {
                stopNotify_ = stopNotify;
            }
            virtual int const run();
            void willStop() {
                isQuit_ = true;
            }
            void Start(std::wstring const& filename, int loopCount = 0xFF);
            void Stop(bool const release = true);
            void Pause();
            void Resume();
            int const GetVolume() const {
                return volume_;
            }
            void SetVolume(int const value);
            void DecreaseVolume(int const offset);
            void IncreaseVolume(int const offset);
            void SetMinVolume();
            void SetMaxVolume();
            void SetDefaultVolume();
        private:
            RingtonePlayer();
            ~RingtonePlayer();
            //static void threadFunction_();
            //void startThread_();
            //void stopThread_();
            void init_();
            void release_();
            //
            HANDLE playThread_;
            volatile bool isQuit_;
            volatile bool isActive_;
            //ring tone
            IGraphBuilder* graphBuilder_;
            //指向Filter Graph的指针
            IMediaControl* mediaControl_;
            IMediaPosition* mediaPosition_;
            //音频相关接口
            IBasicAudio* basicAudio_;
            IMediaEvent* mediaEvent_;
            //是否循环播放
            UINT8 loopCount_; //0xFF 循环播放
            //
            CRITICAL_SECTION criticalSection_;
            //
            int volume_;
            std::wstring filename_;
            bool playing_;
            void (*stopNotify_)();
        };
    }
}

#endif //__UTIL_AUDIO_RINGTONEPLAYER_H__
