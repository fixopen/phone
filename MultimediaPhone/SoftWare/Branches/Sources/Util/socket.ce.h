#ifndef __UDP_H__
#define __UDP_H__

#include "WinSock.h"

#include <string>

#pragma pack(push, 1)

typedef void (CALLBACK* ONUDPERROR)(int);
typedef void (CALLBACK* ONUDPRECV)(char const* const buffer, int const bufferLength); //, sockaddr*

class UDP
{
	friend UINT RecvThread(LPVOID lparam);
public:
	DWORD Open(int const localPort, char const* const remoteHost, int const remotePort);
	DWORD Close(void);
	DWORD SendData(char const* const buffer, int const length);
	static std::string GetLocalIP(void);
	ONUDPERROR onError;
	ONUDPRECV onReceived;
private:
	SOCKET handle_;
	sockaddr_in remoteAddr_;
	static unsigned int RecvThread(void* param);
	volatile bool exit_;
};

#pragma pack(pop)

#endif //__UDP_H__
