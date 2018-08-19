// deletetipdlg.cpp : implementation file
//

#include "stdafx.h"
#include "../multimediaphone.h"
#include "../MultimediaPhoneDlg.h"
#include "CTelePhoneOptionDlg.h"
#include "../Util/stringOp.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CDeleteTipDlg dialog


CTelePhoneOptionDlg::CTelePhoneOptionDlg(CWnd* pParent /*=NULL*/)
	: CCEDialog(CTelePhoneOptionDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDeleteTipDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	type = 0;
}


void CTelePhoneOptionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDeleteTipDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTelePhoneOptionDlg, CCEDialog)
	ON_MESSAGE(WM_CLICKMJPG_TOAPP, OnClickMJPG)
	ON_MESSAGE(WM_MJPGTOGGLE,OnClickMJPG)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDeleteTipDlg message handlers
void CTelePhoneOptionDlg::OnClickMJPG(WPARAM w, LPARAM l)
{
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	switch(w)
	{
	case 0: //保留
		main->m_pATCommandWarp1->CHLD(2);
		break;

	case 1: //恢复
		main->m_pATCommandWarp1->CHLD(2);
		break;

	case 2: //建立新的通话
		main->m_pTelphoneDialDlg->SubDialShow();
		break;
	
	case 10: //3G之间切换
		main->m_pATCommandWarp1->CHLD(2);
	//	main->m_nline = (main->m_nline + 1) %2 ;//3G_1 和 3G_2切换
	//	main->m_pTelephoneDlg->ShowMainInfo();
		break;

	case 11: //建立会议
		main->m_pATCommandWarp1->CHLD(3);
		break;
	
	case 12: //激活所有
		main->m_pATCommandWarp1->CHLD(2);
		break;

	case 20: //私下交谈
		{	
			CString toholdline;
			toholdline.Format(L"2%d",(main->m_nline == 0)?1:2);
			main->m_pATCommandWarp1->CHLD(atoi(toholdline));	
		}
		break;
		
	case 21: //保持
		{	
			CString toholdline;
			toholdline.Format(L"2%d",(main->m_nline == 0)?2:1);
			main->m_pATCommandWarp1->CHLD(atoi(toholdline));	
		}
		break;
		
	case 22: //保持所有
		main->m_pATCommandWarp1->CHLD(2);
		break;

	case 23: //结束所有
		main->m_pTelephoneDlg->HandleOff();
		break;

	case 1000:
		ShowWindow_(SW_HIDE);
		break;
	default:
		break;
	}
	ShowWindow_(SW_HIDE);

}
BOOL CTelePhoneOptionDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_MJPGList.Create(L"", WS_VISIBLE|WS_CHILD, CRect(241, 52, 241+317,52+375), this);
	m_MJPGList.SetCurrentLinkFile(".\\adv\\mjpg\\k5\\中文\\电话选项.xml");
	m_MJPGList.SetMJPGRect(CRect(241, 52, 241+317, 52+375));
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CTelePhoneOptionDlg::SetMainWnd(CWnd *pwnd)
{
	m_pwnd = pwnd;
}

void CTelePhoneOptionDlg::ShowOption(phonestate ps)
{	
	m_phonestate = ps ;

	for (int i = 0 ; i < 3 ;i++)//保留(恢复)，新建通话
	{
		m_MJPGList.SetUnitIsShow(i,false);	
	}

	for (i = 10 ; i < 13 ;i++)//切换，建立会议
	{
		m_MJPGList.SetUnitIsShow(i,false);	
	}

	for (i = 20 ; i < 24 ;i++)//私下交谈，保持，保持所有，结束所有
	{
		m_MJPGList.SetUnitIsShow(i,false);	
	}

	switch (ps)
	{
		case onePhoneActive:
		{	
			m_MJPGList.SetUnitIsShow(0,true);
			m_MJPGList.SetUnitIsShow(2,true);
		}
		break;

		case onePhoneHold:
		{
			m_MJPGList.SetUnitIsShow(1,true);
			m_MJPGList.SetUnitIsShow(2,true);
		}
		break;
		
		case twoPhoneHoldAndActive:
		{	
			m_MJPGList.SetUnitIsShow(10,true);
			m_MJPGList.SetUnitIsShow(11,true);	
		}
		break;
		
		case twoPhoneHoldAndHold:
		{	
			m_MJPGList.SetUnitIsShow(12,true);
		}
		break;
		
		case threePhone:
		{
			m_MJPGList.SetUnitIsShow(20,true);
			m_MJPGList.SetUnitIsShow(21,true);	
			m_MJPGList.SetUnitIsShow(22,true);
			m_MJPGList.SetUnitIsShow(23,true);	
		}
		break;
		
		case pstnAnd3g://pstn跟3G切换
		{
		}
		break;

	}
	m_MJPGList.Invalidate();
	ShowWindow_(SW_SHOW);
}





