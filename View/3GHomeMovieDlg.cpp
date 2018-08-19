// 3GHomeMovieDlg.cpp : implementation file
//

#include "stdafx.h"
#include "../multimediaphone.h"
#include "../MultimediaPhoneDlg.h"
#include "3GHomeMovieDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// C3GHomeMovieDlg dialog
extern CString g_destDir;
extern CString g_srcDir;
extern CString g_showDir;
extern BOOL DetectDIR(TCHAR* sDir);
extern BOOL DetectFile(TCHAR* sDir);
extern DWORD GetFileSize(TCHAR* sFile);
extern DWORD GetDirSize(TCHAR* sDir, DWORD& fSize);

C3GHomeMovieDlg::C3GHomeMovieDlg(CWnd* pParent /*=NULL*/)
: CDialog(C3GHomeMovieDlg::IDD, pParent) {
    //{{AFX_DATA_INIT(C3GHomeMovieDlg)
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT

    m_pageCount = 0;
    m_fileCount = 0;
    m_currentPage = 0;
    m_firstSelPage = 0;
    m_lastSelPage = 0;
    m_isSelectAll = FALSE;
    m_pageSize = 5;
    m_offSet = 0;
    m_cardType1 = 0;
    m_cardType2 = 0;
}


void C3GHomeMovieDlg::DoDataExchange(CDataExchange* pDX) {
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(C3GHomeMovieDlg)
    // NOTE: the ClassWizard will add DDX and DDV calls here
    //}}AFX_DATA_MAP

}


BEGIN_MESSAGE_MAP(C3GHomeMovieDlg, CDialog)
    //{{AFX_MSG_MAP(C3GHomeMovieDlg)
    ON_MESSAGE(WM_MJPGTOGGLE, OnClickMJPG)
    ON_MESSAGE(WM_CLICKMJPG_TOAPP, OnClickMJPG)
    ON_MESSAGE(WM_USB_MSG, OnDeviceChange)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// C3GHomeMovieDlg message handlers

BOOL C3GHomeMovieDlg::OnInitDialog() {
    CDialog::OnInitDialog();

    // TODO: Add extra initialization here

    m_MJPGList.Create(L"", WS_VISIBLE | WS_CHILD, CRect(0, 0, 800, 423), this);
    m_MJPGList.SetCurrentLinkFile(".\\adv\\mjpg\\k5\\中文\\影院.xml");

    MoveWindow(0, 57, 800, 423);

    SetUnitFont();
    m_MJPGList.SetUnitText(8, L"当前页", TRUE);
    m_MJPGList.SetUnitIsDisable(1, TRUE);
    m_MJPGList.SetUnitIsDisable(2, TRUE);
    m_MJPGList.SetUnitIsShow(5, TRUE);
    m_MJPGList.SetUnitIsShow(6, TRUE);
    m_uiType = 0;
    ShowItemsInList(m_uiType);
    PostMessage(WM_USB_MSG, 0x8000, 0);
    PostMessage(WM_USB_MSG, 0x8000, 0);
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

LRESULT C3GHomeMovieDlg::OnClickMJPG(WPARAM w, LPARAM l) {
    LRESULT result = 0;
    CMultimediaPhoneDlg* pMainDlg = ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd));
    switch (w) {
    case 0:
    case 1:
    case 2:
        m_MJPGList.SetUnitIsDownStatus(50, FALSE);
        m_isSelectAll = FALSE;
        m_uiType = w;
        ShowItemsInList(w);
        break;
    case 3:
        //选择当前页数
        break;
    case 5:
        //上翻
        PageUp();
        break;
    case 6:
        //下翻
        PageDown();
        break;
    case 10:
        //播放
        FindSelectPage();
        if (m_firstSelPage > 0) {
            pMainDlg->m_pMainDlg->m_mainVideoDlg_->OnOpenFile();
            pMainDlg->m_pMainDlg->m_mainmenuDlg_->OnVideoBtn();
            OnPlayer();
        }
        break;
    case 11:
        //转移文件
        FindSelectPage();
        if (m_firstSelPage > 0) {
            if (0 == m_uiType) {
                pMainDlg->m_pShiftFileDlg->m_MJPGList.SetUnitIsDisable(10, FALSE);
                pMainDlg->m_pShiftFileDlg->m_MJPGList.SetUnitIsDownStatus(10, FALSE);
                pMainDlg->m_pShiftFileDlg->m_MJPGList.SetUnitIsDisable(11, FALSE);
                pMainDlg->m_pShiftFileDlg->m_MJPGList.SetUnitIsDownStatus(11, FALSE);
                pMainDlg->m_pShiftFileDlg->m_MJPGList.SetUnitIsDownStatus(20, FALSE);
                pMainDlg->m_pShiftFileDlg->m_MJPGList.SetUnitIsDisable(20, TRUE);
                pMainDlg->m_pShiftFileDlg->m_MJPGList.SetUnitIsDownStatus(21, FALSE);
                pMainDlg->m_pShiftFileDlg->m_MJPGList.SetUnitIsDisable(21, TRUE);
            } else {
                  pMainDlg->m_pShiftFileDlg->m_MJPGList.SetUnitIsDisable(20, FALSE);
                  pMainDlg->m_pShiftFileDlg->m_MJPGList.SetUnitIsDownStatus(20, FALSE);
                  pMainDlg->m_pShiftFileDlg->m_MJPGList.SetUnitIsDisable(21, FALSE);
                  pMainDlg->m_pShiftFileDlg->m_MJPGList.SetUnitIsDownStatus(21, FALSE);
                  pMainDlg->m_pShiftFileDlg->m_MJPGList.SetUnitIsDownStatus(10, FALSE);
                  pMainDlg->m_pShiftFileDlg->m_MJPGList.SetUnitIsDisable(10, TRUE);
                  pMainDlg->m_pShiftFileDlg->m_MJPGList.SetUnitIsDownStatus(11, FALSE);
                  pMainDlg->m_pShiftFileDlg->m_MJPGList.SetUnitIsDisable(11, TRUE);
            }
            OnShiftFile();
        }
        break;
    case 12:
        //删除
        FindSelectPage();
        if (m_firstSelPage > 0) {
            OnClickDelete();
        }
        break;
    case 50:
        OnSelectAll();
        break;
    case 100:
        //选择
    case 200:
    case 300:
    case 400:
    case 500:
        if (m_UnitStatus[w / 100 - 1]) {
            m_offSet = w / 100 - 1;
            OnButtonCheck(w);
        }
        break;
    case 1000:
        //返回
        OnExit_();
        break;
    case 1001:
        //确定
        break;
    default:
        break;
    }
    return result;
}

