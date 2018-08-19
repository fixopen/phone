#ifndef __PROTOCOL_UPDATEMENUSESSION_H__
#define __PROTOCOL_UPDATEMENUSESSION_H__

#include "Session.h"

namespace Protocol {
    class UpdateMenuSession : public Session {
    public:
        UpdateMenuSession(void);
        ~UpdateMenuSession(void);
    private:
        void selfProcess_(Util::ResponseMessage const & response) const;
        std::string const resourceName_() const;
    };
}

#endif //__PROTOCOL_UPDATEMENUSESSION_H__
