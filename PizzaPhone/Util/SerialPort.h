#ifndef __UTIL_SERIALPORT_H__
#define __UTIL_SERIALPORT_H__

#include "Thread.h"
#include "SmartPtr.h"
#include <winbase.h> //for NOPARITY ONESTOPBIT HANDLE

namespace Util {
    typedef void (CALLBACK* OnSerialPortDataReceived)(unsigned char const* const data, unsigned int const length);
    class RS232 {
        class ReadThread;
        friend ReadThread;
    public:
        RS232();
        ~RS232() {
            if (handle_ != INVALID_HANDLE_VALUE) {
                Close();
            }
        }
        void SetReadFunc(OnSerialPortDataReceived const onSerialPortDataReceived, int const pri = 251, size_t const readBufferLength = 32 * 1024);
        bool const Open(unsigned int portNo = 1, unsigned int baud = 9600, unsigned int parity = NOPARITY, unsigned int databits = 8, unsigned int stopbits = ONESTOPBIT);
        bool const Write(unsigned char const* const data, unsigned int length);
        bool const Write(std::string const& data) {
            return Write((unsigned char const* const)data.data(), data.length());
        }
        void Close();
    private:
        RS232(RS232 const& rhs);
        RS232& operator=(RS232& rhs);
    private:
        class ReadThread : public Util::Thread {
        public:
            ReadThread(RS232* const serialPort, size_t const readBufferLength);
            virtual int const run();
            void willStop(void) {
                quit_ = true;
            }
        private:
            int const findSync_(unsigned char const* const data, size_t const length);
        private:
            RS232* serialPort_;
            volatile bool quit_;
            int frameHeadPos_;
            bool isFirstPacket_;
            size_t readBufferLength_;
        };
    private:
        OnSerialPortDataReceived onSerialPortDataReceived_;
        ReadThread* readThread_;
        unsigned char* buffer_;
        unsigned int bufferLength_;
        bool isVirtual_;
    public:
        HANDLE handle_;
    };
}

#endif //__UTIL_SERIALPORT_H__
