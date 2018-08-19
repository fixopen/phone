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
#define HUNGOFF_VALUE	0x83		//0x83   �һ�
#define HUNGON_VALUE	0x82		//0x82	 ժ��
#define FREEHAND_VALUE	0xDE		//0xDE	 ժ���źţ�δ���ժ���Ƿ�ɹ���
#define RING_VALUE		0x84		//0x84   �����ź�
#define FSKEND_VALUE	0x8F		//0x8F	 FSK����
#define DTMFEND_VALUE	0x8E		//0x8E   DTMF����
#define OPENRING_VALUE	0xD1		//0xD0	 ������
#define CLOSERING_VALUE	0xD0		//0xD1	 �ر�����
#define PAGEUP_VALUE	0xa5		//page up
#define PAGEDOWN_VALUE	0xa6		//page down
#define MUTE_VALUE		0xBE		//mute
#define HUNGONTORECORD_VALUE  0x8A        //�������ժ��
#define FORCEHUNGOFF_VALUE  0x8B        //ǿ�ƹһ�.


#define MAX_PHONE_SEND		128		// ���Ͷ��г��� 
#define MAX_PHONE_RECV		128		// ���ն��г��� 

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
		int m_nSendIn;		// ���Ͷ��е�����ָ�� 
		int m_nSendOut;		// ���Ͷ��е����ָ�� 

		int m_nRecvIn;		// ���ն��е�����ָ�� 
		int m_nRecvOut;		// ���ն��е����ָ�� 

		TEL_NUM m_PhoneSend[MAX_PHONE_SEND];		// ���Ͷ���Ϣ���� 
		TEL_NUM m_PhoneRecv[MAX_PHONE_RECV];		// ���ն���Ϣ���� 

		CRITICAL_SECTION m_csSend;		// �뷢����ص��ٽ�� 
		CRITICAL_SECTION m_csRecv;		// �������ص��ٽ�� 

		HANDLE m_hKillThreadEvent;		// ֪ͨ���̹߳رյ��¼� 
		HANDLE m_hThreadKilledEvent;	// ���߳�����رյ��¼� 

		void PutSendMessage(TEL_NUM* pSmParam);	// ������Ϣ���뷢�Ͷ��� 
		BOOL GetSendMessage(TEL_NUM* pSmParam);	// �ӷ��Ͷ�����ȡһ������Ϣ 
		void PutRecvMessage(TEL_NUM* pSmParam, int nCount);	// ������Ϣ������ն��� 
		BOOL GetRecvMessage(TEL_NUM* pSmParam);	// �ӽ��ն�����ȡһ������Ϣ 

		static UINT TelephoneThread(LPVOID lpParam);	// �绰�շ��������߳�

	private:
		//�ڲ�����
		bool Dial_(TEL_NUM num);
		unsigned char State_(void);
		bool Hangup_(void);
		bool Answer_(void);
		bool Redial_();
		bool Incoming_(TEL_NUM* num);
		unsigned int SignalQuality_(void);
		unsigned int PhoneNettype_(void);
		
		//�ص�
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
		//�ⲿ����
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
		
		//����绰
		void DialNumber(char* telcode, int dial_tyle = 0);   //int dial_type 0 ���ᣬ 1 ժ��
		//�һ�
		void HungOff();
		//ժ��
		void HungOn(BOOL isRing = FALSE);
		//��/�ر�����
		void OpenTelRing(BOOL Flag = TRUE);
		//����
		void Mute();
		//¼��
		void HungOnToRecord();

		//ring tone
	public:
		IGraphBuilder   *pGraph ;   
		//ָ��Filter   Graph��ָ��   
		IMediaControl   *pMediaControl; 
		IMediaPosition  *pMediaPosition;
		CRITICAL_SECTION m_ringSetion_;
		
		//��Ƶ��ؽӿ� 
		IBasicAudio		*ipBasAudio;
		IMediaEvent		*pEvent;
		//�Ƿ�ѭ������
		UINT8 nRingCount;			//0xFF  ѭ������			
		
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
		
		//ring tone ����˵����
		//����������У��������:StartRing(TCHAR *filename); 
		//��������:StopRing();
		//����������SetSoundValueRing(value); value ��Χ -7000~0  ��startring����ǰ����
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
