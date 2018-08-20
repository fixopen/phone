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
	private:
		//int id_;
		Util::Timestamp startTime_;
		Util::TimeSpan duration_;
		std::string location_;
		std::string what_;
		TipsType tipsType_;
		RepeatStyle repeatStyle_;
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
		static std::vector<boost::shared_ptr<Scheduler> > GetFromDatabase(std::string filter); //get from database
		void Update() const; //sync to database
		void Insert(); //insert new instance to database
		static void Remove(std::string filter); //delete form database
	};
}

#endif //__DATA_SCHEDULER_H__
