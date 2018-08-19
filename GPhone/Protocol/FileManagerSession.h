#ifndef __PROTOCOL_FILEMANAGERSESSION_H__
#define __PROTOCOL_FILEMANAGERSESSION_H__

#include "Session.h"

namespace Protocol {
    class FileManagerSession : public Session {
    public:
        FileManagerSession(void);
        ~FileManagerSession(void);
    };
}

#endif //__PROTOCOL_FILEMANAGERSESSION_H__
