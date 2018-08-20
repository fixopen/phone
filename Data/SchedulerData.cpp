#include "stdafx.h"
#include "SchedulerData.h"

namespace Data
{
	std::string DataAccess<Scheduler>::tableName_ = "scheduler";
	//sqlite3* DataAccess<Scheduler>::db_ = Environment::GetDB();
	//void (DataAccess<Scheduler>::*indication_)(DataManipulatorError& e) = 0;

	Scheduler::Scheduler()
	{
	}

	Scheduler::~Scheduler()
	{
	}

	std::vector<boost::shared_ptr<Scheduler> > Scheduler::GetFromDatabase(std::string filter)
	{
		std::vector<boost::shared_ptr<Scheduler> > result;
		return result;
	}

	void Scheduler::Update() const
	{
	}

	void Scheduler::Insert()
	{
	}

	void Scheduler::Remove(std::string filter)
	{
	}
}
