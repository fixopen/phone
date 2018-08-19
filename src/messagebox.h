/******************************************************************************/
/*                                                                            */
/*  版权所有(c)  2001汉王科技                                                 */
/*                                                                            */
/******************************************************************************/
/*  文 件 名                                                                  */
/*             messagebox.h                                                   */
/*                                                                            */
/*  版 本 号                                                                  */
/*             1.0                                                            */
/*                                                                            */
/*  描    述                                                                  */
/*             汉王科技E-PhoneII窗口类库的头文件                              */
/*             HanWang Technology E-PhoneII Function Class Library head file  */
/*                                                                            */
/*  组    成                                                                  */
/*             MessageBox函数声明                                             */
/*             InputBox函数声明                                               */
/*             NumInputBox函数声明                                            */
/*                                                                            */
/*  作    者                                                                  */
/*             王飞                                                           */
/*                                                                            */
/*  日    期                                                                  */
/*             2001-07-16                                                     */
/*                                                                            */
/*  历史记录                                                                  */
/*             2001-08-07   增加NumInputBox                                   */
/*                                                                            */
/*             2001-10-15    增加日期输入函数                                 */
/*                           增加时间输入函数                                 */
/******************************************************************************/
#ifndef _HWFCL_MESSAGEBOX_H__
#define _HWFCL_MESSAGEBOX_H__
#include "hwfcl.h"
#include "window.h"
#include "datadrv.h"
#define BUTTONTYPE          4

////////////////////////////////////////////////////////////////////////////////
//类型定义
#define MB_OK               1               //"确定"按钮
#define MB_YES              2               //"是"按钮
#define MB_NO               4               //"否"按钮
#define MB_CANCEL           8               //"取消"按钮
#define MB_ICONSTOP         16              //显示STOP图标
#define MB_ICONQUESTION     32              //显示QUESTION图标
#define MB_ICONEXCLAMATION  64              //显示EXCLAMATION图标
#define MB_ICONINFORMATION  128             //显示INFORMATION图标

#define MB_OKCANCEL         (MB_OK+MB_CANCEL)
#define MB_YESNO            (MB_YES+MB_NO)
#define MB_YESNOCANCEL      (MB_YES+MB_NO+MB_CANCEL)

////////////////////////////////////////////////////////////////////////////////
//描述
//     显示一个提示窗口
//参数
//    strText      提示文字信息 
//    strCaption   提示框标题
//    uType        提示窗口类型见上面的定义
//返回
//    IDOK         用户按下"确定"按钮
//    IDYES        用户按下"是"按钮
//    IDNO         用户按下"否"按钮
//    IDCANCEL     用户按下"取消"按钮或右上角的窗口关闭按钮
//
UINT32 MessageBox(char * strText, char * strCaption = NULL, UINT32 uType = MB_OK);


#define    INIT_WRITE   0
#define    INIT_PY      1
#define    INIT_ENG     2
#define    INIT_SYMBO   3
////////////////////////////////////////////////////////////////////////////////
//描述
//    显示一个输入窗口
//参数
//    strCaption   输入框标题
//    strInputBuf  存放用户输入内容缓冲区
//    nBufLen      缓冲区长度
//    nInitStatus  初始状态  INIT_WRITE 手写 INIT_PY 拼音 INIT_ENG英文 INIT_SYMBO符号
//返回
//    IDOK         用户按下"确定"按钮
//    IDCANCEL     用户按下"取消"按钮
//
UINT32 InputBox(char * strCaption, char * strInputBuf, INT32 nBufLen, UINT8 nInitStatus = INIT_WRITE , BOOL bCanEnter = FALSE);

////////////////////////////////////////////////////////////////////////////////
//描述
//    显示一个电话号码数字输入窗口
//参数
//    strCaption   输入框标题
//    strInputBuf  存放用户输入内容缓冲区
//    nBufLen      缓冲区长度
//返回
//    IDOK         用户按下"确定"按钮
//    IDCANCEL     用户按下"取消"按钮
//
UINT32 NumInputBox(char * strCaption, char * strInputBuf, INT32 nBufLen);

////////////////////////////////////////////////////////////////////////////////
//描述
//    显示一个日期输入窗口
//参数
//    strCaption   输入框标题
//    nYear
//    nMonth
//    nDay         入口时初始化输入年月日，出口时返回用户选择的年月日
//返回
//    IDOK         用户按下"确定"按钮
//    IDCANCEL     用户按下"取消"按钮
//
UINT32 DateInputBox(char * strCaption, INT16 &nYear, INT16 &nMonth, INT16 &nDay,UINT32 nHelpID = 0);

////////////////////////////////////////////////////////////////////////////////
//描述
//    显示一个时间输入窗口
//参数
//    strCaption   输入框标题
//    nHour
//    nMinute      入口时初始化输入时分，出口时返回用户选择的时分
//返回
//    IDOK         用户按下"确定"按钮
//    IDCANCEL     用户按下"取消"按钮
//
UINT32 TimeInputBox(char * strCaption, INT16 &nHour, INT16 &nMinute, INT16 &nTimeType, UINT32 nHelpID =0);

class CIcon :public CIconButton
{
public:
	virtual BOOL Create(UINT32 nIconID , CRect &ctrlRect , CFrameWnd *pParent , UINT32 nCtrlID =0);
	virtual void OnPaint(CDC&dc)
	{
		dc.SetBackColor();
		dc.EraseRect(m_sRect);
		INT16 x = (m_sRect.left + m_sRect.right)/2 - gICON_WIDTH(m_nIconId)/2; 
		INT16 y = (m_sRect.top + m_sRect.bottom)/2 - gICON_HEIGHT(m_nIconId)/2; 
		dc.PutIcon(x , y, m_nIconId);
	}
};


#endif
////////////////////////////////////////////////////////////////////////////////
//end messagebox.h