
// MultimediaPhoneDlg.cpp : 实现文件
//

#include "stdafx.h"
#include <ntddndis.h>
//#include <sip.h>
//#include <INITGUID.h>
//#include <imaging.h>
#include "MultimediaPhone.h"
#include "MultimediaPhoneDlg.h"
#include "Data/SkinStyle.h"
#include "Data/LanguageResource.h"
#include "View/ViewConstants.h"
#include "View/ContactDlg.h"
#include "View/InformationDlg.h"
#include "View/SettingDlg.h"
#include "View/ContactNewDlg.h"
#include "Util/socket.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CString photoPath = _T("C:/flashdrv/my_photo");
CString musicPath = _T("C:/flashdrv/my_music");
CString videoPath = _T("C:/flashdrv/my_video");
CString ringPath = _T("C:/flashdrv/my_ring");
CString adPath = _T("C:/flashdrv/my_show");
CString recordPath = _T("C:/flashdrv/my_record");
CString notePath = _T("C:/flashdrv/my_note");
CString webPath = _T("C:/flashdrv/my_web");

// CMultimediaPhoneDlg 对话框

//#define WM_PLAYVIDEO	8002
#define  BMP_WIDTH			 800
#define  BMP_HEIGHT          420
#define  BMP_WIDTH1			 692
#define  BMP_HEIGHT1         296

#define  BMP_WIDTH2			 800
#define  BMP_HEIGHT2         60
#define  BMP_WIDTH3			 535
#define  BMP_HEIGHT3         420
#define  BMP_WIDTH4			 800
#define  BMP_HEIGHT4         480

#define  KEYTIMEOUT  10 * 1000
#define  RINGTIMEOUT  10 * 1000
extern  BOOL DetectDIR(TCHAR *sDir);
void CopyDirFiles()
{
    CString SrcDir = _T("/usbdisk");
    CString DesDir = photoPath;
    CString findFilename = SrcDir + _T("/*.*");
    //wprintf(_T("find file name is %s\n"), (LPCTSTR)findFilename);
    WIN32_FIND_DATA FindFileData;
    HANDLE hFind = FindFirstFile((LPCTSTR)findFilename, &FindFileData);
    if (hFind == INVALID_HANDLE_VALUE)
    {
        //Dprintf("not find file\n");
    }
    else
    {
        bool finished = false;
        do
        {
            wchar_t wideToName[256];
            //	wchar_t root[256] = {0};
            //	mbstowcs(root, rootPath.c_str(), rootPath.length());
            wsprintf(wideToName, _T("%s/%s"), DesDir, (LPCTSTR)FindFileData.cFileName); //findFileName
            //	wideToName =  
            wchar_t wideFromName[256] = {0};
            //	wchar_t usb[256] = {0};
            wsprintf(wideFromName, _T("%s/%s"), SrcDir, (LPCTSTR)FindFileData.cFileName); //findFileName

            if ((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
            {
                //wprintf(_T("create dir %s to %s\n"), wideFromName, wideToName);
                //CopyDirFiles(wideFromName, wideToName);
            }
            else
            {
                if (!::CopyFile(wideFromName, wideToName, false))
                {
                    //Dprintf("current error is %d.", GetLastError());
                }
                wprintf(_T("copy %s to %s\n"), wideFromName, wideToName);
            }

            if (!FindNextFile(hFind, &FindFileData))
            {
                if (GetLastError() == ERROR_NO_MORE_FILES)
                {
                    //Dprintf("find end.\n");
                    finished = true;
                }
                else
                {
                    //Dprintf("Couldn't find next file.\n");
                }
            }
        } while (!finished);
        FindClose(hFind);
    }
}

int ALLBMPMEMORY_MAX =	0;
int	g_ALLBMPMemoryCount = -1;
struct  ALLBmpMemoryInfo
{
    CString  sFileName;
    HDC		 hdc;
    HBITMAP  hbmp;
    HBITMAP  hOldbmp;
    CRect	 rect;
};

ALLBmpMemoryInfo *g_allBmpMemory;
void InitALLBmpMemoryInfo()
{

    MEMORYSTATUS   mem;     
    mem.dwLength=sizeof(mem);     
    GlobalMemoryStatus(&mem);     
    if (mem.dwAvailPhys >= 64*1024*1024)
    { 
        ALLBMPMEMORY_MAX = 15;     //   15   remark 
        //Dprintf("BMP Allac 30\r\n");
    }
    else
    {
        //Dprintf("BMP Allac 0\r\n");
    }

    if (ALLBMPMEMORY_MAX > 0)
        g_allBmpMemory = new ALLBmpMemoryInfo[ALLBMPMEMORY_MAX];

    for (int i = 0; i < ALLBMPMEMORY_MAX; ++i)
    {
        g_allBmpMemory[i].sFileName = "";
        g_allBmpMemory[i].hdc = NULL;
        g_allBmpMemory[i].rect = CRect(0, 0, 0 ,0);
        g_allBmpMemory[i].hbmp = NULL;
        g_allBmpMemory[i].hOldbmp = NULL;
    }
}

BOOL FindMemoryInfo(CString sAllFilename, int &inDexAllBmpMemeoryinfo)
{
    for (int i = 0; i <= g_ALLBMPMemoryCount; ++i)
    {
        if (g_allBmpMemory[i].sFileName == sAllFilename)
        {
            inDexAllBmpMemeoryinfo = i;

            //	wprintf(g_allBmpMemory[i].sFileName);
            //Dprintf(" FindAllMemeory %d \r\n", i);

            return TRUE;
        }
    }
    return FALSE;
}

BOOL AddALLBmpMemoryInfo(CString sFileName, CRect rect, CDC *pdc)
{
    int n;
    if (FindMemoryInfo(sFileName, n))
        return FALSE;

    if (g_ALLBMPMemoryCount < (ALLBMPMEMORY_MAX - 1))
    {
        ++g_ALLBMPMemoryCount;

        g_allBmpMemory[g_ALLBMPMemoryCount].sFileName = sFileName;
        g_allBmpMemory[g_ALLBMPMemoryCount].rect = rect;
        g_allBmpMemory[g_ALLBMPMemoryCount].hdc = CreateCompatibleDC(pdc->m_hDC); 

        BYTE  *lpBitmapBits = NULL;
        BITMAPINFO RGB24BitsBITMAPINFO; 
        ZeroMemory(&RGB24BitsBITMAPINFO, sizeof(BITMAPINFO));
        RGB24BitsBITMAPINFO.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        RGB24BitsBITMAPINFO.bmiHeader.biWidth = rect.Width();
        RGB24BitsBITMAPINFO.bmiHeader.biHeight = rect.Height();
        RGB24BitsBITMAPINFO.bmiHeader.biPlanes = 1;
        RGB24BitsBITMAPINFO.bmiHeader.biBitCount = 24;

        g_allBmpMemory[g_ALLBMPMemoryCount].hbmp = CreateDIBSection(g_allBmpMemory[g_ALLBMPMemoryCount].hdc, (BITMAPINFO*)&RGB24BitsBITMAPINFO, DIB_RGB_COLORS, (void **)&lpBitmapBits, NULL, 0);
        g_allBmpMemory[g_ALLBMPMemoryCount].hOldbmp = (HBITMAP)SelectObject(g_allBmpMemory[g_ALLBMPMemoryCount].hdc, g_allBmpMemory[g_ALLBMPMemoryCount].hbmp);
        BitBlt(g_allBmpMemory[g_ALLBMPMemoryCount].hdc, 0, 0, rect.Width(), rect.Height(), pdc->m_hDC, 0, 0, SRCCOPY);

        //	wprintf(sFileName);
        //Dprintf(" AddAllMemeory %d \r\n", g_ALLBMPMemoryCount);

        return TRUE;
    }
    return FALSE;
}

BOOL ReleaseALLBmpMemoryInfo()
{
    for (int i = 0; i <= g_ALLBMPMemoryCount; ++i)
    {
        SelectObject(g_allBmpMemory[i].hdc, g_allBmpMemory[i].hOldbmp);
        DeleteObject(g_allBmpMemory[i].hbmp);
        DeleteDC(g_allBmpMemory[i].hdc);
    }
    g_ALLBMPMemoryCount = -1;
    return TRUE;
}

CMultimediaPhoneDlg::CMultimediaPhoneDlg(CWnd* pParent /*=NULL*/)
: CDialog(CMultimediaPhoneDlg::IDD, pParent)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    //InitImage();
    n_bklightcount = 0;
    m_bIsHungOn = FALSE;
    m_bNetOkStatus = FALSE;
    m_bNetType = 0;
}

CMultimediaPhoneDlg::~CMultimediaPhoneDlg()
{
    for (std::map<int, CDialog*>::iterator i = panels_.begin(); i != panels_.end(); ++i)
        delete i->second;
}

void CMultimediaPhoneDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMultimediaPhoneDlg, CDialog)
    //{{AFX_MSG_MAP(CMultimediaPhoneDlg)
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_WM_LBUTTONDOWN()
    ON_WM_TIMER()
    ON_MESSAGE(WM_SOFTKEY, OnMainSoftKey)
    ON_MESSAGE(WM_GEN_EVENT, OnEvent)
    ON_MESSAGE(WM_DEVICECHANGE, OnDeviceChange)
    ON_MESSAGE(WM_CLICKMJPG_TOAPP, OnClickMJPG)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()


// CMultimediaPhoneDlg 消息处理程序

/*BOOL CMultimediaPhoneDlg::OnInitDialog()
{
CDialog::OnInitDialog();

// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
//  执行此操作
SetIcon(m_hIcon, TRUE);			// 设置大图标
SetIcon(m_hIcon, FALSE);		// 设置小图标

// TODO: 在此添加额外的初始化代码

return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}*/

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMultimediaPhoneDlg::OnPaint()
{
    if (IsIconic())
    {
        CPaintDC dc(this); // 用于绘制的设备上下文

        SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

        // 使图标在工作区矩形中居中
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;

        // 绘制图标
        dc.DrawIcon(x, y, m_hIcon);
    }
    else
    {
        CDialog::OnPaint();
    }
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMultimediaPhoneDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}

