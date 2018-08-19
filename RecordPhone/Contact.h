#pragma once
#include "Util/Data.h"
#include "ContactCategory.h"

class CallInfo;

enum ContactType {
    ctNull,
    ctNormal,
    ctVip,
    ctBlacklist,
    ctError,
};

enum FirewallType {
    ftNull,
    ftBlacklist,
    ftNotInContacts,
    ftNotVIP,
    ftAll,
};

ContactType const StringToContactType(std::wstring const& v);
std::wstring const ContactTypeToString(ContactType const v);

typedef std::wstring TelephoneNumber;

class Contact : public Util::DataAccess<Contact> {
public:
    ContactCategory const* const category() const;
    void category(ContactCategory const* const category);
    void categoryId(int const categoryId) {
        categoryId_ = categoryId;
    }
    int const categoryId() const {
        return categoryId_;
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
    void type(ContactType const type) {
        type_ = type;
    }
    TelephoneNumber const mobile() const {
        return mobiles_[0];
    }
    void mobile(TelephoneNumber const& mobiles) {
        mobiles_[0] = mobiles;
    }
    TelephoneNumber const mobile2() const {
        return mobiles_[1];
    }
    void mobile2(TelephoneNumber const& mobiles2) {
        mobiles_[1] = mobiles2;
    }
    TelephoneNumber const telephone() const {
        return telephones_[0];
    }
    void telephone(TelephoneNumber const& telephone) {
        telephones_[0] = telephone;
    }
    TelephoneNumber const telephone2() const {
        return telephones_[1];
    }
    void telephone2(TelephoneNumber const& telephone2) {
        telephones_[1] = telephone2;
    }
    std::wstring const email() const {
        return email_;
    }
    void email(std::wstring const& email) {
        email_ = email;
    }
    std::wstring const company() const {
        return company_;
    }
    void company(std::wstring const& company) {
        company_ = company;
    }
    std::wstring const state() const {
        return company_;
    }
    void state(std::wstring const& state) {
        company_ = state;
    }
    std::wstring const department() const {
        return department_;
    }
    void department(std::wstring const& department) {
        department_ = department;
    }
    std::wstring const postCode() const {
        return department_;
    }
    void postCode(std::wstring const& postCode) {
        department_ = postCode;
    }
    std::wstring const duty() const {
        return duty_;
    }
    void duty(std::wstring const& duty) {
        duty_ = duty;
    }
    std::wstring const address() const {
        return address_;
    }
    void address(std::wstring const& address) {
        address_ = address;
    }
    std::wstring const street() const {
        return address_;
    }
    void street(std::wstring const& street) {
        address_ = street;
    }
    std::wstring const remarks() const {
        return remarks_;
    }
    void remarks(std::wstring const& remarks) {
        remarks_ = remarks;
    }
    std::vector<Util::shared_ptr<CallInfo> > const histories() const;
public:
    Contact(); //create
    //~Contact(); //destroy
    static std::vector<Util::shared_ptr<Contact> > Select(std::wstring const& filter, std::wstring const& orderFieldName = L"id", SeqenceRelation const sr = srDesc, int const offset = 0, int const pageSize = -1);
    void Update() const; //sync to database
    void Insert(); //insert new instance to database
    void Remove() const; //delete self from database
    static void Remove(std::wstring const& filter); //delete from database
    std::wstring const ToCSV() const;
    std::wstring const ToVCard() const;
    static int const GetIdByName(std::wstring const& name);
    static int const GetIdByNumber(std::wstring const& number);
    static Util::shared_ptr<Contact> const GetByName(std::wstring const& name);
    static Util::shared_ptr<Contact> const GetByNumber(std::wstring const& number);
    static std::vector<Util::shared_ptr<Contact> > const GetSuggestContactsByName(std::wstring const& name);
    static bool const IsBlock(std::wstring const& number);
    static void Import(std::wstring const& filename);
    std::wstring const GetCategoryName() const;
private:
    static void modifyFieldByDB_(int argc, char** argv, char** columnName, Util::shared_ptr<Contact>& item);
private:
    int categoryId_;
    mutable bool categoryFinded_;
    mutable ContactCategory* category_;
    std::wstring name_;
    std::wstring alias_;
    ContactType type_;
    //telecommunication
    TelephoneNumber mobiles_[2];
    TelephoneNumber telephones_[2];
    std::wstring email_;
    //post
    std::wstring company_; //65 @@rename to state
    std::wstring department_; //65 @@rename to postCode
    std::wstring duty_; //25
    std::wstring address_; //@@rename to street
    //other
    std::wstring remarks_; //513
    //history
    mutable bool historiesFinded_;
    mutable std::vector<Util::shared_ptr<CallInfo> > histories_;
};
