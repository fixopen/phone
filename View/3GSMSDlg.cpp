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
BOOL g_IsSignal = FALSE;
BOOL g_isSimEnable = FALSE;
int  g_SignalSize = 0;
extern void FindTP_DCS(const  char *pSrc);
extern g_isUCS2;
extern BOOL FindVideoDlg(BOOL flag);

#define  ALL_SENDER  20

CSMSDlg::CSMSDlg(CWnd* pParent /*=NULL*/)
: CDialog(CSMSDlg::IDD, pParent)
,pageSize(5)
{
	m_iCurrentPage	= 0 ;
	m_msgNumber		= 1 ;
	m_charNumber	= 0	;
	m_charMax		= 160;
	m_nInsesrtIndex	= -1;
	m_vTelnum.resize(20);
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
	ON_MESSAGE(WM_CHARNUMBER, OnCharNumberChange)

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

	
	int height;
	m_senderEdit1.Create(WS_CHILD|WS_VISIBLE |ES_AUTOHSCROLL , CRect(28, 120, 28+189, 120+29), this, 0xFFFF);
	height = 120 + 29 + 11;
	m_senderEdit2.Create(WS_CHILD|WS_VISIBLE |ES_AUTOHSCROLL, CRect(28, height, 28+189, height+29), this, 0xFFFF);
	height += 29 + 11;
	m_senderEdit3.Create(WS_CHILD|WS_VISIBLE |ES_AUTOHSCROLL, CRect(28, height, 28+189, height+29), this, 0xFFFF);
	height += 29 + 12;
	m_senderEdit4.Create(WS_CHILD|WS_VISIBLE |ES_AUTOHSCROLL, CRect(28, height, 28+189, height+29), this, 0xFFFF);
	height += 29 + 11;
	m_senderEdit5.Create(WS_CHILD|WS_VISIBLE |ES_AUTOHSCROLL, CRect(28, height, 28+189, height+29), this, 0xFFFF);
	
	m_senderEdit2.SetReadOnly(TRUE);
	m_senderEdit3.SetReadOnly(TRUE);
	m_senderEdit4.SetReadOnly(TRUE);
	m_senderEdit5.SetReadOnly(TRUE);

	m_contentEdit.Create(WS_CHILD|WS_VISIBLE|ES_MULTILINE | ES_AUTOVSCROLL | ES_WANTRETURN , CRect(239, 125, 239+396, 125+260), this, 0xFFFF);
	
	m_p10PhrasesDlg = new C10PhrasesDlg();
	m_p10PhrasesDlg->Create(C10PhrasesDlg::IDD);
	m_p10PhrasesDlg->ShowWindow(SW_HIDE);

	m_MJPGList.Create(L"", WS_VISIBLE|WS_CHILD, CRect(0, 0, 800, 423), this,10086);
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
		CString content1;
		if (pMsg->hwnd == m_senderEdit2.m_hWnd)
		{
			m_senderEdit1.GetWindowText(content);
			m_senderEdit2.GetWindowText(content1);
			if (!content1.IsEmpty())
			{
				m_senderEdit2.SetReadOnly(false);
			}
			else if (!content.IsEmpty())
			{
				m_senderEdit2.SetReadOnly(false);
			}
		}
		if (pMsg->hwnd == m_senderEdit3.m_hWnd)
		{
			m_senderEdit2.GetWindowText(content);
			m_senderEdit3.GetWindowText(content1);
			if (!content1.IsEmpty())
			{
				m_senderEdit3.SetReadOnly(false);
			}
			else if (!content.IsEmpty())
			{
				m_senderEdit3.SetReadOnly(false);
			}
		}		
		if (pMsg->hwnd == m_senderEdit4.m_hWnd)
		{
			m_senderEdit3.GetWindowText(content);
			m_senderEdit4.GetWindowText(content1);
			if (!content1.IsEmpty())
			{
				m_senderEdit4.SetReadOnly(false);
			}
			else if (!content.IsEmpty())
			{
				m_senderEdit4.SetReadOnly(false);
			}
		}		
		if (pMsg->hwnd == m_senderEdit5.m_hWnd)
		{
			m_senderEdit4.GetWindowText(content);
			m_senderEdit5.GetWindowText(content1);
			if (!content1.IsEmpty())
			{
				m_senderEdit5.SetReadOnly(false);
			}
			else if (!content.IsEmpty())
			{
				m_senderEdit5.SetReadOnly(false);
			}
		}
	}
	return CDialog::PreTranslateMessage(pMsg);

}

