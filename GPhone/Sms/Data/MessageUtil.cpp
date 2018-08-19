#include "MessageUtil.h"
#include "../../Util/Text/StringOp.h"

namespace Sms {
    namespace Data {
        std::string const TrimF(std::string const & v) {
            std::string result = v;
            if (v[v.length() - 1] == 'F') {
                result = v.substr(0, v.length() - 1);
            }
            return result;
        }

        std::string const Reverse(std::string const & v) {
            std::string result;
            for (size_t i = 0; i < v.length(); i += 2) {
                result += v[i + 1];
                result += v[i];
            }
            return result;
        }

        std::string const PaddingAndReverse(std::string const & v) {
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
            typeOfNumber = static_cast<NumberType>((v & 0x70) >> 4);
            indentification = static_cast<Indentification>(v & 0xF);
        }

        unsigned char const AddressType::ToString() const {
            unsigned char result = 0;
            result |= (mustSet << 7) | (typeOfNumber << 3) | (indentification << 0);
            return result;
        }

        std::string const Endpoint::ToString() const {
            std::string result;
            char l[3] = {
                0
            };
            //sprintf(l, "%02X", addressLength);
            sprintf_s(l, 3, "%02X", addressLength);
            result += Reverse(l);
            char c = addressType.ToString();
            //sprintf(l, "%02X", c);
            sprintf_s(l, 3, "%02X", c);
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
            isSetReplyPath = !!(v & 0x80);
            hasHeaderInfo = !!(v & 0x40);
            requestReport = !!(v & 0x20);
            validityPeriodFormat = static_cast<ValidityPeriodFormat>((v & 0x18) >> 3);
            rejectCopy = !!(v & 0x4);
            type = static_cast<Type>(v & 0x3);
        }

        unsigned char const Flag::ToString() const {
            unsigned char result = 0;
            result |= (isSetReplyPath << 7)
            | (hasHeaderInfo << 6)
            | (requestReport << 5) | (validityPeriodFormat << 3) | (rejectCopy << 2) | (type << 0);
            return result;
        }

        Encoding::Encoding(EncodingCode aencoding)
        //: high(0)

        : encoding(aencoding) {
            //, low(0) {
        }

        Encoding::Encoding(unsigned char v) {
            encoding = static_cast<EncodingCode>((v & 0xC) >> 2);
        }

        unsigned char const Encoding::ToString() const {
            unsigned char result = 0;
            result |= (0 << 4)
            | (encoding << 2)
            | (0 << 0);
            return result;
        }

        SMSCTimestamp::SMSCTimestamp(std::string const & v)
        : timezone(0) {
            if (v != "") {
                year = Util::Text::StringOp::ToInt(Reverse(v.substr(0, 2)));
                month = Util::Text::StringOp::ToInt(Reverse(v.substr(2, 2)));
                day = Util::Text::StringOp::ToInt(Reverse(v.substr(4, 2)));
                hour = Util::Text::StringOp::ToInt(Reverse(v.substr(6, 2)));
                minite = Util::Text::StringOp::ToInt(Reverse(v.substr(8, 2)));
                second = Util::Text::StringOp::ToInt(Reverse(v.substr(10, 2)));
                timezone = Util::Text::StringOp::ToInt(Reverse(v.substr(12, 2)));
            }
        }

        std::string const SMSCTimestamp::ToString() const {
            std::string result;
            char l[3] = {
                0
            };
            //sprintf(l, "%02X", Util::Text::StringOp::FromInt(year));
            sprintf_s(l, 3, "%02X", Util::Text::StringOp::FromInt(year));
            result += Reverse(l);
            //sprintf(l, "%02X", Util::Text::StringOp::FromInt(month));
            sprintf_s(l, 3, "%02X", Util::Text::StringOp::FromInt(month));
            result += Reverse(l);
            //sprintf(l, "%02X", Util::Text::StringOp::FromInt(day));
            sprintf_s(l, 3, "%02X", Util::Text::StringOp::FromInt(day));
            result += Reverse(l);
            //sprintf(l, "%02X", Util::Text::StringOp::FromInt(hour));
            sprintf_s(l, 3, "%02X", Util::Text::StringOp::FromInt(hour));
            result += Reverse(l);
            //sprintf(l, "%02X", Util::Text::StringOp::FromInt(minite));
            sprintf_s(l, 3, "%02X", Util::Text::StringOp::FromInt(minite));
            result += Reverse(l);
            //sprintf(l, "%02X", Util::Text::StringOp::FromInt(second));
            sprintf_s(l, 3, "%02X", Util::Text::StringOp::FromInt(second));
            result += Reverse(l);
            //sprintf(l, "%02X", Util::Text::StringOp::FromInt(timezone));
            sprintf_s(l, 3, "%02X", Util::Text::StringOp::FromInt(timezone));
            result += Reverse(l);
            return result;
        }
    }
}
