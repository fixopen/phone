#ifndef __DATA_LOG_H__
#define __DATA_LOG_H__

#include "Data.h"
#include "../Util/DateTime.h"
#include "../Util/SmartPtr.h"

namespace Data
{
	enum LogLevel
	{
		llNull,
		llFailureError,
		llError,
		llWarning,
		llNotice,
		llInfo,
		llOther,
	};

	enum LogCode
	{
		ecNull,
		ecSystemError = 1,
		ecPowerFailure = 2,
		ecScreenError = 3,
		ecScreenControllerError = 4,
		ecSystemOverRun = 5,
		ecTemperature = 6,
		ecMoisture = 7,
		ecShake = 8,
		ecWaterLevel = 9,
		ecDeviceBox = 10,
		ecControlServerConnectionError = 11,
		ecWatchServerConnectionError = 12,
		ecFileDownloadError = 13,
		ecFileUploadError = 14,
		ecNetworkUnreached = 15,
		ecFileChecksumError = 16,
		//ecFileChecksumError = 16,
		ecDiskSpaceLow = 17,
		ecStorageFreeSpaceLow = 17, //5%
		ecOpenFileError = 18,
		ecDownloadFileInUsed = 18,
		ecLEDConnectionError = 19,
		ecLEDScreenConnectError = 19,
		ecSDCardRemoved = 20,
		ecFileNotFound = 21,
		ecPlayListError = 22,
		ecMediaFormatNotSupport = 23,
		ecRemoteUpgradeFailure = 24,
		ecFTPServerConnectError = 25,
		ecURLError = 26,
		ecUpgradeFailure = 27,

		ecDeviceStarted = 200,
		ecDeviceClosed = 201,
		ecCurrentSoftwareVersion = 202,
		ecLEDOpened = 203,
		ecLEDClosed = 204,

		//ecSendPacket,
		//ecReceivePacket,
		ecCheckSumError,
		ecIdError,
		ecReceiveDeviceControlPacket,
		ecReceiveFileOperatorPacket,
		ecReceiveStateRequestPacket,
		ecReceiveInsertTextAdPacket,
		ecReceiveTimesyncPacket,
		ecReceiveUpgradePacket,
		ecReceiveLayoutPacket,
		ecReceiveLoginResponsePacket,
		ecReceiveLogoutResponsePacket,
		ecReceiveHeartbeatResponsePacket,
		ecReceiveAlertResponsePacket,
		ecReceiveNoticeResponsePacket,
		ecReceiveFileOperatorFinishResponsePacket,
		ecReceiveTransportFinishResponsePacket,
		ecServerRequestFileList,
		ecServerRequestDeleteFile,
		ecPlayVideoFile,
		ecNotFoundVideoFile,
		ecPlayDefaultVideoFile,
		ecPlayImageFile,
		ecNotFoundImageFile,
		ecPlayDefaultImageFile,
		ecPlayTextItem,
		ecNotFoundTextItem,
		ecPlayDefaultTextItem,
		ecSendLoginPacket,
		ecSendLogoutPacket,
		ecSendHeartbeatPacket,
		ecSendAlertPacket,
		ecSendNoticePacket,
		ecResetSystemOk,
		ecResetSystemError,
		ecTelephoneUsedChanged,
		ecWaterLevelAlarm,
		ecScreenPowerChanged,
		ecModemPowerChanged,
		ecDoorStateChanged,
		ecScreenLockChanged,
		ecScreenDarkChanged,
		ecControlOfflineChanged,
		ecUnitPanelStateChanged,
		ecBrightnessModifyModeChanged,
		ecBrightnessChanged,
		ecBoxTempAlarm,
		ecScreenTempAlaram,
		ecError,
	};

	class Log : public DataAccess<Log>
	{
    public:
        LogLevel const level() const
        {
            return level_;
        }
        void level(LogLevel const level)
        {
            level_ = level;
        }
        std::string const who() const
        {
            return who_;
        }
        void who(std::string const who)
        {
            who_ = who;
        }
        Util::Timestamp const when() const
        {
            return when_;
        }
        void when(Util::Timestamp const when)
        {
            when_ = when;
        }
        std::string const what() const
        {
            return what_;
        }
        void what(std::string const what)
        {
            what_ = what;
        }
        LogCode const code() const
        {
            return code_;
        }
        void code(LogCode const code)
        {
            code_ = code;
        }
        double value(void) const
        {
            return value_;
        }
        void value(double const value)
        {
            value_ = value;
        }
	private:
		//int id_;
		LogLevel level_;
		std::string who_;
		Util::Timestamp when_;
		std::string what_;
		LogCode code_;
		double value_;
	public:
		Log(); //create
		//~Log(); //destroy
        static std::vector<Util::shared_ptr<Log> > GetFromDatabase(std::string const& filter, std::string const& orderFieldName = "id", SeqenceRelation const dir = srNull, int const offset = 0, int const pageSize = -1); //get from database
		void Update() const; //sync to database
		void Insert(); //insert new instance to database
		static void Remove(std::string filter); //delete form database
		std::string ToString(void);
	private:
		static void modifyFieldByDB_(int argc, char** argv, char** columnName, Util::shared_ptr<Log> item);
	};
}

#endif //__DATA_LOG_H__