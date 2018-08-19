#include "StockSession.h"

namespace Protocol {
    StockSession::StockSession(void) {
    }

    StockSession::~StockSession(void) {
    }

    void StockSession::SetParameter(std::string const & newUrl) {
        newUrl_ = newUrl;
    }

    void StockSession::selfProcess_(Util::ResponseMessage const & response) const {
        /*
        <?xml version="1.0" encoding="UTF-8"?>
        <stockinforesponse>
        <code>000969</code>
        <name>°²Ì©¿Æ¼¼</name>
        <toprice>26.03</toprice>
        <ycprice>26.03</ycprice>
        <tmaxprice>26.03</tmaxprice>
        <tminprice>26.03</tminprice>
        <tradingvolumn>4697</tradingvolumn>
        <tradingprice>26.03</tradingprice>
        <b1volumn>4697</b1volumn>
        <b1price>26.03</b1price>
        <b2volumn>4697</b2volumn>
        <b2price>26.03</b2price>
        <b3volumn>4697</b1vo3umn>
        <b3price>26.03</b3price>
        <b4volumn>4697</b4volumn>
        <b4price>26.03</b4price>
        <b5volumn>4697</b5volumn>
        <b5price>26.03</b5price>
        <s1volumn>4697</s1volumn>
        <s1price>26.03</s1price>
        <s2volumn>4697</s2volumn>
        <s2price>26.03</s2price>
        <s3volumn>4697</s1vo3umn>
        <s3price>26.03</s3price>
        <s4volumn>4697</s4volumn>
        <s4price>26.03</s4price>
        <s5volumn>4697</s5volumn>
        <s5price>26.03</s5price>
        <date>2009-10-18</date>
        <time>15:05:00</time>
        </stockinforesponse>
        */
    }

    std::string const StockSession::resourceName_() const {
        return "ZF3G/tic/parameter";
    }

    std::string const StockSession::additionParameter_() const {
        return "stockcode=XXX";
    }

    std::string const StockSession::alterServer_() const {
        return newUrl_;
    }
}
