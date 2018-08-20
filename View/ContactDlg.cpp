// ContactDlg.cpp : implementation file
//

#include "stdafx.h"
#include "../MultimediaPhone.h"
#include "../Data/LanguageResource.h"
#include "../MultimediaPhoneDlg.h"
#include "ContactDlg.h"
#include "../Data/ContactGroup.h"
#include "../Data/SkinStyle.h"
#include "../Data/Contact.h"
#include "ContactNewDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define TITLE_NUM     4

int g_iContactCount = 0;
/////////////////////////////////////////////////////////////////////////////
// CContactDlg dialog

CContactDlg::CContactDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CContactDlg::IDD, pParent)
	, ContactTotal(10000)
	, PageSize(6)
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
//	ON_MESSAGE(WM_CHANGELANGUAGE,OnChangeLanguage)
     ON_WM_KEYDOWN()
	 ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_NOTIFY(NM_CLICK, IDC_LIST_CONTACT_TYPE, OnClickListType)
	ON_NOTIFY(NM_CLICK, IDC_LIST_CONTACT_LIST, OnClickListList)
	ON_MESSAGE(WM_LISTCTRL_CLICK, OnListCltrlClick)
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

	std::string strTemp;
	CString str;

	m_lsType.m_nFontHeight = 16;
	m_lsType.m_nLineHeight = 25;
	m_lsType.Create(WS_CHILD|WS_VISIBLE|\
		LVS_REPORT|LVS_NOCOLUMNHEADER|LVS_NOSORTHEADER/*|LVS_OWNERDRAWFIXED*/,\
		CRect(8, 27*VHEIGHT, 100, 157*VHEIGHT), this, IDC_LIST_CONTACT_TYPE, TRUE);
	m_lsType.SetExtendedStyle(m_lsType.GetExtendedStyle()|LVS_EX_FULLROWSELECT);
	m_lsType.InsertColumn(0, _T("Type"), LVCFMT_LEFT, 92);

	m_pImageList = new CImageList();
	m_pImageList->Create(16, m_lsType.m_nLineHeight, ILC_COLOR32|ILC_MASK, 12, 2);   
    
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
	
	m_lsList.m_nLineHeight = 25 ;
	m_lsList.Create(WS_CHILD|WS_VISIBLE|LVS_REPORT|LVS_NOCOLUMNHEADER|LVS_NOSORTHEADER,\
		CRect(101, 27*VHEIGHT, 416, 157*VHEIGHT), this, IDC_LIST_CONTACT_LIST,\
		TRUE, 0, FALSE);
	m_lsList.SetExtendedStyle(m_lsList.GetExtendedStyle()|LVS_EX_FULLROWSELECT);
	m_lsList.InsertColumn(0, _T("Name"), LVCFMT_LEFT, 160/*70*/);
	m_lsList.InsertColumn(1, _T("Company"), LVCFMT_LEFT, 136/*96*/);
