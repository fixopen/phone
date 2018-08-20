#include "Contact.h"
#include "../../Telephone/Data/CallInfo.h"
#include "../../Util/Properties.h"

namespace Util {
    std::wstring DataAccess<Contact::Data::Contact>::tableName_ = L"contact";
    int DataAccess<Contact::Data::Contact>::count_ = 0;
    int DataAccess<Contact::Data::Contact>::offset_ = 0;
}

namespace Contact {
    namespace Data {
        ContactType const StringToContactType(std::wstring const & v) {
            ContactType result = ctNull;
            if (v == L"Normal") {
                result = ctNormal;
            } else if (v == L"VIP") {
                result = ctVip;
            } else if (v == L"Blacklist") {
                result = ctBlacklist;
            } else {
                result = ctError;
            }
            return result;
        }

        std::wstring const ContactTypeToString(ContactType const v) {
            std::wstring result = L"Null";
            switch (v) {
            case ctNormal:
                result = L"Normal";
                break;
            case ctVip:
                result = L"VIP";
                break;
            case ctBlacklist:
                result = L"Blacklist";
                break;
            default:
                result = L"Error";
                break;
            }
            return result;
        }

        ContactCategory const * const Contact::category() const {
            if (!categoryFinded_) {
                for (size_t i = 0; i < ContactCategory::All().size(); ++i) {
                    if (ContactCategory::All()[i]->id() == categoryId_) {
                        category_ = ContactCategory::All()[i].get();
                    }
                }
                //std::vector<Util::shared_ptr<ContactCategory> > t = ContactCategory::Select("[id] = " + Util::StringOp::FromInt(categoryId_));
                //if (t.size() == 1) {
                //    category_ = t[0];
                //}
                categoryFinded_ = true;
            }
            return category_;
        }

        void Contact::category(ContactCategory const * const category) {
            category_ = const_cast<ContactCategory*>(category);
            categoryId_ = category->id();
            categoryFinded_ = true;
        }

        std::vector<Util::shared_ptr<Telephone::Data::CallInfo> > const Contact::histories() const {
            if (!historiesFinded_) {
                histories_ = Telephone::Data::CallInfo::Select(L"[contactId] = " + Util::Text::StringOp::FromInt(id()));
                historiesFinded_ = true;
            }
            return histories_;
        }

        Contact::Contact()
        : DataAccess<Contact>()
        , categoryFinded_(false)
        , categoryId_(0)
        , category_(0)
        , type_(ctNormal)
        , historiesFinded_(false) {
        }

        std::vector<Util::shared_ptr<Contact> > Contact::Select(std::wstring const & filter, std::wstring const & orderFieldName, SeqenceRelation const sr, int const offset, int const pageSize) {
            return GetDatasByFilter(filter, modifyFieldByDB_, orderFieldName, sr, offset, pageSize);
        }

        void Contact::Update() const {
            std::wstring cmd = L"UPDATE [";
            cmd += tableName_;
            cmd += L"] SET [categoryId] = ";
            cmd += Util::Text::StringOp::FromInt(categoryId_);
            cmd += L", [name] = '";
            cmd += name_;
            cmd += L"', [alias] = '";
            cmd += alias_;
            cmd += L"', [type] = ";
            cmd += Util::Text::StringOp::FromInt(type_);
            cmd += L", [mobile] = '";
            cmd += mobiles_[0];
            cmd += L"', [mobile2] = '";
            cmd += mobiles_[1];
            cmd += L"', [telephone] = '";
            cmd += telephones_[0];
            cmd += L"', [telephone2] = '";
            cmd += telephones_[1];
            cmd += L"', [email] = '";
            cmd += email_;
            cmd += L"', [company] = '";
            cmd += company_;
            cmd += L"', [department] = '";
            cmd += department_;
            cmd += L"', [duty] = '";
            cmd += duty_;
            cmd += L"', [address] = '";
            cmd += address_;
            cmd += L"', [remarks] = '";
            cmd += remarks_;
            cmd += L"' WHERE [id] = ";
            cmd += Util::Text::StringOp::FromInt(id());
            ExecCommand(cmd);
        }

