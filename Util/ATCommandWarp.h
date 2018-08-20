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

		CRITICAL_SECTION m_csAT;

		int Connect_Test();
		void Bind(Util::ComWarp* com);
		int Connect(char* pin);
		bool Command(char* cmd, int len, char* out = NULL, int count = 10);
		int Transaction(char* req, int reqLen, char* res, int resLen);

		int Init(void);
		int On(char* pin);
		bool Off(void);

		bool PhoneDial(char * number);
		bool PhoneRedial(void);
		int PhoneState(void);
		bool PhoneRing(char * number, int * type);
		bool PhoneHangup(void);
		bool PhoneAnswer(void);
		bool PhoneSubDial(char number);
		bool PhoneVolume(unsigned int level);
		bool PhoneMute(bool isMute);
		bool PhoneAutoAnswer(unsigned int second);
		unsigned int PhoneSignalQuality(void);
		bool PhoneHandFree(bool isHandFree);
		std::string PhoneNumber(void);

		bool SmsSend(int dataLength);
		int SmsSend(char* pdu, int pduLength, char* ans, int ansLength);
		int SmsDelete(int index, char* ans, int ansLength);
		int SmsReadList(char* ans, int ansLength);
		int SmsReadResponse(void* pData, int nLength);
		int SmsWriteRequest(void* pData, int nLength);
		//std::string SmsCenterAddress(void);
		//bool SmsCenterAddress(std::string sca);

	private:
		static bool bInit;
		Util::ComWarp* m_pCom;
		std::string m_strPhoneNumber;
	};
}


#endif // ATCommandWarp_h
