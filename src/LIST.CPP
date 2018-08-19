/******************************************************************************/
/*                                                                            */
/*  版权所有(c)  2001汉王科技                                                 */
/*                                                                            */
/******************************************************************************/
/*  文 件 名                                                                  */
/*             listview.cpp                                                   */
/*                                                                            */
/*  版 本 号                                                                  */
/*             1.0                                                            */
/*                                                                            */
/*  描    述                                                                  */
/*             汉王科技E-PhoneII窗口类库的源文件                              */
/*             HanWang Technology E-PhoneII Function Class Library head file  */
/*                                                                            */
/*  组    成                                                                  */
/*             列表控件类源文件                                               */
/*                                                                            */
/*  作    者                                                                  */
/*             王飞                                                           */
/*                                                                            */
/*  日    期                                                                  */
/*             2001-07-01                                                     */
/*                                                                            */
/*  历史记录                                                                  */
/*             2001-07-22    增加接口函数ModifyItem                           */
/*                           增加接口函数DeleteItem                           */
/*                           增加接口函数DeleteAllItems                       */
/*                           修改AppendItems时内存的分配方案                  */
/*                                                                            */
/*             2001-08-21    增加ListView选中风格                             */
/*                           增加设置选中和获取选中单元格接口                 */
/*                                                                            */
/*             2001-09-05    修改SetStatus的一个BUG                           */
/*                           增加接口InsertItem接口                           */
/*                                                                            */
/*             2001-09-06    给AppendItem函数增加一个参数(郝庆丰)             */
/*             2001-09-07    给ModifyItem函数增加一个参数(郝庆丰)             */
/*             2001-11-05    修改ScrollTo函数(郝庆丰)                         */
/*             2001-11-05    修改SetSelItem函数(郝庆丰)                       */
/*             2001-11-05    修改ItemPaint函数(郝庆丰)                        */
/******************************************************************************/
#include "listview.h"
#include "datadrv.h"

////////////////////////////////////////////////////////////////////////////////
//
CListView::CListView()
{
	m_iColsNum = 3;
}

////////////////////////////////////////////////////////////////////////////////
//
CListView::~CListView()
{
	m_iColsNum = 3;
}

