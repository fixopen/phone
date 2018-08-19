#include <afxwin.h>
#include "SipTelephone.h"
#include "../../Util/HardwareKeypad.h"
#include "../../Util/AT/ATCommand.h"
#include "../../Util/Log.h"

namespace Telephone {
    namespace Logical {
        SipTelephone::SipTelephone(/*Util::Observable* atChannel, Util::Observable* keypad*/)
        : Util::FSM<SipTelephone>()
        , Util::Observer() {
            registerRule(sIdle, ePickup, sPickuping, &SipTelephone::showDialpad_);
            registerRule(sIdle, eRemoteCall, sIncalling, &SipTelephone::showRemoteCallAndRecordCallId_);
            registerRule(sIdle, eSendNumber, sWaitConnecting, &SipTelephone::waitRemoteAnswer_);

            registerRule(sPickuping, eDial, sDialing, &SipTelephone::recordDialCode_);
            registerRule(sPickuping, eSendNumber, sWaitConnecting, &SipTelephone::waitRemoteAnswer_);
            registerRule(sPickuping, eTimeout, sIdle, &SipTelephone::hideDialpad_);
            registerRule(sPickuping, eHangup, sIdle, &SipTelephone::hideDialpad_);

            registerRule(sDialing, eSendNumber, sWaitConnecting, &SipTelephone::waitRemoteAnswer_);
            registerRule(sDialing, eTimeout, sError, &SipTelephone::hideDialpad_);
            registerRule(sDialing, eHangup, sIdle, &SipTelephone::finishWaitRemoteAnswer_);

            registerRule(sWaitConnecting, eRemotePickup, sConnecting, &SipTelephone::hideDialpadAndShowCallController_);
            registerRule(sWaitConnecting, eTimeout, sError, &SipTelephone::finishWaitRemoteAnswer_);
            registerRule(sWaitConnecting, eHangup, sIdle, &SipTelephone::finishWaitRemoteAnswer_);
            registerRule(sWaitConnecting, eRemoteReject, sRemoteBusy, &SipTelephone::finishWaitRemoteAnswer_);

            registerRule(sConnecting, eStartRecord, sRecording, &SipTelephone::startRecord_);
            registerRule(sConnecting, eHangup, sIdle, &SipTelephone::hideCallControllerAndRecordHistory_);
            registerRule(sConnecting, eRemoteHangup, sRemoteBusy, &SipTelephone::recordHistory_);

            registerRule(sRecording, eStopRecord, sConnecting, &SipTelephone::stopRecordAndSaveFile_);
            registerRule(sRecording, eHangup, sIdle, &SipTelephone::stopRecordAndSaveFileAndHideCallControllerAndRecordHistory_);
            registerRule(sRecording, eRemoteHangup, sIdle, &SipTelephone::stopRecordAndSaveFileAndHideCallControllerAndRecordHistory_);
            registerRule(sRecording, eStorageLow, sConnecting, &SipTelephone::stopRecord_);

            registerRule(sIncalling, eHangup, sConnecting, &SipTelephone::showCallController_);
            registerRule(sIncalling, eLeaveWord, sRecording, &SipTelephone::startRecord_);
            registerRule(sIncalling, eRemoteHangup, sIdle, &SipTelephone::recordHistory_);
            registerRule(sIncalling, eReject, sIdle, &SipTelephone::hideRemoteCallAndRecordHistory_);

            registerRule(sRemoteBusy, eHangup, sIdle, &SipTelephone::hideCallController_);

            setStartState(sIdle);

            Util::AT::Dispatcher::Instance()->RegisterObserver(this);
            Util::HardwareKeypad::Instance()->RegisterObserver(this);
        }

