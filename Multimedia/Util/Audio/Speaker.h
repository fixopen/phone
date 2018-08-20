#ifndef __UTIL_AUDIO_SPEAKER_H__
#define __UTIL_AUDIO_SPEAKER_H__

#pragma once

#include "AudioRender.h"

namespace Util {
    namespace Audio {
        class Speaker : public AudioRender {
        public:
            Speaker();
            ~Speaker();
        };
    }
}

#endif //__UTIL_AUDIO_SPEAKER_H__
