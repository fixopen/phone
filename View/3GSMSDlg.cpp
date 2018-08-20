// YHTelDlg.cpp : implementation file
//

#include "stdafx.h"
#include "../multimediaphone.h"
#include "../MultimediaPhoneDlg.h"
#include "3GSMSDlg.h"
#include "sip.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// C3GSMSDlg dialog


#define  ALL_SENDER  20


CSMSDlg::CSMSDlg(CWnd* pParent /*=NULL*/)
: CDialog(CSMSDlg::IDD, pParent)
,pageSize(5)
{
	m_iCurrentPage	= 0 ;

}


void CSMSDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(C3GSMSDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSMSDlg, CDialog)
	//{{AFX_MSG_MAP(C3GSMSDlg)
	ON_MESSAGE(WM_CLICKMJPG_TOAPP, OnClickMJPG)
//	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// C3GSMSDlg message handlers
/*
void C3GSMSDlg::OnTimer(UINT nIDEvent)
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

BOOL CSMSDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

//	m_pSMSListDlg = new C3GSMSListDlg(this);
//	m_pSMSListDlg->Create(C3GSMSListDlg::IDD);

	m_pSMSDetailDlg = new C3GSMSDetailDlg(this);
	m_pSMSDetailDlg->Create(C3GSMSDetailDlg::IDD);
	
	int height;
	m_senderEdit1.Create(WS_CHILD|WS_VISIBLE|ES_MULTILINE | ES_AUTOVSCROLL | ES_WANTRETURN , CRect(28, 120, 28+189, 120+29), this, 0xFFFF);
	height = 120 + 29 + 11;
	m_senderEdit2.Create(WS_CHILD|WS_VISIBLE|ES_MULTILINE | ES_AUTOVSCROLL | ES_WANTRETURN , CRect(28, height, 28+189, height+29), this, 0xFFFF);
	height += 29 + 11;
	m_senderEdit3.Create(WS_CHILD|WS_VISIBLE|ES_MULTILINE | ES_AUTOVSCROLL | ES_WANTRETURN , CRect(28, height, 28+189, height+29), this, 0xFFFF);
	height += 29 + 12;
	m_senderEdit4.Create(WS_CHILD|WS_VISIBLE|ES_MULTILINE | ES_AUTOVSCROLL | ES_WANTRETURN , CRect(28, height, 28+189, height+29), this, 0xFFFF);
	height += 29 + 11;
	m_senderEdit5.Create(WS_CHILD|WS_VISIBLE|ES_MULTILINE | ES_AUTOVSCROLL | ES_WANTRETURN , CRect(28, height, 28+189, height+29), this, 0xFFFF);
	
	m_senderEdit2.SetReadOnly(TRUE);
	m_senderEdit3.SetReadOnly(TRUE);
	m_senderEdit4.SetReadOnly(TRUE);
	m_senderEdit5.SetReadOnly(TRUE);

	m_contentEdit.Create(WS_CHILD|WS_VISIBLE|ES_MULTILINE | ES_AUTOVSCROLL | ES_WANTRETURN , CRect(239, 125, 239+396, 125+260), this, 0xFFFF);
	
	m_p10PhrasesDlg = new C10PhrasesDlg();
	m_p10PhrasesDlg->Create(C10PhrasesDlg::IDD);
	m_p10PhrasesDlg->ShowWindow(SW_HIDE);

	m_MJPGList.Create(L"", WS_VISIBLE|WS_CHILD, CRect(0, 0, 800, 423), this);
	m_MJPGList.SetCurrentLinkFile(".\\adv\\mjpg\\k5\\中文\\写短信.xml");
	m_MJPGList.SetMJPGRect(CRect(0, 0, 800, 423));
	
	m_MJPGList.SetUnitIsDownStatus(0,true);

	MoveWindow(0,57,800,423);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
BOOL CSMSDlg::PreTranslateMessage(MSG* pMsg)
{
	if ((pMsg->wParam) == MK_LBUTTON )
	{
		CString content;
		if (pMsg->hwnd == m_senderEdit2.m_hWnd)
		{
			m_senderEdit1.GetWindowText(content);
			if (!content.IsEmpty())
			{
				m_senderEdit2.SetReadOnly(false);
			}
		}
		if (pMsg->hwnd == m_senderEdit3.m_hWnd)
		{
			m_senderEdit2.GetWindowText(content);
			if (!content.IsEmpty())
			{
				m_senderEdit3.SetReadOnly(false);
			}
		}		
		if (pMsg->hwnd == m_senderEdit4.m_hWnd)
		{
			m_senderEdit3.GetWindowText(content);
			if (!content.IsEmpty())
			{
				m_senderEdit4.SetReadOnly(false);
			}
		}		
		if (pMsg->hwnd == m_senderEdit5.m_hWnd)
		{
			m_senderEdit4.GetWindowText(content);
			if (!content.IsEmpty())
			{
				m_senderEdit5.SetReadOnly(false);
			}
		}
	}
	return CDialog::PreTranslateMessage(pMsg);

}
void CSMSDlg::OnClickMJPG(WPARAM w, LPARAM l)
{
	CMultimediaPhoneDlg *main = ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd));
	CString icon ;
	std::vector<CString> vnum;
	switch(w)
	{
	case 0:
		break;

	case 1://写彩信
		GetTelnum(current_page);
		GetSender(vnum);
		main->m_pMMSDlg->ShowWindow_(SW_SHOW);
		main->m_pMMSDlg->SetSender(vnum);
		ShowWindow(SW_HIDE);
		break;

	case 2://收件箱
		main->m_pSMSListDlg->ShowWindow_(RECV_TYPE,SMS_TYPE);
		ShowWindow(SW_HIDE);
		break ;

	case 3://发件箱
		main->m_pSMSListDlg->ShowWindow_(SEND_TYPE,SMS_TYPE);
		ShowWindow(SW_HIDE);
		break ;

	case 4://草稿箱
		main->m_pDraftDlg->ShowWindow_(DRAFT_TYPE,SMS_TYPE);
		ShowWindow(SW_HIDE);
		break ;

	case 5://设置
		main->m_pSmsSettingDlg->ShowWindow(SW_SHOW);
		ShowWindow(SW_HIDE);
		break;

	case 12://发送
		SendSMS();
		break;

	case 13://存成草稿
		SaveDraft();
		break;

	case 14://通讯录
		main->m_pContactDlg->SetType(1);//写短信
		main->m_pContactDlg->ShowRightBtn(false);
		main->m_pContactDlg->ShowWindow_();
		icon = Allicon[2];
		main->AddIcon(icon);
		break;

	case 15://通话记录
		main->m_pContactInfoDlg->SetType(1);
		main->m_pContactInfoDlg->ShowRightBtn(false);
		main->m_pContactInfoDlg->ShowWindow_();
		icon = Allicon[3];
		main->AddIcon(icon);
		break;

	case 16://常用短语
		Show10Phrases();
		break;

	case 20:// up_page
		PageSwitch(up_page);
		break;
	case 21:// down_page
		PageSwitch(down_page);
		break;

	case 30://短息内容上翻页
		SMSPageUp();
		break;
	
	case 31://短息内容下翻页
		SMSPageDown();
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

		SipShowIM(SIPF_OFF);

	}

}

void CSMSDlg::SetUpBtn(int ID)
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

void CSMSDlg::GetTelnum(Action action)
{
	//获得发信人的号码
	CString telnum;
	int temPage;
	if (action == up_page)
	{
		temPage = m_iCurrentPage+1;
	}

	if (action == down_page)
	{
		temPage = m_iCurrentPage-1;
	}
	
	if (action == current_page)
	{
		temPage = m_iCurrentPage;
	}
	

	for (int i = 0 ; i < 5 ; i++)
	{
		if ( 0 == i)
		{
			m_senderEdit1.GetWindowText(telnum);
		}

		if ( 1 == i)
		{
			m_senderEdit2.GetWindowText(telnum);
		}

		if ( 2 == i)
		{
			m_senderEdit3.GetWindowText(telnum);
		}

		if ( 3 == i)
		{
			m_senderEdit4.GetWindowText(telnum);
		}
		if ( 4 == i)
		{
			m_senderEdit5.GetWindowText(telnum);
		}

		if (!telnum.IsEmpty())
		{
			m_mapTelnum[temPage*pageSize+i] = telnum;
		}
		else
		{
			if (!m_mapTelnum[temPage*pageSize+i].IsEmpty())//该项以前有内容
			{
				m_mapTelnum[temPage*pageSize+i] = telnum;
			}
		}

		telnum.Empty();
	}

}
void CSMSDlg::PageSwitch(Action action)
{

	if (action == up_page)
	{	
		m_iCurrentPage--;
		if (m_iCurrentPage < 0)
		{
			m_iCurrentPage++;
			return ;
		}
		
		GetTelnum(action);

		m_senderEdit1.SetFocus();
		
		m_senderEdit1.SetWindowText(m_mapTelnum[m_iCurrentPage*pageSize]);
		m_senderEdit2.SetWindowText(m_mapTelnum[m_iCurrentPage*pageSize+1]);
		m_senderEdit3.SetWindowText(m_mapTelnum[m_iCurrentPage*pageSize+2]);
		m_senderEdit4.SetWindowText(m_mapTelnum[m_iCurrentPage*pageSize+3]);
		m_senderEdit5.SetWindowText(m_mapTelnum[m_iCurrentPage*pageSize+4]);
		
		//可读
		m_senderEdit2.SetReadOnly(false);
		m_senderEdit3.SetReadOnly(false);
		m_senderEdit4.SetReadOnly(false);
		m_senderEdit5.SetReadOnly(false);

	}

	if (action == down_page)
	{	
		if (SenderIsFull())
		{
			m_iCurrentPage++;
			if (m_iCurrentPage*5 > ALL_SENDER)//一个只能是20个
			{
				m_iCurrentPage--;
				return ;
			}
		}
		else
		{
			return ;
		}

		GetTelnum(action);
		
		m_senderEdit1.SetFocus();
		
		//清空
		CString empty(L"");
		m_senderEdit1.SetWindowText(empty);
		m_senderEdit2.SetWindowText(empty);
		m_senderEdit3.SetWindowText(empty);
		m_senderEdit4.SetWindowText(empty);
		m_senderEdit5.SetWindowText(empty);

		if (!m_mapTelnum[m_iCurrentPage*pageSize].IsEmpty())
		{
			m_senderEdit1.SetWindowText(m_mapTelnum[m_iCurrentPage*pageSize]);
		}

		if (!m_mapTelnum[m_iCurrentPage*pageSize+1].IsEmpty())
		{
			m_senderEdit2.SetWindowText(m_mapTelnum[m_iCurrentPage*pageSize+1]);
		}
		else
		{	
			m_senderEdit2.SetReadOnly(true);
		}

		if (!m_mapTelnum[m_iCurrentPage*pageSize+2].IsEmpty())
		{
			m_senderEdit3.SetWindowText(m_mapTelnum[m_iCurrentPage*pageSize+2]);
		}
		else
		{	
			m_senderEdit3.SetReadOnly(true);
		}

		if (!m_mapTelnum[m_iCurrentPage*pageSize+3].IsEmpty())
		{
			m_senderEdit4.SetWindowText(m_mapTelnum[m_iCurrentPage*pageSize+3]);
		}
		else
		{	
			m_senderEdit4.SetReadOnly(true);
		}

		if (!m_mapTelnum[m_iCurrentPage*pageSize+4].IsEmpty())
		{
			m_senderEdit5.SetWindowText(m_mapTelnum[m_iCurrentPage*pageSize+4]);
		}
		else
		{	
			m_senderEdit5.SetReadOnly(true);
		}

	}
}
bool CSMSDlg::SenderIsFull()
{	
	
	CString content[5] ;
	m_senderEdit1.GetWindowText(content[0]);
	m_senderEdit2.GetWindowText(content[1]);
	m_senderEdit3.GetWindowText(content[2]);
	m_senderEdit4.GetWindowText(content[3]);
	m_senderEdit5.GetWindowText(content[4]);

	if (!content[0].IsEmpty() && !content[1].IsEmpty()
		&& !content[2].IsEmpty()&& !content[3].IsEmpty()
		&& !content[4].IsEmpty())
	{
		return true ;
	}
	return false ;

}
void CSMSDlg::SetAppend(std::vector<CString> append)
{
	if (m_vTelnum.size())
	{
		m_vTelnum.clear();
	}
	m_vTelnum.swap(append);
}

void CSMSDlg::SetSender(std::vector<CString> telnum)
{	
	if (m_mapTelnum.size())
	{
		m_mapTelnum.clear();
	}
	
	m_contentEdit.SetWindowText(L"");

	m_senderEdit1.SetWindowText(L"");
	m_senderEdit2.SetWindowText(L"");
	m_senderEdit3.SetWindowText(L"");
	m_senderEdit4.SetWindowText(L"");
	m_senderEdit5.SetWindowText(L"");
	
	for (int i = 0 ; i < telnum.size() ; i++)
	{
		m_mapTelnum[i]	= telnum[i] ;

		if ( 0 == i)
		{
			m_senderEdit1.SetWindowText(telnum[i]);
		}

		if ( 1 == i)
		{
			m_senderEdit2.SetWindowText(telnum[i]);
		}

		if ( 2 == i)
		{
			m_senderEdit3.SetWindowText(telnum[i]);
		}

		if ( 3 == i)
		{
			m_senderEdit4.SetWindowText(telnum[i]);
		}

		if ( 4 == i)
		{
			m_senderEdit5.SetWindowText(telnum[i]);
		}
	}

}

void CSMSDlg::SendSMS()//短消息
{
	CMultimediaPhoneDlg *pMainDlg = ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd));
	
	if (!pMainDlg->GetSimStatus())
	{
		pMainDlg->m_pWarningNoFlashDlg->SetTitle("没插入SIM卡!");
		pMainDlg->m_pWarningNoFlashDlg->ShowWindow_(SW_SHOW);
		return ;
	}

	CString smsContent;
	m_contentEdit.GetWindowText(smsContent);

	GetTelnum(current_page);
	std::string address ;
	for (int i = 0 ; i < m_mapTelnum.size();i++)
	{
		if (!m_mapTelnum[i].IsEmpty())
		{	
			bool bname = false ;
			for (int j = 0 ; j < m_mapTelnum[i].GetLength();j++)
			{
				if (m_mapTelnum[i].GetAt(j) < '0' || m_mapTelnum[i].GetAt(j) > '9')
				{   
					bname = true ;
					break ;
				}
			}
			
			if (bname)
			{
				address += Util::StringOp::FromCString(m_vTelnum[i])+";";
			}
			else
			{
				address += Util::StringOp::FromCString(m_mapTelnum[i])+";";
			}			

		}
	}

	if (!address.empty())
	{
		SipShowIM(SIPF_OFF);
		
		m_pMessageData = boost::shared_ptr<Data::Message> (new Data::Message);
		m_pMessageData->unicodeData = Util::StringOp::FromCString(smsContent);
		
		m_pMessageData->remote.address = address;
		SYSTEMTIME tm;
		GetLocalTime(&tm);
		m_pMessageData->timestamp.year = tm.wYear%100;
		m_pMessageData->timestamp.month = tm.wMonth;
		m_pMessageData->timestamp.day = tm.wDay;
		m_pMessageData->timestamp.hour = tm.wHour;
		m_pMessageData->timestamp.minite = tm.wMinute;
		m_pMessageData->timestamp.second = tm.wSecond;
		m_pMessageData->group = Data::Message::gSend;
		m_pMessageData->Insert();
		
		//g_iSendSMSId = m_pMessageData->id();			
		pMainDlg->m_pSMSWarp->Send(m_pMessageData->remote.address, m_pMessageData->unicodeData);	
	}

}

void CSMSDlg::SaveDraft()
{	
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	CString smsContent;
	m_contentEdit.GetWindowText(smsContent);
	
	if (!smsContent.IsEmpty() || m_mapTelnum.size() > 0)
	{
		boost::shared_ptr<Data::Message> pMessage ;
		pMessage = boost::shared_ptr<Data::Message> (new Data::Message);
		
		pMessage->unicodeData = Util::StringOp::FromCString(smsContent);//短信内容
		
		std::string address ;//保存收件人，每个联系人用逗号隔开
		for (int i = 0 ; i < m_mapTelnum.size(); i++)
		{	
			address += ",";
			address += Util::StringOp::FromCString(m_mapTelnum[i]);
		}
		pMessage->remote.address = address ;
		
		SYSTEMTIME tm;
		GetLocalTime(&tm);
		pMessage->timestamp.year = tm.wYear%100;
		pMessage->timestamp.month = tm.wMonth;
		pMessage->timestamp.day = tm.wDay;
		pMessage->timestamp.hour = tm.wHour;
		pMessage->timestamp.minite = tm.wMinute;
		pMessage->timestamp.second = tm.wSecond;
		pMessage->group = Data::Message::gUnSend;
		bool bt = pMessage->Insert();
		
		CString title = L"保存成草稿成功!";
		if (!bt)
		{
			title = L"保存成草稿失败!";
		}	
		main->m_pDeleteTipDlg->SetTitle(title,3000);
		main->m_pDeleteTipDlg->ShowWindow_(SW_SHOW);
		
	}	
}

void CSMSDlg::InsertPhrases(CString phrases)
{	
	DWORD dwSel = m_contentEdit.GetSel();
	m_contentEdit.SetSel(HIWORD(dwSel), -1);
	m_contentEdit.ReplaceSel(phrases);
	
}

void CSMSDlg::SMSPageUp()
{
// 	LPARAM   lParam=   0x01490001;   
// 	WPARAM   wParam=   0x00000021;   
// 	m_contentEdit.SendMessage(WM_KEYDOWN,wParam,lParam);   
// 	m_contentEdit.SendMessage(WM_KEYUP, wParam,lParam); 

 	m_contentEdit.SendMessage(WM_VSCROLL, MAKELONG(SB_PAGEUP,0),NULL);
 	m_contentEdit.Invalidate();
	
}

void CSMSDlg::SMSPageDown()
{
// 	LPARAM   lParam=   0x01510001;
// 	WPARAM   wParam=   0x00000021;   
// 	m_contentEdit.SendMessage(WM_KEYDOWN,wParam,lParam);   
// 	m_contentEdit.SendMessage(WM_KEYUP, wParam,lParam);

 	m_contentEdit.SendMessage(WM_VSCROLL, MAKELONG(SB_PAGEDOWN,0),NULL);
 	m_contentEdit.Invalidate();

}

void CSMSDlg::Show10Phrases()
{	
	m_p10PhrasesDlg->InitialData();
	m_p10PhrasesDlg->ShowWindow_(SW_SHOW);

}

void CSMSDlg::SetSmsContent(std::string content)
{	
	if (m_mapTelnum.size())
	{
		m_mapTelnum.clear();
	}
		
	m_senderEdit1.SetWindowText(L"");
	m_senderEdit2.SetWindowText(L"");
	m_senderEdit3.SetWindowText(L"");
	m_senderEdit4.SetWindowText(L"");
	m_senderEdit5.SetWindowText(L"");

	CString scontent = Util::StringOp::ToCString(content);
	m_contentEdit.SetWindowText(scontent);
}

void CSMSDlg::ShowWindow_(int nCmdShow )
{	
	if (m_mapTelnum.size())
	{
		m_mapTelnum.clear();
	}

	m_contentEdit.SetWindowText(L"");

	m_senderEdit1.SetWindowText(L"");
	m_senderEdit2.SetWindowText(L"");
	m_senderEdit3.SetWindowText(L"");
	m_senderEdit4.SetWindowText(L"");
	m_senderEdit5.SetWindowText(L"");

	ShowWindow(nCmdShow);
}

void CSMSDlg::GetSender(std::vector<CString> &telnum)
{
	for (int i = 0 ; i < m_mapTelnum.size() ; i++)
	{
		if (!m_mapTelnum[i].IsEmpty())
		{
			telnum.push_back(m_mapTelnum[i]);
		}		
	}
}

void CSMSDlg::SetMessge(boost::shared_ptr<Data::Message> pmessage)
{
	//遍历联系人
	std::vector<CString> vcontact;//
	vcontact.resize(20);
	std::string address = pmessage->remote.address ;
	std::string contact ;
	size_t pos ;
	while (address.find(',') != std::string::npos)
	{
		pos = address.find(',');
		if (pos != std::string::npos)
		{
			address	 = address.substr(1);
			pos = address.find(',');
			if (pos != std::string::npos)
			{	
				contact	= address.substr(0,pos);
				vcontact.push_back(Util::StringOp::ToCString(contact));
				address.substr(pos);
			}
			else
			{	
				contact	= address.substr(0);
				vcontact.push_back(Util::StringOp::ToCString(contact));
				break;
			}
		}		 
	}
	
	SetSender(vcontact);

	//添加内容
	std::string content ;
	content = pmessage->unicodeData ;
	m_contentEdit.SetWindowText(Util::StringOp::ToCString(content));
	m_contentEdit.SetReadOnly(false);

}