// TestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MultimediaPhone.h"
#include "TestATDlg.h"

#include "../MultimediaPhoneDlg.h"
#include "../Logical/Phone.h"
#include "../Data/Contact.h"
#include <memory>
#include "../View/ContactNewDlg.h"
#include "../View/TelephoneDlg.h"
#include "../View/settingdlg.h"
#include "../Util/ATCommandWarp.h"
#include "../Util/SMSWarp.h"
#include "../MultimediaPhoneDlg.h"
#include "../MultimediaPhone.h"
#include <map>
#include "sip.h"
//#include <time.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CTestATDlg::CTestATDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTestATDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTestATDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

}

void CTestATDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTestATDlg)
	//DDX_Control(pDX, IDC_TEST_EDIT, m_edit1);
	// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTestATDlg, CDialog)
	//{{AFX_MSG_MAP(CTestATDlg)
	ON_BN_CLICKED(0x101, OnBtnSend)
	ON_BN_CLICKED(0x102, OnBtnExit)
	ON_MESSAGE(WM_COMMBOX_CLICKED, OnATSelect)
	ON_BN_CLICKED(0x106, OnBtnSetDDTM)
	ON_BN_CLICKED(0x107, OnBtnReSearchNet)
	ON_BN_CLICKED(0x108, OnBtnReRist)
	ON_BN_CLICKED(0x109, OnBtnCopyLog)
	ON_BN_CLICKED(0x201, OnBtnNewStart)
	ON_BN_CLICKED(0x202, OnBtnNewEnd)
	ON_BN_CLICKED(0x10A, OnBtnADSL)
	ON_BN_CLICKED(0x10B, OnBtnADSLOFF)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CTestATDlg::OnBtnADSL(WPARAM w, LPARAM l)
{
	/*
	main->SetAPN(1);
	CNetStatusDlg::ADSLDial(main->m_nAPN.dialnumber, "", "", (CWnd *)this, (DIALTYPE)main->m_nAPN.apn_type);
	*/

	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;

	extern BOOL IsTestCTA();
	if(IsTestCTA())    //调试
	{
		if(!main->m_pComWarp7)
		{
			main->m_pComWarp7 = new Util::ComWarp();
			BOOL ret = main->m_pComWarp7->OpenComm(L"COM7:");
			if(!ret)
			{
				AfxMessageBox(L"COM7 error!");
			}
		}
	}
	
	CString s;
	char cmd[1024] = {0};
	char ans[1024] = {0};
	GetDlgItemText(0x103, s);
	if(s.GetLength() > 0)
	{
		std::string s_ = Util::StringOp::FromCString(s);
		strcpy(cmd, s_.c_str());
		strcat(cmd, "\r");
		
		((CEdit *)GetDlgItem(0x104))->ReplaceSel(Util::StringOp::ToCString(cmd));
		
		char ans[1024] = {0};
		main->m_pComWarp7->WriteComm(cmd, strlen(cmd));
		Sleep(100);
		int count = 0;
		while(1)
		{
			int c = main->m_pComWarp7->ReadComm(ans, 256);
			if(c > 0)
				break;
			Sleep(1000);
			if(count++ > 5)
				break;
		}
		((CEdit *)GetDlgItem(0x104))->ReplaceSel(Util::StringOp::ToCString(ans));
	}
}
void CTestATDlg::OnBtnADSLOFF(WPARAM w, LPARAM l)
{
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	main->Net3GHungOff();

	main->SetAPN(1);
	CNetStatusDlg::ADSLDial(main->m_nAPN.dialnumber, "", "", (CWnd *)this, (DIALTYPE)main->m_nAPN.apn_type, NULL);
}

/////////////////////////////////////////////////////////////////////////////
// CTestATDlg message handlers
BOOL isKaInterfaceTest()
{
#if(CTA_ONLY == 1)
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	if(main->m_pSettingDlg->m_pSetting->isUseScreenSaverPassword()) 
		return TRUE;
	return FALSE;
#else
	return FALSE;
#endif
}

