// ReviewCallDialog.cpp : implementation file
//

#include "stdafx.h"
#include "PizzaPhone.h"
#include "ReviewCallDialog.h"


// ReviewCallDialog dialog

IMPLEMENT_DYNAMIC(ReviewCallDialog, CDialog)

ReviewCallDialog::ReviewCallDialog(CWnd* pParent /*=NULL*/)
	: CDialog(ReviewCallDialog::IDD, pParent)
{

}

ReviewCallDialog::~ReviewCallDialog()
{
}

void ReviewCallDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(ReviewCallDialog, CDialog)
END_MESSAGE_MAP()


// ReviewCallDialog message handlers
