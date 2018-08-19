/*************************************************************************/
/*                                                                       */
/*                        版权所有(c) 2001汉王科技                        */
/*                                                                       */
/*************************************************************************/
  
/*************************************************************************/
/*                                                                       */
/* 文件名                                               版本             */
/*                                                                       */
/*      keydrv.cpp                                      PRELIMINARY      */
/*                                                      VERSION 1.0      */
/* 描述                                                 2001年8月22日    */
/*                                                                       */
/*      键盘驱动程序                                                     */
/*                                                                       */
/* 作者                                                                  */
/*                                                                       */
/*      余国荣                                                   */
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
/*     2001-9-25         支持4*3键盘                                     */
/*                                                                       */
/* ygr 2001-11-13         修改GetKeyValue函数                            */
/*                        修改ShutOffLCD函数                             */
/*                                                                       */
/*     2001-11-14         修改DisableSpeakerOut函数                      */
/*                        修改EnableSpeakerOut函数                       */
/*                                                                       */
/*     2001-12-06         修改 PointInTouchKey() 函数, byw               */
/*                                                                       */
/*     2001-12-06         modify InitKeyDrv(), byw                       */
/*                                                                       */
/*     2001-12-21         byw, add:  AutoOpenBackLight()				 */
/*							       	 AutoCloseBackLight()                */
/*     2001-12-21         byw, add:  AutoOpenBackLight()				 */
/*							       	 AutoCloseBackLight()                */
/*                                                                       */
/*                                                                       */
/*************************************************************************/
#include "ring.h"
#include "keydrv.h"
//#include "hwfcl.h"
#include "register.h"
#include "record.h"
/*
*	键盘终端高级中断 STATK 大小
*/
#define		KEY_HISR_STACK_SIZE			1024
extern	SYSTEM_OPTION g_SystemOption;

NU_HISR KeyHISR;
int	Watch_TimeOut=0;

BOOL   g_bPowerOn = FALSE;              // 是否处于开机状态
void KeyDelay(int time)
{
	while(time--);
}

static UINT32 iKeyValue=0;			//键盘扫描码
static	UINT8 iColume=0;			//发生中断的行
static UINT8 iOrgColume=0;				//键盘按键所在行，无键按下为0

static int keybufer[]={0,0,0,0,0,0,0,0,0,0};

static void SendKeyMessage(UINT32 iKey,UINT8 key_msg)
{
	static int ptr=0;
	SYSTEM_MESSAGE cm;
	cm.message =key_msg ; 
	cm.handle  = 0;
	cm.lparam = 0;
	if(key_msg==WM_KEY)
	{
//		if(ptr>9)	ptr=9;
		//第二个键不能组成组合键则返回
		if(ptr &&	iKey<KEY_CALIBRATON)
			return;
		keybufer[ptr]=iKey;
		cm.wparam = keybufer[ptr++]; //压栈
	}
	else
	{
		if(--ptr>=0) 
			cm.wparam = keybufer[ptr]; 
		ptr=0;
	}
	PostMessageToQueue(&cm); 
}

//ygr -----------------------------------------------
static UINT32 GetKeyValue(UINT32 keyvalue)
{
	switch(keyvalue)
	{
	case 0x11:	
	case 0x01:
		return KEY_0_0;
	case 0x12:	
	case 0x02:
		return KEY_0_1;    
	case 0x14:	
	case 0x04:
		return KEY_0_2;
	case 0x18:	
	case 0x08:
		return KEY_0_3;
	case 0x21:
		return KEY_0_4;
	case 0x22:
		return KEY_1_0;
	case 0x24:
		return KEY_1_1;
	case 0x28:
		return KEY_1_2;
	case 0x41:
		return KEY_1_3;
	case 0x42:
		return KEY_1_4;
	case 0x62:
		return KEY_CALIBRATON;
	case 0x64:
		return KEY_TEST;
	case 0x68:
		return KEY_SUPERUSER;
	default:
		return keyvalue;
	}
}


#define KEY_ROW	0X07
#define KEY_COL	0x0f
#define KEYSCANDELAY	( 3000)	//扫描延时0.3*3ms
#define KEYDELAY		(30000)	//延时3＊3ms

