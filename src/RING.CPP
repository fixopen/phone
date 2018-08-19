/******************************************************************************
                                                                             
    版权所有(c)  2001汉王科技                                                 
                                                                            
/**********************************************************************************************
  文 件 名                                                                  
             PWMSound.cpp                                                   
                                                                            
  版 本 号                                                                  
             1.0                                                            
                                                                            
  描    述                                                                  
             汉王科技E-PhoneII 铃声音乐播放驱动程序的源文件                              
                                                                            
  组    成                                                                  
             铃声驱动程序源文件                                             
                                                                            
  作    者                                                                  
             余国荣                                                           
                                                                            
  日    期                                                                  
             2001-09-11                                                     
  函数													
	1.int sndPlayMusic(short iRingID,bool NonBreakMode,int PlayTimes,int Stringendo)
		参数:																	
			short iRingID		音乐数据ID						
			int PlayTimes		可选参数，循环放音次数，默认为一次		
			bool NonBreakMode   非打断模式。即不打断正在进行放音
			int Stringendo		可选参数，允许渐快，默认为不渐快		
		返回值：																	
			SUCCESS			 0
			FAILCREATETIMER	-1
			PWMINUSE		-2
			VALIDPLAYTIMES	-3
			其它为创建放音任务返回值										
	2.void 		StopRing(bool bActiveHisr=false)		//结束放音								
 
	1=全音符   2=二分音符  4=四分音符  8=八分音符  16=十六分音符   32=三十二分音符
    附点音符则需要常按住该音的按键超过三秒.就可以变成附点音符的拍数了.
    Ps.铃声谱中若有出现第一个数字与第二个字母中间出现黑点的音.那就是附点的拍子
  
	第二个英文字.表示不同的音...
    c=Do d=Re e=Mi f=Fa g=So a=La b=Si -(负号)=休止符
	播放的铃声可以调整速度.
    40.45.50.56.63.70.80.90.100.112.125.140.160.180.200.225.
    这几种节拍.我的铃声谱里也会附上建议速度.
    当然.这只是建议用的.可以随自己喜好来调整铃声.
                                                                            
  历史记录                                                                  
			2001-9-17   添加打断前次放音模式
			2001-10-9	改用timer放音，添加数据驱动
	   yrg, 2001/12/3, modify StopRing()
	        2001-12-29	加入短消息特色振铃，改了好多地方
*************************************************************************************************/

#include "hwfcl.h"
#include <string.h>
#include "keydrv.h"
#include "datadrv.h"
#include "ring.H"

#define PWMC2		(* ((UINT16 *) 0XFFFFF510)) 
#define PWMP2		(* ((UINT16 *) 0XFFFFF512))
#define PWMW2		(* ((UINT16 *) 0XFFFFF514))
#define PWMCNT2		(* ((UINT8  *) 0xFFFFF516))
#define IMR			(* ((UINT32 *) 0xFFFFF304))
#define ISR			(* ((UINT32 *) 0xFFFFF30C))
#define ILCR		(* ((UINT16 *) 0xFFFFF314))
#define PKSEL		(* ((UINT8  *) 0xFFFFF443))
#define PKDIR		(* ((UINT8  *) 0xFFFFF440))
#define PKPUL		(* ((UINT8  *) 0xFFFFF442))
#define PCR				(* ((UINT8  *) 0XFFFFF003))

//所耍的全局变量

int g_bMusicStaus=0;			//PWM状态，为1 表示当前正在使用。
  
//#define NoSound() 	{PWMC2&=0XFFEF;	 } //放完，则关闭中断，禁能PWM2


static void PWM2Init(void)
{
	PCR=(PCR&0XE3)|0X08;
	PKDIR|=0X01;			//设置PK0为专用引脚，上拉，I/O方向为输出
	PKPUL|=0X01;
	PKSEL&=0XFE;
	PWMP2=0X00FF;			//设置PWM Period 为256.
	PWMC2=0X0000;			//设置PWM默认输出为高，正极性，时钟频率为SYSCLK/4 屏蔽PWM2中断，禁能PWM2
	IMR  |=~(0XFFFFDFFF);	//关闭IMR（中断屏蔽寄存器）里PWM2的中断，
}

void Sound(UINT16 Frequence)
{
	EnablePWMOutput();
	EnableSpeakerOut();					    //功放
	if (Frequence)
	{	PWMP2=8290304/Frequence;			//设置PWM Period 调整频率
		PWMW2=PWMP2>>6;					    //设置PWM 的脉宽
		PWMC2|=0X0010;						//enable pwm2	//使能PWM2，
	}
	else
		NoSound();
}

/******************new ring**************************
*****************************************************/
extern short AppendRing(RING *pRing);	// 返回 ID(ID>=0)，返回 -1 表示失败
extern bool DeleteRing(short iRingID);	// 成功返回true, 失败返回false
extern bool ModifyRing(short iRingID, RING const *pRing); // 成功返回true, 失败返回false
extern int FindRing(short *pRingIDBuf, int iBufLen); // 返回查到个数
extern RING* GetRingPointer(short iRingID); // 返回RING结构指针(只读)，返回NULL表示失败

