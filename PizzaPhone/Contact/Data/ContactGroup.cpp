#include "..\..\stdafx.h"
#include "ContactGroup.h"
#include "Contact.h"

namespace Util {
    std::wstring Contact::Data::ContactGroup::tableName_ = L"contactGroup";
    int Contact::Data::ContactGroup::count_ = 0;
    int Contact::Data::ContactGroup::offset_ = 0;
}

namespace Contact {
    namespace Data {
        Util::shared_ptr<SoundSegment> ContactGroup::ring(void) {
            if (!ringFinded_) {
                std::vector<Util::shared_ptr<SoundSegment> > t = SoundSegment::Select(L"id = " + Util::Text::StringOp::FromInt(ringId_));
                //assert(t.size() == 1);
                if (t.size() == 1) {
                    ring_ = t[0];
                }
                ringFinded_ = true;
            }
            return ring_;
        }

        void ContactGroup::ring(Util::shared_ptr<SoundSegment> ring) {
            if (ring_) {
                //destory old ring
            }
            if (ring->id()) {
                //!= 0, always in database
                ring->Update();
            } else {
                ring->Insert();
            }
            ringId_ = ring->id();
        }

        Util::shared_ptr<SoundSegment> ContactGroup::tips() {
            if (!tipsFinded_) {
                std::vector<Util::shared_ptr<SoundSegment> > t = SoundSegment::Select(L"id = " + Util::Text::StringOp::FromInt(tipsId_));
                //assert(t.size() == 1);
                if (t.size() == 1) {
                    tips_ = t[0];
                }
                tipsFinded_ = true;
            }
            return tips_;
        }

        void ContactGroup::tips(Util::shared_ptr<SoundSegment> tips) {
            if (tips_) {
                //destory old ring
            }
            if (tips->id()) {
                //!= 0, always in database
                tips->Update();
            } else {
                tips->Insert();
            }
            tipsId_ = tips->id();
        }

        std::vector<Util::shared_ptr<Contact> > const ContactGroup::contacts(void) {
            if (!contactsFinded_) {
                contacts_ = Contact::Select(L"groupId = " + Util::Text::StringOp::FromInt(id()));
                contactsFinded_ = true;
            }
            return contacts_;
        }

        ContactGroup::ContactGroup()
        : DataAccess<ContactGroup>()
            //, name_(std::string())
            //, alias_(std::string())

        , useDefaultRing_(true)
        , ringFinded_(false)
            //, ring_(SoundSegment())
            //, useDefaultSMRing_(true)
            //, smRing_(SoundSegment())

        , useDefaultTips_(true)
        , tipsFinded_(false)
            //, tips_(SoundSegment())
            //, iconFinded_(false)
            //, icon_(Picture())
            //, parent_(0)
            //, isLeaf_(true)
            //, isEnableAppendSubGroup_(false)

        , contactsFinded_(false)
            //, contacts_(std::vector<Contact*>())
         {
        }

        std::vector<Util::shared_ptr<ContactGroup> > ContactGroup::Select(std::wstring const & filter, std::wstring const & orderFieldName, SeqenceRelation const dir, int const offset, int const pageSize) {
            return GetDatasByFilter(filter, modifyFieldByDB_, orderFieldName, dir, offset, pageSize);
        }

        void ContactGroup::Update() const {
            std::wstring cmd = L"UPDATE ";
            cmd += tableName_;
            cmd += L" SET [name] = '";
            cmd += name_;
            cmd += L"', [alias] = '";
            cmd += alias_;
            cmd += L"', [useDefaultRing] = ";
            cmd += Util::Text::StringOp::FromInt(useDefaultRing_);
            cmd += L", [ringId] = ";
            cmd += Util::Text::StringOp::FromInt(ringId_);
            /*
            cmd += ", [useDefaultSMRing_] = ";
            cmd += Util::Text::StringOp::FromInt(useDefaultSMRing_);
            cmd += ", [smRingId] = ";
            cmd += Util::Text::StringOp::FromInt(smRingId_);
            */
            cmd += L", [useDefaultTips] = ";
            cmd += Util::Text::StringOp::FromInt(useDefaultTips_);
            cmd += L", [tipsId] = ";
            cmd += Util::Text::StringOp::FromInt(tipsId_);
            cmd += L", [iconId] = ";
            cmd += Util::Text::StringOp::FromInt(iconId_);
            /*
            cmd += ", [parentId] = ";
            cmd += Util::Text::StringOp::FromInt(parentId_);
            cmd += ", [isLeaf] = ";
            cmd += Util::Text::StringOp::FromInt(isLeaf_);
            cmd += ", [isEnableAppendSubGroup] = ";
            cmd += Util::Text::StringOp::FromInt(isEnableAppendSubGroup_);
            */

            cmd += L", ringName = '";
            cmd += ringName_;
            cmd += L"', tipName = '";
            cmd += tipName_;
            cmd += L"'";

            cmd += L" WHERE id = ";
            cmd += Util::Text::StringOp::FromInt(id());
            ExecCommand(cmd);
        }

