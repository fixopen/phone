#include "../Util/StringOp.h"
#include "Communicator.h"

Communicator::Communicator()
: sessionCount_(0)
, socket_(0) {
    //reconnect_();
}

Communicator::~Communicator() {
    ::closesocket(socket_);
}

std::string const Communicator::ConsultKey(std::string const& deviceId, std::string const& platformPublicKey) {
    std::string randomKey; //random function generation the 256bit key
    if (sessionCount_ == 0) {
        std::string encriptRandomKey = rsaEncript_(platformPublicKey, deviceId, randomKey);
        std::string info; //msisdn //info->MSISDN->MSISDN-D
        std::string sendString = deviceId + info + encriptRandomKey;
        ::send(socket_, sendString.c_str(), sendString.length(), 0);
        //randomKey = RSADecript(platformPrivateKey, deviceId, encriptRandomKey);
        //MAC = hash_(randomKey, MSISDN-D, KeyTime);
        //send(MSISDN-D, KeyTime, MAC);
        std::string KeyTime, MAC; //KeyTime format and length, MAC length
        size_t length = info.length() + KeyTime.length() + MAC.length();
        char* buffer = static_cast<char*>(malloc(length));
        ::recv(socket_, buffer, length, 0);
        //(info, KeyTime, MAC);
        info = std::string(buffer, info.length());
        KeyTime = std::string(buffer + info.length(), KeyTime.length());
        MAC = std::string(buffer + info.length() + KeyTime.length(), MAC.length());
        std::string computeMac = hash_(randomKey, info, KeyTime);
        if (MAC == computeMac) { // ok
            int p = 3;
        } else { //error
            int q = 4;
        }
    }
    return randomKey;
}

ResponseMessage const Communicator::Get(RequestMessage const& message) const {
    ResponseMessage result;
    std::string received;
    char buffer[4 * 1024];
    size_t currentPos = 0;
    size_t findPos = std::string::npos;
    std::string sendString = message.ToString();
    reconnect_();
    int r = ::send(socket_, sendString.c_str(), sendString.length(), 0);
    if (r == SOCKET_ERROR) { //failure
        return result;
    }
    while (true) {
        memset(buffer, 0, 4 * 1024);
        int receivedLength = ::recv(socket_, buffer, 4 * 1024, 0);
        if (receivedLength == 0) { //or SIGPIPE in UNIX
            break;
        } else {
            received += std::string(buffer, receivedLength);
            findPos = received.find("\r\n\r\n", currentPos);
            if (findPos != std::string::npos) {
                break;
            }
        }
    }
    size_t contentLength = 0;
    std::vector<std::string> parts = Util::StringOp::Split(received, "\r\n\r\n");
    std::vector<std::string> headers = Util::StringOp::Split(parts[0], "\r\n");
    //headers[0]; //status line
    size_t offset = 0;
    size_t pos = headers[0].find(" ", offset);
    if (pos != std::string::npos) {
        result.version = headers[0].substr(0, pos);
        offset = pos + 1;
        pos = headers[0].find(" ", offset);
        if (pos != std::string::npos) {
            result.state = static_cast<ResponseMessage::State>(Util::StringOp::ToInt(headers[0].substr(offset, pos - offset)));
            offset = pos + 1;
            result.description = headers[0].substr(offset);
        }
    }
    for (size_t i = 1; i < headers.size(); ++i) {
        size_t del = headers[i].find(":");
        std::string name = Util::StringOp::Trim(headers[i].substr(0, del));
        std::string value = Util::StringOp::Trim(headers[i].substr(del + 1));
        result.HeaderFields.insert(std::make_pair(name, value));
        if (name == "Content-Length") {
            contentLength = Util::StringOp::ToInt(value);
        }
    }
    result.Body = parts[1];
    if (contentLength) {
        size_t remindLength = contentLength - parts[1].length();
        if (remindLength > 0) {
            memset(buffer, 0, 4 * 1024);
            ::recv(socket_, buffer, remindLength, 0);
            result.Body += buffer;
        }
    }
    return result;
}

void Communicator::reconnect_() const {
    static bool isFirst = true;
    if (isFirst) {
        WSADATA wsaData;
        int r = WSAStartup(MAKEWORD(2,2), &wsaData);
        if (r != NO_ERROR) { //failure
            return;
        }
        isFirst = false;
    }
    socket_ = ::socket(AF_INET, SOCK_STREAM, 0);
    if (socket_ == INVALID_SOCKET) { //failure
        return;
    }
    ::sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof serverAddr);
    serverAddr.sin_family = AF_INET;
    //serverAddr.sin_addr.s_addr = ::inet_addr("218.249.60.87"); //@@https ip
    serverAddr.sin_addr.s_addr = ::inet_addr("192.168.13.245"); //@@https ip
    serverAddr.sin_port = ::htons(8088); //@@https port
    int r = ::connect(socket_, reinterpret_cast<sockaddr*>(&serverAddr), sizeof serverAddr);
    if (r == 0) { //success
        //do nothing
        int p = 3;
    } else { //failure
        //do nothing
        int q = 4;
    }
}

//boost::shared_ptr<Session> const Communicator::BeginSession(void)
//{
//    ++sessionCount_;
//    return NULL;
//}

//void Communicator::EndSession(boost::shared_ptr<Session> const& session)
//{
//    --sessionCount_;
//}

std::string const Communicator::rsaEncript_(std::string const& platformPublicKey, std::string const& deviceId, std::string const& randomKey) {
    std::string result;
    return result;
}

std::string const Communicator::hash_(std::string const& randomKey, std::string const& msisdn, std::string const& keyTime) {
    std::string result;
    return result;
}