        void SipTelephone::Update(std::string const & data) {
            if (data.length() == 1) {
                //keypad event notify
                Util::HardwareKeypad::KeyCode c = static_cast<Util::HardwareKeypad::KeyCode>(static_cast<unsigned char>(data[0]));
                switch (c) {
                case Util::HardwareKeypad::cPickup:
                    Util::Log::Log("摘机");
                    break;
                case Util::HardwareKeypad::cHangup:
                    Util::Log::Log("挂机");
                    break;
                case Util::HardwareKeypad::cUp:
                    Util::Log::Log("上");
                    break;
                case Util::HardwareKeypad::cDown:
                    Util::Log::Log("下");
                    break;
                case Util::HardwareKeypad::cOne:
                    Util::Log::Log("1");
                    break;
                case Util::HardwareKeypad::cTwo:
                    Util::Log::Log("2");
                    break;
                case Util::HardwareKeypad::cThree:
                    Util::Log::Log("3");
                    break;
                case Util::HardwareKeypad::cFour:
                    Util::Log::Log("4");
                    break;
                case Util::HardwareKeypad::cFive:
                    Util::Log::Log("5");
                    break;
                case Util::HardwareKeypad::cSix:
                    Util::Log::Log("6");
                    break;
                case Util::HardwareKeypad::cSeven:
                    Util::Log::Log("7");
                    break;
                case Util::HardwareKeypad::cEight:
                    Util::Log::Log("8");
                    break;
                case Util::HardwareKeypad::cNine:
                    Util::Log::Log("9");
                    break;
                case Util::HardwareKeypad::cZero:
                    Util::Log::Log("0");
                    break;
                case Util::HardwareKeypad::cStar:
                    Util::Log::Log("*");
                    break;
                case Util::HardwareKeypad::cPound:
                    Util::Log::Log("#");
                    break;
                case Util::HardwareKeypad::cHandsfreeEnd:
                    Util::Log::Log("结束免提");
                    break;
                case Util::HardwareKeypad::cHandsfreeBegin:
                    Util::Log::Log("免提");
                    break;
                case Util::HardwareKeypad::cHandsfreeStart:
                    Util::Log::Log("开始免提");
                    break;
                case Util::HardwareKeypad::cHandsfreeStop:
                    Util::Log::Log("停止免提");
                    break;
                default:
                    break;
                }
            } else if (data.find("^DCPI") != std::string::npos) {
                Util::AT::DCPIResponse e(data);
                switch (e.type) {
                case Util::AT::DCPIResponse::tSetup:
                    //sent out
                    //fireEvent(sentOut, 0);
                    break;
                case Util::AT::DCPIResponse::tCallProceeding:
                    //call processing
                    break;
                case Util::AT::DCPIResponse::tChannelAssignment:
                    //channel assignment
                    break;
                case Util::AT::DCPIResponse::tAlerting:
                    //remote alerting
                    break;
                case Util::AT::DCPIResponse::tConnect:
                    //connection
                    break;
                case Util::AT::DCPIResponse::tDisconnect:
                    //disconnection
                    break;
                case Util::AT::DCPIResponse::tProgressDescription:
                    //progress description
                    break;
                default:
                    break;
                }
            } else if (data.find("^DSCI") != std::string::npos) {
                  Util::AT::DSCIResponse i(data);
                if (i.direction == Util::AT::dOriginated) {
                    //主叫
                    switch (i.state) {
                    case Util::AT::sDialing4O:
                        //被叫已收到
                        break;
                    case Util::AT::sAlerting4O:
                        //被叫已振铃
                        break;
                    case Util::AT::sTerminated:
                        //对方挂机
                        break;
                    case Util::AT::sWaiting4T:
                        break;
                    case Util::AT::sHeld:
                        break;
                    case Util::AT::sActive:
                        break;
                    case Util::AT::sIncoming4T:
                        //来电
                        break;
                    default:
                        break;
                    }
                } else if (i.direction == Util::AT::dTerminated) {
                      //被叫
                    switch (i.state) {
                    case Util::AT::sIncoming4T:
                        fireEvent(eRemoteCall, reinterpret_cast<void*>(&i.number));
                        //来电
                        break;
                    case Util::AT::sTerminated:
                        //对方挂机
                        break;
                    case Util::AT::sHeld:
                        break;
                    case Util::AT::sActive:
                        break;
                    case Util::AT::sDialing4O:
                        break;
                    case Util::AT::sAlerting4O:
                        break;
                    case Util::AT::sWaiting4T:
                        break;
                    default:
                        break;
                    }
                } else {
                    //error
                }
            } else if (data.find("+CRING") != std::string::npos) {
                //not receive it : Util::AT::Incoming i(data);
            } else if (data.find("+CLIP") != std::string::npos) {
                //not receive it : Util::AT::Incoming i(data);
            } else if (data.find("+CLCC")) {
                //CLCCResponse* p = new CLCCResponse(data);
                //thisTelephone->fireEvent(eRemoteCall, p);
                //delete p;
            } else if (data.find("+CPAS")) {
                //CPASResponse* p = new CPASResponse(data);
                //thisTelephone->fireEvent(eRemoteCall, p);
                //delete p;
            } else if (data.find("+CME ERROR")) {
                //Error* p = new Error(data);
                //thisTelephone->fireEvent(eError, p);
                //delete p;
            } else if (data.find("ERROR")) {
                //Error* p = new Error(data);
                //thisTelephone->fireEvent(eError, p);
                //delete p;
            } else {
                //not process
            }
        }

