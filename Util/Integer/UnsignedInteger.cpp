#include "UnsignedInteger.h"
#include "Integer.h"
#include <stdlib.h>

namespace Util {
    namespace Integer {
        UnsignedInteger::UnsignedInteger() {
        }

        namespace {
            unsigned char const getValue(unsigned char const first, unsigned char const second) {
                unsigned char result = 0;
                if (first >= '1' && first <= '9') {
                    result = (first - '0') << 4;
                } else if (first >= 'a' && first <= 'f') {
                    result = (first - 'a' + 10) << 4;
                } else if (first >= 'A' && first <= 'F') {
                    result = (first - 'A' + 10) << 4;
                } else if (first == '0') {
                    //do nothing
                } else {
                    throw std::runtime_error("first's value is bad");
                }
                if (second >= '0' && second <= '9') {
                    result += second - '0';
                } else if (second >= 'a' && second <= 'f') {
                    result += second - 'a' + 10;
                } else if (second >= 'A' && second <= 'F') {
                    result += second - 'A' + 10;
                } else {
                    throw std::runtime_error("second's value is bad");
                }
            }
            unsigned short const getValue(unsigned char const first, unsigned char const second, unsigned char const third, unsigned char const four) {
                unsigned short result = 0;
                unsigned char temp = getValue(first, second);
                result = temp << 16;
                temp = getValue(second, four);
                result += temp;
                return result;
            }
            unsigned int const getValue(unsigned char const number[5]) {
                unsigned int result = 0;
                for (size_t i = 0; i < 5; ++i) {
                    if (number[i] >= '0' && number[i] <= '9') {
                        result += (number[i] - '0') * static_cast<int>(::pow(10, 4 - i));
                    } else {
                        throw std::runtime_error("decimal's value is bad");
                    }
                }
                return result;
            }
        }

        UnsignedInteger::UnsignedInteger(std::string const& number) {
            bool isHex = false;
            std::string value = number;
            if ((number[0] == '0') && ((number[1] == 'x') || (number[1] == 'X'))) {
                value = number.substr(2);
                isHex = true;
            }
            if (isHex) {
                for (size_t i = 0; i < value.length(); i += 4) {
                    std::string segment = value.substr(value.length() - (i + 4), 4);
                    unsigned short v = getValue(segment[0], segment[1], segment[2], segment[3]);
                    value_.push_back(v);
                }
            } else {
                unsigned short const Max = -1;
                int carry = 0;
                for (size_t i = 0; i < value.length(); i += 5) {
                    std::string segment = value.substr(value.length() - (i + 5), 5);
                    unsigned int v = getValue(reinterpret_cast<unsigned char const*>(segment.c_str())) + carry;
                    if (v > Max) {
                        carry = v - Max;
                        v = v - carry;
                    } else {
                        carry = 0;
                    }
                    value_.push_back(v);
                }
            }
        }

        UnsignedInteger const& UnsignedInteger::operator=(std::string const& number) {
            UnsignedInteger t(number);
            //t.swap(*this);
            swap(t);
            return *this;
        }

        bool const operator<(UnsignedInteger const& lhs, UnsignedInteger const& rhs) {
            bool result = false;
            if (lhs.value().size() < rhs.value().size()) {
                result = true;
            } else if (lhs.value().size() == rhs.value().size()) {
                for (size_t i = lhs.value().size() - 1; i >= 0; --i) {
                    if (lhs.value()[i] < rhs.value()[i]) {
                        result = true;
                        break;
                    }
                }
            }
            return result;
        }

        bool const operator==(UnsignedInteger const& lhs, UnsignedInteger const& rhs) {
            bool result = true;
            if (lhs.value().size() != rhs.value().size()) {
                result = false;
            } else if (lhs.value().size() == rhs.value().size()) {
                for (size_t i = 0; i < lhs.value().size(); ++i) {
                    if (lhs.value()[i] != rhs.value()[i]) {
                        result = false;
                        break;
                    }
                }
            }
            return result;
        }

        Integer const operator-(UnsignedInteger const& v) {
            return Integer(v, true);
        }

