#include "HttpMessage.h"

namespace Util {
    namespace Http {
        HttpMessage::~HttpMessage() {
        }

        std::string const HttpMessage::ToString() const {
            std::string result;
            for (std::map<std::string, std::string>::const_iterator i = HeaderFields.begin(); i != HeaderFields.end(); ++i) {
                result += i->first + ": " + i->second + "\r\n";
            }
            result += "\r\n" + Body;
            return result;
        }
    }
}
