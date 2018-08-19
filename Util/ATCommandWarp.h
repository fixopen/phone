#ifndef ATCommandWarp_h
#define ATCommandWarp_h
#include "ComWarp.h"
#include "../UTIL_/ATCommandWarp_.h"
#include <string>
#include <vector>
#include <deque>

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

enum OKStatus{
	okIdle,
	okNoHandle,
	okAtd,
	okCTA,
	okChld,
	okCcfcregister,
	okCcfcUnconditionReg,
	okCcfcCancel,
	okCcfcUnconditionCancel,
	okCcfcCheck,
	okDdtm,
	okOpenPin,
	okClosePin,
	okPuk,
	okPuk2,
	okChangepin,
	okChangepin2,
	okCrsm,
	okChangecCsca,
	okInputpin,
	okInputpin2,
	okDsatref,
	okCsim,
	okSmsWrite,
	okSmsSend,
	okCmglRead1,
	okCmglRead2,
	okCmglRead3,
	okCmglRead4,
	okCmglRecive,
	okCcwaAble,
	okCcwaCancel,
	okCcwaCheck,
	okCmgsSmsSend,
	okCimi,
	okCsqRegister,
	okCsqSearch,
	okCpolDel,
	okCpolAdd,
	okCpolChange,
	okCpbsSelect,
	okCpbsCheck,
	okCpbwAdd,
	okCpbwChange,
	okCpbwDelete,
	okDsactActive,
	okDsactDeactive,
	okDsactQuery,
	okCpbr,
	okCnmiOpen,
	okCnmiClose,
};//�������������ϱ�Ok��Ϣ��

