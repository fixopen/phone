#include "SchedulerData.h"

namespace Data {
	std::string DataAccess<Scheduler>::tableName_ = "scheduler";
	sqlite3* DataAccess<Scheduler>::db_ = Data::GetDB();
    int DataAccess<Scheduler>::count_ = 0;
	int DataAccess<Scheduler>::offset_ = 0;
	Indication DataAccess<Scheduler>::indication_ = 0;

	Scheduler::Scheduler() {
	}

    std::vector<Util::shared_ptr<Scheduler> > Scheduler::GetFromDatabase(std::string const& filter, std::string const& orderFieldName, SeqenceRelation const dir, int const offset, int const pageSize) {
        return DataAccess<Scheduler>::GetDatasByFilter(filter, modifyFieldByDB_, orderFieldName, dir, offset, pageSize);
	}
	
	Util::shared_ptr<Data::Scheduler> Scheduler::GetFromDatabaseById(int id) {
		return DataAccess<Scheduler>::GetDataById(id, modifyFieldByDB_);
	}
	
	void Scheduler::Update() const {
		std::string cmd = "UPDATE ";
        cmd += tableName();
        cmd += " SET startTime = '";
        cmd += Util::StringOp::FromTimestamp(startTime_);
		cmd += "', [type] = ";
        cmd += Util::StringOp::FromInt(tipsType_);
		cmd += ", ring = '";
        cmd += ring_;
        cmd += "', what = '";
        cmd += what_;
        cmd += "' WHERE id = ";
        cmd += Util::StringOp::FromInt(id());
        ExecCommand(cmd);
	}

    void Scheduler::Insert() {
		std::string cmd = "INSERT INTO ";
        cmd += tableName();
        cmd += " ( startTime, type, ring, what ) VALUES ( '"; //classId,
        cmd += Util::StringOp::FromTimestamp(startTime_);
		cmd += "', ";
        cmd += Util::StringOp::FromInt(tipsType_);
		cmd += ", '";
        cmd += ring_;
        cmd += "', '";
        cmd += what_;
        cmd += "')";
        ExecCommand(cmd);
        id(GetCurrentId());
	}

	void Scheduler::Remove() {
		Scheduler::Remove("id = " + Util::StringOp::FromInt(id()));
	}

	void Scheduler::Remove(std::string filter) {
        DataAccess<Scheduler>::RemoveDatasByFilter(filter);
    }

	void Scheduler::modifyFieldByDB_(int argc, char** argv, char** columnName, Util::shared_ptr<Scheduler> item) {
        item->id(atoi(argv[Data::getIndexByName(argc, columnName, "id")]));
        item->startTime_ = Util::StringOp::ToTimestamp(std::string(argv[Data::getIndexByName(argc, columnName, "startTime")]));
		item->tipsType_ = (TipsType)atoi(argv[Data::getIndexByName(argc, columnName, "type")]);
		item->ring_ = argv[Data::getIndexByName(argc, columnName, "ring")];
		item->what_ = argv[Data::getIndexByName(argc, columnName, "what")];
	}       
}
