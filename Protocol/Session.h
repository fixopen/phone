#pragma once

#include <string>
#include <map>
#include "ResponseMessage.h"
class Communicator;

    //data:[MIME-type;parameters][;base64],data
    //base64 or urlencode

class Session {
public:
    Session(std::string const& server = "218.249.60.87:8088", std::string const& userAgent = "yd6618", std::string const& os = "99", std::string const& version = "1");
    ~Session();
    /*virtual */void Process(std::string const& body = "");
protected:
    ResponseMessage const auth_(std::map<std::string, std::string>& values, std::string const& digestUriValue) const;
    ResponseMessage const timeout_() const;
    virtual void selfProcess_(ResponseMessage const& response) const;
	std::string const findContent_(std::string const& whole, std::string const& tagName, size_t& offset) const;

    std::string const sha1_(std::string const& value) const;
    std::string const md5_(std::string const& value) const;
    std::string const base16_(std::string const& value) const;
    std::string const unquote_(std::string const& value) const;
protected:
    Communicator* communicator_;
    static int nc_;
    std::string const msisdn_d_() const;
    std::string const deviceId_() const;
    std::string const platformPublicKey_() const;
    virtual std::string const resourceName_() const;
    virtual std::string const additionParameter_() const;
    std::string server_;
    std::string userAgent_;
    std::string os_;
    std::string version_;
};
