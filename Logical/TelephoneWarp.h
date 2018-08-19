#ifndef TelephoneWarp_h
#define TelephoneWarp_h

#include "../Util/ATCommandWarp.h"
#include "../UTIL_/ATCommandWarp_.h"
#include "../UTIL_/Observer.h"
#include "SerialPort.h"
#include   <dshow.h > 

#define MIN_SOUND -7000    //-10000
#define MAX_SOUND 0

//key value
#define UP_VALUE		0xA5			//0xA0   first
#define DOWN_VALUE		0xA6			//0xA1
#define BS_VALUE		0xA2			//0xA2
//define tel code
#define HAND_ERROR		0x81		//		 摘机不成功
#define HUNGOFF_VALUE	0x83		//0x83   挂机
#define HUNGON_VALUE	0x82		//0x82	 摘机
#define FREEHAND_VALUE	0xDE		//0xDE	 摘机信号（未检测摘机是否成功）
#define RING_VALUE		0x84		//0x84   振铃信号
#define FSKEND_VALUE	0x8F		//0x8F	 FSK结束
#define DTMFEND_VALUE	0x8E		//0x8E   DTMF结束
#define OPENRING_VALUE	0xD1		//0xD0	 打开振铃
#define CLOSERING_VALUE	0xD0		//0xD1	 关闭振铃
#define PAGEUP_VALUE	0xa5		//page up
#define PAGEDOWN_VALUE	0xa6		//page down
#define MUTE_VALUE		0xBE		//mute
#define HUNGONTORECORD_VALUE  0x8A        //留言软件摘机
#define FORCEHUNGOFF_VALUE  0x8B        //强制挂机.

// DT VALUE
#define DT_VK_F1			0x0a 
#define DT_VK_F2			0x0b
#define DT_VK_F3			0x0c
#define DT_VK_F4			0x0d
#define REIVER_VALUE		0x01
#define SPEED_DIAL1			0x02
#define SPEED_DIAL2			0x03
#define FLASH_OFF			0xbf
#define DT_MUTE_VALUE		0xbe
#define DT_UP_VALUE			0xa5
#define DT_DOWN_VALUE		0xa6
#define DT_RIGHT_VALUE		0xa7
#define DT_LEFT_VALUE		0xa8
#define DT_OK_VALUE			0xa9
#define DT_HUNGON_VALUE		0x90 //没有插电话线



#define MAX_PHONE_SEND		128		// 发送队列长度 
#define MAX_PHONE_RECV		128		// 接收队列长度 

#define CALLID_NUM_LEN		32
#define CALLID_NAME_LEN		32
typedef struct
{
	BYTE type;  // CALLID_TYPE_SIMPLE / CALLID_TYPE_COMPLEX
	BYTE timestatus; // CALLID_OK / CALLID_NONE / CALLID_FORBID / CALLID_CANNOT_GET
	BYTE month;  // 1 - 12
	BYTE day;  // 0 - 31
	BYTE hour;  // 0 - 23
	BYTE minute;  // 0 - 59
	BYTE Line;//
	BYTE numberstatus; // CALLID_OK / CALLID_NONE / CALLID_FORBID / CALLID_CANNOT_GET
	char number[CALLID_NUM_LEN];  // string ending with '\0'
	BYTE namestatus; // CALLID_OK / CALLID_NONE / CALLID_FORBID / CALLID_CANNOT_GET
	char name[CALLID_NAME_LEN];  // string ending with '\0'
} CALLID_INFO;

enum BATTERY{BATTERY_1, BATTERY_DC,BATTERY_INIT};
typedef struct
{
	BATTERY batteryType;          //0 电池  1  直流电  
	BOOL    isCharge;			  //1 充电 
	BOOL    isBattery_DC;		  //	
	int		batteryProccess;
	int		typeChange;           //0 电池到电源  1 电源到电池
}BATTERYSTATUS;

//程序调用该函数
bool DecodeCallIDPackage(BYTE *buf, CALLID_INFO *pcallid);
void DecodeFSKSimpleParam(BYTE *pd, int datalen, CALLID_INFO *pcallid);
void DecodeFSKComplexParam(BYTE *pparm, CALLID_INFO *pcallid);
bool DecodeFSKCallIDPackage(BYTE *buf, CALLID_INFO *pcallid);
bool DecodeDTMFCallIDPackage(BYTE *buf, CALLID_INFO *pcallid);

