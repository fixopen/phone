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
BOOL ComWarp::OpenComm(const wchar_t* pPort, int nBaudRate, int nParity, int nByteSize, int nStopBits)   
{   
    DCB dcb;        // ���ڿ��ƿ�    
    COMMTIMEOUTS timeouts = {   // ���ڳ�ʱ���Ʋ���    
        100,                // ���ַ������ʱʱ��: 100 ms    
        1,                  // ������ʱÿ�ַ���ʱ��: 1 ms (n���ַ��ܹ�Ϊn ms)    
        500,                // ������(�����)����ʱʱ��: 500 ms    
        1,                  // д����ʱÿ�ַ���ʱ��: 1 ms (n���ַ��ܹ�Ϊn ms)    
        100               // ������(�����)д��ʱʱ��: 100 ms    
    };

    hComm = CreateFile(pPort,   // �������ƻ��豸·��    
        GENERIC_READ | GENERIC_WRITE,   // ��д��ʽ    
        0,              // ����ʽ����ռ    
        NULL,           // Ĭ�ϵİ�ȫ������    
        OPEN_EXISTING,  // ������ʽ    
        0,              // ���������ļ�����    
        NULL);          // �������ģ���ļ�    

    if (hComm == INVALID_HANDLE_VALUE)     // �򿪴���ʧ��    
        return FALSE;

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

    WriteFile(hComm, pData, (DWORD)nLength, &dwNumWrite, NULL);   

    return (int)dwNumWrite;   
}   

// ������    
// ����: pData - ���������ݻ�����ָ��    
//       nLength - ������������ݳ���    
// ����: ʵ�ʶ��������ݳ���    
int ComWarp::ReadComm(void* pData, int nLength)   
{   
    DWORD dwNumRead;    // �����յ������ݳ���

    ReadFile(hComm, pData, (DWORD)nLength, &dwNumRead, NULL);   

    return (int)dwNumRead;   
}   
