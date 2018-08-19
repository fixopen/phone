/******************************************************************************/
/*                                                                            */
/*  版权所有(c)  2001汉王科技                                                 */
/*                                                                            */
/******************************************************************************/
/*  文 件 名                                                                  */
/*             hwfcl.h                                                        */
/*                                                                            */
/*  版 本 号                                                                  */
/*             1.0                                                            */
/*                                                                            */
/*  描    述                                                                  */
/*             汉王科技E-PhoneII窗口类库的头文件                              */
/*             HanWang Technology E-PhoneII Function Class Library head file  */
/*                                                                            */
/*  组    成                                                                  */
/*             所有窗口类、控件类及用于显示输出类的声明和类库中的派生结构     */
/*             HWFCL中需要用到的常量及宏定义                                  */
/*             HWFCL中的全局函数声明                                          */
/*             显示输出相关类的声明                                           */
/*             时间日期相关类的声明                                           */
/*             其它一些杂项类的声明                                           */
/*             所有窗口类的声明及派生结构                                     */
/*             所有控件类的声明及派生结构                                     */
/*                                                                            */
/*  设计说明                                                                  */
/*             此窗口类库适用于嵌入实时操作系统Nucleus，建立在Nucleus Kennel  */
/*             和LCD显示驱动的基础上                                          */
/*                                                                            */
/*  作    者                                                                  */
/*             王飞                                                           */
/*                                                                            */
/*  日    期                                                                  */
/*             2001-06-15                                                     */
/*                                                                            */
/*  历史记录                                                                  */
/******************************************************************************/
#ifndef __E201_HWFCL_H_
#define __E201_HWFCL_H_
#include "nucleus.h"
#include <string.h>
#include <math.h>
#include <stdio.h>

/*
*	Debug relative
*/
//#define   DEBUG              //调试版本标记
void DebugInfo(char *Info);

/*============================================================================*/
//  数据类型定义                                                              //
/*============================================================================*/
typedef long                 INT32;
typedef short                INT16;
typedef char                 INT8;

typedef unsigned long        UINT32;
typedef unsigned short       UINT16;
typedef unsigned char        UINT8;

typedef long  *              PINT32;
typedef short *              PINT16;
typedef char  *              PINT8;

typedef unsigned long   *    PUINT32;
typedef unsigned short  *    PUINT16;
typedef unsigned char   *    PUINT8;

typedef UINT16               BOOL;
typedef UINT16               WORD;
typedef UINT8                BYTE;

/*============================================================================*/
//  常量及宏定义                                                              //
/*============================================================================*/
#define FALSE                 0
#define TRUE                  1
#define NULL                  0

#define MAX(a,b)	          (((a) > (b)) ? (a) : (b))
#define MIN(a,b)	          (((a) < (b)) ? (a) : (b))
#define ABS(x)                (((x)>0) ? (x) : -(x))

#define LOBYTE(w)	          ((UINT8)(w))
#define HIBYTE(w)	          ((UINT8)((UINT32)(w) >> 8))
#define MAKEWORD(low, high)   (((UINT8)(low)) | (((UINT16)(high)) << 8))

#define LOWORD(l)	          ((UINT16)(l))
#define HIWORD(l)	          ((UINT16)((UINT32)(l) >> 16))
#define MAKELONG(low, high)   (((UINT16)(low)) | (((UINT32)(high)) << 16))

#define DESK_TOP_RECT         (CRect(0,0,159,159))
#define PHONE_WND_RECT        (CRect(3,18, 156, 136))

/*============================================================================*/
//  窗口相关常量及宏定义                                                      //
/*============================================================================*/

////////////////////////////////////////////////////////////////////////////////
//窗口标题可容纳字符数
#define WND_TITLE_LENGTH      128

////////////////////////////////////////////////////////////////////////////////
//窗口标题栏的高度
#define WND_TITLE_HEIGHT        18
#define WND_TOOLBAR_HEIGHT      18

////////////////////////////////////////////////////////////////////////////////
//窗口标题按钮的宽度(两个汉字)
#define    BTN_WIDTH             34
////////////////////////////////////////////////////////////////////////////////
//窗口标题的宽度
#define    TITLE_WIDTH           75

