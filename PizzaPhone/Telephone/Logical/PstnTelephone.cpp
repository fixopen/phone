#include "PstnTelephone.h"
#include "../View/CallDialog.h"
#include "../../Util/Audio/DSoundPlayer.h"
#include "../../Util/Audio/WaveUtil.h"
#include "../../Util/ControlCode.h"
#include "../../Util/Properties.h"
#include "../../PizzaPhone.h"
#include "../../Contact/Data/SoundSegment.h"

#define SCL_IOCODE_BASE 2050
//reset system
#define SCL_SYSTEM_RESET CTL_CODE(FILE_DEVICE_STREAMS,SCL_IOCODE_BASE+0x1,METHOD_IN_DIRECT,FILE_ANY_ACCESS)
//enable watch dog
#define SCL_ENABLE_WATCHDOG CTL_CODE(FILE_DEVICE_STREAMS,SCL_IOCODE_BASE+0x2,METHOD_IN_DIRECT,FILE_ANY_ACCESS)
//LCD 灯控制
#define SCL_ADJUST_BACKLIGHT CTL_CODE(FILE_DEVICE_STREAMS,SCL_IOCODE_BASE+0x5,METHOD_IN_DIRECT,FILE_ANY_ACCESS)

#define __ZYN__
//define telephone code
#if defined(__LDJ__)
unsigned char const CMD_HANDSFREE = '\x00'; //免提摘机命令（未检测摘机是否成功）
unsigned char const CMD_HANGUP = '\x01'; //挂机命令
unsigned char const CMD_PICKUP = '\x02'; //摘机命令

unsigned char const CMD_OPENRING = '\xD0'; //打开振铃命令
unsigned char const CMD_CLOSERING = '\xD1'; //关闭振铃命令
#elif defined(__ZYN__)
unsigned char const CMD_PICKUP = '\x82'; //摘机命令,unmute
unsigned char const CMD_HANGUP = '\x83'; //挂机命令

unsigned char const CMD_CLOSERING = '\xD0'; //关闭振铃命令
unsigned char const CMD_OPENRING = '\xD1'; //打开振铃命令
#endif
unsigned char const CMD_GETVERSION = '\x80';
unsigned char const CMD_PICKUPANDRECORD = '\x8A'; //软件摘机,mute
unsigned char const CMD_FORCEHANGUP = '\x8B'; //强制挂机命令

unsigned char const CMD_LEAVEWORD_PICKUP = '\x94';
unsigned char const CMD_QUERY_LINESTATE = '\x96';

unsigned char const CMD_REDIAL = '\xBD'; //重播命令
unsigned char const CMD_MUTE = '\xBE'; //静音命令
unsigned char const CMD_RKEY = '\xBF'; //R键命令

unsigned char const CMD_SOUND_SWITCH_TO_HANDSFREE = '\xC1';
unsigned char const CMD_SOUND_SWITCH_TO_HANDSET = '\xC2';
unsigned char const CMD_QUERY_STATE = '\xC3';

unsigned char const CMD_MIC_ON = '\xD8'; //Mic1 Mic2 on
unsigned char const CMD_MIC_OFF = '\xD9'; //Mic1 Mic2 off

#if defined(__LDJ__)
unsigned char const NOTIFY_PICKUP = '\x01'; //摘机通知
unsigned char const NOTIFY_HANGUP = '\x02'; //挂机通知
unsigned char const NOTIFY_RING = '\x0C'; //振铃信号【通知】
#elif defined(__ZYN__)
unsigned char const NOTIFY_PICKUP = '\x82'; //摘机通知
unsigned char const NOTIFY_HANGUP = '\x83'; //挂机通知
unsigned char const NOTIFY_RING = '\x84'; //振铃信号【通知】
#endif
unsigned char const NOTIFY_PICKUPERROR = '\x81'; //no pstn line
unsigned char const NOTIFY_VERSIONEND = '\x8C';
unsigned char const NOTIFY_DTMFEND = '\x8E'; //DTMF结束通知
unsigned char const NOTIFY_FSKEND = '\x8F'; //FSK结束通知

