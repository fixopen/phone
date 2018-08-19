#include "OTAData.h"

namespace Util {
    std::wstring SMS::Data::Register::tableName_ = L"register";
    int SMS::Data::Register::count_ = 0;
    int SMS::Data::Register::offset_ = 0;

    std::wstring SMS::Data::RegisterReply::tableName_ = L"registerReply";
    int SMS::Data::RegisterReply::count_ = 0;
    int SMS::Data::RegisterReply::offset_ = 0;

    std::wstring SMS::Data::Weather::tableName_ = L"weather";
    int SMS::Data::Weather::count_ = 0;
    int SMS::Data::Weather::offset_ = 0;

    std::wstring SMS::Data::WeatherPicture::tableName_ = L"weatherPicture";
    int SMS::Data::WeatherPicture::count_ = 0;
    int SMS::Data::WeatherPicture::offset_ = 0;

    std::wstring SMS::Data::SoftwareDownload::tableName_ = L"softwareDownload";
    int SMS::Data::SoftwareDownload::count_ = 0;
    int SMS::Data::SoftwareDownload::offset_ = 0;

    std::wstring SMS::Data::MultimediaDownload::tableName_ = L"multimediaDownload";
    int SMS::Data::MultimediaDownload::count_ = 0;
    int SMS::Data::MultimediaDownload::offset_ = 0;

    std::wstring SMS::Data::MenuManager::tableName_ = L"menuManager";
    int SMS::Data::MenuManager::count_ = 0;
    int SMS::Data::MenuManager::offset_ = 0;
}

namespace SMS {
    namespace Data {
        std::string const TrimF(std::string const& v) {
            std::string result = v;
            if (v[v.length() -1] == 'F') {
                result = v.substr(0, v.length() - 1);
            }
            return result;
        }

        std::string const Reverse(std::string const& v) {
            std::string result;
            for (size_t i = 0; i < v.length(); i += 2) {
                result += v[i + 1];
                result += v[i];
            }
            return result;
        }

        std::string const PaddingAndReverse(std::string const& v) {
            std::string r = v;
            if (v.length() % 2) {
                r += 'F';
            }
            return Reverse(r);
        }

        AddressType::AddressType()
            : mustSet(true)
            , typeOfNumber(tInternationNumber)
            , indentification(iIsdnTelephoneNumber) {
        }

        AddressType::AddressType(unsigned char v)
            : mustSet(true)
            , typeOfNumber(tInternationNumber)
            , indentification(iIsdnTelephoneNumber) {
                typeOfNumber = static_cast<NumberType>(v << 1 >> 4);
                indentification = static_cast<Indentification>(v << 4 >> 4);
        }

        unsigned char const AddressType::ToString() const {
            unsigned char result = 0;
            result |= (mustSet << 7) | (typeOfNumber << 3) | (indentification << 0);
            return result;
        }

        std::string const Endpoint::ToString() const {
            std::string result;
            char l[3] = {0};
            sprintf(l, "%02X", addressLength);
            result += Reverse(l);
            char c = addressType.ToString();
            sprintf(l, "%02X", c);
            result += l;
            result += PaddingAndReverse(address);
            return result;
        }

        Flag::Flag(Type const atype)
            : isSetReplyPath(1)
            , hasHeaderInfo(0)
            , requestReport(1)
            , validityPeriodFormat(pRelativePresent)
            , rejectCopy(0)
            , type(atype) {
        }

        Flag::Flag(unsigned char v) {
            isSetReplyPath = v & 0x80;
            hasHeaderInfo = v & 0x40;
            requestReport = v & 0x20;
            validityPeriodFormat = static_cast<ValidityPeriodFormat>(v << 2 >> 5);
            rejectCopy = v & 0x04;
            type = static_cast<Type>(v << 6 >> 6);
        }

        unsigned char const Flag::ToString() const {
            unsigned char result = 0;
            result |= (isSetReplyPath << 7)
                | (hasHeaderInfo << 6)
                | (requestReport << 5)
                | (validityPeriodFormat << 3)
                | (rejectCopy << 2)
                | (type << 0);
            return result;
        }

        Encoding::Encoding(EncodingCode aencoding)
        //: high(0)
        : encoding(aencoding) {
        //, low(0) {
        }

        Encoding::Encoding(unsigned char v) {
            encoding = static_cast<EncodingCode>(v << 4 >> 2);
        }

        unsigned char const Encoding::ToString() const {
            unsigned char result = 0;
            result |= (0 << 4)
                | (encoding << 2)
                | (0 << 0);
            return result;
        }

        SMSCTimestamp::SMSCTimestamp(std::string const& v)
            : timezone(0) {
                if (v != "") {
                    year = Util::StringOp::ToInt(Reverse(v.substr(0, 2)));
                    month = Util::StringOp::ToInt(Reverse(v.substr(2, 2)));
                    day = Util::StringOp::ToInt(Reverse(v.substr(4, 2)));
                    hour = Util::StringOp::ToInt(Reverse(v.substr(6, 2)));
                    minite = Util::StringOp::ToInt(Reverse(v.substr(8, 2)));
                    second = Util::StringOp::ToInt(Reverse(v.substr(10, 2)));
                    timezone = Util::StringOp::ToInt(Reverse(v.substr(12, 2)));
                }
        }

