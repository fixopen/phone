/*************************************************************************/
/*                                                                       */
/*                       版权所有(c) 2001汉王科技                        */
/*                                                                       */
/*************************************************************************/
  
/*************************************************************************/
/*                                                                       */
/* 文件名                                               版本             */
/*                                                                       */
/*      pendrv.h                                        PRELIMINARY      */
/*                                                      VERSION 1.0      */
/* 描述                                                 2001年7月7日     */
/*                                                                       */
/*      触摸屏驱动程序头文件                                             */
/*                                                                       */
/* 作者                                                                  */
/*                                                                       */
/*      白亚伟                                                           */
/*                                                                       */
/* 数据结构                                                              */
/*                                                                       */
/*      PENMESSAGE                              笔消息                   */
/*                                                                       */
/* 函数                                                                  */
/*                                                                       */
/*      InitPenDrv - - - - - - - - - - - - - 初始化触摸屏驱动            */
/*      CalibrationPen - - - - - - - - - - - - - - 触摸屏校准            */
/*      EnablePenMessage - - - - - - - - - - - -允许发送触摸屏消息       */
/*      DisablePenMessage - - - - - - - - - - - 禁止发送触摸屏消息       */
/*      SetTouchPaneSampleRate  - - - - - - - - 设置触摸屏采样速率       */
/*                                                                       */
/* 历史记录                                                              */
/*                                                                       */
/*      2001/9/29, byw                                                   */
/*		增加 EnablePenMessage() 和 DisablePenMessage()                   */
/*                                                                       */
/*************************************************************************/


#ifndef __E201_PENDRV_H_
#define __E201_PENDRV_H_

#include "dc.h"
//-------------------------------- Constants -------------------------------------
#ifdef   CIRCUMGYRATE
#define	COORDINATE_ROTATION			// 旋转坐标系
#endif

//-------------------------------- Prototypes -------------------------------------

void InitPenDrv(bool bInitSystem=true);				// 初始化触摸屏
void EnablePenMessage(void);		// 允许发送触摸屏消息
void DisablePenMessage(void);		// 禁止发送触摸屏消息
void CalibrationPen(void);			// 校准触摸屏
void SetTouchPaneSampleRate(int frequency = 48);	
									// 设置触摸屏采样速率
									//
									// frequency	采样速率(点/秒)
									//				最小值：5
									//				最大值：500
	
#endif
