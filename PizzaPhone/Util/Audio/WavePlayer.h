#ifndef __UTIL_AUDIOPLAYER_H__
#define __UTIL_AUDIOPLAYER_H__

#include "../Player.h"

namespace Util {
    namespace Audio {
        class WavePlayer : public Player {
        public:
            virtual void Start();
            virtual void Previous();
            virtual void Next();
            virtual void Stop();
            virtual void SetVolume(size_t const volume);
            virtual void SetLightness(size_t const lightness);
            virtual void SetContract(size_t const contract);
            virtual TimeSpan const GetCurrentMediaDuration() const;
            virtual size_t const GetCurrentMediaPlayPrecent() const;
        };
    }
}

#endif //__UTIL_AUDIOPLAYER_H__
