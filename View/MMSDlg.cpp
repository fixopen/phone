// YHTelDlg.cpp : implementation file
//

#include "stdafx.h"
#include "../multimediaphone.h"
#include "../MultimediaPhoneDlg.h"
#include "MMSDlg.h"
#include "sip.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// C3GSMSDlg dialog


#define  ALL_SENDER  20


CMMSDlg::CMMSDlg(CWnd* pParent /*=NULL*/)
: CDialog(CMMSDlg::IDD, pParent)
,pageSize(5)
{
	m_iCurrentPage	= 0 ;

}


void CMMSDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(C3GSMSDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMMSDlg, CDialog)
	//{{AFX_MSG_MAP(C3GSMSDlg)
	ON_MESSAGE(WM_CLICKMJPG_TOAPP, OnClickMJPG)
//	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// C3GSMSDlg message handlers


BOOL CMMSDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();


	
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
	
	m_MmsShow.Create(WS_CHILD|WS_VISIBLE, CRect(227, 64, 227+419, 64+239), this, 0, 0);
	m_MmsShow.SetMmsEdit();

	m_MJPGList.Create(L"", WS_VISIBLE|WS_CHILD, CRect(0, 0, 800, 423), this);
	m_MJPGList.SetCurrentLinkFile(".\\adv\\mjpg\\k5\\中文\\写彩信.xml");
	m_MJPGList.SetMJPGRect(CRect(0, 0, 800, 423));

	m_MJPGList.SetUnitIsDownStatus(1,true);
	
	MoveWindow(0,57,800,423);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
BOOL CMMSDlg::PreTranslateMessage(MSG* pMsg)
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

void CMMSDlg::OnClickMJPG(WPARAM w, LPARAM l)
{
	CMultimediaPhoneDlg *main = ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd));
	CString icon ;
	std::vector<CString > vnum;
	SipShowIM(SIPF_OFF);

	switch(w)
	{
	case 0:	
		GetTelnum(current_page);
		GetSender(vnum);
		main->m_pMainDlg->m_p3GSMSDlg->ShowWindow_(SW_SHOW);
		main->m_pMainDlg->m_p3GSMSDlg->SetSender(vnum);
		ShowWindow(SW_HIDE);
		break;

	case 1://写彩信
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

	case 10://发送
		SendMMS();
		break;

	case 11://存成草稿
		SaveDraft();
		break;

	case 12://预览
		Preview();
		break;

	case 13://通讯录
		main->m_pContactDlg->SetType(2);
		main->m_pContactDlg->ShowRightBtn(false);
		main->m_pContactDlg->ShowWindow_();
		icon = Allicon[2];
		main->AddIcon(icon);
		break;

	case 14://通话记录
		main->m_pContactInfoDlg->SetType(2);
		main->m_pContactInfoDlg->ShowRightBtn(false);
		main->m_pContactInfoDlg->ShowWindow_();
		icon = Allicon[3];
		main->AddIcon(icon);
		break;

	case 20://插入图片
		//m_MmsShow.OnBtnPicture();
		OnBtnPicture();
		break;
	case 21://插入音频文件
		break;

	case 22://插入视频截图
		break;

	case 23://插入视频文件
		break;
	case 24://新建一页
		break;

	case 30://彩信内容上一页
		m_MmsShow.OnBtnUp();
		break;
	case 31://彩信内容下一页
		m_MmsShow.OnBtnNext();
		break;
	
	case 50://联系人的上一页
		break;
	case 51://联系人的下一页
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

void CMMSDlg::SetUpBtn(int ID)
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

void CMMSDlg::GetTelnum(Action action)
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

void CMMSDlg::PageSwitch(Action action)
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
bool CMMSDlg::SenderIsFull()
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

void CMMSDlg::SetAppend(std::vector<CString> append)
{
	if (m_vTelnum.size())
	{
		m_vTelnum.clear();
	}
	m_vTelnum.swap(append);
}

void CMMSDlg::SetSender(std::vector<CString> telnum)
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

void CMMSDlg::ShowWindow_(int nCmdShow )
{	
	if (m_mapTelnum.size())
	{
		m_mapTelnum.clear();
	}
	m_MmsShow.SetMmsEdit();
		
	m_senderEdit1.SetWindowText(L"");
	m_senderEdit2.SetWindowText(L"");
	m_senderEdit3.SetWindowText(L"");
	m_senderEdit4.SetWindowText(L"");
	m_senderEdit5.SetWindowText(L"");
	
	ShowWindow(nCmdShow);
}

void CMMSDlg::SendMMS()//发送彩信
{
	CMultimediaPhoneDlg *pMainDlg = ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd));
	
	if (!pMainDlg->GetSimStatus())
	{
		pMainDlg->m_pWarningNoFlashDlg->SetTitle("没插入SIM卡!");
		pMainDlg->m_pWarningNoFlashDlg->ShowWindow_(SW_SHOW);
		return ;
	}

	GetTelnum(current_page);
	
	MMS::MMSWarp::MMS_LAYOUT layout;
	std::vector<MMS::MMSWarp::MMS_PAR> pars;
	std::wstring sTitle;
	BOOL ret = m_MmsShow.GetParInfo(pars, layout, sTitle);//获得par里的内容
	
	std::string address ;
	for (int i = 0 ; i < m_mapTelnum.size();i++)
	{
		if (!m_mapTelnum[i].IsEmpty() && ret)
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
		SYSTEMTIME tm;
		GetLocalTime(&tm);
		CTime ct = CTime(tm);
		CTimeSpan sp = ct - CTime(1970, 1, 1, 8, 0, 0);
		char datetime[24];
		char sDir[64];
		sprintf(datetime, "%04d%02d%02d%02d%02d%02d", tm.wYear, tm.wMonth, tm.wDay, tm.wHour, tm.wMinute, tm.wSecond);
		sprintf(sDir, "/flashdrv/3g/mms/%s/", datetime);
		CString sDir_ = sDir;
		CreateDirectory((LPTSTR)(LPCTSTR)sDir_, NULL);
		
		MMS::MMSWarp::GetMMSWarp()->EncodeSmil(pars, layout, sDir);
		
		m_pMMSData = boost::shared_ptr<Data::MMSData> (new Data::MMSData);
		m_pMMSData->type = Data::MMSData::tpReadySend;
		m_pMMSData->isRead = false; //表示发送失败
		m_pMMSData->TransactionId = datetime;
		m_pMMSData->RecipientAddress = address; //To-value = Encoded-string-value
		m_pMMSData->SenderAddress = pMainDlg->m_sMobileNumber; //Optional From-value = Value-length (Address-present-token Encoded-string-value | Insert-address-token )Address-present-token = <Octet 128>Insert-address-token = <Octet 129>
		m_pMMSData->Subject = MMS::MMSWarp::GetMMSWarp()->FromUnicode(sTitle);   //存进去应该是 GB2312
		//m_pMMSData->Subject = Util::StringOp::ToUTF8(MMS::MMSWarp::GetMMSWarp()->FromUnicode(sTitle));
		m_pMMSData->DateAndTime = sp.GetSeconds(); //Date-value = Long-integer	In seconds from 1970-01-01, 00:00:00 GMT.
		m_pMMSData->SavePath = sDir;
		m_pMMSData->isRead = TRUE;
		m_pMMSData->Insert();

	}
}

