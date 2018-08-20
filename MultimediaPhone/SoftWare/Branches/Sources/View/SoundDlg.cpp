// SoundDlg.cpp : implementation file
//

#include "stdafx.h"
#include "../MultimediaPhone.h"
#include "SoundDlg.h"
#include "../Data/LanguageResource.h"
#include "../Data/Data.h"
#include "../Data/ContactInfo.h"
#include "../Data/Contact.h"
#include "../Data/SkinStyle.h"
#include "../MultimediaPhoneDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSoundDlg dialog

const int SoundTotal = 30000;
const int PageSize = 9;

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
	ON_BN_CLICKED(IDC_BUTTON_SOUND_CLOSE, OnButtonClose)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSoundDlg message handlers

BOOL CSoundDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	std::string strTemp;
	CString str;

	m_lsType.Create(WS_CHILD|WS_VISIBLE|LVS_REPORT|LVS_NOCOLUMNHEADER|LVS_NOSORTHEADER, CRect(8, 27, 80, 198), this, IDC_LIST_SOUND_TYPE, FALSE);
	m_lsType.SetExtendedStyle(m_lsType.GetExtendedStyle()|LVS_EX_FULLROWSELECT);
	m_lsType.InsertColumn(0, _T("Type"), LVCFMT_LEFT, 72);

	m_pImageList = new CImageList();
	m_pImageList->Create(16, 16, ILC_COLOR32|ILC_MASK, 3, 2);   
	CBitmap bm;
	bm.LoadBitmap(IDB_BITMAP1);
	m_pImageList->Add(&bm, RGB(255, 0, 255));
	bm.DeleteObject();
	bm.LoadBitmap(IDB_BITMAP24);
	m_pImageList->Add(&bm, RGB(255, 0, 255));
	bm.DeleteObject();
	bm.LoadBitmap(IDB_BITMAP25);
	m_pImageList->Add(&bm, RGB(255, 0, 255));
	bm.DeleteObject();

	m_lsType.SetImageList(m_pImageList, LVSIL_SMALL);

	strTemp = Data::LanguageResource::Get(Data::RI_SOUND_SOUND);
	str = strTemp.c_str();
	m_lsType.InsertItem(0, str, 1);

	strTemp = Data::LanguageResource::Get(Data::RI_SOUND_RECORD);
	str = strTemp.c_str();
	m_lsType.InsertItem(1, str, 2);

	strTemp = Data::LanguageResource::Get(Data::RI_RECORD_SEARCHRESULT);
	str = strTemp.c_str();
	m_lsType.InsertItem(2, str, 0);
	m_lsType.SetScrollRagle(FALSE);
	
	m_lsList.Create(WS_CHILD|WS_VISIBLE|LVS_REPORT|LVS_NOCOLUMNHEADER|LVS_NOSORTHEADER, CRect(81, 27, 416, 198), this, IDC_LIST_SOUND_LIST, TRUE, 0, FALSE);
	m_lsList.SetExtendedStyle(m_lsList.GetExtendedStyle()|LVS_EX_FULLROWSELECT);
	m_lsList.InsertColumn(0, _T("Sound"), LVCFMT_LEFT, 80);
	m_lsList.InsertColumn(1, _T("NameNO"), LVCFMT_LEFT, 100);
	m_lsList.InsertColumn(2, _T("Time"), LVCFMT_LEFT, 80);
	m_lsList.InsertColumn(3, _T("Duration"), LVCFMT_LEFT, 55);


	m_pImageList = new CImageList();
	m_pImageList->Create(16, 16, ILC_COLOR32|ILC_MASK, 2, 2);   
	bm.LoadBitmap(IDB_BITMAP23);
	m_pImageList->Add(&bm, RGB(255, 0, 255));
	bm.DeleteObject();
	bm.LoadBitmap(IDB_BITMAP26);
	m_pImageList->Add(&bm, RGB(255, 0, 255));
	bm.DeleteObject();

	m_lsList.SetImageList(m_pImageList, LVSIL_SMALL);


	strTemp = Data::LanguageResource::Get(Data::RI_SOUND_PLAY);
	str = strTemp.c_str();
	m_btnSound.Create(str, Data::g_buttonRectBMPID[0][Data::g_skinstyle], Data::g_buttonRectBMPID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(417, 26, 471, 45), this, IDC_BUTTON_SOUND_SOUND);
//	m_btnSound.SetBackRGB(Data::g_allFramBackRGB[Data::g_skinstyle]);

	strTemp = Data::LanguageResource::Get(Data::RI_COMN_FINDBTN);
	str = strTemp.c_str();
	m_btnSearch.Create(str, Data::g_buttonRectBMPID[0][Data::g_skinstyle], Data::g_buttonRectBMPID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(417, 46, 471, 65), this, IDC_BUTTON_SOUND_SEARCH);
