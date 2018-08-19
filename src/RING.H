/************************************************************************
                                                                       
 文件名                                                 版本             
                                                                       
      ring.h                                          PRELIMINARY      
                                                      VERSION 1.0      
 描述                                                 2001年8月22日    
                                                                       
      铃声驱动程序头文件                                               
                                                                       
 作者                                                                  
                                                                       
      余国荣                                                           
                                                                       
 数据结构                                                              
    铃声乐谱：                                                                   
	MusicFile 
                                                                       
 函数	
 


	1.int sndPlayMusic(short iRingID=SYSTEM_RING,bool NonBreakMode=true,int PlayTimes=1,int Stringendo=0);
		参数:																	
			MusicFile * Music	音乐数据头及数据						
			int PlayTimes		可选参数，循环放音次数，默认为一次		
			int Stringendo		可选参数，允许渐快，默认为不渐快		
		返回值：																	
			-1	当前PWM不空闲												
			其它为创建放音任务返回值										
	2.void StopRing();		//结束放音								
	3.int  intiRinger();		//暂停放音									

 关联                                                                  
                                                                       
                                                                       
 历史记录          
	2001-9-17   添加打断前次放音模式
	2001-11-22  append GetRingStatus()
	2001-12-29	加入短消息特色振铃

************************************************************************/
#ifndef _E201_PWM2_H
#define _E201_PWM2_H

#include "hwfcl.h"


#define RING_DATA_ADDR     0x400000
#define SYSTEM_RING		   0 
#define	SUCCESS			   0
#define FAILCREATETIMER	  -1
#define PWMINUSE		  -2
#define VALIDPLAYTIMES	  -3

/*
sms save return value
*/
#define SAVE_SUCESSE	 0
#define RING_MEM_FULL	-1		//铃声数据已满
#define ERR_SYSTEMMEM	-2		//内存错误
#define ERR_DATA		-3		//数据错或有不支持格式的数据，如特殊频率
#define ERR_FLASH		-4		//擦写Flash错误

int SMS_SAVE_RING(BYTE * ringBuf);		//保存特色振铃

extern int intiRinger();
extern int sndPlayMusic(short iRingID=SYSTEM_RING,bool NonBreakMode=true,int PlayTimes=1,int Stringendo=0);
extern void StopRing(bool bActiveHisr=false);
extern bool GetRingStatus();

#endif

