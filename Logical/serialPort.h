#ifndef __UTIL_SERIALPORT_H__
#define __UTIL_SERIALPORT_H__

#include "../Util/Thread.h"
#include "../Util/SmartPtr.h"

namespace Util
{
	typedef void (CALLBACK* OnSerialPortDataReceived)(unsigned char const* const data, unsigned int const length);
	class RS232
	{
		class ReadThread;
		friend ReadThread;
	public:
		RS232(void);
		~RS232(void)
		{
			readThread_->willStop();
			readThread_->stop();
			ClosePort();
		}
		void SetReadFunc(OnSerialPortDataReceived onSerialPortDataReceived);
		//void SetBuffer(unsigned char* buffer, unsigned int bufferLength);
		bool OpenPort(unsigned int portNo = 1, unsigned int baud = 9600, unsigned int parity = NOPARITY, unsigned int databits = 8, unsigned int stopbits = 1);
		bool WritePort(unsigned char const* const data, unsigned int length);
		void ClosePort(void);
		void Bind(HANDLE handle_);//add by qi 0716
		HANDLE Handle();//add by qi 0716
		void ClearWriteCom();
	private:
		RS232(RS232 const& rhs);
		RS232& operator=(RS232& rhs);
	private:
		class ReadThread : public Util::Thread
		{
		public:
			ReadThread(RS232* serialPort);
			virtual int const run(void);
			void willStop(void)
			{
				quit_ = true;
			}
		private:
			int const findSync_(unsigned char const* const data, size_t const length);
		private:
			RS232* serialPort_;
			volatile bool quit_;
			int frameHeadPos_;
			bool isFirstPacket_;
		};
	//	friend class ATCommand;

	private:
		OnSerialPortDataReceived onSerialPortDataReceived_;
		unsigned char* buffer_;
		unsigned int bufferLength_;
		HANDLE handle_;
	public:
		boost::shared_ptr<ReadThread> readThread_;

	};
}

#endif //__UTIL_SERIALPORT_H__
