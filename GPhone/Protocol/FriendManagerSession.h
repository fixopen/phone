#ifndef __PROTOCOL_FRIENDMANAGERSESSION_H__
#define __PROTOCOL_FRIENDMANAGERSESSION_H__

#include "Session.h"

namespace Protocol {
    class FriendManagerSession : public Session {
    public:
        FriendManagerSession(void);
        ~FriendManagerSession(void);
    private:
        void selfProcess_(Util::ResponseMessage const & response) const;
        std::string const resourceName_() const;
        virtual std::string const additionParameter_() const;
    };
}

#endif //__PROTOCOL_FRIENDMANAGERSESSION_H__
