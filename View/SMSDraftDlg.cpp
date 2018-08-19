// YHTelDlg.cpp : implementation file
//

#include "stdafx.h"
#include "../multimediaphone.h"
#include "../MultimediaPhoneDlg.h"
#include "SMSDraftDlg.h"
#include "3GSMSDlg.h"
#include "3GSMSDetailDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// C3GSMSListDlg dialog

#define PAGE_COUNT  6
CDraftDlg::CDraftDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDraftDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(C3GSMSListDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_iTurnOverPages = 0 ;
	m_bSelectAll = false ;
	m_nPageSize = PAGE_COUNT;
	m_strHomeRecordeTelCode = ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pSetting->speCode3_.c_str();
}


void CDraftDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(C3GSMSListDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDraftDlg, CDialog)
	//{{AFX_MSG_MAP(C3GSMSListDlg)
	ON_MESSAGE(WM_CLICKMJPG_TOAPP, OnClickMJPG)
	ON_MESSAGE(WM_MJPGTOGGLE, OnClickMJPG)
	ON_MESSAGE(WM_LISTCTRL_CLICKSUBITEM, OnListCltrlClick)
//	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// C3GSMSListDlg message handlers
/*
void C3GSMSListDlg::OnTimer(UINT nIDEvent)
{
	if(IsWindowVisible())
	{
	if(nIDEvent == 1)
	{
			
		}
	}
	CDialog::OnTimer(nIDEvent);
}
*/


BOOL CDraftDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_MJPGList.Create(L"", WS_VISIBLE|WS_CHILD, CRect(0, 0, 800, 423), this);
	m_MJPGList.SetCurrentLinkFile(".\\adv\\mjpg\\k5\\中文\\草稿箱.xml");
	m_MJPGList.SetMJPGRect(CRect(0, 0, 800, 423));
	
	m_MJPGList.SetUnitIsDownStatus(4,true);

	MoveWindow(0,57,800,423);
	
	SetPagefont();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDraftDlg::OnClickMJPG(WPARAM w, LPARAM l)
{
	CMultimediaPhoneDlg *main = ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd));
	CString icon ;
	switch(w)
	{
	case 0://写消息	
		main->m_pMainDlg->m_p3GSMSDlg->ShowWindow(SW_SHOW);
		ShowWindow(SW_HIDE);
		break;

	case 1://写彩信息			
		main->m_pMMSDlg->ShowWindow(SW_SHOW);
		ShowWindow(SW_HIDE);
		break ;

	case 2://收件箱	
		main->m_pSMSListDlg->ShowWindow_(RECV_TYPE,SMS_TYPE);
		ShowWindow(SW_HIDE);
		break;

	case 3://发件箱	
		main->m_pSMSListDlg->ShowWindow_(SEND_TYPE,SMS_TYPE);
		ShowWindow(SW_HIDE);
		break;

	case 4://草稿箱

		break;
	case 5://设置
		main->m_pSmsSettingDlg->ShowWindow(SW_SHOW);
		ShowWindow(SW_HIDE);
		break;
	
	case 10://删除
		ShowDeleteDlg();
		break;
		
	case 11://编辑
		Edit();
		main->AddIcon(Allicon[1]);
		break;

	case 12://转发
		Transit();
		break;

	case 13://详情
		Details();
		main->AddIcon(Allicon[1]);
		break;

	case 14://重发
		Repeat();
		break;

	case 30://列表第1行
	case 40://列表第2行
	case 50://列表第3行
	case 60://列表第4行
	case 70://列表第5行
	case 80://列表第6行
		ClickedOneItem(w,w/10 - 3);
		break;

	case 31://点击某行
	case 32:
	case 33:

	case 41:
	case 42:
	case 43:

	case 51:
	case 52:
	case 53:
		
	case 61:
	case 62:
	case 63:

	case 71:
	case 72:
	case 73:
		
	case 81:
	case 82:
	case 83:
		ReadOneItem(w/10-3);
		main->AddIcon(Allicon[1]);
		break;

	case 100://短消息
		ShowSMS(SMS_TYPE);
		break;

	case 101://彩信息
		ShowSMS(MMS_TYPE);
		break;
	
	case 110://页数显示
		break;

	case 120://上一页
		PageSwitch(up_page);
		break;

	case 121://下一页
		PageSwitch(down_page);
		break;

	case 140://全选
		SelectAll();
		break;

	case 1000:
		ShowWindow(SW_HIDE);
		main->PopbackIcon();		
		break;

	case 1001:
		ShowWindow(SW_HIDE);		
		main->PopbackIcon();				
		break;	

	default:
		break;
	}
}


