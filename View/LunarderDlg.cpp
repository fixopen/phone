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
		ON_MESSAGE(WM_CHANGELANGUAGE,OnChangeLanguage)
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
	rt = CRect(5, 8*VHEIGHT, 5+18, 23*VHEIGHT);
	m_BtnYleft.Create(L"", Data::g_lunarderNoteLeftBMPID[0][Data::g_skinstyle], Data::g_lunarderNoteLeftBMPID[1][Data::g_skinstyle],WS_CHILD|WS_VISIBLE, rt, this, IDC_BTN_YL);
	rt = CRect(25, 8*VHEIGHT, 102, 23*VHEIGHT);
	m_StcYear.Create(L"", WS_CHILD|WS_VISIBLE, rt, this);
	m_StcYear.SetAlign(DT_CENTER);
	rt = CRect(104, 8*VHEIGHT, 104+18, 23*VHEIGHT);
	m_BtnYright.Create(L"", Data::g_lunarderNoteRightBMPID[0][Data::g_skinstyle], Data::g_lunarderNoteRightBMPID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, rt, this, IDC_BTN_YR);
	
	rt = CRect(156, 8*VHEIGHT, 156+18, 23*VHEIGHT);
	m_BtnMleft.Create(L"", Data::g_lunarderNoteLeftBMPID[0][Data::g_skinstyle], Data::g_lunarderNoteLeftBMPID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, rt, this, IDC_BTN_ML);
	rt = CRect(178, 8*VHEIGHT, 216, 23*VHEIGHT);
	m_StcMonth.Create(L"", WS_CHILD|WS_VISIBLE, rt, this);
	m_StcMonth.SetAlign(DT_CENTER);
	rt = CRect(216, 8*VHEIGHT, 216+18, 23*VHEIGHT);
	m_BtnMright.Create(L"", Data::g_lunarderNoteRightBMPID[0][Data::g_skinstyle], Data::g_lunarderNoteRightBMPID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, rt, this, IDC_BTN_MR);
	
	m_BtnMright.SetBackRGB(Data::g_allFramBackRGB[Data::g_skinstyle]);
	m_BtnMleft.SetBackRGB(Data::g_allFramBackRGB[Data::g_skinstyle]);
	m_BtnYright.SetBackRGB(Data::g_allFramBackRGB[Data::g_skinstyle]);
	m_BtnYleft.SetBackRGB(Data::g_allFramBackRGB[Data::g_skinstyle]);
	m_StcMonth.SetColor(RGB(0, 0, 0), Data::g_allFramBackRGB[Data::g_skinstyle]);
	m_StcYear.SetColor(RGB(0, 0, 0), Data::g_allFramBackRGB[Data::g_skinstyle]);

	rt = CRect(375-4, 32*VHEIGHT, 375-4+15, (28+4+15)*VHEIGHT);
	m_BtnUleft.Create(L"", Data::g_lunarderNoteUpBMPID[0][Data::g_skinstyle], Data::g_lunarderNoteUpBMPID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, rt, this, IDC_BTN_UP);
	rt = CRect(391-4, 32*VHEIGHT, 391-4+15, 47*VHEIGHT);
	m_BtnDright.Create(L"", Data::g_lunarderNoteDownBMPID[0][Data::g_skinstyle], Data::g_lunarderNoteDownBMPID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, rt, this, IDC_BTN_DOWN);
	m_BtnUleft.SetBackRGB(Data::g_allFramBackRGB[Data::g_skinstyle]);
	m_BtnDright.SetBackRGB(Data::g_allFramBackRGB[Data::g_skinstyle]);

	m_btnExit.Create(L"", Data::g_buttonExitBMPID[0][Data::g_skinstyle], Data::g_buttonExitBMPID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(455, 7*VHEIGHT, 455+19, 7*VHEIGHT+17), this, IDC_BUTTON_LUNARDEREXIT);
	m_lunarder.Create(&m_StcYear, &m_StcMonth, WS_VISIBLE|WS_CHILD, CRect(8, 27*VHEIGHT, 232, 196*VHEIGHT), this, IDC_LUNARDER_LUNARDER);

	SYSTEMTIME curtime;
	GetLocalTime(&curtime);
	m_lunarder.SetData(curtime.wYear, curtime.wMonth, curtime.wDay);
	m_lunarder.SetIsTipStatic(TRUE);

	m_Line1Static.Create(L"", WS_VISIBLE|WS_CHILD, CRect(232, 27*VHEIGHT, 232+1, 195*VHEIGHT), this);
	m_Line2Static.Create(L"", WS_VISIBLE|WS_CHILD, CRect(237, 31*VHEIGHT, 237+1, 193*VHEIGHT), this);
	m_Line1Static.SetColor(RGB(0, 0, 0), RGB(255, 255, 255));
	m_Line2Static.SetColor(RGB(0, 0, 0), Data::g_lunarderLineRGB[Data::g_skinstyle]);

	m_Line3Static.Create(L"", WS_VISIBLE|WS_CHILD, CRect(238, 30*VHEIGHT, 403, 31*VHEIGHT), this);
	m_Line4Static.Create(L"", WS_VISIBLE|WS_CHILD, CRect(238, 193*VHEIGHT, 404, 194*VHEIGHT), this);
	m_Line3Static.SetColor(RGB(0, 0, 0), Data::g_lunarderLineRGB[Data::g_skinstyle]);
	m_Line4Static.SetColor(RGB(0, 0, 0), RGB(255, 255, 255));

	m_Line5Static.Create(L"", WS_VISIBLE|WS_CHILD, CRect(237, 30*VHEIGHT, 237+1, 193*VHEIGHT), this);
	m_Line6Static.Create(L"", WS_VISIBLE|WS_CHILD, CRect(404, 30*VHEIGHT, 404+1, 193*VHEIGHT), this);
	m_Line5Static.SetColor(RGB(0, 0, 0), Data::g_lunarderLineRGB[Data::g_skinstyle]);
	m_Line6Static.SetColor(RGB(0, 0, 0), RGB(255, 255, 255));

	m_Edit.Create(WS_CHILD|WS_VISIBLE | ES_MULTILINE | ES_AUTOVSCROLL | ES_WANTRETURN , CRect(238, 49*VHEIGHT, 407-4, 192*VHEIGHT), this, 0xFFFF);

	char txt[24];
	sprintf(txt, "%04d-%02d-%02d %s:", curtime.wYear, curtime.wMonth, curtime.wDay, Data::LanguageResource::Get(Data::RI_NOTE_TITLE).c_str());//?
	s = txt;
	m_titleStatic.Create(s, WS_VISIBLE|WS_CHILD, CRect(239, 32*VHEIGHT, 407-4, 48*VHEIGHT), this);
	m_titleStatic.SetLeftMargin(5);
	m_titleStatic.SetColor(RGB(0, 0, 0), Data::g_lunarderTitleRGB[Data::g_skinstyle]);

	s = Data::LanguageResource::Get(Data::RI_SETTING_SAVE).c_str();
	m_btnOk.Create(s, Data::g_buttonArcBMPALLDIALOGID[0][Data::g_skinstyle], Data::g_buttonArcBMPALLDIALOGID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(413, 40*VHEIGHT, 413+56, 60*VHEIGHT-2), this, IDC_LUNARDER_OK);
	s = Data::LanguageResource::Get(Data::RI_COMN_CANCELBTN).c_str();
	m_btnCancel.Create(s, Data::g_buttonArcBMPALLDIALOGID[0][Data::g_skinstyle], Data::g_buttonArcBMPALLDIALOGID[1][Data::g_skinstyle], WS_CHILD, CRect(413, 65*VHEIGHT, 413+56, 85*VHEIGHT), this, IDC_LUNARDER_CANCEL);
	s = Data::LanguageResource::Get(Data::RI_RECORD_DELETE).c_str();
	m_btnDelete.Create(s, Data::g_buttonArcBMPALLDIALOGID[0][Data::g_skinstyle], Data::g_buttonArcBMPALLDIALOGID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(413, 65*VHEIGHT, 413+56, 85*VHEIGHT), this, IDC_LUNARDER_DELETE);
	m_btnCancel.SetBackRGB(Data::g_allFramAngleBackLineRGB[Data::g_skinstyle]);
	m_btnOk.SetBackRGB(Data::g_allFramAngleBackLineRGB[Data::g_skinstyle]);
	m_btnDelete.SetBackRGB(Data::g_allFramAngleBackLineRGB[Data::g_skinstyle]);
	
	m_backStatic.Create(CRect(0, 0, 480, 204*VHEIGHT), this, 1);

	m_pScheduler = boost::shared_ptr<Data::Scheduler>();

	OnLunarClick(0, 0);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CLunarderDlg::OnExit()
{
	ShowWindow(SW_HIDE);

	KeyBoardOff();
}

