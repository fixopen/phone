#include "SerialPort.h"
#include "Log.h"

namespace Util {
    RS232::RS232()
    : onSerialPortDataReceived_(0)
    , readThread_(0)
    , buffer_(0)
    , bufferLength_(0)
    , handle_(INVALID_HANDLE_VALUE)
    , isVirtual_(false) {
    }

    void RS232::SetReadFunc(OnSerialPortDataReceived const onSerialPortDataReceived, int const pri, size_t const readBufferLength) {
        onSerialPortDataReceived_ = onSerialPortDataReceived;
        readThread_ = new ReadThread(this, readBufferLength);
        readThread_->start(pri);
    }

    bool const RS232::Open(unsigned int portNo, unsigned int baud, unsigned int parity, unsigned int databits, unsigned int stopbits) {
        // 已经打开的话，直接返回
        if (handle_ != INVALID_HANDLE_VALUE) {
            Log::Log("串口已经打开，直接返回");
            return true;
        }

        wchar_t portName[15] = {0};	
        //设置串口名
        wsprintf(portName, L"COM%d:", portNo);
        Log::Log(Log::Entry(portName));
        //Log::Log(Log::Entry("串口%s", portName));
        //打开串口
        handle_ = ::CreateFile(portName, GENERIC_READ | GENERIC_WRITE/*允许读和写*/, 0/*独占方式（共享模式）*/, NULL, OPEN_EXISTING, 0, NULL);

        if (handle_ == INVALID_HANDLE_VALUE) {
            // 无效句柄,返回。		
            Log::Log(Log::Entry("CreateFile 返回无效句柄，串口%s打开失败", portName));
            return false;
        }

        if (baud != 0) {
            // 得到打开串口的当前属性参数，修改后再重新设置串口。
            // 设置串口的超时特性为立即返回。
            ::DCB commParam;
            if (!::GetCommState(handle_, &commParam)) {
                Log::Log("获取串口参数失败");
                return false;
            }

            commParam.BaudRate = baud;					// 设置波特率 
            commParam.fBinary = TRUE;					// 设置二进制模式，此处必须设置TRUE
            commParam.fParity = TRUE;					// 支持奇偶校验
            commParam.ByteSize = databits;				// 数据位,范围:4-8
            commParam.Parity = NOPARITY;				// 校验模式
            commParam.StopBits = stopbits;				// 停止位
            commParam.fOutxCtsFlow = FALSE;				// No CTS output flow control
            commParam.fOutxDsrFlow = FALSE;				// No DSR output flow control
            commParam.fDtrControl = DTR_CONTROL_ENABLE;
            // DTR flow control type
            commParam.fDsrSensitivity = FALSE;			// DSR sensitivity
            commParam.fTXContinueOnXoff = TRUE;			// XOFF continues Tx
            commParam.fOutX = FALSE;					// No XON/XOFF out flow control
            commParam.fInX = FALSE;						// No XON/XOFF in flow control
            commParam.fErrorChar = FALSE;				// Disable error replacement
            commParam.fNull = FALSE;					// Disable null stripping
            commParam.fRtsControl = RTS_CONTROL_ENABLE;
            // RTS flow control
            commParam.fAbortOnError = FALSE;			// 当串口发生错误，并不终止串口读写

            if (!::SetCommState(handle_, &commParam)) {
                Log::Log("SetCommState error");
                return false;
            }

            //设置串口读写时间
            ::COMMTIMEOUTS commTimeOuts;
            ::GetCommTimeouts(handle_, &commTimeOuts);
            commTimeOuts.ReadIntervalTimeout = MAXDWORD;
            commTimeOuts.ReadTotalTimeoutMultiplier = 0;
            commTimeOuts.ReadTotalTimeoutConstant = 0;
            commTimeOuts.WriteTotalTimeoutMultiplier = 10;
            commTimeOuts.WriteTotalTimeoutConstant = 1000;

            if (!::SetCommTimeouts(handle_, &commTimeOuts)) {
                Log::Log("SetCommTimeouts 返回错误");
                return false;
            }

            //指定端口监测的事件集
            ::SetCommMask(handle_, EV_RXCHAR);

            //分配设备缓冲区
            ::SetupComm(handle_, 512, 512);

            //初始化缓冲区中的信息
            ::PurgeComm(handle_, PURGE_TXCLEAR | PURGE_RXCLEAR);
    	} else {
    		isVirtual_ = true;
    	}

        //Log::Log("串口打开成功");

        return true;
    }

