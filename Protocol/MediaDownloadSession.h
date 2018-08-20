#pragma once
#include "Session.h"

class MediaDownloadSession : public Session {
public:
    MediaDownloadSession(void);
    ~MediaDownloadSession(void);
private:
    void selfProcess_(ResponseMessage const& response) const;
    std::string const resourceName_() const;
    virtual std::string const additionParameter_() const;
};
