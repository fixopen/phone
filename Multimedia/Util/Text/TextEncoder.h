#ifndef __UTIL_TEXT_TEXTENCODER_H__
#define __UTIL_TEXT_TEXTENCODER_H__

#pragma once

#include "../Encoder.h"

namespace Util {
    namespace Text {
        class TextEncoder : public Encoder {
        public:
            virtual ~TextEncoder();
            virtual std::pair<unsigned char const* const, size_t const> const Encode(unsigned char const* const data, size_t const length) = 0;
            virtual void SetMetaData(MetaData const* const metaData) = 0;
        };
    }
}

#endif //__UTIL_TEXT_TEXTENCODER_H__
