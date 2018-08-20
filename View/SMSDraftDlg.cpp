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

extern HANDLE g_hEvent;
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

	m_MJPGList.Create(L"", WS_VISIBLE|WS_CHILD, CRect(0, 0, 800, 423), this,10086);
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
		main->AddIcon(Allicon[1],main->m_pMainDlg->m_p3GSMSDlg,false);
		main->PopIcon(this);
		break;

	case 1://写彩信息			
		main->m_pMMSDlg->ShowWindow_(SW_SHOW);
		main->AddIcon(Allicon[1],main->m_pMMSDlg,false);
		main->PopIcon(this);
		break ;

	case 2://收件箱	
		main->m_pSMSListDlg->ShowWindow_(RECV_TYPE,SMS_TYPE);
		main->PopIcon(this);
		break;

	case 3://发件箱	
		main->m_pSMSListDlg->ShowWindow_(SEND_TYPE,SMS_TYPE);
		main->PopIcon(this);
		break;

	case 4://草稿箱
		break;

	case 5://设置
		if (main->m_bSearchNetWin)
		{
			main->AddIcon(Allicon[1],main->m_pSmsSettingDlg,false);
			main->PopIcon(this);
		}
		main->m_pSmsSettingDlg->ShowWindow_(SW_SHOW);
		break;
	
	case 10://删除
		ShowDeleteDlg();
		break;
		
	case 11://编辑
		Edit();
		break;

	case 12://转发
		Transit();
		break;

	case 13://详情
		Details();
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
		//ReadOneItem(w/10-3);
		Edit(w/10-3);
//		main->AddIcon(Allicon[1]);
		break;

	case 100://短消息
		ShowSMS(SMS_TYPE);
		break;

	case 101://彩信息
		ShowSMS(MMS_TYPE);
		break;

	case 102:
		main->m_pSMSListDlg->SetCapacity(m_nSMSType);
		break;

	case 110://页数显示
		break;

	case 120://上一页
		PageSwitch(up_page);
		break;

	case 121://下一页
		PageSwitch(down_page);
		break;

	case 130://全选
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
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
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
			std::string number ;
			std::string name ;
			number = m_vMMSDataCurrentResult[i]->RecipientAddress;//在发件箱里，表示是否发送成功。收件箱里表示是否阅读
			main->m_pSMSListDlg->AnalyseSender(number,name);
			
			temp = Util::StringOp::ToCString(name);
			if (temp.Mid(temp.GetLength()-1) == L";")
			{
				temp = temp.Mid(0,temp.GetLength()-1);
			}

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
			
			std::string number ;
			std::string name ;
			number = m_vMessageCurrentResult[i]->remote.address;//在发件箱里，表示是否发送成功。收件箱里表示是否阅读
			main->m_pSMSListDlg->AnalyseSender(number,name);
			
			temp = Util::StringOp::ToCString(name);
			if (temp.Mid(temp.GetLength()-1) == L";")
			{
				temp = temp.Mid(0,temp.GetLength()-1);
			}
			
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
			if (m_bSelectAll)
			{
				DeleteALL();
			}
			else
			{
				DeleteItems();
			}
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
	
