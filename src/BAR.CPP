/******************************************************************************/
/*                                                                            */
/*  版权所有(c)  2001汉王科技                                                 */
/*                                                                            */
/******************************************************************************/
/*  文 件 名                                                                  */
/*             scrollbar.cpp                                                  */
/*                                                                            */
/*  版 本 号                                                                  */
/*             1.0                                                            */
/*                                                                            */
/*  描    述                                                                  */
/*             汉王科技E-PhoneII窗口类库的源文件                              */
/*             HanWang Technology E-PhoneII Function Class Library source file*/
/*                                                                            */
/*  组    成                                                                  */
/*             滚动条控件类源文件                                             */
/*                                                                            */
/*  作    者                                                                  */
/*             王飞                                                           */
/*                                                                            */
/*  日    期                                                                  */
/*             2001-07-02                                                     */
/*                                                                            */
/*  历史记录                                                                  */
/******************************************************************************/
#include "scrollbar.h"

////////////////////////////////////////////////////////////////////////////////
//  滚动条类相关常量定义 
#define SB_ACTIVE_UP         0x01
#define SB_ACTIVE_DOWN       0x02
#define SB_ACTIVE_SLED       0x03
#define SB_ACTIVE_OTHERUP    0x04
#define SB_ACTIVE_OTHERDOWN  0x05
#define SLED_MOVE_SPACE         6
#define SB_CLIENT_HEIGHT     (m_sDownRect.top - m_sUpRect.bottom -1)

////////////////////////////////////////////////////////////////////////////////
//
void CScrollBar::NotifyBuddy()
{
    if(m_pBuddyCtrl)
		 m_pBuddyCtrl->SendMessage(CN_SCROLLTO, m_nCtrlId, MAKELONG(m_iCurRow, m_iRows));
    else
        m_pParent->SendMessage(CN_SCROLLTO, m_nCtrlId, MAKELONG(m_iCurRow, m_iRows));
}

////////////////////////////////////////////////////////////////////////////////
//
void CScrollBar::DoUpActive()
{
  if(m_iActive != SB_ACTIVE_UP)
  {
    CDC::InvertRect(m_sUpRect);
    m_iActive = SB_ACTIVE_UP;
  }
  if(m_iCurRow >0)
        m_iCurRow--;
}

////////////////////////////////////////////////////////////////////////////////
//
void CScrollBar::DoDownActive()
{

  if(m_iActive != SB_ACTIVE_DOWN)
  {
      CDC::InvertRect(m_sDownRect);
      m_iActive = SB_ACTIVE_DOWN;
  }
  if((m_iRows > m_iScreenRows) && (m_iCurRow < m_iRows - m_iScreenRows))
        m_iCurRow++;
}

////////////////////////////////////////////////////////////////////////////////
//
void CScrollBar::DoOtherUpActive()
{
   if(m_iActive == SB_ACTIVE_UP)
       CDC::InvertRect(m_sUpRect);

   if(m_iActive == SB_ACTIVE_DOWN)
       CDC::InvertRect(m_sDownRect);

  m_iActive = SB_ACTIVE_OTHERUP;
  m_iCurRow = (m_iCurRow >(m_iScreenRows-1)) ? (m_iCurRow-m_iScreenRows+1) : 0;
}

////////////////////////////////////////////////////////////////////////////////
//
void CScrollBar::DoOtherDownActive()
{
  if(m_iActive == SB_ACTIVE_UP)
       CDC::InvertRect(m_sUpRect);

  if(m_iActive == SB_ACTIVE_DOWN)
       CDC::InvertRect(m_sDownRect);

  m_iActive = SB_ACTIVE_OTHERDOWN;
  if(m_iCurRow < (m_iRows - m_iScreenRows *2 +1))
      m_iCurRow += m_iScreenRows-1;
  else 
      m_iCurRow = m_iRows - m_iScreenRows;
}

////////////////////////////////////////////////////////////////////////////////
//
void CScrollBar::DoPenDown(CPoint &pt)
{
   if(pt.InRect(m_sSledRect)) 
   {
      m_iActive = SB_ACTIVE_SLED;
      m_iLastPenY = pt.y;
      return;
   } 
  if(pt.InRect(m_sUpRect))
    DoUpActive();
  else if(pt.InRect(m_sDownRect))
    DoDownActive();
  else if(pt.y < m_sSledRect.top)
    DoOtherUpActive();  
  else 
    DoOtherDownActive();
  
  CalcSledPos();
  PaintSled(CDC());
  NotifyBuddy();
} 

