#include "stdafx.h"
#include "ContactInfo.h"
#include "Contact.h"

namespace Data
{
    std::string DataAccess<ContactInfo>::tableName_ = "contactInfo";
    sqlite3* DataAccess<ContactInfo>::db_ = Data::GetDB();
	Indication DataAccess<ContactInfo>::indication_ = 0;

    boost::shared_ptr<Contact> const ContactInfo::contact(void)
    {
        if (!contactFinded_)
        {
            std::vector<boost::shared_ptr<Contact> > t = Contact::GetFromDatabase("id = " + Util::StringOp::FromInt(contactId_));
            //assert(t.size() == 1);
			if (t.size() != 0)
				contact_ = t[0];
            contactFinded_ = true;
        }
        return contact_;
    }
    //void ContactInfo::contact(boost::shared_ptr<Contact> const contact)
    //{
    //    contact_ = contact;
    //}

    std::map<Util::Timestamp, boost::shared_ptr<SoundSegment> > ContactInfo::sounds()
    {
        if (!soundsFinded_)
        {
            std::vector<boost::shared_ptr<ContactInfoSoundSegmentMap> > t = ContactInfoSoundSegmentMap::GetFromDatabase("contactInfoId = " + Util::StringOp::FromInt(id()));
            std::string idList;
            for (int k = 0; k < t.size(); ++k)
            {
                idList += Util::StringOp::FromInt(t[k]->soundSegmentId());
            }
            std::vector<boost::shared_ptr<SoundSegment> > sounds = SoundSegment::GetFromDatabase("id IN ( " + idList + " )");
            for (int i = 0; i < sounds.size(); ++i)
            {
                Util::Timestamp timestamp;
                for (int j = 0; j < t.size(); ++j)
                {
                    if (t[j]->soundSegmentId() == sounds[i]->id())
                    {
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

    std::vector<boost::shared_ptr<Picture> > ContactInfo::notes()
    {
        if (!notesFinded_)
        {
            std::vector<boost::shared_ptr<ContactInfoPictureMap> > t = ContactInfoPictureMap::GetFromDatabase("contactId = " + Util::StringOp::FromInt(id()));
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

    boost::shared_ptr<SoundSegment> ContactInfo::StartRecord()
    {
        boost::shared_ptr<SoundSegment> result(new SoundSegment());
        ms_.push_back(boost::shared_ptr<ContactInfoSoundSegmentMap>(new ContactInfoSoundSegmentMap()));
        Util::Timestamp now = Util::Timestamp::GetCurrentTime();
        ms_[ms_.size() - 1]->startTime(now);
        sounds_[now] = result;
        return result;
    }
    void ContactInfo::StopRecord(boost::shared_ptr<SoundSegment> soundSegment)
    {
        soundSegment->Insert();
        m_[ms_.size() - 1]->soundSegmentId(soundSegment->id());
    }

    ContactInfo::ContactInfo()
    : DataAccess<ContactInfo>()
    //, direction_()
    //, isConnect_(true)
    //, startTime_()
    //, duration_()
    //, class_()
    //, classId_(0)
    //, telephoneNumber_()
    //, contact_(0)
    //, contactId_(0)
    , contactFinded_(false)
    , soundsFinded_(false)
    , notesFinded_(false)
    {
    }

    ContactInfo::~ContactInfo()
    {
        //do nothing
    }

    std::vector<boost::shared_ptr<ContactInfo> > ContactInfo::GetFromDatabase(std::string filter)
    {
        return DataAccess<ContactInfo>::GetDatasByFilter(filter, modifyFieldByDB_);
    }

    void ContactInfo::Update() const
    {
        std::string cmd = "UPDATE ";
        cmd += tableName();
        cmd += " SET [direction] = ";
        cmd += Util::StringOp::FromInt(direction_);
        cmd += ", isConnect = ";
        cmd += Util::StringOp::FromInt(isConnect_);
        cmd += ", startTime = '";
        cmd += Util::StringOp::FromTimestamp(startTime_);
        cmd += "', duration = '";
        cmd += Util::StringOp::FromTimeSpan(duration_);
        //cmd += "', classId = ";
        //cmd += Util::StringOp::FromInt(classId_);
        cmd += "', telephoneNumber = ";
        cmd += telephoneNumber_.ToString();
        cmd += ", contactId = ";
        cmd += Util::StringOp::FromInt(contactId_);
        cmd += " WHERE id = ";
        cmd += Util::StringOp::FromInt(id());
        ExecCommand(cmd);
    }

    void ContactInfo::Insert()
    {
        std::string cmd = "INSERT INTO ";
        cmd += tableName();
        cmd += " ( direction, isConnect, startTime, duration, telephoneNumber, contactId ) VALUES ( "; //classId,
        cmd += Util::StringOp::FromInt(direction_);
        cmd += ", ";
        cmd += Util::StringOp::FromInt(isConnect_);
        cmd += ", '";
        cmd += Util::StringOp::FromTimestamp(startTime_);
        cmd += "', '";
        cmd += Util::StringOp::FromTimeSpan(duration_);
        cmd += "', '";
        //cmd += Util::StringOp::FromInt(classId_);
        //cmd += ", ";
        cmd += telephoneNumber_.number();// ToString();
        cmd += "', ";
        cmd += Util::StringOp::FromInt(contactId_);
        cmd += " )";
        ExecCommand(cmd);
        id(GetCurrentId());
        for (int i = 0; i < ms_.size(); ++i)
        {
            ms_[i]->contactInfoId(id());
            ms_[i]->Insert();
        }
        soundsFinded_ = true;
    }

    void ContactInfo::Remove() const
    {
        ContactInfo::Remove("id = " + Util::StringOp::FromInt(id()));
    }

    void ContactInfo::Remove(std::string filter)
    {
        DataAccess<ContactInfo>::RemoveDatasByFilter(filter);
    }

    void ContactInfo::modifyFieldByDB_(int argc, char** argv, char** columnName, boost::shared_ptr<ContactInfo> item)
    {
        item->id(atoi(argv[Data::getIndexByName(argc, columnName, "id")]));
        item->direction_ = static_cast<Direction>(atoi(argv[Data::getIndexByName(argc, columnName, "direction")]));
        item->isConnect_ = !!atoi(argv[Data::getIndexByName(argc, columnName, "isConnect")]);
        item->startTime_ = Util::StringOp::ToTimestamp(std::string(argv[Data::getIndexByName(argc, columnName, "startTime")]));
        item->duration_ = Util::StringOp::ToTimeSpan(std::string(argv[Data::getIndexByName(argc, columnName, "duration")]));
        //item->classId_ = atoi(argv[Data::getIndexByName(argc, columnName, "classId")]);
        item->telephoneNumber_ = TelephoneNumber(argv[Data::getIndexByName(argc, columnName, "telephoneNumber")], true);
        item->contactId_ = atoi(argv[Data::getIndexByName(argc, columnName, "contactId")]);
    }

    std::string DataAccess<ContactInfoSoundSegmentMap>::tableName_ = "contactInfoSoundSegmentMap";
    sqlite3* DataAccess<ContactInfoSoundSegmentMap>::db_ = Data::GetDB();
	Indication DataAccess<ContactInfoSoundSegmentMap>::indication_ = 0;

    ContactInfoSoundSegmentMap::ContactInfoSoundSegmentMap()
    : DataAccess<ContactInfoSoundSegmentMap>()
    //, contactInfoId_(0)
    //, soundSegmentId_(0)
    //, startTime_(0)
    {
    }

    ContactInfoSoundSegmentMap::~ContactInfoSoundSegmentMap()
    {
    }

    std::vector<boost::shared_ptr<ContactInfoSoundSegmentMap> > ContactInfoSoundSegmentMap::GetFromDatabase(std::string filter)
    {
        return DataAccess<ContactInfoSoundSegmentMap>::GetDatasByFilter(filter, modifyFieldByDB_);
    }

    void ContactInfoSoundSegmentMap::Update() const
    {
        std::string cmd = "UPDATE ";
        cmd += tableName();
        cmd += " SET [soundSegmentId] = ";
        cmd += Util::StringOp::FromInt(soundSegmentId_);
        cmd += ", contactInfoId = ";
        cmd += Util::StringOp::FromInt(contactInfoId_);
        cmd += ", startTime = '";
        cmd += Util::StringOp::FromTimestamp(startTime_);
        cmd += "' WHERE id = ";
        cmd += Util::StringOp::FromInt(id());
        ExecCommand(cmd);
    }

    void ContactInfoSoundSegmentMap::Insert()
    {
        std::string cmd = "INSERT INTO ";
        cmd += tableName();
        cmd += " ( soundSegmentId, contactId, startTime ) VALUES ( ";
        cmd += Util::StringOp::FromInt(soundSegmentId_);
        cmd += ", ";
        cmd += Util::StringOp::FromInt(contactInfoId_);
        cmd += ", '";
        cmd += Util::StringOp::FromTimestamp(startTime_);
        cmd += "' )";
        cmd += Util::StringOp::FromInt(id());
        ExecCommand(cmd);
        id(GetCurrentId());
    }

    void ContactInfoSoundSegmentMap::Remove() const
    {
        ContactInfoSoundSegmentMap::Remove("id = " + Util::StringOp::FromInt(id()));
    }

    void ContactInfoSoundSegmentMap::Remove(std::string filter)
    {
        DataAccess<ContactInfoSoundSegmentMap>::RemoveDatasByFilter(filter);
    }

    void ContactInfoSoundSegmentMap::modifyFieldByDB_(int argc, char** argv, char** columnName, boost::shared_ptr<ContactInfoSoundSegmentMap> item)
    {
        item->id(atoi(argv[Data::getIndexByName(argc, columnName, "id")]));
        item->soundSegmentId_ = atoi(argv[Data::getIndexByName(argc, columnName, "soundSegmentId")]);
        item->contactInfoId_ = atoi(argv[Data::getIndexByName(argc, columnName, "contactInfoId")]);
        item->startTime_ = Util::StringOp::ToTimestamp(std::string(argv[Data::getIndexByName(argc, columnName, "startTime")]));
    }

    std::string DataAccess<ContactInfoPictureMap>::tableName_ = "contactInfoPictureMap";
    sqlite3* DataAccess<ContactInfoPictureMap>::db_ = Data::GetDB();
	Indication DataAccess<ContactInfoPictureMap>::indication_ = 0;

    ContactInfoPictureMap::ContactInfoPictureMap()
    : DataAccess<ContactInfoPictureMap>()
    //, contactInfoId_(0)
    //, pictureId_(0)
    {
    }

    ContactInfoPictureMap::~ContactInfoPictureMap()
    {
    }

    std::vector<boost::shared_ptr<ContactInfoPictureMap> > ContactInfoPictureMap::GetFromDatabase(std::string filter)
    {
        return DataAccess<ContactInfoPictureMap>::GetDatasByFilter(filter, modifyFieldByDB_);
    }

    void ContactInfoPictureMap::Update() const
    {
        std::string cmd = "UPDATE ";
        cmd += tableName();
        cmd += " SET [pictureId] = ";
        cmd += Util::StringOp::FromInt(pictureId_);
        cmd += ", contactInfoId = ";
        cmd += Util::StringOp::FromInt(contactInfoId_);
        cmd += " WHERE id = ";
        cmd += Util::StringOp::FromInt(id());
        ExecCommand(cmd);
    }

    void ContactInfoPictureMap::Insert()
    {
        std::string cmd = "INSERT INTO ";
        cmd += tableName();
        cmd += " ( pictureId, contactId ) VALUES ( ";
        cmd += Util::StringOp::FromInt(pictureId_);
        cmd += ", ";
        cmd += Util::StringOp::FromInt(contactInfoId_);
        cmd += " )";
        cmd += Util::StringOp::FromInt(id());
        ExecCommand(cmd);
        id(GetCurrentId());
    }

    void ContactInfoPictureMap::Remove() const
    {
        ContactInfoPictureMap::Remove("id = " + Util::StringOp::FromInt(id()));
    }

    void ContactInfoPictureMap::Remove(std::string filter)
    {
        DataAccess<ContactInfoPictureMap>::RemoveDatasByFilter(filter);
    }

    void ContactInfoPictureMap::modifyFieldByDB_(int argc, char** argv, char** columnName, boost::shared_ptr<ContactInfoPictureMap> item)
    {
        item->id(atoi(argv[Data::getIndexByName(argc, columnName, "id")]));
        item->pictureId_ = atoi(argv[Data::getIndexByName(argc, columnName, "soundSegmentId")]);
        item->contactInfoId_ = atoi(argv[Data::getIndexByName(argc, columnName, "contactId")]);
    }
}
