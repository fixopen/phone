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

	m_type = 0;  //缺省记事
	m_maindeskIn = FALSE;
	m_curIndex = -1;

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
		ON_BN_CLICKED(IDC_BTN_YL, OnYleft)
		ON_BN_CLICKED(IDC_BTN_YR, OnYright)
		ON_BN_CLICKED(IDC_BTN_ML, OnMleft)
		ON_BN_CLICKED(IDC_BTN_MR, OnMright)
		ON_BN_CLICKED(IDC_BTN_UP, OnUp)
		ON_BN_CLICKED(IDC_BTN_DOWN, OnDown)
		ON_BN_CLICKED(IDC_LUNARDER_OK, OnOk)
		ON_BN_CLICKED(IDC_LUNARDER_CANCEL, OnCancel)
		ON_BN_CLICKED(IDC_LUNARDER_DELETE, OnDelete)
		ON_MESSAGE(WM_LUNAR_CLICKED, OnLunarClick)
		ON_MESSAGE(WM_CLICKMJPG_TOAPP, OnClickMJPG)
		ON_MESSAGE(WM_DELETESELITEM, OnDeleteItem)
		ON_MESSAGE(WM_STOPTRYRING, OnStopTryRing)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLunarderDlg message handlers
#define ADD_OFFSET 0
BOOL CLunarderDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	/*
	CString s;

	CRect rt;
	rt = CRect(5, 8, 5+18, 8+15);
	m_BtnYleft.Create(L"", Data::g_lunarderNoteLeftBMPID[0][Data::g_skinstyle], Data::g_lunarderNoteLeftBMPID[1][Data::g_skinstyle],WS_CHILD|WS_VISIBLE, rt, this, IDC_BTN_YL);
	rt = CRect(25, 8, 102, 23);
	m_StcYear.Create(L"", WS_CHILD|WS_VISIBLE, rt, this);
	rt = CRect(104, 8, 104+18, 8+15);
	m_BtnYright.Create(L"", Data::g_lunarderNoteRightBMPID[0][Data::g_skinstyle], Data::g_lunarderNoteRightBMPID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, rt, this, IDC_BTN_YR);
	
	rt = CRect(156, 8, 156+18, 23);
	m_BtnMleft.Create(L"", Data::g_lunarderNoteLeftBMPID[0][Data::g_skinstyle], Data::g_lunarderNoteLeftBMPID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, rt, this, IDC_BTN_ML);
	rt = CRect(178, 8, 216, 23);
	m_StcMonth.Create(L"", WS_CHILD|WS_VISIBLE, rt, this);
	rt = CRect(216, 8, 216+18, 23);
	m_BtnMright.Create(L"", Data::g_lunarderNoteRightBMPID[0][Data::g_skinstyle], Data::g_lunarderNoteRightBMPID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, rt, this, IDC_BTN_MR);
	
	m_BtnMright.SetBackRGB(Data::g_allFramBackRGB[Data::g_skinstyle]);
	m_BtnMleft.SetBackRGB(Data::g_allFramBackRGB[Data::g_skinstyle]);
	m_BtnYright.SetBackRGB(Data::g_allFramBackRGB[Data::g_skinstyle]);
	m_BtnYleft.SetBackRGB(Data::g_allFramBackRGB[Data::g_skinstyle]);
	m_StcMonth.SetColor(RGB(0, 0, 0), Data::g_allFramBackRGB[Data::g_skinstyle]);
	m_StcYear.SetColor(RGB(0, 0, 0), Data::g_allFramBackRGB[Data::g_skinstyle]);

	rt = CRect(375-4, 28+4, 375-4+15, 28+4+15);
	m_BtnUleft.Create(L"", Data::g_lunarderNoteUpBMPID[0][Data::g_skinstyle], Data::g_lunarderNoteUpBMPID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, rt, this, IDC_BTN_UP);
	rt = CRect(391-4, 28+4, 391-4+15, 28+4+15);
	m_BtnDright.Create(L"", Data::g_lunarderNoteDownBMPID[0][Data::g_skinstyle], Data::g_lunarderNoteDownBMPID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, rt, this, IDC_BTN_DOWN);
	m_BtnUleft.SetBackRGB(Data::g_allFramBackRGB[Data::g_skinstyle]);
	m_BtnDright.SetBackRGB(Data::g_allFramBackRGB[Data::g_skinstyle]);

	m_btnExit.Create(L"", Data::g_buttonExitBMPID[0][Data::g_skinstyle], Data::g_buttonExitBMPID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(455, 7, 455+19, 7+17), this, IDC_BUTTON_LUNARDEREXIT);
	m_lunarder.Create(&m_StcYear, &m_StcMonth, WS_VISIBLE|WS_CHILD, CRect(8, 27, 232, 196), this, IDC_LUNARDER_LUNARDER);

	SYSTEMTIME curtime;
	GetLocalTime(&curtime);
	m_lunarder.SetData(curtime.wYear, curtime.wMonth, curtime.wDay);
	m_lunarder.SetIsTipStatic(TRUE);
	
	m_Line1Static.Create(L"", WS_VISIBLE|WS_CHILD, CRect(232, 27, 232+1, 195), this);
	m_Line2Static.Create(L"", WS_VISIBLE|WS_CHILD, CRect(237, 27+4, 237+1, 195-2), this);
	m_Line1Static.SetColor(RGB(0, 0, 0), RGB(255, 255, 255));
	m_Line2Static.SetColor(RGB(0, 0, 0), Data::g_lunarderLineRGB[Data::g_skinstyle]);

	m_Line3Static.Create(L"", WS_VISIBLE|WS_CHILD, CRect(238, 30, 403, 30+1), this);
	m_Line4Static.Create(L"", WS_VISIBLE|WS_CHILD, CRect(238, 193, 404, 193+1), this);
	m_Line3Static.SetColor(RGB(0, 0, 0), Data::g_lunarderLineRGB[Data::g_skinstyle]);
	m_Line4Static.SetColor(RGB(0, 0, 0), RGB(255, 255, 255));

	m_Line5Static.Create(L"", WS_VISIBLE|WS_CHILD, CRect(237, 30, 237+1, 193), this);
	m_Line6Static.Create(L"", WS_VISIBLE|WS_CHILD, CRect(404, 30, 404+1, 193), this);
	m_Line5Static.SetColor(RGB(0, 0, 0), Data::g_lunarderLineRGB[Data::g_skinstyle]);
	m_Line6Static.SetColor(RGB(0, 0, 0), RGB(255, 255, 255));


	m_Edit.Create(WS_CHILD|WS_VISIBLE | ES_MULTILINE | ES_AUTOVSCROLL | ES_WANTRETURN , CRect(238, 45+4, 407-4, 192), this, 0xFFFF);

	char txt[24];
	sprintf(txt, "%04d-%02d-%02d %s:", curtime.wYear, curtime.wMonth, curtime.wDay, Data::LanguageResource::Get(Data::RI_NOTE_TITLE).c_str());
	s = txt;
	m_titleStatic.Create(s, WS_VISIBLE|WS_CHILD, CRect(239, 28+4, 407-4, 28+4+16), this);
	m_titleStatic.SetLeftMargin(5);
	m_titleStatic.SetColor(RGB(0, 0, 0), Data::g_lunarderTitleRGB[Data::g_skinstyle]);

	s = Data::LanguageResource::Get(Data::RI_COMN_OKBTN).c_str();
	m_btnOk.Create(s, Data::g_buttonArcBMPALLDIALOGID[0][Data::g_skinstyle], Data::g_buttonArcBMPALLDIALOGID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(413, 40, 413+56, 40+20), this, IDC_LUNARDER_OK);
	s = Data::LanguageResource::Get(Data::RI_COMN_CANCELBTN).c_str();
	m_btnCancel.Create(s, Data::g_buttonArcBMPALLDIALOGID[0][Data::g_skinstyle], Data::g_buttonArcBMPALLDIALOGID[1][Data::g_skinstyle], WS_CHILD, CRect(413, 65, 413+56, 65+20), this, IDC_LUNARDER_CANCEL);
	s = Data::LanguageResource::Get(Data::RI_RECORD_DELETE).c_str();
	m_btnDelete.Create(s, Data::g_buttonArcBMPALLDIALOGID[0][Data::g_skinstyle], Data::g_buttonArcBMPALLDIALOGID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(413, 65, 413+56, 65+20), this, IDC_LUNARDER_DELETE);
	m_btnCancel.SetBackRGB(Data::g_allFramAngleBackLineRGB[Data::g_skinstyle]);
	m_btnOk.SetBackRGB(Data::g_allFramAngleBackLineRGB[Data::g_skinstyle]);
	m_btnDelete.SetBackRGB(Data::g_allFramAngleBackLineRGB[Data::g_skinstyle]);
	
	m_backStatic.Create(CRect(0, 0, 480, 204), this, 1);

	OnLunarClick(0, 0);
	*/

