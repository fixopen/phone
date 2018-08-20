#ifndef __UTIL_VIDEO_PNG_H__
#define __UTIL_VIDEO_PNG_H__

#include <string>

namespace Util {
    namespace Video {
        class PNG {
        public:
            PNG(std::wstring const & filename);
            ~PNG();
        };
    }
}

#endif //__UTIL_VIDEO_PNG_H__
