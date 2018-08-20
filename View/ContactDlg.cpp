// ContactDlg.cpp : implementation file
//

#include "stdafx.h"
#include "../MultimediaPhone.h"
#include "../Data/LanguageResource.h"
#include "../MultimediaPhoneDlg.h"
#include "ContactDlg.h"
#include "../Data/ContactGroup.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define WM_SOFTKEY		(WM_USER+200)
/////////////////////////////////////////////////////////////////////////////
// CContactDlg dialog


CContactDlg::CContactDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CContactDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CContactDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CContactDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CContactDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CContactDlg, CDialog)
	//{{AFX_MSG_MAP(CContactDlg)
	//}}AFX_MSG_MAP
	ON_NOTIFY(NM_CLICK, IDC_LIST_CONTACT_TYPE, OnClickListType)
	ON_NOTIFY(NM_CLICK, IDC_LIST_CONTACT_LIST, OnClickListList)
	ON_BN_CLICKED(IDC_BUTTON_CONTACT_NEW, OnButtonContactNew)
	ON_BN_CLICKED(IDC_BUTTON_CONTACT_NEWTYPE, OnButtonContactNewtype)
	ON_BN_CLICKED(IDC_BUTTON_CONTACT_EDITTYPE, OnButtonContactEdittype)
	ON_BN_CLICKED(IDC_BUTTON_CONTACT_SEARCH, OnButtonContactSearch)
	ON_MESSAGE(WM_SOFTKEY, OnSoftKeyMessage)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CContactDlg message handlers

