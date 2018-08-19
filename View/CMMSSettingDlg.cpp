// YHTelDlg.cpp : implementation file
//

#include "stdafx.h"
#include "../multimediaphone.h"
#include "../MultimediaPhoneDlg.h"
#include "CMMSSettingDlg.h"
#include "sip.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

bool g_bSendReport = false;
bool g_bReadReport = false;

/////////////////////////////////////////////////////////////////////////////
// C3GSMSDlg dialog


#define  ALL_SENDER  20


CMMSSettingDlg::CMMSSettingDlg(CWnd* pParent /*=NULL*/)
: CDialog(CMMSSettingDlg::IDD, pParent)
{
	m_setType = -1;
}


void CMMSSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(C3GSMSDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMMSSettingDlg, CDialog)
	//{{AFX_MSG_MAP(C3GSMSDlg)
	ON_MESSAGE(WM_CLICKMJPG_TOAPP, OnClickMJPG)
	ON_MESSAGE(WM_MJPGTOGGLE, OnClickMJPG)
//	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// C3GSMSDlg message handlers

BOOL CMMSSettingDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_MJPGList.Create(L"", WS_VISIBLE|WS_CHILD, CRect(0, 0, 800, 423), this,10086);
	m_MJPGList.SetCurrentLinkFile(".\\adv\\mjpg\\k5\\中文\\彩消息设置.xml");
	m_MJPGList.SetMJPGRect(CRect(0, 0, 800, 423));
	
	m_MJPGList.SetUnitIsDownStatus(5,true);
	m_MJPGList.SetUnitIsDownStatus(71,true);


	MoveWindow(0,57,800,423);
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	InitData(main->m_pMessageSet);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
BOOL CMMSSettingDlg::PreTranslateMessage(MSG* pMsg)
{
	return CDialog::PreTranslateMessage(pMsg);
}

