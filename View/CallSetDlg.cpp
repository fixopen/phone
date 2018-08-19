#include "stdafx.h"
#include "..\multimediaphone.h"
#include "..\MultimediaPhoneDlg.h"
#include "CallSetDlg.h"
#include "../Data/LanguageResource.h"
#include "../Data/SkinStyle.h"
#include "sip.h"
//#include "../Util/ATCommandWarp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCallSetDlg dialog

CCallSetDlg::CCallSetDlg(CWnd* pParent /*=NULL*/)
	: CCEDialog(CCallSetDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCallSetDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CCallSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCallSetDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCallSetDlg, CCEDialog)
	//{{AFX_MSG_MAP(CCallSetDlg)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_FASTDIALS_OK, OnButtonFastDialsOk)
	ON_BN_CLICKED(IDC_BUTTON_FASTDIALS_CANCEL, OnButtonFastDialsCancel)
	ON_MESSAGE(WM_CLICKMJPG_TOAPP, OnClickMJPG)
	ON_MESSAGE(WM_MJPGTOGGLE, OnClickMJPG)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCallSetDlg message handlers
void CCallSetDlg::OnClickMJPG(WPARAM w, LPARAM l)
{
	switch(w)
	{
	case 10:
	case 41://呼叫等待设置
		Start();
		break;

	case 11:
	case 42://呼叫等待关闭
		CallCancel();
		break;

	case 12:
	case 43://呼叫等待查询
		CheckStatus();
		break;

	case 21:
	case 22:
		break;

	case 31:
	case 32:
		break;

	case 1000:
		OnButtonFastDialsCancel();
		break;
	case 1001:
		OnButtonFastDialsOk();
		break;
	}
}

BOOL CCallSetDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	int xbegin = 190;
	int ybegin = 105;

	m_edtTransferNumber.Create(WS_CHILD|WS_VISIBLE, 
		CRect(xbegin+197, ybegin+99,xbegin+197+199, ybegin+99+34), this, IDC_EDIT_TRANSFERNUMBER);
	m_edtTransferNumber.SetLimitText(15);
	m_edtTransferNumber.SetLimitDiagital();
	
	m_cmbCallTransferTimes.CreateEx(WS_CHILD|WS_VISIBLE, 
		CRect(xbegin+318, ybegin+134, xbegin+318+112, ybegin+134+200),this, 0xFFFF, 22, 55, 26);
	m_cmbCallTransferTimes.AddString(L"5秒");
	m_cmbCallTransferTimes.AddString(L"10秒");
	m_cmbCallTransferTimes.AddString(L"15秒");
	m_cmbCallTransferTimes.AddString(L"20秒");
	m_cmbCallTransferTimes.AddString(L"25秒");
	m_cmbCallTransferTimes.AddString(L"30秒");
	m_cmbCallTransferTimes.SetCurSel(0);

// 	m_MJPGList.Create(L"", WS_VISIBLE|WS_CHILD, CRect(100, 137, 540, 407), this);
// 	m_MJPGList.SetMJPGRect(CRect(100, 137, 540, 407));
	
	//PLMN
	m_cmbPlmn.CreateEx(WS_CHILD,CRect(xbegin +40 , ybegin +145, xbegin+40+347, ybegin+145+ 300), this, IDC_COMBOBOX_PLMN, 22, 55, 26);

	m_MJPGList.Create(L"", WS_VISIBLE|WS_CHILD, CRect(xbegin, ybegin, xbegin + 440, ybegin+270), this);
	m_MJPGList.SetMJPGRect(CRect(xbegin, ybegin, xbegin + 440, ybegin+270));

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCallSetDlg::OnButtonFastDialsOk()
{	
	if ( 4 == m_uiType)//网络运营商
	{	
		CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
		CString s;
		m_cmbPlmn.GetWindowText(s);
		
		if(s.GetLength() > 0)
		{	
			oper op;
			ParseOperater(Util::StringOp::FromCString(s),op);
			if(op.sta != 3)
			{
			//	main->m_pATCommandWarp1->ClosePLMN();
				
				main->m_pATCommandWarp1->RegisterPLMN(op.op.c_str(),op.act);
				
				main->m_pWarningNoFlashDlg->SetTitle(L"正在注册网络...",120000);
				main->m_pWarningNoFlashDlg->SetNoClick(true);
				main->m_pWarningNoFlashDlg->ShowWindow_(SW_SHOW);
				return;
			}
			else 
			{
				main->m_pWarningNoFlashDlg->SetTitle(L"当前网络运营商禁止注册");
				main->m_pWarningNoFlashDlg->ShowWindow_(SW_SHOW);
				return;
			}

		}
	}
	SipShowIM(SIPF_OFF);
	ShowWindow_(FALSE);
	SipShowIM(SIPF_OFF);

}