        std::string const SMSCTimestamp::ToString() const {
            std::string result;
            char l[3] = {0};
            sprintf(l, "%02X", Util::StringOp::FromInt(year));
            result += Reverse(l);
            sprintf(l, "%02X", Util::StringOp::FromInt(month));
            result += Reverse(l);
            sprintf(l, "%02X", Util::StringOp::FromInt(day));
            result += Reverse(l);
            sprintf(l, "%02X", Util::StringOp::FromInt(hour));
            result += Reverse(l);
            sprintf(l, "%02X", Util::StringOp::FromInt(minite));
            result += Reverse(l);
            sprintf(l, "%02X", Util::StringOp::FromInt(second));
            result += Reverse(l);
            sprintf(l, "%02X", Util::StringOp::FromInt(timezone));
            result += Reverse(l);
            return result;
        }

        namespace SimpleParser {
            std::string const FindContent(std::string const& content, size_t& pos, std::string const& tagName) {
                size_t beginFindPos = content.find("<" + tagName + ">", pos);
                beginFindPos += tagName.length() + 2;
                pos = beginFindPos + 1;
                size_t endFindPos = content.find("</" + tagName + ">", pos);
                return content.substr(beginFindPos, endFindPos - beginFindPos);
            }
        }

        std::vector<Util::shared_ptr<Register> > const Register::Select(std::wstring const& filter) {
            return GetDatasByFilter(filter, modifyFieldByDB_);
        }

        void Register::Update() const {
            std::wstring cmd = L"UPDATE ";
            cmd += tableName_;
            cmd += L" SET [version] = '";
            cmd += Util::StringOp::FromUTF8(header.version);
            cmd += L"', [timestamp] = '";
            cmd += Util::StringOp::FromTimestamp(header.timestamp);
            cmd += L"', [terminalVersion] = '";
            cmd += Util::StringOp::FromUTF8(terminalVersion);
            cmd += L"', [imei] = '";
            cmd += Util::StringOp::FromUTF8(IMEI);
            cmd += L"', [terminalCode] = '";
            cmd += Util::StringOp::FromUTF8(terminalId);//对应项不一样
            cmd += L"' WHERE id = ";
            cmd += Util::StringOp::FromInt(id());
            ExecCommand(cmd);
        }

        void Register::Insert() {
            std::wstring cmd = L"INSERT INTO ";
            cmd += tableName_;
            cmd += L" ( version, timestamp, terminalVersion, imei, terminalCode) VALUES ( '";
            cmd += Util::StringOp::FromUTF8(header.version);
            cmd += L"', '";
            cmd += Util::StringOp::FromTimestamp(header.timestamp);
            cmd += L"', '";
            cmd += Util::StringOp::FromUTF8(terminalVersion);
            cmd += L"', '";
            cmd += Util::StringOp::FromUTF8(IMEI);
            cmd += L"', '";
            cmd += Util::StringOp::FromUTF8(terminalId);
            cmd += L"' )";
            ExecCommand(cmd);
            id(GetCurrentId());
        }

        void Register::Remove() const {
            std::wstring filter = L"id = " + Util::StringOp::FromInt(id());
            Remove(filter);
        }

        void Register::Remove(std::wstring const& filter) {
            RemoveDatasByFilter(filter);
        }

        Register const Register::Parse(std::string const& content) {
            Register result;
            return result;
        }

        std::string const Register::ToString() const {
            std::string result;
            return result;
        }

        void Register::modifyFieldByDB_(int argc, char** argv, char** columnName, Util::shared_ptr<Register> item) {
            item->id(atoi(argv[GetIndexByName(argc, columnName, "id")]));
            item->header.version		=	argv[GetIndexByName(argc, columnName, "version")];
            item->header.timestamp		=	Util::StringOp::ToTimestamp(std::string(argv[GetIndexByName(argc, columnName, "timestamp")]));
            item->terminalVersion		=	argv[GetIndexByName(argc, columnName, "terminalVersion")];
            item->IMEI					=	argv[GetIndexByName(argc, columnName, "imei")];
            item->terminalId			=	argv[GetIndexByName(argc, columnName, "terminalCode")];
        }

        std::vector<Util::shared_ptr<RegisterReply> > const RegisterReply::Select(std::wstring const& filter) {
            return GetDatasByFilter(filter, modifyFieldByDB_);
        }

        void RegisterReply::Update() const {
            std::wstring cmd = L"UPDATE ";
            cmd += tableName_;
            cmd += L" SET [version] = '";
            cmd += Util::StringOp::FromUTF8(header.version);
            cmd += L"', [responseResult] = '";
            cmd += Util::StringOp::FromInt(header.result);
            cmd += L"', [timestamp] = '";
            cmd += Util::StringOp::FromTimestamp(header.timestamp);
            cmd += L"', [result] = ";
            cmd += Util::StringOp::FromInt(result);
            cmd += L", [area] = '";
            cmd += areaCode;
            cmd += L"', [description] = '";
            cmd += description;
            cmd += L"' WHERE id = ";
            cmd += Util::StringOp::FromInt(id());
            ExecCommand(cmd);
        }

