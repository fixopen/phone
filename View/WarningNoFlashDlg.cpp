// AlarmShowDlg.cpp : implementation file
//

#include "stdafx.h"
#include "../MultimediaPhone.h"
#include "../MultimediaPhoneDlg.h"
#include "WarningNoFlashDlg.h"
#include "../Pblmember.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern BOOL g_bReplay;

/////////////////////////////////////////////////////////////////////////////
// CWarningNoFlashDlg dialog


CWarningNoFlashDlg::CWarningNoFlashDlg(CWnd* pParent /*=NULL*/)
	: CCEDialog(CWarningNoFlashDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CWarningNoFlashDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CWarningNoFlashDlg::DoDataExchange(CDataExchange* pDX)
{
	CCEDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWarningNoFlashDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWarningNoFlashDlg, CCEDialog)
	//{{AFX_MSG_MAP(CWarningNoFlashDlg)
	ON_MESSAGE(WM_CLICKMJPG_TOAPP, OnClickMJPG)
	ON_BN_CLICKED(IDC_BTN_OK, OnClickOK)
	ON_BN_CLICKED(IDC_BTN_CANCEL, OnClickCancel)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWarningNoFlashDlg message handlers

BOOL CWarningNoFlashDlg::OnInitDialog() 
{
	CCEDialog::OnInitDialog();
	m_bIsNoclick = FALSE;

	// TODO: Add extra initialization here
	m_MJPGList.Create(L"", WS_VISIBLE|WS_CHILD, CRect(180, 120, 180+440, 120+192), this);
	m_MJPGList.SetCurrentLinkFile(".\\adv\\mjpg\\k5\\ÖÐÎÄ\\¾¯¸æ¿ò.xml");
	m_MJPGList.SetMJPGRect(CRect(180, 120, 180+440, 120+192));

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CWarningNoFlashDlg::SetTitle(CString title, int nTimer)
{
	m_bIsNoclick =  FALSE;
	
	m_MJPGList.SetUnitText(4,title,false);
	m_MJPGList.SetUnitIsShow(4, true);
	
	SetTimer(3,nTimer,NULL);
	
}

void CWarningNoFlashDlg::OnClickMJPG(WPARAM w, LPARAM l)
{
	if(m_bIsNoclick)
		return;

	ShowWindow_(SW_HIDE);
	KillTimer(1);
	KillTimer(2);
	KillTimer(3);
	m_MJPGList.SetUnitIsShow(2, TRUE);

	if(g_bReplay)
	{
		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->IsSendMessage(FALSE);
		g_bReplay = FALSE;
	}
}

void CWarningNoFlashDlg::OnTimer(UINT nIDEvent)
{
	if(nIDEvent == 1)
	{
		KillTimer(1);
	}
	else if(nIDEvent == 2)
	{
		CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
		main->CancelBalckLightSaveTimer();
	}
	else
	{
		m_bIsNoclick = FALSE;
		OnClickMJPG(0,0);
	}
	CCEDialog::OnTimer(nIDEvent);
}

void CWarningNoFlashDlg::SetHWnd(HWND handle)
{
	m_handle = handle;
}

void CWarningNoFlashDlg::OnClickOK()
{
	::PostMessage(m_handle, WM_DELETESELITEM, 0, 0);
}

void CWarningNoFlashDlg::OnClickCancel()
{
	ShowWindow_(SW_HIDE);
}