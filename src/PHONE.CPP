/*************************************************************************/
/*                                                                       */
/*                       版权所有(c) 2001汉王科技                        */
/*                                                                       */
/*************************************************************************/
  
/*************************************************************************/
/*                                                                       */
/* 文件名                                               版本             */
/*                                                                       */
/*      phone.cpp                                       PRELIMINARY      */
/*                                                      VERSION 1.0      */
/* 描述                                                 2001年11月19日   */
/*                                                                       */
/*      电话驱动程序                                                     */
/*                                                                       */
/* 作者                                                                  */
/*                                                                       */
/*      白亚伟                                                           */
/*                                                                       */
/*                                                                       */
/* 数据结构                                                              */
/*                                                                       */
/*                                                                       */
/* 函数                                                                  */
/*                                                                       */
/*                                                                       */
/* 关联                                                                  */
/*                                                                       */
/*                                                                       */
/* 历史记录                                                              */
/*                                                                       */
/*	2001/12/1, byw                                                       */
/*	修改：RingEndProcess() 函数                                          */
/*                                                                       */
/*	2001/12/07, byw                                                      */
/*	增加：IsSMSData() 函数                                               */ 
/*                                                                       */
/*  2001/12/07, byw                                                      */
/*  修改：DoRing() 函数                                                  */
/*                                                                       */
/*  2001/12/09,xgw                                                       */
/*  增加SMS处理系列函数                                                  */                                                                     
/*                                                                       */
/*  2001/12/21,byw                                                       */
/*  增加 DoSendMessageToSMSL2() 函数								     */
/*                                                                       */
/*  2001/12/26,byw                                                       */
/*  修改：PHONE_HISR_STACK 值                                            */                                                                     
/*                                                                       */
/*  2001/12/27,byw                                                       */
/*  修改：ResetPhone() 函数                                              */                                                                     
/*                                                                       */
/*  2001/12/09,xgw                                                       */
/*  增加SMS处理系列函数        
/*                                                                       */
//  2001/12/26,xgw
//  PHONE_HISR_STACK=2048
//  delay(5/10)=delay(1)                                           */                                                                     
/*************************************************************************/


#include	"vz328.h"
#include	"phonedrv.h"
#include	"datadrv.h"
#include	"sms.h"
#include    "record.h"

/*
*	[ 驱动程序发往应用的消息列表 ]
*
消息名称		含义		wparam			lparam
－－－－－－－－－－－－－－－－－－－－－－－－－－－
WM_PICKUP		手柄摘机	0				0
WM_MICOPEN		免提摘机	0				0
WM_PICKDOWN		手柄挂机	0				0
WM_MICCLOSE		免提挂机	0				0
WM_RING			振铃		来电结构指针	0
WM_RINGEND		振铃结束	0				0
WM_DIAL			拨号		拨号键值(*)		0
WM_REDIAL		重拨		0				0
WM_MUTESTART	静音开始	0				0
WM_MUTEEND		静音结束	0				0
WM_FLASH		闪断		0				0

注：
	*	拨号键码定义参见“PhoneKeyDef[]”
*/


/*--------------------------- constants and macros --------------------------*/
/*
*	振铃时间间隔判定值，单位：系统时间片
*		注意：保证此判定值满足下面条件：
*		双振铃时间间隔 < 判定值 < 国标规定间歇振铃时间间隔(1秒通，4秒断)
*/
#define	PHONE_RING_DELAY	300		// 300 * 5ms = 1.5s

#define	SEND_TIMEOUT		10000
#define	PHONE_HISR_STACK	2048



#define	RING_DELAY_TIME			1600	// 1600 * 5 = 8s

/*---------------------------- function prototypes ---------------------*/

void InitPhoneDrv(void);
BOOL GetPhoneStatus(void);
void PhoneLisrProcess(INT vector);
void PhoneHisrProcess(void);
void RingEndProcess(UNSIGNED id);
void DiscardCallIDProcess(UNSIGNED id);

void ResetPhone(void);
static	inline BYTE ReceiveByte(void);
static	BOOL SendByte(BYTE data);
static	inline BYTE GetPhoneData(void);
static	inline bool EndOfPackage(BYTE data);
static	inline void DoKey(SYSTEM_MESSAGE *p_msg, BYTE key);
static	inline void DoOthers(SYSTEM_MESSAGE *p_msg, BYTE data);
static void delay(unsigned int n);
static inline bool IsSMSData(BYTE data);

bool DecodeDTMFCallIDPackage(BYTE *buf, CALLID *pcallid);
void DecodeFSKSimpleParam(BYTE *pd, int datalen, CALLID *pcallid);
void DecodeFSKComplexParam(BYTE *pparm, CALLID *pcallid);
bool DecodeFSKCallIDPackage(BYTE *buf, CALLID *pcallid);
static bool DecodeCallIDPackage(BYTE *buf, CALLID *pcallid);

// SMS
static void DoSMSData(SYSTEM_MESSAGE *msg, BYTE data);
static bool SendSMSCallMessage(char *number);
static void SendOneByte(BYTE data);
void  SmsPickUpPhone();
// SMS

/*----------------------------- variables and objects ----------------------*/
/*
*	电话数据 buffer
*/
static	BYTE			PhoneBuf[TEL_BUFFER_SIZE];
static	BYTE			CallIDBuf[PHONE_CALLIDBUF_SEZE];
static	int				PhoneBufFPtr = 0, PhoneBufBPtr = 0, CallIDBufP = 0;
static	BYTE			PhoneVersion[128];
static	int				PhoneVerP = 0;

/*
*	电话号码 buffer
*/
//static	BYTE			PhoneNumberBuf[TEL_NUM_LEN] = {0};

/*
*	来电显示存储变量
*/
static	CALLID			CallID;

