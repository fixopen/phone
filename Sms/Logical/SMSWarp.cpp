#include <afxwin.h>
#include <vector>
#include "SMSWarp.h"
//#include "SMSParser.h"
#include "../../Util/HardwareKeypad.h"

namespace SMS {
    namespace Logical {
        SMSWarp::SMSWarp()
        : Util::FSM<SMSWarp>()
        , Util::Observer()
        , Util::Thread()
        , isQuit_(false) {
            //registerRule(sIdle, ePickup, sPickuping, &TelephoneWarp::showDialpad);
            //registerRule(sIdle, eRemoteCall, sIncalling, &TelephoneWarp::showRemoteCallAndRecordCallId);
            //registerRule(sPickuping, eDial, sDialing, &TelephoneWarp::waitRemoteAnswer);
            //registerRule(sPickuping, eHangup, sIdle, &TelephoneWarp::hideDialpad);
            //registerRule(sDialing, eRemotePickup, sConnecting, &TelephoneWarp::hideDialpadAndShowCallController);
            //registerRule(sDialing, eHangup, sIdle, &TelephoneWarp::finishWaitRemoteAnswer);
            //registerRule(sDialing, eRemoteReject, sRemoteBusy, &TelephoneWarp::finishWaitRemoteAnswer);
            //registerRule(sConnecting, eStartRecord, sRecording, &TelephoneWarp::startRecord);
            //registerRule(sConnecting, eHangup, sIdle, &TelephoneWarp::hideCallControllerAndRecordHistory);
            //registerRule(sConnecting, eRemoteHangup, sRemoteBusy, &TelephoneWarp::recordHistory);
            //registerRule(sRecording, eStopRecord, sConnecting, &TelephoneWarp::stopRecordAndSaveFile);
            //registerRule(sRecording, eHangup, sIdle, &TelephoneWarp::stopRecordAndSaveFileAndHideCallControllerAndRecordHistory);
            //registerRule(sIncalling, eHangup, sConnecting, &TelephoneWarp::showCallController);
            //registerRule(sIncalling, eLeaveWord, sRecording, &TelephoneWarp::startRecord);
            //registerRule(sIncalling, eRemoteHangup, sIdle, &TelephoneWarp::recordHistory);
            //registerRule(sIncalling, eReject, sIdle, &TelephoneWarp::hideRemoteCallAndRecordHistory);
            //registerRule(sRemoteBusy, eHangup, sIdle, &TelephoneWarp::hideCallController);
            setStartState(sIdle);
            Util::ATCommand::Instance()->RegisterObserver(this);
            Util::HardwareKeypad::Instance()->RegisterObserver(this);
            //AT+CMGF=0\xD
        }

        void SMSWarp::Update(std::string const& data) {
            //IRA GSM UCS2 HEX PCCP PCDN 8859_1
			if (data.find("+CMT") != std::string::npos) {
				Util::AT::CMTIndicator m(data);
				Data::Message msg = Data::Message::Parse(m.pdu);
			}
        }

        //void SMSWarp::processMessage(Data::Message const& message) {
        //    //message.Insert(); //insert modify the message's id
        //    state_ = sNotifier;
        //    if (message.remote.address == "spec code") {
        //        SMSParser::MessageType mt = SMSParser::getMessageType(message);
        //        //switch (mt) {
        //        //case SMSParser::tRegister:
        //        //    //no this case
        //        //    break;
        //        //case SMSParser::tRegisterReply:
        //        //    //no this case
        //        //    break;
        //        //case SMSParser::tWeather:
        //        //    transport_->Get(message.binaryData.substr(4), localFilename);
        //        //    state_ = sContentCompleted;
        //        //    notifyWeather("new weather info arravied", localFilename);
        //        //    break;
        //        //case SMSParser::tSoftwareDownload:
        //        //    //not process
        //        //    break;
        //        //case SMSParser::tMultimediaDownload:
        //        //    transport_->Get(message.binaryData.substr(4), localFilename);
        //        //    state_ = sContentCompleted;
        //        //    notifyMultimedia("new multimedia info arravied", localFilename);
        //        //    break;
        //        //case SMSParser::tMenuManager:
        //        //    transport_->Get(message.binaryData.substr(4), localFilename);
        //        //    state_ = sContentCompleted;
        //        //    notifyMenu("new menu info arravied", localFilename);
        //        //    break;
        //        //default:
        //        //    break;
        //        //}
        //    } else {
        //        //if (confirm("you have a message, do you want see it?") == yes) {
        //        //    showMessageDetail(message); //view same as message history message detail
        //        //} else {
        //        //    unreadedMessages_.push_back(message);
        //        //    indicationView("new message received");
        //        //}
        //    }
        //}

        void SMSWarp::SendMessage(std::string const& number, std::string const& content) {
            //AT+CMGS=%d\xD //%d is strlen
            //str\x1A //1A = Ctrl + Z
            //AT+CMGD=%d\xD //delete
            //AT+CMGL=4\xD //read list
            //AT+CSCA=\"%s\",145\xD //set sms center address
        }

        int const SMSWarp::run() {
            while (true) {
                if (!isQuit_) {
                    //get short message list

                    //AT+CMGR

                    //AT+CMGL=0\xD //get sm list

                    //AT+CMGD=%d\xD //delete sm %d is index

                    //parse it
                    //write to database
                    //notify the form and show it
                    ::Sleep(1000);
                } else {
                    break;
                }
            }
            return 0;
        }
    }
}
