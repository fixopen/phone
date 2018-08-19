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

extern HANDLE g_hEvent;
#define  ALL_SENDER  20
#define MMS_MAX_PARSIZE 20


CMMSDlg::CMMSDlg(CWnd* pParent /*=NULL*/)
: CDialog(CMMSDlg::IDD, pParent)
,pageSize(5)
{
	m_iCurrentPage	= 0 ;
	m_vTelnum.resize(20);
	m_nInsesrtIndex = -1;
	m_MmsSize = 0;
	n = 0 ;
	m_currentPage = 1;
	m_pageCount = 1;
	m_picCount = 0;
	m_mp3Count = 0;
	m_isMid = FALSE;
	m_nSelVideo = 0;
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
	m_senderEdit1.Create(WS_CHILD|WS_VISIBLE|ES_AUTOHSCROLL, CRect(28, 120, 28+189, 120+29), this, 0xFFFF);
	height = 120 + 29 + 11;
	m_senderEdit2.Create(WS_CHILD|WS_VISIBLE|ES_AUTOHSCROLL, CRect(28, height, 28+189, height+29), this, 0xFFFF);
	height += 29 + 11;
	m_senderEdit3.Create(WS_CHILD|WS_VISIBLE|ES_AUTOHSCROLL, CRect(28, height, 28+189, height+29), this, 0xFFFF);
	height += 29 + 12;
	m_senderEdit4.Create(WS_CHILD|WS_VISIBLE|ES_AUTOHSCROLL, CRect(28, height, 28+189, height+29), this, 0xFFFF);
	height += 29 + 11;
	m_senderEdit5.Create(WS_CHILD|WS_VISIBLE|ES_AUTOHSCROLL, CRect(28, height, 28+189, height+29), this, 0xFFFF);
	
	m_senderEdit2.SetReadOnly(TRUE);
	m_senderEdit3.SetReadOnly(TRUE);
	m_senderEdit4.SetReadOnly(TRUE);
	m_senderEdit5.SetReadOnly(TRUE);
	
	m_MmsShow.Create(WS_CHILD|WS_VISIBLE, CRect(227, 64, 227+419, 64+268), this, 0, 0);
	m_MmsShow.SetMmsEdit();
	m_MmsShow.SetpWnd(this);

	m_MJPGList.Create(L"", WS_VISIBLE|WS_CHILD, CRect(0, 0, 800, 423), this,10086);
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
	CWnd *p;
	std::vector<CString > vnum;
	SipShowIM(SIPF_OFF);

	switch(w)
	{
	case 0:	
		GetTelnum(current_page);
		GetSender(vnum);
		main->m_pMainDlg->m_p3GSMSDlg->ShowWindow_(SW_SHOW);
		main->AddIcon(Allicon[1],main->m_pMainDlg->m_p3GSMSDlg,false);
		main->m_pMainDlg->m_p3GSMSDlg->TransferSender(vnum);
		main->m_pMainDlg->m_p3GSMSDlg->TransferTelnum(m_vTelnum);
		main->PopIcon(this);
		break;

	case 1://写彩信
		break;

	case 2://收件箱
		main->m_pSMSListDlg->ShowWindow_(RECV_TYPE,SMS_TYPE);
		main->PopIcon(this);
		break ;

	case 3://发件箱
		main->m_pSMSListDlg->ShowWindow_(SEND_TYPE,SMS_TYPE);
		main->PopIcon(this);
		break ;

	case 4://草稿箱
		main->m_pDraftDlg->ShowWindow_(DRAFT_TYPE,SMS_TYPE);
		main->PopIcon(this);
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
				main->m_pMmsSettingDlg->ShowWindow_(SW_SHOW);
				main->AddIcon(Allicon[1],main->m_pMmsSettingDlg,false);
				main->PopIcon(this);
			}
		}
		break;

	case 10://发送
		m_MmsShow.StopVideo();
		SendMMS();
		break;

	case 11://存成草稿
		m_MmsShow.StopVideo();
		SaveDraft();
		break;

	case 12://预览
		Preview();
		break;

	case 13://通讯录
		main->m_pContactDlg->SetType(2);
		main->m_pContactDlg->ShowRightBtn(false);
		main->m_pContactDlg->ShowWindow_();
		p = main->m_pContactDlg;
		icon = Allicon[2];
		main->AddIcon(icon,p,false);
		break;

	case 14://通话记录
		main->m_pContactInfoDlg->SetType(2);
		main->m_pContactInfoDlg->ShowRightBtn(false);
		main->m_pContactInfoDlg->ShowWindow_();
		p = main->m_pContactInfoDlg;
		icon = Allicon[3];
		main->AddIcon(icon,p,true);
		break;

	case 20://插入图片
		OnBtnPicture();
		break;

	case 21://插入音频文件
		OnBtnAudio();
		break;

	case 22://插入视频截图
		break;

	case 23://插入视频文件
		break;

	case 30://彩信内容上一页
		m_MmsShow.StopVideo();
		StopAudio();
		m_MmsShow.OnBtnUp();
		break;

	case 31://彩信内容下一页
		m_MmsShow.StopVideo();
		if(m_currentPage < MMS_MAX_PARSIZE-1)
		{
			StopAudio();
			m_MmsShow.OnBtnNext();
		}
		else
		{
			CString title = L"信息已超过最大页数";
			main->m_pWarningNoFlashDlg->SetTitle(title);
			main->m_pWarningNoFlashDlg->ShowWindow_(TRUE);
		}
		break;

	case 32://彩信插入
		if(m_MmsShow.m_cMmsPar.size() < MMS_MAX_PARSIZE && m_pageCount < MMS_MAX_PARSIZE)
		{
			m_MmsShow.SetMmsInsert();
		}
		else
		{
			CString title = L"信息已超过最大页数";
			main->m_pWarningNoFlashDlg->SetTitle(title);
			main->m_pWarningNoFlashDlg->ShowWindow_(TRUE);
		}
		break;

	case 40://试听
		OnBtnPlayAudio();
		break;
	
	case 50://联系人的上一页
		PageSwitch(up_page);
		break;
		
	case 51://联系人的下一页
		PageSwitch(down_page);
		break;

	case 1000:
	case 1001:
		{	
			m_MmsShow.StopVideo();
			SaveDraft(false);//有数据就保存						
			Clear();
			ShowWindow(SW_HIDE);
			//wangzhenxing20100610
			if(m_nAudioOperate == stop)  //如果正在试听音频则关闭
			{
				StopAudio();
				m_nAudioOperate = play;
			}
			main->PopbackIcon();
			main->IsSendMessage(FALSE);
			break;
		}
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
			if (!m_mapTelnum[temPage*pageSize+i].IsEmpty())
			{
				if (m_mapTelnum[temPage*pageSize+i] != telnum)
				{
					m_vTelnum[temPage*pageSize+i] = L"";
				}
			}
			m_mapTelnum[temPage*pageSize+i] = telnum;
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