int ScanKeyBoard(void)		//返回高四位为行位置，低四位为列位置。可复用。
{	
	PDSEL	|=KEY_COL;
	PDPUL	|=KEY_COL;
	PDKBEN	&=~(KEY_COL);
	PDDIR	&=~(KEY_COL);
	KeyDelay(KEYDELAY);	
	register int row=0;
	register UINT8 OrgPDData=PDDATA;
	register UINT8 colume=OrgPDData;
	for(int i=0;i<3;i++)
	{	
		PKDATA |=(1<<i);	//逐位拉
		KeyDelay(KEYSCANDELAY);	
		if(PDDATA!=OrgPDData)
		{
			OrgPDData=PDDATA;
			row |=(1<<i);
		}
	}
	PKDATA &=~(KEY_ROW);
	KeyDelay(KEYSCANDELAY);	
	PDKBEN	|=KEY_COL;
	PDSEL	&=~(KEY_COL);
	return (row<<4)|(colume&KEY_COL);
}

//----------------------------------------------------------------
VOID g301_rtc_isr(INT vector);

static void KeyHisrEntry()			//键盘扫描与发送消息。
{
	static UINT32 keyvalue;
	register int kesSataus;
	if((~(iOrgColume))&iColume)		//KEY down?
	{
		iKeyValue=ScanKeyBoard();
		keyvalue=GetKeyValue(iKeyValue);
		kesSataus=WM_KEY;
		if(keyvalue==KEY_STOP)
		{
			if(GetReplayStatus())
//				StopPlay(fase);//modified by hqf
				StopPlay(false ,true);
			if(GetRecordStatus())
				StopRec(false);
		}
	}
	else
	{
		kesSataus=WM_KEYUP;
	}
	iOrgColume^=iColume;
	SendKeyMessage(keyvalue,kesSataus );
	PDPOL^=((UINT8)iColume);	//极性置反
	ISR		&=(0xFFFFF0FF);
	IMR		&=(0xFFFFF0FF);		//第9位，INT0 MASK位
}


void KeyLISR(INT Vector)
{
	register UINT32 ISRValue;
	ISRValue=ISR;
	if(ISRValue & 0X00000F00)   
	{
		IMR		|=0x00000F00;	//第9位，INT0 MASK位 等待高级中断完成
		iColume=(ISRValue>>8)&0x0f;
		NU_Activate_HISR(&KeyHISR);
	}
	else  if(ISRValue & 0X000000010)
	{	//RTC Interrupt;
		g301_rtc_isr(Vector);
	} 
	// temp, test uart, by bbj
	extern void UART_Lisr(int Vector);
	UART_Lisr(Vector);
}

void *pKey_Hisr=NULL;

void InitKeyDrv()
{
    VOID        (*old_lisr)(INT);   
	
	PKSEL	|=KEY_ROW;
	PKDIR	|=KEY_ROW;
	PKPUL	|=KEY_ROW;
	PKDATA	&=~(KEY_ROW);		//拉低行

	PDSEL   &=0XF0;		// byw
	PDPUL   |=0X0F;
	PDDIR   &=0xF0;		//输入引脚
	PDPOL   =0x0F;		//置1，负极性触发中断，
	PDIRQEN |=0x0F;
	PDKBEN  |=0xfF;		//中断允许
	PDIRQEG &=0xF0;		//低，电平触发
	IMR &=0xFFFFF0FF;	//第9位，INT0 MASK位
	if(!pKey_Hisr )
	{
		pKey_Hisr = SysMalloc(KEY_HISR_STACK_SIZE);
		memset(pKey_Hisr, 0xFF, KEY_HISR_STACK_SIZE);		// byw
	}
	NU_Create_HISR(&KeyHISR, "KeyHisr", KeyHisrEntry, 2, pKey_Hisr,  KEY_HISR_STACK_SIZE);
	NU_Register_LISR(68, KeyLISR, &old_lisr);
}



///////////////////////////////////////////////////////////
//
//	判断笔是否按在触摸屏按键上
//
//	参数:	x		笔点 x 坐标（逻辑值）
//			y		笔点 y 坐标（逻辑值）
//
//  返回:	键值，返回 -1 表示没有按在触摸屏按键上。
//
///////////////////////////////////////////////////////////
const int	TouchKeyMap[] = {KEY_CATALOG, KEY_HELP, KEY_CLOCK, KEY_CACULATOR, KEY_DOCUMENT, KEY_SETUP, KEY_FASTDIAL, -1, -1, -1, -1, -1, -1};
int	PointInTouchKey(int x, int y)
{
	if ((x < KEY_EREA_X0) || (x >= KEY_EREA_X1) || \
	    (y < KEY_EREA_Y0) || (y >= KEY_EREA_Y1))
		return -1;

	//int width =  (KEY_EREA_X1 - KEY_EREA_X0 +1)/KEY_COLUMNS;
	//int height = (KEY_EREA_Y1 - KEY_EREA_Y0 +1)/KEY_ROWS;
	int		row = (y - KEY_EREA_Y0) * KEY_ROWS / (KEY_EREA_Y1 - KEY_EREA_Y0 +1);
	int		column = (x - KEY_EREA_X0) * KEY_COLUMNS /(KEY_EREA_X1 - KEY_EREA_X0 +1);
	return TouchKeyMap[row * KEY_COLUMNS + column];
}


