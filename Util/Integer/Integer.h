#ifndef __UTIL_INTEGER_INTEGER_H__
#define __UTIL_INTEGER_INTEGER_H__

#include "UnsignedInteger.h"

namespace Util {
    namespace Integer {
        class Integer : public UnsignedInteger {
        public:
            Integer(bool const isNegative = false);
            Integer(UnsignedInteger const& v, bool const isNegative = false);
        private:
            bool isNegative_;
        };
    }
}

#endif //__UTIL_INTEGER_INTEGER_H__
