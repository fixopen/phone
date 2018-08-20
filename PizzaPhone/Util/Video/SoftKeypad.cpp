#include "SoftKeypad.h"
#include "../Log.h"

namespace Util {
    namespace Video {
        unsigned short const SoftKeypad::getCode_(Point const & point) {
            unsigned short result = -1;
            for (size_t i = 0; i < layout_.size(); ++i) {
                if (CoordsOp::PointInRect(point, layout_[i].bounds)) {
                    result = layout_[i].code;
                    break;
                }
            }
            return result;
        }
    }
}
