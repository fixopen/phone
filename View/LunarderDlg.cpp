// LunarderDlg.cpp : implementation file
//

#include "stdafx.h"
#include "LunarderDlg.h"
#include "../Data/LanguageResource.h"
#include "../MultimediaPhoneDlg.h"
#include "../MultimediaPhone.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLunarderDlg dialog


CLunarderDlg::CLunarderDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLunarderDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLunarderDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
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
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLunarderDlg message handlers

BOOL CLunarderDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CString s;
	s = Data::LanguageResource::Get(Data::RI_MAIN_CALENDARBTN).c_str();
//	m_stcTitle.Create(s, WS_CHILD|WS_VISIBLE, CRect(4, 0, 80, 15), this);
//	m_stcTitle.SetColor(RGB(0, 0, 0), RGB(192, 192, 192));
	m_btnExit.Create(L"", WS_CHILD|WS_VISIBLE, CRect(227, 0, 242, 15), this, IDC_BUTTON_LUNARDEREXIT);
	m_btnExit.SetIcon(IDI_ICON_EXIT, CSize(12, 12));
	m_lunarder.Create(WS_VISIBLE|WS_CHILD, CRect(0, 0, 227, 204), this, IDC_LUNARDER_LUNARDER);

	SYSTEMTIME curtime;
	GetLocalTime(&curtime);
	m_lunarder.SetData(curtime.wYear, curtime.wMonth, curtime.wDay);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CLunarderDlg::OnExit()
{
	GetParent()->SendMessage(WM_CHANGEWINDOW, (WPARAM)this, (LPARAM)SW_HIDE);
}