BOOL IsTestCTA()
{
#if(CTA_ONLY == 1)
	
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	if(main->m_pSettingDlg->m_pSetting->isNightControlBlackLight()) 
		return TRUE;
	return FALSE;
#else
	return FALSE;
#endif
}

void EditPrintf(char *ptr)
{
//	if(IsTestCTA())
	{
		if(strlen(ptr) > 0)
		{
			CString s = ptr;
			extern CTestATDlg *m_pTestATDlg0;
			if (m_pTestATDlg0)
			{
				m_pTestATDlg0->m_lstAT.ReplaceSel(s);
			}
		}
	}
}

void CTestATDlg::OnBtnNewEnd(WPARAM w, LPARAM l)
{
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	main->m_pATCommandWarp1->Off();
//	::Sleep(50);
}

void CTestATDlg::OnBtnNewStart(WPARAM w, LPARAM l)
{
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	int ret = main->m_pATCommandWarp1->Connect("",false);
	if (ret == 1)    //1 成功    
	{
		((CEdit *)GetDlgItem(0x104))->ReplaceSel(L"搜网成功!\r");	
	}
	else if(ret == 2) //2   需要注册码
	{
		((CEdit *)GetDlgItem(0x104))->ReplaceSel(L"网络拒绝注册,搜网失败!\r");	
	}
	else if(ret == 3)   //未插卡
	{
		((CEdit *)GetDlgItem(0x104))->ReplaceSel(L"未插卡,搜网失败!\r");
	}
	::Sleep(50);
}

void CTestATDlg::OnBtnSetDDTM(WPARAM w, LPARAM l)
{
	char cmd[32];
	if(m_Radio1.GetCheck_())
	{
		strcpy(cmd, "AT^DDTM=1,2\r");
	}
	else if(m_Radio2.GetCheck_())
	{
		strcpy(cmd, "AT^DDTM=0,2\r");
	}
	else if(m_Radio3.GetCheck_())
		strcpy(cmd, "AT^DDTM=0,0\r");
	else
		strcpy(cmd, "AT^DDTM=1,0\r");

	char ans[1024] = {0};
	OnATSend(cmd, ans);
/*
	if(strstr(ans, "OK"))
	{

	}
	else
	{

	}
*/
}

void CTestATDlg::OnBtnReSearchNet(WPARAM w, LPARAM l)
{
	char cmd[32];
	strcpy(cmd, "AT+COPS=2\r");
	char ans[1024] = {0};
	OnATSend(cmd, ans);
	if(strstr(ans, "OK"))
	{
		memset(ans, 0, 1024);
		strcpy(cmd, "AT+COPS=?\r");
		OnATSend(cmd, ans);
		if(strstr(ans, "OK"))
		{
			m_cmbSearchBox.ResetContent();
			char *ptr = ans;
			char *p1 = strstr(ptr, "(");
			while(p1)
			{
				char *p11 = strstr(p1, "\",\"");
				if(p11)
				{
					p11 += 3;
					char *p11_ = strstr(p11, "\",\"");
					if(p11_)
					{
						p11 = p11_ + 3;
						char *p12 = strstr(p11, "\"");
						if(p12)
						{
							char temp[32] = {0};
							memcpy(temp, p11, p12-p11);
							CString s = temp;
							m_cmbSearchBox.AddString(s);
						}
					}
				}
				if(p11)
					p1 = strstr(p11, "(");
				else
					p1 = NULL;
			}

		}
	}
}


