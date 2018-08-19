#include "WeatherSession.h"

WeatherSession::WeatherSession(void)
{
}

WeatherSession::~WeatherSession(void)
{
}

void WeatherSession::selfProcess_(ResponseMessage const& response) const {
}

std::string const WeatherSession::resourceName_() const {
    return "ZF3G/tic/weatherreport";
}

std::string const WeatherSession::additionParameter_() const {
    return "&taskid=20090531";
}
