#include "ContactGroup.h"
#include "Contact.h"

namespace Util {
    std::string DataAccess<ContactGroup>::tableName_ = "contactGroup";
    int DataAccess<ContactGroup>::count_ = 0;
    int DataAccess<ContactGroup>::offset_ = 0;
}

namespace Contact {
    namespace Data
    {
        Util::shared_ptr<SoundSegment> ContactGroup::ring(void) {
            if (!ringFinded_) {
                std::vector<Util::shared_ptr<SoundSegment> > t = SoundSegment::GetFromDatabase("id = " + Util::StringOp::FromInt(ringId_));
                //assert(t.size() == 1);
                if (t.size() == 1) {
                    ring_ = t[0];
                }
                ringFinded_ = true;
            }
            return ring_;
        }

        void ContactGroup::ring(Util::shared_ptr<SoundSegment> ring)
        {
            if (ring_) {
                //destory old ring
            }
            if (ring->id()) { //!= 0, always in database
                ring->Update();
            } else {
                ring->Insert();
            }
            ringId_ = ring->id();
        }

        Util::shared_ptr<SoundSegment> ContactGroup::tips() {
            if (!tipsFinded_) {
                std::vector<Util::shared_ptr<SoundSegment> > t = SoundSegment::GetFromDatabase("id = " + Util::StringOp::FromInt(tipsId_));
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
            if (tips->id()) { //!= 0, always in database
                tips->Update();
            } else {
                tips->Insert();
            }
            tipsId_ = tips->id();
        }

        std::vector<Util::shared_ptr<Contact> > const ContactGroup::contacts(void) {
            if (!contactsFinded_) {
                contacts_ = Contact::GetFromDatabase("groupId = " + Util::StringOp::FromInt(id()));
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

        std::vector<Util::shared_ptr<ContactGroup> > ContactGroup::GetFromDatabase(std::string const& filter, std::string const& orderFieldName, SeqenceRelation const dir, int const offset, int const pageSize) {
            return DataAccess<ContactGroup>::GetDatasByFilter(filter, modifyFieldByDB_, orderFieldName, dir, offset, pageSize);
        }

        void ContactGroup::Update() const {
            std::string cmd = "UPDATE ";
            cmd += tableName();
            cmd += " SET [name] = '";
            cmd += name_;
            cmd += "', [alias] = '";
            cmd += alias_;
            cmd += "', [useDefaultRing] = ";
            cmd += Util::StringOp::FromInt(useDefaultRing_);
            cmd += ", [ringId] = ";
            cmd += Util::StringOp::FromInt(ringId_);
            /*
            cmd += ", [useDefaultSMRing_] = ";
            cmd += Util::StringOp::FromInt(useDefaultSMRing_);
            cmd += ", [smRingId] = ";
            cmd += Util::StringOp::FromInt(smRingId_);
            */
            cmd += ", [useDefaultTips] = ";
            cmd += Util::StringOp::FromInt(useDefaultTips_);
            cmd += ", [tipsId] = ";
            cmd += Util::StringOp::FromInt(tipsId_);
            cmd += ", [iconId] = ";
            cmd += Util::StringOp::FromInt(iconId_);
            /*
            cmd += ", [parentId] = ";
            cmd += Util::StringOp::FromInt(parentId_);
            cmd += ", [isLeaf] = ";
            cmd += Util::StringOp::FromInt(isLeaf_);
            cmd += ", [isEnableAppendSubGroup] = ";
            cmd += Util::StringOp::FromInt(isEnableAppendSubGroup_);
            */

            cmd += ", ringName = '";
            cmd += ringName_;
            cmd += "', tipName = '";
            cmd += tipName_;
            cmd += "'";

            cmd += " WHERE id = ";
            cmd += Util::StringOp::FromInt(id());
            ExecCommand(cmd);
        }

        void ContactGroup::Insert() {
            std::string cmd = "INSERT INTO ";
            cmd += tableName();
            cmd += " ( name, alias, useDefaultRing, ringId, useDefaultTips, tipsId, iconId, ringName, tipName) VALUES ( '"; //useDefaultSMRing, smRingId, , parentId, isLeaf, isEnableAppendSubGroup
            cmd += name_;
            cmd += "', '";
            cmd += alias_;
            cmd += "', ";
            cmd += Util::StringOp::FromInt(useDefaultRing_);
            cmd += ", ";
            cmd += Util::StringOp::FromInt(ringId_);
            /*
            cmd += ", ";
            cmd += Util::StringOp::FromInt(useDefaultSMRing_);
            cmd += ", ";
            cmd += Util::StringOp::FromInt(smRingId_);
            */
            cmd += ", ";
            cmd += Util::StringOp::FromInt(useDefaultTips_);
            cmd += ", ";
            cmd += Util::StringOp::FromInt(tipsId_);
            cmd += ", ";
            cmd += Util::StringOp::FromInt(iconId_);
            /*
            cmd += ", ";
            cmd += Util::StringOp::FromInt(parentId_);
            cmd += ", ";
            cmd += Util::StringOp::FromInt(isLeaf_);
            cmd += ", ";
            cmd += Util::StringOp::FromInt(isEnableAppendSubGroup_);
            */

            cmd += ", '";
            cmd += ringName_;
            cmd += "', '";
            cmd += tipName_;
            cmd += "'";

            cmd += " )";
            ExecCommand(cmd);
            id(GetCurrentId());
        }

        void ContactGroup::Remove() const {
            ContactGroup::Remove("id = " + Util::StringOp::FromInt(id()));
        }

        void ContactGroup::Remove(std::string filter) {
            DataAccess<ContactGroup>::RemoveDatasByFilter(filter);
        }

        void ContactGroup::modifyFieldByDB_(int argc, char** argv, char** columnName, Util::shared_ptr<ContactGroup> item) {
            item->id(atoi(argv[Data::getIndexByName(argc, columnName, "id")]));
            item->name_ = argv[Data::getIndexByName(argc, columnName, "name")];
            item->alias_ = argv[Data::getIndexByName(argc, columnName, "alias")];
            item->useDefaultRing_ = !!atoi(argv[Data::getIndexByName(argc, columnName, "useDefaultRing")]);
            item->ringId_ = atoi(argv[Data::getIndexByName(argc, columnName, "ringId")]);
            //item->useDefaultSMRing_ = !!atoi(argv[Data::getIndexByName(argc, columnName, "useDefaultSMRing")]);
            //item->smRingId_ = atoi(argv[Data::getIndexByName(argc, columnName, "smRingId")]);
            item->useDefaultTips_ = !!atoi(argv[Data::getIndexByName(argc, columnName, "useDefaultTips")]);
            item->tipsId_ = atoi(argv[Data::getIndexByName(argc, columnName, "tipsId")]);
            item->iconId_ = atoi(argv[Data::getIndexByName(argc, columnName, "iconId")]);
            //item->parentId_ = atoi(argv[Data::getIndexByName(argc, columnName, "parentId")]);
            //item->isLeaf_ = !!atoi(argv[Data::getIndexByName(argc, columnName, "isLeaf")]);
            //item->isEnableAppendSubGroup_ = !!atoi(argv[Data::getIndexByName(argc, columnName, "isEnableAppendSubGroup")]);
            item->ringName_ = argv[Data::getIndexByName(argc, columnName, "ringName")];
            item->tipName_ = argv[Data::getIndexByName(argc, columnName, "tipName")];
        }
    }
}
