// LunarderDlg.cpp : implementation file
//

#include "stdafx.h"
#include "LunarderDlg.h"
#include "../Logical/LunarderDate.h"
#include "../Data/LanguageResource.h"
#include "../Data/SkinStyle.h"
#include "../MultimediaPhoneDlg.h"
#include "../MultimediaPhone.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLunarderDlg dialog
#define IDC_BTN_YL			0xFF00
#define IDC_BTN_YR			0xFF01
#define IDC_BTN_ML			0xFF02
#define IDC_BTN_MR			0xFF03
#define IDC_BTN_UP			0xFF04
#define IDC_BTN_DOWN		0xFF05

CLunarderDlg::CLunarderDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLunarderDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLunarderDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_type = 0;
	m_nAlarmCount = 0;
	m_maindeskIn = FALSE;
	m_curIndex = -1;
	m_TimeOutAlarmCount = 0;

	VERIFY(m_font.CreateFont(
		24,                        // nHeight
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
		_T("宋体")));                 // lpszFacename
}

void CLunarderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLunarderDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLunarderDlg, CDialog)
	//{{AFX_MSG_MAP(CLunarderDlg)
		ON_BN_CLICKED(IDC_BUTTON_LUNARDEREXIT, OnExit)
		ON_BN_CLICKED(IDC_BTN_YL, OnDateUp)
		ON_BN_CLICKED(IDC_BTN_YR, OnDateDown)
		ON_BN_CLICKED(IDC_BTN_ML, OnTimeUp)
		ON_BN_CLICKED(IDC_BTN_MR, OnTimeDown)
		ON_BN_CLICKED(IDC_LUNARDER_OK, OnOk)
		ON_BN_CLICKED(IDC_LUNARDER_CANCEL, OnCancel)
		ON_BN_CLICKED(IDC_LUNARDER_DELETE, OnDelete)
		ON_MESSAGE(WM_LUNAR_CLICKED, OnLunarClick)
		ON_MESSAGE(WM_CLICKMJPG_TOAPP, OnClickMJPG)
		ON_MESSAGE(WM_MJPGTOGGLE, OnClickMJPG)
		ON_MESSAGE(WM_DELETESELITEM, OnDeleteItem)
		ON_MESSAGE(WM_STOPTRYRING, OnStopTryRing)
		ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLunarderDlg message handlers
