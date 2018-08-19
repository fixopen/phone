// deletetipdlg.cpp : implementation file
//

#include "stdafx.h"
#include "../multimediaphone.h"
#include "../MultimediaPhoneDlg.h"
#include "10PhrasesDlg.h"
#include "sip.h"
#include "../Util/stringOp.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CDeleteTipDlg dialog


C10PhrasesDlg::C10PhrasesDlg(CWnd* pParent /*=NULL*/)
	: CCEDialog(C10PhrasesDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDeleteTipDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	type = 0;
	m_iUnitNo = -1;
}


void C10PhrasesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDeleteTipDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(C10PhrasesDlg, CCEDialog)

	ON_MESSAGE(WM_CLICKMJPG_TOAPP, OnClickMJPG)
	ON_MESSAGE(WM_MJPGTOGGLE,OnClickMJPG)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDeleteTipDlg message handlers
void C10PhrasesDlg::OnClickMJPG(WPARAM w, LPARAM l)
{
	switch(w)
	{
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
	case 7:
	case 8:
	case 9:
	case 10:
		ClickOneItem(w);
		break;
	case 1000:
		ShowWindow_(SW_HIDE);
		break;

	case 1001:
		OnBtnOK();
		break;

	default:
		break;
	}
}
BOOL C10PhrasesDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_MJPGList.Create(L"", WS_VISIBLE|WS_CHILD, CRect(241, 52, 241+317,52+375), this);
	m_MJPGList.SetCurrentLinkFile(".\\adv\\mjpg\\k5\\中文\\常用短语.xml");
	m_MJPGList.SetMJPGRect(CRect(241, 52, 241+317, 52+375));
	
	AddPhrases();
//	MoveWindow(180,105,440,270);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void C10PhrasesDlg::SetMainWnd(CWnd *pwnd)
{
	m_pwnd = pwnd;
}

void C10PhrasesDlg::ClickOneItem(int item)
{	
	if (m_iUnitNo > -1)
	{
		m_MJPGList.SetUnitIsDownStatus(m_iUnitNo,false);
		m_MJPGList.SetUnitIsShow(m_iUnitNo,true,true);
	}

	m_MJPGList.SetUnitIsDownStatus(item,true);
	m_MJPGList.SetUnitIsShow(item,true,true);
	m_iUnitNo = item ;
}

void C10PhrasesDlg::OnBtnOK()
{
	//设置号码
	if (m_iUnitNo > -1)
	{
		CString tel = m_MJPGList.GetUnitText(m_iUnitNo);
		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pMainDlg->m_p3GSMSDlg->InsertPhrases(tel);
		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pMainDlg->m_p3GSMSDlg->ShowWindow(SW_SHOW);
		ShowWindow_(SW_HIDE);
	}
	
}

void C10PhrasesDlg::InitialData()
{	
	m_MJPGList.SetUnitIsDownStatus(m_iUnitNo,false);
	m_MJPGList.SetUnitIsShow(m_iUnitNo,true,true);
	m_iUnitNo = -1;
}

void C10PhrasesDlg::InsertPhrases2SMS()
{
	CMultimediaPhoneDlg*main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	if (m_iUnitNo > -1)
	{
		CString phrase = m_MJPGList.GetUnitText(m_iUnitNo);
		main->m_pMainDlg->m_p3GSMSDlg->InsertPhrases(phrase);
	}
}
void C10PhrasesDlg::AddPhrases()
{
	CString phrases[10] = {	L"会议已被取消。",							
							L"节日快乐!"	,
							L"老地方见。",
							L"请打办公室电话。",
							L"请打我家里电话。",
							L"请回电。",
							L"晚些到，抱歉",
							L"我尽力而为。",
							L"我XX点到达。",
							L"正在开会，稍后打。"
							};

	 for (int i = 0 ; i < 10 ; i++)
	 {
		m_MJPGList.SetUnitText(i+1,phrases[i],false);
	 }

	m_MJPGList.Invalidate();

}



