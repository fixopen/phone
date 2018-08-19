#ifndef __UTIL_VIDEO_SCREEN_H__
#define __UTIL_VIDEO_SCREEN_H__

#include "Graphics.h"

namespace Util {
    namespace Video {
        class Screen {
        public:
            static int const width = 800;
            static int const height = 480;
            static Size const size();
            static int const fontPoint = 180;
        };
    }
}

#endif //__UTIL_VIDEO_SCREEN_H__
