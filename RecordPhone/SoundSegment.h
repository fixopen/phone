#pragma once
#include "ContactInfo.h"
#include "Contact.h"
#include "StoragePath.h"

enum SoundSegmentType {
    sstLeaveWord, //¡Ù—‘
    sstCallRecord, //Õ®ª∞¬º“Ù
    sstLocalRecord, //±æµÿ¬º“Ù
    sstPrompt, //Ã· æ”Ô
    sstRingtone, //’Ò¡Â“Ù
};

class SoundSegment : public Util::DataAccess<SoundSegment> {
public:
    std::wstring const filename() const {
        return filename_;
    }
    void filename(std::wstring const& filename) {
        filename_ = filename;
    }
    Util::TimeSpan const duration() const {
        return duration_;
    }
    void duration(Util::TimeSpan const& duration) {
        duration_ = duration;
    }
    SoundSegmentType const type() const {
        return type_;
    }
    void type(SoundSegmentType const type) {
        type_ = type;
    }
    static std::wstring const folderName() {
        return folderName_;
    }
    static void folderName(std::wstring const& folderName) {
        folderName_ = folderName;
    }
    static std::wstring const ringtoneFolderName() {
        return ringtoneFolderName_;
    }
    static void ringtoneFolderName(std::wstring const& folderName) {
        ringtoneFolderName_ = folderName;
    }
    static std::wstring const promptFolderName() {
        return promptFolderName_;
    }
    static void promptFolderName(std::wstring const& folderName) {
        promptFolderName_ = folderName;
    }
    TelephoneNumber const telephoneNumber() const {
        return telephoneNumber_;
    }
    void telephoneNumber(TelephoneNumber const& telephoneNumber) {
        telephoneNumber_ = telephoneNumber;
    }
    Util::Timestamp const startTime() const {
        return startTime_;
    }
    void startTime(Util::Timestamp const& startTime) {
        startTime_ = startTime;
    }
    void contactInfoId(int const contactInfoId) {
        contactInfoId_ = contactInfoId;
    }
    CallInfo const* const contactInfo() const {
        switch (type_) {
            case sstLeaveWord:
            case sstCallRecord:
                if (!contactInfoFinded_) {
		            std::vector<Util::shared_ptr<CallInfo> > t = CallInfo::Select(L"id = " + Util::StringOp::FromInt(contactInfoId_));
                    if (t.size() == 1) {
			            contactInfo_ = t[0].get();
                    }
                    contactInfoFinded_ = true;
                }
                break;
            default:
                break;
        }
        return contactInfo_;
    }
    void contactInfo(CallInfo const* const contactInfo) const {
        contactInfo_ = const_cast<CallInfo*>(contactInfo);
        contactInfoId_ = contactInfo->id();
        contactInfoFinded_ = true;
    }
    std::wstring name() const {
        if (!contactInfoFinded_) {
            contactInfo();
        }
        return contactInfo_->name();
    }
    bool const isPlayed() const {
        return isPlayed_;
    }
    void isPlayed(bool const isPlayed) {
        isPlayed_ = isPlayed;
    }
    bool isPrompt() const {
        return type_ == sstPrompt;
    }
    Contact const* const contact() const {
	    if (!contactFinded_) {
		    std::vector<Util::shared_ptr<Contact> > t = Contact::Select(L"id = " + Util::StringOp::FromInt(contactInfo_->contactId()));
            if (t.size() != 0) {
			    contact_ = t[0].get();
            }
            contactFinded_ = true;
	    }
	    return contact_;
    }
    static int remainSecondCount;
    std::wstring const fullPathname() const {
        return StoragePath::GetFlashDrvPath() + L"/" + StoragePath::GetCallRecordPath() + L"/" + filename_; //GetSdCardPath
    }
public:
    SoundSegment(); //create
    SoundSegment(SoundSegmentType const type, std::wstring const& filename);
    //~SoundSegment(); //destroy
    static std::vector<Util::shared_ptr<SoundSegment> > Select(std::wstring const& filter, std::wstring const& orderFieldName = L"id", SeqenceRelation const dir = srNull, int const offset = 0, int const pageSize = -1);
    void Update() const; //sync to database
    void Insert(); //insert new instance to database
    void Remove() const; //delete this from database
    static void Remove(std::wstring const& filter); //delete from database
    enum Status {
        sIdle,
        sPlaying,
        sPausePlaying,
        sBreakPlaying,
    };
    static void Init();
    void (*StopNotify)();
    void Play();
    void PausePlay();
    void ResumePlay();
    void BreakPlay();
private:
    static void modifyFieldByDB_(int argc, char** argv, char** columnName, Util::shared_ptr<SoundSegment>& item);
public:
    static SoundSegment DefaultRingtone;
    static SoundSegment VipRingtone;
    static SoundSegment BlacklistRingtone;
    static SoundSegment DefaultPrompt;
    static SoundSegment VipPrompt;
    static SoundSegment BlacklistPrompt;
private:
    static std::wstring folderName_;
    static std::wstring ringtoneFolderName_;
    static std::wstring promptFolderName_;
private:
    std::wstring filename_;
    SoundSegmentType type_;
    Util::Timestamp startTime_;
    Util::TimeSpan duration_;
    bool isPlayed_;
    //contactInfo
    mutable int contactInfoId_;
    mutable bool contactInfoFinded_;
    mutable CallInfo* contactInfo_;
    mutable TelephoneNumber telephoneNumber_;
    //contact
    mutable bool contactFinded_;
    mutable Contact* contact_;
    //play control
    Status state_;
};
