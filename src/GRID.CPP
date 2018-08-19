/******************************************************************************/
/*                                                                            */
/*  版权所有(c)  2001汉王科技                                                 */
/*                                                                            */
/******************************************************************************/
/*  文 件 名                                                                  */
/*             grid.cpp                                                       */
/*                                                                            */
/*  版 本 号                                                                  */
/*             1.0                                                            */
/*                                                                            */
/*  描    述                                                                  */
/*             汉王科技网格控件类的源文件                                     */
/*             HanWang Technology E-PhoneII grid control class source file    */
/*                                                                            */
/*  组    成                                                                  */
/*             CGrid类定义                                                    */
/*                                                                            */
/*  设计说明                                                                  */
/*             参见《网格控件设计说明书》文档                                 */
/*                                                                            */
/*  作    者                                                                  */
/*             郝庆丰                                                         */
/*                                                                            */
/*  日    期                                                                  */
/*             2001-07-24                                                     */
/*                                                                            */
/*  修改记录                                                                  */
/*             2001-07-26                                                     */
/*             改正了一个bug, 现象是单元格折行显示时，                        */
/*             当选中后行与行之间会出现白色线条                               */
/*             2001-08-04                                                     */
/*             增强了单元格的功能，即有图标的单元也可换行，但图标必须为16×16 */            
/*             2001-08-06                                                     */
/*             增加GetRectFromClick函数,即可以得到笔点中的单元的大小和位置    */
/*             2001-08-09                                                     */
/*             修改了Destroy中的bug.                                          */
/*             2001-08-28                                                     */
/*             修改了AddRow等函数，使添加数据时可以不动态修改；               */ 
/*             修改了Create函数，使网格可以有不选中风格.                      */   
/*             还需要测试如果设定单元格文本内容为'\0'时的结果                 */
/*             2001-09-06                                                     */
/*             修改了ft函数，使设置单元格内容时可以更改图标（待测试）*/
/*             2001-09-07                                                     */
/*             修改了DoPenUP函数中的一个BUG                                   */
/*             2001-09-18                                                     */
/*             修改了DoPenDown函数中的一个BUG(反显得问题)                     */
/*             2001-10-24                                                     */
/*             修改了GetUnitFromPt函数中的一个BUG                             */                                      
/*			   (有时得到的单元格列数会超出范围)                               */
/*             2001-10-30                                                     */
/*             修改了DoPenDown函数中的一个BUG(反显得问题)                     */
/*             2001-10-31                                                     */
/*             修改了AddRow函数，加了对行数据指针的判断                       */
/*             2001-10-31                                                     */
/*             修改了PaintRow函数                                             */
/*             2001-11-01                                                     */
/*             修改了DoPenDown函数(允许设置行的风格为不选中)                  */
/*             2001-11-13                                                     */
/*             增加了SetUnitIcon函数(设定单元格图标，待测试)                  */
/*             2001-11-18                                                     */
/*             修改了SetUnitText函数                                          */
/*             2001-11-26                                                     */
/*             增加了SetStartRow, SetSelRow函数(待测试)                       */
/*             2001-11-27                                                     */
/*             增加了TBS_NOVERLINE(没有竖线风格)(待测试)                      */
/*add ConvertTest() ,modify SetStartRow()  余国荣
/******************************************************************************/
/************************存在的问题********************************************/
/*            选中时滚动有时会死机!!!                                         */
/******************************************************************************/

#include "grid.h"
 
#ifndef ISCC
#define  ISCC(ch)    ((ch)> 0xa0)
#endif

CGrid::CGrid()
{
	m_iActualRows = m_iAppearRows = 0;
	m_pScrollBar = NULL;
}

CGrid::~CGrid()
{
	
}

void CGrid::Destroy()
{

	RemoveAll();
	SysFree(m_pRowHead);
	m_pRowHead = m_pDisplayRow = NULL;
	CCtrl::Destroy();
/*    CDC dc;
	dc.SetBackColor();
	dc.EraseRect(m_sRect)*/
}

BOOL CGrid::Create(CFrameWnd *pParent , CRect &rectPos , UINT32 dwStyle, int iRows , int iCols , 
				   UINT32 nID , BOOL bCanSel)
{
	if(iCols > TABLE_MAX_COLS || iCols <1 || iRows <0 )
		return FALSE;
	m_iActualRows = m_iAppearRows = iRows, m_iTotalCols = iCols ;
	m_iScreenRows = rectPos.Height()/TABLE_ROW_HEIGHT;
	rectPos.bottom = rectPos.top + m_iScreenRows * TABLE_ROW_HEIGHT;
	m_iStartRow = 1; m_iEndRow = MIN(m_iScreenRows , iRows);
	m_iCurRow = m_iCurCol = -1, m_iSelectedRow = m_iSelectedCol = -1;
	m_cUnitStyle = UNS_TEXT;
	m_bCanSel = bCanSel;
	if(!CCtrl::Create(NULL , rectPos , pParent , dwStyle | WS_VISIBLE, nID))
		return FALSE;
	m_pRowHead = (GridRowData *)SysMalloc(sizeof(GridRowData));
	memset(m_pRowHead, 0 , sizeof(GridRowData));
	m_pRowHead->pPrev = m_pRowHead->pNext = NULL;
	GridRowData *pRow = NULL, *pRow2 = m_pRowHead ;
	//	pRow = (GridRowData *)SysMalloc(sizeof(GridRowData));
	for(UINT16 row = 0 ; row < iRows ; row++)
	{
		pRow = (GridRowData *)SysMalloc(sizeof(GridRowData));
		memset(pRow , 0 , sizeof(GridRowData));
        pRow2->pNext = pRow;
		pRow->pPrev = pRow2;
		pRow2 = pRow;
	}
	m_pDisplayRow = m_pRowHead->pNext;
	if(m_iAppearRows > m_iScreenRows)
		if(!m_pScrollBar)
		{
			CPoint pt(m_sRect.right+1 , m_sRect.top);
			m_pScrollBar = new CScrollBar;
			m_pScrollBar->Create(pt , m_sRect.Height() , pParent , 
				this , GRID_IDSCROLLBAR);
			m_pScrollBar->SetPerameter(m_iAppearRows , m_iScreenRows ,
				0 , FALSE);
		}
//	m_iColsWidth = (UINT16 *)SysMalloc(iCols *sizeof(UINT16));
	for(UINT16 col = 0 ; col < iCols ; col++)
		m_iColsWidth[col] = (m_sRect.Width()-1)/iCols;
	return TRUE;
}


