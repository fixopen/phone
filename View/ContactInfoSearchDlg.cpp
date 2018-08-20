// ContactInfoSearchDlg.cpp : implementation file
//

#include "stdafx.h"
#include "../MultimediaPhone.h"
#include "ContactInfoSearchDlg.h"
#include "../Data/LanguageResource.h"
#include "../Data/Contact.h"
#include "ContactInfoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CContactInfoSearchDlg dialog


CContactInfoSearchDlg::CContactInfoSearchDlg(CWnd* pParent /*=NULL*/)
	: CCEDialog(CContactInfoSearchDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CContactInfoSearchDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CContactInfoSearchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CContactInfoSearchDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CContactInfoSearchDlg, CCEDialog)
	//{{AFX_MSG_MAP(CContactInfoSearchDlg)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_CONTACTINFOSEARCH_CANCEL, OnButtonSearchcinfoCancel)
	ON_BN_CLICKED(IDC_BUTTON_CONTACTINFOSEARCH_SEARCH, OnButtonSearchcinfoSearch)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CContactInfoSearchDlg message handlers

BOOL CContactInfoSearchDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CenterWindow(GetDesktopWindow());	// center to the hpc screen
	int X = 35;
	int Y = 40;

	std::string strTemp = Data::LanguageResource::Get(Data::RI_COMN_FINDBTN);
	CString str = strTemp.c_str();
	m_btnSearch.Create(str, WS_CHILD|WS_VISIBLE, CRect(340 + X, 25 + Y, 402 + X, 45 + Y), this, IDC_BUTTON_CONTACTINFOSEARCH_SEARCH);
	m_btnSearch.SetColor(CCEButtonST::BTNST_COLOR_BK_IN, RGB(115,210,138));
	m_btnSearch.SetColor(CCEButtonST::BTNST_COLOR_BK_OUT, RGB(115,210,138));
	m_btnSearch.SetColor(CCEButtonST::BTNST_COLOR_BK_FOCUS, RGB(115,210,138));

	strTemp = Data::LanguageResource::Get(Data::RI_COMN_CANCELBTN);
	str = strTemp.c_str();
	m_btnCancel.Create(str, WS_CHILD|WS_VISIBLE, CRect(340 + X, 100 + Y, 402 + X, 120 + Y), this, IDC_BUTTON_CONTACTINFOSEARCH_CANCEL);
	m_btnCancel.SetColor(CCEButtonST::BTNST_COLOR_BK_IN, RGB(115,210,138));
	m_btnCancel.SetColor(CCEButtonST::BTNST_COLOR_BK_OUT, RGB(115,210,138));
	m_btnCancel.SetColor(CCEButtonST::BTNST_COLOR_BK_FOCUS, RGB(115,210,138));

//	m_sticTitle.Create(str, WS_CHILD|WS_VISIBLE, CRect(0 + X, 0 + Y, 410 + X, 20 + Y), this, IDC_STATIC_CONTACTINFOSEARCH_TITLE);
//	m_sticTitle.Create(str, WS_CHILD|WS_VISIBLE, CRect(0 + X, 0 + Y, 410 + X, 20 + Y), this, 0xFFFF);

