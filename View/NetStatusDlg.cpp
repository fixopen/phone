// AlarmShowDlg.cpp : implementation file
//

#include "stdafx.h"
#include "../MultimediaPhone.h"
#include "../MultimediaPhoneDlg.h"
#include "NetStatusDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNetStatusDlg dialog

#include <Ras.h>
#include <Wininet.h>
#include <Raserror.h>
UINT WM_RASEVENT; //= ::RegisterWindowMessageA(RASDIALEVENT);

CString const GetPPPoEIP()
{
	HKEY hKey = NULL;
	LONG hRes = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("Comm\\PPPoE Line 0\\Parms\\TcpIp"), 0, 0, &hKey);
	DWORD dwType = REG_MULTI_SZ;
	TCHAR ipAddr[20] = {0};
	DWORD dwSize = sizeof(ipAddr);
	hRes = ::RegQueryValueEx(hKey, _T("IpAddress"), 0, &dwType, (LPBYTE)ipAddr, &dwSize);
	RegCloseKey(hKey);
	return CString(ipAddr);
}

HRASCONN gHRasConn = 0;
bool CNetStatusDlg::ADSLInit()
{
//	gHRasConn = 0;

	//RASENTRY   RasEntry;

	RASDEVINFO   devinfo;   
	devinfo.dwSize   =   sizeof(devinfo);   
	//RasEnumDevices(&devinfo,&cb,&cDevices);   

	RASENTRY entry;
	
	memset(&entry, 0, sizeof(RASENTRY));
	entry.dwSize = sizeof(RASENTRY);
	DWORD rr = sizeof(RASENTRY);
	//DWORD ret1=  RasGetEntryProperties(0, L"D123", &entry, &rr, NULL, NULL);
	
	entry.dwSize = sizeof(RASENTRY);
	//entry.dwfOptions = RASEO_RemoteDefaultGateway | RASEO_ModemLights | RASEO_IpHeaderCompression | 
	//					RASEO_SwCompression | RASEO_NetworkLogon;
	//entry.dwfOptions = 0x38010210;
	entry.dwfOptions = 0x200;
	entry.dwfNetProtocols = RASNP_Ip /*| RASNP_NetBEUI | RASNP_Ipx*/;
	entry.dwFramingProtocol = RASFP_Ppp;

	wsprintf(entry.szDeviceType, L"PPPoE");
	wsprintf(entry.szDeviceName, L"PPPoE Line 0");

	wsprintf(entry.szLocalPhoneNumber, L"");

	DWORD ret = RasSetEntryProperties(NULL,			// pointer to full path and filename of phone-book file
									  L"890",	// pointer to an entry name
									  &entry,		// buffer that contains entry information
									  sizeof(RASENTRY),// size, in bytes, of the lpRasEntry buffer
									  NULL,			// buffer that contains device-specific 
									  0);		
	Dprintf("RasSetEntryProperties = %d\n", ret);
	
	return 1;
}

