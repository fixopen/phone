// ContactDlg.cpp : implementation file
//

#include "stdafx.h"
#include "../MultimediaPhone.h"
#include "../Data/LanguageResource.h"
#include "../MultimediaPhoneDlg.h"
#include "ContactDlg.h"
#include "../Data/ContactGroup.h"
#include "../Data/SkinStyle.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CContactDlg dialog
const int ContactTotal = 10000;
const int PageSize = 7;

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
	ON_BN_CLICKED(IDC_BUTTON_CONTACT_CLOSE, OnButtonContactClose)
	ON_MESSAGE(WM_SOFTKEY, OnSoftKeyMessage)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CContactDlg message handlers

BOOL CContactDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	std::string strTemp;
	CString str;

	m_lsType.Create(WS_CHILD|WS_VISIBLE|LVS_REPORT|LVS_NOCOLUMNHEADER|LVS_NOSORTHEADER, CRect(8, 27, 100, 157), this, IDC_LIST_CONTACT_TYPE, TRUE);
	m_lsType.SetExtendedStyle(m_lsType.GetExtendedStyle()|LVS_EX_FULLROWSELECT);
	m_lsType.InsertColumn(0, _T("Type"), LVCFMT_LEFT, 72);

	m_pImageList = new CImageList();
	m_pImageList->Create(16, 16, ILC_COLOR32|ILC_MASK, 12, 2);   
    
	CBitmap bm;
	bm.LoadBitmap(IDB_BITMAP5);
	m_pImageList->Add(&bm, RGB(255, 0, 255));
	bm.DeleteObject();
	bm.LoadBitmap(IDB_BITMAP1);
	m_pImageList->Add(&bm, RGB(255, 0, 255));
	bm.DeleteObject();
	bm.LoadBitmap(IDB_BITMAP9);
	m_pImageList->Add(&bm, RGB(255, 0, 255));
	bm.DeleteObject();
	bm.LoadBitmap(IDB_BITMAP10);
	m_pImageList->Add(&bm, RGB(255, 0, 255));
	bm.DeleteObject();
	bm.LoadBitmap(IDB_BITMAP11);
	m_pImageList->Add(&bm, RGB(255, 0, 255));
	bm.DeleteObject();
	bm.LoadBitmap(IDB_BITMAP12);
	m_pImageList->Add(&bm, RGB(255, 0, 255));
	bm.DeleteObject();
	bm.LoadBitmap(IDB_BITMAP13);
	m_pImageList->Add(&bm, RGB(255, 0, 255));
	bm.DeleteObject();
	bm.LoadBitmap(IDB_BITMAP14);
	m_pImageList->Add(&bm, RGB(255, 0, 255));
	bm.DeleteObject();
	bm.LoadBitmap(IDB_BITMAP15);
	m_pImageList->Add(&bm, RGB(255, 0, 255));
	bm.DeleteObject();
	bm.LoadBitmap(IDB_BITMAP16);
	m_pImageList->Add(&bm, RGB(255, 0, 255));
	bm.DeleteObject();
	bm.LoadBitmap(IDB_BITMAP17);
	m_pImageList->Add(&bm, RGB(255, 0, 255));
	bm.DeleteObject();
	bm.LoadBitmap(IDB_BITMAP18);
	m_pImageList->Add(&bm, RGB(255, 0, 255));
	bm.DeleteObject();

	m_lsType.SetImageList(m_pImageList, LVSIL_SMALL);
	
	m_lsList.Create(WS_CHILD|WS_VISIBLE|LVS_REPORT|LVS_NOCOLUMNHEADER|LVS_NOSORTHEADER, CRect(101, 27, 416, 157), this, IDC_LIST_CONTACT_LIST, TRUE, 0, FALSE);
	m_lsList.SetExtendedStyle(m_lsList.GetExtendedStyle()|LVS_EX_FULLROWSELECT);
	m_lsList.InsertColumn(0, _T("Name"), LVCFMT_LEFT, 70);
	m_lsList.InsertColumn(1, _T("Company"), LVCFMT_LEFT, 96);
	m_lsList.InsertColumn(2, _T("Department"), LVCFMT_LEFT, 70);
	m_lsList.InsertColumn(3, _T("Duty"), LVCFMT_LEFT, 60);

	m_pImageList = new CImageList();
	m_pImageList->Create(16, 16, ILC_COLOR32|ILC_MASK, 3, 2);   

	bm.LoadBitmap(IDB_BITMAP8);
	m_pImageList->Add(&bm, RGB(255, 0, 255));
	bm.DeleteObject();
	bm.LoadBitmap(IDB_BITMAP7);
	m_pImageList->Add(&bm, RGB(255, 0, 255));
	bm.DeleteObject();
	bm.LoadBitmap(IDB_BITMAP6);
	m_pImageList->Add(&bm, RGB(255, 0, 255));
	bm.DeleteObject();

	m_lsList.SetImageList(m_pImageList, LVSIL_SMALL);

	CRect rectKey[29];
	UINT16 uiKey[29];
	int offsetX = 0;
	int offsetY = 0;
	for (int j = 0; j < 26; ++j)
	{	
		if (j > 12)
		{
			offsetX = (j - 13) * 29;
			offsetY = 21;
		}
		else
		{
			offsetX = j * 29;
		}
		rectKey[j] = CRect(87 + offsetX, offsetY, 116 + offsetX, offsetY + 20);
		uiKey[j] = 'a' + j;
	};
	rectKey[26] = CRect(0, 21, 29, 41);
	uiKey[26] = 0x7F;
	rectKey[27] = CRect(29, 21, 58, 41);
	uiKey[27] = 0x00;
	rectKey[28] = CRect(58, 21, 87, 41);
	uiKey[28] = '\?';
	
	m_sticPinYin.Create(_T(""), WS_CHILD|WS_VISIBLE, CRect(9, 160, 90, 176), this, IDC_STATIC_CONTACT_PINYIN);
	m_sticPinYin.SetColor(RGB(0, 0, 0), RGB(255, 255, 255));
	m_sticPinYin.SetLeftMargin(10);

	m_skSoftKey.Create(IDB_VOIPPYSEARCH, CPoint(0,0), 29, rectKey, uiKey, CRect(8, 157, 472, 198),this,WS_CHILD|WS_VISIBLE, IDC_SOFTKEY_CONTACT_PYSEARCH);

	strTemp = Data::LanguageResource::Get(Data::RI_CARD_NEWCARDBTN);
	str = strTemp.c_str();
	m_btnNewContact.Create(str, Data::g_buttonRectBMPID[0][Data::g_skinstyle], Data::g_buttonRectBMPID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(417, 26, 471, 45), this, IDC_BUTTON_CONTACT_NEW);
	m_btnNewContact.SetBackRGB(Data::g_allFramBackRGB[Data::g_skinstyle]);

	strTemp = Data::LanguageResource::Get(Data::RI_CARD_NEWTYPEBTN);
	str = strTemp.c_str();
	m_btnNewType.Create(str, Data::g_buttonRectBMPID[0][Data::g_skinstyle], Data::g_buttonRectBMPID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(417, 46, 471, 65), this, IDC_BUTTON_CONTACT_NEWTYPE);
	m_btnNewType.SetBackRGB(Data::g_allFramBackRGB[Data::g_skinstyle]);
	
	strTemp = Data::LanguageResource::Get(Data::RI_CARD_EDITTYPEBTN);
	str = strTemp.c_str();
	m_btnEditType.Create(str, Data::g_buttonRectBMPID[0][Data::g_skinstyle], Data::g_buttonRectBMPID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(417, 66, 471, 85), this, IDC_BUTTON_CONTACT_EDITTYPE);
	m_btnEditType.SetBackRGB(Data::g_allFramBackRGB[Data::g_skinstyle]);
	
	strTemp = Data::LanguageResource::Get(Data::RI_COMN_FINDBTN);
	str = strTemp.c_str();
	m_btnSearch.Create(str, Data::g_buttonRectBMPID[0][Data::g_skinstyle], Data::g_buttonRectBMPID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(417, 86, 471, 105), this, IDC_BUTTON_CONTACT_SEARCH);
	m_btnSearch.SetBackRGB(Data::g_allFramBackRGB[Data::g_skinstyle]);
		
	strTemp = Data::LanguageResource::Get(Data::RI_SOUND_PLAY_CLOSE);
	str = strTemp.c_str();
	m_btnClose.Create(str, Data::g_buttonRectBMPID[0][Data::g_skinstyle], Data::g_buttonRectBMPID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(417, 138, 471, 157), this, IDC_BUTTON_CONTACT_CLOSE);
	m_btnClose.SetBackRGB(Data::g_allFramBackRGB[Data::g_skinstyle]);

	m_sticPanel.Create(_T(""), WS_CHILD|WS_VISIBLE, CRect(416, 26, 472, 157), this);
	m_sticPanel.SetColor(RGB(0, 0, 0), Data::g_allFrameInFrameLine1RGB[0][Data::g_skinstyle]);

	m_sticBackground.Create(CRect(0, 0, 480, 204), this, 2);

	TextStruct ts[6];
	memset(ts, 0, sizeof(TextStruct) * 6);

	ts[0].txtRect = CRect(0, 0, 100, 20);
	ts[0].txtFontSize = 16;
	ts[0].sAlign = DT_CENTER | DT_BOTTOM;
	memcpy(ts[0].sTxt, Data::LanguageResource::Get(Data::RI_CARD_TYPESTC).c_str(), Data::LanguageResource::Get(Data::RI_CARD_TYPESTC).length());
	
	ts[1].txtRect = CRect(112, 0, 170, 20);
	ts[1].txtFontSize = 16;
	ts[1].sAlign = DT_CENTER | DT_BOTTOM;
	memcpy(ts[1].sTxt, Data::LanguageResource::Get(Data::RI_CARD_NAMESTC).c_str(), Data::LanguageResource::Get(Data::RI_CARD_NAMESTC).length());
	
	ts[2].txtRect = CRect(170, 0, 266, 20);
	ts[2].txtFontSize = 16;
	ts[2].sAlign = DT_CENTER | DT_BOTTOM;
	memcpy(ts[2].sTxt, Data::LanguageResource::Get(Data::RI_CARD_COMPANYSTC).c_str(), Data::LanguageResource::Get(Data::RI_CARD_COMPANYSTC).length());

	ts[3].txtRect = CRect(266, 0, 336, 20);
	ts[3].txtFontSize = 16;
	ts[3].sAlign = DT_CENTER | DT_BOTTOM;
	memcpy(ts[3].sTxt, Data::LanguageResource::Get(Data::RI_CARD_DEPARTSTC).c_str(), Data::LanguageResource::Get(Data::RI_CARD_DEPARTSTC).length());
	
	ts[4].txtRect = CRect(336, 0, 396, 20);
	ts[4].txtFontSize = 16;
	ts[4].sAlign = DT_CENTER | DT_BOTTOM;
	memcpy(ts[4].sTxt, Data::LanguageResource::Get(Data::RI_CARD_TITLESTC).c_str(), Data::LanguageResource::Get(Data::RI_CARD_TITLESTC).length());
	
	ts[5].txtRect = CRect(417, 0, 470, 20);
	ts[5].txtFontSize = 16;
	ts[5].sAlign = DT_CENTER | DT_BOTTOM;
	memcpy(ts[5].sTxt, Data::LanguageResource::Get(Data::RI_CARD_OPERATORSTC).c_str(), Data::LanguageResource::Get(Data::RI_CARD_OPERATORSTC).length());
	
	m_sticBackground.SetTextStruct(ts, 6);

	m_pContactSearchDlg = new CContactSearchDlg(this);
	m_pContactSearchDlg->Create(CContactSearchDlg::IDD);

	ShowTypeItems();
	m_lsType.SetItemState(0, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);

	m_sListFilter = "";
	m_sListSearchFilter = "";	
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
// 	//GetDlgItem(IDC_STATIC_CONTACT_PINYIN)->SetWindowText(_T(""));

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
	imageID = 2;
	std::vector<boost::shared_ptr<Data::ContactGroup> > result = Data::ContactGroup::GetFromDatabase("");
	for (int i = 0; i < result.size(); ++i)
	{
		m_lsType.InsertItem(i + 1, CString(result[i]->name().c_str()), imageID);
		m_lsType.SetItemData(i + 1, result[i]->id());
		++imageID;
		if (imageID > 10)
		{
			imageID = 2;
		}
	}
	strTemp = Data::LanguageResource::Get(Data::RI_CARD_FINDREAULTSTC);
	str = strTemp.c_str();
