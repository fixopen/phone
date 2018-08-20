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
			GetDlgItem(ID_BTN_TEST_CALL)->SetWindowText(L"电话拨号测试结束");
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
		GetDlgItem(ID_BTN_TEST_CALL)->SetWindowText(L"电话拨号测试开始");
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
			GetDlgItem(ID_BTN_TEST_DB)->SetWindowText(L"数据库测试结束");
			m_listBox.ResetContent(); 
			m_bFlagTestDb = TRUE;
			DWORD dwThread = 0;
			HANDLE hThread = CreateThread(NULL, 0, ThreadTestDbProc, reinterpret_cast<LPVOID>(this), 0, &dwThread);
		}
	}

	else
	{
		GetDlgItem(ID_BTN_TEST_DB)->SetWindowText(L"数据库测试开始");
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
	pDlg->m_listBox.AddString(_T("开始拨打电话测试:"));

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
	pDlg->m_listBox.AddString(_T("拨打电话测试结束"));
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

	pDlg->m_listBox.AddString(_T("开始数据库写入测试:"));
	
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
				s.Format(_T("写名片 %d"), nCount);
				pDlg->m_listBox.AddString(s);
				pDlg->m_listBox.SendMessage(WM_VSCROLL, SB_BOTTOM, 0);
			}

			Sleep(50);
		}

		pDlg->m_bFlagTestDb = FALSE;
		nCurQuantity = Data::Contact::GetDataCount("id>0");
		if (nCurQuantity == nTotal)
		{
		//	pDlg->MessageBox(_T("数据库已满, 测试线程退出"), _T("提示"));
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

	pDlg->m_listBox.AddString(_T("数据库写入测试结束"));
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
	else if(nIDEvent == 1)		//自动录音
	{
		//可以设置强制录音
	//	CTelephoneDlg *PhoneDlg = reinterpret_cast<CTelephoneDlg*>(((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pTelephoneDlg);
		KillTimer(1);
	//	PhoneDlg->OnButtonTelephoneRecord();
		SetTimer(2, 29000, NULL);
	}
	else if(nIDEvent == 2)  //自动录音结束
	{
		KillTimer(2);
		CTelephoneDlg *PhoneDlg = reinterpret_cast<CTelephoneDlg*>(((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pTelephoneDlg);
		PhoneDlg->HandleOff();
	}
	CDialog::OnTimer(nIDEvent);
}

//测试用--------------------------------------------------------------------------------------------//
//  [12/3/2008 wwf]
const char *pFamily[] = {
"王","李","张","刘","陈","杨","黄","赵","周","吴",
"徐","孙","朱","马","胡","郭","林","何","高","梁",
"郑","罗","宋","谢","唐","韩","曹","许","邓","萧",
"冯","曾","程","蔡","彭","潘","袁","于","董","余",
"苏","叶","吕","魏","蒋","田","杜","丁","沈","姜",
"范","江","傅","钟","卢","汪","戴","崔","任","陆",
"廖","姚","方","金","邱","夏","谭","韦","车","贾",
"邹","石","熊","孟","秦","阎","薛","侯","雷","白",
"龙","段","郝","孔","邵","史","毛","常","万","顾",
"赖","武","康","贺","严","尹","钱","施","牛","洪",		
"龚","怯",
"万俟","司马","上官","欧阳","夏侯",
"诸葛","闻人","东方","赫连","皇甫",
"尉迟","公羊","澹台","公冶","宗政",
"濮阳","淳于","单于","太叔","申屠",
"公孙","仲孙","轩辕","令狐","钟离",
"宇文","长孙","慕容","鲜于","闾丘",
"司徒","司空","亓官","司寇","仉督", 
"子车","颛孙","端木","巫马","公西",
"漆雕","乐正","壤驷","公良","拓拔",
"夹谷","宰父","谷梁","晋楚","闫法",
"汝鄢","涂钦","段干","百里","东郭",
"南门","呼延","归海","羊舌","微生",
"岳帅","缑亢","况后","有琴","梁丘",
"左丘","东门","西门","商牟","佘佴",
"伯赏","南宫","墨哈","谯笪","年爱",						
"阳佟"};			//163

const char *pName[] = {
"六","伉","光","匡","共","各","考","交","件","价",
"企","伍","伎","仰","吉","圭","曲","机","艮","仲",
"吉","州","朱","兆","决","匠","地","旨","朵","吏",
"列","年","劣","同","打","汀","至","臼","灯","竹",
"老","舟","伎","吊","吏","圳","的","宅","机","老",
"肉","虫","伊","仰","伍","印","因","宇","安","屹",
"有","羊","而","耳","衣","亦","吃","夷","奸","聿",
"丞","企","休","任","先","全","再","冲","刑","向",
"在","夙","如","宅","守","字","存","寺","式","戌",
"收","早","旭","旬","曲","次","此","求","系","肉",
"臣","自","舌","血","行","圳","西","休","交","件",
"企","匠","吉","尖","而","至","色","伏","后","名",
"回","好","妃","帆","灰","牟","百","份","米","伐",
"亥","卉","冰","刑","合","向","旭","朴","系","行",
"汜","复","克","告","改","攻","更","杆","谷","况",
"伽","估","君","吴","吸","吾","圻","均","坎","研",
"完","局","岐","我","扣","杞","江","究","见","角",
"言","住","占","低","佃","况","里","冷","伶","利",
"助","努","君","吝","昌","壮","妓","妞","局","弄",
"廷","弟","彤","志","托","杖","杜","呆","李","江",
"男","究","良","见","角","具","皂","里","舟","佟",
"你","体","足","甸","町","豆","吞","玎","位","佐",
"佘","冶","吾","吟","吴","吻","完","尾","巫","役",
"忘","我","修","言","邑","酉","吟","吴","研","呆",
"角","七","伸","佐","作","些","伽","些","初","吹",
"呈","坐","孝","宋","岐","希","岑","床","序","巡",
"形","忍","成","杏","材","杉","束","村","杞","步",
"汝","汐","池","私","秀","赤","足","身","车","辰",
"系","占","伺","住","余","助","劭","劬","邵","吸",
"坐","壮","妆","局","床","志","汕","江","灶","见",
"即","却","克","早","何","布","伯","伴","佛","兵",
"判","别","含","坊","坂","吵","宏","旱","每","甫",
"牡","况","免","孚","孝","尾","巫","希","庇","形",
"忙","杏","步","汛","贝","儿","庚","快","抗","昆",
"果","空","亟","其","具","奇","委","季","宜","居"};  //350
const char *pCompany[] = {
	"龙族鲜花店",	
	"怡红快绿鲜花店",
	"送爽鲜花店",
	"阅色珠宝玉器",
	"童氏地缘珠宝",
	"宝之苑商贸有限公司",
	"靓伊爱尔工业饰品连锁(中国）总部",
	"颐和祥珠宝有限公司",
	"浙江温岭雪瑶珍珠养殖公司",
	"中鼎元珠宝有限公司",
	"聚天宝首饰行",
	"七色麻服装专卖店",
	"FUBU专卖店",
	"北京东圣高策商贸有限公司",
	"北京山之苑商贸有限公司",
	"天雅服装市场",
		"北京动物园天乐服装批发市场",
		"北京福昌隆服装店",
		"北京百衣百顺时装店",
		"北京瑞丽华服装店",
		"北京梅兰妮服装店",
		"品博苑",
		"丰泽园饭庄",
		"湘川鄂情",
		"嘉丰湘菜馆",
		"龙鼎湘江",
		"红色江山",
		"潭州酒楼",
		"湘鄂春",
		"洞庭楼"
};		//30

const char *pDepartment[] = {
	"董事长办公室",
		"总经理办公室",
		"人力资源部",
		"企管部",
		"技术部",
		"生产部",
		"销售部",
		"财务部",
		"后勤部",
		"基建设备部",
		"采购部"
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

//  [12/10/2008 wwf]---测试数据存储----------------------------------------------------------------//
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