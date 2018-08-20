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
#define HUNGOFF_VALUE	0x02		//0x83   挂机
#define HUNGON_VALUE	0x01		//0x82	 摘机
#define FREEHAND_VALUE	0x00		//0xDE	 免提

#define RING_VALUE		0x0C		//0x84   振铃信号
#define FSKEND_VALUE	0x8F		//0x8F	 FSK结束
#define DTMFEND_VALUE	0x8E		//0x8E   DTMF结束

#else
#define UP_VALUE		0xA0			//0xA0   first
#define DOWN_VALUE		0xA1			//0xA1
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
#define PAGEUP_VALUE	0xA5		//page up
#define PAGEDOWN_VALUE	0xA6		//page down
#define MUTE_VALUE		0xBE		//mute
#define HUNGONTORECORD_VALUE  0x8A        //留言软件摘机

/*
80H		读取版本信息
81H	摘机失败(电话线检测无效)	
82H	手柄或免提摘机消息	软件摘机命令
83H	手柄或免提挂机消息	软件挂机命令
84H	振铃	
8AH	留言软件摘机 
8CH	版本信息结束命令	
8EH	DTMF来电号码结束命令	
8FH	FSK来电号码结束命令	
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

	BOOL writestatus_;		// 1 正常状态， 0 取消
	int writecount_;		// 需要写总的个数，
	int writecurrentcount_; // 已写的个数
	char dialcode_[DIAL_CODE_MAX];

	int GetWriteCount(){return writecount_;}
	int GetWriteCurrentCount(){return writecurrentcount_;}
	
	//拨打电话
	void DialNumber(char* telcode, int dial_tyle = 0);   //int dial_type 0 免提， 1 摘机
	//挂机
	void HungOff();
	//免提摘机
	void HungOn();
	//设置取消拨号
	void SetDialCancel(BOOL flag = TRUE){writestatus_ = !flag;}
	//打开/关闭振铃
	void OpenTelRing(BOOL Flag = TRUE);

	void Mute();
	void HungOnToRecord();
	
	int ReadPhoneData(char *pData, int num);
	void WritePhoneData();
	void Num2Dialnum(CHAR *dst, CHAR *src, DWORD num);


	//ring tone
	IGraphBuilder   *pGraph ;   
	//指向Filter   Graph的指针   
	IMediaControl   *pMediaControl; 

	//音频相关接口 
	IBasicAudio		*ipBasAudio;
	IMediaEvent		*pEvent;
	//是否循环播放
	UINT8 nRingCount;			//0xFF  循环播放			

	int				g_RingSound; 
	HANDLE			hPlayRingThread;
	TCHAR			gRingFilename[64];

	static void PlayRingPrc(void);

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
	//程序调用该函数
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
物理层命令定义
**************************************************************************************************/
#define DTMF_CST                0x30 // DTMF 来电通道开始
#define DTMF_CEND               0x3F // DTMF 来电通道结束
#define FSK_CST                 0x40 // FSK 来电通道开始
#define FSK_CEND                0x4F // FSK 来电通道结束

#define DTMF_CHANNEL_STRAT      DTMF_CST // DTMF 来电通道开始
#define DTMF_CHANNEL_END        DTMF_CEND // DTMF 来电通道结束
#define FSK_CHANNEL_STRAT       FSK_CST         // FSK 来电通道开始
#define FSK_CHANNEL_END         FSK_CEND // FSK 来电通道结束

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

//fjm
/*
const char *CALL_FORBID  = "P";
const char *CALL_NOTGET  = "O";
const char *CALL_ERROR   = "E";
*/

#define RINGSTOP                0
#define RINGSTART               1

//版本信息结构
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