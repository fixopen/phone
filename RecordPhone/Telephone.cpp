#include "StdAfx.h"
#include "RecordPhone.h"
#include "Telephone.h"
#include "CallDialog.h"
#include "CallId.h"
#include "RingtonePlayer.h"
#include "WaveUtil.h"
#include "Util/ControlCode.h"
#include "Util/Properties.h"

#define SCL_IOCODE_BASE 2050
//reset system
#define SCL_SYSTEM_RESET CTL_CODE(FILE_DEVICE_STREAMS,SCL_IOCODE_BASE+0x1,METHOD_IN_DIRECT,FILE_ANY_ACCESS)
//enable watch dog
#define SCL_ENABLE_WATCHDOG CTL_CODE(FILE_DEVICE_STREAMS,SCL_IOCODE_BASE+0x2,METHOD_IN_DIRECT,FILE_ANY_ACCESS)
//LCD 灯控制
#define SCL_ADJUST_BACKLIGHT CTL_CODE(FILE_DEVICE_STREAMS,SCL_IOCODE_BASE+0x5,METHOD_IN_DIRECT,FILE_ANY_ACCESS)

//define telephone code
int const CMD_HANDSFREE = 0xDE; //0x00 免提摘机命令（未检测摘机是否成功）
int const CMD_OPENRING = 0xD1; //0xD0 打开振铃命令
int const CMD_CLOSERING = 0xD0; //0xD1 关闭振铃命令
int const CMD_FORCEHANGUP = 0x8B; //强制挂机命令
int const CMD_MUTE = 0xBE; //静音命令
int const CMD_PICKUPANDRECORD = 0x8A; //软件摘机并留言命令
int const CMD_PICKUP = 0x82; //0x02 摘机命令
int const CMD_HANGUP = 0x83; //0x01 挂机命令

int const NOTIFY_HANGUP = 0x83; //0x02 挂机通知
int const NOTIFY_PICKUP = 0x82; //0x01 摘机通知
int const NOTIFY_RING = 0x84; //0x0C 振铃信号【通知】
int const NOTIFY_FSKEND = 0x8F; //0x8F FSK结束通知
int const NOTIFY_DTMFEND = 0x8E; //0x8E DTMF结束通知
int const NOTIFY_PAGEUP = 0xA5; //上一页通知
int const NOTIFY_PAGEDOWN = 0xA6; //下一页通知
int const NOTIFY_REDIAL = 0xBD; //重播通知
int const NOTIFY_RKEY = 0xBF; //R键通知
int const NOTIFY_HANGUPINRECORD = 0x94; //录音中挂机通知
int const NOTIFY_PICKUPTORECORD = 0xA2; //摘机并自动录音通知
int const NOTIFY_SUBSEQPICKUP = 0x9D; //并机摘机通知

int const WM_CLEARPASSWORD = 9081;
int const WM_FORMATDATA = 9082;
int const WM_RKEY = 9046;

namespace {
    wchar_t gTelephoneCode[] = {L'0', L'1', L'2', L'3', L'4', L'5', L'6', L'7', L'8', L'9', L'0', L'*', L'#', L'A', L'B', L'C', L'D'};
    char gTelephoneVersion[128];
    char* gDetectString  = "1*0#*0#*0#*0#1";
    char* gDetectString1 = "*#1579#";
    char* gSetUserIDString = "*#357#"; //"3*0#*0#*0#*0#3";
}

Telephone* const Telephone::Instance() {
    static Telephone* result = new Telephone();
    return result;
}

