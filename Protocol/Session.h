#pragma once

#include <string>
#include <map>
#include "ResponseMessage.h"
#include "../Util/RSA/rsaeuro/source/rsaeuro.h"

class Communicator;

    //data:[MIME-type;parameters][;base64],data
    //base64 or urlencode
class Session {
public:
	//218.249.60.87
    Session(std::string const& server = "192.168.13.245:8088", std::string const& userAgent = "yd6618", std::string const& os = "99", std::string const& version = "1");
    ~Session();
    /*virtual */void Process(std::string const& body = "");
	std::string const msisdn() const {
		return msisdn_;
	}
protected:
    ResponseMessage const auth_(std::map<std::string, std::string>& values, std::string const& digestUriValue) const;
    ResponseMessage const timeout_() const;
    virtual void selfProcess_(ResponseMessage const& response) const;
	std::string const findContent_(std::string const& whole, std::string const& tagName, size_t& offset) const;

    std::string const sha1_(std::string const& value) const;
    std::string const md5_(std::string const& value) const;
    std::string const base16_(std::string const& value) const;
    std::string const unquote_(std::string const& value) const;
    std::string const rsaEncript_(R_RSA_PUBLIC_KEY const* const platformPublicKey, std::string const& deviceId, std::string const& randomKey) const;
protected:
    Communicator* communicator_;
    static int nc_;
	static std::string msisdn_;

    std::string const msisdn_d_() const;
    std::string const deviceId_() const;
    R_RSA_PUBLIC_KEY const* const platformPublicKey_() const;
    virtual std::string const resourceName_() const;
    virtual std::string const additionParameter_() const;
    std::string server_;
    std::string userAgent_;
    std::string os_;
    std::string version_;
};
