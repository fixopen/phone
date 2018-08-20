// contactsearchdlg.cpp : implementation file
//

#include "stdafx.h"
#include "..\multimediaphone.h"
#include "contactsearchdlg.h"
#include "../Data/LanguageResource.h"
#include "../Data/Contact.h"
#include "ContactDlg.h"
#include "../Data/SkinStyle.h"

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
	std::string strTemp;
	CString str;
		
	strTemp = Data::LanguageResource::Get(Data::RI_CARD_FIND_TIP);
	str = strTemp.c_str();
	m_sticTip.Create(str, WS_CHILD|WS_VISIBLE, CRect(62, 73, 340, 91), this);
	m_sticTip.SetColor(RGB(0, 0, 0), Data::g_partFrameInRectBackRGB[Data::g_skinstyle]);
// 	m_sticTip.SetAlign(SS_CENTER);

	strTemp = Data::LanguageResource::Get(Data::RI_CARD_FIND_NAME);
	str = strTemp.c_str();
	m_sticName.Create(str, WS_CHILD|WS_VISIBLE, CRect(50, 93, 144, 111), this);
	m_sticName.SetColor(RGB(0, 0, 0), Data::g_partFrameInRectBackRGB[Data::g_skinstyle]);
	m_sticName.SetAlign(SS_CENTER);

	strTemp = Data::LanguageResource::Get(Data::RI_CARD_FIND_NUMBER);
	str = strTemp.c_str();
	m_sticNumber.Create(str, WS_CHILD|WS_VISIBLE, CRect(50, 113, 144, 131), this);
	m_sticNumber.SetColor(RGB(0, 0, 0), Data::g_partFrameInRectBackRGB[Data::g_skinstyle]);
	m_sticNumber.SetAlign(SS_CENTER);

	strTemp = Data::LanguageResource::Get(Data::RI_CARD_FIND_COMPANY);
	str = strTemp.c_str();
	m_sticCompany.Create(str, WS_CHILD|WS_VISIBLE, CRect(50, 133, 144, 151), this);
	m_sticCompany.SetColor(RGB(0, 0, 0), Data::g_partFrameInRectBackRGB[Data::g_skinstyle]);
	m_sticCompany.SetAlign(SS_CENTER);

	strTemp = Data::LanguageResource::Get(Data::RI_CARD_FIND_DUTY);
	str = strTemp.c_str();
	m_sticDuty.Create(str, WS_CHILD|WS_VISIBLE, CRect(50, 153, 144, 171), this);
	m_sticDuty.SetColor(RGB(0, 0, 0), Data::g_partFrameInRectBackRGB[Data::g_skinstyle]);
	m_sticDuty.SetAlign(SS_CENTER);

	m_edtName.Create(WS_CHILD|WS_VISIBLE, CRect(148, 93, 338, 111), this, IDC_EDIT_CONTACTSEARCH_NAME);
	m_edtName.SetLimitText(15);

	m_edtNumber.Create(WS_CHILD|WS_VISIBLE, CRect(148, 113, 338, 131), this, IDC_EDIT_CONTACTSEARCH_NUMBER);
	m_edtNumber.SetLimitText(15);

	m_edtCompany.Create(WS_CHILD|WS_VISIBLE, CRect(148, 133, 338, 151), this, IDC_EDIT_CONTACTSEARCH_COMPANY);
	m_edtCompany.SetLimitText(15);
	
	m_edtDuty.Create(WS_CHILD|WS_VISIBLE, CRect(148, 153, 338, 171), this, IDC_EDIT_CONTACTSEARCH_DUTY);
	m_edtDuty.SetLimitText(15);

	strTemp = Data::LanguageResource::Get(Data::RI_COMN_FINDBTN);
	str = strTemp.c_str();
	m_btnSearch.Create(str, Data::g_buttonArcBMPPARTDILOAGID[0][Data::g_skinstyle], Data::g_buttonArcBMPPARTDILOAGID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(378, 78, 434, 98), this, IDC_BUTTON_CONTACTSEARCH_SEARCH);
	m_btnSearch.SetBackRGB(Data::g_partFrameMainBackRGB[Data::g_skinstyle]);
	
	strTemp = Data::LanguageResource::Get(Data::RI_COMN_CANCELBTN);
	str = strTemp.c_str();
	m_btnCancel.Create(str, Data::g_buttonArcBMPPARTDILOAGID[0][Data::g_skinstyle], Data::g_buttonArcBMPPARTDILOAGID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(378, 103, 434, 123), this, IDC_BUTTON_CONTACTSEARCH_CANCEL);	
	m_btnSearch.SetBackRGB(Data::g_partFrameMainBackRGB[Data::g_skinstyle]);

	m_sticBackground.Create(CRect(34, 40, 448, 184), this, 1);
	
	TextStruct ts[1];
	memset(ts, 0, sizeof(TextStruct) * 1);
	
	ts[0].txtRect = CRect(8, 0, 100, 20);
	ts[0].txtFontSize = 16;
	ts[0].sAlign = DT_LEFT | DT_BOTTOM;
	memcpy(ts[0].sTxt, Data::LanguageResource::Get(Data::RI_CARD_FIND_TITLE).c_str(), Data::LanguageResource::Get(Data::RI_CARD_FIND_TITLE).length());
	
	m_sticBackground.SetTextStruct(ts, 1);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CContactSearchDlg::OnButtonSearchContactCancel() 
{
	// TODO: Add your control notification handler code here
 	ShowWindow_(FALSE);
	theApp.inputDlg_->ShowWindow(FALSE);
}

void CContactSearchDlg::OnButtonSearchContactSearch() 
{
	// TODO: Add your control notification handler code here
	ShowWindow_(FALSE);
	theApp.inputDlg_->ShowWindow(FALSE);

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
		
	((CContactDlg*)m_pParentWnd)->Search(filter);
}