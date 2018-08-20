// ContactInfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "../MultimediaPhone.h"
#include "ContactInfoDlg.h"
#include "../Data/LanguageResource.h"
#include "../Data/Data.h"
#include "../Data/Contact.h"
#include "../MultimediaPhoneDlg.h"
#include "../Data/SkinStyle.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CContactInfoDlg dialog

const int ContactInfoTotal = 30000;
const int PageSize = 9;

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
	ON_BN_CLICKED(IDC_BUTTON_CONTACTINFO_CLOSE, OnButtonClose)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CContactInfoDlg message handlers

BOOL CContactInfoDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	std::string strTemp;
	CString str;

	m_lsType.Create(WS_CHILD|WS_VISIBLE|LVS_REPORT|LVS_NOCOLUMNHEADER|LVS_NOSORTHEADER, CRect(8, 27, 80, 198), this, IDC_LIST_CONTACTINFO_TYPE, FALSE);
	m_lsType.SetExtendedStyle(m_lsType.GetExtendedStyle()|LVS_EX_FULLROWSELECT);
	m_lsType.InsertColumn(0, _T("Type"), LVCFMT_LEFT, 72);

	m_pImageList = new CImageList();
	m_pImageList->Create(16, 16, ILC_COLOR32|ILC_MASK, 4, 2); 
	CBitmap bm;
	bm.LoadBitmap(IDB_BITMAP1);
	m_pImageList->Add(&bm, RGB(255, 0, 255));
	bm.DeleteObject();
	bm.LoadBitmap(IDB_BITMAP2);
	m_pImageList->Add(&bm, RGB(255, 0, 255));
	bm.DeleteObject();
	bm.LoadBitmap(IDB_BITMAP3);
	m_pImageList->Add(&bm, RGB(255, 0, 255));
	bm.DeleteObject();
	bm.LoadBitmap(IDB_BITMAP4);
	m_pImageList->Add(&bm, RGB(255, 0, 255));
	bm.DeleteObject();

	m_lsType.SetImageList(m_pImageList, LVSIL_SMALL);

	strTemp = Data::LanguageResource::Get(Data::RI_RECORD_IN);
	str = strTemp.c_str();
	m_lsType.InsertItem(0, str, 1);
	//m_lsType.SetItemText(0, 1, str);

	strTemp = Data::LanguageResource::Get(Data::RI_RECORD_INED);
	str = strTemp.c_str();
	m_lsType.InsertItem(1, str, 2);
	//m_lsType.SetItemText(0, 1, str);

	strTemp = Data::LanguageResource::Get(Data::RI_RECORD_OUT);
	str = strTemp.c_str();
	m_lsType.InsertItem(2, str, 3);
	//m_lsType.SetItemText(0, 1, str);

	strTemp = Data::LanguageResource::Get(Data::RI_RECORD_SEARCHRESULT);
	str = strTemp.c_str();
	m_lsType.InsertItem(3, str, 0);
	//m_lsType.SetItemText(0, 1, str);
	//m_lsType.SetScrollRagle(FALSE);
	
	m_lsList.Create(WS_CHILD|WS_VISIBLE|LVS_REPORT|LVS_NOCOLUMNHEADER|LVS_NOSORTHEADER, CRect(81, 27, 416, 198), this, IDC_LIST_CONTACTINFO_LIST, TRUE, 0, FALSE);
	m_lsList.SetExtendedStyle(m_lsList.GetExtendedStyle()|LVS_EX_FULLROWSELECT|LVS_EX_SUBITEMIMAGES);
	m_lsList.InsertColumn(0, _T("NameNO"), LVCFMT_LEFT, 140);
	m_lsList.InsertColumn(1, _T("Time"), LVCFMT_LEFT, 85);
	m_lsList.InsertColumn(2, _T("Duration"), LVCFMT_LEFT, 65);
	m_lsList.InsertColumn(3, _T("Sound"), LVCFMT_LEFT, 25);


	m_pImageList = new CImageList();
	m_pImageList->Create(16, 16, ILC_COLOR32|ILC_MASK, 6, 2); 
	bm.LoadBitmap(IDB_BITMAP20);
	m_pImageList->Add(&bm, RGB(255, 0, 255));
	bm.DeleteObject();
	bm.LoadBitmap(IDB_BITMAP22);
	m_pImageList->Add(&bm, RGB(255, 0, 255));
	bm.DeleteObject();
	bm.LoadBitmap(IDB_BITMAP21);
	m_pImageList->Add(&bm, RGB(255, 0, 255));
	bm.DeleteObject();
	bm.LoadBitmap(IDB_BITMAP19);
	m_pImageList->Add(&bm, RGB(255, 0, 255));
	bm.DeleteObject();
	bm.LoadBitmap(IDB_BITMAP24);
	m_pImageList->Add(&bm, RGB(255, 0, 255));
	bm.DeleteObject();
	bm.LoadBitmap(IDB_BITMAP32);
	m_pImageList->Add(&bm, RGB(255, 0, 255));
	bm.DeleteObject();

	m_lsList.SetImageList(m_pImageList, LVSIL_SMALL);

	strTemp = Data::LanguageResource::Get(Data::RI_RECORD_DIAL);
	str = strTemp.c_str();
	m_btnDial.Create(str, Data::g_buttonRectBMPID[0][Data::g_skinstyle], Data::g_buttonRectBMPID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(417, 26, 471, 45), this, IDC_BUTTON_CONTACTINFO_DIAL);