/*
	strTemp = Data::LanguageResource::Get(Data::RI_RECORD_FIND_TITLE);
	str = strTemp.c_str();
	m_sticTitle.Create(str, WS_CHILD|WS_VISIBLE, CRect(0 + X, 0 + Y, 410 + X, 20 + Y), this, IDC_STATIC_CONTACTINFOSEARCH_TITLE);
	m_sticTitle.SetColor(RGB(0, 0, 0), RGB(115,210,138));
	m_sticTitle.SetLeftMargin(170);
	
	strTemp = Data::LanguageResource::Get(Data::RI_RECORD_FIND_TIP);
	str = strTemp.c_str();
	m_sticTip.Create(str, WS_CHILD|WS_VISIBLE, CRect(16 + X, 25 + Y, 340 + X, 40 + Y), this);
	SetStaticDefaultColor(&m_sticTip);

	strTemp = Data::LanguageResource::Get(Data::RI_RECORD_FIND_TIME);
	str = strTemp.c_str();
	m_sticTimeRange.Create(str, WS_CHILD|WS_VISIBLE, CRect(32 + X, 50 + Y, 88 + X, 65 + Y), this);
	SetStaticDefaultColor(&m_sticTimeRange);
	
	strTemp = Data::LanguageResource::Get(Data::RI_RECORD_FIND_TIMEFROM);
	str = strTemp.c_str();
	m_sticFrom.Create(str, WS_CHILD|WS_VISIBLE, CRect(88 + X, 50 + Y, 105 + X, 65 + Y), this);
	SetStaticDefaultColor(&m_sticFrom);

	strTemp = Data::LanguageResource::Get(Data::RI_RECORD_FIND_TIMETO);
	str = strTemp.c_str();
	m_sticTo.Create(str, WS_CHILD|WS_VISIBLE, CRect(203 + X, 50 + Y, 220 + X, 65 + Y), this);
	SetStaticDefaultColor(&m_sticTo);

	strTemp = Data::LanguageResource::Get(Data::RI_RECORD_FIND_NAME);
	str = strTemp.c_str();
	m_sticName.Create(str, WS_CHILD|WS_VISIBLE, CRect(32 + X, 75 + Y, 88 + X, 90 + Y), this);
	SetStaticDefaultColor(&m_sticName);

	strTemp = Data::LanguageResource::Get(Data::RI_RECORD_FIND_NUMBER);
	str = strTemp.c_str();
	m_sticNumber.Create(str, WS_CHILD|WS_VISIBLE, CRect(32 + X, 100 + Y, 88 + X, 115 + Y), this);
	SetStaticDefaultColor(&m_sticNumber);

	m_edtName.Create(WS_CHILD|WS_VISIBLE, CRect(94 + X, 75 + Y, 320 + X, 95 + Y), this, IDC_EDIT_CONTACTINFOSEARCH_NAME);
	m_edtName.SetLimitText(15);

	m_edtNumber.Create(WS_CHILD|WS_VISIBLE, CRect(94 + X, 100 + Y, 320 + X, 120 + Y), this, IDC_EDIT_CONTACTINFOSEARCH_NUMBER);
	m_edtNumber.SetLimitText(15);

	m_dtFrom.Create(WS_VISIBLE|WS_CHILD|DTS_SHORTDATEFORMAT, CRect(110 + X, 50 + Y, 204 + X, 65 + Y), this, IDC_DATETIME_CONTACTINFOSEARCH_FROM);
	m_dtTo.Create(WS_VISIBLE|WS_CHILD|DTS_SHORTDATEFORMAT, CRect(225 + X, 50 + Y, 319 + X, 65 + Y), this, IDC_DATETIME_CONTACTINFOSEARCH_TO);
	*/
	//lxz test
	//m_sticBackground.Create(_T(""), WS_CHILD|WS_VISIBLE, CRect(X, Y, 410 + X, 125 + Y), this);
	//m_sticBackground.SetColor(RGB(255,255,255), RGB(255,255,255));
	//lxz test
	m_sticBackground.Create(CRect(X, Y, 415 + X, 144 + Y), this);

	TextStruct txtstruct[4];
	memset(txtstruct, 0, sizeof(TextStruct)*4);

	txtstruct[0].sAlign = DT_CENTER;
	strTemp = Data::LanguageResource::Get(Data::RI_RECORD_FIND_TITLE);
	str = strTemp.c_str();
	memcpy(txtstruct[0].sTxt, strTemp.c_str(), strTemp.length());
	txtstruct[0].txtFontSize = 18;
	txtstruct[0].txtColor = RGB(0, 0, 0);
	txtstruct[0].txtRect = CRect(5, 3, 115, 23);

	txtstruct[1].sAlign = DT_CENTER;
	strTemp = Data::LanguageResource::Get(Data::RI_RECORD_FIND_TIP);
	str = strTemp.c_str();
	memcpy(txtstruct[1].sTxt, strTemp.c_str(), strTemp.length());
	txtstruct[1].txtFontSize = 12;
	txtstruct[1].txtColor = RGB(255, 0, 0);
	txtstruct[1].txtRect = CRect(24, 34, 244, 54);
	m_sticBackground.SetTextStruct(txtstruct, 2);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CContactInfoSearchDlg::SetStaticDefaultColor(CCEStatic* cestatic)
{
// 	cestatic->SetColor(RGB(0, 0, 0), RGB(194,235,149));
	cestatic->SetColor(RGB(0, 0, 0), RGB(255,255,255));
}