void C3GHomeMovieDlg::SetUnitFont() {
    for (int i = 2; i <= 4; i++) {
        m_MJPGList.SetUnitFont(i, font_14);
        m_MJPGList.SetUnitColor(i, font_white, TRUE);
    }

    for (int i = 1; i <= 5; i++) {
        m_MJPGList.SetUnitFont(i * 100 + 1, font_18);
        m_MJPGList.SetUnitColor(i * 100 + 1, font_white, TRUE);
        m_MJPGList.SetUnitFont(i * 100 + 2, font_18);
        m_MJPGList.SetUnitColor(i * 100 + 2, font_white, TRUE);
    }
}

void C3GHomeMovieDlg::AddFileMovie() {
    int dirCount = m_fileDir.size();
    std::vector<CString>::iterator dir;
    std::vector<int>::iterator idir;
    for (int i = 0; i < dirCount; i++) {
        dir = m_fileMovie.begin();
        idir = m_isFileSelect.begin();
        m_fileMovie.insert(dir, m_fileDir[i]);
        m_isFileSelect.insert(idir, 0);
    }
}

void C3GHomeMovieDlg::SetInitShow() {
    m_fileCount = m_fileDir.size() + m_fileMovie.size();
    int count = m_fileCount;
    if (0 == m_fileCount) {
        m_currentPage = 0;
    } else {
        m_currentPage = 1;
    }
    if (0 == count % m_pageSize) {
        m_pageCount = count / m_pageSize;
    } else {
        m_pageCount = count / m_pageSize + 1;
    }

    CString str;
    str.Format(L"/%d", m_pageCount);
    m_MJPGList.SetUnitText(4, str, TRUE);
    str.Empty();
    str.Format(L"%d", m_currentPage);
    m_MJPGList.SetUnitText(3, str, TRUE);

    if (m_currentPage <= 1)   //当第一次显示时上翻按钮不可用
    {
        m_MJPGList.SetUnitIsDisable(5, TRUE);
    } else {
        m_MJPGList.SetUnitIsDisable(5, FALSE);
    }
    if (m_fileCount <= m_pageSize)      //当总数小于6时设置下翻按钮不可用
    {
        m_MJPGList.SetUnitIsDisable(6, TRUE);
    } else {
        m_MJPGList.SetUnitIsDisable(6, FALSE);
    }
}