////////////////////////////////////////////////////////////////////////////////
//
void CScrollBar::DoSledMove(UINT16 iPenY)
{
   INT16 space = iPenY - m_iLastPenY;
   UINT16 height = m_sSledRect.Height();
   UINT16 maxy0 =  m_sDownRect.top - height;
 
   if(m_sSledRect.top+space <= m_sUpRect.bottom)
   {
       m_sSledRect.top    = m_sUpRect.bottom;
       m_sSledRect.bottom = m_sSledRect.top+height-1;
   }
   else if(m_sSledRect.top+space >= maxy0)
   {
       m_sSledRect.top    = maxy0;
       m_sSledRect.bottom = m_sDownRect.top -1; 
   }
   else 
   {
       m_sSledRect.top    += space;
       m_sSledRect.bottom += space;
   }

   PaintSled(CDC());
   m_iLastPenY  = iPenY;

   if(SB_CLIENT_HEIGHT - height+1)
      m_iCurRow = (m_sSledRect.top - m_sUpRect.bottom) *  \
		          (m_iRows-m_iScreenRows) / (SB_CLIENT_HEIGHT - height+1);
   else
      m_iCurRow =0; 

   if(m_iCurRow > m_iRows - m_iScreenRows && m_iScreenRows < m_iRows)
            m_iCurRow = m_iRows - m_iScreenRows;
   static count=0;
   if(count++>3)
   {
    NotifyBuddy();
	count =0;
   }

}

////////////////////////////////////////////////////////////////////////////////
//

void CScrollBar::DoPenMove(CPoint &pt) 
{
  static int movecount=0;
  if(pt.InRect(m_sSledRect))
  {
     if(m_iActive == SB_ACTIVE_SLED)
        DoSledMove(pt.y);
     return;
  } 

  if(movecount++ > MOVE_FILTER_NUM && m_iActive != SB_ACTIVE_SLED )
  {
      if(pt.InRect(m_sUpRect))
         DoUpActive();
      else if(pt.InRect(m_sDownRect))
         DoDownActive();
      else if(pt.y < m_sSledRect.top)
         DoOtherUpActive();  
      else 
         DoOtherDownActive();

    CalcSledPos();
    PaintSled(CDC());
    NotifyBuddy();
	movecount =0; 
  }
}
////////////////////////////////////////////////////////////////////////////////
//
void CScrollBar::DoPenUp()
{
   if(!(m_dwStyle & WS_VISIBLE))  // hqf 12/18
	   return;
   if(m_iActive == SB_ACTIVE_UP)
       CDC::InvertRect(m_sUpRect);

   if(m_iActive == SB_ACTIVE_DOWN)
       CDC::InvertRect(m_sDownRect);

   if(m_iActive == SB_ACTIVE_SLED)
   {
       CalcSledPos();
       PaintSled(CDC());
       NotifyBuddy();
   }
    m_iActive = 0;
}

////////////////////////////////////////////////////////////////////////////////
//
void CScrollBar::WindowProcess(UINT32 nMessage, UINT32 wParam, UINT32 lParam)
{
    switch(nMessage)
    {
    case WM_PENDOWN:
        DoPenDown(CPoint(HIWORD(wParam), LOWORD(wParam)));
        break;
    case WM_PENMOVE:
        DoPenMove(CPoint(HIWORD(wParam), LOWORD(wParam)));
        break;
    case WM_PENUP:
        DoPenUp();
        break;
    case WM_DACTIVE: //hqf /12/18
    case WM_PENMOVEOUT:
		{
		  DoPenUp();
		  if(m_pBuddyCtrl)
	     	m_pBuddyCtrl->SendMessage(nMessage, wParam, lParam);
//		  if(m_pParent)
//			m_pParent->SendMessage(nMessage, wParam, lParam);
			 
		}
        break;
    }
}

////////////////////////////////////////////////////////////////////////////////
//
void CScrollBar::PaintUpDownBut(CDC &dc, CRect &scrRect, BOOL bUp)
{
    UINT32 iconid = bUp ? ICON_UPARROW : ICON_DOWNARROW;
    dc.PutIcon(scrRect.left+1, scrRect.top+2, iconid);
    dc.Draw3dRect(scrRect);
} 

////////////////////////////////////////////////////////////////////////////////
//
void CScrollBar::PaintSled(CDC &dc)
{
    CRect rec(m_sRect);
    rec.top    = m_sUpRect.bottom+1;
    rec.bottom = m_sDownRect.top-1;
    rec.right --;
    dc.SetPenColor(LGRAY);
    dc.PaintRect(rec);
    rec =  m_sSledRect;
	rec.DeflateRect(1,1); 
    rec.right --;
    dc.Draw3dRect(rec);
    rec.DeflateRect(1,1); 
    dc.SetPenColor(GRAY);
    dc.PaintRect(rec);
}

