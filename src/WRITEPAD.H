


/******************************************************************************/
/*                                                                            */
/*  版权所有(c)  2001汉王科技                                                 */
/*                                                                            */
/******************************************************************************/
/*  文 件 名                                                                  */
/*             cwritepad.h                                                    */
/*                                                                            */
/*  版 本 号                                                                  */
/*             1.0                                                            */
/*                                                                            */
/*  描    述                                                                  */
/*             汉王科技E-PhoneII窗口类库的源文件                              */
/*             HanWang Technology E-PhoneII Function Class Library head file  */
/*                                                                            */
/*  组    成                                                                  */
/*             手写板控件类头文件                                             */
/*                                                                            */
/*  作    者                                                                  */
/*             甘辉                                                           */
/*                                                                            */
/*  日    期                                                                  */
/*             2001-07-20                                                     */
/*                                                                            */
/*  历史记录                                                                  */
/*             2001-11-1  王飞修改很多地方                                    */
/******************************************************************************/
#ifndef __E401_WRITEPAD_H_
#define __E401_WRITEPAD_H_

#include "hwfcl.h"
#include "string.h"
#include "dc.h"    

/*
*	手写识别
*/
/*
#define UINT unsigned int
#define WORD unsigned short
#define BYTE unsigned char
#define DWORD unsigned long
*/
#define MAXPOINT 1024
//------------------识别范围设置----------------------
//说明:
//25个符号:￥下面可以有一横或两横
//4个手势:回车(0x0D)、空格(0x20)、回删(0x08)、删除(0x10)
//  |--------------------------------------------------------------|
//  | 常用汉字   | 4096 |                                          |
//  | 次常用汉字 | 2688 |                                          |
//  | 异 体 字   | 2778 |  写繁得简+异体字                         |
//  | 数    字   | 27   |  0..9                                    |
//  | 大写字母   | 76   |  A..Z                                    |
//  | 小写字母   | 63   |  a..z                                    |
//  | 标    点   | 18   |  !",:;?、。' ( ) ·—…〈〉《》          |
//  | 符    号   | 25   |  #$%&*+-./<=>@￥[ \ ] ^ _ ` { | } ~      |
//  | 手势符号   | 6    |  Enter+Blank+BackSpace+Delete            |
//  |--------------------------------------------------------------|
#define ALC_CHINESE_COMMON      0x0001  //常用汉字
#define ALC_CHINESE_RARE        0x0002  //次常用汉字	
#define ALC_CHINESE_VARIANT     0x0008  //异体字	
#define ALC_NUMERIC             0x0010  //数字	
#define ALC_UCALPHA             0x0020  //大写字母	
#define ALC_LCALPHA             0x0040  //小写字母	
#define	ALC_PUNCTUATION         0x0080  //标点	
#define	ALC_SYMBOLS             0x0100  //符号	
#define	ALC_GESTURE             0x0200  //手势	

#define ALC_UNUSED				0xFC00	/* Reserved for future use			*/
#define	ALC_VALID				0x03FF

#ifdef __cplusplus
extern "C" { 
#endif

int HWRecognize(UINT8* pb, int length, char* pw, int nCand, UINT16 wALC );
void HWSetDictionary( UINT32 dwRam, UINT32 dwRom );

#ifdef __cplusplus
}
#endif

//CWritePad+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//#define		HW_DIC_ADDRESS		(ASC_FONT_ADDR + 265792)
#define		HAND_STACK_SIZE		(0x1000)

#define		MAX_POINT_NUM		(400)
#define		IDENTIFY_TIME		(100)
#define		PEN_END_SOMBOY		(-1)
#define		PEN_STOP_SOMBOY		(0)

#define		TOP_HEIGHT			(17)
#define     WRITECHARHEIGHT     (16)

#define		RECOG_SEL_COUNT		14		// 候选字 buffer 大小

class CWritePad  : public CCtrl  
{
private:
	NU_TASK		m_RecognnizeTask1;						//手写识别框1的识别任务
	NU_TASK		m_RecognnizeTask2;						//手写识别框2的识别任务
	UINT8		m_uiTaskStack1[HAND_STACK_SIZE];		//手写识别框1的识别任务的堆栈
	UINT8		m_uiTaskStack2[HAND_STACK_SIZE];		//手写识别框2的识别任务的堆栈
	NU_TIMER	m_Timer1;								//手写识别框1的记时器
	NU_TIMER	m_Timer2;								//手写识别框2的记时器
	UINT16	*	m_currentPoint1;
	UINT16		m_pointBuff1[MAX_POINT_NUM];
	UINT16	*	m_currentPoint2;
	UINT16		m_pointBuff2[MAX_POINT_NUM];
	char		m_szResult[40];							//识别结果缓存

	UINT8		m_isDoMouseDown;						//鼠标点中区域标记
	UINT8		m_oldisDoMouseDown1;					//任务同步协调变量1
	UINT8		m_isMouseUp1;
	UINT8		m_oldisDoMouseDown2;					//任务同步协调变量2
	UINT8		m_isMouseUp2;
	CCtrl	*	m_CennectedCtrl;						//关联控件（编辑框）

	UINT16		m_bPressed;
	BOOL        m_bInit;

private:
	void	DoMouseDown(int x, int y);					//鼠标点击处理函数
	void	DoMouseMove(int x, int y);					//鼠标移动处理函数
	void	DoPenOther(UINT32 nMessage, 
				UINT32 wParam, UINT32 lParam);			//其他鼠标事件处理函数
	
private:
	static void	TimerProc1(CWritePad *pCtrl);			//手写识别框1的timer处理函数
	static void	TimerProc2(CWritePad *pCtrl);			//手写识别框2的timer处理函数
	static void RecognnizePrc1(UNSIGNED argc, 
								CWritePad *CWritePad);	//手写识别框1的识别任务
	static void RecognnizePrc2(UNSIGNED argc, 
								CWritePad *CWritePad);	//手写识别框2的识别任务

private:
	BOOL Init();
	BOOL UnInit();
public:
	BOOL			Create(char *strName, 
							const CRect wndRect, 
							CWnd * pParent, 
							UINT32 dwStyle, 
							UINT32 nCtrlId);			//创建函数
	virtual void	Destroy();							//销毁函数
	BOOL			CennectCtrl(CCtrl *pCtrl);			//关联一个控件

public:
	virtual void	WindowProcess(UINT32 nMessage, 
						UINT32 wParam, UINT32 lParam);	//消息处理函数
	virtual void	OnPaint(CDC &dc	);					//重画函数
	virtual void	OnTimer(NU_TIMER * pTimer);			//记时器处理函数
};


void InitHwDictionary();								//汉王手写字典初始化函数

//CWritePad----------------------------------------------------------------

#endif