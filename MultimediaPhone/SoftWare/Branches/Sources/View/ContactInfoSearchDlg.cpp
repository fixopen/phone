// ContactInfoSearchDlg.cpp : implementation file
//

#include "stdafx.h"
#include "../MultimediaPhone.h"
#include "ContactInfoSearchDlg.h"
#include "../Data/LanguageResource.h"
//#include "../Data/Contact.h"
#include "ContactInfoDlg.h"
#include "../Data/SkinStyle.h"

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

 	std::string strTemp;
	CString str;

	strTemp = Data::LanguageResource::Get(Data::RI_RECORD_FIND_TIP);
	str = strTemp.c_str();
	m_sticTip.Create(str, WS_CHILD|WS_VISIBLE, CRect(62, 76, 340, 94), this);
	m_sticTip.SetColor(RGB(0, 0, 0), Data::g_partFrameInRectBackRGB[Data::g_skinstyle]);

	strTemp = Data::LanguageResource::Get(Data::RI_RECORD_FIND_TIME);
	str = strTemp.c_str();
	m_sticTimeRange.Create(str, WS_CHILD|WS_VISIBLE, CRect(50, 98, 144, 116), this);
	m_sticTimeRange.SetColor(RGB(0, 0, 0), Data::g_partFrameInRectBackRGB[Data::g_skinstyle]);
	m_sticTimeRange.SetAlign(SS_CENTER);
	
	strTemp = Data::LanguageResource::Get(Data::RI_RECORD_FIND_TIMETO);
	str = strTemp.c_str();
	str = "-";
	m_sticTo.Create(str, WS_CHILD|WS_VISIBLE, CRect(241, 98, 245, 116), this);
	m_sticTo.SetColor(RGB(0, 0, 0), Data::g_partFrameInRectBackRGB[Data::g_skinstyle]);
	m_sticTo.SetAlign(SS_CENTER);

	strTemp = Data::LanguageResource::Get(Data::RI_RECORD_FIND_NAME);
	str = strTemp.c_str();
	m_sticName.Create(str, WS_CHILD|WS_VISIBLE, CRect(50, 123, 144, 141), this);
	m_sticName.SetColor(RGB(0, 0, 0), Data::g_partFrameInRectBackRGB[Data::g_skinstyle]);
	m_sticName.SetAlign(SS_CENTER);

	strTemp = Data::LanguageResource::Get(Data::RI_RECORD_FIND_NUMBER);
	str = strTemp.c_str();
	m_sticNumber.Create(str, WS_CHILD|WS_VISIBLE, CRect(50, 148, 144, 166), this);
	m_sticNumber.SetColor(RGB(0, 0, 0), Data::g_partFrameInRectBackRGB[Data::g_skinstyle]);
	m_sticNumber.SetAlign(SS_CENTER);

	m_edtName.Create(WS_CHILD|WS_VISIBLE, CRect(148, 123, 338, 141), this, IDC_EDIT_CONTACTINFOSEARCH_NAME);
	m_edtName.SetLimitText(15);

	m_edtNumber.Create(WS_CHILD|WS_VISIBLE, CRect(148, 148, 338, 166), this, IDC_EDIT_CONTACTINFOSEARCH_NUMBER);
	m_edtNumber.SetLimitText(15);

	m_dtFrom.Create(WS_VISIBLE|WS_CHILD|DTS_SHORTDATEFORMAT, CRect(148, 98, 240, 116), this, IDC_DATETIME_CONTACTINFOSEARCH_FROM);
	m_dtTo.Create(WS_VISIBLE|WS_CHILD|DTS_SHORTDATEFORMAT, CRect(246, 98, 338, 116), this, IDC_DATETIME_CONTACTINFOSEARCH_TO);

	strTemp = Data::LanguageResource::Get(Data::RI_COMN_FINDBTN);
	str = strTemp.c_str();
	m_btnSearch.Create(str, Data::g_buttonArcBMPPARTDILOAGID[0][Data::g_skinstyle], Data::g_buttonArcBMPPARTDILOAGID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(378, 78, 434, 98), this, IDC_BUTTON_CONTACTINFOSEARCH_SEARCH);
	m_btnSearch.SetBackRGB(Data::g_partFrameMainBackRGB[Data::g_skinstyle]);
	
	strTemp = Data::LanguageResource::Get(Data::RI_COMN_CANCELBTN);
	str = strTemp.c_str();
	m_btnCancel.Create(str, Data::g_buttonArcBMPPARTDILOAGID[0][Data::g_skinstyle], Data::g_buttonArcBMPPARTDILOAGID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(378, 103, 434, 123), this, IDC_BUTTON_CONTACTINFOSEARCH_CANCEL);
	m_btnCancel.SetBackRGB(Data::g_partFrameMainBackRGB[Data::g_skinstyle]);
	
	m_sticBackground.Create(CRect(34, 40, 448, 184), this, 1);

	TextStruct ts[1];
	memset(ts, 0, sizeof(TextStruct) * 1);
	
	ts[0].txtRect = CRect(8, 0, 100, 20);
	ts[0].txtFontSize = 16;
	ts[0].sAlign = DT_LEFT | DT_BOTTOM;
	memcpy(ts[0].sTxt, Data::LanguageResource::Get(Data::RI_RECORD_FIND_TITLE).c_str(), Data::LanguageResource::Get(Data::RI_RECORD_FIND_TITLE).length());
	
	m_sticBackground.SetTextStruct(ts, 1);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CContactInfoSearchDlg::OnButtonSearchcinfoCancel() 
{
	// TODO: Add your control notification handler code here
 	ShowWindow_(FALSE);
	theApp.inputDlg_->ShowWindow(FALSE);
}