/*
*	电话状态记录变量
*/
static	BYTE			PhoneHandleStatus = 0;	// 手柄状态，0 ＝ 挂机，1 ＝ 摘机
static	BYTE			PhoneSpeakerStatus = 0;	// 免提状态，0 ＝ 挂机，1 ＝ 摘机
static	BYTE			PhoneMuteStatus = 0;	// 静音状态，0 ＝ 正常，1 ＝ 静音
static	int				PhoneRingCount = 0;		// 振铃次数计数器
static	UINT32			PhoneRingTime = 0;			// 振铃时间间隔

/*
*	高级中断
*/
static	NU_HISR			PhoneHISR;				

/*
*	振铃结束定时器
*/
static	NU_TIMER		RingEndTimer;

/*
*	抛弃无效来电数据定时器
*/
static	NU_TIMER		DiscardCallIDTimer;

//免提标记 为1表示免提时显示速拨窗口，为0表示不显示
static INT16            g_nMicFlag =1;

/*
*	来电显示编码表, 键盘定义表
*/
const char	DTMFNumber[17] = "D1234567890*#ABC";
//const char	FSKNumber[17] = "0123456789?*#???";
const char	PhoneKeyDef[17] = "?1234567890*#???";

// SMS
static UINT8		g_aReceiveBuf[512];			// 接收缓冲区
static int			g_nCurByte;					// 缓冲区当前字节数
extern UINT32		g_pL12MsgBuffer[2],			// 物理层与链路层交换参数的缓冲区
					g_pL23MsgBuffer[2];			// 消息层与链路层交换参数的缓冲区
extern UINT8		L3Buffer[ArrayLength];      // 数据缓冲区
static BOOL         g_aReceiveFskSign = true;
BOOL                g_pPhoneSmsState = false;   //true is sms state
BOOL                g_SendReceiveState = false;
BOOL                g_SameSendReceive = false;
extern int			iCurrentLetterBox;          ////当前选中的信箱
extern UINT8		g_iServerType;

// SMS

//temp, debug
static	int			tempcount = 0;

/*------------------------------------ Functions ---------------------------*/

void InitPhoneDrv()
{
	void * pointer;
	void (*old_lisr)(int);
	STATUS	status;

	status = NU_Create_Timer (&RingEndTimer, "ring end", RingEndProcess, 707, RING_DELAY_TIME, RING_DELAY_TIME, NU_DISABLE_TIMER);
	status = NU_Create_Timer (&DiscardCallIDTimer, "discard callid", DiscardCallIDProcess, 808, RING_DELAY_TIME, RING_DELAY_TIME, NU_DISABLE_TIMER);
	
	
	OUTBYTE(IVR, 64);

	OUTBYTE(PDDIR, INBYTE(PDDIR) & ~BIT6);	
	OUTBYTE(PDSEL, INBYTE(PDSEL) & ~BIT6);
	OUTBYTE(PDDATA, INBYTE(PDDATA) | BIT6);
//	OUTBYTE(PDPUEN, INBYTE(PDPUEN) | BIT6);


	OUTBYTE(PDSEL, INBYTE(PDSEL) | BIT4);
	OUTBYTE(PDDIR, INBYTE(PDDIR) | BIT4);
	OUTBYTE(PDPUEN, INBYTE(PDPUEN) | BIT4);
	OUTBYTE(PDDATA, INBYTE(PDDATA) | BIT4);
	
	NU_Register_LISR(67, PhoneLisrProcess, &old_lisr);
	pointer = SysMalloc(PHONE_HISR_STACK);
	NU_Create_HISR(&PhoneHISR, "PhoneHisr", PhoneHisrProcess, 0, pointer, PHONE_HISR_STACK);

	ResetPhone();

	OUTLONG(ISR, INLONG(ISR) & ~(0x00040000));
	OUTLONG(IMR, INLONG(IMR) & ~(0x00040000));

	g_nCurByte = 0;
}


void PhoneLisrProcess(INT vector)
{
	extern void LISR_sample(int vector_number);
	UINT32		oldimr;
	BYTE		data = 0;
	
	// IRQ3 中断
	if((INLONG(ISR) & 0x00040000)){
		
		/*
		*	屏蔽所有中断
		*/
		oldimr = INLONG(IMR);
		OUTLONG(IMR, 0xFFFFFFFF);
	
		/*
		*	把 PD6 改为IO输入
		*/
		OUTBYTE(PDSEL, INBYTE(PDSEL) | BIT6);
		OUTBYTE(PDDIR, INBYTE(PDDIR) & ~BIT6);
		//OUTBYTE(PDSEL, INBYTE(PDSEL) | BIT4);
		
		/* 
		*	接收数据
		*/
		if (!(INBYTE(PDDATA) & BIT6)){
			data = ReceiveByte();
			// temp, version data
			if (data >= VER_CHANNEL_START && data <= VER_CHANNEL_END){
				if (PhoneVerP < 128)
					PhoneVersion[PhoneVerP++] = data;
			}
			else{	
				PhoneBuf[PhoneBufFPtr] = data;
				if ((++PhoneBufFPtr &= (TEL_BUFFER_SIZE - 1)) == PhoneBufBPtr){
					// TODO: alert phone buffer overflow
					--PhoneBufFPtr &= (TEL_BUFFER_SIZE - 1);
				}
				if (EndOfPackage(data))
					NU_Activate_HISR(&PhoneHISR);
			}
		}

		/*
		*	把 PD6 改为INT
		*/
		OUTBYTE(PDSEL, INBYTE(PDSEL) & ~BIT6);

		/*
		*	恢复中断
		*/
		OUTLONG(IMR, oldimr);
	}
	
	// Pen timer interrupt
	if((INLONG(ISR) & 0x00000020))
		LISR_sample(vector);
}

