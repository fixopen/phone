// MainVideoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MainVideoDlg.h"
#include "player.h"
#include "../MultimediaPhoneDlg.h"
#include "../MultimediaPhone.h"
#include "../Data/LanguageResource.h"
#include "../Data/SkinStyle.h"
#include "../Util/ScanDir.h"

#include <string>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern BOOL DetectDIR(TCHAR *sDir);

namespace View {
    //#define IDT_GETINFO_TIIMER	8000
    /////////////////////////////////////////////////////////////////////////////
    // CMainVideoDlg dialog
    //#define WM_CHANGEWINDOW		8001
    //#define WM_PLAYVIDEO		8002    //WPARAM 0 ��ͣ���� 1 �ָ����� 2 ȫ������ 3 �˳�ȫ��
    CMainVideoDlg::CMainVideoDlg(CWnd* pParent /*=NULL*/)
        : CDialog(CMainVideoDlg::IDD, pParent)
    {
        //{{AFX_DATA_INIT(CMainVideoDlg)
        // NOTE: the ClassWizard will add member initialization here
        //}}AFX_DATA_INIT
        memset(m_chDir, 0, 128*2);
        m_IsPlay = 0;
        m_IsSound = TRUE;
        m_Volume = SOUND_DEFAULT_VOLUME;
        m_bIsPausebyEvent = FALSE;
    }


    void CMainVideoDlg::DoDataExchange(CDataExchange* pDX)
    {
        CDialog::DoDataExchange(pDX);
        //{{AFX_DATA_MAP(CMainVideoDlg)
        // NOTE: the ClassWizard will add DDX and DDV calls here
        //}}AFX_DATA_MAP
    }


    BEGIN_MESSAGE_MAP(CMainVideoDlg, CDialog)
        //{{AFX_MSG_MAP(CMainVideoDlg)
        ON_BN_CLICKED(IDC_BUTTON_VIDEOEXIT, OnExit)
        ON_BN_CLICKED(IDC_BUTTON_VIDEOALL, OnSelectAll)
        ON_BN_CLICKED(IDC_BUTTON_VIDEOCLEAR, OnClearAll)
        ON_BN_CLICKED(IDC_BUTTON_VIDEOPLAY, OnPlayer)
        ON_BN_CLICKED(IDC_BUTTON_VIDEOPRE, OnPre)
        ON_BN_CLICKED(IDC_BUTTON_VIDEOBACK, OnBack)
        ON_BN_CLICKED(IDC_BUTTON_VIDEOOPEN, OnOpenFile)
        ON_BN_CLICKED(IDC_BUTTON_VIDEOPLAYALL, OnPlayerAll)
        ON_BN_CLICKED(IDC_BUTTON_VIDEOMUTE, OnMute)
        ON_MESSAGE(WM_PROCESS_POS, OnPregress)
        ON_NOTIFY(NM_CLICK, IDC_LIST_PLAYLIST, OnClickPlayList)
        ON_WM_TIMER()
        ON_WM_ACTIVATE()
        ON_MESSAGE(WM_USB_MSG, OnDeviceChange)
        ON_MESSAGE(WM_OUTEVENT, OnOutEvent)
        ON_MESSAGE(WM_CLICKMJPG_TOAPP, OnClickMJPG)
        ON_MESSAGE(WM_LISTCTRL_CLICK, OnListCtrlClick)
        ON_WM_KEYDOWN()
        //}}AFX_MSG_MAP
    END_MESSAGE_MAP()

    void CMainVideoDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
    {
        if(nChar == 'U')
        {
            OnPre();
        }
        else if(nChar == 'D')
        {
            OnBack();
        }
    }

    LRESULT CMainVideoDlg::OnListCtrlClick(WPARAM w, LPARAM l)
    {
        LRESULT ret;
        if (w == IDC_LIST_PLAYLIST)
            OnClickPlayList(NULL, &ret);
        return ret;
    }
    /////////////////////////////////////////////////////////////////////////////
    // CMainVideoDlg message handlers
    void CMainVideoDlg ::OnActivate( UINT nState, CWnd* pWndOther, BOOL bMinimized )
    {
        if(nState & WA_ACTIVE )
        {
            SetPlayList((TCHAR*)(LPCTSTR)::videoPath, 0);
        }
    }

