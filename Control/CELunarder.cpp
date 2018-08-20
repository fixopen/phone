// CELunarder.cpp : implementation file
//

#include "stdafx.h"
#include "CELunarder.h"
#include "../Logical/LunarderDate.h"
#include "../Data/LanguageResource.h"
#include "../resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define IDC_BTN_YL			0xFF00
#define IDC_BTN_YR			0xFF01
#define IDC_BTN_ML			0xFF02
#define IDC_BTN_MR			0xFF03

#define IDC_BTN_D0			0xFF04

/////////////////////////////////////////////////////////////////////////////
// CCELunarder

//227*200

CCELunarder::CCELunarder()
{
	m_nY = 2007;
	m_nM = 10;
	m_nD = 1;
}

CCELunarder::~CCELunarder()
{
}

void CCELunarder::SetData(UINT16 year, UINT8 month, UINT8 day, BOOL reDraw)
{
	m_nY = year;
	m_nM = month;
	m_nD = day;
	if(reDraw)
	{
		SetStcDay();
		OnPaint();
	}
}

void CCELunarder::GetData(UINT16 &year, UINT8 &month, UINT8 &day)
{
	year = m_nY;
	month = m_nM;
	day = m_nD;
}


BOOL CCELunarder::Create(int style, CRect &rect, CWnd *pParentWnd, int nCtrlID)
{
	CStatic::Create(L"", style, rect, pParentWnd);

	CRect rt;
	rt = CRect(3, 0, 19, 15);
	m_BtnYleft.Create(L"", WS_CHILD|WS_VISIBLE, rt, this, IDC_BTN_YL);
	m_BtnYleft.SetIcon(IDI_ICON_COMBOBOX, CSize(16, 16));
	rt = CRect(20, 0, 125, 15);
	m_StcYear.Create(L"", WS_CHILD|WS_VISIBLE, rt, this);
	rt = CRect(126, 0, 142, 15);
	m_BtnYRight.Create(L"", WS_CHILD|WS_VISIBLE, rt, this, IDC_BTN_YR);
	m_BtnYRight.SetIcon(IDI_ICON_COMBOBOX, CSize(16, 16));

	//rt = CRect(28, 16, 44, 31);
	rt = CRect(150, 0, 166, 15);
	m_BtnMlelt.Create(L"", WS_CHILD|WS_VISIBLE, rt, this, IDC_BTN_ML);
	m_BtnMlelt.SetIcon(IDI_ICON_COMBOBOX, CSize(16, 16));
	//rt = CRect(45, 16, 75, 31);
	rt = CRect(167, 0, 197, 15);
	m_StcMonth.Create(L"", WS_CHILD|WS_VISIBLE, rt, this);
	//rt = CRect(76, 16, 92, 31);
	rt = CRect(198, 0, 214, 15);
	m_BtnMright.Create(L"", WS_CHILD|WS_VISIBLE, rt, this, IDC_BTN_MR);
	m_BtnMright.SetIcon(IDI_ICON_COMBOBOX, CSize(16, 16));

	rt = CRect(3, 16, 227, 31);
	CString s = Data::LanguageResource::Get(Data::RI_LANUDER_WEEK).c_str();
	m_StcWeek.Create(s, WS_CHILD|WS_VISIBLE, rt, this);
	m_StcWeek.SetLeftMargin(0);
	m_StcWeek.SetColor(RGB(0, 0, 0,), RGB(204, 184, 218));
	m_StcYear.SetColor(RGB(0, 0, 0,), RGB(204, 184, 218));
	m_StcMonth.SetColor(RGB(0, 0, 0,), RGB(204, 184, 218));

	for(int i = 0; i < 6; i++)
		for(int j = 0; j < 7; j++)
		{
			rt = CRect(3+j*32, 32+i*28, 3+(j+1)*32-1, 32+(i+1)*28-1);
			m_StcDay[i*7+j].Create(L"", WS_CHILD|WS_VISIBLE, rt, this, IDC_BTN_D0+i*7+j);
			m_StcDay[i*7+j].SetLeftMargin(0);
			m_StcDay[i*7+j].SetColor(RGB(0, 0, 0,), RGB(253, 216, 216));
		}
	SetStcDay();
	return TRUE;
}

