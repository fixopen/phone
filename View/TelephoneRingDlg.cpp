// TelephoneDialDlg.cpp : implementation file
//

#include "stdafx.h"
#include "..\multimediaphone.h"
#include "../MultimediaPhoneDlg.h"
#include "TelephoneRingDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTelephoneDialDlg dialog


CTelephoneRingDlg::CTelephoneRingDlg(CWnd* pParent /*=NULL*/)
	: CCEDialog(CTelephoneRingDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTelephoneDialDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

void CTelephoneRingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTelephoneDialDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTelephoneRingDlg, CDialog)
	//{{AFX_MSG_MAP(CTelephoneDialDlg)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_CLICKMJPG_TOAPP, OnClickMJPG)

END_MESSAGE_MAP()

BOOL CTelephoneRingDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_MJPGList.Create(L"", WS_VISIBLE|WS_CHILD, CRect(0, 0, 800, 423), this);
	m_MJPGList.SetCurrentLinkFile(".\\adv\\mjpg\\k5\\中文\\电话接听.xml");
	m_MJPGList.SetMJPGRect(CRect(0, 0, 800, 423));
	
	HideContact(100,false);
	HideContact(200,false);

	MoveWindow(0,57,800,423);

	return TRUE ;

}

void CTelephoneRingDlg::OnClickMJPG(WPARAM w, LPARAM l)
{
	CMultimediaPhoneDlg *pMainDlg = ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd));
	switch (w)
	{	
	case 1://语音接听
		HandleOn();
		break;

	case 2://可视接听
		ShowWindow_(SW_HIDE);
		break;

	case 3://拒接
		HangOff_();
		break;
		
	case 4://切换
		
		break;

	default:
		break;
	}

}

void CTelephoneRingDlg::HangOff_()
{	
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd ;
	
	if (main->m_phoneLine[0].pFSM->getCurrentState() == CMultimediaPhoneDlg::p3gsRing 
		&& main->m_phoneLine[2].pFSM->getCurrentState() == CMultimediaPhoneDlg::pstnsRing)
	{
		if ( m_MJPGList.GetUnitIsShow(100))//3g
		{	
			::PostMessage(main->m_hWnd, WM_TEL_HUNGOFF, 1, 0);
			//main->phone_->Hangup();

		}
		else//pstn
		{	
			::PostMessage(main->m_hWnd, WM_TEL_HUNGOFF, 3, 0);
			//main->phone_->PSTNHangoff_();
		}	
	}
	else
	{
		main->PopbackIcon();	
		ShowWindow_(SW_HIDE);
		main->m_pTelephoneDlg->HandleOff();
	}

}


void CTelephoneRingDlg::OnTimer(UINT nIDEvent) 
{
	CCEDialog::OnTimer(nIDEvent);
}

void CTelephoneRingDlg::ShowContact(boost::shared_ptr<Data::Contact> contact, std::string number,int unitNo)
{	

	m_MJPGList.SetUnitText(unitNo, CString(contact->name().c_str()), TRUE);	
	m_MJPGList.SetUnitText(unitNo+3, CString(contact->company().c_str()), TRUE);
	m_MJPGList.SetUnitText(unitNo+4, CString(contact->department().c_str()), TRUE);
	m_MJPGList.SetUnitText(unitNo+5, CString(contact->email().c_str()), TRUE);
	m_MJPGList.SetUnitText(unitNo+6, CString(contact->duty().c_str()), TRUE);
	m_MJPGList.SetUnitText(unitNo+7, CString(contact->memo().c_str()), TRUE);

}

void CTelephoneRingDlg::HideContact(int uintNo,bool bshow )
{
	for (int i = 0 ; i < 8 ; i++)
	{
		m_MJPGList.SetUnitIsShow(uintNo+i,!bshow,true);		
	}
}

void CTelephoneRingDlg::Switch()
{	
	if ( m_MJPGList.GetUnitIsShow(100) )
	{	
		HideContact(100,true);
		HideContact(200,false);
	}
	else
	{	
		HideContact(100,false);
		HideContact(200,true);
	}

}

void CTelephoneRingDlg::HandleOn()
{	
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	extern void GMute(BOOL isOn);
	GMute(FALSE);//打开speeker
	main->phone_->Free(Telephone::TelephoneWarp::FreeOn);//按过免提
	if(main->m_nTELRigster >= TELRIGSTER_TD)
	Telephone::TelephoneWarp::GetTelephoneWarp()->HandFree(true);//切换到免提

	if ( m_MJPGList.GetUnitIsShow(100) )
	{	
		::PostMessage(theApp.m_pMainWnd->m_hWnd, WM_TEL_HUNGON, 1, 0);
		main->phone_->HungOn(TRUE);
	}
	else
	{
		main->phone_->PSTNHangOn();	
	}

}

void CTelephoneRingDlg::ClearData(int unitNo)
{	
	for (int i = 0 ; i < 8;i++)
	{
		m_MJPGList.SetUnitText(unitNo+i,L"", TRUE);	
	}

}

void CTelephoneRingDlg::InitData()
{
	m_MJPGList.SetUnitIsShow(4,false,true);
	m_MJPGList.SetUnitIsShow(5,false,true);

}