void CCallSetDlg::OnButtonFastDialsCancel()
{
	m_edtTransferNumber.SetWindowText(L"");
	SipShowIM(SIPF_OFF);
	ShowWindow_(FALSE);
	SipShowIM(SIPF_OFF);
}

void CCallSetDlg::SetCallSetParam()
{
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	
	if ( 1 == m_uiType)//呼叫转移
	{	
		m_cmbCallTransferTimes.ShowWindow(SW_SHOW);
		m_edtTransferNumber.ShowWindow(SW_SHOW);
		m_cmbPlmn.ShowWindow(SW_HIDE);

		CString type;
		CString typeicon[4] ={
							  L".\\adv\\mjpg\\k5\\common\\设置\\所有来电.bmp",
							  L".\\adv\\mjpg\\k5\\common\\设置\\正在通话中.bmp",
							  L".\\adv\\mjpg\\k5\\common\\设置\\无人接听时.bmp",
							  L".\\adv\\mjpg\\k5\\common\\设置\\无人接通时.bmp",
							};

		main->m_pSettingDlg->m_cmbCallTransfer.GetWindowText(type);
		
		m_cmbCallTransferTimes.ShowWindow(false);
		if ( 0 == type.Compare(L"所有来电"))//贴图所有来电
		{
			m_nCallType = 0;
		}
		else if ( 0 == type.Compare(L"正在通话中"))
		{
			m_nCallType = 1;
		}
		else if ( 0 == type.Compare(L"无人接听时"))
		{
			m_cmbCallTransferTimes.ShowWindow(true);
			m_cmbCallTransferTimes.SetCurSel(0);
			m_nCallType = 2;		
		}	
		else if ( 0 == type.Compare(L"无法接通时"))
		{
			m_nCallType = 3;		
		}
		m_MJPGList.SetUnitBitmap(14,typeicon[m_nCallType],L"",true);
	}
	else if (2 == m_uiType)//呼叫限制
	{
		m_cmbCallTransferTimes.ShowWindow(SW_HIDE);
		m_edtTransferNumber.ShowWindow(SW_HIDE);
		m_cmbPlmn.ShowWindow(SW_HIDE);
	}
	else if (3 == m_uiType)//呼叫等待
	{
		m_edtTransferNumber.ShowWindow(false);
		m_cmbCallTransferTimes.ShowWindow(false);
		m_cmbPlmn.ShowWindow(SW_HIDE);
	}
	else if ( 4 == m_uiType)//网络运营商选择
	{
		m_edtTransferNumber.ShowWindow(false);
		m_cmbCallTransferTimes.ShowWindow(false);
		m_cmbPlmn.ShowWindow(SW_SHOW);
		m_cmbPlmn.SetCurSel(0);

	}

}

void CCallSetDlg::ShowItemsControl()
{

// 	m_edtTransferNumber.ShowWindow(FALSE);
// 
// 	switch(m_uiType)
// 	{
// 	case 1:
// 		m_edtTransferNumber.ShowWindow(TRUE);
// 		break;
// 	case 4:
// 		break;
// 	}

}

LRESULT CCallSetDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{	
	if ( WM_CALLTRANSFER == message)
	{	
		CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
		CString number ;
		m_edtTransferNumber.GetWindowText(number);

		if ( 0 == wParam)//not active
		{			
			CString tip ;
			if (cancel == m_nAction)
			{
				tip = L"呼叫设置取消成功";
			}
			else if ( check == m_nAction)
			{
				tip = L"此呼叫业务没有设置";
			}
			main->m_pDeleteTipDlg->SetTitle(tip,3000);
			main->m_pDeleteTipDlg->ShowWindow_(true);
			
		}
		else if(1 == wParam)//active
		{			
			CString tip ;
			if (start == m_nAction)
			{
				tip = L"呼叫业务设置成功";
			}
			else if (check == m_nAction)
			{
				tip = L"此呼叫业务已经设置";
				if (m_uiType == 1)
				{
					std::string *s = (std::string *)lParam ; 
					m_edtTransferNumber.SetWindowText(Util::StringOp::ToCString(*s));
				}

			}
			main->m_pDeleteTipDlg->SetTitle(tip,3000);
			main->m_pDeleteTipDlg->ShowWindow(true);
			
		}
		else if(2 == wParam)//操作不允许
		{	
			main->m_pDeleteTipDlg->SetTitle(L"操作不允许",3000);
			main->m_pDeleteTipDlg->ShowWindow(true);
			m_nSetResult = 1;
		}

	}
	return CDialog::WindowProc(message, wParam, lParam);

}

