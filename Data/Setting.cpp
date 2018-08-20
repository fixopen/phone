#include "../StdAfx.h"
#include "Setting.h"

namespace Data
{
	std::string DataAccess<Setting>::tableName_ = "setting";
	sqlite3* DataAccess<Setting>::db_ = Data::GetDB();
    int DataAccess<Setting>::rowCount_ = 0;
	Indication DataAccess<Setting>::indication_ = 0;
	int DataAccess<Setting>::offset_ = 0;

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
		for (std::map<char, std::string>::const_iterator i = speedDials_.begin(); i != speedDials_.end(); ++i)
		{
			++idx;
			cmd += ", speedDialName" + Util::StringOp::FromInt(idx) + " = ";
			int c = i->first;
			cmd += Util::StringOp::FromInt(i->first);
			cmd += ", speedDialValue" + Util::StringOp::FromInt(idx) + " = '";
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
        cmd += "', hardwareVersion = '";
        cmd += hardwareVersion_;
        cmd += "', softwareVersion = '";
        cmd += softwareVersion_;
        cmd += "', maxContactUseSize = ";
        cmd += Util::StringOp::FromInt(maxContactUseSize_);
        cmd += ", maxContactInfoUseSize = ";
        cmd += Util::StringOp::FromInt(maxContactInfoUseSize_);
        cmd += ", maxSoundUseSize = ";
        cmd += Util::StringOp::FromInt(maxSoundUseSize_);
        cmd += ", soundVolume = ";
        cmd += Util::StringOp::FromInt(soundVolume_);
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
//defect 36
//        cmd += ", screenSaverPassword = '";
//        cmd += screenSaverPassword_;
        cmd += ", isAdmin = ";
//=================================
		cmd += Util::StringOp::FromInt(isAdmin_);
		cmd += ", isPlayProtect = ";
		cmd += Util::StringOp::FromInt(isPlayProtect_);
		cmd += ", isMustRecord = ";
		cmd += Util::StringOp::FromInt(isMustRecord_);
		cmd += ", isDeleteProtect = ";
		cmd += Util::StringOp::FromInt(isDeleteProtect_);
//defect 36
//		cmd += ", adminPassword = '";
//		cmd += adminPassword_;
//		cmd += "', playRecordPassword = '";
//		cmd += playRecordPassword_;

		cmd += ", backlightDuration = ";
//======================
		cmd += Util::StringOp::FromTimeSpan(backlightDuration_);

		cmd += ", defaultTipFilename = '";
		cmd += defaultTipFilename_;
		cmd += "', localNumber = '";
		cmd += localNumber_;
		cmd += "'";
		cmd += " WHERE id = ";
        cmd += Util::StringOp::FromInt(id());
 		FILE *file;
 		file = fopen("/temp/cms.txt", "w+b");
 		if(file)
 		{
 			fwrite(cmd.c_str(), sizeof(char), strlen(cmd.c_str()), file);
 			fclose(file);
 		}
        ExecCommand(cmd);
	}
//defect 36
	void Setting::UpdateSoundProtectPassword() const
	{
        std::string cmd = "UPDATE ";
        cmd += tableName();
        cmd += " SET [type] = ";
        cmd += Util::StringOp::FromInt(type_);

        cmd += ", soundProtectPassword = '";
        cmd += soundProtectPassword_;

		cmd += "'";
		cmd += " WHERE id = ";
        cmd += Util::StringOp::FromInt(id());
 		FILE *file;
 		file = fopen("/temp/cmssoundprotect.txt", "w+b");
 		if(file)
 		{
 			fwrite(cmd.c_str(), sizeof(char), strlen(cmd.c_str()), file);
 			fclose(file);
 		}
        ExecCommand(cmd);
	}
	void Setting::UpdateScreenSaverPassword(std::string const screenSaverPassword) const
	{

        std::string cmd = "UPDATE ";
        cmd += tableName();
        cmd += " SET screenSaverPassword = '";
        
		cmd += screenSaverPassword;

		cmd += "'";
		cmd += " WHERE id = ";
        cmd += Util::StringOp::FromInt(id());
 		FILE *file;
 		file = fopen("/temp/cmsscreensaver.txt", "w+b");
 		if(file)
 		{
 			fwrite(cmd.c_str(), sizeof(char), strlen(cmd.c_str()), file);
 			fclose(file);
 		}
        ExecCommand(cmd);
	}

