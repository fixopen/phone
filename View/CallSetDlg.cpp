// fastdialsdlg.cpp : implementation file
//

#include "stdafx.h"
#include "..\multimediaphone.h"
#include "CallSetDlg.h"
#include "../Data/LanguageResource.h"
#include "../Data/SkinStyle.h"
#include "sip.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCallSetDlg dialog


CCallSetDlg::CCallSetDlg(CWnd* pParent /*=NULL*/)
	: CCEDialog(CCallSetDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCallSetDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_Data = NULL;
}


void CCallSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCallSetDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCallSetDlg, CCEDialog)
	//{{AFX_MSG_MAP(CCallSetDlg)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_FASTDIALS_OK, OnButtonFastDialsOk)
	ON_BN_CLICKED(IDC_BUTTON_FASTDIALS_CANCEL, OnButtonFastDialsCancel)
	ON_MESSAGE(WM_CLICKMJPG_TOAPP, OnClickMJPG)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCallSetDlg message handlers
void CCallSetDlg::OnClickMJPG(WPARAM w, LPARAM l)
{
	switch(w)
	{
	case 100:
		OnButtonFastDialsOk();
		break;
	case 101:
		OnButtonFastDialsCancel();
		break;
	}
}

BOOL CCallSetDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	m_MJPGList.Create(L"", WS_VISIBLE|WS_CHILD, CRect(54, 62, 746, 358), this);
	m_MJPGList.SetCurrentLinkFile(".\\adv\\mjpg\\k1\\÷–Œƒ\\∫ÙΩ–…Ë÷√.xml");
	m_MJPGList.SetMJPGRect(CRect(54, 62, 746, 358));

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCallSetDlg::OnButtonFastDialsOk()
{
	if(m_Data)
	{
	
	}
	
	SipShowIM(SIPF_OFF);

	ShowWindow_(FALSE);
	SipShowIM(SIPF_OFF);
}

void CCallSetDlg::OnButtonFastDialsCancel()
{
	SipShowIM(SIPF_OFF);
	ShowWindow_(FALSE);
	SipShowIM(SIPF_OFF);
}

void CCallSetDlg::SetCallSetParam(boost::shared_ptr<Data::Setting> data)
{
	m_Data = data;
	
}
