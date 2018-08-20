#include "stdafx.h"
#include "SoundSegment.h"
#include "../Util/StringOp.h"

namespace Data
{
	std::string DataAccess<SoundSegment>::tableName_ = "soundSegment";
	sqlite3* DataAccess<SoundSegment>::db_ = Data::GetDB();
    int DataAccess<SoundSegment>::rowCount_ = 0;
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
		, contactInfoId_(0)
		, contactId_(0)
	{
		id(0);
	}

	SoundSegment::~SoundSegment()
	{
		//do nothing
	}
	boost::shared_ptr<Contact> const SoundSegment::GetContact(void)
	{
		if (contactId_ != 0)
		{
			std::vector<boost::shared_ptr<Contact> > t = Contact::GetFromDatabase("id = " + Util::StringOp::FromInt(contactId_));
			if (t.size() != 0)
				contact_ = t[0];
		}
		return contact_;
	}

	std::vector<boost::shared_ptr<SoundSegment> > SoundSegment::GetFromDatabase(std::string filter, Direction const dir, int const id, int const pageSize)
	{
		return DataAccess<SoundSegment>::GetDatasByFilter(filter, modifyFieldByDB_, dir, id, pageSize);
	}

	void SoundSegment::Update() const
	{
        std::string cmd = "UPDATE ";
        cmd += tableName();
        cmd += " SET [filename] = '";
        cmd += filename_;
        cmd += "', duration = '";
        cmd += Util::StringOp::FromTimeSpan(duration_);
        cmd += "', [type] = ";
        cmd += Util::StringOp::FromInt(type_);
        cmd += ", startTime = '";
        cmd += Util::StringOp::FromTimestamp(startTime_);
		cmd += "', telephoneNumber = '";
        cmd += telephoneNumber_.number();
        cmd += "', contactId = ";
        cmd += Util::StringOp::FromInt(contactId_);
        cmd += ", contactInfoId = ";
        cmd += Util::StringOp::FromInt(contactInfoId_);
		cmd += ", name = '";
		cmd += name_;
        cmd += "' WHERE id = ";
        cmd += Util::StringOp::FromInt(id());
        ExecCommand(cmd);
	}

	void SoundSegment::Insert()
	{
        std::string cmd = "INSERT INTO ";
        cmd += tableName();
        cmd += " ( filename, duration, type , startTime, telephoneNumber, contactId, contactInfoId, name) VALUES ( '";
        cmd += filename_;
        cmd += "', '";
        cmd += Util::StringOp::FromTimeSpan(duration_);
        cmd += "', ";
        cmd += Util::StringOp::FromInt(type_);
        cmd += ", '";
        cmd += Util::StringOp::FromTimestamp(startTime_);
        cmd += "', '";
        cmd += telephoneNumber_.number();
        cmd += "', ";
        cmd += Util::StringOp::FromInt(contactId_);
        cmd += ", ";
        cmd += Util::StringOp::FromInt(contactInfoId_);
		cmd += ", '";
		cmd += name_;
        cmd += "' )";
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
        item->startTime_ = Util::StringOp::ToTimestamp(std::string(argv[Data::getIndexByName(argc, columnName, "startTime")]));
        item->telephoneNumber_ = TelephoneNumber(argv[Data::getIndexByName(argc, columnName, "telephoneNumber")], true);
        item->contactId_ = atoi(argv[Data::getIndexByName(argc, columnName, "contactId")]);
        item->contactInfoId_ = atoi(argv[Data::getIndexByName(argc, columnName, "contactInfoId")]);
        item->name_ = argv[Data::getIndexByName(argc, columnName, "name")];
	}
}