#include "SoundSegment.h"
#include "../../Util/Audio/DSoundPlayer.h"
#include "../../Util/Audio/WaveOut.h"
#include "../../Util/Properties.h"

namespace Util {
    std::wstring DataAccess<Contact::Data::SoundSegment>::tableName_ = L"soundSegment";
    int DataAccess<Contact::Data::SoundSegment>::count_ = 0;
    int DataAccess<Contact::Data::SoundSegment>::offset_ = 0;
}

namespace Contact {
    namespace Data {
        int SoundSegment::remainSecondCount = 0;

        std::wstring SoundSegment::folderName_;
        std::wstring SoundSegment::ringtoneFolderName_;
        std::wstring SoundSegment::promptFolderName_;

        SoundSegment SoundSegment::DefaultRingtone;
        SoundSegment SoundSegment::VipRingtone;
        SoundSegment SoundSegment::BlacklistRingtone;
        SoundSegment SoundSegment::DefaultPrompt;
        SoundSegment SoundSegment::VipPrompt;
        SoundSegment SoundSegment::BlacklistPrompt;

        void SoundSegment::Init() {
            folderName_ = Util::StoragePath::GetCallRecordPath(); //Util::Properties::Get(L"recordFolderName");

            ringtoneFolderName_ = Util::StoragePath::GetRingtonePath(); //Util::Properties::Get(L"ringtoneFolderName");

            promptFolderName_ = Util::StoragePath::GetPromptPath(); //Util::Properties::Get(L"promptFolderName");

            DefaultRingtone = SoundSegment(sstRingtone, Util::Properties::Get(L"defaultRingtoneFilename"));

            VipRingtone = SoundSegment(sstRingtone, Util::Properties::Get(L"vipRingtoneFilename"));

            BlacklistRingtone = SoundSegment(sstRingtone, Util::Properties::Get(L"blacklistRingtoneFilename"));

            DefaultPrompt = SoundSegment(sstPrompt, Util::Properties::Get(L"defaultPromptFilename"));

            VipPrompt = SoundSegment(sstPrompt, Util::Properties::Get(L"vipPromptFilename"));

            BlacklistPrompt = SoundSegment(sstPrompt, Util::Properties::Get(L"blacklistPromptFilename"));
        }

        SoundSegment::SoundSegment()
        : DataAccess<SoundSegment>()
        , type_(sstCallRecord)
        , callInfoId_(0)
        , callInfo_(0)
        , contact_(0)
        , isPlayed_(false)
        , StopNotify(0) {
        }

        SoundSegment::SoundSegment(SoundSegmentType const type, std::wstring const & filename)
        : DataAccess<SoundSegment>()
        , type_(type)
        , callInfoId_(0)
        , callInfo_(0)
        , contact_(0)
        , isPlayed_(false)
        , filename_(filename)
        , StopNotify(0) {
        }

        std::vector<Util::shared_ptr<SoundSegment> > SoundSegment::Select(std::wstring const & filter, std::wstring const & orderFieldName, SeqenceRelation const dir, int const offset, int const pageSize) {
            return GetDatasByFilter(filter, modifyFieldByDB_, orderFieldName, dir, offset, pageSize);
        }

        void SoundSegment::Update() const {
            std::wstring cmd = L"UPDATE ";
            cmd += tableName_;
            cmd += L" SET [filename] = '";
            cmd += filename_;
            cmd += L"', [duration] = '";
            cmd += Util::Text::StringOp::FromTimeSpan(duration_);
            cmd += L"', [type] = ";
            cmd += Util::Text::StringOp::FromInt(type_);
            cmd += L", [startTime] = '";
            cmd += Util::Text::StringOp::FromTimestamp(startTime_);
            cmd += L"', [telephoneNumber] = '";
            cmd += telephoneNumber_;
            cmd += L"', [contactInfoId] = ";
            cmd += Util::Text::StringOp::FromInt(callInfoId_);
            cmd += L", [isPlayed] = ";
            cmd += Util::Text::StringOp::FromInt(isPlayed_);
            cmd += L" WHERE [id] = ";
            cmd += Util::Text::StringOp::FromInt(id());
            ExecCommand(cmd);
        }

