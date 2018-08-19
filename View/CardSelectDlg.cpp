// ContactDlg.cpp : implementation file
//

#include "stdafx.h"
#include "../MultimediaPhone.h"
#include "../Data/LanguageResource.h"
#include "../MultimediaPhoneDlg.h"
#include "CardSelectDlg.h"
#include "../Data/ContactGroup.h"
#include "../Data/SkinStyle.h"

#include "SIMImportDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CContactDlg dialog

CCardSelectDlg::CCardSelectDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCardSelectDlg::IDD, pParent)
	, ContactTotal(3000)
	, ContactGroupTotal(8)
	, PageSize(8)
{
	m_bSelectAll = FALSE ;	
}


void CCardSelectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CContactDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCardSelectDlg, CDialog)
	//{{AFX_MSG_MAP(CContactDlg)
	//}}AFX_MSG_MAP
	ON_NOTIFY(NM_CLICK, IDC_LIST_CONTACT_TYPE, OnClickListType)
	ON_NOTIFY(NM_CLICK, IDC_LIST_CONTACT_LIST, OnClickListList)
	ON_BN_CLICKED(IDC_BUTTON_CONTACT_NEW, OnButtonContactNew)
	ON_BN_CLICKED(IDC_BUTTON_CONTACT_NEWTYPE, OnButtonContactNewtype)
	ON_BN_CLICKED(IDC_BUTTON_CONTACT_EDITTYPE, OnButtonContactEdittype)
	ON_BN_CLICKED(IDC_BUTTON_CONTACT_SEARCH, OnButtonContactSearch)
	ON_BN_CLICKED(IDC_BUTTON_CONTACT_CLOSE, OnButtonContactClose)
	ON_MESSAGE(WM_LISTCTRL_CLICK, OnListCltrlClick)
	ON_WM_TIMER()
	
	ON_BN_CLICKED(IDC_BTN_SELECTALL,OnBtnSelectAll)
	ON_BN_CLICKED(IDC_BTN_CANCELALL,OnBtnCancelAll)
	ON_BN_CLICKED(MMS_BTN_OK,OnBtnOk)
	ON_BN_CLICKED(MMS_BTN_CANCEL,OnBtnCancel)

	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

void CCardSelectDlg::OnListCltrlClick(WPARAM w, LPARAM l)
{
	LRESULT ret;
	if(w == IDC_LIST_CONTACT_TYPE)
		OnClickListType(NULL, &ret);
	else if(w == IDC_LIST_CONTRAL_CARDSELECT)
		OnClickListList(NULL, &ret);
}

/////////////////////////////////////////////////////////////////////////////
// CContactDlg message handlers

void CCardSelectDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if(nChar == 'U')
	{
		m_lsList.m_scollbar_.DoPageUp();
	}
	else if(nChar == 'D')
	{
		m_lsList.m_scollbar_.DoPageDown();
	}
}

BOOL CCardSelectDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	std::string strTemp;
	CString str;

	int xlbegin = 10,ylbegin = 10 ;

	m_lsType.m_nLineHeight = 30;
	m_lsType.Create(WS_CHILD|WS_VISIBLE|LVS_REPORT|LVS_NOCOLUMNHEADER|LVS_NOSORTHEADER, CRect(1, ylbegin, 112, ylbegin+303), this, IDC_LIST_CONTACT_TYPE, FALSE);
	m_lsType.SetExtendedStyle(m_lsType.GetExtendedStyle()|LVS_EX_FULLROWSELECT);
	m_lsType.InsertColumn(0, _T("Type"), LVCFMT_LEFT, 110);

	m_lsList.Create(WS_CHILD|WS_VISIBLE|LVS_REPORT, CRect(113, ylbegin, 113+472+50, ylbegin+303), this, IDC_LIST_CONTRAL_CARDSELECT, TRUE, 0, FALSE);
	m_lsList.SetExtendedStyle(m_lsList.GetExtendedStyle()|LVS_EX_FULLROWSELECT);
	m_lsList.InsertColumn(0, _T("姓名"), LVCFMT_LEFT, 250);
	m_lsList.InsertColumn(1, _T("电话号码"), LVCFMT_LEFT, 180+60);

	m_lsList.ShowWindow(false);
	m_lsType.ShowWindow(false);
