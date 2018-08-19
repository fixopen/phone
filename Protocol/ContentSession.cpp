#include "ContentSession.h"

ContentSession::ContentSession(void)
{
}

ContentSession::~ContentSession(void)
{
}

void ContentSession::SetType(Type const type) {
    type_ = type;
}

void ContentSession::selfProcess_(ResponseMessage const& response) const {
}

std::string const ContentSession::resourceName_() const {
    std::string result;
    switch (type_) {
        case tNormal:
            result = "ZF3G/tic/contentlist";
            break;
        case tDel:
            result = "ZF3G/tic/contentdellist";
            break;
        default:
            break;
    }
    return result;
}
