#include "socket.h"

#include <stdio.h>

namespace Util
{
#define Error ERROR

    Address::Address(void)
    {
        init_();
    }

    Address::Address(char const* const ipAddr, unsigned short const port)
    {
        init_();
        *((unsigned short*)&data_[2]) = htons(port);
        *((unsigned int*)&data_[4]) = inet_addr(ipAddr);
    }

    char const* const Address::ipAddr(void) const
    {
        return uintToString_(*((unsigned int*)&data_[4]));
    }

    void Address::ipAddr(char const* const ipaddr)
    {
        *((unsigned int*)&data_[4]) = inet_addr((char* const)ipaddr);
    }

    void Address::ipAddr(int const ipaddr)
    {
        *((unsigned int*)&data_[4]) = htonl(ipaddr);
    }

    unsigned short const Address::port(void) const
    {
        return ntohs(*((unsigned short*)&data_[2]));
    }

    void Address::port(unsigned short const portNumber)
    {
        *((unsigned short*)&data_[2]) = htons((unsigned short)portNumber);
    }

    sockaddr const* const Address::data(void) const
    {
        return(sockaddr*)data_;
    }

    void Address::data(sockaddr const* const sockAddr)
    {
        ::memcpy(data_, sockAddr, 16);
    }

    char const* const Address::intIpToStrIp(unsigned int const ipAddr)
    {
        return uintToString_(ipAddr);
    }

    Address const Address::getLocalIp(unsigned short const port)
    {
        static char localName[128];
        memset(localName, 0, 128);
        gethostname(localName, 128);
        hostent* local = gethostbyname(localName);
        return Address(local->h_addr_list[0], port);
        //struct hostent
        //{
        //    char FAR* h_name;
        //    char FAR  FAR** h_aliases;
        //    short h_addrtype;
        //    short h_length;
        //    char FAR  FAR** h_addr_list;
        //    #define h_addr h_addr_list[0];
        //};

        //int getaddrinfo(const char* nodename, const char* servname, const struct addrinfo* hints, struct addrinfo** res);
        //struct addrinfo
        //{
        //    int ai_flags;
        //    int ai_family;
        //    int ai_socktype;
        //    int ai_protocol;
        //    size_t ai_addrlen;
        //    char* ai_canonname;
        //    struct sockaddr* ai_addr;
        //    struct addrinfo* ai_next;
        //};
    }

    void Address::init_(void)
    {
        ::memset(data_, 0, 16);
        *((short*)data_) = AF_INET;
    }

    char const* const Address::uintToString_(unsigned int const ipAddr)
    {
        static char result[16];
        ::memset(result, 0, 16);

        int totalLength = 0;
        char part[4];
        int length = 0;
        for (int i = 0; i <= 3; ++i)
        {
            length = ::sprintf(part, "%d", ipAddr >> (i * 8) & 0x000000FF);
            ::memcpy(&result[totalLength], part, length);
            totalLength += length;
            result[totalLength] = '.';
            ++totalLength;
        }
        --totalLength;
        result[totalLength] = 0;
        return result;
    }

    Socket::Socket(int type, int protocol)
    {
        static bool isFirst = true;
        if (isFirst)
        {
            init();
            isFirst = false;
        }
        if ((theSocket_ = ::socket(AF_INET, type, protocol)) == Error)
        {
            errorState_ = esCreateError;
        }
        flags_ = 0;
        errorState_ = esNormal;
    }

    Socket::Socket(SOCKET socket)
        : theSocket_(socket)
        , flags_(0)
    {
    }

    Socket::~Socket(void)
    {
        if (errorState_ != esCreateError && (::closesocket(theSocket_) == Error))
        {
            errorState_ = esDestroyError;
        }
    }

    void Socket::bind(char const* const ipAddr, unsigned short const port)
    {
        here_.ipAddr(ipAddr);
        here_.port(port);
        if (::bind(theSocket_, here_.data(), 16) == Error)
        {
            errorState_ = esBindError;
        }
    }

    void Socket::bind(int const ipAddr, unsigned short const port)
    {
        here_.ipAddr(ipAddr);
        here_.port(port);
        if (::bind(theSocket_, here_.data(), 16) == Error)
        {
            errorState_ = esBindError;
        }
    }

    void Socket::bindLocal(unsigned short const port)
    {
        bind((int const)INADDR_ANY, port);
    }

    void Socket::listen(int backlog)
    {
        if (::listen(theSocket_, backlog) == Error)
        {
            errorState_ = esListenError;
        }
    }

    StreamSocket* Socket::accept(void)
    {
        SOCKET clientSocket = 0;
        int thereLength = 16;
        if ((clientSocket = ::accept(theSocket_, (sockaddr*)there_.data(), &thereLength)) == Error)
        {
            errorState_ = esAcceptError;
        }
        StreamSocket* result = new StreamSocket(clientSocket, StreamSocket::isHandle);
        result->setHere(here_);
        result->setThere(there_);
        return result;
    }

    void Socket::connect(char const* const ipAddr, unsigned short const port)
    {
        there_.ipAddr(ipAddr);
        there_.port(port);
        if (::connect(theSocket_, (sockaddr*)there_.data(), 16) == Error)
        {
            errorState_ = esConnectionError;
        }
    }

    void Socket::receive(char* const buffer, int const bufferLength, int* const length)
    {
        while ((*length = ::recv(theSocket_, buffer, bufferLength, flags_)) == Error)
        {
            errorState_ = esReceiveError;
        }
    }

    void Socket::send(char const* const buffer, int const length)
    {
        if (::send(theSocket_, buffer, length, flags_) == Error)
        {
            errorState_ = esSendError;
        }
    }

    void Socket::receiveFrom(char* const buffer, int const bufferLength, int* const length, char* const ipAddr, unsigned short* const port)
    {
        int fromLength = 16;
        while ((*length = ::recvfrom(theSocket_, buffer, bufferLength, flags_, (sockaddr*)there_.data(), &fromLength)) <= 0)
        {
            errorState_ = esReceiveFromError;
        }
        strcpy(ipAddr, there_.ipAddr());
        *port = there_.port();
    }

    void Socket::sendTo(char const* const buffer, int const length, char const* const ipAddr, unsigned short const port)
    {
        int toLength = 16;
        there_.ipAddr(ipAddr);
        there_.port(port);
        if (::sendto(theSocket_, buffer, length, flags_, there_.data(), toLength) == Error)
        {
            errorState_ = esSendToError;
        }
    }

    void Socket::close(void)
    {
        //::shutdown(theSocket_, 0); //SD_RECEIVE, SD_SEND, SD_BOTH
        ::closesocket(theSocket_);
        delete this;
    }

    enum StreamSocket::HandleFlag const StreamSocket::isHandle = IsHandle;

#undef Error
}
