#include "OTAData.h"

namespace Util {
    std::wstring SMS::Data::Register::tableName_ = L"register";
    int SMS::Data::Register::count_ = 0;
    int SMS::Data::Register::offset_ = 0;

    std::wstring SMS::Data::RegisterReply::tableName_ = L"registerReply";
    int SMS::Data::RegisterReply::count_ = 0;
    int SMS::Data::RegisterReply::offset_ = 0;

    std::wstring SMS::Data::Weather::tableName_ = L"weather";
    int SMS::Data::Weather::count_ = 0;
    int SMS::Data::Weather::offset_ = 0;

    std::wstring SMS::Data::SoftwareDownload::tableName_ = L"softwareDownload";
    int SMS::Data::SoftwareDownload::count_ = 0;
    int SMS::Data::SoftwareDownload::offset_ = 0;

    std::wstring SMS::Data::MultimediaDownload::tableName_ = L"multimediaDownload";
    int SMS::Data::MultimediaDownload::count_ = 0;
    int SMS::Data::MultimediaDownload::offset_ = 0;

    std::wstring SMS::Data::MenuManager::tableName_ = L"menuManager";
    int SMS::Data::MenuManager::count_ = 0;
    int SMS::Data::MenuManager::offset_ = 0;
}

namespace SMS {
    namespace Data {
        std::vector<Util::shared_ptr<Register> > const Register::Select(std::wstring const& filter) {
            return GetDatasByFilter(filter, modifyFieldByDB_);
        }

        void Register::Update() const {
            std::wstring cmd = L"UPDATE ";
            //cmd += tableName_;
            //cmd += L" SET [mobileNumber] = '";
            //cmd += static_cast<LPCTSTR>(mobileNumber);
            //cmd += L"', [nickname] = '";
            //cmd += static_cast<LPCTSTR>(nickname);
            //cmd += L"', [type] = ";
            //cmd += Util::StringOp::FromInt(type);
            //cmd += L" WHERE id = ";
            //cmd += Util::StringOp::FromInt(id());
            ExecCommand(cmd);
        }

        void Register::Insert() {
            std::wstring cmd = L"INSERT INTO ";
            cmd += tableName_;
            //cmd += L" ( mobileNumber, nickname, type ) VALUES ( '";
            //cmd += static_cast<LPCTSTR>(mobileNumber);
            //cmd += L"', '";
            //cmd += static_cast<LPCTSTR>(nickname);
            //cmd += L"', ";
            //cmd += Util::StringOp::FromInt(type);
            cmd += L" )";
            ExecCommand(cmd);
            id(GetCurrentId());
        }

        void Register::Remove() const {
            std::wstring filter = L"id = " + Util::StringOp::FromInt(id());
            Remove(filter);
        }

        void Register::Remove(std::wstring filter) {
            RemoveDatasByFilter(filter);
        }

        Register const Register::Parse(std::string const& content) {
            Register result;
            return result;
        }

        std::string const Register::ToString() const {
            std::string result;
            return result;
        }

        void Register::modifyFieldByDB_(int argc, char** argv, char** columnName, Util::shared_ptr<Register> item) {
            item->id(atoi(argv[Util::BaseData::GetIndexByName(argc, columnName, "id")]));
            //item->mobileNumber = Util::StringOp::FromUTF8(argv[Util::BaseData::GetIndexByName(argc, columnName, "mobileNumber")]).c_str();
            //item->nickname = Util::StringOp::FromUTF8(argv[Util::BaseData::GetIndexByName(argc, columnName, "nickname")]).c_str();
            //item->type = static_cast<Type>(atoi(argv[Util::BaseData::GetIndexByName(argc, columnName, "type")]));
        }

        std::vector<Util::shared_ptr<RegisterReply> > const RegisterReply::Select(std::wstring const& filter) {
            return GetDatasByFilter(filter, modifyFieldByDB_);
        }