//	m_btnDial.SetBackRGB(Data::g_allFramBackRGB[Data::g_skinstyle]);

	strTemp = Data::LanguageResource::Get(Data::RI_RECORD_LISTENSOUND);
	str = strTemp.c_str();
	m_btnSound.Create(str, Data::g_buttonRectBMPID[0][Data::g_skinstyle], Data::g_buttonRectBMPID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(417, 46, 471, 65), this, IDC_BUTTON_CONTACTINFO_SOUND);
//	m_btnSound.SetBackRGB(Data::g_allFramBackRGB[Data::g_skinstyle]);
	
 	strTemp = Data::LanguageResource::Get(Data::RI_RECORD_NOTE);
 	str = strTemp.c_str();
 	m_btnNote.Create(str, Data::g_buttonRectBMPID[0][Data::g_skinstyle], Data::g_buttonRectBMPID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(417, 126, 471, 145), this, IDC_BUTTON_CONTACTINFO_NOTE);
 //	m_btnNote.SetBackRGB(Data::g_allFramBackRGB[Data::g_skinstyle]);
	
	strTemp = Data::LanguageResource::Get(Data::RI_RECORD_NEWCARD);
	str = strTemp.c_str();
	m_btnNewContact.Create(str, Data::g_buttonRectBMPID[0][Data::g_skinstyle], Data::g_buttonRectBMPID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(417, 66, 471, 85), this, IDC_BUTTON_CONTACTINFO_NEW);
//	m_btnNewContact.SetBackRGB(Data::g_allFramBackRGB[Data::g_skinstyle]);

	strTemp = Data::LanguageResource::Get(Data::RI_RECORD_SEARCH);
	str = strTemp.c_str();
	m_btnSearch.Create(str, Data::g_buttonRectBMPID[0][Data::g_skinstyle], Data::g_buttonRectBMPID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(417, 86, 471, 105), this, IDC_BUTTON_CONTACTINFO_SEARCH);
//	m_btnSearch.SetBackRGB(Data::g_allFramBackRGB[Data::g_skinstyle]);

	strTemp = Data::LanguageResource::Get(Data::RI_RECORD_DELETE);
	str = strTemp.c_str();
	m_btnDelete.Create(str, Data::g_buttonRectBMPID[0][Data::g_skinstyle], Data::g_buttonRectBMPID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(417, 106, 471, 125), this, IDC_BUTTON_CONTACTINFO_DELETE);
