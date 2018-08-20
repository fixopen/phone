// fastdialsdlg.cpp : implementation file
//

#include "stdafx.h"
#include "..\multimediaphone.h"
#include "fastdialsdlg.h"
#include "../Data/LanguageResource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFastDialsDlg dialog


CFastDialsDlg::CFastDialsDlg(CWnd* pParent /*=NULL*/)
	: CCEDialog(CFastDialsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFastDialsDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CFastDialsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFastDialsDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFastDialsDlg, CCEDialog)
	//{{AFX_MSG_MAP(CFastDialsDlg)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_FASTDIALS_OK, OnButtonFastDialsOk)
	ON_BN_CLICKED(IDC_BUTTON_FASTDIALS_CANCEL, OnButtonFastDialsCancel)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFastDialsDlg message handlers

BOOL CFastDialsDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	int X = 35;
	int Y = 12;

	std::string strTemp;
	CString str;
	strTemp = Data::LanguageResource::Get(Data::RI_FASTDIALS_TITLE);
	str = strTemp.c_str();
	m_sticTitle.Create(str, WS_CHILD|WS_VISIBLE, CRect(0 + X, 0 + Y, 410 + X, 20 + Y), this);
	m_sticTitle.SetColor(RGB(0, 0, 0), RGB(115,210,138));
	m_sticTitle.SetLeftMargin(160);
	
	strTemp = Data::LanguageResource::Get(Data::RI_FASTDIALS_NUMBER01);
	str = strTemp.c_str();
	m_sticNumber01.Create(str, WS_CHILD|WS_VISIBLE, CRect(6 + X, 25 + Y, 56 + X + 20, 40 + Y), this);
	SetStaticDefaultColor(&m_sticNumber01);
	
	strTemp = Data::LanguageResource::Get(Data::RI_FASTDIALS_NUMBER02);
	str = strTemp.c_str();
	m_sticNumber02.Create(str, WS_CHILD|WS_VISIBLE, CRect(6 + X, 50 + Y, 56 + X + 20, 65 + Y), this);
	SetStaticDefaultColor(&m_sticNumber02);
	
	strTemp = Data::LanguageResource::Get(Data::RI_FASTDIALS_NUMBER03);
	str = strTemp.c_str();
	m_sticNumber03.Create(str, WS_CHILD|WS_VISIBLE, CRect(6 + X, 75 + Y, 56 + X + 20, 90 + Y), this);
	SetStaticDefaultColor(&m_sticNumber03);
	
	strTemp = Data::LanguageResource::Get(Data::RI_FASTDIALS_NUMBER04);
	str = strTemp.c_str();
	m_sticNumber04.Create(str, WS_CHILD|WS_VISIBLE, CRect(6 + X, 100 + Y, 56 + X + 20, 115 + Y), this);
	SetStaticDefaultColor(&m_sticNumber04);
	
	strTemp = Data::LanguageResource::Get(Data::RI_FASTDIALS_NUMBER05);
	str = strTemp.c_str();
	m_sticNumber05.Create(str, WS_CHILD|WS_VISIBLE, CRect(6 + X, 125 + Y, 56 + X + 20, 140 + Y), this);
	SetStaticDefaultColor(&m_sticNumber05);
	
	strTemp = Data::LanguageResource::Get(Data::RI_FASTDIALS_NUMBER06);
	str = strTemp.c_str();
	m_sticNumber06.Create(str, WS_CHILD|WS_VISIBLE, CRect(6 + X, 150 + Y, 56 + X + 20, 165 + Y), this);
	SetStaticDefaultColor(&m_sticNumber06);

	strTemp = Data::LanguageResource::Get(Data::RI_FASTDIALS_NUMBER07);
	str = strTemp.c_str();
	m_sticNumber07.Create(str, WS_CHILD|WS_VISIBLE, CRect(176 + X, 25 + Y, 226 + X + 20, 40 + Y), this);
	SetStaticDefaultColor(&m_sticNumber07);
	
	strTemp = Data::LanguageResource::Get(Data::RI_FASTDIALS_NUMBER08);
	str = strTemp.c_str();
	m_sticNumber08.Create(str, WS_CHILD|WS_VISIBLE, CRect(176 + X, 50 + Y, 226 + X + 20, 65 + Y), this);
	SetStaticDefaultColor(&m_sticNumber08);
	
	strTemp = Data::LanguageResource::Get(Data::RI_FASTDIALS_NUMBER09);
	str = strTemp.c_str();
	m_sticNumber09.Create(str, WS_CHILD|WS_VISIBLE, CRect(176 + X, 75 + Y, 226 + X + 20, 90 + Y), this);
	SetStaticDefaultColor(&m_sticNumber09);
	
	strTemp = Data::LanguageResource::Get(Data::RI_FASTDIALS_NUMBER10);
	str = strTemp.c_str();
	m_sticNumber10.Create(str, WS_CHILD|WS_VISIBLE, CRect(176 + X, 100 + Y, 226 + X + 20, 115 + Y), this);
	SetStaticDefaultColor(&m_sticNumber10);
	
	strTemp = Data::LanguageResource::Get(Data::RI_FASTDIALS_NUMBER11);
	str = strTemp.c_str();
	m_sticNumber11.Create(str, WS_CHILD|WS_VISIBLE, CRect(176 + X, 125 + Y, 226 + X + 20, 140 + Y), this);
	SetStaticDefaultColor(&m_sticNumber11);
	
	strTemp = Data::LanguageResource::Get(Data::RI_FASTDIALS_NUMBER12);
	str = strTemp.c_str();
	m_sticNumber12.Create(str, WS_CHILD|WS_VISIBLE, CRect(176 + X, 150 + Y, 226 + X + 20, 165 + Y), this);
	SetStaticDefaultColor(&m_sticNumber12);
	
	m_edtNumber01.Create(WS_CHILD|WS_VISIBLE, CRect(60 + X + 20, 25 + Y, 170 + X, 45 + Y), this, IDC_EDIT_FASTDIALS_NUMBER01);
	m_edtNumber01.SetLimitText(15);

	m_edtNumber02.Create(WS_CHILD|WS_VISIBLE, CRect(60 + X + 20, 50 + Y, 170 + X, 70 + Y), this, IDC_EDIT_FASTDIALS_NUMBER02);
	m_edtNumber02.SetLimitText(15);

	m_edtNumber03.Create(WS_CHILD|WS_VISIBLE, CRect(60 + X + 20, 75 + Y, 170 + X, 95 + Y), this, IDC_EDIT_FASTDIALS_NUMBER03);
	m_edtNumber03.SetLimitText(15);

	m_edtNumber04.Create(WS_CHILD|WS_VISIBLE, CRect(60 + X + 20, 100 + Y, 170 + X, 120 + Y), this, IDC_EDIT_FASTDIALS_NUMBER04);
	m_edtNumber04.SetLimitText(15);

	m_edtNumber05.Create(WS_CHILD|WS_VISIBLE, CRect(60 + X + 20, 125 + Y, 170 + X, 145 + Y), this, IDC_EDIT_FASTDIALS_NUMBER05);
	m_edtNumber05.SetLimitText(15);

	m_edtNumber06.Create(WS_CHILD|WS_VISIBLE, CRect(60 + X + 20, 150 + Y, 170 + X, 170 + Y), this, IDC_EDIT_FASTDIALS_NUMBER06);
	m_edtNumber06.SetLimitText(15);

	m_edtNumber07.Create(WS_CHILD|WS_VISIBLE, CRect(230 + X + 20, 25 + Y, 340 + X, 45 + Y), this, IDC_EDIT_FASTDIALS_NUMBER07);
	m_edtNumber07.SetLimitText(15);

	m_edtNumber08.Create(WS_CHILD|WS_VISIBLE, CRect(230 + X + 20, 50 + Y, 340 + X, 70 + Y), this, IDC_EDIT_FASTDIALS_NUMBER08);
	m_edtNumber08.SetLimitText(15);

	m_edtNumber09.Create(WS_CHILD|WS_VISIBLE, CRect(230 + X + 20, 75 + Y, 340 + X, 95 + Y), this, IDC_EDIT_FASTDIALS_NUMBER09);
	m_edtNumber09.SetLimitText(15);

	m_edtNumber10.Create(WS_CHILD|WS_VISIBLE, CRect(230 + X + 20, 100 + Y, 340 + X, 120 + Y), this, IDC_EDIT_FASTDIALS_NUMBER10);
	m_edtNumber10.SetLimitText(15);

	m_edtNumber11.Create(WS_CHILD|WS_VISIBLE, CRect(230 + X + 20, 125 + Y, 340 + X, 145 + Y), this, IDC_EDIT_FASTDIALS_NUMBER11);
	m_edtNumber11.SetLimitText(15);

	m_edtNumber12.Create(WS_CHILD|WS_VISIBLE, CRect(230 + X + 20, 150 + Y, 340 + X, 170 + Y), this, IDC_EDIT_FASTDIALS_NUMBER12);
	m_edtNumber12.SetLimitText(15);
	
	strTemp = Data::LanguageResource::Get(Data::RI_COMN_OKBTN);
	str = strTemp.c_str();
	m_btnOk.Create(str, WS_CHILD|WS_VISIBLE, CRect(340 + X, 25 + Y, 402 + X, 45 + Y), this, IDC_BUTTON_FASTDIALS_OK);
	m_btnOk.SetColor(CCEButtonST::BTNST_COLOR_BK_IN, RGB(115,210,138));
	m_btnOk.SetColor(CCEButtonST::BTNST_COLOR_BK_OUT, RGB(115,210,138));
	m_btnOk.SetColor(CCEButtonST::BTNST_COLOR_BK_FOCUS, RGB(115,210,138));

	strTemp = Data::LanguageResource::Get(Data::RI_COMN_CANCELBTN);
	str = strTemp.c_str();
	m_btnCancel.Create(str, WS_CHILD|WS_VISIBLE, CRect(340 + X, 150 + Y, 402 + X, 170 + Y), this, IDC_BUTTON_FASTDIALS_CANCEL);
	m_btnCancel.SetColor(CCEButtonST::BTNST_COLOR_BK_IN, RGB(115,210,138));
	m_btnCancel.SetColor(CCEButtonST::BTNST_COLOR_BK_OUT, RGB(115,210,138));
	m_btnCancel.SetColor(CCEButtonST::BTNST_COLOR_BK_FOCUS, RGB(115,210,138));

	m_sticBackground.Create(_T(""), WS_CHILD|WS_VISIBLE, CRect(X, Y, 410 + X, 180 + Y), this);
	m_sticBackground.SetColor(RGB(255,255,255), RGB(255,255,255));
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CFastDialsDlg::SetStaticDefaultColor(CCEStatic* cestatic)
{
 	cestatic->SetColor(RGB(0, 0, 0), RGB(194,235,149));
//	cestatic->SetColor(RGB(0, 0, 0), RGB(240,250,223));
}

void CFastDialsDlg::OnButtonFastDialsOk()
{
	ShowWindow(FALSE);
}

void CFastDialsDlg::OnButtonFastDialsCancel()
{
	ShowWindow(FALSE);
}
