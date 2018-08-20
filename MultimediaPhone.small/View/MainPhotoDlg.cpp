// MainPhotoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MainPhotoDlg.h"
#include "../MultimediaPhoneDlg.h"
#include "../MultimediaPhone.h"
#include "../resource.h"
#include "../Data/LanguageResource.h"
#include "../Data/SkinStyle.h"
#include "../Util/ScanDir.h"

#include <algorithm>
#include <string>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern BOOL DetectDIR(TCHAR *sDir);

namespace View {
    /////////////////////////////////////////////////////////////////////////////
    // CMainPhotoDlg dialog

    CMainPhotoDlg::CMainPhotoDlg(CWnd* pParent /*=NULL*/)
        : CDialog(CMainPhotoDlg::IDD, pParent)
    {
        //{{AFX_DATA_INIT(CMainPhotoDlg)
        // NOTE: the ClassWizard will add member initialization here

        //}}AFX_DATA_INIT
        memset(m_chDir, 0, 128*2);
    }


    void CMainPhotoDlg::DoDataExchange(CDataExchange* pDX)
    {
        CDialog::DoDataExchange(pDX);
        //{{AFX_DATA_MAP(CMainPhotoDlg)
        // NOTE: the ClassWizard will add DDX and DDV calls here
        //}}AFX_DATA_MAP
    }


    BEGIN_MESSAGE_MAP(CMainPhotoDlg, CDialog)
        //{{AFX_MSG_MAP(CMainPhotoDlg)
        ON_BN_CLICKED(IDC_BUTTON_PHOTOEXIT, OnExit)
        ON_BN_CLICKED(IDC_BUTTON_PHOTOALL, OnSelectAll)
        ON_BN_CLICKED(IDC_BUTTON_PHOTOCLEAR, OnClearAll)
        ON_BN_CLICKED(IDC_BUTTON_PHOTOPLAY, OnPlayer)
        ON_BN_CLICKED(IDC_BUTTON_PHOTOPRE, OnPre)
        ON_BN_CLICKED(IDC_BUTTON_PHOTOBACK, OnBack)
        ON_BN_CLICKED(IDC_BUTTON_PHOTOOPEN, OnOpenFile)
        ON_BN_CLICKED(IDC_BUTTON_PHOTOPLAYALL, OnPlayerAll)
        ON_BN_CLICKED(IDC_BUTTON_PHOTOSCREENSAVE, OnScreenSave)
        ON_NOTIFY(NM_CLICK, IDC_LIST_PLAYPHOTOLIST, OnClickPlayList)
        ON_WM_ACTIVATE()
        ON_MESSAGE(WM_USB_MSG, OnDeviceChange)
        ON_MESSAGE(WM_CLICKMJPG_TOAPP, OnClickMJPG)
        ON_MESSAGE(WM_LISTCTRL_CLICK, OnListCtrlClick)
        ON_MESSAGE(WM_OUTEVENT, OnOutEvent)
        ON_WM_TIMER()
        //}}AFX_MSG_MAP
    END_MESSAGE_MAP()

    LRESULT CMainPhotoDlg::OnListCtrlClick(WPARAM w, LPARAM l)
    {
        LRESULT ret;
        if(w == IDC_LIST_PLAYPHOTOLIST)
            OnClickPlayList(NULL, &ret);
        return ret;
    }

    void CMainPhotoDlg::OnActivate( UINT nState, CWnd* pWndOther, BOOL bMinimized )
    {
        if(nState & WA_ACTIVE )
        {
            m_nCountPhoto = SetPlayList((TCHAR*)(LPCTSTR)::photoPath, 0);
        }
    }

    /////////////////////////////////////////////////////////////////////////////
    // CMainPhotoDlg message handlers

