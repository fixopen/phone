// TelephoneDialDlg.cpp : implementation file
//

#include "stdafx.h"
#include "..\multimediaphone.h"
#include "../MultimediaPhoneDlg.h"
#include "TelephoneRingDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTelephoneDialDlg dialog


CTelephoneRingDlg::CTelephoneRingDlg(CWnd* pParent /*=NULL*/)
	: CDialog/*CCEDialog*/(CTelephoneRingDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTelephoneDialDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

void CTelephoneRingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTelephoneDialDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTelephoneRingDlg, CDialog)
	//{{AFX_MSG_MAP(CTelephoneDialDlg)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_CLICKMJPG_TOAPP, OnClickMJPG)

END_MESSAGE_MAP()

BOOL CTelephoneRingDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_MJPGList.Create(L"", WS_VISIBLE|WS_CHILD, CRect(0, 0, 800, 423), this);
	m_MJPGList.SetCurrentLinkFile(".\\adv\\mjpg\\k5\\中文\\电话接听.xml");
	m_MJPGList.SetMJPGRect(CRect(0, 0, 800, 423));
	
	HideContact(100,false);
	HideContact(200,false);
	
	RightUnitShow(false);

	MoveWindow(0,57,800,423);

	return TRUE ;

}

void CTelephoneRingDlg::OnClickMJPG(WPARAM w, LPARAM l)
{
	CMultimediaPhoneDlg *main = ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd));
	switch (w)
	{	
	case 1://语音接听
		HandleOn();
		break;

	case 2://可视接听
		ShowWindow_(SW_HIDE);
		break;

	case 3://拒接
		HangOff_();
		break;
		
	case 4://切换
		break;

	case 302://右路接听
		{	
			CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
			if ( !main->phone_->Hand())
			{
				extern void GMute(BOOL isOn);
				GMute(FALSE);//打开speeker
				main->phone_->Free(Telephone::TelephoneWarp::FreeOn);//按过免提
				if(main->m_nTELRigster >= TELRIGSTER_TD)
					Telephone::TelephoneWarp::GetTelephoneWarp()->HandFree(true);//切换到免提
			}

			int line = 0 ;
			for (int i = 0 ; i < 2 ;i++)
			{
				if (main->m_phoneLine[i].pFSM->getCurrentState() == CMultimediaPhoneDlg::p3gsRing)
				{	
					line = i;
					break;
				}
			}

			if (!(main->m_phoneLine[line].b3gFisrtRing))//接3G
			{
				main->m_nline = line ;
				::PostMessage(theApp.m_pMainWnd->m_hWnd, WM_TEL_HUNGON, main->m_nline+1, 1);
	
			}

			if (!(main->m_pTelephoneDlg->m_bPstnFirstRing))//接PSTN
			{
				main->m_nline = 2;
				main->phone_->SWitchPhone3G(FALSE);
				main->phone_->PSTNHangOn();
			}

		}
		break;

	case 303://右路拒接
		{
			int line = 0 ;
			for (int i = 0 ; i < 2 ;i++)
			{
				if (main->m_phoneLine[i].pFSM->getCurrentState() == CMultimediaPhoneDlg::p3gsRing)
				{	
					line = i;
					break;
				}
			}
			
			if (!(main->m_phoneLine[line].b3gFisrtRing))//拒接3G
			{
				::PostMessage(main->m_hWnd, WM_TEL_HUNGOFF, 0, line+1);
				main->phone_->Hangup();
				RightUnitShow(false);
			}
			
			if (!(main->m_pTelephoneDlg->m_bPstnFirstRing))//拒接PSTN
			{
				main->m_pTelephoneDlg->m_bPstnReject = true;
				main->phone_->PSTNRingMute(true);
				RightUnitShow(false);
			}

		}
		break;

	default:
		break;
	}

}

