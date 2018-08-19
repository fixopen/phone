#ifndef __UTIL_MESSAGE_H__
#define __UTIL_MESSAGE_H__

#include "Data.h"
#include "OTAData.h"

//namespace SMS {
    namespace Data {
        class Message : public DataAccess<Message> {
		public:
            static std::vector<boost::shared_ptr<Message> > GetFromDatabase(std::string filter, Direction const dir = dNull, int const id = 0, int const pageSize = 0);
            static std::vector<boost::shared_ptr<Message> > GetFromDatabaseByTypeOffsetLength(std::string type, int const offset, int const pageSize);
			static boost::shared_ptr<Message> GetDataById(int id);
			void Update() const; //sync to database
            void Insert(); //insert new instance to database
            void Remove() const; //delete self from database
            static void Remove(std::string const& filter); //delete from database
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
            std::string unicodeData;
            enum State {
                sNoRead,
                sReaded,
            };
            State state;
            enum Group
            {
            	gReceive,
            	gSend,
            	gUnSend,		//草稿
				gReMoteSMS,		//家庭远程留言
				gExternSms		//家庭百事通长短信      
            };
            Group group;

			std::string transactionId;
			int total;
			int no;

        private:
            static void modifyFieldByDB_(int argc, char** argv, char** columnName, boost::shared_ptr<Message> item);
        };
    }
//}

#endif //__UTIL_MESSAGE_H__
