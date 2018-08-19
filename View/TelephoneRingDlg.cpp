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
		
	default:
		break;
	}
}

void CTelephoneRingDlg::HangOff_()
{	
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd ;
	main->PopbackIcon();

	
	ShowWindow_(SW_HIDE);
	main->m_pTelephoneDlg->HandleOff();
}

void CTelephoneRingDlg::OnTimer(UINT nIDEvent) 
{
	CCEDialog::OnTimer(nIDEvent);
}

void CTelephoneRingDlg::ShowContact(boost::shared_ptr<Data::Contact> contact, std::string number)
{
	CString str = CString(contact->name().c_str());
	str += _T("(");
	str += CString(number.c_str());
	str += _T(")");
	
	m_MJPGList.SetUnitText(100, str, TRUE);
	m_MJPGList.SetUnitText(101, CString(contact->company().c_str()), TRUE);
	m_MJPGList.SetUnitText(102, CString(contact->department().c_str()), TRUE);
	m_MJPGList.SetUnitText(103, CString(contact->duty().c_str()), TRUE);

}

void CTelephoneRingDlg::HandleOn()
{
	::PostMessage(theApp.m_pMainWnd->m_hWnd, WM_TEL_HUNGON, 0, 0);	
}