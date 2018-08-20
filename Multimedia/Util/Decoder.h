#ifndef __UTIL_DECODER_H__
#define __UTIL_DECODER_H__

#pragma once

#include <utility> //for std::pair
#include "MetaData.h" //for MetaData

namespace Util {
    class Decoder {
    public:
        virtual ~Decoder();
        virtual std::pair<unsigned char const* const, size_t const> const Decode(unsigned char const* const data, size_t const length) = 0;
        virtual MetaData const* const GetMetaData() const = 0;
    };
}

#endif //__UTIL_DECODER_H__
