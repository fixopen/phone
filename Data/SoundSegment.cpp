#include "stdafx.h"
#include "SoundSegment.h"
#include "../Util/StringOp.h"

namespace Data
{
	std::string DataAccess<SoundSegment>::tableName_ = "soundSegment";
	sqlite3* DataAccess<SoundSegment>::db_ = Data::GetDB();
	Indication DataAccess<SoundSegment>::indication_ = 0;

	std::string SoundSegment::folderName_;
	SoundSegment SoundSegment::DefaultRing;
	SoundSegment SoundSegment::VipRing;
	SoundSegment SoundSegment::BlacklistRing;
	SoundSegment SoundSegment::DefaultSMRing;
	SoundSegment SoundSegment::VipSMRing;
	SoundSegment SoundSegment::BlacklistSMRing;
	SoundSegment SoundSegment::DefaultTips;
	SoundSegment SoundSegment::VipTips;
	SoundSegment SoundSegment::BlacklistTips;

	SoundSegment::SoundSegment()
		: DataAccess<SoundSegment>()
        //, filename_()
		//, duration_()
		//, type_()
	{
	}

	SoundSegment::~SoundSegment()
	{
		//do nothing
	}

	std::vector<boost::shared_ptr<SoundSegment> > SoundSegment::GetFromDatabase(std::string filter)
	{
		return DataAccess<SoundSegment>::GetDatasByFilter(filter, modifyFieldByDB_);
	}

	void SoundSegment::Update() const
	{
        std::string cmd = "UPDATE ";
        cmd += tableName();
        cmd += " SET [filename] = '";
        cmd += filename_;
        cmd += "', [filesize] = ";
        cmd += Util::StringOp::FromTimeSpan(duration_);
        cmd += ", [type] = ";
        cmd += Util::StringOp::FromInt(type_);
        cmd += " WHERE id = ";
        cmd += Util::StringOp::FromInt(id());
        ExecCommand(cmd);
	}

	void SoundSegment::Insert()
	{
        std::string cmd = "INSERT INTO ";
        cmd += tableName();
        cmd += " ( filename, duration, type, ) VALUES ( '";
        cmd += filename_;
        cmd += "', ";
        cmd += Util::StringOp::FromTimeSpan(duration_);
        cmd += ", ";
        cmd += Util::StringOp::FromInt(type_);
        cmd += " )";
        ExecCommand(cmd);
		id(GetCurrentId());
	}

    void SoundSegment::Remove() const
    {
        SoundSegment::Remove("id = " + Util::StringOp::FromInt(id()));
    }

	void SoundSegment::Remove(std::string filter)
	{
		DataAccess<SoundSegment>::RemoveDatasByFilter(filter);
	}

	void SoundSegment::modifyFieldByDB_(int argc, char** argv, char** columnName, boost::shared_ptr<SoundSegment> item)
	{
        item->id(atoi(argv[Data::getIndexByName(argc, columnName, "id")]));
        item->filename_ = argv[Data::getIndexByName(argc, columnName, "filename")];
        item->duration_ = Util::StringOp::ToTimeSpan(std::string((char*)argv[Data::getIndexByName(argc, columnName, "duration")]));
        item->type_ = static_cast<SoundSegmentType>(atoi(argv[Data::getIndexByName(argc, columnName, "type")]));
	}
}