//	m_btnSearch.SetBackRGB(Data::g_allFramBackRGB[Data::g_skinstyle]);
	
	strTemp = Data::LanguageResource::Get(Data::RI_SOUND_PROTECT);
	str = strTemp.c_str();
	m_btnProtect.Create(str, Data::g_buttonRectBMPID[0][Data::g_skinstyle], Data::g_buttonRectBMPID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(417, 66, 471, 85), this, IDC_BUTTON_SOUND_PROTECT);
//	m_btnProtect.SetBackRGB(Data::g_allFramBackRGB[Data::g_skinstyle]);

	strTemp = Data::LanguageResource::Get(Data::RI_RECORD_DELETE);
	str = strTemp.c_str();
	m_btnDelete.Create(str, Data::g_buttonRectBMPID[0][Data::g_skinstyle], Data::g_buttonRectBMPID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(417, 86, 471, 105), this, IDC_BUTTON_SOUND_DELETE);
//	m_btnDelete.SetBackRGB(Data::g_allFramBackRGB[Data::g_skinstyle]);

	strTemp = Data::LanguageResource::Get(Data::RI_SOUND_PLAY_CLOSE);
	str = strTemp.c_str();
	m_btnClose.Create(str, Data::g_buttonRectBMPID[0][Data::g_skinstyle], Data::g_buttonRectBMPID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(417, 178, 471, 198), this, IDC_BUTTON_SOUND_CLOSE);
	m_btnClose.SetBackRGB(Data::g_allFramBackRGB[Data::g_skinstyle]);

	m_sticPanel.Create(_T(""), WS_CHILD|WS_VISIBLE, CRect(416, 26, 472, 198), this);
	m_sticPanel.SetColor(RGB(0, 0, 0), Data::g_allFrameInFrameLine1RGB[0][Data::g_skinstyle]);

	m_sticBackground.Create(CRect(0, 0, 480, 204), this, 2);
	
	TextStruct ts[6];
	memset(ts, 0, sizeof(TextStruct) * 6);
	
	ts[0].txtRect = CRect(0, 0, 80, 20);
	ts[0].txtFontSize = 16;
	ts[0].sAlign = DT_CENTER | DT_BOTTOM;
	memcpy(ts[0].sTxt, Data::LanguageResource::Get(Data::RI_RECORD_TYPE).c_str(), Data::LanguageResource::Get(Data::RI_RECORD_TYPE).length());
	
	ts[1].txtRect = CRect(81, 0, 160, 20);
	ts[1].txtFontSize = 16;
	ts[1].sAlign = DT_CENTER | DT_BOTTOM;
	memcpy(ts[1].sTxt, Data::LanguageResource::Get(Data::RI_RECORD_SOUND).c_str(), Data::LanguageResource::Get(Data::RI_RECORD_SOUND).length());
	
	ts[2].txtRect = CRect(160, 0, 260, 20);
	ts[2].txtFontSize = 16;
	ts[2].sAlign = DT_CENTER | DT_BOTTOM;
	memcpy(ts[2].sTxt, Data::LanguageResource::Get(Data::RI_RECORD_NAMENO).c_str(), Data::LanguageResource::Get(Data::RI_RECORD_NAMENO).length());
	
	ts[3].txtRect = CRect(260, 0, 340, 20);
	ts[3].txtFontSize = 16;
	ts[3].sAlign = DT_CENTER | DT_BOTTOM;
	memcpy(ts[3].sTxt, Data::LanguageResource::Get(Data::RI_RECORD_TIME).c_str(), Data::LanguageResource::Get(Data::RI_RECORD_TIME).length());
	
	ts[4].txtRect = CRect(340, 0, 396, 20);
	ts[4].txtFontSize = 16;
	ts[4].sAlign = DT_CENTER | DT_BOTTOM;
	memcpy(ts[4].sTxt, Data::LanguageResource::Get(Data::RI_RECORD_DURATION).c_str(), Data::LanguageResource::Get(Data::RI_RECORD_DURATION).length());
	
	ts[5].txtRect = CRect(417, 0, 470, 20);
	ts[5].txtFontSize = 16;
	ts[5].sAlign = DT_CENTER | DT_BOTTOM;
	memcpy(ts[5].sTxt, Data::LanguageResource::Get(Data::RI_RECORD_OPERATION).c_str(), Data::LanguageResource::Get(Data::RI_RECORD_OPERATION).length());
	
	m_sticBackground.SetTextStruct(ts, 6);


	m_pPlaySoundDlg = new CPlaySoundDlg(this);
	m_pPlaySoundDlg->Create(CPlaySoundDlg::IDD);
	m_pPlaySoundDlg->SetModel(0);

	m_pSoundSearchDlg = new CSoundSearchDlg(this);
	m_pSoundSearchDlg->Create(CSoundSearchDlg::IDD);

	m_pPasswordDlg = new CPasswordDlg(this);
	m_pPasswordDlg->Create(CPasswordDlg::IDD);

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
	POSITION pos = m_lsList.GetFirstSelectedItemPosition();
	if (pos != NULL)
	{
		int index = m_lsList.GetNextSelectedItem (pos);   		
		pos = m_lsType.GetFirstSelectedItemPosition();
		m_pPlaySoundDlg->SetSound(m_lsType.GetNextSelectedItem(pos), m_lsList.GetItemData(index), 0, m_sListSearchFilter);
		m_pPlaySoundDlg->ShowWindow(TRUE);
	}
}