//	m_lsList.InsertColumn(2, _T("Department"), LVCFMT_LEFT, 70);
//	m_lsList.InsertColumn(3, _T("Duty"), LVCFMT_LEFT, 60);

	m_pImageList = new CImageList();
	m_pImageList->Create(16,m_lsList.m_nLineHeight /*16*/, ILC_COLOR32|ILC_MASK, 3, 2);   

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
		rectKey[j] = CRect(87 + offsetX, offsetY*VHEIGHT, 116 + offsetX, (offsetY + 20)*VHEIGHT);
		uiKey[j] = 'a' + j;
	};
	rectKey[26] = CRect(0, 21*VHEIGHT, 29, 41*VHEIGHT);
	uiKey[26] = 0x7F;
	rectKey[27] = CRect(29, 21*VHEIGHT, 58, 41*VHEIGHT);
	uiKey[27] = 0x00;
	rectKey[28] = CRect(58, 21*VHEIGHT, 87, 41*VHEIGHT);
	uiKey[28] = '\?';
	
	m_sticPinYin.Create(_T(""), WS_CHILD|WS_VISIBLE, CRect(9, 160*VHEIGHT, 90, 176*VHEIGHT), this, IDC_STATIC_CONTACT_PINYIN);
	m_sticPinYin.SetColor(RGB(0, 0, 0), RGB(255, 255, 255));
	m_sticPinYin.SetLeftMargin(10);

	m_skSoftKey.Create(IDB_VOIPPYSEARCH, CPoint(0,0), 29, rectKey, uiKey, CRect(8, 157*VHEIGHT, 474, 198*VHEIGHT),this,WS_CHILD|WS_VISIBLE, IDC_SOFTKEY_CONTACT_PYSEARCH);

	strTemp = Data::LanguageResource::Get(Data::RI_CARD_NEWCARDBTN_NEW);
	str = strTemp.c_str();
	m_btnNewContact.Create(str, Data::g_buttonRectBMPID[0][Data::g_skinstyle], Data::g_buttonRectBMPID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(417, 26*VHEIGHT, 474, 45*VHEIGHT), this, IDC_BUTTON_CONTACT_NEW, 14);
	m_btnNewContact.SetBackRGB(Data::g_allFramBackRGB[Data::g_skinstyle]);                                                                                     /* 30              52*/
	
	//新类型
	strTemp = Data::LanguageResource::Get(Data::RI_CARD_NEWTYPEBTN);
	str = strTemp.c_str();
	m_btnNewType.Create(str, Data::g_buttonRectBMPID[0][Data::g_skinstyle], Data::g_buttonRectBMPID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE|BS_LEFT, CRect(417, 46*VHEIGHT, 474, 65*VHEIGHT), this, IDC_BUTTON_CONTACT_NEWTYPE, 14);
	m_btnNewType.SetBackRGB(Data::g_allFramBackRGB[Data::g_skinstyle]);                                                                                     /* 53               76*/                                      
	
	//编辑类型
	strTemp = Data::LanguageResource::Get(Data::RI_CARD_EDITTYPEBTN);
	str = strTemp.c_str();
	m_btnEditType.Create(str, Data::g_buttonRectBMPID[0][Data::g_skinstyle], Data::g_buttonRectBMPID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE|BS_LEFT, CRect(417, 66*VHEIGHT, 474, 85*VHEIGHT), this, IDC_BUTTON_CONTACT_EDITTYPE, 14);
	m_btnEditType.SetBackRGB(Data::g_allFramBackRGB[Data::g_skinstyle]);                                                                                    /* 77               99*/

	strTemp = Data::LanguageResource::Get(Data::RI_COMN_FINDBTN);
	str = strTemp.c_str(); 
	m_btnSearch.Create(str, Data::g_buttonRectBMPID[0][Data::g_skinstyle], Data::g_buttonRectBMPID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(417, 86*VHEIGHT, 474, 105*VHEIGHT), this, IDC_BUTTON_CONTACT_SEARCH);
	m_btnSearch.SetBackRGB(Data::g_allFramBackRGB[Data::g_skinstyle]);																						 /* 101               122*/
		
	strTemp = Data::LanguageResource::Get(Data::RI_SOUND_PLAY_CLOSE);
	str = strTemp.c_str();
	m_btnClose.Create(str, Data::g_buttonRectBMPID[0][Data::g_skinstyle], Data::g_buttonRectBMPID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(417, 138*VHEIGHT, 474, 157*VHEIGHT), this, IDC_BUTTON_CONTACT_CLOSE);
	m_btnClose.SetBackRGB(Data::g_allFramBackRGB[Data::g_skinstyle]);																						 /*  161              183*/

	m_sticPanel.Create(_T(""), WS_CHILD|WS_VISIBLE, CRect(416, 26*VHEIGHT, 472, 157*VHEIGHT), this);
	m_sticPanel.SetColor(RGB(0, 0, 0), Data::g_allFrameInFrameLine1RGB[0][Data::g_skinstyle]);
	
	m_sticBackground.Create(CRect(0, 0, 480, 204*VHEIGHT), this, 2);
    m_pContt = boost::shared_ptr<Data::Contact>();
	TextStruct ts[TITLE_NUM];
	memset(ts, 0, sizeof(TextStruct) *TITLE_NUM);

	ts[0].txtRect = CRect(0, 0, 100, 20*VHEIGHT);
	ts[0].txtFontSize = 16;
	ts[0].sAlign = DT_CENTER | DT_BOTTOM;
	memcpy(ts[0].sTxt, Data::LanguageResource::Get(Data::RI_CARD_TYPESTC).c_str(), Data::LanguageResource::Get(Data::RI_CARD_TYPESTC).length());
	
	ts[1].txtRect = CRect(122, 0, 272, 20*VHEIGHT);
	ts[1].txtFontSize = 16;
	ts[1].sAlign = DT_LEFT | DT_BOTTOM;
	memcpy(ts[1].sTxt, Data::LanguageResource::Get(Data::RI_CARD_NAMESTC).c_str(), Data::LanguageResource::Get(Data::RI_CARD_NAMESTC).length());
	
	ts[2].txtRect = CRect(272, 0, 396, 20*VHEIGHT);
	ts[2].txtFontSize = 16;
	ts[2].sAlign = DT_LEFT | DT_BOTTOM;
	memcpy(ts[2].sTxt, \
		Data::LanguageResource::Get(Data::RI_CARD_COMPANYSTC).c_str(), \
		Data::LanguageResource::Get(Data::RI_CARD_COMPANYSTC).length());
