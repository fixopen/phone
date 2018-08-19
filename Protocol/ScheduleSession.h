#pragma once
#include "Session.h"

class ScheduleSession : public Session {
public:
    ScheduleSession(void);
    ~ScheduleSession(void);
	void SetParameter(std::string const& beginDate, std::string const& endDate);
private:
    void selfProcess_(ResponseMessage const& response) const;
    std::string const resourceName_() const;
    virtual std::string const additionParameter_() const;
private:
	std::string beginDate_;
	std::string endDate_;
};