//IImagingFactory* pImgFactory;

//void InitImage()
//{
//    CoInitializeEx(NULL, COINIT_MULTITHREADED); 
//    CoCreateInstance(CLSID_ImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_IImagingFactory, (void**)&pImgFactory);
//}

void DrawImage(LPCTSTR szFileImage, CDC *pDC, CRect rt, int isOffset = 0)
{ 
    wprintf(szFileImage);
    printf("\r\n");

    CRect newRect = rt;
    if (isOffset > 0)
    {
        newRect = CRect(0, 0, rt.Width(), rt.Height());
    }

    CString sFile = szFileImage;
    if (sFile.Find(L".bmp") != -1 || sFile.Find(L".BMP") != -1)
    {
        CFile file;   
        if (!file.Open(szFileImage, CFile::modeRead))   
        {     
            return;
        }  

        BITMAPFILEHEADER bfhHeader;   
        file.Read(&bfhHeader, sizeof(BITMAPFILEHEADER));   
        if (bfhHeader.bfType != ((WORD)('M' << 8) | 'B'))   
        {   
            ::MessageBox(NULL, L"The file is not a bitmap file!",  L"warning", MB_OK);  
            file.Close();
            return;   
        }   
        if (bfhHeader.bfSize != file.GetLength())     
        {   
            ::MessageBox(NULL, L"The BMP file header error!", L"warning", MB_OK);   
            file.Close();
            return;   
        }   
        UINT uBmpInfoLen = (UINT)bfhHeader.bfOffBits - sizeof(BITMAPFILEHEADER);   
        LPBITMAPINFO lpBitmap = (LPBITMAPINFO)new BYTE[uBmpInfoLen];  

        file.Read((LPVOID)lpBitmap, uBmpInfoLen);   
        if ((*(LPDWORD)(lpBitmap)) != sizeof(BITMAPINFOHEADER))     
        {   
            ::MessageBox(NULL, L"The BMP is not Windows 3.0 format!", L"warning", MB_OK);  
            delete []lpBitmap;    //20090216 test
            file.Close();
            return;   
        }   
        file.Close();

        LPVOID lpBits;   

        int w = lpBitmap->bmiHeader.biWidth;
        int h = lpBitmap->bmiHeader.biHeight;

        /////////////////////////
        IPicture* pPic;
        IStream* pStm;

        CFileStatus fstatus;
        // CFile file;
        LONG cb;

        if (file.Open(szFileImage, CFile::modeRead)
            && file.GetStatus(szFileImage, fstatus)
            && ((cb = fstatus.m_size) != -1))
        {
            HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, cb);   
            LPVOID pvData = NULL;
            if (hGlobal != NULL)
            {
                if ((pvData = GlobalLock(hGlobal)) != NULL)
                {
                    file.Read(pvData, cb);
                    GlobalUnlock(hGlobal);
                    CreateStreamOnHGlobal(hGlobal, TRUE, &pStm);

                    if (SUCCEEDED(OleLoadPicture(pStm, fstatus.m_size, TRUE, IID_IPicture, (LPVOID*)&pPic)))
                    {
                        OLE_XSIZE_HIMETRIC hmWidth;
                        OLE_YSIZE_HIMETRIC hmHeight;

                        pPic->get_Width(&hmWidth);
                        pPic->get_Height(&hmHeight);

                        double fX, fY;
                        fX = (double)pDC->GetDeviceCaps(HORZRES) * (double)hmWidth / ((double)pDC->GetDeviceCaps(HORZSIZE) * 100.0);
                        fY = (double)pDC->GetDeviceCaps(VERTRES) * (double)hmHeight / ((double)pDC->GetDeviceCaps(VERTSIZE) * 100.0);

                        DWORD x1 = 0;
                        DWORD y1 = 0;
                        DWORD w1 = hmWidth;
                        DWORD h1 = hmHeight;
                        DWORD x0 = newRect.left;
                        DWORD y0 = newRect.top;
                        if (isOffset > 0) {
                            x1 = rt.left* hmWidth/w;
                            y1 = rt.top*hmHeight/h;
                            w1 = newRect.Width()*hmWidth/w;
                            h1 = newRect.Height()*hmHeight/h;
                        }
                        if(isOffset == 2)
                        {
                            x0 = rt.left;
                            y0 = rt.top;
                        }

                        if (FAILED(pPic->Render(*pDC, x0, y0, (DWORD)newRect.Width(), (DWORD)newRect.Height(), x1, hmHeight-y1, w1, -h1, NULL)))
                        {
                            ::MessageBox(NULL, L"Render error!", L"warning", MB_OK);  
                            GlobalFree(hGlobal);
                            file.Close();
                            //return -1;  //render error 
                            return;
                        }
                        pPic->Release();

                        GlobalFree(hGlobal);
                        file.Close();
                    }
                    else
                    {
                        ::MessageBox(NULL, L"OleLoadPicture error!", L"warning", MB_OK);  
                        file.Close();
                        GlobalFree(hGlobal);
                        //return -2;
                        return;
                    }
                    pStm->Release();
                }
            }
            else
                ::MessageBox(NULL, L"No Memory", L"warning", MB_OK);  
        }
        else {
            //return -3;
            return;
        }
        delete []lpBitmap;    //20090216 test
    }
    else
         ::MessageBox(NULL, L"No BMP", L"warning", MB_OK);  
    //return 0;
    return;
}

/////////////////////////////////////////////////////////////////////////////
// CMultimediaPhoneDlg message handlers
extern BOOL watchdog_test;
LRESULT CMultimediaPhoneDlg::OnClickMJPG(WPARAM w, LPARAM l)
{
    LRESULT result = 0;
    switch (w)
    {
    case 1:
        //	watchdog_test = !watchdog_test;
        m_pNetStatusDlg->SetType(m_bNetType, m_bNetOkStatus);		//网络状态
        m_pNetStatusDlg->ShowWindow_(SW_SHOW);
        break;
    case 2:
        OnButtonLine();		//电话状态
        break;
    case 3:
        OnButtonMain();
        break;
    case 4:
        OnButtonContact();
        break;
    case 5:
        OnButtonContactinfo();
        break;
    case 6:
        OnButtonSound();
        break;
    case 7:
        OnButtonInformation();
        break;
    case 8:
        OnButtonSetting();
        break;
    }
    if (w >= 3)
    {
        m_MJPGList.SetUnitIsDownStatus(2, FALSE);
        m_MJPGList.SetUnitIsDownStatus(3, FALSE);
        m_MJPGList.SetUnitIsDownStatus(4, FALSE);
        m_MJPGList.SetUnitIsDownStatus(5, FALSE);
        m_MJPGList.SetUnitIsDownStatus(6, FALSE);
        m_MJPGList.SetUnitIsDownStatus(7, FALSE);
        m_MJPGList.SetUnitIsDownStatus(w-1, TRUE);
        m_MJPGList.Invalidate();
    }
    return 0;
}

BOOL  watchdog_test = FALSE;
void WatchDogProc(void *)
{
    CMultimediaPhoneDlg *pMainDlg = ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd));
    //	Util::ClientSocket *netSocket = NULL;

    while (true)
    {
#ifdef _DEBUG
        //pMainDlg->SetWatchDog();   //设置watchdog
#else
        //if (!watchdog_test)
        pMainDlg->SetWatchDog();   //设置watchdog
#endif
        SYSTEMTIME tm;
        GetLocalTime(&tm);
        if (tm.wHour == 0 && tm.wMinute == 0 && tm.wSecond < 8)
        {
            pMainDlg->SetAlarmList();
            Sleep(8000);
        }
        else
            Sleep(5000);
    }
}

static BOOL gIsToLight;

