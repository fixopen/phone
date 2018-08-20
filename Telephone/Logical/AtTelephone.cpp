#include <afxwin.h>
#include "AtTelephone.h"
#include "../../Util/HardwareKeypad.h"
#include "../../Util/ATCommandWarp.h"
#include "../../Util/Log.h"
#include "../../Util/Text/StringOp.h"

namespace Telephone {
    namespace Logical {
        AtTelephone::AtTelephone(/*Util::Observable* atChannel, Util::Observable* keypad*/)
        : Util::FSM<AtTelephone>()
        , Util::Observer()
        , videoPort_(0) {
            registerRule(sIdle, ePickup, sPickuping, &AtTelephone::showDialpad_);
            registerRule(sIdle, eRemoteCall, sIncalling, &AtTelephone::showRemoteCallAndRecordCallId_);
            registerRule(sIdle, eSendNumber, sWaitConnecting, &AtTelephone::waitRemoteAnswer_);
            registerRule(sIdle, eVideoDial, sVideoWaitConnecting, &AtTelephone::waitRemoteAnswer_);

            registerRule(sPickuping, eDial, sDialing, &AtTelephone::recordDialCode_);
            registerRule(sPickuping, eSendNumber, sWaitConnecting, &AtTelephone::waitRemoteAnswer_);
            registerRule(sPickuping, eTimeout, sIdle, &AtTelephone::hideDialpad_);
            registerRule(sPickuping, eHangup, sIdle, &AtTelephone::hideDialpad_);

            registerRule(sDialing, eSendNumber, sWaitConnecting, &AtTelephone::waitRemoteAnswer_);
            registerRule(sDialing, eTimeout, sError, &AtTelephone::hideDialpad_);
            registerRule(sDialing, eHangup, sIdle, &AtTelephone::finishWaitRemoteAnswer_);

            registerRule(sWaitConnecting, eRemoteReceived, sConnecting, &AtTelephone::hideDialpadAndShowCallController_);
            registerRule(sWaitConnecting, eTimeout, sError, &AtTelephone::finishWaitRemoteAnswer_);
            registerRule(sWaitConnecting, eHangup, sIdle, &AtTelephone::finishWaitRemoteAnswer_);
            registerRule(sWaitConnecting, eRemoteReject, sRemoteDisconnecting, &AtTelephone::finishWaitRemoteAnswer_);

            registerRule(sVideoWaitConnecting, eRemoteVideoStart, sMediaStarting, &AtTelephone::hideDialpadAndShowMediaController_);
            registerRule(sVideoWaitConnecting, eTimeout, sError, &AtTelephone::finishWaitRemoteAnswer_);
            registerRule(sVideoWaitConnecting, eRemoteReject, sRemoteBusy, &AtTelephone::finishWaitRemoteAnswer_);

            registerRule(sMediaStarting, eVideoStart, sMediaStarting, &AtTelephone::startMedia_);
            registerRule(sMediaStarting, eTimeout, sError, &AtTelephone::finishWaitRemoteAnswer_);
            registerRule(sMediaStarting, eRemoteReject, sRemoteBusy, &AtTelephone::finishWaitRemoteAnswer_);
            registerRule(sMediaStarting, eVideoConsultation, sMediaConsultation, &AtTelephone::consultationMedia_);

            registerRule(sMediaConsultation, eRemoteReject, sRemoteBusy, &AtTelephone::finishWaitRemoteAnswer_);
            registerRule(sMediaConsultation, eRemoteVideoStop, sMediaStoping, &AtTelephone::stopMedia_);
            registerRule(sMediaConsultation, eVideoConsultation, sConnecting);

            registerRule(sMediaStoping, eRemoteHangup, sRemoteBusy, &AtTelephone::recordHistory_);

			registerRule(sConnecting, eStartRecord, sRecording, &AtTelephone::startRecord_);
            registerRule(sConnecting, eHangup, sIdle, &AtTelephone::hideCallControllerAndRecordHistory_);
            registerRule(sConnecting, eRemoteHangup, sRemoteBusy, &AtTelephone::recordHistory_);
            registerRule(sConnecting, eRemoteVideoStop, sMediaStoping, &AtTelephone::stopMedia_);
            registerRule(sConnecting, eVideoStop, sMediaStoping, &AtTelephone::stopMedia_);

            registerRule(sRecording, eStopRecord, sConnecting, &AtTelephone::stopRecordAndSaveFile_);
            registerRule(sRecording, eHangup, sIdle, &AtTelephone::stopRecordAndSaveFileAndHideCallControllerAndRecordHistory_);
            registerRule(sRecording, eRemoteHangup, sIdle, &AtTelephone::stopRecordAndSaveFileAndHideCallControllerAndRecordHistory_);
            registerRule(sRecording, eStorageLow, sConnecting, &AtTelephone::stopRecord_);

            registerRule(sIncalling, eHangup, sConnecting, &AtTelephone::showCallController_);
            registerRule(sIncalling, eLeaveWord, sRecording, &AtTelephone::startRecord_);
            registerRule(sIncalling, eRemoteHangup, sIdle, &AtTelephone::recordHistory_);
            registerRule(sIncalling, eReject, sIdle, &AtTelephone::hideRemoteCallAndRecordHistory_);

            registerRule(sRemoteBusy, eHangup, sIdle, &AtTelephone::hideCallController_);

            setStartState(sIdle);

            Util::ATCommand::Instance()->RegisterObserver(this);
            Util::HardwareKeypad::Instance()->RegisterObserver(this);
        }

