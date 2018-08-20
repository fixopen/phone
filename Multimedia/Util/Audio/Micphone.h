#ifndef __UTIL_AUDIO_MICPHONE_H__
#define __UTIL_AUDIO_MICPHONE_H__

#pragma once

#include "AudioCapture.h"

namespace Util {
    namespace Audio {
        class Micphone : public AudioCapture {
        public:
            Micphone();
            ~Micphone();
        };
    }
}

#endif //__UTIL_AUDIO_MICPHONE_H__