/*
	ts[3].txtRect = CRect(266, 0, 336, 20*VHEIGHT);
	ts[3].txtFontSize = 16;
	ts[3].sAlign = DT_CENTER | DT_BOTTOM;
	memcpy(ts[3].sTxt, Data::LanguageResource::Get(Data::RI_CARD_DEPARTSTC).c_str(), Data::LanguageResource::Get(Data::RI_CARD_DEPARTSTC).length());
	
	ts[4].txtRect = CRect(336, 0, 396, 20*VHEIGHT);
	ts[4].txtFontSize = 16;
	ts[4].sAlign = DT_CENTER | DT_BOTTOM;
	memcpy(ts[4].sTxt, Data::LanguageResource::Get(Data::RI_CARD_TITLESTC).c_str(), Data::LanguageResource::Get(Data::RI_CARD_TITLESTC).length());
*/	
	ts[3].txtRect = CRect(417, 0, 475, 20*VHEIGHT);
	ts[3].txtFontSize = 16;  //
	ts[3].sAlign = DT_CENTER | DT_BOTTOM;
	memcpy(ts[3].sTxt, \
		Data::LanguageResource::Get(Data::RI_CARD_OPERATORSTC).c_str(), \
		Data::LanguageResource::Get(Data::RI_CARD_OPERATORSTC).length());
	
	m_sticBackground.SetTextStruct(ts, TITLE_NUM);

	m_pContactSearchDlg = new CContactSearchDlg(this);
	m_pContactSearchDlg->Create(CContactSearchDlg::IDD);
    
	m_pContt=boost::shared_ptr<Data::Contact>();
	ShowTypeItems();
	m_sListFilter = "";
	m_sListSearchFilter = "";
	g_iContactCount = Data::Contact::GetDataCount("");
	ShowItemsInList();
	m_lsType.SetItemState(0,LVIS_SELECTED|LVIS_FOCUSED,LVIS_SELECTED|LVIS_FOCUSED);
	m_lsList.SetItemState(0, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
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

	GetDlgItem(IDC_STATIC_CONTACT_PINYIN)->SetWindowText(_T(""));

 	ShowItemsInList();

	*pResult = 0;
}

void CContactDlg::OnListCltrlClick(WPARAM w, LPARAM l)
{
	LRESULT ret;
	if(w == IDC_LIST_CONTACT_TYPE)
		OnClickListType(NULL, &ret);
	else if(w == IDC_LIST_CONTACT_LIST)
		OnClickListList(NULL, &ret);
}

