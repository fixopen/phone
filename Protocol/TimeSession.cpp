#include "TimeSession.h"

TimeSession::TimeSession(void)
{
}

TimeSession::~TimeSession(void)
{
}

void TimeSession::selfProcess_(ResponseMessage const& response) const {
}

std::string const TimeSession::resourceName_() const {
    return "ZF3G/tic/getservertime";
}
