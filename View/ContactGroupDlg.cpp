// contactgroupdlg.cpp : implementation file
//

#include "stdafx.h"
#include "..\MultimediaPhone.h"
#include "contactgroupdlg.h"
#include "../Data/LanguageResource.h"
#include "ContactDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CContactGroupDlg dialog


CContactGroupDlg::CContactGroupDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CContactGroupDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CContactGroupDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CContactGroupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CContactGroupDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CContactGroupDlg, CDialog)
	//{{AFX_MSG_MAP(CContactGroupDlg)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_CONTACTGROUP_OK, OnButtonContactGroupOk)
	ON_BN_CLICKED(IDC_BUTTON_CONTACTGROUP_CANCEL, OnButtonContactGroupCancel)
	ON_BN_CLICKED(IDC_BUTTON_CONTACTGROUP_DELETE, OnButtonContactGroupDelete)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CContactGroupDlg message handlers

BOOL CContactGroupDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	std::string strTemp = Data::LanguageResource::Get(Data::RI_CARD_EDITTYPEBTN);
	CString str = strTemp.c_str();

	m_sticTitle.Create(str, WS_CHILD|WS_VISIBLE, CRect(0, 0, 429, 20), this);
	m_sticTitle.SetColor(RGB(0, 0, 0), RGB(78, 165, 229));
	m_sticTitle.SetLeftMargin(25);

	m_sticSepLine2.Create(_T(""), WS_CHILD|WS_VISIBLE, CRect(429, 0, 430, 204), this);
	m_sticSepLine2.SetColor(RGB(255, 255, 255), RGB(255, 255, 255));
	
	strTemp = Data::LanguageResource::Get(Data::RI_CARD_TYPENAME);
	str = strTemp.c_str();
	m_sticName.Create(str, WS_CHILD|WS_VISIBLE, CRect(6, 25, 66, 40), this);
	m_sticName.SetColor(RGB(0, 0, 0), RGB(255, 255, 255));

	strTemp = Data::LanguageResource::Get(Data::RI_CARD_RING);
	str = strTemp.c_str();
	m_sticRing.Create(str, WS_CHILD|WS_VISIBLE, CRect(6, 45, 66, 65), this);
	m_sticRing.SetColor(RGB(0, 0, 0), RGB(255, 255, 255));

	strTemp = Data::LanguageResource::Get(Data::RI_CARD_SOUNDTIP);
	str = strTemp.c_str();
	m_sticSoudTip.Create(str, WS_CHILD|WS_VISIBLE, CRect(6, 65, 66, 85), this);
	m_sticSoudTip.SetColor(RGB(0, 0, 0), RGB(255, 255, 255));

	m_edtName.Create(WS_CHILD|WS_VISIBLE, CRect(70, 25, 188, 45), this, IDC_EDIT_CONTACTGROUP_NAME);
	m_cmbRing.Create(WS_CHILD|WS_VISIBLE, CRect(66,45,188,145), this, IDC_COMBOBOX_CONTACTGROUP_RING);
	m_cmbSoundTip.Create(WS_CHILD|WS_VISIBLE, CRect(66,65,188,165), this, IDC_COMBOBOX_CONTACTGROUP_SOUNDTIP);

	strTemp = Data::LanguageResource::Get(Data::RI_CARD_OPERATORSTC);
	str = strTemp.c_str();
	m_sticOperation.Create(str, WS_CHILD|WS_VISIBLE, CRect(430, 0, 480, 20), this);
	m_sticOperation.SetColor(RGB(0, 0, 0), RGB(78, 165, 229));
	m_sticOperation.SetLeftMargin(10);

	strTemp = Data::LanguageResource::Get(Data::RI_COMN_OKBTN);
	str = strTemp.c_str();
	m_btnOk.Create(str, WS_CHILD|WS_VISIBLE, CRect(430, 20, 480, 40), this, IDC_BUTTON_CONTACTGROUP_OK);
	SetButtonDefaultColor(&m_btnOk);

	strTemp = Data::LanguageResource::Get(Data::RI_COMN_CANCELBTN);
	str = strTemp.c_str();
	m_btnCancel.Create(str, WS_CHILD|WS_VISIBLE, CRect(430, 40, 480, 60), this, IDC_BUTTON_CONTACTGROUP_CANCEL);
	SetButtonDefaultColor(&m_btnCancel);

	strTemp = Data::LanguageResource::Get(Data::RI_CARD_DELETETYPEBTN);
	str = strTemp.c_str();
	m_btnDelete.Create(str, WS_CHILD|WS_VISIBLE, CRect(430, 184, 480, 204), this, IDC_BUTTON_CONTACTGROUP_DELETE);
	SetButtonDefaultColor(&m_btnDelete);
	
	m_pContactGroup = boost::shared_ptr<Data::ContactGroup>();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CContactGroupDlg::SetButtonDefaultColor(CCEButtonST* button)
{
	button->SetColor(CCEButtonST::BTNST_COLOR_BK_IN, RGB(248,214,147));
	button->SetColor(CCEButtonST::BTNST_COLOR_BK_OUT, RGB(248,214,147));
	button->SetColor(CCEButtonST::BTNST_COLOR_BK_FOCUS, RGB(248,214,147));
}

