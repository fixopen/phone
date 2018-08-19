#include "WeatherSession.h"

namespace Protocol {
    WeatherSession::WeatherSession(void) {
    }

    WeatherSession::~WeatherSession(void) {
    }

    void WeatherSession::SetParameter(std::string const & taskId) {
        taskId_ = taskId;
    }

    void WeatherSession::selfProcess_(Util::ResponseMessage const & response) const {
        /*
        <?xml version="1.0" encoding="UTF-8"?>
        <weatherreport>
        <count>3</count>
        <pack>
        <city>北京</city>
        <date>20090619</date>
        <weatherstatus>晴</weatherstatus>
        <uppertemp>10</uppertemp>
        <lowertemp>-2</lowertemp>
        <windlevel>北风3级</windlevel>
        <airquality>良</airquality>
        <otherinfo>洗车指数：1</otherinfo>
        </pack>
        <pack>
        <city>北京</city>
        <date>20090620</date>
        <weatherstatus>晴</weatherstatus>
        <uppertemp>10</uppertemp>
        <lowertemp>-2</lowertemp>
        <windlevel>北风3级</windlevel>
        <airquality>良</airquality>
        <otherinfo>洗车指数：1</otherinfo>
        </pack>
        <pack>
        <city>北京</city>
        <date>20090621</date>
        <weatherstatus>晴</weatherstatus>
        <uppertemp>10</uppertemp>
        <lowertemp>-2</lowertemp>
        <windlevel>北风3级</windlevel>
        <airquality>良</airquality>
        <extinfo>洗车指数：1</extinfo>
        </pack>
        </weatherreport>
        */
        size_t o = 0;
        std::string pack = findContent_(response.Body, "pack", o);
        while (pack != "") {
            size_t offset = 0;
            std::string city = findContent_(pack, "city", offset);
            std::string date = findContent_(pack, "date", offset);
            std::string weatherstatus = findContent_(pack, "weatherstatus", offset);
            std::string uppertemp = findContent_(pack, "uppertemp", offset);
            std::string lowertemp = findContent_(pack, "lowertemp", offset);
            std::string windlevel = findContent_(pack, "windlevel", offset);
            std::string airquality = findContent_(pack, "airquality", offset);
            std::string extinfo = findContent_(pack, "extinfo", offset);
            //Calendar::Data::Weather w;
            //w.header.timestamp = Util::Timestamp::GetCurrentTime();
            //w.header.version = "1.0.0";
            //w.weatherInfos.area = city;
            //w.weatherInfos.datetime = date;
            //w.weatherInfos.temperature = uppertemp + "~" + lowertemp;
            //w.weatherInfos.wind = windlevel;
            //w.weatherInfos.description = extinfo;
            //w.Insert();
            pack = findContent_(response.Body, "pack", o);
        }
    }

    std::string const WeatherSession::resourceName_() const {
        return "ZF3G/tic/weatherreport";
    }

    std::string const WeatherSession::additionParameter_() const {
        return "&taskid=" + taskId_;
    }
}
