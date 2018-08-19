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


		//�绰����
		void PhoneSetTrans(PhoneCallTRans calltrans);//���ú���ת��
		void CallTransferCancel(int type);//����ȡ��
		void CallTransferStart(int type, std::string const number,int second = 0);
		void CallTransferStatus(int type);//��ȡ����ת������
		
		void PhoneSetCallLimit(PhoneCallLimit calllimit); //���ú�������
		void PhoneGetCallLimit(PhoneCallLimit &calllimit); //��ȡ������������
		void CallLimit(const char *fac,bool able);//���ƺ�������ȹ��� fac, "AI"���ƺ���,"AO"���ƺ����������Ĳ���LC6311 ,AT+CLCK.

		bool PhoneSetCallWaiting(bool able);//���ú��еȴ�
		bool PhoneGetCallWaiting(int &status);//��ȡ���еȴ�����
		void PhoneCallSwitch(int n);//�����л�

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
			std::string  id;//��Ϣ�ڿ��е�λ��
			std::string  name;
			std::string	 telnum;
		}SIM_FORMAT;
		void SimContactNum();
		void ReadSimContent(int const nstart);
		void ResetGSM();
		void ReadSimContact();
		void ParseAnswerData(char *src,bool bgsm = true);

		void Save2Sim(std::vector<SIM_FORMAT> vsim);
		void DeleteSim(int id);//ɾ��SIM���е���Ƭ
		void GB2312toUSC2(const char *src,char *des,int srclength);
		void USC2toGB2312(char *src,char *des,int length);
		int  gsmBytes2String(const unsigned char* pSrc, char* pDst, int nSrcLength); 
		int  gsmString2Bytes(const char* pSrc, unsigned char* pDst, int nSrcLength); 
		int  EncodeGsm(const char *src,unsigned char *des,int srclength );
		int  DecodeGsm(const unsigned char *src,char *des,int srclength);
		int  GetSimCapacity();//���SIM��������

		bool CheckPin();
		void InputPin(const char *pin);
		void InputPuk(const char *puk,const char *newpin);//
		bool InputPin2(const char *pin2);
		bool InputPuk2(const char *puk2,const char *newpin);
	
		bool LockSim( const char *password);//��SIM������
		bool UnlockSim(const char *password);//��SIM����
		bool GetSimStatus(int &status);//���SIM��״̬
		bool ChangePin(const char *pin,const char *newpin);//��SIM������
		bool ChangePin2(const char *pin,const char *newpin);//
		bool HandlePin();
		bool HandlePuk();
		
		//����ת��
		void UnconditionalTransfer( bool able);//
		void BusyTransfer(bool able);//
		void NoReplyTransfer(bool able);//
		void NoReachTranfer(bool able);//
		bool GetOperator(std::string &opeator);//��õ�ǰ������Ӫ��
		bool CommandFun5(char* cmd, int len, char* out = NULL, int count = 5);
		bool CommandFun5_(char* cmd, int len,char* out = NULL);

		
		void HandleSIM(bool binsert);
		void Command_2(char *cmd,int len,char *out,int outlen ,char *stop);
		bool CheckCSCS(char *code);//�����뼯

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
			sim,//sim��
			ai,//����
			ao,//����
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

/*		Util::ComWarp* m_pSecondCom;     //20090807  ����һ��ͨ��������Ƶ����ͨ��*/

		std::string m_strPhoneNumber;
		std::string m_strSmsCentreAddress;

		//@@for debug
		int m_index;
	};
}


#endif // ATCommandWarp_h
