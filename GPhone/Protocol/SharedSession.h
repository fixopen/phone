#ifndef __PROTOCOL_SHAREDSESSION_H__
#define __PROTOCOL_SHAREDSESSION_H__

#include "Session.h"

namespace Protocol {
    class SharedSession : public Session {
    public:
        SharedSession(void);
        ~SharedSession(void);
    };
}

#endif //__PROTOCOL_SHAREDSESSION_H__
