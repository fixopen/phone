#ifndef __PROTOCOL_WEATHERSESSION_H__
#define __PROTOCOL_WEATHERSESSION_H__

#include "Session.h"

namespace Protocol {
    class WeatherSession : public Session {
    public:
        WeatherSession(void);
        ~WeatherSession(void);
        void SetParameter(std::string const & taskId);
    private:
        void selfProcess_(Util::ResponseMessage const & response) const;
        std::string const resourceName_() const;
        virtual std::string const additionParameter_() const;
    private:
        std::string taskId_;
    };
}

#endif //__PROTOCOL_WEATHERSESSION_H__
