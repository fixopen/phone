// RenameDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MultimediaPhone.h"
#include "../MultimediaPhoneDlg.h"
#include "CSmsDetailDlg.h"
#include "Pblmember.h"
#include "../resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRenameDlg dialog


CSmsDetailDlg::CSmsDetailDlg(CWnd* pParent /*=NULL*/)
	: CCEDialog(CSmsDetailDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRenameDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CSmsDetailDlg::DoDataExchange(CDataExchange* pDX)
{
	CCEDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRenameDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


void CSmsDetailDlg::OnClickMJPG(WPARAM w, LPARAM l)
{
	switch(w)
	{
	case 1001:
		{

		}
		break;
	case 1000:
		Clear();
		ShowWindow_(SW_HIDE);
		break;
	}
}

BEGIN_MESSAGE_MAP(CSmsDetailDlg, CCEDialog)
	//{{AFX_MSG_MAP(CRenameDlg)
	ON_MESSAGE(WM_CLICKMJPG_TOAPP, OnClickMJPG)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRenameDlg message handlers

BOOL CSmsDetailDlg::OnInitDialog() 
{
	CCEDialog::OnInitDialog();
	
	int xbegin = 241;
	int ybegin = 52;

	m_MJPGList.Create(L"", WS_VISIBLE|WS_CHILD, CRect(xbegin, ybegin, xbegin + 317, ybegin+375), this);
	m_MJPGList.SetCurrentLinkFile(".\\adv\\mjpg\\k5\\中文\\信息详情.xml");
	m_MJPGList.SetMJPGRect(CRect(xbegin, ybegin, xbegin + 317, ybegin+375));

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSmsDetailDlg::ShowMmsDetail(boost::shared_ptr<Data::MMSData > pMmsData )
{	
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	if (pMmsData)
	{	
		m_MJPGList.SetUnitText(20,L"信息详情",false);

		//收件人或者发件人
		if (pMmsData->type == Data::MMSData::tpSend || pMmsData->type == Data::MMSData::tpUnSend)//发送类型
		{
			m_MJPGList.SetUnitText(0,L"收件人:",false);
		}
		else if (pMmsData->type == Data::MMSData::tpReceive)//接受
		{
			m_MJPGList.SetUnitText(0,L"发件人:",false);
		}
		
		CString temp;
		std::string  number ;
		std::string name ;
		if (pMmsData->type == Data::MMSData::tpSend || pMmsData->type == Data::MMSData::tpUnSend)
		{
			number = pMmsData->RecipientAddress;
			main->m_pSMSListDlg->AnalyseSender(number,name);
			
			temp = Util::StringOp::ToCString(name);
			if (temp.Mid(temp.GetLength()-1) == L";")
			{
				temp = temp.Mid(0,temp.GetLength()-1);
			}
		}
		else if (pMmsData->type == Data::MMSData::tpReceive)
		{
			number = pMmsData->SenderAddress;
		
			name = main->GetName(number);
			if (!name.empty())
			{
				temp = Util::StringOp::ToCString(name);
			}
			else
			{
				temp = 	Util::StringOp::ToCString(number);
			}		
		}

		m_MJPGList.SetUnitText(10,temp,false);//收件人或者发件人
		
		//主题
		m_MJPGList.SetUnitText(1,L"主题:",false);
		temp = Util::StringOp::ToCString(pMmsData->Subject);
		m_MJPGList.SetUnitText(11,temp,false);//主题

		//日期
		m_MJPGList.SetUnitText(2,L"日期:",false);
		CTime tm = CTime(1970, 1, 1, 0, 0, 0);
		tm += CTimeSpan(0, 0, 0, pMmsData->DateAndTime);
		WCHAR buffer[256] = {0};
		wsprintf(buffer, L"%04d-%02d-%02d", tm.GetYear(), tm.GetMonth(),tm.GetDay());
		
		temp = buffer ;
		m_MJPGList.SetUnitText(12,temp,false);//日期

		//时间
		m_MJPGList.SetUnitText(3,L"时间:",false);
		memset(buffer,256,0);
		wsprintf(buffer, L"%02d:%02d%",tm.GetHour(), tm.GetMinute());
		temp = buffer ;
		m_MJPGList.SetUnitText(13,temp,false);//日期
		
		//信息类型
		m_MJPGList.SetUnitText(4,L"类型:",false);
		m_MJPGList.SetUnitText(14,L"彩信",false);
		
		//大小
		m_MJPGList.SetUnitText(5,L"占用空间:",false);
		ULONGLONG  u = GetDirAllFileSize(Util::StringOp::ToCString(pMmsData->SavePath));
// 		if (u < 1024)
// 		{
// 			temp.Format(L"%dB",u);//只显示比特	
// 		}
// 		else
		{	
			float f = u ;
			temp.Format(L"%0.1fKB",f/1024);
		}
		m_MJPGList.SetUnitText(15,temp,false);//K

		m_MJPGList.Invalidate();
	}	
		
}

void CSmsDetailDlg::ShowSmsDetail(boost::shared_ptr< Data::Message > pMessage )
{
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	if (pMessage)
	{	
		m_MJPGList.SetUnitText(20,L"信息详情",false);

		if (pMessage->group == Data::Message::gReceive)//
		{
			m_MJPGList.SetUnitText(0,L"发件人:",false);
		}
		else if (pMessage->group == Data::Message::gSend || pMessage->group == Data::Message::gUnSend)
		{
			m_MJPGList.SetUnitText(0,L"收件人:",false);
		}

		CString temp;
		std::string number ;//在发件箱里，表示是否发送成功。收件箱里表示是否阅读			
		std::string name ;
		if (pMessage->group == Data::Message::gSend || pMessage->group == Data::Message::gUnSend)
		{
			number = pMessage->remote.address;
			main->m_pSMSListDlg->AnalyseSender(number,name);
			
			temp = Util::StringOp::ToCString(name);
			if (temp.Mid(temp.GetLength()-1) == L";")
			{
				temp = temp.Mid(0,temp.GetLength()-1);
			}
			
		}
		else
		{
			number = pMessage->remote.address;
			name = main->GetName(number);
			if (!name.empty())
			{
				temp = Util::StringOp::ToCString(name);
			}
			else
			{
				temp = 	Util::StringOp::ToCString(number);
			}
			
		}
		m_MJPGList.SetUnitText(10,temp,false);//收件人或者发件人

		//主题
		m_MJPGList.SetUnitText(1,L"主题:",false);
		temp = Util::StringOp::ToCString(pMessage->unicodeData);
		m_MJPGList.SetUnitText(11,temp,false);
		
		//日期
		m_MJPGList.SetUnitText(2,L"日期:",false);
		temp = Util::StringOp::ToCString(pMessage->timestamp.ToString_NoSecond());
		if (!temp.IsEmpty())
		{
			temp = temp.Mid(0,10);	
		}
		m_MJPGList.SetUnitText(12,temp,false);
		
		//时间
		m_MJPGList.SetUnitText(3,L"时间:",false);
		temp = Util::StringOp::ToCString(pMessage->timestamp.ToString_NoSecond());
		if (!temp.IsEmpty())
		{
			temp = temp.Mid(11);	
		}
		m_MJPGList.SetUnitText(13,temp,false);

		//类型
		m_MJPGList.SetUnitText(4,L"类型:",false);
		m_MJPGList.SetUnitText(14,L"短信",false);//时间


	}
}		

void CSmsDetailDlg::ShowContactinfoDetail(boost::shared_ptr<Data::ContactInfo > pContactInfo )
{	
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	if (pContactInfo)
	{	
		m_MJPGList.SetUnitText(20,L"通话记录详情",false);
		//日期
		CString temp;
		m_MJPGList.SetUnitText(0,L"日期:",false);

		temp.Format(_T("%04d-%02d-%02d"), pContactInfo->startTime().GetYear(),
			pContactInfo->startTime().GetMonth(), pContactInfo->startTime().GetDay());
		m_MJPGList.SetUnitText(10,temp,FALSE);//通话开始时间
		
		//时间
		m_MJPGList.SetUnitText(1,L"时间:",false);
		temp.Format(_T("%02d:%02d"), pContactInfo->startTime().GetHour(), pContactInfo->startTime().GetMinute());
		m_MJPGList.SetUnitText(11,temp,false);

		//联系人:
		m_MJPGList.SetUnitText(2,L"姓名:",false);
		//temp = Util::StringOp::ToCString(pContactInfo->name());
		temp = Util::StringOp::ToCString(main->GetName(pContactInfo->telephoneNumber().number()));
		m_MJPGList.SetUnitText(12, temp, FALSE);

		//电话号码
		m_MJPGList.SetUnitText(3,L"电话:",false);
		temp = Util::StringOp::ToCString(pContactInfo->telephoneNumber().number());
		m_MJPGList.SetUnitText(13,temp,FALSE);
		
		//类别
		m_MJPGList.SetUnitText(4,L"类别:",false);
		std::string groupName;
		int groupId = -1;
		if (!temp.IsEmpty())
		{
			main->m_pContactInfoDlg->GetName(Util::StringOp::FromCString(temp), groupId);
			groupName = main->m_pContactInfoDlg->GetType(groupId);
		}
		m_MJPGList.SetUnitText(14, Util::StringOp::ToCString(groupName),FALSE);

		//通话时长
		m_MJPGList.SetUnitText(5,L"时长:",false);
		if (pContactInfo->duration() > 3600)
		{
			temp.Format(_T("%d:%02d:%02d"), pContactInfo->duration() / 3600 ,
				pContactInfo->duration() % 3600 / 60, pContactInfo->duration() % 60);
		}
		else
		{
			temp.Format(_T("%d:%02d"), pContactInfo->duration() / 60, 
				pContactInfo->duration() % 60);
		}
		m_MJPGList.SetUnitText(15,temp,FALSE);
	}

}

void CSmsDetailDlg::Clear()
{	
	m_MJPGList.SetUnitText(20,L"",false);
	for (int i = 0 ; i < 7;i++)
	{
		m_MJPGList.SetUnitText(i,L"",false);
		m_MJPGList.SetUnitText(10+i,L"",false);
	}
	m_MJPGList.Invalidate();
}