void CMMSDlg::PageSwitch(Action action)
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
		
		m_senderEdit1.SetFocus();

		//清空
		CString empty(L"");
		m_senderEdit1.SetWindowText(empty);
		m_senderEdit2.SetWindowText(empty);
		m_senderEdit3.SetWindowText(empty);
		m_senderEdit4.SetWindowText(empty);
		m_senderEdit5.SetWindowText(empty);
		
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
			if (m_iCurrentPage*5 >= ALL_SENDER)//一个只能是20个
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

	SetPage();
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

void CMMSDlg::SetSender(std::vector<CString> telnum)
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
		main->m_pWarningNoFlashDlg->SetTitle(L"联系人已满!");
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

void CMMSDlg::TransferSender(std::vector<CString> Sender)
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

void CMMSDlg::TransferTelnum(std::vector<CString> telnum)
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

void CMMSDlg::ShowWindow_(int nCmdShow )
{
	m_picCount = 0;
	m_mp3Count = 0;

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
	
	CString image[2][2] = {
		{
			L".\\adv\\mjpg\\k5\\common\\短信\\插入图片_fore.bmp",
			L".\\adv\\mjpg\\k5\\common\\短信\\插入图片_back.bmp",
		},
		{
			L".\\adv\\mjpg\\k5\\common\\短信\\插入音频文件_fore.bmp",
			L".\\adv\\mjpg\\k5\\common\\短信\\插入音频文件_back.bmp",
		}
	};
	m_MJPGList.SetUnitBitmap(20,image[0][0],image[0][1],false);
	m_MJPGList.SetUnitBitmap(21,image[1][0],image[1][1],false);
	m_MJPGList.SetUnitIsShow(40,false,false);

	m_nImageAction = insertImage ;
	m_nAudioAction = insertAudio ;

	m_MJPGList.SetUnitText(60,L"1/1",false);

	m_MJPGList.SetUnitText(24,L"当前页:1/1",false);

	ShowWindow(nCmdShow);
	Sleep(100);
	m_MmsShow.AllFileSize();

	m_MJPGList.Invalidate();
	m_currentPage = 1;
	m_pageCount = 1;
}

void CMMSDlg::SendMMS()//发送彩信
{
	CMultimediaPhoneDlg *pMainDlg = ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd));
	extern  BOOL g_IsSignal;

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

