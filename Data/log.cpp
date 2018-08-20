#include "stdafx.h"
#include "log.h"

#include "../Util/StringOp.h"

namespace Data
{
	std::string DataAccess<Log>::tableName_ = "log";
	sqlite3* DataAccess<Log>::db_ = Data::GetDB();
    int DataAccess<Log>::rowCount_ = 0;
	Indication DataAccess<Log>::indication_ = 0;

	Log::Log()
		: level_(llError)
		, who_()
		, when_() //Util::Timestamp::GetCurrentTime()
		//, what_("")
		//, code_(ecSystemError)
		, value_(0.0)
	{
	}

	Log::~Log()
	{
	}
	
	std::vector<boost::shared_ptr<Log> > Log::GetFromDatabase(std::string filter, Direction const dir, int const id, int const pageSize)
	{
		return DataAccess<Log>::GetDatasByFilter(filter, modifyFieldByDB_, dir, id, pageSize);
	}

	void Log::Update() const
	{
        std::string cmd = "UPDATE ";
        cmd += tableName();
        cmd += " SET [level] = ";
        cmd += Util::StringOp::FromInt(level_);
        cmd += ", [who] = '";
        cmd += value_;
        cmd += ", [when] = '";
        cmd += value_;
        cmd += ", [value] = '";
        cmd += value_;
        cmd += ", [value] = '";
        cmd += value_;
        cmd += ", [value] = '";
        cmd += value_;
        cmd += ", [value] = '";
        cmd += value_;
        cmd += ", [value] = '";
        cmd += value_;
        cmd += ", [value] = '";
        cmd += value_;
        cmd += ", [value] = '";
        cmd += value_;
        cmd += ", [value] = '";
        cmd += value_;
        cmd += ", [value] = '";
        cmd += value_;
        cmd += "' WHERE id = ";
        cmd += Util::StringOp::FromInt(id());
        ExecCommand(cmd);
	}

	void Log::Insert()
	{
        std::string cmd = "INSERT INTO ";
        cmd += tableName();
        cmd += " ( level, who, when, what, code, value ) VALUES ( ";
        cmd += Util::StringOp::FromInt(level_);
        cmd += ", '";
        cmd += who_;
        cmd += "', '";
        cmd += Util::StringOp::FromTimestamp(when_);
        cmd += "', '";
        cmd += what_;
        cmd += "', ";
        cmd += Util::StringOp::FromInt(code_);
        cmd += ", ";
        cmd += Util::StringOp::FromDouble(value_);
        cmd += " )";
        ExecCommand(cmd);
		id(GetCurrentId());
	}
	
	void Log::Remove(std::string filter)
	{
		DataAccess<Log>::RemoveDatasByFilter(filter);
	}

	std::string Log::ToString(void)
	{
		std::string result = "<ITEM>\n\t<LEVEL>";
		/*
		result += Util::StringOp::FromEnumLogLevel(level);
		result += "</LEVEL>\n\t<WHO>";
		result += who;
		result += "</WHO>\n\t<TIME>";
		result += Util::StringOp::FromTimestamp(when); //yyyy:mm:dd:hh:mm:ss;
		result += "</TIME>\n\t<CODE>";
		result += Util::StringOp::FromEnumErrorCode(code);
		result += "</CODE>\n\t<DESCRIPTION>";
		result += what;
		result += "</DESCRIPTION>\n\t<VALUE>";
		result += Util::StringOp::FromDouble(value);
		result += "</VALUE>\n</ITEM>";
		*/
		return result;
	}

	void Log::modifyFieldByDB_(int argc, char** argv, char** columnName, boost::shared_ptr<Log> item)
	{
        item->id(atoi(argv[Data::getIndexByName(argc, columnName, "id")]));
        item->level_ = static_cast<LogLevel>(atoi(argv[Data::getIndexByName(argc, columnName, "value")]));
        item->who_ = argv[Data::getIndexByName(argc, columnName, "value")];
        item->when_ = Util::StringOp::ToTimestamp(std::string((char*)argv[Data::getIndexByName(argc, columnName, "value")]));
        item->what_ = argv[Data::getIndexByName(argc, columnName, "value")];
        item->code_ = static_cast<LogCode>(atoi(argv[Data::getIndexByName(argc, columnName, "value")]));
        item->value_ = atof(argv[Data::getIndexByName(argc, columnName, "value")]);
	}
}