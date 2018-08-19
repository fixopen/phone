#include "Screen.h"

namespace Util {
    namespace Video {
        Size const Screen::size() {
            static Size result = Size(width, height);
            return result;
        }
    }
}