void CSoundDlg::OnButtonSearch() 
{
	// TODO: Add your control notification handler code here
	m_pSoundSearchDlg->ShowWindow(TRUE);
}

void CSoundDlg::OnButtonProtect() 
{
	// TODO: Add your control notification handler code here
 	m_pPasswordDlg->SetType(SETTING_PASSWORD);
	m_pPasswordDlg->SetOldPassWord((char*)((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pSettingDlg->m_pSetting->soundProtectPassword().c_str());
	m_pPasswordDlg->SetHWnd(this->m_hWnd);
	m_pPasswordDlg->SetOwner(this->m_hWnd);
	m_pPasswordDlg->ShowWindow(TRUE);
}

void CSoundDlg::OnButtonDelete() 
{
	// TODO: Add your control notification handler code here
	POSITION pos = m_lsList.GetFirstSelectedItemPosition();
	if (pos != NULL)
	{
		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetHWnd(this->GetSafeHwnd());
		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetPasswordModel(true);
		std::string pw = ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pSettingDlg->m_pSetting->soundProtectPassword();
		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetPassword(Util::StringOp::ToCString(pw));
		std::string strTemp = Data::LanguageResource::Get(Data::RI_DELETETIP_TIP);
		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetDelTip(strTemp.c_str());
		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->ShowWindow(TRUE);
	}
}

void CSoundDlg::OnButtonClose()
{
	((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->SwitchPanel_(IDC_BUTTON_MAIN);
}

void CSoundDlg::ShowItemsInList(void)
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
		m_sListFilter = "type = " + Util::StringOp::FromInt(Data::sstLeaveWord);
		filter = m_sListFilter;
	}
	else if (type == 1)
	{
		m_sListFilter = "type = " + Util::StringOp::FromInt(Data::sstTeleRecord);
		filter = m_sListFilter;
	}
	else if (type == 2)
	{
		if (m_sListSearchFilter == "")
		{
			return;
		}
		filter = m_sListSearchFilter;
	}

	if (Data::SoundSegment::GetDataCount("") > 0)
	{
		m_vCurrentResult = Data::SoundSegment::GetFromDatabase(filter, Data::dDown, Data::SoundSegment::GetCurrentId() + 1, PageSize);
		int count = Data::SoundSegment::GetDataCount(filter);
		m_lsList.m_scollbar_.SetRangle(0, count, PageSize);
		ShowArrayInList(m_vCurrentResult, type);	
		m_lsList.SetScrollRagle(TRUE);
	}
}

void CSoundDlg::ScrollItemsInList(int step)
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
	
	std::vector<boost::shared_ptr<Data::SoundSegment> > temp;
	int currentID = 0;
	
	if (step == SB_LINEUP)
	{
		currentID = m_vCurrentResult[0]->id();
		temp = Data::SoundSegment::GetFromDatabase(filter, Data::dUp, currentID, 1);
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
		temp = Data::SoundSegment::GetFromDatabase(filter, Data::dDown, currentID, 1);
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
		temp = Data::SoundSegment::GetFromDatabase(filter, Data::dUp, currentID, PageSize);
		if (temp.size() < PageSize)
		{
			temp.insert(temp.end(), m_vCurrentResult.begin(), m_vCurrentResult.begin() + PageSize - temp.size());
		}
		m_vCurrentResult = temp;
	}
	else if(step == SB_PAGEDOWN)
	{
		currentID = m_vCurrentResult[m_vCurrentResult.size() - 1]->id();
		temp = Data::SoundSegment::GetFromDatabase(filter, Data::dDown, currentID, PageSize);
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

void CSoundDlg::ShowArrayInList(std::vector<boost::shared_ptr<Data::SoundSegment> > array, int type)
{
	m_lsList.DeleteAllItems();
	UINT recordCount = 0;
	if (type == 0)
	{
		recordCount = ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pTelephoneDlg->GetRecordCount();
	}
	CString temp;
	for (int i = 0; i < (array).size(); ++i)
	{	
		std::string s = array[i]->filename();
		s = s.substr(s.length() - 10, 6);
		//temp = array[i]->filename().c_str();
		temp = s.c_str();

		if (type == 0 && i < recordCount)
		{
			m_lsList.InsertItem(i, temp, 0);
		}
		else
		{
			m_lsList.InsertItem(i, temp, 1);
		}

// 		if (array[i]->GetContact())
// 		{
// 			temp = (array[i]->GetContact()->name() + "(" + array[0]->telephoneNumber().number() + ")").c_str();
// 		}
// 		else
// 		{
// 			temp = array[i]->telephoneNumber().number().c_str();
// 		}
		if (array[i]->name() != "")
		{
			temp = (array[i]->name() + "(" + array[0]->telephoneNumber().number() + ")").c_str();
		}
		else
		{
			temp = array[i]->telephoneNumber().number().c_str();
		}
		m_lsList.SetItemText(i, 1, temp);
		
			temp.Format(_T("%02d-%02d %d:%02d"),
										array[i]->startTime().GetMonth(),
										array[i]->startTime().GetDay(),
										array[i]->startTime().GetHour(),
										array[i]->startTime().GetMinute());
			m_lsList.SetItemText(i, 2, temp);

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
		m_lsList.SetItemText(i, 3, temp);

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

void CSoundDlg::DeleteSelectedItem(void)
{
	POSITION pos = m_lsList.GetFirstSelectedItemPosition();
	if (pos != NULL)
	{
		int index = m_lsList.GetNextSelectedItem (pos);   		
//		int soundsegmentID = m_lsList.GetItemData(index);

		std::string filename = m_vCurrentResult[index]->filename();			
		m_vCurrentResult[index]->Remove();
		
		CFileStatus   status;
		if (CFile::GetStatus(LPCTSTR(Util::StringOp::ToCString(filename)),status))
		{
			try
			{
				CFile::Remove(LPCTSTR(Util::StringOp::ToCString(filename)));
			}
			catch (CFileException* pEx)
			{
				pEx->Delete();
			}
		}
		ShowItemsInList();		
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

void CSoundDlg::Search(std::string filter)
{
	m_sListSearchFilter = filter;
	ShowItemsInList();
}

void CSoundDlg::SaveSoundSegment(boost::shared_ptr<Data::SoundSegment> ss)
{
	ss->Insert();
	//	m_vContactInfo.insert(m_vContactInfo.begin(), ci);
	ShowItemsInList();
	if (Data::SoundSegment::GetDataCount("") >= SoundTotal)
	{
		MessageBox(Util::StringOp::ToCString(Data::LanguageResource::Get(Data::RI_SYSTEM_DISK)));
		if (Data::SoundSegment::GetDataCount("") > SoundTotal)
		{
			std::vector<boost::shared_ptr<Data::SoundSegment> > temp = Data::SoundSegment::GetFromDatabase("", Data::dUp, 0, 1);
			if (!temp.empty())
			{
				std::string filter = "id = ";
				filter += Util::StringOp::FromInt(temp[0]->id());
				Data::SoundSegment::Remove(filter);
			}
		}
	}
}

int CSoundDlg::GetSoundID(int listID)
{
	if ((listID >= 0) && (listID < m_lsList.GetItemCount()))
	{
		int soundsegmentID = m_lsList.GetItemData(listID);
		return soundsegmentID;
	}
	else
	{
		return -1;
	}
}

std::string CSoundDlg::GetSoundNameNo(int listID)
{
	if ((listID >= 0) && (listID < m_lsList.GetItemCount()))
	{
		CString nameno = m_lsList.GetItemText(listID, 1);
		return Util::StringOp::FromCString(nameno);
	}
	else
	{
		return "";
	}
}

LRESULT CSoundDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	switch (message)
	{
	case WM_DELETESELITEM:
		DeleteSelectedItem();
		break;
	case WM_SETTINGPASSWORD:
		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pSettingDlg->m_pSetting->soundProtectPassword(std::string((char*)wParam, lParam));
		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pSettingDlg->m_pSetting->Update();
		break;
	case WM_VSCROLL:
		ScrollItemsInList(wParam);
		break;
	};
	return CDialog::WindowProc(message, wParam, lParam);
}
