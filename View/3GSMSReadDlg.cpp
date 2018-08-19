// YHTelDlg.cpp : implementation file
//

#include "stdafx.h"
#include "../multimediaphone.h"
#include "../MultimediaPhoneDlg.h"
#include "3GSMSReadDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// C3GSMSDlg dialog
extern void FindTP_DCS(const  char *pSrc);
extern g_isUCS2;

#define  ALL_SENDER  20


CSMSReadDlg::CSMSReadDlg(CWnd* pParent /*=NULL*/)
: CDialog(CSMSDlg::IDD, pParent)
{

}


void CSMSReadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(C3GSMSDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSMSReadDlg, CDialog)
	//{{AFX_MSG_MAP(C3GSMSDlg)
	ON_MESSAGE(WM_CLICKMJPG_TOAPP, OnClickMJPG)
//	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CSMSReadDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_contentEdit.Create(WS_CHILD|WS_VISIBLE|ES_MULTILINE | ES_AUTOVSCROLL | ES_WANTRETURN , CRect(237, 92, 237+401, 92+292), this, 0xFFFF);
	
	m_MJPGList.Create(L"", WS_VISIBLE|WS_CHILD, CRect(0, 0, 800, 423), this,10086);
	m_MJPGList.SetCurrentLinkFile(".\\adv\\mjpg\\k5\\中文\\短信详情.xml");
	m_MJPGList.SetMJPGRect(CRect(0, 0, 800, 423));
	
	MoveWindow(0,57,800,423);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
BOOL CSMSReadDlg::PreTranslateMessage(MSG* pMsg)
{

	return CDialog::PreTranslateMessage(pMsg);

}
void CSMSReadDlg::OnClickMJPG(WPARAM w, LPARAM l)
{
	CMultimediaPhoneDlg *main = ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd));
	CString icon ;
	switch(w)
	{
	case  10://回复
		if (m_pMessage->group == Data::Message::gSend)
		{
			Repeat();
		} 
		else if (m_pMessage->group == Data::Message::gReceive)
		{
			Replay();
		}
		break;
	
	case  11://回拨
		DialBack();
		break;
	
	case  12://转发
		Transit();
		break;
	
	case  13://存成草稿
		SaveDraft();
		break;

	case  14://提取号码
		NumberExtract();
		break;
	
	case  20://向上翻找内容
		PageUp();
		break;

	case  21://向下查找内容
		PageDown();
		break;

	case 1000:
		
		ShowWindow(SW_HIDE);
		main->PopbackIcon();
		main->IsSendMessage(TRUE);
		break;

	default:
	    break;

	}
}

void CSMSReadDlg::SetSMSInfo(int id,SMSBOX_TYPE type)					
{	
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	CString recIcon = L"";//收件人图标
	CString sendIcon = L".\\adv\\mjpg\\k5\\common\\短信\\发件人1.bmp";//发件人图标
	CString repeatIcon[2] = { 
		L".\\adv\\mjpg\\k5\\common\\短信\\repeat_fore.bmp",
		L".\\adv\\mjpg\\k5\\common\\短信\\repeat_back.bmp",
		};

	CString replayIcon[2] = { 
		L".\\adv\\mjpg\\k5\\common\\短信\\replay_fore.bmp",
		L".\\adv\\mjpg\\k5\\common\\短信\\replay_back.bmp",
		};

	if ( SEND_TYPE == type)
	{
		m_MJPGList.SetUnitBitmap(30,recIcon,L"",true);
		m_MJPGList.SetUnitBitmap(10,repeatIcon[0],repeatIcon[1],false);
		m_MJPGList.SetUnitIsShow(11,false);
	}
	else if ( RECV_TYPE == type)
	{
		m_MJPGList.SetUnitBitmap(30,sendIcon,L"",true);	
		m_MJPGList.SetUnitBitmap(10,replayIcon[0],replayIcon[1],false);
		m_MJPGList.SetUnitIsShow(11,true);
	}
	m_nBoxType = type ;

	//数据显示
	m_pMessage = Data::Message::GetDataById(id);
	if (m_pMessage)
	{		
		CString temp ; 
		std::string name = main->GetName(m_pMessage->remote.address);
		if (!name.empty())
		{
			temp = Util::StringOp::ToCString(name);	
			CString adr = Util::StringOp::ToCString(m_pMessage->remote.address);
			temp += L"(" + adr + L")";	
		}
		else
		{
			temp = Util::StringOp::ToCString(m_pMessage->remote.address);
		}

		m_MJPGList.SetUnitText(31,temp,false);//收件人或者发件人	
		if ( SEND_TYPE == type || DRAFT_TYPE == type)
		{	
			std::string name ;
			std::string number = Util::StringOp::FromCString(temp);
			main->m_pSMSListDlg->AnalyseSender(number,name);
			
			temp  = Util::StringOp::ToCString(name);
			if (temp.Mid(temp.GetLength()-1) == L";")
			{
				temp = temp.Mid(0,temp.GetLength()-1);
			}
			m_MJPGList.SetUnitText(31,temp,false);
		}

		temp = Util::StringOp::ToCString(m_pMessage->unicodeData);		
		m_MJPGList.SetUnitText(32,temp,false);//主题
		
		m_contentEdit.SetWindowText(L"");
		m_contentEdit.SetWindowText(temp);
		m_contentEdit.Invalidate();
		CalCulateContent(temp);

	//	temp = Util::StringOp::ToCString(m_pMessage->timestamp.ToString_());
		temp = Util::StringOp::ToCString(m_pMessage->timestamp.ToStringFormat_());
		temp = temp.Mid(2);
		m_MJPGList.SetUnitText(33,temp,false);//时间

		m_MJPGList.Invalidate();

	}

	if ( RECV_TYPE == m_nBoxType )
	{
		if (!(m_pMessage->state))
		{
			m_pMessage->state = Data::Message::sReaded;
			if (!(m_pMessage->unicodeData.empty()))
			{
				CString data = Util::StringOp::ToCString(m_pMessage->unicodeData);
				CSMSDlg::SingleQuotes(data);
				m_pMessage->unicodeData = Util::StringOp::FromCString(data);
			}
			m_pMessage->Update();			
			::SendMessage(main->m_pMainDlg->GetSafeHwnd(), WM_TELNOTIFY, 3, 0);
		}
	}

}

