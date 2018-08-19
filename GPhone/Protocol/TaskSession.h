#ifndef __PROTOCOL_TASKSESSION_H__
#define __PROTOCOL_TASKSESSION_H__

#include "Session.h"

namespace Protocol {
    class TaskSession : public Session {
    public:
        TaskSession(void);
        ~TaskSession(void);
    private:
        void selfProcess_(Util::ResponseMessage const & response) const;
        std::string const resourceName_() const;
    };
}

#endif //__PROTOCOL_TASKSESSION_H__