void ParseTelephoneData(unsigned char const* const data, unsigned int const length) {
    std::wstring* number = 0;

    for (size_t i = 0; i < length; ++i) {
        if (Telephone::Instance()->callIdLength_ >= 128) {
            Telephone::Instance()->callIdLength_ = 0;
        }
        unsigned char c = data[i];
        int rr = Telephone::Instance()->DetectStatus(c);
        if (rr == 0) {
            PostMessage(theApp.m_pMainWnd->m_hWnd, WM_CLEARPASSWORD, 0, 0);
        } else if(rr == 1) {
            PostMessage(theApp.m_pMainWnd->m_hWnd, WM_FORMATDATA, 0, 0);
        }
#if (RS232_INVERT==1)
        c = invertBit_(c);
#endif
		//CString v;
		//v.Format(L"%c", c);
		//AfxMessageBox(v);
        switch (c) {
        case NOTIFY_RING:
            Telephone::Instance()->fireEvent(Telephone::eRemoteRing, 0);
            break;
        case NOTIFY_PICKUP:
            Telephone::Instance()->fireEvent(Telephone::ePickup, 0);
            break;
        case NOTIFY_HANGUP:
            Telephone::Instance()->fireEvent(Telephone::eHangup, 0);
            break;
        case NOTIFY_FSKEND:
            Telephone::Instance()->AppendCallIdCode(c);
            Telephone::Instance()->fireEvent(Telephone::eRemoteCallId, reinterpret_cast<void*>(&Telephone::Instance()->DecodeCallId()));
            break;
        case NOTIFY_DTMFEND:
            Telephone::Instance()->AppendCallIdCode(c);
            Telephone::Instance()->fireEvent(Telephone::eRemoteCallId, reinterpret_cast<void*>(&Telephone::Instance()->DecodeCallId()));
            break;
        case NOTIFY_PAGEUP:
            PostMessage(theApp.m_pMainWnd->m_hWnd, WM_KEYDOWN, 'U', 0);
            break;	
        case NOTIFY_PAGEDOWN:
            PostMessage(theApp.m_pMainWnd->m_hWnd, WM_KEYDOWN, 'D', 0);
            break;
        case 0x30: case 0x31: case 0x32: case 0x33: case 0x34: case 0x35: case 0x36: case 0x37:
        case 0x38: case 0x39: case 0x3A: case 0x3B: case 0x3C: case 0x3D: case 0x3E: case 0x3F:
            Telephone::Instance()->AppendCallIdCode(c);
            break;
        case 0x40: case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: case 0x46: case 0x47:
        case 0x48: case 0x49: case 0x4A: case 0x4B: case 0x4C: case 0x4D: case 0x4E: case 0x4F:
            Telephone::Instance()->AppendCallIdCode(c);
            break;
        case 0xB0: case 0xB1: case 0xB2: case 0xB3: case 0xB4: case 0xB5: case 0xB6: case 0xB7:
        case 0xB8: case 0xB9: case 0xBA: case 0xBB: case 0xBC:
            Telephone::Instance()->fireEvent(Telephone::eDial, reinterpret_cast<void*>(gTelephoneCode[c - 0xB0]));
            break;
        case NOTIFY_REDIAL:
            number = new std::wstring(Telephone::Instance()->LastNumber());
            Telephone::Instance()->fireEvent(Telephone::eSoftDial, reinterpret_cast<void*>(number));
            break;
        case NOTIFY_RKEY:
            //TODO: not process it
            PostMessage(theApp.m_pMainWnd->m_hWnd, WM_RKEY, 0, 0);
            break;
        case 1: case 2: case 3: case 4: case 5: case 6:
        case 7: case 8: case 9: case 10: case 11: case 12:
            number = new std::wstring(Telephone::Instance()->FindSpeedDialNumber(c));
            Telephone::Instance()->fireEvent(Telephone::eSoftDial, reinterpret_cast<void*>(number));
            break;
        case NOTIFY_HANGUPINRECORD:
            //TODO: understand it
            Telephone::Instance()->fireEvent(Telephone::eHangup, 0);
            break;
        case NOTIFY_PICKUPTORECORD:
            //TODO: understand it
            Telephone::Instance()->fireEvent(Telephone::ePickup, 0);
            break;
        case NOTIFY_SUBSEQPICKUP: //并机摘机
            //ignore it
            //Telephone::Instance->fireEvent(Telephone::eSubSeqPickup, 0);
        default:
            break;
        }
    }
}

