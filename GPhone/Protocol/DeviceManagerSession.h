#ifndef __PROTOCOL_DEVICEMANAGERSESSION_H__
#define __PROTOCOL_DEVICEMANAGERSESSION_H__

#include "Session.h"

namespace Protocol {
    class DeviceManagerSession : public Session {
    public:
        DeviceManagerSession(void);
        ~DeviceManagerSession(void);
    };
}

#endif //__PROTOCOL_DEVICEMANAGERSESSION_H__
