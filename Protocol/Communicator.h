#pragma once

//#include <WinSock2.h>
#include <WinSock.h>
#include <string>
#include "ResponseMessage.h"
#include "RequestMessage.h"
class Session;

class Communicator {
public:
    Communicator();
    ~Communicator();
    std::string const ConsultKey(std::string const& deviceId, std::string const& platformPublicKey);
    ResponseMessage const Get(RequestMessage const& message) const;
    //boost::shared_ptr<Session> const BeginSession(void);
    //void EndSession(boost::shared_ptr<Session> const& session);
private:
    void reconnect_() const;
    std::string const rsaEncript_(std::string const& platformPublicKey, std::string const& deviceId, std::string const& randomKey);
    std::string const hash_(std::string const& randomKey, std::string const& msisdn, std::string const& keyTime);
    int sessionCount_;
    mutable SOCKET socket_;
};
