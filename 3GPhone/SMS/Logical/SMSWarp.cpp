#include "SMSWarp.h"
#include "SMSParser.h"

namespace SMS {
    namespace Logical {
        void SMSWarp::processMessage(Data::Message const& message) {
            message.Insert();
            state_ = sNotifier;
            if (message.remote.address == "spec code") {
                SMSParser::MessageType mt = SMSParser::getMessageType(message);
                switch (mt) {
                case SMSParser::tRegister:
                    //no this case
                    break;
                case SMSParser::tRegisterReply:
                    //no this case
                    break;
                case SMSParser::tWeather:
                    transport_->Get(message.binaryData.substr(4), localFilename);
                    state_ = sContentCompleted;
                    notifyWeather("new weather info arravied", localFilename);
                    break;
                case SMSParser::tSoftwareDownload:
                    //not process
                    break;
                case SMSParser::tMultimediaDownload:
                    transport_->Get(message.binaryData.substr(4), localFilename);
                    state_ = sContentCompleted;
                    notifyMultimedia("new multimedia info arravied", localFilename);
                    break;
                case SMSParser::tMenuManager:
                    transport_->Get(message.binaryData.substr(4), localFilename);
                    state_ = sContentCompleted;
                    notifyMenu("new menu info arravied", localFilename);
                    break;
                default:
                    break;
                }
            } else {
                if (confirm("you have a message, do you want see it?") == yes) {
                    showMessageDetail(message); //view same as message history message detail
                } else {
                    unreadedMessages_.push_back(message);
                    indicationView("new message received");
                }
            }
        }
    }
}
