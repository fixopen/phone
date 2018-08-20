#ifndef __UTIL_TEXT_TEXTDECODER_H__
#define __UTIL_TEXT_TEXTDECODER_H__

#pragma once

#include "../Decoder.h"

namespace Util {
    namespace Text {
        class TextDecoder : public Decoder {
        public:
            virtual ~TextDecoder();
            virtual std::pair<unsigned char const* const, size_t const> const Decode(unsigned char const* const data, size_t const length) = 0;
            virtual MetaData const* const GetMetaData() const = 0;
        };
    }
}

#endif //__UTIL_TEXT_TEXTDECODER_H__
