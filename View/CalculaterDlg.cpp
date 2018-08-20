// Calculater.cpp : implementation file
//

#include "stdafx.h"

#include <complex>

#include "../resource.h"
#include "../Data/LanguageResource.h"
#include "../Data/SkinStyle.h"
#include "CalculaterDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
namespace View {
    //wsprintf(buffer, _T("%16.16f"), m_number1);

    double WTOF(CString buffer)
    {
        char buf[32];
        memset(buf, 0, 32);
        int n = wcstombs( buf, buffer.GetBuffer(30), 30);
        buf[n] = '\0';
        return atof(buf);
    }

    //自定义键值
#define SOFTKEY1_BACK	 400
#define m_keynull        490
#define C_KEYV           500
#define MJ_KEYV          510
#define MS_KEYV          520 
#define MR_KEYV          530 
#define MC_KEYV          540 
#define GENHAO_KEYV      550 
#define DAOSHU_KEYV      560 
#define FUHAO_KEYV       570 
#define CHU_KEYV         580 
#define CHENG_KEYV       590 
#define JIAN_KEYV        600 
#define JIA_KEYV         610 
#define DENG_KEYV        620 
#define BAIFENHAO_KEYV   630 
#define BACKSPACE_KEYV	 640
#define CE_KEYV			 650
    /////////////////////////////////////////////////////////////////////////////
    // CCalculaterDlg dialog
    ///////////////////////2004.12.16////////////////

    /////////////////////sprintf()//////////////////
    void wsprintf_1(WCHAR *wstr, int num, double val)
    {
        char str[128]; 
        memset(str, 0, 128);

        _gcvt(val, num, str);
        int len = strlen(str);
        str[len] = '0';

        /*
        double tt1= ceil(val);
        char *pTxt = ecvt(val, num, &n, &s);

        if(s == 1)
        {
        strcpy(str, "-");
        if(n <= 0)
        {
        n = abs(n);
        strcat(str, "0.");
        while(n--)
        {
        strcat(str, "0");
        }
        strcat(str, pTxt);
        }
        else
        {
        char txt[20];
        memset(txt, 0, 20);
        strncpy(txt, pTxt, n);
        strcat(str, txt);
        strcat(str, ".");
        strcat(str, (pTxt+n));
        }
        }
        else
        {
        if(n <= 0)
        {
        n = abs(n);
        strcpy(str, "0.");
        while(n--)
        {
        strcat(str, "0");
        }
        strcat(str, pTxt);
        }
        else
        {
        char txt[20];
        memset(txt, 0, 20);
        strncpy(txt, pTxt, n);
        strcpy(str, txt);
        strcat(str, ".");
        strcat(str, (pTxt+n));
        }
        }
        */

        mbstowcs(wstr, str, strlen(str)+1);
    }

    CCalculaterDlg::CCalculaterDlg(CWnd* pParent /*=NULL*/)
        : CDialog(CCalculaterDlg::IDD, pParent)
    {
        //{{AFX_DATA_INIT(CCalculaterDlg)
        // NOTE: the ClassWizard will add member initialization here
        //}}AFX_DATA_INIT
    }


    void CCalculaterDlg::DoDataExchange(CDataExchange* pDX)
    {
        CDialog::DoDataExchange(pDX);
        //{{AFX_DATA_MAP(CCalculaterDlg)
        // NOTE: the ClassWizard will add DDX and DDV calls here
        //}}AFX_DATA_MAP
    }


    BEGIN_MESSAGE_MAP(CCalculaterDlg, CDialog)
        //{{AFX_MSG_MAP(CCalculaterDlg)
        ON_MESSAGE(WM_CLICKMJPG_TOAPP, OnClickMJPG)
        ON_WM_PAINT()
        //}}AFX_MSG_MAP
    END_MESSAGE_MAP()

    void CCalculaterDlg::OnExit()
    {
        OnSoftKey(C_KEYV, 0);
        GetParent()->SendMessage(WM_CHANGEWINDOW, (WPARAM)this, (LPARAM)SW_HIDE);
    }
    /////////////////////////////////////////////////////////////////////////////
    // CCalculaterDlg message handlers

