// TestCEDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MultimediaPhone.h"
#include "TestCEDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTestCEDlg dialog


CTestCEDlg::CTestCEDlg(CWnd* pParent /*=NULL*/)
	: CCEDialog(CTestCEDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTestCEDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CTestCEDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTestCEDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTestCEDlg, CCEDialog)
	//{{AFX_MSG_MAP(CTestCEDlg)
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestCEDlg message handlers

void CTestCEDlg::OnOK() 
{
	// TODO: Add extra validation here
	
//	CCEDialog::OnOK();
//	EndDialog(0);
	this->ShowWindow(FALSE);
}

void CTestCEDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	this->ShowWindow(FALSE);
//	EndDialog(0);
}

void CTestCEDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CCEDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	
}
