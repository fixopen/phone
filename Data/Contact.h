#ifndef __DATA_CONTACT_H__
#define __DATA_CONTACT_H__

#pragma warning(disable: 4786)
#include <string>
#include <vector>

#include "Data.h"
#include "TelephoneNumber.h"
//#include "SoundSegment.h"
//#include "Picture.h"

//#include "../Util/DateTime.h"
#include "../Util/SmartPtr.h"

namespace Data
{
	class ContactInfo;
	class ContactGroup;

	enum ContactType
	{
		ctNormal,
		ctBlacklist,
		ctVip,
	};

	class Contact : public DataAccess<Contact>
	{
    public:
        boost::shared_ptr<ContactGroup> group();
        void group(boost::shared_ptr<ContactGroup> group)
        {
            group_ = group;
        }
        void groupId(int groupId)
        {
            groupId_ = groupId;
        }
		int const groupId() const
		{
			return groupId_;
		}
        std::string const name() const
        {
            return name_;
        }
        void name(std::string const name)
        {
            name_ = name;
        }
        std::string const alias() const
        {
            return alias_;
        }
        void alias(std::string const alias)
        {
            alias_ = alias;
        }
        ContactType const type() const
        {
            return type_;
        }
        void type(ContactType const type)
        {
            type_ = type;
        }
        TelephoneNumber const mobilesTelephone() const
        {
            return mobilesTelephone_[0];
        }
        void mobilesTelephone(TelephoneNumber const mobilesTelephone)
        {
            mobilesTelephone_[0] = mobilesTelephone;
        }
        TelephoneNumber const mobilesTelephone2() const
        {
            return mobilesTelephone_[1];
        }
        void mobilesTelephone2(TelephoneNumber const mobilesTelephone2)
        {
            mobilesTelephone_[1] = mobilesTelephone2;
        }
        TelephoneNumber const worksTelephone() const
        {
            return worksTelephone_[0];
        }
        void worksTelephone(TelephoneNumber const worksTelephone)
        {
            worksTelephone_[0] = worksTelephone;
        }
        TelephoneNumber const worksTelephone2() const
        {
            return worksTelephone_[1];
        }
        void worksTelephone2(TelephoneNumber const worksTelephone2)
        {
            worksTelephone_[1] = worksTelephone2;
        }
        TelephoneNumber const internalTelephone() const
        {
            return internalTelephone_;
        }
        void internalTelephone(TelephoneNumber const internalTelephone)
        {
            internalTelephone_ = internalTelephone;
        }
        TelephoneNumber const fax() const
        {
            return fax_;
        }
        void fax(TelephoneNumber const fax)
        {
            fax_ = fax;
        }
        TelephoneNumber const familyTelephone() const
        {
            return familyTelephone;
        }
        void familyTelephone(TelephoneNumber const familyTelephone)
        {
            familyTelephone_ = familyTelephone;
        }
        std::string const msn() const
        {
            return msn_;
        }
        void msn(std::string const msn)
        {
            msn_ = msn;
        }
        std::string const qq() const
        {
            return qq_;
        }
        void qq(std::string const qq)
        {
            qq_ = qq;
        }
        std::string const skype() const
        {
            return skype_;
        }
        void skype(std::string const skype)
        {
            skype_ = skype;
        }
        std::string const email() const
        {
            return email_;
        }
        void email(std::string const email)
        {
            email_ = email;
        }
        std::string const homesite() const
        {
            return homesite_;
        }
        void homesite(std::string const homesite)
        {
            homesite_ = homesite;
        }
        std::string const company() const
        {
            return company_;
        }
        void company(std::string const company)
        {
            company_ = company;
        }
        std::string const department() const
        {
            return department_;
        }
        void department(std::string const department)
        {
            department_ = department;
        }
        std::string const duty() const
        {
            return duty_;
        }
        void duty(std::string const duty)
        {
            duty_ = duty;
        }
        std::string const companyAddress() const
        {
            return companyAddress_;
        }
        void companyAddress(std::string const companyAddress)
        {
            companyAddress_ = companyAddress;
        }
        std::string const companyPostCode() const
        {
            return companyPostCode_;
        }
        void companyPostCode(std::string const companyPostCode)
        {
            companyPostCode_ = companyPostCode;
        }
        std::string const familyAddress() const
        {
            return familyAddress_;
        }
        void familyAddress(std::string const familyAddress)
        {
            familyAddress_ = familyAddress;
        }
        std::string const postCode() const
        {
            return postCode_;
        }
        void postCode(std::string const postCode)
        {
            postCode_ = postCode;
        }
        std::string const memo() const
        {
            return memo_;
        }
        void memo(std::string const memo)
        {
            memo_ = memo;
        }
        std::vector<boost::shared_ptr<ContactInfo> > histories();
	private:
		//int id_;
        bool groupFinded_;
		boost::shared_ptr<ContactGroup> group_;
        int groupId_;
		std::string name_;
		std::string alias_;
		ContactType type_;
		//telecommunication
        TelephoneNumber mobilesTelephone_[2]; //20
        TelephoneNumber worksTelephone_[2]; //21
        TelephoneNumber internalTelephone_; //8
        TelephoneNumber fax_; //17
        TelephoneNumber familyTelephone_; //17
		//internet
        std::string msn_; //33
        std::string qq_; //12
        std::string skype_; //33
        std::string email_; //33
        std::string homesite_; //33
		//post
        std::string company_; //65
        std::string department_; //65
        std::string duty_; //25
        std::string companyAddress_; //65
        std::string companyPostCode_; //9
        std::string familyAddress_; //65
        std::string postCode_; //9
		//other
        std::string memo_; //513
		//history
        bool historiesFinded_;
        std::vector<boost::shared_ptr<ContactInfo> > histories_;
		/*
        //meta info
        Util::Timestamp creationTime_;
        Util::Timestamp lastReadTime_;
        Util::Timestamp lastWriteTime_;
        std::vector<Util::Timestamp> accessTimes_;
        Util::Timestamp destroyTime_;
		*/
	public:
		Contact(); //create
		~Contact(); //destroy
		static std::vector<boost::shared_ptr<Contact> > GetFromDatabase(std::string filter); //get from database
		void Update() const; //sync to database
		void Insert(); //insert new instance to database
        void Remove() const; //delete self from database
		static void Remove(std::string filter); //delete from database
		std::string ToCSV();
	private:
		static void modifyFieldByDB_(int argc, char** argv, char** columnName, boost::shared_ptr<Contact> item);
	};
}

#endif //__DATA_CONTACT_H__
