#include "stdafx.h"
#include "MmsMessage.h"

namespace Data
{
    std::string MmsMessage::tableName_ = "mmsMessage";
    int MmsMessage::rowCount_ = 0;
    int MmsMessage::offset_ = 0;
    Indication MmsMessage::indication_ = 0;
    sqlite3* MmsMessage::db_ = Data::GetDB();
}

//namespace SMS {
namespace Data
{
    std::vector<boost::shared_ptr<MmsMessage> > MmsMessage::GetFromDatabase(std::string filter, Direction const dir, int const id, int const pageSize) {
        return DataAccess<MmsMessage>::GetDatasByFilter(filter, modifyFieldByDB_, dir, id, pageSize);
    }

    void MmsMessage::Update() const {
        std::string cmd = "UPDATE ";
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
        cmd += ", [timestamp] = '";
        cmd += timestamp.ToString_();
        cmd += "', [state] = ";
        cmd += Util::StringOp::FromInt(state);
        cmd += ", [transactionId] = '";
        cmd += transactionId;
        cmd += "', [messageClass] = ";
        cmd += Util::StringOp::FromInt(messageClass);
        cmd += ", [messageSize] = ";
        cmd += Util::StringOp::FromInt(messageSize);
        cmd += ", [timeOfExpiry] = ";
        cmd += Util::StringOp::FromInt(timeOfExpiry);
        cmd += ", [url] = '";
        cmd += url;
        cmd += "' WHERE [id] = ";
        cmd += Util::StringOp::FromInt(id());
        ExecCommand(cmd);
        extern VOID WriteLog(char*ptr);
        WriteLog((char*)cmd.c_str());
    }

    void MmsMessage::Insert() {
        std::string cmd = "INSERT INTO ";
        cmd += tableName();
        cmd += " (smscAddressType, smscAddress, isSetReplyPath, hasHeaderInfo, requestReport, validityPeriodFormat, rejectCopy, type, reference, remoteAddressType, remoteAddress, uplevelProtocol, encoding, validityPeriod, timestamp, state, transactionId, messageClass, messageSize, timeOfExpiry, url ) VALUES ( " ; 
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
        cmd += timestamp.ToString_();
        cmd += "', ";
        cmd += Util::StringOp::FromInt(state);
        cmd += ", '";
        cmd += transactionId;
        cmd += "', ";
        cmd += Util::StringOp::FromInt(messageClass);
        cmd += ", ";
        cmd += Util::StringOp::FromInt(messageSize);
        cmd += ", ";
        cmd += Util::StringOp::FromInt(timeOfExpiry);
        cmd += ", '";
        cmd += url;
        cmd += "' )";
        ExecCommand(cmd);
        extern VOID WriteLog(char*ptr);
        WriteLog((char*)cmd.c_str());
        id(GetCurrentId());
    }

    void MmsMessage::Remove() const {
        std::string filter = "id = " + Util::StringOp::FromInt(id());
        Remove(filter);
    }

    void MmsMessage::Remove(std::string const & filter) {
        RemoveDatasByFilter(filter);
    }

