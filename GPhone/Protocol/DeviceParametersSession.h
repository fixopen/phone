#ifndef __PROTOCOL_DEVICEPARAMETERSSESSION_H__
#define __PROTOCOL_DEVICEPARAMETERSSESSION_H__

#include "Session.h"

namespace Protocol {
    class DeviceParametersSession : public Session {
    public:
        DeviceParametersSession(void);
        ~DeviceParametersSession(void);
    private:
        void selfProcess_(Util::ResponseMessage const & response) const;
        std::string const resourceName_() const;
    };
}

#endif //__PROTOCOL_DEVICEPARAMETERSSESSION_H__
