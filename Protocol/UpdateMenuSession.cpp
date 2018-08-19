#include "UpdateMenuSession.h"

UpdateMenuSession::UpdateMenuSession(void)
{
}

UpdateMenuSession::~UpdateMenuSession(void)
{
}

void UpdateMenuSession::selfProcess_(ResponseMessage const& response) const {
}

std::string const UpdateMenuSession::resourceName_() const {
    return "ZF3G/tic/getmenuinfo";
}
