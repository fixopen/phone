#include "SMSParser.h"
#include "../Data/OTAData.h"

namespace SMS {
    namespace Logical {
        SMSParser::MessageType const SMSParser::getMessageType(Data::Message const& message) {
            MessageType result;
            return result;
        }
        void SMSParser::ProcessOTAMessageContent(Data::Type const mt, std::string const& content) {
            size_t startPos = 0;
            std::string opCode = Data::SimpleParser::FindContent(content, startPos, "OperCode");
            std::string version = Data::SimpleParser::FindContent(content, startPos, "Version");
            Util::Timestamp timestamp = Util::StringOp::ToTimestamp(Data::SimpleParser::FindContent(content, startPos, "TimeStamp"));
            if (opCode == "REGISTER") {
                //register reply
            } else if (opCode == "WEATHERINFO") {
                //weather
            } else if (opCode == "SOFTWARELOAD") {
                //software
            } else if (opCode == "FILELOAD") {
                ////multimedia
                //std::vector<Data::MultimediaDownload> result;
                //std::string info = Data::SimpleParser::FindContent(content, startPos, "MultimediaInfo");
                //while(info != "") {
                //    Data::MultimediaDownload item;
                //    item.header.opCode = opCode;
                //    item.header.version = version;
                //    item.header.timestamp = timestamp;
                //    size_t pos = 0;
                //    item.multimediaInfos.type = static_cast<MultimediaInfo::Type>(Util::StringOp::ToInt(Data::SimpleParser::FindContent(info, pos, "FileType")));
                //    item.multimediaInfos.spCode = Data::SimpleParser::FindContent(info, pos, "SpCode");
                //    item.multimediaInfos.subject = Data::SimpleParser::FindContent(info, pos, "Subject");
                //    item.multimediaInfos.filename = Data::SimpleParser::FindContent(info, pos, "FileName");
                //    item.multimediaInfos.weatherCode = Util::StringOp::ToInt(Data::SimpleParser::FindContent(info, pos, "WeatherCode"));
                //    item.multimediaInfos.content = Data::SimpleParser::FindContent(info, pos, "FileMeterialContent");
                //    result.push_back(item);
                //    info = Data::SimpleParser::FindContent(content, startPos, "MultimediaInfo");
                //}
                //for (size_t i = 0; i < result.size(); ++i) {
                //    result[i].Insert();
                //    std::string d = Util::Base64::Decode(result[i].multimediaInfos.content);
                //}
            } else if (opCode == "MENU") {
                //menu
            } else {
                //not support
            }
        }
    }
}
