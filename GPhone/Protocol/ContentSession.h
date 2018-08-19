#ifndef __PROTOCOL_CONTENTSESSION_H__
#define __PROTOCOL_CONTENTSESSION_H__

#include "Session.h"

namespace Protocol {
    class ContentSession : public Session {
    public:
        enum Type {
            tNormal,
            tDel,
        };
        ContentSession(void);
        ~ContentSession(void);
        void SetType(Type const type);
    private:
        void selfProcess_(Util::ResponseMessage const & response) const;
        std::string const resourceName_() const;
        //virtual std::string const additionParameter_() const;
        Type type_;
    };
}

#endif //__PROTOCOL_CONTENTSESSION_H__
