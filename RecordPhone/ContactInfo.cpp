#include "ContactInfo.h"
#include "SoundSegment.h"
#include "StoragePath.h"
#include "WaveRecorder.h"

int const BytesOfSecond8 = 2048;
int const BytesOfSecond2 = 850;

namespace Util {
    std::wstring DataAccess<CallInfo>::tableName_ = L"contactInfo";
    int DataAccess<CallInfo>::count_ = 0;
    int DataAccess<CallInfo>::offset_ = 0;
}

std::wstring const ContactInfoTypeToString(ContactInfoType const type) {
    std::wstring result;
    switch (type) {
        case citMissed:
            result = L"Missed";
            break;
        case citReceived:
            result = L"Received";
            break;
        case citDialed:
            result = L"Dialed";
            break;
        case citLeaveWord:
            result = L"Leave Word";
            break;
        default:
            break;
    }
    return result;
}

ContactInfoType const StringToContactInfoType(std::wstring const& string) {
    ContactInfoType result;
    if (string == L"Missed") {
        result = citMissed;
    } else if (string == L"Received") {
        result = citReceived;
    } else if (string == L"Dialed") {
        result = citDialed;
    } else if (string == L"Leave Word") {
        result = citLeaveWord;
    } else {
    }
    return result;
}

std::vector<Util::shared_ptr<SoundSegment> > const CallInfo::sounds() const {
    if (hasSound_) {
        if (!soundsFinded_) {
            sounds_ = SoundSegment::Select(L"[contactInfoId] = " + Util::StringOp::FromInt(id()));
            soundsFinded_ = true;
        }
    }
    return sounds_;
}

void CallInfo::appendSound(Util::shared_ptr<SoundSegment>& soundSegment) {
    soundSegment->contactInfo(this);
    sounds_.push_back(soundSegment);
    soundsFinded_ = true;
    hasSound_ = true;
}

CallInfo::CallInfo()
: DataAccess<CallInfo>()
, type_(citMissed)
, contactId_(0)
, contactFinded_(false)
, contact_(0)
, hasSound_(false)
, soundsFinded_(false) {
}

std::vector<Util::shared_ptr<CallInfo> > CallInfo::Select(std::wstring const& filter, std::wstring const& orderFieldName, SeqenceRelation const dir, int const offset, int const pageSize) {
    return GetDatasByFilter(filter, modifyFieldByDB_, orderFieldName, dir, offset, pageSize);
}

void CallInfo::Update() const {
    std::wstring cmd = L"UPDATE ";
    cmd += tableName_;
    cmd += L" SET [type] = ";
    cmd += Util::StringOp::FromInt(type_);
    cmd += L", [hasSound] = ";
    cmd += Util::StringOp::FromInt(hasSound_);
    cmd += L", [startTime] = '";
    cmd += Util::StringOp::FromTimestamp(startTime_);
    cmd += L"', [duration] = '";
    cmd += Util::StringOp::FromTimeSpan(duration_);
    cmd += L"', [telephoneNumber] = '";
    cmd += telephoneNumber_;
    cmd += L"', [contactId] = ";
    cmd += Util::StringOp::FromInt(contactId_);
    cmd += L" WHERE [id] = ";
    cmd += Util::StringOp::FromInt(id());
    ExecCommand(cmd);
}

void CallInfo::Insert() {
    std::wstring cmd = L"INSERT INTO ";
    cmd += tableName_;
    cmd += L" ( [type], [hasSound], [startTime], [duration], [telephoneNumber], [contactId] ) VALUES ( ";
    cmd += Util::StringOp::FromInt(type_);
    cmd += L", ";
    cmd += Util::StringOp::FromInt(hasSound_);
    cmd += L", '";
    cmd += Util::StringOp::FromTimestamp(startTime_);
    cmd += L"', '";
    cmd += Util::StringOp::FromTimeSpan(duration_);
    cmd += L"', '";
    cmd += telephoneNumber_;
    cmd += L"', ";
    cmd += Util::StringOp::FromInt(contactId_);
    cmd += L" )";
    ExecCommand(cmd);
    id(GetCurrentId());
}

void CallInfo::Remove() const {
    Remove(L"id = " + Util::StringOp::FromInt(id()));
}

void CallInfo::Remove(std::wstring const& filter) {
    RemoveDatasByFilter(filter);
}

