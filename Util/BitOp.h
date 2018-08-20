#ifndef __UTIL_BITOP_H__
#define __UTIL_BITOP_H__

namespace Util {
    class Bitop {
    public:
		static bool getBit8(unsigned char value, size_t bitPos) {
			//assert(bitPos >= 0 && bitPos <= 7);
			return !!(value & (unsigned char)(1 << bitPos));
		}

		static void setBit8(unsigned char& value, size_t bitPos) {
			//assert(bitPos >= 0 && bitPos <= 7);
			value |= (unsigned char)(1 << bitPos);
		}

		static void clearBit8(unsigned char& value, size_t bitPos) {
			//assert(bitPos >= 0 && bitPos <= 7);
			value &= (unsigned char)~(1 << bitPos);
		}

		static bool getBit16(unsigned short value, size_t bitPos) {
			//assert(bitPos >= 0 && bitPos <= 15);
			return !!(value & (unsigned short)(1 << bitPos));
		}

		static void setBit16(unsigned short& value, size_t bitPos) {
			//assert(bitPos >= 0 && bitPos <= 7);
			value |= (unsigned short)(1 << bitPos);
		}

		static void clearBit16(unsigned short& value, size_t bitPos) {
			//assert(bitPos >= 0 && bitPos <= 15);
			value &= (unsigned short)~(1 << bitPos);
		}

		static bool getBit32(unsigned int value, size_t bitPos) {
			//assert(bitPos >= 0 && bitPos <= 31);
			return !!(value & (unsigned int)(1 << bitPos));
		}

		static void setBit32(unsigned int& value, size_t bitPos) {
			//assert(bitPos >= 0 && bitPos <= 31);
			value |= (unsigned int)(1 << bitPos);
		}

		static void clearBit32(unsigned int& value, size_t bitPos) {
			//assert(bitPos >= 0 && bitPos <= 31);
			value &= (unsigned int)~(1 << bitPos);
		}
    };
}

#endif //__UTIL_BITOP_H__
