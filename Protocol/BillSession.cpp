#include "BillSession.h"

BillSession::BillSession(void)
{
}

BillSession::~BillSession(void)
{
}

void BillSession::selfProcess_(ResponseMessage const& response) const {
}

std::string const BillSession::resourceName_() const {
    return "ZF3G/tic/billinfo";
}

std::string const BillSession::additionParameter_() const {
    return "&billtype=" + billType_ + "&billtime=" + billTime_;
}
