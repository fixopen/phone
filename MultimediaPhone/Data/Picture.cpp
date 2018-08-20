#include "Picture.h"

#include "../Util/StringOp.h"

namespace Data {
	std::string DataAccess<Picture>::tableName_ = "picture";
	sqlite3* DataAccess<Picture>::db_ = Data::GetDB();
    int DataAccess<Picture>::count_ = 0;
	int DataAccess<Picture>::offset_ = 0;
	Indication DataAccess<Picture>::indication_ = 0;

	std::string Picture::folderName_;

	Picture::Picture()
		: DataAccess<Picture>()
        //, filename_()
		//, filesize_()
		//, type_()
	{
	}

    std::vector<Util::shared_ptr<Picture> > Picture::GetFromDatabase(std::string const& filter, std::string const& orderFieldName, SeqenceRelation const dir, int const offset, int const pageSize) {
		return DataAccess<Picture>::GetDatasByFilter(filter, modifyFieldByDB_, orderFieldName, dir, offset, pageSize);
	}

	void Picture::Update() const {
        std::string cmd = "UPDATE ";
        cmd += tableName();
        cmd += " SET [filename] = '";
        cmd += filename_;
        cmd += "', [filesize] = ";
        cmd += Util::StringOp::FromInt(filesize_);
        cmd += ", [type] = ";
        cmd += Util::StringOp::FromInt(type_);
        cmd += " WHERE id = ";
        cmd += Util::StringOp::FromInt(id());
        ExecCommand(cmd);
	}

	void Picture::Insert() {
        std::string cmd = "INSERT INTO ";
        cmd += tableName();
        cmd += " ( filename, filesize, type, ) VALUES ( '";
        cmd += filename_;
        cmd += "', ";
        cmd += Util::StringOp::FromInt(filesize_);
        cmd += ", ";
        cmd += Util::StringOp::FromInt(type_);
        cmd += " )";
        ExecCommand(cmd);
		id(GetCurrentId());
	}

	void Picture::Remove(std::string filter) {
		DataAccess<Picture>::RemoveDatasByFilter(filter);
	}

	void Picture::modifyFieldByDB_(int argc, char** argv, char** columnName, Util::shared_ptr<Picture> item) {
        item->id(atoi(argv[Data::getIndexByName(argc, columnName, "id")]));
        item->filename_ = argv[Data::getIndexByName(argc, columnName, "value")];
        item->filesize_ = atoi(argv[Data::getIndexByName(argc, columnName, "value")]);
        item->type_ = static_cast<PictureType>(atoi(argv[Data::getIndexByName(argc, columnName, "value")]));
	}
}