void CContactInfoSearchDlg::OnButtonSearchcinfoSearch() 
{
	// TODO: Add your control notification handler code here
	ShowWindow_(FALSE);
	theApp.inputDlg_->ShowWindow(FALSE);

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
	CString csName;
	m_edtName.GetWindowText(csName);
	std::string strName = Util::StringOp::FromCString(csName);
	if (!csTel.IsEmpty() && !csName.IsEmpty())
	{
		filter += " AND telephoneNumber Like '%";
		filter += Util::StringOp::FromCString(csTel);
		filter += "%'";

		filter += " AND name Like '%";
		filter += Util::StringOp::FromCString(csName);
		filter += "%'";

// 		std::vector<boost::shared_ptr<Data::ContactInfo> > temp = Data::ContactInfo::GetFromDatabase(filter);
// 
// 		for (std::vector<boost::shared_ptr<Data::ContactInfo> >::iterator iter = temp.begin(); iter != temp.end(); ++iter)
// 		{
// 			if ((*iter)->GetContact() && (*iter)->GetContact()->name().find(strName) != std::string::npos)
// 			{
// 				searchResult.push_back(*iter);
// 			}
// 		}
	}
	else if (!csTel.IsEmpty() && csName.IsEmpty())
	{
		filter += " AND telephoneNumber Like '%";
		filter += Util::StringOp::FromCString(csTel);
		filter += "%'";
		
// 		searchResult = Data::ContactInfo::GetFromDatabase(filter);		
	}
	else if (csTel.IsEmpty() && !csName.IsEmpty())
	{
		filter += " AND name Like '%";
		filter += Util::StringOp::FromCString(csName);
		filter += "%'";

// 		std::vector<boost::shared_ptr<Data::ContactInfo> > temp = Data::ContactInfo::GetFromDatabase(filter);
// 		
// 		for (std::vector<boost::shared_ptr<Data::ContactInfo> >::iterator iter = temp.begin(); iter != temp.end(); ++iter)
// 		{
// 			if ((*iter)->GetContact() && (*iter)->GetContact()->name().find(strName) != std::string::npos)
// 			{
// 				searchResult.push_back(*iter);
// 			}
// 		}
	}
	else
	{
//		searchResult = Data::ContactInfo::GetFromDatabase(filter);
	}

// 	((CContactInfoDlg*)m_pParentWnd)->ShowSearchResultInList(searchResult);
	((CContactInfoDlg*)m_pParentWnd)->Search(filter);
	//::SendMessage(m_pParentWnd->m_hWnd, WM_USER + 5, 0, 0);
}