//	m_MJPGList.SetUnitIsDownStatus(130,false);
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
			m_MJPGList.SetUnitIsDownStatus(unitID,true);									
			unitID +=10;
		}
		m_MJPGList.SetUnitIsDownStatus(130,true);
	}
	else
	{	
		//add by qi 0727
		if (m_MJPGList.GetUnitIsDownStatus(130))//如果是按下状态
		{
			m_bSelectAll = false ;
			for (int i = 0 ; i < GetCurrentItems(); i++)
			{											
				m_MJPGList.SetUnitIsDownStatus(unitID,false);					
				unitID +=10;
			}
			m_MJPGList.SetUnitIsDownStatus(130,false);

		}
		else//不是按下状态还是全选
		{	
			m_bSelectAll = true ;
			for (int i = 0 ; i < GetCurrentItems() ; i++)
			{					   			
				m_MJPGList.SetUnitIsDownStatus(unitID,true);									
				unitID +=10;
			}
			m_MJPGList.SetUnitIsDownStatus(130,true);
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

void CDraftDlg::DeleteALL()
{
	int icount ;
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	if ( MMS_TYPE == m_nSMSType)
	{	
		icount = Data::MMSData::GetDataCount(m_sListFilter);
		int num = 0 ;
		int ndelnum = 0; 
		int nalldelnum = 0;
		int nleft = 0 ;
		while (num < icount)
		{	
			ndelnum = 0;
			std::vector<boost::shared_ptr<Data::MMSData> > result;
			result = Data::MMSData::GetFromDatabaseByTypeOffsetLength(m_sListFilter, 
				nleft+1, m_nPageSize);
			
			for (int i = 0 ; i < result.size();i++)
			{	
				bool bdel = false;
				if ((i+num) >= m_vClick.size())
				{
					bdel = true;
				}
				else
				{
					if ( 1 == m_vClick[i+num])
					{
						bdel = true;
					}
					else//没有被选中的
					{
						nleft++;
					}
				}

				if(bdel)
				{
					CString s = result[i]->SavePath.c_str();				
					CString filename = L"";
					CString path = L"";
					
					extern int FindFileEx(CString s, CString &sPath, CString &sFilename);
					FindFileEx(s, path, filename);
					extern void DeleteDirectory(CString SrcDir, BOOL isShow = TRUE);
					DeleteDirectory(path);
					
					result[i]->Remove();
					ndelnum++;

				}

			}
			
			num += result.size();
			if (ndelnum)
			{
				nalldelnum += ndelnum; 
				main->m_pDeleteTipDlg->SetProcessPos(nalldelnum);
			}
			
		}
		
	}
	else 
	{
		icount = Data::Message::GetDataCount(m_sListFilter);
		
		int num = 0 ;
		int ndelnum = 0; 
		int nalldelnum = 0;
		int nleft = 0 ;
		while (num < icount)
		{	
			ndelnum = 0;
			std::vector<boost::shared_ptr<Data::Message> > result;
			result = Data::Message::GetFromDatabaseByTypeOffsetLength(m_sListFilter, 
				nleft+1, m_nPageSize);
			for (int i = 0 ; i < result.size();i++)
			{	
				bool bdel = false;
				if ((i+num) >= m_vClick.size())
				{
					bdel = true;
				}
				else
				{
					if ( 1 == m_vClick[i+num])
					{
						bdel = true;
					}
					else//没有被选中的
					{
						nleft++;
					}
				}

				if (bdel)
				{
					result[i]->Remove();
					ndelnum++;
				}
			}	
			
			num += result.size();
			if (ndelnum)
			{
				nalldelnum += ndelnum; 
				main->m_pDeleteTipDlg->SetProcessPos(nalldelnum);
			}
			
		}
		
	}
	
	m_iCurrentPage = 1 ;
	Clear();
	ClearCurrentPage();
	FromDataBase();
	ShowArrayInList();
	SetPageTurnBtn();
}

void CDraftDlg::DeleteItems()
{	
	int npos = 0 ;
	//删除打勾的行，目前只删除当前的行
	for (int i = 0 ; i < m_vClick.size() ;i++)
	{	
		if ( 1 == m_vClick[i])
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
	SetPageTurnBtn(1);

	ClearCurrentPage();

	//比较容器里的页数 和 当前所在的页
	int nCount = m_iCurrentPage;
	int pages ;
	pages = m_vClick.size()/m_nPageSize ;
	if (pages >= m_iCurrentPage)//剩余的数据大于当前的页，直接找当前页的数据
	{
		FromDataBase();
	}
	else
	{
		m_iCurrentPage = pages ;//
		for (;pages < nCount;pages++)//循环往容器里添数据
		{	
			m_iCurrentPage++;//
			FromDataBase();
		}
	}

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
	 
	 SetPageTurnBtn(1);

	 ClearCurrentPage();
	 FromDataBase();
	 ShowArrayInList();
}

int CDraftDlg::GetCurrentItems()
{	
	int icurrent = 0 ;//当前页的行数
			
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
	
	m_MJPGList.SetUnitIsDownStatus(130,false);
	m_bSelectAll = false ;
}

void CDraftDlg::ClickedOneItem(int unitID,int item)
{	

	if (m_vClick[(m_iCurrentPage-1)*m_nPageSize+item] == 0)//被点击了
	{		
		m_MJPGList.SetUnitIsDownStatus(unitID,true);		
		m_vClick[(m_iCurrentPage-1)*m_nPageSize+item] = 1;

		//add by qi 0727
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
				m_MJPGList.SetUnitIsDownStatus(130,true);
				m_MJPGList.SetUnitIsShow(130,true,true);
			}
		}

	}
	else
	{		
		m_MJPGList.SetUnitIsDownStatus(unitID,false);
		m_vClick[(m_iCurrentPage-1)*m_nPageSize+item] = 0;

		//add by qi 0727
		if (m_bSelectAll)
		{
			m_MJPGList.SetUnitIsDownStatus(130,false);
			m_MJPGList.SetUnitIsShow(130,true,true);	
		}

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
		SetPageTurnBtn();
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
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;

	m_nBoxType = type ;
	m_nSMSType = smsType;
	m_iCurrentPage = 1 ;
	
	Clear();
	ClearCurrentPage();
	ShowWindow(SW_SHOW);
	FromDataBase();
	ShowArrayInList();
	SetPageTurnBtn();

	main->AddIcon(Allicon[1],this,false);
}

void CDraftDlg::ShowDeleteDlg()
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
		enble = true ;
	}

	if (enble)
	{	
		CString title = L"";
		if (m_bSelectAll)
		{	
			int allcount;
			if (MMS_TYPE == m_nSMSType)
			{
				allcount = Data::MMSData::GetDataCount(m_sListFilter);	
			}

			if (SMS_TYPE == m_nSMSType)
			{
				allcount = Data::Message::GetDataCount(m_sListFilter);	
			}
			
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

		}
		else
		{
			title.Format(L"%d", count);
			title = L"是否删除已选中的" + title + L"条信息?";
		}
		main->m_pDeleteTipDlg->SetTitle(title,0);
		main->m_pDeleteTipDlg->SetProcessMax(count);
		main->m_pDeleteTipDlg->SetHWnd(this->GetSafeHwnd());
		main->m_pDeleteTipDlg->ShowWindow_(SW_SHOW);
	}
	else
	{
		main->m_pWarningNoFlashDlg->SetTitle(L"请选择信息");
		main->m_pWarningNoFlashDlg->ShowWindow_(SW_SHOW);
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
	CWnd *p ;
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
				Sleep(100);
				main->m_pMainDlg->m_p3GSMSDlg->OnCharNumberChange();
				p = main->m_pMainDlg->m_p3GSMSDlg;
			}
		}
		else
		{	
			boost::shared_ptr<Data::MMSData> pmmsdata ;
			pmmsdata = Data::MMSData::GetDataById( m_vID[index]);
			if (pmmsdata)
			{
				main->m_pMMSDlg->SetMmsContent(pmmsdata,true);
				main->m_pMMSDlg->ClearParPage();
				main->m_pMMSDlg->ShowWindow(SW_SHOW);
				Sleep(100);
				main->m_pMMSDlg->m_MmsShow.AllFileSize();
				p = main->m_pMMSDlg;
			}
		}
		main->AddIcon(Allicon[1],p,false);
	}

}

