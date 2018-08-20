#ifndef __SOCKET_H__
#define __SOCKET_H__

#include <winsock.h>

namespace Util
{
    class Address;
    class Socket;
    class DatagramSocket;
    class StreamSocket;
    class ClientSocket;
    class ServerSocket;

    class Address
    {
    public:
        Address(void);
        Address(char const* const ipAddr, unsigned short const port);
        char const* const ipAddr(void) const;
        void ipAddr(char const* const ipaddr);
        void ipAddr(int const ipaddr);
        unsigned short const port(void) const;
        void port(unsigned short const portNumber);
        sockaddr const* const data(void) const;
        void data(sockaddr const* const sockAddr);
        static char const* const intIpToStrIp(unsigned int const ipAddr);
        static Address const getLocalIp(unsigned short const port);
    private:
        void init_(void);
        static char const* const uintToString_(unsigned int const ipAddr);
        unsigned char data_[16];
    };

    class Socket
    {
    public:
        enum ErrorState
        {
            esNormal,
            esCreateError,
            esBindError,
            esListenError,
            esConnectionError,
            esAcceptError,
            esSendError,
            esReceiveError,
            esSendToError,
            esReceiveFromError,
            esDestroyError,
            esSetOptionError,
            esTimeoutError,
            esOtherError
        };

        enum ErrorState getErrorState(void)
        {
            return errorState_;
        }

        void clearErrorState(void)
        {
            errorState_ = esNormal;
        }

        Address const getHere(void) const
        {
            return here_;
        }

        void setHere(Address here)
        {
            here_ = here;
        }

        Address const getThere(void) const
        {
            return there_;
        }

        void setThere(Address there)
        {
            there_ = there;
        }

        SOCKET const getHandle(void) const
        {
            return theSocket_;
        }

		void (*onError)(int errorCode);
		void (*onReceived)(int errorCode);

        void close(void);
    protected:
        Socket(int const type, int const protocol);
        Socket(SOCKET socket);
        virtual ~Socket(void);
        void bind(int const ipAddr, unsigned short const port);
        void bind(char const* const ipAddr, unsigned short const port);
        void bindLocal(unsigned short const port);
        void listen(int const backlog = 5);
        StreamSocket* accept(void);
        void connect(char const* const ipAddr, unsigned short const port);
        void receive(char* const buffer, int const bufferLength, int* const length);
        void send(char const* const buffer, int const length);
        void receiveFrom(char* const buffer, int const bufferLength, int* const length, char* const ipAddr, unsigned short* const port);
        void sendTo(char const* const buffer, int const length, char const* const ipAddr, unsigned short const port);
    private:
        static void init(void)
        {
            WSADATA WSAData;
            WSAStartup(0x0110, &WSAData);
        }

        static void term(void)
        {
            WSACleanup();
        }
    private:
        SOCKET theSocket_;//Ì×½Ó×Ö
        Address here_;
        Address there_;
        int flags_;
        enum ErrorState errorState_;
    };

    class DatagramSocket : public Socket
    {
    public:
        DatagramSocket(int const protocol = 0)
            : Socket(SOCK_DGRAM, protocol)
        {
        }

        ~DatagramSocket(void)
        {
        }

        void bind(int const ipAddr, unsigned short const port)
		{
			Socket::bind(ipAddr, port);
		}

        void bind(char const* const ipAddr, unsigned short const port)
		{
			Socket::bind(ipAddr, port);
		}

        void bindLocal(unsigned short const port)
		{
			Socket::bindLocal(port);
		}

        void receiveFrom(char* const buffer, int const bufferLength, int* const length, char* const ipAddr, unsigned short* const port)
		{
			Socket::receiveFrom(buffer, bufferLength, length, ipAddr, port);
		}

        void sendTo(char const* const buffer, int const length, char const* const ipAddr, unsigned short const port)
		{
			Socket::sendTo(buffer, length, ipAddr, port);
		}
        //using Socket::bind;
        //using Socket::bindLocal;
        //using Socket::receiveFrom;
        //using Socket::sendTo;
    };

    class StreamSocket : public Socket
    {
    public:
        StreamSocket(int const protocol = 0)
            : Socket(SOCK_STREAM, protocol)
        {
        }

        enum HandleFlag
        {
            IsHandle
        };
        static enum HandleFlag const isHandle;
        StreamSocket(SOCKET socket, enum HandleFlag isHandle)
            : Socket(socket)
        {
        }

        ~StreamSocket(void)
        {
        }

        using Socket::bind;
        using Socket::bindLocal;
        using Socket::listen;
        using Socket::accept;
        using Socket::connect;
        using Socket::receive;
        using Socket::send;
    };

    class ClientSocket : public StreamSocket
    {
    public:
        ClientSocket(int const protocol = 0)
            : StreamSocket(protocol)
        {
        }
        ~ClientSocket(void)
        {
        }

        using StreamSocket::connect;
        using StreamSocket::receive;
        using StreamSocket::send;
    };

    class ServerSocket : public StreamSocket
    {
    public:
        ServerSocket(int const protocol = 0)
            : StreamSocket(protocol)
        {
        }
        ~ServerSocket(void)
        {
        }

        using StreamSocket::bind;
        using StreamSocket::bindLocal;
        using StreamSocket::listen;
        using StreamSocket::accept;
        using StreamSocket::receive;
        using StreamSocket::send;
    };
}

#endif //__SOCKET_H__