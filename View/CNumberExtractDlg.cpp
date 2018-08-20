// deletetipdlg.cpp : implementation file
//

#include "stdafx.h"
#include "../multimediaphone.h"
#include "../MultimediaPhoneDlg.h"
#include "CNumberExtractDlg.h"
#include "../Util/stringOp.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CDeleteTipDlg dialog


CNumberExtractDlg::CNumberExtractDlg(CWnd* pParent /*=NULL*/)
: CCEDialog(CNumberExtractDlg::IDD, pParent)
,m_nPagesize(4)
{
	//{{AFX_DATA_INIT(CDeleteTipDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	type = 0;
}

void CNumberExtractDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDeleteTipDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNumberExtractDlg, CCEDialog)

	ON_MESSAGE(WM_CLICKMJPG_TOAPP, OnClickMJPG)
	ON_MESSAGE(WM_MJPGTOGGLE,OnClickMJPG)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDeleteTipDlg message handlers
void CNumberExtractDlg::OnClickMJPG(WPARAM w, LPARAM l)
{
	switch(w)
	{
	case 1://新建名片
		NewContact();
		break;

	case 2://更改已有的名片
		OldContact();
		break;

	case 10://上翻页
		PageUp();
		break;

	case 11://下翻页
		PageDown();
		break;

	case 12://固话
	case 13:
	case 14:
	case 15:
		SelectContact(w);
		break;

	case 20://号码1-号码4
	case 30:
	case 40:
	case 50:
		ClickNumber(w);		
		break;

	case 1000:
		ShowWindow_(SW_HIDE);
		break;

	case 1001:
		break;

	default:
		break;
	}
}
BOOL CNumberExtractDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_MJPGList.Create(L"", WS_VISIBLE|WS_CHILD, CRect(180, 20, 180+440,20+440), this);
	m_MJPGList.SetCurrentLinkFile(".\\adv\\mjpg\\k5\\中文\\号码提取.xml");
	m_MJPGList.SetMJPGRect(CRect(180, 20, 180+440,20+440));
	
//	MoveWindow(180,105,440,270);
	
	m_nItem		= -1;
	m_ncontact  = -1;
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CNumberExtractDlg::SetMainWnd(CWnd *pwnd)
{
	m_pwnd = pwnd;
}

void CNumberExtractDlg::SetNumber(std::vector<CString> vNumber)
{	
	m_nItem		= -1;
	m_ncontact  = -1;

	if (m_vNumber.size())
	{
		m_vNumber.clear();
	}
	m_iCurrentPage = 1;

	m_vNumber.swap(vNumber);

	m_MJPGList.SetUnitIsShow(10,false,true);
	m_MJPGList.SetUnitIsShow(11,false,true);
	
	if (m_vNumber.size() > m_nPagesize)
	{
		m_MJPGList.SetUnitIsShow(11,true,true);
	}

	m_iTotalPage = m_vNumber.size()/m_nPagesize ;
	if (vNumber.size()%m_nPagesize != 1)
	{
		m_iTotalPage += 1;
	}

}

void CNumberExtractDlg::PageDown()
{
	m_iCurrentPage++;
	if (m_iCurrentPage > m_iTotalPage )
	{
		m_iCurrentPage--;
		return ;
	}
	
	if ((m_iCurrentPage+1) <= m_iTotalPage )
	{
		m_MJPGList.SetUnitIsShow(11,true,true);		
	}
	else
	{
		m_MJPGList.SetUnitIsShow(11,false,true);		
	}
	m_MJPGList.SetUnitIsShow(10,true,true);

	ShowNumber();
}

void CNumberExtractDlg::PageUp()
{
	m_iCurrentPage--;
	if (m_iCurrentPage < 1)
	{
		m_iCurrentPage++;
		return ;
	}
	
	if ((m_iCurrentPage - 1) >= 1)
	{
		m_MJPGList.SetUnitIsShow(10,true,true);		
	}
	else
	{
		m_MJPGList.SetUnitIsShow(10,false,true);		
	}
	m_MJPGList.SetUnitIsShow(11,true,true);

	ShowNumber();
}