//	m_pScheduler = boost::shared_ptr<Data::Scheduler>();

	m_dtDate.Create(WS_CHILD|DTS_SHORTDATEFORMAT, CRect(159, 80, 159+184, 80+32), this, IDC_SETTING_DATE, 28, CDateCtrl::em_mode::modeDate);
	m_dtTime.Create(WS_CHILD|DTS_TIMEFORMAT, CRect(356, 80, 356+154, 80+32), this, IDC_SETTING_TIME, 28, CDateCtrl::em_mode::modeTime);


	m_rdoIsAlarm.Create(L"", WS_CHILD, CRect(151, 138, 151+36, 138+36), this, IDC_RADIO_SETTING_STATIC);
	m_rdoIsAlarm.SetColor(RGB(0, 0, 0), RGB(237, 237, 237));
	m_rdoIsNoAlarm.Create(L"", WS_CHILD, CRect(239, 138, 239+36, 138+36), this, IDC_RADIO_SETTING_STATIC);
	m_rdoIsNoAlarm.SetColor(RGB(0, 0, 0), RGB(237, 237, 237));

	CButton *pButton[2];
	pButton[0] = &m_rdoIsAlarm;
	pButton[1] = &m_rdoIsNoAlarm;
	
	m_rdoIsAlarm.SetGroupButton(pButton, 2);
	m_rdoIsNoAlarm.SetGroupButton(pButton, 2);

	m_cmbRing.Create(WS_CHILD, CRect(159, 195, 508, 410), this, IDC_COMBOBOX_CONTACTGROUP_SOUNDTIP);
	
	SetRingLst();

	m_alarmEdit.Create(WS_CHILD|ES_MULTILINE | ES_AUTOVSCROLL | ES_WANTRETURN , CRect(160, 257, 508, 395), this, 0xFFFF);
	m_alarmEdit.SetLimitText(768);
