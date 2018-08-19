#ifndef __PROTOCOL_MMINFOSESSION_H__
#define __PROTOCOL_MMINFOSESSION_H__

#include "Session.h"

namespace Protocol {
    class MMInfoSession : public Session {
    public:
        MMInfoSession(void);
        ~MMInfoSession(void);
    private:
        void selfProcess_(Util::ResponseMessage const & response) const;
        std::string const resourceName_() const;
        virtual std::string const additionParameter_() const;
    };
}

#endif //__PROTOCOL_MMINFOSESSION_H__
