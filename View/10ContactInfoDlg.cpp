// deletetipdlg.cpp : implementation file
//

#include "stdafx.h"
#include "../multimediaphone.h"
#include "../MultimediaPhoneDlg.h"
#include "10ContactInfoDlg.h"
#include "sip.h"
#include "../Util/stringOp.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CDeleteTipDlg dialog


C10ContactInfoDlg::C10ContactInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(C10ContactInfoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDeleteTipDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	type = 0;
	m_iUnitNo = -1;
}


void C10ContactInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDeleteTipDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(C10ContactInfoDlg, CDialog)

	ON_MESSAGE(WM_CLICKMJPG_TOAPP, OnClickMJPG)
	ON_MESSAGE(WM_MJPGTOGGLE,OnClickMJPG)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDeleteTipDlg message handlers
void C10ContactInfoDlg::OnClickMJPG(WPARAM w, LPARAM l)
{
	switch(w)
	{
	case 2:
	case 12:
	case 22:
	case 32:
	case 42:
	case 52:
	case 62:
	case 72:
	case 82:
	case 92:
		ClickOneItem(w);
		break;

	case 1000:
		ShowWindow(SW_HIDE);
		break;

	case 1001:
		OnBtnOK();
		break;

	default:
		break;
	}

}
BOOL C10ContactInfoDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_MJPGList.Create(L"", WS_VISIBLE|WS_CHILD, CRect(0, 0, 317,376), this);
	m_MJPGList.SetCurrentLinkFile(".\\adv\\mjpg\\k5\\中文\\10条通话记录.xml");
	m_MJPGList.SetMJPGRect(CRect(0, 0, 241+317, 52+376));
	
//	MoveWindow(241,52,317,376);
	MoveWindow(41,20+52,317,376);
	SetPageFont();

//	MoveWindow(180,105,440,270);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void C10ContactInfoDlg::SetMainWnd(CWnd *pwnd)
{
	m_pwnd = pwnd;
}

void C10ContactInfoDlg::FromContactInfoDataBase(void)
{
	if (Data::ContactInfo::GetDataCount("") > 0)
	{	
		std::string filter = "telephoneNumber <> " ;
		filter += "''";		
		m_iCount = Data::ContactInfo::GetDataCount(filter);
		m_vCurrentResult = Data::ContactInfo::GetFromDatabase(filter, Data::dNull, Data::ContactInfo::GetCurrentId() + 1, 10);
	
	}

}

void C10ContactInfoDlg::ShowContactInfo()
{	
	CString in  = L".\\adv\\mjpg\\k5\\common\\电话\\打入电话.png";//呼入
	CString out = L".\\adv\\mjpg\\k5\\common\\电话\\打出电话.png";//呼出
	CString noread = L".\\adv\\mjpg\\k5\\common\\电话\\未接电话.png";//未接
	CString tel ;
	for (int i = 0 ; i < m_vCurrentResult.size() ; i++)
	{
		if ( Data::citInOk == m_vCurrentResult[i]->type())
		{	
			m_MJPGList.SetUnitBitmap(i*10+1,in,L"",false);
		}
		else if ( Data::citInNo == m_vCurrentResult[i]->type())//未接
		{	
			m_MJPGList.SetUnitBitmap(i*10+1,noread,L"",false);
		}
		else if ( Data::citOut == m_vCurrentResult[i]->type()) 
		{
			m_MJPGList.SetUnitBitmap(i*10+1,out,L"",false);
		}

		tel = Util::StringOp::ToCString(m_vCurrentResult[i]->telephoneNumber().number());
		m_MJPGList.SetUnitText(i*10,tel,false);

		m_MJPGList.SetUnitIsShow(i*10+2,true,false);//显示该unit

	}
	m_MJPGList.Invalidate();

}

void C10ContactInfoDlg::ClickOneItem(int item)
{	

// 	if (m_iUnitNo > -1)
// 	{	
// 		m_MJPGList.SetUnitIsDownStatus(m_iUnitNo,false);
// 		m_MJPGList.SetUnitIsShow(m_iUnitNo,true,true);
// 	}

//	m_MJPGList.SetUnitIsDownStatus(item,true);
//	m_MJPGList.SetUnitIsShow(item,true,true);
	
	ShowWindow(SW_HIDE);

// 	if (m_iUnitNo > 0)
// 	{
// 		m_MJPGList.SetUnitIsDownStatus(m_iUnitNo,false);
// 	}
	m_iUnitNo = -1 ;

	CString tel = m_MJPGList.GetUnitText(item-2);
	((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pTelphoneDialDlg->SetTel(tel);

	m_iUnitNo = item ;

}
void C10ContactInfoDlg::Show10ContactInfo()
{	
	ClearPage();

	FromContactInfoDataBase();
	ShowContactInfo();
	ShowWindow(SW_SHOW);

} 

void C10ContactInfoDlg::SetPageFont()
{
	for (int i = 0 ; i < 10 ; i++)
	{
		m_MJPGList.SetUnitFont(i*10,font_20);
	}
	m_MJPGList.Invalidate();
}

void C10ContactInfoDlg::ClearPage()
{	
	int item = 0;
	for (int i = 0 ; i < 10 ;i++)
	{
		m_MJPGList.SetUnitBitmap(item+i,L"",L"",false);
		m_MJPGList.SetUnitText(item+i+1,L"",false);
		m_MJPGList.SetUnitIsShow(item+2,false,false);
		m_MJPGList.SetUnitIsDownStatus(item+2,false);
		item += 10;
	}
	m_MJPGList.Invalidate();

}

void C10ContactInfoDlg::OnBtnOK()
{
	//if (m_iUnitNo > -1)
	//{	
		ShowWindow(SW_HIDE);
	//	CString tel = m_MJPGList.GetUnitText(m_iUnitNo-2);
	//	((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pTelphoneDialDlg->SetTel(tel);
	//}
}


