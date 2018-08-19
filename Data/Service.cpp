#include "stdafx.h"
#include "Service.h"

namespace Util {
}

//namespace Contact {
    namespace Data {
		std::string DataAccess<Service>::tableName_ = "service";
		sqlite3* DataAccess<Service>::db_ = Data::GetDB();
		int DataAccess<Service>::rowCount_ = 0;
		int DataAccess<Service>::offset_ = 0;
		Indication DataAccess<Service>::indication_ = 0;

        Service::Service(void)
        : DataAccess<Service>() {
        }

        std::vector<boost::shared_ptr<Service> > Service::Select(std::string const& filter/*, std::wstring const& orderFieldName, SeqenceRelation const sr, int const offset, int const pageSize*/) {
            return GetDatasByFilter(filter, modifyFieldByDB_/*, orderFieldName, sr, offset, pageSize*/);
        }

		std::vector<boost::shared_ptr<Service> > Service::GetFromDatabaseByTypeOffsetLength(std::string type, int const offset, int const pageSize) {
			return DataAccess<Service>::GetDatasByFilterAndPageInfo(type, modifyFieldByDB_, offset, pageSize);
		}

        void Service::Update() const {
            std::string cmd = "UPDATE [";
            cmd += tableName();
            cmd += "] SET [name] = '";
            cmd += name_;
            cmd += "', [code] = '";
            cmd += code_;
            cmd += "', [welcomeInfo] = '";
            cmd += welcomeInfo_;
            cmd += "' WHERE [id] = ";
            cmd += Util::StringOp::FromInt(id());
            ExecCommand(cmd);
        }

        void Service::Insert() {
            std::string cmd = "INSERT INTO [";
            cmd += tableName();
            cmd += "] ( [name], [code], [welcomeInfo] ) VALUES ( '";
            cmd += name_;
            cmd += "', '";
            cmd += code_;
            cmd += "', '";
            cmd += welcomeInfo_;
            cmd += "' )";
            ExecCommand(cmd);
            id(GetCurrentId());
        }

        void Service::Remove() const {
            Remove("[id] = " + Util::StringOp::FromInt(id()));
        }

        void Service::Remove(std::string const& filter) {
            RemoveDatasByFilter(filter);
        }

        void Service::modifyFieldByDB_(int argc, char** argv, char** columnName, boost::shared_ptr<Service> item) {
            item->id(atoi(argv[getIndexByName(argc, columnName, "id")]));
            item->name_ = argv[getIndexByName(argc, columnName, "name")];
            item->code_ = argv[getIndexByName(argc, columnName, "code")];
            item->welcomeInfo_ = argv[getIndexByName(argc, columnName, "welcomeInfo")];
        }
    }
//}
