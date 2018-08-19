#ifndef __UTIL_INTEGER_UNSIGNEDINTEGER_H__
#define __UTIL_INTEGER_UNSIGNEDINTEGER_H__

#include "ByteArray.h"
#include <vector>

namespace Util {
    namespace Integer {
        class Integer;
        class UnsignedInteger : public ByteArray {
        public:
            UnsignedInteger();
            UnsignedInteger(std::string const & number);
            UnsignedInteger const & operator=(std::string const & number);
            void swap(UnsignedInteger& rhs) throw() {
                value_.swap(rhs.value_);
            }
            std::vector<unsigned short> const & value() const {
                return value_;
            }
            void appendBigBit(unsigned short const v) {
                value_.push_back(v);
            }
        private:
            std::vector<unsigned short> value_;
        };
        bool const operator<(UnsignedInteger const & lhs, UnsignedInteger const & rhs);
        bool const operator==(UnsignedInteger const & lhs, UnsignedInteger const & rhs);
        Integer const operator-(UnsignedInteger const & v);
        UnsignedInteger const operator+(UnsignedInteger const & lhs, UnsignedInteger const & rhs);
        Integer const operator-(UnsignedInteger const & lhs, UnsignedInteger const & rhs);
        UnsignedInteger const operator *(UnsignedInteger const & lhs, UnsignedInteger const & rhs);
        std::pair<UnsignedInteger, UnsignedInteger> const operator/(UnsignedInteger const & lhs, UnsignedInteger const & rhs);
        std::pair<UnsignedInteger, UnsignedInteger> const operator%(UnsignedInteger const & lhs, UnsignedInteger const & rhs);
        UnsignedInteger const pow(UnsignedInteger const & lhs, UnsignedInteger const & rhs);
        UnsignedInteger const extract(UnsignedInteger const & lhs, UnsignedInteger const & rhs);
        inline UnsignedInteger const & operator+=(UnsignedInteger& lhs, UnsignedInteger const & rhs) {
            return lhs = lhs + rhs;
        }
    }
}

#endif //__UTIL_INTEGER_UNSIGNEDINTEGER_H__
