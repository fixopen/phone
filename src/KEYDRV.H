/*************************************************************************/
/*                                                                       */
/*                       版权所有(c) 2001汉王科技                        */
/*                                                                       */
/*************************************************************************/
  
/*************************************************************************/
/*                                                                       */
/* 文件名                                               版本             */
/*                                                                       */
/*      keydrv.h                                        PRELIMINARY      */
/*                                                      VERSION 1.0      */
/* 描述                                                 2001年8月22日    */
/*                                                                       */
/*      键盘驱动程序头文件                                               */
/*                                                                       */
/* 作者                                                                  */
/*                                                                       */
/*      余国荣，白亚伟                                                    */
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
/*         2001.9.24     3＊4键盘 带组合键                               */
/*                                                                       */
/*************************************************************************/

#ifndef _E201_KEY_DRIVER_H__ 
#define _E201_KEY_DRIVER_H__
#include "hwfcl.h"
#include "window.h"

#define N_GROOVE						//最新版上板需得定义这个宏！!!!

/* 触摸屏键盘分布 */

#define		KEY_EREA_X0		(-50)
#define		KEY_EREA_Y0		(-1)           // byw
#define		KEY_EREA_X1		(-12)
#define		KEY_EREA_Y1		164            // byw
#define		KEY_COLUMNS		1
#define		KEY_ROWS		7

/* 触摸屏键盘定义 */

/***************************************************************************
*	键盘定义:
*		高四位为行状态（共三行，一行对应一位，保留最高位），
*		低四位为列状态（四行，一行对应一位）
*	可以用各键进行组合，键值为两键相或 分先后次序！！！
*
*
*****************************************************************************/

/* 触摸屏按键 */

#define		KEY_FASTDIAL	1000
#define		KEY_SETUP		1001
#define		KEY_DOCUMENT	1002
#define		KEY_CACULATOR	1003
#define		KEY_CLOCK		1004
#define     KEY_HELP        1005
#define		KEY_CATALOG		1006

#define		KEY_0_0			(1100 +1)
#define     KEY_0_1			(KEY_0_0 +1)
#define     KEY_0_2			(KEY_0_1 +1)
#define     KEY_0_3			(KEY_0_2 +1)
#define     KEY_0_4			(KEY_0_3 +1)
#define     KEY_1_0			(KEY_0_4 +1)
#define     KEY_1_1       	(KEY_1_0 +1)
#define     KEY_1_2			(KEY_1_1 +1)
#define     KEY_1_3			(KEY_1_2 +1)
#define     KEY_1_4			(KEY_1_3 +1)

/* 硬按键 */
#define		KEY_SMS			KEY_0_0
#define		KEY_ALARM		KEY_0_1
#define		KEY_MEMO		KEY_0_2
#define		KEY_PICTURE		KEY_0_3
#define		KEY_CARD		KEY_0_4
#define		KEY_RECORD		KEY_1_0
#define		KEY_STOP		KEY_1_1
#define		KEY_PLAY		KEY_1_2
#define		KEY_RECBROWSE	KEY_1_3
#define		KEY_HISTORY		KEY_1_4

/* 组合键 */
#define		KEY_CALIBRATON	1200					//
#define		KEY_TEST		(KEY_CALIBRATON +1)		//
#define		KEY_SUPERUSER	(KEY_TEST +1)			//

/* 软键盘按键 */
#define     KEY_SHIFT        0xF004
#define     KEY_COPY         0xF005
#define     KEY_PASTE        0xF006
#define     KEY_STATUS       0xF007 


///////////////////////////////////////////////////////////
//
//	判断笔是否按在触摸屏按键上
//
//	参数:	x		笔点 x 坐标（逻辑值）
//			y		笔点 y 坐标（逻辑值）
//
//  返回:	笔点的触摸屏按键序号，第一行第一个键为0，第二个
//			键为1，一次类推，最后一行的最后一个键为(行*列-1)；
//			返回 -1 表示没有按在触摸屏按键上。
//
///////////////////////////////////////////////////////////
int	PointInTouchKey(int x, int y);


/////////////////////////////////////////////////////////////////////
// 函数名:     InitKeyDrv                                          
//                                                                  
// 描  述:     初始化,。                  
//                                                                  
// 调  用:     应用程序初始化时                                     
//                                                                  
extern void InitKeyDrv();

/***********************************************************/
//		LED 驱动
//
/************************************************************/
#define LED_POWER				0
#define LED_INUSE				1
#define LED_HANDFREE			2
//以下为VZ控制LED
#define LED_SMS					3
#define	LED_MISSEDALARM			4
#define LED_MISSEDRECORD		5
#define LED_MISSEDCALL			6


