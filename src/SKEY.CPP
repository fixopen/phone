/******************************************************************************/
/*                                                                            */
/*  版权所有(c)  2001汉王科技                                                 */
/*                                                                            */
/******************************************************************************/
/*  文 件 名                                                                  */
/*             softkey.cpp                                                    */
/*                                                                            */
/*  版 本 号                                                                  */
/*             1.0                                                            */
/*                                                                            */
/*  描    述                                                                  */
/*             汉王科技E-PhoneII窗口类库的源文件                              */
/*             HanWang Technology E-PhoneII Function Class Library source file*/
/*                                                                            */
/*  组    成                                                                  */
/*             软键盘控件类源文件                                             */
/*                                                                            */
/*  作    者                                                                  */
/*             王飞                                                           */
/*                                                                            */
/*  日    期                                                                  */
/*             2001-07-21                                                     */
/*                                                                            */
/*  历史记录                                                                  */
/*             2001-08-07  修正destroy时的BUG                                 */
/*                                                                            */
/*             2001-08-08  把每个按键的键值改为16位                           */
/*                                                                            */
/*             2001-11-01  修改DoPenDown的一个BUG                             */
/*             2001-11-15  郝庆丰修改CClock的主要界面                         */
/******************************************************************************/
#include "softkey.h"
////////////////////////////////////////////////////////////////////////////////
//
INT32 CSoftKey::GetPenKey(CPoint &pt)
{
	KEYSTRUCT * pKey = m_pKeys;
	for(int i=0; i < m_nKeyCount; i++, pKey++)
       if(pt.InRect(pKey->sKeyRect))
		   return i;
	return -1;
}

////////////////////////////////////////////////////////////////////////////////
//
void CSoftKey::DoPenDown(CPoint &pt)
{
	INT32 sel = GetPenKey(pt);
	if(sel == m_nCurSel)
		return;
	if(m_nCurSel >= 0)
	{
		CDC::InvertRect(m_pKeys[m_nCurSel].sKeyRect);
		m_nCurSel = -1;
	}
	if(sel >= 0)
	{
		CDC::InvertRect(m_pKeys[sel].sKeyRect);
		m_nCurSel = sel;
	}
}
////////////////////////////////////////////////////////////////////////////////
//
void CSoftKey::DoPenUp()
{
	if(m_nCurSel >= 0)
	{
		CDC::InvertRect(m_pKeys[m_nCurSel].sKeyRect);
		CCtrl::SendNotifyMessage(WM_KEY, UINT32(m_pKeys[m_nCurSel].nKeyValue));   
		m_nCurSel = -1;
	}
}
////////////////////////////////////////////////////////////////////////////////
//
void CSoftKey::WindowProcess(UINT32 nMessage, UINT32 wParam, UINT32 lParam)
{
	switch(nMessage)
	{
		case WM_PENDOWN:
		case WM_PENMOVE:
			DoPenDown(CPoint(HIWORD(wParam), LOWORD(wParam)));
			break;
		case WM_PENUP:
			DoPenUp();
			break;
		case WM_PENMOVEOUT:
			if(m_nCurSel >= 0)
	        	CDC::InvertRect(m_pKeys[m_nCurSel].sKeyRect);
			m_nCurSel = -1;
			break;
	}
}
////////////////////////////////////////////////////////////////////////////////
//
void CSoftKey::OnPaint(CDC &dc)
{
	dc.PutIcon(m_sRect.left, m_sRect.top, m_nBkBmpId);
}
////////////////////////////////////////////////////////////////////////////////
//
BOOL CSoftKey::Create(UINT32      nBkBmpId, 
					  UINT32      nKeyCount, 
					  CRect     * pKeyRectArray, 
					  UINT16    * pKeyValueArray, 
					  CFrameWnd * pParent, 
					  UINT32      nCtrlId)
{
	m_pKeys  = new KEYSTRUCT[nKeyCount];
	if(!m_pKeys)
		return FALSE;
	CRect ctrRect(9999, 9999, 0,0);
	for(int i=0; i <nKeyCount; i++)
	{
		if(ctrRect.left > pKeyRectArray[i].left) 
		   ctrRect.left = pKeyRectArray[i].left;

		if(ctrRect.top  > pKeyRectArray[i].top)
		   ctrRect.top  = pKeyRectArray[i].top;

		if(ctrRect.right < pKeyRectArray[i].right)
		   ctrRect.right = pKeyRectArray[i].right;

		if(ctrRect.bottom < pKeyRectArray[i].bottom)
		   ctrRect.bottom = pKeyRectArray[i].bottom; 

		m_pKeys[i].sKeyRect  = pKeyRectArray[i];
		m_pKeys[i].nKeyValue = pKeyValueArray[i];
	}

	if(!CCtrl::Create(NULL, ctrRect, pParent, WS_CHILD|WS_VISIBLE,nCtrlId))
	{
		delete[] m_pKeys;
		return FALSE;
	} 
	m_nBkBmpId  = nBkBmpId;
	m_nKeyCount = nKeyCount;
	m_nCurSel   = -1;
	return TRUE;
}
////////////////////////////////////////////////////////////////////////////////
//
void CSoftKey::SetParameters(UINT32 nBkBmpId, UINT16 *pKeyValueArray, BOOL bRedraw)
{
  m_nBkBmpId = nBkBmpId;
  if(pKeyValueArray)
  {
	  for(int i=0; i< m_nKeyCount; i++)
		  m_pKeys[i].nKeyValue = pKeyValueArray[i];
  }
  if(bRedraw)
	  OnPaint(CDC());
}
////////////////////////////////////////////////////////////////////////////////
//
void CSoftKey::Destroy()
{
	delete[] m_pKeys;
	CCtrl::Destroy();
}
////////////////////////////////////////////////////////////////////////////////
//end softkey.cpp

