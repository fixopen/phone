#include "Screen.h"

namespace Util {
    SIZE const Screen::size() {
        static bool isFirst = true;
        static SIZE result;
        if (isFirst) {
            result.cx = width;
            result.cy = height;
            isFirst = false;
        }
        return result;
    }
}
