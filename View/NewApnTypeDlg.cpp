// RenameDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MultimediaPhone.h"
#include "NewApnTypeDlg.h"
#include "../resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRenameDlg dialog


CNewApnTypeDlg::CNewApnTypeDlg(CWnd* pParent /*=NULL*/)
	: CCEDialog(CNewApnTypeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRenameDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CNewApnTypeDlg::DoDataExchange(CDataExchange* pDX)
{
	CCEDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRenameDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

void CNewApnTypeDlg::SetWndParam(CWnd *pWnd, int index, CString sName)
{
	m_pOwner = pWnd;
	m_oldName = sName;
	m_index = index;
	
}

void CNewApnTypeDlg::OnClickMJPG(WPARAM w, LPARAM l)
{
	switch(w)
	{
	case 1001:
		{
			CString APN;
			m_cApn.GetWindowText(APN);
			if (!APN.IsEmpty())
			{	
				m_pOwner->SendMessage(WM_NEW_APN, m_index, 0);
				m_cApn.SetWindowText(L"");
				ShowWindow_(SW_HIDE);	
			}
		}
		break;
	case 1000:
		ShowWindow_(SW_HIDE);
		break;
	}
}

BEGIN_MESSAGE_MAP(CNewApnTypeDlg, CCEDialog)
	//{{AFX_MSG_MAP(CRenameDlg)
	ON_MESSAGE(WM_CLICKMJPG_TOAPP, OnClickMJPG)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRenameDlg message handlers

BOOL CNewApnTypeDlg::OnInitDialog() 
{
	CCEDialog::OnInitDialog();
	
	int xbegin = 190;
	int ybegin = 105;

	// TODO: Add extra initialization here
	m_cApn.Create(WS_CHILD|WS_VISIBLE, CRect(xbegin+153, ybegin+148, xbegin+153+255, ybegin+148+40), this, IDC_EDIT_NEW_APN);

	m_MJPGList.Create(L"", WS_VISIBLE|WS_CHILD, CRect(xbegin, ybegin, xbegin + 440, ybegin+270), this);
	m_MJPGList.SetCurrentLinkFile(".\\adv\\mjpg\\k5\\ÖÐÎÄ\\ÐÞ¸ÄAPN.xml");
	m_MJPGList.SetMJPGRect(CRect(xbegin, ybegin, xbegin + 440, ybegin+270));

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
