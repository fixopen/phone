// TelephoneDialDlg.cpp : implementation file
//

#include "stdafx.h"
#include "..\multimediaphone.h"
#include "../MultimediaPhoneDlg.h"
#include "TelephoneDialDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTelephoneDialDlg dialog


CTelephoneDialDlg::CTelephoneDialDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTelephoneDialDlg::IDD, pParent)
	/*: CCEDialog(CTelephoneDialDlg::IDD, pParent)*/
{
	m_bSoftware = false;
	m_bSetGainLevel = false;
}


void CTelephoneDialDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTelephoneDialDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTelephoneDialDlg, CDialog)
	//{{AFX_MSG_MAP(CTelephoneDialDlg)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_CLICKMJPG_TOAPP, OnClickMJPG)
	ON_MESSAGE(WM_CLICKMJPG_TOAPP1, OnClickMJPG1)
	ON_MESSAGE(WM_CHLD_OK,OKMessage)

END_MESSAGE_MAP()

BOOL CTelephoneDialDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_p10ContactInfoDlg = new C10ContactInfoDlg();
	m_p10ContactInfoDlg->Create(C10ContactInfoDlg::IDD);
	m_p10ContactInfoDlg->ShowWindow(SW_HIDE);

	m_MJPGList.Create(L"", WS_VISIBLE|WS_CHILD, CRect(0, 0, 800, 423), this,10086);
	m_MJPGList.SetCurrentLinkFile(".\\adv\\mjpg\\k5\\中文\\电话拨打.xml");
	m_MJPGList.SetMJPGRect(CRect(0, 0, 800, 423));
	
	m_MJPGList.SetUnitIsShow(30,false,true);
	MoveWindow(0,57,800,423);

	m_bsubDial = false ;

	return TRUE ;

}

void CTelephoneDialDlg::OnClickMJPG1(WPARAM w, LPARAM l)
{	
	CMultimediaPhoneDlg *pMainDlg = (CMultimediaPhoneDlg*)(theApp.m_pMainWnd);

	if(!m_bMayReturn)
	{
		return ;
	}
	
	char telcode[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '*', "#"};
	switch (w)
	{	
	case 0:
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
	case 7:
	case 8:
	case 9:
	case 10:
	case 11:
		pMainDlg->SendMessage(WM_TEL_KEYCODE, (WPARAM)telcode[w],0);//拨号
		break;
	}
}

void CTelephoneDialDlg::OnClickMJPG(WPARAM w, LPARAM l)
{	
 	if(!m_bMayReturn)
 	{
 		return ;
 	}

	CMultimediaPhoneDlg *pMainDlg = (CMultimediaPhoneDlg*)(theApp.m_pMainWnd);
	CWnd *p ;
	CString icon;
	char telcode[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '*', "#"};
	switch (w)
	{	
// 	case 0:
// 	case 1:
// 	case 2:
// 	case 3:
// 	case 4:
// 	case 5:
// 	case 6:
// 	case 7:
// 	case 8:
// 	case 9:
// 	case 10:
// 	case 11:
// 		pMainDlg->SendMessage(WM_TEL_KEYCODE, (WPARAM)telcode[w],0);//拨号
// 		break;

	case 18://3g 拨号
		{	
			if (!pMainDlg->m_bSearchNetOver)//没有收网结束不能打电话
			{
				pMainDlg->m_pDeleteTipDlg->SetTitle(L"正在初始化...");
				pMainDlg->m_pDeleteTipDlg->ShowWindow_(SW_SHOW);
				return;
			}
			
			/*if (pMainDlg->m_bInsertSim)
			{
				if (!pMainDlg->m_bSearchNetWin)//没收网成功
				{
					pMainDlg->m_pDeleteTipDlg->SetTitle(L"正在搜网中...");
					pMainDlg->m_pDeleteTipDlg->ShowWindow_(SW_SHOW);
					return;
				}
			}*/
		
			CString s = m_MJPGList.GetUnitText(100);
			if(s.GetLength() > 0)
			{
				if (m_bsubDial)
				{
					KillTimer(IDT_AUTO_DIAL);
					if (pMainDlg->m_phoneLine[(pMainDlg->m_nline+1)%2].pFSM->getCurrentState()
						== CMultimediaPhoneDlg::p3gsHold)
					{
						m_bsubDial = false;
						Dial();	
					}
					else
					{
						pMainDlg->m_pATCommandWarp1->CHLD(2);
					}
					return;
				}
				else 
				{
					//摘手柄按键的输出增益恢复
					if (pMainDlg->phone_->Hand())
					{	
						pMainDlg->m_pATCommandWarp1->SetOutputGainLevel(1);
					}
					Dial();
				}
	
			}

		}
		break;

	case 13:
		pMainDlg->phone_->StopRing();
		pMainDlg->m_pContactDlg->SetType(4);//类型4
		pMainDlg->m_pContactDlg->ShowRightBtn(false);
		pMainDlg->m_pContactDlg->ShowWindow_();
		p = pMainDlg->m_pContactDlg;
		icon = Allicon[2];
		pMainDlg->AddIcon(icon,p,false);
		break;
	
	case 14:
		pMainDlg->phone_->StopRing();
		pMainDlg->m_pContactInfoDlg->SetType(3);
		pMainDlg->m_pContactInfoDlg->ShowRightBtn(false);
		pMainDlg->m_pContactInfoDlg->ShowWindow_();
		p = pMainDlg->m_pContactInfoDlg;
		icon = Allicon[3];
		pMainDlg->AddIcon(icon,p,false);
		break;

	case 19://重播
		pMainDlg->m_pTelephoneDlg->Redial();
		break;

	case 20://删除一个号码
		DeleteNum();
		break;

	case 21:
		m_p10ContactInfoDlg->Show10ContactInfo();
		break;

	case 1000:
		ExitWindow();
		break;
		
	default:
		break;
	}
}

