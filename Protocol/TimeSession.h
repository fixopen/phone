#pragma once
#include "Session.h"

class TimeSession : public Session {
public:
    TimeSession(void);
    ~TimeSession(void);
private:
    void selfProcess_(ResponseMessage const& response) const;
    std::string const resourceName_() const;
    //virtual std::string const additionParameter_() const;
};
