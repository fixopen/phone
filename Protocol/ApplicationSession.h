#pragma once
#include "Session.h"

class ApplicationSession : public Session {
public:
    ApplicationSession(void);
    ~ApplicationSession(void);
private:
    void selfProcess_(ResponseMessage const& response) const;
    std::string const resourceName_() const;
};
