#ifndef ATCommandWarp_h
#define ATCommandWarp_h
#include "ComWarp.h"
#include "../UTIL_/ATCommandWarp_.h"
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
	class ATCommandWarp:public Util::Observer
	{
	public:
		ATCommandWarp();
		~ATCommandWarp();
		ATCommandWarp(const ATCommandWarp& ATWarp);

		CRITICAL_SECTION m_csAT;
		CRITICAL_SECTION m_csCPBR;

		int  Connect_Test();
		void Bind(Util::ComWarp* com);
		int  Connect(char* pin);
		bool Command(char* cmd, int len, char* out = NULL, int count = 5,BOOL isWating = FALSE);
		bool Command_1(char* cmd, int len);
		int	 Transaction(char* req, int reqLen, char* res, int resLen, int nCount = 1,bool bwait = false);
		void Transaction_(char* req, int reqLen = 0 ,char const *condition = "" );

		int  Init(void);
		int  On(char* pin);
		void Off(void);

		void PhoneDial(char * number, BOOL isVideo = FALSE);
		void PhoneRedial(void);
		int	 PhoneState(void);
		void PhoneRing(char * number, int * type);
		void PhoneHangup(void);
		void PhoneAnswer(void);
		void PhoneSubDial(char number);
		bool PhoneVolume(unsigned int level);
		void PhoneMute(bool isMute);
		void PhoneAutoAnswer(unsigned int second);
		void PhoneSignalQuality(void);
		void PhoneNettype(void);
		void PhoneHandFree(bool isHandFree);
		void ReportSCE();
		std::string PhoneNumber(void);
		void PhoneDialTone(BOOL isOn, char *tone = NULL);

		void SmsSend(int dataLength);
		int SmsSend(char* pdu, int pduLength, char* ans, int ansLength);
		int SmsDelete(int index, char* ans, int ansLength);
		int SmsReadList(char* ans, int ansLength);
		int SmsReadResponse(void* pData, int nLength);
		int SmsWriteRequest(void* pData, int nLength);
		std::string SmsCenterAddress(void);
		bool SmsCenterAddress(std::string sca);


		//电话功能
		void PhoneSetTrans(PhoneCallTRans calltrans);//设置呼叫转移
		void CallTransferCancel(int type);//设置取消
		void CallTransferStart(int type, std::string const number,int second = 0);
		void CallTransferStatus(int type);//获取呼叫转移设置
		
		void PhoneSetCallLimit(PhoneCallLimit calllimit); //设置呼叫受限
		void PhoneGetCallLimit(PhoneCallLimit &calllimit); //获取呼叫受限设置
		void CallLimit(const char *fac,bool able);//限制呼入呼出等功能 fac, "AI"限制呼入,"AO"限制呼出。其它的参照LC6311 ,AT+CLCK.

		bool PhoneSetCallWaiting(bool able);//设置呼叫等待
		bool PhoneGetCallWaiting(int &status);//获取呼叫等待设置
		void PhoneCallSwitch(int n);//呼叫切换

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
			std::string  id;//信息在卡中的位置
			std::string  name;
			std::string	 telnum;
		}SIM_FORMAT;
		void SimContactNum();
		void ReadSimContent(int const nstart);
		void ResetGSM();
		void ReadSimContact();
		void ParseAnswerData(char *src,bool bgsm = true);

		void Save2Sim(std::vector<SIM_FORMAT> vsim);
		void DeleteSim(int id);//删除SIM卡中的名片
		void GB2312toUSC2(const char *src,char *des,int srclength);
		void USC2toGB2312(char *src,char *des,int length);
		int  gsmBytes2String(const unsigned char* pSrc, char* pDst, int nSrcLength); 
		int  gsmString2Bytes(const char* pSrc, unsigned char* pDst, int nSrcLength); 
		int  EncodeGsm(const char *src,unsigned char *des,int srclength );
		int  DecodeGsm(const unsigned char *src,char *des,int srclength);
		int  GetSimCapacity();//获得SIM卡的容量

		bool CheckPin();
		void InputPin(const char *pin);
		void InputPuk(const char *puk,const char *newpin);//
		bool InputPin2(const char *pin2);
		bool InputPuk2(const char *puk2,const char *newpin);
	
		bool LockSim( const char *password);//对SIM卡加锁
		bool UnlockSim(const char *password);//对SIM解锁
		bool GetSimStatus(int &status);//获得SIM卡状态
		bool ChangePin(const char *pin,const char *newpin);//改SIM卡密码
		bool ChangePin2(const char *pin,const char *newpin);//
		bool HandlePin();
		bool HandlePuk();
		
		//呼叫转移
		void UnconditionalTransfer( bool able);//
		void BusyTransfer(bool able);//
		void NoReplyTransfer(bool able);//
		void NoReachTranfer(bool able);//
		bool GetOperator(std::string &opeator);//获得当前网络运营商
		bool CommandFun5(char* cmd, int len, char* out = NULL, int count = 5);
		bool CommandFun5_(char* cmd, int len,char* out = NULL);

		
		void HandleSIM(bool binsert);
		void Command_2(char *cmd,int len,char *out,int outlen ,char *stop);
		bool CheckCSCS(char *code);//检查编码集

		//Parse
		virtual void Update(std::string const& data);
		void ParseDAUDO(std::string const& data);
		void ParseCLCK(std::string const& data);
		void ParseCPBS(std::string const& data);
		void ParseCPBR(std::string const& data);
		void ParseCCFC(std::string const& data);
		void ParseCCWA(std::string const& data);

	private:
		enum   clcktype
		{
			sim,//sim卡
			ai,//呼入
			ao,//呼出
		};

		int				m_nContactNum;
		int				m_nCCFCtype;
		int				m_iContact;
		PhoneCallTRans  m_calltrans;

		clcktype		m_type;		
		int				m_nDAUDO ;
		bool			m_bgsm ;
		static bool		bInit;
		Util::ComWarp* m_pCom;

/*		Util::ComWarp* m_pSecondCom;     //20090807  加入一个通道用于视频可视通道*/

		std::string m_strPhoneNumber;
		std::string m_strSmsCentreAddress;

		//@@for debug
		int m_index;
	};
}


#endif // ATCommandWarp_h
