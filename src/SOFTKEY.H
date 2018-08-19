/******************************************************************************/
/*                                                                            */
/*  版权所有(c)  2001汉王科技                                                 */
/*                                                                            */
/******************************************************************************/
/*  文 件 名                                                                  */
/*             softkey.h                                                      */
/*                                                                            */
/*  版 本 号                                                                  */
/*             1.0                                                            */
/*                                                                            */
/*  描    述                                                                  */
/*             汉王科技E-PhoneII窗口类库的头文件                              */
/*             HanWang Technology E-PhoneII Function Class Library head file  */
/*                                                                            */
/*  组    成                                                                  */
/*             软键盘控件类头文件                                             */
/*                                                                            */
/*  作    者                                                                  */
/*             王飞                                                           */
/*                                                                            */
/*  日    期                                                                  */
/*             2001-07-21                                                     */
/*                                                                            */
/*  历史记录                                                                  */
/******************************************************************************/
#ifndef _E201_SOFTKEY_H__
#define _E201_SOFTKEY_H__
#include "hwfcl.h"

////////////////////////////////////////////////////////////////////////////////
//接口说明:
//    Create        创建
//    SetParameter  运行时改变样子的接口
//    向父窗口发送  WM_KEY消息

struct KEYSTRUCT
{
	CRect    sKeyRect;
	UINT16   nKeyValue;
};

class CSoftKey : public CCtrl
{
protected:
	UINT32      m_nBkBmpId;
	UINT32      m_nKeyCount;
	INT32       m_nCurSel;
	KEYSTRUCT * m_pKeys;

protected:
	void    DoPenDown(CPoint &pt);
	void    DoPenUp();
	INT32   GetPenKey(CPoint &pt);
public:
    virtual void WindowProcess(UINT32 nMessage, UINT32 wParam, UINT32 lParam);
	virtual void OnPaint(CDC &dc);
	virtual void Destroy(); 	

public:
	BOOL Create(UINT32      nBkBmpId,       //软键盘的图标ID
		        UINT32      nKeyCount,      //软键盘按键个数
				CRect     * pKeyRectArray,  //每个按键的矩形
				UINT16    * pKeyValueArray, //每个按键的键值 
				CFrameWnd * pParent,        //父窗口指针
				UINT32      nCtrlId);       //软键盘的控件ID


public:
	void SetParameters(UINT32 nBkBmpId,                //图标 ID
		               UINT16 *pKeyValueArray = NULL,  //每个按键的键值
					   BOOL bRedraw = FALSE);           //是否重绘
};
#endif
////////////////////////////////////////////////////////////////////////////////
//end softkey.h

/******************************************************************************/
/*                                                                            */
/*  版权所有(c)  2001汉王科技                                                 */
/*                                                                            */
/******************************************************************************/
/*  文 件 名                                                                  */
/*             calendar.h                                                     */
/*                                                                            */
/*  版 本 号                                                                  */
/*             1.0                                                            */
/*                                                                            */
/*  描    述                                                                  */
/*             汉王科技E-PhoneII窗口类库的头文件                              */
/*             HanWang Technology E-PhoneII Function Class Library head file  */
/*                                                                            */
/*  组    成                                                                  */
/*             日历控件类头文件                                               */
/*                                                                            */
/*  作    者                                                                  */
/*             王飞                                                           */
/*                                                                            */
/*  日    期                                                                  */
/*             2001-10-15                                                     */
/*                                                                            */
/*  历史记录     
/* 2001/11/19 程晓东修改 private:PaintOneDay为虚函数 protected:               */
/*            virtual void PaintOneDay(CDC &dc, INT16 nDay, UINT32 nIconId,   */
/*                           INT16 startx, INT16 starty);                     */
/*2001/12/10 //清空选中区域--程晓东添加                                       */
/******************************************************************************/ 

#ifndef _E201_CALENDAR_H__
#define _E201_CALENDAR_H__
#include "hwfcl.h"
#include "datetime.h"
////////////////////////////////////////////////////////////////////////////////
//接口说明:
//    Create        创建
//    向父窗口发送  CN_COMMAND消息
class CCalendar : public CCtrl
{
private:
   INT16  m_nYear, m_nMonth, m_nDay;
   INT16  m_nPenSelDay;
   CRect  m_selRect;
   BOOL   AlarmUseFlag;//cxd add 判断是否在提醒中使用
private:
	void DoPenMove(CPoint &pt);
    void DoPenMoveOut();
    void DoPenUp();

