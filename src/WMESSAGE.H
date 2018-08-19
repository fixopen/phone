/******************************************************************************/
/*                                                                            */
/*  版权所有(c)  2001汉王科技                                                 */
/*                                                                            */
/******************************************************************************/
/*  文 件 名                                                                  */
/*             wmessage.h                                                     */
/*                                                                            */
/*  版 本 号                                                                  */
/*             1.0                                                            */
/*                                                                            */
/*  描    述                                                                  */
/*             汉王科技E-PhoneII窗口类库的头文件                              */
/*             HanWang Technology E-PhoneII Function Class Library            */
/*                                                                            */
/*  组    成                                                                  */
/*             窗口及系统消息的宏定义                                         */
/*             CFrameWnd消息映射表相关的结构和宏定义                          */
/*                                                                            */
/*  设计说明                                                                  */
/*             无                                                             */
/*                                                                            */
/*  作    者                                                                  */
/*             王飞                                                           */
/*                                                                            */
/*  日    期                                                                  */
/*             2001-06-15                                                     */
/*                                                                            */
/*  历史记录        
/*			   2001-09-07
/*             增加WM_CLOSE消息(郝庆丰)                                               */
/******************************************************************************/

#ifndef __E201_WMESSAGE_H_
#define __E201_WMESSAGE_H_

#include "hwfcl.h"

/*============================================================================*/
//  窗口及系统消息                                                            //
/*============================================================================*/
#define WM_MESSAGE     0

////////////////////////////////////////////////////////////////////////////////
//描述:
//         笔按下消息
//说明:
//         wParam            包含当前笔坐标
//            HIWORD(wParam)  横坐标x
//            LOWORD(wParam)  纵坐标y
//         lParam            无定义 1
#define WM_PENDOWN     (WM_MESSAGE    + 1)

////////////////////////////////////////////////////////////////////////////////
//描述:
//           笔移动消息
//说明:
//         wParam            包含当前笔坐标
//            HIWORD(wParam)  横坐标x
//            LOWORD(wParam)  纵坐标y
//         lParam            无定义 2
#define WM_PENMOVE     (WM_PENDOWN    + 1)

////////////////////////////////////////////////////////////////////////////////
//描述:
//         笔抬起消息
//说明:
//         wParam            无定义
//         lParam            无定义 3
#define WM_PENUP       (WM_PENMOVE    + 1)

////////////////////////////////////////////////////////////////////////////////
//描述:
//         笔移动入某个窗口或控件
//说明:
//         wParam            包含当前笔坐标
//            HIWORD(wParam)  横坐标x
//            LOWORD(wParam)  纵坐标y
//         lParam            无定义 4
#define WM_PENMOVEIN   (WM_PENUP      + 1)

////////////////////////////////////////////////////////////////////////////////
//描述:
//         笔移出某个窗口或控件
//说明:
//         wParam            包含当前笔坐标
//            HIWORD(wParam)  横坐标x
//            LOWORD(wParam)  纵坐标y
//         lParam            无定义 5
#define WM_PENMOVEOUT  (WM_PENMOVEIN  + 1)

////////////////////////////////////////////////////////////////////////////////
//描述:
//         按键消息
//说明:
//         wParam             包含当前键值
//         lParam             无定义 6
#define WM_KEY         (WM_PENMOVEOUT + 1)

////////////////////////////////////////////////////////////////////////////////
//描述:
//         按键抬起消息
//说明:
//         wParam             抬起的键值
//         lParam             无定义 7
#define WM_KEYUP       (WM_KEY        + 1) 

////////////////////////////////////////////////////////////////////////////////
//描述:
//         按键保持消息
//说明:
//         wParam             键值
//         lParam             无定义 8
#define WM_KEYKEEP     (WM_KEYUP      +1)

////////////////////////////////////////////////////////////////////////////////
//描述:
//         窗口重绘消息
//说明:
//         wParam             无定义
//         lParam             无定义 9
#define WM_PAINT       (WM_KEYKEEP    + 1)

////////////////////////////////////////////////////////////////////////////////
//描述:
//         窗口或控件获得焦点消息
//说明:
//         wParam             失去焦点的窗口或控件指针
//         lParam             无定义 10
#define WM_ACTIVE      (WM_PAINT      + 1)

////////////////////////////////////////////////////////////////////////////////
//描述:
//         窗口或控件失去焦点消息
//说明:
//         wParam             将要获得焦点的窗口或控件指针
//         lParam             无定义 11
#define WM_DACTIVE     (WM_ACTIVE    + 1)

