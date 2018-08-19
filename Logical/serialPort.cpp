#include "stdafx.h"

#include "SerialPort.h"

namespace Util
{
    RS232::RS232(void)
    : onSerialPortDataReceived_(0)
        //, readThread_(0)

    , buffer_(0)
    , bufferLength_(0)
    , handle_(INVALID_HANDLE_VALUE) {
    }

    void RS232::SetReadFunc(OnSerialPortDataReceived onSerialPortDataReceived) {
        onSerialPortDataReceived_ = onSerialPortDataReceived;
        readThread_ = boost::shared_ptr<ReadThread>(new ReadThread(this));
        readThread_->start();
    }

    //void RS232::SetBuffer(unsigned char* buffer, unsigned int bufferLength)
    //{
    //  buffer_ = buffer;
    //  bufferLength_ = bufferLength;
    //}

    bool RS232::OpenPort(unsigned int portNo, unsigned int baud, unsigned int parity, unsigned int databits, unsigned int stopbits) {
        // 已经打开的话，直接返回
        if (handle_ != INVALID_HANDLE_VALUE) {
            return true;
        }
        //ASSERT(portNo > 0 && portNo < 5);

        TCHAR szPort[15] = {
            0
        };  
        //设置串口名
        wsprintf(szPort, L"COM%d:", portNo);
        //打开串口
        handle_ = ::CreateFile(szPort, GENERIC_READ | GENERIC_WRITE/*允许读和写*/, 0/*独占方式（共享模式）*/, NULL, OPEN_EXISTING, 0, NULL);

        if (handle_ == INVALID_HANDLE_VALUE) {
            // 无效句柄,返回。      
            //TRACE(_T("CreateFile 返回无效句柄"));
            return false;
        }

        // 得到打开串口的当前属性参数，修改后再重新设置串口。
        // 设置串口的超时特性为立即返回。
        DCB commParam;
        if (!GetCommState(handle_, &commParam)) {
            return false;
        }

        commParam.BaudRate = baud;                  // 设置波特率 
        commParam.fBinary = TRUE;                   // 设置二进制模式，此处必须设置TRUE
        commParam.fParity = TRUE;                   // 支持奇偶校验 
        commParam.ByteSize = databits;              // 数据位,范围:4-8 
        commParam.Parity = NOPARITY;                // 校验模式
        commParam.StopBits = stopbits;              // 停止位 
        commParam.fOutxCtsFlow = FALSE;             // No CTS output flow control 
        commParam.fOutxDsrFlow = FALSE;             // No DSR output flow control 
        commParam.fDtrControl = DTR_CONTROL_ENABLE; 
        // DTR flow control type 
        commParam.fDsrSensitivity = FALSE;          // DSR sensitivity 
        commParam.fTXContinueOnXoff = TRUE;         // XOFF continues Tx 
        commParam.fOutX = FALSE;                    // No XON/XOFF out flow control 
        commParam.fInX = FALSE;                     // No XON/XOFF in flow control 
        commParam.fErrorChar = FALSE;               // Disable error replacement 
        commParam.fNull = FALSE;                    // Disable null stripping 
        commParam.fRtsControl = RTS_CONTROL_ENABLE; 
        // RTS flow control 
        commParam.fAbortOnError = FALSE;            // 当串口发生错误，并不终止串口读写

        if (!SetCommState(handle_, &commParam)) {
            //TRACE(_T("SetCommState error"));
            return false;
        }

        //设置串口读写时间
        COMMTIMEOUTS CommTimeOuts;
        GetCommTimeouts(handle_, &CommTimeOuts);
        CommTimeOuts.ReadIntervalTimeout = MAXDWORD;
        CommTimeOuts.ReadTotalTimeoutMultiplier = 0;
        CommTimeOuts.ReadTotalTimeoutConstant = 0;
        CommTimeOuts.WriteTotalTimeoutMultiplier = 10;
        CommTimeOuts.WriteTotalTimeoutConstant = 1000;

        if (!SetCommTimeouts(handle_, &CommTimeOuts)) {
            //TRACE(_T("SetCommTimeouts 返回错误"));
            return false;
        }

        //指定端口监测的事件集
        SetCommMask(handle_, EV_RXCHAR);

        //分配设备缓冲区
        SetupComm(handle_, 512, 512);

        //初始化缓冲区中的信息
        PurgeComm(handle_, PURGE_TXCLEAR | PURGE_RXCLEAR);

        //TRACE(_T("串口打开成功"));

        return true;
    }

