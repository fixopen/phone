// DateCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "DateCtrl.h"
#include "..\\resource.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDateCtrl
const int RANGE_YEAR_MIN = 1970;

const int CDateCtrl::BUTTON_WIDTH = 30;
const int CDateCtrl::ID_BUTTON_UP = 21;
const int CDateCtrl::ID_BUTTON_DOWN = 22;

CDateCtrl::CDateCtrl()
:m_strBuffer("")
{
}

CDateCtrl::~CDateCtrl()
{
}


BEGIN_MESSAGE_MAP(CDateCtrl, CEdit)
	//{{AFX_MSG_MAP(CDateCtrl)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_CHAR()
	ON_WM_SIZE()
	ON_BN_CLICKED(ID_BUTTON_UP, OnButtonUp)
	ON_BN_CLICKED(ID_BUTTON_DOWN, OnButtonDown)
	ON_MESSAGE(UM_BTN_UP, OnSubBtnUp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDateCtrl message handlers

BOOL CDateCtrl::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, UINT uFontHeight, em_mode mode) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	dwStyle |= /*ES_READONLY|*/ES_NOHIDESEL|ES_CENTER|ES_LEFT|ES_NUMBER;
	BOOL bRes = CEdit::Create(dwStyle, rect, pParentWnd, nID);
	if (bRes)
	{
		CRect rect;
		GetClientRect(&rect);
		
		rect.DeflateRect(1, 1);

		CRect rcButton(rect.right-BUTTON_WIDTH, rect.top, rect.right, rect.bottom);
		if (!m_btnDown.Create(_T(""), IDB_BITMAP_RIGHTARROW, IDB_BITMAP_RIGHTARROWD, WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON, rcButton, this, ID_BUTTON_DOWN)) return FALSE;
		rcButton.OffsetRect((0-BUTTON_WIDTH), 0);

		if (!m_btnUp.Create(_T(""), IDB_BITMAP_LEFTARROW, IDB_BITMAP_LEFTARROWD, WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON, rcButton, this, ID_BUTTON_UP)) return FALSE;
		
		m_font.CreateFont(
			uFontHeight,				// nHeight
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
			_T("宋体")); 
		SetFont(&m_font);

		m_mode = mode;

// 		SYSTEMTIME sm;
// 		GetSystemTime(&sm/*&m_sysTime*/);
// 		m_nYear = sm.wYear;
// 		m_nMonth = sm.wMonth;
// 		m_nDay = sm.wDay;
// 		m_nHour = sm.wHour;
// 		m_nMinute = sm.wMinute;
// 		m_nSecond = sm.wSecond;
		
		CTime sm(CTime::GetCurrentTime());
		m_nYear = sm.GetYear();
		m_nMonth = sm.GetMonth();
		m_nDay = sm.GetDay();
		m_nHour = sm.GetHour();
		m_nMinute = sm.GetMinute();
		m_nSecond = sm.GetSecond();
		
		CString strTemp;
	
		if (modeDate == m_mode)
		{
			FormatDateOrTime(strTemp, m_nYear, m_nMonth, m_nDay, m_mode);
			SetWindowText(strTemp);
			SetSel(0, 4);
			m_curSel = esel_year;
		}
		else
		{
			FormatDateOrTime(strTemp, m_nHour, m_nMinute, m_nSecond, modeTime);
			SetWindowText(strTemp);
			SetSel(0, 2);
			m_curSel = esel_hour;
		}		
	}

	return bRes;
}

BOOL CDateCtrl::OnEraseBkgnd(CDC* pDC )
{
// 	CRect rect;
// 	GetClientRect(&rect);
// 	pDC->FillSolidRect(&rect, RGB(255, 255, 255));
	return CEdit::OnEraseBkgnd(pDC);
}

LRESULT CDateCtrl::OnSubBtnUp(WPARAM wParam, LPARAM lParam)
{
    LRESULT result = 0;
	if(IsWindowVisible())
	{
		if(wParam == ID_BUTTON_DOWN)
			OnButtonDown();
		else if(wParam == ID_BUTTON_UP)
			OnButtonUp();
	}
    return result;
}

void CDateCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	HDC pDC = ::GetDC(m_hWnd);

	BOOL bFlag = FALSE;
	HFONT pOldFont;
	if (m_font.GetSafeHandle())
	{
		pOldFont = (HFONT)::SelectObject(pDC, m_font.m_hObject);
		bFlag = TRUE;
	}

	SIZE szY, szM, szF;
	GetTextExtentPoint(pDC, L"XXXX", 4, &szY);
	GetTextExtentPoint(pDC, L"XX", 2, &szM);
	GetTextExtentPoint(pDC, L"-", 1, &szF);


//  	CSize szY = pDC->GetTextExtent("XXXX");
// 	CSize szM = pDC->GetTextExtent("XX");
// 	CSize szF = pDC->GetTextExtent("-");

//	TEXTMETRIC tm;
//	pDC->GetTextMetrics(&tm);
	
	switch(m_mode)
	{
	case modeDate:
		{
			if (point.x < szY.cx +szF.cx/2)
			{
				SetSel(0, 4);
				m_curSel = esel_year;
			}
			else if(point.x < szM.cx+szY.cx+szF.cx+szF.cx/2)
			{
				SetSel(5, 7);
				m_curSel = esel_month;
			}
			else
			{
				SetSel(8, -1);
				m_curSel = esel_day;
			}
		}
		break;
	case modeTime:
		{
			if (point.x < szM.cx+szF.cx/2)
			{
				SetSel(0, 2);
				m_curSel = esel_hour;
			}
			else if(point.x < szM.cx*2+szF.cx*3/2)
			{
				SetSel(3, 5);
				m_curSel = esel_minute;
			}
			else
			{
				SetSel(6, -1);
				m_curSel = esel_second;
			}
		}
	    break;
	default:
	    break;
	}
	
	
	if (bFlag)
	{
		::SelectObject(pDC, pOldFont);
	}

	::ReleaseDC(m_hWnd, pDC);
	SetFocus();

//	CEdit::OnLButtonDown(nFlags, point);
}

void CDateCtrl::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	return;
}

void CDateCtrl::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	
	CEdit::OnChar(nChar, nRepCnt, nFlags);
}

void CDateCtrl::OnButtonUp()
{
	CString strTemp;
	TCHAR tBuf[16] = {0};
	
	switch(m_curSel)
	{
	case esel_year:
		{
			if (--m_nYear < RANGE_YEAR_MIN)
			{
				m_nYear = RANGE_YEAR_MIN;
				return;
			}

			int nNew = MonthDays(m_nYear, m_nMonth);
			
			if (m_nDay > nNew)
			{
				m_nDay = nNew;
				FormatDateOrTime(strTemp, m_nYear, m_nMonth, m_nDay);
				SetWindowText(strTemp);
			}
			else
			{
				FormatSingleVal(strTemp, m_nYear, m_curSel);
				ReplaceSel(strTemp);
			}
			
			SetSel(0, 4);
		}
		break;
	case esel_month:
		{			
			if (--m_nMonth < 1) m_nMonth = 12;

			int nNew = MonthDays(m_nYear, m_nMonth);

			if (m_nDay > nNew)
			{
				m_nDay = nNew;
				FormatDateOrTime(strTemp, m_nYear, m_nMonth, m_nDay);
				SetWindowText(strTemp);
			}
			else
			{
				FormatSingleVal(strTemp, m_nMonth, m_curSel);
				ReplaceSel(strTemp);
			}

			SetSel(5, 7);
		}
		break;
	case esel_day:
		{
			int nDays = MonthDays(m_nYear, m_nMonth);
			if (--m_nDay < 1) m_nDay = nDays;
			FormatSingleVal(strTemp, m_nDay, m_curSel);

			ReplaceSel(strTemp);
			SetSel(8, -1);
		}
		break;
	case esel_hour:
		{
			if (--m_nHour < 0) m_nHour = 23;
			
			FormatSingleVal(strTemp, m_nHour, m_curSel);
			ReplaceSel(strTemp);
			SetSel(0, 2);
		}
		break;
	case esel_minute:
		{
			if (--m_nMinute < 0) m_nMinute = 59;
			FormatSingleVal(strTemp, m_nMinute, m_curSel);
			ReplaceSel(strTemp);
			SetSel(3, 5);
		}
		break;
	case esel_second:
		{
			if (--m_nSecond < 0) m_nSecond = 59;
			FormatSingleVal(strTemp, m_nSecond, m_curSel);
			ReplaceSel(strTemp);
			SetSel(6, -1);
		}
		break;
	default:
		break;
	}
}

