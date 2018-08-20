// PizzaPhoneDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PizzaPhone.h"
#include "PizzaPhoneDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CPizzaPhoneDlg dialog

CPizzaPhoneDlg::CPizzaPhoneDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPizzaPhoneDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPizzaPhoneDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CPizzaPhoneDlg, CDialog)
#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
	ON_WM_SIZE()
#endif
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// CPizzaPhoneDlg message handlers

BOOL CPizzaPhoneDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
void CPizzaPhoneDlg::OnSize(UINT /*nType*/, int /*cx*/, int /*cy*/)
{
	if (AfxIsDRAEnabled())
	{
		DRA::RelayoutDialog(
			AfxGetResourceHandle(), 
			this->m_hWnd, 
			DRA::GetDisplayMode() != DRA::Portrait ? 
			MAKEINTRESOURCE(IDD_PIZZAPHONE_DIALOG_WIDE) : 
			MAKEINTRESOURCE(IDD_PIZZAPHONE_DIALOG));
	}
}
#endif