void CSMSReadDlg::PageDown()
{
	m_contentEdit.SendMessage(WM_VSCROLL, MAKELONG(SB_PAGEDOWN,0),NULL);
	m_contentEdit.Invalidate();
}

void CSMSReadDlg::PageUp()
{
	m_contentEdit.SendMessage(WM_VSCROLL, MAKELONG(SB_PAGEUP,0),NULL);
	m_contentEdit.Invalidate();
}

void CSMSReadDlg::Replay(void)
{
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	
	if ( RECV_TYPE == m_nBoxType )//只有收件的时候才能回复
	{	
		std::vector<CString> vtel;
		std::vector<CString> vappend;
		
		if (m_pMessage)
		{	
			std::string sender = m_pMessage->remote.address;
			CString name = m_MJPGList.GetUnitText(31);
			if (!sender.empty())
			{
				vtel.push_back(name);
				vappend.push_back(Util::StringOp::ToCString(sender));
				main->m_pMainDlg->m_p3GSMSDlg->Clear();
				main->m_pMainDlg->m_p3GSMSDlg->SetSender(vtel);
				main->m_pMainDlg->m_p3GSMSDlg->SetAppend(vappend);
				main->m_pMainDlg->m_p3GSMSDlg->ShowWindow(true);
				CWnd *p = main->m_pMainDlg->m_p3GSMSDlg;
				main->AddIcon(Allicon[1],p,false);
			}

		}			
	}

}

void CSMSReadDlg::Transit()
{
	//只能选择一个
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd ;
	
	std::string data ;
	if (m_pMessage)
	{
		data = m_pMessage->unicodeData;	
		main->m_pMainDlg->m_p3GSMSDlg->SetSmsContent(data);
		main->m_pMainDlg->m_p3GSMSDlg->ShowWindow(SW_SHOW);
		CWnd *p = main->m_pMainDlg->m_p3GSMSDlg;
		main->AddIcon(Allicon[1],p,false);
	}
	
}

void CSMSReadDlg::DialBack()
{
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd ;
	if (m_pMessage)
	{
		std::string telnum ;
		telnum = m_pMessage->remote.address;
		
		CString contact = Util::StringOp::ToCString(telnum);
		CheckChatacter(contact);
		if ( 0 == contact.Mid(0,2).Compare(L"86"))
		{
			contact = contact.Mid(2);	
		}

		main->m_pTelephoneDlg->Dialback(Util::StringOp::FromCString(contact));
		main->m_pTelephoneDlg->ShowWindow(SW_SHOW);
	}
}

void CSMSReadDlg::SaveDraft()
{
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	if (m_pMessage)
	{
		boost::shared_ptr<Data::Message> pmessge = boost::shared_ptr<Data::Message>(new Data::Message) ;
		*pmessge = *m_pMessage;

		//add 20100919
		std::string address = pmessge->remote.address;
		if (!address.empty() && 
			address.substr(address.size()-1,1).compare(";") != 0)
		{
			pmessge->remote.address = address + ";";
		}
		//

		pmessge->group = Data::Message::gUnSend ;
		bool bt = main->m_pMainDlg->m_p3GSMSDlg->SaveSmsData(pmessge,Data::Message::gUnSend);

		//	bool bt = pmessge->Insert();

		CString title = L"保存成草稿成功!";
		if (!bt)
		{
			title = L"保存成草稿失败!";
		}	
		main->m_pDeleteTipDlg->SetTitle(title,3000);
		main->m_pDeleteTipDlg->ShowWindow_(SW_SHOW);
	}

}

