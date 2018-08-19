#pragma once
#include "Message.h"

class RequestMessage : public Message {
public:
    enum Type {
        tGet,
        tPost,
        tPut,
        tDelete,
        tHead,
        tOptions,
        tTrace,
        tConnect,
    };
    RequestMessage();
    ~RequestMessage(void);
    std::string const ToString() const;
    Type type;
    std::string uri;
    std::string version;
private:
    std::string const typeToString_(Type type) const;
};
