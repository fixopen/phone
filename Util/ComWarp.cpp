#include "stdafx.h"
#include "ComWarp.h"
using Util::ComWarp;

ComWarp::ComWarp()
{

}
ComWarp::~ComWarp()
{

}

// �򿪴���    
// ����: pPort - �������ƻ��豸·��������"COM1"��"\\.\COM1"���ַ�ʽ�������ú���    
//       nBaudRate - ������    
//       nParity - ��żУ��    
//       nByteSize - �����ֽڿ��    
//       nStopBits - ֹͣλ    
BOOL ComWarp::OpenComm(const WCHAR* pPort, int nBaudRate, int nParity, int nByteSize, int nStopBits)   
{   
	DCB dcb;        // ���ڿ��ƿ�    
	COMMTIMEOUTS timeouts = {   // ���ڳ�ʱ���Ʋ���    
		MAXDWORD,                // ���ַ������ʱʱ��: ms    
			0,                  // ������ʱÿ�ַ���ʱ��:  ms (n���ַ��ܹ�Ϊn ms)    
			0,                // ������(�����)����ʱʱ��:  ms    
			50,                  // д����ʱÿ�ַ���ʱ��:  ms (n���ַ��ܹ�Ϊn ms)    
			2000};               // ������(�����)д��ʱʱ��:  ms    
		
	//     COMMTIMEOUTS timeouts = {   // ���ڳ�ʱ���Ʋ���    
	// 			100,                // ���ַ������ʱʱ��: 100 ms    
	// 			1,                  // ������ʱÿ�ַ���ʱ��: 1 ms (n���ַ��ܹ�Ϊn ms)    
	// 			500,                // ������(�����)����ʱʱ��: 500 ms    
	// 			1,                  // д����ʱÿ�ַ���ʱ��: 1 ms (n���ַ��ܹ�Ϊn ms)    
	// 			100};               // ������(�����)д��ʱʱ��: 100 ms    

		hComm = CreateFile(pPort,   // �������ƻ��豸·��    
			GENERIC_READ | GENERIC_WRITE,   // ��д��ʽ    
			0,              // ����ʽ����ռ    
			NULL,           // Ĭ�ϵİ�ȫ������    
			OPEN_EXISTING,  // ������ʽ    
			0,              // ���������ļ�����    
			NULL);          // �������ģ���ļ�    

		if(hComm == INVALID_HANDLE_VALUE) return FALSE;     // �򿪴���ʧ��    

		GetCommState(hComm, &dcb);      // ȡDCB    

		dcb.BaudRate = nBaudRate;   
		dcb.ByteSize = nByteSize;   
		dcb.Parity = nParity;   
		dcb.StopBits = nStopBits;   

		SetCommState(hComm, &dcb);      // ����DCB    

		SetupComm(hComm, 4096, 1024);   // �������������������С    

		SetCommTimeouts(hComm, &timeouts);  // ���ó�ʱ    

		return TRUE;   
}   

// �رմ��� 
BOOL ComWarp::CloseComm()   
{   
	return CloseHandle(hComm);   
}   

// д����    
// ����: pData - ��д�����ݻ�����ָ��    
//       nLength - ��д�����ݳ���    
// ����: ʵ��д������ݳ���    
int ComWarp::WriteComm(void* pData, int nLength)   
{   
	DWORD dwNumWrite;   // ���ڷ��������ݳ���    
	
//	Dprintf("\r\nWrite_Begin:\r\n");
//	Dprintf((char *)pData);

	WriteFile(hComm, pData, (DWORD)nLength, &dwNumWrite, NULL);  
	
//	Dprintf("\r\nWrite_Middle:\r\n");
//	Dprintf((char *)pData);

	if (dwNumWrite > 0)
	{
	//	extern VOID WriteLog(char *ptr);
	//	WriteLog((char *)pData);

		VOID WriteMyLog_(char *ptr, int size);
		WriteMyLog_((char *)pData,nLength);
	
		Dprintf("\r\nWrite: %d\r\n", dwNumWrite);
		Dprintf((char *)pData);

	//	extern void EditPrintf(char *ptr);
	//	EditPrintf((char *)pData);

	}
	else //add by qi 20100913
	{
		DWORD D = GetLastError();
		Dprintf("Write_Error:%d\r\n",D);
	}
	return (int)dwNumWrite;   
}   

// ������    
// ����: pData - ���������ݻ�����ָ��    
//       nLength - ������������ݳ���    
// ����: ʵ�ʶ��������ݳ���    
int ComWarp::ReadComm(void* pData, int nLength)   
{   
//	Sleep(1000);
	DWORD dwNumRead;    // �����յ������ݳ���  
	DWORD ret = 0;

	memset(pData, 0, nLength);
	DWORD  s = GetTickCount();

	while((GetTickCount() -s) < 1000)
	{
		ReadFile(hComm, pData, (DWORD)nLength, &dwNumRead, NULL);  
		ret += dwNumRead;
		if (dwNumRead > 0)
		{
		//	extern VOID WriteLog(char *ptr);
		//	WriteLog((char *)pData);

			VOID WriteMyLog_(char *ptr, int size);
			WriteMyLog_((char*)pData,dwNumRead);
			
			Dprintf((char *)pData);

	//		extern void EditPrintf(char *ptr);
	//		EditPrintf((char *)pData);
			
			break;
		}
		else if (dwNumRead == 0)
		{
			//TRACE(L"00000000000000000000000000000000000000000\n");
			// Dprintf("00000000000000000000000000000000000000000\n");
		}
		::Sleep(0);
	}

	return (int)dwNumRead;   
}   
