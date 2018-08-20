#pragma once
#include "Session.h"

class SoftwareUpdaterSession : public Session {
public:
    SoftwareUpdaterSession(void);
    ~SoftwareUpdaterSession(void);
private:
    void selfProcess_(ResponseMessage const& response) const;
    std::string const resourceName_() const;
    virtual std::string const additionParameter_() const;
};
