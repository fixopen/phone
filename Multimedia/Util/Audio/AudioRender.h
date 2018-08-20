#ifndef __UTIL_AUDIO_AUDIORENDER_H__
#define __UTIL_AUDIO_AUDIORENDER_H__

#pragma once

#include "../Render.h"

namespace Util {
    namespace Audio {
        class AudioRender : public Render {
        public:
            virtual ~AudioRender();
        };
    }
}

#endif //__UTIL_AUDIO_AUDIORENDER_H__
