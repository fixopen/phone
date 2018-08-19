// deletetipdlg.cpp : implementation file
//

#include "stdafx.h"
#include "..\multimediaphone.h"
#include "..\MultimediaPhoneDlg.h"
#include "deletetipdlg.h"
#include "../Data/LanguageResource.h"
#include "../Data/SkinStyle.h"
#include "sip.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern BOOL g_bStartring;

/////////////////////////////////////////////////////////////////////////////
// CDeleteTipDlg dialog


CDeleteTipDlg::CDeleteTipDlg(CWnd* pParent /*=NULL*/)
	: CCEDialog(CDeleteTipDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDeleteTipDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_ntype = 0;
}


void CDeleteTipDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDeleteTipDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDeleteTipDlg, CCEDialog)
	//{{AFX_MSG_MAP(CDeleteTipDlg)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_DELETETIP_OK, OnButtonDeleteTipOk)
	ON_BN_CLICKED(IDC_BUTTON_DELETETIP_CANCEL, OnButtonDeleteTipCancel)
	ON_MESSAGE(WM_CLICKMJPG_TOAPP, OnClickMJPG)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDeleteTipDlg message handlers
void CDeleteTipDlg::OnClickMJPG(WPARAM w, LPARAM l)
{
	switch(w)
	{
	case 1001:
		OnButtonDeleteTipOk();
		break;
	case 1000:
		OnButtonDeleteTipCancel();
		break;
	default:
		break;
	}
}
BOOL CDeleteTipDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_procbarSound.Create(WS_CHILD, CRect(190+20 , 105+202, 190+20 + 400, 105+202 + 32), this, 0xFF10);

	m_MJPGList.Create(L"", WS_VISIBLE|WS_CHILD, CRect(190, 105, 190+440,105+270), this);
	m_MJPGList.SetCurrentLinkFile(".\\adv\\mjpg\\k5\\中文\\删除确认.xml");
	m_MJPGList.SetMJPGRect(CRect(190, 105, 190+440, 105+270));

//	MoveWindow(180,105,440,270);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDeleteTipDlg::OnButtonDeleteTipOk()
{	

	if (m_iMaxPos)
	{	
		m_MJPGList.SetUnitIsShow(1001,false,true);
		m_procbarSound.ShowWindow(true);
	}
	else
	{
		ShowWindow_(SW_HIDE);
		((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->IsSendMessage(TRUE);
	}
	
	if ( 0 == m_ntype)
	{
		if (m_handle)
		{
			::PostMessage(m_handle, WM_DELETESELITEM, 0, 0);
			if(g_bStartring)
			{
				::PostMessage(m_handle, WM_RINGTIME, 0, 0);
			}
		}
	}
	else if ( 1 == m_ntype)
	{
		if (m_handle)
		{
			::PostMessage(m_handle, WM_SEND_MMS, m_ntype, 0);
		}
		
	}
	else if ( 2 == m_ntype || 3 == m_ntype)
	{
		if (m_handle)
		{
			::PostMessage(m_handle, WM_DELETESELITEM, m_ntype, 0);
		}
	}
	else if ( 4 == m_ntype)//用于名片的群组发送
	{
		if (m_handle)
		{
			::PostMessage(m_handle, WM_GROUP_SEND, 0, 0);
		}
	}

	KillTimer(100);
	m_ntype = 0 ;
	m_handle = NULL;

}

void CDeleteTipDlg::OnButtonDeleteTipCancel()
{	
	ShowWindow_(SW_HIDE);
	KillTimer(100);
	((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->IsSendMessage(TRUE);

	if ( 1 == m_ntype)
	{
		if (m_handle)
		{
			::PostMessage(m_handle, WM_SEND_MMS, m_ntype, 1);
		}
		
	}

	m_handle = NULL;
	m_ntype  = 0 ;
}

void CDeleteTipDlg::SetHWnd(HWND handle)
{
	m_handle = handle;
}

void CDeleteTipDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	
	if(nIDEvent == 100)
	{
		OnButtonDeleteTipCancel();
	}
	else if ( 1 == nIDEvent)
	{
		KillTimer(1);
		m_procbarSound.ShowWindow(SW_HIDE);
		ShowWindow_(SW_HIDE);
	}
	else 
	{
		KillTimer(nIDEvent);
	}

	CCEDialog::OnTimer(nIDEvent);
}

void CDeleteTipDlg::SetPasswordModel(bool b)
{

}

void CDeleteTipDlg::SetPassword(CString password)
{
	//	m_sPassword = password;
}

void CDeleteTipDlg::SetDelTip(CString tips)
{
	//m_MJPGList.SetUnitText(100, "", TRUE);
	//m_MJPGList.SetUnitBitmap(100, tips, "", TRUE);
	//m_sticTip.SetWindowText(tips);
}

void CDeleteTipDlg::SetTitle(CString title,int isTime,int type,TEXTSIZE font)
{
	m_iMaxPos = 0	 ;
	m_ntype   = type ;
	m_MJPGList.SetUnitFont(1,font);
	m_MJPGList.SetUnitText(1,title,true);
	
	if(isTime > 0)	
	{
		SetTimer(100, isTime, NULL);
	}
	
	m_MJPGList.SetUnitIsShow(1001,true,true);
	m_procbarSound.ShowWindow(SW_HIDE);
}

void CDeleteTipDlg::SetProcessMax(int max )
{
	m_procbarSound.SetParam(0,0,max,1);
	m_procbarSound.SetPos(0);
	m_iMaxPos = max;
}

void CDeleteTipDlg::SetProcessPos(int npos)
{
	m_procbarSound.SetPos(npos);
	if (npos == m_iMaxPos)
	{
		SetTimer(1,100,NULL);
		m_iMaxPos = 0;
	}
}