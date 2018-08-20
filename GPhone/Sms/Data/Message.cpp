#include "Message.h"

namespace Util {
    std::wstring SMS::Data::Message::tableName_ = L"message";
    int SMS::Data::Message::count_ = 0;
    int SMS::Data::Message::offset_ = 0;
}

namespace SMS {
    namespace Data {
        std::vector<Util::shared_ptr<Message> > const Message::Select(std::wstring const& filter) {
            return GetDatasByFilter(filter, modifyFieldByDB_);
        }

        void Message::Update() const {
            std::wstring cmd = L"UPDATE ";
            cmd += tableName_;
            cmd += L" SET [smscAddressType] = ";
            cmd += Util::StringOp::FromInt(smsc.addressType.typeOfNumber);
            cmd += L", [smscAddress] = '";
            cmd += Util::StringOp::FromUTF8(smsc.address);
            cmd += L"', [isSetReplyPath] = ";
            cmd += Util::StringOp::FromInt(flag.isSetReplyPath);
            cmd += L", [hasHeaderInfo] = ";
            cmd += Util::StringOp::FromInt(flag.hasHeaderInfo);
            cmd += L", [requestReport] = ";
            cmd += Util::StringOp::FromInt(flag.requestReport);
            cmd += L", [validityPeriodFormat] = ";
            cmd += Util::StringOp::FromInt(flag.validityPeriodFormat);
            cmd += L", [rejectCopy] = ";
            cmd += Util::StringOp::FromInt(flag.rejectCopy);
            cmd += L", [type] = ";
            cmd += Util::StringOp::FromInt(flag.type);
            cmd += L", [reference] = ";
            cmd += Util::StringOp::FromInt(reference);
            cmd += L", [remoteAddressType] = ";
            cmd += Util::StringOp::FromInt(remote.addressType.typeOfNumber);
            cmd += L", [remoteAddress] = '";
            cmd += Util::StringOp::FromUTF8(remote.address);
            cmd += L"', [uplevelProtocol] = ";
            cmd += Util::StringOp::FromInt(uplevelProtocol);
            cmd += L", [encoding] = ";
            cmd += Util::StringOp::FromInt(encoding.encoding);
            cmd += L", [validityPeriod] = ";
            cmd += Util::StringOp::FromInt(validityPeriod);
            cmd += L", [data] = '";
            cmd += unicodeData;
            cmd += L"', [timestamp] = '";
            cmd += Util::StringOp::FromUTF8(timestamp.ToString());
            cmd += L"', [state] = ";
            cmd += Util::StringOp::FromInt(state);
            cmd += L", [group] = ";
            cmd += Util::StringOp::FromInt(group);
            cmd += L", WHERE [id] = ";
            cmd += Util::StringOp::FromInt(id());
            ExecCommand(cmd);
        }

        void Message::Insert() {
            std::wstring cmd = L"INSERT INTO ";
            cmd += tableName_;
            cmd += L" (smscAddressType, smscAddress, isSetReplyPath, hasHeaderInfo, requestReport, validityPeriodFormat, rejectCopy, type, reference, remoteAddressType, remoteAddress, uplevelProtocol, encoding, validityPeriod, data, timestamp, state, group ) VALUES ( " ; 
            cmd += Util::StringOp::FromInt(smsc.addressType.typeOfNumber);
            cmd += L", '";
            cmd += Util::StringOp::FromUTF8(smsc.address);
            cmd += L"', ";
            cmd += Util::StringOp::FromInt(flag.isSetReplyPath);
            cmd += L", ";
            cmd += Util::StringOp::FromInt(flag.hasHeaderInfo);
            cmd += L", ";
            cmd += Util::StringOp::FromInt(flag.requestReport);
            cmd += L", ";
            cmd += Util::StringOp::FromInt(flag.validityPeriodFormat);
            cmd += L", ";
            cmd += Util::StringOp::FromInt(flag.rejectCopy);
            cmd += L", ";
            cmd += Util::StringOp::FromInt(flag.type);
            cmd += L", ";
            cmd += Util::StringOp::FromInt(reference);
            cmd += L", ";
            cmd += Util::StringOp::FromInt(remote.addressType.typeOfNumber);
            cmd += L", '";
            cmd += Util::StringOp::FromUTF8(remote.address);
            cmd += L"', ";
            cmd += Util::StringOp::FromInt(uplevelProtocol) ;
            cmd += L", ";
            cmd += Util::StringOp::FromInt(encoding.encoding);
            cmd += L", ";
            cmd += Util::StringOp::FromInt(validityPeriod);
            cmd += L", '";
            cmd += unicodeData;
            cmd += L"', '";
            cmd += Util::StringOp::FromUTF8(timestamp.ToString());
            cmd += L"', ";
            cmd += Util::StringOp::FromInt(state);
            cmd += L", ";
            cmd += Util::StringOp::FromInt(group);
            cmd += L" )";
            ExecCommand(cmd);
            id(GetCurrentId());
        }