unsigned char const NOTIFY_LINE_PICKUP = '\x96';
unsigned char const NOTIFY_LINE_NONPICKUP = '\x97';
unsigned char const NOTIFY_START_SPEED_SET = '\x98';
unsigned char const NOTIFY_STOP_SPEED_SET = '\x99';
unsigned char const NOTIFY_SUBSEQPICKUP = '\x9D'; //并机摘机通知
unsigned char const NOTIFY_SUBSEQHANGUP = '\x9E'; //并机挂机通知

unsigned char const NOTIFY_PICKUPTORECORD = '\xA2'; //摘机并自动录音通知

unsigned char const NOTIFY_PAGEUP = '\xA5'; //上一页通知
unsigned char const NOTIFY_PAGEDOWN = '\xA6'; //下一页通知

//B1-B9 : 1-9; BA : 0; BB : *; BC : #;
unsigned char const NOTIFY_REDIAL = '\xBD'; //重播通知
unsigned char const NOTIFY_MUTE = '\xBE'; //静音通知
unsigned char const NOTIFY_RKEY = '\xBF'; //R键通知

unsigned char const NOTIFY_SOUND_SWITCH_TO_HANDSFREE = '\xC1';
unsigned char const NOTIFY_SOUND_SWITCH_TO_HANDSET = '\xC2';
unsigned char const NOTIFY_HANDSET_PICKUP = '\xC8';
unsigned char const NOTIFY_HANDSFREE_PICKUP = '\xC9';
unsigned char const NOTIFY_SWITCH_TO_HANDSFREE = '\xCA';
unsigned char const NOTIFY_SWITCH_TO_HANDSET = '\xCB';

unsigned char const NOTIFY_HANDSFREE = '\xDE'; //免提摘机命令（未检测摘机是否成功）

int const WM_CLEARPASSWORD = 9081;
int const WM_FORMATDATA = 9082;
int const WM_RKEY = 9046;

namespace Telephone {
    namespace Logical {
        namespace {
            //wchar_t gTelephoneCode[] = {L'0', L'1', L'2', L'3', L'4', L'5', L'6', L'7', L'8', L'9', L'0', L'*', L'#', L'A', L'B', L'C', L'D'};
            //wchar_t gTelephoneCode[] = L"01234567890*#ABCD";
            char gTelephoneCode[] = {
                '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '*', '#', 'A', 'B', 'C', 'D'
            };
            //char gTelephoneCode[] = "01234567890*#ABCD";
            char gTelephoneVersion[128];
            char* gDetectString = "1*0#*0#*0#*0#1";
            char* gDetectString1 = "*#1579#";
            char* gSetUserIDString = "*#357#"; //"3*0#*0#*0#*0#3";
        }

        PstnTelephone* const PstnTelephone::Instance() {
            static PstnTelephone* result = new PstnTelephone();
            return result;
        }

