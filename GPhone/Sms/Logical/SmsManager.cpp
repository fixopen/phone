#include <afxwin.h>
#include <vector>
#include "SmsManager.h"
#include "../../Util/HardwareKeypad.h"

namespace Sms {
    namespace Logical {
        SmsManager::SmsManager()
        : Util::Observer()
        , Util::Observable() {
            Util::AT::Dispatcher::Instance()->RegisterObserver(this);
            Util::HardwareKeypad::Instance()->RegisterObserver(this);
            //AT+CMGF=0\xD
        }

        void SmsManager::Update(std::string const & data) {
            //IRA GSM UCS2 HEX PCCP PCDN 8859_1
            if (data.find("+CMT") != std::string::npos) {
                Util::AT::CMTIndicator m(data);
                processReceiveMessage_(Data::SmsMessage::Parse(m.pdu));
            }
        }

        void SmsManager::processReceiveMessage_(Data::SmsMessage const & message) {
            //message.Insert(); //insert modify the message's id
            if (message.remote.address == "spec code") {
                //SMSParser::MessageType mt = SMSParser::getMessageType(message);
                //switch (mt) {
                //case SMSParser::tRegister:
                //    //no this case
                //    break;
                //case SMSParser::tRegisterReply:
                //    //no this case
                //    break;
                //case SMSParser::tWeather:
                //    transport_->Get(message.binaryData.substr(4), localFilename);
                //    state_ = sContentCompleted;
                //    notifyWeather("new weather info arravied", localFilename);
                //    break;
                //case SMSParser::tSoftwareDownload:
                //    //not process
                //    break;
                //case SMSParser::tMultimediaDownload:
                //    transport_->Get(message.binaryData.substr(4), localFilename);
                //    state_ = sContentCompleted;
                //    notifyMultimedia("new multimedia info arravied", localFilename);
                //    break;
                //case SMSParser::tMenuManager:
                //    transport_->Get(message.binaryData.substr(4), localFilename);
                //    state_ = sContentCompleted;
                //    notifyMenu("new menu info arravied", localFilename);
                //    break;
                //default:
                //    break;
                //}
            } else {
                  //if (confirm("you have a message, do you want see it?") == yes) {
                  //    showMessageDetail(message); //view same as message history message detail
                  //} else {
                  //    unreadedMessages_.push_back(message);
                  //    indicationView("new message received");
                  //}
            }
        }

        void SmsManager::SendMessage(std::string const & number, std::string const & content) {
            //AT+CMGS=%d\xD //%d is strlen
            //str\x1A //1A = Ctrl + Z
            //AT+CMGD=%d\xD //delete
            //AT+CMGL=4\xD //read list
            //AT+CSCA=\"%s\",145\xD //set sms center address

            //AT+CMGR

            //AT+CMGL=0\xD //get sm list

            //AT+CMGD=%d\xD //delete sm %d is index
        }
    }
}