//	m_lsList.InsertColumn(2, _T("Duty"), LVCFMT_LEFT, 150-22+26);
//	m_lsList.Create(WS_CHILD|WS_VISIBLE|LVS_REPORT|LVS_NOCOLUMNHEADER|LVS_NOSORTHEADER, CRect(115, 45, 115+572, 45+303), this, IDC_LIST_CONTRAL_CARDSELECT, TRUE, 0, FALSE);

	m_pImageList = new CImageList();
	m_pImageList->Create(32, 32, ILC_COLOR32|ILC_MASK, 3, 2);   
	
	CBitmap bm;
	bm.LoadBitmap(IDB_BITMAP_SELECT1);
	m_pImageList->Add(&bm, RGB(255, 0, 255)); 
	bm.DeleteObject();	
	bm.LoadBitmap(IDB_BITMAP_UNSELECT1);
	m_pImageList->Add(&bm, RGB(255, 0, 255)); 
	bm.DeleteObject();

	m_lsList.SetImageList(m_pImageList, LVSIL_SMALL);
	
	int xbegin = 652 ,ybegin = 10 ;
	int btnwidth = 130,btnheight = 55;
	int space = 25 ;//按钮之间的间隔
	m_btnSelectAll.Create(L"全部选择",WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON,CRect(xbegin,ybegin,xbegin+btnwidth,ybegin+btnheight),this,IDC_BTN_SELECTALL);
	m_btnSelectAll.SetColor(CCEButtonST::BTNST_COLOR_BK_IN, RGB(189, 206, 239));
	m_btnSelectAll.SetColor(CCEButtonST::BTNST_COLOR_BK_OUT, RGB(189, 206, 239));
	m_btnSelectAll.SetColor(CCEButtonST::BTNST_COLOR_BK_FOCUS, RGB(189, 206, 239));
	m_btnSelectAll.DrawBorder();
	
	//xbegin += btnwidth + space ;
	ybegin += btnheight + space ;
	m_btnCancelAll.Create(L"取消选择",WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON,CRect(xbegin,ybegin,xbegin+btnwidth,ybegin+btnheight),this,IDC_BTN_CANCELALL);
	m_btnCancelAll.SetColor(CCEButtonST::BTNST_COLOR_BK_IN, RGB(189, 206, 239));//RGB(248,214,147)
	m_btnCancelAll.SetColor(CCEButtonST::BTNST_COLOR_BK_OUT, RGB(189, 206, 239));
	m_btnCancelAll.SetColor(CCEButtonST::BTNST_COLOR_BK_FOCUS, RGB(189, 206, 239));
	m_btnCancelAll.DrawBorder();
	
	//xbegin += btnwidth + space ;
	ybegin += btnheight + space ;
	m_btnOK.Create(L"确定",WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON,CRect(xbegin,ybegin,xbegin+btnwidth,ybegin+btnheight),this,MMS_BTN_OK);
	m_btnOK.SetColor(CCEButtonST::BTNST_COLOR_BK_IN, RGB(189, 206, 239));
	m_btnOK.SetColor(CCEButtonST::BTNST_COLOR_BK_OUT, RGB(189, 206, 239));
	m_btnOK.SetColor(CCEButtonST::BTNST_COLOR_BK_FOCUS, RGB(189, 206, 239));
	m_btnOK.DrawBorder();
	
	//xbegin += btnwidth + space ; 
	ybegin += btnheight + space ;
	m_btnCancel.Create(L"取消",WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON,CRect(xbegin,ybegin,xbegin+btnwidth,ybegin+btnheight),this,MMS_BTN_CANCEL);
	m_btnCancel.SetColor(CCEButtonST::BTNST_COLOR_BK_IN, RGB(189, 206, 239));
	m_btnCancel.SetColor(CCEButtonST::BTNST_COLOR_BK_OUT, RGB(189, 206, 239));
	m_btnCancel.SetColor(CCEButtonST::BTNST_COLOR_BK_FOCUS, RGB(189, 206, 239));
	m_btnCancel.DrawBorder();

	ShowCard();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCardSelectDlg::SetButtonDefaultColor(CCEButtonST* button)
{
	button->SetColor(CCEButtonST::BTNST_COLOR_BK_IN, RGB(248,214,147));
	button->SetColor(CCEButtonST::BTNST_COLOR_BK_OUT, RGB(248,214,147));
	button->SetColor(CCEButtonST::BTNST_COLOR_BK_FOCUS, RGB(248,214,147));
}

