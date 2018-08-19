// SettingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "../MultimediaPhone.h"
#include "SettingDlg.h"
#include "../Data/LanguageResource.h"
#include "../Data/SkinStyle.h"
#include "FireWallDlg.h"
#include "../MultimediaPhoneDlg.h"
#include "../Pblmember.h"
#include "TestDlg.h"
#include "sip.h"

//#include "Calibrui.h"

#define new DEBUG_NEW
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
/////////////////////////////////////////////////////////////////////////////
// CSettingDlg dialog
CString s_VerSionTitle = "7.37090731";
CString s_VerSion = "\
6.03090325\
1.修改了录音密码.\
6.00090319\
1.改为录音程序\
2.50090302\
1.关闭SIM卡功能\
2.将播放录音的音量大小改为系统设置中的音量.\
2.42090226\
1.在maindlg加入watchdog\
2.加入内存不够启动watchdog\
2.39090221\
1.加入了内存监视文件\\NandFlash\\Resource\\log.txt\
2.38090220\
1.修改了名片振铃，不管系统设置是否设上特色振铃都特色振铃的bug。\
2.修改了多个闹铃，一个闹铃响过后，看了还是提示未看的bug。\
3.增加了不停的写名片数据库的测试.\
2.37090220\
1.将屏保中的时间显示cstatic去掉，直接在view上显示。有内存泄露.\
2.36090219\
1.修改了CString getbuffer()的内存泄露.\
2.修改了mp3与来电暂停，再退出，再进入mp3，mp3,乱的bug.\
2.35090218\
1.修改了cprocessbar, mjpgstatic 的releasedc内存泄露\
2.34090217\
1.修改了Cwnd::GetDC的内存泄露\
2.33090217\
1.修改了只来CALL ID不振铃，而响了特色铃bug.\
2.28090117\
1.春节前发布版本\r\n\
2.7.091211\r\n\
1.武汉烟草演示版本\r\n\
2.7.081210\r\n\
1.修改了便笺飞笔问题\r\n\
2.6.081209\r\n\
1.增加了ADSL功能\r\n\
2.5.081209\r\n\
1.资源做了调整\r\n\
2.桌面的状态做了调整\r\n\
2.4.081206\r\n\
1.修改了切换到主界面慢\
2.0.081204\r\n\
1.发布给武汉烟草试用版本\r\n\
1.2.081126\r\n\
\t1.修改了在听音乐时，不停拨打电话，电话死机的bug\r\n\
1.1.081028\r\n\
\t1.修改了电话有分机改为直拨，显示出局号的bug\r\n\
\t2.修改了分机拨号中，拨内线号码，加上拨出局号的bug,目前判断的条件是分机号长度小于等于4，且第一个号码不为1，否则当外线拨号\r\n\
\t3.修改了不能自动答录功能\r\n\
\t4.不包含上网功能\r\n\
0.9.080531.0\r\n\
\t1.武汉烟草定制版本\r\n\
\t2.增加集团文化介绍，用MJPG实现\r\n\
\t3.增加上网功能\r\n\
0.8.071225.3\r\n\
\t1.添加股票功能.测试用户名:2945011密码:123456\r\n\
\t2.将便笺功能从下面的状态栏变成了股票.\r\n\
\t3.便笺功能这个版本只能从通话记录中进入.\r\n\
0.8.071220.1\r\n\
\t未完成功能及版本缺陷：\r\n\
\t1.无手写输入\r\n\
\t2.无自动答录提示语和提示音\r\n\
\t3.日期时钟不正确\r\n\
\t4.手写校正未保存\r\n\
\t5.手写笔迹不圆滑\r\n";

extern BOOL DetectDIR(TCHAR* sDir);
CSettingDlg::CSettingDlg(CWnd* pParent /*=NULL*/)
: CDialog(CSettingDlg::IDD, pParent) {
    //{{AFX_DATA_INIT(CSettingDlg)
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
    m_bLogin = FALSE;
    m_clickType = 0;
    m_iDeleteType = 0;
    m_systemVolume = 0;
    m_editFocusIndex = -1;
    memset(m_ringDir, 0, 2 * 128);
    memcpy(m_ringDir, my_ring_dir, wcslen(my_ring_dir) * 2);
}

void CSettingDlg::DoDataExchange(CDataExchange* pDX) {
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CSettingDlg)
    // NOTE: the ClassWizard will add DDX and DDV calls here
    //}}AFX_DATA_MAP

}

#define IDC_SETTING_BACKUP              9010
#define IDC_SETTING_RESTORE             9011
#define IDC_SETTING_CMBSELECT           9012
#define IDC_SETTING_DELETE              9013
#define IDC_SETTING_ALLSELECT           9014
#define IDC_SETTING_CLEARSELECT         9015
#define IDC_SETTING_IN                  9016
#define IDC_SETTING_OUT                 9017
#define IDC_SETTING_LSTLOCAL            9018
#define IDC_SETTING_LSTUSB              9019

BEGIN_MESSAGE_MAP(CSettingDlg, CDialog)
    //{{AFX_MSG_MAP(CSettingDlg)
    ON_WM_TIMER()
    //}}AFX_MSG_MAP
    ON_NOTIFY(NM_CLICK, IDC_LIST_SETTING_TYPE, OnClickListType)
    ON_BN_CLICKED(IDC_BUTTON_SETTING_CANCEL, OnButtonSettingCancel)
    ON_BN_CLICKED(IDC_BUTTON_SETTING_DEFAULT, OnButtonSetting)
    ON_BN_CLICKED(IDC_BUTTON_SETTING_ADJUSTTOUCH, OnAdjustTouchPanel)
    ON_BN_CLICKED(IDC_BUTTON_SETTING_SETPASSWORD, OnSettingSaveScreenPassword)

    ON_BN_CLICKED(IDC_SETTING_BACKUP, OnBackup)
    ON_BN_CLICKED(IDC_SETTING_RESTORE, OnRestore)
    ON_BN_CLICKED(IDC_SETTING_OUT, OnFileOut)

    ON_MESSAGE(WM_SETTINGPASSWORD, OnSetSaveScreenPassWordOK)
    ON_MESSAGE(WM_CHECKPASSWORD, OnCheckPWD)
    ON_MESSAGE(WM_STATIC_CLICK, OnStaticClick)

    ON_MESSAGE(WM_CLICKMJPG_TOAPP, OnClickMJPG)
    ON_MESSAGE(WM_MJPGTOGGLE, OnClickMJPG)
    ON_MESSAGE(WM_STOPTRYRING, OnStopTryRing)
    ON_MESSAGE(WM_BTNST_CLICK, OnBtnSTClick)

    ON_BN_CLICKED(IDC_BUTTON_FASTDIALS_OK, OnButtonFastDialsOk)
    ON_MESSAGE(WM_COMMBOX_CLICKED, OnComboSelect)

    ON_WM_PAINT()
END_MESSAGE_MAP()

LRESULT CSettingDlg::OnCheckPWD(WPARAM w, LPARAM l) {
    LRESULT result = 0;
    if (l == IDC_CHECK_SETTING_PLAYPROTECT) {
        if (w == 0) {
            //@@
            //@@BOOL ret;
            //@@
        }
    }
    return result;
}

BOOL CSettingDlg::PreTranslateMessage(MSG* pMsg) {
    if ((pMsg->wParam) == MK_LBUTTON) {
        CString content;
        for (int i = 0; i < m_edtShowNumber; i++) {
            if (pMsg->hwnd == m_edtFastDialNumber[i].m_hWnd || pMsg->hwnd == m_edtFastDialName[i].m_hWnd) {
                m_curEditWnd = pMsg->hwnd;
                m_editFocusIndex = i;
                break;
            }
        }
    }
    return CDialog::PreTranslateMessage(pMsg);
}

void CSettingDlg::SetShowTimer() {
    m_edtLocalAreaNumber.ShowWindow(FALSE);
    m_edtOutLineDial1.ShowWindow(FALSE);
    m_cmbRing.ShowWindow(FALSE);
    m_cmbRingTimes.ShowWindow(FALSE);
    m_cmbAutoRecoedeTimes.ShowWindow(FALSE);
    m_edtADSLName.ShowWindow(FALSE);
    m_edtADSLPassword.ShowWindow(FALSE);
    m_cmbSMSRing.ShowWindow(FALSE);
    m_cmbSpecRing.ShowWindow(FALSE);
    m_cmbTime.ShowWindow(FALSE);

    for (int i = 0; i < 4; i++) {
        m_edtIP[i].ShowWindow(FALSE);
        m_edtMask[i].ShowWindow(FALSE);
        m_edtGateway[i].ShowWindow(FALSE);
        m_edtDNS[i].ShowWindow(FALSE);
    }

    for (int i = 0; i < 12; i++) {
        m_edtFastDialNumber[i].ShowWindow(FALSE);
        m_edtFastDialName[i].ShowWindow(FALSE);
    }

    //systom
    m_cmbWaitTime.ShowWindow(FALSE);
    m_dtTime.ShowWindow(FALSE);
    m_dtDate.ShowWindow(FALSE);
    //blacklight
    m_cmbBlackLightWaitTime.ShowWindow(FALSE);
    m_cmbNightBlackLightStartTime.ShowWindow(FALSE);
    m_cmbNightBlackLightEndTime.ShowWindow(FALSE);

    SetTimer(0x11, 200, NULL);
}

LRESULT CSettingDlg::OnStopTryRing(WPARAM w, LPARAM l) {
    LRESULT result = 0;
    if (m_MJPGList.GetUnitIsDownStatus(10)) {
        m_MJPGList.SetUnitIsDownStatus(10, FALSE);
        m_MJPGList.SetUnitIsShow(10, TRUE);
        ((CMultimediaPhoneDlg *)(theApp.m_pMainWnd))->m_pMainDlg->m_mainMp3Dlg_->SendMessage(WM_OUTEVENT, 0, 1);
    }
    return result;
}

LRESULT CSettingDlg::OnBtnSTClick(WPARAM w, LPARAM l) {
    LRESULT result = 0;
    //@@
    //@@BOOL ret;
    //@@
    if (IDC_SETTING_BLACKSCREEN1 == w) {
    } else if (IDC_SETTING_BLACKSCREEN2 == w) {
    } else if (IDC_CHECK_SETTING_PLAYPROTECT == w) {
        if (!m_bLogin) {
            std::string strTemp = m_pTempSetting->playRecordPassword();
            if (strTemp != "") {
                m_passwordDlg->SetType(CHECK_PLAYPASSWORD, w);
                m_passwordDlg->SetOldPassWord((char*)strTemp.c_str());
                m_passwordDlg->SetHWnd(this->m_hWnd);
                m_passwordDlg->ShowWindow_(SW_SHOW);
            }
        }
    }
    return result;
}

void CSettingDlg::StopTryRing() {
    if (m_MJPGList.GetUnitIsDownStatus(10)) {
        ((CMultimediaPhoneDlg *)(theApp.m_pMainWnd))->phone_->SetMsgWnd(this);
        ((CMultimediaPhoneDlg *)(theApp.m_pMainWnd))->phone_->StartRing(L"");
    }
}

