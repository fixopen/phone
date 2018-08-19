// fastdialsdlg.cpp : implementation file
//

#include "stdafx.h"
#include "..\multimediaphone.h"
#include "..\MultimediaPhoneDlg.h"
#include "CallSetDlg.h"
#include "../Data/LanguageResource.h"
#include "../Data/SkinStyle.h"
#include "sip.h"
#include "../Util/ATCommandWarp.h"

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
LRESULT CCallSetDlg::OnClickMJPG(WPARAM w, LPARAM l)
{
    LRESULT result = 0;
	switch(w)
	{
	case 100:
		OnButtonFastDialsOk();
		break;
	case 101:
		OnButtonFastDialsCancel();
		break;
	}
    return result;
}

void CCallSetDlg::SetButtonDefaultColor(CCEButtonST* button)
{
	button->SetColor(CCEButtonST::BTNST_COLOR_BK_IN, RGB(206,206,206));
	button->SetColor(CCEButtonST::BTNST_COLOR_BK_OUT, RGB(206,206,206));
	button->SetColor(CCEButtonST::BTNST_COLOR_BK_FOCUS, RGB(206,206,206));
}

BOOL CCallSetDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_chbCallToCall.Create(L"", WS_CHILD|WS_VISIBLE|BS_CHECKBOX, CRect(54+68, 62+74, 54+68+35, 62+74+32), this, IDC_CHECK_SETTING_AUTOLEAVEWORD);
	m_chbCallToCall.SetIcon(IDI_ICON_CHECK1, CSize(32, 32), IDI_ICON_CHECK0, CSize(32, 32));
	SetButtonDefaultColor(&m_chbCallToCall);	
	
	m_chbNoAnswer.Create(L"", WS_CHILD|WS_VISIBLE|BS_CHECKBOX, CRect(54+202, 62+74, 54+202+35, 62+74+32), this, IDC_CHECK_SETTING_AUTOLEAVEWORD);
	m_chbNoAnswer.SetIcon(IDI_ICON_CHECK1, CSize(32, 32), IDI_ICON_CHECK0, CSize(32, 32));
	SetButtonDefaultColor(&m_chbNoAnswer);	

	m_chbUnCondifie.Create(L"", WS_CHILD|WS_VISIBLE|BS_CHECKBOX, CRect(54+366, 62+74, 54+366+35, 62+74+32), this, IDC_CHECK_SETTING_AUTOLEAVEWORD);
	m_chbUnCondifie.SetIcon(IDI_ICON_CHECK1, CSize(32, 32), IDI_ICON_CHECK0, CSize(32, 32));
	SetButtonDefaultColor(&m_chbUnCondifie);	

	m_chbIsTeling.Create(L"", WS_CHILD|WS_VISIBLE|BS_CHECKBOX, CRect(54+202, 62+112, 54+202+35, 62+112+32), this, IDC_CHECK_SETTING_AUTOLEAVEWORD);
	m_chbIsTeling.SetIcon(IDI_ICON_CHECK1, CSize(32, 32), IDI_ICON_CHECK0, CSize(32, 32));
	SetButtonDefaultColor(&m_chbIsTeling);	

	m_chbIsNoTeling.Create(L"", WS_CHILD|WS_VISIBLE|BS_CHECKBOX, CRect(54+366, 62+112, 54+366+35, 62+112+32), this, IDC_CHECK_SETTING_AUTOLEAVEWORD);
	m_chbIsNoTeling.SetIcon(IDI_ICON_CHECK1, CSize(32, 32), IDI_ICON_CHECK0, CSize(32, 32));
	SetButtonDefaultColor(&m_chbIsNoTeling);	

	m_edtTelCode.Create(WS_CHILD|WS_VISIBLE, CRect(54+203, 62+148, 54+203+280, 62+148+33), this, IDC_EDIT_SETTING_AREANO);
	m_edtTelCode.SetLimitText(15);
	m_edtTelCode.SetLimitDiagital();

	m_chbCallToLimit.Create(L"", WS_CHILD|WS_VISIBLE|BS_CHECKBOX, CRect(54+68, 62+189, 54+68+35, 62+189+32), this, IDC_CHECK_SETTING_AUTOLEAVEWORD);
	m_chbCallToLimit.SetIcon(IDI_ICON_CHECK1, CSize(32, 32), IDI_ICON_CHECK0, CSize(32, 32));
	SetButtonDefaultColor(&m_chbCallToLimit);	

	m_chbLimitCallIn.Create(L"", WS_CHILD|WS_VISIBLE|BS_CHECKBOX, CRect(54+202, 62+189, 54+202+35, 62+189+32), this, IDC_CHECK_SETTING_AUTOLEAVEWORD);
	m_chbLimitCallIn.SetIcon(IDI_ICON_CHECK1, CSize(32, 32), IDI_ICON_CHECK0, CSize(32, 32));
	SetButtonDefaultColor(&m_chbLimitCallIn);	

	m_chbLimitCallOut.Create(L"", WS_CHILD|WS_VISIBLE|BS_CHECKBOX, CRect(54+366, 62+189, 54+366+35, 62+189+32), this, IDC_CHECK_SETTING_AUTOLEAVEWORD);
	m_chbLimitCallOut.SetIcon(IDI_ICON_CHECK1, CSize(32, 32), IDI_ICON_CHECK0, CSize(32, 32));
	SetButtonDefaultColor(&m_chbLimitCallOut);	

	m_chbCallToWaiting.Create(L"", WS_CHILD|WS_VISIBLE|BS_CHECKBOX, CRect(54+68, 62+228, 54+68+35, 62+228+32), this, IDC_CHECK_SETTING_AUTOLEAVEWORD);
	m_chbCallToWaiting.SetIcon(IDI_ICON_CHECK1, CSize(32, 32), IDI_ICON_CHECK0, CSize(32, 32));
	SetButtonDefaultColor(&m_chbCallToWaiting);	

	m_rdoIsYes.Create(L"", WS_CHILD|WS_VISIBLE/*|BS_RADIOBUTTON*/, CRect(54+201, 62+227, 54+201+32, 62+227+34), this, IDC_RADIO_SETTINT_CONNECTADSL);
	m_rdoIsYes.SetColor(RGB(0, 0, 0), RGB(206, 206, 206));//Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
	
	m_rdoIsNo.Create(L"", WS_CHILD|WS_VISIBLE/*|BS_RADIOBUTTON*/, CRect(54+362, 62+227, 54+362+32, 62+227+34), this, IDC_RADIO_SETTINT_CONNECTADSL);
	m_rdoIsNo.SetColor(RGB(0, 0, 0), RGB(206, 206, 206));//Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
	
	CButton *pButton[2];
	pButton[0] = &m_rdoIsYes;
	pButton[1] = &m_rdoIsNo;
	
	m_rdoIsYes.SetGroupButton(pButton, 2);
	m_rdoIsNo.SetGroupButton(pButton, 2);


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
	PhoneCallTRans calltrans;
	Util::ATCommandWarp::PhoneGetTrans(calltrans);
}
