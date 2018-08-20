// ContactInfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "../MultimediaPhone.h"
#include "ContactInfoDlg.h"
#include "../Data/LanguageResource.h"
#include "../Data/Data.h"
#include "../Data/Contact.h"
#include "../MultimediaPhoneDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CContactInfoDlg dialog


CContactInfoDlg::CContactInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CContactInfoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CContactInfoDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CContactInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CContactInfoDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CContactInfoDlg, CDialog)
	//{{AFX_MSG_MAP(CContactInfoDlg)
	//}}AFX_MSG_MAP
	ON_NOTIFY(NM_CLICK, IDC_LIST_CONTACTINFO_TYPE, OnClickListType)
	ON_NOTIFY(NM_CLICK, IDC_LIST_CONTACTINFO_LIST, OnClickListList)
	ON_BN_CLICKED(IDC_BUTTON_CONTACTINFO_DIAL, OnButtonDial)
	ON_BN_CLICKED(IDC_BUTTON_CONTACTINFO_SOUND, OnButtonSound)
	ON_BN_CLICKED(IDC_BUTTON_CONTACTINFO_NOTE, OnButtonNote)
	ON_BN_CLICKED(IDC_BUTTON_CONTACTINFO_NEW, OnButtonNew)
	ON_BN_CLICKED(IDC_BUTTON_CONTACTINFO_SEARCH, OnButtonSearch)
	ON_BN_CLICKED(IDC_BUTTON_CONTACTINFO_DELETE, OnButtonDelete)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CContactInfoDlg message handlers

