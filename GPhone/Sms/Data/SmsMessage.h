#ifndef __SMS_DATA_SMSMESSAGE_H__
#define __SMS_DATA_SMSMESSAGE_H__

#include "../../Util/Data.h"
#include "MessageUtil.h"

namespace Sms {
    namespace Data {
        enum PduType {
            tConnect = 0x01,
            tConnectReply = 0x02,
            tRedirect = 0x03,
            tReply = 0x04,
            tDisconnect = 0x05,
            tPush = 0x06,
            tConfirmedPush = 0x07,
            tSuspend = 0x08,
            tResume = 0x09,
            tGet = 0x40,
            tOptions_ = 0x41,
            tHead_ = 0x42,
            tDelete_ = 0x43,
            tTrace_ = 0x44,
            tPost = 0x60,
            tPut_ = 0x61,
            tDataFragment = 0x80,
        };
        enum Type {
            tSms,
            tMms,
            tOta,
            tSi,
        };
        struct SmsMessage : Util::DataAccess<SmsMessage> {
            static std::vector<Util::shared_ptr<SmsMessage> > const Select(std::wstring const & filter);
            void Update() const; //sync to database
            void Insert(); //insert new instance to database
            void Remove() const; //delete self from database
            static void Remove(std::wstring const & filter); //delete from database
            static SmsMessage const Parse(std::string const & content);

            std::string const ToString() const;
            Endpoint smsc;
            Flag flag;
            unsigned char reference; //00 or 19
            Endpoint remote;
            unsigned char uplevelProtocol; //00 GSM p2p
            Encoding encoding; //GSM03.38
            //union {
            unsigned char validityPeriod; //FF == max
            SMSCTimestamp timestamp;
            //} timeInfo;
            //union {
            //    std::string defaultData;
            //    std::string binaryData;
            //    std::wstring unicodeData;
            //} data;
            std::string rawContent;
            std::wstring content;
            Type type;
            enum State {
                sNoRead,
                sReaded,
            };
            State state;
            enum Group {
                gReceive,
                gSend,
                gUnSend
            };
            Group group;
        private:
            static void modifyFieldByDB_(int argc, char** argv, char** columnName, Util::shared_ptr<SmsMessage>& item);
        };
    }
}

#endif //__SMS_DATA_SMSMESSAGE_H__