// 	m_lsType.InsertItem(result.size() + 1, str, imageID % 2);
	m_lsType.InsertItem(result.size() + 1, str, 1);
	m_lsType.SetItemData(result.size() + 1, result.size() + 1);
	m_lsType.SetScrollRagle(TRUE);
}

void CContactDlg::ShowItemsInList(void)
{
// 	if (ShowItemsInListByAlias())
// 		return;
// 
// 	POSITION pos = m_lsType.GetFirstSelectedItemPosition();
// 	if (pos == NULL)
// 	{
// 		return;
// 	}
// 
// 	int type = m_lsType.GetNextSelectedItem(pos);
// 	std::string filter;
// 	if (type == 0)
// 	{
// 		filter = "";
// 	}
// 	else if (type == m_lsType.GetItemCount() - 1)
// 	{
// 		ShowArrayInList(m_vSearchResult);
// 		return;
// 	}
// 	else
// 	{
// 		filter = "groupId = " + Util::StringOp::FromInt(m_lsType.GetItemData(type));
// 	}
// 
// 	CTime t = CTime::GetCurrentTime();
// 	printf(("\n%d:%d:%d\n"), t.GetHour(), t.GetMinute(), t.GetSecond());
// 	std::vector<boost::shared_ptr<Data::Contact> > result = Data::Contact::GetFromDatabase(filter);
// 	t = CTime::GetCurrentTime();
// 	printf(("\n%d:%d:%d\n"), t.GetHour(), t.GetMinute(), t.GetSecond());
// 	ShowArrayInList(result);	
// 	t = CTime::GetCurrentTime();
// 	printf(("\n%d:%d:%d\n"), t.GetHour(), t.GetMinute(), t.GetSecond());
// 	int c = Data::Contact::GetDataCount("");
// 	printf("\ncount = %d\n", c);


	POSITION pos = m_lsType.GetFirstSelectedItemPosition();
	if (pos != NULL)
	{
		m_lsList.DeleteAllItems();

		m_sListFilter = "";

		std::string filter;
		std::string aliasFilter;
		std::string alias = GetPYIndex();
		if (alias != "")
		{
			aliasFilter = "alias LIKE '";
			aliasFilter += alias;
			aliasFilter += "%'";
		}

		int type = m_lsType.GetNextSelectedItem(pos);
		if (type == 0)
		{
			filter = aliasFilter;
		}
		else if (type == m_lsType.GetItemCount() - 1)
		{
			if (m_sListSearchFilter == "")
			{
				return;
			}

			filter = m_sListSearchFilter;
			if (aliasFilter != "")
			{
				filter += " AND ";
				filter += aliasFilter;
			}
		}
		else
		{
			m_sListFilter = "groupId = " + Util::StringOp::FromInt(m_lsType.GetItemData(type));
			filter = m_sListFilter;
			if (aliasFilter != "")
			{
				filter += " AND ";
				filter += aliasFilter;
			}
		}	

		if (Data::Contact::GetDataCount("") > 0)
		{
			m_vCurrentResult = Data::Contact::GetFromDatabase(filter, Data::dDown, Data::Contact::GetCurrentId() + 1, PageSize);
			int count = Data::Contact::GetDataCount(filter);
			m_lsList.m_scollbar_.SetRangle(0, count, PageSize);
			ShowArrayInList(m_vCurrentResult);		
			m_lsList.SetScrollRagle(TRUE);
		}
	}
}