void CSMSDlg::OnCharNumberChange()
{
	m_charNumber = m_contentEdit.GetWindowTextLength();
	CString str;
	m_contentEdit.GetWindowText(str);
	std::string content = Util::StringOp::FromCString(str);
	FindTP_DCS(content.c_str());

	if(g_isUCS2)
	{
		m_charMax = 70;
	}
	else
	{
		m_charMax = 160;
	}

	CString number;
	if(m_charNumber <= m_charMax)
	{
		number.Format(_T("%d/%d"), m_charMax-m_charNumber, m_charMax);
		m_MJPGList.SetUnitText(8, number, TRUE);
		m_MJPGList.SetUnitText(7, L"1", TRUE);
	}
	else
	{
		if(g_isUCS2)
		{
			m_charMax = 67;
		}
		else
		{
			m_charMax = 152;
		}

		if(0 == (m_charNumber%m_charMax))
		{
			m_msgNumber = m_charNumber/m_charMax;
		}
		else
		{
			m_msgNumber = m_charNumber/m_charMax + 1;
		}

		number.Format(_T("%d"), m_msgNumber);
		m_MJPGList.SetUnitText(7, number, TRUE);
		m_charNumber -= (m_msgNumber-1)*m_charMax;
		number.Format(_T("%d/%d"), m_charMax-m_charNumber, m_charMax);
		m_MJPGList.SetUnitText(8, number, TRUE);
	}
}

void CSMSDlg::OnClickMJPG(WPARAM w, LPARAM l)
{
	CMultimediaPhoneDlg *main = ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd));
	CWnd *p;
	CString icon ;
	std::vector<CString> vnum;
	SipShowIM(SIPF_OFF);
	switch(w)
	{
	case 0:
		break;

	case 1://写彩信
		GetTelnum(current_page);
		GetSender(vnum);
		main->m_pMMSDlg->ShowWindow_(SW_SHOW);
		main->AddIcon(Allicon[1],main->m_pMMSDlg,false);
		main->m_pMMSDlg->TransferSender(vnum);
		main->m_pMMSDlg->TransferTelnum(m_vTelnum);
		main->PopIcon(this);
		break;

	case 2://收件箱
		main->m_pSMSListDlg->ShowWindow_(RECV_TYPE,SMS_TYPE);
		main->PopIcon(this);
//		ShowWindow(SW_HIDE);
//		main->PopbackIcon(this);
		break ;

	case 3://发件箱
		main->m_pSMSListDlg->ShowWindow_(SEND_TYPE,SMS_TYPE);
		main->PopIcon(this);
// 		ShowWindow(SW_HIDE);
// 		main->PopbackIcon(this);
		break ;

	case 4://草稿箱
		main->m_pDraftDlg->ShowWindow_(DRAFT_TYPE,SMS_TYPE);
		main->PopIcon(this);
// 		ShowWindow(SW_HIDE);
// 		main->PopbackIcon(this);
		break ;

	case 5://设置
		{
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
				if (main->m_bSearchNetWin)
				{
					main->AddIcon(Allicon[1],main->m_pSmsSettingDlg,false);
					main->PopIcon(this);
				}

				main->m_pSmsSettingDlg->ShowWindow_(SW_SHOW);
			}
		}
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
		p =main->m_pContactDlg;
		main->AddIcon(icon,p,false);
		break;

	case 15://通话记录
		main->m_pContactInfoDlg->SetType(1);
		main->m_pContactInfoDlg->ShowRightBtn(false);
		main->m_pContactInfoDlg->ShowWindow_();
		icon = Allicon[3];
		p = main->m_pContactInfoDlg;
		main->AddIcon(icon,p,false);
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
	case 1001:
		{	
			CString smsContent;
			m_contentEdit.GetWindowText(smsContent);

			GetTelnum(current_page);
			std::string address ;
			FindNumber(address);
			if (!address.empty() || !smsContent.IsEmpty())//有信息就保存
			{	
				SaveDraft(false);
			}

			Clear();
			ShowWindow(SW_HIDE);
			main->PopbackIcon();
			main->IsSendMessage(FALSE);
		}
		break;
	default:
	    break;

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
			if (!m_mapTelnum[temPage*pageSize+i].IsEmpty())
			{
				if (m_mapTelnum[temPage*pageSize+i] != telnum)
				{
					m_vTelnum[temPage*pageSize+i] = L"";
				}
			}
			m_mapTelnum[temPage*pageSize+i] = telnum ;
		}
		else
		{
			if (!m_mapTelnum[temPage*pageSize+i].IsEmpty())//该项以前有内容
			{
				m_mapTelnum[temPage*pageSize+i] = telnum;
				m_vTelnum[temPage*pageSize+i] = L"";
			}
		}

		telnum.Empty();
	}

}
void CSMSDlg::PageSwitch(Action action)
{
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	if (action == up_page)
	{	
		m_iCurrentPage--;
		if (m_iCurrentPage < 0)
		{
			m_iCurrentPage++;
			return ;
		}
		
		GetTelnum(action);
		
		//清空
		CString empty(L"");
		m_senderEdit1.SetWindowText(empty);
		m_senderEdit2.SetWindowText(empty);
		m_senderEdit3.SetWindowText(empty);
		m_senderEdit4.SetWindowText(empty);
		m_senderEdit5.SetWindowText(empty);

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
			if (m_iCurrentPage*5 >= ALL_SENDER)//一共只能是20个
			{
				m_iCurrentPage--;
				main->m_pWarningNoFlashDlg->SetTitle(L"已经达到最大联系人数");
				main->m_pWarningNoFlashDlg->ShowWindow_(SW_SHOW);
				return ;
			}
		}
		else
		{
			return ;
		}

		GetTelnum(action);
		
		
		//清空
		CString empty(L"");
		m_senderEdit1.SetWindowText(empty);
		m_senderEdit2.SetWindowText(empty);
		m_senderEdit3.SetWindowText(empty);
		m_senderEdit4.SetWindowText(empty);
		m_senderEdit5.SetWindowText(empty);

		m_senderEdit1.SetFocus();

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

	SetPage();

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
	if (m_nInsesrtIndex > -1)
	{
		for (int i = 0 ; i < append.size() ;i++)//插入新的联系人
		{
			m_vTelnum[m_nInsesrtIndex++] = append[i];
			if (m_nInsesrtIndex >= ALL_SENDER)
			{	
				break;
			}	
		}
	}

	m_nInsesrtIndex = -1;

}

