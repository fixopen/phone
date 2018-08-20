// SettingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SettingDlg.h"
#include "TestDlg.h"
#include "FireWallDlg.h"
//#include "Calibrui.h"
#include "../MultimediaPhone.h"
#include "../MultimediaPhoneDlg.h"
#include "../Pblmember.h"
#include "../Data/LanguageResource.h"
#include "../Data/SkinStyle.h"
#include "../Util/ScanDir.h"

#define new DEBUG_NEW
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern BOOL DetectDIR(TCHAR *sDir);

namespace View {
    extern BOOL _test_call;
}

namespace View {
    /////////////////////////////////////////////////////////////////////////////
    // CSettingDlg dialog
    CString s_VerSionTitle = _T("2.37090207");
    CString s_VerSion = _T("\
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
                           \t5.手写笔迹不圆滑\r\n");

    CSettingDlg::CSettingDlg(CWnd* pParent /*=NULL*/)
        : CDialog(CSettingDlg::IDD, pParent)
    {
        //{{AFX_DATA_INIT(CSettingDlg)
        // NOTE: the ClassWizard will add member initialization here
        //}}AFX_DATA_INIT
        m_bLogin = TRUE;
    }


    void CSettingDlg::DoDataExchange(CDataExchange* pDX)
    {
        CDialog::DoDataExchange(pDX);
        //{{AFX_DATA_MAP(CSettingDlg)
        // NOTE: the ClassWizard will add DDX and DDV calls here
        //}}AFX_DATA_MAP
    }


#define IDC_SETTING_BACKUP				9010
#define IDC_SETTING_RESTORE				9011
#define IDC_SETTING_CMBSELECT			9012
#define IDC_SETTING_DELETE				9013
#define IDC_SETTING_ALLSELECT			9014
#define IDC_SETTING_CLEARSELECT			9015
#define IDC_SETTING_IN					9016
#define IDC_SETTING_OUT					9017
#define IDC_SETTING_LSTLOCAL			9018
#define IDC_SETTING_LSTUSB				9019

    LRESULT CSettingDlg::OnListCtrlClick(WPARAM w, LPARAM l)
    {
        LRESULT ret;
        if(w == IDC_LIST_SETTING_TYPE)
            OnClickListType(NULL, &ret);
        else if(w == IDC_SETTING_LSTRING)
            OnRingLst(NULL, &ret);
        return ret;
    }

    BEGIN_MESSAGE_MAP(CSettingDlg, CDialog)
        //{{AFX_MSG_MAP(CSettingDlg)
        ON_WM_TIMER()
        //}}AFX_MSG_MAP
        ON_MESSAGE(WM_LISTCTRL_CLICK, OnListCtrlClick)
        ON_NOTIFY(NM_CLICK, IDC_LIST_SETTING_TYPE, OnClickListType)
        ON_BN_CLICKED(IDC_BUTTON_SETTING_OK, OnButtonSettingOk)
        ON_BN_CLICKED(IDC_BUTTON_SETTING_CANCEL, OnButtonSettingCancel)
        ON_BN_CLICKED(IDC_BUTTON_SETTING_DEFAULT, OnButtonSettingDefault)
        ON_BN_CLICKED(IDC_BUTTON_SETTING_RECORD, OnButtonSettingRecord)
        ON_BN_CLICKED(IDC_BUTTON_SETTING_DEFINEFASTDAIL,OnButtonSettingDefineFastDial)
        ON_BN_CLICKED(IDC_BUTTON_SETTING_ADJUSTTOUCH, OnAdjustTouchPanel)
        ON_BN_CLICKED(IDC_BUTTON_SETTING_SETPASSWORD, OnSettingSaveScreenPassword)

        ON_BN_CLICKED(IDC_BUTTON_SETTING_CLEARCONTACT, OnSettingClearContact)
        ON_BN_CLICKED(IDC_BUTTON_SETTING_CLEARCONTACTINFO, OnSettingClearContactInfo)
        ON_BN_CLICKED(IDC_BUTTON_SETTING_CLEARSOUND, OnSettingClearSound)
        ON_BN_CLICKED(IDC_BUTTON_SETTING_CLEARINFO, OnSettingClearInfo)

        ON_BN_CLICKED(IDC_SETTING_BACKUP, OnBackup)
        ON_BN_CLICKED(IDC_SETTING_RESTORE, OnRestore)
        ON_BN_CLICKED(IDC_SETTING_OUT, OnFileOut)
        ON_BN_CLICKED(IDC_SETTING_RINGSELECT, OnRingSelect)
        ON_NOTIFY(NM_CLICK, IDC_SETTING_LSTRING, OnRingLst)

        ON_MESSAGE(WM_SETTINGPASSWORD, OnSetSaveScreenPassWordOK)
        ON_MESSAGE(WM_STATIC_CLICK, OnStaticClick)
        ON_MESSAGE(WM_DELETESELITEM, OnDeleteTipOk)
        ON_MESSAGE(WM_CLICKMJPG_TOAPP, OnClickMJPG)
        ON_MESSAGE(WM_STOPTRYRING, OnStopTryRing)
        ON_WM_PAINT()
    END_MESSAGE_MAP()

    void CSettingDlg::SetShowTimer()
    {
        m_cmbRing.ShowWindow(SW_HIDE);

        //tel
        m_edtLocalAreaNumber.ShowWindow(FALSE);
        m_chbEnableOutLine.ShowWindow(FALSE);
        m_edtOutLine.ShowWindow(FALSE);
        m_chbEnableIPDial.ShowWindow(FALSE);
        m_edtIPDial.ShowWindow(FALSE);
        m_chbEnableRing.ShowWindow(FALSE);

        m_cmbRing.ShowWindow(FALSE);


        m_chbEnableAutoLeaveWord.ShowWindow(FALSE);
        m_cmbRingTimes.ShowWindow(FALSE);

        //browser
        m_rdoConnectAuto.ShowWindow(FALSE);
        m_rdoConnectADSL.ShowWindow(FALSE);
        m_edtADSLName.ShowWindow(FALSE);
        m_edtADSLPassword.ShowWindow(FALSE);	
        m_rdoGetIPDHCP.ShowWindow(FALSE);
        m_rdoGetIPStatic.ShowWindow(FALSE);

        for (int i = 0; i < 4; ++i)
        {
            m_edtIP[i].ShowWindow(FALSE);
            m_edtMask[i].ShowWindow(FALSE);
            m_edtGateway[i].ShowWindow(FALSE);
            m_edtDNS[i].ShowWindow(FALSE);
        }

        //systom
        m_chbEnnabeScreenProtect.ShowWindow(FALSE);
        m_rdoTimeScreen.ShowWindow(FALSE);
        m_rdoImageScreen.ShowWindow(FALSE);
        m_cmbWaitTime.ShowWindow(FALSE);
        m_cmbRingVolume.ShowWindow(FALSE);
        m_cmbSystemVolume.ShowWindow(FALSE);
        m_chbEnablePassword.ShowWindow(FALSE);
        m_cmbSoundSavePath.ShowWindow(FALSE);
        m_dtTime.ShowWindow(FALSE);
        m_dtDate.ShowWindow(FALSE);

        //information
        m_sticSystomTitle.ShowWindow(FALSE);
        m_sticSoftwareVersion.ShowWindow(FALSE);
        m_editVersion.ShowWindow(FALSE);
        m_sticMemorySize.ShowWindow(FALSE);
        m_sticStorageSize.ShowWindow(FALSE);
        m_sticContactSize.ShowWindow(FALSE);
        m_sticContactInfoSize.ShowWindow(FALSE);
        m_sticSoundSize.ShowWindow(FALSE);
        m_sticInformationSize.ShowWindow(FALSE);
        m_sticVersionRight.ShowWindow(FALSE);

        //blacklight
        m_chbContrlBlackLight.ShowWindow(FALSE);
        m_cmbContrlBlackLightWaitTime.ShowWindow(FALSE);
        m_chbNightContrlBlackLight.ShowWindow(FALSE);
        m_dtNightContrlBlackLightStartTime.ShowWindow(FALSE);
        m_dtNightContrlBlackLightEndTime.ShowWindow(FALSE);

        SetTimer(1, 5, NULL);
    }

    LRESULT CSettingDlg::OnStopTryRing(WPARAM w, LPARAM l)
    {
        LRESULT result = 0;
        if (m_MJPGList.GetUnitIsDownStatus(10))
        {
            ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->StartRing(L"");
            m_MJPGList.SetUnitIsDownStatus(10, FALSE);
            m_MJPGList.SetUnitIsShow(10, TRUE);
            ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pMainDlg->m_mainMp3Dlg_->SendMessage(WM_OUTEVENT, 0, 1);
        }
        return result;
    }

    /////////////////////////////////////////////////////////////////////////////
    // CSettingDlg message handlers
    LRESULT CSettingDlg::OnClickMJPG(WPARAM w, LPARAM l)
    {
        LRESULT result = 0;
        if (m_uiType == 0 && w != 110)
        {
            OnStopTryRing(0, 0);
        }
        switch (w)
        {
        case 1:
            m_MJPGList.SetCurrentLinkFile(_T(".\\adv\\mjpg\\k1\\中文\\系统设置1.xml"));
            m_MJPGList.Invalidate();
            m_uiType = 0;
            SetShowTimer();
            //ShowConfigItems();
            break;
        case 2:
            m_MJPGList.SetCurrentLinkFile(_T(".\\adv\\mjpg\\k1\\中文\\系统设置2.xml"));
            m_MJPGList.Invalidate();
            m_uiType = 1;
            SetShowTimer();
            //ShowConfigItems();
            break;
        case 3:
            m_MJPGList.SetCurrentLinkFile(_T(".\\adv\\mjpg\\k1\\中文\\系统设置3.xml"));
            m_MJPGList.Invalidate();
            m_uiType = 2;
            SetShowTimer();
            //ShowConfigItems();
            break;
        case 4:
            m_MJPGList.SetCurrentLinkFile(_T(".\\adv\\mjpg\\k1\\中文\\系统设置4.xml"));
            m_MJPGList.Invalidate();
            m_uiType = 3;
            SetShowTimer();
            //ShowConfigItems();
            break;
        case 5:
            m_MJPGList.SetCurrentLinkFile(_T(".\\adv\\mjpg\\k1\\中文\\系统设置5.xml"));
            m_MJPGList.Invalidate();
            m_uiType = 4;
            //ShowConfigItems();
            SetShowTimer();
            break;
        case 9:
            {
                m_MJPGList.SetCurrentLinkFile(_T(".\\adv\\mjpg\\k1\\中文\\系统设置6.xml"));

                for (int i = 0; i < 8; ++i)
                {
                    m_MJPGList.SetUnitIsDownStatus(600+i, FALSE);
                }
                m_MJPGList.SetUnitIsDownStatus(600+m_pSetting->blackLightValue(), TRUE);

                m_MJPGList.Invalidate();
                m_uiType = 5;
                //ShowConfigItems();
                SetShowTimer();
            }
            break;

        case 6:
            OnButtonSettingOk();
            break;
        case 7:
            OnButtonSettingDefault();
            break;
        case 8:
            OnButtonSettingCancel();
            break;
        case 101:
            OnButtonSettingDefineFastDial();
            break;
        case 102:
            break;
        case 110:
            {
                if (!m_MJPGList.GetUnitIsDownStatus(10))
                {
                    CString str;
                    m_cmbRing.GetWindowText(str);
                    if (str == _T(""))
                        break;
                    CString s = m_ringDir; 
                    s += _T("/") + str;

                    ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pMainDlg->m_mainMp3Dlg_->OnTimer(1002); //SendMessage(WM_OUTEVENT, 0, 0);

                    ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->SetMsgWnd(this);
                    ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->StartRing((LPTSTR)(LPCTSTR)s, 1);
                    m_MJPGList.SetUnitIsDownStatus(10, TRUE);
                    m_MJPGList.SetUnitIsShow(10, TRUE);
                }
                else
                {
                    OnStopTryRing(0, 0);
                }
                break;
            }
        case 301:   //屏幕校正
            WinExec("explorer.exe", SW_SHOW);
            //OnAdjustTouchPanel();
            break;
        case 401:
            OnBackup();
            break;
        case 402:
            OnRestore();
            break;
        case 403:
            OnFileOut();
            break;
        case 404:	//MP3导入
            {
                m_copyfileDlg->SetType(mp3_in);
                m_copyfileDlg->ShowWindow(SW_SHOW);
            }
            break;
        case 405:	//电子相册导入
            {
                m_copyfileDlg->SetType(photo_in);
                m_copyfileDlg->ShowWindow(SW_SHOW);
            }
            break;
        case 406:	//振铃铃声导入
            {
                m_copyfileDlg->SetType(bell_in);
                m_copyfileDlg->ShowWindow(SW_SHOW);
            }
            break;
        case 407:	//电话秀导入
            {
                m_copyfileDlg->SetType(show_in);
                m_copyfileDlg->ShowWindow(SW_SHOW);
            }
            break;
        case 408:	//MP3导出
            {
                m_copyfileDlg->SetType(mp3_out);
                m_copyfileDlg->ShowWindow(SW_SHOW);
            }
            break;
        case 409:	//电子相册导出
            {
                m_copyfileDlg->SetType(photo_out);
                m_copyfileDlg->ShowWindow(SW_SHOW);
            }
            break;
        case 410:	//振铃铃声导出
            {
                m_copyfileDlg->SetType(bell_out);
                m_copyfileDlg->ShowWindow(SW_SHOW);
            }
            break;
        case 411:	//电话秀导出
            {
                m_copyfileDlg->SetType(show_out);
                m_copyfileDlg->ShowWindow(SW_SHOW);
            }
            break;
        case 501:
            OnSettingClearContact();
            break;
        case 502:
            OnSettingClearContactInfo();
            break;
        case 503:
            OnSettingClearSound();
            break;
        case 600:		//设置背光级别
        case 601:
        case 602:
        case 603:
        case 604:
        case 605:
        case 606:
        case 607:
            {
                int old_value = m_pSetting->blackLightValue();
                if (old_value + 600 != w)
                {
                    int i = w - 600;
                    m_pSetting->blackLightValue(i);
                    //将屏幕变黑
                    m_MJPGList.SetUnitIsDownStatus(old_value+600, FALSE);
                    m_MJPGList.SetUnitIsDownStatus(w, TRUE);
                    m_MJPGList.Invalidate();
                    int v = 7 - (w - 600);
                    if (v == 0)
                        v = 1;
                    ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->SetBackLight(v);
                }

                break;
            }
        }
        return result;
    }

    BOOL CSettingDlg::OnInitDialog() 
    {
        CDialog::OnInitDialog();

        // TODO: Add extra initialization here
        std::string strTemp = Data::LanguageResource::Get(Data::RI_RECORD_TYPE);
        CString str = Util::StringOp::ToCString(strTemp.c_str());

		m_MJPGList.Create(L"", WS_VISIBLE|WS_CHILD, CRect(0*X_XISHU, 0*Y_XISHU, 800*X_XISHU, 420*Y_XISHU), this);
		m_MJPGList.SetCurrentLinkFile(_T(".\\adv\\mjpg\\k1\\中文\\系统设置1.xml"));
		m_MJPGList.SetMJPGRect(CRect(0*X_XISHU, 0*Y_XISHU, 800*X_XISHU, 420*Y_XISHU));

        m_edtLocalAreaNumber.Create(WS_CHILD|WS_VISIBLE, CRect(355*X_XISHU, 61*Y_XISHU, 608*X_XISHU,100*Y_XISHU), &m_MJPGList, IDC_EDIT_SETTING_AREANO);
        m_edtLocalAreaNumber.SetLimitText(15);
        m_edtLocalAreaNumber.SetLimitDiagital();

        strTemp = Data::LanguageResource::Get(Data::RI_SETTING_OUTLINE);
        str = L"";
        m_chbEnableOutLine.Create(str, WS_CHILD|WS_VISIBLE|BS_CHECKBOX, CRect(166*X_XISHU, 118*Y_XISHU, (166+35)*X_XISHU, (118+32)*Y_XISHU), &m_MJPGList, IDC_CHECK_SETTING_OUTLINE);
        m_chbEnableOutLine.SetIcon(IDI_ICON_CHECK1, CSize(32, 32), IDI_ICON_CHECK0, CSize(32, 32));
        SetButtonDefaultColor(&m_chbEnableOutLine);	
        m_edtOutLine.Create(WS_CHILD|WS_VISIBLE, CRect(355*X_XISHU, 114*Y_XISHU, 608*X_XISHU, 153*Y_XISHU), &m_MJPGList, IDC_EDIT_SETTING_OUTLINE);
        m_edtOutLine.SetLimitText(15);
        m_edtOutLine.SetLimitDiagital();

        strTemp = Data::LanguageResource::Get(Data::RI_SETTING_IPDIAL);
        str = L"";
        m_chbEnableIPDial.Create(str, WS_CHILD|WS_VISIBLE|BS_CHECKBOX, CRect(166*X_XISHU, 168*Y_XISHU, (166+35)*X_XISHU, (168+32)*Y_XISHU), &m_MJPGList, IDC_CHECK_SETTING_IPDIAL);
        m_chbEnableIPDial.SetIcon(IDI_ICON_CHECK1, CSize(32, 32), IDI_ICON_CHECK0, CSize(32, 32));
        SetButtonDefaultColor(&m_chbEnableIPDial);	
        m_edtIPDial.Create(WS_CHILD|WS_VISIBLE|ES_AUTOHSCROLL, CRect(355*X_XISHU, 167*Y_XISHU, 608*X_XISHU, 206*Y_XISHU), &m_MJPGList, IDC_EDIT_SETTING_IPDIAL);
        m_edtIPDial.SetLimitText(64);

        strTemp = Data::LanguageResource::Get(Data::RI_SETTING_ENABLERING);
        str = L"";
        m_chbEnableRing.Create(str, WS_CHILD|WS_VISIBLE|BS_CHECKBOX, CRect(166*X_XISHU, 229*Y_XISHU, (166+35)*X_XISHU, (229+32)*Y_XISHU), &m_MJPGList, IDC_CHECK_SETTING_RING);
        m_chbEnableRing.SetIcon(IDI_ICON_CHECK1, CSize(32, 32), IDI_ICON_CHECK0, CSize(32, 32));
        SetButtonDefaultColor(&m_chbEnableRing);	

        m_cmbRing.Create(WS_CHILD|WS_VISIBLE, CRect(354*X_XISHU,238*Y_XISHU,608*X_XISHU,460*Y_XISHU), &m_MJPGList, IDC_COMBOBOX_SETTING_RING);

        strTemp = Data::LanguageResource::Get(Data::RI_SETTING_ENABLEAUTOLEAVEWORD);
        str = L"";
        m_chbEnableAutoLeaveWord.Create(str, WS_CHILD|WS_VISIBLE|BS_CHECKBOX, CRect(166*X_XISHU, 294*Y_XISHU, (166+35)*X_XISHU, (294+32)*Y_XISHU), &m_MJPGList, IDC_CHECK_SETTING_AUTOLEAVEWORD);
        m_chbEnableAutoLeaveWord.SetIcon(IDI_ICON_CHECK1, CSize(32, 32), IDI_ICON_CHECK0, CSize(32, 32));
        SetButtonDefaultColor(&m_chbEnableAutoLeaveWord);	

        m_cmbRingTimes.Create(WS_CHILD|WS_VISIBLE, CRect(353*X_XISHU, 304*Y_XISHU, 433*X_XISHU, 460*Y_XISHU), &m_MJPGList, IDC_COMBOBOX_SETTING_RINGTIMES);
        m_cmbAutoRecoedeTimes.Create(WS_CHILD|WS_VISIBLE, CRect(530*X_XISHU, 304*Y_XISHU, 610*X_XISHU, 460*Y_XISHU), &m_MJPGList, IDC_COMBOBOX_SETTING_RINGTIMES);

        strTemp = Data::LanguageResource::Get(Data::RI_SETTING_CONNECTAUTO);
        str = L"";
        str = _T("");
        m_rdoConnectAuto.Create(str, WS_CHILD|WS_VISIBLE/*|BS_RADIOBUTTON*/, CRect((182-15)*X_XISHU, (62-4)*Y_XISHU, (214-15)*X_XISHU, (100-4)*Y_XISHU), &m_MJPGList, IDC_RADIO_SETTING_CONNECTAUTO);
        m_rdoConnectAuto.SetColor(RGB(0, 0, 0), RGB(237, 237, 237));//Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
        strTemp = Data::LanguageResource::Get(Data::RI_SETTING_CONNECTMANUAL);
        str = L"";
        str = _T("");
        m_rdoConnectADSL.Create(str, WS_CHILD|WS_VISIBLE/*|BS_RADIOBUTTON*/, CRect((182-15)*X_XISHU, (102-4)*Y_XISHU, (214-15)*X_XISHU, (136-4)*Y_XISHU), &m_MJPGList, IDC_RADIO_SETTINT_CONNECTADSL);
        m_rdoConnectADSL.SetColor(RGB(0, 0, 0), RGB(237, 237, 237));//Data::g_allFramInRectBackRGB[Data::g_skinstyle]);

        CButton *pButton[2];
        pButton[0] = &m_rdoConnectAuto;
        pButton[1] = &m_rdoConnectADSL;

        m_rdoConnectAuto.SetGroupButton(pButton, 2);
        m_rdoConnectADSL.SetGroupButton(pButton, 2);

        m_edtADSLName.Create(ES_AUTOHSCROLL|WS_CHILD|WS_VISIBLE, CRect((295-15)*X_XISHU, (140-4)*Y_XISHU, (433-15)*X_XISHU, (171-4)*Y_XISHU), &m_MJPGList, IDC_EDIT_SETTING_ADSLNAME);
        m_edtADSLName.SetLimitText(15);

        m_edtADSLPassword.Create(WS_CHILD|WS_VISIBLE|ES_AUTOHSCROLL|ES_PASSWORD, CRect((512-15)*X_XISHU, (140-4)*Y_XISHU, (649-15)*X_XISHU, (171-4)*Y_XISHU), &m_MJPGList, IDC_EDIT_SETTING_ADSLPASSWORD);
        m_edtADSLPassword.SetLimitText(15);

        strTemp = Data::LanguageResource::Get(Data::RI_SETTING_USEDYNAMICIP);
        str = L"";
        str = _T("");
        m_rdoGetIPDHCP.Create(str, WS_CHILD|WS_VISIBLE/*|BS_RADIOBUTTON*/, CRect((182-15)*X_XISHU, (174-4)*Y_XISHU, (214-15)*X_XISHU, (208-4)*Y_XISHU), &m_MJPGList, IDC_RADIO_SETTING_DHCP);
        m_rdoGetIPDHCP.SetColor(RGB(0, 0, 0), RGB(237, 237, 237));//Data::g_allFramInRectBackRGB[Data::g_skinstyle]);

        strTemp = Data::LanguageResource::Get(Data::RI_SETTING_USESTATICIIP);
        str = L"";
        str = _T("");
        m_rdoGetIPStatic.Create(str, WS_CHILD|WS_VISIBLE, CRect((182-15)*X_XISHU, (211-4)*Y_XISHU, (214-15)*X_XISHU, (245-4)*Y_XISHU), &m_MJPGList, IDC_RADIO_SETTING_STATIC);
        m_rdoGetIPStatic.SetColor(RGB(0, 0, 0), RGB(237, 237, 237));//Data::g_allFramInRectBackRGB[Data::g_skinstyle]);

        pButton[0] = &m_rdoGetIPDHCP;
        pButton[1] = &m_rdoGetIPStatic;

        m_rdoGetIPDHCP.SetGroupButton(pButton, 2);
        m_rdoGetIPStatic.SetGroupButton(pButton, 2);

        for(int i = 0; i < 4; i++)
        {
            m_edtIP[i].Create(WS_CHILD|WS_VISIBLE, CRect((329+i*58-15)*X_XISHU, (252-4)*Y_XISHU, (375+i*58-15)*X_XISHU, (278-4)*Y_XISHU), &m_MJPGList, IDC_EDIT_SETTING_IP, 26);
            m_edtIP[i].SetLimitText(3);
            m_edtIP[i].SetLimitDiagital();

            m_edtMask[i].Create(WS_CHILD|WS_VISIBLE, CRect((329+i*58-15)*X_XISHU,(288-4)*Y_XISHU, (375+i*58-15)*X_XISHU, (314-4)*Y_XISHU), &m_MJPGList, IDC_EDIT_SETTING_MASK, 26);
            m_edtMask[i].SetLimitText(3);
            m_edtMask[i].SetLimitDiagital();

            m_edtGateway[i].Create(WS_CHILD|WS_VISIBLE, CRect((329+i*58-15)*X_XISHU, (324-4)*Y_XISHU, (375+i*58-15)*X_XISHU, (350-4)*Y_XISHU), &m_MJPGList, IDC_EDIT_SETTING_GATEWAY, 26);
            m_edtGateway[i].SetLimitText(3);
            m_edtGateway[i].SetLimitDiagital();

            m_edtDNS[i].Create(WS_CHILD|WS_VISIBLE, CRect((329+i*58-15)*X_XISHU, (360-4)*Y_XISHU, (375+i*58-15)*X_XISHU, (386-4)*Y_XISHU), &m_MJPGList, IDC_EDIT_SETTING_DNS, 26);
            m_edtDNS[i].SetLimitText(3);
            m_edtDNS[i].SetLimitDiagital();
        }

        strTemp = Data::LanguageResource::Get(Data::RI_SETTING_ENABLESP);
        str = L"";
        m_chbEnnabeScreenProtect.Create(str, WS_CHILD|WS_VISIBLE|BS_CHECKBOX, CRect((182-44)*X_XISHU, (68-4)*Y_XISHU, (217-44)*X_XISHU, (100-4)*Y_XISHU), &m_MJPGList, IDC_CHECK_SETTING_SCREENPROTECT);
        m_chbEnnabeScreenProtect.SetIcon(IDI_ICON_CHECK1, CSize(32, 32), IDI_ICON_CHECK0, CSize(32, 32));
        SetButtonDefaultColor(&m_chbEnnabeScreenProtect);	

        strTemp = Data::LanguageResource::Get(Data::RI_SETTING_TIMESP);
        str = L"";
        m_rdoTimeScreen.Create(str, WS_CHILD|WS_VISIBLE/*|BS_CHECKBOX*/, CRect((312-37)*X_XISHU, (101-4)*Y_XISHU, (344-37)*X_XISHU, (135-4)*Y_XISHU), &m_MJPGList, IDC_RADIO_SETTING_TIMESCREEN);
        m_rdoTimeScreen.SetColor(RGB(0, 0, 0), RGB(237, 237, 237));//Data::g_allFramInRectBackRGB[Data::g_skinstyle]);

        strTemp = Data::LanguageResource::Get(Data::RI_SETTING_IMAGESP);
        str = L"";
        m_rdoImageScreen.Create(str, WS_CHILD|WS_VISIBLE/*|BS_CHECKBOX*/, CRect((442-37)*X_XISHU, (101-4)*Y_XISHU, (474-37)*X_XISHU, (135-4)*Y_XISHU), &m_MJPGList, IDC_RADIO_SETTING_IMAGESCREEN);
        m_rdoImageScreen.SetColor(RGB(0, 0, 0), RGB(237, 237, 237));//Data::g_allFramInRectBackRGB[Data::g_skinstyle]);

        pButton[0] = &m_rdoTimeScreen;
        pButton[1] = &m_rdoImageScreen;

        m_rdoTimeScreen.SetGroupButton(pButton, 2);
        m_rdoImageScreen.SetGroupButton(pButton, 2);

        m_cmbWaitTime.Create(WS_CHILD|WS_VISIBLE, CRect((338-37)*X_XISHU, (136-4)*Y_XISHU, (476-37)*X_XISHU, (410-4)*Y_XISHU), &m_MJPGList, IDC_COMBOBOX_SETTING_WAITTIME);

        m_cmbRingVolume.Create(WS_CHILD|WS_VISIBLE, CRect((338-37)*X_XISHU, (221-4)*Y_XISHU, (476-37)*X_XISHU, (410-4)*Y_XISHU), &m_MJPGList, 0xFFFF);
        m_cmbSystemVolume.Create(WS_CHILD|WS_VISIBLE, CRect((338-37)*X_XISHU, (263-4)*Y_XISHU, (476-37)*X_XISHU, 460*Y_XISHU), &m_MJPGList, 0xFFFF);

        strTemp = Data::LanguageResource::Get(Data::RI_SETTING_ENABLEPASSWORD);
        str = L"";
        m_chbEnablePassword.Create(str, WS_CHILD|WS_VISIBLE|BS_CHECKBOX, CRect((218-37)*X_XISHU, (176-4)*Y_XISHU, (253-37)*X_XISHU, (208-4)*Y_XISHU), &m_MJPGList, IDC_CHECK_SETTING_SPPASSWORD);
        m_chbEnablePassword.SetIcon(IDI_ICON_CHECK1, CSize(32, 32), IDI_ICON_CHECK0, CSize(32, 32));
        SetButtonDefaultColor(&m_chbEnablePassword);	

        m_cmbSoundSavePath.Create(WS_CHILD, CRect((338-37)*X_XISHU, (178-4)*Y_XISHU, (476-37)*X_XISHU, (410-4)*Y_XISHU), &m_MJPGList, IDC_COMBOBOX_SETTING_SOUNDSAVEPATH);

        m_dtDate.Create(WS_VISIBLE|WS_CHILD|DTS_SHORTDATEFORMAT, CRect((319-13)*X_XISHU, (317-4)*Y_XISHU, (319+184-13)*X_XISHU, (317+32-4)*Y_XISHU), &m_MJPGList, IDC_SETTING_DATE, 28, CDateCtrl::modeDate);
        m_dtTime.Create(WS_VISIBLE|WS_CHILD/*|DTS_TIMEFORMAT*/, CRect((521-13)*X_XISHU, (317-4)*Y_XISHU, (521+154-13)*X_XISHU, (317+32-4)*Y_XISHU), &m_MJPGList, IDC_SETTING_TIME, 28,  CDateCtrl::modeTime);

        m_Font.CreateFont(
            28,                        // nHeight
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

        CTime time1 = CTime(2007, 1, 1, 0, 0, 0);
        CTime time2 = CTime(2031, 1, 1, 0, 0, 0);
        //	m_dtDate.SetRange(&time1, &time2);
        GetLocalTime(&m_curtime);
        m_dtDate.SetTime(m_curtime);
        m_dtTime.SetTime(m_curtime);

        strTemp = Data::LanguageResource::Get(Data::RI_SETTING_SYSTOMTITLE);
        str = L"";
        str = _T("");
        m_sticSystomTitle.Create(str, WS_CHILD|WS_VISIBLE, CRect((222-15)*X_XISHU, (76-4)*Y_XISHU, (510-15)*X_XISHU, (102-4)*Y_XISHU), &m_MJPGList);
        m_sticSystomTitle.SetColor(RGB(0, 0, 0), RGB(237, 237, 237));//Data::g_allFramInRectBackRGB[Data::g_skinstyle]);

        m_sticSoftwareVersion.Create(s_VerSionTitle, WS_CHILD|WS_VISIBLE, CRect((328-15)*X_XISHU, (108-4)*Y_XISHU, (450-15)*X_XISHU, (134-4)*Y_XISHU), &m_MJPGList, IDC_SETTING_VERSION);
        m_sticSoftwareVersion.SetColor(RGB(255, 0, 0), RGB(237, 237, 237));//Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
        m_sticSoftwareVersion.SetUnderLine(TRUE);

        m_editVersion.Create(WS_VISIBLE|WS_CHILD|WS_BORDER|ES_MULTILINE | ES_WANTRETURN | WS_VSCROLL, CRect((170-15)*X_XISHU, (135-4)*Y_XISHU, (170+512-15)*X_XISHU, (135+282-4)*Y_XISHU), &m_MJPGList, IDC_SETTING_EDITVERSION );
        m_editVersion.ShowWindow(SW_HIDE);
        m_editVersion.SetWindowText(s_VerSion);

        str = "";
        m_sticMemorySize.Create(str, WS_CHILD|WS_VISIBLE, CRect((328-15)*X_XISHU, (144-4)*Y_XISHU, (650-15)*X_XISHU, (170-4)*Y_XISHU), &m_MJPGList);
        m_sticMemorySize.SetColor(RGB(0, 0, 0), RGB(237, 237, 237));//Data::g_allFramInRectBackRGB[Data::g_skinstyle]);

        str = "";
        m_sticStorageSize.Create(str, WS_CHILD|WS_VISIBLE, CRect((328-15)*X_XISHU, (180-4)*Y_XISHU, (650-15)*X_XISHU, (206-4)*Y_XISHU), &m_MJPGList);
        m_sticStorageSize.SetColor(RGB(0, 0, 0), RGB(237, 237, 237));//Data::g_allFramInRectBackRGB[Data::g_skinstyle]);

        str = "";
        m_sticContactSize.Create(str, WS_CHILD|WS_VISIBLE, CRect((360-15)*X_XISHU, (234-4)*Y_XISHU, (440-15)*X_XISHU, (260-4)*Y_XISHU), &m_MJPGList);
        m_sticContactSize.SetColor(RGB(0, 0, 0), RGB(237, 237, 237));//Data::g_allFramInRectBackRGB[Data::g_skinstyle]);

        str = "";
        m_sticContactInfoSize.Create(str, WS_CHILD|WS_VISIBLE, CRect((360-15)*X_XISHU, (274-4)*Y_XISHU, (440-15)*X_XISHU, (300-4)*Y_XISHU), &m_MJPGList);
        m_sticContactInfoSize.SetColor(RGB(0, 0, 0), RGB(237, 237, 237));//Data::g_allFramInRectBackRGB[Data::g_skinstyle]);

        str = "";
        m_sticSoundSize.Create(str, WS_CHILD|WS_VISIBLE, CRect((360-15)*X_XISHU, (314-4)*Y_XISHU, (440-15)*X_XISHU, (340-4)*Y_XISHU), &m_MJPGList);
        m_sticSoundSize.SetColor(RGB(0, 0, 0), RGB(237, 237, 237));//Data::g_allFramInRectBackRGB[Data::g_skinstyle]);

        str = "";
        m_sticInformationSize.Create(str, WS_CHILD|WS_VISIBLE, CRect((194-15)*X_XISHU, (164-4)*Y_XISHU, (270-15)*X_XISHU, (180-4)*Y_XISHU), &m_MJPGList);
        m_sticInformationSize.SetColor(RGB(0, 0, 0), RGB(237, 237, 237));//Data::g_allFramInRectBackRGB[Data::g_skinstyle]);

        str = _T("");
        m_sticVersionRight.Create(str, WS_CHILD|WS_VISIBLE, CRect(0, 0, 0, 0), &m_MJPGList);
        m_sticVersionRight.SetColor(RGB(0, 0, 0), RGB(237, 237, 237));//Data::g_allFramInRectBackRGB[Data::g_skinstyle]);

        m_chbContrlBlackLight.Create(str, WS_CHILD|WS_VISIBLE|BS_CHECKBOX, CRect(153*X_XISHU, 169*Y_XISHU, (153+35)*X_XISHU, (169+32)*Y_XISHU), &m_MJPGList, IDC_CHECK_SETTING_SCREENPROTECT);
        m_chbContrlBlackLight.SetIcon(IDI_ICON_CHECK1, CSize(32, 32), IDI_ICON_CHECK0, CSize(32, 32));
        SetButtonDefaultColor(&m_chbContrlBlackLight);	

        m_chbNightContrlBlackLight.Create(str, WS_CHILD|WS_VISIBLE|BS_CHECKBOX, CRect(153*X_XISHU, 293*Y_XISHU, (153+35)*X_XISHU, (293+32)*Y_XISHU), &m_MJPGList, IDC_CHECK_SETTING_SCREENPROTECT);
        m_chbNightContrlBlackLight.SetIcon(IDI_ICON_CHECK1, CSize(32, 32), IDI_ICON_CHECK0, CSize(32, 32));
        SetButtonDefaultColor(&m_chbNightContrlBlackLight);

        m_cmbContrlBlackLightWaitTime.Create(WS_CHILD|WS_VISIBLE, CRect(288*X_XISHU,206*Y_XISHU,431*X_XISHU,410*Y_XISHU), &m_MJPGList, IDC_COMBOBOX_SETTING_RINGTIMES);

        m_dtNightContrlBlackLightStartTime.Create(WS_VISIBLE|WS_CHILD, CRect(242*X_XISHU, 335*Y_XISHU, (242+154)*X_XISHU, (335+32)*Y_XISHU), &m_MJPGList, IDC_SETTING_TIME, 28, CDateCtrl::modeTime);
        m_dtNightContrlBlackLightEndTime.Create(WS_VISIBLE|WS_CHILD, CRect(444*X_XISHU, 334*Y_XISHU, (444+154)*X_XISHU, (334+32)*Y_XISHU), &m_MJPGList, IDC_SETTING_TIME, 28, CDateCtrl::modeTime);

        m_uiType = 0;

        m_pFastDialsDlg = new CFastDialsDlg(this);
        m_pFastDialsDlg->Create(CFastDialsDlg::IDD);

        m_passwordDlg = new CPasswordDlg(this);
        m_passwordDlg->Create(CPasswordDlg::IDD);

        m_copyfileDlg = new copyfileDlg(this);
        m_copyfileDlg->Create(copyfileDlg::IDD);

        m_pSetting = Data::Setting::GetCurrentConfig();

        ShowConfigItems();
        IniCtrlData();

        if(!(((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->SetNightControlBackLightTimer()))
        {
            int v = m_pSetting->blackLightValue();
            v = 7-v;
            ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->SetBackLight(v);
        }
        else
        {
            ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->SetBackLight(0);    //关闭背光
        }

        return TRUE;  // return TRUE unless you set the focus to a control
        // EXCEPTION: OCX Property Pages should return FALSE
    }


    void CSettingDlg::SetButtonDefaultColor(CCEButtonST* button)
    {
        button->SetColor(CCEButtonST::BTNST_COLOR_BK_IN,  RGB(237, 237, 237));//Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
        button->SetColor(CCEButtonST::BTNST_COLOR_BK_OUT,  RGB(237, 237, 237));//Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
        button->SetColor(CCEButtonST::BTNST_COLOR_BK_FOCUS,  RGB(237, 237, 237));//Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
    }

    void CSettingDlg::SetButtonSelectedColor(CCEButtonST* button)
    {
        button->SetColor(CCEButtonST::BTNST_COLOR_BK_IN, Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
        button->SetColor(CCEButtonST::BTNST_COLOR_BK_OUT, Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
        button->SetColor(CCEButtonST::BTNST_COLOR_BK_FOCUS, Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
    }

    void CSettingDlg::OnClickListType(NMHDR* pNMHDR, LRESULT* pResult) 
    {
        // TODO: Add your control notification handler code here
        ShowConfigItems();
        *pResult = 0;
    }

    void CSettingDlg::IniCtrlData()
    {
        if (m_pSetting->thisTelephoneNumber().number() != "")
        {
            m_edtLocalAreaNumber.SetWindowText(Util::StringOp::ToCString(m_pSetting->thisTelephoneNumber().number()));
        }
        if (m_pSetting->isAutoAppendOutlinePrefix())
        {
            m_chbEnableOutLine.SetCheck(BST_CHECKED);
        }
        else
        {
            m_chbEnableOutLine.SetCheck(BST_UNCHECKED);
        }
        m_edtOutLine.SetWindowText(Util::StringOp::ToCString(m_pSetting->outlinePrefix()));

        if (m_pSetting->isAppendIpPrefix())
        {
            m_chbEnableIPDial.SetCheck(BST_CHECKED);
        }
        else
        {
            m_chbEnableIPDial.SetCheck(BST_UNCHECKED);
        }
        m_edtIPDial.SetWindowText(Util::StringOp::ToCString(m_pSetting->ipPrefix()));

        if (m_pSetting->isUseSpecRing())
        {
            m_chbEnableRing.SetCheck(BST_CHECKED);
        }
        else
        {
            m_chbEnableRing.SetCheck(BST_UNCHECKED);
        }

        std::string sfilename = m_pSetting->defaultRingFilename();
        sfilename = "";
        CString s = Util::StringOp::ToCString(sfilename);
        int nStart = 0;
        int n = s.Find(_T("/"), nStart);
        while (n != -1)
        {
            nStart = n + 1;
            n = s.Find(_T("/"), nStart);
        }
        CString filename = s.Mid(nStart);
        CString path = s.Mid(0, nStart);

        SetRingLst((TCHAR*)(LPCTSTR)::ringPath);
        //@@m_cmbRing.m_edit.SetWindowText(filename);
        m_cmbRing.SetWindowText(filename);

        wcscpy(m_ringDir, (LPCTSTR)::ringPath);

        if (m_pSetting->isAutoReply())
        {
            m_chbEnableAutoLeaveWord.SetCheck(BST_CHECKED);
        }
        else
        {
            m_chbEnableAutoLeaveWord.SetCheck(BST_UNCHECKED);
        }

        m_cmbRingTimes.ResetContent();
        CString sCount[4] = {_T("3"), _T("4"), _T("5"), _T("6")};
        m_cmbRingTimes.AddString(sCount[0]);
        m_cmbRingTimes.AddString(sCount[1]);
        m_cmbRingTimes.AddString(sCount[2]);
        m_cmbRingTimes.AddString(sCount[3]);
        int nRingCount = m_pSetting->autoReplyRingCount();
        int nn = 0;
        if (nRingCount == 3)
            nn = 0;
        else if (nRingCount == 4)
            nn = 1;
        else if (nRingCount == 5)
            nn = 2;
        else if (nRingCount == 6)
            nn = 3;
        m_cmbRingTimes.SetCurSel(nn);

        m_cmbAutoRecoedeTimes.ResetContent();
        CString sCount_[4] = {_T("15秒"), _T("30秒"), _T("60秒"), _T("90秒")};
        m_cmbAutoRecoedeTimes.AddString(sCount_[0]);
        m_cmbAutoRecoedeTimes.AddString(sCount_[1]);
        m_cmbAutoRecoedeTimes.AddString(sCount_[2]);
        m_cmbAutoRecoedeTimes.AddString(sCount_[3]);
        int nTimes = m_pSetting->maxSoundUseSize();    //暂时最大声音条数来记录自动录音的时长

        m_cmbAutoRecoedeTimes.SetCurSel(nTimes);

        if (m_pSetting->linkMode() == Data::lmDirect)
        {
            m_rdoConnectAuto.SetCheck_(BST_CHECKED);
            m_rdoConnectADSL.SetCheck_(BST_UNCHECKED);
        }
        else
        {
            m_rdoConnectAuto.SetCheck_(BST_UNCHECKED);
            m_rdoConnectADSL.SetCheck_(BST_CHECKED);
        }

        m_edtADSLName.SetWindowText(Util::StringOp::ToCString(m_pSetting->dialUsername()));
        m_edtADSLPassword.SetWindowText(Util::StringOp::ToCString(m_pSetting->dialPassword()));

        //从注册表中获取IP信息显示

        GetIPInfo();
        if (m_ip.isDHCP)
        {
            m_rdoGetIPDHCP.SetCheck_(BST_CHECKED);
            m_rdoGetIPStatic.SetCheck_(BST_UNCHECKED);
        }
        else
        {
            m_rdoGetIPDHCP.SetCheck_(BST_UNCHECKED);
            m_rdoGetIPStatic.SetCheck_(BST_CHECKED);
        }

        CString ip = m_ip.ipAddress;
        int index1 = 0;
        for (int i = 0; i < 4; ++i)
        {
            int index = ip.Find('.', index1);
            CString ss;
            if (index > 0)
            {
                ss = ip.Mid(index1, index-index1);
            }
            else
            {
                ss = ip.Mid(index1);
            }
            index1 = index+1;
            m_edtIP[i].SetWindowText(ss);
        }

        ip = m_ip.mask;
        index1 = 0;
        for (int i = 0; i < 4; ++i)
        {
            int index = ip.Find('.', index1);
            CString ss;
            if(index>0)
            {
                ss = ip.Mid(index1, index-index1);
            }
            else
            {
                ss = ip.Mid(index1);
            }
            index1 = index+1;
            m_edtMask[i].SetWindowText(ss);
        }

        ip = m_ip.gw;
        index1 = 0;
        for (int i = 0; i < 4; ++i)
        {
            int index = ip.Find('.', index1);
            CString ss;
            if (index > 0)
            {
                ss = ip.Mid(index1, index-index1);
            }
            else
            {
                ss = ip.Mid(index1);
            }
            index1 = index+1;
            m_edtGateway[i].SetWindowText(ss);
        }

        ip = m_ip.dns;
        index1 = 0;
        for (int i = 0; i < 4; ++i)
        {
            int index = ip.Find('.', index1);
            CString ss;
            if(index>0)
            {
                ss = ip.Mid(index1, index-index1);
            }
            else
            {
                ss = ip.Mid(index1);
            }
            index1 = index+1;
            m_edtDNS[i].SetWindowText(ss);
        }

        if (m_pSetting->isUseScreenSaver())
        {
            m_chbEnnabeScreenProtect.SetCheck(BST_CHECKED);
        }
        else
        {
            m_chbEnnabeScreenProtect.SetCheck(BST_UNCHECKED);
        }

        if (m_pSetting->screenSaverContent() == Data::sscClock)
        {
            m_rdoTimeScreen.SetCheck_(BST_CHECKED);
            m_rdoImageScreen.SetCheck_(BST_UNCHECKED);
        }
        else
        {
            m_rdoTimeScreen.SetCheck_(BST_UNCHECKED);
            m_rdoImageScreen.SetCheck_(BST_CHECKED);
        }

        CString sSecond[5] = {_T("15秒"), _T("30秒"), _T("1分钟"), _T("5分钟"), _T("15分钟")};
        m_cmbWaitTime.ResetContent();

        m_cmbWaitTime.AddString(sSecond[0]);
        m_cmbWaitTime.AddString(sSecond[1]);
        m_cmbWaitTime.AddString(sSecond[2]);
        m_cmbWaitTime.AddString(sSecond[3]);
        m_cmbWaitTime.AddString(sSecond[4]);

        CString sRingVolume[5] = {_T("最大"), _T("较大"), _T("中间"), _T("较小"), _T("最小")};
        m_cmbRingVolume.ResetContent();
        m_cmbRingVolume.AddString(sRingVolume[0]);
        m_cmbRingVolume.AddString(sRingVolume[1]);
        m_cmbRingVolume.AddString(sRingVolume[2]);
        m_cmbRingVolume.AddString(sRingVolume[3]);
        m_cmbRingVolume.AddString(sRingVolume[4]);

        int nVolume = m_pSetting->soundVolume();
        m_cmbRingVolume.SetCurSel(nVolume);

        m_cmbSystemVolume.ResetContent();
        m_cmbSystemVolume.AddString(sRingVolume[0]);
        m_cmbSystemVolume.AddString(sRingVolume[1]);
        m_cmbSystemVolume.AddString(sRingVolume[2]);
        m_cmbSystemVolume.AddString(sRingVolume[3]);
        m_cmbSystemVolume.AddString(sRingVolume[4]);
        nVolume = m_pSetting->sysVolume();
        m_cmbSystemVolume.SetCurSel(nVolume);

        ::UINT16 volume[] = {0xFF00, 0xcc00, 0x8800, 0x4400, 0x1000}; //(5-nVolume)*0xFFFF/5;
        ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->phone_->g_RingSound =  volume[nVolume]|(volume[nVolume]<<16);   //(nVolume*-8000/5);

        CTimeSpan time = m_pSetting->screenSaverDuration();
        int nSel = 0;
        int nSeconds = (int)time.GetTotalSeconds();
        if (nSeconds == 15)
        {
            nSel = 0;
        }
        else if (nSeconds == 30)
        {
            nSel = 1;
        }
        else if (nSeconds == 60)
        {
            nSel = 2;
        }
        else if (nSeconds == 5*60)
        {
            nSel = 3;
        }
        else if (nSeconds == 15*60)
        {
            nSel = 4;
        }
        m_cmbWaitTime.SetCurSel(nSel);

        if (m_pSetting->isUseScreenSaverPassword())
        {
            m_chbEnablePassword.SetCheck(BST_CHECKED);
        }
        else
        {
            m_chbEnablePassword.SetCheck(BST_UNCHECKED);
        }

        m_cmbSoundSavePath.ResetContent();

        std::string temp = Data::LanguageResource::Get(Data::RI_SETTING_LOCAL);
        m_cmbSoundSavePath.AddString(Util::StringOp::ToCString(temp));

        BOOL sd = ::DetectDIR(_T("\\StorageCard"));
        if (sd)
        {
            temp = Data::LanguageResource::Get(Data::RI_SETTING_SDCARD);
            m_cmbSoundSavePath.AddString(Util::StringOp::ToCString(temp));
        }

        std::string soundPath = m_pSetting->soundPath();
        if (soundPath.substr(0, 12) == "\\StorageCard" && sd)
        {
            m_cmbSoundSavePath.SetCurSel(1);
        }
        else
        {
            m_cmbSoundSavePath.SetCurSel(0);
        }

        //blacklight
        if (m_pSetting->isContrlBlackLight())
        {
            m_chbContrlBlackLight.SetCheck(BST_CHECKED);
        }
        else
        {
            m_chbContrlBlackLight.SetCheck(BST_UNCHECKED);
        }
        if (m_pSetting->isNightControlBlackLight())
        {
            m_chbNightContrlBlackLight.SetCheck(BST_CHECKED);
        }
        else
        {
            m_chbNightContrlBlackLight.SetCheck(BST_UNCHECKED);
        }

        CString sWait[4] = {_T("1分钟"), _T("5分钟"), _T("10分钟"), _T("30分钟")};
        m_cmbContrlBlackLightWaitTime.ResetContent();
        m_cmbContrlBlackLightWaitTime.AddString(sWait[0]);
        m_cmbContrlBlackLightWaitTime.AddString(sWait[1]);
        m_cmbContrlBlackLightWaitTime.AddString(sWait[2]);
        m_cmbContrlBlackLightWaitTime.AddString(sWait[3]);
        m_cmbContrlBlackLightWaitTime.SetCurSel(m_pSetting->contrlBlackLightWaitTime());

        CTime t = m_pSetting->nightControlBlackLightStartTime();
        m_dtNightContrlBlackLightStartTime.SetTime(&t);
        t = m_pSetting->nightControlBlackLightEndTime();
        m_dtNightContrlBlackLightEndTime.SetTime(&t);
    }

    void CSettingDlg::OnButtonSettingOk() 
    {
        if (m_uiType == 2)
        {
            SYSTEMTIME curtime, curtime1, curtime2;
            m_dtDate.GetTime(curtime1);
            m_dtTime.GetTime(curtime2);

            if(!(m_curtime.wYear == curtime1.wYear && m_curtime.wMonth == curtime1.wMonth &&m_curtime.wDay == curtime1.wDay &&\
                m_curtime.wHour == curtime2.wHour && m_curtime.wMinute == curtime2.wMinute &&m_curtime.wSecond == curtime2.wSecond))
            {
                memcpy(&curtime, &curtime1, sizeof(SYSTEMTIME));
                curtime.wHour = curtime2.wHour;
                curtime.wMinute = curtime2.wMinute;
                curtime.wSecond = curtime2.wSecond;
                SetLocalTime(&curtime);	
                memcpy(&m_curtime, &curtime, sizeof(SYSTEMTIME));

                ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->SetAlarmList();    //重新计算闹铃
            }
        }

        CString str;
        m_edtLocalAreaNumber.GetWindowText(str);
        m_pSetting->thisTelephoneNumber(Data::TelephoneNumber(Util::StringOp::FromCString(str)));
        if (m_chbEnableOutLine.GetCheck() == BST_CHECKED)
        {
            m_pSetting->isAutoAppendOutlinePrefix(true);
        }
        else
        {
            m_pSetting->isAutoAppendOutlinePrefix(false);
        }

        m_edtOutLine.GetWindowText(str);
        m_pSetting->outlinePrefix(Util::StringOp::FromCString(str));

        if (m_chbEnableIPDial.GetCheck() == BST_CHECKED)
        {
            m_pSetting->isAppendIpPrefix(true);
        }
        else
        {
            m_pSetting->isAppendIpPrefix(false);
        }

        m_edtIPDial.GetWindowText(str);
        m_pSetting->ipPrefix(Util::StringOp::FromCString(str));

        if (m_chbEnableRing.GetCheck() == BST_CHECKED)
        {
            m_pSetting->isUseSpecRing(true);
        }
        else
        {
            m_pSetting->isUseSpecRing(false);
        }

        //m_ringEdit.GetWindowText(str);
        m_cmbRing.GetWindowText(str);

        CString s = m_ringDir;
        s += _T("/") + str;
        m_pSetting->defaultRingFilename(Util::StringOp::FromCString(s));

        if (m_chbEnableAutoLeaveWord.GetCheck() == BST_CHECKED)
        {
            m_pSetting->isAutoReply(true);
        }
        else
        {
            m_pSetting->isAutoReply(false);
        }

        m_cmbRingTimes.GetWindowText(str);
        m_pSetting->autoReplyRingCount(Util::StringOp::ToInt(Util::StringOp::FromCString(str)));

        m_pSetting->maxSoundUseSize(m_cmbAutoRecoedeTimes.GetCurSel());

        if (m_rdoConnectAuto.GetCheck_() == BST_CHECKED)
        {
            m_pSetting->linkMode(Data::lmDirect);
        }
        else
        {
            m_pSetting->linkMode(Data::lmDial);
        }

        m_edtADSLName.GetWindowText(str);
        m_pSetting->dialUsername(Util::StringOp::FromCString(str));

        m_edtADSLPassword.GetWindowText(str);
        m_pSetting->dialPassword(Util::StringOp::FromCString(str));

        NETWORK_ADPT_INFO ipConfig;
        memset(&ipConfig, 0, sizeof(NETWORK_ADPT_INFO ));

        if (m_rdoGetIPDHCP.GetCheck_() == BST_CHECKED)
        {
            ipConfig.fUseDHCP = TRUE;
            m_pSetting->ipMode(Data::imAuto);
        }
        else
        {
            ipConfig.fUseDHCP = FALSE;
            m_pSetting->ipMode(Data::imManual);
        }

        char txt[4][32];
        memset(txt, 0, 4 * 32);
        Data::IPAddr  nIP[4] = {0, 0, 0, 0};
        for (int i = 0; i < 4; ++i)
        {
            m_edtIP[i].GetWindowText(str);
            nIP[0] += ((Util::StringOp::ToInt(Util::StringOp::FromCString(str))) << (i * 8));
            m_edtMask[i].GetWindowText(str);
            nIP[1] += ((Util::StringOp::ToInt(Util::StringOp::FromCString(str))) << (i * 8));
            m_edtGateway[i].GetWindowText(str);
            nIP[2] += ((Util::StringOp::ToInt(Util::StringOp::FromCString(str))) << (i * 8));
            m_edtDNS[i].GetWindowText(str);
            nIP[3] += ((Util::StringOp::ToInt(Util::StringOp::FromCString(str))) << (i * 8));
        }

        sprintf(txt[0], "%d.%d.%d.%d", (nIP[0] & 0xFF), ((nIP[0] >> 8) & 0xFF), ((nIP[0] >> 16) & 0xFF), ((nIP[0] >> 24) & 0xFF));
        sprintf(txt[1], "%d.%d.%d.%d", (nIP[1] & 0xFF), ((nIP[1] >> 8) & 0xFF), ((nIP[1] >> 16) & 0xFF), ((nIP[1] >> 24) & 0xFF));
        if (m_rdoConnectAuto.GetCheck_() != BST_CHECKED && m_rdoGetIPDHCP.GetCheck_() != BST_CHECKED)
            sprintf(txt[2], "");
        else
            sprintf(txt[2], "%d.%d.%d.%d", (nIP[2] & 0xFF), ((nIP[2] >> 8) & 0xFF), ((nIP[2] >> 16) & 0xFF), ((nIP[2] >> 24) & 0xFF));
        sprintf(txt[3], "%d.%d.%d.%d", (nIP[3] & 0xFF), ((nIP[3] >> 8) & 0xFF), ((nIP[3] >> 16) & 0xFF), ((nIP[3] >> 24) & 0xFF));

        CString sIP = Util::StringOp::ToCString(txt[0]);
        memcpy(ipConfig.IPAddr, (LPCTSTR)sIP, sIP.GetLength() * 2);
        sIP = txt[1];
        memcpy(ipConfig.SubnetMask, (LPCTSTR)sIP, sIP.GetLength() * 2);
        sIP = txt[2];
        memcpy(ipConfig.Gateway, (LPCTSTR)sIP, sIP.GetLength() * 2);
        sIP = txt[3];
        memcpy(ipConfig.DNSAddr, (LPCTSTR)sIP, sIP.GetLength() * 2);
        memcpy(ipConfig.WINSAddr, (LPCTSTR)sIP, sIP.GetLength() * 2);
        SetIPConfig(ipConfig);

        m_pSetting->thisIPAddr(nIP[0]);
        m_pSetting->subnetMask(nIP[1]);
        m_pSetting->gateway(nIP[2]);
        m_pSetting->dns(nIP[3]);

        if (m_chbEnnabeScreenProtect.GetCheck() == BST_CHECKED)
        {
            m_pSetting->isUseScreenSaver(true);
        }
        else
        {
            m_pSetting->isUseScreenSaver(false);
        }

        if (m_rdoTimeScreen.GetCheck_() == BST_CHECKED)
        {
            m_pSetting->screenSaverContent(Data::sscClock);
        }
        else
        {
            m_pSetting->screenSaverContent(Data::sscImage);
        }

        int nSecond[5] = {15, 30, 60, 5 * 60, 15 * 60};
        int nSel = m_cmbWaitTime.GetCurSel();
        CTimeSpan time = CTimeSpan(0, 0, nSecond[nSel] / 60, nSecond[nSel] % 60);
        m_pSetting->screenSaverDuration(time);

        int nVolume;
        nVolume = m_cmbSystemVolume.GetCurSel();
        m_pSetting->sysVolume(nVolume);

        ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pMainDlg->m_mainMp3Dlg_->SetVolume();
        ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pMainDlg->m_mainVideoDlg_->SetVolume();

        nVolume = m_cmbRingVolume.GetCurSel();
        ::UINT16 volume[] = {0xFF00, 0xcc00, 0x8800, 0x4400, 0x1000}; //(5-nVolume)*0xFFFF/5;
        ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->phone_->g_RingSound = volume[nVolume] | (volume[nVolume] << 16);   //(nVolume*-8000/5);
        m_pSetting->soundVolume(nVolume);

        if (m_chbEnablePassword.GetCheck() == BST_CHECKED)
        {
            m_pSetting->isUseScreenSaverPassword(true);
        }
        else
        {
            m_pSetting->isUseScreenSaverPassword(false);
        }

        std::string soundPath;
        if (m_cmbSoundSavePath.GetCurSel() == 0)
        {
            soundPath = Util::StringOp::FromCString(::recordPath) + "/";
        }
        else
        {
            soundPath = "\\StorageCard\\MY_RECORD\\";
        }

        //	BOOL sd = DetectDIR(_T("\\storagecard"));
        BOOL sd = ::DetectDIR((LPTSTR)(LPCTSTR)Util::StringOp::ToCString(soundPath));
        if (!sd)
        {
            CreateDirectory(Util::StringOp::ToCString(soundPath), NULL);
        }

        m_pSetting->soundPath(soundPath);

        //blacklight
        if (m_chbContrlBlackLight.GetCheck() == BST_CHECKED)
        {
            m_pSetting->isContrlBlackLight(true);
        }
        else
        {
            m_pSetting->isContrlBlackLight(false);
        }

        if (m_chbNightContrlBlackLight.GetCheck() == BST_CHECKED)
        {
            m_pSetting->isNightControlBlackLight(true);
        }
        else
        {
            m_pSetting->isNightControlBlackLight(false);
        }

        m_pSetting->contrlBlackLightWaitTime(m_cmbContrlBlackLightWaitTime.GetCurSel());
        CTime t1;
        m_dtNightContrlBlackLightStartTime.GetTime(t1);
        m_pSetting->nightControlBlackLightStartTime(t1);
        m_dtNightContrlBlackLightEndTime.GetTime(t1);
        m_pSetting->nightControlBlackLightEndTime(t1);

        m_pSetting->Update();
        ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->SetScreenSaveTimer();
        ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->SwitchPanel_(IDC_BUTTON_MAIN);

    }

    void CSettingDlg::OnButtonSettingCancel() 
    {
        // TODO: Add your control notification handler code here
        IniCtrlData();
        ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->SwitchPanel_(IDC_BUTTON_MAIN);
    }

    LRESULT CSettingDlg::OnDeleteTipOk(WPARAM w, LPARAM l)
    {
        LRESULT result = 0;
        int id = m_pSetting->id();
        Data::SettingType type = m_pSetting->type();
        m_pSetting = Data::Setting::GetDefaultConfig();
        m_pSetting->id(id);
        m_pSetting->type(type);
        m_pSetting->Update();
        IniCtrlData();
        ShowConfigItems();
        return result;
    }

    void CSettingDlg::OnButtonSettingDefault() 
    {
        // TODO: Add your control notification handler code here
        m_iDeleteType = 4;
        std::string strTemp = "确定恢复默认设置吗?";
        ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetDelTip(Util::StringOp::ToCString(strTemp.c_str()));
        ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetHWnd(m_hWnd);
        ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->ShowWindow(TRUE);
    }

    void CSettingDlg::OnButtonSettingRecord()
    {	
    }

    typedef BOOL (WINAPI *_TouchCalibrate)();

    //触摸屏校正
    void CSettingDlg::OnAdjustTouchPanel()
    {
        HINSTANCE  hinstance = LoadLibrary(_T("coredll.dll"));
        if  (hinstance == NULL)  
        {  
            AfxMessageBox (L"instance == NULL"); 
            return; 
        }  
        _TouchCalibrate TouchCalibrate = NULL; 
        TouchCalibrate = (_TouchCalibrate)GetProcAddress(hinstance , (LPCSTR)"TouchCalibrate"); 
        if (TouchCalibrate == NULL)  
        {  
            AfxMessageBox(L"TouchCalibrate == NULL"); 
            return; 
        }  
        TouchCalibrate(); 
        FreeLibrary(hinstance);
        TCHAR szTemp[256] = {0};
        _tcscpy(szTemp, TEXT("HARDWARE\\DEVICEMAP\\TOUCH"));
        HKEY hKey = NULL;
        LONG hRes = RegOpenKeyEx(HKEY_LOCAL_MACHINE, szTemp, 0, 0, &hKey);
        if (ERROR_SUCCESS == hRes)
        {
        }
        LONG l = RegFlushKey(HKEY_LOCAL_MACHINE);
        RegCloseKey(hKey);
    }

    LRESULT CSettingDlg::OnSetSaveScreenPassWordOK(WPARAM w, LPARAM l)
    {
        LRESULT result = 0;
        m_pSetting->screenSaverPassword((char *)w);
        return result;
    }

    void CSettingDlg::OnSettingSaveScreenPassword()
    {
        m_passwordDlg->SetType(SETTING_PASSWORD);
        std::string strTemp = m_pSetting->screenSaverPassword();
        m_passwordDlg->SetOldPassWord((char *)strTemp.c_str());
        m_passwordDlg->SetHWnd(this->m_hWnd);
        m_passwordDlg->ShowWindow(SW_SHOW);	
    }

    void CSettingDlg::OnButtonSettingDefineFastDial()
    {
        m_pFastDialsDlg->SetFastDialParam(m_pSetting);
        m_pFastDialsDlg->ShowWindow(TRUE);	
    }

    void CSettingDlg::ShowConfigItems(void)
    {
        m_cmbRing.ShowWindow(SW_HIDE);

        m_edtLocalAreaNumber.ShowWindow(FALSE);
        m_chbEnableOutLine.ShowWindow(FALSE);
        m_edtOutLine.ShowWindow(FALSE);
        m_chbEnableIPDial.ShowWindow(FALSE);
        m_edtIPDial.ShowWindow(FALSE);
        m_chbEnableRing.ShowWindow(FALSE);
        m_cmbRing.ShowWindow(SW_HIDE);

        m_chbEnableAutoLeaveWord.ShowWindow(FALSE);
        m_cmbRingTimes.ShowWindow(FALSE);
        m_cmbAutoRecoedeTimes.ShowWindow(FALSE);
        m_rdoConnectAuto.ShowWindow(FALSE);
        m_rdoConnectADSL.ShowWindow(FALSE);
        m_edtADSLName.ShowWindow(FALSE);
        m_edtADSLPassword.ShowWindow(FALSE);	
        m_rdoGetIPDHCP.ShowWindow(FALSE);
        m_rdoGetIPStatic.ShowWindow(FALSE);
        for (int i = 0; i < 4; ++i)
        {
            m_edtIP[i].ShowWindow(FALSE);
            m_edtMask[i].ShowWindow(FALSE);
            m_edtGateway[i].ShowWindow(FALSE);
            m_edtDNS[i].ShowWindow(FALSE);
        }

        //systom
        m_chbEnnabeScreenProtect.ShowWindow(FALSE);
        m_rdoTimeScreen.ShowWindow(FALSE);
        m_rdoImageScreen.ShowWindow(FALSE);
        m_cmbWaitTime.ShowWindow(FALSE);
        m_cmbRingVolume.ShowWindow(FALSE);
        m_cmbSystemVolume.ShowWindow(FALSE);
        m_chbEnablePassword.ShowWindow(FALSE);
        m_cmbSoundSavePath.ShowWindow(FALSE);
        m_dtTime.ShowWindow(FALSE);
        m_dtDate.ShowWindow(FALSE);
        //information
        m_sticSystomTitle.ShowWindow(FALSE);
        m_sticSoftwareVersion.ShowWindow(FALSE);
        m_editVersion.ShowWindow(FALSE);
        //m_sticHardwareVersion.ShowWindow(FALSE);
        m_sticMemorySize.ShowWindow(FALSE);
        m_sticStorageSize.ShowWindow(FALSE);
        m_sticContactSize.ShowWindow(FALSE);
        m_sticContactInfoSize.ShowWindow(FALSE);
        m_sticSoundSize.ShowWindow(FALSE);
        m_sticInformationSize.ShowWindow(FALSE);
        m_sticVersionRight.ShowWindow(FALSE);

        //blacklight
        m_chbContrlBlackLight.ShowWindow(FALSE);
        m_cmbContrlBlackLightWaitTime.ShowWindow(FALSE);
        m_chbNightContrlBlackLight.ShowWindow(FALSE);
        m_dtNightContrlBlackLightStartTime.ShowWindow(FALSE);
        m_dtNightContrlBlackLightEndTime.ShowWindow(FALSE);

        switch (m_uiType)
        {
        case 0:
            m_edtLocalAreaNumber.ShowWindow(TRUE);
            m_chbEnableOutLine.ShowWindow(TRUE);
            m_edtOutLine.ShowWindow(TRUE);
            m_chbEnableIPDial.ShowWindow(TRUE);
            m_edtIPDial.ShowWindow(TRUE);
            m_chbEnableRing.ShowWindow(TRUE);
            m_cmbRing.ShowWindow(TRUE);
            m_chbEnableAutoLeaveWord.ShowWindow(TRUE);
            m_cmbRingTimes.ShowWindow(TRUE);
            m_cmbAutoRecoedeTimes.ShowWindow(TRUE);
            break;
        case 1:
            m_rdoConnectAuto.ShowWindow(TRUE);
            m_rdoConnectADSL.ShowWindow(TRUE);
            m_edtADSLName.ShowWindow(TRUE);
            m_edtADSLPassword.ShowWindow(TRUE);	
            m_rdoGetIPDHCP.ShowWindow(TRUE);
            m_rdoGetIPStatic.ShowWindow(TRUE);
            for (int i = 0; i < 4; ++i)
            {
                m_edtIP[i].ShowWindow(TRUE);
                m_edtMask[i].ShowWindow(TRUE);
                m_edtGateway[i].ShowWindow(TRUE);
                m_edtDNS[i].ShowWindow(TRUE);
            }

            break;
        case 2:
            m_chbEnnabeScreenProtect.ShowWindow(TRUE);
            m_rdoTimeScreen.ShowWindow(TRUE);
            m_rdoImageScreen.ShowWindow(TRUE);
            m_cmbWaitTime.ShowWindow(TRUE);
            m_cmbRingVolume.ShowWindow(TRUE);
            m_cmbSystemVolume.ShowWindow(TRUE);
            m_cmbSoundSavePath.ShowWindow(TRUE);
            {
                m_cmbSoundSavePath.ResetContent();
                std::string temp = Data::LanguageResource::Get(Data::RI_SETTING_LOCAL);
                m_cmbSoundSavePath.AddString(Util::StringOp::ToCString(temp));

                BOOL sd = ::DetectDIR(_T("\\StorageCard"));
                if (sd)
                {
                    temp = Data::LanguageResource::Get(Data::RI_SETTING_SDCARD);
                    m_cmbSoundSavePath.AddString(Util::StringOp::ToCString(temp));
                }

                std::string soundPath = m_pSetting->soundPath();
                if (soundPath.substr(0, 12) == "\\StorageCard" && sd)
                {
                    m_cmbSoundSavePath.SetCurSel(1);
                }
                else
                {
                    m_cmbSoundSavePath.SetCurSel(0);
                }
            }
            m_dtTime.ShowWindow(TRUE);
            m_dtDate.ShowWindow(TRUE);
            break;
        case 3:
            break;
        case 4:
            m_sticSystomTitle.ShowWindow(TRUE);
            m_sticSoftwareVersion.ShowWindow(TRUE);
            m_sticMemorySize.ShowWindow(TRUE);		
            m_sticStorageSize.ShowWindow(TRUE);
            m_sticContactSize.ShowWindow(TRUE);
            m_sticContactInfoSize.ShowWindow(TRUE);
            m_sticSoundSize.ShowWindow(TRUE);
            SetDiskInFo();
            m_sticVersionRight.ShowWindow(TRUE);
            break;
        case 5:
            m_chbContrlBlackLight.ShowWindow(TRUE);
            m_cmbContrlBlackLightWaitTime.ShowWindow(TRUE);
            m_chbNightContrlBlackLight.ShowWindow(TRUE);
            m_dtNightContrlBlackLightStartTime.ShowWindow(TRUE);
            m_dtNightContrlBlackLightEndTime.ShowWindow(TRUE);
            break;
        }
    }

    void CSettingDlg::SetDiskInFo()
    {
        std::string temp = Data::LanguageResource::Get(Data::RI_SETTING_AVAILABLESIZE);
        CString as = Util::StringOp::ToCString(temp);
        CString text;
        ULARGE_INTEGER freeBytes;
        ULARGE_INTEGER totalBytes;
        ULARGE_INTEGER totalFreeBytes;
        GetDiskFreeSpaceEx(_T("\\Flashdrv"), &freeBytes, &totalBytes, &totalFreeBytes);
        int m = totalBytes.QuadPart / (1024 * 1024);
        int f = totalFreeBytes.QuadPart / (1024 * 1024);
        f = f - m * 0.1;
        if (f < 0)
        {
            f = 0;
        }
        text.Format(_T("%dM (%dM%s)"), m, f, as);
        m_sticMemorySize.SetWindowText(text);

        freeBytes.QuadPart = 0;
        totalBytes.QuadPart = 0;
        totalFreeBytes.QuadPart = 0;
        GetDiskFreeSpaceEx(_T("\\StorageCard"), &freeBytes, &totalBytes, &totalFreeBytes);
        m = totalBytes.QuadPart / (1024 * 1024);
        f = totalFreeBytes.QuadPart / (1024 * 1024);
        text.Format(_T("%dM (%dM%s)"), m, f, as);
        m_sticStorageSize.SetWindowText(text);

        int count = Data::Contact::GetDataCount("");
        text.Format(_T("%d"), count);
        m_sticContactSize.SetWindowText(text);

        count = Data::ContactInfo::GetDataCount("");
        text.Format(_T("%d"), count);
        m_sticContactInfoSize.SetWindowText(text);

        count = Data::SoundSegment::GetDataCount("");
        text.Format(_T("%d"), count);
        m_sticSoundSize.SetWindowText(text);
    }

    void CSettingDlg::OnSettingClearContact()
    {
        m_iDeleteType = 0;
        ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetHWnd(this->GetSafeHwnd());
        std::string strTemp = Data::LanguageResource::Get(Data::RI_DELETETIP_CONTACT2);
        ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetDelTip(Util::StringOp::ToCString(strTemp.c_str()));
        ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->ShowWindow(TRUE);
    }

    void CSettingDlg::OnSettingClearContactInfo()
    {
        m_iDeleteType = 1;
        ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetHWnd(this->GetSafeHwnd());
        std::string strTemp = Data::LanguageResource::Get(Data::RI_DELETETIP_CONTACTINFO2);
        ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetDelTip(Util::StringOp::ToCString(strTemp.c_str()));
        ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->ShowWindow(TRUE);
    }

    void CSettingDlg::OnSettingClearSound()
    {
        m_iDeleteType = 2;
        ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetHWnd(this->GetSafeHwnd());
        std::string strTemp = Data::LanguageResource::Get(Data::RI_DELETETIP_SOUND2);
        ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetDelTip(Util::StringOp::ToCString(strTemp.c_str()));
        ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->ShowWindow(TRUE);
    }

    void CSettingDlg::OnSettingClearInfo()
    {
        m_iDeleteType = 3;
        ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetHWnd(this->GetSafeHwnd());
        std::string strTemp = Data::LanguageResource::Get(Data::RI_DELETETIP_TITLE);
        ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetDelTip(Util::StringOp::ToCString(strTemp.c_str()));
        ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->ShowWindow(TRUE);
    }

    //数据库备份
    void CSettingDlg::OnBackup()
    {
        m_copyfileDlg->SetType(backup_file);
        m_copyfileDlg->ShowWindow(SW_SHOW);
    }

    //数据库恢复
    void CSettingDlg::OnRestore()
    {
        m_copyfileDlg->SetType(restore_file);
        m_copyfileDlg->ShowWindow(SW_SHOW);
    }

    //录音文件导出
    void CSettingDlg::OnFileOut()
    {
        m_copyfileDlg->SetType(copy_file);
        m_copyfileDlg->ShowWindow(SW_SHOW);
    }

    void CSettingDlg::OnRingLst(NMHDR* pNMHDR, LRESULT* pResult)
    {
        *pResult = 0;
    }

    void CSettingDlg::OnRingSelect()
    {
    }

    class FindRing : public Util::ProcessFileProcedure {
    public:
        FindRing(CString dir, CCEComboBox_& ring)
            : ProcessFileProcedure(dir)
            , ring_(ring) {
        }
        virtual void operator()(WIN32_FIND_DATA const& FindFileData) {
            ring_.AddString(FindFileData.cFileName);
        }
    private:
        CCEComboBox_& ring_;
    };

    void CSettingDlg::SetRingLst(TCHAR* cdir)
    {
        m_cmbRing.ResetContent();
        CString dir = cdir;
        ProcessFiles(_T("*.mp3"), FindRing(dir, m_cmbRing));
        ProcessFiles(_T("*.wav"), FindRing(dir, m_cmbRing));
    }

    void CSettingDlg::GetIPInfo()
    {
        TCHAR szTemp[256] = {0};
        _tcscpy (szTemp, TEXT("Comm\\"));
        //_tcscat (szTemp, TEXT("SMSC91181"));
        _tcscat (szTemp, TEXT("JZ47MAC"));
        _tcscat (szTemp, TEXT("\\Parms\\TcpIp"));
        HKEY hKey = NULL;
        LONG hRes = RegOpenKeyEx (HKEY_LOCAL_MACHINE, szTemp, 0, 0, &hKey);
        if (ERROR_SUCCESS == hRes)
        {
            DWORD dwDataSize = sizeof(DWORD);
            DWORD dType;
            DWORD dNcount = 128;
            BYTE  Buffer[128];
            char temp[128];
            memset(Buffer, 0, 128);
            dNcount = 128;
            hRes = RegQueryValueEx(hKey, TEXT("EnableDHCP"), NULL, &dType, Buffer, &dNcount);

            if(*(DWORD *)Buffer == 1)
            {
                m_ip.isDHCP = 1;
                memset(Buffer, 0, 64);
                dNcount = 128;
                hRes = RegQueryValueEx(hKey, TEXT("DhcpIPAddress"), NULL, &dType, Buffer, &dNcount);
                memset(temp, 0, 128);
                wcstombs(temp, (const wchar_t *)Buffer, dNcount);
                m_ip.ipAddress = temp;
                memset(Buffer, 0, 64);
                dNcount = 128;
                hRes = RegQueryValueEx(hKey, TEXT("DhcpDefaultGateway"), NULL, &dType, Buffer, &dNcount);
                memset(temp, 0, 128);
                wcstombs(temp, (const wchar_t *)Buffer, dNcount);
                m_ip.gw = temp;
                memset(Buffer, 0, 64);
                dNcount = 128;
                hRes = RegQueryValueEx(hKey, TEXT("DhcpSubnetMask"), NULL, &dType, Buffer, &dNcount);
                memset(temp, 0, 128);
                wcstombs(temp, (const wchar_t *)Buffer, dNcount);
                m_ip.mask = temp;
                memset(Buffer, 0, 64);
                dNcount = 128;
                hRes = RegQueryValueEx(hKey, TEXT("DhcpDNS"), NULL, &dType, Buffer, &dNcount);
                memset(temp, 0, 128);
                wcstombs(temp, (const wchar_t *)Buffer, dNcount);
                m_ip.dns = temp;
            }
            else
            {
                m_ip.isDHCP = 0;
                memset(Buffer, 0, 64);
                dNcount = 128;
                hRes = RegQueryValueEx(hKey, TEXT("IpAddress"), NULL, &dType, Buffer, &dNcount);
                memset(temp, 0, 128);
                wcstombs(temp, (const wchar_t *)Buffer, dNcount);
                m_ip.ipAddress = temp;
                memset(Buffer, 0, 64);
                dNcount = 128;
                hRes = RegQueryValueEx(hKey, TEXT("Subnetmask"), NULL, &dType, Buffer, &dNcount);
                memset(temp, 0, 128);
                wcstombs(temp, (const wchar_t *)Buffer, dNcount);
                m_ip.mask = temp;
                memset(Buffer, 0, 64);
                dNcount = 128;
                hRes = RegQueryValueEx(hKey, TEXT("DefaultGateway"), NULL, &dType, Buffer, &dNcount);
                memset(temp, 0, 128);
                wcstombs(temp, (const wchar_t *)Buffer, dNcount);
                m_ip.gw = temp;
                memset(Buffer, 0, 64);
                dNcount = 128;
                hRes = RegQueryValueEx(hKey, TEXT("DNS"), NULL, &dType, Buffer, &dNcount);
                memset(temp, 0, 128);
                wcstombs(temp, (const wchar_t *)Buffer, dNcount);
                m_ip.dns = temp;
            }
            RegCloseKey(hKey);

            //	RegCloseKey(hKey);
        }
    }

    void CSettingDlg::SetIPConfig(NETWORK_ADPT_INFO& AdptInfo)
    {
        TCHAR szTemp[256] = {0};
        _tcscpy (szTemp, TEXT("Comm\\"));
        //_tcscat (szTemp, TEXT("SMSC91181"));
        _tcscat (szTemp, TEXT("JZ47MAC"));
        _tcscat (szTemp, TEXT("\\Parms\\TcpIp"));
        HKEY hKey = NULL;
        LONG hRes = RegOpenKeyEx (HKEY_LOCAL_MACHINE, szTemp, 0, 0, &hKey);
        if (ERROR_SUCCESS == hRes)
        {
            DWORD dwDataSize = sizeof(DWORD);
            if (AdptInfo.fUseDHCP)
            {
                hRes = RegSetValueEx(hKey, TEXT("EnableDHCP"), NULL, REG_DWORD, (LPBYTE)&AdptInfo.fUseDHCP, dwDataSize);
            }
            else
            {
                hRes = RegSetValueEx(hKey, TEXT("EnableDHCP"), NULL, REG_DWORD, (LPBYTE)&AdptInfo.fUseDHCP, dwDataSize);
                dwDataSize = wcslen((wchar_t*)AdptInfo.IPAddr) * 2+2;
                hRes = RegSetValueEx(hKey, TEXT("IpAddress"), NULL, REG_MULTI_SZ, (LPBYTE)AdptInfo.IPAddr, dwDataSize);
                dwDataSize = wcslen((wchar_t*)AdptInfo.SubnetMask) * 2+2;
                hRes = RegSetValueEx(hKey, TEXT("Subnetmask"), NULL, REG_MULTI_SZ, (LPBYTE)AdptInfo.SubnetMask, dwDataSize);
                dwDataSize = wcslen((wchar_t*)AdptInfo.Gateway) * 2+2;
                hRes = RegSetValueEx(hKey, TEXT("DefaultGateway"), NULL, REG_MULTI_SZ, (LPBYTE)AdptInfo.Gateway, dwDataSize);
                dwDataSize = wcslen((wchar_t*)AdptInfo.DNSAddr) * 2+2;
                hRes = RegSetValueEx(hKey, TEXT("DNS"), NULL, REG_MULTI_SZ, (LPBYTE)AdptInfo.DNSAddr, dwDataSize);
            }
            RegFlushKey(HKEY_LOCAL_MACHINE);
            RegCloseKey(hKey);
        }
    }

    CTestDlg *m_pTestDlg0 = NULL;
    LRESULT CSettingDlg::OnStaticClick(WPARAM w, LPARAM l)
    {
        LRESULT result = 0;
        if (m_pTestDlg0 == NULL)
        {
            m_pTestDlg0 = new CTestDlg(this);
            m_pTestDlg0->Create(CTestDlg::IDD);
        }
        View::_test_call = TRUE;
        m_pTestDlg0->ShowWindow(SW_SHOW);
        return result;
    }

    LRESULT CSettingDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
    {
        // TODO: Add your specialized code here and/or call the base class
        int count = 0;
        CString text;
        switch (message)
        {
        case WM_DELETESELITEM:
            if (m_iDeleteType == 0)
            {
                Data::Contact::Remove("");
                Data::ContactGroup::Remove("");
                ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactDlg->ShowTypeItems();
                ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactDlg->SetTypeListSelected(0);
                ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactDlg->ShowItemsInList();
                count = Data::Contact::GetDataCount("");
                text.Format(_T("%d"), count);
                m_sticContactSize.SetWindowText(text);

                ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactDlg->ShowTypeItems();
                ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactDlg->ShowItemsInList();
            }
            else if (m_iDeleteType == 1)
            {
                Data::ContactInfo::Remove("");
                ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactInfoDlg->ShowItemsInList(-1);
                count = Data::ContactInfo::GetDataCount("");
                text.Format(_T("%d"), count);
                m_sticContactInfoSize.SetWindowText(text);			
            }
            else if (m_iDeleteType == 2)
            {
                Data::SoundSegment::Remove("");
                DeleteFiles((LPTSTR)(LPCTSTR)CString(csFlashRecordPath + _T("*")));
                if (::DetectDIR((LPTSTR)(LPCTSTR)csStorageCardRecordPath))
                {
                    DeleteFiles((LPTSTR)(LPCTSTR)CString(csStorageCardRecordPath + _T("*")));
                }
                ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pSoundDlg->ShowItemsInList(-1);
                count = Data::SoundSegment::GetDataCount("");
                text.Format(_T("%d"), count);
                m_sticSoundSize.SetWindowText(text);			
            }
            else if (m_iDeleteType == 3)
            {
            }
            break;
        }	
        return CDialog::WindowProc(message, wParam, lParam);
    }

    void CSettingDlg::UpdateTips(void)
    {
    }

    void CSettingDlg::OnPaint()     
    {   
        //TODO: Add your message handler code here
        CPaintDC dc(this);
    }

    void CSettingDlg::OnTimer(UINT nIDEvent) 
    {
        // TODO: Add your message handler code here and/or call default
        if(nIDEvent == 1)
        {
            KillTimer(1);
            ShowConfigItems();
        }
        CDialog::OnTimer(nIDEvent);
    }
}
