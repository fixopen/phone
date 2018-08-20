// deletetipdlg.cpp : implementation file
//

#include "stdafx.h"
#include "..\multimediaphone.h"
#include "..\MultimediaPhoneDlg.h"
#include "StorageStatusDlg.h"
#include "Pblmember.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CDeleteTipDlg dialog


CStorageStatusDlg::CStorageStatusDlg(CWnd* pParent /*=NULL*/)
	: CCEDialog(CStorageStatusDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDeleteTipDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CStorageStatusDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDeleteTipDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStorageStatusDlg, CCEDialog)

	ON_MESSAGE(WM_CLICKMJPG_TOAPP, OnClickMJPG)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDeleteTipDlg message handlers
void CStorageStatusDlg::OnClickMJPG(WPARAM w, LPARAM l)
{
	switch(w)
	{
	case 1001:
		OnClickOK();
		break;
	default:
		break;
	}
}

BOOL CStorageStatusDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_MJPGList.Create(L"", WS_VISIBLE|WS_CHILD, CRect(190, 105, 190+440,105+270), this);
	m_MJPGList.SetCurrentLinkFile(".\\adv\\mjpg\\k5\\中文\\存储状态.xml");
	m_MJPGList.SetMJPGRect(CRect(190, 105, 190+440, 105+270));

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CStorageStatusDlg::OnClickOK()
{	
	ShowWindow_(SW_HIDE);		
}

void CStorageStatusDlg::SetStorage(StorStaus ss,bool bstor)
{	
	CString status;
	status.Format(L"%d/%d",ss.nreciveUsed,ss.nreciveSize);
	m_MJPGList.SetUnitText(10,status,false);

	status.Format(L"%d/%d",ss.nsendUsed,ss.nsendSize);
	m_MJPGList.SetUnitText(11,status,false);

	status.Format(L"%d/%d",ss.ndraftUsed,ss.ndraftSize);
	m_MJPGList.SetUnitText(12,status,false);

	if (bstor)
	{	
		//存储类型
		m_MJPGList.SetUnitText(20,L"类型:彩信",false);

		m_MJPGList.SetUnitIsShow(3,true);
		
		//设置声誉空间
		CString c;
		ULONGLONG ufree;
		ULONGLONG utotal;
		ULONGLONG uleft = 0;

		extern void GetDiskStatus(ULONGLONG &free,ULONGLONG &total,ULONGLONG &uleft);
		GetDiskStatus(ufree,utotal,uleft);

// 		if (ufree > utotal/10.00)
// 		{
// 			uleft = ufree - utotal/10.00;
// 		}
		
		if (uleft > 1024 * 1024)
		{
			c.Format(L"%.2fMB",uleft/(1024*1024.00));	
		}
		else if (uleft > 1024)
		{
			c.Format(L"%.2fkB",uleft/1024.00);	
		}
		else if (uleft < 1024)
		{
			c.Format(L"%.2fB",uleft);
		}
		m_MJPGList.SetUnitText(13,c,true);

	}
	else
	{	
		//存储类型
		m_MJPGList.SetUnitText(20,L"类型:短信",false);

		m_MJPGList.SetUnitIsShow(3,false);
		m_MJPGList.SetUnitText(13,L"",false);
	}

	m_MJPGList.Invalidate();
}