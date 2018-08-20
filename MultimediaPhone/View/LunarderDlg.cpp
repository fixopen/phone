// LunarderDlg.cpp : implementation file
//

#include "stdafx.h"
#include "LunarderDlg.h"
#include "../MultimediaPhoneDlg.h"
#include "../MultimediaPhone.h"
#include "../Logical/LunarderDate.h"
#include "../Data/LanguageResource.h"
#include "../Data/SkinStyle.h"
#include "../Util/ScanDir.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace Control {
    extern std::vector<std::string> month();
    extern std::vector<std::string> day();
    extern std::vector<std::string> day1();
}

namespace View {
    /////////////////////////////////////////////////////////////////////////////
    // CLunarderDlg dialog
#define IDC_BTN_YL			0xFF00
#define IDC_BTN_YR			0xFF01
#define IDC_BTN_ML			0xFF02
#define IDC_BTN_MR			0xFF03
#define IDC_BTN_UP			0xFF04
#define IDC_BTN_DOWN		0xFF05

    CLunarderDlg::CLunarderDlg(CWnd* pParent /*=NULL*/)
        : CDialog(CLunarderDlg::IDD, pParent)
    {
        //{{AFX_DATA_INIT(CLunarderDlg)
        // NOTE: the ClassWizard will add member initialization here
        //}}AFX_DATA_INIT

        m_type = 0;  //缺省记事
        m_maindeskIn = FALSE;
        m_curIndex = -1;

        VERIFY(m_font.CreateFont(
            24,                        // nHeight
            0,                         // nWidth
            0,                         // nEscapement
            0,                         // nOrientation
            FW_NORMAL,                 // nWeight
            FALSE,                     // bItalic
            FALSE,                     // bUnderline
            0,                         // cStrikeOut
            ANSI_CHARSET,              // nCharSet
            OUT_DEFAULT_PRECIS,        // nOutPrecision
            CLIP_DEFAULT_PRECIS,       // nClipPrecision
            DEFAULT_QUALITY,           // nQuality
            DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
            _T("宋体")));                 // lpszFacename
    }

    void CLunarderDlg::DoDataExchange(CDataExchange* pDX)
    {
        CDialog::DoDataExchange(pDX);
        //{{AFX_DATA_MAP(CLunarderDlg)
        // NOTE: the ClassWizard will add DDX and DDV calls here
        //}}AFX_DATA_MAP
    }


    BEGIN_MESSAGE_MAP(CLunarderDlg, CDialog)
        //{{AFX_MSG_MAP(CLunarderDlg)
        ON_BN_CLICKED(IDC_BUTTON_LUNARDEREXIT, OnExit)
        ON_BN_CLICKED(IDC_BTN_YL, OnYleft)
        ON_BN_CLICKED(IDC_BTN_YR, OnYright)
        ON_BN_CLICKED(IDC_BTN_ML, OnMleft)
        ON_BN_CLICKED(IDC_BTN_MR, OnMright)
        ON_BN_CLICKED(IDC_BTN_UP, OnUp)
        ON_BN_CLICKED(IDC_BTN_DOWN, OnDown)
        ON_BN_CLICKED(IDC_LUNARDER_OK, OnOk)
        ON_BN_CLICKED(IDC_LUNARDER_CANCEL, OnCancel)
        ON_BN_CLICKED(IDC_LUNARDER_DELETE, OnDelete)
        ON_MESSAGE(WM_LUNAR_CLICKED, OnLunarClick)
        ON_MESSAGE(WM_CLICKMJPG_TOAPP, OnClickMJPG)
        ON_MESSAGE(WM_DELETESELITEM, OnDeleteItem)
        ON_MESSAGE(WM_STOPTRYRING, OnStopTryRing)
        //}}AFX_MSG_MAP
    END_MESSAGE_MAP()

    /////////////////////////////////////////////////////////////////////////////
    // CLunarderDlg message handlers

