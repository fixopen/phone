#pragma once
#include "Session.h"

class MemberManagerSession : public Session {
public:
    MemberManagerSession(void);
    ~MemberManagerSession(void);
	void SetParameter(std::string const& familyId);
private:
    void selfProcess_(ResponseMessage const& response) const;
    std::string const resourceName_() const;
    virtual std::string const additionParameter_() const;
private:
	std::string familyId_;
};
