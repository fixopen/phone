#pragma once
#include "Session.h"

class BizManagerSession : public Session {
public:
    BizManagerSession(void);
    ~BizManagerSession(void);
    //virtual void Process();
protected:
    virtual void selfProcess_(ResponseMessage const& response) const;
private:
    virtual std::string const resourceName_() const;
};
