// PlayerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PlayerDlg.h"
#include "../MultimediaPhoneDlg.h"
#include "../MultimediaPhone.h"
#include "../Util/ScanDir.h"

#include <algorithm>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace View {
    /////////////////////////////////////////////////////////////////////////////
    // CPlayerDlg dialog

#define PHOTOSHOW_TIME		10*1000
    CPlayerDlg::CPlayerDlg(Player *player, CWnd* pParent /*=NULL*/)
        : CDialog(CPlayerDlg::IDD, pParent)
    {
        //{{AFX_DATA_INIT(CPlayerDlg)
        // NOTE: the ClassWizard will add member initialization here
        memset(m_chVideoFilename, 0, 128);
        isActiveMode_ = 0;
        player_ = player;
        player_->SetOwner(this);
        //}}AFX_DATA_INIT
    }

    void CPlayerDlg::DoDataExchange(CDataExchange* pDX)
    {
        CDialog::DoDataExchange(pDX);
        //{{AFX_DATA_MAP(CPlayerDlg)
        // NOTE: the ClassWizard will add DDX and DDV calls here
        //}}AFX_DATA_MAP
    }

    BEGIN_MESSAGE_MAP(CPlayerDlg, CDialog)
        //{{AFX_MSG_MAP(CPlayerDlg)
        ON_WM_LBUTTONDOWN()
        ON_WM_CHAR()
        ON_WM_PAINT()
        ON_WM_TIMER()
        ON_MESSAGE(WM_USER+100, OnReceiveMsg)
        ON_WM_ACTIVATE()
        ON_WM_LBUTTONDOWN()
        ON_WM_KEYDOWN()
        //}}AFX_MSG_MAP
    END_MESSAGE_MAP()

    /////////////////////////////////////////////////////////////////////////////
    // CPlayerDlg message handlers
    void CPlayerDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
    {
        // TODO: Add your message handler code here and/or call default
        CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
        if(player_->mt_ == mtImage && isActiveMode_) //图片 && 手动
        {
            if(nChar == 'U')			//page up
            {
                StopTimer();
                KillTimer(0x1003);
                SetTimer(0x10003, 60*1000, NULL);
                player_->Up();	
            }
            else if(nChar == 'D')		//page down
            {
                StopTimer();
                KillTimer(0x1003);
                SetTimer(0x1003, 60*1000, NULL);
                player_->Down();
            }
        }

        CDialog::OnKeyDown(nChar, nRepCnt, nFlags);
    }

    void CPlayerDlg::OnLButtonDown(UINT nFlags, CPoint point) 
    {
        // TODO: Add your message handler code here and/or call default
        HWND hWnd;
        CDialog::OnLButtonDown(nFlags, point);
        CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
        if(player_->mt_ == mtImage)
        {
            main->m_pMainDlg->SendMessage(WM_PLAYPHOTO, 3);  //退出全屏
        }
    }

    BOOL CPlayerDlg::OnInitDialog() 
    {
        CDialog::OnInitDialog();

        return TRUE;  // return TRUE unless you set the focus to a control
        // EXCEPTION: OCX Property Pages should return FALSE
    }

    LRESULT CPlayerDlg::OnReceiveMsg(WPARAM w, LPARAM l)
    {
        LRESULT result = 0;
        return result;
    }

    void CPlayerDlg::OnOk(void)
    {
    }

    void CPlayerDlg::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
    {
        HWND hWnd;
        CDialog::OnChar(nChar, nRepCnt, nFlags);
    }

    void CPlayerDlg::AddImageList(char *picDir[], int ncount)
    {
        std::vector<CString> list1;
        for(int i = 0; i < ncount; i++)
        {
            if(i == 0)
                list1 = SetImageList_(picDir[i]);
            else
            {
                std::vector<CString> list = SetImageList_(picDir[i]);
                list1.insert(list1.end(), list.begin(), list.end());
            }
        }
        CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
        player_->SetImageList(list1);
    }

    BOOL CPlayerDlg::SetImageListForScreenSave(char *FileName)
    {
        FILE *file;
        if((file = fopen(FileName, "r+b")) != NULL)
        {
            CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
            std::vector<CString> filelist;
            char name[128];
            memset(name, 0, 128);
            while(!feof(file))
            {
                if(fgets(name, 128, file))
                {
                    int n = strlen(name);
                    name[n-2] = '\0';
                    CString sf = Util::StringOp::ToCString(name);
                    filelist.push_back(sf);
                }
            }
            fclose(file);
            std::sort(filelist.begin(), filelist.end());
            player_->SetImageList(filelist);
            return TRUE;
        }
        else
        {
            //SetImageList((TCHAR*)(LPCTSTR)::photoPath);
            SetImageList((char*)Util::StringOp::FromCString(::photoPath).c_str());
        }
        return FALSE;
    }

    void CPlayerDlg::SetImageList(char *DIR)
    {
        CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
        player_->SetImageList(SetImageList_(DIR));
    }

    class ImageFileLister : public Util::ProcessFileProcedure
    {
    public:
        ImageFileLister(CString const directoryName, std::vector<CString>& result)
        : ProcessFileProcedure(directoryName)
        , result_(result)
        {
        }

        void operator()(WIN32_FIND_DATA const& FindFileData)
        {
            CString file = directoryName_ + _T("/") + FindFileData.cFileName;
            result_.push_back(file);
        }
    private:
        std::vector<CString>& result_;
    };

    std::vector<CString> CPlayerDlg::SetImageList_(char* DIR)
    {
        std::vector<CString> result;
        ImageFileLister fl(Util::StringOp::ToCString(DIR), result);
        ProcessFiles(_T("*.jpg"), fl);
        ProcessFiles(_T("*.bmp"), fl);
        return result;
    }