        void RegisterReply::Insert() {
            std::wstring cmd = L"INSERT INTO ";
            cmd += tableName_;
            cmd += L" ( version, responseResult, timestamp, result, area, description ) VALUES ( '";
            cmd += Util::StringOp::FromUTF8(header.version);
            cmd += L"', '";
            cmd += Util::StringOp::FromInt(header.result);
            cmd += L"', '";
            cmd += Util::StringOp::FromTimestamp(header.timestamp);
            cmd += L"', ";
            cmd += Util::StringOp::FromInt(result);
            cmd += L", '";
            cmd += areaCode;
            cmd += L"', '";
            cmd += description;
            cmd += L"' )";
            ExecCommand(cmd);
            id(GetCurrentId());
        }

        void RegisterReply::Remove() const {
            std::wstring filter = L"id = " + Util::StringOp::FromInt(id());
            Remove(filter);
        }

        void RegisterReply::Remove(std::wstring const& filter) {
            RemoveDatasByFilter(filter);
        }

        RegisterReply const RegisterReply::Parse(std::string const& content) {
            RegisterReply result;
            size_t startPos = 0;
            result.header.opCode = SimpleParser::FindContent(content, startPos, "OperCode");
            result.header.version = SimpleParser::FindContent(content, startPos, "Version");
            result.header.result = static_cast<Header::Result>(atoi(SimpleParser::FindContent(content, startPos, "ResponseResult").c_str()));
            result.header.timestamp = Util::StringOp::ToTimestamp(SimpleParser::FindContent(content, startPos, "TimeStamp"));
            result.result = static_cast<Result>(Util::StringOp::ToInt(SimpleParser::FindContent(content, startPos, "Result")));
            result.areaCode = Util::StringOp::ToUnicode(SimpleParser::FindContent(content, startPos, "AreaCode"));
            result.description = Util::StringOp::ToUnicode(SimpleParser::FindContent(content, startPos, "Desc"));
            return result;
        }

        void RegisterReply::modifyFieldByDB_(int argc, char** argv, char** columnName, Util::shared_ptr<RegisterReply> item) {
            item->id(atoi(argv[GetIndexByName(argc, columnName, "id")]));
            item->header.version		=	argv[GetIndexByName(argc, columnName, "version")];
            item->header.result = static_cast<Header::Result>(atoi(argv[GetIndexByName(argc, columnName, "responseResult")]));
            item->header.timestamp		=	Util::StringOp::ToTimestamp(std::string(argv[GetIndexByName(argc, columnName, "timestamp")]));
            item->result				=	static_cast<Result>(atoi(argv[GetIndexByName(argc, columnName, "result")]));
            item->areaCode				=	Util::StringOp::FromUTF8(argv[GetIndexByName(argc, columnName, "area")]);
            item->description			=	Util::StringOp::FromUTF8(argv[GetIndexByName(argc, columnName, "description")]);

        }

        std::vector<Util::shared_ptr<Weather> > const Weather::Select(std::wstring const& filter) {
            return GetDatasByFilter(filter, modifyFieldByDB_);
        }

        void Weather::Update() const {         
            std::wstring cmd = L"UPDATE ";
            cmd += tableName_ ;
            cmd += L" SET [version] = '";
            cmd += Util::StringOp::FromUTF8(header.version);
            cmd += L"', [responseResult] = '";
            cmd += Util::StringOp::FromInt(header.result);
            cmd += L"', [timestamp] = '";
            cmd += Util::StringOp::FromTimestamp(header.timestamp);
            cmd += L"', [area] = '";
            cmd += weatherInfos.area;
            cmd += L"', [wind] = '";
            cmd += weatherInfos.wind;
            cmd += L"', [temperature] = '";
            cmd += weatherInfos.temperature;
            cmd += L"', [code] = ";
            cmd += Util::StringOp::FromInt(weatherInfos.code);
            cmd += L", [date] = '";
            cmd += Util::StringOp::FromUTF8(weatherInfos.date);
            cmd += L"', [description] = '";
            cmd += weatherInfos.description;
            cmd += L"' WHERE id = ";
            cmd += Util::StringOp::FromInt(id());
            ExecCommand(cmd);

        }

        void Weather::Insert() {
            std::wstring cmd = L"INSERT INTO ";
            cmd += tableName_;
            cmd += L" ( version, responseResult, timestamp, area, wind, temperature, code, date, description ) VALUES ( '";
            cmd += Util::StringOp::FromUTF8(header.version);
            cmd += L"', '";
            cmd += Util::StringOp::FromInt(header.result);
            cmd += L"', '";
            cmd += Util::StringOp::FromTimestamp(header.timestamp);
            cmd += L"', '";
            cmd += weatherInfos.area;
            cmd += L"', '";
            cmd += weatherInfos.wind;
            cmd += L"', '";
            cmd += weatherInfos.temperature;
            cmd += L"', ";
            cmd += Util::StringOp::FromInt(weatherInfos.code);
            cmd += L", '";
            cmd += Util::StringOp::FromUTF8(weatherInfos.date);
            cmd += L"', '";
            cmd += weatherInfos.description;
            cmd += L"' )";
            ExecCommand(cmd);
            id(GetCurrentId());
        }

