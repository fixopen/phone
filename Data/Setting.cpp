#include "stdafx.h"
#include "Setting.h"

namespace Data
{
	std::string DataAccess<Setting>::tableName_ = "setting";
	sqlite3* DataAccess<Setting>::db_ = Data::GetDB();
    int DataAccess<Setting>::rowCount_ = 0;
	int DataAccess<Setting>::offset_ = 0;
	Indication DataAccess<Setting>::indication_ = 0;

	Setting::Setting()
    : type_(stCurrent)
	{
	}

	Setting::~Setting()
	{
	}

	std::vector<boost::shared_ptr<Setting> > Setting::GetFromDatabase(std::string filter, Direction const dir, int const id, int const pageSize)
	{
		return DataAccess<Setting>::GetDatasByFilter(filter, modifyFieldByDB_, dir, id, pageSize);
	}

	boost::shared_ptr<Setting> Setting::GetCurrentConfig()
	{
		std::vector<boost::shared_ptr<Setting> > t = GetFromDatabase("type = " + Util::StringOp::FromInt(stCurrent));
		//assert(t.size() == 1)
		return t[0];
	}

	boost::shared_ptr<Setting> Setting::GetDefaultConfig()
	{
		std::vector<boost::shared_ptr<Setting> > t = GetFromDatabase("type = " + Util::StringOp::FromInt(stDefault));
		//assert(t.size() == 1)
		return t[0];
	}

	boost::shared_ptr<Setting> Setting::GetNormalConfig()
	{
		std::vector<boost::shared_ptr<Setting> > t = GetFromDatabase("type = " + Util::StringOp::FromInt(stNormal));
		//assert(t.size() == 1)
		return t[0];
	}

	boost::shared_ptr<Setting> Setting::GetSpeedConfig()
	{
		std::vector<boost::shared_ptr<Setting> > t = GetFromDatabase("type = " + Util::StringOp::FromInt(stSpeed));
		//assert(t.size() == 1)
		return t[0];
	}

	boost::shared_ptr<Setting> Setting::GetSpaceConfig()
	{
		std::vector<boost::shared_ptr<Setting> > t = GetFromDatabase("type = " + Util::StringOp::FromInt(stSpace));
		//assert(t.size() == 1)
		return t[0];
	}

