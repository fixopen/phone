#include "SerialPort.h"
#include "Log.h"

namespace Util {
    RS232::RS232(void)
    : onSerialPortDataReceived_(0)
    //, readThread_(0)
    , buffer_(0)
    , bufferLength_(0)
    , handle_(INVALID_HANDLE_VALUE)
	, isVirtual_(false) {
    }

    void RS232::SetReadFunc(OnSerialPortDataReceived onSerialPortDataReceived) {
        onSerialPortDataReceived_ = onSerialPortDataReceived;
        readThread_ = Util::shared_ptr<ReadThread>(new ReadThread(this));
        readThread_->start();
        //Log::Log("serial port thread is start");
    }

    //void RS232::SetBuffer(unsigned char* buffer, unsigned int bufferLength) {
    //	buffer_ = buffer;
    //	bufferLength_ = bufferLength;
    //}

    bool RS232::OpenPort(unsigned int portNo, unsigned int baud, unsigned int parity, unsigned int databits, unsigned int stopbits) {
        // �Ѿ��򿪵Ļ���ֱ�ӷ���
        if (handle_ != INVALID_HANDLE_VALUE) {
            Log::Log("�����Ѿ��򿪣�ֱ�ӷ���");
            return true;
        }
        //ASSERT(portNo > 0 && portNo < 5);

        wchar_t portName[15] = {0};	
        //���ô�����
        wsprintf(portName, L"COM%d:", portNo);
        Log::Log(portName);
        //�򿪴���
        handle_ = ::CreateFile(portName, GENERIC_READ | GENERIC_WRITE/*�������д*/, 0/*��ռ��ʽ������ģʽ��*/, NULL, OPEN_EXISTING, 0, NULL);

        if (handle_ == INVALID_HANDLE_VALUE) {
            // ��Ч���,���ء�		
            Log::Log("CreateFile ������Ч���");
            return false;
        }

        if (baud != 0) {
            // �õ��򿪴��ڵĵ�ǰ���Բ������޸ĺ����������ô��ڡ�
            // ���ô��ڵĳ�ʱ����Ϊ�������ء�
            DCB commParam;
            if (!GetCommState(handle_, &commParam)) {
                Log::Log("��ȡ���ڲ���ʧ��");
                return false;
            }

            commParam.BaudRate = baud;					// ���ò����� 
            commParam.fBinary = TRUE;					// ���ö�����ģʽ���˴���������TRUE
            commParam.fParity = TRUE;					// ֧����żУ��
            commParam.ByteSize = databits;				// ����λ,��Χ:4-8
            commParam.Parity = NOPARITY;				// У��ģʽ
            commParam.StopBits = stopbits;				// ֹͣλ
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
            commParam.fAbortOnError = FALSE;			// �����ڷ������󣬲�����ֹ���ڶ�д

            if (!SetCommState(handle_, &commParam)) {
                Log::Log("SetCommState error");
                return false;
            }

            //���ô��ڶ�дʱ��
            COMMTIMEOUTS commTimeOuts;
            GetCommTimeouts(handle_, &commTimeOuts);
            commTimeOuts.ReadIntervalTimeout = MAXDWORD;
            commTimeOuts.ReadTotalTimeoutMultiplier = 0;
            commTimeOuts.ReadTotalTimeoutConstant = 0;
            commTimeOuts.WriteTotalTimeoutMultiplier = 10;
            commTimeOuts.WriteTotalTimeoutConstant = 1000;

            if (!SetCommTimeouts(handle_, &commTimeOuts)) {
                Log::Log("SetCommTimeouts ���ش���");
                return false;
            }

            //ָ���˿ڼ����¼���
            SetCommMask(handle_, EV_RXCHAR);

            //�����豸������
            SetupComm(handle_, 512, 512);

            //��ʼ���������е���Ϣ
            PurgeComm(handle_, PURGE_TXCLEAR | PURGE_RXCLEAR);
		} else {
			isVirtual_ = true;
		}

        //Log::Log("���ڴ򿪳ɹ�");

        return true;
    }

    bool RS232::WritePort(unsigned char const* const data, unsigned int length) {
        DWORD writedBytes;
        DWORD totalWritedBytes = 0;

        //ASSERT(handle_ != INVALID_HANDLE_VALUE);
        //char buffer[512] = {0};
        //sprintf(buffer, "Data length is %d", length);
        //Log::Log(buffer);

        do {
            if (WriteFile(handle_, data + totalWritedBytes, length - totalWritedBytes, &writedBytes, NULL)) {
                totalWritedBytes += writedBytes;
                //memset(buffer, 0, 512);
                //sprintf(buffer, "writedBytes length is %d", writedBytes);
                //Log::Log(buffer);
                if (totalWritedBytes == length) {
                    break;
                }
                //::Sleep(10);
            } else {
                return false;
            }
        } while (true);

        return true;
    }

    void RS232::ClosePort(void) {
        //��ʾ���ڻ�û�д�
        if (handle_ == INVALID_HANDLE_VALUE) {
            return;
        }

        //�رն��߳�
        readThread_->willStop();
        ::Sleep(60);
        readThread_->stop();

        //�رմ���
        if (!CloseHandle (handle_)) {
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

    int const RS232::ReadThread::run() {
        //��鴮���Ƿ�򿪡�
        //ASSERT(serialPort_->handle_ != INVALID_HANDLE_VALUE);
        //Log::Log("serial port thread is run");

        //��մ���
        ::PurgeComm(serialPort_->handle_, PURGE_RXCLEAR | PURGE_TXCLEAR);
        ::SetCommMask(serialPort_->handle_, EV_RXCHAR | EV_CTS | EV_DSR);
        DWORD evtMask = 0;
        DWORD readErrors;
        COMSTAT	commState;
        size_t const readBufferLength = 1024;
        unsigned char readBuffer[readBufferLength] = {0};
        while (!quit_) {
			if (serialPort_->isVirtual_) {
				unsigned long actualReadLen = 0;
				ReadFile(serialPort_->handle_, readBuffer, readBufferLength, &actualReadLen, 0);
				if (actualReadLen > 0) {
				    //char buffer[1248] = {0};
				    //sprintf(buffer, "receive data is %s", readBuffer);
				    //Log::Log(buffer);
				    if (serialPort_->onSerialPortDataReceived_) {
				        serialPort_->onSerialPortDataReceived_(readBuffer, (unsigned int const)actualReadLen);
				    }
				    memset(readBuffer, 0, 1024);
				} else {
				    Sleep(100);
				}
			} else {
				if (::WaitCommEvent(serialPort_->handle_, &evtMask, 0)) {
					::SetCommMask(serialPort_->handle_, EV_RXCHAR | EV_CTS | EV_DSR);
					if (evtMask & EV_RXCHAR) {
						::ClearCommError(serialPort_->handle_, &readErrors, &commState);
						int willReadLen = commState.cbInQue;
						if (willReadLen > 0) {
							unsigned long actualReadLen = 0;
							ReadFile(serialPort_->handle_, readBuffer, min(willReadLen, readBufferLength), &actualReadLen, 0);
							char buffer[1248] = {0};
							sprintf(buffer, "receive data is %s", readBuffer);
							Log::Log(buffer);
							if (serialPort_->onSerialPortDataReceived_) {
								serialPort_->onSerialPortDataReceived_(readBuffer, (unsigned int const)actualReadLen);
								//serialPort_->onSerialPortDataReceived_(totalBuffer, totalReadChars);
							}
							memset(readBuffer, 0, 1024);
						}
					}
				}
			}
        }
        return 0;
    }
}