void C3GHomeMovieDlg::ShowItemsInList(int type) {
    if (type == -1) {
        type = m_uiType;
    } else {
        m_uiType = type;
    }

    if (type == 0) {
        SetPlayList(my_video_dir);
    } else if (type == 1) {
        if (1 == m_cardType1) {
            SetPlayList(_T("/usbdisk/"));
        } else if (2 == m_cardType1) {
            SetPlayList(_T("/storagecard/"));
        }
    } else if (type == 2) {
        if (1 == m_cardType2) {
            SetPlayList(_T("/usbdisk/"));
        } else if (2 == m_cardType2) {
            SetPlayList(_T("/storagecard/"));
        }
    }

    for (int i = 0; i < 3; ++i) {
        if (i == m_uiType) {
            m_MJPGList.SetUnitIsDownStatus(i, TRUE);
        } else {
            m_MJPGList.SetUnitIsDownStatus(i, FALSE);
        }
    }
    m_MJPGList.Invalidate();

    SetInitShow();
    AddFileMovie();
    ShowArrayInList(m_fileMovie);
}

void C3GHomeMovieDlg::ShowArrayInList(std::vector<CString> fileName) {
    OnClearAll();
    int index = 100;
    if (m_fileCount > 0) {
        CString temp;
        DWORD fileSize;
        for (size_t i = (m_currentPage - 1) * m_pageSize; i < m_currentPage* m_pageSize; i++) {
            fileSize = 0;
            if (fileName.begin() + i < fileName.end()) {
                CString path = m_chDir + m_fileMovie[i];

                m_UnitStatus[index / 100 - 1] = TRUE;
                SetShowStatus(index, TRUE);
                m_MJPGList.SetUnitText(index + 1, m_fileMovie[i], FALSE);
                if (i < m_fileDir.size()) {
                    m_MJPGList.SetUnitBitmap(index, ".\\adv\\mjpg\\k5\\common\\png\\文件夹.png", L".\\adv\\mjpg\\k5\\common\\png\\文件夹.png", FALSE);
                    fileSize = GetDirSize((LPTSTR)(LPCTSTR)path, fileSize);
                    fileSize /= (1024 * 1024);
                    if (fileSize < 1.0 && fileSize > 0.0000001) {
                        fileSize *= 1024;
                        temp.Format(_T("%0.1fK"), fileSize);
                    } else if (fileSize >= -0.0000001 && fileSize <= 0.0000001) {
                          temp = L"0字节";
                    } else {
                        temp.Format(_T("%0.1fM"), fileSize);
                    }
                    m_MJPGList.SetUnitText(index + 2, temp, FALSE);
                } else {
                      m_MJPGList.SetUnitBitmap(index, L"", L"", FALSE);

                      fileSize = GetFileSize((LPTSTR)(LPCTSTR)path);
                      fileSize /= (1024 * 1024);
                    if (fileSize < 1.0 && fileSize > 0.0000001) {
                        fileSize *= 1024;
                        temp.Format(_T("%0.1fK"), fileSize);
                    } else if (fileSize >= -0.0000001 && fileSize <= 0.0000001) {
                          temp = L"0字节";
                    } else {
                        temp.Format(_T("%0.1fM"), fileSize);
                    }
                      m_MJPGList.SetUnitText(index + 2, temp, FALSE);

                    if (1 == m_isFileSelect[i]) {
                        SetShowStatus(index, FALSE);
                    }
                }
            } else {
                m_MJPGList.SetUnitBitmap(index, L"", L"", FALSE);
                m_UnitStatus[index / 100 - 1] = FALSE;
                SetShowStatus(index, TRUE);
            }

            index += 100;
        }
    } else {
        for (size_t i = 1; i <= m_pageSize; i++) {
            m_MJPGList.SetUnitBitmap(i * 100, L"", L"", FALSE);
            m_UnitStatus[i - 1] = FALSE;
            SetShowStatus(i * 100, TRUE);
        }
    }

    m_MJPGList.Invalidate();
}

void C3GHomeMovieDlg::OnClearAll() {
    for (size_t i = 1; i <= m_pageSize; i++) {
        m_MJPGList.SetUnitText(i * 100 + 1, L"", TRUE);
        m_MJPGList.SetUnitText(i * 100 + 2, L"", TRUE);
    }
}

