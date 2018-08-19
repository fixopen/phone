#include "SoundSegment.h"
#include "../../Util/StringOp.h"

namespace Util{
	std::wstring Contact_::Data::SoundSegment::tableName_ = L"soundSegment";
	int Contact_::Data::SoundSegment::count_ = 0;
	int Contact_::Data::SoundSegment::offset_ = 0;
}

namespace Contact_{
	namespace Data {
		std::wstring SoundSegment::folderName_;
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
			, played_(true)
			, isTip_(false) {
			id(0);
		}

		SoundSegment::~SoundSegment() {
			//do nothing
		}
		Util::shared_ptr<Contact> const SoundSegment::GetContact() {
			if (contactId_ != 0) {
				std::vector<Util::shared_ptr<Contact> > t = Contact::Select(L"id = " + Util::StringOp::FromInt(contactId_));
				if (t.size() != 0)
					contact_ = t[0];
			}
			return contact_;
		}

		std::vector<Util::shared_ptr<SoundSegment> > SoundSegment::Select(std::wstring const& filter, std::wstring const& orderFieldName, Util::BaseData::SeqenceRelation const dir, int const offset, int const pageSize) {
			return GetDatasByFilter(filter, modifyFieldByDB_, orderFieldName, dir, offset, pageSize);
		}

		void SoundSegment::Update() const {
			std::wstring cmd = L"UPDATE ";
			cmd += tableName_;
			cmd += L" SET [filename] = '";
			cmd += filename_;
			cmd += L"', duration = '";
			cmd += Util::StringOp::FromInt(duration_);
			cmd += L"', [type] = ";
			cmd += Util::StringOp::FromInt(type_);
			cmd += L", startTime = '";
			cmd += Util::StringOp::FromTimestamp(startTime_);
			cmd += L"', telephoneNumber = '";
			cmd += telephoneNumber_.number();
			cmd += L"', contactId = ";
			cmd += Util::StringOp::FromInt(contactId_);
			cmd += L", contactInfoId = ";
			cmd += Util::StringOp::FromInt(contactInfoId_);
			cmd += L", name = '";
			cmd += name_;
			cmd += L"', played = ";
			cmd += Util::StringOp::FromInt(played_);
			cmd += L", isTip = ";
			cmd += Util::StringOp::FromInt(isTip_);
			cmd += L" WHERE id = ";
			cmd += Util::StringOp::FromInt(id());
			ExecCommand(cmd);
		}

		void SoundSegment::Insert() {
			std::wstring cmd = L"INSERT INTO ";
			cmd += tableName_;
			cmd += L" ( filename, duration, type , startTime, telephoneNumber, contactId, contactInfoId, name, played, isTip) VALUES ( '";
			cmd += filename_;
			cmd += L"', '";
			cmd += Util::StringOp::FromInt(duration_);
			cmd += L"', ";
			cmd += Util::StringOp::FromInt(type_);
			cmd += L", '";
			cmd += Util::StringOp::FromTimestamp(startTime_);
			cmd += L"', '";
			cmd += telephoneNumber_.number();
			cmd += L"', ";
			cmd += Util::StringOp::FromInt(contactId_);
			cmd += L", ";
			cmd += Util::StringOp::FromInt(contactInfoId_);
			cmd += L", '";
			cmd += name_;
			cmd += L"', ";
			cmd += Util::StringOp::FromInt(played_);
			cmd += L", ";
			cmd += Util::StringOp::FromInt(isTip_);
			cmd += L")";
			ExecCommand(cmd);
			id(GetCurrentId());
		}

		void SoundSegment::Remove() const {
			SoundSegment::Remove(L"id = " + Util::StringOp::FromInt(id()));
		}

		void SoundSegment::Remove(std::wstring filter) {
			DataAccess<SoundSegment>::RemoveDatasByFilter(filter);
		}

		void SoundSegment::modifyFieldByDB_(int argc, char** argv, char** columnName, Util::shared_ptr<SoundSegment> item) {
			item->id(atoi(argv[Util::BaseData::GetIndexByName(argc, columnName, "id")]));
			item->filename_ = Util::StringOp::FromUTF8(argv[Util::BaseData::GetIndexByName(argc, columnName, "filename")]);
			item->duration_ = atoi(argv[Util::BaseData::GetIndexByName(argc, columnName, "duration")]);
			item->type_ = static_cast<SoundSegmentType>(atoi(argv[Util::BaseData::GetIndexByName(argc, columnName, "type")]));
			item->startTime_ = Util::StringOp::ToTimestamp(std::string(argv[Util::BaseData::GetIndexByName(argc, columnName, "startTime")]));
			item->telephoneNumber_ = TelephoneNumber(Util::StringOp::FromUTF8(argv[Util::BaseData::GetIndexByName(argc, columnName, "telephoneNumber")]), true);
			item->contactId_ = atoi(argv[Util::BaseData::GetIndexByName(argc, columnName, "contactId")]);
			item->contactInfoId_ = atoi(argv[Util::BaseData::GetIndexByName(argc, columnName, "contactInfoId")]);
			item->name_ = Util::StringOp::FromUTF8(argv[Util::BaseData::GetIndexByName(argc, columnName, "name")]);
			item->played_ = !!atoi(argv[Util::BaseData::GetIndexByName(argc, columnName, "played")]);
			item->isTip_ = !!atoi(argv[Util::BaseData::GetIndexByName(argc, columnName, "isTip")]);
		}
	}
}