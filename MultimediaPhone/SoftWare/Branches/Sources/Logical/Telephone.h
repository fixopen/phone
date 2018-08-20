#ifndef __LOGICAL_TELEPHONE_H__
#define __LOGICAL_TELEPHONE_H__

#include "../Data/ContactData.h"
#include "../Data/ContactInfo.h"

#include   <dshow.h > 
//#include   <streams.h > 
//#include <windows.h>
//#include <streams.h>
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
#define UP_VALUE		0xA0			//0xA0   first
#define DOWN_VALUE		0xA1			//0xA1
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
#define PAGEUP_VALUE	0xA5		//page up
#define PAGEDOWN_VALUE	0xA6		//page down
#define MUTE_VALUE		0xBE		//mute
#define HUNGONTORECORD_VALUE  0x8A        //�������ժ��

/*
80H		��ȡ�汾��Ϣ
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
	HANDLE hPort;

	enum CtrlKey
	{
		UP = 0x25,
		DOWN = 0x27,
		BS = 0x8,
	};

	static void ReadPhonePrc(void);
	static void WritePhonePrc(void);

	BOOL writestatus_;		// 1 ����״̬�� 0 ȡ��
	int writecount_;		// ��Ҫд�ܵĸ�����
	int writecurrentcount_; // ��д�ĸ���
	char dialcode_[DIAL_CODE_MAX];

	int GetWriteCount(){return writecount_;}
	int GetWriteCurrentCount(){return writecurrentcount_;}
	
	//����绰
	void DialNumber(char* telcode, int dial_tyle = 0);   //int dial_type 0 ���ᣬ 1 ժ��
	//�һ�
	void HungOff();
	//����ժ��
	void HungOn();
	//����ȡ������
	void SetDialCancel(BOOL flag = TRUE){writestatus_ = !flag;}
	//��/�ر�����
	void OpenTelRing(BOOL Flag = TRUE);

	void Mute();
	void HungOnToRecord();
	
	int ReadPhoneData(char *pData, int num);
	void WritePhoneData();
	void Num2Dialnum(CHAR *dst, CHAR *src, DWORD num);


	//ring tone
	IGraphBuilder   *pGraph ;   
	//ָ��Filter   Graph��ָ��   
	IMediaControl   *pMediaControl; 

	//��Ƶ��ؽӿ� 
	IBasicAudio		*ipBasAudio;
	IMediaEvent		*pEvent;
	//�Ƿ�ѭ������
	UINT8 nRingCount;			//0xFF  ѭ������			

	int				g_RingSound; 
	HANDLE			hPlayRingThread;
	TCHAR			gRingFilename[64];

	static void PlayRingPrc(void);

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
	void StopRing();
	void PauseRing();
	void ResumeRing();
	void ReduceSoundRing(int offset);
	void SetSoundRingVal(int val);
	void IncreaseSoundRing(int offset);
	void MinSoundRing();
	void SetSoundValueRing(int value){g_RingSound = value;}
	void MaxSoundRing();

	//callID
	//������øú���
	bool DecodeCallIDPackage(BYTE *buf, CALLID_INFO *pcallid);
	void DecodeFSKSimpleParam(BYTE *pd, int datalen, CALLID_INFO *pcallid);
	void DecodeFSKComplexParam(BYTE *pparm, CALLID_INFO *pcallid);
	bool DecodeFSKCallIDPackage(BYTE *buf, CALLID_INFO *pcallid);
	bool DecodeDTMFCallIDPackage(BYTE *buf, CALLID_INFO *pcallid);
};


/*
call id
*/

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