void C3GHomeMovieDlg::SetShowStatus(int unitNO, BOOL status) {
    if (m_UnitStatus[unitNO / 100 - 1]) {
        if (status)  //设置为前景
        {
            m_MJPGList.SetUnitIsDisable(unitNO, FALSE);
            m_MJPGList.SetUnitIsDownStatus(unitNO, FALSE);
        } else    //设置为后景
        {
            m_MJPGList.SetUnitIsDisable(unitNO, FALSE);
            m_MJPGList.SetUnitIsDownStatus(unitNO, TRUE);
        }

        if (!m_MJPGList.GetUnitText(unitNO + 1).IsEmpty() || !m_MJPGList.GetUnitText(unitNO + 2).IsEmpty()) {
            CString strText;
            m_MJPGList.SetUnitIsDisable(unitNO + 1, FALSE);
            strText = m_MJPGList.GetUnitText(unitNO + 1);
            m_MJPGList.SetUnitText(unitNO + 1, strText, FALSE);
            strText.Empty();
            strText = m_MJPGList.GetUnitText(unitNO + 2);
            m_MJPGList.SetUnitText(unitNO + 2, strText, FALSE);
        }
    } else {
        m_MJPGList.SetUnitIsDownStatus(unitNO, FALSE);
        m_MJPGList.SetUnitIsDisable(unitNO, TRUE);

        m_MJPGList.SetUnitText(unitNO + 1, L"", FALSE);
        m_MJPGList.SetUnitText(unitNO + 2, L"", FALSE);
        m_MJPGList.SetUnitIsDisable(unitNO + 1, TRUE);
        m_MJPGList.SetUnitIsDisable(unitNO + 2, TRUE);
    }

    m_MJPGList.Invalidate();
}

int C3GHomeMovieDlg::SetPlayList(TCHAR* dir) {
    g_srcDir = L"";
    g_srcDir = dir;     //转移文件时的源文件目录

    if (!m_fileMovie.empty()) {
        m_fileMovie.clear();
        m_fileDir.clear();
        m_isFileSelect.clear();
    }
    memset(m_chDir, 0, 128 * 2);
    int ncount = 0;
    memcpy(m_chDir, g_srcDir, wcslen(g_srcDir) * 2);

    CString sDir = m_chDir;
    WIN32_FIND_DATA FindFileData;           //查找文件时要使用的数据结构
    HANDLE hFind = INVALID_HANDLE_VALUE;    //定义查找句柄

    sDir += "*.*";
    hFind = FindFirstFile(sDir, &FindFileData);//使用FindFirstFile函数来开始文件查找

    if (hFind == INVALID_HANDLE_VALUE) {
        return 0;
    } else {
        char filename[128] = {
            0
        };
        size_t convertCount = 0;
        int i = wcstombs_s(&convertCount, filename, 128, FindFileData.cFileName, 128);
          filename[i] = '\0';
        if (strstr(filename, ".MP4") || strstr(filename, ".Mp4") || strstr(filename, ".mp4") || strstr(filename, ".rm") || strstr(filename, ".RM") || strstr(filename, ".AVI") || strstr(filename, ".avi")) {
            m_fileMovie.push_back(FindFileData.cFileName);
            m_isFileSelect.push_back(0);
            ncount++;
        } else if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            m_fileDir.push_back(FindFileData.cFileName);
            ncount++;
        }

          //以下是循环使用FindNextFile函数来查找文件
        while (FindNextFile(hFind, &FindFileData) != 0) {
            i = wcstombs_s(&convertCount, filename, 128, FindFileData.cFileName, /*wcslen(FindFileData.cFileName)*/128);
            filename[i] = '\0';
            if (strstr(filename, ".MP4") || strstr(filename, ".Mp4") || strstr(filename, ".mp4") || strstr(filename, ".rm") || strstr(filename, ".RM") || strstr(filename, ".AVI") || strstr(filename, ".avi")) {
                m_fileMovie.push_back(FindFileData.cFileName);
                m_isFileSelect.push_back(0);
                ncount++;
            } else if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                  m_fileDir.push_back(FindFileData.cFileName);
                  ncount++;
            }
        }

        DWORD dwError = GetLastError();
        if (dwError == ERROR_NO_MORE_FILES) {
            FindClose(hFind);//关闭查找句柄
        } else {
            //Dprintf ("FindNextFile error. Error is %u\n", dwError);
            return 0;
        }
    }
    return ncount;
}