const char *month[] = {
Data::LanguageResource::Get(Data::RI_LANUDER_MONTH1).c_str(),\
Data::LanguageResource::Get(Data::RI_LANUDER_MONTH2).c_str(),\
Data::LanguageResource::Get(Data::RI_LANUDER_MONTH3).c_str(),\
Data::LanguageResource::Get(Data::RI_LANUDER_MONTH4).c_str(),\
Data::LanguageResource::Get(Data::RI_LANUDER_MONTH5).c_str(),\
Data::LanguageResource::Get(Data::RI_LANUDER_MONTH6).c_str(),\
Data::LanguageResource::Get(Data::RI_LANUDER_MONTH7).c_str(),\
Data::LanguageResource::Get(Data::RI_LANUDER_MONTH8).c_str(),\
Data::LanguageResource::Get(Data::RI_LANUDER_MONTH9).c_str(),\
Data::LanguageResource::Get(Data::RI_LANUDER_MONTH10).c_str(),\
Data::LanguageResource::Get(Data::RI_LANUDER_MONTH11).c_str(),\
Data::LanguageResource::Get(Data::RI_LANUDER_MONTH12).c_str()};

const char *day[] = {
Data::LanguageResource::Get(Data::RI_LANUDER_DAY10).c_str(),\
Data::LanguageResource::Get(Data::RI_LANUDER_DAY11).c_str(),\
Data::LanguageResource::Get(Data::RI_LANUDER_DAY12).c_str(),\
Data::LanguageResource::Get(Data::RI_LANUDER_DAY13).c_str()
};

const char *day1[] = {
Data::LanguageResource::Get(Data::RI_LANUDER_DAY0).c_str(),\
Data::LanguageResource::Get(Data::RI_LANUDER_DAY1).c_str(),\
Data::LanguageResource::Get(Data::RI_LANUDER_DAY2).c_str(),\
Data::LanguageResource::Get(Data::RI_LANUDER_DAY3).c_str(),\
Data::LanguageResource::Get(Data::RI_LANUDER_DAY4).c_str(),\
Data::LanguageResource::Get(Data::RI_LANUDER_DAY5).c_str(),\
Data::LanguageResource::Get(Data::RI_LANUDER_DAY6).c_str(),\
Data::LanguageResource::Get(Data::RI_LANUDER_DAY7).c_str(),\
Data::LanguageResource::Get(Data::RI_LANUDER_DAY8).c_str(),\
Data::LanguageResource::Get(Data::RI_LANUDER_DAY9).c_str()
};

void CCELunarder::SetStcDay()
{
	int nDay = Logical::LunarderDate::MonthDays(m_nY, m_nM);
	int nWeek = Logical::LunarderDate::WeekDay(m_nY, m_nM, 1);

	for(int i = 0; i < 42; i++)
	{
		CString s1 = "";
		m_StcDay[i].SetDateTxt(s1, s1);
		m_StcDay[i].SetWindowText(L"");
		m_StcDay[i].SetClicked(FALSE);
		m_StcDay[i].SetTogle(FALSE);
	}

	for(i = 0; i < nDay; i++)
	{
		char txt[3];
		sprintf(txt, "%d", i+1);
		CString s = txt;
		CString s1 = "";
		//m_StcDay[nWeek+i].SetWindowText(s);
		UINT16 y, m, d, ret;
		ret = Logical::LunarderDate::GetLunarDate(m_nY, m_nM, i+1, y, m, d);
		if(ret)
		{
			char txt[6];
			memset(txt, 0, 6);
			Logical::LunarderDate::FormatHolDay(ret, txt);
			s1 = txt;
		}
		else
		{
			if(d == 1)
			{
				//s1 = month[m-1];
				UINT16 iYear, iMonth, iDay;
				int n = Logical::LunarderDate::CalcLeapLunarYear(y, iYear, iMonth, iDay);
				if(n > 0 )
				{
					 if(iYear == m_nY && iMonth == m_nM && iDay == (i+1))
					 {
						 s1 = Data::LanguageResource::Get(Data::RI_LANUDER_LOOP).c_str();
						 s1 += month[m-1];
					 }
					 else
					 {
						s1 = month[m-1];
					 }
				}
				else
					s1 = month[m-1];
			}
			else
			{
				if(d <= 10)
				{
					s1 = day[0];
				}
				else if(d < 20)
				{
					s1 = day[1];
				}
				else if(d < 30)
				{
					s1 = day[2];
				}
				else
				{
					s1 = day[3];
				} 
				s1 += day1[d%10];
			}
		}
		m_StcDay[nWeek+i].SetDateTxt(s, s1);
		if((i+1) == m_nD)
		{
			m_StcDay[nWeek+i].SetTogle(TRUE);
			char txt1[24], txt2[24];
			Logical::LunarderDate::FormatLunarYear(y, txt1);	
			sprintf(txt2, "%d %s %s", m_nY, txt1, Data::LanguageResource::Get(Data::RI_COMN_YEARSTC).c_str());
			CString s = txt2;
			m_StcYear.SetWindowText(s);
			sprintf(txt, "%d%s", m_nM, Data::LanguageResource::Get(Data::RI_COMN_MONTHSTC).c_str());
			s = txt;
			m_StcMonth.SetWindowText(s);	
		}
		m_StcDay[nWeek+i].SetClicked(TRUE);
	}
}

