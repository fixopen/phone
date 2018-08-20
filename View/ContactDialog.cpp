// contactdialog.cpp : implementation file
//

#include "stdafx.h"
#include "..\MultimediaPhone.h"
#include "ContactDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CContactDialog dialog


CContactDialog::CContactDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CContactDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CContactDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CContactDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CContactDialog)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CContactDialog, CDialog)
	//{{AFX_MSG_MAP(CContactDialog)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CContactDialog message handlers