BOOL CMultimediaPhoneDlg::OnInitDialog()
{
    //lxz test jpg
    CDialog::OnInitDialog();
    SetWindowText(L"EDO Phone Dialog");
    ModifyStyleEx(0, WS_EX_TOPMOST);
    MoveWindow(0*X_XISHU, 0*Y_XISHU, 800*X_XISHU, 480*Y_XISHU);

    InitALLBmpMemoryInfo();

    // Set the icon for this dialog.  The framework does this automatically
    //  when the application's main window is not a dialog
    SetIcon(m_hIcon, TRUE);			// Set big icon
    SetIcon(m_hIcon, FALSE);		// Set small icon

    CenterWindow(GetDesktopWindow());	// center to the hpc screen
    //step1: global variable init

    //step2: global workthread init

    //step3: config data read and fill

    //step4: device and environment check and init

    //step5: init UI
    //检测SD ,USB
    InitSCLPort();
    m_bIsHaveUSB = DetectDIR(L"UsbDisk");
    m_bIsSD = DetectDIR(L"StorageCard");
    //SetTimer(IDT_DETECTUSB, 500, NULL);

    //init player
    //other wnd
    playervideo_ = new Player(NULL, mtVideo);
    playervideo_->SetPlayerOwner(this);

    playeraudio_ = new Player(NULL, mtAudio);
    playeraudio_->SetPlayerOwner(this);

    playerimage_ = new Player(NULL, mtImage);
    playerimage_->SetPlayerOwner(this);

    //m_pPhone = new Logical::Phone(PickupFunc, HandsfreeFunc, HangupFunc, KeypadDialFunc, ConnectFunc, RingInFunc, RemoteHangupFunc, TimeoutFunc, ErrorFunc);
    //m_pPhone->start();

    phone_ = new Phone();
    simdevice_ = new SIMDevice(NULL);

    m_MJPGList.Create(L"", WS_VISIBLE|WS_CHILD, CRect(0*X_XISHU, 420*Y_XISHU, 800*X_XISHU, 480*Y_XISHU), this);
    m_MJPGList.SetCurrentLinkFile(_T(".\\adv\\mjpg\\k1\\中文\\1_new.xml"));
    m_MJPGList.SetMJPGRect(CRect(0*X_XISHU, 420*Y_XISHU, 800*X_XISHU, 480*Y_XISHU));
    //m_MJPGList.SetUnitIsDownStatus(2, TRUE);
    m_MJPGList.SetUnitIsDownStatus(2, TRUE);

    m_MJPGList.SetUnitIsDisable(0, FALSE);
    m_MJPGList.SetUnitIsDisable(1, TRUE);

    CDialog* panel; 
    panel = new CInformationDlg();
    panel->Create(CInformationDlg::IDD);
    panels_[IDC_BUTTON_INFORMATION] = panel;

    m_pSettingDlg = new CSettingDlg();
    panel = m_pSettingDlg;
    panel->Create(CSettingDlg::IDD);
    panels_[IDC_BUTTON_SETTING] = panel;


    /*
    设置 三个 界面
    */
    m_pTelephoneDlg = new CTelephoneDlg();
    m_pTelephoneDlg->Create(CTelephoneDlg::IDD);

    m_pMainDlg = new CMainDlg();
    panel = m_pMainDlg;
    panel->Create(CMainDlg::IDD);
    panels_[IDC_BUTTON_MAIN] = panel;
    currentPanel_ = panel;

    m_pContactDlg = new CContactDlg();
    m_pContactDlg->Create(CContactDlg::IDD);
    panel = m_pContactDlg;
    panels_[IDC_BUTTON_CONTACT] = panel;

    m_pContactInfoDlg = new CContactInfoDlg();
    panel = m_pContactInfoDlg;
    panel->Create(CContactInfoDlg::IDD);
    panels_[IDC_BUTTON_CONTACTINFO] = panel;

    m_pSoundDlg = new CSoundDlg();
    panel = m_pSoundDlg;
    panel->Create(CSoundDlg::IDD);
    panels_[IDC_BUTTON_SOUND] = panel;

    m_pContactNewDlg = new CContactNewDlg(panels_[IDC_BUTTON_CONTACT]);
    panel = m_pContactNewDlg;
    panel->Create(CContactNewDlg::IDD);
    panels_[IDC_BUTTON_CONTACTNEW] = panel;

    m_pContactGroupDlg = new CContactGroupDlg(panels_[IDC_BUTTON_CONTACT]);
    panel = m_pContactGroupDlg;
    panel->Create(CContactGroupDlg::IDD);
    panels_[IDC_BUTTON_CONTACTGROUP] = panel;


    m_pDeleteTipDlg = new CDeleteTipDlg(this);
    m_pDeleteTipDlg->Create(CDeleteTipDlg::IDD);

    m_pNotebookDlg = new CNotebookDlg(this);
    m_pNotebookDlg->Create(CNotebookDlg::IDD);

    m_AlarmShowDlg = new CAlarmShowDlg(this);
    m_AlarmShowDlg->Create(CAlarmShowDlg::IDD);

    m_pWarningNoFlashDlg = new CWarningNoFlashDlg(this);
    m_pWarningNoFlashDlg->Create(CWarningNoFlashDlg::IDD);

    m_pNetStatusDlg = new CNetStatusDlg(this);
    m_pNetStatusDlg->Create(CNetStatusDlg::IDD);

    currentPanel_->ShowWindow(SW_SHOW);
    //m_pBtnCurrent = &m_btnMain;
    //SetButtonSelectedColor(m_pBtnCurrent);

    //and so on
    //m_btnLine.EnableWindow(FALSE);
    m_pFSM = new Util::FSM;
    AddFSMRules();
    m_uiKeyTimer = 1;
    m_uiRingTimer = 2;

    //设置进入屏保
    SetScreenSaveTimer();
    SetTimer(IDT_CONTROLBACKLIGHT_TIME+1, 60*1000, NULL);
    SetTimer(0x125, 1000, NULL);

    //是否启动背光   lxz 20081223
    //	if(!SetNightControlBackLightTimer())
    //		ReStoreBackLight();

    //设置闹铃
    SetAlarmList();
    //查找今天没有看过过期的闹铃
    m_pMainDlg->FindTodayAlarm();

    DWORD watchdogThreadID = 0;
    HANDLE m_pThread1 = CreateThread (NULL, 0, (LPTHREAD_START_ROUTINE)WatchDogProc, 0, 0, &watchdogThreadID );

    return TRUE;  // return TRUE  unless you set the focus to a control
}

#define NDISUIO_DEVICE_NAME TEXT( "UIO1: ")
#define FILE_ANY_ACCESS 0
#define METHOD_BUFFERED 0
#define FILE_DEVICE_NETWORK 0x00000012

#define FSCTL_NDISUIO_BASE FILE_DEVICE_NETWORK

typedef struct _NDISUIO_QUERY_BINDING { 
    ULONG BindingIndex; //   0-based   binding   number 
    ULONG DeviceNameOffset; //   from   start   of   this   struct 
    ULONG DeviceNameLength; //   in   bytes 
    ULONG DeviceDescrOffset; //   from   start   of   this   struct 
    ULONG DeviceDescrLength; //   in   bytes 

} NDISUIO_QUERY_BINDING, *PNDISUIO_QUERY_BINDING; 

typedef ULONG NDIS_OID, *PNDIS_OID; 

typedef struct _NDISUIO_QUERY_OID {
    NDIS_OID Oid;
    PTCHAR ptcDeviceName;
    UCHAR Data[sizeof(ULONG)];
} NDISUIO_QUERY_OID, *PNDISUIO_QUERY_OID;

//#define CTL_CODE(DeviceType, Function, Method, Access) (((DeviceType) << 16) | ((Access) << 14) | ((Function) << 2) | (Method)) 
#define _NDISUIO_CTL_CODE(_Function, _Method, _Access) CTL_CODE(FSCTL_NDISUIO_BASE, _Function, _Method, _Access) 
#define IOCTL_NDISUIO_QUERY_BINDING _NDISUIO_CTL_CODE(0x203, METHOD_BUFFERED, FILE_ANY_ACCESS) 
#define IOCTL_NDISUIO_OPEN_DEVICE _NDISUIO_CTL_CODE(0x200, METHOD_BUFFERED, FILE_ANY_ACCESS) 
#define IOCTL_NDISUIO_QUERY_OID_VALUE _NDISUIO_CTL_CODE(0x201, METHOD_BUFFERED, FILE_ANY_ACCESS) 

int CMultimediaPhoneDlg::CheckInternetStatus()
{
    if (m_pSettingDlg->m_pSetting->linkMode() == Data::lmDirect)   //网络直连
    {
        m_bNetType = 0;
        int ret = 0;
        HANDLE hDevice = INVALID_HANDLE_VALUE;

        hDevice = CreateFile(TEXT("UIO1:"), GENERIC_READ | GENERIC_WRITE,
            FILE_SHARE_READ | FILE_SHARE_WRITE,
            NULL, OPEN_ALWAYS, 0, NULL);


        if (hDevice == INVALID_HANDLE_VALUE)
        {
            //Dprintf("Open Error!\r\n");
            return 0;
        }

        //--------------------------------------------------------------------- 
        //   retrieve   the   name   and   description   of   the   adapter. 
        //--------------------------------------------------------------------- 
        CHAR chData[1024] = {0};
        PNDISUIO_QUERY_BINDING pBindInfo = (PNDISUIO_QUERY_BINDING)chData;
        pBindInfo->BindingIndex = 0;
        int length = sizeof(NDISUIO_QUERY_BINDING);
        DWORD bytesconsumed = 0;
        BOOL bValue = DeviceIoControl(hDevice,   
            IOCTL_NDISUIO_QUERY_BINDING,   
            pBindInfo,   
            length,   
            NULL,   
            1024,   
            &bytesconsumed,   
            NULL);   
        if (!bValue)   
        {   
            CloseHandle(hDevice);
            return 0;   
        }   

        DWORD dwReturnedBytes;   
        PTCHAR ptcDeviceName = (PTCHAR)((PUCHAR)pBindInfo + pBindInfo->DeviceNameOffset);   

        //--------------------------------------------------------------------- 
        //   Open   Device 
        //---------------------------------------------------------------------   
        DWORD deErrCode = 0;   
        bValue = DeviceIoControl(hDevice,   
            IOCTL_NDISUIO_OPEN_DEVICE,   
            ptcDeviceName,   
            wcslen(ptcDeviceName) * sizeof(TCHAR),   
            NULL,   
            0,   
            &dwReturnedBytes,   
            NULL);   

        if (0 == bValue)
        {
            deErrCode = GetLastError();
            CloseHandle(hDevice);
            return 0;
        }   

        PNDISUIO_QUERY_OID queryOid;
        PUCHAR QueryBuffer[1024];

        queryOid = (PNDISUIO_QUERY_OID)QueryBuffer;

        queryOid->Oid = OID_GEN_MEDIA_CONNECT_STATUS;          //   Custom   OID;   
        queryOid->ptcDeviceName = NULL;						//   You   dont   need   this   once   you   have   done   a   OPEN   Device.   

        bValue = DeviceIoControl(hDevice,
            IOCTL_NDISUIO_QUERY_OID_VALUE,
            (LPVOID)queryOid,
            sizeof(NDISUIO_QUERY_OID) + sizeof(DWORD),
            (LPVOID)queryOid,
            sizeof(NDISUIO_QUERY_OID) + sizeof(DWORD),
            &bytesconsumed,
            NULL);

        if (*(ULONG*)(queryOid->Data) == NdisMediaStateConnected)// Connected! 
        {
            ret = 1;
        }
        else if (*(ULONG*)(queryOid->Data) == NdisMediaStateDisconnected)
        {
            ret = 0;
        }

        CloseHandle(hDevice); // What was the result?
        return ret;
    }
    else if (m_pSettingDlg->m_pSetting->linkMode() == Data::lmDial)  //网络拨号
    {
        m_bNetType = 1;
        int Statuscode;
        if (CNetStatusDlg::CheckADSLStatus(Statuscode))
        {
            m_pNetStatusDlg->m_bADSLISConnnect = TRUE;
            return 1;
        }
        else
        {
            m_pNetStatusDlg->m_bADSLISConnnect = FALSE;
            return 0;
        }
    }
}

