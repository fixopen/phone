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
	: CCEDialog(CTelephoneDialDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTelephoneDialDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
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

END_MESSAGE_MAP()

BOOL CTelephoneDialDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_p10ContactInfoDlg = new C10ContactInfoDlg();
	m_p10ContactInfoDlg->Create(C10ContactInfoDlg::IDD);
	m_p10ContactInfoDlg->ShowWindow(SW_HIDE);

	m_MJPGList.Create(L"", WS_VISIBLE|WS_CHILD, CRect(0, 0, 800, 423), this);
	m_MJPGList.SetCurrentLinkFile(".\\adv\\mjpg\\k5\\中文\\电话拨打.xml");
	m_MJPGList.SetMJPGRect(CRect(0, 0, 800, 423));
	
	MoveWindow(0,57,800,423);

	return TRUE ;

}

void CTelephoneDialDlg::OnClickMJPG(WPARAM w, LPARAM l)
{	
	if(!m_bMayReturn)
	{
		return ;
	}

	CMultimediaPhoneDlg *pMainDlg = ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd));
	CString icon;
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
	case 18://3g 拨号
		{
			((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->phone_->PhoneDialTone(FALSE, NULL);
			KillTimer(IDT_AUTO_DIAL);
			CString s = m_MJPGList.GetUnitText(100);
			if(s.GetLength() > 0)
			{	
				//添加图标
				icon = Allicon[0];
				pMainDlg->AddIcon(icon);

				if (m_bFrom10ContactInfo)
				{	
					std::string number = Util::StringOp::FromCString(s);
					((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->PhoneDialTone(TRUE, (char *)number.c_str());
				}
				((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->phone_->PhoneDialTone(FALSE, NULL);

				pMainDlg->m_pTelephoneDlg->FromTelDial(m_spContactInfo,m_sTelephoneNumber);
				pMainDlg->m_pTelephoneDlg->ShowWindow_(SW_SHOW);
				pMainDlg->phone_->DialNumber((char*)(Util::StringOp::FromCString(s)).c_str());

			}
		}
		break;

	case 13:
		pMainDlg->m_pContactDlg->ShowRightBtn(false);
		pMainDlg->m_pContactDlg->ShowWindow_();
		icon = Allicon[2];
		pMainDlg->AddIcon(icon);
		break;
	
	case 14:
		pMainDlg->m_pContactInfoDlg->ShowRightBtn(false);
		pMainDlg->m_pContactInfoDlg->ShowWindow_();
		icon = Allicon[3];
		pMainDlg->AddIcon(icon);
		break;

	case 19://重播
		pMainDlg->m_pTelephoneDlg->Redial();
		break;

	case 21:
		m_p10ContactInfoDlg->Show10ContactInfo();
		break;

	case 1000:
		if (m_bMayReturn)
		{
			ShowWindow(SW_HIDE);
			KillTimer(IDT_AUTO_DIAL);
			pMainDlg->m_pTelephoneDlg->HangOff_(NULL);
			
			if (m_spContactInfo)//号没拨出去删除
			{
				m_spContactInfo->Remove();
			}

			pMainDlg->PopbackIcon();	
		}
		break;
		
	default:
		break;
	}
}

void CTelephoneDialDlg::Key_(void* param)
{
	
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
	number += char(param);
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

	if (!m_spContactInfo)
	{
		m_spContactInfo = boost::shared_ptr<Data::ContactInfo>(new Data::ContactInfo);
		m_spContactInfo->type(Data::citOut);
		m_spContactInfo->startTime(CTime::GetCurrentTime());
		m_spContactInfo->Insert();
	}
	m_spContactInfo->telephoneNumber(Util::StringOp::FromCString(number));
	
	m_sTelephoneNumber = Util::StringOp::FromCString(number);
	

	char code[2] = {0};
	code[0] = char(param);
	((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->PhoneDialTone(TRUE, code);
	KillTimer(IDT_AUTO_DIAL);
	SetTimer(IDT_AUTO_DIAL, 4000, NULL);

}

void CTelephoneDialDlg::HangOff_()
{
	m_spContactInfo = boost::shared_ptr<Data::ContactInfo>();
	m_MJPGList.SetUnitText(100,L"",true);
}

void CTelephoneDialDlg::SetTel(CString tel)
{	
	if (!m_spContactInfo)
	{
		m_spContactInfo = boost::shared_ptr<Data::ContactInfo>(new Data::ContactInfo);
		m_spContactInfo->type(Data::citOut);
		m_spContactInfo->startTime(CTime::GetCurrentTime());
		m_spContactInfo->Insert();
	}	
	m_spContactInfo->telephoneNumber(Util::StringOp::FromCString(tel));

	m_MJPGList.SetUnitText(100,tel,true);
	m_sTelephoneNumber = Util::StringOp::FromCString(tel);
	m_bFrom10ContactInfo = true ;

	SetTimer(IDT_AUTO_DIAL, 4000, NULL);
}

void CTelephoneDialDlg::OnTimer(UINT nIDEvent) 
{

	if(nIDEvent == IDT_AUTO_DIAL)
	{
		KillTimer(IDT_AUTO_DIAL);
		OnClickMJPG(18, 0);
	}

	CCEDialog::OnTimer(nIDEvent);
}

void CTelephoneDialDlg::ShowWindow_(int nCmdShow)
{
	m_bFrom10ContactInfo = false ;
	m_bMayReturn = false ;

	((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pMainDlg->SendMessage(WM_PLAYVIDEO, 0, 0); //暂停视频 

	CCEDialog::ShowWindow_(nCmdShow);
}