BOOL CContactInfoDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	std::string strTemp = Data::LanguageResource::Get(Data::RI_RECORD_TYPE);
	CString str = strTemp.c_str();
	m_sticTypeTitle.Create(str, WS_CHILD|WS_VISIBLE, CRect(0, 0, 69, 20), this);
	m_sticTypeTitle.SetColor(RGB(0, 0, 0), RGB(78, 165, 229));
	m_sticTypeTitle.SetLeftMargin(25);

	m_sticSepLine1.Create(_T(""), WS_CHILD|WS_VISIBLE, CRect(69, 0, 70, 204), this);
	m_sticSepLine1.SetColor(RGB(255, 255, 255), RGB(255, 255, 255));
	m_sticSepLine2.Create(_T(""), WS_CHILD|WS_VISIBLE, CRect(429, 0, 430, 204), this);
	m_sticSepLine2.SetColor(RGB(255, 255, 255), RGB(255, 255, 255));

	m_lsType.Create(WS_CHILD|WS_VISIBLE|LVS_REPORT|LVS_NOCOLUMNHEADER|LVS_NOSORTHEADER, CRect(0, 20, 69, 204), this, IDC_LIST_CONTACTINFO_TYPE, FALSE);
	m_lsType.SetListColor(RGB(255, 255, 255), RGB(235, 252, 255));
	m_lsType.SetExtendedStyle(m_lsType.GetExtendedStyle()|LVS_EX_FULLROWSELECT);
	m_lsType.InsertColumn(0, _T("Type"), LVCFMT_LEFT, 100);

	m_pImageList = new CImageList();
	m_pImageList->Create(12, 12, ILC_COLOR32|ILC_MASK, 2, 2);   
    HICON hIcon = ::LoadIcon(AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_ICON1));   
	m_pImageList->Add(hIcon);  
    hIcon = ::LoadIcon(AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_ICON2));   
	m_pImageList->Add(hIcon);   
	m_lsType.SetImageList(m_pImageList, LVSIL_SMALL);

	strTemp = Data::LanguageResource::Get(Data::RI_RECORD_IN);
	str = strTemp.c_str();
	m_lsType.InsertItem(0, str, 0);
	//m_lsType.SetItemText(0, 1, str);

	strTemp = Data::LanguageResource::Get(Data::RI_RECORD_INED);
	str = strTemp.c_str();
	m_lsType.InsertItem(1, str, 1);
	//m_lsType.SetItemText(0, 1, str);

	strTemp = Data::LanguageResource::Get(Data::RI_RECORD_OUT);
	str = strTemp.c_str();
	m_lsType.InsertItem(2, str, 0);
	//m_lsType.SetItemText(0, 1, str);

	strTemp = Data::LanguageResource::Get(Data::RI_RECORD_SEARCHRESULT);
	str = strTemp.c_str();
	m_lsType.InsertItem(3, str, 1);
	//m_lsType.SetItemText(0, 1, str);
	//m_lsType.SetScrollRagle(FALSE);

	strTemp = Data::LanguageResource::Get(Data::RI_RECORD_NAMENO);
	str = strTemp.c_str();
	m_sticNameNO.Create(str, WS_CHILD|WS_VISIBLE, CRect(70, 0, 220, 20), this);
	m_sticNameNO.SetColor(RGB(0, 0, 0), RGB(78, 165, 229));
	m_sticNameNO.SetLeftMargin(40);

	strTemp = Data::LanguageResource::Get(Data::RI_RECORD_TIME);
	str = strTemp.c_str();
	m_sticTime.Create(str, WS_CHILD|WS_VISIBLE, CRect(220, 0, 300, 20), this);
	m_sticTime.SetColor(RGB(0, 0, 0), RGB(78, 165, 229));
	m_sticTime.SetLeftMargin(25);

	strTemp = Data::LanguageResource::Get(Data::RI_RECORD_DURATION);
	str = strTemp.c_str();
	m_sticDuration.Create(str, WS_CHILD|WS_VISIBLE, CRect(300, 0, 355, 20), this);
	m_sticDuration.SetColor(RGB(0, 0, 0), RGB(78, 165, 229));
	m_sticDuration.SetLeftMargin(15);

	strTemp = Data::LanguageResource::Get(Data::RI_RECORD_SOUND);
	str = strTemp.c_str();
	m_sticSound.Create(str, WS_CHILD|WS_VISIBLE, CRect(355, 0, 430, 20), this);
	m_sticSound.SetColor(RGB(0, 0, 0), RGB(78, 165, 229));
	m_sticSound.SetLeftMargin(15);

	
	m_lsList.Create(WS_CHILD|WS_VISIBLE|LVS_REPORT|LVS_NOCOLUMNHEADER|LVS_NOSORTHEADER, CRect(70, 20, 429, 204), this, IDC_LIST_CONTACTINFO_LIST, TRUE);
	m_lsList.SetListColor(RGB(255, 255, 255), RGB(235, 252, 255));
	m_lsList.SetExtendedStyle(m_lsList.GetExtendedStyle()|LVS_EX_FULLROWSELECT);
	m_lsList.InsertColumn(0, _T("NameNO"), LVCFMT_LEFT, 150);
	m_lsList.InsertColumn(1, _T("Time"), LVCFMT_LEFT, 80);
	m_lsList.InsertColumn(2, _T("Duration"), LVCFMT_LEFT, 55);
	m_lsList.InsertColumn(3, _T("Sound"), LVCFMT_LEFT, 55);


