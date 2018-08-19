#ifndef __CONTACT_DATA_CONTACT_H__
#define __CONTACT_DATA_CONTACT_H__

#include <string>
#include <vector>

#include "TelephoneNumber.h"
//typedef std::wstring TelephoneNumber;

//#include "SoundSegment.h"
//#include "Picture.h"
#include "../../Util/Data.h"
//#include "../../Util/DateTime.h"
#include "../../Util/SmartPtr.h"

namespace Telephone {
	namespace Data {
		class ContactInfo;
	}
}

namespace Contact_{
    namespace Data {
        class ContactGroup;

        enum ContactType {
            ctNormal,
            ctVip,
            ctBlacklist,
        };

        class Contact : public Util::DataAccess<Contact> {
        public:
            Util::shared_ptr<ContactGroup> group();
            void group(Util::shared_ptr<ContactGroup> group) {
                group_ = group;
            }
            void groupId(int groupId) {
                groupId_ = groupId;
            }
            int const groupId() const {
                return groupId_;
            }
            std::wstring const name() const {
                return name_;
            }
            void name(std::wstring const& name) {
                name_ = name;
            }
            std::wstring const alias() const {
                return alias_;
            }
            void alias(std::wstring const& alias) {
                alias_ = alias;
            }
            ContactType const type() const {
                return type_;
            }
            void type(ContactType const& type) {
                type_ = type;
            }
            TelephoneNumber const mobilesTelephone() const {
                return mobilesTelephone_[0];
            }
            void mobilesTelephone(TelephoneNumber const& mobilesTelephone) {
                mobilesTelephone_[0] = mobilesTelephone;
            }
            TelephoneNumber const mobilesTelephone2() const {
                return mobilesTelephone_[1];
            }
            void mobilesTelephone2(TelephoneNumber const& mobilesTelephone2) {
                mobilesTelephone_[1] = mobilesTelephone2;
            }
            TelephoneNumber const worksTelephone() const {
                return worksTelephone_[0];
            }
            void worksTelephone(TelephoneNumber const& worksTelephone) {
                worksTelephone_[0] = worksTelephone;
            }
            TelephoneNumber const worksTelephone2() const {
                return worksTelephone_[1];
            }
            void worksTelephone2(TelephoneNumber const& worksTelephone2) {
                worksTelephone_[1] = worksTelephone2;
            }
            TelephoneNumber const internalTelephone() const {
                return internalTelephone_;
            }
            void internalTelephone(TelephoneNumber const& internalTelephone) {
                internalTelephone_ = internalTelephone;
            }
            TelephoneNumber const fax() const {
                return fax_;
            }
            void fax(TelephoneNumber const fax) {
                fax_ = fax;
            }
            TelephoneNumber const familyTelephone() const {
                return familyTelephone_;
            }
            void familyTelephone(TelephoneNumber const& familyTelephone) {
                familyTelephone_ = familyTelephone;
            }
            std::wstring const msn() const {
                return msn_;
            }
            void msn(std::wstring const& msn) {
                msn_ = msn;
            }
            std::wstring const qq() const {
                return qq_;
            }
            void qq(std::wstring const& qq) {
                qq_ = qq;
            }
            std::wstring const skype() const {
                return skype_;
            }
            void skype(std::wstring const& skype) {
                skype_ = skype;
            }
            std::wstring const email() const {
                return email_;
            }
            void email(std::wstring const& email) {
                email_ = email;
            }
            std::wstring const homesite() const {
                return homesite_;
            }
            void homesite(std::wstring const& homesite) {
                homesite_ = homesite;
            }
            std::wstring const company() const {
                return company_;
            }
            void company(std::wstring const& company) {
                company_ = company;
            }
            std::wstring const department() const {
                return department_;
            }
            void department(std::wstring const& department) {
                department_ = department;
            }
            std::wstring const duty() const {
                return duty_;
            }
            void duty(std::wstring const& duty) {
                duty_ = duty;
            }
            std::wstring const companyAddress() const {
                return companyAddress_;
            }
            void companyAddress(std::wstring const& companyAddress) {
                companyAddress_ = companyAddress;
            }
            std::wstring const companyPostCode() const {
                return companyPostCode_;
            }
            void companyPostCode(std::wstring const& companyPostCode) {
                companyPostCode_ = companyPostCode;
            }
            std::wstring const familyAddress() const {
                return familyAddress_;
            }
            void familyAddress(std::wstring const& familyAddress) {
                familyAddress_ = familyAddress;
            }
            std::wstring const postCode() const {
                return postCode_;
            }
            void postCode(std::wstring const& postCode) {
                postCode_ = postCode;
            }
            std::wstring const memo() const {
                return memo_;
            }
            void memo(std::wstring const& memo) {
                memo_ = memo;
            }
            std::wstring const photo() const {
                return photo_;
            }
            void photo(std::wstring const& photo) {
                photo_ = photo;
            }
            std::vector<Util::shared_ptr<Telephone::Data::ContactInfo>> histories();
        private:
            //int id_;
            bool groupFinded_;
            Util::shared_ptr<ContactGroup> group_;
            int groupId_;
            std::wstring name_;
            std::wstring alias_;
            ContactType type_;
            //telecommunication
            TelephoneNumber mobilesTelephone_[2]; //20
            TelephoneNumber worksTelephone_[2]; //21
            TelephoneNumber internalTelephone_; //8
            TelephoneNumber fax_; //17
            TelephoneNumber familyTelephone_; //17
            //internet
            std::wstring msn_; //33
            std::wstring qq_; //12
            std::wstring skype_; //33
            std::wstring email_; //33
            std::wstring homesite_; //33
            //post
            std::wstring company_; //65
            std::wstring department_; //65
            std::wstring duty_; //25
            std::wstring companyAddress_; //65
            std::wstring companyPostCode_; //9
            std::wstring familyAddress_; //65
            std::wstring postCode_; //9
            //other
            std::wstring memo_; //513
            std::wstring photo_;
            //history
            bool historiesFinded_;
			std::vector<Util::shared_ptr<Telephone::Data::ContactInfo> > histories_;
        public:
            Contact(); //create
		
            static std::vector<Util::shared_ptr<Contact> > const Select(std::wstring const& filter, std::wstring const& orderFieldName = L"id", SeqenceRelation const sr = srNull, int const offset = 0, int const pageSize = -1);
            void Update() const; //sync to database
            void Insert(); //insert new instance to database
            void Remove() const; //delete self from database
            static void Remove(std::wstring const& filter); //delete from database
            std::wstring const ToCSV() const;
        private:
            static void modifyFieldByDB_(int argc, char** argv, char** columnName, Util::shared_ptr<Contact> item);
        };
    }
}

#endif //__CONTACT_DATA_CONTACT_H__