        void Contact::Insert() {
            std::wstring cmd = L"INSERT INTO [";
            cmd += tableName_;
            cmd += L"] ( [categoryId], [name], [alias], [type], [mobile], [mobile2], [telephone], [telephone2], [email], [company], [department], [duty], [address], [remarks] ) VALUES ( ";
            cmd += Util::Text::StringOp::FromInt(categoryId_);
            cmd += L", '";
            cmd += name_;
            cmd += L"', '";
            cmd += alias_;
            cmd += L"', ";
            cmd += Util::Text::StringOp::FromInt(type_);
            cmd += L", '";
            cmd += mobiles_[0];
            cmd += L"', '";
            cmd += mobiles_[1];
            cmd += L"', '";
            cmd += telephones_[0];
            cmd += L"', '";
            cmd += telephones_[1];
            cmd += L"', '";
            cmd += email_;
            cmd += L"', '";
            cmd += company_;
            cmd += L"', '";
            cmd += department_;
            cmd += L"', '";
            cmd += duty_;
            cmd += L"', '";
            cmd += address_;
            cmd += L"', '";
            cmd += remarks_;
            cmd += L"' )";
            ExecCommand(cmd);
            id(GetCurrentId());
        }

        void Contact::Remove() const {
            Remove(L"[id] = " + Util::Text::StringOp::FromInt(id()));
        }

        void Contact::Remove(std::wstring const & filter) {
            RemoveDatasByFilter(filter);
        }

        std::wstring const Contact::ToCSV() const {
            std::wstring result;
            //TODO: fill it
            return result;
        }

        std::wstring const Contact::ToVCard() const {
            std::wstring result;
            //TODO: fill it
            return result;
        }

        int const Contact::GetIdByName(std::wstring const & name) {
            int result = 0;
            Util::shared_ptr<Contact> contact = GetByName(name);
            if (contact) {
                result = contact->id();
            }
            return result;
        }

        int const Contact::GetIdByNumber(std::wstring const & number) {
            int result = 0;
            Util::shared_ptr<Contact> contact = GetByNumber(number);
            if (contact) {
                result = contact->id();
            }
            return result;
        }

        Util::shared_ptr<Contact> const Contact::GetByName(std::wstring const & name) {
            Util::shared_ptr<Contact> result;
            std::vector<Util::shared_ptr<Contact> > contacts = Select(L"[name] = '" + name + L"'");
            if (!contacts.empty()) {
                result = contacts[0];
            }
            return result;
        }

        Util::shared_ptr<Contact> const Contact::GetByNumber(std::wstring const & number) {
            Util::shared_ptr<Contact> result;
            std::vector<Util::shared_ptr<Contact> > contacts = Select(L"[telephone] = '" + number + L"' OR [telephone2] = '" + number + L"' OR [mobile] = '" + number + L"' OR [mobile2] = '" + number + L"'");
            if (!contacts.empty()) {
                result = contacts[0];
            }
            return result;
        }

        std::vector<Util::shared_ptr<Contact> > const Contact::GetSuggestContactsByName(std::wstring const & name) {
            return Select(L"[name] LIKE '%" + name + L"%'");
        }

        bool const Contact::IsBlock(std::wstring const & number) {
            bool result = false;
            //Util::shared_ptr<Contact> contact = GetByNumber(number);
            //FirewallType ft = static_cast<FirewallType>(Util::StringOp::ToInt(Util::Properties::Get(L"firewallType")));
            //switch (ft) {
            //    case ftNull:
            //        break;
            //    case ftBlacklist:
            //        if (contact) {
            //            result = contact->type() == ctBlacklist;
            //        }
            //        break;
            //    case ftNotInContacts:
            //        result = !contact;
            //        break;
            //    case ftNotVIP:
            //        result = contact->type() != ctVip;
            //        break;
            //    case ftAll:
            //        result = true;
            //        break;
            //    default:
            //        break;
            //}
            return result;
        }

