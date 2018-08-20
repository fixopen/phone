// YHTelDlg.cpp : implementation file
//

#include "stdafx.h"
#include "../multimediaphone.h"
#include "../MultimediaPhoneDlg.h"
#include "3GSysToolTelDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// C3GSysToolDlg dialog


C3GSysToolDlg::C3GSysToolDlg(CWnd* pParent /*=NULL*/)
	: CDialog(C3GSysToolDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(C3GSysToolDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void C3GSysToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(C3GSysToolDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(C3GSysToolDlg, CDialog)
	//{{AFX_MSG_MAP(C3GSysToolDlg)
	ON_MESSAGE(WM_CLICKMJPG_TOAPP, OnClickMJPG)
//	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// C3GSysToolDlg message handlers
/*
void C3GSysToolDlg::OnTimer(UINT nIDEvent)
{
	if(IsWindowVisible())
	{
		if(nIDEvent == 1)
		{
			
		}
	}
	CDialog::OnTimer(nIDEvent);
}
*/

BOOL C3GSysToolDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	m_MJPGList.Create(L"", WS_VISIBLE|WS_CHILD, CRect(0, 0, 600, 420), this);
	m_MJPGList.SetCurrentLinkFile(".\\adv\\mjpg\\k1\\中文\\系统工具.xml");
	m_MJPGList.SetMJPGRect(CRect(0, 0, 600, 420));
//	SetTimer(1, 1000, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void C3GSysToolDlg::OnClickMJPG(WPARAM w, LPARAM l)
{
	CMultimediaPhoneDlg *pMainDlg = ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd));
	switch(w)
	{
	case 1:			//家庭影院
		pMainDlg->m_pMainDlg->m_mainmenuDlg_->OnVideoBtn();
		break;
	case 2:			//计算器
		pMainDlg->m_pMainDlg->m_mainmenuDlg_->OnCalculBtn();
		break;
	case 3:			//电子日历
		SYSTEMTIME curtime;
		GetLocalTime(&curtime);
		pMainDlg->m_pMainDlg->m_pHuangLiDlg_->SetData(curtime.wYear, curtime.wMonth, curtime.wDay, TRUE);
		pMainDlg->m_pMainDlg->m_pHuangLiDlg_->ShowWindow(SW_SHOW);    //老黄历
		break;
	case 4:			//日程提醒
		pMainDlg->m_pMainDlg->m_mainmenuDlg_->OnLunarderBtn();
		break;
	case 5:
		break;
	case 6:
		break;
	case 7:
		break;
	case 8:
		break;
	case 9:
		break;
	case 10:
		break;
	case 100:
		ShowWindow(SW_HIDE);
	default:
		break;
	}
}