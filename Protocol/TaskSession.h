#pragma once
#include "Session.h"

class TaskSession : public Session {
public:
    TaskSession(void);
    ~TaskSession(void);
private:
    void selfProcess_(ResponseMessage const& response) const;
    std::string const resourceName_() const;
};
