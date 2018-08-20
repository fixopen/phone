// TestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MultimediaPhone.h"
#include "TestDlg.h"

#include "../MultimediaPhoneDlg.h"
#include "../Logical/Phone.h"
#include "../Data/Contact.h"
#include <memory>
#include "../View/ContactNewDlg.h"
#include "../View/TelephoneDlg.h"
#include "../View/settingdlg.h"
#include <map>
//#include <time.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//static CRITICAL_SECTION cs;
#define UM_CALL_COUNT		(WM_USER + 1)
#define TIMER_MEMORY		10

extern BOOL RandName(char chName[], int nBufSize);		//nBufSize >= 10
extern BOOL RandPhone(char chPhone[12], int nBufSize);	//nBufSize >= 12

extern const char *pFamily[];
extern const char *pName[];
extern const char *pCompany[];
extern const char *pDepartment[];

extern const TCHAR *ptcFileCall;
extern const TCHAR *ptcFileDbTest;
extern const TCHAR *ptcFileMem;
extern BOOL Writelog(LPCTSTR filePath, LPCTSTR lpContent);

/////////////////////////////////////////////////////////////////////////////
// CTestDlg dialog
BOOL _test_call;

CTestDlg::CTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTestDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTestDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bFlagCall = FALSE;
	m_bFlagTestDb = FALSE;
	m_bExit = FALSE;
}

void CTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTestDlg)
	//DDX_Control(pDX, IDC_TEST_EDIT, m_edit1);
	DDX_Control(pDX, IDC_TEST_LISTBOX, m_listBox);
	DDX_Control(pDX, IDC_LIST_MEMORY, m_listBoxMem);
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

void CTestDlg::OnTelData(WPARAM w, LPARAM l)
{
	unsigned char *data = (unsigned char *)w;
	int len = l;
	CString s;
	s.Format(_T("Tel recv: %x\r\n"), len); 
	m_listBoxMem.AddString(s);
	for(int i = 0; i < len; i++)
	{
		s.Format(_T("%x "), *(unsigned char *)(data+i)); 
		m_listBoxMem.AddString(s);
	}
	s.Format(_T("\r\n"), len); 
	m_listBoxMem.AddString(s);
//	m_listBoxMem.SendMessage(WM_VSCROLL, SB_BOTTOM, 0);
}

