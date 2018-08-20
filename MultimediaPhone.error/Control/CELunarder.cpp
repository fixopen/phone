// CELunarder.cpp : implementation file
//

#include "stdafx.h"
#include "CELunarder.h"
#include "../Logical/LunarderDate.h"
#include "../Data/LanguageResource.h"
#include "../Data/SkinStyle.h"
#include "../Data/SchedulerData.h"
#include "../resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace Control {
#define IDC_BTN_D0			0xFF04
#define IDT_TIP_TIMER		4000

    /////////////////////////////////////////////////////////////////////////////
    // CCELunarder

    //227*200

    CCELunarder::CCELunarder()
    {
        m_nY = 2007;
        m_nM = 10;
        m_nD = 1;
        m_pStcYear = NULL;
        m_pStcMonth = NULL;
        m_bIsTipStatic = FALSE;
    }

    CCELunarder::~CCELunarder()
    {
    }

    void CCELunarder::SetData(UINT16 year, UINT8 month, UINT8 day, BOOL reDraw)
    {
        m_nY = year;
        if(m_nY < 1900)
            m_nY = 1901;
        m_nM = month;
        m_nD = day;
        if(reDraw)
        {
            char txt[24], txt1[24], txt2[24];
            Logical::LunarderDate::FormatLunarYear(m_nY, txt1);	
            //sprintf(txt2, "%d %s %s", m_nY, txt1, Data::LanguageResource::Get(Data::RI_COMN_YEARSTC).c_str());
            //CString s = txt2;
            CString s;
            s.Format(_T("%d %s %s"), m_nY, txt1, Data::LanguageResource::Get(Data::RI_COMN_YEARSTC).c_str());
            if(m_pStcYear)
                m_pStcYear->SetWindowText(s);
            //sprintf(txt, "%d%s", m_nM, Data::LanguageResource::Get(Data::RI_COMN_MONTHSTC).c_str());
            //s = txt;
            s.Format(_T("%d%s"), m_nM, Data::LanguageResource::Get(Data::RI_COMN_MONTHSTC).c_str());
            if(m_pStcMonth)
                m_pStcMonth->SetWindowText(s);
            SetStcDay();
            OnPaint();
        }
    }

    void CCELunarder::GetData(UINT16 &year, UINT8 &month, UINT8 &day)
    {
        year = m_nY;
        if(year < 1900)
            year = 1901;
        month = m_nM;
        day = m_nD;
    }

    void CCELunarder::SetStatic(CCEStatic *pStatic1, CCEStatic *pStatic2)
    {
        m_pStcYear =  pStatic1;
        m_pStcMonth = pStatic2;
    }

    BOOL CCELunarder::Create(CCEStatic *pStatic1, CCEStatic *pStatic2, int style, CRect &rect, CWnd *pParentWnd, int nCtrlID)
    {
        m_pStcYear =  pStatic1;
        m_pStcMonth = pStatic2;
        CStatic::Create(L"", style, rect, pParentWnd);
        CRect rt;
        rt = CRect(0, 0, 224, 15);
        //CString s = Data::LanguageResource::Get(Data::RI_LANUDER_WEEK).c_str();
        CString s;
        s.Format(_T("%s"), Data::LanguageResource::Get(Data::RI_LANUDER_WEEK).c_str());
        m_StcWeek.Create(s, WS_CHILD|WS_VISIBLE, rt, this);
        m_StcWeek.SetLeftMargin(0);
        m_StcWeek.SetColor(RGB(0, 0, 0), Data::g_lunarderTitleRGB[Data::g_skinstyle]);

        //	m_StcYear.SetColor(RGB(0, 0, 0,), RGB(204, 184, 218));
        //	m_StcMonth.SetColor(RGB(0, 0, 0,), RGB(204, 184, 218));

        m_tipStatic.Create(L"", WS_CHILD|WS_BORDER, CRect(0, 0, 192, 770), this);
        m_tipStatic.SetColor(RGB(0, 0, 0), Data::g_partFrameInRectBackRGB[Data::g_skinstyle]);
        m_tipStatic.SetAlign(DT_LEFT|DT_WORDBREAK);

        for(int i = 0; i < 6; i++)
            for(int j = 0; j < 7; j++)
            {
                rt = CRect(j*32, 16+i*26, (j+1)*32-1, 16+(i+1)*26-1);
                m_StcDay[i*7+j].Create(L"", WS_CHILD|WS_VISIBLE, rt, this, IDC_BTN_D0+i*7+j);
                m_StcDay[i*7+j].SetLeftMargin(0);
                m_StcDay[i*7+j].SetColor(RGB(0, 0, 0), Data::g_lunarderBackRGB[Data::g_skinstyle]);
                m_StcDay[i*7+j].SetToggleColor(Data::g_lunarderSecDayRGB[Data::g_skinstyle]);
            }
            SetStcDay();

            return TRUE;
    }

    std::vector<std::string> month() {
        /*static const char* data[] = {
            Data::LanguageResource::Get(Data::RI_LANUDER_MONTH1).c_str(),\
            Data::LanguageResource::Get(Data::RI_LANUDER_MONTH2).c_str(),\
            Data::LanguageResource::Get(Data::RI_LANUDER_MONTH3).c_str(),\
            Data::LanguageResource::Get(Data::RI_LANUDER_MONTH4).c_str(),\
            Data::LanguageResource::Get(Data::RI_LANUDER_MONTH5).c_str(),\
            Data::LanguageResource::Get(Data::RI_LANUDER_MONTH6).c_str(),\
            Data::LanguageResource::Get(Data::RI_LANUDER_MONTH7).c_str(),\
            Data::LanguageResource::Get(Data::RI_LANUDER_MONTH8).c_str(),\
            Data::LanguageResource::Get(Data::RI_LANUDER_MONTH9).c_str(),\
            Data::LanguageResource::Get(Data::RI_LANUDER_MONTH10).c_str(),\
            Data::LanguageResource::Get(Data::RI_LANUDER_MONTH11).c_str(),\
            Data::LanguageResource::Get(Data::RI_LANUDER_MONTH12).c_str()
        };*/
        static std::vector<std::string> result;
        if (result.empty()) {
            result.reserve(12);
            result.push_back(Data::LanguageResource::Get(Data::RI_LANUDER_MONTH1));
            result.push_back(Data::LanguageResource::Get(Data::RI_LANUDER_MONTH2));
            result.push_back(Data::LanguageResource::Get(Data::RI_LANUDER_MONTH3));
            result.push_back(Data::LanguageResource::Get(Data::RI_LANUDER_MONTH4));
            result.push_back(Data::LanguageResource::Get(Data::RI_LANUDER_MONTH5));
            result.push_back(Data::LanguageResource::Get(Data::RI_LANUDER_MONTH6));
            result.push_back(Data::LanguageResource::Get(Data::RI_LANUDER_MONTH7));
            result.push_back(Data::LanguageResource::Get(Data::RI_LANUDER_MONTH8));
            result.push_back(Data::LanguageResource::Get(Data::RI_LANUDER_MONTH9));
            result.push_back(Data::LanguageResource::Get(Data::RI_LANUDER_MONTH10));
            result.push_back(Data::LanguageResource::Get(Data::RI_LANUDER_MONTH11));
            result.push_back(Data::LanguageResource::Get(Data::RI_LANUDER_MONTH12));
        }
        return result;
    }

    std::vector<std::string> day() {
        /*static const char* data[] = {
            Data::LanguageResource::Get(Data::RI_LANUDER_DAY10).c_str(),\
            Data::LanguageResource::Get(Data::RI_LANUDER_DAY11).c_str(),\
            Data::LanguageResource::Get(Data::RI_LANUDER_DAY12).c_str(),\
            Data::LanguageResource::Get(Data::RI_LANUDER_DAY13).c_str()
        };*/
        static std::vector<std::string> result;
        if (result.empty()) {
            result.reserve(4);
            result.push_back(Data::LanguageResource::Get(Data::RI_LANUDER_DAY10));
            result.push_back(Data::LanguageResource::Get(Data::RI_LANUDER_DAY11));
            result.push_back(Data::LanguageResource::Get(Data::RI_LANUDER_DAY12));
            result.push_back(Data::LanguageResource::Get(Data::RI_LANUDER_DAY13));
        }
        return result;
    }

    std::vector<std::string> day1() {
        /*static const char* data[] = {
            Data::LanguageResource::Get(Data::RI_LANUDER_DAY0).c_str(),\
            Data::LanguageResource::Get(Data::RI_LANUDER_DAY1).c_str(),\
            Data::LanguageResource::Get(Data::RI_LANUDER_DAY2).c_str(),\
            Data::LanguageResource::Get(Data::RI_LANUDER_DAY3).c_str(),\
            Data::LanguageResource::Get(Data::RI_LANUDER_DAY4).c_str(),\
            Data::LanguageResource::Get(Data::RI_LANUDER_DAY5).c_str(),\
            Data::LanguageResource::Get(Data::RI_LANUDER_DAY6).c_str(),\
            Data::LanguageResource::Get(Data::RI_LANUDER_DAY7).c_str(),\
            Data::LanguageResource::Get(Data::RI_LANUDER_DAY8).c_str(),\
            Data::LanguageResource::Get(Data::RI_LANUDER_DAY9).c_str()
        };*/
        static std::vector<std::string> result;
        if (result.empty()) {
            result.reserve(10);
            result.push_back(Data::LanguageResource::Get(Data::RI_LANUDER_DAY0));
            result.push_back(Data::LanguageResource::Get(Data::RI_LANUDER_DAY1));
            result.push_back(Data::LanguageResource::Get(Data::RI_LANUDER_DAY2));
            result.push_back(Data::LanguageResource::Get(Data::RI_LANUDER_DAY3));
            result.push_back(Data::LanguageResource::Get(Data::RI_LANUDER_DAY4));
            result.push_back(Data::LanguageResource::Get(Data::RI_LANUDER_DAY5));
            result.push_back(Data::LanguageResource::Get(Data::RI_LANUDER_DAY6));
            result.push_back(Data::LanguageResource::Get(Data::RI_LANUDER_DAY7));
            result.push_back(Data::LanguageResource::Get(Data::RI_LANUDER_DAY8));
            result.push_back(Data::LanguageResource::Get(Data::RI_LANUDER_DAY9));
        }
        return result;
    }

    void CCELunarder::SetStcDay()
    {
        int nDay = Logical::LunarderDate::MonthDays(m_nY, m_nM);
        int nWeek = Logical::LunarderDate::WeekDay(m_nY, m_nM, 1);

        for(int i = 0; i < 42; i++)
        {
            CString s1 = _T("");
            m_StcDay[i].SetDateTxt(s1, s1);
            m_StcDay[i].SetColor(RGB(0, 0, 0), Data::g_lunarderBackRGB[Data::g_skinstyle]);
            m_StcDay[i].SetWindowText(L"");
            m_StcDay[i].SetClicked(FALSE);
            m_StcDay[i].SetTogle(FALSE);
        }

        for(int i = 0; i < nDay; i++)
        {
            //char txt[3];
            //sprintf(txt, "%d", i+1);
            //CString s = txt;
            CString s;
            s.Format(_T("%d"), i + 1);
            //CString s1 = _T("");
            CString s1;
            //m_StcDay[nWeek+i].SetWindowText(s);
            UINT16 y, m, d, ret;
            ret = Logical::LunarderDate::GetLunarDate(m_nY, m_nM, i+1, y, m, d);
            if(ret)
            {
                char txt[6];
                memset(txt, 0, 6);
                Logical::LunarderDate::FormatHolDay(ret, txt);
                s1 = txt;
            }
            else
            {
                if(d == 1)
                {
                    //s1 = month[m-1];
                    UINT16 iYear, iMonth, iDay;
                    int n = Logical::LunarderDate::CalcLeapLunarYear(y, iYear, iMonth, iDay);
                    if(n > 0 )
                    {
                        if(iYear == m_nY && iMonth == m_nM && iDay == (i+1))
                        {
                            s1 = Data::LanguageResource::Get(Data::RI_LANUDER_LOOP).c_str();
                            s1 += Control::month()[m-1].c_str();
                        }
                        else
                        {
                            s1 = Control::month()[m-1].c_str();
                        }
                    }
                    else
                        s1 = Control::month()[m-1].c_str();
                }
                else
                {
                    if(d <= 10)
                    {
                        s1 = Control::day()[0].c_str();
                    }
                    else if(d < 20)
                    {
                        s1 = Control::day()[1].c_str();
                    }
                    else if(d < 30)
                    {
                        s1 = Control::day()[2].c_str();
                    }
                    else
                    {
                        s1 = Control::day()[3].c_str();
                    } 
                    s1 += Control::day1()[d % 10].c_str();
                }
            }

            //添加是否有记事的接口，如果有记事用红色字体表示
            CTime time = CTime(m_nY, m_nM, i+1, 0, 0, 0);
            std::vector<Util::shared_ptr<Data::Scheduler> > result = Data::Scheduler::GetFromDatabase("startTime = '" + Util::StringOp::FromTimestamp(time) + "'"); 
            if (!result.empty())
            {
                m_StcDay[nWeek+i].SetColor(Data::g_lunarderNoteDayRGB[Data::g_skinstyle], Data::g_lunarderBackRGB[Data::g_skinstyle]);
            }

            m_StcDay[nWeek+i].SetDateTxt(s, s1);
            if ((i + 1) == m_nD)
            {
                m_StcDay[nWeek+i].SetTogle(TRUE);
                char txt1[24], txt2[24];
                Logical::LunarderDate::FormatLunarYear(y, txt1);	
                //sprintf(txt2, "%d %s %s", m_nY, txt1, Data::LanguageResource::Get(Data::RI_COMN_YEARSTC).c_str());
                //CString s = txt2;
                CString s;
                s.Format(_T("%d %s %s"), m_nY, txt1, Data::LanguageResource::Get(Data::RI_COMN_YEARSTC).c_str());
                if(m_pStcYear)
                    m_pStcYear->SetWindowText(s);
                //sprintf(txt, "%d%s", m_nM, Data::LanguageResource::Get(Data::RI_COMN_MONTHSTC).c_str());
                //s = txt;
                s.Format(_T("%d%s"), m_nM, Data::LanguageResource::Get(Data::RI_COMN_MONTHSTC).c_str());
                if(m_pStcMonth)
                    m_pStcMonth->SetWindowText(s);	
            }
            m_StcDay[nWeek+i].SetClicked(TRUE);
        }
    }

    BEGIN_MESSAGE_MAP(CCELunarder, CStatic)
        //{{AFX_MSG_MAP(CCELunarder)
        ON_WM_TIMER()
        ON_WM_PAINT()
        ON_WM_ERASEBKGND()
        ON_MESSAGE(WM_STATIC_CLICK, OnDaySelect)
        //}}AFX_MSG_MAP
    END_MESSAGE_MAP()

    /////////////////////////////////////////////////////////////////////////////
    // CCELunarder message handlers

    void CCELunarder::OnPaint() 
    {
        CPaintDC dc(this); // device context for painting

        // TODO: Add your message handler code here

        // Do not call CStatic::OnPaint() for painting messages
    }

    BOOL CCELunarder::OnEraseBkgnd(CDC* pDC) 
    {
        // TODO: Add your message handler code here and/or call default
        CRect rt;
        GetClientRect(&rt);
        pDC->FillSolidRect(&rt, Data::g_lunarderLineRGB[Data::g_skinstyle]);
        return CStatic::OnEraseBkgnd(pDC);
    }

    LRESULT CCELunarder::OnDaySelect(WPARAM w, LPARAM l)
    {
        LRESULT result = 0;
        CString s;
        m_StcDay[w-IDC_BTN_D0].GetDateTxt(s);
        char txt[3];
        int n = wcstombs( txt, s.GetBuffer(3), 3);
        txt[n] = '\0';


        int nWeek = Logical::LunarderDate::WeekDay(m_nY, m_nM, 1);
        n = nWeek+m_nD-1;
        m_StcDay[n].SetTogle(FALSE, TRUE);
        m_nD = atoi(txt);
        n = nWeek+m_nD-1;
        m_StcDay[n].SetTogle(TRUE, TRUE);

        UINT16 y, m, d;
        int ret = Logical::LunarderDate::GetLunarDate(m_nY, m_nM, m_nD, y, m, d);
        char txt1[24], txt2[24];
        Logical::LunarderDate::FormatLunarYear(y, txt1);	
        sprintf(txt2, "%d %s %s", m_nY, txt1, Data::LanguageResource::Get(Data::RI_COMN_YEARSTC).c_str());
        s = txt2;
        if(m_pStcYear)
            m_pStcYear->SetWindowText(s);

        CRect rt;
        int index = w - IDC_BTN_D0;
        m_StcDay[index].GetWindowRect(&rt);

        if(l < 18)
        {
            //m_tipStatic.ShowWindow(SW_HIDE);
            //通知显示记事
            CWnd *pParent = GetParent();
            if(pParent)
                pParent->SendMessage(WM_LUNAR_CLICKED, 0);
        }

        else
        {
            //老黄历自动显示
            if(m_bIsTipStatic)
            {
                if( index >= 0 )
                {
                    //移到日程应用中去处理
                    //通知显示黄历
                    CWnd *pParent = GetParent();
                    if(pParent)
                        pParent->SendMessage(WM_LUNAR_CLICKED, 1);
                    /*
                    ScreenToClient(&rt);

                    char data[512];
                    memset(data, 0, 512);
                    CTime time = CTime(m_nY, m_nM, m_nD, 0, 0, 0);
                    if(Logical::LunarderDate::FindHuangDaoData(time, data))
                    {
                    CString s = data;
                    if(index < 21)
                    {
                    m_tipStatic.SetWindowText(s);
                    m_tipStatic.MoveWindow(16, rt.bottom+4, 192, 70, FALSE);
                    m_tipStatic.SendMessage(WM_PAINT);

                    }
                    else
                    {
                    m_tipStatic.SetWindowText(s);
                    m_tipStatic.MoveWindow(16, rt.top-4-70, 192, 70, FALSE);
                    m_tipStatic.SendMessage(WM_PAINT);
                    }
                    m_tipStatic.ShowWindow(SW_SHOW);
                    SetTimer(IDT_TIP_TIMER, 8000, NULL);
                    }
                    */
                }
            }
        }
        return result;
    }

    void CCELunarder::OnTimer(UINT nID)
    {
        CStatic::OnTimer(nID);

        if(nID = IDT_TIP_TIMER)
        {
            KillTimer(IDT_TIP_TIMER);
            m_tipStatic.ShowWindow(SW_HIDE);
        }
    }

    void CCELunarder::OnYleft()
    {
        m_nY--;
        if(m_nY < START_YEAR)
        {
            m_nY = END_YEAR;
        }
        SetStcDay();
    }

    void CCELunarder::OnYright()
    {
        ++m_nY;
        if (m_nY > END_YEAR)
        {
            m_nY = START_YEAR;
        }
        SetStcDay();
    }

    void CCELunarder::OnMleft()
    {
        --m_nM;
        if (m_nM < 1)
        {
            m_nM = 12;
        }
        SetStcDay();
    }

    void CCELunarder::OnMright()
    {
        ++m_nM;
        if (m_nM > 12)
        {
            m_nM = 1;
        }
        SetStcDay();
    }

    BOOL CCELunarder::IsHaveNote(int y, int m ,int d, CString &s)
    {
        if( y < 1900 )
            y = 2007;
        CTime time = CTime(y, m, d, 0, 0, 0);
        std::vector<Util::shared_ptr<Data::Scheduler> > result = Data::Scheduler::GetFromDatabase("startTime = '" + Util::StringOp::FromTimestamp(time) + "'"); 
        if (!result.empty())
        {
            Util::shared_ptr<Data::Scheduler> m_pScheduler = result[0];
            s = m_pScheduler->what().c_str();
            return TRUE;
        }
        return FALSE;
    }
}
