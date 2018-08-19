#include "GroupManagerSession.h"

namespace Protocol {
    GroupManagerSession::GroupManagerSession(void) {
    }

    GroupManagerSession::~GroupManagerSession(void) {
    }

    void GroupManagerSession::selfProcess_(Util::ResponseMessage const & response) const {
    }

    std::string const GroupManagerSession::resourceName_() const {
        std::string result;
        switch (type_) {
        case tInfo:
            result = "ZF3G/tic/groupinfo";
            break;
        case tList:
            result = "ZF3G/tic/grouplist";
            break;
        default:
            break;
        }
        return result;
    }

    std::string const GroupManagerSession::additionParameter_() const {
        std::string result;
        switch (type_) {
        case tInfo:
            result = "&groupid=12345";
            break;
        case tList:
            result = "&familyid=12345";
            break;
        default:
            break;
        }
        return result;
    }
}
