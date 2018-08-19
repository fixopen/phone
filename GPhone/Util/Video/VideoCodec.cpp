#include "VideoCodec.h"

#include <stdlib.h>

namespace Util {
    namespace Video {
        void YUVConvert::YVU420PtoYUV420P(unsigned char const * const yuv420PData, size_t const width, size_t const height, unsigned char* const yvu420PData) {
            memcpy(yvu420PData, yuv420PData, width * height);
            memcpy(yvu420PData + width * height, yuv420PData + width * height + width * height / 4, width * height / 4);
            memcpy(yvu420PData + width * height + width * height / 4, yuv420PData + width * height, width * height / 4);
        }
    }
}
