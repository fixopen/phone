#ifndef __UTIL_HTTP_REQUESTMESSAGE_H__
#define __UTIL_HTTP_REQUESTMESSAGE_H__

#include "Message.h"

namespace Util {
    namespace Http {
        class RequestMessage : public Message {
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
            ~RequestMessage(void);
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
