#ifndef __DATA_PICTURE_H__
#define __DATA_PICTURE_H__

#pragma warning(disable: 4786)
#include <string>
#pragma warning(disable: 4786)
#include <vector>

#include "Data.h"

#include "../Util/DateTime.h"
#include "../Util/SmartPtr.h"

namespace Data
{
	enum PictureType
	{
		ptGif,
		ptPng,
		ptJpeg,

		ptPhoto, //照片
		ptNote, //通话记事
		ptDraw, //绘图创作
		ptIcon, //界面图符
	};

	class Picture : public DataAccess<Picture>
	{
    public:
        std::string const filename() const
        {
            return filename_;
        }
        void filename(std::string const filename)
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
        static std::string const folderName()
        {
            return Picture::folderName_;
        }
        static void folderName(std::string const folderName)
        {
            Picture::folderName_ = folderName;
        }
	private:
		//int id_;
        std::string filename_; //255
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
		static std::string folderName_;
	public:
		Picture(); //create
		~Picture(); //destroy
		static std::vector<boost::shared_ptr<Picture> > GetFromDatabase(std::string filter, Direction const dir = dNull, int const id = 0, int const pageSize = 0); //get from database
		void Update() const; //sync to database
		void Insert(); //insert new instance to database
		static void Remove(std::string filter); //delete form database
	private:
		static void modifyFieldByDB_(int argc, char** argv, char** columnName, boost::shared_ptr<Picture> item);
	};
}

#endif //__DATA_PICTURE_H__