void CSMSDlg::SetSender(std::vector<CString> telnum)
{	
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;

	if (  0 == telnum.size())
	{
		return;
	}
	
	GetTelnum(current_page);

	for (int i = 0 ; i < ALL_SENDER;i++)
	{
		if (m_mapTelnum[i].IsEmpty())
		{
			m_nInsesrtIndex = i ;
			break;
		}
	}
	
	int index = m_nInsesrtIndex;

	if (m_nInsesrtIndex > -1)
	{
		for (i = 0 ; i < telnum.size() ;i++)//插入新的联系人
		{
			m_mapTelnum[index++] = telnum[i];
			if ( index >= ALL_SENDER)
			{	
				break;
			}
		}
	}
	else 
	{
		main->m_pWarningNoFlashDlg->SetTitle("联系人已满!");
		main->m_pWarningNoFlashDlg->ShowWindow_(SW_SHOW);
		return;
	}
	
	//清空
	CString empty(L"");
	m_senderEdit1.SetWindowText(empty);
	m_senderEdit2.SetWindowText(empty);
	m_senderEdit3.SetWindowText(empty);
	m_senderEdit4.SetWindowText(empty);
	m_senderEdit5.SetWindowText(empty);

	int ibegin ;
	int iend;
	int count = 0 ;
	for (i = 0 ; i < m_mapTelnum.size() ;i++)
	{
		if (!m_mapTelnum[i].IsEmpty())
		{	
			count++;
		}
	}

	ibegin = count - (count%5 == 0 ? 5:count%5);
	iend   = (count - ibegin) > 5 ? 5 : count - ibegin;
	
	m_iCurrentPage = (count%5 == 0)?count/5-1:count/5;

// 	if (count > (m_iCurrentPage+1)*5)
// 	{
// 		m_iCurrentPage += (count - (m_iCurrentPage+1)*5)/5 +1;
// 	}
	
	for ( i = ibegin ; i < (ibegin + iend) ; i++)
	{	
		int id = i % 5;
		if ( 0 == id)
		{
			m_senderEdit1.SetWindowText(m_mapTelnum[i]);
		}

		if ( 1 == id)
		{
			m_senderEdit2.SetWindowText(m_mapTelnum[i]);
		}

		if ( 2 == id)
		{
			m_senderEdit3.SetWindowText(m_mapTelnum[i]);
		}

		if ( 3 == id)
		{
			m_senderEdit4.SetWindowText(m_mapTelnum[i]);
		}

		if ( 4 == id)
		{
			m_senderEdit5.SetWindowText(m_mapTelnum[i]);
		}

	}

	SetPage();
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
	
	if (!g_IsSignal)//无网络信号
	{
		pMainDlg->m_pWarningNoFlashDlg->SetTitle("无网络信号!");
		pMainDlg->m_pWarningNoFlashDlg->ShowWindow_(SW_SHOW);
		return ;
	}

	if (!(pMainDlg->m_bSearchNetWin))
	{	
		pMainDlg->m_pWarningNoFlashDlg->SetTitle("网路未注册!");
		pMainDlg->m_pWarningNoFlashDlg->ShowWindow_(SW_SHOW);
		return ;
	}

	CString smsContent;
	m_contentEdit.GetWindowText(smsContent);

	GetTelnum(current_page);
	std::string address ;
	FindNumber(address);

	if (address.empty())//联系人是空的
	{	
		pMainDlg->m_pWarningNoFlashDlg->SetTitle("请输入联系人!");
		pMainDlg->m_pWarningNoFlashDlg->ShowWindow_(SW_SHOW);
		return;
	}

	if (!address.empty())
	{
		SipShowIM(SIPF_OFF);
		
		m_pMessageData = boost::shared_ptr<Data::Message> (new Data::Message);

		SingleQuotes(smsContent);
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
		
		if (pMainDlg->m_pMessageSet->SmsSendSaveSim())//发送的内容保存到SIM
		{
			m_pMessageData->reference = 1;
			m_pMessageData->uplevelProtocol = 200;
		}
		else 
		{
			m_pMessageData->reference = 0;
			m_pMessageData->uplevelProtocol = 200;	
		}
		
		m_pMessageData->state = Data::Message::sNoRead;
		m_pMessageData->group = Data::Message::gSend;
		
		if(pMainDlg->m_pMessageSet->SaveSendMessage())
		{	
			////已发信息不保存
			SaveSmsData(m_pMessageData,Data::Message::gSend);
		}
		
		pMainDlg->m_pSMSWarp->Send(m_pMessageData->remote.address, m_pMessageData->unicodeData);	
		
		OnBtnCancel();
	}

}

