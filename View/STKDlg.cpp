// fastdialsdlg.cpp : implementation file
//

#include "stdafx.h"
#include "..\multimediaphone.h"
#include "..\MultimediaPhoneDlg.h"
#include "STKDlg.h"
#include "../Data/LanguageResource.h"
#include "../Data/SkinStyle.h"
#include "sip.h"
#include "../Util/ATCommandWarp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSTKDlg dialog


CSTKDlg::CSTKDlg(CWnd* pParent /*=NULL*/)
	: CCEDialog(CSTKDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSTKDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nCurrentCount = -1;
	memset(m_nCmdidFather, -1, sizeof(int)*18);
	m_nMenuCount = 0;
}

void CSTKDlg::OnATSend(char *src, char *dsc, char *ack, char *ack1)
{

}

void CSTKDlg::ShowMenu()
{
	for(int i = 0; i < 18; i++)
	{
		m_MJPGList.SetUnitIsShow(i+3, FALSE, FALSE);
	}
	for(i = 0; i < m_nMenuCount; i++)
	{
		CString b1 = L".\\adv\\mjpg\\k5\\common\\百事通列表底图1.bmp";
		CString b2 = L".\\adv\\mjpg\\k5\\common\\百事通列表底图2.bmp";
		m_MJPGList.SetUnitText(i+3, m_STKMenu[i].sName, FALSE);
		m_MJPGList.SetUnitBitmap(i+3,b1,b2,FALSE);
		m_MJPGList.SetUnitIsShow(i+3, TRUE, FALSE);
	}
	m_MJPGList.Invalidate();
}	

void CSTKDlg::GenResPonse(char *src, BOOL isOK)
{
// 	src[15] = '2';
// 	src[17] = '1';
// 	char cmd[128] = {0};
// 	if(isOK)
// 	{
// 		sprintf(cmd, "A0140000%02X%s030100", strlen(src)/2+3, src);
// 
// 		char cmd_[128] = {0};
// 		sprintf(cmd_, "AT+CSIM=%d,%s\r", strlen(cmd),cmd);
// 		char ans[1024] = {0};
// 		OnATSend(cmd_, cmd, "OK", "^DTUSATURC:");
// 	}
}