#define ADD_OFFSET 0
BOOL CLunarderDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	m_dtDateShow.Create(WS_CHILD|DTS_SHORTDATEFORMAT, CRect(43,37,262,90), this, IDC_DATE_SHOW, 30, CDateCtrl::em_mode::modeDate);
	m_dtTimeShow.Create(WS_CHILD|DTS_TIMEFORMAT, CRect(339,37,339+219,90), this, IDC_TIME_SHOW, 30, CDateCtrl::em_mode::modeTime);

	m_dtDate.Create(WS_CHILD|DTS_SHORTDATEFORMAT, CRect(42, 31, 197, 84), this, IDC_SETTING_DATE, 28, CDateCtrl::em_mode::modeDate);
	m_dtTime.Create(WS_CHILD|DTS_TIMEFORMAT, CRect(262, 31, 417, 84), this, IDC_SETTING_TIME, 28, CDateCtrl::em_mode::modeTime);
 
 	m_cmbRing.CreateEx(WS_CHILD, CRect(202, 124, 413, 302), this, IDC_COMBOBOX_CONTACTGROUP_SOUNDTIP, 22, 55, 26);
 	SetRingLst();
 
 	m_alarmEdit.Create(WS_CHILD|ES_MULTILINE | ES_AUTOVSCROLL | ES_WANTRETURN , CRect(203, 217, 554, 360), this, 0xFFFF);
 	m_alarmEdit.SetLimitText(768);

	m_editTaboo.Create(WS_CHILD|ES_MULTILINE | ES_AUTOVSCROLL | ES_WANTRETURN , CRect(42, 94, 617, 394), this, 0xFFFF);
	m_editTaboo.SetReadOnly(true);
	m_editTaboo.SetLimitText(768);

	m_MJPGList.Create(L"", WS_VISIBLE|WS_CHILD, CRect(0, 0, 800, 423), this,10086);
	m_MJPGList.SetCurrentLinkFile(".\\adv\\mjpg\\k5\\中文\\日程提醒.xml");
	m_MJPGList.SetMJPGRect(CRect(0, 0, 800, 423));

	MoveWindow(0,57,800,423);

	m_bIsMainLunder = TRUE;
	SYSTEMTIME curtime;
	GetLocalTime(&curtime);
	SetData(curtime.wYear, curtime.wMonth, curtime.wDay);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CLunarderDlg::SetRingLst()
{
	CString sDir = "\\flashdrv\\my_ring\\";
	WIN32_FIND_DATA FindFileData;			//查找文件时要使用的数据结构
	HANDLE hFind = INVALID_HANDLE_VALUE;	//定义查找句柄
	
	sDir += "*.*";
	hFind = FindFirstFile(sDir, &FindFileData);//使用FindFirstFile函数来开始文件查找
	
	if (hFind == INVALID_HANDLE_VALUE) 
	{
		return;
	} 
	else 
	{
		char filename[128];
		int i = wcstombs( filename, FindFileData.cFileName, 128);
		filename[i] = '\0';
		if(strstr(filename, ".mp3")||strstr(filename, ".MP3")|| strstr(filename, ".MID")||strstr(filename, ".mid") || strstr(filename, ".wav")||strstr(filename, ".WAV"))
		{
			m_cmbRing.AddString(FindFileData.cFileName);
		}
		
		//以下是循环使用FindNextFile函数来查找文件
		while (FindNextFile(hFind, &FindFileData) != 0) 
		{
			i = wcstombs( filename, FindFileData.cFileName, /*wcslen(FindFileData.cFileName)*/128);
			filename[i] = '\0';
			if(strstr(filename, ".mp3")||strstr(filename, ".MP3")|| strstr(filename, ".MID")||strstr(filename, ".mid") || strstr(filename, ".wav")||strstr(filename, ".WAV"))
			{
				m_cmbRing.AddString(FindFileData.cFileName);
			}
		}
		
		DWORD dwError = GetLastError();
		if (dwError == ERROR_NO_MORE_FILES) 
		{
			FindClose(hFind);//关闭查找句柄
		} 
		else 
		{
			return;
		}
	}
	return;
}

void CLunarderDlg::OnExit()
{
	ShowWindow(SW_HIDE);
	
	if(m_maindeskIn)
	{
		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->SwitchPanel_(IDC_BUTTON_MAIN);
		m_maindeskIn = FALSE;
	}
	else
	{
		GetParent()->SendMessage(WM_CHANGEWINDOW, (WPARAM)this, (LPARAM)SW_HIDE);
	}
}