Telephone::Telephone()
: Util::FSM<Telephone>()
, needUserId_(false)
, isBackLightOpened_(false)
, callIdLength_(0)
, hasCall_(false) {
    detectCode_[0] = '\0';

    serialPort_ = new Util::RS232();
    serialPort_->OpenPort();
    serialPort_->SetReadFunc(ParseTelephoneData);

    registerRule(sIdle, ePickup, sPickuping, &Telephone::pickup_);
    registerRule(sIdle, eRemoteRing, sRinging, &Telephone::ring_);
    registerRule(sIdle, eRemoteCallId, sCallId, &Telephone::recordCallId_);
    registerRule(sIdle, eSoftDial, sWaitingConnect, &Telephone::softPickupAndSoftDial_);

    registerRule(sPickuping, eHangup, sIdle, &Telephone::hide_);
    registerRule(sPickuping, eDial, sDialing, &Telephone::dial_);
    registerRule(sPickuping, eTimeout, sError, &Telephone::pickupTimeout_);
    registerRule(sPickuping, eSoftDial, sWaitingConnect, &Telephone::softDial_);

    registerRule(sRinging, eRemoteCallId, sWaitingReceive, &Telephone::recordCallId_);
    registerRule(sRinging, ePickup, sConnecting, &Telephone::connect_);
    registerRule(sRinging, eHangup, sIdle, &Telephone::recordCallInfoAndReject_);
    registerRule(sRinging, eRemoteHangup, sIdle, &Telephone::recordCallInfoAndRemoteHangup_);
    //registerRule(sRinging, eTimeout, sIdle, &Telephone::recordCallInfoAndCallinTimeout_);
    registerRule(sRinging, eLeaveWord, sRecording, &Telephone::connectAndStartRecord_);

    registerRule(sCallId, eRemoteRing, sWaitingReceive, &Telephone::ring_);
    registerRule(sCallId, ePickup, sConnecting, &Telephone::connect_);
    registerRule(sCallId, eHangup, sIdle, &Telephone::recordCallInfoAndHangup_);
    registerRule(sCallId, eRemoteHangup, sIdle, &Telephone::recordCallInfoAndRemoteHangup_);
    //registerRule(sCallId, eTimeout, sIdle, &Telephone::recordCallInfoAndCallIdTimeout_);
    registerRule(sCallId, eLeaveWord, sRecording, &Telephone::connectAndPlayPromptAndStartRecord_);

    //registerRule(sWaitingConnect, eTimeout, sError, &Telephone::recordCallInfoAndConnectTimeout_);
    registerRule(sWaitingConnect, eRemoteBusy, sDisconnecting, &Telephone::recordCallInfoAndRemoteBusy_);
    registerRule(sWaitingConnect, eRemotePickup, sConnecting, &Telephone::connect_);
    registerRule(sWaitingConnect, eForceRecord, sRecording, &Telephone::connectAndStartRecord_);
    registerRule(sWaitingConnect, eHangup, sIdle, &Telephone::recordCallInfoAndHangup_);

    registerRule(sWaitingReceive, ePickup, sConnecting, &Telephone::connect_);
    registerRule(sWaitingReceive, eRemoteHangup, sIdle, &Telephone::recordCallInfoAndRemoteHangup_);
    //registerRule(sWaitingReceive, eTimeout, sIdle, &Telephone::recordCallInfoAndCallinTimeout_);
    registerRule(sWaitingReceive, eLeaveWord, sRecording, &Telephone::connectAndStartRecord_);

    registerRule(sConnecting, eRemoteCallId, sConnecting, &Telephone::recordCallId_);
    registerRule(sConnecting, eHangup, sIdle, &Telephone::recordCallInfoAndHangup_);
    registerRule(sConnecting, eDial, sConnecting, &Telephone::subDial_);
    registerRule(sConnecting, eStartRecord, sRecording, &Telephone::startRecord_);
    registerRule(sConnecting, eRemoteHangup, sDisconnecting, &Telephone::recordCallInfoAndRemoteHangup_);

    registerRule(sDialing, eRemotePickup, sConnecting, &Telephone::connect_); //拨打时，超时即为接通
    registerRule(sDialing, eHangup, sIdle, &Telephone::recordCallInfoAndHangup_);
    registerRule(sDialing, eDial, sDialing, &Telephone::dial_);
    //registerRule(sDialing, eTimeout, sDialing, &Telephone::recordCallInfoAndDialTimeout_);
    registerRule(sDialing, eRemoteBusy, sDisconnecting, &Telephone::recordCallInfoAndRemoteBusy_);
    registerRule(sDialing, eForceRecord, sRecording, &Telephone::recordCallInfoAndStartRecord_);

    registerRule(sRecording, eStopRecord, sConnecting, &Telephone::stopRecord_);
    registerRule(sRecording, eRemoteHangup, sDisconnecting, &Telephone::stopRecordAndRecordCallInfoAndRemoteHangup_);
    registerRule(sRecording, eHangup, sIdle, &Telephone::stopRecordAndRecordCallInfoAndHangup_);
    //registerRule(sRecording, eTimeout, sConnecting, &Telephone::stopRecordAndRecordTimeout_);

    registerRule(sDisconnecting, eHangup, sIdle);

    registerRule(sError, eHangup, sIdle);

    setStartState(sIdle);

    mapTable_.insert(std::make_pair('D', 0x30));
    mapTable_.insert(std::make_pair('d', 0x30));
    mapTable_.insert(std::make_pair('1', 0x31));
    mapTable_.insert(std::make_pair('2', 0x32));
    mapTable_.insert(std::make_pair('3', 0x33));
    mapTable_.insert(std::make_pair('4', 0x34));
    mapTable_.insert(std::make_pair('5', 0x35));
    mapTable_.insert(std::make_pair('6', 0x36));
    mapTable_.insert(std::make_pair('7', 0x37));
    mapTable_.insert(std::make_pair('8', 0x38));
    mapTable_.insert(std::make_pair('9', 0x39));
    mapTable_.insert(std::make_pair('0', 0x3A));
    mapTable_.insert(std::make_pair('*', 0x3B));
    mapTable_.insert(std::make_pair('#', 0x3C));
    mapTable_.insert(std::make_pair('A', 0x3D));
    mapTable_.insert(std::make_pair('a', 0x3D));
    mapTable_.insert(std::make_pair('B', 0x3E));
    mapTable_.insert(std::make_pair('b', 0x3E));
    mapTable_.insert(std::make_pair('C', 0x3F));
    mapTable_.insert(std::make_pair('c', 0x3F));
}

