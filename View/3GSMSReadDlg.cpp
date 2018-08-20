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
	
	m_MJPGList.Create(L"", WS_VISIBLE|WS_CHILD, CRect(0, 0, 800, 423), this);
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
		Replay();
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
		((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pMainDlg->SendMessage(WM_PLAYVIDEO, 1, 0);			//恢复视频
		((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pMainDlg->m_mainMp3Dlg_->SendMessage(WM_OUTEVENT, 0, 1);
		ShowWindow(SW_HIDE);
		main->PopbackIcon();
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
	if ( SEND_TYPE == type)
	{
		m_MJPGList.SetUnitBitmap(30,recIcon,L"",true);
	}
	else if ( RECV_TYPE == type)
	{
		m_MJPGList.SetUnitBitmap(30,sendIcon,L"",true);		
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
		}
		else
		{
			temp = Util::StringOp::ToCString(m_pMessage->remote.address);
		}
		m_MJPGList.SetUnitText(31,temp,false);//收件人或者发件人

		temp = Util::StringOp::ToCString(m_pMessage->unicodeData);
		m_MJPGList.SetUnitText(32,temp,false);//主题
		m_contentEdit.SetWindowText(temp);
		
		temp = Util::StringOp::ToCString(m_pMessage->timestamp.ToString_());
		m_MJPGList.SetUnitText(33,temp,false);//主题

		m_MJPGList.Invalidate();
	}

	if ( RECV_TYPE == m_nBoxType )
	{
		if (!(m_pMessage->state))
		{
			m_pMessage->state = Data::Message::sReaded;
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
				main->m_pMainDlg->m_p3GSMSDlg->SetSender(vtel);
				main->m_pMainDlg->m_p3GSMSDlg->SetAppend(vappend);
				main->m_pMainDlg->m_p3GSMSDlg->ShowWindow(true);
				
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
	}
	
}

void CSMSReadDlg::DialBack()
{
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd ;
	if (m_pMessage)
	{
		std::string telnum ;
		telnum = m_pMessage->remote.address;
		main->m_pTelephoneDlg->Dialback(telnum);
		main->m_pTelephoneDlg->ShowWindow(SW_SHOW);
	}
}

void CSMSReadDlg::SaveDraft()
{
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	if (m_pMessage)
	{
		boost::shared_ptr<Data::Message> pmessge ;
		pmessge = m_pMessage;
		pmessge->group = Data::Message::gUnSend ;
		bool bt = pmessge->Insert();

		CString title = L"保存成草稿成功!";
		if (!bt)
		{
			title = L"保存成草稿失败!";
		}	
		main->m_pDeleteTipDlg->SetTitle(title,3000);
		main->m_pDeleteTipDlg->ShowWindow_(SW_SHOW);
	}

}

void CSMSReadDlg::NumberExtract()
{	
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	std::vector<CString> vNumber ;
	CString content ;
	content = m_MJPGList.GetUnitText(31);
	vNumber.push_back(content);
	
	m_contentEdit.GetWindowText(content);
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