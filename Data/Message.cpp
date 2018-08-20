#include "stdafx.h"
#include "Message.h"

namespace Data {
   	std::string Message::tableName_ = "message";
	int Message::rowCount_ = 0;
	int Message::offset_ = 0;
	Indication Message::indication_ = 0;
	sqlite3* Message::db_ = Data::GetDB();
}

//namespace SMS {
    namespace Data {
        std::vector<boost::shared_ptr<Message> > Message::GetFromDatabase(std::string filter, Direction const dir, int const id, int const pageSize) {
            return DataAccess<Message>::GetDatasByFilter(filter, modifyFieldByDB_, dir, id, pageSize);
        }
		std::vector<boost::shared_ptr<Message> > Message::GetFromDatabaseByTypeOffsetLength(std::string type, int const offset, int const pageSize)
		{
			std::string filter = type;
			return DataAccess<Message>::GetDatasByFilterAndPageInfo(filter, modifyFieldByDB_, offset, pageSize);
		}
		boost::shared_ptr<Message> Message::GetDataById(int id)
		{
			return DataAccess<Message>::GetDataById(id, modifyFieldByDB_);
		}
        void Message::Update() const {
            std::string  cmd = "UPDATE ";
            cmd += tableName();
            cmd += " SET [smscAddressType] = ";
            cmd += Util::StringOp::FromInt(smsc.addressType.typeOfNumber);
            cmd += ", [smscAddress] = '";
            cmd += smsc.address;
            cmd += "', [isSetReplyPath] = ";
            cmd += Util::StringOp::FromInt(flag.isSetReplyPath);
            cmd += ", [hasHeaderInfo] = ";
            cmd += Util::StringOp::FromInt(flag.hasHeaderInfo);
            cmd += ", [requestReport] = ";
            cmd += Util::StringOp::FromInt(flag.requestReport);
            cmd += ", [validityPeriodFormat] = ";
            cmd += Util::StringOp::FromInt(flag.validityPeriodFormat);
            cmd += ", [rejectCopy] = ";
            cmd += Util::StringOp::FromInt(flag.rejectCopy);
            cmd += ", [type] = ";
            cmd += Util::StringOp::FromInt(flag.type);
            cmd += ", [reference] = ";
            cmd += Util::StringOp::FromInt(reference);
            cmd += ", [remoteAddressType] = ";
            cmd += Util::StringOp::FromInt(remote.addressType.typeOfNumber);
            cmd += ", [remoteAddress] = '";
            cmd += remote.address;
            cmd += "', [uplevelProtocol] = ";
            cmd += Util::StringOp::FromInt(uplevelProtocol);
            cmd += ", [encoding] = ";
            cmd += Util::StringOp::FromInt(encoding.encoding);
            cmd += ", [validityPeriod] = ";
            cmd += Util::StringOp::FromInt(validityPeriod);
            cmd += ", [data] = '";
            cmd += unicodeData;
            cmd += "', [timestamp] = '";
            cmd += timestamp.ToString_();
            cmd += "', [state] = ";
            cmd += Util::StringOp::FromInt(state);
            cmd += ", [group] = ";
            cmd += Util::StringOp::FromInt(group);
			cmd += ", [transactionId] = '";
            cmd += transactionId;
			cmd += "', [total] = ";
            cmd += Util::StringOp::FromInt(total);
			cmd += ", [no] = ";
            cmd += Util::StringOp::FromInt(no);
            cmd += " WHERE [id] = ";
            cmd += Util::StringOp::FromInt(id());
			extern void WriteLog(char *ptr);
			WriteLog((char *)cmd.c_str());
            ExecCommand(cmd);
        }

        bool Message::Insert() {
            std::string  cmd = "INSERT INTO ";
            cmd += tableName();
	        cmd += " (smscAddressType, smscAddress, isSetReplyPath, hasHeaderInfo, requestReport, validityPeriodFormat, rejectCopy, type, reference, remoteAddressType, remoteAddress, uplevelProtocol, encoding, validityPeriod, data, timestamp, state, [group], [transactionId], [total], [no] ) VALUES ( " ; 
            cmd += Util::StringOp::FromInt(smsc.addressType.typeOfNumber);
            cmd += ", '";
            cmd += smsc.address;
            cmd += "', ";
            cmd += Util::StringOp::FromInt(flag.isSetReplyPath);
            cmd += ", ";
            cmd += Util::StringOp::FromInt(flag.hasHeaderInfo);
            cmd += ", ";
            cmd += Util::StringOp::FromInt(flag.requestReport);
            cmd += ", ";
            cmd += Util::StringOp::FromInt(flag.validityPeriodFormat);
            cmd += ", ";
            cmd += Util::StringOp::FromInt(flag.rejectCopy);
            cmd += ", ";
            cmd += Util::StringOp::FromInt(flag.type);
            cmd += ", ";
            cmd += Util::StringOp::FromInt(reference);
            cmd += ", ";
            cmd += Util::StringOp::FromInt(remote.addressType.typeOfNumber);
            cmd += ", '";
            cmd += remote.address;
            cmd += "', ";
            cmd += Util::StringOp::FromInt(uplevelProtocol) ;
            cmd += ", ";
            cmd += Util::StringOp::FromInt(encoding.encoding);
            cmd += ", ";
            cmd += Util::StringOp::FromInt(validityPeriod);
            cmd += ", '";
            cmd += unicodeData;
            cmd += "', '";
            cmd += timestamp.ToString_();
            cmd += "', ";
            cmd += Util::StringOp::FromInt(state);
            cmd += ", ";
            cmd += Util::StringOp::FromInt(group);
			cmd += ", '";
			cmd += transactionId;
			cmd += "', ";
			cmd += Util::StringOp::FromInt(total);
			cmd += ", ";
			cmd += Util::StringOp::FromInt(no);
            cmd += " )";
			extern VOID WriteLog(CHAR* str);
			WriteLog((char *)cmd.c_str());
			
			bool btr = ExecCommand(cmd);

            id(GetCurrentId());

			return btr ;
        }

        void Message::Remove() const {
            std::string  filter = "id = " + Util::StringOp::FromInt(id());
            Remove(filter);
        }

        void Message::Remove(std::string  const& filter) {
            RemoveDatasByFilter(filter);
        }

        Message const Message::Parse(std::string const& content) {
            Message result;
            size_t pos = 0;
            result.smsc.addressLength = Util::StringOp::ToInt(content.substr(pos, 2), 16);
            size_t codeLength = (result.smsc.addressLength - 1) * 2;
            pos += 2;
            result.smsc.addressType = AddressType(Util::StringOp::ToInt(content.substr(pos, 2), 16));
            pos += 2;
            result.smsc.address = TrimF(Reverse(content.substr(pos, codeLength)));
            pos += codeLength;
            result.flag = Flag(Util::StringOp::ToInt(content.substr(pos, 2), 16));
            pos += 2;
            result.reference = Util::StringOp::ToInt(content.substr(pos, 2), 16);
            pos += 2;
            result.remote.addressLength = Util::StringOp::ToInt(content.substr(pos, 2), 16);
            codeLength = result.remote.addressLength - 2;
            if (codeLength % 2) {
                ++codeLength;
            }
            pos += 2;
            result.remote.addressType = AddressType(Util::StringOp::ToInt(content.substr(pos, 2), 16));
            pos += 2;
            result.remote.address = TrimF(Reverse(content.substr(pos, codeLength)));
            pos += codeLength;
            result.uplevelProtocol = Util::StringOp::ToInt(content.substr(pos, 2), 16);
            pos += 2;
            result.encoding = Encoding(Util::StringOp::ToInt(content.substr(pos, 2), 16));
            pos += 2;
            result.timestamp = SMSCTimestamp(content.substr(pos, 14));
            pos += 14;
            size_t dataCount = Util::StringOp::ToInt(content.substr(pos, 2), 16);
            pos += 2;
            switch (result.encoding.encoding) {
            case Encoding::DefaultAlphabet:
                break;
            case Encoding::OctetBit:
                //result.unicodeData = Util::StringOp::ToUnicode(content.substr(pos, dataCount));
				result.unicodeData = content.substr(pos, dataCount);
                break;
            case Encoding::UnicodeCodeSet:
				{
					CString s = "";
					for (size_t i = 0; i < dataCount; ++i) {
						wchar_t c = Util::StringOp::ToInt(content.substr(pos, 2), 16);
						pos += 2;
						s += c;
					}
					result.unicodeData = Util::StringOp::FromCString(s);
				}
                break;
            case Encoding::Reserved:
                break;
            default:
                break;
            }
            return result;
        }

        std::string const Message::ToString() const {
            std::string result;
            result += smsc.ToString();
            char l[3] = {0};
            char c = flag.ToString();
            sprintf(l, "%02X", c);
            result += l;
            sprintf(l, "%02X", reference);
            result += l;
            result += remote.ToString();
            sprintf(l, "%02X", uplevelProtocol);
            result += uplevelProtocol;
            c = encoding.ToString();
            sprintf(l, "%02X", c);
            result += l;
            sprintf(l, "%02X", validityPeriod);
            result += validityPeriod;
            switch (encoding.encoding) {
            case Encoding::DefaultAlphabet:
                sprintf(l, "%02X", unicodeData.length());
                result += l;
                //result += unicodeData;
                break;
            case Encoding::OctetBit:
                sprintf(l, "%02X", unicodeData.length());
                result += l;
                //result += unicodeData;
                break;
            case Encoding::UnicodeCodeSet:
				{
					sprintf(l, "%02X", unicodeData.length());
					result += l;
					for (size_t i = 0; i < unicodeData.length(); ++i) {
						char c[3] = {0};
						sprintf(c, "%02X", unicodeData[i] >> 8);
						result += c;
						sprintf(c, "%02X", unicodeData[i] << 8 >> 8);
						result += c;
					}
				}
                break;
            case Encoding::Reserved:
                //error
                break;
            default:
                //error
                break;
            }
            return result;
        }

        void Message::modifyFieldByDB_(int argc, char** argv, char** columnName, boost::shared_ptr<Message> item) {
            item->id(atoi(argv[getIndexByName(argc, columnName, "id")]));
            item->smsc.addressType.mustSet				=	true;
            item->smsc.addressType.typeOfNumber			=	static_cast<AddressType::NumberType>((atoi(argv[getIndexByName(argc,columnName,"smscAddressType")])));
            //item->smsc.addressType.indentification		=	static_cast<AddressType::Indentification>((atoi(argv[getIndexByName(argc,columnName,"smscAddressType")])&0x0f) >> 0);
            //AddressType::AddressType(atoi(argv[Util::BaseData::getIndexByName(argc,columnName,"smscAddressType")]));
            item->smsc.address							=	argv[getIndexByName(argc,columnName,"smscAddress")];//得到的是utf_8
            item->flag.isSetReplyPath					=	atoi(argv[getIndexByName(argc,columnName,"isSetReplyPath")]);
            item->flag.hasHeaderInfo					=	atoi(argv[getIndexByName(argc,columnName,"hasHeaderInfo")]);
            item->flag.requestReport					=	atoi(argv[getIndexByName(argc,columnName,"requestReport")]);
            item->flag.validityPeriodFormat				=	static_cast<Flag::ValidityPeriodFormat>(atoi(argv[getIndexByName(argc,columnName,"validityPeriodFormat")]));
            item->flag.rejectCopy						=	atoi(argv[getIndexByName(argc,columnName,"rejectCopy")]);
            item->flag.type								=	static_cast<Flag::Type>(atoi(argv[getIndexByName(argc,columnName,"type")]));
            item->reference								=	atoi(argv[getIndexByName(argc,columnName,"reference")]);
            item->remote.addressType.mustSet			=	true; //(atoi(argv[getIndexByName(argc,columnName,"remoteAddressType")])&0x80) >> 7;
            item->remote.addressType.typeOfNumber		=	static_cast<AddressType::NumberType>((atoi(argv[getIndexByName(argc,columnName,"remoteAddressType")])));
            //item->remote.addressType.indentification	=	static_cast<AddressType::Indentification>((atoi(argv[getIndexByName(argc,columnName,"remoteAddressType")])&0x0f) >> 0);
            //AddressType::AddressType(atoi(argv[getIndexByName(argc,columnName,"remoteAddressType")]));
            item->remote.address						=	argv[getIndexByName(argc,columnName,"remoteAddress")];		
            item->uplevelProtocol						=	atoi(argv[getIndexByName(argc,columnName,"uplevelProtocol")]);
            item->encoding.encoding						=	static_cast<Encoding::EncodingCode>(atoi(argv[getIndexByName(argc,columnName,"encoding")]));
            item->validityPeriod						=	atoi(argv[getIndexByName(argc,columnName,"validityPeriod")]);
            item->unicodeData	= argv[getIndexByName(argc,columnName,"data")];
            item->timestamp = SMSCTimestamp(argv[getIndexByName(argc,columnName,"timestamp")]);
            item->state = static_cast<State>(atoi(argv[getIndexByName(argc, columnName, "state")]));
            item->group = static_cast<Group>(atoi(argv[getIndexByName(argc, columnName, "group")]));
			item->transactionId							=	argv[getIndexByName(argc,columnName,"transactionId")];
			item->total									=	atoi(argv[getIndexByName(argc,columnName,"total")]);
         	item->no									=	atoi(argv[getIndexByName(argc,columnName,"no")]);    
        }
    }
//}
