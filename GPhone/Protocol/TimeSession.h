#ifndef __PROTOCOL_TIMESESSION_H__
#define __PROTOCOL_TIMESESSION_H__

#include "Session.h"

namespace Protocol {
    class TimeSession : public Session {
    public:
        TimeSession(void);
        ~TimeSession(void);
    private:
        void selfProcess_(Util::ResponseMessage const & response) const;
        std::string const resourceName_() const;
        //virtual std::string const additionParameter_() const;
    };
}

#endif //__PROTOCOL_TIMESESSION_H__