void PhoneHisrProcess(void)
{
	BYTE			data;
	SYSTEM_MESSAGE	msg;
	
	memset(&msg, 0, sizeof(SYSTEM_MESSAGE));
	
	while (1){
	
		/*
		*	数据处理完毕
		*/
		if ((data = GetPhoneData()) == TEL_NONE)
			break;
		
		/*
		*	处理短消息
		*/
		if (IsSMSData(data)){
			DoSMSData(&msg, data);
			continue;
		}
			
		/*
		*	处理来电显示号码
		*/
		else if ((data >= DTMF_CHANNEL_STRAT && data <= DTMF_CHANNEL_END) || \
				 (data >= FSK_CHANNEL_STRAT && data <= FSK_CHANNEL_END)){
			
			if (CallIDBufP < PHONE_CALLIDBUF_SEZE)		// byw, 2002/3/1
			{
				CallIDBuf[CallIDBufP++] = data;
			}

			if (PhoneRingTime == 0)
				PhoneRingTime = NU_Retrieve_Clock() - PHONE_RING_DELAY;
		}
		
		/*
		*	处理按键
		*/
		else if ((data >= TEL_KEYSTART && data <= TEL_KEYEND) || data == TEL_KEYUP)
			DoKey(&msg, data);
			
		/*
		*	处理其他命令
		*/
		else 
			DoOthers(&msg, data);

		/*
		*	向系统消息队列发送消息
		*/
		if (msg.message){
			PostMessageToQueue(&msg);
			memset(&msg, 0, sizeof(SYSTEM_MESSAGE));
		}
	}
}


void RingEndProcess(UNSIGNED id)
{
	STATUS		status;
	SYSTEM_MESSAGE	msg;

	memset(&msg, 0, sizeof(SYSTEM_MESSAGE));
	msg.message = WM_RINGEND;
	PostMessageToQueue(&msg);
	status = NU_Control_Timer(&RingEndTimer, NU_DISABLE_TIMER);
	ResetPhone();

	// temp, debug
	tempcount = 0;
	// temp, debug
}

void DiscardCallIDProcess(UNSIGNED id)
{
	NU_Control_Timer(&DiscardCallIDTimer, NU_DISABLE_TIMER);
	ResetPhone();
	//CallIDBufP = 0;
	//PhoneRingTime = 0;
}

///////////////////////////////////////////////////////////////////

static void delay(unsigned int n)
{
	while(n--)
		;
}

/*
*	判断收到的数据是否短消息数据
*/
static inline bool IsSMSData(BYTE data)
{
	if ((data >= SMS_CHANNEL_START) && (data <= SMS_CHANNEL_END))
		return true;
	else if ((data == SMS_OFF) || (data == SMS_ON) || (data == SMS_CASTONE) || (data == SMS_FSKEND))
		return true;
	else
		return false;
}

/*
*	判断接收端口数据流是否到达了一个整包
*/
static inline bool EndOfPackage(BYTE data)
{
	
	// temp, bbj
	if (data >= 0x80)
		return true;
	else if((data >= DTMF_CHANNEL_STRAT) && (data <= FSK_CHANNEL_END) && (PhoneRingTime == 0))
		return true;
	else
		return false;
}

/* xgw
static inline BYTE ReceiveByte()
{
	register UINT8 data = 0;
	for (int i = 0; i < 7; i++){
		OUTBYTE(PDDATA, INBYTE(PDDATA) & ~BIT4);
		delay(10);   //烧录后: 1=9us;2=10us;5=14us;8=17us;10=20us  xgw 2001/12/26
		data <<= 1;
		data |= ((INBYTE(PDDATA) & BIT6) >> 6);
		OUTBYTE(PDDATA, INBYTE(PDDATA) | BIT4);
			//for(int j=0;j<2;j++);  // 1=5us;2=6us;5=11us;xgw 2001/12/26
		delay(5);
	}
	OUTBYTE(PDDATA, INBYTE(PDDATA) & ~BIT4);
	delay(10); 
	data <<= 1;
	data |= ((INBYTE(PDDATA) & BIT6) >> 6);
	OUTBYTE(PDDATA, INBYTE(PDDATA) | BIT4);
	return data;
}
*/

static inline BYTE ReceiveByte()
{
	register UINT8 data = 0;
	for (int i = 0; i < 8; i++){
		OUTBYTE(PDDATA, INBYTE(PDDATA) & ~BIT4);
		delay(10); 
		//OUTBYTE(PDDATA, INBYTE(PDDATA) | BIT4);
		data <<= 1;
		data |= ((INBYTE(PDDATA) & BIT6) >> 6);
		OUTBYTE(PDDATA, INBYTE(PDDATA) | BIT4);
		delay(5);  
	}
	OUTBYTE(PDDATA, INBYTE(PDDATA) | BIT4);
	return data;
}

static BOOL SendByte(BYTE data)
{
	register int i, count;
	UINT32   oldimr;

	/*
	*	屏蔽所有中断
	*/
	oldimr = INLONG(IMR);
	OUTLONG(IMR, 0xFFFFFFFF);
	
	/*
	*	把 PD6 改为IO输入
	*/
	OUTBYTE(PDSEL, INBYTE(PDSEL) | BIT6);
	OUTBYTE(PDDIR, INBYTE(PDDIR) & ~BIT6);
	OUTBYTE(PDDATA, INBYTE(PDDATA) & ~BIT4);
	//OUTBYTE(PDSEL, INBYTE(PDSEL) | BIT4);     // xgw+

	/*
	*	发送数据
	*/
	for(i = 0; i < 8; i ++){
		for(count = 0; (INBYTE(PDDATA) & BIT6) && count < SEND_TIMEOUT; count ++)
		   ;
		if(count >= SEND_TIMEOUT)
			break;
		
		OUTBYTE(PDDATA, (INBYTE(PDDATA) & ~BIT4) | (data & BIT7 ? BIT4 : 0));
		data <<= 1;
	  
		for(count=0; !(INBYTE(PDDATA) & BIT6) && count < SEND_TIMEOUT; count ++)
			;
		if(count >= SEND_TIMEOUT)
			break;
	}
	OUTBYTE(PDDATA, INBYTE(PDDATA) | BIT4);
	delay(20);

	/*
	*	把 PD6 改为INT
	*/
	OUTBYTE(PDSEL, INBYTE(PDSEL) & ~BIT6);

	/*
	*	恢复中断
	*/
	OUTLONG(IMR, oldimr);
	
	return !(i<8);
}

