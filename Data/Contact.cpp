#include "../stdafx.h"
#include "Contact.h"
#include "ContactGroup.h"
#include "ContactInfo.h"

#include "../Util/StringOp.h"

namespace Data
{
	CRITICAL_SECTION sqliteSetion_;
    std::string DataAccess<Contact>::tableName_ = "contact";
    sqlite3* DataAccess<Contact>::db_ = Data::GetDB();
    int DataAccess<Contact>::rowCount_ = 0;
	Indication DataAccess<Contact>::indication_ = 0;
	int DataAccess<Contact>::offset_ = 0;

    boost::shared_ptr<ContactGroup> Contact::group()
    {
        if (!groupFinded_)
        {
            std::vector<boost::shared_ptr<ContactGroup> > t = ContactGroup::GetFromDatabase("id = " + Util::StringOp::FromInt(groupId_));
            //assert(t.size() == 1);
			if (t.size() != 0)
				group_ = t[0];
            groupFinded_ = true;
        }
        return group_;
    }

    std::vector<boost::shared_ptr<ContactInfo> > Contact::histories()
    {
        if (!historiesFinded_)
        {
            histories_ = ContactInfo::GetFromDatabase("contactId = " + Util::StringOp::FromInt(id()));
            historiesFinded_ = true;
        }
        return histories_;
    }

    Contact::Contact()
    : DataAccess<Contact>()
    , groupFinded_(false)
    , groupId_(0)
    , type_(ctNormal)
    , historiesFinded_(false)
    {
		id(0);
    }

    Contact::~Contact()
    {
        //do nothing
    }

    std::vector<boost::shared_ptr<Contact> > Contact::GetFromDatabase(std::string filter, Direction const dir, int const id, int const pageSize)
    {
		int contactLength = sizeof(Contact);
		int boostPointerLength = sizeof(boost::shared_ptr<Contact>);
		int vectorLength = sizeof(std::vector<boost::shared_ptr<Contact> >);
        return DataAccess<Contact>::GetDatasByFilter(filter, modifyFieldByDB_, dir, id, pageSize);
    }

	std::vector<boost::shared_ptr<Contact> >\
		Contact::GetFromDatabaseByOrderOffsetLength(\
		std::string type,std::string order,\
		int const offset /* = 0 */, int const pageSize /* = 0 */)
	{
		std::string filter = type;
        return DataAccess<Contact>::GetDatasByFilterAndLimit(\
			filter, modifyFieldByDB_, offset, pageSize,order,true);
	}

    void Contact::Update() const
    {
        std::string cmd = "UPDATE ";
        cmd += tableName();
        cmd += " SET [groupId] = ";
        cmd += Util::StringOp::FromInt(groupId_);
        cmd += ", [name] = '";
        cmd += name_;
        cmd += "', [alias] = '";
        cmd += alias_;
        cmd += "', [type] = ";
        cmd += Util::StringOp::FromInt(type_);
        cmd += ", [mobilesTelephone] = '";
        cmd += mobilesTelephone_[0].number();
        cmd += "', [mobilesTelephone2] = '";
        cmd += mobilesTelephone_[1].number();
        cmd += "', [worksTelephone] = '";
        cmd += worksTelephone_[0].number();
        cmd += "', [worksTelephone2] = '";
        cmd += worksTelephone_[1].number();
//         cmd += "', [internalTelephone] = '";
//         cmd += ""; //internalTelephone_.number();
//         cmd += "', [fax] = '";
//         cmd += ""; //fax_.number();
//         cmd += "', [familyTelephone] = '";
//         cmd += ""; //familyTelephone_.number();
//         cmd += "', [msn] = '";
//         cmd += ""; //msn_;
//         cmd += "', [qq] = '";
//         cmd += ""; //qq_;
//         cmd += "', [skype] = '";
//         cmd += ""; //skype_;
        cmd += "', [email] = '";
        cmd += email_;
        cmd += "', [homesite] = '";
        cmd += homesite_;
        cmd += "', [company] = '";
        cmd += company_;
        cmd += "', [department] = '";
        cmd += department_;
        cmd += "', [duty] = '";
        cmd += duty_;
        cmd += "', [companyAddress] = '";
        cmd += companyAddress_;
        cmd += "', [companyPostCode] = '";
        cmd += companyPostCode_;
//         cmd += "', [familyAddress] = '";
//         cmd += ""; //familyAddress_;
//         cmd += "', [postCode] = '";
//         cmd += ""; //postCode_;
        cmd += "', [memo] = '";
        cmd += memo_;
        cmd += "' WHERE id = ";
        cmd += Util::StringOp::FromInt(id());
        ExecCommand(cmd);
    }
	
