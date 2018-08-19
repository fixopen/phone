#ifndef __PROTOCOL_SOFTWAREUPDATERSESSION_H__
#define __PROTOCOL_SOFTWAREUPDATERSESSION_H__

#include "Session.h"

namespace Protocol {
    class SoftwareUpdaterSession : public Session {
    public:
        SoftwareUpdaterSession(void);
        ~SoftwareUpdaterSession(void);
    private:
        void selfProcess_(Util::ResponseMessage const & response) const;
        std::string const resourceName_() const;
        virtual std::string const additionParameter_() const;
    };
}

#endif //__PROTOCOL_SOFTWAREUPDATERSESSION_H__