class CRing			//容含放音过程中的部份外部变量。
{
public:
	int TotalTimes;
	int iStringendo;
	//允许渐快
	int SoundLong;
	int g_iSoundSpeed;			//每全音符的时间,单位为5ms;	乐谱节拍速度有下面几种40.45.50.56.63.70.80.90.100.112.125.140.160.180.200.225.
	UINT8 * pRingElement;
	RING *pRing;
public:
	CRing();
	~CRing();
};

CRing::CRing()
{
	TotalTimes=0;
	SoundLong =0;
}
CRing::~CRing()
{	
}

NU_TIMER  RingTimer;
CRing *pCRing=NULL;

//const int FrequencyTable[]={0,262,294,330,349,392,440,494, 277,311,349,370,415,466,523};	//#	//FrqTable[0]为基本音，下面一行为半音
const int FrequencyTable[]={0,131,147,165,175,196,220,247, 139,156,175,185,208,233,261};	//#	//FrqTable[0]为基本音，下面一行为半音
					  //    -, 1,  2, 3,  4,	 5,	 6,	 7,   #1, #2, #3, #4, #5, #6, #7
const int OtherFreqTable[]={0,647,809,222,266,363,400,500,667,800,1000,1333,2000};

/*****************************
 
*****************************/
int TransLong(UINT8 sl)
{
	if(pCRing->pRing->Flag & RING_SMS)
		return sl<<1;	//特色振铃第二字节为单位为10ms的时间长度
	register int k=sl&0x7f;
	if(!k) return 0;
	if (sl&0x80)
	{
		k=pCRing->g_iSoundSpeed*3/(2*k);
		return (k);
	}
	else
	{
		k=pCRing->g_iSoundSpeed/k;
		return (k);
	}

}
extern  bool GetReplayStatus();
void StopRing(bool bActiveHisr)
{
	if(!GetRingStatus())
		return;
	g_bMusicStaus=0;			//PWM状态，为1 表示当前正在使用。
	NU_Control_Timer(&RingTimer,NU_DISABLE_TIMER);
	NoSound();
	if(!GetReplayStatus()) //12-18
	{
		DisablePWMOutput();  
		DisableSpeakerOut();
	}
	PostSystemMessage(WM_SOUND_CLOSE,0,(UINT32)pCRing->pRing,0,bActiveHisr);//ygr
	delete 	pCRing;
	pCRing = NULL;
}

static bool bContinuSound=false;
static int	nPreSoundLong=0;
static BYTE nPreFrequency=0;

void Ring_timer(unsigned long STime)
{
	UINT8  bFrequency;
	if(*pCRing->pRingElement==0xFF)		//放音到尾
	{
		if(!(--pCRing->TotalTimes))		//放音次数够
		{
			StopRing(true);
			return;
		}
		else
		{
			bContinuSound=false;
			nPreSoundLong=0;
			nPreFrequency=0;
			pCRing->pRingElement=(UINT8 *)pCRing->pRing->Content ;
			if(pCRing->iStringendo)
				if(pCRing->g_iSoundSpeed>40) 
					pCRing->g_iSoundSpeed=pCRing->g_iSoundSpeed-48 ;

		}
	}
	if(!bContinuSound)
	{
		pCRing->SoundLong=TransLong(*pCRing->pRingElement++);
		bFrequency=*pCRing->pRingElement++;
	}
	else
	{
		pCRing->SoundLong=nPreSoundLong-4;
		if(pCRing->SoundLong <0) pCRing->SoundLong=0;
		bFrequency=nPreFrequency;
	}
	if(bFrequency&0x80)//特殊频率
		Sound(OtherFreqTable[bFrequency&0x7f]);
	else
	{
		if(bFrequency==*(pCRing->pRingElement-3))
		{
			if(!bContinuSound)
			{
			nPreSoundLong=pCRing->SoundLong;
			nPreFrequency=bFrequency;
			bContinuSound=true;
			bFrequency=0;
			pCRing->SoundLong =4;
			}
			else
				bContinuSound=false;
		}
		Sound(FrequencyTable[bFrequency&0XF] *(1<<((bFrequency)>>4)) );
	}	
	STATUS status;
	NU_Control_Timer(&RingTimer,NU_DISABLE_TIMER);
	if((status = NU_Reset_Timer(&RingTimer,Ring_timer, pCRing->SoundLong,1, NU_ENABLE_TIMER))!=NU_SUCCESS )
		StopRing(true);
}

