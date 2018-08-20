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
	, ContactInfoTotal(3000)
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
		SetPageTurnBtn();
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
		main->IsSendMessage(FALSE);
		break;

	case 1001: //确定
		OnBtnOK();
		main->IsSendMessage(FALSE);
		break;
		
	case 11://短信息
		if (OnBtnSMS())
		{
			CWnd *p = main->m_pMainDlg->m_p3GSMSDlg ;
			icon = Allicon[1];
			main->AddIcon(icon,p,false);
		}		
		break;

	case 12://拨打
		OnButtonDial();
		break;

	case 13://删除
	case 14://详情
	case 15://新建名片
		{
			m_operateType = w;
			BOOL flag1 = main->m_pSettingDlg->m_pSetting->isAdmin();
			if(flag1 && !main->m_pSettingDlg->m_bLogin)
			{
				main->m_pPasswordDlg->SettingType(CHECK_SUPPERPASSWORD);
				std::string strTemp = main->m_pSettingDlg->m_pSetting->adminPassword();
				main->m_pPasswordDlg->SetOldPassWord((char *)strTemp.c_str());
				main->m_pPasswordDlg->SetHWnd(this->m_hWnd);
				main->m_pPasswordDlg->ShowWindow_(SW_SHOW);
			}
			else
			{
				WindowProc(CHECK_SUPPERPASSWORD, 1, 0);
			}
		}
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

	m_MJPGList.Create(L"", WS_VISIBLE|WS_CHILD, CRect(0, 0, 800, 423), this,10086);
	m_MJPGList.SetCurrentLinkFile(".\\adv\\mjpg\\k5\\中文\\通话记录界面.xml");
	m_MJPGList.SetMJPGRect(CRect(0, 0, 800, 423));	

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