////////////////////////////////////////////////////////////////////////////////
//窗口风格
#define WS_BORDER             0x00000001        //边框  
#define WS_CHILD              0x00000002        //子窗口
#define WS_DISABLE            0x00000004        //是否接收处理消息
#define WS_VISIBLE            0x00000008        //是否可见
#define WS_TRANSPARENT        0x00000010        //是否透明
#define WS_CAPTION            0x00000020        //是否有标题
#define WS_CONTROLBOX         0x00000040        //是否有关闭按钮
#define WS_OKBOX              0x00000080        //是否有确定按钮

#define WS_NORMAL             (WS_BORDER | WS_VISIBLE | WS_CAPTION | WS_CONTROLBOX)    ///缺省窗口风格

////////////////////////////////////////////////////////////////////////////////
//隐藏及显示窗口参数常量
#define SW_HIDE               0
#define SW_SHOW               1

/*============================================================================*/
//  点，矩形类                                                                //
/*============================================================================*/
class CPoint;
class CRect;
#include "pointrect.h"

/*============================================================================*/
//  显示输出类CDC                                                             //   
/*============================================================================*/
class CDC;
#include "dc.h"
/*============================================================================*/
//  时间日期相关类                                                            //
/*============================================================================*/
class CDate;
class CTime;

/*============================================================================*/
//  窗口及控件类                                                              //
/*============================================================================*/
class CObject;
      class CWnd;
	        class CFrameWnd;
			      class CDeskTop;
				  class CAppWnd;
				  class CDialog;
				        class CInputBox;
						class CMessageBox;
	        class CCtrl;
			      class CStatic;
			      class CButton;
				        class CIconButton;
						class CCheckButton;

				  class CLineEdit;
				  class CMenu;
				  class CListView;
				  class CCombox;
				  class CScrollBar;

				  class CEdit;
				  class CWritePad;
				  class CGrid;
				  class CAniCtrl;
				  class CSoftKey;


/*============================================================================*/
//  引入与窗口消息相关的宏定义头文件                                          //
/*============================================================================*/
#include "wmessage.h"

/*============================================================================*/
//  NUSDK中的全局函数声明                                                     //
/*============================================================================*/

////////////////////////////////////////////////////////////////////////////////
//描述:
//            系统消息结构
//说明:
//            系统消息队列中每一条消息的结构定义
//            handle   消息窗口句柄，表示目标窗口
//            message  消息类型
//            wparam   消息参数1
//            lparam   消息参数2
struct SYSTEM_MESSAGE 
{
  UINT32 handle;
  UINT32 message;
  UINT32 wparam;
  UINT32 lparam;
};

////////////////////////////////////////////////////////////////////////////////
//描述:
//            创建系统内存池
//参数:
//            void *first_available_memory     系统内存池的开始地址    
//返回:
//            无
//说明:
//            此函数在系统初始化时调用一次，且只能调用一次
//            系统内存池的大小由nusdk.cpp中的常量指定
extern void   InitSystemMemory(void *first_available_memory);

////////////////////////////////////////////////////////////////////////////////
//描述:
//            动态内存分配
//参数:
//            nSize         需分配的内存字节数
//返回:
//            成功则返回指向分配的内存块的指针
//            失败则返回NULL
//说明:
//            系统中所有的动态内存分配必需用此函数完成
extern void * SysMalloc(UINT32 nSize);

////////////////////////////////////////////////////////////////////////////////
//描述:
//            动态内存再分配
//参数:
//			  pPointer		原来 buffer 指针
//            nBeforeSize   原来分配的内存字节数
//			  nAfterSize	新分配的内存字节数
//返回:
//            成功则返回指向新分配的内存块的指针
//            失败则返回NULL
//说明:
//            系统中所有的动态内存分配必需用此函数完成
extern void * SysRealloc(void *pPointer , UINT32 nBeforeSize , UINT32 nAfterSize);

////////////////////////////////////////////////////////////////////////////////
//描述:
//            内存释放
//参数:
//            void *pPointer         指定待释放的内存块的指针
//返回:
//            无
//说明:
//            使用SysMalloc分配的内存必需用此函数释放
extern void   SysFree(void *pPointer);