void CSMSReadDlg::Repeat()
{	
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	if (m_pMessage)
	{	
		boost::shared_ptr<Data::Message> pMessage = boost::shared_ptr<Data::Message>(new Data::Message);
		
		if (pMessage)
		{
			SYSTEMTIME tm;
			GetLocalTime(&tm);
			pMessage->timestamp.year = tm.wYear%100;
			pMessage->timestamp.month = tm.wMonth;
			pMessage->timestamp.day = tm.wDay;
			pMessage->timestamp.hour = tm.wHour;
			pMessage->timestamp.minite = tm.wMinute;
			pMessage->timestamp.second = tm.wSecond;
			pMessage->group = Data::Message::gSend;
			pMessage->remote.address = m_pMessage->remote.address;
			pMessage->unicodeData = m_pMessage->unicodeData;

			if (main->m_pMessageSet->SmsSendSaveSim())//发送的内容保存到SIM
			{
				pMessage->reference = 1;
				pMessage->uplevelProtocol = 200;
			}
			else 
			{
				pMessage->reference = 0;
				pMessage->uplevelProtocol = 200;	
			}
			
			//pMessage->Insert();
			if(main->m_pMessageSet->SaveSendMessage())
			{
				main->m_pMainDlg->m_p3GSMSDlg->SaveSmsData(pMessage,Data::Message::gSend);
			}
			main->m_pSMSWarp->Send(pMessage->remote.address, pMessage->unicodeData);

		}		
	}
}

void CSMSReadDlg::NumberExtract()
{	
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	std::vector<CString> vNumber ;
	CString content ;
	if (m_pMessage)
	{	
		CString name  = Util::StringOp::ToCString(m_pMessage->remote.address);	
		CheckChatacter(name);
		extern void ExtractNumber(CString content , std::vector<CString> &vnum);
		ExtractNumber(name,vNumber);
		//vNumber.push_back(name);//发件人的号码
	}
	
	m_contentEdit.GetWindowText(content);//发件内容
	void ExtractNumber(CString content , std::vector<CString> &vnum);
	if (!content.IsEmpty())
	{
		ExtractNumber(content,vNumber);
	}	

	if (vNumber.size())
	{
		main->m_pSMSListDlg->m_pNumberExtractDlg->SetNumber(vNumber);
		main->m_pSMSListDlg->m_pNumberExtractDlg->ShowNumber();
		main->m_pSMSListDlg->m_pNumberExtractDlg->ShowWindow_(SW_SHOW);
	}
}

void CSMSReadDlg::CheckChatacter(CString &ct)
{
	if (ct.Mid(ct.GetLength()-1) == L";")
	{
		ct = ct.Mid(0,ct.GetLength()-1);
	}

	if (ct.Mid(0,1) == L"+")
	{
		ct = ct.Mid(1);
	}
}

void CSMSReadDlg::RefreshName(std::string name)
{
//	m_MJPGList.SetUnitText(31, Util::StringOp::ToCString(name), TRUE);
//	m_MJPGList.Invalidate();
}

void CSMSReadDlg::CalCulateContent(CString content)
{
	int charNumber = 0;

	if(content == L"")
	{
		charNumber = m_contentEdit.GetWindowTextLength();
		m_contentEdit.GetWindowText(content);
	}
	else
	{
		charNumber = content.GetLength();
	}

	std::string str = Util::StringOp::FromCString(content);
	FindTP_DCS(str.c_str());

	int charMax = 160;
	if(g_isUCS2)
	{
		charMax = 70;
	}

	CString number;
	if(charNumber <= charMax)
	{
		number.Format(_T("%d/%d"),charNumber, charMax);
		m_MJPGList.SetUnitText(16, number, TRUE);
		m_MJPGList.SetUnitText(15, L"1", TRUE);
	}
	else
	{
		int msgNumber = 0;
		if(g_isUCS2)
		{
			charMax = 67;
		}
		else
		{
			charMax = 152;
		}
		
		if(0 == (charNumber%charMax))
		{
			msgNumber = charNumber/charMax;
		}
		else
		{
			msgNumber = charNumber/charMax + 1;
		}

		number.Format(_T("%d"), msgNumber);
		m_MJPGList.SetUnitText(15, number, TRUE);
		charNumber -= (msgNumber-1)*charMax;
		number.Format(_T("%d/%d"), charNumber, charMax);
		m_MJPGList.SetUnitText(16, number, TRUE);
	}
}