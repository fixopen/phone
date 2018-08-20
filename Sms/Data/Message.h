#ifndef __SMS_DATA_MESSAGE_H__
#define __SMS_DATA_MESSAGE_H__

#include "../../Util/Data.h"
#include "MessageUtil.h"

namespace SMS {
    namespace Data {
        struct Message : Util::DataAccess<Message> {
            static std::vector<Util::shared_ptr<Message> > const Select(std::wstring const& filter);
            void Update() const; //sync to database
            void Insert(); //insert new instance to database
            void Remove() const; //delete self from database
            static void Remove(std::wstring const& filter); //delete from database
            static Message const Parse(std::string const& content);

            std::string const ToString() const;
            Endpoint smsc;
            Flag flag;
            unsigned char reference; //00 or 19
            Endpoint remote;
            unsigned char uplevelProtocol; //00 GSM p2p
            Encoding encoding;
            //unsigned char encoding; //GSM03.38
            //union {
                unsigned char validityPeriod; //FF == max
                SMSCTimestamp timestamp;
            //} timeInfo;
            //std::string defaultData;
            //std::string binaryData;
            std::wstring unicodeData;
            enum State {
                sNoRead,
                sReaded,
            };
            State state;
            enum Group
            {
            	gReceive,
            	gSend,
            	gUnSend
            };
            Group group;
        private:
            static void modifyFieldByDB_(int argc, char** argv, char** columnName, Util::shared_ptr<Message>& item);
        };
    }
}

#endif //__SMS_DATA_MESSAGE_H__
