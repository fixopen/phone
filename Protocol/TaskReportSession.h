#pragma once
#include "Session.h"

class TaskReportSession : public Session {
public:
    TaskReportSession(void);
    ~TaskReportSession(void);
private:
    void selfProcess_(ResponseMessage const& response) const;
    std::string const resourceName_() const;
    virtual std::string const additionParameter_() const;
};
