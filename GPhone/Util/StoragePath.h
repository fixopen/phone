#ifndef __UTIL_STORAGEPATH_H__
#define __UTIL_STORAGEPATH_H__

#include <string>

namespace Util {
    class StoragePath {
    public:
        static std::wstring const GetFlashDrvPath();
        static std::wstring const GetSdCardPath();
        static std::wstring const GetUsbDiskPath();
        static std::wstring const GetCallRecordPath();
        static std::wstring const GetLeaveWordPath();
        static std::wstring const GetLocalRecordPath();
        static std::wstring const GetRingtonePath();
        static std::wstring const GetPromptPath();
        static bool const HasSdCard();
        static bool const HasUsbDisk();
        static bool const HasFolder(std::wstring const& folder);
        static bool const HasFile(std::wstring const& file);
    };
}

#endif //__UTIL_STORAGEPATH_H__
