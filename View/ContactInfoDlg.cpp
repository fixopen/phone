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

extern int g_iContactCount;
int g_iContactInfoCount = 0;
/////////////////////////////////////////////////////////////////////////////
// CContactInfoDlg dialog

CContactInfoDlg::CContactInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CContactInfoDlg::IDD, pParent)
	, ContactInfoTotal(10000)
	, PageSize(7)
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
	ON_WM_TIMER()
	ON_WM_MEASUREITEM()
	//}}AFX_MSG_MAP
	ON_NOTIFY(NM_CLICK, IDC_LIST_CONTACTINFO_TYPE, OnClickListType)
	ON_NOTIFY(NM_CLICK, IDC_LIST_CONTACTINFO_LIST, OnClickListList)
	ON_MESSAGE(WM_LISTCTRL_CLICK, OnListCltrlClick)
	ON_BN_CLICKED(IDC_BUTTON_CONTACTINFO_DIAL, OnButtonDial)
	ON_BN_CLICKED(IDC_BUTTON_CONTACTINFO_SOUND, OnButtonSound)
	ON_BN_CLICKED(IDC_BUTTON_CONTACTINFO_NOTE, OnButtonNote)
	ON_BN_CLICKED(IDC_BUTTON_CONTACTINFO_NEW, OnButtonNew)
	ON_BN_CLICKED(IDC_BUTTON_CONTACTINFO_SEARCH, OnButtonSearch)
	ON_BN_CLICKED(IDC_BUTTON_CONTACTINFO_DELETE, OnButtonDelete)
	ON_BN_CLICKED(IDC_BUTTON_DETAIL_SHOW_TIP,OnButtonDetail)
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
	
	m_lsType.m_nLineHeight = 25;
	m_lsType.m_nFontHeight = 16;
	m_lsType.Create(WS_CHILD|WS_VISIBLE|\
		LVS_REPORT|LVS_NOCOLUMNHEADER|LVS_NOSORTHEADER/*|LVS_OWNERDRAWFIXED*/,\
		CRect(6, 27*VHEIGHT, 80, 198*VHEIGHT), this, IDC_LIST_CONTACTINFO_TYPE, FALSE);
	m_lsType.SetExtendedStyle(m_lsType.GetExtendedStyle()|LVS_EX_FULLROWSELECT);
	m_lsType.InsertColumn(0, _T("Type"), LVCFMT_LEFT, 76);
	
	m_pImageList = new CImageList();
	m_pImageList->Create(16, m_lsType.m_nLineHeight, ILC_COLOR32|ILC_MASK, 4, 2); 
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

	strTemp = Data::LanguageResource::Get(Data::RI_RECORD_INED);
	str = strTemp.c_str();
	m_lsType.InsertItem(1, str, 2);

	strTemp = Data::LanguageResource::Get(Data::RI_RECORD_OUT);
	str = strTemp.c_str();
	m_lsType.InsertItem(2, str, 3);

	strTemp = Data::LanguageResource::Get(Data::RI_RECORD_SEARCHRESULT);
	str = strTemp.c_str();
	m_lsType.InsertItem(3, str, 0);
	
	m_lsList.m_nLineHeight = 27;
	m_lsList.Create(WS_CHILD|WS_VISIBLE|LVS_REPORT|LVS_NOCOLUMNHEADER|LVS_NOSORTHEADER, \
		CRect(81, 27*VHEIGHT, 416, 198*VHEIGHT), this, IDC_LIST_CONTACTINFO_LIST,\
		TRUE, 0, FALSE);
	m_lsList.SetExtendedStyle(m_lsList.GetExtendedStyle()|LVS_EX_FULLROWSELECT|LVS_EX_SUBITEMIMAGES);
	m_lsList.InsertColumn(0, _T("NameNO"), LVCFMT_LEFT, 135);
	m_lsList.InsertColumn(1, _T("Time"), LVCFMT_LEFT, 85+20);
	m_lsList.InsertColumn(2, _T("Duration"), LVCFMT_LEFT, 50);
	m_lsList.InsertColumn(3, _T("Sound"), LVCFMT_LEFT, 25);

	m_pImageList = new CImageList();
	m_pImageList->Create(16, m_lsList.m_nLineHeight, ILC_COLOR32|ILC_MASK, 6, 2); 
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
	bm.LoadBitmap(IDB_BITMAP26);
	m_pImageList->Add(&bm, RGB(255, 0, 255));
	bm.DeleteObject();

	m_lsList.SetImageList(m_pImageList, LVSIL_SMALL);

	strTemp = Data::LanguageResource::Get(Data::RI_RECORD_DIAL);
	str = strTemp.c_str();
	m_btnDial.Create(str, Data::g_buttonRectBMPID[0][Data::g_skinstyle], Data::g_buttonRectBMPID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(417, 26*VHEIGHT, 471, 45*VHEIGHT), this, IDC_BUTTON_CONTACTINFO_DIAL);

	strTemp = Data::LanguageResource::Get(Data::RI_RECORD_LISTENSOUND);
	str = strTemp.c_str();
	m_btnSound.Create(str, Data::g_buttonRectBMPID[0][Data::g_skinstyle], Data::g_buttonRectBMPID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(417, 46*VHEIGHT, 471, 65*VHEIGHT), this, IDC_BUTTON_CONTACTINFO_SOUND);

	
	strTemp = Data::LanguageResource::Get(Data::RI_RECORD_NEWCARD);
	str = strTemp.c_str();
	m_btnNewContact.Create(str, Data::g_buttonRectBMPID[0][Data::g_skinstyle], Data::g_buttonRectBMPID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(417, 66*VHEIGHT, 471, 85*VHEIGHT), this, IDC_BUTTON_CONTACTINFO_NEW);

	strTemp = Data::LanguageResource::Get(Data::RI_RECORD_SEARCH);
	str = strTemp.c_str();
	m_btnSearch.Create(str, Data::g_buttonRectBMPID[0][Data::g_skinstyle], Data::g_buttonRectBMPID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(417, 86*VHEIGHT, 471, 105*VHEIGHT), this, IDC_BUTTON_CONTACTINFO_SEARCH);

	strTemp = Data::LanguageResource::Get(Data::RI_RECORD_DELETE);
	str = strTemp.c_str();
	m_btnDelete.Create(str, Data::g_buttonRectBMPID[0][Data::g_skinstyle], Data::g_buttonRectBMPID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(417, 106*VHEIGHT, 471, 125*VHEIGHT), this, IDC_BUTTON_CONTACTINFO_DELETE);

	strTemp = Data::LanguageResource::Get(Data::DETAIL_TIP);
	str = strTemp.c_str();
	m_btnDetail.Create(str, Data::g_buttonRectBMPID[0][Data::g_skinstyle], Data::g_buttonRectBMPID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(417, 126*VHEIGHT, 471, 145*VHEIGHT), this, IDC_BUTTON_DETAIL_SHOW_TIP);

	strTemp = Data::LanguageResource::Get(Data::RI_SOUND_PLAY_CLOSE);
	str = strTemp.c_str();
	m_btnClose.Create(str, Data::g_buttonRectBMPID[0][Data::g_skinstyle], Data::g_buttonRectBMPID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(417, 178*VHEIGHT, 471, 198*VHEIGHT), this, IDC_BUTTON_CONTACTINFO_CLOSE);
	m_btnClose.SetBackRGB(Data::g_allFramBackRGB[Data::g_skinstyle]);

	m_sticPanel.Create(_T(""), WS_CHILD|WS_VISIBLE, CRect(416,26*VHEIGHT,472,197*VHEIGHT), this);
	m_sticPanel.SetColor(RGB(0, 0, 0), Data::g_allFrameInFrameLine1RGB[0][Data::g_skinstyle]);

	m_sticBackground.Create(CRect(0, 0, 480, 204*VHEIGHT), this, 2);
	
	TextStruct ts[6];
	memset(ts, 0, sizeof(TextStruct) * 6);
	
	ts[0].txtRect = CRect(0, 0, 80, 20*VHEIGHT);
	ts[0].txtFontSize = 16;
	ts[0].sAlign = DT_CENTER | DT_BOTTOM;
	memcpy(ts[0].sTxt, Data::LanguageResource::Get(Data::RI_RECORD_TYPE).c_str(),\
		Data::LanguageResource::Get(Data::RI_RECORD_TYPE).length());
	
	ts[1].txtRect = CRect(81, 0, 220, 20*VHEIGHT);
	ts[1].txtFontSize = 16;
	ts[1].sAlign = DT_CENTER | DT_BOTTOM;
	memcpy(ts[1].sTxt, Data::LanguageResource::Get(Data::RI_RECORD_NAMENO).c_str(),\
		Data::LanguageResource::Get(Data::RI_RECORD_NAMENO).length());
	
	ts[2].txtRect = CRect(220, 0, 305, 20*VHEIGHT);
	ts[2].txtFontSize = 16;
	ts[2].sAlign = DT_CENTER | DT_BOTTOM;
	memcpy(ts[2].sTxt, Data::LanguageResource::Get(Data::RI_RECORD_TIME).c_str(),\
		Data::LanguageResource::Get(Data::RI_RECORD_TIME).length());
	
	ts[3].txtRect = CRect(305, 0, 365, 20*VHEIGHT);
	ts[3].txtFontSize = 16;
	ts[3].sAlign = DT_CENTER | DT_BOTTOM;
	memcpy(ts[3].sTxt, Data::LanguageResource::Get(Data::RI_RECORD_DURATION).c_str(),\
		Data::LanguageResource::Get(Data::RI_RECORD_DURATION).length());
	
	ts[4].txtRect = CRect(366, 0, 396, 20*VHEIGHT);
	ts[4].txtFontSize = 16;
	ts[4].sAlign = DT_CENTER | DT_BOTTOM;
	memcpy(ts[4].sTxt, Data::LanguageResource::Get(Data::RI_RECORD_SOUND).c_str(),\
		Data::LanguageResource::Get(Data::RI_RECORD_SOUND).length());
	
	ts[5].txtRect = CRect(417, 0, 475, 20*VHEIGHT);
	ts[5].txtFontSize = 16;
	ts[5].sAlign = DT_CENTER | DT_BOTTOM;
	memcpy(ts[5].sTxt, Data::LanguageResource::Get(Data::RI_RECORD_OPERATION).c_str(),\
		Data::LanguageResource::Get(Data::RI_RECORD_OPERATION).length());
	
	m_sticBackground.SetTextStruct(ts, 6);

	m_pPlaySoundDlg = new CPlaySoundDlg(this);
	m_pPlaySoundDlg->Create(CPlaySoundDlg::IDD);
	m_pPlaySoundDlg->SetModel(1);

	m_pSearchContactInfoDlg = new CContactInfoSearchDlg(this);
	m_pSearchContactInfoDlg->Create(CContactInfoSearchDlg::IDD);

    m_pDetailTip=new CDetailTip(this);
    m_pDetailTip->Create(CDetailTip::IDD);
    
	m_pPasswordDlg = new CPasswordDlg(this);
	m_pPasswordDlg->Create(CPasswordDlg::IDD);

	m_bTelephoneInUse = false;
	
	m_sListFilter = "";
	m_sListSearchFilter = "";
	g_iContactInfoCount = Data::ContactInfo::GetDataCount("");  //wzx 20101231
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
			m_lsList.DeleteAllItems();

			return;
		}
		filter = m_sListSearchFilter;
	}

	if (g_iContactInfoCount > 0)
	{
		m_vCurrentResult = Data::ContactInfo::GetFromDatabase(filter, Data::dDown, Data::ContactInfo::GetCurrentId() + 1, PageSize);
		int count = Data::ContactInfo::GetDataCount(filter);
		m_lsList.m_scollbar_.SetRangle(0, count, PageSize);
		ShowArrayInList(m_vCurrentResult, type);	
		m_lsList.SetScrollRagle(TRUE);
    	m_lsList.SetItemState(0,LVIS_SELECTED|LVIS_FOCUSED,LVIS_FOCUSED|LVIS_SELECTED);
	}
	else
	{
		m_lsList.DeleteAllItems();
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
	int iSel = 0;

	POSITION pos = m_lsList.GetFirstSelectedItemPosition(); 
	
	if (pos != NULL)
	{   
		iSel = m_lsList.GetNextSelectedItem(pos);   
	}

	m_lsList.DeleteAllItems();

	CString temp;

	for (int i = 0; i < (array).size(); ++i)
	{	
			if (array[i]->name() != "")
			{
				temp = Util::StringOp::ToCString((array[i]->name() + "(" + array[i]->telephoneNumber().number() + ")"));
			}
			else
			{
				temp = Util::StringOp::ToCString(array[i]->telephoneNumber().number());
			}
				
			if (!array[i]->played())
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

			if (array[i]->duration() > 3600)
			{
				temp.Format(_T("%d:%02d:%02d"), array[i]->duration() / 3600 ,array[i]->duration() % 3600 / 60, array[i]->duration() % 60);
			}
			else
			{
				temp.Format(_T("%d:%02d"), array[i]->duration() / 60, array[i]->duration() % 60);
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
		if (iSel > array.size() - 1)
		{
			iSel = array.size() - 1;
		}

		m_lsList.SetItemState(iSel, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
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
	POSITION pos = m_lsList.GetFirstSelectedItemPosition();

	if (pos != NULL)
	{
		int index = m_lsList.GetNextSelectedItem (pos); 
		std::string strTemp;
		CString str;

		if (m_vCurrentResult[index]->contactId() > 0)
		{
			strTemp = Data::LanguageResource::Get(Data::RI_MAIN_CARDCASEBTN);
		}
		else
		{
			strTemp = Data::LanguageResource::Get(Data::RI_RECORD_NEWCARD);
		}
		str = strTemp.c_str();

		m_btnNewContact.SetWindowText(str);

		if (!m_vCurrentResult[index]->played())
		{
			m_vCurrentResult[index]->played(true);
			m_vCurrentResult[index]->Update();
			ShowArrayInList(m_vCurrentResult);
			if (((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pMainDlg->m_nUnconnectCount > 0)
			{
				((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pMainDlg->m_nUnconnectCount--;
				::PostMessage(((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pMainDlg->GetSafeHwnd(), WM_TELNOTIFY, 0, 0);
			}
		}
	}

	*pResult = 0;
}

void CContactInfoDlg::OnListCltrlClick(WPARAM w, LPARAM l)
{
	LRESULT ret;
	if(w == IDC_LIST_CONTACTINFO_TYPE)
		OnClickListType(NULL, &ret);
	else if(w == IDC_LIST_CONTACTINFO_LIST)
		OnClickListList(NULL, &ret);
}

void CContactInfoDlg::ReDialNumber()
{
	return;
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

					if (!result.empty() && result[0]->contactId() > 0)
					{
						((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pTelephoneDlg->DialContact(s, result[0]->contactId());
					}
					else
					{
						((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pTelephoneDlg->DialContact(s);
					}

				}
			}
		}
	}
	else
	{
		// add by yy 20111027
		CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
		std::string strTemp = Data::LanguageResource::Get(Data::RI_CHOOSEITEM);
        main->m_pCCWarningDlg->SetDelTip(strTemp.c_str());		
		main->m_pCCWarningDlg->ShowWindow_(SW_SHOW);
		return;
	}
}
void CContactInfoDlg::OnButtonplaysound()
{
	POSITION pos = m_lsList.GetFirstSelectedItemPosition();
	
	if (pos != NULL)
	{
		int index = m_lsList.GetNextSelectedItem (pos);
		if (m_vCurrentResult[index]->isSound())
		{
			CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
			pos = m_lsType.GetFirstSelectedItemPosition();
			m_pPlaySoundDlg->SetSound(m_lsType.GetNextSelectedItem(pos), m_lsList.GetItemData(index), 0, m_sListSearchFilter);
			if (main->m_pSettingDlg->m_pSetting->isPlayProtect() && !main->m_pSettingDlg->m_bLogin && main->m_pSoundDlg->m_bPopPasswordDlg)
			{	
				std::string strTemp = main->m_pSettingDlg->m_pSetting->playRecordPassword();
				
				//change by qi 20111104
				if (!strTemp.empty())
				{
					m_pPasswordDlg->SetType(CHECK_PASSWORD);
					m_pPasswordDlg->SetOldPassWord((char *)strTemp.c_str());
					m_pPasswordDlg->SetHWnd(this->m_hWnd);
					m_pPasswordDlg->ShowWindow(SW_SHOW);
				}
				else
				{	
					//密码为空不用输入密码
					m_pPlaySoundDlg->SetModel(1);
					m_pPlaySoundDlg->SetAutoClose(TRUE);
					m_pPlaySoundDlg->ShowWindow(TRUE);
					m_pPlaySoundDlg->StartPlay(TRUE);
				}
		
			}
			else
			{ 
				m_pPlaySoundDlg->SetModel(1);
				m_pPlaySoundDlg->SetAutoClose(TRUE);
				m_pPlaySoundDlg->ShowWindow(TRUE);
				m_pPlaySoundDlg->StartPlay(TRUE);			  
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
		if (m_vCurrentResult[index]->isSound())
		{
			CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
			pos = m_lsType.GetFirstSelectedItemPosition();
			m_pPlaySoundDlg->SetSound(m_lsType.GetNextSelectedItem(pos), m_lsList.GetItemData(index), 0, m_sListSearchFilter);
			
			std::string strTemp = main->m_pSettingDlg->m_pSetting->playRecordPassword();
			if (main->m_pSettingDlg->m_pSetting->isPlayProtect() &&\
				!main->m_pSettingDlg->m_bLogin &&\
				main->m_pSoundDlg->m_bPopPasswordDlg &&\
				!strTemp.empty())//add by qi 20111104 密码为空不用输入密码
			{
				m_pPasswordDlg->SetType(CHECK_PASSWORD);
				m_pPasswordDlg->SetOldPassWord((char *)strTemp.c_str());
				m_pPasswordDlg->SetHWnd(this->m_hWnd);
				m_pPasswordDlg->ShowWindow(SW_SHOW);	
			}
			else
			{
				m_pPlaySoundDlg->StartPlay(false);
				m_pPlaySoundDlg->ShowWindow(TRUE);
			}
		}
	}
	else
	{
		// add by yy 20111027
		CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
		std::string strTemp = Data::LanguageResource::Get(Data::RI_CHOOSEITEM);
        main->m_pCCWarningDlg->SetDelTip(strTemp.c_str());		
		main->m_pCCWarningDlg->ShowWindow_(SW_SHOW);
		return;
	}
}

void CContactInfoDlg::OnButtonNote() 
{
	// TODO: Add your control notification handler code here
    // 	m_pNoteDlg->ShowWindow(TRUE);
}

void CContactInfoDlg::OnButtonNew() 
{
	// TODO: Add your control notification handler code here
	std::string str;
	str=Data::LanguageResource::Get(Data::RI_ERROR);
	if (g_iContactCount < ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactDlg->ContactTotal)
	{
		POSITION pos = m_lsList.GetFirstSelectedItemPosition();
		if (pos != NULL)
		{
			int index = m_lsList.GetNextSelectedItem (pos); 
			if (m_vCurrentResult[index]->contactId() > 0)
			{
				std::string filter;
				filter = "id = " + Util::StringOp::FromInt(m_vCurrentResult[index]->contactId());
				std::vector<boost::shared_ptr<Data::Contact> > result = Data::Contact::GetFromDatabase(filter);
				if (!result.empty())
				{
					((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->SwitchPanel_(IDC_BUTTON_CONTACT);
					((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactNewDlg->SetContact(result[0]);
					((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactNewDlg->ShowWindow(TRUE);
				}
				else
				{
					((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetPasswordModel(false);
					((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetTitle(str.c_str());
					((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->setM_bIsNewContact(true);
					((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetDelTip(Util::StringOp::ToCString(Data::LanguageResource::Get(Data::RI_NO_CONTACT)));
		            ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->ShowWindow_(SW_SHOW);	    
				}
			}
			else
			{
				boost::shared_ptr<Data::Contact> contact(new Data::Contact);
				contact->id(0);
				CString number = m_lsList.GetItemText(index, 0);
				number.TrimLeft();
				number.TrimRight();
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
				else
				{   
					std::string number_=Util::StringOp::FromCString(number);
					int pos1 = number_.find('(');
					int pos2 = number_.rfind(')');
					std::string name_;
					name_=number_.substr(0,pos1);
					number_ =number_.substr(pos1+1,pos2-pos1-1);
					if ((number_.size()>10)&&number_[0]=='1')
					{
						contact->mobilesTelephone(number_);
					}
					else
					{
						contact->worksTelephone(number_);
					}
					contact->name(name_);
				}

				((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->SwitchPanel_(IDC_BUTTON_CONTACT);
				((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactNewDlg->SetContact(contact);
				((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactNewDlg->ShowWindow(TRUE);
			}
		}
		else
		{
			// add by yy 20111027
			CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
			std::string strTemp = Data::LanguageResource::Get(Data::RI_CHOOSEITEM);
			main->m_pCCWarningDlg->SetDelTip(strTemp.c_str());		
			main->m_pCCWarningDlg->ShowWindow_(SW_SHOW);
			return;

		}

	}
	else
	{
		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetPasswordModel(false);
		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetTitle(str.c_str());
		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->setM_bIsNewContact(false);
		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetDelTip(Util::StringOp::ToCString(Data::LanguageResource::Get(Data::RI_SYSTEM_CONTACT_TIP)));
		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->ShowWindow_(SW_SHOW);
	}
}

void CContactInfoDlg::OnButtonSearch() 
{
	// TODO: Add your control notification handler code here
	m_pSearchContactInfoDlg->SetDataSerchTime();
	m_pSearchContactInfoDlg->ShowWindow(TRUE);
}

void CContactInfoDlg::OnButtonDelete() 
{
	// TODO: Add your control notification handler code here
	POSITION pos = m_lsList.GetFirstSelectedItemPosition();

	if (pos != NULL)
	{	
		//add by qi 20111104
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
		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetHWnd(this->GetSafeHwnd());

		std::string strTemp = Data::LanguageResource::Get(Data::RI_DELETETIP_CONTACTINFO);

		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetDelTip(strTemp.c_str());

		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->ShowWindow(TRUE);
	}
	else
	{
		// add by yy 20111027
		CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
		std::string strTemp = Data::LanguageResource::Get(Data::RI_CHOOSEITEM);
        main->m_pCCWarningDlg->SetDelTip(strTemp.c_str());		
		main->m_pCCWarningDlg->ShowWindow_(SW_SHOW);
		return;
	}
}

void CContactInfoDlg::OnButtonClose()
{
	ShowItemsInList();

	ShowWindow(FALSE);
	///zzw0910//////
	//ShowWindow_(SW_HIDE);

	((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->SwitchPanel_(IDC_BUTTON_MAIN);
}

void CContactInfoDlg::DeleteSelectedItem(void)
{
	POSITION pos = m_lsList.GetFirstSelectedItemPosition();
	if (pos != NULL)
	{
		int index = m_lsList.GetNextSelectedItem (pos);   		

		if (!m_vCurrentResult[index]->played())
		{
			((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pMainDlg->m_nUnconnectCount--;

			::PostMessage(((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pMainDlg->GetSafeHwnd(), WM_TELNOTIFY, 0, 0);
		}

		m_vCurrentResult[index]->Remove();
		--g_iContactInfoCount;
		ShowItemsInList();		
	}
}

////////zzw100823/////////////////
void CContactInfoDlg::SaveContactInfo(boost::shared_ptr<Data::ContactInfo> ci)
{
	std::string str;

	str=Data::LanguageResource::Get(Data::RI_CARDINFO_WARNING);
	//只有在拨打时的号码为空时才删除通话记录
    if (ci->telephoneNumber().number() == "" && ci->type() == Data::citOut)
    {  
		if (!theApp.m_bIsCreateRecord)
		{
		   ci->Remove();
		}
		return;
    }

	if (ci->id() == 0)
	{  
		ci->Insert();
	}
	else
	{
		ci->Update();
	}
	
	++g_iContactInfoCount;
	if (g_iContactInfoCount > ContactInfoTotal)
	{
		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetPasswordModel(false);
		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetTitle(str.c_str());
		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetDelTip(Util::StringOp::ToCString(Data::LanguageResource::Get(Data::RI_SYSTEM_CONTACTINFO_TIP)));
		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->ShowWindow_(SW_SHOW);
	
		std::vector<boost::shared_ptr<Data::ContactInfo> > temp = Data::ContactInfo::GetFromDatabase("", Data::dUp, 0, 1);
		if (!temp.empty())
		{
			temp[0]->Remove();
		}
		--g_iContactInfoCount;
	}

	ShowItemsInList();
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

	case WM_OK:
		{
			POSITION pos = m_lsList.GetFirstSelectedItemPosition();
			if (pos != NULL)
			{
				int index = m_lsList.GetNextSelectedItem (pos); 
				std::string strTemp;
				CString str;
				
				if (m_vCurrentResult[index]->contactId() > 0)
				{
					strTemp = Data::LanguageResource::Get(Data::RI_MAIN_CARDCASEBTN);
				}
				else
				{
					strTemp = Data::LanguageResource::Get(Data::RI_RECORD_NEWCARD);
				}
				str = strTemp.c_str();
				m_btnNewContact.SetWindowText(str);
				
				if (!m_vCurrentResult[index]->played())
				{
					m_vCurrentResult[index]->played(true);
					m_vCurrentResult[index]->Update();
					ShowArrayInList(m_vCurrentResult);
					if (((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pMainDlg->m_nUnconnectCount > 0)
					{
						((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pMainDlg->m_nUnconnectCount--;
						::PostMessage(((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pMainDlg->GetSafeHwnd(), WM_TELNOTIFY, 0, 0);
					}
				}
			}
		}
		break;

	case WM_CHECKPASSWORD:

		SetTimer(50,800,0);
		
		break;
	case WM_TEL_INUSE:

		if (!m_bTelephoneInUse)
		{
			m_bTelephoneInUse = true;

			m_pPlaySoundDlg->CloseSound();
		}

		break;

	case WM_TEL_NOUSE:

		m_bTelephoneInUse = false;

		break;

	case WM_CHANGELANGUAGE:

          ChangeLanguage();

	    break;

	case WM_KEYDOWN:

		if (((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactInfoDlg->IsWindowVisible())
		{
			int i;
			POSITION pos=m_lsList.GetFirstSelectedItemPosition();

			if (pos!=NULL)
			{
				iSel =m_lsList.GetNextSelectedItem(pos);
				i=iSel;

				if (wParam=='U')
				{
					i--;
					m_lsList.SetExtendedStyle(m_lsList.GetExtendedStyle()|LVS_EX_FULLROWSELECT);
					m_lsList.SetItemState(i,LVIS_SELECTED|LVIS_FOCUSED,LVIS_SELECTED|LVIS_FOCUSED);
					if (i==-1)
					{
						ScrollItemsInList(SB_LINEUP);
						int nPos = m_lsList.m_scollbar_.GetPos();
						nPos--;
						m_lsList.m_scollbar_.SetPos(nPos);
						m_lsList.SetItemState(0,LVIS_SELECTED|LVIS_FOCUSED,LVIS_SELECTED|LVIS_FOCUSED); 
					}
				}
				
			if (wParam=='D')
			{  
				 i++;
			
				 m_lsList.SetExtendedStyle(m_lsList.GetExtendedStyle()|LVS_EX_FULLROWSELECT);
				 m_lsList.SetItemState(i,LVIS_SELECTED|LVIS_FOCUSED,LVIS_SELECTED|LVIS_FOCUSED);
				 
				 if (i == m_lsList.GetItemCount())
				 {
					 //ScrollItemsInList(SB_PAGEDOWN);  //wzx 20101230
					 ScrollItemsInList(SB_LINEDOWN);
					 int nPos = m_lsList.m_scollbar_.GetPos();
					 if(nPos < m_lsList.m_scollbar_.m_nMax - i)
					 {
						 nPos++;
						 m_lsList.m_scollbar_.SetPos(nPos);
						 m_lsList.SetItemState(m_lsList.GetItemCount()-1,LVIS_SELECTED|LVIS_FOCUSED,LVIS_SELECTED|LVIS_FOCUSED);
					 }
				 }
			 }
			 m_lsList.Invalidate();
		}
	}
	break;

	case WM_CANCEL:
		OnButtonClose();
		break;

    case WM_PLAYLEAVEWORD:
		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactInfoDlg->OnButtonplaysound();  
	
		break;

	case WM_DELETE:
		if (((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactInfoDlg->IsWindowVisible())
		{
			if (((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pSoundDlg->m_pPlaySoundDlg->IsWindowVisible())
			{
				return 0;
			}else
			{
			if(m_lsList.GetItemCount()>0)
			{
				POSITION pos=m_lsList.GetFirstSelectedItemPosition();
				if (pos!=NULL)
				{	
					CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
					if (main->m_pSettingDlg->m_pSetting->isAdmin() && \
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
					
					iSel =m_lsList.GetNextSelectedItem(pos);                
					((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetHWnd(m_hWnd);
					std::string strTemp = Data::LanguageResource::Get(Data::RI_DELETETIP_CONTACTINFO);
					((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetDelTip(strTemp.c_str());
					((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->ShowWindow(TRUE);
				}
			}
			}
		}
		break;

	case WM_ITYPE:
        if (((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactInfoDlg->IsWindowVisible())
		{
            POSITION pos = m_lsType.GetFirstSelectedItemPosition();
			if (pos != NULL)
			{
				int type = m_lsType.GetNextSelectedItem(pos);
				type++;
				m_lsType.SetExtendedStyle(m_lsType.GetExtendedStyle()|LVS_EX_FULLROWSELECT);
				m_lsType.SetItemState(type,LVIS_SELECTED|LVIS_FOCUSED,LVIS_SELECTED|LVIS_FOCUSED);
				if (type==4)
				{
					type=0;
					m_lsType.SetExtendedStyle(m_lsType.GetExtendedStyle()|LVS_EX_FULLROWSELECT);
					m_lsType.SetItemState(type,LVIS_SELECTED|LVIS_FOCUSED,LVIS_SELECTED|LVIS_FOCUSED);
				}
			}
			ShowItemsInList();
			m_lsType.Invalidate();
		}
		break;
	case WM_SEARCH:
        if (((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactInfoDlg->IsWindowVisible())
		{
		m_pSearchContactInfoDlg->ShowWindow(TRUE);
		}
		break;
	case WM_NEWCONTACT:

		if (((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactInfoDlg->IsWindowVisible())
		{   
			POSITION pos = m_lsList.GetFirstSelectedItemPosition();
			
			if (pos != NULL)
			{
				int index = m_lsList.GetNextSelectedItem (pos); 
				
				{
					boost::shared_ptr<Data::Contact> contact(new Data::Contact);
					
					contact->id(0);
					
					CString number = m_lsList.GetItemText(index, 0);
					
					number.TrimRight();
                    
					number.TrimLeft();
					
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
					else
					{   
						std::string number_=Util::StringOp::FromCString(number);
                        int pos1 = number_.find('(');
						int pos2 = number_.rfind(')');
						std::string name_;
						name_=number_.substr(0,pos1);
						number_=number_.substr(pos1+1,pos2-pos1-1);
						if ((number_.size()>10)&&number_[0]=='1')
						{
							contact->mobilesTelephone(number_);
						}
						else
						{
                            contact->worksTelephone(number_);
						}
						contact->name(name_);
					}
					
					((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->SwitchPanel_(IDC_BUTTON_CONTACT);
					((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactNewDlg->SetContact(contact);
					((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactNewDlg->ShowWindow(TRUE);
				}
			}
		}
		break;
	case WM_TEL_KEYCODE:
	        if(!(((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->IsWindowVisible()||
				m_pPlaySoundDlg->IsWindowVisible()||m_pSearchContactInfoDlg->IsWindowVisible()||
				((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pMainDlg->m_mainScreenSaveDlg_->IsWindowVisible()||
				m_pDetailTip->IsWindowVisible()))
	        {
				std::string c;
				c=wParam;
				findfilter+=c;
			    SetTimer(IDT_SEARCHER_TIME,500,NULL);
	        }
	 break;
    case WM_KEYNEWCONTACT:
		OnButtonNew();
		break;
	}
	return CDialog::WindowProc(message, wParam, lParam);
}

void CContactInfoDlg::OnTimer(UINT nIDEvent)
{
	std::string filter="";
	if (IDT_SEARCHER_TIME==nIDEvent)
	{
		KillTimer(IDT_SEARCHER_TIME);
		
		filter += "(telephoneNumber Like '%";		
	
		filter += Data::TelephoneNumber(findfilter).ToString();
		
		filter += "%')";
		Search(filter);
	}
	findfilter="";
	if (50==nIDEvent)
	{
        KillTimer(50);
		m_pPlaySoundDlg->SetAutoClose(TRUE);
		m_pPlaySoundDlg->ShowWindow(TRUE);
		m_pPlaySoundDlg->StartPlay(TRUE);

	}
	CDialog::OnTimer(nIDEvent);
	}
void CContactInfoDlg::ChangeLanguage()
{
	std::string strTemp;
	CString str;
    m_lsType.DeleteAllItems( ); 
    strTemp = Data::LanguageResource::Get(Data::RI_RECORD_IN);
	str = strTemp.c_str();
	m_lsType.InsertItem(0, str, 1);

    strTemp = Data::LanguageResource::Get(Data::RI_RECORD_INED);
	str = strTemp.c_str();
	m_lsType.InsertItem(1, str, 2);

	strTemp = Data::LanguageResource::Get(Data::RI_RECORD_OUT);
	str = strTemp.c_str();
	m_lsType.InsertItem(2, str, 3);

    strTemp = Data::LanguageResource::Get(Data::RI_RECORD_SEARCHRESULT);
	str = strTemp.c_str();
	m_lsType.InsertItem(3, str, 0);

    m_lsType.SetItemState(0, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
	ShowItemsInList();
	strTemp = Data::LanguageResource::Get(Data::RI_RECORD_DIAL);
	str = strTemp.c_str();
    m_btnDial.SetWindowText(str);

	strTemp = Data::LanguageResource::Get(Data::RI_RECORD_LISTENSOUND);
	str = strTemp.c_str();
	m_btnSound.SetWindowText(str);

	strTemp = Data::LanguageResource::Get(Data::RI_RECORD_NEWCARD);
	str = strTemp.c_str();
    m_btnNewContact.SetWindowText(str);

	strTemp = Data::LanguageResource::Get(Data::RI_RECORD_DELETE);
	str = strTemp.c_str();
	m_btnDelete.SetWindowText(str);

	strTemp = Data::LanguageResource::Get(Data::DETAIL_TIP);
	str = strTemp.c_str();
	m_btnDetail.SetWindowText(str);

   	strTemp = Data::LanguageResource::Get(Data::RI_SOUND_PLAY_CLOSE);
	str = strTemp.c_str();
    m_btnClose.SetWindowText(str);

	strTemp = Data::LanguageResource::Get(Data::RI_RECORD_SEARCH);
	str = strTemp.c_str();
	m_btnSearch.SetWindowText(str);

	TextStruct ts[6];
	memset(ts, 0, sizeof(TextStruct) * 6);
	
	ts[0].txtRect = CRect(0, 0, 80, 20*VHEIGHT);
	ts[0].txtFontSize = 16;
	ts[0].sAlign = DT_CENTER | DT_BOTTOM;
	memcpy(ts[0].sTxt, Data::LanguageResource::Get(Data::RI_RECORD_TYPE).c_str(), Data::LanguageResource::Get(Data::RI_RECORD_TYPE).length());
	
	ts[1].txtRect = CRect(81, 0, 220, 20*VHEIGHT);
	ts[1].txtFontSize = 16;
	ts[1].sAlign = DT_CENTER | DT_BOTTOM;
	memcpy(ts[1].sTxt, Data::LanguageResource::Get(Data::RI_RECORD_NAMENO).c_str(), Data::LanguageResource::Get(Data::RI_RECORD_NAMENO).length());
	
	ts[2].txtRect = CRect(220, 0, 305, 20*VHEIGHT);
	ts[2].txtFontSize = 16;
	ts[2].sAlign = DT_CENTER | DT_BOTTOM;
	memcpy(ts[2].sTxt, Data::LanguageResource::Get(Data::RI_RECORD_TIME).c_str(), Data::LanguageResource::Get(Data::RI_RECORD_TIME).length());
	
	ts[3].txtRect = CRect(305, 0, 365, 20*VHEIGHT);
	ts[3].txtFontSize = 16;
	ts[3].sAlign = DT_CENTER | DT_BOTTOM;
	memcpy(ts[3].sTxt, Data::LanguageResource::Get(Data::RI_RECORD_DURATION).c_str(), Data::LanguageResource::Get(Data::RI_RECORD_DURATION).length());
	
	ts[4].txtRect = CRect(366, 0, 396, 20*VHEIGHT);
	ts[4].txtFontSize = 16;
	ts[4].sAlign = DT_CENTER | DT_BOTTOM;
	memcpy(ts[4].sTxt, Data::LanguageResource::Get(Data::RI_RECORD_SOUND).c_str(), Data::LanguageResource::Get(Data::RI_RECORD_SOUND).length());
	
	ts[5].txtRect = CRect(417, 0, 475, 20*VHEIGHT);
	ts[5].txtFontSize = 16;
	ts[5].sAlign = DT_CENTER | DT_BOTTOM;
	memcpy(ts[5].sTxt, Data::LanguageResource::Get(Data::RI_RECORD_OPERATION).c_str(), Data::LanguageResource::Get(Data::RI_RECORD_OPERATION).length());
	m_sticBackground.SetTextStruct(ts, 6);

}
void CContactInfoDlg::OnButtonDetail()
{  
   POSITION pos = m_lsList.GetFirstSelectedItemPosition();

   if(pos!=NULL)
   {   
        
	   int nCount=m_lsList.GetSelectedCount();

	   int bPrompt = nCount>1 ? 0 : 1;
		
		if (!bPrompt)
		{
			int nRes = MessageBox(_T("您选择了多条记录，您确定要删除这些记录吗？"), _T("提示"), MB_OKCANCEL|MB_ICONQUESTION);
			
			if (IDCANCEL == nRes) 
			{
				m_lsList.SetRedraw(TRUE);
				
				return;
			}
		}
     	   
       int index = m_lsList.GetNextSelectedItem (pos);

	   CString NameNoText=m_lsList.GetItemText(index,0);

	   CString NameText;

	   CString NumberText;

	   int indexPos=NameNoText.Find('(');

	   if (indexPos==-1)
	   {   
		   
           std::string  strTemp=Data::LanguageResource::Get(Data::STRANGERS);
          
		   NameText=strTemp.c_str();
           
		   NumberText=NameNoText;
	   }else
	   {

		   NameText=NameNoText.Mid(0,indexPos);
		   
	       NumberText=NameNoText.Mid(indexPos+1,NameNoText.GetLength()-indexPos-2);

	   }
	   CString StartTime=m_lsList.GetItemText(index,1);

	   CString Duration=m_lsList.GetItemText(index,2);

	   CString RecordCaption=m_lsList.GetItemText(index,3);

	   m_pDetailTip->setHwnd(this->m_hWnd);
    
	   m_pDetailTip->setDetialContent(NameText,NumberText,StartTime,Duration,RecordCaption,false);

	   m_pDetailTip->ShowWindow(TRUE);

	   //ShowItemsInList();
  }
  else
  {
	  // add by yy 20111027
	  CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	  std::string strTemp = Data::LanguageResource::Get(Data::RI_CHOOSEITEM);
	  main->m_pCCWarningDlg->SetDelTip(strTemp.c_str());		
	  main->m_pCCWarningDlg->ShowWindow_(SW_SHOW);
	  return;

  }

}

void CContactInfoDlg::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
	// TODO: Add your message handler code here and/or call default
	
	if(nIDCtl   ==  IDC_LIST_CONTACTINFO_TYPE) 
	{ 
		lpMeasureItemStruct-> itemHeight   =   34; 
	} 

	CDialog::OnMeasureItem(nIDCtl, lpMeasureItemStruct);
}