void CSMSDlg::SaveDraft(bool tip )
{	
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	
	GetTelnum(current_page);

	CString smsContent;
	m_contentEdit.GetWindowText(smsContent);
	
	if (!smsContent.IsEmpty() || m_mapTelnum.size() > 0)
	{
		boost::shared_ptr<Data::Message> pMessage ;
		pMessage = boost::shared_ptr<Data::Message> (new Data::Message);
		
		SingleQuotes(smsContent);
		pMessage->unicodeData = Util::StringOp::FromCString(smsContent);//短信内容
		
		std::string address ;//保存收件人，每个联系人用逗号隔开
		FindNumber(address);

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
		bool bt = SaveSmsData(pMessage,Data::Message::gUnSend);
	//	bool bt = pMessage->Insert();
		
		if (tip)
		{
			CString title = L"保存成草稿成功!";
			if (!bt)
			{
				title = L"保存成草稿失败!";
			}	
			main->m_pTipDlg->SetTitle(title,3000);
			main->m_pTipDlg->ShowWindow_(SW_SHOW);
		}

		//
	
	}	
}

void CSMSDlg::InsertPhrases(CString phrases)
{	
	DWORD dwSel = m_contentEdit.GetSel();
	m_contentEdit.SetSel(HIWORD(dwSel), -1);
	m_contentEdit.ReplaceSel(phrases);
	OnCharNumberChange();
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
	ShowWindow(nCmdShow);
	Sleep(100);
	OnCharNumberChange();
	m_MJPGList.SetUnitText(40,L"1/1",true);
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
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	std::vector<CString> vcontact;//
	std::vector<CString> vappend;//全是号码
	std::string address = pmessage->remote.address ;
	std::string contact ;
	size_t pos ;
	while (address.find(';') != std::string::npos)
	{
		pos = address.find(';');
		if (pos != std::string::npos)
		{	
			contact	= address.substr(0,pos);
			vappend.push_back(Util::StringOp::ToCString(contact));//号码
			
			//去联系人查找姓名
			std::string name = main->GetName(contact);
			if (!name.empty())
			{
				vcontact.push_back(Util::StringOp::ToCString(name));//姓名						
			}
			else
			{
				vcontact.push_back(Util::StringOp::ToCString(contact));//号码
			}
			
			address = address.substr(pos+1);

		}
				 
	}
	
	SetSender(vcontact);
	SetAppend(vappend);

	//添加内容
	std::string content ;
	content = pmessage->unicodeData ;
	m_contentEdit.SetWindowText(Util::StringOp::ToCString(content));
	m_contentEdit.SetReadOnly(false);

}