        void ParseTelephoneData(unsigned char const * const data, unsigned int const length) {
            std::wstring* number = 0;

            for (size_t i = 0; i < length; ++i) {
                if (PstnTelephone::Instance()->callIdLength_ >= 128) {
                    PstnTelephone::Instance()->callIdLength_ = 0;
                }
                unsigned char c = data[i];
                int rr = PstnTelephone::Instance()->DetectStatus(c);
                if (rr == 0) {
                    PostMessage(theApp.m_pMainWnd->m_hWnd, WM_CLEARPASSWORD, 0, 0);
                } else if (rr == 1) {
                      PostMessage(theApp.m_pMainWnd->m_hWnd, WM_FORMATDATA, 0, 0);
                }
#if (RS232_INVERT == 1)
                c = invertBit_(c);
#endif
                switch (c) {
                case NOTIFY_RING:
                    PstnTelephone::Instance()->fireEvent(PstnTelephone::eRemoteRing, 0);
                    break;
                case NOTIFY_PICKUP:
                    PstnTelephone::Instance()->fireEvent(PstnTelephone::ePickup, 0);
                    break;
                case NOTIFY_HANGUP:
                    PstnTelephone::Instance()->fireEvent(PstnTelephone::eHangup, 0);
                    break;
                case NOTIFY_FSKEND:
                    PstnTelephone::Instance()->AppendCallIdCode(c);
                    PstnTelephone::Instance()->fireEvent(PstnTelephone::eRemoteCallId, reinterpret_cast<void*>(&PstnTelephone::Instance()->DecodeCallId()));
                    break;
                case NOTIFY_DTMFEND:
                    PstnTelephone::Instance()->AppendCallIdCode(c);
                    PstnTelephone::Instance()->fireEvent(PstnTelephone::eRemoteCallId, reinterpret_cast<void*>(&PstnTelephone::Instance()->DecodeCallId()));
                    break;
                case NOTIFY_PAGEUP:
                    PostMessage(theApp.m_pMainWnd->m_hWnd, WM_KEYDOWN, 'U', 0);
                    break;  
                case NOTIFY_PAGEDOWN:
                    PostMessage(theApp.m_pMainWnd->m_hWnd, WM_KEYDOWN, 'D', 0);
                    break;
                case 0x30:
                case 0x31:
                case 0x32:
                case 0x33:
                case 0x34:
                case 0x35:
                case 0x36:
                case 0x37:
                case 0x38:
                case 0x39:
                case 0x3A:
                case 0x3B:
                case 0x3C:
                case 0x3D:
                case 0x3E:
                case 0x3F:
                    PstnTelephone::Instance()->AppendCallIdCode(c);
                    break;
                case 0x40:
                case 0x41:
                case 0x42:
                case 0x43:
                case 0x44:
                case 0x45:
                case 0x46:
                case 0x47:
                case 0x48:
                case 0x49:
                case 0x4A:
                case 0x4B:
                case 0x4C:
                case 0x4D:
                case 0x4E:
                case 0x4F:
                    PstnTelephone::Instance()->AppendCallIdCode(c);
                    break;
                case 0xB0:
                case 0xB1:
                case 0xB2:
                case 0xB3:
                case 0xB4:
                case 0xB5:
                case 0xB6:
                case 0xB7:
                case 0xB8:
                case 0xB9:
                case 0xBA:
                case 0xBB:
                case 0xBC:
                    PstnTelephone::Instance()->fireEvent(PstnTelephone::eDial, reinterpret_cast<void*>(gTelephoneCode[c - 0xB0]));
                    break;
                case NOTIFY_REDIAL:
                    number = new std::wstring(PstnTelephone::Instance()->LastNumber());
                    //PstnTelephone::Instance()->fireEvent(PstnTelephone::eSoftDial, reinterpret_cast<void*>(number));
                    break;
                case NOTIFY_RKEY:
                    //TODO: not process it
                    PostMessage(theApp.m_pMainWnd->m_hWnd, WM_RKEY, 0, 0);
                    break;
                case 1:
                case 2:
                case 3:
                case 4:
                case 5:
                case 6:
                case 7:
                case 8:
                case 9:
                case 10:
                case 11:
                case 12:
                    number = new std::wstring(PstnTelephone::Instance()->FindSpeedDialNumber(c));
                    //PstnTelephone::Instance()->fireEvent(PstnTelephone::eSoftDial, reinterpret_cast<void*>(number));
                    break;
                    //case NOTIFY_HANGUPINRECORD:
                    //    //TODO: understand it
                    //    PstnTelephone::Instance()->fireEvent(PstnTelephone::eHangup, 0);
                    //    break;
                case NOTIFY_PICKUPTORECORD:
                    //TODO: understand it
                    PstnTelephone::Instance()->fireEvent(PstnTelephone::ePickup, 0);
                    break;
                case NOTIFY_SUBSEQPICKUP:
                    //并机摘机
                    //TODO: ignore it
                    //PstnTelephone::Instance->fireEvent(PstnTelephone::eSubSeqPickup, 0);
                default:
                    break;
                }
            }
        }