////////////////////////////////////////////////////////////////////////////////
//描述:
//            创建系统消息队列
//参数:
//            无
//返回:
//            是否成功标志
//说明:
//            系统初始化时调用一次
extern BOOL   CreateSystemMessageQueue();

////////////////////////////////////////////////////////////////////////////////
//描述:
//            向系统消息队列中发送一条消息
//参数:
//            SYSTEM_MESSAGE *pMessage   指向待发消息的指针
//返回:
//            无
//说明:
//            消息队列中所有的消息由此函数发送
extern void   PostMessageToQueue(SYSTEM_MESSAGE *pMessage);

////////////////////////////////////////////////////////////////////////////////
//描述:
//            激活一高级中断向系统消息队列中发送一条消息
//参数:
//            
//返回:
//            无
//说明:
//            可用于任何地方LISR TIMER TASK .
extern void PostSystemMessage(UINT32 msg,UINT32 handle,UINT32 wparam,UINT32 lparam,bool bActivHisr=true);

////////////////////////////////////////////////////////////////////////////////
//描述:
//            从系统消息队列中取出一条消息
//参数:
//            SYSTEM_MESSAGE *pMessageBuf  指向存放消息的内存指针
//返回:
//            无
//说明:
//            消息队列中所有的消息由此获取发送
extern void   GetMessageFromeQueue(SYSTEM_MESSAGE *pMessageBuf);

////////////////////////////////////////////////////////////////////////////////
//描述:
//            把与窗口有关的消息派发到目标窗口
//参数:
//            SYSTEM_MESSAGE *lpMsg  指向存放消息的内存指针
//返回:
//            无
extern void DispatchMessage(const SYSTEM_MESSAGE *lpMsg);

////////////////////////////////////////////////////////////////////////////////
//描述:
//            初始化窗口系统
//参数:
//            无
//返回:
//            无
extern void InitWndSystem();

////////////////////////////////////////////////////////////////////////////////
//描述:
//            拷贝n个字符后面自动添0（但测试需要，现在未加上）
//参数:
//            无
//返回:
//            无

extern char *hwstrncpy(char *strDest, const char *strSource, size_t count );
////////////////////////////////////////////////////////////////////////////////
//这些常量是g_nCurApp的取值, 每个值代表一个应用,
//应用程序进入时必须设把g_nCurApp设为相应的值
//退出时必须调用SetCurrentApp(APPID_DESKTOP)
//
#define		APPID_CATALOG		0
#define		APPID_HELP			1
#define		APPID_CLOCK			2
#define		APPID_CALCULATOR	3
#define		APPID_DOCUMENT		4
#define		APPID_SETUP			5
#define		APPID_FASTDIAL		6
#define		APPID_SMS			7
#define		APPID_ALARM			8
#define		APPID_MEMO			9
#define		APPID_PICTURE		10
#define		APPID_CARD			11
#define		APPID_RECORD		12
#define		APPID_RECSTOP		13
#define		APPID_RECPLAY		14
#define		APPID_RECBROWSE		15
#define		APPID_HISTORY		16
#define     APPID_CALIBRATON    17            //added by hqf  12/14

#define		APPID_DESKTOP		APPID_CATALOG

////////////////////////////////////////////////////////////////////////////////
//描述:
//            这两个函数是访问全局量g_nCurApp的接口
extern void SetCurrentApp(UINT32 nAppId = APPID_DESKTOP);
extern UINT32 GetCurrentApp();

////////////////////////////////////////////////////////////////////////////////
//描述:
//            这两个函数是访问全局量g_iCurDialogApp的接口
extern  int   SetCurrentDialogApp(int iCurDialogApp);
extern  int   GetCurrentDialogApp();
////////////////////////////////////////////////////////////////////////////////
//
extern CFrameWnd * GetActiveWindow(CFrameWnd *pStartWnd = NULL);
//#include "keydrv.h"
#include "window.h"
#include "..\\res\\bmp\\e201icon.h"
#include "..\\res\\doc\\doc.h"
#include "datetime.h"
//#include "datadrv.h"

#endif 

//end hwfc.h
///////////////////////////////////////////////////////////