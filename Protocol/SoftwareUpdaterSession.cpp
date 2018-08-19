#include "SoftwareUpdaterSession.h"

SoftwareUpdaterSession::SoftwareUpdaterSession(void)
{
}

SoftwareUpdaterSession::~SoftwareUpdaterSession(void)
{
}

void SoftwareUpdaterSession::selfProcess_(ResponseMessage const& response) const {
    std::FILE* file = std::fopen("img.jpg", "wb");
    size_t length = response.Body.length();
    if (file) {
        std::fwrite(response.Body.c_str(), 1, response.Body.length(), file);
        std::fclose(file);
    }
}

std::string const SoftwareUpdaterSession::resourceName_() const {
    return "ZF3G/tic/softwareUpdate";
}

std::string const SoftwareUpdaterSession::additionParameter_() const {
    return "&taskid=12345";
}