void CLunarderDlg::OnDateUp()
{
	SYSTEMTIME datetime;
	m_dtDateShow.OnButtonUp();
	m_dtDateShow.GetTime(datetime);
	m_Year = datetime.wYear;
	m_Month = datetime.wMonth;
	m_Day = datetime.wDay;
	int nDay = Logical::LunarderDate::MonthDays(m_Year, m_Month);
	int nWeek = Logical::LunarderDate::WeekDay(m_Year, m_Month, 1);
	if (m_dtDateShow.m_curSel != 2)
	{
		SetData(m_Year, m_Month, m_Day);
	}
	else
	{
		if(IsHaveAlarm(m_Year, m_Month, m_Day+1))
		{
			m_MJPGList.SetUnitColor(m_curIndex, font_purple, TRUE);
		}
		else
		{
			m_MJPGList.SetUnitColor(m_curIndex, font_black, TRUE);
		}

		if (m_Day >= 1)
		{
			m_curIndex--;
		}
		else
		{
			m_curIndex = nWeek + 10 + nDay;
		}
		m_MJPGList.SetUnitColor(m_curIndex, font_green, TRUE);
	}
}
void CLunarderDlg::OnDateDown()
{
	SYSTEMTIME datetime;
	m_dtDateShow.OnButtonDown();
	m_dtDateShow.GetTime(datetime);
	m_Year = datetime.wYear;
	m_Month = datetime.wMonth;
	m_Day = datetime.wDay;
	int nDay = Logical::LunarderDate::MonthDays(m_Year, m_Month);
	int nWeek = Logical::LunarderDate::WeekDay(m_Year, m_Month, 1);
	if (m_dtDateShow.m_curSel != 2)  //2相当于esel_day
	{
		SetData(m_Year, m_Month, m_Day);
	}
	else
	{
		if(IsHaveAlarm(m_Year, m_Month, m_Day-1))
		{
			m_MJPGList.SetUnitColor(m_curIndex, font_purple, TRUE);
		}
		else
		{
			m_MJPGList.SetUnitColor(m_curIndex, font_black, TRUE);
		}

		if (m_Day <= nDay)
		{
			m_curIndex++;
		}
		else
		{
			m_curIndex = nWeek + 10;
		}
		
		m_MJPGList.SetUnitColor(m_curIndex, font_green, TRUE);
	}
}

void CLunarderDlg::OnTimeUp()
{
	m_dtTimeShow.OnButtonUp();
}

void CLunarderDlg::OnTimeDown()
{
	m_dtTimeShow.OnButtonDown();
}

void CLunarderDlg::OnLunarClick(WPARAM w, LPARAM l)
{
}

//保存
void CLunarderDlg::OnOk()
{
}

void CLunarderDlg::OnCancel()
{

}

//删除
void CLunarderDlg::OnDelete()
{
}

void CLunarderDlg::OnAlarmDateDown()
{
	SYSTEMTIME datetime;
	m_dtDate.OnButtonDown();
	m_dtDate.GetTime(datetime);
	if(IsHaveAlarm(datetime.wYear, datetime.wMonth, datetime.wDay))
	{
		ShowAlarmDlg(FALSE);
	}
	else
	{
		NewAlarm(FALSE);
	}
	GetSchedulerCount();
}

void CLunarderDlg::OnAlarmDateUp()
{
	SYSTEMTIME datetime;
	m_dtDate.OnButtonUp();
	m_dtDate.GetTime(datetime);
	if(IsHaveAlarm(datetime.wYear, datetime.wMonth, datetime.wDay))
	{
		ShowAlarmDlg(FALSE);
	}
	else
	{
		NewAlarm(FALSE);
	}
	GetSchedulerCount();
}

extern const char *month[];
extern const char *day[];
extern const char *day1[];