    BOOL CMainPhotoDlg::OnInitDialog() 
    {
        CDialog::OnInitDialog();

        // TODO: Add extra initialization here

        m_lstPlayList.Create(WS_CHILD|WS_VISIBLE|LVS_REPORT|LVS_NOCOLUMNHEADER|LVS_NOSORTHEADER, CRect(7, 50, 7+522, 50+213), this, IDC_LIST_PLAYPHOTOLIST, TRUE, 1);
        m_lstPlayList.SetListColor(Data::g_listctrlBackRGB1[Data::g_skinstyle], Data::g_listctrlBackRGB2[Data::g_skinstyle]);
        m_lstPlayList.InsertColumn(0, _T("Filename"), LVCFMT_LEFT, 480);

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

        m_nCountPhoto = SetPlayList((TCHAR*)(LPCTSTR)::photoPath, 0);
        m_lstPlayList.SetScrollRagle(FALSE);

        /*
        char stxt[32];
        sprintf(stxt, "%d/%d", 0, m_nCountPhoto);
        s = stxt;
        m_stcCount.Create(L"", WS_CHILD|WS_VISIBLE, CRect(208, 165, 232, 178), this);
        m_stcCount.SetColor(RGB(0, 0, 0), Data::g_allFramBackRGB[Data::g_skinstyle]);
        */

        //播放器
        CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
        playerDlg_ = new CPlayerDlg(main->playerimage_);
        playerDlg_->Create(CPlayerDlg::IDD, this);

        m_MJPGList.Create(L"", WS_VISIBLE|WS_CHILD, CRect(0, 0, 535, 420), this);
        m_MJPGList.SetCurrentLinkFile(_T(".\\adv\\mjpg\\k1\\中文\\相册1.xml"));
        m_MJPGList.SetMJPGRect(CRect(0, 0, 535, 420));

        m_MJPGList.SetUnitIsShow(3, TRUE);
        m_MJPGList.SetUnitIsShow(4, TRUE);
        m_MJPGList.SetUnitIsShow(11, FALSE);
        m_MJPGList.SetUnitIsShow(10, FALSE);

        //	SetTimer(1, 10*1000, NULL);

        return TRUE;  // return TRUE unless you set the focus to a control
        // EXCEPTION: OCX Property Pages should return FALSE
    }

    void CMainPhotoDlg::SetPhoto()
    {
        playerDlg_->SetParam("", mtImage, FALSE);
        playerDlg_->ReSetWindowsRect(CRect(7, 50, 7+522, 50+213));
    }

    class FindJpg : public Util::ProcessFileProcedure {
    public:
        FindJpg(CString dir, CCEListCtrl& imageList)
            : ProcessFileProcedure(dir)
            , imageList_(imageList) {
        }
        virtual void operator()(WIN32_FIND_DATA const& FindFileData) {
            int itemIndex = 0;
            int imageIndex = 0;
            if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                imageIndex = 2;
            } else {
                imageIndex = 0;
            }
            imageList_.InsertItem(itemIndex, FindFileData.cFileName, imageIndex);
            ++itemIndex;
        }
    private:
        CCEListCtrl& imageList_;
    };

    int CMainPhotoDlg::SetPlayList(TCHAR* cdir, int local)
    {
        m_lstPlayList.DeleteAllItems();
        //wcscpy(m_chDir, _T("D:/flashdrv/my_photo/"));
        wcscpy(m_chDir, cdir);
        //wcscat(m_chDir, _T("/"));
        CString dir = cdir;
        ProcessFiles(_T("*.JPG"), FindJpg(dir, m_lstPlayList));
        ProcessFiles(_T("*.BMP"), FindJpg(dir, m_lstPlayList));
        m_lstPlayList.SetScrollRagle(TRUE);
        return 1;
    }
