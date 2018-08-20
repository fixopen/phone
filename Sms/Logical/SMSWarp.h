#ifndef __SMS_LOGICAL_SMSWARP_H__
#define __SMS_LOGICAL_SMSWARP_H__

#include "../../Util/Fsm.h"
#include "../../Util/Observer.h"
#include "../../Util/Thread.h"
#include "../../Util/ATCommandWarp.h"
#include "../../Util/HTTPTransport.h"
#include "../Data/Message.h"

namespace SMS {
    namespace Logical {
        class SMSWarp : public Util::FSM<SMSWarp>, public Util::Observer, public Util::Thread {
        public:
			static SMSWarp* const Instance() {
				static SMSWarp* result = new SMSWarp();
				return result;
			}
            enum Type {
                tSMS,
                tMMS,
                tKLSF,
            };
            enum Status {
                sIdle,
                sReceived,
                sDelete,
                sDeleted,
                sSend,
                sSended,
                sError,
                //sReadedNotifier,
                //sGetContent,
                sContentCompleted,
                sComplete,
            };
            enum Event {
                eSend,
                eReceive,
                eError,
            };
            SMSWarp();
            virtual void Update(std::string const& data); //process the serial port data and command response
            void SendMessage(std::string const& number, std::string const& content);
            virtual int const run();
            void willStop() {
                isQuit_ = true;
            }
        protected:
            void showDialpad(void* p);
            void waitRemoteAnswer(void* p);
            void hideDialpadAndShowCallController(void* p);
            void startRecord(void* p);
            void stopRecordAndSaveFile(void* p);
            void hideCallControllerAndRecordHistory(void* p);
            void showRemoteCallAndRecordCallId(void* p);
            void showCallController(void* p);
            void recordHistory(void* p);
            void hideCallController(void* p);
            void hideDialpad(void* p);
            void finishWaitRemoteAnswer(void* p);
            void hideRemoteCallAndRecordHistory(void* p);
            void stopRecordAndSaveFileAndHideCallControllerAndRecordHistory(void* p);
        private:
            static void telephoneParser(std::string const& data);
            void showDialpad_(void* p);
            void waitRemoteAnswer_(void* p);
            void hideDialpad_(void* p);
            void showCallController_(void* p);
            void startRecord_(void* p);
            void stopRecord_(void* p);
            void saveFile_(void* p);
            void hideCallController_(void* p);
            void recordHistory_(void* p);
            void showRemoteCall_(void* p);
            void recordCallId_(void* p);
            void finishWaitRemoteAnswer_(void* p);
            void hideRemoteCall_(void* p);
        private:
            unsigned int timerId_;
            Util::HTTPTransport* transport_;
            std::vector<Data::Message> unreadedMessages_;
            Status state_;
            volatile bool isQuit_;
            Type type_;
        };
    }
}

#endif // __SMS_LOGICAL_SMSWARP_H__