void CGrid::AddRow(char *pText[] , char *pStyle , UINT32 *pIconID , BOOL bRedraw )
{
    if(m_dwStyle & TBS_HAVFIXROW|| !pText || !pStyle || !pIconID)
		return;
//	GridColData *pCol = NULL , *pColHead;
	static char childs = 0;
	int y ;
	UINT16 len , unitLen , num, maxnum= 1;
	UINT32 col, row ,pSubAddr[MAX_SUB_LINES];
	//下面分配行头所需的空间
	GridRowData *pRow , *pRow2 ;

	row = UnConvert(m_iStartRow);
	y = m_sRect.top + (m_iAppearRows - row +1) * TABLE_ROW_HEIGHT;
	pRow2 = (m_pDisplayRow == NULL ? m_pRowHead: m_pDisplayRow);
	for(row = LOWORD(m_iStartRow); row < (UINT32)m_iActualRows  ; row++)//待优化
        pRow2 = pRow2->pNext;
    childs = sizeof(GridRowData);
	pRow = (GridRowData *)SysMalloc(sizeof(GridRowData));
	memset(pRow , 0 , sizeof(GridRowData));
//	pRow->style |= cRowStyle;
 	pRow->pPrev = pRow2;
    pRow2->pNext = pRow;   //加到行的链表中
	m_iActualRows++;
    //下面添加每个单元格的数据
	for(col = 0; col < (UINT32)m_iTotalCols ; col++)
	{
		pRow->pColData[col].nIconIndex = pIconID[col];
	    pRow->pColData[col].style = pStyle[col];
		
		len = strlen(pText[col]);
		if(len > MAX_UNITTEXT_LEN)
			len = MAX_UNITTEXT_LEN;
		if(pText[col] == NULL) 
			len = 0;
		if(pIconID[col])
		{
		    pRow->pColData[col].pText = (char *)SysMalloc( len +2+1);
			pRow->pColData[col].pText[0] = 
				pRow->pColData[col].pText[1] = 0xa1;
            memcpy(pRow->pColData[col].pText +2 , pText[col], len); 
			pRow->pColData[col].pText[len +2] = '\0';
		}
		else
		{
			pRow->pColData[col].pText = (char *)SysMalloc( len +1);
		    memcpy(pRow->pColData[col].pText, pText[col], len);
			pRow->pColData[col].pText[len] = '\0';
		}
		pRow->style |= pStyle[col];
		unitLen = (m_iColsWidth[col]-2) / ASC_WIDTH;
		if(pRow->pColData[col].style & UNS_COMBOX)
			   continue;
        num = GetSubStringAddr(pRow->pColData[col].pText , unitLen,pSubAddr);
		if(num > maxnum)
			maxnum = num;

	}
	pRow->cChilds = maxnum -1;
	m_iAppearRows += maxnum;
    
	//画所加的行
	if(y < m_sRect.bottom)
	{
		if(m_iStartRow==1)
		    m_pDisplayRow= m_pRowHead->pNext;
		row = MAKELONG(m_iActualRows , RD_DRAWALL);
		UINT16 u = (m_sRect.bottom - y)/TABLE_ROW_HEIGHT ;
		u = MIN(pRow->cChilds , u -1 );
		m_iEndRow = MAKELONG(m_iActualRows , u);
		if(bRedraw)
		{
			PaintGrid();
			PaintRow(row);
		}
	}
	else
        pRow->style |= RS_DISABLE;
	if(m_iAppearRows > m_iScreenRows)
	{
		row = UnConvert(m_iStartRow);
		if(!m_pScrollBar)
		{
			CPoint pt(m_sRect.right+1 , m_sRect.top);
			m_pScrollBar = new CScrollBar;
			m_pScrollBar->Create(pt , m_sRect.Height(), (CFrameWnd *)m_pParent , 
				 this , GRID_IDSCROLLBAR);
			m_pScrollBar->SetPerameter(m_iAppearRows , m_iScreenRows , 
				   (UINT16)row -1);
			if(bRedraw)
				m_pScrollBar->OnPaint(CDC());

		}
		else
			m_pScrollBar->SetPerameter(m_iAppearRows , m_iScreenRows , 
				   (UINT16)row -1, bRedraw);
	}
}

void CGrid::InsertRow(UINT16 iBeforeRow, char *pText[] , char *pStyle, UINT32 *pIconID , BOOL bRedraw)
{
    if(iBeforeRow > m_iActualRows)
		iBeforeRow = m_iActualRows;
		
	UINT16 l, l2, t, i, unitLen, num ,maxnum =1, col;
	UINT32 pSubAddr[MAX_SUB_LINES];
	GridRowData *pRow, *pRow2;

    l = LOWORD(m_iStartRow), l2 = LOWORD(m_iEndRow);
	if(iBeforeRow <= l)
	{
		t = 1;
		pRow= m_pRowHead;
		m_iStartRow++;
	}
	else
	{
		t = l+1;
		pRow = m_pDisplayRow;
	}
	if(m_iSelectedRow>=0 && iBeforeRow <= LOWORD(m_iSelectedRow))
		m_iSelectedRow++;
	for(i = t; i < iBeforeRow; i++)
    	pRow = pRow->pNext;

    pRow2 = (GridRowData *)SysMalloc(sizeof(GridRowData));
	memset(pRow2 , 0 ,sizeof(GridRowData));
	pRow2->pNext = pRow->pNext;
	if(pRow->pNext)
	   pRow->pNext->pPrev = pRow2;
	pRow2->pPrev = pRow;
	pRow->pNext = pRow2;
    for(col = 0; col < m_iTotalCols ; col++)
	{
		pRow2->pColData[col].nIconIndex = pIconID[col];
	    pRow2->pColData[col].style = pStyle[col];
		//添加单元格文本
		t = strlen(pText[col]);
		if(pText[col]==NULL) t = 0;
		if(t > MAX_UNITTEXT_LEN)
              t = MAX_UNITTEXT_LEN;
        if(pIconID[col])
		{
		    pRow2->pColData[col].pText = (char *)SysMalloc( t +2+1);
			pRow2->pColData[col].pText[0] = 
				pRow2->pColData[col].pText[1] = 0xa1;
            memcpy(pRow2->pColData[col].pText +2 , pText[col], t); 
			pRow2->pColData[col].pText[t +2] = '\0';
		}
		else
		{
			pRow2->pColData[col].pText = (char *)SysMalloc( t +1);
		    memcpy(pRow2->pColData[col].pText, pText[col], t);
			pRow2->pColData[col].pText[t] = '\0';
		}		
		//判断折行数
		pRow->style |= pStyle[col];
		unitLen = (m_iColsWidth[col] -2) / ASC_WIDTH;
		if(pRow2->pColData[col].style & UNS_COMBOX)
			   continue;
        num = GetSubStringAddr(pRow2->pColData[col].pText , unitLen,pSubAddr);
		if(num > maxnum)
			maxnum = num;
	}
	pRow2->cChilds = maxnum -1;
	m_iActualRows++;
	m_iAppearRows += maxnum;
	//如果需要，画插入的行
    
	if( m_iAppearRows - maxnum <= m_iScreenRows)
	{
			m_iStartRow = 1; 
			m_pDisplayRow = m_pRowHead->pNext;
		if(bRedraw)
		{
		
			PaintGrid();
			PaintRows();
		}
	}
	else if(iBeforeRow > l && iBeforeRow <= l2 )
	{
		if(bRedraw)
		{
			PaintGrid();
			PaintRows();
		}
	}
	if(m_iAppearRows > m_iScreenRows)
	{
		t = (UINT16)UnConvert(m_iStartRow);
		if(!m_pScrollBar)
		{
			CPoint pt(m_sRect.right +1 , m_sRect.top);
			m_pScrollBar = new CScrollBar;
			m_pScrollBar->Create(pt , m_sRect.Height(), (CFrameWnd *)m_pParent , 
				 this , GRID_IDSCROLLBAR);
			m_pScrollBar->SetPerameter(m_iAppearRows , m_iScreenRows , 
				   t -1);
			if(bRedraw)
				m_pScrollBar->OnPaint(CDC());

		}
		else
			m_pScrollBar->SetPerameter(m_iAppearRows , m_iScreenRows , 
				   t -1, bRedraw);
	}
}