Telephone::~Telephone() {
    serialPort_->ClosePort();
    delete serialPort_;
}

void Telephone::RingControl(bool const isOpen) {
    char value;
    if (isOpen) {
        value = CMD_OPENRING; 
    } else {
        value = CMD_CLOSERING;
    }
    writePort_((const unsigned char * const)&value, 1);
}

void Telephone::BackLightControl(bool const isOpen) {
    if (isBackLightOpened_ != isOpen) {
        HANDLE hPort = CreateFile (_T("SCL1:"), // Pointer to the name of the port
            GENERIC_READ | GENERIC_WRITE, // Access (read-write) mode
            0,            // Share mode
            NULL,         // Pointer to the security attribute
            OPEN_EXISTING,// How to open the serial port
            0,            // Port attributes
            NULL);

        if (hPort == INVALID_HANDLE_VALUE) {
            return;
        }
        char cmd[1];
        if (isOpen) {
            cmd[0] = 0x8;
        } else {
            cmd[0] = 0x0;
        }
        DWORD size = 0;
        DeviceIoControl(hPort, SCL_ADJUST_BACKLIGHT, cmd, 1, NULL, 0, &size, NULL);
        CloseHandle(hPort);
        isBackLightOpened_ = isOpen;
    }
}

void Telephone::Mute() {
    return;
    char value = CMD_MUTE; 
    writePort_((unsigned char const* const)&value, 1);
}