	void Setting::UpdateAdminPassword(std::string const adminPassword) const
	{
        std::string cmd = "UPDATE ";
        cmd += tableName();
        cmd += " SET adminPassword = '";

		cmd += adminPassword;
		cmd += "'";

		cmd += " WHERE id = ";
        cmd += Util::StringOp::FromInt(id());
 		FILE *file;
 		file = fopen("/temp/cmsadmin.txt", "w+b");
 		if(file)
 		{
 			fwrite(cmd.c_str(), sizeof(char), strlen(cmd.c_str()), file);
 			fclose(file);
 		}
        ExecCommand(cmd);
	}
    
   void Setting::Closedb()
	{
		Data::CloseDB(DataAccess<Setting>::db_);
	}
	void Setting::Opendb()
	{
		DataAccess<Setting>::db_=Data::OpenDB();
	}

	void Setting::UpdatePlayRecordPassword(std::string const playRecordPassword ) const
	{
        std::string cmd = "UPDATE ";
        cmd += tableName();
        cmd += " SET playRecordPassword = '";

		cmd += playRecordPassword;

		cmd += "'";
		cmd += " WHERE id = ";
        cmd += Util::StringOp::FromInt(id());
 		FILE *file;
 		file = fopen("/temp/cmsplayrecord.txt", "w+b");
 		if(file)
 		{
 			fwrite(cmd.c_str(), sizeof(char), strlen(cmd.c_str()), file);
 			fclose(file);
 		}
        ExecCommand(cmd);
	}

/////////////////////
	void Setting::Insert()
	{
        std::string cmd = "INSERT INTO ";
        cmd += tableName();
        cmd += " ( type, thisTelephoneNumber, outlinePrefix, isAutoAppendOutlinePrefix, ipPrefix, isAppendIpPrefix, isUseSpecRing, defaultRingFilename, isAutoReply, autoReplyRingCount, speedDialName1, speedDialValue1, speedDialName2, speedDialValue2, speedDialName3, speedDialValue3, speedDialName4, speedDialValue4, speedDialName5, speedDialValue5, speedDialName6, speedDialValue6, speedDialName7, speedDialValue7, speedDialName8, speedDialValue8, speedDialName9, speedDialValue9, speedDialName10, speedDialValue10, speedDialName11, speedDialValue11, speedDialName12, speedDialValue12, linkMode, dialUsername, dialPassword, ipMode, thisIPAddr, subnetMask, gateway, dns, isIndicationUser, isAlwaysSave, copyright, hardwareVersion, softwareVersion, maxContactUseSize, maxContactInfoUseSize, maxSoundUseSize, soundVolume, flashDrvRoot, storageCardRoot, usbDiskRoot, dataRootPath, picturePath, notePath, musicPath, soundPath, videoPath, removeableSemiFixPath, removeableDynamicPath, thisUserId, isFirewall, firewallType, blockAllTimeDuration, soundProtectPassword, isUseScreenSaver, screenSaverDuration, screenSaverContent, isUseScreenSaverPassword, screenSaverPassword, isAdmin, isPlayProtect, isMustRecord, isDeleteProtect, adminPassword, playRecordPassword, backlightDuration, defaultTipFilename, localNumber) VALUES ( ";
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
		for (std::map<char, std::string>::iterator i = speedDials_.begin(); i != speedDials_.end(); ++i)
		{
			cmd += Util::StringOp::FromInt(i->first);
			cmd += ", '";
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
        cmd += "', '";
		cmd += hardwareVersion_;
		cmd += "', '";
		cmd += softwareVersion_;
        cmd += "', ";
        cmd += Util::StringOp::FromInt(maxContactUseSize_);
        cmd += ", ";
        cmd += Util::StringOp::FromInt(maxContactInfoUseSize_);
        cmd += ", ";
        cmd += Util::StringOp::FromInt(maxSoundUseSize_);
        cmd += ", ";
        cmd += Util::StringOp::FromInt(soundVolume_);
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
		cmd += "', '";
		cmd += localNumber_;
		cmd += "' )";
		
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
			item->speedDials_[Util::StringOp::ToInt(n)] = v;
			//item->speedDials_[*] = ;
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
        item->maxContactUseSize_ = atoi(argv[Data::getIndexByName(argc, columnName, "maxContactUseSize")]);
        item->maxContactInfoUseSize_ = atoi(argv[Data::getIndexByName(argc, columnName, "maxContactInfoUseSize")]);
        item->maxSoundUseSize_ = atoi(argv[Data::getIndexByName(argc, columnName, "maxSoundUseSize")]);
        item->soundVolume_ = atoi(argv[Data::getIndexByName(argc, columnName, "soundVolume")]);

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
        item->screenSaverDuration_ = atoi(argv[Data::getIndexByName(argc, columnName, "screenSaverDuration")]);
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
		item->localNumber_ = argv[Data::getIndexByName(argc, columnName, "localNumber")];
	}
}
