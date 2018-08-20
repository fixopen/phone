#ifndef __LOGICAL_TELEPHONE_H__
#define __LOGICAL_TELEPHONE_H__

#include "Util/SerialPort.h"
#include   <dshow.h > 

#define MIN_SOUND -7000    //-10000
#define MAX_SOUND 0

#define TEL_VERSION_LDJ		0	
#define DIAL_CODE_MAX		512

#if (TEL_VERSION_LDJ == 1)
//define up down bs key code
#define UP_VALUE		0x24			//0xA0   first
#define DOWN_VALUE		0x25			//0xA1
#define BS_VALUE		0x27			//0xA2
//define tel code
#define HUNGOFF_VALUE	0x02		//0x83   �һ�
#define HUNGON_VALUE	0x01		//0x82	 ժ��
#define FREEHAND_VALUE	0x00		//0xDE	 ����

#define RING_VALUE		0x0C		//0x84   �����ź�
#define FSKEND_VALUE	0x8F		//0x8F	 FSK����
#define DTMFEND_VALUE	0x8E		//0x8E   DTMF����

#else
#define UP_VALUE		0xA5			//0xA0   first
#define DOWN_VALUE		0xA6			//0xA1
#define BS_VALUE		0xA2			//0xA2
//define tel code
#define DC_BATTERY		0xDA		//��ѯ��������״̬, 0xda��ع���, 0xdb��Դ����
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
#define HUNGONTORECORD_VALUE  0x8A        //�������ժ��,������
#define HUNCONTORECORD_NO_VALUE 0x88       //���Բ�������
#define FORCEHUNGOFF_VALUE  0x8B        //ǿ�ƹһ�.
#define SREADOUTPUTSTRING_DATA   0x80        //��ʼ��ȡ�汾��Ϣ
#define EREADOUTPUTSTRING_DATA	 0x8C        //�汾��Ϣ��������
#define FORCEHANGON             0x96        //�绰����ժ�� ��·æ
#define FORCEHANGOFF              0x97        //�绰��δժ�� ��·����

/*
80H	��ȡ�汾��Ϣ
81H	ժ��ʧ��(�绰�߼����Ч)	
82H	�ֱ�������ժ����Ϣ	���ժ������
83H	�ֱ�������һ���Ϣ	����һ�����
84H	����	
8AH	�������ժ�� 
8CH	�汾��Ϣ��������	
8EH	DTMF��������������	
8FH	FSK��������������	
*/

#endif

#ifdef _DEBUG

#define  outputstring_data 1

#else

#define outputstring_data  0

#endif

/*
call id
*/
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

class Phone
{
public:
	Phone();

	virtual ~Phone();

	Util::RS232* m_pRS232;

	char  m_chDialNumber[128];

	int   m_nDialNumber;

	HANDLE m_hPort;

	BOOL  m_bDial;

	bool flag;

	BOOL SetWatchDog(void);
	BOOL ResetSystem(void);
	int DetectTestStatus(unsigned char c);
	//����绰
	void DialNumber(char* telcode, int dial_tyle = 0);   //int dial_type 0 ���ᣬ 1 ժ��
	//�һ�
	void HungOff();
	//ǿ�ƹһ�
	void ForceHungOff();
	//����ժ��
	void HungOn();
	//��/�ر�����
	void OpenTelRing(BOOL Flag = TRUE);
	//zzw
	void RestorRing(int index);

	void Mute();
	void HungOnToRecord();
	BOOL isBackLight_;				//��¼�����Ƿ��Ǵ�   
	void OpenBacklight(BOOL Flag = TRUE);

	BOOL iscallLight_;
	void OpenCallLight(BOOL Flag= TRUE);

	BOOL isleveaMsgLight;
	void OpenLeveaLight(BOOL Flag= TRUE);
	void Num2Dialnum(CHAR *dst, CHAR *src, DWORD num);

	void WritePort(const unsigned char * const data, int dataLength);

	//ring tone
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

