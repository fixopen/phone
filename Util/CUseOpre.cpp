#include "stdafx.h"

#include "CUseOpre.h"
#include "pserialport.h"

#include "../Usb/UsbData.h"
#include "stringOp.h"
#include <assert.h>

CPSerialPort m_pSerial;

HANDLE m_hRead ;
char readBuff[2*1024+1] = {0};

//extern std::string FileDBPath;
extern "C" BOOL InitUsbFunction()
{
	if(!CCUseOpre::openUsbPort())
	{	
		//打开USB口
		return FALSE;
	}
	else
	{
		if(!CCUseOpre::StartReadThread())
		{	
			//启动读串口线程
			return FALSE;
		}
	}
	return TRUE;
}

extern "C"  CloseUsbFunction()
{
	CCUseOpre::CloseUsbPort();
}

extern "C" BOOL CheckUsbState()
{
	return CCUseOpre::openUsbPort();
}

void ReadData(char *ptr, int &nPtrLen)
{	
	m_pSerial.ReadPort(ptr, nPtrLen);
}

bool SendData(char *data,int length)
{
	return (m_pSerial.WritePort(data,length) == length) ;	
}


/////////////////////////////////////////

CCUseOpre::CCUseOpre(void)
{

}

CCUseOpre::~CCUseOpre(void)
{

}

BOOL CCUseOpre::openUsbPort()
{
	if(!m_pSerial.OpenPort(L"COM4:",9600,8,1,NOPARITY))
	{
		return FALSE;
	}	
	return TRUE;
}

BOOL CCUseOpre::StartReadThread()
{   
	if (!m_pSerial.Activate())
	{
		return FALSE;
	}
	return TRUE; 
}

void CCUseOpre::CloseUsbPort()
{
   m_pSerial.ClosePort();
}


