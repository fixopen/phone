#ifndef __PROTOCOL_KEYSESSION_H__
#define __PROTOCOL_KEYSESSION_H__

#include "Session.h"

namespace Protocol {
    class KeySession : public Session {
    public:
        KeySession(void);
        ~KeySession(void);
    private:
        virtual void selfProcess_(Util::Http::ResponseMessage const & response) const;
        virtual std::string const KeySession::resourceName_() const;
        virtual std::string const additionParameter_() const;
    };
}

#endif //__PROTOCOL_KEYSESSION_H__
