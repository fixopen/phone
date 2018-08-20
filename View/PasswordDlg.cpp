// passworddlg.cpp : implementation file
//

#include "stdafx.h"
#include "..\multimediaphone.h"
#include "..\MultimediaPhoneDlg.h"
#include "passworddlg.h"
#include "../Data/LanguageResource.h"
#include "../Data/SkinStyle.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPasswordDlg dialog


CPasswordDlg::CPasswordDlg(CWnd* pParent /*=NULL*/)
	: CCEDialog(CPasswordDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPasswordDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_passwordType = SETTING_PASSWORD;
	m_password = "";
	m_nStep = 0;
	m_Owner = NULL;
}

void CPasswordDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPasswordDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPasswordDlg, CCEDialog)
	//{{AFX_MSG_MAP(CPasswordDlg)
	ON_MESSAGE(WM_CHANGELANGUAGE,OnChangeLanguage)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_PASSWORD_OK, OnButtonPasswordOk)
	ON_BN_CLICKED(IDC_BUTTON_PASSWORD_CANCEL, OnButtonPasswordCancel)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPasswordDlg message handlers

BOOL CPasswordDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	// TODO: Add extra initialization here
	m_bIsSetPassword=FALSE;
	m_bIsSetPasswordError=FALSE;
	m_bIsFirstSetSPassword=FALSE;
	m_bIsFirstSetPlPassword=FALSE;
	m_bIsFirstSetApassword=FALSE;
	std::string strTemp;
	CString str;

	strTemp = Data::LanguageResource::Get(Data::RI_PASSWORD_TIP);
	str = strTemp.c_str();
	m_sticTip.Create(str, WS_CHILD|WS_VISIBLE, CRect(62, 76*VHEIGHT, 340, 94*VHEIGHT), this);
	m_sticTip.SetColor(RGB(0, 0, 0), Data::g_partFrameInRectBackRGB[Data::g_skinstyle]);

	strTemp = Data::LanguageResource::Get(Data::RI_PASSWORD_OLDPASSWORD);
	str = strTemp.c_str();
	m_sticOldPassword.Create(str, WS_CHILD|WS_VISIBLE, CRect(50, 98*VHEIGHT, 144, 116*VHEIGHT), this);
	m_sticOldPassword.SetColor(RGB(0, 0, 0), Data::g_partFrameInRectBackRGB[Data::g_skinstyle]);
	m_sticOldPassword.SetAlign(SS_RIGHT);	

	m_edtOldPassword.Create(WS_CHILD|WS_VISIBLE|ES_PASSWORD, CRect(148, 98*VHEIGHT, 338, 116*VHEIGHT), this, IDC_SETTING_PASSWORD1);
	m_edtOldPassword.SetLimitText(6);

	strTemp = Data::LanguageResource::Get(Data::RI_PASSWORD_NEWPASSWORD1);
	str = strTemp.c_str();
	m_sticNewPassword1.Create(str, WS_CHILD|WS_VISIBLE, CRect(50, 123*VHEIGHT, 144, 141*VHEIGHT), this);
	m_sticNewPassword1.SetColor(RGB(0, 0, 0), Data::g_partFrameInRectBackRGB[Data::g_skinstyle]);
	m_sticNewPassword1.SetAlign(SS_RIGHT);	

	m_edtNewPassword1.Create(WS_CHILD|WS_VISIBLE|ES_PASSWORD, CRect(148, 123*VHEIGHT, 338, 141*VHEIGHT), this, IDC_SETTING_PASSWORD2);
	m_edtNewPassword1.SetLimitText(6);

	strTemp = Data::LanguageResource::Get(Data::RI_PASSWORD_NEWPASSWORD2);
	str = strTemp.c_str();
	m_sticNewPassword2.Create(str, WS_CHILD|WS_VISIBLE, CRect(50, 148*VHEIGHT, 144, 166*VHEIGHT), this);
	m_sticNewPassword2.SetColor(RGB(0, 0, 0), Data::g_partFrameInRectBackRGB[Data::g_skinstyle]);
	m_sticNewPassword2.SetAlign(SS_RIGHT);	

	m_edtNewPassword2.Create(WS_CHILD|WS_VISIBLE|ES_PASSWORD, CRect(148, 148*VHEIGHT, 338, 166*VHEIGHT), this, IDC_SETTING_PASSWORD3);
	m_edtNewPassword2.SetLimitText(6);
		
	strTemp = Data::LanguageResource::Get(Data::RI_COMN_OKBTN);
	str = strTemp.c_str();
	m_btnOk.Create(str, Data::g_buttonArcBMPPARTDILOAGID[0][Data::g_skinstyle], Data::g_buttonArcBMPPARTDILOAGID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(378, 78*VHEIGHT, 434, 98*VHEIGHT), this, IDC_BUTTON_PASSWORD_OK);
	m_btnOk.SetBackRGB(Data::g_partFrameMainBackRGB[Data::g_skinstyle]);
	
	strTemp = Data::LanguageResource::Get(Data::RI_COMN_CANCELBTN);
	str = strTemp.c_str();
	m_btnCancel.Create(str, Data::g_buttonArcBMPPARTDILOAGID[0][Data::g_skinstyle], Data::g_buttonArcBMPPARTDILOAGID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(378, 103*VHEIGHT, 434, 123*VHEIGHT), this, IDC_BUTTON_PASSWORD_CANCEL);
	m_btnCancel.SetBackRGB(Data::g_partFrameMainBackRGB[Data::g_skinstyle]);
	
	m_sticBackground.Create(CRect(34, 40*VHEIGHT, 448, 184*VHEIGHT), this, 1);
	
	TextStruct ts[1];
	memset(ts, 0, sizeof(TextStruct) * 1);
	
	ts[0].txtRect = CRect(8, 0, 100, 20*VHEIGHT);
	ts[0].txtFontSize = 16;
	ts[0].sAlign = DT_LEFT | DT_BOTTOM;
	memcpy(ts[0].sTxt, Data::LanguageResource::Get(Data::RI_PASSWORD_TITLE).c_str(), Data::LanguageResource::Get(Data::RI_PASSWORD_TITLE).length());
	
	m_sticBackground.SetTextStruct(ts, 1);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPasswordDlg::SetStaticDefaultColor(CCEStatic* cestatic)
{
	// 	cestatic->SetColor(RGB(0, 0, 0), RGB(194,235,149));
	cestatic->SetColor(RGB(0, 0, 0), RGB(255,255,255));
}
//defect 36
char gPassWord[1024] ={0};
//====================

