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
#include < Sipapi.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define  IDC_EDIT_INPUT  1

/////////////////////////////////////////////////////////////////////////////
// CContactInfoDlg dialog

CContactInfoDlg::CContactInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CContactInfoDlg::IDD, pParent)
	, ContactInfoTotal(10000)
	, PageSize(6)
{
	//{{AFX_DATA_INIT(CContactInfoDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_bSelectAll = false ;
	m_bSearch = FALSE;
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
	ON_BN_CLICKED(IDC_BUTTON_CONTACTINFO_DIAL, OnButtonDial)
	ON_BN_CLICKED(IDC_BUTTON_CONTACTINFO_SOUND, OnButtonSound)
	ON_BN_CLICKED(IDC_BUTTON_CONTACTINFO_NOTE, OnButtonNote)
	ON_BN_CLICKED(IDC_BUTTON_CONTACTINFO_NEW, OnButtonNew)
	ON_BN_CLICKED(IDC_BUTTON_CONTACTINFO_SEARCH, OnButtonSearch)
	ON_BN_CLICKED(IDC_BUTTON_CONTACTINFO_DELETE, OnButtonDelete)
	ON_MESSAGE(WM_CLICKMJPG_TOAPP, OnClickMJPG)
	ON_MESSAGE(WM_MJPGTOGGLE,OnClickMJPG)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CContactInfoDlg message handlers

void CContactInfoDlg::OnClickMJPG(WPARAM w, LPARAM l)
{
	CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg *)theApp.m_pMainWnd;
	CString content(L"");
	CString icon ;

	switch (w)
	{
	case 0://未接电话
	case 1://已接电话
	case 2://已拨电话
	case 3://所有来电
		m_iTabs = w ;
		SetUpBtn(w);
		Clear();
		ClearCurrentPage();
		SetContactInfoFilter(w);
		FromContactInfoDataBase();
		ShowTypeInfo();
		break;

	case 4://查找		
 		OnButtonSearch();
		break;

	case 7: //上移
		PageSwitch(up_page);
		break;

	case 8://下移
		PageSwitch(down_page);
		break;

	case 1000: //返回
		ShowWindow(SW_HIDE);
		main->PopbackIcon();
		break;

	case 1001: //确定
		OnBtnOK();
		break;	
		
	case 11://短信息
		if (OnBtnSMS())
		{
			icon = Allicon[1];
			main->AddIcon(icon);
		}		
		break;

	case 12://拨打
		OnButtonDial();
		break;

	case 13://删除
		OnButtonDelete();
		break;

	case 14://听留言
		OnButtonSound();
		break;

	case 15://新建名片
		OnButtonNew();
		break;

	case 50://全选
		SelectAll();
		break;

	case 100://勾选
	case 200:
	case 300:
	case 400:
	case 500:
	case 600:
		ClickedOneItem(w,w/100-1);
		break;

	case 101://勾选
	case 102:
	case 103:
	case 104:
	case 105:
	case 106:
	case 107:

	case 201://勾选
	case 202:
	case 203:
	case 204:
	case 205:
	case 206:
	case 207:

	case 301://勾选
	case 302:
	case 303:
	case 304:
	case 305:
	case 306:
	case 307:

	case 401://勾选
	case 402:
	case 403:
	case 404:
	case 405:
	case 406:
	case 407:

	case 501://勾选
	case 502:
	case 503:
	case 504:
	case 505:
	case 506:
	case 507:

	case 601://勾选
	case 602:
	case 603:
	case 604:
	case 605:
	case 606:
	case 607:
		SeeOneItem(w - w%100);
		break;

	case 801://切换到字母
		m_MJPGData_key.ShowWindow(SW_HIDE);
		m_MJPGLetter_key.ShowWindow(SW_SHOW);
		break;
	case 802://删除
		content = m_MJPGData_key.GetUnitText(800);
		content.Delete(content.GetLength()-1);
		m_MJPGData_key.SetUnitText(800,content,true);
		Add(L"");
		break;
	case 803://清空
		m_MJPGData_key.SetUnitText(800,L"",true);
		m_sAddtion = "";
		m_MJPGList.SetUnitText(5, L"0/", TRUE);
		m_MJPGList.SetUnitText(6, L"0", TRUE);
		Clear();
		ClearCurrentPage();
		break;

	case 804:  //?号
		 Add(L"?");
		break;
	case 810://9-0
		m_MJPGData_key.SetUnitFont(800,font_30);
		m_MJPGData_key.SetUnitColor(800,font_black,true);
		Add(L"1");
		break;

	case 811:
		Add(L"2");
		break;
	case 812:
		Add(L"3");
		break;
	case 813:
		Add(L"4");
		break;	
	case 814:
		Add(L"5");
		break;	
	case 815:
		Add(L"6");
		break;
	case 816:
		Add(L"7");
		break;	
	case 817:
		Add(L"8");
		break;
	case 818:
		Add(L"9");
		break;
	case 819:
		Add(L"0");
		break;

	case 901: //切换到数字
		m_MJPGLetter_key.ShowWindow(SW_HIDE);
		m_MJPGData_key.ShowWindow(SW_SHOW);
		break;
	case 902://删除
		content = m_MJPGLetter_key.GetUnitText(900);
		content.Delete(content.GetLength()-1);
		m_MJPGLetter_key.SetUnitText(900,content,true);
		Add(L"");
		break;
	case 903://清空
		m_MJPGLetter_key.SetUnitText(900,L"",true);
		m_sAddtion = "";
		m_MJPGList.SetUnitText(5, L"0/", TRUE);
		m_MJPGList.SetUnitText(6, L"0", TRUE);
		Clear();
		ClearCurrentPage();
		break;
	case 904://问号
		Add(L"?");
		break;

	case 910://310-335,a-z
		Add(L"a");
		break;	
	case 911:
		Add(L"b");
		break;
	case 912:
		Add(L"c");
		break;
	case 913:
		Add(L"d");
		break;
	case 914:
		Add(L"e");
		break;
	case 915:
		Add(L"f");
		break;
	case 916:
		Add(L"g");
		break;
	case 917:
		Add(L"h");
		break;
	case 918:
		Add(L"i");
		break;
	case 919:
		Add(L"j");
		break;
	case 920:
		Add(L"k");
		break;
	case 921:
		Add(L"l");
		break;
	case 922:
		Add(L"m");
		break;
	case 923:
		Add(L"n");
		break;
	case 924:
		Add(L"o");
		break;
	case 925:
		Add(L"p");
		break;
	case 926:
		Add(L"q");
		break;
	case 927:
		Add(L"r");
		break;
	case 928:
		Add(L"s");
		break;
	case 929:
		Add(L"t");
		break;
	case 930:
		Add(L"u");
		break;
	case 931:
		Add(L"v");
		break;
	case 932:
		Add(L"w");
		break;
	case 933:
		Add(L"x");
		break;
	case 934:
		Add(L"y");
		break;
	case 935:
		Add(L"z");
		break;
	}
}

