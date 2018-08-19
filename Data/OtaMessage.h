#ifndef __UTIL_OTAMESSAGE_H__
#define __UTIL_OTAMESSAGE_H__

#include "Data.h"
#include "OTAData.h"

//namespace SMS {
namespace Data
{
    class OtaMessage : public DataAccess<OtaMessage> {
    public:
        static std::vector<boost::shared_ptr<OtaMessage> > GetFromDatabase(std::string filter, Direction const dir = dNull, int const id = 0, int const pageSize = 0);
        void Update() const; //sync to database
        void Insert(); //insert new instance to database
        void Remove() const; //delete self from database
        static void Remove(std::string const & filter); //delete from database
        static OtaMessage const Parse(std::string const & content);

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
        enum State {
            stUnDownload,
            stDownloaded,
            stUnParse,
            stParsed,
            stDownloadedError
        };
        State state;
        Type instruction;
        std::string url;
        //////////////////////////////////////////////////////////////////////              
    private:
        static void modifyFieldByDB_(int argc, char** argv, char** columnName, boost::shared_ptr<OtaMessage> item);
    };
}
//}

#endif //__UTIL_MESSAGE_H__
