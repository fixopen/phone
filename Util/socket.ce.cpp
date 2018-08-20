#include "socket.ce.h"

DWORD UDP::Open(int const localPort, char const* const remoteHost, int const remotePort)
{
	WSADATA wsa;
	//����winsock��̬���ӿ�
	if (WSAStartup(MAKEWORD(1, 1), &wsa) != 0)
	{
		return -1;//����ʧ��
	}
	//����UDP�׽���
	handle_ = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (handle_ == INVALID_SOCKET)
	{
		return -2;
	}
	SOCKADDR_IN localAddr;
	localAddr.sin_family = AF_INET;
	localAddr.sin_port = localPort; //htons();
	localAddr.sin_addr.s_addr = INADDR_ANY;
	//�󶨵�ַ
	if (bind(handle_, (sockaddr*)&localAddr, sizeof(localAddr)) != 0)
	{
		return -3;
	}
	//���÷Ƕ���ͨѶ
	DWORD ul = 1;
	ioctlsocket(handle_, FIONBIO, &ul);
	//����ͨѶ�߳�
	//@@AfxBeginThread(RecvThread, this);

	remoteAddr_.sin_family = AF_INET;
	remoteAddr_.sin_port = htons(remotePort);
	//�˴�Ҫ��˫�ֽ�ת���ɵ��ֽ�
	//char ansiRemoteHost[255] = {0};
	//WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, remoteHost, wcslen(remoteHost), ansiRemoteHost, wcslen(remoteHost), NULL, NULL);
	remoteAddr_.sin_addr.s_addr = inet_addr(remoteHost);
	exit_ = false;
	return 1;
}
/*
*�������ܣ��ر�UDPͨѶ�˿�
*��ڲ�����(��)
*���ڲ�����(��)
*�� �� ֵ��1����ɹ���-1��-2�ȶ�����ʧ��
*/
DWORD UDP::Close(void)
{
	exit_ = true;
	Sleep(2000);
	//�ر�socket
	if (closesocket(handle_) == SOCKET_ERROR)
	{
		return -1;
	}
	//�ͷ�socket��Դ
	if (WSACleanup() == SOCKET_ERROR)
	{
		return -2;
	}
	return 1;
}

/*
*�������ܣ���������
*��ڲ�����dataPriority: �������ȼ� 
dataType: ��������
buf:����������
len:�������ݳ���
*���ڲ�����(��)
*����ֵ�����ͳɹ�����ʵ�ʷ��͵��ֽ��������򷵻�-1
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
*�������ܣ������̺߳���
*��ڲ�����lparam : ָ�����̵߳Ĳ���
*���ڲ�����(��)
*�� �� ֵ�������塣
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
		//��set��ʼ���ռ���
		FD_ZERO(&fdRead);
		//��socket->handle_�׽�����ӵ�������
		FD_SET(socket->handle_, &fdRead);
		//����select�������ж��׽���I/O״̬
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