/******************************************************************************/
/*                                                                            */
/*  版权所有(c)  2001汉王科技                                                 */
/*                                                                            */
/******************************************************************************/
/*  文 件 名                                                                  */
/*             calendar.cpp                                                   */
/*                                                                            */
/*  版 本 号                                                                  */
/*             1.0                                                            */
/*                                                                            */
/*  描    述                                                                  */
/*             汉王科技E-PhoneII窗口类库的源文件                              */
/*             HanWang Technology E-PhoneII Function Class Library source file*/
/*                                                                            */
/*  组    成                                                                  */
/*             日历控件类源文件                                               */
/*                                                                            */
/*  作    者                                                                  */
/*             王飞                                                           */
/*                                                                            */
/*  日    期                                                                  */
/*             2001-07-21                                                     */
/*                                                                            */
/*  历史记录                                                                  */
/*2001/12/10 //清空选中区域--程晓东添加                                       */
/******************************************************************************/

/////////////////////////////////////////////////////////////////////////////////
//
void CCalendar::WindowProcess(UINT32 nMessage, UINT32 wParam, UINT32 lParam)
{
	if(m_dwStyle & WS_DISABLE)
		return;
    switch(nMessage)
    {
    case WM_PENDOWN:
    case WM_PENMOVE:
		 SelectDay(CalcPenDay(CPoint(HIWORD(wParam), LOWORD(wParam))), TRUE);
         break;
    case WM_PENMOVEOUT:
		if(AlarmUseFlag==TRUE)//cxd add 判断是否在提醒中使用
		{
			CDC::InvertRect(m_selRect);
			m_selRect=0;
			break;
		}
		
    case WM_PENUP:
		 if(AlarmUseFlag==TRUE)//cxd add 判断是否在提醒中使用
			CDC::InvertRect(m_selRect);
       	 if(m_selRect !=0 && m_nPenSelDay != 0)
            SendNotifyMessage(CN_COMMAND, UINT32(m_nPenSelDay));  // hqf
         if(AlarmUseFlag==TRUE)//cxd add 判断是否在提醒中使用
			m_selRect=0;
       	 break; 
    }
}
#define CTITLE_HEIGHT 17
#define COL_WIDTH    22
#define ROW_HEIGHT   17 
/////////////////////////////////////////////////////////////////////////////////
//
void CCalendar::SetDate(INT16 nYear, INT16 nMonth, INT16 nDay)
{
	if(TESTDAY(nYear, nMonth, nDay))
	{
		m_nYear = nYear; m_nMonth = nMonth; m_nDay = nDay;
	}
}
/////////////////////////////////////////////////////////////////////////////////
//

