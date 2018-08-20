#include "stdafx.h"
#include "LanguageResource.h"
#include "LangRes.h"
#include "../Util/StringOp.h"

namespace Data
{
	std::string DataAccess<LanguageResource>::tableName_ = "chineseResource";
	sqlite3* DataAccess<LanguageResource>::db_ = Data::GetDB();
    int DataAccess<LanguageResource>::rowCount_ = 0;
	Indication DataAccess<LanguageResource>::indication_ = 0;

	Language LanguageResource::language_ = lChinese;

    void LanguageResource::InsertToDatabase()
    {
        for (int i = 0; i < sizeof langResource / sizeof langResource[0]; ++i)
        {
            LanguageResource lr;
            lr.id(langResource[i].id);
            lr.value(langResource[i].value);
            lr.Insert();
        }
    }

    LanguageResource::LanguageResource()
        : DataAccess<LanguageResource>()
        //, value_()
    {
    }

	LanguageResource::~LanguageResource()
	{
	}

	std::vector<boost::shared_ptr<LanguageResource> > LanguageResource::GetFromDatabase(std::string filter, Direction const dir, int const id, int const pageSize)
	{
		return DataAccess<LanguageResource>::GetDatasByFilter(filter, modifyFieldByDB_, dir, id, pageSize);
	}

	std::string LanguageResource::Get(int const id)
	{
        // method1:
		//boost::shared_ptr<LanguageResource> lr = DataAccess<LanguageResource>::GetDataById(id, modifyFieldByDB_);
		//return lr->value();
        // 
        // method2:
        //find from langResource array
        std::string result;
        for (int i = 0; i < sizeof langResource / sizeof langResource[0]; ++i)
        {
            if (langResource[i].id == id)
            {
                result = langResource[i].value;
                break;
            }
        }
        return result;
	}

	void LanguageResource::Update() const
	{
        std::string cmd = "UPDATE ";
        cmd += tableName();
        cmd += " SET [value] = '";
        cmd += value_;
        cmd += "' WHERE id = ";
        cmd += Util::StringOp::FromInt(id());
        ExecCommand(cmd);
	}

	void LanguageResource::Insert() const
	{
        std::string cmd = "INSERT INTO ";
        cmd += tableName();
        cmd += " ( id, value ) VALUES ( ";
        cmd += Util::StringOp::FromInt(id());
        cmd += ", '";
        cmd += value_;
        cmd += "' )";
        ExecCommand(cmd);
	}

	void LanguageResource::Remove(std::string filter)
	{
		DataAccess<LanguageResource>::RemoveDatasByFilter(filter);
	}

	void LanguageResource::modifyFieldByDB_(int argc, char** argv, char** columnName, boost::shared_ptr<LanguageResource> item)
	{
        item->id(atoi(argv[Data::getIndexByName(argc, columnName, "id")]));
        item->value_ = argv[Data::getIndexByName(argc, columnName, "value")];
	}
}
