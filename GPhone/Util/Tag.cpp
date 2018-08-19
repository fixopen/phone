#include "Tag.h"

namespace Util {
    std::wstring DataAccess<ObjectCategory>::tableName_ = L"objectCategory";
    int DataAccess<ObjectCategory>::offset_ = 0;
    int DataAccess<ObjectCategory>::count_ = 0;

    int const ObjectCategory::GetIdByName(std::wstring const& name) {
        int result = 0;
        return result;
    }

    std::vector<shared_ptr<ObjectCategory> > const ObjectCategory::Select(std::wstring const& filter) {
        return GetDatasByFilter(filter, modifyFieldByDB_);
    }

    void ObjectCategory::Update() const {
        std::wstring cmd = L"UPDATE ";
        cmd += tableName_;
        cmd += L" SET [name] = '";
        cmd += name;
        cmd += L"' WHERE id = ";
        cmd += Text::StringOp::FromInt(id());
        ExecCommand(cmd);
    }

    void ObjectCategory::Insert() {
        std::wstring cmd = L"INSERT INTO ";
        cmd += tableName_;
        cmd += L" ( name ) VALUES ( '";
        cmd += name;
        cmd += L"' )";
        ExecCommand(cmd);
        id(GetCurrentId());
    }

    void ObjectCategory::modifyFieldByDB_(int argc, char** argv, char** columnName, Util::shared_ptr<ObjectCategory>& item) {
        item->id(atoi(argv[GetIndexByName(argc, columnName, "id")]));
        item->name = Text::StringOp::FromUTF8(argv[GetIndexByName(argc, columnName, "name")]);
    }

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
        std::vector<Util::shared_ptr<Tag> > tags = GetDatasByFilter(L"[type] = 1 AND [name] = '" + name + L"'", modifyFieldByDB_);
        if (tags.size() == 1) { //has this properties
            tags[0]->value = value;
            tags[0]->Update();
        } else { //not has this properties
            Tag tag;
            tag.objectCategoryId = 0;
            tag.value = value;
            tag.type = tCurrent;
            tag.Insert();
        }
    }

    void Tag::Update() const {
        std::wstring cmd = L"UPDATE ";
        cmd += tableName_;
        cmd += L" SET [objectCategoryId] = ";
        cmd += Text::StringOp::FromInt(objectCategoryId);
        cmd += L", [value] = '";
        cmd += value;
        cmd += L"', [type] = ";
        cmd += Text::StringOp::FromInt(type);
        cmd += L" WHERE id = ";
        cmd += Text::StringOp::FromInt(id());
        ExecCommand(cmd);
    }

    void Tag::Insert() {
        std::wstring cmd = L"INSERT INTO ";
        cmd += tableName_;
        cmd += L" ( objectCategoryId, value, type ) VALUES ( ";
        cmd += Text::StringOp::FromInt(objectCategoryId);
        cmd += L", '";
        cmd += value;
        cmd += L"', ";
        cmd += Text::StringOp::FromInt(type);
        cmd += L" )";
        ExecCommand(cmd);
        id(GetCurrentId());
    }

    void Tag::modifyFieldByDB_(int argc, char** argv, char** columnName, Util::shared_ptr<Tag>& item) {
        item->id(atoi(argv[GetIndexByName(argc, columnName, "id")]));
        item->objectCategoryId = Text::StringOp::ToInt(argv[GetIndexByName(argc, columnName, "catagory")]);
        item->value = Text::StringOp::FromUTF8(argv[GetIndexByName(argc, columnName, "value")]);
        item->type = static_cast<Type>(atoi(argv[GetIndexByName(argc, columnName, "type")]));
    }
}
