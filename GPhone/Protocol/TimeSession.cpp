#include "TimeSession.h"

namespace Protocol {
    TimeSession::TimeSession(void) {
    }

    TimeSession::~TimeSession(void) {
    }

    void TimeSession::selfProcess_(Util::ResponseMessage const & response) const {
        /*
        <servertime>YYYYMMDDHHMISS</servertime>
        */
    }

    std::string const TimeSession::resourceName_() const {
        return "ZF3G/tic/getservertime";
    }
}