    BOOL CCalculaterDlg::OnInitDialog() 
    {
        CDialog::OnInitDialog();

        // TODO: Add extra initialization here
        //2005.3.30－zmy 初始化
        m_number1 = 0;
        m_dtemp0 = 0;
        m_dtemp1 = 0;
        m_dtemp2 = 0;
        m_IsError = FALSE; //2005.5.19 zmy

		m_MJPGList.Create(L"", WS_VISIBLE|WS_CHILD, CRect(0, 0, 535*X_XISHU, 420*Y_XISHU), this);
        m_MJPGList.SetCurrentLinkFile(_T(".\\adv\\mjpg\\k1\\中文\\计算器.xml"));
        m_MJPGList.SetMJPGRect(CRect(0, 0, 535*X_XISHU, 420*Y_XISHU));

        CRect r(32*X_XISHU, 60*Y_XISHU, (32+470)**X_XISHU, (60+38)*Y_XISHU);
        m_DialNumEdit.Create(WS_CHILD|WS_VISIBLE|ES_RIGHT|ES_NUMBER|ES_MULTILINE, r, &m_MJPGList, 0xFFFF);
        m_DialNumEdit.SetIsAutoInput();
        /*	
        VERIFY(m_Font.CreateFont(
        18,                        // nHeight
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
        m_DialNumEdit.SetFont(&m_Font,FALSE);   
        */
        m_DialNumEdit.HideCaret();
        m_DialNumEdit.SetLimitText(16);

        //	m_DialNumEdit.SetFontSize(24);
        //	m_DialNumEdit.SetColor(RGB(0, 0, 0), RGB(255, 255, 255));

        return TRUE;  // return TRUE unless you set the focus to a control
        // EXCEPTION: OCX Property Pages should return FALSE
    }

    LRESULT CCalculaterDlg ::OnClickMJPG(WPARAM w, LPARAM l)
    {
        LRESULT result = 0;
        ::UINT16 keyV[] = {0x800, BACKSPACE_KEYV, CE_KEYV, C_KEYV,\
            MC_KEYV, '7', '8', '9', CHU_KEYV, GENHAO_KEYV,\
            MR_KEYV, '4', '5', '6', CHENG_KEYV, BAIFENHAO_KEYV,\
            MS_KEYV, '1', '2', '3', JIAN_KEYV, DAOSHU_KEYV,\
            MJ_KEYV, '0', FUHAO_KEYV, '.', JIA_KEYV, DENG_KEYV
        };
        OnSoftKey(keyV[w-1], 0);
        return result;
    }
    
    void CCalculaterDlg::OnPaint()
    {
        CPaintDC dc(this); // 用于绘制的设备上下文
    }