/***********************************************************
//
//		端口控制
//
//***********************************************************/
#define BAK_LIGHT_BIT	0X01	//PB0
#define LCD_VEE_BIT		0X80	//PK7 ??
#define EN_MIC_BIT		0X80	//~PE7
#define BUSY_BIT		SBIT5	//PM5 input
#define POWERTESTBIT	SBIT5	//PF5 input
#define EN_SPEAKER_BIT	0X10	//~PG4
#define EN_FILTER_BIT	0X40	//~PE6
#define EN_PWM_BIT		0X8		//~PE3
#define EN_RING_BIT		0X40	//PF6
#define RING_VOL_BIT	0X80	//PJ7

#define ENABLE_ADS()		(PBDATA &= MBIT6)
#define DISABLE_ADS()		(PBDATA |= SBIT6)

#define VOL_AD_ON		0xEF00
#define VOL_AD_OFF		0xEC00
/*
inline UINT8 SPITX(UINT16 TxData)
{
	register UINT8 tmpx=200;		
	SPICONT &=0XFE7F;			//清传送标志
	SPICONT |=0X0200;			//使能
	SPITXD16 =(UINT16)TxData;	//填数据
	SPICONT |=0X0100;			//传送
	while(SPICONT & 0x0100 && tmpx-- )
		;
	return SPIRXD;
}*/
inline UINT8 SPITX1(UINT16 TxData)
{
	register UINT8 tmpx=200;		
	while(SPICONT & 0x0100 && tmpx-- )
		;
	tmpx=SPIRXD;				//保存上次的返回值
	SPICONT &=0XFE7F;			//清传送标志
	SPICONT |=0X0200;			//使能
	SPITXD16 =TxData;			//填数据
	SPICONT |=0X0100;			//传送
	return tmpx;
}

bool g_bLowPower;

//		LED 驱动
const UINT8 BIT[7]={0,0,0,0X8,0X10,0X20,0X40};
#define LEDREG_RANGE	0X78
UINT8 g_bLEDSTatus=0xFF;
bool g_bEnableLED=true;

void DisableLEDLight()
{
	g_bEnableLED=false;
	PKDATA |=LEDREG_RANGE;
}

void EnableLEDLight()
{
	g_bEnableLED=true;
	PKDATA &=g_bLEDSTatus;
}

void SetLED(short iID)
{
	if(iID<3 || iID>6) return;
	PKSEL |=BIT[iID];
	PKDIR |=BIT[iID];
	PKPUL |=BIT[iID];
	g_bLEDSTatus &= ~BIT[iID];
	if(g_bEnableLED)
		PKDATA &=g_bLEDSTatus | ~LEDREG_RANGE;
}

void ClearLED(short iID)
{
	if(iID<3 || iID>6) return;
	PKSEL |=LEDREG_RANGE;
	PKDIR |=LEDREG_RANGE;
	PKSEL |=LEDREG_RANGE;
	g_bLEDSTatus |= BIT[iID];
	if(g_bEnableLED)
		PKDATA |=g_bLEDSTatus & LEDREG_RANGE;
}

bool LEDIsSet(short iID)
{
	if(iID<3 || iID>6) return false;
	UINT8 Bit= BIT[iID];
	bool data=PKDATA & Bit;
	bool dir =PKDIR  & Bit;
	bool sel =PKSEL  & Bit;
	if( (data)&&(dir)&&(sel) )
		return false;
	return true;
}

UINT8 LEDStatus()
{
	return g_bLEDSTatus;
}

void InitLEDDevice()
{
	PKSEL |=LEDREG_RANGE;
	PKDIR |=LEDREG_RANGE;
	PKSEL |=LEDREG_RANGE;
	PKDATA|=LEDREG_RANGE;	//高为关
}