BOOL CContactInfoDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	// TODO: Add extra initialization here
	MoveWindow(0, 57, 800, 480);

	std::string strTemp;
	CString str;
	
// 	m_rEditInput = new CCERectEdit();
// 	m_rEditInput->Create(WS_CHILD|WS_VISIBLE, CRect(461,77,32,16), this, IDC_EDIT_INPUT, 15);
// 	m_rEditInput->SetIsAutoInput(TRUE);
// 	m_rEditInput->MoveWindow(461, 77, 32, 16);
// 	m_rEditInput->ShowWindow(SW_HIDE);

	//创建数字键盘
	m_MJPGData_key.Create(L"", WS_CHILD, CRect(0, 308, 800, 400), this);
	m_MJPGData_key.SetCurrentLinkFile(".\\adv\\mjpg\\k5\\中文\\data_key.xml");
	m_MJPGData_key.SetMJPGRect(CRect(0, 308, 800, 400));
	// 	
	m_MJPGLetter_key.Create(L"", WS_CHILD, CRect(0, 308, 800, 400), this);
	m_MJPGLetter_key.SetCurrentLinkFile(".\\adv\\mjpg\\k5\\中文\\letter_key.xml");
 	m_MJPGLetter_key.SetMJPGRect(CRect(0, 308, 800, 400));

	m_MJPGList.Create(L"", WS_VISIBLE|WS_CHILD, CRect(0, 0, 800, 420), this);
	m_MJPGList.SetCurrentLinkFile(".\\adv\\mjpg\\k5\\中文\\通话记录界面.xml");
	m_MJPGList.SetMJPGRect(CRect(0, 0, 800, 420));	

	m_pPlaySoundDlg = new CPlaySoundDlg(this);
	m_pPlaySoundDlg->Create(CPlaySoundDlg::IDD);
	m_pPlaySoundDlg->SetModel(1);

	m_pPasswordDlg = new CPasswordDlg(this);
	m_pPasswordDlg->Create(CPasswordDlg::IDD);

	m_bTelephoneInUse = false;
	
	m_sListFilter = "";
	m_sListSearchFilter = "";
	
	m_iTabs = 3;
	SetPageFont();
	ShowWindow_();	

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

