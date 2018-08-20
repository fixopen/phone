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
                           1.����ǰ�����汾\r\n\
                           2.7.091211\r\n\
                           1.�人�̲���ʾ�汾\r\n\
                           2.7.081210\r\n\
                           1.�޸��˱��ɱ�����\r\n\
                           2.6.081209\r\n\
                           1.������ADSL����\r\n\
                           2.5.081209\r\n\
                           1.��Դ���˵���\r\n\
                           2.�����״̬���˵���\r\n\
                           2.4.081206\r\n\
                           1.�޸����л�����������\
                           2.0.081204\r\n\
                           1.�������人�̲����ð汾\r\n\
                           1.2.081126\r\n\
                           \t1.�޸�����������ʱ����ͣ����绰���绰������bug\r\n\
                           1.1.081028\r\n\
                           \t1.�޸��˵绰�зֻ���Ϊֱ������ʾ���ֺŵ�bug\r\n\
                           \t2.�޸��˷ֻ������У������ߺ��룬���ϲ����ֺŵ�bug,Ŀǰ�жϵ������Ƿֻ��ų���С�ڵ���4���ҵ�һ�����벻Ϊ1���������߲���\r\n\
                           \t3.�޸��˲����Զ���¼����\r\n\
                           \t4.��������������\r\n\
                           0.9.080531.0\r\n\
                           \t1.�人�̲ݶ��ư汾\r\n\
                           \t2.���Ӽ����Ļ����ܣ���MJPGʵ��\r\n\
                           \t3.������������\r\n\
                           0.8.071225.3\r\n\
                           \t1.��ӹ�Ʊ����.�����û���:2945011����:123456\r\n\
                           \t2.����㹦�ܴ������״̬������˹�Ʊ.\r\n\
                           \t3.��㹦������汾ֻ�ܴ�ͨ����¼�н���.\r\n\
                           0.8.071220.1\r\n\
                           \tδ��ɹ��ܼ��汾ȱ�ݣ�\r\n\
                           \t1.����д����\r\n\
                           \t2.���Զ���¼��ʾ�����ʾ��\r\n\
                           \t3.����ʱ�Ӳ���ȷ\r\n\
                           \t4.��дУ��δ����\r\n\
                           \t5.��д�ʼ���Բ��\r\n");

    /*
    BOOL DeleteFiles(TCHAR* sDir)
    {
    CString s = sDir;
    s = s.Left(s.GetLength() - 1);

    BOOL flag = FALSE;
    WIN32_FIND_DATA fd; 
    HANDLE hFind = FindFirstFile(sDir, &fd); 
    if ((hFind != INVALID_HANDLE_VALUE)) 
    { 
    if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
    {
    try
    {
    CFile::Remove(s + fd.cFileName);
    }
    catch (CFileException* pEx)
    {
    pEx->Delete();
    }
    }

    while(FindNextFile(hFind, &fd))
    {
    if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
    {
    try
    {
    CFile::Remove(s + fd.cFileName);
    }
    catch (CFileException* pEx)
    {
    pEx->Delete();
    }
    }
    }
    flag = TRUE;
    } 
    FindClose(hFind);
    return flag;
    }
    */
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

        /*
        ON_BN_CLICKED(IDC_SETTING_DELETE, OnDeleteFile)
        ON_BN_CLICKED(IDC_SETTING_ALLSELECT, OnAllSelect)
        ON_BN_CLICKED(IDC_SETTING_CLEARSELECT, OnClearDelect)
        ON_BN_CLICKED(IDC_SETTING_IN, OnFileIn)
        ON_BN_CLICKED(IDC_SETTING_OUT, OnFileOut)
        ON_MESSAGE(WM_COMMBOX_CLICKED, OnFileSelect)
        ON_NOTIFY(NM_CLICK, IDC_SETTING_LSTLOCAL, OnLocalLst)
        ON_NOTIFY(NM_CLICK, IDC_SETTING_LSTUSB, OnUSBLst)
        */
        ON_MESSAGE(WM_CLICKMJPG_TOAPP, OnClickMJPG)
        ON_MESSAGE(WM_STOPTRYRING, OnStopTryRing)
        ON_WM_PAINT()
    END_MESSAGE_MAP()

    void CSettingDlg::SetShowTimer()
    {
        /*
        m_ringList.ShowWindow_(SW_HIDE);
        m_ringStatic.ShowWindow(SW_HIDE);
        */
        m_cmbRing.ShowWindow(SW_HIDE);

        //tel
        //	m_sticLocalAreaNumber.ShowWindow(FALSE);
        m_edtLocalAreaNumber.ShowWindow(FALSE);
        m_chbEnableOutLine.ShowWindow(FALSE);
        m_edtOutLine.ShowWindow(FALSE);
        m_chbEnableIPDial.ShowWindow(FALSE);
        m_edtIPDial.ShowWindow(FALSE);
        m_chbEnableRing.ShowWindow(FALSE);
        //	m_sticDefaultRing.ShowWindow(FALSE);

        m_cmbRing.ShowWindow(FALSE);
        /*
        m_ringEdit.ShowWindow(FALSE);
        m_ringButton.ShowWindow(FALSE);
        */


        m_chbEnableAutoLeaveWord.ShowWindow(FALSE);
        //	m_sticRingTimes.ShowWindow(FALSE);
        m_cmbRingTimes.ShowWindow(FALSE);
        //	m_sticAutoLeaveWord.ShowWindow(FALSE);
        //	m_cmbAutoLeaveWordTip.ShowWindow(FALSE);
        //	m_btnRecardLeaveWordTip.ShowWindow(FALSE);
        //	m_btnDefineSpeedButton.ShowWindow(FALSE);
        //	m_sticDefineSpeedButton.ShowWindow(FALSE);
        //browser
        m_rdoConnectAuto.ShowWindow(FALSE);
        m_rdoConnectADSL.ShowWindow(FALSE);
        // 	m_sticADSLName.ShowWindow(FALSE);
        // 	m_sticADSLPassword.ShowWindow(FALSE);
        m_edtADSLName.ShowWindow(FALSE);
        m_edtADSLPassword.ShowWindow(FALSE);	
        m_rdoGetIPDHCP.ShowWindow(FALSE);
        m_rdoGetIPStatic.ShowWindow(FALSE);
        // 	m_sticIP.ShowWindow(FALSE);
        // 	m_sticMask.ShowWindow(FALSE);
        // 	m_sticGateway.ShowWindow(FALSE);
        // 	m_sticDNS.ShowWindow(FALSE);
        for(int i = 0; i < 4; i++)
        {
            m_edtIP[i].ShowWindow(FALSE);
            m_edtMask[i].ShowWindow(FALSE);
            m_edtGateway[i].ShowWindow(FALSE);
            m_edtDNS[i].ShowWindow(FALSE);
        }

        //systom
        m_chbEnnabeScreenProtect.ShowWindow(FALSE);
        //	m_sticScreenProtctType.ShowWindow(FALSE);
        m_rdoTimeScreen.ShowWindow(FALSE);
        m_rdoImageScreen.ShowWindow(FALSE);
        //	m_sticWaitTime.ShowWindow(FALSE);
        m_cmbWaitTime.ShowWindow(FALSE);
        m_cmbRingVolume.ShowWindow(FALSE);
        m_cmbSystemVolume.ShowWindow(FALSE);
        m_chbEnablePassword.ShowWindow(FALSE);
        //	m_btnSetPassword.ShowWindow(FALSE);
        //	m_sticSoundSavePath.ShowWindow(FALSE);
        m_cmbSoundSavePath.ShowWindow(FALSE);
        //	m_sticTouchAdjust.ShowWindow(FALSE);
        //	m_btnTouchAdjust.ShowWindow(FALSE);
        //	m_stcTime.ShowWindow(FALSE);
        m_dtTime.ShowWindow(FALSE);
        m_dtDate.ShowWindow(FALSE);
        //information
        m_sticSystomTitle.ShowWindow(FALSE);
        // 	m_sticSoftwareVersion1.ShowWindow(FALSE);
        // 	m_sticHardwareVersion1.ShowWindow(FALSE);
        // 	m_sticMemorySize1.ShowWindow(FALSE);
        // 	m_sticStorageSize1.ShowWindow(FALSE);
        // 	m_sticContactSize1.ShowWindow(FALSE);
        // 	m_sticContactInfoSize1.ShowWindow(FALSE);
        // 	m_sticSoundSize1.ShowWindow(FALSE);
        // 	m_sticInformationSize1.ShowWindow(FALSE);
        // 	m_sticVersionRight1.ShowWindow(FALSE);
        m_sticSoftwareVersion.ShowWindow(FALSE);
        m_editVersion.ShowWindow(FALSE);
        //	m_sticHardwareVersion.ShowWindow(FALSE);
        m_sticMemorySize.ShowWindow(FALSE);
        m_sticStorageSize.ShowWindow(FALSE);
        m_sticContactSize.ShowWindow(FALSE);
        m_sticContactInfoSize.ShowWindow(FALSE);
        m_sticSoundSize.ShowWindow(FALSE);
        m_sticInformationSize.ShowWindow(FALSE);
        m_sticVersionRight.ShowWindow(FALSE);
        // 	m_btnClearContact.ShowWindow(FALSE);
        // 	m_btnClearContactInfo.ShowWindow(FALSE);
        // 	m_btnClearSound.ShowWindow(FALSE);
        // 	m_btnClearInformation.ShowWindow(FALSE);
        // 	m_stcBackUP.ShowWindow(FALSE);
        // //	m_stcReStore.ShowWindow(FALSE);
        // 	m_btnBackUP.ShowWindow(FALSE);
        // 	m_btnReStore.ShowWindow(FALSE);
        // 	m_stcTitle.ShowWindow(FALSE);
        // 	m_btnOut.ShowWindow(FALSE);
        /*
        m_cmbTitle.ShowWindow(FALSE);
        m_lstLocal.ShowWindow_(FALSE);
        m_lstUSB.ShowWindow_(FALSE);
        m_btnIn.ShowWindow(FALSE);
        m_btnOut.ShowWindow(FALSE);
        m_btnDelete.ShowWindow(FALSE);
        m_stcLocal.ShowWindow(FALSE);
        m_stcUSB.ShowWindow(FALSE);
        m_stcLocalTitle.ShowWindow(FALSE);
        m_stcUSBTitle.ShowWindow(FALSE);
        m_btnAllSelect.ShowWindow(FALSE);
        m_btnClearSelect.ShowWindow(FALSE);
        */

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
        if(m_MJPGList.GetUnitIsDownStatus(10))
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
        if(m_uiType == 0 && w != 110)
        {
            OnStopTryRing(0, 0);
        }
        switch (w)
        {
        case 1:
            m_MJPGList.SetCurrentLinkFile(_T(".\\adv\\mjpg\\k1\\����\\ϵͳ����1.xml"));
            m_MJPGList.Invalidate();
            m_uiType = 0;
            SetShowTimer();
            //ShowConfigItems();
            break;
        case 2:
            m_MJPGList.SetCurrentLinkFile(_T(".\\adv\\mjpg\\k1\\����\\ϵͳ����2.xml"));
            m_MJPGList.Invalidate();
            m_uiType = 1;
            SetShowTimer();
            //ShowConfigItems();
            break;
        case 3:
            m_MJPGList.SetCurrentLinkFile(_T(".\\adv\\mjpg\\k1\\����\\ϵͳ����3.xml"));
            m_MJPGList.Invalidate();
            m_uiType = 2;
            SetShowTimer();
            //ShowConfigItems();
            break;
        case 4:
            m_MJPGList.SetCurrentLinkFile(_T(".\\adv\\mjpg\\k1\\����\\ϵͳ����4.xml"));
            m_MJPGList.Invalidate();
            m_uiType = 3;
            SetShowTimer();
            //ShowConfigItems();
            break;
        case 5:
            m_MJPGList.SetCurrentLinkFile(_T(".\\adv\\mjpg\\k1\\����\\ϵͳ����5.xml"));
            m_MJPGList.Invalidate();
            m_uiType = 4;
            //ShowConfigItems();
            SetShowTimer();
            break;
        case 9:
            {
                m_MJPGList.SetCurrentLinkFile(_T(".\\adv\\mjpg\\k1\\����\\ϵͳ����6.xml"));

                for(int i = 0; i < 8; i++)
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
            // 		m_firWallDlg->SetParameters(((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pSetting->isFirewall(),
            // 			((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pSetting->firewallType(),
            // 			((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pSetting->blockAllTimeDuration().GetTotalSeconds());
            // 		m_firWallDlg->ShowWindow(TRUE);
            break;
        case 110:
            {
                if(!m_MJPGList.GetUnitIsDownStatus(10))
                {
                    CString str;
                    m_cmbRing.GetWindowText(str);
                    if(str == "")
                        break;
                    CString s = m_ringDir; 
                    s += str;

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
        case 301:
            OnAdjustTouchPanel();
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
        case 404:	//MP3����
            {
                m_copyfileDlg->SetType(mp3_in);
                m_copyfileDlg->ShowWindow(SW_SHOW);
            }
            break;
        case 405:	//������ᵼ��
            {
                m_copyfileDlg->SetType(photo_in);
                m_copyfileDlg->ShowWindow(SW_SHOW);
            }
            break;
        case 406:	//������������
            {
                m_copyfileDlg->SetType(bell_in);
                m_copyfileDlg->ShowWindow(SW_SHOW);
            }
            break;
        case 407:	//�绰�㵼��
            {
                m_copyfileDlg->SetType(show_in);
                m_copyfileDlg->ShowWindow(SW_SHOW);
            }
            break;
        case 408:	//MP3����
            {
                m_copyfileDlg->SetType(mp3_out);
                m_copyfileDlg->ShowWindow(SW_SHOW);
            }
            break;
        case 409:	//������ᵼ��
            {
                m_copyfileDlg->SetType(photo_out);
                m_copyfileDlg->ShowWindow(SW_SHOW);
            }
            break;
        case 410:	//������������
            {
                m_copyfileDlg->SetType(bell_out);
                m_copyfileDlg->ShowWindow(SW_SHOW);
            }
            break;
        case 411:	//�绰�㵼��
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
        case 600:		//���ñ��⼶��
        case 601:
        case 602:
        case 603:
        case 604:
        case 605:
        case 606:
        case 607:
            {
                int old_value = m_pSetting->blackLightValue();
                if(old_value + 600 != w)
                {
                    int i = w - 600;
                    m_pSetting->blackLightValue(i);
                    //����Ļ���
                    m_MJPGList.SetUnitIsDownStatus(old_value+600, FALSE);
                    m_MJPGList.SetUnitIsDownStatus(w, TRUE);
                    m_MJPGList.Invalidate();
                    int v = 7-(w-600);
                    if(v == 0)
                        v =1;
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


        /*
        m_sticTypeTitle.Create(str, WS_CHILD|WS_VISIBLE, CRect(0, 0, 69, 20), this);
        m_sticTypeTitle.SetColor(RGB(0, 0, 0), RGB(78, 165, 229));
        m_sticTypeTitle.SetLeftMargin(25);

        m_sticSepLine1.Create(_T(""), WS_CHILD|WS_VISIBLE, CRect(69, 0, 70, 204), this);
        m_sticSepLine1.SetColor(RGB(78, 165, 229), RGB(255, 255, 255));
        m_sticSepLine2.Create(_T(""), WS_CHILD|WS_VISIBLE, CRect(429, 0, 430, 204), this);
        m_sticSepLine2.SetColor(RGB(255, 255, 255), RGB(255, 255, 255));

        std::string strTemp;
        CString str;
        // 	strTemp = Data::LanguageResource::Get(Data::RI_RECORD_TYPE);
        // 	str = strTemp.c_str();
        // 	m_sticTypeTitle.Create(str, WS_CHILD|WS_VISIBLE, CRect(0, 0, 69, 20), this);
        // 	m_sticTypeTitle.SetColor(RGB(0, 0, 0), RGB(78, 165, 229));
        // 	m_sticTypeTitle.SetLeftMargin(25);
        // 
        // 	m_sticSepLine1.Create(_T(""), WS_CHILD|WS_VISIBLE, CRect(69, 0, 70, 204), this);
        // 	m_sticSepLine1.SetColor(RGB(78, 165, 229), RGB(255, 255, 255));
        // 	m_sticSepLine2.Create(_T(""), WS_CHILD|WS_VISIBLE, CRect(429, 0, 430, 204), this);
        // 	m_sticSepLine2.SetColor(RGB(255, 255, 255), RGB(255, 255, 255));
        */
        /*
        m_lsType.Create(WS_CHILD|WS_VISIBLE|LVS_REPORT|LVS_NOCOLUMNHEADER|LVS_NOSORTHEADER, CRect(8, 27, 80, 198), this, IDC_LIST_SETTING_TYPE, FALSE);
        m_lsType.SetExtendedStyle(m_lsType.GetExtendedStyle()|LVS_EX_FULLROWSELECT);
        m_lsType.InsertColumn(0, _T("Type"), LVCFMT_LEFT, 80);

        m_pImageList = new CImageList();
        m_pImageList->Create(16, 16, ILC_COLOR32|ILC_MASK, 4, 2);   
        CBitmap bm;
        bm.LoadBitmap(IDB_BITMAP28);
        m_pImageList->Add(&bm, RGB(255, 0, 255));
        bm.DeleteObject();
        bm.LoadBitmap(IDB_BITMAP29);
        m_pImageList->Add(&bm, RGB(255, 0, 255));
        bm.DeleteObject();
        bm.LoadBitmap(IDB_BITMAP30);
        m_pImageList->Add(&bm, RGB(255, 0, 255));
        bm.DeleteObject();
        bm.LoadBitmap(IDB_BITMAP27);
        m_pImageList->Add(&bm, RGB(255, 0, 255));
        bm.DeleteObject();
        bm.LoadBitmap(IDB_BITMAP_SETTINGINOUT);
        m_pImageList->Add(&bm, RGB(255, 0, 255));
        bm.DeleteObject();


        m_lsType.SetImageList(m_pImageList, LVSIL_SMALL);

        strTemp = Data::LanguageResource::Get(Data::RI_SETTING_TELEPHONE);
        str = strTemp.c_str();
        m_lsType.InsertItem(0, str, 0);

        strTemp = Data::LanguageResource::Get(Data::RI_SETTING_INTERNET);
        str = strTemp.c_str();
        m_lsType.InsertItem(1, str, 1);

        strTemp = Data::LanguageResource::Get(Data::RI_SETTING_SYSTEM);
        str = strTemp.c_str();
        m_lsType.InsertItem(2, str, 2);

        strTemp = Data::LanguageResource::Get(Data::RI_SETTING_IN);
        str = strTemp.c_str();
        strTemp = Data::LanguageResource::Get(Data::RI_SETTING_OUT);
        str += strTemp.c_str();
        m_lsType.InsertItem(3, str, 4);

        strTemp = Data::LanguageResource::Get(Data::RI_SETTING_INFORMATION);
        str = strTemp.c_str();
        m_lsType.InsertItem(4, str, 3);
        */

        // 	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_DETAILTITLE);
        // 	str = strTemp.c_str();
        // 	m_sticDetailTitle.Create(str, WS_CHILD|WS_VISIBLE, CRect(70, 0, 429, 20), this);
        // 	m_sticDetailTitle.SetColor(RGB(0, 0, 0), RGB(78, 165, 229));
        // 	m_sticDetailTitle.SetLeftMargin(25);

        // 	strTemp = Data::LanguageResource::Get(Data::RI_RECORD_OPERATION);
        // 	str = strTemp.c_str();
        // 	m_sticOperation.Create(str, WS_CHILD|WS_VISIBLE, CRect(430, 0, 480 * 125 / 100, 20), this);
        // 	m_sticOperation.SetColor(RGB(0, 0, 0), RGB(78, 165, 229));
        // 	m_sticOperation.SetLeftMargin(10);

        /*
        strTemp = Data::LanguageResource::Get(Data::RI_COMN_OKBTN);
        str = strTemp.c_str();
        m_btnOk.Create(str, Data::g_buttonRectBMPID[0][Data::g_skinstyle], Data::g_buttonRectBMPID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(417, 26, 471, 45), this, IDC_BUTTON_SETTING_OK);
        m_btnOk.SetBackRGB(Data::g_allFramAngleBackLineRGB[Data::g_skinstyle]);

        strTemp = Data::LanguageResource::Get(Data::RI_SOUND_PLAY_CLOSE);
        str = strTemp.c_str();
        m_btnCancel.Create(str, Data::g_buttonRectBMPID[0][Data::g_skinstyle], Data::g_buttonRectBMPID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(417, 178, 471, 198), this, IDC_BUTTON_SETTING_CANCEL);
        m_btnCancel.SetBackRGB(Data::g_allFramAngleBackLineRGB[Data::g_skinstyle]);

        strTemp = Data::LanguageResource::Get(Data::RI_SETTING_DEFAULT);
        str = strTemp.c_str();
        m_btnDefault.Create(str, Data::g_buttonRectBMPID[0][Data::g_skinstyle], Data::g_buttonRectBMPID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(417, 46, 471, 65), this, IDC_BUTTON_SETTING_DEFAULT);
        m_btnDefault.SetBackRGB(Data::g_allFramAngleBackLineRGB[Data::g_skinstyle]);
        */
        //tel======================================================================================================
        // 	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_AREACODE);
        // 	str = strTemp.c_str();
        // 	m_sticLocalAreaNumber.Create(str, WS_CHILD|WS_VISIBLE, CRect(100, 28, 186, 44), this);
        // 	m_sticLocalAreaNumber.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
        m_edtLocalAreaNumber.Create(WS_CHILD|WS_VISIBLE, CRect(355, 61, 608,100), this, IDC_EDIT_SETTING_AREANO);
        m_edtLocalAreaNumber.SetLimitText(15);
        m_edtLocalAreaNumber.SetLimitDiagital();

        strTemp = Data::LanguageResource::Get(Data::RI_SETTING_OUTLINE);
        str = strTemp.c_str();
        m_chbEnableOutLine.Create(str, WS_CHILD|WS_VISIBLE|BS_CHECKBOX, CRect(166, 118, 166+35, 118+32), this, IDC_CHECK_SETTING_OUTLINE);
        m_chbEnableOutLine.SetIcon(IDI_ICON_CHECK1, CSize(32, 32), IDI_ICON_CHECK0, CSize(32, 32));
        SetButtonDefaultColor(&m_chbEnableOutLine);	
        m_edtOutLine.Create(WS_CHILD|WS_VISIBLE, CRect(355, 114, 608, 153), this, IDC_EDIT_SETTING_OUTLINE);
        m_edtOutLine.SetLimitText(15);
        m_edtOutLine.SetLimitDiagital();

        strTemp = Data::LanguageResource::Get(Data::RI_SETTING_IPDIAL);
        str = strTemp.c_str();
        m_chbEnableIPDial.Create(str, WS_CHILD|WS_VISIBLE|BS_CHECKBOX, CRect(166, 168, 166+35, 168+32), this, IDC_CHECK_SETTING_IPDIAL);
        m_chbEnableIPDial.SetIcon(IDI_ICON_CHECK1, CSize(32, 32), IDI_ICON_CHECK0, CSize(32, 32));
        SetButtonDefaultColor(&m_chbEnableIPDial);	
        m_edtIPDial.Create(WS_CHILD|WS_VISIBLE|ES_AUTOHSCROLL, CRect(355, 167, 608, 206), this, IDC_EDIT_SETTING_IPDIAL);
        m_edtIPDial.SetLimitText(64);
        //	m_edtIPDial.SetLimitDiagital();

        strTemp = Data::LanguageResource::Get(Data::RI_SETTING_ENABLERING);
        str = strTemp.c_str();
        m_chbEnableRing.Create(str, WS_CHILD|WS_VISIBLE|BS_CHECKBOX, CRect(166, 229, 166+35, 229+32), this, IDC_CHECK_SETTING_RING);
        m_chbEnableRing.SetIcon(IDI_ICON_CHECK1, CSize(32, 32), IDI_ICON_CHECK0, CSize(32, 32));
        SetButtonDefaultColor(&m_chbEnableRing);	

        // 	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_ENABLERINGSTC);
        // 	str = strTemp.c_str();
        // 	m_sticDefaultRing.Create(str, WS_CHILD|WS_VISIBLE, CRect(100, 104, 186, 120), this);
        // 	m_sticDefaultRing.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);

        /*
        m_ringEdit.Create(WS_CHILD|WS_VISIBLE, CRect(355,245,608,282), this, 0xFFFF);
        m_ringEdit.SetIsAutoInput(FALSE);
        m_ringButton.Create(L"", Data::g_comboxBMPID[0][Data::g_skinstyle], Data::g_comboxBMPID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(613,242,671,282), this, IDC_SETTING_RINGSELECT);
        m_ringList.Create(WS_CHILD|LVS_REPORT|LVS_NOCOLUMNHEADER|LVS_NOSORTHEADER, CRect(355,290,672,430), this, IDC_SETTING_LSTRING, TRUE, 1);
        m_ringList.SetListColor(Data::g_listctrlBackRGB1[Data::g_skinstyle], Data::g_listctrlBackRGB2[Data::g_skinstyle]);
        m_ringList.InsertColumn(0, _T("Filename"), LVCFMT_LEFT, 138);
        m_ringList.ShowWindow_(SW_HIDE);


        m_pImageList1 = new CImageList();
        m_pImageList1->Create(12, 12, ILC_COLOR32|ILC_MASK, 6, 6);   
        CBitmap bm; 	
        bm.LoadBitmap(Data::g_listctrlSelectBMPID[1][Data::g_skinstyle]);
        m_pImageList1->Add(&bm, RGB(255, 0, 255)); 
        bm.DeleteObject();
        bm.LoadBitmap(Data::g_listctrlSelectBMPID[0][Data::g_skinstyle]);
        m_pImageList1->Add(&bm, RGB(255, 0, 255)); 
        bm.DeleteObject();
        bm.LoadBitmap(Data::g_listctrlSelectBMPID[2][Data::g_skinstyle]);
        m_pImageList1->Add(&bm, RGB(255, 0, 255)); 
        bm.DeleteObject();
        bm.LoadBitmap(Data::g_listctrlSelectBMPID[3][Data::g_skinstyle]);
        m_pImageList1->Add(&bm, RGB(255, 0, 255)); 
        bm.DeleteObject();
        bm.LoadBitmap(Data::g_listctrlSelectBMPID[4][Data::g_skinstyle]);
        m_pImageList1->Add(&bm, RGB(255, 0, 255)); 
        bm.DeleteObject();
        bm.LoadBitmap(Data::g_listctrlSelectBMPID[5][Data::g_skinstyle]);
        m_pImageList1->Add(&bm, RGB(255, 0, 255)); 
        bm.DeleteObject();


        m_ringList.SetImageList(m_pImageList1, LVSIL_SMALL);

        m_ringStatic.Create(L"", WS_CHILD, CRect(354,288,673,412), this);
        m_ringStatic.SetBorder(TRUE);
        */

        m_cmbRing.Create(WS_CHILD|WS_VISIBLE, CRect(354,238,608,460), this, IDC_COMBOBOX_SETTING_RING);

        strTemp = Data::LanguageResource::Get(Data::RI_SETTING_ENABLEAUTOLEAVEWORD);
        str = strTemp.c_str();
        m_chbEnableAutoLeaveWord.Create(str, WS_CHILD|WS_VISIBLE|BS_CHECKBOX, CRect(166, 294, 166+35, 294+32), this, IDC_CHECK_SETTING_AUTOLEAVEWORD);
        m_chbEnableAutoLeaveWord.SetIcon(IDI_ICON_CHECK1, CSize(32, 32), IDI_ICON_CHECK0, CSize(32, 32));
        SetButtonDefaultColor(&m_chbEnableAutoLeaveWord);	

        // 	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_RINGTIMES);
        // 	str = strTemp.c_str();
        // 	m_sticRingTimes.Create(str, WS_CHILD|WS_VISIBLE, CRect(100, 144, 186, 160), this);
        // 	m_sticRingTimes.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);

        m_cmbRingTimes.Create(WS_CHILD|WS_VISIBLE, CRect(353, 304, 466, 460), this, IDC_COMBOBOX_SETTING_RINGTIMES);
        m_cmbAutoRecoedeTimes.Create(WS_CHILD|WS_VISIBLE, CRect(562, 304, 675, 460), this, IDC_COMBOBOX_SETTING_RINGTIMES);


        // 	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_LEAVEWORD);
        // 	str = strTemp.c_str();
        // 	m_sticAutoLeaveWord.Create(str, WS_CHILD|WS_VISIBLE, CRect(104, 165, 186, 180), this);
        // 	m_sticAutoLeaveWord.SetColor(RGB(0, 0, 0), RGB(78, 165, 229));
        // 	m_cmbAutoLeaveWordTip.Create(WS_CHILD|WS_VISIBLE, CRect(190,162,350,230), this, IDC_COMBOBOX_SETTING_AUTOLEAVEWORD);

        // 	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_RECORDLEAVEWORD);
        // 	str = strTemp.c_str();
        // 	m_btnRecardLeaveWordTip.Create(str, Data::g_buttonArcBMPSETTINGID[0][Data::g_skinstyle], Data::g_buttonArcBMPSETTINGID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(270, 142, 326, 162), this, IDC_BUTTON_SETTING_RECORD);
        // 	m_btnRecardLeaveWordTip.SetBackRGB(Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
        // 
        // 	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_RECORDLEAVEWORD);
        // 	str = strTemp.c_str();
        // 	m_btnPlayLeaveWordTip.Create(str, Data::g_buttonArcBMPSETTINGID[0][Data::g_skinstyle], Data::g_buttonArcBMPSETTINGID[1][Data::g_skinstyle], WS_CHILD/*|WS_VISIBLE*/, CRect(334, 142, 390, 162), this, IDC_BUTTON_SETTING_PLAY);
        // 	m_btnPlayLeaveWordTip.SetBackRGB(Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
        // 
        // 
        // 	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_DEFINESPEEDBUTTON);
        // 	str = strTemp.c_str();
        // 	m_btnDefineSpeedButton.Create(str, Data::g_buttonArcBMPSETTINGID[0][Data::g_skinstyle], Data::g_buttonArcBMPSETTINGID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(100, 164, 156, 184), this, IDC_BUTTON_SETTING_DEFINEFASTDAIL);
        // 	m_btnDefineSpeedButton.SetBackRGB(Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
        // 
        // 	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_DEFINESPEEDBUTTONSTC);
        // 	str = strTemp.c_str();
        // 	m_sticDefineSpeedButton.Create(str, WS_CHILD|WS_VISIBLE, CRect(160, 166, 410, 182), this);
        // 	m_sticDefineSpeedButton.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);

        //browser======================================================================================================
        strTemp = Data::LanguageResource::Get(Data::RI_SETTING_CONNECTAUTO);
        str = strTemp.c_str();
        str = _T("");
        m_rdoConnectAuto.Create(str, WS_CHILD|WS_VISIBLE/*|BS_RADIOBUTTON*/, CRect(182-15, 62-4, 214-15, 100-4), this, IDC_RADIO_SETTING_CONNECTAUTO);
        m_rdoConnectAuto.SetColor(RGB(0, 0, 0), RGB(237, 237, 237));//Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
        strTemp = Data::LanguageResource::Get(Data::RI_SETTING_CONNECTMANUAL);
        str = strTemp.c_str();
        str = _T("");
        m_rdoConnectADSL.Create(str, WS_CHILD|WS_VISIBLE/*|BS_RADIOBUTTON*/, CRect(182-15, 102-4, 214-15, 136-4), this, IDC_RADIO_SETTINT_CONNECTADSL);
        m_rdoConnectADSL.SetColor(RGB(0, 0, 0), RGB(237, 237, 237));//Data::g_allFramInRectBackRGB[Data::g_skinstyle]);

        CButton *pButton[2];
        pButton[0] = &m_rdoConnectAuto;
        pButton[1] = &m_rdoConnectADSL;

        m_rdoConnectAuto.SetGroupButton(pButton, 2);
        m_rdoConnectADSL.SetGroupButton(pButton, 2);

        // 	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_ADSLNAME);
        // 	str = strTemp.c_str();
        // 	m_sticADSLName.Create(str, WS_CHILD|WS_VISIBLE, CRect(105, 64, 170, 80), this);
        // 	m_sticADSLName.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);

        m_edtADSLName.Create(ES_AUTOHSCROLL|WS_CHILD|WS_VISIBLE, CRect(295-15, 140-4, 433-15, 171-4), this, IDC_EDIT_SETTING_ADSLNAME);
        m_edtADSLName.SetLimitText(15);

        // 	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_ADSLPASSWORD);
        // 	str = strTemp.c_str();
        // 	m_sticADSLPassword.Create(str, WS_CHILD|WS_VISIBLE, CRect(304, 64, 340, 80), this);
        // 	m_sticADSLPassword.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);

        m_edtADSLPassword.Create(WS_CHILD|WS_VISIBLE|ES_AUTOHSCROLL|ES_PASSWORD, CRect(512-15, 140-4, 649-15, 171-4), this, IDC_EDIT_SETTING_ADSLPASSWORD);
        m_edtADSLPassword.SetLimitText(15);


        strTemp = Data::LanguageResource::Get(Data::RI_SETTING_USEDYNAMICIP);
        str = strTemp.c_str();
        str = _T("");
        m_rdoGetIPDHCP.Create(str, WS_CHILD|WS_VISIBLE/*|BS_RADIOBUTTON*/, CRect(182-15, 174-4, 214-15, 208-4), this, IDC_RADIO_SETTING_DHCP);
        m_rdoGetIPDHCP.SetColor(RGB(0, 0, 0), RGB(237, 237, 237));//Data::g_allFramInRectBackRGB[Data::g_skinstyle]);

        strTemp = Data::LanguageResource::Get(Data::RI_SETTING_USESTATICIIP);
        str = strTemp.c_str();
        str = _T("");
        m_rdoGetIPStatic.Create(str, WS_CHILD|WS_VISIBLE, CRect(182-15, 211-4, 214-15, 245-4), this, IDC_RADIO_SETTING_STATIC);
        m_rdoGetIPStatic.SetColor(RGB(0, 0, 0), RGB(237, 237, 237));//Data::g_allFramInRectBackRGB[Data::g_skinstyle]);

        pButton[0] = &m_rdoGetIPDHCP;
        pButton[1] = &m_rdoGetIPStatic;

        m_rdoGetIPDHCP.SetGroupButton(pButton, 2);
        m_rdoGetIPStatic.SetGroupButton(pButton, 2);

        // 	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_IP);
        // 	str = strTemp.c_str();
        // 	m_sticIP.Create(str, WS_CHILD|WS_VISIBLE, CRect(105, 122, 170, 138), this);
        // 	m_sticIP.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
        // 
        // 	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_MASK);
        // 	str = strTemp.c_str();
        // 	m_sticMask.Create(str, WS_CHILD|WS_VISIBLE, CRect(105, 140, 170, 156), this);
        // 	m_sticMask.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
        // 	
        // 	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_GATEWAY);
        // 	str = strTemp.c_str();
        // 	m_sticGateway.Create(str, WS_CHILD|WS_VISIBLE, CRect(105, 158, 170, 174), this);
        // 	m_sticGateway.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
        // 	
        // 	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_DNS);
        // 	str = strTemp.c_str();
        // 	m_sticDNS.Create(str, WS_CHILD|WS_VISIBLE, CRect(105, 176, 170, 192), this);
        // 	m_sticDNS.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);

        for(int i = 0; i < 4; i++)
        {
            m_edtIP[i].Create(WS_CHILD|WS_VISIBLE, CRect(329+i*58-15, 252-4, 375+i*58-15, 278-4), this, IDC_EDIT_SETTING_IP, 26);
            m_edtIP[i].SetLimitText(3);
            m_edtIP[i].SetLimitDiagital();

            m_edtMask[i].Create(WS_CHILD|WS_VISIBLE, CRect(329+i*58-15, 288-4, 375+i*58-15, 314-4), this, IDC_EDIT_SETTING_MASK, 26);
            m_edtMask[i].SetLimitText(3);
            m_edtMask[i].SetLimitDiagital();

            m_edtGateway[i].Create(WS_CHILD|WS_VISIBLE, CRect(329+i*58-15, 324-4, 375+i*58-15, 350-4), this, IDC_EDIT_SETTING_GATEWAY, 26);
            m_edtGateway[i].SetLimitText(3);
            m_edtGateway[i].SetLimitDiagital();

            m_edtDNS[i].Create(WS_CHILD|WS_VISIBLE, CRect(329+i*58-15, 360-4, 375+i*58-15, 386-4), this, IDC_EDIT_SETTING_DNS, 26);
            m_edtDNS[i].SetLimitText(3);
            m_edtDNS[i].SetLimitDiagital();
        }


        //systom======================================================================================================
        strTemp = Data::LanguageResource::Get(Data::RI_SETTING_ENABLESP);
        str = strTemp.c_str();
        m_chbEnnabeScreenProtect.Create(str, WS_CHILD|WS_VISIBLE|BS_CHECKBOX, CRect(182-44, 68-4, 217-44, 100-4), this, IDC_CHECK_SETTING_SCREENPROTECT);
        m_chbEnnabeScreenProtect.SetIcon(IDI_ICON_CHECK1, CSize(32, 32), IDI_ICON_CHECK0, CSize(32, 32));
        SetButtonDefaultColor(&m_chbEnnabeScreenProtect);	

        // 	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_SPTYPE);
        // 	str = strTemp.c_str();
        // 	m_sticScreenProtctType.Create(str, WS_CHILD|WS_VISIBLE, CRect(100, 46, 170, 62), this);
        // 	m_sticScreenProtctType.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
        // 
        strTemp = Data::LanguageResource::Get(Data::RI_SETTING_TIMESP);
        str = strTemp.c_str();
        m_rdoTimeScreen.Create(str, WS_CHILD|WS_VISIBLE/*|BS_CHECKBOX*/, CRect(312-37, 101-4, 344-37, 135-4), this, IDC_RADIO_SETTING_TIMESCREEN);
        m_rdoTimeScreen.SetColor(RGB(0, 0, 0), RGB(237, 237, 237));//Data::g_allFramInRectBackRGB[Data::g_skinstyle]);

        strTemp = Data::LanguageResource::Get(Data::RI_SETTING_IMAGESP);
        str = strTemp.c_str();
        m_rdoImageScreen.Create(str, WS_CHILD|WS_VISIBLE/*|BS_CHECKBOX*/, CRect(442-37, 101-4, 474-37, 135-4), this, IDC_RADIO_SETTING_IMAGESCREEN);
        m_rdoImageScreen.SetColor(RGB(0, 0, 0), RGB(237, 237, 237));//Data::g_allFramInRectBackRGB[Data::g_skinstyle]);

        pButton[0] = &m_rdoTimeScreen;
        pButton[1] = &m_rdoImageScreen;

        m_rdoTimeScreen.SetGroupButton(pButton, 2);
        m_rdoImageScreen.SetGroupButton(pButton, 2);

        m_cmbWaitTime.Create(WS_CHILD|WS_VISIBLE, CRect(338-37, 136-4, 476-37, 410-4), this, IDC_COMBOBOX_SETTING_WAITTIME);

        m_cmbRingVolume.Create(WS_CHILD|WS_VISIBLE, CRect(338-37, 221-4, 476-37, 410-4), this, 0xFFFF);
        m_cmbSystemVolume.Create(WS_CHILD|WS_VISIBLE, CRect(338-37, 263-4, 476-37, 460), this, 0xFFFF);

        strTemp = Data::LanguageResource::Get(Data::RI_SETTING_ENABLEPASSWORD);
        str = strTemp.c_str();
        m_chbEnablePassword.Create(str, WS_CHILD|WS_VISIBLE|BS_CHECKBOX, CRect(218-37, 176-4, 253-37, 208-4), this, IDC_CHECK_SETTING_SPPASSWORD);
        m_chbEnablePassword.SetIcon(IDI_ICON_CHECK1, CSize(32, 32), IDI_ICON_CHECK0, CSize(32, 32));
        SetButtonDefaultColor(&m_chbEnablePassword);	

        // 	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_SETPASSWORD);
        // 	str = strTemp.c_str();
        // 	m_btnSetPassword.Create(str, Data::g_buttonArcBMPSETTINGID[0][Data::g_skinstyle], Data::g_buttonArcBMPSETTINGID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(244, 82, 300, 102), this, IDC_BUTTON_SETTING_SETPASSWORD);
        // 	m_btnSetPassword.SetBackRGB(Data::g_allFramInRectBackRGB[Data::g_skinstyle]);

        //¼���ļ��洢λ��
        // 	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_SOUNDSAVEPATH);
        // 	str = strTemp.c_str();
        // 	m_sticSoundSavePath.Create(str, WS_CHILD|WS_VISIBLE, CRect(340, 212, 476, 242), this);
        // 	m_sticSoundSavePath.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);

        m_cmbSoundSavePath.Create(WS_CHILD, CRect(338-37, 178-4, 476-37, 410-4), this, IDC_COMBOBOX_SETTING_SOUNDSAVEPATH);

        //ʱ������
        // 	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_SYSTEMTIME);
        // 	str = strTemp.c_str();
        // 	m_stcTime.Create(str, WS_CHILD|WS_VISIBLE, CRect(86, 128, 170, 146), this);
        // 	m_stcTime.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
        m_dtDate.Create(WS_VISIBLE|WS_CHILD|DTS_SHORTDATEFORMAT, CRect(319-13, 317-4, 319+184-13, 317+32-4), this, IDC_SETTING_DATE, 28, CDateCtrl::modeDate);
        m_dtTime.Create(WS_VISIBLE|WS_CHILD|DTS_TIMEFORMAT, CRect(521-13, 317-4, 521+154-13, 317+32-4), this, IDC_SETTING_TIME, 28,  CDateCtrl::modeTime);

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
            _T("����"));                 // lpszFacename
        m_dtDate.SetFont(&m_Font);
        m_dtTime.SetFont(&m_Font);

        CTime time1 = CTime(2007, 1, 1, 0, 0, 0);
        CTime time2 = CTime(2031, 1, 1, 0, 0, 0);
        //	m_dtDate.SetRange(&time1, &time2);
        GetLocalTime(&m_curtime);
        m_dtDate.SetTime(m_curtime);
        m_dtTime.SetTime(m_curtime);


        //

        // 	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_SCREENADJUSTSTC);
        // 	str = strTemp.c_str();
        // 	m_sticTouchAdjust.Create(str, WS_CHILD|WS_VISIBLE, CRect(160, 166, 410, 182), this);
        // 	m_sticTouchAdjust.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
        // 
        // 	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_SCREENADJUST);
        // 	str = strTemp.c_str();
        // 	m_btnTouchAdjust.Create(str, Data::g_buttonArcBMPSETTINGID[0][Data::g_skinstyle], Data::g_buttonArcBMPSETTINGID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(100, 164, 156, 184), this, IDC_BUTTON_SETTING_ADJUSTTOUCH);
        // 	m_btnTouchAdjust.SetBackRGB(Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
        //info======================================================================================================
        strTemp = Data::LanguageResource::Get(Data::RI_SETTING_SYSTOMTITLE);
        str = strTemp.c_str();
        str = _T("");
        m_sticSystomTitle.Create(str, WS_CHILD|WS_VISIBLE, CRect(222-15, 76-4, 510-15, 102-4), this);
        m_sticSystomTitle.SetColor(RGB(0, 0, 0), RGB(237, 237, 237));//Data::g_allFramInRectBackRGB[Data::g_skinstyle]);

        // 	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_SOFTVERSION);
        // 	str = strTemp.c_str();
        // 	m_sticSoftwareVersion1.Create(str, WS_CHILD|WS_VISIBLE, CRect(104, 50, 170, 66), this);
        // 	m_sticSoftwareVersion1.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
        m_sticSoftwareVersion.Create(s_VerSionTitle, WS_CHILD|WS_VISIBLE, CRect(328-15, 108-4, 450-15, 134-4), this, IDC_SETTING_VERSION);
        m_sticSoftwareVersion.SetColor(RGB(255, 0, 0), RGB(237, 237, 237));//Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
        // 	m_sticSoftwareVersion.SetClicked(TRUE);
        m_sticSoftwareVersion.SetUnderLine(TRUE);

        m_editVersion.Create(WS_VISIBLE|WS_CHILD|WS_BORDER|ES_MULTILINE | ES_WANTRETURN | WS_VSCROLL, CRect(170-15, 135-4, 170+512-15, 135+282-4), this, IDC_SETTING_EDITVERSION );
        m_editVersion.ShowWindow(SW_HIDE);
        m_editVersion.SetWindowText(s_VerSion);

        // 	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_HARDVERSION);
        // 	str = strTemp.c_str();
        // 	m_sticHardwareVersion1.Create(str, WS_CHILD|WS_VISIBLE, CRect(254, 50, 320, 66), this);
        // 	m_sticHardwareVersion1.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
        //	str = "1.0.0.0";
        //	m_sticHardwareVersion.Create(str, WS_CHILD|WS_VISIBLE, CRect(542-15, 108-4, 650-15, 134-4), this);
        //	m_sticHardwareVersion.SetColor(RGB(0, 0, 0), RGB(237, 237, 237));//Data::g_allFramInRectBackRGB[Data::g_skinstyle]);

        // 	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_FLASHSIZE);
        // 	str = strTemp.c_str();
        // 	m_sticMemorySize1.Create(str, WS_CHILD|WS_VISIBLE, CRect(104, 68, 170, 84), this);
        // 	m_sticMemorySize1.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
        str = "";
        m_sticMemorySize.Create(str, WS_CHILD|WS_VISIBLE, CRect(328-15, 144-4, 650-15, 170-4), this);
        m_sticMemorySize.SetColor(RGB(0, 0, 0), RGB(237, 237, 237));//Data::g_allFramInRectBackRGB[Data::g_skinstyle]);

        // 	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_SDSTORAGESIZE);
        // 	str = strTemp.c_str();
        // 	m_sticStorageSize1.Create(str, WS_CHILD|WS_VISIBLE, CRect(104, 86, 170, 102), this);
        // 	m_sticStorageSize1.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
        str = "";
        m_sticStorageSize.Create(str, WS_CHILD|WS_VISIBLE, CRect(328-15, 180-4, 650-15, 206-4), this);
        m_sticStorageSize.SetColor(RGB(0, 0, 0), RGB(237, 237, 237));//Data::g_allFramInRectBackRGB[Data::g_skinstyle]);

        // 	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_CONTACTSIZE);
        // 	str = strTemp.c_str();
        // 	m_sticContactSize1.Create(str, WS_CHILD|WS_VISIBLE, CRect(124, 104, 190, 120), this);
        // 	m_sticContactSize1.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
        str = "";
        m_sticContactSize.Create(str, WS_CHILD|WS_VISIBLE, CRect(360-15, 234-4, 440-15, 260-4), this);
        m_sticContactSize.SetColor(RGB(0, 0, 0), RGB(237, 237, 237));//Data::g_allFramInRectBackRGB[Data::g_skinstyle]);

        // 	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_CONTACTINFOSIZE);
        // 	str = strTemp.c_str();
        // 	m_sticContactInfoSize1.Create(str, WS_CHILD|WS_VISIBLE, CRect(124, 124, 190, 140), this);
        // 	m_sticContactInfoSize1.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
        str = "";
        m_sticContactInfoSize.Create(str, WS_CHILD|WS_VISIBLE, CRect(360-15, 274-4, 440-15, 300-4), this);
        m_sticContactInfoSize.SetColor(RGB(0, 0, 0), RGB(237, 237, 237));//Data::g_allFramInRectBackRGB[Data::g_skinstyle]);

        // 	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_SOUNDSIZE);
        // 	str = strTemp.c_str();
        // 	m_sticSoundSize1.Create(str, WS_CHILD|WS_VISIBLE, CRect(124, 144, 190, 160), this);
        // 	m_sticSoundSize1.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
        str = "";
        m_sticSoundSize.Create(str, WS_CHILD|WS_VISIBLE, CRect(360-15, 314-4, 440-15, 340-4), this);
        m_sticSoundSize.SetColor(RGB(0, 0, 0), RGB(237, 237, 237));//Data::g_allFramInRectBackRGB[Data::g_skinstyle]);

        // 	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_INFORMATIONSIZE);
        // 	str = strTemp.c_str();
        // 	m_sticInformationSize1.Create(str, WS_CHILD|WS_VISIBLE, CRect(124, 164, 190, 180), this);
        // 	m_sticInformationSize1.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
        str = "";
        m_sticInformationSize.Create(str, WS_CHILD|WS_VISIBLE, CRect(194-15, 164-4, 270-15, 180-4), this);
        m_sticInformationSize.SetColor(RGB(0, 0, 0), RGB(237, 237, 237));//Data::g_allFramInRectBackRGB[Data::g_skinstyle]);

        // 	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_CLEARCONTACT);
        // 	str = strTemp.c_str();
        // 	m_btnClearContact.Create(str, Data::g_buttonArcBMPSETTINGID[0][Data::g_skinstyle], Data::g_buttonArcBMPSETTINGID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(274, 102, 330, 122), this, IDC_BUTTON_SETTING_CLEARCONTACT);
        // 	m_btnClearContact.SetBackRGB(Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
        // 
        // 	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_CLEARCONTACTINFO);
        // 	str = strTemp.c_str();
        // 	m_btnClearContactInfo.Create(str, Data::g_buttonArcBMPSETTINGID[0][Data::g_skinstyle], Data::g_buttonArcBMPSETTINGID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(274, 122, 330, 142), this, IDC_BUTTON_SETTING_CLEARCONTACTINFO);
        // 	m_btnClearContactInfo.SetBackRGB(Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
        // 
        // 	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_CLEARSOUND);
        // 	str = strTemp.c_str();
        // 	m_btnClearSound.Create(str, Data::g_buttonArcBMPSETTINGID[0][Data::g_skinstyle], Data::g_buttonArcBMPSETTINGID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(274, 142, 330, 162), this, IDC_BUTTON_SETTING_CLEARSOUND);
        // 	m_btnClearSound.SetBackRGB(Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
        // 
        // 	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_CLEARINFORMATION);
        // 	str = strTemp.c_str();
        // 	m_btnClearInformation.Create(str, Data::g_buttonArcBMPSETTINGID[0][Data::g_skinstyle], Data::g_buttonArcBMPSETTINGID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(274, 162, 330, 182), this, IDC_BUTTON_SETTING_CLEARINFO);
        // 	m_btnClearInformation.SetBackRGB(Data::g_allFramInRectBackRGB[Data::g_skinstyle]);

        // 	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_VERSIONRIGHT);
        // 	str = strTemp.c_str();
        // 	m_sticVersionRight1.Create(str, WS_CHILD|WS_VISIBLE, CRect(104, 182, 170, 198), this);
        // 	m_sticVersionRight1.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
        str = _T("");
        m_sticVersionRight.Create(str, WS_CHILD|WS_VISIBLE, CRect(0, 0, 0, 0), this);
        m_sticVersionRight.SetColor(RGB(0, 0, 0), RGB(237, 237, 237));//Data::g_allFramInRectBackRGB[Data::g_skinstyle]);

        //���ݵ��뵼��
        /*	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_TITILEBACKUP);
        str = strTemp.c_str();
        m_stcBackUP.Create(str, WS_VISIBLE|WS_VISIBLE, CRect(90, 30, 280, 50), this);
        m_stcBackUP.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
        strTemp = Data::LanguageResource::Get(Data::RI_SETTING_TITLERESTORE);
        //str = strTemp.c_str();
        //m_stcReStore.Create(str, WS_VISIBLE|WS_VISIBLE, CRect(90, 53, 300, 73), this);
        //m_stcReStore.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);

        strTemp = Data::LanguageResource::Get(Data::RI_SETTING_BACKUP);
        str = strTemp.c_str();
        m_btnBackUP.Create(str, Data::g_buttonArcBMPSETTINGID[0][Data::g_skinstyle], Data::g_buttonArcBMPSETTINGID[1][Data::g_skinstyle], WS_VISIBLE|WS_VISIBLE, CRect(282, 30, 338, 50), this, IDC_SETTING_BACKUP);
        m_btnBackUP.SetBackRGB(Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
        strTemp = Data::LanguageResource::Get(Data::RI_SETTING_RESTORE);
        str = strTemp.c_str();
        m_btnReStore.Create(str, Data::g_buttonArcBMPSETTINGID[0][Data::g_skinstyle], Data::g_buttonArcBMPSETTINGID[1][Data::g_skinstyle], WS_VISIBLE|WS_VISIBLE, CRect(342, 30, 398, 50), this, IDC_SETTING_RESTORE);
        m_btnReStore.SetBackRGB(Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
        strTemp = Data::LanguageResource::Get(Data::RI_SETTING_TITLE);
        str = strTemp.c_str();
        m_stcTitle.Create(str, WS_VISIBLE|WS_VISIBLE, CRect(90, 53, 180, 73), this);
        m_stcTitle.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);

        strTemp = Data::LanguageResource::Get(Data::RI_SETTING_OUT);
        str = strTemp.c_str();
        m_btnOut.Create(str, Data::g_buttonArcBMPSETTINGID[0][Data::g_skinstyle], Data::g_buttonArcBMPSETTINGID[1][Data::g_skinstyle], WS_VISIBLE|WS_VISIBLE, CRect(182, 53, 238, 73), this, IDC_SETTING_OUT);
        m_btnOut.SetBackRGB(Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
        */
        /*
        m_cmbTitle.Create(WS_VISIBLE|WS_CHILD, CRect(152, 51, 252, 173), this, IDC_SETTING_CMBSELECT);
        strTemp = Data::LanguageResource::Get(Data::RI_SETTING_RECORDE);
        str = strTemp.c_str();
        m_cmbTitle.AddString(str);
        strTemp = Data::LanguageResource::Get(Data::RI_SETTING_RING);
        str = strTemp.c_str();
        m_cmbTitle.AddString(str);
        strTemp = Data::LanguageResource::Get(Data::RI_SETTING_PHOTO);
        str = strTemp.c_str();
        m_cmbTitle.AddString(str);
        strTemp = Data::LanguageResource::Get(Data::RI_SETTING_MP3);
        str = strTemp.c_str();
        m_cmbTitle.AddString(str);
        strTemp = Data::LanguageResource::Get(Data::RI_SETTING_VIDEO);
        str = strTemp.c_str();
        m_cmbTitle.AddString(str);

        strTemp = Data::LanguageResource::Get(Data::RI_SETTING_LOCALFILE);
        str = strTemp.c_str();
        m_stcLocalTitle.Create(str, WS_VISIBLE|WS_VISIBLE, CRect(90, 75, 150, 93), this);
        m_stcLocalTitle.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
        strTemp = Data::LanguageResource::Get(Data::RI_SETTING_USBFILE);
        str = strTemp.c_str();
        m_stcUSBTitle.Create(str, WS_VISIBLE|WS_VISIBLE, CRect(282, 75, 405, 93), this);
        m_stcUSBTitle.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);

        m_lstLocal.Create(WS_CHILD|WS_VISIBLE|LVS_REPORT|LVS_NOCOLUMNHEADER|LVS_NOSORTHEADER, CRect(90, 95, 215, 193), this, IDC_SETTING_LSTLOCAL, TRUE, 1);
        m_lstLocal.SetListColor(Data::g_listctrlBackRGB1[Data::g_skinstyle], Data::g_listctrlBackRGB2[Data::g_skinstyle]);
        m_lstLocal.InsertColumn(0, _T("Filename"), LVCFMT_LEFT, 105);
        m_lstUSB.Create(WS_CHILD|WS_VISIBLE|LVS_REPORT|LVS_NOCOLUMNHEADER|LVS_NOSORTHEADER, CRect(282, 95, 405, 193), this, IDC_SETTING_LSTUSB, TRUE, 1);
        m_lstUSB.SetListColor(Data::g_listctrlBackRGB1[Data::g_skinstyle], Data::g_listctrlBackRGB2[Data::g_skinstyle]);
        m_lstUSB.InsertColumn(0, _T("Filename"), LVCFMT_LEFT, 105);

        m_stcLocal.Create(L"", WS_VISIBLE|WS_VISIBLE, CRect(89, 94, 216, 194), this);
        m_stcLocal.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
        m_stcLocal.SetBorder(TRUE);
        m_stcUSB.Create(L"", WS_VISIBLE|WS_VISIBLE, CRect(281, 94, 406, 194), this);
        m_stcUSB.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
        m_stcUSB.SetBorder(TRUE);

        strTemp = Data::LanguageResource::Get(Data::RI_RECORD_DELETE);
        str = strTemp.c_str();
        m_btnDelete.Create(str, Data::g_buttonArcBMPSETTINGID[0][Data::g_skinstyle], Data::g_buttonArcBMPSETTINGID[1][Data::g_skinstyle], WS_VISIBLE|WS_VISIBLE, CRect(156, 72, 212, 92), this, IDC_SETTING_DELETE);
        m_btnDelete.SetBackRGB(Data::g_allFramInRectBackRGB[Data::g_skinstyle]);

        strTemp = Data::LanguageResource::Get(Data::RI_VEDIO_ALLBTN);
        str = strTemp.c_str();
        m_btnAllSelect.Create(str, Data::g_buttonArcBMPSETTINGID[0][Data::g_skinstyle], Data::g_buttonArcBMPSETTINGID[1][Data::g_skinstyle], WS_VISIBLE|WS_VISIBLE, CRect(221, 104, 277, 124), this, IDC_SETTING_ALLSELECT);
        m_btnAllSelect.SetBackRGB(Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
        strTemp = Data::LanguageResource::Get(Data::RI_VEDIO_DELETEALLBTN);
        str = strTemp.c_str();
        m_btnClearSelect.Create(str, Data::g_buttonArcBMPSETTINGID[0][Data::g_skinstyle], Data::g_buttonArcBMPSETTINGID[1][Data::g_skinstyle], WS_VISIBLE|WS_VISIBLE, CRect(221, 127, 277, 147), this, IDC_SETTING_CLEARSELECT);
        m_btnClearSelect.SetBackRGB(Data::g_allFramInRectBackRGB[Data::g_skinstyle]);

        strTemp = Data::LanguageResource::Get(Data::RI_SETTING_IN);
        str = strTemp.c_str();
        m_btnIn.Create(str, Data::g_buttonArcBMPSETTINGID[0][Data::g_skinstyle], Data::g_buttonArcBMPSETTINGID[1][Data::g_skinstyle], WS_VISIBLE|WS_VISIBLE, CRect(221, 150, 277, 170), this, IDC_SETTING_IN);
        m_btnIn.SetBackRGB(Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
        strTemp = Data::LanguageResource::Get(Data::RI_SETTING_OUT);
        str = strTemp.c_str();
        m_btnOut.Create(str, Data::g_buttonArcBMPSETTINGID[0][Data::g_skinstyle], Data::g_buttonArcBMPSETTINGID[1][Data::g_skinstyle], WS_VISIBLE|WS_VISIBLE, CRect(221, 173, 277, 193), this, IDC_SETTING_OUT);
        m_btnOut.SetBackRGB(Data::g_allFramInRectBackRGB[Data::g_skinstyle]);

        m_sticPanel.Create(_T(""), WS_CHILD|WS_VISIBLE, CRect(416, 26, 472, 198), this);
        m_sticPanel.SetColor(RGB(0, 0, 0), Data::g_allFrameInFrameLine1RGB[0][Data::g_skinstyle]);

        m_sticPanel2.Create(_T(""), WS_CHILD|WS_VISIBLE, CRect(81, 27, 416, 198), this);
        m_sticPanel2.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
        /*
        m_sticBackground.Create(CRect(0, 0, 480 * 125 / 100, 204), this, 2);

        TextStruct ts[3];
        memset(ts, 0, sizeof(TextStruct) * 3);

        ts[0].txtRect = CRect(0, 0, 80, 20);
        ts[0].txtFontSize = 16;
        ts[0].sAlign = DT_CENTER | DT_BOTTOM;
        memcpy(ts[0].sTxt, Data::LanguageResource::Get(Data::RI_RECORD_TYPE).c_str(), Data::LanguageResource::Get(Data::RI_RECORD_TYPE).length());

        ts[1].txtRect = CRect(81, 0, 415, 20);
        ts[1].txtFontSize = 16;
        ts[1].sAlign = DT_CENTER | DT_BOTTOM;
        memcpy(ts[1].sTxt, Data::LanguageResource::Get(Data::RI_SETTING_DETAILTITLE).c_str(), Data::LanguageResource::Get(Data::RI_SETTING_DETAILTITLE).length());

        ts[2].txtRect = CRect(415, 0, 470, 20);
        ts[2].txtFontSize = 16;
        ts[2].sAlign = DT_CENTER | DT_BOTTOM;
        memcpy(ts[2].sTxt, Data::LanguageResource::Get(Data::RI_RECORD_OPERATION).c_str(), Data::LanguageResource::Get(Data::RI_RECORD_OPERATION).length());

        m_sticBackground.SetTextStruct(ts, 3);
        */


        //blacklight
        m_chbContrlBlackLight.Create(str, WS_CHILD|WS_VISIBLE|BS_CHECKBOX, CRect(153, 169, 153+35, 169+32), this, IDC_CHECK_SETTING_SCREENPROTECT);
        m_chbContrlBlackLight.SetIcon(IDI_ICON_CHECK1, CSize(32, 32), IDI_ICON_CHECK0, CSize(32, 32));
        SetButtonDefaultColor(&m_chbContrlBlackLight);	

        m_chbNightContrlBlackLight.Create(str, WS_CHILD|WS_VISIBLE|BS_CHECKBOX, CRect(153, 293, 153+35, 293+32), this, IDC_CHECK_SETTING_SCREENPROTECT);
        m_chbNightContrlBlackLight.SetIcon(IDI_ICON_CHECK1, CSize(32, 32), IDI_ICON_CHECK0, CSize(32, 32));
        SetButtonDefaultColor(&m_chbNightContrlBlackLight);

        m_cmbContrlBlackLightWaitTime.Create(WS_CHILD|WS_VISIBLE, CRect(288,206,431,410), this, IDC_COMBOBOX_SETTING_RINGTIMES);

        m_dtNightContrlBlackLightStartTime.Create(WS_VISIBLE|WS_CHILD, CRect(242, 335, 242+154, 335+32), this, IDC_SETTING_TIME, 28, CDateCtrl::modeTime);
        m_dtNightContrlBlackLightEndTime.Create(WS_VISIBLE|WS_CHILD, CRect(444, 334, 444+154, 334+32), this, IDC_SETTING_TIME, 28, CDateCtrl::modeTime);

        m_MJPGList.Create(L"", WS_VISIBLE|WS_CHILD, CRect(0, 0, 800 * 125 / 100, 420 * 125 / 100), this);
        m_MJPGList.SetCurrentLinkFile(_T(".\\adv\\mjpg\\k1\\����\\ϵͳ����1.xml"));
        m_MJPGList.SetMJPGRect(CRect(0, 0, 800 * 125 / 100, 420 * 125 / 100));

        m_uiType = 0;

        m_pFastDialsDlg = new CFastDialsDlg(this);
        m_pFastDialsDlg->Create(CFastDialsDlg::IDD);

        m_passwordDlg = new CPasswordDlg(this);
        m_passwordDlg->Create(CPasswordDlg::IDD);

        m_copyfileDlg = new copyfileDlg(this);
        m_copyfileDlg->Create(copyfileDlg::IDD);

        // 	m_firWallDlg = new CFireWallDlg(this);
        // 	m_firWallDlg->Create(CFireWallDlg::IDD);


        //	m_lsType.SetItemState(0, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);

        /*
        m_pImageList1 = new CImageList();
        m_pImageList1->Create(12, 12, ILC_COLOR32|ILC_MASK, 5, 5);   
        //HICON hIcon = ::LoadIcon(AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_ICON_CHECK0));   
        //m_pImageList->Add(hIcon);  
        //hIcon = ::LoadIcon(AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_ICON_CHECK1));   
        //m_pImageList->Add(hIcon);   

        bm.LoadBitmap(Data::g_listctrlSelectBMPID[1][Data::g_skinstyle]);
        m_pImageList1->Add(&bm, RGB(255, 0, 255)); 
        bm.DeleteObject();
        bm.LoadBitmap(Data::g_listctrlSelectBMPID[0][Data::g_skinstyle]);
        m_pImageList1->Add(&bm, RGB(255, 0, 255)); 
        bm.DeleteObject();
        bm.LoadBitmap(Data::g_listctrlSelectBMPID[2][Data::g_skinstyle]);
        m_pImageList1->Add(&bm, RGB(255, 0, 255)); 
        bm.DeleteObject();
        bm.LoadBitmap(Data::g_listctrlSelectBMPID[3][Data::g_skinstyle]);
        m_pImageList1->Add(&bm, RGB(255, 0, 255)); 
        bm.DeleteObject();
        bm.LoadBitmap(Data::g_listctrlSelectBMPID[4][Data::g_skinstyle]);
        m_pImageList1->Add(&bm, RGB(255, 0, 255)); 
        bm.DeleteObject();
        m_lstLocal.SetImageList(m_pImageList1, LVSIL_SMALL);
        m_lstUSB.SetImageList(m_pImageList1, LVSIL_SMALL);
        */
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
            ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->SetBackLight(0);    //�رձ���
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

        CString s = Util::StringOp::ToCString(m_pSetting->defaultRingFilename());
        int nStart = 0;
        int n = s.Find(_T("/"), nStart);
        while (n != -1)
        {
            nStart = n+1;
            n = s.Find(_T("/"), nStart);
        }
        CString filename = s.Mid(nStart);
        CString path = s.Mid(0, nStart);

        //	m_ringEdit.SetWindowText(filename);

        SetRingLst(L"");
        m_cmbRing.SetWindowText_(filename);

        //	memset(m_ringDir, 0, 128*2);
        //	memcpy(m_ringDir, path.GetBuffer(path.GetLength()), path.GetLength()*2);

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
        if(nRingCount == 3)
            nn = 0;
        else if(nRingCount == 4)
            nn = 1;
        else if(nRingCount == 5)
            nn = 2;
        else if(nRingCount == 6)
            nn = 3;
        m_cmbRingTimes.SetCurSel(nn);

        m_cmbAutoRecoedeTimes.ResetContent();
        CString sCount_[4] = {_T("15��"), _T("30��"), _T("60��"), _T("90��")};
        m_cmbAutoRecoedeTimes.AddString(sCount_[0]);
        m_cmbAutoRecoedeTimes.AddString(sCount_[1]);
        m_cmbAutoRecoedeTimes.AddString(sCount_[2]);
        m_cmbAutoRecoedeTimes.AddString(sCount_[3]);
        int nTimes = m_pSetting->maxSoundUseSize();    //��ʱ���������������¼�Զ�¼����ʱ��

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

        //��ע����л�ȡIP��Ϣ��ʾ

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
        for(int i = 0; i < 4; i++)
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
            if(index>0)
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

        CString sSecond[5] = {_T("15��"), _T("30��"), _T("1����"), _T("5����"), _T("15����")};
        m_cmbWaitTime.ResetContent();

        m_cmbWaitTime.AddString(sSecond[0]);
        m_cmbWaitTime.AddString(sSecond[1]);
        m_cmbWaitTime.AddString(sSecond[2]);
        m_cmbWaitTime.AddString(sSecond[3]);
        m_cmbWaitTime.AddString(sSecond[4]);

        CString sRingVolume[5] = {_T("���"), _T("�ϴ�"), _T("�м�"), _T("��С"), _T("��С")};
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
        if(nSeconds == 15)
        {
            nSel = 0;
        }
        else if(nSeconds == 30)
        {
            nSel = 1;
        }
        else if(nSeconds == 60)
        {
            nSel = 2;
        }
        else if(nSeconds == 5*60)
        {
            nSel = 3;
        }
        else if(nSeconds == 15*60)
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

        CString sWait[4] = {_T("1����"), _T("5����"), _T("10����"), _T("30����")};
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

        // 	m_sticSoundSavePath;
        // 	m_cmbSoundSavePath;

        // todo: ��������
        // todo: �ٲ�����
        // todo: ϵͳ������ʾ
        // 	m_sticSystomTitle.ShowWindow(TRUE);
        // 	m_sticSoftwareVersion.SetWindowText(Util::StringOp::ToCString(m_pSetting->softwareVersion()));
        // 	m_sticHardwareVersion.SetWindowText(Util::StringOp::ToCString(m_pSetting->hardwareVersion()));
        // 	m_sticMemorySize.SetWindowText(Util::StringOp::ToCString(m_pSetting->memorySize()));
        // 	m_sticStorageSize.SetWindowText(Util::StringOp::ToCString(m_pSetting->storageSize()));
        // 	m_sticContactSize.SetWindowText(Util::StringOp::ToCString(m_pSetting->contactUseSize()));
        // 	m_sticContactInfoSize.SetWindowText(Util::StringOp::ToCString(m_pSetting->contactInfoUseSize()));
        // 	m_sticSoundSize.SetWindowText(Util::StringOp::ToCString(m_pSetting->soundUseSize()));
        // 	m_sticInformationSize.SetWindowText(Util::StringOp::ToCString(m_pSetting->soundUseSize()));
        // 	m_sticVersionRight.SetWindowText(Util::StringOp::ToCString(m_pSetting->copyright()));
    }

    void CSettingDlg::OnButtonSettingOk() 
    {
        //test
        // TODO: Add your control notification handler code here
        // 	POSITION pos = m_lsType.GetFirstSelectedItemPosition();
        // 	if (pos == NULL)
        // 	{
        // 		return;
        // 	}
        // 	int type = m_lsType.GetNextSelectedItem(pos);
        //	if(type == 2)		//��������
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

                ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->SetAlarmList();    //���¼�������
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

        CString s = m_ringDir; s += str;
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
        memset(txt, 0, 4*32);
        Data::IPAddr  nIP[4] = {0, 0, 0, 0};
        for(int i = 0; i < 4; i++)
        {
            m_edtIP[i].GetWindowText(str);
            nIP[0] += ((Util::StringOp::ToInt(Util::StringOp::FromCString(str))) << (i*8));
            m_edtMask[i].GetWindowText(str);
            nIP[1] += ((Util::StringOp::ToInt(Util::StringOp::FromCString(str))) << (i*8));
            m_edtGateway[i].GetWindowText(str);
            nIP[2] += ((Util::StringOp::ToInt(Util::StringOp::FromCString(str))) << (i*8));
            m_edtDNS[i].GetWindowText(str);
            nIP[3] += ((Util::StringOp::ToInt(Util::StringOp::FromCString(str))) << (i*8));
        }

        sprintf(txt[0], "%d.%d.%d.%d", (nIP[0]&0xFF), ((nIP[0] >> 8)&0xFF), ((nIP[0] >> 16)&0xFF), ((nIP[0] >> 24)&0xFF));
        sprintf(txt[1], "%d.%d.%d.%d", (nIP[1]&0xFF), ((nIP[1] >> 8)&0xFF), ((nIP[1] >> 16)&0xFF), ((nIP[1] >> 24)&0xFF));
        if(m_rdoConnectAuto.GetCheck_() != BST_CHECKED && m_rdoGetIPDHCP.GetCheck_() != BST_CHECKED)
            sprintf(txt[2], "");
        else
            sprintf(txt[2], "%d.%d.%d.%d", (nIP[2]&0xFF), ((nIP[2] >> 8)&0xFF), ((nIP[2] >> 16)&0xFF), ((nIP[2] >> 24)&0xFF));
        sprintf(txt[3], "%d.%d.%d.%d", (nIP[3]&0xFF), ((nIP[3] >> 8)&0xFF), ((nIP[3] >> 16)&0xFF), ((nIP[3] >> 24)&0xFF));

        CString sIP = Util::StringOp::ToCString(txt[0]);
        memcpy(ipConfig.IPAddr, (LPCTSTR)sIP, sIP.GetLength()*2);
        sIP = txt[1];
        memcpy(ipConfig.SubnetMask, (LPCTSTR)sIP, sIP.GetLength()*2);
        sIP = txt[2];
        memcpy(ipConfig.Gateway, (LPCTSTR)sIP, sIP.GetLength()*2);
        sIP = txt[3];
        memcpy(ipConfig.DNSAddr, (LPCTSTR)sIP, sIP.GetLength()*2);
        memcpy(ipConfig.WINSAddr, (LPCTSTR)sIP, sIP.GetLength()*2);
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

        int nSecond[5] = {15, 30, 60, 5*60, 15*60};
        int nSel = m_cmbWaitTime.GetCurSel();
        CTimeSpan time = CTimeSpan(0, 0, nSecond[nSel]/60, nSecond[nSel]%60);
        m_pSetting->screenSaverDuration(time);

        int nVolume;
        nVolume = m_cmbSystemVolume.GetCurSel();
        m_pSetting->sysVolume(nVolume);

        ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pMainDlg->m_mainMp3Dlg_->SetVolume();
        ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pMainDlg->m_mainVideoDlg_->SetVolume();

        nVolume = m_cmbRingVolume.GetCurSel();
        ::UINT16 volume[] = {0xFF00, 0xcc00, 0x8800, 0x4400, 0x1000}; //(5-nVolume)*0xFFFF/5;
        ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->phone_->g_RingSound =  volume[nVolume]|(volume[nVolume]<<16);   //(nVolume*-8000/5);
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
        std::string strTemp = "ȷ���ָ�Ĭ��������?";
        ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetDelTip(Util::StringOp::ToCString(strTemp.c_str()));
        ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetHWnd(m_hWnd);
        ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->ShowWindow(TRUE);
    }

    void CSettingDlg::OnButtonSettingRecord()
    {	
    }

    typedef BOOL (WINAPI *_TouchCalibrate)();

    //������У��
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
            AfxMessageBox (L"TouchCalibrate == NULL"); 
            return; 
        }  
        TouchCalibrate (); 
        FreeLibrary(hinstance );
        TCHAR szTemp[256] = {0};
        _tcscpy (szTemp, TEXT("HARDWARE\\DEVICEMAP\\TOUCH"));
        HKEY hKey = NULL;
        LONG hRes = RegOpenKeyEx (HKEY_LOCAL_MACHINE, szTemp, 0, 0, &hKey);
        if (ERROR_SUCCESS == hRes)
        {
            /*	TCHAR  value[256];
            DWORD dwDataSize;
            DWORD dType;
            memset(value, 0, 256*2);
            hRes = RegQueryValueEx(hKey, TEXT("CalibrationData"), NULL, &dType, (LPBYTE)value, &dwDataSize);
            dwDataSize = wcslen(value);
            hRes = RegSetValueEx(hKey, TEXT("CalibrationData"), NULL, REG_DWORD, (LPBYTE)value, dwDataSize);
            */
        }
        LONG l = RegFlushKey(HKEY_LOCAL_MACHINE);
        RegCloseKey(hKey);

        //дע���  ������У��
        /*
        TCHAR szTemp[256] = {0};
        _tcscpy (szTemp, TEXT("HARDWARE\\DEVICEMAP\\TOUCH"));
        HKEY hKey = NULL;
        LONG hRes = RegOpenKeyEx (HKEY_LOCAL_MACHINE, szTemp, 0, 0, &hKey);
        if (ERROR_SUCCESS == hRes)
        {
        TCHAR  value[256];
        DWORD dwDataSize;
        DWORD dType;
        memset(value, 0, 256*2);
        hRes = RegQueryValueEx(hKey, TEXT("CalibrationData"), NULL, &dType, (LPBYTE)value, &dwDataSize);
        dwDataSize = wcslen(value);
        hRes = RegSetValueEx(hKey, TEXT("CalibrationData"), NULL, REG_DWORD, (LPBYTE)value, dwDataSize);
        }
        //RegCloseKey(hKey);
        */
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
        // 	POSITION pos = m_lsType.GetFirstSelectedItemPosition();
        // 	if (pos == NULL)
        // 	{
        // 		return;
        // 	}

        /*
        m_ringList.ShowWindow_(SW_HIDE);
        m_ringStatic.ShowWindow(SW_HIDE);
        */
        m_cmbRing.ShowWindow(SW_HIDE);

        //tel
        //	m_sticLocalAreaNumber.ShowWindow(FALSE);
        m_edtLocalAreaNumber.ShowWindow(FALSE);
        m_chbEnableOutLine.ShowWindow(FALSE);
        m_edtOutLine.ShowWindow(FALSE);
        m_chbEnableIPDial.ShowWindow(FALSE);
        m_edtIPDial.ShowWindow(FALSE);
        m_chbEnableRing.ShowWindow(FALSE);
        //	m_sticDefaultRing.ShowWindow(FALSE);
        //	m_cmbRing.ShowWindow(FALSE);

        /*
        m_ringEdit.ShowWindow(FALSE);
        m_ringButton.ShowWindow(FALSE);
        */
        m_cmbRing.ShowWindow(SW_HIDE);

        m_chbEnableAutoLeaveWord.ShowWindow(FALSE);
        //	m_sticRingTimes.ShowWindow(FALSE);
        m_cmbRingTimes.ShowWindow(FALSE);
        m_cmbAutoRecoedeTimes.ShowWindow(FALSE);
        //	m_sticAutoLeaveWord.ShowWindow(FALSE);
        //	m_cmbAutoLeaveWordTip.ShowWindow(FALSE);
        //	m_btnRecardLeaveWordTip.ShowWindow(FALSE);
        //	m_btnDefineSpeedButton.ShowWindow(FALSE);
        //	m_sticDefineSpeedButton.ShowWindow(FALSE);
        //browser
        m_rdoConnectAuto.ShowWindow(FALSE);
        m_rdoConnectADSL.ShowWindow(FALSE);
        // 	m_sticADSLName.ShowWindow(FALSE);
        // 	m_sticADSLPassword.ShowWindow(FALSE);
        m_edtADSLName.ShowWindow(FALSE);
        m_edtADSLPassword.ShowWindow(FALSE);	
        m_rdoGetIPDHCP.ShowWindow(FALSE);
        m_rdoGetIPStatic.ShowWindow(FALSE);
        // 	m_sticIP.ShowWindow(FALSE);
        // 	m_sticMask.ShowWindow(FALSE);
        // 	m_sticGateway.ShowWindow(FALSE);
        // 	m_sticDNS.ShowWindow(FALSE);
        for(int i = 0; i < 4; i++)
        {
            m_edtIP[i].ShowWindow(FALSE);
            m_edtMask[i].ShowWindow(FALSE);
            m_edtGateway[i].ShowWindow(FALSE);
            m_edtDNS[i].ShowWindow(FALSE);
        }

        //systom
        m_chbEnnabeScreenProtect.ShowWindow(FALSE);
        //	m_sticScreenProtctType.ShowWindow(FALSE);
        m_rdoTimeScreen.ShowWindow(FALSE);
        m_rdoImageScreen.ShowWindow(FALSE);
        //	m_sticWaitTime.ShowWindow(FALSE);
        m_cmbWaitTime.ShowWindow(FALSE);
        m_cmbRingVolume.ShowWindow(FALSE);
        m_cmbSystemVolume.ShowWindow(FALSE);
        m_chbEnablePassword.ShowWindow(FALSE);
        //	m_btnSetPassword.ShowWindow(FALSE);
        //	m_sticSoundSavePath.ShowWindow(FALSE);
        m_cmbSoundSavePath.ShowWindow(FALSE);
        //	m_sticTouchAdjust.ShowWindow(FALSE);
        //	m_btnTouchAdjust.ShowWindow(FALSE);
        //	m_stcTime.ShowWindow(FALSE);
        m_dtTime.ShowWindow(FALSE);
        m_dtDate.ShowWindow(FALSE);
        //information
        m_sticSystomTitle.ShowWindow(FALSE);
        // 	m_sticSoftwareVersion1.ShowWindow(FALSE);
        // 	m_sticHardwareVersion1.ShowWindow(FALSE);
        // 	m_sticMemorySize1.ShowWindow(FALSE);
        // 	m_sticStorageSize1.ShowWindow(FALSE);
        // 	m_sticContactSize1.ShowWindow(FALSE);
        // 	m_sticContactInfoSize1.ShowWindow(FALSE);
        // 	m_sticSoundSize1.ShowWindow(FALSE);
        // 	m_sticInformationSize1.ShowWindow(FALSE);
        // 	m_sticVersionRight1.ShowWindow(FALSE);
        m_sticSoftwareVersion.ShowWindow(FALSE);
        m_editVersion.ShowWindow(FALSE);
        //	m_sticHardwareVersion.ShowWindow(FALSE);
        m_sticMemorySize.ShowWindow(FALSE);
        m_sticStorageSize.ShowWindow(FALSE);
        m_sticContactSize.ShowWindow(FALSE);
        m_sticContactInfoSize.ShowWindow(FALSE);
        m_sticSoundSize.ShowWindow(FALSE);
        m_sticInformationSize.ShowWindow(FALSE);
        m_sticVersionRight.ShowWindow(FALSE);
        // 	m_btnClearContact.ShowWindow(FALSE);
        // 	m_btnClearContactInfo.ShowWindow(FALSE);
        // 	m_btnClearSound.ShowWindow(FALSE);
        // 	m_btnClearInformation.ShowWindow(FALSE);
        // 	m_stcBackUP.ShowWindow(FALSE);
        // //	m_stcReStore.ShowWindow(FALSE);
        // 	m_btnBackUP.ShowWindow(FALSE);
        // 	m_btnReStore.ShowWindow(FALSE);
        // 	m_stcTitle.ShowWindow(FALSE);
        // 	m_btnOut.ShowWindow(FALSE);
        /*
        m_cmbTitle.ShowWindow(FALSE);
        m_lstLocal.ShowWindow_(FALSE);
        m_lstUSB.ShowWindow_(FALSE);
        m_btnIn.ShowWindow(FALSE);
        m_btnOut.ShowWindow(FALSE);
        m_btnDelete.ShowWindow(FALSE);
        m_stcLocal.ShowWindow(FALSE);
        m_stcUSB.ShowWindow(FALSE);
        m_stcLocalTitle.ShowWindow(FALSE);
        m_stcUSBTitle.ShowWindow(FALSE);
        m_btnAllSelect.ShowWindow(FALSE);
        m_btnClearSelect.ShowWindow(FALSE);
        */
        //int type = m_lsType.GetNextSelectedItem(pos);

        //blacklight
        m_chbContrlBlackLight.ShowWindow(FALSE);
        m_cmbContrlBlackLightWaitTime.ShowWindow(FALSE);
        m_chbNightContrlBlackLight.ShowWindow(FALSE);
        m_dtNightContrlBlackLightStartTime.ShowWindow(FALSE);
        m_dtNightContrlBlackLightEndTime.ShowWindow(FALSE);

        switch (m_uiType)
        {
        case 0:
            //		m_sticLocalAreaNumber.ShowWindow(TRUE);
            m_edtLocalAreaNumber.ShowWindow(TRUE);
            m_chbEnableOutLine.ShowWindow(TRUE);
            m_edtOutLine.ShowWindow(TRUE);
            m_chbEnableIPDial.ShowWindow(TRUE);
            m_edtIPDial.ShowWindow(TRUE);
            m_chbEnableRing.ShowWindow(TRUE);
            //		m_sticDefaultRing.ShowWindow(TRUE);
            m_cmbRing.ShowWindow(TRUE);

            /*
            m_ringEdit.ShowWindow(TRUE);
            m_ringButton.ShowWindow(TRUE);
            */

            m_chbEnableAutoLeaveWord.ShowWindow(TRUE);
            //		m_sticRingTimes.ShowWindow(TRUE);
            m_cmbRingTimes.ShowWindow(TRUE);
            m_cmbAutoRecoedeTimes.ShowWindow(TRUE);
            //		m_sticAutoLeaveWord.ShowWindow(TRUE);
            //		m_cmbAutoLeaveWordTip.ShowWindow(TRUE);
            //		m_btnRecardLeaveWordTip.ShowWindow(TRUE);
            //		m_btnDefineSpeedButton.ShowWindow(TRUE);
            //		m_sticDefineSpeedButton.ShowWindow(TRUE);
            break;
        case 1:
            m_rdoConnectAuto.ShowWindow(TRUE);
            m_rdoConnectADSL.ShowWindow(TRUE);
            // 		m_sticADSLName.ShowWindow(TRUE);
            // 		m_sticADSLPassword.ShowWindow(TRUE);
            m_edtADSLName.ShowWindow(TRUE);
            m_edtADSLPassword.ShowWindow(TRUE);	
            m_rdoGetIPDHCP.ShowWindow(TRUE);
            m_rdoGetIPStatic.ShowWindow(TRUE);
            // 		m_sticIP.ShowWindow(TRUE);
            // 		m_sticMask.ShowWindow(TRUE);
            // 		m_sticGateway.ShowWindow(TRUE);
            // 		m_sticDNS.ShowWindow(TRUE);
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
            //		m_sticScreenProtctType.ShowWindow(TRUE);
            m_rdoTimeScreen.ShowWindow(TRUE);
            m_rdoImageScreen.ShowWindow(TRUE);
            //		m_sticWaitTime.ShowWindow(TRUE);
            m_cmbWaitTime.ShowWindow(TRUE);
            m_cmbRingVolume.ShowWindow(TRUE);
            m_cmbSystemVolume.ShowWindow(TRUE);
            //		m_chbEnablePassword.ShowWindow(TRUE);
            //		m_btnSetPassword.ShowWindow(TRUE);
            //		m_sticSoundSavePath.ShowWindow(TRUE);
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

            //		m_sticTouchAdjust.ShowWindow(TRUE);
            //		m_btnTouchAdjust.ShowWindow(TRUE);
            //		m_stcTime.ShowWindow(TRUE);
            m_dtTime.ShowWindow(TRUE);
            m_dtDate.ShowWindow(TRUE);
            break;
        case 3:
            // 		m_stcBackUP.ShowWindow(TRUE);
            // 	//	m_stcReStore.ShowWindow(TRUE);
            // 		m_btnBackUP.ShowWindow(TRUE);
            // 		m_btnReStore.ShowWindow(TRUE);
            // 		m_stcTitle.ShowWindow(TRUE);
            // 		m_btnOut.ShowWindow(TRUE);
            /*
            m_cmbTitle.ShowWindow(TRUE);
            m_lstLocal.ShowWindow_(TRUE);
            m_lstUSB.ShowWindow_(TRUE);
            m_btnIn.ShowWindow(TRUE);
            m_btnOut.ShowWindow(TRUE);
            m_btnDelete.ShowWindow(TRUE);
            m_stcLocal.ShowWindow(TRUE);
            m_stcUSB.ShowWindow(TRUE);
            m_stcLocalTitle.ShowWindow(TRUE);
            m_stcUSBTitle.ShowWindow(TRUE);
            m_btnAllSelect.ShowWindow(TRUE);
            m_btnClearSelect.ShowWindow(TRUE);
            */
            break;
        case 4:
            m_sticSystomTitle.ShowWindow(TRUE);
            // 		m_sticSoftwareVersion1.ShowWindow(TRUE);
            // 		m_sticHardwareVersion1.ShowWindow(TRUE);
            // 		m_sticMemorySize1.ShowWindow(TRUE);
            // 		m_sticStorageSize1.ShowWindow(TRUE);
            // 		m_sticContactSize1.ShowWindow(TRUE);
            // 		m_sticContactInfoSize1.ShowWindow(TRUE);
            // 		m_sticSoundSize1.ShowWindow(TRUE);
            // //		m_sticInformationSize1.ShowWindow(TRUE);
            // 		m_sticVersionRight1.ShowWindow(TRUE);	
            m_sticSoftwareVersion.ShowWindow(TRUE);
            //		m_sticHardwareVersion.ShowWindow(TRUE);
            {
                /*
                std::string temp = Data::LanguageResource::Get(Data::RI_SETTING_AVAILABLESIZE);
                CString as = Util::StringOp::ToCString(temp);
                CString text;
                ULARGE_INTEGER freeBytes;
                ULARGE_INTEGER totalBytes;
                ULARGE_INTEGER totalFreeBytes;
                GetDiskFreeSpaceEx(_T("\\Flashdrv"), &freeBytes, &totalBytes, &totalFreeBytes);
                int m = totalBytes.QuadPart / (1024 * 1024);
                int f = totalFreeBytes.QuadPart / (1024 * 1024);
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
                */
                m_sticMemorySize.ShowWindow(TRUE);		
                m_sticStorageSize.ShowWindow(TRUE);
            }

            m_sticContactSize.ShowWindow(TRUE);
            m_sticContactInfoSize.ShowWindow(TRUE);
            m_sticSoundSize.ShowWindow(TRUE);
            SetDiskInFo();
            //		m_sticInformationSize.ShowWindow(TRUE);
            {
                /*
                int count = Data::Contact::GetDataCount("");
                CString text;
                text.Format(_T("%d"), count);
                m_sticContactSize.SetWindowText(text);

                count = Data::ContactInfo::GetDataCount("");
                text.Format(_T("%d"), count);
                m_sticContactInfoSize.SetWindowText(text);

                count = Data::SoundSegment::GetDataCount("");
                text.Format(_T("%d"), count);
                m_sticSoundSize.SetWindowText(text);
                */
            }
            m_sticVersionRight.ShowWindow(TRUE);
            // 		m_btnClearContact.ShowWindow(TRUE);
            // 		m_btnClearContactInfo.ShowWindow(TRUE);
            // 		m_btnClearSound.ShowWindow(TRUE);
            // //		m_btnClearInformation.ShowWindow(TRUE);
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


    //���ݿⱸ��
    void CSettingDlg::OnBackup()
    {
        m_copyfileDlg->SetType(backup_file);
        m_copyfileDlg->ShowWindow(SW_SHOW);
    }

    //���ݿ�ָ�
    void CSettingDlg::OnRestore()
    {
        m_copyfileDlg->SetType(restore_file);
        m_copyfileDlg->ShowWindow(SW_SHOW);
    }

    //¼���ļ�����
    void CSettingDlg::OnFileOut()
    {
        m_copyfileDlg->SetType(copy_file);
        m_copyfileDlg->ShowWindow(SW_SHOW);
    }

    void CSettingDlg::OnRingLst(NMHDR* pNMHDR, LRESULT* pResult)
    {
        /*
        POSITION pos = m_ringList.GetFirstSelectedItemPosition();
        if (pos != NULL)
        {
        int index = m_ringList.GetNextSelectedItem (pos);
        CString s;
        CString s1 = Data::LanguageResource::Get(Data::RI_COMN_TOBOTTOM).c_str();
        s = m_ringList.GetItemText(index, 0);
        if(s.Compare(_T("storagecard")) == 0)
        {
        SetRingLst(_T("/storagecard/"));
        }
        //��һ��
        else if(s.Compare(s1) == 0)
        {
        //	m_chDir
        CString sDir = m_ringDir;
        int n = sDir.Find(_T("/"));
        UINT8 old[16];
        int i = 0;
        while(n != -1)
        {
        old[i++] = n;
        n += 1;
        n = sDir.Find(_T("/"), n);
        }
        sDir = sDir.Mid(0, old[i-2]+1);
        if(sDir == _T("/"))
        SetRingLst(L"");
        else
        SetRingLst(sDir.GetBuffer(128));
        }

        else
        {
        int n = s.Find(_T(".mp3"));
        int n1 = s.Find(_T(".MP3"));
        int n2 = s.Find(_T(".wav"));
        int n3 = s.Find(_T(".WAV"));
        if((n1 == -1) && (n == -1) && (n2 == -1) && (n3 == -1))
        {
        CString sDir = m_ringDir;
        sDir += s; 
        sDir += "/";
        SetRingLst(sDir.GetBuffer(128));
        }
        else
        {
        m_ringEdit.SetWindowText(s);
        m_ringList.ShowWindow_(SW_HIDE);
        m_ringStatic.ShowWindow(SW_HIDE);
        }
        }
        }
        */
        *pResult = 0;
    }

    void CSettingDlg::OnRingSelect()
    {
        /*
        if(m_ringStatic.IsWindowVisible())
        {
        m_ringList.ShowWindow_(SW_HIDE);
        m_ringStatic.ShowWindow(SW_HIDE);
        }
        else
        {
        SetRingLst(L"");
        m_ringList.ShowWindow_(SW_SHOW);
        m_ringStatic.ShowWindow(SW_SHOW);
        }
        */
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
        //wcscpy(m_chDir, _T("D:/flashdrv/my_ring"));
        //wcscpy(m_chDir, cdir);
        CString dir = cdir;
        ProcessFiles(_T("*.mp3"), FindRing(dir, m_cmbRing));
        ProcessFiles(_T("*.MP3"), FindRing(dir, m_cmbRing));
        ProcessFiles(_T("*.wav"), FindRing(dir, m_cmbRing));
        ProcessFiles(_T("*.WAV"), FindRing(dir, m_cmbRing));
        //m_cmbRing.SetScrollRagle(TRUE);
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
        /*
        if(w == IDC_SETTING_VERSION)
        {
        if(m_editVersion.IsWindowVisible())
        m_editVersion.ShowWindow(SW_HIDE);
        else
        m_editVersion.ShowWindow(SW_SHOW);
        }
        */

        if(m_pTestDlg0 == NULL)
        {
            m_pTestDlg0 = new CTestDlg(this);
            m_pTestDlg0->Create(CTestDlg::IDD);
        }
        View::_test_call = TRUE;
        m_pTestDlg0->ShowWindow(SW_SHOW);
        return result;
    }

    /*
    //ɾ���ļ�
    void CSettingDlg::OnDeleteFile()
    {

    }

    //����ļ�ȫ��ѡ��
    void CSettingDlg::OnAllSelect()
    {
    if(m_nListSelect == 0)
    {
    int ncount = m_lstLocal.GetItemCount();

    LVITEM lvitem;
    lvitem.mask=LVIF_TEXT   |   LVIF_IMAGE;   

    for(int i = 0; i < ncount; i++)
    {
    lvitem.iItem=i;   
    lvitem.iSubItem=0;   
    lvitem.pszText = m_lstLocal.GetItemText(i, 0).GetBuffer(128);  
    CString s = lvitem.pszText;
    lvitem.lParam=i;   
    lvitem.iImage=1;
    m_lstLocal.SetItem(&lvitem);
    m_lstLocal.SetItemData(i, 1);
    }
    }
    else if(m_nListSelect == 1)
    {

    }
    }

    //����ļ�ѡ��
    void CSettingDlg::OnClearDelect()
    {
    if(m_nListSelect == 0)
    {
    int ncount = m_lstLocal.GetItemCount();

    LVITEM lvitem;
    lvitem.mask=LVIF_TEXT   |   LVIF_IMAGE;   

    for(int i = 0; i < ncount; i++)
    {
    lvitem.iItem=i;   
    lvitem.iSubItem=0;   
    lvitem.pszText = m_lstLocal.GetItemText(i, 0).GetBuffer(128);  
    CString s = lvitem.pszText;
    lvitem.lParam=i;   
    lvitem.iImage=0;
    m_lstLocal.SetItem(&lvitem);
    m_lstLocal.SetItemData(i, 0);
    }
    }
    else if(m_nListSelect == 1)
    {

    }
    }
    //ѡ�е��ļ�����
    void CSettingDlg::OnFileIn()
    {

    }

    void CSettingDlg::SetLocalLst(TCHAR *dir)
    {
    memset(m_localDir, 0, 128*2);
    memcpy(m_localDir, dir, wcslen(dir)*2);
    m_lstLocal.DeleteAllItems();
    m_lstLocal.SetScrollPos(0, TRUE);
    int ncount = 0;

    CString sDir = dir;
    WIN32_FIND_DATA FindFileData;			//�����ļ�ʱҪʹ�õ����ݽṹ
    HANDLE hFind = INVALID_HANDLE_VALUE;	//������Ҿ��

    sDir += "*.*";
    hFind = FindFirstFile(sDir, &FindFileData);//ʹ��FindFirstFile��������ʼ�ļ�����

    if (hFind == INVALID_HANDLE_VALUE) 
    {
    return;
    } 
    else 
    {
    char filename[128];
    //		printf ("First file name is %s\n", FindFileData.cFileName);
    int i = wcstombs( filename, FindFileData.cFileName, 128);
    filename[i] = '\0';
    if(!(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
    {
    m_lstLocal.InsertItem(ncount++, FindFileData.cFileName, 0);
    }

    //������ѭ��ʹ��FindNextFile�����������ļ�
    while (FindNextFile(hFind, &FindFileData) != 0) 
    {
    if(!(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
    {
    m_lstLocal.InsertItem(ncount++, FindFileData.cFileName, 0);
    }
    }

    DWORD dwError = GetLastError();
    if (dwError == ERROR_NO_MORE_FILES) 
    {
    FindClose(hFind);//�رղ��Ҿ��
    } 
    else 
    {
    //			printf ("FindNextFile error. Error is %u\n", dwError);
    return;
    }
    }
    m_lstLocal.SetScrollRagle(FALSE);
    CRect rt;
    m_lstLocal.GetWindowRect(&rt);
    m_lstLocal.InvalidateRect(&rt);
    return;	
    }

    void CSettingDlg::SetLocalLst()
    {

    }

    //ѡ���ļ���������
    void CSettingDlg::OnFileSelect(WPARAM w, LPARAM l)
    {
    std::string strTemp = Data::LanguageResource::Get(Data::RI_SETTING_RING);
    CString s1 = strTemp.c_str();
    strTemp = Data::LanguageResource::Get(Data::RI_SETTING_PHOTO);
    CString s2 = strTemp.c_str();
    strTemp = Data::LanguageResource::Get(Data::RI_SETTING_MP3);
    CString s3 = strTemp.c_str();
    strTemp = Data::LanguageResource::Get(Data::RI_SETTING_VIDEO);
    CString s4 = strTemp.c_str();
    strTemp = Data::LanguageResource::Get(Data::RI_SETTING_RING);
    CString s5 = strTemp.c_str();

    CString s;
    m_cmbTitle.GetWindowText(s);

    //
    if(s == s1)
    {
    m_nFileOperaterType = RING_TYPE;
    SetLocalLst(_T("/flashdrv/my_ring/"));
    }
    else if(s == s2)
    {
    m_nFileOperaterType = PHOTO_TYPE;
    SetLocalLst(_T("/flashdrv/my_photo/"));
    }
    else if(s == s3)
    {
    m_nFileOperaterType = MP3_TYPE;
    SetLocalLst(_T("/flashdrv/my_music/"));
    //ѡ�е��ļ�����
    }
    else if(s == s4)
    {
    m_nFileOperaterType = VIDEO_TYPE;
    SetLocalLst(_T("/flashdrv/my_video/"));
    }
    else if(s == s5)
    {
    m_nFileOperaterType = RECORDE_TYPE;
    SetLocalLst(_T("/flashdrv/my_recorde/"));
    }
    }
    //����ļ��б�
    void CSettingDlg::OnLocalLst(NMHDR* pNMHDR, LRESULT* pResult)
    {
    m_nListSelect = 0;
    }
    //����ļ��б�
    void CSettingDlg::OnUSBLst(NMHDR* pNMHDR, LRESULT* pResult)
    {
    m_nListSelect = 1;
    }
    */

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
        /*
        m_cmbtip.ResetContent();
        if (Data::SoundSegment::GetDataCount("type = 2 AND isTip = 1") > 0)
        {
        std::vector<Util::shared_ptr<Data::SoundSegment> > sounds = Data::SoundSegment::GetFromDatabase("type = 2 AND isTip = 1");
        for (int i = 0; i < sounds.size(); ++i)
        {
        int pos = sounds[i]->filename().rfind('\\');
        m_cmbtip.AddString(Util::StringOp::ToCString(sounds[i]->filename().substr(pos + 1)));
        }

        int pos = m_pSetting->defaultTipFilename().rfind('\\');
        CString s = Util::StringOp::ToCString(m_pSetting->defaultTipFilename().substr(pos + 1));

        int index = m_cmbtip.SelectString(0, s);
        if (index == CB_ERR)
        {
        m_cmbtip.SetCurSel(-1);
        // 			m_pSetting->defaultTipFilename("");
        // 			m_pSetting->Update();
        }
        }
        */
    }

    void CSettingDlg::OnPaint()     
    {   
        //   TODO:   Add   your   message   handler   code   here   
        CPaintDC   dc(this); 
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