//背光 PB0
void SetBackLight(void)
{
	PBSEL |=BAK_LIGHT_BIT ;
	PBDIR |=BAK_LIGHT_BIT ;
	PBPUL |=BAK_LIGHT_BIT ;
	if(GetRecordStatus() || TestLowPower() )
		PBDATA &=~(BAK_LIGHT_BIT );
	else
		PBDATA |=BAK_LIGHT_BIT ;
}

void ClearBackLight(void)
{
	PBSEL |=BAK_LIGHT_BIT ;
	PBDIR |=BAK_LIGHT_BIT ;
	PBPUL |=BAK_LIGHT_BIT ;
	PBDATA &=~(BAK_LIGHT_BIT );
}

bool BackLightIsSet(void)
{
	if((PBDATA & BAK_LIGHT_BIT)
	  &&(PBSEL & BAK_LIGHT_BIT)&&(PBDIR & BAK_LIGHT_BIT ) )
		return true;
	return false;
}


void ShutOffLCD(bool bLowPower)
{
		PKSEL	|=LCD_VEE_BIT;
		PKDIR	|=LCD_VEE_BIT;
		PKPUL	|=LCD_VEE_BIT;
#ifdef N_GROOVE
	if(bLowPower!=SHUTDOWN_LCD)		//低为关
#else 
	if(bLowPower==SHUTDOWN_LCD)		//高为关
#endif
	{	//LCD关闭  LCD_VEE_BIT CONTROL LCD POWER !
		PKDATA	&=~(LCD_VEE_BIT);		//拉低		
	}
	else
	{
		PKDATA	|=LCD_VEE_BIT;			//拉高
	}
}

bool LCDIsSet(void)
{
	return ( (LCKCON & 0X80) && PKSEL&LCD_VEE_BIT 
		&& PKDIR&LCD_VEE_BIT && PKDATA&LCD_VEE_BIT );
}

//振铃音量  PJ7
/*
void SetRingVolume(int iVolume)
{
	if(iVolume)
	{
		PJSEL |=RING_VOL_BIT;
		PJDIR |=RING_VOL_BIT;
		PJPUL |=RING_VOL_BIT;
		PJDATA |=RING_VOL_BIT;
	}
	else
	{
		PJSEL |=RING_VOL_BIT;
		PJDIR |=RING_VOL_BIT;
		PJPUL |=RING_VOL_BIT;
		PJDATA &=~RING_VOL_BIT;
	}

}
*/

/*
int  GetRingVolume(void)
{
	if( (PJDATA & RING_VOL_BIT) &&
		(PJSEL & RING_VOL_BIT)&&(PJDIR & RING_VOL_BIT) )
		return RINGVOLUME_HIGH;
	return RINGVOLUME_LOW;
}
*/
//振铃开关 PF6
void EnableHardwareRing(void)
{
	PFSEL |=EN_RING_BIT;
	PFDIR |=EN_RING_BIT;
	PFPUL |=EN_RING_BIT;
	PFDATA &=~(EN_RING_BIT);

	PJSEL |=RING_VOL_BIT;
	PJDIR |=RING_VOL_BIT;
	PJPUL |=RING_VOL_BIT;
	PJDATA &=~RING_VOL_BIT;
}

void DisableHardwareRing(void)
{
	PFSEL |=EN_RING_BIT;
	PFDIR |=EN_RING_BIT;
	PFPUL |=EN_RING_BIT;
	PFDATA |=EN_RING_BIT;

	PJSEL |=RING_VOL_BIT;
	PJDIR |=RING_VOL_BIT;
	PJPUL |=RING_VOL_BIT;
	PJDATA |=RING_VOL_BIT;
}
bool HardwareRingIsEnabled(void)
{
	if((!(PFDATA & EN_RING_BIT))&&
		(PFSEL & EN_RING_BIT)&&(PFDIR & EN_RING_BIT) )
		return true;
	return false;
}

extern void Sound(UINT16 Frequence);
void Beep()
{
	EnableLineOutput();
	EnablePWMOutput();
	EnableSpeakerOut();
	Sound(2000);
	NU_Sleep(50);
	NoSound();
	DisablePWMOutput();
	DisableLineOutput();
	DisableSpeakerOut();
}
/******************************************************************
/	                录音端口
/
/*****************************************************************/
#define SETIO_PE(bit)	{	PESEL |=bit;\
							PEDIR |=bit;\
							PEPUL |=bit;\
							PEDATA |=bit;}
#define CLRIO_PE(bit)	{	PESEL |=bit;\
							PEDIR |=bit;\
							PEPUL |=bit;\
							PEDATA &=~bit;}