    void CMainVideoDlg::SetVolume()
    {
        CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
        int index = main->m_pSettingDlg->m_pSetting->sysVolume();
        int Volume[] = {10, 8, 6, 4, 2};
        main->playeraudio_->SetVolume(Volume[index]);
        m_Volume = Volume[index];
        m_prgSoundSelect.SetPos(m_Volume);
    }

    BOOL CMainVideoDlg::OnInitDialog() 
    {
        CDialog::OnInitDialog();

        // TODO: Add extra initialization here

		m_MJPGList.Create(L"", WS_VISIBLE|WS_CHILD, CRect(0*X_XISHU, 0*Y_XISHU, 535*X_XISHU, 420*Y_XISHU), this);
        m_MJPGList.SetCurrentLinkFile(_T(".\\adv\\mjpg\\k1\\����\\ӰԺ1.xml"));
        m_MJPGList.SetMJPGRect(CRect(0*X_XISHU, 0, 535*X_XISHU, 420*Y_XISHU));

        m_prgPlayTime.Create(WS_CHILD|WS_VISIBLE, CRect(25*X_XISHU, 268*Y_XISHU, (25*X_XISHU+393), (268*Y_XISHU+16)), &m_MJPGList, IDC_PROGRESS_VIDEOTIME);
        m_prgPlayTime.SetParam(IDB_PROGRESS_TIME_THINCK, 0, 10, 1, 1, IDB_PROGRESS_TIME1, IDB_PROGRESS_TIME2);

        m_prgPlayTime.SetPos(0, FALSE);
        m_prgPlayTime.SetBackRGB(Data::g_allFramBackRGB[Data::g_skinstyle]);

        m_prgSoundSelect.Create(WS_CHILD|WS_VISIBLE, CRect(498*X_XISHU, 313*Y_XISHU, (498*X_XISHU+27), (313*Y_XISHU+90)), &m_MJPGList,IDC_PROGRESS_VIDEOSOUND);
        m_prgSoundSelect.SetParam(IDB_PROGRESS_VOLUMETHINCK, 0, 10, 1, 3, IDB_PROGRESS_VOLUME1, IDB_PROGRESS_VOLUME1);
        SetVolume();
        m_prgSoundSelect.SetBackRGB(Data::g_allFramBackRGB[Data::g_skinstyle]);

        m_lstPlayList.Create(WS_CHILD|WS_VISIBLE|LVS_REPORT|LVS_NOCOLUMNHEADER|LVS_NOSORTHEADER, CRect(7*X_XISHU, 50*Y_XISHU, (7+522)*X_XISHU, (50+213)*Y_XISHU), &m_MJPGList, IDC_LIST_PLAYLIST, TRUE, 1);
        m_lstPlayList.SetListColor(Data::g_listctrlBackRGB1[Data::g_skinstyle], Data::g_listctrlBackRGB2[Data::g_skinstyle]);
        m_lstPlayList.InsertColumn(0, _T("Filename"), LVCFMT_LEFT, 480*X_XISHU);

        m_pImageList = new CImageList();
        m_pImageList->Create(32, 32, ILC_COLOR32|ILC_MASK, 2, 2);   
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

        SetPlayList((TCHAR*)(LPCTSTR)::videoPath, 0);
        m_lstPlayList.SetScrollRagle(FALSE);

        //������
        CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
        playerDlg_ = new CPlayerDlg(main->playervideo_);

        playerDlg_->Create(CPlayerDlg::IDD, this);

        m_MJPGList.SetUnitIsShow(3, TRUE);
        m_MJPGList.SetUnitIsShow(4, TRUE);
        m_MJPGList.SetUnitIsShow(11, FALSE);
        m_MJPGList.SetUnitIsShow(10, FALSE);


        return TRUE;  // return TRUE unless you set the focus to a control
        // EXCEPTION: OCX Property Pages should return FALSE
    }

