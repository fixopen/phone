#pragma once
#include "Session.h"

class InitializeSession : public Session {
public:
    InitializeSession(void);
    ~InitializeSession(void);
    //virtual void Process();
protected:
    virtual void selfProcess_(ResponseMessage const& response) const;
private:
    virtual std::string const resourceName_() const;
};