// 	m_pImageList = new CImageList();
// 	m_pImageList->Create(12, 12, ILC_COLOR32|ILC_MASK, 2, 2);   
//  hIcon = ::LoadIcon(AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_ICON1));   
// 	m_pImageList->Add(hIcon);  
//  hIcon = ::LoadIcon(AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_ICON2));   
// 	m_pImageList->Add(hIcon);   

	m_lsList.SetImageList(m_pImageList, LVSIL_SMALL);

	strTemp = Data::LanguageResource::Get(Data::RI_RECORD_OPERATION);
	str = strTemp.c_str();
	m_sticOperation.Create(str, WS_CHILD|WS_VISIBLE, CRect(430, 0, 480, 20), this);
	m_sticOperation.SetColor(RGB(0, 0, 0), RGB(78, 165, 229));
	m_sticOperation.SetLeftMargin(10);

	strTemp = Data::LanguageResource::Get(Data::RI_RECORD_DIAL);
	str = strTemp.c_str();
	m_btnDial.Create(str, WS_CHILD|WS_VISIBLE, CRect(430, 20, 480, 40), this, IDC_BUTTON_CONTACTINFO_DIAL);
	SetButtonDefaultColor(&m_btnDial);

	strTemp = Data::LanguageResource::Get(Data::RI_RECORD_LISTENSOUND);
	str = strTemp.c_str();
	m_btnSound.Create(str, WS_CHILD|WS_VISIBLE, CRect(430, 40, 480, 60), this, IDC_BUTTON_CONTACTINFO_SOUND);
	SetButtonDefaultColor(&m_btnSound);
	
	strTemp = Data::LanguageResource::Get(Data::RI_RECORD_NOTE);
	str = strTemp.c_str();
	m_btnNote.Create(str, WS_CHILD|WS_VISIBLE, CRect(430, 60, 480, 80), this, IDC_BUTTON_CONTACTINFO_NOTE);
	SetButtonDefaultColor(&m_btnNote);
	
	strTemp = Data::LanguageResource::Get(Data::RI_RECORD_NEWCARD);
	str = strTemp.c_str();
	m_btnNewContact.Create(str, WS_CHILD|WS_VISIBLE, CRect(430, 80, 480, 100), this, IDC_BUTTON_CONTACTINFO_NEW);
	SetButtonDefaultColor(&m_btnNewContact);

	strTemp = Data::LanguageResource::Get(Data::RI_RECORD_SEARCH);
	str = strTemp.c_str();
	m_btnSearch.Create(str, WS_CHILD|WS_VISIBLE, CRect(430, 100, 480, 120), this, IDC_BUTTON_CONTACTINFO_SEARCH);
	SetButtonDefaultColor(&m_btnSearch);

	strTemp = Data::LanguageResource::Get(Data::RI_RECORD_DELETE);
	str = strTemp.c_str();
	m_btnDelete.Create(str, WS_CHILD|WS_VISIBLE, CRect(430, 184, 480, 204), this, IDC_BUTTON_CONTACTINFO_DELETE);
	SetButtonDefaultColor(&m_btnDelete);

	m_lsType.SetItemState(0, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
	ShowItemsInList();

	m_pPlaySoundDlg = new CPlaySoundDlg(this);
	m_pPlaySoundDlg->Create(CPlaySoundDlg::IDD);

	m_pSearchContactInfoDlg = new CContactInfoSearchDlg(this);
	m_pSearchContactInfoDlg->Create(CContactInfoSearchDlg::IDD);
	//m_pSearchContactInfoDlg->SetParent(this);

	m_pNoteDlg = new CNoteDlg(this);
	m_pNoteDlg->Create(CNoteDlg::IDD);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CContactInfoDlg::SetButtonDefaultColor(CCEButtonST* button)
{
	button->SetColor(CCEButtonST::BTNST_COLOR_BK_IN, RGB(248,214,147));
	button->SetColor(CCEButtonST::BTNST_COLOR_BK_OUT, RGB(248,214,147));
	button->SetColor(CCEButtonST::BTNST_COLOR_BK_FOCUS, RGB(248,214,147));
}

void CContactInfoDlg::SetButtonSelectedColor(CCEButtonST* button)
{
	button->SetColor(CCEButtonST::BTNST_COLOR_BK_IN, RGB(248,214,147));
	button->SetColor(CCEButtonST::BTNST_COLOR_BK_OUT, RGB(248,214,147));
	button->SetColor(CCEButtonST::BTNST_COLOR_BK_FOCUS, RGB(248,214,147));
}

void CContactInfoDlg::OnClickListType(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	ShowItemsInList();
	*pResult = 0;
}

void CContactInfoDlg::ShowItemsInList(void)
{
	POSITION pos = m_lsType.GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
		return;
	}

	int type = m_lsType.GetNextSelectedItem(pos);
	std::string filter;
	
	switch (type)
	{
	case 0:
		filter = "direction = 1 AND isConnect = 0";
		break;
	case 1:
		filter = "direction = 1 AND isConnect = 1";
		break;
	case 2:
		filter = "direction = 0";
		break;
	case 3:
		ShowArrayInList(m_vSearchResult);
		return;
		break;
	}

	std::vector<boost::shared_ptr<Data::ContactInfo> > result = Data::ContactInfo::GetFromDatabase(filter);
	ShowArrayInList(result);	
}

