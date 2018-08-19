#include "stdafx.h"
#include "ComWarp.h"
using Util::ComWarp;

ComWarp::ComWarp()
{

}
ComWarp::~ComWarp()
{

}

// 打开串口    
// 输入: pPort - 串口名称或设备路径，可用"COM1"或"\\.\COM1"两种方式，建议用后者    
//       nBaudRate - 波特率    
//       nParity - 奇偶校验    
//       nByteSize - 数据字节宽度    
//       nStopBits - 停止位    
BOOL ComWarp::OpenComm(const WCHAR* pPort, int nBaudRate, int nParity, int nByteSize, int nStopBits)   
{   
	DCB dcb;        // 串口控制块    
	COMMTIMEOUTS timeouts = {   // 串口超时控制参数    
		MAXDWORD,                // 读字符间隔超时时间: ms    
			0,                  // 读操作时每字符的时间:  ms (n个字符总共为n ms)    
			0,                // 基本的(额外的)读超时时间:  ms    
			50,                  // 写操作时每字符的时间:  ms (n个字符总共为n ms)    
			2000};               // 基本的(额外的)写超时时间:  ms    
		
//     COMMTIMEOUTS timeouts = {   // 串口超时控制参数    
// 			100,                // 读字符间隔超时时间: 100 ms    
// 			1,                  // 读操作时每字符的时间: 1 ms (n个字符总共为n ms)    
// 			500,                // 基本的(额外的)读超时时间: 500 ms    
// 			1,                  // 写操作时每字符的时间: 1 ms (n个字符总共为n ms)    
// 			100};               // 基本的(额外的)写超时时间: 100 ms    

		hComm = CreateFile(pPort,   // 串口名称或设备路径    
			GENERIC_READ | GENERIC_WRITE,   // 读写方式    
			0,              // 共享方式：独占    
			NULL,           // 默认的安全描述符    
			OPEN_EXISTING,  // 创建方式    
			0,              // 不需设置文件属性    
			NULL);          // 不需参照模板文件    

		if(hComm == INVALID_HANDLE_VALUE) return FALSE;     // 打开串口失败    

		GetCommState(hComm, &dcb);      // 取DCB    

		dcb.BaudRate = nBaudRate;   
		dcb.ByteSize = nByteSize;   
		dcb.Parity = nParity;   
		dcb.StopBits = nStopBits;   

		SetCommState(hComm, &dcb);      // 设置DCB    

		SetupComm(hComm, 4096, 1024);   // 设置输入输出缓冲区大小    

		SetCommTimeouts(hComm, &timeouts);  // 设置超时    

		return TRUE;   
}   

// 关闭串口    
BOOL ComWarp::CloseComm()   
{   
	return CloseHandle(hComm);   
}   

// 写串口    
// 输入: pData - 待写的数据缓冲区指针    
//       nLength - 待写的数据长度    
// 返回: 实际写入的数据长度    
int ComWarp::WriteComm(void* pData, int nLength)   
{   
	DWORD dwNumWrite;   // 串口发出的数据长度    

	WriteFile(hComm, pData, (DWORD)nLength, &dwNumWrite, NULL);   
	if (dwNumWrite > 0)
	{
		extern VOID WriteLog(char *ptr);
		WriteLog((char *)pData);
//		Dprintf("Write: %d ", dwNumWrite);
		Dprintf((char *)pData);
	}
	return (int)dwNumWrite;   
}   

// 读串口    
// 输入: pData - 待读的数据缓冲区指针    
//       nLength - 待读的最大数据长度    
// 返回: 实际读出的数据长度    
int ComWarp::ReadComm(void* pData, int nLength)   
{   
//	Sleep(1000);
	DWORD dwNumRead;    // 串口收到的数据长度  
	DWORD ret = 0;

	memset(pData, 0, nLength);
	DWORD  s = GetTickCount();

	while((GetTickCount() -s) < 1000)
	{
		ReadFile(hComm, pData, (DWORD)nLength, &dwNumRead, NULL);  
		ret += dwNumRead;
		if (dwNumRead > 0)
		{
			extern VOID WriteLog(char *ptr);
			WriteLog((char *)pData);
			
			Dprintf((char *)pData);
			break;
		}
		else if (dwNumRead == 0)
		{
			TRACE(L"00000000000000000000000000000000000000000\n");
			// Dprintf("00000000000000000000000000000000000000000\n");
		}
		::Sleep(0);
	}

	return (int)dwNumRead;   
}   