void CMultimediaPhoneDlg::SetNetTelStatus()
{
    static CString gsNet;
    static CString gsTelName;
    CString sTel = _T(".\\adv\\mjpg\\k1\\common\\电话2.bmp");
    CString sNet = _T(".\\adv\\mjpg\\k1\\common\\网络2.bmp");
    static int gnTel = 0;
    static int gnNet = 0;

    if (m_pFSM->getCurrentState() != tsHangOff)   //摘机
    {
        if (!m_pTelephoneDlg->IsWindowVisible())
        {
            m_MJPGList.SetUnitIsDisable(1, FALSE);
            if (gnTel++ % 2)
                sTel = _T(".\\adv\\mjpg\\k1\\common\\电话1.bmp");
            else
                sTel = _T(".\\adv\\mjpg\\k1\\common\\电话2.bmp");
        }
    }
    else
    {
        m_MJPGList.SetUnitIsDisable(1, TRUE);
    }
    m_bNetOkStatus = CheckInternetStatus();
    if (m_bNetOkStatus)
    {
        if (gnNet++ % 2)
            sNet = _T(".\\adv\\mjpg\\k1\\common\\网络1.bmp");
        else
            sNet = _T(".\\adv\\mjpg\\k1\\common\\网络2.bmp");
    }

    if (gsTelName != sTel)
    {
        m_MJPGList.SetUnitBitmap(1, sTel, _T(""), TRUE);
        gsTelName = sTel;
    }
    if (gsNet != sNet)
    {
        m_MJPGList.SetUnitBitmap(0, sNet, _T(""), TRUE);
        gsNet = sNet;
    }
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

void CMultimediaPhoneDlg::SwitchPanel_(int panelId)
{
    //lxz 20071218 stocks
    //if(panelId == IDC_BUTTON_CONTACT || panelId == IDC_BUTTON_CONTACT || panelId == IDC_BUTTON_CONTACTINFO || panelId == IDC_BUTTON_SOUND)
    //	return;

    //停止试听音乐
    m_pSettingDlg->OnStopTryRing(0, 0);
    m_pContactGroupDlg->OnStopTryRing(0, 0);
    m_pMainDlg->m_mainLunarderDlg_->OnStopTryRing(0, 0);

    //关闭输入法
    //SipShowIM(SIPF_OFF);
    if (m_pMainDlg->m_pWebDialog->IsWindowVisible())
    {
        m_pMainDlg->m_pWebDialog->ShowWindow_(SW_HIDE);
    }

    if (panelId == 0)
    {
        m_pMainDlg->m_mainVideoDlg_->OnExit_();
        if (m_pMainDlg->m_mainLunarderDlg_->IsWindowVisible())
            m_pMainDlg->m_mainLunarderDlg_->OnExit_();
        SwitchPanel_(IDC_BUTTON_MAIN);
        //		SwitchPanel_(oldcurrentPanelID_);
        return;
    }
    /*
    oldcurrentPanelID_ = currentPanelID_;
    currentPanelID_ = panelId;
    */

    if (currentPanel_ == panels_[IDC_BUTTON_MAIN])	//停止播放
    {
        panels_[IDC_BUTTON_MAIN]->SendMessage(WM_PLAYVIDEO, 0);
    }
    if (panels_[panelId] != currentPanel_)
    {
        /*CWnd *pWnd = */
        //	CWnd *pWnd = currentPanel_->GetFocus();
        //	pWnd->SendMessage(WM_KILLFOCUS, 0, 0);

        /*
        SetButtonDefaultColor(m_pBtnCurrent);
        m_pBtnCurrent = buttons_[panelId];
        SetButtonSelectedColor(m_pBtnCurrent);
        */
        if (currentPanel_ == panels_[IDC_BUTTON_MAIN])
            ((CMainDlg *)currentPanel_)->ShowWindow_(SW_HIDE);
        else if (currentPanel_ == panels_[IDC_BUTTON_INFORMATION])    //20071220 stocks
            ((CInformationDlg*)currentPanel_)->ShowWindow_(SW_HIDE);
        else
            currentPanel_->ShowWindow(SW_HIDE);
        currentPanel_ = panels_[panelId];

        if (currentPanel_ != panels_[IDC_BUTTON_MAIN])   //lxz 20081206切换到主窗口很慢
            ::SetFocus(currentPanel_->m_hWnd);

        if (currentPanel_ == panels_[IDC_BUTTON_MAIN])
            ((CMainDlg *)currentPanel_)->ShowWindow_(SW_SHOW);
        else if (currentPanel_ == panels_[IDC_BUTTON_INFORMATION])   //20071220 stocks
            ((CInformationDlg*)currentPanel_)->ShowWindow_(SW_SHOW);
        else
            currentPanel_->ShowWindow(SW_SHOW);
        /*
        if (panelId == IDC_BUTTON_MAIN)
        {
        m_StatusKey.SetToggleKey(3);
        }
        if (panelId == IDC_BUTTON_CONTACT)
        {
        m_StatusKey.SetToggleKey(4);
        }
        if (panelId == IDC_BUTTON_CONTACTINFO)
        {
        m_StatusKey.SetToggleKey(5);
        }
        if (panelId == IDC_BUTTON_SOUND)
        {
        m_StatusKey.SetToggleKey(6);
        }
        if (panelId == IDC_BUTTON_INFORMATION)
        {
        m_StatusKey.SetToggleKey(7);
        }
        */
    }
    else if (currentPanel_ == panels_[IDC_BUTTON_MAIN])  //多次点击桌面
    {
        //lxz  20080623
        m_pMainDlg->SetMainMenu();
    }

    if (currentPanel_ == panels_[IDC_BUTTON_MAIN])	//恢复播放消息
    {
        panels_[IDC_BUTTON_MAIN]->SendMessage(WM_PLAYVIDEO, 1);
    }

    ///////////////
    m_MJPGList.SetUnitIsDownStatus(2, FALSE);
    m_MJPGList.SetUnitIsDownStatus(3, FALSE);
    m_MJPGList.SetUnitIsDownStatus(4, FALSE);
    m_MJPGList.SetUnitIsDownStatus(5, FALSE);
    m_MJPGList.SetUnitIsDownStatus(6, FALSE);
    m_MJPGList.SetUnitIsDownStatus(7, FALSE);
    if(panelId < IDC_BUTTON_SETTING)
        m_MJPGList.SetUnitIsDownStatus(panelId-1001, TRUE);
    else if(panelId == IDC_BUTTON_SETTING)
        m_MJPGList.SetUnitIsDownStatus(7, FALSE);
    else
        m_MJPGList.SetUnitIsDownStatus(2, FALSE);
    m_MJPGList.Invalidate();
    ///////////////
}

void CMultimediaPhoneDlg::OnButtonMain() 
{
    // TODO: Add your control notification handler code here
    SwitchPanel_(IDC_BUTTON_MAIN);
}

void CMultimediaPhoneDlg::OnButtonContact() 
{
    // TODO: Add your control notification handler code here
    SwitchPanel_(IDC_BUTTON_CONTACT);
}

void CMultimediaPhoneDlg::OnButtonContactinfo() 
{
    // TODO: Add your control notification handler code here
    SwitchPanel_(IDC_BUTTON_CONTACTINFO);
    //lxz 20080617
    //	((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pTelephoneDlg->ClearUnconnectCount();
}

void CMultimediaPhoneDlg::OnButtonSound() 
{
    // TODO: Add your control notification handler code here
    SwitchPanel_(IDC_BUTTON_SOUND);	
    //lxz 20080617
    //	((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pTelephoneDlg->ClearRecordCount();
}

void CMultimediaPhoneDlg::OnButtonInformation() 
{
    // TODO: Add your control notification handler code here
    SwitchPanel_(IDC_BUTTON_INFORMATION);
    //test CEDialog
    //	m_pTestCEDlg->ShowWindow(TRUE);
}

void CMultimediaPhoneDlg::OnButtonSetting() 
{
    // TODO: Add your control notification handler code here
    m_pSettingDlg->SetDiskInFo();
    SwitchPanel_(IDC_BUTTON_SETTING);	
}

void CMultimediaPhoneDlg::OnButtonTime() 
{
    // TODO: Add your control notification handler code here
}

void CMultimediaPhoneDlg::OnButtonIME() 
{
    // TODO: Add your control notification handler code here
}

void CMultimediaPhoneDlg::OnButtonLine() 
{
    // TODO: Add your control notification handler code here
    if (m_pFSM->getCurrentState() != tsHangOff)
    {
        m_pTelephoneDlg->ShowWindow_(TRUE);	
    }
}

BOOL CMultimediaPhoneDlg::PreTranslateMessage(MSG* pMsg) 
{
    // TODO: Add your specialized code here and/or call the base class

    return CDialog::PreTranslateMessage(pMsg);
}

LRESULT CMultimediaPhoneDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    if ((message == WM_TEL_RING)
        || (message == WM_TEL_HUNGON))
    {
        m_pContactInfoDlg->SendMessage(WM_TEL_INUSE, 0, 0);
        m_pSoundDlg->SendMessage(WM_TEL_INUSE, 0, 0);
        m_pSettingDlg->SendMessage(WM_TEL_INUSE, 0, 0);
    }

    switch (message) 
    {
    case WM_LBUTTONDOWN:
        if(!ReStoreBackLight())
            panels_[IDC_BUTTON_MAIN]->SendMessage(WM_PLAYVIDEO, 3); //退出全屏
        break;

    case WM_APP + 88: //WM_PlayEnded:
        {
            static int gCount = 0;
        }
        break;
    case WM_APP+60:
        //Dprintf("0:");
        break;
    case WM_TEL_RING:
        //停止试听音乐
        m_pSettingDlg->OnStopTryRing(0, 0);
        m_pContactGroupDlg->OnStopTryRing(0, 0);
        m_pMainDlg->m_mainLunarderDlg_->OnStopTryRing(0, 0);

        ReStoreBackLight();
        SendOutEvent(WM_TEL_RING, 0);
        //Dprintf("RING \n");
        m_pFSM->fireEvent(teRing, (void * const)wParam);
        break;
    case WM_TEL_HUNGOFF:
        //Dprintf("HUNG OFF \n");
        m_pFSM->fireEvent(teHangOff, (void * const)wParam);
        SendOutEvent(WM_TEL_HUNGOFF, 1);
        m_bIsHungOn = FALSE;
        break;
    case WM_TEL_HUNGON:
        //Dprintf("HUNG ON 0\n");

        //停止试听音乐
        m_pSettingDlg->OnStopTryRing(0, 0);
        m_pContactGroupDlg->OnStopTryRing(0, 0);
        m_pMainDlg->m_mainLunarderDlg_->OnStopTryRing(0, 0);

        ReStoreBackLight();
        //Dprintf("HUNG ON 1\n");
        m_bIsHungOn = TRUE;
        m_pFSM->fireEvent(teHangOn, (void * const)wParam);
        SendOutEvent(WM_TEL_HUNGON, 0);
        break;
    case WM_TEL_CALLIDEND:
        //Dprintf("CALL ID \n");
        m_pFSM->fireEvent(teCallID, (void * const)wParam);
        break;
    case WM_TEL_KEYCODE:
        SendOutEvent(WM_TEL_KEYCODE, 0);
        //Dprintf("%x ", wParam);
        m_pFSM->fireEvent(teKey, (void * const)wParam);
        break;
    case WM_REDIAL:
        //Dprintf("REDIAL \n");
        //lxz 20090207
        //	SendOutEvnet(WM_TEL_HUNGON, 0);
        //	((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pTelephoneDlg->Redial();
        break;
    case WM_MUTE:
        //Dprintf("MUTE \n");
        ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pTelephoneDlg->Mute();
        break;
    case WM_RKEY:
        //Dprintf("R KEY \n");
        break;
    case WM_SPEEDDIAL:
        {
            char c = wParam;
            if (m_pFSM->getCurrentState() == tsHangOn)
            {
                std::map<char, std::string> m = m_pSettingDlg->m_pSetting->speedDials();
                if (m[c] != "")
                {
                    m_pTelephoneDlg->DialContact(Data::TelephoneNumber(m[c]));
                }
            }

        }
        break;
    case WM_HUNGONTORECORD:
        //Dprintf("RECORD ... \n");
        if (m_pFSM->getCurrentState() != tsHangOff)
            ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pTelephoneDlg->OnButtonTelephoneRecord();
        break;
    case WM_HUNGONINRECORD:
        //Dprintf("HUNG ON IN RECORD \n");
        ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pTelephoneDlg->HangOnInRecord();
        break;
    case WM_KEYDOWN:
        {
            ReStoreBackLight();
            SetScreenSaveTimer();

            if(playervideo_->isPlaying_ && m_pMainDlg->m_mainVideoDlg_->IsWindowVisible())
            {
                m_pMainDlg->m_mainVideoDlg_->SendMessage(WM_KEYDOWN, wParam, lParam);
            }
            else if(playerimage_->isPlaying_)
            {
                playerimage_->owner_->SendMessage(WM_KEYDOWN, wParam, lParam);
            }
            else if(playeraudio_->isPlaying_ && m_pMainDlg->m_mainMp3Dlg_->IsWindowVisible())
            {
                m_pMainDlg->m_mainMp3Dlg_->SendMessage(WM_KEYDOWN, wParam, lParam);
            }
        }
    case (WM_USER+1000):
        //Dprintf("InputWnd click\r\n");
        ReStoreBackLight();
        SetScreenSaveTimer();
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
    //lxz 20080620
    //	m_StatusKey.SetEnableKey(2, bEnable);
    //	m_btnLine.EnableWindow(bEnable);
}

void CMultimediaPhoneDlg::AddFSMRules(void)
{
    m_pFSM->registerRule(tsHangOff, teHangOn, tsHangOn, (void (__cdecl *const )(void *)) HangOn);
    m_pFSM->registerRule(tsHangOff, teRing, tsRing, (void (__cdecl *const )(void *)) Ring);

    m_pFSM->registerRule(tsHangOff, teCallID, tsRing, (void (__cdecl *const )(void *)) CallID);

    m_pFSM->registerRule(tsRing, teHangOn, tsConnect, (void (__cdecl *const )(void *)) Connect);
    m_pFSM->registerRule(tsRing, teHangOff, tsHangOff, (void (__cdecl *const )(void *)) HangOff);
    m_pFSM->registerRule(tsRing, teRing, tsRing, (void (__cdecl *const )(void *)) Ring);
    m_pFSM->registerRule(tsRing, teCallID, tsRing, (void (__cdecl *const )(void *)) CallID);	
    //  m_pFSM->registerRule(tsRing, teCallID, tsCallID, (void (__cdecl *const )(void *)) CallID);	
    // 
    //  m_pFSM->registerRule(tsCallID, teHangOn, tsConnect, (void (__cdecl *const )(void *)) Connect);
    //  m_pFSM->registerRule(tsCallID, teHangOff, tsHangOff, (void (__cdecl *const )(void *)) HangOff);
    // 	m_pFSM->registerRule(tsCallID, teRing, tsCallID, (void (__cdecl *const )(void *)) Ring);

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
    //	::KillTimer((theApp.m_pMainWnd)->m_hWnd, 0x123);
    //	::SetTimer((theApp.m_pMainWnd)->m_hWnd, 0x123, 50, NULL);    //挂机   lxz
    ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->OnTimer(0x123);

    // 	::KillTimer((theApp.m_pMainWnd)->m_hWnd, ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_uiKeyTimer);
    // 	((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->EnableLine(FALSE);
    // 	((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pTelephoneDlg->HangOff_(param);
    // 
    // 	//未接接听，自动关闭窗口时模拟挂机事件
    // 	((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->SendOutEvnet(WM_TEL_HUNGOFF, 1);
}

void CMultimediaPhoneDlg::HangOn(void* param)
{
    ::KillTimer((theApp.m_pMainWnd)->m_hWnd, 0x124);
    ::SetTimer((theApp.m_pMainWnd)->m_hWnd, 0x124, 50, NULL);		//摘机

    // 	((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->EnableLine(TRUE);
    // 	((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pTelephoneDlg->HangOn_(param);
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
    ::KillTimer((theApp.m_pMainWnd)->m_hWnd, ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_uiRingTimer);
    ::SetTimer((theApp.m_pMainWnd)->m_hWnd, ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_uiRingTimer, RINGTIMEOUT, 0);

    ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pTelephoneDlg->CallID_(param);
}

void CMultimediaPhoneDlg::OnTimer(UINT nIDEvent) 
{
    // TODO: Add your message handler code here and/or call default
    if (nIDEvent == ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_uiKeyTimer)
    {
        ::KillTimer((theApp.m_pMainWnd)->m_hWnd, ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_uiKeyTimer);
        m_pFSM->fireEvent(teConnect, 0);
    }
    else if (nIDEvent == ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_uiRingTimer)
    {
        ::KillTimer((theApp.m_pMainWnd)->m_hWnd, ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_uiRingTimer);
        // 		if ((m_pFSM->getCurrentState() == tsCallID) || (m_pFSM->getCurrentState() == tsRing))
        if (m_pFSM->getCurrentState() == tsRing)
        {
            m_pFSM->fireEvent(teHangOff, 0);
        }
    }
    else if(nIDEvent == IDT_CONTROLBACKLIGHT_TIME+1)
    {
        //刷新闹铃
        m_pMainDlg->FindTodayAlarm();

        gIsToLight = FALSE;
        SetNightControlBackLightTimer();
    }
    else if(nIDEvent == IDT_CONTROLBACKLIGHT_TIME)		//进入黑屏
    {
        KillTimer(IDT_CONTROLBACKLIGHT_TIME);

        ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->SetBackLight(0);
        //Dprintf("Black BackLight\r\n");
    }
    else if(nIDEvent == IDT_MAIN_SAVESCREEN_TIME)		//进入屏保
    {
        //lxz 20080623
        //return;

        CMainDlg *pWnd = (CMainDlg *)GetPanel(IDC_BUTTON_MAIN);
        HWND hWnd = ::FindWindow(L"csplayer_win2", L"csplayer window2");
        BOOL flag2 = ::IsWindowVisible(hWnd);
        BOOL flag3 = pWnd->m_mainVideoDlg_->IsWindowVisible();
        if(flag2 && flag3)
            return;

        KillTimer(IDT_MAIN_SAVESCREEN_TIME);
        int type = (int)m_pSettingDlg->m_pSetting->screenSaverContent();
        pWnd->m_mainScreenSaveDlg_->SetSaveScreenType(type, TRUE);	  //1 图片屏保   0  时间屏保
        pWnd->SendMessage(WM_CHANGEWINDOW, (WPARAM)6, SW_SHOW);   //切换到屏保
    }
    else if(nIDEvent == IDT_ALARMTIME)		//设置闹铃
    {
        KillTimer(IDT_ALARMTIME);
        Util::shared_ptr<Data::Scheduler> m_result = Data::Scheduler::GetFromDatabaseById(m_nAlarmID);
        SetAlarmList();
        if(m_result)
        {
            std::string content = m_result->what();
            std::string ring = Util::StringOp::FromCString(ringPath); 
            ring += "/" + m_result->ring();
            CString content_ = _T("内容: ");
            content_ += content.c_str();
            CTime tm = m_result->startTime();
            CString sTime;
            sTime.Format(_T("时间: %04d-%02d-%02d %02d:%02d:%02d"), tm.GetYear(), tm.GetMonth(), tm.GetDay(), tm.GetHour(), tm.GetMinute(), tm.GetSecond());
            m_pMainDlg->m_pWebDialog->SendMessage(WM_KILLWEBSHOW, 1, 0);
            m_AlarmShowDlg->SetTxt(m_nAlarmID, sTime, _T("标题: "), content_, ring);
            m_AlarmShowDlg->ShowWindow_(SW_SHOW);
        }
    }
    else if(nIDEvent == 0x123)		//挂机
    {
        ::KillTimer((theApp.m_pMainWnd)->m_hWnd, 0x123);

        ::KillTimer((theApp.m_pMainWnd)->m_hWnd, ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_uiKeyTimer);
        ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->EnableLine(FALSE);
        ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pTelephoneDlg->HangOff_(NULL);

        //未接接听，自动关闭窗口时模拟挂机事件
        ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->SendOutEvent(WM_TEL_HUNGOFF, 1);
    }
    else if(nIDEvent == 0x124)		//摘机
    {
        ::KillTimer((theApp.m_pMainWnd)->m_hWnd, 0x124);

        ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->EnableLine(TRUE);
        ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pTelephoneDlg->HangOn_(NULL);
    }
    else if(nIDEvent == 0x125)
    {
        SetNetTelStatus();
    }

    CDialog::OnTimer(nIDEvent);
}

LRESULT CMultimediaPhoneDlg::OnMainSoftKey(WPARAM w, LPARAM l)
{
    LRESULT result = 0;
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
    return result;
}

void CMultimediaPhoneDlg::SetSkinStyle()
{
}

//振铃 摘机 按键等外部事情发送给 屏保 MP3 VIDEO 听留言窗口
LRESULT CMultimediaPhoneDlg::SendOutEvent(WPARAM w, LPARAM l)
{
    LRESULT result = 0;
    SendMessage(WM_GEN_EVENT);
    CMainDlg *pWnd = (CMainDlg *)GetPanel(IDC_BUTTON_MAIN);
    BOOL isShow = !pWnd->m_mainScreenSaveDlg_->IsWindowVisible();
    switch (w)
    {
    case WM_TEL_KEYCODE:
        {
            pWnd->m_mainScreenSaveDlg_->SendMessage(WM_OUTEVENT, w, isShow);
        }
        break;
    case WM_TEL_RING:
    case WM_TEL_HUNGON:
        {
            pWnd->m_mainScreenSaveDlg_->SendMessage(WM_OUTEVENT, w, isShow);
            pWnd->m_mainVideoDlg_->SendMessage(WM_OUTEVENT, w, l);
            pWnd->m_mainMp3Dlg_->SendMessage(WM_OUTEVENT, w, l);
            pWnd->m_mainPhotoDlg_->SendMessage(WM_OUTEVENT, w, l);
        }
        break;
    case WM_TEL_HUNGOFF:
        {
            pWnd->m_mainVideoDlg_->SendMessage(WM_OUTEVENT, w, l);
            pWnd->m_mainMp3Dlg_->SendMessage(WM_OUTEVENT, w, l);
            pWnd->m_mainPhotoDlg_->SendMessage(WM_OUTEVENT, w, l);

        }
        break;
    case WM_TOUCH_CLICK:
        {
            //pWnd->m_mainScreenSaveDlg_->SendMessage(WM_OUTEVENT, w, isShow);
        }
        break;
    }
    return result;
}

LRESULT CMultimediaPhoneDlg::OnEvent(WPARAM w, LPARAM l)
{
    LRESULT result = 0;
    if (w == 1)
        SendOutEvent(WM_TOUCH_CLICK, 0);
    SetScreenSaveTimer();
    return result;
}

BOOL CMultimediaPhoneDlg::SetNightControlBackLightTimer()
{
    BOOL ret = FALSE;
    BOOL flag2 = m_pSettingDlg->m_pSetting->isNightControlBlackLight();
    BOOL flag1 = m_pSettingDlg->m_pSetting->isContrlBlackLight();
    if (flag2)
    {
        CTime t1 = m_pSettingDlg->m_pSetting->nightControlBlackLightStartTime();
        CTime t2 = m_pSettingDlg->m_pSetting->nightControlBlackLightEndTime();

        SYSTEMTIME sTime;
        GetLocalTime(&sTime);
        CTime tm = CTime(sTime);
        CTime tStart = CTime(tm.GetYear(), tm.GetMonth(), tm.GetDay(), t1.GetHour(), t1.GetMinute(), t1.GetSecond());
        CTime tEnd = CTime(tm.GetYear(), tm.GetMonth(), tm.GetDay(), t2.GetHour(), t2.GetMinute(), t2.GetSecond());
        if (tStart > tEnd)
        {
            if (tm >= tStart || tm <= tEnd)
            {
                SetTimer(IDT_CONTROLBACKLIGHT_TIME, 30*1000, NULL);
                //Dprintf("Set NightControlBackLightTimer %d\r\n", 30000);
                ret = TRUE;
                return ret;
            }
        }
        else 
        {
            if (tm > tStart && tm < tEnd)
            {
                SetTimer(IDT_CONTROLBACKLIGHT_TIME, 30*1000, NULL);
                //Dprintf("Set NightControlBackLightTimer %d\r\n", 30000);
                ret = TRUE;
                return ret;
            }
        }

        if (!gIsToLight)
        {
            gIsToLight = TRUE;	
            KillTimer(IDT_CONTROLBACKLIGHT_TIME);
            if (!flag1) //黑屏唤醒一次
            {
                int v = m_pSettingDlg->m_pSetting->blackLightValue();
                v = 7-v;
                ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->SetBackLight(v);
                //Dprintf("Night BackLight to Light\r\n");
            }
        }
    }
    return ret;
}

void CMultimediaPhoneDlg::SetControlBackLightTimer()
{
    KillTimer(IDT_CONTROLBACKLIGHT_TIME);
    BOOL flag1 = m_pSettingDlg->m_pSetting->isContrlBlackLight();
    if(flag1)
    {
        CMainDlg *pWnd = (CMainDlg *)GetPanel(IDC_BUTTON_MAIN);
        HWND hWnd = ::FindWindow(L"csplayer_win0", L"csplayer window0");
        BOOL flag2 = ::IsWindowVisible(hWnd);
        BOOL flag3 = pWnd->m_mainVideoDlg_->IsWindowVisible();
        if(flag2 && flag3)
            return;

        int nTimer_[] = {60*1000, 5*60*1000, 10*60*1000, 30*60*1000};
        int nTimer = m_pSettingDlg->m_pSetting->contrlBlackLightWaitTime();
        SetTimer(IDT_CONTROLBACKLIGHT_TIME, nTimer[nTimer_], NULL);
        //Dprintf("Set ControlBackLightTimer %d\r\n", nTimer[nTimer_]);
    }
}

BOOL CMultimediaPhoneDlg::CancelBalckLightSaveTimer()
{
    //Dprintf("CancelBalckLightSaveTimer\r\n");
    KillTimer(IDT_MAIN_SAVESCREEN_TIME);
    KillTimer(IDT_CONTROLBACKLIGHT_TIME);
    return TRUE;
}

BOOL CMultimediaPhoneDlg::ReStoreBackLight()
{
    if (m_nBackLightStatus == 0)    //为黑
    {
        SetScreenSaveTimer();

        //恢复背光
        int v = m_pSettingDlg->m_pSetting->blackLightValue();
        v = 7-v;
        if(v == 0)
            v = 1;
        ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->SetBackLight(v);
        return TRUE;
    }
    return FALSE;
}

void CMultimediaPhoneDlg::SetScreenSaveTimer()
{
    //lxz test 20080623
    //return;

    //Dprintf("UnSet ControlBackLightTimer\r\n");
    SetControlBackLightTimer();

    CMainDlg *pWnd = (CMainDlg *)GetPanel(IDC_BUTTON_MAIN);
    HWND hWnd = ::FindWindow(L"csplayer_win0", L"csplayer window0");
    HWND hWnd1 = ::FindWindow(L"csplayer_win2", L"csplayer window2");

    KillTimer(IDT_MAIN_SAVESCREEN_TIME);
    BOOL flag1 = m_pSettingDlg->m_pSetting->isUseScreenSaver();
    BOOL flag2 = ::IsWindowVisible(hWnd);
    BOOL flag3 = pWnd->m_mainVideoDlg_->IsWindowVisible();
    BOOL flag4 = FALSE;
    if (m_pFSM->getCurrentState() != tsHangOff)
        flag4 = TRUE;
    if (flag1)
    {
        CTimeSpan nTimer = m_pSettingDlg->m_pSetting->screenSaverDuration();
        ULONGLONG nSecond = nTimer.GetTotalSeconds();
        if (flag2 && flag3)    //video 
        {
            //Dprintf("UnSet ScreenSave Timer \r\n");
        }
        if (::IsWindowVisible(hWnd1))   //点子相册
        {
            //Dprintf("UnSet ScreenSave Timer \r\n");
        }
        else if (flag4)  //电话正在使用
        {
            //Dprintf("UnSet ScreenSave Timer \r\n");
        }
        else
        {
            SetTimer(IDT_MAIN_SAVESCREEN_TIME, (UINT)nSecond*1000, NULL);
            static int gTimerCount = 0;
            //Dprintf("Set ScreenSave Timer = %d \r\n", gTimerCount++);
        }
    }
    else
    {
        //Dprintf("UnSet ScreenSave Timer \r\n");
    }
}

void CMultimediaPhoneDlg::KillScreenSaveTimer()
{
    KillTimer(IDT_MAIN_SAVESCREEN_TIME);
    //Dprintf("Kill ScreenSave Timer \r\n");
}

LRESULT CMultimediaPhoneDlg::OnDeviceChange(WPARAM w, LPARAM l)
{
    LRESULT result = 0;
    if(m_pMainDlg->m_mainPhotoDlg_)
        m_pMainDlg->m_mainPhotoDlg_->SendMessage(WM_USB_MSG, w, 0);
    if(m_pMainDlg->m_mainMp3Dlg_)
        m_pMainDlg->m_mainMp3Dlg_->SendMessage(WM_USB_MSG, w, 0);
    if(m_pMainDlg->m_mainVideoDlg_)
        m_pMainDlg->m_mainVideoDlg_->SendMessage(WM_USB_MSG, w, 0);
    return result;
}

////////SCL////////////////
/////SCL////////////////////////
void CMultimediaPhoneDlg::InitSCLPort()
{
    m_hPort = CreateFile (_T("SCL1:"), // Pointer to the name of the port
        GENERIC_READ | GENERIC_WRITE, // Access (read-write) mode
        0,            // Share mode
        NULL,         // Pointer to the security attribute
        CREATE_ALWAYS,// Create New File as any case
        0,            // Port attributes
        NULL);
}

BOOL CMultimediaPhoneDlg::GetManuInfo(char *pBuffer, int length)
{
    if (m_hPort != INVALID_HANDLE_VALUE)
    {
        //Get Information;
        unsigned long size;
        if (DeviceIoControl(m_hPort, SCL_GET_PRODUCT_INFO, NULL, 0, pBuffer, 512, &size, NULL))
        {
            return TRUE;
        }
    }
    return FALSE;
}

//设置watchdog
BOOL CMultimediaPhoneDlg::SetWatchDog()
{
    if (m_hPort != INVALID_HANDLE_VALUE)
    {
        unsigned long size;
        if (DeviceIoControl(m_hPort, SCL_ENABLE_WATCHDOG, NULL, 0, NULL, 0, &size, NULL))
        {
            return TRUE;
        }
    }
    return FALSE;
}

//设置ADSL电源开关
BOOL CMultimediaPhoneDlg::SetADSLPower(BOOL isOn)
{
    if (m_hPort != INVALID_HANDLE_VALUE)
    {
        //Dprintf("SetADSLPower %d\n", isOn);

        unsigned long size;
        DWORD inBuff;
        if (isOn)
            inBuff=1;
        else
            inBuff=2;
        DWORD value;
        if (DeviceIoControl(m_hPort, SCL_ADSL_POWERCTL, &inBuff, sizeof(DWORD), &value, sizeof(DWORD), &size, NULL))
        {
            return TRUE;
        }
    }
    return FALSE;
}

//设置系统重启
BOOL CMultimediaPhoneDlg::SetReStart()
{
    if (m_hPort != INVALID_HANDLE_VALUE)
    {
        unsigned long size;
        if(DeviceIoControl(m_hPort, SCL_SYSTEM_RESET, NULL, 0, NULL, 0, &size, NULL))
        {
            return TRUE;
        }
    }
    return FALSE;
}

//关闭打开背光
BOOL CMultimediaPhoneDlg::SetBackLight(int isType)
{
    if (m_nBackLightStatus != isType)
    {
        //todo: 设置
        if (m_hPort != INVALID_HANDLE_VALUE)
        {
            unsigned long size;
            BACKLIGHTCMD blightcmd;
            blightcmd.bkCmd = 0x01;
            blightcmd.bkLevel = isType;
            if(DeviceIoControl(m_hPort, SCL_ADJUST_BACKLIGHT, &blightcmd, sizeof(BACKLIGHTCMD), NULL, 0, &size, NULL))
            {
                m_nBackLightStatus = isType;
                return TRUE;
            }
        }
    }
    return FALSE;
}

//读取背光的光敏值
#define OLDVALUE_MAX   3
UINT32 CMultimediaPhoneDlg::GetBackLightValue()
{
    int ret;
    static int oldblValue[OLDVALUE_MAX] = {0, 0, 0};
    static int oldcount = 0;
    //todo: 设置
    if (m_hPort != INVALID_HANDLE_VALUE)
    {
        unsigned long size;
        BACKLIGHTCMD blightcmd;
        blightcmd.bkCmd = 0x02;
        blightcmd.bkLevel = 0;
        DWORD value;
        if (DeviceIoControl(m_hPort, SCL_ADJUST_BACKLIGHT, &blightcmd, sizeof(BACKLIGHTCMD), &value, sizeof(DWORD), &size, NULL))
        {
            //	Dprintf("back light %d\n", value);	
            oldblValue[oldcount++%OLDVALUE_MAX] = value;
            BOOL flag = FALSE;
            for (int i = 0; i < OLDVALUE_MAX; ++i)
            {
                if (m_nBackLightStatus == 0)   //黑
                {
                    ret = 7;
                    if (oldblValue[i] >= 5000)    //5000
                    {
                        ret = 0;    //黑
                        break;
                    }
                }
                else
                {
                    ret = 0;
                    if (oldblValue[i] < 5000)    //50000
                    {
                        ret = 7;    //亮
                        break;
                    }
                }
            }
        }
    }

    return ret;
}

BOOL CMultimediaPhoneDlg::EnryRun()
{
    if (m_hPort != INVALID_HANDLE_VALUE)
    {
        unsigned long size;
        ENCRYCMD cmd;
        cmd.bkCmd = 0x01;
        cmd.bkRandom = rand();
        DWORD ret;
        if (DeviceIoControl(m_hPort, SCL_ENCRYGPT_VERIFY, &cmd, sizeof(ENCRYCMD), &ret, sizeof(DWORD), &size, NULL))
        {
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }
    return FALSE;
}

UINT32 CMultimediaPhoneDlg::GetEnry()
{
    if (m_hPort != INVALID_HANDLE_VALUE)
    {
        unsigned long size;
        ENCRYCMD cmd;
        cmd.bkCmd = 0x02;
        UINT32 value;
        if (DeviceIoControl(m_hPort, SCL_ENCRYGPT_VERIFY, &cmd, sizeof(ENCRYCMD), &value, sizeof(UINT32), &size, NULL))
        {
            return value;
        }
    }
    return 0;
}


UINT32 CMultimediaPhoneDlg::GetRawBackLightValue()
{
    int ret = 0;
    //todo: 设置
    if (m_hPort != INVALID_HANDLE_VALUE)
    {
        unsigned long size;
        BACKLIGHTCMD blightcmd;
        blightcmd.bkCmd = 0x02;
        blightcmd.bkLevel = 0;
        DWORD value;
        if (DeviceIoControl(m_hPort, SCL_ADJUST_BACKLIGHT, &blightcmd, sizeof(BACKLIGHTCMD), &value, sizeof(DWORD), &size, NULL))
        {
            ret = value;
            //		Dprintf("back light %d\n", value);	
        }
    }

    return ret;
}
//////////////////end SCL/////////////////////////

//alarm setting
void CMultimediaPhoneDlg::SetAlarmList()
{
    //	return;

    KillTimer(IDT_ALARMTIME);

    SYSTEMTIME curtime;
    GetLocalTime(&curtime);
    CTime time = CTime(curtime.wYear, curtime.wMonth, curtime.wDay, 0, 0, 0);

    std::string filter = "startTime BETWEEN '";
    char buff[32];
    memset(buff, 0, 32);
    sprintf(buff, "%04d%02d%02d000000", time.GetYear(), time.GetMonth(),time.GetDay());//, dateFrom.GetHour(), dateFrom.GetMinute(), dateFrom.GetSecond());
    filter += buff;
    filter += "' AND '";
    memset(buff, 0, 32);
    sprintf(buff, "%04d%02d%02d235959", time.GetYear(), time.GetMonth(),time.GetDay());//, dateTo.GetHour(), dateTo.GetMinute(), dateTo.GetSecond());
    filter += buff;
    filter += "'";
    std::string order = "startTime";
    std::vector<Util::shared_ptr<Data::Scheduler> > m_result = Data::Scheduler::GetFromDatabase(filter, order); 
    if (!m_result.empty())
    {
        for (std::size_t i = 0; i < m_result.size(); ++i)
        {
            CTime tm = m_result[i]->startTime();
            GetLocalTime(&curtime);
            CTime nowtm = CTime(curtime.wYear, curtime.wMonth, curtime.wDay, curtime.wHour, curtime.wMinute, curtime.wSecond);
            if (m_result[i]->tipsType() && (nowtm < tm))
            {
                m_nAlarmID = m_result[i]->id();
                int timeout = (tm.GetHour() - curtime.wHour)*3600 + (tm.GetMinute() - curtime.wMinute)*60 + (tm.GetSecond() - curtime.wSecond);
                SetTimer(IDT_ALARMTIME, timeout*1000, NULL);
                break;
            }
        }
    }
}

void PickupFunc()
{
    //Dprintf("\nPickupFunc\n");
}

void HandsfreeFunc()
{
    //Dprintf("\nHandsfreeFunc\n");
}

void HangupFunc()
{
    //Dprintf("\nHangupFunc\n");
}

void KeypadDialFunc(unsigned char const& keyCode)
{
    //Dprintf("\nKeypadDialFunc=%d\n", keyCode);
}

void ConnectFunc()
{
    //Dprintf("\nConnectFunc\n");
}

void RingInFunc(Data::TelephoneNumber number)
{
    //Dprintf("\nRingInFunc=%s\n", number.number().c_str());
}

void RemoteHangupFunc()
{
    //Dprintf("\nRemoteHangupFunc\n");
}

void TimeoutFunc()
{
    //Dprintf("\nTimeoutFunc\n");
}

void ErrorFunc()
{
    //Dprintf("\nErrorFunc\n");
}


#if 0
int CSettingView::DrawImage(CString szFileImage, CRect rt, CDC *pDC) 
{
}
CRect newRect = rect;
if(isOffset)
{
    newRect = CRect(0, 0, rect.Width(), rect.Height());
}

CString sFile = szFileImage;
if(sFile.Find(L".bmp") != -1 || sFile.Find(L".BMP") != -1)
{
    //读bmp文件头的数据  
    DWORD   dwStart   =   GetTickCount(); 	

    CFile   file;   
    CFileException   ex;   
    if(   !file.Open(szFileImage,  CFile::modeRead,   &ex ) )   
    {     

        TCHAR   szError[1024];   
        int error = GetLastError();
        ex.GetErrorMessage(szError,   1024);   
        //	wprintf(L"%s open error %d\n", szFileImage, error );
        //::MessageBox(NULL,szError, L"warning", MB_OK);  
        return;
    }  


    BITMAPFILEHEADER   bfhHeader;   
    file.Read(&bfhHeader,sizeof(BITMAPFILEHEADER));   
    if(bfhHeader.bfType!=((WORD)   ('M'<<8)|'B'))   
    {   
        ::MessageBox(NULL,L"The   file   is   not   a   file!", L"warning",MB_OK);  
        file.Close();
        return;   
    }   
    if(bfhHeader.bfSize!=file.GetLength())     
    {   
        ::MessageBox(NULL,L"The   BMP   file   header   error!", L"warning", MB_OK);   
        file.Close();
        return;   
    }   
    UINT   uBmpInfoLen=(UINT)   bfhHeader.bfOffBits-sizeof(BITMAPFILEHEADER);   
    LPBITMAPINFO   lpBitmap= (LPBITMAPINFO)   new   BYTE[uBmpInfoLen];     
    file.Read((LPVOID)   lpBitmap,uBmpInfoLen);   
    if((*   (LPDWORD)(lpBitmap))!=sizeof(BITMAPINFOHEADER))     
    {   
        ::MessageBox(NULL,L"The   BMP   is   not   Windows   3.0   format!", L"warning",MB_OK);  
        file.Close();
        delete []lpBitmap;
        return;   
    }     
    DWORD   dwBitlen=bfhHeader.bfSize   -   bfhHeader.bfOffBits;     
    LPVOID   lpBits;   
    DWORD offset = GetTickCount() - dwStart;   
    //TRACE(L"file DC 1 %d\n", offset);
    dwStart   =   GetTickCount();

    //如果背景图是 800*420，692*296，535*420，800*60，Unit框小于他们，则取出相应的BMP画出来.否则整个画
    if(((lpBitmap->bmiHeader.biWidth == BMP_WIDTH && lpBitmap->bmiHeader.biHeight == BMP_HEIGHT) && (!(rect.Width() >= BMP_WIDTH && rect.Height() >= BMP_HEIGHT))) ||\
        ((lpBitmap->bmiHeader.biWidth == BMP_WIDTH1 && lpBitmap->bmiHeader.biHeight == BMP_HEIGHT1) && (!(rect.Width() >= BMP_WIDTH1 && rect.Height() >= BMP_HEIGHT1)))||\
        ((lpBitmap->bmiHeader.biWidth == BMP_WIDTH2 && lpBitmap->bmiHeader.biHeight == BMP_HEIGHT2) && (!(rect.Width() >= BMP_WIDTH2 && rect.Height() >= BMP_HEIGHT2))) ||\
        ((lpBitmap->bmiHeader.biWidth == BMP_WIDTH3 && lpBitmap->bmiHeader.biHeight == BMP_HEIGHT3) && (!(rect.Width() >= BMP_WIDTH3 && rect.Height() >= BMP_HEIGHT3)))||\
        ((lpBitmap->bmiHeader.biWidth == BMP_WIDTH4 && lpBitmap->bmiHeader.biHeight == BMP_HEIGHT4) && (!(rect.Width() >= BMP_WIDTH4 && rect.Height() >= BMP_HEIGHT4))))
    {
        ;
    }
    else
    {
        BOOL flag = FALSE;
        if (BMP_WIDTH == lpBitmap->bmiHeader.biWidth || BMP_WIDTH1 == lpBitmap->bmiHeader.biWidth ||BMP_WIDTH2 == lpBitmap->bmiHeader.biWidth ||BMP_WIDTH3 == lpBitmap->bmiHeader.biWidth)
        {
            flag = TRUE;

            //wprintf(szFileImage);
            //Dprintf( " All Draw \n");

            dwStart = GetTickCount();
            int index;
            if (FindMemoryInfo(szFileImage, index))
            {
                if (index >= 0 && index < ALLBMPMEMORY_MAX)
                {
                    BitBlt(pdc->m_hDC, rect.left, rect.top, rect.Width(), rect.Height(), g_allBmpMemory[index].hdc, 0, 0, SRCCOPY);

                    offset = GetTickCount() - dwStart;   
                    if (flag) {
                        ; //Dprintf("Draw ALL BMP %d\n", offset);
                    }
                    return;
                }
            }
        }
        dwStart = GetTickCount();

        file.Close();
        delete []lpBitmap;

        IImage   *pImage   =   NULL; 

        //CoInitializeEx(NULL,   COINIT_MULTITHREADED); 

        //Create   the   imaging   factory. 
        // if (SUCCEEDED(CoCreateInstance   (CLSID_ImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_IImagingFactory, (void   **)&pImgFactory))) 
        { 
            //Load   the   image   from   the   JPG   file. 
            if (SUCCEEDED(pImgFactory-> CreateImageFromFile(szFileImage, &pImage))) 
            { 
                // Draw   the   image. 
                pImage->Draw(pdc->m_hDC, rect, NULL); 
                pImage->Release(); 

                if(flag)
                    AddALLBmpMemoryInfo(szFileImage, rect, pdc);

                ::Sleep(1);
            } 
            else
            {
                //Dprintf("DrawImage error\n");
            }

            //pImgFactory-> Release(); 
        }

        offset = GetTickCount() - dwStart;   
        if(flag) {
            //Dprintf("Draw ALL BMP %d\n", offset);
            //TRACE(L"Draw ALL BMP %d\n", offset);
        }
        return;
    }
    //创建HBITMAP句柄   

    //重新根据rect 创建位图信息

    int w = (((lpBitmap->bmiHeader.biWidth*lpBitmap->bmiHeader.biBitCount)+31)>>5)<<2;
    int h = lpBitmap->bmiHeader.biHeight;
    lpBitmap->bmiHeader.biHeight = rect.Height();
    lpBitmap->bmiHeader.biWidth = rect.Width();

    CDC   dcMem;   
    dcMem.CreateCompatibleDC(pdc);   
    CBitmap   BmpImg;   
    HBITMAP   hBmp = ::CreateDIBSection(dcMem.m_hDC, lpBitmap, DIB_RGB_COLORS, &lpBits, NULL, 0);
    if (lpBits == NULL)
    {
        ::MessageBox(NULL,L"Draw BMP no memory!", L"warning", MB_OK);  
        //return;
    }

    offset = GetTickCount() - dwStart;   
    //TRACE(L"file DC 2 %d\n", offset);
    dwStart   =   GetTickCount();

    //file.ReadHuge(lpBits, dwBitlen);  

    int off = (h-rect.top-rect.Height())*w + rect.left*(lpBitmap->bmiHeader.biBitCount/8);

    file.Seek(off, CFile::current);
    int size = (((rect.Width()*lpBitmap->bmiHeader.biBitCount)+31)>>5)<<2;   //rect.Width()*3;
    int offline = w - size; 

    for (int i = 0; i < rect.Height(); ++i)
    {
        file.Read(lpBits, size); 
        LPBYTE lpbyte = (LPBYTE)lpBits;
        lpbyte += size;
        lpBits = (LPVOID)lpbyte;
        file.Seek(offline, CFile::current);
    }
    file.Close();       

    offset = GetTickCount() - dwStart;   
    //TRACE(L"file DC 3 %d\n", offset);
    dwStart = GetTickCount();

    BmpImg.Attach(hBmp);   

    CBitmap* pOldBmp = dcMem.SelectObject(&BmpImg);   

    offset = GetTickCount() - dwStart;   
    //TRACE(L"file DC 4 %d\n", offset);
    dwStart = GetTickCount(); 

    //pdc->BitBlt(rect.left, rect.top, rect.Width(), rect.Height(), &dcMem, 0, 0, SRCCOPY);
    pdc->BitBlt(newRect.left, newRect.top, newRect.Width(), newRect.Height(), &dcMem, 0, 0, SRCCOPY);

    dcMem.SelectObject(pOldBmp);  
    delete []lpBitmap;
    DeleteObject(hBmp);

    offset = GetTickCount() - dwStart;   
    //TRACE(L"end file DC %d\n", offset);
    ::Sleep(1);

}
else
{
    DWORD dwStart = GetTickCount(); 	

    IImage* pImage = NULL; 

    //CoInitializeEx(NULL,   COINIT_MULTITHREADED); 

    //Create   the   imaging   factory. 
    //if (SUCCEEDED(CoCreateInstance   (CLSID_ImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_IImagingFactory, (void   **)&pImgFactory))) 
    { 
        //Load   the   image   from   the   JPG   file. 
        if (SUCCEEDED(pImgFactory-> CreateImageFromFile(szFileImage, &pImage))) 
        { 
            //   Draw   the   image. 

            pImage->Draw(pdc->m_hDC, rect, NULL); 
            pImage->Release(); 
            ::Sleep(1);
        } 
        else
        {
            //Dprintf("DrawImage error\n");
        }

        //pImgFactory-> Release(); 
    } 
    int offset = GetTickCount() - dwStart;  
    //	wprintf(szFileImage);
    //	Dprintf(" Draw ALL BMP %d\n", offset);
    //	CoUninitialize(); 

}
#endif