static BOOL gDialRas = FALSE;
bool CNetStatusDlg::ADSLDial(char *username, char *password, CWnd *pMsgWnd) 
{
	if(gDialRas)
		ADSLHungUp();

	gDialRas = TRUE;

	RASDIALPARAMS RasDialParams;
	memset(&RasDialParams, 0, sizeof(RasDialParams));
 
    // Configure the RASDIALPARAMS structure. 
    RasDialParams.dwSize = sizeof (RASDIALPARAMS);
    wsprintf(RasDialParams.szPhoneNumber, L"");
    wsprintf(RasDialParams.szCallbackNumber, L"");;
	
	wsprintf(RasDialParams.szEntryName, L"890");
	//wprintf(RasDialParams.szEntryName);

	TCHAR userName[24];
	memset(userName, 0, sizeof(userName));
	//@@maybe error
    mbstowcs(userName, username, strlen(username) + 1);
	//wsprintf(RasDialParams.szUserName, L"100001644588");
	wsprintf(RasDialParams.szUserName, userName);

	Dprintf(username);
    
	/*
    strcpy(RasDialParams.szUserName, "100001644588");
    strcpy(RasDialParams.szPassword, "m2y8g4k9");
	*/

	TCHAR passWord[24];
	memset(passWord, 0, sizeof(passWord));
	//@@maybe error
    mbstowcs(passWord, password, strlen(password)+1);
	//wsprintf(RasDialParams.szPassword, L"m2y8g4k9");
	wsprintf(RasDialParams.szPassword, passWord);
	Dprintf(password);

    wsprintf(RasDialParams.szDomain, L"");

	DWORD ret1 = RasDial(NULL,NULL,&RasDialParams, 0xFFFFFFFF, pMsgWnd->m_hWnd, &gHRasConn);
	
	char txt[64];
	sprintf(txt, "Ras Dial %d \r\n", ret1);
	//	Dprintf("Ras code %d %d %d\r\n", result, w, l);
	DWriteDat("", (UINT8 *)txt, strlen(txt));

	if (ret1 != 0)
	{
		Dprintf("Dial error %d \n", ret1);
		//AfxMessageBox (TEXT("Could not connect using RAS"),NULL, MB_OK);
        return 0 ;
	}

	return 1;
}

//ERROR_NOT_ENOUGH_MEMORY

bool CNetStatusDlg::ADSLHungUp()
{
	gDialRas = FALSE;
	if(RasHangUp(gHRasConn)== 0)
	{
		Sleep(2000);
		return 1;
	}
	Dprintf("ADSL Hung up error\n");
	Sleep(2000);
	return 0;
}

char *adsl_status[] = {
"�򿪶˿�",
"�˿��Ѵ�",
"�����豸",
"�豸������",
"�����豸������",
"��֤",
"��֤֪ͨ",
"��֤����",
"��֤�ص�",
"��֤�����Ѹı�",
"��֤����",
"��֤��������",
"��֤��Ӧ",
"������֤",
"��֤��ͨ��",
"׼����Ӧ",
"�ȴ�modem��λ",
"�ȴ���Ӧ",
"����",
"��ͣ",
"������֤",
"���еȴ���Ӧ",
"�������",
"������",
"���ӶϿ�",
"δ֪����"
};

int CNetStatusDlg::CheckADSLStatus(int &nStatusCode)
{
	/*
	int ret = 0;
	DWORD flags;//������ʽ   
	unsigned short name[64] = {0};
	//BOOL m_bOnline = InternetGetConnectedState(&flags,0);   
	BOOL m_bOnline = InternetGetConnectedStateEx(&flags, name, 64, 0);
	if(m_bOnline)//����   
	{   
		  switch(flags)   
		  {   
		  case  INTERNET_CONNECTION_MODEM   :   
				 return 1;	    
		  }   
		  if (flags == INTERNET_CONNECTION_MODEM_BUSY)   
			  return 0;
	} 
	*/

	RASCONNSTATUS result;
	DWORD failure = RasGetConnectStatus(gHRasConn, &result);
	nStatusCode = result.rasconnstate;
	if(result.rasconnstate >= RASCS_Interactive && result.rasconnstate <= RASCS_PasswordExpired)
		nStatusCode = result.rasconnstate - RASCS_Interactive + 19;
	else if(result.rasconnstate >= RASCS_Connected && result.rasconnstate <= RASCS_Disconnected)
		nStatusCode = result.rasconnstate - RASCS_Connected + 23;
	
	if(RASCS_Connected == result.rasconnstate)
		return 1;
	else 
		return 0;
}


CNetStatusDlg::CNetStatusDlg(CWnd* pParent /*=NULL*/)
	: CCEDialog(CNetStatusDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNetStatusDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nADSLTimeCount = 0;
	m_bADSLISConnnect = FALSE;
	ADSLInit();
}


