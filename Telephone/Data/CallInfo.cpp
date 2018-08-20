#include "CallInfo.h"
#include "../../Contact/Data/SoundSegment.h"
#include "../../Util/StoragePath.h"
#include "../../Util/Audio/WaveRecorder.h"

namespace Util {
    std::wstring DataAccess<Telephone::Data::CallInfo>::tableName_ = L"callInfo";
    int DataAccess<Telephone::Data::CallInfo>::count_ = 0;
    int DataAccess<Telephone::Data::CallInfo>::offset_ = 0;
}

namespace Telephone {
    namespace Data {
        int const BytesOfSecond8 = 2048;
        int const BytesOfSecond2 = 850;

        std::wstring const CallInfoTypeToString(CallInfoType const type) {
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

        CallInfoType const StringToCallInfoType(std::wstring const& string) {
            CallInfoType result;
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

        std::vector<Util::shared_ptr<Record> > const CallInfo::sounds() const {
            if (hasSound_) {
                if (!soundsFinded_) {
                    sounds_ = Contact::Data::SoundSegment::Select(L"[contactInfoId] = " + Util::Text::StringOp::FromInt(id()));
                    soundsFinded_ = true;
                }
            }
            return sounds_;
        }

        void CallInfo::appendSound(Util::shared_ptr<::Telephone::Data::Record>& soundSegment) {
            soundSegment->callInfo(this);
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
        , soundsFinded_(false)
        , isInserted_(false) {
        }

        std::vector<Util::shared_ptr<CallInfo> > CallInfo::Select(std::wstring const& filter, std::wstring const& orderFieldName, SeqenceRelation const dir, int const offset, int const pageSize) {
            return GetDatasByFilter(filter, modifyFieldByDB_, orderFieldName, dir, offset, pageSize);
        }

        void CallInfo::Update() const {
            std::wstring cmd = L"UPDATE ";
            cmd += tableName_;
            cmd += L" SET [type] = ";
            cmd += Util::Text::StringOp::FromInt(type_);
            cmd += L", [hasSound] = ";
            cmd += Util::Text::StringOp::FromInt(hasSound_);
            cmd += L", [startTime] = '";
            cmd += Util::Text::StringOp::FromTimestamp(startTime_);
            cmd += L"', [duration] = '";
            cmd += Util::Text::StringOp::FromTimeSpan(duration_);
            cmd += L"', [telephoneNumber] = '";
            cmd += telephoneNumber_;
            cmd += L"', [contactId] = ";
            cmd += Util::Text::StringOp::FromInt(contactId_);
            cmd += L" WHERE [id] = ";
            cmd += Util::Text::StringOp::FromInt(id());
            ExecCommand(cmd);
        }

        void CallInfo::Insert() {
            if (isInserted_) {
                Update();
            } else {
                std::wstring cmd = L"INSERT INTO ";
                cmd += tableName_;
                cmd += L" ( [type], [hasSound], [startTime], [duration], [telephoneNumber], [contactId] ) VALUES ( ";
                cmd += Util::Text::StringOp::FromInt(type_);
                cmd += L", ";
                cmd += Util::Text::StringOp::FromInt(hasSound_);
                cmd += L", '";
                cmd += Util::Text::StringOp::FromTimestamp(startTime_);
                cmd += L"', '";
                cmd += Util::Text::StringOp::FromTimeSpan(duration_);
                cmd += L"', '";
                cmd += telephoneNumber_;
                cmd += L"', ";
                cmd += Util::Text::StringOp::FromInt(contactId_);
                cmd += L" )";
                ExecCommand(cmd);
                id(GetCurrentId());
            }
        }

        void CallInfo::Remove() const {
            Remove(L"id = " + Util::Text::StringOp::FromInt(id()));
        }

        void CallInfo::Remove(std::wstring const& filter) {
            RemoveDatasByFilter(filter);
        }

        std::vector<Util::shared_ptr<CallInfo> > CallInfo::GetAllNotPlayedLeaveWord() {
            std::vector<Util::shared_ptr<CallInfo> > result;
            std::vector<Util::shared_ptr<CallInfo> > allLeaveWords = Select(L"[type] = " + Util::Text::StringOp::FromInt(citLeaveWord));
            for (size_t i = 0; i < allLeaveWords.size(); ++i) {
                bool isPlayed = true;
                std::vector<Util::shared_ptr<Contact::Data::SoundSegment> > sounds = allLeaveWords[i]->sounds();
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

        Util::shared_ptr<::Telephone::Data::Record> const CallInfo::Record() {
            Insert();
            isInserted_ = true;
            Util::shared_ptr<::Telephone::Data::Record> result(new ::Telephone::Data::Record());
            appendSound(result);
            Util::Timestamp now = Util::Timestamp::GetCurrentTime();
            result->startTime(now);
            result->isPlayed(false);
            result->telephoneNumber(this->telephoneNumber());
            std::wstring path;
            int secondBytes = BytesOfSecond2;
            if (true/*setting->system->isDeleteProtect()*/) {
                secondBytes = BytesOfSecond8;
            }

            if (Util::StoragePath::HasSdCard()) {
                path = Util::StoragePath::GetSdCardPath() + L"/" + Util::StoragePath::GetCallRecordPath();
                if (Util::StoragePath::HasFolder(path)) {
                    CreateDirectory(path.c_str(), NULL);
                }
                ULARGE_INTEGER freeBytes;
                ULARGE_INTEGER totalBytes;
                GetDiskFreeSpaceEx(path.c_str(), &freeBytes, &totalBytes, NULL);
                if (freeBytes.QuadPart < secondBytes * 2) {
                    //warning and return
                }
                Contact::Data::SoundSegment::remainSecondCount = static_cast<int>(freeBytes.QuadPart / secondBytes + 1);
            } else {
                path = Util::StoragePath::GetFlashDrvPath() + L"/" + Util::StoragePath::GetCallRecordPath();
                if (Util::StoragePath::HasFolder(path)) {
                    CreateDirectory(path.c_str(), NULL);
                }
                ULARGE_INTEGER freeBytes;
                ULARGE_INTEGER totalBytes;
                GetDiskFreeSpaceEx(path.c_str(), &freeBytes, &totalBytes, NULL);
                if (freeBytes.QuadPart < totalBytes.QuadPart * 0.1) {
                    //warning and return
                }
                Contact::Data::SoundSegment::remainSecondCount = static_cast<int>((freeBytes.QuadPart - (totalBytes.QuadPart * 0.1)) / secondBytes + 1);
            }
            switch (type_) {
        case citMissed:
            result->type(Contact::Data::sstLeaveWord);
            break;
        case citDialed:
        case citReceived:
            result->type(Contact::Data::sstCallRecord);
            break;
        default:
            //result->type(Contact::Data::sstLocalRecord);
            //filename = Contact::Data::SoundSegment::ringtoneFolderName();
            //result->type(sstPrompt);
            //filename = Contact::Data::SoundSegment::promptFolderName();
            break;
            }
            result->filename(path + L"/" + Util::Audio::WaveRecorder::Instance()->GetFilename(now, true));
            if (Util::Audio::WaveRecorder::Instance()->IsRecording()) {
                Util::Audio::WaveRecorder::Instance()->Stop();
            }
            Util::Audio::WaveRecorder::Instance()->Start(result->filename());
            //result->Insert();
            return result;
        }

        void CallInfo::StopRecord() {
            Util::Audio::WaveRecorder::Instance()->Stop();
            sounds_[sounds_.size() - 1]->duration(Util::Timestamp::GetCurrentTime() - sounds_[sounds_.size() - 1]->startTime());
            sounds_[sounds_.size() - 1]->Insert();
        }

        void CallInfo::modifyFieldByDB_(int argc, char** argv, char** columnName, Util::shared_ptr<CallInfo>& item) {
            item->id(atoi(argv[GetIndexByName(argc, columnName, "id")]));
            item->type_ = static_cast<CallInfoType>(atoi(argv[GetIndexByName(argc, columnName, "type")]));
            item->hasSound_ = !!atoi(argv[GetIndexByName(argc, columnName, "hasSound")]);
            item->startTime_ = Util::Text::StringOp::ToTimestamp(std::string(argv[GetIndexByName(argc, columnName, "startTime")]));
            item->duration_ = Util::Text::StringOp::ToTimeSpan(argv[GetIndexByName(argc, columnName, "duration")]);
            item->telephoneNumber_ = Util::Text::StringOp::FromUTF8(argv[GetIndexByName(argc, columnName, "telephoneNumber")]);
            item->contactId_ = atoi(argv[GetIndexByName(argc, columnName, "contactId")]);
        }
    }
}
