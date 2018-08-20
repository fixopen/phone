// passworddlg.cpp : implementation file
//

#include "stdafx.h"
#include "..\multimediaphone.h"
#include "passworddlg.h"
#include "../Data/LanguageResource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPasswordDlg dialog


CPasswordDlg::CPasswordDlg(CWnd* pParent /*=NULL*/)
	: CCEDialog(CPasswordDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPasswordDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CPasswordDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPasswordDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPasswordDlg, CCEDialog)
	//{{AFX_MSG_MAP(CPasswordDlg)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_PASSWORD_OK, OnButtonPasswordOk)
	ON_BN_CLICKED(IDC_BUTTON_PASSWORD_CANCEL, OnButtonPasswordCancel)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPasswordDlg message handlers

BOOL CPasswordDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	int X = 35;
	int Y = 40;
	std::string strTemp = Data::LanguageResource::Get(Data::RI_COMN_OKBTN);
	CString str = strTemp.c_str();
	m_btnOk.Create(str, WS_CHILD|WS_VISIBLE, CRect(340 + X, 25 + Y, 402 + X, 45 + Y), this, IDC_BUTTON_PASSWORD_OK);
	m_btnOk.SetColor(CCEButtonST::BTNST_COLOR_BK_IN, RGB(115,210,138));
	m_btnOk.SetColor(CCEButtonST::BTNST_COLOR_BK_OUT, RGB(115,210,138));
	m_btnOk.SetColor(CCEButtonST::BTNST_COLOR_BK_FOCUS, RGB(115,210,138));

	strTemp = Data::LanguageResource::Get(Data::RI_COMN_CANCELBTN);
	str = strTemp.c_str();
	m_btnCancel.Create(str, WS_CHILD|WS_VISIBLE, CRect(340 + X, 100 + Y, 402 + X, 120 + Y), this, IDC_BUTTON_PASSWORD_CANCEL);
	m_btnCancel.SetColor(CCEButtonST::BTNST_COLOR_BK_IN, RGB(115,210,138));
	m_btnCancel.SetColor(CCEButtonST::BTNST_COLOR_BK_OUT, RGB(115,210,138));
	m_btnCancel.SetColor(CCEButtonST::BTNST_COLOR_BK_FOCUS, RGB(115,210,138));

	strTemp = Data::LanguageResource::Get(Data::RI_PASSWORD_TITLE);
	str = strTemp.c_str();
	m_sticTitle.Create(str, WS_CHILD|WS_VISIBLE, CRect(0 + X, 0 + Y, 410 + X, 20 + Y), this, IDC_STATIC_CONTACTINFOSEARCH_TITLE);
	m_sticTitle.SetColor(RGB(0, 0, 0), RGB(115,210,138));
	m_sticTitle.SetLeftMargin(170);

	strTemp = Data::LanguageResource::Get(Data::RI_PASSWORD_OLDPASSWORD);
	str = strTemp.c_str();
	m_sticOldPassword.Create(str, WS_CHILD|WS_VISIBLE, CRect(25 + X, 30 + Y, 100 + X, 50 + Y), this);
	SetStaticDefaultColor(&m_sticOldPassword);
	m_edtOldPassword.Create(WS_CHILD|WS_VISIBLE, CRect(108 + X, 30 + Y, 320 + X, 50 + Y), this, IDC_EDIT_CONTACTINFOSEARCH_NAME);
	m_edtOldPassword.SetLimitText(15);

	strTemp = Data::LanguageResource::Get(Data::RI_PASSWORD_NEWPASSWORD1);
	str = strTemp.c_str();
	m_sticNewPassword1.Create(str, WS_CHILD|WS_VISIBLE, CRect(25 + X, 60 + Y, 100 + X, 80 + Y), this);
	SetStaticDefaultColor(&m_sticNewPassword1);
	m_edtNewPassword1.Create(WS_CHILD|WS_VISIBLE, CRect(108 + X, 60 + Y, 320 + X, 80 + Y), this, IDC_EDIT_CONTACTINFOSEARCH_NAME);
	m_edtNewPassword1.SetLimitText(15);

	strTemp = Data::LanguageResource::Get(Data::RI_PASSWORD_NEWPASSWORD2);
	str = strTemp.c_str();
	m_sticNewPassword2.Create(str, WS_CHILD|WS_VISIBLE, CRect(25 + X, 90 + Y, 100 + X, 110 + Y), this);
	SetStaticDefaultColor(&m_sticNewPassword2);
	m_edtNewPassword2.Create(WS_CHILD|WS_VISIBLE, CRect(108 + X, 90 + Y, 320 + X, 110 + Y), this, IDC_EDIT_CONTACTINFOSEARCH_NAME);
	m_edtNewPassword2.SetLimitText(15);
		
	m_sticBackground.Create(_T(""), WS_CHILD|WS_VISIBLE, CRect(X, Y, 410 + X, 125 + Y), this);
	m_sticBackground.SetColor(RGB(255,255,255), RGB(255,255,255));
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPasswordDlg::SetStaticDefaultColor(CCEStatic* cestatic)
{
	// 	cestatic->SetColor(RGB(0, 0, 0), RGB(194,235,149));
	cestatic->SetColor(RGB(0, 0, 0), RGB(255,255,255));
}

void CPasswordDlg::OnButtonPasswordOk()
{
	ShowWindow(FALSE);
}

void CPasswordDlg::OnButtonPasswordCancel()
{
	ShowWindow(FALSE); 
}