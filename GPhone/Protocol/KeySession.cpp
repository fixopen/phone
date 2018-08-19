#include "KeySession.h"
#include "../Util/StringOp.h"

namespace Protocol {
    KeySession::KeySession(void) {
    }

    KeySession::~KeySession(void) {
    }

    void KeySession::selfProcess_(Util::ResponseMessage const & response) const {
        //Util::DocSource* source = new Util::MemDocSource(Util::StringOp::FromUTF8(response.Body));
        //Util::XMLParser parser;
        //Util::shared_ptr<Xml::Document> doc = parser.Parse(source);
        //delete source;
        /*
        <?xml version="1.0" encoding="UTF-8"?>
        <newkeyresponse>
        <msisdn_d>13912345678</msisdn_d>
        <keytime>13912345678</keytime>
        <mac>11111111</mac>//Hash£¨Dkey, msisdn, keytime£©
        </newkeyresponse>
        */
        size_t offset = 0;
        Session::msisdn_ = findContent_(response.Body, "msisdn_d", offset);
    }

    std::string const KeySession::resourceName_() const {
        return "ZF3G/tic/newdkey";
    }

    std::string const KeySession::additionParameter_() const {
        std::string randomKey = "Hello, world"; //Util::StringOp::ToBinary("d37b055a17ef033f");
        std::string r = Util::StringOp::ToBase16("abcd");
        char buffer[64] = {
            0
        };
        sprintf(buffer, "%d", 0x61626364);
        return "&edkey=" + rsaEncript_(platformPublicKey_(), deviceId_(), randomKey);
    }
}