void CNumberExtractDlg::ShowNumber()
{
	int item = GetItems();
	
	for (int i = 0 ; i < m_nPagesize ;i++)//清空
	{
		m_MJPGList.SetUnitIsShow(i*10+20,false,false);
		m_MJPGList.SetUnitIsDownStatus(i*10+20,false);
		m_MJPGList.SetUnitText(i*10+21,L"",false);		
	}
	
	m_MJPGList.SetUnitIsDownStatus(12,false);
	m_MJPGList.SetUnitIsDownStatus(13,false);
	m_MJPGList.SetUnitIsDownStatus(14,false);
	m_MJPGList.SetUnitIsDownStatus(15,false);

	for (i = 0 ; i < item ;i++)
	{
		m_MJPGList.SetUnitIsShow(i*10+20,true,false);
		m_MJPGList.SetUnitText(i*10+21,m_vNumber[(m_iCurrentPage-1)*m_nPagesize+i],false);
	}
	
	m_MJPGList.Invalidate();
}
void CNumberExtractDlg::ClickNumber(int item)
{	
	if (m_nItem > -1 )
	{
		m_MJPGList.SetUnitIsDownStatus(m_nItem,false);
		m_MJPGList.SetUnitIsShow(m_nItem,true,true);
	}

	m_MJPGList.SetUnitIsDownStatus(item,true);
	m_MJPGList.SetUnitIsShow(item,true,true);

	m_nItem = item ;	
}

void CNumberExtractDlg::SelectContact(int item)
{
	if (m_ncontact > -1 )
	{
		m_MJPGList.SetUnitIsDownStatus(m_ncontact,false);
		m_MJPGList.SetUnitIsShow(m_ncontact,true,true);
	}
	
	m_MJPGList.SetUnitIsDownStatus(item,true);
	m_MJPGList.SetUnitIsShow(item,true,true);
	
	m_ncontact = item ;
}

void CNumberExtractDlg::OnBtnOk()
{
	if (m_nItem && m_ncontact )
	{
		
	}
}

void CNumberExtractDlg::NewContact()
{
	if (m_nItem > -1 && m_ncontact > -1 )
	{		
		CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg *)theApp.m_pMainWnd ;
		std::string number = Util::StringOp::FromCString(m_MJPGList.GetUnitText(m_nItem+1));
		int index = m_ncontact - 11;

		Data::TelephoneNumber telnumber ;
		telnumber.number(number);
		boost::shared_ptr<Data::Contact> contact(new Data::Contact);
		contact->id(0);
		if ( 1 == index )
		{	
			contact->mobilesTelephone(telnumber);
		}
		else if( 2 == index )
		{
			contact->mobilesTelephone2(telnumber);	
		}
		if ( 3 == index )
		{
			contact->worksTelephone(telnumber);	
		}
		else if (4 == index )
		{
			contact->worksTelephone2(telnumber);	
		}

		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactNewDlg->SetContact(contact);
		::SetFocus(((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactNewDlg->m_hWnd);
		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactNewDlg->ShowWindow(true);
		ShowWindow_(SW_HIDE);
	}
}

void CNumberExtractDlg::OldContact()
{
	if (m_nItem  > -1 && m_ncontact > -1 )
	{
		CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg *)theApp.m_pMainWnd ;
		
		std::string number = Util::StringOp::FromCString(m_MJPGList.GetUnitText(m_nItem+1));
		int index = m_ncontact - 11;

		main->m_pContactDlg->SetType(3);
		main->m_pContactDlg->SetNumber(number,index);
		main->m_pContactDlg->ShowRightBtn(false);
		main->m_pContactDlg->ShowWindow_();
		
		ShowWindow_(SW_HIDE);
		CWnd *p = main->m_pContactDlg;
		main->AddIcon(Allicon[2],p,false);
	}

}

int CNumberExtractDlg::GetItems()
{
	int item = 0 ;
	if (m_vNumber.size() > (m_iCurrentPage-1)*m_nPagesize)
	{
		item = m_vNumber.size() - (m_iCurrentPage-1)*m_nPagesize ;
	}

	if (item > m_nPagesize)
	{
		item = m_nPagesize ;
	}
	return item ;
}










