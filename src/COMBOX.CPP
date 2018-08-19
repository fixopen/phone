/******************************************************************************/
/*                                                                            */
/*  版权所有(c)  2001汉王科技                                                 */
/*                                                                            */
/******************************************************************************/
/*  文 件 名                                                                  */
/*             combox.cpp                                                     */
/*                                                                             */
/*  版 本 号                                                                  */
/*             1.0                                                            */
/*                                                                            */
/*  描    述                                                                  */
/*             汉王科技E-PhoneII窗口类库的头文件                              */
/*             HanWang Technology E-PhoneII Function Class Library source file*/
/*                                                                            */
/*  组    成                                                                  */
/*             组合框控件类源文件                                             */
/*                                                                            */
/*  作    者                                                                  */
/*             王飞                                                           */
/*                                                                            */
/*  日    期                                                                  */
/*             2001-07-01                                                     */
/*                                                                            */
/*  历史记录                                                                  */
/*             2001-08-07  修改发送CN_UNDROPDOWN消息的BUG                     */
/*                                                                            */
/*             2001-08-09  修改甘辉测试的BUG DeleteItem                       */
/*                         DeleteAllItem                                      */
/*                                                                            */
/*             2001-08-23  修改隐藏列表时的一个BUG                            */
/*                         修改Paint列表项时的一个BUG                         */
/*                                                                            */
/*             2001-08-24  增消息响应时对WS_DISABLE风格支持                   */
/*                         修改PopUpList弹出时的条件判断                      */
/*                                                                            */
/*             2001-08-27  增加接口SetCurSel                                  */
/*                         增加接口GetCurSel                                  */
/*                                                                            */
/*             2001-10-16  CPopUpList接口SetCurSel                            */
/*                                                                            */
/*             2001-10-22  按郝庆丰的方法修改CCombox的OnPaint和WndProcess     */
/*                                                                            */
/*             2001-11-05  修改 CPopUpList::SetCurSel(byw)                    */
/*                                                                            */
/*             2001-12-06  修改 CPopUpList::DoDactive(), byw                  */
/*                                                                            */
/*			   2002-1-9    修改 CPopUpList::DoDactive(), hqf                  */
/*                                                                            */
/*                                                                            */
/*                                                                            */
/******************************************************************************/

#include "combox.h"  
#include "datadrv.h"	
/*============================================================================*/
//  弹出列表类定义                                                            //
/*============================================================================*/
void CPopUpList::DoPenDown(CPoint &pt) 
{
	 CRect sRect;
	 INT32   nPenItem = GetPenItem(pt, sRect);
	 if(nPenItem >=0 && nPenItem != m_nCurSelItem)
	 {
		 if(m_nCurSelItem >= 0)
		 {
			 CDC::InvertRect(m_sSelRect); 
		 }
		 m_nCurSelItem = nPenItem;
		 m_sSelRect = sRect;
		 CDC::InvertRect(m_sSelRect); 
	 }
}
////////////////////////////////////////////////////////////////////////////////
//
void CPopUpList::DoPenUp()
{
  if(m_nCurSelItem >= 0)
	{ 
		CDC::InvertRect(m_sSelRect); 
		HideList();
		if(m_pBuddy)
			m_pBuddy->SendMessage(CN_LISTSEL, m_nCtrlId, UINT32(m_nCurSelItem));
		else
		    SendNotifyMessage(CN_LISTSEL, m_nCurSelItem); 
		m_nCurSelItem = -1;
	}
}
////////////////////////////////////////////////////////////////////////////////
//
void CPopUpList::DoDactive(UINT32 wParam)
{
	if(m_nCurSelItem >= 0)
	{ 
		//CDC::InvertRect(m_sSelRect); 
		m_nCurSelItem = -1;
	}
	if(!bSbUnCreated && ((CScrollBar *)wParam == &m_oScrollBar))
	{
       CDC::InvertRect(m_sSelRect);
	   m_sSelRect.SetRect(0,0, 0,0);
	   return;
	}
	if((CPopUpList *)wParam == this)
		return;
    HideList(); 
	
}
////////////////////////////////////////////////////////////////////////////////
//
void CPopUpList::WindowProcess(UINT32 nMessage, UINT32 wParam, UINT32 lParam)
{
	switch(nMessage)
	{
	case WM_PENDOWN:
		DoPenDown(CPoint(HIWORD(wParam), LOWORD(wParam)));
		break;
	case WM_PENMOVE:
		DoPenDown(CPoint(HIWORD(wParam), LOWORD(wParam)));
		break;
	case WM_PENUP:
		DoPenUp();
		break;
	case WM_DACTIVE:
		DoDactive(wParam);
		break;
	case CN_SCROLLTO:
		{
		  INT32 nDispRow = LOWORD(lParam);
		  if(m_nDispRow != nDispRow)
		  {
		    m_nDispRow = nDispRow;
            PaintItems(CDC());
		  }
		}
		break;
	}
}
void CPopUpList::SetCurSel(INT32 nSelNumber)
{
	if(nSelNumber>= m_nItems - m_nScreenItems)
		 m_nDispRow = m_nItems - m_nScreenItems;
	else 
		 m_nDispRow = nSelNumber;
	if (m_nDispRow < 0)
		m_nDispRow = 0;

	if(!bSbUnCreated)
		m_oScrollBar.SetPerameter(m_nItems, m_nScreenItems, m_nDispRow);
}
////////////////////////////////////////////////////////////////////////////////
//
void CPopUpList::OnPaint(CDC &dc)
{
	dc.EraseRect(m_sRect);
	dc.FrameRect(m_sRect); 
    PaintItems(dc);
}