void CDraftDlg::Edit(int item)
{	
	//只能选择一个
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd ;
	std::string data ;
	std::string mmstitle;
	if (SMS_TYPE == m_nSMSType)
	{
		boost::shared_ptr<Data::Message> pmessge ;
		pmessge = Data::Message::GetDataById( m_vID[(m_iCurrentPage-1)*m_nPageSize+item]);
		if ( pmessge )
		{
			main->m_pMainDlg->m_p3GSMSDlg->Clear();
			main->m_pMainDlg->m_p3GSMSDlg->SetMessge(pmessge);
			main->m_pMainDlg->m_p3GSMSDlg->ShowWindow(SW_SHOW);
			Sleep(100);
			main->m_pMainDlg->m_p3GSMSDlg->OnCharNumberChange();
			main->AddIcon(Allicon[1],main->m_pMainDlg->m_p3GSMSDlg,false);
		}
	}
	else
	{	
		boost::shared_ptr<Data::MMSData> pmmsdata ;
		pmmsdata = Data::MMSData::GetDataById( m_vID[(m_iCurrentPage-1)*m_nPageSize+item]);
		if (pmmsdata)
		{	
			main->m_pMMSDlg->Clear();
			main->m_pMMSDlg->SetMmsContent(pmmsdata,true);
			main->m_pMMSDlg->ClearParPage();
			main->m_pMMSDlg->ShowWindow(SW_SHOW);
			Sleep(100);
			main->m_pMMSDlg->m_MmsShow.AllFileSize();
			main->AddIcon(Allicon[1],main->m_pMMSDlg,false);
		}
	}

}