void CDateCtrl::OnButtonDown()
{
	CString strTemp;
	TCHAR tBuf[16] = {0};

	
	switch(m_curSel)
	{
	case esel_year:
		{
			++m_nYear;
			int nNew = MonthDays(m_nYear, m_nMonth);
			
			if (m_nDay > nNew)
			{
				m_nDay = nNew;
				FormatDateOrTime(strTemp, m_nYear, m_nMonth, m_nDay);
				SetWindowText(strTemp);
			}
			else
			{
				FormatSingleVal(strTemp, m_nYear, m_curSel);
				ReplaceSel(strTemp);
			}

			SetSel(0, 4);
		}
		break;
	case esel_month:
		{			
			if (++m_nMonth > 12) m_nMonth = 1;
			
			int nNew = MonthDays(m_nYear, m_nMonth);
			
			if (m_nDay > nNew)
			{
				m_nDay = nNew;
				FormatDateOrTime(strTemp, m_nYear, m_nMonth, m_nDay);
				SetWindowText(strTemp);
			}
			else
			{
				FormatSingleVal(strTemp, m_nMonth, m_curSel);
				ReplaceSel(strTemp);
			}

			SetSel(5, 7);
		}
		break;
	case esel_day:
		{	
			int nDays = MonthDays(m_nYear, m_nMonth);
			if (++m_nDay > nDays) m_nDay = 1;
			FormatSingleVal(strTemp, m_nDay, m_curSel);

			ReplaceSel(strTemp);
			SetSel(8, -1);
		}
		break;
	case esel_hour:
		{
			if (++m_nHour >= 24) m_nHour = 0;
			FormatSingleVal(strTemp, m_nHour, m_curSel);
			ReplaceSel(strTemp);
			SetSel(0, 2);
		}
		break;
	case esel_minute:
		{
			if (++m_nMinute >= 60) m_nMinute = 0;
			FormatSingleVal(strTemp, m_nMinute, m_curSel);
			ReplaceSel(strTemp);
			SetSel(3, 5);
		}
		break;
	case esel_second:
		{
			if (++m_nSecond >= 60) m_nSecond = 0;
			FormatSingleVal(strTemp, m_nSecond, m_curSel);
			ReplaceSel(strTemp);
			SetSel(6, -1);
		}
		break;
	default:
		break;
	}
}

UINT16 CDateCtrl::MonthDays(UINT16 iYear, UINT16 iMonth)
{
	switch(iMonth)
	{
	case 1:case 3:case 5:case 7:case 8:case 10:case 12:
		return 31;
		break;
	case 4:case 6:case 9:case 11:
		return 30;
		break;
	case 2:     //如果是闰年
		if(IsLeapYear(iYear))
			return 29;
		else
			return 28;
		break;
	}
	return 0;
}

void CDateCtrl::FormatDateOrTime(CString &strData, const int nYear, const int nMonth, const int nDay, em_mode mode)
{
	TCHAR tBuf[16] = {0};
	TCHAR tchDiv = _T('-');

	if (modeDate == mode)
	{
		ASSERT(nYear >= RANGE_YEAR_MIN);
		ASSERT(nMonth >= 1 && nMonth <= 12);
		ASSERT(nDay >= 1 && nDay <= 31);
	}
	else
	{
		ASSERT(nYear < 24);
		ASSERT(nMonth  < 60);
		ASSERT(nDay < 60);
		tchDiv = _T(':');
	}
	
	
	
	_itot(nYear, tBuf, 10);
	if (nYear < 10)
	{
		strData = _T("0");
	}
	strData += tBuf;
	strData += tchDiv/*_T("-")*/;

	memset(tBuf, 0, sizeof(tBuf));
	_itot(nMonth, tBuf, 10);
	if (nMonth < 10)
	{
		strData += _T("0");
	}
	strData += tBuf;
	strData += tchDiv/*_T("-")*/;
	
	memset(tBuf, 0, sizeof(tBuf));
	_itot(nDay, tBuf, 10);
	if (nDay < 10)
	{
		strData += _T("0");
	}
	
	strData += tBuf;
}