BOOL CCalendar::Create(CPoint &ptLeftTop, CFrameWnd *pParent, UINT32 nCtrlId,BOOL Flag)
{
	DateTime dt;
	GetSysTime(&dt);
	m_nYear   = dt.year;
	m_nMonth  = dt.month;
	m_nDay    = dt.day; 
	m_selRect = 0;
	AlarmUseFlag=Flag;//cxd add 判断是否在提醒中使用
	CRect ctrRect(ptLeftTop.x, ptLeftTop.y, ptLeftTop.x + COL_WIDTH * 7,  ptLeftTop.y + ROW_HEIGHT*7-1);
	return CCtrl::Create(NULL, ctrRect, pParent, WS_CHILD|WS_VISIBLE, nCtrlId); 
}
/////////////////////////////////////////////////////////////////////////////////
//
void CCalendar::OnPaint(CDC &dc)
{
	PaintDate(dc);
	PaintTitle(dc);
}
/////////////////////////////////////////////////////////////////////////////////
//
void CCalendar::PaintTitle(CDC &dc)
{
   INT16 x = m_sRect.left+4;
   char *text[7] = {"日","一","二","三","四","五","六"};
   for(int i=0; i<7; i++,x+= COL_WIDTH)
   {
       dc.TextOut(x, m_sRect.top, text[i]); 
	   if(i>=2 && i<=5)x++;
   }
}
/////////////////////////////////////////////////////////////////////////////////
//
void CCalendar::PaintOneDay(CDC &dc, INT16 nDay, UINT32 nIconId, INT16 x, INT16 y)
{   
	if(CDate::IsLunarHolDay(m_nYear, m_nMonth, nDay))
	{
		dc.SetBackColor(LGRAY); 
		dc.EraseRect(x+1, y, x+COL_WIDTH-1, y+ROW_HEIGHT-1);
		dc.SetROP2(DRAWMODE_AND); 
	}
	dc.PutIcon(x, y, (nDay-1) + DAY_ICON01); 
    dc.PutIcon(x+14, y, nIconId);
	dc.Reset(); 
}
/////////////////////////////////////////////////////////////////////////////////
//
void CCalendar::PaintDate(CDC &dc)
{
	UINT16 iLunarYear, iLunarMonth, iLunarDay;
	UINT16 nMonthDays = CDate::MonthDays(m_nYear, m_nMonth);
	UINT16 nStartWeek = CDate::WeekDay(m_nYear, m_nMonth, 1);
	INT8   bSecMonth  = CDate::CalcLunarDate(iLunarYear, iLunarMonth, iLunarDay, 
		                                          CDate::CalcDateDiff(m_nYear, m_nMonth, 1));
	INT16 iDay =1,starty =m_sRect.top+CTITLE_HEIGHT;
	for(int i=0; i<6; i++, starty += ROW_HEIGHT)
	{
		int startx = m_sRect.left +1;
		dc.EraseRect(m_sRect.left, starty, m_sRect.right+1, starty + ROW_HEIGHT-1); 
		for(int j=0; j<7; j++,startx += COL_WIDTH)
		{
			if(j < nStartWeek && i==0)
				continue;
			if(iDay > nMonthDays)
				break;

			PaintOneDay(dc, iDay++, LUNAR_ICON01+(iLunarDay!=1?(iLunarDay-1):(iLunarMonth+29)), startx, starty);

			if(iLunarYear <START_YEAR && iLunarDay == 29)
				iLunarDay = 1, iLunarMonth = iLunarMonth%12 + 1;
			else  if(iLunarDay<29)
				iLunarDay++;
			else
			{
				UINT32 days = CDate::LunarMonthDays(iLunarYear, iLunarMonth);
				if(iLunarDay >= (bSecMonth ? HIWORD(days) : LOWORD(days)))
				{
					iLunarDay = 1;
					if(bSecMonth || !HIWORD(days))
						iLunarMonth = (iLunarMonth)%12+1;
				}
				else
					iLunarDay ++;
			}
		}//end of j 
	} //end of i
	if(AlarmUseFlag==FALSE)//cxd add 判断是否在提醒中使用
		SelectDay(m_nDay);
}
/////////////////////////////////////////////////////////////////////////////////
//
UINT16 CCalendar::CalcPenDay(CPoint &pt)
{
    UINT16 iRow = (pt.y - m_sRect.top - CTITLE_HEIGHT)/ROW_HEIGHT;
    UINT16 iCol = (pt.x - m_sRect.left )/COL_WIDTH;

    UINT16 startcol = CDate::WeekDay(m_nYear, m_nMonth, 1);
    UINT16 endcol   = CDate::WeekDay(m_nYear, m_nMonth, CDate::MonthDays(m_nYear,m_nMonth));
    UINT16 endrow   = (CDate::MonthDays(m_nYear, m_nMonth) + startcol -1)/7 ;
    if(iRow == 0 && iCol < startcol || iRow == endrow && iCol > endcol ||  iRow > endrow)
        return 0;
    return iRow *7 + iCol + 1 - startcol ;
}
/////////////////////////////////////////////////////////////////////////////////
//
void CCalendar::SelectDay(UINT16 iDay, BOOL bUnSelLast)
{
	if(iDay)
    {
		CRect rec;
        UINT16 startcol = CDate::WeekDay(m_nYear, m_nMonth, 1);
	    UINT16 iRow = (iDay+startcol-1)/7;
	    UINT16 iCol = (iDay+startcol-1)%7;
	    rec.left   = m_sRect.left + iCol * COL_WIDTH+2;
	    rec.right  = rec.left + COL_WIDTH-1;
	    rec.top    = m_sRect.top + CTITLE_HEIGHT + iRow * ROW_HEIGHT;
	    rec.bottom = rec.top + ROW_HEIGHT-1;
		if(rec != m_selRect && m_selRect != 0 && bUnSelLast)
			CDC::InvertRect(m_selRect);
		if(rec != m_selRect)
		{
			CDC::InvertRect(rec);
			m_selRect = rec;
			m_nDay   = iDay; 
		}
		
    }
	m_nPenSelDay = iDay;

}
//清空选中区域--程晓东添加
/*
void CCalendar::ResetSelectRect()
{
	m_selRect=0;
}
*/
#undef COL_WIDTH  
#undef ROW_HEIGHT 
#undef CTITLE_HEIGHT
/////////////////////////////////////////////////////////////////////////////////
//enf of calendar.cpp

