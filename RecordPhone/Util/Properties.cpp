#include "Properties.h"

namespace Util {
    std::wstring DataAccess<Properties>::tableName_ = L"properties";
    int DataAccess<Properties>::offset_ = 0;
    int DataAccess<Properties>::count_ = 0;

    std::wstring const Properties::Get(std::wstring const& name) {
        std::wstring value;
        std::vector<Util::shared_ptr<Properties> > properties = GetDatasByFilter(L"[type] = 1 AND [name] = '" + name + L"'", modifyFieldByDB_);
        if (properties.size() == 1) {
            value = properties[0]->value;
        }
        return value;
    }

    void Properties::Set(std::wstring const& name, std::wstring const& value) {
        std::vector<Util::shared_ptr<Properties> > properties = DataAccess<Properties>::GetDatasByFilter(L"[type] = 1 AND [name] = '" + name + L"'", modifyFieldByDB_);
        if (properties.size() == 1) { //has this properties
            properties[0]->value = value;
            properties[0]->Update();
        } else { //not has this properties
            Properties prop;
            prop.type = tCurrent;
            prop.category = L"";
            prop.name = name;
            prop.value = value;
            prop.Insert();
        }
    }

    void Properties::Update() const {
        std::wstring cmd = L"UPDATE [";
        cmd += tableName_;
        cmd += L"] SET [name] = '";
        cmd += name;
        cmd += L"', [value] = '";
        cmd += value;
        cmd += L"', [category] = '";
        cmd += category;
        cmd += L"', [type] = ";
        cmd += Util::StringOp::FromInt(type);
        cmd += L" WHERE [id] = ";
        cmd += Util::StringOp::FromInt(id());
        ExecCommand(cmd);
    }

    void Properties::Insert() {
        std::wstring cmd = L"INSERT INTO [";
        cmd += tableName_;
        cmd += L"] ( [name], [value], [category], [type] ) VALUES ( '";
        cmd += name;
        cmd += L"', '";
        cmd += value;
        cmd += L"', '";
        cmd += category;
        cmd += L"', ";
        cmd += Util::StringOp::FromInt(type);
        cmd += L" )";
        ExecCommand(cmd);
        id(GetCurrentId());
    }

    void Properties::RestoreDefault(std::wstring const& category) {
        //ys :: SELECT name FROM properties WHERE type = 0 AND category = category
        //x :: WHERE name = ys[i] AND type = 0 AND category = category
        //UPDATE properties SET value = x WHERE name = y AND type = 1 AND category = category
    }

    void Properties::modifyFieldByDB_(int argc, char** argv, char** columnName, Util::shared_ptr<Properties>& item) {
        item->id(atoi(argv[Util::BaseData::GetIndexByName(argc, columnName, "id")]));
        item->name = Util::StringOp::FromUTF8(argv[Util::BaseData::GetIndexByName(argc, columnName, "name")]);
        item->value = Util::StringOp::FromUTF8(argv[Util::BaseData::GetIndexByName(argc, columnName, "value")]);
        item->category = Util::StringOp::FromUTF8(argv[Util::BaseData::GetIndexByName(argc, columnName, "category")]);
        item->type = static_cast<Type>(atoi(argv[Util::BaseData::GetIndexByName(argc, columnName, "type")]));
    }
}
