#include <afxwin.h>
#include "TelephoneWarp.h"
#include "../../Util/HardwareKeypad.h"
#include "../../Util/ATCommandWarp.h"
#include "../../Util/Log.h"

namespace Telephone {
    namespace Logical {
        TelephoneWarp::TelephoneWarp(/*Util::Observable* atChannel, Util::Observable* keypad*/)
        : Util::FSM<TelephoneWarp>()
        , Util::Observer() {
            registerRule(sIdle, ePickup, sPickuping, &TelephoneWarp::showDialpad_);
            registerRule(sIdle, eRemoteCall, sIncalling, &TelephoneWarp::showRemoteCallAndRecordCallId_);
            registerRule(sIdle, eSendNumber, sWaitConnecting, &TelephoneWarp::waitRemoteAnswer_);

            registerRule(sPickuping, eDial, sDialing, &TelephoneWarp::recordDialCode_);
            registerRule(sPickuping, eSendNumber, sWaitConnecting, &TelephoneWarp::waitRemoteAnswer_);
            registerRule(sPickuping, eTimeout, sIdle, &TelephoneWarp::hideDialpad_);
            registerRule(sPickuping, eHangup, sIdle, &TelephoneWarp::hideDialpad_);

            registerRule(sDialing, eSendNumber, sWaitConnecting, &TelephoneWarp::waitRemoteAnswer_);
            registerRule(sDialing, eTimeout, sError, &TelephoneWarp::hideDialpad_);
            registerRule(sDialing, eHangup, sIdle, &TelephoneWarp::finishWaitRemoteAnswer_);

            registerRule(sWaitConnecting, eRemotePickup, sConnecting, &TelephoneWarp::hideDialpadAndShowCallController_);
            registerRule(sWaitConnecting, eTimeout, sError, &TelephoneWarp::finishWaitRemoteAnswer_);
            registerRule(sWaitConnecting, eHangup, sIdle, &TelephoneWarp::finishWaitRemoteAnswer_);
            registerRule(sWaitConnecting, eRemoteReject, sRemoteBusy, &TelephoneWarp::finishWaitRemoteAnswer_);

            registerRule(sConnecting, eStartRecord, sRecording, &TelephoneWarp::startRecord_);
            registerRule(sConnecting, eHangup, sIdle, &TelephoneWarp::hideCallControllerAndRecordHistory_);
            registerRule(sConnecting, eRemoteHangup, sRemoteBusy, &TelephoneWarp::recordHistory_);

            registerRule(sRecording, eStopRecord, sConnecting, &TelephoneWarp::stopRecordAndSaveFile_);
            registerRule(sRecording, eHangup, sIdle, &TelephoneWarp::stopRecordAndSaveFileAndHideCallControllerAndRecordHistory_);
            registerRule(sRecording, eRemoteHangup, sIdle, &TelephoneWarp::stopRecordAndSaveFileAndHideCallControllerAndRecordHistory_);
            registerRule(sRecording, eStorageLow, sConnecting, &TelephoneWarp::stopRecord_);

            registerRule(sIncalling, eHangup, sConnecting, &TelephoneWarp::showCallController_);
            registerRule(sIncalling, eLeaveWord, sRecording, &TelephoneWarp::startRecord_);
            registerRule(sIncalling, eRemoteHangup, sIdle, &TelephoneWarp::recordHistory_);
            registerRule(sIncalling, eReject, sIdle, &TelephoneWarp::hideRemoteCallAndRecordHistory_);

            registerRule(sRemoteBusy, eHangup, sIdle, &TelephoneWarp::hideCallController_);

            setStartState(sIdle);

            Util::ATCommand::Instance()->RegisterObserver(this);
            Util::HardwareKeypad::Instance()->RegisterObserver(this);
        }