/******************************************************************************/
/*                                                                            */
/*  版权所有(c)  2001汉王科技                                                 */
/*                                                                            */
/******************************************************************************/
/*  文 件 名                                                                  */
/*             clock.cpp                                                        */
/*                                                                            */
/*  版 本 号                                                                  */
/*             1.0                                                            */
/*                                                                            */
/*  描    述                                                                  */
/*             汉王科技E-PhoneII窗口类库的头文件                              */
/*             HanWang Technology E-PhoneII Function Class Library head file  */
/*                                                                            */
/*  组    成                                                                  */
/*             时钟控件类源文件                                               */
/*                                                                            */
/*  作    者                                                                  */
/*             王飞                                                           */
/*                                                                            */
/*  日    期                                                                  */
/*             2001-10-15                                                     */
/*                                                                            */
/*  历史记录                                                                  */
/*             2001/11/14													  */	
/*             郝庆丰修改时钟界面                                             */ 
/******************************************************************************/
#include "hwfcl.h"
#include "datetime.h"

#define CLOCK_WIDTH  83
#define CLOCK_HEIGHT 83
#ifndef SWAP
#define  SWAP(a, b)  {(a) = (a)^(b), (b)=(b)^(a), (a)=(a)^(b);}
#endif

//指针的长度
const INT16 g_iHandLen[3]   = {25, 29, 30}; 
const INT16 g_iHandWidth[3] = {3, 2, 0};
const INT16 g_iHandLen1[3]  = {2, 4, 6};
//每相隔6度的sin值*10000取整后的数组
const int SIN_DATA[]=
{
    0,     1045,  2079,  3090,  
    4067,  4999,  5877,  6691,  
    7431,  8090,  8660,  9135,  
    9510,  9781,  9945,  10000
};
////////////////////////////////////////////////////////////////////////////////
//
void CUpDownButton::WindowProcess(UINT32 nMessage, UINT32 wParam, UINT32 lParam)
{
	if(m_dwStyle & WS_DISABLE)
		return;
	static int count, range;
    switch(nMessage)
    {
    case WM_PENMOVEIN:
    case WM_PENDOWN:
		count = NU_Retrieve_Clock();
		range = 60;
    case WM_PENMOVE:
        if(!m_bPressed)
        {
         CDC::InvertRect (m_sRect);
         m_bPressed = 1;
        }
		if(NU_Retrieve_Clock() - count > range)
		{
          SendNotifyMessage(CN_COMMAND, 0);
		  if(range>10)
			  range-=5;
		  count = NU_Retrieve_Clock();
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
        SendNotifyMessage(CN_COMMAND, 0);
		range = 60;
        }
        break; 
    }
}

