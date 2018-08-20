#ifndef __UTIL_HTTP_HTTPMESSAGE_H__
#define __UTIL_HTTP_HTTPMESSAGE_H__

#include <vector>
#include <string>
#include <map>

namespace Util {
    namespace Http {
        class HttpMessage {
        public:
            virtual ~HttpMessage();
            std::map<std::string, std::string> HeaderFields;
            std::string Body;
            std::string const ToString() const;
        };
    }
}

#endif //__UTIL_HTTP_HTTPMESSAGE_H__
