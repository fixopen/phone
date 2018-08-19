#ifndef __PROTOCOL_DOWNLOADSESSION_H__
#define __PROTOCOL_DOWNLOADSESSION_H__

#include "Session.h"

namespace Protocol {
    class DownloadSession : public Session {
    public:
        DownloadSession(void);
        ~DownloadSession(void);
    };
}

#endif //__PROTOCOL_DOWNLOADSESSION_H__