	DWORD			g_RingSound;
	DWORD			m_ringSound;
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
	#if outputstring_data
	void getOutPutStringData();
    #endif
	BOOL GetManuInfo(char *pBuffer, int length);
	void CheckForceHungOff();
};

 class SIMDevice
 {
 public:
 	CWnd *m_pOwnerCwd;
	SIMDevice(CWnd *pWnd);
	virtual ~SIMDevice();
	BYTE    m_chSendBuff[64];
	int     m_nSendLength;

//	Util::RS232* m_pRS232;	
	HANDLE m_hPort;
	BOOL m_bRecvFlag;
	BOOL m_bExit;
	void SetExitTread(){m_bExit = TRUE;}
	void SetRecvFlag(){m_bRecvFlag = TRUE;}	
	void SetOwner(CWnd *pWnd){m_pOwnerCwd = pWnd;}
	void WritePort(const unsigned char * const data, int dataLength, int recvLen);
	BOOL Reset();

	
	int mLength;				//response lenth
	BYTE m_chReadBuff[1024];		//databuff
	int  m_nReadCount;			//datalen
	int  m_nRecvLen;
//	int mIndex = 0;				//record index
//	int gCmdIndex = 0;			//cmd index
	int mRecordCount;
	int m_nBackLightStatus;
};

/*
call id
*/

//callID
//������øú���
bool DecodeCallIDPackage(BYTE *buf, CALLID_INFO *pcallid);
void DecodeFSKSimpleParam(BYTE *pd, int datalen, CALLID_INFO *pcallid);
void DecodeFSKComplexParam(BYTE *pparm, CALLID_INFO *pcallid);
bool DecodeFSKCallIDPackage(BYTE *buf, CALLID_INFO *pcallid);
bool DecodeDTMFCallIDPackage(BYTE *buf, CALLID_INFO *pcallid);

/**************************************************************************************************
����������
**************************************************************************************************/
#define DTMF_CST                0x30 // DTMF ����ͨ����ʼ
#define DTMF_CEND               0x3F // DTMF ����ͨ������
#define FSK_CST                 0x40 // FSK ����ͨ����ʼ
#define FSK_CEND                0x4F // FSK ����ͨ������

#define DTMF_CHANNEL_STRAT      DTMF_CST // DTMF ����ͨ����ʼ
#define DTMF_CHANNEL_END        DTMF_CEND // DTMF ����ͨ������
#define FSK_CHANNEL_STRAT       FSK_CST         // FSK ����ͨ����ʼ
#define FSK_CHANNEL_END         FSK_CEND // FSK ����ͨ������

#define TEL_DTMFEND             0x8E // DTMF �������
#define TEL_FSKEND              0x8F // FSK �������

#define TEL_NONE                0 // ��Ч�ַ�
#define TEL_BUSY                1 // busy info

//���� buffer ��С����
#define TEL_BUFFER_SIZE         4096 // ������ջ����С
#define TEL_VERSION_LEN         128 // ������볤��
#define TEL_NUM_LEN             30 // ������볤��

//FSK ������
#define CALLID_TYPE_NONE        0
#define CALLID_TYPE_DTMF        1
#define CALLID_TYPE_FSK_SIMPLE  0x04 // YDN069-1997 ��׼�涨
#define CALLID_TYPE_FSK_COMPLEX 0x80 // YDN069-1997 ��׼�涨

//���ϸ�ʽ�������ͣ����ڽ��룩
#define CALLID_PARAM_TIME       0x01 // YDN069-1997 ��׼�涨
#define CALLID_PARAM_NUMBER     0x02 // YDN069-1997 ��׼�涨
#define CALLID_PARAM_NONUMBER   0x04 // YDN069-1997 ��׼�涨
#define CALLID_PARAM_NAME       0x07 // YDN069-1997 ��׼�涨
#define CALLID_PARAM_NONAME     0x08 // YDN069-1997 ��׼�涨

//������ʾ����ԭ������Ӧ�ã�
#define CALLID_OK               0
#define CALLID_NONE             1
#define CALLID_FORBID           2
#define CALLID_NOTGET           3
#define CALLID_ERROR            4

//fjm
/*
const char *CALL_FORBID  = "P";
const char *CALL_NOTGET  = "O";
const char *CALL_ERROR   = "E";
*/

#define RINGSTOP                0
#define RINGSTART               1

//�汾��Ϣ�ṹ
struct PHONEVERSION
{
	unsigned int softver;
	unsigned int hardwarever;
	unsigned int year;
	unsigned int month;
	unsigned int day;
	unsigned int chipver;
};
#endif //__LOGICAL_TELEPHONE_H__