INT32 CGrid::GetUnitIcon(UINT16 iRow , UINT16 iCol)
{
	if(iCol < 1|| iCol > m_iTotalCols)
		return -1;
	GridRowData *pRow ;
	pRow =GetRowPointer(iRow);
	if(!pRow)
		return -1;
	return pRow->pColData[iCol-1].nIconIndex;
}

UINT16 CGrid::GetUnitText(UINT16 iRow , UINT16 iCol , char *pBuf , UINT16 nBufLen)
{
	if(iCol <1 || iCol > m_iTotalCols)
	{
		pBuf[0] = '\0';
		return 0;
	}
	UINT16 l;

	GridRowData *pRow = GetRowPointer(iRow);
	if(!pRow)
		return 0;
	if(pRow->pColData[iCol-1].nIconIndex==0)
	{
	    hwstrncpy(pBuf, pRow->pColData[iCol-1].pText , nBufLen-1);
		l = MIN(strlen(pRow->pColData[iCol-1].pText) , nBufLen-1);
		pBuf[l] = '\0';
	}
	else
	{
		hwstrncpy(pBuf, pRow->pColData[iCol-1].pText +2 , nBufLen -1);
		l = MIN(strlen(pRow->pColData[iCol-1].pText) -1 ,nBufLen -1);
		pBuf[l] = '\0';
	}
	return l;
}

void CGrid::SetUnitIcon(UINT16 iRow , UINT16 iCol , UINT32 nIconID , BOOL bRedraw)
{
	if(iRow < 1 || iRow > m_iActualRows || iCol <0 || iCol > m_iTotalCols)
		return;
	GridRowData *pRow; 
	UINT16 l , l2 ,row;

	l = LOWORD(m_iStartRow) , l2 = LOWORD(m_iEndRow);
	
	pRow = GetRowPointer(iRow);
	if(!pRow)	return;
	
	pRow->pColData[iCol-1].nIconIndex = nIconID;

	char *pTemp = pRow->pColData[iCol].pText;
	short iLen = strlen(pTemp);
	//如果没有图标
	if(nIconID==0)
	{
		if(iLen < 2) iLen =2;
		strncpy(pTemp,&pTemp[2], iLen-2); 
		pTemp[iLen -2]=0;
	}
	//如果单元格在网格范围内，更新单元格内容 size_t 
	if(iRow >= l && iRow <= l2)
	{
	   if(bRedraw)
	   {
		   if(iRow == l)
			 m_iStartRow &= 0x0FFFF;
		   PaintGrid();
		   PaintRows();
	   }
	}
	//刷新滚动条
	if(m_pScrollBar)
	{
		row = (UINT16)UnConvert(m_iStartRow);
		m_pScrollBar->SetPerameter(m_iAppearRows , m_iScreenRows , row-1 , bRedraw); 
	}
}	
	
void CGrid::SetUnitText(UINT16 iRow , UINT16 iCol , char *pText , UINT32 nIconID ,BOOL bRedraw )
{
	if(iRow < 1 || iRow > m_iActualRows || iCol <0 || iCol > m_iTotalCols)
		return;
	UINT16 l , col , l2, row , unitLen, num, maxnum =1;
	UINT32 pSubAddr[MAX_SUB_LINES];
	GridRowData *pRow;

	l = LOWORD(m_iStartRow) , l2 = LOWORD(m_iEndRow);
	if(iRow >= l)
	{
		pRow = m_pDisplayRow;
		for(row = l ; row < iRow ; row++)
		    pRow = pRow->pNext;
	}
	else
	{
		pRow = m_pRowHead->pNext;
		for(row = 1 ; row < iRow; row++)
			pRow = pRow->pNext;
	}
	//改变单元格数据
	iCol--;
	SysFree(pRow->pColData[iCol].pText);
	
	num = strlen(pText); 
	if(num > MAX_UNITTEXT_LEN)
		num = MAX_UNITTEXT_LEN;
	if(pText== NULL)
		num = 0;
//	if(nIconID >0)
	pRow->pColData[iCol].nIconIndex = nIconID;

	if(pRow->pColData[iCol].nIconIndex)
	{
	    pRow->pColData[iCol].pText = (char *)SysMalloc(num +2+1);
		pRow->pColData[iCol].pText[0] = pRow->pColData[iCol].pText[1] = 0xa1;
	    memcpy(pRow->pColData[iCol].pText +2 , pText, num);
		pRow->pColData[iCol].pText[num +2] = '\0';
	}
	else
	{
	    pRow->pColData[iCol].pText = (char *)SysMalloc(num +1);
	    memcpy(pRow->pColData[iCol].pText, pText, num);
		pRow->pColData[iCol].pText[num] = '\0';
	}
    
	for(col = 0 ; col < m_iTotalCols ; col++)
	{
		unitLen = (m_iColsWidth[col]-2)/ASC_WIDTH;
		if(pRow->pColData[col].style & UNS_COMBOX)
			   continue;
		num = GetSubStringAddr(pRow->pColData[col].pText , unitLen , pSubAddr);
		if(num > maxnum)
			maxnum = num;
	}
	m_iAppearRows -=(pRow->cChilds - maxnum +1);
	pRow->cChilds = maxnum -1;
	//如果单元格在网格范围内，更新单元格内容
	if(iRow >= l && iRow <= l2)
	{
	   if(bRedraw)
	   {
		   if(iRow == l)
			 m_iStartRow &= 0x0FFFF;
		   PaintGrid();
		   PaintRows();
	   }
	}
	if(m_iAppearRows <= m_iScreenRows)
	{
		if(m_pScrollBar && bRedraw)
		{
			CRect rcSb = m_pScrollBar->GetWindowRect();
			CDC dc;
			dc.EraseRect(rcSb);
			PaintGrid();
			m_pScrollBar->Destroy();
			delete m_pScrollBar;
			m_pScrollBar = NULL;
		}
	}
	else if(!m_pScrollBar)
	{
		CPoint pt(m_sRect.right +1 , m_sRect.top);
		m_pScrollBar = new CScrollBar;
		m_pScrollBar->Create(pt , m_sRect.Height(), (CFrameWnd *)m_pParent , 
			 this , GRID_IDSCROLLBAR);
	}
	if(m_pScrollBar)
	{
		row = (UINT16)UnConvert(m_iStartRow);
		m_pScrollBar->SetPerameter(m_iAppearRows , m_iScreenRows , row-1 ); 
		if(bRedraw)
			m_pScrollBar->OnPaint(CDC());
	}
}

