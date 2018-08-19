#ifndef __PROTOCOL_REGISTERSESSION_H__
#define __PROTOCOL_REGISTERSESSION_H__

#include "Session.h"

namespace Protocol {
    class RegisterSession : public Session {
    public:
        RegisterSession(void);
        ~RegisterSession(void);
        //virtual void Process();
    private:
        virtual std::string const resourceName_() const;
    };
}

#endif //__PROTOCOL_REGISTERSESSION_H__