//	if (pMainDlg->m_nTELRigster < TELRIGSTER_TD)
	if (!(pMainDlg->m_bSearchNetWin))
	{	
		pMainDlg->m_pWarningNoFlashDlg->SetTitle("网络未注册!");
		pMainDlg->m_pWarningNoFlashDlg->ShowWindow_(SW_SHOW);
		return ;
	}
	
	//判断磁盘空间
	if (pMainDlg->IsDiskFull())
	{
		return;
	}

	GetTelnum(current_page);
	
	MMS::MMSWarp::MMS_LAYOUT layout;
	std::vector<MMS::MMSWarp::MMS_PAR> pars;
	std::wstring sTitle;
	BOOL ret = m_MmsShow.GetParInfo(pars, layout, sTitle);//获得par里的内容
	
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
		SYSTEMTIME tm;
		GetLocalTime(&tm);
		CTime ct = CTime(tm);
		CTimeSpan sp = ct - CTime(1970, 1, 1, 0, 0, 0);
		char datetime[24];
		char sDir[64];
		sprintf(datetime, "%04d%02d%02d%02d%02d%02d", tm.wYear, tm.wMonth, tm.wDay, tm.wHour, tm.wMinute, tm.wSecond);
		sprintf(sDir, "/flashdrv/3g/mms/%s/", datetime);
		CString sDir_ = sDir;
		CreateDirectory((LPTSTR)(LPCTSTR)sDir_, NULL);
		
		MMS::MMSWarp::GetMMSWarp()->EncodeSmil(pars, layout, sDir);
		
		int mmsSetSize = GetMMSMaxSize();
		CString str = L"";
		if(m_picCount > 0 || m_mp3Count > 0)
		{
			str.Format(L"%0.1fK", m_MmsSize+0.05);
		}
		else
		{
			str.Format(L"%0.1fK", m_MmsSize);
		}
		str = L"发送的彩信息大小为" + str;
		if((m_MmsSize+0.05)*1024 < 50*1024)
		{
			pMainDlg->m_pDeleteTipDlg->SetTitle(str, 10*1000,1);
			pMainDlg->m_pDeleteTipDlg->SetHWnd(this->GetSafeHwnd());
			pMainDlg->m_pDeleteTipDlg->ShowWindow_(TRUE);
		}
		else if((m_MmsSize+0.05)*1024 > (float)mmsSetSize*1024)
		{
			str = L"信息过大!无法发送!";
			pMainDlg->m_pWarningNoFlashDlg->SetTitle(str);
			pMainDlg->m_pWarningNoFlashDlg->SetHWnd(this->GetSafeHwnd());
			pMainDlg->m_pWarningNoFlashDlg->ShowWindow_(TRUE);
			return;
		}
		else
		{
			str += L"对方可能无法接收!是否继续发送!";
			pMainDlg->m_pDeleteTipDlg->SetTitle(str, 10*1000,1);
			pMainDlg->m_pDeleteTipDlg->SetHWnd(this->GetSafeHwnd());
			pMainDlg->m_pDeleteTipDlg->ShowWindow_(TRUE);
		}
		
		//lxz 20100828
	    pMainDlg->SetPreADSLDial();

		m_pMMSData = boost::shared_ptr<Data::MMSData> (new Data::MMSData);
		m_pMMSData->type = Data::MMSData::tpReadySend;
		m_pMMSData->isRead = false; //表示发送失败
		m_pMMSData->TransactionId = datetime;
		m_pMMSData->RecipientAddress = address; //To-value = Encoded-string-value
		m_pMMSData->SenderAddress = pMainDlg->m_sMobileNumber; //Optional From-value = Value-length (Address-present-token Encoded-string-value | Insert-address-token )Address-present-token = <Octet 128>Insert-address-token = <Octet 129>
		m_pMMSData->Subject = MMS::MMSWarp::GetMMSWarp()->FromUnicode(sTitle);   //存进去应该是 GB2312
		//m_pMMSData->Subject = Util::StringOp::ToUTF8(MMS::MMSWarp::GetMMSWarp()->FromUnicode(sTitle));
		m_pMMSData->DateAndTime = sp.GetTotalSeconds(); //Date-value = Long-integer	In seconds from 1970-01-01, 00:00:00 GMT.
		m_pMMSData->SavePath = sDir;
		m_pMMSData->repeatCount = 0;

		m_pMMSData->localSendTime.year = tm.wYear - 2000;
		m_pMMSData->localSendTime.month = tm.wMonth;
		m_pMMSData->localSendTime.day = tm.wDay;
		m_pMMSData->localSendTime.hour = tm.wHour;
		m_pMMSData->localSendTime.minite = tm.wMinute;
		m_pMMSData->localSendTime.second = tm.wSecond;

		if(1 == pMainDlg->m_pMessageSet->MmsValidity())
		{
			m_pMMSData->TimeOfExpiry = 24*3600;
		}
		else
		{
			m_pMMSData->TimeOfExpiry = 48*3600;
		}
	}
}

