#include "Session.h"
#include "Communicator.h"
#include "../Util/StringOp.h"
#include "../Util/MD5/md5.h"
//#define MD_CTX MD5_CTX
//#define MDInit MD5Init
//#define MDUpdate MD5Update
//#define MDFinal MD5Final

int Session::nc_ = 0;

Session::Session(std::string const& server, std::string const& userAgent, std::string const& os, std::string const& version)
: server_(server)
, userAgent_(userAgent)
, os_(os)
, version_(version) {
}

Session::~Session() {
}

void Session::Process(std::string const& body) {
    /*Communicator* */communicator_ = new Communicator();
    //c->ConsultKey("deviceId", "platformPublicKey");
    RequestMessage request;
    //request.type = RequestMessage::tGet;
    request.uri = "http://" + server_ + "/" + resourceName_() + "?msisdn_d=" + msisdn_d_() + "&ua=" + userAgent_ + "&os=" + os_ + "&version=" + version_ + additionParameter_();
    request.HeaderFields.insert(std::make_pair("Content-Length", Util::StringOp::ToUTF8(Util::StringOp::FromInt(body.length()))));
    request.Body = body;
    //request.version = "HTTP/1.1";
    //request.HeaderFields.insert(std::make_pair("x-device-id", deviceId_()));
    ResponseMessage response = communicator_->Get(request);
    if (response.state == ResponseMessage::sUnauthorized) {
        //WWW-Authenticate: Digest realm="hljt@service.cmcc.cn",qop="auth",nonce="c8a46fb0dec24b7285e1b9516a7de345",opaque="17c91d3e2e9e4a74b6af2cc4c732f2d0"
        std::vector<std::string> bigParts = Util::StringOp::Split(response.HeaderFields["WWW-Authenticate"], " ");
        //bigParts[0]; //method == Digest
        std::vector<std::string> parts = Util::StringOp::Split(bigParts[1], ",");
        std::map<std::string, std::string> values;
        for (size_t i = 0; i < parts.size(); ++i) {
            std::vector<std::string> nameValuePair = Util::StringOp::Split(parts[i], "=");
            values.insert(std::make_pair(nameValuePair[0], nameValuePair[1]));
        }
        response = auth_(values, request.uri);
    } else if (response.state == ResponseMessage::sRequestTimeout) {
        response = timeout_();
    } else {
        selfProcess_(response);
    }
    delete communicator_;
}

