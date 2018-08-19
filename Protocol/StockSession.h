#ifndef __PROTOCOL_STOCKSESSION_H__
#define __PROTOCOL_STOCKSESSION_H__

#include "Session.h"

class StockSession : public Session {
public:
    StockSession(void);
    ~StockSession(void);
    void SetParameter(std::string const& stockCode);
private:
    void selfProcess_(ResponseMessage const& response) const;
    std::string const resourceName_() const;
    virtual std::string const additionParameter_() const;
    virtual std::string const alterServer_() const;
private:
	std::string stockCode_;
    std::string newUrl_;
};

#endif //__PROTOCOL_STOCKSESSION_H__