void CGrid::SetUnSel()
{
	if(m_iSelectedRow ==-1 || m_iSelectedCol== -1)
		return;
	m_iSelectedRow= m_iSelectedCol = -1;
	PaintRows();
}

void CGrid::MoveDisplayRowPointer(INT16 iNum)
{
	if(iNum < 0)
	{
		for(INT16 i = 0; i > iNum && m_pDisplayRow;i--)
			m_pDisplayRow = m_pDisplayRow->pPrev;
	}
	else 
	{
		for(INT16 i =0 ;i< iNum && m_pDisplayRow;i++)
			m_pDisplayRow = m_pDisplayRow->pNext;	
	}
}

void CGrid::SetSelRow(INT16 iRow, BOOL bRedraw)
{
	if(!m_bCanSel || ( iRow < 1 || iRow > m_iActualRows))
		return;
	m_iSelectedRow = iRow;
	m_iSelectedCol =1;
	if(LOWORD(m_iStartRow) > m_iSelectedRow)
	{
		MoveDisplayRowPointer(m_iSelectedRow - LOWORD(m_iStartRow));
		m_iStartRow = m_iSelectedRow;
	}
	else if(LOWORD(m_iEndRow) < m_iSelectedRow)
	{	
		MoveDisplayRowPointer(m_iSelectedRow - LOWORD(m_iEndRow));
		m_iStartRow +=(m_iSelectedRow - LOWORD(m_iEndRow));
	
	}
	if(bRedraw)
	{
		PaintRows();
		iRow = UnConvert(m_iStartRow);
	}
	m_pScrollBar->SetPerameter(m_iAppearRows , m_iScreenRows, (UINT16)iRow -1, bRedraw);
}

UINT16 CGrid::GetSubStringAddr(char *pText , UINT16 nUnitLen, UINT32 *pSubAddr)
{
	UINT16 len , i , sublen, j;
	len = strlen(pText);
	if(pText==NULL)
	{
		pSubAddr[0] = pSubAddr[1]= 0;
		return 0;
	}
	if(len <= nUnitLen)
	{
		pSubAddr[0] = (UINT32)pText;
		pSubAddr[1] = pSubAddr[0] + len; 
  		return 1;
	}
	pSubAddr[0] = (UINT32)pText;
	sublen = 0;
	j = 1;
    for(i = 0 ; i< len ; )
	{
		if(ISCC((BYTE)pText[0]))
		{
			i+=2;
			pText +=2;
			sublen +=2;
		}
		else
		{
			i++;
			pText++;
			sublen++;
		}
		if(sublen > nUnitLen)
		{
			pSubAddr[j++] = (UINT32)(pText-2);
			sublen = 2;
		}
		else if(sublen == nUnitLen)
		{
			if(i!=len)
			   pSubAddr[j++] =(UINT32)pText;
			else
			   pSubAddr[j] =(UINT32)pText;
			sublen = 0;
		}
	}
	pSubAddr[j] = pSubAddr[0] + len;
	return j;
}

void CGrid::DeleteScrollbar()
{
	if(m_pScrollBar)
	{
		CRect rcSb = m_pScrollBar->GetWindowRect();
		CDC dc;
		dc.EraseRect(rcSb);
		m_pScrollBar->Destroy();
		delete m_pScrollBar;
		m_pScrollBar = NULL;
	}
}

void CGrid::DeleteRow(UINT16 iRow , BOOL bRedraw)
{
	if(iRow <1 || iRow > m_iActualRows|| m_iActualRows ==0)
		return;
	UINT16 l, row,l2;
	UINT32 iRow2;
	GridRowData *pRow;
	l2 = LOWORD(m_iSelectedRow);
    if(iRow == l2)
		m_iSelectedRow = -1;
    else
		m_iSelectedRow--;
	l = LOWORD(m_iStartRow);
	if(iRow >= l )
	{
		pRow = m_pDisplayRow;
		for(row = l ; row < iRow; row++)
			 pRow = pRow->pNext;
	}
	else
	{
		pRow = m_pRowHead->pNext;
        for(row = 1; row < iRow; row++)
		    pRow = pRow->pNext;
	}
	pRow->pPrev->pNext = pRow->pNext;
	if(pRow->pNext)
		pRow->pNext->pPrev = pRow->pPrev;
	m_iActualRows--;
	m_iAppearRows-= (pRow->cChilds +1);
	if(iRow == l)
	{
    	m_iStartRow &= 0x0FFFF;
		if(pRow->pNext)
		   m_pDisplayRow = pRow->pNext;
		else
		{
			m_iStartRow--;
			m_pDisplayRow = pRow->pPrev;
		}
	}
	else if(iRow < l)
		m_iStartRow--;
	FreeRow(pRow);
	
	if(m_iAppearRows <= m_iScreenRows)
	{
		m_iStartRow = 1;
		m_pDisplayRow = m_pRowHead->pNext;
	}
	iRow2 = UnConvert(m_iStartRow);	
	if(iRow <= LOWORD(m_iEndRow))
	{
		if(iRow < l)
			m_pDisplayRow = Convert(iRow2 , m_iStartRow);
		Convert((UINT32)MIN(iRow2 + m_iScreenRows , m_iAppearRows) , (UINT32)m_iEndRow);
		if(bRedraw)
		{
			PaintGrid();
			PaintRows();
		}
	}
	if(m_iAppearRows <= m_iScreenRows)
	{
		if(m_pScrollBar)
		{
		    DeleteScrollbar();
			PaintGrid();
		}
	}
	else
	    m_pScrollBar->SetPerameter(m_iAppearRows , m_iScreenRows, (UINT16)iRow2 -1, bRedraw);

}

void CGrid::RemoveAll()
{
	if(m_iActualRows < 1)
		return;
	GridRowData *pRow ,*pRow2;
	UINT16 i;
	pRow = pRow2 = m_pRowHead->pNext;
	for(i = 0 ; i< m_iActualRows; i++)
	{
        if(pRow)
		   pRow2 = pRow->pNext;
		FreeRow(pRow);
		pRow = pRow2;
	}
	m_pRowHead->pNext = NULL;
	m_pDisplayRow= NULL;
	m_iActualRows = m_iAppearRows = 0;
	m_iStartRow = m_iEndRow = 1;
	m_iSelectedRow = m_iSelectedCol = -1;
    if(m_pScrollBar)
		DeleteScrollbar();
    PaintGrid();
}


void CGrid::FreeRow(GridRowData *pRow)
{
	if(!pRow)
		return;
	UINT16 col;
//	GridRowData *pRow2;
    for(col = 0 ; col < m_iTotalCols ; col++)
	{
	    if(pRow->pColData[col].pText)
			SysFree(pRow->pColData[col].pText);
	}
	SysFree(pRow);	
}