void CContactInfoDlg::ShowArrayInList(std::vector<boost::shared_ptr<Data::ContactInfo> > array)
{
	m_lsList.DeleteAllItems();
	CString temp;
	for (int i = 0; i < (array).size(); ++i)
	{	
// 		temp = GetName(array[i]->telephoneNumber().ToString()).c_str();
//		temp = GetName(array[i]->telephoneNumber().number()).c_str();
		if (array[i]->contact())
		{
			temp = (array[i]->contact()->name() + "(" + array[i]->telephoneNumber().number() + ")").c_str();
		}
		else
		{
			temp = array[i]->telephoneNumber().number().c_str();
		}
		m_lsList.InsertItem(i, temp, i % 2);
		
// 		temp.Format(_T("%d-%02d-%02d %02d:%02d"),array[i]->startTime().GetYear(),
// 									array[i]->startTime().GetMonth(),
// 									array[i]->startTime().GetDay(),
// 									array[i]->startTime().GetHour(),
// 									array[i]->startTime().GetMinute());
		temp.Format(_T("%2d-%02d %2d:%02d"),
									array[i]->startTime().GetMonth(),
									array[i]->startTime().GetDay(),
									array[i]->startTime().GetHour(),
									array[i]->startTime().GetMinute());

		m_lsList.SetItemText(i, 1, temp);

		temp.Format(_T("%02d:%02d"), array[i]->duration().GetMinutes(),
							array[i]->duration().GetSeconds());
		m_lsList.SetItemText(i, 2, temp);

		if (!array[i]->sounds().empty())
		{
			temp = array[i]->sounds().begin()->second->filename().c_str();
		}
		else
		{
			temp = TEXT("");
		}
		m_lsList.SetItemText(i, 3, temp);

		m_lsList.SetItemData(i, array[i]->id());
	}
	m_lsList.SetScrollRagle(FALSE);

	if (!array.empty())
	{
		POSITION pos = m_lsList.GetFirstSelectedItemPosition();   
		while (pos != NULL)
		{   
			int iSel = m_lsList.GetNextSelectedItem(pos);   
			m_lsList.SetItemState(iSel, 0, LVIS_SELECTED);   
		}
		m_lsList.SetItemState(0, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	}
}

std::string CContactInfoDlg::GetName(std::string number)
{
	if (number == "")
	{
		return number;
	}
	std::string filter;
	filter += "mobilesTelephone = '";
	filter += Data::TelephoneNumber(number).ToString();
	filter += "' OR mobilesTelephone2 = '";
	filter += Data::TelephoneNumber(number).ToString();
	filter += "' OR worksTelephone = '";
	filter += Data::TelephoneNumber(number).ToString();
	filter += "' OR worksTelephone2 = '";
	filter += Data::TelephoneNumber(number).ToString();
	filter += "'";
	std::vector<boost::shared_ptr<Data::Contact> > result = Data::Contact::GetFromDatabase(filter);
	if (!result.empty())
	{
		return result[0]->name() + "(" + number + ")";
	}
	return number;
}

void CContactInfoDlg::OnClickListList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here

// 	CString   str;   
// 	str=m_lsType.GetItemText(pos,1);
		
	*pResult = 0;
}

