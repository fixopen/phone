#ifndef __UTIL_NETWORKSTREAM_H__
#define __UTIL_NETWORKSTREAM_H__

#pragma once

#include "Stream.h"

namespace Util {
    class NetworkStream : public Stream {
    public:
        ~NetworkStream();
    };
}

#endif //__UTIL_NETWORKSTREAM_H__
