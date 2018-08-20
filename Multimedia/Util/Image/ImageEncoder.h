#ifndef __UTIL_IMAGE_IMAGEENCODER_H__
#define __UTIL_IMAGE_IMAGEENCODER_H__

#pragma once

#include "../Encoder.h"

namespace Util {
    namespace Image {
        class ImageEncoder : public Encoder {
        public:
            virtual ~ImageEncoder();
            virtual std::pair<unsigned char const* const, size_t const> const Encode(unsigned char const* const data, size_t const length) = 0;
            virtual void SetMetaData(MetaData const* const metaData) = 0;
        };
    }
}

#endif //__UTIL_IMAGE_IMAGEENCODER_H__