	void Setting::Update() const
	{
        std::string cmd = "UPDATE ";
        cmd += tableName();
        cmd += " SET [type] = ";
        cmd += Util::StringOp::FromInt(type_);
        cmd += ", thisTelephoneNumber = '";
        cmd += thisTelephoneNumber_.ToString();
        cmd += "', outlinePrefix = '";
        cmd += outlinePrefix_;
        cmd += "', isAutoAppendOutlinePrefix = ";
        cmd += Util::StringOp::FromInt(isAutoAppendOutlinePrefix_);
        cmd += ", ipPrefix = '";
        cmd += ipPrefix_;
        cmd += "', isAppendIpPrefix = ";
        cmd += Util::StringOp::FromInt(isAppendIpPrefix_);
        cmd += ", isUseSpecRing = ";
        cmd += Util::StringOp::FromInt(isUseSpecRing_);
        cmd += ", defaultRingFilename = '";
        cmd += defaultRingFilename_;
        cmd += "', isAutoReply = ";
        cmd += Util::StringOp::FromInt(isAutoReply_);
        cmd += ", autoReplyRingCount = ";
        cmd += Util::StringOp::FromInt(autoReplyRingCount_);
		int idx = 0;
		for (std::vector< std::pair<std::string, std::string> >::const_iterator i = speedDials_.begin(); i != speedDials_.end(); ++i)
		{
			++idx;
			cmd += ", speedDialName" + Util::StringOp::FromInt(idx) + " = '";
			std::string name = i->first;
			cmd += i->first;
			cmd += "', speedDialValue" + Util::StringOp::FromInt(idx) + " = '";
			std::string str = i->second;
			cmd += i->second;
			cmd += "'";
		}
        cmd += ", linkMode = ";
        cmd += Util::StringOp::FromInt(linkMode_);
        cmd += ", dialUsername = '";
        cmd += dialUsername_;
        cmd += "', dialPassword = '";
        cmd += dialPassword_;
        cmd += "', ipMode = ";
        cmd += Util::StringOp::FromInt(ipMode_);
        cmd += ", thisIPAddr = ";
        cmd += Util::StringOp::FromInt(thisIPAddr_);
        cmd += ", subnetMask = ";
        cmd += Util::StringOp::FromInt(subnetMask_);
        cmd += ", gateway = ";
        cmd += Util::StringOp::FromInt(gateway_);
        cmd += ", dns = ";
        cmd += Util::StringOp::FromInt(dns_);
        cmd += ", isIndicationUser = ";
        cmd += Util::StringOp::FromInt(isIndicationUser_);
        cmd += ", isAlwaysSave = ";
        cmd += Util::StringOp::FromInt(isAlwaysSave_);
        cmd += ", copyright = '";
        cmd += copyright_;
        //cmd += "', currentTime = '";
        //cmd += Util::StringOp::FromTimestamp(currentTime_);
        cmd += "', hardwareVersion = '";
        cmd += hardwareVersion_;
        cmd += "', softwareVersion = '";
        cmd += softwareVersion_;
        //cmd += "', memorySize = ";
        //cmd += Util::StringOp::FromInt(memorySize_);
        //cmd += ", memoryUsedSize = ";
        //cmd += Util::StringOp::FromInt(memoryUsedSize_);
        //cmd += ", storageSize = ";
        //cmd += Util::StringOp::FromInt(storageSize_);
        //cmd += ", storageUsedSize = ";
        //cmd += Util::StringOp::FromInt(storageUsedSize_);
        //cmd += ", contactUseSize = ";
        //cmd += Util::StringOp::FromInt(contactUseSize_);
        //cmd += ", contactInfoUseSize = ";
        //cmd += Util::StringOp::FromInt(contactInfoUseSize_);
        //cmd += ", soundUseSize = ";
        //cmd += Util::StringOp::FromInt(soundUseSize_);
        cmd += "', maxContactUseSize = ";
        cmd += Util::StringOp::FromInt(maxContactUseSize_);
        cmd += ", maxContactInfoUseSize = ";
        cmd += Util::StringOp::FromInt(maxContactInfoUseSize_);
        cmd += ", maxSoundUseSize = ";
        cmd += Util::StringOp::FromInt(maxSoundUseSize_);
        cmd += ", soundVolume = ";
        cmd += Util::StringOp::FromInt(soundVolume_);
		cmd += ", systemVolume = ";
        cmd += Util::StringOp::FromInt(sysVolume_);
        cmd += ", flashDrvRoot = '";
        cmd += flashDrvRoot_;
        cmd += "', storageCardRoot = '";
        cmd += storageCardRoot_;
        cmd += "', usbDiskRoot = '";
        cmd += usbDiskRoot_;
        cmd += "', dataRootPath = '";
        cmd += dataRootPath_;
        cmd += "', picturePath = '";
        cmd += picturePath_;
        cmd += "', notePath = '";
        cmd += notePath_;
        cmd += "', musicPath = '";
        cmd += musicPath_;
        cmd += "', soundPath = '";
        cmd += soundPath_;
        cmd += "', videoPath = '";
        cmd += videoPath_;
        cmd += "', removeableSemiFixPath = '";
        cmd += removeableSemiFixPath_;
        cmd += "', removeableDynamicPath = '";
        cmd += removeableDynamicPath_;
        cmd += "', thisUserId = ";
        cmd += Util::StringOp::FromInt(thisUserId_);
		cmd += ", isFirewall = ";
		cmd += Util::StringOp::FromInt(isFirewall_);
        cmd += ", firewallType = ";
        cmd += Util::StringOp::FromInt(firewallType_);
        cmd += ", blockAllTimeDuration = ";
        cmd += Util::StringOp::FromTimeSpan(blockAllTimeDuration_);
        cmd += ", soundProtectPassword = '";
        cmd += soundProtectPassword_;
        cmd += "', isUseScreenSaver = ";
        cmd += Util::StringOp::FromInt(isUseScreenSaver_);
        cmd += ", screenSaverDuration = ";
        cmd += Util::StringOp::FromTimeSpan(screenSaverDuration_);
        cmd += ", screenSaverContent = ";
        cmd += Util::StringOp::FromInt(screenSaverContent_);
        cmd += ", isUseScreenSaverPassword = ";
        cmd += Util::StringOp::FromInt(isUseScreenSaverPassword_);
        cmd += ", screenSaverPassword = '";
        cmd += screenSaverPassword_;
        cmd += "', isAdmin = ";
		cmd += Util::StringOp::FromInt(isAdmin_);
		cmd += ", isPlayProtect = ";
		cmd += Util::StringOp::FromInt(isPlayProtect_);
		cmd += ", isMustRecord = ";
		cmd += Util::StringOp::FromInt(isMustRecord_);
		cmd += ", isDeleteProtect = ";
		cmd += Util::StringOp::FromInt(isDeleteProtect_);
		cmd += ", adminPassword = '";
		cmd += adminPassword_;
		cmd += "', playRecordPassword = '";
		cmd += playRecordPassword_;
		cmd += "', backlightDuration = ";
		cmd += Util::StringOp::FromTimeSpan(backlightDuration_);
		cmd += ", defaultTipFilename = '";
		cmd += defaultTipFilename_;
		cmd += "',";
		cmd += "blackLinghtValue = ";
		cmd += Util::StringOp::FromInt(blackLightValue_);
		cmd += ", isContrlBlackLight = ";
		cmd += Util::StringOp::FromInt(isContrlBlackLight_);
		cmd += ", controlBlackLightWaitTime = ";
		cmd += Util::StringOp::FromInt(contrlBlackLightWaitTime_);
		cmd += ", isNightControlBlackLight = ";
		cmd += Util::StringOp::FromInt(isNightControlBlackLight_);
		cmd += ", nightControlBlackLightStartTime = '";
		cmd += Util::StringOp::FromInt(nightControlBlackLightStartTime_);
		cmd += "', nightControlBlackLightEndTime = '";
		cmd += Util::StringOp::FromInt(nightControlBlackLightEndTime_);
		cmd += "', isPhoneCallRing = ";
		cmd += Util::StringOp::FromInt(isPhoneCallRing_);
		cmd += ", phoneCallRingFilename ='";
		cmd += phoneCallRingFilename_;
		cmd += "', phoneCallRingVolume = ";
		cmd += Util::StringOp::FromInt(phoneCallRingVolume_);
		cmd += ", isSmsRing = "; 
		cmd += Util::StringOp::FromInt(isSmsRing_);
		cmd += ", smsRingFilename ='";
		cmd += smsRingFilename_;
		cmd += "', smsRingVolume = ";
		cmd += Util::StringOp::FromInt(smsRingVolume_);
		cmd += ", isSpecodeRing = ";
		cmd += Util::StringOp::FromInt(isSpecodeRing_);
		cmd += ", specodeRingFilename = '";
		cmd += specodeRingFilename_;
		cmd += "', specodeRingVolume =";
		cmd += Util::StringOp::FromInt(specodeRingVolume_);
		cmd += ", speCode1 = '";
		cmd += speCode1_;
		cmd += "', speCode2 = '";
		cmd += speCode2_;
		cmd += "', speCode3 = '";
		cmd += speCode3_;
		cmd += "', speCode4 = '";
		cmd += speCode4_;
		cmd += "', speCode5 = '";
		cmd += speCode5_;
		cmd += "', speCode6 = '";
		cmd += speCode6_;
		cmd += "', speCode7 = '";
		cmd += speCode7_;
		cmd += "', speCode8 = '";
		cmd += speCode8_;
		cmd += "', speCode9 = '";
		cmd += speCode9_;
		cmd += "', speCode10 = '";
		cmd += speCode10_;
		cmd += "', speCode11 = '";
		cmd += speCode11_;
		cmd += "', speCode12 = '";
		cmd += speCode12_;
		cmd += "', callRecall = ";
		cmd += Util::StringOp::FromInt(callRecall_);
		cmd += ", phonedialtimes = ";
		cmd += Util::StringOp::FromInt(PhoneDialTimes_);
		cmd += ", callWaiting = ";
		cmd += Util::StringOp::FromInt(callWaiting_);
		cmd += ", isPin = ";
		cmd += Util::StringOp::FromInt(isPin_);
		cmd += ", pinPassWord = '";
		cmd += pinPassword_;
		cmd += "', gprsDialType1 = ";
		cmd += Util::StringOp::FromInt(gprsType1_);
		cmd += ", gprsHttp1 ='";
		cmd += gprsHttp1_;
		cmd += "', gprsDialNumber1 ='";
		cmd += gprsDialnumber1_;
		cmd += "', gprsProxyIp1 ='";
		cmd += gprsProxyIp1_;
		cmd += "', gprsProxyPort1 = ";
		cmd += Util::StringOp::FromInt(gprsProxyPort1_);
		cmd += ", gprsDialType2 = ";
		cmd += Util::StringOp::FromInt(gprsType2_);
		cmd += ", gprsHttp2 ='";
		cmd += gprsHttp2_;
		cmd += "', gprsDialNumber2 ='";
		cmd += gprsDialnumber2_;
		cmd += "', gprsProxyIp2 ='";
		cmd += gprsProxyIp2_;
		cmd += "', gprsProxyPort2 = ";
		cmd +=Util::StringOp::FromInt( gprsProxyPort2_);
		cmd += ", gprsDialType3 = '";
		cmd += gprsType3_;
		cmd += "', gprsHttp3 ='";
		cmd += gprsHttp3_;
		cmd += "', gprsDialNumber3 ='";
		cmd += gprsDialnumber3_;
		cmd += "', gprsProxyIp3 ='";
		cmd += gprsProxyIp3_;
		cmd += "', gprsProxyPort3 = ";
		cmd +=Util::StringOp::FromInt( gprsProxyPort3_);
		cmd += ", gprsDialType4 = '";
		cmd += gprsType4_;
		cmd += "', gprsHttp4 ='";
		cmd += gprsHttp4_;
		cmd += "', gprsDialNumber4 ='";
		cmd += gprsDialnumber4_;
		cmd += "', gprsProxyIp4 ='";
		cmd += gprsProxyIp4_;
		cmd += "', gprsProxyPort4 = ";
		cmd +=Util::StringOp::FromInt( gprsProxyPort4_);
		cmd += ", netType = ";
		cmd += Util::StringOp::FromInt(netType_);
		cmd += ", mmsType = ";
		cmd += Util::StringOp::FromInt(mmsType_);
		cmd += ", isphonedialtimes = ";
		cmd += Util::StringOp::FromInt(isPhoneDialTimes_);
		cmd += ", isDeepSleep = ";
		cmd += Util::StringOp::FromInt(isDeepSleep);
		cmd += " WHERE id = ";
        cmd += Util::StringOp::FromInt(id());
// 		FILE *file;
		//blackLinghtValue INTEGER, isContrlBlackLight INTEGER, controlBlackLightWaitTime INTEGER, isNightControlBlackLight INTEGER, nightControlBlackLightStartTime TEXT, nightControlBlackLightEndTime TEXT
// 		file = fopen("/temp/cms.txt", "w+b");
// 		if(file)
// 		{
// 			fwrite(cmd.c_str(), sizeof(char), strlen(cmd.c_str()), file);
// 			fclose(file);
// 		}
		extern VOID WriteLog(CHAR* str);
//		WriteLog((char *)cmd.c_str());

        ExecCommand(cmd);
	}

