#ifndef __UTIL_IMAGE_IMAGEDECODER_H__
#define __UTIL_IMAGE_IMAGEDECODER_H__

#pragma once

#include "../Decoder.h"

namespace Util {
    namespace Image {
        class ImageDecoder : public Decoder {
        public:
            virtual ~ImageDecoder();
            virtual std::pair<unsigned char const* const, size_t const> const Decode(unsigned char const* const data, size_t const length) = 0;
            virtual MetaData const* const GetMetaData() const = 0;
        };
    }
}

#endif //__UTIL_IMAGE_IMAGEDECODER_H__
