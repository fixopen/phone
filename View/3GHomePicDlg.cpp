// YHTelDlg.cpp : implementation file
//

#include "stdafx.h"
#include "../multimediaphone.h"
#include "../MultimediaPhoneDlg.h"
#include "3GHomePicDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// C3GHomePicDlg dialog


C3GHomePicDlg::C3GHomePicDlg(CWnd* pParent /*=NULL*/)
	: CDialog(C3GHomePicDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(C3GHomePicDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void C3GHomePicDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(C3GHomePicDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(C3GHomePicDlg, CDialog)
	//{{AFX_MSG_MAP(C3GHomePicDlg)
	ON_MESSAGE(WM_CLICKMJPG_TOAPP, OnClickMJPG)
//	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// C3GHomePicDlg message handlers
/*
void C3GHomePicDlg::OnTimer(UINT nIDEvent)
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

BOOL C3GHomePicDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	m_MJPGList.Create(L"", WS_VISIBLE|WS_CHILD, CRect(0, 0, 600, 420), this, 10086);
	m_MJPGList.SetCurrentLinkFile(".\\adv\\mjpg\\k1\\中文\\3g_家庭相册.xml");
	m_MJPGList.SetMJPGRect(CRect(0, 0, 600, 420));
//	SetTimer(1, 1000, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void C3GHomePicDlg::OnClickMJPG(WPARAM w, LPARAM l)
{
	CMultimediaPhoneDlg *pMainDlg = ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd));
	switch(w)
	{
	case 1:			//我的图片库
		break;
	case 2:			//我的相册
		pMainDlg->m_pMainDlg->m_mainPhotoDlg_->OnOpenFile();
		pMainDlg->m_pMainDlg->m_mainmenuDlg_->OnPhotoBtn(1);
		break;
	case 3:			//照片冲印
		break;
	case 4:			//网络相册
		pMainDlg->m_pMainDlg->m_mainPhotoDlg_->OnOpenFile();
		pMainDlg->m_pMainDlg->m_mainmenuDlg_->OnPhotoBtn(0);
		break;
	case 5:			//大头贴
		break;
	case 6:			//屏保设置
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