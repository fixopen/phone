#ifndef __UTIL_OTAMESSAGE_H__
#define __UTIL_OTAMESSAGE_H__

#include "../../Util/Data.h"
#include "OTAData.h"

namespace SMS {
    namespace Data {
        struct OtaMessage : Util::DataAccess<OtaMessage> {
            static std::vector<Util::shared_ptr<OtaMessage> > const Select(std::wstring const& filter);
            void Update() const; //sync to database
            void Insert(); //insert new instance to database
            void Remove() const; //delete self from database
            static void Remove(std::wstring const& filter); //delete from database
            static OtaMessage const Parse(std::string const& content);

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

            //////////////////////////////////////////////////////////////////////
            enum State
            {
                stUnDownload,
                stDownloaded,
                stUnParse,
                stParsed,
            };
            State state;
            Type instruction;
            std::wstring url;
            //////////////////////////////////////////////////////////////////////            	
        private:
            static void modifyFieldByDB_(int argc, char** argv, char** columnName, Util::shared_ptr<OtaMessage> item);
        };
    }
}

#endif //__UTIL_MESSAGE_H__
