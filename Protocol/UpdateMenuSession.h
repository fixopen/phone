#pragma once
#include "session.h"

class UpdateMenuSession : public Session {
public:
    UpdateMenuSession(void);
    ~UpdateMenuSession(void);
private:
    void selfProcess_(ResponseMessage const& response) const;
    std::string const resourceName_() const;
};
