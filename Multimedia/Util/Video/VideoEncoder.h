#ifndef __UTIL_VIDEO_VIDEOENCODER_H__
#define __UTIL_VIDEO_VIDEOENCODER_H__

#pragma once

#include "../Encoder.h"

namespace Util {
    namespace Video {
        class VideoEncoder : public Encoder {
        public:
            virtual ~VideoEncoder();
            virtual std::pair<unsigned char const* const, size_t const> const Encode(unsigned char const* const data, size_t const length) = 0;
            virtual void SetMetaData(MetaData const* const metaData) = 0;
        };
    }
}

#endif //__UTIL_VIDEO_VIDEOENCODER_H__
