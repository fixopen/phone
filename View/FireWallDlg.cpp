// firewalldlg.cpp : implementation file
//

#include "stdafx.h"
#include "..\multimediaphone.h"
#include "firewalldlg.h"
#include "../Data/LanguageResource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFireWallDlg dialog


CFireWallDlg::CFireWallDlg(CWnd* pParent /*=NULL*/)
	: CCEDialog(CFireWallDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFireWallDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CFireWallDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFireWallDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFireWallDlg, CCEDialog)
	//{{AFX_MSG_MAP(CFireWallDlg)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_FIREWALL_OK, OnButtonFireWallOk)
	ON_BN_CLICKED(IDC_BUTTON_FIREWALL_CANCEL, OnButtonFireWallCancel)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFireWallDlg message handlers

BOOL CFireWallDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	int X = 35;
	int Y = 12;

	std::string strTemp;
	CString str;

	strTemp = Data::LanguageResource::Get(Data::RI_FIREWALL_TITLE);
	str = strTemp.c_str();
	m_sticTitle.Create(str, WS_CHILD|WS_VISIBLE, CRect(0 + X, 0 + Y, 410 + X, 20 + Y), this);
	m_sticTitle.SetColor(RGB(0, 0, 0), RGB(115,210,138));
	m_sticTitle.SetLeftMargin(160);
	
	strTemp = Data::LanguageResource::Get(Data::RI_FIREWALL_ENABLE);
	str = strTemp.c_str();
	m_chbEnableFireWall.Create(str, WS_CHILD|WS_VISIBLE|BS_CHECKBOX, CRect(25 + X, 25 + Y, 150 + X, 45 + Y), this, IDC_CHECKBOX_FIREWALL_ENABLE);
	m_chbEnableFireWall.SetIcon(IDI_ICON1, CSize(12, 12), IDI_ICON2, CSize(12, 12));
	m_chbEnableFireWall.SetColor(CCEButtonST::BTNST_COLOR_BK_IN, RGB(115,210,138));
	m_chbEnableFireWall.SetColor(CCEButtonST::BTNST_COLOR_BK_OUT, RGB(115,210,138));
	m_chbEnableFireWall.SetColor(CCEButtonST::BTNST_COLOR_BK_FOCUS, RGB(115,210,138));

	strTemp = Data::LanguageResource::Get(Data::RI_FIREWALL_BLACKLIST);
	str = strTemp.c_str();
	m_chbRefuseBlacklist.Create(str, WS_CHILD|WS_VISIBLE|BS_CHECKBOX, CRect(50 + X, 50 + Y, 200 + X, 70 + Y), this, IDC_CHECKBOX_FIREWALL_BLACKLIST);
	m_chbRefuseBlacklist.SetIcon(IDI_ICON1, CSize(12, 12), IDI_ICON2, CSize(12, 12));
	m_chbRefuseBlacklist.SetColor(CCEButtonST::BTNST_COLOR_BK_IN, RGB(115,210,138));
	m_chbRefuseBlacklist.SetColor(CCEButtonST::BTNST_COLOR_BK_OUT, RGB(115,210,138));
	m_chbRefuseBlacklist.SetColor(CCEButtonST::BTNST_COLOR_BK_FOCUS, RGB(115,210,138));

	strTemp = Data::LanguageResource::Get(Data::RI_FIREWALL_CONTACT);
	str = strTemp.c_str();
	m_chbContactExceptBlacklist.Create(str, WS_CHILD|WS_VISIBLE|BS_CHECKBOX, CRect(50 + X, 75 + Y, 250 + X, 95 + Y), this, IDC_CHECKBOX_FIREWALL_CONTACT);
	m_chbContactExceptBlacklist.SetIcon(IDI_ICON1, CSize(12, 12), IDI_ICON2, CSize(12, 12));
	m_chbContactExceptBlacklist.SetColor(CCEButtonST::BTNST_COLOR_BK_IN, RGB(115,210,138));
	m_chbContactExceptBlacklist.SetColor(CCEButtonST::BTNST_COLOR_BK_OUT, RGB(115,210,138));
	m_chbContactExceptBlacklist.SetColor(CCEButtonST::BTNST_COLOR_BK_FOCUS, RGB(115,210,138));

	strTemp = Data::LanguageResource::Get(Data::RI_FIREWALL_VIP);
	str = strTemp.c_str();
	m_chbOnlyVIP.Create(str, WS_CHILD|WS_VISIBLE|BS_CHECKBOX, CRect(50 + X, 100 + Y, 200 + X, 120 + Y), this, IDC_CHECKBOX_FIREWALL_VIP);
	m_chbOnlyVIP.SetIcon(IDI_ICON1, CSize(12, 12), IDI_ICON2, CSize(12, 12));
	m_chbOnlyVIP.SetColor(CCEButtonST::BTNST_COLOR_BK_IN, RGB(115,210,138));
	m_chbOnlyVIP.SetColor(CCEButtonST::BTNST_COLOR_BK_OUT, RGB(115,210,138));
	m_chbOnlyVIP.SetColor(CCEButtonST::BTNST_COLOR_BK_FOCUS, RGB(115,210,138));

	strTemp = Data::LanguageResource::Get(Data::RI_FIREWALL_ALL);
	str = strTemp.c_str();
	m_chbRefuseAllByTime.Create(str, WS_CHILD|WS_VISIBLE|BS_CHECKBOX, CRect(50 + X, 125 + Y, 200 + X, 145 + Y), this, IDC_CHECKBOX_FIREWALL_ALL);
	m_chbRefuseAllByTime.SetIcon(IDI_ICON1, CSize(12, 12), IDI_ICON2, CSize(12, 12));
	m_chbRefuseAllByTime.SetColor(CCEButtonST::BTNST_COLOR_BK_IN, RGB(115,210,138));
	m_chbRefuseAllByTime.SetColor(CCEButtonST::BTNST_COLOR_BK_OUT, RGB(115,210,138));
	m_chbRefuseAllByTime.SetColor(CCEButtonST::BTNST_COLOR_BK_FOCUS, RGB(115,210,138));

	strTemp = Data::LanguageResource::Get(Data::RI_FIREWALL_PART1);
	str = strTemp.c_str();
	m_sticTimePart1.Create(str, WS_CHILD|WS_VISIBLE, CRect(200 + X, 125 + Y, 260 + X, 145 + Y), this);

	m_cmbTime.Create(WS_CHILD|WS_VISIBLE, CRect(260 + X, 125 + Y, 320 + X, 205 + Y), this, IDC_COMBO_FIREWALL_TIME);
	m_cmbTime.AddString(_T("1"));
	m_cmbTime.AddString(_T("2"));
	m_cmbTime.AddString(_T("3"));
	m_cmbTime.SelectString(0, _T("1"));

	strTemp = Data::LanguageResource::Get(Data::RI_FIREWALL_PART2);
	str = strTemp.c_str();
	m_sticTimePart2.Create(str, WS_CHILD|WS_VISIBLE, CRect(320 + X, 125 + Y, 380 + X, 145 + Y), this);


	strTemp = Data::LanguageResource::Get(Data::RI_COMN_OKBTN);
	str = strTemp.c_str();
	m_btnOk.Create(str, WS_CHILD|WS_VISIBLE, CRect(340 + X, 25 + Y, 402 + X, 45 + Y), this, IDC_BUTTON_FIREWALL_OK);
	m_btnOk.SetColor(CCEButtonST::BTNST_COLOR_BK_IN, RGB(115,210,138));
	m_btnOk.SetColor(CCEButtonST::BTNST_COLOR_BK_OUT, RGB(115,210,138));
	m_btnOk.SetColor(CCEButtonST::BTNST_COLOR_BK_FOCUS, RGB(115,210,138));

	strTemp = Data::LanguageResource::Get(Data::RI_COMN_CANCELBTN);
	str = strTemp.c_str();
	m_btnCancel.Create(str, WS_CHILD|WS_VISIBLE, CRect(340 + X, 150 + Y, 402 + X, 170 + Y), this, IDC_BUTTON_FIREWALL_CANCEL);
	m_btnCancel.SetColor(CCEButtonST::BTNST_COLOR_BK_IN, RGB(115,210,138));
	m_btnCancel.SetColor(CCEButtonST::BTNST_COLOR_BK_OUT, RGB(115,210,138));
	m_btnCancel.SetColor(CCEButtonST::BTNST_COLOR_BK_FOCUS, RGB(115,210,138));

	m_sticBackground.Create(_T(""), WS_CHILD|WS_VISIBLE, CRect(X, Y, 410 + X, 180 + Y), this);
	m_sticBackground.SetColor(RGB(255,255,255), RGB(255,255,255));
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

	
void CFireWallDlg::OnButtonFireWallOk()
{
	ShowWindow(FALSE);
}

void CFireWallDlg::OnButtonFireWallCancel()
{
	ShowWindow(FALSE);
}