void CMMSDlg::SaveDraft()
{	
	CMultimediaPhoneDlg *pMainDlg = ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd));
	GetTelnum(current_page);
	
	MMS::MMSWarp::MMS_LAYOUT layout;
	std::vector<MMS::MMSWarp::MMS_PAR> pars;
	std::wstring sTitle;
	BOOL ret = m_MmsShow.GetParInfo(pars, layout, sTitle);//获得par里的内容
	
	if (!m_mapTelnum.empty() && ret)
	{
		SipShowIM(SIPF_OFF);
		
		SYSTEMTIME tm;
		GetLocalTime(&tm);
		CTime ct = CTime(tm);
		CTimeSpan sp = ct - CTime(1970, 1, 1, 8, 0, 0);
		char datetime[24];
		char sDir[64];
		sprintf(datetime, "%04d%02d%02d%02d%02d%02d", tm.wYear, tm.wMonth, tm.wDay, tm.wHour, tm.wMinute, tm.wSecond);
		sprintf(sDir, "/flashdrv/3g/mms/%s/", datetime);
		CString sDir_ = sDir;
		CreateDirectory((LPTSTR)(LPCTSTR)sDir_, NULL);
		
		MMS::MMSWarp::GetMMSWarp()->EncodeSmil(pars, layout, sDir);
		
		m_pMMSData = boost::shared_ptr<Data::MMSData> (new Data::MMSData);
		m_pMMSData->type = Data::MMSData::tpUnSend;
		m_pMMSData->isRead = false; //表示发送失败
		m_pMMSData->TransactionId = datetime;
		
		std::string RecipientAddress ;
		for (int i = 0 ; i < m_mapTelnum.size() ;i++)
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
				RecipientAddress += Util::StringOp::FromCString(m_vTelnum[i])+";";
			}
			else
			{
				RecipientAddress += Util::StringOp::FromCString(m_mapTelnum[i])+";";
			}

			RecipientAddress += Util::StringOp::FromCString(m_mapTelnum[i]); //To-value = Encoded-string-value
			RecipientAddress += ";";
		}	
		m_pMMSData->RecipientAddress = RecipientAddress;

		m_pMMSData->SenderAddress = pMainDlg->m_sMobileNumber; //Optional From-value = Value-length (Address-present-token Encoded-string-value | Insert-address-token )Address-present-token = <Octet 128>Insert-address-token = <Octet 129>
		m_pMMSData->Subject = MMS::MMSWarp::GetMMSWarp()->FromUnicode(sTitle);   //存进去应该是 GB2312
		//m_pMMSData->Subject = Util::StringOp::ToUTF8(MMS::MMSWarp::GetMMSWarp()->FromUnicode(sTitle));
		m_pMMSData->DateAndTime = sp.GetSeconds(); //Date-value = Long-integer	In seconds from 1970-01-01, 00:00:00 GMT.
		m_pMMSData->SavePath = sDir;
		m_pMMSData->isRead = TRUE;
		bool bt = m_pMMSData->Insert();

		CString title = L"保存成草稿成功!";
		if (!bt)
		{
			title = L"保存成草稿失败!";
		}	
		pMainDlg->m_pDeleteTipDlg->SetTitle(title,3000);
		pMainDlg->m_pDeleteTipDlg->ShowWindow_(SW_SHOW);
		
	}
		
}

