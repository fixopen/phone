#ifndef __MMS_DATA_MMSMESSAGE_H__
#define __MMS_DATA_MMSMESSAGE_H__

#include "../../Util/Data.h"
#include "../../Util/Video/Graphics.h"
#include <map>

namespace Mms {
    namespace Data {
        enum MessageType {
            m_send_req = 0x80,
            m_send_conf = 0x81,
            m_notification_ind = 0x82,
            m_notifyresp_ind = 0x83,
            m_retrieve_conf = 0x84,
            m_acknowledge_ind = 0x85,
            m_delivery_ind = 0x86,
            m_read_rec_ind = 0x87,
            m_read_orig_ind = 0x88,
        };
        enum ContentType {
            any = 0x80,
            text_any = 0x81,
            text_html = 0x82,
            text_plain = 0x83,
            text_x_hdml = 0x84,
            text_x_ttml = 0x85,
            text_x_vCalendar = 0x86,
            text_x_vCard = 0x87,
            text_vnd_wap_wml = 0x88,
            text_vnd_wap_wmlscript = 0x89,
            application_vnd_wap_catc = 0x8A,
            multipart_any = 0x8B,
            multipart_mixed = 0x8C,
            multipart_form_data = 0x8D,
            multipart_byteranges = 0x8E,
            multipart_alternative = 0x8F,
            application_any = 0x90,
            application_java_vm = 0x91,
            application_x_www_form_urlencoded = 0x92,
            application_x_hdmlc = 0x93,
            application_vnd_wap_wmlc = 0x94,
            application_vnd_wap_wmlscriptc = 0x95,
            application_vnd_wap_wsic = 0x96,
            application_vnd_wap_uaprof = 0x97,
            application_vnd_wap_wtls_ca_certificate = 0x98,
            application_vnd_wap_wtls_user_certificate = 0x99,
            application_x_x509_ca_cert = 0x9A,
            application_x_x509_user_cert = 0x9B,
            image_any = 0x9C,
            image_gif = 0x9D,
            image_jpeg = 0x9E,
            image_tiff = 0x9F,
            image_png = 0xA0,
            image_vnd_wap_wbmp = 0xA1,
            application_vnd_wap_multipart_any = 0xA2,
            application_vnd_wap_multipart_mixed = 0xA3,
            application_vnd_wap_multipart_form_data = 0xA4,
            application_vnd_wap_multipart_byteranges = 0xA5,
            application_vnd_wap_multipart_alternative = 0xA6,
            application_xml = 0xA7,
            text_xml = 0xA8,
            application_vnd_wap_wbxml = 0xA9,
        };
        enum ChunkHeadName {
            Accept = 0x80,
            Accept_Charset = 0x81,
            Accept_Encoding = 0x82,
            Accept_Language = 0x83,
            Accept_Ranges = 0x84,
            Age = 0x85,
            Allow = 0x86,
            Authorization = 0x87,
            Cache_Control = 0x88,
            Connection = 0x89,
            Content_Base = 0x8A,
            Content_Encoding = 0x8B,
            Content_Language = 0x8C,
            Content_Length = 0x8D,
            Content_Location = 0x8E,
            Content_MD5 = 0x8F,
            Content_Range = 0x90,
            Content_Type_Single = 0x91,
            Date_Single = 0x92,
            Etag = 0x93,
            Expires = 0x94,
            From_Single = 0x95,
            Host = 0x96,
            If_Modified_Since = 0x97,
            If_Match = 0x98,
            If_None_Match = 0x99,
            If_Range = 0x9A,
            If_Unmodified_Since = 0x9B,
            Location = 0x9C,
            Last_Modified = 0x9D,
            Max_Forwards = 0x9E,
            Pragma = 0x9F,
            Proxy_Authenticate = 0xA0,
            Proxy_Authorization = 0xA1,
            Range = 0xA3,
            Referer = 0xA4,
            Retry_After = 0xA5,
            Server = 0xA6,
            Transfer_Encoding = 0xA7,
            Upgrade = 0xA8,
            User_Agent = 0xA9,
            Vary = 0xAA,
            Via = 0xAB,
            Warning = 0xAC,
            WWW_Authenticate = 0xAD,
            Content_Disposition = 0xAE,
            X_Wap_Application_Id = 0xAF,
            X_Wap_Content_URI = 0xB0,
            X_Wap_Initiator_URI = 0xB1,
            Accept_Application = 0xB2,
            Bearer_Indication = 0xB3,
            Push_Flag = 0xB4,
            Profile = 0xB5,
            Profile_Diff = 0xB6,
            Profile_Warning = 0xB7,
            Content_ID = 0xC0,
        };
        enum Fit {
            fHide,
            fFill,
            fMeet,
            fScroll,
            fSlice,
        };
        struct SmilRegion {
            Util::Video::Size root;
            Util::Video::Rect bounds;
            Fit fit;
        };
        enum MediaType {
            mtText,
            mtImage,
            mtAudio,
            mtVideo,
        };
        struct SmilParallel {
            SmilRegion* region;
            MediaType mediaType;
            std::string name;
            std::string uri;
            std::string alt;
        };
        struct Smil {
            static Smil const Parse(unsigned char const * const data, size_t const maxLength, size_t& currentPos);
            std::string const ToString() const;