        void Weather::Remove() const {
            std::wstring filter = L"id = " + Util::StringOp::FromInt(id());
            Remove(filter);
        }

        void Weather::Remove(std::wstring const& filter) {
            RemoveDatasByFilter(filter);
        }

        std::vector<Weather> const Weather::Parse(std::string const& content) {
            std::vector<Weather> result;
            size_t startPos = 0;
            std::string opCode = SimpleParser::FindContent(content, startPos, "OperCode");
            std::string version = SimpleParser::FindContent(content, startPos, "Version");
            Util::Timestamp timestamp = Util::StringOp::ToTimestamp(SimpleParser::FindContent(content, startPos, "TimeStamp"));
            Header::Result r = static_cast<Header::Result>(Util::StringOp::ToInt(SimpleParser::FindContent(content, startPos, "ResponseResult")));
            std::string info = SimpleParser::FindContent(content, startPos, "WeatherInfo");
            while(info != "") {
                Weather item;
                item.header.opCode = opCode;
                item.header.version = version;
                item.header.timestamp = timestamp;
                item.header.result = r;
                size_t pos = 0;
                item.weatherInfos.area = Util::StringOp::ToUnicode(SimpleParser::FindContent(info, pos, "Area"));
                item.weatherInfos.wind = Util::StringOp::ToUnicode(SimpleParser::FindContent(info, pos, "Wind"));
                item.weatherInfos.temperature = Util::StringOp::ToUnicode(SimpleParser::FindContent(info, pos, "TemperaTure"));
                item.weatherInfos.date = SimpleParser::FindContent(info, pos, "ValidTime");
                item.weatherInfos.code = Util::StringOp::ToInt(SimpleParser::FindContent(info, pos, "WeatherCode"));
                item.weatherInfos.description = Util::StringOp::ToUnicode(SimpleParser::FindContent(info, pos, "OtherInfo"));
                result.push_back(item);
                info = SimpleParser::FindContent(content, startPos, "WeatherInfo");
            }
            return result;
        }

        void Weather::modifyFieldByDB_(int argc, char** argv, char** columnName, Util::shared_ptr<Weather> item) {
            item->id(atoi(argv[GetIndexByName(argc, columnName, "id")]));		
            item->header.version			=	argv[GetIndexByName(argc, columnName, "version")];
            item->header.result = static_cast<Header::Result>(atoi(argv[GetIndexByName(argc, columnName, "responseResult")]));
            item->header.timestamp			=	Util::StringOp::ToTimestamp(std::string(argv[GetIndexByName(argc, columnName, "timestamp")]));
            item->weatherInfos.area			=	Util::StringOp::FromUTF8(argv[GetIndexByName(argc, columnName, "area")]);
            item->weatherInfos.wind			=	Util::StringOp::FromUTF8(argv[GetIndexByName(argc, columnName, "wind")]);
            item->weatherInfos.temperature	=	Util::StringOp::FromUTF8(argv[GetIndexByName(argc, columnName, "temperature")]);
            item->weatherInfos.code			=	atoi(argv[GetIndexByName(argc, columnName, "code")]);
            item->weatherInfos.description	=	Util::StringOp::FromUTF8(argv[GetIndexByName(argc, columnName, "description")]);
            item->weatherInfos.date			=	argv[GetIndexByName(argc, columnName, "date")];
        }

        std::vector<Util::shared_ptr<WeatherPicture> > const WeatherPicture::Select(std::wstring const& filter) {
            return GetDatasByFilter(filter, modifyFieldByDB_);
        }

        void WeatherPicture::Update() const {         
            std::wstring cmd = L"UPDATE ";
            cmd += tableName_ ;
            cmd += L" SET [filename] = '";
            cmd += filename;
            cmd += L"', [content] = '";
            cmd += L""; //Util::StringOp::ToUnicode(content); //not save it
            cmd += L"', [item] = '";
            cmd += item;
            cmd += L"' WHERE id = ";
            cmd += Util::StringOp::FromInt(id());
            ExecCommand(cmd);
        }

        void WeatherPicture::Insert() {
            std::wstring cmd = L"INSERT INTO ";
            cmd += tableName_;
            cmd += L" ( filename, content, item ) VALUES ( '";
            cmd += filename;
            cmd += L"', '";
            cmd += L""; //Util::StringOp::ToUnicode(content); //not save it
            cmd += L"', '";
            cmd += item;
            cmd += L"' )";
            ExecCommand(cmd);
            id(GetCurrentId());
        }

        void WeatherPicture::Remove() const {
            std::wstring filter = L"id = " + Util::StringOp::FromInt(id());
            Remove(filter);
        }

        void WeatherPicture::Remove(std::wstring const& filter) {
            RemoveDatasByFilter(filter);
        }