void CMMSSettingDlg::OnClickMJPG(WPARAM w, LPARAM l)
{
	CMultimediaPhoneDlg *main = ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd));
	CString icon ;
	switch(w)
	{
	case 0:	//写短信
		main->m_pMainDlg->m_p3GSMSDlg->ShowWindow(SW_SHOW);
		main->AddIcon(Allicon[1],main->m_pMainDlg->m_p3GSMSDlg,false);		
		main->PopIcon(this);
		break;		

	case 1://写彩信
		main->m_pMMSDlg->ShowWindow_(SW_SHOW);
		main->AddIcon(Allicon[1],main->m_pMMSDlg,false);		
		main->PopIcon(this);
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
		break;

	case 11:
	case 12:
	case 13:
		{
			main->m_pTempMsgSet->MmsValidity(w-10);
			m_MJPGList.SetUnitIsDownStatus(w, TRUE);
			for(int i=11; i<=13; i++)
			{
				if(w != i)
				{
					m_MJPGList.SetUnitIsDownStatus(i, FALSE);
				}
				m_MJPGList.SetUnitIsShow(i, TRUE);
			}
		}
		break;
	case 20: //网络选择
	case 21:
	case 22:
	case 23:
		{
			main->m_pTempMsgSet->MmsNetSelect(w-20+1);
			m_MJPGList.SetUnitIsDownStatus(w, TRUE);
			for(int i=20; i<24; i++)
			{
				if(w != i)
				{
					m_MJPGList.SetUnitIsDownStatus(i, FALSE);
				}
				m_MJPGList.SetUnitIsShow(i, TRUE);
			}
		}
		break;

	case 30://手动接收
		main->m_pTempMsgSet->IsAutoReceive(false);
		m_MJPGList.SetUnitIsDownStatus(w, TRUE);
		m_MJPGList.SetUnitIsDownStatus(w+1, FALSE);
		m_MJPGList.SetUnitIsShow(w, TRUE);
		m_MJPGList.SetUnitIsShow(w+1, TRUE);
		break;
	case 31://自动接收
		main->m_pTempMsgSet->IsAutoReceive(true);
		m_MJPGList.SetUnitIsDownStatus(w, TRUE);
		m_MJPGList.SetUnitIsDownStatus(w-1, FALSE);
		m_MJPGList.SetUnitIsShow(w, TRUE);
		m_MJPGList.SetUnitIsShow(w-1, TRUE);
		break;

	case 40://匿名信息
	case 41:
	case 42:
		{
			main->m_pTempMsgSet->AnonymousMMSReceive(w-40+1);
			m_MJPGList.SetUnitIsDownStatus(w, TRUE);
			for(int i=40; i<43; i++)
			{
				if(w != i)
				{
					m_MJPGList.SetUnitIsDownStatus(i, FALSE);
				}
				m_MJPGList.SetUnitIsShow(i, TRUE);
			}
		}
		break;
	
	case 50://开启发送报告
		main->m_pTempMsgSet->MmsSendReport(true);
		m_MJPGList.SetUnitIsDownStatus(w, TRUE);
		m_MJPGList.SetUnitIsDownStatus(w+1, FALSE);
		m_MJPGList.SetUnitIsShow(w, TRUE);
		m_MJPGList.SetUnitIsShow(w+1, TRUE);
		break;
	case 51: //不开启发送报告
		main->m_pTempMsgSet->MmsSendReport(false);
		m_MJPGList.SetUnitIsDownStatus(w, TRUE);
		m_MJPGList.SetUnitIsDownStatus(w-1, FALSE);
		m_MJPGList.SetUnitIsShow(w, TRUE);
		m_MJPGList.SetUnitIsShow(w-1, TRUE);
		break;

	case 60://开启阅读报告
		main->m_pTempMsgSet->MmsReadReport(false);
		m_MJPGList.SetUnitIsDownStatus(w, TRUE);
		m_MJPGList.SetUnitIsDownStatus(w+1, FALSE);
		m_MJPGList.SetUnitIsShow(w, TRUE);
		m_MJPGList.SetUnitIsShow(w+1, TRUE);
		break;
	case 61: //不开启阅读报告
		main->m_pTempMsgSet->MmsReadReport(false);
		m_MJPGList.SetUnitIsDownStatus(w, TRUE);
		m_MJPGList.SetUnitIsDownStatus(w-1, FALSE);
		m_MJPGList.SetUnitIsShow(w, TRUE);
		m_MJPGList.SetUnitIsShow(w-1, TRUE);
		break;

	case 70: //最大容量
	case 71:
	case 72:
		{
			main->m_pTempMsgSet->MmsMaxSize(w-70+1);
			m_MJPGList.SetUnitIsDownStatus(w, TRUE);
			for(int i=70; i<73; i++)
			{
				if(w != i)
				{
					m_MJPGList.SetUnitIsDownStatus(i, FALSE);
				}
				m_MJPGList.SetUnitIsShow(i, TRUE);
			}
		}
		break;

	case 80: //恢复出厂设置
		main->m_pTempMsgSet->RecoveryDefSet(true);
		m_setType = 0;
		
		m_MJPGList.SetUnitIsDownStatus(w, TRUE);
		m_MJPGList.SetUnitIsDownStatus(w+1, FALSE);
		m_MJPGList.SetUnitIsShow(w, TRUE);
		m_MJPGList.SetUnitIsShow(w+1, TRUE);
		break;
	case 81: //不恢复出厂设置
		main->m_pTempMsgSet->RecoveryDefSet(false);
		m_setType = 1;
		
		m_MJPGList.SetUnitIsDownStatus(w, TRUE);
		m_MJPGList.SetUnitIsDownStatus(w-1, FALSE);
		m_MJPGList.SetUnitIsShow(w, TRUE);
		m_MJPGList.SetUnitIsShow(w-1, TRUE);
		break;

	case 90://短消息设置
		main->m_pSmsSettingDlg->InitData(main->m_pTempMsgSet);
		main->m_pSmsSettingDlg->ShowWindow(SW_SHOW);
		main->AddIcon(Allicon[1],main->m_pSmsSettingDlg,false);
		main->PopIcon(this);
		break;

	case 91://彩消息设置
		break;

	case 1000:
		OnButtonCancel();
		main->PopbackIcon();
		main->IsSendMessage(FALSE);
		break;

	case 1001:
		OnButtonOK();
		break;

	default:
	    break;

	}

}

void CMMSSettingDlg::SetUpBtn(int ID)
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

void CMMSSettingDlg::InitData(boost::shared_ptr<Data::MessageSet> const& msgSet)
{
	for(int i=1; i<=4; i++)
	{
		if(i == msgSet->MmsNetSelect())
		{
			m_MJPGList.SetUnitIsDownStatus(19+i,TRUE);
		}
		else
		{
			m_MJPGList.SetUnitIsDownStatus(19+i,FALSE);
		}
	}

	for(i=1; i<=3; i++)
	{
		if(i == msgSet->MmsValidity())
		{
			m_MJPGList.SetUnitIsDownStatus(10+i,TRUE);
		}
		else
		{
			m_MJPGList.SetUnitIsDownStatus(10+i,FALSE);
		}
	}
	
	if(msgSet->IsAutoReceive())
	{
		m_MJPGList.SetUnitIsDownStatus(31,TRUE);
		m_MJPGList.SetUnitIsDownStatus(30,FALSE);
	}
	else
	{
		m_MJPGList.SetUnitIsDownStatus(30,TRUE);
		m_MJPGList.SetUnitIsDownStatus(31,FALSE);
	}
	
	for(i=1; i<=3; i++)
	{
		if(i == msgSet->AnonymousMMSReceive())
		{
			m_MJPGList.SetUnitIsDownStatus(39+i,TRUE);
		}
		else
		{
			m_MJPGList.SetUnitIsDownStatus(39+i,FALSE);
		}
	}

	if(msgSet->MmsSendReport())
	{
		m_MJPGList.SetUnitIsDownStatus(50,TRUE);
		m_MJPGList.SetUnitIsDownStatus(51,FALSE);
	}
	else
	{
		m_MJPGList.SetUnitIsDownStatus(51,TRUE);
		m_MJPGList.SetUnitIsDownStatus(50,FALSE);
	}

	if(msgSet->MmsReadReport())
	{
		m_MJPGList.SetUnitIsDownStatus(60,TRUE);
		m_MJPGList.SetUnitIsDownStatus(61,FALSE);
	}
	else
	{
		m_MJPGList.SetUnitIsDownStatus(61,TRUE);
		m_MJPGList.SetUnitIsDownStatus(60,FALSE);
	}

	for(i=1; i<=3; i++)
	{
		if(i == msgSet->MmsMaxSize())
		{
			m_MJPGList.SetUnitIsDownStatus(69+i,TRUE);
		}
		else
		{
			m_MJPGList.SetUnitIsDownStatus(69+i,FALSE);
		}
	}

	if(msgSet->RecoveryDefSet())
	{
		m_MJPGList.SetUnitIsDownStatus(80,TRUE);
		m_MJPGList.SetUnitIsDownStatus(81,FALSE);
	}
	else
	{
		m_MJPGList.SetUnitIsDownStatus(81,TRUE);
		m_MJPGList.SetUnitIsDownStatus(80,FALSE);
	}
}

