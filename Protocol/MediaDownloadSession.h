#pragma once
#include "Session.h"

class MediaDownloadSession : public Session {
public:
    MediaDownloadSession(void);
    ~MediaDownloadSession(void);
    void SetParameters(std::string const& msgType, std::string const& taskId, std::string const& msgId, std::string const& cid, std::string const& cname);
private:
    void selfProcess_(ResponseMessage const& response) const;
    std::string const resourceName_() const;
    virtual std::string const additionParameter_() const;
private:
    std::string taskId_;
    std::string msgId_;
    std::string cid_;
    std::string cname_;

    std::string msgType_;
};