//	m_scollbar.Create(L"", WS_CHILD|WS_VISIBLE|WS_GROUP, CRect(451, 253, 451+SCROLL_ARROW_WIDTH+1, 395), this, 0);
//	m_scollbar.SetParam(0, 0, 0, 5, this);

	m_MJPGList.Create(L"", WS_VISIBLE|WS_CHILD, CRect(0, 0, 600, 420), this);
	m_MJPGList.SetCurrentLinkFile(".\\adv\\mjpg\\k1\\中文\\日历.xml");
	m_MJPGList.SetMJPGRect(CRect(0, 0, 600, 420));
	m_bIsMainLunder = TRUE;
	SYSTEMTIME curtime;
	GetLocalTime(&curtime);
	SetData(curtime.wYear, curtime.wMonth, curtime.wDay, FALSE);
	
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
	if(m_maindeskIn)
		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->SwitchPanel_(IDC_BUTTON_MAIN);
	else
		GetParent()->SendMessage(WM_CHANGEWINDOW, (WPARAM)this, (LPARAM)SW_HIDE);
}

void CLunarderDlg ::OnYleft()
{
	if(m_Month == 1)
	{
		m_Month = 12;
		m_Year--;
	}
	else
		m_Month--;
	SetData(m_Year, m_Month, m_Day);
	
	//m_lunarder.OnYleft();
}
void CLunarderDlg::OnYright()
{
	//m_lunarder.OnYright();
	if(m_Month == 12)
	{
		m_Month = 1;
		m_Year++;
	}
	else
		m_Month++;
	SetData(m_Year, m_Month, m_Day);
}