void CNetStatusDlg::DoDataExchange(CDataExchange* pDX)
{
	CCEDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNetStatusDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

void CNetStatusDlg::OnRasErrorCode(WPARAM w, LPARAM l)
{
	int result = w;
	char txt[64];
	sprintf(txt, "Ras code %d %d %d\r\n", result, w, l);
//	Dprintf("Ras code %d %d %d\r\n", result, w, l);
	DWriteDat("", (UINT8 *)txt, strlen(txt));
	if(result >= RASCS_Interactive && result <= RASCS_PasswordExpired)
		result = result - RASCS_Interactive + 19;
	else if(result >= RASCS_Connected && result <= RASCS_Disconnected)
		result = result - RASCS_Connected + 23;

	if(result > 25)
		result = 25;

	CString s = adsl_status[result];
	m_MJPGList.SetUnitText(4, s, TRUE);

	if(result == 23)   //������
	{
		KillTimer(1);
		m_bADSLISConnnect = TRUE;
		m_MJPGList.SetUnitIsShow(4, TRUE);
		m_nADSLTimeCount++;
		SetTimer(1, 1000, NULL);
		SetType(1, TRUE);
	}
	else if(result == 24)   //δ����
	{
		m_bADSLISConnnect = FALSE;
	}
}

LRESULT CNetStatusDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	Dprintf("CNetStatusDlg %x\r\n", message);
	switch (message)
	{
	case 1:
		break;
	case 0xCCCD:
		OnRasErrorCode(wParam, lParam);
		break;
	default:
		break;
	};
	
	return CDialog::WindowProc(message, wParam, lParam);
}

BEGIN_MESSAGE_MAP(CNetStatusDlg, CCEDialog)
	//{{AFX_MSG_MAP(CNetStatusDlg)
	ON_MESSAGE(WM_CLICKMJPG_TOAPP, OnClickMJPG)
	ON_WM_TIMER()
	ON_MESSAGE(WM_RASDIALEVENT, OnRasErrorCode)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNetStatusDlg message handlers
