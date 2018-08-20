// deletetipdlg.cpp : implementation file
//

#include "stdafx.h"
#include "..\multimediaphone.h"
#include "deletetipdlg.h"
#include "../Data/LanguageResource.h"
#include "../Data/SkinStyle.h"
#include "sip.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CDeleteTipDlg dialog


CDeleteTipDlg::CDeleteTipDlg(CWnd* pParent /*=NULL*/)
	: CCEDialog(CDeleteTipDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDeleteTipDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
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
	case 1:
		OnButtonDeleteTipOk();
		break;
	case 2:
		OnButtonDeleteTipCancel();
		break;
	}
}
BOOL CDeleteTipDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	m_edtNewPassword1.Create(WS_CHILD|ES_PASSWORD, CRect(54 + 267, 62 + 148, 54 + 522, 62 + 188), this, IDC_SETTING_PASSWORD2);
	m_edtNewPassword1.SetLimitText(6);

	m_MJPGList.Create(L"", WS_VISIBLE|WS_CHILD, CRect(54, 62, 746, 358), this);
	m_MJPGList.SetCurrentLinkFile(".\\adv\\mjpg\\k1\\中文\\删除确认.xml");
	m_MJPGList.SetMJPGRect(CRect(54, 62, 746, 358));
	m_MJPGList.SetUnitBitmap(4, L"", L"",  TRUE);
	m_MJPGList.SetUnitBitmap(5, L"", L"",  TRUE);

	m_bIsPasswordModel = false;
	m_bIsTipModel = false;
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDeleteTipDlg::OnButtonDeleteTipOk()
{	
	m_edtNewPassword1.ShowWindow(SW_HIDE);
	m_MJPGList.SetUnitBitmap(4, L"", L"",  TRUE);
	m_MJPGList.SetUnitBitmap(5, L"", L"",  TRUE);

	if (m_bIsPasswordModel)
	{
		CString p;
		m_edtNewPassword1.GetWindowText(p);
		if (m_sPassword == p)
		{
			ShowWindow_(FALSE);	
			m_edtNewPassword1.SetWindowText(_T(""));
				
			SetPasswordModel(false);
			//::SendMessage(m_handle, WM_DELETESELITEM, 0, 0);
			::PostMessage(m_handle, WM_DELETESELITEM, 0, 0);
		}
		else
		{

			//std::string strTemp;
			//strTemp = Data::LanguageResource::Get(Data::RI_PASSWORD_TIP3);
			SetDelTip(L"");
			m_MJPGList.SetUnitBitmap(4, L".\\adv\\mjpg\\k1\\common\\password_checkerror.bmp", L"",  TRUE);
			m_edtNewPassword1.SetWindowText(_T(""));
			SetPasswordModel(false);
			m_bIsTipModel = TRUE;
			return;
		}
	}
	else if (m_bIsTipModel)
	{
		m_bIsTipModel = FALSE; 
		ShowWindow_(FALSE);
		SetTipModel(false);
	}
	else
	{
		ShowWindow_(FALSE);
		SetPasswordModel(false);
		::PostMessage(m_handle, WM_DELETESELITEM, 0, 0);
	}
	m_edtNewPassword1.SetWindowText(_T(""));
	SipShowIM(SIPF_OFF);
}

void CDeleteTipDlg::OnButtonDeleteTipCancel()
{
	m_edtNewPassword1.SetWindowText(_T(""));
	m_edtNewPassword1.ShowWindow(SW_HIDE);
	m_MJPGList.SetUnitBitmap(4, L"", L"",  TRUE);
	m_MJPGList.SetUnitBitmap(5, L"", L"",  TRUE);

	ShowWindow_(FALSE);
	SipShowIM(SIPF_OFF);
}

void CDeleteTipDlg::SetHWnd(HWND handle)
{
	m_handle = handle;
}

void CDeleteTipDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	KillTimer(nIDEvent);
	m_edtNewPassword1.SetWindowText(_T(""));
	m_edtNewPassword1.ShowWindow(SW_HIDE);
	m_MJPGList.SetUnitBitmap(4, L"", L"",  TRUE);
	m_MJPGList.SetUnitBitmap(5, L"", L"",  TRUE);
	CCEDialog::OnTimer(nIDEvent);
}

void CDeleteTipDlg::SetPasswordModel(bool b)
{
 	m_bIsPasswordModel = b;
	if(b)
	{
		m_edtNewPassword1.ShowWindow(SW_SHOW);
		m_MJPGList.SetUnitBitmap(4, L".\\adv\\mjpg\\k1\\common\\password_supper.bmp", L"",  TRUE);
		m_MJPGList.SetUnitBitmap(5, L".\\adv\\mjpg\\k1\\common\\input_null.bmp", L"",  TRUE);
	}
}

void CDeleteTipDlg::SetPassword(CString password)
{
	m_sPassword = password;
}

void CDeleteTipDlg::SetDelTip(CString tips)
{
	m_MJPGList.SetUnitBitmap(100, tips, "", TRUE);
	//m_sticTip.SetWindowText(tips);
}

void CDeleteTipDlg::SetTitle(std::string title)
{
// 	TextStruct ts[1];
// 	memset(ts, 0, sizeof(TextStruct) * 1);
// 	
// 	ts[0].txtRect = CRect(8, 0, 100, 20);
// 	ts[0].txtFontSize = 16;
// 	ts[0].sAlign = DT_LEFT | DT_BOTTOM;
// 	memcpy(ts[0].sTxt, title.c_str(), title.length());
// 	
// 	m_sticBackground.SetTextStruct(ts, 1);
}

void CDeleteTipDlg::SetTipModel(BOOL b)
{
//	m_bIsTipModel = b;
//	m_btnCancel.ShowWindow(!b);
}