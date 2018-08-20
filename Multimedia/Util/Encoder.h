#ifndef __UTIL_ENCODER_H__
#define __UTIL_ENCODER_H__

#pragma once

#include <utility> //for std::pair
#include "MetaData.h" //for MetaData

namespace Util {
    class Encoder {
    public:
        virtual ~Encoder();
        virtual std::pair<unsigned char const* const, size_t const> const Encode(unsigned char const* const data, size_t const length) = 0;
        virtual void SetMetaData(MetaData const* const metaData) = 0;
    };
}

#endif //__UTIL_ENCODER_H__
