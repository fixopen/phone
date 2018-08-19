#include "MMInfoSession.h"

MMInfoSession::MMInfoSession(void)
{
}

MMInfoSession::~MMInfoSession(void)
{
}

void MMInfoSession::selfProcess_(ResponseMessage const& response) const {
}

std::string const MMInfoSession::resourceName_() const {
    return "ZF3G/tic/mminfo";
}

std::string const MMInfoSession::additionParameter_() const {
    return "&taskid=12345";
}
