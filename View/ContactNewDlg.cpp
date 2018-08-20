// ContactNewDlg.cpp : implementation file
//

#include "stdafx.h"
#include "../MultimediaPhone.h"
#include "ContactNewDlg.h"
#include "../Data/LanguageResource.h"
#include "ContactDlg.h"
#include "../Data/ContactGroup.h"
#include "../MultimediaPhoneDlg.h"

#include "../Data/ContactInfo.h"
#include "../Data/SkinStyle.h"
#include "../Data/Data.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CContactNewDlg dialog
extern int g_iContactCount;

CContactNewDlg::CContactNewDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CContactNewDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CContactNewDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CContactNewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CContactNewDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CContactNewDlg, CDialog)
	//{{AFX_MSG_MAP(CContactNewDlg)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_CONTACTNEW_OK, OnButtonContactNewOk)
	ON_BN_CLICKED(IDC_BUTTON_CONTACTNEW_CANCEL, OnButtonContactNewCancel)
	ON_BN_CLICKED(IDC_BUTTON_CONTACTNEW_DELETE, OnButtonContactNewDelete)
	ON_BN_CLICKED(IDC_BUTTON_CONTACTNEW_DIALMOBILE1, OnButtonContactNewDialMobile1)
	ON_BN_CLICKED(IDC_BUTTON_CONTACTNEW_DIALMOBILE2, OnButtonContactNewDialMobile2)
	ON_BN_CLICKED(IDC_BUTTON_CONTACTNEW_DIALTEL1, OnButtonContactNewDialTelephone1)
	ON_BN_CLICKED(IDC_BUTTON_CONTACTNEW_DIALTEL2, OnButtonContactNewDialTelephone2)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CContactNewDlg message handlers

