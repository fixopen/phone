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
		ttNoAlarm,
		ttAlarmSound,
		ttPopupForm,
		ttPopupFormAndCanDial,
		ttAlarmSoundAndPopupForm,
		ttAlarmSoundAndPopupFormAndCanDial,
		ttAutoDial,
		ttNote,
		ttRead,
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
		TipsType const tipsType(void) const
		{
			return tipsType_;
		}
		void tipsType(TipsType type)
		{
			tipsType_ = type;
		}
		std::string const ring() const
        {
            return ring_;
        }
        void ring(std::string const ring)
        {
            ring_ = ring;
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
		Util::Timestamp startTime_;		//����ʱ��
	//	Util::TimeSpan duration_;		//�Ժ���չ
	//	std::string location_;			//�Ժ���չ
		std::string what_;				//��������
		TipsType tipsType_;				//�Ƿ�����
		std::string ring_;				//��������
	//	RepeatStyle repeatStyle_;		//�Ժ���չ
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
		static std::vector<boost::shared_ptr<Scheduler> > GetFromDatabaseByOrder(std::string const filter, std::string order, Direction const dir = dUp);
		static boost::shared_ptr<Data::Scheduler> GetFromDatabaseById(int id);
		static void modifyFieldByDB_(int argc, char** argv, char** columnName, boost::shared_ptr<Scheduler> item);
		void Update() const; //sync to database
		void Insert(); //insert new instance to database
		void Remove();
		static void Remove(std::string filter); //delete form database
	};
}

#endif //__DATA_SCHEDULER_H__