        void Contact::Import(std::wstring const & filename) {
            FILE* file = fopen(Util::Text::StringOp::FromUnicode(filename).c_str(), "r");
            //FILE* file = 0;
            //fopen_s(&file, Util::Text::StringOp::FromUnicode(filename).c_str(), "r");
            if (file) {
                //std::string line;
                //std::getline(file, line);
                char buffer[1024];
                Contact contact;
                while (!feof(file)) {
                    memset(buffer, 0, 1024);
                    /*buffer = */fgets(buffer, 1024, file);
                    if (strlen(buffer) == 1023) {
                        //the buffer is overflow
                        //throw internal error
                    }
                    std::wstring line = Util::Text::StringOp::ToUnicode(buffer);
                    std::vector<std::wstring> fields = Util::Text::StringOp::Split(line, L",");
                    contact.name(fields[0]);
                    contact.mobile(fields[1]);
                    contact.street(fields[2]);
                    contact.state(fields[3]);
                    contact.postCode(fields[4]);
                    contact.Insert();
                }
                fclose(file);
            }
        }

        std::wstring const Contact::GetCategoryName() const {
            std::wstring result;
            if (!categoryFinded_) {
                for (size_t i = 0; i < ContactCategory::All().size(); ++i) {
                    if (ContactCategory::All()[i]->id() == categoryId_) {
                        result = ContactCategory::All()[i]->name();
                        category_ = ContactCategory::All()[i].get();
                    }
                }
                //std::vector<Util::shared_ptr<ContactCategory> > t = ContactCategory::Select("[id] = " + Util::Text::StringOp::FromInt(categoryId_));
                //if (t.size() == 1) {
                //    category_ = t[0];
                //}
                categoryFinded_ = true;
            }
            return result;
        }

        void Contact::modifyFieldByDB_(int argc, char** argv, char** columnName, Util::shared_ptr<Contact>& item) {
            item->id(atoi(argv[GetIndexByName(argc, columnName, "id")]));
            item->categoryId_ = atoi(argv[GetIndexByName(argc, columnName, "categoryId")]);
            item->name_ = Util::Text::StringOp::FromUTF8(argv[GetIndexByName(argc, columnName, "name")]);
            item->alias_ = Util::Text::StringOp::FromUTF8(argv[GetIndexByName(argc, columnName, "alias")]);
            item->type_ = static_cast<ContactType>(atoi(argv[GetIndexByName(argc, columnName, "type")]));
            item->mobiles_[0] = Util::Text::StringOp::FromUTF8(argv[GetIndexByName(argc, columnName, "mobile")]);
            item->mobiles_[1] = Util::Text::StringOp::FromUTF8(argv[GetIndexByName(argc, columnName, "mobile2")]);
            item->telephones_[0] = Util::Text::StringOp::FromUTF8(argv[GetIndexByName(argc, columnName, "telephone")]);
            item->telephones_[1] = Util::Text::StringOp::FromUTF8(argv[GetIndexByName(argc, columnName, "telephone2")]);
            item->email_ = Util::Text::StringOp::FromUTF8(argv[GetIndexByName(argc, columnName, "email")]);
            item->company_ = Util::Text::StringOp::FromUTF8(argv[GetIndexByName(argc, columnName, "company")]);
            item->department_ = Util::Text::StringOp::FromUTF8(argv[GetIndexByName(argc, columnName, "department")]);
            item->duty_ = Util::Text::StringOp::FromUTF8(argv[GetIndexByName(argc, columnName, "duty")]);
            item->address_ = Util::Text::StringOp::FromUTF8(argv[GetIndexByName(argc, columnName, "address")]);
            item->remarks_ = Util::Text::StringOp::FromUTF8(argv[GetIndexByName(argc, columnName, "remarks")]);
        }
    }
}
