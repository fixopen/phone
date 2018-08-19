#ifndef ATCommandWarp_h
#define ATCommandWarp_h
#include "ComWarp.h"
#include <string>

namespace Util
{
	class ATCommandWarp
	{
	public:
		ATCommandWarp();
		~ATCommandWarp();
		ATCommandWarp(const ATCommandWarp& ATWarp);

//		static ATCommandWarp* GetATCommandWarp();

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
		bool PhoneVolume(unsigned int level);
		bool PhoneAutoAnswer(unsigned int second);

		bool SmsSend(int dataLength);
		int SmsSend(char* pdu, int pduLength);
		int SmsDelete(int index);
		int SmsReadList(void);
		int SmsReadResponse(void* pData, int nLength);
		int SmsWriteRequest(void* pData, int nLength);
		//std::string SmsCenterAddress(void);
		//bool SmsCenterAddress(std::string sca);

	private:
		static bool bInit;
		Util::ComWarp* m_pCom;
	};
}


#endif // ATCommandWarp_h
