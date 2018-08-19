#ifndef __PROTOCOL_APPLICATIONSESSION_H__
#define __PROTOCOL_APPLICATIONSESSION_H__

#include "Session.h"

namespace Protocol {
    class ApplicationSession : public Session {
    public:
        ApplicationSession(void);
        ~ApplicationSession(void);
    private:
        void selfProcess_(Util::ResponseMessage const & response) const;
        std::string const resourceName_() const;
    };
}

#endif //__PROTOCOL_APPLICATIONSESSION_H__
