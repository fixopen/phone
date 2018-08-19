#include "ContactCategory.h"
#include "SoundSegment.h"

namespace Util {
    std::wstring DataAccess<Contact::Data::ContactCategory>::tableName_ = L"contactCategory";
    int DataAccess<Contact::Data::ContactCategory>::count_ = 0;
    int DataAccess<Contact::Data::ContactCategory>::offset_ = 0;
}

namespace Contact {
    namespace Data {
        std::vector<Util::shared_ptr<Contact> > const ContactCategory::contacts() const {
            if (!contactsFinded_) {
                contacts_ = Contact::Select(L"[categoryId] = " + Util::Text::StringOp::FromInt(id()));
                contactsFinded_ = true;
            }
            return contacts_;
        }

        ContactCategory::ContactCategory()
        : DataAccess<ContactCategory>()
        , useDefaultRingtone_(true)
        , useDefaultPrompt_(true)
        , contactsFinded_(false) {
        }

        std::vector<Util::shared_ptr<ContactCategory> > ContactCategory::Select(std::wstring const & filter, std::wstring const & orderFieldName, SeqenceRelation const dir, int const offset, int const pageSize) {
            return GetDatasByFilter(filter, modifyFieldByDB_, orderFieldName, dir, offset, pageSize);
        }

        void ContactCategory::Update() const {
            std::wstring cmd = L"UPDATE [";
            cmd += tableName_;
            cmd += L"] SET [name] = '";
            cmd += name_;
            cmd += L"', [alias] = '";
            cmd += alias_;
            cmd += L"', [useDefaultRingtone] = ";
            cmd += Util::Text::StringOp::FromInt(useDefaultRingtone_);
            cmd += L", [ringtoneName] = '";
            cmd += ringtoneName_;
            cmd += L"', [useDefaultPrompt] = ";
            cmd += Util::Text::StringOp::FromInt(useDefaultPrompt_);
            cmd += L"', [promptName] = '";
            cmd += promptName_;
            cmd += L"' WHERE [id] = ";
            cmd += Util::Text::StringOp::FromInt(id());
            ExecCommand(cmd);
        }

        void ContactCategory::Insert() {
            std::wstring cmd = L"INSERT INTO [";
            cmd += tableName_;
            cmd += L"] ( [name], [alias], [useDefaultRingtone], [ringtoneName], [useDefaultPrompt], [promptName] ) VALUES ( '";
            cmd += name_;
            cmd += L"', '";
            cmd += alias_;
            cmd += L"', ";
            cmd += Util::Text::StringOp::FromInt(useDefaultRingtone_);
            cmd += L", '";
            cmd += ringtoneName_;
            cmd += L"', ";
            cmd += Util::Text::StringOp::FromInt(useDefaultPrompt_);
            cmd += L", '";
            cmd += promptName_;
            cmd += L"' )";
            ExecCommand(cmd);
            id(GetCurrentId());
        }

        void ContactCategory::Remove() const {
            Remove(L"id = " + Util::Text::StringOp::FromInt(id()));
        }

        void ContactCategory::Remove(std::wstring const & filter) {
            RemoveDatasByFilter(filter);
        }

        std::wstring const ContactCategory::GetRingtone(std::wstring const & number) {
            std::wstring result = SoundSegment::DefaultRingtone.filename();
            Util::shared_ptr<Contact> contact = Contact::GetByNumber(number);
            if (contact) {
                //in contact list
                ContactCategory const * category = contact->category();
                if (category) {
                    result = category->ringtoneName();
                }
            }
            return result;
        }

        std::wstring const ContactCategory::GetPrompt(std::wstring const & number) {
            std::wstring result = SoundSegment::DefaultRingtone.filename();
            Util::shared_ptr<Contact> contact = Contact::GetByNumber(number);
            if (contact) {
                //in contact list
                ContactCategory const * category = contact->category();
                if (category) {
                    result = category->promptName();
                }
            }
            return result;
        }

        std::vector<Util::shared_ptr<ContactCategory> >& ContactCategory::All() {
            static std::vector<Util::shared_ptr<ContactCategory> > result = ContactCategory::Select(L"");
            return result;
        }

        int const ContactCategory::GetIdByName(std::wstring const & name) {
            int result = 0;
            std::vector<Util::shared_ptr<ContactCategory> > all = All();
            for (size_t i = 0; i < all.size(); ++i) {
                if (all[i]->name() == name) {
                    result = all[i]->id();
                    break;
                }
            }
            return result;
        }

        void ContactCategory::modifyFieldByDB_(int argc, char** argv, char** columnName, Util::shared_ptr<ContactCategory>& item) {
            item->id(atoi(argv[GetIndexByName(argc, columnName, "id")]));
            item->name_ = Util::Text::StringOp::FromUTF8(argv[GetIndexByName(argc, columnName, "name")]);
            item->alias_ = Util::Text::StringOp::FromUTF8(argv[GetIndexByName(argc, columnName, "alias")]);
            item->useDefaultRingtone_ = !!atoi(argv[GetIndexByName(argc, columnName, "useDefaultRingtone")]);
            item->ringtoneName_ = Util::Text::StringOp::FromUTF8(argv[GetIndexByName(argc, columnName, "ringtoneName")]);
            item->useDefaultPrompt_ = !!atoi(argv[GetIndexByName(argc, columnName, "useDefaultPrompt")]);
            item->promptName_ = Util::Text::StringOp::FromUTF8(argv[GetIndexByName(argc, columnName, "promptName")]);
        }
    }
}
