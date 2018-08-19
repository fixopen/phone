#ifndef __PROTOCOL_PUSHSESSION_H__
#define __PROTOCOL_PUSHSESSION_H__

#include "Session.h"

namespace Protocol {
    class PushSession : public Session {
    public:
        PushSession(void);
        ~PushSession(void);
    };
}

#endif //__PROTOCOL_PUSHSESSION_H__