BOOL CContactNewDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
    
	m_NewContact=false;
	m_NewContactW=false;
	std::string strTemp;
	CString str;

	strTemp = Data::LanguageResource::Get(Data::RI_CARD_NAMESTC);
	strTemp += ":";
	str = strTemp.c_str();
	m_sticName.Create(str, WS_CHILD|WS_VISIBLE|SS_CENTER, CRect(18, 40*VHEIGHT, 60, 58*VHEIGHT), this);
	m_sticName.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
	m_sticName.SetAlign(SS_CENTER);

	strTemp = Data::LanguageResource::Get(Data::RI_CARD_MOBILESTC);
	str = strTemp.c_str();
	m_sticMobile1.Create(str, WS_CHILD|WS_VISIBLE|SS_CENTER, CRect(18, 61*VHEIGHT, 60, 79*VHEIGHT), this);
	m_sticMobile1.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
	m_sticMobile1.SetAlign(SS_CENTER);

	strTemp = Data::LanguageResource::Get(Data::RI_CARD_MOBILESTC);
	str = strTemp.c_str();
	m_sticMobile2.Create(str, WS_CHILD|WS_VISIBLE|SS_CENTER, CRect(18, 82*VHEIGHT, 60, 100*VHEIGHT), this);
	m_sticMobile2.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
	m_sticMobile2.SetAlign(SS_CENTER);

	strTemp = Data::LanguageResource::Get(Data::RI_CARD_TELPHONESTC);
	str = strTemp.c_str();
	m_sticTelephone1.Create(str, WS_CHILD|WS_VISIBLE|SS_CENTER, CRect(18, 103*VHEIGHT, 60, 121*VHEIGHT), this);
	m_sticTelephone1.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
	m_sticTelephone1.SetAlign(SS_CENTER);
	
	strTemp = Data::LanguageResource::Get(Data::RI_CARD_TELPHONESTC);
	str = strTemp.c_str();
	m_sticTelephone2.Create(str, WS_CHILD|WS_VISIBLE|SS_CENTER, CRect(18, 124*VHEIGHT, 60, 142*VHEIGHT), this);
	m_sticTelephone2.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
	m_sticTelephone2.SetAlign(SS_CENTER);

	strTemp = Data::LanguageResource::Get(Data::RI_RECORD_DIAL);
	str = strTemp.c_str();
	m_btnDialMobile1.Create(str, Data::g_buttonDialBMPID[0][Data::g_skinstyle], Data::g_buttonDialBMPID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(184, 61*VHEIGHT, 230, 80*VHEIGHT), this, IDC_BUTTON_CONTACTNEW_DIALMOBILE1);

	m_btnDialMobile2.Create(str, Data::g_buttonDialBMPID[0][Data::g_skinstyle], Data::g_buttonDialBMPID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(184, 82*VHEIGHT, 230, 101*VHEIGHT), this, IDC_BUTTON_CONTACTNEW_DIALMOBILE2);

	m_btnDialTelephone1.Create(str, Data::g_buttonDialBMPID[0][Data::g_skinstyle], Data::g_buttonDialBMPID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(184, 103*VHEIGHT, 230, 122*VHEIGHT), this, IDC_BUTTON_CONTACTNEW_DIALTEL1);

	m_btnDialTelephone2.Create(str, Data::g_buttonDialBMPID[0][Data::g_skinstyle], Data::g_buttonDialBMPID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(184, 124*VHEIGHT, 230, 143*VHEIGHT), this, IDC_BUTTON_CONTACTNEW_DIALTEL2);

	strTemp = Data::LanguageResource::Get(Data::RI_CARD_EMAILSTC);
	str = strTemp.c_str();
	m_sticEmail.Create(str, WS_CHILD|WS_VISIBLE|SS_CENTER, CRect(18, 145*VHEIGHT, 60, 163*VHEIGHT), this);
	m_sticEmail.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
	m_sticEmail.SetAlign(SS_CENTER);
	
	strTemp = Data::LanguageResource::Get(Data::RI_CARD_WEBSTC);
	str = strTemp.c_str();
	m_sticSite.Create(str, WS_CHILD|WS_VISIBLE|SS_CENTER, CRect(18, 166*VHEIGHT, 60, 184*VHEIGHT), this);
	m_sticSite.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
	m_sticSite.SetAlign(SS_CENTER);

	strTemp = Data::LanguageResource::Get(Data::RI_CARD_GROUPSTC);
	str = strTemp.c_str();
	m_sticGroup.Create(str, WS_CHILD|WS_VISIBLE|SS_CENTER, CRect(243, 40*VHEIGHT, 285, 58*VHEIGHT), this);
	m_sticGroup.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
	m_sticGroup.SetAlign(SS_CENTER);

	strTemp = Data::LanguageResource::Get(Data::RI_CARD_COMPANYSTC);
	strTemp += ":";
	str = strTemp.c_str();
	m_sticCompany.Create(str, WS_CHILD|WS_VISIBLE|SS_CENTER, CRect(243, 61*VHEIGHT, 285, 79*VHEIGHT), this);
	m_sticCompany.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
	m_sticCompany.SetAlign(SS_CENTER);

	strTemp = Data::LanguageResource::Get(Data::RI_CARD_COMPANYADDRESSSTC);
	strTemp += ":";
	str = strTemp.c_str();
	m_sticCompanyAddress.Create(str, WS_CHILD|WS_VISIBLE|SS_CENTER, CRect(243, 82*VHEIGHT, 285, 100*VHEIGHT), this);
	m_sticCompanyAddress.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
	m_sticCompanyAddress.SetAlign(SS_CENTER);

	strTemp = Data::LanguageResource::Get(Data::RI_CARD_POSTCODESTC);
	str = strTemp.c_str();
	m_sticPostCode.Create(str, WS_CHILD|WS_VISIBLE|SS_CENTER, CRect(243, 103*VHEIGHT, 285, 121*VHEIGHT), this);
	m_sticPostCode.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
	m_sticPostCode.SetAlign(SS_CENTER);

	strTemp = Data::LanguageResource::Get(Data::RI_CARD_DEPARTSTC);
	strTemp += ":";
	str = strTemp.c_str();
	m_sticDepartment.Create(str, WS_CHILD|WS_VISIBLE|SS_CENTER, CRect(243, 124*VHEIGHT, 285, 142*VHEIGHT), this);
	m_sticDepartment.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
	m_sticDepartment.SetAlign(SS_CENTER);

	strTemp = Data::LanguageResource::Get(Data::RI_CARD_TITLESTC);
	strTemp += ":";
	str = strTemp.c_str();
	m_sticDuty.Create(str, WS_CHILD|WS_VISIBLE|SS_CENTER, CRect(243, 145*VHEIGHT, 285, 163*VHEIGHT), this);
	m_sticDuty.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
	m_sticDuty.SetAlign(SS_CENTER);
	
	strTemp = Data::LanguageResource::Get(Data::RI_CARD_MOTOSTC);
	str = strTemp.c_str();
	m_sticMemo.Create(str, WS_CHILD|WS_VISIBLE|SS_CENTER, CRect(240, 166*VHEIGHT, 285, 184*VHEIGHT), this);
	m_sticMemo.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
	m_sticMemo.SetAlign(SS_CENTER);

	m_cmbType.Create(WS_CHILD|WS_VISIBLE, CRect(180, 40, 241, 120), this, IDC_COMBOBOX_CONTACTNEW_TYPE);
	m_cmbGroup.Create(WS_CHILD|WS_VISIBLE, CRect(286, 40, 400, 130), this, IDC_COMBOBOX_CONTACTNEW_GROUP);
			
	m_cmbType.AddString(Util::StringOp::ToCString((Data::LanguageResource::Get(Data::RI_CARD_COMNSTC))));
   	m_cmbType.AddString(Util::StringOp::ToCString((Data::LanguageResource::Get(Data::RI_CARD_VIPSTC))));
	m_cmbType.AddString(Util::StringOp::ToCString((Data::LanguageResource::Get(Data::RI_CARD_BLACKLISTSTC))));

	m_edtName.Create(WS_CHILD|WS_VISIBLE, CRect(61, 40*VHEIGHT, 175, 58*VHEIGHT), this, IDC_EDIT_CONTACTNEW_NAME);
	m_edtName.SetWindowText(L"");
	m_edtName.SetLimitText(15);
	m_edtMobile1.Create(WS_CHILD|WS_VISIBLE|WS_HSCROLL|ES_AUTOHSCROLL, CRect(61, 61*VHEIGHT, 175, 79*VHEIGHT), this, IDC_EDIT_CONTACTNEW_MOBILE1);
	m_edtMobile1.SetLimitText(32);
	m_edtMobile1.SetLimitDiagital();
	m_edtMobile2.Create(WS_CHILD|WS_VISIBLE|WS_HSCROLL|ES_AUTOHSCROLL, CRect(61, 82*VHEIGHT, 175, 100*VHEIGHT), this, IDC_EDIT_CONTACTNEW_MOBILE2);
	m_edtMobile2.SetLimitText(32);
	m_edtMobile2.SetLimitDiagital();
	m_edtTelephone1.Create(WS_CHILD|WS_VISIBLE|WS_HSCROLL|ES_AUTOHSCROLL, CRect(61, 103*VHEIGHT, 175, 121*VHEIGHT), this, IDC_EDIT_CONTACTNEW_TELEPHONE1);
	m_edtTelephone1.SetLimitText(32);
	m_edtTelephone1.SetLimitDiagital();
	m_edtTelephone2.Create(WS_CHILD|WS_VISIBLE|WS_HSCROLL|ES_AUTOHSCROLL, CRect(61, 124*VHEIGHT, 175, 142*VHEIGHT), this, IDC_EDIT_CONTACTNEW_TELEPHONE2);
	m_edtTelephone2.SetLimitText(32);
	m_edtTelephone2.SetLimitDiagital();
	m_edtEmail.Create(WS_CHILD|WS_VISIBLE|ES_AUTOHSCROLL, CRect(61, 145*VHEIGHT, 175, 163*VHEIGHT), this, IDC_EDIT_CONTACTNEW_EMAIL);
	m_edtEmail.SetLimitText(30);
	m_edtSite.Create(WS_CHILD|WS_VISIBLE|ES_AUTOHSCROLL, CRect(61, 166*VHEIGHT, 175, 184*VHEIGHT), this, IDC_EDIT_CONTACTNEW_SITE);
	m_edtSite.SetLimitText(30);

	m_edtCompany.Create(WS_CHILD|WS_VISIBLE|ES_AUTOHSCROLL, CRect(286, 61*VHEIGHT, 400, 79*VHEIGHT), this, IDC_EDIT_CONTACTNEW_COMPANY);
	m_edtCompany.SetLimitText(30);
	m_edtCompanyAddress.Create(WS_CHILD|WS_VISIBLE|ES_AUTOHSCROLL, CRect(286, 82*VHEIGHT, 400, 100*VHEIGHT), this, IDC_EDIT_CONTACTNEW_ADDRESS);
	m_edtCompanyAddress.SetLimitText(30);
	m_edtPostCode.Create(WS_CHILD|WS_VISIBLE, CRect(286, 103*VHEIGHT, 400, 121*VHEIGHT), this, IDC_EDIT_CONTACTNEW_POSTCODE);
	m_edtPostCode.SetLimitText(6);
	m_edtDepartment.Create(WS_CHILD|WS_VISIBLE|ES_AUTOHSCROLL, CRect(286, 124*VHEIGHT, 400, 142*VHEIGHT), this, IDC_EDIT_CONTACTNEW_DEPARTMENT);
	m_edtDepartment.SetLimitText(30);
	m_edtDuty.Create(WS_CHILD|WS_VISIBLE, CRect(286, 145*VHEIGHT, 400, 163*VHEIGHT), this, IDC_EDIT_CONTACTNEW_DUTY);
	m_edtDuty.SetLimitText(15);
	m_edtMemo.Create(WS_CHILD|WS_VISIBLE|ES_AUTOHSCROLL, CRect(286, 166*VHEIGHT, 400, 184*VHEIGHT), this, IDC_EDIT_CONTACTNEW_MEMO);
	m_edtMemo.SetLimitText(30);

	strTemp = Data::LanguageResource::Get(Data::RI_COMN_OKBTN);
	str = strTemp.c_str();
	m_btnOk.Create(str, Data::g_buttonArcBMPALLDIALOGID[0][Data::g_skinstyle], Data::g_buttonArcBMPALLDIALOGID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(415, 41*VHEIGHT, 471, 61*VHEIGHT-2), this, IDC_BUTTON_CONTACTNEW_OK);
	m_btnOk.SetBackRGB(Data::g_allFramAngleBackLineRGB[Data::g_skinstyle]);

	strTemp = Data::LanguageResource::Get(Data::RI_COMN_CANCELBTN);
	str = strTemp.c_str();
	m_btnCancel.Create(str, Data::g_buttonArcBMPALLDIALOGID[0][Data::g_skinstyle], Data::g_buttonArcBMPALLDIALOGID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(415, 66*VHEIGHT, 471, 86*VHEIGHT), this, IDC_BUTTON_CONTACTNEW_CANCEL);
	m_btnCancel.SetBackRGB(Data::g_allFramAngleBackLineRGB[Data::g_skinstyle]);

	strTemp = Data::LanguageResource::Get(Data::RI_CARD_DELETECARDBTN);
	str = strTemp.c_str();
	m_btnDelete.Create(str, Data::g_buttonArcBMPALLDIALOGID[0][Data::g_skinstyle], Data::g_buttonArcBMPALLDIALOGID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(415, 91*VHEIGHT, 471, 111*VHEIGHT), this, IDC_BUTTON_CONTACTNEW_DELETE);
	m_btnDelete.SetBackRGB(Data::g_allFramAngleBackLineRGB[Data::g_skinstyle]);

	m_sticBackground.Create(CRect(0, 0, 480, 204*VHEIGHT), this, 1);
	m_pPinYinIndex = LoadPinYinIndex();
	m_pContact = boost::shared_ptr<Data::Contact>();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CContactNewDlg::SetButtonDefaultColor(CCEButtonST* button)
{
	button->SetColor(CCEButtonST::BTNST_COLOR_BK_IN, RGB(248,214,147));
	button->SetColor(CCEButtonST::BTNST_COLOR_BK_OUT, RGB(248,214,147));
	button->SetColor(CCEButtonST::BTNST_COLOR_BK_FOCUS, RGB(248,214,147));
}

