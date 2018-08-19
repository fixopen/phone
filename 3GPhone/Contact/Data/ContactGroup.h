#ifndef __DATA_CONTACTGROUP_H__
#define __DATA_CONTACTGROUP_H__

#include <string>
#include <vector>

#include "SoundSegment.h"
#include "Picture.h"

#include "../../Util/Data.h"
//#include "../../Util/DateTime.h"
#include "../../Util/SmartPtr.h"
namespace Contact_{
	namespace Data {
		class Contact;
		class ContactGroup : public Util::DataAccess<ContactGroup> {
		public:
			std::wstring const name(void) const
			{
				return name_;
			}
			void name(std::wstring const n)
			{
				name_ = n;
			}
			std::wstring const alias(void) const
			{
				return alias_;
			}
			void alias(std::wstring const alias)
			{
				alias_ = alias;
			}

			std::wstring ringName(void) const
			{
				return ringName_;
			}
			
			void ringName(std::wstring const ringName)
			{
				ringName_ = ringName;
			}

			std::wstring const tipName() const
			{
				return tipName_;
			}
			
			void tipName(std::wstring const& tipName)
			{
				tipName_ = tipName;
			}


			Util::shared_ptr<SoundSegment> ring();
			void ring(Util::shared_ptr<SoundSegment> ring);
			bool const useDefaultRing() const
			{
				return useDefaultRing_;
			}
			void useDefaultRing(bool const useDefaultRing)
			{
				useDefaultRing_ = useDefaultRing;
			}
			Util::shared_ptr<SoundSegment> tips();
			void tips(Util::shared_ptr<SoundSegment> tips);
			int const tipsId() const
			{
				return tipsId_;
			}
			void tipsId(int const tipsId)
			{
				tipsId_ = tipsId;
			}

			std::vector<Util::shared_ptr<Contact> > const contacts(void);
		private:
			//int id_;
			std::wstring name_; //32
			std::wstring alias_;
			bool useDefaultRing_;
			bool ringFinded_;
			Util::shared_ptr<SoundSegment> ring_;
			int ringId_;
			//bool useDefaultSMRing_;
			//Util::shared_ptr<SoundSegment> smRing_;
			int smRingId_;

			std::wstring ringName_;
			std::wstring tipName_;

			bool useDefaultTips_;
			bool tipsFinded_;
			Util::shared_ptr<SoundSegment> tips_;
			int tipsId_;
			bool iconFinded_;
			Util::shared_ptr<Picture> icon_;
			int iconId_;
			//nest info
			//Util::shared_ptr<ContactGroup> parent_;
			//int parentId_;
			//bool isLeaf_;
			//bool isEnableAppendSubGroup_;
			//contacts
			bool contactsFinded_;
			std::vector<Util::shared_ptr<Contact> > contacts_;
		public:
			ContactGroup(); //create
			//~ContactGroup(); //destroy
			static std::vector<Util::shared_ptr<ContactGroup> > Select(std::wstring const& filter, std::wstring const& orderFieldName = L"id", SeqenceRelation const dir = srNull, int const id = 0, int const pageSize = -1);
			void Update() const; //sync to database
			void Insert(); //insert new instance to database
			void Remove() const; //remove self from database
			static void Remove(std::wstring filter); //delete from database
		private:
			static void modifyFieldByDB_(int argc, char** argv, char** columnName, Util::shared_ptr<ContactGroup> item);
		};
	}
}

#endif //__DATA_CONTACTGROUP_H__
