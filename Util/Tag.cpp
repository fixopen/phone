#include "Tag.h"

namespace Util {
    std::wstring DataAccess<ObjectCategory>::tableName_ = L"object";
    int DataAccess<ObjectCategory>::offset_ = 0;
    int DataAccess<ObjectCategory>::count_ = 0;

    int const ObjectCategory::GetIdByName(std::wstring const& name);
    std::vector<shared_ptr<ObjectCategory> > const ObjectCategory::Select(std::wstring const& filter = L"");
    void ObjectCategory::Update() const; //sync to database
    void ObjectCategory::Insert(); //insert new instance to database

    std::wstring DataAccess<Tag>::tableName_ = L"tag";
    int DataAccess<Tag>::offset_ = 0;
    int DataAccess<Tag>::count_ = 0;

    std::wstring const Tag::Get(std::wstring const& name) {
        std::wstring value;
        std::vector<Util::shared_ptr<Tag> > properties = GetDatasByFilter(L"[type] = 1 AND [name] = '" + name + L"'", modifyFieldByDB_);
        if (properties.size() == 1) {
            value = properties[0]->value;
        }
        return value;
    }

    void Tag::Set(std::wstring const& name, std::wstring const& value) {
        std::vector<Util::shared_ptr<Tag> > properties = GetDatasByFilter(L"[type] = 1 AND [name] = '" + name + L"'", modifyFieldByDB_);
        if (properties.size() == 1) { //has this properties
            properties[0]->value = value;
            properties[0]->Update();
        } else { //not has this properties
            Tag prop;
            prop.type = tCurrent;
            prop.catagory = L"";
            prop.name = name;
            prop.value = value;
            prop.Insert();
        }
    }

    void Tag::Update() const {
        std::wstring cmd = L"UPDATE ";
        cmd += tableName_;
        cmd += L" SET [name] = '";
        cmd += name;
        cmd += L"', [value] = '";
        cmd += value;
        cmd += L"', [catagory] = '";
        cmd += catagory;
        cmd += L"', [type] = ";
        cmd += Text::StringOp::FromInt(type);
        cmd += L" WHERE id = ";
        cmd += Text::StringOp::FromInt(id());
        ExecCommand(cmd);
    }

    void Tag::Insert() {
        std::wstring cmd = L"INSERT INTO ";
        cmd += tableName_;
        cmd += L" ( name, value, catagory, type ) VALUES ( '";
        cmd += name;
        cmd += L"', '";
        cmd += value;
        cmd += L"', '";
        cmd += catagory;
        cmd += L"', ";
        cmd += Text::StringOp::FromInt(type);
        cmd += L" )";
        ExecCommand(cmd);
        id(GetCurrentId());
    }

    void Tag::modifyFieldByDB_(int argc, char** argv, char** columnName, Util::shared_ptr<Tag>& item) {
        item->id(atoi(argv[GetIndexByName(argc, columnName, "id")]));
        item->name = Text::StringOp::FromUTF8(argv[GetIndexByName(argc, columnName, "name")]);
        item->value = Text::StringOp::FromUTF8(argv[GetIndexByName(argc, columnName, "value")]);
        item->catagory = Text::StringOp::FromUTF8(argv[GetIndexByName(argc, columnName, "catagory")]);
        item->type = static_cast<Type>(atoi(argv[GetIndexByName(argc, columnName, "type")]));
    }
}
