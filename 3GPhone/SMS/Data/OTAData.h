#ifndef __SMS_DATA_VALUEADDSERVICE_H__
#define __SMS_DATA_VALUEADDSERVICE_H__

#include "../../Util/DateTime.h"
#include "../../Util/Data.h"

namespace SMS {
    namespace Data {
        enum Type {
            tMenuManager = 0x22B7,
            tMultimediaDownload = 0x22B8,
            tSoftwareDownload = 0x22B9,
            tWeather = 0x2328,
        };
        //JTXX
        struct Header { //Header
            std::string opCode; //OperCode
            std::string version; //Version
            Util::Timestamp timestamp; //TimeStamp
        };

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
            std::string areaCode; //AreaCode
            std::string description; //Desc
        private:
            static void modifyFieldByDB_(int argc, char** argv, char** columnName, Util::shared_ptr<RegisterReply> item);
        };

        struct Weather : Util::DataAccess<Weather> { //WEATHERINFO
            static std::vector<Util::shared_ptr<Weather> > const Select(std::wstring const& filter);
            void Update() const; //sync to database
            void Insert(); //insert new instance to database
            void Remove() const; //delete self from database
            static void Remove(std::wstring const& filter); //delete from database
            static Weather const Parse(std::string const& content);
            struct WeatherInfo { //WeatherInfo
                std::string area; //Area
                std::string wind; //Wind
                std::string temperature; //TemperaTure
                int code; //WeatherCode
                std::string description; //OtherInfo
            };
            Header header;
            //Body
            std::vector<WeatherInfo> weatherInfos;
        private:
            static void modifyFieldByDB_(int argc, char** argv, char** columnName, Util::shared_ptr<Weather> item);
        };

        struct SoftwareDownload : Util::DataAccess<SoftwareDownload> { //SOFTWARELOAD
            struct SoftInfo { //SoftInfo
                std::string name; //SoftWareName
                std::string filename; //SoftWareFileName
                std::string content; //SoftWareMaterialContent
            };
            static std::vector<Util::shared_ptr<SoftwareDownload> > const Select(std::wstring const& filter);
            void Update() const; //sync to database
            void Insert(); //insert new instance to database
            void Remove() const; //delete self from database
            static void Remove(std::wstring const& filter); //delete from database
            static SoftwareDownload const Parse(std::string const& content);
            Header header;
            //Body
            std::vector<SoftInfo> softInfos;
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
                std::string subject; //Subject
                std::string filename; //FileName
                int weatherCode; //WeatherCode
                std::string content; //FileMeterialContent
            };
            static std::vector<Util::shared_ptr<MultimediaDownload> > const Select(std::wstring const& filter);
            void Update() const; //sync to database
            void Insert(); //insert new instance to database
            void Remove() const; //delete self from database
            static void Remove(std::wstring const& filter); //delete from database
            static MultimediaDownload const Parse(std::string const& content);
            Header header;
            //Body
            std::vector<MultimediaInfo> multimediaInfos;
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
                std::string imageName; //ImageName
                std::string imageContent; //ImageMaterialContent
                std::string url; //Url
                std::string menuName; //MenuName
            };
            static std::vector<Util::shared_ptr<MenuManager> > const Select(std::wstring const& filter);
            void Update() const; //sync to database
            void Insert(); //insert new instance to database
            void Remove() const; //delete self from database
            static void Remove(std::wstring const& filter); //delete from database
            static MenuManager const Parse(std::string const& content);
            Header header;
            //Body
            MenuOperator opType; //OperType
            std::vector<MenuInfo> menuInfos;
        private:
            static void modifyFieldByDB_(int argc, char** argv, char** columnName, Util::shared_ptr<MenuManager> item);
        };
    }
}

#endif //__SMS_DATA_VALUEADDSERVICE_H__