            std::vector<SmilRegion> layout;
            std::map<Util::TimeSpan, SmilParallel> contents;
        };
        struct Chunk {
            //@@[ContentTypeLength + otherHeaderLength][dataLength][ContentType + OtherHeader][data]
            static Chunk const Parse(unsigned char const * const data, size_t const maxLength, size_t& currentPos);
            std::string const ToString() const;

            ContentType type;
            std::string typeName; //length + value + 0
            std::string contentId; //"value + 0
            std::string contentLocation; //value + 0
        };
        //union ContrainedMedia {
        //    std::string extensionMedia;
        //    size_t value;
        //};
        //union GeneralMediaHeader {
        //    size_t wellKnownMedia;
        //    std::string extensionMedia;
        //};
        //struct TypedParameter {
        //    token;
        //    value;
        //};
        //union UntypeValue {
        //    size_t integerValue;
        //    std::string stringValue;
        //};
        //struct UntypedParameter {
        //    token;
        //    UntypedValue value;
        //};
        //union Parameter {
        //    typedParameter;
        //    untypedParamter;
        //};
        //struct GeneralMedia {
        //    GeneralMediaHeader media;
        //    std::vector<Parameter> parameters;
        //};
        //union ContentTypeValue {
        //    ConstrainedMedia constrainedMedia;
        //    GeneralMedia generalMedia;
        //};
        struct MmsMessage : Util::DataAccess<MmsMessage> {
            static std::vector<Util::shared_ptr<MmsMessage> > const Select(std::wstring const & filter);
            void Update() const; //sync to database
            void Insert(); //insert new instance to database
            void Remove() const; //delete self from database
            static void Remove(std::wstring const & filter); //delete from database
            static MmsMessage const Parse(unsigned char const * const data, size_t const length, size_t& currentPos);

            std::string const ToString() const;

            MessageType type; //m-retrieve-conf = <Octet 132>
            std::string transactionId; //Optional
            std::string uri;
            size_t size;
            unsigned char version;
            std::string messageId; //Message-ID-value = Text-string
            std::string senderAddress;//Optional
            ContentType contentType;
            std::string contentTypeName;
            std::string multipartRelatedType;
            std::string contentId;
            std::string recipientAddress;//Optional
            unsigned char messageClass;
            unsigned char readReport;
            unsigned int dateAndTime;
            std::string subject;
            unsigned char priority;
            unsigned char deliveryReport;
            unsigned char requestStatus;
            //unsigned char state;
            std::vector<Chunk> chunks;
        private:
            static bool const isKnownedType_(unsigned char const type);
            static void modifyFieldByDB_(int argc, char** argv, char** columnName, Util::shared_ptr<MmsMessage>& item);
        };
    }
}

#endif //__MMS_DATA_MMSMESSAGE_H__
