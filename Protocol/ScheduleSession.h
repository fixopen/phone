#pragma once
#include "Session.h"

class ScheduleSession : public Session {
public:
    ScheduleSession(void);
    ~ScheduleSession(void);
private:
    void selfProcess_(ResponseMessage const& response) const;
    std::string const resourceName_() const;
    virtual std::string const additionParameter_() const;
};
