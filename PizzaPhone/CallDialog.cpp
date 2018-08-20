// CallDialog.cpp : implementation file
//

#include "stdafx.h"
#include "PizzaPhone.h"
#include "CallDialog.h"


// CallDialog dialog

IMPLEMENT_DYNAMIC(CallDialog, CDialog)

CallDialog::CallDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CallDialog::IDD, pParent)
{

}

CallDialog::~CallDialog()
{
}

void CallDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CallDialog, CDialog)
END_MESSAGE_MAP()


// CallDialog message handlers