void ResetPhone(void)
{
	PhoneBufFPtr = 0;
	PhoneBufBPtr = 0;
	PhoneHandleStatus = 0;
	PhoneSpeakerStatus = 0;
	PhoneMuteStatus = 0;
	PhoneRingCount = 0;		
	PhoneRingTime = 0;
	//memset(&CallID, 0, sizeof(CALLID));
	CallID.type = CALLID_TYPE_NONE;
	CallID.namestatus = CALLID_NONE;
	CallID.numberstatus = CALLID_NONE;
	CallID.timestatus = CALLID_NONE;
	CallIDBufP = 0;
}

BOOL GetPhoneStatus(void)
{
	return (PhoneHandleStatus || PhoneSpeakerStatus);
}

static	inline BYTE GetPhoneData(void)
{
	BYTE	data;
	
	if (PhoneBufFPtr == PhoneBufBPtr)
		return TEL_NONE;
	data = PhoneBuf[PhoneBufBPtr++];
	PhoneBufBPtr &= (TEL_BUFFER_SIZE - 1);
	return data;
}
/*
static	void TranslateFSKNumber(BYTE *buf)
{
	while (*buf){
		*buf = FSKNumber[*buf - '0'];
		buf ++;
	}
}

static	void TranslateDTMFNumber(BYTE *buf)
{
	while (*buf){
		*buf = DTMFNumber[*buf - '0'];
		buf ++;
	}
}
*/
extern SYSTEM_OPTION g_SystemOption;
static	void DoRing(SYSTEM_MESSAGE *p_msg)
{			
	UINT32	syscount = NU_Retrieve_Clock();
	STATUS	status;
	
	status = NU_Control_Timer(&RingEndTimer, NU_DISABLE_TIMER);
	status = NU_Reset_Timer(&RingEndTimer, RingEndProcess, RING_DELAY_TIME, RING_DELAY_TIME, NU_ENABLE_TIMER);

	if (PhoneRingTime == 0)
		PhoneRingTime = syscount;
	else if ((syscount - PhoneRingTime) > PHONE_RING_DELAY)
		PhoneRingCount ++;
	PhoneRingTime = syscount;

	//if ((PhoneRingCount == 1) && (CallID.numberstatus != CALLID_OK)){
	if (PhoneRingCount == 1){
		
		// 停止抛弃来电数据定时器
		NU_Control_Timer(&DiscardCallIDTimer, NU_DISABLE_TIMER);

		if (CallID.numberstatus == CALLID_OK){
			if (!SendSMSCallMessage((char*)CallID.number)){
				p_msg->message = WM_RING;
				p_msg->wparam  = UINT32(CallID.number);
			}
		}
		else{
			p_msg->message = WM_RING;
			p_msg->wparam  = 0;
		}
	}
	else if (PhoneRingCount < 1)	// ygr, ????
	{
		extern CRecord * pRec;
		if(GetRecordStatus())// StopRec(false);
		{
			p_msg->message = WM_RECORD_END;
			p_msg->handle= (UINT32)pRec;
		}
		if(GetReplayStatus()) StopPlay(false,false);
	}
	if (PhoneRingCount == g_SystemOption.nRecordRingCount){
		//	振铃次数匹配，开始留言录音
		status = NU_Control_Timer(&RingEndTimer, NU_DISABLE_TIMER);
		p_msg->message = WM_AUTOREC_START;
	}
}

static	void DoMute(SYSTEM_MESSAGE *p_msg)
{
	PhoneMuteStatus = !PhoneMuteStatus;
	p_msg->message = PhoneMuteStatus ? WM_MUTESTART : WM_MUTEEND;
}

static void DoRedial(SYSTEM_MESSAGE *p_msg)
{
	p_msg->message = WM_REDIAL;
}

static void DoFlash(SYSTEM_MESSAGE *p_msg)
{
	p_msg->message = WM_FLASH;
}

static void DoDial(SYSTEM_MESSAGE *p_msg, BYTE key)
{
	p_msg->message = WM_DIAL;
	p_msg->wparam  = UINT32(PhoneKeyDef[key - TEL_KEYSTART]);
}

static void DoHandleOn(SYSTEM_MESSAGE *p_msg)
{
	PhoneHandleStatus = 1;
	p_msg->message = WM_PICKUP;
}

static void DoHandleOff(SYSTEM_MESSAGE *p_msg)
{
	PhoneHandleStatus = 0;
	if (!GetPhoneStatus())
		ResetPhone();
	p_msg->message = WM_PICKDOWN;
}

static void DoSpeakerOn(SYSTEM_MESSAGE *p_msg)
{
	STATUS		status;
	/*
	*	停止振铃结束定时器
	*/
	status = NU_Control_Timer(&RingEndTimer, NU_DISABLE_TIMER);
	status = NU_Reset_Timer(&RingEndTimer, RingEndProcess, RING_DELAY_TIME, RING_DELAY_TIME, NU_DISABLE_TIMER);
	PhoneSpeakerStatus = 1;
	p_msg->message = WM_MICOPEN;
}

static void DoSpeakerOff(SYSTEM_MESSAGE *p_msg)
{
	PhoneSpeakerStatus = 0;
	if (!GetPhoneStatus())
		ResetPhone();
	p_msg->message = WM_MICCLOSE;
}

static inline void DoKey(SYSTEM_MESSAGE *p_msg, BYTE key)
{
	switch (key){
		case TEL_REDIAL:
			DoRedial(p_msg);	// 重拨
			break;
		case TEL_MUTE:
			DoMute(p_msg);		// 静音
			break;
		case TEL_FLASH:
			DoFlash(p_msg);		// 闪断
			break;
		case TEL_KEYUP:		
			//					// 键抬起
			break;
		default:
			DoDial(p_msg, key);		// 拨号
			break;
	}
}