void EnableMICInput(void)		//~PE7
{	
	CLRIO_PE(EN_MIC_BIT);
}

void DisableMICInput(void)
{	
	SETIO_PE(EN_MIC_BIT);
}

bool MICInputStatus(void)
{
	if( (!(PEDATA & EN_MIC_BIT))&&
		(PESEL & EN_MIC_BIT)&&(PEDIR & EN_MIC_BIT) )
		return true;
	return false;
}

void EnableLineOutput(void)		// 允许 PWM 输出到电话线 //ygr
{
	DisableMICInput();
}

void DisableLineOutput(void)	//
{
	EnableMICInput();
}

bool LineStatus(void)
{
	return (!MICInputStatus());
}

void EnablePWMOutput(void)		//~PE3
{
	if(TestLowPower())
	{
		SETIO_PE(EN_PWM_BIT);
	}
	else
	{
		CLRIO_PE(EN_PWM_BIT);
	}
}

void DisablePWMOutput(void)
{
	SETIO_PE(EN_PWM_BIT);
}

bool PWMChannelStatus(void)
{
	if( !(PEDATA & EN_PWM_BIT)&&(PESEL & EN_PWM_BIT)
		  &&(PEDIR & EN_PWM_BIT) && (!TestLowPower() ) )
		return true;
	return false;
}
void EnableFilterChannel(void)	//~PE6
{
	if(!TestLowPower())
	{	
		CLRIO_PE(EN_FILTER_BIT);
	}
	else
	{
		SETIO_PE(EN_FILTER_BIT);
	}
}

void DisableFilterChannel(void)
{
	SETIO_PE(EN_FILTER_BIT);
}

bool FilterChannelStatus(void)
{
	if( !(PEDATA & EN_FILTER_BIT)&&(PESEL & EN_FILTER_BIT)
		&&(PEDIR & EN_FILTER_BIT) && (TestLowPower()) )
		return true;
	return false;
}

void EnableSpeakerOut(void)		//~PG4
{
	if(TestLowPower()) return;
	PGSEL |=EN_SPEAKER_BIT	;
	PGDIR |=EN_SPEAKER_BIT	;
	PGPUL |=EN_SPEAKER_BIT	;
	PGDATA &=~(EN_SPEAKER_BIT);
	asm ( " nop " );
	PGDATA |=EN_SPEAKER_BIT	;
}

void DisableSpeakerOut(void)
{
	PGSEL |=EN_SPEAKER_BIT	;
	PGDIR |=EN_SPEAKER_BIT	;
	PGPUL |=EN_SPEAKER_BIT	;
	PGDATA &=~(EN_SPEAKER_BIT	);
}

bool SpeakerStatus(void)		//~PG4
{
	if( !(PGDATA & EN_SPEAKER_BIT)&&(PGSEL & EN_SPEAKER_BIT	)
		   &&(PGDIR & EN_SPEAKER_BIT)&&(TestLowPower())	 )
		return true;
	return false;
}

void InitBusyInspect(void)
{
	PMSEL |=BUSY_BIT;
	PMDIR &=~BUSY_BIT;
}

inline void InitPowerTest()
{
	PFSEL |=POWERTESTBIT;
	PFPUL |=POWERTESTBIT;
	PFDIR &=~POWERTESTBIT;
}

extern void InitPenDrv(bool bInitSystem);
VOID LISR1_none(int vecter)
{
}
VOID LISR2_none(int vecter)
{
}
VOID LISR5_none(int vecter)
{
}
VOID LISR7_none(int vecter)
{
}

void InitControlDev()		//	仅为初始化
{
	ILCR=( (ILCR&0XFFF0) | 3);
//	LCKCON  |=0X80;			//enable lcd control
	InitLEDDevice();
	DisableLEDLight();
//	ShutOffLCD(OPENON_LCD);
//	ClearBackLight();
//	EnableMICInput();		// 允许 MIC 输入
	DisableMICInput();		// 
//	EnablePWMOutput();		// 允许 PWM 输出
	DisablePWMOutput();		//
//	EnableFilterChannel();	// 允许滤波器工作
	DisableFilterChannel();	//
//	EnableSpeakerOut();		//允许功放工作
	DisableSpeakerOut();	// 
	InitBusyInspect();
	InitPowerTest();

    VOID        (*old_lisr)(INT);   
	NU_Register_LISR(65, LISR1_none, &old_lisr);
	NU_Register_LISR(66, LISR2_none, &old_lisr);
	NU_Register_LISR(69, LISR5_none, &old_lisr);
	NU_Register_LISR(71, LISR7_none, &old_lisr);
}