void CContactNewDlg::SetButtonSelectedColor(CCEButtonST* button)
{
	button->SetColor(CCEButtonST::BTNST_COLOR_BK_IN, RGB(248,214,147));
	button->SetColor(CCEButtonST::BTNST_COLOR_BK_OUT, RGB(248,214,147));
	button->SetColor(CCEButtonST::BTNST_COLOR_BK_FOCUS, RGB(248,214,147));
}

void CContactNewDlg::AddGroupItems(void)
{
	m_cmbGroup.ResetContent();
	std::string strTemp;
	strTemp = Data::LanguageResource::Get(Data::RI_CARD_ALLCARDSTC);
	CString str = Util::StringOp::ToCString(strTemp);
	m_cmbGroup.AddString(str);
	m_cmbGroup.SetItemData(0, 0);
	std::vector<boost::shared_ptr<Data::ContactGroup> > result = Data::ContactGroup::GetFromDatabase("");
	for (int i = 0; i < result.size(); ++i)
	{
		if(result[i]->name() != strTemp)
		{
			m_cmbGroup.AddString(Util::StringOp::ToCString((result[i]->name())));
			m_cmbGroup.SetItemData(i + 1, result[i]->id());
		}
	}
}

void CContactNewDlg::OnButtonContactNewOk() //保存
{
	// TODO: Add your control notification handler code here	
	CString s;
	m_edtName.GetWindowText(s);
	if (s.IsEmpty())
	{	
		// add by yy 20111027
		CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
		std::string strTemp = Data::LanguageResource::Get(Data::RI_CONTACTNEW);
        main->m_pCCWarningDlg->SetDelTip(strTemp.c_str());		
		main->m_pCCWarningDlg->ShowWindow_(SW_SHOW);
		return;
	}
	CString name_ = s;
	std::string filter("");
	std::string filter_("");

	if (!m_pContact)
	{
		m_pContact = boost::shared_ptr<Data::Contact>(new Data::Contact);
	}	

	m_pContact->name(ReplaceSingleQuotation(Util::StringOp::FromCString(s)));
	m_pContact->alias(GetAlias(m_pContact->name()));

	m_edtMobile1.GetWindowText(s);

	if(!s.IsEmpty())
	{
		filter += "telephoneNumber like '%";
		filter += Util::StringOp::FromCString(s);
		filter += "'";
	}
	filter_ += "mobilesTelephone = '";
	filter_ += Util::StringOp::FromCString(s);
	filter_ += "'";

	m_pContact->mobilesTelephone(Data::TelephoneNumber(ReplaceSingleQuotation(Util::StringOp::FromCString(s))));
	m_edtMobile2.GetWindowText(s);
	
	if(!s.IsEmpty())
	{
		if(!filter.empty())
		{
			filter += "or telephoneNumber like '%";
			filter += Util::StringOp::FromCString(s);
			filter += "'";
		}
		else
		{
			filter += "telephoneNumber like '%";
			filter += Util::StringOp::FromCString(s);
			filter += "'";
		}
	}
	filter_ += "and mobilesTelephone2 = '";
	filter_ += Util::StringOp::FromCString(s);
	filter_ += "'";

	m_pContact->mobilesTelephone2(Data::TelephoneNumber(ReplaceSingleQuotation(Util::StringOp::FromCString(s))));
	m_edtTelephone1.GetWindowText(s);

	if(!s.IsEmpty())
	{
		if(!filter.empty())
		{
			filter += "or telephoneNumber = '";
			filter += Util::StringOp::FromCString(s);
			filter += "'";
		}
		else
		{
			filter += "telephoneNumber = '";
			filter += Util::StringOp::FromCString(s);
			filter += "'";
		}
	}
	filter_ += "and worksTelephone = '";
	filter_ += Util::StringOp::FromCString(s);
	filter_ += "'";

	m_pContact->worksTelephone(Data::TelephoneNumber(ReplaceSingleQuotation(Util::StringOp::FromCString(s))));
	m_edtTelephone2.GetWindowText(s);

	if(!s.IsEmpty())
	{
		if(!filter.empty())
		{
			filter += "or telephoneNumber = '";
			filter += Util::StringOp::FromCString(s);
			filter += "'";
		}
		else
		{
			filter += "telephoneNumber = '";
			filter += Util::StringOp::FromCString(s);
			filter += "'";
		}
	}
	filter_ += "and worksTelephone2 = '";
	filter_ += Util::StringOp::FromCString(s);
	filter_ += "'";

	m_pContact->worksTelephone2(Data::TelephoneNumber(ReplaceSingleQuotation(Util::StringOp::FromCString(s))));
	m_edtEmail.GetWindowText(s);
	m_pContact->email(ReplaceSingleQuotation(Util::StringOp::FromCString(s)));
	m_edtSite.GetWindowText(s);
	m_pContact->homesite(ReplaceSingleQuotation(Util::StringOp::FromCString(s)));
	m_edtCompany.GetWindowText(s);
	m_pContact->company(ReplaceSingleQuotation(Util::StringOp::FromCString(s)));
	m_edtCompanyAddress.GetWindowText(s);
	m_pContact->companyAddress(ReplaceSingleQuotation(Util::StringOp::FromCString(s)));
	m_edtPostCode.GetWindowText(s);
	m_pContact->companyPostCode(ReplaceSingleQuotation(Util::StringOp::FromCString(s)));
	m_edtDepartment.GetWindowText(s);
	m_pContact->department(ReplaceSingleQuotation(Util::StringOp::FromCString(s)));
	m_edtDuty.GetWindowText(s);
	m_pContact->duty(ReplaceSingleQuotation(Util::StringOp::FromCString(s)));
	m_edtMemo.GetWindowText(s);
	m_pContact->memo(ReplaceSingleQuotation(Util::StringOp::FromCString(s)));

	if (m_cmbType.GetCurSel() == CB_ERR)
	{
		m_pContact->type(Data::ctNormal);
	}
	else
	{
		int index = m_cmbType.GetCurSel();
		m_pContact->type(Data::ContactType(index));
	}
	
	if (m_cmbGroup.GetCurSel() == CB_ERR)
	{
		m_pContact->groupId(0);
	}
	else
	{
		int gId = m_cmbGroup.GetItemData(m_cmbGroup.GetCurSel());
		m_pContact->groupId(gId);
	}

	if (m_pContact->id() == 0)
	{
		m_pContact->Insert();
		++g_iContactCount;
	}
	else
	{   
		m_pContact->Update();
	}

	if(!filter.empty())
	{
		std::vector<boost::shared_ptr<Data::ContactInfo> > ContactInfoResult=Data::ContactInfo::GetFromDatabase(filter);
		if(ContactInfoResult.size()>0)
		{
			Data::DataAccess<Data::ContactInfo>::startBegin();
			for (int index=0;index<ContactInfoResult.size();index++)
			{   
				ContactInfoResult[index]->contactId(m_pContact->id());
				ContactInfoResult[index]->name(m_pContact->name());
				ContactInfoResult[index]->Update();
			}
			Data::DataAccess<Data::ContactInfo>::endCommit();
		}
		
		std::vector<boost::shared_ptr<Data::SoundSegment> > SoundSegmentResult=Data::SoundSegment::GetFromDatabase(filter);
		if (SoundSegmentResult.size()>0)
		{   
			Data::DataAccess<Data::SoundSegment>::startBegin();
			for(int index=0;index<SoundSegmentResult.size();index++)
			{
				SoundSegmentResult[index]->contactId(m_pContact->id());	
				SoundSegmentResult[index]->name(m_pContact->name());
				SoundSegmentResult[index]->Update();
			}
			Data::DataAccess<Data::SoundSegment>::endCommit();
		}
	}

	ShowWindow(FALSE);
	//关闭输入法
	KeyBoardOff();
	
	((CContactDlg*)m_pParentWnd)->ShowItemsInList();
	((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactInfoDlg->ShowItemsInList();
    ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pSoundDlg->ShowItemsInList();
}

void CContactNewDlg::OnButtonContactNewCancel() 
{
	// TODO: Add your control notification handler code here
	ShowWindow(FALSE);

	//关闭输入法
	KeyBoardOff();
	
}

void CContactNewDlg::OnButtonContactNewDelete() 
{
	// TODO: Add your control notification handler code here
	if (m_pContact->id() > 0)
	{	
		CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
		if (main->m_pSettingDlg->m_pSetting->isAdmin() &&\
			!main->m_pSettingDlg->m_bLogin)
		{	
			std::string pw = main->m_pSettingDlg->m_pSetting->adminPassword();
			if (!pw.empty())
			{	
				//add by qi 20111104
				//密码为空不用输入密码
				main->m_pDeleteTipDlg->SetPasswordModel(true);
				main->m_pDeleteTipDlg->SetPassword(Util::StringOp::ToCString(pw));
			}
		}

		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetHWnd(m_hWnd);

		std::string strTemp = Data::LanguageResource::Get(Data::RI_DELETETIP_CONTACT);

		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetDelTip(strTemp.c_str());

		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->ShowWindow(TRUE);
	}
}

void CContactNewDlg::SetContact(boost::shared_ptr<Data::Contact> contact)
{
	m_pContact = contact;
	std::string strTemp;
	if (contact->id() == 0)
	{
		strTemp = Data::LanguageResource::Get(Data::RI_CARD_NEWCARDSTC);
		m_btnDelete.ShowWindow(FALSE);
	}
	else
	{
		strTemp = Data::LanguageResource::Get(Data::RI_CARD_EDITCARDSTC);
		m_btnDelete.ShowWindow(TRUE);
	}
	TextStruct ts[1];
	memset(ts, 0, sizeof(TextStruct) * 1);
	ts[0].txtRect = CRect(8, 0, 100, 20*VHEIGHT);
	ts[0].txtFontSize = 16;
	ts[0].sAlign = DT_LEFT | DT_BOTTOM;
	memcpy(ts[0].sTxt, strTemp.c_str(), strTemp.length());
	m_sticBackground.SetTextStruct(ts, 1);
	m_edtName.SetWindowText(Util::StringOp::ToCString((contact->name())));
	if (contact->id()==0)
	{
    	if (contact->mobilesTelephone().number()!="")
		{
			m_NewContact=true;
			
		}else if (contact->worksTelephone().number()!="")
		{
			m_NewContactW=true;
		}
	}
	m_edtMobile1.SetWindowText(Util::StringOp::ToCString((contact->mobilesTelephone().number())));
	m_edtMobile2.SetWindowText(Util::StringOp::ToCString((contact->mobilesTelephone2().number())));
	m_edtTelephone1.SetWindowText(Util::StringOp::ToCString((contact->worksTelephone().number())));
	m_edtTelephone2.SetWindowText(Util::StringOp::ToCString((contact->worksTelephone2().number())));
	m_edtEmail.SetWindowText(Util::StringOp::ToCString((contact->email())));
	m_edtSite.SetWindowText(Util::StringOp::ToCString((contact->homesite())));
	m_edtCompany.SetWindowText(Util::StringOp::ToCString((contact->company())));
	m_edtCompanyAddress.SetWindowText(Util::StringOp::ToCString((contact->companyAddress())));
	m_edtPostCode.SetWindowText(Util::StringOp::ToCString((contact->companyPostCode())));
	m_edtDepartment.SetWindowText(Util::StringOp::ToCString((contact->department())));
	m_edtDuty.SetWindowText(Util::StringOp::ToCString((contact->duty())));
	m_edtMemo.SetWindowText(Util::StringOp::ToCString((contact->memo())));
	
	if (m_cmbType.SetCurSel(contact->type()) == CB_ERR)
	{
		m_cmbType.SetCurSel(Data::ctNormal);
	}

	AddGroupItems();
	
	int id = contact->groupId();

	std::string filter;

	filter = "id = " + Util::StringOp::FromInt(id);

	std::vector<boost::shared_ptr<Data::ContactGroup> > result = Data::ContactGroup::GetFromDatabase(filter);

	if (!result.empty())
	{
		CString group = Util::StringOp::ToCString(result[0]->name());
		CString txt;
		for (int i = 0; i < m_cmbGroup.GetCount(); i++)
		{
			m_cmbGroup.GetLBText(i, txt);
			if (group == txt)
			{
				m_cmbGroup.SetCurSel(i);

				break;
			}
			else
			{
				m_cmbGroup.SetCurSel(0);
			}
		}
	}
	else
	{
		m_cmbGroup.SetCurSel(0);
	}
}

char* CContactNewDlg::LoadPinYinIndex(void)
{
	CFile m_pHexFile;   

	m_pHexFile.Open(_T("/HIVE/RES_DAT/firstchar.txt"), CFile::modeRead | CFile::typeBinary);   

	if(!m_pHexFile)   
	{   
		return NULL;   
	}   
	int nFilelen = m_pHexFile.GetLength();   
	char* szBuf;   
	szBuf = new char[nFilelen];   
	int nRet = m_pHexFile.Read(szBuf,nFilelen);   
	if(nRet <= 0)   
	{   
		return NULL;   
	}   
	m_pHexFile.Close();  
	return szBuf;
}

std::string CContactNewDlg::GetAlias(std::string name)
{
	if (m_pPinYinIndex == NULL)
	{
		return "";
	}

	std::string alias;
	for (int i = 0; i < name.size(); ++i)
	{
		if ((unsigned char)name[i] > (unsigned char)127)
		{
			unsigned int index = ((unsigned char)name[i] - 0xb0) * 94 + ((unsigned char)name[i+1] - 0xa1);
			alias += m_pPinYinIndex[index];	
			++i;
		}
		else
		{
			if (name[i] >= 'A' && name[i] <= 'Z')
			{
				alias += char(name[i] + 32);
			}
			else
			{
				alias += name[i];
			}
		}
	}
	return alias;
}

std::string CContactNewDlg::ReplaceSingleQuotation(std::string& field)
{
	for (int i = 0; i < field.length(); ++i)
	{
		if (field[i] == '\'')
		{
			field[i] = '\"';
		}
	}
	return field;
}

void CContactNewDlg::OnButtonContactNewDialMobile1()
{
	CString mobile;
	m_edtMobile1.GetWindowText(mobile);
	if (mobile.IsEmpty())
	{
		return;
	}
	((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pTelephoneDlg->DialContact(Util::StringOp::FromCString(mobile), m_pContact->id());
}

void CContactNewDlg::OnButtonContactNewDialMobile2()
{
	CString mobile;
	m_edtMobile2.GetWindowText(mobile);
	if (mobile.IsEmpty())
	{
		return;
	}
	((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pTelephoneDlg->DialContact(Util::StringOp::FromCString(mobile), m_pContact->id());
}

void CContactNewDlg::OnButtonContactNewDialTelephone1()
{
	CString telephone;

	m_edtTelephone1.GetWindowText(telephone);

	if (telephone.IsEmpty())
	{
		return;
	}
	((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pTelephoneDlg->DialContact(Util::StringOp::FromCString(telephone), m_pContact->id());
}

void CContactNewDlg::OnButtonContactNewDialTelephone2()
{
	CString telephone;

	m_edtTelephone2.GetWindowText(telephone);

	if (telephone.IsEmpty())
	{
		return;
	}

	((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pTelephoneDlg->DialContact(Util::StringOp::FromCString(telephone), m_pContact->id());
}

LRESULT CContactNewDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	switch (message)
	{
	case WM_DELETESELITEM:
		{
			std::string filter="contactId= ";
			
			filter+=Util::StringOp::FromInt(m_pContact->id());
			
			std::vector<boost::shared_ptr<Data::ContactInfo> > ContactInfoResult=Data::ContactInfo::GetFromDatabase(filter);
			
			if(ContactInfoResult.size()>0)
			{
				Data::DataAccess<Data::ContactInfo>::startBegin();
				
				for (int index=0;index<ContactInfoResult.size();index++)
				{
					ContactInfoResult[index]->name("");
					
					ContactInfoResult[index]->Update();
				}
				Data::DataAccess<Data::ContactInfo>::endCommit();
			}
			std::vector<boost::shared_ptr<Data::SoundSegment> > SoundSegmentResult=Data::SoundSegment::GetFromDatabase(filter);

		   if (SoundSegmentResult.size()>0)
		   {   
			   Data::DataAccess<Data::SoundSegment>::startBegin();
			   
			   for(int index=0;index<SoundSegmentResult.size();index++)
			   {
				   SoundSegmentResult[index]->name("");
				   
				   SoundSegmentResult[index]->Update();
			   }
			   
			   Data::DataAccess<Data::SoundSegment>::endCommit();
		   }
		   
		   m_pContact->Remove();
		   --g_iContactCount;
		   ((CContactDlg*)m_pParentWnd)->ShowItemsInList();
		   
		   ShowWindow(FALSE);
	}

	break;

	case WM_CHANGELANGUAGE:

       ChangeLanguage();

     break;

	case WM_OK:

		  OnButtonContactNewOk();
		  
		break;

	case WM_CANCEL:
		if (((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactNewDlg->IsWindowVisible())
		{
			 OnButtonContactNewCancel();
		}
	   
		break;
	case WM_DELETE:
		if (((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactNewDlg->IsWindowVisible())
		{
			if (m_pContact->id()==0)
			{
			 return 0;
			}
			else
			{
			   
				OnButtonContactNewDelete();
			}
		
		}
		break;
	case WM_D:
		OnButtonContactNewDelete();
		break;
	}
	return CDialog::WindowProc(message, wParam, lParam);
}
void CContactNewDlg::ChangeLanguage()
{
	std::string strTemp;
	CString str;
	strTemp = Data::LanguageResource::Get(Data::RI_CARD_NAMESTC);
	strTemp += ":";
	str = strTemp.c_str();
    m_sticName.SetWindowText(str);

	strTemp = Data::LanguageResource::Get(Data::RI_CARD_MOBILESTC);
	str = strTemp.c_str();
	m_sticMobile1.SetWindowText(str);

    strTemp = Data::LanguageResource::Get(Data::RI_CARD_MOBILESTC);
	str = strTemp.c_str();
	m_sticMobile2.SetWindowText(str);

	strTemp = Data::LanguageResource::Get(Data::RI_CARD_TELPHONESTC);
	str = strTemp.c_str();
	m_sticTelephone1.SetWindowText(str);

	strTemp = Data::LanguageResource::Get(Data::RI_CARD_TELPHONESTC);
	str = strTemp.c_str();
	m_sticTelephone2.SetWindowText(str);

	strTemp = Data::LanguageResource::Get(Data::RI_RECORD_DIAL);
	str = strTemp.c_str();
	m_btnDialMobile1.SetWindowText(str);
	m_btnDialMobile2.SetWindowText(str);
	m_btnDialTelephone1.SetWindowText(str);
	m_btnDialTelephone2.SetWindowText(str);

	strTemp = Data::LanguageResource::Get(Data::RI_CARD_EMAILSTC);
	str = strTemp.c_str();
	m_sticEmail.SetWindowText(str);

	strTemp = Data::LanguageResource::Get(Data::RI_CARD_WEBSTC);
	str = strTemp.c_str();
	m_sticSite.SetWindowText(str);

	strTemp = Data::LanguageResource::Get(Data::RI_CARD_GROUPSTC);
	str = strTemp.c_str();
    m_sticGroup.SetWindowText(str);

	strTemp = Data::LanguageResource::Get(Data::RI_CARD_COMPANYSTC);
	strTemp += ":";
	str = strTemp.c_str();
	m_sticCompany.SetWindowText(str);

	strTemp = Data::LanguageResource::Get(Data::RI_CARD_COMPANYADDRESSSTC);
	strTemp += ":";
	str = strTemp.c_str();
	m_sticCompanyAddress.SetWindowText(str);

	strTemp = Data::LanguageResource::Get(Data::RI_CARD_POSTCODESTC);
	str = strTemp.c_str();
	m_sticPostCode.SetWindowText(str);

	strTemp = Data::LanguageResource::Get(Data::RI_CARD_DEPARTSTC);
	strTemp += ":";
	str = strTemp.c_str();
	m_sticDepartment.SetWindowText(str);

	strTemp = Data::LanguageResource::Get(Data::RI_CARD_TITLESTC);
	strTemp += ":";
	str = strTemp.c_str();
	m_sticDuty.SetWindowText(str);

	strTemp = Data::LanguageResource::Get(Data::RI_CARD_MOTOSTC);
	str = strTemp.c_str();
	m_sticMemo.SetWindowText(str);

	strTemp = Data::LanguageResource::Get(Data::RI_COMN_OKBTN);
	str = strTemp.c_str();
	m_btnOk.SetWindowText(str);

	strTemp = Data::LanguageResource::Get(Data::RI_COMN_CANCELBTN);
	str = strTemp.c_str();
	m_btnCancel.SetWindowText(str);

	strTemp = Data::LanguageResource::Get(Data::RI_CARD_DELETECARDBTN);
	str = strTemp.c_str();
	m_btnDelete.SetWindowText(str);
   
	m_cmbType.ResetContent();
	m_cmbType.AddString(Util::StringOp::ToCString((Data::LanguageResource::Get(Data::RI_CARD_COMNSTC))));
    m_cmbType.AddString(Util::StringOp::ToCString((Data::LanguageResource::Get(Data::RI_CARD_VIPSTC))));
	m_cmbType.AddString(Util::StringOp::ToCString((Data::LanguageResource::Get(Data::RI_CARD_BLACKLISTSTC))));
	}