    void Contact::Insert()
    {
        std::string cmd = "INSERT INTO ";
        cmd += tableName();
//        cmd += " ( groupId, name, alias, type, mobilesTelephone, mobilesTelephone2, worksTelephone, worksTelephone2, internalTelephone, fax, familyTelephone, msn, qq, skype, email, homesite, company, department, duty, companyAddress, companyPostCode, familyAddress, postCode, memo ) VALUES ( ";
        cmd += " ( groupId, name, alias, type, mobilesTelephone, mobilesTelephone2, worksTelephone, worksTelephone2, email, homesite, company, department, duty, companyAddress, companyPostCode, memo ) VALUES ( ";
        cmd += Util::StringOp::FromInt(groupId_);
        cmd += ", '";
        cmd += name_;
        cmd += "', '";
        cmd += alias_;
        cmd += "', ";
        cmd += Util::StringOp::FromInt(type_);
        cmd += ", '";
        cmd += mobilesTelephone_[0].number();
        cmd += "', '";
        cmd += mobilesTelephone_[1].number();
        cmd += "', '";
        cmd += worksTelephone_[0].number();
        cmd += "', '";
        cmd += worksTelephone_[1].number();
        cmd += "', '";
//         cmd += ""; //internalTelephone_.number();
//         cmd += "', '";
//         cmd += ""; //fax_.ToString();
//         cmd += "', '";
//         cmd += ""; //familyTelephone_.number();
//         cmd += "', '";
//         cmd += ""; //msn_;
//         cmd += "', '";
//         cmd += ""; //qq_;
//         cmd += "', '";
//         cmd += ""; //skype_;
//         cmd += "', '";
        cmd += email_;
        cmd += "', '";
        cmd += homesite_;
        cmd += "', '";
        cmd += company_;
        cmd += "', '";
        cmd += department_;
        cmd += "', '";
        cmd += duty_;
        cmd += "', '";
        cmd += companyAddress_;
        cmd += "', '";
        cmd += companyPostCode_;
//         cmd += "', '";
//         cmd += ""; //familyAddress_;
//         cmd += "', '";
//         cmd += ""; //postCode_;
        cmd += "', '";
        cmd += memo_;
        cmd += "' )";
        ExecCommand(cmd);
        id(GetCurrentId());
    }

    void Contact::Remove() const
    {
        Contact::Remove("id = " + Util::StringOp::FromInt(id()));
    }

	void Contact::Opendb()
	{
		DataAccess<Contact>::db_=Data::OpenDB();
	}

	void Contact::CloseDb()
	{
		Data::CloseDB(DataAccess<Contact>::db_);
	}

    void Contact::Remove(std::string filter)
    {
        DataAccess<Contact>::RemoveDatasByFilter(filter);
    }

	void Contact::Compression()
    {
		Data::DataAccess<Contact>::Compression();
    }

    void Contact::modifyFieldByDB_(int argc, char** argv, char** columnName, boost::shared_ptr<Contact> item)
    {
        item->id(atoi(argv[Data::getIndexByName(argc, columnName, "id")]));
        item->groupId_ = atoi(argv[Data::getIndexByName(argc, columnName, "groupId")]);
        item->name_ = argv[Data::getIndexByName(argc, columnName, "name")];
        item->alias_ = argv[Data::getIndexByName(argc, columnName, "alias")];
        item->type_ = static_cast<ContactType>(atoi(argv[Data::getIndexByName(argc, columnName, "type")]));
        item->mobilesTelephone_[0] = TelephoneNumber(argv[Data::getIndexByName(argc, columnName, "mobilesTelephone")], true);
        item->mobilesTelephone_[1] = TelephoneNumber(argv[Data::getIndexByName(argc, columnName, "mobilesTelephone2")], true);
        item->worksTelephone_[0] = TelephoneNumber(argv[Data::getIndexByName(argc, columnName, "worksTelephone")], true);
        item->worksTelephone_[1] = TelephoneNumber(argv[Data::getIndexByName(argc, columnName, "worksTelephone2")], true);
        //item->internalTelephone_ = TelephoneNumber(argv[Data::getIndexByName(argc, columnName, "internalTelephone")], true);
        //item->fax_ = TelephoneNumber(argv[Data::getIndexByName(argc, columnName, "fax")], true);
        //item->familyTelephone_ = TelephoneNumber(argv[Data::getIndexByName(argc, columnName, "familyTelephone")], true);
        //item->msn_ = argv[Data::getIndexByName(argc, columnName, "msn")];
        //item->qq_ = argv[Data::getIndexByName(argc, columnName, "qq")];
        //item->skype_ = argv[Data::getIndexByName(argc, columnName, "skype")];
        item->email_ = argv[Data::getIndexByName(argc, columnName, "email")];
        item->homesite_ = argv[Data::getIndexByName(argc, columnName, "homesite")];
        item->company_ = argv[Data::getIndexByName(argc, columnName, "company")];
        item->department_ = argv[Data::getIndexByName(argc, columnName, "department")];
        item->duty_ = argv[Data::getIndexByName(argc, columnName, "duty")];
        item->companyAddress_ = argv[Data::getIndexByName(argc, columnName, "companyAddress")];
        item->companyPostCode_ = argv[Data::getIndexByName(argc, columnName, "companyPostCode")];
        //item->familyAddress_ = argv[Data::getIndexByName(argc, columnName, "familyAddress")];
        //item->postCode_ = argv[Data::getIndexByName(argc, columnName, "postCode")];
        item->memo_ = argv[Data::getIndexByName(argc, columnName, "memo")];
    }
}
