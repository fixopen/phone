#ifndef __UTIL_VIDEO_VIDEODECODER_H__
#define __UTIL_VIDEO_VIDEODECODER_H__

#pragma once

#include "../Decoder.h"

namespace Util {
    namespace Video {
        class VideoDecoder : public Decoder {
        public:
            virtual ~VideoDecoder();
            virtual std::pair<unsigned char const* const, size_t const> const Decode(unsigned char const* const data, size_t const length) = 0;
            virtual MetaData const* const GetMetaData() const = 0;
        };
    }
}

#endif //__UTIL_VIDEO_VIDEODECODER_H__