////////////////////////////////////////////////////////////////////////////////
//
INT32 CPopUpList::GetPenItem(CPoint &pt, CRect &sItemRect)
{
	sItemRect.SetRect(m_sRect.left+1, m_sRect.top, 
		              m_sRect.right-1, m_sRect.top + LIST_ITEM_HEIGHT-1);
	INT32 maxi = MIN(m_nScreenItems + m_nDispRow, m_nItems);
	for(INT32 i=m_nDispRow; i< maxi; i++)
	{
        if(pt.InRect(sItemRect))
		{
			sItemRect.DeflateRect(1, 1);
			return i;
		}
		sItemRect.top = sItemRect.bottom +1;
		sItemRect.bottom = sItemRect.top + LIST_ITEM_HEIGHT-1;
	}
	return -1;
}
////////////////////////////////////////////////////////////////////////////////
//
void CPopUpList::PaintItems(CDC &dc)
{
	CRect r = m_sRect;
//	r.DeflateRect(1, 1);
	r.bottom = r.top + LIST_ITEM_HEIGHT-1;
	for(INT32 i=m_nDispRow; i< m_nScreenItems+m_nDispRow; i++)
	{
    	PaintItem(i, r, dc);
		r.top = r.bottom +1;
		r.bottom = r.top + LIST_ITEM_HEIGHT-1;
	}
}
////////////////////////////////////////////////////////////////////////////////
//
void CPopUpList::PaintItem(INT32 nItemNumber, CRect &r, CDC &dc)
{
	CRect rec = r;
	if(nItemNumber >= m_nItems)
		return;
//	dc.EraseRect(rec);
	rec.DeflateRect(1,1); 
	dc.EraseRect(rec);
	if(m_pListItems[nItemNumber].nIconId)
	{
	 dc.PutIcon(rec.left+1, rec.top, m_pListItems[nItemNumber].nIconId);
	 rec.left += gICON_WIDTH(m_pListItems[nItemNumber].nIconId) + 2;
	}
	dc.DrawText(rec, m_pListItems[nItemNumber].strText); 
}
////////////////////////////////////////////////////////////////////////////////
//
void CPopUpList::Destroy()
{
	delete[] m_pListItems;
	CCtrl::Destroy(); 
}
////////////////////////////////////////////////////////////////////////////////
//
BOOL CPopUpList::Create(CRect &ctrRect, CFrameWnd *pParent, CCtrl *pBuddyCtrl, UINT32 nCtrlId)
{
	if(!CCtrl::Create(NULL, ctrRect, pParent, WS_CHILD, nCtrlId))
		return FALSE;

	m_pBuddy       = pBuddyCtrl;
	m_nItems       = m_nDispRow =  m_nMemoryItemCount = 0;
	m_nScreenItems = ctrRect.Height()/LIST_ITEM_HEIGHT;
	m_nCurSelItem  = -1;
	m_pListItems   = NULL;
	bSbUnCreated   = TRUE;
    m_sBkScrRect   = m_sRect;    
	m_sSelRect.SetRect(0,0, 0,0);
	return TRUE;
}
////////////////////////////////////////////////////////////////////////////////
//
BOOL CPopUpList::AppendItem(char *strText, UINT32 nIconId, UINT32 exData)
{
	if(m_nItems >= m_nMemoryItemCount)
	{
		CPopListItem * pTemp = m_pListItems;
		m_pListItems = new CPopListItem[m_nMemoryItemCount + 50];
		if(!m_pListItems)
		{
			m_pListItems = pTemp;
			return FALSE;
		}
		m_nMemoryItemCount += 50;
		memcpy(m_pListItems, pTemp, sizeof(CPopListItem) * m_nItems);
		delete[] pTemp;
	}

	if(strText)
	  hwstrncpy(m_pListItems[m_nItems].strText, strText, LIST_ITEM_LENGTH-1); 
	else
      m_pListItems[m_nItems].strText[0] = 0; 

	m_pListItems[m_nItems].nIconId = nIconId;
	m_pListItems[m_nItems++].exData  = exData;
    
    if(m_nItems > m_nScreenItems && bSbUnCreated)
    {
	   m_sRect.right -= SB_WIDTH;
	   if(!m_oScrollBar.Create(CPoint(m_sRect.right+1, m_sRect.top), m_sRect.Height(), (CFrameWnd*)m_pParent, this))
		   return FALSE;
	   m_oScrollBar.SetWindowStyle(WS_CHILD); 
	   bSbUnCreated = FALSE;
    }
    if(!bSbUnCreated)
		m_oScrollBar.SetPerameter(m_nItems, m_nScreenItems, m_nDispRow);
	
    return TRUE;
}
////////////////////////////////////////////////////////////////////////////////
//
BOOL CPopUpList::PopUpList(INT16 x, INT16 y)
{
	m_sBkScrRect.MoveTo(x, y); 
 	if(m_sBkScrRect.right > m_pParent->GetWindowRect().right || m_sBkScrRect.bottom > m_pParent->GetWindowRect().bottom)
		return FALSE;
	if(!(m_pScrBuf = CDC::SaveScreen(m_sBkScrRect)))
		return FALSE;

    SendNotifyMessage(CN_DROPDOWN, 0);
    m_sRect.MoveTo(x, y);
	if(!bSbUnCreated)
	{
	  m_oScrollBar.Moveto(m_sBkScrRect.right-SB_WIDTH+1, y);
	  m_oScrollBar.SetWindowStyle(m_oScrollBar.GetWindowStyle() | WS_VISIBLE);
	  ((CFrameWnd *)m_pParent)->DeleteChild(&m_oScrollBar); 
	  ((CFrameWnd *)m_pParent)->AddChild(&m_oScrollBar); 
	  m_oScrollBar.OnPaint(CDC());
	}

	m_dwStyle |= WS_VISIBLE; 
	((CFrameWnd *)m_pParent)->m_pCurCtrl = this;  
	((CFrameWnd *)m_pParent)->DeleteChild(this); 
	((CFrameWnd *)m_pParent)->AddChild(this); 
	
    Paint();
	return TRUE;
}
////////////////////////////////////////////////////////////////////////////////
//
void CPopUpList::HideList()
{
	if(m_dwStyle & WS_VISIBLE)
	{
		if(!bSbUnCreated)
		  m_oScrollBar.SetWindowStyle(m_oScrollBar.GetWindowStyle() & ~WS_VISIBLE);

		m_dwStyle &= ~WS_VISIBLE;
		CDC::PutScreen(m_pScrBuf); 
		((CFrameWnd *)m_pParent)->m_pCurCtrl = NULL;
		SendNotifyMessage(CN_UNDROPDOWN, 0);
	}
}
////////////////////////////////////////////////////////////////////////////////
//
char * CPopUpList::GetListItemText(INT32 nItemNumber)
{
	if(nItemNumber <0 || nItemNumber >= m_nItems)
		return NULL;
	return m_pListItems[nItemNumber].strText;
}
////////////////////////////////////////////////////////////////////////////////
//
BOOL CPopUpList::DeleteItem(INT32 nItemNumber)
{
	if(nItemNumber < 0 || nItemNumber >= m_nItems)
		return FALSE;
	memmove(&m_pListItems[nItemNumber], &m_pListItems[nItemNumber+1], sizeof(CPopListItem) * (m_nItems-nItemNumber-1));
	m_nItems --;

	if(m_nItems <= m_nScreenItems && !bSbUnCreated)
	{
		m_oScrollBar.Destroy();
		bSbUnCreated  = 1;
		m_sRect.right += SB_WIDTH;
	}
    if(m_nDispRow > m_nItems - m_nScreenItems)
		m_nDispRow = m_nItems - m_nScreenItems;
	if(m_nDispRow<0)
		m_nDispRow = 0;
		

	if(!bSbUnCreated)
		m_oScrollBar.SetPerameter(m_nItems, m_nScreenItems, m_nDispRow);

	return TRUE;
}
////////////////////////////////////////////////////////////////////////////////
//
void CPopUpList::DeleteAllItems()
{
	m_nItems = m_nDispRow =0;
	if(!bSbUnCreated)
	{
		m_oScrollBar.Destroy();
		bSbUnCreated = 1;
		m_sRect.right += SB_WIDTH;
	}
}
////////////////////////////////////////////////////////////////////////////////
//
CPopListItem * CPopUpList::GetItem(INT32 nItemNumber)
{
	if(nItemNumber < 0 || nItemNumber >= m_nItems)
		return NULL;

	return &m_pListItems[nItemNumber];
}
////////////////////////////////////////////////////////////////////////////////
//
BOOL CPopUpList::SetItem(INT32 nItemNumber, CPopListItem * pItemBuf)
{
	if(nItemNumber < 0 || nItemNumber >= m_nItems)
		return FALSE;
    memcpy(&m_pListItems[nItemNumber], pItemBuf, sizeof(CPopListItem));
	return TRUE;
}
/*============================================================================*/
//  组合框类定义                                                              //
/*============================================================================*/
void CCombox::WindowProcess(UINT32 nMessage, UINT32 wParam, UINT32 lParam)
{
	if(m_dwStyle & WS_DISABLE)
		return;
	switch(nMessage)
    {
    case WM_PENMOVEIN:    case WM_PENDOWN:    case WM_PENMOVE:
        if(!m_bPressed)
        {
         CDC::InvertRect (m_sRect);
         m_bPressed = 1;
        }
        break;
	case WM_ACTIVE:
		{
		if(wParam == (UINT32)&m_oPopList)
			m_bHasDropDown = TRUE;
		else
			m_bHasDropDown = FALSE;
		}
		break;
    case WM_PENMOVEOUT:
        if(m_bPressed)
        {
         CDC::InvertRect (m_sRect);
         m_bPressed = 0;
        }
         break;
    case WM_PENUP:
        if(m_bPressed)
        {
         CDC::InvertRect (m_sRect); 
         m_bPressed = 0;
         ShowDropDown(!m_bHasDropDown);
        }
        break; 
	case CN_LISTSEL:
		{
		 SetWindowText(m_oPopList.GetListItemText(INT32(lParam)));
		 m_nCurSel = INT32(lParam);
	     CRect r(m_sRect.left +2, m_sRect.top +2, m_sRect.right - 19, m_sRect.bottom-2);
		 CDC dc;
		 dc.EraseRect(r);  
	 	 UINT32 nIconID;
		 if((nIconID = GetItem(m_nCurSel)->nIconId) > 0)
		 {
			dc.PutIcon(r.left , r.top , nIconID);
			r.left += gICON_WIDTH(nIconID);
		 }
	     dc.DrawText(r, m_strName); 
		 SendNotifyMessage(CN_COMMAND, lParam);
		}
		break;
	}
}
////////////////////////////////////////////////////////////////////////////////
//
void CCombox::ShowDropDown(BOOL bShowIt)
{
	if(bShowIt)
	{
		 if(!m_oPopList.PopUpList(m_sRect.left, m_sRect.bottom+1))
		 {
		     INT16 x = m_sRect.left; 
			 INT16 y = m_sRect.top - m_oPopList.GetWindowRect().Height();
			 m_oPopList.PopUpList(x, y); 
		 }
	}
	else
	{
      m_oPopList.HideList(); 
	}
}
////////////////////////////////////////////////////////////////////////////////
//
void CCombox::OnPaint(CDC &dc) 
{
    dc.EraseRect(m_sRect);
	dc.FrameRect(m_sRect);
	CRect r(m_sRect.left +1, m_sRect.top +1, m_sRect.right - 17, m_sRect.bottom-1);
	CRect r1(r.right+1, m_sRect.top+1, m_sRect.right-1, m_sRect.bottom -1);  
	dc.SetPenColor(LGRAY); 
	dc.FrameRect(r);
	dc.PaintRect(r1); 
	r.DeflateRect(1,1);  
	dc.SetPenColor();
	CPopListItem * ptemp = GetItem(m_nCurSel); 
	UINT32 nIconID = ptemp? ptemp->nIconId: 0;
	if(nIconID> 0)
	{
		dc.PutIcon(r.left , r.top , nIconID);
		r.left += gICON_WIDTH(nIconID);
	}
	dc.DrawText(r, m_strName); 
	dc.PutIcon(r1.left+2, r1.top+4, ICON_DOWNARROW);  
	dc.Draw3dRect(r1); 
}

