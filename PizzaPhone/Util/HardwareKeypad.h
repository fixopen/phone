#ifndef __UTIL_HARDWAREKEYPAD_H__
#define __UTIL_HARDWAREKEYPAD_H__

#include "SerialPort.h"
#include "Observer.h"

namespace Util {
    class HardwareKeypad : public Observable {
    public:
        enum KeyCode {
            cPickup = 0x82,
            cHangup = 0x83,
            cUp = 0xA5,
            cDown = 0xA6,
            //cZeroUnknown = 0xB0,
            cOne = 0xB1,
            cTwo = 0xB2,
            cThree = 0xB3,
            cFour = 0xB4,
            cFive = 0xB5,
            cSix = 0xB6,
            cSeven = 0xB7,
            cEight = 0xB8,
            cNine = 0xB9,
            cZero = 0xBA,
            cStar = 0xBB,
            cPound = 0xBC,
            cHandsfreeEnd = 0xC8,
            cHandsfreeBegin = 0xC9,
            cHandsfreeStart = 0xCA,
            cHandsfreeStop = 0xCB,
        };
        static HardwareKeypad* const Instance() {
            static HardwareKeypad* result = new HardwareKeypad(1);
            return result;
        }
    protected:
        HardwareKeypad(unsigned int const portNo);
        ~HardwareKeypad();
        static void disptch_(unsigned char const* const data, unsigned int const length);
    private:
        RS232* hardware_;
    };
}

#endif //__UTIL_HARDWAREKEYPAD_H__
