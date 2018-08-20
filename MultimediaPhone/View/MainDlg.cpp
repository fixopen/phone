// MainDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MainDlg.h"
#include "../MultimediaPhone.h"
#include "../MultimediaPhoneDlg.h"
#include "../Logical/LunarderDate.h"
#include "../Data/LanguageResource.h"
#include "../Data/SkinStyle.h"
#include "../Util/ScanDir.h"

#include <string>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace View {
    /////////////////////////////////////////////////////////////////////////////
    // CMainDlg dialog

#define TIMER_MAINDLG		0xFFF3

    CMainDlg::CMainDlg(CWnd* pParent /*=NULL*/)
        : CDialog(CMainDlg::IDD, pParent)
    {
        //{{AFX_DATA_INIT(CMainDlg)
        m_mainmenuDlg_ = NULL;
        m_nUnTel = 0;
        m_nLeaveSound = 0;
        m_bIsFireWall = FALSE;
        m_bIsAlarm = FALSE;
        //}}AFX_DATA_INIT
    }

    CMainDlg::~CMainDlg()
    {
        if(m_mainmenuDlg_)
            delete m_mainmenuDlg_;
        m_mainmenuDlg_ = NULL;
    }

    void CMainDlg::DoDataExchange(CDataExchange* pDX)
    {
        CDialog::DoDataExchange(pDX);
        //{{AFX_DATA_MAP(CMainDlg)
        //}}AFX_DATA_MAP
        /*
        DDX_Control(pDX, IDC_STATIC_LINE, m_LineStatic);
        DDX_Control(pDX, IDC_STATIC_TODAY, m_TodayStatic);
        DDX_Control(pDX, IDC_STATIC_TELSTATUS, m_TelStatusStatic);
        DDX_Control(pDX, IDC_STATIC_CALLWALL, m_CallWallStatic);
        DDX_Control(pDX, IDC_STATIC_TIME, m_TimeStatic);
        */
    }

    void CMainDlg::OnActivate( UINT nState, CWnd* pWndOther, BOOL bMinimized )
    {
        if(nState)
        {
            /*
            OnShowCallWallStatic();
            OnShowNoteStatic();
            OnShowTelStatusStatic();
            */
        }
    }


    BEGIN_MESSAGE_MAP(CMainDlg, CDialog)
        //{{AFX_MSG_MAP(CMainDlg)
        ON_WM_PAINT()
        ON_WM_TIMER()
        ON_MESSAGE(WM_CHANGEWINDOW, OnChangeWindow)
        ON_MESSAGE(WM_PLAYVIDEO, OnPlayVideo)
        ON_MESSAGE(WM_PLAYPHOTO, OnPlayPhoto)
        ON_MESSAGE(WM_STATIC_CLICK, OnStaticClick)
        ON_MESSAGE(WM_CLICKMJPG_TOAPP, OnClickMJPGToApp)
        ON_MESSAGE(WM_MJPGSHOWHALF, OnMJPGShowHalf)
        ON_WM_ACTIVATE()
        //}}AFX_MSG_MAP
    END_MESSAGE_MAP()

    /////////////////////////////////////////////////////////////////////////////
    // CMainDlg message handlers
    void CMainDlg::SetCtrlEnable(BOOL flag)
    {
        //	m_LineStatic.EnableWindow(flag);
        //	m_TodayStatic.EnableWindow(flag);
        /*
        m_TelStatusStatic.EnableWindow(flag);
        m_CallWallStatic.EnableWindow(flag);
        m_TimeStatic.EnableWindow(flag);
        */
    }

    BOOL CMainDlg::OnInitDialog() 
    {
        CDialog::OnInitDialog();

        m_mainLunarderDlg_ = new CLunarderDlg;
        m_mainLunarderDlg_->Create(CLunarderDlg::IDD, this);
        m_mainLunarderDlg_->ShowWindow(SW_HIDE);

        m_pWebDialog = new CWebDialog(this);
        m_pWebDialog->Create(CWebDialog::IDD);
        m_pWebDialog->ShowWindow_(SW_HIDE);

        OnShowNoteStatic();
        OnShowCallWallStatic();
        OnShowTelStatusStatic();

        m_mainmenuDlg_ = new CMainMenuDlg;
        m_mainmenuDlg_->Create(CMainMenuDlg::IDD, this);
        m_mainmenuDlg_->ShowWindow_(SW_HIDE);
        m_currentWnd = (CWnd *)m_mainmenuDlg_;

        m_mainVideoDlg_ = new CMainVideoDlg;
        m_mainVideoDlg_->Create(CMainVideoDlg::IDD, this);
        m_mainVideoDlg_->ShowWindow(SW_HIDE);

        m_mainPhotoDlg_ = new CMainPhotoDlg;
        m_mainPhotoDlg_->Create(CMainPhotoDlg::IDD, this);
        m_mainPhotoDlg_->ShowWindow(SW_HIDE);

        m_mainMp3Dlg_ = new CMainMp3Dlg;
        m_mainMp3Dlg_->Create(CMainMp3Dlg::IDD, this);
        m_mainMp3Dlg_->ShowWindow(SW_HIDE);

        m_mainScreenSaveDlg_ = new CScreenSaveDlg;
        m_mainScreenSaveDlg_->Create(CScreenSaveDlg::IDD, this);
        m_mainScreenSaveDlg_->ShowWindow(SW_HIDE);

        m_mainCalucaterDlg_ = new CCalculaterDlg();
        m_mainCalucaterDlg_->Create(CCalculaterDlg::IDD, this);
        m_mainCalucaterDlg_->ShowWindow(SW_HIDE);

        m_firewalDlg_ = new CFireWallDlg();
        m_firewalDlg_->Create(CFireWallDlg::IDD, this);
        m_firewalDlg_->ShowWindow(SW_HIDE);

        m_pHuangLiDlg_ = new CHuangliDlg;
        m_pHuangLiDlg_->Create(CHuangliDlg::IDD, this);
        m_pHuangLiDlg_->ShowWindow(SW_HIDE);

        m_MJPGList.Create(L"", WS_VISIBLE|WS_CHILD, CRect(0*X_XISHU, 0*Y_XISHU, 800*X_XISHU, 420*Y_XISHU), this);
        m_MJPGList.SetCurrentLinkFile(_T(".\\adv\\mjpg\\k1\\中文\\桌面.xml"));
        m_MJPGList.SetMJPGRect(CRect(0*X_XISHU, 0*Y_XISHU, 800*X_XISHU, 420*Y_XISHU));

        SetDateTime(FALSE);
        SetTimer(1, 1000, NULL);
        SetTimer(2, 5000, NULL);

        SetPhotoList();

        return TRUE;  // return TRUE unless you set the focus to a control
        // EXCEPTION: OCX Property Pages should return FALSE
    }

    BOOL CMainDlg::ShowTodayAlarm()
    {
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
                if (m_result[i]->tipsType() == Data::ttAlarmSound && (nowtm > tm))
                {
                    std::string content = m_result[i]->what();
                    std::string ring = Util::StringOp::FromCString(::ringPath); 
                    ring += "/" + m_result[i]->ring();
                    CString content_ = _T("内容: ");
                    content_ += content.c_str();
                    CTime tm = m_result[i]->startTime();
                    CString sTime;
                    sTime.Format(_T("时间: %04d-%02d-%02d %02d:%02d:%02d"), tm.GetYear(), tm.GetMonth(), tm.GetDay(), tm.GetHour(), tm.GetMinute(), tm.GetSecond());
                    m_result[i]->tipsType(Data::ttRead);
                    m_result[i]->Update();
                    ((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->m_AlarmShowDlg->SetTxt(m_result[i]->id(), sTime, _T("标题: "), content_, "");
                    ((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->m_AlarmShowDlg->ShowWindow(SW_SHOW);
                    return TRUE;
                }
            }
        }
        return FALSE;
    }


    BOOL CMainDlg::FindTodayAlarm()
    {
        m_bIsAlarm = FALSE;

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
                if (m_result[i]->tipsType() == Data::ttAlarmSound && (nowtm > tm))
                {
                    m_bIsAlarm = TRUE;
                    return TRUE;
                }
            }
        }
        return FALSE;
    }

    void CMainDlg::SetStatusAll(BOOL flag)
    {
        CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg *)theApp.m_pMainWnd;
        main->m_MJPGList.SetUnitIsDownStatus(2, FALSE);
        main->m_MJPGList.SetUnitIsDownStatus(3, FALSE);
        main->m_MJPGList.SetUnitIsDownStatus(4, FALSE);
        main->m_MJPGList.SetUnitIsDownStatus(5, FALSE);
        main->m_MJPGList.SetUnitIsDownStatus(6, FALSE);
        main->m_MJPGList.SetUnitIsDownStatus(7, FALSE);
        if(!flag)
            main->m_MJPGList.SetUnitIsDownStatus(2, TRUE);
        main->m_MJPGList.Invalidate();
    }

    void CMainDlg::SetDateTime(BOOL isDraw)
    {
        CString sTime;
        SYSTEMTIME curtime;
        GetLocalTime(&curtime);
        static int gHour1, gHour2, gMinute1, gMinute2, gSecond1, gSecond2;
        gHour1=gHour2=gMinute1=gMinute2=gSecond1=gSecond2 = -1;
        CString sFile;
        char filename[64];
        if(gHour1 != curtime.wHour/10)
        {
            sprintf(filename, ".\\adv\\mjpg\\k1\\common\\%d.bmp", curtime.wHour/10);
            sFile = filename;
            m_MJPGList.SetUnitBitmap(12, sFile, _T(""), isDraw);
            gHour1 = curtime.wHour/10;
        }
        if(gHour2 != curtime.wHour%10)
        {
            sprintf(filename, ".\\adv\\mjpg\\k1\\common\\%d.bmp", curtime.wHour%10);
            sFile = filename;
            m_MJPGList.SetUnitBitmap(13, sFile, _T(""), isDraw);
            gHour2 = curtime.wHour%10;
        }
        if(gMinute1 != curtime.wMinute/10)
        {
            sprintf(filename, ".\\adv\\mjpg\\k1\\common\\%d.bmp", curtime.wMinute/10);
            sFile = filename;
            m_MJPGList.SetUnitBitmap(15, sFile, _T(""), isDraw);
            gMinute1 = curtime.wMinute/10;
        }
        if(gMinute2 !=curtime.wMinute%10)
        {
            sprintf(filename, ".\\adv\\mjpg\\k1\\common\\%d.bmp", curtime.wMinute%10);
            sFile = filename;
            m_MJPGList.SetUnitBitmap(16, sFile, _T(""), isDraw);
            gMinute2 =curtime.wMinute%10;
        }

        if(gSecond1 != curtime.wSecond/10)
        {
            sprintf(filename, ".\\adv\\mjpg\\k1\\common\\%d.bmp", curtime.wSecond/10);
            sFile = filename;
            m_MJPGList.SetUnitBitmap(18, sFile, _T(""), isDraw);
            gSecond1 = curtime.wSecond/10;
        }

        sprintf(filename, ".\\adv\\mjpg\\k1\\common\\%d.bmp", curtime.wSecond%10);
        sFile = filename;
        m_MJPGList.SetUnitBitmap(19, sFile, _T(""), isDraw);

        static int nFresh = 0;
        CString sTel;
        CString sRecorde;
        CString sAlarm;
        CString sFireWall;
        m_bIsFireWall = ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pSetting->isFirewall();
        if(nFresh++ % 2)
        {
            if(m_nUnTel > 0)
                sTel = _T(".\\adv\\mjpg\\k1\\common\\未接电话.bmp");
            if(m_nLeaveSound > 0)
                sRecorde = _T(".\\adv\\mjpg\\k1\\common\\未听留言.bmp");
            if(m_bIsFireWall)
                sFireWall = _T(".\\adv\\mjpg\\k1\\common\\防火墙设置.bmp");
            if(m_bIsAlarm)
                sAlarm = _T(".\\adv\\mjpg\\k1\\common\\未看闹铃.bmp");
        }

        static CString gsTel;
        static CString gsRecorde;
        static CString gsAlarm;
        static CString gsFireWall;

        if(gsTel != sTel)
        {
            m_MJPGList.SetUnitBitmap(9, sTel, _T(""), isDraw);
            gsTel = sTel;
        }
        if(gsRecorde != sRecorde)
        {
            m_MJPGList.SetUnitBitmap(8, sRecorde, _T(""), isDraw);
            gsRecorde = sRecorde;
        }
        if(gsAlarm != sAlarm)
        {
            m_MJPGList.SetUnitBitmap(59, sAlarm, _T(""), isDraw);
            gsAlarm = sAlarm;
        }
        if(gsFireWall != sFireWall)
        {
            m_MJPGList.SetUnitBitmap(58, sFireWall, _T(""), isDraw);
            gsFireWall = sFireWall;
        }

        int nWeekDay = Logical::LunarderDate::WeekDay(curtime.wYear, curtime.wMonth, curtime.wDay);
        char txt[64];
        sprintf(txt, "%s %04d-%02d-%02d", Data::LanguageResource::Get(Data::RI_COMN_SUNSTC+nWeekDay).c_str(), curtime.wYear, curtime.wMonth, curtime.wDay);
        sTime = Util::StringOp::ToCString(txt);
        static CString oldTime;
        //if(oldTime != sTime)    //2008
        {
            m_MJPGList.SetUnitText(10, sTime, isDraw);
            oldTime = sTime;
        }
    }

    //add function
    //获取未接电话个数
    int	CMainDlg::GetMissedTelInfo()
    {
        //todo :: Missed finish!
        return 3;
    }
    //获取留言电话个数
    int	CMainDlg::GetRecordedTelInfo()
    {
        //todo :: Missed finish!
        return 2;
    }

    //获取是否设置启动防火墙
    BOOL CMainDlg::GetCallWallInfo()
    {
        //todo :: Missed finish!
        return TRUE;
    }

    void CMainDlg::OnPaint() 
    {
        CPaintDC dc(this); // device context for painting
        // TODO: Add your message handler code here

        // Do not call CDialog::OnPaint() for painting message
    }

    class FindBmpAndJpg : public Util::ProcessFileProcedure {
    public:
        FindBmpAndJpg(CString dir, std::vector<CString>& imageList)
            : ProcessFileProcedure(dir)
            , imageList_(imageList) {
        }
        virtual void operator()(WIN32_FIND_DATA const& FindFileData) {
            imageList_.push_back(directoryName_ + _T("/") + FindFileData.cFileName);
        }
    private:
        std::vector<CString>& imageList_;
    };

    int CMainDlg::SetPhotoList()
    {
        CString dir = ::adPath;
        ProcessFiles(_T("*.bmp"), FindBmpAndJpg(dir, m_PhotoList));
        ProcessFiles(_T("*.jpg"), FindBmpAndJpg(dir, m_PhotoList));
        return 1;
    }
    void CMainDlg::OnTimer(UINT nIDEvent)
    {
        if (IsWindowVisible())
        {
            if (TIMER_MAINDLG == nIDEvent)
            {

            }
            else if (nIDEvent == 1)
            {
                SetDateTime(TRUE);
                //CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg *)theApp.m_pMainWnd;
                //main->SetNetTelStatus();
            }
            else if (nIDEvent == 2)				//左边的广告区
            {
                int size = m_PhotoList.size();
                if (size > 0)
                {
                    static int nAdvIndex = 0; 
                    CString sFile;
                    m_MJPGList.SetUnitBitmap(20, m_PhotoList[nAdvIndex%size], _T(""), TRUE);
                    nAdvIndex++;
                }	
            }
        }
        CDialog::OnTimer(nIDEvent);
    }

    LRESULT CMainDlg::OnChangeWindow(WPARAM w, LPARAM l)
    {
        LRESULT result = 0;
        if (l == SW_HIDE)
        {
            if((CWnd *)w != m_mainmenuDlg_)
            {
                if(w != 0xFFFFFFFF)
                    ((CWnd *)w)->ShowWindow(SW_HIDE);
                m_mainmenuDlg_->SetVideo();
                if((CWnd *)w != (CWnd *)m_mainScreenSaveDlg_ || (m_currentWnd == m_mainmenuDlg_ && IsWindowVisible()))
                {
                    if((CWnd *)w == (CWnd *)m_mainScreenSaveDlg_)
                        m_mainmenuDlg_->ShowWindow_(SW_SHOW, 1);
                    else
                        m_mainmenuDlg_->ShowWindow_(SW_SHOW);
                    //		m_currentWnd = (CWnd *)m_mainmenuDlg_ ;
                }
                else if((CWnd *)w == (CWnd *)m_mainScreenSaveDlg_ && m_currentWnd == m_mainPhotoDlg_ )
                {
                    //将playDlg设置成photoDlg
                    ((CMultimediaPhoneDlg *)GetParent())->playerimage_->SetOwner(m_mainPhotoDlg_->playerDlg_);
                }

                //lxz 0531
                m_mainmenuDlg_->ShowWindow_(SW_HIDE);

                m_mainVideoDlg_->playerDlg_->ShowWindow(SW_HIDE);
                m_mainVideoDlg_->ShowWindow(SW_HIDE);
                m_mainPhotoDlg_->playerDlg_->StopTimer();
                m_mainPhotoDlg_->playerDlg_->ShowWindow(SW_HIDE);
                m_mainPhotoDlg_->ShowWindow(SW_HIDE);
                m_mainMp3Dlg_->ShowWindow(SW_HIDE);
                m_mainLunarderDlg_->ShowWindow(SW_HIDE);
                m_mainScreenSaveDlg_->ShowWindow(SW_HIDE);
                m_mainCalucaterDlg_->ShowWindow(SW_HIDE);
                m_pWebDialog->ShowWindow_(SW_HIDE);	
                m_MJPGList.ShowWindow(SW_SHOW);

                SetStatusAll(FALSE);
            }
        }
        else if(l == SW_SHOW)
        {
            /*
            if(m_currentWnd == m_mainVideoDlg_)
            m_mainVideoDlg_->OnExit_();
            else if(m_currentWnd != m_mainScreenSaveDlg_ )
            m_currentWnd->ShowWindow(SW_HIDE);
            */
            m_mainmenuDlg_->ShowWindow_(SW_HIDE);
            if(w == 0)	//显示桌面影院
            {
                std::string videoFilename = Util::StringOp::FromCString(::videoPath);
                videoFilename += "/playlist.pls";
                m_mainVideoDlg_->SetVideo((char*)videoFilename.c_str());
                m_mainVideoDlg_->ShowWindow(SW_SHOW);
                m_currentWnd = (CWnd *)m_mainVideoDlg_ ;
            }
            else if(w == 1)	//显示Photo
            {
                m_mainPhotoDlg_->SetPhoto();
                m_mainPhotoDlg_->ShowWindow(SW_SHOW);
                m_currentWnd = (CWnd *)m_mainPhotoDlg_ ;
            }
            else if(w == 2)	//显示Mp3
            {
                m_mainMp3Dlg_->SetMP3("");
                m_mainMp3Dlg_->ShowWindow(SW_SHOW);
                m_currentWnd = (CWnd *)m_mainMp3Dlg_ ;
            }
            else if(w == 3)		//显示日历
            {
                m_mainLunarderDlg_->ShowWindow(SW_SHOW);
                m_currentWnd = (CWnd *)m_mainLunarderDlg_ ;
            }
            else if(w == 4)
            {
                m_mainCalucaterDlg_->ShowWindow(SW_SHOW);
                m_currentWnd = (CWnd *)m_mainCalucaterDlg_ ;
            }
            else if(w == 5)	//显示屏保
            {
                //0530 lxz
                //	m_pWebDialog->SetPlayList(_T("\\storagecard\\"), 1);
                //	m_pWebDialog->ShowWindow(SW_SHOW);
                //	m_mainScreenSaveDlg_->ShowWindow(SW_SHOW);
                //	StartWeb();
            }
            else if(w == 6) //显示屏保
            {
                //lxz 20090207
                if(m_currentWnd == m_mainVideoDlg_)
                    m_mainVideoDlg_->OnExit_();
                else if(m_currentWnd == m_mainPhotoDlg_)
                    m_mainPhotoDlg_->OnExit_();

                m_mainScreenSaveDlg_->ShowWindow(SW_SHOW);
            }
            //::SetWindowPos(m_currentWnd->m_hWnd, HWND_TOP, 0, 0, 535, 420, SWP_NOMOVE);
            //m_currentWnd->Invalidate();
            //m_MJPGList.ShowWindow(SW_SHOW);
        }
        return result;
    }

    LRESULT CMainDlg::OnPlayPhoto(WPARAM w, LPARAM l)
    {
        LRESULT result = 0;
        CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg *)theApp.m_pMainWnd;
        if(m_mainPhotoDlg_->IsWindowVisible())
        {	
            //	if(!main->playerimage_->type_)		//图片
            {
                if(w == 2)	//全屏播放
                {
                    if(main->playerimage_->isPlaying_ )
                    {
                        m_mainPhotoDlg_->SetAllScreenPlayer(TRUE);
                    }
                }
                else if(w == 3) //恢复播放
                {
                    if(main->playerimage_->isPlaying_)
                    {
                        m_mainPhotoDlg_->SetAllScreenPlayer(FALSE);
                    }
                }
            }
        }
        else if(m_mainScreenSaveDlg_->IsWindowVisible())
        {
            if(w == 3)	//退出屏保
            {
                if(main->playerimage_->isPlaying_ )
                {
                    m_mainScreenSaveDlg_->OnExit();
                }
            }
        }
        return result;
    }

    void CMainDlg::SetTimer_(BOOL flag)
    {
        if(flag)
        {
            //开启定时器
            SetTimer(1, 1000, NULL);
            SetTimer(2, 5000, NULL);
        }
        else
        {
            KillTimer(1);
            KillTimer(2);
        }
    }

    LRESULT CMainDlg::OnPlayVideo(WPARAM w, LPARAM l)
    {
        LRESULT result = 0;
        CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg *)theApp.m_pMainWnd;
        {

            if (w == 0)	//暂停视频
            {
                //视频暂停
                if(main->playervideo_->isPlaying_)
                    m_mainVideoDlg_->SendMessage(WM_OUTEVENT, 0, 0);
                if(main->playerimage_->isPlaying_)
                    m_mainPhotoDlg_->SendMessage(WM_OUTEVENT, 0, 0);
                if(m_mainPhotoDlg_->IsWindowVisible())
                    m_mainPhotoDlg_->SendMessage(WM_OUTEVENT, 0, 0);
            }
            else if (w == 1) //播放视频
            {
                {
                    if(m_mainMp3Dlg_->IsWindowVisible())
                    {
                        //main->playeraudio_->InitPlayer();
                    }
                    else if(m_mainVideoDlg_->IsWindowVisible())
                    {
                        m_mainVideoDlg_->SendMessage(WM_OUTEVENT, 0, 1);						
                    }
                }
            }
            else if(w == 2)	//全屏播放
            {
                if(main->playervideo_->isPlaying_)
                {
                    main->playervideo_->SetAllScreenPlayer(TRUE);
                }
                if(main->playerimage_->isPlaying_)
                    main->playerimage_->SetAllScreenPlayer(TRUE);
            }
            else if(w == 3)	//退出全屏
            {
                if(m_mainScreenSaveDlg_->playerDlg_->player_->owner_ == m_mainScreenSaveDlg_->playerDlg_ && m_mainScreenSaveDlg_->playerDlg_->player_->isPlaying_)
                    m_mainScreenSaveDlg_->SendMessage(WM_OUTEVENT, 0, 0);
                if(main->playervideo_->isPlaying_)
                {
                    main->playervideo_->SetAllScreenPlayer(FALSE);
                }
                if(m_mainPhotoDlg_->playerDlg_->player_->owner_ == m_mainPhotoDlg_->playerDlg_  && main->playerimage_->isPlaying_)
                    m_mainPhotoDlg_->SetAllScreenPlayer(FALSE);
            }
            else if(w == 4 ) //文件开始播放
            {
                //有问题
                /*
                if(m_mainVideoDlg_->IsWindowVisible() )//|| m_mainVideoDlg_->IsWindowVisible())	//视频
                {
                m_mainVideoDlg_->StartOpenNewFile();
                }
                else if(m_mainMp3Dlg_->IsWindowVisible())
                {
                m_mainMp3Dlg_->StartOpenNewFile();
                }
                */
            }
        }
        return result;
    }

    LRESULT CMainDlg::OnStaticClick(WPARAM w, LPARAM l)
    {
        LRESULT result = 0;
        if(w == IDC_STATIC_CALLWALL)		//防火墙
        {
            m_firewalDlg_->OnStart();
            m_firewalDlg_->ShowWindow(TRUE);
        }
        else if(w == IDC_STATIC_TELSTATUS)	//未接电话、留言电话
        {
            CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg *)theApp.m_pMainWnd;
            //todo: 进入未读电话记录
            main->m_pContactInfoDlg->ShowUnconnectItems();
            main->m_pContactInfoDlg->ShowWindow(SW_SHOW);
            //lxz 20080617
            //	((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pTelephoneDlg->ClearUnconnectCount();
        }
        else if(w == IDC_STATIC_NOTETATUS)		//今天是否有记事
        {	
            SYSTEMTIME curtime;
            GetLocalTime(&curtime);
            //		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_mainLunarderDlg1_->SetIn();		//是否需要暂停视频播放
            //		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_mainLunarderDlg1_->SetData(curtime.wYear, curtime.wMonth, curtime.wDay);
            //		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->SwitchPanel_(IDC_BUTTON_MAINNOTE);
        }
        return result;
    }

    void CMainDlg::OnShowNoteStatic()
    {

    }

    void CMainDlg::OnShowCallWallStatic()
    {

    }

    void CMainDlg::OnShowTelStatusStatic()
    {
        //todo: 获取未接电话条数
        CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg *)theApp.m_pMainWnd;
        m_nUnTel = main->m_pTelephoneDlg->GetUnconnectCount();
        m_nLeaveSound = main->m_pTelephoneDlg->GetRecordCount();
    }

    LRESULT CMainDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
    {
        // TODO: Add your specialized code here and/or call the base class
        switch (message)
        {
        case WM_TELNOTIFY:
            OnShowCallWallStatic();
            OnShowNoteStatic();
            OnShowTelStatusStatic();
            break;
        case WM_FIRENOTIFY:
            OnShowCallWallStatic();
            break;
        }

        return CDialog::WindowProc(message, wParam, lParam);
    }

    void CMainDlg::ShowWindow_(int nCmdShow)
    {
        SetTimer_(FALSE);

        //主界面来回切换 main不显示bug
        if (m_currentWnd == m_mainmenuDlg_ /* && nCmdShow == SW_SHOW*/)
        {
            //		m_mainmenuDlg_->ShowWindow_(nCmdShow);
        }
        else if (m_currentWnd == m_mainVideoDlg_)
        {
            HWND hWnd = ::FindWindow(L"csplayer_win0", L"csplayer window0");
            BOOL flag2 = ::IsWindowVisible(hWnd);
            if(flag2 && nCmdShow == SW_SHOW)
                m_mainVideoDlg_->playerDlg_->ShowWindow(nCmdShow);
            else if(nCmdShow == SW_HIDE)
                m_mainVideoDlg_->playerDlg_->ShowWindow(nCmdShow);
            m_mainVideoDlg_->ShowWindow(nCmdShow);
        }
        else if (m_currentWnd == m_mainPhotoDlg_)
        {
            m_mainPhotoDlg_->playerDlg_->ShowWindow(nCmdShow);
            m_mainPhotoDlg_->ShowWindow(nCmdShow);
        }

        DWORD dwStart   =   GetTickCount();

        ShowWindow(nCmdShow);

        DWORD offset = GetTickCount() - dwStart; 

        //显示 MJPG
        if (nCmdShow == SW_SHOW)
        {
            m_mainmenuDlg_->ShowWindow_(SW_HIDE);
            m_mainVideoDlg_->ShowWindow(SW_HIDE);
            m_mainPhotoDlg_->playerDlg_->ShowWindow(SW_HIDE);
            m_mainPhotoDlg_->ShowWindow(SW_HIDE);
            m_mainMp3Dlg_->ShowWindow(SW_HIDE);
            m_mainLunarderDlg_->ShowWindow(SW_HIDE);
            m_mainScreenSaveDlg_->ShowWindow(SW_HIDE);
            m_mainCalucaterDlg_->ShowWindow(SW_HIDE);
            m_pWebDialog->ShowWindow_(SW_HIDE);	
            ((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->m_pNotebookDlg->ShowWindow(SW_HIDE);
            m_MJPGList.ShowWindow(SW_SHOW);
        }
        SetTimer_(TRUE);
    }

    void CMainDlg::SetMainMenu()
    {
        ((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->m_pNotebookDlg->ShowWindow(SW_HIDE);
        m_firewalDlg_->ShowWindow(SW_HIDE);
        m_pHuangLiDlg_->ShowWindow(SW_HIDE);
        if(m_currentWnd == m_mainVideoDlg_)
            m_mainVideoDlg_->OnExit_();
        else if(m_currentWnd == m_mainMp3Dlg_)
            m_mainMp3Dlg_->OnExit_(FALSE);
        else if(m_currentWnd == m_mainPhotoDlg_)
            m_mainPhotoDlg_->OnExit_();
        else if(m_currentWnd == m_mainLunarderDlg_)
        {
            m_mainLunarderDlg_->OnExit_();
            ShowWindow(SW_SHOW);
        }
        else if(m_currentWnd == m_mainCalucaterDlg_)
            m_mainCalucaterDlg_->OnExit_();
        //	else if(m_currentWnd == m_pWebDialog)
        //	{
        //		m_pWebDialog->OnButtonClose();
        //	}
        else if(m_MJPGList.IsWindowVisible())
        {
            CString s = _T(".\\adv\\mjpg\\k1\\中文\\桌面.xml");
            m_MJPGList.SetCurrentLinkFile(s);
            SetDateTime(FALSE);
            m_MJPGList.DrawMJPGPage(s);
        }
    }

    LRESULT CMainDlg::OnClickMJPGToApp(WPARAM w, LPARAM l)
    {
        LRESULT result = 0;

        CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg *)theApp.m_pMainWnd;
        //	m_MJPGList.ShowWindow(SW_HIDE);   //lxz 20080602
        switch (w)
        {
        case 1:					//公司简介
            {
                m_mainmenuDlg_->OnMp3Btn();	
            }
            break;
        case 2:									
            m_mainmenuDlg_->OnVideoBtn();
            break;
        case 3:
            m_mainmenuDlg_->OnPhotoBtn();
            break;
        case 4:			//防火墙

            m_firewalDlg_->SetParameters(((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pSetting->isFirewall(),
                ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pSetting->firewallType(),
                (int)((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pSetting->blockAllTimeDuration().GetTotalSeconds());
            m_firewalDlg_->ShowWindow(SW_SHOW);
            break;
        case 5:							
            m_mainmenuDlg_->OnLunarderBtn();    //日程
            break;
        case 6:								//
            m_mainmenuDlg_->OnCalculBtn();
            break;
        case 7:
            StartWeb();    //20081112  	//上网
            break;
        case 8:									//便笺
            {
                ((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->m_pNotebookDlg->ShowWindow(SW_SHOW);
            }
            break;
        case 9:								
            {
                SYSTEMTIME curtime;
                GetLocalTime(&curtime);
                m_pHuangLiDlg_->SetData(curtime.wYear, curtime.wMonth, curtime.wDay, TRUE);
                m_pHuangLiDlg_->ShowWindow(SW_SHOW);    //老黄历
            }
            break;
        case 10:										//显示未接电话
            {
                CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg *)theApp.m_pMainWnd;
                //todo: 进入未读电话记录
                main->m_pContactInfoDlg->ShowUnconnectItems();
                main->m_pContactInfoDlg->ShowWindow(SW_SHOW);
            }
            break;
        case 11:										//显示录音电话
            {
                CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg *)theApp.m_pMainWnd;
                main->m_pSoundDlg->ShowItemsInList(0);
                main->OnMainSoftKey(7, 0);
            }
            break;
        case 30:		//闹铃
            {
                if(m_bIsAlarm)
                {
                    SetMainMenu();
                    ShowTodayAlarm();
                }
                break;
            }
        case 31:	//防火墙
            {
                if(m_bIsFireWall)
                {
                    SetMainMenu();
                    OnClickMJPGToApp(4, 0);
                }
                break;
            }
        default:
            break;
        }

        if(w >= 1 && w <= 9)
            SetStatusAll(TRUE);

        m_MJPGList.m_nIndexSelectUnit = -1;
        return result;
    }

    void CMainDlg::ShowRightCtrl(int nCmdShow)
    {
    }

    LRESULT CMainDlg::OnMJPGShowHalf(WPARAM w, LPARAM l)
    {
        LRESULT result = 0;
        if(w == 0)
            ShowRightCtrl(SW_HIDE);
        else
            ShowRightCtrl(SW_SHOW);
        return result;

    }

    void  CMainDlg::StartWeb()
    {
        MEMORYSTATUS mem;     
        mem.dwLength = sizeof(mem);     
        GlobalMemoryStatus(&mem);     

        if (m_pWebDialog)
        {
            m_pWebDialog->ShowWindow_(SW_SHOW);
        }
    }
}
