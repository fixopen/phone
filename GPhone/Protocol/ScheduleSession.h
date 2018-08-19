#ifndef __PROTOCOL_SCHEDULESESSION_H__
#define __PROTOCOL_SCHEDULESESSION_H__

#include "Session.h"

namespace Protocol {
    class ScheduleSession : public Session {
    public:
        ScheduleSession(void);
        ~ScheduleSession(void);
        void SetParameter(std::string const & beginDate, std::string const endDate);
    private:
        void selfProcess_(Util::ResponseMessage const & response) const;
        std::string const resourceName_() const;
        virtual std::string const additionParameter_() const;
    private:
        std::string beginDate_;
        std::string endDate_;
    };
}

#endif //__PROTOCOL_SCHEDULESESSION_H__