void CContactInfoDlg::OnButtonDial() 
{	
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd ;
	int num = 0;
	for (int i = 0 ; i < m_vClick.size() ;i++)
	{
		if (1 == m_vClick[i])
		{	
			num++;
		}	
	}
	
	if (num > 1)
	{	
		CString title = L"不能选择多个拨打!" ;
		main->m_pWarningNoFlashDlg->SetTitle(title);
		main->m_pWarningNoFlashDlg->ShowWindow_(SW_SHOW);
		return ;
	}

	for (i = 0 ; i < m_vClick.size() ;i++)
	{
		if (1 == m_vClick[i])
		{	
			break ;
		}	
	}
	std::string filter = "id = " + Util::StringOp::FromInt(m_vContactID[i]);
	std::vector<boost::shared_ptr<Data::ContactInfo> > result = Data::ContactInfo::GetFromDatabase(filter);
	
	if (!result.empty() && result[0]->GetContact())
	{
		std::string number = result[0]->telephoneNumber().number();
		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pTelephoneDlg->DialContact(number, result[0]->GetContact()->id());
	}
	else
	{	
		std::string number = result[0]->telephoneNumber().number();
		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pTelephoneDlg->DialContact(number);
	}	

}

void CContactInfoDlg::OnButtonSound() 
{
	for (int i = 0 ; i < m_vClick.size() ;i++)
	{
		if (1 == m_vClick[i])
		{	
			break ;
		}	
	}
	
	//这里需要加警告框
	
	std::string filter = "id = ";
	filter += Util::StringOp::FromInt(m_vContactID[i]);
	std::vector< boost::shared_ptr<Data::ContactInfo> > result ;
	result	= Data::ContactInfo::GetFromDatabase(filter);

	if ( result.size() && result[0]->GetSoundsCount() > 0)
	{
		m_pPlaySoundDlg->SetSound((int)result[0]->type(), result[0]->id(), 0, m_sListSearchFilter);
		if (((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pSettingDlg->m_pTempSetting->isPlayProtect() && !((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pSettingDlg->m_bLogin)
		{
			m_pPasswordDlg->SetType(CHECK_PLAYPASSWORD);
			std::string strTemp = ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pSettingDlg->m_pTempSetting->playRecordPassword();
			m_pPasswordDlg->SetOldPassWord((char *)strTemp.c_str());
			m_pPasswordDlg->SetHWnd(this->m_hWnd);
			m_pPasswordDlg->ShowWindow_(SW_SHOW);	
		}
		else
		{
			m_pPlaySoundDlg->StartPlay(false);
			m_pPlaySoundDlg->ShowWindow_(TRUE);
		}
	}

}

void CContactInfoDlg::OnButtonNote() 
{

	CMultimediaPhoneDlg *pMain = ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd));

}

void CContactInfoDlg::OnButtonNew() 
{	
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd ;
	int num = 0;
	for (int i = 0 ; i < m_vClick.size() ;i++)
	{
		if (1 == m_vClick[i])
		{	
			num++;
		}	
	}
	
	if (num > 1)
	{	
		CString title = L"不能选择多个去新联系人!" ;
		main->m_pWarningNoFlashDlg->SetTitle(title);
		main->m_pWarningNoFlashDlg->ShowWindow_(SW_SHOW);
		return ;
	}

	if (Data::Contact::GetDataCount("") < ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactDlg->ContactTotal)
	{	
		int index = -1; 
		for (int i = 0 ; i < m_vClick.size() ;i++)
		{
			if (1 == m_vClick[i])
			{	
				index = i ;
				break;
			}	
		}
		

		if (index > -1)
		{	
			std::string filter;
			filter = "id = " + Util::StringOp::FromInt(m_vContactID[index]);
			std::vector<boost::shared_ptr<Data::ContactInfo> > result = Data::ContactInfo::GetFromDatabase(filter);

			//更新
			if ( result.size() && result[0]->contactId() > 0)
			{
				std::string filter;
				filter = "id = " + Util::StringOp::FromInt(result[0]->contactId());
				std::vector<boost::shared_ptr<Data::Contact> > result = Data::Contact::GetFromDatabase(filter);
				if (!result.empty())
				{
					//	((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->SwitchPanel_(IDC_BUTTON_CONTACT);
					((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactNewDlg->SetContact(result[0]);
					((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactNewDlg->ShowWindow(TRUE);
				}
			}
			else
			{
				//插入
				boost::shared_ptr<Data::Contact> contact(new Data::Contact);
				contact->id(0);
				std::string stelnum = result[0]->telephoneNumber().number();
				CString number = Util::StringOp::ToCString(stelnum);
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
				
				//((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->SwitchPanel_(IDC_BUTTON_CONTACT);
				((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactNewDlg->SetContact(contact);
				((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactNewDlg->ShowWindow(TRUE);
			}
		}
		
	}
	else
	{
		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pWarningNoFlashDlg->ShowWindow_(SW_SHOW);
	}
}

void CContactInfoDlg::OnButtonSearch() 
{	
	CString hideIcon1 = L".\\adv\\mjpg\\k5\\common\\隐藏_fore.bmp";
	CString hideIcon2 = L".\\adv\\mjpg\\k5\\common\\隐藏_back.bmp";
	
	if (!m_bSearch)
	{	
		m_MJPGList.SetUnitBitmap(4,hideIcon1,hideIcon2,true);
		m_MJPGLetter_key.ShowWindow(SW_SHOW);
		m_bSearch = true ;
	}
	else
	{	
		SetContactInfoFilter(3);
		m_MJPGList.SetUnitBitmap(4,L"",L"",true);
		m_MJPGLetter_key.ShowWindow(SW_HIDE);
		m_MJPGData_key.ShowWindow(SW_HIDE);
		m_bSearch = false ;
		m_sAddtion = "";
		m_MJPGData_key.SetUnitText(800,	L"",true);
		m_MJPGLetter_key.SetUnitText(900,L"",true);
	}

}

void CContactInfoDlg::OnButtonDelete() 
{
	bool enble = false ;
	int  count = 0 ;
	
	for (int i = 0 ; i < m_vClick.size() ;i++)
	{
		if (1 == m_vClick[i])
		{	
			enble = true ;
			count++;
		}	
	}
	
	if (enble)
	{	
		CString title = "确认删除已选的内容吗?";
		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetTitle(title,0);
		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetProcessMax(count);
		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetHWnd(this->GetSafeHwnd());
		if (((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pSettingDlg->m_pTempSetting->isAdmin() && !((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pSettingDlg->m_bLogin)
		{
			((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetPasswordModel(true);
		}
		std::string pw = ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pSettingDlg->m_pTempSetting->adminPassword();
		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetPassword(Util::StringOp::ToCString(pw));
		std::string strTemp = Data::LanguageResource::Get(Data::RI_DELETETIP_CONTACTINFO);
		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetDelTip(strTemp.c_str());
		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->ShowWindow_(TRUE);

	}
}


void CContactInfoDlg::SaveContactInfo(boost::shared_ptr<Data::ContactInfo> ci)
{
	if (ci->id() == 0)
	{
		ci->Insert();
	}
	else
	{
		ci->Update();	
	}

	int count = Data::ContactInfo::GetDataCount("");
	if (count > ContactInfoTotal)
	{
		std::vector<boost::shared_ptr<Data::ContactInfo> > temp = Data::ContactInfo::GetFromDatabase("", Data::dUp, 0, count - ContactInfoTotal);
		if (!temp.empty())
		{
			for (int i = 0; i < temp.size(); ++i)
			{
				temp[i]->Remove();
			}
		}
	}
}

void CContactInfoDlg::Search(std::string filter)
{
	m_sListSearchFilter = filter;
}

void CContactInfoDlg::ShowUnconnectItems(void)
{
	SetUpBtn(0);
	Clear();
	ClearCurrentPage();
	SetContactInfoFilter(0);
	FromContactInfoDataBase();
	ShowTypeInfo();
}

LRESULT CContactInfoDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class	
	switch (message)
	{
	case WM_DELETESELITEM:
		DeleteItems();
		break;
		
	case WM_CHECKPASSWORD:
		if(wParam == 1)
		{
			m_pPlaySoundDlg->StartPlay(false);
			m_pPlaySoundDlg->ShowWindow_(TRUE);
		}
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
	};
	return CDialog::WindowProc(message, wParam, lParam);
}


void CContactInfoDlg::Add(CString ch)
{
	CString content ;
	int index ;
	if (m_MJPGData_key.IsWindowVisible())
	{	
		index = 800 ;
		content = m_MJPGData_key.GetUnitText(800);
		content = content + ch ;
		m_MJPGData_key.SetUnitText(800,content,true);
		
	}	
	else if (m_MJPGLetter_key.IsWindowVisible())
	{   
		index = 900 ;
		content = m_MJPGLetter_key.GetUnitText(900);
		content = content + ch ;
		m_MJPGLetter_key.SetUnitText(900,content,true);
		
	}
	
	
	if (content.IsEmpty())//没有内容直接返回
	{	
		CString page ;
		page.Format(L"%d/%d",0,0);
		m_MJPGList.SetUnitText(6,page,TRUE);//设置当前的页数
		
		Clear();
		ClearCurrentPage();
		return ;	
	}
	
	//查询
	std::string tempFilter;
	std::string filter = GetPYIndex(content);
	if (filter != "")
	{	
		if (800 == index)//电话号码查找
		{
			tempFilter = "(telephoneNumber Like '";
			tempFilter += filter;
			tempFilter += "%')";
			m_sAddtion = tempFilter ;
		}
		
		if (900 == index)//按字母查找
		{
			tempFilter = "name LIKE '";
			tempFilter += filter;
			tempFilter += "%'";
			m_sAddtion = tempFilter;
		}
		
	}
	
	m_sListFilter = m_sAddtion;
	Clear();
	ClearCurrentPage();
	FromContactInfoDataBase();
	ShowTypeInfo();
	SetUpBtn(3);	
}

std::string CContactInfoDlg::GetPYIndex(CString content)
{
	std::string alias;
	CString py = content;
	//	GetDlgItem(IDC_STATIC_CONTACT_PINYIN)->GetWindowText(py);
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

std::string CContactInfoDlg::GetType(std::string number)
{
	std::string filter;
	int groupID ;
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
		std::string name = result[0]->name();
		groupID = result[0]->groupId();
		filter = "id = " + Util::StringOp::FromInt(groupID);
	}
	
	std::vector<boost::shared_ptr<Data::ContactGroup> > Groupresult = Data::ContactGroup::GetFromDatabase(filter);
	
	if (Groupresult.size() > 0)
	{
		return Groupresult[0]->name();	
	}
	
	return "";
}


bool CContactInfoDlg::OnBtnSMS()
{	
	std::vector<CString> telNumber ;
	std::vector<CString> vname ;
	GetNumName(vname,telNumber);
	if (vname.size())
	{	
		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pMainDlg->m_p3GSMSDlg->SetSender(vname);
		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pMainDlg->m_p3GSMSDlg->SetAppend(telNumber);
		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pMainDlg->m_p3GSMSDlg->ShowWindow(TRUE);
		 return true ;
	}
	return false ;
}


void CContactInfoDlg::SetUpBtn(int ID)
{
	m_MJPGList.SetUnitIsDownStatus(ID,TRUE);
	for (int i = 0 ; i < 4;i++)
	{	
		if (i != ID)
		{
			m_MJPGList.SetUnitIsDownStatus(i,FALSE);
		}
	}
	m_MJPGList.Invalidate(FALSE);
}

void CContactInfoDlg::PageSwitch(Action action)
{	
	
	if (action == up_page)
	{
		m_iCurrentPage--;
		if (m_iCurrentPage < 1)
		{	
			m_iCurrentPage = 1;
			return ;
		}
	}
	
	if (action == down_page)
	{
		m_iCurrentPage++;
		if (m_iCurrentPage > m_iTotalPages)
		{
			m_iCurrentPage--;
			return ;
		}
		
	}
	
	ClearCurrentPage();
	FromContactInfoDataBase();
	ShowTypeInfo();		
}

void CContactInfoDlg::ClearCurrentPage()
{
	for (int i=1; i <= PageSize; i++)
	{
		int index = i*100;
		for (int j=1; j<7; j++)
		{
			m_MJPGList.SetUnitText(index +j, L"", FALSE);
			m_MJPGList.SetUnitIsShow(index+j, FALSE, FALSE);
			m_MJPGList.SetUnitIsDisable(index+j, TRUE);
			m_MJPGList.SetUnitColor(index+j,font_white,false);
		}
		
		m_MJPGList.SetUnitIsShow(index, FALSE, FALSE);
		m_MJPGList.SetUnitIsDownStatus(index,FALSE);
		
		m_MJPGList.SetUnitBitmap(index+7,L"",L"",false);
		m_MJPGList.SetUnitIsShow(index+7, FALSE, FALSE);
		m_MJPGList.SetUnitIsDisable(index+7, TRUE);
	}
	m_MJPGList.SetUnitIsShow(50,false,false);

	m_MJPGList.Invalidate();
}

void CContactInfoDlg::ClickedOneItem(int unitID,int item)
{	
	
	if (m_vClick[(m_iCurrentPage-1)*PageSize+item] == 0)//被点击了
	{		
		m_MJPGList.SetUnitIsDownStatus(unitID,true);		
		m_vClick[(m_iCurrentPage-1)*PageSize+item] = 1;
	}
	else
	{		
		m_MJPGList.SetUnitIsDownStatus(unitID,false);
		m_vClick[(m_iCurrentPage-1)*PageSize+item] = 0;
	}

	m_MJPGList.SetUnitIsShow(unitID,true,true);
	
}

void CContactInfoDlg::SeeOneItem(int item)
{
	int index = item/100 - 1;
	if ( !m_vCurrentResult[index]->played() )
	{
		for (int j = 1; j < 8 ;j++)
		{
			m_MJPGList.SetUnitColor(item+j,font_white,true);
			m_MJPGList.SetUnitIsDisable(item+j,true);
		}

		m_vCurrentResult[index]->played(true) ;
		m_vCurrentResult[index]->Update();

		::SendMessage(((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->
			m_pMainDlg->GetSafeHwnd(), WM_TELNOTIFY, 1, 2);

	}	
}

void CContactInfoDlg::DeleteItems()
{	
	int npos = 0;
	for (int i = 0 ; i < m_vClick.size() ;i++)
	{	
		if ( 1 == m_vClick[i])
		{	
			Data::ContactInfo::Remove("id = " + Util::StringOp::FromInt(m_vContactID[i]));
			
			npos++;
			((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetProcessPos(npos);
			
			if (!m_sListFilter.compare("type = 0"))//未接留言
			{
				std::vector< boost::shared_ptr<Data::ContactInfo > > result ;
				result = Data::ContactInfo::GetFromDatabase("id =" + Util::StringOp::FromInt(m_vContactID[i]));
				if (result.size())
				{
					if (!result[0]->played())
					{
						::SendMessage(((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pMainDlg->GetSafeHwnd(), WM_TELNOTIFY, 1, 2);
					}
				}
			}
			
		}
	}
	
	//删除容器里的数据
	std::vector<int>::iterator it;
	it = m_vClick.begin();	
	std::vector<int>::iterator itID;
	itID = m_vContactID.begin();
	for ( it ; it < m_vClick.end() ; it)
	{	
		if ( 1 == (*it))
		{
			m_vClick.erase(it);
			m_vContactID.erase(itID);
		}
		else
		{
			itID++ ;
			it++;
		}
	}	
		
	//容器的末尾值也去掉，保留为页的整数
	if ( m_vClick.size()%PageSize != 0)
	{
		it	 = m_vClick.end() - m_vClick.size()%PageSize ;
		itID = m_vContactID.end() - m_vContactID.size()%PageSize ;
		for (it ; it < m_vClick.end() ;it)
		{
			m_vClick.erase(it);
			m_vContactID.erase(itID);
		}
	}
	
	
	m_iTotalPages = Data::ContactInfo::GetDataCount(m_sListFilter)/PageSize;//总页数
	if (Data::ContactInfo::GetDataCount(m_sListFilter)%PageSize != 0)
	{
		m_iTotalPages +=1;
	}
	if (m_iCurrentPage > m_iTotalPages)
	{
		m_iCurrentPage = m_iTotalPages ;	
	}
	
	if ( 0 == m_iTotalPages)
	{
		m_iCurrentPage = 1;
	}
	
	ClearCurrentPage();
	FromContactInfoDataBase();
	ShowTypeInfo();
	
}

void CContactInfoDlg::SetPageFont()
{
	int items = 0 ;
	for (int i = 1 ;i <= PageSize;i++)
	{   
		items = i * 100 ;
		for (int j = 1 ;j < 7;j++)
		{
			m_MJPGList.SetUnitFont(items + j,font_18);
			m_MJPGList.SetUnitColor(items + j,font_white,false);
		}		
	}
	
	m_MJPGList.SetUnitFont(6,font_18);
	m_MJPGList.SetUnitColor(6,font_white,false);
	
	m_MJPGData_key.SetUnitFont(800,font_30);
	m_MJPGData_key.SetUnitColor(800,font_black,true);
	
	m_MJPGLetter_key.SetUnitFont(900,font_30);
	m_MJPGLetter_key.SetUnitColor(900,font_black,true);
		
}

void CContactInfoDlg::Clear()
{
	if (m_vClick.size() >0)// 该项是否被点击
	{
		m_vClick.clear() ;
	}
	
	if (m_vContactID.size() > 0)
	{
		m_vContactID.clear() ;	
	}
	
	if (m_vCurrentResult.size() > 0)
	{
		m_vCurrentResult.clear();
	}
	m_bSelectAll = false ;
	
	m_iCurrentPage = 1 ;
	m_iTotalPages  = 0 ;
	
	m_MJPGList.SetUnitIsDownStatus(50,false);


}

void CContactInfoDlg::SelectAll()
{	
	int unitID = 0 ;
	if (!m_bSelectAll)
	{	
		m_bSelectAll = true ;
		for (int i = 1 ; i <= m_vCurrentResult.size() ; i++)
		{	
			unitID = i * 100 ;
			m_MJPGList.SetUnitIsDownStatus(50,true);
			m_MJPGList.SetUnitIsDownStatus(unitID,true);		
			
		}
	}
	else
	{	
		m_bSelectAll = false ;
		for (int i = 1 ; i <= m_vCurrentResult.size() ; i++)
		{	
			unitID = i * 100 ;
			m_MJPGList.SetUnitIsDownStatus(50,false);
			m_MJPGList.SetUnitIsDownStatus(unitID,false);		
		
		}
	}
	
	//所有项全部全选
	for (int i = 0 ; i < m_vClick.size();i++)
	{
		if (m_bSelectAll)
		{
			m_vClick[i] = 1;		
		}
		else
		{
			m_vClick[i] = 0;
		}
	}
	
	m_MJPGList.Invalidate();
	
}

void CContactInfoDlg::SetUpPages()
{	
	CString page ;
	page.Format(L"%d/%d",m_iCurrentPage,m_iTotalPages);
	if (  0 == m_iTotalPages)
	{
		page.Format(L"%d/%d",0,m_iTotalPages);
	}
	m_MJPGList.SetUnitText(6,page,TRUE);//设置当前的页
}

void CContactInfoDlg::SetUnitStatus()
{
	int item  = 0;	
	for(int i = 0; i < m_vCurrentResult.size(); ++i)
	{	
		item = (i +1) * 100 ;
		if ( 1 == m_vClick[(m_iCurrentPage-1)*PageSize + i])
		{
			m_MJPGList.SetUnitIsDownStatus(item,true);
		}
		
		if (!m_vCurrentResult[i]->played())
		{
			for (int j = 1; j < 8 ;j++)
			{
				m_MJPGList.SetUnitColor(item+j,font_blue,false);
				m_MJPGList.SetUnitIsDisable(item+j,false);
			}
		}

		for (int j = 0 ; j < 8 ;j++)
		{
			m_MJPGList.SetUnitIsShow(item+j,true,false);
		}
		
		m_MJPGList.SetUnitIsDisable(item + 7 ,false);

	}
	
	if (m_vCurrentResult.size() > 0)
	{
		m_MJPGList.SetUnitIsShow(50,true,false);
	}
}

int CContactInfoDlg::GetVectorPages()
{
	int pages ;
	pages = m_vClick.size()/PageSize ;
	if ( m_vClick.size()%PageSize != 0)
	{
		pages += 1;
	}
	return pages ;
}

void CContactInfoDlg::ShowTypeInfo()
{	
	CString record = L".\\adv\\mjpg\\k5\\common\\png\\录音.bmp" ;
	CString temp ;
	int items	= 0  ;
	int row = 100;
	
	SetUnitStatus();
	SetUpPages();
	for (int i = 0; i < m_vCurrentResult.size(); ++i)
	{					
		boost::shared_ptr<Data::ContactInfo> result = 
			m_vCurrentResult[i];
		
		if (result->name() != "")//姓名
		{
			temp = result->name().c_str();
			m_MJPGList.SetUnitText(row+2, temp, FALSE);
		}
		if(result->telephoneNumber().number() != "")//电话与类别
		{
			temp = result->telephoneNumber().number().c_str();
			m_MJPGList.SetUnitText(row+3,temp,FALSE); 
			
			std::string groupName;
			groupName = GetType(Util::StringOp::FromCString(temp));
			m_MJPGList.SetUnitText(row+1, Util::StringOp::ToCString(groupName), 
				FALSE);
		}
		
		//wangzhenxing0925
		//if (!array[i]->played())
		if (result->isSound())
		{
			m_MJPGList.SetUnitIsDisable(row+7, FALSE);
			m_MJPGList.SetUnitBitmap(row+7, record, L"", FALSE);
		}
		else
		{
			m_MJPGList.SetUnitIsDisable(row+7, TRUE);
		}
		
		temp.Format(_T("%02d/%02d/%02d"), result->startTime().GetYear(),
			result->startTime().GetMonth(), result->startTime().GetDay());
		m_MJPGList.SetUnitText(row+5,temp,FALSE);
		temp.Format(_T("%02d:%02d"), result->startTime().GetHour(), result->startTime().GetMinute());
		m_MJPGList.SetUnitText(row+6,temp,FALSE);//通话开始时间
		
		//lxz 20090112
		if (result->duration() > 3600)
		{
			temp.Format(_T("%d:%02d:%02d"), result->duration() / 3600 ,
				result->duration() % 3600 / 60, result->duration() % 60);
		}
		else
		{
			temp.Format(_T("%d:%02d"), result->duration() / 60, 
				result->duration() % 60);
		}
		m_MJPGList.SetUnitText(row+4,temp,FALSE);//通话时长
		
		row += 100;
	}
	
}

void CContactInfoDlg::SetContactInfoFilter(int index)
{
	if (index == 0)
	{
		m_sListFilter = "type = 0";
	}
	else if (index == 1)
	{
		m_sListFilter = "type = 1";
	}
	else if (index == 2)
	{
		m_sListFilter = "type = 2";
	}
	else if (index == 3)
	{
		m_sListFilter = "";
	}	
}

void CContactInfoDlg::FromContactInfoDataBase()
{	
	if (m_vCurrentResult.size() > 0)
	{
		m_vCurrentResult.clear();
	}
	
	if (Data::ContactInfo::GetDataCount("") > 0)
	{
		m_iTotalPages = Data::ContactInfo::GetDataCount(m_sListFilter)/PageSize;//总页数
		if (Data::ContactInfo::GetDataCount(m_sListFilter)%PageSize != 0)
		{
			m_iTotalPages +=1;
		}
		
		m_vCurrentResult = Data::ContactInfo::GetFromDatabaseByTypeOffsetLength(m_sListFilter, (m_iCurrentPage-1)*PageSize+1, PageSize);
				
		//是否往容器里添加数据
		if (m_iCurrentPage > GetVectorPages())
		{
			for (int i = 0 ; i < m_vCurrentResult.size() ;i++)
			{
				if(m_bSelectAll)
				{
					m_vClick.push_back(1);
				}
				else
				{
					m_vClick.push_back(0);
				}
				m_vContactID.push_back(m_vCurrentResult[i]->id());
			}	
		}
	}
}

void CContactInfoDlg::ShowRightBtn(bool bshow)
{	
	for (int i = 0 ; i < 5;i++)
	{
		m_MJPGList.SetUnitIsShow(i+11,bshow,false);
	}
}

void CContactInfoDlg::SetType(int type)
{
	m_nType = type;
}

void CContactInfoDlg::GetNumName(std::vector<CString> &vname,std::vector<CString> &vnumber)
{
	for (int i = 0 ; i < m_vClick.size() ;i++)
	{
		if (1 == m_vClick[i])
		{	
			std::string filter = "id = " + Util::StringOp::FromInt(m_vContactID[i]);
			std::vector<boost::shared_ptr<Data::ContactInfo> > result = Data::ContactInfo::GetFromDatabase(filter);
			
			if (!result.empty())
			{
				std::string stelnum = result[0]->telephoneNumber().number();
				CString number = Util::StringOp::ToCString(stelnum);
				
				std::string sname = result[0]->name();
				CString name = Util::StringOp::ToCString(sname);
				if (!name.IsEmpty())
				{
					vname.push_back(name);
					vnumber.push_back(number);
				}
				else if(!number.IsEmpty())
				{
					vname.push_back(number);
					vnumber.push_back(number);
				}
				
			}
		}	
	}
}
void CContactInfoDlg::OnBtnOK()
{	
	CMultimediaPhoneDlg*main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	std::vector<CString> telNumber ;
	std::vector<CString> vname ;
	GetNumName(vname,telNumber);
	if (vname.size())
	{	
		if (1 == m_nType)
		{
			((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pMainDlg->m_p3GSMSDlg->SetSender(vname);
			((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pMainDlg->m_p3GSMSDlg->SetAppend(telNumber);
			((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pMainDlg->m_p3GSMSDlg->ShowWindow(TRUE);

		}
		else if( 2 == m_nType)
		{
			((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pMMSDlg->SetSender(vname);
			((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pMMSDlg->SetAppend(telNumber);
			((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pMMSDlg->ShowWindow(TRUE);
		}

	}
	
	if (m_nType > 0)
	{
		ShowWindow(SW_HIDE);			
		main->PopbackIcon();
		m_nType = 0;
	}

}

void CContactInfoDlg::ShowWindow_()
{
	Clear();
	ClearCurrentPage();
	FromContactInfoDataBase();
	SetUpBtn(3);
	ShowTypeInfo();
	ShowWindow(SW_SHOW);
}

void CContactInfoDlg::ResetTypeInfo()
{
	Clear();
	ClearCurrentPage();
	FromContactInfoDataBase();
	SetUpBtn(m_iTabs);
	ShowTypeInfo();
}

bool CContactInfoDlg::HaveClicked()
{
	bool enble = false ;	
	for (int i = 0 ; i < m_vClick.size() ;i++)
	{
		if (1 == m_vClick[i])
		{	
			enble = true ;
		}	
	}
	return enble ;
}