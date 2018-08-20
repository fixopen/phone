#ifndef __UTIL_AUDIO_AUDIODECODER_H__
#define __UTIL_AUDIO_AUDIODECODER_H__

#pragma once

#include "../Decoder.h"

namespace Util {
    namespace Audio {
        class AudioDecoder : public Decoder {
        public:
            virtual ~AudioDecoder();
            virtual std::pair<unsigned char const* const, size_t const> const Decode(unsigned char const* const data, size_t const length) = 0;
            virtual MetaData const* const GetMetaData() const = 0;
        };
    }
}

#endif //__UTIL_AUDIO_AUDIODECODER_H__