    BOOL CLunarderDlg::OnInitDialog() 
    {
        CDialog::OnInitDialog();

		m_MJPGList.Create(L"", WS_VISIBLE|WS_CHILD, CRect(0*X_XISHU, 0*Y_XISHU, 535*X_XISHU, 420*Y_XISHU), this);
        m_MJPGList.SetCurrentLinkFile(_T(".\\adv\\mjpg\\k1\\中文\\日历.xml"));
        m_MJPGList.SetMJPGRect(CRect(0*X_XISHU, 0*Y_XISHU, 535*X_XISHU, 420*Y_XISHU));

        m_dtDate.Create(WS_CHILD|DTS_SHORTDATEFORMAT, CRect(125*X_XISHU, 67*Y_XISHU, (125+184)*X_XISHU, (67+32)*Y_XISHU), &m_MJPGList, IDC_SETTING_DATE, 28, CDateCtrl::modeDate);
        m_dtTime.Create(WS_CHILD/*|DTS_TIMEFORMAT*/, CRect(346*X_XISHU, 67*Y_XISHU, (346+154)*X_XISHU, (67+32)*Y_XISHU), &m_MJPGList, IDC_SETTING_TIME, 28, CDateCtrl::modeTime);


        m_rdoIsAlarm.Create(L"", WS_CHILD, CRect(128*X_XISHU, 124*Y_XISHU, (128+36)*X_XISHU, (124+36)*Y_XISHU), &m_MJPGList, IDC_RADIO_SETTING_STATIC);
        m_rdoIsAlarm.SetColor(RGB(0, 0, 0), RGB(237, 237, 237));
        m_rdoIsNoAlarm.Create(L"", WS_CHILD, CRect(215*X_XISHU, 124*Y_XISHU, (215+36)*X_XISHU, (124+36)*Y_XISHU), &m_MJPGList, IDC_RADIO_SETTING_STATIC);
        m_rdoIsNoAlarm.SetColor(RGB(0, 0, 0), RGB(237, 237, 237));

        CButton *pButton[2];
        pButton[0] = &m_rdoIsAlarm;
        pButton[1] = &m_rdoIsNoAlarm;

        m_rdoIsAlarm.SetGroupButton(pButton, 2);
        m_rdoIsNoAlarm.SetGroupButton(pButton, 2);

        m_cmbRing.Create(WS_CHILD, CRect(131*X_XISHU, 182*Y_XISHU, 457*X_XISHU, 410*Y_XISHU), &m_MJPGList, IDC_COMBOBOX_CONTACTGROUP_SOUNDTIP);

        SetRingLst();

        m_alarmEdit.Create(WS_CHILD|ES_MULTILINE | ES_AUTOVSCROLL | ES_WANTRETURN , CRect(131*X_XISHU, 253*Y_XISHU, 450*X_XISHU, 395*Y_XISHU), &m_MJPGList, 0xFFFF);
        m_alarmEdit.SetLimitText(768);
        //	m_scollbar.Create(L"", WS_CHILD|WS_VISIBLE|WS_GROUP, CRect(451, 253, 451+SCROLL_ARROW_WIDTH+1, 395), this, 0);
        //	m_scollbar.SetParam(0, 0, 0, 5, this);

        m_bIsMainLunder = TRUE;
        SYSTEMTIME curtime;
        GetLocalTime(&curtime);
        SetData(curtime.wYear, curtime.wMonth, curtime.wDay, TRUE);

        return TRUE;  // return TRUE unless you set the focus to a control
        // EXCEPTION: OCX Property Pages should return FALSE
    }

    class FindMp3AndWav : public Util::ProcessFileProcedure {
    public:
        FindMp3AndWav(CString dir, CCEComboBox_& ringList)
            : ProcessFileProcedure(dir)
            , ringList_(ringList) {
        }
        virtual void operator()(WIN32_FIND_DATA const& FindFileData) {
            ringList_.AddString(FindFileData.cFileName);
        }
    private:
        CCEComboBox_& ringList_;
    };

    void CLunarderDlg::SetRingLst()
    {
        CString dir = ::ringPath;
        ProcessFiles(_T("*.mp3"), FindMp3AndWav(dir, m_cmbRing));
        ProcessFiles(_T("*.wav"), FindMp3AndWav(dir, m_cmbRing));
    }

    void CLunarderDlg::OnExit()
    {
        if(m_maindeskIn)
            ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->SwitchPanel_(IDC_BUTTON_MAIN);
        else
            GetParent()->SendMessage(WM_CHANGEWINDOW, (WPARAM)this, (LPARAM)SW_HIDE);
    }