/////////////////////////////////////////////////////////////////////////////
// CSettingDlg message handlers
LRESULT CSettingDlg::OnClickMJPG(WPARAM w, LPARAM l) {
    LRESULT result = 0;
    CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)(theApp.m_pMainWnd) ;
    CString icon;
    std::string soundPath;
    BOOL sd = DetectDIR(_T("\\StorageCard"));
    int i;
    //  if(m_uiType == 1 && w != 100)
    //  {
    //      StopTryRing();
    //  }
    switch (w) {
    case 1:
        m_MJPGList.SetCurrentLinkFile(".\\adv\\mjpg\\k5\\中文\\铃声设置.xml");
        SetRingSetParam(m_pTempSetting);
        m_uiType = 1;
        m_clickType = 1;
        SetPageTab(m_uiType);
        m_MJPGList.Invalidate();
        SetShowTimer();
        break;
    case 2:
        m_MJPGList.SetCurrentLinkFile(".\\adv\\mjpg\\k5\\中文\\网络通讯.xml");

        if (m_pTempSetting->linkMode() == Data::lmDirect) {
            m_MJPGList.SetUnitIsDownStatus(200, TRUE);
            m_MJPGList.SetUnitIsDownStatus(201, FALSE);
        } else {
            m_MJPGList.SetUnitIsDownStatus(200, FALSE);
            m_MJPGList.SetUnitIsDownStatus(201, TRUE);
        }

        if (m_ip.isDHCP) {
            m_MJPGList.SetUnitIsDownStatus(202, TRUE);
            m_MJPGList.SetUnitIsDownStatus(203, FALSE);
        } else {
            m_MJPGList.SetUnitIsDownStatus(202, FALSE);
            m_MJPGList.SetUnitIsDownStatus(203, TRUE);
        }

        m_uiType = 2;
        m_clickType = 2;
        SetPageTab(m_uiType);
        SetShowTimer();
        m_MJPGList.Invalidate();
        break;
    case 3:
        m_MJPGList.SetCurrentLinkFile(".\\adv\\mjpg\\k5\\中文\\系统参数.xml");
        SetSyetemParam(m_pTempSetting);
        m_uiType = 3;
        m_clickType = 3;
        SetPageTab(m_uiType);
        SetShowTimer();
        m_MJPGList.Invalidate();
        break;
    case 4:
        m_MJPGList.SetCurrentLinkFile(".\\adv\\mjpg\\k5\\中文\\背光设置.xml");

        for (i = 0; i < 7; i++) {
            m_MJPGList.SetUnitIsDownStatus(410 + i, FALSE);
        }
        m_MJPGList.SetUnitIsDownStatus(410 + m_pTempSetting->blackLightValue(), TRUE);

        SetBlackLightParam(m_pTempSetting);
        m_uiType = 4;
        m_clickType = 4;
        SetPageTab(m_uiType);
        SetShowTimer();
        m_MJPGList.Invalidate();
        break;
    case 5:
        m_MJPGList.SetCurrentLinkFile(".\\adv\\mjpg\\k5\\中文\\导入导出.xml");
        m_uiType = 5;
        m_clickType = 5;
        SetPageTab(m_uiType);
        SetShowTimer();
        m_MJPGList.Invalidate();
        break;
    case 6:
        {
            m_MJPGList.SetCurrentLinkFile(".\\adv\\mjpg\\k5\\中文\\系统信息.xml");
            m_MJPGList.Invalidate();
            m_uiType = 6;
            m_clickType = 6;
            SetPageTab(m_uiType);
            SetShowTimer();
            m_MJPGList.Invalidate();
            break;
        }
    case 100:
        //  设置铃声
        m_MJPGList.SetCurrentLinkFile(".\\adv\\mjpg\\k5\\中文\\铃声设置.xml");
        SetRingSetParam(m_pTempSetting);
        SetPageTab(1);
        m_MJPGList.Invalidate();
        m_clickType = 100;
        SetShowTimer(); 
        break;
    case 101:
        //号码设置
        m_MJPGList.SetCurrentLinkFile(".\\adv\\mjpg\\k5\\中文\\号码设置.xml");
        if (1 == m_pTempSetting->isAppendIpPrefix_) {
            m_MJPGList.SetUnitIsDownStatus(122, TRUE);
        } else {
            m_MJPGList.SetUnitIsDownStatus(122, FALSE);
        }
        SetPageTab(1);
        m_MJPGList.Invalidate();
        m_clickType = 101;
        SetShowTimer();
        break;
    case 102:
        //  设置呼叫
        m_MJPGList.SetCurrentLinkFile(".\\adv\\mjpg\\k5\\中文\\呼叫设置.xml");
        SetPageTab(1);
        m_MJPGList.Invalidate();
        m_clickType = 102;
        SetShowTimer(); 
        break;
    case 103:
        //防火墙设置
        m_MJPGList.SetCurrentLinkFile(".\\adv\\mjpg\\k5\\中文\\防火墙.xml");
        SetPageTab(1);
        SetParameters(/*m_pTempSetting->isFirewall(),*/ m_pTempSetting->firewallType(), (int)m_pTempSetting->blockAllTimeDuration().GetTotalSeconds());
        m_MJPGList.Invalidate();
        m_clickType = 103;
        SetShowTimer();
        break;
    case 117:
    case 118:
    case 119:
        {
            UINT16 volume[] = {
                0xFF00, 0xdd00, 0xbb00, 0xaa00, 0x8800, 0x6600, 0x4400, 0x2200, 0x1600, 0x1000
            };
            int nVolume;
            UINT32 v;
            CString str = L"";;
            if (117 == w) {
                m_cmbRing.GetWindowText(str);
            } else if (118 == w) {
                m_cmbSMSRing.GetWindowText(str);
            } else if (119 == w) {
                m_cmbSpecRing.GetWindowText(str);
            }

            if (str == "") {
                break;
            } else {
                  nVolume = m_systemVolume;
                CString s = m_ringDir;
                  s += str;
                  v = volume[nVolume] | (volume[nVolume] << 16);
                  ((CMultimediaPhoneDlg *)(theApp.m_pMainWnd))->phone_->g_RingSound = v;

                  ((CMultimediaPhoneDlg *)(theApp.m_pMainWnd))->m_pMainDlg->m_mainMp3Dlg_->OnTimer(1002);
                  ((CMultimediaPhoneDlg *)(theApp.m_pMainWnd))->phone_->SetMsgWnd(this);
                  ((CMultimediaPhoneDlg *)(theApp.m_pMainWnd))->phone_->StartRing((LPTSTR)(LPCTSTR)s, 1);
            }
        }
        break;
    case 126:
        //设置速拨
        for (i = 0; i < 12; i++) {
            m_edtFastDialNumber[i].SetWindowText(L"");
            m_edtFastDialName[i].SetWindowText(L"");
        }
        m_MJPGList.SetCurrentLinkFile(".\\adv\\mjpg\\k5\\中文\\速拨设置.xml");
        SetFastDialParam(m_pTempSetting);
        m_MJPGList.SetUnitColor(1260, font_white, FALSE);
        m_MJPGList.SetUnitText(1260, L"速拨号码", FALSE);
        SetPageTab(1);
        m_MJPGList.Invalidate();
        m_clickType = 126;
        SetShowTimer();
        break;
    case 1261:
        //新建速拨号
    case 1262:
        //添加
        if (m_edtShowNumber < 12) {
            m_edtFastDialNumber[m_edtShowNumber].ShowWindow(TRUE);
            m_edtFastDialName[m_edtShowNumber].ShowWindow(TRUE);
            m_edtShowNumber++;
        } else {
              ((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->m_pWarningNoFlashDlg->SetTitle(L"速拨号已达到最大限制！");
              ((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->m_pWarningNoFlashDlg->SetHWnd(this->GetSafeHwnd());
              ((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->m_pWarningNoFlashDlg->ShowWindow_(TRUE);
        }
        break;
    case 1263:
        //删除
        OnBtnDelFastDial();
        break;
    case 111:
    case 112:
    case 113:
        if (m_MJPGList.GetUnitIsDownStatus(w)) {
            if (111 == w) {
                m_cmbRing.EnableWindow(FALSE);
                m_pTempSetting->isPhoneCallRing_ = 0;
            } else if (112 == w) {
                m_cmbSMSRing.EnableWindow(FALSE);
                m_pTempSetting->isSmsRing_ = 0;
            } else if (113 == w) {
                m_cmbSpecRing.EnableWindow(FALSE);
                m_pTempSetting->isSpecodeRing_ = 0;
            }

            m_MJPGList.SetUnitIsDisable(w + 3, TRUE);

            for (size_t i = w * 10; i < (w + 1) * 10; ++i) {
                m_MJPGList.SetUnitIsDisable(i, TRUE);
            }

            m_MJPGList.SetUnitIsDownStatus(w, FALSE);
            m_MJPGList.SetUnitIsShow(w, TRUE);
        } else {
            if (111 == w) {
                m_cmbRing.EnableWindow(TRUE);
                m_pTempSetting->isPhoneCallRing_ = 1;
            } else if (112 == w) {
                m_cmbSMSRing.EnableWindow(TRUE);
                m_pTempSetting->isSmsRing_ = 1;
            } else if (113 == w) {
                m_cmbSpecRing.EnableWindow(TRUE);
                m_pTempSetting->isSpecodeRing_ = 1;
            }

            m_MJPGList.SetUnitIsDisable(w + 3, FALSE);

            for (size_t i = w * 10; i < (w + 1) * 10; ++i) {
                m_MJPGList.SetUnitIsDisable(i, FALSE);
            }

            m_MJPGList.SetUnitIsDownStatus(w, TRUE);
            m_MJPGList.SetUnitIsShow(w, TRUE);
        }
        break;
    case 121:
        break;
    case 122:
        if (m_MJPGList.GetUnitIsDownStatus(w)) {
            m_MJPGList.SetUnitIsDownStatus(w, FALSE);
            m_pTempSetting->isAppendIpPrefix_ = 0;
        } else {
            m_MJPGList.SetUnitIsDownStatus(w, TRUE);
            m_pTempSetting->isAppendIpPrefix_ = 1;
        }
        break;
    case 123:
        if (m_MJPGList.GetUnitIsDownStatus(w)) {
            m_MJPGList.SetUnitIsDownStatus(w, FALSE);
            m_pTempSetting->isAutoReply_ = 0;
        } else {
            m_MJPGList.SetUnitIsDownStatus(w, TRUE);
            m_pTempSetting->isAutoReply_ = 1;
        }
        break;
    case 131:
        m_pCallSetDlg->m_MJPGList.SetCurrentLinkFile(".\\adv\\mjpg\\k5\\中文\\呼叫转接.xml");
        m_pCallSetDlg->m_MJPGList.Invalidate();
        m_pCallSetDlg->m_uiType = 1;
        m_pCallSetDlg->SetCallSetParam();
        m_pCallSetDlg->ShowItemsControl();
        m_pCallSetDlg->ShowWindow(SW_SHOW);
        break;
    case 132:
        m_pCallSetDlg->m_MJPGList.SetCurrentLinkFile(".\\adv\\mjpg\\k5\\中文\\呼叫限制.xml");
        m_pCallSetDlg->m_MJPGList.Invalidate();
        m_pCallSetDlg->m_uiType = 2;
        m_pCallSetDlg->SetCallSetParam();
        m_pCallSetDlg->ShowItemsControl();
        m_pCallSetDlg->ShowWindow(SW_SHOW);
        break;
    case 133:
        m_pCallSetDlg->m_MJPGList.SetCurrentLinkFile(".\\adv\\mjpg\\k5\\中文\\呼叫等待.xml");
        m_pCallSetDlg->m_MJPGList.Invalidate();
        m_pCallSetDlg->m_uiType = 3;
        m_pCallSetDlg->SetCallSetParam();
        m_pCallSetDlg->ShowItemsControl();
        m_pCallSetDlg->ShowWindow(SW_SHOW);
        break;
    case 134:
        m_pPinSetDlg->SetCallSetParam();
        m_pPinSetDlg->ShowWindow(SW_SHOW);
        break;
    case 141:
    case 142:
    case 143:
    case 144:
        {
            for (int i = 141; i <= 144; i++) {
                if (i == w) {
                    m_MJPGList.SetUnitIsDownStatus(w, TRUE);
                    m_pTempSetting->firewallType((const Data::FirewallType)(w - 141));
                } else {
                    m_MJPGList.SetUnitIsDownStatus(i, FALSE);
                }
                m_MJPGList.SetUnitIsShow(i, TRUE);
            }
        }
        break;
    case 1110:
    case 1111:
    case 1112:
    case 1113:
    case 1114:
    case 1115:
    case 1116:
    case 1117:
    case 1118:
    case 1119:
        m_pTempSetting->phoneCallRingVolume_ = w + 1 - 1110;
        m_systemVolume = w - 1110;
        ChangeVolume(w, 0);
        break;
    case 1120:
    case 1121:
    case 1122:
    case 1123:
    case 1124:
    case 1125:
    case 1126:
    case 1127:
    case 1128:
    case 1129:
        m_pTempSetting->smsRingVolume_ = w + 1 - 1120;
        m_systemVolume = w - 1120;
        ChangeVolume(w, 1);
        break;
    case 1130:
    case 1131:
    case 1132:
    case 1133:
    case 1134:
    case 1135:
    case 1136:
    case 1137:
    case 1138:
    case 1139:
        m_pTempSetting->specodeRingVolume_ = w + 1 - 1130;
        m_systemVolume = w - 1130;
        ChangeVolume(w, 2);
        break;
    case 200:
    case 201:
        if (200 == w) {
            m_MJPGList.SetUnitIsDownStatus(w + 1, FALSE);
            m_MJPGList.SetUnitIsShow(w + 1, TRUE);
            m_pTempSetting->linkMode(Data::lmDirect);
        } else if (201 == w) {
            m_MJPGList.SetUnitIsDownStatus(w - 1, FALSE);
            m_MJPGList.SetUnitIsShow(w - 1, TRUE);
            m_pTempSetting->linkMode(Data::lmDial);
        }
        m_MJPGList.SetUnitIsDownStatus(w, TRUE);
        m_MJPGList.SetUnitIsShow(w, TRUE);
        break;
    case 202:
    case 203:
        if (202 == w) {
            m_MJPGList.SetUnitIsDownStatus(w + 1, FALSE);
            m_MJPGList.SetUnitIsShow(w + 1, TRUE);
            m_ip.isDHCP = 1;
            m_pSetting->ipMode(Data::imAuto);
        } else if (203 == w) {
            m_MJPGList.SetUnitIsDownStatus(w - 1, FALSE);
            m_MJPGList.SetUnitIsShow(w - 1, TRUE);
            m_ip.isDHCP = 0;
            m_pSetting->ipMode(Data::imManual);
        }
        m_MJPGList.SetUnitIsDownStatus(w, TRUE);
        m_MJPGList.SetUnitIsShow(w, TRUE);
        break;
    case 301:
        //是否启用管理员用户
        if (m_MJPGList.GetUnitIsDownStatus(w)) {
            m_pTempSetting->isAdmin(false);
            m_MJPGList.SetUnitIsDownStatus(w, FALSE);
        } else {
            m_pTempSetting->isAdmin(true);
            m_MJPGList.SetUnitIsDownStatus(w, TRUE);
        }
        m_MJPGList.SetUnitIsShow(w, TRUE);
        break;
    case 302:
        //是否设置屏幕保护密码
        if (!m_MJPGList.GetUnitIsDisable(w)) {
            if (m_MJPGList.GetUnitIsDownStatus(w)) {
                m_pTempSetting->isUseScreenSaverPassword(false);
                m_MJPGList.SetUnitIsDownStatus(w, FALSE);
            } else {
                m_pTempSetting->isUseScreenSaverPassword(true);
                m_MJPGList.SetUnitIsDownStatus(w, TRUE);
            }
        }
        m_MJPGList.SetUnitIsShow(w, TRUE);
        break;
    case 303:
        //是否设置屏保
        if (m_MJPGList.GetUnitIsDownStatus(w)) {
            m_pTempSetting->isUseScreenSaver(false);
            m_MJPGList.SetUnitIsDownStatus(w, FALSE);
            m_MJPGList.SetUnitIsDisable(302, TRUE);
        } else {
            m_pTempSetting->isUseScreenSaver(true);
            m_MJPGList.SetUnitIsDownStatus(w, TRUE);
            m_MJPGList.SetUnitIsDisable(302, FALSE);
        }
        m_MJPGList.SetUnitIsShow(w, TRUE);
        break;
    case 304:
        OnSettingAdminPassword();
        break;
    case 305:
        OnSettingSaveScreenPassword();
        break;
    case 307:
        if (sd) {
            m_MJPGList.SetUnitIsDownStatus(308, FALSE);
            m_MJPGList.SetUnitIsShow(308, TRUE);
        }

        m_MJPGList.SetUnitIsDownStatus(w, TRUE);
        m_MJPGList.SetUnitIsShow(w, TRUE);
        soundPath = "\\FlashDrv\\MY_RECORD\\";
        ((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->m_pMainDlg->OnTimer(2);
        if (!DetectDIR((LPTSTR)(LPCTSTR)Util::StringOp::ToCString(soundPath))) {
            CreateDirectory(Util::StringOp::ToCString(soundPath), NULL);
        }
        m_pTempSetting->soundPath(soundPath);

        break;
    case 308:
        m_MJPGList.SetUnitIsDownStatus(307, FALSE);
        m_MJPGList.SetUnitIsDownStatus(w, TRUE);
        m_MJPGList.SetUnitIsShow(308, TRUE);
        m_MJPGList.SetUnitIsShow(w, TRUE);
        soundPath = "\\StorageCard\\MY_RECORD\\";
        ((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->m_pMainDlg->OnTimer(2);
        if (!DetectDIR((LPTSTR)(LPCTSTR)Util::StringOp::ToCString(soundPath))) {
            CreateDirectory(Util::StringOp::ToCString(soundPath), NULL);
        }
        m_pTempSetting->soundPath(soundPath);
        break;
    case 310:
        //触屏校正
        OnAdjustTouchPanel();
        break;
    case 311:
        //设置系统时间
        m_dtDate.OnButtonUp();
        break;
    case 312:
        m_dtDate.OnButtonDown();
        break;
    case 313:
        m_dtTime.OnButtonUp();
        break;
    case 314:
        m_dtTime.OnButtonDown();
        break;
    case 501:
        OnBackup();
        break;
    case 502:
        OnRestore();
        break;
    case 503:
        OnFileOut();
        break;
    case 504:
        //MP3导入
        {
            m_copyfileDlg->SetType(mp3_in);
            m_copyfileDlg->ShowWindow_(SW_SHOW);
        }
        break;
    case 505:
        //电子相册导入
        {
            m_copyfileDlg->SetType(photo_in);
            m_copyfileDlg->ShowWindow_(SW_SHOW);
        }
        break;
    case 506:
        //振铃铃声导入
        {
            m_copyfileDlg->SetType(bell_in);
            m_copyfileDlg->ShowWindow_(SW_SHOW);
        }
        break;
    case 507:
        //MP3导出
        {
            m_copyfileDlg->SetType(mp3_out);
            m_copyfileDlg->ShowWindow_(SW_SHOW);
        }
        break;
    case 508:
        //电子相册导出
        {
            m_copyfileDlg->SetType(photo_out);
            m_copyfileDlg->ShowWindow_(SW_SHOW);
        }
        break;
    case 509:
        //振铃铃声导出
        {
            m_copyfileDlg->SetType(bell_out);
            m_copyfileDlg->ShowWindow_(SW_SHOW);
        }
        break;
    case 401:
        if (m_MJPGList.GetUnitIsDownStatus(w)) {
            m_cmbBlackLightWaitTime.EnableWindow(FALSE);
            m_MJPGList.SetUnitIsDisable(w + 2, TRUE);
            m_MJPGList.SetUnitIsDownStatus(w, FALSE);
            m_pTempSetting->isContrlBlackLight_ = false;
        } else {
            m_cmbBlackLightWaitTime.EnableWindow(TRUE);
            m_MJPGList.SetUnitIsDisable(w + 2, FALSE);
            m_MJPGList.SetUnitIsDownStatus(w, TRUE);
            m_pTempSetting->isContrlBlackLight_ = true;
        }
        m_MJPGList.SetUnitIsShow(w, TRUE);
        break;
    case 402:
        if (m_MJPGList.GetUnitIsDownStatus(w)) {
            m_cmbNightBlackLightStartTime.EnableWindow(FALSE);
            m_cmbNightBlackLightEndTime.EnableWindow(FALSE);
            m_MJPGList.SetUnitIsDisable(w + 2, TRUE);
            m_MJPGList.SetUnitIsDisable(w + 3, TRUE);
            m_MJPGList.SetUnitIsDownStatus(w, FALSE);
            m_pTempSetting->isNightControlBlackLight_ = false;
        } else {
            m_cmbNightBlackLightStartTime.EnableWindow(TRUE);
            m_cmbNightBlackLightEndTime.EnableWindow(TRUE);
            m_MJPGList.SetUnitIsDisable(w + 2, FALSE);
            m_MJPGList.SetUnitIsDisable(w + 3, FALSE);
            m_MJPGList.SetUnitIsDownStatus(w, TRUE);
            m_pTempSetting->isNightControlBlackLight_ = true;
        }
        m_MJPGList.SetUnitIsShow(w, TRUE);
        break;
    case 410:
        //设置背光级别
    case 411:
    case 412:
    case 413:
    case 414:
    case 415:
    case 416:
        {
            int old_value = m_pTempSetting->blackLightValue();
            if (old_value + 410 != w) {
                int i = w - 410;
                m_pTempSetting->blackLightValue(i);
                //将屏幕变黑
                m_MJPGList.SetUnitIsDownStatus(old_value + 410, FALSE);
                m_MJPGList.SetUnitIsDownStatus(w, TRUE);
                m_MJPGList.Invalidate();
                int v = 6 - (w - 410);
                if (v == 0) {
                    v = 1;
                }
                ((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->SetBackLight(v);
            }
            break;
        }
    case 1000:
        //返回
    case 1001:
        //确定
    case 1002:
        //保存默认设置
        if (1000 == w) {
            OnButtonSettingCancel();
        } else if (1001 == w) {
            m_iDeleteType = 1;
            OnButtonSetting();
        } else if (1002 == w) {
            m_iDeleteType = 2;
            OnButtonSetting();
        }
        main->PopbackIcon();

        break;
    }
    return result;
}

BOOL CSettingDlg::OnInitDialog() {
    CDialog::OnInitDialog();

    // TODO: Add extra initialization here
    //号码设置
    std::string strTemp = Data::LanguageResource::Get(Data::RI_RECORD_TYPE);
    CString str = strTemp.c_str();

    m_edtLocalAreaNumber.Create(WS_CHILD | WS_VISIBLE, CRect(220, 158, 439, 185), this, IDC_EDIT_SETTING_AREANO);
    m_edtLocalAreaNumber.SetLimitText(15);
    m_edtLocalAreaNumber.SetLimitDiagital();

    m_edtOutLineDial1.Create(WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, CRect(220, 199, 439, 226), this, IDC_EDIT_OUTLINEDIAL1);
    m_edtOutLineDial1.SetLimitText(64);

    m_cmbRingTimes.CreateEx(WS_CHILD | WS_VISIBLE, CRect(332, 243, 439, 400), this, IDC_COMBOBOX_SETTING_RINGTIMES, 22, 55, 26);
    m_cmbAutoRecoedeTimes.CreateEx(WS_CHILD | WS_VISIBLE, CRect(495, 242, 602, 400), this, IDC_COMBOBOX_SETTING_RINGTIMES, 22, 55, 26);

    //browser======================================================================================================
    m_edtADSLName.Create(ES_AUTOHSCROLL | WS_CHILD | WS_VISIBLE, CRect(183, 150, 352, 177), this, IDC_EDIT_SETTING_ADSLNAME);
    m_edtADSLName.SetLimitText(15);

    m_edtADSLPassword.Create(WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | ES_PASSWORD, CRect(418, 147, 607, 174), this, IDC_EDIT_SETTING_ADSLPASSWORD);
    m_edtADSLPassword.SetLimitText(15);

    for (int i = 0; i < 4; i++) {
        m_edtIP[i].Create(WS_CHILD | WS_VISIBLE, CRect(225 + i * (60 + 5), 258, 285 + i * (60 + 5), 285), this, IDC_EDIT_SETTING_IP, 26);
        m_edtIP[i].SetLimitText(3);
        m_edtIP[i].SetLimitDiagital();

        m_edtMask[i].Create(WS_CHILD | WS_VISIBLE, CRect(225 + i * (60 + 5), 291, 285 + i * (60 + 5), 318), this, IDC_EDIT_SETTING_MASK, 26);
        m_edtMask[i].SetLimitText(3);
        m_edtMask[i].SetLimitDiagital();

        m_edtGateway[i].Create(WS_CHILD | WS_VISIBLE, CRect(225 + i * (60 + 5), 324, 285 + i * (60 + 5), 351), this, IDC_EDIT_SETTING_GATEWAY, 26);
        m_edtGateway[i].SetLimitText(3);
        m_edtGateway[i].SetLimitDiagital();

        m_edtDNS[i].Create(WS_CHILD | WS_VISIBLE, CRect(225 + i * (60 + 5), 357, 285 + i * (60 + 5), 384), this, IDC_EDIT_SETTING_DNS, 26);
        m_edtDNS[i].SetLimitText(3);
        m_edtDNS[i].SetLimitDiagital();
    }
    //速拨
    for (int i = 0; i < 12; ++i) {
        if (i < 6) {
            m_edtFastDialNumber[i].Create(WS_CHILD | WS_VISIBLE, CRect(166, 157 + i * (27 + 10), 318, 184 + i * (27 + 10)), this, IDC_EDIT_FASTDIALS_NUMBER01, 24);
            m_edtFastDialNumber[i].SetLimitText(15);
            m_edtFastDialNumber[i].SetLimitDiagital();

            m_edtFastDialName[i].Create(WS_CHILD | WS_VISIBLE, CRect(70, 157 + i * (27 + 10), 158, 184 + i * (27 + 10)), this, IDC_EDIT_FASTDIALS_NUMBER02, 24);
            m_edtFastDialName[i].SetLimitText(15);
        } else {
            m_edtFastDialNumber[i].Create(WS_CHILD | WS_VISIBLE, CRect(421, 157 + (i - 6) * (27 + 10), 571, 184 + (i - 6) * (27 + 10)), this, IDC_EDIT_FASTDIALS_NUMBER01, 24);
            m_edtFastDialNumber[i].SetLimitText(15);
            m_edtFastDialNumber[i].SetLimitDiagital();

            m_edtFastDialName[i].Create(WS_CHILD | WS_VISIBLE, CRect(327, 157 + (i - 6) * (27 + 10), 415, 184 + (i - 6) * (27 + 10)), this, IDC_EDIT_FASTDIALS_NUMBER02, 24);
            m_edtFastDialName[i].SetLimitText(15);
        }
    }
    //铃声
    m_cmbRing.CreateEx(WS_CHILD | WS_VISIBLE, CRect(167, 174, 374, 400), this, IDC_COMBOBOX_SETTING_RING, 22, 55, 26);
    m_cmbSMSRing.CreateEx(WS_CHILD | WS_VISIBLE, CRect(167, 260, 374, 450), this, IDC_COMBOBOX_SETTING_SMSRING, 22, 55, 26);
    m_cmbSpecRing.CreateEx(WS_CHILD | WS_VISIBLE, CRect(167, 344, 374, 470), this, IDC_COMBOBOX_SETTING_SPECRING, 22, 55, 26);
    //防火墙
    m_cmbTime.CreateEx(WS_CHILD | WS_VISIBLE, CRect(374, 295, 500, 470), this, IDC_COMBO_FIREWALL_TIME, 22, 55, 26);
    m_cmbTime.AddString(_T("30分钟"));
    m_cmbTime.AddString(_T("1小时"));
    m_cmbTime.AddString(_T("2小时"));
    m_cmbTime.AddString(_T("3小时"));
    m_cmbTime.SelectString(0, _T("1小时"));

    //system======================================================================================================  
    m_cmbWaitTime.CreateEx(WS_CHILD | WS_VISIBLE, CRect(216, 219, 423, 400), this, IDC_COMBOBOX_SETTING_WAITTIME, 22, 55, 26);

    m_dtDate.Create(WS_VISIBLE | WS_CHILD | DTS_SHORTDATEFORMAT, CRect(215, 303, 367, 329), this, IDC_SETTING_DATE, 22, CDateCtrl::modeDate);
    m_dtTime.Create(WS_VISIBLE | WS_CHILD | DTS_TIMEFORMAT, CRect(430, 303, 582, 329), this, IDC_SETTING_TIME, 22, CDateCtrl::modeTime);

    m_Font.CreateFont(28,                        // nHeight
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
        _T("宋体"));                 // lpszFacename
    m_dtDate.SetFont(&m_Font);
    m_dtTime.SetFont(&m_Font);

    //blacklight
    m_cmbBlackLightWaitTime.CreateEx(WS_CHILD | WS_VISIBLE, CRect(236, 242, 393, 410), this, IDC_BLACKLIGHTWAIT_TIME, 22, 55, 26);
    m_cmbNightBlackLightStartTime.CreateEx(WS_VISIBLE | WS_CHILD, CRect(237, 339, 395, 470), this, IDC_NIGHTLIGHTSTART_TIME, 22, 55, 26);
    m_cmbNightBlackLightEndTime.CreateEx(WS_VISIBLE | WS_CHILD, CRect(422, 339, 580, 470), this, IDC_NIGHTLIGHTEND_TIME, 22, 55, 26);

    m_MJPGList.Create(L"", WS_VISIBLE | WS_CHILD, CRect(0, 0, 800, 423), this);
    m_MJPGList.SetMJPGRect(CRect(0, 0, 800, 423));

    MoveWindow(0, 57, 800, 423);

    m_uiType = 1;

    m_passwordDlg = new CPasswordDlg(this);
    m_passwordDlg->Create(CPasswordDlg::IDD);

    m_pTipDlg = new CTipDlg(this);
    m_pTipDlg->Create(CTipDlg::IDD);

    m_copyfileDlg = new copyfileDlg(this);
    m_copyfileDlg->Create(copyfileDlg::IDD);

    m_pCallSetDlg = new CCallSetDlg(this);
    m_pCallSetDlg->Create(CCallSetDlg::IDD);

    m_pPinSetDlg = new CPinSetDlg(this);
    m_pPinSetDlg->Create(CPinSetDlg::IDD);

    m_pSetting = Data::Setting::GetCurrentConfig();
    m_pTempSetting = m_pSetting;
    IniCtrlData();
    OnClickMJPG(1, 0);  

    if (!(((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->SetNightControlBackLightTimer())) {
        int v = m_pTempSetting->blackLightValue();
        v = 6 - v;
        ((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->SetBackLight(v);
    } else {
        ((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->SetBackLight(0);    //关闭背光
    }

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}


void CSettingDlg::SetButtonDefaultColor(CCEButtonST* button) {
    button->SetColor(CCEButtonST::BTNST_COLOR_BK_IN, RGB(237, 237, 237));
    button->SetColor(CCEButtonST::BTNST_COLOR_BK_OUT, RGB(237, 237, 237));
    button->SetColor(CCEButtonST::BTNST_COLOR_BK_FOCUS, RGB(237, 237, 237));
}

void CSettingDlg::SetButtonSelectedColor(CCEButtonST* button) {
    button->SetColor(CCEButtonST::BTNST_COLOR_BK_IN, Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
    button->SetColor(CCEButtonST::BTNST_COLOR_BK_OUT, Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
    button->SetColor(CCEButtonST::BTNST_COLOR_BK_FOCUS, Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
}

void CSettingDlg::OnClickListType(NMHDR* pNMHDR, LRESULT* pResult) {
    // TODO: Add your control notification handler code here
    ShowConfigItems();
    *pResult = 0;
}

void CSettingDlg::IniCtrlData() {
    ASSERT(m_pTempSetting);
    //号码
    if (m_pTempSetting->thisTelephoneNumber().number() != "") {
        m_edtLocalAreaNumber.SetWindowText(Util::StringOp::ToCString(m_pTempSetting->thisTelephoneNumber().number()));
    }
    m_edtOutLineDial1.SetWindowText(Util::StringOp::ToCString(m_pTempSetting->ipPrefix()));

    CString s = Util::StringOp::ToCString(m_pTempSetting->defaultRingFilename());
    CString filename;
    CString path;
    CFileStatus status;
    if (CFile::GetStatus(s, status)) {
        int nStart = 0;
        int n = s.Find(_T("/"), nStart);
        while (n != -1) {
            nStart = n + 1;
            n = s.Find(_T("/"), nStart);
        }
        filename = s.Mid(nStart);
        path = s.Mid(0, nStart);
    }

    m_cmbRingTimes.ResetContent();
    CString sCount[4] = {
        "3", "4", "5", "6"
    };
    m_cmbRingTimes.AddString(sCount[0]);
    m_cmbRingTimes.AddString(sCount[1]);
    m_cmbRingTimes.AddString(sCount[2]);
    m_cmbRingTimes.AddString(sCount[3]);
    int nRingCount = m_pTempSetting->autoReplyRingCount();
    int nn = 0;
    if (nRingCount == 3) {
        nn = 0;
    } else if (nRingCount == 4) {
        nn = 1;
    } else if (nRingCount == 5) {
        nn = 2;
    } else if (nRingCount == 6) {
        nn = 3;
    }
    m_cmbRingTimes.SetCurSel(nn);

    m_cmbAutoRecoedeTimes.ResetContent();
    CString sCount_[4] = {
        "15秒", "30秒", "60秒", "90秒"
    };
    m_cmbAutoRecoedeTimes.AddString(sCount_[0]);
    m_cmbAutoRecoedeTimes.AddString(sCount_[1]);
    m_cmbAutoRecoedeTimes.AddString(sCount_[2]);
    m_cmbAutoRecoedeTimes.AddString(sCount_[3]);
    int nTimes = m_pTempSetting->maxSoundUseSize();    //暂时最大声音条数来记录自动录音的时长

    m_cmbAutoRecoedeTimes.SetCurSel(nTimes);

    m_edtADSLName.SetWindowText(Util::StringOp::ToCString(m_pTempSetting->dialUsername()));
    m_edtADSLPassword.SetWindowText(Util::StringOp::ToCString(m_pTempSetting->dialPassword()));

    //网络 从注册表中获取IP信息显示
    GetIPInfo();
    CString ip = m_ip.ipAddress;
    int index1 = 0;
    for (int i = 0; i < 4; i++) {
        int index = ip.Find('.', index1);
        CString ss;
        if (index > 0) {
            ss = ip.Mid(index1, index - index1);
        } else {
            ss = ip.Mid(index1);
        }
        index1 = index + 1;
        m_edtIP[i].SetWindowText(ss);
    }

    ip = m_ip.mask;
    index1 = 0;
    for (int i = 0; i < 4; i++) {
        int index = ip.Find('.', index1);
        CString ss;
        if (index > 0) {
            ss = ip.Mid(index1, index - index1);
        } else {
            ss = ip.Mid(index1);
        }
        index1 = index + 1;
        m_edtMask[i].SetWindowText(ss);
    }

    ip = m_ip.gw;
    index1 = 0;
    for (int i = 0; i < 4; i++) {
        int index = ip.Find('.', index1);
        CString ss;
        if (index > 0) {
            ss = ip.Mid(index1, index - index1);
        } else {
            ss = ip.Mid(index1);
        }
        index1 = index + 1;
        m_edtGateway[i].SetWindowText(ss);
    }

    ip = m_ip.dns;
    index1 = 0;
    for (int i = 0; i < 4; i++) {
        int index = ip.Find('.', index1);
        CString ss;
        if (index > 0) {
            ss = ip.Mid(index1, index - index1);
        } else {
            ss = ip.Mid(index1);
        }
        index1 = index + 1;
        m_edtDNS[i].SetWindowText(ss);
    }
    //铃声
    UINT16 volume[] = {
        0xFF00, 0xdd00, 0xbb00, 0xaa00, 0x8800, 0x6600, 0x4400, 0x2200, 0x1600, 0x1000
    };
    int nVolume;
    nVolume = m_pTempSetting->phoneCallRingVolume_;
    ChangeVolume(nVolume + 1110, 0);
    nVolume = m_pTempSetting->smsRingVolume_;
    ChangeVolume(nVolume + 1120, 1);
    nVolume = m_pTempSetting->specodeRingVolume_;
    ChangeVolume(nVolume + 1130, 2);
    //系统参数
    CString sSecond[5] = {
        "15秒", "30秒", "1分钟", "5分钟", "15分钟"
    };
    m_cmbWaitTime.ResetContent();

    m_cmbWaitTime.AddString(sSecond[0]);
    m_cmbWaitTime.AddString(sSecond[1]);
    m_cmbWaitTime.AddString(sSecond[2]);
    m_cmbWaitTime.AddString(sSecond[3]);
    m_cmbWaitTime.AddString(sSecond[4]);

    CTimeSpan time = m_pTempSetting->screenSaverDuration();
    int nSel = 0;
    int nSeconds = (int)time.GetTotalSeconds();
    if (nSeconds == 15) {
        nSel = 0;
    } else if (nSeconds == 30) {
        nSel = 1;
    } else if (nSeconds == 60) {
        nSel = 2;
    } else if (nSeconds == 5 * 60) {
        nSel = 3;
    } else if (nSeconds == 15 * 60) {
        nSel = 4;
    }
    m_cmbWaitTime.SetCurSel(nSel);

    //blacklight
    CString sWait[4] = {
        "1分钟", "5分钟", "10分钟", "30分钟"
    };
    m_cmbBlackLightWaitTime.ResetContent();
    m_cmbBlackLightWaitTime.AddString(sWait[0]);
    m_cmbBlackLightWaitTime.AddString(sWait[1]);
    m_cmbBlackLightWaitTime.AddString(sWait[2]);
    m_cmbBlackLightWaitTime.AddString(sWait[3]);
    m_cmbBlackLightWaitTime.SetCurSel(m_pTempSetting->contrlBlackLightWaitTime());

    CString sStartTime[5] = {
        "晚上6点", "晚上7点", "晚上8点", "晚上9点", "晚上10点"
    };
    m_cmbNightBlackLightStartTime.ResetContent();
    m_cmbNightBlackLightStartTime.AddString(sStartTime[0]);
    m_cmbNightBlackLightStartTime.AddString(sStartTime[1]);
    m_cmbNightBlackLightStartTime.AddString(sStartTime[2]);
    m_cmbNightBlackLightStartTime.AddString(sStartTime[3]);
    m_cmbNightBlackLightStartTime.AddString(sStartTime[4]);
    m_cmbNightBlackLightStartTime.SetCurSel(m_pTempSetting->nightControlBlackLightStartTime());

    CString sEndTime[5] = {
        "早晨5点", "早晨6点", "早晨7点", "早晨8点", "早晨9点"
    };
    m_cmbNightBlackLightEndTime.ResetContent();
    m_cmbNightBlackLightEndTime.AddString(sEndTime[0]);
    m_cmbNightBlackLightEndTime.AddString(sEndTime[1]);
    m_cmbNightBlackLightEndTime.AddString(sEndTime[2]);
    m_cmbNightBlackLightEndTime.AddString(sEndTime[3]);
    m_cmbNightBlackLightEndTime.AddString(sEndTime[4]);
    m_cmbNightBlackLightEndTime.SetCurSel(m_pTempSetting->nightControlBlackLightEndTime());
}

void CSettingDlg::SetSettingOK() {
    CString oldLinkFile = L"";
    m_MJPGList.GetCurrentLinkFile(oldLinkFile);

    if (m_uiType == 3) {
        SYSTEMTIME curtime, curtime1, curtime2;
        m_dtDate.GetTime(curtime1);
        m_dtTime.GetTime(curtime2);

        if ((m_curtime.wYear != curtime1.wYear) || (m_curtime.wMonth != curtime1.wMonth) || (m_curtime.wDay != curtime1.wDay) || (m_curtime.wHour != curtime2.wHour) || (m_curtime.wMinute != curtime2.wMinute) || (m_curtime.wSecond != curtime2.wSecond)) {
            memcpy(&curtime, &curtime1, sizeof(SYSTEMTIME));
            curtime.wHour = curtime2.wHour;
            curtime.wMinute = curtime2.wMinute;
            curtime.wSecond = curtime2.wSecond;
            SetLocalTime(&curtime); 
            memcpy(&m_curtime, &curtime, sizeof(SYSTEMTIME));

            ((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->SetAlarmList();    //重新计算闹铃
        }
    }
    //号码
    CString str;
    m_edtLocalAreaNumber.GetWindowText(str);
    m_pTempSetting->thisTelephoneNumber(Data::TelephoneNumber(Util::StringOp::FromCString(str)));
    str = L"";
    m_edtOutLineDial1.GetWindowText(str);
    m_pTempSetting->ipPrefix(Util::StringOp::FromCString(str));

    m_cmbRingTimes.GetWindowText(str);
    m_pTempSetting->autoReplyRingCount(Util::StringOp::ToInt(Util::StringOp::FromCString(str)));

    m_pTempSetting->maxSoundUseSize(m_cmbAutoRecoedeTimes.GetCurSel());  //?自动答录时长

    //防火墙
    OnButtonFireWallOk();
    //速拨
    OnButtonFastDialsOk();
    //网络通讯
    m_edtADSLName.GetWindowText(str);
    m_pTempSetting->dialUsername(Util::StringOp::FromCString(str));

    m_edtADSLPassword.GetWindowText(str);
    m_pTempSetting->dialPassword(Util::StringOp::FromCString(str));

    NETWORK_ADPT_INFO ipConfig;
    memset(&ipConfig, 0, sizeof(NETWORK_ADPT_INFO));

    char txt[4][32];
    memset(txt, 0, 4 * 32);
    Data::IPAddr nIP[4] = {
        0, 0, 0, 0
    };
    for (int i = 0; i < 4; i++) {
        m_edtIP[i].GetWindowText(str);
        nIP[0] += ((Util::StringOp::ToInt(Util::StringOp::FromCString(str))) << (i * 8));
        m_edtMask[i].GetWindowText(str);
        nIP[1] += ((Util::StringOp::ToInt(Util::StringOp::FromCString(str))) << (i * 8));
        m_edtGateway[i].GetWindowText(str);
        nIP[2] += ((Util::StringOp::ToInt(Util::StringOp::FromCString(str))) << (i * 8));
        m_edtDNS[i].GetWindowText(str);
        nIP[3] += ((Util::StringOp::ToInt(Util::StringOp::FromCString(str))) << (i * 8));
    }

    sprintf_s(txt[0], 32, "%d.%d.%d.%d", (nIP[0] & 0xFF), ((nIP[0] >> 8) & 0xFF), ((nIP[0] >> 16) & 0xFF), ((nIP[0] >> 24) & 0xFF));
    sprintf_s(txt[1], 32, "%d.%d.%d.%d", (nIP[1] & 0xFF), ((nIP[1] >> 8) & 0xFF), ((nIP[1] >> 16) & 0xFF), ((nIP[1] >> 24) & 0xFF));
    if (!m_MJPGList.GetUnitIsDownStatus(200) && !m_MJPGList.GetUnitIsDownStatus(202)) {
        sprintf_s(txt[2], 32, "");
    } else {
          sprintf_s(txt[2], 32, "%d.%d.%d.%d", (nIP[2] & 0xFF), ((nIP[2] >> 8) & 0xFF), ((nIP[2] >> 16) & 0xFF), ((nIP[2] >> 24) & 0xFF));
    }
    sprintf_s(txt[3], 32, "%d.%d.%d.%d", (nIP[3] & 0xFF), ((nIP[3] >> 8) & 0xFF), ((nIP[3] >> 16) & 0xFF), ((nIP[3] >> 24) & 0xFF));

    CString sIP = txt[0];
    memcpy(ipConfig.IPAddr, (LPCTSTR)sIP, sIP.GetLength() * 2);
    sIP = txt[1];
    memcpy(ipConfig.SubnetMask, (LPCTSTR)sIP, sIP.GetLength() * 2);
    sIP = txt[2];
    memcpy(ipConfig.Gateway, (LPCTSTR)sIP, sIP.GetLength() * 2);
    sIP = txt[3];
    memcpy(ipConfig.DNSAddr, (LPCTSTR)sIP, sIP.GetLength() * 2);
    memcpy(ipConfig.WINSAddr, (LPCTSTR)sIP, sIP.GetLength() * 2);
    SetIPConfig(ipConfig);

    m_pTempSetting->thisIPAddr(nIP[0]);
    m_pTempSetting->subnetMask(nIP[1]);
    m_pTempSetting->gateway(nIP[2]);
    m_pTempSetting->dns(nIP[3]);

    int nSecond[5] = {
        15, 30, 60, 5 * 60, 15 * 60
    };
    int nSel = m_cmbWaitTime.GetCurSel();
    CTimeSpan time = CTimeSpan(0, 0, nSecond[nSel] / 60, nSecond[nSel] % 60);
    m_pTempSetting->screenSaverDuration(time);

    //blacklight
    int index = m_cmbBlackLightWaitTime.GetCurSel();
    m_pTempSetting->contrlBlackLightWaitTime(index);
    index = m_cmbNightBlackLightStartTime.GetCurSel();
    m_pTempSetting->nightControlBlackLightStartTime(index);
    index = m_cmbNightBlackLightEndTime.GetCurSel();
    m_pTempSetting->nightControlBlackLightEndTime(index);

    m_pTempSetting->Update();

    if (m_bLogin) {
        if (!m_pTempSetting->isAdmin()) {
            m_MJPGList.SetUnitIsDownStatus(301, FALSE);
            m_bLogin = FALSE;
        }
    }

    ((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->SetScreenSaveTimer();
    ((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->SwitchPanel_(IDC_BUTTON_MAIN);

    m_MJPGList.SetCurrentLinkFile(oldLinkFile);
}

void CSettingDlg::OnButtonSettingCancel() {
    // TODO: Add your control notification handler code here
    IniCtrlData();
    ShowWindow(SW_HIDE);
    ((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->SwitchPanel_(IDC_BUTTON_MAIN);
}

void CSettingDlg::OnButtonSetting() {
    // TODO: Add your control notification handler code here
    if (1 == m_iDeleteType) {
        ((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetTitle(L"确定要保存设置吗？", 0);
    } else if (2 == m_iDeleteType) {
          ((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetTitle(L"确定要恢复默认设置吗？", 0);
    }
    ((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetHWnd(this->GetSafeHwnd());
    if ((m_pTempSetting->isAdmin() && !m_bLogin)) {
        //      ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetPasswordModel(true);
    }
    std::string pw = m_pTempSetting->adminPassword();
    ((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->m_pDeleteTipDlg->ShowWindow_(TRUE);
}

typedef BOOL (WINAPI *_TouchCalibrate)();

//触摸屏校正
void CSettingDlg::OnAdjustTouchPanel() {
    HINSTANCE hinstance = LoadLibrary(_T("coredll.dll"));
    if (hinstance == NULL) {
        AfxMessageBox(L"instance == NULL"); 
        return;
    }  
    _TouchCalibrate TouchCalibrate = NULL; 
    TouchCalibrate = (_TouchCalibrate)GetProcAddress(hinstance, L"TouchCalibrate"); 
    if (TouchCalibrate == NULL) {
        AfxMessageBox(L"TouchCalibrate == NULL"); 
        return;
    }  
    extern BOOL g_bAdjustPanel;
    g_bAdjustPanel = TRUE;
    TouchCalibrate(); 
    g_bAdjustPanel = FALSE;
    FreeLibrary(hinstance);
    TCHAR szTemp[256] = {
        0
    };
    _tcscpy_s(szTemp, 256, TEXT("HARDWARE\\DEVICEMAP\\TOUCH"));
    HKEY hKey = NULL;
    LONG hRes = RegOpenKeyEx(HKEY_LOCAL_MACHINE, szTemp, 0, 0, &hKey);
    if (ERROR_SUCCESS == hRes) {
    }
    LONG l = RegFlushKey(HKEY_LOCAL_MACHINE);
    RegCloseKey(hKey);
}

LRESULT CSettingDlg::OnSetSaveScreenPassWordOK(WPARAM w, LPARAM l) {
    LRESULT result = 0;
    if (l == SETTINGPLAY_PASSWORD) {
        m_pTempSetting->playRecordPassword((char*)w);
    } else if (l == SETTINGSUPPER_PASSWORD) {
        m_pTempSetting->adminPassword((char*)w);
    } else if (l == SETTINGSCREEN_PASSWORD) {
        m_pTempSetting->screenSaverPassword((char*)w);
    }
    m_pTempSetting->Update();
    return result;
}

void CSettingDlg::OnSettingAdminPassword() {
    m_passwordDlg->SettingType(SETTINGSUPPER_PASSWORD);
    std::string strTemp = m_pTempSetting->adminPassword();
    m_passwordDlg->SetOldPassWord((char*)strTemp.c_str());
    m_passwordDlg->SetHWnd(this->m_hWnd);
    m_passwordDlg->ShowWindow_(SW_SHOW);
}

void CSettingDlg::OnSettingSaveScreenPassword() {
    m_passwordDlg->SettingType(SETTINGSCREEN_PASSWORD);
    std::string strTemp = m_pTempSetting->screenSaverPassword();
    m_passwordDlg->SetOldPassWord((char*)strTemp.c_str());
    m_passwordDlg->SetHWnd(this->m_hWnd);
    m_passwordDlg->ShowWindow_(SW_SHOW);
}

void CSettingDlg::SetBlackLightParam(boost::shared_ptr<Data::Setting> data) {
    if (1 == data->isContrlBlackLight_) {
        m_MJPGList.SetUnitIsDownStatus(401, TRUE);
    } else {
        m_MJPGList.SetUnitIsDownStatus(401, FALSE);
    }

    if (1 == data->isNightControlBlackLight_) {
        m_MJPGList.SetUnitIsDownStatus(402, TRUE);
    } else {
        m_MJPGList.SetUnitIsDownStatus(402, FALSE);
    }
}

void CSettingDlg::SetSyetemParam(boost::shared_ptr<Data::Setting> data) {
    if (data->isAdmin()) {
        m_MJPGList.SetUnitIsDownStatus(301, TRUE);
    } else {
        m_MJPGList.SetUnitIsDownStatus(301, FALSE);
    }

    if (data->isUseScreenSaver()) {
        m_MJPGList.SetUnitIsDownStatus(303, TRUE);
        //      m_MJPGList.SetUnitIsShow(303, TRUE, FALSE);
        m_MJPGList.SetUnitIsDisable(302, FALSE);
    } else {
        m_MJPGList.SetUnitIsDownStatus(303, FALSE);
        m_MJPGList.SetUnitIsDisable(302, TRUE);
    }

    if (!m_MJPGList.GetUnitIsDisable(302)) {
        if (data->isUseScreenSaverPassword()) {
            m_MJPGList.SetUnitIsDownStatus(302, TRUE);
            //          m_MJPGList.SetUnitIsShow(302, TRUE);
        } else {
            m_MJPGList.SetUnitIsDownStatus(302, FALSE);
        }
    }
}

void CSettingDlg::ShowConfigItems(void) {
    m_edtLocalAreaNumber.ShowWindow(FALSE);
    m_edtOutLineDial1.ShowWindow(FALSE);
    m_cmbRing.ShowWindow(FALSE);
    m_cmbRingTimes.ShowWindow(FALSE);
    m_cmbAutoRecoedeTimes.ShowWindow(FALSE);
    m_edtADSLName.ShowWindow(FALSE);
    m_edtADSLPassword.ShowWindow(FALSE);
    m_cmbSMSRing.ShowWindow(FALSE);
    m_cmbSpecRing.ShowWindow(FALSE);
    m_cmbTime.ShowWindow(FALSE);

    for (int i = 0; i < 4; i++) {
        m_edtIP[i].ShowWindow(FALSE);
        m_edtMask[i].ShowWindow(FALSE);
        m_edtGateway[i].ShowWindow(FALSE);
        m_edtDNS[i].ShowWindow(FALSE);
    }

    for (int i = 0; i < 12; i++) {
        m_edtFastDialNumber[i].ShowWindow(FALSE);
        m_edtFastDialName[i].ShowWindow(FALSE);
    }

    //systom
    m_cmbWaitTime.ShowWindow(FALSE);
    m_dtTime.ShowWindow(FALSE);
    m_dtDate.ShowWindow(FALSE);
    //blacklight
    m_cmbBlackLightWaitTime.ShowWindow(FALSE);
    m_cmbNightBlackLightStartTime.ShowWindow(FALSE);
    m_cmbNightBlackLightEndTime.ShowWindow(FALSE);

    switch (m_clickType) {
    case 1:
        m_cmbRing.ShowWindow(TRUE);
        m_cmbSMSRing.ShowWindow(TRUE);
        m_cmbSpecRing.ShowWindow(TRUE);
        break;
    case 2:
        m_edtADSLName.ShowWindow(TRUE);
        m_edtADSLPassword.ShowWindow(TRUE); 
        for (int i = 0; i < 4; i++) {
            m_edtIP[i].ShowWindow(TRUE);
            m_edtMask[i].ShowWindow(TRUE);
            m_edtGateway[i].ShowWindow(TRUE);
            m_edtDNS[i].ShowWindow(TRUE);
        }
        break;
    case 3:
        {
            BOOL sd = DetectDIR(_T("\\StorageCard"));
            if (sd) {
                m_MJPGList.SetUnitIsShow(308, TRUE, FALSE);
                m_MJPGList.SetUnitFont(309, font_20);
                m_MJPGList.SetUnitText(309, L"SD卡", FALSE);
            } else {
                m_MJPGList.SetUnitIsShow(308, FALSE, FALSE);
                m_MJPGList.SetUnitText(309, L"", FALSE);
            }
            std::string soundPath = m_pTempSetting->soundPath();
            if (soundPath.substr(0, 12) == "\\StorageCard" && sd) {
                m_MJPGList.SetUnitIsDownStatus(308, TRUE);
                m_MJPGList.SetUnitIsDownStatus(307, FALSE);
                m_pTempSetting->soundPath("\\StorageCard\\MY_RECORD\\");
            } else {
                m_MJPGList.SetUnitIsDownStatus(307, TRUE);
                m_MJPGList.SetUnitIsShow(307, TRUE, FALSE);
                m_MJPGList.SetUnitIsDownStatus(308, FALSE);
                m_pTempSetting->soundPath("\\FlashDrv\\MY_RECORD\\");
            }

            m_MJPGList.Invalidate();
        }
        GetLocalTime(&m_curtime);
        m_dtDate.SetTime(m_curtime);
        m_dtTime.SetTime(m_curtime);

        m_dtTime.ShowWindow(TRUE);
        m_dtDate.ShowWindow(TRUE);
        m_cmbWaitTime.ShowWindow(TRUE);
        break;
    case 4:
        //背光设置
        m_cmbBlackLightWaitTime.ShowWindow(TRUE);
        m_cmbNightBlackLightStartTime.ShowWindow(TRUE);
        m_cmbNightBlackLightEndTime.ShowWindow(TRUE);
        break;
    case 6:
        //系统信息
        SetDiskInFo();
        break;
    case 100:
        //铃声设置
        m_cmbRing.ShowWindow(TRUE);
        m_cmbSMSRing.ShowWindow(TRUE);
        m_cmbSpecRing.ShowWindow(TRUE);
        break;
    case 101:
        //号码设置
        m_edtLocalAreaNumber.ShowWindow(TRUE);
        m_edtOutLineDial1.ShowWindow(TRUE);
        m_cmbRingTimes.ShowWindow(TRUE);
        m_cmbAutoRecoedeTimes.ShowWindow(TRUE);
        break;
    case 103:
        //防火墙设置
        m_cmbTime.ShowWindow(TRUE);
        break;
    case 126:
        //速拨
        for (int i = 0; i < m_edtShowNumber; i++) {
            m_edtFastDialNumber[i].ShowWindow(TRUE);
            m_edtFastDialName[i].ShowWindow(TRUE);
        }
        break;
    }
}

void CSettingDlg::ShowWindow_(int nCmdShow) {
    m_pTempSetting = m_pSetting;
    IniCtrlData();
    OnClickMJPG(1, 0);
    ShowWindow(nCmdShow);
}

void CSettingDlg::SetPageTab(int type) {
    for (int i = 1; i <= 6; i++) {
        if (i == type) {
            m_MJPGList.SetUnitIsDownStatus(i, TRUE);
        } else {
            m_MJPGList.SetUnitIsDownStatus(i, FALSE);
        }
        m_MJPGList.SetUnitIsShow(i, TRUE);
    }
}

void CSettingDlg::SetDiskInFo() {
    for (int i = 601; i <= 608; i++) {
        m_MJPGList.SetUnitFont(i, font_20);
        m_MJPGList.SetUnitColor(i, font_white, FALSE);
    }

    std::string temp = Data::LanguageResource::Get(Data::RI_SETTING_AVAILABLESIZE);
    CString as = Util::StringOp::ToCString(temp);
    CString text;
    ULARGE_INTEGER freeBytes;
    ULARGE_INTEGER totalBytes;
    ULARGE_INTEGER totalFreeBytes;
    GetDiskFreeSpaceEx(_T("\\Flashdrv"), &freeBytes, &totalBytes, &totalFreeBytes);
    int m = static_cast<int>(totalBytes.QuadPart / (1024 * 1024));
    int f = static_cast<int>(totalFreeBytes.QuadPart / (1024 * 1024));
    f = f - static_cast<int>((float)m * 0.1);
    if (f < 0) {
        f = 0;
    }
    text.Format(_T("%dM (%dM%s)"), m, f, as);
    m_MJPGList.SetUnitText(606, text, FALSE);

    freeBytes.QuadPart = 0;
    totalBytes.QuadPart = 0;
    totalFreeBytes.QuadPart = 0;
    GetDiskFreeSpaceEx(_T("\\StorageCard"), &freeBytes, &totalBytes, &totalFreeBytes);
    m = static_cast<int>(totalBytes.QuadPart / (1024 * 1024));
    f = static_cast<int>(totalFreeBytes.QuadPart / (1024 * 1024));
    text.Format(_T("%dM (%dM%s)"), m, f, as);
    m_MJPGList.SetUnitText(607, text, FALSE);

    char cSerielNo[64];
    memset(cSerielNo, 0, 64);
    memcpy(cSerielNo, ((CMultimediaPhoneDlg *)(theApp.m_pMainWnd))->m_manuinfo.serialNum, 32);
    char cTime[5] = {
        0
    };
    SYSTEMTIME tm;
    GetLocalTime(&tm);
    int y = tm.wYear + tm.wMonth + tm.wDay;
    sprintf_s(cTime, 5, "%04d", y);
    strcat_s(cSerielNo, 64, cTime);
    m_MJPGList.SetUnitText(601, cSerielNo, FALSE);

    char cHardVer[64];
    memset(cHardVer, 0, 64);
    memcpy(cHardVer, ((CMultimediaPhoneDlg *)(theApp.m_pMainWnd))->m_manuinfo.hardVer, 32);
    m_MJPGList.SetUnitText(605, cHardVer, FALSE);

    m_MJPGList.SetUnitText(604, s_VerSionTitle, FALSE);
    m_MJPGList.SetUnitText(608, L"北京易度科技", FALSE);

    m_MJPGList.Invalidate();
}

//数据库备份
void CSettingDlg::OnBackup() {
    m_copyfileDlg->SetType(backup_file);
    m_copyfileDlg->ShowWindow_(SW_SHOW);
}

//数据库恢复
void CSettingDlg::OnRestore() {
    m_copyfileDlg->SetType(restore_file);
    m_copyfileDlg->ShowWindow_(SW_SHOW);
}

//录音文件导出
void CSettingDlg::OnFileOut() {
    m_copyfileDlg->SetType(copy_file);
    m_copyfileDlg->ShowWindow_(SW_SHOW);
}

#include <iphlpapi.h>
#include <stdio.h>
#include <stdlib.h>

#define MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x))
#define FREE(x) HeapFree(GetProcessHeap(), 0, (x))

IPInfo CSettingDlg::GetIPInfo(char* AdapterName) {
    /* Declare and initialize variables */
    IPInfo ipInfo;

    PIP_ADAPTER_INFO pAdapterInfo;
    PIP_ADAPTER_INFO pAdapter = NULL;
    DWORD dwRetVal = 0;
    UINT i;

    /* variables used to print DHCP time info */
    //@@
    //@@struct tm newtime;
    //@@char buffer[32];
    //@@

    ULONG ulOutBufLen = sizeof(IP_ADAPTER_INFO);
    pAdapterInfo = (IP_ADAPTER_INFO *)MALLOC(sizeof(IP_ADAPTER_INFO));
    if (pAdapterInfo == NULL) {
        Dprintf("Error allocating memory needed to call GetAdaptersinfo\n");
        return ipInfo;
    }
    if (GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW) {
        FREE(pAdapterInfo);
        pAdapterInfo = (IP_ADAPTER_INFO *)MALLOC(ulOutBufLen);
        if (pAdapterInfo == NULL) {
            Dprintf("Error allocating memory needed to call GetAdaptersinfo\n");
            return ipInfo;
        }
    }

    if ((dwRetVal = GetAdaptersInfo(pAdapterInfo, &ulOutBufLen)) == NO_ERROR) {
        pAdapter = pAdapterInfo;
        while (pAdapter) {
            if (strstr(AdapterName, pAdapter->AdapterName)) {
                Dprintf("\tComboIndex: \t%d\n", pAdapter->ComboIndex);
                Dprintf("\tAdapter Name: \t%s\n", pAdapter->AdapterName);
                Dprintf("\tAdapter Desc: \t%s\n", pAdapter->Description);
                Dprintf("\tAdapter Addr: \t");
                for (i = 0; i < pAdapter->AddressLength; i++) {
                    if (i == (pAdapter->AddressLength - 1)) {
                        Dprintf("%.2X\n", (int)pAdapter->Address[i]);
                    } else {
                        Dprintf("%.2X-", (int)pAdapter->Address[i]);
                    }
                }
                Dprintf("\tIndex: \t%d\n", pAdapter->Index);
                Dprintf("\tType: \t");
                switch (pAdapter->Type) {
                case MIB_IF_TYPE_OTHER:
                    Dprintf("Other\n");
                    break;
                case MIB_IF_TYPE_ETHERNET:
                    Dprintf("Ethernet\n");
                    break;
                case MIB_IF_TYPE_TOKENRING:
                    Dprintf("Token Ring\n");
                    break;
                case MIB_IF_TYPE_FDDI:
                    Dprintf("FDDI\n");
                    break;
                case MIB_IF_TYPE_PPP:
                    Dprintf("PPP\n");
                    break;
                case MIB_IF_TYPE_LOOPBACK:
                    Dprintf("Lookback\n");
                    break;
                case MIB_IF_TYPE_SLIP:
                    Dprintf("Slip\n");
                    break;
                default:
                    Dprintf("Unknown type %ld\n", pAdapter->Type);
                    break;
                }

                Dprintf("\tIP Address: \t%s\n", pAdapter->IpAddressList.IpAddress.String);
                Dprintf("\tIP Mask: \t%s\n", pAdapter->IpAddressList.IpMask.String);

                Dprintf("\tGateway: \t%s\n", pAdapter->GatewayList.IpAddress.String);
                Dprintf("\t***\n");

                if (pAdapter->DhcpEnabled) {
                    Dprintf("\tDHCP Enabled: Yes\n");
                    Dprintf("\t  DHCP Server: \t%s\n", pAdapter->DhcpServer.IpAddress.String);

                    Dprintf("\t  Lease Obtained: ");    
                    ipInfo.isDHCP = TRUE;
                } else {
                    Dprintf("\tDHCP Enabled: No\n");
                    ipInfo.isDHCP = FALSE;
                }
                ipInfo.ipAddress = pAdapter->IpAddressList.IpAddress.String;
                ipInfo.mask = pAdapter->IpAddressList.IpMask.String;
                ipInfo.gw = pAdapter->GatewayList.IpAddress.String;
                break;
            }
            pAdapter = pAdapter->Next;
            Dprintf("\n");
        }
    } else {
        Dprintf("GetAdaptersInfo failed with error: %d\n", dwRetVal);
    }
    if (pAdapterInfo) {
        FREE(pAdapterInfo);
    }

    return ipInfo;
}

void CSettingDlg::GetIPInfo() {
    TCHAR szTemp[256] = {
        0
    };
    _tcscpy_s(szTemp, 256, TEXT("Comm\\"));
    _tcscat_s(szTemp, 256, TEXT("JZ47MAC"));
    _tcscat_s(szTemp, 256, TEXT("\\Parms\\TcpIp"));
    HKEY hKey = NULL;
    LONG hRes = RegOpenKeyEx(HKEY_LOCAL_MACHINE, szTemp, 0, 0, &hKey);
    if (ERROR_SUCCESS == hRes) {
        DWORD dwDataSize = sizeof(DWORD);
        DWORD dType;
        DWORD dNcount = 128;
        BYTE Buffer[128];
        char temp[128];
        memset(Buffer, 0, 128);
        dNcount = 128;
        hRes = RegQueryValueEx(hKey, TEXT("EnableDHCP"), NULL, &dType, Buffer, &dNcount);

        if (*(DWORD *)Buffer == 1) {
            m_ip.isDHCP = 1;
            memset(Buffer, 0, 64);
            dNcount = 128;
            hRes = RegQueryValueEx(hKey, TEXT("DhcpIPAddress"), NULL, &dType, Buffer, &dNcount);
            memset(temp, 0, 128);
            size_t convertCount = 0;
            wcstombs_s(&convertCount, temp, 128, (const wchar_t*)Buffer, dNcount);
            m_ip.ipAddress = temp;
            memset(Buffer, 0, 64);
            dNcount = 128;
            hRes = RegQueryValueEx(hKey, TEXT("DhcpDefaultGateway"), NULL, &dType, Buffer, &dNcount);
            memset(temp, 0, 128);
            wcstombs_s(&convertCount, temp, 128, (const wchar_t*)Buffer, dNcount);
            m_ip.gw = temp;
            memset(Buffer, 0, 64);
            dNcount = 128;
            hRes = RegQueryValueEx(hKey, TEXT("DhcpSubnetMask"), NULL, &dType, Buffer, &dNcount);
            memset(temp, 0, 128);
            wcstombs_s(&convertCount, temp, 128, (const wchar_t*)Buffer, dNcount);
            m_ip.mask = temp;
            memset(Buffer, 0, 64);
            dNcount = 128;
            hRes = RegQueryValueEx(hKey, TEXT("DhcpDNS"), NULL, &dType, Buffer, &dNcount);
            memset(temp, 0, 128);
            wcstombs_s(&convertCount, temp, 128, (const wchar_t*)Buffer, dNcount);
            m_ip.dns = temp;
        } else {
              m_ip.isDHCP = 0;
              memset(Buffer, 0, 64);
              dNcount = 128;
              hRes = RegQueryValueEx(hKey, TEXT("IpAddress"), NULL, &dType, Buffer, &dNcount);
              memset(temp, 0, 128);
            size_t convertCount = 0;
              wcstombs_s(&convertCount, temp, 128, (const wchar_t*)Buffer, dNcount);
              m_ip.ipAddress = temp;
              memset(Buffer, 0, 64);
              dNcount = 128;
              hRes = RegQueryValueEx(hKey, TEXT("Subnetmask"), NULL, &dType, Buffer, &dNcount);
              memset(temp, 0, 128);
              wcstombs_s(&convertCount, temp, 128, (const wchar_t*)Buffer, dNcount);
              m_ip.mask = temp;
              memset(Buffer, 0, 64);
              dNcount = 128;
              hRes = RegQueryValueEx(hKey, TEXT("DefaultGateway"), NULL, &dType, Buffer, &dNcount);
              memset(temp, 0, 128);
              wcstombs_s(&convertCount, temp, 128, (const wchar_t*)Buffer, dNcount);
              m_ip.gw = temp;
              memset(Buffer, 0, 64);
              dNcount = 128;
              hRes = RegQueryValueEx(hKey, TEXT("DNS"), NULL, &dType, Buffer, &dNcount);
              memset(temp, 0, 128);
              wcstombs_s(&convertCount, temp, 128, (const wchar_t*)Buffer, dNcount);
              m_ip.dns = temp;
        }
        RegCloseKey(hKey);
    }
}

void CSettingDlg::SetIPConfig(NETWORK_ADPT_INFO& AdptInfo) {
    TCHAR szTemp[256] = {
        0
    };
    _tcscpy_s(szTemp, 256, TEXT("Comm\\"));
    //_tcscat_s(szTemp, 256, TEXT("SMSC91181"));
    _tcscat_s(szTemp, 256, TEXT("JZ47MAC"));
    _tcscat_s(szTemp, 256, TEXT("\\Parms\\TcpIp"));
    HKEY hKey = NULL;
    LONG hRes = RegOpenKeyEx(HKEY_LOCAL_MACHINE, szTemp, 0, 0, &hKey);
    if (ERROR_SUCCESS == hRes) {
        DWORD dwDataSize = sizeof(DWORD);
        if (AdptInfo.fUseDHCP) {
            hRes = RegSetValueEx(hKey, TEXT("EnableDHCP"), NULL, REG_DWORD, (LPBYTE)&AdptInfo.fUseDHCP, dwDataSize);
        } else {
              hRes = RegSetValueEx(hKey, TEXT("EnableDHCP"), NULL, REG_DWORD, (LPBYTE)&AdptInfo.fUseDHCP, dwDataSize);
              dwDataSize = wcslen((wchar_t*)AdptInfo.IPAddr) * 2 + 2;
              hRes = RegSetValueEx(hKey, TEXT("IpAddress"), NULL, REG_MULTI_SZ, (LPBYTE)AdptInfo.IPAddr, dwDataSize);
              dwDataSize = wcslen((wchar_t*)AdptInfo.SubnetMask) * 2 + 2;
              hRes = RegSetValueEx(hKey, TEXT("Subnetmask"), NULL, REG_MULTI_SZ, (LPBYTE)AdptInfo.SubnetMask, dwDataSize);
              dwDataSize = wcslen((wchar_t*)AdptInfo.Gateway) * 2 + 2;
              hRes = RegSetValueEx(hKey, TEXT("DefaultGateway"), NULL, REG_MULTI_SZ, (LPBYTE)AdptInfo.Gateway, dwDataSize);
              dwDataSize = wcslen((wchar_t*)AdptInfo.DNSAddr) * 2 + 2;
              hRes = RegSetValueEx(hKey, TEXT("DNS"), NULL, REG_MULTI_SZ, (LPBYTE)AdptInfo.DNSAddr, dwDataSize);
        }
        RegFlushKey(HKEY_LOCAL_MACHINE);
        RegCloseKey(hKey);
    }
}

CTestDlg* m_pTestDlg0 = NULL;
LRESULT CSettingDlg::OnStaticClick(WPARAM w, LPARAM l) {
    LRESULT result = 0;
    if (m_pTestDlg0 == NULL) {
        m_pTestDlg0 = new CTestDlg(this);
        m_pTestDlg0->Create(CTestDlg::IDD);
    }
    extern BOOL _test_call;
    _test_call = TRUE;
    m_pTestDlg0->ShowWindow(SW_SHOW);
    return result;
}

//速拨
void CSettingDlg::OnButtonFastDialsOk() {
    std::vector<std::pair<std::string, std::string> > diallist;
    for (int i = 0; i < 12; ++i) {
        CString number;
        CString name;
        m_edtFastDialName[i].GetWindowText(name);
        m_edtFastDialNumber[i].GetWindowText(number);
        std::pair<std::string, std::string> pair(Util::StringOp::FromCString(name), Util::StringOp::FromCString(number));
        diallist.push_back(pair);
    }
    if (m_pTempSetting) {
        m_pTempSetting->speedDials(diallist);
    }

    SipShowIM(SIPF_OFF);

    ShowWindow_(FALSE);
    SipShowIM(SIPF_OFF);
}

void CSettingDlg::OnBtnDelFastDial() {
    m_iDeleteType = 3;
    ((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetTitle(L"确定要删除该速拨号码吗？", 0);
    ((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetHWnd(this->GetSafeHwnd());
    ((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->m_pDeleteTipDlg->ShowWindow_(TRUE);
}

void CSettingDlg::DeleteFastDialNum(boost::shared_ptr<Data::Setting> data) {
    std::vector<std::pair<std::string, std::string> > diallist = data->speedDials();
    std::vector<std::pair<std::string, std::string> >::iterator index = diallist.begin();//const_iterator
    if (m_editFocusIndex >= 0) {
        index += m_editFocusIndex;
        diallist.erase(index);
        m_edtShowNumber--;
        if (m_editFocusIndex < m_edtShowNumber) {
            for (int i = m_editFocusIndex; i < m_edtShowNumber; i++) {
                CString text;
                m_edtFastDialName[i + 1].GetWindowText(text);
                m_edtFastDialName[i].SetWindowText(text);
                text.Empty();
                m_edtFastDialNumber[i + 1].GetWindowText(text);
                m_edtFastDialNumber[i].SetWindowText(text);
            }
        }
        m_edtFastDialName[m_edtShowNumber].SetWindowText(L"");
        m_edtFastDialNumber[m_edtShowNumber].SetWindowText(L"");
        m_edtFastDialName[m_edtShowNumber].ShowWindow(FALSE);
        m_edtFastDialNumber[m_edtShowNumber].ShowWindow(FALSE);
        m_editFocusIndex = -1;
    }
}

void CSettingDlg::SetFastDialParam(boost::shared_ptr<Data::Setting> data) {
    for (int i = 321; i <= 332; i++) {
        m_MJPGList.SetUnitFont(i, font_18);
    }
    std::vector<std::pair<std::string, std::string> > diallist = data->speedDials();
    int idx = 0;
    m_edtShowNumber = 0;
    for (std::vector<std::pair<std::string, std::string> >::const_iterator value = diallist.begin(); value != diallist.end(); ++value) {
        std::string name = value->first;
        std::string number = value->second;
        if (name != "" || number != "") {
            m_edtShowNumber++;
            m_edtFastDialName[idx].SetWindowText(Util::StringOp::ToCString(name));
            m_edtFastDialNumber[idx].SetWindowText(Util::StringOp::ToCString(number));
            idx++;
        }
    }
}

//防火墙
void CSettingDlg::OnButtonFireWallOk() {
    KillTimer(2);

    int nSel = m_cmbTime.GetCurSel();
    int nSecond;

    if (nSel > 0) {
        nSecond = nSel * 3600;
    } else {
        nSecond = 1800;
        m_pTempSetting->blockAllTimeDuration(nSecond);
    }

    ::SendMessage(((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->m_pMainDlg->GetSafeHwnd(), WM_TELNOTIFY, 3, 0);
    ((CMultimediaPhoneDlg *)(theApp.m_pMainWnd))->m_pMainDlg->SetStatusAll(FALSE);
}

void CSettingDlg::BrushWindow(void) {
    SetParameters(m_pTempSetting->firewallType(), (int)m_pTempSetting->blockAllTimeDuration().GetTotalSeconds());
}

void CSettingDlg::SetParameters(int type, int duration) {
    for (int i = 141; i <= 144; i++) {
        m_MJPGList.SetUnitIsDownStatus(i, FALSE);
    }

    switch (type) {
    case 0:
        m_MJPGList.SetUnitIsDownStatus(141, TRUE);
        break;
    case 1:
        m_MJPGList.SetUnitIsDownStatus(142, TRUE);
        break;
    case 2:
        m_MJPGList.SetUnitIsDownStatus(143, TRUE);
        break;
    case 3:
        m_MJPGList.SetUnitIsDownStatus(144, TRUE);
        break;
    default:
        break;
    }
    CString v;
    v.Format(L"%d", duration / 3600);
    m_cmbTime.SelectString(0, (LPCTSTR)v);
}

int CSettingDlg::GetFirewallType() {
    if (!((CMultimediaPhoneDlg *)(theApp.m_pMainWnd))->m_pSettingDlg->m_pTempSetting->isFirewall()) {
        return -1;
    }
    return m_pTempSetting->firewallType();
}

//铃声设置
LRESULT CSettingDlg::OnComboSelect(WPARAM w, LPARAM l) {
    LRESULT result = 0;
    CString s = "";

    CString sFile = my_ring_dir;
    if (w == IDC_COMBOBOX_SETTING_RING || w == IDC_COMBOBOX_SETTING_RINGV) {
        m_cmbRing.GetWindowText(s);
        m_pTempSetting->phoneCallRingFilename_ = Util::StringOp::FromCString(sFile + s);
    } else if (w == IDC_COMBOBOX_SETTING_SMSRING || w == IDC_COMBOBOX_SETTING_SMSRINGV) {
          m_cmbSMSRing.GetWindowText(s);
          m_pTempSetting->smsRingFilename_ = Util::StringOp::FromCString(sFile + s);
    } else if (w == IDC_COMBOBOX_SETTING_SPECRING || w == IDC_COMBOBOX_SETTING_SPECRINGV) {
          m_cmbSpecRing.GetWindowText(s);
          m_pTempSetting->specodeRingFilename_ = Util::StringOp::FromCString(sFile + s);
    }
    return result;
}

void CSettingDlg::SetRingLst(TCHAR* dir) {
    m_cmbRing.ResetContent();
    m_cmbSMSRing.ResetContent();
    m_cmbSpecRing.ResetContent();

    CString sDir = my_ring_dir ;
    sDir += "*.*";
    WIN32_FIND_DATA FindFileData;           //查找文件时要使用的数据结构
    HANDLE hFind = INVALID_HANDLE_VALUE;    //定义查找句柄

    //sDir += "*.*";
    hFind = FindFirstFile(sDir, &FindFileData);//使用FindFirstFile函数来开始文件查找

    if (hFind == INVALID_HANDLE_VALUE) {
        return;
    } else {
        char filename[128];
          //  printf ("First file name is %s\n", FindFileData.cFileName);
        size_t convertCount = 0;
        int i = wcstombs_s(&convertCount, filename, 128, FindFileData.cFileName, 128);
          filename[i] = '\0';
        if (strstr(filename, ".mp3") || strstr(filename, ".MP3") || strstr(filename, ".wav") || strstr(filename, ".WAV")) {
            m_cmbRing.AddString(FindFileData.cFileName);
            m_cmbSMSRing.AddString(FindFileData.cFileName);
            m_cmbSpecRing.AddString(FindFileData.cFileName);
        } else if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
        }

          //以下是循环使用FindNextFile函数来查找文件
        while (FindNextFile(hFind, &FindFileData) != 0) {
            //printf ("Next file name is %s\n", FindFileData.cFileName);
            size_t convertCount = 0;
            i = wcstombs_s(&convertCount, filename, 128, FindFileData.cFileName, 128);
            filename[i] = '\0';
            if (strstr(filename, ".mp3") || strstr(filename, ".MP3") || strstr(filename, ".wav") || strstr(filename, ".WAV")) {
                m_cmbRing.AddString(FindFileData.cFileName);
                m_cmbSMSRing.AddString(FindFileData.cFileName);
                m_cmbSpecRing.AddString(FindFileData.cFileName);
            } else if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                  //  m_ringList.InsertItem(ncount++, FindFileData.cFileName, 2);
            }
        }

        DWORD dwError = GetLastError();
        if (dwError == ERROR_NO_MORE_FILES) {
            FindClose(hFind);//关闭查找句柄
        } else {
            //printf ("FindNextFile error. Error is %u\n", dwError);
            return;
        }
    }

    return;
}

void CSettingDlg::SetRingSetParam(boost::shared_ptr<Data::Setting> data) {
    extern int FindFileEx(CString s, CString & sPath, CString & sFilename);
    CString s;
    CString filename;
    CString path;
    int nVolume;

    SetRingLst(L"");

    if (data->isPhoneCallRing_) {
        m_MJPGList.SetUnitIsDownStatus(111, TRUE);
    } else {
        m_MJPGList.SetUnitIsDownStatus(111, FALSE);
    }
    s = Util::StringOp::ToCString(data->phoneCallRingFilename_);
    filename = "";
    FindFileEx(s, path, filename);
    m_cmbRing.SetWindowText_(filename);
    nVolume = data->phoneCallRingVolume_;
    ChangeVolume(nVolume + 1110 - 1, 0);

    if (data->isSmsRing_) {
        m_MJPGList.SetUnitIsDownStatus(112, TRUE);
    } else {
        m_MJPGList.SetUnitIsDownStatus(112, FALSE);
    }
    s = Util::StringOp::ToCString(data->smsRingFilename_);
    filename = "";
    FindFileEx(s, path, filename);
    m_cmbSMSRing.SetWindowText_(filename);
    nVolume = data->smsRingVolume_;
    ChangeVolume(nVolume + 1120 - 1, 1);

    if (data->isSpecodeRing_) {
        m_MJPGList.SetUnitIsDownStatus(113, TRUE);
    } else {
        m_MJPGList.SetUnitIsDownStatus(113, FALSE);
    }
    s = Util::StringOp::ToCString(data->specodeRingFilename_);
    filename = "";
    FindFileEx(s, path, filename);
    m_cmbSpecRing.SetWindowText_(filename);
    nVolume = data->specodeRingVolume_;
    ChangeVolume(nVolume + 1130 - 1, 2);
}

void CSettingDlg::ChangeVolume(int w, const int type) {
    for (int i = 0; i < 10; i++) {
        int index = 0;
        if (0 == type) {
            index = i + 1110;
        } else if (1 == type) {
            index = i + 1120;
        } else if (2 == type) {
            index = i + 1130;
        }
        if (index <= w) {
            m_MJPGList.SetUnitIsDownStatus(index, TRUE);
        } else {
            m_MJPGList.SetUnitIsDownStatus(index, FALSE);
        }
    }
    m_MJPGList.Invalidate();
}

//特服号
/*void CSettingDlg::OnButtonSpacodeOk()
{
if(m_pTempSetting)
{
int i = 0;
CString s;
m_edtFastDialNumber[i++].GetWindowText(s);
m_pTempSetting->speCode1_ = Util::StringOp::FromCString(s);
m_edtFastDialNumber[i++].GetWindowText(s);
m_pTempSetting->speCode2_ = Util::StringOp::FromCString(s);
m_edtFastDialNumber[i++].GetWindowText(s);
m_pTempSetting->speCode3_ = Util::StringOp::FromCString(s);
m_edtFastDialNumber[i++].GetWindowText(s);
m_pTempSetting->speCode4_ = Util::StringOp::FromCString(s);
m_edtFastDialNumber[i++].GetWindowText(s);
m_pTempSetting->speCode5_ = Util::StringOp::FromCString(s);
m_edtFastDialNumber[i++].GetWindowText(s);
m_pTempSetting->speCode6_ = Util::StringOp::FromCString(s);
m_edtFastDialNumber[i++].GetWindowText(s);
m_pTempSetting->speCode7_ = Util::StringOp::FromCString(s);
m_edtFastDialNumber[i++].GetWindowText(s);
m_pTempSetting->speCode8_ = Util::StringOp::FromCString(s);
}

SipShowIM(SIPF_OFF);

ShowWindow_(FALSE);
SipShowIM(SIPF_OFF);
}

void CSettingDlg::SetSpecCodeParam(boost::shared_ptr<Data::Setting> data)
{
int idx = 0;
std::string str = data->speCode1_;
m_edtFastDialNumber[idx++].SetWindowText(Util::StringOp::ToCString(str));
str = data->speCode2_;
m_edtFastDialNumber[idx++].SetWindowText(Util::StringOp::ToCString(str));
str = data->speCode3_;
m_edtFastDialNumber[idx++].SetWindowText(Util::StringOp::ToCString(str));
str = data->speCode4_;
m_edtFastDialNumber[idx++].SetWindowText(Util::StringOp::ToCString(str));
str = data->speCode5_;
m_edtFastDialNumber[idx++].SetWindowText(Util::StringOp::ToCString(str));
str = data->speCode6_;
m_edtFastDialNumber[idx++].SetWindowText(Util::StringOp::ToCString(str));
str = data->speCode7_;
m_edtFastDialNumber[idx++].SetWindowText(Util::StringOp::ToCString(str));
str = data->speCode8_;
m_edtFastDialNumber[idx++].SetWindowText(Util::StringOp::ToCString(str));
}
*/
LRESULT CSettingDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) {
    // TODO: Add your specialized code here and/or call the base class
    int count = 0;
    CString text;
    switch (message) {
    case WM_DELETESELITEM:
        if (1 == m_iDeleteType) {
            SetSettingOK();
        } else if (2 == m_iDeleteType)      //恢复成默认设置
        {
            SetSettingDefault();
        } else if (3 == m_iDeleteType) {
            DeleteFastDialNum(m_pTempSetting);
        }
        break;
    case WM_SETTINGPASSWORD:
        if (lParam == SETTINGSUPPER_PASSWORD) {
            m_pTempSetting->adminPassword(Util::StringOp::FromInt(wParam));
        } else if (lParam == SETTINGSCREEN_PASSWORD) {
              m_pTempSetting->screenSaverPassword(Util::StringOp::FromInt(wParam));
        }
        break;
    }   
    return CDialog::WindowProc(message, wParam, lParam);
}

void CSettingDlg::SetSettingDefault() {
    int id = m_pTempSetting->id();
    Data::SettingType type = m_pTempSetting->type();
    m_pTempSetting = Data::Setting::GetDefaultConfig();
    m_pTempSetting->id(id);
    m_pTempSetting->type(type);
    m_pTempSetting->Update();
    IniCtrlData();
    ShowConfigItems();

    if (((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->m_pSettingDlg->m_bLogin) {
        ((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->m_pMainDlg->m_MJPGList.SetUnitIsDownStatus(170, FALSE);
        ((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->m_pSettingDlg->m_bLogin = FALSE;
    }
}

void CSettingDlg::UpdateTips(void) {
    /*
    m_cmbtip.ResetContent();
    if (Data::SoundSegment::GetDataCount("type = 2 AND isTip = 1") > 0)
    {
    std::vector<boost::shared_ptr<Data::SoundSegment> > sounds = Data::SoundSegment::GetFromDatabase("type = 2 AND isTip = 1");
    for (int i = 0; i < sounds.size(); ++i)
    {
    int pos = sounds[i]->filename().rfind('\\');
    m_cmbtip.AddString(Util::StringOp::ToCString(sounds[i]->filename().substr(pos + 1)));
    }
    int pos = m_pTempSetting->defaultTipFilename().rfind('\\');
    CString s = Util::StringOp::ToCString(m_pTempSetting->defaultTipFilename().substr(pos + 1));
    int index = m_cmbtip.SelectString(0, s);
    if (index == CB_ERR)
    {
    m_cmbtip.SetCurSel(-1);
    // m_pTempSetting->defaultTipFilename("");
    // m_pTempSetting->Update();
    }
    }
    */
}

void CSettingDlg::OnPaint() {
    //   TODO:   Add   your   message   handler   code   here   
    CPaintDC dc(this);
}

void CSettingDlg::OnTimer(UINT nIDEvent) {
    // TODO: Add your message handler code here and/or call default
    if (nIDEvent == 0x11) {
        KillTimer(0x11);
        ShowConfigItems();
    } else if (2 == nIDEvent) {
          KillTimer(nIDEvent);
          ((CMultimediaPhoneDlg *)(theApp.m_pMainWnd))->m_pSettingDlg->m_pTempSetting->isFirewall(false);
          ((CMultimediaPhoneDlg *)(theApp.m_pMainWnd))->m_pSettingDlg->m_pTempSetting->Update();
          ::SendMessage(((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->m_pMainDlg->GetSafeHwnd(), WM_FIRENOTIFY, 0, 0);
    }
    CDialog::OnTimer(nIDEvent);
}
