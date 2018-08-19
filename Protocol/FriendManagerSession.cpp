#include "FriendManagerSession.h"

FriendManagerSession::FriendManagerSession(void)
{
}

FriendManagerSession::~FriendManagerSession(void)
{
}

void FriendManagerSession::SetParameter(std::string const& familyId) {
	familyId_ = familyId;
}

void FriendManagerSession::selfProcess_(ResponseMessage const& response) const {
}

std::string const FriendManagerSession::resourceName_() const {
    return "ZF3G/tic/friendlist";
}

std::string const FriendManagerSession::additionParameter_() const {
    return "&familyid=" + familyId_;
}
