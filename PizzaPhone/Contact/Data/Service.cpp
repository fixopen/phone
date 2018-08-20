#include "Service.h"

namespace Util {
    std::wstring DataAccess<Contact::Data::Service>::tableName_ = L"service";
    int DataAccess<Contact::Data::Service>::count_ = 0;
    int DataAccess<Contact::Data::Service>::offset_ = 0;
}

namespace Contact {
    namespace Data {
        Service::Service(void)
        : DataAccess<Service>() {
        }

        std::vector<Util::shared_ptr<Service> > Service::Select(std::wstring const & filter, std::wstring const & orderFieldName, SeqenceRelation const sr, int const offset, int const pageSize) {
            return GetDatasByFilter(filter, modifyFieldByDB_, orderFieldName, sr, offset, pageSize);
        }

        void Service::Update() const {
            std::wstring cmd = L"UPDATE [";
            cmd += tableName_;
            cmd += L"] SET [name] = '";
            cmd += name_;
            cmd += L"', [code] = '";
            cmd += code_;
            cmd += L"', [welcomeInfo] = '";
            cmd += welcomeInfo_;
            cmd += L"' WHERE [id] = ";
            cmd += Util::Text::StringOp::FromInt(id());
            ExecCommand(cmd);
        }

        void Service::Insert() {
            std::wstring cmd = L"INSERT INTO [";
            cmd += tableName_;
            cmd += L"] ( [name], [code], [welcomeInfo] ) VALUES ( '";
            cmd += name_;
            cmd += L"', '";
            cmd += code_;
            cmd += L"', '";
            cmd += welcomeInfo_;
            cmd += L"' )";
            ExecCommand(cmd);
            id(GetCurrentId());
        }

        void Service::Remove() const {
            Remove(L"[id] = " + Util::Text::StringOp::FromInt(id()));
        }

        void Service::Remove(std::wstring const & filter) {
            RemoveDatasByFilter(filter);
        }

        void Service::modifyFieldByDB_(int argc, char** argv, char** columnName, Util::shared_ptr<Service>& item) {
            item->id(atoi(argv[GetIndexByName(argc, columnName, "id")]));
            item->name_ = Util::Text::StringOp::FromUTF8(argv[GetIndexByName(argc, columnName, "name")]);
            item->code_ = Util::Text::StringOp::FromUTF8(argv[GetIndexByName(argc, columnName, "code")]);
            item->welcomeInfo_ = Util::Text::StringOp::FromUTF8(argv[GetIndexByName(argc, columnName, "welcomeInfo")]);
        }
    }
}