/*******************************************************************
/           睡眠
/
/*******************************************************************/

extern "C" void NoMemUseDelay();
#define RTCI_DAY        (0x0008)    //天 
#define RTCI_ALARM      (0x0004)    //定时器
#define ENABLE_IRQ5()	{	PFSEL	&=  0xFD;\
							ICR		&=0xFF7F;}

//#pragma section CODE ".sleepcode" far-absolute

void SystemSleep(void)	//不能在中断里！！不能调试！
{	
//SAVE SOME REGISTER SHOUD TO BE BEFORE SLEEP
	register UINT16 dramc,sdctrl;
	dramc=DRAMC;
	sdctrl=SDCTRL;
	int LEDStatus=0,RtcStatus,IMRStats;
	RtcStatus=RTCIENR;
	IMRStats=IMR;
	for(int i=LED_SMS;i<=LED_MISSEDCALL;i++)     
	{
		if(LEDIsSet(i))
			LEDStatus |=1<<i;
		ClearLED(i);		// 熄灭 LED
	}
//Close some special application 
	if(GetRingStatus())
		StopRing();
	if(GetRecordStatus())
		StopRec();
	if(GetReplayStatus())
		StopPlay();
//Close serial peripheral to save power
	//lower PCB
	EnableHardwareRing();
	DisableFilterChannel();//
	DisableSpeakerOut();	//
	DisablePWMOutput();
	
	//upper PCB
	ENABLE_ADS();
	SPITX1(0XD0);//power down touch pen 
	SPITX1(0X00);
	DISABLE_ADS();//disable ads7843 
	CloseRtcInterrupt(RTCI_SECOND);//关闭秒中断和看门狗。
	ClearBackLight();

	ShutOffLCD(SHUTDOWN_LCD);
	int delay=200000;
	while(delay--)
		;
	LCKCON &=~0X80;	//disable lcd cotroler

	DRAMC &=~0X0800;//CLK32
	//Open IRQ5 for touch pen wakenup
	ENABLE_IRQ5();
	//Close Interrupt avoid mistake awaken 
	asm ( " nop " );//
	IMR |=0X2;		//TIMER1
	IMR |=0X20;		//TIMER2
	RTCIENR =RTCI_DAY |RTCI_ALARM;	//Keep alarm & day RTC Interrupt.
	DRAMC |=0x0010;		//set LSP, Light Sleep
	DRAMC |=0x8000;		//Enable DRAM controler
	DRAMC |=0x4000;		//Self-refresh mode ~CKE go low
	NoMemUseDelay();	
	asm ( " nop " );	
	asm ( " nop " );	//62179550徐风
	PLLCR |=0X0008;		//DISABLE PLL(IN 30 CLOCK)
	//------------------------------------------------------------
	asm ( " nop " );	
	asm ( " nop " );	
	DRAMC &=0xBfff;	//clear RM bit
	DRAMC |=0X8000;	//Enable DRAM control
	//Restor backup register before sleep saved
	SDCTRL=sdctrl;
	DRAMC=dramc;
	RTCIENR=RtcStatus;

	LCKCON |=0X80;	//enable LCD control
	delay=200000;
	while(delay--)
		;
	ShutOffLCD(OPENON_LCD);

	OpenRtcInterrupt(RTCI_SECOND);//打开秒中断和看门狗。
	//Restore serial peripheral 
	if(g_SystemOption.nDefaultRingID==-1)
	{
		EnableHardwareRing();
	}
	else
		DisableHardwareRing();
	for(int i=LED_SMS;i<=LED_MISSEDCALL;i++)
	{
		if(	LEDStatus & (1<<i))
			SetLED(i);
	}
	SetBackLight();
	InitPenDrv(false);
	IMR=IMRStats;
	IMR &=~(0X2);
}

//#pragma section CODE


extern bool GetPowerFlag();
#define LOW_POWER	true
#define FULL_POWER	false

bool GetPowerFlag()
{
	return g_bLowPower;
}

bool TestLowPower()	//外电源断开否
{
	InitPowerTest();
	if(!(PFDATA & POWERTESTBIT))
	{
		g_bLowPower=true;
		return TRUE;
	}
	g_bLowPower=false;
	return false;
}

