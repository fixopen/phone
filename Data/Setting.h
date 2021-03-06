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
		stCurrent,
		stDefault,
		stNormal,
		stSpeed,
		stSpace,
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
		sscClock,
		sscImage,
        sscVideo,
    };

	enum FirewallType
	{
		fwtBlockBlacklist,
		fwtBlockNotInContactAndBlacklist,
		fwtOnlyVip,
		fwtBlockAll,
		fwtNull,
	};

	class Setting : public DataAccess<Setting>
	{
    public:
        SettingType const type() const
        {
            return type_;
        }
		void type(SettingType const type)
		{
			type_ = type;
		}

        TelephoneNumber const thisTelephoneNumber() const
        {
            return thisTelephoneNumber_;
        }
        void thisTelephoneNumber(TelephoneNumber const& thisTelephoneNumber)
        {
            thisTelephoneNumber_ = thisTelephoneNumber;
        }
		bool const isAutoAppendOutlinePrefix() const
		{
			return isAutoAppendOutlinePrefix_;
		}
		void isAutoAppendOutlinePrefix(bool const isAutoAppendOutlinePrefix)
		{
			isAutoAppendOutlinePrefix_ = isAutoAppendOutlinePrefix;
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

		std::string const LocalNumber() const
        {
            return localNumber_;
        }
        void LocalNumber(std::string const localNumber)
        {
            localNumber_ = localNumber;
        }

        std::string const defaultTipFilename() const
        {
            return defaultTipFilename_;
        }
        void defaultTipFilename(std::string const defaultTipFilename)
        {
            defaultTipFilename_ = defaultTipFilename;
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

        int const maxContactUseSize() const
        {
            return maxContactUseSize_;
        }
        void maxContactUseSize(int const contactUseSize)
        {
            maxContactUseSize_ = contactUseSize;
        }
        int const maxContactInfoUseSize() const
        {
            return maxContactInfoUseSize_;
        }
        void maxContactInfoUseSize(int const contactInfoUseSize)
        {
            maxContactInfoUseSize_ = contactInfoUseSize;
        }
        int const maxSoundUseSize() const
        {
            return maxSoundUseSize_;
        }
        void maxSoundUseSize(int const soundUseSize)
        {
            maxSoundUseSize_ = soundUseSize;
        }
        int const soundVolume() const
        {
            return soundVolume_;
        }
        void soundVolume(int const soundVolume)
        {
            soundVolume_ = soundVolume;
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
        std::string const notePath() const
        {
            return notePath_;
        }
        void notePath(std::string const notePath)
        {
            notePath_ = notePath;
        }
        std::string const musicPath() const
        {
            return musicPath_;
        }
        void musicPath(std::string const musicPath)
        {
            musicPath_ = musicPath;
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
		bool isFirewall(void) const
		{
			return isFirewall_;
		}

		void isFirewall(bool const isFirewall)
		{
			isFirewall_ = isFirewall;
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
        std::string const soundProtectPassword() const
        {
            return soundProtectPassword_;
        }
        void soundProtectPassword(std::string const soundProtectPassword)
        {
            soundProtectPassword_ = soundProtectPassword;
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
		bool const isAdmin() const
		{
			return isAdmin_;
		}
		void isAdmin(bool const isAdmin)
		{
			isAdmin_ = isAdmin;
		}
		bool const isPlayProtect() const
		{
			return isPlayProtect_;
		}
		void isPlayProtect(bool const isPlayProtect)
		{
			isPlayProtect_ = isPlayProtect;
		}
		bool const isMustRecord() const
		{
			return isMustRecord_;
		}
		void isMustRecord(bool const isMustRecord)
		{
			isMustRecord_ = isMustRecord;
		}
		bool const isDeleteProtect() const
		{
			return isDeleteProtect_;
		}
		void isDeleteProtect(bool const isDeleteProtect)
		{
			isDeleteProtect_ = isDeleteProtect;
		}
		std::string const adminPassword() const
        {
            return adminPassword_;
        }
        void adminPassword(std::string const adminPassword)
        {
            adminPassword_ = adminPassword;
        }
		std::string const playRecordPassword() const
        {
            return playRecordPassword_;
        }
        void playRecordPassword(std::string const playRecordPassword)
        {
            playRecordPassword_ = playRecordPassword;
        }
		
		Util::TimeSpan const backlightDuration() const
        {
            return backlightDuration_;
        }
        void backlightDuration(Util::TimeSpan const& backlightDuration)
        {
            backlightDuration_ = backlightDuration;
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
		std::string localNumber_;  //wzx20101213
		bool isAutoReply_;
		int autoReplyRingCount_;
		std::string defaultTipFilename_;
		std::map<char, std::string> speedDials_; //12

		//protect
		bool isAdmin_;
		bool isPlayProtect_;
		bool isMustRecord_;
		bool isDeleteProtect_;
		std::string adminPassword_;
		std::string playRecordPassword_;

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
		bool isFirewall_;
		FirewallType firewallType_;
		Util::TimeSpan blockAllTimeDuration_;
        std::string soundProtectPassword_;

		//screenSaver setting
		bool isUseScreenSaver_;
        Util::TimeSpan screenSaverDuration_;
        ScreenSaverContent screenSaverContent_;
		bool isUseScreenSaverPassword_;
		std::string screenSaverPassword_;
		Util::TimeSpan backlightDuration_;
	public:
		Setting(); //create
		~Setting(); //destroy
		static std::vector<boost::shared_ptr<Setting> > GetFromDatabase(std::string filter, Direction const dir = dNull, int const id = 0, int const pageSize = 0); //get from database
		static boost::shared_ptr<Setting> GetCurrentConfig();
		static boost::shared_ptr<Setting> GetDefaultConfig();
		static boost::shared_ptr<Setting> GetNormalConfig();
		static boost::shared_ptr<Setting> GetSpeedConfig();
		static boost::shared_ptr<Setting> GetSpaceConfig();
		void Update() const; //sync to database
		void UpdateSoundProtectPassword( ) const; //sync sound protect password to database defect 36
		void UpdateScreenSaverPassword(std::string const screenSaverPassword) const; //sync screen saver password to database defect 36
		void UpdateAdminPassword(std::string const adminPassword) const; //sync admin password to database defect 36
		void UpdatePlayRecordPassword(std::string const playRecordPassword ) const; //sync play record password to database defect 36

		void Insert(); //insert new instance to database
		static void Closedb();
		static void Opendb();
		static void Remove(std::string filter); //delete form database
	private:
		static void modifyFieldByDB_(int argc, char** argv, char** columnName, boost::shared_ptr<Setting> item);
	};
}

#endif //__DATA_SETTING_H__