void CMMSSettingDlg::OnButtonOK()
{
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	if(0 == m_setType)
	{
		main->m_pDeleteTipDlg->SetTitle(L"确定要恢复出厂设置吗?");
	}
	else
	{
		main->m_pDeleteTipDlg->SetTitle(L"确定要保存设置吗?");
	}
	main->m_pDeleteTipDlg->SetHWnd(this->GetSafeHwnd());
	main->m_pDeleteTipDlg->ShowWindow_(TRUE);
}

void CMMSSettingDlg::OnButtonCancel()
{
	ShowWindow(SW_HIDE);
}

void CMMSSettingDlg::SettingOK()
{
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	
	main->m_pMessageSet->MmsValidity(main->m_pTempMsgSet->MmsValidity());
	main->m_pMessageSet->MmsNetSelect(main->m_pTempMsgSet->MmsNetSelect());
	main->m_pMessageSet->IsAutoReceive(main->m_pTempMsgSet->IsAutoReceive());
	main->m_pMessageSet->AnonymousMMSReceive(main->m_pTempMsgSet->AnonymousMMSReceive());
	main->m_pMessageSet->MmsSendReport(main->m_pTempMsgSet->MmsSendReport());
	main->m_pMessageSet->MmsReadReport(main->m_pTempMsgSet->MmsReadReport());
	main->m_pMessageSet->MmsMaxSize(main->m_pTempMsgSet->MmsMaxSize());
	main->m_pMessageSet->RecoveryDefSet(main->m_pTempMsgSet->RecoveryDefSet());

	g_bSendReport = main->m_pTempMsgSet->MmsSendReport();
	g_bReadReport = main->m_pTempMsgSet->MmsReadReport();

	main->m_pMessageSet->Update();
	ShowWindow(SW_HIDE);
	main->PopbackIcon();
	main->IsSendMessage(FALSE);

	m_setType = 1;
}

void CMMSSettingDlg::SettingDefault()
{
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	int id = main->m_pTempMsgSet->id();
	Data::SetMessageType type = main->m_pTempMsgSet->Type();
	main->m_pTempMsgSet = Data::MessageSet::GetDefaultConfig();
	main->m_pMessageSet = main->m_pTempMsgSet;
	main->m_pTempMsgSet->id(id);
	main->m_pTempMsgSet->Type(type);
	main->m_pTempMsgSet->Update();

	g_bSendReport = main->m_pTempMsgSet->MmsSendReport();
	g_bReadReport = main->m_pTempMsgSet->MmsReadReport();
	
	ShowWindow(SW_HIDE);
	main->PopbackIcon();
	main->IsSendMessage(FALSE);
	m_setType = 1;
}

void CMMSSettingDlg::ShowWindow_(int nCmdShow)
{
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	if(nCmdShow)
	{
		main->m_pMessageSet = Data::MessageSet::GetCurrentConfig();
		main->m_pMessageSet->RecoveryDefSet(false);   //每次进入把恢复默认设置为否
		main->m_pTempMsgSet->Update();
		main->m_pTempMsgSet = main->m_pMessageSet;
		InitData(main->m_pMessageSet);
		ShowWindow(SW_SHOW);
	}
	else
	{
		ShowWindow(SW_HIDE);
	}
}

LRESULT CMMSSettingDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
	case WM_DELETESELITEM:
		if(0 == m_setType)
		{
			SettingDefault();
		}
		else
		{
			SettingOK();
		}
		break;

	};
	return CDialog::WindowProc(message, wParam, lParam);
}



	





