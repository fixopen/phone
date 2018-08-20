#include "stdafx.h"

#include "SerialPort.h"

namespace Util
{
	RS232::RS232(void)
		: onSerialPortDataReceived_(0)
		, buffer_(0)
		, bufferLength_(0)
		, handle_(INVALID_HANDLE_VALUE)
	{
	}

	void RS232::SetReadFunc(OnSerialPortDataReceived onSerialPortDataReceived)//?
	{
		onSerialPortDataReceived_ = onSerialPortDataReceived;
		readThread_ = boost::shared_ptr<ReadThread>(new ReadThread(this));
		readThread_->start();
	}

	bool RS232::OpenPort(unsigned int portNo, unsigned int baud, unsigned int parity, unsigned int databits, unsigned int stopbits)
	{
		// 已经打开的话，直接返回
		if (handle_ != INVALID_HANDLE_VALUE)
		{
			return true;
		}
		//ASSERT(portNo > 0 && portNo < 5);

		TCHAR szPort[15] = {0};	
		//设置串口名
		//wsprintf(szPort, L"COM%d:", portNo);
		wsprintf(szPort, L"TEL1:");
		//打开串口
	//	handle_ = ::CreateFile(szPort, GENERIC_READ | GENERIC_WRITE/*允许读和写*/, 0/*独占方式（共享模式）*/, NULL, OPEN_EXISTING, 0, NULL);
		handle_ = CreateFile(TEXT("TEL1:"), GENERIC_READ | GENERIC_WRITE/*允许读和写*/, 0/*独占方式（共享模式）*/, NULL, OPEN_EXISTING, 0, NULL);
	
		if (handle_ == INVALID_HANDLE_VALUE)
		{
			// 无效句柄,返回。		
			//TRACE(_T("CreateFile 返回无效句柄"));
			return false;
		}
		
		return true;
	}

	bool RS232::WritePort(unsigned char const* const data, unsigned int length)
	{
		DWORD writedBytes;
		DWORD totalWritedBytes = 0;

		ASSERT(handle_ != INVALID_HANDLE_VALUE);

		do
		{
			if (WriteFile(handle_, data + totalWritedBytes, length - totalWritedBytes, &writedBytes, NULL))
			{
				totalWritedBytes += writedBytes;
				if (totalWritedBytes == length)
				{
					break;
				}
				::Sleep(10);
			}
			else
			{
				return false;
			}
		} while (true);

		return true;
	}

	void RS232::ClosePort(void)
	{
		//表示串口还没有打开
		if (handle_ == INVALID_HANDLE_VALUE)
		{
			return;
		}

		//关闭读线程
		readThread_->willStop();
		readThread_->stop();

		//关闭串口
		if (!CloseHandle (handle_))
		{
			handle_ = INVALID_HANDLE_VALUE;
			return;
		}
	}

	RS232::ReadThread::ReadThread(RS232* serialPort)
		: serialPort_(serialPort)
		, quit_(false)
		, frameHeadPos_(-1)
		, isFirstPacket_(true)
	{
	}

	int const RS232::ReadThread::run(void)
	{
		//0 through 96 Reserved for real-time above drivers. 
		//97 through 152 Used by the default Windows CE-based device drivers. 
		//153 through 247 Reserved for real-time below drivers. 
		//248 through 255 Maps to non-real-time priorities 

		Dprintf("serial port start\n");

		//检查串口是否打开。
		ASSERT(serialPort_->handle_ != INVALID_HANDLE_VALUE);

		//清空串口
		::PurgeComm(serialPort_->handle_, PURGE_RXCLEAR | PURGE_TXCLEAR);
		::SetCommMask(serialPort_->handle_, EV_RXCHAR | EV_CTS | EV_DSR);

		DWORD evtMask = 0;
		DWORD dwReadErrors;
		COMSTAT	cmState;
		BYTE totalBuffer[20] = {0};
		int totalReadChars = 0;
		unsigned char readBuffer;//[1024] = {0};

		while (!quit_)
		{
			Sleep(0);
			int actualReadLen = 0;
			//	ReadFile(serialPort_->handle_, readBuffer, 1, (unsigned long*)&actualReadLen, 0);
			ReadFile(serialPort_->handle_, &readBuffer, 1, (unsigned long*)&actualReadLen, 0);
			if (serialPort_->onSerialPortDataReceived_)
			{
				serialPort_->onSerialPortDataReceived_(&readBuffer, actualReadLen);//mhq
			}
		}
		return 0;
	}
}
