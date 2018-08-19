#include "RegisterSession.h"
#include "../Util/RequestMessage.h"
#include "../Util/ResponseMessage.h"
#include "../Util/Communicator.h"
#include "../Util/StringOp.h"

namespace Protocol {
    RegisterSession::RegisterSession(void) {
    }

    RegisterSession::~RegisterSession(void) {
    }

    //void RegisterSession::Process() {
    //    Communicator* c = new Communicator();
    //    //c->ConsultKey("deviceId", "platformPublicKey");
    //    RequestMessage request;
    //    //request.type = RequestMessage::tGet;
    //    request.uri = "http://" + server_ + "/" + resourceName_() + "?msisdn_d=" + msisdn_d_() + "&ua=" + userAgent_ + "&os=" + os_ + "&version=" + version_;
    //    //request.version = "HTTP/1.1";
    //    //request.HeaderFields.insert(std::make_pair("x-device-id", deviceId_()));
    //    ResponseMessage response = c->Get(request);
    //    if (response.state == ResponseMessage::sUnauthorized) {
    //        //WWW-Authenticate: Digest realm="hljt@service.cmcc.cn",qop="auth",nonce="c8a46fb0dec24b7285e1b9516a7de345",opaque="17c91d3e2e9e4a74b6af2cc4c732f2d0"
    //        std::vector<std::string> bigParts = Util::StringOp::Split(response.HeaderFields["WWW-Authenticate"], " ");
    //        //bigParts[0]; //method == Digest
    //        std::vector<std::string> parts = Util::StringOp::Split(bigParts[1], ",");
    //        std::map<std::string, std::string> values;
    //        for (size_t i = 0; i < parts.size(); ++i) {
    //            std::vector<std::string> nameValuePair = Util::StringOp::Split(parts[i], "=");
    //            values.insert(std::make_pair(nameValuePair[0], nameValuePair[1]));
    //        }
    //        response = auth_(values, request.uri);
    //    } else if (response.state == ResponseMessage::sRequestTimeout) {
    //        response = timeout_();
    //    } else {
    //        selfProcess_(response);
    //    }
    //    delete c;
    //}

    std::string const RegisterSession::resourceName_() const {
        return "ZF3G/tic/register";
        //return "ZPA/register";
    }
}
