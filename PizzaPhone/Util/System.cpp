#include "System.h"
#include <winbase.h> //for DeviceIoControl

#include "ControlCode.h"
#define SCL_IOCODE_BASE 2050
#define SCL_SYSTEM_RESET CTL_CODE(FILE_DEVICE_STREAMS, SCL_IOCODE_BASE + 0x1, METHOD_IN_DIRECT, FILE_ANY_ACCESS) //reset system
#define SCL_ENABLE_WATCHDOG CTL_CODE(FILE_DEVICE_STREAMS, SCL_IOCODE_BASE + 0x2, METHOD_IN_DIRECT, FILE_ANY_ACCESS) //enable watch dog
#define	SCL_ADJUST_BACKLIGHT CTL_CODE(FILE_DEVICE_STREAMS, SCL_IOCODE_BASE + 0x5, METHOD_IN_DIRECT, FILE_ANY_ACCESS) //LCD 灯控制	
#define SCL_GET_PRODUCT_INFO CTL_CODE(FILE_DEVICE_STREAMS, SCL_IOCODE_BASE + 0x7, METHOD_IN_DIRECT, FILE_ANY_ACCESS) //产品信息
#define SCL_ADSL_POWERCTL CTL_CODE(FILE_DEVICE_STREAMS, SCL_IOCODE_BASE + 0xA, METHOD_IN_DIRECT, FILE_ANY_ACCESS) //ADSL 电源控制
#define SCL_ENCRYGPT_VERIFY CTL_CODE(FILE_DEVICE_STREAMS, SCL_IOCODE_BASE + 0xE, METHOD_IN_DIRECT, FILE_ANY_ACCESS)	

namespace Util {
    bool const System::Reset() {
        bool result = false;
        DWORD size = 0;
        UINT value = 0;
        if (::DeviceIoControl(system_, SCL_SYSTEM_RESET, 0, 0, &value, sizeof(UINT), &size, NULL)) {
            result = true;
        }
        return result;
    }

    bool const System::SetWatchDog() {
        bool result = false;
        DWORD size = 0;
        UINT value = 0;
        if (::DeviceIoControl(system_, SCL_ENABLE_WATCHDOG, 0, 0, &value, sizeof(UINT), &size, NULL)) {
            result = true;
        }
        return result;
    }

    struct BACKLIGHTCMD {
        DWORD  command;			//控制命令
        DWORD  level;			//背光控制级别
    };

    bool const System::SetBackLight(int const backLightLevel) {
        bool result = false;
        if (backLightLevel_ == backLightLevel) {
            result = true;
        } else {
            DWORD size = 0;
            UINT value = 0;
            BACKLIGHTCMD in;
            in.command = 0x01;
            in.level = backLightLevel;
            if (::DeviceIoControl(system_, SCL_ADJUST_BACKLIGHT, &in, sizeof in, &value, sizeof(UINT), &size, NULL)) {
                backLightLevel_ = backLightLevel;
                result = true;
            }
        }
        return result;
    }

    int const System::GetBackLight() {
        int result = 0;
        size_t const OLDVALUE_MAX = 3;
        static int oldValueTable[OLDVALUE_MAX] = {
            0, 0, 0
        };
        static int oldValueCount = 0;
        DWORD size = 0;
        BACKLIGHTCMD in;
        in.command = 0x02;
        in.level = 0;
        DWORD value = 0;
        if (::DeviceIoControl(system_, SCL_ADJUST_BACKLIGHT, &in, sizeof(BACKLIGHTCMD), &value, sizeof(DWORD), &size, NULL)) {
            oldValueTable[oldValueCount % OLDVALUE_MAX] = value;
            ++oldValueCount;
            for (size_t i = 0; i < OLDVALUE_MAX; ++i) {
                if (backLightLevel_ == 0) {  //黑
                    result = 7;
                    if (oldValueTable[i] >= 5000) {   //5000
                        result = 0;    //黑
                        break;
                    }
                } else {
                    result = 0;
                    if (oldValueTable[i] < 5000) {   //50000
                        result = 7;    //亮
                        break;
                    }
                }
            }
        }
        return result;
    }

    int const System::GetRawBackLight() {
        unsigned int result = 0;
        DWORD size = 0;
        UINT value = 0;
        BACKLIGHTCMD in;
        in.command = 0x02;
        in.level = 0;
        if (::DeviceIoControl(system_, SCL_ADJUST_BACKLIGHT, &in, sizeof in, &value, sizeof(UINT), &size, NULL)) {
            result = value;
        }
        return result;
    }

    struct MANUINFO {
        BYTE deviceID[32];
        BYTE manuInfo[32];
        BYTE flashID[128];
        BYTE serialNum[32];
        BYTE model[32];
        BYTE date[32];	
        BYTE osVer[32];
        BYTE bootVer[4];
        BYTE appVer[32];
        BYTE hardVer[32];
        BYTE MD5Key[64];
        BYTE bootCkSum[16];
        BYTE res[12];
    };
    typedef MANUINFO* PMANUINFO;

    bool const System::GetProductInfo(void* const buffer, size_t const length) {
        bool result = false;
        DWORD size = 0;
        if (::DeviceIoControl(system_, SCL_GET_PRODUCT_INFO, 0, 0, buffer, length, &size, NULL)) {
            result = true;
        }
        return result;
    }

    bool const System::SetADSLPower(bool const isPowerOn) {
        bool result = false;
        DWORD size = 0;
        DWORD inBuffer = isPowerOn ? 1 : 2;
        DWORD value = 0;
        if (::DeviceIoControl(system_, SCL_ADSL_POWERCTL, &inBuffer, sizeof(DWORD), &value, sizeof(DWORD), &size, NULL)) {
            result = true;
        }
        return result;
    }

    struct ENCRYCMD{
        DWORD  command;			//控制命令
        DWORD  random;			//随机数		
    };

    bool const System::EncriptVerify() {
        bool result = false;
        DWORD size = 0;
        UINT value = 0;
        ENCRYCMD in;
        in.command = 0x01;
        in.random = rand();
        if (::DeviceIoControl(system_, SCL_ENCRYGPT_VERIFY, &in, sizeof in, &value, sizeof(UINT), &size, NULL)) {
            result = true;
        }
        return result;
    }

    unsigned int const System::GetEncriptVerify() {
        unsigned int result = 0;
        DWORD size = 0;
        UINT value = 0;
        ENCRYCMD in;
        in.command = 0x02;
        in.random = 0;
        if (::DeviceIoControl(system_, SCL_ENCRYGPT_VERIFY, &in, sizeof in, &value, sizeof(UINT), &size, NULL)) {
            result = value;
        }
        return result;
    }

    System::System() {
        system_ = ::CreateFile(L"SCL1:", // Pointer to the name of the port
            GENERIC_READ | GENERIC_WRITE, // Access (read-write) mode
            0,            // Share mode
            NULL,         // Pointer to the security attribute
            CREATE_ALWAYS,// Create New File as any case
            0,            // Port attributes
            NULL);
    }

    System::~System() {
        ::CloseHandle(system_);
    }
}