////////////////////////////////////////////////////////////////////////////////
//
void CCombox::SetWindowText(const char * strText, INT16 nAlign, BOOL bReDraw)
{
	CDC		dc;
	if ((nAlign != HALIGN_RIGHT) || !bReDraw)
		CCtrl::SetWindowText(strText);
	else {
		INT16 len = dc.GetTextExtent((char*)strText);
		if (len > m_sRect.Width() - gICON_WIDTH(ICON_DOWNARROW) - 3)
			dc.SetAlign(HALIGN_RIGHT);
		strncpy(m_strName , strText , WND_TITLE_LENGTH -1);
		m_strName[WND_TITLE_LENGTH- 1] = '\0';
		OnPaint(dc);
	}
}	

////////////////////////////////////////////////////////////////////////////////
//
BOOL CCombox::Create(char *strText, CRect &ctrRect, INT16 nDropListHeight, CFrameWnd *pParent, UINT32 nCtrlId)
{
	if(!pParent)
		return FALSE;
	ctrRect.bottom = ctrRect.top + COMBOX_HEIGHT -1;

	if(!CCtrl::Create(strText, ctrRect, pParent, WS_CHILD|WS_VISIBLE, nCtrlId))
		return FALSE;
   
	CRect r(0,0, ctrRect.Width()-1, nDropListHeight -1); 
	if(!m_oPopList.Create(r, pParent, this, nCtrlId))
		return FALSE;

	m_bHasDropDown  = FALSE;
	m_bPressed = FALSE;
	m_nCurSel  = 0;
    return TRUE;
}
////////////////////////////////////////////////////////////////////////////////
//
BOOL CCombox::AppendItem(char *strText, UINT32 nIconId, UINT32 exData)
{
	return m_oPopList.AppendItem(strText, nIconId, exData); 
}
////////////////////////////////////////////////////////////////////////////////
//
BOOL CCombox::DeleteItem(INT32 nItemNumber)
{
	return m_oPopList.DeleteItem(nItemNumber); 
}
////////////////////////////////////////////////////////////////////////////////
//
void CCombox::DeleteAllItems()
{
	m_oPopList.DeleteAllItems(); 
}
////////////////////////////////////////////////////////////////////////////////
//
CPopListItem * CCombox::GetItem(INT32 nItemNumber)
{
	return m_oPopList.GetItem(nItemNumber); 
}
////////////////////////////////////////////////////////////////////////////////
//
BOOL CCombox::SetItem(INT32 nItemNumber, CPopListItem * pItemBuf)
{
	return m_oPopList.SetItem(nItemNumber, pItemBuf); 
}
////////////////////////////////////////////////////////////////////////////////
//
void CCombox::SetCurSel(INT32 nItemNumber)
{
	if(m_oPopList.GetItem(nItemNumber))
	{
	 m_nCurSel = nItemNumber;
	 SetWindowText(m_oPopList.GetItem(m_nCurSel)->strText);
	 m_oPopList.SetCurSel(nItemNumber); 
	}
}
////////////////////////////////////////////////////////////////////////////////
//
INT32 CCombox::GetCurSel()
{
	return m_nCurSel;
}
////////////////////////////////////////////////////////////////////////////////
// end combox.cpp
