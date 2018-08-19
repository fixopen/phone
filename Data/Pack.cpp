#include "stdafx.h"
#include "Pack.h"

namespace Util {
}

//namespace Contact {
    namespace Data {
		std::string DataAccess<Pack>::tableName_ = "pack";
		sqlite3* DataAccess<Pack>::db_ = Data::GetDB();
		int DataAccess<Pack>::rowCount_ = 0;
		int DataAccess<Pack>::offset_ = 0;
		Indication DataAccess<Pack>::indication_ = 0;

        Pack::Pack()
        : DataAccess<Pack>()
        , serviceId_(0)
        , isOrder_(false) {
        }

        std::vector<boost::shared_ptr<Pack> > Pack::GetFromDatabase(std::string const& filter/*, std::wstring const& orderFieldName, SeqenceRelation const sr, int const offset, int const pageSize*/) {
            return GetDatasByFilter(filter, modifyFieldByDB_/*, orderFieldName, sr, offset, pageSize*/);
        }

        void Pack::Update() const {
            std::string cmd = "UPDATE [";
            cmd += tableName();
            cmd += "] SET [serviceId] = ";
            cmd += Util::StringOp::FromInt(serviceId_);
            cmd += ", [type] = '";
            cmd += type_;
            cmd += "', [no] = '";
            cmd += no_;
            cmd += "', [rate] = '";
            cmd += rate_;
            cmd += "' [isOrder] = ";
            cmd += Util::StringOp::FromInt(isOrder_);
            cmd += " WHERE [id] = ";
            cmd += Util::StringOp::FromInt(id());
            ExecCommand(cmd);
        }

        void Pack::Insert() {
            std::string cmd = "INSERT INTO [";
            cmd += tableName();
            cmd += "] ( [serviceId], [type], [no], [rate], [isOrder] ) VALUES ( ";
            cmd += Util::StringOp::FromInt(serviceId_);
            cmd += ", '";
            cmd += type_;
            cmd += "', '";
            cmd += no_;
            cmd += "', '";
            cmd += rate_;
            cmd += "', ";
            cmd += Util::StringOp::FromInt(isOrder_);
            cmd += " )";
            ExecCommand(cmd);
            id(GetCurrentId());
        }

        void Pack::Remove() const {
            Remove("[id] = " + Util::StringOp::FromInt(id()));
        }

        void Pack::Remove(std::string const& filter) {
            RemoveDatasByFilter(filter);
        }

        void Pack::modifyFieldByDB_(int argc, char** argv, char** columnName, boost::shared_ptr<Pack> item) {
            item->id(atoi(argv[getIndexByName(argc, columnName, "id")]));
            item->serviceId_ = Util::StringOp::ToInt(argv[getIndexByName(argc, columnName, "serviceId")]);
            item->type_ = argv[getIndexByName(argc, columnName, "type")];
            item->no_ = argv[getIndexByName(argc, columnName, "no")];
            item->rate_ = argv[getIndexByName(argc, columnName, "rate")];
            item->isOrder_ = !!Util::StringOp::ToInt(argv[getIndexByName(argc, columnName, "isOrder")]);
        }
    }
//}