	void Setting::Insert()
	{
        std::string cmd = "INSERT INTO ";
        cmd += tableName();
        cmd += " ( type, thisTelephoneNumber, outlinePrefix, isAutoAppendOutlinePrefix, ipPrefix, isAppendIpPrefix, isUseSpecRing, defaultRingFilename, isAutoReply, autoReplyRingCount, speedDialName1, speedDialValue1, speedDialName2, speedDialValue2, speedDialName3, speedDialValue3, speedDialName4, speedDialValue4, speedDialName5, speedDialValue5, speedDialName6, speedDialValue6, speedDialName7, speedDialValue7, speedDialName8, speedDialValue8, speedDialName9, speedDialValue9, speedDialName10, speedDialValue10, speedDialName11, speedDialValue11, speedDialName12, speedDialValue12, linkMode, dialUsername, dialPassword, ipMode, thisIPAddr, subnetMask, gateway, dns, isIndicationUser, isAlwaysSave, copyright, hardwareVersion, softwareVersion, maxContactUseSize, maxContactInfoUseSize, maxSoundUseSize, soundVolume, systemVolume, flashDrvRoot, storageCardRoot, usbDiskRoot, dataRootPath, picturePath, notePath, musicPath, soundPath, videoPath, removeableSemiFixPath, removeableDynamicPath, thisUserId, isFirewall, firewallType, blockAllTimeDuration, soundProtectPassword, isUseScreenSaver, screenSaverDuration, screenSaverContent, isUseScreenSaverPassword, screenSaverPassword, isAdmin, isPlayProtect, isMustRecord, isDeleteProtect, adminPassword, playRecordPassword, backlightDuration, defaultTipFilename, blackLinghtValue, isContrlBlackLight, controlBlackLightWaitTime, isNightControlBlackLight, nightControlBlackLightStartTime, nightControlBlackLightEndTime, isSmsRing, smsRingFilename, smsRingVolume, isSpecodeRing , specodeRingFilename , specodeRingVolume , speCode1 , speCode2 , speCode3 , speCode4 ,speCode5 , speCode6 , speCode7 , speCode8 ,speCode9 , speCode10 , speCode11 , speCode12 , callRecall , phonedialtimes , callWaiting , isPin , pinPassWord , ";
        cmd += "gprsDialType1 , gprsHttp1 , gprsDialNumber1 , gprsProxyIp1 , gprsProxyPort1 , gprsDialType2 , gprsHttp2 , gprsDialNumber2 , gprsProxyIp2 , gprsProxyPort2, gprsDialType3 , gprsHttp3 , gprsDialNumber3 , gprsProxyIp3 , gprsProxyPort3, gprsDialType4 , gprsHttp4 , gprsDialNumber4 , gprsProxyIp4 , gprsProxyPort4, netType, mmsType,isphonedialtimes,isDeepSleep) VALUES ( ";
        cmd += Util::StringOp::FromInt(type_);
        cmd += ", '";
        cmd += thisTelephoneNumber_.ToString();
        cmd += "', '";
		cmd += outlinePrefix_;
		cmd += "', ";
		cmd += Util::StringOp::FromInt(isAutoAppendOutlinePrefix_);
        cmd += ", '";
		cmd += ipPrefix_;
		cmd += "', ";
		cmd += Util::StringOp::FromInt(isAppendIpPrefix_);
		cmd += ", ";
		cmd += Util::StringOp::FromInt(isUseSpecRing_);
		cmd += ", '";
		cmd += defaultRingFilename_;
		cmd += "', ";
		cmd += Util::StringOp::FromInt(isAutoReply_);
		cmd += ", ";
		cmd += Util::StringOp::FromInt(autoReplyRingCount_);
		cmd += ", ";
		for (std::vector<std::pair<std::string, std::string> >::iterator i = speedDials_.begin(); i != speedDials_.end(); ++i)
		{
			cmd += "'";
			cmd += i->first;
			cmd += "', '";
			cmd += i->second;
			cmd += "', ";
		}
		cmd += Util::StringOp::FromInt(linkMode_);
		cmd += ", '";
		cmd += dialUsername_;
		cmd += "', '";
		cmd += dialPassword_;
		cmd += "', ";
		cmd += Util::StringOp::FromInt(ipMode_);
		cmd += ", ";
		cmd += Util::StringOp::FromInt(thisIPAddr_);
		cmd += ", ";
		cmd += Util::StringOp::FromInt(subnetMask_);
		cmd += ", ";
		cmd += Util::StringOp::FromInt(gateway_);
		cmd += ", ";
		cmd += Util::StringOp::FromInt(dns_);
		cmd += ", ";
		cmd += Util::StringOp::FromInt(isIndicationUser_);
		cmd += ", ";
		cmd += Util::StringOp::FromInt(isAlwaysSave_);
		cmd += ", '";
		cmd += copyright_;
		//cmd += "', '";
		//cmd += Util::StringOp::FromTimestamp(currentTime_);
        cmd += "', '";
		cmd += hardwareVersion_;
		cmd += "', '";
		cmd += softwareVersion_;
		//cmd += "', ";
		//cmd += Util::StringOp::FromInt(memorySize_);
		//cmd += ", ";
		//cmd += Util::StringOp::FromInt(memoryUsedSize_);
		//cmd += ", ";
		//cmd += Util::StringOp::FromInt(storageSize_);
		//cmd += ", ";
		//cmd += Util::StringOp::FromInt(storageUsedSize_);
		//cmd += ", ";
		//cmd += Util::StringOp::FromInt(contactUseSize_);
		//cmd += ", ";
		//cmd += Util::StringOp::FromInt(contactInfoUseSize_);
		//cmd += ", ";
		//cmd += Util::StringOp::FromInt(soundUseSize_);
        cmd += "', ";
        cmd += Util::StringOp::FromInt(maxContactUseSize_);
        cmd += ", ";
        cmd += Util::StringOp::FromInt(maxContactInfoUseSize_);
        cmd += ", ";
        cmd += Util::StringOp::FromInt(maxSoundUseSize_);
        cmd += ", ";
        cmd += Util::StringOp::FromInt(soundVolume_);
		cmd += ", ";
        cmd += Util::StringOp::FromInt(sysVolume_);
		cmd += ", '";
		cmd += flashDrvRoot_;
		cmd += "', '";
		cmd += storageCardRoot_;
		cmd += "', '";
		cmd += usbDiskRoot_;
		cmd += "', '";
		cmd += dataRootPath_;
		cmd += "', '";
		cmd += picturePath_;
		cmd += "', '";
		cmd += notePath_;
		cmd += "', '";
		cmd += musicPath_;
		cmd += "', '";
		cmd += soundPath_;
		cmd += "', '";
		cmd += videoPath_;
		cmd += "', '";
		cmd += removeableSemiFixPath_;
		cmd += "', '";
		cmd += removeableDynamicPath_;
		cmd += "', '";
		cmd += storageCardRoot_;
		cmd += "', ";
		cmd += Util::StringOp::FromInt(thisUserId_);
		cmd += ", ";
		cmd += Util::StringOp::FromInt(isFirewall_);
		cmd += ", ";
		cmd += Util::StringOp::FromInt(firewallType_);
		cmd += ", ";
		cmd += Util::StringOp::FromTimeSpan(blockAllTimeDuration_);
		cmd += ", '";
		cmd += soundProtectPassword_;
		cmd += "', ";
		cmd += Util::StringOp::FromInt(isUseScreenSaver_);
		cmd += ", ";
		cmd += Util::StringOp::FromTimeSpan(screenSaverDuration_);
		cmd += ", ";
		cmd += Util::StringOp::FromInt(screenSaverContent_);
		cmd += ", ";
		cmd += Util::StringOp::FromInt(isUseScreenSaverPassword_);
		cmd += ", '";
        cmd += screenSaverPassword_;
        cmd += "', ";
		cmd += Util::StringOp::FromInt(isAdmin_);
		cmd += ", ";
		cmd += Util::StringOp::FromInt(isPlayProtect_);
		cmd += ", ";
		cmd += Util::StringOp::FromInt(isMustRecord_);
		cmd += ", ";
		cmd += Util::StringOp::FromInt(isDeleteProtect_);
		cmd += ", '";
		cmd += adminPassword_;
		cmd += "', '";
		cmd += playRecordPassword_;
		cmd += "', ";
		cmd += Util::StringOp::FromTimeSpan(backlightDuration_);
		cmd += ", '";
		cmd += defaultTipFilename_;
		cmd += "',";
		cmd += Util::StringOp::FromInt(blackLightValue_);
		cmd += ", ";
		cmd += Util::StringOp::FromInt(isContrlBlackLight_);
		cmd += ", ";
		cmd += Util::StringOp::FromInt(contrlBlackLightWaitTime_);
	    cmd += ", ";
		cmd += Util::StringOp::FromInt(isNightControlBlackLight_);
		cmd += ", '";
		cmd += Util::StringOp::FromInt(nightControlBlackLightStartTime_);
		cmd += "', '";
		cmd += Util::StringOp::FromInt(nightControlBlackLightEndTime_);
		cmd += "', ";
		cmd += Util::StringOp::FromInt(isPhoneCallRing_);
		cmd += ", '";
		cmd += phoneCallRingFilename_;
		cmd += "', ";
		cmd += Util::StringOp::FromInt(phoneCallRingVolume_);
		cmd += ", ";
		cmd += Util::StringOp::FromInt(isSmsRing_);
		cmd += ", '";
		cmd += smsRingFilename_;
		cmd += "', ";
		cmd += Util::StringOp::FromInt(smsRingVolume_);
		cmd += ", ";
		cmd += Util::StringOp::FromInt(isSpecodeRing_);
		cmd += ", '";
		cmd += specodeRingFilename_;
		cmd += "', ";
		cmd += Util::StringOp::FromInt(specodeRingVolume_);
		cmd += ", '";
		cmd += speCode1_;
		cmd += "', '";
		cmd += speCode2_;
		cmd += "', '";
		cmd += speCode3_;
		cmd += "', '";
		cmd += speCode4_;
		cmd += "', '";
		cmd += speCode5_;
		cmd += "', '";
		cmd += speCode6_;
		cmd += "', '";
		cmd += speCode7_;
		cmd += "', '";
		cmd += speCode8_;
		cmd += "', '";
		cmd += speCode9_;
		cmd += "', '";
		cmd += speCode10_;
		cmd += "', '";
		cmd += speCode11_;
		cmd += "', '";
		cmd += speCode12_;
		cmd += "', '";
		cmd += Util::StringOp::FromInt(callRecall_);
		cmd += ", ";
		cmd += Util::StringOp::FromInt(PhoneDialTimes_);
		cmd += ", ";
		cmd += Util::StringOp::FromInt(callWaiting_);
		cmd += ", ";
		cmd += Util::StringOp::FromInt(isPin_);
		cmd += ", '";
		cmd += pinPassword_;
		cmd += "', ";
		cmd += Util::StringOp::FromInt(gprsType1_);
		cmd += ", '";
		cmd += gprsHttp1_;
		cmd += "', '";
		cmd += gprsDialnumber1_;
		cmd += "', '";
		cmd += gprsProxyIp1_;
		cmd += "', ";
		cmd += Util::StringOp::FromInt(gprsProxyPort1_);
		cmd += ", ";
		cmd += Util::StringOp::FromInt(gprsType2_);
		cmd += ", '";
		cmd += gprsHttp2_;
		cmd += "', '";
		cmd += gprsDialnumber2_;
		cmd += "', '";
		cmd += gprsProxyIp2_;
		cmd += "', ";
		cmd += Util::StringOp::FromInt(gprsProxyPort2_);
		cmd += ", '";
		cmd += gprsType3_;
		cmd += "', '";
		cmd += gprsHttp3_;
		cmd += "', '";
		cmd += gprsDialnumber3_;
		cmd += "', '";
		cmd += gprsProxyIp3_;
		cmd += "', ";
		cmd += Util::StringOp::FromInt(gprsProxyPort3_);
		cmd += ", '";
		cmd += gprsType4_;
		cmd += "', '";
		cmd += gprsHttp4_;
		cmd += "', '";
		cmd += gprsDialnumber4_;
		cmd += "', '";
		cmd += gprsProxyIp4_;
		cmd += "', ";
		cmd += Util::StringOp::FromInt(gprsProxyPort4_);
		cmd += ", ";
		cmd += Util::StringOp::FromInt(netType_);
		cmd += ", ";
		cmd += Util::StringOp::FromInt(mmsType_);
		cmd += ", ";
		cmd += Util::StringOp::FromInt(isPhoneDialTimes_);
		cmd += ", ";
		cmd += Util::StringOp::FromInt(isDeepSleep);
		cmd += ")";
		
        ExecCommand(cmd);
        id(GetCurrentId());
	}

