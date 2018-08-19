#include "AtTelephone.h"
#include "../View/CallDialog.h" //for callForm
//#include "../../Util/HardwareKeypad.h"
#include "../../Util/AT/ATCommand.h" //for Dispatcher
#include "../../Util/Log.h" //for Log
#include "../../Util/Thread.h" //for Thread
#include "../../Util/Text/StringOp.h" //for StringOp
#include "../../Util/Video/H263CodecHard.h" //for H263Decoder H263Encoder
#include "../../Util/Video/H264CodecHard.h" //for H264Decoder H264Encoder
#include "../../Util/Video/MP4CodecHard.h" //for MP4Decoder MP4Encoder

namespace Telephone {
    namespace Logical {
        AtTelephone::AtTelephone(/*Util::Observable* atChannel, Util::Observable* keypad*/)
        : Util::FSM<AtTelephone>()
        , Util::Observer()
        , videoPort_(0)
        , decoder_(0)
        , isDecoderInited_(false)
        , encoder_(0)
        , isEncoderInited_(false)
        , localDisplay_(0)
        , remoteDisplay_(0)
        , camera_(0) {
            registerRule(sIdle, ePickup, sPickuping, &AtTelephone::showDialpad_);
            registerRule(sIdle, eDial, sDialing, &AtTelephone::recordDialCode_);
            registerRule(sIdle, eSendCall, sWaitConnecting, &AtTelephone::waitRemoteAnswer_);
            registerRule(sIdle, eSendVideoCall, sWaitVideoConnecting, &AtTelephone::waitRemoteVideoAnswer_);
            registerRule(sIdle, eRemoteCall, sRemoteCalling, &AtTelephone::showRemoteCallAndRecordCallId_);
            registerRule(sIdle, eRemoteVideoCall, sRemoteVideoCalling, &AtTelephone::showRemoteVideoCallAndRecordCallId_);

            registerRule(sPickuping, eDial, sDialing, &AtTelephone::recordDialCode_);
            registerRule(sPickuping, eSendCall, sWaitConnecting, &AtTelephone::waitRemoteAnswer_);
            registerRule(sPickuping, eSendVideoCall, sWaitVideoConnecting, &AtTelephone::waitRemoteVideoAnswer_);
            registerRule(sPickuping, eHangup, sIdle, &AtTelephone::hideDialpad_);
            registerRule(sPickuping, eRemoteCall, sRemoteCalling, &AtTelephone::showRemoteCallAndRecordCallId_);
            registerRule(sPickuping, eRemoteVideoCall, sRemoteVideoCalling, &AtTelephone::showRemoteVideoCallAndRecordCallId_);

            registerRule(sDialing, eDial, sDialing, &AtTelephone::recordDialCode_);
            registerRule(sDialing, eSendCall, sWaitConnecting, &AtTelephone::waitRemoteAnswer_);
            registerRule(sDialing, eSendVideoCall, sWaitVideoConnecting, &AtTelephone::waitRemoteVideoAnswer_);
            registerRule(sDialing, eHangup, sIdle, &AtTelephone::finishWaitRemoteAnswer_);
            registerRule(sDialing, eRemoteCall, sRemoteCalling, &AtTelephone::showRemoteCallAndRecordCallId_);
            registerRule(sDialing, eRemoteVideoCall, sRemoteVideoCalling, &AtTelephone::showRemoteVideoCallAndRecordCallId_);

            registerRule(sWaitConnecting, eRemoteAnswer, sConnecting, &AtTelephone::hideDialpadAndShowCallController_);
            registerRule(sWaitConnecting, eRemoteReject, sRemoteDisconnecting, &AtTelephone::finishWaitRemoteAnswer_);
            registerRule(sWaitConnecting, eHangup, sIdle, &AtTelephone::finishWaitRemoteAnswer_);

            registerRule(sWaitVideoConnecting, eRemoteMediaStart, sMediaConsultation, &AtTelephone::startMedia_);
            registerRule(sWaitVideoConnecting, eRemoteVideoReject, sWaitConnecting, &AtTelephone::autoRedialAudio_);
            registerRule(sWaitVideoConnecting, eRemoteReject, sRemoteDisconnecting, &AtTelephone::finishWaitRemoteAnswer_);
            registerRule(sWaitVideoConnecting, eHangup, sIdle, &AtTelephone::finishWaitRemoteVideoAnswer_);

            registerRule(sConnecting, eStartRecord, sRecording, &AtTelephone::startRecord_);
            registerRule(sConnecting, eHold, sRecording, &AtTelephone::hold_);
            registerRule(sConnecting, eHangup, sIdle, &AtTelephone::hideCallControllerAndRecordHistory_);
            registerRule(sConnecting, eRemoteHangup, sRemoteDisconnecting, &AtTelephone::recordHistory_);

            registerRule(sMediaConsultation, eRemoteMediaConsultation, sVideoConnecting, &AtTelephone::consultationMedia_);
            registerRule(sMediaConsultation, eRemoteHangup, sRemoteDisconnecting, &AtTelephone::hideCallControllerAndRecordHistory_);
            registerRule(sMediaConsultation, eHangup, sIdle, &AtTelephone::hideCallControllerAndRecordHistory_);

            registerRule(sVideoConnecting, eRemoteMediaStop, sRemoteVideoDisconnecting, &AtTelephone::stopMedia_);
            registerRule(sVideoConnecting, eHold, sHolding, &AtTelephone::hold_);
            registerRule(sVideoConnecting, eStartVideoRecord, sVideoRecording, &AtTelephone::startRecord_);
            registerRule(sVideoConnecting, eRemoteHangup, sRemoteDisconnecting, &AtTelephone::hideRemoteCallAndRecordHistory_);
            registerRule(sVideoConnecting, eVideoHangup, sIdle, &AtTelephone::hideRemoteCallAndRecordHistory_);

            registerRule(sRemoteVideoDisconnecting, eRemoteCall, sConnecting, &AtTelephone::answer_);
            registerRule(sRemoteVideoDisconnecting, eRemoteVideoReject, sWaitConnecting, &AtTelephone::stopMediaAndRedial_);
            registerRule(sRemoteVideoDisconnecting, eRemoteVideoHangup, sRemoteDisconnecting, &AtTelephone::recordHistory_);
            registerRule(sRemoteVideoDisconnecting, eRemoteHangup, sRemoteDisconnecting, &AtTelephone::recordHistory_);
            registerRule(sRemoteVideoDisconnecting, eHangup, sIdle, &AtTelephone::recordHistory_);

            registerRule(sRemoteDisconnecting, eHangup, sIdle, &AtTelephone::hideCallController_);
            registerRule(sRemoteDisconnecting, eRemoteMediaStop, sIdle, &AtTelephone::hideCallController_);

            registerRule(sRecording, eHold, sHolding, &AtTelephone::stopRecordAndSaveFile_);
            registerRule(sRecording, eStopRecord, sConnecting, &AtTelephone::stopRecordAndSaveFile_);
            registerRule(sRecording, eHangup, sIdle, &AtTelephone::stopRecordAndSaveFileAndHideCallControllerAndRecordHistory_);
            registerRule(sRecording, eRemoteHangup, sRemoteDisconnecting, &AtTelephone::stopRecordAndSaveFileAndHideCallControllerAndRecordHistory_);
            registerRule(sRecording, eStorageLow, sConnecting, &AtTelephone::stopRecord_);

            registerRule(sVideoRecording, eHold, sHolding, &AtTelephone::stopRecordAndSaveFile_);
            registerRule(sVideoRecording, eStopVideoRecord, sConnecting, &AtTelephone::stopRecordAndSaveFile_);
            registerRule(sVideoRecording, eRemoteMediaStop, sRemoteVideoDisconnecting, &AtTelephone::stopRecordAndSaveFileAndHideCallControllerAndRecordHistory_);
            registerRule(sVideoRecording, eHangup, sIdle, &AtTelephone::stopRecordAndSaveFileAndHideCallControllerAndRecordHistory_);
            registerRule(sVideoRecording, eRemoteHangup, sRemoteDisconnecting, &AtTelephone::stopRecordAndSaveFileAndHideCallControllerAndRecordHistory_);
            registerRule(sVideoRecording, eStorageLow, sVideoConnecting, &AtTelephone::stopRecord_);

            registerRule(sHolding, eUnhold, sConnecting);
            registerRule(sHolding, eUnhold, sVideoConnecting);
            registerRule(sHolding, eUnhold, sRecording);
            registerRule(sHolding, eUnhold, sVideoRecording);
            registerRule(sHolding, eRemoteMediaStop, sRemoteVideoDisconnecting);
            registerRule(sHolding, eHangup, sIdle);
            registerRule(sHolding, eRemoteHangup, sIdle);

            registerRule(sRemoteCalling, eAnswer, sConnecting, &AtTelephone::showCallController_);
            registerRule(sRemoteCalling, eReject, sIdle, &AtTelephone::recordHistory_);
            registerRule(sRemoteCalling, eRemoteHangup, sIdle, &AtTelephone::recordHistory_);

            registerRule(sRemoteVideoCalling, eVideoAnswer, sVideoAnswered);
            registerRule(sRemoteVideoCalling, eVideoReject, sRemoteVideoDisconnecting);
            registerRule(sRemoteVideoCalling, eReject, sIdle, &AtTelephone::recordHistory_);
            registerRule(sRemoteVideoCalling, eRemoteHangup, sIdle, &AtTelephone::recordHistory_);

            registerRule(sVideoAnswered, eRemoteMediaStart, sMediaConsultation, &AtTelephone::startMedia_);
            registerRule(sVideoAnswered, eHangup, sIdle, &AtTelephone::hideRemoteCallAndRecordHistory_);
            registerRule(sVideoAnswered, eRemoteHangup, sRemoteDisconnecting, &AtTelephone::hideRemoteCallAndRecordHistory_);

            setStartState(sIdle);

            Util::AT::Dispatcher::Instance()->RegisterObserver(this);
            //Util::HardwareKeypad::Instance()->RegisterObserver(this);
        }

