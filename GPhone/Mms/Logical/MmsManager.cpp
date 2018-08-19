#include "MmsManager.h"
#include "../../Sms/Logical/SmsManager.h" //for SmsManager

namespace Mms {
    namespace Logical {
        MmsManager::MmsManager()
        : Observer() {
            Sms::Logical::SmsManager::Instance()->RegisterObserver(this);
        }

        void MmsManager::Update(std::string const & data) {
            //for any mms notify, use the data for uri
            //processReceiveMessage_(msg);
        }

        void MmsManager::SendMessage(std::string const & number, std::string const & content) {
        }

        void MmsManager::processReceiveMessage_(Data::MmsMessage const & message) {
        }
    }
}