        AtTelephone::~AtTelephone() {
            if (videoPort_) {
                videoPort_->Close();
                delete videoPort_;
            }
        }

        void AtTelephone::Update(std::string const& data) {
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
                        //fireEvent(eSentOut, 0);
                        break;
                    case Util::AT::DCPIResponse::tCallProceeding:
                        //call processing
                        break;
                    case Util::AT::DCPIResponse::tChannelAssignment:
                        //channel assignment
                        break;
                    case Util::AT::DCPIResponse::tAlerting:
                        //remote alerting
						fireEvent(eRemoteRing, 0);
                        break;
                    case Util::AT::DCPIResponse::tConnect:
                        //connection
						fireEvent(eConnect, 0);
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
							fireEvent(eRemoteRing, 0);
                            break;
                        case Util::AT::sTerminated:
                            //对方挂机
							fireEvent(eRemoteHangup, 0);
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
							fireEvent(eRemoteHangup, 0);
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
            } else if (data.find("^DVTCODEC") != std::string::npos) {
                Util::AT::DVTCODECResponse c(data);
                switch (c.action) {
                    case Util::AT::DVTCODECResponse::aStart:
                        //setup
						fireEvent(eRemoteVideoStart, 0);
                        break;
                    case Util::AT::DVTCODECResponse::aStop:
                        //teardown
						fireEvent(eRemoteVideoStop, 0);
                        break;
                    case Util::AT::DVTCODECResponse::aConsultation:
                        //consultation
						fireEvent(eRemoteVideoConsultation, 0);
                        break;
                    default:
                        break;
                }
            } else if (data.find("^DVTDATA") != std::string::npos) {
                //require send video data
            } else if (data.find("^DVTRING") != std::string::npos) {
                //terminated
            } else if (data.find("^DVTCLOSED") != std::string::npos) {
                //remote close
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

        void AtTelephone::Dial(std::string const& number) {
            std::string ATD = "ATD" + number + ";\xD"; //\"\"
            Util::ATCommand::Instance()->Write(ATD);
            fireEvent(eDial, 0);
        }

        void AtTelephone::CallLimit() {
            //AT+CLCK=\"%s\",%d,\"0000\",2\xD //limit AI or AO with 0 or 1
        }

        void ReportScenes() {
            //AT^DAUDSCS?\xD //report audio scenes
        }

        void DectectNetworkType() {
            //AT^DACTI?\xD //detect network type
            //AT^DACTI?\xD //network type request
        }

        void AtTelephone::CallWaiting(bool const isTrue) {
            //AT+CCWA=1,mode,class\xD //call waiting, mode: 0-disable, 1-enable, 2-query
            std::string callWaiting = "AT+CCWA=1," + Util::Text::StringOp::ToUTF8(Util::Text::StringOp::FromInt(isTrue));
			Util::ATCommand::Instance()->Write(callWaiting);
        }

        void AtTelephone::CallSwitch() {
            //AT+CHLD=%d\xD //call switch
			std::string callSwitch = "AT_CHLD=" + std::string(); //{"0|1|1X|2|2X|3|4"};
			Util::ATCommand::Instance()->Write(callSwitch);
        }

        void AtTelephone::AutoAnswer(int const secondCount) {
            //ATS0=%d\xD //auto answer
            std::string autoAnswer = "ATS0=" + Util::Text::StringOp::ToUTF8(Util::Text::StringOp::FromInt(secondCount));
            //ATS0=%d\xD //auto answer in %d second
			Util::ATCommand::Instance()->Write(autoAnswer);
        }

        void AtTelephone::Handsfree() {
			//AT^DAUDO=type,gain //1: receiver, 2: speaker, 3: earphone; 0-5
			std::string setOut = "AT^DAUDO=2,2";
			//AT^DAUDI=type,gain //1: microphone, 2: earphone; 0: mute, 1: normal
			std::string setIn = "AT^DAUDI=1,1";
            Util::ATCommand::Instance()->Write(setOut);
            Util::ATCommand::Instance()->Write(setIn);
        }

        namespace {
            void videoDataProcess(unsigned char const* const data, unsigned int const length) {
				std::FILE* file = std::fopen("/FlashDrv/vt.dat", "ab");
				if (file) {
					std::fwrite(data, 1, length, file);
					std::fclose(file);
				}
                //decode video data and show it
            }
        }

        //SetVideoChannel(3);
        void AtTelephone::SetVideoChannel(int const channelNumber) {
            //AT^DVTCHL=1\xD
            videoPort_ = new Util::RS232();
            videoPort_->Open(channelNumber, 0, 0, 0, 0);
            videoPort_->SetReadFunc(videoDataProcess);
            std::string SELCH = "AT^DVTCHL=1" + Util::Text::StringOp::ToUTF8(Util::Text::StringOp::FromInt(channelNumber)) + "\xD"; //set video channel
            Util::ATCommand::Instance()->Write(SELCH, videoPort_);
            //currentAction_ = aSetVideoChannel;
            //actionState_ = sIdle;
        }

        void AtTelephone::VideoDial(std::string const& number) {
            //AT^DUSBPOWER=1\xD
            std::string ATD = "AT^DVTDIAL=" + number + ",,0\xD"; //\"\"
            Util::ATCommand::Instance()->Write(ATD);
            fireEvent(eVideoDial, 0);
        }

        void AtTelephone::VideoChannelSetup() {
            std::string startCodec = "AT^DVTCODEC=1";
            Util::ATCommand::Instance()->Write(startCodec);
        }

        void AtTelephone::VideoChannelTeardown() {
            std::string stopCodec = "AT^DVTCODEC=2";
            Util::ATCommand::Instance()->Write(stopCodec);
        }

        void AtTelephone::VideoChannelConsultation() { //negotiation
            std::string consultationCodec = "AT^DVTCODEC=3,1,0,1,176,144,0,1000,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,10,20,0,0,0,0,0,0,0,0";
            Util::ATCommand::Instance()->Write(consultationCodec);
        }

        void AtTelephone::VideoDataControl(bool const isSend) {
            //AT^DVTSEND=isSend
        }

        void AtTelephone::VideoHangup() {
            std::string sCodec = "AT^DVTEND";
            Util::ATCommand::Instance()->Write(sCodec);
            fireEvent(eHangup, 0);
        }

	    void AtTelephone::Hangup() {
            //AT+CHUP
            //AT+CHLD
	        std::string ATH = "ATH\xD";
            Util::ATCommand::Instance()->Write(ATH);
            fireEvent(eHangup, 0);
        }

	    void AtTelephone::Answer() {
	        std::string ATA = "ATA\xD";
            Util::ATCommand::Instance()->Write(ATA);
            fireEvent(ePickup, 0);
        }

        void AtTelephone::Reject() {
	        std::string ATH = "ATH\xD";
            Util::ATCommand::Instance()->Write(ATH);
            fireEvent(eHangup, 0);
        }

        void AtTelephone::Volume() {
            //AT^DAUDO=1,%d\xD
            //AT^DAUDO=2,%d\xD
            //AT^DAUDO=3,%d\xD
        }

        void AtTelephone::Mute(bool const isTrue) {
            //AT+CMUT=%d\xD
        }

		void SetAudioScenes(int scenesNo) {
            //AT^DAUDSCS=%d\xD //handsfree
            //AT^DAUDSCS=%d\xD //handsfree
		}

        void AtTelephone::SetTransfer(TransferType const type, bool const isEnable) {
            //AT+CCFC=0,%d\xD //unconditional transfer
            //AT+CCFC=1,%d\xD //busy transfer
            //AT+CCFC=2,%d\xD //no reply transfer
            //AT+CCFC=3,%d\xD //no reach transfer
        }

	    void AtTelephone::Redial() {
	        std::string ATDL = "ATDL;\xD";
            Util::ATCommand::Instance()->Write(ATDL);
            fireEvent(eDial, 0);
        }

	    void AtTelephone::SubDial(char const number) {
            std::string cmd = "AT+VTS=";
            cmd += number;
            cmd += "\xD";
            Util::ATCommand::Instance()->Write(cmd);
            fireEvent(eDial, 0);
        }

        void AtTelephone::showDialpad_(void* p) {
            //@@
		}

        void AtTelephone::recordDialCode_(void* p) {
            //@@
        }

        void AtTelephone::waitRemoteAnswer_(void* p) {
			Util::Log::Log("wait remote answer");
		}

        void AtTelephone::hideDialpadAndShowCallController_(void* p) {
            hideDialpad_(p);
            showCallController_(p);
		}

		void AtTelephone::hideDialpadAndShowMediaController_(void* p) {
		}

        void AtTelephone::startRecord_(void* p) {
            //@@
		}

        void AtTelephone::stopRecord_(void* p) {
            //@@
		}

        void AtTelephone::stopRecordAndSaveFile_(void* p) {
            stopRecord_(p);
            saveFile_(p);
		}

        void AtTelephone::hideCallControllerAndRecordHistory_(void* p) {
            hideCallController_(p);
            recordHistory_(p);
		}

        void AtTelephone::showRemoteCallAndRecordCallId_(void* p) {
            showRemoteCall_(p);
            recordCallId_(p);
		}

        void AtTelephone::showCallController_(void* p) {
            //@@
		}

        void AtTelephone::recordHistory_(void* p) {
            //@@
		}

        void AtTelephone::hideCallController_(void* p) {
            //@@
		}

        void AtTelephone::hideDialpad_(void* p) {
            //@@
		}

        void AtTelephone::finishWaitRemoteAnswer_(void* p) {
            //@@
		}

        void AtTelephone::hideRemoteCallAndRecordHistory_(void* p) {
            hideRemoteCall_(p);
            recordHistory_(p);
		}

        void AtTelephone::stopRecordAndSaveFileAndHideCallControllerAndRecordHistory_(void* p) {
            stopRecord_(p);
            saveFile_(p);
            hideCallController_(0);
            recordHistory_(p);
		}

        void AtTelephone::saveFile_(void* p) {
		}

        void AtTelephone::showRemoteCall_(void* p) {
		}

        void AtTelephone::recordCallId_(void* p) {
	        std::string CLCC = "AT+CLCC\xD";
            Util::ATCommand::Instance()->Write(CLCC);
            fireEvent(ePickup, 0);
		}

        void AtTelephone::hideRemoteCall_(void* p) {
		}

		void AtTelephone::startMedia_(void* p) {
			VideoChannelSetup();
		}

		void AtTelephone::stopMedia_(void* p) {
			VideoChannelTeardown();
		}

		void AtTelephone::consultationMedia_(void* p) {
			VideoChannelConsultation();
		}
    }
}
