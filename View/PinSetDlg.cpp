// fastdialsdlg.cpp : implementation file
//

#include "stdafx.h"
#include "..\multimediaphone.h"
#include "..\MultimediaPhoneDlg.h"
#include "PinSetDlg.h"
#include "../Data/LanguageResource.h"
#include "../Data/SkinStyle.h"
#include "../Util/ATCommandWarp.h"
#include "sip.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPinSetDlg dialog


CPinSetDlg::CPinSetDlg(CWnd* pParent /*=NULL*/)
	: CCEDialog(CPinSetDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPinSetDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CPinSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPinSetDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPinSetDlg, CCEDialog)

ON_MESSAGE(WM_CLICKMJPG_TOAPP, OnClickMJPG)
ON_MESSAGE(WM_MJPGTOGGLE, OnClickMJPG)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPinSetDlg message handlers
void CPinSetDlg::OnClickMJPG(WPARAM w, LPARAM l)
{	
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	switch(w)
	{
	case 41://打开PIN
		if (m_MJPGList.GetUnitIsDownStatus(42))
		{
			main->m_pATCommandWarp1->CheckPuk();

			m_MJPGList.SetUnitIsDownStatus(42,false);
			m_MJPGList.SetUnitIsShow(42,true,true);		
			m_MJPGList.SetUnitIsDownStatus(41,true);

		}
		break;

	case 42://关闭PIN
		if (m_MJPGList.GetUnitIsDownStatus(41))
		{
			main->m_pATCommandWarp1->CheckPuk();

			m_MJPGList.SetUnitIsDownStatus(41,false);
			m_MJPGList.SetUnitIsShow(41,true,true);		
			m_MJPGList.SetUnitIsDownStatus(42,true);

		}
		break;

	case 43://修改PIN码
		if(m_bPin)
		{	
			extern BOOL g_IsFDNFindDrap;
			g_IsFDNFindDrap = FALSE;
			main->m_pATCommandWarp1->Drap(Util::ATCommandWarp::DrapChangePinPassword);   //lxz 20100907
		}
		else
		{
			main->m_pDeleteTipDlg->SetTitle(L"请先设置PIN码为开");
			main->m_pDeleteTipDlg->ShowWindow_(SW_SHOW);
		}
		break;

	case 44://修改PIN2码
		{
			extern BOOL g_IsFDNFindDrap;
			g_IsFDNFindDrap = FALSE;
			main->m_pATCommandWarp1->Drap(Util::ATCommandWarp::DrapChangePin2Password);   //lxz 20100907

		}
		break;

	case 1001:
		ShowWindow_(SW_HIDE);
		break;

	case 1000:
		ShowWindow_(SW_HIDE);
		break;
	}
}

BOOL CPinSetDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	 
	int xtop = 190 ;
	int ytop = 105 ;

	m_MJPGList.Create(L"", WS_VISIBLE|WS_CHILD, CRect(xtop, ytop, xtop+440, ytop+270), this);
	m_MJPGList.SetCurrentLinkFile(".\\adv\\mjpg\\k5\\中文\\设置PIN码.xml");
	m_MJPGList.SetMJPGRect(CRect(xtop, ytop, xtop+440, ytop+270));

	m_pPinInputDlg = new CPinInputDlg(this);
	m_pPinInputDlg->Create(CPinInputDlg::IDD);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPinSetDlg::SetCallSetParam()
{	
	extern Util::ATCommandWarp* GetATCommandWarp();
	Util::ATCommandWarp *pATCommanWarp = GetATCommandWarp();
	pATCommanWarp->PinStatus();
}

void CPinSetDlg::SetPinStatus()
{
	 if (m_bPin)
	 {
		 m_MJPGList.SetUnitIsDownStatus(42,false);
		 m_MJPGList.SetUnitIsShow(42,true,true);
		 
		 m_MJPGList.SetUnitIsDownStatus(41,true);
		 m_MJPGList.SetUnitIsShow(41,true,true);
	 }
	 else
	 {
		 m_MJPGList.SetUnitIsDownStatus(42,true);
		 m_MJPGList.SetUnitIsShow(42,true,true);
		 
		 m_MJPGList.SetUnitIsDownStatus(41,false);
		 m_MJPGList.SetUnitIsShow(41,true,true);
	 }
}

