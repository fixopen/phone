#pragma once
#include "Session.h"

class FriendManagerSession : public Session {
public:
    FriendManagerSession(void);
    ~FriendManagerSession(void);
private:
    void selfProcess_(ResponseMessage const& response) const;
    std::string const resourceName_() const;
    virtual std::string const additionParameter_() const;
};