void CMMSDlg::GetSender(std::vector<CString> &telnum)
{
	for (int i = 0 ; i < m_mapTelnum.size() ; i++)
	{
		if (!m_mapTelnum[i].IsEmpty())
		{
			telnum.push_back(m_mapTelnum[i]);
		}		
	}
}

void CMMSDlg::Preview()
{
	
	CMultimediaPhoneDlg *pMainDlg = ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd));
	GetTelnum(current_page);
	
	MMS::MMSWarp::MMS_LAYOUT layout;
	std::vector<MMS::MMSWarp::MMS_PAR> pars;
	std::wstring sTitle;
	BOOL ret = m_MmsShow.GetParInfo(pars, layout, sTitle);//获得par里的内容
	
	if (!m_mapTelnum.empty() && ret)
	{
		SipShowIM(SIPF_OFF);
		
		SYSTEMTIME tm;
		GetLocalTime(&tm);
		CTime ct = CTime(tm);
		CTimeSpan sp = ct - CTime(1970, 1, 1, 8, 0, 0);
		char datetime[24];
		char sDir[64];
		sprintf(datetime, "%04d%02d%02d%02d%02d%02d", tm.wYear, tm.wMonth, tm.wDay, tm.wHour, tm.wMinute, tm.wSecond);
		sprintf(sDir, "/flashdrv/3g/mms/%s/", datetime);
		CString sDir_ = sDir;
		CreateDirectory((LPTSTR)(LPCTSTR)sDir_, NULL);
		
		MMS::MMSWarp::GetMMSWarp()->EncodeSmil(pars, layout, sDir);
		
		std::wstring path = sDir_ ;
		pMainDlg->m_pSMSListDlg->m_pMmsReadDlg->MMSPreview(path,sTitle);
		pMainDlg->m_pSMSListDlg->m_pMmsReadDlg->ShowWindow(SW_SHOW);

	}

}

void CMMSDlg::SetMmsContent(boost::shared_ptr<Data::MMSData> pmmsdata,bool baddname)
{
	std::wstring title ;
	title = Util::StringOp::ToCString(pmmsdata->Subject);
	m_MmsShow.SetTitle(title);
	
	std::wstring path ;
	path = Util::StringOp::ToCString(pmmsdata->SavePath);
	m_MmsShow.SetTransit(path);
	
	if (baddname)
	{
		std::vector<CString> vNumber ;
		std::string address = pmmsdata->RecipientAddress ;
		std::string contact ;
		
		if (!address.empty())
		{
			while (address.find(';') != std::string::npos)
			{	
				size_t pos ;
				pos = address.find(';');
				if (pos != std::string::npos)
				{	
					contact	= address.substr(0,pos);
					vNumber.push_back(Util::StringOp::ToCString(contact));
					address.substr(pos+1);
				}
				
			}
		}

		SetSender(vNumber);
	}

}

void CMMSDlg::OnBtnPicture()
{
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	
	main->m_pMainDlg->m_p3GHomePicDlg->ShowWindow_(SW_SHOW,false);
	main->m_pMainDlg->m_p3GHomePicDlg->m_callType = 1;
	main->AddIcon(Allicon[6]);

}

void CMMSDlg::InsertPicture(CString path)
{	
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd ;
	CFile file;
	if (file.Open(path,CFile::modeRead,NULL))
	{
		ULONGLONG dwLength = file.GetLength();
		if (dwLength > (48*1024))//文件小于48K
		{
			main->m_pWarningNoFlashDlg->SetTitle(L"图片太大,不能插入!");
			main->m_pWarningNoFlashDlg->ShowWindow_(SW_SHOW);
			return;			
		}
		file.Close();
	}
	m_MmsShow.FromListName(path);
}