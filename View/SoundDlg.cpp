// SoundDlg.cpp : implementation file
//

#include "stdafx.h"
#include "../MultimediaPhone.h"
#include "SoundDlg.h"
#include "../Data/LanguageResource.h"
#include "../Data/Data.h"
#include "../Data/ContactInfo.h"
#include "../Data/Contact.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSoundDlg dialog


CSoundDlg::CSoundDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSoundDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSoundDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CSoundDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSoundDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSoundDlg, CDialog)
	//{{AFX_MSG_MAP(CSoundDlg)
	//}}AFX_MSG_MAP
	ON_NOTIFY(NM_CLICK, IDC_LIST_SOUND_TYPE, OnClickListType)
	ON_NOTIFY(NM_CLICK, IDC_LIST_SOUND_LIST, OnClickListList)
	ON_BN_CLICKED(IDC_BUTTON_SOUND_SOUND, OnButtonSound)
	ON_BN_CLICKED(IDC_BUTTON_SOUND_SEARCH, OnButtonSearch)
	ON_BN_CLICKED(IDC_BUTTON_SOUND_PROTECT, OnButtonProtect)
	ON_BN_CLICKED(IDC_BUTTON_SOUND_DELETE, OnButtonDelete)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSoundDlg message handlers

BOOL CSoundDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
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

	m_lsType.Create(WS_CHILD|WS_VISIBLE|LVS_REPORT|LVS_NOCOLUMNHEADER|LVS_NOSORTHEADER, CRect(0, 20, 69, 204), this, IDC_LIST_SOUND_TYPE, FALSE);
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

	strTemp = Data::LanguageResource::Get(Data::RI_SOUND_SOUND);
	str = strTemp.c_str();
	m_lsType.InsertItem(0, str, 0);

	strTemp = Data::LanguageResource::Get(Data::RI_SOUND_RECORD);
	str = strTemp.c_str();
	m_lsType.InsertItem(1, str, 1);

	strTemp = Data::LanguageResource::Get(Data::RI_RECORD_SEARCHRESULT);
	str = strTemp.c_str();
	m_lsType.InsertItem(2, str, 0);
	m_lsType.SetScrollRagle(FALSE);

	strTemp = Data::LanguageResource::Get(Data::RI_RECORD_SOUND);
	str = strTemp.c_str();
	m_sticSound.Create(str, WS_CHILD|WS_VISIBLE, CRect(70, 0, 125, 20), this);
	m_sticSound.SetColor(RGB(0, 0, 0), RGB(78, 165, 229));
	m_sticSound.SetLeftMargin(25);

	strTemp = Data::LanguageResource::Get(Data::RI_RECORD_NAMENO);
	str = strTemp.c_str();
	m_sticNameNO.Create(str, WS_CHILD|WS_VISIBLE, CRect(125, 0, 275, 20), this);
	m_sticNameNO.SetColor(RGB(0, 0, 0), RGB(78, 165, 229));
	m_sticNameNO.SetLeftMargin(25);

	strTemp = Data::LanguageResource::Get(Data::RI_RECORD_TIME);
	str = strTemp.c_str();
	m_sticTime.Create(str, WS_CHILD|WS_VISIBLE, CRect(275, 0, 350, 20), this);
	m_sticTime.SetColor(RGB(0, 0, 0), RGB(78, 165, 229));
	m_sticTime.SetLeftMargin(25);

	strTemp = Data::LanguageResource::Get(Data::RI_RECORD_DURATION);
	str = strTemp.c_str();
	m_sticDuration.Create(str, WS_CHILD|WS_VISIBLE, CRect(350, 0, 430, 20), this);
	m_sticDuration.SetColor(RGB(0, 0, 0), RGB(78, 165, 229));
	m_sticDuration.SetLeftMargin(25);

	
	m_lsList.Create(WS_CHILD|WS_VISIBLE|LVS_REPORT|LVS_NOCOLUMNHEADER|LVS_NOSORTHEADER, CRect(70, 20, 429, 204), this, IDC_LIST_SOUND_LIST, TRUE);
	m_lsList.SetListColor(RGB(255, 255, 255), RGB(235, 252, 255));
	m_lsList.SetExtendedStyle(m_lsList.GetExtendedStyle()|LVS_EX_FULLROWSELECT);
	m_lsList.InsertColumn(0, _T("Sound"), LVCFMT_LEFT, 55);
	m_lsList.InsertColumn(1, _T("NameNO"), LVCFMT_LEFT, 150);
	m_lsList.InsertColumn(2, _T("Time"), LVCFMT_LEFT, 80);
	m_lsList.InsertColumn(3, _T("Duration"), LVCFMT_LEFT, 55);


	m_pImageList = new CImageList();
	m_pImageList->Create(12, 12, ILC_COLOR32|ILC_MASK, 2, 2);   
    hIcon = ::LoadIcon(AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_ICON1));   
	m_pImageList->Add(hIcon);  
    hIcon = ::LoadIcon(AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_ICON2));   
	m_pImageList->Add(hIcon);   

	m_lsList.SetImageList(m_pImageList, LVSIL_SMALL);

	strTemp = Data::LanguageResource::Get(Data::RI_RECORD_OPERATION);
	str = strTemp.c_str();
	m_sticOperation.Create(str, WS_CHILD|WS_VISIBLE, CRect(430, 0, 480, 20), this);
	m_sticOperation.SetColor(RGB(0, 0, 0), RGB(78, 165, 229));
	m_sticOperation.SetLeftMargin(10);

	strTemp = Data::LanguageResource::Get(Data::RI_SOUND_PLAY);
	str = strTemp.c_str();
	m_btnSound.Create(str, WS_CHILD|WS_VISIBLE, CRect(430, 20, 480, 40), this, IDC_BUTTON_SOUND_SOUND);
	SetButtonDefaultColor(&m_btnSound);

	strTemp = Data::LanguageResource::Get(Data::RI_COMN_FINDBTN);
	str = strTemp.c_str();
	m_btnSearch.Create(str, WS_CHILD|WS_VISIBLE, CRect(430, 40, 480, 60), this, IDC_BUTTON_SOUND_SEARCH);
	SetButtonDefaultColor(&m_btnSearch);
	
	strTemp = Data::LanguageResource::Get(Data::RI_SOUND_PROTECT);
	str = strTemp.c_str();
	m_btnProtect.Create(str, WS_CHILD|WS_VISIBLE, CRect(430, 164, 480, 184), this, IDC_BUTTON_SOUND_PROTECT);
	SetButtonDefaultColor(&m_btnProtect);

	strTemp = Data::LanguageResource::Get(Data::RI_RECORD_DELETE);
	str = strTemp.c_str();
	m_btnDelete.Create(str, WS_CHILD|WS_VISIBLE, CRect(430, 184, 480, 204), this, IDC_BUTTON_SOUND_DELETE);
	SetButtonDefaultColor(&m_btnDelete);

	m_pSoundSearchDlg = new CSoundSearchDlg(this);
	m_pSoundSearchDlg->Create(CSoundSearchDlg::IDD);

	m_pPasswordDlg = new CPasswordDlg(this);
	m_pPasswordDlg->Create(CPasswordDlg::IDD);

	m_pDeleteTipDlg = new CDeleteTipDlg(this);
	m_pDeleteTipDlg->Create(CDeleteTipDlg::IDD);

	m_lsType.SetItemState(0, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
	ShowItemsInList();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSoundDlg::SetButtonDefaultColor(CCEButtonST* button)
{
	button->SetColor(CCEButtonST::BTNST_COLOR_BK_IN, RGB(248,214,147));
	button->SetColor(CCEButtonST::BTNST_COLOR_BK_OUT, RGB(248,214,147));
	button->SetColor(CCEButtonST::BTNST_COLOR_BK_FOCUS, RGB(248,214,147));
}

void CSoundDlg::SetButtonSelectedColor(CCEButtonST* button)
{
	button->SetColor(CCEButtonST::BTNST_COLOR_BK_IN, RGB(248,214,147));
	button->SetColor(CCEButtonST::BTNST_COLOR_BK_OUT, RGB(248,214,147));
	button->SetColor(CCEButtonST::BTNST_COLOR_BK_FOCUS, RGB(248,214,147));
}

void CSoundDlg::OnClickListType(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	ShowItemsInList();
	*pResult = 0;
}

void CSoundDlg::OnClickListList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}