LRESULT CSMSDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
	case WM_SMS_SENDRESULT:
		if(lParam == 1)
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
			
			m_msgNumber = 1;
			m_charNumber = 0;
			m_MJPGList.SetUnitText(7, L"1", TRUE);
			m_MJPGList.SetUnitText(8, L"0/70", TRUE);
		}
		break;
	case CHECK_SUPPERPASSWORD:
		if(1 == wParam)
		{
			((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_bLogin = TRUE;
			((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSmsSettingDlg->ShowWindow_(SW_SHOW);
			ShowWindow(SW_HIDE);
		}
		break;
	}
	return CDialog::WindowProc(message, wParam, lParam);
}

void CSMSDlg::Clear()
{	
	if (m_mapTelnum.size())
	{
		m_mapTelnum.clear();
	}

	if (m_vTelnum.size())
	{
		m_vTelnum.clear();
		m_vTelnum.resize(20);
	}
	
	m_iCurrentPage = 0 ;

	m_senderEdit1.SetWindowText(L"");
	m_senderEdit2.SetWindowText(L"");
	m_senderEdit3.SetWindowText(L"");
	m_senderEdit4.SetWindowText(L"");
	m_senderEdit5.SetWindowText(L"");
	
	m_contentEdit.SetWindowText(L"");

	m_MJPGList.SetUnitText(40,L"1/1",true);
}


void CSMSDlg::TransferSender(std::vector<CString> Sender)
{
	//清空
	if (m_mapTelnum.size())
	{
		m_mapTelnum.clear();
	}
	m_iCurrentPage = 0 ;

	CString empty(L"");
	m_senderEdit1.SetWindowText(empty);
	m_senderEdit2.SetWindowText(empty);
	m_senderEdit3.SetWindowText(empty);
	m_senderEdit4.SetWindowText(empty);
	m_senderEdit5.SetWindowText(empty);
	
	for (int i = 0 ; i < Sender.size();i++)
	{
		m_mapTelnum[i] = Sender[i];
	}
	
	int size = (Sender.size() > 5?5:Sender.size());
	for (i = 0 ;i < size ; i++)
	{
		if ( 0 == i)
		{
			m_senderEdit1.SetWindowText(Sender[i]);
		}
		if ( 1 == i)
		{
			m_senderEdit2.SetWindowText(Sender[i]);
		}
		if ( 2 == i)
		{
			m_senderEdit3.SetWindowText(Sender[i]);
		}
		if ( 3 == i)
		{
			m_senderEdit4.SetWindowText(Sender[i]);
		}
		if ( 4 == i)
		{
			m_senderEdit5.SetWindowText(Sender[i]);
		}
	}
	
	SetPage();
}

void CSMSDlg::TransferTelnum(std::vector<CString> telnum)
{
	if (m_vTelnum.size())
	{
		m_vTelnum.clear();
	}

	for (int i = 0 ; i < telnum.size() ;i++)
	{
		m_vTelnum.push_back(telnum[i]);
	}
}

void CSMSDlg::OnBtnCancel()
{	
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	Clear();
	ShowWindow(SW_HIDE);
	main->PopbackIcon();
}

int CSMSDlg::SmsBoxCount(Data::Message::Group group)
{	
	std::string filter = "[group] =";//发件箱
	filter += Util::StringOp::FromInt(group);
	
	return Data::Message::GetDataCount(filter);
//	std::vector<boost::shared_ptr<Data::Message> > vmessage;
//	vmessage = Data::Message::GetFromDatabase(filter);
//	return vmessage.size();
}

bool CSMSDlg::SaveSmsData(boost::shared_ptr<Data::Message> pmessage,Data::Message::Group group)
{
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	bool bin;
	if (pmessage)
	{	
		int ncount = SmsBoxCount(group);
		switch (group)
		{
			case Data::Message::gReceive:
					{
						if (ncount < RECEIVE_BOX_SIZE)
						{
							bin = pmessage->Insert();
						}
						else
						{
							FindVideoDlg(TRUE);
							main->m_pWarningNoFlashDlg->SetTitle(L"短信收件箱已满,请及时删除");	
							main->m_pWarningNoFlashDlg->ShowWindow_(SW_SHOW);
							
							//删除以前最早添加的
							bin = pmessage->Insert();
							SmsDelOldest(group);

							main->m_pSMSListDlg->RefreshList(SMS_TYPE);

						}
					}
					break;
			case Data::Message::gSend:
					{
						if (ncount < SEND_BOX_SIZE)
						{
							bin = pmessage->Insert();
						}
						else
						{
							main->m_pWarningNoFlashDlg->SetTitle(L"短信发件箱已满,请及时删除");	
							main->m_pWarningNoFlashDlg->ShowWindow_(SW_SHOW);

							bin = pmessage->Insert();
							SmsDelOldest(group);

							main->m_pSMSListDlg->RefreshList(SMS_TYPE);

						}
					}
					break;
			case Data::Message::gUnSend:
					{
						if (ncount < DRAFT_BOX_SIZE)
						{
							bin = pmessage->Insert();
						}
						else
						{
							main->m_pWarningNoFlashDlg->SetTitle(L"短信草稿箱已满,请及时删除");	
							main->m_pWarningNoFlashDlg->ShowWindow_(SW_SHOW);

							bin = pmessage->Insert();
							SmsDelOldest(group);

							main->m_pDraftDlg->FreshList();

						}
					}
					break;

		}	
	}
	return bin ;
}

void CSMSDlg::SaveSmsData(Data::Message *pmessage)
{	
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	int ncount = SmsBoxCount(Data::Message::gReceive);
	if (ncount < RECEIVE_BOX_SIZE)
	{
		pmessage->Insert();
	}
	else
	{
		FindVideoDlg(TRUE);
		main->m_pWarningNoFlashDlg->SetTitle(L"短信收件箱已满,请及时删除",10000);	
		main->m_pWarningNoFlashDlg->ShowWindow_(SW_SHOW);
		
		//删除以前最早添加的
		pmessage->Insert();
		SmsDelOldest(Data::Message::gReceive);

		main->m_pSMSListDlg->RefreshList(SMS_TYPE);

	}
}

void CSMSDlg::SmsDelOldest(Data::Message::Group group)
{
	std::string filter = "[group] =";
	filter += Util::StringOp::FromInt(group);
	std::vector<boost::shared_ptr<Data::Message> > vmessage;
	vmessage = Data::Message::GetFromDatabase(filter,Data::dUp,0,1);
	if (vmessage.size())
	{
		vmessage[0]->Remove();
	}
}

void CSMSDlg::FindNumber(std::string &adr)
{
	/*for (int i = 0 ; i < m_mapTelnum.size();i++)
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
				if ( 0 == m_vTelnum[i].GetLength())
				{	
					return;
				}
				adr += Util::StringOp::FromCString(m_vTelnum[i])+";";
			}
			else
			{
				adr += Util::StringOp::FromCString(m_mapTelnum[i])+";";
			}		
			
		}
	}*/

	
	for (int i = 0 ; i < m_mapTelnum.size();i++)
	{
		if (!m_mapTelnum[i].IsEmpty())
		{	
			if (!m_vTelnum[i].IsEmpty())
			{
				adr += Util::StringOp::FromCString(m_vTelnum[i])+";";
			}
			else
			{
				adr += Util::StringOp::FromCString(m_mapTelnum[i])+";";
			}	
		}
	}
}

void CSMSDlg::SetPage()
{	
	int count = 0 ;
	int allPage = 0 ;
	for (int i = 0 ; i < m_mapTelnum.size() ;i++)
	{
		if (!m_mapTelnum[i].IsEmpty())
		{	
			count++;	
		}
	}

	allPage = ( count%5 == 0 ?(count/5):(count/5 +1));

	if ( (m_iCurrentPage +1) > allPage)//向下翻页的情况
	{
		allPage++ ;
	}

	CString page ;
	page.Format(L"%d/%d",m_iCurrentPage+1,allPage);
	m_MJPGList.SetUnitText(40,page,true);

}

void CSMSDlg::SingleQuotes(CString &content)
{
	std::string str = Util::StringOp::FromCString(content);
	size_t index = 0;
	size_t pos = 0;
	while(str.find("'") != std::string::npos)
	{
		char *subStr = (char*)str.c_str();
		char *len = strstr(subStr, "'");
		index = len - subStr;
		pos += index;
		content.Insert(pos, _T("'"));
		str = str.substr(index+1);
		pos += 2;
	}
}