    //软键盘的响应函数
    void CCalculaterDlg::OnSoftKey(WPARAM w, LPARAM l)
    {
        if(w == 0x800)   //退出
        {
            OnExit();
            return;
        }

        if (m_IsError && w != C_KEYV)return; //2005.5.19 zmy 错误，点击任何键都无反应		

        //数字键的输入
        if (w < m_keynull)
        {
            if (m_keyvalue < C_KEYV)
            {	
                WCHAR t_buffer[20];
                //CString s;
                int len = m_DialNumEdit.GetWindowText(t_buffer, 20);
                if((len == 0) && (w == '.'))return;
                if((len == 1) && (t_buffer[0] == '0') && (w == '0'))return;
                for (int i = wcslen(t_buffer); i > 0; i --)
                    if ((t_buffer[i-1] == '.') && (w == '.')) return;//小数点只能点击一次

                if (len < 9)
                {
                    CString s;
                    m_DialNumEdit.GetWindowText(s);
                    if(s == "0")
                        s == "";
                    s += (char)w;
                    m_DialNumEdit.SetWindowText(s);

                    m_dCurtemp = WTOF(s);
                }			
            }
            else
            {
                m_DialNumEdit.SetWindowText(L"");
                if(w == '.')return;
                CString s;
                m_DialNumEdit.GetWindowText(s);
                if(s == "0")
                    s == "";
                s += (char)w;
                m_DialNumEdit.SetWindowText(s);
                m_keyvalue1 = m_keyvalue;
                m_keyvalue = m_keynull;
                m_keyvalue2 = m_keynull;
                m_keyvalue3 = m_keynull;
                m_dCurtemp = WTOF(s);
            }
            if (m_keyvalue1 >= CHU_KEYV)
            {
                WCHAR buffer[20];
                memset(buffer, 0, 40);
                m_DialNumEdit.GetWindowText(buffer, 20);
                m_dtemp2 = WTOF(buffer);

                m_dCurtemp = m_dtemp2;		//wwf
            }
        }

        //符号键输入的响应函数
        else
        {
            //2005.3.30-zmy 当输入框为空时，点击符号键（除了MS外），不显示
            WCHAR buff[20];
            int len = m_DialNumEdit.GetWindowText(buff, 20);
            if ((len == 0) && (w != MR_KEYV)) return;
            WCHAR buffer[20];
            WCHAR buffer1[20];
            int i;
            int j;
            double temp1 = 0;
            double temp2 = 0;
            double dtemp1;

            switch ( w ) 
            {
            case BACKSPACE_KEYV:	//delete
                {
                    CString s;
                    m_DialNumEdit.GetWindowText(s);
                    int len = s.GetLength();
                    if(len > 0)
                    {
                        s = s.Left(len-1);
                        m_DialNumEdit.SetWindowText(s);

                        m_dCurtemp = WTOF(s);		//wwf
                    }
                }

                break;
                //清除作用
            case C_KEYV :
                m_DialNumEdit.SetWindowText(L"");
                m_dtemp0 = 0;
                m_keyvalue = m_keynull;
                m_keyvalue1 = m_keynull;
                m_IsError = FALSE;  //2005.5.19 zmy 出现错误时，只能点击清楚键，其他键无效
                if (m_keyvalue2 == CHU_KEYV || m_keyvalue2 == CHENG_KEYV || m_keyvalue2 == JIAN_KEYV \
                    ||m_keyvalue2 == JIA_KEYV || m_keyvalue2 == DENG_KEYV)
                {
                    m_keyvalue2 = m_keynull;
                }
                m_dCurtemp = 0;					//wwf

                break;

            case CE_KEYV :
                //ClearData();
                m_DialNumEdit.SetWindowText(L"");

                m_dCurtemp = 0;					//wwf
                //	m_dtemp0 = 0;
                /*
                m_keyvalue = m_keynull;
                m_keyvalue1 = m_keynull;
                m_keyvalue2 = m_keynull;
                */
                break;

                //当前数字和存储器中的数据相加并存到存储器中
            case MJ_KEYV :
                // 				m_DialNumEdit.GetWindowText(buffer, 20);
                // 				dtemp1 = WTOF(buffer);
                // 				m_number1 = m_number1 + dtemp1;
                m_number1 = m_number1 + m_dCurtemp;
                m_keyvalue = MJ_KEYV;
                break;

                //当前数字存储到存储器中并替换掉原来存储器中的值
            case MS_KEYV :
                m_number1 = m_dCurtemp;
                m_keyvalue = MS_KEYV;
                break;

                //调用存储器中的数值
            case MR_KEYV :
                m_DialNumEdit.SetWindowText(L"");
                wsprintf(buffer, _T("%16.14f"), m_number1);
                for (i =  wcslen(buffer); i > 0; i --)
                    if (buffer[i-1] != '0')break;
                if (buffer[i-1] == '.')
                {
                    i = i - 1;
                }
                memcpy(buffer1, buffer, i*2);
                buffer1[i] = '\0';
                m_DialNumEdit.SetWindowText(buffer1);

                m_dCurtemp = WTOF(buffer1);				//wwf

                m_keyvalue = MR_KEYV;
                break;

                //把存储器中的数值清除并设为零
            case MC_KEYV :
                m_number1 = 0;
                m_keyvalue = MC_KEYV;
                break;

                //当前数字取根号值
            case GENHAO_KEYV :
                if(m_dCurtemp >= 0)
                {
                    m_dCurtemp = std::sqrt(m_dCurtemp);		//lxz 2004.9.5			
                    wsprintf(buffer, _T("%16.14f"),m_dCurtemp);				
                    for (i =  wcslen(buffer); i > 0; i --)
                        if (buffer[i-1] != '0')break;
                    if (buffer[i-1] == '.')
                    {
                        i = i - 1;
                    }
                    memcpy(buffer1, buffer, i*2);
                    buffer1[i] = '\0';
                    m_DialNumEdit.SetWindowText(buffer1);

                }
                else 
                {
                    //m_DialNumEdit.SetWindowText("函数输入无效!");
                    m_DialNumEdit.SetWindowText(L"Error");
                    m_IsError = TRUE;//2005.5.19 zmy 设置错误的标志
                    m_dCurtemp = 0;
                }
                m_keyvalue = GENHAO_KEYV;
                break;

                //当前数字取倒数值
            case DAOSHU_KEYV :
                if (m_dCurtemp != 0)
                {
                    m_dCurtemp = 1.0 / m_dCurtemp;
                    wsprintf(buffer, _T("%16.14f"), m_dCurtemp);
                    for (i =  wcslen(buffer); i > 0; i --)
                        if (buffer[i-1] != '0')break;
                    if (buffer[i-1] == '.')
                    {
                        i = i - 1;
                    }
                    memcpy(buffer1, buffer, i*2);
                    buffer1[i] = '\0';
                    m_DialNumEdit.SetWindowText(buffer1);
                }
                else
                {
                    m_DialNumEdit.SetWindowText(L"Error");
                    m_IsError = TRUE;//2005.5.19 zmy 设置错误的标志
                }

                m_keyvalue = DAOSHU_KEYV;
                break;

                //当前数字取负号
            case FUHAO_KEYV :
                m_dCurtemp = -m_dCurtemp;
                //sprintf_(buffer, "%.12f", temp2);
                wsprintf(buffer, _T("%16.16f"), m_dCurtemp);
                for (i =  wcslen(buffer); i > 0; i --)
                    if (buffer[i-1] != '0')break;
                if (buffer[i-1] == '.')
                {
                    i = i - 1;
                }
                memcpy(buffer1, buffer, i*2);
                buffer1[i] = '\0';	
                m_DialNumEdit.SetWindowText(buffer1);

                if (m_keyvalue != DAOSHU_KEYV && m_keyvalue != GENHAO_KEYV)
                {
                    j =  wcslen(buffer1);
                    m_keyvalue = m_keynull;
                }
                else m_keyvalue = FUHAO_KEYV;
                break;

                //按百分比的形式显示乘积结果
            case BAIFENHAO_KEYV :
                m_dCurtemp = m_dCurtemp / 100;
                //sprintf_(buffer, "%.12f", dtemp1);				
                wsprintf(buffer, _T("%16.14f"),m_dCurtemp);				
                for (i =  wcslen(buffer); i > 0; i --)
                    if (buffer[i-1] != '0')break;
                if (buffer[i-1] == '.')
                {
                    i = i - 1;
                }
                memcpy(buffer1, buffer, i*2);
                buffer1[i] = '\0';
                m_DialNumEdit.SetWindowText(buffer1);
                m_keyvalue = BAIFENHAO_KEYV;
                break;

                //除号
            case CHU_KEYV :				
                dtemp1 = m_dCurtemp;
                if ( ( m_keyvalue1 != CHU_KEYV ) || ( dtemp1 != 0 ) || ( m_keyvalue2 != m_keynull ) )
                {
                    if (m_keyvalue2 == m_keynull)
                    {
                        switch ( m_keyvalue1 )
                        {
                        case CHU_KEYV :
                            dtemp1 = m_dtemp0 / dtemp1;
                            break;
                        case CHENG_KEYV :
                            dtemp1 = m_dtemp0 * dtemp1;
                            break;
                        case JIAN_KEYV :
                            dtemp1 = m_dtemp0 - dtemp1;
                            break;
                        case JIA_KEYV :
                            dtemp1 = m_dtemp0 + dtemp1;
                            break;
                        }
                    }else if (m_keyvalue2 == DENG_KEYV)			//wwf
                    {
                        dtemp1 = m_dtemp0;
                    }
                    //2005.1.24-zmy
                    if ( dtemp1 >= 99999999999999999 )
                    {
                        m_DialNumEdit.SetWindowText(L"Error");
                        m_dCurtemp = 0;
                        m_IsError = TRUE;//2005.5.19 zmy 设置错误的标志
                        m_dtemp0 = 0;
                        m_keyvalue = m_keynull;
                        m_keyvalue1 = m_keynull;
                        m_keyvalue2 = m_keynull;
                        return;
                    }
                    //if ( dtemp1 >= 999999999 ) dtemp1 = 999999999;

                    m_dCurtemp = dtemp1;
                    wsprintf(buffer, _T("%16.14f"),dtemp1);
                    for (i =  wcslen(buffer); i > 0; i --)
                        if (buffer[i-1] != '0')break;
                    if (buffer[i-1] == '.')
                    {
                        i = i - 1;
                    }
                    memcpy(buffer1, buffer, i*2);
                    buffer1[i] = '\0';
                    m_DialNumEdit.SetWindowText(buffer1);
                }
                else if (m_keyvalue2 == m_keynull)
                {
                    //strcpy(buffer1, "除数不能为零");
                    m_DialNumEdit.SetWindowText(L"Error");
                    m_IsError = TRUE;//2005.5.19 zmy 设置错误的标志
                    m_dCurtemp = 0;
                }

                m_dtemp0 = dtemp1;
                m_dtemp1 = dtemp1;
                m_keyvalue = CHU_KEYV;
                m_keyvalue2 = CHU_KEYV;
                m_keyvalue3 = CHU_KEYV;
                break;

                //乘号
            case CHENG_KEYV :
                dtemp1 = m_dCurtemp;
                if ( ( m_keyvalue1 != CHU_KEYV ) || ( dtemp1 != 0 ) || ( m_keyvalue2 != m_keynull ) )
                {
                    if (m_keyvalue2 == m_keynull)
                    {					
                        switch ( m_keyvalue1 )
                        {
                        case CHU_KEYV :
                            dtemp1 = m_dtemp0 / dtemp1;
                            break;
                        case CHENG_KEYV :
                            dtemp1 = m_dtemp0 * dtemp1;
                            break;
                        case JIAN_KEYV :
                            dtemp1 = m_dtemp0 - dtemp1;
                            break;
                        case JIA_KEYV :
                            dtemp1 = m_dtemp0 + dtemp1;
                            break;
                        }
                    }else if (m_keyvalue2 == DENG_KEYV)			//wwf
                    {
                        dtemp1 = m_dtemp0;
                    }

                    //2005.1.24-zmy
                    if ( dtemp1 >= 99999999999999999 )
                    {
                        m_DialNumEdit.SetWindowText(L"Error");
                        m_dCurtemp = 0;
                        m_IsError = TRUE;//2005.5.19 zmy 设置错误的标志
                        m_dtemp0 = 0;
                        m_keyvalue = m_keynull;
                        m_keyvalue1 = m_keynull;
                        m_keyvalue2 = m_keynull;
                        return;
                    }
                    //if ( dtemp1 >= 999999999 ) dtemp1 = 999999999;

                    m_dCurtemp = dtemp1;
                    wsprintf(buffer, _T("%16.14f"),dtemp1);
                    for (i =  wcslen(buffer); i > 0; i --)
                        if (buffer[i-1] != '0')break;
                    if(buffer[i-1] == '.')
                    {
                        i = i - 1;
                    }
                    memcpy(buffer1, buffer, i*2);
                    buffer1[i] = '\0';
                    m_DialNumEdit.SetWindowText(buffer1);		
                }
                else if (m_keyvalue2 == m_keynull)
                {
                    //strcpy(buffer1, "除数不能为零");
                    m_DialNumEdit.SetWindowText(L"Error");		
                    m_IsError = TRUE;//2005.5.19 zmy 设置错误的标志
                    m_dCurtemp = 0;
                }

                m_dtemp0 = dtemp1;
                m_dtemp1 = dtemp1;
                m_keyvalue = CHENG_KEYV;
                m_keyvalue2 = CHENG_KEYV;
                m_keyvalue3 = CHENG_KEYV;
                break;

                //减号
            case JIAN_KEYV :
                dtemp1 = m_dCurtemp;
                if ( ( m_keyvalue1 != CHU_KEYV ) || ( dtemp1 != 0 ) || ( m_keyvalue2 != m_keynull ) )
                {
                    if (m_keyvalue2 == m_keynull)
                    {
                        switch ( m_keyvalue1 )
                        {
                        case CHU_KEYV :
                            dtemp1 = m_dtemp0 / dtemp1;
                            break;
                        case CHENG_KEYV :
                            dtemp1 = m_dtemp0 * dtemp1;
                            break;
                        case JIAN_KEYV :
                            dtemp1 = m_dtemp0 - dtemp1;
                            break;
                        case JIA_KEYV :
                            dtemp1 = m_dtemp0 + dtemp1;
                            break;
                        }
                    }else if (m_keyvalue2 == DENG_KEYV)			//wwf
                    {
                        dtemp1 = m_dtemp0;
                    }
                    //2005.1.24-zmy
                    if ( dtemp1 >= 99999999999999999 )
                    {
                        m_DialNumEdit.SetWindowText(L"Error");
                        m_dCurtemp = 0;
                        m_IsError = TRUE;//2005.5.19 zmy 设置错误的标志
                        m_dtemp0 = 0;
                        m_keyvalue = m_keynull;
                        m_keyvalue1 = m_keynull;
                        m_keyvalue2 = m_keynull;
                        return;
                    }
                    //if ( dtemp1 >= 999999999 ) dtemp1 = 999999999;

                    m_dCurtemp = dtemp1;
                    wsprintf(buffer, _T("%16.14f"),dtemp1);
                    for (i =  wcslen(buffer); i > 0; i --)
                        if (buffer[i-1] != '0')break;
                    if (buffer[i-1] == '.')
                    {
                        i = i - 1;
                    }
                    memcpy(buffer1, buffer, i*2);
                    buffer1[i] = '\0';
                    m_DialNumEdit.SetWindowText(buffer1);	
                }
                else if (m_keyvalue2 == m_keynull)
                {
                    //strcpy(buffer1, "除数不能为零");					
                    m_DialNumEdit.SetWindowText(L"Error");		
                    m_IsError = TRUE;//2005.5.19 zmy 设置错误的标志
                    m_dCurtemp = 0;
                }

                m_dtemp0 = dtemp1;
                m_dtemp1 = dtemp1;
                m_keyvalue = JIAN_KEYV;
                m_keyvalue2 = JIAN_KEYV;
                m_keyvalue3 = JIAN_KEYV;
                break;

                //加号
            case JIA_KEYV :
                dtemp1 = m_dCurtemp;
                if ( ( m_keyvalue1 != CHU_KEYV ) || ( dtemp1 != 0 ) || ( m_keyvalue2 != m_keynull ) )
                {
                    if (m_keyvalue2 == m_keynull)
                    {
                        switch ( m_keyvalue1 )
                        {
                        case CHU_KEYV :
                            dtemp1 = m_dtemp0 / dtemp1;
                            break;
                        case CHENG_KEYV :
                            dtemp1 = m_dtemp0 * dtemp1;
                            break;
                        case JIAN_KEYV :
                            dtemp1 = m_dtemp0 - dtemp1;
                            break;
                        case JIA_KEYV :
                            dtemp1 = m_dtemp0 + dtemp1;
                            break;
                        }
                    }else if (m_keyvalue2 == DENG_KEYV)			//wwf
                    {
                        dtemp1 = m_dtemp0;
                    }
                    //2005.1.24-zmy
                    if ( dtemp1 >= 99999999999999999 )
                    {
                        m_DialNumEdit.SetWindowText(L"Error");
                        m_dCurtemp = 0;
                        m_IsError = TRUE;//2005.5.19 zmy 设置错误的标志
                        m_dtemp0 = 0;
                        m_keyvalue = m_keynull;
                        m_keyvalue1 = m_keynull;
                        m_keyvalue2 = m_keynull;
                        return;
                    }
                    //if ( dtemp1 >= 999999999 ) dtemp1 = 999999999;

                    m_dCurtemp = dtemp1;
                    wsprintf(buffer, _T("%16.14f"),dtemp1);
                    for (i =  wcslen(buffer); i > 0; i --)
                        if (buffer[i-1] != '0')break;
                    if (buffer[i-1] == '.')
                    {
                        i = i - 1;
                    }
                    memcpy(buffer1, buffer, i*2);
                    buffer1[i] = '\0';
                    m_DialNumEdit.SetWindowText(buffer1);	

                }
                else if (m_keyvalue2 == m_keynull)
                {
                    //strcpy(buffer1, "除数不能为零");
                    m_DialNumEdit.SetWindowText(L"Error");		
                    m_IsError = TRUE;//2005.5.19 zmy 设置错误的标志
                    m_dCurtemp = 0;
                }

                m_dtemp0 = dtemp1;
                m_dtemp1 = dtemp1;
                m_keyvalue = JIA_KEYV;
                m_keyvalue2 = JIA_KEYV;
                m_keyvalue3 = JIA_KEYV;
                break;

                //等号
            case DENG_KEYV :
                dtemp1 = m_dCurtemp;
                if ( ( m_keyvalue1 != CHU_KEYV ) || ( dtemp1 != 0 ) || ( m_keyvalue2 != m_keynull ) )
                {
                    switch ( m_keyvalue2 )	
                    {
                        //数字与等号之间无其它运算符时
                    case m_keynull :
                        switch ( m_keyvalue1 )
                        {
                        case m_keynull :
                            dtemp1 = dtemp1;
                            break;
                        case CHU_KEYV :
                            dtemp1 = m_dtemp0 / dtemp1;
                            break;
                        case CHENG_KEYV :
                            dtemp1 = m_dtemp0 * dtemp1;
                            break;
                        case JIAN_KEYV :
                            dtemp1 = m_dtemp0 - dtemp1;
                            break;
                        case JIA_KEYV :
                            dtemp1 = m_dtemp0 + dtemp1;
                            break;
                        }				
                        break;
                        //等号前有运算符且为等时
                    case DENG_KEYV :
                        switch ( m_keyvalue3 )
                        {
                        case m_keynull :						
                            switch ( m_keyvalue1 )
                            {
                            case m_keynull :
                                dtemp1 = dtemp1;
                                break;
                            case CHU_KEYV :
                                dtemp1 = m_dtemp2;
                                dtemp1 = m_dtemp0 / dtemp1;
                                break;
                            case CHENG_KEYV :
                                dtemp1 = m_dtemp2;
                                dtemp1 = m_dtemp0 * dtemp1;
                                break;
                            case JIAN_KEYV :
                                dtemp1 = m_dtemp2;
                                dtemp1 = m_dtemp0 - dtemp1;
                                break;
                            case JIA_KEYV :
                                dtemp1 = m_dtemp2;
                                dtemp1 = m_dtemp0 + dtemp1;										
                                break;
                            }
                            break;
                        case CHU_KEYV :
                            dtemp1 = m_dtemp1;
                            dtemp1 = m_dtemp0 / dtemp1;
                            break;
                        case CHENG_KEYV :
                            dtemp1 = m_dtemp1;
                            dtemp1 = m_dtemp0 * dtemp1;
                            break;
                        case JIAN_KEYV :
                            dtemp1 = m_dtemp1;
                            dtemp1 = m_dtemp0 - dtemp1;
                            break;
                        case JIA_KEYV :
                            dtemp1 = m_dtemp1;
                            dtemp1 = m_dtemp0 + dtemp1;
                            break;		
                        }
                        break;
                        //等号前有运算符且为除时
                    case CHU_KEYV :
                        dtemp1 = m_dtemp0 / dtemp1;
                        break;
                        //等号前有运算符且为乘时
                    case CHENG_KEYV :
                        dtemp1 = m_dtemp0 * dtemp1;
                        break;
                        //等号前有运算符且为减时
                    case JIAN_KEYV :
                        dtemp1 = m_dtemp0 - dtemp1;
                        break;
                        //等号前有运算符且为加等时
                    case JIA_KEYV :
                        dtemp1 = m_dtemp0 + dtemp1;
                        break;
                    }
                    //2005.1.24-zmy
                    if ( dtemp1 >= 99999999999999999 )
                    {
                        m_DialNumEdit.SetWindowText(L"Error");
                        m_dCurtemp = 0;
                        m_IsError = TRUE;//2005.5.19 zmy 设置错误的标志
                        m_dtemp0 = 0;
                        m_keyvalue = m_keynull;
                        m_keyvalue1 = m_keynull;
                        m_keyvalue2 = m_keynull;
                        return;
                    }
                    //if ( dtemp1 >= 999999999 ) dtemp1 = 999999999;

                    m_dCurtemp = dtemp1;
                    wsprintf(buffer, _T("%16.14f"), dtemp1);
                    for (i =  wcslen(buffer); i > 0; i --)
                        if (buffer[i-1] != '0') break;
                    if (buffer[i-1] == '.')
                    {
                        i = i - 1;
                    }
                    memcpy(buffer1, buffer, i*2);
                    buffer1[i] = '\0';
                    m_DialNumEdit.SetWindowText(buffer1);								
                }
                else if (m_keyvalue2 == m_keynull)
                {
                    //strcpy(buffer1, "除数不能为零");
                    m_DialNumEdit.SetWindowText(L"Error");	
                    m_IsError = TRUE;//2005.5.19 zmy 设置错误的标志
                    m_dCurtemp = 0;
                }

                m_dtemp0 = dtemp1 ;
                m_keyvalue = DENG_KEYV;
                m_keyvalue2 = DENG_KEYV;
                break;

            }	
        }
    }

    void CCalculaterDlg::ClearData()
    {
        m_DialNumEdit.SetWindowText(L"");
        m_dtemp0 = 0;
        m_dtemp1 = 0;
        m_dtemp2 = 0;
        m_number1 = 0;
        m_IsError = FALSE;//2005.5.19 zmy 
    }
}