void CMMSDlg::SaveDraft(bool tip)
{	
	CMultimediaPhoneDlg *pMainDlg = ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd));
	GetTelnum(current_page);
	
	MMS::MMSWarp::MMS_LAYOUT layout;
	std::vector<MMS::MMSWarp::MMS_PAR> pars;
	std::wstring sTitle;
	BOOL ret = m_MmsShow.GetParInfo(pars, layout, sTitle);//获得par里的内容
	
	if (!m_mapTelnum.empty() || !sTitle.empty() || pars.size())
	{
		SipShowIM(SIPF_OFF);
		
		SYSTEMTIME tm;
		GetLocalTime(&tm);
		CTime ct = CTime(tm);
		CTimeSpan sp = ct - CTime(1970, 1, 1, 0, 0, 0);
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
		FindNumber(RecipientAddress);
		
		m_pMMSData->RecipientAddress = RecipientAddress;

		m_pMMSData->SenderAddress = pMainDlg->m_sMobileNumber; //Optional From-value = Value-length (Address-present-token Encoded-string-value | Insert-address-token )Address-present-token = <Octet 128>Insert-address-token = <Octet 129>
		m_pMMSData->Subject = MMS::MMSWarp::GetMMSWarp()->FromUnicode(sTitle);   //存进去应该是 GB2312
		//m_pMMSData->Subject = Util::StringOp::ToUTF8(MMS::MMSWarp::GetMMSWarp()->FromUnicode(sTitle));
		m_pMMSData->DateAndTime = sp.GetTotalSeconds(); //Date-value = Long-integer	In seconds from 1970-01-01, 00:00:00 GMT.
		m_pMMSData->SavePath = sDir;
		m_pMMSData->repeatCount = 0;

		m_pMMSData->localSendTime.year = tm.wYear - 2000;
		m_pMMSData->localSendTime.month = tm.wMonth;
		m_pMMSData->localSendTime.day = tm.wDay;
		m_pMMSData->localSendTime.hour = tm.wHour;
		m_pMMSData->localSendTime.minite = tm.wMinute;
		m_pMMSData->localSendTime.second = tm.wSecond;

		if(1 == pMainDlg->m_pMessageSet->MmsValidity())
		{
			m_pMMSData->TimeOfExpiry = 24*3600;
		}
		else
		{
			m_pMMSData->TimeOfExpiry = 48*3600;
		}

		bool bt = SaveMmsData(m_pMMSData,Data::MMSData::tpUnSend);
	//	bool bt = m_pMMSData->Insert();
		
		if (tip)
		{
			CString title = L"保存成草稿成功!";
			if (!bt)
			{
				title = L"保存成草稿失败!";
			}	
			pMainDlg->m_pDeleteTipDlg->SetTitle(title,3000);
			pMainDlg->m_pDeleteTipDlg->ShowWindow_(SW_SHOW);
		}
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
	
	std::string address ;
	FindNumber(address);

	if(m_nAudioOperate == stop)///wangzhenxing20100611
	{
		StopAudio();
		m_nAudioOperate = play;
	}

	if (ret)
	{
		SipShowIM(SIPF_OFF);
		
		SYSTEMTIME tm;
		GetLocalTime(&tm);
		CTime ct = CTime(tm);
		CTimeSpan sp = ct - CTime(1970, 1, 1, 0, 0, 0);
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
		pMainDlg->AddIcon(Allicon[1], pMainDlg->m_pSMSListDlg->m_pMmsReadDlg, false);

		//wangzhenxing20100610
		m_pMMSData = boost::shared_ptr<Data::MMSData> (new Data::MMSData);
		m_pMMSData->type = Data::MMSData::tpReadySend;
		m_pMMSData->isRead = false; //表示发送失败
		m_pMMSData->TransactionId = datetime;
		m_pMMSData->RecipientAddress = address;
		m_pMMSData->SenderAddress = pMainDlg->m_sMobileNumber; //Optional From-value = Value-length (Address-present-token Encoded-string-value | Insert-address-token )Address-present-token = <Octet 128>Insert-address-token = <Octet 129>
		m_pMMSData->Subject = MMS::MMSWarp::GetMMSWarp()->FromUnicode(sTitle);   //存进去应该是 GB2312
		m_pMMSData->DateAndTime = sp.GetTotalSeconds(); //Date-value = Long-integer	In seconds from 1970-01-01, 00:00:00 GMT.
		m_pMMSData->SavePath = sDir;
		m_pMMSData->repeatCount = 0;

		pMainDlg->m_pSMSListDlg->m_pMmsReadDlg->m_pMMSData = m_pMMSData;
	}
	else
	{	
		pMainDlg->m_pWarningNoFlashDlg->SetTitle(L"没有内容可以预览");
		pMainDlg->m_pWarningNoFlashDlg->ShowWindow_(SW_SHOW);
		return;
	}
}

