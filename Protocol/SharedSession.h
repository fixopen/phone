#pragma once
#include "Session.h"

class SharedSession :
    public Session
{
public:
    SharedSession(void);
    ~SharedSession(void);
};
