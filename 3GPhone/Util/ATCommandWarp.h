#ifndef __UTIL_ATCOMMANDWARP_H__
#define __UTIL_ATCOMMANDWARP_H__

#include "SerialPort.h"
#include "ComWarp.h"
#include <string>
#include <vector>

namespace Util {
    class ATCommand {
    public:
        //Command : {AT|at}[{&|^}]cmd args<CR> //args : {n=[m]|a}
        //AT+<x>=? | AT+<x>? | AT+<x>=<...> | AT+<x>
        //{AT|or}cmd;cmd;cmd
        //A/ reexecute last command
        //Response : <CR><LF>response<CR><LF>
        ATCommand(unsigned int const portNo);
        typedef void (*Observer)(std::string const& data);
        void RegisterObserver(Observer ob);
        void Write(std::string const& data);
        std::string const Read();
        void Init3GModule();
        void Startup();
        void StartupWithPin(std::string const& pin);
        void RegisterNetwork(std::string const& pin);
        void UnregisterNetwork();
        void ShutDown();
        void Finally();
    private:
        void Init(unsigned int const portNo = 1, unsigned int const baud = 9600, unsigned int const parity = NOPARITY, unsigned int const databits = 8, unsigned int const stopbits = 1);
        static void disptch_(unsigned char const* const data, unsigned int const length);
        Util::RS232* serialPort_; // = 0;
        std::vector<Observer> observers_;

        enum State {
            sNull,
            sRequest,
            //sIndication,
            //sReqeustIndication,
            //sResponse,
            //sConfirmation,
            //sError,
        };
        volatile State state_; // = sNull;
    };
}

namespace Util
{
	class ATCommandWarp
	{
	public:
		ATCommandWarp();
		~ATCommandWarp();
		ATCommandWarp(const ATCommandWarp& ATWarp);

		void Bind(Util::ComWarp* com);
		bool Connect(std::string pin);

		bool Init(void);
		bool On(const char* pin);
		bool Off(void);

		bool PhoneDial(char * number);
		bool PhoneRedial(void);
		int PhoneState(void);
		bool PhoneRing(char * number, int * type);
		bool PhoneHangup(void);
		bool PhoneAnswer(void);
		bool PhoneSubDial(char number);

		bool SmsSend(int dataLength);
		int SmsSend(char* pdu, int pduLength);
		int SmsDelete(int index);
		int SmsReadList(void);
		int SmsReadResponse(void* pData, int nLength);
		int SmsWriteRequest(void* pData, int nLength);

	private:
		static bool bInit;
		Util::ComWarp* m_pCom;
	};
}


#endif // __UTIL_ATCOMMANDWARP_H__
