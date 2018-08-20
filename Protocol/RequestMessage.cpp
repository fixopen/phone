#include "RequestMessage.h"

RequestMessage::RequestMessage()
: type(tGet)
, version("HTTP/1.1") {
}

RequestMessage::~RequestMessage(void)
{
}

std::string const RequestMessage::ToString() const {
    std::string result = typeToString_(type) + " " + uri + " " + version + "\r\n" + Message::ToString();
    return result;
}

std::string const RequestMessage::typeToString_(Type type) const {
    std::string result;
    switch (type) {
        case tGet:
            result = "GET";
            break;
        case tPost:
            result = "POST";
            break;
        case tPut:
            result = "PUT";
            break;
        case tDelete:
            result = "DELETE";
            break;
        case tHead:
            result = "HEAD";
            break;
        case tOptions:
            result = "OPTIONS";
            break;
        case tTrace:
            result = "TRACE";
            break;
        case tConnect:
            result = "CONNECT";
            break;
        default:
            break;
    }
    return result;
}