void CLunarderDlg::SetData(int y, int m, int d, BOOL flag)
{	
	if(	!m_bIsMainLunder)
	{
		return;
	}

	BOOL isDraw = flag;

	if(y < 1901)
		y = 2008;
	m_Year = y;
	m_Month = m;
	m_Day = d;

	int nDay = Logical::LunarderDate::MonthDays(m_Year, m_Month);
	int nWeek = Logical::LunarderDate::WeekDay(m_Year, m_Month, 1);

	if(m_Day > nDay)
		m_Day = nDay;
	
	for(int i = 0; i < 42; i++)
	{
		CString s1 = "";
		int nIndex = i+10;
		m_MJPGList.SetUnitColor(nIndex, font_black, FALSE);
		m_MJPGList.SetUnitFont(nIndex, font_18);
		if(i < nWeek)
		{
			m_MJPGList.SetUnitText(nIndex, s1, FALSE);
		}
		else if(i >= (nWeek+nDay))
		{
			m_MJPGList.SetUnitText(nIndex, s1, FALSE);
		}
		else
		{
			s1.Format(_T("%d \r\n"), (i+1-nWeek));
		
			////////////////start/////////////
			UINT16 y, m, d, ret;
			ret = Logical::LunarderDate::GetLunarDate(m_Year, m_Month, i+1-nWeek, y, m, d);
			if(ret)
			{
				m_MJPGList.SetUnitColor(nIndex, font_green, FALSE);
				char txt[6];
				memset(txt, 0, 6);
				Logical::LunarderDate::FormatHolDay(ret, txt);
				s1 += txt;
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
						if(iYear == m_Year && iMonth == m_Month && iDay == (i+1-nWeek))
						{
							s1 += Data::LanguageResource::Get(Data::RI_LANUDER_LOOP).c_str();
							s1 += month[m-1];
						}
						else
						{
							s1 += month[m-1];
						}
					}
					else
						s1 += month[m-1];
				}
				else
				{
					if(d <= 10)
					{
						s1 += day[0];
					}
					else if(d < 20)
					{
						s1 += day[1];
					}
					else if(d < 30)
					{
						s1 += day[2];
					}
					else
					{
						s1 += day[3];
					} 
					s1 += day1[d%10];
				}
			}
			////////////////end///////////////
			if(m_Day == (i+1-nWeek))
			{
				m_MJPGList.SetUnitColor(nIndex, font_green, FALSE);
				m_curIndex = nIndex;
			}

			m_MJPGList.SetUnitText(nIndex, s1, FALSE);

			//如果有提醒，将显示的文字改为红色
			if(IsHaveAlarm(m_Year, m_Month, i+1-nWeek))
				m_MJPGList.SetUnitColor(nIndex, font_purple, FALSE);
		}
		
		//判断天，toggle
	}
	m_MJPGList.Invalidate();
}

