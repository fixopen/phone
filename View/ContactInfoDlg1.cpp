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

#define  IDC_EDIT_INPUT 1000

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
	m_offSet = 0;
	m_dataCount = 0;
	m_pageCount = 0;
	m_bSearch = FALSE;
	m_isEditClick = FALSE;
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
	ON_BN_CLICKED(IDC_BUTTON_CONTACTINFO_DIAL, OnButtonDial)
	ON_BN_CLICKED(IDC_BUTTON_CONTACTINFO_SOUND, OnButtonSound)
	ON_BN_CLICKED(IDC_BUTTON_CONTACTINFO_NOTE, OnButtonNote)
	ON_BN_CLICKED(IDC_BUTTON_CONTACTINFO_NEW, OnButtonNew)
	ON_BN_CLICKED(IDC_BUTTON_CONTACTINFO_SEARCH, OnButtonSearch)
	ON_BN_CLICKED(IDC_BUTTON_CONTACTINFO_DELETE, OnButtonDelete)
	ON_MESSAGE(WM_CLICKMJPG_TOAPP, OnClickMJPG)
	ON_MESSAGE(WM_LISTCTRL_CLICK, OnListCltrlClick)
//	ON_WM_KEYDOWN()
END_MESSAGE_MAP()


// void CContactInfoDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
// {
// 	if(nChar == 'U')
// 	{
// 		m_lsList.m_scollbar_.DoPageUp();
// 	}
// 	else if(nChar == 'D')
// 	{
// 		m_lsList.m_scollbar_.DoPageDown();
// 	}
// }

void CContactInfoDlg::OnListCltrlClick(WPARAM w, LPARAM l)
{
// 	LRESULT ret;
// 	if(w == IDC_LIST_CONTACTINFO_TYPE)
// 		OnClickListType(NULL, &ret);
// 	else if(w == IDC_LIST_CONTACTINFO_LIST)
// 		OnClickListList(NULL, &ret);
}