char gDeviceBuff[64] = {0};
void CSTKDlg::ParseSTK(const char *src, BOOL isACK)
{
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
			
			char *pMenuStr = strstr(src, "030125");
			char *pStart_ = strstr(src, "02818285");
			char temp[512] = {0};
			char ctkTitle[512] = {0};
			

			char *pdevice = strstr(src, "D0");
			if(pdevice)
			{
				pdevice += 2;
				if(strstr(pdevice, "81") == pdevice)
				{
					pdevice += 6;
				}
				else
				{
					pdevice += 2;
				}
			}
			memset(gDeviceBuff, 0, 64);
			strncpy(gDeviceBuff, pdevice, 9*2);
			//if(isACK)
			//	SetTimer(1, 50, NULL);

			//SetUpMenu
			if(/*strstr(cSIM_C_TAG, "D0") &&*/ pdevice && pMenuStr && pStart_)
			{
				m_nCurrentCount++;
				SMS::SMSWarp::GetSMSWarp()->gsmString2Bytes(pStart_+8, (UINT8 *)temp, 2);
				int len_ = temp[0];
				pStart_ += 10;
				if(len_ > 0)
				{
					memset(temp, 0, 512);
					memset(ctkTitle, 0, 512);
					int len = SMS::SMSWarp::GetSMSWarp()->gsmString2Bytes(pStart_+2, (UINT8 *)temp, (len_-1)*2);
					pStart_ += (len_*2);
					SMS::SMSWarp::GetSMSWarp()->gsmDecodeUcs2((UINT8 *)temp, ctkTitle, len);
					CString sTitle = ctkTitle;
					m_MJPGList.SetUnitText(2, sTitle, FALSE);				
				}
				
				char menubz[3] = {0};
				strncpy(menubz, pStart_, 2);
				char *pItemStart = strstr(pStart_, menubz);
				int jCount = 0;
				
				while(pItemStart)
				{
					if(jCount >= 18)
						break;
					pItemStart+=2;
					SMS::SMSWarp::GetSMSWarp()->gsmString2Bytes(pItemStart, (UINT8 *)temp, 2);
					int off_ = temp[0];
					char *pItemNext = strstr(pItemStart+((off_+1)*2), menubz);
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
						m_STKMenu[jCount].cmdid = nMemuId;
						m_STKMenu[jCount].sName = cMenuTile;
						jCount++;
					
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

						m_STKMenu[jCount].cmdid = nMemuId;
						m_STKMenu[jCount].sName = cMenuTile;
						jCount++;
					
						break;
					}
				}
				m_nMenuCount = jCount;
				ShowMenu();
				return;
			}

			 //SelectItem
			char *pSelectMenuStr = strstr(src, "030124");
			char *pSelectStart_ = strstr(src, "028182");
			//SelectItem
			if(pdevice && pSelectStart_ && pSelectMenuStr)
			{
				m_nCurrentCount++;
				int len_;
				if(strstr(pSelectStart_, "02818285"))
				{
					SMS::SMSWarp::GetSMSWarp()->gsmString2Bytes(pSelectStart_+8, (UINT8 *)temp, 2);
					len_ = temp[0];
					pSelectStart_ += 10;
					if(len_ > 0)
					{
						memset(temp, 0, 512);
						memset(ctkTitle, 0, 512);
						int len = SMS::SMSWarp::GetSMSWarp()->gsmString2Bytes(pSelectStart_+2, (UINT8 *)temp, (len_-1)*2);
						pSelectStart_ += (len_*2);
						SMS::SMSWarp::GetSMSWarp()->gsmDecodeUcs2((UINT8 *)temp, ctkTitle, len);
						CString sTitle = ctkTitle;
						m_MJPGList.SetUnitText(2, sTitle, FALSE);				
					}
				}
				else
				{
					pSelectStart_ += 6;
				}
				
				char menubz[3] = {0};
				strncpy(menubz, pSelectStart_, 2);
				char *pItemStart = strstr(pSelectStart_, menubz);
				int jCount = 0;
				
				while(pItemStart)
				{
					if(jCount >= 18)
						break;
					pItemStart+=2;
					SMS::SMSWarp::GetSMSWarp()->gsmString2Bytes(pItemStart, (UINT8 *)temp, 2);
					int off_ = temp[0];
					char *pItemNext = strstr(pItemStart+((off_+1)*2), menubz);
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
						m_STKMenu[jCount].cmdid = nMemuId;
						m_STKMenu[jCount].sName = cMenuTile;
						jCount++;
						
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
						
						m_STKMenu[jCount].cmdid = nMemuId;
						m_STKMenu[jCount].sName = cMenuTile;
						jCount++;
						
						break;
					}
				}
				m_nMenuCount = jCount;
				ShowMenu();
				return;
			}

			//处理文本
			/*
			pdevice = strstr(src, "D0");
			if(pdevice)
			{
				pdevice += 2;
				if(strstr(pdevice, "81") == pdevice)
				{
					pdevice += 6;
				}
				else
				{
					pdevice += 2;
				}
			}
			memset(gDeviceBuff, 0, 64);
			strncpy(gDeviceBuff, pdevice, 9*2);
			*/
			char *pTextStr = strstr(src, "030121");
			char *pTextStart = strstr(src, "028102");
			if(pdevice && pTextStr && pTextStart)
			{
				char *pBegine = strstr(pTextStart, "0D");
				if(!pBegine)
					pBegine = strstr(pTextStart, "8D");
				
				if((pBegine -  pTextStart) <= 8)
				{
					SMS::SMSWarp::GetSMSWarp()->gsmString2Bytes(pBegine+2, (UINT8 *)temp, 2);
					int len_ = temp[0];
					pBegine += 6;
					if(len_ > 0)
					{
						memset(temp, 0, 512);
						memset(ctkTitle, 0, 512);
						int len = SMS::SMSWarp::GetSMSWarp()->gsmString2Bytes(pBegine, (UINT8 *)temp, (len_- 1)*2);
						SMS::SMSWarp::GetSMSWarp()->gsmDecodeUcs2((UINT8 *)temp, ctkTitle, len);
						CString sTitle = ctkTitle;
						((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetTitle(sTitle);
						((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetHWnd(NULL);
						((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->ShowWindow_(TRUE);				
					}
				}
				return;
			}
		}
	}
}

void CSTKDlg::OnTimer(UINT nIDEvent)
{
	if(nIDEvent == 1)
	{
		KillTimer(1);
		GenResPonse(gDeviceBuff, 1);
	}
}

BOOL CSTKDlg::SetCmdID(int id, int nCount, BOOL isFindFatherCmd)
{
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	int nCmdid;
	if(isFindFatherCmd)
	{
		m_nCmdidFather[m_nCurrentCount]  = -1;
		m_nCurrentCount--;
		if(m_nCurrentCount > 0)
		{
			nCmdid = 0;
		}
		else
			nCmdid = -1;
		m_nCurrentCount--;
	}
	else
	{
		m_nCurrentCount++;
		m_nCmdidFather[m_nCurrentCount] = id;
		m_nCurrentCount--;
		nCmdid = id;
	}

	if(nCmdid == -1)
	{
		//检查SIM卡状态
		main->m_pATCommandWarp1->DSATREF_(0);	
	}
	else 
	{
		char cmd[64] = {0};
		sprintf(cmd, "D307020201811001%02X", (UINT8)nCmdid);
		char cmd1[64] = {0};
		sprintf(cmd1, "A0C20000%02X%s", strlen(cmd)/2,cmd);
		
		char cmd_[128] = {0};
		sprintf(cmd_, "AT+CSIM=%d,%s\r",strlen(cmd1), cmd1);

		main->m_pATCommandWarp1->Transaction_(cmd_,strlen(cmd_));
	}
	return TRUE;
}

void CSTKDlg::ResetCmdID()
{
	m_nCurrentCount = -1;
	memset(m_nCmdidFather, -1, sizeof(int)*12);
	m_nMenuCount = 0;
}

void CSTKDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSTKDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSTKDlg, CCEDialog)
	//{{AFX_MSG_MAP(CSTKDlg)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_CLICKMJPG_TOAPP, OnClickMJPG)
	ON_WM_TIMER()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSTKDlg message handlers

void CSTKDlg::doOK()
{
	for(int i = 0; i < 18; i++)
	{
		if(m_MJPGList.GetUnitIsDownStatus(i+3))
		{
			if(m_nMenuCount > i)
			{
				SetCmdID(m_STKMenu[i].cmdid, m_nCurrentCount, FALSE);
			}
			break;
		}
	}
}

void CSTKDlg::doSelectMenu(int w)
{
	for(int i = 0; i < 18; i++)
	{
		if(m_MJPGList.GetUnitIsDownStatus(i+3))
			m_MJPGList.SetUnitIsDownStatus(i+3, FALSE);
	}
	m_MJPGList.SetUnitIsDownStatus(w, TRUE);
	m_MJPGList.Invalidate();
}

void CSTKDlg::doCancel()
{
	ResetCmdID();
	ShowWindow_(FALSE);
	return;

	if(m_nCurrentCount <= 0)
	{	
		CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
		main->m_pATCommandWarp1->DSATREF_(4);
		ResetCmdID();
		ShowWindow_(FALSE);
	}
	else
	{
		SetCmdID(0, m_nCurrentCount, TRUE);
	}
}

void CSTKDlg::OnClickMJPG(WPARAM w, LPARAM l)
{
	switch(w)
	{
	case 100:
		doOK();
		break;
	case 101:
		doCancel();
		break;
	case 3:
	case 4:
	case 5:
	case 6:
	case 7:
	case 8:
	case 9:
	case 10:
	case 11:
	case 12:
	case 13:
	case 14:
	case 15:
	case 16:
	case 17:
	case 18:
	case 19:
	case 20:
		doSelectMenu(w);
		break;
	}
}

BOOL CSTKDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
//	m_chbCallToCall.Create(L"", WS_CHILD|WS_VISIBLE|BS_CHECKBOX, CRect(54+68, 62+74, 54+68+35, 62+74+32), this, IDC_CHECK_SETTING_AUTOLEAVEWORD);
//	m_chbCallToCall.SetIcon(IDI_ICON_CHECK1, CSize(32, 32), IDI_ICON_CHECK0, CSize(32, 32));
//	SetButtonDefaultColor(&m_chbCallToCall);	
	

	m_MJPGList.Create(L"", WS_VISIBLE|WS_CHILD, CRect(54, 62, 746, 358), this);
	m_MJPGList.SetCurrentLinkFile(".\\adv\\mjpg\\k5\\中文\\stk.xml");
	m_MJPGList.SetMJPGRect(CRect(54, 62, 746, 358));

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}