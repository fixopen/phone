// MultimediaPhone.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "MultimediaPhone.h"
#include "MultimediaPhoneDlg.h"
#include "sip.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CMultimediaPhoneApp

BEGIN_MESSAGE_MAP(CMultimediaPhoneApp, CWinApp)
	//{{AFX_MSG_MAP(CMultimediaPhoneApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMultimediaPhoneApp construction

CMultimediaPhoneApp::CMultimediaPhoneApp()
	: CWinApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CMultimediaPhoneApp object

CMultimediaPhoneApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CMultimediaPhoneApp initialization
void KeyBoardOff()
{
	if(theApp.inputDlg_->IsWindowVisible())
	{
		theApp.inputDlg_->hide();
	}
    
	if(theApp.handWriteDlg_->IsWindowVisible())
	{
		theApp.handWriteDlg_->hide();
	}
	SipShowIM(SIPF_OFF);
}

BOOL CMultimediaPhoneApp::InitInstance()
{
	extern void InitPinYin();
	InitPinYin();
	stream=_wfreopen(L"\\hive\\stdErr.out", L"w", stderr);
	//
	inputDlg_ = new CInputDlg;
	inputDlg_->Create(CInputDlg::IDD);
	
	//手写
	handWriteDlg_ = new CHandWriteDlg ;
	handWriteDlg_->Create(CHandWriteDlg::IDD);

	CMultimediaPhoneDlg dlg;
	m_pMainWnd = &dlg;
	IsHangON=false;
	IsCheckTakingState=false;
	m_bIsPlayLeaveWord=TRUE;
	m_bIsHungOnFireWall=FALSE;
	m_bIsCreateRecord=FALSE;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		
	}else if (nResponse == IDCANCEL)
	{
		
	}
	return FALSE;
}

BOOL CMultimediaPhoneApp::PreTranslateMessage(MSG* pMsg)
{
	static int oldx = 0;

	static int oldy = 0;
    
	if ((pMsg->message == WM_LBUTTONUP) || (pMsg->message == WM_LBUTTONDOWN))
	{
		if (!((CMultimediaPhoneDlg*)m_pMainWnd)->phone_->isBackLight_)
		{
			((CMultimediaPhoneDlg*)m_pMainWnd)->phone_->OpenBacklight();

			 return TRUE;
		}

		if(pMsg->message == WM_LBUTTONUP)
		{ 
			if(pMsg->pt.x != oldx && pMsg->pt.y != oldy)
			{
				::SendMessage(m_pMainWnd->m_hWnd, WM_GEN_EVENT, 1, 0);

			}

			oldx = pMsg->pt.x;

			oldy = pMsg->pt.y;
		}
	}
	else  if (WM_KEYDOWN == pMsg->message)
    {
        UINT nKey = static_cast<UINT>(pMsg->wParam);	
        if( VK_ESCAPE == nKey) 
        {
			SipShowIM(SIPF_OFF);
            return TRUE; // 如果是回车键或者ESC直接返回TRUE
        }
    }
	
	
	return CWinApp::PreTranslateMessage(pMsg);
}
int CMultimediaPhoneApp::ExitInstance()
{
    fclose(stream);

	return CWinApp::ExitInstance();
}