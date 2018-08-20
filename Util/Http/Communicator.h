#ifndef __UTIL_HTTP_COMMUNICATOR_H__
#define __UTIL_HTTP_COMMUNICATOR_H__

#include <WinSock2.h>
#include <string>
#include "ResponseMessage.h"
#include "RequestMessage.h"

namespace Protocol {
    class Session;
}

namespace Util {
    namespace Http {
        class Communicator {
        public:
            Communicator();
            ~Communicator();
            //std::string const ConsultKey(std::string const& deviceId, std::string const& platformPublicKey);
            ResponseMessage const Get(RequestMessage const& message) const;
            //boost::shared_ptr<Session> const BeginSession(void);
            //void EndSession(boost::shared_ptr<Session> const& session);
        private:
            void reconnect_() const;
            std::string const hash_(std::string const& randomKey, std::string const& msisdn, std::string const& keyTime);
            int sessionCount_;
            mutable SOCKET socket_;
        };
    }
}

#endif //__UTIL_HTTP_COMMUNICATOR_H__
