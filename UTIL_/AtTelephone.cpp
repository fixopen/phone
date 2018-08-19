#include "../Util/StringOp.h"
#include <afxwin.h>
#include "AtTelephone.h"
#include "HardwareKeypad.h"
#include "ATCommandWarp_.h"
//#include "../../Util/Log.h"


namespace Telephone {
    namespace Logical {
        AtTelephone::AtTelephone(/*Util::Observable* atChannel, Util::Observable* keypad*/)
			:Util::Observer()
        /*videoPort_(0) */{

            Util::ATCommand::Instance()->RegisterObserver(this);
//             Util::HardwareKeypad::Instance()->RegisterObserver(this);
        
		}

        AtTelephone::~AtTelephone() {
//             if (videoPort_) {
//                
// 				videoPort_->Close();
//                 delete videoPort_;
// 
//             }
        }

        void AtTelephone::Update(std::string const& data) {
            if (data.length() == 1) { //keypad event notify
                Util::HardwareKeypad::KeyCode c = static_cast<Util::HardwareKeypad::KeyCode>(static_cast<unsigned char>(data[0]));
                switch (c) {
                    case Util::HardwareKeypad::cPickup:
                        break;

                    case Util::HardwareKeypad::cHangup:
                        break;

                    case Util::HardwareKeypad::cUp:
                        break;

                    case Util::HardwareKeypad::cDown:
                        break;

                    case Util::HardwareKeypad::cOne:
                        break;

                    case Util::HardwareKeypad::cTwo:
                        break;

                    case Util::HardwareKeypad::cThree:
                        break;

                    case Util::HardwareKeypad::cFour:
                        break;

                    case Util::HardwareKeypad::cFive:
                        break;
						
                    case Util::HardwareKeypad::cSix:
                        break;

                    case Util::HardwareKeypad::cSeven:
                        break;

                    case Util::HardwareKeypad::cEight:
                        break;
						
                    case Util::HardwareKeypad::cNine:
                        break;
						
                    case Util::HardwareKeypad::cZero:
                        break;

                    case Util::HardwareKeypad::cStar:
                        break;

                    case Util::HardwareKeypad::cPound:
                        break;
						
                    case Util::HardwareKeypad::cHandsfreeEnd:
                        break;
						
                    case Util::HardwareKeypad::cHandsfreeBegin:
                        break;
						
                    case Util::HardwareKeypad::cHandsfreeStart:
                        break;
						
                    case Util::HardwareKeypad::cHandsfreeStop:
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
            } else if (data.find("^DVTCODEC") != std::string::npos) {
                Util::AT::DVTCODECResponse c(data);
                switch (c.action) {
                    case Util::AT::DVTCODECResponse::aStart:
                        //setup
                        break;
                    case Util::AT::DVTCODECResponse::aStop:
                        //teardown
                        break;
                    case Util::AT::DVTCODECResponse::aConsultation:
                        //consultation
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
            std::string callWaiting = "AT+CCWA=1," + Util::StringOp::ToUTF8(Util::StringOp::FromInt(isTrue));
			Util::ATCommand::Instance()->Write(callWaiting);
        }

        void AtTelephone::CallSwitch() {
            //AT+CHLD=%d\xD //call switch
			std::string callSwitch = "AT_CHLD=" + std::string(); //{"0|1|1X|2|2X|3|4"};
			Util::ATCommand::Instance()->Write(callSwitch);
        }

        void AtTelephone::AutoAnswer(int const secondCount) {
            //ATS0=%d\xD //auto answer
            std::string autoAnswer = "ATS0=" + Util::StringOp::ToUTF8(Util::StringOp::FromInt(secondCount));
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
// 				std::FILE* file = std::fopen("/FlashDrv/vt.dat", "ab");
// 				if (file) {
// 					std::fwrite(data, 1, length, file);
// 					std::fclose(file);
//				}
                //decode video data and show it
            }
        }

        //SetVideoChannel(3);
        void AtTelephone::SetVideoChannel(int const channelNumber) {
            //AT^DVTCHL=1\xD
            videoPort_ = new Util::RS232();
//            videoPort_->Open(channelNumber, 0, 0, 0, 0);
            videoPort_->SetReadFunc(videoDataProcess);
            std::string SELCH = "AT^DVTCHL=1" + Util::StringOp::ToUTF8(Util::StringOp::FromInt(channelNumber)) + "\xD"; //set video channel
//            Util::ATCommand::Instance()->Write(SELCH, videoPort_);
          
			//currentAction_ = aSetVideoChannel;
            //actionState_ = sIdle;
        }

        void AtTelephone::VideoDial(std::string const& number) {
            //AT^DUSBPOWER=1\xD
            std::string ATD = "AT^DVTDIAL=" + number + ",,0\xD"; //\"\"
            Util::ATCommand::Instance()->Write(ATD);
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
        }

	    void AtTelephone::Hangup() {
            //AT+CHUP
            //AT+CHLD
	        std::string ATH = "ATH\xD";
            Util::ATCommand::Instance()->Write(ATH);
        }

	    void AtTelephone::Answer() {
	        std::string ATA = "ATA\xD";
            Util::ATCommand::Instance()->Write(ATA);
        }

        void AtTelephone::Reject() {
	        std::string ATH = "ATH\xD";
            Util::ATCommand::Instance()->Write(ATH);
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
        }

	    void AtTelephone::SubDial(char const number) {
            std::string cmd = "AT+VTS=";
            cmd += number;
            cmd += "\xD";
            Util::ATCommand::Instance()->Write(cmd);
        }

        void AtTelephone::showDialpad_(void* p) {
            //@@
		}

        void AtTelephone::recordDialCode_(void* p) {
            //@@
        }

        void AtTelephone::waitRemoteAnswer_(void* p) {
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
