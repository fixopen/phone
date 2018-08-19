#ifndef __PROTOCOL_STOCKSESSION_H__
#define __PROTOCOL_STOCKSESSION_H__

#include "Session.h"

namespace Protocol {
    class StockSession : public Session {
    public:
        StockSession(void);
        ~StockSession(void);
        void SetParameter(std::string const & newUrl);
    private:
        void selfProcess_(Util::ResponseMessage const & response) const;
        std::string const resourceName_() const;
        virtual std::string const additionParameter_() const;
        virtual std::string const alterServer_() const;
    private:
        std::string newUrl_;
    };
}

#endif //__PROTOCOL_STOCKSESSION_H__
