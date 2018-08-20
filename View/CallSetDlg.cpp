#include "stdafx.h"
#include "..\multimediaphone.h"
#include "..\MultimediaPhoneDlg.h"
#include "CallSetDlg.h"
#include "../Data/LanguageResource.h"
#include "../Data/SkinStyle.h"
#include "sip.h"
//#include "../Util/ATCommandWarp.h"

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
	ON_MESSAGE(WM_MJPGTOGGLE, OnClickMJPG)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCallSetDlg message handlers
void CCallSetDlg::OnClickMJPG(WPARAM w, LPARAM l)
{
	switch(w)
	{
	case 11:
	case 12:
	case 13:
	case 14:
		m_MJPGList.SetUnitIsDownStatus(w, TRUE);
		m_MJPGList.SetUnitIsShow(w, TRUE);
		break;
	case 21:
	case 22:
		m_MJPGList.SetUnitIsDownStatus(w, TRUE);
		m_MJPGList.SetUnitIsShow(w, TRUE);
		break;
	case 31:
	case 32:
		if(31 == w)
		{
			m_MJPGList.SetUnitIsDownStatus(w+1, FALSE);
			m_MJPGList.SetUnitIsShow(w+1, TRUE);
		}
		else if(32 == w)
		{
			m_MJPGList.SetUnitIsDownStatus(w-1, FALSE);
			m_MJPGList.SetUnitIsShow(w-1, TRUE);
		}
		m_MJPGList.SetUnitIsDownStatus(w, TRUE);
		m_MJPGList.SetUnitIsShow(w, TRUE);
		break;
	case 1000:
		OnButtonFastDialsCancel();
		break;
	case 1001:
		OnButtonFastDialsOk();
		break;
	}
}