BEGIN_MESSAGE_MAP(CCELunarder, CStatic)
	//{{AFX_MSG_MAP(CCELunarder)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_MESSAGE(WM_STATIC_CLICK, OnDaySelect)
	ON_BN_CLICKED(IDC_BTN_YL, OnYleft)
	ON_BN_CLICKED(IDC_BTN_YR, OnYright)
	ON_BN_CLICKED(IDC_BTN_ML, OnMleft)
	ON_BN_CLICKED(IDC_BTN_MR, OnMright)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCELunarder message handlers

void CCELunarder::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	
	// Do not call CStatic::OnPaint() for painting messages
}

BOOL CCELunarder::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	CRect rt;
	GetClientRect(&rt);
	pDC->FillSolidRect(&rt, RGB(204, 184, 218));
	return CStatic::OnEraseBkgnd(pDC);
}

void CCELunarder::OnDaySelect(WPARAM w, LPARAM l)
{
	CString s;
	m_StcDay[w-IDC_BTN_D0].GetDateTxt(s);
	char txt[3];
	int n = wcstombs( txt, s.GetBuffer(3), 3);
	txt[n] = '\0';
	
	
	int nWeek = Logical::LunarderDate::WeekDay(m_nY, m_nM, 1);
	n = nWeek+m_nD-1;
	m_StcDay[n].SetTogle(FALSE, TRUE);
	m_nD = atoi(txt);
	n = nWeek+m_nD-1;
	m_StcDay[n].SetTogle(TRUE, TRUE);

	UINT16 y, m, d;
	int ret = Logical::LunarderDate::GetLunarDate(m_nY, m_nM, m_nD, y, m, d);
	char txt1[24], txt2[24];
	Logical::LunarderDate::FormatLunarYear(y, txt1);	
	sprintf(txt2, "%d %s %s", m_nY, txt1, Data::LanguageResource::Get(Data::RI_COMN_YEARSTC).c_str());
	s = txt2;
	m_StcYear.SetWindowText(s);
	//SetStcDay();
}

void CCELunarder::OnYleft()
{
	m_nY--;
	if(m_nY < START_YEAR)
	{
		m_nY = END_YEAR;
	}
	SetStcDay();
}
void CCELunarder::OnYright()
{
	m_nY++;
	if(m_nY > END_YEAR)
	{
		m_nY = START_YEAR;
	}
	SetStcDay();
}
void CCELunarder::OnMleft()
{
	m_nM--;
	if(m_nM < 1)
	{
		m_nM = 12;
	}
	SetStcDay();
}

void CCELunarder::OnMright()
{
	m_nM++;
	if(m_nM > 12)
	{
		m_nM = 1;
	}
	SetStcDay();
}
