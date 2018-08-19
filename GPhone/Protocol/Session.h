#ifndef __PROTOCOL_SESSION_H__
#define __PROTOCOL_SESSION_H__

#include <string>
#include <map>
#include "../Util/Http/ResponseMessage.h"
#include "../Util/Encript/rsaeuro/source/rsaeuro.h"

namespace Util {
    namespace Http {
        class Communicator;
    }
}
//data:[MIME-type;parameters][;base64],data
//base64 or urlencode
namespace Protocol {
    class Session {
    public:
        //192.168.13.245
        Session(std::string const & server = "218.249.60.87:8088", std::string const & userAgent = "yd6618", std::string const & os = "99", std::string const & version = "1");
        ~Session();
        /*virtual */void Process(std::string const & body = "");
    protected:
        Util::Http::ResponseMessage const auth_(std::map<std::string, std::string>& values, std::string const & digestUriValue) const;
        Util::Http::ResponseMessage const timeout_() const;
        virtual void selfProcess_(Util::Http::ResponseMessage const & response) const;
        std::string const findContent_(std::string const & whole, std::string const & tagName, size_t& offset) const;
        std::string const getParameters_() const;

        std::string const sha1_(std::string const & value) const;
        std::string const md5_(std::string const & value) const;
        std::string const base16_(std::string const & value) const;
        std::string const unquote_(std::string const & value) const;
        std::string const rsaEncript_(R_RSA_PUBLIC_KEY const * const platformPublicKey, std::string const & deviceId, std::string const & randomKey) const;
    protected:
        Util::Http::Communicator* communicator_;
        static int nc_;
        static std::string msisdn_;
        std::string const msisdn_d_() const;
        std::string const deviceId_() const;
        R_RSA_PUBLIC_KEY const * const platformPublicKey_() const;
        virtual std::string const resourceName_() const;
        virtual std::string const additionParameter_() const;
        virtual std::string const alterServer_() const;
        std::string server_;
        std::string userAgent_;
        std::string os_;
        std::string version_;

        std::map<std::string, std::string> parameters_; //userAgent_, os_, version_, taskid_, ...;
        std::map<std::string, std::string> headers_;
        std::string body_;
    };
}

#endif //__PROTOCOL_SESSION_H__