////////////////////////////////////////////////////////////////////////////////
//
void CClock::SetTime(INT16 nHour, INT16 nMinute, INT16 nSecond)
{
	if(nHour>=0 && nHour<=23 && nMinute>=0 && nMinute<=59 && nSecond>=0 && nSecond<=59)
	{
		m_nHour   = nHour;
		m_nMinute = nMinute;
		m_nSecond = nSecond;
	}
}
////////////////////////////////////////////////////////////////////////////////
//
BOOL CClock::Create(CPoint &ptLeftTop, CFrameWnd *pParent, UINT32 nCtrlId)
{
	DateTime dt;
	GetSysTime(&dt);
	m_nHour = dt.hour;
	m_nMinute = dt.minute; 
	m_nSecond = dt.second; 
	m_nClockStyle  = 0;
	CRect ctrRect(ptLeftTop.x, ptLeftTop.y, ptLeftTop.x+CLOCK_WIDTH, ptLeftTop.y+CLOCK_HEIGHT);
	return CCtrl::Create(NULL, ctrRect, pParent, WS_CHILD|WS_VISIBLE, nCtrlId); 
}

////////////////////////////////////////////////////////////////////////////////
//
void CClock::GetDxDy(INT16 &dx, INT16 &dy, INT16 &dx1, INT16 &dy1, INT16 &dx2, INT16 &dy2,
                           INT16 iSubScript, INT16 iFlag, INT16 iHandType)
{
    INT16 *handlen   = (INT16 *)g_iHandLen;
    INT16 *handwidth = (INT16 *)g_iHandWidth;
    INT16 *handlen1  = (INT16 *)g_iHandLen1;

    dy = handlen[iHandType] * SIN_DATA[15-iSubScript]/10000;
    dx = handlen[iHandType] * SIN_DATA[iSubScript]/10000;

    dx1 = handwidth[iHandType] * SIN_DATA[15-iSubScript]/10000;
    dy1 = handwidth[iHandType] * SIN_DATA[iSubScript]/10000;

    dx2 = -handlen1[iHandType] * SIN_DATA[iSubScript]/10000;
    dy2 = -handlen1[iHandType] * SIN_DATA[15-iSubScript]/10000;
    switch(iFlag)
    {
    case 1:
        SWAP(dx, dy);
        dy = -dy;
        SWAP(dx2, dy2)
        dy2 = -dy2;
        SWAP(dx1, dy1);
        dx1 = -dx1;
        break;
    case 2:
        dy = -dy;
        dx = -dx;
        dy2 = -dy2;
        dx2 = -dx2;
        break;
    case 3:
        SWAP(dx,dy);
        dx = -dx;
        SWAP(dx2, dy2)
        dx2 = - dx2;
        SWAP(dx1, dy1);
        dy1 = -dy1;
        break;
    }
}

