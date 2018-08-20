#include "ContactInfo.h"

namespace Util {
	std::wstring DataAccess<Telephone::Data::ContactInfo>::tableName_ = L"contactInfo";
	int DataAccess<Telephone::Data::ContactInfo>::count_ = 0;
	int DataAccess<Telephone::Data::ContactInfo>::offset_ = 0;
	
	std::wstring DataAccess<Telephone::Data::ContactInfoSoundSegmentMap>::tableName_ = L"contactInfoSoundSegmentMap";
	int DataAccess<Telephone::Data::ContactInfoSoundSegmentMap>::count_ = 0;
	int DataAccess<Telephone::Data::ContactInfoSoundSegmentMap>::offset_ = 0;

	std::wstring DataAccess<Telephone::Data::ContactInfoPictureMap>::tableName_ = L"contactInfoPictureMap";
	int DataAccess<Telephone::Data::ContactInfoPictureMap>::count_ = 0;
	int DataAccess<Telephone::Data::ContactInfoPictureMap>::offset_ = 0;
}
namespace Telephone{
namespace Data {

	Util::shared_ptr<Contact_::Data::Contact> const ContactInfo::contact() {
        if (!contactFinded_) {
			std::vector<Util::shared_ptr<Contact_::Data::Contact> > t = Contact_::Data::Contact::Select(L"id = " + Util::StringOp::FromInt(contactId_));
            //assert(t.size() == 1);
            if (t.size() == 1) {
				contact_ = t[0];
            }
            contactFinded_ = true;
        }
        return contact_;
    }

    //void ContactInfo::contact(Util::shared_ptr<Contact> const contact)
    //{
    //    contact_ = contact;
    //}

    std::map<Util::Timestamp, Util::shared_ptr<Contact_::Data::SoundSegment> > ContactInfo::sounds() {
        if (!soundsFinded_) {
            std::vector<Util::shared_ptr<ContactInfoSoundSegmentMap> > t = ContactInfoSoundSegmentMap::Select(L"contactInfoId = " + Util::StringOp::FromInt(id()));
            std::wstring idList;
            for (int k = 0; k < t.size(); ++k) {
                idList += Util::StringOp::FromInt(t[k]->soundSegmentId());
            }
			std::vector<Util::shared_ptr<Contact_::Data::SoundSegment> > sounds = Contact_::Data::SoundSegment::Select(L"id IN ( " + idList + L" )");
            for (int i = 0; i < sounds.size(); ++i) {
                Util::Timestamp timestamp;
                for (int j = 0; j < t.size(); ++j) {
                    if (t[j]->soundSegmentId() == sounds[i]->id()) {
                        timestamp = t[j]->startTime();
                        break;
                    }
                }
                sounds_.insert(std::make_pair(timestamp, sounds[i]));
            }
            soundsFinded_ = true;
        }
        return sounds_;
    }

	/*
    std::vector<Util::shared_ptr<Picture> > ContactInfo::notes()
    {
        if (!notesFinded_)
        {
            std::vector<Util::shared_ptr<ContactInfoPictureMap> > t = ContactInfoPictureMap::GetFromDatabase("contactId = " + Util::StringOp::FromInt(id()));
            std::string idList;
            for (int i = 0; i < t.size(); ++i)
            {
                idList += Util::StringOp::FromInt(t[i]->pictureId());
            }
            notes_ = Picture::GetFromDatabase("id IN ( " + idList + " )");
            notesFinded_ = true;
        }
        return notes_;
    }
	*/

    /*
    Util::shared_ptr<Contact> const ContactInfo::GetContact() {
        //same as contact
    }

    int ContactInfo::GetSoundsCount() {
        //same as sounds().size();
    }
    */

    ContactInfo::ContactInfo()
    : DataAccess<ContactInfo>()
    //, direction_()
    , isSound_(false)
    //, startTime_()
    //, duration_()
    //, class_()
    //, classId_(0)
    //, telephoneNumber_()
    //, contact_(0)
    , contactId_(0)
    , contactFinded_(false)
    , soundsFinded_(false)
    //, notesFinded_(false)
	, played_(true)
    {
		//id(0);
    }

    std::vector<Util::shared_ptr<ContactInfo> > ContactInfo::Select(std::wstring const& filter, std::wstring const& orderFieldName, SeqenceRelation const dir, int const offset, int const pageSize) {
		return GetDatasByFilter(filter, modifyFieldByDB_, orderFieldName, dir, offset, pageSize);
    }

