#pragma once
#include "Session.h"

class RegisterSession :
    public Session
{
public:
    RegisterSession(void);
    ~RegisterSession(void);
    //virtual void Process();
private:
    virtual std::string const resourceName_() const;
};
