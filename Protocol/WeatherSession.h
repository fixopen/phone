#pragma once
#include "Session.h"

class WeatherSession : public Session {
public:
    WeatherSession(void);
    ~WeatherSession(void);
private:
    void selfProcess_(ResponseMessage const& response) const;
    std::string const resourceName_() const;
    virtual std::string const additionParameter_() const;
};
