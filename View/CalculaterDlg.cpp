// Calculater.cpp : implementation file
//

#include "stdafx.h"
#include "../resource.h"
#include "../Data/LanguageResource.h"
#include "../Data/SkinStyle.h"
#include "CalculaterDlg.h"
#include "../multimediaphone.h"
#include "../MultimediaPhoneDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//wsprintf(buffer, _T("%16.16f"), m_number1);

double WTOF(CString buffer) {
    char buf[32];
    memset(buf, 0, 32);
    size_t convertCount = 0;
    int n = wcstombs_s(&convertCount, buf, 32, (LPTSTR)(LPCTSTR)buffer/*.GetBuffer(30)*/, 30);
    buf[n] = '\0';
    return atof(buf);
}

//�Զ����ֵ
#define SOFTKEY1_BACK    400
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
#define BACKSPACE_KEYV   640
#define CE_KEYV          650
/////////////////////////////////////////////////////////////////////////////
// CCalculaterDlg dialog
///////////////////////2004.12.16////////////////

/////////////////////sprintf()//////////////////
void wsprintf_1(WCHAR* wstr, int num, double val) {
    char str[128]; 
    memset(str, 0, 128);

    //_gcvt(val, num, str);
    _gcvt_s(str, 128, val, num);
    int len = strlen(str);
    str[len] = '0';

    //@@
    mbstowcs(wstr, str, strlen(str) + 1);
}

CCalculaterDlg::CCalculaterDlg(CWnd* pParent /*=NULL*/)
: CDialog(CCalculaterDlg::IDD, pParent) {
    //{{AFX_DATA_INIT(CCalculaterDlg)
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT

}

void CCalculaterDlg::DoDataExchange(CDataExchange* pDX) {
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CCalculaterDlg)
    // NOTE: the ClassWizard will add DDX and DDV calls here
    //}}AFX_DATA_MAP

}


BEGIN_MESSAGE_MAP(CCalculaterDlg, CDialog)
    //{{AFX_MSG_MAP(CCalculaterDlg)
    ON_MESSAGE(WM_CLICKMJPG_TOAPP, OnClickMJPG)
    ON_WM_TIMER()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CCalculaterDlg::OnExit() {
    CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
    CString icon ;

    OnSoftKey(C_KEYV, 0);
    GetParent()->SendMessage(WM_CHANGEWINDOW, (WPARAM)this, (LPARAM)SW_HIDE);

    main->PopbackIcon();
}
/////////////////////////////////////////////////////////////////////////////
// CCalculaterDlg message handlers