BEGIN_MESSAGE_MAP(CTestDlg, CDialog)
	//{{AFX_MSG_MAP(CTestDlg)
	ON_BN_CLICKED(ID_BTN_TEST_CALL, OnBtnTestCall)
	ON_BN_CLICKED(ID_BTN_TEST_DB, OnBtnTestDb)
	ON_BN_CLICKED(ID_BTN_TEST_EXIT,OnBtnTestExit)
	ON_WM_TIMER()
	ON_MESSAGE(WM_TEL_RING, OnTelData)
	ON_MESSAGE(WM_USER+2, OnExit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestDlg message handlers

void CTestDlg::OnBtnTestCall() 
{
	// TODO: Add your control notification handler code here
	if (!m_bFlagCall)
	{
		if(!m_bFlagTestDb)
		{
			GetDlgItem(ID_BTN_TEST_CALL)->SetWindowText(L"�绰���Ų��Խ���");
			m_listBox.ResetContent();
			m_bFlagCall = TRUE;
			DWORD dwThread = 0;
			HANDLE hThread = CreateThread(NULL, 0, ThreadCallProc, reinterpret_cast<LPVOID>(this), 0, &dwThread);
		}
	}

	else
	{
		KillTimer(1);
		KillTimer(2);
		GetDlgItem(ID_BTN_TEST_CALL)->SetWindowText(L"�绰���Ų��Կ�ʼ");
		m_bFlagCall = FALSE;
	}
}

void CTestDlg::OnBtnTestDb() 
{
	// TODO: Add your control notification handler code here
	if (!m_bFlagTestDb)
	{
		if (!m_bFlagCall)
		{
			GetDlgItem(ID_BTN_TEST_DB)->SetWindowText(L"���ݿ���Խ���");
			m_listBox.ResetContent(); 
			m_bFlagTestDb = TRUE;
			DWORD dwThread = 0;
			HANDLE hThread = CreateThread(NULL, 0, ThreadTestDbProc, reinterpret_cast<LPVOID>(this), 0, &dwThread);
		}
	}

	else
	{
		GetDlgItem(ID_BTN_TEST_DB)->SetWindowText(L"���ݿ���Կ�ʼ");
		m_bFlagTestDb = FALSE;
	}
}

DWORD CTestDlg::ThreadCallProc(LPVOID lParam)
{
	CTestDlg *pDlg = reinterpret_cast<CTestDlg*>(lParam);
	int nCount = 0;

	CTelephoneDlg *PhoneDlg = reinterpret_cast<CTelephoneDlg*>(((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pTelephoneDlg);
	CSettingDlg *pSetDlg = reinterpret_cast<CSettingDlg*>(((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pSettingDlg);
	std::map<char, std::string> ms =pSetDlg->m_pSetting->speedDials();
	pDlg->m_listBox.AddString(_T("��ʼ����绰����:"));

	//log
	CTime time = CTime::GetCurrentTime();
	CString strInfo(_T("begin to test calling: "));
//	strInfo += time.Format(_T("Y%-m%-%d %H:%M:%S"));
	CString strTime;
	strTime.Format(_T("%02d-%02d-%02d %02d:%02d:%02d"), time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond());
	strInfo += strTime;
	strInfo += _T("\r\n");
	Writelog(ptcFileCall, strInfo);

	while (pDlg->m_bFlagCall /* && nCount < 10000*/)
	{
// 		Phone *pPhone = reinterpret_cast<CMultimediaPhoneDlg*>(theApp.m_pMainWnd)->phone_;
// 		pPhone->HungOn();
// 		Sleep(1000);
// 		pPhone->DialNumber("612");
// 		Sleep(5000);
// 		pPhone->HungOff();
// 		Sleep(3000);
		//log
		strInfo.Empty();
		strInfo.Format(_T("Times %d:\t"), nCount+1);
		time = CTime::GetCurrentTime();
		//strInfo += time.Format(_T("Y%-m%-%d %H:%M:%S"));
		strTime.Format(_T("%02d-%02d-%02d %02d:%02d:%02d"), time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond());
		strInfo += strTime;
		strInfo += _T("\r\n");
		Writelog(ptcFileCall, strInfo);


		PhoneDlg->HandleOn();
		Sleep(1500);
		PhoneDlg->DialContact(ms[1]);

		pDlg->SetTimer(1, 1000, NULL);
	//	Sleep(5000);
// 
// #if 1	//  [12/19/2008 wwf]
// //		PhoneDlg->Connect_(theApp.m_pMainWnd);
// 		PhoneDlg->OnButtonTelephoneRecord();
// 	//	Sleep(40000);
// #endif
// 		PhoneDlg->HandleOff();
		
		TCHAR buf[64];
		swprintf(buf, L"Times of calling: %d", ++nCount);
		pDlg->m_listBox.AddString(buf);
		pDlg->m_listBox.SendMessage(WM_VSCROLL, SB_BOTTOM, 0);
		Sleep(60000);
	}
	
	pDlg->m_bFlagCall = FALSE;
	pDlg->m_listBox.AddString(_T("����绰���Խ���"));
	pDlg->m_listBox.SendMessage(WM_VSCROLL, SB_BOTTOM, 0);
	Sleep(1000);
	
	//log
	time = CTime::GetCurrentTime();
	strInfo = _T("end of calling test: ");
	//strInfo += time.Format(_T("Y%-m%-%d %H:%M:%S"));
	strTime.Format(_T("%02d-%02d-%02d %02d:%02d:%02d"), time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond());
	strInfo += strTime;
	strInfo += _T("\r\n");
	Writelog(ptcFileCall, strInfo);
	strInfo.Format(_T("running times: %d\r\n\r\n"), nCount);
	Writelog(ptcFileCall, strInfo);

	if(pDlg->m_bExit)
		pDlg->PostMessage(WM_USER+2, 0, 0);

	return 0;
}

DWORD CTestDlg::ThreadTestDbProc(LPVOID lParam)
{
	CTestDlg *pDlg = reinterpret_cast<CTestDlg*>(lParam);
	CMultimediaPhoneDlg *pMainDlg = reinterpret_cast<CMultimediaPhoneDlg*>(theApp.m_pMainWnd);

	pDlg->m_listBox.AddString(_T("��ʼ���ݿ�д�����:"));
	
	HWND p = pMainDlg->m_pContactDlg->GetSafeHwnd();
	if ( p == NULL )
	{
		pDlg->m_bFlagTestDb = FALSE;
		return -1;
	}
	int nTotal = pMainDlg->m_pContactDlg->GetContactTotal();
	unsigned int nCurQuantity = Data::Contact::GetDataCount("id>0");
	int nRange = nTotal-nCurQuantity;
	int i = 0;

	CTime time = CTime::GetCurrentTime();
	CString strInfo(_T("begin to test db: "));
	//	strInfo += time.Format(_T("Y%-m%-%d %H:%M:%S"));
	CString strTime;
	strTime.Format(_T("%02d-%02d-%02d %02d:%02d:%02d"), time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond());
	strInfo += strTime;
	strInfo += _T("\r\n");
	Writelog(ptcFileDbTest, strInfo);
	
	while (pDlg->m_bFlagTestDb/* && i < 1*/)
	{
		int nCount = 0;
		
		strInfo.Empty();
		strInfo.Format(_T("Times of %d:\t"), i+1);
		time = CTime::GetCurrentTime();
		//strInfo += time.Format(_T("Y%-m%-%d %H:%M:%S"));
		strTime.Format(_T("%02d-%02d-%02d %02d:%02d:%02d"), time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond());
		strInfo += strTime;

		while(pDlg->m_bFlagTestDb && nCount<nRange)
		{
			::Sleep(100);
			std::auto_ptr<Data::Contact> pContact( new Data::Contact );
			
			// 		srand( (unsigned)time(NULL) );
			// 		int n = rand();
			
			char chName[16] = {0};
			char chPhone[16] = {0};
			RandName(chName, 16);
			RandPhone(chPhone, 16);
			
			pContact->alias(pMainDlg->m_pContactNewDlg->GetAlias(chName));
			pContact->name(chName);
			pContact->mobilesTelephone(Data::TelephoneNumber(chPhone));
			
			int nIndex = rand()%30;
			pContact->company(pCompany[nIndex]);
			nIndex = rand()%11;
			pContact->department(pDepartment[nIndex]);
			
			pContact->Insert(); 
			nCount++;

			if (Data::Contact::GetDataCount("") == 0xFFFFFFFF)
			{
				int i = 0;
				i++;
			//	pDlg->m_bFlagTestDb = FALSE;
			//	break;
			}

			if(nCount%10 == 0)
			{
				CString s;
				s.Format(_T("д��Ƭ %d"), nCount);
				pDlg->m_listBox.AddString(s);
				pDlg->m_listBox.SendMessage(WM_VSCROLL, SB_BOTTOM, 0);
			}

			Sleep(50);
		}

		pDlg->m_bFlagTestDb = FALSE;
		nCurQuantity = Data::Contact::GetDataCount("id>0");
		if (nCurQuantity == nTotal)
		{
		//	pDlg->MessageBox(_T("���ݿ�����, �����߳��˳�"), _T("��ʾ"));
		}
		
		strInfo += _T("\t");
		time = CTime::GetCurrentTime();
		//strInfo += time.Format(_T("Y%-m%-%d %H:%M:%S"));
		strTime.Format(_T("%02d-%02d-%02d %02d:%02d:%02d"), time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond());
		strInfo += strTime;
		strInfo += _T("\r\n");
		Writelog(ptcFileDbTest, strInfo);
		i++;
	}

	pDlg->m_listBox.AddString(_T("���ݿ�д����Խ���"));
	pDlg->m_listBox.SendMessage(WM_VSCROLL, SB_BOTTOM, 0);
	Sleep(1000);

	//log
	time = CTime::GetCurrentTime();
	strInfo = _T("end of db test: ");
	//strInfo += time.Format(_T("Y%-m%-%d %H:%M:%S"));
	strTime.Format(_T("%02d-%02d-%02d %02d:%02d:%02d"), time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond());
	strInfo += strTime;
	strInfo += _T("\r\n");
	Writelog(ptcFileDbTest, strInfo);
	strInfo.Format(_T("running times: %d\r\n\r\n"), i);
	Writelog(ptcFileDbTest, strInfo);

	if(pDlg->m_bExit)
		pDlg->PostMessage(WM_USER+2, 0, 0);

	return 0;
}

void CTestDlg::OnBtnTestExit()
{
	if(!m_bFlagCall && !m_bFlagTestDb)
	{
		OnExit(0, 0);
		return;
	}

	m_bFlagCall = FALSE;
	m_bFlagTestDb = FALSE;

	m_bExit = TRUE;
}

void CTestDlg::OnExit(WPARAM w, LPARAM l)
{
	if (!m_bFlagCall && !m_bFlagTestDb)
	{
		KillTimer(1);
		KillTimer(2);
		_test_call = FALSE;
		m_bExit = FALSE;
		ShowWindow(SW_HIDE);
	}
}

BOOL CTestDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	SetTimer(TIMER_MEMORY, 60000, NULL);

	return TRUE;
}

void CTestDlg::OnTimer(UINT nIDEvent) 
{
	if (TIMER_MEMORY == nIDEvent)
	{
		static int nCount = 0;
		if (nCount!=0 && nCount%100 == 0)
		{
			m_listBoxMem.ResetContent();
			nCount = 0;
		}

		CString strMemInfo;
		MEMORYSTATUS memInfo;
		
		memInfo.dwLength = sizeof(memInfo);
		GlobalMemoryStatus(&memInfo);
		strMemInfo.Format(_T("memory load: %u%%, total physical: %u(B), available physical: %u(B)  "), memInfo.dwMemoryLoad, memInfo.dwTotalPhys, memInfo.dwAvailPhys);
		
		CTime time = CTime::GetCurrentTime();
		CString strTime;
		strTime.Format(_T("%02d-%02d-%02d %02d:%02d:%02d"), time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond());
		strMemInfo += strTime;

		m_listBoxMem.AddString(strMemInfo);

		strMemInfo += _T("\r\n");
		Writelog(ptcFileMem, strMemInfo);
		m_listBoxMem.PostMessage(WM_VSCROLL, SB_BOTTOM, 0);

		nCount++;
	}
	else if(nIDEvent == 1)		//�Զ�¼��
	{
		//��������ǿ��¼��
	//	CTelephoneDlg *PhoneDlg = reinterpret_cast<CTelephoneDlg*>(((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pTelephoneDlg);
		KillTimer(1);
	//	PhoneDlg->OnButtonTelephoneRecord();
		SetTimer(2, 29000, NULL);
	}
	else if(nIDEvent == 2)  //�Զ�¼������
	{
		KillTimer(2);
		CTelephoneDlg *PhoneDlg = reinterpret_cast<CTelephoneDlg*>(((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pTelephoneDlg);
		PhoneDlg->HandleOff();
	}
	CDialog::OnTimer(nIDEvent);
}

//������--------------------------------------------------------------------------------------------//
//  [12/3/2008 wwf]
const char *pFamily[] = {
"��","��","��","��","��","��","��","��","��","��",
"��","��","��","��","��","��","��","��","��","��",
"֣","��","��","л","��","��","��","��","��","��",
"��","��","��","��","��","��","Ԭ","��","��","��",
"��","Ҷ","��","κ","��","��","��","��","��","��",
"��","��","��","��","¬","��","��","��","��","½",
"��","Ҧ","��","��","��","��","̷","Τ","��","��",
"��","ʯ","��","��","��","��","Ѧ","��","��","��",
"��","��","��","��","��","ʷ","ë","��","��","��",
"��","��","��","��","��","��","Ǯ","ʩ","ţ","��",		
"��","��",
"��ٹ","˾��","�Ϲ�","ŷ��","�ĺ�",
"���","����","����","����","�ʸ�",
"ξ��","����","�̨","��ұ","����",
"���","����","����","̫��","����",
"����","����","��ԯ","���","����",
"����","����","Ľ��","����","����",
"˾ͽ","˾��","����","˾��","�붽", 
"�ӳ�","���","��ľ","����","����",
"���","����","����","����","�ذ�",
"�й�","�׸�","����","����","�Ʒ�",
"��۳","Ϳ��","�θ�","����","����",
"����","����","�麣","����","΢��",
"��˧","�ÿ�","����","����","����",
"����","����","����","��Ĳ","��٦",
"����","�Ϲ�","ī��","����","�갮",						
"��١"};			//163

const char *pName[] = {
"��","��","��","��","��","��","��","��","��","��",
"��","��","��","��","��","��","��","��","��","��",
"��","��","��","��","��","��","��","ּ","��","��",
"��","��","��","ͬ","��","͡","��","��","��","��",
"��","��","��","��","��","��","��","լ","��","��",
"��","��","��","��","��","ӡ","��","��","��","��",
"��","��","��","��","��","��","��","��","��","�",
"ة","��","��","��","��","ȫ","��","��","��","��",
"��","��","��","լ","��","��","��","��","ʽ","��",
"��","��","��","Ѯ","��","��","��","��","ϵ","��",
"��","��","��","Ѫ","��","��","��","��","��","��",
"��","��","��","��","��","��","ɫ","��","��","��",
"��","��","��","��","��","Ĳ","��","��","��","��",
"��","��","��","��","��","��","��","��","ϵ","��",
"��","��","��","��","��","��","��","��","��","��",
"٤","��","��","��","��","��","��","��","��","��",
"��","��","�","��","��","�","��","��","��","��",
"��","ס","ռ","��","��","��","��","��","��","��",
"��","Ŭ","��","��","��","׳","��","�","��","Ū",
"͢","��","ͮ","־","��","��","��","��","��","��",
"��","��","��","��","��","��","��","��","��","١",
"��","��","��","��","�","��","��","��","λ","��",
"��","ұ","��","��","��","��","��","β","��","��",
"��","��","��","��","��","��","��","��","��","��",
"��","��","��","��","��","Щ","٤","Щ","��","��",
"��","��","Т","��","�","ϣ","�","��","��","Ѳ",
"��","��","��","��","��","ɼ","��","��","�","��",
"��","ϫ","��","˽","��","��","��","��","��","��",
"ϵ","ռ","��","ס","��","��","ۿ","۾","��","��",
"��","׳","ױ","��","��","־","��","��","��","��",
"��","ȴ","��","��","��","��","��","��","��","��",
"��","��","��","��","��","��","��","��","ÿ","��",
"ĵ","��","��","��","Т","β","��","ϣ","��","��",
"æ","��","��","Ѵ","��","��","��","��","��","��",
"��","��","ؽ","��","��","��","ί","��","��","��"};  //350
const char *pCompany[] = {
	"�����ʻ���",	
	"��������ʻ���",
	"��ˬ�ʻ���",
	"��ɫ�鱦����",
	"ͯ�ϵ�Ե�鱦",
	"��֮Է��ó���޹�˾",
	"����������ҵ��Ʒ����(�й����ܲ�",
	"�ú����鱦���޹�˾",
	"�㽭����ѩ��������ֳ��˾",
	"�ж�Ԫ�鱦���޹�˾",
	"���챦������",
	"��ɫ���װר����",
	"FUBUר����",
	"������ʥ�߲���ó���޹�˾",
	"����ɽ֮Է��ó���޹�˾",
	"���ŷ�װ�г�",
		"��������԰���ַ�װ�����г�",
		"��������¡��װ��",
		"�������°�˳ʱװ��",
		"������������װ��",
		"����÷���ݷ�װ��",
		"Ʒ��Է",
		"����԰��ׯ",
		"�洨����",
		"�η���˹�",
		"�����潭",
		"��ɫ��ɽ",
		"̶�ݾ�¥",
		"�����",
		"��ͥ¥"
};		//30

const char *pDepartment[] = {
	"���³��칫��",
		"�ܾ���칫��",
		"������Դ��",
		"��ܲ�",
		"������",
		"������",
		"���۲�",
		"����",
		"���ڲ�",
		"�����豸��",
		"�ɹ���"
};		//11

BOOL RandName(char chName[], int nBufSize)
{
	if ( !chName || nBufSize<10) return FALSE;
	
	memset(chName, 0, nBufSize);

	int nFlag = rand()%2;
	
	int nIndex1 = rand()%163;
	int nIndex2 = rand()%350;
	sprintf(chName, "%s%s", pFamily[nIndex1], pName[nIndex2]);
	
	if (nFlag)
	{
		int nIndex3 = rand()%350;
		strcat(chName, pName[nIndex3]);
	}

	return TRUE;
}

BOOL RandPhone(char chPhone[12], int nBufSize)
{
	if ( !chPhone || nBufSize<12 ) return FALSE;
	
	memset(chPhone, 0, nBufSize);

	int nFlag = rand()%2;
	

	if (nFlag)
	{
		char chbuf[6] = {0};
		int nNumber = rand()%100000;
		sprintf(chPhone, "13%05d", nNumber);

		nNumber = rand()%10000;
		sprintf(chbuf, "%04d", nNumber);
		strcat(chPhone, chbuf);
	}
	else
	{
		char chbuf[6] = {0};
		int nNumber = rand()%100000;
		sprintf(chPhone, "15%05d", nNumber);
		
		nNumber = rand()%10000;
		sprintf(chbuf, "%04d", nNumber);
		strcat(chPhone, chbuf);
	}

	return TRUE;
}
//-------------------------------------------------------------------------------------------------//

//  [12/10/2008 wwf]---�������ݴ洢----------------------------------------------------------------//
const TCHAR *ptcFileCall = _T("/flashdrv/testcall.log");
const TCHAR *ptcFileDbTest = _T("/flashdrv/testdb.log");
const TCHAR *ptcFileMem = _T("/flashdrv/testMem.log");

BOOL Writelog(LPCTSTR filePath, LPCTSTR lpContent)
{
	
	CStdioFile file;
	if ( !file.Open(filePath, CStdioFile::modeCreate|CStdioFile::modeNoTruncate|CStdioFile::modeWrite) )
	{
		return FALSE;
	}
	file.SeekToEnd();
//	file.WriteString(lpContent);
	file.Write(lpContent, lstrlen(lpContent)*sizeof(TCHAR));
	file.Close();
	return TRUE;
}
//--------------------------------------------------------------------------------------------------//