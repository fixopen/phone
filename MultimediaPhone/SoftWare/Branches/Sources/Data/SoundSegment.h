#ifndef __DATA_SOUNDSEGMENT_H__
#define __DATA_SOUNDSEGMENT_H__

#pragma warning(disable: 4786)
#include <string>
#pragma warning(disable: 4786)
#include <vector>

#include "Data.h"

#include "../Util/DateTime.h"
#include "../Util/SmartPtr.h"
#include "TelephoneNumber.h"
#include "Contact.h"

namespace Data
{
	enum SoundSegmentType
	{
		sstLeaveWord, //¡Ù—‘
		sstTeleRecord, //Õ®ª∞¬º“Ù
		sstLocalRecord, //±æµÿ¬º“Ù
		sstMusic, //“Ù¿÷
		
		sstRing, //’Ò¡Â“Ù
		sstTips, //Ã· æ”Ô

		sstMidi,
		sstMp3,
		sstAac,
	};

    class SoundSegment : public DataAccess<SoundSegment>
    {
    public:
        std::string const filename() const
        {
            return filename_;
        }
        void filename(std::string const filename)
        {
            filename_ = filename;
        }
        Util::TimeSpan const duration() const
        {
            return duration_;
        }
		
		void duration(Util::TimeSpan const duration)
		{
			duration_ = duration;
		}

        SoundSegmentType const type() const
        {
            return type_;
        }
        void type(SoundSegmentType const type)
        {
            type_ = type;
        }
        static std::string const folderName()
        {
            return SoundSegment::folderName_;
        }
        static void folderName(std::string const folderName)
        {
            SoundSegment::folderName_ = folderName;
        }
        TelephoneNumber const telephoneNumber(void) const
        {
            return telephoneNumber_;
        }
        void telephoneNumber(TelephoneNumber const telephoneNumber)
        {
            telephoneNumber_ = telephoneNumber;
        }
        Util::Timestamp const startTime(void) const
        {
            return startTime_;
        }
        void startTime(Util::Timestamp const startTime)
        {
            startTime_ = startTime;
        }
		void contactInfoId(int const contactInfoId)
		{
			contactInfoId_ = contactInfoId;
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

		boost::shared_ptr<Contact> const GetContact(void);

    private:
        //int id_;
        std::string filename_; //255
		Util::TimeSpan duration_;
		SoundSegmentType type_;
		TelephoneNumber telephoneNumber_;
		Util::Timestamp startTime_;
        int contactInfoId_;
        int contactId_;
		std::string name_;
		boost::shared_ptr<Contact> contact_;

		/*
		//meta info
        Util::Timestamp creationTime_;
        Util::Timestamp lastReadTime_;
        Util::Timestamp lastWriteTime_;
        std::vector<Util::Timestamp> accessTimes_;
        Util::Timestamp destroyTime_;
		*/
	private:
		static std::string folderName_;
	public:
        static SoundSegment DefaultRing;
        static SoundSegment VipRing;
        static SoundSegment BlacklistRing;
        static SoundSegment DefaultSMRing;
        static SoundSegment VipSMRing;
        static SoundSegment BlacklistSMRing;
        static SoundSegment DefaultTips;
        static SoundSegment VipTips;
        static SoundSegment BlacklistTips;
	public:
		SoundSegment(); //create
		~SoundSegment(); //destroy
		static std::vector<boost::shared_ptr<SoundSegment> > GetFromDatabase(std::string filter, Direction const dir = dNull, int const id = 0, int const pageSize = 0); //get from database
		void Update() const; //sync to database
		void Insert(); //insert new instance to database
        void Remove() const; //delete this from database
		static void Remove(std::string filter); //delete from database
	private:
		static void modifyFieldByDB_(int argc, char** argv, char** columnName, boost::shared_ptr<SoundSegment> item);
    };
}

#endif //__DATA_SOUNDSEGMENT_H__