    void ContactInfo::Update() const {
        std::wstring cmd = L"UPDATE ";
        cmd += tableName_;
        cmd += L" SET [type] = ";
        cmd += Util::StringOp::FromInt(type_);
        cmd += L", isSound = ";
        cmd += Util::StringOp::FromInt(isSound_);
        cmd += L", startTime = '";
        cmd += Util::StringOp::FromTimestamp(startTime_);
        cmd += L"', duration = '";
        cmd += Util::StringOp::FromInt(duration_);
        //cmd += "', classId = ";
        //cmd += Util::StringOp::FromInt(classId_);
        cmd += L"', telephoneNumber = '";
        cmd += telephoneNumber_.number();
        cmd += L"', contactId = ";
        cmd += Util::StringOp::FromInt(contactId_);
		cmd += L", name = '";
		cmd += name_;
        cmd += L"', played = ";
		cmd += Util::StringOp::FromInt(played_);
		cmd += L" WHERE id = ";
        cmd += Util::StringOp::FromInt(id());
        ExecCommand(cmd);
    }

    void ContactInfo::Insert() {
        std::wstring cmd = L"INSERT INTO ";
        cmd += tableName_;
        cmd += L" ( type, isSound, startTime, duration, telephoneNumber, contactId, name, played ) VALUES ( "; //classId,
        cmd += Util::StringOp::FromInt(type_);
        cmd += L", ";
        cmd += Util::StringOp::FromInt(isSound_);
        cmd += L", '";
        cmd += Util::StringOp::FromTimestamp(startTime_);
        cmd += L"', '";
        cmd += Util::StringOp::FromInt(duration_);
        cmd += L"', '";
        //cmd += Util::StringOp::FromInt(classId_);
        //cmd += ", ";
        cmd += telephoneNumber_.number();// ToString();
        cmd += L"', ";
        cmd += Util::StringOp::FromInt(contactId_);
		cmd += L", '";
		cmd += name_;
		cmd += L"', ";
		cmd += Util::StringOp::FromInt(played_);
        cmd += L" )";
        ExecCommand(cmd);
        id(GetCurrentId());
    }

    void ContactInfo::Remove() const {
        Remove(L"id = " + Util::StringOp::FromInt(id()));
    }

    void ContactInfo::Remove(std::wstring const& filter) {
        RemoveDatasByFilter(filter);
    }

    void ContactInfo::modifyFieldByDB_(int argc, char** argv, char** columnName, Util::shared_ptr<ContactInfo> item) {
		item->id(atoi(argv[GetIndexByName(argc, columnName, "id")]));
        item->type_ = static_cast<ContatcInfoType>(atoi(argv[GetIndexByName(argc, columnName, "type")]));
        item->isSound_ = !!atoi(argv[GetIndexByName(argc, columnName, "isSound")]);
        item->startTime_ = Util::StringOp::ToTimestamp(std::string(argv[GetIndexByName(argc, columnName, "startTime")]));
        item->duration_ = atoi(argv[GetIndexByName(argc, columnName, "duration")]);
        //item->classId_ = atoi(argv[GetIndexByName(argc, columnName, "classId")]);
		item->telephoneNumber_ = Contact_::Data::TelephoneNumber(Util::StringOp::FromUTF8(argv[GetIndexByName(argc, columnName, "telephoneNumber")]), true);
        item->contactId_ = atoi(argv[GetIndexByName(argc, columnName, "contactId")]);
		item->name_ = Util::StringOp::FromUTF8(argv[GetIndexByName(argc, columnName, "name")]);
        item->played_ = !!atoi(argv[GetIndexByName(argc, columnName, "played")]);
    }

    ContactInfoSoundSegmentMap::ContactInfoSoundSegmentMap()
    : DataAccess<ContactInfoSoundSegmentMap>()
    //, contactInfoId_(0)
    //, soundSegmentId_(0)
    //, startTime_(0)
    {
    }

    std::vector<Util::shared_ptr<ContactInfoSoundSegmentMap> > ContactInfoSoundSegmentMap::Select(std::wstring const& filter, std::wstring const& orderFieldName, SeqenceRelation const dir, int const offset, int const pageSize) {
        return GetDatasByFilter(filter, modifyFieldByDB_, orderFieldName, dir, offset, pageSize);
    }

