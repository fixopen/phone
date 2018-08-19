#ifndef TelephoneWarp_h
#define TelephoneWarp_h

#include "../Util/ATCommandWarp.h"
#include "SerialPort.h"
#include   <dshow.h > 

#define MIN_SOUND -7000    //-10000
#define MAX_SOUND 0

//key value
#define UP_VALUE		0xA5			//0xA0   first
#define DOWN_VALUE		0xA6			//0xA1
#define BS_VALUE		0xA2			//0xA2
//define tel code
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
	BYTE numberstatus; // CALLID_OK / CALLID_NONE / CALLID_FORBID / CALLID_CANNOT_GET
	char number[CALLID_NUM_LEN];  // string ending with '\0'
	BYTE namestatus; // CALLID_OK / CALLID_NONE / CALLID_FORBID / CALLID_CANNOT_GET
	char name[CALLID_NAME_LEN];  // string ending with '\0'
} CALLID_INFO;

namespace Telephone
{
	class TelephoneWarp 
	{
	//key
	public:
		Util::RS232* m_pRS232;

	 public:
		 TelephoneWarp();
		 TelephoneWarp(const TelephoneWarp& tel);

		 ~TelephoneWarp();

		 static TelephoneWarp* GetTelephoneWarp();
		 void Bind(Util::ATCommandWarp* at);
		 typedef struct
		 {
			 char NUM[16];
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
		bool Dial_(TEL_NUM num);
		unsigned char State_(void);
		bool Hangup_(void);
		bool Answer_(void);
		bool Redial_();
		bool Incoming_(TEL_NUM* num);
		unsigned int SignalQuality_(void);
		unsigned int PhoneNettype_(void);
		
		//回调
		void Incoming(char* num);
		void Ring(void);
		void Connected(void);
		void Held(void);
		void Dialing(void);
		void Alerting(void);
		void Waiting(void);
		void NoDialTone(void);
		void Busy(void);
		void NoAnswer(void);
		void NoCarrier(void);
		void Connect(void);
		void Congestion(void);
		void OppHangup(void);
		void Odb(void);
		void SignalQuality(int level);
		void PhoneNettype(int type);
		
	public:
		//外部调用
		bool Dial(char* number);
		bool Hangup(void);
		bool Answer(void);
		bool Redial(void);
		bool SubDial(char number);
		bool Volume(unsigned int level);
		bool Mute(bool isMute);
		bool AutoAnswer(unsigned int second);
		bool HandFree(bool isHandFree);
		std::string GetNumber(void);
		void PhoneDialTone(BOOL isOn, char *tone);
		
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
		
		DWORD				g_RingSound; 
		HANDLE			hPlayRingThread;
		TCHAR			gRingFilename[64];
		BOOL m_bInputtingUserID;
		BOOL m_bStartRing;
		
		char m_chDetectCode[24];
		char m_chTelVersion[24];
		BOOL DeteTelVersion(unsigned char c);
		
		static void PlayRingPrc(void);
		CWnd *m_pMsgWnd;
		void SetMsgWnd(CWnd *pWnd){m_pMsgWnd = pWnd;}
		
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
		void StopRing(bool releasesrc = true);
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
