#ifndef __SMS_DATA_VALUEADDSERVICE_H__
#define __SMS_DATA_VALUEADDSERVICE_H__

#include "../../Util/DateTime.h"
#include "../../Util/Data.h"

namespace SMS {
    namespace Data {
        std::string const TrimF(std::string const& v);
        std::string const Reverse(std::string const& v);
        std::string const PaddingAndReverse(std::string const& v);
        struct AddressType {
            enum NumberType {
                tUnknown = 0, //000
                tInternationNumber = 1, //001
                tNationNumber = 2, //010
                tNetworkSpecNumber = 3, //011
                tSubscribeNumber = 4, //100
                tAlphaNumeric = 5, //101 TS03.38 7-bit default alphabet
                tAbbreviatedNumber = 6, //110
                tReserved = 7, //111
            };
            enum Indentification {
                iUnknown = 0, //0000
                iIsdnTelephoneNumber = 1, //0001 E.164/E.163
                iDataNumber = 3, //0011 X.121
                iTelexNumber = 4, //0100
                iNationNumber = 8, //1000
                iPrivateNumber = 9, //1001
                iERMESNumber = 10, //1010 ETSI (DE/PS301-1)
                iReserved = 15, //1111
            };
            AddressType();
            AddressType(unsigned char v);
            unsigned char const ToString() const;
            bool mustSet;
            NumberType typeOfNumber;
            Indentification indentification;
        };
        struct Endpoint {
            std::string const ToString() const;
            size_t addressLength;
            AddressType addressType;
            std::string address;
        };
        struct Flag{
            enum Type {
                tDeliver = 0, //00 SC ==> MS
                tDeliverReport = 0, //00 MS ==> SC
                tSubmit = 1, //01 MS ==> SC
                tSubmitReport = 1, //01 SC ==> MS
                tStatusReport = 2, //10 SC == MS
                tCommand = 2, //10 MS ==> SC
                tReserved = 3, //11
            };
            enum ValidityPeriodFormat {
                pNotPresent = 0, //00
                pReserved = 1, //01
                pRelativePresent = 2, //10 integer
                pAbsolutePresent = 3, //11 semi-octect
            };
            Flag(Type const atype = tDeliver);
            Flag(unsigned char v);
            unsigned char const ToString() const;
            bool isSetReplyPath; //RP
            bool hasHeaderInfo; //UDHI   EMS include header info, text not include
            bool requestReport; //SRR
            ValidityPeriodFormat validityPeriodFormat; //VPF
            bool rejectCopy; //RD MMS
            Type type; //MTI
        };
        struct Encoding {
            enum EncodingCode {
                DefaultAlphabet = 0, //00
                OctetBit = 1, //01
                UnicodeCodeSet = 2, //10
                Reserved = 3, //11 reserved
            };
            Encoding(EncodingCode aencoding = UnicodeCodeSet);
            Encoding(unsigned char v);
            unsigned char const ToString() const;
            //unsigned char high : 4; //reserved
            EncodingCode encoding;// : 2;
            //unsigned char low : 2; //reserved
        };
        struct SMSCTimestamp {
            SMSCTimestamp(std::string const& v = "");
            std::string const ToString() const;
            unsigned char year;
            unsigned char month;
            unsigned char day;
            unsigned char hour;
            unsigned char minite;
            unsigned char second;
            unsigned char timezone;
        };
        enum Type {
            tMenuManager = 0x22B7,
            tMultimediaDownload = 0x22B8,
            tSoftwareDownload = 0x22B9,
            tWeather = 0x2328,
        };
        //JTXX
        struct Header { //Header
            enum Result {
                rSuccess, //0
                rFailure, //1
            };
            std::string opCode; //OperCode
            std::string version; //Version
            Util::Timestamp timestamp; //TimeStamp
            Result result; //ResponseResult
        };

        namespace SimpleParser {
            std::string const FindContent(std::string const& content, size_t& pos, std::string const& tagName);
        }

        struct Register : Util::DataAccess<Register> { //REGISTER
            static std::vector<Util::shared_ptr<Register> > const Select(std::wstring const& filter);
            void Update() const; //sync to database
            void Insert(); //insert new instance to database
            void Remove() const; //delete self from database
            static void Remove(std::wstring const& filter); //delete from database
            static Register const Parse(std::string const& content);
            std::string const ToString() const;
            Header header;
            //Body
            std::string terminalVersion; //TerminalVersion
            std::string IMEI; //IMEI
            std::string terminalId; //TerminalId
        private:
            static void modifyFieldByDB_(int argc, char** argv, char** columnName, Util::shared_ptr<Register> item);
        };

        struct RegisterReply : Util::DataAccess<RegisterReply> { //REGISTER
            enum Result {
                rSuccess = 0,
                rFailure = 1,
            };
            static std::vector<Util::shared_ptr<RegisterReply> > const Select(std::wstring const& filter);
            void Update() const; //sync to database
            void Insert(); //insert new instance to database
            void Remove() const; //delete self from database
            static void Remove(std::wstring const& filter); //delete from database
            static RegisterReply const Parse(std::string const& content);
            Header header;
            //Body
            Result result; //Result
            std::wstring areaCode; //AreaCode
            std::wstring description; //Desc
        private:
            static void modifyFieldByDB_(int argc, char** argv, char** columnName, Util::shared_ptr<RegisterReply> item);
        };

