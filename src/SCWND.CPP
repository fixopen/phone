
/******************************************************************************/
/*                                                                            */
/*  版权所有(c)  2001汉王科技                                                 */
/*                                                                            */
/******************************************************************************/
/*  文 件 名                                                                  */
/*             cscrollwnd.cpp                                                 */
/*                                                                            */
/*  版 本 号                                                                  */
/*             1.0                                                            */
/*                                                                            */
/*  描    述                                                                  */
/*             汉王科技E-PhoneII窗口类库的源文件                              */
/*             HanWang Technology E-PhoneII Function Class Library head file  */
/*                                                                            */
/*  组    成                                                                  */
/*             滚动条窗口控件类源文件                                         */
/*                                                                            */
/*  作    者                                                                  */
/*             甘辉                                                           */
/*                                                                            */
/*  日    期                                                                  */
/*             2001-07-20                                                     */
/*                                                                            */
/*  历史记录                                                                  */
/*             2001-11-01   modify by wangfei                                 */
/******************************************************************************/
#include "scrollwnd.h"
#include "scrollbar.h"

/////////////////////////////////////////////////////////////////////
//功能    : 判断一个点是否在矩形内
//参数    : 
//返回值  : 成功，真；否则， 假
//注意事项: 
/////////////////////////////////////////////////////////////////////
INT32 PointInRect(CPoint TESTPT, short Xmin, short Ymin, 
											short Xmax, short Ymax)
{
	if(TESTPT.x < Xmax && TESTPT.x > Xmin && 
				TESTPT.y < Ymax && TESTPT.y > Ymin)
		return 1;
	return 0;
}

