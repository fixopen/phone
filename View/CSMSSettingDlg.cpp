// YHTelDlg.cpp : implementation file
//

#include "stdafx.h"
#include "../multimediaphone.h"
#include "../MultimediaPhoneDlg.h"
#include "CSMSSettingDlg.h"
#include "sip.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// C3GSMSDlg dialog


#define  ALL_SENDER  20


CSMSSettingDlg::CSMSSettingDlg(CWnd* pParent /*=NULL*/)
: CDialog(CSMSSettingDlg::IDD, pParent)
{

}


void CSMSSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	
	//{{AFX_DATA_MAP(C3GSMSDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSMSSettingDlg, CDialog)
	//{{AFX_MSG_MAP(C3GSMSDlg)
	ON_MESSAGE(WM_CLICKMJPG_TOAPP, OnClickMJPG)
	ON_MESSAGE(WM_MJPGTOGGLE, OnClickMJPG)
	ON_WM_TIMER()
	ON_BN_CLICKED(BTN_CSCA,OnbtnCsca)
	ON_BN_CLICKED(BTN_VP,OnbtnVp)
//	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// C3GSMSDlg message handlers

BOOL CSMSSettingDlg::OnInitDialog() 
{
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	CDialog::OnInitDialog();
	
	m_edtSmsc.Create(WS_VISIBLE|WS_CHILD, CRect(273, 75, 273+319, 75+34), this, WM_EDIT_SMSC);
	m_edtSmsc.ShowWindow(SW_HIDE);
	
	//sms 信息保存
// 	int inter = 12;
// 	int height = 35;
// 	int ybegin = 360;
// 	int xbegin = 270;
// 	int xwidth = 75;
// 	m_Radio1.Create(L"本地", WS_VISIBLE|WS_CHILD, CRect(xbegin, ybegin, xbegin+xwidth, ybegin+height), this);
// 	m_Radio2.Create(L"SIM卡", WS_VISIBLE|WS_CHILD, CRect(xbegin+xwidth+10, ybegin, xbegin+xwidth*2+10,ybegin+height), this);
// 	m_Radio3.Create(L"本地", WS_VISIBLE|WS_CHILD, CRect(541, ybegin, 541+xwidth, ybegin+height), this);
// 	m_Radio4.Create(L"SIM卡", WS_VISIBLE|WS_CHILD, CRect(541+xwidth+10, ybegin, 541+xwidth*2+10, ybegin+height), this);
// 	m_Radio1.SetColor(RGB(0, 0, 0), RGB(237, 237, 237));
// 	m_Radio2.SetColor(RGB(0, 0, 0), RGB(237, 237, 237));
// 	m_Radio3.SetColor(RGB(0, 0, 0), RGB(237, 237, 237));
// 	m_Radio4.SetColor(RGB(0, 0, 0), RGB(237, 237, 237));
// 
// 	CButton *pButton[2] = {&m_Radio1, &m_Radio2};
// 	m_Radio1.SetGroupButton(pButton, 2);
// 	m_Radio2.SetGroupButton(pButton, 2);
// 	
// 	CButton *pButton_[2] = {&m_Radio3,&m_Radio4};
// 	m_Radio3.SetGroupButton(pButton_, 2);
// 	m_Radio4.SetGroupButton(pButton_, 2);
// 	
// 	m_Radio1.SetCheck_(0);
// 	m_Radio2.SetCheck_(0);
// 	m_Radio3.SetCheck_(0);
// 	m_Radio4.SetCheck_(0);


// 	if (main->m_pMessageSet->SmsReciveSaveSim())//保存到SIM卡
// 	{
// 		m_Radio4.SetCheck_(1);
// 	}
// 	else 
// 	{
// 		m_Radio3.SetCheck_(1);
// 	}
	
//	m_btnSetCSCA.Create(L"设置",WS_CHILD|WS_VISIBLE,CRect(608, 79, 608+40, 79+50),this,BTN_CSCA);
//	m_btnSetVP.Create(L"设置",WS_CHILD|WS_VISIBLE,CRect(608, 149, 608+40, 149+50),this,BTN_VP);
	
	m_MJPGList.Create(L"", WS_VISIBLE|WS_CHILD, CRect(0, 0, 800, 423), this,10086);
	m_MJPGList.SetCurrentLinkFile(".\\adv\\mjpg\\k5\\中文\\短消息设置.xml");
	m_MJPGList.SetMJPGRect(CRect(0, 0, 800, 423));

	
	m_MJPGList.SetUnitIsDownStatus(5,true);

	MoveWindow(0,57,800,423);

	m_vpState = normal;

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
BOOL CSMSSettingDlg::PreTranslateMessage(MSG* pMsg)
{
	return CDialog::PreTranslateMessage(pMsg);
}

void CSMSSettingDlg::OnClickMJPG(WPARAM w, LPARAM l)
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

	case 11://设置有效期
		//OnbtnCsca();
		m_vpState = setcsca;
		main->m_pATCommandWarp1->GetVP();
		break;

	case 20://短信有效时间
	case 21:
	case 22:
	case 23:
		{
			main->m_pTempMsgSet->SmsValidity(w-20+1);
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
	case 24://有效期
		OnbtnVp();
		break;
	case 30://短信回执
		m_MJPGList.SetUnitIsDownStatus(w, TRUE);
		m_MJPGList.SetUnitIsDownStatus(w+1, FALSE);
		m_MJPGList.SetUnitIsShow(w, TRUE);
		m_MJPGList.SetUnitIsShow(w+1, TRUE);
		break;
	case 31:
		m_MJPGList.SetUnitIsDownStatus(w, TRUE);
		m_MJPGList.SetUnitIsDownStatus(w-1, FALSE);
		m_MJPGList.SetUnitIsShow(w, TRUE);
		m_MJPGList.SetUnitIsShow(w-1, TRUE);
		break;

	case 32://短息回执设置
		{	
			if (main->m_pTempMsgSet->SmsReceipt())//短息回执现在已经开启状态
			{
				if (m_MJPGList.GetUnitIsDownStatus(30))
				{
					main->m_pWarningNoFlashDlg->SetTitle(L"短信回执已经是开启的");
					main->m_pWarningNoFlashDlg->ShowWindow_(SW_SHOW);				
				}
				else
				{
					main->m_pATCommandWarp1->SmsReceiptOpen(false);
				}
			}
			else 
			{
				if (m_MJPGList.GetUnitIsDownStatus(31))
				{
					main->m_pWarningNoFlashDlg->SetTitle(L"短信回执已经是关闭的");
					main->m_pWarningNoFlashDlg->ShowWindow_(SW_SHOW);				
				}
				else
				{
					main->m_pATCommandWarp1->SmsReceiptOpen(true);
				}
			}
			
		}
		break;

	case 40://已发送消息保存
		main->m_pTempMsgSet->SaveSendMessage(true);
		m_MJPGList.SetUnitIsDownStatus(w, TRUE);
		m_MJPGList.SetUnitIsDownStatus(w+1, FALSE);
		m_MJPGList.SetUnitIsShow(w, TRUE);
		m_MJPGList.SetUnitIsShow(w+1, TRUE);
		break;
	case 41:
		main->m_pTempMsgSet->SaveSendMessage(false);
		m_MJPGList.SetUnitIsDownStatus(w, TRUE);
		m_MJPGList.SetUnitIsDownStatus(w-1, FALSE);
		m_MJPGList.SetUnitIsShow(w, TRUE);
		m_MJPGList.SetUnitIsShow(w-1, TRUE);
		break;
	
	case 50://是否将短消息存储到草稿箱
		main->m_pTempMsgSet->SaveMessageDraft(true);
		m_MJPGList.SetUnitIsDownStatus(w, TRUE);
		m_MJPGList.SetUnitIsDownStatus(w+1, FALSE);
		m_MJPGList.SetUnitIsShow(w, TRUE);
		m_MJPGList.SetUnitIsShow(w+1, TRUE);
		break;
	case 51:
		main->m_pTempMsgSet->SaveMessageDraft(false);
		m_MJPGList.SetUnitIsDownStatus(w, TRUE);
		m_MJPGList.SetUnitIsDownStatus(w-1, FALSE);
		m_MJPGList.SetUnitIsShow(w, TRUE);
		m_MJPGList.SetUnitIsShow(w-1, TRUE);
		break;

	case 61:
		m_MJPGList.SetUnitIsDownStatus(61,true);
		m_MJPGList.SetUnitIsDownStatus(62,false);
		m_MJPGList.SetUnitIsShow(61,true);
		m_MJPGList.SetUnitIsShow(62,true);
		break;

	case 62:
		m_MJPGList.SetUnitIsDownStatus(61,false);
		m_MJPGList.SetUnitIsDownStatus(62,true);
		m_MJPGList.SetUnitIsShow(61,true);
		m_MJPGList.SetUnitIsShow(62,true);
		break;

	case 90://短消息设置
		break;

	case 91://彩消息设置
		main->m_pMmsSettingDlg->InitData(main->m_pTempMsgSet);
		main->m_pMmsSettingDlg->ShowWindow(SW_SHOW);
		main->AddIcon(Allicon[1],main->m_pMmsSettingDlg,false);
		main->PopIcon(this);
	//	ShowWindow(SW_HIDE);
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

void CSMSSettingDlg::SetUpBtn(int ID)
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

void CSMSSettingDlg::InitData(boost::shared_ptr<Data::MessageSet> const& msgSet)
{	
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
//	CString number = Util::StringOp::ToCString(msgSet->SmsCenterAddress());
//	m_edtSmsc.SetWindowText(number);
	
	main->m_pATCommandWarp1->CSCS("GSM");
	main->m_pATCommandWarp1->GetCsca();
	main->m_pATCommandWarp1->GetVP();

//  change by qi 20100413
// 	for(int i=1; i<=4; i++)
// 	{
// 		if(i == msgSet->SmsValidity())
// 		{
// 			m_MJPGList.SetUnitIsDownStatus(19+i,TRUE);
// 		}
// 		else
// 		{
// 			m_MJPGList.SetUnitIsDownStatus(19+i,FALSE);
// 		}
// 	}
	
	
	if(msgSet->SmsReceipt())
	{
		m_MJPGList.SetUnitIsDownStatus(30,TRUE);
		m_MJPGList.SetUnitIsDownStatus(31,FALSE);
	}
	else
	{
		m_MJPGList.SetUnitIsDownStatus(31,TRUE);
		m_MJPGList.SetUnitIsDownStatus(30,FALSE);
	}
	
	if (main->m_pMessageSet->SmsSendSaveSim() || 
		main->m_pMessageSet->SmsReciveSaveSim())//保存到SIM卡
	{	
		m_MJPGList.SetUnitIsDownStatus(61,true);
		m_MJPGList.SetUnitIsDownStatus(62,false);

		m_MJPGList.SetUnitIsShow(61,true,true);
		m_MJPGList.SetUnitIsShow(62,true,true);
	}
	else 
	{	
		m_MJPGList.SetUnitIsDownStatus(61,false);
		m_MJPGList.SetUnitIsShow(61,true,true);

		m_MJPGList.SetUnitIsDownStatus(62,true);
		m_MJPGList.SetUnitIsShow(62,true,true);
	}

	if(msgSet->SaveSendMessage())
	{
		m_MJPGList.SetUnitIsDownStatus(40,TRUE);
		m_MJPGList.SetUnitIsDownStatus(41,FALSE);
	}
	else
	{
		m_MJPGList.SetUnitIsDownStatus(41,TRUE);
		m_MJPGList.SetUnitIsDownStatus(40,FALSE);
	}

	if(msgSet->SaveMessageDraft())
	{
		m_MJPGList.SetUnitIsDownStatus(50,TRUE);
		m_MJPGList.SetUnitIsDownStatus(51,FALSE);
	}
	else
	{
		m_MJPGList.SetUnitIsDownStatus(51,TRUE);
		m_MJPGList.SetUnitIsDownStatus(50,FALSE);
	}
}

void CSMSSettingDlg::OnButtonOK()
{
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	
	main->m_pDeleteTipDlg->SetTitle(L"确定要保存设置吗?");
	main->m_pDeleteTipDlg->SetHWnd(this->GetSafeHwnd());
	main->m_pDeleteTipDlg->ShowWindow_(TRUE);
}

void CSMSSettingDlg::OnButtonCancel()
{
	ShowWindow(SW_HIDE);
}

void CSMSSettingDlg::SettingOK()
{
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	CString smsc;
//	m_edtSmsc.GetWindowText(smsc);
//	main->m_pMessageSet->SmsCenterAddress(Util::StringOp::FromCString(smsc));
	
	main->m_pMessageSet->SmsValidity(main->m_pTempMsgSet->SmsValidity());
	main->m_pMessageSet->SmsReceipt(main->m_pTempMsgSet->SmsReceipt());
	main->m_pMessageSet->SaveSendMessage(main->m_pTempMsgSet->SaveSendMessage());
	main->m_pMessageSet->SaveMessageDraft(main->m_pTempMsgSet->SaveMessageDraft());

	//add by qi 20100407
// 	if (m_Radio1.GetCheck_())//本地
// 	{
// 		main->m_pMessageSet->SmsSendSaveSim(false);	
// 	}
// 	if (m_Radio2.GetCheck_())//SIM卡
// 	{
// 		main->m_pMessageSet->SmsSendSaveSim(true);	
// 	}	
// 	
// 	if (m_Radio3.GetCheck_())//本地
// 	{
// 		main->m_pMessageSet->SmsReciveSaveSim(false);	
// 	}	
// 	if (m_Radio4.GetCheck_())//SIM卡
// 	{
// 		main->m_pMessageSet->SmsReciveSaveSim(true);	
// 	}
	
	if (m_MJPGList.GetUnitIsDownStatus(61))
	{
		main->m_pMessageSet->SmsSendSaveSim(true);
		main->m_pMessageSet->SmsReciveSaveSim(true);
	}
	else if (m_MJPGList.GetUnitIsDownStatus(62))
	{
		main->m_pMessageSet->SmsSendSaveSim(false);
		main->m_pMessageSet->SmsReciveSaveSim(false);
	}

	main->m_pMessageSet->Update();
	ShowWindow(SW_HIDE);
	main->PopbackIcon();
	main->IsSendMessage(FALSE);
}

void CSMSSettingDlg::ShowWindow_(int nCmdShow)
{
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	if(nCmdShow)
	{	
		if (!(main->m_bSearchNetWin))
		{
			main->m_pWarningNoFlashDlg->SetTitle(L"没有搜到网络");
			main->m_pWarningNoFlashDlg->ShowWindow_(SW_SHOW);
			return;
		}

		main->m_pMessageSet = Data::MessageSet::GetCurrentConfig();
		main->m_pTempMsgSet = main->m_pMessageSet;
		InitData(main->m_pMessageSet);
		ShowWindow(SW_SHOW);
		SetTimer(1, 50, NULL);
	}
	else
	{
		ShowWindow(SW_HIDE);
	}
}

void CSMSSettingDlg::OnTimer(UINT nIDEvent)
{
	if(1 == nIDEvent)
	{
		m_edtSmsc.ShowWindow(SW_SHOW);
		KillTimer(1);
	}
}

LRESULT CSMSSettingDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	switch(message)
	{
	case WM_DELETESELITEM:
		SettingOK();
		break;
	case WM_SMS_VP:
		{	
			switch (m_vpState)
			{
				case setvp:
					UINT vp;
					vp = main->m_pTempMsgSet->SmsValidity();
					main->m_pATCommandWarp1->SetVP(vp);
					m_vpState = normal;
					break;

				case setcsca:
					OnbtnCsca();
					m_vpState = restorevp;
					main->m_pATCommandWarp1->GetVP();
					break;

				case restorevp:
					main->m_pATCommandWarp1->SetVP();
					m_vpState = normal;
					break;

				case normal:
					{
						CString *vp = (CString *)wParam;
						m_MJPGList.SetUnitText(63,*vp,false);
						
						//有效期
						for (int i = 0 ; i < 4; i++)
						{
							if (i != lParam)
							{
								m_MJPGList.SetUnitIsDownStatus(20+i,false);
						//		m_MJPGList.SetUnitIsShow(20+i, true);
							}
							else
							{
								m_MJPGList.SetUnitIsDownStatus(20+i,true);
						//		m_MJPGList.SetUnitIsShow(20+i, true);
							}
						}

					}
					break;
			}
		}
		break;
	case WM_CSCA_SET_OK:
		{
			CString csca ;
			m_edtSmsc.GetWindowText(csca);
			main->m_pMessageSet->SmsCenterAddress(Util::StringOp::FromCString(csca));
			main->m_pMessageSet->Update();
			//在重新设置有效期
		}
		break;

	case WM_SMS_CSCA:
		{
			std::string *s = (std::string*)(wParam);
			m_edtSmsc.SetWindowText(Util::StringOp::ToCString(*s));	
		}
		break;
	case WM_VP_OK:
		{
			for (int i = 0 ; i < 4;i++)
			{
				if (m_MJPGList.GetUnitIsDownStatus(20+i))
				{	
					CString vp ;
					switch (i)
					{
					case 0:
						vp = L"60分钟";
						break;
					case 1:
						vp = L"24小时";
						break;
					case 2:
						vp = L"七天";
						break;
					case 3:
						vp = L"63周";
						break;
					}
					m_MJPGList.SetUnitText(63,vp,true);
				}
			}
		}
		break;
	
	case WM_CNMI_OPEN:		
		if ( 1 == wParam)//设置成功
		{
			main->m_pTempMsgSet->SmsReceipt(true);
		}
		main->m_pTempMsgSet->Update();		
		break;

	case WM_CNMI_CLOSE:		
		if ( 1 == wParam)//设置成功
		{
			main->m_pTempMsgSet->SmsReceipt(false);
		}
		main->m_pTempMsgSet->Update();		
		break;
	};
	return CDialog::WindowProc(message, wParam, lParam);
}

void CSMSSettingDlg::OnbtnVp()
{
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	
	for (int i = 0 ; i < 4;i++)//有没有选择有效期
	{
		if(m_MJPGList.GetUnitIsDownStatus(20+i))
		{	
			main->m_pATCommandWarp1->GetVP();//先获得下有效期
			m_vpState = setvp;
			break;
		}
	}
}

void CSMSSettingDlg::OnbtnCsca()
{	
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	CString smsc;
	m_edtSmsc.GetWindowText(smsc);
	if (smsc == L"")
	{
		return ;		
	}
	main->m_pATCommandWarp1->ChangeCsca(Util::StringOp::FromCString(smsc).c_str());

}	

