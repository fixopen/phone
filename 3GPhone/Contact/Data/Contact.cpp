#include "Contact.h"
#include "ContactGroup.h"
#include "../../Telephone/Data/ContactInfo.h" 


#include "../../Util/StringOp.h"

namespace Util {
    std::wstring Contact_::Data::Contact::tableName_ = L"contact";
    int Contact_::Data::Contact::count_ = 0;
    int Contact_::Data::Contact::offset_ = 0;
}

namespace Contact_ {
    namespace Data {
		Util::shared_ptr<ContactGroup> Data::Contact::group() {
            if (!groupFinded_) {
				std::vector<Util::shared_ptr<ContactGroup> > t = ::Contact_::Data::ContactGroup::Select(L"id = " + Util::StringOp::FromInt(groupId_));
                //assert(t.size() == 1);
                if (t.size() == 1) {
                    group_ = t[0];
                }
                groupFinded_ = true;
            }
            return group_;
        }

		std::vector<Util::shared_ptr<Telephone::Data::ContactInfo>> Contact::histories() {
            if (!historiesFinded_) {
				histories_ = Telephone::Data::ContactInfo::Select(L"contactId = " + Util::StringOp::FromInt(id()));//type differ
                historiesFinded_ = true;
            }
            return histories_;
        }

        Contact::Contact()
        : DataAccess<Contact>()
        , groupFinded_(false)
        //, group_(0)
        , groupId_(0)
        , name_()
        , alias_()
        , type_(ctNormal)
		/*, mobilesTelephone_[2]()
        , worksTelephone_[2]()*/
        , internalTelephone_()
        , fax_()
        , familyTelephone_()
        , msn_()
        , qq_()
        , skype_()
        , email_()
        , homesite_()
        , company_()
        , duty_()
        , companyAddress_()
        , companyPostCode_()
        , familyAddress_()
        , postCode_()
        , memo_()
        , photo_()
        , historiesFinded_(false)
        {
			//histories_() 

        }

		
        std::vector<Util::shared_ptr<Contact> > const Data::Contact::Select(std::wstring const& filter, std::wstring const& orderFieldName, SeqenceRelation const sr, int const offset, int const pageSize) {
            return GetDatasByFilter(filter, modifyFieldByDB_, orderFieldName, sr, offset, pageSize); 
        }

        void Data::Contact::Update() const {
            std::wstring cmd = L"UPDATE ";
            cmd += tableName_;
            cmd += L" SET [groupId] = ";
            cmd += Util::StringOp::FromInt(groupId_);
            cmd += L", [name] = '";
            cmd += name_;
            cmd += L"', [alias] = '";
            cmd += alias_;
            cmd += L"', [type] = ";
            cmd += Util::StringOp::FromInt(type_);
            cmd += L", [mobilesTelephone] = '";
            cmd += mobilesTelephone_[0].number();
            cmd += L"', [mobilesTelephone2] = '";
            cmd += mobilesTelephone_[1].number();
            cmd += L"', [worksTelephone] = '";
            cmd += worksTelephone_[0].number();
            cmd += L"', [worksTelephone2] = '";
            cmd += worksTelephone_[1].number();
            cmd += L"', [internalTelephone] = '";
            cmd += internalTelephone_.number();
            cmd += L"', [fax] = '";
            cmd += fax_.number();
            cmd += L"', [familyTelephone] = '";
            cmd += familyTelephone_.number();
            cmd += L"', [msn] = '";
            cmd += msn_;
            cmd += L"', [qq] = '";
            cmd += qq_;
            cmd += L"', [skype] = '";
            cmd += skype_;
            cmd += L"', [email] = '";
            cmd += email_;
            cmd += L"', [homesite] = '";
            cmd += homesite_;
            cmd += L"', [company] = '";
            cmd += company_;
            cmd += L"', [department] = '";
            cmd += department_;
            cmd += L"', [duty] = '";
            cmd += duty_;
            cmd += L"', [companyAddress] = '";
            cmd += companyAddress_;
            cmd += L"', [companyPostCode] = '";
            cmd += companyPostCode_;
            cmd += L"', [familyAddress] = '";
            cmd += familyAddress_;
            cmd += L"', [postCode] = '";
            cmd += postCode_;
            cmd += L"', [memo] = '";
            cmd += memo_;
            cmd += L"', [photo] = '";
            cmd += photo_;
            cmd += L"' WHERE id = ";
            cmd += Util::StringOp::FromInt(id());
            ExecCommand(cmd);
        }

