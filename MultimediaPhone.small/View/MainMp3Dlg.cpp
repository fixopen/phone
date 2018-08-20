// MainMp3Dlg.cpp : implementation file
//
#include "stdafx.h"
#include "player.h"
#include "MainMp3Dlg.h"

#include "../MultimediaPhoneDlg.h"
#include "../MultimediaPhone.h"
#include "../Pblmember.h"
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
    // CMainMp3Dlg dialog

    CMainMp3Dlg::CMainMp3Dlg(CWnd* pParent /*=NULL*/)
        : CDialog(CMainMp3Dlg::IDD, pParent)
    {
        //{{AFX_DATA_INIT(CMainMp3Dlg)
        // NOTE: the ClassWizard will add member initialization here
        //}}AFX_DATA_INIT
        memset(m_chDir, 0, 128*2);
        m_IsPlay = 0;
        m_IsSound = TRUE;
        m_Volume = SOUND_DEFAULT_VOLUME;
        m_bIsPausebyEvent = FALSE;
    }


    void CMainMp3Dlg::DoDataExchange(CDataExchange* pDX)
    {
        CDialog::DoDataExchange(pDX);
        //{{AFX_DATA_MAP(CMainMp3Dlg)
        // NOTE: the ClassWizard will add DDX and DDV calls here
        //}}AFX_DATA_MAP
    }


    BEGIN_MESSAGE_MAP(CMainMp3Dlg, CDialog)
        //{{AFX_MSG_MAP(CMainMp3Dlg)
        ON_BN_CLICKED(IDC_BUTTON_MP3EXIT, OnExit)
        ON_BN_CLICKED(IDC_BUTTON_MP3ALL, OnSelectAll)
        ON_BN_CLICKED(IDC_BUTTON_MP3CLEAR, OnClearAll)
        ON_BN_CLICKED(IDC_BUTTON_MP3PLAY, OnPlayer)
        ON_BN_CLICKED(IDC_BUTTON_MP3PRE, OnPre)
        ON_BN_CLICKED(IDC_BUTTON_MP3BACK, OnBack)
        ON_BN_CLICKED(IDC_BUTTON_MP3OPEN, OnOpenFile)
        ON_BN_CLICKED(IDC_BUTTON_MP3MUTE, OnMute)
        ON_MESSAGE(WM_PROCESS_POS, OnPregress)
        //	ON_NOTIFY(NM_CLICK, IDC_LIST_MP3LIST, OnClickPlayList)
        ON_WM_TIMER()
        ON_MESSAGE(WM_USB_MSG, OnDeviceChange)
        ON_WM_ACTIVATE()
        ON_MESSAGE(WM_OUTEVENT, OnOutEvent)
        ON_MESSAGE(WM_CLICKMJPG_TOAPP, OnClickMJPG)
        ON_MESSAGE(WM_LISTCTRL_CLICK, OnListCtrlClick)
        ON_WM_KEYDOWN()
        //}}AFX_MSG_MAP
    END_MESSAGE_MAP()

    void CMainMp3Dlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
    {
        if (nChar == 'U')
        {
            OnPre();
        }
        else if (nChar == 'D')
        {
            OnBack();
        }
    }

    LRESULT CMainMp3Dlg::OnListCtrlClick(WPARAM w, LPARAM l)
    {
        LRESULT result = 0;
        if (m_IsPlay == 0)
        {
            LRESULT ret;
            OnClickPlayList(NULL, &ret);
        }
        return result;
    }

    void CMainMp3Dlg::OnActivate( UINT nState, CWnd* pWndOther, BOOL bMinimized )
    {
        if(nState & WA_ACTIVE )
        {
            SetPlayList((TCHAR*)(LPCTSTR)::musicPath, 0);
        }
    }


    BOOL CMainMp3Dlg::OnInitDialog() 
    {
        CDialog::OnInitDialog();

        // TODO: Add extra initialization here

        // 	m_prgPlayTime.Create(WS_CHILD|WS_VISIBLE, CRect(32, 286, 32+11, 286+120), this, IDC_PROGRESS_MP3TIME);
        // 	m_prgPlayTime.SetParam(0, 0, 10, 1, 3, IDB_BITMAP_PROGRESS_BAR2, IDB_BITMAP_PROGRESS_BAR1);

        m_prgPlayTime.Create(WS_CHILD|WS_VISIBLE, CRect(5, 268, 5+393, 268+16), this, IDC_PROGRESS_MP3TIME);
        m_prgPlayTime.SetParam(IDB_PROGRESS_TIME_THINCK, 0, 10, 1, 1, IDB_PROGRESS_TIME1, IDB_PROGRESS_TIME2);


        m_prgPlayTime.SetPos(0, FALSE);
        m_prgPlayTime.SetBackRGB(Data::g_allFramBackRGB[Data::g_skinstyle]);

        m_prgSoundSelect.Create(WS_CHILD|WS_VISIBLE, CRect(498, 313, 498+27, 313+90), this,IDC_PROGRESS_MP3SOUND);
        m_prgSoundSelect.SetParam(IDB_PROGRESS_VOLUMETHINCK, 0, 10, 1, 3, IDB_PROGRESS_VOLUME1, IDB_PROGRESS_VOLUME1);
        m_prgSoundSelect.SetBackRGB(Data::g_allFramBackRGB[Data::g_skinstyle]);
        SetVolume();

        m_lstPlayList.Create(WS_CHILD|WS_VISIBLE|LVS_REPORT|LVS_NOCOLUMNHEADER|LVS_NOSORTHEADER, CRect(7, 50, 7+522, 50+213), this, IDC_LIST_MP3LIST, TRUE, 1);
        m_lstPlayList.SetListColor(Data::g_listctrlBackRGB1[Data::g_skinstyle], Data::g_listctrlBackRGB2[Data::g_skinstyle]);
        m_lstPlayList.InsertColumn(0, _T("Filename"), LVCFMT_LEFT, 480);

        m_lstPlayList1.Create(WS_CHILD|WS_VISIBLE|LVS_REPORT|LVS_NOCOLUMNHEADER|LVS_NOSORTHEADER, CRect(7, 50, 7+522, 50+213), this, 0xFFFF, TRUE, 1);
        m_lstPlayList1.SetListColor(Data::g_listctrlBackRGB1[Data::g_skinstyle], Data::g_listctrlBackRGB2[Data::g_skinstyle]);
        m_lstPlayList1.InsertColumn(0, _T("Filename"), LVCFMT_LEFT, 480);
        m_lstPlayList1.ShowWindow_(SW_HIDE);

        m_pImageList = new CImageList();
        m_pImageList->Create(32, 32, ILC_COLOR32|ILC_MASK, 5, 5);   

        CBitmap bm;
        bm.LoadBitmap(Data::g_listctrlSelectBMPID[1][Data::g_skinstyle]);
        m_pImageList->Add(&bm, RGB(255, 0, 255)); 
        bm.DeleteObject();
        bm.LoadBitmap(Data::g_listctrlSelectBMPID[0][Data::g_skinstyle]);
        m_pImageList->Add(&bm, RGB(255, 0, 255)); 
        bm.DeleteObject();
        bm.LoadBitmap(Data::g_listctrlSelectBMPID[2][Data::g_skinstyle]);
        m_pImageList->Add(&bm, RGB(255, 0, 255)); 
        bm.DeleteObject();
        bm.LoadBitmap(Data::g_listctrlSelectBMPID[3][Data::g_skinstyle]);
        m_pImageList->Add(&bm, RGB(255, 0, 255)); 
        bm.DeleteObject();
        bm.LoadBitmap(Data::g_listctrlSelectBMPID[4][Data::g_skinstyle]);
        m_pImageList->Add(&bm, RGB(255, 0, 255)); 
        bm.DeleteObject();
        m_lstPlayList.SetImageList(m_pImageList, LVSIL_SMALL);

        SetPlayList((TCHAR*)(LPCTSTR)::musicPath, 0);
        m_lstPlayList.SetScrollRagle(FALSE);

        //播放器
        CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
        playerDlg_ = new CPlayerDlg(main->playeraudio_);
        playerDlg_->Create(CPlayerDlg::IDD, this);
        playerDlg_->MoveWindow(CRect(15, 28, 15, 28), FALSE);

        m_MJPGList.Create(L"", WS_CHILD|WS_VISIBLE, CRect(0, 0, 535, 420), this);
        m_MJPGList.SetCurrentLinkFile(_T(".\\adv\\mjpg\\k1\\中文\\mp3.xml"));
        m_MJPGList.SetMJPGRect(CRect(0, 0, 535, 420));

        m_MJPGList.SetUnitIsShow(3, TRUE);
        m_MJPGList.SetUnitIsShow(4, TRUE);
        m_MJPGList.SetUnitIsShow(11, FALSE);
        m_MJPGList.SetUnitIsShow(10, FALSE);
        return TRUE;  // return TRUE unless you set the focus to a control
        // EXCEPTION: OCX Property Pages should return FALSE
    }

    void CMainMp3Dlg::SetMP3(char *filename)
    {
        playerDlg_->SetParam( /*"/flashdrv/my_music/playlist.pls"*/filename , mtAudio);
        playerDlg_->ReSetWindowsRect(CRect(0, 0, 0, 0));
        m_bIsPausebyEvent = FALSE;
    }

    class FindMp3 : public Util::ProcessFileProcedure {
    public:
        FindMp3(CString dir, CCEListCtrl& audioList)
            : ProcessFileProcedure(dir)
            , audioList_(audioList) {
        }
        virtual void operator()(WIN32_FIND_DATA const& FindFileData) {
            int itemIndex = 0;
            int imageIndex = 0;
            if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                imageIndex = 2;
            } else {
                imageIndex = 0;
            }
            audioList_.InsertItem(itemIndex, FindFileData.cFileName, imageIndex);
            ++itemIndex;
        }
    private:
        CCEListCtrl& audioList_;
    };

    void CMainMp3Dlg::SetPlayList(TCHAR* cdir, int local)
    {
        m_lstPlayList.DeleteAllItems();
        wcscpy(m_chDir, cdir);
        CString dir = cdir;
        ProcessFiles(_T("*.MP3"), FindMp3(dir, m_lstPlayList));
        ProcessFiles(_T("*.WAV"), FindMp3(dir, m_lstPlayList));
        //ProcessFiles(_T("*.mp3"), FindMp3(dir, m_lstPlayList));
        m_lstPlayList.SetScrollRagle(TRUE);
    }

    void CMainMp3Dlg::OnExit_(BOOL isStopMusic)
    {
        CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
        if(isStopMusic)
        {
            main->playeraudio_->ExitPlayer();
            KillTimer(IDT_GETINFO_TIIMER);
            m_lstPlayList1.ShowWindow_(SW_HIDE);
            m_lstPlayList.ShowWindow_(SW_SHOW);

            playerDlg_->ShowWindow(SW_HIDE);
            m_MJPGList.SetUnitIsShow(3, TRUE);
            m_MJPGList.SetUnitIsShow(4, TRUE);
            m_MJPGList.SetUnitIsShow(11, FALSE);
            m_MJPGList.SetUnitIsShow(10, FALSE);
            m_MJPGList.SetUnitIsDownStatus(0, FALSE);
            m_prgPlayTime.SetPos(0);

            m_IsPlay = 0;
        }

        if(IsWindowVisible())	
            GetParent()->SendMessage(WM_CHANGEWINDOW, (WPARAM)this, (LPARAM)SW_HIDE);
    }

    void CMainMp3Dlg::OnExit()
    {
        OnExit_(TRUE);
    }

    void CMainMp3Dlg::OnSelectAll()
    {
        int ncount = m_lstPlayList.GetItemCount();

        LVITEM lvitem;
        lvitem.mask=LVIF_TEXT   |   LVIF_IMAGE;   

        for(int i = 0; i < ncount; i++)
        {
            lvitem.iItem=i;   
            lvitem.iSubItem=0;   
            lvitem.pszText = m_lstPlayList.GetItemText(i, 0).GetBuffer(128);  
            CString s = lvitem.pszText;
            int n = s.Find(_T(".mp3"));
            int n1 = s.Find(_T(".MP3"));
            int n2 = s.Find(_T(".wav"));
            int n3 = s.Find(_T(".WAV"));
            if((n1 == -1) && (n == -1) && (n2 == -1) && (n3 == -1))
                continue;
            lvitem.lParam=i;   
            lvitem.iImage=1;
            m_lstPlayList.SetItem(&lvitem);
            m_lstPlayList.SetItemData(i, 1);
        }
    }

    void CMainMp3Dlg::OnClearAll()
    {
        int ncount = m_lstPlayList.GetItemCount();

        LVITEM lvitem;
        lvitem.mask=LVIF_TEXT   |   LVIF_IMAGE;   

        for(int i = 0; i < ncount; i++)
        {
            lvitem.iItem=i;   
            lvitem.iSubItem=0;   
            lvitem.pszText = m_lstPlayList.GetItemText(i, 0).GetBuffer(128);  
            CString s = lvitem.pszText;
            int n = s.Find(_T(".mp3"));
            int n1 = s.Find(_T(".MP3"));
            int n2 = s.Find(_T(".wav"));
            int n3 = s.Find(_T(".WAV"));
            if((n1 == -1) && (n == -1) && (n2 == -1) && (n3 == -1))
                continue;
            lvitem.lParam=i;   
            lvitem.iImage=0;
            m_lstPlayList.SetItem(&lvitem);
            m_lstPlayList.SetItemData(i, 0);
        }
    }

    void CMainMp3Dlg::OnClickPlayList(NMHDR* pNMHDR, LRESULT* pResult)
    {
        POSITION pos = m_lstPlayList.GetFirstSelectedItemPosition();
        if (pos != NULL)
        {
            int index = m_lstPlayList.GetNextSelectedItem(pos);
            m_nListIndex = index;
            CString s;
            CString s1 = Util::StringOp::ToCString(Data::LanguageResource::Get(Data::RI_COMN_TOBOTTOM).c_str());
            s = m_lstPlayList.GetItemText(index, 0);
            //usb
            if(s.Compare(_T("usbdisk")) == 0)
            {
                SetPlayList(_T("/usbdisk/"), 1);
            }
            //sd
            else if(s.Compare(_T("storagecard")) == 0)
            {
                SetPlayList(_T("/storagecard/"), 1);
            }
            //上一级
            else if(s.Compare(s1) == 0)
            {
                //	m_chDir
                CString sDir = m_chDir;
                int n = sDir.Find(_T("/"));
                ::UINT8 old[16];
                int i = 0;
                while(n != -1)
                {
                    old[i++] = n;
                    n += 1;
                    n = sDir.Find(_T("/"), n);
                }
                sDir = sDir.Mid(0, old[i-2]+1);
                if(sDir == _T("/"))
                    SetPlayList(sDir.GetBuffer(128), 0);
                else
                    SetPlayList(sDir.GetBuffer(128), 1);
            }

            else
            {
                int n = s.Find(_T(".mp3"));
                int n1 = s.Find(_T(".MP3"));
                int n2 = s.Find(_T(".wav"));
                int n3 = s.Find(_T(".WAV"));
                if((n1 == -1) && (n == -1) && (n2 == -1) && (n3 == -1))
                {
                    CString sDir = m_chDir;
                    sDir += s; 
                    sDir += "/";
                    SetPlayList(sDir.GetBuffer(128), 1);
                }
                else
                {
                    int id = m_lstPlayList.GetItemData(index);
                    id = (id == 0) ? 1 : 0;
                    LVITEM lvitem;
                    lvitem.mask = LVIF_TEXT | LVIF_IMAGE;   
                    lvitem.iItem = index;   
                    lvitem.iSubItem = 0;   
                    lvitem.pszText = (LPWSTR)(LPCTSTR)s; //m_lstPlayList.GetItemText(index, 0).GetBuffer(128);   
                    lvitem.lParam = index;   
                    lvitem.iImage = id;
                    m_lstPlayList.SetItem(&lvitem);
                    m_lstPlayList.SetItemData(index, id);
                }
            }
        }
        *pResult = 0;
    }

    static int gPlayIndex = 0;
    //播放
    void CMainMp3Dlg::OnPlayer()
    {
        if (m_IsPlay == 1)
        {
            if (m_lstPlayList1.IsWindowVisible())
            {
                POSITION pos = m_lstPlayList1.GetFirstSelectedItemPosition();
                //Dprintf("Mp3 list clicked\n");
                if (pos != NULL)
                {
                    int index = m_lstPlayList1.GetNextSelectedItem(pos);
                    if (index >= 0 && index != gPlayIndex)
                    {
                        CString sFilename = m_lstPlayList1.GetItemText(index, 0);
                        std::string s = Util::StringOp::FromCString(sFilename);
                        char filename_[128];
                        char file_dir[128];
                        int n = wcstombs( file_dir, m_chDir, 128);
                        sprintf(filename_, "%s/%s", file_dir, s.c_str());
                        //sprintf(filename_, "/flashdrv/my_music/%s", s.c_str());
                        SetMP3(filename_);
                        gPlayIndex = index;
                        playerDlg_->Show();	
                        return;
                    }
                }
            }
        }
        if (m_IsPlay == 0)
        {
            if (((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pTelephoneDlg->GetIsRecordStatus())
                return;
            //else
            {
                int ncount = m_lstPlayList.GetItemCount();
                m_lstPlayList1.DeleteAllItems();
                char *buffer = new char[1024*10];
                memset(buffer, 0, 1024*10);
                int nFileSelected = 0;
                for (int i = 0; i < ncount; ++i) {
                    int val = m_lstPlayList.GetItemData(i);
                    if (val == 1) {
                        m_lstPlayList1.InsertItem(nFileSelected, m_lstPlayList.GetItemText(i, 0));
                        ++nFileSelected;
                    }
                }

#if 0
                char txt[128];
                strcpy(txt, "[Playlist]\r\n");
                strcpy(buffer, txt);
                for (int i = 0; i < ncount; ++i)
                {
                    int val = m_lstPlayList.GetItemData(i);
                    if (val == 1)
                    {
                        char filename[128];
                        CString s = m_lstPlayList.GetItemText(i, 0);
                        int n = wcstombs( filename, s.GetBuffer(128), 128/*s.GetLength()*/);
                        filename[n] = '\0';

                        char file_dir[128];
                        n = wcstombs( file_dir, m_chDir, 128);
                        file_dir[n] = '\0';
                        sprintf(txt, "File%d=%s%s\r\nLength%d=255\r\n", i,file_dir,filename, i);
                        if(strlen(buffer) < (1024*8))
                            strcat(buffer, txt);
                        m_lstPlayList1.InsertItem(nFileSelected, s);
                        ++nFileSelected;
                    }
                }
                m_lstPlayList1.SetScrollRagle(FALSE);
                sprintf(txt, "NumberOfEntries=%d\r\nVersion=2\r\n", nFileSelected);
                strcat(buffer, txt);
                    FILE *file;
                    std::string musicFilename = Util::StringOp::FromCString(::musicPath);
                    musicFilename += "/playlist.pls";
                    file = fopen(musicFilename.c_str(), "w+b");
                    if (file)
                    {
                        fwrite(buffer, sizeof(char), strlen(buffer), file);
                        fclose(file);
                    }
#endif
                if (nFileSelected != 0)
                {
                    //调用播放窗口
                    if (playerDlg_)
                    {
                        m_lstPlayList1.ShowWindow_(SW_SHOW);
                        m_lstPlayList.ShowWindow_(SW_HIDE);

                        CString sFilename = m_lstPlayList1.GetItemText(0, 0);
                        std::string s = Util::StringOp::FromCString(sFilename);
                        char filename_[128];

                        char file_dir[128];
                        int n = wcstombs( file_dir, m_chDir, 128);
                        sprintf(filename_, "%s/%s", file_dir, s.c_str());

                        SetMP3(filename_);
                        gPlayIndex = 0;

                        playerDlg_->Show();

                        SetTimer(IDT_GETINFO_TIIMER, 500, NULL);
                    }
                    delete []buffer;
                    m_MJPGList.SetUnitIsShow(3, FALSE);
                    m_MJPGList.SetUnitIsShow(4, FALSE);
                    m_MJPGList.SetUnitIsShow(11, TRUE);
                    m_MJPGList.SetUnitIsShow(10, TRUE);
                    m_IsPlay = 1;
                    m_MJPGList.SetUnitIsDownStatus(0, TRUE);
                    m_MJPGList.SetUnitIsShow(0, TRUE);
                }
            }
        }
        else if (m_IsPlay == 1)
        {
            m_IsPlay = 2;
            m_MJPGList.SetUnitIsDownStatus(0, FALSE);
            m_MJPGList.SetUnitIsShow(0, TRUE);
            //m_btnPlayMp3.SetIcon(IDI_ICON_PLAY, CSize(24, 24));

            CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
            main->playeraudio_->PausePlayer(TRUE);
        }
        else if (m_IsPlay == 2)
        {
            m_IsPlay = 1;
            m_MJPGList.SetUnitIsDownStatus(0, TRUE);
            m_MJPGList.SetUnitIsShow(0, TRUE);
            CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
            main->playeraudio_->PausePlayer(FALSE);
        }
    }
    //前一条
    void CMainMp3Dlg::OnPre()
    {
        //	SetTimer(123, 1000, NULL);
        if(m_lstPlayList1.IsWindowVisible())
        {
            int  nCount = m_lstPlayList1.GetItemCount() -1;
            --gPlayIndex;
            if(gPlayIndex < 0)
                gPlayIndex = nCount;
            CString sFilename = m_lstPlayList1.GetItemText(gPlayIndex, 0);
            std::string s = Util::StringOp::FromCString(sFilename);
            char filename_[128];
            char file_dir[128];
            int n = wcstombs( file_dir, m_chDir, 128);
            sprintf(filename_, "%s%s", file_dir, s.c_str());
            //sprintf(filename_, "/flashdrv/my_music/%s", s.c_str());
            SetMP3(filename_);
            playerDlg_->Show();	
        }
    }
    //后一条
    void CMainMp3Dlg::OnBack()
    {
        //	KillTimer(123);
        if(m_lstPlayList1.IsWindowVisible())
        {
            int  nCount = m_lstPlayList1.GetItemCount() - 1;
            ++gPlayIndex;
            if(gPlayIndex > nCount)
                gPlayIndex = 0;
            CString sFilename = m_lstPlayList1.GetItemText(gPlayIndex, 0);
            std::string s = Util::StringOp::FromCString(sFilename);
            char filename_[128];
            char file_dir[128];
            int n = wcstombs( file_dir, m_chDir, 128);
            sprintf(filename_, "%s%s", file_dir, s.c_str());
            //sprintf(filename_, "/flashdrv/my_music/%s", s.c_str());
            SetMP3(filename_);
            playerDlg_->Show();	
        }
    }
    //打开播放列表
    void CMainMp3Dlg::OnOpenFile()
    {
        CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;

        main->playeraudio_->ExitPlayer();

        m_MJPGList.SetUnitIsShow(3, TRUE);
        m_MJPGList.SetUnitIsShow(4, TRUE);
        m_MJPGList.SetUnitIsShow(11, FALSE);
        m_MJPGList.SetUnitIsShow(10, FALSE);
        m_MJPGList.SetUnitIsDownStatus(0, FALSE);
        m_MJPGList.SetUnitIsShow(0, TRUE);
        m_IsPlay = 0;
        KillTimer(IDT_GETINFO_TIIMER);
        char txt[32];
        //sprintf(txt, "%d:%02d/%d:%02d", 0, 0, 0, 0);
        //CString s = txt;
        CString s;
        s.Format(_T("%d:%02d/%d:%02d"), 0, 0, 0, 0);
        m_MJPGList.SetUnitText(7, s, TRUE);
        m_prgPlayTime.SetPos(0);

        m_lstPlayList1.ShowWindow_(SW_HIDE);
        m_lstPlayList.ShowWindow_(SW_SHOW);

        //	m_btnPlayMp3.SetIcon(IDI_ICON_PLAY, CSize(24, 24));
    }

    //设置声音开关
    void CMainMp3Dlg::OnMute()
    {
        CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
        if(m_IsSound)
        {
            main->playeraudio_->SetVolume(0); 
            m_MJPGList.SetUnitIsDownStatus(5, TRUE);
            m_MJPGList.SetUnitIsShow(5, TRUE);
            //		m_btnMuteSound.SetIcon(IDI_ICON_SOUNDCACEL, CSize(12,12));
        }
        else
        {
            main->playeraudio_->SetVolume(m_Volume);
            m_MJPGList.SetUnitIsDownStatus(5, FALSE);
            m_MJPGList.SetUnitIsShow(5, TRUE);
            //		m_btnMuteSound.SetIcon(IDI_ICON_SOUNDOK, CSize(12,12));
        }
        m_IsSound = !m_IsSound;
    }

    //
    void CMainMp3Dlg::SetVolume()
    {
        CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
        int index = main->m_pSettingDlg->m_pSetting->sysVolume();
        int Volume[] = {10, 8, 6, 4, 2};
        main->playeraudio_->SetVolume(Volume[index]);
        m_Volume = Volume[index];
        m_prgSoundSelect.SetPos(m_Volume);
    }

    LRESULT CMainMp3Dlg::OnPregress(WPARAM w, LPARAM l)
    {
        LRESULT result = 0;
        CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
        //设置声音
        if(l == IDC_PROGRESS_MP3SOUND)
        {
            //Dprintf("SetSound %d \n", w);
            if(m_IsSound)
                main->playeraudio_->SetVolume(w);
            m_Volume = w;
        }
        //调整播放时间
        else if(l == IDC_PROGRESS_MP3TIME)
        {
            int percent = m_prgPlayTime.GetPercent();
            main->playeraudio_->PlayPos(percent);
        }
        return result;
    }

    void CMainMp3Dlg::SetCtrlEnable(BOOL flag)
    {

    }

    void CMainMp3Dlg::StartOpenNewFile()
    {

        KillTimer(IDT_GETINFO_TIIMER);
        SetTimer(IDT_GETINFO_TIIMER, 500, NULL);
    }

    /////////////////////////////////////////////////////////////////////////////
    // CMainMp3Dlg message handlers
    void CMainMp3Dlg::OnTimer(UINT nIDEvent) 
    {
        // TODO: Add your message handler code here and/or call default
        if(IDT_GETINFO_TIIMER == nIDEvent)
        {
            //KillTimer(IDT_GETINFO_TIIMER);
            CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
            MEDIA_INFO info;
            main->playeraudio_->GetVideoFileInfo(&info);

            int Hour,Min,Sec;
            int tick = info.nTick;
            Hour = tick / 3600 / TICKSPERSEC;
            tick -= Hour * 3600 * TICKSPERSEC;
            Min = tick / 60 / TICKSPERSEC;
            tick -= Min * 60 * TICKSPERSEC;
            Sec = tick / TICKSPERSEC;

            int Hour1,Min1,Sec1;
            tick = info.nDuration;
            Hour1 = tick / 3600 / TICKSPERSEC;
            tick -= Hour1 * 3600 * TICKSPERSEC;
            Min1 = tick / 60 / TICKSPERSEC;
            tick -= Min1 * 60 * TICKSPERSEC;
            Sec1 = tick / TICKSPERSEC;

            int n1 = Hour*3600+Min*60+Sec;
            int n2 = Hour1*3600+Min1*60+Sec1;

            m_prgPlayTime.SetParam(IDB_PROGRESS_TIME_THINCK, 0, n2, 1, 1, IDB_PROGRESS_TIME1, IDB_PROGRESS_TIME2);
            m_prgPlayTime.SetPos(n1);

            CString sFilename = info.szFileName;
            int nstart = 0;
            int noldstart = 0;
            nstart = sFilename.Find('/', noldstart);
            while(nstart >= 0)
            {
                noldstart = nstart+1;
                nstart = sFilename.Find('/', noldstart);
            }
            CString sFilename1 = sFilename.Mid(noldstart);
            static int gNReFresh = 0;
            if(gNReFresh++%3)
                sFilename1 = "";
            m_MJPGList.SetUnitText(12, sFilename1, TRUE);

            //m_stcFilename.SetTxt(sFilename1);

            //m_stcFilename.SetWindowText(info.szFileName);
            char txt[32];
            //sprintf(txt, "%d:%02d/%d:%02d", Hour*60+Min, Sec, Hour1*60+Min1, Sec1);
            //CString s = txt;
            CString s;
            s.Format(_T("%d:%02d/%d:%02d"), Hour*60+Min, Sec, Hour1*60+Min1, Sec1);
            m_MJPGList.SetUnitText(7, s, TRUE);

            if((n1 >= (n2 - 3)) && n1 != 0)		//换到下一首
            {
                //if(m_lstPlayList1.IsWindowVisible())
                if(main->playeraudio_->isPlaying_)
                {
                    int  nCount = m_lstPlayList1.GetItemCount() -1;
                    gPlayIndex ++;
                    if(gPlayIndex > nCount)
                        gPlayIndex = 0;
                    CString sFilename = m_lstPlayList1.GetItemText(gPlayIndex, 0);
                    std::string s = Util::StringOp::FromCString(sFilename);
                    char filename_[128];
                    char file_dir[128];
                    int n = wcstombs( file_dir, m_chDir, 128);
                    sprintf(filename_, "%s%s", file_dir, s.c_str());
                    //sprintf(filename_, "/flashdrv/my_music/%s", s.c_str());
                    SetMP3(filename_);
                    playerDlg_->Show();	
                }
            }
        }
        else if(nIDEvent == 123)
        {
            int nn = rand();
            nn = nn%12;
            //Dprintf("SetSound %d \n", nn);
            CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
            main->playeraudio_->SetVolume(nn);
            m_Volume = nn;
        }
        else if(nIDEvent == 1001)   //恢复播放
        {
            //Dprintf("Mp3 resume\r\n");
            KillTimer(1001);
            CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
            if(m_bIsPausebyEvent)
            {
                m_bIsPausebyEvent = FALSE;
                main->playeraudio_->SetOwner(playerDlg_);
                main->playeraudio_->InitPlayer();

                main->playeraudio_->SetVolume(m_Volume);

                main->playeraudio_->isActiveMode_ = 1;
                main->playeraudio_->ResumePlayer();

                if(m_IsPlay == 2)
                {
                    main->playeraudio_->PausePlayer(TRUE);
                }
                else
                {
                    SetTimer(IDT_GETINFO_TIIMER, 500, NULL);
                }
            }
        }
        else if(nIDEvent == 1002)	//暂停播放
        {
            //Dprintf("Mp3 stop\r\n");
            KillTimer(1002);
            CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
            if(main->playeraudio_->isPlaying_ )
            {
                KillTimer(IDT_GETINFO_TIIMER);
                main->playeraudio_->ExitPlayer(TRUE);
                m_bIsPausebyEvent = TRUE;
            }
        }
        CDialog::OnTimer(nIDEvent);
    }

    LRESULT CMainMp3Dlg::OnDeviceChange(WPARAM w, LPARAM l)
    {
        LRESULT result = 0;
        if (w == 0x8000) //insert
        {
            ::Sleep(500);
            if(DetectDIR(_T("/usbdisk")))
                ;
            //		m_stcFilename.SetTxt(L"USB insert");
            else if(DetectDIR(_T("/storagecard")))
                ;
            //		m_stcFilename.SetTxt(L"SD insert");

            if(memcmp(m_chDir, (TCHAR*)(LPCTSTR)::musicPath, wcslen(m_chDir)*2) == 0)
                SetPlayList((TCHAR*)(LPCTSTR)::musicPath, 0);

        }
        else if (w == 0x8004) //remove
        {
            ::Sleep(1000);
            //	m_stcFilename.SetTxt(L"remove");
            if(memcmp(m_chDir, (TCHAR*)(LPCTSTR)::musicPath, wcslen(m_chDir)*2) == 0)
                SetPlayList((TCHAR*)(LPCTSTR)::musicPath, 0);
        }

        //GetLogicalDrives()
        return result;
    }

    LRESULT CMainMp3Dlg::OnOutEvent(WPARAM w, LPARAM l)
    {
        LRESULT result = 0;
        CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;

        if(l == 0)  //暂停播放
        {
            KillTimer(1002);
            SetTimer(1002, 10, NULL);
        }
        else if(l == 1) //恢复播放
        {
            KillTimer(1001);
            SetTimer(1001, 10, NULL);
        }
        return result;
    }

    LRESULT CMainMp3Dlg::OnClickMJPG(WPARAM w, LPARAM l)
    {
        LRESULT result = 0;
        switch(w)
        {
        case 1:			//全选
            OnSelectAll();
            break;
        case 2:         //上一条
            OnPre();
            break;
        case 3:			//播放
            OnPlayer();
            break;      
        case 4:			//下一条
            OnBack();
            break;
        case 5:			//清空
            OnClearAll();
            break;
        case 6:			//声音开关
            OnMute();
            break;
        case 7:			//退出
            OnExit();
            break;
        case 8:			//打开
            OnOpenFile();
            break;
        case 9:			//全屏
            //	OnPlayerAll();
            break;
        }
        return result;
    }
}