        void Message::Remove() const {
            std::wstring filter = L"id = " + Util::StringOp::FromInt(id());
            Remove(filter);
        }

        void Message::Remove(std::wstring const& filter) {
            RemoveDatasByFilter(filter);
        }

        Message const Message::Parse(std::string const& content) {
            Message result;
            size_t pos = 0;
            result.smsc.addressLength = Util::StringOp::ToInt(content.substr(pos, 2), 16);
            size_t codeLength = (result.smsc.addressLength - 1) * 2;
            pos += 2;
            result.smsc.addressType = AddressType(Util::StringOp::ToInt(content.substr(pos, 2), 16));
            pos += 2;
            result.smsc.address = TrimF(Reverse(content.substr(pos, codeLength)));
            pos += codeLength;
            result.flag = Flag(Util::StringOp::ToInt(content.substr(pos, 2), 16));
            pos += 2;
            result.reference = Util::StringOp::ToInt(content.substr(pos, 2), 16);
            pos += 2;
            result.remote.addressLength = Util::StringOp::ToInt(content.substr(pos, 2), 16);
            codeLength = result.remote.addressLength - 2;
            if (codeLength % 2) {
                ++codeLength;
            }
            pos += 2;
            result.remote.addressType = AddressType(Util::StringOp::ToInt(content.substr(pos, 2), 16));
            pos += 2;
            result.remote.address = TrimF(Reverse(content.substr(pos, codeLength)));
            pos += codeLength;
            result.uplevelProtocol = Util::StringOp::ToInt(content.substr(pos, 2), 16);
            pos += 2;
            result.encoding = Encoding(Util::StringOp::ToInt(content.substr(pos, 2), 16));
            pos += 2;
            result.timestamp = SMSCTimestamp(content.substr(pos, 14));
            pos += 14;
            size_t dataCount = Util::StringOp::ToInt(content.substr(pos, 2), 16);
            pos += 2;
            switch (result.encoding.encoding) {
            case Encoding::DefaultAlphabet: //7Bit
                //process7Bit();
                break;
            case Encoding::OctetBit: //8Bit
                result.unicodeData = Util::StringOp::ToUnicode(content.substr(pos, dataCount));
                //standard mms
                //宽连十方业务通知：【type 4Byte】【url】
                break;
            case Encoding::UnicodeCodeSet: //UCS2
                //processUCS2();
                for (size_t i = 0; i < dataCount; ++i) {
                    wchar_t c = Util::StringOp::ToInt(content.substr(pos, 2), 16);
                    pos += 2;
                    result.unicodeData += c;
                }
                break;
            case Encoding::Reserved:
                break;
            default:
                break;
            }
            return result;
        }