void CCallSetDlg::Start()
{
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	m_nAction = start ;	
	m_nSetResult = 1;
	
	if (1 == m_uiType)//呼叫转移
	{
		
		CString number ;
		m_edtTransferNumber.GetWindowText(number);
		
		if (number.IsEmpty())
		{
			main->m_pDeleteTipDlg->SetTitle(L"请先输入号码");
			main->m_pDeleteTipDlg->ShowWindow_(true);
			return ;
		}
		
		main->m_pDeleteTipDlg->SetTitle(L"正在执行呼叫请求...",300000);
		main->m_pDeleteTipDlg->ShowWindow_(true);

		if (2 == m_nCallType)//无人接听时
		{	
			CString times ;
			m_cmbCallTransferTimes.GetWindowText(times);
			int t = atoi(times);
			main->m_pATCommandWarp1->CallTransferStart(m_nCallType,Util::StringOp::FromCString(number),t);
		}
		else
		{	
			main->m_pATCommandWarp1->CallTransferStart(m_nCallType,Util::StringOp::FromCString(number));
		}
		
		OnButtonFastDialsCancel();

	}
	else if (3 == m_uiType)//呼叫等待
	{
		main->m_pDeleteTipDlg->SetTitle(L"正在执行呼叫请求...",300000);
		main->m_pDeleteTipDlg->ShowWindow_(true);
		main->m_pATCommandWarp1->PhoneSetCallWaiting(true);
	}

}

void CCallSetDlg::CallCancel()
{
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	m_nAction = cancel ;
	m_nSetResult = 1;

	if (1 == m_uiType)
	{			
		main->m_pDeleteTipDlg->SetTitle(L"正在执行呼叫请求...",300000);
		main->m_pDeleteTipDlg->ShowWindow_(true);
		main->m_pATCommandWarp1->CallTransferCancel(m_nCallType);

		OnButtonFastDialsCancel();
	}
	else if (3 == m_uiType)
	{
		main->m_pDeleteTipDlg->SetTitle(L"正在执行呼叫请求...",300000);
		main->m_pDeleteTipDlg->ShowWindow_(true);

		main->m_pATCommandWarp1->PhoneSetCallWaiting(false);

	}

	
}

void CCallSetDlg::CheckStatus()
{
	m_nAction = check ;
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	
	if (1 == m_uiType)
	{
		main->m_pDeleteTipDlg->SetTitle(L"正在检查设置状态...",300000);
		main->m_pDeleteTipDlg->ShowWindow_(true);

		main->m_pATCommandWarp1->CallTransferStatus(m_nCallType);
	}
	else if (3 == m_uiType)
	{
		main->m_pDeleteTipDlg->SetTitle(L"正在检查设置状态...",300000);
		main->m_pDeleteTipDlg->ShowWindow_(true);

		main->m_pATCommandWarp1->CHLDStatus();
	}	
		
}

void CCallSetDlg::ParseOperater(std::string d,oper &op)
{	
	size_t t ;
	std::string s = d;
	t = s.find("(");
	if ( t != std::string::npos)
	{
		s = s.substr(t+1);
		CString c ;
		c = Util::StringOp::ToCString(s);
		op.sta = atoi(c);

		t = s.find("\",\"");
		if ( t != std::string::npos)
		{
			s = s.substr(t+1);
			t = s.find("\",\"");
			if ( t != std::string::npos)
			{
				s	= s.substr(t+3);
				t	= s.find("\"");
				if ( t != std::string::npos)
				{
					op.op = s.substr(0,t);//oper	
				}
				
				t = s.find(",");
				if ( t != std::string::npos)
				{	
					s = s.substr(t+1);
					c = Util::StringOp::ToCString(s);
					op.act = atoi(c);//act	
				}
			}
		}
	}
}
