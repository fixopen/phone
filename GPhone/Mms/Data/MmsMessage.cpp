#include "MmsMessage.h"
#include "../../Util/Text/XMLParser.h" //for XMLParser
#include "../../Util/Scanner.h" //for Scanner util

namespace Util {
    std::wstring Mms::Data::MmsMessage::tableName_ = L"mmsMessage";
    int Mms::Data::MmsMessage::count_ = 0;
    int Mms::Data::MmsMessage::offset_ = 0;
}

namespace Mms {
    namespace Data {
        enum HeaderFieldName {
            Bcc = 0x81,
            Cc = 0x82,
            Mms_Content_Location = 0x83,
            Content_Type = 0x84,
            Date = 0x85,
            Mms_Delivery_Report = 0x86,
            Mms_Delivery_Time = 0x87,
            Mms_Expiry = 0x88,
            From = 0x89,
            Message_Class = 0x8A,
            Message_ID = 0x8B,
            Message_Type = 0x8C,
            Version = 0x8D,
            Mms_Message_Size = 0x8E,
            Mms_Priority = 0x8F,
            Mms_Read_Report = 0x90,
            Mms_Report_Allowed = 0x91,
            Mms_Response_Status = 0x92,
            Mms_Response_Text = 0x93,
            Mms_Sender_Visibility = 0x94,
            Subject = 0x96,
            To = 0x97,
            Transaction_ID = 0x98,
            Mms_Retrieve_Status = 0x99,
            Mms_Retrieve_Text = 0x9A,
            Mms_Read_Status = 0x9B,
            Mms_Previously_Sent_By = 0xA0,
            Mms_Previously_Sent_Date = 0xA1,
            Mms_Store = 0xA2,
            Mms_State = 0xA3,
            Mms_Store_Status = 0xA5,
            Mms_Store_Status_Text = 0xA6,
            Mms_Stored = 0xA7,
            Mms_Distribution_Indicator = 0xB1,
            Mms_Element_Descriptor = 0xB2,
        };
        std::vector<Util::shared_ptr<MmsMessage> > const MmsMessage::Select(std::wstring const & filter) {
            return GetDatasByFilter(filter, modifyFieldByDB_);
        }

        void MmsMessage::Update() const {
            std::wstring cmd;
            ExecCommand(cmd);
        }

        void MmsMessage::Insert() {
            std::wstring cmd;
            ExecCommand(cmd);
            id(GetCurrentId());
        }

        void MmsMessage::Remove() const {
            std::wstring filter = L"id = " + Util::Text::StringOp::FromInt(id());
            Remove(filter);
        }

        void MmsMessage::Remove(std::wstring const & filter) {
            RemoveDatasByFilter(filter);
        }