void CDraftDlg::FromDataBase()
{
	
	if(m_nSMSType == MMS_TYPE)
		m_sListFilter = "[type] = " + Util::StringOp::FromInt(m_nBoxType);
	else
		m_sListFilter = "[group] = " + Util::StringOp::FromInt(m_nBoxType);
		
	if(m_nSMSType == MMS_TYPE)//彩信
	{
		if (Data::MMSData::GetDataCount("") > 0)
		{
			m_iTotalPages = Data::MMSData::GetDataCount(m_sListFilter)/m_nPageSize;//总页数
			if (Data::MMSData::GetDataCount(m_sListFilter)%m_nPageSize != 0)
			{
				m_iTotalPages +=1;
			}
			
			m_vMMSDataCurrentResult = Data::MMSData::GetFromDatabaseByTypeOffsetLength(m_sListFilter, (m_iCurrentPage-1)*m_nPageSize+1, m_nPageSize);
			
			//是否往容器里添加数据
			if (m_iCurrentPage > GetVectorPages())
			{
				for (int i = 0 ; i < m_vMMSDataCurrentResult.size() ;i++)
				{
					if(m_bSelectAll)
					{
						m_vClick.push_back(1);
					}
					else
					{
						m_vClick.push_back(0);
					}
					m_vID.push_back(m_vMMSDataCurrentResult[i]->id());
				}
				
			}
		}
		else
		{	
			m_vMMSDataCurrentResult.clear();
		}
	}
	else//短信及家庭留言
	{
		if (Data::Message::GetDataCount("") > 0)
		{
			m_iTotalPages = Data::Message::GetDataCount(m_sListFilter)/m_nPageSize;//总页数
			if (Data::Message::GetDataCount(m_sListFilter)%m_nPageSize != 0)
			{
				m_iTotalPages +=1;
			}
			
			m_vMessageCurrentResult = Data::Message::GetFromDatabaseByTypeOffsetLength(m_sListFilter, (m_iCurrentPage-1)*m_nPageSize+1, m_nPageSize);
			
			//是否往容器里添加数据
			if (m_iCurrentPage > GetVectorPages())
			{
				for (int i = 0 ; i < m_vMessageCurrentResult.size() ;i++)
				{
					if(m_bSelectAll)
					{
						m_vClick.push_back(1);
					}
					else
					{
						m_vClick.push_back(0);
					}
					m_vID.push_back(m_vMessageCurrentResult[i]->id());
				}
				
			}
		}
		else
		{
			m_vMessageCurrentResult.clear();
		}
	}

}

void CDraftDlg::ShowArrayInList()
{	
	int icurrentPageItems = GetCurrentItems();//当前页有几行
	int ibeginID = 30 ;
	int addItems = 0;
	CString temp;
	
	SetUnitStatus();
	SetUpPages();
	if(m_nSMSType == MMS_TYPE)	//彩信
	{
		for (int i = 0; i < icurrentPageItems; ++i)
		{				
			temp = Util::StringOp::ToCString(m_vMMSDataCurrentResult[i]->RecipientAddress);			
		
			m_MJPGList.SetUnitText(ibeginID+addItems+1,temp,false);//收件人或者发件人

			temp = Util::StringOp::ToCString(m_vMMSDataCurrentResult[i]->Subject);
			m_MJPGList.SetUnitText(ibeginID+addItems+2,temp,false);//内容
			
			CTime tm = CTime(1970, 1, 1, 0, 0, 0);
			tm += CTimeSpan(0, 0, 0, m_vMMSDataCurrentResult[i]->DateAndTime);
			temp = Util::StringOp::FromTimestamp(tm).c_str();
			m_MJPGList.SetUnitText(ibeginID+addItems+3,temp,false);//时间

			addItems +=10;
		}

	}
	else //短信
	{
		for (int i = 0; i < icurrentPageItems; ++i)
		{	
			temp = Util::StringOp::ToCString(m_vMessageCurrentResult[i]->remote.address);//在发件箱里，表示是否发送成功。收件箱里表示是否阅读
						
			m_MJPGList.SetUnitText(ibeginID+addItems+1,temp,false);//收件人或者发件人

			temp = Util::StringOp::ToCString(m_vMessageCurrentResult[i]->unicodeData);
			m_MJPGList.SetUnitText(ibeginID+addItems+2,temp,false);//内容

			temp = Util::StringOp::ToCString(m_vMessageCurrentResult[i]->timestamp.ToString_());
			m_MJPGList.SetUnitText(ibeginID+addItems+3,temp,false);//时间
			
			addItems +=10;
		}
	}

}