/**************************************************************************************************
物理层命令定义
**************************************************************************************************/
#define DTMF_CST                0x30 // DTMF 来电通道开始
#define DTMF_CEND               0x3F // DTMF 来电通道结束
#define FSK_CST                 0x40 // FSK 来电通道开始
#define FSK_CEND                0x4F // FSK 来电通道结束

#define DTMF_CHANNEL_STRAT      DTMF_CST  // DTMF 来电通道开始
#define DTMF_CHANNEL_END        DTMF_CEND // DTMF 来电通道结束
#define FSK_CHANNEL_STRAT       FSK_CST   // FSK 来电通道开始
#define FSK_CHANNEL_END         FSK_CEND  // FSK 来电通道结束

#define TEL_DTMFEND             0x8E // DTMF 来电结束
#define TEL_FSKEND              0x8F // FSK 来电结束

#define TEL_NONE                0 // 无效字符
#define TEL_BUSY                1 // busy info

//数据 buffer 大小定义
#define TEL_BUFFER_SIZE         4096 // 命令接收缓存大小
#define TEL_VERSION_LEN         128 // 来电号码长度
#define TEL_NUM_LEN             30 // 来电号码长度

//FSK 包类型
#define CALLID_TYPE_NONE        0
#define CALLID_TYPE_DTMF        1
#define CALLID_TYPE_FSK_SIMPLE  0x04 // YDN069-1997 标准规定
#define CALLID_TYPE_FSK_COMPLEX 0x80 // YDN069-1997 标准规定

//复合格式参数类型（用于解码）
#define CALLID_PARAM_TIME       0x01 // YDN069-1997 标准规定
#define CALLID_PARAM_NUMBER     0x02 // YDN069-1997 标准规定
#define CALLID_PARAM_NONUMBER   0x04 // YDN069-1997 标准规定
#define CALLID_PARAM_NAME       0x07 // YDN069-1997 标准规定
#define CALLID_PARAM_NONAME     0x08 // YDN069-1997 标准规定

//不能显示来电原因（用于应用）
#define CALLID_OK               0
#define CALLID_NONE             1
#define CALLID_FORBID           2
#define CALLID_NOTGET           3
#define CALLID_ERROR            4


namespace Telephone
{
	class TelephoneWarp:public Util::Observer 
	{
		enum HandSet{
			HandOff,
			HandOn,	
		};
		
		enum FreeSet{
			FreeOff,
			FreeOn,	
		};

	private:	
		HandSet		m_HandSet;
		FreeSet		m_FreeSet;
	public:
		bool		m_bOpen3Port;
		bool		m_bOpen1Port;

	public:
		int		Hand(void);
		void    Hand(int hand );
		int		Free(void);
		void	Free(int free );
		void    ParseATD(std::string const& data);
		void	ParseCLCC(std::string const& data);
		void	ParseCLIP(std::string const& data);
		void    ParseCCWA(std::string const& data);//
		void    ParseDACTI(std::string const& data);
		void    ParseCSQ(std::string const& data);
		void	ParseCIEV(std::string const& data);
		void    ParseDSCI(std::string const& data);


		BATTERYSTATUS  m_BatteryStatus;
		Util::RS232* m_pRS232;

		Util::RS232* m_pRS232_;

	 public:
		 TelephoneWarp();
		 TelephoneWarp(const TelephoneWarp& tel);

		 ~TelephoneWarp();

		 static TelephoneWarp* GetTelephoneWarp();
		 void Bind(Util::ATCommandWarp* at);
		 void Bind_(Util::ATCommandWarp* at);
		 
		 virtual void Update(std::string const& data);

		 typedef struct
		 {
			 char NUM[64];
			 int TYPE;
		 }TEL_NUM;

	private:
		Util::ATCommandWarp* m_pAT;
	private:
		int m_nSendIn;		// 发送队列的输入指针 
		int m_nSendOut;		// 发送队列的输出指针 

		int m_nRecvIn;		// 接收队列的输入指针 
		int m_nRecvOut;		// 接收队列的输出指针 

		TEL_NUM m_PhoneSend[MAX_PHONE_SEND];		// 发送短消息队列 
		TEL_NUM m_PhoneRecv[MAX_PHONE_RECV];		// 接收短消息队列 

		CRITICAL_SECTION m_csSend;		// 与发送相关的临界段 
		CRITICAL_SECTION m_csRecv;		// 与接收相关的临界段 

		HANDLE m_hKillThreadEvent;		// 通知子线程关闭的事件 
		HANDLE m_hThreadKilledEvent;	// 子线程宣告关闭的事件 
		
