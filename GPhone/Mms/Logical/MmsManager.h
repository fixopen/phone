#ifndef __MMS_LOGICAL_MMSMANAGER_H__
#define __MMS_LOGICAL_MMSMANAGER_H__

#include "../../Util/Observer.h" //for Observer
#include "../Data/MmsMessage.h" //for MmsMessage

namespace Mms {
    namespace Logical {
        class MmsManager : public Util::Observer {
        public:
            //mms classifer & search
            //mms parser & serialize
            //mms render & edit
            //mms send & receive
            static MmsManager* const Instance() {
                static MmsManager* result = new MmsManager();
                return result;
            }
            MmsManager();
            virtual void Update(std::string const & data); //process the serial port data and command response
            void SendMessage(std::string const & number, std::string const & content);
        private:
            void processReceiveMessage_(Data::MmsMessage const & message);
        };
    }
}

#endif //__MMS_LOGICAL_MMSMANAGER_H__
