// fastdialsdlg.cpp : implementation file
//

#include "stdafx.h"
#include "..\multimediaphone.h"
#include "PinSetDlg.h"
#include "../Data/LanguageResource.h"
#include "../Data/SkinStyle.h"
#include "../Util/ATCommandWarp.h"
#include "sip.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPinSetDlg dialog


CPinSetDlg::CPinSetDlg(CWnd* pParent /*=NULL*/)
	: CCEDialog(CPinSetDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPinSetDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CPinSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPinSetDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPinSetDlg, CCEDialog)
	//{{AFX_MSG_MAP(CPinSetDlg)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_FASTDIALS_OK, OnButtonFastDialsOk)
	ON_BN_CLICKED(IDC_BUTTON_FASTDIALS_CANCEL, OnButtonFastDialsCancel)
	ON_MESSAGE(WM_CLICKMJPG_TOAPP, OnClickMJPG)
	ON_MESSAGE(WM_CHECKPASSWORD, OnCheckPIN)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPinSetDlg message handlers
LRESULT CPinSetDlg::OnClickMJPG(WPARAM w, LPARAM l)
{
    LRESULT result = 0;
	switch(w)
	{
	case 1:
		OnButtonFastDialsOk();
		break;
	case 2:
		OnButtonFastDialsCancel();
		break;
	case 3:
		{
			if(m_bPin)
			{
				m_pPasswordDlg->SetType(SETTINGPIN_PASSWORD);
				m_pPasswordDlg->SetHWnd(this->m_hWnd);
				m_pPasswordDlg->ShowWindow_(SW_SHOW);
			}
		
		}
		break;
	}
    return result;
}

LRESULT CPinSetDlg::OnCheckPIN(WPARAM w, LPARAM l)
{
    LRESULT result = 0;
	SipShowIM(SIPF_OFF);
	ShowWindow_(FALSE);
	SipShowIM(SIPF_OFF);
    return result;
}

void CPinSetDlg::SetButtonDefaultColor(CCEButtonST* button)
{
	button->SetColor(CCEButtonST::BTNST_COLOR_BK_IN, RGB(206,206,206));
	button->SetColor(CCEButtonST::BTNST_COLOR_BK_OUT, RGB(206,206,206));
	button->SetColor(CCEButtonST::BTNST_COLOR_BK_FOCUS, RGB(206,206,206));
}

BOOL CPinSetDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_chbPIN.Create(L"", WS_CHILD|WS_VISIBLE|BS_CHECKBOX, CRect(54+67, 62+74, 54+67+35, 62+74+32), this, IDC_CHECK_SETTING_AUTOLEAVEWORD);
	m_chbPIN.SetIcon(IDI_ICON_CHECK1, CSize(32, 32), IDI_ICON_CHECK0, CSize(32, 32));
	SetButtonDefaultColor(&m_chbPIN);	

	m_MJPGList.Create(L"", WS_VISIBLE|WS_CHILD, CRect(54, 62, 746, 358), this);
	m_MJPGList.SetCurrentLinkFile(".\\adv\\mjpg\\k1\\ÖÐÎÄ\\PINÉèÖÃ.xml");
	m_MJPGList.SetMJPGRect(CRect(54, 62, 746, 358));

	m_pPasswordDlg = new CPasswordDlg(this);
	m_pPasswordDlg->Create(CPasswordDlg::IDD);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPinSetDlg::OnButtonFastDialsOk()
{
	if(m_Data)
	{
	
	}

	BOOL flag = m_chbPIN.GetCheck();
	if(flag == m_bPin)
	{
		SipShowIM(SIPF_OFF);
		
		ShowWindow_(FALSE);
		SipShowIM(SIPF_OFF);
	}
	else
	{
		if(flag)
			m_pPasswordDlg->SetType(CHECK_LOCKPINPASSWORD);
		else
			m_pPasswordDlg->SetType(CHECK_UNLOCKPINPASSWORD);
		m_pPasswordDlg->SetHWnd(this->m_hWnd);
		m_pPasswordDlg->ShowWindow_(SW_SHOW);
	}	
}

void CPinSetDlg::OnButtonFastDialsCancel()
{
	SipShowIM(SIPF_OFF);
	ShowWindow_(FALSE);
	SipShowIM(SIPF_OFF);
}

void CPinSetDlg::SetCallSetParam(boost::shared_ptr<Data::Setting> data)
{
	m_Data = data;
	int nStatus;
	
	extern Util::ATCommandWarp* GetATCommandWarp();
	Util::ATCommandWarp *pATCommanWarp = GetATCommandWarp();
	pATCommanWarp->GetSimStatus(nStatus);
	m_bPin = (BOOL)nStatus;

	m_chbPIN.SetCheck(m_bPin);
}