int sndPlayMusic(short iRingID,bool NonBreakMode,int PlayTimes,int Stringendo)
{
	if (iRingID <0)
		return -3;
	if(PlayTimes==0) return VALIDPLAYTIMES;
	if(NonBreakMode)
		if(g_bMusicStaus) return PWMINUSE;
	NU_Control_Timer(&RingTimer,NU_DISABLE_TIMER);
	if(pCRing==NULL)
		pCRing=new CRing;
	if (pCRing == NULL)
		return -1;
	if((pCRing->pRing=GetRingPointer(iRingID)) == NULL)
		return -3;
	EnablePWMOutput();
	EnableSpeakerOut();
	bContinuSound=false;
	nPreSoundLong=0;
	nPreFrequency=0;
	pCRing->TotalTimes=PlayTimes;
	pCRing->iStringendo =Stringendo;
	pCRing->pRingElement=(UINT8 *)pCRing->pRing->Content; 
	STATUS status;    
	int is=(12000*pCRing->pRing->Rhythm);		
	pCRing->g_iSoundSpeed=is/pCRing->pRing->Tempo ;		//pCRing->g_iSoundSpeed 每全音符所用时间,单位为5ms;
	if((status = NU_Reset_Timer(&RingTimer,Ring_timer, 1,1, NU_ENABLE_TIMER))!=NU_SUCCESS )
			return FAILCREATETIMER;
	g_bMusicStaus=1;			//PWM状态，为1 表示当前正在使用。
	return SUCCESS;
}


int initRinger()
{
	STATUS status;
	PWM2Init();
	NU_Control_Timer(&RingTimer,NU_DISABLE_TIMER);
	NU_Delete_Timer(&RingTimer);
	if((status = NU_Create_Timer(&RingTimer,"Ring_timer",Ring_timer, 0, 1,1, NU_DISABLE_TIMER))!=NU_SUCCESS )
		return FAILCREATETIMER;
	pCRing=NULL;
	return 0;
}
bool GetRingStatus()
{
	return g_bMusicStaus;
}

/**********************************************
	save & trans sms ring 
**********************************************/
#define SIGN_SEPERATE 0xff


BYTE TransYinFu(UINT8 SMS_Data)
{
	const BYTE MusicTable[]={1,1+7,2,2+7,3,4,4+7,5,5+7,6,6+7,7};
	if(SMS_Data<0x80 && SMS_Data>0)		//特珠频率
		return SMS_Data|0x80;
	if(SMS_Data==0 )				    //休止符
		return 0;
	BYTE LowData=SMS_Data&0xf;
	if(SMS_Data>0xBB || (LowData)>0xB ) return 0xff;
	BYTE E201_Data=MusicTable[LowData];
	E201_Data |=((SMS_Data>>4)-7)<<4;		//音阶
	return E201_Data;
}

void TransSMStoE201(BYTE * SmsRingBuf,RING * pRing)
{
	BYTE *  pSMSRingTmp=SmsRingBuf;
	int lenth=*pSMSRingTmp++;
	if(lenth>241)//20 name,1 seperate,220 data
	{
		pRing->Flag =0xff;
		return;
	}
	pRing->Flag =RING_SMS;
	pRing->Flat=0;
	pRing->Rhythm =120;
	pRing->Tempo=4;
	int conter=0;
	//save name
	while( (pSMSRingTmp[conter])!=SIGN_SEPERATE && conter<RING_NAME_LEN)
	{
		pRing->Name[conter]=pSMSRingTmp[conter];
		conter++;
	}
	pRing->Name[conter]=0;
	while(pSMSRingTmp[conter++]!=SIGN_SEPERATE )
		;
	//pick element
	int dataconter=0;
	while(conter<=lenth)
	{
		UINT8 * pYingfu=(UINT8 *)(pRing->Content+dataconter);
		*++pYingfu=TransYinFu(pSMSRingTmp[conter++]);
		if(*pYingfu==0xff)
		{
			pRing->Flag=0xff;
			return;
		}
		*(pYingfu-1)=pSMSRingTmp[conter++];		//sound long
		dataconter++;
	}
}
int SMS_SAVE_RING(BYTE * ringBuf)
{
	if(!ringBuf)
		return ERR_DATA;
	short RingIDBuf[RING_MAX_COUNT];
	int total=0;
	if((total=FindRing(RingIDBuf, RING_MAX_COUNT))>=RING_MAX_COUNT)
		return RING_MEM_FULL;
	RING  *	pRing=NULL;
	pRing=new RING;
	if(!pRing ) 
		return ERR_SYSTEMMEM;
	TransSMStoE201(ringBuf, pRing);
	if(pRing->Flag!=RING_SMS)
	{
		delete pRing;
		return ERR_DATA;
	}
	if( AppendRing(pRing)==-1)
	{
		delete pRing;
		return ERR_FLASH;
	}
	delete pRing;
	return SAVE_SUCESSE;
}

/*
高字节音频定义

  | Bit7 | Bit6 | Bit5 | Bit4 | Bit3 | Bit2 | Bit1 | Bit0 |
  |      |                    |      |                    |   
  |  a   |         b          |   c  |          d         |   

  a		1:特殊频率格式  0:音符格式
  b		0-7 音阶
  c		1:升半音		0:常音
  d     0-7 音符

*/