#if 0
    int CMainPhotoDlg::SetPlayList(TCHAR *dir, int local)
    {
        int filecount = 0;
        memset(m_chDir, 0, 128*2);
        m_lstPlayList.DeleteAllItems();
        int ncount = 0;
        if(local == 0)
        {
            if (::DetectDIR(_T("/usbdisk")))
                m_lstPlayList.InsertItem(ncount++, _T("usbdisk"), 3);
            if (::DetectDIR(_T("/storagecard")))
                m_lstPlayList.InsertItem(ncount++, _T("storagecard"), 3);
            memcpy(m_chDir, _T("/flashdrv/my_photo/"), wcslen(_T("/flashdrv/my_photo/"))*2);
        }

        else
        {
            CString s = Util::StringOp::ToCString(Data::LanguageResource::Get(Data::RI_COMN_TOBOTTOM).c_str());
            m_lstPlayList.InsertItem(ncount++, s, 4);
            memcpy(m_chDir, dir, wcslen(dir)*2);
        }
        CString sDir = m_chDir;
        WIN32_FIND_DATA FindFileData;			//查找文件时要使用的数据结构
        HANDLE hFind = INVALID_HANDLE_VALUE;	//定义查找句柄

        sDir += "*.*";
        hFind = FindFirstFile(sDir, &FindFileData);//使用FindFirstFile函数来开始文件查找

        if (hFind == INVALID_HANDLE_VALUE) 
        {
            return 0;
        } 
        else 
        {
            char filename[128];
            //		Dprintf ("First file name is %s\n", FindFileData.cFileName);
            int i = wcstombs( filename, FindFileData.cFileName, 128);
            filename[i] = '\0';
            if(/*strstr(filename, ".bmp")||strstr(filename, ".BMP") ||*/ strstr(filename, ".jpg")||strstr(filename, ".JPG"))
            {
                //对文件的大小经行限制 < 1024K
                //if(((FindFileData.nFileSizeHigh * MAXDWORD) + FindFileData.nFileSizeLow) < 1024*1024)
                {
                    m_lstPlayList.InsertItem(ncount, FindFileData.cFileName, 0);
                    m_lstPlayList.SetItemData(ncount++, 0);
                    filecount++;
                }
            }
            else if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
                CString sS;
                CString s = Util::StringOp::ToCString(Data::LanguageResource::Get(Data::RI_COMN_TOBOTTOM).c_str());
                sS = m_lstPlayList.GetItemText(0, 0);
                if(sS == s)
                    m_lstPlayList.InsertItem(1, FindFileData.cFileName, 2);
                else 
                    m_lstPlayList.InsertItem(0, FindFileData.cFileName, 2);
                ncount++;
            }

            //以下是循环使用FindNextFile函数来查找文件
            while (FindNextFile(hFind, &FindFileData) != 0) 
            {
                //			Dprintf ("Next file name is %s\n", FindFileData.cFileName);
                i = wcstombs( filename, FindFileData.cFileName, /*wcslen(FindFileData.cFileName)*/128);
                filename[i] = '\0';
                if(/*strstr(filename, ".bmp")||strstr(filename, ".BMP") ||*/ strstr(filename, ".jpg")||strstr(filename, ".JPG"))
                {
                    //对文件的大小经行限制 < 500K
                    //if(((FindFileData.nFileSizeHigh * MAXDWORD) + FindFileData.nFileSizeLow) < 1200*1024)
                    {
                        m_lstPlayList.InsertItem(ncount, FindFileData.cFileName, 0);
                        m_lstPlayList.SetItemData(ncount++, 0);
                        filecount++;
                    }
                }
                else if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
                {
                    CString sS;
                    CString s = Util::StringOp::ToCString(Data::LanguageResource::Get(Data::RI_COMN_TOBOTTOM).c_str());
                    sS = m_lstPlayList.GetItemText(0, 0);
                    if(sS == s)
                        m_lstPlayList.InsertItem(1, FindFileData.cFileName, 2);
                    else 
                        m_lstPlayList.InsertItem(0, FindFileData.cFileName, 2);
                    ncount++;
                }
            }

            DWORD dwError = GetLastError();
            if (dwError == ERROR_NO_MORE_FILES) 
            {
                FindClose(hFind);//关闭查找句柄
            } 
            else 
            {
                //			Dprintf ("FindNextFile error. Error is %u\n", dwError);
                return 0;
            }
        }
        m_lstPlayList.SetScrollRagle(TRUE);

        return filecount;
    }
