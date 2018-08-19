#include "MediaDownloadSession.h"

namespace Protocol {
    MediaDownloadSession::MediaDownloadSession(void) {
    }

    MediaDownloadSession::~MediaDownloadSession(void) {
    }

    void MediaDownloadSession::SetParameters(std::string const & msgType, std::string const & taskId, std::string const & msgId, std::string const & cid, std::string const & cname) {
        msgType_ = msgType;
        taskId_ = taskId;
        msgId_ = msgId;
        cid_ = cid;
        cname_ = cname;
    }

    void MediaDownloadSession::selfProcess_(Util::ResponseMessage const & response) const {
        if (msgType_ == "1" || msgType_ == "picture") {
            //picture
            std::string filename = "/FlashDrv/my_net_photo/" + cname_;
            /*std::*/FILE* file = /*std::*/fopen(filename.c_str(), "wb");
            if (file) {
                /*std::*/fwrite(response.Body.c_str(), 1, response.Body.length(), file);
                /*std::*/fclose(file);
            }
        } else if (msgType_ == "2" || msgType_ == "audio") {
              //audio
            std::string filename = "/FlashDrv/my_net_music/" + cname_;
              /*std::*/FILE* file = /*std::*/fopen(filename.c_str(), "wb");
            if (file) {
                /*std::*/fwrite(response.Body.c_str(), 1, response.Body.length(), file);
                /*std::*/fclose(file);
            }
        } else if (msgType_ == "3" || msgType_ == "video") {
              //video
            std::string filename = "/FlashDrv/my_net_video/" + cname_;
              /*std::*/FILE* file = /*std::*/fopen(filename.c_str(), "wb");
            if (file) {
                /*std::*/fwrite(response.Body.c_str(), 1, response.Body.length(), file);
                /*std::*/fclose(file);
            }
        }
    }

    std::string const MediaDownloadSession::resourceName_() const {
        return "ZF3G/tic/pic";
    }

    std::string const MediaDownloadSession::additionParameter_() const {
        return "&taskid=" + taskId_ + "&msgid=" + msgId_ + "&cid=" + cid_ + "&cname=" + cname_;
    }
}
