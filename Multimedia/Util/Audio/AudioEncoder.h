#ifndef __UTIL_AUDIO_AUDIOENCODER_H__
#define __UTIL_AUDIO_AUDIOENCODER_H__

#pragma once

#include "../Encoder.h"

namespace Util {
    namespace Audio {
        class AudioEncoder : public Encoder {
        public:
            virtual ~AudioEncoder();
            virtual std::pair<unsigned char const* const, size_t const> const Encode(unsigned char const* const data, size_t const length) = 0;
            virtual void SetMetaData(MetaData const* const metaData) = 0;
        };
    }
}

#endif //__UTIL_AUDIO_AUDIOENCODER_H__
