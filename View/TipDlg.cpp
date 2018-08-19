// deletetipdlg.cpp : implementation file
//

#include "stdafx.h"
#include "..\multimediaphone.h"
#include "..\MultimediaPhoneDlg.h"
#include "TipDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern BOOL g_bReplay;
extern BOOL g_bStartring;
/////////////////////////////////////////////////////////////////////////////
// CDeleteTipDlg dialog


CTipDlg::CTipDlg(CWnd* pParent /*=NULL*/)
	: CCEDialog(CTipDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDeleteTipDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CTipDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDeleteTipDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTipDlg, CCEDialog)

	ON_MESSAGE(WM_CLICKMJPG_TOAPP, OnClickMJPG)
	ON_WM_TIMER()

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDeleteTipDlg message handlers
void CTipDlg::OnClickMJPG(WPARAM w, LPARAM l)
{
	switch(w)
	{
	case 1001:
		OnClickOK();
		break;
	default:
		break;
	}
}
BOOL CTipDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_MJPGList.Create(L"", WS_VISIBLE|WS_CHILD, CRect(190, 105, 190+440,105+270), this);
	m_MJPGList.SetCurrentLinkFile(".\\adv\\mjpg\\k5\\中文\\提示.xml");
	m_MJPGList.SetMJPGRect(CRect(190, 105, 190+440, 105+270));

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTipDlg::OnClickOK()
{	
	ShowWindow_(SW_HIDE);
	if(g_bReplay)
	{
		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->IsSendMessage(FALSE);
		g_bReplay = FALSE;
	}
	if(g_bStartring)
	{
		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->PostMessage(WM_RINGTIME, 0, 0);
	}
}

void CTipDlg::SetTitle(CString title,int isTime,TEXTSIZE font)
{	
	m_MJPGList.SetUnitFont(1,font);
	m_MJPGList.SetUnitText(1,title,true);
	if(isTime > 0)	
	{
		SetTimer(100, isTime, NULL);
	}	
}

void CTipDlg::OnTimer(UINT nIDEvent) 
{
	if(nIDEvent == 100)
	{
		KillTimer(100);
		OnClickOK();	
	}
	CCEDialog::OnTimer(nIDEvent);
}
