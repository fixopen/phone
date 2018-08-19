#ifndef __PROTOCOL_TASKREPORTSESSION_H__
#define __PROTOCOL_TASKREPORTSESSION_H__

#include "Session.h"

namespace Protocol {
    class TaskReportSession : public Session {
    public:
        TaskReportSession(void);
        ~TaskReportSession(void);
    private:
        void selfProcess_(Util::ResponseMessage const & response) const;
        std::string const resourceName_() const;
        virtual std::string const additionParameter_() const;
    };
}

#endif //__PROTOCOL_TASKREPORTSESSION_H__
