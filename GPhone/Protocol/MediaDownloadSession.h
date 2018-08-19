#ifndef __PROTOCOL_MEDIADOWNLOADSESSION_H__
#define __PROTOCOL_MEDIADOWNLOADSESSION_H__

#include "Session.h"

namespace Protocol {
    class MediaDownloadSession : public Session {
    public:
        MediaDownloadSession(void);
        ~MediaDownloadSession(void);
        void SetParameters(std::string const & msgType, std::string const & taskId, std::string const & msgId, std::string const & cid, std::string const & cname);
    private:
        void selfProcess_(Util::ResponseMessage const & response) const;
        std::string const resourceName_() const;
        virtual std::string const additionParameter_() const;
    private:
        std::string taskId_;
        std::string msgId_;
        std::string cid_;
        std::string cname_;

        std::string msgType_;
    };
}

#endif //__PROTOCOL_MEDIADOWNLOADSESSION_H__
