#ifndef __UTIL_HTTP_MESSAGE_H__
#define __UTIL_HTTP_MESSAGE_H__

#include <vector>
#include <string>
#include <map>

namespace Util {
    namespace Http {
        class Message {
        public:
            virtual ~Message();
            std::map<std::string, std::string> HeaderFields;
            std::string Body;
            std::string const ToString() const;
        };
    }
}

#endif //__UTIL_HTTP_MESSAGE_H__
