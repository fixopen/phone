// CEDateTimeCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "../MultimediaPhone.h"
#include "CEDateTimeCtrl.h"
#include "../Logical/LunarderDate.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCEDateTimeCtrl

CCEDateTimeCtrl::CCEDateTimeCtrl()
{
	SYSTEMTIME tm;
	GetLocalTime(&tm);
	m_time = tm;
	m_time1 = CTime(1900, 1, 1, 0, 0, 0);
	m_time2 = CTime(2049, 12, 1, 0, 0, 0);
	m_style = DATETIME_DATESTLE;
	m_nClick = 0;
}

CCEDateTimeCtrl::~CCEDateTimeCtrl()
{
}


BEGIN_MESSAGE_MAP(CCEDateTimeCtrl, CStatic)
	//{{AFX_MSG_MAP(CCEDateTimeCtrl)
	ON_WM_PAINT()
	ON_BN_CLICKED(0x1002, OnButton2)
	ON_BN_CLICKED(0x1001, OnButton1)
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CCEDateTimeCtrl::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	CStatic::Create(L"", WS_CHILD | WS_VISIBLE|WS_EX_CLIENTEDGE | WS_EX_STATICEDGE, rect, pParentWnd);

	// Create the font
	VERIFY(m_font.CreateFont(
		26,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_NORMAL,                 // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		_T("ËÎÌו")));                 // lpszFacename
    SetFont(&m_font);

	CRect rt;
	GetClientRect(&rt);

	if(dwStyle & DATETIME_DATESTLE)
	{
		m_style = DATETIME_DATESTLE;
		m_rect1 = CRect(rt.left+2, rt.top+1, rt.left+52, rt.bottom-1);
		m_rect4 = CRect(m_rect1.right+2, rt.top+1, m_rect1.right+15, rt.bottom-1);
		m_rect2 = CRect(m_rect4.right+2, rt.top+1, m_rect4.right+28, rt.bottom-1);
		m_rect5 = CRect(m_rect2.right+2, rt.top+1, m_rect2.right+15, rt.bottom-1);
		m_rect3 = CRect(m_rect5.right+2, rt.top+1, m_rect5.right+28, rt.bottom-1);
	}
	else // if(dwStyle & DATETIME_TIMESTLE)
	{
		m_style = DATETIME_TIMESTLE;
		m_rect1 = CRect(rt.left+2, rt.top+1, rt.left+26, rt.bottom-1);
		m_rect4 = CRect(m_rect1.right+2, rt.top+1, m_rect1.right+15, rt.bottom-1);
		m_rect2 = CRect(m_rect4.right+2, rt.top+1, m_rect4.right+28, rt.bottom-1);
		m_rect5 = CRect(m_rect2.right+2, rt.top+1, m_rect2.right+15, rt.bottom-1);
		m_rect3 = CRect(m_rect5.right+2, rt.top+1, m_rect5.right+28, rt.bottom-1);
	}

	CRect rt1, rt2;
	rt1 = CRect(rt.right - 28, rt.top+1, rt.right -1, rt.bottom -1);
	rt2 = CRect(rt.right - 53, rt.top+1, rt.right - 29, rt.bottom -1);

//	m_button1.Create(L"", dwStyle, rt2, this, 0x1001);
//	m_button2.Create(L"", dwStyle, rt1, this, 0x1002);
	return TRUE;
}

BOOL CCEDateTimeCtrl::SetRange(const CTime* pMinRange, const CTime* pMaxRange)
{
	m_time1 = CTime(pMinRange->GetYear(), pMinRange->GetMonth(), pMinRange->GetDay(), pMinRange->GetHour(), pMinRange->GetMinute(), pMinRange->GetSecond()) ;;
	m_time2 = CTime(pMaxRange->GetYear(), pMaxRange->GetMonth(), pMaxRange->GetDay(), pMaxRange->GetHour(), pMaxRange->GetMinute(), pMaxRange->GetSecond()) ;;
	return TRUE;
}

// Operations
BOOL CCEDateTimeCtrl::SetTime(const CTime* pTimeNew)
{
	m_time = CTime(pTimeNew->GetYear(), pTimeNew->GetMonth(), pTimeNew->GetDay(), pTimeNew->GetHour(), pTimeNew->GetMinute(), pTimeNew->GetSecond()) ;
	return TRUE;
}

DWORD CCEDateTimeCtrl::GetTime(CTime& pTimeDest) const
{
	pTimeDest = m_time;
	return 0;
}
/////////////////////////////////////////////////////////////////////////////
// CCEDateTimeCtrl message handlers

void CCEDateTimeCtrl::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	CString s;
	dc.SelectObject(&m_font);
	if(m_style & DATETIME_DATESTLE)
	{
		s.Format(_T("%04d"), m_time.GetYear());
		dc.DrawText(s, &m_rect1, DT_CENTER|DT_VCENTER);
		s = "-";
		dc.DrawText(s, &m_rect4, DT_CENTER|DT_VCENTER);
		
		s.Format(_T("%02d"), m_time.GetMonth());
		dc.DrawText(s, &m_rect2, DT_CENTER|DT_VCENTER);
		s = "-";
		dc.DrawText(s, &m_rect5, DT_CENTER|DT_VCENTER);
		
		s.Format(_T("%02d"), m_time.GetDay());
		dc.DrawText(s, &m_rect3, DT_CENTER|DT_VCENTER);
	}
			
	else
	{
		s.Format(_T("%02d"), m_time.GetHour());
		dc.DrawText(s, &m_rect1, DT_CENTER|DT_VCENTER);
		s = ":";
		dc.DrawText(s, &m_rect4, DT_CENTER|DT_VCENTER);
		
		s.Format(_T("%02d"), m_time.GetMinute());
		dc.DrawText(s, &m_rect2, DT_CENTER|DT_VCENTER);
		s = ":";
		dc.DrawText(s, &m_rect5, DT_CENTER|DT_VCENTER);
		
		s.Format(_T("%02d"), m_time.GetSecond());
		dc.DrawText(s, &m_rect3, DT_CENTER|DT_VCENTER);
	}
	
	if(m_nClick == 1)
		dc.InvertRect(&m_rect1);
	if(m_nClick == 2)
		dc.InvertRect(&m_rect2);
	if(m_nClick == 3)
		dc.InvertRect(&m_rect3);
	// TODO: Add your message handler code here
	
	// Do not call CStatic::OnPaint() for painting messages
}

