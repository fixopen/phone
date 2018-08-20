#ifndef __DATA_SOUNDSEGMENT_H__
#define __DATA_SOUNDSEGMENT_H__

#include <string>
#include <vector>

#include "../../Util/Data.h"
#include "../../Util/DateTime.h"
#include "../../Util/SmartPtr.h"
#include "TelephoneNumber.h"
#include "Contact.h"

namespace Data {
    enum SoundSegmentType {
		sstLeaveWord, //¡Ù—‘
		sstTeleRecord, //Õ®ª∞¬º“Ù
		sstLocalRecord, //±æµÿ¬º“Ù
		sstTips, //Ã· æ”Ô
		
		sstMusic, //“Ù¿÷
		sstRing, //’Ò¡Â“Ù

		sstMidi,
		sstMp3,
		sstAac,
	};

    class SoundSegment : public Util::DataAccess<SoundSegment> {
    public:
        std::string const filename() const
        {
            return filename_;
        }
        void filename(std::string const filename)
        {
            filename_ = filename;
        }

        int const duration() const
        {
            return duration_;
        }
		
		void duration(int const duration)
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
        TelephoneNumber const telephoneNumber() const
        {
            return telephoneNumber_;
        }
        void telephoneNumber(TelephoneNumber const telephoneNumber)
        {
            telephoneNumber_ = telephoneNumber;
        }
        Util::Timestamp const startTime() const
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
		std::string name() const
		{
			return name_;
		}
		void name(std::string const name)
		{
			name_ = name;
		}

		bool played() const
		{
			return played_;
		}
		
		void played(bool const played)
		{
			played_ = played;
		}

		bool isTip() const
		{
			return isTip_;
		}
		
		void isTip(bool const isTip)
		{
			isTip_ = isTip;
		}
		Util::shared_ptr<Contact> const GetContact();

    private:
        //int id_;
        std::string filename_; //255
		//Util::TimeSpan duration_;
		int duration_;
		SoundSegmentType type_;
		TelephoneNumber telephoneNumber_;
		Util::Timestamp startTime_;
        int contactInfoId_;
        int contactId_;
		std::string name_;
		bool played_;
		bool isTip_;
		Util::shared_ptr<Contact> contact_;

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
        static std::vector<Util::shared_ptr<SoundSegment> > GetFromDatabase(std::string const& filter, std::string const& orderFieldName = "id", SeqenceRelation const dir = srNull, int const offset = 0, int const pageSize = -1);
		void Update() const; //sync to database
		void Insert(); //insert new instance to database
        void Remove() const; //delete this from database
		static void Remove(std::string filter); //delete from database
	private:
		static void modifyFieldByDB_(int argc, char** argv, char** columnName, Util::shared_ptr<SoundSegment> item);
    };
}

#endif //__DATA_SOUNDSEGMENT_H__