void C3GHomeMovieDlg::OnSelectAll() {
    //wangzhenxing0923
    if (m_fileCount > 0) {
        if (!m_isSelectAll) {
            m_isSelectAll = TRUE;
            m_firstSelPage = 1;
            m_lastSelPage = m_pageCount;
            m_MJPGList.SetUnitIsDownStatus(50, TRUE);
            for (size_t i = 0; i < m_fileCount; i++) {
                m_isFileSelect[i] = 1;
            }
        } else {
            m_isSelectAll = FALSE;
            m_firstSelPage = 0;
            m_lastSelPage = 0;
            m_MJPGList.SetUnitIsDownStatus(50, FALSE);
            for (size_t i = 0; i < m_fileCount; i++) {
                m_isFileSelect[i] = 0;
            }
        }
        for (int i = 100; i <= 500; i += 100) {
            SetShowStatus(i, !m_isSelectAll);
        }
    }
}

void C3GHomeMovieDlg::OnButtonCheck(int uintNO) {
    size_t index = (m_currentPage - 1) * m_pageSize + m_offSet;
    if (m_fileCount > 0) {
        if (index >= m_fileDir.size()) {
            if (m_isFileSelect[index] == 0) {
                m_isFileSelect[index] = 1;
                SetShowStatus(uintNO, FALSE);
            } else {
                m_isFileSelect[index] = 0;
                SetShowStatus(uintNO, TRUE);
            }
        } else {
            CString dir = m_chDir + m_MJPGList.GetUnitText(uintNO + 1) + "/";
            SetPlayList(dir.GetBuffer(dir.GetLength()));
            SetInitShow();
            AddFileMovie();
            ShowArrayInList(m_fileMovie);
        }
    }
}

void C3GHomeMovieDlg::FindSelectPage() {
    BOOL find = FALSE;
    if (!m_isSelectAll) {
        for (size_t i = 0; i < m_fileCount; i++) {
            if (1 == m_isFileSelect[i]) {
                if (0 == m_firstSelPage) {
                    m_firstSelPage = i / m_pageSize + 1;
                }
                m_lastSelPage = i / m_pageSize + 1;
                find = TRUE;
            }
        }
        if (!find) {
            m_firstSelPage = 0;
            m_lastSelPage = 0;
        }
    }
}

void C3GHomeMovieDlg::PageUp() {
    m_currentPage--;

    if (m_currentPage <= 1) {
        m_currentPage = 1;
        m_MJPGList.SetUnitIsDisable(5, TRUE);
    }
    if (m_currentPage * m_pageSize < m_fileCount) {
        m_MJPGList.SetUnitIsDisable(6, FALSE);
    }

    CString str;
    str.Format(L"%d", m_currentPage);
    m_MJPGList.SetUnitText(3, str, TRUE);
    str.Empty();

    OnClearAll();
    ShowArrayInList(m_fileMovie);
}

void C3GHomeMovieDlg::PageDown() {
    m_currentPage++;   

    if (m_currentPage > 1) {
        m_MJPGList.SetUnitIsDisable(5, FALSE);
    }
    if (m_currentPage * m_pageSize > m_fileCount) {
        m_MJPGList.SetUnitIsDisable(6, TRUE);
        for (size_t i = m_pageSize; i > m_fileCount - (m_currentPage - 1) * m_pageSize; i--) {
            int row = i*100;
            m_MJPGList.SetUnitIsDownStatus(row, FALSE);
            OnClearAll();
        }
    } else if (m_currentPage * m_pageSize == m_fileCount) {
        m_MJPGList.SetUnitIsDisable(6, TRUE);
    }

    CString str;
    str.Format(L"%d", m_currentPage);
    m_MJPGList.SetUnitText(3, str, TRUE);
    str.Empty();

    OnClearAll();
    ShowArrayInList(m_fileMovie);
}

