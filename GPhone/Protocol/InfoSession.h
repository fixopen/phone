#ifndef __PROTOCOL_INFOSESSION_H__
#define __PROTOCOL_INFOSESSION_H__

#include "Session.h"

namespace Protocol {
    class InfoSession : public Session {
    public:
        InfoSession(void);
        ~InfoSession(void);
    };
}

#endif //__PROTOCOL_INFOSESSION_H__