BOOL CCallSetDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here	
	m_edtTransferNumber.Create(WS_CHILD|WS_VISIBLE, CRect(196, 97+57, 196+202, 97+38+57), this, IDC_EDIT_TRANSFERNUMBER);
	m_edtTransferNumber.SetLimitText(15);
	m_edtTransferNumber.SetLimitDiagital();

	m_edtPinNumber1.Create(WS_CHILD|WS_VISIBLE, CRect(138, 128+57, 138+212, 128+30+57), this, IDC_EDIT_PINNUMBER);
	m_edtPinNumber1.SetLimitText(15);
	m_edtPinNumber1.SetLimitDiagital();

	m_edtPinNumber2.Create(WS_CHILD|WS_VISIBLE, CRect(138, 164+57, 138+212, 164+30+57), this, IDC_EDIT_PINNUMBER);
	m_edtPinNumber2.SetLimitText(15);
	m_edtPinNumber2.SetLimitDiagital();

	m_MJPGList.Create(L"", WS_VISIBLE|WS_CHILD, CRect(100, 137, 540, 407), this);
	m_MJPGList.SetMJPGRect(CRect(100, 137, 540, 407));

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCallSetDlg::OnButtonFastDialsOk()
{
	extern Util::ATCommandWarp* GetATCommandWarp();
	Util::ATCommandWarp *pATCommanWarp = GetATCommandWarp();
	
	PhoneCallTRans callTrans;
	PhoneCallWaiting callWaiting;
	PhoneCallLimit callLimit;

	callTrans.isNoAswer = m_MJPGList.GetUnitIsDownStatus(11);
	callTrans.isUncondify = m_MJPGList.GetUnitIsDownStatus(12);
	callTrans.isBusy = m_MJPGList.GetUnitIsDownStatus(13);
	callTrans.isNoTel = m_MJPGList.GetUnitIsDownStatus(14);

	callLimit.isCallin = m_MJPGList.GetUnitIsDownStatus(21);
	callLimit.isCallout = m_MJPGList.GetUnitIsDownStatus(21);
	
	callWaiting.isCallWaiting = m_MJPGList.GetUnitIsDownStatus(31);
	
	CString s;
	m_edtTransferNumber.GetWindowText(s);
	callTrans.teccode = ::Util::StringOp::FromCString(s);
	if(callTrans.isNoAswer != m_calltrans.isNoAswer || callTrans.isUncondify != m_calltrans.isUncondify ||\
		callTrans.isBusy != m_calltrans.isBusy ||callTrans.isNoTel != m_calltrans.isNoTel)
	{
		pATCommanWarp->PhoneSetTrans(callTrans);
	}
	if(callWaiting.isCallWaiting != m_callwaiting.isCallWaiting)
	{
		pATCommanWarp->PhoneSetCallWaiting(callWaiting.isCallWaiting);
	}
	if(callLimit.isCallin != m_calllimit.isCallin || callLimit.isCallout != m_calllimit.isCallout)
	{
		pATCommanWarp->PhoneSetCallLimit(callLimit);
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

void CCallSetDlg::SetCallSetParam()
{
	extern Util::ATCommandWarp* GetATCommandWarp();
	Util::ATCommandWarp *pATCommanWarp = GetATCommandWarp();

	memset(&m_calltrans, 0, sizeof(PhoneCallTRans));
	memset(&m_calllimit, 0, sizeof(PhoneCallLimit));
	memset(&m_callwaiting, 0, sizeof(PhoneCallWaiting));

//	pATCommanWarp->PhoneGetTrans(m_calltrans);
	pATCommanWarp->PhoneGetCallLimit(m_calllimit);
	int nCallWaiting; 
	pATCommanWarp->PhoneGetCallWaiting(nCallWaiting);
	m_callwaiting.isCallWaiting = (bool)nCallWaiting;

	if(m_calltrans.isNoAswer)
	{
		m_MJPGList.SetUnitIsDownStatus(11, TRUE);
	}
	else
	{
		m_MJPGList.SetUnitIsDownStatus(11, FALSE);
	}
	if(m_calltrans.isUncondify)
	{
		m_MJPGList.SetUnitIsDownStatus(12, TRUE);
	}
	else
	{
		m_MJPGList.SetUnitIsDownStatus(12, FALSE);
	}
	if(m_calltrans.isBusy)
	{
		m_MJPGList.SetUnitIsDownStatus(13, TRUE);
	}
	else
	{
		m_MJPGList.SetUnitIsDownStatus(13, FALSE);
	}
	if(m_calltrans.isNoTel)
	{
		m_MJPGList.SetUnitIsDownStatus(14, TRUE);
	}
	else
	{
		m_MJPGList.SetUnitIsDownStatus(14, FALSE);
	}
	m_edtTransferNumber.SetWindowText(::Util::StringOp::ToCString(m_calltrans.teccode));

	if(m_calllimit.isCallin)
	{
		m_MJPGList.SetUnitIsDownStatus(21, TRUE);
	}
	else
	{
		m_MJPGList.SetUnitIsDownStatus(21, FALSE);
	}
	if(m_calllimit.isCallout)
	{
		m_MJPGList.SetUnitIsDownStatus(22, TRUE);
	}
	else
	{
		m_MJPGList.SetUnitIsDownStatus(22, FALSE);
	}
	
	if(m_callwaiting.isCallWaiting)
	{
		m_MJPGList.SetUnitIsDownStatus(31, TRUE);
		m_MJPGList.SetUnitIsDownStatus(32, FALSE);
	}
	else
	{
		m_MJPGList.SetUnitIsDownStatus(31, FALSE);
		m_MJPGList.SetUnitIsDownStatus(32, TRUE);
	}
}

void CCallSetDlg::ShowItemsControl()
{
	m_edtTransferNumber.ShowWindow(FALSE);
	m_edtPinNumber1.ShowWindow(FALSE);
	m_edtPinNumber2.ShowWindow(FALSE);

	switch(m_uiType)
	{
	case 1:
		m_edtTransferNumber.ShowWindow(TRUE);
		break;
	case 4:
//		m_edtPinNumber1.ShowWindow(TRUE);
//		m_edtPinNumber2.ShowWindow(TRUE);
		break;
	}
}
