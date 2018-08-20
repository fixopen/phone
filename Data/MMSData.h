#ifndef __SMS_DATA_MMSDATA_H__
#define __SMS_DATA_MMSDATA_H__

#include "Data.h"

//namespace SMS {
    namespace Data {
        class MMSData : public DataAccess<MMSData>
        {
        public:
          	static std::vector<boost::shared_ptr<MMSData> > GetFromDatabase(std::string filter, Direction const dir = dNull, int const id = 0, int const pageSize = 0);
            static std::vector<boost::shared_ptr<MMSData> > GetFromDatabaseByTypeOffsetLength(std::string type, int const offset, int const pageSize);
			static boost::shared_ptr<MMSData> GetDataById(int id);
            void Update() const; //sync to database
            void Insert(); //insert new instance to database
            void Remove() const; //delete self from database
            static void Remove(std::string const& filter); //delete from database
            static MMSData const Parse(std::string const& content);

	        std::string const ToString() const;
            //head
            std::string MessageType; //m-send-req = <Octet 128>
            std::string TransactionId;
            std::string MMSVersion; //MMS-version-value = Short-integer
            std::string RecipientAddress; //To-value = Encoded-string-value
            std::string ContentType;
            std::string SenderAddress; //Optional From-value = Value-length (Address-present-token Encoded-string-value | Insert-address-token )Address-present-token = <Octet 128>Insert-address-token = <Octet 129>
            unsigned int TimeOfExpiry; //Expiry-value = Value-length (Absolute-token Date-value | Relative-token Delta-seconds-value)Absolute-token = <Octet 128>	Relative-token = <Octet 129>
            //	char DeliveryReport;//Delivery-report-value = Yes | No	Yes = <Octet 128>		No = <Octet 129>
            //	char MMState;//Status-value = Expired | Retrieved | Rejected | Deferred | Unrecognised	Expired = <Octet 128>		Retrieved = <Octet 129>		Rejected = <Octet 130>		Deferred = <Octet 131>		Unrecognised = <Octet 132>		The value Unrecognized is reserved for version management purpose only.
            std::string Subject; //Encoded-string-value


            std::string MessageId; //Message-ID-value = Text -string
            std::string MessageClass; //Message-class-value = Class-identifier | Token-text	Class-identifier = Personal | Advertisement | Informational | Auto		Personal = <Octet 128>		Advertisement = <Octet 129>		Informational = <Octet 130>		Auto = <Octet 131>
            unsigned int DateAndTime; //Date-value = Long-integer	In seconds from 1970-01-01, 00:00:00 GMT.
            std::string RequestStatus;

            std::string SavePath;

            bool isRead;
            enum Type
            {
                tpReceive,
                tpSend,
                tpUnSend,
				tpSpecod,       //特殊短信
				tpReadySend		//正在发送
            };
            Type type;
        private:
            static void modifyFieldByDB_(int argc, char** argv, char** columnName, boost::shared_ptr<MMSData> item);
        };
    }
//}

#endif //__SMS_DATA_MMSDATA_H__