        void WeatherPicture::modifyFieldByDB_(int argc, char** argv, char** columnName, Util::shared_ptr<WeatherPicture> item) {
            item->id(atoi(argv[GetIndexByName(argc, columnName, "id")]));
            item->filename              =   Util::StringOp::FromUTF8(argv[GetIndexByName(argc, columnName, "filename")]);
            item->content				=   ""; //Util::StringOp::FromUTF8(argv[GetIndexByName(argc, columnName, "content")]);
            item->item					=   Util::StringOp::FromUTF8(argv[GetIndexByName(argc, columnName, "item")]);
        }	

        std::vector<Util::shared_ptr<SoftwareDownload> > const SoftwareDownload::Select(std::wstring const& filter) {
            return GetDatasByFilter(filter, modifyFieldByDB_);
        }

        void SoftwareDownload::Update() const {
            std::wstring cmd = L"UPDATE ";
            cmd += tableName_;
            cmd += L" SET [version] = '";
            cmd += Util::StringOp::FromUTF8(header.version);
            cmd += L"', [responseResult] = '";
            cmd += Util::StringOp::FromInt(header.result);
            cmd += L"', [timestamp] = '";
            cmd += Util::StringOp::FromTimestamp(header.timestamp);
            cmd += L"', [name] = '";
            cmd += softInfos.name;
            cmd += L"', [filename] = '";
            cmd += softInfos.filename;
            cmd += L"', [content] = '";
            cmd += L""; //Util::StringOp::ToUnicode(softInfos.content); //not save it
            cmd += L"' WHERE id = ";
            cmd += Util::StringOp::FromInt(id());
            ExecCommand(cmd);
        }

        void SoftwareDownload::Insert() {
            std::wstring cmd = L"INSERT INTO ";
            cmd += tableName_;
            cmd += L" ( version, responseResult, timestamp, name, filename, content) VALUES ( '";
            cmd += Util::StringOp::FromUTF8(header.version);
            cmd += L"', '";
            cmd += Util::StringOp::FromInt(header.result);
            cmd += L"', '";
            cmd += Util::StringOp::FromTimestamp(header.timestamp);
            cmd += L"', '";
            cmd += softInfos.name;
            cmd += L"', '";
            cmd += softInfos.filename;
            cmd += L"', '";
            cmd += L""; //Util::StringOp::ToUnicode(softInfos.content); //not save it
            cmd += L"' )";
            ExecCommand(cmd);
            id(GetCurrentId());
        }

        void SoftwareDownload::Remove() const {
            std::wstring filter = L"id = " + Util::StringOp::FromInt(id());
            Remove(filter);
        }

        void SoftwareDownload::Remove(std::wstring const& filter) {
            RemoveDatasByFilter(filter);
        }

        std::vector<SoftwareDownload> const SoftwareDownload::Parse(std::string const& content) {
            std::vector<SoftwareDownload> result;
            size_t startPos = 0;
            std::string opCode = SimpleParser::FindContent(content, startPos, "OperCode");
            std::string version = SimpleParser::FindContent(content, startPos, "Version");
            Util::Timestamp timestamp = Util::StringOp::ToTimestamp(SimpleParser::FindContent(content, startPos, "TimeStamp"));
            Header::Result r = static_cast<Header::Result>(Util::StringOp::ToInt(SimpleParser::FindContent(content, startPos, "ResponseResult")));
            std::string info = SimpleParser::FindContent(content, startPos, "WeatherInfo");
            while(info != "") {
                SoftwareDownload item;
                item.header.opCode = opCode;
                item.header.version = version;
                item.header.timestamp = timestamp;
                item.header.result = r;
                size_t pos = 0;
                item.softInfos.name = Util::StringOp::ToUnicode(SimpleParser::FindContent(info, pos, "SoftWareName"));
                item.softInfos.filename = Util::StringOp::ToUnicode(SimpleParser::FindContent(info, pos, "SoftWareFileName"));
                item.softInfos.content = SimpleParser::FindContent(info, pos, "SoftWareMaterialContent");
                result.push_back(item);
                info = SimpleParser::FindContent(content, startPos, "WeatherInfo");
            }
            return result;
        }

        void SoftwareDownload::modifyFieldByDB_(int argc, char** argv, char** columnName, Util::shared_ptr<SoftwareDownload> item) {
            item->id(atoi(argv[GetIndexByName(argc, columnName, "id")]));
            item->header.version		=	argv[GetIndexByName(argc, columnName, "version")];
            item->header.result = static_cast<Header::Result>(atoi(argv[GetIndexByName(argc, columnName, "responseResult")]));
            item->header.timestamp		=	Util::StringOp::ToTimestamp(std::string(argv[GetIndexByName(argc, columnName, "timestamp")]));
            item->softInfos.name					=	Util::StringOp::FromUTF8(argv[GetIndexByName(argc, columnName, "name")]);
            item->softInfos.filename				=	Util::StringOp::FromUTF8(argv[GetIndexByName(argc, columnName, "filename")]);
            item->softInfos.content				=	""; //argv[GetIndexByName(argc, columnName, "content")];
        }

        std::vector<Util::shared_ptr<MultimediaDownload> > const MultimediaDownload::Select(std::wstring const& filter) {
            return GetDatasByFilter(filter, modifyFieldByDB_);
        }