		void Data::Contact::Insert() {
            std::wstring cmd = L"INSERT INTO ";
            cmd += tableName_;
            cmd += L" ( groupId, name, alias, type, mobilesTelephone, mobilesTelephone2, worksTelephone, worksTelephone2, internalTelephone, fax, familyTelephone, msn, qq, skype, email, homesite, company, department, duty, companyAddress, companyPostCode, familyAddress, postCode, memo, photo ) VALUES ( ";
            cmd += Util::StringOp::FromInt(groupId_);
            cmd += L", '";
            cmd += name_;
            cmd += L"', '";
            cmd += alias_;
            cmd += L"', ";
            cmd += Util::StringOp::FromInt(type_);
            cmd += L", '";
            cmd += mobilesTelephone_[0].number();
            cmd += L"', '";
            cmd += mobilesTelephone_[1].number();
            cmd += L"', '";
            cmd += worksTelephone_[0].number();
            cmd += L"', '";
            cmd += worksTelephone_[1].number();
            cmd += L"', '";
            cmd += internalTelephone_.number();
            cmd += L"', '";
            cmd += fax_.ToString();
            cmd += L"', '";
            cmd += familyTelephone_.number();
            cmd += L"', '";
            cmd += msn_;
            cmd += L"', '";
            cmd += qq_;
            cmd += L"', '";
            cmd += skype_;
            cmd += L"', '";
            cmd += email_;
            cmd += L"', '";
            cmd += homesite_;
            cmd += L"', '";
            cmd += company_;
            cmd += L"', '";
            cmd += department_;
            cmd += L"', '";
            cmd += duty_;
            cmd += L"', '";
            cmd += companyAddress_;
            cmd += L"', '";
            cmd += companyPostCode_;
            cmd += L"', '";
            cmd += familyAddress_;
            cmd += L"', '";
            cmd += postCode_;
            cmd += L"', '";
            cmd += memo_;
            cmd += L"', '";
            cmd += photo_;
            cmd += L"' )";
            ExecCommand(cmd);
            id(GetCurrentId());
        }

		void Data::Contact::Remove() const {
            Remove(L"id = " + Util::StringOp::FromInt(id()));
        }

		void Data::Contact::Remove(std::wstring const& filter) {
            RemoveDatasByFilter(filter);
        }

		void Data::Contact::modifyFieldByDB_(int argc, char** argv, char** columnName, Util::shared_ptr<Contact> item) {
            item->id(atoi(argv[GetIndexByName(argc, columnName, "id")]));
            item->groupId_ = atoi(argv[GetIndexByName(argc, columnName, "groupId")]);
            item->name_ = Util::StringOp::FromUTF8(argv[GetIndexByName(argc, columnName, "name")]);
            item->alias_ = Util::StringOp::FromUTF8(argv[GetIndexByName(argc, columnName, "alias")]);
            item->type_ = static_cast<ContactType>(atoi(argv[GetIndexByName(argc, columnName, "type")]));
			item->mobilesTelephone_[0] = TelephoneNumber(Util::StringOp::FromUTF8(argv[GetIndexByName(argc, columnName, "mobilesTelephone")]), true);
            item->mobilesTelephone_[1] = TelephoneNumber(Util::StringOp::FromUTF8(argv[GetIndexByName(argc, columnName, "mobilesTelephone2")]), true);
            item->worksTelephone_[0] = TelephoneNumber(Util::StringOp::FromUTF8(argv[GetIndexByName(argc, columnName, "worksTelephone")]), true);
            item->worksTelephone_[1] = TelephoneNumber(Util::StringOp::FromUTF8(argv[GetIndexByName(argc, columnName, "worksTelephone2")]), true);
            item->internalTelephone_ = TelephoneNumber(Util::StringOp::FromUTF8(argv[GetIndexByName(argc, columnName, "internalTelephone")]), true);
            item->fax_ = TelephoneNumber(Util::StringOp::FromUTF8(argv[GetIndexByName(argc, columnName, "fax")]), true);
            item->familyTelephone_ = TelephoneNumber(Util::StringOp::FromUTF8(argv[GetIndexByName(argc, columnName, "familyTelephone")]), true);
            item->msn_ = Util::StringOp::FromUTF8(argv[GetIndexByName(argc, columnName, "msn")]);
            item->qq_ = Util::StringOp::FromUTF8(argv[GetIndexByName(argc, columnName, "qq")]);
            item->skype_ = Util::StringOp::FromUTF8(argv[GetIndexByName(argc, columnName, "skype")]);
            item->email_ = Util::StringOp::FromUTF8(argv[GetIndexByName(argc, columnName, "email")]);
            item->homesite_ = Util::StringOp::FromUTF8(argv[GetIndexByName(argc, columnName, "homesite")]);
            item->company_ = Util::StringOp::FromUTF8(argv[GetIndexByName(argc, columnName, "company")]);
            item->department_ = Util::StringOp::FromUTF8(argv[GetIndexByName(argc, columnName, "department")]);
            item->duty_ = Util::StringOp::FromUTF8(argv[GetIndexByName(argc, columnName, "duty")]);
            item->companyAddress_ = Util::StringOp::FromUTF8(argv[GetIndexByName(argc, columnName, "companyAddress")]);
            item->companyPostCode_ = Util::StringOp::FromUTF8(argv[GetIndexByName(argc, columnName, "companyPostCode")]);
            item->familyAddress_ = Util::StringOp::FromUTF8(argv[GetIndexByName(argc, columnName, "familyAddress")]);
            item->postCode_ = Util::StringOp::FromUTF8(argv[GetIndexByName(argc, columnName, "postCode")]);
            item->memo_ = Util::StringOp::FromUTF8(argv[GetIndexByName(argc, columnName, "memo")]);
            item->photo_ = Util::StringOp::FromUTF8(argv[GetIndexByName(argc, columnName, "photo")]);
        }
    }
}
