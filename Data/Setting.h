#ifndef __DATA_SETTING_H__
#define __DATA_SETTING_H__

#include "Data.h"
#include "TelephoneNumber.h"

#include "../Util/SmartPtr.h"

#pragma warning(disable: 4786)
#include <string>
#include <map>

namespace Data
{
	enum SettingType
	{
		stDefault,
		stNormal,
		stSpeed,
		stSpace,
		stCurrent,
	};

	enum LinkMode
	{
		lmNull,
		lmDial,
		lmDirect,
		lmError,
	};

	enum IPMode
	{
		imNull,
		imAuto,
		imManual,
		imError,
	};

    typedef int IPAddr;

    enum ScreenSaverContent
    {
        sscVideo,
        sscImage,
        sscClock,
    };

	enum FirewallType
	{
		fwtNull,
		fwtBlockBlacklist,
		fwtBlockNotInContactAndBlacklist,
		fwtOnlyVip,
		fwtBlockAll,
	};

	class Setting : public DataAccess<Setting>
	{
    public:
        SettingType const type() const
        {
            return type_;
        }
        TelephoneNumber const thisTelephoneNumber() const
        {
            return thisTelephoneNumber_;
        }
        void thisTelephoneNumber(TelephoneNumber const& thisTelephoneNumber)
        {
            thisTelephoneNumber_ = thisTelephoneNumber;
        }
        std::string const outlinePrefix() const
        {
            return outlinePrefix_;
        }
        void outlinePrefix(std::string const& outlinePrefix)
        {
            outlinePrefix_ = outlinePrefix;
        }
        std::string const ipPrefix() const
        {
            return ipPrefix_;
        }
        void ipPrefix(std::string const& ipPrefix)
        {
            ipPrefix_ = ipPrefix;
        }
        bool const isAppendIpPrefix() const
        {
            return isAppendIpPrefix_;
        }
        void isAppendIpPrefix(bool const isAppendIpPrefix)
        {
            isAppendIpPrefix_ = isAppendIpPrefix;
        }
        bool const isUseSpecRing() const
        {
            return isUseSpecRing_;
        }
        void isUseSpecRing(bool const isUseSpecRing)
        {
            isUseSpecRing_ = isUseSpecRing;
        }
        std::string const defaultRingFilename() const
        {
            return defaultRingFilename_;
        }
        void defaultRingFilename(std::string const defaultRingFilename)
        {
            defaultRingFilename_ = defaultRingFilename;
        }
        bool const isAutoReply() const
        {
            return isAutoReply_;
        }
        void isAutoReply(bool const isAutoReply)
        {
            isAutoReply_ = isAutoReply;
        }
        int const autoReplyRingCount() const
        {
            return autoReplyRingCount_;
        }
        void autoReplyRingCount(int const autoReplyRingCount)
        {
            autoReplyRingCount_ = autoReplyRingCount;
        }
        std::map<char, std::string> const speedDials() const
        {
            return speedDials_;
        }
        void speedDials(std::map<char, std::string> speedDials)
        {
            speedDials_ = speedDials;
        }
        LinkMode const linkMode() const
        {
            return linkMode_;
        }
        void linkMode(LinkMode const linkMode)
        {
            linkMode_ = linkMode;
        }
        std::string const dialUsername() const
        {
            return dialUsername_;
        }
        void dialUsername(std::string const dialUsername)
        {
            dialUsername_ = dialUsername;
        }
        std::string const dialPassword() const
        {
            return dialPassword_;
        }
        void dialPassword(std::string const dialPassword)
        {
            dialPassword_ = dialPassword;
        }
        IPMode const ipMode() const
        {
            return ipMode_;
        }
        void ipMode(IPMode const ipMode)
        {
            ipMode_ = ipMode;
        }
        IPAddr const thisIPAddr() const
        {
            return thisIPAddr_;
        }
        void thisIPAddr(IPAddr const ipAddr)
        {
            thisIPAddr_ = ipAddr;
        }
        IPAddr const subnetMask() const
        {
            return subnetMask_;
        }
        void subnetMask(IPAddr const subnetMask)
        {
            subnetMask_ = subnetMask;
        }
        IPAddr const gateway() const
        {
            return gateway_;
        }
        void gateway(IPAddr const gateway)
        {
            gateway_ = gateway;
        }
        IPAddr const dns() const
        {
            return dns_;
        }
        void dns(IPAddr const dns)
        {
            dns_ = dns;
        }
        bool const isIndicationUser() const
        {
            return isIndicationUser_;
        }
        void isIndicationUser(bool const isIndicationUser)
        {
            isIndicationUser_ = isIndicationUser;
        }
        bool const isAlwaysSave() const
        {
            return isAlwaysSave_;
        }
        void isAlwaysSave(bool const isAlwaysSave)
        {
            isAlwaysSave_ = isAlwaysSave;
        }
        std::string const copyright() const
        {
            return copyright_;
        }
        void currentTime(Util::Timestamp const currentTime)
        {
            //Util::SetCurrentTime(currentTime);
        }
        std::string const hardwareVersion() const
        {
            return hardwareVersion_;
        }
        void hardwareVersion(std::string const hardwareVersion)
        {
            hardwareVersion_ = hardwareVersion;
        }
        std::string const softwareVersion() const
        {
            return softwareVersion_;
        }
        void softwareVersion(std::string const softwareVersion)
        {
            softwareVersion_ = softwareVersion;
        }
        int const memorySize() const
        {
            return memorySize_;
        }
        void memorySize(int const memorySize)
        {
            memorySize_ = memorySize;
        }
        int const memoryUsedSize() const
        {
            return memoryUsedSize_;
        }
        void memoryUsedSize(int const memoryUsedSize)
        {
            memoryUsedSize_ = memoryUsedSize;
        }
        int const storageSize() const
        {
            return storageSize_;
        }
        void storageSize(int const storageSize)
        {
            storageSize_ = storageSize;
        }
        int const storageUsedSize() const
        {
            return storageUsedSize_;
        }
        void storageUsedSize(int const storageUsedSize)
        {
            storageUsedSize_ = storageUsedSize;
        }
        int const contactUseSize() const
        {
            return contactUseSize_;
        }
        void contactUseSize(int const contactUseSize)
        {
            contactUseSize_ = contactUseSize;
        }
        int const contactInfoUseSize() const
        {
            return contactInfoUseSize_;
        }
        void contactInfoUseSize(int const contactInfoUseSize)
        {
            contactInfoUseSize_ = contactInfoUseSize;
        }
        int const soundUseSize() const
        {
            return soundUseSize_;
        }
        void soundUseSize(int const soundUseSize)
        {
            soundUseSize_ = soundUseSize;
        }
        std::string const flashDrvRoot() const
        {
            return flashDrvRoot_;
        }
        void flashDrvRoot(std::string const flashDrvRoot)
        {
            flashDrvRoot_ = flashDrvRoot;
        }
        std::string const storageCardRoot() const
        {
            return storageCardRoot_;
        }
        void storageCardRoot(std::string const storageCardRoot)
        {
            storageCardRoot_ = storageCardRoot;
        }
        std::string const usbDiskRoot() const
        {
            return usbDiskRoot_;
        }
        void usbDiskRoot(std::string const usbDiskRoot)
        {
            usbDiskRoot_ = usbDiskRoot;
        }
        std::string const dataRootPath() const
        {
            return dataRootPath_;
        }
        void dataRootPath(std::string const dataRootPath)
        {
            dataRootPath_ = dataRootPath;
        }
        std::string const picturePath() const
        {
            return picturePath_;
        }
        void picturePath(std::string const picturePath)
        {
            picturePath_ = picturePath;
        }
        std::string const soundPath() const
        {
            return soundPath_;
        }
        void soundPath(std::string const soundPath)
        {
            soundPath_ = soundPath;
        }
        std::string const videoPath() const
        {
            return videoPath_;
        }
        void videoPath(std::string const videoPath)
        {
            videoPath_ = videoPath;
        }
        std::string const removeableSemiFixPath() const
        {
            return removeableSemiFixPath_;
        }
        void removeableSemiFixPath(std::string const removeableSemiFixPath)
        {
            removeableSemiFixPath_ = removeableSemiFixPath;
        }
        std::string const removeableDynamicPath() const
        {
            return removeableDynamicPath_;
        }
        void removeableDynamicPath(std::string const removeableDynamicPath)
        {
            removeableDynamicPath_ = removeableDynamicPath;
        }
        int const thisUserId() const
        {
            return thisUserId_;
        }
        void thisUserId(int const userId)
        {
            thisUserId_ = userId;
        }
		FirewallType const firewallType() const
        {
            return firewallType_;
        }
        void firewallType(FirewallType const firewallType)
        {
            firewallType_ = firewallType;
        }
		Util::TimeSpan const blockAllTimeDuration() const
        {
            return blockAllTimeDuration_;
        }
        void blockAllTimeDuration(Util::TimeSpan const& blockAllTimeDuration)
        {
            blockAllTimeDuration_ = blockAllTimeDuration;
        }
		bool const isUseScreenSaver() const
        {
            return isUseScreenSaver_;
        }
        void isUseScreenSaver(bool const isUseScreenSaver)
        {
            isUseScreenSaver_ = isUseScreenSaver;
        }
        Util::TimeSpan const screenSaverDuration() const
        {
            return screenSaverDuration_;
        }
        void screenSaverDuration(Util::TimeSpan const& screenSaverDuration)
        {
            screenSaverDuration_ = screenSaverDuration;
        }
        ScreenSaverContent const screenSaverContent() const
        {
            return screenSaverContent_;
        }
        void screenSaverContent(ScreenSaverContent const screenSaverContent)
        {
            screenSaverContent_ = screenSaverContent;
        }
		bool const isUseScreenSaverPassword() const
        {
            return isUseScreenSaverPassword_;
        }
        void isUseScreenSaverPassword(bool const isUseScreenSaverPassword)
        {
            isUseScreenSaverPassword_ = isUseScreenSaverPassword;
        }
		std::string const screenSaverPassword() const
        {
            return screenSaverPassword_;
        }
        void screenSaverPassword(std::string const screenSaverPassword)
        {
            screenSaverPassword_ = screenSaverPassword;
        }
	private:
		//int id_;
		SettingType type_;

