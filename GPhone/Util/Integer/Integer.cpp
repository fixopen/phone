#include "Integer.h"

namespace Util {
    namespace Integer {
        Integer::Integer(bool const isNegative)
        : UnsignedInteger()
        , isNegative_(isNegative) {
        }

        Integer::Integer(UnsignedInteger const & v, bool const isNegative)
        : UnsignedInteger(v)
        , isNegative_(isNegative) {
        }
    }
}
