#ifndef __DATA_CONTACTGROUP_H__
#define __DATA_CONTACTGROUP_H__

#pragma warning(disable: 4786)
#include <string>
#include <vector>

#include "Data.h"
#include "SoundSegment.h"
#include "Picture.h"

//#include "../Util/DateTime.h"
#include "../Util/SmartPtr.h"

namespace Data
{
	class Contact;
    class ContactGroup : public DataAccess<ContactGroup>
    {
    public:
        std::string const name(void) const
        {
            return name_;
        }
        void name(std::string const n)
        {
            name_ = n;
        }
        std::string const alias(void) const
        {
            return alias_;
        }
        void alias(std::string const alias)
        {
            alias_ = alias;
        }

		std::string ringName(void) const
		{
			return ringName_;
		}
		
		void ringName(std::string const ringName)
		{
			ringName_ = ringName;
		}

		std::string tipName(void) const
		{
			return tipName_;
		}
		
		void tipName(std::string const tipName)
		{
			tipName_ = tipName;
		}


        boost::shared_ptr<SoundSegment> ring(void);
        void ring(boost::shared_ptr<SoundSegment> ring);
        bool const useDefaultRing(void) const
        {
            return useDefaultRing_;
        }
        void useDefaultRing(bool const useDefaultRing)
        {
            useDefaultRing_ = useDefaultRing;
        }
		boost::shared_ptr<SoundSegment> tips();
		void tips(boost::shared_ptr<SoundSegment> tips);
		int const tipsId() const
		{
			return tipsId_;
		}
		void tipsId(int const tipsId)
		{
			tipsId_ = tipsId;
		}

        std::vector<boost::shared_ptr<Contact> > const contacts(void);
    private:
        //int id_;
        std::string name_; //32
        std::string alias_;
        bool useDefaultRing_;
        bool ringFinded_;
        boost::shared_ptr<SoundSegment> ring_;
        int ringId_;
        //bool useDefaultSMRing_;
        //boost::shared_ptr<SoundSegment> smRing_;
        int smRingId_;

		std::string ringName_;
		std::string tipName_;

        bool useDefaultTips_;
        bool tipsFinded_;
        boost::shared_ptr<SoundSegment> tips_;
        int tipsId_;
        bool iconFinded_;
        boost::shared_ptr<Picture> icon_;
        int iconId_;
        //nest info
        //boost::shared_ptr<ContactGroup> parent_;
        //int parentId_;
        //bool isLeaf_;
        //bool isEnableAppendSubGroup_;
        //contacts
        bool contactsFinded_;
        std::vector<boost::shared_ptr<Contact> > contacts_;
        /*
        //meta info
        Util::Timestamp creationTime_;
        Util::Timestamp lastReadTime_;
        Util::Timestamp lastWriteTime_;
        std::vector<Util::Timestamp> accessTimes_;
        Util::Timestamp destroyTime_;
        */
    public:
        ContactGroup(); //create
        ~ContactGroup(); //destroy
        static std::vector<boost::shared_ptr<ContactGroup> > GetFromDatabase(std::string filter, Direction const dir = dNull, int const id = 0, int const pageSize = 0); //get from database
        void Update() const; //sync to database
        void Insert(); //insert new instance to database
        void Remove() const; //remove self from database
		static void Closedb();
		static void Opendb();
        static void Remove(std::string filter); //delete from database
    private:
        static void modifyFieldByDB_(int argc, char** argv, char** columnName, boost::shared_ptr<ContactGroup> item);
    };
}

#endif //__DATA_CONTACTGROUP_H__