void CCardSelectDlg::SetButtonSelectedColor(CCEButtonST* button)
{
	button->SetColor(CCEButtonST::BTNST_COLOR_BK_IN, RGB(248,214,147));
	button->SetColor(CCEButtonST::BTNST_COLOR_BK_OUT, RGB(248,214,147));
	button->SetColor(CCEButtonST::BTNST_COLOR_BK_FOCUS, RGB(248,214,147));
}

void CCardSelectDlg::OnClickListType(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here

	//	GetDlgItem(IDC_STATIC_CONTACT_PINYIN)->SetWindowText(_T(""));
	
	//在类型切换的时候清除 vector
	if (m_vContact.size() > 0)
	{
		m_vContact.clear();
	}
 	ShowItemsInList();

	*pResult = 0;
}

void CCardSelectDlg::ShowTypeItems(void)
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
	if (Data::ContactGroup::GetDataCount("") > 0)
	{
		result = Data::ContactGroup::GetFromDatabase("");
		for (int i = 0; i < result.size(); ++i)
		{
			m_lsType.InsertItem(i + 1, CString(result[i]->name().c_str())/*, imageID*/);
			m_lsType.SetItemData(i + 1, result[i]->id());
			++imageID;
			if (imageID > 10)
			{
				imageID = 2;
			}
		}

	}

//	strTemp = Data::LanguageResource::Get(Data::RI_CARD_FINDREAULTSTC);
//	str = strTemp.c_str();
//	m_lsType.InsertItem(result.size() + 1, str/*, 1*/);
//	m_lsType.SetItemData(result.size() + 1, result.size() + 1);
	m_lsType.SetScrollRagle(TRUE);

}