void CTestATDlg::OnBtnCopyLog(WPARAM w, LPARAM l)
{
	CString s;
	m_edtAT.GetWindowText(s);
	s += ".txt";
	CString sDir = "/StorageCard/";
	sDir += s;
	extern BOOL DetectDIR(TCHAR *sDir);
	if(!DetectDIR(L"/StorageCard"))
	{
		MessageBox(L"未插入U盘!");
		return;
	}
	if(s.GetLength() == 0)
	{
		MessageBox(L"文件名为空!");
		return;
	}
	
	CopyFile(L"/flashdrv/mylog.txt", sDir, FALSE);
	CString sDirFile = sDir;
	sDirFile +=  "拷贝完成!"; 
	MessageBox(sDirFile);
	DeleteFile(L"/flashdrv/mylog.txt");
	
}

void CTestATDlg::OnBtnReRist(WPARAM w, LPARAM l)
{
	CString s;
	m_cmbSearchBox.GetWindowText(s);

	if(s.GetLength() > 0)
	{
		char cmd[64];
		sprintf(cmd, "AT+COPS=1,2,\"%s\"\r", Util::StringOp::FromCString(s).c_str());
		char ans[1024] = {0};
		OnATSend(cmd, ans);

		if((strstr(ans, "+CREG: 1") != NULL) || (strstr(ans, "+CREG: 5") != NULL) ||((strstr(ans, "OK") != NULL) && (strstr(ans, "+CREG: 3") == NULL)))
		{
			::PostMessage(theApp.m_pMainWnd->m_hWnd, WM_3GNET_OK, 0, 0);
		}
	}
}

void CTestATDlg::OnATSelect(WPARAM w, LPARAM l)
{
	SipShowIM(SIPF_OFF);
	if(w == 0x105)
	{
		CString s;
		m_cmbBox.GetWindowText(s);
		m_edtAT.SetWindowText(s);
	}
}

void CTestATDlg::ResPonseSTK(char *cmd_indifity, int result)
{
	return;

	char csim[128] = {0};
	char content[128] = {0};
	strcpy(csim, "at+csim=");
	strcpy(content, cmd_indifity);
	if(result == 0)
	{
		strcat(content, "0303");
		strcat(content, "4F4B");
	}

}

void CTestATDlg::ParseSTK(const char *src)
{
	return;

	char cSIM_C_TAG[3] = {0};
	char cCommand_Tag[2] = {0};
	char cDevice_Indefity[3] = {0};
	char cAlpa_Indefity[3]= {0};
	char nTotalLength = 0;
	if(strstr(src, "^DTUSATURC:"))
	{
		char *pStart = strstr(src, ",");
		if(pStart)
		{
			strncpy(cSIM_C_TAG, pStart+3, 2);
			char cLength[2] = {0};
			nTotalLength = SMS::SMSWarp::GetSMSWarp()->gsmString2Bytes(pStart+5, (UINT8 *)cLength, 2);
			nTotalLength = cLength[0];
			char *ptr = (char *)(pStart + 7);
			
			strncpy(cCommand_Tag, ptr, 2);
			ptr += 2;

			char c_Type_Command[3] = {0};
			strncpy(c_Type_Command, ptr+4, 2);
			ptr+=8;

			strncpy(cDevice_Indefity, ptr, 2);
			ptr+=8;

			if(strstr(cSIM_C_TAG, "D0") &&  strstr(c_Type_Command, "25"))
			{
				char *pItemStart = strstr(ptr, "0F80");
				char stkTitle[64] = {0};
				char *endTitle;
				if(pItemStart)
				{
					endTitle = strstr(pItemStart+4, "0F");
					if(endTitle)
					{
						memcpy(stkTitle, pItemStart+4, endTitle-pItemStart-4);
						char ctkTitle[64] = {0};
						int len = SMS::SMSWarp::GetSMSWarp()->gsmString2Bytes(stkTitle, (UINT8 *)ctkTitle, strlen(stkTitle));
						memset(stkTitle, 0, 64);
						SMS::SMSWarp::GetSMSWarp()->gsmDecodeUcs2((UINT8 *)ctkTitle, stkTitle, len);
						CString s = "Title: ";
						s += stkTitle;
						s += "\r\n";
						((CEdit *)GetDlgItem(0x104))->ReplaceSel(s);
					}
				}
				else
				{
					pItemStart = strstr(ptr, "0F");
				}
				pItemStart = endTitle;
				while(pItemStart)
				{
					pItemStart+=2;
					char *pItemNext = strstr(pItemStart, "0F");
					if(pItemNext)
					{
						char sMenuTile[64] = {0};
						char cMenuTile[64] = {0};
						int nMemuId;
						int nLen1;
						int len = SMS::SMSWarp::GetSMSWarp()->gsmString2Bytes(pItemStart, (UINT8 *)sMenuTile, pItemNext-pItemStart);
						nMemuId = sMenuTile[1]; 
						nLen1 = sMenuTile[0];

						SMS::SMSWarp::GetSMSWarp()->gsmDecodeUcs2((UINT8 *)sMenuTile+3, cMenuTile, nLen1-2);
						pItemStart = pItemNext;
						CString s;
						sprintf(sMenuTile, "%d %d: %s\r\n", nMemuId, nLen1, cMenuTile);
						s = sMenuTile;
						((CEdit *)GetDlgItem(0x104))->ReplaceSel(s);
					}
					else
					{
						char sMenuTile[64] = {0};
						char cMenuTile[64] = {0};
						int nLen1;
						int len = SMS::SMSWarp::GetSMSWarp()->gsmString2Bytes(pItemStart, (UINT8 *)sMenuTile, strlen(pItemStart));
						int nMemuId = sMenuTile[1];
						nLen1 = sMenuTile[0];

						SMS::SMSWarp::GetSMSWarp()->gsmDecodeUcs2((UINT8 *)sMenuTile+3, cMenuTile, nLen1-2);
						CString s;
						sprintf(sMenuTile, "%d %d: %s\r\n", nMemuId, nLen1, cMenuTile);
						s = sMenuTile;

						((CEdit *)GetDlgItem(0x104))->ReplaceSel(s);
						break;
					}
				}
			}
		}
	}
}

