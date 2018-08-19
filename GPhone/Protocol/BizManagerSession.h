#ifndef __PROTOCOL_BIZMANAGERSESSION_H__
#define __PROTOCOL_BIZMANAGERSESSION_H__

#include "Session.h"

namespace Protocol {
    class BizManagerSession : public Session {
    public:
        BizManagerSession(void);
        ~BizManagerSession(void);
        //virtual void Process();
    protected:
        virtual void selfProcess_(Util::ResponseMessage const & response) const;
    private:
        virtual std::string const resourceName_() const;
    };
}
#endif //__PROTOCOL_BIZMANAGERSESSION_H__