static inline void DoOthers(SYSTEM_MESSAGE *p_msg, BYTE data)
{
	switch (data){
		case TEL_RING:			// 振铃
			// temp, debug
			{
			char	tempbuf[10];
			CDC		dc;
			sprintf(tempbuf, "%02d", ++tempcount);
//			dc.TextOut(0, 0, tempbuf); 
			// temp, debug
			DoRing(p_msg);
			break;
			}
		case TEL_DTMFEND:		// DTMF 来电结束
		case TEL_FSKEND:		// FSK 来电结束
			CallIDBuf[CallIDBufP++] = data;
			
			/* 
			*	internal debug interface
			*/
			extern void _save_callid_data(BYTE *pdata, BYTE type);
			_save_callid_data(CallIDBuf, data);

			/*
			*	激活抛弃来电数据定时器
			*/
			NU_Control_Timer(&DiscardCallIDTimer, NU_DISABLE_TIMER);
			NU_Reset_Timer(&DiscardCallIDTimer, DiscardCallIDProcess, RING_DELAY_TIME, RING_DELAY_TIME, NU_ENABLE_TIMER);

			CallID.numberstatus = CALLID_NONE;		// byw, 2002/3/6
			DecodeCallIDPackage(CallIDBuf, &CallID);
			CallIDBufP = 0;
			//if (CallID.numberstatus == CALLID_OK)
			if (CallID.numberstatus == CALLID_OK && PhoneRingCount > 1){
				if (!SendSMSCallMessage((char*)CallID.number)){
					p_msg->message = WM_RING;
					p_msg->wparam  = UINT32(CallID.number);
				}
			}
			break;
/* 和免提摘机重复
		case TEL_HANDLEON:		// 手柄摘机
			DoHandleOn(p_msg);
			break;
		case TEL_HANDLEOFF:		// 手柄挂机
			DoHandleOff(p_msg);
			break;
*/
		case TEL_SPEAKERON:		// 免提摘机
			//DoSMSData(p_msg, data);			// xgw, 2001/12/29
			DoSpeakerOn(p_msg);
			break;
		case TEL_SPEAKEROFF:	// 免提挂机
			//DoSMSData(p_msg, data);			// xgw, 2001/12/29
			DoSpeakerOff(p_msg);
			break;
		case TEL_SPEAKINGPICKUP:
			p_msg->message = WM_SPEAKINGPICKUP;
			break;
		case SMS_QUITSMS:
			p_msg->message = WM_QUITSMS;
			break;
	}
}

// wangfei's program ================================

void OpenPhoneMic()			// 免提开
{
	//g_bSoftOpen = TRUE;
	int count=0;
	if(GetPhoneStatus())
		return;
	SendByte(TEL_SPEAKERON);
	
	while(!GetPhoneStatus() && ++count < (SEND_TIMEOUT*5))
		CLR_WATCHDOG();
	return;
}
void ClosePhoneMic()			// 免提关
{
	int count=0;
	if(!GetPhoneStatus())
		return;
    SendByte(TEL_SPEAKEROFF);
    CLR_WATCHDOG();
	while(GetPhoneStatus() && ++count < SEND_TIMEOUT)
		CLR_WATCHDOG();
	return;
}
void ForceToFlash()			// 闪断
{
	SendByte(TEL_FLASH);
}

void DialOut(char *strDial)	// 拨号
{
	if(!strDial)
		return;

	while(*strDial)
	{
		if((*strDial > '0' && *strDial <= '9'))
			SendByte(BYTE(*strDial));
		else if(*strDial == '0')
            SendByte(0x3A);
		else if(*strDial  == '*')
	        SendByte(0x3B);
		else if(*strDial == '#')
		    SendByte(0x3C);
		else if(*strDial == ',')
			 NU_Sleep(420);
		strDial++;
		NU_Sleep(40);	// sleep 200ms
	}
}

BOOL SetCallIdMode(BOOL bMode)		// 设置来电制式，bMode: 0 = FSK, 1 = DTMF 
{
	BYTE data = bMode ? TEL_FSKEND : TEL_DTMFEND;	// TEL_FSKEND = FSk, TEL_DTMFEND = DTMF
	return SendByte(data);
}
//====================================



/*****************************************************************
*
*	来电显示解码
*
/*****************************************************************/

///////////////////////////////
//
//	来电显示包解码
//	
static bool DecodeCallIDPackage(BYTE *buf, CALLID *pcallid)
{
	bool	flag = false;

	if (*buf >= DTMF_CHANNEL_STRAT && *buf <= DTMF_CHANNEL_END)
	{
		flag = DecodeDTMFCallIDPackage(buf, pcallid);
	}
	
	else if (*buf >= FSK_CHANNEL_STRAT && *buf <= FSK_CHANNEL_END)
	{
		flag = DecodeFSKCallIDPackage(buf, pcallid);
	}

	if (flag)
	{
		// 过滤无效号码字符
		int		i, j, k;
		i = strlen((char*)(pcallid->number));

		for (j = 0, k = 0; k < i; k ++)
		{
			if ((pcallid->number[k] >= '0') && (pcallid->number[k] <= '9'))
			{
				pcallid->number[j] = pcallid->number[k];
				j ++;
			}
		}
		pcallid->number[j] = '\0';
		
		if (j == 0)
		{
			pcallid->numberstatus = CALLID_NONE;
			flag = false;
		}
	}

	else
	{
		pcallid->number[0] = '\0';
	}
	
	return flag;
}

//////////////////////////////////////////
//
//	解 DTMF 来电包
//
bool DecodeDTMFCallIDPackage(BYTE *buf, CALLID *pcallid)
{
	int		i;
	
	for (i = 0; (*(buf + i) != TEL_DTMFEND) && (i < CALLID_NUM_LEN - 1); i ++)
	{
		pcallid->number[i] = DTMFNumber[*(buf + i) - DTMF_CHANNEL_STRAT];
	}
	pcallid->number[i] = '\0';
	pcallid->numberstatus = CALLID_OK;
	pcallid->type = CALLID_TYPE_DTMF;
	return true;
}