    bool RS232::WritePort(unsigned char const * const data, unsigned int length) {
        DWORD writedBytes;
        DWORD totalWritedBytes = 0;

        ASSERT(handle_ != INVALID_HANDLE_VALUE);

        do {
            if (WriteFile(handle_, data + totalWritedBytes, length - totalWritedBytes, &writedBytes, NULL)) {
                totalWritedBytes += writedBytes;
                if (totalWritedBytes == length) {
                    break;
                }
                //  ::Sleep(10);
            } else {
                return false;
            }
        } while (true);

        return true;
    }

    void RS232::ClosePort(void) {
        //表示串口还没有打开
        if (handle_ == INVALID_HANDLE_VALUE) {
            return;
        }

        //关闭读线程
        readThread_->willStop();
        readThread_->stop();

        //关闭串口
        if (!CloseHandle(handle_)) {
            handle_ = INVALID_HANDLE_VALUE;
            return;
        }
    }

    RS232::ReadThread::ReadThread(RS232* serialPort)
    : serialPort_(serialPort)
    , quit_(false)
    , frameHeadPos_(-1)
    , isFirstPacket_(true) {
    }

    int const RS232::ReadThread::run(void) {
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
        COMSTAT cmState;
        BYTE totalBuffer[20] = {
            0
        };
        int totalReadChars = 0;
        unsigned char readBuffer[1024] = {
            0
        };
        while (!quit_) {
            Sleep(10);
            //printf("serial port start\n");
            if (::WaitCommEvent(serialPort_->handle_, &evtMask, 0)) {
                ::SetCommMask(serialPort_->handle_, EV_RXCHAR | EV_CTS | EV_DSR);
                if (evtMask & EV_RXCHAR) {
                    ::ClearCommError(serialPort_->handle_, &dwReadErrors, &cmState);
                    int willReadLen = cmState.cbInQue;
                    //printf("*%d", willReadLen); //hardware bytes count \n
                    if (willReadLen > 0) {
                        int actualReadLen = 0;
                        ReadFile(serialPort_->handle_, readBuffer, min(willReadLen, 1024), (unsigned long*)&actualReadLen, 0);
                        //                      if (frameHeadPos_ == -1)
                        //                      {
                        //                          for (int i = 0; i < actualReadLen; ++i)
                        //                          {
                        //                              if (readBuffer[i] == 0xFF)
                        //                              {
                        //                                  frameHeadPos_ = i;
                        //                                  isFirstPacket_ = true;
                        //                                  totalReadChars = 0;
                        //                                  break;
                        //                              }
                        //                          }
                        //                      }
                        //                      if (isFirstPacket_)
                        //                      {
                        //                          memcpy(totalBuffer + totalReadChars, readBuffer + frameHeadPos_, min(actualReadLen - frameHeadPos_, 20 - totalReadChars));
                        //                          totalReadChars += min(actualReadLen - frameHeadPos_, 20 - totalReadChars);
                        //                          isFirstPacket_ = false;
                        //                      }
                        //                      else
                        //                      {
                        //                          memcpy(totalBuffer + totalReadChars, readBuffer, min(actualReadLen, 20 - totalReadChars));
                        //                          totalReadChars += min(actualReadLen, 20 - totalReadChars);
                        //                          if (totalReadChars >= 2)
                        //                          {
                        //                              if (totalBuffer[1] != 0xAA)
                        //                              {
                        //                                  frameHeadPos_ = -1;
                        //                              }
                        //                          }
                        //                          if (totalReadChars >= 3)
                        //                          {
                        //                              if (totalBuffer[2] != 0x55)
                        //                              {
                        //                                  frameHeadPos_ = -1;
                        //                              }
                        //                          }
                        //                      }
                        //                      if (totalReadChars >= 20)
                        //                      {
                        //                          frameHeadPos_ = -1;
                        //                          isFirstPacket_ = true;

                        if (serialPort_->onSerialPortDataReceived_) {
                            serialPort_->onSerialPortDataReceived_(readBuffer, actualReadLen);//mhq
                            //                              serialPort_->onSerialPortDataReceived_(totalBuffer, totalReadChars);
                        }
                        memset(readBuffer, 0, 1024);
                        //                          totalReadChars = 0;
                        //                      }
                    }
                }
            }
        }
        Dprintf("serial port stop\n");
        return 0;
    }
}