    void CMainVideoDlg::SetVideo(char *filename)
    {
        playerDlg_->SetParam( /*"/flashdrv/my_video/playlist.pls"*/"", mtVideo);
        CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
        CRect position;
        main->GetWindowRect(&position);
        position.left += 7 * X_XISHU;
        position.top += 50 * Y_XISHU;
        position.right = position.left + 522 * X_XISHU;
        position.bottom = position.top + 213 * Y_XISHU;
        playerDlg_->ReSetWindowsRect(position);
        m_bIsPausebyEvent = FALSE;
    }

    class FindMovie : public Util::ProcessFileProcedure {
    public:
        FindMovie(CString dir, CCEListCtrl& movieList)
            : ProcessFileProcedure(dir)
            , movieList_(movieList) {
        }
        virtual void operator()(WIN32_FIND_DATA const& FindFileData) {
            int itemIndex = 0;
            int imageIndex = 0;
            if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                imageIndex = 2;
            } else {
                imageIndex = 0;
            }
            movieList_.InsertItem(itemIndex, FindFileData.cFileName, imageIndex);
            ++itemIndex;
        }
    private:
        CCEListCtrl& movieList_;
    };

    void CMainVideoDlg::SetPlayList(TCHAR* cdir, int local)
    {
        m_lstPlayList.DeleteAllItems();
        //wcscpy(m_chDir, _T("D:/flashdrv/my_video"));
        wcscpy(m_chDir, cdir);
        CString dir = cdir;
        ProcessFiles(_T("*.wmv"), FindMovie(dir, m_lstPlayList));
        ProcessFiles(_T("*.avi"), FindMovie(dir, m_lstPlayList));
        ProcessFiles(_T("*.mpg"), FindMovie(dir, m_lstPlayList));
        ProcessFiles(_T("*.mp4"), FindMovie(dir, m_lstPlayList));
        ProcessFiles(_T("*.m4v"), FindMovie(dir, m_lstPlayList));
        m_lstPlayList.SetScrollRagle(TRUE);
    }

    void CMainVideoDlg::InitCtrl()
    {
        m_MJPGList.SetUnitIsShow(3, TRUE);
        m_MJPGList.SetUnitIsShow(4, TRUE);
        m_MJPGList.SetUnitIsShow(11, FALSE);
        m_MJPGList.SetUnitIsShow(10, FALSE);
        m_MJPGList.SetUnitIsDownStatus(0, FALSE);

        m_IsPlay = 0;
    }

    void CMainVideoDlg::OnExit()
    {
        m_bIsPausebyEvent = FALSE;
        CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
        main->playervideo_->ExitPlayer();
        KillTimer(IDT_GETINFO_TIIMER);


        //lxz test 20080704
        /*
        m_btnAllSelect.ShowWindow(SW_SHOW);
        m_btnAllClear.ShowWindow(SW_SHOW);
        m_btnOpenFile.ShowWindow(SW_HIDE);
        m_btnPlayAll.ShowWindow(SW_HIDE);
        m_btnPlayVideo.SetToggleValue(FALSE);
        */
        m_MJPGList.SetUnitIsShow(3, TRUE);
        m_MJPGList.SetUnitIsShow(4, TRUE);
        m_MJPGList.SetUnitIsShow(11, FALSE);
        m_MJPGList.SetUnitIsShow(10, FALSE);
        m_MJPGList.SetUnitIsDownStatus(0, FALSE);
        m_prgPlayTime.SetPos(0);

        m_IsPlay = 0;

        playerDlg_->ShowWindow(SW_HIDE);
        GetParent()->SendMessage(WM_CHANGEWINDOW, (WPARAM)this, (LPARAM)SW_HIDE);
    }

    void CMainVideoDlg::OnSelectAll()
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
            int n = s.Find(_T(".mp4"));
            int n1 = s.Find(_T(".avi"));
            int n2 = s.Find(_T(".AVI"));
            int n3 = s.Find(_T(".wmv"));
            int n4 = s.Find(_T(".WMV"));
            int n5 = s.Find(_T(".MPG"));
            int n6 = s.Find(_T(".mpg"));
            int n7 = s.Find(_T(".MP4"));
            int n8 = s.Find(_T(".m4v"));
            int n9 = s.Find(_T(".M4V"));
            if((n9 == -1) &&(n8 == -1) &&(n7 == -1) &&(n5 == -1) && (n6 == -1) && (n4 == -1) && (n1 == -1) && (n == -1) && (n2 == -1) && (n3 == -1))
                continue;
            lvitem.lParam=i;   
            lvitem.iImage=1;
            m_lstPlayList.SetItem(&lvitem);
            m_lstPlayList.SetItemData(i, 1);
        }
    }

    void CMainVideoDlg::OnClearAll()
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
            int n = s.Find(_T(".mp4"));
            int n1 = s.Find(_T(".avi"));
            int n2 = s.Find(_T(".AVI"));
            int n3 = s.Find(_T(".wmv"));
            int n4 = s.Find(_T(".WMV"));
            int n5 = s.Find(_T(".MPG"));
            int n6 = s.Find(_T(".mpg"));
            int n7 = s.Find(_T(".MP4"));
            int n8 = s.Find(_T(".m4v"));
            int n9 = s.Find(_T(".M4V"));
            if((n9 == -1) &&(n8 == -1) &&(n7 == -1) &&(n5 == -1) && (n6 == -1) && (n4 == -1) && (n1 == -1) && (n == -1) && (n2 == -1) && (n3 == -1))
                continue;
            lvitem.lParam=i;   
            lvitem.iImage=0;
            m_lstPlayList.SetItem(&lvitem);
            m_lstPlayList.SetItemData(i, 0);
        }
    }

    void CMainVideoDlg::OnClickPlayList(NMHDR* pNMHDR, LRESULT* pResult)
    {
        POSITION pos = m_lstPlayList.GetFirstSelectedItemPosition();
        if (pos != NULL)
        {
            int index = m_lstPlayList.GetNextSelectedItem (pos);
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
            //��һ��
            else if(s.Compare(s1) == 0)
            {
                //	m_chDir
                CString sDir = m_chDir;
                int n = sDir.Find(_T("/"));
                ::UINT8 old[16];
                int i = 0;
                while (n != -1)
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
                int n = s.Find(_T(".mp4"));
                int n1 = s.Find(_T(".avi"));
                int n2 = s.Find(_T(".AVI"));
                int n3 = s.Find(_T(".wmv"));
                int n4 = s.Find(_T(".WMV"));
                int n5 = s.Find(_T(".MPG"));
                int n6 = s.Find(_T(".mpg"));
                int n7 = s.Find(_T(".MP4"));
                int n8 = s.Find(_T(".m4v"));
                int n9 = s.Find(_T(".M4V"));
                if((n9 == -1) &&(n8 == -1) &&(n7 == -1) &&(n5 == -1) && (n6 == -1) && (n4 == -1) && (n1 == -1) && (n == -1) && (n2 == -1) && (n3 == -1))
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

    //����
    void CMainVideoDlg::OnPlayer()
    {
        if (m_IsPlay == 0)
        {
            if (((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pTelephoneDlg->GetIsRecordStatus())
                return;

            m_VideoList.clear();
            int ncount = m_lstPlayList.GetItemCount();

#if 0
            char buffer[1024 * 4];
            memset(buffer, 0, 1024 * 4);
            int nFileSelected = 0;

            char txt[128];
            strcpy(txt, "[Playlist]\r\n");
            strcpy(buffer, txt);
            for (int i = 0; i < ncount; ++i)
            {
                int val = m_lstPlayList.GetItemData(i);
                if (val == 1)
                {
                    CString s = m_lstPlayList.GetItemText(i, 0);
                    //char filename[128];
                    //int n = wcstombs(filename, s.GetBuffer(128), 128/*s.GetLength()*/);
                    //filename[n] = '\0';
                    std::string filename = Util::StringOp::FromCString(s);

                    //char file_dir[128];
                    //n = wcstombs(file_dir, m_chDir, 128);
                    //file_dir[n] = '\0';
                    std::string dir = Util::StringOp::FromCString(m_chDir);
                    sprintf(txt, "File%d=%s/%s\r\nLength%d=255\r\n", i, dir.c_str(), filename.c_str(), i);

                    if (strlen(buffer) < (1024 * 3))
                        strcat(buffer, txt);

                    strcat(buffer, txt);
                    ++nFileSelected;

                    char txt[128];
                    sprintf(txt, "%s/%s", dir.c_str(), filename.c_str());

                    m_VideoList.push_back(Util::StringOp::ToCString(txt));
                }
            }
            sprintf(txt, "NumberOfEntries=%d\r\nVersion=2\r\n", nFileSelected);
            strcat(buffer, txt);

            //if (nFileSelected != 0)
            {
                std::string videoFilename = Util::StringOp::FromCString(::videoPath);
                videoFilename += "/playlist.pls";
                FILE* file = fopen(videoFilename.c_str(), "w+b");
                if (file)
                {
                    fwrite(buffer, sizeof(char), strlen(buffer), file);
                    fclose(file);
                }
            }
#endif
            for (int i = 0; i < ncount; ++i) {
                int val = m_lstPlayList.GetItemData(i);
                if (val == 1) {
                    m_VideoList.push_back(CString(m_chDir) + _T("/") + m_lstPlayList.GetItemText(i, 0));
                }
            }
            //���ò��Ŵ���
            if (playerDlg_)
            {
                CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
                main->playervideo_->SetImageList(m_VideoList);

                //�˳�MP3���Ŵ���
                main->m_pMainDlg->m_mainMp3Dlg_->OnExit_(TRUE);

                SetVideo("");
                playerDlg_->Show();
                main->playervideo_->Cur();

                SetTimer(IDT_GETINFO_TIIMER, 1000, NULL);
            }

            m_MJPGList.SetUnitIsShow(3, FALSE);
            m_MJPGList.SetUnitIsShow(4, FALSE);
            m_MJPGList.SetUnitIsShow(11, TRUE);
            m_MJPGList.SetUnitIsShow(10, TRUE);
            m_IsPlay = 1;
            m_MJPGList.SetUnitIsDownStatus(0, TRUE);
            m_MJPGList.SetUnitIsShow(0, TRUE);
        }
        else if (m_IsPlay == 1)	//stop player	
        {
            //lxz test 20080704
            m_IsPlay = 2;
            m_MJPGList.SetUnitIsDownStatus(0, FALSE);
            m_MJPGList.SetUnitIsShow(0, TRUE);

            KillTimer(IDT_GETINFO_TIIMER);
            CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
            main->playervideo_->PausePlayer(TRUE);
        }
        else if (m_IsPlay == 2)	//continue player
        {
            m_IsPlay = 1;
            //lxz test 20080704
            m_MJPGList.SetUnitIsDownStatus(0, TRUE);
            m_MJPGList.SetUnitIsShow(0, TRUE);
            CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
            SetTimer(IDT_GETINFO_TIIMER, 1000, NULL);
            main->playervideo_->PausePlayer(FALSE);
        }
    }
    //ǰһ��
    void CMainVideoDlg::OnPre()
    {
        CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
        main->playervideo_->Up();
    }

    //��һ��
    void CMainVideoDlg::OnBack()
    {
        CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
        main->playervideo_->Down();
    }
    //�򿪲����б�
    void CMainVideoDlg::OnOpenFile()
    {
        CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
        main->playervideo_->ExitPlayer();

        playerDlg_->ShowWindow(SW_HIDE);
        //lxz test 20080704
        /*
        m_btnAllSelect.ShowWindow(SW_SHOW);
        m_btnAllClear.ShowWindow(SW_SHOW);
        m_btnOpenFile.ShowWindow(SW_HIDE);
        m_btnPlayAll.ShowWindow(SW_HIDE);
        m_btnPlayVideo.SetToggleValue(FALSE);
        */
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
        /*
        m_stcTime.SetWindowText(s);
        m_stcFilename.SetTxt(L"");
        */
        m_prgPlayTime.SetPos(0);

        //	m_btnPlayVideo.SetIcon(IDI_ICON_PLAY, CSize(24, 24));
    }
    //ȫ������
    void CMainVideoDlg::OnPlayerAll()
    {
        SetCtrlEnable(FALSE);//hide control
        GetParent()->SendMessage(WM_PLAYVIDEO, 2);	

        ::SetCursorPos(799, 479);
    }
    //������������
    void CMainVideoDlg::OnMute()
    {
        CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
        if(m_IsSound)
        {
            main->playervideo_->SetVolume(0); 

            //lxz 20080704
            m_MJPGList.SetUnitIsDownStatus(5, TRUE);
            m_MJPGList.SetUnitIsShow(5, TRUE);
        }
        else
        {
            main->playervideo_->SetVolume(m_Volume); 
            //lxz 20080704
            m_MJPGList.SetUnitIsDownStatus(5, FALSE);
            m_MJPGList.SetUnitIsShow(5, TRUE);
        }
        m_IsSound = !m_IsSound;
    }
    //
    LRESULT CMainVideoDlg::OnPregress(WPARAM w, LPARAM l)
    {
        LRESULT result = 0;
        CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
        //��������
        if(l == IDC_PROGRESS_VIDEOSOUND)
        {
            if(m_IsSound)
                main->playervideo_->SetVolume(w);
            m_Volume = w;
        }
        //��������ʱ��
        else if(l == IDC_PROGRESS_VIDEOTIME)
        {
            int percent = m_prgPlayTime.GetPercent();
            main->playervideo_->PlayPos(percent);	
        }
        return result;
    }

    void CMainVideoDlg::SetCtrlEnable(BOOL flag)
    {
        /*
        m_stcTitle.EnableWindow(flag);
        m_stcTime.EnableWindow(flag);
        m_btnExit.EnableWindow(flag);
        m_btnAllSelect.EnableWindow(flag);
        m_btnAllClear.EnableWindow(flag);
        m_btnOpenFile.EnableWindow(flag);
        m_btnPlayAll.EnableWindow(flag);
        m_btnPlayVideo.EnableWindow(flag);
        m_btnPreVideo.EnableWindow(flag);
        m_btnBackVideo.EnableWindow(flag);
        m_btnMuteSound.EnableWindow(flag);
        m_lstPlayList.EnableWindow(flag);
        m_prgPlayTime.EnableWindow(flag);
        m_prgSoundSelect.EnableWindow(flag);
        */
    }

    void CMainVideoDlg::StartOpenNewFile()
    {
        KillTimer(IDT_GETINFO_TIIMER);
        SetTimer(IDT_GETINFO_TIIMER, 1000, NULL);
        //	printf("Get MSG_END\n");
    }

    void CMainVideoDlg::OnTimer(UINT nIDEvent) 
    {
        // TODO: Add your message handler code here and/or call default
        CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
        if(IDT_GETINFO_TIIMER == nIDEvent)
        {
            //		KillTimer(IDT_GETINFO_TIIMER);
            MEDIA_INFO info;
            main->playervideo_->GetVideoFileInfo(&info);

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

            //lxz 20080704
            //m_stcFilename.SetTxt(sFilename1);

            char txt[32];
            //sprintf(txt, "%d:%02d/%d:%02d", Hour*60+Min, Sec, Hour1*60+Min1, Sec1);
            //CString s = txt;
            CString s;
            s.Format(_T("%d:%02d/%d:%02d"), Hour*60+Min, Sec, Hour1*60+Min1, Sec1);

            m_MJPGList.SetUnitText(7, s, TRUE);

            if((n1 >= (n2 - 3)) && n1 != 0)		//������һ��
            {
                //if(IsWindowVisible())
                if(main->playervideo_->isPlaying_)
                {
                    main->playervideo_->Down();
                }
            }
            main->CancelBalckLightSaveTimer();
        }
        else if(IDT_PAUSE_PLAYER == nIDEvent)
        {
            KillTimer(nIDEvent);
            main->playervideo_->ExitPlayer();
        }

        else if(nIDEvent == 1001)   //�ָ�����
        {
            KillTimer(1001);
            CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
            if (m_bIsPausebyEvent)
            {
                if (IsWindowVisible() && m_bIsPausebyEvent)
                {
                    m_bIsPausebyEvent = FALSE;
                    //main->playervideo_->SetOwner(playerDlg_);
                    main->playervideo_->InitPlayer();
                    main->playervideo_->SetVolume(m_Volume);
                    main->playervideo_->ResumePlayer();

                    if(m_IsPlay == 2)
                    {
                        main->playervideo_->PausePlayer(TRUE);
                    }
                    else
                    {
                        SetTimer(IDT_GETINFO_TIIMER, 1000, NULL);
                    }

                    main->KillScreenSaveTimer();  //�ָ�����ʱ,ֹͣ����
                }
            }
        }
        else if(nIDEvent == 1002)	//��ͣ����
        {
            KillTimer(1002);
            CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
            if(main->playervideo_->isPlaying_)
            {
                KillTimer(IDT_GETINFO_TIIMER);
                main->playervideo_->ExitPlayer(TRUE);
                m_bIsPausebyEvent = TRUE;
            }
        }
        CDialog::OnTimer(nIDEvent);
    }

    LRESULT CMainVideoDlg::OnDeviceChange(WPARAM w, LPARAM l)
    {
        LRESULT result = 0;
        if (w == 0x8000) //insert
        {
            ::Sleep(500);
            if (::DetectDIR(_T("/usbdisk")))
                ;
            //	m_stcFilename.SetTxt(L"USB insert");
            else if (::DetectDIR(_T("/storagecard")))
                ;
            //	m_stcFilename.SetTxt(L"SD insert");

            if(memcmp(m_chDir, (TCHAR*)(LPCTSTR)::videoPath, wcslen(m_chDir)*2) == 0)
                SetPlayList((TCHAR*)(LPCTSTR)::videoPath, 0);

        }
        else if (w == 0x8004) //remove
        {
            ::Sleep(1500);
            //m_stcFilename.SetTxt(L"remove");
            if(memcmp(m_chDir, (TCHAR*)(LPCTSTR)::videoPath, wcslen(m_chDir)*2) == 0)
                SetPlayList((TCHAR*)(LPCTSTR)::videoPath, 0);
        }
        return result;
    }

    LRESULT CMainVideoDlg::OnOutEvent(WPARAM w, LPARAM l)
    {
        LRESULT result = 0;
        CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;

        if(l == 0)  //��ͣ����
        {
            //SetTimer(1002, 5, NULL);
            OnTimer(1002);
        }
        else if(l == 1) //�ָ�����
        {
            SetTimer(1001, 50, NULL);
        }
        return result;
    }

    /*
    ON_BN_CLICKED(IDC_BUTTON_VIDEOEXIT, OnExit)
    ON_BN_CLICKED(IDC_BUTTON_VIDEOALL, OnSelectAll)
    ON_BN_CLICKED(IDC_BUTTON_VIDEOCLEAR, OnClearAll)
    ON_BN_CLICKED(IDC_BUTTON_VIDEOPLAY, OnPlayer)
    ON_BN_CLICKED(IDC_BUTTON_VIDEOPRE, OnPre)
    ON_BN_CLICKED(IDC_BUTTON_VIDEOBACK, OnBack)
    ON_BN_CLICKED(IDC_BUTTON_VIDEOOPEN, OnOpenFile)
    ON_BN_CLICKED(IDC_BUTTON_VIDEOPLAYALL, OnPlayerAll)
    ON_BN_CLICKED(IDC_BUTTON_VIDEOMUTE, OnMute)
    */
    LRESULT CMainVideoDlg ::OnClickMJPG(WPARAM w, LPARAM l)
    {
        LRESULT result = 0;
        switch(w)
        {
        case 1:			//ȫѡ
            OnSelectAll();
            break;
        case 2:         //��һ��
            OnPre();
            break;
        case 3:			//����
            OnPlayer();
            break;      
        case 4:			//��һ��
            OnBack();
            break;
        case 5:			//���
            OnClearAll();
            break;
        case 6:			//��������
            OnMute();
            break;
        case 7:			//�˳�
            OnExit();
            break;
        case 8:			//��
            OnOpenFile();
            break;
        case 9:			//ȫ��
            OnPlayerAll();
            break;
        }
        return result;
    }
}