void CTelephoneDialDlg::Key_(void* param)
{
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;

	if(main->m_phoneLine[main->m_nline].pFSM->getCurrentState() 
		== CMultimediaPhoneDlg::p3gsConnected)
	{
		main->m_pTelephoneDlg->Key_(param);
		return;
	}

	if(param == NULL)
		return;
	BOOL flag = FALSE;
	if (IsWindowVisible())
	{
		flag = TRUE;
	}

	if (m_p10ContactInfoDlg->IsWindowVisible())
	{
		m_p10ContactInfoDlg->ShowWindow(SW_HIDE);
	}

	CString number;
	number = m_MJPGList.GetUnitText(100);
	static int gRMarkJia = 0;
	char val_key = char(param);
	if(char(param) == '0' && gRMarkJia == 0 && number.GetLength() == 0)
	{
		gRMarkJia++;
	}
	else if(char(param) == '0' && gRMarkJia >= 1)
	{
		val_key = '+';
		gRMarkJia = 0;
	}
	else
	{
		gRMarkJia = 0;
	}
		
	if((char(param) == '+' || char(param) >= '0' && char(param) <= '9') || (char(param) == '*') || (char(param) == '#') || ((char(param) >= 'A' && char(param) <= 'F')))
		number += val_key;
//	number += char(param);

	if (number.GetLength() > 20)
	{
		m_MJPGList.SetUnitFont(100, font_16);
		m_MJPGList.SetUnitText(100, number, flag);			
	}
	else
	{
		m_MJPGList.SetUnitFont(100, font_30);
		m_MJPGList.SetUnitText(100, number, flag);
	}

	if (!main->m_pTelephoneDlg->m_spContactInfo[main->m_nline])
	{
		main->m_pTelephoneDlg->m_spContactInfo[main->m_nline] = boost::shared_ptr<Data::ContactInfo>(new Data::ContactInfo);
		main->m_pTelephoneDlg->m_spContactInfo[main->m_nline]->type(Data::citOut);
		main->m_pTelephoneDlg->m_spContactInfo[main->m_nline]->startTime(CTime::GetCurrentTime());
		main->m_pTelephoneDlg->m_spContactInfo[main->m_nline]->Insert();

	}
	main->m_pTelephoneDlg->m_spContactInfo[main->m_nline]->telephoneNumber(Util::StringOp::FromCString(number));
	
	m_sTelephoneNumber = Util::StringOp::FromCString(number);
	
	char code[2] = {0};
	code[0] = char(param);

//  change by qi 20100318
	if (!m_bsubDial)
	{
		main->phone_->PhoneDialTone(TRUE, code);
	}

	SetDialTimer();
}

