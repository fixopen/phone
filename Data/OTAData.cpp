#include "stdafx.h"
#include "OTAData.h"

#include "../Util/StringOp.h"

namespace Data {
    std::string Register::tableName_ = "register";
    int Register::rowCount_ = 0;
    int Register::offset_ = 0;
	Indication Register::indication_ = 0;
	sqlite3* Register::db_ = Data::GetDB();

    std::string RegisterReply::tableName_ = "registerReply";
    int RegisterReply::rowCount_ = 0;
    int RegisterReply::offset_ = 0;
	Indication RegisterReply::indication_ = 0;
	sqlite3* RegisterReply::db_ = Data::GetDB();

    std::string Weather::tableName_ = "weather";
    int Weather::rowCount_ = 0;
    int Weather::offset_ = 0;
	Indication Weather::indication_ = 0;
	sqlite3* Weather::db_ = Data::GetDB();

    std::string SoftwareDownload::tableName_ = "softwareDownload";
    int SoftwareDownload::rowCount_ = 0;
    int SoftwareDownload::offset_ = 0;
	Indication SoftwareDownload::indication_ = 0;
	sqlite3* SoftwareDownload::db_ = Data::GetDB();

    std::string MultimediaDownload::tableName_ = "multimediaDownload";
    int MultimediaDownload::rowCount_ = 0;
    int MultimediaDownload::offset_ = 0;
	Indication MultimediaDownload::indication_ = 0;
	sqlite3* MultimediaDownload::db_ = Data::GetDB();

    std::string MenuManager::tableName_ = "menuManager";
    int MenuManager::rowCount_ = 0;
    int MenuManager::offset_ = 0;
	Indication MenuManager::indication_ = 0;
	sqlite3* MenuManager::db_ = Data::GetDB();

	std::string WeatherPicture::tableName_ = "weatherPicture";
	int WeatherPicture::rowCount_ = 0;
	int WeatherPicture::offset_ = 0;
	Indication WeatherPicture::indication_ = 0;
	sqlite3* WeatherPicture::db_ = Data::GetDB();
}

//namespace SMS {
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
					/*
                    year = Util::StringOp::ToInt((std::string)Reverse(v.substr(0, 2)));
                    month = Util::StringOp::ToInt((std::string)Reverse(v.substr(2, 2)));
                    day = Util::StringOp::ToInt((std::string)Reverse(v.substr(4, 2)));
                    hour = Util::StringOp::ToInt((std::string)Reverse(v.substr(6, 2)));
                    minite = Util::StringOp::ToInt((std::string)Reverse(v.substr(8, 2)));
                    second = Util::StringOp::ToInt((std::string)Reverse(v.substr(10, 2)));
                    timezone = Util::StringOp::ToInt((std::string)Reverse(v.substr(12, 2)));]
					*/
					year = Util::StringOp::ToInt((std::string)(v.substr(2, 2)));
                    month = Util::StringOp::ToInt((std::string)(v.substr(4, 2)));
                    day = Util::StringOp::ToInt((std::string)(v.substr(6, 2)));
                    hour = Util::StringOp::ToInt((std::string)(v.substr(8, 2)));
                    minite = Util::StringOp::ToInt((std::string)(v.substr(10, 2)));
                    second = Util::StringOp::ToInt((std::string)(v.substr(12, 2)));
                    timezone = Util::StringOp::ToInt((std::string)(v.substr(14, 2)));
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

		std::string const SMSCTimestamp::ToString_() const {
            std::string result;
            char l[5] = {0};
            sprintf(l, "%04d", (year+2000));
            result += l;
            sprintf(l, "%02d", (month));
            result += (l);
            sprintf(l, "%02d", (day));
            result += (l);
            sprintf(l, "%02d", (hour));
            result += (l);
            sprintf(l, "%02d", (minite));
            result += (l);
            sprintf(l, "%02d", (second));
            result += (l);
            return result;
        }

		std::string const SMSCTimestamp::ToStringFormat_() const {
            std::string result;
            char l[5] = {0};
            sprintf(l, "%04d-", (year+2000));
            result += l;
            sprintf(l, "%02d-", (month));
            result += (l);
            sprintf(l, "%02d ", (day));
            result += (l);
            sprintf(l, "%02d:", (hour));
            result += (l);
            sprintf(l, "%02d:", (minite));
            result += (l);
            sprintf(l, "%02d", (second));
            result += (l);
            return result;
        }


		Util::Timestamp SMSCTimestamp::ToCTimeStamp()
		{
			Util::Timestamp tm(year+2000,	month, day, hour, minite, second);
			return tm;
		}

		namespace SimpleParser {
            std::string const FindContent(std::string const& content, size_t& pos, std::string const& tagName) {
                size_t beginFindPos = content.find("<" + tagName + " />", pos);
				if (beginFindPos != std::string::npos) {
					pos += tagName.length() + 5;
					return "";
				}
                beginFindPos = content.find("<" + tagName + ">", pos);
				if (beginFindPos != std::string::npos) {
					beginFindPos += tagName.length() + 2;
					pos = beginFindPos + 1;
					size_t endFindPos = content.find("</" + tagName + ">", pos);
					pos = endFindPos + 1;
					return content.substr(beginFindPos, endFindPos - beginFindPos);
				}
				return "";
            }
        }

		std::vector<boost::shared_ptr<Register> > Register::GetFromDatabase(std::string filter, Direction const dir, int const id, int const pageSize)
		{
			return DataAccess<Register>::GetDatasByFilter(filter, modifyFieldByDB_, dir, id, pageSize);
		}


