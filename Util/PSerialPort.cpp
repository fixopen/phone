// PSerialPort.cpp: implementation of the CPSerialPort class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PSerialPort.h"
#include "Usb/UsbData.h"
#include "json/jsonObject.h"
#include "Util/stringOp.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//extern CYYUsbWorkerThread * m_pYYUsbWorkerThread;

#define READ_LENGTH 1024*2
#define SEND_SIZE   1024*2 -1

#define METHOD_QUERY     "\"method\":\"query.children\""
#define QUERY_TYPE_47    "\"type\":\"47\""  
#define QUERY_TYPE_48    "\"type\":\"48\""
#define RETURN_MESSAGE   "\"return_message\":\"success\"" 


extern void	CloseFile();
extern void ReadData(char *ptr, int &nPtrLen);

CPSerialPort::CPSerialPort()
{
	m_hComm=INVALID_HANDLE_VALUE;
	m_hReadThread=NULL;
	m_bReceiving=FALSE;
	m_nBufferSize=256; //缓冲大小

}

CPSerialPort::~CPSerialPort()
{
	ClosePort();
}

char data[1024];
int  rcount=0;
std::string text="xiaoyanzi";
FILE *rfp = NULL;
int flen=0;
int flt=0;
BOOL bStatus=FALSE;


uchar			     usbDataBuff[1024*4] = {0}; //接收到的USB数据
UsbData				 g_usbData;

//old
/*DWORD WINAPI CPSerialPort::ReadPortThread(LPVOID lpParameter)
{		
	 DWORD           dwEvtMask;
	 int             usbDataLen = 0;
	 CPSerialPort*   pSerial;
	 pSerial=(CPSerialPort*)lpParameter;
	
	 std::string data   ;
	 std::string result ;

	 SetCommMask(pSerial->m_hComm, EV_RXCHAR);
	 while((pSerial->m_hComm !=INVALID_HANDLE_VALUE))
	 {	    
		   DWORD dBegin  = GetTickCount();
           WaitCommEvent(pSerial->m_hComm,&dwEvtMask,NULL);

		   if((dwEvtMask & EV_RXCHAR) == EV_RXCHAR)
		   {				
				memset(usbDataBuff, 0, sizeof(usbDataBuff));
				//usbDataLen 收到的USB数据的长度
				TRACE(L"ReadData_Begin:%d\n",GetTickCount() - dBegin);
				ReadData((char *)usbDataBuff, usbDataLen);			
				
				TRACE(L"ReadData:%d\n",GetTickCount() - dBegin);

				if(usbDataLen > 0)
				{	
					//处理USB数据头
					g_usbData.parseDataHeader((char *)usbDataBuff);
					//添加数据,创建文件在UsbData里处理
					g_usbData.addData((char *)usbDataBuff,usbDataLen);

					//文件数据在USBdata里处理
					if (g_usbData.bHaveNext || \
						g_usbData.dataType_ != UsbData::dtData)
					{
						continue;
					}
					
					//处理收完的USB数据
					data = g_usbData.GetData();
					//解析数据
					result = jsonProcessor(data);

					if (result.compare(JSON_SENDFILE) == 0)	
					{
						//发送文件
						UsbData::SendFile();
					}
					else if (result.compare(JSON_NOFILE) == 0)
					{
						//没这个文件
						result = "B0001E00010";
						pSerial->WritePort((char *)result.c_str(),\
							result.size());
					}
					else
					{	
						//发送数据
						pSerial->WritePort((char *)Util::StringOp::ToUTF8(result).c_str(),\
							Util::StringOp::ToUTF8(result).size(),true);
					}
				}
		   }  
	 }

	return 0;
}*/



//
bool HandData(	 CPSerialPort*   pSerial ,	int usbDataLen = 0 )
{	
	//处理USB数据头
	std::string data   ;
	std::string result ;

	g_usbData.parseDataHeader((char *)usbDataBuff);
	//添加数据,创建文件在UsbData里处理
	g_usbData.addData((char *)usbDataBuff,usbDataLen);
	
	//文件数据在USBdata里处理
	if (g_usbData.bHaveNext || \
		g_usbData.dataType_ != UsbData::dtData)
	{
		return false;
	}
	
	//处理收完的USB数据
	data = g_usbData.GetData();
	//解析数据
	result = jsonProcessor(data);
	
	if (result.compare(JSON_SENDFILE) == 0)	
	{
		//发送文件
		UsbData::SendFile();
	}
	else if (result.compare(JSON_NOFILE) == 0)
	{
		//没这个文件
		result = "B0001E00010";
		pSerial->WritePort((char *)result.c_str(),\
			result.size());
	}
	else
	{	
		//发送数据
		std::string v = Util::StringOp::ToUTF8(result);
		pSerial->WritePort((char *)v.c_str(),v.size(),true);
	}
	
	return true ;

}