////////////////////////////////
//
//	解 FSK 来电显示包
//
bool DecodeFSKCallIDPackage(BYTE *buf, CALLID *pcallid)
{
	int		pklen, datalen, i;
	BYTE	*pparm;		// pparm: pointer_parameter
	BYTE	*pd;		// pd:    pointer_data
	/*
	*	Assemble package
	*/
	for (pklen = 0; *(buf + pklen) != TEL_FSKEND; pklen ++){
		if (pklen % 2 == 0)
			*(buf + pklen / 2) = (*(buf + pklen) << 4) & 0xF0;
		else
			*(buf + pklen / 2) |= (*(buf + pklen) & 0x0F);
	}
	
	/*
	*	Package frame error
	*/
	//if (pklen % 2 != 0)	
	//	return false;	

	/*
	*	Locate pakage start flag error
	*/
	pklen /= 2;
	for (i = 0; i < pklen; i ++)
	{
		if ((*buf == CALLID_TYPE_FSK_SIMPLE) 
		   ||(*buf == CALLID_TYPE_FSK_COMPLEX))
			break;
		else
			buf ++;
	}
	if (i == pklen)
		return false;
	else
		pklen -= i;
	
	/*
	*	Package check sum error
	*/
	BYTE	sum = 0;
	for (i = 0; i < pklen - 1; i ++)
		sum += *(buf + i);
	//if ((BYTE)(sum + *(buf + pklen - 1)) != 0)
	//	return false;

	/*
	*	Package length error
	*/
	datalen = *(buf + 1);
	//if (datalen + 3 != pklen)
	//	return false;
	
	/*
	*	Decode simple format
	*/
	if (*buf == CALLID_TYPE_FSK_SIMPLE){
		pd = buf + 2;		
		DecodeFSKSimpleParam(pd, datalen, pcallid);
		pcallid->type = CALLID_TYPE_FSK_SIMPLE;
	}

	/*
	*	Decode complex format
	*/
	else if (*buf == CALLID_TYPE_FSK_COMPLEX){
		pparm = buf + 2;
		while (pparm < buf + 2 + datalen){
			DecodeFSKComplexParam(pparm, pcallid);		// Decode every parameter
			pparm += *(pparm + 1) + 2;
		}
		pcallid->type = CALLID_TYPE_FSK_COMPLEX;
	}

	return true;
}

////////////////////////////////
//
//	解 FSK 来电简单格式参数包
//
void DecodeFSKSimpleParam(BYTE *pd, int datalen, CALLID *pcallid)
{
	/*
	*	Data length error
	*/
	//if (datalen < 8 + 1)
	//	return;

	/*
	*	Decode time
	*/
	pcallid->month = (*(pd + 0) - '0') * 10 + (*(pd + 1) - '0');
	pcallid->day = (*(pd + 2) - '0') * 10 + (*(pd + 3) - '0');
	pcallid->hour = (*(pd + 4) - '0') * 10 + (*(pd + 5) - '0');
	pcallid->minute = (*(pd + 6) - '0') * 10 + (*(pd + 7) - '0');
	pcallid->timestatus = CALLID_OK;

	/*
	*	Decode number
	*/
	BYTE	*pnum = pd + 8;
	int		numlen = datalen - 8, i;
	if (*pnum == 'O')
		pcallid->numberstatus = CALLID_CANNOT_GET;
	else if (*pnum == 'P')
		pcallid->numberstatus = CALLID_FORBID;
	else{
		for (i = 0; (i < numlen) && (i < CALLID_NUM_LEN -1); i ++)
			pcallid->number[i] = *(pnum + i);
		pcallid->number[i] = '\0';
		pcallid->numberstatus = CALLID_OK;
	}
}

////////////////////////////////
//
//	解 FSK 来电复杂格式参数包
//
void DecodeFSKComplexParam(BYTE *pparm, CALLID *pcallid)
{
	// pparm: pointer_parameter, pd: pointer_data
	int		datalen, i;
	BYTE	*pd;

	datalen = *(pparm + 1);
	pd = pparm + 2;

	/*
	*	odd-even check error
	*	just cast off check bit temporarily
	*/
	for (i = 0; i < datalen; i ++)
		*(pd + i) &= 0x7F;

	switch (*pparm){
	
	case CALLID_PARAM_TIME:
		
		/*
		*	Data time length error
		*/
		//if (datalen != 8)
		//	return;
		
		/*
		*	Decode date time
		*/
		pcallid->month = (*(pd + 0) - '0') * 10 + (*(pd + 1) - '0');
		pcallid->day = (*(pd + 2) - '0') * 10 + (*(pd + 3) - '0');
		pcallid->hour = (*(pd + 4) - '0') * 10 + (*(pd + 5) - '0');
		pcallid->minute = (*(pd + 6) - '0') * 10 + (*(pd + 7) - '0');
		pcallid->timestatus = CALLID_OK;
		break;

	case CALLID_PARAM_NUMBER:
		/*
		*	Decode number
		*/
		for (i = 0; (i < datalen) && (i < CALLID_NUM_LEN -1); i ++)
			pcallid->number[i] = *(pd + i);
		pcallid->number[i] = '\0';
		pcallid->numberstatus = CALLID_OK;
		break;

	case CALLID_PARAM_NONUMBER:
		/*
		*	Set 'no number'
		*/
		if (*pd == 'O')
			pcallid->numberstatus = CALLID_CANNOT_GET;
		else if (*pd == 'P')
			pcallid->numberstatus = CALLID_FORBID;
		else
			pcallid->numberstatus = CALLID_FORBID;
		break;

	case CALLID_PARAM_NAME:
		/*
		*	Decode name
		*/
		for (i = 0; (i < datalen) && (i < CALLID_NAME_LEN -1); i ++)
			pcallid->name[i] = *(pd + i);
		pcallid->name[i] = '\0';
		pcallid->namestatus = CALLID_OK;
		break;

	case CALLID_PARAM_NONAME:
		/*
		*	Set 'no name'
		*/
		if (*pd == 'O')
			pcallid->namestatus = CALLID_CANNOT_GET;
		else if (*pd == 'P')
			pcallid->namestatus = CALLID_FORBID;
		else
			pcallid->namestatus = CALLID_FORBID;
		break;
	}
}

