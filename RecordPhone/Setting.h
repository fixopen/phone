#pragma once

#include <string>
#include <map>
#include "Util/DateTime.h"

class TelephoneSetting {
public:
    enum FireWallType {
        fwtNull,
        fwtBlacklist,
        fwtNotContacts,
        fwtNotVIP,
        fwtAll,
    };
    TelephoneSetting();
    ~TelephoneSetting();
    std::wstring thisTelephoneNumber;
    std::wstring localAreaCode;
    std::wstring outlinePrefix;
    std::wstring ipPrefix;
    std::wstring defaultRing;
    std::wstring defaultPrompt;
    int delayCount;
    FireWallType firewallType;
    Util::TimeSpan firewallDuration;
    bool isAutoRecord;
    std::map<int, std::wstring> speedDialNumbers;
};

class SystemSetting {
public:
    SystemSetting();
    ~SystemSetting();
    std::wstring recordDataFolder;
    int ringtoneVolume;
    int masterVolume;
    void SetSystemTime(Util::Timestamp const& now);
    std::wstring productId;
    std::wstring hardwareVersion;
    std::wstring softwareVersion;
    int const GetSDCardSpace() const;
    int const GetFlashDrvSpace() const;
    int const GetContactCount() const;
    int const GetVoiceMessageCount() const;
    int const GetCallHistoryCount() const;
};

class Setting {
public:
    Setting()
    : telephone(0)
    , system(0) { //load
        telephone = new TelephoneSetting();
        system = new SystemSetting();
    }
    ~Setting() { //save
        delete telephone;
        delete system;
    }
    TelephoneSetting* telephone;
    SystemSetting* system;
};

extern Setting* setting;