#include "StoragePath.h"

std::wstring const StoragePath::GetFlashDrvPath() {
    std::wstring result = L"/FlashDrv";
    return result;
}

std::wstring const StoragePath::GetSdCardPath() {
    std::wstring result = L"/SDCard";
    return result;
}

std::wstring const StoragePath::GetUsbDiskPath() {
    std::wstring result = L"/UsbDisk";
    return result;
}

std::wstring const StoragePath::GetCallRecordPath() {
    std::wstring result = L"my_record"; //L"Record";
    return result;
}

std::wstring const StoragePath::GetLeaveWordPath() {
    std::wstring result = L"my_record"; //L"Record";
    return result;
}

std::wstring const StoragePath::GetLocalRecordPath() {
    std::wstring result = L"Local";
    return result;
}

std::wstring const StoragePath::GetRingtonePath() {
    std::wstring result = L"MY_RING"; //L"Ringtone";
    return result;
}

std::wstring const StoragePath::GetPromptPath() {
    std::wstring result = L"MY_RING"; //L"Prompt";
    return result;
}

bool const StoragePath::HasSdCard() {
    bool result = false;
    return result;
}

bool const StoragePath::HasUsbDisk() {
    bool result = false;
    return result;
}

bool const StoragePath::HasFolder(std::wstring const& folder) {
    bool result = false;
    return result;
}