/////////////////////////////////
//	
//
#define SAM_NUM  12
UINT8 BatteryVoltage()
{
	UINT32 nIMRtmp=IMR;
	IMR |= 0X0FFFFFFF;
	UINT8 Val=0; 
	PESEL &=0XF8;		//PE 0:2 为SPI用
	PEPUL |=0X07;
	SPICONT= 0x420F ;	//SPI2使能，
	PBDIR |=  0x40;		//PB6 ADSchip
	PBPUL &= ~0x40;		
	ENABLE_ADS();
	SPITX1(0xEF00);
//	ClearBackLight();
	DisableSpeakerOut();
	int samval[255];
	for(int j=0;j<255;j++)
		samval[j]=0;
	for(int i=0;i<SAM_NUM;i++)
	{
		Val=SPITX1(0xEF00);
		samval[Val]++;
		for(int j=0x1fff;j;j--)
			;
	}
	int max=0;
	for(int j=1;j<255;j++)
	{
		if(samval[max]<samval[j])
			max=j;
	}
	SPITX1(0xEc00);
	DISABLE_ADS();
	InitPenDrv(false);
	IMR=nIMRtmp;
	return max;
}

#define  BATTERY_FULL	0
#define  BATTERY_LOW	(-1)
#define  NO_BATTERY		(-2)

int g_bBatteryStatus=NO_BATTERY;

int GetBatteryStatus()
{
	return g_bBatteryStatus;
}

int LowBatteryVoltage()
{
	//	采样电池电压
	UINT8 Val=BatteryVoltage();
	//	判断电压是否低于临界值
	if (Val < URGENT_VALUE)
	{	
		if(Val>=20)  //大于0.5V 小于2.4V为低电压。0.5V以下视为无电池。
			g_bBatteryStatus=BATTERY_LOW;
		else
			g_bBatteryStatus=NO_BATTERY;
	}
	else
		g_bBatteryStatus=BATTERY_FULL;
	return g_bBatteryStatus;
}

void DoPrimaryPowerOff()
{
	//电压检测
	switch(LowBatteryVoltage())
	{
	case BATTERY_LOW:
//		Beep();
		MessageTip("电池电量已不足，请尽快更换电池!");
		break;
	case NO_BATTERY:
//		Todo Test no battery
		break;
	}
	//关LED
	DisableLEDLight();
	//关留言录音
	//diable 放音，特色振铃 
	//振铃切换到电话板
	EnableHardwareRing();
	//关背光
	ClearBackLight();
	AutoOpenBackLight();
	//关下板
	DisableSpeakerOut();
	DisableFilterChannel();
	DisablePWMOutput();
}


void DoPrimaryPowerOn()
{
	//电压检测
	switch(LowBatteryVoltage())
	{
	case BATTERY_LOW:
		Beep();
		MessageTip("电池电量已不足，请尽快更换电池!");
		break;
	case NO_BATTERY:
		Beep();
		MessageTip("没有检测到电池! 掉电后录音数据无法保存!");
		break;
	}
	//开背光
	AutoOpenBackLight();
	//振铃切换回系统状态
	if(g_SystemOption.nDefaultRingID==-1)
	{
		EnableHardwareRing();
	}
	else
		DisableHardwareRing();
	//开LED
	EnableLEDLight();
}

#undef VOL_AD_ON		
#undef VOL_AD_OFF		
#undef BAK_LIGHT_BIT	
#undef LCD_VEE_BIT		
#undef EN_MIC_BIT		
#undef BUSY_BIT			
#undef POWERTESTBIT		
#undef EN_SPEAKER_BIT	
#undef EN_FILTER_BIT	
#undef EN_PWM_BIT		
#undef EN_RING_BIT		
#undef RING_VOL_BIT		

//////////////// 背光延时关闭 ///////////////////

static	short	g_BackLightCount = BACKLIGHT_KEEP_TIME;
extern int g_iEnterScrTime;
extern SYSTEM_OPTION   g_SystemOption;
void AutoOpenBackLight()
{
	if (!BackLightIsSet())
		SetBackLight();
	g_BackLightCount = BACKLIGHT_KEEP_TIME;
	g_iEnterScrTime = 0;
}

static bool PrePowerStatus=LOW_POWER;