ResponseMessage const Session::auth_(std::map<std::string, std::string>& values, std::string const& digestUriValue) const {
    ResponseMessage result;
    std::string username = userAgent_ + "&" + os_ + "&" + version_;

    char buffer[9] = {0};
    ++nc_;
    sprintf(buffer, "%08d", nc_);
    std::string nc = buffer;

    std::string cnonceValue = "1234567890";

    std::string responseValue = base16_(md5_(base16_(md5_(username + ":" + unquote_(values["realm"]) + ":" + msisdn_d_() + "123456"))
        + ":"
        + unquote_(values["nonce"])
        + ":"
        + nc
        + ":"
        + cnonceValue
        + ":"
        + unquote_(values["qop"])
        + ":"
        + base16_(md5_("GET:" + digestUriValue))));
    
    std::string authInfo = "Digest username=\"" + username
        + "\",realm=" + values["realm"]
        + ",nonce=" + values["nonce"]
        + ",uri=\"" + digestUriValue
        + "\",qop=" + values["qop"]
        + ",nc=" + nc
        + ",cnonce=\"" + cnonceValue
        + "\",response=\"" + responseValue
        + "\",opaque=" + values["opaque"];
    RequestMessage request;
    request.uri = "http://" + server_ + "/" + resourceName_() + "?msisdn_d=" + msisdn_d_() + "&ua=" + userAgent_ + "&os=" + os_ + "&version=" + version_;
    request.HeaderFields.insert(std::make_pair("Authorization", authInfo));
    result = communicator_->Get(request);
    return result;

    //Authentication-Info: qop="auth",nextnonce="dcd98b7102d34fd0e8b11aabc0bf762",rspauth="6629fae49393a053974509376845abcd",nc=00000001,cnonce="0a4f113b"
    std::vector<std::string> parts = Util::StringOp::Split(result.HeaderFields["WWW-Authenticate"], ",");
    values.clear();
    for (size_t i = 0; i < parts.size(); ++i) {
        std::vector<std::string> nameValuePair = Util::StringOp::Split(parts[i], "=");
        values.insert(std::make_pair(nameValuePair[0], nameValuePair[1]));
    }
    //end of register
    /*
    std::string username = userAgent_ + "&" + os_ + "&" + version_;
    //std::string nc = "00000001";
    char buffer[9] = {0};
    ++nc_;
    sprintf(buffer, "%08d", nc_);
    std::string nc = buffer;

    std::string cnonceValue = "1234567890";

    //std::string digestUriValue = "";
    //std::string a = base16_(md5_(username + ":" + unquote_(values["realm"]) + ":" + msisdn_d_() + "123456"));
    //std::string b = base16_(md5_("GET:" + request.uri));
    std::string responseValue = base16_(md5_(base16_(md5_(username + ":" + unquote_(values["realm"]) + ":" + msisdn_d_() + "123456"))
        + ":"
        + unquote_(values["nonce"])
        + ":"
        + nc
        + ":"
        + cnonceValue
        + ":"
        + unquote_(values["qop"])
        + ":"
        + base16_(md5_("GET:" + request.uri))));
    
    std::string authInfo = "Digest username=\"" + username
        + "\",realm=" + values["realm"]
        + ",nonce=" + values["nonce"]
        + ",uri=\"" + request.uri
        + "\",qop=" + values["qop"]
        + ",nc=" + nc
        + ",cnonce=\"" + cnonceValue
        + "\",response=\"" + responseValue
        + "\",opaque=" + values["opaque"];
    request.HeaderFields.insert(std::make_pair("Authorization", authInfo));
    ResponseMessage res = c->Get(request);
    //Authentication-Info: qop="auth",nextnonce="dcd98b7102d34fd0e8b11aabc0bf762",rspauth="6629fae49393a053974509376845abcd",nc=00000001,cnonce="0a4f113b"
    parts = Util::StringOp::Split(response.HeaderFields["WWW-Authenticate"], ",");
    values.clear();
    for (size_t i = 0; i < parts.size(); ++i) {
        std::vector<std::string> nameValuePair = Util::StringOp::Split(parts[i], "=");
        values.insert(std::make_pair(nameValuePair[0], nameValuePair[1]));
    }
    //end of register
    */
    //request
    //WWW-Authenticate: {Digest realm="hljt@service.cmcc.cn",qop={auth|auth-int},nonce="nonceValue",opaque="opaqueValue"}
    //Authorization: {Digest username="ua&os&version",realm="hljt@service.cmcc.cn",nonce="nonceValue",uri="http://[server-url]/register?ua=yd6618&os=99&version=1",qop="{auth|auth-int}",nc=00000001,cnonce="cnonceValue",respone="responeValue",opaque="opaqueValue"}
    //Authentication-Info: {qop="{auth|auth-int}",nextnonce="nextnonceValue",rspauth="rspauthValue",nc=00000001,cnonce="cnonceValue"}
    //Communicator* c = 0;
    //c->Send();
    //http://[server-url]/register?ua=yd6618&os=99&version=1
    //platform-public-key
    //x-device-id: {vendor[3]type[3]assembly[3]serial[8]reserved[1]}
    //nonceValue is a unique string value
    //opaqueValue is a unique string value
    //ncValue is a unique number, increament by one at once
    //cnonceValue is a unique string value
    //responeValue is a value by:
    //  md5(concat(md5(concat(unquote(usernameValue), ":" , unquote(realmValue), ":", password)),
    //          ":", concat(unquote(nonceValue), ":", ncValue, ":", unquote(cnonceValue), ":", unquote(qopValue),
    //              ":", md5(concat(Method, ":", digestUriValue[, ":", md5(entityBody)])))))
    //  md5(concat(md5(password),
    //          ":", concat(unquote(nonceValue), ":", ncValue, ":", unquote(cnonceValue))))
    //nextnonceValue is a unique string value
    //rspauthValue is a value by:
    //  md5(concat(md5(concat(unquote(usernameValue), ":" , unquote(realmValue), ":", password)),
    //          ":", concat(unquote(nextnonceValue), ":", ncValue, ":", unquote(cnonceValue), ":", unquote(qopValue),
    //              ":", md5(concat(":", digestUriValue[, ":", md5(entityBody)])))))
    //  md5(concat(md5(password),
    //          ":", concat(unquote(nextnonceValue), ":", ncValue, ":", unquote(cnonceValue),
    //              ":", md5(concat(":", digestUriValue[, ":", md5(entityBody)])))))
    //password is md5(concat(deviceId, ":", randomKey))
    //unquote(x) remove the endian quote mark
    //concat(x,y,z,...) concat the parameters to one string
    //md5(x) standard md5 algorithm, return the base16 value
    //Method is GET
    //digestUriValue maybe is uriValue's md5
    //entityBody is none
}

ResponseMessage const Session::timeout_() const {
    ResponseMessage result;
    return result;
}

void Session::selfProcess_(ResponseMessage const& response) const {
}

std::string const Session::sha1_(std::string const& value) const {
    std::string result;
    return result;
}

std::string const Session::md5_(std::string const& value) const {
    //std::string result;
    MD5_CTX context;
    MD5Init (&context);
    MD5Update (&context, reinterpret_cast<unsigned char*>(const_cast<char*>(value.c_str())), value.length());
    unsigned char digest[16];
    MD5Final (digest, &context);
    //return result;
    return std::string(reinterpret_cast<char*>(digest), 16);
}

std::string const Session::base16_(std::string const& value) const {
    std::string result;
    char buffer[3];
    for (size_t i = 0; i < value.length(); ++i) {
        memset(buffer, 0, 3);
        sprintf(buffer, "%02x", static_cast<unsigned char>(value[i]));
        result += buffer;
    }
    return result;
}

std::string const Session::unquote_(std::string const& value) const {
    //std::string result;
    //return result;
    return value.substr(1, value.length() - 2);
}

std::string const Session::msisdn_d_() const {
    return "12345678901";
}

std::string const Session::deviceId_() const {
    return "246801357933333444";
}

std::string const Session::platformPublicKey_() const {
    return "@@platformPublicKey";
}

std::string const Session::resourceName_() const {
    return "";
}

std::string const Session::additionParameter_() const {
    return "";
}