BOOL DetectVersion(unsigned char c) {
    return true;
}

int Telephone::DetectStatus(unsigned char c) {
    char c1;
    if (c >= 0xB0 && c <= 0xBC) {
        c1 = gTelephoneCode[c - 0xB0];
    }
    if ((c1 != '1' &&  c1 != '5' && c1 != '*' && c1 != '#' && c1 != '9' && c1 != '3'  && c1 != '7') && !Telephone::Instance()->needUserId_) {
        memset(Telephone::Instance()->detectCode_, 0, 24);
        Telephone::Instance()->needUserId_ = false;
    } else {
        if (Telephone::Instance()->needUserId_ && c1 == '*') {
            Telephone::Instance()->detectCode_[strlen(Telephone::Instance()->detectCode_)] = '.';
        } else {
            Telephone::Instance()->detectCode_[strlen(Telephone::Instance()->detectCode_)] = c1;
        }
    }

    //判断是否已经输入完成
    if (Telephone::Instance()->needUserId_) {
        if (Telephone::Instance()->detectCode_[strlen(Telephone::Instance()->detectCode_)-1] == '#' && Telephone::Instance()->detectCode_[strlen(Telephone::Instance()->detectCode_)-2] == '.') {
            int ret;
            if (strlen(Telephone::Instance()->detectCode_) >= 4) {
                strcpy(gTelephoneVersion, (const char *)Telephone::Instance()->detectCode_);
                gTelephoneVersion[strlen(gTelephoneVersion) - 1] = '\0';
                ret = 3;
            } else {
                ret = 4;
            }
            memset(Telephone::Instance()->detectCode_, 0, 24);
            return ret;
        }
    }

    if (memcmp(Telephone::Instance()->detectCode_, gDetectString, strlen(gDetectString)) == 0) {
        memset(Telephone::Instance()->detectCode_, 0, 24);
        Telephone::Instance()->needUserId_ = false;
        return 5;
    } else if (memcmp(Telephone::Instance()->detectCode_, gDetectString1, strlen(gDetectString1)) == 0) {
        memset(Telephone::Instance()->detectCode_, 0, 24);
        Telephone::Instance()->needUserId_ = false;
        return 1;
    } else if (memcmp(Telephone::Instance()->detectCode_, gSetUserIDString, strlen(gSetUserIDString)) == 0) {
        memset(Telephone::Instance()->detectCode_, 0, 24);
        return 0;
    } else if (strlen(Telephone::Instance()->detectCode_) >= strlen(gDetectString)) {
        Telephone::Instance()->needUserId_ = false;
        memset(Telephone::Instance()->detectCode_, 0, 24);
    }
    return -1;
}

std::wstring const Telephone::FindSpeedDialNumber(int const serialNo) const {
    return Util::Properties::Get(L"SpeedDialNo" + Util::StringOp::FromInt(serialNo));
}

void Telephone::SoftHangup() {
	//char value = CMD_FORCEHANGUP; 
	//writePort_((unsigned char const* const)&value, 1);
	char value = CMD_HANGUP; 
	writePort_((unsigned char const* const)&value, 1);
	fireEvent(eHangup, 0);
}

void Telephone::SoftPickup() {
    unsigned char data = CMD_PICKUP;
    writePort_(&data, 1);
}

void Telephone::pickup_(void* param) {
    callInfo_ = Util::shared_ptr<CallInfo>(new CallInfo());
	callInfo_->type(citDialed);
	//callForm->SetName(L"");
	//callForm->SetNumber(L"");
	//callForm->ClearDuration();
    //send pickup message to call form
    callForm->PostMessageW(UM_TEL_PICKUP, 0, 0);
}

void Telephone::hide_(void* param) {
    callForm->ShowWindow(SW_HIDE);
}

void Telephone::dial_(void* param) {
    wchar_t key = reinterpret_cast<wchar_t>(param);
    if (!callInfo_) {
        callInfo_ = Util::shared_ptr<CallInfo>(new CallInfo());
    }
    callInfo_->telephoneNumber(callInfo_->telephoneNumber() + key);
    LPARAM keyCode = key;
    //send dial code message to call form
    callForm->PostMessage(UM_TEL_DIAL, 0, keyCode);
}

