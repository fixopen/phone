#include "StdAfx.h"
#include "Properties.h"

namespace Util {
    std::wstring DataAccess<Properties>::tableName_ = L"properties";
    int DataAccess<Properties>::offset_ = 0;
    int DataAccess<Properties>::count_ = 0;
}

CString const Properties::Get(CString const& name) {
    CString value;
    std::vector<Util::shared_ptr<Properties> > properties = DataAccess<Properties>::GetDatasByFilter(L"[type] = 1 AND [name] = '" + std::wstring(static_cast<LPCTSTR>(name)) + L"'", modifyFieldByDB_);
    if (properties.size() == 1) {
        value = properties[0]->value;
    }
    return value;
}

void Properties::Set(CString const& name, CString const& value) {
    std::vector<Util::shared_ptr<Properties> > properties = DataAccess<Properties>::GetDatasByFilter(L"[type] = 1 AND [name] = '" + std::wstring(static_cast<LPCTSTR>(name)) + L"'", modifyFieldByDB_);
    if (properties.size() == 1) { //has this properties
        properties[0]->value = value;
        properties[0]->Update();
    } else { //not has this properties
        Properties prop;
        prop.type = tCurrent;
        prop.catagory = L"";
        prop.name = name;
        prop.value = value;
        prop.Insert();
    }
}

void Properties::Update() const {
    std::wstring cmd = L"UPDATE ";
    cmd += tableName_;
    cmd += L" SET [name] = '";
    cmd += static_cast<LPCTSTR>(name);
    cmd += L"', [value] = '";
    cmd += static_cast<LPCTSTR>(value);
    cmd += L"', [catagory] = '";
    cmd += static_cast<LPCTSTR>(catagory);
    cmd += L"', [type] = ";
    cmd += Util::StringOp::FromInt(type);
    cmd += L" WHERE id = ";
    cmd += Util::StringOp::FromInt(id());
    ExecCommand(cmd);
}

void Properties::Insert() {
    std::wstring cmd = L"INSERT INTO ";
    cmd += tableName_;
    cmd += L" ( name, value, catagory, type ) VALUES ( '";
    cmd += static_cast<LPCTSTR>(name);
    cmd += L"', '";
    cmd += static_cast<LPCTSTR>(value);
    cmd += L"', '";
    cmd += static_cast<LPCTSTR>(catagory);
    cmd += L"', ";
    cmd += Util::StringOp::FromInt(type);
    cmd += L" )";
    ExecCommand(cmd);
    id(GetCurrentId());
}

void Properties::modifyFieldByDB_(int argc, char** argv, char** columnName, Util::shared_ptr<Properties> item) {
    item->id(atoi(argv[Util::BaseData::GetIndexByName(argc, columnName, "id")]));
    item->name = Util::StringOp::FromUTF8(argv[Util::BaseData::GetIndexByName(argc, columnName, "name")]).c_str();
    item->value = Util::StringOp::FromUTF8(argv[Util::BaseData::GetIndexByName(argc, columnName, "value")]).c_str();
    item->catagory = Util::StringOp::FromUTF8(argv[Util::BaseData::GetIndexByName(argc, columnName, "catagory")]).c_str();
    item->type = static_cast<Type>(atoi(argv[Util::BaseData::GetIndexByName(argc, columnName, "type")]));
}
