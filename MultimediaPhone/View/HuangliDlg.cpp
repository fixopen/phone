// HuangliDlg.cpp : implementation file
//

#include "stdafx.h"
#include "HuangliDlg.h"
#include "../MultimediaPhone.h"
#include "../MultimediaPhoneDlg.h"
#include "../Logical/LunarderDate.h"
#include "../Data/LanguageResource.h"

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
    // CHuangliDlg dialog

    CHuangliDlg::CHuangliDlg(CWnd* pParent /*=NULL*/)
        : CDialog(CHuangliDlg::IDD, pParent)
    {
        //{{AFX_DATA_INIT(CHuangliDlg)
        // NOTE: the ClassWizard will add member initialization here
        //}}AFX_DATA_INIT
        m_curIndex = -1;
    }


    void CHuangliDlg::DoDataExchange(CDataExchange* pDX)
    {
        CDialog::DoDataExchange(pDX);
        //{{AFX_DATA_MAP(CHuangliDlg)
        // NOTE: the ClassWizard will add DDX and DDV calls here
        //}}AFX_DATA_MAP
    }


    BEGIN_MESSAGE_MAP(CHuangliDlg, CDialog)
        //{{AFX_MSG_MAP(CHuangliDlg)
        ON_MESSAGE(WM_CLICKMJPG_TOAPP, OnClickMJPG)
        //}}AFX_MSG_MAP
    END_MESSAGE_MAP()

    /////////////////////////////////////////////////////////////////////////////
    // CHuangliDlg message handlers

    BOOL CHuangliDlg::OnInitDialog() 
    {
        CDialog::OnInitDialog();

        // TODO: Add extra initialization here
        m_MJPGList.Create(L"", WS_VISIBLE|WS_CHILD, CRect(0, 0, 800 * 125 / 100, 420 * 125 / 100), this);
        m_MJPGList.SetCurrentLinkFile(_T(".\\adv\\mjpg\\k1\\中文\\老黄历.xml"));
        m_MJPGList.SetMJPGRect(CRect(0, 0, 800 * 125 / 100, 420 * 125 / 100));

        SYSTEMTIME curtime;
        GetLocalTime(&curtime);
        SetData(curtime.wYear, curtime.wMonth, curtime.wDay, TRUE);

        return TRUE;  // return TRUE unless you set the focus to a control
        // EXCEPTION: OCX Property Pages should return FALSE
    }

    void CHuangliDlg::OnYleft()
    {
        --m_Year;
        if (m_Year < 1901)
        {
            m_Year = 2050;
        }
        SetData(m_Year, m_Month, m_Day);
    }

    void CHuangliDlg::OnYright()
    {
        ++m_Year;
        if (m_Year > 2051)
        {
            m_Year = 1901;
        }
        SetData(m_Year, m_Month, m_Day);
    }

    void CHuangliDlg::OnMleft()
    {
        --m_Month;
        if(m_Month <= 0)
        {
            m_Month = 12;
        }
        SetData(m_Year, m_Month, m_Day);
    }

    void CHuangliDlg::OnMright()
    {
        m_Month++;
        if(m_Month > 12)
        {
            m_Month = 1;
        }
        SetData(m_Year, m_Month, m_Day);
    }

    void CHuangliDlg::OnExit()
    {
        ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->SwitchPanel_(IDC_BUTTON_MAIN);
    }

    LRESULT CHuangliDlg ::OnClickMJPG(WPARAM w, LPARAM l)
    {
        LRESULT result = 0;
        switch(w)
        {
        case 1:				//上一月
            OnMleft();
            break;
        case 2:				//下一月	
            OnMright();
            break;
        case 3:				//上一年 
            OnYleft();
            break;
        case 4:				//下一年
            OnYright();
            break;
        case 100:			//退出日历		
            OnExit();
            break;
        default:
            if( 6 <= w && w <= 47) 
            {
                int nDay = Logical::LunarderDate::MonthDays(m_Year, m_Month);
                int nWeek = Logical::LunarderDate::WeekDay(m_Year, m_Month, 1);
                int i = w-6;
                if(i >= nWeek && i < (nWeek+nDay))
                {
                    int nIndex = i+5;
                    int nOldIndex = m_curIndex;
                    m_MJPGList.SetUnitIsDownStatus(m_curIndex, FALSE);	
                    m_MJPGList.SetUnitIsDownStatus(nIndex, TRUE);
                    m_curIndex = nIndex;
                    m_Day = i - nWeek + 1;

                    //SetData(m_Year, m_Month, m_Day, TRUE);
                    ShowLunderRight(FALSE);

                    m_MJPGList.Invalidate();
                }
            }
            break;
        }
        return result;
    }

    void CHuangliDlg::SetData(int y, int m, int d, BOOL flag)
    {
        using Control::month;
        using Control::day;
        using Control::day1;
        DWORD dwStart = GetTickCount();

        BOOL isDraw = flag;
        BOOL isRunDayStart = FALSE;

        if(y < 1901)
            y = 2008;
        m_Year = y;
        m_Month = m;
        m_Day = d;

        //计算星期节日
        int nWeekRet = 0;
        int nWeekJieRi[3];
        char *pWeekJieRi[3];
        nWeekRet = Logical::LunarderDate::FindWeekJieri(y, m, nWeekJieRi, pWeekJieRi);

        char txt[64];
        //sprintf(txt, "%02d月", m_Month);
        //CString  sT = txt;
        CString sT;
        sT.Format(_T("%02d月"), m_Month);
        m_MJPGList.SetUnitText(56, sT, FALSE);
        //sprintf(txt, "%04d年", m_Year);
        //sT = txt;
        sT.Format(_T("%04d年"), m_Year);
        m_MJPGList.SetUnitText(59, sT, FALSE);

        int nDay = Logical::LunarderDate::MonthDays(m_Year, m_Month);
        int nWeek = Logical::LunarderDate::WeekDay(m_Year, m_Month, 1);

        if(m_Day > nDay)
            m_Day = nDay;

        m_MJPGList.SetUnitIsDownStatus(m_curIndex, FALSE);

        for (int i = 0; i < 42; ++i)
        {
            CString s1;
            CString sLunder;
            CString sJieri;
            int nIndex = i + 5;

            m_MJPGList.SetUnitColor(nIndex, font_black, isDraw, TRUE);
            m_MJPGList.SetUnitColor(nIndex+100, font_blue, isDraw, TRUE);
            m_MJPGList.SetUnitColor(nIndex+200, font_black, isDraw, TRUE);
            m_MJPGList.SetUnitColor(nIndex+300, font_black, isDraw, TRUE);

            if (i < nWeek)
            {
                m_MJPGList.SetUnitText(nIndex, s1, isDraw, TRUE);
                m_MJPGList.SetUnitText(nIndex+100, s1, isDraw, TRUE);
                m_MJPGList.SetUnitText(nIndex+200, s1, isDraw, TRUE);
                m_MJPGList.SetUnitText(nIndex+300, s1, isDraw, TRUE);

                m_MJPGList.SetUnitIsShow(nIndex, isDraw, TRUE);
                m_MJPGList.SetUnitIsShow(nIndex+100, isDraw, TRUE);
                m_MJPGList.SetUnitIsShow(nIndex+200, isDraw, TRUE);
                m_MJPGList.SetUnitIsShow(nIndex+300, isDraw, TRUE);
            }
            else if (i >= (nWeek + nDay))
            {
                m_MJPGList.SetUnitText(nIndex, s1, isDraw, TRUE);
                m_MJPGList.SetUnitText(nIndex+100, s1, isDraw, TRUE);
                m_MJPGList.SetUnitText(nIndex+200, s1, isDraw, TRUE);
                m_MJPGList.SetUnitText(nIndex+300, s1, isDraw, TRUE);

                m_MJPGList.SetUnitIsShow(nIndex, isDraw, TRUE);
                m_MJPGList.SetUnitIsShow(nIndex+100, isDraw, TRUE);
                m_MJPGList.SetUnitIsShow(nIndex+200, isDraw, TRUE);
                m_MJPGList.SetUnitIsShow(nIndex+300, isDraw, TRUE);
            }
            else
            {
                s1.Format(_T("%d"), (i + 1 - nWeek));

                ////////////////start/////////////
                ::UINT16 y, m, d, ret;
                ret = Logical::LunarderDate::GetLunarDate(m_Year, m_Month, i + 1 - nWeek, y, m, d);
                if (ret)
                {
                    m_MJPGList.SetUnitColor(nIndex, font_green, isDraw, TRUE);
                    m_MJPGList.SetUnitColor(nIndex+200, font_green, isDraw, TRUE);

                    char txt[6];
                    memset(txt, 0, 6);
                    Logical::LunarderDate::FormatHolDay(ret, txt);
                    sLunder = Util::StringOp::ToCString(txt);

                    //  lxz 20081104
                    int nn_ = i + 1 - nWeek;
                    char sBuff_[64] = {0};
                    sprintf(sBuff_, "%d日%s", nn_, txt);
                    sT= Util::StringOp::ToCString(sBuff_);
                    if(nn_<= 15)
                        m_MJPGList.SetUnitText(47, sT, FALSE);
                    else
                        m_MJPGList.SetUnitText(48, sT, FALSE);
                }
                else
                {
                    //红色表示休息日
                    if (d == 1)
                    {
                        ::UINT16 iYear, iMonth, iDay;
                        int n = Logical::LunarderDate::CalcLeapLunarYear(y, iYear, iMonth, iDay);
                        if (n > 0)
                        {
                            if (iYear == m_Year && iMonth == m_Month && iDay == (i + 1 - nWeek))
                            {
                                isRunDayStart = TRUE;
                                sLunder = Util::StringOp::ToCString(Data::LanguageResource::Get(Data::RI_LANUDER_LOOP));
                                sLunder += Util::StringOp::ToCString(Control::month()[m - 1]);
                            }
                            else
                            {
                                sLunder = Util::StringOp::ToCString(Control::month()[m - 1]);
                            }
                        }
                        else
                            sLunder = Util::StringOp::ToCString(Control::month()[m - 1]);
                    }
                    else
                    {
                        if(d <= 10)
                        {
                            sLunder = Util::StringOp::ToCString(Control::day()[0]);
                        }
                        else if(d < 20)
                        {
                            sLunder = Util::StringOp::ToCString(Control::day()[1]);
                        }
                        else if(d < 30)
                        {
                            sLunder = Util::StringOp::ToCString(Control::day()[2]);
                        }
                        else
                        {
                            sLunder = Util::StringOp::ToCString(Control::day()[3]);
                        } 
                        sLunder += Util::StringOp::ToCString(Control::day1()[d % 10]);
                    }
                }

                int nweek = Logical::LunarderDate::WeekDay(m_Year, m_Month, i + 1 - nWeek);
                if (nweek == 0 || nweek == 6)
                {
                    m_MJPGList.SetUnitColor(nIndex, font_red, isDraw, TRUE);
                    m_MJPGList.SetUnitColor(nIndex+100, font_red, isDraw, TRUE);
                    m_MJPGList.SetUnitColor(nIndex+200, font_red, isDraw, TRUE);
                    m_MJPGList.SetUnitColor(nIndex+300, font_red, isDraw, TRUE);
                }
                ////////////////end///////////////
                if (m_Day == (i + 1 - nWeek))
                {
                    m_MJPGList.SetUnitIsDownStatus(nIndex, TRUE);
                    m_curIndex = nIndex;
                }

                //	m_MJPGList.SetUnitText(nIndex, s1, isDraw, TRUE);
                m_MJPGList.SetUnitText(nIndex+300, s1, isDraw, TRUE);
                m_MJPGList.SetUnitText(nIndex+200, sLunder, isDraw, TRUE);

                char *sPtr = NULL;

                for (int ii = 0; ii < nWeekRet; ++ii)
                {
                    if ((i + 1 - nWeek) == nWeekJieRi[ii])
                    {
                        sJieri += pWeekJieRi[ii];
                    }
                }

                if (!isRunDayStart && (sPtr = Logical::LunarderDate::FindNongLiJieri(y, m, d, m_Year, m_Month, i+1-nWeek)) != NULL)
                {	
                    sJieri += sPtr;
                }

                if((sPtr = Logical::LunarderDate::FindYangLiJieri(m_Month, i+1-nWeek)) != NULL)
                {
                    sJieri += sPtr;
                }

                m_MJPGList.SetUnitText(nIndex+100, sJieri, isDraw, TRUE);

                m_MJPGList.SetUnitIsShow(nIndex, isDraw, TRUE);
                m_MJPGList.SetUnitIsShow(nIndex+100, isDraw, TRUE);
                m_MJPGList.SetUnitIsShow(nIndex+200, isDraw, TRUE);
                m_MJPGList.SetUnitIsShow(nIndex+300, isDraw, TRUE);


                //如果有提醒，将显示的文字改为红色
                //if(IsHaveAlarm(m_Year, m_Month, i+1-nWeek))
                //	m_MJPGList.SetUnitColor(nIndex, font_red, isDraw, TRUE);
            }

            //判断天，toggle
        }
        ShowLunderRight(isDraw);
        DWORD offset = GetTickCount() - dwStart;   
        TRACE(L"Lunder %d年%d月 %d\n", m_Year, m_Month, offset);
    }


    void CHuangliDlg::ShowLunderRight(BOOL isDraw)
    {
        char txt[64];
        CString sT;
        //lxz 20081104
        sprintf(txt, "%04d年%02d月", m_Year, m_Month);
        sT = txt;
        m_MJPGList.SetUnitText(49, sT, FALSE);

        sprintf(txt, "%d", m_Day);
        sT = txt;
        m_MJPGList.SetUnitText(50, sT, FALSE);

        int nWeek = Logical::LunarderDate::WeekDay(m_Year, m_Month, m_Day);
        sprintf(txt, "%s", Data::LanguageResource::Get(Data::RI_COMN_SUNSTC+nWeek).c_str());
        sT = txt;
        m_MJPGList.SetUnitText(51, sT, FALSE);

        ::UINT16 nY, nM, nD;
        int nHoliday = Logical::LunarderDate::GetLunarDate(m_Year, m_Month, m_Day,  nY, nM, nD);
        int nLeapMonth = Logical::LunarderDate::GetLeapMonth(nY);
        char chYear[16], chMonth[16], chDay[16], chLeapMonth[16];

        memset(chYear, 0, 16);
        memset(chMonth, 0, 16);
        memset(chDay, 0, 16);
        memset(chLeapMonth, 0, 16);

        int nYear = Logical::LunarderDate::FormatLunarYear(nY, chYear);
        Logical::LunarderDate::FormatLunarMonth(nM, chMonth);
        Logical::LunarderDate::FormatLunarDay(nD, chDay);
        if(nLeapMonth == nM )
        {
            ::UINT16 iY, iM, iD;
            Logical::LunarderDate::CalcLeapLunarYear(nY, iY, iM, iD);
            if((m_Month == iM && m_Day >= iD) || m_Month > iM)
                strcpy(chLeapMonth, "润");
        }
        sprintf(txt, "%s%s%s%s%s%s", Data::LanguageResource::Get(Data::RI_MAIN_LUNARCALENDARSTC).c_str(), chYear, Data::LanguageResource::Get(Data::RI_COMN_YEARSTC).c_str(), chLeapMonth, chMonth, chDay);
        sT = txt;
        m_MJPGList.SetUnitText(52, sT, FALSE);

        //计算星期节日
        int nWeekRet = 0;
        int nWeekJieRi[3];
        char *pWeekJieRi[3];
        nWeekRet = Logical::LunarderDate::FindWeekJieri(m_Year, m_Month, nWeekJieRi, pWeekJieRi);

        CString sJieri = _T("");
        char *sPtr = NULL;
        for(int ii = 0; ii < nWeekRet; ii++)
        {
            if(m_Day == nWeekJieRi[ii])
            {
                sJieri += pWeekJieRi[ii];
            }
        }

        if( (sPtr = Logical::LunarderDate::FindNongLiJieri(nY, nM, nD, m_Year, m_Month, m_Day)))
        {	
            sJieri += "";
            sJieri += sPtr;
        }
        if((sPtr = Logical::LunarderDate::FindYangLiJieri(m_Month, m_Day)))
        {
            sJieri += "";
            sJieri += sPtr;
        }
        m_MJPGList.SetUnitText(53, sJieri, FALSE);

        char yi_ji[512] = {0};
        Logical::LunarderDate::FindHuangDaoData(CTime(m_Year, m_Month, m_Day, 0, 0, 0), yi_ji);
        sT = yi_ji;
        m_MJPGList.SetUnitText(54, sT, FALSE);
    }
}
