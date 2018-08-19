#include "MMSData2.h"

namespace Util {
    std::wstring SMS::Data::MMSData::tableName_ = L"mmsData";
    int SMS::Data::MMSData::count_ = 0;
    int SMS::Data::MMSData::offset_ = 0;
}

namespace SMS {
    namespace Data {
        std::vector<Util::shared_ptr<MMSData> > const MMSData::Select(std::wstring const& filter) {
            return GetDatasByFilter(filter, modifyFieldByDB_);
        }

        void MMSData::Update() const {
            std::wstring cmd = L"UPDATE ";
            cmd += tableName_;
            cmd += L" SET [messageType] = '";
            cmd += MessageType;
            cmd += L"', [transactionId] = '";
            cmd += TransactionId;
            cmd += L"', [mmsVersion] = '";
            cmd += MMSVersion;
            cmd += L"', [recipientAddress] = '";
            cmd += RecipientAddress;
            cmd += L"', [contentType] = '";
            cmd += ContentType;
            cmd += L"', [senderAddress] = '";
            cmd += SenderAddress;
            cmd += L"', [timeOfExpiry] = ";
            cmd += Util::StringOp::FromInt(TimeOfExpiry);
            cmd += L", [subject] = '";
            cmd += Subject;
            cmd += L"', [messageId] = '";
            cmd += MessageId;
            cmd += L"', [messageClass] = '";
            cmd += MessageClass;
            cmd += L"', [dateAndTime] = '";
            cmd += Util::StringOp::FromInt(DateAndTime);
            cmd += L"', [requestStatus] = '";
            cmd += RequestStatus;
            cmd += L"', [savePath] = '";
            cmd += SavePath;
            cmd += L"', [isRead] = ";
            cmd += Util::StringOp::FromInt(isRead);
            cmd += L", [type] = ";
            cmd += Util::StringOp::FromInt(type);
            cmd += L", WHERE [id] = ";
            cmd += Util::StringOp::FromInt(id());
            ExecCommand(cmd);
        }

        void MMSData::Insert() {
            std::wstring cmd = L"INSERT INTO ";
            cmd += tableName_;
            cmd += L" ( messageType, transactionId, mmsVersion, recipientAddress, contentType, timeOfExpiry, subject, messageId, messageClass, dateAndTime, requestStatus, savePath, isRead, type ) VALUES ( '" ; 
            cmd += MessageType;
            cmd += L"', '";
            cmd += TransactionId;
            cmd += L"', '";
            cmd += MMSVersion;
            cmd += L"', '";
            cmd += RecipientAddress;
            cmd += L"', '";
            cmd += ContentType;
            cmd += L"', '";
            cmd += SenderAddress;
            cmd += L"', ";
            cmd += Util::StringOp::FromInt(TimeOfExpiry);
            cmd += L", '";
            cmd += Subject;
            cmd += L"', '";
            cmd += MessageId;
            cmd += L"', '";
            cmd += MessageClass;
            cmd += L"', ";
            cmd += Util::StringOp::FromInt(DateAndTime);
            cmd += L", '";
            cmd += RequestStatus;
            cmd += L"', '";
            cmd += SavePath;
            cmd += L"', ";
            cmd += Util::StringOp::FromInt(isRead);
            cmd += L", ";
            cmd += Util::StringOp::FromInt(type);
            cmd += L" )";
            ExecCommand(cmd);
            id(GetCurrentId());
        }

        void MMSData::Remove() const {
            std::wstring filter = L"id = " + Util::StringOp::FromInt(id());
            Remove(filter);
        }

        void MMSData::Remove(std::wstring const& filter) {
            RemoveDatasByFilter(filter);
        }

        MMSData const MMSData::Parse(std::string const& content) {
            MMSData result;
            return result;
        }

        std::string const MMSData::ToString() const {
            std::string result;
            return result;
        }

        void MMSData::modifyFieldByDB_(int argc, char** argv, char** columnName, Util::shared_ptr<MMSData> item) {
            item->id(atoi(argv[GetIndexByName(argc, columnName, "id")]));
            item->MessageType = Util::StringOp::FromUTF8(argv[GetIndexByName(argc, columnName, "MessageType")]);
            item->TransactionId			=	Util::StringOp::FromUTF8(argv[GetIndexByName(argc, columnName, "TransactionId")]);
            item->MMSVersion					=	Util::StringOp::FromUTF8(argv[GetIndexByName(argc, columnName, "mmsVersion")]);
            item->RecipientAddress					=	Util::StringOp::FromUTF8(argv[GetIndexByName(argc, columnName, "recipientAddress")]);
            item->ContentType					=	Util::StringOp::FromUTF8(argv[GetIndexByName(argc, columnName, "contentType")]);
            item->TimeOfExpiry				=	atoi(argv[GetIndexByName(argc, columnName, "timeOfExpiry")]);
            item->Subject						=	Util::StringOp::FromUTF8(argv[GetIndexByName(argc, columnName, "subject")]);
            item->MessageId								=	Util::StringOp::FromUTF8(argv[GetIndexByName(argc, columnName, "messageId")]);
            item->MessageClass								=	atoi(argv[GetIndexByName(argc, columnName, "messageClass")]);
            item->DateAndTime			=	atoi(argv[GetIndexByName(argc, columnName, "dateAndTime")]);
            item->RequestStatus		=	Util::StringOp::FromUTF8(argv[GetIndexByName(argc, columnName, "requestStatus")]);
            item->SavePath						=	Util::StringOp::FromUTF8(argv[GetIndexByName(argc, columnName, "savePath")]);
            item->isRead						=	atoi(argv[GetIndexByName(argc, columnName, "isRead")]);
            item->type						=	static_cast<Type>(atoi(argv[GetIndexByName(argc, columnName, "type")]));
        }
    }
}
