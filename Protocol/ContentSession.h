#pragma once
#include "Session.h"

class ContentSession : public Session {
public:
    enum Type {
        tNormal,
        tDel,
    };
    ContentSession(void);
    ~ContentSession(void);
    void SetType(Type const type);
private:
    void selfProcess_(ResponseMessage const& response) const;
    std::string const resourceName_() const;
    //virtual std::string const additionParameter_() const;
    Type type_;
};