void CGrid::OnPaint(CDC &dc)
{
//	dc.SetBackColor(GRID_BACK_COLOR);
//	dc.EraseRect(m_sRect);
    PaintGrid();
    PaintRows();
	if(m_pScrollBar)
		m_pScrollBar->OnPaint(CDC());
}

void CGrid::PaintEmptyRows()
{
//	if(m_iAppearRows >= m_iScreenRows)
//		return;
	CDC dc;
	INT16 x, y;
	if(m_dwStyle & TBS_SPACELINE)
	{
		CRect r(m_sRect);
		r.top = r.top + TABLE_ROW_HEIGHT * m_iAppearRows;
//		r.DeflateRect(2, 0);
		dc.SetBackColor(GRID_BACK_COLOR);
		dc.EraseRect(r);
		dc.SetPenColor(LINE_COLOR);
		dc.SetLineStyle(1);
		y = r.top;
		//画横线
		while(y < m_sRect.bottom)
		{
			dc.MoveTo(r.left+2 , y);
			dc.LineTo(r.right-2 , y);
			y += TABLE_ROW_HEIGHT;
		}
//		if(m_dwStyle & TBS_ROWSELECT);
//			dc.SetLineStyle(1);
		if(!(m_dwStyle & TBS_NOVERLINE))
		{
			dc.SetPenColor(LINE_COLOR);
			dc.SetLineStyle(1);
		}
		else
		{
			dc.SetPenColor(GRID_BACK_COLOR);
			dc.SetLineStyle(0);
		}
		//画竖线
		x = r.left + m_iColsWidth[0]; 
		//	dc.SetLineStyle(0);
		for(UINT16 col = 1; col < m_iTotalCols  ; col++)
		{
			dc.MoveTo(x , r.top);
			dc.LineTo(x , r.bottom);
			x += m_iColsWidth[col];
		}
	}
}

	
	
void CGrid::PaintGrid()
{
	int x, y ,i;
	CDC dc;
    GridRowData *pRow = m_pDisplayRow;
	
	dc.Reset();
	if(m_dwStyle & WS_BORDER)
		dc.FrameRect(m_sRect);
	CRect r(m_sRect.left + 1, m_sRect.top +1 , m_sRect.right-1 , m_sRect.bottom -1);
	
	if(m_dwStyle & TBS_SPACELINE)
	{
//		dc.SetLineStyle(1);
		//擦横线
		if(pRow)
		{
			dc.SetLineStyle(0);
			dc.SetPenColor(GRID_BACK_COLOR);
			y = m_sRect.top + TABLE_ROW_HEIGHT;
			for(i = 0 ; i<m_iScreenRows -1 ;i++)
			{
				dc.MoveTo(m_sRect.left , y);
				dc.LineTo(m_sRect.right , y);
				y += TABLE_ROW_HEIGHT;
			}
			//		if(m_dwStyle & TBS_SPACELINE)
			//画横线
			dc.SetPenColor(LINE_COLOR);//GRID_FORE_COLOR
			dc.SetLineStyle(1);
			y = m_sRect.top + (pRow->cChilds - HIWORD(m_iStartRow) +1) *TABLE_ROW_HEIGHT;
			if(y < m_sRect.bottom)
			{
				dc.MoveTo(m_sRect.left , y);
				dc.LineTo(m_sRect.right , y);
			}
			for( ; pRow && y < m_sRect.bottom ; )
			{
				pRow = pRow->pNext;
				y +=(pRow->cChilds +1) * TABLE_ROW_HEIGHT;
				if(y < m_sRect.bottom)
				{
					dc.MoveTo(m_sRect.left , y);
					dc.LineTo(m_sRect.right , y);
				}   
			} 
			
			//画竖线
			if(!(m_dwStyle & TBS_NOVERLINE))
			{
				dc.SetPenColor(LINE_COLOR);
				dc.SetLineStyle(1);
			}
			else
			{
				dc.SetPenColor(GRID_BACK_COLOR);
				dc.SetLineStyle(0);
			}
//			if(m_dwStyle & TBS_ROWSELECT);
			
			x = m_sRect.left + m_iColsWidth[0]; 
			//	dc.SetLineStyle(0);
			for(UINT16 col = 1; col < m_iTotalCols  ; col++)
			{
				dc.MoveTo(x , m_sRect.top);
				dc.LineTo(x , m_sRect.bottom);
				x += m_iColsWidth[col];
			}
		}
		if(m_iAppearRows ==0 )// m_iScreenRows )//画空行
			PaintEmptyRows();
	}
}        

void CGrid::SetColsWidth(UINT16 *pWidth , BOOL bRedraw)
{
	UINT16 totalwidth = 0;
	UINT16 col , row,i;
	GridRowData *pRow;
	for(col = 0; col < m_iTotalCols ; col++)
	{
		 if(col < m_iTotalCols -1 && pWidth[col] < SB_WIDTH)
			 return;
	     totalwidth+= pWidth[col];
	}
	if(totalwidth > m_sRect.Width())
	//	ResetColsWidth();
		return;
	for(col = 0 ; col < m_iTotalCols-1 ; col++)
		m_iColsWidth[col] = pWidth[col];
	m_iColsWidth[col] = m_sRect.Width() -1 - (totalwidth - pWidth[col]);
	pRow = m_pRowHead->pNext;
	UINT16 maxnum  , unitLen, num, num2;
	UINT32 pSubAddr[MAX_SUB_LINES];
	row = 0, i =0;
	m_iStartRow &= 0x0FFFF;
	while(pRow)
	{
		maxnum = 1;
		for(col = 0 ; col < m_iTotalCols; col++)
		{
	   	   unitLen = (m_iColsWidth[col]-2) / ASC_WIDTH;
		   if(pRow->pColData[col].style & UNS_COMBOX)
			   continue;
           num = GetSubStringAddr(pRow->pColData[col].pText , unitLen,pSubAddr);
		   if(num > maxnum)
		    	maxnum = num;
		}
		pRow->cChilds = maxnum -1;
	
		if(++i == m_iStartRow)
			num2 = row;	
		row += maxnum;
		pRow= pRow->pNext;
	}
	m_iAppearRows = row;
	if(num2 ==0) num2 = 1;
	if(m_iAppearRows > m_iScreenRows)
	{
//		num = UnConvert(m_iStartRow);
		if(!m_pScrollBar)
		{
			CPoint pt(m_sRect.right +1 , m_sRect.top);
			m_pScrollBar = new CScrollBar;
			m_pScrollBar->Create(pt , m_sRect.Height(), (CFrameWnd *)m_pParent , 
				 this , GRID_IDSCROLLBAR);
			m_pScrollBar->SetPerameter(m_iAppearRows , m_iScreenRows , 
				   (UINT16)num2 -1);
			if(bRedraw)
				m_pScrollBar->OnPaint(CDC());

		}
		else
			m_pScrollBar->SetPerameter(m_iAppearRows , m_iScreenRows , 
				   (UINT16)num2 -1, bRedraw);
	}
	else
	{
		if(m_pScrollBar && bRedraw)
			DeleteScrollbar();
		m_iStartRow = 1;
	}

	if(bRedraw)
	{
		PaintGrid();
		PaintRows();
	}
}