void CTelephoneRingDlg::HangOff_()
{	
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd ;
	
	//3G_1保持或者联通,3G_2振铃 和 PSTN 联通 或者 保持
	if ((main->m_phoneLine[main->m_nline].pFSM->getCurrentState() == CMultimediaPhoneDlg::p3gsConnected ||
		main->m_phoneLine[main->m_nline].pFSM->getCurrentState() == CMultimediaPhoneDlg::p3gsHold) &&
		main->m_phoneLine[(main->m_nline+1)%2].pFSM->getCurrentState() == CMultimediaPhoneDlg::p3gsRing &&
		(main->m_phoneLine[2].pFSM->getCurrentState() == CMultimediaPhoneDlg::pstnsConnected ||
		main->m_phoneLine[2].pFSM->getCurrentState() == CMultimediaPhoneDlg::pstnsHold))
	{
		
	}
	//3G_1保持或者联通,3G_2 Ring 和 PSTN 联通 或者 保持
	else if ((main->m_phoneLine[main->m_nline].pFSM->getCurrentState() == CMultimediaPhoneDlg::p3gsConnected ||
		main->m_phoneLine[main->m_nline].pFSM->getCurrentState() == CMultimediaPhoneDlg::p3gsHold) &&
		main->m_phoneLine[(main->m_nline+1)%2].pFSM->getCurrentState() == CMultimediaPhoneDlg::p3gsRing &&
		(main->m_phoneLine[2].pFSM->getCurrentState() == CMultimediaPhoneDlg::pstnsConnected ||
		main->m_phoneLine[2].pFSM->getCurrentState() == CMultimediaPhoneDlg::pstnsHold))
	{
		
	}
	//3G_1保持或者联通,3G_2 Ring 和 PSTN Ring
	else if ((main->m_phoneLine[main->m_nline].pFSM->getCurrentState() == CMultimediaPhoneDlg::p3gsConnected ||
		main->m_phoneLine[main->m_nline].pFSM->getCurrentState() == CMultimediaPhoneDlg::p3gsHold) &&
		main->m_phoneLine[(main->m_nline+1)%2].pFSM->getCurrentState() == CMultimediaPhoneDlg::p3gsRing &&
		main->m_phoneLine[2].pFSM->getCurrentState() == CMultimediaPhoneDlg::pstnsRing)
	{
		
	}
	//3G_1保持或者联通,3G_2保持或者联通 和 PSTN Ring
	else if ((main->m_phoneLine[main->m_nline].pFSM->getCurrentState() == CMultimediaPhoneDlg::p3gsConnected ||
		main->m_phoneLine[main->m_nline].pFSM->getCurrentState() == CMultimediaPhoneDlg::p3gsHold) &&
		(main->m_phoneLine[(main->m_nline+1)%2].pFSM->getCurrentState() == CMultimediaPhoneDlg::p3gsConnected ||
		main->m_phoneLine[(main->m_nline+1)%2].pFSM->getCurrentState() == CMultimediaPhoneDlg::p3gsConnected )&&
		main->m_phoneLine[2].pFSM->getCurrentState() == CMultimediaPhoneDlg::pstnsRing)
	{
		
	}
	//3G_1保持或者联通,3G_2 Ring 和 PSTN 没联通
	else if ((main->m_phoneLine[main->m_nline].pFSM->getCurrentState() == CMultimediaPhoneDlg::p3gsConnected ||
		main->m_phoneLine[main->m_nline].pFSM->getCurrentState() == CMultimediaPhoneDlg::p3gsHold) &&
		main->m_phoneLine[(main->m_nline+1)%2].pFSM->getCurrentState() == CMultimediaPhoneDlg::p3gsRing &&
		main->m_phoneLine[2].pFSM->getCurrentState() == CMultimediaPhoneDlg::pstnsHangOff)
	{
		Dprintf("hangogg anoter line\r\n");
		int line = (main->m_nline == 0)?2:1;
		
		//add by qi 20100602
		main->m_pTelephoneDlg->SetContactInfoOK(line-1);

		main->m_pATCommandWarp1->DHUP(line);
		//main->phone_->StopRing(true);
		main->PopbackIcon();
		ShowWindow_(SW_HIDE);
		//	main->m_pTelephoneDlg->HandleOff(main->m_nline+1%2);
	}
	//3G_1 Ring,3G_2 Hangoff 和 PSTN Ring
	else if (main->m_phoneLine[main->m_nline].pFSM->getCurrentState() == CMultimediaPhoneDlg::p3gsRing &&
		main->m_phoneLine[(main->m_nline+1)%2].pFSM->getCurrentState() == CMultimediaPhoneDlg::pstnsHangOff &&
		main->m_phoneLine[2].pFSM->getCurrentState() == CMultimediaPhoneDlg::pstnsRing)
	{
		RightReject();	
	}
	//3G_1 Ring 和 //3G_2 HangOFF PSTN 联通 或者 保持
	else if ((main->m_phoneLine[0].pFSM->getCurrentState() == CMultimediaPhoneDlg::p3gsRing ||
		main->m_phoneLine[1].pFSM->getCurrentState() == CMultimediaPhoneDlg::p3gsRing)&&
		(main->m_phoneLine[2].pFSM->getCurrentState() == CMultimediaPhoneDlg::pstnsConnected ||
		main->m_phoneLine[2].pFSM->getCurrentState() == CMultimediaPhoneDlg::pstnsHold ))
	{	
		int line = 1;
		if (main->m_phoneLine[1].pFSM->getCurrentState() == CMultimediaPhoneDlg::p3gsRing)
		{
			line = 2;
		}
		
		//add by qi 20100602
		main->m_pTelephoneDlg->SetContactInfoOK(line-1);

		//main->m_phoneLine[line-1].b3gReject = true;
		::PostMessage(main->m_hWnd, WM_TEL_HUNGOFF, 0, line);
		main->phone_->Hangup();

		main->m_pTelephoneDlg->m_MJPGList.SetUnitText(400,L"通话中...",true);
		main->PopbackIcon();
		ShowWindow_(SW_HIDE);

	}
	//3G_1 联通或者保持 ,3G_2 HangOff,PSTN ring
	else if ((main->m_phoneLine[main->m_nline].pFSM->getCurrentState() == CMultimediaPhoneDlg::p3gsConnected || 
		main->m_phoneLine[main->m_nline].pFSM->getCurrentState() == CMultimediaPhoneDlg::p3gsHold) &&
		main->m_phoneLine[2].pFSM->getCurrentState() == CMultimediaPhoneDlg::pstnsRing )
	{
		main->m_pTelephoneDlg->m_bPstnReject = true;
		main->PopbackIcon();
		ShowWindow_(SW_HIDE);
	}
	//3G_1 Ring
	else if (main->m_phoneLine[main->m_nline].pFSM->getCurrentState() != CMultimediaPhoneDlg::p3gsHangOff)
	{	
		//add by qi 20100602
		main->m_pTelephoneDlg->SetContactInfoOK(main->m_nline);

		main->PopbackIcon();	
		ShowWindow_(SW_HIDE);
		main->m_pTelephoneDlg->HandleOff();

	}
	// Pstn Ring
	else if (main->m_phoneLine[2].pFSM->getCurrentState() != CMultimediaPhoneDlg::pstnsHangOff)
	{
		main->PopbackIcon();	
		ShowWindow_(SW_HIDE);
		main->m_pTelephoneDlg->HandleOff();
	
	}


// 	if (main->m_phoneLine[0].pFSM->getCurrentState() == CMultimediaPhoneDlg::p3gsRing 
// 		&& main->m_phoneLine[2].pFSM->getCurrentState() == CMultimediaPhoneDlg::pstnsRing)
// 	{
// 		if ( m_MJPGList.GetUnitIsShow(100))//3g
// 		{	
// 			::PostMessage(main->m_hWnd, WM_TEL_HUNGOFF, 1, 0);
// 			//main->phone_->Hangup();
// 
// 		}
// 		else//pstn
// 		{	
// 			::PostMessage(main->m_hWnd, WM_TEL_HUNGOFF, 3, 0);
// 			//main->phone_->PSTNHangoff_();
// 		}	
// 	}
// 	else
// 	{
// 		main->PopbackIcon();	
// 		ShowWindow_(SW_HIDE);
// 		main->m_pTelephoneDlg->HandleOff();
// 	}

}