BOOL CContactDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	std::string strTemp = Data::LanguageResource::Get(Data::RI_CARD_TYPESTC);
	CString str = strTemp.c_str();
	m_sticTypeTitle.Create(str, WS_CHILD|WS_VISIBLE, CRect(0, 0, 89, 20), this);
	m_sticTypeTitle.SetColor(RGB(0, 0, 0), RGB(78, 165, 229));
	m_sticTypeTitle.SetLeftMargin(25);

	m_sticSepLine1.Create(_T(""), WS_CHILD|WS_VISIBLE, CRect(89, 0, 90, 164), this);
	m_sticSepLine1.SetColor(RGB(255, 255, 255), RGB(255, 255, 255));
	m_sticSepLine2.Create(_T(""), WS_CHILD|WS_VISIBLE, CRect(429, 0, 430, 164), this);
	m_sticSepLine2.SetColor(RGB(255, 255, 255), RGB(255, 255, 255));

	m_lsType.Create(WS_CHILD|WS_VISIBLE|LVS_REPORT|LVS_NOCOLUMNHEADER|LVS_NOSORTHEADER, CRect(0, 20, 89, 164), this, IDC_LIST_CONTACT_TYPE, TRUE);
	m_lsType.SetListColor(RGB(255, 255, 255), RGB(235, 252, 255));
	m_lsType.SetExtendedStyle(m_lsType.GetExtendedStyle()|LVS_EX_FULLROWSELECT);
	m_lsType.InsertColumn(0, _T("Type"), LVCFMT_LEFT, 70);

	m_pImageList = new CImageList();
	m_pImageList->Create(12, 12, ILC_COLOR32|ILC_MASK, 2, 2);   
    HICON hIcon = ::LoadIcon(AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_ICON1));   
	m_pImageList->Add(hIcon);  
    hIcon = ::LoadIcon(AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_ICON2));   
	m_pImageList->Add(hIcon);   
	m_lsType.SetImageList(m_pImageList, LVSIL_SMALL);

	strTemp = Data::LanguageResource::Get(Data::RI_CARD_NAMESTC);
	str = strTemp.c_str();
	m_sticName.Create(str, WS_CHILD|WS_VISIBLE, CRect(90, 0, 160, 20), this);
	m_sticName.SetColor(RGB(0, 0, 0), RGB(78, 165, 229));
	m_sticName.SetLeftMargin(20);

	strTemp = Data::LanguageResource::Get(Data::RI_CARD_COMPANYSTC);
	str = strTemp.c_str();
	m_sticCompany.Create(str, WS_CHILD|WS_VISIBLE, CRect(160, 0, 280, 20), this);
	m_sticCompany.SetColor(RGB(0, 0, 0), RGB(78, 165, 229));
	m_sticCompany.SetLeftMargin(40);

	strTemp = Data::LanguageResource::Get(Data::RI_CARD_DEPARTSTC);
	str = strTemp.c_str();
	m_sticDepartment.Create(str, WS_CHILD|WS_VISIBLE, CRect(280, 0, 350, 20), this);
	m_sticDepartment.SetColor(RGB(0, 0, 0), RGB(78, 165, 229));
	m_sticDepartment.SetLeftMargin(15);

	strTemp = Data::LanguageResource::Get(Data::RI_CARD_TITLESTC);
	str = strTemp.c_str();
	m_sticDuty.Create(str, WS_CHILD|WS_VISIBLE, CRect(350, 0, 430, 20), this);
	m_sticDuty.SetColor(RGB(0, 0, 0), RGB(78, 165, 229));
	m_sticDuty.SetLeftMargin(15);

	
	m_lsList.Create(WS_CHILD|WS_VISIBLE|LVS_REPORT|LVS_NOCOLUMNHEADER|LVS_NOSORTHEADER, CRect(90, 20, 429, 164), this, IDC_LIST_CONTACT_LIST, TRUE);
	m_lsList.SetListColor(RGB(255, 255, 255), RGB(235, 252, 255));
	m_lsList.SetExtendedStyle(m_lsList.GetExtendedStyle()|LVS_EX_FULLROWSELECT);
	m_lsList.InsertColumn(0, _T("Name"), LVCFMT_LEFT, 70);
	m_lsList.InsertColumn(1, _T("Company"), LVCFMT_LEFT, 120);
	m_lsList.InsertColumn(2, _T("Department"), LVCFMT_LEFT, 70);
	m_lsList.InsertColumn(3, _T("Duty"), LVCFMT_LEFT, 60);

	m_pImageList = new CImageList();
	m_pImageList->Create(12, 12, ILC_COLOR32|ILC_MASK, 2, 2);   
    hIcon = ::LoadIcon(AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_ICON1));   
	m_pImageList->Add(hIcon);  
    hIcon = ::LoadIcon(AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_ICON2));   
	m_pImageList->Add(hIcon);   

	m_lsList.SetImageList(m_pImageList, LVSIL_SMALL);

	CRect rectKey[29];
	UINT16 uiKey[29];
	int offsetX = 0;
	int offsetY = 0;
	for (int j = 0; j < 26; ++j)
	{	
		if (j > 12)
		{
			offsetX = (j - 13) * 30;
			offsetY = 20;
		}
		else
		{
			offsetX = j * 30;
		}
		rectKey[j] = CRect(90 + offsetX, offsetY, 120 + offsetX, offsetY + 20);
		uiKey[j] = 'a' + j;
	};
	rectKey[26] = CRect(0, 20, 30, 40);
	uiKey[26] = 0x7F;
	rectKey[27] = CRect(30, 20, 60, 40);
	uiKey[27] = 0x00;
	rectKey[28] = CRect(60, 20, 90, 40);
	uiKey[28] = '\?';
	
	m_sticPinYin.Create(_T(""), WS_CHILD|WS_VISIBLE, CRect(1, 165, 88, 183), this, IDC_STATIC_CONTACT_PINYIN);
	m_sticPinYin.SetColor(RGB(0, 0, 0), RGB(255, 255, 255));
	m_sticPinYin.SetLeftMargin(4);

	m_skSoftKey.Create(IDB_VOIPPYSEARCH, CPoint(0,0), 29, rectKey, uiKey, CRect(0, 164, 480, 204),this,WS_CHILD|WS_VISIBLE, IDC_SOFTKEY_CONTACT_PYSEARCH);

	strTemp = Data::LanguageResource::Get(Data::RI_CARD_OPERATORSTC);
	str = strTemp.c_str();
	m_sticOperation.Create(str, WS_CHILD|WS_VISIBLE, CRect(430, 0, 480, 20), this);
	m_sticOperation.SetColor(RGB(0, 0, 0), RGB(78, 165, 229));
	m_sticOperation.SetLeftMargin(10);

	strTemp = Data::LanguageResource::Get(Data::RI_CARD_NEWCARDBTN);
	str = strTemp.c_str();
	m_btnNewContact.Create(str, WS_CHILD|WS_VISIBLE, CRect(430, 20, 480, 40), this, IDC_BUTTON_CONTACT_NEW);
	SetButtonDefaultColor(&m_btnNewContact);

	strTemp = Data::LanguageResource::Get(Data::RI_CARD_NEWTYPEBTN);
	str = strTemp.c_str();
	m_btnNewType.Create(str, WS_CHILD|WS_VISIBLE, CRect(430, 40, 480, 60), this, IDC_BUTTON_CONTACT_NEWTYPE);
	SetButtonDefaultColor(&m_btnNewType);
	
	strTemp = Data::LanguageResource::Get(Data::RI_CARD_EDITTYPEBTN);
	str = strTemp.c_str();
	m_btnEditType.Create(str, WS_CHILD|WS_VISIBLE, CRect(430, 60, 480, 80), this, IDC_BUTTON_CONTACT_EDITTYPE);
	SetButtonDefaultColor(&m_btnEditType);
	
	strTemp = Data::LanguageResource::Get(Data::RI_COMN_FINDBTN);
	str = strTemp.c_str();
	m_btnSearch.Create(str, WS_CHILD|WS_VISIBLE, CRect(430, 80, 480, 100), this, IDC_BUTTON_CONTACT_SEARCH);
	SetButtonDefaultColor(&m_btnSearch);
	
	m_pContactSearchDlg = new CContactSearchDlg(this);
	m_pContactSearchDlg->Create(CContactSearchDlg::IDD);

	ShowTypeItems();
	m_lsType.SetItemState(0, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
	ShowItemsInList();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CContactDlg::SetButtonDefaultColor(CCEButtonST* button)
{
	button->SetColor(CCEButtonST::BTNST_COLOR_BK_IN, RGB(248,214,147));
	button->SetColor(CCEButtonST::BTNST_COLOR_BK_OUT, RGB(248,214,147));
	button->SetColor(CCEButtonST::BTNST_COLOR_BK_FOCUS, RGB(248,214,147));
}

void CContactDlg::SetButtonSelectedColor(CCEButtonST* button)
{
	button->SetColor(CCEButtonST::BTNST_COLOR_BK_IN, RGB(248,214,147));
	button->SetColor(CCEButtonST::BTNST_COLOR_BK_OUT, RGB(248,214,147));
	button->SetColor(CCEButtonST::BTNST_COLOR_BK_FOCUS, RGB(248,214,147));
}

void CContactDlg::OnClickListType(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	ShowItemsInList();
	//GetDlgItem(IDC_STATIC_CONTACT_PINYIN)->SetWindowText(_T(""));
	*pResult = 0;
}

void CContactDlg::ShowTypeItems(void)
{
	m_lsType.DeleteAllItems();
	int imageID = 0;
	std::string strTemp;
	strTemp = Data::LanguageResource::Get(Data::RI_CARD_ALLCARDSTC);
	CString str = strTemp.c_str();
	m_lsType.InsertItem(0, str, imageID);
	m_lsType.SetItemData(0, 0);
	++imageID;
	std::vector<boost::shared_ptr<Data::ContactGroup> > result = Data::ContactGroup::GetFromDatabase("");
	for (int i = 0; i < result.size(); ++i)
	{
		m_lsType.InsertItem(i + 1, CString(result[i]->name().c_str()), imageID % 2);
		m_lsType.SetItemData(i + 1, result[i]->id());
		++imageID;
	}
	strTemp = Data::LanguageResource::Get(Data::RI_CARD_FINDREAULTSTC);
	str = strTemp.c_str();
	m_lsType.InsertItem(result.size() + 1, str, imageID % 2);
	m_lsType.SetItemData(result.size() + 1, result.size() + 1);
	m_lsType.SetScrollRagle(FALSE);
}

void CContactDlg::ShowItemsInList(void)
{
	if (ShowItemsInListByAlias())
		return;

	POSITION pos = m_lsType.GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
		return;
	}

	int type = m_lsType.GetNextSelectedItem(pos);
	std::string filter;
	if (type == 0)
	{
		filter = "";
	}
	else if (type == m_lsType.GetItemCount() - 1)
	{
		ShowArrayInList(m_vSearchResult);
		return;
	}
	else
	{
		filter = "groupId = " + Util::StringOp::FromInt(m_lsType.GetItemData(type));
	}

	std::vector<boost::shared_ptr<Data::Contact> > result = Data::Contact::GetFromDatabase(filter);
	ShowArrayInList(result);	
}