void CTestATDlg::OnATSend(char *src, char *dsc)
{
	((CEdit *)GetDlgItem(0x104))->ReplaceSel(Util::StringOp::ToCString(src));
	
	extern Util::ATCommandWarp* GetATCommandWarp();
	Util::ATCommandWarp *pATCommanWarp = GetATCommandWarp();
	char ans[1024] = {0};
	pATCommanWarp->Transaction_(src, strlen(src),"",okCTA);
	((CEdit *)GetDlgItem(0x104))->ReplaceSel(Util::StringOp::ToCString(ans));
	strcpy(dsc, ans);
}

void CTestATDlg::OnBtnSend()
{
	SipShowIM(SIPF_OFF);
	char cmd[1024] = {0};
	CString s;
	GetDlgItemText(0x103, s);
//	if(s.Find(L"AT") >= 0 || s.Find(L"at") >= 0)
	{
		std::string s_ = Util::StringOp::FromCString(s);
		strcpy(cmd, s_.c_str());
		strcat(cmd, "\r");

		((CEdit *)GetDlgItem(0x104))->ReplaceSel(Util::StringOp::ToCString(cmd));

		extern Util::ATCommandWarp* GetATCommandWarp();
		Util::ATCommandWarp *pATCommanWarp = GetATCommandWarp();
		char ans[1024] = {0};
		pATCommanWarp->Transaction_(cmd, strlen(cmd),"",okCTA);
		
		((CEdit *)GetDlgItem(0x104))->ReplaceSel(Util::StringOp::ToCString(ans));
		ParseSTK(ans);
	}
}

void CTestATDlg::OnBtnExit()
{
	SipShowIM(SIPF_OFF);
	ShowWindow(SW_HIDE);
}