//////////////////////////////////////////////////////////////////////////
//控制接口定义函数
extern void InitControlDev(void);		//	初始化

//LED 控制
extern void SetLED(short iID);			// 点亮 LED
extern void ClearLED(short iID);		// 熄灭 LED
extern bool LEDIsSet(short iID);		// 判断 LED 状态
void EnableLEDLight();
void DisableLEDLight();

//背光控制
extern void SetBackLight(void);
extern void ClearBackLight(void);
extern bool BackLightIsSet(void);
//LCD控制
extern bool LCDIsSet(void);
//LCD vEE电源控制 
#define	SHUTDOWN_LCD	true
#define	OPENON_LCD		false
extern void ShutOffLCD(bool bLowPower);	//单独关闭LCD,bLowPower为true关闭。

//振铃控制
#define	RINGVOLUME_LOW		0
#define	RINGVOLUME_HIGH		1
bool HardwareRingIsEnabled(void);
extern void EnableHardwareRing(void);
extern void DisableHardwareRing(void);
#define NoSound() 		PWMC2&=0XFFEF;	


//int  GetRingVolume(void);
//void SetRingVolume(int iVolume);

//录音外部电路控制
//麦克控制
extern void EnableMICInput(void);		// 允许 MIC 输入
extern void DisableMICInput(void);		// 
extern bool MICInputStatus(void);	
extern void InitBusyInspect(void);		//忙音检测脚初始化

//滤波器控制
extern void EnableFilterChannel(void);	// 允许滤波器工作
extern void DisableFilterChannel(void);	//
extern bool FilterChannelStatus(void);
//电话通道控制
extern void EnableLineOutput(void);		// 允许 PWM 输出到电话线
extern void DisableLineOutput(void);	//
extern bool LineStatus(void);

//振铃录音共通通道控制。
//功放控制
extern void EnableSpeakerOut(void);		// 允许功放工作
extern void DisableSpeakerOut(void);	//
extern bool SpeakerStatus(void);		//
//vz声音输出控制
extern void EnablePWMOutput(void);		// 允许 PWM 输出
extern void DisablePWMOutput(void);		//
extern bool PWMChannelStatus(void);

//睡眠
extern void SystmeSleep(void);			//进入睡眠

extern void TestPower(void);			//外部电源断开，使用电池供电开始,进入睡眠

extern void SystemSleep(void);			//不能在中断里！！不能调试！

extern void Beep();
//电源
#define URGENT_VALUE	127*22/32		//单节低于1.1V

extern int LowBatteryVoltage();			//检测电池电压
extern bool TestLowPower();				//外电源掉电
// 背光延时关闭

#define		BACKLIGHT_KEEP_TIME		30		// 背光点亮 10 秒，自动关闭

//看门狗
extern void OpenWatchDog(void);		//打开看门狗
extern void KillWatchDog(void);		//关闭看门狗
extern int	Watch_TimeOut;

#define WATCH_TIMEOUT	9
#define		CLR_WATCHDOG()		Watch_TimeOut=0;//清楚看门狗计数器

void AutoOpenBackLight();
void AutoCloseBackLight();

class CMsgTip:	public CFrameWnd
{
protected:
	char m_strTip[80];										//标题
	int	 m_nDisplaySec;
	int  m_iCurSecond;										//当前记录录音时间
public :
	bool m_bTiped;
	void DoPen(CPoint &pt);
	bool DoTiped();
	void SetTipCation(char * strNewCaption);
	virtual void ShowWindow(UINT32 nCmdShow = SW_SHOW);
	virtual BOOL Create(char * strCaption,int nDisplaySec=4);
	virtual void OnRtc(UINT16 nRtcType);
	virtual void OnPaint(CDC &dc);
	virtual void OnDactive();
	virtual void Destroy();
	virtual void WindowProcess(UINT32 nMessage , UINT32 wParam , UINT32 lParam);
};

///////// hqf //////////////////
class CPowerOnTip : public CMsgTip
{
protected:
	UINT32   m_nWndIcon;             
public:
	virtual BOOL Create(char * strCaption, UINT32 nIconID, CRect &wndRect,
					int nDisplaySec=4);
	virtual void OnPaint(CDC &dc);
	virtual BOOL OnDestroy();
};

extern void MessageTip(char * pStrCaption);

#define MSGTIPRECT  (CRect(10,46, 156, 114))
#define TXTRECT		(CRect(22,46+8, 148, 114-8))

extern void MessageTip(char * pStrCaption);
#endif