struct PHONEVERSION
{
	UINT16		softver;
	UINT16		hardwarever;
	UINT16		year;
	UINT8		month;
	UINT8		day;
	UINT8		chipver;
};

////////////////////////////////
//
//	获取 EM78806 程序版本
//
char* GetPhoneVersion()
{
	int				pklen;
	PHONEVERSION	ver;

	PhoneVerP = 0;
	SendByte(TEL_VERSION);
	
	NU_Sleep(20);  // hqf temp
	
	if (!PhoneVerP){
		PhoneVersion[0] = '\0';
		return (char*)PhoneVersion;
	}
	for (pklen = 0; pklen < PhoneVerP; pklen ++){
		if (!(pklen & 0x01))
			*(PhoneVersion + pklen / 2) = (*(PhoneVersion + pklen) << 4) & 0xF0;
		else
			*(PhoneVersion + pklen / 2) |= (*(PhoneVersion + pklen) & 0x0F);
	}
	PhoneVersion[pklen / 2] = '\0';
	memcpy(&ver, (PHONEVERSION*)PhoneVersion, sizeof(PHONEVERSION));
	sprintf((char*)PhoneVersion, "%d.%02d", ver.softver>>8, ver.softver & 0xFF);
	return (char*)PhoneVersion;
}

//-------------- hqf -----------------
void SetSoftMicFlag(BOOL bShow)
{
	g_nMicFlag = bShow;
}

BOOL GetSoftMicFlag()
{
	return g_nMicFlag;
}
//-------------- hqf -----------------

//-------------- ygr ----------------- 11-28
void MicMute()			// 假免提+麦克静音
{
	int count=0;
	if(GetPhoneStatus())
		return;
	SendByte(TEL_MICMUTE);
	while(!GetPhoneStatus() && ++count < (SEND_TIMEOUT*5))		;
	return;
}
//-------------- ygr ----------------- 11-28

/******************************************************************************/
/*                                                                            */
/*  版权所有(c)  2001汉王科技                                                 */
/*  xgw                                                                          */
/******************************************************************************/

void SoftPickUpPhone()
{
	g_SendReceiveState = true;
	SendOneByte(0x87);	//	0x87表示软件摘机
	g_pPhoneSmsState = true;
	NU_Sleep(1);        // Delay   5*1=5ms

	if ( g_SystemOption.bIsNeedSound == 0 ) 
		 SendOneByte(0x92);	 // 0x92 express close voice 
	
	NU_Sleep(250);      // Delay   5*250=1.25s

}

void  SmsPickUpPhone()
{
	SendOneByte(0x87);	//	0x87表示软件摘机
	g_pPhoneSmsState = true;
	if ( g_SystemOption.bIsNeedSound == 0 ) 
		SendOneByte(0x92);	 // 0x92 express close voice 


}

static inline void DoReceiveFsk()
{
	if( g_nCurByte >= ArrayLength )           
		g_nCurByte = ArrayLength;               
	                                  
	for(int j=0;j<g_nCurByte; j++)
	  {
			L3Buffer[j] = g_aReceiveBuf[j];
			g_aReceiveBuf[j] = 0x00;
	  }
    g_aReceiveFskSign = TRUE;
	SendMessageToSMSL2(Msg_Receive_FSK_VZ_Chain ,0 ,0); 
}

////////////////////////////////////////////////////////////////////////////////
bool SmsDialOut(char *strDial)
{
	if(!strDial)
		return false;
	SoftPickUpPhone();
	if(g_SameSendReceive)
	{
		g_SendReceiveState = false;
		g_SameSendReceive = false;
		return false;
	}
	while(*strDial)
	{
		if((*strDial > '0' && *strDial <= '9'))
			SendByte(BYTE(*strDial));
		else if(*strDial == '0')
            SendByte(0x3A);
		else if(*strDial  == '*')
	        SendByte(0x3B);
		else if(*strDial == '#')
		    SendByte(0x3C);
		else if(*strDial == ',')
			 NU_Sleep(200);   //waiting 1s
		strDial++;
	}
    NU_Sleep(200);        // 2002/3
	g_pL12MsgBuffer[0] = Msg_TS1_EM;
	g_pL12MsgBuffer[1] = 0x00;
	SendMessageToSMSL2(Msg_EM_VZ_Chain, 0, 0);
	g_SendReceiveState = false;
	return true;
}
/*

extern void SetLED(short iID);			// 点亮 LED
extern void ClearLED(short iID);		// 熄灭 LED
extern bool LEDIsSet(short iID);		// 判断 LED 状态

*/


////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
// send DTMF signal in 60ms/60ms
// and open the fskpower


void OrderSendDTMFData(UINT8 *strDTMFdata,int ByteNumber,UINT8 ServerType)
{

	int i;
	
	bool LedSign;
	LedSign=LEDIsSet(LED_SMS);

	for( i=0; i < (ByteNumber-1); i++)
	{
		SendOneByte(strDTMFdata[i]); //// Send data
		CLR_WATCHDOG();

		SetLED(LED_SMS);
		NU_Sleep(10);
		//NU_Sleep(25);    //	延时5*25=125ms;
		ClearLED(LED_SMS);
		NU_Sleep(15);
	
	}


	SendOneByte(strDTMFdata[(ByteNumber-1)]);  // send the last data start 
	NU_Sleep(12);    //	延时5*12=60ms;
	SendOneByte(0x90);   // 0x90 express openning the fskpower 
	int m_ServerType = ServerType;
//	if( m_ServerType == 1);
//	{
		g_pL12MsgBuffer[0] = Msg_TS6_EM;
		g_pL12MsgBuffer[1] = 0x00;
		SendMessageToSMSL2(Msg_EM_VZ_Chain, 0, 0);
//	}


	if( LedSign )
		SetLED( LED_SMS );
	else 
		ClearLED( LED_SMS );
	
}

