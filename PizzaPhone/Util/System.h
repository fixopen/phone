#ifndef __UTIL_SYSTEM_H__
#define __UTIL_SYSTEM_H__

#include <windef.h> //for HANDLE

namespace Util {
    class System {
    public:
        static System* const Instance() {
            static System* result = new System();
            return result;
        }
    public:
        bool const Reset();
        bool const SetWatchDog();
        bool const SetBackLight(int const backLightLevel);
        int const GetBackLight();
        int const GetRawBackLight();
        bool const GetProductInfo(void* const buffer, size_t const length);
        bool const SetADSLPower(bool const isPowerOn);
        bool const EncriptVerify();
        unsigned int const GetEncriptVerify();
    private:
        System();
        ~System();
    private:
        HANDLE system_;
        int backLightLevel_;
    };
}

#endif // __UTIL_SYSTEM_H__