DWORD WINAPI CPSerialPort::ReadPortThread(LPVOID lpParameter)
{		
	 DWORD           dwEvtMask;
	 int             usbDataLen = 0;
	 CPSerialPort*   pSerial;
	 pSerial=(CPSerialPort*)lpParameter;
	
	 SetCommMask(pSerial->m_hComm, EV_RXCHAR);
	 while((pSerial->m_hComm !=INVALID_HANDLE_VALUE))
	 {	    

           WaitCommEvent(pSerial->m_hComm,&dwEvtMask,NULL);
		   if((dwEvtMask & EV_RXCHAR) == EV_RXCHAR)
		   {				
				memset(usbDataBuff, 0, sizeof(usbDataBuff));
				//usbDataLen 收到的USB数据的长度
				ReadData((char *)usbDataBuff, usbDataLen);			
				
				if(usbDataLen > 0)
				{	

					if(!HandData(pSerial,usbDataLen))
						continue;
				}
		   }  
	 }

	return 0;
}

BOOL CPSerialPort::OpenPort(LPCTSTR Port,int BaudRate,int DataBits,int StopBits,int Parity)
{
	if(m_hComm ==INVALID_HANDLE_VALUE)
	{	
		//打开串口
		m_hComm=CreateFile(Port,GENERIC_READ|GENERIC_WRITE,0,0,OPEN_EXISTING,0,0);
		if(m_hComm==INVALID_HANDLE_VALUE )
		{
			return FALSE;
		}
	
	//
		GetCommState(m_hComm,&dcb);
        dcb.BaudRate=BaudRate;
		dcb.ByteSize=DataBits;
		dcb.Parity=Parity;
		dcb.StopBits=StopBits;
		dcb.fParity=FALSE;
		dcb.fBinary=TRUE;
		dcb.fDtrControl=0;
		dcb.fRtsControl=0;
		dcb.fOutX=dcb.fInX=dcb.fTXContinueOnXoff=0;

		//设置状态参数
		SetCommMask(m_hComm,EV_RXCHAR);		
		SetupComm(m_hComm,2048,2048);
		
		if(!SetCommState(m_hComm,&dcb))
		{
			PurgeComm(m_hComm,PURGE_TXCLEAR|PURGE_RXCLEAR);
			ClosePort();
			return FALSE;
		}

		//设置超时参数
		GetCommTimeouts(m_hComm,&CommTimeOuts);		
		CommTimeOuts.ReadIntervalTimeout=100;
		CommTimeOuts.ReadTotalTimeoutMultiplier=1;
		CommTimeOuts.ReadTotalTimeoutConstant=100;
		CommTimeOuts.WriteTotalTimeoutMultiplier=10;
		CommTimeOuts.WriteTotalTimeoutConstant=100;
		
		if(!SetCommTimeouts(m_hComm,&CommTimeOuts))
		{
			AfxMessageBox(_T("无法设置超时参数！"));
			PurgeComm(m_hComm,PURGE_TXCLEAR|PURGE_RXCLEAR);
			ClosePort();
			return FALSE;
		}

		PurgeComm(m_hComm,PURGE_TXCLEAR|PURGE_RXCLEAR);	
		

		memset(&m_osRead, 0, sizeof(OVERLAPPED));
		if ((m_osRead.hEvent=CreateEvent(NULL,TRUE,FALSE,NULL))==NULL)
		{
			return FALSE;
		}

		memset(&m_osWrite, 0, sizeof(OVERLAPPED));
		if ((m_osWrite.hEvent=CreateEvent(NULL,FALSE,TRUE,NULL))==NULL)
		{
			return FALSE;
		}

		return TRUE;		
	}
	
	return FALSE;
}

void CPSerialPort::ClosePort()
{
	Deactivate();

	if(m_hComm !=INVALID_HANDLE_VALUE)
	{
		SetCommMask(m_hComm,0);		
		PurgeComm(m_hComm,PURGE_TXCLEAR|PURGE_RXCLEAR);
		CloseHandle(m_hComm);
		m_hComm=INVALID_HANDLE_VALUE;
	}
	
	//清USB数据
	g_usbData.vData.clear();
	//关闭文件
	CloseFile();

}

BOOL CPSerialPort::Activate()
{
	if(m_hComm==INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}

	if(!m_bReceiving)
	{
		PurgeComm(m_hComm,PURGE_TXCLEAR|PURGE_RXCLEAR);
		m_bReceiving  = TRUE;
		m_hReadThread = CreateThread(NULL,0,ReadPortThread,this,0,NULL);
	}

	if(m_hReadThread != NULL)
	{		
		return TRUE;
	}
	else
	{
		m_bReceiving=FALSE;
		return FALSE;
	}

	return FALSE;
}

BOOL CPSerialPort::Deactivate()
{
	if(m_hComm==INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}

	if(m_bReceiving)
	{
		m_bReceiving=FALSE;
		WaitForSingleObject(m_hReadThread,500);
		CloseHandle(m_hReadThread);
		m_hReadThread=NULL;
		return TRUE;
	}

	return FALSE;
}

BOOL CPSerialPort::IsActive()
{
	return m_bReceiving;
}