	void Setting::Remove(std::string filter)
	{
		DataAccess<Setting>::RemoveDatasByFilter(filter);
	}

	void Setting::modifyFieldByDB_(int argc, char** argv, char** columnName, boost::shared_ptr<Setting> item)
	{
        item->id(atoi(argv[Data::getIndexByName(argc, columnName, "id")]));
        item->type_ = static_cast<SettingType>(atoi(argv[Data::getIndexByName(argc, columnName, "type")]));
        item->thisTelephoneNumber_ = TelephoneNumber(argv[Data::getIndexByName(argc, columnName, "thisTelephoneNumber")]);
        item->outlinePrefix_ = argv[Data::getIndexByName(argc, columnName, "outlinePrefix")];
        item->isAutoAppendOutlinePrefix_ = !!atoi(argv[Data::getIndexByName(argc, columnName, "isAutoAppendOutlinePrefix")]);
        item->ipPrefix_ = argv[Data::getIndexByName(argc, columnName, "ipPrefix")];
        item->isAppendIpPrefix_ = !!atoi(argv[Data::getIndexByName(argc, columnName, "isAppendIpPrefix")]);
        item->isUseSpecRing_ = !!atoi(argv[Data::getIndexByName(argc, columnName, "isUseSpecRing")]);
        item->defaultRingFilename_ = argv[Data::getIndexByName(argc, columnName, "defaultRingFilename")];
        item->isAutoReply_ = !!atoi(argv[Data::getIndexByName(argc, columnName, "isAutoReply")]);
        item->autoReplyRingCount_ = atoi(argv[Data::getIndexByName(argc, columnName, "autoReplyRingCount")]);
		for (int i = 1; i < 13; ++i)
		{
			std::string name = "speedDialName" + Util::StringOp::FromInt(i);
			std::string value = "speedDialValue" + Util::StringOp::FromInt(i);
			std::string n = argv[Data::getIndexByName(argc, columnName, const_cast<char*>(name.c_str()))];
			std::string v = argv[Data::getIndexByName(argc, columnName, const_cast<char*>(value.c_str()))];
			std::pair<std::string, std::string> temp(n, v);
			item->speedDials_.push_back(temp);
		}

        item->id(atoi(argv[Data::getIndexByName(argc, columnName, "id")]));
        item->linkMode_ = static_cast<LinkMode>(atoi(argv[Data::getIndexByName(argc, columnName, "linkMode")]));
        item->dialUsername_ = argv[Data::getIndexByName(argc, columnName, "dialUsername")];
        item->dialPassword_ = argv[Data::getIndexByName(argc, columnName, "dialPassword")];
        item->ipMode_ = static_cast<IPMode>(atoi(argv[Data::getIndexByName(argc, columnName, "ipMode")]));
        item->thisIPAddr_ = atoi(argv[Data::getIndexByName(argc, columnName, "thisIPAddr")]);
        item->subnetMask_ = atoi(argv[Data::getIndexByName(argc, columnName, "subnetMask")]);
        item->gateway_ = atoi(argv[Data::getIndexByName(argc, columnName, "gateway")]);
        item->dns_ = atoi(argv[Data::getIndexByName(argc, columnName, "dns")]);

        item->isIndicationUser_ = !!atoi(argv[Data::getIndexByName(argc, columnName, "isIndicationUser")]);
        item->isAlwaysSave_ = !!atoi(argv[Data::getIndexByName(argc, columnName, "isAlwaysSave")]);

        item->copyright_ = argv[Data::getIndexByName(argc, columnName, "copyright")];
        //item->currentTime_ = Util::StringOp::ToTimestamp(argv[Data::getIndexByName(argc, columnName, "currentTime")]);
        item->hardwareVersion_ = argv[Data::getIndexByName(argc, columnName, "hardwareVersion")];
        item->softwareVersion_ = argv[Data::getIndexByName(argc, columnName, "softwareVersion")];
        //item->memorySize_ = atoi(argv[Data::getIndexByName(argc, columnName, "memorySize")]);
        //item->memoryUsedSize_ = atoi(argv[Data::getIndexByName(argc, columnName, "memoryUsedSize")]);
        //item->storageSize_ = atoi(argv[Data::getIndexByName(argc, columnName, "storageSize")]);
        //item->storageUsedSize_ = atoi(argv[Data::getIndexByName(argc, columnName, "storageUsedSize")]);
        //item->contactUseSize_ = atoi(argv[Data::getIndexByName(argc, columnName, "contactUseSize")]);
        //item->contactInfoUseSize_ = atoi(argv[Data::getIndexByName(argc, columnName, "contactInfoUseSize")]);
        //item->soundUseSize_ = atoi(argv[Data::getIndexByName(argc, columnName, "soundUseSize")]);
        item->maxContactUseSize_ = atoi(argv[Data::getIndexByName(argc, columnName, "maxContactUseSize")]);
        item->maxContactInfoUseSize_ = atoi(argv[Data::getIndexByName(argc, columnName, "maxContactInfoUseSize")]);
        item->maxSoundUseSize_ = atoi(argv[Data::getIndexByName(argc, columnName, "maxSoundUseSize")]);
        item->soundVolume_ = atoi(argv[Data::getIndexByName(argc, columnName, "soundVolume")]);
		item->sysVolume_ = atoi(argv[Data::getIndexByName(argc, columnName, "systemVolume")]);

        item->flashDrvRoot_ = argv[Data::getIndexByName(argc, columnName, "flashDrvRoot")];
        item->storageCardRoot_ = argv[Data::getIndexByName(argc, columnName, "storageCardRoot")];
        item->usbDiskRoot_ = argv[Data::getIndexByName(argc, columnName, "usbDiskRoot")];
        item->dataRootPath_ = argv[Data::getIndexByName(argc, columnName, "dataRootPath")];
        item->picturePath_ = argv[Data::getIndexByName(argc, columnName, "picturePath")];
        item->notePath_ = argv[Data::getIndexByName(argc, columnName, "notePath")];
        item->musicPath_ = argv[Data::getIndexByName(argc, columnName, "musicPath")];
        item->soundPath_ = argv[Data::getIndexByName(argc, columnName, "soundPath")];
        item->videoPath_ = argv[Data::getIndexByName(argc, columnName, "videoPath")];
        item->removeableSemiFixPath_ = argv[Data::getIndexByName(argc, columnName, "removeableSemiFixPath")];
        item->removeableDynamicPath_ = argv[Data::getIndexByName(argc, columnName, "removeableDynamicPath")];

        item->thisUserId_ = atoi(argv[Data::getIndexByName(argc, columnName, "thisUserId")]);
        item->isFirewall_ = !!atoi(argv[Data::getIndexByName(argc, columnName, "isFirewall")]);
        item->firewallType_ = static_cast<FirewallType>(atoi(argv[Data::getIndexByName(argc, columnName, "firewallType")]));
        item->blockAllTimeDuration_ = atoi(argv[Data::getIndexByName(argc, columnName, "blockAllTimeDuration")]);
        item->soundProtectPassword_ = argv[Data::getIndexByName(argc, columnName, "soundProtectPassword")];

        item->isUseScreenSaver_ = !!atoi(argv[Data::getIndexByName(argc, columnName, "isUseScreenSaver")]);
		item->screenSaverDuration_ = Util::StringOp::ToTimeSpan(std::string(argv[Data::getIndexByName(argc, columnName, "screenSaverDuration")]));
		item->screenSaverContent_ = static_cast<ScreenSaverContent>(atoi(argv[Data::getIndexByName(argc, columnName, "screenSaverContent")]));
        item->isUseScreenSaverPassword_ = !!atoi(argv[Data::getIndexByName(argc, columnName, "isUseScreenSaverPassword")]);
        item->screenSaverPassword_ = argv[Data::getIndexByName(argc, columnName, "screenSaverPassword")];
		
        item->isAdmin_ = !!atoi(argv[Data::getIndexByName(argc, columnName, "isAdmin")]);
        item->isPlayProtect_ = !!atoi(argv[Data::getIndexByName(argc, columnName, "isPlayProtect")]);
        item->isMustRecord_ = !!atoi(argv[Data::getIndexByName(argc, columnName, "isMustRecord")]);
        item->isDeleteProtect_ = !!atoi(argv[Data::getIndexByName(argc, columnName, "isDeleteProtect")]);
        item->adminPassword_ = argv[Data::getIndexByName(argc, columnName, "adminPassword")];
        item->playRecordPassword_ = argv[Data::getIndexByName(argc, columnName, "playRecordPassword")];
        item->backlightDuration_ = atoi(argv[Data::getIndexByName(argc, columnName, "backlightDuration")]);
        item->defaultTipFilename_ = argv[Data::getIndexByName(argc, columnName, "defaultTipFilename")];

		item->blackLightValue_ = atoi(argv[Data::getIndexByName(argc, columnName, "blackLinghtValue")]);
		item->isContrlBlackLight_ = !!atoi(argv[Data::getIndexByName(argc, columnName, "isContrlBlackLight")]);
		item->isNightControlBlackLight_ = !!atoi(argv[Data::getIndexByName(argc, columnName, "isNightControlBlackLight")]);
		item->contrlBlackLightWaitTime_ = atoi(argv[Data::getIndexByName(argc, columnName, "controlBlackLightWaitTime")]);
		item->nightControlBlackLightStartTime_ = atoi(argv[Data::getIndexByName(argc, columnName, "nightControlBlackLightStartTime")]);
		item->nightControlBlackLightEndTime_ = atoi(argv[Data::getIndexByName(argc, columnName, "nightControlBlackLightEndTime")]);
		
		item->isPhoneCallRing_ = atoi(argv[Data::getIndexByName(argc, columnName, "isPhoneCallRing")]);
		item->phoneCallRingFilename_ = argv[Data::getIndexByName(argc, columnName, "phoneCallRingFilename")];
		item->phoneCallRingVolume_ = atoi(argv[Data::getIndexByName(argc, columnName, "phoneCallRingVolume")]);
		item->isSmsRing_ = atoi(argv[Data::getIndexByName(argc, columnName, "isSmsRing")]);
		item->smsRingFilename_ = argv[Data::getIndexByName(argc, columnName, "smsRingFilename")];
		item->smsRingVolume_ = atoi(argv[Data::getIndexByName(argc, columnName, "smsRingVolume")]); 
		item->isSpecodeRing_ = atoi(argv[Data::getIndexByName(argc, columnName, "isSpecodeRing")]);
		item->specodeRingFilename_ = argv[Data::getIndexByName(argc, columnName, "specodeRingFilename")]; 
		item->specodeRingVolume_ = atoi(argv[Data::getIndexByName(argc, columnName, "specodeRingVolume")]);
		item->speCode1_ = argv[Data::getIndexByName(argc, columnName, "speCode1")];
		item->speCode2_ = argv[Data::getIndexByName(argc, columnName, "speCode2")];
		item->speCode3_ = argv[Data::getIndexByName(argc, columnName, "speCode3")];
		item->speCode4_ = argv[Data::getIndexByName(argc, columnName, "speCode4")];
		item->speCode5_ = argv[Data::getIndexByName(argc, columnName, "speCode5")];
		item->speCode6_ = argv[Data::getIndexByName(argc, columnName, "speCode6")];
		item->speCode7_ = argv[Data::getIndexByName(argc, columnName, "speCode7")];
		item->speCode8_ = argv[Data::getIndexByName(argc, columnName, "speCode8")];
		item->speCode9_ = argv[Data::getIndexByName(argc, columnName, "speCode9")];
		item->speCode10_ = argv[Data::getIndexByName(argc, columnName, "speCode10")];
		item->speCode11_ = argv[Data::getIndexByName(argc, columnName, "speCode11")];
		item->speCode12_ = argv[Data::getIndexByName(argc, columnName, "speCode12")];
		item->callRecall_ = atoi(argv[Data::getIndexByName(argc, columnName, "callRecall")]);
		item->PhoneDialTimes_ = atoi(argv[Data::getIndexByName(argc, columnName, "phonedialtimes")]);
		item->callWaiting_ = atoi(argv[Data::getIndexByName(argc, columnName, "callWaiting")]);
		item->isPin_ = atoi(argv[Data::getIndexByName(argc, columnName, "isPin")]);
		item->pinPassword_ = argv[Data::getIndexByName(argc, columnName, "pinPassWord")];
		item->gprsType1_ = atoi(argv[Data::getIndexByName(argc, columnName, "gprsDialType1")]);
		item->gprsHttp1_ = argv[Data::getIndexByName(argc, columnName, "gprsHttp1")];
		item->gprsDialnumber1_ = argv[Data::getIndexByName(argc, columnName, "gprsDialNumber1")];
		item->gprsProxyIp1_ = argv[Data::getIndexByName(argc, columnName, "gprsProxyIp1")];
		item->gprsProxyPort1_ = atoi(argv[Data::getIndexByName(argc, columnName, "gprsProxyPort1")]);
		item->gprsType2_ = atoi(argv[Data::getIndexByName(argc, columnName, "gprsDialType2")]);
		item->gprsHttp2_ = argv[Data::getIndexByName(argc, columnName, "gprsHttp2")];
		item->gprsDialnumber2_ = argv[Data::getIndexByName(argc, columnName, "gprsDialNumber2")];
		item->gprsProxyIp2_ = argv[Data::getIndexByName(argc, columnName, "gprsProxyIp2")];
		item->gprsProxyPort2_ = atoi(argv[Data::getIndexByName(argc, columnName, "gprsProxyPort2")]);
		//wangzhenxing20100505
		item->gprsType3_ = argv[Data::getIndexByName(argc, columnName, "gprsDialType3")];
		item->gprsHttp3_ = argv[Data::getIndexByName(argc, columnName, "gprsHttp3")];
		item->gprsDialnumber3_ = argv[Data::getIndexByName(argc, columnName, "gprsDialNumber3")];
		item->gprsProxyIp3_ = argv[Data::getIndexByName(argc, columnName, "gprsProxyIp3")];
		item->gprsProxyPort3_ = atoi(argv[Data::getIndexByName(argc, columnName, "gprsProxyPort3")]);

		item->gprsType4_ = argv[Data::getIndexByName(argc, columnName, "gprsDialType4")];
		item->gprsHttp4_ = argv[Data::getIndexByName(argc, columnName, "gprsHttp4")];
		item->gprsDialnumber4_ = argv[Data::getIndexByName(argc, columnName, "gprsDialNumber4")];
		item->gprsProxyIp4_ = argv[Data::getIndexByName(argc, columnName, "gprsProxyIp4")];
		item->gprsProxyPort4_ = atoi(argv[Data::getIndexByName(argc, columnName, "gprsProxyPort4")]);

		item->netType_ = atoi(argv[Data::getIndexByName(argc, columnName, "netType")]);
		item->mmsType_ = atoi(argv[Data::getIndexByName(argc, columnName, "mmsType")]);

		//add by qi 
		item->isPhoneDialTimes_ = atoi(argv[Data::getIndexByName(argc, columnName, "isphonedialtimes")]);
		item->isDeepSleep = !!atoi(argv[Data::getIndexByName(argc, columnName, "isDeepSleep")]);
	}
}
