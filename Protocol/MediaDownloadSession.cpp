#include "MediaDownloadSession.h"

MediaDownloadSession::MediaDownloadSession(void)
{
}

MediaDownloadSession::~MediaDownloadSession(void)
{
}

void MediaDownloadSession::selfProcess_(ResponseMessage const& response) const {
}

std::string const MediaDownloadSession::resourceName_() const {
    return "ZF3G/tic/pic";
}

std::string const MediaDownloadSession::additionParameter_() const {
    return "&taskid=12345&msgid=123&cid=1&cname=xyz";
}
