#ifndef __UTIL_VIDEO_PNG_H__
#define __UTIL_VIDEO_PNG_H__

#include "ImageCodec.h"

#include <string>

namespace Util {
    namespace Video {
        class PNG : public ImageCodec{
        public:
            PNG(std::wstring const & filename);
            ~PNG();
        };
    }
}

#endif //__UTIL_VIDEO_PNG_H__
