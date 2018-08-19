#ifndef __SMS_LOGICAL_SMSMANAGER_H__
#define __SMS_LOGICAL_SMSMANAGER_H__

#include "../../Util/Fsm.h"
#include "../../Util/Observer.h" //for Observer & Observable
#include "../../Util/Thread.h"
#include "../../Util/AT/ATCommand.h"
#include "../../Util/HTTPTransport.h"
#include "../Data/SmsMessage.h"

namespace Sms {
    namespace Logical {
        class SmsManager : public Util::Observer, public Util::Observable {
        public:
            //sms classifer & search
            //sms parser & serialize
            //sms render & edit
            //sms send & receive
            //smsc config
            static SmsManager* const Instance() {
                static SmsManager* result = new SmsManager();
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
            SmsManager();
            virtual void Update(std::string const & data); //process the serial port data and command response
            void SendMessage(std::string const & number, std::string const & content);
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
            void processReceiveMessage_(Data::SmsMessage const & message);
            static void telephoneParser(std::string const & data);
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
            std::vector<Data::SmsMessage> unreadedMessages_;
            Status state_;
            volatile bool isQuit_;
            Type type_;
        };
    }
}

#endif //__SMS_LOGICAL_SMSMANAGER_H__
