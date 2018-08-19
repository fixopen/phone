#ifndef __UTIL_TEXT_GSMCODEC_H__
#define __UTIL_TEXT_GSMCODEC_H__

namespace Util {
    namespace Text {
        struct GsmCodec {
            GsmCodec() {
                memset(convertBuffer, 0, 7);
            }
            unsigned char sevenBit0() {
                return convertBuffer[0] >> 1;
            }
            unsigned char sevenBit1() {
                return ((convertBuffer[0] & 0x1) << 6) | (convertBuffer[1] >> 2);
            }
            unsigned char sevenBit2() {
                return ((convertBuffer[1] & 0x3) << 5) | (convertBuffer[2] >> 3);
            }
            unsigned char sevenBit3() {
                return ((convertBuffer[2] & 0x7) << 4) | (convertBuffer[3] >> 4);
            }
            unsigned char sevenBit4() {
                return ((convertBuffer[3] & 0xF) << 3) | (convertBuffer[4] >> 5);
            }
            unsigned char sevenBit5() {
                return ((convertBuffer[4] & 0x1F) << 2) | (convertBuffer[5] >> 6);
            }
            unsigned char sevenBit6() {
                return ((convertBuffer[5] & 0x3F) << 1) | (convertBuffer[6] >> 7);
            }
            unsigned char sevenBit7() {
                return ((convertBuffer[6] & 0x7F) << 0);
            }
            void sevenBit0(unsigned char ch) {
                ch &= 0x7F;
                convertBuffer[0] = ch << 1;
            }
            void sevenBit1(unsigned char ch) {
                ch &= 0x7F;
                convertBuffer[0] |= (ch & 0x40) >> 6;
                convertBuffer[1] = (ch & 0x3F) << 2;
            }
            void sevenBit2(unsigned char ch) {
                convertBuffer[1] |= (ch & 0x60) >> 5;
                convertBuffer[2] = (ch & 0x1F) << 3;
            }
            void sevenBit3(unsigned char ch) {
                convertBuffer[2] |= (ch & 0x70) >> 4;
                convertBuffer[3] = (ch & 0xF) << 4;
            }
            void sevenBit4(unsigned char ch) {
                convertBuffer[3] |= (ch & 0x78) >> 3;
                convertBuffer[4] = (ch & 0x7) << 5;
            }
            void sevenBit5(unsigned char ch) {
                convertBuffer[4] |= (ch & 0x7C) >> 2;
                convertBuffer[5] = (ch & 0x3) << 6;
            }
            void sevenBit6(unsigned char ch) {
                convertBuffer[5] |= (ch & 0x7E) >> 1;
                convertBuffer[6] = (ch & 0x1) << 7;
            }
            void sevenBit7(unsigned char ch) {
                convertBuffer[6] |= (ch & 0x7F) >> 0;
            }
        private:
            unsigned char convertBuffer[7];
        };
    }
}

#endif //__UTIL_GSMCODEC_H__