/////////////////////////////////////////////////////////////////////////////
// CContactInfoDlg message handlers
void CContactInfoDlg::OnClickMJPG(WPARAM w, LPARAM l)
{
	CString content(L"");
	int i;
	switch (w)
	{
	case 0:
	case 1:
	case 2:
	case 3:
		m_MJPGList.SetUnitIsDownStatus(50, FALSE);
		m_isSelectAll = FALSE;
		
		if (m_isEditClick)
		{
			m_rEditInput->ShowWindow(SW_HIDE);
			SipShowIM(SIPF_OFF);
			m_isEditClick = FALSE;
		}

		if(!m_ItemChecked.empty())
		{
			m_ItemChecked.clear();
		}
		m_uiType = w;
		ShowItemsInList(w);
		break;
	case 4:           //查找
// 		if (m_isEditClick)
// 		{
// 			CString number;
// 			m_rEditInput->GetWindowText(number);
// 			m_isEditClick = FALSE;
// 			m_rEditInput->ShowWindow(SW_HIDE);
// 			SipShowIM(SIPF_OFF);
// 			SearchPage(number);
// 		}
// 		else
// 		{
// 			m_uiType = 3;
// 			OnButtonSearch();
// 		}
		
		m_uiType = 3;
 		OnButtonSearch();
		break;
	case 5:                //输入页数
		if (m_currentPage > 0)
		{
			m_rEditInput->EnableWindow(TRUE);
			m_rEditInput->ShowWindow(SW_SHOW);
			m_isEditClick = TRUE;
		}
		else
		{
			m_rEditInput->EnableWindow(FALSE);
		}
		break;
	case 7:               //上移
		OnClickPageUP(m_currentPage);
		break;
	case 8:			     //下移
		OnClickPageDown(m_currentPage);
		break;
	case 9:            //返回
		ShowWindow(SW_HIDE);
		break;
	case 10:		  //确定
		break;		
	case 11:		  //短信息
		//OnButtonNote();
		OnBtnSMS();
		break;
	case 12:		  //拨打
		OnButtonDial();
		break;
	case 13:		  //删除
		OnButtonDelete();
		break;
	case 14:		  //听留言
		OnButtonSound();
		break;
	case 15:		//新建名片
		OnButtonNew();
		break;
	case 50:        //全选
		SelectAll();
		break;
	case 100:		//勾选
	case 200:
	case 300:
	case 400:
	case 500:
	case 600:
		if (m_UnitStatus[w/100-1])
		{
			m_offSet = w/100 -1;
			OnButtonCheck(w);
		}
		break;
// 	case 101:        //显示联系人信息
// 	case 102:
// 	case 103:
// 	case 201:
// 	case 202:
// 	case 203:
// 	case 301:
// 	case 302:
// 	case 303:
// 	case 401:
// 	case 402:
// 	case 403:
// 	case 501:
// 	case 502:
// 	case 503:
// 	case 601:
// 	case 602:
// 	case 603:
// 		break;
// 	case 104:		//显示通话时间
// 	case 105:
// 	case 106:
// 	case 204:
// 	case 205:
// 	case 206:
// 	case 304:
// 	case 305:
// 	case 306:
// 	case 404:
// 	case 405:
// 	case 406:
// 	case 504:
// 	case 505:
// 	case 506:
// 	case 604:
// 	case 605:
// 	case 606:
// 		break;
// 	case 107:           //录音
// 	case 207:
// 	case 307:
// 	case 407:
// 	case 507:
// 	case 607:
// 		break;
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
		Clear();
		ClearAllText();
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
		Clear();
		ClearAllText();
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
	
	m_rEditInput = new CCERectEdit();
	m_rEditInput->Create(WS_CHILD|WS_VISIBLE, CRect(461,77,32,16), this, IDC_EDIT_INPUT, 15);
	m_rEditInput->SetIsAutoInput(TRUE);
	m_rEditInput->MoveWindow(461, 77, 32, 16);
	m_rEditInput->ShowWindow(SW_HIDE);

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

	m_MJPGList.SetUnitFont(5, font_16);
	m_MJPGList.SetUnitFont(6, font_16);
	m_MJPGList.SetUnitColor(5,font_white,TRUE);
	m_MJPGList.SetUnitColor(6,font_white,TRUE);
	for (int i=1; i<=PageSize; i++)
	{
		int index = i*100;
		for (int j=1; j<7; j++)
		{
			m_MJPGList.SetUnitFont(index+j, font_16);
			m_MJPGList.SetUnitColor(index+j,font_white,TRUE);
		}
	}

	m_pPlaySoundDlg = new CPlaySoundDlg(this);
	m_pPlaySoundDlg->Create(CPlaySoundDlg::IDD);
	m_pPlaySoundDlg->SetModel(1);

	m_pSearchContactInfoDlg = new CContactInfoSearchDlg(this);
	m_pSearchContactInfoDlg->Create(CContactInfoSearchDlg::IDD);

	m_pPasswordDlg = new CPasswordDlg(this);
	m_pPasswordDlg->Create(CPasswordDlg::IDD);

	m_bTelephoneInUse = false;
	m_uiSelectIndex = 0;
	m_sListFilter = "";
	m_sListSearchFilter = "";

	m_isSelectAll = FALSE;
	m_uiType = 0;
	m_currentPage = 0;
	ShowItemsInList(m_uiType);	

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
//	ShowItemsInList();
	*pResult = 0;
}

void CContactInfoDlg::ShowItemsInList(int type)
{
	if (type == -1)
	{
		type = m_uiType;
	}
	else
	{
		m_uiType = type;
	}

	for (int i = 0; i < 4; ++i)
	{
		if (i == m_uiType)
		{
			m_MJPGList.SetUnitIsDownStatus(i, TRUE);
		}
		else
		{
			m_MJPGList.SetUnitIsDownStatus(i, FALSE);
		}
	}
	m_MJPGList.Invalidate();  

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
		filter = m_sAddtion;
	}

	if (Data::ContactInfo::GetDataCount("") > 0)
	{
		m_dataCount = Data::ContactInfo::GetDataCount(filter);
		m_vCurrentResult = Data::ContactInfo::GetFromDatabase(filter, Data::dDown, Data::ContactInfo::GetCurrentId() + 1, m_dataCount);
		if (0 == m_dataCount)
		{
			m_currentPage = 0;
		}
		else
		{
			m_currentPage = 1;
		}
		
		if (0 == m_dataCount%PageSize)
		{
			m_pageCount = m_dataCount/PageSize;
		}
		else
		{
			m_pageCount = m_dataCount/PageSize + 1;
		}
		
		CString str;
		str.Format(L"/%d",m_pageCount);
		m_MJPGList.SetUnitText(6, str, TRUE);
		str.Empty();
		str.Format(L"%d",m_currentPage);
		m_MJPGList.SetUnitText(5, str, TRUE);
		
		if (m_currentPage <= 1)   //当第一次显示时上翻按钮不可用
		{
			m_MJPGList.SetUnitIsDisable(7, TRUE);
		}
		else
		{
			m_MJPGList.SetUnitIsDisable(7, FALSE); 
		}
		if (m_dataCount <= PageSize)		//当总数小于6时设置下翻按钮不可用
		{
			m_MJPGList.SetUnitIsDisable(8, TRUE);
		}		
		else
		{
			m_MJPGList.SetUnitIsDisable(8, FALSE); 
		}
		
		for (int i=0; i<m_dataCount; i++)
		{
			m_ItemChecked.push_back(0); 
		}

		ShowArrayInList(m_vCurrentResult);
	}
}

