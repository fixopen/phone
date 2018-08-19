#ifndef __UTIL_HTTP_REQUESTMESSAGE_H__
#define __UTIL_HTTP_REQUESTMESSAGE_H__

#include "HttpMessage.h"

namespace Util {
    namespace Http {
        class RequestMessage : public HttpMessage {
        public:
            enum Type {
                tGet,
                tPost,
                tPut,
                tDelete,
                tHead,
                tOptions,
                tTrace,
                tConnect,
            };
            RequestMessage();
            std::string const ToString() const;
            Type type;
            std::string uri;
            std::string version;
        private:
            std::string const typeToString_(Type type) const;
        };
    }
}

#endif //__UTIL_HTTP_REQUESTMESSAGE_H__