bool CContactDlg::ShowItemsInListByAlias(void)
{
	CString py;
	GetDlgItem(IDC_STATIC_CONTACT_PINYIN)->GetWindowText(py);
	if (!py.IsEmpty())
	{
		std::string alias = Util::StringOp::FromCString(py);
		for (int i = 0; i < alias.size(); ++i)
		{
			if (alias[i] == '\?')
			{
				alias[i] = '_';
			}
		}
		ShowItemsInListByAlias(alias);
		return true;
	}
	return false;
}

void CContactDlg::ShowItemsInListByAlias(std::string alias)
{
	POSITION pos = m_lsType.GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
		return;
	}

	int type = m_lsType.GetNextSelectedItem(pos);
	std::string filter;
	if (type == 0)
	{
		filter = "alias LIKE '";
		filter += alias;
		filter += "%'";
	}
	else if (type == m_lsType.GetItemCount() - 1)
	{
		std::vector<boost::shared_ptr<Data::Contact> > aliasResult;
		for (std::vector<boost::shared_ptr<Data::Contact> >::iterator iter = m_vSearchResult.begin(); iter != m_vSearchResult.end(); ++iter)
		{
			if ((*iter)->alias().find(alias) != std::string.npos)
			{
				aliasResult.push_back(*iter);
			}
		}
		ShowArrayInList(aliasResult);
		return;
	}
	else
	{
		filter = "groupId = " + Util::StringOp::FromInt(m_lsType.GetItemData(type));
		filter += " AND alias LIKE '";
		filter += alias;
		filter += "%'";
	}

	std::vector<boost::shared_ptr<Data::Contact> > result = Data::Contact::GetFromDatabase(filter);
	ShowArrayInList(result);	
}

