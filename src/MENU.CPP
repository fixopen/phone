/******************************************************************************/
/*                                                                            */
/*  版权所有(c)  2001汉王科技                                                 */
/*                                                                            */
/******************************************************************************/
/*  文 件 名                                                                  */
/*             menu.cpp                                                       */
/*                                                                            */
/*  版 本 号                                                                  */
/*             1.0                                                            */
/*                                                                            */
/*  描    述                                                                   */
/*             汉王科技E-PhoneII窗口类库的头文件                              */
/*             HanWang Technology E-PhoneII Function Class Library head file  */
/*                                                                            */
/*  组    成                                                                  */
/*             菜单控件类源文件                                               */
/*                                                                            */
/*  作    者                                                                  */
/*             王飞                                                           */
/*                                                                            */
/*  日    期                                                                  */
/*             2001-07-01                                                     */
/*                                                                            */
/*  历史记录                                                                  */
/*             2001-07-25   增加接口EnableItem                                */
/*                          增加接口DisableItem                               */
/*                                                                            */
/*             2001-08-08   修改Destroy函数的BUG                              */
/*                          增加checkItem函数                                 */
/*                                                                            */
/*             2001-08-09   修改HideMenu时发送消息的BUG                       */
/*             2001-08-09   修改PopupMenu(郝庆丰)                             */
/******************************************************************************/

#include "menu.h"
/*============================================================================*/
//  CMenuItem类定义                                                           //
/*============================================================================*/
void CMenuItem::PaintItem(CDC &dc)
{
   if(*strText != '-')
   {
		INT16 x = sRect.left + MENU_LEFT_SPACE;
		INT16 y = sRect.top + 1;
		if(nStyle & MS_DISABLE)
		{
			dc.SetBackColor(MENU_BACK_COLOR);
			dc.SetROP2(DRAWMODE_OR); 
            
			if(nIconId > 0) // hqf
			{
				dc.PutIcon(x, y, nIconId);
				x += MENU_ICON_WIDTH + MENU_MID_SPACE;
			}
            dc.SetROP2();
			dc.SetPenColor(GRAY);
    	    dc.TextOut(x, y, strText);
 			dc.SetBackColor(BLACK); 
			dc.SetPenColor(WHITE);
			dc.SetROP2(DRAWMODE_OR); 
			dc.TextOut(x+1, y+1, strText);
			if(nStyle & MS_POPUP)
			{
				x = sRect.right - MENU_ICON_WIDTH;
				dc.PutIcon(x, y, ICON_RIGHTARROW); 
			}
			dc.SetROP2(); 
		}
		else
		{
			if(nIconId > 0) // hqf
			{
				dc.PutIcon(x, y, nIconId);
				x += MENU_ICON_WIDTH;
			}
			dc.SetBackColor(MENU_BACK_COLOR);
			dc.SetPenColor(MENU_FORE_COLOR);
		    
		    dc.TextOut(x, y, strText);
			if(nStyle & MS_POPUP)
			{
				x = sRect.right - MENU_ICON_WIDTH;
				dc.PutIcon(x, y, ICON_RIGHTARROW); 
			}
		}
   }
   else
   {
        dc.SetPenColor(GRAY);
        dc.MoveTo(sRect.left+1, sRect.top);
        dc.LineTo(sRect.right-1, sRect.top);
        dc.SetPenColor(WHITE);
        dc.MoveTo(sRect.left+1, sRect.bottom);
        dc.LineTo(sRect.right-1, sRect.bottom);
   } 
}
 
////////////////////////////////////////////////////////////////////////////////
//
void  CMenuItem::Active()
{
  if(!(nStyle & MS_DISABLE))
  {
	CDC::InvertRect(sRect.left + 1, sRect.top + 1, sRect.right - 1, sRect.bottom - 1);
	if((nStyle & MS_POPUP) && pPopupMenu)
        pPopupMenu->PopupMenu(sRect.right-2, sRect.top);   
  }
}
////////////////////////////////////////////////////////////////////////////////
//
void  CMenuItem::Dactive()
{
  if(!(nStyle & MS_DISABLE))
  {
    if((nStyle & MS_POPUP) && pPopupMenu)
       pPopupMenu->HideMenu();  
	CDC::InvertRect(sRect.left + 1, sRect.top + 1, sRect.right - 1, sRect.bottom - 1);
  }
}
/*============================================================================*/
//  CMenu类定义                                                               //
/*============================================================================*/
 
