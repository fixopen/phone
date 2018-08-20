#ifndef __DATA_SCHEDULER_H__
#define __DATA_SCHEDULER_H__

#pragma warning(disable: 4786)
#include <string>
#pragma warning(disable: 4786)
#include <vector>

#include "Data.h"

#include "../Util/DateTime.h"
#include "../Util/SmartPtr.h"

namespace Data
{
	enum TipsType
	{
		ttAlarmSound,
		ttPopupForm,
		ttPopupFormAndCanDial,
		ttAlarmSoundAndPopupForm,
		ttAlarmSoundAndPopupFormAndCanDial,
		ttAutoDial,
		ttNote,
	};

	enum RepeatStyle
	{
		rsDay,
		rsDayButSaturdayAndSunday,
		rsWeek,
		rsMonth,
		rsYear,
	};

	class Scheduler : public DataAccess<Scheduler>
	{
	public:
		Util::Timestamp const startTime(void) const
        {
            return startTime_;
        }
        void startTime(Util::Timestamp const startTime)
        {
            startTime_ = startTime;
        }
		std::string const what() const
        {
            return what_;
        }
        void what(std::string const what)
        {
            what_ = what;
        }
      
	private:
		int id_;
		Util::Timestamp startTime_;
	//	Util::TimeSpan duration_;		//以后扩展
	//	std::string location_;			//以后扩展
		std::string what_;
	//	TipsType tipsType_;				//以后扩展
	//	RepeatStyle repeatStyle_;		//以后扩展
		/*
		//meta info
        Util::Timestamp creationTime_;
        Util::Timestamp lastReadTime_;
        Util::Timestamp lastWriteTime_;
        std::vector<Util::Timestamp> accessTimes_;
        Util::Timestamp destroyTime_;
		*/
	public:
		Scheduler(); //create
		~Scheduler(); //destroy
		static std::vector<boost::shared_ptr<Scheduler> > GetFromDatabase(std::string filter, Direction const dir = dNull, int const id = 0, int const pageSize = 0); //get from database
		static void modifyFieldByDB_(int argc, char** argv, char** columnName, boost::shared_ptr<Scheduler> item);
		void Update() const; //sync to database
		void Insert(); //insert new instance to database
		void Remove();
		static void Remove(std::string filter); //delete form database
	};
}

#endif //__DATA_SCHEDULER_H__
