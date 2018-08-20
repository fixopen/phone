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
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLunarderDlg message handlers

BOOL CLunarderDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
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

	m_pScheduler = boost::shared_ptr<Data::Scheduler>();

	OnLunarClick(0, 0);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
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
	m_lunarder.OnYleft();
}
void CLunarderDlg::OnYright()
{
	m_lunarder.OnYright();
}
void CLunarderDlg::OnMleft()
{
	m_lunarder.OnMleft();
}

void CLunarderDlg::OnMright()
{
	m_lunarder.OnMright();
}

void CLunarderDlg::OnUp()
{
	m_Edit.SendMessage(WM_VSCROLL, MAKELONG(SB_PAGEUP,0),NULL);
	m_Edit.Invalidate();
}

void CLunarderDlg::OnDown()
{
	m_Edit.SendMessage(WM_VSCROLL, MAKELONG(SB_PAGEDOWN,0),NULL);  
	m_Edit.Invalidate();
}

void CLunarderDlg::OnLunarClick(WPARAM w, LPARAM l)
{
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
}

//保存
void CLunarderDlg::OnOk()
{
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
}

void CLunarderDlg::OnCancel()
{

}

//删除
void CLunarderDlg::OnDelete()
{
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
}

void CLunarderDlg::SetData(int y, int m, int d)
{
	m_lunarder.SetData(y, m, d, TRUE);	
	OnLunarClick(0, 0);
}
