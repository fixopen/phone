#include "FriendManagerSession.h"

namespace Protocol {
    FriendManagerSession::FriendManagerSession(void) {
    }

    FriendManagerSession::~FriendManagerSession(void) {
    }

    void FriendManagerSession::selfProcess_(Util::ResponseMessage const & response) const {
    }

    std::string const FriendManagerSession::resourceName_() const {
        return "ZF3G/tic/friendlist";
    }

    std::string const FriendManagerSession::additionParameter_() const {
        return "&familyid=12345";
    }
}
