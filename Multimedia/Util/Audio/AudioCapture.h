#ifndef __UTIL_AUDIO_AUIDIOCAPTURE_H__
#define __UTIL_AUDIO_AUIDIOCAPTURE_H__

#pragma once

#include "../Capture.h"

namespace Util {
    namespace Audio {
        class AudioCapture : public Capture {
        public:
            virtual ~AudioCapture();
        };
    }
}

#endif //__UTIL_AUDIO_AUIDIOCAPTURE_H__