BOOL CMenu::Create(CFrameWnd * pParent)
{
	m_pFirstItem = m_pLastItem = m_pCurItem = NULL;
	m_pBackScreenBuf = NULL;
	m_pParentMenu    = NULL;
	return CCtrl::Create(NULL, CRect(0,0,0,0), pParent, WS_CHILD); 
}
////////////////////////////////////////////////////////////////////////////////
//
BOOL CMenu::AppendItem(char *strText,  UINT32 nItemID, UINT32 nIconID, UINT8 nStyle, CMenu * pPopUpMenu)
{
    CMenuItem * pItem = new CMenuItem;

	if(pItem == NULL) 	return FALSE;

	pItem->nStyle     = (*strText == '-') ? MS_DISABLE: nStyle;
	pItem->nIconId    = nIconID;
	pItem->nItemId    = nItemID;
	pItem->pPopupMenu = pPopUpMenu;
	pItem->pNextItem  = NULL;

	if(strText != NULL)
	  hwstrncpy(pItem->strText, strText, MENU_ITEM_TEXT_LONG-1);
	else
      pItem->strText[0] = 0;
    
	if(pPopUpMenu)
		pPopUpMenu->m_pParentMenu = this;

	INT16 width = CDC::GetTextExtent(pItem->strText) + MENU_ICON_WIDTH + MENU_LEFT_SPACE*2 + MENU_MID_SPACE*2;
	if(nStyle & MS_POPUP)
		width += MENU_ICON_WIDTH;

	if(width > m_sRect.Width())
	{
		m_sRect.right = MIN(width-1, LCD_WIDTH-1);
		m_sRect.left = 0;
	}

	if(m_pFirstItem == NULL) 
	{
		m_pFirstItem = m_pLastItem = pItem;
	}
	else
	{
		m_pLastItem->pNextItem = pItem;
		m_pLastItem = pItem;
	}
	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
//
BOOL CMenu::AppendItem(char *strText,  UINT32 nItemID, UINT32 nIconID)
{
	AppendItem(strText, nItemID, nIconID, MS_NORMAL, NULL);
}
////////////////////////////////////////////////////////////////////////////////
//
void CMenu::Destroy()
{
   CMenuItem * pItem = m_pCurItem = m_pFirstItem;
   while(m_pCurItem)
   {
	   m_pCurItem = m_pCurItem->pNextItem;
	   delete pItem;
	   pItem = m_pCurItem;
   }
   CCtrl::Destroy(); 
}

////////////////////////////////////////////////////////////////////////////////
//
BOOL CMenu::PopupMenu(INT16 x, INT16 y)
{
	if(!m_pFirstItem)
		return FALSE;
	m_sRect.bottom = m_sRect.top;
	m_sRect.MoveTo(x, y);
	for(CMenuItem * pItem = m_pFirstItem; pItem; pItem = pItem->pNextItem)
	{
	  pItem->sRect.left  = m_sRect.left;
	  pItem->sRect.right = m_sRect.right; 
	  pItem->sRect.top   = m_sRect.bottom;

	  if(* pItem->strText != '-')
        m_sRect.bottom += MENU_ITEM_HEIGHT;
	  else
        m_sRect.bottom += 1;

	  pItem->sRect.bottom = m_sRect.bottom;   
	}

/*	if(m_sRect.right >= LCD_WIDTH || m_sRect.bottom >= LCD_HEIGHT)
		return FALSE;*/
	if(m_sRect.right >= LCD_WIDTH)
		m_sRect.right = LCD_WIDTH -1;
	if(m_sRect.bottom >= LCD_HEIGHT)
		m_sRect.bottom = LCD_HEIGHT -1;
//	return PopupMenu(x, y);
	if(!(m_pBackScreenBuf = CDC::SaveScreen(m_sRect)))
		return FALSE;

	m_dwStyle |= WS_VISIBLE; 
	m_pCurItem = NULL;
	((CFrameWnd *)m_pParent)->m_pCurCtrl = this;  
	((CFrameWnd *)m_pParent)->DeleteChild(this); 
	((CFrameWnd *)m_pParent)->AddChild(this); 
	Paint();
	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
//
CMenuItem * CMenu::GetPenItem(CPoint &pt)
{
	CMenuItem * pItem;
	for(pItem = m_pFirstItem; pItem; pItem = pItem->pNextItem)
	{
       if(pt.InRect(pItem->sRect))
		   break;
	}
	return pItem;
}
////////////////////////////////////////////////////////////////////////////////
//
void CMenu::DoPenDown(CPoint &pt)
{
     CMenuItem * pPenItem = GetPenItem(pt);
	 if(pPenItem && pPenItem != m_pCurItem)
	 {
		 if(m_pCurItem)
			 m_pCurItem->Dactive(); 
		 m_pCurItem = pPenItem;
		 m_pCurItem->Active(); 
	 }
	 else if(pPenItem && (m_pCurItem->nStyle & MS_POPUP) && pPenItem == m_pCurItem)
	     ((CFrameWnd *)m_pParent)->m_pCurCtrl = m_pCurItem->pPopupMenu; 		 

}
////////////////////////////////////////////////////////////////////////////////
//
void CMenu::DoPenMove(CPoint &pt)
{
  DoPenDown(pt);
}
////////////////////////////////////////////////////////////////////////////////
//
void CMenu::HideMenu()
{
	if(m_dwStyle & WS_VISIBLE)
	{
		m_dwStyle &= ~WS_VISIBLE;
		CDC::PutScreen(m_pBackScreenBuf); 
		((CFrameWnd *)m_pParent)->m_pCurCtrl = NULL;
		SendNotifyMessage(CN_MENUHIDE, 0);
	}
}
////////////////////////////////////////////////////////////////////////////////
//
void CMenu::DoDactive(UINT32 wParam)
{ 
   if(((CMenu *)wParam == m_pParentMenu) && m_pParentMenu)
	   return;

   if(m_pCurItem && wParam && ((CMenu *)wParam == m_pCurItem->pPopupMenu))
 	   return;

   if(!(m_dwStyle & WS_VISIBLE))  // added by hqf 12/03
	   return;
   if(m_pCurItem)
      m_pCurItem->Dactive(); 

    HideMenu();
	m_pCurItem	= NULL;
	if(m_pParentMenu)
    	m_pParentMenu->SendMessage(WM_DACTIVE, wParam, 0); 
}
////////////////////////////////////////////////////////////////////////////////
//
void CMenu::DoPenUp()
{
	if(m_pCurItem && !(m_pCurItem->nStyle & MS_POPUP))
	{
	  m_pCurItem->Dactive(); 
      HideMenu();

	  if(m_pParentMenu)
    	m_pParentMenu->HideMenu(); 

	  if(!(m_pCurItem->nStyle & WS_DISABLE)) 
	        ((CFrameWnd *)m_pParent)->SendMessage(CN_COMMAND, 
		                                          m_pCurItem->nItemId, 
	     		                                  UINT32(m_pCurItem)); 
 	 m_pCurItem	= NULL;
	}
	else if(m_pCurItem && (m_pCurItem->nStyle & MS_POPUP))
	     ((CFrameWnd *)m_pParent)->m_pCurCtrl = m_pCurItem->pPopupMenu; 		 

}
////////////////////////////////////////////////////////////////////////////////
//
void CMenu::WindowProcess(UINT32 nMessage, UINT32 wParam, UINT32 lParam)
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
	case WM_DACTIVE:
		DoDactive(wParam);
		break;
	}
}
////////////////////////////////////////////////////////////////////////////////
//
void CMenu::OnPaint(CDC &dc)
{
	dc.SetBackColor(MENU_BACK_COLOR);
	dc.EraseRect(m_sRect);
	dc.Draw3dRect(m_sRect);
	for(CMenuItem * pItem = m_pFirstItem; pItem; pItem = pItem->pNextItem)
	{
		pItem->PaintItem(dc);
	}
}
////////////////////////////////////////////////////////////////////////////////
//
void CMenu::SetItem(UINT32 nItemID, CMenuItem *pNewItem)
{
  for(CMenuItem * pItem = m_pFirstItem; pItem; pItem = pItem->pNextItem)
  {
	  if(pItem->nItemId == nItemID)
	  {
		  pItem->nIconId    = pNewItem->nIconId;
		  pItem->nStyle     = pNewItem->nStyle;
		  pItem->pPopupMenu = pNewItem->pPopupMenu;
		  strcpy(pItem->strText, pNewItem->strText);
		  break;
	  }
  }
}
////////////////////////////////////////////////////////////////////////////////
//
void CMenu::ModifyItem(UINT16 nItemNumber, CMenuItem *pNewItem)
{
  UINT16 nNumber = 0;
  for( CMenuItem * pItem = m_pFirstItem; pItem; pItem = pItem->pNextItem, nNumber++)
  {
	  if(nNumber == nItemNumber)
	  {
		  pItem->nIconId    = pNewItem->nIconId;
		  pItem->nStyle     = pNewItem->nStyle;
		  pItem->pPopupMenu = pNewItem->pPopupMenu;
		  strcpy(pItem->strText, pNewItem->strText);
		  break;
	  }
  }
}