INT16 CGrid::GetColsWidth(UINT16 col)
{
	if(col <1 || col > m_iTotalCols)
		return -1;
	return m_iColsWidth[col-1];
}

GridRowData *CGrid::GetRowPointer(UINT16 iRow) 
// 得到指定行的头指针
{
	if(iRow <1 || iRow > m_iActualRows)
		return NULL;
	UINT16 l , t,i;
	GridRowData *pRow;
	l = LOWORD(m_iStartRow);
	//为了加快搜索速度，从显示行搜索。
	if(iRow >= l)
	{
		t = l;
		pRow = m_pDisplayRow;
	}
	else
	{
		t = 1;
		pRow = m_pRowHead->pNext;
	} 

	for(i = t; i< iRow ; i++)
		pRow= pRow->pNext;
	return pRow;
}


GridRowData *CGrid::Convert(UINT32 appeRow , UINT32 &pcrow) 
// 把总行数转换成父行的索引和子行的索引
{
     UINT16 i;
	 UINT32 row = 0;
	 GridRowData *pRow ;
	 if(m_iActualRows==0)
	 {
		 pcrow = 0;
		 return NULL;
	 }
	 pRow= m_pRowHead->pNext;
	 for(i = 1 ; i<= m_iActualRows; i++)
	 {
		 row+= (pRow->cChilds+ 1);
		 if(row >= appeRow) //
			 break;
		 pRow = pRow->pNext;
	 }
     pcrow = MAKELONG(i , (pRow->cChilds  - row + appeRow));

	 return pRow;
}

UINT32 CGrid::UnConvert(UINT32 iRow) 
{
     UINT16 i ;
	 UINT32 prow = LOWORD(iRow) , crow = HIWORD(iRow) +1 , row = 0;
	 GridRowData *pRow = m_pRowHead->pNext;
	 for(i = 1 ; i< prow; i++)
	 {
		 row+= (pRow->cChilds+ 1);
		 pRow = pRow->pNext;
	 }
     row += crow;
	 return row;
}
	 
void CGrid::ScrollTo(UINT32 iRow)
{
	UINT32 row ;
	m_pDisplayRow = Convert(iRow , row);
	if(row == m_iStartRow)
		return;
    m_iStartRow = row;
	Convert((UINT32)MIN(iRow + m_iScreenRows -1 , m_iAppearRows), (UINT32)m_iEndRow);
//	ResetRowComboxes();
	PaintGrid();
	PaintRows();
}

void CGrid::GetUnitFromPt(INT16 x, INT16 y, int &iRow, int &iCol)
//考虑用指针记录当前行 , 返回的行数是总的行数（父行＋子行）
{
	UINT16 i, k ;
	iRow = (y - m_sRect.top)/ TABLE_ROW_HEIGHT ;
	x -= m_sRect.left;
	for(i = 0 ; i< m_iTotalCols ;i++)
	{
		x -= m_iColsWidth[i];
		if(x < 0)
			break;
	}
	iCol = ((i + 1) <= m_iTotalCols? (i+1) : m_iTotalCols);// modified by hqf 10.24
	k = 0;
	GridRowData *pRow = m_pRowHead->pNext;
    for(i = 1 ; i< LOWORD(m_iStartRow) ; i++)
	{
        k += (pRow->cChilds +1);
        pRow = pRow->pNext;
	}
	iRow += (k  + HIWORD(m_iStartRow) +1);
}

BOOL CGrid::GetRectFromUnit(int iRow , int iCol , CRect &rcUnit)
//iRow的低16位表示父行索引，高16位表示子行索引。
{
	int j;
	UINT16 l ;
	GridRowData *pRow;
	if(LOWORD(iRow) < LOWORD(m_iStartRow) || iCol < 1)
	{
		rcUnit.SetRect(0 , 0 , 0 ,0);
		return FALSE;
	}
	l = LOWORD(iRow) ;
	if(l > m_iActualRows || m_iActualRows == 0)
	{
		rcUnit.SetRect(0 , 0 , 0, 0);
		return FALSE;
	}
	rcUnit.left = m_sRect.left;
	for(j = 1 ; j< iCol; j++)
	   rcUnit.left +=m_iColsWidth[j-1];
    rcUnit.right = rcUnit.left + m_iColsWidth[j-1];
	if(l == LOWORD(m_iStartRow))
	{
       rcUnit.top = m_sRect.top;
	   rcUnit.bottom = rcUnit.top + (m_pDisplayRow->cChilds - HIWORD(m_iStartRow) +1) *TABLE_ROW_HEIGHT;
	   if(rcUnit.bottom > m_sRect.bottom)
		   rcUnit.bottom = m_sRect.bottom;
	   pRow = m_pDisplayRow;
	  
	}
	else if(l <= LOWORD(m_iEndRow))
	{
       UINT16 s;
	   pRow = m_pDisplayRow->pNext;
	   s = m_pDisplayRow->cChilds - HIWORD(m_iStartRow) +1;
	   for(j = LOWORD(m_iStartRow) +1 ; j< l ; j++)
	   {
		   s += (pRow->cChilds +1);
		   pRow= pRow->pNext;
	   }
       rcUnit.top = m_sRect.top + s * TABLE_ROW_HEIGHT;
 	   j = rcUnit.top + (pRow->cChilds +1) *TABLE_ROW_HEIGHT;
	   rcUnit.bottom = MIN(j , m_sRect.bottom);
	}
	else
	{
		rcUnit.SetRect(0 , 0 , 0, 0);
		return FALSE;
	}
    rcUnit.DeflateRect(1, 1);
	return TRUE;
}

BOOL CGrid::GetRectFromClick(int iRow , int iCol , CRect &rcUnit)
{
	if(iRow <1 || iRow > m_iActualRows || iCol > m_iTotalCols)
	{
		rcUnit.SetRect(0,0 ,0, 0);
		return FALSE;
	}
	GetRectFromUnit(iRow, iCol , rcUnit);
	rcUnit.bottom = rcUnit.top + TABLE_ROW_HEIGHT -2;
	rcUnit.InflateRect(1, 1);
	return TRUE;
}

void CGrid::WindowProcess(UINT32 nMessage , UINT32 wParam , UINT32 lParam)
{
	switch(nMessage)
	{
	case CN_SCROLLTO:
       if(wParam == GRID_IDSCROLLBAR)
	   {
		   UINT32 curRow = LOWORD(lParam) +1;
           ScrollTo(curRow);
	   }
	   break;
	   case WM_PENDOWN:
	   case WM_PENMOVEIN:
	   case WM_PENMOVEOUT://此处需要改动
		   lParam = nMessage;
		   DoPenDown(nMessage , wParam , lParam);
		   break;
	   case WM_PENMOVE:
		   lParam = nMessage;
		   DoPenMove(nMessage , wParam , lParam);
		   break;
	   case WM_PENUP:
		   lParam = nMessage;
		   DoPenUp(nMessage , wParam , lParam);
		   break;

	   
	}
}
 