//	m_btnDelete.SetBackRGB(Data::g_allFramBackRGB[Data::g_skinstyle]);

	strTemp = Data::LanguageResource::Get(Data::RI_SOUND_PLAY_CLOSE);
	str = strTemp.c_str();
	m_btnClose.Create(str, Data::g_buttonRectBMPID[0][Data::g_skinstyle], Data::g_buttonRectBMPID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(417, 178, 471, 198), this, IDC_BUTTON_CONTACTINFO_CLOSE);
	m_btnClose.SetBackRGB(Data::g_allFramBackRGB[Data::g_skinstyle]);

	m_sticPanel.Create(_T(""), WS_CHILD|WS_VISIBLE, CRect(416, 26, 472, 197), this);
	m_sticPanel.SetColor(RGB(0, 0, 0), Data::g_allFrameInFrameLine1RGB[0][Data::g_skinstyle]);

	m_sticBackground.Create(CRect(0, 0, 480, 204), this, 2);
	
	TextStruct ts[6];
	memset(ts, 0, sizeof(TextStruct) * 6);
	
	ts[0].txtRect = CRect(0, 0, 80, 20);
	ts[0].txtFontSize = 16;
	ts[0].sAlign = DT_CENTER | DT_BOTTOM;
	memcpy(ts[0].sTxt, Data::LanguageResource::Get(Data::RI_RECORD_TYPE).c_str(), Data::LanguageResource::Get(Data::RI_RECORD_TYPE).length());
	
	ts[1].txtRect = CRect(81, 0, 220, 20);
	ts[1].txtFontSize = 16;
	ts[1].sAlign = DT_CENTER | DT_BOTTOM;
	memcpy(ts[1].sTxt, Data::LanguageResource::Get(Data::RI_RECORD_NAMENO).c_str(), Data::LanguageResource::Get(Data::RI_RECORD_NAMENO).length());
	
	ts[2].txtRect = CRect(220, 0, 305, 20);
	ts[2].txtFontSize = 16;
	ts[2].sAlign = DT_CENTER | DT_BOTTOM;
	memcpy(ts[2].sTxt, Data::LanguageResource::Get(Data::RI_RECORD_TIME).c_str(), Data::LanguageResource::Get(Data::RI_RECORD_TIME).length());
	
	ts[3].txtRect = CRect(305, 0, 365, 20);
	ts[3].txtFontSize = 16;
	ts[3].sAlign = DT_CENTER | DT_BOTTOM;
	memcpy(ts[3].sTxt, Data::LanguageResource::Get(Data::RI_RECORD_DURATION).c_str(), Data::LanguageResource::Get(Data::RI_RECORD_DURATION).length());
	
	ts[4].txtRect = CRect(366, 0, 396, 20);
	ts[4].txtFontSize = 16;
	ts[4].sAlign = DT_CENTER | DT_BOTTOM;
	memcpy(ts[4].sTxt, Data::LanguageResource::Get(Data::RI_RECORD_SOUND).c_str(), Data::LanguageResource::Get(Data::RI_RECORD_SOUND).length());
	
	ts[5].txtRect = CRect(417, 0, 470, 20);
	ts[5].txtFontSize = 16;
	ts[5].sAlign = DT_CENTER | DT_BOTTOM;
	memcpy(ts[5].sTxt, Data::LanguageResource::Get(Data::RI_RECORD_OPERATION).c_str(), Data::LanguageResource::Get(Data::RI_RECORD_OPERATION).length());
	
	m_sticBackground.SetTextStruct(ts, 6);

	m_pPlaySoundDlg = new CPlaySoundDlg(this);
	m_pPlaySoundDlg->Create(CPlaySoundDlg::IDD);
	m_pPlaySoundDlg->SetModel(1);

	m_pSearchContactInfoDlg = new CContactInfoSearchDlg(this);
	m_pSearchContactInfoDlg->Create(CContactInfoSearchDlg::IDD);

	m_sListFilter = "";
	m_sListSearchFilter = "";
	m_lsType.SetItemState(0, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);	
	ShowItemsInList();

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

	m_lsList.DeleteAllItems();
	int type = m_lsType.GetNextSelectedItem(pos);
	std::string filter;

	if (type == 0)
	{
		m_sListFilter = "type = 0";
		filter = m_sListFilter;
	}
	else if (type == 1)
	{
		m_sListFilter = "type = 1";
		filter = m_sListFilter;
	}
	else if (type == 2)
	{
		m_sListFilter = "type = 2";
		filter = m_sListFilter;
	}
	else if (type == 3)
	{
		if (m_sListSearchFilter == "")
		{
			return;
		}
		filter = m_sListSearchFilter;
	}

	if (Data::ContactInfo::GetDataCount("") > 0)
	{
		m_vCurrentResult = Data::ContactInfo::GetFromDatabase(filter, Data::dDown, Data::ContactInfo::GetCurrentId() + 1, PageSize);
		int count = Data::ContactInfo::GetDataCount(filter);
		m_lsList.m_scollbar_.SetRangle(0, count, PageSize);
		ShowArrayInList(m_vCurrentResult, type);	
		m_lsList.SetScrollRagle(TRUE);
	}
}

