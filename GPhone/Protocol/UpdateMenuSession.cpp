#include "UpdateMenuSession.h"

namespace Protocol {
    UpdateMenuSession::UpdateMenuSession(void) {
    }

    UpdateMenuSession::~UpdateMenuSession(void) {
    }

    void UpdateMenuSession::selfProcess_(Util::ResponseMessage const & response) const {
    }

    std::string const UpdateMenuSession::resourceName_() const {
        return "ZF3G/tic/getmenuinfo";
    }
}