		//telephone setting
        TelephoneNumber thisTelephoneNumber_;
        std::string outlinePrefix_;
		bool isAutoAppendOutlinePrefix_;
		std::string ipPrefix_;
		bool isAppendIpPrefix_;
		bool isUseSpecRing_;
		std::string defaultRingFilename_;
		bool isAutoReply_;
		int autoReplyRingCount_;
		std::map<char, std::string> speedDials_; //12

		//network setting
        LinkMode linkMode_;
		std::string dialUsername_;
		std::string dialPassword_;
        IPMode ipMode_;
        IPAddr thisIPAddr_;
        IPAddr subnetMask_;
        IPAddr gateway_;
        IPAddr dns_;

		//UI setting
        bool isIndicationUser_; //default is false;
        bool isAlwaysSave_; //default is false

        //system setting
		std::string copyright_; //readonly
        Util::Timestamp currentTime_; //writeonly
		std::string hardwareVersion_;
		std::string softwareVersion_;
		int memorySize_; //readonly
		int memoryUsedSize_; //readonly
		int storageSize_; //readonly
		int storageUsedSize_; //readonly
		int contactUseSize_; //readonly
		int contactInfoUseSize_; //readonly
		int soundUseSize_; //readonly
        int maxContactUseSize_;
        int maxContactInfoUseSize_;
        int maxSoundUseSize_;
        int soundVolume_;

