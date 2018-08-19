// YHTelDlg.cpp : implementation file
//

#include "stdafx.h"
#include "../multimediaphone.h"
#include "../MultimediaPhoneDlg.h"
#include "3GHomeJoyDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// C3GHomeJoyDlg dialog


C3GHomeJoyDlg::C3GHomeJoyDlg(CWnd* pParent /*=NULL*/)
	: CDialog(C3GHomeJoyDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(C3GHomeJoyDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void C3GHomeJoyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(C3GHomeJoyDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(C3GHomeJoyDlg, CDialog)
	//{{AFX_MSG_MAP(C3GHomeJoyDlg)
	ON_MESSAGE(WM_CLICKMJPG_TOAPP, OnClickMJPG)
//	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// C3GHomeJoyDlg message handlers
/*
void C3GHomeJoyDlg::OnTimer(UINT nIDEvent)
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

BOOL C3GHomeJoyDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	m_MJPGList.Create(L"", WS_VISIBLE|WS_CHILD, CRect(0, 0, 600, 420), this);
	m_MJPGList.SetCurrentLinkFile(".\\adv\\mjpg\\k1\\中文\\3g_家庭娱乐.xml");
	m_MJPGList.SetMJPGRect(CRect(0, 0, 600, 420));
//	SetTimer(1, 1000, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

LRESULT C3GHomeJoyDlg::OnClickMJPG(WPARAM w, LPARAM l)
{
    LRESULT result = 0;
	CMultimediaPhoneDlg *pMainDlg = ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd));
	switch(w)
	{
	case 1:			//音乐商店
		pMainDlg->m_pMainDlg->m_mainmenuDlg_->OnMp3Btn();
		break;
	case 2:			//影音天地
		pMainDlg->m_pMainDlg->m_mainmenuDlg_->OnVideoBtn();
		break;
	case 3:			//游戏集中营
		if(pMainDlg->IsConnectNet())
			pMainDlg->m_pMainDlg->StartWeb();
		break;
	case 4:			//收音机
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
    return result;
}