void CContactDlg::ShowTypeItems(void)
{
	m_lsType.DeleteAllItems();

	int imageID = 0;

	std::string strTemp;

	strTemp = Data::LanguageResource::Get(Data::RI_CARD_ALLCARDSTC);

	CString str = Util::StringOp::ToCString(strTemp);

	m_lsType.InsertItem(0, str, imageID);

	m_lsType.SetItemData(0, 0);

	imageID = 2;

	std::vector<boost::shared_ptr<Data::ContactGroup> > result;

	std::string SGroup=Data::LanguageResource::Get(Data::RI_CARD_ALLCARDSTC);

	std::string SCHGroup="所有名片";

	if (Data::ContactGroup::GetDataCount("") > 0)
	{
		result = Data::ContactGroup::GetFromDatabase("");

		for (int i = 0, index=0; i < result.size(); ++i)
		{   
		    if (SGroup==result[i]->name() || result[i]->name() == SCHGroup)
			{
				continue;
			}

			m_lsType.InsertItem(index+1, Util::StringOp::ToCString(result[i]->name()), imageID);
			int gId = result[i]->id();
			m_lsType.SetItemData(index+1, gId);
			index++;
			++imageID;
			if (imageID > 10)
			{
				imageID = 2;
			}	
		}
	}

	strTemp = Data::LanguageResource::Get(Data::RI_CARD_FINDREAULTSTC);

	str = strTemp.c_str();

    //m_lsType.InsertItem(result.size() + 1, str, imageID % 2);

	m_lsType.InsertItem(result.size() + 1, str, 1);

	m_lsType.SetItemData(result.size() + 1, result.size() + 1);

	m_lsType.SetScrollRagle(TRUE);

	m_lsType.SetItemState(0, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);

}

