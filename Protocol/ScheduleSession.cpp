#include "ScheduleSession.h"

ScheduleSession::ScheduleSession(void)
{
}

ScheduleSession::~ScheduleSession(void)
{
}

void ScheduleSession::selfProcess_(ResponseMessage const& response) const {
}

std::string const ScheduleSession::resourceName_() const {
    return "ZF3G/tic/schedullist";
}

std::string const ScheduleSession::additionParameter_() const {
    return "&begindate=20090501&enddate=20090531";
}
