#include "Message.h"

namespace Util {
    namespace Http {
        Message::~Message() {
        }

        std::string const Message::ToString() const {
            std::string result;
            for (std::map<std::string, std::string>::const_iterator i = HeaderFields.begin(); i != HeaderFields.end(); ++i) {
                result += i->first + ": " + i->second + "\r\n";
            }
            result += "\r\n" + Body;
            return result;
        }
    }
}
