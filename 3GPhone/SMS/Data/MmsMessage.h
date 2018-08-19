#ifndef __UTIL_MESSAGE_H__
#define __UTIL_MESSAGE_H__

#include "../../Util/Data.h"
#include "OTAData.h"

namespace SMS {
    namespace Data {
        struct MmsMessage : Util::DataAccess<MmsMessage> {
            static std::vector<Util::shared_ptr<MmsMessage> > const Select(std::wstring const& filter);
            void Update() const; //sync to database
            void Insert(); //insert new instance to database
            void Remove() const; //delete self from database
            static void Remove(std::wstring const& filter); //delete from database
            static MmsMessage const Parse(std::string const& content);

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

            //////////////////////////////////////////////////////////////////////////////
            enum State
            {
                stUnDownload,
                stDownloaded,
                stUnParse,
                stParsed,
            };
            State state;

            std::wstring transactionId;
            char messageClass;
            unsigned int messageSize;
            unsigned int timeOfExpiry;
            std::wstring url;

            //////////////////////////////////////////////////////////////////////////////            	
        private:
            static void modifyFieldByDB_(int argc, char** argv, char** columnName, Util::shared_ptr<MmsMessage> item);
        };
    }
}

#endif //__UTIL_MESSAGE_H__