//         std::vector<boost::shared_ptr<Register> > const Register::GetFromDatabase(std::string filter, Direction const dir, int const id, int const pageSize) {
//             return DataAccess<Register>GetDatasByFilter(filter, modifyFieldByDB_);
//         }

        void Register::Update() const {
            std::string cmd = "UPDATE ";
            cmd +=  tableName();
            cmd += " SET [version] = '";
			cmd += header.version;
            cmd += "', [timestamp] = '";
            cmd += Util::StringOp::FromTimestamp(header.timestamp);
            cmd += "', [terminalVersion] = '";
			cmd += terminalVersion;
			cmd += "', [imei] = '";
			cmd += IMEI;
			cmd += "', [terminalCode] = '";
			cmd += terminalId;//对应项不一样
            cmd += "' WHERE id = ";
            cmd += Util::StringOp::FromInt(id());
            ExecCommand(cmd);
        }

        void Register::Insert() {
            std::string cmd = "INSERT INTO ";
            cmd +=  tableName();
            cmd += " ( version, timestamp, terminalVersion,imei,terminalCode) VALUES ( '";
            cmd += header.version;
            cmd += "', '";
            cmd += Util::StringOp::FromTimestamp(header.timestamp);
            cmd += "', '";
            cmd += terminalVersion;
			cmd += "', '";
			cmd += IMEI;
			cmd += "', '";
			cmd += terminalId;
            cmd += "' )";
            ExecCommand(cmd);
            id(GetCurrentId());
        }

        void Register::Remove() const {
            std::string filter = "id = " + Util::StringOp::FromInt(id());
            Remove(filter);
        }

        void Register::Remove(std::string const& filter) {
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

        void Register::modifyFieldByDB_(int argc, char** argv, char** columnName, boost::shared_ptr<Register> item) {
            item->id(atoi(argv[Data::getIndexByName(argc, columnName, "id")]));
       		item->header.version		=	argv[Data::getIndexByName(argc, columnName, "version")];
			item->header.timestamp		=	Util::StringOp::ToTimestamp(std::string(argv[Data::getIndexByName(argc, columnName, "timestamp")]));
			item->terminalVersion		=	argv[Data::getIndexByName(argc, columnName, "terminalVersion")];
			item->IMEI					=	argv[Data::getIndexByName(argc, columnName, "imei")];
			item->terminalId			=	argv[Data::getIndexByName(argc, columnName, "terminalCode")];

        }

        std::vector<boost::shared_ptr<RegisterReply> > RegisterReply::GetFromDatabase(std::string filter, Direction const dir, int const id, int const pageSize) {
            return DataAccess<RegisterReply>::GetDatasByFilter(filter, modifyFieldByDB_, dir, id, pageSize);
        }
		
        void RegisterReply::Update() const {
            std::string cmd = "UPDATE ";
            cmd +=  tableName();
            cmd += " SET [version] = '";
            cmd += header.version;
            cmd += "', [timestamp] = '";
            cmd += Util::StringOp::FromTimestamp(header.timestamp);
            cmd += "', [result] = ";
            cmd += Util::StringOp::FromInt(result);
			cmd += ", [area] = '";
			cmd += areaCode;
			cmd += "', [description] = '";
			cmd += description;
			cmd += "' WHERE id = ";
			cmd += Util::StringOp::FromInt(id());
            ExecCommand(cmd);
        }

        void RegisterReply::Insert() {
            std::string cmd = "INSERT INTO ";
            cmd +=  tableName();
            cmd += " ( version, timestamp, result,area,description) VALUES ( '";
            cmd += header.version;
            cmd += "', '";
            cmd += Util::StringOp::FromTimestamp(header.timestamp);
            cmd += "', ";
            cmd += Util::StringOp::FromInt(result);
			cmd += ", '";
			cmd += areaCode;
			cmd += "', '";
			cmd += description;
            cmd += "' )";
            ExecCommand(cmd);
            id(GetCurrentId());
        }

        void RegisterReply::Remove() const {
            std::string filter = "id = " + Util::StringOp::FromInt(id());
            Remove(filter);
        }

        void RegisterReply::Remove(std::string const& filter) {
            RemoveDatasByFilter(filter);
        }

		RegisterReply const RegisterReply::Parse(std::string const& content) {
            RegisterReply result;
            size_t startPos = 0;
            result.header.opCode = SimpleParser::FindContent(content, startPos, "OperCode");
            result.header.version = SimpleParser::FindContent(content, startPos, "Version");
            result.header.timestamp = Util::StringOp::ToTimestamp(SimpleParser::FindContent(content, startPos, "TimeStamp"));
			std::string s = SimpleParser::FindContent(content, startPos, "Result");
			result.result = (RegisterReply::Result)Util::StringOp::ToInt(s);
		//	result.result = Util::StringOp::ToInt(SimpleParser::FindContent(content, startPos, "Result"));
            result.areaCode = SimpleParser::FindContent(content, startPos, "AreaCode");
            result.description = SimpleParser::FindContent(content, startPos, "Desc");
            return result;
        }

        void RegisterReply::modifyFieldByDB_(int argc, char** argv, char** columnName, boost::shared_ptr<RegisterReply> item) {
            item->id(atoi(argv[Data::getIndexByName(argc, columnName, "id")]));
			
			item->header.version		=	argv[Data::getIndexByName(argc, columnName, "version")];
			item->header.timestamp		=	Util::StringOp::ToTimestamp(std::string(argv[Data::getIndexByName(argc, columnName, "timestamp")]));
			item->result				=	(RegisterReply::Result)(atoi(argv[Data::getIndexByName(argc, columnName, "result")]));
			item->areaCode				=	argv[Data::getIndexByName(argc, columnName, "area")];
			item->description			=	argv[Data::getIndexByName(argc, columnName, "description")];

        }

        std::vector<boost::shared_ptr<Weather> > Weather::GetFromDatabase(std::string filter, Direction const dir, int const id, int const pageSize) {
            return DataAccess<Weather>::GetDatasByFilter(filter, modifyFieldByDB_, dir, id, pageSize);
        }
		std::vector<boost::shared_ptr<Weather> > Weather::GetFromDatabaseByTypeOffsetLength(std::string type, int const offset, int const pageSize)
		{
			std::string filter = type;
			return DataAccess<Weather>::GetDatasByFilterAndPageInfo(filter, modifyFieldByDB_, offset, pageSize);
		}

        void Weather::Update() const {         
		//	for (int i = 0 ; i < weatherInfos.size();i++)
			{
				std::string cmd = "UPDATE ";
				cmd +=  tableName() ;
				cmd += " SET [version] = '";
				cmd += header.version;
				cmd += "', [timestamp] = '";
				cmd += Util::StringOp::FromTimestamp(header.timestamp);
				cmd += "', [datetime] = '";
				cmd += Util::StringOp::FromTimestamp(weatherInfos.datetime);
				cmd += "', [area] = '";
				cmd += weatherInfos.area;
				cmd += "', [wind] = '";
				cmd += weatherInfos.wind;
				cmd += "', [temperature] = '";
				cmd += weatherInfos.temperature;
				cmd += "', [code] = ";
				cmd += Util::StringOp::FromInt(weatherInfos.code);
				cmd += ", [description] = '";
				cmd += weatherInfos.description;
				cmd += "' WHERE id = ";
				cmd += Util::StringOp::FromInt(id());
				ExecCommand(cmd);
            }
        }

        void Weather::Insert() {
	//		for (int i = 0 ; i < weatherInfos.size();i++)
			{
				std::string cmd = "INSERT INTO ";
				cmd +=  tableName();
				cmd += " ( version, timestamp,datetime, area,wind,temperature,code,description ) VALUES ( '";
				cmd += header.version;
				cmd += "', '";
				cmd += Util::StringOp::FromTimestamp(header.timestamp);
				cmd += "', '";
				cmd += Util::StringOp::FromTimestamp(weatherInfos.datetime);
				cmd += "', '";
				cmd += weatherInfos.area;
				cmd += "', '";
				cmd += weatherInfos.wind;
				cmd += "', '";
				cmd += weatherInfos.temperature;
				cmd += "', ";
				cmd += Util::StringOp::FromInt(weatherInfos.code);
				cmd += ", '";
				cmd += weatherInfos.description;
				cmd += "' )";
				ExecCommand(cmd);
				id(GetCurrentId());
			}
        }

        void Weather::Remove() const {
            std::string filter = "id = " + Util::StringOp::FromInt(id());
            Remove(filter);
        }

        void Weather::Remove(std::string const& filter) {
            RemoveDatasByFilter(filter);
        }

		std::vector<Weather> const Weather::Parse(std::string const& content) {
            std::vector<Weather> result;
            size_t startPos = 0;
            std::string opCode = SimpleParser::FindContent(content, startPos, "OperCode");
            std::string version = SimpleParser::FindContent(content, startPos, "Version");
            Util::Timestamp timestamp = Util::StringOp::ToTimestamp(SimpleParser::FindContent(content, startPos, "TimeStamp"));
			std::string s = SimpleParser::FindContent(content, startPos, "ResponseResult");
			Header::Result r = (Header::Result)(Util::StringOp::ToInt(s));
            std::string info = SimpleParser::FindContent(content, startPos, "WeathInfo");
            while(info != "") {
                Weather item;
                item.header.opCode = opCode;
                item.header.version = version;
                item.header.timestamp = timestamp;
                item.header.result = r;
                size_t pos = 0;
                item.weatherInfos.area = SimpleParser::FindContent(info, pos, "Area");
                item.weatherInfos.wind = SimpleParser::FindContent(info, pos, "Wind");
                item.weatherInfos.temperature = SimpleParser::FindContent(info, pos, "TemperaTure");
				std::string s = SimpleParser::FindContent(info, pos, "WeatherCode");
				item.weatherInfos.code = Util::StringOp::ToInt(s);
				item.weatherInfos.description = SimpleParser::FindContent(info, pos, "OtherInfo");
                item.weatherInfos.datetime = Util::StringOp::ToTimestamp(SimpleParser::FindContent(info, pos, "ValidTime"));
			    result.push_back(item);
                info = SimpleParser::FindContent(content, startPos, "WeathInfo");
            }
            return result;
        }

        void Weather::modifyFieldByDB_(int argc, char** argv, char** columnName, boost::shared_ptr<Weather> item) {
            item->id(atoi(argv[Data::getIndexByName(argc, columnName, "id")]));
			item->header.version		=	argv[Data::getIndexByName(argc, columnName, "version")];
			item->header.timestamp		=	Util::StringOp::ToTimestamp(std::string(argv[Data::getIndexByName(argc, columnName, "timestamp")]));
			WeatherInfo weather;
			weather.datetime        	=	Util::StringOp::ToTimestamp(std::string(argv[Data::getIndexByName(argc, columnName, "datetime")]));
			weather.area				=	argv[Data::getIndexByName(argc, columnName, "area")];
			weather.wind				=	argv[Data::getIndexByName(argc, columnName, "wind")];
			weather.temperature			=	argv[Data::getIndexByName(argc, columnName, "temperature")];
			weather.code				=	atoi(argv[Data::getIndexByName(argc, columnName, "code")]);
			weather	.description		=	argv[Data::getIndexByName(argc, columnName, "description")];
			item->weatherInfos = (weather);
		}

		std::vector<boost::shared_ptr<WeatherPicture> > WeatherPicture::GetFromDatabase(std::string filter, Direction const dir, int const id, int const pageSize) {
            return DataAccess<WeatherPicture>::GetDatasByFilter(filter, modifyFieldByDB_, dir, id, pageSize);
        }
		
		void WeatherPicture::Update() const {         
			std::string cmd = "UPDATE ";
			cmd += tableName() ;
			cmd += " SET [version] = '";
			cmd += header.version;
			cmd += "', [timestamp] = '";
			cmd += Util::StringOp::FromTimestamp(header.timestamp);
			cmd += "', [code] = '";
			cmd += code;
			cmd += "', [filename] = '";
			cmd += filename;
			cmd += "', [content] = '";
			cmd += content;
			cmd += "' WHERE id = ";
			cmd += Util::StringOp::FromInt(id());
			ExecCommand(cmd);		
		}
		
		void WeatherPicture::Insert() {
			std::string cmd = "INSERT INTO ";
			cmd += tableName();
			cmd += " ( version, timestamp,code,filename,content) VALUES ( '";
			cmd += header.version;
			cmd += "', '";
			cmd += Util::StringOp::FromTimestamp(header.timestamp);
			cmd += "', '";
			cmd += code;
			cmd += "', '";
			cmd += filename;
			cmd += "', '";
			cmd += content;
			cmd += "' )";
			ExecCommand(cmd);
			id(GetCurrentId());
		}
		
		void WeatherPicture::Remove() const {
			std::string filter = "id = " + Util::StringOp::FromInt(id());
			Remove(filter);
		}
		
		void WeatherPicture::Remove(std::string const& filter) {
			RemoveDatasByFilter(filter);
		}
	/*	
		WeatherPicture const WeatherPicture::Parse(std::string const& content) {
			WeatherPicture result;
			return result;
		}
		*/
		
		void WeatherPicture::modifyFieldByDB_(int argc, char** argv, char** columnName, boost::shared_ptr<WeatherPicture> item) {
			item->id(atoi(argv[Data::getIndexByName(argc, columnName, "id")]));
			item->header.version		=	argv[Data::getIndexByName(argc, columnName, "version")];
			item->header.timestamp		=	Util::StringOp::ToTimestamp(std::string(argv[Data::getIndexByName(argc, columnName, "timestamp")]));
			item->code                  =   argv[Data::getIndexByName(argc, columnName, "code")];
			item->filename              =   argv[Data::getIndexByName(argc, columnName, "filename")];
			item->content				=   argv[Data::getIndexByName(argc, columnName, "content")];
		}	


        std::vector<boost::shared_ptr<SoftwareDownload> > SoftwareDownload::GetFromDatabase(std::string filter, Direction const dir, int const id, int const pageSize) {
            return DataAccess<SoftwareDownload>::GetDatasByFilter(filter, modifyFieldByDB_, dir, id, pageSize);
        }

        void SoftwareDownload::Update() const {
		//	for (int i = 0 ; i < softInfos.size();i++)
			{
				std::string cmd = "UPDATE ";
				cmd +=  tableName();
				cmd += " SET [version] = '";
				cmd += header.version;
				cmd += "', [timestamp] = '";
				cmd += Util::StringOp::FromTimestamp(header.timestamp);
				cmd += "', [name] = '";
				cmd += softInfos.name;
				cmd += "', [filename] = '";
				cmd += softInfos.filename;
				cmd += "', [content] = '";
				cmd += softInfos.content;
				cmd += "'WHERE id = ";
				cmd += Util::StringOp::FromInt(id());
				ExecCommand(cmd);
			}
        }

        void SoftwareDownload::Insert() {
      //      for (int i = 0 ; i < softInfos.size();i++)
			{
				std::string cmd = "INSERT INTO ";
				cmd +=  tableName();
				cmd += " ( version, timestamp, name,filename,content) VALUES ( '";
				cmd += header.version;
				cmd += "', '";
				cmd += Util::StringOp::FromTimestamp(header.timestamp);
				cmd += "', '";
				cmd += softInfos.name;
				cmd += "', '";
				cmd += softInfos.filename;
				cmd += "', '";
				cmd += softInfos.content;
				cmd += "' )";
				ExecCommand(cmd);
				id(GetCurrentId());
			}
        }

        void SoftwareDownload::Remove() const {
            std::string filter = "id = " + Util::StringOp::FromInt(id());
            Remove(filter);
        }

        void SoftwareDownload::Remove(std::string const& filter) {
            RemoveDatasByFilter(filter);
        }

        std::vector<SoftwareDownload> const SoftwareDownload::Parse(std::string const& content) {
            std::vector<SoftwareDownload> result;
            size_t startPos = 0;
            std::string opCode = SimpleParser::FindContent(content, startPos, "OperCode");
            std::string version = SimpleParser::FindContent(content, startPos, "Version");
            Util::Timestamp timestamp = Util::StringOp::ToTimestamp(SimpleParser::FindContent(content, startPos, "TimeStamp"));
			std::string s = SimpleParser::FindContent(content, startPos, "ResponseResult");
			Header::Result r = (Header::Result)(Util::StringOp::ToInt(s));
            std::string info = SimpleParser::FindContent(content, startPos, "WeatherInfo");
            while(info != "") {
                SoftwareDownload item;
                item.header.opCode = opCode;
                item.header.version = version;
                item.header.timestamp = timestamp;
                item.header.result = r;
                size_t pos = 0;
                item.softInfos.name = SimpleParser::FindContent(info, pos, "SoftWareName");
                item.softInfos.filename = SimpleParser::FindContent(info, pos, "SoftWareFileName");
                item.softInfos.content = SimpleParser::FindContent(info, pos, "SoftWareMaterialContent");
                result.push_back(item);
                info = SimpleParser::FindContent(content, startPos, "WeatherInfo");
            }
            return result;
        }

        void SoftwareDownload::modifyFieldByDB_(int argc, char** argv, char** columnName, boost::shared_ptr<SoftwareDownload> item) {
            item->id(atoi(argv[Data::getIndexByName(argc, columnName, "id")]));
  		
			item->header.version		=	argv[Data::getIndexByName(argc, columnName, "version")];
			item->header.timestamp		=	Util::StringOp::ToTimestamp(std::string(argv[Data::getIndexByName(argc, columnName, "timestamp")]));
			SoftInfo soft;
			soft.name					=	argv[Data::getIndexByName(argc, columnName, "name")];
			soft.filename				=	argv[Data::getIndexByName(argc, columnName, "filename")];
			soft.content				=	argv[Data::getIndexByName(argc, columnName, "content")];
			item->softInfos = (soft);

		}

        std::vector<boost::shared_ptr<MultimediaDownload> > MultimediaDownload::GetFromDatabase(std::string filter, Direction const dir, int const id, int const pageSize) {
            return DataAccess<MultimediaDownload>::GetDatasByFilter(filter, modifyFieldByDB_, dir, id, pageSize);
        }

		std::vector<boost::shared_ptr<MultimediaDownload> > MultimediaDownload::GetFromDatabaseByTypeOffsetLength(std::string type, int const offset, int const pageSize)
		{
			std::string filter = type;
			return DataAccess<MultimediaDownload>::GetDatasByFilterAndPageInfo(filter, modifyFieldByDB_, offset, pageSize);
		}

        void MultimediaDownload::Update() const {
        //    for (int i = 0 ; i < multimediaInfos.size();i++)
			{
				std::string cmd = "UPDATE ";
				cmd +=  tableName();
				cmd += " SET [version] = '";
				cmd += header.version;
				cmd += "', [timestamp] = '";
				cmd += Util::StringOp::FromTimestamp(header.timestamp);
				cmd += "', [type] = ";
				cmd += Util::StringOp::FromInt(multimediaInfos.type);
				cmd += ", [spCode] = '";
				cmd += multimediaInfos.spCode;
				cmd += "', [subject] = '";
				cmd += multimediaInfos.subject;
				cmd += "', [filename] = '";
				cmd += multimediaInfos.filename;
				cmd += "', [weatherCode] = ";
				cmd += Util::StringOp::FromInt(multimediaInfos.weatherCode);
				cmd += ", [isRead] =";
				cmd += Util::StringOp::FromInt(multimediaInfos.isRead);
				cmd += ", [content] = '";
				cmd += multimediaInfos.content;
				cmd += "' WHERE id = ";
				cmd += Util::StringOp::FromInt(id());
				ExecCommand(cmd);
			}
        }

        void MultimediaDownload::Insert() {
      //      for (int i = 0 ; i < multimediaInfos.size();i++)
			{
				std::string cmd = "INSERT INTO ";
				cmd +=  tableName();
				cmd += " ( version, timestamp, type,spCode,subject,filename,weatherCode,isRead, content ) VALUES ( '";
				cmd += header.version;
				cmd += "', '";
				cmd += Util::StringOp::FromTimestamp(header.timestamp);
				cmd += "', ";
				cmd += Util::StringOp::FromInt(multimediaInfos.type);
				cmd += ", '";
				cmd += multimediaInfos.spCode;
				cmd += "', '";
				cmd += multimediaInfos.subject;
				cmd += "', '";
				cmd += multimediaInfos.filename;
				cmd += "', ";
				cmd += Util::StringOp::FromInt(multimediaInfos.weatherCode);
				cmd += ", ";
				cmd += Util::StringOp::FromInt(multimediaInfos.isRead);
				cmd += ", '";
				cmd += multimediaInfos.content;
				cmd += "' )";
				ExecCommand(cmd);
				id(GetCurrentId());
			}
        }

        void MultimediaDownload::Remove() const {
            std::string filter = "id = " + Util::StringOp::FromInt(id());
            Remove(filter);
        }

        void MultimediaDownload::Remove(std::string const& filter) {
            RemoveDatasByFilter(filter);
        }

        std::vector<MultimediaDownload> const MultimediaDownload::Parse(std::string const& content) {
          	std::vector<MultimediaDownload> result;
            size_t startPos = 0;
            std::string opCode = SimpleParser::FindContent(content, startPos, "OperCode");
            std::string version = SimpleParser::FindContent(content, startPos, "Version");
            Util::Timestamp timestamp = Util::StringOp::ToTimestamp(SimpleParser::FindContent(content, startPos, "TimeStamp"));
            std::string info = SimpleParser::FindContent(content, startPos, "MultimediaInfo");
            while(info != "") {
				size_t pos = 0;
                MultimediaDownload item;
                item.header.opCode = opCode;
                item.header.version = version;
                item.header.timestamp = timestamp;
                item.multimediaInfos.type = static_cast<MultimediaInfo::Type>(Util::StringOp::ToInt(CString(SimpleParser::FindContent(info, pos, "FileType").c_str())));
                item.multimediaInfos.spCode = SimpleParser::FindContent(info, pos, "SpCode");
                item.multimediaInfos.subject = SimpleParser::FindContent(info, pos, "Subject");
                item.multimediaInfos.filename = SimpleParser::FindContent(info, pos, "FileName");
                item.multimediaInfos.weatherCode = Util::StringOp::ToInt(CString(SimpleParser::FindContent(info, pos, "WeatherCode").c_str()));
                item.multimediaInfos.content = SimpleParser::FindContent(info, pos, "FileMaterialContent");
                item.multimediaInfos.isRead = false; 
				result.push_back(item);
                info = SimpleParser::FindContent(content, startPos, "MultimediaInfo");
            }
            return result;
        }

        void MultimediaDownload::modifyFieldByDB_(int argc, char** argv, char** columnName, boost::shared_ptr<MultimediaDownload> item) {
            item->id(atoi(argv[Data::getIndexByName(argc, columnName, "id")]));
           	item->header.version		=	argv[Data::getIndexByName(argc, columnName, "version")];
			item->header.timestamp		=	Util::StringOp::ToTimestamp(std::string(argv[Data::getIndexByName(argc, columnName, "timestamp")]));
			MultimediaInfo multimedia;
			multimedia.type				=	static_cast<MultimediaInfo::Type>(atoi(argv[Data::getIndexByName(argc, columnName, "type")]));
			multimedia.spCode			=	argv[Data::getIndexByName(argc, columnName, "spCode")];
			multimedia.subject			=	argv[Data::getIndexByName(argc, columnName, "subject")];
			multimedia.filename			=	argv[Data::getIndexByName(argc, columnName, "filename")];
			multimedia.weatherCode		=	atoi(argv[Data::getIndexByName(argc, columnName, "weatherCode")]);
			multimedia.isRead		=	atoi(argv[Data::getIndexByName(argc, columnName, "isRead")]);
			multimedia.content			=	argv[Data::getIndexByName(argc, columnName, "content")];
			item->multimediaInfos = (multimedia);
		}
        std::vector<boost::shared_ptr<MenuManager> > MenuManager::GetFromDatabase(std::string filter, Direction const dir, int const id, int const pageSize) {
            return DataAccess<MenuManager>::GetDatasByFilter(filter, modifyFieldByDB_, dir, id, pageSize);
        }
		std::vector<boost::shared_ptr<MenuManager> > MenuManager::GetFromDatabaseByOrder(std::string const filter, std::string order, Direction const dir)
		{
			return DataAccess<MenuManager>::GetDatasByFilterOrderBy(filter, modifyFieldByDB_, order, dir);
		}

        void MenuManager::Update() const {
        //    for (int i = 0 ; i < menuInfos.size();i++)
			{
				std::string cmd = "UPDATE ";
				cmd +=  tableName();
				cmd += " SET [version] = '";
				cmd += header.version;
				cmd += "', [timestamp] = '";
				cmd += Util::StringOp::FromTimestamp(header.timestamp);
				cmd += "', [operator] = ";
				cmd += Util::StringOp::FromInt(opType);
				cmd += ", [menuLevel] = ";
				cmd += Util::StringOp::FromInt(menuInfos.menuLevel);
				cmd += ", [parentId] = ";
				cmd += Util::StringOp::FromInt(menuInfos.parentId);
				cmd += ", [selfId] = ";
				cmd += Util::StringOp::FromInt(menuInfos.menuId);// 没找到值啊
				cmd += ", [type] = ";
				cmd += Util::StringOp::FromInt(menuInfos.type);
				cmd += ", [showOrder] = ";
				cmd += Util::StringOp::FromInt(menuInfos.showOrder);
				cmd += ", [spCode] = '";
				cmd += menuInfos.spCode;
				cmd += "', [pageCount] = ";
				cmd += Util::StringOp::FromInt(menuInfos.pageCount);
				cmd += ", [imageName] = '";
				cmd += menuInfos.imageName;
				cmd += "', [imageContent] = '";
				cmd += menuInfos.imageContent;
				cmd += "', [specImageName] = '";
				cmd += menuInfos.specImageName;
				cmd += "', [specImageContent] = '";
				cmd += menuInfos.specImageContent;
				cmd += "', [url] = '";
				cmd += menuInfos.url;
				cmd += "', [name] = '";
				cmd += menuInfos.menuName;
				cmd += "' WHERE id = ";
				cmd += Util::StringOp::FromInt(id());
				ExecCommand(cmd);
			}
        }

        void MenuManager::Insert() {
	//		for (int i = 0 ; i < menuInfos.size();i++)
			{
				std::string cmd = "INSERT INTO ";
				cmd +=  tableName();
				cmd += " ( version, timestamp, operator,menuLevel,parentId,selfId,type,showOrder,spCode,pageCount,imageName,imageContent,specImageName,specImageContent,url,name ) VALUES ( '";
				cmd += header.version;
				cmd += "', '";
				cmd += Util::StringOp::FromTimestamp(header.timestamp);
				cmd += "', ";
				cmd += Util::StringOp::FromInt(opType);
				cmd += ", ";
				cmd += Util::StringOp::FromInt(menuInfos.menuLevel);
				cmd += ", ";
				cmd += Util::StringOp::FromInt(menuInfos.parentId);
				cmd += ", ";
				cmd += Util::StringOp::FromInt(menuInfos.menuId);
				cmd += ", ";
				cmd += Util::StringOp::FromInt(menuInfos.type);
				cmd += ", ";
				cmd += Util::StringOp::FromInt(menuInfos.showOrder);
				cmd += ", '";
				cmd += menuInfos.spCode;
				cmd += "', ";
				cmd += Util::StringOp::FromInt(menuInfos.pageCount);
				cmd += ", '";
				cmd += menuInfos.imageName;
				cmd += "', '";
				cmd += menuInfos.imageContent;
				cmd += "', '";
				cmd += menuInfos.specImageName;
				cmd += "', '";
				cmd += menuInfos.specImageContent;
				cmd += "', '";
				cmd += menuInfos.url;
				cmd += "', '";
				cmd += menuInfos.menuName;
				cmd += "' )";
				ExecCommand(cmd);
				extern VOID WriteLog(CHAR* str);
				WriteLog((char *)cmd.c_str());
				id(GetCurrentId());
			}
        }

        void MenuManager::Remove() const {
            std::string filter = "id = " + Util::StringOp::FromInt(id());
            Remove(filter);
        }

        void MenuManager::Remove(std::string const& filter) {
            RemoveDatasByFilter(filter);
        }

		std::vector<MenuManager> const MenuManager::Parse(std::string const& content) {
            std::vector<MenuManager> result;
            size_t startPos = 0;
            std::string opCode = SimpleParser::FindContent(content, startPos, "OperCode");
            std::string version = SimpleParser::FindContent(content, startPos, "Version");
            Util::Timestamp timestamp = Util::StringOp::ToTimestamp(SimpleParser::FindContent(content, startPos, "TimeStamp"));
			std::string s = SimpleParser::FindContent(content, startPos, "ResponseResult");
			Header::Result r = (Header::Result)(Util::StringOp::ToInt(s));
            MenuOperator opType = static_cast<MenuOperator>(Util::StringOp::ToInt((std::string)SimpleParser::FindContent(content, startPos, "OperType")));
            std::string info = SimpleParser::FindContent(content, startPos, "MenuInfo");
            while(info != "") {
                MenuManager item;
                item.header.opCode = opCode;
                item.header.version = version;
                item.header.timestamp = timestamp;
                item.header.result = r;
                item.opType = opType;
                size_t pos = 0;
                item.menuInfos.menuLevel = Util::StringOp::ToInt((std::string)SimpleParser::FindContent(info, pos, "MenuLevel"));
                item.menuInfos.parentId = Util::StringOp::ToInt((std::string)SimpleParser::FindContent(info, pos, "ParentId"));
                item.menuInfos.menuId = Util::StringOp::ToInt((std::string)SimpleParser::FindContent(info, pos, "MenuID"));
                item.menuInfos.type = static_cast<MenuInfo::Type>(Util::StringOp::ToInt((std::string)SimpleParser::FindContent(info, pos, "Menu")));
                item.menuInfos.showOrder = Util::StringOp::ToInt((std::string)SimpleParser::FindContent(info, pos, "ShowOrder"));
                item.menuInfos.spCode = SimpleParser::FindContent(info, pos, "SpCode");
                item.menuInfos.pageCount = Util::StringOp::ToInt((std::string)SimpleParser::FindContent(info, pos, "ShowNum"));
                item.menuInfos.imageName = SimpleParser::FindContent(info, pos, "ImageNormalName");
                item.menuInfos.imageContent = SimpleParser::FindContent(info, pos, "ImageNormalContent");
                item.menuInfos.specImageName = SimpleParser::FindContent(info, pos, "ImageSpecName");
                item.menuInfos.specImageContent = SimpleParser::FindContent(info, pos, "ImageSpecContent");
                item.menuInfos.url = SimpleParser::FindContent(info, pos, "Url");
                item.menuInfos.menuName = SimpleParser::FindContent(info, pos, "MenuName");
                result.push_back(item);
                info = SimpleParser::FindContent(content, startPos, "MenuInfo");
            }
            return result;
        }

        void MenuManager::modifyFieldByDB_(int argc, char** argv, char** columnName, boost::shared_ptr<MenuManager> item) {
            item->id(atoi(argv[Data::getIndexByName(argc, columnName, "id")]));
            //item->mobileNumber = Util::StringOp::FromUTF8(argv[Data::getIndexByName(argc, columnName, "mobileNumber")]).c_str();
            //item->nickname = Util::StringOp::FromUTF8(argv[Data::getIndexByName(argc, columnName, "nickname")]).c_str();
            //item->type = static_cast<Type>(atoi(argv[Data::getIndexByName(argc, columnName, "type")]));
			
			item->header.version		=	argv[Data::getIndexByName(argc, columnName, "version")];
			item->header.timestamp		=	Util::StringOp::ToTimestamp(std::string(argv[Data::getIndexByName(argc, columnName, "timestamp")]));
			item->opType				=	static_cast<MenuOperator>(atoi(argv[Data::getIndexByName(argc, columnName, "operator")]));
			MenuInfo menu;
			menu.menuLevel				=	atoi(argv[Data::getIndexByName(argc, columnName, "menuLevel")]);
			menu.parentId				=	atoi(argv[Data::getIndexByName(argc, columnName, "parentId")]);
			menu.menuId					=	atoi(argv[Data::getIndexByName(argc, columnName, "selfId")]);
			menu.type					=	static_cast<MenuInfo::Type>(atoi(argv[Data::getIndexByName(argc, columnName, "type")]));
			menu.showOrder				=	atoi(argv[Data::getIndexByName(argc, columnName, "showOrder")]);
			menu.spCode					=	argv[Data::getIndexByName(argc, columnName, "spCode")];
			menu.pageCount				=	atoi(argv[Data::getIndexByName(argc, columnName, "pageCount")]);
			menu.imageName				=	argv[Data::getIndexByName(argc, columnName, "imageName")];
			menu.imageContent			=	argv[Data::getIndexByName(argc, columnName, "imageContent")];
			menu.specImageName				=	argv[Data::getIndexByName(argc, columnName, "specImageName")];
			menu.specImageContent			=	argv[Data::getIndexByName(argc, columnName, "specImageContent")];
			menu.url					=	argv[Data::getIndexByName(argc, columnName, "url")];
			menu.menuName				=	argv[Data::getIndexByName(argc, columnName, "name")];
			item->menuInfos = (menu);
		}
    }
//}