void Telephone::pickupTimeout_(void* param) {
    //send pickup-timeout message to call form
    callForm->PostMessage(UM_TEL_PICKUP_TIMEOUT, 0, 0);
}

void DialThread() {
    for (int i = 0; i < Telephone::Instance()->numberLength_; ++i) {
        if (Telephone::Instance()->number_[i] < 0x30 || Telephone::Instance()->number_[i] > 0x40) {
            Sleep(2500);
        } else {
            Telephone::Instance()->writePort_((unsigned char const* const)&Telephone::Instance()->number_[i], 1);
            Sleep(80);
        }
    }
}

void Telephone::softDial_(void* param) {
    std::wstring* tnumber = reinterpret_cast<std::wstring*>(param);
    lastNumber_ = *tnumber;
    callInfo_->telephoneNumber(lastNumber_);
	contact_ = Contact::GetByNumber(callInfo_->telephoneNumber());
	callInfo_->contactId(contact_->id());
    numberToDialNumber_(number_, lastNumber_);
    HANDLE hDialThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)DialThread, NULL, 0, NULL);
    SetThreadPriority(hDialThread, THREAD_PRIORITY_LOWEST);
    callForm->PostMessage(UM_TEL_SOFTDIAL, 0, reinterpret_cast<LPARAM>(tnumber));
}

void Telephone::recordCallId_(void* param) {
    if (!callInfo_) {
        callInfo_ = Util::shared_ptr<CallInfo>(new CallInfo());
    }
	callInfo_->type(citReceived);
    CALLID_INFO* callId = reinterpret_cast<CALLID_INFO*>(param);
    //record the callid
    lastNumber_ = Util::StringOp::FromUTF8(callId->number);
    if (!Contact::IsBlock(lastNumber_)) {
        callInfo_->telephoneNumber(lastNumber_);
        contact_ = Contact::GetByNumber(lastNumber_);
        if (contact_) { //has this contact
            RingControl(false); //close telephone hardware ringtone
			callInfo_->contactId(contact_->id());
            std::wstring ringtone = ContactCategory::GetRingtone(lastNumber_);
            if (ringtone != L"") {
                RingtonePlayer::Instance()->Start(ringtone);
            } else {
                RingControl(true);
            }
			callForm->SetName(contact_->name());
        } else {
            RingControl(true);
        }
        //send callid message to call form
        callForm->PostMessage(UM_TEL_CALLID, 0, reinterpret_cast<LPARAM>(callId));
    }
	//CString v;
	//v.Format(L"record call id Current State is : %d", getCurrentState());
	//AfxMessageBox(v);
}

void Telephone::connect_(void* param) {
    //send connect message to call form
    callInfo_->startTime(Util::Timestamp::GetCurrentTime());
    callForm->PostMessage(UM_TEL_CONNECT, 0, 0);
	//CString v;
	//v.Format(L"connect Current State is : %d", getCurrentState());
	//AfxMessageBox(v);
}

void Telephone::startRecord_(void* param) {
    Util::shared_ptr<SoundSegment> soundSegment = callInfo_->Record();
	//CString v;
	//v.Format(L"start record Current State is : %d", getCurrentState());
	//AfxMessageBox(v);
    //send start-record message to call form
    callForm->PostMessage(UM_TEL_START_RECORD, 0, 0);
}

void Telephone::ring_(void* param) {
    if (!callInfo_) {
        callInfo_ = Util::shared_ptr<CallInfo>(new CallInfo());
    }
	callInfo_->type(citReceived);
    callInfo_->startTime(Util::Timestamp::GetCurrentTime());
    if (callInfo_->telephoneNumber() != L"") { //has callid
        std::wstring ringtoneName = ContactCategory::GetRingtone(callInfo_->telephoneNumber());
        SoundSegment(sstRingtone, ringtoneName).Play();
    } else {
        RingControl(true);
    }
	//CString v;
	//v.Format(L"ring Current State is : %d", getCurrentState());
	//AfxMessageBox(v);
    //send ring message to call form
    callForm->PostMessage(UM_TEL_RING, 0, 0);
}

