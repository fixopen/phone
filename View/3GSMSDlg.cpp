// YHTelDlg.cpp : implementation file
//

#include "stdafx.h"
#include "../multimediaphone.h"
#include "../MultimediaPhoneDlg.h"
#include "3GSMSDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// C3GSMSDlg dialog


C3GSMSDlg::C3GSMSDlg(CWnd* pParent /*=NULL*/)
	: CDialog(C3GSMSDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(C3GSMSDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void C3GSMSDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(C3GSMSDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(C3GSMSDlg, CDialog)
	//{{AFX_MSG_MAP(C3GSMSDlg)
	ON_MESSAGE(WM_CLICKMJPG_TOAPP, OnClickMJPG)
//	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// C3GSMSDlg message handlers
/*
void C3GSMSDlg::OnTimer(UINT nIDEvent)
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

BOOL C3GSMSDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_pSMSListDlg = new C3GSMSListDlg(this);
	m_pSMSListDlg->Create(C3GSMSListDlg::IDD);

	m_pSMSDetailDlg = new C3GSMSDetailDlg(this);
	m_pSMSDetailDlg->Create(C3GSMSDetailDlg::IDD);

	// TODO: Add extra initialization here
	m_MJPGList.Create(L"", WS_VISIBLE|WS_CHILD, CRect(0, 0, 600, 420), this, 10086);
	m_MJPGList.SetCurrentLinkFile(".\\adv\\mjpg\\k1\\ÖÐÎÄ\\3g_¼ÒÍ¥ÁôÑÔ.xml");
	m_MJPGList.SetMJPGRect(CRect(0, 0, 600, 420));
//	SetTimer(1, 1000, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void C3GSMSDlg::OnClickMJPG(WPARAM w, LPARAM l)
{
	CMultimediaPhoneDlg *pMainDlg = ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd));
	switch(w)
	{
	case 1:			//Ð´¶ÌÐÅ
		m_pSMSDetailDlg->initDataBase(SMS_NEW, -1);
		m_pSMSDetailDlg->ShowWindow(SW_SHOW);
		break;
	case 2:			//ÊÕ¼þÏä
		m_pSMSListDlg->m_bChangeMain = FALSE;
		m_pSMSListDlg->initType(RECV_TYPE, SMS_TYPE);
		m_pSMSListDlg->ShowWindow(SW_SHOW);
		break;
	case 3:			//ÁôÑÔÏä
		m_pSMSListDlg->m_bChangeMain = FALSE;
		m_pSMSListDlg->initType(HOME_RECORD_TYPE, HOME_TYPE);
		m_pSMSListDlg->ShowWindow(SW_SHOW);
		break;
	case 4:			//²Ý¸åÏä
		m_pSMSListDlg->m_bChangeMain = FALSE;
		m_pSMSListDlg->initType(DRAFT_TYPE, SMS_TYPE);
		m_pSMSListDlg->ShowWindow(SW_SHOW);
		break;
	case 5:			//·¢ËÍÏä
		m_pSMSListDlg->m_bChangeMain = FALSE;
		m_pSMSListDlg->initType(SEND_TYPE, SMS_TYPE);
		m_pSMSListDlg->ShowWindow(SW_SHOW);
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