		//Storage setting
		std::string flashDrvRoot_;
		std::string storageCardRoot_;
		std::string usbDiskRoot_;
        std::string dataRootPath_;
        std::string picturePath_;
        std::string notePath_;
        std::string musicPath_;
        std::string soundPath_;
        std::string videoPath_;
        std::string removeableSemiFixPath_;
        std::string removeableDynamicPath_;

		//security setting
        int thisUserId_;
		FirewallType firewallType_;
		Util::TimeSpan blockAllTimeDuration_;
        std::string soundProtectPassword_;

		//screenSaver setting
		bool isUseScreenSaver_;
        Util::TimeSpan screenSaverDuration_;
        ScreenSaverContent screenSaverContent_;
		bool isUseScreenSaverPassword_;
		std::string screenSaverPassword_;
		/*
		//meta info
		Util::Timestamp creationTime_;
        Util::Timestamp lastReadTime_;
        Util::Timestamp lastWriteTime_;
        std::vector<Util::Timestamp> accessTimes_;
        Util::Timestamp destroyTime_;
		*/
	public:
		Setting(); //create
		~Setting(); //destroy
		static std::vector<boost::shared_ptr<Setting> > GetFromDatabase(std::string filter); //get from database
		static boost::shared_ptr<Setting> GetCurrentConfig();
		static boost::shared_ptr<Setting> GetDefaultConfig();
		static boost::shared_ptr<Setting> GetNormalConfig();
		static boost::shared_ptr<Setting> GetSpeedConfig();
		static boost::shared_ptr<Setting> GetSpaceConfig();
		void Update() const; //sync to database
		void Insert(); //insert new instance to database
		static void Remove(std::string filter); //delete form database
	private:
		static void modifyFieldByDB_(int argc, char** argv, char** columnName, boost::shared_ptr<Setting> item);
	};
}

#endif //__DATA_SETTING_H__
