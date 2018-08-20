#ifndef __DATA_USER_H__
#define __DATA_USER_H__

#include "Contact.h"
#include "Setting.h"

#pragma warning(disable: 4786)
#include <string>

namespace Data
{
	class User : public DataAccess<User> //: public Contact
	{
    public:
        boost::shared_ptr<Contact> baseInfo()
        {
            if (!baseInfo_)
            {
                std::vector<boost::shared_ptr<Contact> > t = Contact::GetFromDatabase("id = " + Util::StringOp::FromInt(baseInfoId_));
                //assert(t.size() == 1);
                baseInfo_ = t[0];
            }
            return baseInfo_;
        }
        std::string const password() const
        {
            return password_;
        }
        void password(std::string const password)
        {
            password_ = password;
        }
        boost::shared_ptr<Setting> setting()
        {
            if (!setting_)
            {
                std::vector<boost::shared_ptr<Setting> > t = Setting::GetFromDatabase("id = " + Util::StringOp::FromInt(settingId_));
                //assert(t.size() == 1);
                setting_ = t[0];
            }
            return setting_;
        }
        static User currentUser()
        {
            return User::currentUser_;
        }
	private:
		//int id_;
		boost::shared_ptr<Contact> baseInfo_;
		int baseInfoId_;
		std::string password_;
		boost::shared_ptr<Setting> setting_;
		int settingId_;
		/*
		//meta info
		Util::Timestamp creationTime_;
        Util::Timestamp lastReadTime_;
        Util::Timestamp lastWriteTime_;
        std::vector<Util::Timestamp> accessTimes_;
        Util::Timestamp destroyTime_;
		*/
	private:
		static User currentUser_;
	public:
		User(); //create
		~User(); //destroy
		static std::vector<boost::shared_ptr<User> > GetFromDatabase(std::string filter, Direction const dir = dNull, int const id = 0, int const pageSize = 0); //get from database
		void Update() const; //sync to database
		void Insert(); //insert new instance to database
        void Remove() const; //remove self from database
		static void Remove(std::string filter); //delete from database
	private:
		static void modifyFieldByDB_(int argc, char** argv, char** columnName, boost::shared_ptr<User> item);
	};
}

#endif //__DATA_USER_H__