void CClock::GetDxDy(INT16 &dx, INT16 &dy, INT16 iLength , INT16 iAngle)
{
	BOOL bSinNeg = FALSE, bCosNeg = FALSE;
	INT16 iSinAngle = iAngle % 360 , iCosAngle = iAngle % 360;
	if(iSinAngle < 0)
		iSinAngle += 360;
	if(iSinAngle > 180)
	{
		bSinNeg = !bSinNeg;
		iSinAngle -= 180;
	}
	if(iSinAngle > 90)
		iSinAngle = 180- iSinAngle;
	dy = iLength * SIN_DATA[iSinAngle/6]/10000;
	dy = (bSinNeg ? (-dy) : dy);

	if(iCosAngle < 0)
		iCosAngle += 360;
	if(iCosAngle > 90 && iCosAngle < 270)
		bCosNeg = !bCosNeg;
	if(iCosAngle > 180)
		iCosAngle -= 180;
	if(iCosAngle > 90)
	    iCosAngle = 180 - iCosAngle;
	dx = iLength * SIN_DATA[15- iCosAngle/6]/10000;
	dx = (bCosNeg ? (-dx): dx);
}
////////////////////////////////////////////////////////////////////////////////
//
void CClock::OnPaint(CDC &dc)
{
	dc.PutIcon(m_sRect.left, m_sRect.top, ICON_CLOCK);

    INT16 rx = m_sRect.HCenter(), ry = m_sRect.VCenter();
    INT16 dx, dy, dx1, dy1,iAngle;//, dx2, dy2;
	float fTemp,fTemp2 = m_nMinute;
	//画时针
	fTemp = fTemp2 /12.0 + 0.5;
	iAngle = (m_nHour %12 -3) * (-30)- (INT16) fTemp * 6;
	GetDxDy(dx , dy , 11 , iAngle);
	iAngle += 180;
	GetDxDy(dx1, dy1 , 4 , iAngle);
	dc.SetPenWidth(3);
	dc.MoveTo(rx + dx1 , ry - dy1);
	dc.LineTo(rx + dx, ry -dy);
	//画分针
	iAngle = 90 - m_nMinute * 6;
	GetDxDy(dx , dy , 15 , iAngle);
	iAngle += 180;
	GetDxDy(dx1 , dy1 , 4, iAngle);
	dc.SetPenWidth(2);
	dc.MoveTo(rx +dx1 , ry - dy1);
	dc.LineTo(rx +dx , ry -dy);
	//画秒针
	if(m_nClockStyle == 0) 
	{
		iAngle = 90 - m_nSecond * 6;
		GetDxDy(dx , dy , 19 , iAngle);
		iAngle += 180;
		GetDxDy(dx1 , dy1 , 6, iAngle);
		dc.SetPenWidth(1);
		dc.MoveTo(rx +dx1 , ry - dy1);
		dc.LineTo(rx +dx , ry -dy);
	}
	/*
	//画时针
    GetDxDy(dx, dy, dx1, dy1,dx2, dy2, (m_nHour*5+(m_nMinute+3)/12)%15, ((m_nHour%12)*5+(m_nMinute+3)/12)/15, 0);
    dc.MoveTo(rx + dx, ry-dy); 
    dc.LineTo(rx - dx1, ry-dy1);
    dc.LineTo(rx + dx2, ry - dy2); 
    dc.LineTo(rx + dx1, ry+dy1); 
    dc.LineTo(rx + dx, ry-dy); 
    //画分针
    GetDxDy(dx, dy, dx1, dy1, dx2, dy2, (m_nMinute%15), m_nMinute/15, 1);
    dc.MoveTo(rx + dx, ry-dy); 
    dc.LineTo(rx - dx1, ry-dy1);
    dc.LineTo(rx + dx2, ry - dy2); 
    dc.LineTo(rx + dx1, ry+dy1); 
    dc.LineTo(rx + dx, ry-dy); 
    //画秒针
	if(m_nClockStyle == 0) 
	{
		GetDxDy(dx, dy, dx1, dy1, dx2, dy2, m_nSecond%15, m_nSecond/15, 2);
		dc.MoveTo(rx + dx, ry - dy);
		dc.LineTo(rx+dx2, ry-dy2); 
	}*/
}
////////////////////////////////////////////////////////////////////////////////
//
void CClock::StepMinute(BOOL bAdd)
{
	if(bAdd)
	{
		if(++m_nMinute >= 60)
		{
			m_nMinute = 0;
			m_nHour = (m_nHour+1)%24;
		}
	}
	else
	{
		if(--m_nMinute<0)
		{
			m_nMinute = 59;
			m_nHour = (m_nHour-1)<0? 23:m_nHour-1;
		}
	}
	OnPaint(CDC());
}
////////////////////////////////////////////////////////////////////////////////
//
void CClock::StepHour(BOOL bAdd)
{
	if(bAdd)
		m_nHour = (m_nHour+1)%24;
	else
        m_nHour = (m_nHour-1)<0? 23:m_nHour-1;

	OnPaint(CDC());
}
////////////////////////////////////////////////////////////////////////////////
//
void CClock::StepSecond(BOOL bAdd)
{
	if(bAdd)
	{
		if(++m_nSecond >= 60)
		{
			m_nSecond =0;
			m_nMinute ++;
			if(m_nMinute >=60)
			{
				m_nMinute =0;
				m_nHour = (m_nHour+1)%24;
			}
		}
	}
	else
	{
		if(--m_nSecond < 0)
		{
			m_nSecond = 59;
			m_nMinute--;
			if(m_nMinute < 0)
			{
				m_nMinute = 59;
				m_nHour = (m_nHour-1)<0 ? 23 : m_nHour-1;
			}
		}
	}
	OnPaint(CDC());
}

#undef CLOCK_WIDTH
#undef CLOCK_HEIGHT
////////////////////////////////////////////////////////////////////////////////
//end clock.cpp