////////////////////////////////////////////////////////////////////////////////
//描述:
//         窗口或控件失去焦点消息
//说明:
//         wParam             
//         lParam             12
#define WM_TIMER       (WM_DACTIVE +1)

////////////////////////////////////////////////////////////////////////////////
//描述:
//         RTC消息
//说明:
//         wParam       RTC类型 参见datetime.h            
//         lParam             13
#define WM_RTC         (WM_TIMER +1)     

////////////////////////////////////////////////////////////////////////////////
//描述:
//         窗口关闭 消息
//说明:
//         wParam            
//         lParam             14
#define WM_CLOSE        (WM_RTC+1)   

////////////////////////////////////////////////////////////////////////////////
//描述:
//         手写识别结束（但候选字尚未显示）消息
//说明:
//         wParam                        
//         lParam             14
#define WM_WRITEREG_OVER  (WM_CLOSE + 1)

#define WM_EXIT_SCRPROT   (WM_WRITEREG_OVER + 1)
/*============================================================================*/
//  电话相关的消息                                                            //
/*============================================================================*/
#define WM_PHONE       500

////////////////////////////////////////////////////////////////////////////////
#define WM_PICKUP      (WM_PHONE     + 1)    //手柄摘机
#define WM_PICKDOWN    (WM_PICKUP    + 1)    //手柄挂机
#define WM_MICOPEN     (WM_PICKDOWN  + 1)    //免提摘机 
#define WM_MICCLOSE    (WM_MICOPEN   + 1)    //免提挂机 
#define WM_RING        (WM_MICCLOSE  + 1)    //振铃 wParam 指向CallId的字符串指针
#define WM_RINGEND     (WM_RING      + 1)    //振铃结束 
#define WM_DIAL        (WM_RINGEND   + 1)    //拨号,当话机处于摘机状态时的按键, wParam的最低8bit为拨的号码
#define WM_REDIAL      (WM_DIAL      + 1)    //重拨 
#define WM_MUTESTART   (WM_REDIAL    + 1)    //静音开始
#define WM_MUTEEND     (WM_MUTESTART + 1)    //静音结束
#define WM_FLASH       (WM_MUTEEND   + 1)    //闪断
#define WM_SOFTDIAL    (WM_FLASH     + 1)    //软件拨号消息 wParam为指向号码字符串的指针
#define WM_TALKSTART   (WM_SOFTDIAL  + 1)

/*============================================================================*/
//  录音相关的消息                                                            //
/*============================================================================*/
#define	RECORD_AUTO	0
#define	RECORD_LINE	1
#define	RECORD_MIC	2

#define	WM_RECORD_START	    600				// 录音开始, wParam = RECORD_TYPE_AUTO、RECORD_TYPE_LINE、RECORD_TYPE_MIC
#define	WM_RECORD_END		601				// 录音结束，wParam = RECORD_TYPE_AUTO、RECORD_TYPE_LINE、RECORD_TYPE_MIC
#define WM_AUTOREC_START    602             // 自动留言开始 wParam = 0 , lParam = 0

#define WM_REPLAY_END		620				// 放音结束消息

#define WM_SOUND_CLOSE      650				// 振铃放音结束
//按键抬起 
#define WM_ALARM       (WM_TALKSTART + 1)   //提醒消息

////////////////////////////////////////////////////////////////////////////////

/*============================================================================*/
//  控件发给父窗口的通知消息                                                  //
/*============================================================================*/
#define WM_NOTIFY      1000

////////////////////////////////////////////////////////////////////////////////
//描述:
//         命令通知
//说明:
//         wParam             控件的ID
//         lParam             据不同的控件有不同定义
#define CN_COMMAND     (WM_NOTIFY     + 1)

////////////////////////////////////////////////////////////////////////////////
//描述:
//         滚动条滚动消息通知
//说明:
//         wParam             控件的ID
//         lParam             
//            HIWORD(lParam)       总行数
//            LOWORD(lParam)       滚动到的行数
#define CN_SCROLLTO    (CN_COMMAND    + 1)

////////////////////////////////////////////////////////////////////////////////
//描述:
//         列表控件选中消息
//说明:
//         wParam             控件的ID
//         lParam             选中的行号
#define CN_LISTSEL     (CN_SCROLLTO   + 1)

////////////////////////////////////////////////////////////////////////////////
//描述:
//         组合框控件弹出消息
//说明:
//         wParam             控件ID
//         lParam             无定义
#define CN_DROPDOWN    (CN_LISTSEL    + 1)

