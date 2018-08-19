
/******************************************************************************/
/*                                                                            */
/*  版权所有(c)  2001汉王科技                                                 */
/*                                                                            */
/******************************************************************************/
/*  文 件 名                                                                  */
/*             cscrollwnd.h                                                   */
/*                                                                            */
/*  版 本 号                                                                  */
/*             1.0                                                            */
/*                                                                            */
/*  描    述                                                                  */
/*             汉王科技E-PhoneII窗口类库的源文件                              */
/*             HanWang Technology E-PhoneII Function Class Library head file  */
/*                                                                            */
/*  组    成                                                                  */
/*             滚动条窗口控件类头文件                                         */
/*                                                                            */
/*  作    者                                                                  */
/*             甘辉                                                           */
/*                                                                            */
/*  日    期                                                                  */
/*             2001-07-20                                                     */
/*                                                                            */
/*  历史记录                                                                  */
/******************************************************************************/
#ifndef __E401_MYCTRL_H_
#define __E401_MYCTRL_H_

#include "hwfcl.h"
#include "string.h"
#include "dc.h"    


class CScrollWnd  : public CCtrl  
{
private:
	UINT32 m_count;
	//
	UINT32				m_currentScrollPos;					//竖滚动条当前滚动条滑块位置
	UINT32				m_scrollHeight;						//竖滚动条当前滚动条滑块高度
	INT32				m_scrollRage;						//竖滚动条滚动范围
	INT32				m_linesPerPage;						//竖滚动条每页行数
	INT32				m_currentRagePos;					//竖滚动条当前滚动位置

	INT32				BARWIDTH;							//竖滚动条宽度
	INT32				m_mouseOnSlider;					//竖滚动条鼠标是否点种滑块
	INT32				m_mouseOnArrow;						//竖滚动条鼠标是否点种滑块

	UINT32				m_b_currentScrollPos;				//横滚动条当前滚动条滑块位置
	UINT32				m_b_scrollHeight;					//横滚动条当前滚动条滑块高度
	INT32				m_b_scrollRage;						//横滚动条滚动范围
	INT32				m_b_linesPerPage;					//横滚动条每页行数
	INT32				m_b_currentRagePos;					//横滚动条当前滚动位置

	INT32				b_BARWIDTH;							//横滚动条宽度
	INT32				m_b_mouseOnSlider;					//横滚动条鼠标是否点种滑块
	INT32				m_b_mouseOnArrow;					//横滚动条鼠标quyubiaozhi
    int					m_iMoveCount;
	INT32				BARMINHEIGHT;						//滚动条最小宽度

public:
	BOOL Create(char *strName, const CRect wndRect, 
				   CWnd * pParent, 
				   UINT32 dwStyle, UINT32 nCtrlId, 
				   UINT32 uiBarWidth, 
				   int iV, int iH);

public:
	virtual void	WindowProcess(UINT32 nMessage, 
							UINT32 wParam, UINT32 lParam);	//处理函数
	virtual void	OnScrollPosChanged(UINT32 uiNewPos){};	//拖动滑块处理函数（由用户重载）
	virtual void	OnClientEvent(UINT32 nMessage, 
							UINT32 x, UINT32 y){};			//点击客户区处理函数（由用户重载）			
	virtual void	OnPaint(CDC &dc	);						//重画函数

//以下的iSelect表示是对竖滚动条操作还是横滚动条操作，
//真是对竖滚动条操作,假是对横滚动条操作
	virtual void	PageDown(INT32 iSelect);				//下翻页处理函数
	virtual void	PageUp(INT32 iSelect);					//上翻页处理函数
	virtual void	ScrollDown(INT32 iSelect);				//下翻一行处理函数
	virtual void	ScrollUp(INT32 iSelect);				//上翻一行处理函数

	//以下函数操作滚动条
public:
	void	SetScrollPos(INT32 iPos, INT32 iSelect);		//设置滚动位置
	INT32	GetScrollPos(INT32 iSelect);					//取得滚动位置
	void	SetScrollRage(INT32 iRage, INT32 iSelect);		//设置滚动范围
	INT32	GetScrollRage(INT32 iSelect);					//取得滚动范围
	void	SetRowsPerPage(INT32 iRage, INT32 iSelect);		//设置每页滚动范围
	INT32	GetRowsPerPage(INT32 iSelect);					//取得每页滚动范围
	INT32	GetBarWid(INT32 iSelect);						//取得滚动位置
	void		DrawScroll();								//画滚动条//ygr 11-27

private:
	void	DoPtInSlider(INT32 x, INT32 y);					//拖动滑块处理函数
	void	DoPenDown(UINT32 nMessage, 
							UINT32 wParam, UINT32 lParam);	//鼠标点击处理函数
	void	DoPenMove(UINT32 nMessage, 
							UINT32 wParam, UINT32 lParam);	//鼠标移动处理函数
	void	DoPenOther(UINT32 nMessage, 
							UINT32 wParam, UINT32 lParam);	//其他鼠标事件处理函数
	

private:
	void	SetSliderWid(INT32 iSelect);					//设置滑块高度
	UINT32	GetSliderWid(INT32 iSelect);					//取得滑块高度
	void	SetSliderOff(INT32 iSelect);					//设置滑块偏移
	UINT32	GetSliderOff(INT32 iSelect);					//取得滑块偏移

private:
	INT32		ResetData();								//重设数据
	INT32		Init();										//初始化
	
};
//*/




#endif