////////////////////////////////////////////////////////////////////////////////
//
void CScrollBar::OnPaint(CDC &dc)
{
   dc.SetPenColor(LGRAY);
   dc.PaintRect(m_sRect);
   PaintUpDownBut(dc, m_sUpRect); 
   PaintUpDownBut(dc, m_sDownRect, FALSE);
   PaintSled(dc);
}

////////////////////////////////////////////////////////////////////////////////
//
void CScrollBar::CalcSledPos()
{
    UINT16 S0=0, S1=0;
    if(!m_iScreenRows)
        return;

    if(m_iCurRow > m_iRows - m_iScreenRows)
        m_iCurRow = m_iRows - m_iScreenRows;

    if(m_iRows <= m_iScreenRows)
        m_iCurRow =0;

    UINT16 SP1 = (m_iRows>m_iScreenRows+m_iCurRow) ? (m_iRows-m_iScreenRows-m_iCurRow) : 0;
    UINT16 sbClientHeight = SB_CLIENT_HEIGHT;

    UINT16 XiSu1 =1, XiSu2=1;
    if(m_iScreenRows <= m_iRows)
    {
       XiSu1 = m_iScreenRows;
       XiSu2 = m_iRows;
    }
    UINT16 SledHeight = MAX(SB_SLED_MINHEIGHT, sbClientHeight*XiSu1/XiSu2);
    
    if(m_iCurRow + SP1)
    {
     S0  = m_iCurRow * (sbClientHeight - SledHeight) / (m_iCurRow + SP1);
     S1  = SP1 * (sbClientHeight - SledHeight) / (m_iCurRow + SP1);
    }
    m_sSledRect.left    = m_sRect.left;
    m_sSledRect.right   = m_sRect.right;
    m_sSledRect.top     = m_sUpRect.bottom + S0;
    m_sSledRect.bottom  = m_sDownRect.top - S1-1; 
}

////////////////////////////////////////////////////////////////////////////////
//
void CScrollBar::SetPerameter(UINT16 iRows, UINT16 iScreenRows, 
							                  UINT16 iCurRow, BOOL bReDraw)
{
    if(iScreenRows < 2)
        return;
    //保存三个参数
    m_iRows       = iRows;
    m_iScreenRows = iScreenRows;
    m_iCurRow     = iCurRow; 

    //计算滚动条滑块的大小及位置
    CalcSledPos();

    if(bReDraw)
		PaintSled(CDC());
}

////////////////////////////////////////////////////////////////////////////////
//
void CScrollBar::GetPerameter(UINT16 &iRows, UINT16 &iScreenRows, UINT16 &iCurRow)
{
    iRows       = m_iRows;
    iScreenRows = m_iScreenRows;
    iCurRow     = m_iCurRow; 
}

////////////////////////////////////////////////////////////////////////////////
//
BOOL CScrollBar::Create(CPoint &ptLeftTop, UINT16 iHeight, CFrameWnd * pParent, 
		                                       CCtrl *pBuddyCtrl, UINT32 iScrollId)
{

  if(iHeight < SB_WIDTH*2+ SB_SLED_MINHEIGHT)
        return FALSE;

  CRect  r(ptLeftTop.x, ptLeftTop.y, ptLeftTop.x+SB_WIDTH-1 , ptLeftTop.y+iHeight-1);
  if(!CCtrl::Create(NULL, r, pParent, WS_VISIBLE|WS_CHILD, iScrollId))
		return FALSE;


   m_sUpRect.SetRect(m_sRect.left+1, m_sRect.top+1, 
	                 m_sRect.right-1, m_sRect.top+SB_WIDTH);

   m_sDownRect.SetRect(m_sRect.left+1, m_sRect.bottom-SB_WIDTH, 
	                 m_sRect.right-1, m_sRect.bottom-1);

   m_pBuddyCtrl = pBuddyCtrl;
   m_iRows = m_iScreenRows = 1;
   m_iCurRow = 0; 
   CalcSledPos();
   return TRUE;
}
 
////////////////////////////////////////////////////////////////////////////////
//
void CScrollBar::Moveto(INT16 x, INT16 y)
{
	m_sRect.MoveTo(x, y); 
	m_sUpRect.SetRect(m_sRect.left+1, m_sRect.top+1, 
	                 m_sRect.right-1, m_sRect.top+SB_WIDTH);

   m_sDownRect.SetRect(m_sRect.left+1, m_sRect.bottom-SB_WIDTH, 
	                 m_sRect.right-1, m_sRect.bottom-1);

   CalcSledPos(); 
}

#undef SB_ACTIVE_UP    
#undef SB_ACTIVE_DOWN  
#undef SB_ACTIVE_SLED  
#undef SB_ACTIVE_OTHERUP 
#undef SB_ACTIVE_OTHERDOWN
#undef SLED_MOVE_SPACE

////////////////////////////////////////////////////////////////////////////////
// end scrollbar.cpp