enum SMSSendStatus{smsSendL, smsSendC, smsSendSIML, smsSendSIMC, smsSendNULL};

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

		CRITICAL_SECTION m_csTranch;
		CRITICAL_SECTION m_csOk;

		int  Connect_Test();
		void Bind(Util::ComWarp* com);
		int  Connect(char* pin, BOOL isSleepStart);
		void CommandCIMI(char* cmd, int len, char* out = NULL);
		bool Command(char* cmd, int len, char* out = NULL, int count = 5,BOOL isWating = FALSE);
		bool Command_1(char* cmd, int len);
		int	 Transaction(char* req, int reqLen, char* res, int resLen, int nCount = 1,bool bwait = false,char *condition = "OK");
		void Transaction_(char* req, int reqLen = 0 ,char const *condition = "" ,OKStatus flag = okNoHandle,bool head = false );

		void SetSMSStatus(SMSSendStatus status);
		SMSSendStatus GetSMSStatus(SMSSendStatus status);
		
		int  Init(BOOL isSleepStart);
		int  On(char* pin, BOOL isSleepStart);
		void Off(void);

		void AdjustAudio();
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
		int SmsSend(int len, char* pdu, int pduLength, char* ans, int ansLength);
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
		void UnconditionalTransferStatus();//������ת��
		
		void PhoneSetCallLimit(PhoneCallLimit calllimit); //���ú�������
		void PhoneGetCallLimit(PhoneCallLimit &calllimit); //��ȡ������������
		void CallLimit(const char *fac,bool able);//���ƺ�������ȹ��� fac, "AI"���ƺ���,"AO"���ƺ����������Ĳ���LC6311 ,AT+CLCK.

		bool PhoneSetCallWaiting(bool able);//���ú��еȴ�
		void CHLDStatus();//��ȡ���еȴ�����
		void CHLD(int n);//�����л�

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
		void ParseData(char const *src,std::vector<SIM_FORMAT> &vsim);

		void Save2Sim(std::vector<SIM_FORMAT> vsim,bool bsave = true, BOOL isFDN = FALSE);
		void DeleteSim(int id);//ɾ��SIM���е���Ƭ
		void GB2312toUSC2(const char *src,char *des,int srclength);
		void USC2toGB2312(char *src,char *des,int length);
		int  gsmBytes2String(const unsigned char* pSrc, char* pDst, int nSrcLength); 
		int  gsmString2Bytes(const char* pSrc, unsigned char* pDst, int nSrcLength); 
		int  EncodeGsm(const char *src,unsigned char *des,int srclength );
		int  DecodeGsm(const unsigned char *src,char *des,int srclength);
		int  GetSimCapacity();//���SIM��������
		void DecodeRegisterSMS(char *pDst, int &dstLen);  //��ע����ű���

		BOOL CheckPin();
		void InputPin(const char *pin);
		void InputPuk(const char *puk,const char *newpin);//
		bool InputPin2(const char *pin2);
		bool InputPuk2(const char *puk2,const char *newpin);
	
		bool LockSim( const char *password);//��SIM������
		bool UnlockSim(const char *password);//��SIM����
		bool GetSimStatus(int &status);//���SIM��״̬
		bool ChangePin(const char *pin,const char *newpin);//��SIM������
		bool ChangePin2(const char *pin,const char *newpin);//
		void HandlePin();
		void HandlePuk();
		void HandlePuk2();
		
		//����ת��
		void UnconditionalTransfer( bool able);//
		void BusyTransfer(bool able);//
		void NoReplyTransfer(bool able);//
		void NoReachTranfer(bool able);//
		void GetOperator();//��õ�ǰ������Ӫ��
		bool CommandFun5(char* cmd, int len, char* out = NULL, int count = 5);
		bool CommandFun5_(char* cmd, int len,char* out = NULL);
		
		void HandleSIM(bool binsert,bool bfresh = false);
		void Command_2(char *cmd,int len,char *out,int outlen ,char *stop);
		bool CheckCSCS(char *code);//�����뼯
		void GetPinErrorCount(int &nPin1, int &nPin2, int &nPuk1, int &nPuk2);

		//net
		void ReportNet(bool bover);//������������
		void CMER();
		
		//�Լ������Ҷ�
		void DHUP(int const cid);//�Ҷ�ĳһ·
		void DUSIMR();//��ѯ����״̬

		//Parse
		virtual void Update(std::string const& data);
		void ParseDialUp(std::string const& data);
		void ParseCLCK(std::string const& data);
		void ParseCPBS(std::string const& data);
		void ParseCPBR(std::string const& data);
		void ParseCCFC(std::string const& data);
		void ParseCCWA(std::string const& data);
		void ParseDUSIMR(std::string const& data);//���SIM����״̬
		void ParseError(std::string const& data);
		void ParseReceipt(std::string const& data);
		void ParseQUSIM(std::string const& data);
		void ParseOK(std::string const& data);//����Ok
		void ParseBR(std::string const& data);//�Լ����ŵĴ���
		void ParseCMGL(std::string const& data);//����SIM�������Ϣ
		void ParseCPIN(std::string const& data);//���PIN���״̬
		void ParseCRSM(std::string const&data);//����smsp����
		void ParseCSCA(std::string const&data);
		void ParseDTUSATURC(std::string const&data);//����STK����
		void ParseCOPS(std::string const&data);//����
		void ParseCREG(std::string const&data);//����CREG
		void ParseCOPL(std::string const&data);//
		void ParseDSACT(std::string const&data);//
		void ParseDRAP(std::string const&data);
		void ParseCPMS(std::string const&data);//����CPMS

		void ParseTest(std::string const&data);//^DCALLPI
		void SetOkStatus(OKStatus oks);
		OKStatus GetOkStatus();//
		
		//sms
		void SmsWrite2Sim(int smsLength,int sta);
		void SmsWrite2Sim(char const *smsPdu,int pduLength);
		void SmsReadFromSim(int sta);//��SIM��������Ϣ
		void SmsReadFromSim_(int sta);//
		void SmsDeleteFromSim(int index , int delflag = 0 );//ɾ��SIM�������Ϣ
		
		//Pin
		void PinStatus();//���PIN�������ǹر�
		void CheckPuk();//�Ƿ���Ҫ����PUK
		void AppInputPin(const char *pin);//����PIN��
		void AppInputPin2(const char *pin2);//����PIN2��
		void AppInputPuk(const char *puk,const char *newpin);
		void AppChangePin(const char *pin,const char *newpin);
		void AppChangePin2(const char *pin,const char *newpin);
		void AppInputPuk2(const char *puk2,const char *newpin);//û�п�������PUK2��ĺ���
		void AppLockSim(const char *password);//����SIM����pin��
		void AppUnlockSim(const char *password);//����SIM��
		
		//add by qi 20100910
		enum DrapStatus
		{
			DrapNormal,
			DrapChangePinPassword,
			DrapChangePin2Password
		};
		DrapStatus m_DrapStatus;
		void Drap(DrapStatus ds = DrapNormal);//����PINʣ�����

		//vp
		void GetVP();//�����Ч��
		void SetVP(int const vp);//������Ч��
		void SetVP();//�ָ���Ч��
		void ChangeCsca(char const *csca);//�޸Ĳ������ĺ���
		void GetCsca();//��ò������ĺ���

		void SimIdentification(std::string &identification);
		void RegisterNewCom();//ע�ᵽ�µĶ˿�

		//stk
		void DSATREF_(int const mode);
		void CSIM();
		void CPMS();//����Ϣ�洢
		void CSCA();//��������
		void CMMS();//���ͳ�������
		bool GetInstance();//��ʼ������
		
		void SimStatus();//Sim��״̬
		void CMGF(int f);//���ŷ��͸�ʽ
		
		//plmn
		void FindPLMN();//�ҵ�ǰ��������Ӫ��
		void SearchPLMN();//����
		void RegisterPLMN(char const *plmn,int const act);
		void ClosePLMN();//�ر�����

		//UPLMN
		void UPLMN_Delete(int const index);
		void UPLMN_Add(char *oper,int gsm_act,int gsm_compact,int urtan_act);
		void UPLMN_Change(int const index,char *oper,int gsm_act,int gsm_compact,int urtan_act);
		void UPLMN_Search();

		void CSCS(char const *code); //Hex
		
		//FD
		void CPBS(char const *storage);//ѡ��������
		void CPBS();
		void CPBR(int const index);//
		void DSACT(int const mode,char*pwd = NULL);//FD�Ĳ���
		static UINT SendMsgThread(LPVOID lpParam);	// ����Ϣ�շ��������߳� 

		//DialTone
		void SendDialTone(char *code, int isON);
		void SetOutputGainLevel(int type);//������������С

		//���Ż�ִ
		void SmsReceiptOpen(bool bopen);//��Ϣ��ִ 
		void DMBR(int n);//���ò�����
		
	private:
		enum   clcktype
		{
			sim,//sim��
			ai,//����
			ao,//����
		};

		struct  SendMsg{
			std::string data;
			OKStatus flag;
		};
			
		std::deque<SendMsg>  m_vMsg; //��Ϣ����

		int				m_nContactNum;
		int				m_nCCFCtype;
		int				m_iContact;
		PhoneCallTRans  m_calltrans;

		volatile OKStatus m_okstatus;//ok��˭��
		clcktype		m_type;		
		int				m_nDAUDO ;
		bool			m_bgsm ;
		static bool		bInit;
		Util::ComWarp* m_pCom;

/*		Util::ComWarp* m_pSecondCom;     //20090807  ����һ��ͨ��������Ƶ����ͨ��*/

		std::string m_strPhoneNumber;
		std::string m_strSmsCentreAddress;
		std::string m_strSmsPdu;//Ҫд��SIM����Pdu����
		std::string m_svp;

		std::string m_strSMSP;//������sim��smsp���ݡ�

		bool		m_bInstance;
		bool		m_bWaitCSQ;
		//@@for debug
		int m_index;

	};
}


#endif // ATCommandWarp_h