    void CLunarderDlg::OnYleft()
    {
        if(m_Month == 1)
        {
            m_Month = 12;
            m_Year--;
        }
        else
            m_Month--;
        SetData(m_Year, m_Month, m_Day);

        //m_lunarder.OnYleft();
    }
    void CLunarderDlg::OnYright()
    {
        //m_lunarder.OnYright();
        if(m_Month == 12)
        {
            m_Month = 1;
            m_Year++;
        }
        else
            m_Month++;
        SetData(m_Year, m_Month, m_Day);
    }

    void CLunarderDlg::OnMleft()
    {
        //	m_lunarder.OnMleft();
    }

    void CLunarderDlg::OnMright()
    {
        //	m_lunarder.OnMright();
    }

    void CLunarderDlg::OnUp()
    {
        //	m_Edit.SendMessage(WM_VSCROLL, MAKELONG(SB_PAGEUP,0),NULL);
        //	m_Edit.Invalidate();
    }

    void CLunarderDlg::OnDown()
    {
        //	m_Edit.SendMessage(WM_VSCROLL, MAKELONG(SB_PAGEDOWN,0),NULL);  
        //	m_Edit.Invalidate();
    }

    LRESULT CLunarderDlg::OnLunarClick(WPARAM w, LPARAM l)
    {
        LRESULT result = S_OK;
        return result;
    }

    //保存
    void CLunarderDlg::OnOk()
    {
    }

    void CLunarderDlg::OnCancel()
    {
    }

    //删除
    void CLunarderDlg::OnDelete()
    {
    }


    void CLunarderDlg::SetData(int y, int m, int d, BOOL flag)
    {
        if (!m_bIsMainLunder)
        {
            return;
        }

        BOOL isDraw = flag;

        if(y < 1901)
            y = 2008;
        m_Year = y;
        m_Month = m;
        m_Day = d;

        char txt[64];
        //sprintf(txt, "%04d年%02d月", m_Year, m_Month);
        //CString sT = txt;
        CString sT;
        sT.Format(_T("%04d年%02d月"), m_Year, m_Month);
        m_MJPGList.SetUnitText(4, sT, FALSE);

        int nDay = Logical::LunarderDate::MonthDays(m_Year, m_Month);
        int nWeek = Logical::LunarderDate::WeekDay(m_Year, m_Month, 1);

        if(m_Day > nDay)
            m_Day = nDay;

        m_MJPGList.SetUnitIsDownStatus(m_curIndex, FALSE);

        for(int i = 0; i < 42; i++)
        {
            CString s1;
            int nIndex = i + 5;
            m_MJPGList.SetUnitColor(nIndex, font_black, isDraw, TRUE);
            if(i < nWeek)
            {
                m_MJPGList.SetUnitText(nIndex, s1, isDraw, TRUE);
                m_MJPGList.SetUnitIsShow(nIndex, isDraw, TRUE);
            }
            else if(i >= (nWeek+nDay))
            {
                m_MJPGList.SetUnitText(nIndex, s1, isDraw, TRUE);
                m_MJPGList.SetUnitIsShow(nIndex, isDraw,TRUE);
            }
            else
            {
                s1.Format(_T("%d \r\n"), (i + 1 - nWeek));

                ////////////////start/////////////
                ::UINT16 y, m, d, ret;
                ret = Logical::LunarderDate::GetLunarDate(m_Year, m_Month, i + 1 - nWeek, y, m, d);
                if (ret)
                {
                    m_MJPGList.SetUnitColor(nIndex, font_green, isDraw, TRUE);
                    char txt[6];
                    memset(txt, 0, 6);
                    Logical::LunarderDate::FormatHolDay(ret, txt);
                    s1 += Util::StringOp::ToCString(txt);
                }
                else
                {
                    if (d == 1)
                    {
                        ::UINT16 iYear, iMonth, iDay;
                        int n = Logical::LunarderDate::CalcLeapLunarYear(y, iYear, iMonth, iDay);
                        if (n > 0)
                        {
                            if(iYear == m_Year && iMonth == m_Month && iDay == (i + 1 - nWeek))
                            {
                                s1 += Util::StringOp::ToCString(Data::LanguageResource::Get(Data::RI_LANUDER_LOOP));
                                s1 += Util::StringOp::ToCString(Control::month()[m - 1]);
                            }
                            else
                            {
                                s1 += Util::StringOp::ToCString(Control::month()[m - 1]);
                            }
                        }
                        else
                            s1 += Util::StringOp::ToCString(Control::month()[m-1]);
                    }
                    else
                    {
                        if (d <= 10)
                        {
                            s1 += Util::StringOp::ToCString(Control::day()[0]);
                        }
                        else if (d < 20)
                        {
                            s1 += Util::StringOp::ToCString(Control::day()[1]);
                        }
                        else if (d < 30)
                        {
                            s1 += Util::StringOp::ToCString(Control::day()[2]);
                        }
                        else
                        {
                            s1 += Util::StringOp::ToCString(Control::day()[3]);
                        }
                        s1 += Util::StringOp::ToCString(Control::day1()[d % 10]);
                    }
                }
                ////////////////end///////////////
                if (m_Day == (i + 1 - nWeek))
                {
                    m_MJPGList.SetUnitIsDownStatus(nIndex, TRUE);
                    m_curIndex = nIndex;
                }

                m_MJPGList.SetUnitText(nIndex, s1, isDraw, TRUE);
                m_MJPGList.SetUnitIsShow(nIndex, isDraw, TRUE);

                //如果有提醒，将显示的文字改为红色
                if (IsHaveAlarm(m_Year, m_Month, i + 1 - nWeek))
                    m_MJPGList.SetUnitColor(nIndex, font_red, isDraw, TRUE);
            }

            //判断天，toggle
        }

    }