    MmsMessage const MmsMessage::Parse(std::string const & content) {
        MmsMessage result;
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
            break;
        case Encoding::UnicodeCodeSet:
            break;
        case Encoding::Reserved:
            break;
        default:
            break;
        }
        return result;
    }

    std::string const MmsMessage::ToString() const {
        std::string result;
        result += smsc.ToString();
        char l[3] = {
            0
        };
        char c = flag.ToString();
        sprintf_s(l, 3, "%02X", c);
        result += l;
        sprintf_s(l, 3, "%02X", reference);
        result += l;
        result += remote.ToString();
        sprintf_s(l, 3, "%02X", uplevelProtocol);
        result += uplevelProtocol;
        c = encoding.ToString();
        sprintf_s(l, 3, "%02X", c);
        result += l;
        sprintf_s(l, 3, "%02X", validityPeriod);
        result += validityPeriod;
        switch (encoding.encoding) {
        case Encoding::DefaultAlphabet:
            sprintf_s(l, 3, "%02X", url.length());
            result += l;
            break;
        case Encoding::OctetBit:
            sprintf_s(l, 3, "%02X", url.length());
            result += l;
            break;
        case Encoding::UnicodeCodeSet:
            sprintf_s(l, 3, "%02X", url.length());
            result += l;
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

    void MmsMessage::modifyFieldByDB_(int argc, char** argv, char** columnName, boost::shared_ptr<MmsMessage> item) {
        item->id(atoi(argv[getIndexByName(argc, columnName, "id")]));
        item->smsc.addressType.mustSet = true;
        item->smsc.addressType.typeOfNumber = static_cast<AddressType::NumberType>((atoi(argv[getIndexByName(argc, columnName, "smscAddressType")])));
        //item->smsc.addressType.indentification        =   static_cast<AddressType::Indentification>((atoi(argv[getIndexByName(argc,columnName,"smscAddressType")])&0x0f) >> 0);
        //AddressType::AddressType(atoi(argv[Util::BaseData::getIndexByName(argc,columnName,"smscAddressType")]));
        item->smsc.address = argv[getIndexByName(argc, columnName, "smscAddress")];//得到的是utf_8
        item->flag.isSetReplyPath = !!atoi(argv[getIndexByName(argc, columnName, "isSetReplyPath")]);
        item->flag.hasHeaderInfo = !!atoi(argv[getIndexByName(argc, columnName, "hasHeaderInfo")]);
        item->flag.requestReport = !!atoi(argv[getIndexByName(argc, columnName, "requestReport")]);
        item->flag.validityPeriodFormat = static_cast<Flag::ValidityPeriodFormat>(atoi(argv[getIndexByName(argc, columnName, "validityPeriodFormat")]));
        item->flag.rejectCopy = !!atoi(argv[getIndexByName(argc, columnName, "rejectCopy")]);
        item->flag.type = static_cast<Flag::Type>(atoi(argv[getIndexByName(argc, columnName, "type")]));
        item->reference = atoi(argv[getIndexByName(argc, columnName, "reference")]);
        item->remote.addressType.mustSet = true; //(atoi(argv[getIndexByName(argc,columnName,"remoteAddressType")])&0x80) >> 7;
        item->remote.addressType.typeOfNumber = static_cast<AddressType::NumberType>((atoi(argv[getIndexByName(argc, columnName, "remoteAddressType")])));
        //item->remote.addressType.indentification  =   static_cast<AddressType::Indentification>((atoi(argv[getIndexByName(argc,columnName,"remoteAddressType")])&0x0f) >> 0);
        //AddressType::AddressType(atoi(argv[getIndexByName(argc,columnName,"remoteAddressType")]));
        item->remote.address = argv[getIndexByName(argc, columnName, "remoteAddress")];     
        item->uplevelProtocol = atoi(argv[getIndexByName(argc, columnName, "uplevelProtocol")]);
        item->encoding.encoding = static_cast<Encoding::EncodingCode>(atoi(argv[getIndexByName(argc, columnName, "encoding")]));
        item->validityPeriod = atoi(argv[getIndexByName(argc, columnName, "validityPeriod")]);
        item->url = argv[getIndexByName(argc, columnName, "url")];
        item->timestamp = SMSCTimestamp(argv[getIndexByName(argc, columnName, "timestamp")]);
        item->state = static_cast<State>(atoi(argv[getIndexByName(argc, columnName, "state")]));
        item->transactionId = argv[getIndexByName(argc, columnName, "transactionId")];
        item->messageClass = atoi(argv[getIndexByName(argc, columnName, "messageClass")]);
        item->messageSize = atoi(argv[getIndexByName(argc, columnName, "messageSize")]);
        item->timeOfExpiry = atoi(argv[getIndexByName(argc, columnName, "timeOfExpiry")]);
    }
}
//}
