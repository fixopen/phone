#ifndef __SMS_DATA_MMSDATA_H__
#define __SMS_DATA_MMSDATA_H__

#include "../../Util/Data.h"

namespace SMS {
    namespace Data {
        class MMSData : public Util::DataAccess<MMSData>
        {
        public:
            MMSData() {}
            MMSData(const MMSData& mms) {}
            static std::vector<Util::shared_ptr<MMSData> > const Select(std::wstring const& filter);
            void Update() const; //sync to database
            void Insert(); //insert new instance to database
            void Remove() const; //delete self from database
            static void Remove(std::wstring const& filter); //delete from database
            static MMSData const Parse(std::string const& content);

            std::string const ToString() const;
            //head
            std::wstring MessageType; //m-send-req = <Octet 128>
            std::wstring TransactionId;
            std::wstring MMSVersion; //MMS-version-value = Short-integer
            std::wstring RecipientAddress; //To-value = Encoded-string-value
            std::wstring ContentType;
            std::wstring SenderAddress; //Optional From-value = Value-length (Address-present-token Encoded-string-value | Insert-address-token )Address-present-token = <Octet 128>Insert-address-token = <Octet 129>
            unsigned int TimeOfExpiry; //Expiry-value = Value-length (Absolute-token Date-value | Relative-token Delta-seconds-value)Absolute-token = <Octet 128>	Relative-token = <Octet 129>
            //	char DeliveryReport;//Delivery-report-value = Yes | No	Yes = <Octet 128>		No = <Octet 129>
            //	char MMState;//Status-value = Expired | Retrieved | Rejected | Deferred | Unrecognised	Expired = <Octet 128>		Retrieved = <Octet 129>		Rejected = <Octet 130>		Deferred = <Octet 131>		Unrecognised = <Octet 132>		The value Unrecognized is reserved for version management purpose only.
            std::wstring Subject; //Encoded-string-value


            std::wstring MessageId; //Message-ID-value = Text -string
            std::wstring MessageClass; //Message-class-value = Class-identifier | Token-text	Class-identifier = Personal | Advertisement | Informational | Auto		Personal = <Octet 128>		Advertisement = <Octet 129>		Informational = <Octet 130>		Auto = <Octet 131>
            unsigned int DateAndTime; //Date-value = Long-integer	In seconds from 1970-01-01, 00:00:00 GMT.
            std::wstring RequestStatus;

            std::wstring SavePath;

            bool isRead;
            enum Type
            {
                tpReceive,
                tpSend,
                tpUnSend
            };
            Type type;
        private:
            static void modifyFieldByDB_(int argc, char** argv, char** columnName, Util::shared_ptr<MMSData> item);
        };
    }
}

#endif //__SMS_DATA_MMSDATA_H__