        void MultimediaDownload::Update() const {
            std::wstring cmd = L"UPDATE ";
            cmd += tableName_;
            cmd += L" SET [version] = '";
            cmd += Util::StringOp::FromUTF8(header.version);
            cmd += L"', [responseResult] = '";
            cmd += Util::StringOp::FromInt(header.result);
            cmd += L"', [timestamp] = '";
            cmd += Util::StringOp::FromTimestamp(header.timestamp);
            cmd += L"', [type] = ";
            cmd += Util::StringOp::FromInt(multimediaInfos.type);
            cmd += L", [spCode] = '";
            cmd += Util::StringOp::FromUTF8(multimediaInfos.spCode);
            cmd += L"', [subject] = '";
            cmd += multimediaInfos.subject;
            cmd += L"', [filename] = '";
            cmd += multimediaInfos.filename;
            cmd += L"', [weatherCode] = ";
            cmd += Util::StringOp::FromInt(multimediaInfos.weatherCode);
            cmd += L", [content] = '";
            cmd += L""; //Util::StringOp::ToUnicode(multimediaInfos.content); //not save it
            cmd += L"' WHERE id = ";
            cmd += Util::StringOp::FromInt(id());
            ExecCommand(cmd);
        }

        void MultimediaDownload::Insert() {
            std::wstring cmd = L"INSERT INTO ";
            cmd += tableName_;
            cmd += L" ( version, responseResult, timestamp, type, spCode, subject, filename, weatherCode, content ) VALUES ( '";
            cmd += Util::StringOp::FromUTF8(header.version);
            cmd += L"', '";
            cmd += Util::StringOp::FromInt(header.result);
            cmd += L"', '";
            cmd += Util::StringOp::FromTimestamp(header.timestamp);
            cmd += L"', ";
            cmd += Util::StringOp::FromInt(multimediaInfos.type);
            cmd += L", '";
            cmd += Util::StringOp::FromUTF8(multimediaInfos.spCode);
            cmd += L"', '";
            cmd += multimediaInfos.subject;
            cmd += L"', '";
            cmd += multimediaInfos.filename;
            cmd += L"', ";
            cmd += Util::StringOp::FromInt(multimediaInfos.weatherCode);
            cmd += L", '";
            cmd += L""; //Util::StringOp::ToUnicode(multimediaInfos.content); //not save it
            cmd += L"' )";
            ExecCommand(cmd);
            id(GetCurrentId());
        }

        void MultimediaDownload::Remove() const {
            std::wstring filter = L"id = " + Util::StringOp::FromInt(id());
            Remove(filter);
        }

        void MultimediaDownload::Remove(std::wstring const& filter) {
            RemoveDatasByFilter(filter);
        }

        std::vector<MultimediaDownload> const MultimediaDownload::Parse(std::string const& content) {
            std::vector<MultimediaDownload> result;
            size_t startPos = 0;
            std::string opCode = SimpleParser::FindContent(content, startPos, "OperCode");
            std::string version = SimpleParser::FindContent(content, startPos, "Version");
            Util::Timestamp timestamp = Util::StringOp::ToTimestamp(SimpleParser::FindContent(content, startPos, "TimeStamp"));
            Header::Result r = static_cast<Header::Result>(Util::StringOp::ToInt(SimpleParser::FindContent(content, startPos, "ResponseResult")));
            std::string info = SimpleParser::FindContent(content, startPos, "MultimediaInfo");
            while(info != "") {
                MultimediaDownload item;
                item.header.opCode = opCode;
                item.header.version = version;
                item.header.timestamp = timestamp;
                item.header.result = r;
                size_t pos = 0;
                item.multimediaInfos.type = static_cast<MultimediaInfo::Type>(Util::StringOp::ToInt(SimpleParser::FindContent(info, pos, "FileType")));
                item.multimediaInfos.spCode = SimpleParser::FindContent(info, pos, "SpCode");
                item.multimediaInfos.subject = Util::StringOp::ToUnicode(SimpleParser::FindContent(info, pos, "Subject"));
                item.multimediaInfos.filename = Util::StringOp::ToUnicode(SimpleParser::FindContent(info, pos, "FileName"));
                item.multimediaInfos.weatherCode = Util::StringOp::ToInt(SimpleParser::FindContent(info, pos, "WeatherCode"));
                item.multimediaInfos.content = SimpleParser::FindContent(info, pos, "FileMeterialContent");
                result.push_back(item);
                info = SimpleParser::FindContent(content, startPos, "MultimediaInfo");
            }
            return result;
        }

        void MultimediaDownload::modifyFieldByDB_(int argc, char** argv, char** columnName, Util::shared_ptr<MultimediaDownload> item) {
            item->id(atoi(argv[GetIndexByName(argc, columnName, "id")]));
            item->header.version		=	argv[GetIndexByName(argc, columnName, "version")];
            item->header.result = static_cast<Header::Result>(atoi(argv[GetIndexByName(argc, columnName, "responseResult")]));
            item->header.timestamp		=	Util::StringOp::ToTimestamp(std::string(argv[GetIndexByName(argc, columnName, "timestamp")]));
            item->multimediaInfos.type				=	static_cast<MultimediaInfo::Type>(atoi(argv[GetIndexByName(argc, columnName, "type")]));
            item->multimediaInfos.spCode			=	argv[GetIndexByName(argc, columnName, "spCode")];
            item->multimediaInfos.subject			=	Util::StringOp::FromUTF8(argv[GetIndexByName(argc, columnName, "subject")]);
            item->multimediaInfos.filename			=	Util::StringOp::FromUTF8(argv[GetIndexByName(argc, columnName, "filename")]);
            item->multimediaInfos.weatherCode		=	atoi(argv[GetIndexByName(argc, columnName, "weatherCode")]);
            item->multimediaInfos.content			=	""; //argv[GetIndexByName(argc, columnName, "content")];
        }