void  CNetStatusDlg::SetType(int type, BOOL bIsConenctStatus)
{
	m_type = type;
	if(type == 0)
	{
		m_MJPGList.SetUnitIsShow(101, FALSE);
		m_MJPGList.SetUnitIsShow(102, FALSE);
		m_MJPGList.SetUnitIsShow(4, FALSE);
		if(bIsConenctStatus)
			m_MJPGList.SetUnitBitmap(2, ".\\adv\\mjpg\\k1\\common\\������ͨ.bmp", "", FALSE);
		else
			m_MJPGList.SetUnitBitmap(2, ".\\adv\\mjpg\\k1\\common\\����Ͽ�.bmp", "", FALSE);
	}
	else if(type == 1)
	{
		m_bADSLISConnnect = bIsConenctStatus;
		if(bIsConenctStatus)
		{
			m_MJPGList.SetUnitBitmap(2, ".\\adv\\mjpg\\k1\\common\\ADSL��ͨ.bmp", "", FALSE);
			m_MJPGList.SetUnitIsShow(102, FALSE);
			m_MJPGList.SetUnitIsShow(101, TRUE);
			m_MJPGList.SetUnitIsShow(4, TRUE);
			m_MJPGList.Invalidate();
		}
		else
		{
			m_MJPGList.SetUnitBitmap(2, ".\\adv\\mjpg\\k1\\common\\ADSL�Ͽ�.bmp", "", FALSE);
			m_MJPGList.SetUnitIsShow(102, TRUE);
			m_MJPGList.SetUnitIsShow(101, FALSE);
			m_MJPGList.Invalidate();
		}
	}
	CString s = "";
	((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pSettingDlg->GetIPInfo();
	((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pSettingDlg->m_ip;
	if(((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pSettingDlg->m_ip.isDHCP)
		s = "����";
	else
		s = "�ر�";
	m_MJPGList.SetUnitText(3, s, FALSE);
	
	s = ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pSettingDlg->m_ip.ipAddress;
	m_MJPGList.SetUnitText(7, s, FALSE);
	
	s = ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pSettingDlg->m_ip.mask;
	m_MJPGList.SetUnitText(8, s, FALSE);
	
	s = ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pSettingDlg->m_ip.gw;
	m_MJPGList.SetUnitText(9, s, FALSE);
	
	s = ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pSettingDlg->m_ip.dns;
	m_MJPGList.SetUnitText(10, s, FALSE);

	if(type == 1 && bIsConenctStatus)
	{
		m_MJPGList.SetUnitIsShow(11, TRUE);
		s = GetPPPoEIP();
		m_MJPGList.SetUnitText(12, s, FALSE);
	}
	else
	{
		m_MJPGList.SetUnitIsShow(11, FALSE);
		s = "";
		m_MJPGList.SetUnitText(12, s, FALSE);
	}
}

BOOL CNetStatusDlg::OnInitDialog() 
{
	CCEDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_MJPGList.Create(L"", WS_VISIBLE|WS_CHILD, CRect(54, 62, 746, 358), this);
	m_MJPGList.SetCurrentLinkFile(".\\adv\\mjpg\\k1\\����\\����״̬.xml");
	m_MJPGList.SetMJPGRect(CRect(54, 62, 746, 358));

//	WM_RASEVENT = ::RegisterWindowMessageA(RASDIALEVENT);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CNetStatusDlg::OnClickMJPG(WPARAM w, LPARAM l)
{
	CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	switch(w)
	{
	case 1:
		m_MJPGList.SetUnitIsShow(4, FALSE);
	//	KillTimer(2);
		ShowWindow_(SW_HIDE);
		break;
	case 3:
		if(!m_bADSLISConnnect)
		{
			char *user = (char *)main->m_pSettingDlg->m_pSetting->dialUsername().c_str();
			char *pwd =  (char *)main->m_pSettingDlg->m_pSetting->dialPassword().c_str();
			int ret = /*m_bADSLISConnnect =*/ ADSLDial(user, pwd, this);
			KillTimer(1);
			SetTimer(1, 1000, NULL);
		
// 			if(m_bADSLISConnnect)
// 			{
// 				m_MJPGList.SetUnitIsShow(4, TRUE);
// 				m_nADSLTimeCount++;
// 				SetTimer(1, 1000, NULL);
// 				SetType(1, TRUE);
// 			}
		}
		break;
	case 2:
		if(m_bADSLISConnnect)
		{
			m_nADSLTimeCount = 0;
			KillTimer(1);
			ADSLHungUp();
			SetType(1, FALSE);
		}
		break;
	}
}

void CNetStatusDlg::ShowWindow_(int cmdshow)
{
	if(cmdshow > 0)
	{
		((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pMainDlg->SendMessage(WM_PLAYVIDEO, 0, 0);    //��ͣ��Ƶ
	
		((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pMainDlg->m_pWebDialog->SendMessage(WM_KILLWEBSHOW, 1, 0);
		::SetWindowPos(m_hWnd, HWND_TOPMOST, 0, 0, 800, 480, 0);
		CCEDialog::ShowWindow_(cmdshow);
	}
	else
	{
		CCEDialog::ShowWindow_(cmdshow);
		((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pMainDlg->SendMessage(WM_PLAYVIDEO, 1, 0);			//�ָ���Ƶ
		((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pMainDlg->m_pWebDialog->SendMessage(WM_KILLWEBSHOW, 0, 0);
	}
}

void CNetStatusDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent == 1)
	{
		m_nADSLTimeCount++;
		int nStatuscode;
		if(!CheckADSLStatus(nStatuscode))
		{
			OnClickMJPG(2, NULL);
		}
		else
		{
			m_nADSLTimeCount++;
			CString s;
			s.Format(_T("%02d:%02d:%02d"), m_nADSLTimeCount/3600, m_nADSLTimeCount%3600/60, m_nADSLTimeCount%60);
			m_MJPGList.SetUnitText(4, s, TRUE);
		}
	}
	else if(nIDEvent == 2)
	{
		int nStatuscode;
		if(!CheckADSLStatus(nStatuscode))
		{
			if(nStatuscode >= 25)
				nStatuscode = 25;
			CString s = adsl_status[nStatuscode];
			m_MJPGList.SetUnitText(4, s, TRUE);
		}
	}

	CCEDialog::OnTimer(nIDEvent);
}