BOOL CTestATDlg::OnReadATFile() 
{
	BOOL flag = FALSE;
	m_cmbBox.ResetContent();
	FILE *file = fopen("/StorageCard/at.txt", "r+b");
	if(file)
	{
		while(!feof(file))
		{
			char txt[1024] = {0};
			fgets(txt, 1024, file);
			CString s = txt;
			s.TrimRight(L" ");
			s.TrimLeft(L" ");
			s.TrimRight(L"\n");
			s.TrimRight(L"\r");
			s.TrimRight(L"\r");
			s.TrimRight(L"\n");
			s.TrimRight(L"\n");
			s.TrimRight(L"\r");
			s.TrimRight(L"\r");
			s.TrimRight(L"\n");
			s.TrimRight(L" ");
			if(s != "")
			{
				m_cmbBox.AddString(s);
			}
		}
	}
	else
	{
		m_cmbBox.AddString(L"at+");
		m_cmbBox.AddString(L"+++");

		m_cmbBox.AddString(L"AT+CGDCONT=1,\"IP\",\"\",\"\",0,0");
		m_cmbBox.AddString(L"AT+CGEQREQ=1,2,64,64,0,0,0,0,\"0E0\",\"0E0\",,0,0");
		m_cmbBox.AddString(L"at+cgact=1,1");

		m_cmbBox.AddString(L"at^");
		m_cmbBox.AddString(L"atd10086;");
		m_cmbBox.AddString(L"atd0123456902;");
		m_cmbBox.AddString(L"atd112;");
		m_cmbBox.AddString(L"ath");
		m_cmbBox.AddString(L"ata");
		m_cmbBox.AddString(L"at+cmms=1");
		m_cmbBox.AddString(L"AT^DCTA=1");
		m_cmbBox.AddString(L"AT^DCTA=0");
		m_cmbBox.AddString(L"AT+CGATT=1");
		m_cmbBox.AddString(L"AT+CGATT=0");		
		m_cmbBox.AddString(L"at+cpls=");
		m_cmbBox.AddString(L"at+cpol?");
		m_cmbBox.AddString(L"at+cops?");
		m_cmbBox.AddString(L"at^ddtm?");
		m_cmbBox.AddString(L"at^ddtm=1,2");
		m_cmbBox.AddString(L"AT^DACTI?");
		m_cmbBox.AddString(L"at+csq");
		m_cmbBox.AddString(L"at^dstm=1,1,0");
		m_cmbBox.AddString(L"at^dstm=1,1,2");
		m_cmbBox.AddString(L"at^dsatref=0");
		m_cmbBox.AddString(L"at^dtusat=1");
		m_cmbBox.AddString(L"at^dusatp=0,1,1");
	}
	return TRUE;
}

BOOL CTestATDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_btnSend.Create(L"发送", WS_VISIBLE|WS_CHILD, CRect(680, 50, 790, 100), this, 0x101);
//	if(IsTestCTA())
//		m_btnStart.Create(L"开机", WS_VISIBLE|WS_CHILD, CRect(640, 120, 720, 170), this, 0x201);
//	m_btnEnd.Create(L"关机", WS_VISIBLE|WS_CHILD, CRect(720, 120, 790, 170), this, 0x202);
	m_btnExit.Create(L"退出", WS_VISIBLE|WS_CHILD, CRect(680, 420, 790, 470), this, 0x102);

	m_cmbBox.Create(WS_VISIBLE|WS_CHILD, CRect(50, 2, 630, 479), this, 0x105);
	m_edtAT.Create(WS_VISIBLE|WS_CHILD|ES_MULTILINE | ES_AUTOVSCROLL | WS_VSCROLL, CRect(45, 50, 630, 108+60), this, 0x103);
	m_lstAT.Create(WS_VISIBLE|WS_CHILD|ES_MULTILINE| ES_AUTOVSCROLL | WS_VSCROLL, CRect(50, 120+60, 500, 470), this, 0x104);
	m_cmbBox.AddString(L"at+");
	m_cmbBox.AddString(L"at^");
	m_cmbBox.AddString(L"atd");
