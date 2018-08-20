#ifndef __UTIL_SCREEN_H__
#define __UTIL_SCREEN_H__

#include <windef.h>

namespace Util {
    class Screen {
    public:
        static int const width = 800;
        static int const height = 480;
        static SIZE const size();
        static int const fontPoint = 180;
    };
}

#endif //__UTIL_SCREEN_H__
