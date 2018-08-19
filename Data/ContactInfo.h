#ifndef __DATA_CONTACTINFO_H__
#define __DATA_CONTACTINFO_H__

#pragma warning(disable: 4786)
#include <string>
#include <vector>
#include  <map>

#include "Data.h"
#include "SoundSegment.h"
#include "Picture.h"
#include "TelephoneNumber.h"
#include "Contact.h"

#include "../Util/DateTime.h"
#include "../Util/SmartPtr.h"

namespace Data
{
	enum ContatcInfoType
	{
		citInNo,
		citInOk,
		citOut
	};

    class Contact;
	class ContactInfo : public DataAccess<ContactInfo>
	{
    public:
        ContatcInfoType const type(void) const
        {
            return type_;
        }
        void type(ContatcInfoType const type)
        {
            type_ = type;
        }
        bool const isSound(void) const
        {
            return isSound_;
        }
        void isSound(bool const isSound)
        {
            isSound_ = isSound;
        }
        Util::Timestamp const startTime(void) const
        {
            return startTime_;
        }
        void startTime(Util::Timestamp const startTime)
        {
            startTime_ = startTime;
        }
        int const duration(void) const
        {
            return duration_;
        }
        void duration(int const duration)
        {
            duration_ = duration;
        }
        TelephoneNumber const telephoneNumber(void) const
        {
            return telephoneNumber_;
        }
        void telephoneNumber(TelephoneNumber const telephoneNumber)
        {
            telephoneNumber_ = telephoneNumber;
        }
		int const contactId(void) const
		{
			return contactId_;
		}
		void contactId(int const contactId)
		{
			contactId_ = contactId;
		}
		std::string name(void) const
		{
			return name_;
		}
		void name(std::string const name)
		{
			name_ = name;
		}

		bool played(void) const
		{
			return played_;
		}
		
		void played(bool const played)
		{
			played_ = played;
		}
		
        boost::shared_ptr<Contact> const contact(void);
        //void contact(boost::shared_ptr<Contact> const contact);
        std::map<Util::Timestamp, boost::shared_ptr<SoundSegment> > sounds();
        //std::vector<boost::shared_ptr<Picture> > notes();

		boost::shared_ptr<Contact> const GetContact(void);
		int GetSoundsCount(void);

	private:
		//int id_;
		ContatcInfoType type_;
		bool isSound_;
		Util::Timestamp startTime_;
		//Util::TimeSpan duration_;
		int duration_;
		//class
		//ContactInfoClass* class_;
        //int classId_;
        //contact
		TelephoneNumber telephoneNumber_;
        bool contactFinded_;
        boost::shared_ptr<Contact> contact_;
        int contactId_;
		std::string name_;
		bool played_;
		//sounds
        bool soundsFinded_;
		std::map<Util::Timestamp, boost::shared_ptr<SoundSegment> > sounds_;
		//notes
        //bool notesFinded_;
		//std::vector<boost::shared_ptr<Picture> > notes_;
		/*
		//meta info
        Util::Timestamp creationTime_;
        Util::Timestamp lastReadTime_;
        Util::Timestamp lastWriteTime_;
        std::vector<Util::Timestamp> accessTimes_;
        Util::Timestamp destroyTime_;
		*/
	public:
		ContactInfo(); //create
		~ContactInfo(); //destroy
		static std::vector<boost::shared_ptr<ContactInfo> > GetFromDatabase(std::string filter, Direction const dir = dNull, int const id = 0, int const pageSize = 0); //get from database
		static std::vector<boost::shared_ptr<ContactInfo> > GetFromDatabaseByTypeOffsetLength(std::string type, int const offset, int const pageSize);
		static std::vector<int> GetIdByFilter(std::string const filter, Direction const dir = dNull, std::vector<int>& itemId = std::vector<int>(), int const pageSize = 0);
		static boost::shared_ptr<ContactInfo> GetDataById(int id);//add by qi 20100518

		void Update() const; //sync to database
		void Insert(); //insert new instance to database
        void Remove() const; //delete self from database
		static void Remove(std::string filter); //delete from database
	private:
		static void modifyFieldByDB_(int argc, char** argv, char** columnName, boost::shared_ptr<ContactInfo> item);
		static std::vector<int> modifyFieldByDb_(int argc, char** argv, char** columnName, int itemId);
	};

    class ContactInfoSoundSegmentMap : public DataAccess<ContactInfoSoundSegmentMap>
    {
    public:
        int contactInfoId() const
        {
            return contactInfoId_;
        }
        void contactInfoId(int contactInfoId)
        {
            contactInfoId_ = contactInfoId;
        }
        int soundSegmentId() const
        {
            return soundSegmentId_;
        }
        void soundSegmentId(int soundSegmentId)
        {
            soundSegmentId_ = soundSegmentId;
        }
        Util::Timestamp const startTime() const
        {
            return startTime_;
        }
        void startTime(Util::Timestamp const startTime)
        {
            startTime_ = startTime;
        }
    private:
        //int id_;
        int contactInfoId_;
        int soundSegmentId_;
        Util::Timestamp startTime_;
        /*
        //meta info
        Util::Timestamp creationTime_;
        Util::Timestamp lastReadTime_;
        Util::Timestamp lastWriteTime_;
        std::vector<Util::Timestamp> accessTimes_;
        Util::Timestamp destroyTime_;
        */
    public:
        ContactInfoSoundSegmentMap(); //create
        ~ContactInfoSoundSegmentMap(); //destroy
        static std::vector<boost::shared_ptr<ContactInfoSoundSegmentMap> > GetFromDatabase(std::string filter, Direction const dir = dNull, int const id = 0, int const pageSize = 0); //get from database
        void Update() const; //sync to database
        void Insert(); //insert new instance to database
        void Remove() const; //delete self from database
        static void Remove(std::string filter); //delete from database
    private:
        static void modifyFieldByDB_(int argc, char** argv, char** columnName, boost::shared_ptr<ContactInfoSoundSegmentMap> item);
    };

    class ContactInfoPictureMap : public DataAccess<ContactInfoPictureMap>
    {
    public:
        int contactInfoId() const
        {
            return contactInfoId_;
        }
        void contactInfoId(int contactInfoId)
        {
            contactInfoId_ = contactInfoId;
        }
        int pictureId() const
        {
            return pictureId_;
        }
        void pictureId(int pictureId)
        {
            pictureId_ = pictureId;
        }
    private:
        //int id_;
        int contactInfoId_;
        int pictureId_;
        /*
        //meta info
        Util::Timestamp creationTime_;
        Util::Timestamp lastReadTime_;
        Util::Timestamp lastWriteTime_;
        std::vector<Util::Timestamp> accessTimes_;
        Util::Timestamp destroyTime_;
        */
    public:
        ContactInfoPictureMap(); //create
        ~ContactInfoPictureMap(); //destroy
        static std::vector<boost::shared_ptr<ContactInfoPictureMap> > GetFromDatabase(std::string filter, Direction const dir = dNull, int const id = 0, int const pageSize = 0); //get from database
        void Update() const; //sync to database
        void Insert(); //insert new instance to database
        void Remove() const; //delete self from database
        static void Remove(std::string filter); //delete from database
    private:
        static void modifyFieldByDB_(int argc, char** argv, char** columnName, boost::shared_ptr<ContactInfoPictureMap> item);
    };
}

#endif //__DATA_CONTACTINFO_H__
