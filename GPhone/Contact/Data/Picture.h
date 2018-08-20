#ifndef __DATA_PICTURE_H__
#define __DATA_PICTURE_H__

#include <string>
#include <vector>

#include "../../Util/Data.h"
#include "../../Util/DateTime.h"
#include "../../Util/SmartPtr.h"
namespace Contact {
	namespace Data {
		enum PictureType {
			ptGif,
			ptPng,
			ptJpeg,

			ptPhoto, //照片
			ptNote, //通话记事
			ptDraw, //绘图创作
			ptIcon, //界面图符
		};

		class Picture : public Util::DataAccess<Picture> {
		public:
			std::wstring const filename() const
			{
				return filename_;
			}
			void filename(std::wstring const filename)
			{
				filename_ = filename;
			}
			size_t const filesize() const
			{
				return filesize_;
			}
			PictureType const type() const
			{
				return type_;
			}
			void type(PictureType const type)
			{
				type_ = type;
			}
			static std::wstring const folderName()
			{
				return Picture::folderName_;
			}
			static void folderName(std::wstring const folderName)
			{
				Picture::folderName_ = folderName;
			}
		private:
			//int id_;
			std::wstring filename_; //255
			size_t filesize_;
			PictureType type_;
			/*
			//meta info
			Util::Timestamp creationTime_;
			Util::Timestamp lastReadTime_;
			Util::Timestamp lastWriteTime_;
			std::vector<Util::Timestamp> accessTimes_;
			Util::Timestamp destroyTime_;
			*/
		private:
			static std::wstring folderName_;
		public:
			Picture(); //create
			//~Picture(); //destroy
			static std::vector<Util::shared_ptr<Picture> > GetFromDatabase(std::wstring const& filter, std::wstring const& orderFieldName = L"id", SeqenceRelation const dir = srNull, int const offset = 0, int const pageSize = -1);
			void Update() const; //sync to database
			void Insert(); //insert new instance to database
			static void Remove(std::wstring filter); //delete form database
		private:
			static void modifyFieldByDB_(int argc, char** argv, char** columnName, Util::shared_ptr<Picture>& item);
		};
	}
}

#endif //__DATA_PICTURE_H__