////////////////////////////////////////////////////////////////////////////////
//描述:
//         组合框控件收回弹出消息
//说明:
//         wParam             控件的ID
//         lParam             无定义
#define CN_UNDROPDOWN  (CN_DROPDOWN   + 1)

////////////////////////////////////////////////////////////////////////////////
//描述:
//         菜单控件隐藏消息
//说明:
//         wParam             控件的ID
//         lParam             无定义
#define CN_MENUHIDE    (CN_UNDROPDOWN + 1)

////////////////////////////////////////////////////////////////////////////////
//描述:
//         手写控件选候选字消息
//说明:
//         wParam             控件的ID
//         lParam             候选
#define CM_WRITEPAD_WRITE  (CN_MENUHIDE +1)   


////////////////////////////////////////////////////////////////////////////////
//描述:
//         手写控件识别结束消息
//说明:
//         wParam             控件的ID
//         lParam             识别候选字 buffer 指针
#define CM_WRITEPAD_RECOG  (CM_WRITEPAD_WRITE +1)   

////////////////////////////////////////////////////////////////////////////////
//描述:
//         手写控件候选字回显消息（由Edit控件发出）
//说明:
//         wParam             控件的ID
//         lParam             识别候选字 buffer 指针
#define CM_WRITEPAD_SHOWREC  (CM_WRITEPAD_RECOG +1)   

////////////////////////////////////////////////////////////////////////////////
//描述:
//         网格控件单元格点击消息(含有组合框)
//说明:
//         wParam             控件的ID
//         lParam             低字是行数（1表示第一行!!!)高字是列数（1表示第一列!!!)
#define CN_GRIDDROPDOWN    (CM_WRITEPAD_SHOWREC+1)
////////////////////////////////////////////////////////////////////////////////
//描述:
//         网格控件单元格点击消息(一般网格)
//说明:
//         wParam             控件的ID
//         lParam             低字是行数（1表示第一行!!!)高字是列数（1表示第一列!!!)
#define CN_GRIDSEL         (CN_GRIDDROPDOWN+1)

////////////////////////////////////////////////////////////////////////////////
//描述:
//         进度条点击消息(一般网格)
//说明:
//         wParam             控件的ID
//         lParam             进度值
#define  CS_PROCESS        (CN_GRIDSEL+1)

////////////////////////////////////////////////////////////////////////////////
//描述:
//         免提情况下手柄摘机(用于停止留言)
//说明:
//         wParam             0
//         lParam             0
#define  WM_SPEAKINGPICKUP  (CS_PROCESS+1)

////////////////////////////////////////////////////////////////////////////////
//描述:
//         短消息模式下按免提(用于终止短消息)
//说明:
//         wParam             0
//         lParam             0
#define  WM_QUITSMS  (WM_SPEAKINGPICKUP+1)

#define WM_USER      5000

/*============================================================================*/
//  CFrameWnd消息映射表相关的结构和宏定义                                     //
/*============================================================================*/
struct MESSAGE_MAP
{
	UINT32  id1;
	UINT32  id2;
	UINT32  message;
	void   (CWnd::*memberFxn)(UINT32 message, UINT32 wParam, UINT32 lParam);
};

struct MESSAGE_ENTRY
{
	MESSAGE_MAP   * messageMap;
	MESSAGE_ENTRY * parentMessageEntry;
};

#define DECLARE_MESSAGE_MAP(classname)      static MESSAGE_MAP   stMessageMap[]; \
	                                        static MESSAGE_ENTRY stMessageEntry; \
											virtual MESSAGE_ENTRY *GetMessageEntry() \
											{ return &stMessageEntry;}


#define   BEGIN_MESSAGE_MAP(classname, baseclass)   \
	      MESSAGE_ENTRY classname :: stMessageEntry = {stMessageMap, &baseclass::stMessageEntry}; \
          MESSAGE_MAP classname :: stMessageMap[] = {

#define   END_MESSAGE_MAP     {0,0,0,NULL}};

#define  ON_MESSAGE_RANGE(id1, id2, message, memberFxn)   {id1, id2, message, (void (CWnd::*)(UINT32, UINT32, UINT32))&memberFxn},

#define  ON_MESSAGE(id, message, memberFxn)  {id, id, message, (void (CWnd::*)(UINT32, UINT32, UINT32))&memberFxn},

#define  MESSAGE_HANDLE

#endif 
//end wmessage.h
////////////////////////////////////////////////////////////////////////////////
