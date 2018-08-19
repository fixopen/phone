#ifndef __SMS_LOGICAL_SMSWARP_H__
#define __SMS_LOGICAL_SMSWARP_H__

#include "../Data/Message.h"
#include "../../Util/HTTPTransport.h"

namespace SMS {
    namespace Logical {
        class SMSWarp {
        public:
            enum Status {
                sNotifier,
                //sReadedNotifier,
                //sGetContent,
                sContentCompleted,
                sComplete,
            };
            void processMessage(Data::Message const& message);
        private:
            unsigned int timerId_;
            Util::HTTPTransport* transport_;
            std::vector<Data::Message> unreadedMessages_;
            Status state_;
        };
    }
}

#endif // __SMS_LOGICAL_SMSWARP_H__
