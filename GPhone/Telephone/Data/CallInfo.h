#ifndef __TELEPHONE_DATA_CALLINFO_H__
#define __TELEPHONE_DATA_CALLINFO_H__

#include "../../Util/Data.h"
#include "../../Contact/Data/Contact.h"
namespace Contact {
    namespace Data {
        class SoundSegment;
    }
}

namespace Telephone {
    namespace Data {
        typedef Contact::Data::SoundSegment Record;
    }
}

namespace Telephone {
    namespace Data {
        enum CallInfoType {
            citMissed,
            citReceived,
            citDialed,
            citLeaveWord,
        };

        std::wstring const CallInfoTypeToString(CallInfoType const type);
        CallInfoType const StringToCallInfoType(std::wstring const & string);

        class CallInfo : public Util::DataAccess<CallInfo> {
        public:
            CallInfoType const type() const {
                return type_;
            }
            void type(CallInfoType const type) {
                type_ = type;
            }
            Util::Timestamp const startTime() const {
                return startTime_;
            }
            void startTime(Util::Timestamp const startTime) {
                startTime_ = startTime;
            }
            Util::TimeSpan const duration() const {
                return duration_;
            }
            void duration(Util::TimeSpan const & duration) {
                duration_ = duration;
            }
            Contact::Data::TelephoneNumber const telephoneNumber() const {
                return telephoneNumber_;
            }
            void telephoneNumber(Contact::Data::TelephoneNumber const & telephoneNumber) {
                telephoneNumber_ = telephoneNumber;
            }
            int const contactId() const {
                return contactId_;
            }
            void contactId(int const contactId) {
                contactId_ = contactId;
            }
            std::wstring const name() const {
                std::wstring result;
                //contact();
                //if (contact_) {
                //    result = contact_->name();
                //}
                //if (!contactFinded_) {
                //    contactFinded_ = true;
                std::vector<Util::shared_ptr<Contact::Data::Contact> > t = Contact::Data::Contact::Select(L"id = " + Util::Text::StringOp::FromInt(contactId_));
                if (t.size() == 1) {
                    result = t[0]->name();
                }
                //}
                return result;
            }
            Contact::Data::Contact const * const contact() const {
                if (!contactFinded_) {
                    std::vector<Util::shared_ptr<Contact::Data::Contact> > t = Contact::Data::Contact::Select(L"id = " + Util::Text::StringOp::FromInt(contactId_));
                    if (t.size() == 1) {
                        contact_ = t[0].get();
                    }
                    contactFinded_ = true;
                }
                return contact_;
            }
            void contact(Contact::Data::Contact const * const contact) {
                contact_ = const_cast<Contact::Data::Contact*>(contact);
                contactId_ = contact->id();
                contactFinded_ = true;
            }
            bool const hasSound() const {
                return hasSound_;
            }
            void hasSound(bool const hasSound) {
                hasSound_ = hasSound;
            }
            std::vector<Util::shared_ptr<Record> > const sounds() const;
            void appendSound(Util::shared_ptr<Record>& soundSegment);
        public:
            CallInfo(); //create
            //~CallInfo(); //destroy
            static std::vector<Util::shared_ptr<CallInfo> > Select(std::wstring const & filter, std::wstring const & orderFieldName = L"id", SeqenceRelation const dir = srNull, int const offset = 0, int const pageSize = -1);
            void Update() const; //sync to database
            void Insert(); //insert new instance to database
            void Remove() const; //delete self from database
            static void Remove(std::wstring const & filter); //delete from database
            static std::vector<Util::shared_ptr<CallInfo> > GetAllNotPlayedLeaveWord();
            Util::shared_ptr<Record> const Record();
            void StopRecord();
        private:
            static void modifyFieldByDB_(int argc, char** argv, char** columnName, Util::shared_ptr<CallInfo>& item);
        private:
            CallInfoType type_;
            Util::Timestamp startTime_;
            Util::TimeSpan duration_;
            //int duration_;
            //contact
            Contact::Data::TelephoneNumber telephoneNumber_;
            int contactId_;
            mutable bool contactFinded_;
            //mutable Util::weak_ptr<Contact> contact_;
            mutable Contact::Data::Contact* contact_;
            //sounds
            bool hasSound_;
            mutable bool soundsFinded_;
            mutable std::vector<Util::shared_ptr<::Telephone::Data::Record> > sounds_;
            bool isInserted_;
            //append thread manager
        };
    }
}

#endif //__TELEPHONE_DATA_CALLINFO_H__