	void PaintTitle(CDC &dc);
	void PaintDate(CDC &dc);

	//void PaintOneDay(CDC &dc, INT16 nDay, UINT32 nIconId, INT16 startx, INT16 starty);
	UINT16 CalcPenDay(CPoint &pt);
	void SelectDay(UINT16 iDay, BOOL bUnSelLast=FALSE);

public:
	//CRect GetSlectRect(){return m_selRect;};//取得选中区域--程晓东添加
	//void ResetSelectRect();//清空选中区域--程晓东添加
	virtual void PaintOneDay(CDC &dc, INT16 nDay, UINT32 nIconId, INT16 startx, INT16 starty);
    void SetDate(INT16 nYear, INT16 nMonth, INT16 nDay);
	void GetDate(INT16 &nYear, INT16 &nMonth, INT16 &nDay)
	{
		nYear = m_nYear, nMonth = m_nMonth, nDay = m_nDay;
	}
	INT16 GetYear(){return m_nYear;}
	INT16 GetMonth(){return m_nMonth;}
	INT16 GetDay(){return m_nDay;}
	BOOL Create(CPoint &ptLeftTop, CFrameWnd *pParent, UINT32 nCtrlId,BOOL Flag=FALSE);
	virtual void OnPaint(CDC &dc);
	virtual void WindowProcess(UINT32 nMessage, UINT32 wParam, UINT32 lParam);
};
#endif
////////////////////////////////////////////////////////////////////////////////
//end calendar.h

/******************************************************************************/
/*                                                                            */
/*  版权所有(c)  2001汉王科技                                                 */
/*                                                                            */
/******************************************************************************/
/*  文 件 名                                                                  */
/*             clock.h                                                        */
/*                                                                            */
/*  版 本 号                                                                  */
/*             1.0                                                            */
/*                                                                            */
/*  描    述                                                                  */
/*             汉王科技E-PhoneII窗口类库的头文件                              */
/*             HanWang Technology E-PhoneII Function Class Library head file  */
/*                                                                            */
/*  组    成                                                                  */
/*             时钟控件类头文件                                               */
/*                                                                            */
/*  作    者                                                                  */
/*             王飞                                                           */
/*                                                                            */
/*  日    期                                                                  */
/*             2001-10-15                                                     */
/*                                                                            */
/*  历史记录                                                                  */
/******************************************************************************/
#ifndef _E201_CLOCK_H__
#define _E201_CLOCK_H__
#include "hwfcl.h"
#include "datetime.h"
////////////////////////////////////////////////////////////////////////////////
//  一个按着不放连续发消息的按钮
class CUpDownButton : public CIconButton
{
public:
	void WindowProcess(UINT32 nMessage, UINT32 wParam, UINT32 lParam);
};
////////////////////////////////////////////////////////////////////////////////
//接口说明:
//    Create        创建
//    向父窗口发送  CN_COMMAND消息
class CClock : public CCtrl
{
private:
    INT16  m_nHour, m_nMinute, m_nSecond;
	INT16  m_nClockStyle;     //时钟风格 为0时有秒针 1时没有秒针
private:
    void GetDxDy(INT16 &dx, INT16 &dy, INT16 &dx1, INT16 &dy1, INT16 &dx2, INT16 &dy2,
                         INT16 iSubScript, INT16 iFlag, INT16 iHandType);	
	void GetDxDy(INT16 &dx, INT16 &dy, INT16 iLength , INT16 iAngle);
public:
    void SetTime(INT16 nHour, INT16 nMinute, INT16 nSecond);
	void GetTime(INT16 &nHour, INT16 &nMinute, INT16 &nSecond)
	{
		nHour = m_nHour, nMinute = m_nMinute, nSecond= m_nSecond;
	}
	void SetClockStyle(INT16 nNewStyle)
	{
		m_nClockStyle = nNewStyle;
	}
	INT16 GetHour(){return m_nHour;}
	INT16 GetMinute(){return m_nMinute;}
	INT16 GetSecond(){return m_nSecond;}
	BOOL Create(CPoint &ptLeftTop, CFrameWnd *pParent, UINT32 nCtrlId);
	void StepSecond(BOOL bAdd = TRUE);
	void StepMinute(BOOL bAdd = TRUE);
	void StepHour(BOOL bAdd = TRUE);
public:
	virtual void OnPaint(CDC &dc);
};
#endif
////////////////////////////////////////////////////////////////////////////////
//end clock.h