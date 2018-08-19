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
};//用来解析串口上报Ok信息的

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


		//电话功能
		void PhoneSetTrans(PhoneCallTRans calltrans);//设置呼叫转移
		void CallTransferCancel(int type);//设置取消
		void CallTransferStart(int type, std::string const number,int second = 0);
		void CallTransferStatus(int type);//获取呼叫转移设置
		void UnconditionalTransferStatus();//无条件转移
		
		void PhoneSetCallLimit(PhoneCallLimit calllimit); //设置呼叫受限
		void PhoneGetCallLimit(PhoneCallLimit &calllimit); //获取呼叫受限设置
		void CallLimit(const char *fac,bool able);//限制呼入呼出等功能 fac, "AI"限制呼入,"AO"限制呼出。其它的参照LC6311 ,AT+CLCK.

		bool PhoneSetCallWaiting(bool able);//设置呼叫等待
		void CHLDStatus();//获取呼叫等待设置
		void CHLD(int n);//呼叫切换

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
		void ParseData(char const *src,std::vector<SIM_FORMAT> &vsim);

		void Save2Sim(std::vector<SIM_FORMAT> vsim,bool bsave = true, BOOL isFDN = FALSE);
		void DeleteSim(int id);//删除SIM卡中的名片
		void GB2312toUSC2(const char *src,char *des,int srclength);
		void USC2toGB2312(char *src,char *des,int length);
		int  gsmBytes2String(const unsigned char* pSrc, char* pDst, int nSrcLength); 
		int  gsmString2Bytes(const char* pSrc, unsigned char* pDst, int nSrcLength); 
		int  EncodeGsm(const char *src,unsigned char *des,int srclength );
		int  DecodeGsm(const unsigned char *src,char *des,int srclength);
		int  GetSimCapacity();//获得SIM卡的容量
		void DecodeRegisterSMS(char *pDst, int &dstLen);  //自注册短信编码

		BOOL CheckPin();
		void InputPin(const char *pin);
		void InputPuk(const char *puk,const char *newpin);//
		bool InputPin2(const char *pin2);
		bool InputPuk2(const char *puk2,const char *newpin);
	
		bool LockSim( const char *password);//对SIM卡加锁
		bool UnlockSim(const char *password);//对SIM解锁
		bool GetSimStatus(int &status);//获得SIM卡状态
		bool ChangePin(const char *pin,const char *newpin);//改SIM卡密码
		bool ChangePin2(const char *pin,const char *newpin);//
		void HandlePin();
		void HandlePuk();
		void HandlePuk2();
		
		//呼叫转移
		void UnconditionalTransfer( bool able);//
		void BusyTransfer(bool able);//
		void NoReplyTransfer(bool able);//
		void NoReachTranfer(bool able);//
		void GetOperator();//获得当前网络运营商
		bool CommandFun5(char* cmd, int len, char* out = NULL, int count = 5);
		bool CommandFun5_(char* cmd, int len,char* out = NULL);
		
		void HandleSIM(bool binsert,bool bfresh = false);
		void Command_2(char *cmd,int len,char *out,int outlen ,char *stop);
		bool CheckCSCS(char *code);//检查编码集
		void GetPinErrorCount(int &nPin1, int &nPin2, int &nPuk1, int &nPuk2);

		//net
		void ReportNet(bool bover);//报告收网进程
		void CMER();
		
		//自己主动挂断
		void DHUP(int const cid);//挂断某一路
		void DUSIMR();//查询卡的状态

		//Parse
		virtual void Update(std::string const& data);
		void ParseDialUp(std::string const& data);
		void ParseCLCK(std::string const& data);
		void ParseCPBS(std::string const& data);
		void ParseCPBR(std::string const& data);
		void ParseCCFC(std::string const& data);
		void ParseCCWA(std::string const& data);
		void ParseDUSIMR(std::string const& data);//检查SIM卡的状态
		void ParseError(std::string const& data);
		void ParseReceipt(std::string const& data);
		void ParseQUSIM(std::string const& data);
		void ParseOK(std::string const& data);//解析Ok
		void ParseBR(std::string const& data);//对尖括号的处理
		void ParseCMGL(std::string const& data);//解析SIM卡里的信息
		void ParseCPIN(std::string const& data);//检查PIN码的状态
		void ParseCRSM(std::string const&data);//解析smsp数据
		void ParseCSCA(std::string const&data);
		void ParseDTUSATURC(std::string const&data);//解析STK数据
		void ParseCOPS(std::string const&data);//解析
		void ParseCREG(std::string const&data);//解析CREG
		void ParseCOPL(std::string const&data);//
		void ParseDSACT(std::string const&data);//
		void ParseDRAP(std::string const&data);
		void ParseCPMS(std::string const&data);//解析CPMS

		void ParseTest(std::string const&data);//^DCALLPI
		void SetOkStatus(OKStatus oks);
		OKStatus GetOkStatus();//
		
		//sms
		void SmsWrite2Sim(int smsLength,int sta);
		void SmsWrite2Sim(char const *smsPdu,int pduLength);
		void SmsReadFromSim(int sta);//从SIM卡读短消息
		void SmsReadFromSim_(int sta);//
		void SmsDeleteFromSim(int index , int delflag = 0 );//删除SIM卡里的信息
		
		//Pin
		void PinStatus();//检查PIN开启还是关闭
		void CheckPuk();//是否需要输入PUK
		void AppInputPin(const char *pin);//输入PIN码
		void AppInputPin2(const char *pin2);//输入PIN2码
		void AppInputPuk(const char *puk,const char *newpin);
		void AppChangePin(const char *pin,const char *newpin);
		void AppChangePin2(const char *pin,const char *newpin);
		void AppInputPuk2(const char *puk2,const char *newpin);//没有看见输入PUK2码的函数
		void AppLockSim(const char *password);//设置SIM卡的pin码
		void AppUnlockSim(const char *password);//解锁SIM卡
		
		//add by qi 20100910
		enum DrapStatus
		{
			DrapNormal,
			DrapChangePinPassword,
			DrapChangePin2Password
		};
		DrapStatus m_DrapStatus;
		void Drap(DrapStatus ds = DrapNormal);//报告PIN剩余次数

		//vp
		void GetVP();//获得有效期
		void SetVP(int const vp);//设置有效期
		void SetVP();//恢复有效期
		void ChangeCsca(char const *csca);//修改彩信中心号码
		void GetCsca();//获得彩信中心号码

		void SimIdentification(std::string &identification);
		void RegisterNewCom();//注册到新的端口

		//stk
		void DSATREF_(int const mode);
		void CSIM();
		void CPMS();//短消息存储
		void CSCA();//短信中心
		void CMMS();//发送超长短信
		bool GetInstance();//初始化结束
		
		void SimStatus();//Sim卡状态
		void CMGF(int f);//短信发送格式
		
		//plmn
		void FindPLMN();//找当前的网络运营商
		void SearchPLMN();//收网
		void RegisterPLMN(char const *plmn,int const act);
		void ClosePLMN();//关闭网络

		//UPLMN
		void UPLMN_Delete(int const index);
		void UPLMN_Add(char *oper,int gsm_act,int gsm_compact,int urtan_act);
		void UPLMN_Change(int const index,char *oper,int gsm_act,int gsm_compact,int urtan_act);
		void UPLMN_Search();

		void CSCS(char const *code); //Hex
		
		//FD
		void CPBS(char const *storage);//选中贮存期
		void CPBS();
		void CPBR(int const index);//
		void DSACT(int const mode,char*pwd = NULL);//FD的操作
		static UINT SendMsgThread(LPVOID lpParam);	// 短消息收发处理子线程 

		//DialTone
		void SendDialTone(char *code, int isON);
		void SetOutputGainLevel(int type);//设置输出增益大小

		//短信回执
		void SmsReceiptOpen(bool bopen);//短息回执 
		void DMBR(int n);//设置波特率
		
	private:
		enum   clcktype
		{
			sim,//sim卡
			ai,//呼入
			ao,//呼出
		};

		struct  SendMsg{
			std::string data;
			OKStatus flag;
		};
			
		std::deque<SendMsg>  m_vMsg; //消息队列

		int				m_nContactNum;
		int				m_nCCFCtype;
		int				m_iContact;
		PhoneCallTRans  m_calltrans;

		volatile OKStatus m_okstatus;//ok是谁的
		clcktype		m_type;		
		int				m_nDAUDO ;
		bool			m_bgsm ;
		static bool		bInit;
		Util::ComWarp* m_pCom;

/*		Util::ComWarp* m_pSecondCom;     //20090807  加入一个通道用于视频可视通道*/

		std::string m_strPhoneNumber;
		std::string m_strSmsCentreAddress;
		std::string m_strSmsPdu;//要写入SIM卡的Pdu数据
		std::string m_svp;

		std::string m_strSMSP;//完整的sim卡smsp数据。

		bool		m_bInstance;
		bool		m_bWaitCSQ;
		//@@for debug
		int m_index;

	};
}


#endif // ATCommandWarp_h