void CContactDlg::ShowItemsInList(void)
{
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

				return;

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

		if (g_iContactCount > 0)
		{
			m_vCurrentResult = Data::Contact::GetFromDatabase(filter, Data::dDown, Data::Contact::GetCurrentId() + 1, PageSize);

			int count = Data::Contact::GetDataCount(filter);

			m_lsList.m_scollbar_.SetRangle(0, count, PageSize);

			ShowArrayInList(m_vCurrentResult);
			
			m_lsList.SetScrollRagle(TRUE);	
			
		}

		m_lsList.SetItemState(0, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
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

void CContactDlg::ShowArrayInList(std::vector<boost::shared_ptr<Data::Contact> > array)
{
	int iSel = 0;

	POSITION pos = m_lsList.GetFirstSelectedItemPosition(); 
	
	if (pos != NULL)
	{   
		iSel = m_lsList.GetNextSelectedItem(pos);   
	}
	SetWindowRedraw(m_lsList.m_hWnd, FALSE);

	m_lsList.DeleteAllItems();

	CString temp;

	for (int i = 0; i < (array).size(); ++i)
	{	
		temp = Util::StringOp::ToCString(array[i]->name());
		
		m_lsList.InsertItem(i, temp, array[i]->type());

		temp = Util::StringOp::ToCString(array[i]->company());

		m_lsList.SetItemText(i, 1, temp);

		temp = Util::StringOp::ToCString(array[i]->department());

		m_lsList.SetItemText(i, 2, temp);

		temp = Util::StringOp::ToCString(array[i]->duty());

		m_lsList.SetItemText(i, 3, temp);

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

	SetWindowRedraw(m_lsList.m_hWnd, TRUE);

	m_lsList.Invalidate();
}

void CContactDlg::OnClickListList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	SetTimer(3, 100, NULL);  //wzx 20110104
	*pResult = 0;
}

///zzw100823////////////////////////////////////////
void CContactDlg::OnButtonContactNew() 
{
	// TODO: Add your control notification handler code here
	if (g_iContactCount < ContactTotal)
	{
		boost::shared_ptr<Data::Contact> contact(new Data::Contact);
		contact->id(0);
		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactNewDlg->SetContact(contact);
		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactNewDlg->ShowWindow(TRUE);

	}else{

		std::string str;
		str=Data::LanguageResource::Get(Data::RI_ERROR);
		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetPasswordModel(false);
		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetTitle(str);
		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetDelTip(Util::StringOp::ToCString(Data::LanguageResource::Get(Data::RI_SYSTEM_CONTACT_TIP)));
		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->ShowWindow_(SW_SHOW);
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
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
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
	else
	{
		main->m_pDeleteTipDlg->SetPasswordModel(false);
		main->m_pDeleteTipDlg->SetTitle(Data::LanguageResource::Get(Data::RI_ERROR));
		std::string str1 = Data::LanguageResource::Get(Data::RI_DELETETIP_SELECT);  //是否格式化话机进入升级模式
		main->m_pDeleteTipDlg->SetDelTip(str1.c_str());
		main->m_pDeleteTipDlg->ShowWindow_(SW_SHOW);
	}
}

void CContactDlg::OnButtonContactSearch() 
{
	// TODO: Add your control notification handler code here
	m_pContactSearchDlg->ShowWindow(TRUE);
}

void CContactDlg::OnButtonContactClose()
{    
	 ShowWindow(FALSE);
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

		if (py.GetLength() >= 8)
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

void CContactDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{ 	
}

LRESULT CContactDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	switch (message)
	{
	case WM_VSCROLL:

		ScrollItemsInList(wParam);

		break;

	case WM_CHANGELANGUAGE:

		ChangeLanguage();

     break;

	case WM_NEWCONTACT:

		if (((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactNewDlg->IsWindowVisible() || 

			((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactGroupDlg->IsWindowVisible() )
		{
			return 0;
		}
	   else

		  {
			OnButtonContactNew();
		  }

	break;

	case WM_KEYDOWN:

	if (((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactDlg->IsWindowVisible())
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
				 
				 if(i==-1)
				 {
					 ScrollItemsInList(SB_LINEUP);
					 int nPos = m_lsList.m_scollbar_.GetPos();
					 nPos--;
					 m_lsList.m_scollbar_.SetPos(nPos);
					 
					 m_lsList.SetItemState(0,LVIS_SELECTED|LVIS_FOCUSED,LVIS_SELECTED|LVIS_FOCUSED);
					 
				 }
				 
			 }else if (wParam=='D')
			 {   
				 i++;
				 
				 m_lsList.SetExtendedStyle(m_lsList.GetExtendedStyle()|LVS_EX_FULLROWSELECT);
				 m_lsList.SetItemState(i,LVIS_SELECTED|LVIS_FOCUSED,LVIS_SELECTED|LVIS_FOCUSED);
				 
				 if (i==m_lsList.GetItemCount())
				 {
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
		 }
		 
		 m_lsList.Invalidate();
	}

	break;

	case WM_OK:
		{
			if (((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pTelephoneDlg->IsWindowVisible())
			{
				return 1;

			}else{

				POSITION pos = m_lsList.GetFirstSelectedItemPosition();	
				
				if (pos != NULL)	
				{		
					int index = m_lsList.GetNextSelectedItem (pos);	
					
					int id = m_lsList.GetItemData(index);

					std::vector<boost::shared_ptr<Data::Contact> > result = Data::Contact::GetFromDatabase("id = " + Util::StringOp::FromInt(id));

					if (!result.empty())
					{
						((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactNewDlg->SetContact(result[0]);

						((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactNewDlg->ShowWindow(SW_SHOW);
					}
				}
				else
				{
					OnButtonContactNew();	
				}

			}

		}

	break;

	case WM_CANCEL:
		OnButtonContactClose();
		break;

	case WM_DELETE:
		if (((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactDlg->IsWindowVisible())
		{   
			if(m_lsList.GetItemCount()>0)
			{
				POSITION pos=m_lsList.GetFirstSelectedItemPosition();
				if (pos!=NULL)
				{	
					//change by qi 20111104
					CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
					if (main->m_pSettingDlg->m_pSetting->isAdmin() && \
						!main->m_pSettingDlg->m_bLogin)
					{							
						std::string pw = main->m_pSettingDlg->m_pSetting->adminPassword();
						if (!pw.empty())
						{	
							//change by qi 20111104
							//密码不为空,才输入密码
							main->m_pDeleteTipDlg->SetPasswordModel(true);
							main->m_pDeleteTipDlg->SetPassword(Util::StringOp::ToCString(pw));
						}
					}
					iSel =m_lsList.GetNextSelectedItem(pos);                
					((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetHWnd(m_hWnd);
					std::string strTemp = Data::LanguageResource::Get(Data::RI_DELETETIP_CONTACT);
					((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetDelTip(strTemp.c_str());
					((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->ShowWindow(TRUE);
				}
			}
		}
	 break;

 	case WM_DELETESELITEM:
		{
			POSITION pos=m_lsList.GetFirstSelectedItemPosition();
			int isel;
			if (pos != NULL)
			{
				isel=m_lsList.GetNextSelectedItem(pos);
			}
			m_vCurrentResult[isel]->Remove();
			--g_iContactCount;
			ShowItemsInList();
		}
 		break;
	case WM_ITYPE:
        if (((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactDlg->IsWindowVisible())
		{
            POSITION pos = m_lsType.GetFirstSelectedItemPosition();
			if (pos != NULL)
			{
				int type = m_lsType.GetNextSelectedItem(pos);
				type++;
				m_lsType.SetExtendedStyle(m_lsType.GetExtendedStyle()|LVS_EX_FULLROWSELECT);
				m_lsType.SetItemState(type,LVIS_SELECTED|LVIS_FOCUSED,LVIS_SELECTED|LVIS_FOCUSED);
				if (type==m_lsType.GetItemCount())
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
        if (((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactDlg->IsWindowVisible())
		{
          m_pContactSearchDlg->ShowWindow(TRUE);			
		}
	break;

	case WM_TEL_KEYCODE:
	 if (!(((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactNewDlg->IsWindowVisible()||
		 ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactGroupDlg->IsWindowVisible()||
		  ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->IsWindowVisible()||
		 m_pContactSearchDlg->IsWindowVisible()||
		 ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pMainDlg->m_mainScreenSaveDlg_->IsWindowVisible()
		 ))
	 { 
			 std::string c;
			 c=wParam;
			 findfilter+=c;
	         SetTimer(IDT_SEARCHER_TIME,500,NULL);
	 }
	  break;
	}
	return CDialog::WindowProc(message, wParam, lParam);
}
void CContactDlg::OnTimer(UINT nIDEvent)
{
	std::string filter;
	if (IDT_SEARCHER_TIME==nIDEvent)
	{
		KillTimer(IDT_SEARCHER_TIME);
		filter += "(mobilesTelephone Like '%";
		filter += findfilter;
		filter += "%'";
		filter += " OR mobilesTelephone2 Like '%";
		filter += findfilter;
		filter += "%'";
		filter += " OR worksTelephone Like '%";
		filter += findfilter;
		filter += "%'";
		filter += " OR worksTelephone2 Like '%";
		filter += findfilter;
		filter += "%')";
		Search(filter);
	}
	else if(3 == nIDEvent)
	{
		KillTimer(nIDEvent);
		POSITION pos = m_lsList.GetFirstSelectedItemPosition();
		if (pos != NULL)
		{
			int index = m_lsList.GetNextSelectedItem (pos);
			int id = m_lsList.GetItemData(index);
			std::vector<boost::shared_ptr<Data::Contact> > result = Data::Contact::GetFromDatabase("id = " + Util::StringOp::FromInt(id));
			if (!result.empty())
			{
				((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactNewDlg->SetContact(result[0]);
				((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactNewDlg->ShowWindow(SW_SHOW);
			}
		}
	}
	findfilter=""; 
	CDialog::OnTimer(nIDEvent);
}

void CContactDlg:: ChangeLanguage()
{   
	std::string strTemp;

    CString str;

	strTemp = Data::LanguageResource::Get(Data::RI_CARD_ALLCARDSTC);

    str= Util::StringOp::ToCString(strTemp);

	m_lsType.DeleteItem(0);

	m_lsType.InsertItem(0, str, 0);

	strTemp = Data::LanguageResource::Get(Data::RI_CARD_FINDREAULTSTC);

	str = strTemp.c_str();

	m_lsType.DeleteItem(m_lsType.GetItemCount()-1);

	m_lsType.InsertItem(m_lsType.GetItemCount(), str, 1);

	ShowItemsInList();

	strTemp = Data::LanguageResource::Get(Data::RI_CARD_NEWCARDBTN_NEW);

	str = strTemp.c_str();

    m_btnNewContact.SetWindowText(str);

	strTemp = Data::LanguageResource::Get(Data::RI_CARD_NEWTYPEBTN);

	str = strTemp.c_str();

    m_btnNewType.SetWindowText(str);

	strTemp = Data::LanguageResource::Get(Data::RI_CARD_EDITTYPEBTN);
	str = strTemp.c_str();
    m_btnEditType.SetWindowText(str);

    strTemp = Data::LanguageResource::Get(Data::RI_COMN_FINDBTN);
	str = strTemp.c_str();
    m_btnSearch.SetWindowText(str);

	strTemp = Data::LanguageResource::Get(Data::RI_SOUND_PLAY_CLOSE);
	str = strTemp.c_str();
    m_btnClose.SetWindowText(str);

	TextStruct ts[TITLE_NUM];
	memset(ts, 0, sizeof(TextStruct) * TITLE_NUM);
	
	ts[0].txtRect = CRect(0, 0, 100, 20*VHEIGHT);
	ts[0].txtFontSize = 16;
	ts[0].sAlign = DT_CENTER | DT_BOTTOM;
	memcpy(ts[0].sTxt, Data::LanguageResource::Get(Data::RI_CARD_TYPESTC).c_str(), Data::LanguageResource::Get(Data::RI_CARD_TYPESTC).length());
	
	ts[1].txtRect = CRect(122, 0, 272, 20*VHEIGHT);
	ts[1].txtFontSize = 16;
	ts[1].sAlign = DT_LEFT | DT_BOTTOM;
	memcpy(ts[1].sTxt, Data::LanguageResource::Get(Data::RI_CARD_NAMESTC).c_str(), Data::LanguageResource::Get(Data::RI_CARD_NAMESTC).length());
	
	ts[2].txtRect = CRect(272, 0, 396, 20*VHEIGHT);
	ts[2].txtFontSize = 16;
	ts[2].sAlign = DT_LEFT | DT_BOTTOM;
	memcpy(ts[2].sTxt,\
		Data::LanguageResource::Get(Data::RI_CARD_COMPANYSTC).c_str(),\
		Data::LanguageResource::Get(Data::RI_CARD_COMPANYSTC).length());


/*	
	ts[3].txtRect = CRect(266, 0, 336, 20);
	ts[3].txtFontSize = 16;
	ts[3].sAlign = DT_CENTER | DT_BOTTOM;
	memcpy(ts[3].sTxt, Data::LanguageResource::Get(Data::RI_CARD_DEPARTSTC).c_str(), Data::LanguageResource::Get(Data::RI_CARD_DEPARTSTC).length());
	
	ts[4].txtRect = CRect(336, 0, 396, 20);
	ts[4].txtFontSize = 16;
	ts[4].sAlign = DT_CENTER | DT_BOTTOM;
	memcpy(ts[4].sTxt, Data::LanguageResource::Get(Data::RI_CARD_TITLESTC).c_str(), Data::LanguageResource::Get(Data::RI_CARD_TITLESTC).length());
*/	
	ts[3].txtRect = CRect(417, 0, 475, 20*VHEIGHT);
	ts[3].txtFontSize = 16;			
	ts[3].sAlign = DT_CENTER | DT_BOTTOM;
	memcpy(ts[3].sTxt, Data::LanguageResource::Get(Data::RI_CARD_OPERATORSTC).c_str(), Data::LanguageResource::Get(Data::RI_CARD_OPERATORSTC).length());
	m_sticBackground.SetTextStruct(ts, TITLE_NUM);
}