void CLunarderDlg::OnClickMJPG(WPARAM w, LPARAM l)
{
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)(theApp.m_pMainWnd) ;
	CString icon ;
	
	if(w != 209)
		StopTryRing();

	switch(w)
	{
	case 2:				//上翻日期	
		OnDateUp();
		break;
	case 3:				//下翻日期 
		OnDateDown();
		break;
	case 4:				//上翻时间
		OnTimeUp();
		break;
	case 5:				//下翻时间
		OnTimeDown();
		break;
	case 6:				//设置当前系统时间
		{
			SYSTEMTIME date;
			SYSTEMTIME time;
			SYSTEMTIME datetime;
			m_dtDateShow.GetTime(date);
			m_dtTimeShow.GetTime(time);
			memcpy(&datetime, &date, sizeof(SYSTEMTIME));
			datetime.wHour = time.wHour;
			datetime.wMinute = time.wMinute;
			datetime.wSecond = time.wSecond;
			SetLocalTime(&datetime);
			//重新计算闹铃
			((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->SetAlarmList();
		}
		break;
	case 7:				//记事
		if(IsHaveAlarm(m_Year, m_Month, m_Day))
		{
			ShowAlarmDlg(FALSE);
		}
		else
		{
			ShowAlarmDlg(TRUE);
		}
		GetSchedulerCount();  //求日程提醒总数
		break;
	case 8:			//宜忌
		ShowTaboo();
		break;
	case 201:
		OnAlarmDateUp();
		break;
	case 202:
		OnAlarmDateDown();
		break;
	case 203:
		m_dtTime.OnButtonUp();
		break;
	case 204:
		m_dtTime.OnButtonDown();
		break;
	case 205:			//该天上一条记事
		SetPreAlarm();
		break;
	case 206:			//该天下一条记事
		SetNextAlarm();
		break;
	case 207:			//清除该记事
		DeleteCurrentAlarm();
		break;
	case 208:			//新建
		NewAlarm(TRUE);
		break;
	case 209:   //试听铃声
		{
			{
				if(!m_MJPGList.GetUnitIsDownStatus(209))
				{
					CString str;
					m_cmbRing.GetWindowText(str);
					if(str == "")
						break;
					CString s = "\\flashdrv\\my_ring\\"; 
					s += str;
					
					main->m_pMainDlg->m_mainMp3Dlg_->OnTimer(1002); //SendMessage(WM_OUTEVENT, 0, 0);
					
					main->phone_->SetMsgWnd(this);
					main->phone_->StartRing((LPTSTR)(LPCTSTR)s, 1);
					m_MJPGList.SetUnitIsDownStatus(209, TRUE);
					m_MJPGList.SetUnitIsShow(209, TRUE);
				}
				else
				{
					StopTryRing();
				}
				break;
			}
		}
	case 210:		//提醒
		m_MJPGList.SetUnitIsDownStatus(210, TRUE);
		m_MJPGList.SetUnitIsDownStatus(211, FALSE);
		m_MJPGList.SetUnitIsShow(211, TRUE, TRUE);
		break;
	case 211:		//不提醒
		m_MJPGList.SetUnitIsDownStatus(210, FALSE);
		m_MJPGList.SetUnitIsDownStatus(211, TRUE);
		m_MJPGList.SetUnitIsShow(210, TRUE, TRUE);
		break;
	case 250:			//退出日历记事
		ShowLunarderDlg();
		break;
	case 251:			//确定
		{
			CString s;
			m_alarmEdit.GetWindowText(s);
			if(s != "")
			{
				SetAlarmDateBase();
				{
					ShowLunarderDlg();
				}
			}
			else
			{
				main->m_pWarningNoFlashDlg->SetTitle(L"请输入提示内容");     //提示输入内容
				main->m_pWarningNoFlashDlg->ShowWindow_(SW_SHOW);
			}
		}
		break;
	case 302:		//宜忌上翻页
		break;
	case 303:		//宜忌下翻页
		break;
	case 350:		//宜忌返回
		ShowLunarderDlg();
		break;
	case 351:		//宜忌确定
		ShowLunarderDlg();
		break;
	case 1000:
		main->PopbackIcon();
		OnExit();
		break;
	}
	if( 10 <= w && w <= 51) 
	{
		int nDay = Logical::LunarderDate::MonthDays(m_Year, m_Month);
		int nWeek = Logical::LunarderDate::WeekDay(m_Year, m_Month, 1);
		int i = w-10;
		if(i >= nWeek && i < (nWeek+nDay))
		{
			int nIndex = i+10;
			if(!IsHaveAlarm(m_Year, m_Month, m_Day))
			{
				m_MJPGList.SetUnitColor(m_curIndex, font_black, TRUE);
			}
			m_MJPGList.SetUnitColor(nIndex, font_green, TRUE);
			m_curIndex = nIndex;
			m_Day = i - nWeek + 1;
			SYSTEMTIME curtime;
			SYSTEMTIME time;
			m_dtTimeShow.GetTime(time);
			curtime.wYear = m_Year;
			curtime.wMonth = m_Month;
			curtime.wDay = m_Day;
			curtime.wHour = time.wHour;
			curtime.wMinute = time.wMinute;
			curtime.wSecond = time.wSecond;
			m_dtDateShow.SetTime(curtime);
			//m_dtDateShow.SetSel(8, -1);
			//m_dtDateShow.m_curSel = CDateCtrl::em_selection::esel_day;
			CPoint point(110, 15);
			m_dtDateShow.OnLButtonDown(TRUE, point);

			//显示提醒
			if(IsHaveAlarm(m_Year, m_Month, m_Day))
			{
				ShowAlarmDlg(FALSE);
				GetSchedulerCount();
			}
		}
	}
}

//显示日历
void CLunarderDlg::ShowLunarderDlg()
{
	m_bIsMainLunder = TRUE;
	m_type = 1;
	m_MJPGList.SetCurrentLinkFile(".\\adv\\mjpg\\k5\\中文\\日程提醒.xml");
	SetData(m_Year, m_Month, m_Day, TRUE);
	m_MJPGList.Invalidate();
	SetCtrlHide();
}
//显示alarm
void CLunarderDlg::ShowAlarmDlg(BOOL isNew)
{
	m_bIsMainLunder = FALSE;
	m_type = 2;
	m_MJPGList.SetCurrentLinkFile(".\\adv\\mjpg\\k5\\中文\\记事.xml");
	m_MJPGList.Invalidate();
	SetCtrlHide();
	SetAlarmCtrl(isNew);	
}

void CLunarderDlg::ShowTaboo()
{
	m_type = 3;
	m_MJPGList.SetCurrentLinkFile(".\\adv\\mjpg\\k5\\中文\\宜忌.xml");
	char yi_ji[512] = {0};
	Logical::LunarderDate::FindHuangDaoData(CTime(m_Year, m_Month, m_Day, 0, 0, 0), yi_ji);
	m_editTaboo.SetWindowText(Util::StringOp::ToCString(yi_ji));
	m_MJPGList.Invalidate();
	m_MJPGList.SetUnitIsShow(350, TRUE, FALSE);
	m_MJPGList.SetUnitIsShow(351, TRUE, FALSE);
	SetCtrlHide();
}

void CLunarderDlg::SetCtrlHide()
{
	m_dtTime.ShowWindow(FALSE);
	m_dtDate.ShowWindow(FALSE);
	m_alarmEdit.ShowWindow(FALSE);
	m_cmbRing.ShowWindow(FALSE);
	m_dtDateShow.ShowWindow(FALSE);
	m_dtTimeShow.ShowWindow(FALSE);
	m_editTaboo.ShowWindow(FALSE);
	SetTimer(1, 200, NULL);
}

void CLunarderDlg::ShowControl()
{
	switch(m_type)
	{
	case 1:
		m_dtDateShow.ShowWindow(TRUE);
		m_dtTimeShow.ShowWindow(TRUE);
		break;
	case 2:
		m_dtTime.ShowWindow(TRUE);
		m_dtDate.ShowWindow(TRUE);
		m_alarmEdit.ShowWindow(TRUE);
		m_cmbRing.ShowWindow(TRUE);
		break;
	case 3:
		m_editTaboo.ShowWindow(TRUE);
		break;
	}
}

void CLunarderDlg::ShowWindow_(int nCmdShow)
{
	m_bIsMainLunder = TRUE;
	m_type = 1;
	SYSTEMTIME curtime;
	GetLocalTime(&curtime);
	m_dtDateShow.SetTime(curtime);
	m_dtTimeShow.SetTime(curtime);
	SetData(curtime.wYear, curtime.wMonth, curtime.wDay);
	ShowWindow(nCmdShow);
	SetCtrlHide();
}

void CLunarderDlg::HideWindow()
{
	m_bIsMainLunder = TRUE;
	m_type = 1;
	m_MJPGList.SetCurrentLinkFile(".\\adv\\mjpg\\k5\\中文\\日程提醒.xml");

	m_dtTime.ShowWindow(FALSE);
	m_dtDate.ShowWindow(FALSE);
	m_alarmEdit.ShowWindow(FALSE);
	m_cmbRing.ShowWindow(FALSE);
	m_dtDateShow.ShowWindow(FALSE);
	m_dtTimeShow.ShowWindow(FALSE);
	m_editTaboo.ShowWindow(FALSE);

	ShowWindow(SW_HIDE);
}

void CLunarderDlg::OnDeleteItem(WPARAM w, LPARAM l)
{
	m_pScheduler->Remove();
	m_nAlarmCount--;
	if(IsHaveAlarm(m_Year, m_Month, m_Day))
	{
		SetAlarmCtrl(FALSE);
	}
	else
	{
		ShowLunarderDlg();
	}
	((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->SetAlarmList();
	GetSchedulerCount();
}

void CLunarderDlg::DeleteCurrentAlarm()
{
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	if(!m_bIsNewAlarm && m_pScheduler)
	{
		main->m_pDeleteTipDlg->SetTitle(L"确定删除该记事吗?");
		main->m_pDeleteTipDlg->SetHWnd(m_hWnd);
		main->m_pDeleteTipDlg->ShowWindow_(TRUE);
	}
}

void CLunarderDlg::SetPreAlarm()
{
	if(m_nAlarmCurrent > 0)
	{
		m_nAlarmCurrent--;
		m_pScheduler = m_result[m_nAlarmCurrent];
		SetAlarmCtrl(FALSE);
	}

}

void CLunarderDlg::SetNextAlarm()
{
	if(m_nAlarmCurrent < (m_nAlarmCount -1))
	{
		m_nAlarmCurrent++;
		m_pScheduler = m_result[m_nAlarmCurrent];
		SetAlarmCtrl(FALSE);
	}
}

void CLunarderDlg::SetCurrentAlarmBase()
{
	if(m_pScheduler)
	{
		BOOL isAlarm = m_MJPGList.GetUnitIsDownStatus(210);
		CTime tm1, tm2;		
		m_dtTime.GetTime(tm1);
		m_dtDate.GetTime(tm2);
		CTime time = CTime(tm2.GetYear(), tm2.GetMonth(), tm2.GetDay(), tm1.GetHour(), tm1.GetMinute(), tm1.GetSecond());
		m_pScheduler->startTime(time);
		
		CString s;
		m_pScheduler->tipsType((Data::TipsType)isAlarm);
		m_cmbRing.GetWindowText(s);
		m_pScheduler->ring(Util::StringOp::FromCString(s));
		m_alarmEdit.GetWindowText(s);
		m_pScheduler->what(Util::StringOp::FromCString(s));
		m_pScheduler->Update();
	}
}

void CLunarderDlg::SetAlarmDateBase()
{
	if(m_bIsNewAlarm)
	{
		m_pScheduler = boost::shared_ptr<Data::Scheduler> (new Data::Scheduler);		
		BOOL isAlarm = m_MJPGList.GetUnitIsDownStatus(210);
		CTime tm1, tm2;		
		m_dtTime.GetTime(tm1);
		m_dtDate.GetTime(tm2);
		int y, m, d, h, mi, s1;
		y = tm2.GetYear();
		m = tm2.GetMonth();
		d = tm2.GetDay();
		h = tm1.GetHour();
		mi = tm1.GetMinute();
		s1 = tm1.GetSecond();

		CTime time = CTime(y, m, d, h, mi, s1);
		m_pScheduler->startTime(time);
		
		CString s;
		m_pScheduler->tipsType((Data::TipsType)isAlarm);
		m_cmbRing.GetWindowText(s);
		m_pScheduler->ring(Util::StringOp::FromCString(s));
		m_alarmEdit.GetWindowText(s);
		m_pScheduler->what(Util::StringOp::FromCString(s));
		m_pScheduler->Insert();
	}
	else
	{
		SetCurrentAlarmBase();
	}
	((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->SetAlarmList();
}

void CLunarderDlg::SetAlarmCtrl(BOOL isNew)
{
	m_bIsNewAlarm = isNew;
	if(isNew)
	{
		m_nAlarmCurrent = -1;
		m_MJPGList.SetUnitIsDownStatus(210, FALSE);
		m_MJPGList.SetUnitIsDownStatus(211, TRUE);
		
		CTime tm1 = CTime(m_Year, m_Month, m_Day, 0, 0, 0);
		CTime tm2 = CTime(m_Year, m_Month, m_Day, 8, 0, 0);
		m_dtTime.SetTime(&tm2);
		m_dtDate.SetTime(&tm1);
		m_cmbRing.SetCurSel(0);
		m_alarmEdit.SetWindowText(L"");
	}
	else
	{
		CTime tm = m_pScheduler->startTime();
		
		Data::TipsType type = m_pScheduler->tipsType();
		m_MJPGList.SetUnitIsDownStatus(210, type);
		m_MJPGList.SetUnitIsDownStatus(211, !type);
			
		int y, m, d, h, mi, s1;
		y = tm.GetYear();
		m = tm.GetMonth();
		d = tm.GetDay();
		h = tm.GetHour();
		mi = tm.GetMinute();
		s1 = tm.GetSecond();

		m_dtTime.SetTime(&tm);
		m_dtDate.SetTime(&tm);

		std::string s = m_pScheduler->ring();
		m_cmbRing.SetWindowText_(Util::StringOp::ToCString(s));
		m_cmbRing.Invalidate();
		s = m_pScheduler->what();
		m_alarmEdit.SetWindowText(L"");
		m_alarmEdit.SetWindowText(Util::StringOp::ToCString(s));
		//wangzhenxing20100610
		m_pScheduler->tipsType(Data::ttRead);
		m_pScheduler->Update();
		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pMainDlg->FindTodayAlarm();
	}
}

BOOL CLunarderDlg::IsHaveAlarm(int y, int m ,int d)
{
	m_nAlarmCount = 0;
	m_nAlarmCurrent = 0;
	
	CTime time = CTime(y, m, d, 0, 0, 0);

	std::string filter = "startTime BETWEEN '";
	char buff[32];
	memset(buff, 0, 32);
	sprintf(buff, "%04d%02d%02d000000", time.GetYear(), time.GetMonth(),time.GetDay());
	filter += buff;
	filter += "' AND '";
	memset(buff, 0, 32);
	sprintf(buff, "%04d%02d%02d235959", time.GetYear(), time.GetMonth(),time.GetDay());
	filter += buff;
	filter += "'";
	m_result = Data::Scheduler::GetFromDatabaseByOrder(filter, "startTime");
	if (!m_result.empty())
	{
		m_nAlarmCount = m_result.size();
		m_nAlarmCurrent = 0;
		m_pScheduler = m_result[0];
		return TRUE;
	}
	return FALSE;
}

void CLunarderDlg::OnStopTryRing(WPARAM w, LPARAM l)
{
	if(m_MJPGList.GetUnitIsDownStatus(209))
	{
		m_MJPGList.SetUnitIsDownStatus(209, FALSE);
		m_MJPGList.SetUnitIsShow(209, TRUE);

		((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pMainDlg->m_mainMp3Dlg_->SendMessage(WM_OUTEVENT, 0, 1);
	}
}

void CLunarderDlg::StopTryRing()
{
	if(m_MJPGList.GetUnitIsDownStatus(209))
	{
		((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->SetMsgWnd(this);
		((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->StartRing(L"");
	}
}

void CLunarderDlg::OnTimer(UINT nIDEvent)
{
	if(1 == nIDEvent)
	{
		KillTimer(nIDEvent);
		ShowControl();
	}
}

void CLunarderDlg::GetSchedulerCount()
{
	CString str;
	str.Format(L"%d", m_nAlarmCount);
	m_MJPGList.SetUnitText(214, str, FALSE);
	m_MJPGList.Invalidate();
}

void CLunarderDlg::NewAlarm(BOOL isNew)
{
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	m_bIsNewAlarm = isNew;
	if(m_nAlarmCount < 20)
	{
		m_nAlarmCurrent = -1;
		m_MJPGList.SetUnitIsDownStatus(210, FALSE);
		m_MJPGList.SetUnitIsDownStatus(211, TRUE);
		m_MJPGList.SetUnitIsShow(210, TRUE);
		m_MJPGList.SetUnitIsShow(211, TRUE);
		
		CTime tm2 = CTime(m_Year, m_Month, m_Day, 8, 0, 0);
		m_dtTime.SetTime(&tm2);
		m_cmbRing.SetCurSel(0);
		m_alarmEdit.SetWindowText(L"");
	}
	else
	{
		if(isNew)
		{
			main->m_pWarningNoFlashDlg->SetTitle(L"记事项已超过最大值");
			main->m_pWarningNoFlashDlg->ShowWindow_(SW_SHOW);
		}
		else
		{
			m_nAlarmCurrent = -1;
			m_MJPGList.SetUnitIsDownStatus(211, FALSE);
			m_MJPGList.SetUnitIsDownStatus(210, TRUE);
			m_MJPGList.SetUnitIsShow(210, TRUE);
			m_MJPGList.SetUnitIsShow(211, TRUE);
			
			CTime tm2 = CTime(m_Year, m_Month, m_Day, 8, 0, 0);
			m_dtTime.SetTime(&tm2);
			m_cmbRing.SetCurSel(0);
			m_alarmEdit.SetWindowText(L"");
		}
	}
}