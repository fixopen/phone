// MultimediaPhoneDlg.cpp : implementation file
//
#include "stdafx.h"
#include "MultimediaPhone.h"
#include "MultimediaPhoneDlg.h"
#include "./View/ViewConstants.h"
#include "./Data/SkinStyle.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "View/InformationDlg.h"
#include "Data/LanguageResource.h"
#include "winbase.h"
#include "inc/ximadef.h"
#include "Util/CParseFile.h"
#include "Util/Md5.h"
#include "Util/CUseOpre.h"

//#define WM_PLAYVIDEO	8002
#define  KEYTIMEOUT  5 * 1000
#define  RINGTIMEOUT 6 * 1000
#define  CALLIDTIMEOUT 2*1000
#define USBINSERT_EVENT_NAME     _T("USBINSERTED")
#define USBREMOVE_EVENT_NAME     _T("USBREMOVED")
extern BOOL DetectDIR(TCHAR *sDir);
extern BOOL DetectFile(TCHAR *sDir);
extern  int GetItem();
///zzw//////////////////////010818
BOOL ISSpeedDial=TRUE;
int gLangItem=1;
HANDLE m_hReadThread;
CPSerialPort  port;
BOOL dbIsIntact=TRUE;
BOOL m_bProgressContine=FALSE;
BOOL g_bBattery = FALSE;
extern BOOL m_bIsFinishDownLoad;
const std::string LogPath="\\FlashDrv\\Log.txt";

FILE * srcfp=NULL;
FILE * desfp=NULL;
BOOL m_bIsFinishCopy=FALSE;
BOOL m_bInsertUSB=FALSE;

extern BOOL Writelog(std::string filePath,unsigned char * ERRORContent, int len,std::string ERRORTYPE);
extern BOOL Writelog(std::string filePath,std::string conttent, std::string ERRORTYPE);

//extern void MD5_Data(unsigned char * data, unsigned int dataLen, unsigned char md5[16]);

/////////////////////////////////////////////////////////////////////////////
// CMultimediaPhoneDlg dialog

void  Close_DB()
{
	return;

	Data::Contact::CloseDb();

	Data::ContactInfo::Closedb();

	Data::ContactGroup::Closedb();

	Data::SoundSegment::Clsoedb();

	Data::ContactInfoPictureMap::Closedb();

	Data::ContactInfoSoundSegmentMap::Closedb();

	Data::LanguageResource::Closedb();

	Data::Scheduler::Closedb();

	Data::Setting::Closedb();

}

void Open_DB()
{  
	return;

	Data::Contact::Opendb();
	
	Data::ContactInfo::Opendb();
	
	Data::ContactGroup::Opendb();
	
	Data::SoundSegment::Opendb();
	
	Data::ContactInfoPictureMap::Opendb();
	
	Data::ContactInfoSoundSegmentMap::Opendb();
	
	Data::LanguageResource::Opendb();
	
	Data::Scheduler::Opendb();
	
	Data::Setting::Opendb();
}


void CloseRW()
{
	if (srcfp!=NULL)
	{
		fclose(srcfp);

		srcfp=NULL;
	}

	if (desfp!=NULL)
	{
		fclose(desfp);

		desfp=NULL;
	}

}

void CopyDB(std::string srcPath, std::string desPath,std::string testContext)
 {  
	CloseRW();

	srcfp=fopen(srcPath.c_str(),"rb");

	if (srcfp == NULL)
	{  
		m_bIsFinishCopy=TRUE;

		return ;
	}
   
	fseek(srcfp,0L,SEEK_END);
	
	int srclen=ftell(srcfp);

	fseek(srcfp,0L,SEEK_SET);

    char buff[1024];
	
	memset(buff,'\0',1024);
	
	fread(buff,sizeof(char),18,srcfp);
	
	std::string sbuff=buff;
	
	if (sbuff == "SQLite format 3" && srclen >= 51200)
	{	
		memset(buff,'\0',1024);
		
	}else{
		
		fclose(srcfp);

		m_bIsFinishCopy=TRUE;
		
		srcfp=NULL;
			
		return;
	}

		fseek(srcfp,0L,SEEK_SET);
		
		desfp=fopen(desPath.c_str(),"wb+");
		
		if (desfp != NULL)
		{   
			int desWriteCount=0;
			
		    while (desWriteCount < srclen && !feof(srcfp))
			{
				 int srcReadCount =fread(buff,sizeof(char),1024,srcfp);
	
			     fwrite(buff,sizeof(char),srcReadCount,desfp);
					
				 fflush(desfp);
					
				 memset(buff,'\0',1024);
					
			     desWriteCount=desWriteCount+srcReadCount;
			}

			fclose(srcfp);

			srcfp=NULL;
			
			fclose(desfp);

			desfp=NULL;

			m_bIsFinishCopy=TRUE;

		}else{

           fclose(srcfp);

		   srcfp=NULL;

		   m_bIsFinishCopy=TRUE;
		   	
		}
}

BOOL IsInvailedDB()
{
   	sqlite3 * invailed=0;
	
	invailed=Data::OpenDB();

	if (invailed)
	{
		if (Data::Contact::GetDataCount("") < 0)
		{  
			Data::CloseDB(invailed);

			return FALSE;
			
		}else if (Data::ContactInfo::GetDataCount("") <0)
		{  
			Data::CloseDB(invailed);

			return FALSE;
			
		}else if (Data::SoundSegment::GetDataCount("") <0)
		{  
			Data::CloseDB(invailed);

			return FALSE;
			
		}else {
			 
			Data::CloseDB(invailed);

			return TRUE;
		}

	}else{

		return FALSE;
	}
}