        std::string const Message::ToString() const {
            std::string result;
            result += smsc.ToString();
            char l[3] = {0};
            char c = flag.ToString();
            sprintf(l, "%02X", c);
            result += l;
            sprintf(l, "%02X", reference);
            result += l;
            result += remote.ToString();
            sprintf(l, "%02X", uplevelProtocol);
            result += uplevelProtocol;
            c = encoding.ToString();
            sprintf(l, "%02X", c);
            result += l;
            sprintf(l, "%02X", validityPeriod);
            result += validityPeriod;
            switch (encoding.encoding) {
            case Encoding::DefaultAlphabet:
                sprintf(l, "%02X", unicodeData.length());
                result += l;
                //result += unicodeData;
                break;
            case Encoding::OctetBit:
                sprintf(l, "%02X", unicodeData.length());
                result += l;
                //result += unicodeData;
                break;
            case Encoding::UnicodeCodeSet:
                sprintf(l, "%02X", unicodeData.length());
                result += l;
                for (size_t i = 0; i < unicodeData.length(); ++i) {
                    char c[3] = {0};
                    sprintf(c, "%02X", unicodeData[i] >> 8);
                    result += c;
                    sprintf(c, "%02X", unicodeData[i] << 8 >> 8);
                    result += c;
                }
                break;
            case Encoding::Reserved:
                //error
                break;
            default:
                //error
                break;
            }
            return result;
        }

        void Message::modifyFieldByDB_(int argc, char** argv, char** columnName, Util::shared_ptr<Message>& item) {
            item->id(atoi(argv[GetIndexByName(argc, columnName, "id")]));
            item->smsc.addressType.mustSet = true;
            item->smsc.addressType.typeOfNumber = static_cast<AddressType::NumberType>((atoi(argv[GetIndexByName(argc,columnName,"smscAddressType")])));
            //item->smsc.addressType.indentification = static_cast<AddressType::Indentification>((atoi(argv[GetIndexByName(argc,columnName,"smscAddressType")])&0x0f) >> 0);
            //AddressType::AddressType(atoi(argv[Util::BaseData::GetIndexByName(argc,columnName,"smscAddressType")]));
            item->smsc.address = argv[GetIndexByName(argc,columnName,"smscAddress")]; //得到的是utf_8
            item->flag.isSetReplyPath = atoi(argv[GetIndexByName(argc,columnName,"isSetReplyPath")]);
            item->flag.hasHeaderInfo = atoi(argv[GetIndexByName(argc,columnName,"hasHeaderInfo")]);
            item->flag.requestReport = atoi(argv[GetIndexByName(argc,columnName,"requestReport")]);
            item->flag.validityPeriodFormat = static_cast<Flag::ValidityPeriodFormat>(atoi(argv[GetIndexByName(argc,columnName,"validityPeriodFormat")]));
            item->flag.rejectCopy = atoi(argv[GetIndexByName(argc,columnName,"rejectCopy")]);
            item->flag.type = static_cast<Flag::Type>(atoi(argv[GetIndexByName(argc,columnName,"type")]));
            item->reference = atoi(argv[GetIndexByName(argc,columnName,"reference")]);
            item->remote.addressType.mustSet = true; //(atoi(argv[GetIndexByName(argc,columnName,"remoteAddressType")])&0x80) >> 7;
            item->remote.addressType.typeOfNumber = static_cast<AddressType::NumberType>((atoi(argv[GetIndexByName(argc,columnName,"remoteAddressType")])));
            //item->remote.addressType.indentification = static_cast<AddressType::Indentification>((atoi(argv[GetIndexByName(argc,columnName,"remoteAddressType")])&0x0f) >> 0);
            //AddressType::AddressType(atoi(argv[GetIndexByName(argc,columnName,"remoteAddressType")]));
            item->remote.address = argv[GetIndexByName(argc,columnName,"remoteAddress")];		
            item->uplevelProtocol = atoi(argv[GetIndexByName(argc,columnName,"uplevelProtocol")]);
            item->encoding.encoding = static_cast<Encoding::EncodingCode>(atoi(argv[GetIndexByName(argc,columnName,"encoding")]));
            item->validityPeriod = atoi(argv[GetIndexByName(argc,columnName,"validityPeriod")]);
            item->unicodeData = Util::StringOp::FromUTF8(argv[GetIndexByName(argc,columnName,"data")]);
            item->timestamp = SMSCTimestamp(argv[GetIndexByName(argc,columnName,"timestamp")]);
            item->state = static_cast<State>(atoi(argv[GetIndexByName(argc, columnName, "state")]));
            item->group = static_cast<Group>(atoi(argv[GetIndexByName(argc, columnName, "group")]));
        }
    }
}
