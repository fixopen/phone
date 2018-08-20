#include "Screen.h"

namespace Util {
    namespace Graphics {
        Size const Screen::size() {
            static Size result = Size(width, height);
            return result;
        }
    }
}
