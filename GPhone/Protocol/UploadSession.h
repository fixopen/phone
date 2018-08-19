#ifndef __PROTOCOL_UPLOADSESSION_H__
#define __PROTOCOL_UPLOADSESSION_H__

#include "Session.h"

namespace Protocol {
    class UploadSession : public Session {
    public:
        UploadSession(void);
        ~UploadSession(void);
    };
}

#endif //__PROTOCOL_UPLOADSESSION_H__