#endif
    void CMainPhotoDlg::OnExit()
    {
        playerDlg_->StopTimer();
        playerDlg_->ShowWindow(SW_HIDE);
        playerDlg_->player_->ExitPlayer(TRUE);

        m_MJPGList.SetUnitIsShow(3, TRUE);
        m_MJPGList.SetUnitIsShow(4, TRUE);
        m_MJPGList.SetUnitIsShow(11, FALSE);
        m_MJPGList.SetUnitIsShow(10, FALSE);

        GetParent()->SendMessage(WM_CHANGEWINDOW, (WPARAM)this, (LPARAM)SW_HIDE);
    }

    void CMainPhotoDlg::OnSelectAll()
    {
        int ncount = m_lstPlayList.GetItemCount();

        LVITEM lvitem;
        lvitem.mask=LVIF_TEXT   |   LVIF_IMAGE;   

        for(int i = 0; i < ncount; i++)
        {
            lvitem.iItem=i;   
            lvitem.iSubItem=0;   
            lvitem.pszText = m_lstPlayList.GetItemText(i, 0).GetBuffer(128);  
            CString s = lvitem.pszText ;
            int n = s.Find(_T(".bmp"));
            int n1 = s.Find(_T(".BMP"));
            int n2 = s.Find(_T(".jpg"));
            int n3 = s.Find(_T(".JPG"));
            if((n1 == -1) && (n == -1) && (n2 == -1) && (n3 == -1))
                continue;

            lvitem.lParam=i;   
            lvitem.iImage=1;
            m_lstPlayList.SetItem(&lvitem);
            m_lstPlayList.SetItemData(i, 1);
        }
    }

    void CMainPhotoDlg::OnClearAll()
    {
        int ncount = m_lstPlayList.GetItemCount();

        LVITEM lvitem;
        lvitem.mask=LVIF_TEXT   |   LVIF_IMAGE;   

        for(int i = 0; i < ncount; i++)
        {
            lvitem.iItem=i;   
            lvitem.iSubItem=0;   
            lvitem.pszText = m_lstPlayList.GetItemText(i, 0).GetBuffer(128); 
            CString s = lvitem.pszText ;
            int n = s.Find(_T(".bmp"));
            int n1 = s.Find(_T(".BMP"));
            int n2 = s.Find(_T(".jpg"));
            int n3 = s.Find(_T(".JPG"));
            if((n1 == -1) && (n == -1) && (n2 == -1) && (n3 == -1))
                continue;
            lvitem.lParam=i;   
            lvitem.iImage=0;
            m_lstPlayList.SetItem(&lvitem);
            m_lstPlayList.SetItemData(i, 0);
        }
    }

    void CMainPhotoDlg::OnClickPlayList(NMHDR* pNMHDR, LRESULT* pResult)
    {
        POSITION pos = m_lstPlayList.GetFirstSelectedItemPosition();
        if (pos != NULL)
        {
            int index = m_lstPlayList.GetNextSelectedItem (pos);
            CString s;
            CString s1 = Util::StringOp::ToCString(Data::LanguageResource::Get(Data::RI_COMN_TOBOTTOM).c_str());
            s = m_lstPlayList.GetItemText(index, 0);
            //usb
            if (s.Compare(_T("usbdisk")) == 0)
            {
                SetPlayList(_T("/usbdisk/"), 1);
            }
            //sd
            else if (s.Compare(_T("storagecard")) == 0)
            {
                SetPlayList(_T("/storagecard/"), 1);
            }
            //上一级
            else if (s.Compare(s1) == 0)
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
                sDir = sDir.Mid(0, old[i - 2] + 1);
                if (sDir == _T("/"))
                    SetPlayList(sDir.GetBuffer(128), 0);
                else
                    SetPlayList(sDir.GetBuffer(128), 1);
            }
            else
            {
                int n = s.Find(_T(".bmp"));
                int n1 = s.Find(_T(".BMP"));
                int n2 = s.Find(_T(".jpg"));
                int n3 = s.Find(_T(".JPG"));
                if ((n1 == -1) && (n == -1) && (n2 == -1) && (n3 == -1))
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
                    lvitem.pszText = (LPWSTR)(LPCTSTR)s;//(LPWSTR)(LPCTSTR)m_lstPlayList.GetItemText(index, 0);   
                    lvitem.lParam = index;   
                    lvitem.iImage = id;
                    m_lstPlayList.SetItem(&lvitem);
                    m_lstPlayList.SetItemData(index, id);
                }
            }
        }

        int ncount = m_lstPlayList.GetItemCount();
        int n = 0;
        for (int i = 0; i < ncount; ++i)
        {
            if (m_lstPlayList.GetItemData(i) == 1)
                ++n;
        }
        m_nCountPhoto = n;
        *pResult = 0;
    }

    //播放
    void CMainPhotoDlg::OnPlayer()
    {
        m_PhotoList.clear();

        int nFileSelected = 0;
        int ncount = m_lstPlayList.GetItemCount();

        for (int i = 0; i < ncount; ++i)
        {
            int val = m_lstPlayList.GetItemData(i);
            if (val == 1)
            {
#if 0
                char filename[128];
                CString s = m_lstPlayList.GetItemText(i, 0);
                int n = wcstombs(filename, (LPTSTR)(LPCTSTR)s, 128/*s.GetLength()*/);
                filename[n] = '\0';

                char file_dir[128];
                n = wcstombs( file_dir, m_chDir, 128);
                file_dir[n] = '\0';

                char txt[128];
                sprintf(txt, "%s/%s",file_dir, filename);

                m_PhotoList.push_back(Util::StringOp::ToCString(txt));
                nFileSelected++;
#else
                m_PhotoList.push_back(CString(m_chDir) + _T("/") + m_lstPlayList.GetItemText(i, 0));
#endif
            }
        }

        if (m_PhotoList.size() != 0)
        {
            std::sort(m_PhotoList.begin(), m_PhotoList.end());
            CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
            main->playerimage_->SetImageList(m_PhotoList);

            //调用播放窗口
            if (playerDlg_)
            {
                SetPhoto();
                playerDlg_->ShowWindow(SW_SHOW);
            }

            m_MJPGList.SetUnitIsShow(3, FALSE);
            m_MJPGList.SetUnitIsShow(4, FALSE);
            m_MJPGList.SetUnitIsShow(11, TRUE);
            m_MJPGList.SetUnitIsShow(10, TRUE);
        }
    }
    //前一条
    void CMainPhotoDlg::OnPre()
    {
        CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
        int n = main->playerimage_->Up();
    }
    //后一条
    void CMainPhotoDlg::OnBack()
    {
        CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
        int n = main->playerimage_->Down();
    }
    //打开播放列表
    void CMainPhotoDlg::OnOpenFile()
    {
        playerDlg_->player_->ExitPlayer(TRUE);

        playerDlg_->StopTimer();
        playerDlg_->ShowWindow(SW_HIDE);

        m_MJPGList.SetUnitIsShow(3, TRUE);
        m_MJPGList.SetUnitIsShow(4, TRUE);
        m_MJPGList.SetUnitIsShow(11, FALSE);
        m_MJPGList.SetUnitIsShow(10, FALSE);

        //m_stcCount.SetWindowText(L"");
    }

    //全屏播放
    void CMainPhotoDlg::OnPlayerAll()
    {
        GetParent()->SendMessage(WM_PLAYPHOTO, 2);	//全屏
        playerDlg_->SetActiveWindow();

        ::SetCursorPos(799, 479);
    }

    void CMainPhotoDlg::OnScreenSave()
    {
        int nFileSelected = 0;
        int ncount = m_lstPlayList.GetItemCount();
        FILE *file =NULL;
        for(int i = 0; i < ncount; i++)
        {
            int val = m_lstPlayList.GetItemData(i);
            if(val == 1)
            {
                /*
                char filename[128];
                CString s = m_lstPlayList.GetItemText(i, 0);
                int n = wcstombs( filename, s.GetBuffer(128), 128);
                filename[n] = '\0';
                */
                char txt[128];
                char filename[128];
                CString s = m_lstPlayList.GetItemText(i, 0);
                int n = wcstombs( filename, s.GetBuffer(128), 128/*s.GetLength()*/);
                filename[n] = '\0';

                char file_dir[128];
                n = wcstombs( file_dir, m_chDir, 128);
                file_dir[n] = '\0';
                sprintf(txt, "%s%s\r\n", file_dir, filename);

                nFileSelected++;
                if(nFileSelected == 1)
                {
                    std::string photoFilename = Util::StringOp::FromCString(::photoPath);
                    photoFilename += "/screensave.pls";
                    file = fopen(photoFilename.c_str(), "w+b");
                    fwrite(txt, sizeof(char), strlen(txt), file);
                }
                else
                {
                    fwrite(txt, sizeof(char), strlen(txt), file);
                }
            }
        }
        if(file)
            fclose(file);
    }

    void CMainPhotoDlg::SetCtrlEnable(BOOL flag)
    {

    }

    void CMainPhotoDlg ::SetAllScreenPlayer(BOOL flag)
    {
        if(flag)
        {
            CRect rt = CRect(0, 0, 800, 480);
            playerDlg_->MoveWindow(&rt);
        }
        else
        {
            CRect rt = CRect(7, 50, 7+522, 50+213);
            playerDlg_->MoveWindow(&rt);
        }

        playerDlg_->player_->SetAllScreenPlayer(flag);
        Invalidate();   //lxz 2007 11 30
    }

    LRESULT CMainPhotoDlg::OnDeviceChange(WPARAM w, LPARAM l)
    {
        LRESULT result = 0;
        if (w == 0x8000) //insert
        {
            ::Sleep(500);

            if (wcscmp(m_chDir, (TCHAR*)(LPCTSTR)::photoPath) == 0)
                SetPlayList((TCHAR*)(LPCTSTR)::photoPath, 0);

        }
        else if (w == 0x8004) //remove
        {
            ::Sleep(1000);
            if (wcscmp(m_chDir, (TCHAR*)(LPCTSTR)::photoPath) == 0)
                SetPlayList((TCHAR*)(LPCTSTR)::photoPath, 0);
        }
        return result;
    }

    LRESULT CMainPhotoDlg ::OnClickMJPG(WPARAM w, LPARAM l)
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
        case 6:			//保存至屏保
            OnScreenSave();
            break;
        case 7:			//退出
            OnExit();
            break;
        case 8:			//打开
            OnOpenFile();
            break;
        case 9:			//全屏
            OnPlayerAll();
            break;
        }
        return result;
    }


    void CMainPhotoDlg::OnTimer(UINT nIDEvent) 
    {
        // TODO: Add your message handler code here and/or call default
        if(nIDEvent == 1 && IsWindowVisible() && playerDlg_->IsWindowVisible())
        {
            CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
            main->playerimage_->Down();
        }

        CDialog::OnTimer(nIDEvent);
    }

    LRESULT CMainPhotoDlg::OnOutEvent(WPARAM w, LPARAM l)
    {
        LRESULT result = 0;
        if(IsWindowVisible())
        {
            if(l == 0)  //暂停播放
            {
                playerDlg_->StopTimer();
                playerDlg_->ShowWindow(SW_HIDE);
                playerDlg_->player_->ExitPlayer(TRUE);
            }
            else if(l == 1) //恢复播放
            {
                playerDlg_->ShowWindow(SW_SHOW);
                CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
                main->playerimage_->Down();
            }
        }
        return result;
    }
}
