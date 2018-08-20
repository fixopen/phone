#ifndef __UTIL_AUDIO_WAVEOUT_H__
#define __UTIL_AUDIO_WAVEOUT_H__

#include <windef.h>
#include <mmsystem.h>
#include <string>
#include "WaveUtil.h"
#include "SpeexDecoder.h"
#include "../SmartPtr.h"
//#include "../../Resource.h"

//OggFileReader->SpeexDecode->Device
namespace Util {
    namespace Audio {
        class WaveOut {
        public:
            struct VolumeValue {
                unsigned short leftVolume; //0x0000-0xFFFF
                unsigned short rightVolumn; //0x0000-0xFFFF
            };
            static WaveOut* const Instance();
            WaveOut();
            void SetDecode(SpeexDecoder* const speexDecoder) {
                speexDecoder_ = speexDecoder;
            }
            void (*EatData)(void const * const data, size_t const length);
            void InsertData(void const * const data, size_t const length);
            bool const Start(std::wstring const & filename = L"");
            bool const Pause();
            bool const Resume();
            bool const Stop();
            void SetStopNotify(void (*notify)()) {
                notify_ = notify;
            }
            bool const IsPlaying() const {
                return isPlaying_;
            }
            void SetVolume(VolumeValue const volume) {
                unsigned int v = (volume.leftVolume << 16) | volume.rightVolumn;
                waveOutSetVolume(waveOut_, v);
            }
        protected:
            void (*notify_)();
        private:
            HWAVEOUT waveOut_;
            unsigned char* buffer_;
            PWAVEHDR waveHeader_;
            WAVEFORMATEX waveFormat_;
            bool isPlaying_;
            bool isLeaveWord_;
            volatile bool isEnding_;
            int dataCount_;
            DWORD dataLength_;
            //OggCodec* oggCodec_;
            SpeexDecoder* speexDecoder_;
            CRITICAL_SECTION criticalSection_;
        };
    }
}

#endif //__UTIL_AUDIO_WAVEOUT_H__