void CDraftDlg::Transit()
{
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd ;
	CWnd *p;
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
				Sleep(100);
				main->m_pMainDlg->m_p3GSMSDlg->OnCharNumberChange();
				p = main->m_pMainDlg->m_p3GSMSDlg;
			}
		}
		else
		{	
			boost::shared_ptr<Data::MMSData> pmmsdata ;
			pmmsdata = Data::MMSData::GetDataById(m_vID[index]);
			if (pmmsdata)
			{
				main->m_pMMSDlg->SetMmsContent(pmmsdata,true);
				main->m_pMMSDlg->ClearParPage();
				main->m_pMMSDlg->ShowWindow(SW_SHOW);
				Sleep(100);
				main->m_pMMSDlg->m_MmsShow.AllFileSize();
				p = main->m_pMMSDlg;

			}
		}		
		main->AddIcon(Allicon[1],p,false);
	}

}
void CDraftDlg::Details()
{	
	CMultimediaPhoneDlg * main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	CWnd *p ;
	int index ;
	if (GetFirstClickID(index,true))
	{
		if (MMS_TYPE == m_nSMSType)
		{	
			boost::shared_ptr<Data::MMSData > pMmsData = Data::MMSData::GetDataById(m_vID[index]);
			main->m_pSMSListDlg->m_pSmsDetailDlg->ShowMmsDetail(pMmsData);
			main->m_pSMSListDlg->m_pSmsDetailDlg->ShowWindow_(SW_SHOW);
			p = main->m_pSMSListDlg->m_pMmsReadDlg;
		}
		else
		{
			boost::shared_ptr<Data::Message > pMessage = Data::Message::GetDataById(m_vID[index]);
			main->m_pSMSListDlg->m_pSmsDetailDlg->ShowSmsDetail(pMessage);
			main->m_pSMSListDlg->m_pSmsDetailDlg->ShowWindow_(SW_SHOW);
			p = main->m_pSMSListDlg->m_pSmsReadDlg;

		}

		main->AddIcon(Allicon[1],p,false);

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
			//判断磁盘空间
			if (main->IsDiskFull())
			{
				return;
			}

			boost::shared_ptr<Data::MMSData> pmmsdata ;
			pmmsdata = Data::MMSData::GetDataById(m_vID[index]);
			
			SYSTEMTIME tm;
			GetLocalTime(&tm);
			CTime ct = CTime(tm);
			CTimeSpan sp = ct - CTime(1970, 1, 1, 0, 0, 0);
		
			char datetime[24];
			sprintf(datetime, "%04d%02d%02d%02d%02d%02d", tm.wYear, tm.wMonth, tm.wDay, tm.wHour, tm.wMinute, tm.wSecond);
						
			pmmsdata->type = Data::MMSData::tpReadySend;
			pmmsdata->isRead = false; //表示发送失败
			pmmsdata->TransactionId = datetime;
			pmmsdata->DateAndTime = sp.GetTotalSeconds(); //Date-value = Long-integer	In seconds from 1970-01-01, 00:00:00 GMT.

			pmmsdata->Insert();
			main->m_isDialTimeout = 0;
			SetEvent(g_hEvent);
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

			if(main->m_pMessageSet->SaveSendMessage())
			{	
				main->m_pMainDlg->m_p3GSMSDlg->SaveSmsData(pmessge,Data::Message::gSend);
			//	pmessge->Insert();
			}
			
			if (pmessge->remote.address.empty())
			{
				CString tille = L"没有号码不能重发!";
				main->m_pWarningNoFlashDlg->SetTitle(tille);
				main->m_pWarningNoFlashDlg->ShowWindow_(TRUE);
				return ;
			}

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

void CDraftDlg::FreshList()
{
	m_iCurrentPage = 1 ;
	
	Clear();
	ClearCurrentPage();
	FromDataBase();
	ShowArrayInList();
	SetPageTurnBtn();
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

	if (!enble)
	{
		CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
		main->m_pWarningNoFlashDlg->SetTitle(L"请选择信息");
		main->m_pWarningNoFlashDlg->ShowWindow_(SW_SHOW);
	}
	return enble ;
}

void CDraftDlg::SetPageTurnBtn(int type )
{
	if ( 0 == type)
	{	
		int ncount = 0  ; 
		if(m_nSMSType == SMS_TYPE)
			ncount = Data::Message::GetDataCount(m_sListFilter);
		else
			ncount = Data::MMSData::GetDataCount(m_sListFilter);
		
		if (ncount <= m_nPageSize)//小于一页
		{
			m_MJPGList.SetUnitIsDisable(120,true);
			m_MJPGList.SetUnitIsDisable(121,true);
		}
		else //大于一页
		{
			m_MJPGList.SetUnitIsDisable(120,true);
			m_MJPGList.SetUnitIsDisable(121,false);
		}
	}
	else if( 1 == type)
	{
		//判断翻页按钮是否有效
		m_MJPGList.SetUnitIsDisable(120,false);
		m_MJPGList.SetUnitIsDisable(121,false);
		if (m_iCurrentPage == 1)
		{
			m_MJPGList.SetUnitIsDisable(120,true);
		}
		
		if (m_iCurrentPage == m_iTotalPages)
		{
			m_MJPGList.SetUnitIsDisable(121,true);
		}
	}
}