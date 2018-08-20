#ifndef __UTIL_LOG_H__
#define __UTIL_LOG_H__

#include <vector>
#include <string>
#include "StringOp.h"

namespace Util {
    namespace Log {
        struct Entry;
        class Processor;
            class Source;
            class Sink;
            class Filter;
            //class Forker;
            //class Decorate;
                class TimeDecorate;
        class Receiver;
            class NetworkReceiver;
        class Formatter;
            class SQLFormatter;
        class Parser;
            class SQLParser;
        class Destination;
            class FileDestination;
            class MemoryDestination;
            class NetworkDestination;
            class DatabaseDestination;

        int const AddSource(Source* const processor);
        int const AddProcess(Processor* const processor, int const previousNode);
        Processor* const RemoveProcess(int const id);
        //int const AddSource(Source* const source, Parser* const parser, Receiver* const receiver);
        //int const AddSink(Sink* const sink, Formatter* const formatter, Destination* const destination);
        void Log(Entry const& entry);
        void StartLog();
        Processor* const FindNodeById(Processor* const node, int const id);

        struct Entry {
            Entry(std::string const& w)
            : level(lNotice)
            , what(w) {
            }

            Entry(char const* const w)
            : level(lNotice)
            , what(w) {
            }

            Entry(wchar_t const* const w)
            : level(lNotice)
            , what(StringOp::FromUnicode(w)) {
            }

            enum Level {
                lFatalError,
                lError,
                lWarning,
                lNotice,
                lInfo,
            };
            Level level;
            std::string what;
        };
        class Processor {
            friend int const AddSource(Source* const processor);
            friend int const AddProcess(Processor* const processor, int const previousNode);
            friend Processor* const RemoveProcess(int const id);
            //friend int const AddSource(Source* const source, Parser* const parser, Receiver* const receiver);
            //friend int const AddSink(Sink* const sink, Formatter* const formatter, Destination* const destination);
            friend Processor* const FindNodeById(Processor* const node, int const id);
        public:
            virtual Entry const Process(Entry const& entry);
            void AddNext(Processor* const next);
            void RemoveNext(Processor* const next);
            virtual ~Processor();
        protected:
            std::vector<Processor*> nexts_;
            int id_;
        };
        class Source : public Processor {
        public:
            virtual Entry const Process(Entry const& entry);
            virtual void SetParser(Parser* const parser);
            virtual void SetReceiver(Receiver* const receiver);
            virtual ~Source();
        private:
            Parser* parser_;
            Receiver* receiver_;
        };
        class Sink : public Processor {
        public:
            virtual Entry const Process(Entry const& entry);
            virtual void SetFormatter(Formatter* const formatter);
            virtual void SetDestination(Destination* const destination);
            virtual ~Sink();
        private:
            Formatter* formatter_;
            Destination* destination_;
        };
        class Filter : public Processor {
        public:
            virtual bool const IsAccept(Entry const& entry);
            virtual void SetLevel(Entry::Level const level);
            virtual ~Filter();
        private:
            Entry::Level level_;
        };
        //class Forker : public Processor { //same as processor
        //public:
        //    virtual ~Forker();
        //};
        class Receiver {
        public:
            virtual std::string const Read();
        };
        class NetworkReceiver : public Receiver {
        public:
            NetworkReceiver(std::string const& ip, unsigned short const port);
            virtual std::string const Read();
        private:
            typedef int Socket;
            Socket socket_;
        };
        class Formatter {
        public:
            virtual std::string const Format(Entry const& entry);
        };
        class SQLFormatter : public Formatter {
        public:
            virtual std::string const Format(Entry const& entry);
        };
        class Parser {
        public:
            virtual Entry const Parse(std::string const& value);
        };
        class SQLParser : public Parser {
        public:
            virtual Entry const Parse(std::string const& sql);
        };
        class Destination {
        public:
            virtual void Write(std::string const& value);
        };
        class FileDestination : public Destination {
        public:
            FileDestination(std::string const& filename);
            virtual void Write(std::string const& value);
        private:
            FILE* file_;
        };
        class MemoryDestination : public Destination {
        public:
            MemoryDestination(size_t count);
            virtual void Write(std::string const& value);
        private:
            std::vector<std::string> contents_;
        };
        class NetworkDestination : public Destination {
        public:
            NetworkDestination(std::string const& ip, unsigned short const port);
            virtual void Write(std::string const& value);
        private:
            typedef int Socket;
            Socket* socket_;
        };
        class DatabaseDestination : public Destination {
        public:
            DatabaseDestination(std::string const& databaseName, std::string const& tableName);
            virtual void Write(std::string const& value);
        private:
            typedef int DB;
            DB* database_;
        };
    }
#if 0
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
#endif
}

#endif //__UTIL_LOG_H__
