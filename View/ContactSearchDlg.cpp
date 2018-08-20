// contactsearchdlg.cpp : implementation file
//

#include "stdafx.h"
#include "..\multimediaphone.h"
#include "contactsearchdlg.h"
#include "../Data/LanguageResource.h"
#include "../Data/Contact.h"
#include "ContactDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CContactSearchDlg dialog


CContactSearchDlg::CContactSearchDlg(CWnd* pParent /*=NULL*/)
	: CCEDialog(CContactSearchDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CContactSearchDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CContactSearchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CContactSearchDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CContactSearchDlg, CCEDialog)
	//{{AFX_MSG_MAP(CContactSearchDlg)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_CONTACTSEARCH_CANCEL, OnButtonSearchContactCancel)
	ON_BN_CLICKED(IDC_BUTTON_CONTACTSEARCH_SEARCH, OnButtonSearchContactSearch)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CContactSearchDlg message handlers

BOOL CContactSearchDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	int X = 35;
	int Y = 12;
	std::string strTemp = Data::LanguageResource::Get(Data::RI_COMN_FINDBTN);
	CString str = strTemp.c_str();
	m_btnSearch.Create(str, WS_CHILD|WS_VISIBLE, CRect(340 + X, 25 + Y, 402 + X, 45 + Y), this, IDC_BUTTON_CONTACTSEARCH_SEARCH);
	m_btnSearch.SetColor(CCEButtonST::BTNST_COLOR_BK_IN, RGB(115,210,138));
	m_btnSearch.SetColor(CCEButtonST::BTNST_COLOR_BK_OUT, RGB(115,210,138));
	m_btnSearch.SetColor(CCEButtonST::BTNST_COLOR_BK_FOCUS, RGB(115,210,138));

	strTemp = Data::LanguageResource::Get(Data::RI_COMN_CANCELBTN);
	str = strTemp.c_str();
	m_btnCancel.Create(str, WS_CHILD|WS_VISIBLE, CRect(340 + X, 150 + Y, 402 + X, 170 + Y), this, IDC_BUTTON_CONTACTSEARCH_CANCEL);
	m_btnCancel.SetColor(CCEButtonST::BTNST_COLOR_BK_IN, RGB(115,210,138));
	m_btnCancel.SetColor(CCEButtonST::BTNST_COLOR_BK_OUT, RGB(115,210,138));
	m_btnCancel.SetColor(CCEButtonST::BTNST_COLOR_BK_FOCUS, RGB(115,210,138));
	
	
	strTemp = Data::LanguageResource::Get(Data::RI_CARD_FIND_TITLE);
	str = strTemp.c_str();
	m_sticTitle.Create(str, WS_CHILD|WS_VISIBLE, CRect(0 + X, 0 + Y, 410 + X, 20 + Y), this);
	m_sticTitle.SetColor(RGB(0, 0, 0), RGB(115,210,138));
	m_sticTitle.SetLeftMargin(180);
	
	strTemp = Data::LanguageResource::Get(Data::RI_CARD_FIND_TIP);
	str = strTemp.c_str();
	m_sticTip.Create(str, WS_CHILD|WS_VISIBLE, CRect(16 + X, 25 + Y, 340 + X, 40 + Y), this);
	SetStaticDefaultColor(&m_sticTip);

	strTemp = Data::LanguageResource::Get(Data::RI_CARD_FIND_NAME);
	str = strTemp.c_str();
	m_sticName.Create(str, WS_CHILD|WS_VISIBLE, CRect(32 + X, 50 + Y, 88 + X, 65 + Y), this);
	SetStaticDefaultColor(&m_sticName);

	strTemp = Data::LanguageResource::Get(Data::RI_CARD_FIND_NUMBER);
	str = strTemp.c_str();
	m_sticNumber.Create(str, WS_CHILD|WS_VISIBLE, CRect(32 + X, 75 + Y, 88 + X, 90 + Y), this);
	SetStaticDefaultColor(&m_sticNumber);

	strTemp = Data::LanguageResource::Get(Data::RI_CARD_FIND_COMPANY);
	str = strTemp.c_str();
	m_sticCompany.Create(str, WS_CHILD|WS_VISIBLE, CRect(32 + X, 100 + Y, 88 + X, 115 + Y), this);
	SetStaticDefaultColor(&m_sticCompany);

	strTemp = Data::LanguageResource::Get(Data::RI_CARD_FIND_DUTY);
	str = strTemp.c_str();
	m_sticDuty.Create(str, WS_CHILD|WS_VISIBLE, CRect(32 + X, 125 + Y, 88 + X, 140 + Y), this);
	SetStaticDefaultColor(&m_sticDuty);

	m_edtName.Create(WS_CHILD|WS_VISIBLE, CRect(94 + X, 50 + Y, 320 + X, 70 + Y), this, IDC_EDIT_CONTACTSEARCH_NAME);
	m_edtName.SetLimitText(15);

	m_edtNumber.Create(WS_CHILD|WS_VISIBLE, CRect(94 + X, 75 + Y, 320 + X, 95 + Y), this, IDC_EDIT_CONTACTSEARCH_NUMBER);
	m_edtNumber.SetLimitText(15);

	m_edtCompany.Create(WS_CHILD|WS_VISIBLE, CRect(94 + X, 100 + Y, 320 + X, 120 + Y), this, IDC_EDIT_CONTACTSEARCH_COMPANY);
	m_edtCompany.SetLimitText(15);
	
	m_edtDuty.Create(WS_CHILD|WS_VISIBLE, CRect(94 + X, 125 + Y, 320 + X, 145 + Y), this, IDC_EDIT_CONTACTSEARCH_DUTY);
	m_edtDuty.SetLimitText(15);

	m_sticBackground.Create(_T(""), WS_CHILD|WS_VISIBLE, CRect(X, Y, 410 + X, 180 + Y), this);
	m_sticBackground.SetColor(RGB(255,255,255), RGB(255,255,255));
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
	
