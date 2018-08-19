#include "MemberManagerSession.h"
#include "../Contact/Data/Contact.h"
#include "../Contact/Data/ContactCategory.h"

namespace Protocol {
    MemberManagerSession::MemberManagerSession(void) {
    }

    MemberManagerSession::~MemberManagerSession(void) {
    }

    void MemberManagerSession::SetParameter(std::string const & familyId) {
        familyId_ = familyId;
    }

    void MemberManagerSession::selfProcess_(Util::ResponseMessage const & response) const {
        /*
        <?xml version="1.0" encoding="UTF-8"?>
        <memberlistresponse>
        <familiyid>1234</familiyid>
        <count>2</count>
        <pack>
            <id>001</id>
            <type>0</type>
            <name>张三</name>
            <mobilenum>13900001111</mobilenum>
            <mail>1@1.com</mail>
            <desc>管理员</desc>
        </pack>
        <pack>
            <id>002</id>
            <type>1</type>
            <name>李四</name>
            <mobilenum>13900001112</mobilenum>
            <mail>1@2.com</mail>
            <desc>普通成员</desc>
        </pack>
        </memberlistresponse>
            */
        Util::shared_ptr<Contact::Data::ContactCategory> category;// = Contact::Data::ContactCategory::GetByName(L"家庭");
        Contact::Data::Contact::Remove(L"[groupId] = " + Util::StringOp::FromInt(category->id()));
        size_t o = 0;
        std::string pack = findContent_(response.Body, "pack", o);
        while (pack != "") {
            size_t offset = 0;
            std::string id = findContent_(pack, "id", offset);
            std::string type = findContent_(pack, "type", offset);
            std::string name = findContent_(pack, "name", offset);
            std::string phone = findContent_(pack, "mobilenum", offset);
            std::string mail = findContent_(pack, "mail", offset);
            std::string desc = findContent_(pack, "desc", offset);
            Contact::Data::Contact c;
            //c.id(Util::StringOp::ToInt(id));
            //c.type(Util::StringOp::ToUnicode(type));
            c.name(Util::StringOp::ToUnicode(name));
            c.mobile(Util::StringOp::ToUnicode(phone));
            //c.mail(Util::StringOp::ToUnicode(mail));
            //c.description(Util::StringOp::ToUnicode(desc));
            c.categoryId(category->id());
            c.Insert();
            pack = findContent_(response.Body, "pack", o);
        }
    }

    std::string const MemberManagerSession::resourceName_() const {
        return "ZF3G/tic/memberlist";
    }

    std::string const MemberManagerSession::additionParameter_() const {
        return "&familyid=" + familyId_;
    }
}