        UnsignedInteger const operator+(UnsignedInteger const& lhs, UnsignedInteger const& rhs) {
            UnsignedInteger result;
            unsigned short const Max = -1;
            size_t lhsSize = lhs.value().size();
            size_t rhsSize = rhs.value().size();
            bool leftBig = true;
            size_t maxSize = lhsSize;
            if (lhsSize < rhsSize) {
                leftBig = false;
                maxSize = rhsSize;
            }
            int carry = 0;
            for (size_t i = 0; i < maxSize; ++i) {
                unsigned int v = 0;
                if (leftBig) {
                    v = lhs.value()[i];
                    if (i < rhsSize) {
                        v += rhs.value()[i];
                    }
                    v += carry;
                } else {
                    v = rhs.value()[i];
                    if (i < lhsSize) {
                        v += lhs.value()[i];
                    }
                    v += carry;
                }
                if (v > Max) {
                    carry = 1;
                } else {
                    carry = 0;
                }
                result.appendBigBit(v);
            }
            return result;
        }

        namespace {
            UnsignedInteger const sub(UnsignedInteger const& lhs, UnsignedInteger const& rhs) {
                UnsignedInteger result;
                unsigned short const Max = -1;
                size_t lhsSize = lhs.value().size();
                size_t rhsSize = rhs.value().size();
                size_t maxSize = lhsSize;
                int carry = 0;
                for (size_t i = 0; i < maxSize; ++i) {
                    unsigned short v = lhs.value()[i] - carry;
                    if (i < rhsSize) {
                        if (v > rhs.value()[i]) {
                            v -= rhs.value()[i];
                            carry = 0;
                        } else {
                            v += Max - rhs.value()[i];
                            carry = 1;
                        }
                    } else {
                        carry = 0;
                    }
                    result.appendBigBit(v);
                }
                return result;
            }
        }

        Integer const operator-(UnsignedInteger const& lhs, UnsignedInteger const& rhs) {
            Integer result;
            if (lhs < rhs) {
                result = -(sub(rhs, lhs));
            } else {
                result = sub(lhs, rhs);
            }
            return result;
        }

        UnsignedInteger const operator*(UnsignedInteger const& lhs, UnsignedInteger const& rhs) {
            UnsignedInteger result;
            unsigned short const Max = -1;
            size_t lhsSize = lhs.value().size();
            size_t rhsSize = rhs.value().size();
            unsigned int carry = 0;
            for (size_t i = 0; i < rhsSize; ++i) {
                UnsignedInteger mulValue;
                for (size_t adj = 0; adj < i; ++i) {
                    mulValue.appendBigBit(0);
                }
                for (size_t j = 0; j < lhsSize; ++j) {
                    unsigned int v = lhs.value()[i] * rhs.value()[j] + carry;
                    if (v > Max) {
                        carry = (v >> 16) & 0xFF;
                    } else {
                        carry = 0;
                    }
                    mulValue.appendBigBit(v);
                }
                result += mulValue;
            }
            return result;
        }

        std::pair<UnsignedInteger, UnsignedInteger> const operator/(UnsignedInteger const& lhs, UnsignedInteger const& rhs) {
            std::pair<UnsignedInteger, UnsignedInteger> result;
            if (lhs < rhs) {
                //result.first = 0;
                result.second = lhs;
            } else if (lhs == rhs) {
                result.first.appendBigBit(1);
                //result.second = 0;
            } else {
                for (size_t i = 0; i < lhs.value().size(); i += rhs.value().size()) {
                    //UnsignedInteger t = getHighBit(lhs, rhs.value().size());
                    //if (t < rhs) {
                    //    t = getHighBit(lhs, rhs.value().size() + 1);
                    //} else if (t == rhs) {
                    //    1;
                    //} else {
                    //    for (size_t retry = 0; retry < 9; ++retry) {
                    //    }
                    //}

                    //result.first.appendBigBit(v);
                    //result.second;
                }
            }
            return result;
        }

        std::pair<UnsignedInteger, UnsignedInteger> const operator%(UnsignedInteger const& lhs, UnsignedInteger const& rhs) {
            std::pair<UnsignedInteger, UnsignedInteger> result;
            return result;
        }

        UnsignedInteger const pow(UnsignedInteger const& lhs, UnsignedInteger const& rhs) {
            UnsignedInteger result;
            return result;
        }

        UnsignedInteger const extract(UnsignedInteger const& lhs, UnsignedInteger const& rhs) {
            UnsignedInteger result;
            return result;
        }
    }
}
