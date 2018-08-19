/*******************************************************************************************
                                                                             
    版权所有(c)  2001汉王科技                                                 
                                                                            
/**********************************************************************************************
  文 件 名                                                                  
             Record.cpp                                                   
                                                                            
  版 本 号                                                                  
             1.0                                                            
                                                                            
  描    述                                                                  
             汉王科技E-PhoneII 留言录音播放驱动程序的源文件                              
                                                                            
  组    成                                                                  
             铃声驱动程序源文件
			 Record.h
                                                                            
  作    者                                                                  
             余国荣                                                           
                                                                            
  日    期                                                                  
             2001-09-11  
			 第2                                                   
  修改记录   2001-11-23  hqf增加CRePlay::OnDestroy()函数                                                                            
  
			ygr, 2001/12/1, modify CProcessBar::MoveBar()
			ygr, 2001/12/06, modify itos()  				
*************************************************************************************************/
 
#include "record.h"
#include "messagebox.h"
#include "register.h"
#include "datetime.h"
#include "phonedrv.h"
#include "ring.h"

#define LISR_VECTOR(X)			(*(	UINT32  *) (0X100+X*4))  //设置新的中断向量

// Disable\Enable  ADC Chip Select 
#define  ADCCSEnable() {		PBDIR		|= SBIT6; \
								PBPUL		&= MBIT6; \
								PBDATA      &= MBIT6; }

// Disable  ADC Chip Select 
#define  ADCCSDisable()  {		PBDIR		|= SBIT6; \
								PBPUL		|= SBIT6; \
								PBDATA		&= MBIT6; }

#define PWMENABLE( )			(PWMC1 |=0X10)	
#define	PWMDISABLE()			(PWMC1 &=0XFFEF)			//禁能PWM1

#define DISABLE_TIMER2()		(TCTL2&=0XFFFE)			//DISABLE TIMER2
#define ENABLE_TIMER2()			(TCTL2|=0X0001)			//DISABLE TIMER2
#define DISABLEPENIRQ()			(IMR |=0X00100000)		//Disable touch_pen IRQ.
#define ENABLEPENIRQ()			(IMR &=0XFFEFFFFF)		//Enable touch_pen IRQ.

#define PWMINTLEVER				5
#define TIMERINTLEVER			2

#define ADON					(0XA003)		
#define ADOFF					(0XA000)

#define POPWNDTOP				18
#define POPWNDRECT				(CRect(3,POPWNDTOP, 156, 136))// hqf (CRect(6, POPWNDTOP, 154, 130))
								
/////////////////////////////////////////////////////////////////////////////////////////////
//
int  g_bRecStatus=0;;			//为真为当前正在录音。设为否可停止录音。
bool g_bReplayStatus;		//为真为当前正在放音。设为否可停止放音。

UINT16 g_wOrgTimerIntLever; //平时timer2的中断等级

WAVEHEAD * pWave;			//播放声音的数据头指针
int iPlayTime;				//播放声音的次数
int WaveLen;				//播放的声音数据所剩长度
UINT32 g_lOrgVector6;			//原先6级中断向量	
UINT32 g_lOrgVector2;			//原先6级中断向量	


//有关静音
#define	WAVE_MUTE		0						// 静音采样值
#define MUTEWAVE		5						//静音波动8,约为+/- 10mV
#define MUTETIMESECOND	6						//静音秒数
#define MUTETIME		MUTETIMESECOND*8000		//5*1000*8*125 us=5s
#define MUTEVALVE		0x4d					//静音默认阀值
static long	iMuteTime=MUTETIME;					//静音时长
static BYTE	g_MuteValue=MUTEVALVE;				//静音阀值
static bool bWaveAuto=false;					//静音忙音检测变量

//有关忙音
#define BUSYSAMPLEDIV		80		//80 sample  * 125/us = 10 sampple/0.1ms 每1plus 为10ms
static bool bOrgBusyStatu=0;		//前采样值，用于判断忙音信号变跳
int iPlusWide=0;					//本周期脉宽
int iPlusNum=0;						//同周期数
int iDivid=BUSYSAMPLEDIV;			//分频系数
int iPrePeriod=0;					//前周期脉宽
#define TESTPERIOD   5				//忙音周期数

CRecord * pRec = NULL;				

extern "C"	void LEVEL6_LISR(void);
extern "C"	void PWM1LISR(INT Vector);
extern "C"	void LEVEL2_LISR();
extern "C"	void LISR_ADSample(INT Vector);
extern		void InitPenDrv(bool bInitSystem);

extern "C" void HwPutString(INT16 x, INT16 y, UINT8 iBackColor, UINT8 iForColor, UINT8 iDrawMode, UINT8 *pText, INT16 iLen);
extern void ShowWindow(CFrameWnd * pwnd);
extern void HideWindow(CFrameWnd * pwnd);

#define NoSound() 		PWMC2&=0XFFEF;					//放完，则关闭中断，禁能PWM2
extern void Sound(UINT16 Frequence);

extern int g_iScreenStatus ;                    //当前屏幕状态（是否在屏保）
void DeleteLastWave()
{
	long wavelist[WAVE_MAX_COUNT];
	int wavetotal=FindWave(WAVE_ALL, wavelist, WAVE_MAX_COUNT);	// 查找录音，返回查到条数
	if(wavetotal>0)
	{
		DeleteWave(wavelist[wavetotal-1]);
	}
}

void CheckMissRecord(void)
{
	long WaveList[WAVE_MAX_COUNT];
	int RecTotal=FindWave(WAVE_ALL|WAVE_NEW, WaveList, WAVE_MAX_COUNT);	// 查找录音，返回查到条数
	if(RecTotal>0)
		SetLED(LED_MISSEDRECORD);
	else
		ClearLED(LED_MISSEDRECORD);
}
/////////////////////////////////////////////////
//
//     低级中断等结束录音
//
void OpenMuteTest(bool bMute);

inline void EndRecord(int dropbyte) 
{
	DropWaveTail(dropbyte);	
	DISABLE_TIMER2();
	CloseWave();
	if((g_bRecStatus==WAVE_AUTO)  && GetPhoneStatus()) ClosePhoneMic();
	g_bRecStatus=false;		//停止录音（停止采样）
	IMR &=~(0X00000002);			//打开Timer1中断
	ADCCSDisable();
//	ClearLED(LED_RECORD);
	ILCR=g_wOrgTimerIntLever;
//	ENABLE_TIMER2();
	LISR_VECTOR(2)=g_lOrgVector2;
	ENABLEPENIRQ();	
	InitPenDrv(false);
	SetBackLight();
//	OpenMuteTest(false);
	CheckMissRecord();
}

#define NORMAL_END		1
#define BUSY_END		2
#define DIAL_SOUND_END	3
#define MUTE_END		4
#define	DEACTIVE_END	5
#define TIME_END		6
#define MESSAGE_END		7
#define INTERRUPT_END	8

inline void EndRecord_MSG_HISR(int dropbyte,UINT32 iStatus) 
{
	if(g_bRecStatus && pRec)
		PostSystemMessage(WM_RECORD_END,0,pRec->m_iType, iStatus,true);
	EndRecord(dropbyte);
}

inline void EndRecord_MSG(int dropbyte ,UINT32 iStatus) 
{
	if(g_bRecStatus && pRec)
		PostSystemMessage(WM_RECORD_END,0,pRec->m_iType,iStatus,false);
	EndRecord(dropbyte);
}

/////////////////////////////////////////////////
//
//     播放提示音
//		返回：提示音及问候语总时长，秒
//
#define SECOND_REC	(*(UINT8 *)0XFFFFFB03)
int PromptSound()
{
	WAVEHEAD * pGreetingHead=(WAVEHEAD *)GREETINGWAVE_ROMADD;
	if(pGreetingHead->Flag!=WAVE_GREETING || pGreetingHead->Length <0 
		 ||pGreetingHead->Length >0x20000)
		 return 0;
	EnableLineOutput();
	UINT8 TotalTime=0,TimeReg=SECOND_REC;
	if(!RePlay(pGreetingHead,true,1))
		return 0;
	return 1;
}

/////////////////////////////////////////////////
//
//     允许静音检测
//
//     参数： bMute   1检测，0 不检测
//
/////////////////////////////////////////////////
/*void OpenMuteTest(bool bMute)
{
	bWaveAuto=bMute;
}*/

/////////////////////////////////////////////////
//
//     获得当前录音状态
//
bool GetRecordStatus()
{
	return g_bRecStatus;
}

/////////////////////////////////////////////////
//
//     获得当前放音状态
//
bool GetReplayStatus()
{
	return g_bReplayStatus;
}


/////////////////////////////////////////////////
//
//     AD
inline UINT8 SPITX(UINT16 TxData)
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

/////////////////////////////////////////////////
//  检测忙音脉宽
//     
static int iNarrowPlus=0;
inline void CheckPlusWide(int pw)	
{		
	if (pw<162  && pw>65 )  //脉宽 
	{
		if ( ABS(pw-iPrePeriod)<(pw>>4)) //方波误差1/16
		{
			if(	++iPlusNum>=TESTPERIOD)		//忙音1！
				EndRecord_MSG_HISR(pw*(++iPlusNum)*BUSYSAMPLEDIV,BUSY_END);	
		}
		else
			if(pw>132 && iPrePeriod>18 && iPrePeriod > 24 && iNarrowPlus>=3)//小波间隔
				if(	++iPlusNum>=TESTPERIOD)		//忙音2！小波型
					EndRecord_MSG_HISR((pw+(iPrePeriod<<2))*(iPlusNum)*BUSYSAMPLEDIV,BUSY_END);	
				else 
					iNarrowPlus=0;
			else
				iPlusNum=0;
	}
	else
	{
		if((pw<24 && pw>18)			//小波   _|_|_|_|_____|_|_|_|___   型 
     	  && ABS(pw-iPrePeriod)<(pw>>4) || (iPrePeriod<162 && iPrePeriod >132) )  
			iNarrowPlus++;
		else 
			iPlusNum=0;
	}
	iPrePeriod=pw;
}