    LRESULT CLunarderDlg::OnClickMJPG(WPARAM w, LPARAM l)
    {
        LRESULT result = 0;
        if(w != 101)
            OnStopTryRing(0, 0);

        switch(w)
        {
        case 1:				//查看
            if(IsHaveAlarm(m_Year, m_Month, m_Day))
            {
                ShowAlarmDlg(FALSE);
            }

            break;
        case 2:				//上翻日期	
            OnYleft();
            break;
        case 3:				//下翻日期 
            OnYright();
            break;
        case 4:				//记事
            IsHaveAlarm(m_Year, m_Month, m_Day);
            ShowAlarmDlg(TRUE);
            break;
        case 100:			//退出日历		
            OnExit();
            break;
        case 200:			//退出日历记事
            ShowLunarderDlg();
            break;
        case 201:			//清除该记事
            DeleteCurrentAlarm();
            break;
        case 204:			//该天上一条记事
            SetPreAlarm();
            break;
        case 207:			//该天下一条记事
            SetNextAlarm();
            break;
        case 202:			//确定
            {
                CString s;
                m_alarmEdit.GetWindowText(s);
                if(s != "")
                {
                    SetAlarmDateBase();
                    {
                        ShowLunarderDlg();
                    }
                }
                else
                {
                    ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pWarningNoFlashDlg->SetType(0);
                    ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pWarningNoFlashDlg->ShowWindow_(SW_SHOW);
                }
            }
            break;
        case 205:			//记事上翻页
            m_alarmEdit.SendMessage(WM_VSCROLL, MAKELONG(SB_PAGEUP,0),NULL);
            m_alarmEdit.Invalidate();
            break;
        case 206:			//记事下翻页
            m_alarmEdit.SendMessage(WM_VSCROLL, MAKELONG(SB_PAGEDOWN,0),NULL);
            m_alarmEdit.Invalidate();
            break;
        case 203:			//新建
            {
                ShowAlarmDlg(TRUE);
            }
            break;
        case 101:   //试听音乐
            {
                {
                    if(!m_MJPGList.GetUnitIsDownStatus(101))
                    {
                        CString str;
                        m_cmbRing.GetWindowText(str);
                        if(str == "")
                            break;
                        CString s = ::ringPath; 
                        s += _T("/") + str;

                        ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pMainDlg->m_mainMp3Dlg_->OnTimer(1002); //SendMessage(WM_OUTEVENT, 0, 0);

                        ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->SetMsgWnd(this);
                        ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->StartRing((LPTSTR)(LPCTSTR)s, 1);
                        m_MJPGList.SetUnitIsDownStatus(101, TRUE);
                        m_MJPGList.SetUnitIsShow(101, TRUE);
                    }
                    else
                    {
                        OnStopTryRing(0, 0);
                    }
                    break;
                }
            }
        }
        if( 6 <= w && w <= 47) 
        {
            int nDay = Logical::LunarderDate::MonthDays(m_Year, m_Month);
            int nWeek = Logical::LunarderDate::WeekDay(m_Year, m_Month, 1);
            int i = w - 6;
            if (i >= nWeek && i < (nWeek + nDay))
            {
                int nIndex = i + 5;
                m_MJPGList.SetUnitIsDownStatus(m_curIndex, FALSE);
                m_MJPGList.SetUnitIsDownStatus(nIndex, TRUE);
                m_MJPGList.Invalidate();
                m_curIndex = nIndex;
                m_Day = i - nWeek + 1;

                //显示提醒
                if (IsHaveAlarm(m_Year, m_Month, m_Day))
                {
                    ShowAlarmDlg(FALSE);
                }
            }
        }
        return result;
    }