void CGrid::DoPenDown(UINT32 nMessage , UINT32 wParam , UINT32 lParam)
//此函数在组合框下拉遮住下面几个Item ，并且最下端的Item可见时，
//点击此Item时，可能会有bug
{
	INT16  x, y ;
	x = HIWORD(wParam);
	y = LOWORD(wParam);
	int iRow , iCol , i ;
	UINT32 iRow2 , unStyle;
    CRect rcUnit;
	CDC dc;

	if(nMessage == WM_PENMOVEIN || nMessage == WM_PENDOWN)//|| nMessage == WM_PENMOVEOUT)
	{	
	    GetUnitFromPt(x , y , iRow , iCol);
		Convert(iRow , iRow2);
//		if(LOWORD(iRow2) > m_iActualRows)
//			return;
		if(!GetRectFromUnit(iRow2 , iCol , rcUnit))
			return;
		m_iCurRow = iRow2;
		m_iCurCol = iCol;
	
		if(m_dwStyle & TBS_ROWSELECT)
			rcUnit.left = m_sRect.left +1  , rcUnit.right = m_sRect.right -1;
	    dc.InvertRect(rcUnit);
		if(m_iSelectedRow!= -1 && m_iSelectedCol != -1)
		{
			GetRectFromUnit(m_iSelectedRow , m_iSelectedCol , rcUnit);
			if(m_dwStyle & TBS_ROWSELECT)
				rcUnit.left = m_sRect.left +1  , rcUnit.right = m_sRect.right -1;
			dc.InvertRect(rcUnit);
		}
		m_iSelectedRow = m_iCurRow , m_iSelectedCol = m_iCurCol;
	}
	else
	{
	   if(m_iCurRow ==-1 || m_iCurCol == -1 || m_pDisplayRow== NULL)
		   return;
	   BOOL bTemp;
	   GridRowData *pRow = m_pDisplayRow;
	   for(i = LOWORD(m_iStartRow); i< LOWORD(m_iCurRow); i++)
		   pRow= pRow->pNext;
	   bTemp = m_bCanSel;
	   if((m_dwStyle & TBS_ROWSELECT) &&(pRow->style & RS_CANNOTSELECT))
		   m_bCanSel = FALSE;
	   if(lParam == WM_PENMOVEOUT )//&& !m_bCanSel)
	   {
		   if(!GetRectFromUnit(m_iCurRow ,  m_iCurCol , rcUnit))
				   return;
		   if(m_dwStyle & TBS_ROWSELECT)
				rcUnit.left = m_sRect.left +1  , rcUnit.right = m_sRect.right -1;
		   dc.InvertRect(rcUnit);
		   m_iSelectedRow = m_iSelectedCol = -1;
		   m_iCurRow = m_iCurCol = -1;
	   }
	   else if(lParam == WM_PENUP)
	   {   
		   if(!m_bCanSel)
		   {
			   if(!GetRectFromUnit(m_iCurRow ,  m_iCurCol , rcUnit))
				   return;
 			   if(m_dwStyle & TBS_ROWSELECT)
				  rcUnit.left = m_sRect.left +1  , rcUnit.right = m_sRect.right -1;
			   dc.InvertRect(rcUnit);
			   m_iSelectedRow = m_iSelectedCol = -1;
		   }
		   if(HIWORD(m_iCurRow)==0 )
		   {
			   if(LOWORD(m_iCurRow) <= m_iActualRows)
				if(pRow->pColData[m_iCurCol -1].style & UNS_COMBOX)
					  m_pParent->SendMessage(CN_GRIDDROPDOWN , m_nCtrlId, MAKELONG(m_iCurRow,m_iCurCol));
				else
					  m_pParent->SendMessage(CN_GRIDSEL , m_nCtrlId, MAKELONG(m_iCurRow,m_iCurCol)); 
		   }
		   else if(LOWORD(m_iCurRow) <= m_iActualRows)
               m_pParent->SendMessage(CN_GRIDSEL , m_nCtrlId, MAKELONG(LOWORD(m_iCurRow),m_iCurCol));  
		   m_iCurRow = m_iCurCol = -1;
	   }
	   m_bCanSel = bTemp;
	}
}

void CGrid::DoPenMove(UINT32 nMessage , UINT32 wParam , UINT32 lParam)
{
	INT16 x , y ;
	x = HIWORD(wParam) ;
	y = LOWORD(wParam);
	int iRow , iCol ;
	UINT32 iRow2;
	CRect rcUnit;
    GetUnitFromPt(x , y , iRow, iCol);
	Convert(iRow,iRow2);

	if(m_dwStyle & TBS_ROWSELECT)
	{
		if(LOWORD(iRow2)== LOWORD(m_iCurRow))
			return;
	}
	else if(LOWORD(iRow2) == LOWORD(m_iCurRow) && iCol == m_iCurCol)
	     return;
	//如果在当前已反白的单元格内移动，返回。
    CDC dc;
	if(!GetRectFromUnit(m_iCurRow , m_iCurCol , rcUnit))
		return;
	if(m_dwStyle & TBS_ROWSELECT)
		rcUnit.left = m_sRect.left +1 , rcUnit.right = m_sRect.right -1;
	dc.InvertRect(rcUnit);
	m_iCurRow = m_iCurCol = -1;
	m_iSelectedRow = m_iSelectedCol = -1;
	DoPenDown(WM_PENMOVEIN , wParam , lParam);
}

void CGrid::DoPenUp(UINT32 nMessage , UINT32 wParam , UINT32 lParam)
{
    DoPenDown(WM_PENMOVEOUT , wParam , lParam);
}

void CGrid::SetStartRow(INT16 iRow , BOOL bRedraw)
{
	if(iRow <=0) 
		iRow =1;
	else if(iRow > m_iActualRows)
		iRow = m_iActualRows;
	UINT32 row ;
	m_pDisplayRow = GetRowPointer(iRow);//ygr
	m_iStartRow = iRow;
	if (bRedraw){	// byw
		PaintGrid();
		PaintRows(); 
	}
	m_pScrollBar->SetPerameter(m_iAppearRows , m_iScreenRows ,
				iRow , bRedraw);
}

