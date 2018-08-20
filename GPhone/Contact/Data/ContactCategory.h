#ifndef __CONTACT_DATA_CONTACTCATEGORY_H__
#define __CONTACT_DATA_CONTACTCATEGORY_H__

#include "../../Util/Data.h"

namespace Contact {
    namespace Data {
        class Contact;
        class SoundSegment;

        class ContactCategory : public Util::DataAccess<ContactCategory> {
        public:
            std::wstring const name() const {
                return name_;
            }
            void name(std::wstring const& n) {
                name_ = n;
            }
            std::wstring const alias() const {
                return alias_;
            }
            void alias(std::wstring const& alias) {
                alias_ = alias;
            }
            bool const useDefaultRingtone() const {
                return useDefaultRingtone_;
            }
            void useDefaultRingtone(bool const useDefaultRing) {
                useDefaultRingtone_ = useDefaultRing;
            }
            std::wstring const ringtoneName() const {
                return ringtoneName_;
            }	
            void ringtoneName(std::wstring const& ringName) {
                ringtoneName_ = ringName;
            }
            static std::wstring const defaultRingtoneName() {
                return defaultRingtoneName_;
            }
            static void defaultRingtoneName(std::wstring const& defaultRing) {
                defaultRingtoneName_ = defaultRing;
            }
            bool const useDefaultPrompt() const {
                return useDefaultPrompt_;
            }
            void useDefaultPrompt(bool const useDefaultPrompt) {
                useDefaultPrompt_ = useDefaultPrompt;
            }
            std::wstring const promptName() const {
                return promptName_;
            }	
            void promptName(std::wstring const& tipName)	{
                promptName_ = tipName;
            }
            static std::wstring const defaultPromptName() {
                return defaultPromptName_;
            }
            static void defaultPromptName(std::wstring const& defaultPromptName) {
                defaultPromptName_ = defaultPromptName;
            }
            std::vector<Util::shared_ptr<Contact> > const contacts() const;
        public:
            ContactCategory(); //create
            //~ContactCategory(); //destroy
            static std::vector<Util::shared_ptr<ContactCategory> > Select(std::wstring const& filter, std::wstring const& orderFieldName = L"id", SeqenceRelation const dir = srNull, int const id = 0, int const pageSize = -1);
            void Update() const; //sync to database
            void Insert(); //insert new instance to database
            void Remove() const; //remove self from database
            static void Remove(std::wstring const& filter); //delete from database
            static std::wstring const GetRingtone(std::wstring const& number);
            static std::wstring const GetPrompt(std::wstring const& number);
            static std::vector<Util::shared_ptr<ContactCategory> >& All();
            static int const GetIdByName(std::wstring const& name);
        private:
            static void modifyFieldByDB_(int argc, char** argv, char** columnName, Util::shared_ptr<ContactCategory>& item);
        private:
            //int id_;
            std::wstring name_; //32
            std::wstring alias_;
            //ringtone
            bool useDefaultRingtone_;
            std::wstring ringtoneName_;
            static std::wstring defaultRingtoneName_;
            //prompt
            bool useDefaultPrompt_;
            std::wstring promptName_;
            static std::wstring defaultPromptName_;
            //contacts
            mutable bool contactsFinded_;
            mutable std::vector<Util::shared_ptr<Contact> > contacts_;
        };
    }
}

#endif //__CONTACT_DATA_CONTACTCATEGORY_H__