void CLunarderDlg::OnMleft()
{
//	m_lunarder.OnMleft();
}

void CLunarderDlg::OnMright()
{
//	m_lunarder.OnMright();
}

void CLunarderDlg::OnUp()
{
//	m_Edit.SendMessage(WM_VSCROLL, MAKELONG(SB_PAGEUP,0),NULL);
//	m_Edit.Invalidate();
}

void CLunarderDlg::OnDown()
{
//	m_Edit.SendMessage(WM_VSCROLL, MAKELONG(SB_PAGEDOWN,0),NULL);  
//	m_Edit.Invalidate();
}

void CLunarderDlg::OnLunarClick(WPARAM w, LPARAM l)
{
	/*
	UINT16 y;
	UINT8 m;
	UINT8 d;
	m_bIsHaveNote = FALSE;
	m_lunarder.GetData(y, m, d);
	if(w == 0)		//记事
	{
		m_type = 0;
		m_Edit.SetIsAutoInput(TRUE);
		char txt[24];
		sprintf(txt, "%04d-%02d-%02d %s:", y, m, d, Data::LanguageResource::Get(Data::RI_NOTE_TITLE).c_str());
		CString s = txt;
		m_titleStatic.SetWindowText(s);
		m_Edit.SetWindowText(L"");
		
		CTime time = CTime(y, m, d, 0, 0, 0);
		std::vector<boost::shared_ptr<Data::Scheduler> > result = Data::Scheduler::GetFromDatabase("startTime = '" + Util::StringOp::FromTimestamp(time)+"'"); 
		if (!result.empty())
		{
			m_bIsHaveNote = TRUE;
			m_pScheduler = result[0];
			CString s = m_pScheduler->what().c_str();
			m_Edit.SetWindowText(s);
		}
	}
	
	else if(w == 1)   //老黄历
	{
		char data[512];
		memset(data, 0, 512);
		CTime time = CTime(y, m, d, 0, 0, 0);
		if(Logical::LunarderDate::FindHuangDaoData(time, data))
		{
			m_type = 1; //黄历状态
			m_Edit.SetIsAutoInput();
			char txt[24];
			sprintf(txt, "%04d-%02d-%02d:", y, m, d);
			CString s = txt;
			m_titleStatic.SetWindowText(s);
			s = data;
			m_Edit.SetWindowText(L"");
			m_Edit.SetWindowText(s);
		}
		else
		{
			m_type = 0;
			m_Edit.SetIsAutoInput(TRUE);
			char txt[24];
			sprintf(txt, "%04d-%02d-%02d %s:", y, m, d, Data::LanguageResource::Get(Data::RI_NOTE_TITLE).c_str());
			CString s = txt;
			m_titleStatic.SetWindowText(s);
			m_Edit.SetWindowText(L"");
			
			CTime time = CTime(y, m, d, 0, 0, 0);
			std::vector<boost::shared_ptr<Data::Scheduler> > result = Data::Scheduler::GetFromDatabase("startTime = '" + Util::StringOp::FromTimestamp(time)+"'"); 
			if (!result.empty())
			{
				m_bIsHaveNote = TRUE;
				m_pScheduler = result[0];
				CString s = m_pScheduler->what().c_str();
				m_Edit.SetWindowText(s);
			}
		}
	}
	*/
}

//保存
void CLunarderDlg::OnOk()
{
	/*
	if(m_type == 0)
	{
		UINT16 y;
		UINT8 m;
		UINT8 d;
		m_lunarder.GetData(y, m, d);
		CTime time = CTime(y, m, d, 0, 0, 0);
		if(m_bIsHaveNote)
		{
			m_pScheduler->startTime(time);
			CString s;
			m_Edit.GetWindowText(s);
			m_pScheduler->what(Util::StringOp::FromCString(s));
			m_pScheduler->Update();
		}
		else
		{
			m_pScheduler = boost::shared_ptr<Data::Scheduler> (new Data::Scheduler);
			m_pScheduler->startTime(time);
			CString s;
			m_Edit.GetWindowText(s);
			m_pScheduler->what(Util::StringOp::FromCString(s));
			m_pScheduler->Insert();
		}
		m_lunarder.SetStcDay();
	}
	*/
}