void CTelephoneDialDlg::HangOff_()
{	
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
//	m_spContactInfo[0] = boost::shared_ptr<Data::ContactInfo>();
//	m_spContactInfo[1] = boost::shared_ptr<Data::ContactInfo>();
	m_MJPGList.SetUnitText(100,L"",true);
	m_MJPGList.SetUnitIsShow(30,false,true);
	m_bsubDial = false ;
	m_bSetGainLevel = false;

}

void CTelephoneDialDlg::DeleteNum()
{	
	CString num = m_MJPGList.GetUnitText(100);
	if (!num.IsEmpty())
	{
		num = num.Mid(0,num.GetLength()-1);
		m_MJPGList.SetUnitText(100,num,true);
		m_sTelephoneNumber = Util::StringOp::FromCString(num );
		SetDialTimer();
	}
}

void CTelephoneDialDlg::SetTel(CString tel)
{	
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	if (!main->m_pTelephoneDlg->m_spContactInfo[main->m_nline])
	{
		main->m_pTelephoneDlg->m_spContactInfo[main->m_nline] = boost::shared_ptr<Data::ContactInfo>(new Data::ContactInfo);
		main->m_pTelephoneDlg->m_spContactInfo[main->m_nline]->type(Data::citOut);
		main->m_pTelephoneDlg->m_spContactInfo[main->m_nline]->startTime(CTime::GetCurrentTime());
		main->m_pTelephoneDlg->m_spContactInfo[main->m_nline]->Insert();
	}	
	main->m_pTelephoneDlg->m_spContactInfo[main->m_nline]->telephoneNumber(Util::StringOp::FromCString(tel));

	m_MJPGList.SetUnitText(100,tel,true);
	m_sTelephoneNumber = Util::StringOp::FromCString(tel);
	m_bFrom10ContactInfo = true ;

	SetDialTimer();
}

void CTelephoneDialDlg::OnTimer(UINT nIDEvent) 
{

	if(nIDEvent == IDT_AUTO_DIAL)
	{
		KillTimer(IDT_AUTO_DIAL);
		OnClickMJPG(18, 0);
	}

//	CCEDialog::OnTimer(nIDEvent);
	CDialog::OnTimer(nIDEvent);
}

void CTelephoneDialDlg::ShowWindow_(int nCmdShow)
{
//	CCEDialog::ShowWindow_(nCmdShow);
	ShowWindow(nCmdShow);

	m_bFrom10ContactInfo = false ;
	m_bMayReturn = false ;
	
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	if ( nCmdShow == SW_SHOW)
	{
		main->m_pMainDlg->SendMessage(WM_PLAYVIDEO, 0, 0); //暂停视频 
	}
	
	if ( !main->GetSimStatus() )
	{
		m_MJPGList.SetUnitIsShow(30,true,true);	
	}
	
//	main->m_pTipDlg->ShowWindow_(SW_HIDE);
//	main->m_pDeleteTipDlg->ShowWindow_(SW_HIDE);

//	::SetWindowPos(m_hWnd, HWND_TOPMOST, 0, 57, 800, 480, 0);

	if(main->m_pMainDlg->m_mainScreenSaveDlg_->m_isScreenPlay)
	{
		main->m_pMainDlg->m_mainScreenSaveDlg_->OnExit();
	}
}

void CTelephoneDialDlg::SubDialShow()
{	
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd; 
	if (main->m_phoneLine[main->m_nline].pFSM->getCurrentState() < CMultimediaPhoneDlg::p3gsConnected)
	{	
		main->m_pWarningNoFlashDlg->SetTitle(L"电话正在连接，不能进行新呼叫!");
		main->m_pWarningNoFlashDlg->ShowWindow_(SW_SHOW);
		return;
	}

	if (!main->FindIdleLine())//查找空闲的线路
	{
		return ;
	}
	main->m_phoneLine[main->m_nline].pFSM->setStartState(CMultimediaPhoneDlg::p3geHangOn);
	m_MJPGList.SetUnitText(100,L"",true);
	m_bMayReturn = true;
	m_bsubDial = true ;
	
	ShowWindow(SW_SHOW);
//	CCEDialog::ShowWindow_(SW_SHOW);
	
	Dprintf("SubDialShow : %d\n",main->m_nline);

}