void CTelephoneRingDlg::ShowWindow_(int nCmdShow)
{
//	CCEDialog::ShowWindow_(nCmdShow);
	ShowWindow(nCmdShow);
	
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg *)theApp.m_pMainWnd;
	if(main->m_pMainDlg->m_mainScreenSaveDlg_->m_isScreenPlay)
	{
		main->m_pMainDlg->m_mainScreenSaveDlg_->OnExit();
	}
}

void CTelephoneRingDlg::OnTimer(UINT nIDEvent) 
{
	CDialog::OnTimer(nIDEvent);
	//CCEDialog::OnTimer(nIDEvent);
}

void CTelephoneRingDlg::ShowContact(boost::shared_ptr<Data::Contact> contact, std::string number,int unitNo)
{	
	bool bfresh = false;

	//如果名字太长，把名字变小
	CString name = L"";
	if (contact->name().size() > 0)
	{
		name = Util::StringOp::ToCString(contact->name());
	}
	
	if (name.GetLength() > 10)
	{
		m_MJPGList.SetUnitFont(unitNo,font_18);
	}
	else
	{
		m_MJPGList.SetUnitFont(unitNo,font_26);
	}
	m_MJPGList.SetUnitText(unitNo, name, bfresh);		
	//
	
	//手机1
	std::vector<std::string> vmobile;
	std::string temp;
	CString cs = L"";
	temp = contact->mobilesTelephone().number();
	if (!temp.empty())
	{
		vmobile.push_back(temp);
	}
	
	temp = contact->mobilesTelephone2().number();
	if (!temp.empty())
	{
		vmobile.push_back(temp);
	}

	switch(vmobile.size())
	{	
		case 1:
			{	
				m_MJPGList.SetUnitFont(unitNo+1,font_26);
				cs = CString(vmobile[0].c_str());		
			}
			break;
		case 2:
			{	
				m_MJPGList.SetUnitFont(unitNo+1,font_18);
				cs = CString(vmobile[0].c_str())+L" "+ CString(vmobile[1].c_str());
			}
			break;
	}
	m_MJPGList.SetUnitText(unitNo+1, cs, bfresh);
	
	//固话
	if (vmobile.size())
	{
		vmobile.clear();
	}
	cs = L"";

	temp = contact->worksTelephone().number();
	if (!temp.empty())
	{
		vmobile.push_back(temp);
	}
	
	temp = contact->worksTelephone2().number();
	if (!temp.empty())
	{
		vmobile.push_back(temp);
	}
	
	switch(vmobile.size())
	{	
	case 1:
		{	
			m_MJPGList.SetUnitFont(unitNo+2,font_26);
			cs = CString(vmobile[0].c_str());		
		}
		break;
	case 2:
		{	
			m_MJPGList.SetUnitFont(unitNo+2,font_18);
			cs = CString(vmobile[0].c_str())+L" "+ CString(vmobile[1].c_str());
		}
		break;
	}

	m_MJPGList.SetUnitText(unitNo+2, cs, bfresh);


	m_MJPGList.SetUnitText(unitNo+3, CString(contact->company().c_str()), bfresh);
	m_MJPGList.SetUnitText(unitNo+4, CString(contact->department().c_str()), bfresh);
	m_MJPGList.SetUnitText(unitNo+5, CString(contact->email().c_str()), bfresh);
	m_MJPGList.SetUnitText(unitNo+6, CString(contact->duty().c_str()), bfresh);
	m_MJPGList.SetUnitText(unitNo+7, CString(contact->memo().c_str()), bfresh);

}

