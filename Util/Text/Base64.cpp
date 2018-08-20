#include "Base64.h"

namespace Util {
    namespace Text {
        char* Base64::ConvertTable = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

        std::string const Base64::Encode(std::string const& content) {
            std::string result;
            Convert c;
            //not process padding and CRLF
            for (size_t i = 0; i < content.length(); i += 3) {
                c.octetBits[2] = content[i];
                c.octetBits[1] = content[i + 1];
                c.octetBits[0] = content[i + 2];
                result += ConvertTable[c.sixBits.first];
                result += ConvertTable[c.sixBits.second];
                result += ConvertTable[c.sixBits.third];
                result += ConvertTable[c.sixBits.fourth];
            }
            return result;
        }

        std::string const Base64::Decode(std::string const& content) {
            std::string result;
            Convert c;
            for (size_t i = 0; i < content.length(); ) {
                while (content[i] == '\x13' || content[i] == '\x10') {
                    ++i;
                }
                c.sixBits.first = revConvert_(content[i]);
                ++i;
                while (content[i] == '\x13' || content[i] == '\x10') {
                    ++i;
                }
                c.sixBits.second = revConvert_(content[i]);
                ++i;
                while (content[i] == '\x13' || content[i] == '\x10') {
                    ++i;
                }
                if (content[i] != '=') {
                    c.sixBits.third = revConvert_(content[i]);
                    ++i;
                    while (content[i] == '\x13' || content[i] == '\x10') {
                        ++i;
                    }
                    if (content[i] != '=') {
                        c.sixBits.fourth = revConvert_(content[i]);
                        ++i;
                    } else {
                        result += c.octetBits[1];
                        result += c.octetBits[0];
                        break;
                    }
                } else {
                    result += c.octetBits[0];
                    break;
                }
                result += c.octetBits[2];
                result += c.octetBits[1];
                result += c.octetBits[0];
            }
            return result;
        }

        int const Base64::revConvert_(char const c) {
            int result = -1;
            if (c >= 'A' && c <= 'Z') {
                result = c - 'A';
            } else if (c >= 'a' && c <= 'z') {
                result = c - 'a' + 26;
            } else if (c >= '0' && c <= '9') {
                result = c - '0' + 52;
            } else if (c == '+') {
                result = 62;
            } else if (c == '/') {
                result = 63;
            } else {
                //error
            }
            return result;
        }
    }
}
