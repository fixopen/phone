#ifndef __DATA_CONTACTINFO_H__
#define __DATA_CONTACTINFO_H__

//#pragma warning(disable: 4786)
#include <string>
#include <vector>
#include  <map>

#include "../../Contact/Data/SoundSegment.h"
#include "../../Contact/Data/Picture.h"
#include "../../Contact/Data/TelephoneNumber.h"
#include "../../Contact/Data/Contact.h"

#include "../../Util/Data.h"
#include "../../Util/DateTime.h"
#include "../../Util/SmartPtr.h"

namespace Telephone{
	namespace Data {
		enum CallInfoType {
			citInNo,
			citInOk,
			citOut
		};

		//class Contact;
		//class TelephoneNumber ; // add by qi 2009_05_04
		class CallInfo : public Util::DataAccess<CallInfo> {
		public:
			CallInfoType const type() const
			{
				return type_;
			}
			void type(CallInfoType const type)
			{
				type_ = type;
			}
			bool const isSound() const
			{
				return isSound_;
			}
			void isSound(bool const isSound)
			{
				isSound_ = isSound;
			}
			Util::Timestamp const startTime() const
			{
				return startTime_;
			}
			void startTime(Util::Timestamp const startTime)
			{
				startTime_ = startTime;
			}
			int const duration() const
			{
				return duration_;
			}
			void duration(int const duration)
			{
				duration_ = duration;
			}
			Contact::Data::TelephoneNumber const telephoneNumber() const
			{
				return telephoneNumber_;
			}
			void telephoneNumber(Contact::Data::TelephoneNumber const telephoneNumber)
			{
				telephoneNumber_ = telephoneNumber;
			}
			int const contactId() const
			{
				return contactId_;
			}
			void contactId(int const contactId)
			{
				contactId_ = contactId;
			}
			std::wstring name() const
			{
				return name_;
			}
			void name(std::wstring const name)
			{
				name_ = name;
			}

			bool played() const
			{
				return played_;
			}

			void played(bool const played)
			{
				played_ = played;
			}

			Util::shared_ptr<Contact::Data::Contact> const contact();
			//void contact(Util::shared_ptr<Contact> const contact);
			std::map<Util::Timestamp, Util::shared_ptr<Contact::Data::SoundSegment> > sounds();
			//std::vector<Util::shared_ptr<Picture> > notes();
		private:
			//int id_;
			CallInfoType type_;
			bool isSound_;
			Util::Timestamp startTime_;
			//Util::TimeSpan duration_;
			int duration_;
			//class
			//ContactInfoClass* class_;
			//int classId_;
			//contact
			Contact::Data::TelephoneNumber telephoneNumber_;
			bool contactFinded_;
			Util::shared_ptr<Contact::Data::Contact> contact_;
			int contactId_;
			std::wstring name_;
			bool played_;
			//sounds
			bool soundsFinded_;
			std::map<Util::Timestamp, Util::shared_ptr<Contact::Data::SoundSegment> > sounds_;
			//notes
			//bool notesFinded_;
			//std::vector<Util::shared_ptr<Picture> > notes_;
		public:
			CallInfo(); //create
			//~CallInfo(); //destroy
			static std::vector<Util::shared_ptr<CallInfo> > Select(std::wstring const& filter, std::wstring const& orderFieldName = L"id", Util::BaseData::SeqenceRelation const dir = srNull, int const offset = 0, int const pageSize = -1);
			void Update() const; //sync to database
			void Insert(); //insert new instance to database
			void Remove() const; //delete self from database
			static void Remove(std::wstring const& filter); //delete from database
		private:
			static void modifyFieldByDB_(int argc, char** argv, char** columnName, Util::shared_ptr<CallInfo> item);
		};

		class CallInfoSoundSegmentMap : public Util::DataAccess<CallInfoSoundSegmentMap> {
		public:
			int callInfoId() const
			{
				return callInfoId_;
			}
			void callInfoId(int callInfoId)
			{
				callInfoId_ = callInfoId;
			}
			int soundSegmentId() const
			{
				return soundSegmentId_;
			}
			void soundSegmentId(int soundSegmentId)
			{
				soundSegmentId_ = soundSegmentId;
			}
			Util::Timestamp const startTime() const
			{
				return startTime_;
			}
			void startTime(Util::Timestamp const startTime)
			{
				startTime_ = startTime;
			}
		private:
			//int id_;
			int callInfoId_;
			int soundSegmentId_;
			Util::Timestamp startTime_;
			/*
			//meta info
			Util::Timestamp creationTime_;
			Util::Timestamp lastReadTime_;
			Util::Timestamp lastWriteTime_;
			std::vector<Util::Timestamp> accessTimes_;
			Util::Timestamp destroyTime_;
			*/
		public:
			CallInfoSoundSegmentMap(); //create
			//~CallInfoSoundSegmentMap(); //destroy
			static std::vector<Util::shared_ptr<CallInfoSoundSegmentMap> > Select(std::wstring const& filter, std::wstring const& orderFieldName = L"id", Util::BaseData::SeqenceRelation const dir = srNull, int const offset = 0, int const pageSize = -1);
			void Update() const; //sync to database
			void Insert(); //insert new instance to database
			void Remove() const; //delete self from database
			static void Remove(std::wstring filter); //delete from database
		private:
			static void modifyFieldByDB_(int argc, char** argv, char** columnName, Util::shared_ptr<CallInfoSoundSegmentMap> item);
		};

		class CallInfoPictureMap : public Util::DataAccess<CallInfoPictureMap> {
		public:
			int callInfoId() const
			{
				return callInfoId_;
			}
			void callInfoId(int callInfoId)
			{
				callInfoId_ = callInfoId;
			}
			int pictureId() const
			{
				return pictureId_;
			}
			void pictureId(int pictureId)
			{
				pictureId_ = pictureId;
			}
		private:
			//int id_;
			int callInfoId_;
			int pictureId_;
			/*
			//meta info
			Util::Timestamp creationTime_;
			Util::Timestamp lastReadTime_;
			Util::Timestamp lastWriteTime_;
			std::vector<Util::Timestamp> accessTimes_;
			Util::Timestamp destroyTime_;
			*/
		public:
			CallInfoPictureMap(); //create
			//~CallInfoPictureMap(); //destroy
			static std::vector<Util::shared_ptr<CallInfoPictureMap> > Select(std::wstring const& filter, std::wstring const& orderFieldName = L"id", Util::BaseData::SeqenceRelation const dir = srNull, int const offset = 0, int const pageSize = -1);
			void Update() const; //sync to database
			void Insert(); //insert new instance to database
			void Remove() const; //delete self from database
			static void Remove(std::wstring filter); //delete from database
		private:
			static void modifyFieldByDB_(int argc, char** argv, char** columnName, Util::shared_ptr<CallInfoPictureMap> item);
		};
	}
}

#endif //__DATA_CONTACTINFO_H__