    bool const RS232::Write(unsigned char const* const data, unsigned int length) {
        DWORD writedBytes;
        DWORD totalWritedBytes = 0;

        if (handle_ == INVALID_HANDLE_VALUE) {
            return false;
        }
        do {
            if (::WriteFile(handle_, data + totalWritedBytes, length - totalWritedBytes, &writedBytes, NULL)) {
                totalWritedBytes += writedBytes;
                if (totalWritedBytes == length) {
                    break;
                }
            } else {
                return false;
            }
        } while (true);

        return true;
    }

    void RS232::Close() {
        //表示串口还没有打开
        if (handle_ == INVALID_HANDLE_VALUE) {
            return;
        }

        //关闭读线程
        readThread_->willStop();
        ::Sleep(60);
        readThread_->stop();

        //关闭串口
        if (!::CloseHandle (handle_)) {
            handle_ = INVALID_HANDLE_VALUE;
            return;
        }
    }

    RS232::ReadThread::ReadThread(RS232* const serialPort, size_t const readBufferLength)
    : serialPort_(serialPort)
    , quit_(false)
    , frameHeadPos_(-1)
    , isFirstPacket_(true)
    , readBufferLength_(readBufferLength) {
    }

    namespace {
        size_t const readBufferLength = 32 * 1024;
        unsigned char readBuffer[readBufferLength] = {0};
    }

    int const RS232::ReadThread::run() {
        //检查串口是否打开。
        //ASSERT(serialPort_->handle_ != INVALID_HANDLE_VALUE);

        while (!quit_) {
        	if (serialPort_->isVirtual_) {
	            unsigned long actualReadLen = 0;
	            ::ReadFile(serialPort_->handle_, readBuffer, readBufferLength_, &actualReadLen, 0);
	            if (actualReadLen > 0) {
	                if (serialPort_->onSerialPortDataReceived_) {
	                    serialPort_->onSerialPortDataReceived_(readBuffer, (unsigned int const)actualReadLen);
	                }
                    memset(readBuffer, 0, 32 * 1024);
	            } else {
	                Sleep(0);
	            }
			} else {
                //清空串口
                ::PurgeComm(serialPort_->handle_, PURGE_RXCLEAR | PURGE_TXCLEAR);
                ::SetCommMask(serialPort_->handle_, EV_RXCHAR | EV_CTS | EV_DSR);
	            DWORD evtMask = 0;
	            if (::WaitCommEvent(serialPort_->handle_, &evtMask, 0)) {
	                ::SetCommMask(serialPort_->handle_, EV_RXCHAR | EV_CTS | EV_DSR);
	                if (evtMask & EV_RXCHAR) {
	                    DWORD readErrors;
	                    COMSTAT commState;
	                    ::ClearCommError(serialPort_->handle_, &readErrors, &commState);
	                    int willReadLen = commState.cbInQue;
	                    if (willReadLen > 0) {
	                        unsigned long actualReadLen = 0;
	                        ::ReadFile(serialPort_->handle_, readBuffer, min(willReadLen, readBufferLength), &actualReadLen, 0);
	                        char buffer[1248] = {0};
	                        sprintf_s(buffer, 1248, "receive data is %s", readBuffer);
	                        Log::Log(buffer);
	                        if (serialPort_->onSerialPortDataReceived_) {
	                            serialPort_->onSerialPortDataReceived_(readBuffer, (unsigned int const)actualReadLen);
	                            //serialPort_->onSerialPortDataReceived_(totalBuffer, totalReadChars);
	                        }
                            memset(readBuffer, 0,16 * 1024);
	                    }
	                }
	            }
	    	}
        }
        return 0;
    }
}