/////////////////////////////////////////////////////////////////////
//功能    : 
//参数    : 
//返回值  : 
//注意事项: 
/////////////////////////////////////////////////////////////////////
void DrawJiant(int left, int top, int right, int bottom, INT32 iMode)
{
	INT32 i, j;
	CDC dc;
	INT32 xmiddle = (left + right)/2;
	INT32 ymiddle = (top + bottom)/2;
	dc.SetPenColor();

	for(i=5; i>0; i--)
	{
		for(j=0; j<i; j++)
		{	
			if(iMode == 1)
			{
				dc.SetPixel(xmiddle + j, ymiddle - 3 + i);
				dc.SetPixel(xmiddle - j, ymiddle - 3 + i);
			}
			else
			{
				dc.SetPixel(xmiddle + j, ymiddle + 3 - i);
				dc.SetPixel(xmiddle - j, ymiddle + 3 - i);
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////
//功能    : 
//参数    : 
//返回值  : 
//注意事项: 
/////////////////////////////////////////////////////////////////////
void DrawJianth(int left, int top, int right, int bottom, INT32 iMode)
{
	INT32 i, j;
	CDC dc;
	INT32 xmiddle = (left + right)/2;
	INT32 ymiddle = (top + bottom)/2;
	dc.SetPenColor();

	for(i=5; i>0; i--)
	{
		for(j=0; j<i; j++)
		{	
			if(iMode == 1)
			{
				dc.SetPixel(xmiddle - 3 + i, ymiddle + j);
				dc.SetPixel(xmiddle - 3 + i, ymiddle - j);
			}
			else
			{
				dc.SetPixel(xmiddle + 3 - i, ymiddle + j);
				dc.SetPixel(xmiddle + 3 - i, ymiddle - j);
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////
//功能    : 画一个3D矩形
//参数    : 矩形区域
//返回值  : 
//注意事项: 
/////////////////////////////////////////////////////////////////////
void Draw3DFrame(CRect  pRect)
{
	CDC dc;
	dc.SetBackColor(GRAY);
	dc.EraseRect(pRect);
	dc.Draw3dRect(pRect, WHITE, BLACK);
}

/////////////////////////////////////////////////////////////////////
//功能    : 创建手写板
//参数    : 标题(不显示), 大小, 父窗口, 类型, id号，
//          是否需要纵滚动条，是否需要横滚动条
//返回值  : 成功, 真; 否则, 假
//注意事项: 
/////////////////////////////////////////////////////////////////////
BOOL CScrollWnd::Create(char *strName, const CRect wndRect, 
				   CWnd * pParent, UINT32 dwStyle, UINT32 nCtrlId, 
				   UINT32 uiBarWidth, int iV, int iH)//-
{
	m_currentScrollPos = 0;
	m_scrollHeight = 0;
	m_scrollRage = 0;
	m_currentRagePos = 0;
	m_mouseOnSlider = 0;
	m_mouseOnArrow = 0;

	m_linesPerPage = 20;

	m_b_currentScrollPos = 0;	
	m_b_scrollHeight = 0;	 	
	m_b_scrollRage = 0;				
	m_b_currentRagePos = 0;			
	m_b_mouseOnSlider = 0;			
	m_b_mouseOnArrow = 0;			
	
	m_b_linesPerPage =20;			

	if(uiBarWidth && iV)
		BARWIDTH = uiBarWidth;
	else
		BARWIDTH = 0;
	if(uiBarWidth && iH)
		b_BARWIDTH = uiBarWidth;
	else
		b_BARWIDTH = 0;

	BARMINHEIGHT = 14;
	//if(wndRect.Width()<=uiBarWidth+1 || 
	//			wndRect.Height()-uiBarWidth<uiBarWidth*2+BARMINHEIGHT)
	//	BARWIDTH = 0;
	if(wndRect.Width()<=uiBarWidth+1 || 
				wndRect.Height()-2<uiBarWidth*2+BARMINHEIGHT)
		BARWIDTH = 0;
	if(wndRect.Height()<=uiBarWidth+1 || 
				wndRect.Width()-uiBarWidth<uiBarWidth*2+BARMINHEIGHT)
		b_BARWIDTH = 0;

	if(!CCtrl::Create(strName, wndRect, pParent, dwStyle, nCtrlId))
		return 0;

	if(!Init())
		return 0;

	return 1;
}

/////////////////////////////////////////////////////////////////////
//功能    : 初始化
//参数    : 
//返回值  : 
//注意事项: 
/////////////////////////////////////////////////////////////////////
INT32 CScrollWnd::Init()
{
	return ResetData();
}

/////////////////////////////////////////////////////////////////////
//功能    : 
//参数    : 
//返回值  : 
//注意事项: 
/////////////////////////////////////////////////////////////////////
INT32 CScrollWnd::ResetData()//+
{
	SetScrollRage(100, 1);
	SetScrollRage(100, 0);

	return 1;
}

/////////////////////////////////////////////////////////////////////
//功能    : 设当前滚动位置  
//参数    : 位置值，操作选择（真，对横滚动条操作；假,对横纵动条操作）
//返回值  : 
//注意事项: 
/////////////////////////////////////////////////////////////////////
void CScrollWnd::SetScrollPos(INT32 iPos, INT32 iSelect)
{
	if(iSelect)
	{
		if(iPos < 0)
			m_currentRagePos = 0;
		else if(iPos > m_scrollRage - 1)
			m_currentRagePos = m_scrollRage - 1;
		else
			m_currentRagePos = iPos;

		SetSliderOff(1);
	}
	else
	{
		if(iPos < 0)
			m_b_currentRagePos = 0;
		else if(iPos > m_b_scrollRage - 1)
			m_b_currentRagePos = m_b_scrollRage - 1;
		else
			m_b_currentRagePos = iPos;

		SetSliderOff(0);
	}
}

/////////////////////////////////////////////////////////////////////
//功能    : 取当前滚动位置  
//参数    : 真，对横滚动条操作；假，对横纵动条操作
//返回值  : 位置值
//注意事项: 
/////////////////////////////////////////////////////////////////////
INT32  CScrollWnd::GetScrollPos(INT32 iSelect)
{
	if(iSelect)
		return m_currentRagePos;
	else
		return m_b_currentRagePos;
}

/////////////////////////////////////////////////////////////////////
//功能    : 社滚动范围
//参数    : 范围值，操作选择（真，对横滚动条操作；假,对横纵动条操作）
//返回值  : 
//注意事项: 
/////////////////////////////////////////////////////////////////////
void CScrollWnd::SetScrollRage(INT32 iRage, INT32 iSelect)
{
	if(iSelect)
	{
		if(iRage < 0)
			m_scrollRage = 0;
		else
			m_scrollRage = iRage;

		SetSliderWid(1);
	}
	else
	{
		if(iRage < 0)
			m_b_scrollRage = 0;
		else
			m_b_scrollRage = iRage;

		SetSliderWid(0);
	}
}

/////////////////////////////////////////////////////////////////////
//功能    : 取滚动范围
//参数    : 真，对横滚动条操作；假，对横纵动条操作
//返回值  : 范围值
//注意事项: 
/////////////////////////////////////////////////////////////////////
INT32  CScrollWnd::GetScrollRage(INT32 iSelect)
{
	if(iSelect)
		return m_scrollRage;
	else
		return m_b_scrollRage;
}

/////////////////////////////////////////////////////////////////////
//功能    : 设每页行数
//参数    : 真，对横滚动条操作；假，对横纵动条操作
//返回值  : 
//注意事项: 
/////////////////////////////////////////////////////////////////////
void CScrollWnd::SetRowsPerPage(INT32 iRage, INT32 iSelect)
{
	if(iSelect)
	{
		if(iRage <= 0)
			m_linesPerPage = 0;
		else
			m_linesPerPage = iRage;
		
		SetSliderWid(1);
		SetSliderOff(1);
	}
	else
	{
		if(iRage <= 0)
			m_b_linesPerPage = 0;
		else
			m_b_linesPerPage = iRage;
		
		SetSliderWid(0);
		SetSliderOff(0);
	}
}

/////////////////////////////////////////////////////////////////////
//功能    : 取每页行数
//参数    : 真，对横滚动条操作；假，对横纵动条操作
//返回值  : 每页行数
//注意事项: 
/////////////////////////////////////////////////////////////////////
INT32 CScrollWnd::GetRowsPerPage(INT32 iSelect)
{
	if(iSelect)
		return m_linesPerPage;
	else
		return m_b_linesPerPage;
}

/////////////////////////////////////////////////////////////////////
//功能    : 取滚动条偏移
//参数    : 真，对横滚动条操作；假，对横纵动条操作
//返回值  : 宽度值
//注意事项: 
/////////////////////////////////////////////////////////////////////
INT32 CScrollWnd::GetBarWid(INT32 iSelect)
{
	if(iSelect)
		return this->BARWIDTH;
	else
		return this->b_BARWIDTH;
}

/////////////////////////////////////////////////////////////////////
//功能    : 设滑块偏移
//参数    : 真，对横滚动条操作；假，对横纵动条操作
//返回值  : 
//注意事项: 
/////////////////////////////////////////////////////////////////////
void CScrollWnd::SetSliderWid(INT32 iSelect)
{
	INT32			   height;

	if(iSelect)
	{
		height = m_sRect.Height()-b_BARWIDTH;
		if(m_scrollRage > m_linesPerPage)
			m_scrollHeight = m_linesPerPage*(height - 2*BARWIDTH)/
								m_scrollRage;
		else
			m_scrollHeight = height -2*BARWIDTH-1;

		if(m_scrollHeight < BARMINHEIGHT)
			m_scrollHeight = BARMINHEIGHT;
	}
	else
	{
		height = m_sRect.Width()-BARWIDTH;
		if(m_b_scrollRage > m_b_linesPerPage)
			m_b_scrollHeight = m_b_linesPerPage*(height - 2*b_BARWIDTH)/
								m_b_scrollRage;
		else
			m_b_scrollHeight = height -2*b_BARWIDTH-1;

		if(m_b_scrollHeight < BARMINHEIGHT)
			m_b_scrollHeight = BARMINHEIGHT;
	}
}

/////////////////////////////////////////////////////////////////////
//功能    : 取滑块高度
//参数    : 真，对横滚动条操作；假，对横纵动条操作
//返回值  : 高度值
//注意事项: 
/////////////////////////////////////////////////////////////////////
UINT32  CScrollWnd::GetSliderWid(INT32 iSelect)
{
	if(iSelect)
		return m_scrollHeight;
	else
		return m_b_scrollHeight;
}

/////////////////////////////////////////////////////////////////////
//功能    : 设滑块高度
//参数    : 真，对横滚动条操作；假，对横纵动条操作
//返回值  : 
//注意事项: 
/////////////////////////////////////////////////////////////////////
void CScrollWnd::SetSliderOff(INT32 iSelect)
{
	short   top;
	short   bottom;
	INT32	numscre;

	if(iSelect)
	{
		top  = m_sRect.top;//;
		bottom = m_sRect.bottom-b_BARWIDTH;
		numscre = m_linesPerPage;

		if(GetScrollRage(1) <= m_linesPerPage ||
											GetScrollPos(1) == 0)
			m_currentScrollPos = 0;
		else
			m_currentScrollPos = (GetScrollPos(1))* \
					(bottom-top-2*BARWIDTH - GetSliderWid(1))/ \
					(GetScrollRage(1)-m_linesPerPage);

		if(m_currentScrollPos + GetSliderWid(1) > 
				(bottom - top - 2*BARWIDTH))
			m_currentScrollPos = 
					  ((bottom-top-2*BARWIDTH > GetSliderWid(1)) ?
					  (bottom-top-2*BARWIDTH-GetSliderWid(1)) : (0));
	}
	else
	{
		top  = m_sRect.left;//;
		bottom = m_sRect.right-BARWIDTH;
		numscre = m_b_linesPerPage;

		if(GetScrollRage(0) <= m_b_linesPerPage || 
											GetScrollPos(0) == 0)
			m_b_currentScrollPos = 0;
		else
			m_b_currentScrollPos = (GetScrollPos(0))*
					(bottom-top-2*b_BARWIDTH - GetSliderWid(0))/ \
					(GetScrollRage(0)-m_b_linesPerPage);

		if(m_b_currentScrollPos + GetSliderWid(0) > 
				(bottom - top - 2*b_BARWIDTH))
			m_b_currentScrollPos = 
						((bottom-top-2*BARWIDTH > GetSliderWid(0)) ?
						(bottom-top-2*BARWIDTH-GetSliderWid(0)) : (0));
	}
}

/////////////////////////////////////////////////////////////////////
//功能    : 取滑块偏移
//参数    : 真，对横滚动条操作；假，对横纵动条操作
//返回值  : 偏移值
//注意事项: 
/////////////////////////////////////////////////////////////////////
UINT32 CScrollWnd::GetSliderOff(INT32 iSelect)
{
	if(iSelect)
		return m_currentScrollPos;
	else
		return m_b_currentScrollPos;
}

/////////////////////////////////////////////////////////////////////
//功能    : 滚动条下滚一行
//参数    : 真，对横滚动条操作；假，对横纵动条操作
//返回值  : 
//注意事项: 
/////////////////////////////////////////////////////////////////////
void CScrollWnd::ScrollDown(INT32 iSelect)//-
{
	//modify by wangfei
	if(GetScrollPos(iSelect)+m_linesPerPage >= m_scrollRage )
		return;

	if(iSelect)
		SetScrollPos(GetScrollPos(1) + 1, 1);
	else
		SetScrollPos(GetScrollPos(0) + 1, 0);
	OnPaint(CDC());
}

/////////////////////////////////////////////////////////////////////
//功能    : 滚动条下滚一行
//参数    : 真，对横滚动条操作；假，对横纵动条操作
//返回值  : 
//注意事项: 
/////////////////////////////////////////////////////////////////////
void CScrollWnd::PageDown(INT32 iSelect)
{
	
	//modify by wangfei
	if(GetScrollPos(iSelect) >= m_scrollRage - 1)
		return;

	if(GetScrollPos(iSelect)+2*GetRowsPerPage(iSelect) >= 
											GetScrollRage(iSelect))
	
	{
		SetScrollPos(
			GetScrollRage(iSelect)-GetRowsPerPage(iSelect), iSelect);
	}
	else
	{
		SetScrollPos(
			GetScrollPos(iSelect)+GetRowsPerPage(iSelect), iSelect);
	}
	OnPaint(CDC());
}

/////////////////////////////////////////////////////////////////////
//功能    : 滚动条上滚一页
//参数    : 真，对横滚动条操作；假，对横纵动条操作
//返回值  : 
//注意事项: 
/////////////////////////////////////////////////////////////////////
void CScrollWnd::PageUp(INT32 iSelect)
{
	//modify by wangfei
	if(GetScrollPos(iSelect) == 0)
		return;
	SetScrollPos(
		GetScrollPos(iSelect)-GetRowsPerPage(iSelect), iSelect);
	OnPaint(CDC());
}

/////////////////////////////////////////////////////////////////////
//功能    : 滚动条上滚一行
//参数    : 真，对横滚动条操作；假，对横纵动条操作
//返回值  : 
//注意事项: 
/////////////////////////////////////////////////////////////////////
void CScrollWnd::ScrollUp(INT32 iSelect)//-
{
	//modify by wangfei
	if(GetScrollPos(iSelect) == 0)
		return;

	if(iSelect)
		SetScrollPos(GetScrollPos(1) - 1, 1);
	else
		SetScrollPos(GetScrollPos(0) - 1, 0);
	OnPaint(CDC());
}

/////////////////////////////////////////////////////////////////////
//功能    : 鼠标点击滑块处理函数
//参数    : 鼠标横坐标, 鼠标纵坐标
//返回值  : 
//注意事项: 
/////////////////////////////////////////////////////////////////////
void CScrollWnd::DoPtInSlider(INT32 x, INT32 y)//+
{
	CDC dc;
	INT32			   scrheight, left, right, top, bottom, \
					ypos, numscre, oldscrpos;

	left = m_sRect.left;
	top  = m_sRect.top;
	if(BARWIDTH && m_mouseOnArrow == 3)
	{
		right = m_sRect.right;
		bottom = m_sRect.bottom-b_BARWIDTH;
		scrheight = GetSliderWid(1);
		ypos = y - m_mouseOnSlider;
		numscre = m_linesPerPage;
		oldscrpos = GetScrollPos(1);

		if(GetScrollRage(1) <= numscre)
		{
			SetScrollPos(0, 1);
			return;
		}

		if(ypos <= top + BARWIDTH + 1)
			SetScrollPos(0, 1);
		else if(y > (bottom-BARWIDTH-scrheight+m_mouseOnSlider))
		{
			SetScrollPos(GetScrollRage(1) - numscre, 1);
		}
		else
		{
			INT32 l = ypos - top - BARWIDTH;
			SetScrollPos(l*(GetScrollRage(1)-numscre)/
				(bottom - top - 2*BARWIDTH - scrheight), 1);
		}

		if(oldscrpos != GetScrollPos(1))
		{
			OnScrollPosChanged(GetScrollPos(1));
			OnPaint(dc);
		}
		else
		{
			dc.SetBackColor(2);
			dc.EraseRect(right-BARWIDTH, top+BARWIDTH, 
							right-1, bottom-BARWIDTH-1);
			if(ypos <= top+BARWIDTH)
			{
				CRect my_rect31(right - BARWIDTH+1, top + BARWIDTH, 
						right-1, top + BARWIDTH + GetSliderWid(1)); 
				Draw3DFrame(my_rect31);
			}
			else if(ypos >= bottom - BARWIDTH - GetSliderWid(1) - 1)
			{
				CRect my_rect32(right-BARWIDTH+1,
						bottom-BARWIDTH-GetSliderWid(1) - 1, 
						right-1, bottom - BARWIDTH - 1); 
				Draw3DFrame(my_rect32);
			}
			else
			{
				CRect my_rect32(right-BARWIDTH+1, ypos, 
						right-1, ypos + GetSliderWid(1)); 
				Draw3DFrame(my_rect32);
			}
		}
	}
	if(b_BARWIDTH && m_b_mouseOnArrow == 3)
	{
		right = m_sRect.right-BARWIDTH;
		bottom = m_sRect.bottom;
		scrheight = GetSliderWid(0);
		ypos = x - m_b_mouseOnSlider;
		numscre = m_b_linesPerPage;
		oldscrpos = GetScrollPos(0);

		if(GetScrollRage(0) <= numscre)
		{
			SetScrollPos(0, 0);
			return;
		}

		if(ypos <= left + b_BARWIDTH + 1)
			SetScrollPos(0, 0);
		else if(x > (right-b_BARWIDTH-scrheight+m_b_mouseOnSlider))
		{
			SetScrollPos(GetScrollRage(0) - numscre, 0);
		}
		else
		{
			INT32 l = ypos - left - b_BARWIDTH;
			SetScrollPos(l*(GetScrollRage(0)-numscre)/
				(right - left - 2*b_BARWIDTH - scrheight), 0);
		}

		if(oldscrpos != GetScrollPos(0))
		{
			OnScrollPosChanged(GetScrollPos(0));
			OnPaint(dc);
		}
		else
		{
			dc.SetBackColor(2);
			dc.EraseRect(left+b_BARWIDTH, bottom-b_BARWIDTH, 
								right-b_BARWIDTH-1, bottom-1);
			if(ypos <= left+b_BARWIDTH)
			{
				CRect my_rect31(left+b_BARWIDTH,
						bottom-b_BARWIDTH+1, 
						left+b_BARWIDTH+GetSliderWid(0), bottom-1); 
				Draw3DFrame(my_rect31);
			}
			else if(ypos >= right-b_BARWIDTH-GetSliderWid(0)-1)
			{
				CRect my_rect32(right-b_BARWIDTH-GetSliderWid(0)-1, 
									bottom-b_BARWIDTH+1, 
									right-b_BARWIDTH-1, bottom-1); 
				Draw3DFrame(my_rect32);
			}
			else
			{
				CRect my_rect32(ypos, bottom-b_BARWIDTH+1, 
								ypos+GetSliderWid(0), bottom-1); 
				Draw3DFrame(my_rect32);
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////
//功能    : 鼠标点下处理函数
//参数    : 鼠标横坐标, 鼠标纵坐标
//返回值  : 
//注意事项: 
/////////////////////////////////////////////////////////////////////
void	CScrollWnd::DoPenDown(UINT32 nMessage, UINT32 wParam, UINT32 lParam)
{
	CDC dc;
	CPoint	pt(HIWORD(wParam), LOWORD(wParam));
	short	left, right, top, bottom;

	m_iMoveCount = MOVE_FILTER_NUM;
	left = m_sRect.left;
	top  = m_sRect.top;
	right = m_sRect.right-BARWIDTH;
	bottom = m_sRect.bottom-b_BARWIDTH;

	if(PointInRect(pt, left, top, right, bottom))
	{
		m_mouseOnArrow = 6;
		OnClientEvent(nMessage, pt.x, pt.y);
		return;
	}

	if(BARWIDTH)
	{
		right = m_sRect.right;
		bottom = m_sRect.bottom-b_BARWIDTH;
		if(PointInRect(pt, right-BARWIDTH, 
					top+BARWIDTH+GetSliderOff(1), 
					right, 
					top+BARWIDTH+GetSliderOff(1)+GetSliderWid(1)))
		{
			m_mouseOnSlider = pt.y-(top+BARWIDTH+GetSliderOff(1));
			m_mouseOnArrow = 3;
		}
		else if(PointInRect(pt, right - BARWIDTH, 
					top, right, top + BARWIDTH))
		{
			if(m_mouseOnArrow != 1)
			{
				dc.InvertRect(right-BARWIDTH, top+1, 
								right-1, top +BARWIDTH-1);
				m_mouseOnArrow = 1;
			}
		}
		else if(PointInRect(pt, right - BARWIDTH, 
								bottom - BARWIDTH , right, bottom))
		{
			if(m_mouseOnArrow != 2)
			{
				dc.InvertRect(right - BARWIDTH, bottom - BARWIDTH, 
									right-1, bottom-1);
				m_mouseOnArrow = 2;
			}
		}
		else if(PointInRect(pt, right - BARWIDTH, top + BARWIDTH, 
			right, top + BARWIDTH + GetSliderOff(1)))
			m_mouseOnArrow = 4;
		else if(PointInRect(pt, right-BARWIDTH+1, 
					top+BARWIDTH+GetSliderOff(1)+GetSliderWid(1)-2, 
					right-1, bottom - BARWIDTH - 2))
			m_mouseOnArrow = 5;
	}
	if(b_BARWIDTH)
	{
		right = m_sRect.right-BARWIDTH;
		bottom = m_sRect.bottom;
		
		if(PointInRect(pt, left+b_BARWIDTH+GetSliderOff(0), 
					bottom-b_BARWIDTH, 
					left+b_BARWIDTH+GetSliderOff(0)+GetSliderWid(0), 
					bottom))
		{
			m_b_mouseOnSlider = 
						pt.x-(left+b_BARWIDTH+GetSliderOff(0));
			m_b_mouseOnArrow = 3;
		}
		else if(PointInRect(pt, left, bottom-b_BARWIDTH, 
								left+b_BARWIDTH, bottom))
		{
			if(m_b_mouseOnArrow != 1)
			{
				dc.InvertRect(left+1, bottom-b_BARWIDTH, 
								left+b_BARWIDTH-1, bottom-1);
				m_b_mouseOnArrow = 1;
			}
		}
		else if(PointInRect(pt, right-b_BARWIDTH, 
							bottom-b_BARWIDTH , right, bottom))
		{
			if(m_b_mouseOnArrow != 2)
			{
				dc.InvertRect(right-b_BARWIDTH+1, 
							bottom-b_BARWIDTH , right-1, bottom-1);
				m_b_mouseOnArrow = 2;
			}
		}
		else if(PointInRect(pt, left+b_BARWIDTH, 
								bottom-b_BARWIDTH, 
								left+b_BARWIDTH+GetSliderOff(0), 
								bottom))
			m_b_mouseOnArrow = 4;
		else if(PointInRect(pt, 
				left+b_BARWIDTH+GetSliderOff(0)+GetSliderWid(0)-1, 
				bottom-b_BARWIDTH, right-b_BARWIDTH-1, bottom))
			m_b_mouseOnArrow = 5;
	}
}

/////////////////////////////////////////////////////////////////////
//功能    : 鼠标移动处理函数
//参数    : 鼠标横坐标, 鼠标纵坐标
//返回值  : 
//注意事项: 
/////////////////////////////////////////////////////////////////////

void	CScrollWnd::DoPenMove(UINT32 nMessage, 
							  UINT32 wParam, UINT32 lParam)
{
	CDC dc;
	CPoint	pt(HIWORD(wParam), LOWORD(wParam));
	short	left, right, top, bottom;


	left = m_sRect.left;
	top  = m_sRect.top;

	if(m_mouseOnArrow == 6)
	{
		OnClientEvent(nMessage, pt.x, pt.y);
		return;
	}

	if(m_iMoveCount++ >= MOVE_FILTER_NUM) // hqf  2002/1/7
	{
		if(BARWIDTH)
		{
			right = m_sRect.right;
			bottom = m_sRect.bottom-b_BARWIDTH;
			if(m_mouseOnArrow == 1)
				ScrollUp(1);
			else if(m_mouseOnArrow == 2)
				ScrollDown(1);
			else if(m_mouseOnArrow == 3)
				DoPtInSlider(pt.x, pt.y);
			else if(m_mouseOnArrow == 4 && 
						PointInRect(pt, right-BARWIDTH, top+BARWIDTH, 
						right, top + BARWIDTH + GetSliderOff(1)))
				PageUp(1);
			else if(m_mouseOnArrow == 5 && 
						PointInRect(pt, right-BARWIDTH+1, 
						top+BARWIDTH+GetSliderOff(1)+GetSliderWid(1)-2, 
						right-1, bottom - BARWIDTH - 2))
				PageDown(1);
		}
		if(b_BARWIDTH)
		{
			right = m_sRect.right-BARWIDTH;
			bottom = m_sRect.bottom;
			if(m_b_mouseOnArrow == 1)
				ScrollUp(0);
			else if(m_b_mouseOnArrow == 2)
				ScrollDown(0);
			else if(m_b_mouseOnArrow == 3)
				DoPtInSlider(pt.x, pt.y);
			else if(m_b_mouseOnArrow == 4 && 
							PointInRect(pt, left+b_BARWIDTH, 
							bottom-b_BARWIDTH, 
							left+b_BARWIDTH+GetSliderOff(0), bottom))
				PageUp(0);
			else if(m_b_mouseOnArrow == 5 && 
						PointInRect(pt, 
						left+b_BARWIDTH+GetSliderOff(0)+GetSliderWid(0)-1, 
						bottom-b_BARWIDTH, right-b_BARWIDTH-1, bottom))
				PageDown(0);
		}
		m_iMoveCount = 0;
	}
}

/////////////////////////////////////////////////////////////////////
//功能    : 鼠标其他事件处理函数
//参数    : 鼠标横坐标, 鼠标纵坐标
//返回值  : 
//注意事项: 
/////////////////////////////////////////////////////////////////////
void	CScrollWnd::DoPenOther(UINT32 nMessage, 
							   UINT32 wParam, UINT32 lParam)
{
	CDC dc;
	CPoint	pt(HIWORD(wParam), LOWORD(wParam));
	short	left, right, top, bottom;

	left = m_sRect.left;
	top  = m_sRect.top;

	if(m_mouseOnArrow == 6)
	{
		OnClientEvent(nMessage, pt.x, pt.y);
		m_mouseOnArrow = 0;
		m_b_mouseOnArrow = 0;
		return;
	}

	if(BARWIDTH)
	{
		right = m_sRect.right;
		bottom = m_sRect.bottom-b_BARWIDTH;
		if(m_mouseOnArrow == 1)
			dc.InvertRect(right - BARWIDTH, top+1, 
									right-1, top +BARWIDTH-1);
		else if(m_mouseOnArrow == 2)
			dc.InvertRect(right - BARWIDTH, bottom - BARWIDTH, 
									right-1, bottom - 1);
		else if(m_mouseOnArrow == 3 || 
					m_mouseOnArrow == 4 || 
					m_mouseOnArrow == 5)
		{
			m_mouseOnArrow = 0;
			DrawScroll();
		}
	}
	if(b_BARWIDTH)
	{
		right = m_sRect.right-BARWIDTH;
		bottom = m_sRect.bottom;
		if(m_b_mouseOnArrow == 1)
			dc.InvertRect(left+1, bottom-b_BARWIDTH, 
								left+b_BARWIDTH-1, bottom-1);
		else if(m_b_mouseOnArrow == 2)
			dc.InvertRect(right-b_BARWIDTH+1, bottom-b_BARWIDTH , 
								right-1, bottom-1);
		else if(m_b_mouseOnArrow == 3 || 
						m_b_mouseOnArrow == 4 ||
						m_b_mouseOnArrow == 5)
		{
			m_b_mouseOnArrow = 0;
			DrawScroll();
		}
	}
	m_mouseOnArrow = 0;
	m_b_mouseOnArrow = 0;
}

/////////////////////////////////////////////////////////////////////
//功能    : 处理函数
//参数    : 消息类型,  参数一, 参数二
//返回值  : 
//注意事项: 
/////////////////////////////////////////////////////////////////////
void	CScrollWnd::WindowProcess(UINT32 nMessage, 
								  UINT32 wParam, UINT32 lParam)//-
{
	switch(nMessage)
	{
		case WM_PENDOWN:
			DoPenDown(nMessage, wParam, lParam);
			break;
		case WM_PENMOVE:
			DoPenMove(nMessage, wParam, lParam);
			break;
		default:
			DoPenOther(nMessage, wParam, lParam);
			break;
	}
}

/////////////////////////////////////////////////////////////////////
//功能    : 画滚动条
//参数    : 
//返回值  : 
//注意事项: 
/////////////////////////////////////////////////////////////////////
void CScrollWnd::DrawScroll()			//
{
	short		  left, top, right, bottom, distan, ypos;
	CDC dc;

	left = m_sRect.left;
	top  = m_sRect.top;

	if(BARWIDTH)
	{
		right = m_sRect.right;
		bottom = m_sRect.bottom-b_BARWIDTH;

		dc.SetBackColor(GRAY);
		dc.EraseRect(right - BARWIDTH, top + BARWIDTH, 
							right-1, bottom - BARWIDTH - 1);

		if(!m_mouseOnArrow)
		{
			dc.EraseRect(right - BARWIDTH, top+1, 
								right-1, top +BARWIDTH-1);
			CRect my_rect6(right - BARWIDTH + 1, top+1, 
								right-1, top +BARWIDTH-1); 
			Draw3DFrame(my_rect6);
			DrawJiant(right - BARWIDTH + 1, top+1, 
								right-1, top +BARWIDTH-1, 1);

			dc.EraseRect(right - BARWIDTH, bottom - BARWIDTH, 
								right-1, bottom - 1);
			CRect my_rect7(right - BARWIDTH + 1, bottom - BARWIDTH, 
								right-1, bottom - 1); 
			Draw3DFrame(my_rect7);
			DrawJiant(right - BARWIDTH + 1, bottom - BARWIDTH, 
								right-1, bottom - 1, 2);
		}

		if(m_mouseOnArrow == 4)
		{
			dc.SetBackColor(0);
			dc.EraseRect(right - BARWIDTH+1, top + BARWIDTH, 
					right-1, top + BARWIDTH + GetSliderOff(1));
		}
		else if(m_mouseOnArrow == 5)
		{
			dc.SetBackColor(0);
			dc.EraseRect(right-BARWIDTH+1, 
					top+BARWIDTH+GetSliderOff(1)+GetSliderWid(1)-2, 
					right-1, bottom - BARWIDTH - 2);
		}
		CRect my_rect2 (right-BARWIDTH+1, 
					top+GetSliderOff(1)+BARWIDTH, right-1, 
					top+GetSliderOff(1)+BARWIDTH+GetSliderWid(1)-1); 
		Draw3DFrame(my_rect2);
	}
	if(b_BARWIDTH)
	{
		right = m_sRect.right-BARWIDTH;
		bottom = m_sRect.bottom;

		dc.SetBackColor(LGRAY);
		dc.EraseRect(left+b_BARWIDTH, bottom-b_BARWIDTH, 
							right-b_BARWIDTH, bottom- 1);

		if(!m_b_mouseOnArrow)
		{
			dc.EraseRect(left+1, bottom-b_BARWIDTH, 
							left +b_BARWIDTH-1, bottom-1);
			CRect my_rect6(left+1, bottom-b_BARWIDTH+1, 
							left +b_BARWIDTH-1, bottom-1); 
			Draw3DFrame(my_rect6);
			DrawJianth(left+1, bottom-b_BARWIDTH+1, 
							left +b_BARWIDTH-1, bottom-1, 1);

			dc.EraseRect(right-b_BARWIDTH-1, bottom-b_BARWIDTH, 
							right-1, bottom-1);
			CRect my_rect7(right-b_BARWIDTH+1, bottom-b_BARWIDTH+1, 
							right-1, bottom-1); 
			Draw3DFrame(my_rect7);
			DrawJianth(right-b_BARWIDTH+1, bottom-b_BARWIDTH+1, 
							right-1, bottom-1, 2);
		}

		if(m_b_mouseOnArrow == 4)
		{
			dc.SetBackColor(0);
			dc.EraseRect(left+b_BARWIDTH+1, bottom-b_BARWIDTH, 
							left+b_BARWIDTH+GetSliderOff(0)-1, 
							bottom);
		}
		else if(m_b_mouseOnArrow == 5)
		{
			dc.SetBackColor(0);
			dc.EraseRect(
				left+b_BARWIDTH+GetSliderOff(0)+GetSliderWid(0)-1, 
				bottom-b_BARWIDTH, right-b_BARWIDTH-1, bottom);
		}
		CRect my_rect2(left+GetSliderOff(0)+b_BARWIDTH, 
				bottom-b_BARWIDTH+1, 
				left+GetSliderOff(0)+b_BARWIDTH+GetSliderWid(0)-1, 
				bottom-1); 
		Draw3DFrame(my_rect2);
	}

}

/////////////////////////////////////////////////////////////////////
//功能    : 绘制函数
//参数    : 
//返回值  : 
//注意事项: 
/////////////////////////////////////////////////////////////////////
void CScrollWnd::OnPaint(CDC &dc)
{
//	CDC dcc;
	DrawScroll();
	//Draw frame
	dc.SetBackColor(0);
	CRect r(m_sRect.left-1 , m_sRect.top -1, m_sRect.right , m_sRect.bottom );
	if(r.left < 0) r.left = 0;
	if(r.top < 0) r.top = 0;
	dc.FrameRect(r);//modified by hqf
	//Draw end 

}