//有关拨号音检测的变量
#define DIALWAVE	0x10	//拨号音振幅大于80mV
static int iSoundPlus=0;
static int iPlusTime=0;
static int iDiaSound=0;
static int iDiaVoltage=0x50; 
////////////////////////////////////////////////////
//        AD音频采样、8K速率
//
void LISR_ADSample(INT Vector)
{
	UINT8 val;
	static UINT8 OrgVal;
	if(ISR&0X00000020)
	{
		TSTAT2 =0;
		PBDATA  &= MBIT6; //7843 CS Enable
		if( !g_bRecStatus || !(WriteWave(val=(UINT8)SPITX(0xaF00)) )  )
			EndRecord_MSG_HISR(0,NORMAL_END);

		if(!(g_bRecStatus==WAVE_AUTO) ) return; 
//拨号音
		if(val>=iDiaVoltage && OrgVal<iDiaVoltage)//上升过零。
		{
			if(iSoundPlus>16 && iSoundPlus<20)  //450HZ +/- 15% 的拨号音 18 *125us=1/450 s
			{
				if(++iDiaSound>1000)
				{	
					EndRecord_MSG_HISR(0,DIAL_SOUND_END);
					DeleteLastWave();
					PostSystemMessage(WM_CLOSE,(unsigned long)pRec,(UINT32)pWave,0,true);
					return;
				}
			}
			else
				iDiaSound=0;
			iSoundPlus=0;
		}
		iSoundPlus++;
		OrgVal=val;

//静音
		if(ABS(val-g_MuteValue)<MUTEWAVE)//检测在一定范围内波动
		{
			if(iMuteTime--<=0)
			{
				EndRecord_MSG_HISR(MUTETIME,MUTE_END);
				return;
			}
		}
		else
			iMuteTime=MUTETIME;
//忙音
		if(iDivid--<=0)
		{
			if (PMDATA & SBIT5) //1 or 0 ?
			{
				if(!bOrgBusyStatu)		//是反转？
				{
					bOrgBusyStatu=1;
					CheckPlusWide(iPlusWide);	//判断脉宽iPlusWide++是否在范围内 
					iPlusWide=0;				//清宽位计数器开始重新计数
				}
			}
			else 
				bOrgBusyStatu=0;
			iPlusWide++;
			iDivid=BUSYSAMPLEDIV;
		}
	}
}

static bool bStop=false;
///////////////////////////////////////////////////////////////
//
//			停止放音
//
void StopPlay(bool bContinueNext,bool bActiveHisr)
{
	long WaveList[WAVE_MAX_COUNT];
	WaveLen=0;
	iPlayTime=0;
	PWMDISABLE();
//	ENABLETIMER1();	//ENABLE TIME1  
	(*(UINT32 *)0X118)=g_lOrgVector6;
	IMR&=~(0X00000002);
	if(g_bReplayStatus)
	{
		g_bReplayStatus=false;
		PostSystemMessage(WM_REPLAY_END,0,(UINT32)pWave,0,bActiveHisr);
	}
	else 
		return;			//avoid illagle call 
	CloseWave();	
	CheckMissRecord();
	DisablePWMOutput();
	DisableSpeakerOut();
	if(!bContinueNext)
		bStop=true;
}

///////////////////////////////////////////////////////////////
//
//			DA中断。
//
void PWM1LISR(int Vector)
{
	if(ISR	 & 0x80)
	{
		for(int LoadSamples=0; LoadSamples<4; LoadSamples++)
			PWMS1_8=ReadWave();
		PWMC1 &=0XFF7F;
		if(!g_bReplayStatus)
			StopPlay(CONTINUE_PLAY_MODE,SENDMSG_BY_HISR);
		if((WaveLen-=4)<=0 )
		{
			if (--iPlayTime>0) 
			{	
				CloseWave();
				if( !OpenWaveForRead((long)pWave) )
				{	
					StopPlay(CONTINUE_PLAY_MODE,SENDMSG_BY_HISR);
					return ;
				}
				WaveLen=GetWaveLen((long)pWave);
			}
			else
				StopPlay(CONTINUE_PLAY_MODE,SENDMSG_BY_HISR);
		}
	}
}

///////////////////////////////////////////////////////////////
//
//			DA初始化。DA采用脉宽调制
//
static void InitPWMWave(void)
{
	PBDIR	|=0X80;				//设置Pb7为专用引脚，上拉，I/O方向为输出
	PBPUL	|=0X80;
	PBSEL	&=0x7F;
	PWMP1= 0xFF;
	PWMC1= 0x0066;			//选用SYSCLK,使能中断，FIFO可用，33M/8/256/2 即16KHz回放8KHz,每数据样放2次
	IMR		&=0xFFFFff7F;	//开IMR（中断屏蔽寄存器）的PWM1中断，
}

////////////////////////////////////////////////////////////////
//
//			AD串口初始化
//
void ADInitSPI()
{	
	PESEL &=0XF8;		//PE 0:2 为SPI用
	PEPUL |=0X07;
	SPICONT= 0x420F ;	//SPI2使能，
	PBDIR |=  0x40;		//PB6 ADSchip
	PBPUL &= ~0x40;		
	SPICONT &=0XFE7F;	//清SPI发送标志	
	ADCCSEnable() ;		
	SPITX(0xAf00);		//ADS 初始化
	SPITX(0x0000);
	ADCCSDisable();

	PGSEL |=0X08;		//PG3 扬声器使能开关
	PGDIR |=0X08;
	PGPUL |=0X08;
	PGDATA |=0X08;
}

////////////////////////////////////////////////////////////////
//
//			AD7843初始化。
//
static void ADInitTimer2(int iFrequ) 	// initialize timer2
{
	g_wOrgTimerIntLever=ILCR;				//保存原timer2与其它中断等级
	VOID        (*recordlisr)(INT);   
	IMR |= 0x00000020;	// Mask timer2 int
	TCTL2 = 0x0014;		// Restart mode, comwpare interrupt, clk = sysclk/16, enable timer2
	TPRER2= 128;		// prescaler = 128
	TCMP2= 1;			// compare = 2
	TCR2= 0;			// clear capture
	TCN2= 0;			// clear counter
	TSTAT2= 0;			// clear status
		// initialize interrupt
	g_lOrgVector2=LISR_VECTOR(2);
	LISR_VECTOR(2)=(UINT32)LEVEL2_LISR;	
	ILCR=( (ILCR&0XFFF0) | TIMERINTLEVER );
	IMR &= ~0x00000020;	// enable timer2 int
	ADCCSEnable();
}

////////////////////////////////////////////////////////////////
//
//         电话状态检测变量初始化
//
void InitInspectorData()
{
	//静音
	iMuteTime=MUTETIME;				//静音时长

	//有关忙音
	bOrgBusyStatu=0;		//前采样值，用于判断忙音信号变跳
	iPlusWide=0;					//本周期脉宽
	iPlusNum=0;						//同周期数
	iDivid=BUSYSAMPLEDIV;			//分频系数
	iPrePeriod=0;					//前周期脉宽
	iNarrowPlus=0;

	//拨号音
	iSoundPlus=0;
	iPlusTime=0;
	iDiaSound=0;
	iDiaVoltage=g_MuteValue+DIALWAVE;		//波幅要大于80mV
}

////////////////////////////////////////////////////////////////
//
//         整型->字符串
//
void itos(char * dst,int num)
{	
	int gw=num%10;
	int pointPos = 0, i = 0;
	char stmp[20];
	while(num/=10)
	{
		stmp[pointPos++]=char('0'+gw);
		gw=num%10;
	}
	stmp[pointPos++]=char('0'+gw);
	while(pointPos--)
		dst[i++]=stmp[pointPos];
	dst[i]='\0';
}

//////////////////////////////////////////////////////////////////
//
//          日期->字符串
//
inline void DateToString(char * dst,DateTime * time)
{
	sprintf(dst,"%04d-%02d-%02d,%02d:%02d:%02d",time->year,time->month,
					time->day,time->hour,time->minute,time->second);
} 

//////////////////////////////////////////////////////////////////
//
//          录音设备启动
//
void RecordDeviceDrver(int type=WAVE_ALL)
{
	DISABLE_TIMER2();						//停止timer2
	PBSEL   |= 0x40;						//init ADC_CS
	ADInitSPI();							//串口初始化
	ADInitTimer2(1);						//7843 For Record初始化
	InitInspectorData();					//电话状态检测变量初值
	InitBusyInspect();						//初始化忙音检测端口
	DisablePWMOutput();						//禁止PWM向电话线上输出，使能MIC输入
	EnableFilterChannel();					//录音电源开
	DisableSpeakerOut();					//关闭功放
	ClearBackLight();						//关闭背光
	DISABLEPENIRQ();						//关闭触摸屏中断
//	DISABLETIMER1();						//DISABLE TIME1。可提高部分音质
	IMR |=0X00000002;						//屏蔽timer1 ,可提高部分音质!!!!!!!!!
//	SetLED(LED_RECORD);						//点亮录音开始灯
	ADCCSEnable();							//ads7843片选中
	g_bRecStatus=type;						//录音变量置真，录音开始
	ENABLE_TIMER2();						//打开timer2，进行中断采样。
}

