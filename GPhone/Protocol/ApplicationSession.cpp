#include "ApplicationSession.h"

namespace Protocol {
    ApplicationSession::ApplicationSession(void) {
    }

    ApplicationSession::~ApplicationSession(void) {
    }

    void ApplicationSession::selfProcess_(Util::ResponseMessage const & response) const {
    }

    std::string const ApplicationSession::resourceName_() const {
        return "ZF3G/tic/menuinfo";
    }
}