void CLunarderDlg::OnCancel()
{

}

//删除
void CLunarderDlg::OnDelete()
{
	/*
	if(m_type == 0)
	{
		if(m_bIsHaveNote)
		{
			m_pScheduler->Remove();	
			m_lunarder.SetStcDay();
			OnLunarClick(0, 0);
		}
		else
		{
			
		}
	}
	*/
}

/*
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
	Data::LanguageResource::Get(Data::RI_LANUDER_MONTH12).c_str()
};

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
*/

extern const char *month[];
extern const char *day[];
extern const char *day1[];

void CLunarderDlg::SetData(int y, int m, int d, BOOL flag)
{
	/*
	m_lunarder.SetData(y, m, d, TRUE);	
	OnLunarClick(0, 0);
	*/
	
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

	char txt[64];
	sprintf(txt, "%04d年%02d月", m_Year, m_Month);
	CString  sT = txt;
	m_MJPGList.SetUnitText(4, sT, FALSE);

	int nDay = Logical::LunarderDate::MonthDays(m_Year, m_Month);
	int nWeek = Logical::LunarderDate::WeekDay(m_Year, m_Month, 1);

	if(m_Day > nDay)
		m_Day = nDay;

	m_MJPGList.SetUnitIsDownStatus(m_curIndex, FALSE);
	
	for(int i = 0; i < 42; i++)
	{
		CString s1 = "";
		int nIndex = i+5;
		m_MJPGList.SetUnitColor(nIndex, font_black, isDraw, TRUE);
		if(i < nWeek)
		{
			m_MJPGList.SetUnitText(nIndex, s1, isDraw, TRUE);
			m_MJPGList.SetUnitIsShow(nIndex, isDraw, TRUE);
		}
		else if(i >= (nWeek+nDay))
		{
			m_MJPGList.SetUnitText(nIndex, s1, isDraw, TRUE);
			m_MJPGList.SetUnitIsShow(nIndex, isDraw,TRUE);
		}
		else
		{
			s1.Format(_T("%d \r\n"), (i+1-nWeek));
		
			////////////////start/////////////
			UINT16 y, m, d, ret;
			ret = Logical::LunarderDate::GetLunarDate(m_Year, m_Month, i+1-nWeek, y, m, d);
			if(ret)
			{
				m_MJPGList.SetUnitColor(nIndex, font_green, isDraw, TRUE);
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
				m_MJPGList.SetUnitIsDownStatus(nIndex, TRUE);
				m_curIndex = nIndex;
			}

			m_MJPGList.SetUnitText(nIndex, s1, isDraw, TRUE);
			m_MJPGList.SetUnitIsShow(nIndex, isDraw, TRUE);

			//如果有提醒，将显示的文字改为红色
			int ret2 = IsHaveAlarm(m_Year, m_Month, i+1-nWeek);
			if(ret2>0)
			{
				if(ret2 == (int)(Data::ttNet) || ret2 == (int)(Data::ttNetRead))
					m_MJPGList.SetUnitColor(nIndex, font_red, isDraw, TRUE);
				else
					m_MJPGList.SetUnitColor(nIndex, font_green, isDraw, TRUE);
			}
		}
		
		//判断天，toggle
	}

}