#if 0
    std::vector<CString> CPlayerDlg::SetImageList_(char *DIR)
    {
        std::vector<CString> filelist;

        WIN32_FIND_DATA FindFileData;			//查找文件时要使用的数据结构
        HANDLE hFind = INVALID_HANDLE_VALUE;	//定义查找句柄
        //	char filename[MAX_PATH]; 
        int ncount = 0;

        char totalfilename[128];
        sprintf(totalfilename, "%s/*.*", DIR);
        CString sDir = Util::StringOp::ToCString(totalfilename);
        hFind = FindFirstFile(sDir, &FindFileData);//使用FindFirstFile函数来开始文件查找

        if (hFind == INVALID_HANDLE_VALUE) 
        {
            //	  printf ("Inval  return 0\n;");
            return filelist;
        } 
        else 
        {
            char filename[128];
            //printf ("First file name is %s\n", FindFileData.cFileName);
            int i = wcstombs( filename, FindFileData.cFileName, /*wcslen(FindFileData.cFileName)*/128);
            filename[i] = '\0';
            if (strstr(filename, ".jpg")||strstr(filename, ".JPG")/*||strstr(filename, ".bmp")||strstr(filename, ".BMP")*/)
            {
                sprintf(totalfilename, "%s/%s", DIR, filename);
                CString sf = Util::StringOp::ToCString(totalfilename);
                filelist.push_back(sf);
            }

            //以下是循环使用FindNextFile函数来查找文件
            while (FindNextFile(hFind, &FindFileData) != 0) 
            {
                //printf ("Next file name is %s\n", FindFileData.cFileName);
                i = wcstombs( filename, FindFileData.cFileName, /*wcslen(FindFileData.cFileName)*/128);
                filename[i] = '\0';
                if (strstr(filename, ".jpg")||strstr(filename, ".JPG")/*||strstr(filename, ".bmp")||strstr(filename, ".BMP")*/)
                {
                    sprintf(totalfilename, "%s/%s", DIR, filename);
                    CString sf = Util::StringOp::ToCString(totalfilename);
                    filelist.push_back(sf);
                }
            }

            std::sort(filelist.begin(), filelist.end());

            DWORD dwError = GetLastError();
            if (dwError == ERROR_NO_MORE_FILES) 
            {
                FindClose(hFind);//关闭查找句柄
            } 
            else 
            {
                //printf ("FindNextFile error. Error is %u\n", dwError);
                return filelist;
            }
        }

        return filelist;
    }
#endif
    void CPlayerDlg::OnPaint() 
    {
        CPaintDC dc(this); // device context for painting
        CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg *)theApp.m_pMainWnd;
        if(player_->mt_ == mtImage)
        {
            KillTimer(0x10002);
            SetTimer(0x10002, PHOTOSHOW_TIME, NULL);
            player_->PlayerImage();
        }

        //CDialog::OnPaint();
    }

    void CPlayerDlg::OnTimer(UINT nIDEvent) 
    {
        // TODO: Add your message handler code here and/or call default

        if (nIDEvent == 0x10002 && IsWindowVisible())
        {
            CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
            //lxz 20080710
            if ((player_->mt_ == mtImage) && IsWindowVisible() && player_->owner_ == this)
            {
                player_->Down();
            }
        }
        else if(nIDEvent == 0x1003 && IsWindowVisible())
        {
            KillTimer(0x1003);
            SetResumeTimer();
        }

        CDialog::OnTimer(nIDEvent);
    }

    void CPlayerDlg::OnOK()
    {
    }

    void CPlayerDlg::OnCancel()
    {
    }

    void CPlayerDlg::SetParam(char *videoFile, MediaType isVideo, BOOL isSaveScreen)
    {
        CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
        player_->isActiveMode_ = !isSaveScreen;
        player_->mt_ = isVideo;

        isActiveMode_ = !isSaveScreen;
        player_->SetOwner(this);
        if (strlen(videoFile) != 0)
            strcpy(m_chVideoFilename, videoFile);

        if (!isActiveMode_)  //屏保
        {
            if (player_->mt_ == mtImage)	//图片
                SetTimer(0x10002, PHOTOSHOW_TIME, NULL);
        }
    }

    void CPlayerDlg::ReSetWindowsRect(CRect rt)
    {
        MoveWindow(&rt, FALSE);
    }

    void CPlayerDlg::Show()
    {
        CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
        player_->SetOwner(this);
        ShowWindow(SW_SHOW);

        //if(player_->mt_ == mtVideo)
        {
            player_->InitPlayer();
            player_->PlayerFile(m_chVideoFilename);
        }
    }

    void CPlayerDlg::Hide()
    {
        CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
        ShowWindow(SW_HIDE);
    }

    void CPlayerDlg::OnShowWindow(BOOL bShow, UINT nStatus)
    {
        if(bShow)
        {
            Show();
        }
        else
        {
            Hide();
        }
    }

    void CPlayerDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
    {
        CDialog::OnActivate(nState, pWndOther, bMinimized);
    }

    void CPlayerDlg::SetResumeTimer()
    {
        CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
        if(player_->mt_ == mtImage)	//图片
            SetTimer(0x10002, PHOTOSHOW_TIME, NULL);
    }

    void CPlayerDlg::StopTimer()
    {
        CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
        if(player_->mt_ == mtImage)	//图片
            KillTimer(0x10002);
    }
}