///////////////////////////////////////////////////////////////////
// send fsk data ack 
void SendDTMFFskAck(UINT8 *strDTMFdata )
{
	
	SendOneByte(strDTMFdata[0]);
	NU_Sleep(22);    //	延时5*30=120ms;
	SendOneByte(strDTMFdata[1]);
	NU_Sleep(22);    //	延时5*24=120ms;

	if ( strDTMFdata[1] == 0x0a )  ////数据包错误,回0x90，打开fsk
	{
		SendOneByte( 0x90 ); 
	}

//	NU_Sleep(20);    //	延时5*20=100ms;
}

///////////////////////////////////////////////////////////////////
void SMSOpenFSK ( )
{
	SendOneByte(0x90); ////打开FSK
}

///////////////////////////////////////////////////////////////////////
static void SendOneByte(BYTE data)
{
	BOOL SendOk =TRUE ;
	int  SendError = 0x00 ;
   		while( SendOk )
		{
			SendOk = !(SendByte(data));
			SendError++;
			if(SendError > 0x02)
			{
				//SendMessageToSMSL2(Msg_Unsuccess, 0, 0);
				//DisconnectServer();
				return;
			}
		}	
}
///////////////////////////////////////////////////////////////////////
void DisconnectServer()
{
	
	g_pPhoneSmsState = false;
	BOOL SendOk = TRUE ;
	int  SendError = 0x00 ;

   	
	while( SendOk )
	{
		SendOk = !(SendByte(0x86)); //	0x86表示软件挂机

		SendError++;
		if(SendError > 0x0f)
		{
			//SendMessageToSMSL2(Msg_Unsuccess, 0, 0);
			//DisconnectServer();
			return;
		}
	}

}

static bool SendSMSCallMessage(char *number)
{
	/*
	*	无外接电源，无法接收短消息
	*/
	if (TestLowPower()){
		return false;
	}

	if(strcmp("",number) == 0)
		return false;
	if (strcmp(g_SystemOption.g_pCenterSendPhone, number) == 0){
		SendMessageToSMSL2(Msg_CID_Coming_Phone_VZ_Chain, 0, 0);
		return true;
	}
	
	int length=strlen(g_SystemOption.g_pCenterSendPhone);
	int i;
	for(i=0;i<length;i++)
	{ 
		if(g_SystemOption.g_pCenterSendPhone[i]!=number[i])
			break;
		if(i==(length-1)){
			SendMessageToSMSL2(Msg_CID_Coming_Phone_VZ_Chain, 0, 0);
			return true;
		}
	}

	if(g_SystemOption.g_pCenterSendPhone[0]!='0')
		return false;
	for(i=0;i<(length-1);i++)
	{ 
		if(g_SystemOption.g_pCenterSendPhone[i+1]!=number[i])
			break;
		if(i==(length-2)){
			SendMessageToSMSL2(Msg_CID_Coming_Phone_VZ_Chain, 0, 0);
			return true;
		}
	}

	return false;
}
/////////////////////////////////////////////////////
static void DoSMSData(SYSTEM_MESSAGE *msg, BYTE data)
{
	if ((data >= SMS_CHANNEL_START) && (data <= SMS_CHANNEL_END)){
		if( g_aReceiveFskSign ){      // FSK数据高4位	
			data <<= 4;
			data &= 0xF0;
			g_aReceiveBuf[g_nCurByte] = data;
			g_aReceiveFskSign = FALSE;
		}
		else{					    // FSK数据低4位	
			data &= 0x0F;
			g_aReceiveBuf[g_nCurByte] |= data;
			g_nCurByte++;
			g_aReceiveFskSign = TRUE;
		}
		return;
	}
	switch(data)
	{
		case SMS_FSKEND:            // 开始处理FSK数据包 
			 DoReceiveFsk();
			 g_nCurByte = 0;
			 break;
	
		case SMS_OFF:               // SMS挂机成功
			 g_nCurByte = 0;
			 PhoneSpeakerStatus = 0;
			 break;  

		case SMS_ON:                // SMS摘机成功
			 g_nCurByte = 0;
			 PhoneSpeakerStatus = 1;
			 break;    

		case SMS_CASTONE:           // 收到CAS信号,可能会与来电有些冲突.
			 g_nCurByte = 0;
			 if(g_SendReceiveState)
			 {
				g_SameSendReceive = true;
				SendMessageToSMSL2(Msg_Hook_Off_VZ_Chain, 0, 0);
			    break;	
			 }
			 if(!g_SendReceiveState)
			 {
				g_pL12MsgBuffer[0] = Msg_CAS_EM;
				g_pL12MsgBuffer[1] = 0x00;
				SendMessageToSMSL2(Msg_EM_VZ_Chain, 0, 0);
				break;
			 }	
/*		case TEL_SPEAKERON:			// 摘机 
			 if( g_pPhoneSmsState ) 
				{
				    SendMessageToSMSL2(Msg_Hook_Off_VZ_Chain, 0, 0);
					g_nCurByte = 0;
				}
			break;

		case TEL_SPEAKEROFF:       // 挂机
               if( g_pPhoneSmsState ) 
				{
				    SendMessageToSMSL2(Msg_Hook_Off_VZ_Chain, 0, 0);
					g_nCurByte = 0;
				}
			break;*/
		default:
			break;
	} // end of switch()
}
//  xgw 
//////////////////////////////////////////////////////////////////////