    //显示日历
    void CLunarderDlg::ShowLunarderDlg()
    {
        m_bIsMainLunder = TRUE;
        SetCtrlIsShow(FALSE);
        m_MJPGList.SetCurrentLinkFile(_T(".\\adv\\mjpg\\k1\\中文\\日历.xml"));
        SetData(m_Year, m_Month, m_Day, TRUE);
        m_MJPGList.Invalidate();
    }
    //显示alarm
    void CLunarderDlg::ShowAlarmDlg(BOOL isNew)
    {
        m_bIsMainLunder = FALSE;
        SetCtrlIsShow(TRUE);
        m_MJPGList.SetCurrentLinkFile(_T(".\\adv\\mjpg\\k1\\中文\\日历记事.xml"));
        m_MJPGList.Invalidate();
        SetAlarmCtrl(isNew);
    }

    void CLunarderDlg::SetCtrlIsShow(BOOL isShow)
    {
        m_rdoIsAlarm.ShowWindow(isShow);
        m_rdoIsNoAlarm.ShowWindow(isShow);
        m_dtTime.ShowWindow(isShow);
        m_dtDate.ShowWindow(isShow);
        m_alarmEdit.ShowWindow(isShow);
        m_cmbRing.ShowWindow(isShow);
        //	m_scollbar.ShowWindow(isShow);
    }

    LRESULT CLunarderDlg::OnDeleteItem(WPARAM w, LPARAM l)
    {
        LRESULT result = 0;
        m_pScheduler->Remove();
        if(IsHaveAlarm(m_Year, m_Month, m_Day))
        {
            SetAlarmCtrl(FALSE);
        }
        else
        {
            ShowLunarderDlg();
        }
        ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->SetAlarmList();
        return result;
    }

    void CLunarderDlg::DeleteCurrentAlarm()
    {
        if(!m_bIsNewAlarm && m_pScheduler)
        {
            std::string strTemp = "确定删除该记事吗?";
            ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetDelTip(Util::StringOp::ToCString(strTemp.c_str()));
            ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetHWnd(m_hWnd);
            ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->ShowWindow(TRUE);
        }

    }

    void CLunarderDlg::SetPreAlarm()
    {
        if (m_nAlarmCurrent > 0)
        {
            m_nAlarmCurrent--;
            m_pScheduler = m_result[m_nAlarmCurrent];
            SetAlarmCtrl(FALSE);
        }

    }

    void CLunarderDlg::SetNextAlarm()
    {
        if(m_nAlarmCurrent < (m_nAlarmCount -1))
        {
            m_nAlarmCurrent++;
            m_pScheduler = m_result[m_nAlarmCurrent];
            SetAlarmCtrl(FALSE);
        }
    }

    void CLunarderDlg::SetCurrentAlarmBase()
    {
        if (m_pScheduler)
        {
            BOOL isAlarm = m_rdoIsAlarm.GetCheck_();
            CTime tm1, tm2;		
            m_dtTime.GetTime(tm1);
            m_dtDate.GetTime(tm2);
            CTime time = CTime(tm2.GetYear(), tm2.GetMonth(), tm2.GetDay(), tm1.GetHour(), tm1.GetMinute(), tm1.GetSecond());
            m_pScheduler->startTime(time);

            CString s;
            m_pScheduler->tipsType((Data::TipsType)isAlarm);
            m_cmbRing.GetWindowText(s);
            m_pScheduler->ring(Util::StringOp::FromCString(s));
            m_alarmEdit.GetWindowText(s);
            m_pScheduler->what(Util::StringOp::FromCString(s));
            m_pScheduler->Update();
        }
    }