void CTelephoneRingDlg::HideContact(int uintNo,bool bshow )
{
	for (int i = 0 ; i < 8 ; i++)
	{
		m_MJPGList.SetUnitIsShow(uintNo+i,!bshow,true);		
	}
	
	if ( 100 == uintNo)
	{
		m_MJPGList.SetUnitIsShow(6,!bshow,true);
	}
	else
	{	
		m_MJPGList.SetUnitIsShow(7,!bshow,true);
	}

}

void CTelephoneRingDlg::Switch()
{	
	if ( m_MJPGList.GetUnitIsShow(100) )
	{	
		HideContact(100,true);
		HideContact(200,false);
	}
	else
	{	
		HideContact(100,false);
		HideContact(200,true);
	}

}

void CTelephoneRingDlg::HandleOn()
{	
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	if ( !main->phone_->Hand())
	{
		extern void GMute(BOOL isOn);
		GMute(FALSE);//打开speeker
		main->phone_->Free(Telephone::TelephoneWarp::FreeOn);//按过免提
		if(main->m_nTELRigster >= TELRIGSTER_TD)
			Telephone::TelephoneWarp::GetTelephoneWarp()->HandFree(true);//切换到免提
	}
	
	//3G_1保持或者联通,3G_2振铃 和 PSTN 联通 或者 保持
	if ((main->m_phoneLine[main->m_nline].pFSM->getCurrentState() == CMultimediaPhoneDlg::p3gsConnected ||
		main->m_phoneLine[main->m_nline].pFSM->getCurrentState() == CMultimediaPhoneDlg::p3gsHold) &&
		main->m_phoneLine[(main->m_nline+1)%2].pFSM->getCurrentState() == CMultimediaPhoneDlg::p3gsRing &&
		(main->m_phoneLine[2].pFSM->getCurrentState() == CMultimediaPhoneDlg::pstnsConnected ||
		main->m_phoneLine[2].pFSM->getCurrentState() == CMultimediaPhoneDlg::pstnsHold))
	{
		
	}
	//3G_1保持或者联通,3G_2 Ring 和 PSTN 联通 或者 保持
	else if ((main->m_phoneLine[main->m_nline].pFSM->getCurrentState() == CMultimediaPhoneDlg::p3gsConnected ||
		main->m_phoneLine[main->m_nline].pFSM->getCurrentState() == CMultimediaPhoneDlg::p3gsHold) &&
		main->m_phoneLine[(main->m_nline+1)%2].pFSM->getCurrentState() == CMultimediaPhoneDlg::p3gsRing &&
		(main->m_phoneLine[2].pFSM->getCurrentState() == CMultimediaPhoneDlg::pstnsConnected ||
		main->m_phoneLine[2].pFSM->getCurrentState() == CMultimediaPhoneDlg::pstnsHold))
	{
		
	}
	//3G_1保持或者联通,3G_2 Ring 和 PSTN Ring
	else if ((main->m_phoneLine[main->m_nline].pFSM->getCurrentState() == CMultimediaPhoneDlg::p3gsConnected ||
		main->m_phoneLine[main->m_nline].pFSM->getCurrentState() == CMultimediaPhoneDlg::p3gsHold) &&
		main->m_phoneLine[(main->m_nline+1)%2].pFSM->getCurrentState() == CMultimediaPhoneDlg::p3gsRing &&
		main->m_phoneLine[2].pFSM->getCurrentState() == CMultimediaPhoneDlg::pstnsRing)
	{
		
	}
	//3G_1保持或者联通,3G_2保持或者联通 和 PSTN Ring
	else if ((main->m_phoneLine[main->m_nline].pFSM->getCurrentState() == CMultimediaPhoneDlg::p3gsConnected ||
		main->m_phoneLine[main->m_nline].pFSM->getCurrentState() == CMultimediaPhoneDlg::p3gsHold) &&
		(main->m_phoneLine[(main->m_nline+1)%2].pFSM->getCurrentState() == CMultimediaPhoneDlg::p3gsConnected ||
		main->m_phoneLine[(main->m_nline+1)%2].pFSM->getCurrentState() == CMultimediaPhoneDlg::p3gsConnected )&&
		main->m_phoneLine[2].pFSM->getCurrentState() == CMultimediaPhoneDlg::pstnsRing)
	{
		
	}
	//3G_1保持或者联通,3G_2 Ring 和 PSTN 没联通
	else if ((main->m_phoneLine[main->m_nline].pFSM->getCurrentState() == CMultimediaPhoneDlg::p3gsConnected ||
		main->m_phoneLine[main->m_nline].pFSM->getCurrentState() == CMultimediaPhoneDlg::p3gsHold) &&
		main->m_phoneLine[(main->m_nline+1)%2].pFSM->getCurrentState() == CMultimediaPhoneDlg::p3gsRing &&
		main->m_phoneLine[2].pFSM->getCurrentState() == CMultimediaPhoneDlg::pstnsHangOff)
	{
		main->phone_->SWitchPhone3G(TRUE);
		int line = (main->m_nline == 0)?1:2;
		main->m_phoneLine[(main->m_nline+1)%2].pFSM->fireEvent(CMultimediaPhoneDlg::p3gsHangOn,(void *)((main->m_nline+1)%2));
		
	}
	//3G_1 Ring,3G_2 Hangoff 和 PSTN Ring
	else if (main->m_phoneLine[main->m_nline].pFSM->getCurrentState() == CMultimediaPhoneDlg::p3gsRing &&
		main->m_phoneLine[(main->m_nline+1)%2].pFSM->getCurrentState() == CMultimediaPhoneDlg::pstnsHangOff &&
		main->m_phoneLine[2].pFSM->getCurrentState() == CMultimediaPhoneDlg::pstnsRing)
	{
		int line = 0 ;
		for (int i = 0 ; i < 2 ;i++)
		{
			if (main->m_phoneLine[i].pFSM->getCurrentState() == CMultimediaPhoneDlg::p3gsRing)
			{	
				line = i;
				break;
			}
		}
		
		if (main->m_phoneLine[line].b3gFisrtRing)//接3G
		{
			main->m_nline = 0 ;
			::PostMessage(theApp.m_pMainWnd->m_hWnd, WM_TEL_HUNGON, main->m_nline+1, 1);	
		}
		
		if (main->m_pTelephoneDlg->m_bPstnFirstRing)//接PSTN
		{
			main->m_nline = 2;
			main->phone_->SWitchPhone3G(FALSE);
			main->phone_->PSTNHangOn();
		}
			
	}
	//3G_1 Ring 和 3G_2 HangOFF ,PSTN 联通 或者 保持
	else if ((main->m_phoneLine[0].pFSM->getCurrentState() == CMultimediaPhoneDlg::p3gsRing || 
		main->m_phoneLine[1].pFSM->getCurrentState() == CMultimediaPhoneDlg::p3gsRing)&&
		(main->m_phoneLine[2].pFSM->getCurrentState() == CMultimediaPhoneDlg::pstnsConnected ||
		main->m_phoneLine[2].pFSM->getCurrentState() == CMultimediaPhoneDlg::pstnsHold ))
	{
		//先挂断PSTN
		main->m_pTelephoneDlg->PstnHangOff_(NULL);
		main->phone_->PSTNHangoff_();
		main->m_nline = 0;
		if (main->m_phoneLine[1].pFSM->getCurrentState() == CMultimediaPhoneDlg::p3gsRing)
		{
			main->m_nline = 1;
		}
		main->m_pTelephoneDlg->ShowWindow(SW_HIDE);
		::PostMessage(theApp.m_pMainWnd->m_hWnd, WM_TEL_HUNGON, main->m_nline+1, 0);
		
	}
	//3G_1 联通或者保持 ,3G_2 HangOff,PSTN ring
	else if ((main->m_phoneLine[main->m_nline].pFSM->getCurrentState() == CMultimediaPhoneDlg::p3gsConnected || 
		main->m_phoneLine[main->m_nline].pFSM->getCurrentState() == CMultimediaPhoneDlg::p3gsHold) &&
		main->m_phoneLine[2].pFSM->getCurrentState() == CMultimediaPhoneDlg::pstnsRing )
	{
		main->m_pTelephoneDlg->G3HangOff(main->m_nline);
		main->phone_->Hangup();//挂断3G
		
		main->m_nline = 2;
		main->phone_->SWitchPhone3G(FALSE);
		main->phone_->PSTNHangOn();
// 		main->m_phoneLine[2].pFSM->fireEvent(CMultimediaPhoneDlg::
// 			pstneHangOn,NULL);
				
	}
	//3G_1 Ring ,3G_2 HangOff,PSTN Contected
	else if (main->m_phoneLine[2].pFSM->getCurrentState() == CMultimediaPhoneDlg::p3gsConnected &&
		main->m_phoneLine[0].pFSM->getCurrentState() == CMultimediaPhoneDlg::pstnsRing )
	{
		main->phone_->SWitchPhone3G(TRUE);
		::PostMessage(theApp.m_pMainWnd->m_hWnd, WM_TEL_HUNGON, 1, 0);			
	}
	//3G_1 Ring
	else if (main->m_phoneLine[0].pFSM->getCurrentState() != CMultimediaPhoneDlg::p3gsHangOff)
	{	
		//main->phone_->SWitchPhone3G(TRUE);
		main->m_nline = 0 ;
		::PostMessage(theApp.m_pMainWnd->m_hWnd, WM_TEL_HUNGON, main->m_nline+1, 0);
		
	}
	// Pstn Ring
	else if (main->m_phoneLine[2].pFSM->getCurrentState() != CMultimediaPhoneDlg::pstnsHangOff)
	{
		main->m_nline = 2;
		main->phone_->SWitchPhone3G(FALSE);
		main->phone_->PSTNHangOn();
// 		main->m_phoneLine[2].pFSM->fireEvent(CMultimediaPhoneDlg::
// 			pstneHangOn,NULL);
	}


// 	extern void GMute(BOOL isOn);
// 	GMute(FALSE);//打开speeker
// 	main->phone_->Free(Telephone::TelephoneWarp::FreeOn);//按过免提
// 	if(main->m_nTELRigster >= TELRIGSTER_TD)
// 	Telephone::TelephoneWarp::GetTelephoneWarp()->HandFree(true);//切换到免提
// 	
// 	if (main->m_phoneLine[0].pFSM->getCurrentState() != CMultimediaPhoneDlg::p3gsHangOff &&
// 		main->m_phoneLine[2].pFSM->getCurrentState() != CMultimediaPhoneDlg::pstnsHangOff)
// 	{
// 		if ( m_MJPGList.GetUnitIsShow(100) )
// 		{	
// 			main->phone_->SWitchPhone3G(TRUE);
// 			::PostMessage(theApp.m_pMainWnd->m_hWnd, WM_TEL_HUNGON, 1, 0);
// 
// 		}
// 		else
// 		{
// 			main->phone_->SWitchPhone3G(FALSE);
// 			main->phone_->PSTNHangOn();
// 			main->m_phoneLine[2].pFSM->fireEvent(CMultimediaPhoneDlg::
// 			pstneHangOn,NULL);
// 
// 		}
// 
// 	}
// 	//3G_1 Ring
// 	else if (main->m_phoneLine[main->m_nline].pFSM->getCurrentState() != CMultimediaPhoneDlg::p3gsHangOff)
// 	{
// 		main->phone_->SWitchPhone3G(TRUE);
// 		::PostMessage(theApp.m_pMainWnd->m_hWnd, WM_TEL_HUNGON, 1, 0);
// 
// 	}
// 	else if (main->m_phoneLine[2].pFSM->getCurrentState() != CMultimediaPhoneDlg::pstnsHangOff)
// 	{
// 		main->phone_->SWitchPhone3G(FALSE);
// 		main->phone_->PSTNHangOn();
// 		main->m_phoneLine[2].pFSM->fireEvent(CMultimediaPhoneDlg::
// 			pstneHangOn,NULL);
// 
// 	}

}