////////////////////////////////////////////////////////////////////////////////
//
BOOL CListView::SetColumnNum(INT16 iNum)
{
	if(iNum!=2 && iNum!= 3)
		return FALSE;
	m_iColsNum = iNum;
	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
//
BOOL CListView::Create(CRect &ctrRect, CFrameWnd *pParent, UINT32 nCtrlId)
{
	return Create(ctrRect, pParent, 0, nCtrlId);
}

////////////////////////////////////////////////////////////////////////////////
//
BOOL CListView::Create(CRect &ctrRect, CFrameWnd *pParent, UINT8 nAlwaysSel, UINT32 nCtrlId)
{
	if(!pParent)
		return FALSE;

    ctrRect.right -= (SB_WIDTH); 
	if(ctrRect.Width() < (160-SB_WIDTH)/m_iColsNum || ctrRect.Height()< LISTROW_HEIGHT1)
		return FALSE;

    if(!CCtrl::Create(NULL, ctrRect, pParent, WS_VISIBLE|WS_CHILD, nCtrlId))
		return FALSE;

	if(!m_oScrollBar.Create(CPoint(ctrRect.right+1, ctrRect.top), ctrRect.Height(), pParent, this))
		return FALSE;

	m_nAlwaysSel   = nAlwaysSel;

/*	if(m_nAlwaysSel)
		m_nCurSelItem  = 0;
	else
	    m_nCurSelItem  = -1;*/
	m_nCurSelItem  = -1;
	m_nDispRow    = m_nItems = m_nMemoryItemCount = 0;

	m_pListItems  = NULL;
	m_nStatus     = LS_LAGEICON;

	m_nItemWidth  = LISTCOL_WIDTH1;
	m_nItemHeight = LISTROW_HEIGHT1;

	m_nScreenRows = ctrRect.Height()/m_nItemHeight; 
	m_nCols       = ctrRect.Width()/m_nItemWidth;
	
	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
//
void CListView::ScrollTo(INT32 nItemNumber, BOOL bRedraw)
{
    if(nItemNumber <0 || nItemNumber >= m_nItems)
		return;
	if(m_nStatus == LS_LAGEICON)
		return;
    if(nItemNumber > (m_nItems - m_nScreenRows))
		m_nDispRow = m_nItems - m_nScreenRows;
	else
		m_nDispRow = nItemNumber;
	m_oScrollBar.SetPerameter(((m_nItems+m_nCols-1)/m_nCols), m_nScreenRows, m_nDispRow, bRedraw);
}

////////////////////////////////////////////////////////////////////////////////
//
void CListView::SetStatus(UINT8 nStatus, BOOL bRedraw)
{
   if(nStatus == m_nStatus)
	   return;
   if(nStatus == LS_LAGEICON)
   {
	m_nStatus     = LS_LAGEICON;
	m_nItemWidth  = LISTCOL_WIDTH1;
	m_nItemHeight = LISTROW_HEIGHT1;
    if(m_nDispRow %2)
	  m_nDispRow = (m_nDispRow == m_nCurSelItem) ?m_nDispRow-1 : m_nDispRow+1;
   }
   else
   { 
	 m_nStatus     = LS_SMALLICON;
	 m_nItemWidth  = m_sRect.Width();
	 m_nItemHeight = LISTROW_HEIGHT2;
	 if(m_nDispRow > m_nItems - m_nScreenRows * m_nCols)
        m_nDispRow = m_nItems - m_nScreenRows * m_nCols;
   }
   if(m_nDispRow <0)
	  m_nDispRow = 0;
	m_nScreenRows = m_sRect.Height()/m_nItemHeight; 
	m_nCols       = m_sRect.Width()/m_nItemWidth;

	
	if(!m_nAlwaysSel)
	  m_nCurSelItem = -1;
 
    m_oScrollBar.SetPerameter(((m_nItems+m_nCols-1)/m_nCols), m_nScreenRows, m_nDispRow, bRedraw);  
    
	if(bRedraw == TRUE)
		Paint();
}

////////////////////////////////////////////////////////////////////////////////
//
BOOL CListView::InsertItem(INT32 nInsertPos, char *strText, UINT32 nLageIcon, char *strExtText, UINT32 nSmallIcon, UINT32 nExtData)
{
	if(nInsertPos <0)
		return FALSE;

    if(nInsertPos >= m_nItems)
	    return AppendItem(strText, nLageIcon, strExtText, nSmallIcon, nExtData);

	if(m_nItems >= m_nMemoryItemCount)
    {
      CListItem *pTemp = m_pListItems;
	  m_pListItems = new CListItem[m_nMemoryItemCount + 50];
	  if(m_pListItems == NULL)
	  {
		m_pListItems = pTemp;
		return FALSE;
	  }
	  m_nMemoryItemCount += 50;
      memcpy(m_pListItems, pTemp, sizeof(CListItem)*nInsertPos);
	  memcpy(m_pListItems + nInsertPos+1, pTemp+nInsertPos, sizeof(CListItem)*(m_nItems-nInsertPos));
	  delete[] pTemp;
	}
    else
      memmove(m_pListItems+nInsertPos+1, m_pListItems+nInsertPos, sizeof(CListItem)*(m_nItems-nInsertPos));

	if(strText != NULL)
	  hwstrncpy(m_pListItems[nInsertPos].strText, strText, LISTITEM_TEXT_LENGTH-1);
	else
      m_pListItems[nInsertPos].strText[0] = 0;

	if(strExtText != NULL)
	  hwstrncpy(m_pListItems[nInsertPos].strExtText, strExtText, LISTITEM_TEXT_LENGTH-1);
	else
	  m_pListItems[nInsertPos].strExtText[0] = 0;

    m_pListItems[nInsertPos].nLageIcon  = nLageIcon;
	m_pListItems[nInsertPos].nSmallIcon = nSmallIcon;

    m_nItems++;
	m_oScrollBar.SetPerameter(((m_nItems+m_nCols-1)/m_nCols), m_nScreenRows, m_nDispRow); 
    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
//
BOOL CListView::AppendItem(char *strText, UINT32 nLageIcon, char *strExtText, UINT32 nSmallIcon,UINT32 nExtData)
{
	if(m_nItems >= m_nMemoryItemCount)
    {
      CListItem *pTemp = m_pListItems;
	  m_pListItems = new CListItem[m_nMemoryItemCount + 50];
	  if(m_pListItems == NULL)
	  {
		m_pListItems = pTemp;
		return FALSE;
	  }
	  m_nMemoryItemCount += 50;
      memcpy(m_pListItems, pTemp, sizeof(CListItem)*m_nItems);
	  delete[] pTemp;
	}

	if(strText != NULL)
	  hwstrncpy(m_pListItems[m_nItems].strText, strText, LISTITEM_TEXT_LENGTH-1);
	else
      m_pListItems[m_nItems].strText[0] = 0;

	if(strExtText != NULL)
	  hwstrncpy(m_pListItems[m_nItems].strExtText, strExtText, LISTITEM_TEXT_LENGTH-1);
	else
	  m_pListItems[m_nItems].strExtText[0] = 0;

    m_pListItems[m_nItems].nLageIcon  = nLageIcon;
	m_pListItems[m_nItems].nSmallIcon = nSmallIcon;
	m_pListItems[m_nItems++].nExtData = nExtData;
	m_oScrollBar.SetPerameter(((m_nItems+m_nCols-1)/m_nCols), m_nScreenRows, m_nDispRow); 
    
    return TRUE;
}
////////////////////////////////////////////////////////////////////////////////
//
void CListView::DoPenDown(CPoint &pt)
{
	 CRect sRect;
	 INT32   nPenItem = GetPenItem(pt, sRect);
	 if(nPenItem >=0 && nPenItem != m_nCurSelItem)
	 {
		 if(m_nCurSelItem >= m_nDispRow && m_nCurSelItem < m_nDispRow + m_nScreenRows * m_nCols)
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
void CListView::DoPenMove(CPoint &pt)
{
	DoPenDown(pt);
}
////////////////////////////////////////////////////////////////////////////////
//
void CListView::DoPenMoveOut()
{
   if(m_nCurSelItem >= 0)// && !m_nAlwaysSel)
	{ 
		CDC::InvertRect(m_sSelRect); 
		m_nCurSelItem = -1;
	}
	//	??? 查一查 ??? bbj
    //SendNotifyMessage(CN_COMMAND, m_nCurSelItem); // hqf 12/28
}
////////////////////////////////////////////////////////////////////////////////
//
void CListView::DoPenUp()
{
	INT32 nSel = m_nCurSelItem;
	if(m_nCurSelItem >= 0)
	{ 
		if(!m_nAlwaysSel)
		{
		   CDC::InvertRect(m_sSelRect); 
		   m_nCurSelItem = -1;
		}
		SendNotifyMessage(CN_COMMAND, nSel); 
	}
}

////////////////////////////////////////////////////////////////////////////////
//
void CListView::DoScrollTo(INT16 nDispRow)
{
	if(m_nDispRow != nDispRow * m_nCols)
	{
		m_nDispRow = nDispRow * m_nCols;
        PaintItems(CDC());
	}
}
////////////////////////////////////////////////////////////////////////////////
//
INT32 CListView::GetPenItem(CPoint &pt, CRect &sItemRect)
{
  	sItemRect.SetRect(m_sRect.left, m_sRect.top, m_sRect.left+m_nItemWidth-1, m_sRect.top + m_nItemHeight-1);
	INT32 maxi = MIN(m_nItems, m_nScreenRows * m_nCols+m_nDispRow); 
	for(INT32 i=m_nDispRow; i<maxi; i++)
	{
		if(pt.InRect(sItemRect))
			return i;
		if(sItemRect.right + m_nItemWidth <= m_sRect.right + 1)
		{
			sItemRect.left += m_nItemWidth;
			sItemRect.right += m_nItemWidth;
		}
		else
		{
			sItemRect.left = m_sRect.left;
			sItemRect.right = sItemRect.left+m_nItemWidth-1;
			sItemRect.top  += m_nItemHeight;
			sItemRect.bottom += m_nItemHeight;
		}
	}
	return -1;
}
////////////////////////////////////////////////////////////////////////////////
//
void CListView::PaintItems(CDC &dc)
{
	CRect r(m_sRect.left, m_sRect.top, m_sRect.left+m_nItemWidth-1, m_sRect.top + m_nItemHeight-1);
	for(INT32 i=m_nDispRow; i<m_nScreenRows * m_nCols+m_nDispRow; i++)
	{
		ItemPaint(i, r, dc);
		if(r.right + m_nItemWidth <= m_sRect.right + 1)
		{
			r.left += m_nItemWidth;
			r.right += m_nItemWidth;
		}
		else
		{
			r.left = m_sRect.left;
			r.right = r.left+m_nItemWidth-1;
			r.top  += m_nItemHeight;
			r.bottom += m_nItemHeight;
		}
	}
}
////////////////////////////////////////////////////////////////////////////////
//
void CListView::ItemPaint(INT32 nItemNum, CRect &r, CDC &dc)
{
  dc.EraseRect(r);  

  if(nItemNum >= m_nItems)
     return;

  if(m_nStatus == LS_LAGEICON)
  {
     int	i, j;
	 dc.PutIcon(r.HCenter() - gICON_WIDTH(m_pListItems[nItemNum].nLageIcon)/2, r.top+1, m_pListItems[nItemNum].nLageIcon);
	 dc.SetAlign(HALIGN_CENTER);
	 char strTemp[7];
	 hwstrncpy(strTemp, m_pListItems[nItemNum].strText , 6);
	 strTemp[6] = '\0';
	 for (i = j = 0; i < 7; i ++){
		 if (strTemp[i] & 0x80)
			 j ++;
	 }
	 if (j % 2)
		strTemp[5] = '\0';
	 dc.DrawText(r.HCenter()+1, r.bottom - CHAR_HEIGHT,strTemp ); 
  }
  else
  { 
	 dc.PutIcon(r.left + 1, r.top, m_pListItems[nItemNum].nSmallIcon);
	 CRect r1(r.left+17, r.top, r.right, r.bottom);
     //dc.DrawText(r1, m_pListItems[nItemNum].strText);
	 //r1.left = r1.right+1;
	 //r1.right = m_sRect.right;
	 dc.DrawText(r1, m_pListItems[nItemNum].strText);
  }
  if(m_nAlwaysSel && nItemNum == m_nCurSelItem )
  {
	  m_sSelRect = r;
	  dc.InvertRect(r);
  }
}
////////////////////////////////////////////////////////////////////////////////
//
void CListView::WindowProcess(UINT32 nMessage, UINT32 wParam, UINT32 lParam)
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
	case WM_PENMOVEOUT:
		DoPenMoveOut();
		break;
	case WM_DACTIVE:
//		OnDactive(); // hqf 1/11
		break;
	case CN_SCROLLTO:
		DoScrollTo(LOWORD(lParam)); 
		break;
	}
}

////////////////////////////////////////////////////////////////////////////////
//
void CListView::OnDactive()
{
	SetSelItem(-1);
	OnPaint(CDC());
}

////////////////////////////////////////////////////////////////////////////////
//
void CListView::OnPaint(CDC &dc)
{
	dc.EraseRect(m_sRect);
	PaintItems(dc);
}
////////////////////////////////////////////////////////////////////////////////
//
void CListView::Destroy()
{
	delete []m_pListItems;
	CCtrl::Destroy(); 
}
////////////////////////////////////////////////////////////////////////////////
//
BOOL CListView::GetItem(CListItem * pItemBuf, INT32 nItemNumber)
{
	if(nItemNumber <0 || nItemNumber >= m_nItems)
		return FALSE;
	memcpy(pItemBuf, &m_pListItems[nItemNumber], sizeof(CListItem));
	return TRUE;
} 
////////////////////////////////////////////////////////////////////////////////
//
BOOL CListView::ModifyItem(CListItem *pNewItem, INT32 nItemNumber , BOOL bRedraw)
{
	if(nItemNumber <0 || nItemNumber >= m_nItems)
		return FALSE;

	memcpy(&m_pListItems[nItemNumber], pNewItem, sizeof(CListItem));
	if(bRedraw)
		PaintItems(CDC());
	return TRUE;
     
}
////////////////////////////////////////////////////////////////////////////////
//
BOOL CListView::DeleteItem(INT32 nItemNumber)
{
	if(nItemNumber <0 || nItemNumber >= m_nItems)
		return FALSE;

	memmove(&m_pListItems[nItemNumber], &m_pListItems[nItemNumber+1], sizeof(CListItem) * (m_nItems - nItemNumber -1));
    m_nItems --;
	m_oScrollBar.SetPerameter(((m_nItems+m_nCols-1)/m_nCols), m_nScreenRows, m_nDispRow, TRUE); 
   
	if(m_nAlwaysSel)
	{
		if(m_nCurSelItem > nItemNumber)
			m_nCurSelItem--;
		else if(m_nCurSelItem == nItemNumber)
			m_nCurSelItem=-1;
    }

	PaintItems(CDC());
	return TRUE;
}
////////////////////////////////////////////////////////////////////////////////
//
void CListView::DeleteAllItems()
{
	 m_nItems = m_nDispRow =0;
	 m_nCurSelItem = -1;
     m_oScrollBar.SetPerameter(((m_nItems+m_nCols-1)/m_nCols), m_nScreenRows, m_nDispRow, FALSE); 
	 //PaintItems(CDC());
}
////////////////////////////////////////////////////////////////////////////////
//
void CListView::SetSelItem(INT32 nItemNumber)
{
	if(!m_nAlwaysSel)
		return ;

	if(nItemNumber <0 || nItemNumber >= m_nItems)
	{
		m_nCurSelItem = -1; // added by hqf
		return ;
	}
	m_nCurSelItem = nItemNumber;
}
   
////////////////////////////////////////////////////////////////////////////////
//
INT32 CListView::GetSelItem()
{
	return m_nCurSelItem;
}
////////////////////////////////////////////////////////////////////////////////
// end listview.cpp