void CContactInfoDlg::OnButtonDial() 
{
	// TODO: Add your control notification handler code here
	POSITION pos = m_lsList.GetFirstSelectedItemPosition();
	if (pos != NULL)
	{
		int index = m_lsList.GetNextSelectedItem (pos); 
		CString number = m_lsList.GetItemText(index, 0);
		if (number.IsEmpty())
		{
			return;
		}
		
		std::string str = Util::StringOp::FromCString(number);
		if (number.Right(1) != _T(")"))
		{
			if (str != "")
			{
				((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pTelephoneDlg->DialContact(str);
			}
		}
		else
		{
			int pos1 = str.find('(');
			int pos2 = str.find(')');
			if ((pos1 != std::string.npos) && (pos2 != std::string.npos))
			{
				std::string s = str.substr(pos1 + 1, pos2 - pos1 - 1);
				if (s != "")
				{
					std::string filter;
					filter = "id = " + Util::StringOp::FromInt(m_lsList.GetItemData(index));
					std::vector<boost::shared_ptr<Data::ContactInfo> > result = Data::ContactInfo::GetFromDatabase(filter);
					if (!result.empty() && result[0]->contact())
					{
						((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pTelephoneDlg->DialContact(s, result[0]->contact()->id());
					}
					else
					{
						((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pTelephoneDlg->DialContact(s);
					}
				}
			}
		}
	}
}

void CContactInfoDlg::OnButtonSound() 
{
	// TODO: Add your control notification handler code here
	m_pPlaySoundDlg->ShowWindow(TRUE);	
}

void CContactInfoDlg::OnButtonNote() 
{
	// TODO: Add your control notification handler code here
	m_pNoteDlg->ShowWindow(TRUE);
}

void CContactInfoDlg::OnButtonNew() 
{
	// TODO: Add your control notification handler code here

	((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->SwitchPanel_(IDC_BUTTON_CONTACT);
	boost::shared_ptr<Data::Contact> contact(new Data::Contact);
	contact->id(0);
	POSITION pos = m_lsList.GetFirstSelectedItemPosition();
	if (pos != NULL)
	{
		int index = m_lsList.GetNextSelectedItem (pos); 
		CString number = m_lsList.GetItemText(index, 0);
		if (number.Right(1) != _T(")"))
		{
			if (!number.IsEmpty() && isdigit(number[0]))
			{
				if ((number.GetLength() > 10) && (number[0] == '1'))
				{
					contact->mobilesTelephone(Util::StringOp::FromCString(number));
				}
				else
				{
					contact->worksTelephone(Util::StringOp::FromCString(number));
				}
			}		
		}
	}
	((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactNewDlg->SetContact(contact);
	((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactNewDlg->ShowWindow(TRUE);
}

void CContactInfoDlg::OnButtonSearch() 
{
	// TODO: Add your control notification handler code here
 	m_pSearchContactInfoDlg->ShowWindow(TRUE);
}

void CContactInfoDlg::OnButtonDelete() 
{
	// TODO: Add your control notification handler code here
	POSITION pos = m_lsList.GetFirstSelectedItemPosition();
	if (pos != NULL)
	{
		int index = m_lsList.GetNextSelectedItem (pos);   		
		int contactinfoID = m_lsList.GetItemData(index);
		Data::ContactInfo::Remove("id = " + Util::StringOp::FromInt(contactinfoID));
		m_lsList.DeleteItem(index);

		if (!m_vSearchResult.empty())
		{
			for (std::vector<boost::shared_ptr<Data::ContactInfo> >::iterator iter = m_vSearchResult.begin(); iter != m_vSearchResult.end(); ++iter)
			{
				if ((*iter)->id() == contactinfoID)
				{
					m_vSearchResult.erase(iter);
					break;
				}
			}
		}

		if (m_lsList.GetItemCount() > 0)
		{
			POSITION pos = m_lsList.GetFirstSelectedItemPosition();   
			while (pos != NULL)
			{   
				int iSel = m_lsList.GetNextSelectedItem(pos);   
				m_lsList.SetItemState(iSel, 0, LVIS_SELECTED);   
			}

			if (m_lsList.GetItemCount() - 1 < index)
			{
				index -= 1;	
			}
			m_lsList.SetItemState(index, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
		}
	}
}

void CContactInfoDlg::Search(std::string filter)
{
 	std::vector<boost::shared_ptr<Data::ContactInfo> > result = Data::ContactInfo::GetFromDatabase(filter);
	ShowSearchResultInList(result);
}
	
void CContactInfoDlg::ShowSearchResultInList(std::vector<boost::shared_ptr<Data::ContactInfo> > array)
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

LRESULT CContactInfoDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class	
	if (message == WM_USER + 5)
	{
	}
	return CDialog::WindowProc(message, wParam, lParam);
}