		void PutSendMessage(TEL_NUM* pSmParam);	// 将短消息放入发送队列 
		BOOL GetSendMessage(TEL_NUM* pSmParam);	// 从发送队列中取一条短消息 
		void PutRecvMessage(TEL_NUM* pSmParam, int nCount);	// 将短消息放入接收队列 
		BOOL GetRecvMessage(TEL_NUM* pSmParam);	// 从接收队列中取一条短消息 

		static UINT TelephoneThread(LPVOID lpParam);	// 电话收发处理子线程
		
	private:
		//内部调用
		void Dial_(TEL_NUM num);
		void State_(void);
		void Hangup_(void);
		void Answer_(void);
		void Redial_();
		void Incoming_(TEL_NUM* num);
		void SignalQuality_(void);
		void PhoneNettype_(void);
		
		//回调
		void Incoming(char* num,int const line = 1);

		void Connected(int const line);
		void Held(int const line);
		void Dialing(int const line);
		void Alerting(int const line);
		void Waiting(int const line);
		void Ring(int const line);
		
		void NoDialTone(int const line);
		void Busy(int const line);
		void NoAnswer(int const line);
		void NoCarrier(int const line);
		void Connect(int const line);
		void Congestion(int const line);
		void OppHangup(int const line);
		void Odb(int const line);

		void SignalQuality(int level);
		void PhoneNettype(int type);
		
	public:
		//外部调用
		bool Dial(char* number, BOOL isVideo = FALSE);
		bool Hangup(void);
		void Answer(void);
		bool Redial(void);
		void SubDial(char number);
		void Volume(unsigned int level);
		void Mute(bool isMute);
		void AutoAnswer(unsigned int second);
		void HandFree(bool isHandFree);
		std::string GetNumber(void);
		void PhoneDialTone(BOOL isOn, char *tone);
		void SendDialTone(char *code, int isON);
			
		//拨打电话
		void DialNumber(char* telcode, int dial_tyle = 0);   //int dial_type 0 免提， 1 摘机
		//挂机
		void HungOff();
		//摘机
		void HungOn(BOOL isRing = FALSE);
		//打开/关闭振铃
		void OpenTelRing(BOOL Flag = TRUE);
		//静音
		void Mute();
		//录音
		void HungOnToRecord();
		
		bool SWitchPhone3G(BOOL is3G);
		void PSTNHangoff();
		void PSTNHangoff_();
		void PSTNHangOn();
		void PSTNRingMute(bool bmute = true);
		
		//ring tone
	public:
		IGraphBuilder   *pGraph ;   
		//指向Filter   Graph的指针   
		IMediaControl   *pMediaControl; 
		IMediaPosition  *pMediaPosition;
		CRITICAL_SECTION m_ringSetion_;
		
		//音频相关接口 
		IBasicAudio		*ipBasAudio;
		IMediaEvent		*pEvent;
		//是否循环播放
		UINT8 nRingCount;			//0xFF  循环播放			
		
		DWORD		g_RingSound; 
		HANDLE		hPlayRingThread;
		TCHAR		gRingFilename[64];
		BOOL		m_bInputtingUserID;
		int			DetectTestStatus(unsigned char c);
		BOOL		m_bStartRing;
		
		char		m_chDetectCode[64];
		char		m_chTelVersion[24];
		BOOL		DeteTelVersion(unsigned char c);
		
		static void PlayRingPrc(void);
		CWnd *m_pMsgWnd;
		void SetMsgWnd(CWnd *pWnd){m_pMsgWnd = pWnd;}

		int			m_nFirstCall;
		DWORD		m_DOldVoulme;
		
		//ring tone 调用说明：
		//在振铃界面中，来电调用:StartRing(TCHAR *filename); 
		//结束调用:StopRing();
		//设置音量：SetSoundValueRing(value); value 范围 -7000~0  在startring函数前调用
		//
		void InitRing();
		void ReleaseRing();
		void InitRingSrc();
		void RelaseRingSrc();
		void StartRing(TCHAR *filename, int ncount = 0xFF);
		void StopRing(bool releasesrc = true,bool breset = true);
		void PauseRing();
		void ResumeRing();
		void ReduceSoundRing(int offset);
		void SetSoundRingVal(int val);
		void IncreaseSoundRing(int offset);
		void MinSoundRing();
		void SetSoundValueRing(int value){g_RingSound = value;}
		void MaxSoundRing();
		void ResumeSoundValueRing();
	};

}

#endif // TelephoneWarp_h