void CLunarderDlg ::OnYleft()
{
	m_lunarder.OnYleft();
	
	OnLunarClick(0, 0);
}
void CLunarderDlg::OnYright()
{
	m_lunarder.OnYright();
	OnLunarClick(0, 0);
}
void CLunarderDlg::OnMleft()
{
	m_lunarder.OnMleft();
	OnLunarClick(0, 0);
}

void CLunarderDlg::OnMright()
{
	m_lunarder.OnMright();
	OnLunarClick(0, 0);
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
			if (gLangItem == Data::lChinese)
			{
				m_Edit.SetWindowText(s);
			}
			
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
			CString s_ = Util::StringOp::ToCString(m_pScheduler->what());
			if(s == s_)
				return;
			m_pScheduler->what(Util::StringOp::FromCString(s));
			m_pScheduler->Update();
		}
		else
		{
			m_pScheduler = boost::shared_ptr<Data::Scheduler> (new Data::Scheduler);
			m_pScheduler->startTime(time);
			CString s;
			m_Edit.GetWindowText(s);
			CString s_ = Util::StringOp::ToCString(m_pScheduler->what());
			if(s == s_)
				return;
			m_pScheduler->what(Util::StringOp::FromCString(s));
			m_pScheduler->Insert();
			m_bIsHaveNote = TRUE;
		}

		m_lunarder.SetStcDay();
		::PostMessage(((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pMainDlg->GetSafeHwnd(), WM_NOTENOTIFY, 0, 0);
	}
}