void CTelephoneDialDlg::OKMessage(WPARAM w, LPARAM l)
{
	if (m_bsubDial)
	{	
		Dprintf("OKMessage\r\n");
		m_bsubDial = false;
		Dial();
	}	
}

void CTelephoneDialDlg::ExitWindow()
{
	CMultimediaPhoneDlg *pMainDlg = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	if (m_bMayReturn)
	{	
		KillTimer(IDT_AUTO_DIAL);
		ShowWindow(SW_HIDE);
		m_bSetGainLevel = false;

		if (m_bsubDial)//二次拨号没有成功
		{	
			pMainDlg->m_phoneLine[pMainDlg->m_nline].pFSM->setStartState(CMultimediaPhoneDlg::p3gsHangOff);
	//		pMainDlg->Swtich2AnotherLine();
			pMainDlg->m_nline = (pMainDlg->m_nline + 1)%2;
			if (pMainDlg->m_phoneLine[pMainDlg->m_nline].pFSM->getCurrentState() ==
				CMultimediaPhoneDlg::p3gsHold)
			{
				pMainDlg->m_pATCommandWarp1->CHLD(2);//恢复上一次的拨号
			}
			
//			change by qi 20100504			
			int g3num = 0;
			pMainDlg->Find3gLineNum(g3num);
			if (g3num > 1)
			{
				pMainDlg->m_pTelephoneDlg->ShowMainInfo();
			}

		}
		else 
		{
			
			if (!pMainDlg->m_bSearchNetOver)//
			{	
				pMainDlg->m_phoneLine[pMainDlg->m_nline].pFSM->setStartState(CMultimediaPhoneDlg::p3gsHangOff);
				pMainDlg->phone_->PhoneDialTone(FALSE, NULL);
				pMainDlg->phone_->Hand(0);
				pMainDlg->phone_->Free(0);
				m_MJPGList.SetUnitText(100,L"",true);
				pMainDlg->PopbackIcon();
				return;
			}
			
			if (!pMainDlg->m_bSearchNetWin)//
			{	
				pMainDlg->m_phoneLine[pMainDlg->m_nline].pFSM->setStartState(CMultimediaPhoneDlg::p3gsHangOff);
				pMainDlg->phone_->PhoneDialTone(FALSE, NULL);
				pMainDlg->phone_->Hand(0);
				pMainDlg->phone_->Free(0);
				m_MJPGList.SetUnitText(100,L"",true);
				pMainDlg->PopbackIcon();
				return;
			}
			
			::PostMessage(pMainDlg->m_hWnd, WM_TEL_HUNGOFF, 0, pMainDlg->m_nline);
			
			pMainDlg->PopbackIcon();

		}
	}

}

void CTelephoneDialDlg::Dial()
{   
	CMultimediaPhoneDlg *pMainDlg = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;

	pMainDlg->phone_->PhoneDialTone(FALSE, NULL);
	KillTimer(IDT_AUTO_DIAL);
	CString s = m_MJPGList.GetUnitText(100);
	if(s.GetLength() > 0)
	{	
		if (s.Mid(0,2) == L"86")
		{
			s = s.Mid(2);
		}
		
		if (m_bFrom10ContactInfo)
		{	
			std::string number = Util::StringOp::FromCString(s);
			pMainDlg->phone_->PhoneDialTone(TRUE, (char *)number.c_str());
			
		}
		pMainDlg->phone_->PhoneDialTone(FALSE, NULL);
		
		pMainDlg->m_pTelephoneDlg->FromTelDial(m_sTelephoneNumber);
		
		//添加图标
		CWnd *p = pMainDlg->m_pTelephoneDlg;
		pMainDlg->AddIcon(Allicon[0],p,false);
		pMainDlg->m_pTelephoneDlg->ShowWindow_(SW_SHOW);
		
		pMainDlg->phone_->DialNumber((char*)(Util::StringOp::FromCString(s)).c_str());
		
		ShowWindow(SW_HIDE);
		pMainDlg->PopbackIcon(this);

	}
		
}

void CTelephoneDialDlg::SetDialTimer()
{	
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	if (main->m_pSettingDlg->m_pSetting->isPhoneDialTimes_)
	{	
		int time = main->m_pSettingDlg->m_pSetting->PhoneDialTimes_ * 1000;
		KillTimer(IDT_AUTO_DIAL);
		SetTimer(IDT_AUTO_DIAL,time, NULL);		
	}
}