        PstnTelephone::PstnTelephone()
        : Util::FSM<PstnTelephone>()
        , needUserId_(false)
        , isBackLightOpened_(false)
        , callIdLength_(0)
        , hasCall_(false) {
            detectCode_[0] = '\0';

            serialPort_ = new Util::RS232();
            serialPort_->Open();
            serialPort_->SetReadFunc(ParseTelephoneData);

            registerRule(sIdle, ePickup, sPickuping, &PstnTelephone::setPickupTimerAndShowClearCallForm_);
            registerRule(sIdle, eRemoteRing, sRinging, &PstnTelephone::setRingTimerAndNewAndSetTypeAndSetStartTimeAndShowClearCallForm_);
            registerRule(sIdle, eRemoteCallId, sCallId, &PstnTelephone::setCallIdTimerAndNewAndSetTypeAndSetStartTimeAndRecordCallIdAndShowCallForm_);

            registerRule(sPickuping, eHangup, sIdle, &PstnTelephone::clearPickupTimerAndHideCallForm_);
            registerRule(sPickuping, eDial, sDialing, &PstnTelephone::clearPickupTimerAndSetDialTimerAndNewAndSetTypeAndSetStartTimeAndRecordFirstDialKeyAndNotifyCallFormNumber_);
            registerRule(sPickuping, ePickupTimeout, sPickupTimeout, &PstnTelephone::notifyCallFormPickupTimeout_);

            registerRule(sPickupTimeout, eHangup, sIdle, &PstnTelephone::hideCallForm_);

            registerRule(sDialing, eHangup, sIdle, &PstnTelephone::clearDialTimerAndSetTypeAndDurationAndInsertCallInfoAndHangupAndHideCallForm_);
            registerRule(sDialing, eDial, sDialing, &PstnTelephone::resetDialTimerAndRecordDialKeyAndNotifyCallFormNumber_);
            registerRule(sDialing, eDialTimeout, sWaitingConnect, &PstnTelephone::setWaitConnectTimerAndStopRecordDialKeyAndDisableCallFormNumberChange_);

            registerRule(sWaitingConnect, eWaitConnectTimeout, sConnecting, &PstnTelephone::clearWaitConnectTimerAndUpdateStartTimeAndNotifyCallFormConnected_);
            registerRule(sWaitingConnect, eHangup, sIdle, &PstnTelephone::clearWaitConnectTimerAndSetDurationAndInsertCallInfoAndHideCallForm_);

            registerRule(sConnecting, eHangup, sIdle, &PstnTelephone::setDurationAndInsertCallInfoAndHideCallForm_);
            registerRule(sConnecting, eDial, sConnecting, &PstnTelephone::subDial_);
            registerRule(sConnecting, eStartRecord, sRecording, &PstnTelephone::setRecordTimerAndNewRecordAndSetRecordStartTimeAndStartRecordAndNotifyCallFormStartRecord_);
            registerRule(sConnecting, eRemoteHangup, sDisconnecting, &PstnTelephone::setDisconnectTimerAndSetDurationAndInsertCallInfoAndNotifyCallFormRemoteHangup_);
            registerRule(sConnecting, eRemoteCallId, sConnecting, &PstnTelephone::recordCallIdAndNotifyCallFormNumber_);

            registerRule(sRecording, eStopRecord, sConnecting, &PstnTelephone::setRecordDurationAndInsertRecordAndStopRecordAndNotifyCallFormStopRecord_);
            registerRule(sRecording, eRemoteHangup, sDisconnecting, &PstnTelephone::setDisconnectTimerAndStopRecordAndInsertCallInfoAndNotifyCallFormRemoteHangup_);
            registerRule(sRecording, eHangup, sIdle, &PstnTelephone::stopRecordAndInsertCallInfoAndHideCallForm_);
            registerRule(sRecording, eStorageFull, sConnecting, &PstnTelephone::stopRecordAndNotifyCallFormStorageFull_);
            registerRule(sRecording, eRecordMaxDurationOver, sConnecting, &PstnTelephone::stopRecordAndNotifyCallFormMaxDurationOver_);

            registerRule(sRinging, eRemoteCallId, sWaitingReceive, &PstnTelephone::clearRingTimerAndSetCallIdTimerAndRecordCallIdAndNotifyCallFormNumber_);
            registerRule(sRinging, ePickup, sConnecting, &PstnTelephone::clearRingTimerAndNotifyCallFormConnected_);
            registerRule(sRinging, eHangup, sIdle, &PstnTelephone::clearRingTimerAndInsertCallInfoAndHideCallForm_);
            registerRule(sRinging, eRemoteHangup, sIdle, &PstnTelephone::clearRingTimerAndInsertCallInfoAndNotifyCallFormRemoteHangup_);
            registerRule(sRinging, eRemoteRingTimeout, sAnswerTimeout, &PstnTelephone::setAnswerTimerAndInsertCallInfoAndNotifyCallFormCallinTimeout_);
            registerRule(sRinging, eAutoAnswer, sConnecting, &PstnTelephone::clearRingTimerAndPlayPromptAndNotifyCallFormConnected_);

            registerRule(sCallId, eRemoteRing, sWaitingReceive, &PstnTelephone::clearCallIdTimerAndSetWaitReceiveTimerAndNotifyCallFormRing_);
            registerRule(sCallId, ePickup, sConnecting, &PstnTelephone::clearCallIdTimerAndNotifyCallFormConnected_);
            registerRule(sCallId, eHangup, sIdle, &PstnTelephone::clearCallIdTimerAndInsertCallInfoAndHideCallForm_);
            registerRule(sCallId, eRemoteHangup, sIdle, &PstnTelephone::clearCallIdTimerAndInsertCallInfoAndNotifyCallFormRemoteHangup_);
            registerRule(sCallId, eRemoteCallIdTimeout, sAnswerTimeout, &PstnTelephone::setAnswerTimerAndInsertCallInfoAndNotifyCallFormCallIdTimeout_);
            registerRule(sCallId, eAutoAnswer, sConnecting, &PstnTelephone::clearCallIdTimerAndPlayPromptAndNotifyCallFormConnected_);

            registerRule(sWaitingReceive, ePickup, sConnecting, &PstnTelephone::clearWaitReceiveTimerAndNotifyCallFormConnected_);
            registerRule(sWaitingReceive, eRemoteHangup, sIdle, &PstnTelephone::clearWaitReceiveTimerAndInsertCallInfoAndNotifyCallFormRemoteHangup_);
            registerRule(sWaitingReceive, eRemoteWaitReceiveTimeout, sAnswerTimeout, &PstnTelephone::setAnswerTimerAndInsertCallInfoAndNotifyCallFormCallinTimeout_);
            registerRule(sWaitingReceive, eAutoAnswer, sConnecting, &PstnTelephone::clearWaitReceiveTimerAndNotifyCallFormConnected_);

            registerRule(sAnswerTimeout, eAnswerTimeout, sIdle, &PstnTelephone::insertCallInfo_);

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

            SetTimeout(tPickup, 5 * 1000);
            SetTimeout(tDial, 3 * 1000);
            SetTimeout(tWaitReceive, 3 * 1000);
            SetTimeout(tRing, 5 * 1000);
            SetTimeout(tCallId, 5 * 1000);
            SetTimeout(tWaitReceive, 5 * 1000);
            SetTimeout(tAnswer, 2 * 1000);
            SetTimeout(tRecord, 24 * 60 * 60 * 1000);
            SetTimeout(tDisconnect, 10 * 60 * 1000);
        }