void CGrid::PaintRows()
{
	UINT32 row, prow;
	UINT16 i, y ;
	GridRowData *pRow = m_pDisplayRow , *pRow2;
	i = 0;
	if(!m_pDisplayRow)
		return;
    for( ; ; )
	{
	   for(row = HIWORD(m_iStartRow); row <= pRow->cChilds; row++)
	   {
	   	   PaintRow(MAKELONG(LOWORD(m_iStartRow) , row));
		   i++;
		   if(i== m_iScreenRows)
		   {
			   m_iEndRow = MAKELONG(LOWORD(m_iStartRow) , row);
			   return;
		   }
	   }
	   pRow2 = pRow;
	   pRow= pRow->pNext;
	   prow = LOWORD(m_iStartRow) +1;
       while(pRow)
	   {
		   for(row = 0 ; row<= pRow->cChilds; row++)
		   {
			   PaintRow(MAKELONG(prow, row));
		       i++;
		       if(i== m_iScreenRows)
			   {
			      m_iEndRow = MAKELONG(prow , row);
				  return;
			   }
		   }
		   pRow2 = pRow;
		   pRow = pRow->pNext;
		   prow++;
	   }
       break;			   
	}
	m_iEndRow = MAKELONG(m_iActualRows , pRow2->cChilds);
	y = m_sRect.top + i *TABLE_ROW_HEIGHT;
	if(y < m_sRect.bottom )//m_iAppearRows >= m_iScreenRows &&
	{
		CRect r(m_sRect.left +1 , y +1 , m_sRect.right -1 , m_sRect.bottom -1);
		CDC dc;
		dc.SetBackColor(GRID_BACK_COLOR);
		dc.EraseRect(r);
	}
//END: return;
}

void CGrid::PaintRow(UINT32 iRow) 
//iRow的低16位表示父行的索引，高16位表示子行的索引 
{
	UINT16 prow ,crow, row, col , l, h, x ,y;
	GridRowData *pRow ;

	prow = LOWORD(iRow);
	crow = HIWORD(iRow);
	l = LOWORD(m_iStartRow);
	h = HIWORD(m_iStartRow);
	if(prow < l ||(prow ==l && crow < h))
		return;
    if(!(pRow= m_pDisplayRow))
		return;
	x = m_sRect.left;
	y = m_sRect.top ;
	 
	for(row = l ; row < prow; row++)
	{
		if(row == l)
		   y += (pRow->cChilds +1 - h) * TABLE_ROW_HEIGHT;
		else 
		   y += (pRow->cChilds +1) * TABLE_ROW_HEIGHT;
		if(y >= m_sRect.bottom)
			return;
		pRow = pRow->pNext;
	}
	
	CRect rcUnit;
	CDC dc;
	
    if(crow!=RD_DRAWALL)
	{
		if(prow > l)
		   y += crow * TABLE_ROW_HEIGHT;
		else 
		   y += (crow - h) * TABLE_ROW_HEIGHT;
		rcUnit.SetRect(x  , y  , x + m_iColsWidth[0] , y + TABLE_ROW_HEIGHT);
		if(rcUnit.bottom > m_sRect.bottom)
			return;
		rcUnit.DeflateRect(1, 1);
		
		for(col = 0 ; col < m_iTotalCols ; col++)
		{
/*			if(m_iSelectedRow == prow && m_iSelectedCol == col)
			{
				dc.SetBackColor(3 - GRID_BACK_COLOR);
				dc.SetPenColor(3- GRID_FORE_COLOR);
				dc.EraseRect(rcUnit);
			}
			else*/
			{
				dc.SetBackColor(GRID_BACK_COLOR);
				dc.SetPenColor(GRID_FORE_COLOR);
				dc.EraseRect(rcUnit);
			}
//			rcUnit.DeflateRect(1, 1);
			if(pRow->pColData[col].style & UNS_COMBOX)
			{
				if(crow ==0)
				{
					rcUnit.bottom = rcUnit.top + TABLE_ROW_HEIGHT -2;
					CRect r1(rcUnit.right- 16, rcUnit.top, rcUnit.right-1, rcUnit.bottom -1);  
//					dc.SetPenColor(GRID_FORE_COLOR);
                    if(	pRow->pColData[col].nIconIndex)
                        dc.DrawText(rcUnit, pRow->pColData[col].pText +2); 
				    else
						dc.DrawText(rcUnit, pRow->pColData[col].pText ); 
//					dc.FrameRect(r1);
//					r1.DeflateRect(1, 1);
					dc.SetBackColor(LGRAY);
					dc.EraseRect(r1);
					r1.DeflateRect(1, 1);
					dc.PutIcon(r1.left+1, r1.top+1, ICON_DOWNARROW);  
					dc.Draw3dRect(r1);
//					dc.SetBackColor(GRID_BACK_COLOR);
				}
			}
			else
			{
			   UINT32 index = pRow->pColData[col].nIconIndex;
			   INT16 temp , unitLen;
                  UINT32 pSubAddr[MAX_SUB_LINES]= {0};
				  char   pSubText[32];
				  unitLen = (m_iColsWidth[col]-2) / ASC_WIDTH ;
		          GetSubStringAddr(pRow->pColData[col].pText ,  unitLen, pSubAddr);
				  temp = (INT16)(pSubAddr[crow +1] - pSubAddr[crow]);
				  if(pSubAddr[crow +1] != 0 && temp>0)
				  {
				     CRect r(rcUnit.left +1 , rcUnit.top , rcUnit.right ,rcUnit.bottom );
					 if(index != INVALID_ICON_ID && crow ==0)
					 {
                        dc.PutIcon(r.left , r.top  , index);
				        memcpy(pSubText, (char *)(pSubAddr[crow]+2) ,temp-2);
				        pSubText[temp -2]= '\0';
						r.left += GRID_ICON_WIDTH;
                        dc.DrawText(r, pSubText);
                        r.left -= GRID_ICON_WIDTH; 
					 }
					 else 
					 {
						 memcpy(pSubText, (char *)pSubAddr[crow] ,temp);
				         pSubText[temp]= '\0';
						 dc.DrawText(r, pSubText);
					 }
				  }
			}
   		    if(!(m_dwStyle & TBS_ROWSELECT)&& 
				prow == LOWORD(m_iSelectedRow) && col +1 == m_iSelectedCol)
			{
			    if(crow != pRow->cChilds)
				   rcUnit.bottom +=1;
				dc.InvertRect(rcUnit);
			}
		    if(col == m_iTotalCols-1)
			{
				if((m_dwStyle & TBS_ROWSELECT ) && prow == LOWORD(m_iSelectedRow))
				{
					rcUnit.left = m_sRect.left +1 , rcUnit.right = m_sRect.right -1;
					if(crow != pRow->cChilds)
						rcUnit.bottom +=1;
					dc.InvertRect(rcUnit);
				}
				break;
			}
		    else
			{
			   rcUnit.left += m_iColsWidth[col];
			   rcUnit.right = rcUnit.left + m_iColsWidth[col+1] -2;
//			   rcUnit.InflateRect(1, 1);
			}
		}
	}
	else
	{
		if(pRow->style & RS_DISABLE)
			return;
		for(row = 0 ; row <= pRow->cChilds; row ++)
		{
			UINT32 temp = MAKELONG(prow , row); 
			PaintRow(temp);
		}
	}
}

UINT16 CGrid::GetTotalRows()
{
	return m_iActualRows;
}