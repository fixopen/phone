// PSerialPort.h: interface for the CPSerialPort class.
//
//////////////////////////////////////////////////////////////////////
#include <string>

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CPSerialPort  
{
public:
	CPSerialPort();
	virtual ~CPSerialPort();
	BOOL OpenPort(LPCTSTR Port,int Baudrate,int DataBits,int StopBits,int Parity); //打开串口
	void ClosePort(); //关闭串口
	//激活与关闭持续接收
	BOOL Activate();
	BOOL Deactivate();
	BOOL IsActive();
	//接收数据线程函数
	static DWORD WINAPI ReadPortThread(LPVOID lpParameter);
	BOOL  ReadPort(char *data,int length,int &readcount); //读取一定长度的数据
	BOOL  ReadSynPort(void* lpBuf, DWORD dwLength);//异步读取数据
	bool  WritePort(char *data,int length,bool addHead = false); //发送数据
	BOOL  WriteSynPort(const void* lpBuf, DWORD dwLength);
	void  ClearPort();
	HWND  hWnd;

public:
	BOOL  ReadPort(char *data,int &readcount); //读取一定长度的数据

private:
	HANDLE m_hComm; //串口设备handle
	HANDLE m_hReadThread; //读串口线程handle
	BOOL m_bReceiving; //是否持续接收
	int m_nBufferSize; //缓冲大小
	char *Buffer; //缓冲区
	DWORD m_dwUserData;
	//串口设置以及超时参数
	DCB dcb;
	COMMTIMEOUTS CommTimeOuts;
	OVERLAPPED m_osRead, m_osWrite; 
	
};

