#ifndef __PROTOCOL_MEMBERMANAGERSESSION_H__
#define __PROTOCOL_MEMBERMANAGERSESSION_H__

#include "Session.h"

namespace Protocol {
    class MemberManagerSession : public Session {
    public:
        MemberManagerSession(void);
        ~MemberManagerSession(void);
        void SetParameter(std::string const & familyId);
    private:
        void selfProcess_(Util::ResponseMessage const & response) const;
        std::string const resourceName_() const;
        virtual std::string const additionParameter_() const;
    private:
        std::string familyId_;
    };
}

#endif //__PROTOCOL_MEMBERMANAGERSESSION_H__