////////////////////////////////////////////////////////////////////////////////
//
void CMenu::DeleteAllItem()
{
   CMenuItem * pItem = m_pCurItem = m_pFirstItem;
   while(m_pCurItem)
   {
	   m_pCurItem = m_pCurItem->pNextItem;
	   delete pItem;
	   pItem = m_pCurItem;
   }
   m_pFirstItem = m_pCurItem = NULL;
}

////////////////////////////////////////////////////////////////////////////////
//
void CMenu::GetItem(UINT32 nItemID, CMenuItem *pItemBuf)
{
  for(CMenuItem * pItem = m_pFirstItem; pItem; pItem = pItem->pNextItem)
  {
	  if(pItem->nItemId == nItemID)
	  {
		  pItemBuf->nIconId    = pItem->nIconId;
		  pItemBuf->nStyle     = pItem->nStyle;
		  pItemBuf->pPopupMenu = pItem->pPopupMenu;
		  strcpy(pItemBuf->strText, pItem->strText);
		  break;
	  }
  }
}
////////////////////////////////////////////////////////////////////////////////
//
void CMenu::ObtainItem(UINT32 nItemNumber, CMenuItem *pItemBuf)
{
  UINT16 nNumber = 0;
  for( CMenuItem * pItem = m_pFirstItem; pItem; pItem = pItem->pNextItem, nNumber++)
  {
	  if(nNumber == nItemNumber)
	  {
		  pItemBuf->nIconId    = pItem->nIconId;
		  pItemBuf->nStyle     = pItem->nStyle;
		  pItemBuf->pPopupMenu = pItem->pPopupMenu;
		  strcpy(pItemBuf->strText, pItem->strText);
		  break;
	  }
  }
}
////////////////////////////////////////////////////////////////////////////////
//
void CMenu::EnableItem(UINT32 nItemNumber, BOOL bEnable)
{
	UINT16 nNumber = 0;
	for( CMenuItem * pItem = m_pFirstItem; pItem; pItem = pItem->pNextItem, nNumber++)
	{
		if(nNumber == nItemNumber)
		{
			if(bEnable)
				pItem->nStyle &= ~MS_DISABLE;
			else
                pItem->nStyle |= MS_DISABLE;
			break;
		}
	}
}
////////////////////////////////////////////////////////////////////////////////
//
void CMenu::DisableItem(UINT32 nItemID, BOOL bDisable)
{
  for(CMenuItem * pItem = m_pFirstItem; pItem; pItem = pItem->pNextItem)
  {
	  if(pItem->nItemId == nItemID)
	  {
		  	if(bDisable)
				pItem->nStyle |= MS_DISABLE;
			else
				pItem->nStyle &= ~MS_DISABLE;
			break;
	  }
  }
}
////////////////////////////////////////////////////////////////////////////////
//
void CMenu::CheckItem(UINT32 nItemID, BOOL bChecked)
{
	for(CMenuItem * pItem = m_pFirstItem; pItem; pItem = pItem->pNextItem)
	{
	  if(pItem->nItemId == nItemID)
	  {
		  	if(bChecked)
				pItem->nIconId = ICON_CHECK; 
			else
				pItem->nIconId = ICON_UNCHECK; 
			break;
	  }
	}
}
////////////////////////////////////////////////////////////////////////////////
// end menu.cpp