void CContactGroupDlg::SetButtonSelectedColor(CCEButtonST* button)
{
	button->SetColor(CCEButtonST::BTNST_COLOR_BK_IN, RGB(248,214,147));
	button->SetColor(CCEButtonST::BTNST_COLOR_BK_OUT, RGB(248,214,147));
	button->SetColor(CCEButtonST::BTNST_COLOR_BK_FOCUS, RGB(248,214,147));
}

void CContactGroupDlg::AddRingTipItems(void)
{
// 	for (int i = 0; i < m_cmbRing.GetCount(); i++)
// 	{
// 	   m_cmbRing.DeleteString(i);
// 	}
 	m_cmbRing.ResetContent();
	
	std::string filter;
	filter = "type = "	+ Util::StringOp::FromInt(Data::sstRing);
	std::vector<boost::shared_ptr<Data::SoundSegment> > ringresult = Data::SoundSegment::GetFromDatabase(filter);
	if (!ringresult.empty())
	{
		for (int j = 0; j < ringresult.size(); ++j)
		{
			m_cmbRing.AddString(CString(ringresult[j]->filename().c_str()));
		}
	}
//=======================================================
// 	for (int m = 0; m < m_cmbSoundTip.GetCount(); m++)
// 	{
// 	   m_cmbSoundTip.DeleteString(m);
// 	}
	m_cmbSoundTip.ResetContent();
	filter = "type = "	+ Util::StringOp::FromInt(Data::sstTips);
	std::vector<boost::shared_ptr<Data::SoundSegment> > tipresult = Data::SoundSegment::GetFromDatabase(filter);
	if (!tipresult.empty())
	{
		for (int n = 0; n < tipresult.size(); ++n)
		{
			m_cmbSoundTip.AddString(CString(tipresult[n]->filename().c_str()));
		}
	}
}

