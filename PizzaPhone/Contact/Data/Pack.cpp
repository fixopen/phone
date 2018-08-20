#include "Pack.h"

namespace Util {
    std::wstring DataAccess<Contact::Data::Pack>::tableName_ = L"pack";
    int DataAccess<Contact::Data::Pack>::count_ = 0;
    int DataAccess<Contact::Data::Pack>::offset_ = 0;
}

namespace Contact {
    namespace Data {
        Pack::Pack()
        : DataAccess<Pack>()
        , serviceId_(0)
        , isOrder_(false) {
        }

        std::vector<Util::shared_ptr<Pack> > Pack::Select(std::wstring const & filter, std::wstring const & orderFieldName, SeqenceRelation const sr, int const offset, int const pageSize) {
            return GetDatasByFilter(filter, modifyFieldByDB_, orderFieldName, sr, offset, pageSize);
        }

        void Pack::Update() const {
            std::wstring cmd = L"UPDATE [";
            cmd += tableName_;
            cmd += L"] SET [serviceId] = ";
            cmd += Util::Text::StringOp::FromInt(serviceId_);
            cmd += L", [type] = '";
            cmd += type_;
            cmd += L"', [no] = '";
            cmd += no_;
            cmd += L"', [rate] = '";
            cmd += rate_;
            cmd += L"' [isOrder] = ";
            cmd += Util::Text::StringOp::FromInt(isOrder_);
            cmd += L" WHERE [id] = ";
            cmd += Util::Text::StringOp::FromInt(id());
            ExecCommand(cmd);
        }

        void Pack::Insert() {
            std::wstring cmd = L"INSERT INTO [";
            cmd += tableName_;
            cmd += L"] ( [serviceId], [type], [no], [rate], [isOrder] ) VALUES ( ";
            cmd += Util::Text::StringOp::FromInt(serviceId_);
            cmd += L", '";
            cmd += type_;
            cmd += L"', '";
            cmd += no_;
            cmd += L"', '";
            cmd += rate_;
            cmd += L"', ";
            cmd += Util::Text::StringOp::FromInt(isOrder_);
            cmd += L" )";
            ExecCommand(cmd);
            id(GetCurrentId());
        }

        void Pack::Remove() const {
            Remove(L"[id] = " + Util::Text::StringOp::FromInt(id()));
        }

        void Pack::Remove(std::wstring const & filter) {
            RemoveDatasByFilter(filter);
        }

        void Pack::modifyFieldByDB_(int argc, char** argv, char** columnName, Util::shared_ptr<Pack>& item) {
            item->id(atoi(argv[GetIndexByName(argc, columnName, "id")]));
            item->serviceId_ = Util::Text::StringOp::ToInt(argv[GetIndexByName(argc, columnName, "serviceId")]);
            item->type_ = Util::Text::StringOp::FromUTF8(argv[GetIndexByName(argc, columnName, "type")]);
            item->no_ = Util::Text::StringOp::FromUTF8(argv[GetIndexByName(argc, columnName, "no")]);
            item->rate_ = Util::Text::StringOp::FromUTF8(argv[GetIndexByName(argc, columnName, "rate")]);
            item->isOrder_ = !!Util::Text::StringOp::ToInt(argv[GetIndexByName(argc, columnName, "isOrder")]);
        }
    }
}
