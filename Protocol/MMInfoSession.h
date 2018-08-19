#pragma once
#include "Session.h"

class MMInfoSession : public Session {
public:
    MMInfoSession(void);
    ~MMInfoSession(void);
private:
    void selfProcess_(ResponseMessage const& response) const;
    std::string const resourceName_() const;
    virtual std::string const additionParameter_() const;
};
