#include "stdafx.h"
#include "MMSData.h"

namespace Data
{
    std::string MMSData::tableName_ = "mmsData";
    int MMSData::rowCount_ = 0;
    int MMSData::offset_ = 0;
    Indication MMSData::indication_ = 0;
    sqlite3* MMSData::db_ = Data::GetDB();
}

//namespace SMS {
namespace Data
{
    std::vector<boost::shared_ptr<MMSData> > MMSData::GetFromDatabase(std::string filter, Direction const dir, int const id, int const pageSize) {
        return DataAccess<MMSData>::GetDatasByFilter(filter, modifyFieldByDB_, dir, id, pageSize);
    }
    std::vector<boost::shared_ptr<MMSData> > MMSData::GetFromDatabaseByTypeOffsetLength(std::string type, int const offset, int const pageSize) {
        std::string filter = type;
        return DataAccess<MMSData>::GetDatasByFilterAndPageInfo(filter, modifyFieldByDB_, offset, pageSize);
    }

    boost::shared_ptr<MMSData> MMSData::GetDataById(int id) {
        return DataAccess<MMSData>::GetDataById(id, modifyFieldByDB_);
    }

    void MMSData::Update() const {
        std::string cmd = "UPDATE ";
        cmd += tableName();
        cmd += " SET [messageType] = '";
        cmd += MessageType;
        cmd += "', [transactionId] = '";
        cmd += TransactionId;
        cmd += "', [mmsVersion] = '";
        cmd += MMSVersion;
        cmd += "', [recipientAddress] = '";
        cmd += RecipientAddress;
        cmd += "', [contentType] = '";
        cmd += ContentType;
        cmd += "', [senderAddress] = '";
        cmd += SenderAddress;
        cmd += "', [timeOfExpiry] = ";
        cmd += Util::StringOp::FromInt(TimeOfExpiry);
        cmd += ", [subject] = '";
        cmd += Subject;
        cmd += "', [messageId] = '";
        cmd += MessageId;
        cmd += "', [messageClass] = '";
        cmd += MessageClass;
        cmd += "', [dateAndTime] = '";
        cmd += Util::StringOp::FromInt(DateAndTime);
        cmd += "', [requestStatus] = '";
        cmd += RequestStatus;
        cmd += "', [savePath] = '";
        cmd += SavePath;
        cmd += "', [isRead] = ";
        cmd += Util::StringOp::FromInt(isRead);
        cmd += ", [type] = ";
        cmd += Util::StringOp::FromInt(type);
        cmd += " WHERE [id] = ";
        cmd += Util::StringOp::FromInt(id());
        extern void WriteLog(char*ptr);
        WriteLog((char*)cmd.c_str());
        ExecCommand(cmd);
    }

    bool MMSData::Insert() {
        std::string cmd = "INSERT INTO ";
        cmd += tableName();
        cmd += " ( messageType, transactionId, mmsVersion, recipientAddress, contentType, senderAddress, timeOfExpiry, subject, messageId, messageClass, dateAndTime, requestStatus, savePath, isRead, [type] ) VALUES ( '" ; 
        cmd += MessageType;
        cmd += "', '";
        cmd += TransactionId;
        cmd += "', '";
        cmd += MMSVersion;
        cmd += "', '";
        cmd += RecipientAddress;
        cmd += "', '";
        cmd += ContentType;
        cmd += "', '";
        cmd += SenderAddress;
        cmd += "', ";
        cmd += Util::StringOp::FromInt(TimeOfExpiry);
        cmd += ", '";
        cmd += Subject;
        cmd += "', '";
        cmd += MessageId;
        cmd += "', '";
        cmd += MessageClass;
        cmd += "', ";
        cmd += Util::StringOp::FromInt(DateAndTime);
        cmd += ", '";
        cmd += RequestStatus;
        cmd += "', '";
        cmd += SavePath;
        cmd += "', ";
        cmd += Util::StringOp::FromInt(isRead);
        cmd += ", ";
        cmd += Util::StringOp::FromInt(type);
        cmd += " )";
        extern void WriteLog(char*ptr);
        WriteLog((char*)cmd.c_str());

        bool bt = ExecCommand(cmd);

        id(GetCurrentId());

        return bt ;
    }

    void MMSData::Remove() const {
        std::string filter = "id = " + Util::StringOp::FromInt(id());
        Remove(filter);
    }

    void MMSData::Remove(std::string const & filter) {
        RemoveDatasByFilter(filter);
    }

    MMSData const MMSData::Parse(std::string const & content) {
        MMSData result;
        return result;
    }

    std::string const MMSData::ToString() const {
        std::string result;
        return result;
    }

    void MMSData::modifyFieldByDB_(int argc, char** argv, char** columnName, boost::shared_ptr<MMSData> item) {
        item->id(atoi(argv[getIndexByName(argc, columnName, "id")]));
        item->MessageType = argv[getIndexByName(argc, columnName, "MessageType")];
        item->TransactionId = argv[getIndexByName(argc, columnName, "TransactionId")];
        item->MMSVersion = argv[getIndexByName(argc, columnName, "mmsVersion")];
        item->RecipientAddress = argv[getIndexByName(argc, columnName, "recipientAddress")];
        item->ContentType = argv[getIndexByName(argc, columnName, "contentType")];
        item->SenderAddress = argv[getIndexByName(argc, columnName, "senderAddress")];
        item->TimeOfExpiry = atoi(argv[getIndexByName(argc, columnName, "timeOfExpiry")]);
        item->Subject = argv[getIndexByName(argc, columnName, "subject")];
        item->MessageId = argv[getIndexByName(argc, columnName, "messageId")];
        item->MessageClass = argv[getIndexByName(argc, columnName, "messageClass")];
        item->DateAndTime = atoi(argv[getIndexByName(argc, columnName, "dateAndTime")]);
        item->RequestStatus = argv[getIndexByName(argc, columnName, "requestStatus")];
        item->SavePath = argv[getIndexByName(argc, columnName, "savePath")];
        item->isRead = !!atoi(argv[getIndexByName(argc, columnName, "isRead")]);
        item->type = static_cast<Type>(atoi(argv[getIndexByName(argc, columnName, "type")]));
    }
}
//}