void CContactDlg::ScrollItemsInList(int step)
{
	if (m_vCurrentResult.empty())
	{
		return;
	}
	std::string filter = m_sListFilter;
	POSITION pos = m_lsType.GetFirstSelectedItemPosition();
	if (pos != NULL)
	{
		int type = m_lsType.GetNextSelectedItem(pos);
		if (type == m_lsType.GetItemCount() - 1)
		{
			filter = m_sListSearchFilter;
		}
	}

	std::string alias = GetPYIndex();
	if (alias != "")
	{
		if (filter != "")
		{
			filter += " AND ";
		}
		filter = "alias LIKE '";
		filter += alias;
		filter += "%'";
	}

	std::vector<boost::shared_ptr<Data::Contact> > temp;
	int currentID = 0;

	if (step == SB_LINEUP)
	{
		currentID = m_vCurrentResult[0]->id();
		temp = Data::Contact::GetFromDatabase(filter, Data::dUp, currentID, 1);
		if (!temp.empty())
		{
			m_vCurrentResult.insert(m_vCurrentResult.begin(),temp[0]);
			if (m_vCurrentResult.size() > PageSize)
			{
				m_vCurrentResult.pop_back();
			}
		}
	}
	else if (step == SB_LINEDOWN)
	{
		currentID = m_vCurrentResult[m_vCurrentResult.size() - 1]->id();
		temp = Data::Contact::GetFromDatabase(filter, Data::dDown, currentID, 1);
		if (!temp.empty())
		{
			m_vCurrentResult.push_back(temp[0]);
			if (m_vCurrentResult.size() > PageSize)
			{
				m_vCurrentResult.erase(m_vCurrentResult.begin());
			}
		}
	}
	else if (step == SB_PAGEUP)
	{
		currentID = m_vCurrentResult[0]->id();
		temp = Data::Contact::GetFromDatabase(filter, Data::dUp, currentID, PageSize);
		if (temp.size() < PageSize)
		{
			temp.insert(temp.end(), m_vCurrentResult.begin(), m_vCurrentResult.begin() + PageSize - temp.size());
		}
		m_vCurrentResult = temp;
	}
	else if(step == SB_PAGEDOWN)
	{
		currentID = m_vCurrentResult[m_vCurrentResult.size() - 1]->id();
		temp = Data::Contact::GetFromDatabase(filter, Data::dDown, currentID, PageSize);
		if (temp.size() < PageSize)
		{
			m_vCurrentResult.insert(m_vCurrentResult.end(), temp.begin(), temp.end());
			if (m_vCurrentResult.size() > PageSize)
			{
				m_vCurrentResult.erase(m_vCurrentResult.begin(), m_vCurrentResult.begin() + m_vCurrentResult.size() - PageSize);
			}
		}
		else
		{
			m_vCurrentResult = temp;
		}
	}
	ShowArrayInList(m_vCurrentResult);		
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
	SetWindowRedraw(m_lsList.m_hWnd, FALSE);
	m_lsList.DeleteAllItems();
	CString temp;
	for (int i = 0; i < (array).size(); ++i)
	{	
		temp = array[i]->name().c_str();	
		m_lsList.InsertItem(i, temp, array[i]->type());
		temp = array[i]->company().c_str();
		m_lsList.SetItemText(i, 1, temp);
		temp = array[i]->department().c_str();
		m_lsList.SetItemText(i, 2, temp);
		temp = array[i]->duty().c_str();
		m_lsList.SetItemText(i, 3, temp);

		m_lsList.SetItemData(i, array[i]->id());
	}

	SetWindowRedraw(m_lsList.m_hWnd, TRUE);
	m_lsList.Invalidate();
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
			m_bIsDeleteItem = true;
			((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactNewDlg->SetContact(result[0]);
			((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactNewDlg->ShowWindow(TRUE);
		}
	}
	*pResult = 0;
}

void CContactDlg::OnButtonContactNew() 
{
	// TODO: Add your control notification handler code here
	if (Data::Contact::GetDataCount("") < ContactTotal)
	{
		boost::shared_ptr<Data::Contact> contact(new Data::Contact);
		contact->id(0);
		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactNewDlg->SetContact(contact);
		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactNewDlg->ShowWindow(TRUE);
	}
	else
	{
		MessageBox(Util::StringOp::ToCString(Data::LanguageResource::Get(Data::RI_SYSTEM_DISK)));
	}
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
			m_bIsDeleteItem = false;
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

void CContactDlg::OnButtonContactClose()
{
	((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->SwitchPanel_(IDC_BUTTON_MAIN);
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

std::string CContactDlg::GetPYIndex(void)
{
	std::string alias;
	CString py;
	GetDlgItem(IDC_STATIC_CONTACT_PINYIN)->GetWindowText(py);
	if (!py.IsEmpty())
	{
		alias = Util::StringOp::FromCString(py);
		for (int i = 0; i < alias.size(); ++i)
		{
			if (alias[i] == '\?')
			{
				alias[i] = '_';
			}
		}
	}
	return alias;
}

void CContactDlg::DeleteSelectedItem(void)
{
	POSITION pos = m_lsList.GetFirstSelectedItemPosition();
	if (pos != NULL)
	{
		int index = m_lsList.GetNextSelectedItem (pos);   		
//		int contactID = m_lsList.GetItemData(index);
// 		Data::Contact::Remove("id = " + Util::StringOp::FromInt(contactID));
//		m_lsList.DeleteItem(index);	
		
		m_vCurrentResult[index]->Remove();
		ShowItemsInList();		

// 		if (m_lsList.GetItemCount() > 0)
// 		{
// 			POSITION pos = m_lsList.GetFirstSelectedItemPosition();   
// 			while (pos != NULL)
// 			{   
// 				int iSel = m_lsList.GetNextSelectedItem(pos);   
// 				m_lsList.SetItemState(iSel, 0, LVIS_SELECTED);   
// 			}
// 			
// 			if (m_lsList.GetItemCount() - 1 < index)
// 			{
// 				index -= 1;	
// 			}
// 			m_lsList.SetItemState(index, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
// 		}
	}
}

void CContactDlg::DeleteSelectedGroup(void)
{
	POSITION pos = m_lsType.GetFirstSelectedItemPosition();
	if (pos != NULL)
	{
		int index = m_lsType.GetNextSelectedItem (pos);   		
		int typeID = m_lsType.GetItemData(index);
		Data::ContactGroup::Remove("id = " + Util::StringOp::FromInt(typeID));
		m_lsType.DeleteItem(index);
				
		if (m_lsType.GetItemCount() > 0)
		{
			POSITION pos = m_lsType.GetFirstSelectedItemPosition();   
			while (pos != NULL)
			{   
				int iSel = m_lsType.GetNextSelectedItem(pos);   
				m_lsType.SetItemState(iSel, 0, LVIS_SELECTED);   
			}
			
			if (m_lsType.GetItemCount() - 1 < index)
			{
				index -= 1;	
			}
			m_lsType.SetItemState(index, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
		}
		ShowItemsInList();
	}
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

void CContactDlg::Search(std::string filter)
{
	m_sListSearchFilter = filter;
	GetDlgItem(IDC_STATIC_CONTACT_PINYIN)->SetWindowText(_T(""));
	POSITION pos = m_lsType.GetFirstSelectedItemPosition();   
	while (pos != NULL)
	{   
		int iSel = m_lsType.GetNextSelectedItem(pos);   
		m_lsType.SetItemState(iSel, 0, LVIS_SELECTED);   
	}
    m_lsType.SetItemState(m_lsType.GetItemCount() - 1, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	ShowItemsInList();
}

LRESULT CContactDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	switch (message)
	{
	case WM_DELETESELITEM:
		if (m_bIsDeleteItem)
		{
			((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactNewDlg->ShowWindow(FALSE);
			DeleteSelectedItem();
		}
		else
		{
			((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactGroupDlg->ShowWindow(FALSE);
			DeleteSelectedGroup();
		}
		break;
	case WM_VSCROLL:
		ScrollItemsInList(wParam);
		break;
	};
	
	return CDialog::WindowProc(message, wParam, lParam);
}