void CContactInfoDlg::ShowArrayInList(std::vector<boost::shared_ptr<Data::ContactInfo> > array)
{
	ClearAllText();
	
	CString temp;
	int row = 100;
	if (m_dataCount > 0)
	{
		for (int i = (m_currentPage-1)*PageSize; i < m_currentPage*PageSize; ++i)
		{		
			if (array.begin() + i < array.end())
			{
				m_MJPGList.SetUnitIsDisable(row*100, FALSE);
				m_UnitStatus[row/100-1] = TRUE;
				
				if (1 == m_ItemChecked[i])
				{
					SetShowStatus(row, FALSE); 
				}
				else
				{
					SetShowStatus(row, TRUE); 
				}
				
				if (array[i]->name() != "")
				{
					temp = array[i]->name().c_str();
					m_MJPGList.SetUnitText(row+2, temp, FALSE);
				}
				if(array[i]->telephoneNumber().number() != "")
				{
					temp = array[i]->telephoneNumber().number().c_str();
					m_MJPGList.SetUnitText(row+3,temp,FALSE); 
					
					std::string groupName;
					groupName = GetType(Util::StringOp::FromCString(temp));
					m_MJPGList.SetUnitText(row+1, Util::StringOp::ToCString(groupName), FALSE);
				}
				
				//wangzhenxing0925
				//			if (!array[i]->played())
				if (array[i]->isSound())
				{
					m_MJPGList.SetUnitIsDisable(row+7, FALSE);
					m_MJPGList.SetUnitIsShow(row+7, TRUE, FALSE);
					m_MJPGList.SetUnitBitmap(row+7, L".\\adv\\mjpg\\k5\\common\\png\\录音.bmp", L"", FALSE);
				}
				else
				{
					m_MJPGList.SetUnitIsShow(row+7, FALSE, FALSE);
					m_MJPGList.SetUnitIsDisable(row+7, TRUE);
				}
				
				temp.Format(_T("%02d/%02d/%02d"), array[i]->startTime().GetYear(),
					array[i]->startTime().GetMonth(), array[i]->startTime().GetDay());
				m_MJPGList.SetUnitText(row+5,temp,FALSE);
				temp.Format(_T("%02d:%02d"), array[i]->startTime().GetHour(), array[i]->startTime().GetMinute());
				m_MJPGList.SetUnitText(row+6,temp,FALSE);
				
				//lxz 20090112
				if (array[i]->duration() > 3600)
				{
					temp.Format(_T("%d:%02d:%02d"), array[i]->duration() / 3600 ,array[i]->duration() % 3600 / 60, array[i]->duration() % 60);
				}
				else
				{
					temp.Format(_T("%d:%02d"), array[i]->duration() / 60, array[i]->duration() % 60);
				}
				m_MJPGList.SetUnitText(row+4,temp,FALSE);
				//wangzhenxing0925
				// if (array[i]->GetSoundsCount() > 0)
				// {
				// 		m_MJPGList.SetUnitIsDisable(14, FALSE);
				// }
				// 	else
				// 	{
				// 		m_MJPGList.SetUnitIsDisable(14, TRUE);
				// 	}
			}
			else
			{
				m_MJPGList.SetUnitIsDisable(row*100, TRUE);
				m_UnitStatus[row/100-1] = FALSE; 
			}
			
			row += 100;
		}
	}
	else
	{
		for (int i=1; i<=PageSize; i++)
		{
			SetShowStatus(i*100, TRUE);
			m_UnitStatus[i-1] = FALSE;
		} 
	}
	m_MJPGList.Invalidate();
	
	if (!array.empty())
	{
		std::string strTemp;
		CString str;
		
		if (m_vCurrentResult[m_uiSelectIndex]->contactId() > 0)
		{
			strTemp = Data::LanguageResource::Get(Data::RI_MAIN_CARDCASEBTN);
		}
		else
		{
			strTemp = Data::LanguageResource::Get(Data::RI_RECORD_NEWCARD);
		}
		str = strTemp.c_str();
		//lxz 20080623
//		m_btnNewContact.SetWindowText(str);
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

void CContactInfoDlg::OnButtonDial() 
{
	// TODO: Add your control notification handler code here
// 	POSITION pos = m_lsList.GetFirstSelectedItemPosition();
// 	if (pos != NULL)
// 	{
// 		int index = m_lsList.GetNextSelectedItem (pos); 
// 
// 		CString number = m_lsList.GetItemText(index, 0);
// 		if (number.IsEmpty())
// 		{
// 			return;
// 		}
// 		
// 		std::string str = Util::StringOp::FromCString(number);
// 		if (number.Right(1) != _T(")"))
// 		{
// 			if (str != "")
// 			{
// 				((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pTelephoneDlg->DialContact(str);
// 			}
// 		}
// 		else
// 		{
// 			int pos1 = str.find('(');
// 			int pos2 = str.find(')');
// 			if ((pos1 != std::string.npos) && (pos2 != std::string.npos))
// 			{
// 				std::string s = str.substr(pos1 + 1, pos2 - pos1 - 1);
// 				if (s != "")
// 				{
// 					std::string filter;
// 					filter = "id = " + Util::StringOp::FromInt(m_lsList.GetItemData(index));
// 					std::vector<boost::shared_ptr<Data::ContactInfo> > result = Data::ContactInfo::GetFromDatabase(filter);
// 					if (!result.empty() && result[0]->GetContact())
// 					{
// 						((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pTelephoneDlg->DialContact(s, result[0]->GetContact()->id());
// 					}
// 					else
// 					{
// 						((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pTelephoneDlg->DialContact(s);
// 					}
// 				}
// 			}
// 		}
// 	}

	//wangzhenxing0925
	CString number;
	for (int i=0; i<m_vCurrentResult.size(); i++)
	{
		int index = (m_currentPage-1)*PageSize+i;
		if (m_ItemChecked[index] == 1)
		{
			number = m_MJPGList.GetUnitText((i+1)*100+3);
			break;
		}
	}

	if (number.IsEmpty())
	{
		return;
	}
	std::string str = Util::StringOp::FromCString(number);
	std::string filter;
	filter = "id = " + Util::StringOp::FromInt(m_vCurrentResult[(m_currentPage-1)*PageSize+i]->id());
	std::vector<boost::shared_ptr<Data::ContactInfo> > result = Data::ContactInfo::GetFromDatabase(filter);
	if (!result.empty() && result[0]->GetContact())
	{
		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pTelephoneDlg->DialContact(str, result[0]->GetContact()->id());
	}
	else
	{
	 	((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pTelephoneDlg->DialContact(str);
	}
}

void CContactInfoDlg::OnButtonSound() 
{
	// TODO: Add your control notification handler code here

// 	POSITION pos = m_lsList.GetFirstSelectedItemPosition();
// 	if (pos != NULL)
// 	{
//  		int index = m_lsList.GetNextSelectedItem (pos);   	
// 
// 		if (m_vCurrentResult[index]->GetSoundsCount() > 0)
// 		{
// 			//pos = m_lsType.GetFirstSelectedItemPosition();
 			//m_pPlaySoundDlg->SetSound(m_lsType.GetNextSelectedItem(pos), m_lsList.GetItemData(index), 0, m_sListSearchFilter);
// 			m_pPlaySoundDlg->SetSound(m_uiType, m_lsList.GetItemData(index), 0, m_sListSearchFilter);
// 			if (((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pSettingDlg->m_pSetting->isPlayProtect() && !((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pSettingDlg->m_bLogin)
// 			{
// 				m_pPasswordDlg->SetType(CHECK_PLAYPASSWORD);
// 				std::string strTemp = ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pSettingDlg->m_pSetting->playRecordPassword();
// 				m_pPasswordDlg->SetOldPassWord((char *)strTemp.c_str());
// 				m_pPasswordDlg->SetHWnd(this->m_hWnd);
// 				m_pPasswordDlg->ShowWindow_(SW_SHOW);	
// 			}
// 			else
// 			{
// 				m_pPlaySoundDlg->StartPlay(false);
//				m_pPlaySoundDlg->ShowWindow_(TRUE);
// 			}
// 		}
// 	}

	//wangzhenxing0925
	for (int i=0; i<m_vCurrentResult.size(); i++)
	{
		int index = (m_currentPage-1)*PageSize+i;
		if (m_ItemChecked[index] == 1)
		{
		//	if (m_vCurrentResult[i]->GetSoundsCount() > 0)
			{
				m_pPlaySoundDlg->SetSound(m_uiType, m_vCurrentResult[index]->id(), 0, m_sListSearchFilter);
				break;
			}
		}
	}
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

void CContactInfoDlg::OnButtonNote() 
{

}

void CContactInfoDlg::OnButtonNew() 
{
	// change by qi 2009_09_30 down
	if (Data::Contact::GetDataCount("") < ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactDlg->ContactTotal)
	{	
		if ( 0 == m_currentPage )
		{
			return ;
		}
		//判断第一个被点击
		int index = -1 ;
		int icurrent ;//当前页的行数	
		icurrent = m_ItemChecked.size() - (m_currentPage-1)*PageSize ;
		if (icurrent > PageSize)
		{
			icurrent = PageSize ;	
		}

		for (int i = 0 ;i <	icurrent ;i++)
		{
			if (1 == m_ItemChecked[(m_currentPage-1)*PageSize + i])
			{	
				index = i;
				break ;	
			}
		}

		
		if (index > -1)
		{	
			//更新
			if (m_vCurrentResult[(m_currentPage-1)*PageSize + index]->contactId() > 0)
			{
				std::string filter;
				filter = "id = " + Util::StringOp::FromInt(m_vCurrentResult[(m_currentPage-1)*PageSize + index]->contactId());
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
				std::string stelnum = m_vCurrentResult[(m_currentPage-1)*PageSize+index]->telephoneNumber().number();
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
	// TODO: Add your control notification handler code here
	if (!m_bSearch)
	{
		m_MJPGLetter_key.ShowWindow(SW_SHOW);
		Invalidate();
		m_bSearch = true ;
	}
	else
	{
		m_MJPGLetter_key.ShowWindow(SW_HIDE);
		m_MJPGData_key.ShowWindow(SW_HIDE);
		m_bSearch = false ;
		m_sAddtion.empty();
		m_MJPGData_key.SetUnitText(800, L"", true);
		m_MJPGLetter_key.SetUnitText(900, L"", true);
	}
}

void CContactInfoDlg::OnButtonDelete() 
{
	// TODO: Add your control notification handler code here
//	POSITION pos = m_lsList.GetFirstSelectedItemPosition();
//	if (pos != NULL)
	for(int i=0; i<PageSize; i++)
	{
		if (1 == m_ItemChecked[(m_currentPage-1)*PageSize+i])
		{
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
}

void CContactInfoDlg::DeleteSelectedItem(void)
{
	size_t pageStartIndex = (m_currentPage - 1) * PageSize;
	std::vector<int>::iterator checkItem = m_ItemChecked.begin() + pageStartIndex;
	std::vector<boost::shared_ptr<Data::ContactInfo> >::iterator dataErase = m_vCurrentResult.begin() + pageStartIndex;
	for (;(checkItem != m_ItemChecked.end())
		&& (checkItem < (m_ItemChecked.begin() + m_currentPage * PageSize));)
	{
		if (*checkItem == 1)
		{
			(*dataErase)->Remove();
			checkItem = m_ItemChecked.erase(checkItem);
			m_vCurrentResult.erase(dataErase);
		}
		else
		{
			++checkItem;
			++dataErase;
		}
	}
	if ((m_currentPage - 1) * PageSize >= m_vCurrentResult.size()) {
		--m_currentPage;
	}

	CString str;
	str.Format(L"%d",m_currentPage);
	m_MJPGList.SetUnitText(5, str, TRUE);
	
	for (int i=1; i<=PageSize; i++)
	{
		SetShowStatus(i*100, TRUE);
	}
	m_MJPGList.Invalidate();
	ShowArrayInList(m_vCurrentResult);

	//从新求当前总记录数
	m_dataCount = m_vCurrentResult.size();
	if (0 == m_dataCount)
	{
		m_MJPGList.SetUnitText(6, "0", TRUE);
		m_MJPGList.SetUnitIsDisable(8, TRUE);
	}
	else
	{
		if (0 == m_dataCount%PageSize)
		{
			m_pageCount = m_dataCount/PageSize;
		}
		else
		{
			m_pageCount = m_dataCount/PageSize + 1;
		}
		CString str;
		str.Format(L"/%d", m_pageCount);
		m_MJPGList.SetUnitText(6, str, TRUE);
		if (m_dataCount <= PageSize)
		{
			m_MJPGList.SetUnitIsDisable(8, TRUE);
		}
	}
	//检查当前页的情况
	if (m_currentPage == m_pageCount)
	{
		m_MJPGList.SetUnitIsDisable(8, TRUE);
	}
	else
	{
		m_MJPGList.SetUnitIsDisable(8, FALSE);
	}
	if (m_currentPage == 1)
	{
		m_MJPGList.SetUnitIsDisable(7, TRUE);
	}
	else
	{
		m_MJPGList.SetUnitIsDisable(7, FALSE);
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

	if (ci->type() == Data::citInNo)
	{
		// 		POSITION pos = m_lsType.GetFirstSelectedItemPosition();   
		// 		while (pos != NULL)
		// 		{   
		// 			int iSel = m_lsType.GetNextSelectedItem(pos);   
		// 			m_lsType.SetItemState(iSel, 0, LVIS_SELECTED);   
		// 		}
		// 		m_lsType.SetItemState(0, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	}
	m_uiSelectIndex = 0;
	ShowItemsInList(0);
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
// 	POSITION pos = m_lsType.GetFirstSelectedItemPosition();   
// 	while (pos != NULL)
// 	{   
// 		int iSel = m_lsType.GetNextSelectedItem(pos);   
// 		m_lsType.SetItemState(iSel, 0, LVIS_SELECTED);   
// 	}
//     m_lsType.SetItemState(m_lsType.GetItemCount() - 1, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	ShowItemsInList(3);
}

void CContactInfoDlg::ShowUnconnectItems(void)
{
 	((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->SwitchPanel_(IDC_BUTTON_CONTACTINFO);
// 	POSITION pos = m_lsType.GetFirstSelectedItemPosition();   
// 	while (pos != NULL)
// 	{   
// 		int iSel = m_lsType.GetNextSelectedItem(pos);   
// 		m_lsType.SetItemState(iSel, 0, LVIS_SELECTED);   
// 	}
//     m_lsType.SetItemState(0, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
		
	ShowItemsInList(0);
}

LRESULT CContactInfoDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class	
	switch (message)
	{
	case WM_DELETESELITEM:
		DeleteSelectedItem();
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

void CContactInfoDlg::OnButtonCheck(int UnitNO)
{
	if (m_ItemChecked.size() > 0)
	{
		if (m_ItemChecked[(m_currentPage-1)*PageSize + m_offSet] == 0)
		{
			m_ItemChecked[(m_currentPage-1)*PageSize + m_offSet] = 1;
			SetShowStatus(UnitNO, FALSE);
		}
		else
		{
			m_ItemChecked[(m_currentPage-1)*PageSize + m_offSet] = 0;
			SetShowStatus(UnitNO, TRUE);
		}
	}
}

void CContactInfoDlg::SelectAll()
{
	if (!m_isSelectAll)
	{
		m_isSelectAll = TRUE;
		m_MJPGList.SetUnitIsDownStatus(50, TRUE);
		for (int i = 0; i <= m_dataCount; i++)
		{
			m_ItemChecked[i] = 1;
		}
	}
	else
	{
		m_isSelectAll = FALSE;
		m_MJPGList.SetUnitIsDownStatus(50, FALSE);
		for (int i = 0; i <= m_dataCount; i++)
		{
			m_ItemChecked[i] = 0;
		}
	}
	ShowArrayInList(m_vCurrentResult);
	m_MJPGList.Invalidate();
}

void CContactInfoDlg::OnClickPageUP(int currentPage)
{
	if (m_isSelectAll)
	{
		m_MJPGList.SetUnitIsDownStatus(50, TRUE);
	}
	
	m_currentPage--;
	if (m_currentPage <= 1)
	{
		m_currentPage = 1;
		m_MJPGList.SetUnitIsDisable(7, TRUE);
	}
	if (m_currentPage < m_pageCount)
	{
		m_MJPGList.SetUnitIsDisable(8, FALSE);
	}

	CString str;
	str.Format(L"%d", m_currentPage);
	m_MJPGList.SetUnitText(5, str, TRUE);
	str.Empty();
	
	ShowArrayInList(m_vCurrentResult);
}

void CContactInfoDlg::OnClickPageDown(int currentPage)
{
	if (m_isSelectAll)
	{
		m_MJPGList.SetUnitIsDownStatus(50, TRUE);
	}
	
	m_currentPage++;
	
	if (m_currentPage > 1)
	{
		m_MJPGList.SetUnitIsDisable(7, FALSE);
	}
	if (m_currentPage >= m_pageCount)  
	{
		m_MJPGList.SetUnitIsDisable(8, TRUE);
		//将最后一页没有记录的置为前景图
		int row = 100;
		for (int i = m_dataCount; i < m_currentPage*PageSize; i--)
		{
			m_MJPGList.SetUnitBitmap(row+8,L".\\ADV\\MJPG\\k5\\common\\png\\通话记录信息条.PNG",L"",FALSE);
			m_MJPGList.SetUnitIsDownStatus(row, FALSE);
			m_MJPGList.SetUnitBitmap(row, L".adv\\mjpg\\k5\\common\\png\\选择框.PNG", L".adv\\mjpg\\k5\\common\\png\\勾选.PNG", FALSE);
			for (int j=1; j<7; j++)
			{
				m_MJPGList.SetUnitIsDownStatus(row+j, FALSE);
			}
			
			row += 100;
		}
		m_MJPGList.Invalidate();
	}

	CString str;
	str.Format(L"%d", m_currentPage);
	m_MJPGList.SetUnitText(5, str, TRUE);
	str.Empty();

	ShowArrayInList(m_vCurrentResult);
}

void CContactInfoDlg::ClearAllText()
{
	for (int i=1; i<=PageSize; i++)
	{
		int index = i*100;
		for (int j=1; j<7; j++)
		{
			m_MJPGList.SetUnitText(index+j, L"", FALSE);
		}
		m_MJPGList.SetUnitIsShow(index+7, FALSE, FALSE);
		m_MJPGList.SetUnitIsDisable(index+7, TRUE);
	}
	m_MJPGList.Invalidate();
}

void CContactInfoDlg::SetShowStatus(int uintNO, BOOL status)
{
	if (m_UnitStatus[uintNO/100-1])
	{
		if (status)  //设置为前景
		{
			m_MJPGList.SetUnitBitmap(uintNO+8,L".\\ADV\\MJPG\\k5\\common\\png\\通话记录信息条.PNG",L"",FALSE);
			m_MJPGList.SetUnitIsDownStatus(uintNO, FALSE);
			m_MJPGList.SetUnitBitmap(uintNO, L".\\adv\\mjpg\\k5\\common\\png\\选择框.PNG", L".\\adv\\mjpg\\k5\\common\\png\\勾选.PNG", FALSE);
			
			for (int i=uintNO+1; i<uintNO+7; i++)
			{
				m_MJPGList.SetUnitIsDownStatus(i, FALSE);
				if (!m_MJPGList.GetUnitText(i).IsEmpty())
				{
					CString strText;
					strText = m_MJPGList.GetUnitText(i);
					m_MJPGList.SetUnitText(i, strText, FALSE);
					strText.Empty();
				}
			}
		}
		else    //设置为后景
		{
			m_MJPGList.SetUnitBitmap(uintNO+8,L".\\ADV\\MJPG\\k5\\common\\png\\通话记录信息条1.PNG",L"",FALSE);
			m_MJPGList.SetUnitIsDownStatus(uintNO, TRUE);
			m_MJPGList.SetUnitBitmap(uintNO, L".\\adv\\mjpg\\k5\\common\\png\\选择框.PNG", L".\\adv\\mjpg\\k5\\common\\png\\勾选.PNG", FALSE); 
			
			for (int i=uintNO+1; i<uintNO+7; i++)
			{
				m_MJPGList.SetUnitIsDownStatus(i, TRUE);
				if (!m_MJPGList.GetUnitText(i).IsEmpty())
				{
					CString strText;
					strText = m_MJPGList.GetUnitText(i);
					m_MJPGList.SetUnitText(i, strText, FALSE);
					strText.Empty();
				}
			}
		}
		
		m_MJPGList.Invalidate();
	}
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
	
	if (content.IsEmpty())
	{
		Clear();
		ClearAllText();
		return;
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
	Clear();
	ClearAllText();
	m_uiType = 
	ShowItemsInList(m_uiType);
}

void CContactInfoDlg::Clear()
{
	if (m_ItemChecked.size() > 0)
	{
		m_ItemChecked.clear();
	}
	
	m_currentPage = 0;
	m_dataCount = 0;
	m_isSelectAll = FALSE;
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

void CContactInfoDlg::SearchPage(CString pagenumber)
{
	int page = atoi(pagenumber);
	if ((page > 0) && (page <= m_pageCount))
	{
		m_currentPage = page;
		CString str;
		str.Format(L"%d", m_currentPage);
		m_MJPGList.SetUnitText(5, str, TRUE);

		if (m_currentPage == m_pageCount)
		{
			m_MJPGList.SetUnitIsDisable(8, TRUE);
		}
		else
		{
			m_MJPGList.SetUnitIsDisable(8, FALSE);
		}
		if (m_currentPage == 1)
		{
			m_MJPGList.SetUnitIsDisable(7, TRUE);
		}
		else
		{
			m_MJPGList.SetUnitIsDisable(7, FALSE);
		}

		ShowArrayInList(m_vCurrentResult);
	}
}

void CContactInfoDlg::OnBtnSMS()
{	
	if ( 0 == m_currentPage )
	{
		return ;
	}
	//判断第一个被点击
	int index = -1 ;
	int icurrent ;//当前页的行数	
	icurrent = m_ItemChecked.size() - (m_currentPage-1)*PageSize ;
	if (icurrent > PageSize)
	{
		icurrent = PageSize ;	
	}
	
	for (int i = 0 ;i <	icurrent ;i++)
	{
		if (1 == m_ItemChecked[(m_currentPage-1)*PageSize + i])
		{	
			index = i;
			break ;	
		}
	}
	
	//
	if (index > -1)
	{	
		std::string stelnum = m_vCurrentResult[(m_currentPage-1)*PageSize+index]->telephoneNumber().number();
		CString number = Util::StringOp::ToCString(stelnum);
		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pMainDlg->m_p3GSMSDlg->SetSender(number);
		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pMainDlg->m_p3GSMSDlg->ShowWindow(TRUE);
		ShowWindow(SW_HIDE);
	}

}