void CContactInfoDlg::ScrollItemsInList(int step)
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
	
	std::vector<boost::shared_ptr<Data::ContactInfo> > temp;
	int currentID = 0;
	
	if (step == SB_LINEUP)
	{
		currentID = m_vCurrentResult[0]->id();
		temp = Data::ContactInfo::GetFromDatabase(filter, Data::dUp, currentID, 1);
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
		temp = Data::ContactInfo::GetFromDatabase(filter, Data::dDown, currentID, 1);
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
		temp = Data::ContactInfo::GetFromDatabase(filter, Data::dUp, currentID, PageSize);
		if (temp.size() < PageSize)
		{
			temp.insert(temp.end(), m_vCurrentResult.begin(), m_vCurrentResult.begin() + PageSize - temp.size());
		}
		m_vCurrentResult = temp;
	}
	else if(step == SB_PAGEDOWN)
	{
		currentID = m_vCurrentResult[m_vCurrentResult.size() - 1]->id();
		temp = Data::ContactInfo::GetFromDatabase(filter, Data::dDown, currentID, PageSize);
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

void CContactInfoDlg::ShowArrayInList(std::vector<boost::shared_ptr<Data::ContactInfo> > array, int type)
{
	m_lsList.DeleteAllItems();
	UINT unconnectCount = 0;
	if (type == 0)
	{
		unconnectCount = ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pTelephoneDlg->GetUnconnectCount();
	}
	CString temp;
	for (int i = 0; i < (array).size(); ++i)
	{	
// 			if (array[i]->GetContact())
// 			{
// 				temp = (array[i]->GetContact()->name() + "(" + array[i]->telephoneNumber().number() + ")").c_str();
// 			}
// 			else
// 			{
// 				temp = array[i]->telephoneNumber().number().c_str();
// 			}
			if (array[i]->name() != "")
			{
				temp = (array[i]->name() + "(" + array[i]->telephoneNumber().number() + ")").c_str();
			}
			else
			{
				temp = array[i]->telephoneNumber().number().c_str();
			}
				
			if (type == 0 && i < unconnectCount)
			{
				m_lsList.InsertItem(i, temp, 0);
			}
			else
			{
				m_lsList.InsertItem(i, temp, 1);
			}

			temp.Format(_T("%02d-%02d %2d:%02d"),
										array[i]->startTime().GetMonth(),
										array[i]->startTime().GetDay(),
										array[i]->startTime().GetHour(),
										array[i]->startTime().GetMinute());

			m_lsList.SetItemText(i, 1, temp);

			if (array[i]->duration().GetHours() > 0)
			{
				temp.Format(_T("%d:%02d:%02d"), array[i]->duration().GetHours(),array[i]->duration().GetMinutes(),
					array[i]->duration().GetSeconds());
			}
			else
			{
				temp.Format(_T("%02d:%02d"), array[i]->duration().GetMinutes(),
					array[i]->duration().GetSeconds());
			}
			m_lsList.SetItemText(i, 2, temp);

			if (array[i]->isSound())
			{
				m_lsList.SetItem(i, 3, LVIF_TEXT | LVIF_IMAGE , _T(" "), 5, NULL, NULL, 0);
			}
 
 			m_lsList.SetItemData(i, array[i]->id());
	}

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
					if (!result.empty() && result[0]->GetContact())
					{
						((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pTelephoneDlg->DialContact(s, result[0]->GetContact()->id());
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
	POSITION pos = m_lsList.GetFirstSelectedItemPosition();
	if (pos != NULL)
	{
 		int index = m_lsList.GetNextSelectedItem (pos);   	

		if (!m_lsList.GetItemText(index, 3).IsEmpty())
		{
			pos = m_lsType.GetFirstSelectedItemPosition();
			m_pPlaySoundDlg->SetSound(m_lsType.GetNextSelectedItem(pos), m_lsList.GetItemData(index), 0, m_sListSearchFilter);
			m_pPlaySoundDlg->ShowWindow(TRUE);
		}
	}
}

void CContactInfoDlg::OnButtonNote() 
{
	// TODO: Add your control notification handler code here
// 	m_pNoteDlg->ShowWindow(TRUE);
	((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pNotebookDlg->ShowWindow(SW_SHOW);
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
		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetHWnd(this->GetSafeHwnd());
		std::string strTemp = Data::LanguageResource::Get(Data::RI_DELETETIP_CONTACTINFO);
		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetDelTip(strTemp.c_str());
		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->ShowWindow(TRUE);
	}
}

void CContactInfoDlg::OnButtonClose()
{
	((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->SwitchPanel_(IDC_BUTTON_MAIN);
}

void CContactInfoDlg::DeleteSelectedItem(void)
{
	POSITION pos = m_lsList.GetFirstSelectedItemPosition();
	if (pos != NULL)
	{
		int index = m_lsList.GetNextSelectedItem (pos);   		
// 		int contactinfoID = m_lsList.GetItemData(index);
// 		Data::ContactInfo::Remove("id = " + Util::StringOp::FromInt(contactinfoID));
		m_vCurrentResult[index]->Remove();
		ShowItemsInList();		
	}
}

void CContactInfoDlg::SaveContactInfo(boost::shared_ptr<Data::ContactInfo> ci)
{
	ci->Insert();
//	m_vContactInfo.insert(m_vContactInfo.begin(), ci);
	if (ci->type() == Data::citInNo)
	{
		POSITION pos = m_lsType.GetFirstSelectedItemPosition();   
		while (pos != NULL)
		{   
			int iSel = m_lsType.GetNextSelectedItem(pos);   
			m_lsType.SetItemState(iSel, 0, LVIS_SELECTED);   
		}
		m_lsType.SetItemState(0, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	}

	ShowItemsInList();
	if (Data::ContactInfo::GetDataCount("") >= ContactInfoTotal)
	{
		MessageBox(Util::StringOp::ToCString(Data::LanguageResource::Get(Data::RI_SYSTEM_DISK)));
		if (Data::ContactInfo::GetDataCount("") > ContactInfoTotal)
		{
			std::vector<boost::shared_ptr<Data::ContactInfo> > temp = Data::ContactInfo::GetFromDatabase("", Data::dUp, 0, 1);
			if (!temp.empty())
			{
				std::string filter = "id = ";
				filter += Util::StringOp::FromInt(temp[0]->id());
				Data::ContactInfo::Remove(filter);
			}
		}
	}
}

void CContactInfoDlg::Search(std::string filter)
{
	m_sListSearchFilter = filter;
	POSITION pos = m_lsType.GetFirstSelectedItemPosition();   
	while (pos != NULL)
	{   
		int iSel = m_lsType.GetNextSelectedItem(pos);   
		m_lsType.SetItemState(iSel, 0, LVIS_SELECTED);   
	}
    m_lsType.SetItemState(m_lsType.GetItemCount() - 1, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	ShowItemsInList();
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

int CContactInfoDlg::GetSoundID(int& listID, int offset)
{
	int id = listID + offset;
	
	if ((id >= 0) && (id < m_lsList.GetItemCount()))
	{
		if (m_lsList.GetItemText(id, 3).IsEmpty())
		{
			if (offset == 0)
			{
				return -1;
			}
			else
			{	
				while ((id >= 0) && (id < m_lsList.GetItemCount()))
				{
					if (!m_lsList.GetItemText(id, 3).IsEmpty())
					{
						listID = id;
						return m_lsList.GetItemData(id);//return contactinfo'id
						break;
					}
					id += offset;
				}
			}
		}
		else
		{
			listID = id;
			return m_lsList.GetItemData(listID);
		}
	}
		
	return -1;
}

std::string CContactInfoDlg::GetSoundNameNo(int listID)
{
	if ((listID >= 0) && (listID < m_lsList.GetItemCount()))
	{
		CString nameno = m_lsList.GetItemText(listID, 0);
		return Util::StringOp::FromCString(nameno);
	}
	else
	{
		return "";
	}
}

void CContactInfoDlg::ShowUnconnectItems(void)
{
	((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->SwitchPanel_(IDC_BUTTON_CONTACTINFO);
	POSITION pos = m_lsType.GetFirstSelectedItemPosition();   
	while (pos != NULL)
	{   
		int iSel = m_lsType.GetNextSelectedItem(pos);   
		m_lsType.SetItemState(iSel, 0, LVIS_SELECTED);   
	}
    m_lsType.SetItemState(0, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
		
	ShowItemsInList();
}

LRESULT CContactInfoDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class	
	switch (message)
	{
	case WM_DELETESELITEM:
		DeleteSelectedItem();
		break;
	case WM_VSCROLL:
		ScrollItemsInList(wParam);
		break;
	};
	return CDialog::WindowProc(message, wParam, lParam);
}
