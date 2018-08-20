#ifndef ATCommandWarp_h
#define ATCommandWarp_h
#include "ComWarp.h"
#include <string>
#include <vector>

struct PhoneCallTRans
{
//	BOOL isOn;
	std::string teccode;
	BOOL isNoAswer;
	BOOL isBusy;
	BOOL isUncondify;
	BOOL isNoTel;
};

struct PhoneCallLimit
{
//	BOOL isOn;
	BOOL isCallin;
	BOOL isCallout;
};

struct PhoneCallWaiting  
{
//	BOOL isOn;
	BOOL isCallWaiting;
};

typedef int (*pGetVideoData)(void *pData, int length);
typedef int (*pSendVideoData)(void *pData, int length);

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
		bool Command(char* cmd, int len, char* out = NULL, int count = 5, BOOL isWating = FALSE);
		bool Command_1(char* cmd, int len);
		int Transaction(char* req, int reqLen, char* res, int resLen, int nCount = 1);

		int Init(void);
		int On(char* pin);
		bool Off(void);

		bool PhoneDial(char * number, BOOL isVideo = FALSE);
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
		unsigned int PhoneNettype(void);
		bool PhoneHandFree(bool isHandFree);
		int ReportSCE();
		std::string PhoneNumber(void);
		void PhoneDialTone(BOOL isOn, char *tone = NULL);

		bool SmsSend(int dataLength);
		int SmsSend(char* pdu, int pduLength, char* ans, int ansLength);
		int SmsDelete(int index, char* ans, int ansLength);
		int SmsReadList(char* ans, int ansLength);
		int SmsReadResponse(void* pData, int nLength);
		int SmsWriteRequest(void* pData, int nLength);
		std::string SmsCenterAddress(void);
		bool SmsCenterAddress(std::string sca);


		//电话功能
		bool PhoneSetTrans(PhoneCallTRans calltrans);//设置呼叫转移
		void PhoneGetTrans(PhoneCallTRans &calltrans);//获取呼叫转移设置
		
		void PhoneSetCallLimit(PhoneCallLimit calllimit); //设置呼叫受限
		void PhoneGetCallLimit(PhoneCallLimit &calllimit); //获取呼叫受限设置
		bool CallLimit(const char *fac,bool able);//限制呼入呼出等功能 fac, "AI"限制呼入,"AO"限制呼出。其它的参照LC6311 ,AT+CLCK.
		
		bool PhoneSetCallWaiting(bool able);//设置呼叫等待
		bool PhoneGetCallWaiting(int &status);//获取呼叫等待设置
		bool PhoneCallSwitch(int n);//呼叫切换

		//视频电话功能
// 		bool PhoneVDial(char * number);						//视频拨打电话
// 		bool PhoneVRing(char * number, int type);           //视频振铃
// 		bool PhoneCCHL();

		BOOL  m_bIsVideoRun;
		BOOL  m_bIsVideoExit;

		static int GetVideoData(void *pData, int length);
		static int SendVideoData(void *pData, int length);

		//开始视频
		bool StartVideoPhone(Util::ComWarp *pVideoCom);
		//结束视频
		bool EndVideoPhone(Util::ComWarp *pVideoCom);

//add by qi 2009_07_22
		typedef struct SIM{
			std::string  name;
			std::string	 telnum;
		}SIM_FORMAT;
		bool ReadSimPhoneInfo(std::vector<SIM_FORMAT> &vsim);
		bool Save2Sim(std::vector<SIM_FORMAT> vsim);
		void GB2312toUSC2(const char *src,char *des,int srclength);
		void USC2toGB2312(char *src,char *des,int length);
		void ParseAnswerData(char *src,std::vector<SIM_FORMAT> &vsim);
		int  gsmBytes2String(const unsigned char* pSrc, char* pDst, int nSrcLength); 
		int  gsmString2Bytes(const char* pSrc, unsigned char* pDst, int nSrcLength); 
		int  EncodeGsm(const char *src,unsigned char *des,int srclength );
		int  DecodeGsm(const unsigned char *src,char *des,int srclength);
	
		void CheckPin();
		void InputPin(const char *pin);
		void InputPuk(const char *puk,const char *newpin);//
		bool InputPin2(const char *pin2);
		bool InputPuk2(const char *puk2,const char *newpin);
	
		bool LockSim( const char *password);//
		bool UnlockSim(const char *password);//
		bool GetSimStatus(int &status);
		bool ChangePin(const char *pin,const char *newpin);//
		bool ChangePin2(const char *pin,const char *newpin);//
		void HandlePin();
		void HandlePuk();

		//呼叫转移
		bool UnconditionalTransfer( bool able);//
		bool BusyTransfer(bool able);//
		bool NoReplyTransfer(bool able);//
		bool NoReachTranfer(bool able);//
		bool GetOperator(std::string &opeator);//获得当前网络运营商

		bool CommandFun5(char* cmd, int len, char* out = NULL, int count = 5);
		void TestCommand();
	private:
		static bool bInit;
		Util::ComWarp* m_pCom;

/*		Util::ComWarp* m_pSecondCom;     //20090807  加入一个通道用于视频可视通道*/

		std::string m_strPhoneNumber;
		std::string m_strSmsCentreAddress;
	};
}


#endif // ATCommandWarp_h