std::vector<Util::shared_ptr<CallInfo> > CallInfo::GetAllNotPlayedLeaveWord() {
    std::vector<Util::shared_ptr<CallInfo> > result;
    std::vector<Util::shared_ptr<CallInfo> > allLeaveWords = Select(L"[type] = " + Util::StringOp::FromInt(citLeaveWord));
    for (size_t i = 0; i < allLeaveWords.size(); ++i) {
        bool isPlayed = true;
        std::vector<Util::shared_ptr<SoundSegment> > sounds = allLeaveWords[i]->sounds();
        if (!sounds.empty()) {
            for (size_t j = 0; j < sounds.size(); ++j) {
                if (!sounds[j]->isPlayed()) {
                    isPlayed = false;
                    break;
                }
            }
        }
        if (!isPlayed) {
            result.push_back(allLeaveWords[i]);
        }
    }
    return result;
}

Util::shared_ptr<SoundSegment> const CallInfo::Record() {
    Util::shared_ptr<SoundSegment> result(new SoundSegment());
    appendSound(result);
    Util::Timestamp now = Util::Timestamp::GetCurrentTime();
    result->startTime(now);
    result->isPlayed(false);
    std::wstring path;
	int secondBytes = BytesOfSecond2;
	if (true/*setting->system->isDeleteProtect()*/) {
		secondBytes = BytesOfSecond8;
	}

    if (StoragePath::HasSdCard()) {
        path = StoragePath::GetSdCardPath() + L"/" + StoragePath::GetCallRecordPath();
        if (StoragePath::HasFolder(path)) {
            CreateDirectory(path.c_str(), NULL);
        }
        ULARGE_INTEGER freeBytes;
        ULARGE_INTEGER totalBytes;
        GetDiskFreeSpaceEx(path.c_str(), &freeBytes, &totalBytes, NULL);
        if (freeBytes.QuadPart < secondBytes * 2) {
            //warning and return
        }
        SoundSegment::remainSecondCount = freeBytes.QuadPart / secondBytes + 1;
    } else {
        path = StoragePath::GetFlashDrvPath() + L"/" + StoragePath::GetCallRecordPath();
        if (StoragePath::HasFolder(path)) {
            CreateDirectory(path.c_str(), NULL);
        }
        ULARGE_INTEGER freeBytes;
        ULARGE_INTEGER totalBytes;
        GetDiskFreeSpaceEx(path.c_str(), &freeBytes, &totalBytes, NULL);
        if (freeBytes.QuadPart < totalBytes.QuadPart * 0.1) {
            //warning and return
        }
        SoundSegment::remainSecondCount = (freeBytes.QuadPart - (totalBytes.QuadPart * 0.1)) / secondBytes + 1;
    }
    switch (type_) {
        case citMissed:
            result->type(sstLeaveWord);
            break;
        case citDialed:
        case citReceived:
            result->type(sstCallRecord);
            break;
        default:
            //result->type(sstLocalRecord);
            //filename = SoundSegment::ringtoneFolderName();
            //result->type(sstPrompt);
            //filename = SoundSegment::promptFolderName();
            break;
    }
    result->filename(path + L"/" + WaveRecorder::Instance()->GetFilename(now, true));
    if (WaveRecorder::Instance()->IsRecording()) {
        WaveRecorder::Instance()->Stop();
    }
    WaveRecorder::Instance()->Start(result);
    //result->Insert();
    return result;
}

void CallInfo::StopRecord() {
    WaveRecorder::Instance()->Stop();
}

void CallInfo::modifyFieldByDB_(int argc, char** argv, char** columnName, Util::shared_ptr<CallInfo>& item) {
    item->id(atoi(argv[GetIndexByName(argc, columnName, "id")]));
    item->type_ = static_cast<ContactInfoType>(atoi(argv[GetIndexByName(argc, columnName, "type")]));
    item->hasSound_ = !!atoi(argv[GetIndexByName(argc, columnName, "hasSound")]);
    item->startTime_ = Util::StringOp::ToTimestamp(std::string(argv[GetIndexByName(argc, columnName, "startTime")]));
    item->duration_ = Util::StringOp::ToTimeSpan(argv[GetIndexByName(argc, columnName, "duration")]);
    item->telephoneNumber_ = Util::StringOp::FromUTF8(argv[GetIndexByName(argc, columnName, "telephoneNumber")]);
    item->contactId_ = atoi(argv[GetIndexByName(argc, columnName, "contactId")]);
}