        AtTelephone::~AtTelephone() {
            if (videoPort_) {
                videoPort_->Close();
                delete videoPort_;
            }
        }

        namespace {
            size_t const MaxBufferLength = 352 * 288 * 3;
            unsigned char yuvBuffer[MaxBufferLength] = {
                0
            };
            size_t encodedSize;
            size_t startPos;
            unsigned char outBuffer[128 * 1024] = {
                0
            };
        }

        void AtTelephone::Update(std::string const & data) {
            std::vector<std::string> statments = Util::Text::StringOp::Split(data, "\xD\xA");
            for (size_t i = 0; i < statments.size(); ++i) {
                if (statments[i].length() == 1) {
                    //keypad event notify
                    //Util::HardwareKeypad::KeyCode c = static_cast<Util::HardwareKeypad::KeyCode>(static_cast<unsigned char>(data[0]));
                    //switch (c) {
                    //    case Util::HardwareKeypad::cPickup:
                    //        Util::Log::Log("摘机");
                    //        break;
                    //    case Util::HardwareKeypad::cHangup:
                    //        Util::Log::Log("挂机");
                    //        break;
                    //    case Util::HardwareKeypad::cUp:
                    //        Util::Log::Log("上");
                    //        break;
                    //    case Util::HardwareKeypad::cDown:
                    //        Util::Log::Log("下");
                    //        break;
                    //    case Util::HardwareKeypad::cOne:
                    //        Util::Log::Log("1");
                    //        break;
                    //    case Util::HardwareKeypad::cTwo:
                    //        Util::Log::Log("2");
                    //        break;
                    //    case Util::HardwareKeypad::cThree:
                    //        Util::Log::Log("3");
                    //        break;
                    //    case Util::HardwareKeypad::cFour:
                    //        Util::Log::Log("4");
                    //        break;
                    //    case Util::HardwareKeypad::cFive:
                    //        Util::Log::Log("5");
                    //        break;
                    //    case Util::HardwareKeypad::cSix:
                    //        Util::Log::Log("6");
                    //        break;
                    //    case Util::HardwareKeypad::cSeven:
                    //        Util::Log::Log("7");
                    //        break;
                    //    case Util::HardwareKeypad::cEight:
                    //        Util::Log::Log("8");
                    //        break;
                    //    case Util::HardwareKeypad::cNine:
                    //        Util::Log::Log("9");
                    //        break;
                    //    case Util::HardwareKeypad::cZero:
                    //        Util::Log::Log("0");
                    //        break;
                    //    case Util::HardwareKeypad::cStar:
                    //        Util::Log::Log("*");
                    //        break;
                    //    case Util::HardwareKeypad::cPound:
                    //        Util::Log::Log("#");
                    //        break;
                    //    case Util::HardwareKeypad::cHandsfreeEnd:
                    //        Util::Log::Log("结束免提");
                    //        break;
                    //    case Util::HardwareKeypad::cHandsfreeBegin:
                    //        Util::Log::Log("免提");
                    //        break;
                    //    case Util::HardwareKeypad::cHandsfreeStart:
                    //        Util::Log::Log("开始免提");
                    //        break;
                    //    case Util::HardwareKeypad::cHandsfreeStop:
                    //        Util::Log::Log("停止免提");
                    //        break;
                    //    default:
                    //        break;
                    //}
                } else if (statments[i].find("^DCPI") != std::string::npos) {
                    Util::AT::DCPIResponse e(statments[i]);
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
                        //fireEvent(eRemoteRing, 0);
                        break;
                    case Util::AT::DCPIResponse::tConnect:
                        //connection
                        fireEvent(eRemoteAnswer, 0);
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
                } else if (statments[i].find("^DSCI") != std::string::npos) {
                    Util::AT::DSCIResponse i(statments[i]);
                    if (i.direction == Util::AT::dOriginated) {
                        //主叫
                        switch (i.state) {
                        case Util::AT::sDialing4O:
                            //被叫已收到
                            break;
                        case Util::AT::sAlerting4O:
                            //被叫已振铃
                            //fireEvent(eRemoteRing, 0);
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
                    } else if (i.direction == Util::AT::dTerminated) {
                          //被叫
                        switch (i.state) {
                        case Util::AT::sIncoming4T:
                            switch (i.mode) {
                            case Util::AT::mVoice:
                                fireEvent(eRemoteCall, reinterpret_cast<void*>(&i.number));
                                break;
                            case Util::AT::mData:
                                break;
                            default:
                                break;
                            }
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
                } else if (statments[i].find("^DVTCODEC") != std::string::npos) {
                    Util::AT::DVTCODECResponse c(statments[i]);
                    switch (c.action) {
                    case Util::AT::DVTCODECResponse::aStart:
                        //setup
                        fireEvent(eRemoteMediaStart, 0);
                        break;
                    case Util::AT::DVTCODECResponse::aStop:
                        //teardown
                        fireEvent(eRemoteMediaStop, 0);
                        break;
                    case Util::AT::DVTCODECResponse::aConsultation:
                        //consultation
                        fireEvent(eRemoteMediaConsultation, &c);
                        break;
                    default:
                        break;
                    }
                } else if (statments[i].find("^DVTDATA") != std::string::npos) {
                      ////require send video data
                      //AtTelephone::Instance()->videoPort_->Write(outBuffer + startPos, 120); //encodedSize
                      //startPos += 120;
                      //if (startPos >= encodedSize) {
                      //    startPos = 0;
                      //}
                } else if (statments[i].find("^DVTRING") != std::string::npos) {
                      //terminated
                      //@@fireEvent(eRemoteVideoCall, reinterpret_cast<void*>(&i.number));
                      fireEvent(eRemoteVideoCall, 0);
                } else if (statments[i].find("^DVTCLOSED") != std::string::npos) {
                      //remote video call close
                      Util::Log::Log(Util::Text::StringOp::FromInt(::GetTickCount()));
                } else if (statments[i].find("+CRING") != std::string::npos) {
                    //Util::AT::Incoming i(data);
                    //+CRING: SYNC
                } else if (statments[i].find("+CLIP") != std::string::npos) {
                    //not receive it : Util::AT::Incoming i(data);
                    //+CLIP: "18811038275",161,"",,"",0
                } else if (statments[i].find("+CLCC")) {
                    //CLCCResponse* p = new CLCCResponse(data);
                    //thisTelephone->fireEvent(eRemoteCall, p);
                    //delete p;
                } else if (statments[i].find("+CPAS")) {
                    //CPASResponse* p = new CPASResponse(data);
                    //thisTelephone->fireEvent(eRemoteCall, p);
                    //delete p;
                } else if (statments[i].find("+CME ERROR")) {
                    //Error p(data);
                    //thisTelephone->fireEvent(eError, &p);
                } else if (statments[i].find("ERROR")) {
                    //Error p(statments[i]);
                    //thisTelephone->fireEvent(eError, &p);
                } else {
                    //not process
                }
            }
        }

        void AtTelephone::Dial(std::string const & number) {
            std::string ATD = "ATD" + number + ";"; //\"\"
            Util::AT::Dispatcher::Instance()->Write(ATD);
            fireEvent(eSendCall, (void*)&number);
        }

        void AtTelephone::Answer() {
            std::string ATA = "ATA";
            Util::AT::Dispatcher::Instance()->Write(ATA);
            fireEvent(eAnswer, 0);
        }

        void AtTelephone::Reject() {
            std::string ATH = "ATH";
            Util::AT::Dispatcher::Instance()->Write(ATH);
            fireEvent(eReject, 0);
        }

        void AtTelephone::Hangup() {
            //AT+CHUP
            //AT+CHLD
            std::string ATH = "ATH";
            Util::AT::Dispatcher::Instance()->Write(ATH);
            fireEvent(eHangup, 0);
        }

        void AtTelephone::VideoDial(std::string const & number) {
            //AT^DUSBPOWER=1\xD
            std::string ATD = "AT^DVTDIAL=" + number + ",,0"; //\"\"
            Util::AT::Dispatcher::Instance()->Write(ATD);
            fireEvent(eSendVideoCall, (void*)&number);
            number_ = number;
        }

        void AtTelephone::VideoAnswer() {
            std::string command = "AT^DVTANS";
            Util::AT::Dispatcher::Instance()->Write(command);
            fireEvent(eVideoAnswer, 0);
        }

        void AtTelephone::VideoReject() {
            std::string command = "AT^DHUP";
            Util::AT::Dispatcher::Instance()->Write(command);
            command = "ATA";
            Util::AT::Dispatcher::Instance()->Write(command);
            fireEvent(eVideoReject, 0);
        }

        void AtTelephone::VideoHangup() {
            std::string command = "AT^DVTEND";
            Util::AT::Dispatcher::Instance()->Write(command);
            command = "AT^DVTCODEC=2";
            Util::AT::Dispatcher::Instance()->Write(command);
            fireEvent(eVideoHangup, 0);

            camera_->Stop();
            camera_->Close();
            delete camera_;
            camera_ = 0;

            remoteDisplay_->Finally();
            delete remoteDisplay_;
            remoteDisplay_ = 0;

            localDisplay_->Finally();
            delete localDisplay_;
            localDisplay_ = 0;

            encoder_->Uninit();
            delete encoder_;
            encoder_ = 0;
            isEncoderInited_ = false;

            decoder_->Uninit();
            delete decoder_;
            decoder_ = 0;
            isDecoderInited_ = false;
        }

        void AtTelephone::SetVideoChannel(int const channelNumber) {
            videoPort_ = new Util::RS232();
            videoPort_->Open(channelNumber, 0, 0, 0, 0);
            videoPort_->SetReadFunc(remoteVideoDataProcess_, 248/*, 256*/);
            //std::string command = "AT^DUSBPOWER=1"; //set video channel " + Util::Text::StringOp::ToUTF8(Util::Text::StringOp::FromInt(channelNumber)) + "
            //Util::AT::Dispatcher::Instance()->Write(command, "", videoPort_);
            std::string command = "AT^DVTCHL=1"; //set video channel " + Util::Text::StringOp::ToUTF8(Util::Text::StringOp::FromInt(channelNumber)) + "
            Util::AT::Dispatcher::Instance()->Write(command, Util::AT::NullResponse(), videoPort_);
        }

        namespace {
            Util::Video::CameraParameters cameraParam;
        }

        void AtTelephone::SetDisplayWindow(HWND const local, HWND const remote) {
            localDisplay_ = new Util::Video::Display();
            localDisplay_->SetParameters(local, true);
            localDisplay_->Init();

            remoteDisplay_ = new Util::Video::Display();
            remoteDisplay_->SetParameters(remote, false);
            remoteDisplay_->Init();
        }

        void AtTelephone::InitCamera() {
            //camera_ = new Util::Video::Camera6410();
            camera_ = new Util::Video::LocalCamera6410();
            cameraParam.width = 176;
            cameraParam.height = 144;
            cameraParam.frameRate = 20;
            cameraParam.sampleCallBack = cameraDataProcess_;
            camera_->SetParameters(cameraParam);
            camera_->Open();

            camera_->Start();

            encoder_ = new Util::Video::H263Encoder();
            Util::Video::VideoParameters params;
            params.width = 176;
            params.height = 144;
            params.framerate = 20;
            params.bitrate_kbps = 10000;
            params.quantIntra = 8;
            params.quantInter = 8;
            params.gopNum = 20;
            encoder_->SetParameters(params);
            encoder_->Init();
            isEncoderInited_ = true;

            decoder_ = new Util::Video::H263Decoder();
            decoder_->Init();
            isDecoderInited_ = true;
        }

        void AtTelephone::SendVideoData(unsigned char const * const data, size_t const length) {
            videoPort_->Write(data, length);
        }

        void AtTelephone::showDialpad_(void* param) {
        }

        void AtTelephone::recordDialCode_(void* param) {
        }

        void AtTelephone::waitRemoteAnswer_(void* param) {
        }

        void AtTelephone::waitRemoteVideoAnswer_(void* param) {
            //@@View::callForm()->PostMessage(View::UM_TEL_SOFTDIAL, 0, 0);
        }

        void AtTelephone::showRemoteCallAndRecordCallId_(void* param) {
        }

        void AtTelephone::showRemoteVideoCallAndRecordCallId_(void* param) {
        }

        void AtTelephone::hideDialpad_(void* param) {
        }

        void AtTelephone::finishWaitRemoteAnswer_(void* param) {
        }

        void AtTelephone::hideDialpadAndShowCallController_(void* param) {
        }

        void AtTelephone::startMedia_(void* param) {
            std::string startCodec = "AT^DVTCODEC=1";
            Util::AT::Dispatcher::Instance()->Write(startCodec, Util::AT::NullResponse());
        }

        void AtTelephone::autoRedialAudio_(void* param) {
            Dial(number_);
        }

        void AtTelephone::finishWaitRemoteVideoAnswer_(void* param) {
        }

        void AtTelephone::startRecord_(void* param) {
        }

        void AtTelephone::hold_(void* param) {
        }

        void AtTelephone::hideCallControllerAndRecordHistory_(void* param) {
        }

        void AtTelephone::recordHistory_(void* param) {
        }

        void AtTelephone::consultationMedia_(void* param) {
            Util::AT::DVTCODECResponse* c = (Util::AT::DVTCODECResponse*)param;

            //std::string consultationCodec = "AT^DVTCODEC=3,1,0,1,176,144,0,1000,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,10,20,0,0,0,0,0,0,0,1";
            std::string consultationCodec = "AT^DVTCODEC=3";
            Util::AT::Dispatcher::Instance()->Write(consultationCodec);

            //Util::Log::Log(Util::Text::StringOp::FromInt(::GetTickCount()));
        }

        void AtTelephone::stopMedia_(void* param) {
            std::string stopCodec = "AT^DVTCODEC=2";
            Util::AT::Dispatcher::Instance()->Write(stopCodec);
        }

        void AtTelephone::hideRemoteCallAndRecordHistory_(void* param) {
        }

        void AtTelephone::answer_(void* param) {
        }

        void AtTelephone::stopMediaAndRedial_(void* param) {
        }

        void AtTelephone::hideCallController_(void* param) {
        }

        void AtTelephone::stopRecordAndSaveFile_(void* param) {
        }

        void AtTelephone::stopRecordAndSaveFileAndHideCallControllerAndRecordHistory_(void* param) {
        }

        void AtTelephone::stopRecord_(void* param) {
        }

        void AtTelephone::showCallController_(void* param) {
        }

        enum FindState {
            fsOuter,
            fsInner,
        };

        namespace {
            unsigned char frameBuffer[8 * 1024] = {
                0
            };
            size_t framePos = 0;
            FindState findState = fsOuter;

            size_t frameCount = 0;

            unsigned char yuvData[96 * 1024] = {
                0
            };

            size_t cameraFrameIndex = 0;
            size_t selectFactor = 10;

#pragma pack(1)
            struct ZeroFlag {
                unsigned char reserved : 2;
                unsigned char number : 5;
                unsigned char flag : 1;
            };

            union Convert {
                Convert(unsigned char chr)
                : data(chr) {
                }
                ZeroFlag zeroFlag;
                unsigned char data;
            };
#pragma pack()

            bool const isZero(ZeroFlag const & right) {
                bool result = false;
                if ((right.number == 0) && (right.flag == 1)) {
                    result = true;
                }
                return result;
            }
        }

        class SendVideoDataThread : public Util::Thread {
        public:
            virtual int const run() {
                while (true) {
                    AtTelephone::Instance()->SendVideoData(outBuffer + startPos, 120); //encodedSize
                    startPos += 120;
                    if (startPos >= encodedSize) {
                        startPos = 0;
                    }
                    ::Sleep(20);
                }
            }
        };

        void AtTelephone::remoteVideoDataProcess_(unsigned char const * const data, unsigned int const length) {
            //AtTelephone::Instance()->videoPort_->Write(data, length);
            //AtTelephone::Instance()->receiveDataSize_.push_back(length);
            //AtTelephone::Instance()->receiveTicks_.push_back(::GetTickCount());
            //Util::Log::Log(Util::Text::StringOp::FromInt(length));
            //Util::Log::Log(Util::Text::StringOp::FromInt(::GetTickCount()));
            frameCount = 0;
            findState = fsOuter;
            size_t begin = 0;
            size_t pos = 0;
            while (pos < length) {
                if ((data[pos] == 0) && (data[pos + 1] == 0) && isZero(Convert(data[pos + 2]).zeroFlag)) {
                    switch (findState) {
                        case fsOuter:
                            findState = fsInner;
                            break;
                        case fsInner:
                            if (true) {
                                ++frameCount;
                                size_t len = MaxBufferLength;
                                Util::Video::VideoParameters params;
                                if (AtTelephone::Instance()->decoder_) {
                                    AtTelephone::Instance()->decoder_->Decode(data + begin, pos - begin, yuvData, len);
                                    AtTelephone::Instance()->decoder_->GetParameters(params);
                                    //FILE* file = 0;
                                    //fopen_s(&file, "FlashDrv/videoData.dat", "ab");
                                    //if (file) {
                                    //    fwrite(data, 1, length, file);
                                    //    fclose(file);
                                    //}
                                    //DWORD tickCount = ::GetTickCount();
                                    //Util::Log::Log(Util::Text::StringOp::FromInt(tickCount));
                                }
                                if (AtTelephone::Instance()->remoteDisplay_) {
                                    AtTelephone::Instance()->remoteDisplay_->Show(Util::Video::YUV420P, params.width, params.height, yuvData);
                                    //DWORD tickCount = ::GetTickCount();
                                    //Util::Log::Log(Util::Text::StringOp::FromInt(tickCount));
                                }
                                ::Sleep(6);
                            }
                            break;
                    }
                    begin = pos;
                }
                ++pos;
            }
            Util::Log::Log(Util::Text::StringOp::FromInt(frameCount));
            //size_t len = MaxBufferLength;
            //Util::Video::VideoParameters params;
            //if (AtTelephone::Instance()->isDecoderInited_) {
            //    AtTelephone::Instance()->decoder_->Decode(data, length, yuvData, len);
            //    AtTelephone::Instance()->decoder_->GetParameters(params);
            //    FILE* file = 0;
            //    fopen_s(&file, "FlashDrv/videoData.dat", "ab");
            //    if (file) {
            //        fwrite(data, 1, length, file);
            //        fclose(file);
            //    }
            //    //DWORD tickCount = ::GetTickCount();
            //    //Util::Log::Log(Util::Text::StringOp::FromInt(tickCount));
            //}
            //if (AtTelephone::Instance()->remoteDisplay_) {
            //    AtTelephone::Instance()->remoteDisplay_->Show(Util::Video::YUV420P, params.width, params.height, yuvData);
            //    //DWORD tickCount = ::GetTickCount();
            //    //Util::Log::Log(Util::Text::StringOp::FromInt(tickCount));
            //}
        }

        void AtTelephone::cameraDataProcess_(unsigned char const * const data, unsigned int const length) {
            //Util::Log::Log(Util::Text::StringOp::FromInt(::GetTickCount()));
            //Util::Log::Log(Util::Text::StringOp::FromInt(::GetThreadPriority(::GetCurrentThread())));
            AtTelephone::Instance()->localDisplay_->Show(Util::Video::YUV420P, cameraParam.width, cameraParam.height, data);
            if (cameraFrameIndex % selectFactor == 0) {
                size_t outBufferSize = 16 * 1024;
                size_t encodedSize = 0;
                //Util::Video::YUVConvert::YVU420PtoYUV420P(data, cameraParam.width, cameraParam.height, yuvBuffer);
                if (AtTelephone::Instance()->isEncoderInited_) {
                    AtTelephone::Instance()->encoder_->Encode(yuvBuffer, outBuffer, outBufferSize, encodedSize);
                    //if (AtTelephone::Instance()->decoder_) {
                    //    AtTelephone::Instance()->decoder_->Decode(outBuffer, encodedSize, yuvBuffer, outBufferSize);
                    //}
                    //AtTelephone::Instance()->ticks_.push_back(::GetTickCount());
                    //AtTelephone::Instance()->cameraDataSize_.push_back(encodedSize);
                    //Util::Log::Log(Util::Text::StringOp::FromInt(::GetTickCount()));
                    //Util::Log::Log(Util::Text::StringOp::FromInt(encodedSize));
                    AtTelephone::Instance()->videoPort_->Write(outBuffer, encodedSize);
                }
                //AtTelephone::Instance()->encoder_->Encode(yuvBuffer, outBuffer + startPos, outBufferSize - startPos, encodedSize);
                //startPos += encodedSize;
                //if (cameraFrameIndex % (selectFactor * cameraParam.frameRate) == 0) {
                //    AtTelephone::Instance()->videoPort_->Write(outBuffer, encodedSize);
                //    //SendVideoDataThread* sendThread = new SendVideoDataThread();
                //    //sendThread->start();
                //    startPos = 0;
                //}
            }
            ++cameraFrameIndex;
        }

        void AtTelephone::CallLimit() {
            //AT+CLCK=\"%s\",%d,\"0000\",2\xD //limit AI or AO with 0 or 1
        }

        void AtTelephone::ReportScenes() {
            //AT^DAUDSCS?\xD //report audio scenes
        }

        void AtTelephone::DectectNetworkType() {
            //AT^DACTI?\xD //detect network type
            //AT^DACTI?\xD //network type request
        }

        void AtTelephone::CallWaiting(bool const isTrue) {
            //AT+CCWA=1,mode,class\xD //call waiting, mode: 0-disable, 1-enable, 2-query
            std::string callWaiting = "AT+CCWA=1," + Util::Text::StringOp::ToUTF8(Util::Text::StringOp::FromInt(isTrue));
            Util::AT::Dispatcher::Instance()->Write(callWaiting);
        }

        void AtTelephone::CallSwitch() {
            //AT+CHLD=%d\xD //call switch
            std::string callSwitch = "AT+CHLD=" + std::string(); //{"0|1|1X|2|2X|3|4"};
            Util::AT::Dispatcher::Instance()->Write(callSwitch);
        }

        void AtTelephone::AutoAnswer(int const secondCount) {
            //ATS0=%d\xD //auto answer
            std::string autoAnswer = "ATS0=" + Util::Text::StringOp::ToUTF8(Util::Text::StringOp::FromInt(secondCount));
            //ATS0=%d\xD //auto answer in %d second
            Util::AT::Dispatcher::Instance()->Write(autoAnswer);
        }

        void AtTelephone::Handsfree() {
            //AT^DAUDO=type,gain //1: receiver, 2: speaker, 3: earphone; 0-5
            std::string setOut = "AT^DAUDO=2,2";
            //AT^DAUDI=type,gain //1: microphone, 2: earphone; 0: mute, 1: normal
            std::string setIn = "AT^DAUDI=1,1";
            Util::AT::Dispatcher::Instance()->Write(setOut);
            Util::AT::Dispatcher::Instance()->Write(setIn);
        }

        void AtTelephone::VideoDataControl(bool const isSend) {
            //AT^DVTSEND=isSend
            std::string command = "AT^DVTSEND=" + Util::Text::StringOp::ToUTF8(Util::Text::StringOp::FromInt(isSend)) + "";
            Util::AT::Dispatcher::Instance()->Write(command);
        }

        void AtTelephone::Volume() {
            //AT^DAUDO=1,%d\xD
            //AT^DAUDO=2,%d\xD
            //AT^DAUDO=3,%d\xD
        }

        void AtTelephone::Mute(bool const isTrue) {
            //AT+CMUT=%d\xD
            std::string command = "AT+CMUT=" + Util::Text::StringOp::ToUTF8(Util::Text::StringOp::FromInt(isTrue)) + "";
            Util::AT::Dispatcher::Instance()->Write(command);
        }

        void AtTelephone::SetAudioScenes(int scenesNo) {
            //AT^DAUDSCS=%d\xD //handsfree
            std::string command = "AT^DAUDSCS=" + Util::Text::StringOp::ToUTF8(Util::Text::StringOp::FromInt(scenesNo)) + "";
            Util::AT::Dispatcher::Instance()->Write(command);
        }

        void AtTelephone::SetTransfer(TransferType const type, TransferAction const action, std::string const number, size_t const timeout) {
            //AT_CCFC=reason,mode,number,type,class,subaddr,subaddrtype,time
            //reason: 0, 1, 2, 3, 4, 5, 6 unconditional, busy, no-reply, no-reach, all-call-forward, all-call-cond-forward, ZTE-only
            //mode: 0, 1, 2, 3, 4 disable, enable, query-status, registration, erasure
            //class: 1, 2, 4, 5, 7, 8, 12, 13, 16, 32, 247, 248 voice, data, fax, all-but-sms, default, sms, all-data-service, all-service, data-circuit-sync, data-circuit-async, all-sync-service, all-async-service
            //time: 1..30
        }

        void AtTelephone::Redial() {
            std::string ATDL = "ATDL;";
            Util::AT::Dispatcher::Instance()->Write(ATDL);
            fireEvent(eDial, 0);
        }

        void AtTelephone::SubDial(char const number) {
            std::string cmd = "AT+VTS=";
            cmd += number;
            cmd += "";
            Util::AT::Dispatcher::Instance()->Write(cmd);
            fireEvent(eDial, 0);
        }
    }
}