void CContactDlg::ShowArrayInList(std::vector<boost::shared_ptr<Data::Contact> > array)
{
	m_lsList.DeleteAllItems();
	CString temp;
	for (int i = 0; i < (array).size(); ++i)
	{	
		temp = array[i]->name().c_str();	
		m_lsList.InsertItem(i, temp, i % 2);
		temp = array[i]->company().c_str();
		m_lsList.SetItemText(i, 1, temp);
		temp = array[i]->department().c_str();
		m_lsList.SetItemText(i, 2, temp);
		temp = array[i]->duty().c_str();
		m_lsList.SetItemText(i, 3, temp);

		m_lsList.SetItemData(i, array[i]->id());
	}
	m_lsList.SetScrollRagle(FALSE);
}

void CContactDlg::OnClickListList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	POSITION pos = m_lsList.GetFirstSelectedItemPosition();
	if (pos != NULL)
	{
		int index = m_lsList.GetNextSelectedItem (pos);
		int id = m_lsList.GetItemData(index);
		std::vector<boost::shared_ptr<Data::Contact> > result = Data::Contact::GetFromDatabase("id = " + Util::StringOp::FromInt(id));
		if (!result.empty())
		{
			((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactNewDlg->SetContact(result[0]);
			((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactNewDlg->ShowWindow(TRUE);
		}
	}
	*pResult = 0;
}

void CContactDlg::OnButtonContactNew() 
{
	// TODO: Add your control notification handler code here
	boost::shared_ptr<Data::Contact> contact(new Data::Contact);
	contact->id(0);
	((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactNewDlg->SetContact(contact);
	((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactNewDlg->ShowWindow(TRUE);
}

void CContactDlg::OnButtonContactNewtype() 
{
	// TODO: Add your control notification handler code here
	boost::shared_ptr<Data::ContactGroup> contactgroup(new Data::ContactGroup);
	contactgroup->id(0);
	((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactGroupDlg->SetContactGroup(contactgroup);
	((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactGroupDlg->ShowWindow(TRUE);
}

void CContactDlg::OnButtonContactEdittype() 
{
	// TODO: Add your control notification handler code here
	POSITION pos = m_lsType.GetFirstSelectedItemPosition();
	if (pos != NULL)
	{
		int index = m_lsType.GetNextSelectedItem (pos);
		if ((index == 0) || (index == m_lsType.GetItemCount() - 1))
		{
			return;
		}

		int id = m_lsType.GetItemData(index);
		std::vector<boost::shared_ptr<Data::ContactGroup> > result = Data::ContactGroup::GetFromDatabase("id = " + Util::StringOp::FromInt(id));
		if (!result.empty())
		{
			((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactGroupDlg->SetContactGroup(result[0]);
			((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactGroupDlg->ShowWindow(TRUE);
		}
	}
}

void CContactDlg::OnButtonContactSearch() 
{
	// TODO: Add your control notification handler code here
	m_pContactSearchDlg->ShowWindow(TRUE);
}

LRESULT CContactDlg::OnSoftKeyMessage(WPARAM wParam, LPARAM lParam)
{
	unsigned short key = wParam; 
	CString py(key); 
	switch(char(key))
	{
	case 0x7F://del
		GetDlgItem(IDC_STATIC_CONTACT_PINYIN)->GetWindowText(py);
		GetDlgItem(IDC_STATIC_CONTACT_PINYIN)->SetWindowText(py.Left(py.GetLength() - 1));
		break;
	case 0x00://clear
		GetDlgItem(IDC_STATIC_CONTACT_PINYIN)->SetWindowText(_T(""));
		break;
// 	case '\?'://?
// 		break';
	default:
		GetDlgItem(IDC_STATIC_CONTACT_PINYIN)->GetWindowText(py);
		if (py.GetLength() >= 4)
		{
			return 0;
		}
		GetDlgItem(IDC_STATIC_CONTACT_PINYIN)->SetWindowText(py + key);
		break;
	}
	ShowItemsInList();
	return 0;
}

int CContactDlg::GetTypeListSelected(void)
{
	POSITION pos = m_lsType.GetFirstSelectedItemPosition();
	if (pos != NULL)
	{
		int index = m_lsType.GetNextSelectedItem (pos);
		return index;
	}
	return -1;
}

void CContactDlg::SetTypeListSelected(int index)
{
	POSITION pos = m_lsType.GetFirstSelectedItemPosition();   
	while (pos != NULL)
	{   
		int iSel = m_lsType.GetNextSelectedItem(pos);   
		m_lsType.SetItemState(iSel, 0, LVIS_SELECTED);   
	}
    m_lsType.SetItemState(index, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
}

void CContactDlg::ShowSearchResultInList(std::vector<boost::shared_ptr<Data::Contact> > array)
{
	m_vSearchResult = array;
	POSITION pos = m_lsType.GetFirstSelectedItemPosition();   
	while (pos != NULL)
	{   
		int iSel = m_lsType.GetNextSelectedItem(pos);   
		m_lsType.SetItemState(iSel, 0, LVIS_SELECTED);   
	}
    m_lsType.SetItemState(m_lsType.GetItemCount() - 1, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	ShowArrayInList(array);	
}