bool CPSerialPort::WritePort(char *data,int length,bool addHead )
{   
	DWORD dwBytesWritten = 0;
	
	//add by qi 
	//一次性把数据全部发完
	int nWrite = 0 ;
	int nSize = min(SEND_BUFF_LENGTH,length);
	while (nWrite < length)
	{			
		char sendBuff[SEND_BUFF_LENGTH] = {0};
		//
		if (addHead)
		{	
			//添加头
			char head[PACKET_HEADER_LENGTH] = {0};
			//是否有下一包
			if (length - nWrite > nSize)
			{
				//有下一包
				sprintf(head,"B%04dE00001",nSize);
			}
			else 
			{
				//没下一包
				sprintf(head,"B%04dE00000",nSize);
			}
			memcpy(sendBuff,head,PACKET_HEADER_LENGTH);
			
			//填充数据
			memcpy(sendBuff+PACKET_HEADER_LENGTH,data+nWrite,nSize);
			nSize += PACKET_HEADER_LENGTH;

		}
		else
		{
			memcpy(sendBuff,data+nWrite,nSize);
		}
		
		//64位处理
		if (nSize % 64 == 0)
		{
			memcpy(sendBuff+nSize," ",1);
			nSize +=1;
		}
		
		if(m_hComm == INVALID_HANDLE_VALUE)
		{
			return 0 ;
		}
		//发送数据
		if (WriteFile(m_hComm,sendBuff,nSize,&dwBytesWritten,NULL))
		{
			if (addHead)
			{	
				//自动添加头，把头的长度去掉
				nWrite += dwBytesWritten - PACKET_HEADER_LENGTH;
			}
			else
			{
				nWrite += dwBytesWritten ;
			}

			nSize  =  min(SEND_BUFF_LENGTH,length - nWrite);	
			Sleep(5);
		}

	}

	return (nWrite == length);	
}

BOOL CPSerialPort::WriteSynPort(const void* lpBuf, DWORD dwLength)
{  
	BOOL fState;
	DWORD length=dwLength;
	COMSTAT ComStat;
	DWORD dwErrorFlags;
	char* buf=new char[m_nBufferSize];
	ClearCommError(m_hComm,&dwErrorFlags,&ComStat);
	fState=WriteFile(m_hComm,buf,length,&length,&m_osWrite);
	if (!fState)
	{
		if (GetLastError()==ERROR_IO_PENDING)
		{
			//GetOverlappedResult(m_hComm,&m_osWrite,&length,TRUE);
		}else
		{
			length=0;
			fState=FALSE;
		}
	}
	return fState;
}


BOOL CPSerialPort::ReadPort(char *data,int dwLength,int &readcount)
{   

	COMSTAT ComStat;	
	DWORD dwErrorFlags;
	BOOL fReadState=TRUE;	
    BOOL BuffState=TRUE;	
	DWORD length=0;	
	DWORD dwBytesRead = 0;	
	BuffState=ClearCommError(m_hComm,&dwErrorFlags,&ComStat);
	
	if (!BuffState)
	{
		return FALSE;		
	}
	else
	{		
		length=min(dwLength,ComStat.cbInQue);
	}
    
	fReadState = ReadFile(m_hComm, data,length*sizeof(char), &dwBytesRead, NULL);//length*sizeof(char)
		
	if (!fReadState)
	{  		
		fReadState=FALSE;
	}
	
	readcount=dwBytesRead;
	return fReadState;
}

BOOL CPSerialPort::ReadPort(char *data,int &readcount)
{   
	COMSTAT ComStat;	
	DWORD dwErrorFlags;	
    BOOL BuffState=TRUE;
	
	BuffState=ClearCommError(m_hComm,&dwErrorFlags,&ComStat);

	char  read_[READ_LENGTH+1] = {0};
	DWORD nBytesRead ;
	DWORD nAllRead = 0;
	
	ReadFile(m_hComm, read_, READ_LENGTH, &nBytesRead, NULL);	
	memcpy(data,read_,nBytesRead);
	readcount = nBytesRead;

//  
/*	do 
	{
	   ReadFile(m_hComm, read_, READ_LENGTH, &nBytesRead, NULL);
	   TRACE(L"ReadFile:%d\n",GetTickCount() - dBegin);
	   if (nBytesRead)
	   {				
		   memcpy(data+nAllRead,read_,nBytesRead);
		   nAllRead += nBytesRead;									
	   }

	} while (nBytesRead > 0);
	

	readcount = nAllRead;*/

	return (readcount > 0);

}

void CPSerialPort::ClearPort()
{
	PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);
}

BOOL CPSerialPort::ReadSynPort(void *lpBuf, DWORD dwLength )
{
	DWORD    length=0;
	COMSTAT  ComStat;
	DWORD    dwErrorFlags;
	BOOL     fReadState=TRUE;
	ClearCommError(m_hComm,&dwErrorFlags,&ComStat);

	length     = min(dwLength,ComStat.cbInQue);
	char* buf  = new char[m_nBufferSize];
	fReadState = ReadFile(m_hComm,buf,length,&length,&m_osRead);

	if (!fReadState)
	{
		if (GetLastError()==ERROR_IO_PENDING)
		{
			//GetOverlappedResult(m_hComm,&m_osRead,&length,TRUE);

		}else
		{
			length = 0;			
			fReadState = FALSE;
		}
	}
	
	return fReadState;
	
}
