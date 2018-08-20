#pragma once
#include "Session.h"

class GroupManagerSession : public Session {
public:
    enum Type {
        tList,
        tInfo,
    };
    GroupManagerSession(void);
    ~GroupManagerSession(void);
    void SetType(Type type) {
        type_ = type;
    }
private:
    void selfProcess_(ResponseMessage const& response) const;
    std::string const resourceName_() const;
    virtual std::string const additionParameter_() const;
    Type type_;
};
