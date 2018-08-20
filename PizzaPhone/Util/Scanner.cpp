#include "Scanner.h"

namespace Util {
    namespace Scanner {
        size_t const ScanShortInteger(unsigned char const* const data, size_t const maxLength, size_t& currentPos) {
            //@@len
            size_t result = data[currentPos];
            ++currentPos;
            return result;
        }

        size_t const ScanLongInteger(unsigned char const* const data, size_t const maxLength, size_t& currentPos) {
            size_t result = 0;
            size_t length = data[currentPos];
            ++currentPos;
            for (size_t i = 0; i < length; ++i) {
                result = (result << 8) | data[currentPos];
                ++currentPos;
            }
            return result;
        }

        size_t const ScanVarInteger(unsigned char const* const data, size_t const maxLength, size_t& currentPos) {
            size_t result = 0;
            while (currentPos < maxLength) {
                if (data[currentPos] > 80) {
                    result = (result << 7) + (data[currentPos] | 0x7F);
                } else {
                    result = (result << 7) + data[currentPos];
                    break;
                }
                ++currentPos;
            }
            return result;
        }

        size_t const ScanLength(unsigned char const* const data, size_t const maxLength, size_t& currentPos) {
            size_t result = 0;
            unsigned char len = data[currentPos];
            ++currentPos;
            if (len < 0x1F) {
                result = len;
            } else if (len == 0x1F) {
                result = ScanVarInteger(data, maxLength, currentPos);
            } else {
            }
            return result;
        }

        size_t const ScanDate(unsigned char const* const data, size_t const maxLength, size_t& currentPos) {
            return ScanLongInteger(data, maxLength, currentPos) * 8 * 3600;
        }

        size_t const ScanIntegerParameterValue(unsigned char const* const data, size_t const maxLength, size_t& currentPos) {
            //@@shortInteger || longInteger
            size_t result = 0;
            return result;
        }

        size_t const ScanDeltaSecondsParameterValue(unsigned char const* const data, size_t const maxLength, size_t& currentPos) {
            //@@shortInteger || longInteger
            return ScanIntegerParameterValue(data, maxLength, currentPos);
        }

        std::string const ScanToken(unsigned char const* const data, size_t const maxLength, size_t& currentPos) {
            //@@value + 0
            std::string result = std::string(reinterpret_cast<char const* const>(data + currentPos));
            currentPos += result.length() + 1;
            return result;
        }

        std::string const ScanText(unsigned char const* const data, size_t const maxLength, size_t& currentPos) {
            //@@[quote + ]value + 0
            std::string result;
            unsigned char quote = data[currentPos];
            if (quote < 127) {
                result = ScanToken(data, maxLength, currentPos);
            } else if (quote == 127) {
                ++currentPos;
                result = ScanToken(data, maxLength, currentPos);
            } else {
                //error
            }
            return result;
        }

        std::string const ScanQuotedString(unsigned char const* const data, size_t const maxLength, size_t& currentPos) {
            //@@" + value + 0
            unsigned char quote = data[currentPos];
            //assert(quote == 34);
            ++currentPos;
            return ScanToken(data, maxLength, currentPos);
        }

        std::string const ScanExtensionMedia(unsigned char const* const data, size_t const maxLength, size_t& currentPos) {
            //@@value + 0
            return ScanToken(data, maxLength, currentPos);
        }

        std::string const ScanStringParameterValue(unsigned char const* const data, size_t const maxLength, size_t& currentPos) {
            //@@noValue || tokenValue || quotedString
            std::string result;
            return result;
        }

        std::string const ScanUriParameterValue(unsigned char const* const data, size_t const maxLength, size_t& currentPos) {
            //@@textString
            return ScanText(data, maxLength, currentPos);
        }

        std::string const ScanFrom(unsigned char const* const data, size_t const maxLength, size_t& currentPos) {
            //@@length + 0x80 + value + 0
            size_t length = data[currentPos];
            ++currentPos;
            ++currentPos; //skip 0x80
            return ScanToken(data, maxLength, currentPos);
        }

        OptType const ScanConstrainedEncoding(unsigned char const* const data, size_t const maxLength, size_t& currentPos) {
            //@@extensionMedia || shortInteger
            OptType result;
            return result;
        }