void CLunarderDlg::OnCancel()
{

}

void CLunarderDlg::OnDelete()
{
	if(m_type == 0)
	{
		if(m_bIsHaveNote)
		{
			m_pScheduler->Remove();	
		}
		m_lunarder.SetStcDay();
		OnLunarClick(0, 0);
		m_Edit.SetWindowText(_T(""));
		::PostMessage(((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pMainDlg->GetSafeHwnd(), WM_NOTENOTIFY, 0, 0);
	}	
}

void CLunarderDlg::SetData(int y, int m, int d)
{
	m_lunarder.SetData(y, m, d, TRUE);
	OnLunarClick(0, 0);
}
LRESULT CLunarderDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
	case WM_OK:
		OnOk();
		break;
	case WM_CANCEL:
		if ((((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pFSM->getCurrentState() == CMultimediaPhoneDlg::tsHangOff))
		{
			ShowWindow(FALSE);
			KeyBoardOff();
		}
		else
		{
			if (((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pTelephoneDlg->IsWindowVisible())
			{
				return 0;
			}
			else
			{
			  ShowWindow(FALSE);
			  KeyBoardOff();
			}
		}
		break;
	case WM_DELETE:
        OnDelete();
		break;
	case WM_KEYDOWN:
	       if ('U'==wParam)
	       {
			  OnUp();
	       }
		   if('D'==wParam)
		   {
			  OnDown();
		   }
		break;
	}
return CDialog::WindowProc(message,wParam,lParam); 
}

LRESULT CLunarderDlg::OnChangeLanguage(WPARAM wParam,LPARAM lParam)
{
	ChangeLanguage();
	m_lunarder.ChangeLanguage();
	 return 0;
}
void CLunarderDlg::ChangeLanguage()
{
	CString s;
	char txt[24];
	SYSTEMTIME curtime;
	sprintf(txt, "%04d-%02d-%02d %s:", curtime.wYear, curtime.wMonth, curtime.wDay, Data::LanguageResource::Get(Data::RI_NOTE_TITLE).c_str());
    s = txt;
    m_titleStatic.SetWindowText(s);
	s = Data::LanguageResource::Get(Data::RI_SETTING_SAVE).c_str();
	m_btnOk.SetWindowText(s);
	s = Data::LanguageResource::Get(Data::RI_COMN_CANCELBTN).c_str();
    m_btnCancel.SetWindowText(s);
	s = Data::LanguageResource::Get(Data::RI_RECORD_DELETE).c_str();
	m_btnDelete.SetWindowText(s);
}