void CContactInfoSearchDlg::OnButtonSearchcinfoCancel() 
{
	// TODO: Add your control notification handler code here
 	ShowWindow(FALSE);
}

void CContactInfoSearchDlg::OnButtonSearchcinfoSearch() 
{
	// TODO: Add your control notification handler code here
	ShowWindow(FALSE);

	std::vector<boost::shared_ptr<Data::ContactInfo> > searchResult;

    CTime dateFrom;
    CTime dateTo;
	m_dtFrom.GetTime(dateFrom);
	m_dtTo.GetTime(dateTo);
	std::string filter = "startTime BETWEEN '";
	char buff[32];
	memset(buff, 0, 32);
	sprintf(buff, "%04d%02d%02d000000", dateFrom.GetYear(), dateFrom.GetMonth(),dateFrom.GetDay());//, dateFrom.GetHour(), dateFrom.GetMinute(), dateFrom.GetSecond());
	filter += buff;
	filter += "' AND '";
	memset(buff, 0, 32);
	sprintf(buff, "%04d%02d%02d235959", dateTo.GetYear(), dateTo.GetMonth(),dateTo.GetDay());//, dateTo.GetHour(), dateTo.GetMinute(), dateTo.GetSecond());
	filter += buff;
	filter += "'";

	CString csTel;
	m_edtNumber.GetWindowText(csTel);
	if (!csTel.IsEmpty())
	{
		filter += " AND telephoneNumber Like '%";
		filter += Util::StringOp::FromCString(csTel);
		filter += "%'";
	}

	CString csName;
	m_edtName.GetWindowText(csName);
	if (!csName.IsEmpty())
	{
		std::string flt = "name Like '%";
		flt += Util::StringOp::FromCString(csName);
		flt += "%'";
	
		if (!csTel.IsEmpty())
		{
			flt += " OR mobilesTelephone Like '%";
			flt += Util::StringOp::FromCString(csTel);
			flt += "%'";
			flt += " OR mobilesTelephone2 Like '%";
			flt += Util::StringOp::FromCString(csTel);
			flt += "%'";
			flt += " OR worksTelephone Like '%";
			flt += Util::StringOp::FromCString(csTel);
			flt += "%'";
			flt += " OR worksTelephone2 Like '%";
			flt += Util::StringOp::FromCString(csTel);
			flt += "%'";
		}

		std::vector<boost::shared_ptr<Data::Contact> > contactResult = Data::Contact::GetFromDatabase(flt);
		if (!contactResult.empty())
		{
			for (int i = 0; i < contactResult.size(); ++i)
			{
				std::string f = " AND contactId = ";
				f += Util::StringOp::FromInt(contactResult[0]->id());	
				std::vector<boost::shared_ptr<Data::ContactInfo> > contactinfoResult = Data::ContactInfo::GetFromDatabase(filter + f);
				for (std::vector<boost::shared_ptr<Data::ContactInfo> >::iterator iter = contactinfoResult.begin(); iter != contactinfoResult.end(); ++iter)
				{
					searchResult.push_back(*iter);
				}
			}
		}	
	}
	else
	{
		searchResult = Data::ContactInfo::GetFromDatabase(filter);
	}

	((CContactInfoDlg*)m_pParentWnd)->ShowSearchResultInList(searchResult);

	//((CContactInfoDlg*)m_pParentWnd)->Search("id = 1");
	//::SendMessage(m_pParentWnd->m_hWnd, WM_USER + 5, 0, 0);
}
