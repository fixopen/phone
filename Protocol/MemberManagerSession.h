#pragma once
#include "Session.h"

class MemberManagerSession : public Session {
public:
    MemberManagerSession(void);
    ~MemberManagerSession(void);
private:
    void selfProcess_(ResponseMessage const& response) const;
    std::string const resourceName_() const;
    virtual std::string const additionParameter_() const;
};