LRESULT CDraftDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	switch (message)
	{
	case WM_DELETESELITEM:
		{
			DeleteItems();
		}
		break;
	case WM_SCROLL_EDO:
	//	ScrollItemsInList(wParam, lParam);
		break;
	default:
		break;
	}

	return CDialog::WindowProc(message, wParam, lParam);
}

void CDraftDlg::OnListCltrlClick(WPARAM w, LPARAM l)
{
	CMultimediaPhoneDlg *pMainDlg = ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd));
	if(l == 7)    //详情
	{
		if(w >= 0 && w <= (PAGE_COUNT-1))
		{
			C3GSMSDetailDlg *pWnd_ = pMainDlg->m_pMainDlg->m_p3GSMSDlg->m_pSMSDetailDlg;
			if((m_nBoxType == DRAFT_TYPE) || (m_nBoxType == SEND_TYPE))
			{
				if(m_nSMSType == MMS_TYPE)
				{
					pWnd_->initDataBase(MMS_READ, m_vMMSDataCurrentResult[w]->id(), FALSE);
					pWnd_->ShowWindow(SW_SHOW);
				}
				else
				{
					pWnd_->initDataBase(SMS_READ, m_vMessageCurrentResult[w]->id(), FALSE);
					pWnd_->ShowWindow(SW_SHOW);
				}

			}
			else
			{
				if(m_nSMSType == MMS_TYPE)
				{
					pWnd_->initDataBase(MMS_READ, m_vMMSDataCurrentResult[w]->id(), FALSE);
					m_vMMSDataCurrentResult[w]->isRead = TRUE;
					m_vMMSDataCurrentResult[w]->Update();
					pWnd_->ShowWindow(SW_SHOW);
					ShowArrayInList();
				}
				else
				{
					pWnd_->initDataBase(SMS_READ, m_vMessageCurrentResult[w]->id(), FALSE);
					m_vMessageCurrentResult[w]->state = Data::Message::sReaded;
					m_vMessageCurrentResult[w]->Update();
					pWnd_->ShowWindow(SW_SHOW);
					ShowArrayInList();
				}
			}
		}
	}
	else if(l == 6)	//删除
	{
		if(w >= 0 && w <= (PAGE_COUNT-1))
		{
			m_nSelectItem = w;
			((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetHWnd(m_hWnd);
			std::string strTemp = ".\\adv\\mjpg\\k1\\common\\确定删除吗.bmp";
			((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetDelTip(strTemp.c_str());
			((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->ShowWindow_(TRUE);
		}
	}
	else if(l == 5)	//回复  发送
	{
		if(w >= 0 && w <= (PAGE_COUNT-1))
		{
			CString sTelcode = "";
			CString sContent = "";
			C3GSMSDetailDlg *pWnd_ = pMainDlg->m_pMainDlg->m_p3GSMSDlg->m_pSMSDetailDlg;
			if((m_nBoxType == DRAFT_TYPE) || (m_nBoxType == SEND_TYPE))  //发送
			{
				if(m_nSMSType == MMS_TYPE)
				{
					//todo:彩信
				}
				else
				{
					sTelcode = m_vMessageCurrentResult[w]->remote.address.c_str();
					sContent = m_vMessageCurrentResult[w]->unicodeData.c_str();

					pWnd_->initDataBase(SMS_NEW, -1, FALSE);
					pWnd_->SetSMSDetail(sTelcode, sContent);
					pWnd_->ShowWindow(SW_SHOW);
				}		
			}
			else
			{
				if(m_nSMSType == MMS_TYPE)
				{
					sTelcode = m_vMMSDataCurrentResult[w]->SenderAddress.c_str();
				}
				else
				{
					sTelcode = m_vMessageCurrentResult[w]->remote.address.c_str();
				}	
				
				pWnd_->initDataBase(SMS_NEW, -1, FALSE);
				pWnd_->SetSMSDetail(sTelcode, sContent);
				pWnd_->ShowWindow(SW_SHOW);
			}
		}
	}
	else if(l == 4)	//转发
	{
		if(w >= 0 && w <= (PAGE_COUNT-1))
		{
			CString sTelcode = "";
			CString sContent = "";
			C3GSMSDetailDlg *pWnd_ = pMainDlg->m_pMainDlg->m_p3GSMSDlg->m_pSMSDetailDlg;
			if(m_nSMSType == MMS_TYPE)
			{
			//	sTelcode = m_vMMSDataCurrentResult[w]->unicodeData.c_str();
			}
			else
			{
				sContent = m_vMessageCurrentResult[w]->unicodeData.c_str();
				pWnd_->initDataBase(SMS_NEW, -1, FALSE);
				pWnd_->SetSMSDetail(sTelcode, sContent);
				pWnd_->ShowWindow(SW_SHOW);
			}	
		}
	}
	else if(l == 3)	//回电
	{
		if(w >= 0 && w <= (PAGE_COUNT-1))
		{
			CString sTelcode = "";
			if(m_nSMSType == MMS_TYPE)
			{
				sTelcode = m_vMMSDataCurrentResult[w]->SenderAddress.c_str();
			}
			else
			{
				sTelcode = m_vMessageCurrentResult[w]->remote.address.c_str();
			}
			if(sTelcode != "")
			{
				((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pTelephoneDlg->DialContact(Util::StringOp::FromCString(sTelcode), -1);
			}
		}
	}
}

void CDraftDlg::SetUpBtn(int ID)
{
	m_MJPGList.SetUnitIsDownStatus(ID,TRUE);
	for (int i = 0 ; i < 6;i++)
	{	
		if (i != ID)
		{
			m_MJPGList.SetUnitIsDownStatus(i,FALSE);
		}
	}
	m_MJPGList.Invalidate();
}

void CDraftDlg::ClearCurrentPage()
{	
	CString empty(L"");
	int ibegin = 30 ;
	int items = 0 ;
	for (int i = 0 ; i < m_nPageSize ;i++)
	{	

		m_MJPGList.SetUnitIsDownStatus(ibegin+items,false);
		m_MJPGList.SetUnitIsShow(ibegin+items,false,false);
		
		for (int j = 0 ; j < 3 ; j++)
		{
			m_MJPGList.SetUnitText(ibegin+items+j+1,empty,false);//收件人内容
			m_MJPGList.SetUnitIsShow(ibegin+items+j+1,false,false);
		}					
		items = items+10;
	}	
	
	m_MJPGList.SetUnitIsShow(130,false,false);

	m_MJPGList.Invalidate();

}

void CDraftDlg::SelectAll()
{	
	int unitID = 30 ;
	if (!m_bSelectAll)
	{	
		m_bSelectAll = true ;
		for (int i = 0 ; i < GetCurrentItems(); i++)
		{					   			
			m_MJPGList.SetUnitIsDownStatus(130,true);
			m_MJPGList.SetUnitIsDownStatus(unitID,true);		
								
			unitID +=10;
		}
	}
	else
	{	
		m_bSelectAll = false ;
		for (int i = 0 ; i < GetCurrentItems(); i++)
		{											
			m_MJPGList.SetUnitIsDownStatus(130,false);
			m_MJPGList.SetUnitIsDownStatus(unitID,false);		
			
			unitID +=10;
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


void CDraftDlg::DeleteItems()
{	
	int npos = 0 ;
	//删除打勾的行，目前只删除当前的行
	for (int i = 0 ; i < m_vClick.size() ;i++)
	{	
		if ( 1 == m_vClick[(m_iCurrentPage-1)*m_nPageSize+i])
		{	
			if ( SMS_TYPE == m_nSMSType)
			{	
				Data::Message::Remove("id = " + Util::StringOp::FromInt(m_vID[i]));
			}

			if ( MMS_TYPE == m_nSMSType)
			{	
				std::string filter ;
				filter = "id = ";
				filter += Util::StringOp::FromInt(m_vID[i]); 
				std::vector< boost::shared_ptr< Data::MMSData > >result ;
				result = Data::MMSData::GetFromDatabase(filter);
				if (!result.empty())
				{
					CString s = result[0]->SavePath.c_str();				
					CString filename = L"";
					CString path = L"";
					
					extern int FindFileEx(CString s, CString &sPath, CString &sFilename);
					FindFileEx(s, path, filename);
					extern void DeleteDirectory(CString SrcDir, BOOL isShow = TRUE);
					DeleteDirectory(path);
					
					Data::MMSData::Remove("id = " + Util::StringOp::FromInt(m_vID[i]));
					
				}
				
			}
			
			npos++;
			((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetProcessPos(npos);
			
		}
	}
	
	//删除容器里的数据
	std::vector<int>::iterator it;
	it = m_vClick.begin();	
	std::vector<int>::iterator itID;
	itID = m_vID.begin();
	for ( it ; it < m_vClick.end() ; it)
	{	
		if ( 1 == (*it))
		{
			m_vClick.erase(it);
			m_vID.erase(itID);
		}
		else
		{
			itID++ ;
			it++;
		}
	}	
	
	//容器的末尾值也去掉，保留为页的整数
	if ( m_vClick.size()%m_nPageSize != 0)
	{
		it	 = m_vClick.end() - m_vClick.size()%m_nPageSize ;
		itID = m_vID.end() - m_vID.size()%m_nPageSize ;
		for (it ; it < m_vClick.end() ;it)
		{
			m_vClick.erase(it);
			m_vID.erase(itID);
		}
	}
	
	//重新获得当前的总页数
	if (SMS_TYPE == m_nSMSType)
	{
		m_iTotalPages = Data::Message::GetDataCount(m_sListFilter)/m_nPageSize;
		if (Data::Message::GetDataCount(m_sListFilter)%m_nPageSize != 0)
		{
			m_iTotalPages +=1;
		}
	}
	else
	{
		m_iTotalPages = Data::MMSData::GetDataCount(m_sListFilter)/m_nPageSize;
		if (Data::MMSData::GetDataCount(m_sListFilter)%m_nPageSize != 0)
		{
			m_iTotalPages +=1;
		}
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
	FromDataBase();
	ShowArrayInList();
	
} 

void CDraftDlg::PageSwitch(Action action)
{
	 if ( down_page == action)
	 {
		m_iCurrentPage++;
		if (m_iCurrentPage > m_iTotalPages )
		{
			m_iCurrentPage-- ;
			return ;
		}

	 }
		
	 if ( up_page == action)
	 {
		m_iCurrentPage--;
		if (m_iCurrentPage < 1)
		{
			m_iCurrentPage++;
			return ;
		}
	 }
	
	 ClearCurrentPage();
	 FromDataBase();
	 ShowArrayInList();
}

int CDraftDlg::GetCurrentItems()
{	
	int icurrent ;//当前页的行数
			
	if (SMS_TYPE == m_nSMSType)
	{
		icurrent = m_vMessageCurrentResult.size() ;
	}
	
	if (MMS_TYPE == m_nSMSType)
	{
		icurrent = m_vMMSDataCurrentResult.size();
	}

	return icurrent ;
}

void CDraftDlg::Clear()
{	
	if (m_vMessageCurrentResult.size() > 0)
	{
		m_vMessageCurrentResult.clear();
	}

	if (m_vMMSDataCurrentResult.size() > 0)
	{
		m_vMMSDataCurrentResult.clear();
	}

	if (m_vClick.size())
	{
		m_vClick.clear();
	} 
	
	if (m_vID.size())
	{
		m_vID.clear();
	}

	m_bSelectAll = false ;
}

void CDraftDlg::ClickedOneItem(int unitID,int item)
{	

	if (m_vClick[(m_iCurrentPage-1)*m_nPageSize+item] == 0)//被点击了
	{		
		m_MJPGList.SetUnitIsDownStatus(unitID,true);		
		m_vClick[(m_iCurrentPage-1)*m_nPageSize+item] = 1;
	}
	else
	{		
		m_MJPGList.SetUnitIsDownStatus(unitID,false);
		m_vClick[(m_iCurrentPage-1)*m_nPageSize+item] = 0;
	}

	m_MJPGList.SetUnitIsShow(unitID,true,true);
	
}

void CDraftDlg::ReadOneItem(int item)
{	
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd ;
	if (SMS_TYPE == m_nSMSType)
	{	
		main->m_pSMSListDlg->m_pSmsReadDlg->SetSMSInfo(m_vMessageCurrentResult[item]->id(),m_nBoxType);
		main->m_pSMSListDlg->m_pSmsReadDlg->ShowWindow(SW_SHOW);

	}
	else
	{
		main->m_pSMSListDlg->m_pMmsReadDlg->SetMMSInfo(m_vMMSDataCurrentResult[item]->id(),m_nBoxType);
		main->m_pSMSListDlg->m_pMmsReadDlg->ShowWindow(SW_SHOW);	
	}

}

void CDraftDlg::ShowSMS(SMSMMS_TYPE smsType)
{
	if (smsType != m_nSMSType)
	{	
		m_nSMSType = smsType ;
		m_iCurrentPage = 1 ;
		Clear();
		ClearCurrentPage();
		FromDataBase();
		ShowArrayInList();
	}
}
void CDraftDlg::SetUnitStatus()
{
	int ibegin = 30 ;
	int items = 0 ;
	for (int i = 0 ; i < GetCurrentItems();i++)
	{	
		for (int j = 0 ; j < 3 ;j++)
		{
			m_MJPGList.SetUnitIsShow(ibegin+items+j,true,false);
		}

		if ( 1 == m_vClick[(m_iCurrentPage-1)*m_nPageSize + i])
		{
			m_MJPGList.SetUnitIsDownStatus(ibegin+items,true);
		}
		
		items = items+10;
	}

	if ( GetCurrentItems() > 0)
	{
		m_MJPGList.SetUnitIsShow(130,true,false);
	}
	
	m_MJPGList.Invalidate();

}

void CDraftDlg::ShowWindow_(SMSBOX_TYPE type,SMSMMS_TYPE smsType)
{	
	m_nBoxType = type ;
	m_nSMSType = smsType;
	m_iCurrentPage = 1 ;
	
	Clear();
	ClearCurrentPage();
	FromDataBase();
	ShowArrayInList();
	ShowWindow(SW_SHOW);
}

void CDraftDlg::ShowDeleteDlg()
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
		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->ShowWindow_(SW_SHOW);
	}

}

void CDraftDlg::SetUpPages()
{	
	CString page ;
	page.Format(L"%d/%d",m_iCurrentPage,m_iTotalPages);
	if (  0 == m_iTotalPages)
	{
		page.Format(L"%d/%d",0,m_iTotalPages);
	}
	m_MJPGList.SetUnitText(110,page,TRUE);//设置当前的页
}

int CDraftDlg::GetVectorPages()
{
	int pages ;
	pages = m_vClick.size()/m_nPageSize ;
	if ( m_vClick.size()%m_nPageSize!= 0)
	{
		pages += 1;
	}
	return pages ;
}

void CDraftDlg::Edit()
{
	//只能选择一个
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd ;	
	int index ;
	if ( GetFirstClickID(index) )//只有收件的时候才能回复
	{	
		std::string data ;
		std::string mmstitle;
		if (SMS_TYPE == m_nSMSType)
		{
			 boost::shared_ptr<Data::Message> pmessge ;
			pmessge = Data::Message::GetDataById( m_vID[index]);
			if ( pmessge )
			{
				main->m_pMainDlg->m_p3GSMSDlg->SetMessge(pmessge);
				main->m_pMainDlg->m_p3GSMSDlg->ShowWindow(SW_SHOW);
			}
		}
		else
		{	
			boost::shared_ptr<Data::MMSData> pmmsdata ;
			pmmsdata = Data::MMSData::GetDataById(m_vID[index]);
			if (pmmsdata)
			{
				main->m_pMMSDlg->SetMmsContent(pmmsdata);
				main->m_pMMSDlg->ShowWindow(SW_SHOW);
			}
		}						
	}

}

void CDraftDlg::Transit()
{
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd ;
	int index ;
	if ( GetFirstClickID(index) )//只有收件的时候才能回复
	{	
		std::string data ;
		std::string mmstitle;
		if (SMS_TYPE == m_nSMSType)
		{
			boost::shared_ptr<Data::Message> pmessge ;
			pmessge = Data::Message::GetDataById( m_vID[index]);
			if ( pmessge )
			{
				std::string data = pmessge->unicodeData;	
				main->m_pMainDlg->m_p3GSMSDlg->SetSmsContent(data);
				main->m_pMainDlg->m_p3GSMSDlg->ShowWindow(SW_SHOW);
			}
		}
		else
		{	
			boost::shared_ptr<Data::MMSData> pmmsdata ;
			pmmsdata = Data::MMSData::GetDataById(m_vID[index]);
			if (pmmsdata)
			{
				main->m_pMMSDlg->SetMmsContent(pmmsdata,true);
				main->m_pMMSDlg->ShowWindow(SW_SHOW);
			}
		}						
	}

}
void CDraftDlg::Details()
{	
	CMultimediaPhoneDlg * main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	int index ;
	if (GetFirstClickID(index,true))
	{
		if (MMS_TYPE == m_nSMSType)
		{	
			main->m_pSMSListDlg->m_pMmsReadDlg->SetMMSInfo(m_vID[index],m_nBoxType);
			main->m_pSMSListDlg->m_pMmsReadDlg->ShowWindow(SW_SHOW);
		}
		else
		{
			main->m_pSMSListDlg->m_pSmsReadDlg->SetSMSInfo(m_vID[index],m_nBoxType);
			main->m_pSMSListDlg->m_pSmsReadDlg->ShowWindow(SW_SHOW);

		}
	}	
}

void CDraftDlg::Repeat()//重发
{	
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd ; 
	int index ;
	if ( GetFirstClickID(index) )
	{
		if (MMS_TYPE == m_nSMSType)
		{
			boost::shared_ptr<Data::MMSData> pmmsdata ;
			pmmsdata = Data::MMSData::GetDataById(m_vID[index]);
			
			SYSTEMTIME tm;
			GetLocalTime(&tm);
			CTime ct = CTime(tm);
			CTimeSpan sp = ct - CTime(1970, 1, 1, 8, 0, 0);
		
			char datetime[24];
			sprintf(datetime, "%04d%02d%02d%02d%02d%02d", tm.wYear, tm.wMonth, tm.wDay, tm.wHour, tm.wMinute, tm.wSecond);
						
			pmmsdata = boost::shared_ptr<Data::MMSData> (new Data::MMSData);
			pmmsdata->type = Data::MMSData::tpReadySend;
			pmmsdata->isRead = false; //表示发送失败
			pmmsdata->TransactionId = datetime;
			pmmsdata->DateAndTime = sp.GetSeconds(); //Date-value = Long-integer	In seconds from 1970-01-01, 00:00:00 GMT.
			pmmsdata->isRead = TRUE;
			pmmsdata->Insert();

		}
		else
		{	
			boost::shared_ptr<Data::Message> pmessge ;
			pmessge = Data::Message::GetDataById( m_vID[index]);

			SYSTEMTIME tm;
			GetLocalTime(&tm);
			pmessge->timestamp.year = tm.wYear%100;
			pmessge->timestamp.month = tm.wMonth;
			pmessge->timestamp.day = tm.wDay;
			pmessge->timestamp.hour = tm.wHour;
			pmessge->timestamp.minite = tm.wMinute;
			pmessge->timestamp.second = tm.wSecond;
			pmessge->group = Data::Message::gSend;
			pmessge->Insert();
			
			main->m_pSMSWarp->Send(pmessge->remote.address, pmessge->unicodeData);							
			
		}
	}

}

void CDraftDlg::SetPagefont()
{	
	int begin = 30 ;
	for (int i = 0 ; i < m_nPageSize ;i++)
	{
		for (int j = 1; j < 4;j++)
		{
			m_MJPGList.SetUnitColor(begin+i*10+j,font_white,true);
		}		
	}
	m_MJPGList.SetUnitColor(110,font_white,true);
}

bool CDraftDlg::GetFirstClickID(int &index,bool bcurrent )
{	
	bool enble = false ;
	if (!bcurrent)
	{
		for (int i = 0 ; i < m_vClick.size() ;i++)
		{
			if (1 == m_vClick[i])
			{	
				enble = true ;
				index = i;
			}	
		}
	}
	else//从当前页计算
	{
		int icurrent = GetCurrentItems();
		for (int i = 0 ; i < icurrent ;i++)
		{
			if ( 1 == m_vClick[(m_iCurrentPage-1)*m_nPageSize + i])
			{
				enble = true ;
				index = i ;
				break ;
			}
			
		}
	}
	return enble ;
}