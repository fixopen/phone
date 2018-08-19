#include "stdafx.h"
#include "User.h"

namespace Data
{
    std::string DataAccess<User>::tableName_ = "user";
    sqlite3* DataAccess<User>::db_ = Data::GetDB();
    int DataAccess<User>::rowCount_ = 0;
    int DataAccess<User>::offset_ = 0;
    Indication DataAccess<User>::indication_ = 0;

    User User::currentUser_;

    User::User()
    : DataAccess<User>()
        //, baseInfo_(new Contact())
        //, baseInfoId_(0)
        //, password_(0)
        //, setting_(new Setting())
        //, settingId_(0)
     {
    }

    User::~User() {
        //delete baseInfo_;
        //delete setting_;
    }

    std::vector<boost::shared_ptr<User> > User::GetFromDatabase(std::string filter, Direction const dir, int const id, int const pageSize) {
        std::vector<boost::shared_ptr<User> > result = DataAccess<User>::GetDatasByFilter(filter, modifyFieldByDB_, dir, id, pageSize);
        for (size_t i = 0; i < result.size(); ++i) {
            //result[i]->contact_ = Contact::GetFromDatabase("id = " + result[i]->contactId_);
            //result[i]->setting_ = Setting::GetFromDatabase("id = " + result[i]->settingId_);
        }
        return result;
    }

    void User::Update() const {
        std::string cmd = "UPDATE ";
        cmd += tableName();
        cmd += " SET [baseInfoId] = ";
        cmd += Util::StringOp::FromInt(baseInfoId_);
        cmd += ", [password] = '";
        cmd += password_;
        cmd += "', [settingId] = ";
        cmd += Util::StringOp::FromInt(settingId_);
        cmd += " WHERE id = ";
        cmd += Util::StringOp::FromInt(id());
        ExecCommand(cmd);
    }

    void User::Insert() {
        std::string cmd = "INSERT INTO ";
        cmd += tableName();
        cmd += " ( baseInfoId, password, settingId ) VALUES ( ";
        cmd += Util::StringOp::FromInt(baseInfoId_);
        cmd += ", '";
        cmd += password_;
        cmd += "', ";
        cmd += Util::StringOp::FromInt(settingId_);
        cmd += " )";
        ExecCommand(cmd);
        id(GetCurrentId());
    }

    void User::Remove() const {
        User::Remove("id = " + Util::StringOp::FromInt(id()));
    }

    void User::Remove(std::string filter) {
        DataAccess<User>::RemoveDatasByFilter(filter);
    }

    void User::modifyFieldByDB_(int argc, char** argv, char** columnName, boost::shared_ptr<User> item) {
        item->id(atoi(argv[Data::getIndexByName(argc, columnName, "id")]));
        item->baseInfoId_ = atoi(argv[Data::getIndexByName(argc, columnName, "baseInfoId")]);
        item->password_ = argv[Data::getIndexByName(argc, columnName, "password")];
        item->settingId_ = atoi(argv[Data::getIndexByName(argc, columnName, "settingId")]);
    }
}
