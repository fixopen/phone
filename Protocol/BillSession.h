#pragma once
#include "Session.h"

class BillSession : public Session {
public:
    BillSession(void);
    ~BillSession(void);
    void SetBillType(std::string const& billtype) {
        billType_ = billtype;
    }
    void SetBillTime(std::string const& billtime) {
        billTime_ = billtime;
    }
private:
    void selfProcess_(ResponseMessage const& response) const;
    std::string const resourceName_() const;
    virtual std::string const additionParameter_() const;
    std::string billType_;
    std::string billTime_;
};