        void RegisterReply::Update() const {
            std::wstring cmd = L"UPDATE ";
            //cmd += tableName_;
            //cmd += L" SET [mobileNumber] = '";
            //cmd += static_cast<LPCTSTR>(mobileNumber);
            //cmd += L"', [nickname] = '";
            //cmd += static_cast<LPCTSTR>(nickname);
            //cmd += L"', [type] = ";
            //cmd += Util::StringOp::FromInt(type);
            //cmd += L" WHERE id = ";
            //cmd += Util::StringOp::FromInt(id());
            ExecCommand(cmd);
        }

        void RegisterReply::Insert() {
            std::wstring cmd = L"INSERT INTO ";
            cmd += tableName_;
            //cmd += L" ( mobileNumber, nickname, type ) VALUES ( '";
            //cmd += static_cast<LPCTSTR>(mobileNumber);
            //cmd += L"', '";
            //cmd += static_cast<LPCTSTR>(nickname);
            //cmd += L"', ";
            //cmd += Util::StringOp::FromInt(type);
            cmd += L" )";
            ExecCommand(cmd);
            id(GetCurrentId());
        }

        void RegisterReply::Remove() const {
            std::wstring filter = L"id = " + Util::StringOp::FromInt(id());
            Remove(filter);
        }

        void RegisterReply::Remove(std::wstring filter) {
            RemoveDatasByFilter(filter);
        }

        RegisterReply const RegisterReply::Parse(std::string const& content) {
            RegisterReply result;
            return result;
        }

        void RegisterReply::modifyFieldByDB_(int argc, char** argv, char** columnName, Util::shared_ptr<RegisterReply> item) {
            item->id(atoi(argv[Util::BaseData::GetIndexByName(argc, columnName, "id")]));
            //item->mobileNumber = Util::StringOp::FromUTF8(argv[Util::BaseData::GetIndexByName(argc, columnName, "mobileNumber")]).c_str();
            //item->nickname = Util::StringOp::FromUTF8(argv[Util::BaseData::GetIndexByName(argc, columnName, "nickname")]).c_str();
            //item->type = static_cast<Type>(atoi(argv[Util::BaseData::GetIndexByName(argc, columnName, "type")]));
        }

        std::vector<Util::shared_ptr<Weather> > const Weather::Select(std::wstring const& filter) {
            return GetDatasByFilter(filter, modifyFieldByDB_);
        }

        void Weather::Update() const {
            std::wstring cmd = L"UPDATE ";
            //cmd += tableName_;
            //cmd += L" SET [mobileNumber] = '";
            //cmd += static_cast<LPCTSTR>(mobileNumber);
            //cmd += L"', [nickname] = '";
            //cmd += static_cast<LPCTSTR>(nickname);
            //cmd += L"', [type] = ";
            //cmd += Util::StringOp::FromInt(type);
            //cmd += L" WHERE id = ";
            //cmd += Util::StringOp::FromInt(id());
            ExecCommand(cmd);
        }

        void Weather::Insert() {
            std::wstring cmd = L"INSERT INTO ";
            cmd += tableName_;
            //cmd += L" ( mobileNumber, nickname, type ) VALUES ( '";
            //cmd += static_cast<LPCTSTR>(mobileNumber);
            //cmd += L"', '";
            //cmd += static_cast<LPCTSTR>(nickname);
            //cmd += L"', ";
            //cmd += Util::StringOp::FromInt(type);
            cmd += L" )";
            ExecCommand(cmd);
            id(GetCurrentId());
        }

        void Weather::Remove() const {
            std::wstring filter = L"id = " + Util::StringOp::FromInt(id());
            Remove(filter);
        }

        void Weather::Remove(std::wstring filter) {
            RemoveDatasByFilter(filter);
        }

        Weather const Weather::Parse(std::string const& content) {
            Weather result;
            return result;
        }

