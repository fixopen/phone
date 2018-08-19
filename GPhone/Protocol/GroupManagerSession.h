#ifndef __PROTOCOL_GROUPMANAGERSESSION_H__
#define __PROTOCOL_GROUPMANAGERSESSION_H__

#include "Session.h"

namespace Protocol {
    class GroupManagerSession : public Session {
    public:
        enum Type {
            tList,
            tInfo,
        };
        GroupManagerSession(void);
        ~GroupManagerSession(void);
        void SetType(Type type) {
            type_ = type;
        }
    private:
        void selfProcess_(Util::ResponseMessage const & response) const;
        std::string const resourceName_() const;
        virtual std::string const additionParameter_() const;
        Type type_;
    };
}

#endif //__PROTOCOL_GROUPMANAGERSESSION_H__
