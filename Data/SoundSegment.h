#ifndef __DATA_SOUNDSEGMENT_H__
#define __DATA_SOUNDSEGMENT_H__

#pragma warning(disable: 4786)
#include <string>
#pragma warning(disable: 4786)
#include <vector>

#include "Data.h"

#include "../Util/DateTime.h"
#include "../Util/SmartPtr.h"

namespace Data
{
	enum SoundSegmentType
	{
		sstRing, //’Ò¡Â“Ù
		sstTips, //Ã· æ”Ô

		sstMidi,
		sstMp3,
		sstAac,
		
		sstLeaveWord, //¡Ù—‘
		sstTeleRecord, //Õ®ª∞¬º“Ù
		sstLocalRecord, //±æµÿ¬º“Ù
		sstMusic, //“Ù¿÷
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
    private:
        //int id_;
        std::string filename_; //255
		Util::TimeSpan duration_;
		SoundSegmentType type_;
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
		static std::vector<boost::shared_ptr<SoundSegment> > GetFromDatabase(std::string filter); //get from database
		void Update() const; //sync to database
		void Insert(); //insert new instance to database
        void Remove() const; //delete this from database
		static void Remove(std::string filter); //delete from database
	private:
		static void modifyFieldByDB_(int argc, char** argv, char** columnName, boost::shared_ptr<SoundSegment> item);
    };
}

#endif //__DATA_SOUNDSEGMENT_H__