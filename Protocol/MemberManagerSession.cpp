#include "MemberManagerSession.h"

MemberManagerSession::MemberManagerSession(void)
{
}

MemberManagerSession::~MemberManagerSession(void)
{
}

void MemberManagerSession::selfProcess_(ResponseMessage const& response) const {
}

std::string const MemberManagerSession::resourceName_() const {
    return "ZF3G/tic/memberlist";
}

std::string const MemberManagerSession::additionParameter_() const {
    return "&familyid=12345";
}