void CPasswordDlg::OnButtonPasswordOk()
{
	KeyBoardOff();
	
	//在录音播放的时候，作为只播放一次录音的开关
	((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSoundDlg->m_bPopPasswordDlg=false;
	CString s;
	BOOL isError = FALSE;
	if(m_passwordType == SETTING_PASSWORD)
	{
		CString s1, s2, s3;
		GetDlgItemText(IDC_SETTING_PASSWORD1, s1);
		GetDlgItemText(IDC_SETTING_PASSWORD2, s2);
		GetDlgItemText(IDC_SETTING_PASSWORD3, s3);
        if (m_bIsSetPassword)
        {
			s1="";
			m_password="";
			m_bIsSetPassword=FALSE;
        }
		if(m_password != s1)
		{
			std::string strTemp;

			strTemp = Data::LanguageResource::Get(Data::RI_PASSWORD_TIP1);

			s = strTemp.c_str();

			SetDlgItemText(IDC_SETTING_PASSWORD1, L"");

			isError = TRUE;
		}
		else if(s2 != s3)
		{
			std::string strTemp;

			strTemp = Data::LanguageResource::Get(Data::RI_PASSWORD_TIP2);

			s = strTemp.c_str();

			SetDlgItemText(IDC_SETTING_PASSWORD2, L"");

			SetDlgItemText(IDC_SETTING_PASSWORD3, L"");

			isError = TRUE;

		}else{

			KeyBoardOff();
			//给主窗口发送新的密码
			ShowWindow_(FALSE);
			
			if(m_Owner)
			{
				std::string strtemp = Util::StringOp::FromCString(s2);
				
				const char *temp = strtemp.c_str();

				//defect 36
				memset(gPassWord,0,strlen(gPassWord));

				memcpy(gPassWord,temp,1023);

				::PostMessage(m_Owner, WM_SETTINGPASSWORD, (WPARAM)gPassWord, strlen(gPassWord));
				//==========================
				std::string strTemp = Data::LanguageResource::Get(Data::RI_PASSWORD_TIP);

				CString str = strTemp.c_str();

				m_sticTip.SetWindowText(str);

				return;
			}
		}
	}
	else 
	{
		CString s1;

		GetDlgItemText(IDC_SETTING_PASSWORD1, s1);
	
		if(m_password != s1)
		{
			std::string strTemp;

			strTemp = Data::LanguageResource::Get(Data::RI_PASSWORD_TIP3);

			s = strTemp.c_str();

			SetDlgItemText(IDC_SETTING_PASSWORD1, L"");

			isError = TRUE;

		}else
		{
			KeyBoardOff();
			ShowWindow_(FALSE);

			if(m_Owner)
			{
				::PostMessage(m_Owner, WM_CHECKPASSWORD, 0, 0);

				std::string strTemp = Data::LanguageResource::Get(Data::RI_PASSWORD_TIP);

				CString str = strTemp.c_str();

				m_sticTip.SetWindowText(str);

				return;
			}
		}
	}
	if(isError)
	{   
		if (m_bIsSetPasswordError)
		{
			m_bIsSetPassword=FALSE;

		}else
		{
	        m_bIsSetPassword=TRUE;
		}
		m_sticTip.SetWindowText(s);
	}
 	
}

void CPasswordDlg::OnButtonPasswordCancel()
{
	KeyBoardOff();
	
	ShowWindow_(FALSE); 
	
	std::string strTemp = Data::LanguageResource::Get(Data::RI_PASSWORD_TIP);
	
	CString str = strTemp.c_str();
	
	m_sticTip.SetWindowText(str);

	((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSoundDlg->m_bPopPasswordDlg=true;
}

void CPasswordDlg::SetOldPassWord(char *pPassWord)
{
	m_password = pPassWord;
	//if(strlen(pPassWord) > 0)
	//	change by qi 20111102
	//	m_edtOldPassword.EnableWindow(TRUE);
}
//////zzw20100727///////////////////////////////////
void CPasswordDlg::SetType(PASSWORD_TYPE nType,int isFirstSet)
{
	std::string strTemp = Data::LanguageResource::Get(Data::RI_PASSWORD_NEWPASSWORD1);//?
	CString str = strTemp.c_str();
	m_sticNewPassword1.SetWindowText(str);
	strTemp=Data::LanguageResource::Get(Data::RI_PASSWORD_OLDPASSWORD);
	str=strTemp.c_str();
	m_sticOldPassword.SetWindowText(str);
	m_edtNewPassword1.SetWindowText(L"");
	m_edtNewPassword2.SetWindowText(L"");
	m_edtOldPassword.SetWindowText(L"");
	
	m_passwordType = nType;
	TextStruct ts[1];
	memset(ts, 0, sizeof(TextStruct) * 1);
	
	ts[0].txtRect = CRect(8, 0, 100, 20);
	ts[0].txtFontSize = 16;
	ts[0].sAlign = DT_LEFT | DT_BOTTOM;
	if(m_passwordType == SETTING_PASSWORD)
	{    
		switch(isFirstSet)
		{
		case 1:
			if (m_bIsFirstSetApassword)
			{  
				m_edtOldPassword.SetWindowText(L"");
		//		change by qi 20111102
		//		m_edtOldPassword.EnableWindow(TRUE);
				m_bIsSetPasswordError=TRUE;
				m_bIsSetPassword=FALSE;

			}else{

				m_edtOldPassword.SetWindowText((LPCTSTR)CString("******"));
				m_bIsSetPassword=TRUE;
				m_bIsSetPasswordError=FALSE;
//				m_edtOldPassword.EnableWindow(FALSE);
			}
			break;
		case 2:
			if (m_bIsFirstSetPlPassword)
			{  
				m_edtOldPassword.SetWindowText(L"");
//				change by qi 20111102
//				m_edtOldPassword.EnableWindow(TRUE); 
				m_bIsSetPasswordError=TRUE;
				m_bIsSetPassword=FALSE;

			}else{
				m_edtOldPassword.SetWindowText((LPCTSTR)CString("******"));
				m_bIsSetPassword=TRUE;
				m_bIsSetPasswordError=FALSE;

//				change by qi 20111102
//				m_edtOldPassword.EnableWindow(FALSE);
		   }
			break;
       case 3:
		   if (m_bIsFirstSetSPassword)//&&m_bisUseScreenSaverPassword)
		   {  
	    	 m_edtOldPassword.SetWindowText(L"");
			 m_bIsSetPasswordError=TRUE;
//			 change by qi 20111102
//		     m_edtOldPassword.EnableWindow(TRUE);
			 m_bIsSetPassword=FALSE;

		   }else
		   {
			   m_edtOldPassword.SetWindowText((LPCTSTR)CString("******"));
			   m_bIsSetPassword=TRUE;
			   m_bIsSetPasswordError=FALSE;
//				change by qi 20111102
//			    m_edtOldPassword.EnableWindow(FALSE);

		   }
         break;
		}
		memcpy(ts[0].sTxt, Data::LanguageResource::Get(Data::RI_PASSWORD_TITLE).c_str(), Data::LanguageResource::Get(Data::RI_PASSWORD_TITLE).length());
		m_sticNewPassword1.ShowWindow(SW_SHOW);
		m_sticNewPassword2.ShowWindow(SW_SHOW);
		m_edtNewPassword1.ShowWindow(SW_SHOW);
		m_edtNewPassword2.ShowWindow(SW_SHOW);
		m_sticOldPassword.ShowWindow(SW_SHOW);
	}
	else
	{   
		//	change by qi 20111102
		/*if (m_password=="")
		{
	    	m_edtOldPassword.EnableWindow(FALSE);

		}else{

             m_edtOldPassword.EnableWindow(TRUE);
		}*/

		m_sticNewPassword1.ShowWindow(SW_HIDE);
		m_sticNewPassword2.ShowWindow(SW_HIDE);
		m_edtNewPassword1.ShowWindow(SW_HIDE);
		m_edtNewPassword2.ShowWindow(SW_HIDE);
		m_sticOldPassword.ShowWindow(SW_HIDE);
		memcpy(ts[0].sTxt, Data::LanguageResource::Get(Data::RI_PASSWORD_TITLE1).c_str(), Data::LanguageResource::Get(Data::RI_PASSWORD_TITLE).length());
	}
	m_sticBackground.SetTextStruct(ts, 1);
}

void CPasswordDlg::SetFisrtAPassword(BOOL Flag)
{
	m_bIsFirstSetApassword=Flag;
}

void CPasswordDlg::SetFisrtPPassword(BOOL Flag)
{
	m_bIsFirstSetPlPassword=Flag;
}

void CPasswordDlg::SetFisrtSPassword(BOOL Flag)
{
	m_bIsFirstSetSPassword=Flag;
}

void CPasswordDlg::SetType(PASSWORD_TYPE nType)
{
	std::string strTemp = Data::LanguageResource::Get(Data::RI_PASSWORD_NEWPASSWORD1);//?
	CString str = strTemp.c_str();
	m_sticNewPassword1.SetWindowText(str);
	m_edtNewPassword1.SetWindowText(L"");
	m_edtNewPassword2.SetWindowText(L"");
	m_edtOldPassword.SetWindowText(L"");

	m_passwordType = nType;
	TextStruct ts[1];
	memset(ts, 0, sizeof(TextStruct) * 1);
	
	ts[0].txtRect = CRect(8, 0, 100, 20);
	ts[0].txtFontSize = 16;
	ts[0].sAlign = DT_LEFT | DT_BOTTOM;

	if(m_passwordType == SETTING_PASSWORD)
	{   
		if (((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pSetting->isAdmin()||
			((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pSetting->isPlayProtect()||
			((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pSetting->isUseScreenSaverPassword())
		{ 
		   m_edtOldPassword.SetWindowText(L"");
//			 change by qi 20111102
//		   m_edtOldPassword.EnableWindow(TRUE);
		}
		else
		{
		   m_edtOldPassword.SetWindowText((LPCTSTR)CString("******"));
		   m_bIsSetPassword=TRUE;
//			 change by qi 20111102
//           m_edtOldPassword.EnableWindow(FALSE);
		}
		memcpy(ts[0].sTxt, Data::LanguageResource::Get(Data::RI_PASSWORD_TITLE).c_str(), Data::LanguageResource::Get(Data::RI_PASSWORD_TITLE).length());
		m_sticNewPassword1.ShowWindow(SW_SHOW);
		m_sticNewPassword2.ShowWindow(SW_SHOW);
		m_edtNewPassword1.ShowWindow(SW_SHOW);
		m_edtNewPassword2.ShowWindow(SW_SHOW);
		m_sticOldPassword.ShowWindow(SW_SHOW);

	} else if (m_passwordType == CHECK_CLEARPASSWORD)
	{   
		std::string strTemp;
		CString str;
		strTemp = Data::LanguageResource::Get(Data::RI_PASSWORD_SUPERPASSWORD);
	    str = strTemp.c_str();
		m_sticOldPassword.SetWindowText(str);
		m_edtOldPassword.SetLimitText(8);
//			 change by qi 20111102
//		m_edtOldPassword.EnableWindow(TRUE);
		m_sticNewPassword1.ShowWindow(FALSE);
		m_edtNewPassword1.ShowWindow(FALSE);
		m_sticNewPassword2.ShowWindow(FALSE);
		m_edtNewPassword2.ShowWindow(FALSE);

	}else
	{   
		//			 change by qi 20111102
		/*if (m_password=="")
		{
			m_edtOldPassword.EnableWindow(FALSE);			
		}
		else
		{			
			m_edtOldPassword.EnableWindow(TRUE);
		}*/

		m_sticNewPassword1.ShowWindow(SW_HIDE);
		m_sticNewPassword2.ShowWindow(SW_HIDE);
		m_edtNewPassword1.ShowWindow(SW_HIDE);
		m_edtNewPassword2.ShowWindow(SW_HIDE);
		m_sticOldPassword.ShowWindow(SW_HIDE);
		memcpy(ts[0].sTxt, Data::LanguageResource::Get(Data::RI_PASSWORD_TITLE1).c_str(), Data::LanguageResource::Get(Data::RI_PASSWORD_TITLE).length());
	}
	m_sticBackground.SetTextStruct(ts, 1);
}
LRESULT CPasswordDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
	    case WM_OK:	    	
		OnButtonPasswordOk();
		    break;
		case WM_CANCEL:
		OnButtonPasswordCancel();
		break;
	}
	return CDialog::WindowProc(message,wParam,lParam);
}

LRESULT CPasswordDlg::OnChangeLanguage(WPARAM wParam,LPARAM lParam)
{
ChangeLanguage();
return 0;
}
void CPasswordDlg::ChangeLanguage()
{
    std::string strTemp;
	CString str;
	strTemp = Data::LanguageResource::Get(Data::RI_PASSWORD_TIP);
	str = strTemp.c_str();
	m_sticTip.SetWindowText(str);

    strTemp = Data::LanguageResource::Get(Data::RI_PASSWORD_OLDPASSWORD);
	str = strTemp.c_str();
    m_sticOldPassword.SetWindowText(str);

   	strTemp = Data::LanguageResource::Get(Data::RI_PASSWORD_NEWPASSWORD1);
	str = strTemp.c_str();
	m_sticNewPassword1.SetWindowText(str);

	strTemp = Data::LanguageResource::Get(Data::RI_PASSWORD_NEWPASSWORD2);
	str = strTemp.c_str();
	m_sticNewPassword2.SetWindowText(str);

	strTemp = Data::LanguageResource::Get(Data::RI_COMN_OKBTN);
	str = strTemp.c_str();
	m_btnOk.SetWindowText(str);

	strTemp = Data::LanguageResource::Get(Data::RI_COMN_CANCELBTN);
	str = strTemp.c_str();
	m_btnCancel.SetWindowText(str);

	TextStruct ts[1];
	memset(ts, 0, sizeof(TextStruct) * 1);
	
	ts[0].txtRect = CRect(8, 0, 100, 20*VHEIGHT);
	ts[0].txtFontSize = 16;
	ts[0].sAlign = DT_LEFT | DT_BOTTOM;
	memcpy(ts[0].sTxt, Data::LanguageResource::Get(Data::RI_PASSWORD_TITLE).c_str(), Data::LanguageResource::Get(Data::RI_PASSWORD_TITLE).length());
	m_sticBackground.SetTextStruct(ts, 1);

}