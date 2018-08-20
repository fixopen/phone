// SettingDialog.cpp : implementation file
//

#include "stdafx.h"
#include "PizzaPhone.h"
#include "SettingDialog.h"


// SettingDialog dialog

IMPLEMENT_DYNAMIC(SettingDialog, CDialog)

SettingDialog::SettingDialog(CWnd* pParent /*=NULL*/)
	: CDialog(SettingDialog::IDD, pParent)
{

}

SettingDialog::~SettingDialog()
{
}

void SettingDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(SettingDialog, CDialog)
END_MESSAGE_MAP()


// SettingDialog message handlers
