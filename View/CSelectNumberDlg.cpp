// deletetipdlg.cpp : implementation file
//

#include "stdafx.h"
#include "..\multimediaphone.h"
#include "CSelectNumberDlg.h"
#include "sip.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CDeleteTipDlg dialog


CSelectNumberDlg::CSelectNumberDlg(CWnd* pParent /*=NULL*/)
	: CCEDialog(CSelectNumberDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDeleteTipDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_ntype = 0;
}


void CSelectNumberDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDeleteTipDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSelectNumberDlg, CCEDialog)
	//{{AFX_MSG_MAP(CDeleteTipDlg)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_DELETETIP_OK, OnButtonOk)
	ON_BN_CLICKED(IDC_BUTTON_DELETETIP_CANCEL, OnButtonCancel)
	ON_MESSAGE(WM_CLICKMJPG_TOAPP, OnClickMJPG)
	ON_MESSAGE(WM_MJPGTOGGLE, OnClickMJPG)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDeleteTipDlg message handlers
void CSelectNumberDlg::OnClickMJPG(WPARAM w, LPARAM l)
{
	switch(w)
	{
	case 0:
	case 10:
	case 20:
	case 30:
		ClickBtn(w);
		break;

	case 1001:
		OnButtonOk();
		break;

	case 1000:
		OnButtonCancel();
		break;
	default:
		break;
	}

}
BOOL CSelectNumberDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_MJPGList.Create(L"", WS_VISIBLE|WS_CHILD, CRect(190, 105, 190+440,105+270), this);
	m_MJPGList.SetCurrentLinkFile(".\\adv\\mjpg\\k5\\ÖÐÎÄ\\ºÅÂëÑ¡Ôñ.xml");
	m_MJPGList.SetMJPGRect(CRect(190, 105, 190+440, 105+270));

//	MoveWindow(180,105,440,270);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSelectNumberDlg::OnButtonOk()
{	
	::SendMessage(m_handle,WM_NUMBER_SELECT,0,m_nNumber);
}

void CSelectNumberDlg::OnButtonCancel()
{	
//	::SendMessage(m_handle,WM_NUMBER_SELECT,m_nNumber,0);
	ShowWindow_(SW_HIDE);
}

void CSelectNumberDlg::SetHWnd(HWND handle)
{
	m_handle = handle;
}

void CSelectNumberDlg::SetNumber(std::vector<CString> vNumber)
{	
	m_nNumber = -1 ;
	
	for (int i = 0 ; i < 4 ; i++)
	{
		for (int j = 0 ; j < 2;j++)
		{
			m_MJPGList.SetUnitIsDownStatus(j+i*10,false);
			m_MJPGList.SetUnitIsShow(j+i*10,false,false);
			m_MJPGList.SetUnitText(j+i*10+1,L"",false);
		}	
	}

	for ( i = 0 ; i < vNumber.size() ; i++)
	{
		for (int j = 0 ; j < 2;j++)
		{
			m_MJPGList.SetUnitIsShow(j+i*10,true,false);
			m_MJPGList.SetUnitText(j+i*10+1,vNumber[i],false);
		}
	}

	m_MJPGList.Invalidate();
}

void CSelectNumberDlg::ClickBtn(int item)
{
	for (int i = 0 ; i < 4 ;i++)
	{
		if (m_MJPGList.GetUnitIsDownStatus(i*10))
		{
			m_MJPGList.SetUnitIsDownStatus(i*10,false);
			m_MJPGList.SetUnitIsShow(i*10,true,true);
			break;
		}
	}

	m_MJPGList.SetUnitIsDownStatus(item,true);
	m_MJPGList.SetUnitIsShow(item,true,true);

	m_nNumber = item;
	
}