void CContactSearchDlg::SetStaticDefaultColor(CCEStatic* cestatic)
{
// 	cestatic->SetColor(RGB(0, 0, 0), RGB(194,235,149));
	cestatic->SetColor(RGB(0, 0, 0), RGB(255,255,255));
}

void CContactSearchDlg::OnButtonSearchContactCancel() 
{
	// TODO: Add your control notification handler code here
 	ShowWindow(FALSE);
}

void CContactSearchDlg::OnButtonSearchContactSearch() 
{
	// TODO: Add your control notification handler code here
	ShowWindow(FALSE);

	std::string filter;

	CString csName;
	m_edtName.GetWindowText(csName);
	if (!csName.IsEmpty())
	{
		filter = "name Like '%";
		filter += Util::StringOp::FromCString(csName);
		filter += "%'";
	
	}

	CString csTel;
	m_edtNumber.GetWindowText(csTel);
	if (!csTel.IsEmpty())
	{
		if (filter != "")
		{
			filter += " AND ";
		}
		filter += "mobilesTelephone Like '%";
		filter += Util::StringOp::FromCString(csTel);
		filter += "%'";
		filter += " OR mobilesTelephone2 Like '%";
		filter += Util::StringOp::FromCString(csTel);
		filter += "%'";
		filter += " OR worksTelephone Like '%";
		filter += Util::StringOp::FromCString(csTel);
		filter += "%'";
		filter += " OR worksTelephone2 Like '%";
		filter += Util::StringOp::FromCString(csTel);
		filter += "%'";
	}
	
	CString csCompany;
	m_edtCompany.GetWindowText(csCompany);
	if (!csCompany.IsEmpty())
	{
		if (filter != "")
		{
			filter += " AND ";
		}
		filter += "company Like '%";
		filter += Util::StringOp::FromCString(csCompany);
		filter += "%'";
	}

	CString csDuty;
	m_edtDuty.GetWindowText(csDuty);
	if (!csDuty.IsEmpty())
	{
		if (filter != "")
		{
			filter += " AND ";
		}
		filter += "duty Like '%";
		filter += Util::StringOp::FromCString(csDuty);
		filter += "%'";
	}

	if (filter == "")
		return;
		
	std::vector<boost::shared_ptr<Data::Contact> > searchResult;
	searchResult = Data::Contact::GetFromDatabase(filter);
	((CContactDlg*)m_pParentWnd)->ShowSearchResultInList(searchResult);
}