    void ContactInfoSoundSegmentMap::Update() const {
        std::wstring cmd = L"UPDATE ";
        cmd += tableName_;
        cmd += L" SET [soundSegmentId] = ";
        cmd += Util::StringOp::FromInt(soundSegmentId_);
        cmd += L", contactInfoId = ";
        cmd += Util::StringOp::FromInt(contactInfoId_);
        cmd += L", startTime = '";
        cmd += Util::StringOp::FromTimestamp(startTime_);
        cmd += L"' WHERE id = ";
        cmd += Util::StringOp::FromInt(id());
        ExecCommand(cmd);
    }

    void ContactInfoSoundSegmentMap::Insert() {
        std::wstring cmd = L"INSERT INTO ";
        cmd += tableName_;
        cmd += L" ( soundSegmentId, contactId, startTime ) VALUES ( ";
        cmd += Util::StringOp::FromInt(soundSegmentId_);
        cmd += L", ";
        cmd += Util::StringOp::FromInt(contactInfoId_);
        cmd += L", '";
        cmd += Util::StringOp::FromTimestamp(startTime_);
        cmd += L"' )";
        cmd += Util::StringOp::FromInt(id());
        ExecCommand(cmd);
        id(GetCurrentId());
    }

    void ContactInfoSoundSegmentMap::Remove() const {
        Remove(L"id = " + Util::StringOp::FromInt(id()));
    }

    void ContactInfoSoundSegmentMap::Remove(std::wstring filter) {
        RemoveDatasByFilter(filter);
    }

    void ContactInfoSoundSegmentMap::modifyFieldByDB_(int argc, char** argv, char** columnName, Util::shared_ptr<ContactInfoSoundSegmentMap> item) {
		item->id(atoi(argv[GetIndexByName(argc, columnName, "id")]));
        item->soundSegmentId_ = atoi(argv[GetIndexByName(argc, columnName, "soundSegmentId")]);
        item->contactInfoId_ = atoi(argv[GetIndexByName(argc, columnName, "contactInfoId")]);
		item->startTime_ = Util::StringOp::ToTimestamp(std::string(argv[GetIndexByName(argc, columnName, "startTime")]));
    }


    ContactInfoPictureMap::ContactInfoPictureMap()
    : DataAccess<ContactInfoPictureMap>()
    //, contactInfoId_(0)
    //, pictureId_(0)
    {
    }

    std::vector<Util::shared_ptr<ContactInfoPictureMap> > ContactInfoPictureMap::Select(std::wstring const& filter, std::wstring const& orderFieldName, SeqenceRelation const dir, int const offset, int const pageSize) {
        return GetDatasByFilter(filter, modifyFieldByDB_, orderFieldName, dir, offset, pageSize);
    }

    void ContactInfoPictureMap::Update() const {
        std::wstring cmd = L"UPDATE ";
        cmd += tableName_;
        cmd += L" SET [pictureId] = ";
        cmd += Util::StringOp::FromInt(pictureId_);
        cmd += L", contactInfoId = ";
        cmd += Util::StringOp::FromInt(contactInfoId_);
        cmd += L" WHERE id = ";
        cmd += Util::StringOp::FromInt(id());
        ExecCommand(cmd);
    }

    void ContactInfoPictureMap::Insert() {
        std::wstring cmd = L"INSERT INTO ";
        cmd += tableName_;
        cmd += L" ( pictureId, contactId ) VALUES ( ";
        cmd += Util::StringOp::FromInt(pictureId_);
        cmd += L", ";
        cmd += Util::StringOp::FromInt(contactInfoId_);
        cmd += L" )";
        cmd += Util::StringOp::FromInt(id());
        ExecCommand(cmd);
        id(GetCurrentId());
    }

    void ContactInfoPictureMap::Remove() const {
        Remove(L"id = " + Util::StringOp::FromInt(id()));
    }

    void ContactInfoPictureMap::Remove(std::wstring filter) {
        RemoveDatasByFilter(filter);
    }

    void ContactInfoPictureMap::modifyFieldByDB_(int argc, char** argv, char** columnName, Util::shared_ptr<ContactInfoPictureMap> item) {
		item->id(atoi(argv[GetIndexByName(argc, columnName, "id")]));
        item->pictureId_ = atoi(argv[GetIndexByName(argc, columnName, "soundSegmentId")]);
        item->contactInfoId_ = atoi(argv[GetIndexByName(argc, columnName, "contactId")]);
    }
}
}