void CSoundDlg::OnButtonSound() 
{
	// TODO: Add your control notification handler code here
	
}

void CSoundDlg::OnButtonSearch() 
{
	// TODO: Add your control notification handler code here
	m_pSoundSearchDlg->ShowWindow(TRUE);
}

void CSoundDlg::OnButtonProtect() 
{
	// TODO: Add your control notification handler code here
	m_pPasswordDlg->ShowWindow(TRUE);
}

void CSoundDlg::OnButtonDelete() 
{
	m_pDeleteTipDlg->ShowWindow(TRUE);
	// TODO: Add your control notification handler code here
	POSITION pos = m_lsList.GetFirstSelectedItemPosition();
	if (pos != NULL)
	{
		int index = m_lsList.GetNextSelectedItem (pos);   		
		int soundsegmentID = m_lsList.GetItemData(index);
		Data::SoundSegment::Remove("id = " + Util::StringOp::FromInt(soundsegmentID));
		m_lsList.DeleteItem(index);

		if (!m_vSearchResult.empty())
		{
			for (std::vector<boost::shared_ptr<Data::SoundSegment> >::iterator iter = m_vSearchResult.begin(); iter != m_vSearchResult.end(); ++iter)
			{
				if ((*iter)->id() == soundsegmentID)
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

void CSoundDlg::ShowItemsInList(void)
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
		filter = "type = " + Util::StringOp::FromInt(Data::sstLeaveWord);
		break;
	case 1:
		filter = "type = " + Util::StringOp::FromInt(Data::sstTeleRecord);
		break;
	case 2:
		ShowArrayInList(m_vSearchResult);
		return;
		break;
	}

	std::vector<boost::shared_ptr<Data::SoundSegment> > result = Data::SoundSegment::GetFromDatabase(filter);
	ShowArrayInList(result);	
}

void CSoundDlg::ShowArrayInList(std::vector<boost::shared_ptr<Data::SoundSegment> > array)
{
	m_lsList.DeleteAllItems();
	CString temp;
	for (int i = 0; i < (array).size(); ++i)
	{	
		temp = array[i]->filename().c_str();
		m_lsList.InsertItem(i, temp, i % 2);
		
		std::string filter = "soundSegmentId = " + Util::StringOp::FromInt(array[i]->id());
		std::vector<boost::shared_ptr<Data::ContactInfoSoundSegmentMap> > mapresult = Data::ContactInfoSoundSegmentMap::GetFromDatabase(filter);
		if (!mapresult.empty())
		{
// 			temp.Format(_T("%d-%02d-%02d %d:%02d"),mapresult[0]->startTime().GetYear(),
// 										mapresult[0]->startTime().GetMonth(),
// 										mapresult[0]->startTime().GetDay(),
// 										mapresult[0]->startTime().GetHour(),
// 										mapresult[0]->startTime().GetMinute());
			temp.Format(_T("%d-%02d %d:%02d"),
										mapresult[0]->startTime().GetMonth(),
										mapresult[0]->startTime().GetDay(),
										mapresult[0]->startTime().GetHour(),
										mapresult[0]->startTime().GetMinute());
			m_lsList.SetItemText(i, 2, temp);

			temp.Empty();
			int ciid = mapresult[0]->contactInfoId();
			filter = "id = " + Util::StringOp::FromInt(mapresult[0]->contactInfoId());
			std::vector<boost::shared_ptr<Data::ContactInfo> > contactinforesult = Data::ContactInfo::GetFromDatabase(filter);
			if (!contactinforesult.empty())
			{
				if (contactinforesult[0]->contact())
				{
					temp = (contactinforesult[0]->contact()->name() + "(" + contactinforesult[0]->telephoneNumber().number() + ")").c_str();
				}
				else
				{
					temp = contactinforesult[0]->telephoneNumber().number().c_str();
				}

// 				std::string number = contactinforesult[0]->telephoneNumber().ToString();
// 				std::string number = contactinforesult[0]->telephoneNumber().number();
// 				filter = "mobilesTelephone = '";
// 				filter += number;
// 				filter += "' OR mobilesTelephone2 = '";
// 				filter += number;
// 				filter += "' OR worksTelephone = '";
// 				filter += number;
// 				filter += "' OR worksTelephone2 = '";
// 				filter += number;
// 				filter += "'";
// 				std::vector<boost::shared_ptr<Data::Contact> > contactresult = Data::Contact::GetFromDatabase(filter);
// 				if (!contactresult.empty())
// 				{
// 					number = contactresult[0]->name() + "(" + number + ")";
// 				}
// 				temp = number.c_str();
			}
			m_lsList.SetItemText(i, 1, temp);
		}

		temp.Format(_T("%02d:%02d"), array[i]->duration().GetMinutes(),
							array[i]->duration().GetSeconds());
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

void CSoundDlg::ShowSearchResultInList(std::vector<boost::shared_ptr<Data::SoundSegment> > array)
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