        OptType const ScanConstrainedMedia(unsigned char const* const data, size_t const maxLength, size_t& currentPos) {
            //@@extensionMedia || shortInteger
            return ScanConstrainedEncoding(data, maxLength, currentPos);
        }

        OptType const ScanVersionParameterValue(unsigned char const* const data, size_t const maxLength, size_t& currentPos) {
            //@@shortInteger || textString
            OptType result;
            return result;
        }

        OptType const ScanContentType(unsigned char const* const data, size_t const maxLength, size_t& currentPos) {
            //@@Constrained-media || Content-general-form
            //@@Constrained-media = Constrained-encoding
            //@@Constrained-encoding = Extension-Media | Short-integer
            //@@Extension-media = *TEXT End-of-string
            //@@End-of-string = <Octet 0>
            //@@Short-integer = OCTET
            //@@Content-general-form = Value-length Media-type
            //@@Value-length = Short-length | (Length-quote Length)
            //@@; Value length is used to indicate the length of the value to follow
            //@@Short-length = <Any octet 0-30>
            //@@Length-quote = <Octet 31>
            //@@Length = Uintvar-integer
            //@@Uintvar-integer = 1*5 OCTET
            //@@Media-type = (Well-known-media | Extension-Media) *(Parameter)
            //@@Well-known-media = Integer-value
            //@@Extension-media = *TEXT End-of-string
            //@@Parameter = Typed-parameter | Untyped-parameter
            //@@Typed-parameter =  Well-known-parameter-token Typed-value
            //@@; the actual expected type of the value is implied by the well-known parameter
            //@@Well-known-parameter-token = Integer-value
            //@@; the code values used for parameters are specified in the Assigned Numbers appendix
            //@@Typed-value = Compact-value | Text-value
            //@@; In addition to the expected type, there may be no value.
            //@@; If the value cannot be encoded using the expected type, it shall be encoded as text.
            //@@Compact-value = Integer-value | Date-value | Delta-seconds-value | Q-value | Version-value | Uri-value
            //@@Integer-Value = Short-integer | Long-integer
            //@@Long-integer = Short-length Multi-octet-integer
            //@@Short-length = <Any octet 0-30>
            //@@Multi-octet-integer = 1*30 OCTET
            //@@Date-value = Long-integer
            //@@Delta-seconds-value = Integer-value
            //@@Q-value = 1*2 OCTET
            //@@Version-value = Short-integer | Text-string
            //@@Text-string = [Quote] *TEXT End-of-string
            //@@Quote = <Octet 127>
            //@@Uri-value = Text-string
            //@@Text-value = No-value | Token-text | Quoted-string
            //@@No-value = <Octet 0>
            //@@Token-text = Token End-of-string
            //@@Quoted-string = <Octet 34> *TEXT End-of-string
            //@@Untyped-parameter = Token-text Untyped-value
            //@@; the type of the value is unknown, but it shall be encoded as an integer, if that is possible.
            //@@Untyped-value = Integer-value | Text-value
            OptType result;
            //result.first = MMS::Data::any; //default 
            //if (data[currentPos] >= 0x80) { //@@typeCode
            //    result.first = static_cast<MMS::Data::ContentType>(data[currentPos]);
            //    ++currentPos;
            //} else {
            //    //@@length + typeCode + paramType[0x89] + value1 + 0[ + startParam[0x8A] + value2 + 0]
            //    //@@length + value1 + 0[ + 0x86 + value2 + 0 + 0x85 + value2 + 0]
            //    size_t length = ScanLength(data, maxLength, currentPos);
            //    unsigned char typeCode = data[currentPos];
            //    ++currentPos;
            //    unsigned char param = data[currentPos];
            //    ++currentPos;
            //    if (param == '\x89') { //success
            //        result.second = std::string(reinterpret_cast<char const* const>(data + currentPos));
            //        currentPos += result.second.length();
            //        param = data[currentPos];
            //        ++currentPos;
            //        if (param == '\x8A') { //has startParam
            //            std::string startParam = std::string(reinterpret_cast<char const* const>(data + currentPos));
            //            currentPos += startParam.length();
            //        } else { //not has startParam
            //        }
            //    } else { //parse error
            //        result.second = std::string(reinterpret_cast<char const* const>(data + currentPos));
            //        currentPos += result.second.length() + 1;
            //    }
            //}
            return result;
        }
    }
}