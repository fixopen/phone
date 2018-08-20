// ContactsDialog.cpp : implementation file
//

#include "stdafx.h"
#include "PizzaPhone.h"
#include "ContactsDialog.h"


// ContactsDialog dialog

IMPLEMENT_DYNAMIC(ContactsDialog, CDialog)

ContactsDialog::ContactsDialog(CWnd* pParent /*=NULL*/)
	: CDialog(ContactsDialog::IDD, pParent)
{

}

ContactsDialog::~ContactsDialog()
{
}

void ContactsDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(ContactsDialog, CDialog)
END_MESSAGE_MAP()


// ContactsDialog message handlers