void CCEDateTimeCtrl::OnButton1() 
{
	if(m_style & DATETIME_DATESTLE)
	{
		if(m_nClick == 1)
		{
			int y = m_time.GetYear();
			y--;
			if(y >= m_time1.GetYear())
				m_time = CTime(y, m_time.GetMonth(), m_time.GetDay(), m_time.GetHour(), m_time.GetMinute(), m_time.GetSecond());
		}
		else if(m_nClick == 2)
		{
			int y = m_time.GetMonth();
			y--;
			if(y < 0)
				y = 12;
			m_time = CTime(m_time.GetYear(), y, m_time.GetDay(), m_time.GetHour(), m_time.GetMinute(), m_time.GetSecond());
		}
		else if(m_nClick == 3)
		{
			int y = m_time.GetDay();
			y--;
			if(y < 0)
				y = Logical::LunarderDate::MonthDays(m_time.GetYear(), m_time.GetMonth());
			m_time = CTime(m_time.GetYear(), m_time.GetMonth(), y, m_time.GetHour(), m_time.GetMinute(), m_time.GetSecond());
		}
	}
	else
	{
		if(m_nClick == 1)
		{
			int y = m_time.GetHour();
			y--;
			if(y < 0)
				y = 23;
			m_time = CTime(m_time.GetYear(), m_time.GetMonth(), m_time.GetDay(), y, m_time.GetMinute(), m_time.GetSecond());
		}
		else if(m_nClick == 2)
		{
			int y = m_time.GetMinute();
			y--;
			if(y < 0)
				y = 59;
			m_time = CTime(m_time.GetYear(), m_time.GetMonth(), m_time.GetDay(), m_time.GetHour(), y, m_time.GetSecond());
		}
		else if(m_nClick == 3)
		{
			int y = m_time.GetSecond();
			y--;
			if(y < 0)
				y = 59;
			m_time = CTime(m_time.GetYear(), m_time.GetMonth(), m_time.GetDay(), m_time.GetHour(), m_time.GetMinute(), y);
		}
	}
}

void CCEDateTimeCtrl::OnButton2() 
{
	if(m_style & DATETIME_DATESTLE)
	{
		if(m_nClick == 1)
		{
			int y = m_time.GetYear();
			y++;
			if(y <= m_time1.GetYear())
				m_time = CTime(y, m_time.GetMonth(), m_time.GetDay(), m_time.GetHour(), m_time.GetMinute(), m_time.GetSecond());
		}
		else if(m_nClick == 2)
		{
			int y = m_time.GetMonth();
			y++;
			if(y > 12)
				y = 1;
			m_time = CTime(m_time.GetYear(), y, m_time.GetDay(), m_time.GetHour(), m_time.GetMinute(), m_time.GetSecond());
		}
		else if(m_nClick == 3)
		{
			int y = m_time.GetDay();
			y++;
			if(y > Logical::LunarderDate::MonthDays(m_time.GetYear(), m_time.GetMonth()))
				y = 1;
			m_time = CTime(m_time.GetYear(), m_time.GetMonth(), y, m_time.GetHour(), m_time.GetMinute(), m_time.GetSecond());
		}
		Invalidate();
	}
	else
	{
		if(m_nClick == 1)
		{
			int y = m_time.GetHour();
			y++;
			if(y > 23)
				y = 0;
			m_time = CTime(m_time.GetYear(), m_time.GetMonth(), m_time.GetDay(), y, m_time.GetMinute(), m_time.GetSecond());
		}
		else if(m_nClick == 2)
		{
			int y = m_time.GetMinute();
			y++;
			if(y > 59)
				y = 0;
			m_time = CTime(m_time.GetYear(), m_time.GetMonth(), m_time.GetDay(), m_time.GetHour(), y, m_time.GetSecond());
		}
		else if(m_nClick == 3)
		{
			int y = m_time.GetSecond();
			y++;
			if(y > 59)
				y = 0;
			m_time = CTime(m_time.GetYear(), m_time.GetMonth(), m_time.GetDay(), m_time.GetHour(), m_time.GetMinute(), y);
		}
		Invalidate();
	}
}

void CCEDateTimeCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	if(m_rect1.PtInRect(point))
	{
		m_nClick = 1;
		Invalidate();
	}
	else if(m_rect2.PtInRect(point))
	{
		m_nClick = 2;
		Invalidate();
	}
	else if(m_rect3.PtInRect(point))
	{
		m_nClick = 3;
		Invalidate();
	}
	CStatic::OnLButtonDown(nFlags, point);
}