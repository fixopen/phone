#include "Message.h"

namespace Util {
    std::wstring SMS::Data::Message::tableName_ = L"message";
    int SMS::Data::Message::count_ = 0;
    int SMS::Data::Message::offset_ = 0;
}

namespace SMS {
    namespace Data {
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
            result += Reverse(Util::StringOp::ToUTF8(Util::StringOp::FromInt(addressLength)));
            result += addressType.ToString();
            result += PaddingAndReverse(address);
            return result;
        }

        std::vector<Util::shared_ptr<Message> > const Message::Select(std::wstring const& filter) {
            return GetDatasByFilter(filter, modifyFieldByDB_);
        }

        void Message::Update() const {
            std::wstring cmd = L"UPDATE ";
            //cmd += tableName_;
            //cmd += L" SET [mobileNumber] = '";
            //cmd += static_cast<LPCTSTR>(mobileNumber);
            //cmd += L"', [nickname] = '";
            //cmd += static_cast<LPCTSTR>(nickname);
            //cmd += L"', [type] = ";
            //cmd += Util::StringOp::FromInt(type);
            //cmd += L" WHERE id = ";
            //cmd += Util::StringOp::FromInt(id());
            ExecCommand(cmd);
        }

        void Message::Insert() {
            std::wstring cmd = L"INSERT INTO ";
            cmd += tableName_;
            //cmd += L" ( mobileNumber, nickname, type ) VALUES ( '";
            //cmd += static_cast<LPCTSTR>(mobileNumber);
            //cmd += L"', '";
            //cmd += static_cast<LPCTSTR>(nickname);
            //cmd += L"', ";
            //cmd += Util::StringOp::FromInt(type);
            cmd += L" )";
            ExecCommand(cmd);
            id(GetCurrentId());
        }

        void Message::Remove() const {
            std::wstring filter = L"id = " + Util::StringOp::FromInt(id());
            Remove(filter);
        }

        void Message::Remove(std::wstring filter) {
            //std::vector<Util::shared_ptr<Message> > customers = Select(filter);
            //for (size_t i = 0; i < customers.size(); ++i) {
            //    std::wstring f = L"customerId = " + Util::StringOp::FromInt(customers[i]->id());
            //    Message::Remove(f);
            //}
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
            for (size_t i = 0; i < dataCount; ++i) {
                wchar_t c = Util::StringOp::ToInt(content.substr(pos, 2), 16);
                pos += 2;
                result.data += c;
            }
            return result;
        }

        std::string const Message::ToString() const {
            std::string result;
            result += smsc.ToString();
            result += flag.ToString();
            result += reference;
            result += remote.ToString();
            result += uplevelProtocol;
            result += encoding.ToString();
            result += validityPeriod;
            wchar_t l = static_cast<wchar_t>(data.length());
            char f = l >> 8;
            result += f;
            char n = l << 8 >> 8;
            result += n;
            result += Util::StringOp::ToUTF8(data);
            return result;
        }

        Message::Flag::Flag(Type const atype)
        : isSetReplyPath(1)
        , hasHeaderInfo(0)
        , requestReport(1)
        , validityPeriodFormat(pRelativePresent)
        , rejectCopy(0)
        , type(atype) {
        }

        Message::Flag::Flag(unsigned char v) {
            isSetReplyPath = v & 0x80;
            hasHeaderInfo = v & 0x40;
            requestReport = v & 0x20;
            validityPeriodFormat = static_cast<ValidityPeriodFormat>(v << 2 >> 5);
            rejectCopy = v & 0x04;
            type = static_cast<Type>(v << 6 >> 6);
        }

        unsigned char const Message::Flag::ToString() const {
            unsigned char result = 0;
            result |= (isSetReplyPath << 7)
                | (hasHeaderInfo << 6)
                | (requestReport << 5)
                | (validityPeriodFormat << 3)
                | (rejectCopy << 2)
                | (type << 0);
            return result;
        }

        Message::Encoding::Encoding(EncodingCode aencoding)
            //: high(0)
            : encoding(aencoding) {
            //, low(0) {
        }

        Message::Encoding::Encoding(unsigned char v) {
            encoding = static_cast<EncodingCode>(v << 4 >> 2);
        }

        unsigned char const Message::Encoding::ToString() const {
            unsigned char result = 0;
            result |= (0 << 4)
                | (encoding << 2)
                | (0 << 0);
            return result;
        }

        Message::SMSCTimestamp::SMSCTimestamp(std::string const& v)
        : timezone(0) {
            if (v != "") {
                year = Util::StringOp::ToInt(v.substr(0, 2));
                month = Util::StringOp::ToInt(v.substr(2, 2));
                day = Util::StringOp::ToInt(v.substr(4, 2));
                hour = Util::StringOp::ToInt(v.substr(6, 2));
                minite = Util::StringOp::ToInt(v.substr(8, 2));
                second = Util::StringOp::ToInt(v.substr(10, 2));
                timezone = Util::StringOp::ToInt(v.substr(12, 2));
            }
        }

        std::string const Message::SMSCTimestamp::ToString() const {
            std::string result;
            result += Reverse(Util::StringOp::ToUTF8(Util::StringOp::FromInt(year)));
            result += Reverse(Util::StringOp::ToUTF8(Util::StringOp::FromInt(month)));
            result += Reverse(Util::StringOp::ToUTF8(Util::StringOp::FromInt(day)));
            result += Reverse(Util::StringOp::ToUTF8(Util::StringOp::FromInt(hour)));
            result += Reverse(Util::StringOp::ToUTF8(Util::StringOp::FromInt(minite)));
            result += Reverse(Util::StringOp::ToUTF8(Util::StringOp::FromInt(second)));
            result += Reverse(Util::StringOp::ToUTF8(Util::StringOp::FromInt(timezone)));
            return result;
        }

        void Message::modifyFieldByDB_(int argc, char** argv, char** columnName, Util::shared_ptr<Message> item) {
            item->id(atoi(argv[Util::BaseData::GetIndexByName(argc, columnName, "id")]));
            //item->mobileNumber = Util::StringOp::FromUTF8(argv[Util::BaseData::GetIndexByName(argc, columnName, "mobileNumber")]).c_str();
            //item->nickname = Util::StringOp::FromUTF8(argv[Util::BaseData::GetIndexByName(argc, columnName, "nickname")]).c_str();
            //item->type = static_cast<Type>(atoi(argv[Util::BaseData::GetIndexByName(argc, columnName, "type")]));
        }
    }
}