        MmsMessage const MmsMessage::Parse(unsigned char const * const content, size_t const length, size_t& pos) {
            MmsMessage result;
            while (pos < length) {
                switch (content[pos]) {
                case Message_Type:
                    ++pos;
                    result.type = static_cast<MessageType>(content[pos]);
                    ++pos;
                    break;
                case Transaction_ID:
                    ++pos;
                    //@@1byte-id || value + 0
                    result.transactionId = Util::Scanner::ScanToken(content, length, pos);
                    break;
                case Version:
                    ++pos;
                    result.version = content[pos];
                    ++pos;
                    break;
                case Date:
                    ++pos;
                    //@@length + n | n | n | ... | n
                    //@@n * 8 * 3600
                    result.dateAndTime = Util::Scanner::ScanDate(content, length, pos);
                    break;
                case From:
                    ++pos;
                    //@@length + 0x80 + value + 0
                    result.senderAddress = Util::Scanner::ScanFrom(content, length, pos);
                    break;
                case To:
                    ++pos;
                    //@@value + 0
                    result.recipientAddress = Util::Scanner::ScanToken(content, length, pos);
                    break;
                case Cc:
                    break;
                case Bcc:
                    break;
                case Subject:
                    ++pos;
                    //@@value + 0 || 1Byte-subject
                    result.subject = Util::Scanner::ScanToken(content, length, pos);
                    break;
                case Message_Class:
                    ++pos;
                    result.messageClass = content[pos];
                    ++pos;
                    break;
                case Mms_Expiry:
                    //same as date
                    ++pos;
                    //@@Expiry + Length + Relative-token + Length + Delta-secs
                    switch (content[pos]) {
                    case 0x81:
                        {
                            ++pos;
                            size_t len = content[pos];
                            int n = 0;
                            for (size_t i = 0; i < len; ++i) {
                                ++pos;
                                unsigned char t = content[pos];
                                n = ((n << 8) | t);
                            }
                            //MMSTimeOfExpiry = n;
                        }
                        break;
                        //case 0x05:
                        //  break;
                    default:
                        {
                            ++pos;
                            size_t len = content[pos];
                            //length += 1;
                            //char buf[128] = {0};
                            //unsigned int l = gsmSerializeNumbers((const char*)pBuf + length, buf, len);
                            //std::string t(buf, l);
                            //CTime expiry(atoi(t.substr(0, 2).c_str()) + 2000, atoi(t.substr(2, 2).c_str()), atoi(t.substr(4, 2).c_str()), atoi(t.substr(6, 2).c_str()), atoi(t.substr(8, 2).c_str()), atoi(t.substr(10, 2).c_str()));
                            //CTime start(1970, 1, 1, 8, 0, 0);
                            //CTimeSpan ts = expiry - start;
                            //n = ts.GetTotalSeconds();
                            //length += len;
                        }
                        break;
                    }
                    break;
                case Mms_Delivery_Time:
                    break;
                case Mms_Priority:
                    ++pos;
                    result.priority = content[pos];
                    ++pos;
                    break;
                case Mms_Sender_Visibility:
                    break;
                case Mms_Delivery_Report:
                    ++pos;
                    result.deliveryReport = content[pos];
                    ++pos;
                    break;
                case Mms_Store:
                    break;
                case Mms_State:
                    break;
                case Mms_Stored:
                    break;
                case Mms_Response_Status:
                    break;
                case Mms_Response_Text:
                    break;
                case Message_ID:
                    ++pos;
                    //@@value + 0
                    result.messageId = Util::Scanner::ScanToken(content, length, pos);
                    break;
                case Mms_Content_Location:
                    ++pos;
                    //@@value + 0
                    result.uri = Util::Scanner::ScanToken(content, length, pos);
                    break;
                case Mms_Store_Status:
                    break;
                case Mms_Store_Status_Text:
                    break;
                case Mms_Message_Size:
                    ++pos;
                    result.size = Util::Scanner::ScanLongInteger(content, length, pos);
                    break;
                case Mms_Distribution_Indicator:
                    break;
                case Mms_Element_Descriptor:
                    break;
                case Mms_Report_Allowed:
                    break;
                case Mms_Previously_Sent_By:
                    break;
                case Mms_Previously_Sent_Date:
                    break;
                case Mms_Read_Report:
                    ++pos;
                    result.readReport = content[pos];
                    ++pos;
                    break;
                case Mms_Retrieve_Status:
                    break;
                case Mms_Retrieve_Text:
                    break;
                case Mms_Read_Status:
                    break;
                case Content_Type:
                    ++pos;
                    {
                        //@@std::pair<ContentType, std::string> t = Util::Scanner::ScanContentType(content, length, pos);
                        for (size_t i = 0; i < content[pos]; ++i) {
                            result.chunks.push_back(Chunk::Parse(content + pos, length - pos, pos));
                        }
                    }
                    break;
                default:
                    break;
                }
            }
            return result;
        }

        std::string const MmsMessage::ToString() const {
            std::string result;
            return result;
        }

        void MmsMessage::modifyFieldByDB_(int argc, char** argv, char** columnName, Util::shared_ptr<MmsMessage>& item) {
            item->id(atoi(argv[GetIndexByName(argc, columnName, "id")]));
        }