std::string CContactInfoDlg::GetName(std::string number, int &groupId)
{
	
	if (number == "" )
	{
		return "";
	}
	
	if ( number.length() > 6)
	{
		if ( 0 == (number.substr(0,2)).compare("86"))
		{
			number = number.substr(2);
		}
		else if ( 0 == (number.substr(0,3)).compare("+86"))
		{
			number = number.substr(3);
		}
	}

	std::string filter;
	if (number.length() < 7)
	{
		filter += "mobilesTelephone = '";
		filter += (number);
		filter += "'";
		filter += " OR mobilesTelephone2 = '";
		filter += (number);
		filter += "'";
		filter += " OR worksTelephone = '";
		filter += (number);
		filter += "'";
		filter += " OR worksTelephone2 = '";
		filter += (number);
		filter += "'";
	}
	else
	{
		filter += "mobilesTelephone LIKE '%";
		filter += (number);
		filter += "'";
		filter += " OR mobilesTelephone2 LIKE '%";
		filter += (number);
		filter += "'";
		filter += " OR worksTelephone LIKE '%";
		filter += (number);
		filter += "'";
		filter += " OR worksTelephone2 LIKE '%";
		filter += (number);
		filter += "'";
	}
	
	std::vector<boost::shared_ptr<Data::Contact> > result = Data::Contact::GetFromDatabaseByTypeOffsetLength(filter, 0, 1);
	if (!result.empty())
	{
		groupId = result[0]->groupId();
		return result[0]->name();
	}
	return "";
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
	else if ( 0 == num)
	{	
		CString title = L"请先选择记录!" ;
		main->m_pWarningNoFlashDlg->SetTitle(title);
		main->m_pWarningNoFlashDlg->ShowWindow_(SW_SHOW);
		return;
	}

	for (i = 0 ; i < m_vClick.size() ;i++)
	{
		if (1 == m_vClick[i])
		{	
			break ;
		}	
	}

	main->m_pMainDlg->StopVideo(TRUE, 0, 0);  //wangzhenxing20100709

	std::string filter = "id = " + Util::StringOp::FromInt(m_vContactID[i]);
	std::vector<boost::shared_ptr<Data::ContactInfo> > result = Data::ContactInfo::GetFromDatabase(filter);
	
	if (!result.empty() && result[0]->GetContact())
	{	
		main->m_phoneLine[0].pFSM->setStartState(CMultimediaPhoneDlg::p3gsHangOn);
		std::string number = result[0]->telephoneNumber().number();
		main->m_pTelephoneDlg->DialContact(number, result[0]->GetContact()->id());
	}
	else
	{	
		main->m_phoneLine[0].pFSM->setStartState(CMultimediaPhoneDlg::p3gsHangOn);
		std::string number = result[0]->telephoneNumber().number();
		main->m_pTelephoneDlg->DialContact(number);
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
		if (((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pSettingDlg->m_pSetting->isPlayProtect() && !((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pSettingDlg->m_bLogin)
		{
			m_pPasswordDlg->SettingType(CHECK_PLAYPASSWORD);
			std::string strTemp = ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pSettingDlg->m_pSetting->playRecordPassword();
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
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	if (!Warning())
	{
		return;
	}
	
	std::string filter;
	filter = "[groupId] <>";
	filter += Util::StringOp::FromInt(main->m_pContactDlg->GetSimID());

	if (Data::Contact::GetDataCount(filter) < main->m_pContactDlg->ContactTotal)
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
					//((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->SwitchPanel_(IDC_BUTTON_CONTACT);
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
				
				main->m_pContactNewDlg->SetContact(contact);
				main->m_pContactNewDlg->ShowWindow(TRUE);
			}
		}
		
	}
	else
	{	
		main->m_pWarningNoFlashDlg->SetTitle(L"新建名片已满!");
		main->m_pWarningNoFlashDlg->ShowWindow_(SW_SHOW);
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
	//	SetContactInfoFilter(3);
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
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
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
	
	if (m_bSelectAll)
	{
		enble = true;
	}
	else if ( 0 == count)
	{	
		CString title = L"请先选择记录!" ;
		main->m_pWarningNoFlashDlg->SetTitle(title);
		main->m_pWarningNoFlashDlg->ShowWindow_(SW_SHOW);
		return;
	}

	if (enble)
	{	
		CString title = L"";
		if (m_bSelectAll)
		{
			int allcount = Data::ContactInfo::GetDataCount(m_sListFilter);
			if (count == m_vClick.size() && count != 0)//全部
			{	
				count = allcount;
				title = L"是否删除全部信息?";
			}
			else if( count != m_vClick.size())//
			{
				count = allcount - (m_vClick.size() - count);
				if ( 0 == count)
				{
					return ;
				}	
				title.Format(L"%d", count);
				title = L"是否删除已选中的" + title + L"条信息?";
			}
			count = 1;
		}
		else
		{
			title.Format(L"%d", count);
			title = L"是否删除已选中的" + title + L"条记录?";
		}
		
		main->m_pDeleteTipDlg->SetTitle(title,0);
		main->m_pDeleteTipDlg->SetProcessMax(count);
		main->m_pDeleteTipDlg->SetHWnd(this->GetSafeHwnd());
		if (main->m_pSettingDlg->m_pSetting->isAdmin() && !main->m_pSettingDlg->m_bLogin)
		{
			main->m_pDeleteTipDlg->SetPasswordModel(true);
		}
		std::string pw = main->m_pSettingDlg->m_pSetting->adminPassword();
		main->m_pDeleteTipDlg->SetPassword(Util::StringOp::ToCString(pw));
		std::string strTemp = Data::LanguageResource::Get(Data::RI_DELETETIP_CONTACTINFO);
		main->m_pDeleteTipDlg->SetDelTip(strTemp.c_str());
		main->m_pDeleteTipDlg->ShowWindow_(TRUE);

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
	SetPageTurnBtn();
}

LRESULT CContactInfoDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	switch (message)
	{
	case WM_DELETESELITEM:
		if (m_bSelectAll)
		{
			DeleteAll();
		}
		else
		{
			DeleteItems();
		}
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
	case CHECK_SUPPERPASSWORD:
		if(1 == wParam)
		{
			main->m_pSettingDlg->m_bLogin = TRUE;
			switch(m_operateType)
			{
			case 13:
				OnButtonDelete();
				break;
			case 14:
			//	OnButtonSound();
				Detail();
				break;
			case 15:
				OnButtonNew();
				break;
			}
		}
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
	SetPageTurnBtn();
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

std::string CContactInfoDlg::GetType(int groupId)
{
	std::string filter;

	if(groupId >= 0)
	{
		filter = "id = " + Util::StringOp::FromInt(groupId);
		std::vector<boost::shared_ptr<Data::ContactGroup> > Groupresult = Data::ContactGroup::GetFromDatabase(filter);
	
		if (Groupresult.size() > 0)
		{
			return Groupresult[0]->name();
		}
	}

	return "";
}


bool CContactInfoDlg::OnBtnSMS()
{	
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	int num = 0 ;
	for (int i = 0 ; i < m_vClick.size() ;i++)
	{
		if (1 == m_vClick[i])
		{
			num++;
		}
	}

	if (!m_bSelectAll)
	{	
		if ( 0 == num)
		{
			CString title = L"请先选择记录!" ;
			main->m_pWarningNoFlashDlg->SetTitle(title);
			main->m_pWarningNoFlashDlg->ShowWindow_(SW_SHOW);
			return false;
		}
	}

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
	
	SetPageTurnBtn(1);

	ClearCurrentPage();
	FromContactInfoDataBase();
	ShowTypeInfo();		
}

void CContactInfoDlg::ClearCurrentPage()
{
	for (int i=1; i <= PageSize; i++)
	{
		int index = i*100;
		for (int j=1; j<8; j++)
		{
			m_MJPGList.SetUnitText(index +j, L"", FALSE);
			m_MJPGList.SetUnitIsShow(index+j, FALSE, FALSE);
			m_MJPGList.SetUnitIsDisable(index+j, TRUE);
			m_MJPGList.SetUnitColor(index+j,font_white,false);
		}
		
		m_MJPGList.SetUnitIsShow(index, FALSE, FALSE);
		m_MJPGList.SetUnitIsDownStatus(index,FALSE);
		
	//	m_MJPGList.SetUnitBitmap(index+7,L"",L"",false);
	//	m_MJPGList.SetUnitIsShow(index+7, FALSE, FALSE);
	//	m_MJPGList.SetUnitIsDisable(index+7, TRUE);

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
		
		//add by qi 0720
		int count = 0 ;
		if (m_bSelectAll)//判断是不是全选
		{
			for (int i = 0 ; i < m_vClick.size() ;i++)
			{
				if (1 == m_vClick[i])
				{	
					count++;
				}	
			}
			
			if (count == m_vClick.size() && count != 0)//全选了
			{
				m_MJPGList.SetUnitIsDownStatus(50,true);
				m_MJPGList.SetUnitIsShow(50,true,true);
			}
		}

	}
	else
	{		
		m_MJPGList.SetUnitIsDownStatus(unitID,false);
		m_vClick[(m_iCurrentPage-1)*PageSize+item] = 0;
		
		//add by qi 0720
		if (m_bSelectAll)
		{
			m_MJPGList.SetUnitIsDownStatus(50,false);
			m_MJPGList.SetUnitIsShow(50,true,true);	
		}
	}
	
	m_MJPGList.SetUnitIsShow(unitID,true,true);
	
}

void CContactInfoDlg::SeeOneItem(int item)
{
	int index = item/100 - 1;
	if ( !m_vCurrentResult[index]->played() )
	{
// 		for (int j = 1; j < 8 ;j++)
// 		{
// 			m_MJPGList.SetUnitColor(item+j,font_white,true);
// 			m_MJPGList.SetUnitIsDisable(item+j,true);
// 		}
		
		m_MJPGList.SetUnitIsDisable(item+7,true);
		m_MJPGList.SetUnitText(item+7,L"已读",true);

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
			
			Data::ContactInfo::Remove("id = " + Util::StringOp::FromInt(m_vContactID[i]));
			
			npos++;
			((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetProcessPos(npos);

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
	
	SetPageTurnBtn(1);

	ClearCurrentPage();

	//比较容器里的页数 和 当前所在的页
	int nCount = m_iCurrentPage;
	int pages ;
	pages = m_vClick.size()/PageSize ;
	if (pages >= m_iCurrentPage)//剩余的数据大于当前的页，直接找当前页的数据
	{
		FromContactInfoDataBase();
	}
	else
	{
		m_iCurrentPage = pages ;//
		for (;pages < nCount;pages++)//循环往容器里添数据
		{	
			m_iCurrentPage++;//
			FromContactInfoDataBase();
		}
	}

	ShowTypeInfo();
}

void CContactInfoDlg::DeleteAll()
{
	std::vector< boost::shared_ptr<Data::ContactInfo> > result ;
	result = Data::ContactInfo::GetFromDatabase(m_sListFilter);
		
	for (int i = 0 ; i < result.size() ; i++)
	{
		if ( i >= m_vClick.size())
		{
			result[i]->Remove();
		}
		else 
		{
			if (m_vClick[i] == 1)
			{
				result[i]->Remove();	
			}
		}
		
	}
		
	((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetProcessPos(1);
	
	m_iCurrentPage = 1;
	Clear();
	ClearCurrentPage();
	FromContactInfoDataBase();
	ShowTypeInfo();
	SetPageTurnBtn();

	::SendMessage(((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->
		m_pMainDlg->GetSafeHwnd(), WM_TELNOTIFY, 1,0);//清空

}

void CContactInfoDlg::SetPageFont()
{
	int items = 0 ;
	for (int i = 1 ;i <= PageSize;i++)
	{   
		items = i * 100 ;
		for (int j = 1 ;j < 8;j++)
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
			m_MJPGList.SetUnitIsDownStatus(unitID,true);		
			
		}
		m_MJPGList.SetUnitIsDownStatus(50,true);
	}
	else
	{	
		//add by qi 0720
		if (m_MJPGList.GetUnitIsDownStatus(50))//如果是按下状态
		{
			m_bSelectAll = false ;
			for (int i = 1 ; i <= m_vCurrentResult.size() ; i++)
			{	
				unitID = i * 100 ;
				m_MJPGList.SetUnitIsDownStatus(unitID,false);				
			}
			m_MJPGList.SetUnitIsDownStatus(50,false);			
		}
		else//不是按下状态还是全选
		{	
			m_bSelectAll = true ;
			for (int i = 1 ; i <= m_vCurrentResult.size() ; i++)
			{	
				unitID = i * 100 ;
				m_MJPGList.SetUnitIsDownStatus(unitID,true);		
				
			}
			m_MJPGList.SetUnitIsDownStatus(50,true);
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

void CContactInfoDlg::SetPageTurnBtn(int type)
{	
	if ( 0 == type)
	{
		int ncount = Data::ContactInfo::GetDataCount(m_sListFilter);
		if (ncount <= PageSize)//小于一页
		{
			m_MJPGList.SetUnitIsDisable(7,true);
			m_MJPGList.SetUnitIsDisable(8,true);
		}
		else //大于一页
		{
			m_MJPGList.SetUnitIsDisable(7,true);
			m_MJPGList.SetUnitIsDisable(8,false);
		}
	}
	else if( 1 == type)
	{
		//判断翻页按钮是否有效
		m_MJPGList.SetUnitIsDisable(7,false);
		m_MJPGList.SetUnitIsDisable(8,false);
		if (m_iCurrentPage == 1)
		{
			m_MJPGList.SetUnitIsDisable(7,true);
		}

		if (m_iCurrentPage == m_iTotalPages)
		{
			m_MJPGList.SetUnitIsDisable(8,true);
		}
	}
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
// 			for (int j = 1; j < 8 ;j++)
// 			{
// 				m_MJPGList.SetUnitColor(item+j,font_green,false);
// 				m_MJPGList.SetUnitIsDisable(item+j,false);
// 			}
			m_MJPGList.SetUnitText(item + 7,L"未读",false);
			m_MJPGList.SetUnitIsDisable(item + 7 ,false);

		}
		else
		{
			m_MJPGList.SetUnitText(item + 7,L"已读",false);
		}

		for (int j = 0 ; j < 8 ;j++)
		{
			m_MJPGList.SetUnitIsShow(item+j,true,false);
		}
		
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
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;

	CString record = L".\\adv\\mjpg\\k5\\common\\png\\录音.bmp" ;
	CString temp ;
	int items	= 0  ;
	int row = 100;
	int groupId = -1;
	
	SetUnitStatus();
	SetUpPages();
	ShowTypeCount();
	for (int i = 0; i < m_vCurrentResult.size(); ++i)
	{
		groupId = -1;
		boost::shared_ptr<Data::ContactInfo> result = 
			m_vCurrentResult[i];
		
		if (result->name() != "")//姓名
		{
			temp = result->name().c_str();
			m_MJPGList.SetUnitText(row+2, temp, FALSE);
		}

		if(result->telephoneNumber().number() != "")//电话与类别
		{	
			//姓名
			std::string name;
			name = GetName(result->telephoneNumber().number(), groupId);
			m_MJPGList.SetUnitText(row+2,Util::StringOp::ToCString(name),false);			
			//

			temp = result->telephoneNumber().number().c_str();
			m_MJPGList.SetUnitText(row+3,temp,FALSE); 
			
			std::string groupName;
			groupName = GetType(groupId);
			m_MJPGList.SetUnitText(row+1, Util::StringOp::ToCString(groupName), 
				FALSE);
		}
		
		//wangzhenxing0925
		//if (!array[i]->played())
	/*	if (result->isSound())
		{
			m_MJPGList.SetUnitIsDisable(row+7, FALSE);
			m_MJPGList.SetUnitBitmap(row+7, record, L"", FALSE);
		}
		else
		{
			m_MJPGList.SetUnitIsDisable(row+7, TRUE);
		}*/
		

 		temp.Format(_T("%02d-%02d-%02d"), result->startTime().GetYear(),
 			result->startTime().GetMonth(), result->startTime().GetDay());
		temp = temp.Mid(2);
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

void CContactInfoDlg::ShowTypeCount()
{
	int ncount = Data::ContactInfo::GetDataCount(m_sListFilter);
	CString d ;
	d.Format(L"%d",ncount);
	m_MJPGList.SetUnitText(10,d,false);
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
	if (m_bSelectAll)
	{
		std::vector< boost::shared_ptr<Data::ContactInfo> > result ;
		result = Data::ContactInfo::GetFromDatabase(m_sListFilter);
		
		for (int i = 0 ; i < result.size() ; i++)
		{
			bool b = false;
			if ( i >= m_vClick.size())
			{
				b = true;
			}
			else 
			{
				if (m_vClick[i] == 1)
				{
					b = true;		
				}
			}
			
			if (b)
			{
				Num2Name(result[i],vname,vnumber);	
			}
		}
	}
	else
	{
		for (int i = 0 ; i < m_vClick.size() ;i++)
		{
			if (1 == m_vClick[i])
			{	
				std::string filter = "id = " + Util::StringOp::FromInt(m_vContactID[i]);
				std::vector<boost::shared_ptr<Data::ContactInfo> > result = Data::ContactInfo::GetFromDatabase(filter);
				
				if (!result.empty())
				{	
					Num2Name(result[0],vname,vnumber);					
				}
			}	
		}
	}

}

void CContactInfoDlg::Num2Name(boost::shared_ptr<Data::ContactInfo > pcontactInfo,std::vector<CString> &vname,std::vector<CString> &vnumber)
{	
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	std::string stelnum = pcontactInfo->telephoneNumber().number();
	CString number = Util::StringOp::ToCString(stelnum);
	
	//std::string sname = pcontactInfo->name();
	std::string sname = main->GetName(stelnum) ;
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

void CContactInfoDlg::InsertNum2Phone()
{
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	
	int count = 0  ;
	int index;
	for (int i = 0 ; i < m_vClick.size(); i++)
	{
		if ( 1 == m_vClick[i])
		{
			count++;
			index = i ;
		}
	}
	
	if (count > 1)
	{
		main->m_pWarningNoFlashDlg->SetTitle(L"只能选择一个联系人");
		main->m_pWarningNoFlashDlg->ShowWindow_(SW_SHOW);
		return ;
	}
	
	if ( 1 == count)
	{			
		std::string filter = "id = " + Util::StringOp::FromInt(m_vContactID[index]);
		std::vector<boost::shared_ptr<Data::ContactInfo> > result = Data::ContactInfo::GetFromDatabase(filter);
		
		if (!result.empty())
		{
			std::string stelnum = result[0]->telephoneNumber().number();
			CString number = Util::StringOp::ToCString(stelnum);
			main->m_pTelphoneDialDlg->SetTel(number);
			
			ShowWindow(SW_HIDE);
			main->PopbackIcon();		
		}
		
	}

}

void CContactInfoDlg::OnBtnOK()
{	
	CMultimediaPhoneDlg*main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	
	if ( 1 == m_nType || 2 == m_nType)//短信或者彩信
	{
		std::vector<CString> telNumber ;
		std::vector<CString> vname ;
		GetNumName(vname,telNumber);
		if (vname.size())
		{	
			if (1 == m_nType)
			{
				main->m_pMainDlg->m_p3GSMSDlg->SetSender(vname);
				main->m_pMainDlg->m_p3GSMSDlg->SetAppend(telNumber);
				
			}
			else if( 2 == m_nType)
			{
				main->m_pMMSDlg->SetSender(vname);
				main->m_pMMSDlg->SetAppend(telNumber);
				
			}
			
		}
	}
	else if ( 3 == m_nType)//来至于电话
	{
		InsertNum2Phone();		
	}
	
	if (m_nType > 0)
	{
		ShowWindow(SW_HIDE);			
		main->PopbackIcon();
		m_nType = 0;
	}

}

void CContactInfoDlg::Detail()
{	
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;

	if (!Warning())
	{
		return;
	}

	int index = -1; 
	int count = 0 ;
	for (int i = 0 ; i < m_vClick.size() ;i++)
	{
		if (1 == m_vClick[i])
		{	
			index = i ;
			count++;
			break;
		}
		else
		{
			count++;
		}
	}
	
	boost::shared_ptr<Data::ContactInfo> pContactInfo = Data::ContactInfo::GetDataById(m_vContactID[index]);
	main->m_pSMSListDlg->m_pSmsDetailDlg->ShowContactinfoDetail(pContactInfo);
	main->m_pSMSListDlg->m_pSmsDetailDlg->ShowWindow_(SW_SHOW);

	if ( !pContactInfo->played())
	{
		int item = ((count%6 ==0)?6:count%6);
		item *=100 ;
		m_MJPGList.SetUnitText(item+7,L"已读",true);
		m_MJPGList.SetUnitIsDisable(item + 7 ,true);
		
		pContactInfo->played(true) ;
		pContactInfo->Update();
		
		::SendMessage(((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->
			m_pMainDlg->GetSafeHwnd(), WM_TELNOTIFY, 1, 2);
		
	}

}

bool CContactInfoDlg::Warning()
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
		CString title = L"不能选择多个联系人!" ;
		main->m_pWarningNoFlashDlg->SetTitle(title);
		main->m_pWarningNoFlashDlg->ShowWindow_(SW_SHOW);
		return false;
	}

	if ( 0 == num)
	{
		CString title = L"请先选择记录!" ;
		main->m_pWarningNoFlashDlg->SetTitle(title);
		main->m_pWarningNoFlashDlg->ShowWindow_(SW_SHOW);
		return false;
	}
	return true;
}

void CContactInfoDlg::ShowWindow_()
{
	Clear();
	ClearCurrentPage();
	m_sListFilter = "";
	ShowWindow(SW_SHOW);
	FromContactInfoDataBase();
	SetUpBtn(3);
	ShowTypeInfo();
	SetPageTurnBtn();

}

void CContactInfoDlg::ResetTypeInfo()
{
	Clear();
	ClearCurrentPage();
	FromContactInfoDataBase();
	SetUpBtn(m_iTabs);
	ShowTypeInfo();
}