/***************************************************************************************
*
*     录音应用
*  
*
*
*
*
***************************************************************************************/
#define RECORD_TIMEOUT	200*15		//录音留言免提后15秒超时。
#define KEYKEEPTIMEOUT	2
#define KEYDOWNS		1
#define KEYUPED			0

/*BEGIN_MESSAGE_MAP(CRecord, CFrameWnd)
  ON_MESSAGE(5551,	CN_COMMAND, OnBut)
END_MESSAGE_MAP */
////////////////////////////////////////////////////////////////////////////////////////////////////
//		重置录音设备
//
void CRecord::ResetRecord()		
{
//	m_butOK.SetWindowText("");
	strcpy(m_txt,"已录      秒");
//	OpenMuteTest(false);
	iMuteTime=MUTETIME;	//ygr
	EnableMICInput();
	switch (m_iType)
	{
	case WAVE_AUTO:
		DisableMICInput();	//录音MIC关
//		OpenMuteTest(true);
		strcpy(m_Title,"自动留言...");
		break;
	case WAVE_LINE:
		DisableMICInput();	//录音MIC关
		strcpy(m_Title,"电话录音....");
		break;
	case WAVE_MIC:
		strcpy(m_Title,"麦克风录音...");
		break;
	case WAVE_ALL:
		strcpy(m_Title,"正在录音...");
		break;
	}
	DateTime * systime=new DateTime;
	GetSysTime(systime);
	DateToString(m_sBeginTime,systime);
	delete systime;
	if(g_iScreenStatus!=SS_NORMAL && m_iType==WAVE_AUTO)
		m_bRecInScreen=true;
	else
	{
		this->ShowWindow();
		m_bRecInScreen=false;
	}
	m_bRec=true;
//打开外围端口
	RecordDeviceDrver(m_iType);
}

void CRecord::Destroy()
{
	ShowWindow(SW_HIDE);
}

BOOL CRecord::Create(char * strName,UINT32 dwStyle, const CRect &wndRect,UINT32 nHelpId)
{
	if(!CFrameWnd::Create(strName,dwStyle,wndRect,nHelpId))
		return FALSE;
/*	if(!m_butOK.Create("请稍候...", CRect(36,m_sRect.bottom-24,112,m_sRect.bottom-4),
		this, 5551))
		return false;*/
	m_bRec=false;
	m_bRecordKey=KEYUPED;
	m_bRecInScreen=false;
	m_bPrompt=false;
	return TRUE;
}


void CRecord::ShowWindow(UINT32 nCmdShow)
{
	extern void ShowWindow(CFrameWnd *);
	extern void HideWindow(CFrameWnd *);
	if(  g_iScreenStatus !=SS_NORMAL) return;
	if(nCmdShow == SW_SHOW)
	{   
		if(m_dwStyle & WS_VISIBLE)
			Paint();
		else
			::ShowWindow(this); 
	}
    else
	{
		if(GetRecordStatus())
			EndRecord_MSG(0,DEACTIVE_END);
		if(m_dwStyle & WS_VISIBLE)
			::HideWindow(this);
		m_bRecInScreen=false;
	}
}