void AutoCloseBackLight()
{
	if(TestLowPower())			//无外电源30s，系统睡眠
	{
		if(PrePowerStatus==FULL_POWER)
			DoPrimaryPowerOff();
		else
			if (g_BackLightCount-- <= 0)
			{
				SystemSleep();
				AutoOpenBackLight();
			}
	}
	else
	{
		if(PrePowerStatus==LOW_POWER)
			DoPrimaryPowerOn();
		else
		{
			if (g_BackLightCount <= 0)
				ClearBackLight();
			else
				g_BackLightCount--;
			if(g_SystemOption.PowerDownTime && g_iEnterScrTime >= g_SystemOption.PowerDownTime * 60)
			{
				extern void ScrProtectAppRun();
				ScrProtectAppRun();
				g_iEnterScrTime = 0;
			}
			else
				g_iEnterScrTime++;
		}
	}
	PrePowerStatus=GetPowerFlag();
}

void CMsgTip::SetTipCation(char * strNewCaption)
{
	strcpy(m_strTip,strNewCaption);
}

BOOL CMsgTip::Create(char * strCaption,int nDisplaySec)
{
	if(!CFrameWnd::Create(strCaption,WS_VISIBLE,MSGTIPRECT,0))
		return false;
	strcpy(m_strTip,strCaption);
	m_iCurSecond=0;
	m_nDisplaySec=nDisplaySec;
	m_bTiped=true;
	return true;
}

void CMsgTip::Destroy()
{
	CFrameWnd::Destroy();
	m_bTiped=false;
}
bool CMsgTip::DoTiped()
{
	return m_bTiped;
}

void CMsgTip::OnPaint(CDC &dc)
{
	dc.Draw3dButton(m_sRect,NULL);
	dc.SetBackColor(LGRAY);
	if(strlen(m_strTip)<16)
		dc.TextOut(m_sRect.left+8, 72,m_strTip );
	else
		dc.DrawText(TXTRECT, m_strTip);
}

void CMsgTip::ShowWindow(UINT32 nCmdShow)
{
	if(nCmdShow!=SW_SHOW)
		this->Destroy();
	else
		CFrameWnd::ShowWindow(nCmdShow);
}

void CMsgTip::OnDactive()
{
	this->Destroy();
}

void CMsgTip::OnRtc(UINT16 nRtcType)
{
	if(++m_iCurSecond>m_nDisplaySec)
		this->Destroy();
}

void CMsgTip::WindowProcess(UINT32 nMessage,UINT32 wParam,UINT32 lParam)
{
	switch(nMessage)
	{
	case WM_PENUP:
		DoPen(CPoint(HIWORD(wParam), LOWORD(wParam)));
		break;
	}
	CFrameWnd::WindowProcess(nMessage,wParam,lParam);
}

void CMsgTip::DoPen(CPoint &pt)
{
	this->Destroy();	
}

CMsgTip * g_MsgTip=NULL;

void MessageTip(char * pStrCaption)
{
	if(!g_MsgTip)
	{
		g_MsgTip=new CMsgTip;
		g_MsgTip->m_bTiped=false;
	}
	if(g_MsgTip->DoTiped())
	{
		g_MsgTip->SetTipCation(pStrCaption);
		g_MsgTip->OnPaint(CDC());
	}
	else
	{
		g_MsgTip->Create(pStrCaption);
		g_MsgTip->ShowWindow();
	}
}

BOOL CPowerOnTip::Create(char * strCaption, UINT32 nIconID, CRect &wndRect,
					int nDisplaySec)
{
	if(!CMsgTip::Create(strCaption , nDisplaySec))
		return FALSE;
	m_sRect = wndRect;
	m_nWndIcon = nIconID;
	return TRUE;
}

void CPowerOnTip::OnPaint(CDC &dc)
{
//	CFrameWnd::OnPaint(dc);
	dc.PutIcon(m_sRect.left , m_sRect.top , m_nWndIcon);
	CRect r(m_sRect);
	r.DeflateRect(1, 1);
//	if(m_strTip[0])
//		dc.DrawText(r , m_strTip);
}

BOOL CPowerOnTip::OnDestroy()
{
	g_bPowerOn = FALSE;
	return CMsgTip::OnDestroy();
}

static CPowerOnTip *g_pPowerOnTip = NULL;


void ShowPowerOnTip()
{
	if(!g_pPowerOnTip)
		g_pPowerOnTip = new CPowerOnTip;
	g_bPowerOn = TRUE;
	g_pPowerOnTip->Create(NULL, ICON_POWERON , DESK_TOP_RECT, 5);
	g_pPowerOnTip->ShowWindow();
}

void OpenWatchDog(void)
{
	WATCHDOG=1;//WatchDog
}

void KillWatchDog(void)
{
	WATCHDOG=0;//WatchDog
}