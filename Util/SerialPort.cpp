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
		// �Ѿ��򿪵Ļ���ֱ�ӷ���
		if (handle_ != INVALID_HANDLE_VALUE)
		{
			return true;
		}
		//ASSERT(portNo > 0 && portNo < 5);

		TCHAR szPort[15] = {0};	
		//���ô�����
		//wsprintf(szPort, L"COM%d:", portNo);
		wsprintf(szPort, L"TEL1:");
		//�򿪴���
	//	handle_ = ::CreateFile(szPort, GENERIC_READ | GENERIC_WRITE/*�������д*/, 0/*��ռ��ʽ������ģʽ��*/, NULL, OPEN_EXISTING, 0, NULL);
		handle_ = CreateFile(TEXT("TEL1:"), GENERIC_READ | GENERIC_WRITE/*�������д*/, 0/*��ռ��ʽ������ģʽ��*/, NULL, OPEN_EXISTING, 0, NULL);
	
		if (handle_ == INVALID_HANDLE_VALUE)
		{
			// ��Ч���,���ء�		
			//TRACE(_T("CreateFile ������Ч���"));
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
		//��ʾ���ڻ�û�д�
		if (handle_ == INVALID_HANDLE_VALUE)
		{
			return;
		}

		//�رն��߳�
		readThread_->willStop();
		readThread_->stop();

		//�رմ���
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

		//��鴮���Ƿ�򿪡�
		ASSERT(serialPort_->handle_ != INVALID_HANDLE_VALUE);

		//��մ���
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