void CTelephoneRingDlg::ClearData(int unitNo)
{	
	for (int i = 0 ; i < 8;i++)
	{
		m_MJPGList.SetUnitText(unitNo+i,L"", TRUE);	
	}

}

void CTelephoneRingDlg::InitData()
{
	m_MJPGList.SetUnitIsShow(4,false,true);
	m_MJPGList.SetUnitIsShow(5,false,true);

}

void CTelephoneRingDlg::RightReject()
{
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	int line = 0 ;
	for (int i = 0 ; i < 2 ;i++)
	{
		if (main->m_phoneLine[i].pFSM->getCurrentState() == CMultimediaPhoneDlg::p3gsRing)
		{	
			line = i;
			break;
		}
	}
	
	if (main->m_phoneLine[line].b3gFisrtRing)//拒接3G
	{
		//add by qi 20100602
		main->m_pTelephoneDlg->SetContactInfoOK(line);
		main->phone_->StopRing();

		::PostMessage(main->m_hWnd, WM_TEL_HUNGOFF, 0, line+1);
		main->phone_->Hangup();
		RightUnitShow(false);
	}
	
	if (main->m_pTelephoneDlg->m_bPstnFirstRing)//拒接PSTN,PSTN先来的
	{
		main->m_pTelephoneDlg->m_bPstnFirstRing = false;
		main->m_pTelephoneDlg->m_bPstnReject = true;
		main->m_phoneLine[line].b3gFisrtRing = true ;
		main->phone_->PSTNRingMute(true);
		RightUnitShow(false);
		main->m_pTelephoneDlg->Show3gInfo(line);
	
	}
}

void CTelephoneRingDlg::RightUnitShow(bool bshow)
{	
	if (!bshow)
	{
		m_MJPGList.SetUnitText(300,L"",true);
		m_MJPGList.SetUnitText(301,L"",true);
	}
	
	for (int i = 0 ;i < 4; i++)
	{		
		m_MJPGList.SetUnitIsShow(300+i,bshow);
	}

	m_MJPGList.Invalidate();
}

