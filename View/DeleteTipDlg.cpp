// deletetipdlg.cpp : implementation file
//

#include "stdafx.h"
#include "..\multimediaphone.h"
#include "deletetipdlg.h"
#include "../Data/LanguageResource.h"

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
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_DELETETIP_OK, OnButtonDeleteTipOk)
	ON_BN_CLICKED(IDC_BUTTON_DELETETIP_CANCEL, OnButtonDeleteTipCancel)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDeleteTipDlg message handlers

BOOL CDeleteTipDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	int X = 35;
	int Y = 40;
	std::string strTemp = Data::LanguageResource::Get(Data::RI_COMN_OKBTN);
	CString str = strTemp.c_str();
	m_btnOk.Create(str, WS_CHILD|WS_VISIBLE, CRect(340 + X, 25 + Y, 402 + X, 45 + Y), this, IDC_BUTTON_DELETETIP_OK);
	m_btnOk.SetColor(CCEButtonST::BTNST_COLOR_BK_IN, RGB(115,210,138));
	m_btnOk.SetColor(CCEButtonST::BTNST_COLOR_BK_OUT, RGB(115,210,138));
	m_btnOk.SetColor(CCEButtonST::BTNST_COLOR_BK_FOCUS, RGB(115,210,138));

	strTemp = Data::LanguageResource::Get(Data::RI_COMN_CANCELBTN);
	str = strTemp.c_str();
	m_btnCancel.Create(str, WS_CHILD|WS_VISIBLE, CRect(340 + X, 100 + Y, 402 + X, 120 + Y), this, IDC_BUTTON_DELETETIP_CANCEL);
	m_btnCancel.SetColor(CCEButtonST::BTNST_COLOR_BK_IN, RGB(115,210,138));
	m_btnCancel.SetColor(CCEButtonST::BTNST_COLOR_BK_OUT, RGB(115,210,138));
	m_btnCancel.SetColor(CCEButtonST::BTNST_COLOR_BK_FOCUS, RGB(115,210,138));

	strTemp = Data::LanguageResource::Get(Data::RI_DELETETIP_TITLE);
	str = strTemp.c_str();
	m_sticTitle.Create(str, WS_CHILD|WS_VISIBLE, CRect(0 + X, 0 + Y, 410 + X, 20 + Y), this, IDC_STATIC_CONTACTINFOSEARCH_TITLE);
	m_sticTitle.SetColor(RGB(0, 0, 0), RGB(115,210,138));
	m_sticTitle.SetLeftMargin(170);

	strTemp = Data::LanguageResource::Get(Data::RI_DELETETIP_TIP);
	str = strTemp.c_str();
	m_sticTip.Create(str, WS_CHILD|WS_VISIBLE, CRect(25 + X, 30 + Y, 200 + X, 50 + Y), this);
	SetStaticDefaultColor(&m_sticTip);
	
	strTemp = Data::LanguageResource::Get(Data::RI_DELETETIP_PASSWORD);
	str = strTemp.c_str();
	m_sticPassword.Create(str, WS_CHILD|WS_VISIBLE, CRect(50 + X, 70 + Y, 110 + X, 90 + Y), this);
	SetStaticDefaultColor(&m_sticPassword);

	m_edtPassword.Create(WS_CHILD|WS_VISIBLE, CRect(120 + X, 70 + Y, 320 + X, 90 + Y), this, IDC_EDIT_DELETETIP_PASSWORD);
	m_edtPassword.SetLimitText(15);
		
	m_sticBackground.Create(_T(""), WS_CHILD|WS_VISIBLE, CRect(X, Y, 410 + X, 125 + Y), this);
	m_sticBackground.SetColor(RGB(255,255,255), RGB(255,255,255));

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDeleteTipDlg::SetStaticDefaultColor(CCEStatic* cestatic)
{
// 	cestatic->SetColor(RGB(0, 0, 0), RGB(194,235,149));
	cestatic->SetColor(RGB(0, 0, 0), RGB(255,255,255));
}

void CDeleteTipDlg::OnButtonDeleteTipOk()
{
	ShowWindow(FALSE);
}

void CDeleteTipDlg::OnButtonDeleteTipCancel()
{
	ShowWindow(FALSE);
}