        void SoundSegment::Insert() {
            std::wstring cmd = L"INSERT INTO ";
            cmd += tableName_;
            cmd += L" ( [filename], [duration], [type], [startTime], [telephoneNumber], [contactInfoId], [isPlayed] ) VALUES ( '";
            cmd += filename_;
            cmd += L"', '";
            cmd += Util::Text::StringOp::FromTimeSpan(duration_);
            cmd += L"', ";
            cmd += Util::Text::StringOp::FromInt(type_);
            cmd += L", '";
            cmd += Util::Text::StringOp::FromTimestamp(startTime_);
            cmd += L"', '";
            cmd += telephoneNumber_;
            cmd += L"', ";
            cmd += Util::Text::StringOp::FromInt(callInfoId_);
            cmd += L", ";
            cmd += Util::Text::StringOp::FromInt(isPlayed_);
            cmd += L" )";
            ExecCommand(cmd);
            id(GetCurrentId());
        }

        void SoundSegment::Remove() const {
            Remove(L"id = " + Util::Text::StringOp::FromInt(id()));
        }

        void SoundSegment::Remove(std::wstring const & filter) {
            RemoveDatasByFilter(filter);
        }

        void SoundSegment::Play() {
            switch (type_) {
            case sstLeaveWord:
            case sstCallRecord:
            case sstLocalRecord:
                //filename = folderName_ + L"/" + filename_;
                //tcpmp player
                Util::Audio::WaveOut::Instance()->SetStopNotify(StopNotify);
                Util::Audio::WaveOut::Instance()->Start(filename());
                break;
            case sstRingtone:
                Util::Audio::DSoundPlayer::Instance()->SetStopNotify(StopNotify);
                Util::Audio::DSoundPlayer::Instance()->Start(Util::StoragePath::GetFlashDrvPath() + L"/" + ringtoneFolderName_ + L"/" + filename_);
                break;
            case sstPrompt:
                Util::Audio::DSoundPlayer::Instance()->SetStopNotify(StopNotify);
                Util::Audio::DSoundPlayer::Instance()->Start(Util::StoragePath::GetFlashDrvPath() + L"/" + promptFolderName_ + L"/" + filename_, 1);
                break;
            default:
                break;
            }
        }

        void SoundSegment::PausePlay() {
            switch (type_) {
            case sstLeaveWord:
            case sstCallRecord:
            case sstLocalRecord:
                //filename = folderName_ + L"/" + filename_;
                //tcpmp player
                break;
            case sstRingtone:
            case sstPrompt:
                Util::Audio::DSoundPlayer::Instance()->Pause();
                break;
            default:
                break;
            }
        }

        void SoundSegment::ResumePlay() {
            switch (type_) {
            case sstLeaveWord:
            case sstCallRecord:
            case sstLocalRecord:
                //filename = folderName_ + L"/" + filename_;
                //tcpmp player
                break;
            case sstRingtone:
            case sstPrompt:
                Util::Audio::DSoundPlayer::Instance()->Resume();
                break;
            default:
                break;
            }
        }

        void SoundSegment::BreakPlay() {
            switch (type_) {
            case sstLeaveWord:
            case sstCallRecord:
            case sstLocalRecord:
                //filename = folderName_ + L"/" + filename_;
                //tcpmp player
                break;
            case sstRingtone:
            case sstPrompt:
                Util::Audio::DSoundPlayer::Instance()->Stop();
                break;
            default:
                break;
            }
        }

        void SoundSegment::modifyFieldByDB_(int argc, char** argv, char** columnName, Util::shared_ptr<SoundSegment>& item) {
            item->id(Util::Text::StringOp::ToInt(argv[GetIndexByName(argc, columnName, "id")]));
            item->filename_ = Util::Text::StringOp::FromUTF8(argv[GetIndexByName(argc, columnName, "filename")]);
            item->duration_ = Util::Text::StringOp::ToTimeSpan(argv[GetIndexByName(argc, columnName, "duration")]);
            item->type_ = static_cast<SoundSegmentType>(Util::Text::StringOp::ToInt(argv[GetIndexByName(argc, columnName, "type")]));
            item->startTime_ = Util::Text::StringOp::ToTimestamp(std::string(argv[GetIndexByName(argc, columnName, "startTime")]));
            item->telephoneNumber_ = Util::Text::StringOp::FromUTF8(argv[GetIndexByName(argc, columnName, "telephoneNumber")]);
            item->callInfoId_ = Util::Text::StringOp::ToInt(argv[GetIndexByName(argc, columnName, "contactInfoId")]);
            item->isPlayed_ = !!Util::Text::StringOp::ToInt(argv[GetIndexByName(argc, columnName, "isPlayed")]);
        }
    }
}