LRESULT CPinSetDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	CMultimediaPhoneDlg *main = ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd));
	switch (message)
	{
	case WM_BLOCK_OK:
		if (1 == lParam)//pin码输入成功
		{
			bool b1 = m_MJPGList.GetUnitIsDownStatus(41);
			bool b2 = m_MJPGList.GetUnitIsDownStatus(42);
			if (b2)
			{
				m_bPin = false;
				m_MJPGList.SetUnitIsDisable(41,false);				
				m_MJPGList.SetUnitIsDisable(42,true);
			}
			else if (b1)
			{	
				m_bPin = true;
				m_MJPGList.SetUnitIsDisable(41,true);
				m_MJPGList.SetUnitIsDisable(42,false);
			}
			
		}
		else if( 0 == lParam)
		{	
			main->m_pDeleteTipDlg->SetTitle("PIN码输入错误");
			main->m_pDeleteTipDlg->ShowWindow_(SW_SHOW);

			bool b1 = m_MJPGList.GetUnitIsDownStatus(41);
			bool b2 = m_MJPGList.GetUnitIsDownStatus(42);
			if (b2)
			{
				m_MJPGList.SetUnitIsDownStatus(42,false);
				m_MJPGList.SetUnitIsDownStatus(41,true);					
			}
			else if (b1)
			{	
				m_MJPGList.SetUnitIsDownStatus(42,true);
				m_MJPGList.SetUnitIsDownStatus(41,false);
			}
			m_MJPGList.Invalidate();
		}
		else if (2 == lParam)//设置pin码状态
		{
			int status = wParam;
			if ( 1 == status)
			{	
				m_bPin = TRUE;
				m_MJPGList.SetUnitIsDownStatus(41,true);
				m_MJPGList.SetUnitIsDisable(41,true);

				m_MJPGList.SetUnitIsDownStatus(42,false);
				m_MJPGList.SetUnitIsDisable(42,false);
			}
			else
			{	
				m_bPin = FALSE;
				m_MJPGList.SetUnitIsDownStatus(41,false);
				m_MJPGList.SetUnitIsDisable(41,false);

				m_MJPGList.SetUnitIsDownStatus(42,true);
				m_MJPGList.SetUnitIsDisable(42,true);

			}
			m_MJPGList.Invalidate();
		}
		break;

	case WM_DRAP_STATUS :
		{	
			extern int g_p1;
			extern int g_p2;
			extern int g_pu1;
			extern int g_pu2;
			if ( 1== lParam)//修改PIN码
			{
				if(g_p1 == -1)
				{
					main->m_pDeleteTipDlg->SetTitle(L"操作未成功!");
					main->m_pDeleteTipDlg->ShowWindow_(SW_SHOW);
				}
				else if(g_p1 > 0)
				{
					m_pPinInputDlg->ChangePinView();
					m_pPinInputDlg->SetErrorCount(g_p1);
					m_pPinInputDlg->ShowWindow_(SW_SHOW);	
				}
				else if(g_pu1 > 0)
				{
					m_pPinInputDlg->ShowPukInputView();
					m_pPinInputDlg->SetErrorCount(g_pu1);
					m_pPinInputDlg->ShowWindow_(SW_SHOW);
				}
				else if(g_pu1 == 0)
				{
					main->m_pDeleteTipDlg->SetTitle(L"PUK1码被锁住!");
					main->m_pDeleteTipDlg->ShowWindow_(SW_SHOW);
				}
			}
			else if (2 == lParam)
			{
				if(g_p2 == -1)
				{
					main->m_pDeleteTipDlg->SetTitle(L"操作未成功!");
					main->m_pDeleteTipDlg->ShowWindow_(SW_SHOW);
				}
				else if(g_p2 > 0)
				{
					m_pPinInputDlg->ChangePin2View();
					m_pPinInputDlg->SetErrorCount(g_p2);
					m_pPinInputDlg->ShowWindow_(SW_SHOW);	
				}
				else if(g_pu2 > 0)
				{
					m_pPinInputDlg->ShowPuk2InputView();
					m_pPinInputDlg->SetErrorCount(g_pu2);
					m_pPinInputDlg->ShowWindow_(SW_SHOW);
				}
				else if(g_pu2 == 0)
				{
					main->m_pDeleteTipDlg->SetTitle(L"PUK2码被锁住!");
					main->m_pDeleteTipDlg->ShowWindow_(SW_SHOW);
				}
			}

		}
		break;

	}
	return CCEDialog::WindowProc(message, wParam, lParam);
}