        void Weather::modifyFieldByDB_(int argc, char** argv, char** columnName, Util::shared_ptr<Weather> item) {
            item->id(atoi(argv[Util::BaseData::GetIndexByName(argc, columnName, "id")]));
            //item->mobileNumber = Util::StringOp::FromUTF8(argv[Util::BaseData::GetIndexByName(argc, columnName, "mobileNumber")]).c_str();
            //item->nickname = Util::StringOp::FromUTF8(argv[Util::BaseData::GetIndexByName(argc, columnName, "nickname")]).c_str();
            //item->type = static_cast<Type>(atoi(argv[Util::BaseData::GetIndexByName(argc, columnName, "type")]));
        }

        std::vector<Util::shared_ptr<SoftwareDownload> > const SoftwareDownload::Select(std::wstring const& filter) {
            return GetDatasByFilter(filter, modifyFieldByDB_);
        }

        void SoftwareDownload::Update() const {
            std::wstring cmd = L"UPDATE ";
            //cmd += tableName_;
            //cmd += L" SET [mobileNumber] = '";
            //cmd += static_cast<LPCTSTR>(mobileNumber);
            //cmd += L"', [nickname] = '";
            //cmd += static_cast<LPCTSTR>(nickname);
            //cmd += L"', [type] = ";
            //cmd += Util::StringOp::FromInt(type);
            //cmd += L" WHERE id = ";
            //cmd += Util::StringOp::FromInt(id());
            ExecCommand(cmd);
        }

        void SoftwareDownload::Insert() {
            std::wstring cmd = L"INSERT INTO ";
            cmd += tableName_;
            //cmd += L" ( mobileNumber, nickname, type ) VALUES ( '";
            //cmd += static_cast<LPCTSTR>(mobileNumber);
            //cmd += L"', '";
            //cmd += static_cast<LPCTSTR>(nickname);
            //cmd += L"', ";
            //cmd += Util::StringOp::FromInt(type);
            cmd += L" )";
            ExecCommand(cmd);
            id(GetCurrentId());
        }

        void SoftwareDownload::Remove() const {
            std::wstring filter = L"id = " + Util::StringOp::FromInt(id());
            Remove(filter);
        }

        void SoftwareDownload::Remove(std::wstring filter) {
            RemoveDatasByFilter(filter);
        }

        SoftwareDownload const SoftwareDownload::Parse(std::string const& content) {
            SoftwareDownload result;
            return result;
        }

        void SoftwareDownload::modifyFieldByDB_(int argc, char** argv, char** columnName, Util::shared_ptr<SoftwareDownload> item) {
            item->id(atoi(argv[Util::BaseData::GetIndexByName(argc, columnName, "id")]));
            //item->mobileNumber = Util::StringOp::FromUTF8(argv[Util::BaseData::GetIndexByName(argc, columnName, "mobileNumber")]).c_str();
            //item->nickname = Util::StringOp::FromUTF8(argv[Util::BaseData::GetIndexByName(argc, columnName, "nickname")]).c_str();
            //item->type = static_cast<Type>(atoi(argv[Util::BaseData::GetIndexByName(argc, columnName, "type")]));
        }

        std::vector<Util::shared_ptr<MultimediaDownload> > const MultimediaDownload::Select(std::wstring const& filter) {
            return GetDatasByFilter(filter, modifyFieldByDB_);
        }

        void MultimediaDownload::Update() const {
            std::wstring cmd = L"UPDATE ";
            //cmd += tableName_;
            //cmd += L" SET [mobileNumber] = '";
            //cmd += static_cast<LPCTSTR>(mobileNumber);
            //cmd += L"', [nickname] = '";
            //cmd += static_cast<LPCTSTR>(nickname);
            //cmd += L"', [type] = ";
            //cmd += Util::StringOp::FromInt(type);
            //cmd += L" WHERE id = ";
            //cmd += Util::StringOp::FromInt(id());
            ExecCommand(cmd);
        }