void CLunarderDlg ::OnClickMJPG(WPARAM w, LPARAM l)
{
	if(w != 101)
		StopTryRing();

	switch(w)
	{
	case 1:				//查看
		if(IsHaveAlarm(m_Year, m_Month, m_Day))
		{
			ShowAlarmDlg(FALSE);
		}
		
		break;
	case 2:				//上翻日期	
		OnYleft();
		break;
	case 3:				//下翻日期 
		OnYright();
		break;
	case 4:				//记事
		IsHaveAlarm(m_Year, m_Month, m_Day);
		ShowAlarmDlg(TRUE);
		break;
	case 100:			//退出日历		
		OnExit();
		break;
	case 200:			//退出日历记事
		ShowLunarderDlg();
		break;
	case 201:			//清除该记事
		DeleteCurrentAlarm();
		break;
	case 204:			//该天上一条记事
		SetPreAlarm();
		break;
	case 207:			//该天下一条记事
		SetNextAlarm();
		break;
	case 202:			//确定
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
				((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pWarningNoFlashDlg->SetType(0);     //提示输入内容
				((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pWarningNoFlashDlg->ShowWindow_(SW_SHOW);
			}
		}
		break;
	case 205:			//记事上翻页
		m_alarmEdit.SendMessage(WM_VSCROLL, MAKELONG(SB_PAGEUP,0),NULL);
		m_alarmEdit.Invalidate();
		break;
	case 206:			//记事下翻页
		m_alarmEdit.SendMessage(WM_VSCROLL, MAKELONG(SB_PAGEDOWN,0),NULL);
		m_alarmEdit.Invalidate();
		break;
	case 203:			//新建
		{
			if(m_nAlarmCount < 20)
				ShowAlarmDlg(TRUE);
		}
		break;
	case 101:   //试听音乐
		{
			{
				if(!m_MJPGList.GetUnitIsDownStatus(101))
				{
					CString str;
					m_cmbRing.GetWindowText(str);
					if(str == "")
						break;
					CString s = "\\flashdrv\\my_ring\\"; 
					s += str;

					((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pMainDlg->m_mainMp3Dlg_->OnTimer(1002); //SendMessage(WM_OUTEVENT, 0, 0);
					
					((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->SetMsgWnd(this);
					((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->StartRing((LPTSTR)(LPCTSTR)s, 1);
					m_MJPGList.SetUnitIsDownStatus(101, TRUE);
					m_MJPGList.SetUnitIsShow(101, TRUE);
				}
				else
				{
					StopTryRing();
				}
				break;
			}
		}
	}
	if( 6 <= w && w <= 47) 
	{
		int nDay = Logical::LunarderDate::MonthDays(m_Year, m_Month);
		int nWeek = Logical::LunarderDate::WeekDay(m_Year, m_Month, 1);
		int i = w-6;
		if(i >= nWeek && i < (nWeek+nDay))
		{
			int nIndex = i+5;
			m_MJPGList.SetUnitIsDownStatus(m_curIndex, FALSE);
			m_MJPGList.SetUnitIsDownStatus(nIndex, TRUE);
			m_MJPGList.Invalidate();
			m_curIndex = nIndex;
			m_Day = i - nWeek + 1;

			//显示提醒
			if(IsHaveAlarm(m_Year, m_Month, m_Day))
			{
				ShowAlarmDlg(FALSE);
			}
		}
	}
}

//显示日历
void CLunarderDlg::ShowLunarderDlg()
{
	m_bIsMainLunder = TRUE;
	m_MJPGList.SetCurrentLinkFile(".\\adv\\mjpg\\k1\\中文\\日历.xml");
	SetData(m_Year, m_Month, m_Day, TRUE);
	m_MJPGList.Invalidate();
	SetCtrlIsShow(FALSE);
}
//显示alarm
void CLunarderDlg::ShowAlarmDlg(BOOL isNew)
{
	m_bIsMainLunder = FALSE;
	SetCtrlIsShow(TRUE);
	m_MJPGList.SetCurrentLinkFile(".\\adv\\mjpg\\k1\\中文\\日历记事.xml");
	m_MJPGList.Invalidate();
	SetAlarmCtrl(isNew);	
}

void CLunarderDlg::SetCtrlIsShow(BOOL isShow)
{
	m_rdoIsAlarm.ShowWindow(isShow);
	m_rdoIsNoAlarm.ShowWindow(isShow);
	m_dtTime.ShowWindow(isShow);
	m_dtDate.ShowWindow(isShow);
	m_alarmEdit.ShowWindow(isShow);
	m_cmbRing.ShowWindow(isShow);
//	m_scollbar.ShowWindow(isShow);
}

void CLunarderDlg::OnDeleteItem(WPARAM w, LPARAM l)
{
	m_pScheduler->Remove();
	if(IsHaveAlarm(m_Year, m_Month, m_Day))
	{
		SetAlarmCtrl(FALSE);
	}
	else
	{
		ShowLunarderDlg();
	}
	((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->SetAlarmList();
}

void CLunarderDlg::DeleteCurrentAlarm()
{
	if(!m_bIsNewAlarm && m_pScheduler)
	{
		std::string strTemp = ".\\adv\\mjpg\\k1\\common\\删除记事.bmp";
		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetDelTip(strTemp.c_str());
		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetHWnd(m_hWnd);
		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->ShowWindow_(TRUE);
	}

	/*
	if(m_pScheduler)
	{
		if(MessageBox(L"确定删除该记录吗?", L"", MB_YESNO) == IDYES)
		{
			m_pScheduler->Remove();
			if(IsHaveAlarm(m_Year, m_Month, m_Day))
			{
				SetAlarmCtrl(FALSE);
			}
			else
			{
				ShowLunarderDlg();
			}
		}
	}
	*/
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
		BOOL isAlarm = m_rdoIsAlarm.GetCheck_();
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
		BOOL isAlarm = m_rdoIsAlarm.GetCheck_();
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

void CLunarderDlg ::SetAlarmCtrl(BOOL isNew)
{
	m_bIsNewAlarm = isNew;
	if(isNew)
	{
		m_nAlarmCurrent = -1;
		m_rdoIsAlarm.SetCheck_(0);
		m_rdoIsNoAlarm.SetCheck_(1);
		
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
		m_rdoIsAlarm.SetCheck_(type);
		m_rdoIsNoAlarm.SetCheck_(!type);
			
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
		m_alarmEdit.SetWindowText(Util::StringOp::ToCString(s));
	}
	
	char txt[24];
	sprintf(txt, "%d/%d", m_nAlarmCurrent+1, m_nAlarmCount);
	CString sTitle = txt;
	m_MJPGList.SetUnitText(100, sTitle, TRUE);
	m_rdoIsAlarm.Invalidate();
	m_rdoIsNoAlarm.Invalidate();
}

int CLunarderDlg::IsHaveAlarm(int y, int m ,int d)
{
	m_nAlarmCount = 0;
	m_nAlarmCurrent = 0;
	
	CTime time = CTime(y, m, d, 0, 0, 0);

	std::string filter = "startTime BETWEEN '";
	char buff[32];
	memset(buff, 0, 32);
	sprintf(buff, "%04d%02d%02d000000", time.GetYear(), time.GetMonth(),time.GetDay());//, dateFrom.GetHour(), dateFrom.GetMinute(), dateFrom.GetSecond());
	filter += buff;
	filter += "' AND '";
	memset(buff, 0, 32);
	sprintf(buff, "%04d%02d%02d235959", time.GetYear(), time.GetMonth(),time.GetDay());//, dateTo.GetHour(), dateTo.GetMinute(), dateTo.GetSecond());
	filter += buff;
	filter += "'";
	m_result = Data::Scheduler::GetFromDatabaseByOrder(filter, "startTime"); 
	if (!m_result.empty())
	{
		m_nAlarmCount = m_result.size();
		m_nAlarmCurrent = 0;
		m_pScheduler = m_result[0];
		int ret = m_pScheduler->tipsType();
		return ret;
	}
	return FALSE;
}

void CLunarderDlg::OnStopTryRing(WPARAM w, LPARAM l)
{
	if(m_MJPGList.GetUnitIsDownStatus(101))
	{
		m_MJPGList.SetUnitIsDownStatus(101, FALSE);
		m_MJPGList.SetUnitIsShow(101, TRUE);

		((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pMainDlg->m_mainMp3Dlg_->SendMessage(WM_OUTEVENT, 0, 1);
	}
}

void CLunarderDlg::StopTryRing()
{
	if(m_MJPGList.GetUnitIsDownStatus(101))
	{
		((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->SetMsgWnd(this);
		((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->StartRing(L"");
	}
}