void CCardSelectDlg::ShowItemsInList(void)
{
	//类型
	POSITION pos = m_lsType.GetFirstSelectedItemPosition();
	if (pos != NULL)
	{
		m_lsList.DeleteAllItems();

		m_sListFilter = "";

		std::string filter;
		std::string aliasFilter;
	/*	std::string alias = GetPYIndex();
		if (alias != "")
		{
			aliasFilter = "alias LIKE '";
			aliasFilter += alias;
			aliasFilter += "%'";
		}
	*/
		int type = m_lsType.GetNextSelectedItem(pos);
		if (type == 0)
		{
			filter = aliasFilter;
		}
		else
		{
			m_sListFilter = "groupId = " + Util::StringOp::FromInt(m_lsType.GetItemData(type));
			filter = m_sListFilter;
		}	

		m_lsType.SetCheck(type);

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

void CCardSelectDlg::ScrollItemsInList(int step, int nPos)
{
	
	if (m_vCurrentResult.empty())
	{
		return;
	}
	
	std::string filter = m_sListFilter;
	POSITION pos = m_lsType.GetFirstSelectedItemPosition();
	if (pos != NULL)
	{
		/*int type = m_lsType.GetNextSelectedItem(pos);
		if (type == m_lsType.GetItemCount() - 1)
		{
			filter = m_sListSearchFilter;
		}
		*/
	}

/*	std::string alias = GetPYIndex();
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
*/
	m_vCurrentResult.clear();
	m_vCurrentResult = Data::Contact::GetFromDatabaseByTypeOffsetLength(filter, nPos, PageSize);
	
	ShowArrayInList(m_vCurrentResult);		
}

void CCardSelectDlg::ShowArrayInList(std::vector<boost::shared_ptr<Data::Contact> > array)
{	
	// 记录下当前页的状态
	// 设置主列表	
	m_lsList.DeleteAllItems();
	CString temp;
	int id = 1;
	for (int i = 0; i < (array).size(); ++i)
	{	
		temp = array[i]->name().c_str();
		std::string name    = array[i]->name() ;
		std::string telnum  = array[i]->mobilesTelephone().number();
		//全选
		if (m_bSelectAll)
		{	
			id = 0 ;
			m_lsList.InsertItem(i, temp, 0);//设置选中	
		}
		else
		{
			for (int j = 0 ; j < m_vContact.size();j++)
			{
				if (0 == m_vContact[j].name.compare(name) && 0 == m_vContact[j].telnum.compare(telnum))
				{	
					id = 0 ;
					m_lsList.InsertItem(i,temp,0);
					break ;
				}
			}
			if ( j == m_vContact.size())//vector中没有就打X
			{	
				id = 1;
				m_lsList.InsertItem(i,temp,1);
			}
		}

		temp = array[i]->mobilesTelephone().number().c_str();//插入号码
		m_lsList.SetItemText(i,1,temp);
	//	temp = array[i]->company().c_str();
	//	m_lsList.SetItemText(i, 1, temp);
	//	temp = array[i]->duty().c_str();
	//	m_lsList.SetItemText(i, 2, temp);
		m_lsList.SetItemData(i, id);
	}
}

void CCardSelectDlg::OnClickListList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	//由于第一次点击名片列表经常不能显示名片详细信息，启动一个定时器，在定时器中处理.		
	POSITION pos = m_lsList.GetFirstSelectedItemPosition();
	if (pos != NULL)
	{
		int index  = m_lsList.GetNextSelectedItem (pos);
		CString cname = m_lsList.GetItemText(index,0);
		std::string	name = Util::StringOp::FromCString(cname) ;
		CString cphone = m_lsList.GetItemText(index,1);
		std::string  mobilephone = Util::StringOp::FromCString(cphone);
		Util::ATCommandWarp::SIM_FORMAT vc ;
		
		int id = m_lsList.GetItemData(index);
		if ( 1 == id && m_lsList.GetItemState(index,LVIS_SELECTED) ==  LVIS_SELECTED)//如果已经被点击，
		{	
			if (m_vContact.size() < MAX_SEND_CONTACT)
			{
				vc.name   = name ;
				vc.telnum = mobilephone ;
				//vector中没有就插入
				m_vContact.push_back(vc);
			}
		}
		else if (0 == id && m_lsList.GetItemState(index,LVIS_SELECTED) ==  LVIS_SELECTED)//如果已经被点击，
		{				
			std::vector<Util::ATCommandWarp::SIM_FORMAT>:: iterator it;
			for (it = m_vContact.begin() ; it < m_vContact.end() ; it++)
			{	
				if ( (0 == (*it).name.compare(name) && 0 == (*it).telnum.compare(mobilephone))
					|| 0 == (*it).telnum.compare(mobilephone))
				{
					m_vContact.erase(it);
					break ;
				}
			}
		}

		LVITEM lvitem ;
		id=(id == 0)?1:0;
		lvitem.mask =LVIF_TEXT | LVIF_IMAGE;   
		lvitem.iSubItem = 0;   
		lvitem.lParam =	0; 
		lvitem.iImage = id ;	
		
		int count = m_lsList.GetItemCount();
		for (int i = 0 ; i < count ;i++)//相同项也打钩
		{
			CString name =  m_lsList.GetItemText(i,0);
			CString phone = m_lsList.GetItemText(i,1);
			if ( 0 == cname.Compare(name) && 0 == cphone.Compare(phone))
			{
				lvitem.iItem = i;   
				lvitem.pszText =(LPTSTR)(LPCTSTR)m_lsList.GetItemText(i,0); 
				m_lsList.SetItem(&lvitem);
				m_lsList.SetItemData(index,id);
			}
		}
	//	m_lsList.SetItemState(index,0,LVIS_SELECTED);//设置选中状态
		
	}

	//SetTimer(1, 50, NULL);
	*pResult = 0;
}

void CCardSelectDlg::OnTimer(UINT nIDEvent)
{
	if(nIDEvent == 1)
	{
		KillTimer(1);
		POSITION pos = m_lsList.GetFirstSelectedItemPosition();
		Dprintf("List click!!!!\n");
		if (pos != NULL)
		{
			int index = m_lsList.GetNextSelectedItem (pos);
			Dprintf("List click %d!!!!\n", index);
			int id = m_lsList.GetItemData(index);
			std::vector<boost::shared_ptr<Data::Contact> > result = Data::Contact::GetFromDatabase("id = " + Util::StringOp::FromInt(id));
			if (!result.empty())
			{
				((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactNewDlg->SetContact(result[0]);
				::SetFocus(((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactNewDlg->m_hWnd);
				((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactNewDlg->ShowWindow(TRUE);

			}
		}
	}
	CDialog::OnTimer(nIDEvent);
}

void CCardSelectDlg::OnButtonContactNew() 
{
	// TODO: Add your control notification handler code here
	if (Data::Contact::GetDataCount("") < ContactTotal)
	{
		boost::shared_ptr<Data::Contact> contact(new Data::Contact);
		contact->id(0);
		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactNewDlg->SetContact(contact);
		::SetFocus(((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactNewDlg->m_hWnd);
		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactNewDlg->ShowWindow(TRUE);
	}
	else
	{
		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pWarningNoFlashDlg->ShowWindow_(SW_SHOW);
	}
}

void CCardSelectDlg::OnButtonContactNewtype() 
{
	// TODO: Add your control notification handler code here
	if (Data::ContactGroup::GetDataCount("") < ContactGroupTotal)
	{
		boost::shared_ptr<Data::ContactGroup> contactgroup(new Data::ContactGroup);
		contactgroup->id(0);
		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactGroupDlg->SetContactGroup(contactgroup);
		::SetFocus(((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactGroupDlg->m_hWnd);
		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactGroupDlg->ShowWindow(TRUE);
	}
	else
	{
		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pWarningNoFlashDlg->ShowWindow_(SW_SHOW);
	}
}

void CCardSelectDlg::OnButtonContactEdittype() 
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
			::SetFocus(((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactGroupDlg->m_hWnd);
			((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactGroupDlg->ShowWindow(TRUE);
		}
	}
}

void CCardSelectDlg::OnButtonContactSearch() 
{
	// TODO: Add your control notification handler code here
	m_pContactSearchDlg->ShowWindow(TRUE);
}

void CCardSelectDlg::OnButtonContactClose()
{
	((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->SwitchPanel_(IDC_BUTTON_MAIN);
}

int CCardSelectDlg::GetTypeListSelected(void)
{
	POSITION pos = m_lsType.GetFirstSelectedItemPosition();
	if (pos != NULL)
	{
		int index = m_lsType.GetNextSelectedItem (pos);
		return index;
	}
	return -1;
}


LRESULT CCardSelectDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	switch (message)
	{
	case WM_SCROLL_EDO:
		ScrollItemsInList(wParam, lParam);
		break;
	};
	
	return CDialog::WindowProc(message, wParam, lParam);
}

//add by qi 2009_08_05
void CCardSelectDlg::OnBtnSelectAll()
{
	m_bSelectAll = TRUE ;
	int count = m_lsList.GetItemCount();
	for (int i = 0 ; i < count ; i++)
	{		
		int id = 0 ;
		LVITEM lvitem;
		lvitem.mask =LVIF_TEXT | LVIF_IMAGE;   
		lvitem.iItem = i;   
		lvitem.iSubItem = 0;   
		lvitem.pszText =(LPTSTR)(LPCTSTR)m_lsList.GetItemText(i,0);   
		lvitem.lParam =	0;   
		lvitem.iImage =	0;
		m_lsList.SetItem(&lvitem);
		m_lsList.SetItemData(i, id);
		
		//相应数据放到vector中
		CString cs	= m_lsList.GetItemText(i,0);
		std::string	name = Util::StringOp::FromCString(cs) ;
		cs = m_lsList.GetItemText(i,1);
		std::string  mobilephone = Util::StringOp::FromCString(cs);
		Util::ATCommandWarp::SIM_FORMAT vc ;
		vc.name		= name	;	
		vc.telnum   = mobilephone ;
		for (int j = 0 ; j < m_vContact.size() ; j++)
		{
			if ( 0 == m_vContact[j].name.compare(name) && 0 == m_vContact[j].telnum.compare(mobilephone))
			{
				break ;
			}
		}

		if ( j == m_vContact.size() && m_vContact.size() < MAX_SEND_CONTACT)//Vector没有对应的数据就插入
		{
			m_vContact.push_back(vc);
		}
		
	}
}
void CCardSelectDlg::OnBtnCancelAll()
{
	m_bSelectAll = FALSE ;
	int count = m_lsList.GetItemCount();
	for (int i = 0 ; i < count ; i++)
	{		
		int id	= 1;
		LVITEM lvitem;
		lvitem.mask =LVIF_TEXT | LVIF_IMAGE;   
		lvitem.iItem = i;   
		lvitem.iSubItem = 0;   
		lvitem.pszText =(LPTSTR)(LPCTSTR)m_lsList.GetItemText(i,0);   
		lvitem.lParam =	0;   
		lvitem.iImage =	1;
		m_lsList.SetItem(&lvitem);
		m_lsList.SetItemData(i, id);

		//删除相应vector中的数据
		CString cs	= m_lsList.GetItemText(i,0);
		std::string	name = Util::StringOp::FromCString(cs) ;
		cs = m_lsList.GetItemText(i,1);
		std::string  mobilephone = Util::StringOp::FromCString(cs);
		Util::ATCommandWarp::SIM_FORMAT vc ;
		std::vector<Util::ATCommandWarp::SIM_FORMAT>::iterator it ;
		vc.name		= name	;	
		vc.telnum   = mobilephone ;
		for (it = m_vContact.begin() ; it < m_vContact.end() ;it++)
		{
			if ( 0 == (*it).name.compare(name) && 0 == (*it).telnum.compare(mobilephone))
			{
				m_vContact.erase(it);
			}
		}
	}
}
void CCardSelectDlg::OnBtnOk()
{   
	ShowWindow(false);
}
void CCardSelectDlg::OnBtnCancel()
{	
	m_bSelectAll = false ;
	if (m_vContact.size() > 0)
	{
		m_vContact.clear();
	}
	//列表清空
	ShowWindow(false);
}

void CCardSelectDlg::GetSendContact(std::vector<Util::ATCommandWarp::SIM_FORMAT>&vetor)
{	
	for (int i = 0 ; i < m_vContact.size();i++)
	{
		vetor.push_back(m_vContact[i]);
	}	
}

void CCardSelectDlg::ShowCard()
{	
	if (m_vContact.size() >0)
	{
		m_vContact.clear();
	}
	m_bSelectAll = FALSE ;
	ShowTypeItems();
	m_lsType.SetItemState(0, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);	
	m_sListFilter = "";
	m_sListSearchFilter = "";	
	ShowItemsInList();
}