    void CLunarderDlg::SetAlarmDateBase()
    {
        if (m_bIsNewAlarm)
        {
            m_pScheduler = Util::shared_ptr<Data::Scheduler>(new Data::Scheduler);		
            BOOL isAlarm = m_rdoIsAlarm.GetCheck_();
            CTime tm1, tm2;		
            m_dtTime.GetTime(tm1);
            m_dtDate.GetTime(tm2);
            int y, m, d, h, mi, s1;
            y = tm2.GetYear();
            m = tm2.GetMonth();
            d = tm2.GetDay();
            h = tm1.GetHour();
            mi = tm1.GetMinute();
            s1 = tm1.GetSecond();

            CTime time = CTime(y, m, d, h, mi, s1);
            m_pScheduler->startTime(time);

            CString s;
            m_pScheduler->tipsType((Data::TipsType)isAlarm);
            m_cmbRing.GetWindowText(s);
            m_pScheduler->ring(Util::StringOp::FromCString(s));
            m_alarmEdit.GetWindowText(s);
            m_pScheduler->what(Util::StringOp::FromCString(s));
            m_pScheduler->Insert();
        }
        else
        {
            SetCurrentAlarmBase();
        }
        ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->SetAlarmList();
    }

    void CLunarderDlg::SetAlarmCtrl(BOOL isNew)
    {
        m_bIsNewAlarm = isNew;
        if(isNew)
        {
            m_nAlarmCurrent = -1;
            m_rdoIsAlarm.SetCheck_(0);
            m_rdoIsNoAlarm.SetCheck_(1);

            CTime tm1 = CTime(m_Year, m_Month, m_Day, 0, 0, 0);
            CTime tm2 = CTime(m_Year, m_Month, m_Day, 8, 0, 0);
            m_dtTime.SetTime(&tm2);
            m_dtDate.SetTime(&tm1);
            m_cmbRing.SetCurSel(0);
            m_alarmEdit.SetWindowText(L"");
        }
        else
        {
            CTime tm = m_pScheduler->startTime();

            Data::TipsType type = m_pScheduler->tipsType();
            m_rdoIsAlarm.SetCheck_(type);
            m_rdoIsNoAlarm.SetCheck_(!type);

            int y, m, d, h, mi, s1;
            y = tm.GetYear();
            m = tm.GetMonth();
            d = tm.GetDay();
            h = tm.GetHour();
            mi = tm.GetMinute();
            s1 = tm.GetSecond();

            m_dtTime.SetTime(&tm);
            m_dtDate.SetTime(&tm);

            std::string s = m_pScheduler->ring();
            //@@m_cmbRing.m_edit.SetWindowText(Util::StringOp::ToCString(s));
            m_cmbRing.SetWindowText(Util::StringOp::ToCString(s));
            m_cmbRing.Invalidate();
            s = m_pScheduler->what();
            m_alarmEdit.SetWindowText(Util::StringOp::ToCString(s));
        }

        char txt[24];
        //sprintf(txt, "%d/%d", m_nAlarmCurrent+1, m_nAlarmCount);
        //CString sTitle = txt;
        CString sTitle;
        sTitle.Format(_T("%d/%d"), m_nAlarmCurrent+1, m_nAlarmCount);
        m_MJPGList.SetUnitText(100, sTitle, TRUE);
        m_rdoIsAlarm.Invalidate();
        m_rdoIsNoAlarm.Invalidate();
    }

    BOOL CLunarderDlg::IsHaveAlarm(int y, int m ,int d)
    {
        m_nAlarmCount = 0;
        m_nAlarmCurrent = 0;

        CTime time = CTime(y, m, d, 0, 0, 0);

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
        m_result = Data::Scheduler::GetFromDatabase(filter, "startTime"); 
        if (!m_result.empty())
        {
            m_nAlarmCount = m_result.size();
            m_nAlarmCurrent = 0;
            m_pScheduler = m_result[0];
            return TRUE;
        }
        return FALSE;
    }

    LRESULT CLunarderDlg::OnStopTryRing(WPARAM w, LPARAM l)
    {
        LRESULT result = 0;
        if(m_MJPGList.GetUnitIsDownStatus(101))
        {
            ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->StartRing(L"");
            m_MJPGList.SetUnitIsDownStatus(101, FALSE);
            m_MJPGList.SetUnitIsShow(101, TRUE);

            ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pMainDlg->m_mainMp3Dlg_->SendMessage(WM_OUTEVENT, 0, 1);
        }
        return result;
    }
}