//	m_cmbBox.AddString(L"AT^DDEBUG=2,5");
	m_cmbBox.AddString(L"at+cpls=");
	m_cmbBox.AddString(L"at+cpol?");
	m_cmbBox.AddString(L"at+cops?");
	m_cmbBox.AddString(L"at^ddtm?");
	m_cmbBox.AddString(L"at^ddtm=1,2");
	m_cmbBox.AddString(L"AT^DACTI?");
	m_cmbBox.AddString(L"at+csq");
	m_cmbBox.AddString(L"at^dstm=1,1,0");
	m_cmbBox.AddString(L"at^dstm=1,1,2");
	m_cmbBox.AddString(L"at^dsatref=0");
	m_cmbBox.AddString(L"at^dtusat=1");
	m_cmbBox.AddString(L"at^dusatp=0,1,1");

	m_Radio1.Create(L"TD Only", WS_CHILD/*|WS_VISIBLE*/, CRect(530, 120+60, 650, 160+60), this);
	m_Radio2.Create(L"TD 优先", WS_CHILD/*|WS_VISIBLE*/, CRect(530, 165+60, 650, 205+60), this);
	m_Radio4.Create(L"G网 Only", WS_CHILD/*|WS_VISIBLE*/, CRect(660, 120+60, 800, 160+60), this);
	m_Radio3.Create(L"G网 优先", WS_CHILD/*|WS_VISIBLE*/, CRect(660, 165+60, 800, 205+60), this);

	m_SetDDMT.Create(L"设置", /*|WS_VISIBLE*/WS_CHILD, CRect(680, 210+60, 780, 240+60), this, 0x106);

	extern int  gIsTDStatus;
	if(gIsTDStatus == 1)
	{
		m_Radio1.SetCheck_(1);
		m_Radio2.SetCheck_(0);
		m_Radio3.SetCheck_(0);
		m_Radio4.SetCheck_(0);
	}
	else if(gIsTDStatus == 2)
	{
		m_Radio2.SetCheck_(1);
		m_Radio1.SetCheck_(0);
		m_Radio3.SetCheck_(0);
		m_Radio4.SetCheck_(0);
	}
	else if(gIsTDStatus == 0)
	{
		m_Radio3.SetCheck_(1);
		m_Radio1.SetCheck_(0);
		m_Radio2.SetCheck_(0);
		m_Radio4.SetCheck_(0);
	}
	else
	{
		m_Radio3.SetCheck_(0);
		m_Radio1.SetCheck_(0);
		m_Radio2.SetCheck_(0);
		m_Radio4.SetCheck_(1);
	}
	CButton *pButton[4] = {&m_Radio1, &m_Radio2, &m_Radio3, &m_Radio4};
	m_Radio1.SetGroupButton(pButton, 4);
	m_Radio2.SetGroupButton(pButton, 4);
	m_Radio3.SetGroupButton(pButton, 4);
	m_Radio4.SetGroupButton(pButton, 4);
	
//	m_btnSearch.Create(L"重新搜网", WS_VISIBLE|WS_CHILD, CRect(530, 210+60, 630, 240+60), this, 0x107);
//	m_cmbSearchBox.Create(WS_VISIBLE|WS_CHILD, CRect(530, 250+60, 780, 479), this, 0x107);;
//	m_btnReReg.Create(L"重新登陆", WS_VISIBLE|WS_CHILD, CRect(530, 310+30, 630, 340+30), this, 0x108);
	m_btnCopyLog.Create(L"考出Log", WS_VISIBLE|WS_CHILD, CRect(530, 420, 630, 470), this, 0x109);
	m_btnADSLON.Create(L"拨号", WS_VISIBLE|WS_CHILD, CRect(530, 370, 630, 418), this, 0x10A);
	m_btnADSLOFF.Create(L"上网连接", WS_VISIBLE|WS_CHILD, CRect(630, 390, 730, 418), this, 0x10B);

	return TRUE;
}