        Chunk const Chunk::Parse(unsigned char const * const content, size_t const length, size_t& pos) {
            Chunk result;
            while (pos < length) {
                switch (content[pos]) {
                case Accept:
                    break;
                case Accept_Charset:
                    break;
                case Accept_Encoding:
                    break;
                case Accept_Language:
                    break;
                case Accept_Ranges:
                    break;
                case Age:
                    break;
                case Allow:
                    break;
                case Authorization:
                    break;
                case Cache_Control:
                    break;
                case Connection:
                    break;
                case Content_Base:
                    break;
                case Content_Encoding:
                    break;
                case Content_Language:
                    break;
                case Content_Length:
                    break;
                case Content_Location:
                    break;
                case Content_MD5:
                    break;
                case Content_Range:
                    break;
                case Content_Type_Single:
                    break;
                case Date_Single:
                    break;
                case Etag:
                    break;
                case Expires:
                    break;
                case From_Single:
                    break;
                case Host:
                    break;
                case If_Modified_Since:
                    break;
                case If_Match:
                    break;
                case If_None_Match:
                    break;
                case If_Range:
                    break;
                case If_Unmodified_Since:
                    break;
                case Location:
                    break;
                case Last_Modified:
                    break;
                case Max_Forwards:
                    break;
                case Pragma:
                    break;
                case Proxy_Authenticate:
                    break;
                case Proxy_Authorization:
                    break;
                case Range:
                    break;
                case Referer:
                    break;
                case Retry_After:
                    break;
                case Server:
                    break;
                case Transfer_Encoding:
                    break;
                case Upgrade:
                    break;
                case User_Agent:
                    break;
                case Vary:
                    break;
                case Via:
                    break;
                case Warning:
                    break;
                case WWW_Authenticate:
                    break;
                case Content_Disposition:
                    break;
                case X_Wap_Application_Id:
                    break;
                case X_Wap_Content_URI:
                    break;
                case X_Wap_Initiator_URI:
                    break;
                case Accept_Application:
                    break;
                case Bearer_Indication:
                    break;
                case Push_Flag:
                    break;
                case Profile:
                    break;
                case Profile_Diff:
                    break;
                case Profile_Warning:
                    break;
                case Content_ID:
                    break;
                default:
                    break;
                }
            }
            return result;
        }

        std::string const Chunk::ToString() const {
            std::string result;
            return result;
        }

        Smil const Smil::Parse(unsigned char const * const data, size_t const length, size_t& currentPos) {
            Smil result;
            Util::Text::XMLParser p;
            Util::Text::DocSource* docSource = 0; //new MemDocSource(std::wstring const& content);
            Util::shared_ptr<Xml::Document> doc = p.Parse(docSource);
            delete docSource;
            //doc->Smil
            return result;
        }

        std::string const Smil::ToString() const {
            std::string result;
            return result;
        }
    }
}

#if 0
unsigned char flag = bareData[0];
++index;
switch (flag) {
                                case 0x83:
                                    {
                                        std::string uri = bareData.substr(index);
                                        ++index;
                                    }
                                    break;
                                case 0x88:
                                    //Expiry + Length + Relative-token + Length + Delta-secs
                                    ++index;
                                    switch (bareData[index]) {
                                case 0x81:
                                    {
                                        ++index;
                                        size_t len = bareData[index];
                                        int n = 0;
                                        for (size_t i = 0; i < len; ++i) {
                                            ++index;
                                            unsigned char t = bareData[index];
                                            n = ((n << 8) | t);
                                        }
                                        //MMSTimeOfExpiry = n;
                                    }
                                    break;
                                    //case 0x05:
                                    //  break;
                                default:
                                    {
                                        ++index;
                                        size_t len = bareData[index];
                                        //length += 1;
                                        //char buf[128] = {0};
                                        //unsigned int l = gsmSerializeNumbers((const char*)pBuf + length, buf, len);
                                        //std::string t(buf, l);
                                        //CTime expiry(atoi(t.substr(0, 2).c_str()) + 2000, atoi(t.substr(2, 2).c_str()), atoi(t.substr(4, 2).c_str()), atoi(t.substr(6, 2).c_str()), atoi(t.substr(8, 2).c_str()), atoi(t.substr(10, 2).c_str()));
                                        //CTime start(1970, 1, 1, 8, 0, 0);
                                        //CTimeSpan ts = expiry - start;
                                        //n = ts.GetTotalSeconds();
                                        //length += len;
                                    }
                                    break;
                                    }
                                    break;
                                case 0x89:
                                    {
                                        std::string MMSSendAddr = bareData.substr(2, bareData[index]);
                                        ++index;
                                    }
                                    break;
                                case 0x8A:
                                    MMSClass = bareData[index];
                                    ++index;
                                    break;
                                case 0x8C:
                                    MMSType = bareData[index];
                                    ++index;
                                    break;
                                case 0x8D:
                                    MMSVersion = bareData[index];
                                    ++index;
                                    break;
                                case 0x8E:
                                    {
                                        unsigned char len = bareData[index];
                                        unsigned int n = 0;
                                        for (; i < len; ++i) {
                                            ++index;
                                            unsigned char s = bareData[index];
                                            n = (n << 8) | s;
                                        }
                                        MMSSize = n;
                                    }
                                    break;
                                case 0x98:
                                    MMSTransactionID = bareData.substr(1);
                                    break;
                                default:
                                    break;
}
#endif