        std::vector<Util::shared_ptr<MenuManager> > const MenuManager::Select(std::wstring const& filter) {
            return GetDatasByFilter(filter, modifyFieldByDB_);
        }

        void MenuManager::Update() const {
            std::wstring cmd = L"UPDATE ";
            cmd += tableName_;
            cmd += L" SET [version] = '";
            cmd += Util::StringOp::FromUTF8(header.version);
            cmd += L"', [responseResult] = '";
            cmd += Util::StringOp::FromInt(header.result);
            cmd += L"', [timestamp] = '";
            cmd += Util::StringOp::FromTimestamp(header.timestamp);
            cmd += L"', [operator] = ";
            cmd += Util::StringOp::FromInt(opType);
            cmd += L", [menuLevel] = ";
            cmd += Util::StringOp::FromInt(menuInfos.menuLevel);
            cmd += L", [parentId] = ";
            cmd += Util::StringOp::FromInt(menuInfos.parentId);
            cmd += L", [selfId] = ";
            cmd += Util::StringOp::FromInt(menuInfos.menuId);
            cmd += L", [type] = ";
            cmd += Util::StringOp::FromInt(menuInfos.type);
            cmd += L", [showOrder] = ";
            cmd += Util::StringOp::FromInt(menuInfos.showOrder);
            cmd += L", [spCode] = '";
            cmd += Util::StringOp::FromUTF8(menuInfos.spCode);
            cmd += L"', [pageCount] = ";
            cmd += Util::StringOp::FromInt(menuInfos.pageCount);
            cmd += L", [imageName] = '";
            cmd += menuInfos.imageName;
            cmd += L"', [imageContent] = '";
            cmd += L""; //Util::StringOp::ToUnicode(menuInfos.imageContent); //not save it
            cmd += L"', [specImageName] = '";
            cmd += menuInfos.specImageName;
            cmd += L"', [specImageContent] = '";
            cmd += L""; //Util::StringOp::ToUnicode(menuInfos.specImageContent); //not save it
            cmd += L"', [url] = '";
            cmd += Util::StringOp::FromUTF8(menuInfos.url);
            cmd += L"', [name] = '";
            cmd += menuInfos.menuName;
            cmd += L"' WHERE id = ";
            cmd += Util::StringOp::FromInt(id());
            ExecCommand(cmd);
        }

        void MenuManager::Insert() {
            std::wstring cmd = L"INSERT INTO ";
            cmd += tableName_;
            cmd += L" ( version, responseResult, timestamp, operator, menuLevel, parentId, selfId, type, showOrder, spCode, pageCount, imageName, imageContent, specImageName, specImageContent, url, name ) VALUES ( '";
            cmd += Util::StringOp::FromUTF8(header.version);
            cmd += L"', '";
            cmd += Util::StringOp::FromInt(header.result);
            cmd += L"', '";
            cmd += Util::StringOp::FromTimestamp(header.timestamp);
            cmd += L"', ";
            cmd += Util::StringOp::FromInt(opType);
            cmd += L", ";
            cmd += Util::StringOp::FromInt(menuInfos.menuLevel);
            cmd += L", ";
            cmd += Util::StringOp::FromInt(menuInfos.parentId);
            cmd += L", ";
            cmd += Util::StringOp::FromInt(menuInfos.menuId);
            cmd += L", ";
            cmd += Util::StringOp::FromInt(menuInfos.type);
            cmd += L", ";
            cmd += Util::StringOp::FromInt(menuInfos.showOrder);
            cmd += L", '";
            cmd += Util::StringOp::FromUTF8(menuInfos.spCode);
            cmd += L"', ";
            cmd += Util::StringOp::FromInt(menuInfos.pageCount);
            cmd += L", '";
            cmd += menuInfos.imageName;
            cmd += L"', '";
            cmd += L""; //Util::StringOp::ToUnicode(menuInfos.imageContent); //not save it
            cmd += L"', '";
            cmd += menuInfos.specImageName;
            cmd += L"', '";
            cmd += L""; //Util::StringOp::ToUnicode(menuInfos.specImageContent); //not save it
            cmd += L"', '";
            cmd += Util::StringOp::FromUTF8(menuInfos.url);
            cmd += L"', '";
            cmd += menuInfos.menuName;
            cmd += L"' )";
            ExecCommand(cmd);
            id(GetCurrentId());
        }

        void MenuManager::Remove() const {
            std::wstring filter = L"id = " + Util::StringOp::FromInt(id());
            Remove(filter);
        }

        void MenuManager::Remove(std::wstring const& filter) {
            RemoveDatasByFilter(filter);
        }