        void ContactGroup::Insert() {
            std::wstring cmd = L"INSERT INTO ";
            cmd += tableName_;
            cmd += L" ( name, alias, useDefaultRing, ringId, useDefaultTips, tipsId, iconId, ringName, tipName) VALUES ( '"; //useDefaultSMRing, smRingId, , parentId, isLeaf, isEnableAppendSubGroup
            cmd += name_;
            cmd += L"', '";
            cmd += alias_;
            cmd += L"', ";
            cmd += Util::Text::StringOp::FromInt(useDefaultRing_);
            cmd += L", ";
            cmd += Util::Text::StringOp::FromInt(ringId_);
            /*
            cmd += ", ";
            cmd += Util::Text::StringOp::FromInt(useDefaultSMRing_);
            cmd += ", ";
            cmd += Util::Text::StringOp::FromInt(smRingId_);
            */
            cmd += L", ";
            cmd += Util::Text::StringOp::FromInt(useDefaultTips_);
            cmd += L", ";
            cmd += Util::Text::StringOp::FromInt(tipsId_);
            cmd += L", ";
            cmd += Util::Text::StringOp::FromInt(iconId_);
            /*
            cmd += ", ";
            cmd += Util::Text::StringOp::FromInt(parentId_);
            cmd += ", ";
            cmd += Util::Text::StringOp::FromInt(isLeaf_);
            cmd += ", ";
            cmd += Util::Text::StringOp::FromInt(isEnableAppendSubGroup_);
            */

            cmd += L", '";
            cmd += ringName_;
            cmd += L"', '";
            cmd += tipName_;
            cmd += L"'";

            cmd += L" )";
            ExecCommand(cmd);
            id(GetCurrentId());
        }

        void ContactGroup::Remove() const {
            ContactGroup::Remove(L"id = " + Util::Text::StringOp::FromInt(id()));
        }

        void ContactGroup::Remove(std::wstring filter) {
            DataAccess<ContactGroup>::RemoveDatasByFilter(filter);
        }

        void ContactGroup::modifyFieldByDB_(int argc, char** argv, char** columnName, Util::shared_ptr<ContactGroup>& item) {
            item->id(atoi(argv[GetIndexByName(argc, columnName, "id")]));
            item->name_ = Util::Text::StringOp::FromUTF8(argv[GetIndexByName(argc, columnName, "name")]);
            item->alias_ = Util::Text::StringOp::FromUTF8(argv[GetIndexByName(argc, columnName, "alias")]);
            item->useDefaultRing_ = !!atoi(argv[GetIndexByName(argc, columnName, "useDefaultRing")]);
            item->ringId_ = atoi(argv[GetIndexByName(argc, columnName, "ringId")]);
            //item->useDefaultSMRing_ = !!atoi(argv[GetIndexByName(argc, columnName, "useDefaultSMRing")]);
            //item->smRingId_ = atoi(argv[GetIndexByName(argc, columnName, "smRingId")]);
            item->useDefaultTips_ = !!atoi(argv[GetIndexByName(argc, columnName, "useDefaultTips")]);
            item->tipsId_ = atoi(argv[GetIndexByName(argc, columnName, "tipsId")]);
            item->iconId_ = atoi(argv[GetIndexByName(argc, columnName, "iconId")]);
            //item->parentId_ = atoi(argv[GetIndexByName(argc, columnName, "parentId")]);
            //item->isLeaf_ = !!atoi(argv[GetIndexByName(argc, columnName, "isLeaf")]);
            //item->isEnableAppendSubGroup_ = !!atoi(argv[GetIndexByName(argc, columnName, "isEnableAppendSubGroup")]);
            item->ringName_ = Util::Text::StringOp::FromUTF8(argv[GetIndexByName(argc, columnName, "ringName")]);
            item->tipName_ = Util::Text::StringOp::FromUTF8(argv[GetIndexByName(argc, columnName, "tipName")]);
        }
    }
}
