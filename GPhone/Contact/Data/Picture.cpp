#include "Picture.h"

#include "../../Util/StringOp.h"
namespace Util{	
	std::wstring Contact::Data::Picture::tableName_ = L"picture";
	int Contact::Data::Picture::count_ = 0;
	int Contact::Data::Picture::offset_ = 0;
}
namespace Contact {
namespace Data {

	Picture::Picture()
		: DataAccess<Picture>()
        //, filename_()
		//, filesize_()
		//, type_()
	{
	}

    std::vector<Util::shared_ptr<Picture> > Picture::GetFromDatabase(std::wstring const& filter, std::wstring const& orderFieldName, SeqenceRelation const dir, int const offset, int const pageSize) {
		return DataAccess<Picture>::GetDatasByFilter(filter, modifyFieldByDB_, orderFieldName, dir, offset, pageSize);
	}

	void Picture::Update() const {
        std::wstring cmd = L"UPDATE ";
        cmd += tableName_;
        cmd += L" SET [filename] = '";
        cmd += filename_;
        cmd += L"', [filesize] = ";
        cmd += Util::StringOp::FromInt(filesize_);
        cmd += L", [type] = ";
        cmd += Util::StringOp::FromInt(type_);
        cmd += L" WHERE id = ";
        cmd += Util::StringOp::FromInt(id());
        ExecCommand(cmd);
	}

	void Picture::Insert() {
        std::wstring cmd = L"INSERT INTO ";
        cmd += tableName_;
        cmd += L" ( filename, filesize, type, ) VALUES ( '";
        cmd += filename_;
        cmd += L"', ";
        cmd += Util::StringOp::FromInt(filesize_);
        cmd += L", ";
        cmd += Util::StringOp::FromInt(type_);
        cmd += L" )";
        ExecCommand(cmd);
		id(GetCurrentId());
	}

	void Picture::Remove(std::wstring filter) {
		DataAccess<Picture>::RemoveDatasByFilter(filter);
	}

	void Picture::modifyFieldByDB_(int argc, char** argv, char** columnName, Util::shared_ptr<Picture>& item) {
		item->id(atoi(argv[Util::BaseData::GetIndexByName(argc, columnName, "id")]));
		item->filename_ = Util::StringOp::FromUTF8(argv[Util::BaseData::GetIndexByName(argc, columnName, "filename")]);
        item->filesize_ = atoi(argv[Util::BaseData::GetIndexByName(argc, columnName, "filesize")]);
        item->type_ = static_cast<PictureType>(atoi(argv[Util::BaseData::GetIndexByName(argc, columnName, "type")]));
	}
	}
}