        void SipTelephone::Dial(std::string const & number) {
            std::string ATD = "ATD" + number + ";"; //\"\"
            Util::AT::Dispatcher::Instance()->Write(ATD);
            fireEvent(eDial, 0);
        }

        void SipTelephone::VideoDial(std::string const & number) {
            std::string ATD = "AT^DVTDIAL=" + number + ",,0"; //\"\"
            Util::AT::Dispatcher::Instance()->Write(ATD);
            fireEvent(eVideoDial, 0);
        }

        void SipTelephone::VideoChannelSetup() {
            //AT^DVTCODEC=1
            //AT^DVTCODEC=3
        }

        void SipTelephone::Hangup() {
            //AT+CHUP
            //AT+CHLD
            std::string ATH = "ATH";
            Util::AT::Dispatcher::Instance()->Write(ATH);
            fireEvent(eHangup, 0);
        }

        void SipTelephone::Answer() {
            std::string ATA = "ATA";
            Util::AT::Dispatcher::Instance()->Write(ATA);
            fireEvent(ePickup, 0);
        }

        void SipTelephone::Reject() {
        }

        void SipTelephone::Redial() {
            std::string ATDL = "ATDL;";
            Util::AT::Dispatcher::Instance()->Write(ATDL);
            fireEvent(eDial, 0);
        }

        void SipTelephone::SubDial(char const number) {
            std::string cmd = "AT+VTS=";
            cmd += number;
            cmd += "";
            Util::AT::Dispatcher::Instance()->Write(cmd);
            fireEvent(eDial, 0);
        }

        void SipTelephone::showDialpad_(void* p) {
            //@@
        }

        void SipTelephone::recordDialCode_(void* p) {
            //@@
        }

        void SipTelephone::waitRemoteAnswer_(void* p) {
            //@@
        }

        void SipTelephone::hideDialpadAndShowCallController_(void* p) {
            hideDialpad_(p);
            showCallController_(p);
        }

        void SipTelephone::startRecord_(void* p) {
            //@@
        }

        void SipTelephone::stopRecord_(void* p) {
            //@@
        }

        void SipTelephone::stopRecordAndSaveFile_(void* p) {
            stopRecord_(p);
            saveFile_(p);
        }

        void SipTelephone::hideCallControllerAndRecordHistory_(void* p) {
            hideCallController_(p);
            recordHistory_(p);
        }

        void SipTelephone::showRemoteCallAndRecordCallId_(void* p) {
            showRemoteCall_(p);
            recordCallId_(p);
        }

        void SipTelephone::showCallController_(void* p) {
            //@@
        }

        void SipTelephone::recordHistory_(void* p) {
            //@@
        }

        void SipTelephone::hideCallController_(void* p) {
            //@@
        }

        void SipTelephone::hideDialpad_(void* p) {
            //@@
        }

        void SipTelephone::finishWaitRemoteAnswer_(void* p) {
            //@@
        }

        void SipTelephone::hideRemoteCallAndRecordHistory_(void* p) {
            hideRemoteCall_(p);
            recordHistory_(p);
        }

        void SipTelephone::stopRecordAndSaveFileAndHideCallControllerAndRecordHistory_(void* p) {
            stopRecord_(p);
            saveFile_(p);
            hideCallController_(0);
            recordHistory_(p);
        }

        void SipTelephone::saveFile_(void* p) {
        }

        void SipTelephone::showRemoteCall_(void* p) {
        }

        void SipTelephone::recordCallId_(void* p) {
            std::string CLCC = "AT+CLCC";
            Util::AT::Dispatcher::Instance()->Write(CLCC);
            fireEvent(ePickup, 0);
        }

        void SipTelephone::hideRemoteCall_(void* p) {
        }
    }
}
