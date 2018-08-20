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


		//�绰����
		bool PhoneSetTrans(PhoneCallTRans calltrans);//���ú���ת��
		void PhoneGetTrans(PhoneCallTRans &calltrans);//��ȡ����ת������
		
		void PhoneSetCallLimit(PhoneCallLimit calllimit); //���ú�������
		void PhoneGetCallLimit(PhoneCallLimit &calllimit); //��ȡ������������
		bool CallLimit(const char *fac,bool able);//���ƺ�������ȹ��� fac, "AI"���ƺ���,"AO"���ƺ����������Ĳ���LC6311 ,AT+CLCK.
		
		bool PhoneSetCallWaiting(bool able);//���ú��еȴ�
		bool PhoneGetCallWaiting(int &status);//��ȡ���еȴ�����
		bool PhoneCallSwitch(int n);//�����л�

		//��Ƶ�绰����
// 		bool PhoneVDial(char * number);						//��Ƶ����绰
// 		bool PhoneVRing(char * number, int type);           //��Ƶ����
// 		bool PhoneCCHL();

		BOOL  m_bIsVideoRun;
		BOOL  m_bIsVideoExit;

		static int GetVideoData(void *pData, int length);
		static int SendVideoData(void *pData, int length);

		//��ʼ��Ƶ
		bool StartVideoPhone(Util::ComWarp *pVideoCom);
		//������Ƶ
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

		//����ת��
		bool UnconditionalTransfer( bool able);//
		bool BusyTransfer(bool able);//
		bool NoReplyTransfer(bool able);//
		bool NoReachTranfer(bool able);//
		bool GetOperator(std::string &opeator);//��õ�ǰ������Ӫ��

		bool CommandFun5(char* cmd, int len, char* out = NULL, int count = 5);
		void TestCommand();
	private:
		static bool bInit;
		Util::ComWarp* m_pCom;

/*		Util::ComWarp* m_pSecondCom;     //20090807  ����һ��ͨ��������Ƶ����ͨ��*/

		std::string m_strPhoneNumber;
		std::string m_strSmsCentreAddress;
	};
}


#endif // ATCommandWarp_h