void CDateCtrl::OnSize(UINT nType, int cx, int cy) 
{
	CEdit::OnSize(nType, cx, cy);
	
	CRect rect(0, 0, cx, cy);
	CRect rcButton;
	rect.DeflateRect(1, 1);
	if (m_btnDown.GetSafeHwnd())
	{
		rcButton.SetRect(rect.right-BUTTON_WIDTH, rect.top, rect.right, rect.bottom);
		m_btnDown.MoveWindow(rcButton);
	}

	if (m_btnUp.GetSafeHwnd())
	{
		rcButton.OffsetRect((0-BUTTON_WIDTH), 0);
		m_btnUp.MoveWindow(rcButton);
	}
	
}

BOOL CDateCtrl::FormatSingleVal(CString &strData, const int nVal, em_selection sel)
{
	BOOL bRes = TRUE;
	TCHAR tBuf[16] = {0};

	switch(sel)
	{
	case esel_year:
		{
			ASSERT(nVal >= RANGE_YEAR_MIN);

			_itot(nVal, tBuf, 10);
			strData = tBuf;
		}
		break;
	case esel_month:
	case esel_day:
	case esel_hour:
	case esel_minute:
	case esel_second:
		{
			_itot(nVal, tBuf, 10);
			if (nVal < 10)
			{
				strData = _T("0");
			}
			strData += tBuf;
		}
		break;
	default:
		bRes = FALSE;
	    break;
	}
	return bRes;
}

void CDateCtrl::GetTime(CTime &timeDest)
{
	ASSERT(m_hWnd != NULL);
	timeDest = CTime(m_nYear, m_nMonth, m_nDay, m_nHour, m_nMinute, m_nSecond);
}

void CDateCtrl::GetTime(SYSTEMTIME &sti)
{
	sti.wYear = m_nYear;
	sti.wMonth = m_nMonth;
	sti.wDay = m_nDay;
	sti.wHour = m_nHour;
	sti.wMinute = m_nMinute;
	sti.wSecond = m_nSecond;
	sti.wMilliseconds = 0;
	sti.wDayOfWeek = 0;
}

void CDateCtrl::SetTime(const int nYear, const int nMonth, const int nDay, const int nHour, const int nMinute, const int nSecond)
{
	ASSERT(nMonth > 0 && nMonth<=12);
	ASSERT(nDay > 0);
	ASSERT(nHour>=0 && nHour<24);
	ASSERT(nMinute>=0 && nMinute<60);
	ASSERT(nSecond>=0 && nSecond<60);

	CString strTemp;
	int nRealYear = nYear;
	int nRealDay = nDay;

	if (nRealYear < RANGE_YEAR_MIN) nRealYear = RANGE_YEAR_MIN;

	int nums = MonthDays(nYear, nMonth);
	if (nRealDay > nums) nRealDay = nums;

	m_nYear = nRealYear;
	m_nMonth = nMonth;
	m_nDay = nRealDay;
	m_nHour = nHour;
	m_nMinute = nMinute;
	m_nSecond = nSecond;

	switch(m_mode)
	{
	case modeDate:
		{
			FormatDateOrTime(strTemp, m_nYear, m_nMonth, m_nDay, m_mode);
			SetWindowText(strTemp);
			SetSel(0, 4);
			m_curSel = esel_year;
		}
		break;
	case modeTime:
		{
			FormatDateOrTime(strTemp, m_nHour, m_nMinute, m_nSecond, modeTime);
			SetWindowText(strTemp);
			SetSel(0, 2);
			m_curSel = esel_hour;
		}
	    break;
	default:
	    break;
	}
}

void CDateCtrl::SetTime(const SYSTEMTIME &ti)
{
	SetTime(ti.wYear, ti.wMonth, ti.wDay, ti.wHour, ti.wMinute, ti.wSecond);
}

void CDateCtrl::SetTime(CTime *time)
{
	SetTime(time->GetYear(), time->GetMonth(), time->GetDay(), time->GetHour(), time->GetMinute(), time->GetSecond());	
}