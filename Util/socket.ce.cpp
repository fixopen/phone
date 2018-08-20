#include "socket.ce.h"

DWORD UDP::Open(int const localPort, char const* const remoteHost, int const remotePort)
{
	WSADATA wsa;
	//加载winsock动态链接库
	if (WSAStartup(MAKEWORD(1, 1), &wsa) != 0)
	{
		return -1;//代表失败
	}
	//创建UDP套接字
	handle_ = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (handle_ == INVALID_SOCKET)
	{
		return -2;
	}
	SOCKADDR_IN localAddr;
	localAddr.sin_family = AF_INET;
	localAddr.sin_port = localPort; //htons();
	localAddr.sin_addr.s_addr = INADDR_ANY;
	//绑定地址
	if (bind(handle_, (sockaddr*)&localAddr, sizeof(localAddr)) != 0)
	{
		return -3;
	}
	//设置非堵塞通讯
	DWORD ul = 1;
	ioctlsocket(handle_, FIONBIO, &ul);
	//创建通讯线程
	//@@AfxBeginThread(RecvThread, this);

	remoteAddr_.sin_family = AF_INET;
	remoteAddr_.sin_port = htons(remotePort);
	//此处要将双字节转换成单字节
	//char ansiRemoteHost[255] = {0};
	//WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, remoteHost, wcslen(remoteHost), ansiRemoteHost, wcslen(remoteHost), NULL, NULL);
	remoteAddr_.sin_addr.s_addr = inet_addr(remoteHost);
	exit_ = false;
	return 1;
}
/*
*函数介绍：关闭UDP通讯端口
*入口参数：(无)
*出口参数：(无)
*返 回 值：1代表成功；-1，-2等都代表失败
*/
DWORD UDP::Close(void)
{
	exit_ = true;
	Sleep(2000);
	//关闭socket
	if (closesocket(handle_) == SOCKET_ERROR)
	{
		return -1;
	}
	//释放socket资源
	if (WSACleanup() == SOCKET_ERROR)
	{
		return -2;
	}
	return 1;
}

/*
*函数介绍：发送数据
*入口参数：dataPriority: 数据优先级 
dataType: 数据类型
buf:缓冲区数据
len:缓冲数据长度
*出口参数：(无)
*返回值：发送成功代表实际发送的字节数，否则返回-1
*/
DWORD UDP::SendData(char const* const buf, int const length)
{
	int totalSendedByteCount = 0;
	while (totalSendedByteCount < length)
	{
		int sendedByteCount = sendto(handle_, buf + totalSendedByteCount, length - totalSendedByteCount, 0, (sockaddr*)&remoteAddr_, sizeof(remoteAddr_));
		if (sendedByteCount == SOCKET_ERROR)
		{
			int errorCode = WSAGetLastError();
			if (onError)
				onError(errorCode);
			return -1;
		}
		if (totalSendedByteCount == length)
		{
			break;
		}
		Sleep(1000);
		totalSendedByteCount = totalSendedByteCount + sendedByteCount;
	}
	return totalSendedByteCount;
}

std::string UDP::GetLocalIP(void)
{
	std::string result;
	char buffer[256] = {0};
	if (gethostname(buffer, 256) == 0)
	{
		HOSTENT* LocalAddress = gethostbyname(buffer);
		memset(buffer, 0, 256);
		sprintf(buffer, "%d.%d.%d.%d",
			LocalAddress->h_addr_list[0][0] & 0xFF,
			LocalAddress->h_addr_list[0][1] & 0xFF,
			LocalAddress->h_addr_list[0][2] & 0xFF,
			LocalAddress->h_addr_list[0][3] & 0xFF);
		result = std::string((char*)buffer);
	}
	return result;
}
/*
*函数介绍：接收线程函数
*入口参数：lparam : 指传进线程的参数
*出口参数：(无)
*返 回 值：无意义。
*/
UINT UDP::RecvThread(void* param)
{
	UDP* socket = (UDP*)param;
	
	fd_set fdRead;
	TIMEVAL	aTime;
	aTime.tv_sec = 1;
	aTime.tv_usec = 0;
	
	char recvBuf[1024] = {0};
	int recvLen;
	
	SOCKADDR_IN tmpAddr;
	int tmpRecvLen;
	while (!socket->exit_)
	{
		//将set初始化空集合
		FD_ZERO(&fdRead);
		//将socket->handle_套接字添加到集合中
		FD_SET(socket->handle_, &fdRead);
		//调用select函数，判断套接字I/O状态
		int ret = select(0, &fdRead, NULL, NULL, &aTime);
		if (ret == SOCKET_ERROR)
		{
			int errorCode = WSAGetLastError();
			socket->onError(errorCode);
			break;
		}
		if (ret > 0)
		{
			if (FD_ISSET(socket->handle_, &fdRead))
			{
                tmpAddr.sin_family = AF_INET;             
                tmpAddr.sin_port = htons(socket->remoteAddr_.sin_port);
				tmpAddr.sin_addr.s_addr = INADDR_ANY;
                tmpRecvLen = sizeof(tmpAddr);
				//recvBuf = new char[1024];
				memset(recvBuf, 0, 1024);
				recvLen = recvfrom(socket->handle_, recvBuf, 1024, 0, (SOCKADDR*)&tmpAddr, &tmpRecvLen); 
				if (recvLen == SOCKET_ERROR)
				{
					int errorCode = WSAGetLastError();
					socket->onError(errorCode);
					break;
				}
				else if (recvLen == 0)
				{
					int errorCode = WSAGetLastError();
					socket->onError(errorCode);	
					break;
				}
				else
				{
					socket->onReceived(recvBuf, recvLen); 
				}
			}
		}
	}
	return 0;
}