void CRecord::OnPaint(CDC &dc)
{
	int left=m_sRect.left,top=m_sRect.top;
	dc.Draw3dButton(m_sRect,NULL);
	CRect r(m_sRect.left+3, m_sRect.top +3, m_sRect.right-3,m_sRect.bottom-30);
	dc.Draw3dButton(r,NULL);
	dc.SetBackColor(LGRAY);
	dc.SetPenColor(BLACK); 
	char stmp[20];
	dc.MoveTo(left +4,top+26);
	dc.LineTo(left+128,top+26);
	dc.TextOut(left +=8 ,top +6,m_Title);
	dc.TextOut(left,top+30,"开始时间:");
	dc.TextOut(left,top+46,m_sBeginTime+2);
	dc.TextOut(left,top+=62,m_txt,12);
	dc.TextOut(left,m_sRect.bottom-20,"按停止键停止录音");
	if(m_bRec)		//录音开始
	{
		itos(stmp,m_iCurSecond);
		dc.EraseRect(left,top,left+32,top+16);
		dc.TextOut(56,top,stmp,3);
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////
//		录音结束
//
extern CFrameWnd * g_pCurWindow;
void CRecord::RecordOK(UINT32 iType,UINT32 iEndStatus)
{	
	if(g_bRecStatus)
		EndRecord_MSG(0,iEndStatus);
	else
	{
		if(m_bRec)
			SetRecordEndFace(iType,iEndStatus);
	}
}

/////////////////////////////////////////////////////////////////////////////
//
void CRecord::SetRecordEndFace(UINT32 iType,UINT32 iEndStatus)
{
	strcpy(m_Title,"录音完毕!");
	long WaveList[WAVE_MAX_COUNT];
	if(iEndStatus!=DIAL_SOUND_END)
	{
		int RecTotal=FindWave(iType, WaveList, WAVE_MAX_COUNT);	// 查找录音，返回查到条数
		WAVEHEAD *RecNew=GetWavePointer(WaveList[RecTotal-1]);			//如何刷新放音列表
		char stmp[20];
		if(RecNew!=NULL)
			sprintf(m_txt,"录音总长%d秒",(RecNew->Length+8191)/8192);
	}
	else
		strcpy(m_txt," ");
	if(m_bRec)
	{
		CheckMissRecord();		//
		m_bRec=false;
		ShowWindow(SW_HIDE);
	}
}
/////////////////////////////////////////////////////////////////////////////
//
void CRecord::OnDactive()
{
	RecordOK(m_iType,DEACTIVE_END);
}
/////////////////////////////////////////////////////////////////////////////
//
void CRecord::WindowProcess(UINT32 nMessage , UINT32 wParam , UINT32 lParam)
{
	if(nMessage!=WM_RTC && TestLowPower()) return ;
	static UNSIGNED nPickMicTime=0;
	static bool bAutoRecord=false;
	int ntimes;
	ntimes=0;
	switch(nMessage)
	{
	case WM_EXIT_SCRPROT:
		if(m_bRecInScreen)
			this->ShowWindow();
		break;
	case WM_PENDOWN :
		DoPen(CPoint(HIWORD(wParam), LOWORD(wParam)));
		break;
	case WM_RECORD_START :
		if(g_bRecStatus) return;
		switch(wParam)
		{
		case RECORD_AUTO:
			NU_Sleep(200);		//ygr 11-22 evening 
			while(!GetPhoneStatus())
			{
				if( ntimes++>10) break;
				//OpenPhoneMic();
				MicMute();		//电话免提MIC
				NU_Sleep(1);
			}
			if(GetPhoneStatus())
			{
				bAutoRecord=true;
				nPickMicTime=NU_Retrieve_Clock();//1-12
			}
			return;
		case RECORD_LINE:
			if(m_bRecordKey!=KEYDOWNS)
				this->RunRecord(WAVE_LINE);
			else
			{
				m_iType=WAVE_LINE;
				Beep();
				m_bRecordKey=KEYKEEPTIMEOUT;
			}			
			return;
		case RECORD_MIC:
			if(m_bRecordKey!=KEYDOWNS)
				this->RunRecord(WAVE_MIC);
			else
			{
				m_iType=WAVE_MIC;
				Beep();
				m_bRecordKey=KEYKEEPTIMEOUT;
			}
			return;
		default:
			return;
		}
	case WM_KEY:
		if(wParam==KEY_RECORD && !GetRecordStatus())
			m_bRecordKey=KEYDOWNS;
		break;
	case WM_KEYUP:
		if(wParam==KEY_RECORD && m_bRecordKey==KEYKEEPTIMEOUT && !GetRecordStatus())
			this->RunRecord(m_iType);
		m_bRecordKey=KEYUPED;
		break;
	case WM_REPLAY_END:
		if(m_bPrompt)
			this->RunRecord(WAVE_AUTO);
		return;
	case WM_MICOPEN:
		if(bAutoRecord)
		{
			m_bPrompt=false;
			bAutoRecord=false;
			UNSIGNED nrecordtime=NU_Retrieve_Clock();
			if((nrecordtime-nPickMicTime)>RECORD_TIMEOUT)
				return ;
			if(!PromptSound())//没有问候语
				this->RunRecord(WAVE_AUTO);
			else
				m_bPrompt=true;
			return;
		}
	case WM_PICKUP:    //手柄摘机
	case WM_RING:      //振铃 wParam 指向CallId的字符串指针
	case WM_PICKDOWN:  //手柄挂机
	case WM_MICCLOSE:  //免提挂机 
	case WM_FLASH:     //闪断
	case WM_SPEAKINGPICKUP:
			if(nMessage==WM_SPEAKINGPICKUP)
				g_bRecStatus=WAVE_ALL;
			m_bPrompt=false;
	case WM_RECORD_END:
		if(g_bRecStatus)
			this->RecordOK(m_iType,MESSAGE_END);
		else
			this->RecordOK(wParam,lParam);
		return;
	}
	CFrameWnd::WindowProcess(nMessage, wParam, lParam);
}

void CRecord::DoPen(CPoint &pt)
{
	if(!pt.InRect(m_sRect))
		ShowWindow(SW_HIDE);
}

extern "C" void HwPaintRect(INT16 x0, INT16 y0, INT16 x1, INT16 y1, UINT8 iColor);
void CRecord::OnRtc(UINT16 nRtcType)
{
	char stmp[4]={0,0,0,0};
	if(nRtcType==RTCI_SECOND && m_bRec )
	{
		if(g_bRecStatus && m_iCurSecond<m_iTime) 
		{
			m_iCurSecond++;
			if( g_iScreenStatus!=SS_NORMAL) return;
			itos(stmp,this->m_iCurSecond);
			if(GetWindowStyle() & WS_VISIBLE)
			{
				HwPaintRect(56, POPWNDTOP+62, 88,POPWNDTOP+78 , LGRAY);
				HwPutString(56,POPWNDTOP+62,LGRAY,BLACK,0,(UINT8 *)stmp,0);//left+36,top+62
			}
			else
				RecordOK(m_iType,DEACTIVE_END);
		}
		else
		{ 
			RecordOK(m_iType,TIME_END);
		}
	}
}
////////////////////////////////////////////////////////////////////////////////
//   开始录音
//
BOOL CRecord::RunRecord(WAVETYPE iType,int iTotalTime)
{
	if(g_bRecStatus) return false;
	if(g_bReplayStatus) StopPlay();  //正在放音则停止
	m_bPrompt=false;
	m_iCurSecond=0;		//12-6;ygr
	if(iType==WAVE_AUTO)// Replay(提示);//给出提示音和嘟声
	{
		NU_Sleep(60);
		//	EnableLineOutput();
		Sound(800);
		NU_Sleep(80);
		NoSound();
		DisablePWMOutput();
		DisableSpeakerOut();
	}
//	if(GetPhoneStatus()) return false;
	//	m_iCurSecond=-(PromptSound());
	if(!OpenWaveForWrite(iType))
	{
		MessageTip("留言空间已满！");
		return false;	//打开失败，
	} 
	m_bRec=false;
	m_iType=iType;
	if(iTotalTime>0)
		m_iTime=iTotalTime;
	else
		m_iTime=0x7fffffff;
	ResetRecord();
	return true;
}

/*******************************************************************************************************************
	Record function!
*******************************************************************************************************************/
//用于最先的初始化，只能运行一次
//void RecorderInspect(void);
void RecorderInspectCheck(void);

void initRecorder(void)
{
	InitWave();
	g_lOrgVector6=(*(UINT32 *)0X118);
	//RecorderInspect();
	RecorderInspectCheck();
	CheckMissRecord();
	EnableLEDLight();
	if (!pRec)
		pRec = new CRecord;
	pRec->Create("录音", WS_CAPTION,POPWNDRECT,0);//DESK_TOP_RECT, 0); 
}

#undef RECORD_TIMEOUT	
#undef KEYKEEPTIMEOUT	
#undef KEYDOWNS			
#undef KEYUPED			


#include "ring.h"
////////////////////////////////////////////////////////////////////////////////
//   录音接口函数
//
bool Record(WAVETYPE Type, INT32 RecTime)  
{
	return 	pRec->RunRecord(Type,RecTime);
}

////////////////////////////////////////////////////////////////////////////////
//   停止录音
//
void StopRec(bool bSendMsgHisr)
{
	if(bSendMsgHisr)
		EndRecord_MSG_HISR(0,INTERRUPT_END);
	else
		EndRecord_MSG(0,INTERRUPT_END);
}

#undef BUSY_END		
#undef NORMAL_END		
#undef DIAL_SOUND_END	
#undef MUTE_END		
#undef DEACTIVE_END	
#undef TIME_END		
#undef MESSAGE_END	
#undef INTERRUPT_END	

/************************************************************************************************************************
*
*
*
*
*
*		回放录音函数
*			REPLAY FUX
*
*
*
*
*
*
************************************************************************************************************************/
#define IDC_BT		710
#define IDC_BTCLR	711
#define IDC_BTPLAY	712
#define IDC_BTSTOP	713
#define IDC_BTEDIT	714
#define IDC_BTHELP	715

#define IDM_PLAY	701
#define IDM_DEL		702
#define IDM_CANCEL	703
#define IDM_RENAME  704
#define IDM_CLR		705
#define IDM_AUTO	706
#define IDM_MIC		707
#define IDM_LINE	708
#define IDM_ALL		709
#define IDC_GRID	5001
#define IDC_SLIDER	5002

#define	ICON_WAVE_AUTONEW	ICON_MICNEWREC//(ICON_NOSPEAKER)
#define ICON_WAVE_LINENEW	ICON_MICNEWREC//(ICON_LINENEWREC)
#define ICON_WAVE_MICNEW	ICON_MICNEWREC
#define ICON_WAVE_ALLNEW	0

#define	ICON_WAVE_AUTO		ICON_MICREC//(ICON_SPEAKER)
#define ICON_WAVE_LINE		ICON_MICREC//(ICON_LINEREC)
#define ICON_WAVE_MIC		ICON_MICREC
#define ICON_WAVE_ALL		0

CPlay * pPopWndReplay=NULL;
/////////////////////////////////////////////////////////////////////////
//
//			异步回放录音驱动
//
bool RePlay( WAVEHEAD* Wave,bool BreakMod,int PlayTime)
{
	if(BreakMod)
		if(g_bReplayStatus)	StopPlay();
	VOID        (*recordlisr)(INT);   
	if(Wave==NULL || !OpenWaveForRead((long)Wave))
		return false;
	pWave=Wave;
	WaveLen=GetWaveLen((long)Wave);
	iPlayTime=PlayTime;
	InitPWMWave();
	StopRing(false);
//	DISABLETIMER1();						//DISABLE TIME1
	IMR |=0X00000002;						//DISABLE TIME1
	g_lOrgVector6=(*(UINT32 *)0X118);
	(*(UINT32 *)0X118 )=(UINT32)LEVEL6_LISR;
	int TmpLen=WaveLen;
	g_bReplayStatus=true;
	EnableSpeakerOut();						//功放喇叭使能
	PWMENABLE();							//PWM输出使能 寄存器开关
	EnablePWMOutput();						//PWM通道使能 I/O端口控制
	EnableMICInput();						//MIC 开，LINE 关
	return true;
}


CRePlay *pWaveApp = NULL;
/////////////////////////////////////////////////////////////////////////
//
//			留言应用程序调用
//
void RecordBrowseAppRun()
{
	//录音留言
	if(!pWaveApp)
		pWaveApp = new CRePlay;
	pWaveApp->Create("留言", WAVE_ALL); 
	pWaveApp->ShowWindow(); 
	::SetCurrentApp(APPID_RECBROWSE);
}

/////////////////////////////////////////////////////////////////////////
//
//			根据类型取得图标
//
int GetWaveIcon(WAVETYPE iType)
{
	int icon;
	if (iType & WAVE_NEW)
	{
		switch(iType & WAVE_TYPE)
		{
		case WAVE_MIC:
			icon=ICON_WAVE_MICNEW;
			break;
		case WAVE_AUTO:
			icon=ICON_WAVE_AUTONEW;
			break;
		case WAVE_LINE:
			icon=ICON_WAVE_LINENEW;
			break; 
		default:
			icon=ICON_WAVE_ALLNEW;
		}
	}
	else
	{
		switch(iType & WAVE_TYPE)
		{
		case WAVE_MIC:
			icon=ICON_WAVE_MIC;
			break;
		case WAVE_AUTO:
			icon=ICON_WAVE_AUTO;
			break;
		case WAVE_LINE:
			icon=ICON_WAVE_LINE;
			break; 
		default:
			icon=ICON_WAVE_ALL;
		}
	}
	return icon;
}

BEGIN_MESSAGE_MAP(CRePlay, CAppWnd)
  ON_MESSAGE(IDC_GRID,	CN_GRIDSEL, OnGrid)
  ON_MESSAGE(IDC_BTCLR,	CN_COMMAND, OnDeleteAllRecord)
END_MESSAGE_MAP 

///////////////////////////////////////////////////////////////////////////////
//
//         

#define YPOSH	24
#define YPOSL	64
void CRePlay::WindowProcess(UINT32 nMessage,UINT32 wParam,UINT32 lParam)
{
	switch(nMessage)
	{
	case WM_KEYUP:
		if(wParam!=KEY_PLAY) return;
		m_iType |=WAVE_NEW;//按下STOP键后改为查看新留言。
		if(bStop)
			bStop=false;
		PlayNewRecord();
		break;
	}
	CAppWnd::WindowProcess(nMessage,wParam,lParam);
}

void CRePlay::OnGrid(UINT32 nMessage, UINT32 nSrcId, UINT32 lParam)
{
	m_iCurRec=lParam&0xffff;
	CRect r(2,YPOSH,142,YPOSH+76);
	if(pPrompt.Create( NULL,POPWNDRECT,HELP_RECORD_DETAIL
	          	,m_iRecTotal-m_iCurRec))
		pPrompt.ShowWindow();
}
#undef  YPOSH
#undef	YPOSL


////////////////////////////////////////////////////////////////////////////////////// 
//
//		删除指定行的的录音。
//
void CRePlay::DeleteSigleWave(int NumOfWave)
{
	if(NumOfWave<=m_iRecTotal && m_iRecTotal>0)
	{
		long id=WaveList[m_iRecTotal-NumOfWave];
		if(DeleteWave(id))
		{
			m_grid.RemoveAll();
			m_iRecTotal=0;
			m_iCurRec=0;
			ReFill(WAVE_ALL,true);//m_iType);
		}
	}
}
////////////////////////////////////////////////////////////////////////////////////// 
//
//		删除所有的录音。
//
void CRePlay::OnDeleteAllRecord(UINT32 nMessage, UINT32 nSrcId, UINT32 lParam)
{ 
	long wavelist[WAVE_MAX_COUNT];
	int RecTotal=FindWave(WAVE_ALL, wavelist, WAVE_MAX_COUNT);	// 查找录音，返回查到条数
	if(RecTotal>0)
	{	
		if(MessageBox("所有录音都将被删除！确定吗？","清空", MB_YESNO|MB_ICONEXCLAMATION)==IDYES)
		{
			DeleteAllWave();	// 删除所有录音
			m_grid.RemoveAll();
			m_iRecTotal=0;
			m_iCurRec=0;
			this->Paint();
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////// 
//
//		判断新的录音，有则添。
//
void CRePlay::ReFill(int Type,bool bCheckStatus)
{ 
	long wavelist[WAVE_MAX_COUNT];	
	char *ptxt[REC_COL]={NULL,NULL};
	char pstyle[REC_COL]={UNS_TEXT|RS_CANNOTSELECT,UNS_TEXT|RS_CANNOTSELECT};
	UINT32 picon[REC_COL]={0,0};
	CheckMissRecord();
	int	iRecTotal=FindWave(Type, WaveList, WAVE_MAX_COUNT);			// 查找录音，返回查到条数
	for(int i=m_iRecTotal;i<iRecTotal;i++)		//10.164.1.77
	{
		WAVEHEAD *pWave=(GetWavePointer(WaveList[i]));
		AppendNewLine(pWave->Flag,pWave->Name,pWave->Time ); //((pWave->Length+8191)/8192)); 1-11
	}
	m_iRecTotal=iRecTotal;
	if(!bCheckStatus)
		CheckStatus(pPrompt.m_istatus);
	pPrompt.m_istatus=0;
	m_grid.SetStartRow(0);
}

/////////////////////////////////////////////////////////////////////////
//
//			根据录音类型重画图标
//
void CRePlay::CheckStatus(int istatus)
{
	char stmp[14];
	if(istatus==2)
		DeleteSigleWave(m_iCurRec);
	else if(istatus==1)
	{
		WAVEHEAD * phwave=GetWavePointer(WaveList[m_iRecTotal-m_iCurRec]);
		int icon=GetWaveIcon(phwave->Flag);
		m_grid.GetUnitText(m_iCurRec,1,stmp,12);
		m_grid.SetUnitText(m_iCurRec,1,stmp,icon,true);
	}
}

////////////////////////////////////////////////////////////////////////////////////// 
//
//		添加新的行在最前面。
//
void CRePlay::AppendNewLine(WAVETYPE iType,char * txt,DateTime &time) // int timelong)//char * strDate)
{
	char strCol2[8];
	//txt[11]='\0';
	//sprintf(strCol2,"%3d",timelong);1-11
	sprintf(strCol2,"%02d:%02d",time.hour,time.minute);
	char * rowtxt[REC_COL]={txt,strCol2};
	UINT32 icon[REC_COL+1]={0,0,0};
	char style[REC_COL]={UNS_TEXT,UNS_TEXT};
	icon[0]=GetWaveIcon(iType);	
	m_grid.InsertRow(1,rowtxt,style,icon);
}

BOOL CRePlay::Create(char * strName, WAVETYPE type)
{
	m_iType=type;
	if(!CAppWnd::Create( strName,WS_NORMAL & ~WS_BORDER,DESK_TOP_RECT,
		HELP_RECBROWSE))
		return false;
	return true;
}

BOOL CRePlay::OnCreate()
{
	CRect r1 = GetWindowRect();
	r1.top   += (WND_TITLE_HEIGHT+1);
	r1.right -=18;
	m_grid.Create(this,r1,TBS_SPACELINE|TBS_ROWSELECT,0,2,IDC_GRID);

	UINT16	 ColWidth[REC_COL]={96, 46};
	m_grid.SetColsWidth(ColWidth);
	if(!CAppWnd::OnCreate())
		return FALSE;
	CRect r(m_sRect.left + TITLE_WIDTH,  m_sRect.top , 
		m_sRect.left + TITLE_WIDTH + BTN_WIDTH -1, m_sRect.top + WND_TITLE_HEIGHT -1);
	if(!m_obDelete.Create("清空", r, this, IDC_BTCLR))
		return FALSE;
	m_iCurRec=0;
	m_iRecTotal=0;
	pPrompt.m_istatus=0;
	return TRUE;
}

void CRePlay::ShowWindow(UINT32 nCmdShow )
{
	if(nCmdShow == SW_SHOW)
	{   
//		if(m_dwStyle & WS_VISIBLE)
//			Paint();
		::ShowWindow(this); 
		if(!(m_iType&WAVE_NEW))
		{
			int len=WAVE_STORE_LEN-GetWaveUsed();
			if(len<=sizeof(WAVEHEAD))
				 //MessageBox("录音留言空间已满！","警告",MB_OK|MB_ICONINFORMATION);
				 MessageTip("录音留言空间已满！");
			else if(len<=(MUTETIMESECOND+1)*8192)//
				 //MessageBox("录音留言空间快满，留言功能失效！","警告",MB_OK|MB_ICONINFORMATION);
				 MessageTip("录音留言空间快满，留言功能失效！");
		}
	}
    else
	{
		if(m_dwStyle & WS_VISIBLE)
			::HideWindow(this);
	}
}

BOOL CRePlay::OnDestroy()
{
	if(g_bReplayStatus)
		StopPlay();
	::SetCurrentApp();
	return CAppWnd::OnDestroy();
}

void CRePlay::OnPaint( CDC &dc)
{
	ReFill(WAVE_ALL);//m_iType);
	CAppWnd::OnPaint(dc); 
	dc.Reset();
//	dc.TextOut(44,m_sRect.top+WND_TITLE_HEIGHT+1,"名称   时间");
	if(!bStop)
		PlayNewRecord();
}

void CRePlay::PlayNewRecord()
{
	if(m_iType&WAVE_NEW)
	{
		long WaveList[WAVE_MAX_COUNT];
		int RecTotal=FindWave(WAVE_ALL|WAVE_NEW, WaveList, WAVE_MAX_COUNT);	// 查找录音，返回查到条数
		if(RecTotal>0)			//播放第一条录音
		{
			if(!pPopWndReplay)
				pPopWndReplay=new CPlay;
			if(bCPlayOnTop)
				pPopWndReplay->Destroy();
			long id=WaveList[0];
			pPopWndReplay->Create(id,this);
			if(pPopWndReplay->Process(1))
				pPopWndReplay->ShowWindow(); 
			//redraw icon
			RecTotal=FindWave(WAVE_ALL, WaveList, WAVE_MAX_COUNT);	// 查找录音，返回查到条数
			for(int i=0;i<RecTotal;i++)
			{
				if(id==WaveList[i])
				{
					WAVEHEAD * phwave=GetWavePointer(id);
					int icon=GetWaveIcon(phwave->Flag);
					char stmp[14];
					int iRow=RecTotal-i;
					m_grid.GetUnitText(iRow,1,stmp,12);
					m_grid.SetUnitText(iRow,1,stmp,icon,false);
					return;
				}
			}
		}
		else
			m_iType &= ~WAVE_NEW;//新留言完毕

	}
}
/**********************************************************************************/
/*                                                                                */
/*                                                                                */
/*                                                                                */
/*                      CProcessBar 控件                                          */
/*                                                                                */
/*                                                                                */
/*                                                                                */
/*                                                                                */
/*                                                                                */       
/**********************************************************************************/
#define SLIDER_WIDTH	8
#define	SLIDER_HEIGHT	14  // hqf
///////////////////////////////////////////////////////////////////////////////
//
void myline(CDC &dc,int x1,int y1,int x2,int y2,int color=BLACK)
{
	dc.SetPenColor(color);
	dc.MoveTo(x1,y1);
	dc.LineTo(x2,y2);
}
///////////////////////////////////////////////////////////////////////////////
//   设置进度条风格
//
void CProcessBar::SetCtrlStyle(int iStyle)
{
	m_iStyle=iStyle;
}

///////////////////////////////////////////////////////////////////////////////
//   获取进度条风格
//
int CProcessBar::GetCtrlStyle(void)
{
	return m_iStyle;
}

///////////////////////////////////////////////////////////////////////////////
//
void CProcessBar::OnPaint(CDC &dc)
{
	if(m_sRect.Height()<SLIDER_HEIGHT) return;
	int iVCenter=m_sRect.top+SLIDER_HEIGHT/2;
	int left=m_sRect.left+2;	
	int right=m_sRect.right-1;	
	CRect r=CRect(0,0,SLIDER_WIDTH,SLIDER_HEIGHT);
	dc.SetBackColor(LGRAY);
	CRect r2=m_sRect;
	int ichartop=m_sRect.top+SLIDER_HEIGHT-1;
	if(r2.bottom <ichartop)return ;
	if(m_pbarback)	//12-21
	{
		if(m_pbarback->pBuffer)
		{
			SysFree(m_pbarback->pBuffer);
			m_pbarback->pBuffer=NULL;
		}
		SysFree(m_pbarback);
		m_pbarback=NULL;
	}
	if(m_iStyle & CS_BIAOTI && r2.bottom >=ichartop+16 && r2.bottom<160)
		r2.bottom =ichartop+17;
	else
		r2.bottom =ichartop+1;
	if(m_iStyle&CS_FILECOPY)
	{
		CRect r2=m_sRect;
		r2.DeflateRect(1,0);
		dc.SetBackColor();
		r2.left=m_iBarLeft;
		dc.EraseRect(r2);
		dc.SetPenColor();
		dc.FrameRect(m_sRect);
		dc.SetBackColor(GRAY);
		CRect r3=m_sRect;
		r3.DeflateRect(1,0);
		r3.DeflateRect(1,1);
		r3.right=m_iBarLeft;
		dc.EraseRect(r3);
	}
	else
	{
		dc.EraseRect(r2);
		myline(dc,left,	iVCenter-2,	right,	iVCenter-2,	GRAY);
		myline(dc,left,	iVCenter+2,	right,	iVCenter+2,	WHITE);
		dc.SetPenWidth(2);
		myline(dc,left ,iVCenter+1,	left ,	iVCenter-2,	GRAY);
		myline(dc,right,iVCenter+1,	right,	iVCenter-2,	GRAY);
		dc.SetPenWidth(1);
		myline(dc,left,	iVCenter-1,	right,	iVCenter-1,	BLACK);
		if(m_iBarLeft+SLIDER_WIDTH>r2.right)
			m_iBarLeft=r2.right-SLIDER_WIDTH;
		r.MoveTo(m_iBarLeft,m_sRect.top );
		m_pbarback=dc.SaveScreen(r);
		dc.Draw3dButton(r,"");	
	}
	if(m_sRect.bottom>=(ichartop+16) && m_sRect.Width()>56  )
	{
		if( m_iStyle&CS_BIAOCHI )
		{
			char stmp[5];
			sprintf(stmp,"%d",m_iMin);
			dc.TextOut(m_sRect.left ,ichartop,stmp);
			sprintf(stmp,"%3d",m_iMax);
			dc.TextOut(m_sRect.right-24 ,ichartop,stmp);
		}
		if( m_iStyle&CS_BIAOTI )
			dc.TextOut(m_sRect.left+16,ichartop,m_sCaption);
	}
}

///////////////////////////////////////////////////////////////////////////////
//
void CProcessBar::MoveBar()
{
	CDC dc;
	if(m_iStyle&CS_FILECOPY)
	{
		CRect r2=m_sRect;
		r2.DeflateRect(1,0) ;
		r2.left=m_iBarLeft;
		dc.SetBackColor();
		dc.EraseRect(r2);
		dc.SetPenColor();
		dc.FrameRect(m_sRect);
		dc.SetBackColor(GRAY);
		CRect r3=m_sRect;
		r3.DeflateRect(1,0);
		r3.DeflateRect(1,1);
		r3.right=m_iBarLeft;
		dc.EraseRect(r3);
	}
	else
	{
	dc.PutScreen(m_pbarback ); 
	CRect r=CRect(0,0,SLIDER_WIDTH,SLIDER_HEIGHT);
	r.MoveTo(m_iBarLeft,m_sRect.top );
	if(m_iBarLeft+SLIDER_WIDTH>m_sRect.right)
		m_iBarLeft=m_sRect.right-SLIDER_WIDTH;
	r.MoveTo(m_iBarLeft,m_sRect.top );
	m_pbarback=dc.SaveScreen(r);
	dc.Draw3dButton(r,"");	
	}
}

///////////////////////////////////////////////////////////////////////////////
//
BOOL CProcessBar::Create(int iMin,int iMax,int iStep,CRect &ctrlRect, CFrameWnd * pParent,int iStyle, UINT32 nCtrlId)
{
	m_iMax=m_iMin=0;
	m_iBarLeft=ctrlRect.left;
	m_iCurValue=0;
	m_pbarback =NULL;
	if(ctrlRect.Height()>=SLIDER_HEIGHT+16)
		ctrlRect.bottom=ctrlRect.top+SLIDER_HEIGHT+16;
	else 
	{
		if(ctrlRect.Height()>=SLIDER_HEIGHT)
			ctrlRect.bottom=ctrlRect.top+SLIDER_HEIGHT-1;
		else
			return false;
	}
	if(ctrlRect.bottom>LCD_HEIGHT) return false;
	if(CCtrl::Create(NULL, ctrlRect, pParent, WS_CHILD|WS_VISIBLE, nCtrlId)) 
	{
		SetCurRange(iMin,iMax,iStep);
		m_sCaption[0]='\0';
		m_iStyle=iStyle;
		return true;
	}
	else
		return false;
}

///////////////////////////////////////////////////////////////////////////////
//
void CProcessBar::Destroy()
{
	if(m_pbarback)	//12-21
	{
		if(m_pbarback->pBuffer)
		{
			SysFree(m_pbarback->pBuffer);
			m_pbarback->pBuffer=NULL;
		}
		SysFree(m_pbarback);
		m_pbarback=NULL;
	}
	CCtrl::Destroy();
}


///////////////////////////////////////////////////////////////////////////////
//
int	CProcessBar::GetCurValue(void)
{
	return m_iCurValue;
}

///////////////////////////////////////////////////////////////////////////////
//
void CProcessBar::SetCurValue(int iValue ,bool bRedraw)
{
	if(iValue<=m_iMax && iValue>=m_iMin)
	{
		m_iCurValue=iValue;
		if(m_iStyle&CS_FILECOPY)
			m_iBarLeft=(m_iCurValue-m_iMin)*(m_sRect.right-m_sRect.left-2)*m_iStep/m_iRange+m_sRect.left;
		else
			m_iBarLeft=(m_iCurValue-m_iMin)*(m_sRect.right-m_sRect.left-SLIDER_WIDTH)*m_iStep/m_iRange+m_sRect.left;
		m_iXCurPlus=m_iCurValue%m_iXPlusTimes;
		if(bRedraw)
		{
			CDC dc;
			OnPaint(dc);
		}
	}
}
	
///////////////////////////////////////////////////////////////////////////////
//
int	CProcessBar::GetCurRange(void)
{
	return m_iRange;
}

///////////////////////////////////////////////////////////////////////////////
//
void CProcessBar::SetCurRange(int iMin,int iMax,int iStep)
{
	int width=m_sRect.Width();
	if(iMin<iMax && width)
	{
		if(iMin>=0)
		{
			m_iMin=iMin;
			m_iMax=iMax;
			m_iRange=(iMax-iMin);
			m_iStep=iStep;
			if(m_iRange)
			{
				m_iXPlusTimes=m_iRange/width+1;
				m_iXInterval=((width-SLIDER_WIDTH)*iStep)*m_iXPlusTimes/m_iRange;
			}
			else
				m_iRange=0;
			m_iCurValue=m_iMin;
			m_iBarLeft=m_iXInterval*(m_iCurValue-m_iMin)+m_sRect.left;
		}
	}
	else
	{
			m_iMin=iMax;
			m_iMax=iMax;
			m_iRange=iMax;
			m_iXInterval=0;
			m_iStep=iMax;
			m_iCurValue=iMax;
			m_iBarLeft=m_sRect.VCenter();
	}
}

void CProcessBar::WindowProcess(UINT32 nMessage, UINT32 wParam, UINT32 lParam)
{
	if(!(m_iStyle & CS_PENMOVE)) return;
    switch(nMessage)
    {
    case WM_PENMOVEIN:
	case WM_PENMOVE:
		DoPen(CPoint(HIWORD(wParam), LOWORD(wParam)));
		break;
    }
}

void CProcessBar::DoPen(CPoint &pt)
{
	if(pt.InRect(m_sRect))
	{
		if(pt.y>m_sRect.top+SLIDER_HEIGHT) return;
		if(pt.x>m_iBarLeft+SLIDER_WIDTH)
			this->operator ++(); 
		else
			if(pt.x<m_iBarLeft)
				this->operator --();
		int val2=GetCurValue();
		SendNotifyMessage(CS_PROCESS,val2);
	}
}

void CProcessBar::operator --()
{
	m_iCurValue-=m_iStep;
	if(m_iCurValue<m_iMin)
		m_iCurValue=m_iMin;
	else
	{
		m_iXCurPlus=m_iCurValue%m_iXPlusTimes;
		if(!m_iXCurPlus)
		{
			if(m_iStyle&CS_FILECOPY)
				m_iBarLeft=(m_iCurValue-m_iMin)*(m_sRect.right-m_sRect.left-2)*m_iStep/m_iRange+m_sRect.left;
			else
				m_iBarLeft=(m_iCurValue-m_iMin)*(m_sRect.Width()-SLIDER_WIDTH)*m_iStep/m_iRange+m_sRect.left;
			MoveBar();
		}
	}
}

void CProcessBar::operator ++()
{
	m_iCurValue+=m_iStep;
	if(m_iCurValue>m_iMax)
		m_iCurValue=m_iMax;
	else
	{
		m_iXCurPlus=m_iCurValue%m_iXPlusTimes;
		if(!m_iXCurPlus)
		{
			if(m_iStyle&CS_FILECOPY)
				m_iBarLeft=(m_iCurValue-m_iMin)*(m_sRect.right-m_sRect.left-2)*m_iStep/m_iRange+m_sRect.left;
			else		
				m_iBarLeft=(m_iCurValue-m_iMin)*(m_sRect.Width()-SLIDER_WIDTH)*m_iStep/m_iRange+m_sRect.left;
			MoveBar();
		}
	}
}

void CProcessBar::operator =(const int iValue)
{
	SetCurValue(iValue,true);
}

int	CProcessBar::GetCurStep(void)	
{		
	return m_iStep;	
}

#undef SLIDER_WIDTH	
#undef	SLIDER_HEIGHT	
/****************************************************************************/
/*                                                                          */
/*         放音弹出窗口                                                     */
/*                                                                          */
/****************************************************************************/
void CPlay::OnBut()
{
	StopPlay();  //ygr 12/20
	//this->Destroy();
}

#include "datetime.h"

///////////////////////////////////////////////////////////////////////////////
//
void CPlay::Destroy()
{
	bCPlayOnTop=false;
	StopPlay();
	CFrameWnd::Destroy ();
	if(m_bMessage && m_parent && ((CFrameWnd *)m_parent)->OnDestroy() )
		m_parent->SendMessage(WM_REPLAY_END,0,m_lRecId);
}

void CPlay::OnDactive()
{
	bStop=true;
	this->Destroy();
}

void CPlay::plus()
{
	++m_oSlider;
}

///////////////////////////////////////////////////////////////////////////////
//
void CPlay::OnPaint(CDC &dc)
{
	dc.Draw3dButton(m_sRect,NULL);
	CRect r(m_sRect.left+3, POPWNDTOP+3, m_sRect.right-3,m_sRect.bottom-30);
	dc.Draw3dButton(r,NULL);
	CFrameWnd::PaintChildWnd();		
	dc.SetBackColor(LGRAY);
	dc.SetPenColor();
	dc.TextOut(16,POPWNDTOP+5,m_strName);
	dc.MoveTo(m_sRect.left+5,POPWNDTOP +22);
	dc.LineTo(125,POPWNDTOP +22);
	dc.TextOut(16,POPWNDTOP+24,m_stitle);
}

///////////////////////////////////////////////////////////////////////////////
// 
BOOL CPlay::Create(long RecId ,CWnd * parent) //	CRect(8, 10, 139, 130)
{
	if(!CFrameWnd::Create(NULL,WS_VISIBLE,POPWNDRECT,HELP_RECORD_DETAIL_PLAY))
		return FALSE;
	WAVEHEAD* pWave;
	pWave=GetWavePointer(RecId);
	m_parent=parent;
	if(!m_oSlider.Create( 0,(pWave->Length+8191)/8192,1,CRect(20,POPWNDTOP+46,140,POPWNDTOP+86), this, 
						   CS_BIAOCHI,IDC_SLIDER))
		return FALSE;
	sprintf(m_strName,"播放:%s",pWave->Name);
	char stemp[20];
	DateToString(stemp,&pWave->Time);
	strcpy(m_stitle,(char *)(stemp+2) );
	CRect r(36,m_sRect.bottom-24,112,m_sRect.bottom-4);
	if(!m_butOK.Create("停止播放", r, this, 5555))
		return FALSE;
	if(!CFrameWnd::OnCreate())
		return FALSE;
	m_lRecId=RecId;
	m_bRTC=false;
	bCPlayOnTop=true;
	m_bMessage=false;
	return true;
}

///////////////////////////////////////////////////////////////////////////////
//
//  播放 1 成功，0 失败
//
bool CPlay::Process(bool bOnEndDestroy)
{
	WAVEHEAD *pHead=GetWavePointer(m_lRecId);
	if(RePlay(pHead,true,1))
	{
//		InitRtcDriver(); hqf
		OpenRtcInterrupt(RTCI_SECOND);
		m_bRTC=true;
		m_bOnEndDestroy=bOnEndDestroy;
		return true;
	}
	else
		return false;
}

///////////////////////////////////////////////////////////////////////////////
//
void CPlay::OnRtc(UINT16 nRtcType)
{
	if(m_bRTC && nRtcType==RTCI_SECOND)
	{
		if((m_oSlider.GetCurValue()<=m_oSlider.GetMax()) && g_bReplayStatus)	//如谢意结束应destor
			++m_oSlider;
		else
			if(m_bOnEndDestroy)
			{
				StopPlay();
				m_bRTC=false;
			}
	}
}

void CPlay::WindowProcess(UINT32 nMessage,UINT32 wParam,UINT32 lParam)
{
	switch(nMessage)
	{
	case WM_KEYUP:
		if(wParam==KEY_PLAY) 
		{
			long WaveList[WAVE_MAX_COUNT];
			int RecTotal=FindWave(WAVE_ALL|WAVE_NEW, WaveList, WAVE_MAX_COUNT);	// 查找录音，返回查到条数
			if(RecTotal<=0) return;
			StopPlay();
			bStop=false;
			m_bRTC=false;
		}
		else if(wParam==KEY_STOP)
		{
			StopPlay();
			bStop=true;
		}
		break;

	case WM_REPLAY_END:
		m_bMessage=true;
		this->Destroy();
		break;
	}
	CFrameWnd::WindowProcess( nMessage, wParam, lParam);
}
BEGIN_MESSAGE_MAP(CPlay, CFrameWnd)
  ON_MESSAGE(5555,	CN_COMMAND, OnBut)
END_MESSAGE_MAP 

/*****************************************************************************
*
*
*       留言提示窗口
*
*
*
*
******************************************************************************/
///////////////////////////////////////////////////////////////////////////////
//
//
#define IDC_BUTPLAY		6666
#define IDC_BUTDEL		IDC_BUTPLAY+1
#define IDC_BUTEXIT		IDC_BUTDEL+1

BOOL CPromptWnd::Create(char * strName,const CRect &wndRect,UINT32 nHelpId,long WaveID,WAVETYPE type)
{
	long WaveList[WAVE_MAX_COUNT];
	if(!CFrameWnd::Create("",WS_VISIBLE|WS_BORDER,wndRect,nHelpId))
		return FALSE;
	CRect r(0,0,36,18);
	r.MoveTo(m_sRect.left+2,m_sRect.bottom -20);
	if(!m_butplay.Create("播放",r,this,IDC_BUTPLAY))
		return FALSE;
	r.MoveTo(r.right+1,r.top);	
	if(!m_butdel.Create("删除",r,this,IDC_BUTDEL))
		return FALSE;
	r.MoveTo(r.right+1,r.top);	
	if(!m_butexit.Create("退出",r,this,IDC_BUTEXIT))
		return FALSE;
	int RecTotal=FindWave(type, WaveList, WAVE_MAX_COUNT);	// 查找录音，返回查到条数
	if(WaveID>RecTotal) return  FALSE;
	m_bmsgbox=false;
	m_istatus=0;
	m_RecId=WaveList[WaveID];
	WAVEHEAD* pWave=GetWavePointer(m_RecId);
	strcpy(m_stitle,pWave->Name);
	char stemp[20];
	DateToString(stemp,&pWave->Time);
	strncpy(m_sdate,pWave->Number,17);
	strcpy(m_stime,(char *)(stemp+2));
	sprintf(m_slength,"总长%d秒",(pWave->Length+8191)/8192);
	return true;
}

void CPromptWnd::OnPaint(CDC &dc)
{
	int left =m_sRect.left,top=m_sRect.top; 
	CRect r=m_sRect;
	dc.Draw3dButton(r,NULL);
	r.bottom-=20;
	r.DeflateRect(3,3);
	dc.Draw3dButton(r,NULL);
	CFrameWnd::PaintChildWnd();// OnPaint(dc);
	dc.SetBackColor(LGRAY);
	dc.TextOut(left+=4,top+=4,m_stitle);
	myline(dc,left,top+19,left+122,top+19);
	if(m_sdate[0])
	{
		dc.TextOut(left,top+=24,m_sdate);
		dc.TextOut(left,top+=20,m_stime);
		dc.TextOut(left,top+=20,m_slength);
	}
	else
	{
		dc.TextOut(left,top+=24,m_stime);
		dc.TextOut(left,top+=40,m_slength);
	}
}

void CPromptWnd::WindowProcess(UINT32 nMessage,UINT32 wParam,UINT32 lParam)
{
	switch(nMessage)
	{
	case WM_PENDOWN :
		DoPen(nMessage, wParam, lParam);
		break;
	case WM_ACTIVE:
		m_bmsgbox=false;
		break;
	}
	CFrameWnd::WindowProcess( nMessage, wParam, lParam);
}


void CPromptWnd::DoPen(UINT32 nMessage,UINT32 wParam,UINT32 lParam)
{
	CPoint pt(HIWORD(wParam), LOWORD(wParam));
	if(!pt.InRect(m_sRect))
	{
		this->Destroy();
	}
}

BEGIN_MESSAGE_MAP(CPromptWnd, CFrameWnd)
	ON_MESSAGE(IDC_BUTPLAY,	CN_COMMAND, OnPlay)
	ON_MESSAGE(IDC_BUTDEL,	CN_COMMAND, OnDel)
	ON_MESSAGE(IDC_BUTEXIT,	CN_COMMAND, OnOK)
END_MESSAGE_MAP 

void CPromptWnd::OnDel()
{
	m_bmsgbox=true;
	if(	MessageBox("确定删除吗？","删除",MB_YESNO|MB_ICONQUESTION)==IDYES)
		m_istatus=2;
	m_bmsgbox=false;
	this->Destroy();
} 

void  CPromptWnd::OnPlay()
{
	WAVEHEAD* pWave=GetWavePointer(m_RecId);
	if(bCPlayOnTop)
		pPopWndReplay->Destroy();
	if(!pPopWndReplay)
		pPopWndReplay=new CPlay;
//	sprintf(pPopWndReplay->m_stitle,"%s, %2d:%2d",pWave->Content+2,pWave->Time.hour,pWave->Time.minute) ;
	pPopWndReplay->Create(m_RecId,this);
	m_bmsgbox=true;
	if(pPopWndReplay->Process(1))
		pPopWndReplay->ShowWindow(); 
	m_istatus=1;//放音
}

void CPromptWnd::OnOK()
{

	this->Destroy();
}
/**************************************************************************************
*
*
*
*			greeting prompt
*
*
*
*
*************************************************************************************/
#include "flash.h"
#define IDC_BTGRECORD	6888
#define	IDC_BTGPLAY		6889
BEGIN_MESSAGE_MAP(CGreeting, CAppWnd)
	ON_MESSAGE(IDC_BTGPLAY,	CN_COMMAND, OnPlay)
	ON_MESSAGE(IDC_BTGRECORD,	CN_COMMAND, OnRecord)
END_MESSAGE_MAP 
#define GREETINGTIMELONG	((GREETING_DATA_LEN+8191)/8192)

bool CGreeting::FlashSaveGreeting(void)
{
	/*
	*	Get actual length
	*/
	int length = m_pGreeting->Length;	
	
	/*
	*	Cut length
	*/
	if (length > GREETING_DATA_LEN - 4)
		length = GREETING_DATA_LEN - 4;

	/*
	*	Write to flash
	*/
	for (UINT32 i = GREETINGWAVE_ROMADD; i < (GREETINGWAVE_ROMADD + GREETING_DATA_LEN); i += FLASH_SECTOR_SIZE)
		FlashEraseSector((UINT16*)i);
	return FlashWrite((UINT16 *)GREETINGWAVE_ROMADD, 
		(UINT16 *)GREETINGWAVE_RAMADD, (length/2));
}

BOOL CGreeting::OnCreate()
{
	long wavelist[WAVE_MAX_COUNT];
	if(WAVE_STORE_LEN-GetWaveUsed()<=GREETING_DATA_LEN)
		return false;
/*	int wavetotal=FindWave(WAVE_ALL, wavelist, WAVE_MAX_COUNT);	// 查找录音，返回查到条数
	if(wavetotal>0)
	{
		WAVEHEAD * pTail=GetWavePointer(wavelist[wavetotal-1]);
		long ptail=(long)pTail+pTail->Length ;					//录音数据尾指针，判断剩余内存空间 12-18
		if( ptail>=(long)GREETINGWAVE_ROMADD) return false;
	}*/
	if(!CAppWnd::OnCreate())
		return FALSE;
	CRect r(28,80,76,100);
	if(!m_oRecord.Create("录音", r, this, IDC_BTGRECORD))
		return FALSE;
	r.MoveTo (88,80);
	if(!m_oPlay.Create("试听", r, this, IDC_BTGPLAY))
		return FALSE;
	if(!m_oSlider.Create(0,GREETINGTIMELONG,1, CRect(16,52,144,77), this, 
						  CS_BIAOCHI,IDC_SLIDER))
		return FALSE;
	m_pGreeting=(WAVEHEAD *)GREETINGWAVE_ROMADD;
	if(m_pGreeting->Flag == WAVE_GREETING && 
		  m_pGreeting->Length<=GREETING_DATA_LEN && m_pGreeting->Length>0)
		memcpy((char *)GREETINGWAVE_RAMADD,(char *)GREETINGWAVE_ROMADD,m_pGreeting->Length);
	else
		memset((char *)GREETINGWAVE_RAMADD,0,GREETING_DATA_LEN);//12-18
	m_pGreeting=(WAVEHEAD *)GREETINGWAVE_RAMADD;
	m_strStatus[0]=0;
	m_iCurTime=0;
	m_bDirty=false;
	m_bSliderMove=false;
	return true;
}
void CGreeting::OnPaint( CDC &dc) 
{
	CAppWnd::OnPaint(dc); 
	dc.Reset();
	char stmp[2];
	itos(stmp,m_iCurTime);
	dc.TextOut(32,28,"长度:     秒");
	dc.TextOut(80, 28, stmp);
	dc.DrawText(CRect(12,102,150,159),m_strStatus);
	//dc.DrawText(22,122,,"按面板停止按键停止录音");
}
void CGreeting::OnRtc(UINT16 nRtcType)
{
	if(nRtcType==RTCI_SECOND && m_bSliderMove)
	{
		char stmp[2];
		itos(stmp,++m_iCurTime);
		if(m_iCurTime<=GREETINGTIMELONG && (g_bRecStatus || g_bReplayStatus))
		{
			++m_oSlider;
			HwPutString(80, 28, WHITE,BLACK, 0, (UINT8 *)stmp, 0);
		}
		else
		{
			m_strStatus[0]=0;
			m_oPlay.SetWindowText("试听");
			m_bSliderMove=false;
			m_iCurTime--;
			StopRec();
			InitPenDrv(false);
			Paint();
		}
	}
}

void CGreeting::OnOk(UINT32 nMessage, UINT32 nSrcId, UINT32 lParam)
{
	if(WAVE_STORE_LEN-GetWaveUsed() > GREETING_DATA_LEN 
		&& nSrcId==IDOK && m_bDirty)
	{
		CDC dc;
		CRect r2(30,74,112,94);
		dc.EraseRect(r2);
		dc.FrameRect(r2);
		dc.TextOut(32,76,"保存中...");
		FlashSaveGreeting();
	}
	CAppWnd::OnOk(nMessage, nSrcId, lParam) ;
}

BOOL CGreeting::OnDestroy()
{
	if(g_bReplayStatus) 
	{
		StopPlay();
		InitPenDrv(false);
	}
	if(g_bRecStatus)
		StopRec();
	return true;
}

void CGreeting::OnCancel(UINT32 nMessage, UINT32 nSrcId, UINT32 lParam)
{
	memset((void *)GREETINGWAVE_RAMADD,0,GREETING_DATA_LEN);
	CAppWnd::OnCancel(nMessage, nSrcId, lParam);
}
void CGreeting::OnPlay()
{
	if(WAVE_STORE_LEN-GetWaveUsed()<=GREETING_DATA_LEN)
	{
		strcpy(m_strStatus,"录音空间不足，无法试听！请清理录音空间");
		CDC dc;
		dc.DrawText(CRect(12,102,150,159),m_strStatus);
		return;
	}
	if(!m_pGreeting->Flag & WAVE_GREETING) return;
	if(g_bReplayStatus) 
	{
		StopPlay();
		m_strStatus[0]=0;
		m_oPlay.SetWindowText("停止");
		InitPenDrv(false);
	}
	else
	{
		strcpy(m_strStatus,"   试听中...");
		m_oPlay.SetWindowText("停止");
		m_iCurTime=0;
		m_oSlider=0;
		m_bSliderMove=true;
		RePlay( (WAVEHEAD *)GREETINGWAVE_RAMADD,true,1);
		this->Paint();
	}
}
void CGreeting::OnRecord()
{
	if(TestLowPower()||GetRecordStatus()) return;
	CDC dc;
	if(WAVE_STORE_LEN-GetWaveUsed()<=GREETING_DATA_LEN)
	{
		strcpy(m_strStatus,"录音空间不足，无法录音！请清理录音空间.");
		dc.DrawText(CRect(12,102,150,159),m_strStatus);
		return;
	}
	if(g_bReplayStatus) StopPlay();  //正在放音则停止
	if(!OpenWaveForWrite(WAVE_GREETING))
	{
		//MessageBox( "录音空间不足！","录音",MB_OK|MB_ICONINFORMATION);
		MessageTip("录音空间不足！");
		return ;	//打开失败，
	}
	strcpy(m_strStatus,"   正在录音...");
	HwPutString(80, 28, WHITE,BLACK, 0, (UINT8 *)"1 ", 0);
	HwPutString(32,102, WHITE,BLACK, 0, (UINT8 *)m_strStatus, 0);
	dc.DrawText(CRect(22,122,150,159),"按面板停止按键停止录音");
//	OpenMuteTest(false);
	iMuteTime=MUTETIME;	//ygr
	EnableMICInput();
	m_iCurTime=0;
	m_oSlider=0;
	m_bDirty=true; 
	m_bSliderMove=true;
	RecordDeviceDrver(WAVE_GREETING);	
}

CGreeting *pGreeting=NULL;
void GreetingAppRun()
{
	if(!pGreeting)
		pGreeting=new CGreeting;
	pGreeting->Create("问候语", WS_CAPTION|WS_VISIBLE|WS_OKBOX, DESK_TOP_RECT, 0); 
	pGreeting->ShowWindow(); 
}

////////////////////////////////////////////////////////////
//        autoplay application 
//
void AutoPlayNewRecord()
{
	if(!pWaveApp)
		pWaveApp = new CRePlay;
	pWaveApp->Destroy();
	pWaveApp->Create("播放留言", WAVE_ALL|WAVE_NEW); 
	bStop=false;
	pWaveApp->ShowWindow();		
	::SetCurrentApp(APPID_RECPLAY);
}

////////////////////////////////////////////////////////////
//        replay alarm sound application 
//
void ReplayAlarm(CWnd* parent)
{
	if(!pPopWndReplay)
		pPopWndReplay=new CPlay;
	if(bCPlayOnTop)
		pPopWndReplay->Destroy();
	pPopWndReplay->Create((long)GREETINGWAVE_ROMADD,parent);
	if(pPopWndReplay->Process(1))
		pPopWndReplay->ShowWindow(); 
}

/*
ring.cpp	Sound()
setup.cpp	add adjust PromptCreate
*/
/*
void RecorderInspect(void)
{
	if(g_bReplayStatus) StopPlay();  //正在放音则停止
	if(!OpenWaveForWrite(WAVE_LINE))
		return ;
	RecordDeviceDrver();
	DisableMICInput();
	for(int i=100000;i;i--)
		;
	EndRecord(0);
	if(!OpenWaveForRead((long)WAVE_STORE_ADDR))
		return;
	WAVEHEAD * pWave=GetWavePointer((long)WAVE_STORE_ADDR);
	int plus=0;
	int i=0;
	for(;i<0x20;i++)
		ReadWave();
	for (;i<pWave->Length;i++)
		plus+=ReadWave();
	if(i<=0x20) return;
	plus/=(i-0x20);
	CloseWave();
	DeleteWave((long)WAVE_STORE_ADDR);
	if(plus>0)
		g_MuteValue=plus;
}
*/

void RecorderInspectCheck(void)
{
	DisableMICInput();
	PBSEL   |= 0x40;						//init ADC_CS
	ADInitSPI();							//串口初始化
	ADInitTimer2(1);						//7843 For Record初始化
	DisablePWMOutput();						//禁止PWM向电话线上输出，使能MIC输入
	EnableFilterChannel();					//录音电源开
	DisableSpeakerOut();					//关闭功放
	ClearBackLight();						//关闭背光
	DISABLEPENIRQ();						//关闭触摸屏中断
	ADCCSEnable();							//ads7843片选中
	int val=0;
	for(int i=0;i<1000;i++)
	{
		for(int j=0;j<500;j++)
			;
		val+=(UINT8)SPITX(0xaF00);
	}
	val/=1000;
	g_bRecStatus=val;						//录音变量置真，录音开始
}
