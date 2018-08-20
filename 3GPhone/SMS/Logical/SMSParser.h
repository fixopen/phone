#ifndef __SMS_LOGICAL_SMSPARSER_H__
#define __SMS_LOGICAL_SMSPARSER_H__

#include "../Data/Message.h"

namespace SMS {
    namespace Logical {
        class SMSParser {
        public:
            enum MessageType {
                tRegister,
                tRegisterReply,
                tWeather,
                tSoftwareDownload,
                tMultimediaDownload,
                tMenuManager,
            };
            static MessageType const getMessageType(Data::Message const& message);
        };
    }
}

#endif // __SMS_LOGICAL_SMSPARSER_H__