void C3GHomeMovieDlg::OnClickDelete() {
    if (0 == m_firstSelPage) {
        return;
    }
    int count = 0;
    for (size_t i = (m_firstSelPage - 1) * m_pageSize; i < m_lastSelPage* m_pageSize; i++) {
        if (1 == m_isFileSelect[i] && i >= m_fileDir.size()) {
            count++;
        }
    }
    ((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetTitle(L"确定要删除已选文件吗？", 0);
    ((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetHWnd(this->GetSafeHwnd());
    ((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetProcessMax(count);
    if (((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->m_pSettingDlg->m_pSetting->isAdmin() && !((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->m_pSettingDlg->m_bLogin) {
        ((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetPasswordModel(true);
    }
    std::string pw = ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pSettingDlg->m_pSetting->adminPassword();
    ((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetPassword(Util::StringOp::ToCString(pw));
    ((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->m_pDeleteTipDlg->ShowWindow_(TRUE);
}
extern void DeleteDirectory(CString SrcDir, BOOL isShow = TRUE);
void C3GHomeMovieDlg::DeleteSelectedItem() {
    size_t pageStartIndex = m_fileDir.size();
    std::vector<int>::iterator checkItem = m_isFileSelect.begin() + pageStartIndex;
    std::vector<CString>::iterator checkFile = m_fileMovie.begin() + pageStartIndex;
    int delCount = 0;
    for (int i = pageStartIndex; (checkItem != m_isFileSelect.end()) && (checkItem < (m_isFileSelect.begin() + m_lastSelPage* m_pageSize));) {
        if (*checkItem == 1) {
            CString s = m_chDir + m_fileMovie[i];
            DeleteFile(s);
            if (0 == GetLastError()) {
                delCount++;
                checkFile = m_fileMovie.erase(checkFile);
                checkItem = m_isFileSelect.erase(checkItem);
            } else {
                ++checkItem;
                ++checkFile;
                ++i;
            }
            ((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetProcessPos(delCount);
        } else {
            ++checkItem;
            ++checkFile;
            ++i;
        }
    }

    //从新求当前总记录数
    m_fileCount = m_fileMovie.size();
    if (0 == m_fileCount) {
        m_MJPGList.SetUnitText(4, "/0", TRUE);
        m_MJPGList.SetUnitText(3, "0", TRUE);
        m_MJPGList.SetUnitIsDisable(5, TRUE);
        m_MJPGList.SetUnitIsDisable(6, TRUE);
        m_pageCount = 0;
        m_currentPage = 0;
    } else {
        if (0 == m_fileCount % m_pageSize) {
            m_pageCount = m_fileCount / m_pageSize;
        } else {
            m_pageCount = m_fileCount / m_pageSize + 1;
        }
        CString str;
        str.Format(L"/%d", m_pageCount);
        m_MJPGList.SetUnitText(4, str, TRUE);
        if (m_fileCount <= m_pageSize) {
            m_MJPGList.SetUnitIsDisable(6, TRUE);
        }
        if ((m_currentPage - 1) * m_pageSize >= m_fileMovie.size()) {
            m_currentPage = m_pageCount;
        }
        str.Empty();
        str.Format(L"%d", m_currentPage);
        m_MJPGList.SetUnitText(3, str, TRUE);
        //检查当前页的情况
        if (m_currentPage == m_pageCount) {
            m_MJPGList.SetUnitIsDisable(6, TRUE);
        } else {
            m_MJPGList.SetUnitIsDisable(6, FALSE);
        }
        if (m_currentPage == 1) {
            m_MJPGList.SetUnitIsDisable(5, TRUE);
        } else {
            m_MJPGList.SetUnitIsDisable(5, FALSE);
        }
    }
    ShowArrayInList(m_fileMovie);
}

void C3GHomeMovieDlg::OnShiftFile() {
    g_srcDir = L"";
    g_showDir = L"";

    if (0 == m_firstSelPage) {
        return;
    }
    if (!DetectDIR(_T("\\StorageCard")) && !DetectDIR(_T("\\UsbDisk"))) {
        ((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->m_pWarningNoFlashDlg->SetTitle(_T("请插入移动设备"));
        ((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->m_pWarningNoFlashDlg->SetHWnd(this->GetSafeHwnd());
        ((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->m_pWarningNoFlashDlg->ShowWindow_(TRUE);
        return;
    }

    if (0 == m_uiType) {
        if (DetectDIR(L"/usbdisk")) {
            g_destDir = L"/UsbDisk/";
            g_showDir = L"USBDISK";
        } else if (DetectDIR(L"/storagecard")) {
            g_destDir = L"/StorageCard/";
            g_showDir = L"STORAGECARD";
        }
    } else if (1 == m_uiType) {
        if (1 == m_cardType1) {
            g_showDir = L"USBDISK";
        } else if (2 == m_cardType1) {
            g_showDir = L"STORAGECARD";
        }

        g_destDir = my_video_dir;
    } else if (2 == m_uiType) {
        if (1 == m_cardType2) {
            g_showDir = L"USBDISK";
        } else if (2 == m_cardType2) {
            g_showDir = L"STORAGECARD";
        }

        g_destDir = my_video_dir;
    }

    CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
    main->CancelBalckLightSaveTimer();
    main->m_pShiftFileDlg->m_shiftProc->SetPos(0);
    main->m_pShiftFileDlg->SetHWnd(this->GetSafeHwnd());
    main->m_pShiftFileDlg->ShowWindow_(TRUE);

    main->m_pShiftFileDlg->m_fileList.clear();
    int nFileSelected = 0;
    for (size_t loop = 0; loop < m_fileCount; loop++) {
        if (m_isFileSelect[loop] == 1) {
            char filename[128] = {
                0
            };
            size_t convertCount = 0;
            int n = wcstombs_s(&convertCount, filename, 128, (LPTSTR)(LPCTSTR)m_fileMovie[loop], 128);
            filename[n] = '\0';
            main->m_pShiftFileDlg->m_fileList.push_back(filename);
            nFileSelected++;
        }
    }

    CString s;
    s.Format(L"/%d", nFileSelected);
    main->m_pShiftFileDlg->m_MJPGList.SetUnitText(5, s, FALSE);
    main->m_pShiftFileDlg->m_MJPGList.SetUnitText(4, L"0", FALSE);
    main->m_pShiftFileDlg->m_MJPGList.SetUnitText(12, g_showDir, FALSE);
    main->m_pShiftFileDlg->m_MJPGList.SetUnitText(22, g_showDir, FALSE);
    main->m_pShiftFileDlg->m_MJPGList.Invalidate();
    main->m_pShiftFileDlg->m_fileCount = nFileSelected;
    main->m_pShiftFileDlg->m_currentFile = 0;
    main->m_pShiftFileDlg->m_shiftProc->SetParam(0, 0, nFileSelected, 1);
}

LRESULT C3GHomeMovieDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) {
    // TODO: Add your specialized code here and/or call the base class  
    switch (message) {
    case WM_DELETESELITEM:
        DeleteSelectedItem();
        break;
    case WM_SHIFTSELITEM:
        m_MJPGList.SetUnitIsDownStatus(50, FALSE);
        m_isSelectAll = FALSE;
        SetPlayList(m_chDir);
        SetInitShow();
        AddFileMovie();
        ShowArrayInList(m_fileMovie);
        break;
    };
    return CDialog::WindowProc(message, wParam, lParam);
}

LRESULT C3GHomeMovieDlg::OnDeviceChange(WPARAM w, LPARAM l) {
    LRESULT result = 0;
    if (w == 0x8000) //insert
    {
        ::Sleep(500);
        if (m_MJPGList.GetUnitIsDisable(1)) {
            if (DetectDIR(_T("/usbdisk"))) {
                m_MJPGList.SetUnitIsDisable(1, FALSE);
                m_cardType1 = 1;
                m_MJPGList.SetUnitBitmap(1, "./ADV/MJPG/k5/common/影院/U盘_fore.BMP", "./ADV/MJPG/k5/common/影院/U盘_back.BMP", TRUE);
            } else if (DetectDIR(_T("/storagecard"))) {
                  m_MJPGList.SetUnitIsDisable(1, FALSE);
                  m_cardType1 = 2;
                  m_MJPGList.SetUnitBitmap(1, "./ADV/MJPG/k5/common/影院/SD卡_fore.BMP", "./ADV/MJPG/k5/common/影院/SD卡_back.BMP", TRUE);
            }
        } else if (m_MJPGList.GetUnitIsDisable(2)) {
            if (DetectDIR(_T("/usbdisk")) && (m_cardType1 != 1)) {
                m_MJPGList.SetUnitIsDisable(2, FALSE);
                m_cardType2 = 1;
                m_MJPGList.SetUnitBitmap(2, "./ADV/MJPG/k5/common/影院/U盘_fore.BMP", "./ADV/MJPG/k5/common/影院/U盘_back.BMP", TRUE);
            } else if (DetectDIR(_T("/storagecard")) && (m_cardType1 != 2)) {
                  m_MJPGList.SetUnitIsDisable(2, FALSE);
                  m_cardType2 = 2;
                  m_MJPGList.SetUnitBitmap(2, "./ADV/MJPG/k5/common/影院/SD卡_fore.BMP", "./ADV/MJPG/k5/common/影院/SD卡_back.BMP", TRUE);
            }
        }
    } else if (w == 0x8004) //remove
    {
          ::Sleep(500);
        if (DetectDIR(_T("/usbdisk"))) {
            m_cardType1 = 1;
            m_MJPGList.SetUnitBitmap(1, "./ADV/MJPG/k5/common/影院/U盘_fore.BMP", "./ADV/MJPG/k5/common/影院/U盘_back.BMP", TRUE);
        } else if (DetectDIR(_T("/storagecard"))) {
              m_cardType1 = 2;
              m_MJPGList.SetUnitBitmap(1, "./ADV/MJPG/k5/common/影院/SD卡_fore.BMP", "./ADV/MJPG/k5/common/影院/SD卡_back.BMP", TRUE);
        } else {
            m_MJPGList.SetUnitBitmap(1, L"", L"", TRUE);
            m_MJPGList.SetUnitIsDisable(1, TRUE);
            m_cardType1 = 0;
        }
          m_MJPGList.SetUnitBitmap(2, L"", L"", TRUE);
          m_MJPGList.SetUnitIsDisable(2, TRUE);
          m_cardType2 = 0;
          OnClearAll();
          m_uiType = 0;
          ShowItemsInList(m_uiType);
    }
    return result;
}

void C3GHomeMovieDlg::ShowWindow_(int nCmdShow) {
    ShowItemsInList(m_uiType);
    ShowWindow(nCmdShow);
}

void C3GHomeMovieDlg::OnExit_() {
    GetParent()->SendMessage(WM_CHANGEWINDOW, (WPARAM)this, (LPARAM)SW_HIDE);

    CMultimediaPhoneDlg* pMainDlg = ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd));
    pMainDlg->PopbackIcon();

    m_isSelectAll = FALSE;
    m_uiType = 0;
    m_MJPGList.SetUnitIsDownStatus(50, FALSE);
    for (size_t i = 100; i <= 500; i += 100) {
        m_MJPGList.SetUnitIsDownStatus(i, FALSE);
    }
    for (size_t i = 0; i < m_isFileSelect.size(); ++i) {
        m_isFileSelect[i] = 0;
    }
}

extern BOOL g_isAutoPlay;
extern size_t gPlayIndex;
void C3GHomeMovieDlg::OnPlayer(int index) {
    CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
    main->CancelBalckLightSaveTimer();

    main->m_pMainDlg->m_mainVideoDlg_->m_MovieList.clear();
    main->m_pMainDlg->m_mainVideoDlg_->m_ShowList.clear();

    int nFileSelected = 0;
    size_t loop = 0;
    if (m_fileDir.size() > 0) {
        loop = m_fileDir.size();
    }
    for (; loop < m_fileCount; loop++) {
        if (m_isFileSelect[loop] == 1) {
            char filename[128] = {
                0
            };
            size_t convertCount = 0;
            int n = wcstombs_s(&convertCount, filename, 128, (LPTSTR)(LPCTSTR)m_fileMovie[loop], 128);
            filename[n] = '\0';

            char file_dir[128] = {
                0
            };
            n = wcstombs_s(&convertCount, file_dir, 128, m_chDir, 128);
            file_dir[n] = '\0';

            char txt[128] = {
                0
            };
            sprintf_s(txt, 128, "%s%s", file_dir, filename);

            main->m_pMainDlg->m_mainVideoDlg_->m_MovieList.push_back(txt);
            main->m_pMainDlg->m_mainVideoDlg_->m_ShowList.push_back(m_fileMovie[loop]);
            nFileSelected++;
        }
    }
    main->m_pMainDlg->m_mainVideoDlg_->CalculatePage(nFileSelected);
    main->m_pMainDlg->m_mainVideoDlg_->ShowArrayInList(main->m_pMainDlg->m_mainVideoDlg_->m_ShowList);

    if (nFileSelected != 0) {
        CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
        main->playervideo_->SetImageList(main->m_pMainDlg->m_mainVideoDlg_->m_MovieList, index);
        m_MJPGList.Invalidate();
    }
    gPlayIndex = 1;
    g_isAutoPlay = FALSE;
}
