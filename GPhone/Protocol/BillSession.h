#ifndef __PROTOCOL_BILLSESSION_H__
#define __PROTOCOL_BILLSESSION_H__

#include "Session.h"

namespace Protocol {
    class BillSession : public Session {
    public:
        BillSession(void);
        ~BillSession(void);
        void SetBillType(std::string const & billtype) {
            billType_ = billtype;
        }
        void SetBillTime(std::string const & billtime) {
            billTime_ = billtime;
        }
    private:
        void selfProcess_(Util::ResponseMessage const & response) const;
        std::string const resourceName_() const;
        virtual std::string const additionParameter_() const;
        std::string billType_;
        std::string billTime_;
    };
}

#endif //__PROTOCOL_BILLSESSION_H__
