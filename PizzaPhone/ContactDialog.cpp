// ContactDialog.cpp : implementation file
//

#include "stdafx.h"
#include "PizzaPhone.h"
#include "ContactDialog.h"


// ContactDialog dialog

IMPLEMENT_DYNAMIC(ContactDialog, CDialog)

ContactDialog::ContactDialog(CWnd* pParent /*=NULL*/)
	: CDialog(ContactDialog::IDD, pParent)
{

}

ContactDialog::~ContactDialog()
{
}

void ContactDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(ContactDialog, CDialog)
END_MESSAGE_MAP()


// ContactDialog message handlers
