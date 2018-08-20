// WebDialog.cpp : implementation file
//

#include "stdafx.h"
#include "WebDialog.h"
#include "../MultimediaPhone.h"
#include "../MultimediaPhoneDlg.h"
#include "../Data/SkinStyle.h"
#include "../Data/LanguageResource.h"
//#include <sip.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace View {
    /////////////////////////////////////////////////////////////////////////////
    // CWebDialog dialog
#define WM_BROW_FORWARD	WM_USER+100
#define WM_BROW_BACK	WM_USER+101
#define WM_BROW_FRESH	WM_USER+102
#define WM_BROW_STOP	WM_USER+103
#define WM_BROW_URL		WM_USER+104

    TCHAR tUrl[256] = {0};
    COPYDATASTRUCT gdata;

    extern BOOL DetectDIR(TCHAR *sDir);
    CWebDialog::CWebDialog(CWnd* pParent /*=NULL*/)
        : CDialog(CWebDialog::IDD, pParent)
    {
        //{{AFX_DATA_INIT(CWebDialog)
        // NOTE: the ClassWizard will add member initialization here
        //}}AFX_DATA_INIT
        m_bPlaying = FALSE;
        m_hIEWnd = NULL;
    }

    void CWebDialog::DoDataExchange(CDataExchange* pDX)
    {
        CDialog::DoDataExchange(pDX);
        //{{AFX_DATA_MAP(CWebDialog)
        // NOTE: the ClassWizard will add DDX and DDV calls here
        DDX_Control(pDX, IDC_EXPLORER_BROWSER, browser_);
        //}}AFX_DATA_MAP
    }

    void CWebDialog::OnOk() {
        CString url;
        browser_.Navigate(url, 0, 0, 0, 0);
    }

    BEGIN_MESSAGE_MAP(CWebDialog, CDialog)
        //{{AFX_MSG_MAP(CWebDialog)
        //ON_BN_CLICKED(IDC_BUTTON_NOTE_CLOSE, OnButtonOk)
        //ON_BN_CLICKED(IDC_BUTTON_SOUND_CLOSE, OnButtonClose)
        //ON_MESSAGE(WM_COMMBOX_CLICKED, OnUrlSelect)
        //ON_NOTIFY(NM_CLICK, IDC_LIST_MP3LIST, OnClickPlayList)
        //ON_MESSAGE(WM_LISTCTRL_CLICK, OnListCltrlClick)
        ON_WM_TIMER()
        ON_MESSAGE(WM_CLICKMJPG_TOAPP, OnClickMJPG)
        ON_MESSAGE(WM_EDITENTER, OnEnter)
        ON_MESSAGE(WM_KILLWEBSHOW, OnKillWEBShow)
        ON_MESSAGE(WM_COPYDATA, OnSetLink)
        ON_MESSAGE(WM_COMMBOX_CLICKED, OnSelectURL)
        ON_MESSAGE(CBN_EDITCHANGE, OnEditURL)
        ON_MESSAGE(WM_USER+100, OnIEClick)
        ON_MESSAGE(WM_CHAR, OnInputURL)
        //}}AFX_MSG_MAP
    END_MESSAGE_MAP()

    LRESULT CWebDialog::OnIEClick(WPARAM w, LPARAM l)
    {
        LRESULT result = 0;
        CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
        main->ReStoreBackLight();
        return result;
    }

    LRESULT CWebDialog::OnInputURL(WPARAM w, LPARAM l)
    {
        LRESULT result = 0;
        CString s;
        m_cmbURL.GetWindowText(s);
        if (url_ != s)
        {
            SendtoWebMessage(WM_BROW_STOP, 0, 0);
            SetURL(s);
            m_MJPGList.Invalidate();
        }
        return result;
    }

    LRESULT CWebDialog::OnEditURL(WPARAM w, LPARAM l)
    {
        LRESULT result = 0;
        CString s;
        m_cmbURL.GetWindowText(s);
        if (url_ != s)
        {
            SendtoWebMessage(WM_BROW_STOP, 0, 0);
            SetURL(s);
            m_MJPGList.Invalidate();
        }
        return result;
    }

    LRESULT CWebDialog::OnSelectURL(WPARAM w, LPARAM l)
    {
        LRESULT result = 0;
        //if (w == IDC_COMBOBOX_SETTING_RINGTIMES)
        {
            CString s;
            int nIndex = m_cmbURL.GetCurSel();
            m_cmbURL.GetLBText(nIndex, s);
            if (url_ != s)
            {
                SendtoWebMessage(WM_BROW_STOP, 0, 0);
                SetURL(s);
                m_MJPGList.Invalidate();
            }
        }
        return result;
    }

    //void CWebDialog::OnEditchangeCombo1() 
    //{
    //    CString str;
    //    m_cmbURL.GetWindowText(str);
    //    int len = str.GetLength();
    //    if (str[len-1] < '0' || str[len-1] >'9')
    //    {
    //        str = str.Left(len-1);
    //        m_cmbURL.SetWindowText(str);
    //    }
    //}

    LRESULT CWebDialog::OnEnter(WPARAM w, LPARAM l)
    {
        LRESULT result = 0;
        CString s;
        m_edit.GetWindowText(s);
        SetURL(s);
        return result;
    }

    LRESULT CWebDialog::OnClickMJPG(WPARAM w, LPARAM l)
    {
        LRESULT result = 0;
        switch(w)
        {
        case 1:				//上一页
            browser_.GoBack();
            //SendtoWebMessage(WM_BROW_BACK, 0, 0);
            m_MJPGList.Invalidate();
            break;
        case 2:				//下一页
            //SendtoWebMessage(WM_BROW_FORWARD, 0, 0);
            m_MJPGList.Invalidate();
            browser_.GoForward();
            break;
        case 3:				//停止
            //SendtoWebMessage(WM_BROW_STOP, 0, 0);
            m_MJPGList.Invalidate();
            browser_.Stop();
            break;
        case 4:				//刷新
            //SendtoWebMessage(WM_BROW_FRESH, 0, 0);
            m_MJPGList.Invalidate();
            browser_.Refresh();
            break;
        case 5:				//主页
            SetURL(L"http://news.sina.com.cn/");
            m_MJPGList.Invalidate();
            break;
        case 6:				//查找
            break;
        case 7:				//关闭
            KillTimer(1);
            ExitWeb();
            ShowWindow(SW_HIDE);
            ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pMainDlg->SetStatusAll(FALSE);
            break;
        case 8:				//确定
            break;
        case 9:
            {
                CString s;
                m_edit.GetWindowText(s);
                AddHistory(s);
            }
            break;
        case 10:
            break;
        default:
            break;
        }
        return result;
    }

    void CWebDialog::AddHistory(CString s)
    {
        if (s != "")
        {
            for (int j = 0; j < 10; ++j)
            {
                if (strlen(m_sUrlList[j]) > 0)
                {
                    if (memcmp(m_sUrlList[j], Util::StringOp::FromCString(s).c_str(), strlen(m_sUrlList[j])) == 0)
                    {
                        return;
                    }
                }
            }

            int i = m_nUrlCount - 1;
            for (; i >= 0; --i)
            {
                if (i < 9)
                    strcpy(m_sUrlList[i+1], m_sUrlList[i]);
            }
            strcpy(m_sUrlList[0], Util::StringOp::FromCString(s).c_str());
        }
        else
            return;
        std::string webFilename = Util::StringOp::FromCString(::webPath);
        webFilename += "/weblist.txt";
        FILE *file = fopen(webFilename.c_str(), "w+b");
        if (file)
        {
            for (int i = 0; i < 10; ++i)
            {
                if (strlen(m_sUrlList[i]) > 0)
                {
                    fwrite(m_sUrlList[i], sizeof(char), strlen(m_sUrlList[i]), file);
                    fwrite("\r\n", sizeof(char), 2, file);
                }
            }
            fclose(file);
        }
        SetHistoryList();
    }

    void CWebDialog::SetHistoryList()
    {
        m_nUrlCount = 0;
        int nCount = 0;
        m_cmbURL.ResetContent();
        std::string webFilename = Util::StringOp::FromCString(::webPath);
        webFilename += "/weblist.txt";
        FILE *file = fopen(webFilename.c_str(), "r+b");
        if (file)
        {
            while (!feof(file))
            {
                char txt[128] = {0};
                fgets(txt, 127, file);
                CString s = Util::StringOp::ToCString(txt);
                s.TrimRight(L" ");
                s.TrimLeft(L" ");
                s.TrimRight(L"\n");
                s.TrimRight(L"\r");
                s.TrimRight(L" ");
                s.TrimLeft(L" ");
                if (s != "")
                {
                    if (nCount < 10)
                    {
                        strcpy(m_sUrlList[nCount], Util::StringOp::FromCString(s).c_str());
                        m_cmbURL.AddString(s);
                        ++nCount;
                    }
                }
            }
            fclose(file);
            m_nUrlCount = nCount;
            for (int i = nCount; i < 10; ++i)
            {
                memset(m_sUrlList[i], 0, 128);
            }
        }
        else
        {
            m_cmbURL.AddString(L"www.sina.com.cn");
            m_cmbURL.AddString(L"www.sohu.com");
            m_cmbURL.AddString(L"www.baidu.com");
            m_cmbURL.AddString(L"www.126.com");
            m_cmbURL.AddString(L"www.caijing.com.cn");
            m_cmbURL.AddString(L"www.gutx.com");
            m_cmbURL.AddString(L"http://sports.china.com");
            m_cmbURL.AddString(L"http://sports.people.com.cn");
        }
    }

    BOOL CWebDialog::OnInitDialog()
    {
        CDialog::OnInitDialog();

        m_MJPGList.Create(L"", WS_VISIBLE|WS_CHILD, CRect(0*X_XISHU, 0*Y_XISHU, 800*X_XISHU, 420*Y_XISHU), this);
        m_MJPGList.SetCurrentLinkFile(_T(".\\adv\\mjpg\\k1\\中文\\web.xml"));
        m_MJPGList.SetMJPGRect(CRect(0*X_XISHU, 0*Y_XISHU, 800*X_XISHU, 420*Y_XISHU));

        m_cmbURL.Create(WS_CHILD|WS_VISIBLE, CRect((360-12)*X_XISHU, 1*Y_XISHU, 750*X_XISHU, 400*Y_XISHU), &m_MJPGList, IDC_COMBOBOX_SETTING_RINGTIMES);
        memset(m_sUrlList, 0, 10*128);
        SetHistoryList();

        m_IERect = CRect(13*X_XISHU, 55*Y_XISHU, 786*X_XISHU, 474*Y_XISHU);

        return TRUE;  // return TRUE unless you set the focus to a control
        // EXCEPTION: OCX Property Pages should return FALSE
    }

    void IEThreadPrc(void)
    {
    }

    void CWebDialog::SetURL(CString url)
    {
        if (url == "")
            url = _T("http://www.hxb.com.cn/");
        browser_.Navigate(url, 0, 0, 0, 0);
    }

    LRESULT CWebDialog::OnSetLink(WPARAM w, LPARAM l)
    {
        LRESULT result = 0;
        COPYDATASTRUCT *ldata;

        ldata = (COPYDATASTRUCT*)l;
        if (ldata->lpData)
            m_edit.SetWindowText((TCHAR *)ldata->lpData);
        return result;
    }

    void CWebDialog::OnTimer(UINT nIDEvent)
    {
        if (IsWindowVisible() && m_bPlaying)
        {
            if (1 == nIDEvent)
            {
            }
            else if (2 == nIDEvent)
            {
                KillTimer(2);
                SendtoWebMessage(WM_COMMAND, 1001, 0);
            }
            else if (3 == nIDEvent && IsWindowVisible())
            {
                if (m_hIEWnd)
                {
                    //::SetWindowPos(m_hIEWnd, HWND_TOPMOST, m_IERect.left, m_IERect.top, m_IERect.Width(), m_IERect.Height(), 0);
                }
            }
        }
        CDialog::OnTimer(nIDEvent);
    }

    LRESULT CWebDialog::OnKillWEBShow(WPARAM w, LPARAM l)
    {
        LRESULT result = 0;
        if (IsWindowVisible())
        {
            if (m_hIEWnd)
            {
                if (w == 1)
                    KillTimer(3);
                else
                {
                    if (((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pNetStatusDlg->IsWindowVisible() || ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_AlarmShowDlg->IsWindowVisible() || ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pTelephoneDlg->IsWindowVisible())
                        return result;
                    SetTimer(3, 30, NULL);
                }
            }
        }
        return result;
    }

    void CWebDialog::ShowWindow_(BOOL bShow)
    {
        if (bShow)
        {
            SetURL(_T(""));
            ShowWindow(SW_SHOW);
            if (m_hIEWnd)
            {
                ::ShowWindow(m_hIEWnd, SW_SHOW);
                ::SetForegroundWindow(m_hIEWnd);  
                SetTimer(3, 30, NULL);
            }
        }
        else
        {
            //SipShowIM(SIPF_OFF);
            KillTimer(3);
            ShowWindow(SW_HIDE);
            if (m_hIEWnd)
            {
                SendtoWebMessage(WM_BROW_STOP, 0, 0);
                ::ShowWindow(m_hIEWnd, SW_HIDE);
            }
        }
    }

    void CWebDialog::SendtoWebMessage(UINT message, WPARAM w, LPARAM l)
    {
        if (m_hIEWnd)
        {
            ::SendMessage(m_hIEWnd, message, w, l);
        }
    }

    void CWebDialog::ExitWeb()
    {
        if (m_bPlaying)
        {
            KillTimer(3);
            m_bPlaying = FALSE;
            SendtoWebMessage(WM_CLOSE, 0, 0);
            m_hIEWnd = NULL;
        }
    }
}