BOOL CCalculaterDlg::OnInitDialog() {
    CDialog::OnInitDialog();

    // TODO: Add extra initialization here
    //2005.3.30��zmy ��ʼ��
    m_number1 = 0;
    m_dtemp0 = 0;
    m_dtemp1 = 0;
    m_dtemp2 = 0;
    m_transferType = 0;
    m_IsError = FALSE; //2005.5.19 zmy

    //  wangzhenxing1102
    CRect r(49, 30, 537, 85);
    m_NumFont.CreateFont(30,                        // nHeight
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
        _T("����"));
    m_DialNumEdit.Create(WS_CHILD | ES_RIGHT | ES_NUMBER | ES_MULTILINE, r, this, 0xFFFF);
    m_DialNumEdit.SetIsAutoInput();
    m_DialNumEdit.SetFont(&m_NumFont);
    m_DialNumEdit.HideCaret();
    m_DialNumEdit.SetLimitText(16);
    //m_DialNumEdit.ShowWindow(SW_SHOW);

    m_TransFont.CreateFont(18,                        // nHeight
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
        _T("����"));

    m_edtTransferNum1.Create(WS_CHILD | WS_VISIBLE | ES_NUMBER | ES_MULTILINE, CRect(633, 145, 749, 168), this, 0xFFFF);
    m_edtTransferNum1.SetFont(&m_TransFont);
    m_edtTransferNum1.SetLimitDiagital();
    m_edtTransferNum1.ShowWindow(SW_HIDE);

    m_edtTransferNum2.Create(WS_CHILD | WS_VISIBLE | ES_NUMBER | ES_MULTILINE, CRect(633, 202, 749, 225), this, 0xFFFF);
    m_edtTransferNum2.SetFont(&m_TransFont);
    m_edtTransferNum2.SetLimitDiagital();
    m_edtTransferNum2.ShowWindow(SW_HIDE);

    m_cmbType1.CreateEx(WS_CHILD | WS_VISIBLE, CRect(633, 117, 749, 360), this, 0xFFFF, 20, 36, 26, 1);
    m_cmbType2.CreateEx(WS_CHILD | WS_VISIBLE, CRect(633, 172, 749, 400), this, 0xFFFF, 20, 36, 26, 1);
    m_cmbType1.ShowWindow(SW_HIDE);
    m_cmbType2.ShowWindow(SW_HIDE);

    m_MJPGList.Create(L"", WS_VISIBLE | WS_CHILD, CRect(0, 0, 800, 423), this);
    m_MJPGList.SetCurrentLinkFile(".\\adv\\mjpg\\k5\\����\\������.xml");
    m_MJPGList.SetMJPGRect(CRect(0, 0, 800, 423));

    OnClickMJPG(50, 0);
    MoveWindow(0, 57, 800, 423);

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

LRESULT CCalculaterDlg::OnClickMJPG(WPARAM w, LPARAM l) {
    LRESULT result = 0;
    UINT16 keyV[] = {
        0x800, BACKSPACE_KEYV, CE_KEYV, C_KEYV,\
        MC_KEYV, '7', '8', '9', CHU_KEYV, GENHAO_KEYV,\
        MR_KEYV, '4', '5', '6', CHENG_KEYV, BAIFENHAO_KEYV,\
        MS_KEYV, '1', '2', '3', JIAN_KEYV, DAOSHU_KEYV,\
        MJ_KEYV, '0', FUHAO_KEYV, '.', JIA_KEYV, DENG_KEYV
    };
    switch (w) {
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
    case 16:
    case 17:
    case 18:
    case 19:
    case 20:
    case 21:
    case 22:
    case 23:
    case 24:
    case 25:
    case 26:
    case 27:
        //���ܺ����ּ�
        OnSoftKey(keyV[w], 0);
        break;
    case 50:
        //����
        m_MJPGList.SetUnitFont(61, font_16);
        m_MJPGList.SetUnitText(61, L"����", TRUE);
        m_transferType = 1;
        SetComboData(m_transferType);
        break;
    case 51:
        //ʱ��
        m_MJPGList.SetUnitFont(61, font_16);
        m_MJPGList.SetUnitText(61, L"ʱ��", TRUE);
        m_transferType = 2;
        SetComboData(m_transferType);
        break;
    case 52:
        //ѹ��
        m_MJPGList.SetUnitFont(61, font_16);
        m_MJPGList.SetUnitText(61, L"ѹ��", TRUE);
        m_transferType = 3;
        SetComboData(m_transferType);
        break;
    case 53:
        //�¶�
        m_MJPGList.SetUnitFont(61, font_16);
        m_MJPGList.SetUnitText(61, L"�¶�", TRUE);
        m_transferType = 4;
        SetComboData(m_transferType);
        break;
    case 54:
        //����
        m_MJPGList.SetUnitFont(61, font_16);
        m_MJPGList.SetUnitText(61, L"����", TRUE);
        m_transferType = 5;
        SetComboData(m_transferType);
        break;
    case 55:
        //���
        m_MJPGList.SetUnitFont(61, font_16);
        m_MJPGList.SetUnitText(61, L"���", TRUE);
        m_transferType = 6;
        SetComboData(m_transferType);
        break;
    case 62:
        OnButtonEquel(m_transferType);
        break;
    case 1000:
        //����
        OnExit();
        break;
    default:
        break;
    }
    return result;
}

//����̵���Ӧ����
LRESULT CCalculaterDlg::OnSoftKey(WPARAM w, LPARAM l) {
    LRESULT result = 0;
    if (w == 0x800)   //�˳�
    {
        OnExit();
        return result;
    }

    if (m_IsError && w != C_KEYV) {
        return result;
    }       //2005.5.19 zmy ���󣬵���κμ����޷�Ӧ        

    //���ּ�������
    if (w < m_keynull) {
        if (m_keyvalue < C_KEYV) {
            WCHAR t_buffer[20];
            int len = m_DialNumEdit.GetWindowText(t_buffer, 20);
            if ((len == 0) && (w == '.')) {
                return result;
            }
            if ((len == 1) && (t_buffer[0] == '0') && (w == '0')) {
                return result;
            }
            for (int i = wcslen(t_buffer); i > 0; i --) {
                if ((t_buffer[i - 1] == '.') && (w == '.')) {
                    return result;
                }
            }//С����ֻ�ܵ��һ��

            if (len < 9) {
                CString s;
                m_DialNumEdit.GetWindowText(s);
                if (s == "0") {
                    s == "";
                }
                s += (char)w;
                m_DialNumEdit.SetWindowText(s);

                m_dCurtemp = WTOF(s);
            }
        } else {
            m_DialNumEdit.SetWindowText(L"");
            if (w == '.') {
                return result;
            }
            CString s;
            m_DialNumEdit.GetWindowText(s);
            if (s == "0") {
                s == "";
            }
            s += (char)w;
            m_DialNumEdit.SetWindowText(s);
            m_keyvalue1 = m_keyvalue;
            m_keyvalue = m_keynull;
            m_keyvalue2 = m_keynull;
            m_keyvalue3 = m_keynull;
            m_dCurtemp = WTOF(s);
        }
        if (m_keyvalue1 >= CHU_KEYV) {
            WCHAR buffer[20];
            memset(buffer, 0, 40);
            m_DialNumEdit.GetWindowText(buffer, 20);
            m_dtemp2 = WTOF(buffer);

            m_dCurtemp = m_dtemp2;      //wwf
        }
    }

    //���ż��������Ӧ����
    else {
        //2005.3.30-zmy �������Ϊ��ʱ��������ż�������MS�⣩������ʾ
        WCHAR buff[20];
        int len = m_DialNumEdit.GetWindowText(buff, 20);
        if ((len == 0) && (w != MR_KEYV)) {
            return result;
        }
        WCHAR buffer[20];
        WCHAR buffer1[20];
        int i;
        int j;
        double temp1 = 0;
        double temp2 = 0;
        double dtemp1;

        switch (w) {
        case BACKSPACE_KEYV:
            //delete
            {
                CString s;
                m_DialNumEdit.GetWindowText(s);
                int len = s.GetLength();
                if (len > 0) {
                    s = s.Left(len - 1);
                    m_DialNumEdit.SetWindowText(s);

                    m_dCurtemp = WTOF(s);       //wwf
                }
            }

            break;
            //�������
        case C_KEYV:
            m_DialNumEdit.SetWindowText(L"");
            m_dtemp0 = 0;
            m_keyvalue = m_keynull;
            m_keyvalue1 = m_keynull;
            m_IsError = FALSE;  //2005.5.19 zmy ���ִ���ʱ��ֻ�ܵ�����������������Ч
            if (m_keyvalue2 == CHU_KEYV || m_keyvalue2 == CHENG_KEYV || m_keyvalue2 == JIAN_KEYV \
                || m_keyvalue2 == JIA_KEYV || m_keyvalue2 == DENG_KEYV) {
                m_keyvalue2 = m_keynull;
            }
            m_dCurtemp = 0;                 //wwf

            break;

        case CE_KEYV :
            //ClearData();
            m_DialNumEdit.SetWindowText(L"");
            m_dCurtemp = 0;                 //wwf
            break;

            //��ǰ���ֺʹ洢���е�������Ӳ��浽�洢����
        case MJ_KEYV :
            m_number1 = m_number1 + m_dCurtemp;
            m_keyvalue = MJ_KEYV;
            break;

            //��ǰ���ִ洢���洢���в��滻��ԭ���洢���е�ֵ
        case MS_KEYV :
            m_number1 = m_dCurtemp;
            m_keyvalue = MS_KEYV;
            break;

            //���ô洢���е���ֵ
        case MR_KEYV :
            m_DialNumEdit.SetWindowText(L"");
            wsprintf(buffer, _T("%16.14f"), m_number1);
            for (i = wcslen(buffer); i > 0; i --) {
                if (buffer[i - 1] != '0') {
                    break;
                }
            }
            if (buffer[i - 1] == '.') {
                i = i - 1;
            }
            memcpy(buffer1, buffer, i * 2);
            buffer1[i] = '\0';
            m_DialNumEdit.SetWindowText(buffer1);

            m_dCurtemp = WTOF(buffer1);             //wwf

            m_keyvalue = MR_KEYV;
            break;

            //�Ѵ洢���е���ֵ�������Ϊ��
        case MC_KEYV :
            m_number1 = 0;
            m_keyvalue = MC_KEYV;
            break;

            //��ǰ����ȡ����ֵ
        case GENHAO_KEYV :
            if (m_dCurtemp >= 0) {
                m_dCurtemp = sqrt(m_dCurtemp) ;     //lxz 2004.9.5          
                wsprintf(buffer, _T("%16.14f"), m_dCurtemp);                
                for (i = wcslen(buffer); i > 0; i --) {
                    if (buffer[i - 1] != '0') {
                        break;
                    }
                }
                if (buffer[i - 1] == '.') {
                    i = i - 1;
                }
                memcpy(buffer1, buffer, i * 2);
                buffer1[i] = '\0';
                m_DialNumEdit.SetWindowText(buffer1);
            } else {
                //m_DialNumEdit.SetWindowText("����������Ч!");
                m_DialNumEdit.SetWindowText(L"Error");
                m_IsError = TRUE;//2005.5.19 zmy ���ô���ı�־
                m_dCurtemp = 0;
            }
            m_keyvalue = GENHAO_KEYV;
            break;

            //��ǰ����ȡ����ֵ
        case DAOSHU_KEYV :
            if (m_dCurtemp != 0) {
                m_dCurtemp = 1.0 / m_dCurtemp;
                wsprintf(buffer, _T("%16.14f"), m_dCurtemp);
                for (i = wcslen(buffer); i > 0; i --) {
                    if (buffer[i - 1] != '0') {
                        break;
                    }
                }
                if (buffer[i - 1] == '.') {
                    i = i - 1;
                }
                memcpy(buffer1, buffer, i * 2);
                buffer1[i] = '\0';
                m_DialNumEdit.SetWindowText(buffer1);
            } else {
                m_DialNumEdit.SetWindowText(L"Error");
                m_IsError = TRUE;//2005.5.19 zmy ���ô���ı�־
            }

            m_keyvalue = DAOSHU_KEYV;
            break;

            //��ǰ����ȡ����
        case FUHAO_KEYV :
            m_dCurtemp = -m_dCurtemp;
            wsprintf(buffer, _T("%16.14f"), m_dCurtemp);
            for (i = wcslen(buffer); i > 0; i --) {
                if (buffer[i - 1] != '0') {
                    break;
                }
            }
            if (buffer[i - 1] == '.') {
                i = i - 1;
            }
            memcpy(buffer1, buffer, i * 2);
            buffer1[i] = '\0';  
            m_DialNumEdit.SetWindowText(buffer1);

            if (m_keyvalue != DAOSHU_KEYV && m_keyvalue != GENHAO_KEYV) {
                j = wcslen(buffer1);
                m_keyvalue = m_keynull;
            } else {
                m_keyvalue = FUHAO_KEYV;
            }
            break;

            //���ٷֱȵ���ʽ��ʾ�˻����
        case BAIFENHAO_KEYV :
            m_dCurtemp = m_dCurtemp / 100;
            //sprintf_(buffer, "%.12f", dtemp1);                
            wsprintf(buffer, _T("%16.14f"), m_dCurtemp);                
            for (i = wcslen(buffer); i > 0; i --) {
                if (buffer[i - 1] != '0') {
                    break;
                }
            }
            if (buffer[i - 1] == '.') {
                i = i - 1;
            }
            memcpy(buffer1, buffer, i * 2);
            buffer1[i] = '\0';
            m_DialNumEdit.SetWindowText(buffer1);
            m_keyvalue = BAIFENHAO_KEYV;
            break;

            //����
        case CHU_KEYV :
            dtemp1 = m_dCurtemp;
            if ((m_keyvalue1 != CHU_KEYV) || (dtemp1 != 0) || (m_keyvalue2 != m_keynull)) {
                if (m_keyvalue2 == m_keynull) {
                    switch (m_keyvalue1) {
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
                } else if (m_keyvalue2 == DENG_KEYV)            //wwf
                {
                    dtemp1 = m_dtemp0;
                }
                //2005.1.24-zmy
                if (dtemp1 >= 99999999999999999) {
                    m_DialNumEdit.SetWindowText(L"Error");
                    m_dCurtemp = 0;
                    m_IsError = TRUE;//2005.5.19 zmy ���ô���ı�־
                    m_dtemp0 = 0;
                    m_keyvalue = m_keynull;
                    m_keyvalue1 = m_keynull;
                    m_keyvalue2 = m_keynull;
                    return result;
                }
                //if ( dtemp1 >= 999999999 ) dtemp1 = 999999999;

                m_dCurtemp = dtemp1;
                wsprintf(buffer, _T("%16.14f"), dtemp1);
                for (i = wcslen(buffer); i > 0; i --) {
                    if (buffer[i - 1] != '0') {
                        break;
                    }
                }
                if (buffer[i - 1] == '.') {
                    i = i - 1;
                }
                memcpy(buffer1, buffer, i * 2);
                buffer1[i] = '\0';
                m_DialNumEdit.SetWindowText(buffer1);
            } else if (m_keyvalue2 == m_keynull) {
                //strcpy(buffer1, "��������Ϊ��");
                m_DialNumEdit.SetWindowText(L"Error");
                m_IsError = TRUE;//2005.5.19 zmy ���ô���ı�־
                m_dCurtemp = 0;
            }

            m_dtemp0 = dtemp1;
            m_dtemp1 = dtemp1;
            m_keyvalue = CHU_KEYV;
            m_keyvalue2 = CHU_KEYV;
            m_keyvalue3 = CHU_KEYV;
            break;

            //�˺�
        case CHENG_KEYV :
            dtemp1 = m_dCurtemp;
            if ((m_keyvalue1 != CHU_KEYV) || (dtemp1 != 0) || (m_keyvalue2 != m_keynull)) {
                if (m_keyvalue2 == m_keynull) {
                    switch (m_keyvalue1) {
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
                } else if (m_keyvalue2 == DENG_KEYV)            //wwf
                {
                    dtemp1 = m_dtemp0;
                }

                //2005.1.24-zmy
                if (dtemp1 >= 99999999999999999) {
                    m_DialNumEdit.SetWindowText(L"Error");
                    m_dCurtemp = 0;
                    m_IsError = TRUE;//2005.5.19 zmy ���ô���ı�־
                    m_dtemp0 = 0;
                    m_keyvalue = m_keynull;
                    m_keyvalue1 = m_keynull;
                    m_keyvalue2 = m_keynull;
                    return result;
                }
                //if ( dtemp1 >= 999999999 ) dtemp1 = 999999999;

                m_dCurtemp = dtemp1;
                wsprintf(buffer, _T("%16.14f"), dtemp1);
                for (i = wcslen(buffer); i > 0; i --) {
                    if (buffer[i - 1] != '0') {
                        break;
                    }
                }
                if (buffer[i - 1] == '.') {
                    i = i - 1;
                }
                memcpy(buffer1, buffer, i * 2);
                buffer1[i] = '\0';
                m_DialNumEdit.SetWindowText(buffer1);
            } else if (m_keyvalue2 == m_keynull) {
                //strcpy(buffer1, "��������Ϊ��");
                m_DialNumEdit.SetWindowText(L"Error");      
                m_IsError = TRUE;//2005.5.19 zmy ���ô���ı�־
                m_dCurtemp = 0;
            }

            m_dtemp0 = dtemp1;
            m_dtemp1 = dtemp1;
            m_keyvalue = CHENG_KEYV;
            m_keyvalue2 = CHENG_KEYV;
            m_keyvalue3 = CHENG_KEYV;
            break;

            //����
        case JIAN_KEYV :
            dtemp1 = m_dCurtemp;
            if ((m_keyvalue1 != CHU_KEYV) || (dtemp1 != 0) || (m_keyvalue2 != m_keynull)) {
                if (m_keyvalue2 == m_keynull) {
                    switch (m_keyvalue1) {
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
                } else if (m_keyvalue2 == DENG_KEYV)            //wwf
                {
                    dtemp1 = m_dtemp0;
                }
                //2005.1.24-zmy
                if (dtemp1 >= 99999999999999999) {
                    m_DialNumEdit.SetWindowText(L"Error");
                    m_dCurtemp = 0;
                    m_IsError = TRUE;//2005.5.19 zmy ���ô���ı�־
                    m_dtemp0 = 0;
                    m_keyvalue = m_keynull;
                    m_keyvalue1 = m_keynull;
                    m_keyvalue2 = m_keynull;
                    return result;
                }
                //if ( dtemp1 >= 999999999 ) dtemp1 = 999999999;

                m_dCurtemp = dtemp1;
                wsprintf(buffer, _T("%16.14f"), dtemp1);
                for (i = wcslen(buffer); i > 0; i --) {
                    if (buffer[i - 1] != '0') {
                        break;
                    }
                }
                if (buffer[i - 1] == '.') {
                    i = i - 1;
                }
                memcpy(buffer1, buffer, i * 2);
                buffer1[i] = '\0';
                m_DialNumEdit.SetWindowText(buffer1);
            } else if (m_keyvalue2 == m_keynull) {
                //strcpy(buffer1, "��������Ϊ��");                  
                m_DialNumEdit.SetWindowText(L"Error");      
                m_IsError = TRUE;   //2005.5.19 zmy ���ô���ı�־
                m_dCurtemp = 0;
            }

            m_dtemp0 = dtemp1;
            m_dtemp1 = dtemp1;
            m_keyvalue = JIAN_KEYV;
            m_keyvalue2 = JIAN_KEYV;
            m_keyvalue3 = JIAN_KEYV;
            break;

            //�Ӻ�
        case JIA_KEYV :
            dtemp1 = m_dCurtemp;
            if ((m_keyvalue1 != CHU_KEYV) || (dtemp1 != 0) || (m_keyvalue2 != m_keynull)) {
                if (m_keyvalue2 == m_keynull) {
                    switch (m_keyvalue1) {
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
                } else if (m_keyvalue2 == DENG_KEYV)            //wwf
                {
                    dtemp1 = m_dtemp0;
                }
                //2005.1.24-zmy
                if (dtemp1 >= 99999999999999999) {
                    m_DialNumEdit.SetWindowText(L"Error");
                    m_dCurtemp = 0;
                    m_IsError = TRUE;//2005.5.19 zmy ���ô���ı�־
                    m_dtemp0 = 0;
                    m_keyvalue = m_keynull;
                    m_keyvalue1 = m_keynull;
                    m_keyvalue2 = m_keynull;
                    return result;
                }
                //if ( dtemp1 >= 999999999 ) dtemp1 = 999999999;

                m_dCurtemp = dtemp1;
                wsprintf(buffer, _T("%16.14f"), dtemp1);
                for (i = wcslen(buffer); i > 0; i --) {
                    if (buffer[i - 1] != '0') {
                        break;
                    }
                }
                if (buffer[i - 1] == '.') {
                    i = i - 1;
                }
                memcpy(buffer1, buffer, i * 2);
                buffer1[i] = '\0';
                m_DialNumEdit.SetWindowText(buffer1);
            } else if (m_keyvalue2 == m_keynull) {
                //strcpy(buffer1, "��������Ϊ��");
                m_DialNumEdit.SetWindowText(L"Error");      
                m_IsError = TRUE;//2005.5.19 zmy ���ô���ı�־
                m_dCurtemp = 0;
            }

            m_dtemp0 = dtemp1;
            m_dtemp1 = dtemp1;
            m_keyvalue = JIA_KEYV;
            m_keyvalue2 = JIA_KEYV;
            m_keyvalue3 = JIA_KEYV;
            break;

            //�Ⱥ�
        case DENG_KEYV :
            dtemp1 = m_dCurtemp;
            if ((m_keyvalue1 != CHU_KEYV) || (dtemp1 != 0) || (m_keyvalue2 != m_keynull)) {
                switch (m_keyvalue2) {
                    //������Ⱥ�֮�������������ʱ
                case m_keynull :
                    switch (m_keyvalue1) {
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
                    //�Ⱥ�ǰ���������Ϊ��ʱ
                case DENG_KEYV :
                    switch (m_keyvalue3) {
                    case m_keynull :
                        switch (m_keyvalue1) {
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
                    //�Ⱥ�ǰ���������Ϊ��ʱ
                case CHU_KEYV :
                    dtemp1 = m_dtemp0 / dtemp1;
                    break;
                    //�Ⱥ�ǰ���������Ϊ��ʱ
                case CHENG_KEYV :
                    dtemp1 = m_dtemp0 * dtemp1;
                    break;
                    //�Ⱥ�ǰ���������Ϊ��ʱ
                case JIAN_KEYV :
                    dtemp1 = m_dtemp0 - dtemp1;
                    break;
                    //�Ⱥ�ǰ���������Ϊ�ӵ�ʱ
                case JIA_KEYV :
                    dtemp1 = m_dtemp0 + dtemp1;
                    break;
                }
                //2005.1.24-zmy
                if (dtemp1 >= 99999999999999999) {
                    m_DialNumEdit.SetWindowText(L"Error");
                    m_dCurtemp = 0;
                    m_IsError = TRUE;//2005.5.19 zmy ���ô���ı�־
                    m_dtemp0 = 0;
                    m_keyvalue = m_keynull;
                    m_keyvalue1 = m_keynull;
                    m_keyvalue2 = m_keynull;
                    return result;
                }
                //if ( dtemp1 >= 999999999 ) dtemp1 = 999999999;

                m_dCurtemp = dtemp1;
                wsprintf(buffer, _T("%16.14f"), dtemp1);
                for (i = wcslen(buffer); i > 0; i --) {
                    if (buffer[i - 1] != '0') {
                        break;
                    }
                }
                if (buffer[i - 1] == '.') {
                    i = i - 1;
                }
                memcpy(buffer1, buffer, i * 2);
                buffer1[i] = '\0';
                m_DialNumEdit.SetWindowText(buffer1);
            } else if (m_keyvalue2 == m_keynull) {
                //strcpy(buffer1, "��������Ϊ��");
                m_DialNumEdit.SetWindowText(L"Error");  
                m_IsError = TRUE;//2005.5.19 zmy ���ô���ı�־
                m_dCurtemp = 0;
            }

            m_dtemp0 = dtemp1 ;
            m_keyvalue = DENG_KEYV;
            m_keyvalue2 = DENG_KEYV;
            break;
        }
    }
    return result;
}

void CCalculaterDlg::ClearData() {
    m_DialNumEdit.SetWindowText(L"");
    m_dtemp0 = 0;
    m_dtemp1 = 0;
    m_dtemp2 = 0;
    m_number1 = 0;
    m_IsError = FALSE;//2005.5.19 zmy 
}

void CCalculaterDlg::ShowWindow_(int nCmdShow) {
    m_DialNumEdit.SetWindowText(L"");
    OnClickMJPG(50, 0);
    SetTimer(1, 100, NULL);
    ShowWindow(nCmdShow);
}

void CCalculaterDlg::SetComboData(int type) {
    m_edtTransferNum1.SetWindowText(L"");
    m_edtTransferNum2.SetWindowText(L"");
    m_cmbType1.SetWindowText(L"");
    m_cmbType2.SetWindowText(L"");
    switch (type) {
    case 1:
        //����
        m_cmbType1.ResetContent();
        m_cmbType1.AddString(L"��(W)");
        m_cmbType1.AddString(L"ǧ��(kW)");
        m_cmbType1.AddString(L"����/��(J/s)");
        m_cmbType1.AddString(L"Ӣ������(HP)");
        m_cmbType1.AddString(L"��������(PS)");
        m_cmbType1.SetCurSel(0);

        m_cmbType2.ResetContent();
        m_cmbType2.AddString(L"��(W)");
        m_cmbType2.AddString(L"ǧ��(kW)");
        m_cmbType2.AddString(L"����/��(J/s)");
        m_cmbType2.AddString(L"Ӣ������(HP)");
        m_cmbType2.AddString(L"��������(PS)");
        m_cmbType2.SetCurSel(0);
        break;
    case 2:
        //ʱ��
        m_cmbType1.ResetContent();
        m_cmbType1.AddString(L"��(second)");
        m_cmbType1.AddString(L"����(minsecond)");
        m_cmbType1.AddString(L"����(minute)");
        m_cmbType1.AddString(L"Сʱ(hour)");
        m_cmbType1.AddString(L"��(day)");
        m_cmbType1.SetCurSel(0);

        m_cmbType2.ResetContent();
        m_cmbType2.AddString(L"��(second)");
        m_cmbType2.AddString(L"����(minsecond)");
        m_cmbType2.AddString(L"����(minute)");
        m_cmbType2.AddString(L"Сʱ(hour)");
        m_cmbType2.AddString(L"��(day)");
        m_cmbType2.SetCurSel(0);
        break;
    case 3:
        //ѹ��
        m_cmbType1.ResetContent();
        m_cmbType1.AddString(L"��(Pa)");
        m_cmbType1.AddString(L"ǧ��(kPa)");
        m_cmbType1.AddString(L"��׼����ѹ(atm)");
        m_cmbType1.AddString(L"���׹���(��)");
        m_cmbType1.AddString(L"����ˮ��");
        m_cmbType1.SetCurSel(0);

        m_cmbType2.ResetContent();
        m_cmbType2.AddString(L"��(Pa)");
        m_cmbType2.AddString(L"ǧ��(kPa)");
        m_cmbType2.AddString(L"��׼����ѹ(atm)");
        m_cmbType2.AddString(L"���׹���(��)");
        m_cmbType2.AddString(L"����ˮ��");
        m_cmbType2.SetCurSel(0);
        break;
    case 4:
        //�¶�
        m_cmbType1.ResetContent();
        m_cmbType1.AddString(L"���϶�(C)");
        m_cmbType1.AddString(L"���϶�(F)");
        m_cmbType1.SetCurSel(0);

        m_cmbType2.ResetContent();
        m_cmbType2.AddString(L"���϶�(C)");
        m_cmbType2.AddString(L"���϶�(F)");
        m_cmbType2.SetCurSel(0);
        break;
    case 5:
        //����
        m_cmbType1.ResetContent();
        m_cmbType1.AddString(L"��/��");
        m_cmbType1.AddString(L"ǧ��/Сʱ");
        m_cmbType1.AddString(L"Ӣ��/��");
        m_cmbType1.AddString(L"Ӣ��/Сʱ");
        m_cmbType1.SetCurSel(0);

        m_cmbType2.ResetContent();
        m_cmbType2.AddString(L"��/��");
        m_cmbType2.AddString(L"ǧ��/Сʱ");
        m_cmbType2.AddString(L"Ӣ��/��");
        m_cmbType2.AddString(L"Ӣ��/Сʱ");
        m_cmbType2.SetCurSel(0);
        break;
    case 6:
        //���
        m_cmbType1.ResetContent();
        m_cmbType1.AddString(L"��������");
        m_cmbType1.AddString(L"������");
        m_cmbType1.AddString(L"��");
        m_cmbType1.AddString(L"����Ӣ��");
        m_cmbType1.AddString(L"����Ӣ��");
        m_cmbType1.AddString(L"������");
        m_cmbType1.AddString(L"����");
        m_cmbType1.SetCurSel(0);

        m_cmbType2.ResetContent();
        m_cmbType2.AddString(L"��������");
        m_cmbType2.AddString(L"������");
        m_cmbType2.AddString(L"��");
        m_cmbType2.AddString(L"����Ӣ��");
        m_cmbType2.AddString(L"����Ӣ��");
        m_cmbType2.AddString(L"������");
        m_cmbType2.AddString(L"����");
        m_cmbType2.SetCurSel(0);
        break;
    default:
        break;
    }
}

void CCalculaterDlg::OnButtonEquel(int type) {
    CString strUnit1 = L"";
    CString strUnit2 = L"";
    m_edtTransferNum1.GetWindowText(strUnit1);
    m_edtTransferNum2.GetWindowText(strUnit2);
    int sel1 = m_cmbType1.GetCurSel();
    int sel2 = m_cmbType2.GetCurSel();
    double value1;
    double value2;
    double times1;
    double times2;
    if (L"" == strUnit1 && L"" == strUnit2) {
        m_edtTransferNum1.SetWindowText(L"");
        m_edtTransferNum2.SetWindowText(L"");
        return;
    } else if (strUnit1 != L"") {
        value1 = atof((Util::StringOp::FromCString(strUnit1)).c_str());
    } else if (L"" == strUnit1 && strUnit2 != L"") {
        value1 = atof((Util::StringOp::FromCString(strUnit2)).c_str());
    }

    switch (type) {
    case 1:
        if (0 == sel1 || 2 == sel1) {
            times1 = 1;
        } else if (1 == sel1) {
            times1 = 1000;
        } else if (3 == sel1) {
            times1 = 745.712172;
        } else if (4 == sel1) {
            times1 = 735.2941;
        }

        if (0 == sel2 || 2 == sel2) {
            times2 = 1;
        } else if (1 == sel2) {
            times2 = 1000;
        } else if (3 == sel2) {
            times2 = 745.712172;
        } else if (4 == sel2) {
            times2 = 735.2941;
        }

        if (strUnit1 != L"") {
            value2 = value1 * times1 / times2;
            m_edtTransferNum2.SetWindowText(Util::StringOp::ToCString(Util::StringOp::FromDouble(value2)));
        } else {
              value2 = value1 * times2 / times1;
              m_edtTransferNum1.SetWindowText(Util::StringOp::ToCString(Util::StringOp::FromDouble(value2)));
        }
        break;
    case 2:
        if (0 == sel1) {
            times1 = 1;
        } else if (1 == sel1) {
            times1 = 1 / 1000;
        } else if (2 == sel1) {
            times1 = 60;
        } else if (3 == sel1) {
            times1 = 3600;
        } else if (4 == sel1) {
            times1 = 3600 * 24;
        }

        if (0 == sel2) {
            times2 = 1;
        } else if (1 == sel2) {
            times2 = 1 / 1000;
        } else if (2 == sel2) {
            times2 = 60;
        } else if (3 == sel2) {
            times2 = 3600;
        } else if (4 == sel2) {
            times2 = 3600 * 24;
        }

        if (strUnit1 != L"") {
            value2 = value1 * times1 / times2;
            m_edtTransferNum2.SetWindowText(Util::StringOp::ToCString(Util::StringOp::FromDouble(value2)));
        } else {
              value2 = value1 * times2 / times1;
              m_edtTransferNum1.SetWindowText(Util::StringOp::ToCString(Util::StringOp::FromDouble(value2)));
        }
        break;
    case 3:
        if (0 == sel1) {
            times1 = 1;
        } else if (1 == sel1) {
            times1 = 1000;
        } else if (2 == sel1) {
            times1 = 101325;
        } else if (3 == sel1) {
            times1 = 133.32237;
        } else if (4 == sel1) {
            times1 = 9.80665;
        }

        if (0 == sel2) {
            times2 = 1;
        } else if (1 == sel2) {
            times2 = 1000;
        } else if (2 == sel2) {
            times2 = 101325;
        } else if (3 == sel2) {
            times2 = 133.32237;
        } else if (4 == sel2) {
            times2 = 9.80665;
        }

        if (strUnit1 != L"") {
            value2 = value1 * times1 / times2;
            m_edtTransferNum2.SetWindowText(Util::StringOp::ToCString(Util::StringOp::FromDouble(value2)));
        } else {
              value2 = value1 * times2 / times1;
              m_edtTransferNum1.SetWindowText(Util::StringOp::ToCString(Util::StringOp::FromDouble(value2)));
        }
        break;
    case 4:
        if (0 == sel1) {
            if (0 == sel2) {
                value2 = value1;
                if (strUnit1 != L"") {
                    m_edtTransferNum2.SetWindowText(Util::StringOp::ToCString(Util::StringOp::FromDouble(value2)));
                } else {
                      m_edtTransferNum1.SetWindowText(Util::StringOp::ToCString(Util::StringOp::FromDouble(value2)));
                }
            } else if (1 == sel2) {
                if (strUnit1 != L"") {
                    value2 = 33.8 + (value1 - 1) * 1.8;
                    m_edtTransferNum2.SetWindowText(Util::StringOp::ToCString(Util::StringOp::FromDouble(value2)));
                } else {
                      value2 = (value1 - 33.8) / 1.8 + 1;
                      m_edtTransferNum1.SetWindowText(Util::StringOp::ToCString(Util::StringOp::FromDouble(value2)));
                }
            }
        } else if (1 == sel1) {
            if (1 == sel2) {
                value2 = value1;
                if (strUnit1 != L"") {
                    m_edtTransferNum2.SetWindowText(Util::StringOp::ToCString(Util::StringOp::FromDouble(value2)));
                } else {
                      m_edtTransferNum1.SetWindowText(Util::StringOp::ToCString(Util::StringOp::FromDouble(value2)));
                }
            } else if (0 == sel2) {
                if (strUnit1 != L"") {
                    value2 = (value1 - 33.8) / 1.8 + 1;
                    m_edtTransferNum2.SetWindowText(Util::StringOp::ToCString(Util::StringOp::FromDouble(value2)));
                } else {
                      value2 = 33.8 + (value1 - 1) * 1.8;
                      m_edtTransferNum1.SetWindowText(Util::StringOp::ToCString(Util::StringOp::FromDouble(value2)));
                }
            }
        }
        break;
    case 5:
        if (0 == sel1) {
            times1 = 3.6;
        } else if (1 == sel1) {
            times1 = 1;
        } else if (2 == sel1) {
            times1 = 1.09728;
        } else if (3 == sel1) {
            times1 = 1.60934;
        }

        if (0 == sel2) {
            times2 = 3.6;
        } else if (1 == sel2) {
            times2 = 1;
        } else if (2 == sel2) {
            times2 = 1.09728;
        } else if (3 == sel2) {
            times2 = 1.60934;
        }

        if (strUnit1 != L"") {
            value2 = value1 * times1 / times2;
            m_edtTransferNum2.SetWindowText(Util::StringOp::ToCString(Util::StringOp::FromDouble(value2)));
        } else {
              value2 = value1 * times2 / times1;
              m_edtTransferNum1.SetWindowText(Util::StringOp::ToCString(Util::StringOp::FromDouble(value2)));
        }
        break;
    case 6:
        if (0 == sel1) {
            times1 = 1;
        } else if (1 == sel1) {
            times1 = 1000000;
        } else if (2 == sel1) {
            times1 = 1000;
        } else if (3 == sel1) {
            times1 = 16.387;
        } else if (4 == sel1) {
            times1 = 28316.846592;
        } else if (5 == sel1) {
            times1 = 764554.857984;
        } else if (6 == sel1) {
            times1 = 3785.411784;
        }

        if (0 == sel2) {
            times2 = 1;
        } else if (1 == sel2) {
            times2 = 1000000;
        } else if (2 == sel2) {
            times2 = 1000;
        } else if (3 == sel2) {
            times2 = 16.387;
        } else if (4 == sel2) {
            times2 = 28316.846592;
        } else if (5 == sel2) {
            times2 = 764554.857984;
        } else if (6 == sel2) {
            times2 = 3785.411784;
        }

        if (strUnit1 != L"") {
            value2 = value1 * times1 / times2;
            m_edtTransferNum2.SetWindowText(Util::StringOp::ToCString(Util::StringOp::FromDouble(value2)));
        } else {
              value2 = value1 * times2 / times1;
              m_edtTransferNum1.SetWindowText(Util::StringOp::ToCString(Util::StringOp::FromDouble(value2)));
        }
        break;
    default:
        break;
    }
} 

void CCalculaterDlg::OnTimer(UINT nIDEvent) {
    if (1 == nIDEvent) {
        ShowCtrl();
    }
}

void CCalculaterDlg::ShowCtrl() {
    m_DialNumEdit.ShowWindow(SW_SHOW);
    m_edtTransferNum1.ShowWindow(SW_SHOW);
    m_edtTransferNum2.ShowWindow(SW_SHOW);
    m_cmbType1.ShowWindow(SW_SHOW);
    m_cmbType2.ShowWindow(SW_SHOW);
}