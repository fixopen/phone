#include "InitializeSession.h"
#include "../Util/Communicator.h"
#include "../Util/StringOp.h"
#include "../Util/XMLParser.h"

namespace Protocol {
    InitializeSession::InitializeSession(void) {
    }

    InitializeSession::~InitializeSession(void) {
    }

    //void InitializeSession::Process() {
    //    Communicator* c = new Communicator();
    //    RequestMessage request;
    //    request.uri = "http://" + server_ + "/" + resourceName_() + "?msisdn_d=" + msisdn_d_() + "&ua=" + userAgent_ + "&os=" + os_ + "&version=" + version_;
    //    ResponseMessage response = c->Get(request);
    //    if (response.state == ResponseMessage::sUnauthorized) {
    //        std::vector<std::string> bigParts = Util::StringOp::Split(response.HeaderFields["WWW-Authenticate"], " ");
    //        std::vector<std::string> parts = Util::StringOp::Split(bigParts[1], ",");
    //        std::map<std::string, std::string> values;
    //        for (size_t i = 0; i < parts.size(); ++i) {
    //            std::vector<std::string> nameValuePair = Util::StringOp::Split(parts[i], "=");
    //            values.insert(std::make_pair(nameValuePair[0], nameValuePair[1]));
    //        }
    //        std::string username = userAgent_ + "&" + os_ + "&" + version_;
    //        char buffer[9] = {0};
    //        ++nc_;
    //        sprintf(buffer, "%08d", nc_);
    //        std::string nc = buffer;
    //
    //        std::string cnonceValue = "1234567890";
    //
    //        //std::string digestUriValue = "";
    //        //std::string a = base16_(md5_(username + ":" + unquote_(values["realm"]) + ":" + msisdn_d_() + "123456"));
    //        //std::string b = base16_(md5_("GET:" + request.uri));
    //        std::string responseValue = base16_(md5_(base16_(md5_(username + ":" + unquote_(values["realm"]) + ":" + msisdn_d_() + "123456"))
    //            + ":"
    //            + unquote_(values["nonce"])
    //            + ":"
    //            + nc
    //            + ":"
    //            + cnonceValue
    //            + ":"
    //            + unquote_(values["qop"])
    //            + ":"
    //            + base16_(md5_("GET:" + request.uri))));
    //        
    //        std::string authInfo = "Digest username=\"" + username
    //            + "\",realm=" + values["realm"]
    //            + ",nonce=" + values["nonce"]
    //            + ",uri=\"" + request.uri
    //            + "\",qop=" + values["qop"]
    //            + ",nc=" + nc
    //            + ",cnonce=\"" + cnonceValue
    //            + "\",response=\"" + responseValue
    //            + "\",opaque=" + values["opaque"];
    //        request.HeaderFields.insert(std::make_pair("Authorization", authInfo));
    //        ResponseMessage res = c->Get(request);
    //        //Authentication-Info: qop="auth",nextnonce="dcd98b7102d34fd0e8b11aabc0bf762",rspauth="6629fae49393a053974509376845abcd",nc=00000001,cnonce="0a4f113b"
    //        parts = Util::StringOp::Split(response.HeaderFields["WWW-Authenticate"], ",");
    //        values.clear();
    //        for (size_t i = 0; i < parts.size(); ++i) {
    //            std::vector<std::string> nameValuePair = Util::StringOp::Split(parts[i], "=");
    //            values.insert(std::make_pair(nameValuePair[0], nameValuePair[1]));
    //        }
    //        //end of register
    //    } else if (response.state == ResponseMessage::sRequestTimeout) {
    //        //
    //    } else {
    //        std::string b = response.Body;
    //        //parseBody(b);
    //    }
    //}

    void InitializeSession::selfProcess_(Util::ResponseMessage const & response) const {
        Util::DocSource* source = new Util::MemDocSource(Util::StringOp::FromUTF8(response.Body));
        Util::XMLParser parser;
        Util::shared_ptr<Xml::Document> doc = parser.Parse(source);
        delete source;
        /*
        <?xml version="1.0" encoding="UTF-8"?>
        <initialresponse>
        <msisdn>13912345678</msisdn>
        <username>13912345678</username>
        <password>11111111</password>
        <services>
        <service>
        <servicename>互联家庭业务</servicename>
        <servicecode>001</servicecode>
        <pack>
            <feetype>单条计费套餐</feetype>
            <serviceid>001001</serviceid>
            <servicerate>0</servicerate>
            <order>0</order>
        </pack>
        <pack>
            <feetype>5元包月套餐</feetype>
            <serviceid>001002</serviceid>
            <servicerate>20</servicerate>
            <order>1</order>
        </pack>
        <welcomeinfo>欢迎使用互联家庭业务</welcomeinfo>
        </service>
        </services>
        </initialresponse>
        */
    }

    std::string const InitializeSession::resourceName_() const {
        return "ZF3G/tic/initialize";
    }
}