        PstnTelephone::~PstnTelephone() {
            serialPort_->Close();
            delete serialPort_;
        }

        void PstnTelephone::RingControl(bool const isOpen) {
            char value;
            if (isOpen) {
                value = CMD_OPENRING;
            } else {
                value = CMD_CLOSERING;
            }
            writePort_((const unsigned char * const) & value, 1);
        }

        void PstnTelephone::BackLightControl(bool const isOpen) {
            if (isBackLightOpened_ != isOpen) {
                HANDLE hPort = CreateFile(_T("SCL1:"), // Pointer to the name of the port
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

        void PstnTelephone::Mute() {
            return;
            char value = CMD_MUTE; 
            writePort_((unsigned char const * const) & value, 1);
        }

        BOOL DetectVersion(unsigned char c) {
            return true;
        }

        int PstnTelephone::DetectStatus(unsigned char c) {
            char c1;
            if (c >= 0xB0 && c <= 0xBC) {
                c1 = gTelephoneCode[c - 0xB0];
            }
            if ((c1 != '1' && c1 != '5' && c1 != '*' && c1 != '#' && c1 != '9' && c1 != '3' && c1 != '7') && !PstnTelephone::Instance()->needUserId_) {
                memset(PstnTelephone::Instance()->detectCode_, 0, 24);
                PstnTelephone::Instance()->needUserId_ = false;
            } else {
                if (PstnTelephone::Instance()->needUserId_ && c1 == '*') {
                    PstnTelephone::Instance()->detectCode_[strlen(PstnTelephone::Instance()->detectCode_)] = '.';
                } else {
                      PstnTelephone::Instance()->detectCode_[strlen(PstnTelephone::Instance()->detectCode_)] = c1;
                }
            }

            //判断是否已经输入完成
            if (PstnTelephone::Instance()->needUserId_) {
                if (PstnTelephone::Instance()->detectCode_[strlen(PstnTelephone::Instance()->detectCode_) - 1] == '#' && PstnTelephone::Instance()->detectCode_[strlen(PstnTelephone::Instance()->detectCode_) - 2] == '.') {
                    int ret;
                    if (strlen(PstnTelephone::Instance()->detectCode_) >= 4) {
                        //strcpy(gTelephoneVersion, (const char *)PstnTelephone::Instance()->detectCode_);
                        strcpy_s(gTelephoneVersion, 128, (const char*)PstnTelephone::Instance()->detectCode_);
                        gTelephoneVersion[strlen(gTelephoneVersion) - 1] = '\0';
                        ret = 3;
                    } else {
                        ret = 4;
                    }
                    memset(PstnTelephone::Instance()->detectCode_, 0, 24);
                    return ret;
                }
            }

            if (memcmp(PstnTelephone::Instance()->detectCode_, gDetectString, strlen(gDetectString)) == 0) {
                memset(PstnTelephone::Instance()->detectCode_, 0, 24);
                PstnTelephone::Instance()->needUserId_ = false;
                return 5;
            } else if (memcmp(PstnTelephone::Instance()->detectCode_, gDetectString1, strlen(gDetectString1)) == 0) {
                  memset(PstnTelephone::Instance()->detectCode_, 0, 24);
                  PstnTelephone::Instance()->needUserId_ = false;
                  return 1;
            } else if (memcmp(PstnTelephone::Instance()->detectCode_, gSetUserIDString, strlen(gSetUserIDString)) == 0) {
                  memset(PstnTelephone::Instance()->detectCode_, 0, 24);
                  return 0;
            } else if (strlen(PstnTelephone::Instance()->detectCode_) >= strlen(gDetectString)) {
                  PstnTelephone::Instance()->needUserId_ = false;
                  memset(PstnTelephone::Instance()->detectCode_, 0, 24);
            }
            return -1;
        }

        void PstnTelephone::SwitchSound(SoundType const soundType) {
            unsigned char value = CMD_SOUND_SWITCH_TO_HANDSFREE;
            switch (soundType) {
            case stPstn:
                value = CMD_SOUND_SWITCH_TO_HANDSET;
                break;
            case stAudioCard:
                value = CMD_SOUND_SWITCH_TO_HANDSFREE;
                break;
            default:
                break;
            }
            writePort_(&value, 1);
        }
        std::wstring const PstnTelephone::FindSpeedDialNumber(int const serialNo) const {
            return Util::Properties::Get(L"SpeedDialNo" + Util::Text::StringOp::FromInt(serialNo));
        }

        void PstnTelephone::ForceHangup() {
            unsigned char value = CMD_FORCEHANGUP; 
            writePort_((unsigned char const * const) & value, 1);
        }

        void PstnTelephone::SoftHangup() {
            unsigned char value = CMD_HANGUP; 
            writePort_((unsigned char const * const) & value, 1);
            fireEvent(eHangup, 0);
        }

        void PstnTelephone::SoftPickup() {
            unsigned char data = CMD_PICKUP;
            writePort_(&data, 1);
            fireEvent(ePickup, 0);
        }

        void PstnTelephone::SoftDial(std::wstring const & number) {
            lastNumber_ = number;
            numberToDialNumber_(number_, lastNumber_);
            for (int i = 0; i < numberLength_; ++i) {
                if (number_[i] < 0x30 || number_[i] > 0x40) {
                    ::Sleep(2500);
                } else {
                    writePort_((unsigned char const * const) & number_[i], 1);
                    fireEvent(eDial, 0);
                    ::Sleep(80);
                }
            }
        }

        //void PstnTelephone::pickup_(void* param) {
        //    callInfo_ = new Data::CallInfo();
        //    callInfo_->type(Data::citDialed);
        //    //callForm->SetName(L"");
        //    //callForm->SetNumber(L"");
        //    //callForm->ClearDuration();
        //    //send pickup message to call form
        //    //callForm->PostMessageW(UM_TEL_PICKUP, 0, 0);
        //}

        //void PstnTelephone::hide_(void* param) {
        //    //callForm->ShowWindow(SW_HIDE);
        //}

        //void PstnTelephone::dial_(void* param) {
        //    wchar_t key = reinterpret_cast<wchar_t>(param);
        //    if (!callInfo_) {
        //        callInfo_ = new Data::CallInfo();
        //    }
        //    callInfo_->telephoneNumber(callInfo_->telephoneNumber() + key);
        //    LPARAM keyCode = key;
        //    //send dial code message to call form
        //    //callForm->PostMessage(UM_TEL_DIAL, 0, keyCode);
        //}

        //void PstnTelephone::pickupTimeout_(void* param) {
        //    //send pickup-timeout message to call form
        //    //callForm->PostMessage(UM_TEL_PICKUP_TIMEOUT, 0, 0);
        //}

        //void DialThread() {
        //    for (int i = 0; i < PstnTelephone::Instance()->numberLength_; ++i) {
        //        if (PstnTelephone::Instance()->number_[i] < 0x30 || PstnTelephone::Instance()->number_[i] > 0x40) {
        //            Sleep(2500);
        //        } else {
        //            PstnTelephone::Instance()->writePort_((unsigned char const* const)&PstnTelephone::Instance()->number_[i], 1);
        //            Sleep(80);
        //        }
        //    }
        //}

        //void PstnTelephone::softDial_(void* param) {
        //    std::wstring* tnumber = reinterpret_cast<std::wstring*>(param);
        //    lastNumber_ = *tnumber;
        //    callInfo_->telephoneNumber(lastNumber_);
        //    contact_ = Contact::Data::Contact::GetByNumber(callInfo_->telephoneNumber());
        //    callInfo_->contactId(contact_->id());
        //    numberToDialNumber_(number_, lastNumber_);
        //    HANDLE hDialThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)DialThread, NULL, 0, NULL);
        //    SetThreadPriority(hDialThread, THREAD_PRIORITY_LOWEST);
        //    //callForm->PostMessage(UM_TEL_SOFTDIAL, 0, reinterpret_cast<LPARAM>(tnumber));
        //}

        //void PstnTelephone::recordCallId_(void* param) {
        //    if (!callInfo_) {
        //        callInfo_ = new Data::CallInfo();
        //    }
        //    callInfo_->type(Data::citReceived);
        //    CALLID_INFO* callId = reinterpret_cast<CALLID_INFO*>(param);
        //    //record the callid
        //    lastNumber_ = Util::StringOp::FromUTF8(callId->number);
        //    if (!Contact::Data::Contact::IsBlock(lastNumber_)) {
        //        callInfo_->telephoneNumber(lastNumber_);
        //        contact_ = Contact::Data::Contact::GetByNumber(lastNumber_);
        //        if (contact_) { //has this contact
        //            RingControl(false); //close telephone hardware ringtone
        //            callInfo_->contactId(contact_->id());
        //            std::wstring ringtone = Contact::Data::ContactCategory::GetRingtone(lastNumber_);
        //            if (ringtone != L"") {
        //                Util::RingtonePlayer::Instance()->Start(ringtone);
        //            } else {
        //                RingControl(true);
        //            }
        //            //callForm->SetName(contact_->name());
        //        } else {
        //            RingControl(true);
        //        }
        //        //send callid message to call form
        //        //callForm->PostMessage(UM_TEL_CALLID, 0, reinterpret_cast<LPARAM>(callId));
        //    }
        //}

        //void PstnTelephone::connect_(void* param) {
        //    //send connect message to call form
        //    callInfo_->startTime(Util::Timestamp::GetCurrentTime());
        //    //callForm->PostMessage(UM_TEL_CONNECT, 0, 0);
        //}

        //void PstnTelephone::startRecord_(void* param) {
        //    Util::shared_ptr<Contact::Data::SoundSegment> soundSegment = callInfo_->Record();
        //    //send start-record message to call form
        //    //callForm->PostMessage(UM_TEL_START_RECORD, 0, 0);
        //}

        //void PstnTelephone::ring_(void* param) {
        //    if (!callInfo_) {
        //        callInfo_ = new Data::CallInfo();
        //    }
        //    callInfo_->type(Data::citReceived);
        //    if (callInfo_->telephoneNumber() != L"") { //has callid
        //        std::wstring ringtoneName = Contact::Data::ContactCategory::GetRingtone(callInfo_->telephoneNumber());
        //        Contact::Data::SoundSegment(Contact::Data::sstRingtone, ringtoneName).Play();
        //    } else {
        //        RingControl(true);
        //    }
        //    //send ring message to call form
        //    //callForm->PostMessage(UM_TEL_RING, 0, 0);
        //}

        //void PstnTelephone::subDial_(void* param) {
        //    LPARAM keyCode = reinterpret_cast<int>(param);
        //    //send sub-dial message to call form
        //    View::callForm->PostMessage(View::UM_TEL_SUB_DIAL, 0, keyCode);
        //}

        //void PstnTelephone::stopRecord_(void* param) {
        //    callInfo_->StopRecord();
        //    //send stop-record message to call form
        //    //callForm->PostMessage(UM_TEL_STOP_RECORD, 0, 0);
        //}

        //void PstnTelephone::show_(void* param) {
        //    //callForm->ShowWindow(SW_SHOW);
        //}

        //void PstnTelephone::softPickup_(void* param) {
        //    SoftPickup();
        //    //callInfo_ = Util::shared_ptr<CallInfo>(new CallInfo());
        //    ////send pickup message to call form
        //    //callForm->PostMessage(UM_TEL_PICKUP, 0, 0);
        //}

        //void PstnTelephone::recordCallInfo_(void* param) {
        //    //insert call-info to database
        //    if (callInfo_) {
        //        callInfo_->duration(Util::Timestamp::GetCurrentTime() - callInfo_->startTime());
        //        callInfo_->Insert();
        //        callInfo_->telephoneNumber(L"");
        //    }
        //}

        //void PstnTelephone::reject_(void* param) {
        //    SoftHangup();
        //}

        //void PstnTelephone::callinTimeout_(void* param) {
        //    callInfo_->type(Data::citMissed);
        //    //send callin-timeout message to call form
        //    //callForm->PostMessage(UM_TEL_CALLIN_TIMEOUT, 0, 0);
        //}

        //void PstnTelephone::remoteHangup_(void* param) {
        //    //send remote-hangup message to call form
        //    //callForm->PostMessage(UM_TEL_REMOTE_HANGUP, 0, 0);
        //}

        //void PstnTelephone::hangup_(void* param) {
        //    //send hangup message to call form
        //    //callForm->PostMessage(UM_TEL_HANGUP, 0, 0);
        //}

        //void PstnTelephone::callIdTimeout_(void* param) {
        //    callInfo_->type(Data::citMissed);
        //    //send callId-timeout message to call form
        //    //callForm->PostMessage(UM_TEL_CALLID_TIMEOUT, 0, 0);
        //}

        //void PstnTelephone::playPrompt_(void* param) {
        //    if (contact_) {
        //        callInfo_->type(Data::citLeaveWord);
        //        std::wstring prompt = Contact::Data::ContactCategory::GetPrompt(callInfo_->telephoneNumber());
        //        if (prompt != L"") {
        //            if (prompt.find(L".spx")) { //is speex file
        //                Util::OggCodec* oggCodec = Util::GetOggCodec();
        //                oggCodec->StartDecode(Util::StringOp::FromUnicode(prompt));
        //            } else { //is wave file
        //                Util::RingtonePlayer::Instance()->Start(prompt);
        //            }
        //        } else {
        //            Contact::Data::SoundSegment::DefaultPrompt.Play();
        //            //RingtonePlayer::Instance()->Start(defaultPrompt);
        //        }
        //    }
        //}

        //void PstnTelephone::connectTimeout_(void* param) {
        //    //send connect-timeout message to call form
        //    //callForm->PostMessage(UM_TEL_CONNECT_TIMEOUT, 0, 0);
        //}

        //void PstnTelephone::remoteBusy_(void* param) {
        //    //send remote-busy message to call form
        //    //callForm->PostMessage(UM_TEL_REMOTE_BUSY, 0, 0);
        //}

        //void PstnTelephone::dialTimeout_(void* param) {
        //    //send dial-timeout message to call form
        //    //callForm->PostMessage(UM_TEL_DIAL_TIMEOUT, 0, 0);
        //}

        //void PstnTelephone::recordTimeout_(void* param) {
        //    //send record-timeout message to call form
        //    //callForm->PostMessage(UM_TEL_RECORD_TIMEOUT, 0, 0);
        //}
    }
}