void CMMSDlg::SetMmsContent(boost::shared_ptr<Data::MMSData> pmmsdata,bool baddname)
{	
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	
	//创建一个新的目录
	SYSTEMTIME tm;
	GetLocalTime(&tm);
	CTime ct = CTime(tm);
	char datetime[24];
	char sDir[64];
	sprintf(datetime, "%04d%02d%02d%02d%02d%02d", tm.wYear, tm.wMonth, tm.wDay, tm.wHour, tm.wMinute, tm.wSecond);
	sprintf(sDir, "/flashdrv/3g/mms/%s/", datetime);
	CString sDir_ = sDir;
	CreateDirectory((LPTSTR)(LPCTSTR)sDir_, NULL);

//	std::wstring path ;
	CString path;
	path = Util::StringOp::ToCString(pmmsdata->SavePath);
	
	//把以前的目录下数据，拷贝到一个新的目录
	CopyData(path,sDir_);
	
	//m_MmsShow.SetTransit(path);
	m_MmsShow.SetpWnd(this);
	m_MmsShow.SetTransit(std::wstring(sDir_));
	m_MmsShow.GetPicMp3Count();

	std::wstring title ;
	title = Util::StringOp::ToCString(pmmsdata->Subject);
	m_MmsShow.SetTitle(title);

	if (baddname)
	{
		std::vector<CString> vcontact;//
		std::vector<CString> vappend;//全是号码

		std::string address = pmmsdata->RecipientAddress ;
		std::string contact ;
		
		if (!address.empty())
		{
			while (address.find(';') != std::string::npos)
			{	
// 				size_t pos ;
// 				pos = address.find(';');
// 				if (pos != std::string::npos)
// 				{	
// 					contact	= address.substr(0,pos);
// 					vNumber.push_back(Util::StringOp::ToCString(contact));
// 					address = address.substr(pos+1);
// 				}

				size_t pos = address.find(';');
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
		}

		SetSender(vcontact);
		SetAppend(vappend);
	}

}

void CMMSDlg::CopyData(CString srcpath,CString despath)
{	
	CString	str = srcpath;
	str = str + L"*" ;
	WIN32_FIND_DATA fd;
	HANDLE hfind;
	hfind	  =	 FindFirstFile(str,&fd);
	if(hfind !=	 INVALID_HANDLE_VALUE)
	{
		do
		{
			if(fd.dwFileAttributes==FILE_ATTRIBUTE_DIRECTORY)
			{ 
				//如果是文件夹
			}
			else
			{ 
				//如果是文件
				CString strfl = fd.cFileName ;
				CopyFile(srcpath+fd.cFileName,despath+fd.cFileName,false);
			}
		}
		while(FindNextFile(hfind,&fd));
	}
}

void CMMSDlg::OnBtnPicture()
{
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	if ( insertImage == m_nImageAction)//插入图片
	{
		if(m_nSelVideo == 0)
		{
			main->m_pMainDlg->m_p3GHomePicDlg->ShowWindow_(SW_SHOW,false);
			main->m_pMainDlg->m_p3GHomePicDlg->m_callType = 1;
			CWnd *p = main->m_pMainDlg->m_p3GHomePicDlg;
			main->AddIcon(Allicon[6],p,false);
		}
		else
		{
			main->m_pMainDlg->m_p3GHomeMovieDlg->ShowWindow_(SW_SHOW);
			main->m_pMainDlg->m_p3GHomeMovieDlg->m_callType = 1;
			CWnd *p = main->m_pMainDlg->m_p3GHomeMovieDlg;
			main->AddIcon(Allicon[5],p,false);
		}
	}
	else if (deleteImage == m_nImageAction)//删除图片
	{
		CFile file;
		if (file.Open(m_MmsShow.m_cImagePath,CFile::modeRead,NULL))
		{
			double dwLength = file.GetLength();
			m_MmsSize -= dwLength/1024;
			file.Close();
		}
		m_MmsShow.ClosePicture();
		m_picCount--;
	}

	if(m_MmsSize < 0)
	{
		m_MmsSize = 0;
	}
	SetMmsSize(m_MmsSize);
}

void CMMSDlg::OnBtnAudio()
{
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	if ( insertAudio == m_nAudioAction)//插入音频
	{	
		main->m_pMainDlg->m_p3GHomeJoyDlg->m_callType = 1;
		main->m_pMainDlg->m_p3GHomeJoyDlg->ShowWindow_(SW_SHOW,false);
		CWnd *p = main->m_pMainDlg->m_p3GHomeJoyDlg;
		main->AddIcon(Allicon[4],p,false);
	}
	else if (deleteAudio == m_nAudioAction)//删除音频
	{
		CFile file;
		if (file.Open(m_MmsShow.m_cAudioName,CFile::modeRead,NULL))
		{
			double dwLength = file.GetLength();
			m_MmsSize -= dwLength/1024;
			file.Close();
		}
		m_MmsShow.DeleteAudio();
		m_mp3Count--;
	}

	if(m_MmsSize < 0)
	{
		m_MmsSize = 0;
	}
	SetMmsSize(m_MmsSize);
}

void CMMSDlg::OnBtnPlayAudio()
{
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	
	CString audioIcon[2][2] = {
		{
			L".\\adv\\mjpg\\k5\\common\\短信\\试听_fore.bmp",
			L".\\adv\\mjpg\\k5\\common\\短信\\试听_back.bmp",
		},
		{
			L".\\adv\\mjpg\\k5\\common\\短信\\停止试听_fore.bmp",
			L".\\adv\\mjpg\\k5\\common\\短信\\停止试听_back.bmp",
		}
	};

	if (stop == m_nAudioOperate)
	{	
		StopAudio()	;	
		m_nAudioOperate = play;
		m_MJPGList.SetUnitBitmap(40,audioIcon[0][0],audioIcon[0][1],true);
		m_MJPGList.SetUnitIsShow(40,true,true);
		main->m_pMainDlg->StopVideo(TRUE, 1, 0); //恢复播放
	}
	else if (play == m_nAudioOperate)
	{
		if (main->m_pMainDlg->m_mainMp3Dlg_->PlayAudio(m_MmsShow.GetAudio()))
		{
			main->m_pMainDlg->StopVideo(TRUE, 0, 0);  //试听时先暂停原来的音乐
			m_nAudioOperate = stop;
			m_MJPGList.SetUnitBitmap(40,audioIcon[1][0],audioIcon[1][1],true);
			m_MJPGList.SetUnitIsShow(40,true,true);
	
		}		
	}

}

void CMMSDlg::StopAudio()
{
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	if(m_nAudioOperate == stop || main->m_pSMSListDlg->m_pMmsReadDlg->m_isReadMid)
	{
		if(m_isMid)
		{
			main->m_pMainDlg->m_mainMp3Dlg_->pTheMidiPlayer->StopPlay();  //停止时把两种播放器都关掉
			main->m_pMainDlg->m_mainMp3Dlg_->pTheMidiPlayer->CloseSong();
			main->m_pMainDlg->m_mainMp3Dlg_->m_isMidiMusic = FALSE;
			m_isMid = FALSE;
		}
		else
		{
			main->playeraudio_->StopPlayer();
			HWND hWnd = ::FindWindow(L"csplayer_win1", L"csplayer window1");
			if(hWnd != NULL)
			{
				main->playeraudio_->ExitPlayer();
			}
		}
		main->playeraudio_->SetVolume(main->m_pMainDlg->m_mainMp3Dlg_->m_Volume);
		main->m_pMainDlg->m_mainMp3Dlg_->pTheMidiPlayer->SetVolume(main->m_pMainDlg->m_mainMp3Dlg_->m_Volume*15);
		main->m_pMainDlg->m_mainMp3Dlg_->ChangeVolume(main->m_pMainDlg->m_mainMp3Dlg_->m_Volume + 50);
	}
}

void CMMSDlg::InsertPicture(CString path)
{	
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd ;
	CFile file;
	if (file.Open(path,CFile::modeRead,NULL))
	{
		int maxSize = GetMMSMaxSize();
		double dwLength = file.GetLength();
		if(dwLength > maxSize*1024)
		{
			main->m_pWarningNoFlashDlg->SetTitle(L"图片超过彩信最大容量,无法发送!");
			main->m_pWarningNoFlashDlg->ShowWindow_(SW_SHOW);
			return;
		}
		else if (dwLength > (50*1024))//文件大于50K
		{
			main->m_pWarningNoFlashDlg->SetTitle(L"图片过大,对方可能无法接收!");
			main->m_pWarningNoFlashDlg->ShowWindow_(SW_SHOW);			
		}
		file.Close();
		m_MmsSize += dwLength/1024;
		m_picCount++;
	}
	m_MmsShow.FromListName(path);
	SetMmsSize(m_MmsSize);
}

void CMMSDlg::InsertAudio(CString path)
{	
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	CFile file;
	if (file.Open(path,CFile::modeRead,NULL))
	{
		int maxSize = GetMMSMaxSize();
		double dwLength = file.GetLength();
		if(dwLength > maxSize*1024)
		{
			main->m_pWarningNoFlashDlg->SetTitle(L"音频文件超过彩信最大容量,无法发送!");
			main->m_pWarningNoFlashDlg->ShowWindow_(SW_SHOW);
			return;
		}
		else if (dwLength > (50*1024))//文件大于50K
		{
			main->m_pWarningNoFlashDlg->SetTitle(L"音频过大，对方可能无法接收");
			main->m_pWarningNoFlashDlg->ShowWindow_(SW_SHOW);			
		}
		file.Close();
		m_MmsSize += dwLength/1024;
		m_mp3Count++;
	}
	m_MmsShow.InsertAudio(path);
	SetMmsSize(m_MmsSize);
}

void CMMSDlg::SetMmsSize(double size)
{
	CString str;
	int maxSize = GetMMSMaxSize();
	if(size < 0)
	{
		size = 0;
	}
	double size_ = size;
	if(m_picCount > 0 || m_mp3Count > 0)
		size_ += 0.05;
	str.Format(L"%0.1fK/%dK", size_, maxSize);
	m_MJPGList.SetUnitText(22, str, TRUE);
}

int CMMSDlg::GetMMSMaxSize()
{
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	int size = 0;
	if(1 == main->m_pMessageSet->MmsMaxSize())
	{
		size = 50;
	}
	else if(2 == main->m_pMessageSet->MmsMaxSize())
	{
		size = 100;
	}
	else if(3 == main->m_pMessageSet->MmsMaxSize())
	{
		size = 300;
	}
	
	return size;
}

LRESULT CMMSDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_IMAGE:
		{
			CString imageIcon[2][2] = {
				{
					L".\\adv\\mjpg\\k5\\common\\短信\\删除图片_fore.bmp",
					L".\\adv\\mjpg\\k5\\common\\短信\\删除图片_back.bmp",
				},
				{
					L".\\adv\\mjpg\\k5\\common\\短信\\插入图片_fore.bmp",
					L".\\adv\\mjpg\\k5\\common\\短信\\插入图片_back.bmp",
				}			
			};
			
			if ( 0 == wParam)//删除图片
			{
				m_nImageAction = deleteImage;
				m_MJPGList.SetUnitBitmap(20,imageIcon[0][0],imageIcon[0][1],true);
		
			}
			else if (1 == wParam)//插入图片
			{
				m_nImageAction = insertImage;
				m_MJPGList.SetUnitBitmap(20,imageIcon[1][0],imageIcon[1][1],true);
			}		
		}		
		break;
		
	case WM_AUDIO:
		{
			CString audioIcon[3][2] = {
				{
					L".\\adv\\mjpg\\k5\\common\\短信\\删除音频文件_fore.bmp",
					L".\\adv\\mjpg\\k5\\common\\短信\\删除音频文件_back.bmp",
				},
				{
					L".\\adv\\mjpg\\k5\\common\\短信\\插入音频文件_fore.bmp",
					L".\\adv\\mjpg\\k5\\common\\短信\\插入音频文件_back.bmp",
				},
				{
					L".\\adv\\mjpg\\k5\\common\\短信\\试听_fore.bmp",
					L".\\adv\\mjpg\\k5\\common\\短信\\试听_back.bmp",
				}

			};

			if ( 0 == wParam)//删除音频
			{
				m_nAudioAction = deleteAudio;
				m_MJPGList.SetUnitBitmap(21,audioIcon[0][0],audioIcon[0][1],true);
			}
			else if (1 == wParam)//插入音频
			{
				m_nAudioAction = insertAudio;
				m_MJPGList.SetUnitBitmap(21,audioIcon[1][0],audioIcon[1][1],true);	
			}
			else if (3 == wParam)//试听按钮出现
			{
				m_nAudioOperate = play;
				m_MJPGList.SetUnitBitmap(40,audioIcon[2][0],audioIcon[2][1],true);
				m_MJPGList.SetUnitIsShow(40,true,true);
			}
			else if (5 == wParam)//试听按钮消失
			{	
				StopAudio();
				m_MJPGList.SetUnitBitmap(40,L"",L"",true);
				m_MJPGList.SetUnitIsShow(40,false,true);
			}

		}
		break;
	case CHECK_SUPPERPASSWORD:
		if(1 == wParam)
		{
			((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_bLogin = TRUE;
			((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pMmsSettingDlg->ShowWindow_(SW_SHOW);
			ShowWindow(SW_HIDE);
		}
		break;

	case WM_SEND_MMS:
		if(1 == wParam)
		{
			if(0 == lParam)    //lxz 20100828
			{
				if(stop == m_nAudioOperate)
				{
					OnBtnPlayAudio();
				}
				SaveMmsData(m_pMMSData,Data::MMSData::tpSend);
				((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_isDialTimeout = 0;
				((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->PopIcon(this);
				Clear();
			}
			else			//lxz 20100828
			{
				((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->UnSetPreADSLDial();
			}
		}
		break;

	case WM_MMS_INSERT:
		{
			if ( 1== wParam)
			{
				StopAudio();
			}
			else if (3 == wParam)
			{
				CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
				main->m_pWarningNoFlashDlg->SetTitle(L"不能插入内容");
				main->m_pWarningNoFlashDlg->ShowWindow_(SW_SHOW);
			}
		}
		break;

	case WM_PAR_SIZE:
		{	
			CString page;
			m_pageCount = lParam;
			m_currentPage = wParam;
			if (wParam == lParam )
			{
				page.Format(L"当前页:%d/%d",wParam+1,wParam+1);
				if((MMS_MAX_PARSIZE-1) == lParam)
				{
					m_pageCount = MMS_MAX_PARSIZE;
				}
			}
			else
			{
				page.Format(L"当前页:%d/%d",wParam+1,lParam);
			}
			m_MJPGList.SetUnitText(24,page,true);
		}
		break;

	case WM_MMS_SIZE:
		//SetMmsSize(wParam);
		break;
	default:
		break;
	}
	return CDialog::WindowProc(message, wParam, lParam);
	
}

void CMMSDlg::OnBtnCancel()
{	
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	ShowWindow(SW_HIDE);
	Clear();
	main->PopbackIcon();
}

void CMMSDlg::FindNumber(std::string &adr)
{
	for (int i = 0 ; i < m_mapTelnum.size();i++)
	{
		if (!m_mapTelnum[i].IsEmpty())
		{	
			if (m_mapTelnum[i].Find(L".com") != -1|| m_mapTelnum[i].Find(L".COM") != -1)
			{
				adr += Util::StringOp::FromCString(m_mapTelnum[i])+";";	
			}
			else
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
				

				/*bool bname = false ;
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
				}*/
			}
						
		}
	}
}

int CMMSDlg::MmsBoxCount(Data::MMSData::Type type)
{	
	std::string filter = "[type] =";//发件箱
	filter += Util::StringOp::FromInt(type);
	
	std::vector<boost::shared_ptr<Data::MMSData> > vmmsdata;
	vmmsdata = Data::MMSData::GetFromDatabase(filter);
	return vmmsdata.size();
}

bool CMMSDlg::SaveMmsData(boost::shared_ptr<Data::MMSData> pmmsdata,Data::MMSData::Type type)
{
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	bool bin;
	if (pmmsdata)
	{	
		int ncount = MmsBoxCount(type);
		switch (type)
		{
		case Data::MMSData::tpReceive:
			{
				if (ncount < MMS_RECIVE_BOX)
				{
					bin = pmmsdata->Insert();
				}
				else
				{	
					main->m_pWarningNoFlashDlg->SetTitle(L"彩信收件箱已满,请及时删除");	
					main->m_pWarningNoFlashDlg->ShowWindow_(SW_SHOW);
					
					//删除以前最早添加的
					bin = pmmsdata->Insert();
					MmsDelOldest(type);

					main->m_pSMSListDlg->RefreshList(MMS_TYPE);
				}
			}
			break;
		case Data::MMSData::tpSend:
			{
				if (ncount < MMS_SEND_BOX)
				{
					bin = pmmsdata->Insert();
				}
				else
				{
					main->m_pWarningNoFlashDlg->SetTitle(L"彩信发件箱已满,请及时删除");	
					main->m_pWarningNoFlashDlg->ShowWindow_(SW_SHOW);
					
					bin = pmmsdata->Insert();
					MmsDelOldest(type);

					main->m_pSMSListDlg->RefreshList(MMS_TYPE);

				}
				SetEvent(g_hEvent);
			}
			break;
		case Data::MMSData::tpUnSend:
			{
				if (ncount < MMS_DRAFT_BOX)
				{
					bin = pmmsdata->Insert();
				}
				else
				{
					main->m_pWarningNoFlashDlg->SetTitle(L"彩信草稿箱已满,请及时删除");	
					main->m_pWarningNoFlashDlg->ShowWindow_(SW_SHOW);
					
					bin = pmmsdata->Insert();
					MmsDelOldest(type);

					main->m_pDraftDlg->FreshList();
					
				}
			}
			break;
			
		}	
	}
	return bin ;
}

void CMMSDlg::SaveMmsData(Data::MMSData *pmmsdata)
{
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	if (pmmsdata)
	{	
		int ncount = MmsBoxCount(Data::MMSData::tpReceive);

		if (ncount < MMS_RECIVE_BOX)
		{
			pmmsdata->Insert();
		}
		else
		{	
			main->m_pWarningNoFlashDlg->SetTitle(L"彩信收件箱已满,请及时删除");	
			main->m_pWarningNoFlashDlg->ShowWindow_(SW_SHOW);
			
			//删除以前最早添加的
			pmmsdata->Insert();
			MmsDelOldest(Data::MMSData::tpReceive);
		}
			
	}	
	
}

void CMMSDlg::MmsDelOldest(Data::MMSData::Type group)
{
	std::string filter = "[type] =";
	filter += Util::StringOp::FromInt(group);
	std::vector<boost::shared_ptr<Data::MMSData> > vmmsdata;
	vmmsdata = Data::MMSData::GetFromDatabase(filter,Data::dUp,0,1);
	if (vmmsdata.size())
	{
		CString s = vmmsdata[0]->SavePath.c_str();				
		CString filename = L"";
		CString path = L"";
		
		extern int FindFileEx(CString s, CString &sPath, CString &sFilename);
		FindFileEx(s, path, filename);
		extern void DeleteDirectory(CString SrcDir, BOOL isShow = TRUE);
		DeleteDirectory(path);
		
		vmmsdata[0]->Remove();
	}
}

void CMMSDlg::Clear()
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
	
	m_MmsSize = 0;
	m_MmsShow.Clear();
	m_picCount = 0;
	m_mp3Count = 0;
	
	m_MJPGList.SetUnitText(60,L"1/1",TRUE);
	m_MJPGList.SetUnitText(22, L"0.0K/300K", TRUE);
}

void CMMSDlg::SetPage()
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
	m_MJPGList.SetUnitText(60,page,true);
	
}

void CMMSDlg::ClearParPage()
{
	m_currentPage = 1;
}

void CMMSDlg::SetReplayStatus()
{
	m_MmsShow.SetMmsEdit();
	
	CString image[2][2] = {
		{
			L".\\adv\\mjpg\\k5\\common\\短信\\插入图片_fore.bmp",
				L".\\adv\\mjpg\\k5\\common\\短信\\插入图片_back.bmp",
		},
		{
				L".\\adv\\mjpg\\k5\\common\\短信\\插入音频文件_fore.bmp",
					L".\\adv\\mjpg\\k5\\common\\短信\\插入音频文件_back.bmp",
			}
	};
	m_MJPGList.SetUnitBitmap(20,image[0][0],image[0][1],false);
	m_MJPGList.SetUnitBitmap(21,image[1][0],image[1][1],false);
	m_MJPGList.SetUnitIsShow(40,false,false);
	
	m_nImageAction = insertImage ;
	m_nAudioAction = insertAudio ;
	
	m_MJPGList.SetUnitText(60,L"1/1",false);
	m_pageCount = 1;
	
	m_MmsShow.AllFileSize();
	m_MJPGList.Invalidate();
	
}