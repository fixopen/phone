#ifndef __UTIL_MESSAGE_H__
#define __UTIL_MESSAGE_H__

#include "../../Util/Data.h"

namespace SMS {
    namespace Data {
        std::string const TrimF(std::string const& v);
        std::string const Reverse(std::string const& v);
        std::string const PaddingAndReverse(std::string const& v);
        struct AddressType {
            enum NumberType {
                tUnknown = 0, //000
                tInternationNumber = 1, //001
                tNationNumber = 2, //010
                tNetworkSpecNumber = 3, //011
                tSubscribeNumber = 4, //100
                tAlphaNumeric = 5, //101 TS03.38 7-bit default alphabet
                tAbbreviatedNumber = 6, //110
                tReserved = 7, //111
            };
            enum Indentification {
                iUnknown = 0, //0000
                iIsdnTelephoneNumber = 1, //0001 E.164/E.163
                iDataNumber = 3, //0011 X.121
                iTelexNumber = 4, //0100
                iNationNumber = 8, //1000
                iPrivateNumber = 9, //1001
                iERMESNumber = 10, //1010 ETSI (DE/PS301-1)
                iReserved = 15, //1111
            };
            AddressType();
            AddressType(unsigned char v);
            unsigned char const ToString() const;
            bool mustSet;
            NumberType typeOfNumber;
            Indentification indentification;
        };
        struct Endpoint {
            std::string const ToString() const;
            size_t addressLength;
            AddressType addressType;
            std::string address;
        };
        struct Message : Util::DataAccess<Message> {
            static std::vector<Util::shared_ptr<Message> > const Select(std::wstring const& filter);
            void Update() const; //sync to database
            void Insert(); //insert new instance to database
            void Remove() const; //delete self from database
            static void Remove(std::wstring filter); //delete from database
            static Message const Parse(std::string const& content);

            std::string const ToString() const;
            Endpoint smsc;
            struct Flag{
                enum Type {
                    tDeliver = 0, //00 SC ==> MS
                    tDeliverReport = 0, //00 MS ==> SC
                    tSubmit = 1, //01 MS ==> SC
                    tSubmitReport = 1, //01 SC ==> MS
                    tStatusReport = 2, //10 SC == MS
                    tCommand = 2, //10 MS ==> SC
                    tReserved = 3, //11
                };
                enum ValidityPeriodFormat {
                    pNotPresent = 0, //00
                    pReserved = 1, //01
                    pRelativePresent = 2, //10 integer
                    pAbsolutePresent = 3, //11 semi-octect
                };
                Flag(Type const atype = tDeliver);
                Flag(unsigned char v);
                unsigned char const ToString() const;
                bool isSetReplyPath; //RP
                bool hasHeaderInfo; //UDHI   EMS include header info, text not include
                bool requestReport; //SRR
                ValidityPeriodFormat validityPeriodFormat; //VPF
                bool rejectCopy; //RD MMS
                Type type; //MTI
            };
            Flag flag;
            unsigned char reference; //00 or 19
            Endpoint remote;
            unsigned char uplevelProtocol; //00 GSM p2p
            struct Encoding {
                enum EncodingCode {
                    DefaultAlphabet = 0, //00
                    OctetBit = 1, //01
                    UnicodeCodeSet = 2, //10
                    Reserved = 3, //11 reserved
                };
                Encoding(EncodingCode aencoding = UnicodeCodeSet);
                Encoding(unsigned char v);
                unsigned char const ToString() const;
                //unsigned char high : 4; //reserved
                EncodingCode encoding;// : 2;
                //unsigned char low : 2; //reserved
            } encoding;
            //unsigned char encoding; //GSM03.38
            //union {
                unsigned char validityPeriod; //FF == max
                struct SMSCTimestamp {
                    SMSCTimestamp(std::string const& v = "");
                    std::string const ToString() const;
                    unsigned char year;
                    unsigned char month;
                    unsigned char day;
                    unsigned char hour;
                    unsigned char minite;
                    unsigned char second;
                    unsigned char timezone;
                } timestamp;
            //} timeInfo;
            std::wstring data;
        private:
            static void modifyFieldByDB_(int argc, char** argv, char** columnName, Util::shared_ptr<Message> item);
        };
    }
}

#endif //__UTIL_MESSAGE_H__