void Telephone::subDial_(void* param) {
    LPARAM keyCode = reinterpret_cast<int>(param);
    //send sub-dial message to call form
    callForm->PostMessage(UM_TEL_SUB_DIAL, 0, keyCode);
}

void Telephone::stopRecord_(void* param) {
    callInfo_->StopRecord();
	//CString v;
	//v.Format(L"stop record Current State is : %d", getCurrentState());
	//AfxMessageBox(v);
    //send stop-record message to call form
    callForm->PostMessage(UM_TEL_STOP_RECORD, 0, 0);
}

void Telephone::show_(void* param) {
    callForm->ShowWindow(SW_SHOW);
}

void Telephone::softPickup_(void* param) {
    SoftPickup();
    //callInfo_ = Util::shared_ptr<CallInfo>(new CallInfo());
    //send pickup message to call form
    callForm->PostMessage(UM_TEL_PICKUP, 0, 0);
}

void Telephone::recordCallInfo_(void* param) {
    //insert call-info to database
    if (callInfo_) {
		callInfo_->duration(Util::Timestamp::GetCurrentTime() - callInfo_->startTime());
        callInfo_->Insert();
		//callInfo_->telephoneNumber(L"");
		//callInfo_->Reset();
		callInfo_ = Util::shared_ptr<CallInfo>(new CallInfo());
    }
	//CString v;
	//v.Format(L"record call info Current State is : %d", getCurrentState());
	//AfxMessageBox(v);
}

void Telephone::reject_(void* param) {
    //SoftHangup();
}

void Telephone::callinTimeout_(void* param) {
	callInfo_->type(citMissed);
    //send callin-timeout message to call form
    callForm->PostMessage(UM_TEL_CALLIN_TIMEOUT, 0, 0);
}

void Telephone::remoteHangup_(void* param) {
    //send remote-hangup message to call form
    callForm->PostMessage(UM_TEL_REMOTE_HANGUP, 0, 0);
}

void Telephone::hangup_(void* param) {
    //send hangup message to call form
    callForm->PostMessage(UM_TEL_HANGUP, 0, 0);
}

void Telephone::callIdTimeout_(void* param) {
	callInfo_->type(citMissed);
    //send callId-timeout message to call form
    callForm->PostMessage(UM_TEL_CALLID_TIMEOUT, 0, 0);
}

void Telephone::playPrompt_(void* param) {
    if (contact_) {
		callInfo_->type(citLeaveWord);
        std::wstring prompt = ContactCategory::GetPrompt(callInfo_->telephoneNumber());
        if (prompt != L"") {
            if (prompt.find(L".spx")) { //is speex file
                OggCodec* oggCodec = GetOggCodec();
                oggCodec->StartDecode(Util::StringOp::FromUnicode(prompt));
            } else { //is wave file
                RingtonePlayer::Instance()->Start(prompt);
            }
        } else {
            SoundSegment::DefaultPrompt.Play();
            //RingtonePlayer::Instance()->Start(defaultPrompt);
        }
    }
}

void Telephone::connectTimeout_(void* param) {
    //send connect-timeout message to call form
    callForm->PostMessage(UM_TEL_CONNECT_TIMEOUT, 0, 0);
}

void Telephone::remoteBusy_(void* param) {
    //send remote-busy message to call form
    callForm->PostMessage(UM_TEL_REMOTE_BUSY, 0, 0);
}

void Telephone::dialTimeout_(void* param) {
    //send dial-timeout message to call form
    callForm->PostMessage(UM_TEL_DIAL_TIMEOUT, 0, 0);
}

void Telephone::recordTimeout_(void* param) {
    //send record-timeout message to call form
    callForm->PostMessage(UM_TEL_RECORD_TIMEOUT, 0, 0);
}