        void TelephoneWarp::Update(std::string const& data) {
            if (data.length() == 1) { //keypad event notify
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
            }
            else if (data.find("^DCPI") != std::string::npos) {
                Util::AT::DCPIResponse e(data);
                switch (e.type) {
                    case Util::AT::DCPIResponse::tSetup:
                        //sent out
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
                if (i.direction == Util::AT::dOriginated) { //主叫
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
                } else if (i.direction == Util::AT::dTerminated) { //被叫
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

        void TelephoneWarp::Dial(std::string const& number) {
            std::string ATD = "ATD" + number + ";\xD"; //\"\"
            Util::ATCommand::Instance()->Write(ATD);
            fireEvent(eDial, 0);
        }

        void TelephoneWarp::VideoDial(std::string const& number) {
            std::string ATD = "AT^DVTDIAL=" + number + ",,0\xD"; //\"\"
            Util::ATCommand::Instance()->Write(ATD);
            fireEvent(eVideoDial, 0);
        }

        void TelephoneWarp::VideoChannelSetup() {
            //AT^DVTCODEC=1
            //AT^DVTCODEC=3
        }

	    void TelephoneWarp::Hangup() {
            //AT+CHUP
            //AT+CHLD
	        std::string ATH = "ATH\xD";
            Util::ATCommand::Instance()->Write(ATH);
            fireEvent(eHangup, 0);
        }

	    void TelephoneWarp::Answer() {
	        std::string ATA = "ATA\xD";
            Util::ATCommand::Instance()->Write(ATA);
            fireEvent(ePickup, 0);
        }

        void TelephoneWarp::Reject() {
        }

	    void TelephoneWarp::Redial() {
	        std::string ATDL = "ATDL;\xD";
            Util::ATCommand::Instance()->Write(ATDL);
            fireEvent(eDial, 0);
        }

	    void TelephoneWarp::SubDial(char const number) {
            std::string cmd = "AT+VTS=";
            cmd += number;
            cmd += "\xD";
            Util::ATCommand::Instance()->Write(cmd);
            fireEvent(eDial, 0);
        }

        void TelephoneWarp::showDialpad_(void* p) {
            showDialpad_(p);
		}

        void TelephoneWarp::recordDialCode_(void* p) {
            //@@
        }

        void TelephoneWarp::waitRemoteAnswer_(void* p) {
            waitRemoteAnswer_(p);
		}

        void TelephoneWarp::hideDialpadAndShowCallController_(void* p) {
            hideDialpad_(p);
            showCallController_(p);
		}

        void TelephoneWarp::startRecord_(void* p) {
            //@@
		}

        void TelephoneWarp::stopRecord_(void* p) {
            //@@
		}

        void TelephoneWarp::stopRecordAndSaveFile_(void* p) {
            stopRecord_(p);
            saveFile_(p);
		}

        void TelephoneWarp::hideCallControllerAndRecordHistory_(void* p) {
            hideCallController_(p);
            recordHistory_(p);
		}

        void TelephoneWarp::showRemoteCallAndRecordCallId_(void* p) {
            showRemoteCall_(p);
            recordCallId_(p);
		}

        void TelephoneWarp::showCallController_(void* p) {
            showCallController_(p);
		}

        void TelephoneWarp::recordHistory_(void* p) {
            recordHistory_(p);
		}

        void TelephoneWarp::hideCallController_(void* p) {
            hideCallController_(p);
		}

        void TelephoneWarp::hideDialpad_(void* p) {
            hideDialpad_(p);
		}

        void TelephoneWarp::finishWaitRemoteAnswer_(void* p) {
            //@@
		}

        void TelephoneWarp::hideRemoteCallAndRecordHistory_(void* p) {
            hideRemoteCall_(p);
            recordHistory_(p);
		}

        void TelephoneWarp::stopRecordAndSaveFileAndHideCallControllerAndRecordHistory_(void* p) {
            stopRecord_(p);
            saveFile_(p);
            hideCallController_(0);
            recordHistory_(p);
		}

        void TelephoneWarp::saveFile_(void* p) {
		}

        void TelephoneWarp::showRemoteCall_(void* p) {
		}

        void TelephoneWarp::recordCallId_(void* p) {
	        std::string CLCC = "AT+CLCC\xD";
            Util::ATCommand::Instance()->Write(CLCC);
            fireEvent(ePickup, 0);
		}

        void TelephoneWarp::hideRemoteCall_(void* p) {
		}
    }
}