        std::vector<MenuManager> const MenuManager::Parse(std::string const& content) {
            std::vector<MenuManager> result;
            size_t startPos = 0;
            std::string opCode = SimpleParser::FindContent(content, startPos, "OperCode");
            std::string version = SimpleParser::FindContent(content, startPos, "Version");
            Util::Timestamp timestamp = Util::StringOp::ToTimestamp(SimpleParser::FindContent(content, startPos, "TimeStamp"));
            Header::Result r = static_cast<Header::Result>(Util::StringOp::ToInt(SimpleParser::FindContent(content, startPos, "ResponseResult")));
            MenuOperator opType = static_cast<MenuOperator>(Util::StringOp::ToInt(SimpleParser::FindContent(content, startPos, "OperType")));
            std::string info = SimpleParser::FindContent(content, startPos, "MenuInfo");
            while(info != "") {
                MenuManager item;
                item.header.opCode = opCode;
                item.header.version = version;
                item.header.timestamp = timestamp;
                item.header.result = r;
                item.opType = opType;
                size_t pos = 0;
                item.menuInfos.menuLevel = Util::StringOp::ToInt(SimpleParser::FindContent(info, pos, "MenuLevel"));
                item.menuInfos.parentId = Util::StringOp::ToInt(SimpleParser::FindContent(info, pos, "ParentId"));
                item.menuInfos.menuId = Util::StringOp::ToInt(SimpleParser::FindContent(info, pos, "MenuID"));
                item.menuInfos.type = static_cast<MenuInfo::Type>(Util::StringOp::ToInt(SimpleParser::FindContent(info, pos, "Menu")));
                item.menuInfos.showOrder = Util::StringOp::ToInt(SimpleParser::FindContent(info, pos, "ShowOrder"));
                item.menuInfos.spCode = SimpleParser::FindContent(info, pos, "SpCode");
                item.menuInfos.pageCount = Util::StringOp::ToInt(SimpleParser::FindContent(info, pos, "ShowNum"));
                item.menuInfos.imageName = Util::StringOp::ToUnicode(SimpleParser::FindContent(info, pos, "ImageNormalName"));
                item.menuInfos.imageContent = SimpleParser::FindContent(info, pos, "ImageNormalContent");
                item.menuInfos.specImageName = Util::StringOp::ToUnicode(SimpleParser::FindContent(info, pos, "ImageSpecName"));
                item.menuInfos.specImageContent = SimpleParser::FindContent(info, pos, "ImageSpecContent");
                item.menuInfos.url = SimpleParser::FindContent(info, pos, "Url");
                item.menuInfos.menuName = Util::StringOp::ToUnicode(SimpleParser::FindContent(info, pos, "MenuName"));
                result.push_back(item);
                info = SimpleParser::FindContent(content, startPos, "MenuInfo");
            }
            return result;
        }

        void MenuManager::modifyFieldByDB_(int argc, char** argv, char** columnName, Util::shared_ptr<MenuManager> item) {
            item->id(atoi(argv[GetIndexByName(argc, columnName, "id")]));
            item->header.version		=	argv[GetIndexByName(argc, columnName, "version")];
            item->header.result = static_cast<Header::Result>(atoi(argv[GetIndexByName(argc, columnName, "responseResult")]));
            item->header.timestamp		=	Util::StringOp::ToTimestamp(std::string(argv[GetIndexByName(argc, columnName, "timestamp")]));
            item->opType				=	static_cast<MenuOperator>(atoi(argv[GetIndexByName(argc, columnName, "operator")]));
            item->menuInfos.menuLevel				=	atoi(argv[GetIndexByName(argc, columnName, "menuLevel")]);
            item->menuInfos.parentId				=	atoi(argv[GetIndexByName(argc, columnName, "parentId")]);
            item->menuInfos.menuId					=	atoi(argv[GetIndexByName(argc, columnName, "selfId")]);
            item->menuInfos.type					=	static_cast<MenuInfo::Type>(atoi(argv[GetIndexByName(argc, columnName, "type")]));
            item->menuInfos.showOrder				=	atoi(argv[GetIndexByName(argc, columnName, "showOrder")]);
            item->menuInfos.spCode					=	argv[GetIndexByName(argc, columnName, "spCode")];
            item->menuInfos.pageCount				=	atoi(argv[GetIndexByName(argc, columnName, "pageCount")]);
            item->menuInfos.imageName				=	Util::StringOp::FromUTF8(argv[GetIndexByName(argc, columnName, "imageName")]);
            item->menuInfos.imageContent			=	""; //argv[GetIndexByName(argc, columnName, "imageContent")];
            item->menuInfos.specImageName				=	Util::StringOp::FromUTF8(argv[GetIndexByName(argc, columnName, "specImageName")]);
            item->menuInfos.specImageContent			=	""; //argv[GetIndexByName(argc, columnName, "specImageContent")];
            item->menuInfos.url					=	argv[GetIndexByName(argc, columnName, "url")];
            item->menuInfos.menuName				=	Util::StringOp::FromUTF8(argv[GetIndexByName(argc, columnName, "name")]);
        }
    }
}