        void MultimediaDownload::Insert() {
            std::wstring cmd = L"INSERT INTO ";
            cmd += tableName_;
            //cmd += L" ( mobileNumber, nickname, type ) VALUES ( '";
            //cmd += static_cast<LPCTSTR>(mobileNumber);
            //cmd += L"', '";
            //cmd += static_cast<LPCTSTR>(nickname);
            //cmd += L"', ";
            //cmd += Util::StringOp::FromInt(type);
            cmd += L" )";
            ExecCommand(cmd);
            id(GetCurrentId());
        }

        void MultimediaDownload::Remove() const {
            std::wstring filter = L"id = " + Util::StringOp::FromInt(id());
            Remove(filter);
        }

        void MultimediaDownload::Remove(std::wstring filter) {
            RemoveDatasByFilter(filter);
        }

        MultimediaDownload const MultimediaDownload::Parse(std::string const& content) {
            MultimediaDownload result;
            return result;
        }

        void MultimediaDownload::modifyFieldByDB_(int argc, char** argv, char** columnName, Util::shared_ptr<MultimediaDownload> item) {
            item->id(atoi(argv[Util::BaseData::GetIndexByName(argc, columnName, "id")]));
            //item->mobileNumber = Util::StringOp::FromUTF8(argv[Util::BaseData::GetIndexByName(argc, columnName, "mobileNumber")]).c_str();
            //item->nickname = Util::StringOp::FromUTF8(argv[Util::BaseData::GetIndexByName(argc, columnName, "nickname")]).c_str();
            //item->type = static_cast<Type>(atoi(argv[Util::BaseData::GetIndexByName(argc, columnName, "type")]));
        }

        std::vector<Util::shared_ptr<MenuManager> > const MenuManager::Select(std::wstring const& filter) {
            return GetDatasByFilter(filter, modifyFieldByDB_);
        }

        void MenuManager::Update() const {
            std::wstring cmd = L"UPDATE ";
            //cmd += tableName_;
            //cmd += L" SET [mobileNumber] = '";
            //cmd += static_cast<LPCTSTR>(mobileNumber);
            //cmd += L"', [nickname] = '";
            //cmd += static_cast<LPCTSTR>(nickname);
            //cmd += L"', [type] = ";
            //cmd += Util::StringOp::FromInt(type);
            //cmd += L" WHERE id = ";
            //cmd += Util::StringOp::FromInt(id());
            ExecCommand(cmd);
        }

        void MenuManager::Insert() {
            std::wstring cmd = L"INSERT INTO ";
            cmd += tableName_;
            //cmd += L" ( mobileNumber, nickname, type ) VALUES ( '";
            //cmd += static_cast<LPCTSTR>(mobileNumber);
            //cmd += L"', '";
            //cmd += static_cast<LPCTSTR>(nickname);
            //cmd += L"', ";
            //cmd += Util::StringOp::FromInt(type);
            cmd += L" )";
            ExecCommand(cmd);
            id(GetCurrentId());
        }

        void MenuManager::Remove() const {
            std::wstring filter = L"id = " + Util::StringOp::FromInt(id());
            Remove(filter);
        }

        void MenuManager::Remove(std::wstring filter) {
            RemoveDatasByFilter(filter);
        }

        MenuManager const MenuManager::Parse(std::string const& content) {
            MenuManager result;
            return result;
        }

        void MenuManager::modifyFieldByDB_(int argc, char** argv, char** columnName, Util::shared_ptr<MenuManager> item) {
            item->id(atoi(argv[Util::BaseData::GetIndexByName(argc, columnName, "id")]));
            //item->mobileNumber = Util::StringOp::FromUTF8(argv[Util::BaseData::GetIndexByName(argc, columnName, "mobileNumber")]).c_str();
            //item->nickname = Util::StringOp::FromUTF8(argv[Util::BaseData::GetIndexByName(argc, columnName, "nickname")]).c_str();
            //item->type = static_cast<Type>(atoi(argv[Util::BaseData::GetIndexByName(argc, columnName, "type")]));
        }
    }
}