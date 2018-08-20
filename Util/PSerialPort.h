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
	BOOL OpenPort(LPCTSTR Port,int Baudrate,int DataBits,int StopBits,int Parity); //�򿪴���
	void ClosePort(); //�رմ���
	//������رճ�������
	BOOL Activate();
	BOOL Deactivate();
	BOOL IsActive();
	//���������̺߳���
	static DWORD WINAPI ReadPortThread(LPVOID lpParameter);
	BOOL  ReadPort(char *data,int length,int &readcount); //��ȡһ�����ȵ�����
	BOOL  ReadSynPort(void* lpBuf, DWORD dwLength);//�첽��ȡ����
	bool  WritePort(char *data,int length,bool addHead = false); //��������
	BOOL  WriteSynPort(const void* lpBuf, DWORD dwLength);
	void  ClearPort();
	HWND  hWnd;

public:
	BOOL  ReadPort(char *data,int &readcount); //��ȡһ�����ȵ�����

private:
	HANDLE m_hComm; //�����豸handle
	HANDLE m_hReadThread; //�������߳�handle
	BOOL m_bReceiving; //�Ƿ��������
	int m_nBufferSize; //�����С
	char *Buffer; //������
	DWORD m_dwUserData;
	//���������Լ���ʱ����
	DCB dcb;
	COMMTIMEOUTS CommTimeOuts;
	OVERLAPPED m_osRead, m_osWrite; 
	
};

