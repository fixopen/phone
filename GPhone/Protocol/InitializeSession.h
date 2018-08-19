#ifndef __PROTOCOL_INITIALIZESESSION_H__
#define __PROTOCOL_INITIALIZESESSION_H__

#include "Session.h"

namespace Protocol {
    class InitializeSession : public Session {
    public:
        InitializeSession(void);
        ~InitializeSession(void);
        //virtual void Process();
    protected:
        virtual void selfProcess_(Util::ResponseMessage const & response) const;
    private:
        virtual std::string const resourceName_() const;
    };
}

#endif //__PROTOCOL_INITIALIZESESSION_H__