void CContactGroupDlg::OnButtonContactGroupOk() 
{
	// TODO: Add your control notification handler code here
	ShowWindow(FALSE);

	if (!m_pContactGroup)
	{
		m_pContactGroup = boost::shared_ptr<Data::ContactGroup>(new Data::ContactGroup);
	}

	CString s;
	m_edtName.GetWindowText(s);
	m_pContactGroup->name(Util::StringOp::FromCString(s));

	CString   str;   
	if (m_cmbRing.GetCurSel() == CB_ERR)
	{
		str = Data::LanguageResource::Get(Data::RI_CARD_DEFAULT).c_str();
	}
	else
	{
		m_cmbRing.GetLBText(m_cmbRing.GetCurSel(), str);
	}
	std::string filter;
	filter = "filename = '";
	filter += Util::StringOp::FromCString(str);
	filter += "' AND type = ";
    filter += Util::StringOp::FromInt(Data::sstRing);        
	std::vector<boost::shared_ptr<Data::SoundSegment> > ringresult = Data::SoundSegment::GetFromDatabase(filter);
	if (!ringresult.empty())
	{
		m_pContactGroup->ring(ringresult[0]);
	}

	if (m_cmbSoundTip.GetCurSel() == CB_ERR)
	{
		str = Data::LanguageResource::Get(Data::RI_CARD_DEFAULT).c_str();
	}
	else
	{
		m_cmbSoundTip.GetLBText(m_cmbSoundTip.GetCurSel(), str);
	}
	filter = "filename = '";
	filter += Util::StringOp::FromCString(str);
	filter += "' AND type = ";
	filter += Util::StringOp::FromInt(Data::sstTips);
	std::vector<boost::shared_ptr<Data::SoundSegment> > tipresult = Data::SoundSegment::GetFromDatabase(filter);
	if (!tipresult.empty())
	{
		//m_pContactGroup->tip(tipresult[0]);
	}

	bool isInsert = FALSE;
	if (m_pContactGroup->id() == 0)
	{
		m_pContactGroup->Insert();
		isInsert = true;
	}
	else
	{
		m_pContactGroup->Update();
	}
	int sel = ((CContactDlg*)m_pParentWnd)->GetTypeListSelected();
	((CContactDlg*)m_pParentWnd)->ShowTypeItems();

	if ((sel > 0) && isInsert)
	{
		++sel;
	}
	((CContactDlg*)m_pParentWnd)->SetTypeListSelected(sel);
}

void CContactGroupDlg::OnButtonContactGroupCancel() 
{
	// TODO: Add your control notification handler code here
	ShowWindow(FALSE);
}

void CContactGroupDlg::OnButtonContactGroupDelete() 
{
	// TODO: Add your control notification handler code here
	if (m_pContactGroup->id() != 0)
	{
		ShowWindow(FALSE);
		m_pContactGroup->Remove();
		int sel = ((CContactDlg*)m_pParentWnd)->GetTypeListSelected();
		((CContactDlg*)m_pParentWnd)->ShowTypeItems();
		((CContactDlg*)m_pParentWnd)->SetTypeListSelected(sel);
		((CContactDlg*)m_pParentWnd)->ShowItemsInList();
	}
}

void CContactGroupDlg::SetContactGroup(boost::shared_ptr<Data::ContactGroup> contactgroup)
{
	m_pContactGroup = contactgroup;
	AddRingTipItems();
	std::string strTemp;
	if (contactgroup->id() == 0)
	{
		strTemp = Data::LanguageResource::Get(Data::RI_CARD_NEWTYPEBTN);
		CString str = strTemp.c_str();
		m_sticTitle.SetWindowText(str);
		m_edtName.SetWindowText(_T(""));
	}
	else
	{
		strTemp = Data::LanguageResource::Get(Data::RI_CARD_EDITTYPEBTN);
		CString str = strTemp.c_str();
		m_sticTitle.SetWindowText(str);
		m_edtName.SetWindowText(CString(contactgroup->name().c_str()));
	}
		
	if (contactgroup->ring())
	{
		int index = m_cmbRing.SelectString(0, CString(contactgroup->ring()->filename().c_str()));
		if (index == CB_ERR)
		{
			m_cmbRing.SelectString(0, CString(Data::LanguageResource::Get(Data::RI_CARD_DEFAULT).c_str()));
		}
	}
	else
	{
		m_cmbRing.SelectString(0, CString(Data::LanguageResource::Get(Data::RI_CARD_DEFAULT).c_str()));
	}

	if (contactgroup->tips())
	{
		int index = m_cmbSoundTip.SelectString(0, CString(contactgroup->tips()->filename().c_str()));
		if (index == CB_ERR)
		{
			m_cmbSoundTip.SelectString(0, CString(Data::LanguageResource::Get(Data::RI_CARD_DEFAULT).c_str()));
		}

	}
	else
	{
		m_cmbSoundTip.SelectString(0, CString(Data::LanguageResource::Get(Data::RI_CARD_DEFAULT).c_str()));
	}
}

HBRUSH CContactGroupDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	HBRUSH b=CreateSolidBrush(RGB(255,255,255));
	return b;
	
	// TODO: Return a different brush if the default is not desired
	return hbr;
}