        struct Weather : Util::DataAccess<Weather> { //WEATHERINFO
            static std::vector<Util::shared_ptr<Weather> > const Select(std::wstring const& filter);
            void Update() const; //sync to database
            void Insert(); //insert new instance to database
            void Remove() const; //delete self from database
            static void Remove(std::wstring const& filter); //delete from database
            static std::vector<Weather> const Parse(std::string const& content);
            struct WeatherInfo { //WeatherInfo
                std::wstring area; //Area
                std::wstring wind; //Wind
                std::wstring temperature; //TemperaTure
                int code; //WeatherCode
				std::string date; //ValidTime
                std::wstring description; //OtherInfo
            };
            Header header;
            //Body
            WeatherInfo weatherInfos; //std::vector<>
        private:
            static void modifyFieldByDB_(int argc, char** argv, char** columnName, Util::shared_ptr<Weather> item);
        };

		struct WeatherPicture : Util::DataAccess<WeatherPicture> {
			static std::vector<Util::shared_ptr<WeatherPicture> > const Select(std::wstring const& filter);
			void Update() const; //sync to database
			void Insert(); //insert new instance to database
			void Remove() const; //delete self from database
			static void Remove(std::wstring const& filter); //delete from database
			std::wstring filename; //PictureName
			std::string content; //Content
			std::wstring item; //item
		private:
			static void modifyFieldByDB_(int argc, char** argv, char** columnName, Util::shared_ptr<WeatherPicture> item);
		};

        struct SoftwareDownload : Util::DataAccess<SoftwareDownload> { //SOFTWARELOAD
            struct SoftInfo { //SoftInfo
                std::wstring name; //SoftWareName
                std::wstring filename; //SoftWareFileName
                std::string content; //SoftWareMaterialContent
            };
            static std::vector<Util::shared_ptr<SoftwareDownload> > const Select(std::wstring const& filter);
            void Update() const; //sync to database
            void Insert(); //insert new instance to database
            void Remove() const; //delete self from database
            static void Remove(std::wstring const& filter); //delete from database
            static std::vector<SoftwareDownload> const Parse(std::string const& content);
            Header header;
            //Body
            SoftInfo softInfos; //std::vector<>
        private:
            static void modifyFieldByDB_(int argc, char** argv, char** columnName, Util::shared_ptr<SoftwareDownload> item);
        };

        struct MultimediaDownload : Util::DataAccess<MultimediaDownload> { //FILELOAD
            struct MultimediaInfo { //MultimediaInfo
                enum Type {
                    tImage = 1,
                    tMultimedia = 2,
                    tText = 3,
                    tMMS = 4,
                    tWeather = 5,
                };
                Type type; //FileType
                std::string spCode; //SpCode
                std::wstring subject; //Subject
                std::wstring filename; //FileName
                int weatherCode; //WeatherCode
                std::string content; //FileMeterialContent
            };
            static std::vector<Util::shared_ptr<MultimediaDownload> > const Select(std::wstring const& filter);
            void Update() const; //sync to database
            void Insert(); //insert new instance to database
            void Remove() const; //delete self from database
            static void Remove(std::wstring const& filter); //delete from database
            static std::vector<MultimediaDownload> const Parse(std::string const& content);
            Header header;
            //Body
            MultimediaInfo multimediaInfos; //std::vector<>
        private:
            static void modifyFieldByDB_(int argc, char** argv, char** columnName, Util::shared_ptr<MultimediaDownload> item);
        };

        struct MenuManager : Util::DataAccess<MenuManager> { //MENU
            enum MenuOperator {
                oNew = 1,
                oDelete = 2,
                oUpdate = 3,
                oRenew = 4,
            };
            struct MenuInfo { //MenuInfo
                enum Type {
                    tText = 1,
                    tImage = 2,
                    tMultimedia = 3,
                    tSoftware = 4,
                    tUpdate = 5,
                    tWebsite = 6,
                    tRegister = 7,
                };
                int menuLevel; //MenuLevel
                int parentId; //ParentId
                int menuId; //MenuID
                Type type; //Menu
                int showOrder; //ShowOrder
                std::string spCode; //SpCode
                int pageCount; //ShowNum
                std::wstring imageName; //ImageNormalName
                std::string imageContent; //ImageNormalContent
                std::wstring specImageName; //ImageSpecName
                std::string specImageContent; //ImageSpecContent
                std::string url; //Url
                std::wstring menuName; //MenuName
            };
            static std::vector<Util::shared_ptr<MenuManager> > const Select(std::wstring const& filter);
            void Update() const; //sync to database
            void Insert(); //insert new instance to database
            void Remove() const; //delete self from database
            static void Remove(std::wstring const& filter); //delete from database
            static std::vector<MenuManager> const Parse(std::string const& content);
            Header header;
            //Body
            MenuOperator opType; //OperType
            MenuInfo menuInfos; //std::vector<>
        private:
            static void modifyFieldByDB_(int argc, char** argv, char** columnName, Util::shared_ptr<MenuManager> item);
        };
    }
}

#endif //__SMS_DATA_VALUEADDSERVICE_H__