//   删除目录   
void DeleteDirectory(CString SrcDir, BOOL isShow = TRUE)   
{
	int count = 0;
	CString findFilename = SrcDir + "\\*.*";
	if(isShow)
	{
		RETAILMSG(1, (TEXT("start DeleteFile !!!!!!!!!!! \r\n")));
	}
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind = FindFirstFile((LPCTSTR)findFilename, &FindFileData);
	if (hFind == INVALID_HANDLE_VALUE)
	{

	}
	else
	{
		bool finished = false;
		do
		{
			::Sleep(0);
			wchar_t wideFromName[256] = {0};
			wsprintf(wideFromName, _T("%s/%s"), SrcDir, (LPCTSTR)FindFileData.cFileName); //findFileName

			if((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				DeleteDirectory(wideFromName, isShow);
			}
			else
			{
				if (!::DeleteFile(wideFromName))
				{
				}

				++count;
			}
			
			if (!FindNextFile(hFind, &FindFileData))
			{
				if (GetLastError() == ERROR_NO_MORE_FILES)
				{
					finished = true;
				}
			}
		} while (!finished);
		FindClose(hFind);
	}
}

int ReadSelectLanguagelog()
{
	CStdioFile file;

    LPCTSTR filePath=CString("\\hive\\Language.txt");

	if ( !file.Open(filePath, CStdioFile::modeCreate|CStdioFile::modeNoTruncate|CStdioFile::modeReadWrite) )
	{
		return FALSE;
	}
	file.SeekToBegin();
	char buf[5];
    file.Read(buf,4);
	file.Close();
	CString reslut(buf);
	return Util::StringOp::ToInt(reslut);

}
DWORD WINAPI SetWatchThreadPrc(LPVOID lpParameter)
{
	while (true)
	{
#ifdef _DEBUG
		Sleep(1000);
#else
		((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->SetWatchDog();
		Sleep(1000);
#endif
	}

	return 0;

} 

DWORD WINAPI USBProcessThread(LPVOID lpParameter)
{ 
	DWORD dw;
	
	while(true)
	{  
		
		dw=WaitForMultipleObjects(2,\
			((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->hEvent, FALSE, INFINITE);
		
		switch(dw)
		{
			
		case WAIT_FAILED:
			
			break;
			
		case WAIT_TIMEOUT:
			
			break;
			
		case WAIT_OBJECT_0 + 0:
			{
				m_bInsertUSB=TRUE;
				((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->\
					PopUsbTIP(CMultimediaPhoneDlg::UsbINSERTTIP);

				//
				CString portnumber("COM4:");
				port.OpenPort(LPCTSTR(portnumber),9600,8,1,NOPARITY);
				m_hReadThread=CreateThread(NULL,\
					0,(LPTHREAD_START_ROUTINE)CPSerialPort::ReadPortThread,\
					(LPVOID)&port,0,NULL);
			}
			break;
		case WAIT_OBJECT_0 +1:

			m_bInsertUSB=FALSE;
            
			CloseHandle(m_hReadThread);

			m_hReadThread=INVALID_HANDLE_VALUE;
			
			port.ClosePort();
         
			CCParseFile::resetData();

			CString delRIngList = ssFlashPath + _T("RingList.txt");

			DeleteFile((LPCTSTR)delRIngList);
	
			((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->WriteTOFlag("1");
			
			((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_hangon=false;
             
			((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pDeleteTipDlg->ShowWindow_(FALSE);
       
            ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->UpdateShowList();
			
			break;
		}
		
	}	
}

//检查usb是否连接线程的回调函数dxy
void CheckUsbThread(void *p)
{	
	DWORD dw ;
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	while(true)
	{
		dw = WaitForMultipleObjects(2, main->hEvent, FALSE, INFINITE);
		switch (dw)
		{	
		case WAIT_OBJECT_0 + 0: //USB连接上
			{   
				m_bInsertUSB=TRUE;
				main->PopUsbTIP(CMultimediaPhoneDlg::UsbINSERTTIP);
				InitUsbFunction();
			}
			break;
		case WAIT_OBJECT_0 + 1: //USB被拔掉了
			{	
				m_bInsertUSB=FALSE;
				CloseUsbFunction();
			}
			break;
		}
	}
}

CMultimediaPhoneDlg::CMultimediaPhoneDlg(CWnd* pParent /*=NULL*/)
: CDialog(CMultimediaPhoneDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMultimediaPhoneDlg)
	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32

	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

}

CMultimediaPhoneDlg::~CMultimediaPhoneDlg()
{
	for (std::map<int, CDialog*>::iterator i = panels_.begin(); i != panels_.end(); ++i)
		delete i->second;
}

void CMultimediaPhoneDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMultimediaPhoneDlg)
	// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMultimediaPhoneDlg, CDialog)
//{{AFX_MSG_MAP(CMultimediaPhoneDlg)
ON_WM_LBUTTONDOWN()
ON_WM_TIMER()
//	ON_MESSAGE(WM_CHANGELANGUAGE,OnChangeLanguage)
//}}AFX_MSG_MAP
ON_MESSAGE(WM_SOFTKEY, OnMainSoftKey)
ON_MESSAGE(WM_GEN_EVENT, OnEvent)
/*
ON_BN_CLICKED(IDC_BUTTON_TIME, OnButtonTime)
ON_BN_CLICKED(IDC_BUTTON_IME, OnButtonIME)
ON_BN_CLICKED(IDC_BUTTON_LINE, OnButtonLine)
ON_BN_CLICKED(IDC_BUTTON_MAIN, OnButtonMain)
ON_BN_CLICKED(IDC_BUTTON_CONTACT, OnButtonContact)
ON_BN_CLICKED(IDC_BUTTON_CONTACTINFO, OnButtonContactinfo)
ON_BN_CLICKED(IDC_BUTTON_SOUND, OnButtonSound)
ON_BN_CLICKED(IDC_BUTTON_INFORMATION, OnButtonInformation)
ON_BN_CLICKED(IDC_BUTTON_SETTING, OnButtonSetting)
*/
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMultimediaPhoneDlg message handlers

BOOL CMultimediaPhoneDlg::OnInitDialog()
{
	//lxz test jpg
	//CopyDirFiles();
	CDialog::OnInitDialog();
	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);	
	// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	hEvent[0] = CreateEvent(NULL, FALSE, FALSE, USBINSERT_EVENT_NAME);
	hEvent[1] = CreateEvent(NULL, FALSE, FALSE, USBREMOVE_EVENT_NAME);

	m_hangon=false;
	
	//	CenterWindow(GetDesktopWindow());	// center to the hpc screen
	//::SetWindowPos(m_hWnd, NULL, 0, 0, 800, 480, SWP_NOZORDER|SWP_NOSIZE);
	
	// TODO: Add extra initialization here
	//step1: global variable init
	
	//step2: global workthread init
	
	//step3: config data read and fill
	
	//step4: device and environment check and init
	
	//step5: init UI
	//检测SD ,USB

	m_bIsHaveUSB = DetectDIR(L"UsbDisk");
	//首先确定中英文界面
	gLangItem=ReadSelectLanguagelog();

	if(!DetectDIR((LPTSTR)(LPCTSTR)csFlashRecordPath))
	{
		CreateDirectory(csFlashRecordPath, NULL);
	}

	if(!DetectDIR((LPTSTR)(LPCTSTR)csFlashRingPath))
	{
		CreateDirectory(csFlashRingPath, NULL);
	}

	phone_ = new Phone();

	char buffer_[1024];

    phone_->GetManuInfo(buffer_, 1024);

    memcpy(&m_manuinfo, buffer_, sizeof(MANUINFO));

	m_pDeleteTipDlg = new CDeleteTipDlg(this);	
	m_pDeleteTipDlg->Create(CDeleteTipDlg::IDD);
	
	m_pCCWarningDlg = new CCWarningDlg(this);
	m_pCCWarningDlg->Create(CCWarningDlg::IDD);

	m_bIsHaveUSB= DetectDIR(L"\\FlashDrv\\");
	
	if(m_bIsHaveUSB==FALSE)
	{
		PopUsbTIP(CMultimediaPhoneDlg::UsbINSERTTIP_);
		
		return FALSE;
	}

	m_pFSM = new Util::FSM;

	AddFSMRules();
	
	int xbegin  = 90 ;
	int ybegin  = 8  ;
	int xWidth  = 66 ;
	int yHeight = 16 ;
	ICONKEYSTRUCT softkeyval_[9];
//	softkeyval_[0].sKeyRect = CRect(3, 8*VHEIGHT+2, 40, 24*VHEIGHT);
	softkeyval_[0].sKeyRect = CRect(0,0,0,0);
	softkeyval_[0].nKeyValue = 1;
	softkeyval_[0].nTextId = 0;
	softkeyval_[0].nBitmapId = 0;
	softkeyval_[0].nAlign = IMAGE_LEFT;
	softkeyval_[0].bToggle = FALSE;
	
	xbegin = 3 ;
	softkeyval_[1].sKeyRect = CRect(xbegin,2, xbegin+32, 34);
	softkeyval_[1].nKeyValue = 2;
	softkeyval_[1].nTextId = 0;
	softkeyval_[1].nBitmapId = IDB_BITMAP_MAININPUT;
	softkeyval_[1].nAlign = IMAGE_LEFT;
	softkeyval_[1].bToggle = FALSE;
	
	xbegin = 46 ;
	softkeyval_[2].sKeyRect = CRect(xbegin,2, xbegin+32, 34);
//	softkeyval_[2].sKeyRect = CRect(63,8*VHEIGHT, 78, 24*VHEIGHT);
	softkeyval_[2].nKeyValue = 3;
	softkeyval_[2].nTextId = 0;
	softkeyval_[2].nBitmapId = IDB_BITMAP_MAINTEL;
	softkeyval_[2].nAlign = IMAGE_LEFT;
	softkeyval_[2].bToggle = FALSE;
	
	xbegin   = 90 ;
	ybegin   = 2 ;
	yHeight  = 32;
	softkeyval_[3].sKeyRect = CRect(xbegin, ybegin, xbegin+xWidth, ybegin+yHeight);
	softkeyval_[3].nKeyValue = 4;
//	softkeyval_[3].nTextId = Data::RI_MAIN_MAINBTN;
	softkeyval_[3].nTextId = 0;
	softkeyval_[3].nBitmapId = IDB_BITMAP_MAINDESKTOP;
	softkeyval_[3].nAlign = IMAGE_MIDDLE;
	softkeyval_[3].bToggle = TRUE;
	
	xbegin = 156;
	softkeyval_[4].sKeyRect = CRect(xbegin, ybegin, xbegin+xWidth, ybegin+yHeight);
	softkeyval_[4].nKeyValue = 5;
//	softkeyval_[4].nTextId = Data::RI_MAIN_CARDCASEBTN;
	softkeyval_[4].nTextId = 0 ;
	softkeyval_[4].nBitmapId = IDB_BITMAP_MAINCARD;
	softkeyval_[4].nAlign = IMAGE_MIDDLE;
	softkeyval_[4].bToggle = TRUE;
	
	xbegin = 220;
	softkeyval_[5].sKeyRect = CRect(xbegin, ybegin, xbegin+xWidth, ybegin+yHeight);
	softkeyval_[5].nKeyValue = 6;
//	softkeyval_[5].nTextId = Data::RI_MAIN_CALLLISTBTN;
	softkeyval_[5].nTextId = 0;
	softkeyval_[5].nBitmapId = IDB_BITMAP_MAINCALLLIST;
	softkeyval_[5].nAlign = IMAGE_MIDDLE;
	softkeyval_[5].bToggle = TRUE;
	
	xbegin = 285;
//	softkeyval_[6].sKeyRect = CRect(291, 4+2, 346,33);
	softkeyval_[6].sKeyRect = CRect(xbegin, ybegin, xbegin+xWidth,ybegin+yHeight);
	softkeyval_[6].nKeyValue = 7;
//	softkeyval_[6].nTextId = Data::RI_MAIN_SOUND;
	softkeyval_[6].nTextId = 0;
	softkeyval_[6].nBitmapId = IDB_BITMAP_MAINRECORD;
	softkeyval_[6].nAlign = IMAGE_LEFT;
	softkeyval_[6].bToggle = TRUE;
	
	xbegin = 350;
//	softkeyval_[7].sKeyRect = CRect(347, 4+2, 405, 33);
	softkeyval_[7].sKeyRect = CRect(xbegin, ybegin, xbegin+xWidth,ybegin+yHeight);
	softkeyval_[7].nKeyValue = 8;
//	softkeyval_[7].nTextId = Data::RI_MAIN_STOCKINFOBTN;
	softkeyval_[7].nTextId = 0;
	softkeyval_[7].nBitmapId = IDB_BITMAP_MAININFO;
	softkeyval_[7].nAlign = IMAGE_LEFT;
	softkeyval_[7].bToggle = TRUE;
	
	xbegin = 414 ;
	softkeyval_[8].sKeyRect = CRect(xbegin, ybegin,xbegin+xWidth, ybegin+yHeight);
//	softkeyval_[8].sKeyRect = CRect(405, 4+2, 479, 33);
	softkeyval_[8].nKeyValue = 9;
//	softkeyval_[8].nTextId = Data::RI_MAIN_SYSTTEMSETTING;
	softkeyval_[8].nTextId = 0;
	softkeyval_[8].nBitmapId = IDB_BITMAP_MAINSETTING;
	softkeyval_[8].nAlign = IMAGE_LEFT;
	softkeyval_[8].bToggle = TRUE;
	
	m_StatusKey.Create(IDB_BITMAP_MAIN, IDB_BITMAP_MAINTOGGLE, 3, CPoint(0, 0), 9,  softkeyval_, CRect(0, 204*VHEIGHT+1, 479,232*VHEIGHT), this, WS_VISIBLE|WS_CHILD, IDC_BUTTON_TIME);
	SetSkinStyle();
	//m_StatusKey.SetLeftTimer();
	
	CDialog* panel; 
	m_pInformationdlg = new CInformationDlg();
	m_pInformationdlg->Create(CInformationDlg::IDD);
	panel=m_pInformationdlg;
	panels_[IDC_BUTTON_INFORMATION] = panel;
	currentPanel_ = panel;
	
	m_pSettingDlg = new CSettingDlg();
	m_pSettingDlg->Create(CSettingDlg::IDD);
	panel = m_pSettingDlg;
	panels_[IDC_BUTTON_SETTING] = panel;
	
	m_pTelephoneDlg = new CTelephoneDlg();
	m_pTelephoneDlg->Create(CTelephoneDlg::IDD);
	
	m_pMainDlg = new CMainDlg();
	m_pMainDlg->Create(CMainDlg::IDD);
	panel = m_pMainDlg;
	panels_[IDC_BUTTON_MAIN] = panel;
	
	m_pContactDlg = new CContactDlg();
	m_pContactDlg->Create(CContactDlg::IDD);
	panel = m_pContactDlg;
	panels_[IDC_BUTTON_CONTACT] = panel;
	
	m_pContactInfoDlg = new CContactInfoDlg();
	m_pContactInfoDlg->Create(CContactInfoDlg::IDD);
	panel = m_pContactInfoDlg;
	panels_[IDC_BUTTON_CONTACTINFO] = panel;
	
	m_pSoundDlg = new CSoundDlg();
	m_pSoundDlg->Create(CSoundDlg::IDD);
	panel = m_pSoundDlg;
	panels_[IDC_BUTTON_SOUND] = panel;
	
	m_pContactNewDlg = new CContactNewDlg(panels_[IDC_BUTTON_CONTACT]);
	m_pContactNewDlg->Create(CContactNewDlg::IDD);
	panel = m_pContactNewDlg;
	panels_[IDC_BUTTON_CONTACTNEW] = panel;
	
	m_pContactGroupDlg = new CContactGroupDlg(panels_[IDC_BUTTON_CONTACT]);
	m_pContactGroupDlg->Create(CContactGroupDlg::IDD);
	panel = m_pContactGroupDlg;
	panels_[IDC_BUTTON_CONTACTGROUP] = panel;

	//测试窗口
	m_pTestDlg = new CTestDlg();
	m_pTestDlg->Create(CTestDlg::IDD);

	SwitchPanel_(IDC_BUTTON_MAIN);
	m_uiKeyTimer = 1;
	m_uiRingTimer = 2;
	m_uiCallIdTimer = 3;

	m_sListSearchFilter = "";
	EnableLine(FALSE);
	//设置进入屏保
	SetScreenSaveTimer();
	phone_->OpenBacklight(TRUE);
	hthreadID=CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)SetWatchThreadPrc, NULL, 0, NULL);

	SetTimer(15, 2000, NULL);  //开机首先查询一下供电状态
	m_bUpgrade = FALSE;
	m_bComeRing = FALSE;
	m_keyCode = L"";
    m_isTaking=false;
	
	//
	//注册事件
	extern void registerProcessor();
	registerProcessor();

	//打开USB串口
	InitUsbFunction();
	
	//启动检查USB的线程
	DWORD CheckThreadID = 0 ;
	HANDLE m_hCheckThread = CreateThread (NULL, 0,\
		(LPTHREAD_START_ROUTINE)CheckUsbThread, 0, 0, &CheckThreadID);


//	CreateThread(NULL,\
		0,(LPTHREAD_START_ROUTINE)USBProcessThread,NULL,0,NULL);

	return TRUE;  // return TRUE  unless you set the focus to a control
}
void CMultimediaPhoneDlg::SetButtonDefaultColor(CCEButtonST* button)
{
	button->SetColor(CCEButtonST::BTNST_COLOR_BK_IN, RGB(154,212,255));
	button->SetColor(CCEButtonST::BTNST_COLOR_BK_OUT, RGB(154,212,255));
	button->SetColor(CCEButtonST::BTNST_COLOR_BK_FOCUS, RGB(154,212,255));
}
void CMultimediaPhoneDlg::SetButtonSelectedColor(CCEButtonST* button)
{
	button->SetColor(CCEButtonST::BTNST_COLOR_BK_IN, RGB(235,252,255));
	button->SetColor(CCEButtonST::BTNST_COLOR_BK_OUT, RGB(235,252,255));
	button->SetColor(CCEButtonST::BTNST_COLOR_BK_FOCUS, RGB(225,252,255));
}
void CMultimediaPhoneDlg::ScreenPS()
{
	if (m_pMainDlg->m_mainScreenSaveDlg_->IsWindowVisible())
	{
		if (m_pSettingDlg->m_pSetting->isUseScreenSaverPassword())
		{
			m_pSettingDlg->m_pPasswordDlg->SetType(CHECK_PASSWORD);
			std::string strTemp = m_pSettingDlg->m_pSetting->screenSaverPassword();
			m_pSettingDlg->m_pPasswordDlg->SetOldPassWord((char *)strTemp.c_str());
			m_pSettingDlg->m_pPasswordDlg->SetHWnd(this->m_hWnd);
			m_pSettingDlg->m_pPasswordDlg->ShowWindow(SW_SHOW);																								
		}
		else
		{
			m_pMainDlg->m_mainScreenSaveDlg_->ShowWindow(FALSE);
		}
	}
}

void CMultimediaPhoneDlg::SwitchPanel_(int panelId)
{
	//关闭输入法
	KeyBoardOff();

	if (panels_[panelId] != currentPanel_)
	{
		currentPanel_->ShowWindow(SW_HIDE);
		currentPanel_ = panels_[panelId];
		currentPanel_->ShowWindow(SW_SHOW);

		if (panelId == IDC_BUTTON_MAIN)
		{
			((CMainDlg*)currentPanel_)->UpdateDiskInfo();
			m_StatusKey.SetToggleKey(3);
			//((CMainDlg*)currentPanel_)->Invalidate(TRUE);
		}
		if (panelId == IDC_BUTTON_CONTACT)
		{  
			((CContactDlg *)currentPanel_)->ShowItemsInList();
			((CContactDlg *)currentPanel_)->ShowTypeItems();
			m_StatusKey.SetToggleKey(4);
		}
		if (panelId == IDC_BUTTON_CONTACTINFO)
		{   
			((CContactInfoDlg*)currentPanel_)->ShowItemsInList();
			m_StatusKey.SetToggleKey(5);
		}
		if (panelId==IDC_BUTTON_SOUND)
		{   
			((CSoundDlg*)currentPanel_)->ShowItemsInList(); 
			m_StatusKey.SetToggleKey(6);
		}
		if (panelId == IDC_BUTTON_INFORMATION)
		{
			m_StatusKey.SetToggleKey(7);
		}
		if (panelId==IDC_BUTTON_SETTING)
		{  
			((CSettingDlg*)currentPanel_)->IniCtrlData();
			((CSettingDlg*)currentPanel_)->ShowConfigItems();
            m_StatusKey.SetToggleKey(8);
		}
	}
}

void CMultimediaPhoneDlg::OnButtonMain() 
{
	// TODO: Add your control notification handler code here
	m_pMainDlg->m_mainCalucaterDlg_->ShowWindow(FALSE);  //wzx 20101229
	m_pMainDlg->m_mainLunarderDlg_->ShowWindow(FALSE);
	m_pMainDlg->m_firewalDlg_->ShowWindow(FALSE);

	SwitchPanel_(IDC_BUTTON_MAIN);
}

void CMultimediaPhoneDlg::OnButtonContact() 
{
	// TODO: Add your control notification handler code heres

	if (m_pMainDlg->m_firewalDlg_->IsWindowVisible())
	{
		return;
	}

	SwitchPanel_(IDC_BUTTON_CONTACT);
}

void CMultimediaPhoneDlg::OnButtonContactinfo() 
{
	// TODO: Add your control notification handler code here
	SwitchPanel_(IDC_BUTTON_CONTACTINFO);
}

void CMultimediaPhoneDlg::OnButtonSound() 
{
	// TODO: Add your control notification handler code here
	SwitchPanel_(IDC_BUTTON_SOUND);	
}

void CMultimediaPhoneDlg::OnButtonInformation() 
{
	// TODO: Add your control notification handler code here
	
	SwitchPanel_(IDC_BUTTON_INFORMATION);

	//test CEDialog
	//m_pTestDlg->ShowWindow(TRUE);
}

void CMultimediaPhoneDlg::OnButtonSetting() 
{
	// TODO: Add your control notification handler code here
	SwitchPanel_(IDC_BUTTON_SETTING);	
}

void CMultimediaPhoneDlg::OnButtonTime() 
{
	// TODO: Add your control notification handler code here
}

#include "sip.h"
void CMultimediaPhoneDlg::OnButtonIME() 
{
	// TODO: Add your control notification handler code here

	if (gLangItem == Data::lEnglish)
	{
		SipShowIM(SIPF_ON);
	}
	else
	{
		if(!theApp.inputDlg_->IsWindowVisible())
		{
			theApp.inputDlg_->show();
		}
	}

}

void CMultimediaPhoneDlg::OnButtonLine() 
{
	// TODO: Add your control notification handler code here
	if (m_pFSM->getCurrentState() != tsHangOff)
	{
		//关闭输入法
		KeyBoardOff();
		m_pTelephoneDlg->ShowWindow_(TRUE);
		
	}
}

BOOL CMultimediaPhoneDlg::PreTranslateMessage(MSG* pMsg) 
{
	return CDialog::PreTranslateMessage(pMsg);
}

int g_testDialCount = 0;
int g_testFailCount = 0;  //wzx test 20101224
extern VOID WriteMyLog_(char *ptr, int size);
LRESULT CMultimediaPhoneDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if ((message == WM_TEL_RING)
		|| (message == WM_TEL_HUNGON))
	{
		
		m_pContactInfoDlg->SendMessage(WM_TEL_INUSE, 0, 0);

		m_pSoundDlg->SendMessage(WM_TEL_INUSE, 0, 0);

	    m_pSettingDlg->SendMessage(WM_TEL_INUSE, 0, 0);

	}
	
//  add by qi 20111102
	if (IsKeyShield(message))
	{
		return 0 ;
	}
//

	if(!m_keyCode.IsEmpty() && message != WM_TEL_KEYCODE && message != WM_GEN_EVENT)
	{
		m_keyCode.Empty();
	}

	switch (message) 
	{
	case WM_LBUTTONDOWN:
		SendOutEvnet(0, 0);
		break;
	case WM_TEL_RING:
		m_bComeRing = TRUE;
		HideComboBoxDropDown();
		SendOutEvnet(0, 0);
		TRACE(L"RING \n");
		Writelog(LogPath,Util::StringOp::FromInt(WM_TEL_RING),"recive ring message:");
		m_pFSM->fireEvent(teRing, (void * const)wParam);
		break;
	case WM_TEL_HUNGOFF:
		m_bComeRing = FALSE;
		phone_->m_bDial = FALSE;
		SendOutEvnet(0, 0);
		TRACE(L"HUNG OFF \n");
		Writelog(LogPath,Util::StringOp::FromInt(WM_TEL_RING),"recive HUNG OFF message:");
		m_pFSM->fireEvent(teHangOff, (void * const)wParam);
		break;
    case WM_CHECKFOCRHANGON://96
		break;
	case WM_CHECKFOCRHANGOFF://97
		SendOutEvnet(0, 0);
		if (m_pTelephoneDlg->IsWindowVisible()&&theApp.IsCheckTakingState)
		{   
			::KillTimer(((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pTelephoneDlg->m_hWnd,CHECKLINESTATE);
			m_pTelephoneDlg->ShowWindow(FALSE);
			theApp.IsCheckTakingState=false;
			m_pFSM->fireEvent(teHangOff, (void * const)wParam);
		}
		else
		{   
			::KillTimer(((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pTelephoneDlg->m_hWnd,CHECKLINESTATE);
			m_pTelephoneDlg->setCheckTakingstate(false);
			theApp.IsCheckTakingState=false;
		}
		break;
	case WM_TEL_HUNGON:
		{
			m_bComeRing = TRUE;
			
			//test
			extern VOID WriteLog_(CHAR* str);
			extern void WriteMemory();
			WriteLog_("WM_TEL_HUNGON");
			WriteMemory();
			//

			HideComboBoxDropDown();
			SendOutEvnet(0, 0);
			
			//test
			WriteLog_("WM_TEL_HUNGON_SendOutEvnet");
			WriteMemory();
			//

			TRACE(L"HUNG ON \n");
			m_pFSM->fireEvent(teHangOn, (void * const)wParam);
		}
		break;
	case WM_BATTERY_HUNGON:
		if(g_bBattery)  //wzx 20101227
		{
			m_pDeleteTipDlg->SetPasswordModel(false);
			m_pDeleteTipDlg->SetTitle(Data::LanguageResource::Get(Data::RI_ERROR));
			std::string str1 = Data::LanguageResource::Get(Data::RI_DELETETIP_BATTERYHUNGON);
			m_pDeleteTipDlg->SetDelTip(str1.c_str());
			m_pDeleteTipDlg->ShowWindow_(SW_SHOW);
		}
		break;
	case WM_TEL_CALLIDEND:
		{
			static UINT32 gOldTimer = 0;
			static std::string oldtel;
		    SendOutEvnet(0, 0);

			TRACE(L"CALL ID \n");
			CALLID_INFO* info = (CALLID_INFO*)wParam;
			std::string tel;								//lxz 20101207
			UINT32 gNewTimer = GetTickCount();
			if((gNewTimer - gOldTimer) < 500)
				tel = oldtel + info->number;
			else
			    tel = info->number;
			oldtel = tel;
			gOldTimer = gNewTimer;

			tel="来电号码:"+tel;

			Writelog(" \\FlashDrv\\Log.txt",tel,"recive callid number:");

			m_pFSM->fireEvent(teCallID, (void * const)wParam);
		}
		break;
    case WM_CHECKPASSWORD:
		{
          ((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->m_pSettingDlg->SettingDefault();
		}
		break;
	case WM_CLEARPWD:
		{
			m_pSettingDlg->m_pPasswordDlg->SetType(CHECK_CLEARPASSWORD);
			
			unsigned char cMd5[16];
			char cSerielNo[64];
			memset(cSerielNo, 0, 64);
			memcpy(cSerielNo, m_pSettingDlg->m_sSerialNo, 64);
			
			char cTime[5];
			SYSTEMTIME tm;
			GetLocalTime(&tm);
			int y = tm.wYear+tm.wMonth+tm.wDay;
			sprintf(cTime, "%04d", y);
			strcat(cSerielNo, cTime);
			MD5_Data((unsigned char *)cSerielNo, strlen(cSerielNo), cMd5); 
			char cPwd[32] = {0};
			sprintf(cPwd, "%02x%02x%02x%02x", cMd5[0], cMd5[1], cMd5[2], cMd5[3]);

			m_pSettingDlg->m_pPasswordDlg->SetOldPassWord(cPwd);
			m_pSettingDlg->m_pPasswordDlg->SetHWnd(this->m_hWnd);
            m_pSettingDlg->m_pPasswordDlg->ShowWindow_(SW_SHOW); 

		}
	    break;
	case WM_TEL_KEYCODE:
		////////////zzw10////////////////////////////
		OnEvent(0,0);	
		SendOutEvnet(0, 0);	
		TRACE(L"%x ", wParam);
		
		if(theApp.IsHangON)
		{  
			if (m_pTelephoneDlg->IsWindowVisible() ||\
				m_pMainDlg->m_mainScreenSaveDlg_->IsWindowVisible())
			{
				m_pFSM->fireEvent(teKey, (void * const)wParam);
			}
			else
			{   
				m_pFSM->fireEvent(teKey, (void * const)wParam);
			}
		}
		else
		{   
			if(m_pMainDlg->m_mainScreenSaveDlg_->getIsShowScreenSaveDlg())
			{   
				m_pMainDlg->m_mainScreenSaveDlg_->setIsShowScreenSaveDlg(false);
				return 0;
			}
			else if((m_pContactDlg->IsWindowVisible()) && (currentPanel_==panels_[IDC_BUTTON_CONTACT]))
			{
				m_pContactDlg->SendMessage(WM_TEL_KEYCODE,wParam,0);
			}
			else if((m_pContactInfoDlg->IsWindowVisible()) && (currentPanel_==panels_[IDC_BUTTON_CONTACTINFO]))
			{
				m_pContactInfoDlg->SendMessage(WM_TEL_KEYCODE,wParam,0);
			}
			else if((m_pSoundDlg->IsWindowVisible()) && (currentPanel_==panels_[IDC_BUTTON_SOUND])) 
			{
				m_pSoundDlg->SendMessage(WM_TEL_KEYCODE,wParam,0);	
			}
			else
			{
				m_keyCode += (char)wParam;
				if(m_keyCode == L"*#11557799#")
				{
					m_keyCode.Empty();
					m_bUpgrade = TRUE;
					m_pDeleteTipDlg->SetPasswordModel(false);
					m_pDeleteTipDlg->SetHWnd(this->GetSafeHwnd());
					m_pDeleteTipDlg->SetTitle(Data::LanguageResource::Get(Data::RI_ERROR));
					std::string str1 = Data::LanguageResource::Get(Data::RI_DELETETIP_FORMATPHONE);//是否格式化话机进入升级模式
					m_pDeleteTipDlg->SetDelTip(str1.c_str());
					m_pDeleteTipDlg->ShowWindow_(SW_SHOW);
				}
			}
		}
		break;
	case WM_DELETESELITEM:
		if(m_bUpgrade)
		{
			m_bUpgrade = FALSE;
			SetTimer(33, 100, NULL);
		}
		break;
	case WM_RECORD_FULL:   //提示录音文件太多
		{
			m_pDeleteTipDlg->SetPasswordModel(false);
			m_pDeleteTipDlg->SetHWnd(this->GetSafeHwnd());
			m_pDeleteTipDlg->SetTitle(Data::LanguageResource::Get(Data::RI_ERROR));
			std::string str("");
			extern int g_iRecordCount;
			if(g_iRecordCount >= 30000)
				str = Data::LanguageResource::Get(Data::RI_SYSTEM_SOUND_TIP);
			else
				str = Data::LanguageResource::Get(Data::RI_SYSTEM_DISK);
			m_pDeleteTipDlg->SetDelTip(str.c_str());
			m_pDeleteTipDlg->ShowWindow_(SW_SHOW);
			
		}
		break;
	case WM_RKEY:
		OnEvent(0,0);
		SendOutEvnet(0, 0);
		break;
	case WM_SPEEDDIAL:
		OnEvent(0,0);
		if (!phone_->isBackLight_)
		{
			phone_->OpenBacklight(TRUE);
		}
		else
		{
			if (m_pMainDlg->m_mainScreenSaveDlg_->IsWindowVisible())
			{
				SendOutEvnet(0, 0);
			}

			char c = wParam;
			if(c > 2)
			{
				c = c - 2;
			}

			if (m_pFSM->getCurrentState() == tsHangOn && ISSpeedDial)
			{
				std::map<char, std::string> m = m_pSettingDlg->m_pSetting->speedDials();
				if (m[c] != "")
				{
					m_pTelephoneDlg->DialContact(Data::TelephoneNumber(m[c]));
				}
				ISSpeedDial=FALSE;
			}	
		}
		break;
	case WM_HUNGONINRECORD:
		SendOutEvnet(0, 0);
		m_pTelephoneDlg->HangOnInRecord();
		break;
	case WM_HUNGONTORECORD:
		SendOutEvnet(0, 0);
		m_pTelephoneDlg->HangOnToRecord();
		break;
	case WM_LOCALRECORD:
		OnEvent(0,0);
		if (!phone_->isBackLight_)
		{
			phone_->OpenBacklight(TRUE);
		}
		else if (m_pMainDlg->m_mainScreenSaveDlg_->IsWindowVisible())
		{ 
			if (m_pFSM->getCurrentState() == CMultimediaPhoneDlg::tsConnect)
			{
				if (m_pSettingDlg->m_pSetting->isMustRecord())
				{
					return 0;
				}
				else
				{
					m_pTelephoneDlg->SendMessage(WM_LOCALRECORD, 0, 0);
				}	
			}
			SendOutEvnet(0, 0);	
		}
		else 
		{	
			if (m_pDeleteTipDlg->IsWindowVisible())
			{
				return 0;
			}
			else if (m_pContactDlg->m_pContactSearchDlg->IsWindowVisible()||\
				m_pContactInfoDlg->m_pSearchContactInfoDlg->IsWindowVisible()||\
				m_pSoundDlg->m_pSoundSearchDlg->IsWindowVisible()||\
				m_pSoundDlg->m_pDetailTip->IsWindowVisible()||\
				m_pContactInfoDlg->m_pDetailTip->IsWindowVisible()||\
				m_pSoundDlg->m_psoundTipDlg->IsWindowVisible())
			{
				return 0;
			}
			else if (m_pSettingDlg->m_pPasswordDlg->IsWindowVisible())
			{
				return 0;
			}
			else if (m_pSoundDlg->m_pPasswordDlg->IsWindowVisible())
			{
				return 0;
			}
			else if (m_pContactInfoDlg->m_pPasswordDlg->IsWindowVisible())
			{
				return 0;
			}
			else if(m_pSettingDlg->m_pFastDialsDlg->IsWindowVisible())
			{
				return 0;
			}									
			if (m_pFSM->getCurrentState() == CMultimediaPhoneDlg::tsHangOff) //不在通话中
			{
				if (m_pSoundDlg->m_pPlaySoundDlg->IsWindowVisible())//如果有播放器
				{
					return 0;             
				}
				if (m_pContactInfoDlg->m_pPlaySoundDlg->IsWindowVisible())
				{
					return 0;
				}

				if (m_pSoundDlg->m_pRecordSoundDlg->IsWindowVisible())
				{
					m_pSoundDlg->m_pRecordSoundDlg->SendMessage(WM_LOCALRECORD,0,0);
				}
				else
				{
					m_pSoundDlg->SendMessage(WM_LOCALRECORD,0,0);
				}	
			}
			else if (m_pFSM->getCurrentState() == CMultimediaPhoneDlg::tsHangOn ||
				m_pFSM->getCurrentState() == CMultimediaPhoneDlg::tsKey ||
				m_pFSM->getCurrentState() == CMultimediaPhoneDlg::tsConnect)
			{	
				//change by qi 20111104
				//录音键开始录音
				if (m_pSettingDlg->m_pSetting->isMustRecord())
				{
                     return 0;
				}
				else
				{
                    m_pTelephoneDlg->SendMessage(WM_LOCALRECORD, 0, 0);
				}
					
			}
		}
		break;
	case WM_PLAYLEAVEWORD:
		OnEvent(0,0);
		if (!phone_->isBackLight_)
		{
			phone_->OpenBacklight(TRUE);
		}
		else if (m_pMainDlg->m_mainScreenSaveDlg_->IsWindowVisible())
		{
			SendOutEvnet(0, 0);
		}
		else
		{   
			if (m_pTelephoneDlg->IsWindowVisible())
			{
				return 0;
			}
			
			if (m_pDeleteTipDlg->IsWindowVisible())
			{
				return 0;
			}
			else if (m_pContactDlg->m_pContactSearchDlg->IsWindowVisible()||m_pContactInfoDlg->m_pSearchContactInfoDlg->IsWindowVisible()||m_pSoundDlg->m_pSoundSearchDlg->IsWindowVisible()||m_pSoundDlg->m_pDetailTip->IsWindowVisible()||m_pContactInfoDlg->m_pDetailTip->IsWindowVisible()||m_pSoundDlg->m_psoundTipDlg->IsWindowVisible())
			{
				return 0;
			}
			else if (m_pSettingDlg->m_pPasswordDlg->IsWindowVisible())
			{
				return 0;
			}
			else if (m_pSoundDlg->m_pPasswordDlg->IsWindowVisible())
			{
				return 0;
			}
			else if (m_pContactInfoDlg->m_pPasswordDlg->IsWindowVisible())
			{
				return 0;
			}
			else if(m_pSettingDlg->m_pFastDialsDlg->IsWindowVisible())
			{
				return 0;
			}
			
			if (m_pFSM->getCurrentState() == CMultimediaPhoneDlg::tsHangOff)//非通话状态
			{	
				if (m_pSoundDlg->m_pRecordSoundDlg->IsWindowVisible())
				{
					m_pSoundDlg->m_pRecordSoundDlg->PostMessage(WM_PLAYLEAVEWORD,0,0); 
				}
				else
				{
					if (m_pSoundDlg->m_pPlaySoundDlg->IsWindowVisible())//如果有播放器
					{
						m_pSoundDlg->m_pPlaySoundDlg->PostMessage(WM_PLAYLEAVEWORD,0,0);  
					}
					else if(m_pContactInfoDlg->m_pPlaySoundDlg->IsWindowVisible())
					{
						m_pContactInfoDlg->m_pPlaySoundDlg->PostMessage(WM_PLAYLEAVEWORD,0,0);
					}
					else//无播放器
					{
						if (m_pContactInfoDlg->IsWindowVisible())
						{
							m_pContactInfoDlg->PostMessage(WM_PLAYLEAVEWORD, 0, 0);		
						}
						if (m_pSoundDlg->IsWindowVisible())
						{	
							m_pSoundDlg->PostMessage(WM_PLAYLEAVEWORD, 0, 0);	
						}
						if(m_pMainDlg->IsWindowVisible()||m_pSettingDlg->IsWindowVisible()||m_pContactDlg->IsWindowVisible()||m_pInformationdlg->IsWindowVisible())
						{
							std::vector<boost::shared_ptr<Data::SoundSegment> > result = \
								Data::SoundSegment::GetFromDatabase("type = 0 AND played = 0", Data::dUp, 0, 1);//搜索未播放留言
							if (result.size() > 0)//有留言
							{
								m_pSoundDlg->m_pPlaySoundDlg->SetSound(0, result[0]->id(), 0, m_sListSearchFilter);
								if (m_pSettingDlg->m_pSetting->isPlayProtect() &&\
									!m_pSettingDlg->m_bLogin)
								{	
									//播放录音的密码
									std::string strTemp = m_pSettingDlg->m_pSetting->playRecordPassword();
									if (!strTemp.empty())
									{
										m_pSoundDlg->m_pPlaySoundDlg->setonlynew(TRUE);
										m_pSoundDlg->m_pPasswordDlg->SetType(CHECK_PASSWORD);
										m_pSoundDlg->m_pPasswordDlg->SetOldPassWord((char *)strTemp.c_str());
										m_pSoundDlg->m_pPasswordDlg->SetHWnd(m_pSoundDlg->m_hWnd);
										m_pSoundDlg->m_pPasswordDlg->ShowWindow(SW_SHOW);	
									}
									else
									{	
										//密码为空，直接播放
										m_pSoundDlg->m_pPlaySoundDlg->SetAutoClose(TRUE);
										m_pSoundDlg->m_pPlaySoundDlg->ShowWindow(TRUE);
										m_pSoundDlg->m_pPlaySoundDlg->StartPlay(TRUE);

									}
									
								}
								else
								{
									m_pSoundDlg->m_pPlaySoundDlg->SetAutoClose(TRUE);
									m_pSoundDlg->m_pPlaySoundDlg->ShowWindow(TRUE);
									m_pSoundDlg->m_pPlaySoundDlg->StartPlay(TRUE);

								}
							}
							else if(m_pContactNewDlg->IsWindowVisible())
							{
						    	return 0;
							}
							else
							{   
								SwitchPanel_(IDC_BUTTON_SOUND);
							}
						}
					}
				}
			}	
			if (m_pFSM->getCurrentState() == CMultimediaPhoneDlg::tsConnect ||
				m_pFSM->getCurrentState() == CMultimediaPhoneDlg::tsRing)
			{
				
				//defect ****1
				if (m_pTelephoneDlg->IsWindowVisible())
				{
					return 0;

				}else
				{
					if (m_pSoundDlg->m_pPlaySoundDlg->IsWindowVisible())//如果有播放器
					{
						m_pSoundDlg->m_pPlaySoundDlg->PostMessage(WM_PLAYLEAVEWORD,0,0);  
					}
                    else if (m_pContactInfoDlg->m_pPlaySoundDlg->IsWindowVisible())
                    {
                        m_pContactInfoDlg->m_pPlaySoundDlg->PostMessage(WM_PLAYLEAVEWORD,0,0);  

                    }
					else if (m_pContactInfoDlg->IsWindowVisible())
					{
						m_pContactInfoDlg->PostMessage(WM_PLAYLEAVEWORD, 0, 0);								
					}
						
					else if (m_pSoundDlg->IsWindowVisible())
					{
							
						m_pSoundDlg->PostMessage(WM_PLAYLEAVEWORD, 0, 0);
							
					}
					else if(m_pMainDlg->IsWindowVisible()||m_pSettingDlg->IsWindowVisible()||m_pContactDlg->IsWindowVisible()||m_pInformationdlg->IsWindowVisible())
					{ 
					  SwitchPanel_(IDC_BUTTON_SOUND);
					}
					//=======================gjj	
				}
			}
		}
		break;

case WM_NEWCONTACT:
	OnEvent(0,0);
	if (!phone_->isBackLight_)
	{
		phone_->OpenBacklight(TRUE);
	}
	else if (m_pMainDlg->m_mainScreenSaveDlg_->IsWindowVisible())
	{
		SendOutEvnet(0, 0);
	}
	else
	{
		if (m_pTelephoneDlg->IsWindowVisible())
		{
			return 0;
		}
		if (m_pDeleteTipDlg->IsWindowVisible())
		{
			return 0;
		}
		else if(
					m_pContactDlg->m_pContactSearchDlg->IsWindowVisible()    ||
					m_pContactInfoDlg->m_pSearchContactInfoDlg->IsWindowVisible() ||
					m_pSoundDlg->m_pSoundSearchDlg->IsWindowVisible()||
					m_pContactInfoDlg->m_pDetailTip->IsWindowVisible()||
					m_pSoundDlg->m_pDetailTip->IsWindowVisible()||
					m_pSoundDlg->m_psoundTipDlg->IsWindowVisible()
				)
		{
			return 0;
		}
		else if(m_pSoundDlg->m_pPlaySoundDlg->IsWindowVisible())
		{
			return 0;
		}
		else if (m_pContactInfoDlg->m_pPlaySoundDlg->IsWindowVisible())
		{
			return 0;
		}
		else if (m_pSoundDlg->m_pRecordSoundDlg->IsWindowVisible())
		{
			return 0;
		}
		else if (m_pSettingDlg->m_pPasswordDlg->IsWindowVisible())
		{
			return 0;
		}
		else if (m_pSoundDlg->m_pPasswordDlg->IsWindowVisible())
		{
			return 0;
		}
		else if (m_pContactInfoDlg->m_pPasswordDlg->IsWindowVisible())
		{
			return 0;
		}
		else if(m_pSettingDlg->m_pFastDialsDlg->IsWindowVisible())
		{
			return 0;
		}
		//defect 141
		else if(m_pTelephoneDlg->IsWindowVisible())
		{
			return 0;
		}
		//=======================defect 141
		//桌面，配置，便签界面 按名片硬键 可进入通讯录界面
		else if(m_pMainDlg->IsWindowVisible()  ||
				m_pSettingDlg->IsWindowVisible() ||
				m_pInformationdlg->IsWindowVisible() )		
		{
			SwitchPanel_(IDC_BUTTON_CONTACT);

		}else if (m_pContactInfoDlg->IsWindowVisible())
		{
			m_pContactInfoDlg->PostMessage(WM_KEYNEWCONTACT,0,0);
		}
		else if (m_pSoundDlg->IsWindowVisible())
		{
			m_pSoundDlg->PostMessage(WM_KEYNEWCONTACT,0,0);
		}
		else if (m_pInformationdlg->IsWindowVisible())
		{
			m_pInformationdlg->PostMessage(WM_NEWCONTACT,0,0);
		}
		
	}
	break;
case WM_KEYDOWN:
	OnEvent(0,0);
	if (!phone_->isBackLight_)
	{
		phone_->OpenBacklight(TRUE);
	}
	else if (m_pMainDlg->m_mainScreenSaveDlg_->IsWindowVisible())
	{
		SendOutEvnet(0, 0);
	}
	
	else {
		if (m_pTelephoneDlg->IsWindowVisible())
		{
			return 0;
		
		}
		if (wParam == 'U')
		{        
			if (m_pSoundDlg->m_pPlaySoundDlg->IsWindowVisible())
			{
				m_pSoundDlg->m_pPlaySoundDlg->SendMessage(WM_KEYDOWN,'U',0);
			}
			else if(m_pContactInfoDlg->m_pPlaySoundDlg->IsWindowVisible())
			{
				m_pContactInfoDlg->m_pPlaySoundDlg->SendMessage(WM_KEYDOWN,'U',0);
			}
			else if (m_pSoundDlg->m_pRecordSoundDlg->IsWindowVisible())
			{
				return 0;
			}
			else if (m_pDeleteTipDlg->IsWindowVisible())
			{
				return 0;
			}
			else if (m_pContactDlg->m_pContactSearchDlg->IsWindowVisible()||m_pContactInfoDlg->m_pSearchContactInfoDlg->IsWindowVisible()||m_pSoundDlg->m_pSoundSearchDlg->IsWindowVisible()||m_pSoundDlg->m_pDetailTip->IsWindowVisible()||m_pContactInfoDlg->m_pDetailTip->IsWindowVisible()||m_pSoundDlg->m_psoundTipDlg->IsWindowVisible())
			{
				return 0;
			}
			else if(m_pSoundDlg->m_pPlaySoundDlg->IsWindowVisible())
			{
				return 0;
			}
			else if (m_pContactInfoDlg->m_pPlaySoundDlg->IsWindowVisible())
			{
				return 0;
			}
			else if (m_pSoundDlg->m_pRecordSoundDlg->IsWindowVisible())
			{
				return 0;
			}
			else if (m_pSettingDlg->m_pPasswordDlg->IsWindowVisible())
			{
				return 0;
			}
			else if (m_pSoundDlg->m_pPasswordDlg->IsWindowVisible())
			{
				return 0;
			}
			else if (m_pContactInfoDlg->m_pPasswordDlg->IsWindowVisible())
			{
				return 0;
			}
			else if(m_pSettingDlg->m_pFastDialsDlg->IsWindowVisible())
			{
				return 0;
			}
			else
			{
				if (m_pContactDlg->IsWindowVisible())
				{
					m_pContactDlg->SendMessage(WM_KEYDOWN,'U',0);
				}
				if (m_pContactInfoDlg->IsWindowVisible())
				{
					m_pContactInfoDlg->SendMessage(WM_KEYDOWN,'U',0);
				}
				if (m_pSoundDlg->IsWindowVisible())
				{
					m_pSoundDlg->SendMessage(WM_KEYDOWN,'U',0);
				}
				
				if (m_pMainDlg->m_mainLunarderDlg_->IsWindowVisible())
				{
					m_pMainDlg->m_mainLunarderDlg_->SendMessage(WM_KEYDOWN,'U',0);					  
				}
				if (m_pInformationdlg->IsWindowVisible())
				{
					m_pInformationdlg->SendMessage(WM_KEYDOWN,'U',0);
				}
			}
			
		}
		
		if(wParam == 'D')
		{
			if (m_pSoundDlg->m_pPlaySoundDlg->IsWindowVisible())
			{
				m_pSoundDlg->m_pPlaySoundDlg->SendMessage(WM_KEYDOWN,'D',0);
			}
			else if(m_pContactInfoDlg->m_pPlaySoundDlg->IsWindowVisible())
			{
				m_pContactInfoDlg->m_pPlaySoundDlg->SendMessage(WM_KEYDOWN,'D',0);
			}
			else if (m_pSoundDlg->m_pRecordSoundDlg->IsWindowVisible())
			{
				return 0;
			}
			else if (m_pDeleteTipDlg->IsWindowVisible())
			{
				return 0;
			}
			else if (m_pContactDlg->m_pContactSearchDlg->IsWindowVisible()||m_pContactInfoDlg->m_pSearchContactInfoDlg->IsWindowVisible()||m_pSoundDlg->m_pSoundSearchDlg->IsWindowVisible()||m_pSoundDlg->m_pDetailTip->IsWindowVisible()||m_pContactInfoDlg->m_pDetailTip->IsWindowVisible()||m_pSoundDlg->m_psoundTipDlg->IsWindowVisible())
			{
				return 0;
			}
			else if(m_pSoundDlg->m_pPlaySoundDlg->IsWindowVisible())
			{
				return 0;
			}
			else if (m_pContactInfoDlg->m_pPlaySoundDlg->IsWindowVisible())
			{
				return 0;
			}
			else if (m_pSoundDlg->m_pRecordSoundDlg->IsWindowVisible())
			{
				return 0;
			}
			else if (m_pSettingDlg->m_pPasswordDlg->IsWindowVisible())
			{
				return 0;
			}
			else if (m_pSoundDlg->m_pPasswordDlg->IsWindowVisible())
			{
				return 0;
			}
			else if (m_pContactInfoDlg->m_pPasswordDlg->IsWindowVisible())
			{
				return 0;
			}
			else if(m_pSettingDlg->m_pFastDialsDlg->IsWindowVisible())
			{
				return 0;
			}
			else
			{
				if (m_pContactDlg->IsWindowVisible())
				{
					m_pContactDlg->SendMessage(WM_KEYDOWN,'D',0);
				}
				if (m_pContactInfoDlg->IsWindowVisible())
				{
					m_pContactInfoDlg->SendMessage(WM_KEYDOWN,'D',0);
				}
				if (m_pSoundDlg->IsWindowVisible())
				{
					m_pSoundDlg->SendMessage(WM_KEYDOWN,'D',0);
				}				 				 
				if (m_pMainDlg->m_mainLunarderDlg_->IsWindowVisible())
				{
					m_pMainDlg->m_mainLunarderDlg_->SendMessage(WM_KEYDOWN,'D',0);					  
				}
				if (m_pInformationdlg->IsWindowVisible())
				{
					m_pInformationdlg->SendMessage(WM_KEYDOWN,'D',0);
				}
			}
		}
	}
	break;
case WM_BTLEFT:
	OnEvent(0,0);
	if (!phone_->isBackLight_)
	{
		phone_->OpenBacklight(TRUE);
	}
	else if (m_pMainDlg->m_mainScreenSaveDlg_->IsWindowVisible())
	{ 
		//defect 173
		//defect 170
    	  SendOutEvnet(0, 0);
        //*****************************
	}
	else if (m_pMainDlg->m_firewalDlg_->IsWindowVisible())
	{	
		//change by qi 20111104
		//在防火墙界面，按键无效
		return 0;
	}
	else
	{		
		//defect 114   Left Key
		if ((((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pFSM->getCurrentState() == CMultimediaPhoneDlg::tsHangOff)  ||
			(((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pFSM->getCurrentState() == CMultimediaPhoneDlg::tsConnect) ||
			(((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pFSM->getCurrentState() == CMultimediaPhoneDlg::tsRing) ) //未通话 和 通话
		{ 
		   
			if(m_pContactDlg->m_pContactSearchDlg->IsWindowVisible())
			{
				return 0;
			}
			else if (m_pContactInfoDlg->m_pSearchContactInfoDlg->IsWindowVisible())
			{
				return 0;
			}
			else if (m_pSoundDlg->m_pSoundSearchDlg->IsWindowVisible())
			{
				return 0;
			}
			else if (m_pSettingDlg->m_pPasswordDlg->IsWindowVisible())
			{
				return 0;
			}
			else if (m_pSoundDlg->m_pPasswordDlg->IsWindowVisible())
			{
				return 0;
			}else if (m_pSoundDlg->m_psoundTipDlg->IsWindowVisible())
			{
				return 0;
			}
			else if (m_pContactInfoDlg->m_pPasswordDlg->IsWindowVisible())
			{
				return 0;
			}
			else if (m_pContactInfoDlg->m_pDetailTip->IsWindowVisible())
			{
				return 0;
			}
			else if (m_pDeleteTipDlg->IsWindowVisible())
			{
				return 0;
			}
			else if (m_pSoundDlg->m_pDetailTip->IsWindowVisible())
			{
			   return 0;
			}
			else if(m_pSettingDlg->m_pFastDialsDlg->IsWindowVisible())
			{
				return 0;
			}
			else if ( m_pSoundDlg->m_pPlaySoundDlg->IsWindowVisible())
			{
				m_pSoundDlg->m_pPlaySoundDlg->SendMessage(WM_BTLEFT,0,0);

			}else if (m_pSoundDlg->m_pRecordSoundDlg->IsWindowVisible())
			{
				m_pSoundDlg->m_pRecordSoundDlg->SendMessage(WM_BTLEFT,0,0);

			}else if(m_pContactInfoDlg->m_pPlaySoundDlg->IsWindowVisible())
			{
				m_pContactInfoDlg->m_pPlaySoundDlg->SendMessage(WM_BTLEFT,0,0);
			}
			else if (m_pTelephoneDlg->IsWindowVisible())
			{
				return 0;
			}
			else 
			{
				panelToLeft();
			}	
		}
		else //if((CMultimediaPhoneDlg*)theApp.m_pMainWnd->m_pFSM->getCurrentState() == CMultimediaPhoneDlg::tsConnect)
		{
			if (!m_pTelephoneDlg->IsWindowVisible())//
			{
				panelToLeft();
			}
		}	
	}		
	break;
case WM_BTRIGHT:
	OnEvent(0,0);
	if (!phone_->isBackLight_)
	{
		phone_->OpenBacklight(TRUE);
	}
	else if (m_pMainDlg->m_mainScreenSaveDlg_->IsWindowVisible())
	{
		SendOutEvnet(0, 0);
	}
	else if (m_pMainDlg->m_firewalDlg_->IsWindowVisible())
	{	
		//change by qi 20111104
		//在防火墙界面，按键无效
		return 0;
	}
	else
	{
		if ((((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pFSM->getCurrentState() == CMultimediaPhoneDlg::tsHangOff)  ||
			(((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pFSM->getCurrentState() == CMultimediaPhoneDlg::tsConnect)  ||
			(((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pFSM->getCurrentState() == CMultimediaPhoneDlg::tsRing)) //未通话 和 通话
		{ 
			if(m_pContactDlg->m_pContactSearchDlg->IsWindowVisible())
			{
				return 0;
			}
			else if (m_pContactInfoDlg->m_pSearchContactInfoDlg->IsWindowVisible())
			{
				return 0;
			}
			else if (m_pSoundDlg->m_pSoundSearchDlg->IsWindowVisible())
			{
				return 0;
			}
			else if (m_pSettingDlg->m_pPasswordDlg->IsWindowVisible())
			{
				return 0;
			}
			else if (m_pSoundDlg->m_pPasswordDlg->IsWindowVisible())
			{
				return 0;
			}else if (m_pSoundDlg->m_pDetailTip->IsWindowVisible())
			{
				return 0;
			}else if (m_pSoundDlg->m_psoundTipDlg->IsWindowVisible())
			{
				return 0;
			}
			else if (m_pContactInfoDlg->m_pPasswordDlg->IsWindowVisible())
			{
				return 0;
			}else if (m_pContactInfoDlg->m_pDetailTip->IsWindowVisible())
			{
				return 0;
			}
			else if (m_pDeleteTipDlg->IsWindowVisible())
			{
				return 0;
			}
			else if(m_pSettingDlg->m_pFastDialsDlg->IsWindowVisible())
			{
				return 0;
			}
			else if (m_pSoundDlg->m_pPlaySoundDlg->IsWindowVisible())
			{
				m_pSoundDlg->m_pPlaySoundDlg->SendMessage(WM_BTRIGHT,0,0);

			}else if (m_pSoundDlg->m_pRecordSoundDlg->IsWindowVisible())
			{
				m_pSoundDlg->m_pRecordSoundDlg->SendMessage(WM_BTRIGHT,0,0);
			}
			else if(m_pContactInfoDlg->m_pPlaySoundDlg->IsWindowVisible())
			{
				m_pContactInfoDlg->m_pPlaySoundDlg->SendMessage(WM_BTRIGHT,0,0);
			}
			else if (m_pTelephoneDlg->IsWindowVisible())
			{
				return 0;
			}else 
			{
				panelToRight();
			}
			
		}else
		{
			if (!m_pTelephoneDlg->IsWindowVisible())
			{
				panelToRight();
			}
		}
	}

	break;

case WM_OK:
	OnEvent(0,0);

	if (m_pTelephoneDlg->IsWindowVisible())
	{
		if(m_pDeleteTipDlg->IsWindowVisible())   //wzx 20101227
		{
			m_pDeleteTipDlg->SendMessage(WM_OK,0,0);
		}
		return 0;
	}

	if (!phone_->isBackLight_)
	{
	  phone_->OpenBacklight(TRUE);
	}
	else if (m_pMainDlg->m_mainScreenSaveDlg_->IsWindowVisible())
	{
		SendOutEvnet(0, 0);
	}
	else
	{
		if (m_pContactDlg->m_pContactSearchDlg->IsWindowVisible())
		{
			m_pContactDlg->m_pContactSearchDlg->SendMessage(WM_OK,0,0);
		}
		else if (m_pContactInfoDlg->m_pSearchContactInfoDlg->IsWindowVisible())
		{
			m_pContactInfoDlg->m_pSearchContactInfoDlg->SendMessage(WM_OK,0,0);
		}
		else if( m_pSoundDlg->m_pSoundSearchDlg->IsWindowVisible())
		{
			m_pSoundDlg->m_pSoundSearchDlg->SendMessage(WM_OK,0,0);
		}
		else if(m_pDeleteTipDlg->IsWindowVisible())
		{
			m_pDeleteTipDlg->SendMessage(WM_OK,0,0);
		}
		else if(m_pInformationdlg->IsWindowVisible())
		{
			m_pInformationdlg->SendMessage(WM_OK);
		}
		else if(m_pSoundDlg->m_pPasswordDlg->IsWindowVisible())
		{
			//关闭输入法
			KeyBoardOff();
			m_pSoundDlg->m_pPasswordDlg->PostMessage(WM_OK);
		}
		else if(m_pSettingDlg->m_pPasswordDlg->IsWindowVisible())
		{
			//关闭输入法
			KeyBoardOff();

			m_pSettingDlg->m_pPasswordDlg->SendMessage(WM_OK);
		}
		else if(m_pContactInfoDlg->m_pPasswordDlg->IsWindowVisible())
		{
			if(theApp.inputDlg_->IsWindowVisible())
			{
				theApp.inputDlg_->hide();
			}

			if (theApp.handWriteDlg_->IsWindowVisible())
			{
				theApp.handWriteDlg_->hide();
			}
			//隐藏
			SipShowIM(SIPF_OFF);
			m_pContactInfoDlg->m_pPasswordDlg->SendMessage(WM_OK);

		}else if (m_pContactInfoDlg->m_pDetailTip->IsWindowVisible())
		{
			m_pContactInfoDlg->m_pDetailTip->SendMessage(WM_OK,0,0);
		}
		else if (m_pContactGroupDlg->IsWindowVisible() || m_pContactNewDlg->IsWindowVisible() )
		{
			m_pContactGroupDlg->SendMessage(WM_OK,0,0);

			m_pContactNewDlg->SendMessage(WM_OK,0,0);
		}
		else if ( m_pContactNewDlg->IsWindowVisible())
		{
			m_pContactNewDlg->SendMessage(WM_OK,0,0);
		}
		else if (m_pContactDlg->IsWindowVisible())
		{
			m_pContactDlg->SendMessage(WM_OK,0,0);
		}
		else if (m_pContactInfoDlg->IsWindowVisible())
		{
			m_pContactInfoDlg->SendMessage(WM_OK,0,0);
		}
		else if(m_pSettingDlg->m_pFastDialsDlg->IsWindowVisible())
		{
			m_pSettingDlg->m_pFastDialsDlg->SendMessage(WM_OK,0,0);
		}
		else if(m_pSettingDlg->IsWindowVisible())
		{
			m_pSettingDlg->SendMessage(WM_OK,0,0);
		}
		else if(m_pMainDlg->m_firewalDlg_->IsWindowVisible())
		{
			m_pMainDlg->m_firewalDlg_->SendMessage(WM_OK,0,0);
		}
		else if(m_pMainDlg->m_mainLunarderDlg_->IsWindowVisible())
		{
			m_pMainDlg->m_mainLunarderDlg_->SendMessage(WM_OK,0,0);

		}else if (m_pSoundDlg->m_pDetailTip->IsWindowVisible())
		{
            m_pSoundDlg->m_pDetailTip->SendMessage(WM_OK,0,0);

		}else if (m_pSoundDlg->m_psoundTipDlg->IsWindowVisible())
		{  
			//关闭输入法
			KeyBoardOff();
			
			m_pSoundDlg->m_psoundTipDlg->SendMessage(WM_OK);
		}	
	}
	break;
case WM_CANCEL:
	OnEvent(0,0);
	 if (!phone_->isBackLight_)
	 {
		 phone_->OpenBacklight(TRUE);
		 
	 }
	 else if(m_pMainDlg->m_mainScreenSaveDlg_->IsWindowVisible())
	 {
		 SendOutEvnet(0, 0);
	 }
	 else
	 {
		 HideComboBoxDropDown();  //wzx20101227
         if (m_pFSM->getCurrentState() == CMultimediaPhoneDlg::tsConnect ||
			 m_pFSM->getCurrentState()==CMultimediaPhoneDlg::tsKey||
			 m_pFSM->getCurrentState()==CMultimediaPhoneDlg::tsHangOn||
			 m_pFSM->getCurrentState() == CMultimediaPhoneDlg::tsRing)
         {  
			 ///////zzw10827//////////////////////////////////////////
			 if (m_pTelephoneDlg->IsWindowVisible())
			 {
				 if(m_pDeleteTipDlg->IsWindowVisible())
				 {
					 m_pDeleteTipDlg->SendMessage(WM_CANCEL,0,0);
				 }
				 return TRUE;
			 }
			 else if (m_pSoundDlg->m_pPlaySoundDlg->IsWindowVisible())
			 {
				 m_pSoundDlg->m_pPlaySoundDlg->SendMessage(WM_CANCEL,0,0);
			 }
			 else if (m_pContactInfoDlg->m_pPlaySoundDlg->IsWindowVisible())
			 {
				 m_pContactInfoDlg->m_pPlaySoundDlg->SendMessage(WM_CANCEL,0,0);
			 }
			 else if (m_pContactInfoDlg->m_pDetailTip->IsWindowVisible())
			 {   
				 m_pContactInfoDlg->m_pDetailTip->SendMessage(WM_CANCEL,0,0);
				 
			 }else if (m_pSoundDlg->m_psoundTipDlg->IsWindowVisible())
			 {
				 m_pSoundDlg->m_psoundTipDlg->SendMessage(WM_CANCEL,0,0);
			 }
			 else if (m_pSoundDlg->m_pRecordSoundDlg->IsWindowVisible())
			 {
				 m_pSoundDlg->m_pRecordSoundDlg->SendMessage(WM_CANCEL,0,0);
			 }
			 else if(m_pSoundDlg->m_pDetailTip->IsWindowVisible())
			 {
				 m_pSoundDlg->m_pDetailTip->SendMessage(WM_CANCEL,0,0);
			 }
			 else if (m_pInformationdlg->IsWindowVisible())
			 {
				 m_pInformationdlg->SendMessage(WM_CANCEL,0,0);
			 }
			 else if (m_pSettingDlg->m_pFastDialsDlg->IsWindowVisible())
			 {
				 m_pSettingDlg->m_pFastDialsDlg->SendMessage(WM_CANCEL,0,0);
			 }
			 else if(m_pContactDlg->m_pContactSearchDlg->IsWindowVisible())
			 {
				 m_pContactDlg->m_pContactSearchDlg->SendMessage(WM_CANCEL,0,0);
			 }
			 else if (m_pContactInfoDlg->m_pSearchContactInfoDlg->IsWindowVisible())
			 {
				 m_pContactInfoDlg->m_pSearchContactInfoDlg->SendMessage(WM_CANCEL,0,0);
			 }
			 else if (m_pSoundDlg->m_pSoundSearchDlg->IsWindowVisible())
			 {
				 m_pSoundDlg->m_pSoundSearchDlg->SendMessage(WM_CANCEL,0,0);
			 }
			 else if(m_pDeleteTipDlg->IsWindowVisible())
			 {
				 m_pDeleteTipDlg->SendMessage(WM_CANCEL,0,0);
			 }
			 else if (m_pContactNewDlg->IsWindowVisible())
			 {
				 m_pContactNewDlg->SendMessage(WM_CANCEL,0,0);
			 }
			 else if(m_pContactGroupDlg->IsWindowVisible())
			 {
				 m_pContactGroupDlg->SendMessage(WM_CANCEL,0,0);
			 }
			 else if (m_pSettingDlg->m_pPasswordDlg->IsWindowVisible())
			 {
				 m_pSettingDlg->m_pPasswordDlg->SendMessage(WM_CANCEL,0,0);
			 }
			 else if (m_pSettingDlg->IsWindowVisible())
			 {
				 m_pSettingDlg->SendMessage(WM_CANCEL,0,0);
			 }
			 else if (m_pMainDlg->m_firewalDlg_->IsWindowVisible()&&currentPanel_==panels_[IDC_BUTTON_MAIN])
			 {
				 m_pMainDlg->m_firewalDlg_->SendMessage(WM_CANCEL,0,0);
				 
			 }else if (m_pInformationdlg->IsWindowVisible())
			 {
				 m_pInformationdlg->SendMessage(WM_CANCEL,0,0);
			 }
			 else if (m_pMainDlg->m_mainLunarderDlg_->IsWindowVisible()&&currentPanel_==panels_[IDC_BUTTON_MAIN])
			 {
				 m_pMainDlg->m_mainLunarderDlg_->SendMessage(WM_CANCEL,0,0);
			 }
			 //////=============================================================
			 //&&currentPanel_==panels_[IDC_BUTTON_MAIN]
			 else if (m_pMainDlg->m_mainCalucaterDlg_->IsWindowVisible()&&currentPanel_==panels_[IDC_BUTTON_MAIN])
			 {
				 m_pMainDlg->m_mainCalucaterDlg_->SendMessage(WM_CANCEL,0,0);			
			 }
			 else if (m_pSoundDlg->m_pPasswordDlg->IsWindowVisible()&&IsWindowEnabled())
			 {
				 m_pSoundDlg->m_pPasswordDlg->SendMessage(WM_CANCEL);
			 }
			 else if (m_pSoundDlg->m_psoundTipDlg->IsWindowVisible())
			 {
				 m_pSoundDlg->m_psoundTipDlg->SendMessage(WM_CANCEL);
			 }
			 else if (m_pContactInfoDlg->m_pPasswordDlg->IsWindowVisible())
			 {
				 m_pContactInfoDlg->m_pPasswordDlg->SendMessage(WM_CANCEL);
				 
			 }else if (m_pSoundDlg->IsWindowVisible())
			 {
				 m_pSoundDlg->SendMessage(WM_CANCEL,0,0);
				 
			 }else if (m_pContactInfoDlg->IsWindowVisible())
			 {
				 m_pContactInfoDlg->SendMessage(WM_CANCEL,0,0);
				 
			 }else if(m_pContactDlg->IsWindowVisible())
			 {
				 m_pContactDlg->SendMessage(WM_CANCEL,0,0);
				 
			 }else{
				 
				 m_pTelephoneDlg->ShowWindow(TRUE);
			 }
         }
		 else if (m_pFSM->getCurrentState() == CMultimediaPhoneDlg::tsHangOff)
		 {
			 if (m_pSoundDlg->m_pPlaySoundDlg->IsWindowVisible())
			 {
				 m_pSoundDlg->m_pPlaySoundDlg->SendMessage(WM_CANCEL,0,0);
			 }
			 else if (m_pContactInfoDlg->m_pPlaySoundDlg->IsWindowVisible())
			 {
				 m_pContactInfoDlg->m_pPlaySoundDlg->SendMessage(WM_CANCEL,0,0);
			 }
			 else if (m_pContactInfoDlg->m_pDetailTip->IsWindowVisible())
			 {   
				 m_pContactInfoDlg->m_pDetailTip->SendMessage(WM_CANCEL,0,0);
				 
			 }else if (m_pSoundDlg->m_psoundTipDlg->IsWindowVisible())
			 {
				 m_pSoundDlg->m_psoundTipDlg->SendMessage(WM_CANCEL,0,0);
			 }
			 else if (m_pSoundDlg->m_pRecordSoundDlg->IsWindowVisible())
			 {
				 m_pSoundDlg->m_pRecordSoundDlg->SendMessage(WM_CANCEL,0,0);
			 }
			 else if(m_pSoundDlg->m_pDetailTip->IsWindowVisible())
			 {
				 m_pSoundDlg->m_pDetailTip->SendMessage(WM_CANCEL,0,0);
			 }
			 else if (m_pInformationdlg->IsWindowVisible())
			 {
				 m_pInformationdlg->SendMessage(WM_CANCEL,0,0);
			 }
			 else if (m_pSettingDlg->m_pFastDialsDlg->IsWindowVisible())
			 {
				 m_pSettingDlg->m_pFastDialsDlg->SendMessage(WM_CANCEL,0,0);
			 }
			 else if(m_pContactDlg->m_pContactSearchDlg->IsWindowVisible())
			 {
				 m_pContactDlg->m_pContactSearchDlg->SendMessage(WM_CANCEL,0,0);
			 }
			 else if (m_pContactInfoDlg->m_pSearchContactInfoDlg->IsWindowVisible())
			 {
				 m_pContactInfoDlg->m_pSearchContactInfoDlg->SendMessage(WM_CANCEL,0,0);
			 }
			 else if (m_pSoundDlg->m_pSoundSearchDlg->IsWindowVisible())
			 {
				 m_pSoundDlg->m_pSoundSearchDlg->SendMessage(WM_CANCEL,0,0);
			 }
			 else if(m_pDeleteTipDlg->IsWindowVisible())
			 {
				 m_pDeleteTipDlg->SendMessage(WM_CANCEL,0,0);
			 }
			 else if (m_pContactNewDlg->IsWindowVisible())
			 {
				 m_pContactNewDlg->SendMessage(WM_CANCEL,0,0);
			 }
			 else if(m_pContactGroupDlg->IsWindowVisible())
			 {
				 m_pContactGroupDlg->SendMessage(WM_CANCEL,0,0);
			 }
			 else if (m_pSettingDlg->m_pPasswordDlg->IsWindowVisible())
			 {
				 m_pSettingDlg->m_pPasswordDlg->SendMessage(WM_CANCEL,0,0);
			 }
			 else if (m_pSoundDlg->m_pPasswordDlg->IsWindowVisible()&&IsWindowEnabled())
			 {
				 m_pSoundDlg->m_pPasswordDlg->SendMessage(WM_CANCEL);
			 }
			 else if (m_pSettingDlg->IsWindowVisible())
			 {
				 m_pSettingDlg->SendMessage(WM_CANCEL,0,0);
			 }
			 else if (m_pMainDlg->m_firewalDlg_->IsWindowVisible()&&currentPanel_==panels_[IDC_BUTTON_MAIN])
			 {
				 m_pMainDlg->m_firewalDlg_->SendMessage(WM_CANCEL,0,0);
				 
			 }
			 else if (m_pInformationdlg->IsWindowVisible())
			 {
				 m_pInformationdlg->SendMessage(WM_CANCEL,0,0);
			 }
			 else if (m_pMainDlg->m_mainLunarderDlg_->IsWindowVisible()&&currentPanel_==panels_[IDC_BUTTON_MAIN])
			 {
				 m_pMainDlg->m_mainLunarderDlg_->SendMessage(WM_CANCEL,0,0);
			 }
			 else if (m_pMainDlg->m_mainCalucaterDlg_->IsWindowVisible()&&currentPanel_==panels_[IDC_BUTTON_MAIN])
			 {
				 m_pMainDlg->m_mainCalucaterDlg_->SendMessage(WM_CANCEL,0,0);			
			 }
			 else if (m_pSoundDlg->m_psoundTipDlg->IsWindowVisible())
			 {
				 m_pSoundDlg->m_psoundTipDlg->SendMessage(WM_CANCEL);
			 }
			 else if (m_pContactInfoDlg->m_pPasswordDlg->IsWindowVisible())
			 {
				 m_pContactInfoDlg->m_pPasswordDlg->SendMessage(WM_CANCEL);
				 
			 }
			 else if (m_pContactDlg->IsWindowVisible()||m_pContactInfoDlg->IsWindowVisible()||m_pSoundDlg->IsWindowVisible())
			 {
				 SwitchPanel_(IDC_BUTTON_MAIN);
			 }
		 }
	}
	break;
case WM_DELETE:
	OnEvent(0,0);
	if (!phone_->isBackLight_)
	{
		phone_->OpenBacklight(TRUE);
	}
	else if (m_pMainDlg->m_mainScreenSaveDlg_->IsWindowVisible())
	{
		SendOutEvnet(0, 0);
	}
	else
	{ 
		if (m_pTelephoneDlg->IsWindowVisible())
		{
			return 0;
		}
		if(!m_pSoundDlg->m_pRecordSoundDlg->IsWindowVisible()&&!m_pSoundDlg->m_pPlaySoundDlg->IsWindowVisible()&&!m_pContactInfoDlg->m_pPlaySoundDlg->IsWindowVisible()&&!m_pContactInfoDlg->m_pDetailTip->IsWindowVisible()&&!m_pSoundDlg->m_pDetailTip->IsWindowVisible()&&!m_pSoundDlg->m_psoundTipDlg->IsWindowVisible())
		{
			if(m_pContactDlg->m_pContactSearchDlg->IsWindowVisible())
			{
				return 0;
			}
			else if (m_pContactInfoDlg->m_pSearchContactInfoDlg->IsWindowVisible())
			{
				return 0;
			}
			else if (m_pSoundDlg->m_pSoundSearchDlg->IsWindowVisible())
			{
				return 0;
			}
			
			else if (m_pSettingDlg->m_pPasswordDlg->IsWindowVisible())
			{
				return 0;
			}
			else if (m_pSoundDlg->m_pPasswordDlg->IsWindowVisible())
			{
				return 0;
			}
			else if (m_pContactInfoDlg->m_pPasswordDlg->IsWindowVisible())
			{
				return 0;
			}
			else if(m_pSettingDlg->m_pFastDialsDlg->IsWindowVisible())
			{
				return 0;

			}else if (m_pInformationdlg->IsWindowVisible())
			{
				m_pInformationdlg->SendMessage(WM_DELETE);
			}
			else if (m_pContactGroupDlg->IsWindowVisible())
			{
				m_pContactGroupDlg->SendMessage(WM_DELETE,0,0);
			}
			else if (m_pContactDlg->IsWindowVisible()&&!m_pContactNewDlg->IsWindowVisible())
			{
				m_pContactDlg->SendMessage(WM_DELETE,0,0);
			}
			else if (m_pContactInfoDlg->IsWindowVisible())
			{
				m_pContactInfoDlg->SendMessage(WM_DELETE,0,0);
			}
			else if (m_pContactNewDlg->IsWindowVisible())
			{
				m_pContactNewDlg->SendMessage(WM_DELETE,0,0);

			}else if (m_pSoundDlg->IsWindowVisible())
			{
				m_pSoundDlg->SendMessage(WM_DELETE,0,0);
			}
			else if (m_pMainDlg->m_mainLunarderDlg_->IsWindowVisible())
			{
				m_pMainDlg->m_mainLunarderDlg_->SendMessage(WM_DELETE,0,0);
			}
			else if (m_pMainDlg->m_mainCalucaterDlg_->IsWindowVisible())
			{  
				m_pMainDlg->m_mainCalucaterDlg_->SendMessage(WM_DELETE,0,0);
			}		
		}
	}
	break;
case WM_ITYPE:
	OnEvent(0,0);
	if (!phone_->isBackLight_)
	{
		phone_->OpenBacklight(TRUE);
	}
	else if (m_pMainDlg->m_mainScreenSaveDlg_->IsWindowVisible())
	{
		SendOutEvnet(0, 0);
	}
	else
	{
		if (m_pTelephoneDlg->IsWindowVisible())
		{
			return 0;
		}
		if(m_pSoundDlg->m_pPlaySoundDlg->IsWindowVisible())
		{
			return 0;
		}
		else if (m_pContactInfoDlg->m_pPlaySoundDlg->IsWindowVisible())
		{
			return 0;
		}
		else if (m_pSoundDlg->m_pRecordSoundDlg->IsWindowVisible())
		{
			return 0;
		}
		else if (m_pSettingDlg->m_pPasswordDlg->IsWindowVisible())
		{
			return 0;
		}
		else if (m_pSoundDlg->m_pPasswordDlg->IsWindowVisible())
		{
			return 0;
		}
		else if (m_pContactInfoDlg->m_pPasswordDlg->IsWindowVisible())
		{
			return 0;
		}
		else if (m_pSettingDlg->m_pFastDialsDlg->IsWindowVisible())
		{
           return 0;
		}
		else if (m_pContactDlg->m_pContactSearchDlg->IsWindowVisible()||m_pContactInfoDlg->m_pSearchContactInfoDlg->IsWindowVisible()||m_pSoundDlg->m_pSoundSearchDlg->IsWindowVisible()||m_pSoundDlg->m_pDetailTip->IsWindowVisible()||m_pContactInfoDlg->m_pDetailTip->IsWindowVisible()||m_pSoundDlg->m_psoundTipDlg->IsWindowVisible())
		{
			return 0;
		}
		else if (m_pDeleteTipDlg->IsWindowVisible())
		{
			return 0;
		}
		else if (m_pContactNewDlg->IsWindowVisible())
		{
			return 0;
		}
		else if (m_pContactGroupDlg->IsWindowVisible())
		{
			return 0;
		}
		else if (m_pSettingDlg->IsWindowVisible())
		{
			m_pSettingDlg->SendMessage(WM_ITYPE,0,0);
		}
		else if (m_pSoundDlg->IsWindowVisible())
		{
			m_pSoundDlg->SendMessage(WM_ITYPE,0,0);
		}
		else if (m_pContactInfoDlg->IsWindowVisible())
		{
			m_pContactInfoDlg->SendMessage(WM_ITYPE,0,0);
		}
		else if (m_pContactDlg->IsWindowVisible())
		{
			m_pContactDlg->SendMessage(WM_ITYPE,0,0);
		}	
	}
	break;
case WM_SEARCH:
	OnEvent(0,0);
	if (!phone_->isBackLight_)
	{
		phone_->OpenBacklight(TRUE);
	}
	else if (m_pMainDlg->m_mainScreenSaveDlg_->IsWindowVisible())
	{
		SendOutEvnet(0, 0);
	}
	else
	{
		if (m_pTelephoneDlg->IsWindowVisible())
		{
			return 0;
		}
		if(m_pSoundDlg->m_pPlaySoundDlg->IsWindowVisible())
		{
			return 0;
		}
		else if (m_pContactInfoDlg->m_pPlaySoundDlg->IsWindowVisible())
		{
			return 0;
		}
		else if (m_pSoundDlg->m_pRecordSoundDlg->IsWindowVisible())
		{
			return 0;
		}
		else if (m_pSettingDlg->m_pPasswordDlg->IsWindowVisible())
		{
			return 0;
		}
		else if (m_pSoundDlg->m_pPasswordDlg->IsWindowVisible())
		{
			return 0;
		}
		else if (m_pContactInfoDlg->m_pPasswordDlg->IsWindowVisible())
		{
			return 0;
		}
		else if (m_pContactDlg->m_pContactSearchDlg->IsWindowVisible()||m_pContactInfoDlg->m_pSearchContactInfoDlg->IsWindowVisible()||m_pSoundDlg->m_pSoundSearchDlg->IsWindowVisible()||m_pSoundDlg->m_pDetailTip->IsWindowVisible()||m_pContactInfoDlg->m_pDetailTip->IsWindowVisible()||m_pSoundDlg->m_psoundTipDlg->IsWindowVisible())
		{
			return 0;
		}
		else if (m_pDeleteTipDlg->IsWindowVisible())
		{
			return 0;
		}
		else if (m_pContactNewDlg->IsWindowVisible())
		{
			return 0;

		}else if (m_pContactGroupDlg->IsWindowVisible())
		{
			return 0;
		}
		m_pSoundDlg->SendMessage(WM_SEARCH,0,0);
		m_pContactInfoDlg->SendMessage(WM_SEARCH,0,0);
		m_pContactDlg->SendMessage(WM_SEARCH,0,0);
	}
	break;
case WM_UPATADATA:

	if (m_bIsFinishDownLoad)
	{   
		m_bIsFinishDownLoad=FALSE;
		m_bIsFinishCopy=FALSE;

		std::string srcPath=Util::StringOp::FromCString(BackupPath);
		
		std::string desPath=Util::StringOp::FromCString(csFlashPath);
		
		CopyDB(srcPath,desPath," end usb copy from hive to flashdrv ");
		
		while(!m_bIsFinishCopy)
		{
			Sleep(0);		
		}
		
		DeleteFile((LPCTSTR)BackupPath);

		((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->UpdateShowList();

	}
	break;

case WM_REDIAL:
	if (theApp.IsHangON)
	{
		phone_->OpenBacklight(TRUE);

	}else{
		OnEvent(0,0);					     
		SendOutEvnet(0, 0);
	}
	break;
case WM_BATTERYTODC: //电池切换到电源
	if(g_bBattery)
	{
		g_bBattery = FALSE;
		m_pDeleteTipDlg->SetPasswordModel(false);
		m_pDeleteTipDlg->SetTitle(Data::LanguageResource::Get(Data::RI_ERROR));
		std::string str1 = Data::LanguageResource::Get(Data::RI_DELETETIP_BATTERYTODC);
		m_pDeleteTipDlg->SetDelTip(str1.c_str());
		m_pDeleteTipDlg->ShowWindow_(SW_SHOW);
	}
	break;
case WM_DCTOBATTERY:  //电源切换到电池
	{
		g_bBattery = TRUE;
		m_pDeleteTipDlg->SetPasswordModel(false);
		m_pDeleteTipDlg->SetTitle(Data::LanguageResource::Get(Data::RI_ERROR));
		std::string str1 = Data::LanguageResource::Get(Data::RI_DELETETIP_DCTOBATTERY);
		m_pDeleteTipDlg->SetDelTip(str1.c_str());
		m_pDeleteTipDlg->ShowWindow_(SW_SHOW);
	}
	break;
default:
	break;
	}
	if (message == WM_TEL_HUNGOFF)
	{
		m_pContactInfoDlg->SendMessage(WM_TEL_NOUSE, 0, 0);
		m_pSoundDlg->SendMessage(WM_TEL_NOUSE, 0, 0);
		m_pSettingDlg->SendMessage(WM_TEL_NOUSE, 0, 0);
	}
	
	return CWnd::WindowProc(message, wParam, lParam);
}

void CMultimediaPhoneDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CDialog::OnLButtonDown(nFlags, point);
}

void CMultimediaPhoneDlg::EnableLine(BOOL bEnable)
{
	m_StatusKey.SetEnableKey(2, bEnable);
}

void CMultimediaPhoneDlg::AddFSMRules(void)//注册机
{
	m_pFSM->registerRule(tsHangOff, teHangOn, tsHangOn, (void (__cdecl *const )(void *)) HangOn);
	m_pFSM->registerRule(tsHangOff, teRing, tsRing, (void (__cdecl *const )(void *)) Ring);
	
	m_pFSM->registerRule(tsHangOff, teCallID, tsRing, (void (__cdecl *const )(void *)) CallID);
	
	m_pFSM->registerRule(tsRing, teHangOn, tsConnect, (void (__cdecl *const )(void *)) Connect);
	m_pFSM->registerRule(tsRing, teHangOff, tsHangOff, (void (__cdecl *const )(void *)) HangOff);
	m_pFSM->registerRule(tsRing, teRing, tsRing, (void (__cdecl *const )(void *)) Ring);
	m_pFSM->registerRule(tsRing, teCallID, tsRing, (void (__cdecl *const )(void *)) CallID);
	
	m_pFSM->registerRule(tsConnect, teCallID, tsConnect, (void (__cdecl *const )(void *)) CallID);
	m_pFSM->registerRule(tsConnect, teHangOff, tsHangOff, (void (__cdecl *const )(void *)) HangOff);
	
	m_pFSM->registerRule(tsConnect, teKey, tsConnect, (void (__cdecl *const )(void *)) SubKey);
	
	m_pFSM->registerRule(tsHangOn, teHangOff, tsHangOff, (void (__cdecl *const )(void *)) HangOff);
	m_pFSM->registerRule(tsHangOn, teKey, tsKey, (void (__cdecl *const )(void *)) Key);
	m_pFSM->registerRule(tsKey, teConnect, tsConnect, (void (__cdecl *const )(void *)) Connect);//拨打时，超时即为接通
	m_pFSM->registerRule(tsKey, teHangOff, tsHangOff, (void (__cdecl *const )(void *)) HangOff);
	m_pFSM->registerRule(tsKey, teKey, tsKey, (void (__cdecl *const )(void *)) Key);
	
	m_pFSM->setStartState(tsHangOff);
}

void CMultimediaPhoneDlg::HangOff(void* param)
{
	::KillTimer((theApp.m_pMainWnd)->m_hWnd, ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_uiKeyTimer);

	((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->EnableLine(FALSE);

	((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pTelephoneDlg->HangOff_(param);
}

void CMultimediaPhoneDlg::HangOn(void* param)
{
	((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->EnableLine(TRUE);
	((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pTelephoneDlg->HangOn_(param);
}

void CMultimediaPhoneDlg::Key(void* param)
{
	::KillTimer((theApp.m_pMainWnd)->m_hWnd, ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_uiKeyTimer);
	::SetTimer((theApp.m_pMainWnd)->m_hWnd, ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_uiKeyTimer, KEYTIMEOUT, 0);
	((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pTelephoneDlg->Key_(param);

}

void CMultimediaPhoneDlg::SubKey(void* param)
{
	((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pTelephoneDlg->SubKey_(param);
}

void CMultimediaPhoneDlg::Connect(void* param)
{
	((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pTelephoneDlg->Connect_(param);
}

void CMultimediaPhoneDlg::Ring(void* param)
{
	::KillTimer((theApp.m_pMainWnd)->m_hWnd, ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_uiRingTimer);
	::SetTimer((theApp.m_pMainWnd)->m_hWnd, ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_uiRingTimer, RINGTIMEOUT, 0);
	((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->EnableLine(TRUE);
 	((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pTelephoneDlg->Ring_(param);
}

void CMultimediaPhoneDlg::CallID(void* param)
{
	::KillTimer((theApp.m_pMainWnd)->m_hWnd, ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_uiCallIdTimer);
	::SetTimer((theApp.m_pMainWnd)->m_hWnd, ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_uiCallIdTimer, CALLIDTIMEOUT, 0);
	((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pTelephoneDlg->CallID_(param);
}

void CMultimediaPhoneDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent == m_uiKeyTimer)
	{
		KillTimer(m_uiKeyTimer);
		m_pFSM->fireEvent(teConnect, 0);
	}
	else if (nIDEvent == m_uiRingTimer)
	{
		KillTimer(m_uiRingTimer);
		if (m_pFSM->getCurrentState() == tsRing)
		{   	
		   waveOutSetVolume(NULL,0x00000000);
		   phone_->SetSoundRingVal(-8000);
		   PostMessage(WM_TEL_HUNGOFF, 0, 0);
		}
	}
	else if(nIDEvent == m_uiCallIdTimer)  //来了CallID之后2s之后还没有Ring则挂机
	{	
		//wzx 20110107
		KillTimer(nIDEvent);
		if(!m_bComeRing)
		{
			PostMessage(WM_TEL_HUNGOFF, 0, 0);
		}
	}
	else if(nIDEvent == IDT_MAIN_SAVESCREEN_TIME)		//进入屏保
	{
		if(m_pSettingDlg->m_pSetting->isUseScreenSaver())
		{
			if(m_pFSM->getCurrentState() == tsHangOff)
			{
				if (!m_pContactInfoDlg->m_pPlaySoundDlg->IsWindowVisible()
					&& !m_pSoundDlg->m_pPlaySoundDlg->IsWindowVisible()
					&& !m_pSoundDlg->m_pRecordSoundDlg->IsWindowVisible()
					&& !m_pSettingDlg->m_copyfileDlg->IsWindowVisible())
				{
					KillTimer(IDT_MAIN_SAVESCREEN_TIME);
					//关闭输入法
					KeyBoardOff();
					HideComboBoxDropDown();
					m_pMainDlg->m_mainScreenSaveDlg_->ShowWindow(TRUE);
				}
			}
		}
	}
	
	else if(nIDEvent == IDT_MAIN_CLOSEBACKBRIGHT_TIME)		//进入关屏
	{
		if(m_pFSM->getCurrentState() == tsHangOff)
		{
			if(!m_pSettingDlg->m_pSetting->isUseScreenSaver())
			{
				if (!m_pContactInfoDlg->m_pPlaySoundDlg->IsWindowVisible()
					&& !m_pSoundDlg->m_pPlaySoundDlg->IsWindowVisible()
					&& !m_pSoundDlg->m_pRecordSoundDlg->IsWindowVisible()
					&& !m_pMainDlg->m_mainScreenSaveDlg_->IsWindowVisible())
				{
					KillTimer(IDT_MAIN_CLOSEBACKBRIGHT_TIME);
					phone_->OpenBacklight(FALSE);
				}
			}
		}
	}
	else if(33 == nIDEvent)
	{
		KillTimer(nIDEvent);

		CloseHandle(phone_->m_pRS232->handle_);			//lxz 20101228

		PROCESS_INFORMATION processInfo;
		STARTUPINFO lpStartupInfo; // 用于在创建子进程时设置各种属性 
		
		memset(&lpStartupInfo, 0, sizeof(lpStartupInfo));
		
		lpStartupInfo.cb = sizeof(lpStartupInfo);
		
		lpStartupInfo.dwX = 0;
		lpStartupInfo.dwY = 0;
		lpStartupInfo.dwXSize = 0;
		lpStartupInfo.dwYSize = 0;
		lpStartupInfo.wShowWindow = SW_SHOWNORMAL; 
		lpStartupInfo.dwFlags= 1|2|4;
		
		memset(&processInfo, 0, sizeof(processInfo));
		
		if (!CreateProcess(L"\\windows\\test_v31.exe", NULL, NULL, NULL, NULL, CREATE_NEW_CONSOLE, NULL, NULL, /*&lpStartupInfo*/ 0, &processInfo))
		{
			
		}
		
		::Sleep(50);
		
		CloseHandle(processInfo.hThread);
		CloseHandle(processInfo.hProcess);

		EndDialog(0);
		
		/*
		if(DetectFile(L"\\Hive\\updater.exe"))  //release
		{
			if(DetectFile(L"\\Hive\\up.exe"))
			{
				DeleteFile(L"\\Hive\\up.exe");
			}

			CFile::Rename(ssHivePath + L"updater.exe", ssHivePath + L"up.exe");
		}
		*/

		
		/*if(DetectFile(L"\\Hive\\MultimediaPhone.exe"))  //debug6
		{
			CFile::Rename(ssHivePath + L"MultimediaPhone.exe", ssHivePath + L"up.exe");
		}*/

		m_pDeleteTipDlg->SetPasswordModel(false);
		m_pDeleteTipDlg->SetTitle(Data::LanguageResource::Get(Data::RI_ERROR));
		std::string str1 = Data::LanguageResource::Get(Data::RI_DELETETIP_OPSUCCESS);//升级操作已成功
		m_pDeleteTipDlg->SetDelTip(str1.c_str());
		m_pDeleteTipDlg->ShowWindow_(SW_SHOW);
	}
	else if(15 == nIDEvent)
	{
		KillTimer(nIDEvent);
		unsigned char value = DC_BATTERY;
		phone_->WritePort(&value, 1);
	}
	
	CDialog::OnTimer(nIDEvent);
}

void CMultimediaPhoneDlg::OnMainSoftKey(WPARAM w, LPARAM l)
{
	switch(w)
	{

	case 1:
		OnButtonTime();
		break;
	case 2:
		OnButtonIME();
		break;
	case 3:
		OnButtonLine();
		break;
	case 4:
		OnButtonMain();
		break;
	case 5:
		OnButtonContact();
		break;
	case 6:
		OnButtonContactinfo();
		break;
	case 7:
		OnButtonSound();
		break;
	case 8:
		OnButtonInformation();
		break;
	case 9:
		OnButtonSetting();
		break;
	}
	
}

void CMultimediaPhoneDlg::SetSkinStyle()
{
	m_StatusKey.m_pKeys[1].nBitmapId = Data::g_mainstatusIMEBmpID[Data::g_skinstyle];
	m_StatusKey.m_pKeys[2].nBitmapId = Data::g_mainstatusTELBmpID[Data::g_skinstyle];
	m_StatusKey.m_pKeys[3].nBitmapId = Data::g_mainstatusDesktopBmpID[Data::g_skinstyle];
	m_StatusKey.m_pKeys[4].nBitmapId = Data::g_mainstatusCardBmpID[Data::g_skinstyle];
	m_StatusKey.m_pKeys[5].nBitmapId = Data::g_mainstatusCalllistBmpID[Data::g_skinstyle];
	m_StatusKey.m_pKeys[6].nBitmapId = Data::g_mainstatusRecordBmpID[Data::g_skinstyle];
	m_StatusKey.m_pKeys[7].nBitmapId = Data::g_mainstatusInfoBmpID[Data::g_skinstyle];
	m_StatusKey.m_pKeys[8].nBitmapId = Data::g_mainstatusSettingBmpID[Data::g_skinstyle];
	
	m_StatusKey.m_BitmapToggleID = Data::g_mainstatusToggleBmpID[Data::g_skinstyle];
	m_StatusKey.m_BitmapID = Data::g_mainstatusMainBmpID[Data::g_skinstyle];
	m_StatusKey.m_clrBack = Data::g_mainstatusBackRGB[Data::g_skinstyle];
}

//振铃 摘机 按键等外部事情发送给 屏保 MP3 VIDEO 听留言窗口
void CMultimediaPhoneDlg::SendOutEvnet(WPARAM w, LPARAM l)
{
	BOOL isShow = m_pMainDlg->m_mainScreenSaveDlg_->IsWindowVisible();

	if (isShow)
	{   
		m_pMainDlg->m_mainScreenSaveDlg_->SendMessage(WM_OUTEVENT, w, l);
	}

	phone_->OpenBacklight(TRUE);

	SendMessage(WM_GEN_EVENT);
}

void CMultimediaPhoneDlg::OnEvent(WPARAM w, LPARAM l)
{
	SetScreenSaveTimer();
}

void CMultimediaPhoneDlg::SetScreenSaveTimer()
{
	KillTimer(IDT_MAIN_SAVESCREEN_TIME);
	KillTimer(IDT_MAIN_CLOSEBACKBRIGHT_TIME);
	CTimeSpan nTimer;
	int nSecond;
	if (m_pSettingDlg->m_pSetting->isUseScreenSaver())
	{
		nTimer = m_pSettingDlg->m_pSetting->screenSaverDuration();
		nSecond = nTimer.GetTotalSeconds();
		if (nSecond > 0)
			SetTimer(IDT_MAIN_SAVESCREEN_TIME, nSecond*1000, NULL);
	}else
	{
		nTimer = m_pSettingDlg->m_pSetting->backlightDuration();
		nSecond = nTimer.GetTotalSeconds();
		if (nSecond > 0)
			SetTimer(IDT_MAIN_CLOSEBACKBRIGHT_TIME, nSecond*1000, NULL);
	}
}
void CMultimediaPhoneDlg::KillScreenSaveTimer()
{
	KillTimer(IDT_MAIN_SAVESCREEN_TIME);
	KillTimer(IDT_MAIN_CLOSEBACKBRIGHT_TIME);
}

void CMultimediaPhoneDlg::CloseBacklight()
{
	if(m_pFSM->getCurrentState() == tsHangOff)
	{
		if(!m_pSettingDlg->m_pSetting->isUseScreenSaver())
		{
			if (!m_pContactInfoDlg->m_pPlaySoundDlg->IsWindowVisible()
				&& !m_pSoundDlg->m_pPlaySoundDlg->IsWindowVisible()
				&& !m_pSoundDlg->m_pRecordSoundDlg->IsWindowVisible()
				&& !m_pMainDlg->m_mainScreenSaveDlg_->IsWindowVisible())
			{
				phone_->OpenBacklight(FALSE);
			}
		}
	}
}

void CMultimediaPhoneDlg::HideComboBoxDropDown(void)
{
	m_pContactGroupDlg->m_cmbRing.ShowDropDown(FALSE);
	m_pContactGroupDlg->m_cmbSoundTip.ShowDropDown(FALSE);
	m_pContactNewDlg->m_cmbType.ShowDropDown(FALSE);
	m_pContactNewDlg->m_cmbGroup.ShowDropDown(FALSE);
	m_pSettingDlg->m_cmbVolume.ShowDropDown(FALSE);
	m_pSettingDlg->m_cmbRing.ShowDropDown(FALSE);
	m_pSettingDlg->m_cmbRingTimes.ShowDropDown(FALSE);
	m_pSettingDlg->m_cmbTip.ShowDropDown(FALSE);
	m_pSettingDlg->m_cmbRecordLimit.ShowDropDown(FALSE);
	m_pSettingDlg->m_cmbWaitTime.ShowDropDown(FALSE);
	m_pSettingDlg->m_cmbBacklight.ShowDropDown(FALSE);
	m_pSettingDlg->m_cmbSoundSavePath.ShowDropDown(FALSE);
	m_pSettingDlg->m_cmbDelRingList.ShowDropDown(FALSE);
}

void CMultimediaPhoneDlg::panelToLeft()
{
	//关闭输入法
	KeyBoardOff();
	
	if (m_pContactGroupDlg->IsWindowVisible()||m_pContactNewDlg->IsWindowVisible())
	{
		m_pContactGroupDlg->ShowWindow(false);
		
		m_pContactNewDlg->ShowWindow(false);
	}
	if(currentPanel_ == panels_[IDC_BUTTON_SETTING])
	{
		currentPanelID_=IDC_BUTTON_INFORMATION;
	}	
	if (currentPanel_==panels_[IDC_BUTTON_INFORMATION])
	{
		currentPanelID_=IDC_BUTTON_SOUND;
	}
	if (currentPanel_==panels_[IDC_BUTTON_SOUND])
	{
		currentPanelID_=IDC_BUTTON_CONTACTINFO;
	}
	if (currentPanel_==panels_[IDC_BUTTON_CONTACTINFO])
	{
		currentPanelID_=IDC_BUTTON_CONTACT;
	}
	if (currentPanel_==panels_[IDC_BUTTON_CONTACT])
	{
		currentPanelID_=IDC_BUTTON_MAIN;
	}
	if (currentPanel_==panels_[IDC_BUTTON_MAIN])
	{
		currentPanelID_=IDC_BUTTON_SETTING;
	}
    
	HideComboBoxDropDown();

	SwitchPanel_(currentPanelID_);
	
}

void CMultimediaPhoneDlg::panelToRight()
{
	//关闭输入法
	KeyBoardOff();
	
	if (m_pContactGroupDlg->IsWindowVisible()||m_pContactNewDlg->IsWindowVisible())
	{
		m_pContactGroupDlg->ShowWindow(false);
		
		m_pContactNewDlg->ShowWindow(false);
	}
	if(currentPanel_ == panels_[IDC_BUTTON_SETTING])
	{
		currentPanelID_=IDC_BUTTON_MAIN;
	}	
	if (currentPanel_==panels_[IDC_BUTTON_INFORMATION])
	{
		currentPanelID_=IDC_BUTTON_SETTING;
	}
	if (currentPanel_==panels_[IDC_BUTTON_SOUND])
	{
		currentPanelID_=IDC_BUTTON_INFORMATION;
	}
	if (currentPanel_==panels_[IDC_BUTTON_CONTACTINFO])
	{
		currentPanelID_=IDC_BUTTON_SOUND;
	}
	if (currentPanel_==panels_[IDC_BUTTON_CONTACT])
	{
		currentPanelID_=IDC_BUTTON_CONTACTINFO;
	}
	if (currentPanel_==panels_[IDC_BUTTON_MAIN])
	{
		currentPanelID_=IDC_BUTTON_CONTACT;
	}

	HideComboBoxDropDown();

	SwitchPanel_(currentPanelID_);	
}

void CMultimediaPhoneDlg::PopUsbTIP(PopTip Poptype)
{              
	     std::string strTip=Data::LanguageResource::Get(Data::RI_CARDINFO_WARNING);

		  m_pDeleteTipDlg->SetHWnd(NULL);
		  
		  m_pDeleteTipDlg->SetPasswordModel(false); 
		  
		  m_pDeleteTipDlg->SetTitle(strTip);
		  
		  std::string str1;
		  
		  if(Poptype==CMultimediaPhoneDlg::UsbINSERTTIP)
		  {
			  m_hangon=true;
			 
			  str1=Data::LanguageResource::Get(Data::USB_INSERT_TIP1);
			  
              // m_pDeleteTipDlg->getM_btnOK()->ShowWindow(FALSE);
			  
			 // m_pDeleteTipDlg->getM_btnCancel()->ShowWindow(FALSE);
			  
			  
		  }else if(Poptype==CMultimediaPhoneDlg::UsbINSERTTIP_)
		  {
			  m_hangon=true;

			  str1=Data::LanguageResource::Get(Data::USB_INSERT_TIP2);
			  
              m_pDeleteTipDlg->getM_btnOK()->ShowWindow(FALSE);
			  
			  m_pDeleteTipDlg->getM_btnCancel()->ShowWindow(FALSE);
			  
			  
		  } else if (Poptype==CMultimediaPhoneDlg::START_UP_TIP)
		  {
			  
			  m_hangon=true;

			  str1=Data::LanguageResource::Get(Data::START_UP);
			  
			  
		  } else if (Poptype==CMultimediaPhoneDlg::UsbRemoveTip)
		  {
			   
			  m_hangon=true;
			  
			  str1=Data::LanguageResource::Get(Data::USB_REMOVE_TIP);
			  
			  m_pDeleteTipDlg->getM_btnOK()->ShowWindow(FALSE);
			  
			  m_pDeleteTipDlg->getM_btnCancel()->ShowWindow(FALSE);
			  
		  }
		  
		 m_pDeleteTipDlg->SetDelTip(str1.c_str());
		  
		 m_pDeleteTipDlg->ShowWindow_(SW_SHOW);

}

bool CMultimediaPhoneDlg::IsFindFile(CString path)
{
    CFileStatus status;
	
	bool flag=false;
	
	if (CFile::GetStatus(path,status))
	{
		if(status.m_size>0)
		{
			flag=true;
		}	  
	}
	
	return flag;
}
void CMultimediaPhoneDlg::WriteTOFlag(char *s)
{
	CFile c_file(_T("\\hive\\restart.flag"),CFile::modeCreate|CFile::modeReadWrite);
 	c_file.Write(s,1);
    c_file.Close();
}
void CMultimediaPhoneDlg::setIsTaking(bool flag)
{
	m_isTaking=flag;
}

bool CMultimediaPhoneDlg::getIsTaking()
{
	return m_isTaking;
}
void CMultimediaPhoneDlg::UpdateShowList()
{	
	extern int g_iContactCount;
	g_iContactCount = Data::Contact::GetDataCount(""); //wzx 20110103
	if (currentPanel_==panels_[IDC_BUTTON_MAIN])
	{
		((CMainDlg*)currentPanel_)->UpdateDiskInfo();
		((CMainDlg*)currentPanel_)->Invalidate(TRUE);
	}
	else if (currentPanel_==panels_[IDC_BUTTON_CONTACT])
	{   
		((CContactDlg *)currentPanel_)->ShowTypeItems();	
		((CContactDlg *)currentPanel_)->ShowItemsInList();	
	}
	else if (currentPanel_==panels_[IDC_BUTTON_CONTACTINFO])
	{
		((CContactInfoDlg*)currentPanel_)->ShowItemsInList();
	}
	else if (currentPanel_==panels_[IDC_BUTTON_SOUND])
	{
		((CSoundDlg*)currentPanel_)->ShowItemsInList(); 
	}
	else if (currentPanel_==panels_[IDC_BUTTON_SETTING])
	{
		((CSettingDlg*)currentPanel_)->IniCtrlData();
		((CSettingDlg*)currentPanel_)->ShowConfigItems();
	}
}

bool CMultimediaPhoneDlg::IsKeyShield(UINT message)
{	
		
	if (message == WM_RKEY ||
		message == WM_SPEEDDIAL || 
		message == WM_LOCALRECORD ||
		message == WM_PLAYLEAVEWORD ||
		message == WM_NEWCONTACT ||
		message == WM_KEYDOWN || 
		message == WM_BTLEFT ||
		message == WM_BTRIGHT ||
		message == WM_OK ||
		message == WM_CANCEL ||
		message == WM_DELETE ||
		message == WM_ITYPE ||
		message == WM_SEARCH 
		)
	{	
		if (m_pMainDlg && m_pMainDlg->m_mainScreenSaveDlg_)
		{
			if (m_pMainDlg->m_mainScreenSaveDlg_->IsWindowVisible())
			{	
				return true;
			}
		}
	}	
	return false ;
}

