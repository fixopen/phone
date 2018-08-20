// copyfileDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CopyfileDlg.h"
#include "../MultimediaPhone.h"
#include "../MultimediaPhoneDlg.h"
#include "../Pblmember.h"
#include "../Data/SkinStyle.h"
#include "../Data/LanguageResource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace View {
    const TCHAR *ptchExtDb[] = {_T("/UsbDisk/mediaphone/mmp.db"), _T("/StorageCard/mediaphone/mmp.db")};
    const TCHAR *ptchExtMp3[] = {_T("/UsbDisk/mediaphone/my_music"), _T("/StorageCard/mediaphone/my_music")};
    const TCHAR *ptchExtPhoto[] = {_T("/UsbDisk/mediaphone/my_photo"), _T("/StorageCard/mediaphone/my_photo")};
    const TCHAR *ptchExtBell[] = {_T("/UsbDisk/mediaphone/my_ring"), _T("/StorageCard/mediaphone/my_ring")};
    const TCHAR *ptchExtShow[] = {_T("/UsbDisk/mediaphone/my_show"), _T("/StorageCard/mediaphone/my_show")};

    static enum DeviceStatus {
        usb_device,
        sdcard_device,
        none_device
    } m_devStatus;
    static TCHAR m_chExtPath[64];

    /////////////////////////////////////////////////////////////////////////////
    // copyfileDlg dialog

    copyfileDlg::copyfileDlg(CWnd* pParent /*=NULL*/)
        : CCEDialog(copyfileDlg::IDD, pParent)
    {
        //{{AFX_DATA_INIT(copyfileDlg)
        // NOTE: the ClassWizard will add member initialization here
        //}}AFX_DATA_INIT
        m_nPos = 0;
        m_devStatus = none_device;
        memset(m_chExtPath, 0, sizeof(m_chExtPath));
    }


    void copyfileDlg::DoDataExchange(CDataExchange* pDX)
    {
        CDialog::DoDataExchange(pDX);
        //{{AFX_DATA_MAP(copyfileDlg)
        // NOTE: the ClassWizard will add DDX and DDV calls here
        //}}AFX_DATA_MAP
    }


    BEGIN_MESSAGE_MAP(copyfileDlg, CCEDialog)
        //{{AFX_MSG_MAP(copyfileDlg)
        // NOTE: the ClassWizard will add message map macros here
        ON_BN_CLICKED(IDC_BUTTON_FIREWALL_CANCEL, OnButtonCancel)
        ON_WM_TIMER()
        //}}AFX_MSG_MAP
        ON_MESSAGE(WM_CLICKMJPG_TOAPP, OnClickMJPG)
    END_MESSAGE_MAP()

    LRESULT copyfileDlg::OnClickMJPG(WPARAM w, LPARAM l)
    {
        LRESULT result = S_OK;
        // 	switch(w)
        // 	{
        // 	case 1:
        // 		OnButtonOk();
        // 		break;
        // 	case 2:
        // 		OnButtonCancel();
        // 		break;
        // 	}

        switch(w)
        {
        case 1:
            //	OnButtonOk();
            Import(TRUE);
            break;
        case 2:
        case 5:
            OnButtonCancel();	 // no 4
            break;
        case 3:			//替换     no 5
            Import(TRUE);
            break;
        case 4:			//添加     no  6
            Import(FALSE);
            break;		
        }
        return result;

    }
    BOOL copyfileDlg::OnInitDialog()
    {
        CDialog::OnInitDialog();

        std::string strTemp;
        CString str;

		m_MJPGList.Create(L"", WS_VISIBLE|WS_CHILD, CRect(54*X_XISHU, 62*Y_XISHU, 746*X_XISHU, 358*Y_XISHU), this);
        m_MJPGList.SetCurrentLinkFile(_T(".\\adv\\mjpg\\k1\\中文\\文件备份.xml"));
        m_MJPGList.SetMJPGRect(CRect(54*X_XISHU, 62*Y_XISHU, 746*X_XISHU, 358*Y_XISHU));

        m_sticTitle.Create(L"", WS_CHILD|WS_VISIBLE, CRect((42 )*X_XISHU, (70 )*Y_XISHU, (410 )*X_XISHU, (102 )*Y_XISHU), &m_MJPGList);
        m_sticTitle.SetColor(RGB(0, 0, 0), RGB(203, 203, 203));//Data::g_partFrameInRectBackRGB[Data::g_skinstyle]);

        m_procbarSound.Create(WS_CHILD, CRect((57 )*X_XISHU, (190 )*Y_XISHU, (522 )*X_XISHU, (217 )*Y_XISHU), &m_MJPGList, 0xFF10);
        m_procbarSound.SetParam(0, 0, 4, 1);
        m_procbarSound.SetPos(0);

        /*
        strTemp = Data::LanguageResource::Get(Data::RI_COMN_OKBTN);
        str = strTemp.c_str();
        m_btnOk.Create(str, Data::g_buttonArcBMPPARTDILOAGID[0][Data::g_skinstyle], Data::g_buttonArcBMPPARTDILOAGID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(378, 78, 434, 98), this, IDC_BUTTON_FIREWALL_OK);
        m_btnOk.SetBackRGB(Data::g_partFrameMainBackRGB[Data::g_skinstyle]);

        strTemp = Data::LanguageResource::Get(Data::RI_COMN_CANCELBTN);
        str = strTemp.c_str();
        m_btnCancel.Create(str, Data::g_buttonArcBMPPARTDILOAGID[0][Data::g_skinstyle], Data::g_buttonArcBMPPARTDILOAGID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(378, 103, 434, 123), this, IDC_BUTTON_FIREWALL_CANCEL);
        m_btnCancel.SetBackRGB(Data::g_partFrameMainBackRGB[Data::g_skinstyle]);

        m_sticBackground.Create(CRect(34, 40, 448, 184), this, 1);

        TextStruct ts[1];
        memset(ts, 0, sizeof(TextStruct) * 1);

        ts[0].txtRect = CRect(8, 0, 160, 20);
        ts[0].txtFontSize = 16;
        ts[0].sAlign = DT_LEFT | DT_BOTTOM;
        memcpy(ts[0].sTxt, Data::LanguageResource::Get(Data::RI_SETTING_COPYFILE).c_str(), Data::LanguageResource::Get(Data::RI_SOUND_PLAY_TITLE).length());

        m_sticBackground.SetTextStruct(ts, 1);
        */

        m_MJPGList.SetUnitIsShow(0, FALSE);
        m_MJPGList.SetUnitIsShow(1, FALSE);
        m_MJPGList.SetUnitIsShow(2, FALSE);
        m_MJPGList.SetUnitIsShow(3, FALSE);
        m_MJPGList.SetUnitIsShow(4, FALSE);
        m_MJPGList.SetUnitIsShow(5, FALSE);
        m_MJPGList.SetUnitIsShow(6, FALSE);
        m_MJPGList.SetUnitIsShow(7, FALSE);
        return TRUE;
    }

    void copyfileDlg::SetType(operater_type type)
    {
        type_ = type;
        m_procbarSound.ShowWindow(SW_HIDE);
        //	m_MJPGList.SetUnitIsShow(2, FALSE);

        BOOL bUsb = DetectDIR(_T("/UsbDisk"));
        if( bUsb || DetectDIR(L"/StorageCard"))
        {
            if (bUsb)
                m_devStatus = usb_device;
            else
                m_devStatus = sdcard_device;

            progress_status_ = isexitfile_status;
            std::string strTemp;
            CString str;

            strTemp = Data::LanguageResource::Get(Data::RI_SETTING_COPYFILE_ERROR2);
            str = strTemp.c_str();
            m_sticTitle.SetWindowText(str);

            switch(type_)
            {
            case backup_file:
            case restore_file:
            case copy_file:
            case mp3_out:
            case photo_out:
            case bell_out:
            case show_out:
                m_MJPGList.SetUnitIsShow(3, TRUE);
                m_MJPGList.SetUnitIsShow(4, TRUE);
                m_MJPGList.SetUnitIsShow(5, FALSE);
                m_MJPGList.SetUnitIsShow(6, FALSE);
                m_MJPGList.SetUnitIsShow(7, FALSE);
                break;
            case mp3_in:
            case photo_in:
            case bell_in:
            case show_in:
                m_MJPGList.SetUnitIsShow(3, FALSE);
                m_MJPGList.SetUnitIsShow(4, FALSE);
                m_MJPGList.SetUnitIsShow(5, TRUE);
                m_MJPGList.SetUnitIsShow(6, TRUE);
                m_MJPGList.SetUnitIsShow(7, TRUE);
            default:
                break;
            }

        }
        else 
        {
            m_devStatus = none_device;
            memset(m_chExtPath, 0, sizeof(m_chExtPath));

            progress_status_ = nousb_status;
            std::string strTemp;
            CString str;

            strTemp = Data::LanguageResource::Get(Data::RI_SETTING_COPYFILE_ERROR1);
            str = Util::StringOp::ToCString(strTemp);
            m_sticTitle.SetWindowText(str);

            m_MJPGList.SetUnitIsShow(3, TRUE);
            m_MJPGList.SetUnitIsShow(4, TRUE);
            m_MJPGList.SetUnitIsShow(5, FALSE);
            m_MJPGList.SetUnitIsShow(6, FALSE);
            m_MJPGList.SetUnitIsShow(7, FALSE);
        }
    }

    void copyfileDlg::Import(BOOL bOverflow)
    {
        std::string strTemp;
        CString str;

        if(progress_status_ == nousb_status || progress_status_ == finish_status)
        {
            KillTimer(IDT_COPYFILE_TIMER);
            progress_status_ = start_status;
            ShowWindow(SW_HIDE);
        }
        else if(progress_status_ == isexitfile_status)
        {

            TCHAR tchFile[MAX_PATH] = {0};
            TCHAR tchLocalFile[MAX_PATH] = {0};
            int nFlags = 0;

            switch(type_ )
            {
            case backup_file:
            case copy_file:
            case mp3_out:
            case photo_out:
            case bell_out:
            case show_out:
                //m_MJPGList.SetUnitIsShow(2, TRUE);
                //StartCopyFile();
                break;
            case restore_file:
                {		
     				//if(!DetectFile(L"/UsbDisk/mediaphone/backup/mmp.db"))
     				//{
     				//	progress_status_ = finish_status;
     				//	
     				//	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_COPYFILE_ERROR3);
     				//	str = strTemp.c_str();
     				//	m_sticTitle.SetWindowText(str);
     				//}
     				//else			//copy
     				//{
     				//	m_MJPGList.SetUnitIsShow(2, TRUE);
     				//	StartCopyFile();
     				//	
     				//}
                    lstrcpy(m_chExtPath, ptchExtDb[m_devStatus]);
                    //lstrcpy(tchFile, ptchExtDb);
                    nFlags = 1;
                }
                break;
            case mp3_in:
                lstrcpy(m_chExtPath, ptchExtMp3[m_devStatus]);
                //lstrcpy(tchFile, ptchExtMp3);
                lstrcpy(tchLocalFile, ::musicPath);
                nFlags = 2;
                break;
            case photo_in:
                lstrcpy(m_chExtPath, ptchExtPhoto[m_devStatus]);
                //lstrcpy(tchFile, ptchExtPhoto);
                lstrcpy(tchLocalFile, ::photoPath);
                nFlags = 2;
                break;
            case bell_in:
                lstrcpy(m_chExtPath, ptchExtBell[m_devStatus]);
                //lstrcpy(tchFile, ptchExtBell);
                lstrcpy(tchLocalFile, ::ringPath);
                nFlags = 2;
                break;
            case show_in:
                lstrcpy(m_chExtPath, ptchExtShow[m_devStatus]);
                //lstrcpy(tchFile, ptchExtShow);
                lstrcpy(tchLocalFile, ::adPath);
                nFlags = 2;
                break;
            default:
                return;
            }

            if (nFlags == 1)
            {
                if (!DetectFile(m_chExtPath/*tchFile*/))
                {
                    progress_status_ = finish_status;

                    strTemp = Data::LanguageResource::Get(Data::RI_SETTING_COPYFILE_ERROR3);
                    str = Util::StringOp::ToCString(strTemp);
                    m_sticTitle.SetWindowText(str);
                    return;
                }
            }
            else if (nFlags ==2)
            {
                if (!DetectDIR(m_chExtPath/*tchFile*/))
                {
                    progress_status_ = finish_status;

                    strTemp = Data::LanguageResource::Get(Data::RI_SETTING_COPYFILE_ERROR3);
                    str = Util::StringOp::ToCString(strTemp);
                    m_sticTitle.SetWindowText(str);
                    return;
                }
            }

            if (bOverflow)
            {
                switch(type_)
                {
                case mp3_in:
                case photo_in:
                case bell_in:
                case show_in:
                    {
                        lstrcat(tchLocalFile, _T("\\*"));
                        DeleteFiles(tchLocalFile);
                    }
                    break;
                }
            }

            //		m_MJPGList.SetUnitIsShow(2, TRUE);
            StartCopyFile();

        }
    }

    void copyfileDlg::OnButtonCancel()
    {
        KillTimer(IDT_COPYFILE_TIMER);
        progress_status_ = start_status;
        ShowWindow(SW_HIDE);
    }

    void copyfileDlg::OnTimer(UINT nIDEvent) 
    {
        if(nIDEvent == IDT_COPYFILE_TIMER)
        {
            m_procbarSound.SetPos(m_nPos);
        }
        CDialog::OnTimer(nIDEvent);
    }

    void copyfileDlg::CopyFileProc(void)
    {
        CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
        copyfileDlg *pDlg = main->m_pSettingDlg->m_copyfileDlg;
        std::string strTemp;
        CString str;

        switch(pDlg->type_)
        {
        case backup_file:
            {
                pDlg->progress_status_ = copying_status;			
                strTemp = Data::LanguageResource::Get(Data::RI_SETTING_COPYFILE_DOWITH);
                str = Util::StringOp::ToCString(strTemp);
                pDlg->m_sticTitle.SetWindowText(str);
                pDlg->m_procbarSound.ShowWindow(SW_SHOW);
                pDlg->m_procbarSound.SetPos(1);
                pDlg->m_nPos = 1;
                pDlg->SetTimer(IDT_COPYFILE_TIMER, 300, 0);

                if (m_devStatus == usb_device)
                {
                    CreateDirectoryW(L"/UsbDisk/mediaphone", 0);
                    CopyFile(L"C:/flashdrv/res_dat/mmp.db", L"/UsbDisk/mediaphone/mmp.db",  FALSE);
                }
                else if (m_devStatus == sdcard_device)
                {
                    CreateDirectoryW(L"/StorageCard/mediaphone", 0);
                    CopyFile(L"C:/flashdrv/res_dat/mmp.db", L"/StorageCard/mediaphone/mmp.db",  FALSE);
                }

                pDlg->KillTimer(IDT_COPYFILE_TIMER);
                pDlg->m_procbarSound.SetPos(4);

                pDlg->progress_status_ = finish_status;			
                strTemp = Data::LanguageResource::Get(Data::RI_SETTING_COPYFILE_FINISH);
                str = Util::StringOp::ToCString(strTemp);
                pDlg->m_sticTitle.SetWindowText(str);
            }
            break;
        case restore_file:
            {
                BOOL DetectFile(TCHAR *sDir);
                if(!DetectFile(/*L"/UsbDisk/mediaphone/mmp.db"*/m_chExtPath))
                {
                    pDlg->progress_status_ = finish_status;

                    strTemp = Data::LanguageResource::Get(Data::RI_SETTING_COPYFILE_ERROR3);
                    str = Util::StringOp::ToCString(strTemp);
                    pDlg->m_sticTitle.SetWindowText(str);
                }
                else			//copy
                {
                    pDlg->progress_status_ = copying_status;			
                    strTemp = Data::LanguageResource::Get(Data::RI_SETTING_COPYFILE_DOWITH);
                    str = Util::StringOp::ToCString(strTemp);
                    pDlg->m_sticTitle.SetWindowText(str);

                    pDlg->m_procbarSound.ShowWindow(SW_SHOW);
                    pDlg->m_procbarSound.SetPos(1);
                    pDlg->m_nPos = 1;
                    main->m_pSettingDlg->m_copyfileDlg->SetTimer(IDT_COPYFILE_TIMER, 300, 0);

                    //CopyFile(L"/UsbDisk/mediaphone/mmp.db", L"/flashdrv/res_dat/mmp.db", FALSE);
                    CopyFile(m_chExtPath, L"C:/flashdrv/res_dat/mmp.db", FALSE);

                    pDlg->KillTimer(IDT_COPYFILE_TIMER);
                    pDlg->m_procbarSound.SetPos(4);

                    pDlg->progress_status_ = finish_status;			
                    strTemp = Data::LanguageResource::Get(Data::RI_SETTING_COPYFILE_FINISH);
                    str = Util::StringOp::ToCString(strTemp);
                    pDlg->m_sticTitle.SetWindowText(str);
                }
            }
            break;
        case copy_file: 
            {
                //录音备份
                pDlg->progress_status_ = copying_status;			
                strTemp = Data::LanguageResource::Get(Data::RI_SETTING_COPYFILE_DOWITH);
                str = Util::StringOp::ToCString(strTemp);
                pDlg->m_sticTitle.SetWindowText(str);
                pDlg->m_procbarSound.ShowWindow(SW_SHOW);
                pDlg->m_procbarSound.SetPos(1);
                pDlg->m_nPos = 1;
                pDlg->SetTimer(IDT_COPYFILE_TIMER, 300, 0);

                //在USB上建立目录
                if (m_devStatus == usb_device)
                {
                    CreateDirectoryW(L"/UsbDisk/mediaphone", 0);
                    CreateDirectoryW(L"/UsbDisk/mediaphone/backup", 0);
                    CreateDirectoryW(L"/UsbDisk/mediaphone/backup/flashdrv", 0);
                    CreateDirectoryW(L"/UsbDisk/mediaphone/backup/SDcard", 0);
                    CreateDirectoryW(L"/UsbDisk/mediaphone/backup/flashdrv/my_record", 0);
                    CreateDirectoryW(L"/UsbDisk/mediaphone/backup/SDcard/my_record", 0);

                    CopyDirFiles((TCHAR*)(LPCTSTR)::recordPath, L"/UsbDisk/mediaphone/backup/flashdrv/my_record");
                    //CopyDirFiles(L"/storagecard/my_record", L"/UsbDisk/mediaphone/backup/SDcard/my_record");
                }
                else if (m_devStatus = sdcard_device)
                {
                    CreateDirectoryW(L"/StorageCard/mediaphone", 0);
                    CreateDirectoryW(L"/StorageCard/mediaphone/backup", 0);
                    CreateDirectoryW(L"/StorageCard/mediaphone/backup/flashdrv", 0);
                    CreateDirectoryW(L"/StorageCard/mediaphone/backup/SDcard", 0);
                    CreateDirectoryW(L"/StorageCard/mediaphone/backup/flashdrv/my_record", 0);
                    CreateDirectoryW(L"/StorageCard/mediaphone/backup/SDcard/my_record", 0);

                    CopyDirFiles((TCHAR*)(LPCTSTR)::recordPath, L"/StorageCard/mediaphone/backup/flashdrv/my_record");
                }

                pDlg->KillTimer(IDT_COPYFILE_TIMER);
                pDlg->m_procbarSound.SetPos(4);

                pDlg->progress_status_ = finish_status;			
                strTemp = Data::LanguageResource::Get(Data::RI_SETTING_COPYFILE_FINISH);
                str = Util::StringOp::ToCString(strTemp);
                pDlg->m_sticTitle.SetWindowText(str);
            }
            break;
        case mp3_in:
            {
                if(!DetectDIR(m_chExtPath/*L"/UsbDisk/mediaphone/my_music"*/))
                {
                    pDlg->progress_status_ = finish_status;

                    strTemp = Data::LanguageResource::Get(Data::RI_SETTING_COPYFILE_ERROR3);
                    str = Util::StringOp::ToCString(strTemp);
                    pDlg->m_sticTitle.SetWindowText(str);
                }
                else			//copy
                {
                    pDlg->progress_status_ = copying_status;			
                    strTemp = Data::LanguageResource::Get(Data::RI_SETTING_COPYFILE_DOWITH);
                    str = Util::StringOp::ToCString(strTemp);
                    pDlg->m_sticTitle.SetWindowText(str);

                    pDlg->m_procbarSound.ShowWindow(SW_SHOW);
                    pDlg->m_procbarSound.SetPos(1);
                    pDlg->m_nPos = 1;
                    main->m_pSettingDlg->m_copyfileDlg->SetTimer(IDT_COPYFILE_TIMER, 300, 0);

                    //CopyDirFiles(L"/UsbDisk/mediaphone/my_music", L"/flashdrv/my_music", FALSE);
                    CopyDirFiles(m_chExtPath, (TCHAR*)(LPCTSTR)::musicPath, FALSE);

                    pDlg->KillTimer(IDT_COPYFILE_TIMER);
                    pDlg->m_procbarSound.SetPos(4);

                    pDlg->progress_status_ = finish_status;			
                    strTemp = Data::LanguageResource::Get(Data::RI_SETTING_COPYFILE_FINISH);
                    str = Util::StringOp::ToCString(strTemp);
                    pDlg->m_sticTitle.SetWindowText(str);
                }
            }
            break;
        case mp3_out:
            {
                pDlg->progress_status_ = copying_status;			
                strTemp = Data::LanguageResource::Get(Data::RI_SETTING_COPYFILE_DOWITH);
                str = Util::StringOp::ToCString(strTemp);
                pDlg->m_sticTitle.SetWindowText(str);
                pDlg->m_procbarSound.ShowWindow(SW_SHOW);
                pDlg->m_procbarSound.SetPos(1);
                pDlg->m_nPos = 1;
                pDlg->SetTimer(IDT_COPYFILE_TIMER, 300, 0);

                //在USB上建立目录
                if (m_devStatus == usb_device)
                {
                    CreateDirectoryW(L"/UsbDisk/mediaphone", 0);
                    CreateDirectoryW(L"/UsbDisk/mediaphone/my_music", 0);

                    CopyDirFiles((TCHAR*)(LPCTSTR)::musicPath, L"/UsbDisk/mediaphone/my_music");

                    if (DetectFile(ptcPlayList[m_devStatus]))
                    {
                        CFile::Remove(ptcPlayList[m_devStatus]);
                    }

                }
                else if (m_devStatus == sdcard_device)
                {
                    CreateDirectoryW(L"/StorageCard/mediaphone", 0);
                    CreateDirectoryW(L"/StorageCard/mediaphone/my_music", 0);

                    CopyDirFiles((TCHAR*)(LPCTSTR)::musicPath, L"/StorageCard/mediaphone/my_music");

                    if (DetectFile(ptcPlayList[m_devStatus]))
                    {
                        CFile::Remove(ptcPlayList[m_devStatus]);
                    }
                }

                pDlg->KillTimer(IDT_COPYFILE_TIMER);
                pDlg->m_procbarSound.SetPos(4);

                pDlg->progress_status_ = finish_status;			
                strTemp = Data::LanguageResource::Get(Data::RI_SETTING_COPYFILE_FINISH);
                str = Util::StringOp::ToCString(strTemp);
                pDlg->m_sticTitle.SetWindowText(str);
            }
            break;
        case photo_in:
            {
                if(!DetectDIR(m_chExtPath/*L"/UsbDisk/mediaphone/my_photo"*/))
                {
                    pDlg->progress_status_ = finish_status;

                    strTemp = Data::LanguageResource::Get(Data::RI_SETTING_COPYFILE_ERROR3);
                    str = Util::StringOp::ToCString(strTemp);
                    pDlg->m_sticTitle.SetWindowText(str);
                }
                else			//copy
                {
                    pDlg->progress_status_ = copying_status;			
                    strTemp = Data::LanguageResource::Get(Data::RI_SETTING_COPYFILE_DOWITH);
                    str = Util::StringOp::ToCString(strTemp);
                    pDlg->m_sticTitle.SetWindowText(str);

                    pDlg->m_procbarSound.ShowWindow(SW_SHOW);
                    pDlg->m_procbarSound.SetPos(1);
                    pDlg->m_nPos = 1;
                    main->m_pSettingDlg->m_copyfileDlg->SetTimer(IDT_COPYFILE_TIMER, 300, 0);

                    //CopyDirFiles(L"/UsbDisk/mediaphone/my_photo", L"/flashdrv/my_photo", FALSE);
                    CopyDirFiles(m_chExtPath, (TCHAR*)(LPCTSTR)::photoPath, FALSE);

                    pDlg->KillTimer(IDT_COPYFILE_TIMER);
                    pDlg->m_procbarSound.SetPos(4);

                    pDlg->progress_status_ = finish_status;			
                    strTemp = Data::LanguageResource::Get(Data::RI_SETTING_COPYFILE_FINISH);
                    str = Util::StringOp::ToCString(strTemp);
                    pDlg->m_sticTitle.SetWindowText(str);
                }
            }
            break;
        case photo_out:
            {
                pDlg->progress_status_ = copying_status;			
                strTemp = Data::LanguageResource::Get(Data::RI_SETTING_COPYFILE_DOWITH);
                str = Util::StringOp::ToCString(strTemp);
                pDlg->m_sticTitle.SetWindowText(str);
                pDlg->m_procbarSound.ShowWindow(SW_SHOW);
                pDlg->m_procbarSound.SetPos(1);
                pDlg->m_nPos = 1;
                pDlg->SetTimer(IDT_COPYFILE_TIMER, 300, 0);

                //在USB上建立目录
                if (m_devStatus == usb_device)
                {
                    CreateDirectoryW(L"/UsbDisk/mediaphone", 0);
                    CreateDirectoryW(L"/UsbDisk/mediaphone/my_photo", 0);

                    CopyDirFiles((TCHAR*)(LPCTSTR)::photoPath, L"/UsbDisk/mediaphone/my_photo");
                }
                else if (m_devStatus == sdcard_device)
                {
                    CreateDirectoryW(L"/StorageCard/mediaphone", 0);
                    CreateDirectoryW(L"/StorageCard/mediaphone/my_photo", 0);

                    CopyDirFiles((TCHAR*)(LPCTSTR)::photoPath, L"/StorageCard/mediaphone/my_photo");

                }

                pDlg->KillTimer(IDT_COPYFILE_TIMER);
                pDlg->m_procbarSound.SetPos(4);

                pDlg->progress_status_ = finish_status;			
                strTemp = Data::LanguageResource::Get(Data::RI_SETTING_COPYFILE_FINISH);
                str = Util::StringOp::ToCString(strTemp);
                pDlg->m_sticTitle.SetWindowText(str);
            }
            break;
        case bell_in:
            {
                if(!DetectDIR(m_chExtPath/*L"/UsbDisk/mediaphone/my_ring"*/))
                {
                    pDlg->progress_status_ = finish_status;

                    strTemp = Data::LanguageResource::Get(Data::RI_SETTING_COPYFILE_ERROR3);
                    str = Util::StringOp::ToCString(strTemp);
                    pDlg->m_sticTitle.SetWindowText(str);
                }
                else			//copy
                {
                    pDlg->progress_status_ = copying_status;			
                    strTemp = Data::LanguageResource::Get(Data::RI_SETTING_COPYFILE_DOWITH);
                    str = Util::StringOp::ToCString(strTemp);
                    pDlg->m_sticTitle.SetWindowText(str);

                    pDlg->m_procbarSound.ShowWindow(SW_SHOW);
                    pDlg->m_procbarSound.SetPos(1);
                    pDlg->m_nPos = 1;
                    main->m_pSettingDlg->m_copyfileDlg->SetTimer(IDT_COPYFILE_TIMER, 300, 0);

                    //CopyDirFiles(L"/UsbDisk/mediaphone/my_ring", L"/flashdrv/my_ring", FALSE);
                    CopyDirFiles(m_chExtPath, (TCHAR*)(LPCTSTR)::ringPath, FALSE);

                    pDlg->KillTimer(IDT_COPYFILE_TIMER);
                    pDlg->m_procbarSound.SetPos(4);

                    pDlg->progress_status_ = finish_status;			
                    strTemp = Data::LanguageResource::Get(Data::RI_SETTING_COPYFILE_FINISH);
                    str = Util::StringOp::ToCString(strTemp);
                    pDlg->m_sticTitle.SetWindowText(str);
                }
            }
            break;
        case bell_out:
            {
                pDlg->progress_status_ = copying_status;			
                strTemp = Data::LanguageResource::Get(Data::RI_SETTING_COPYFILE_DOWITH);
                str = Util::StringOp::ToCString(strTemp);
                pDlg->m_sticTitle.SetWindowText(str);
                pDlg->m_procbarSound.ShowWindow(SW_SHOW);
                pDlg->m_procbarSound.SetPos(1);
                pDlg->m_nPos = 1;
                pDlg->SetTimer(IDT_COPYFILE_TIMER, 300, 0);

                //在USB上建立目录
                if (m_devStatus == usb_device)
                {
                    CreateDirectoryW(L"/UsbDisk/mediaphone", 0);
                    CreateDirectoryW(L"/UsbDisk/mediaphone/my_ring", 0);

                    CopyDirFiles((TCHAR*)(LPCTSTR)::ringPath, L"D:/UsbDisk/mediaphone/my_ring");

                }
                else if (m_devStatus == sdcard_device)
                {
                    CreateDirectoryW(L"/StorageCard/mediaphone", 0);
                    CreateDirectoryW(L"/StorageCard/mediaphone/my_ring", 0);

                    CopyDirFiles((TCHAR*)(LPCTSTR)::ringPath, L"/StorageCard/mediaphone/my_ring");
                }

                pDlg->KillTimer(IDT_COPYFILE_TIMER);
                pDlg->m_procbarSound.SetPos(4);

                pDlg->progress_status_ = finish_status;			
                strTemp = Data::LanguageResource::Get(Data::RI_SETTING_COPYFILE_FINISH);
                str = Util::StringOp::ToCString(strTemp);
                pDlg->m_sticTitle.SetWindowText(str);
            }
            break;
        case show_in:
            {
                if(!DetectDIR(m_chExtPath/*L"/UsbDisk/mediaphone/my_show"*/))
                {
                    pDlg->progress_status_ = finish_status;

                    strTemp = Data::LanguageResource::Get(Data::RI_SETTING_COPYFILE_ERROR3);
                    str = Util::StringOp::ToCString(strTemp);
                    pDlg->m_sticTitle.SetWindowText(str);
                }
                else			//copy
                {
                    pDlg->progress_status_ = copying_status;			
                    strTemp = Data::LanguageResource::Get(Data::RI_SETTING_COPYFILE_DOWITH);
                    str = Util::StringOp::ToCString(strTemp);
                    pDlg->m_sticTitle.SetWindowText(str);

                    pDlg->m_procbarSound.ShowWindow(SW_SHOW);
                    pDlg->m_procbarSound.SetPos(1);
                    pDlg->m_nPos = 1;
                    main->m_pSettingDlg->m_copyfileDlg->SetTimer(IDT_COPYFILE_TIMER, 300, 0);

                    CopyDirFiles(m_chExtPath, (TCHAR*)(LPCTSTR)::adPath, FALSE);

                    pDlg->KillTimer(IDT_COPYFILE_TIMER);
                    pDlg->m_procbarSound.SetPos(4);

                    pDlg->progress_status_ = finish_status;			
                    strTemp = Data::LanguageResource::Get(Data::RI_SETTING_COPYFILE_FINISH);
                    str = Util::StringOp::ToCString(strTemp);
                    pDlg->m_sticTitle.SetWindowText(str);
                }
            }
            break;
        case show_out:
            {
                pDlg->progress_status_ = copying_status;			
                strTemp = Data::LanguageResource::Get(Data::RI_SETTING_COPYFILE_DOWITH);
                str = Util::StringOp::ToCString(strTemp);
                pDlg->m_sticTitle.SetWindowText(str);
                pDlg->m_procbarSound.ShowWindow(SW_SHOW);
                pDlg->m_procbarSound.SetPos(1);
                pDlg->m_nPos = 1;
                pDlg->SetTimer(IDT_COPYFILE_TIMER, 300, 0);

                //在USB上建立目录
                if (m_devStatus == usb_device)
                {
                    CreateDirectoryW(L"/UsbDisk/mediaphone", 0);
                    CreateDirectoryW(L"/UsbDisk/mediaphone/my_show", 0);

                    CopyDirFiles((TCHAR*)(LPCTSTR)::adPath, L"/UsbDisk/mediaphone/my_show");
                }
                else if (m_devStatus == sdcard_device)
                {
                    CreateDirectoryW(L"/StorageCard/mediaphone", 0);
                    CreateDirectoryW(L"/StorageCard/mediaphone/my_show", 0);

                    CopyDirFiles((TCHAR*)(LPCTSTR)::adPath, L"/StorageCard/mediaphone/my_show");

                }


                pDlg->KillTimer(IDT_COPYFILE_TIMER);
                pDlg->m_procbarSound.SetPos(4);

                pDlg->progress_status_ = finish_status;			
                strTemp = Data::LanguageResource::Get(Data::RI_SETTING_COPYFILE_FINISH);
                str = Util::StringOp::ToCString(strTemp);
                pDlg->m_sticTitle.SetWindowText(str);
            }
            break;
        }

        // 	if(pDlg->type_ == backup_file)		//copy
        // 	{
        // 		pDlg->progress_status_ = copying_status;			
        // 		strTemp = Data::LanguageResource::Get(Data::RI_SETTING_COPYFILE_DOWITH);
        // 		str = strTemp.c_str();
        // 		pDlg->m_sticTitle.SetWindowText(str);
        // 		pDlg->m_procbarSound.ShowWindow(SW_SHOW);
        // 		pDlg->m_procbarSound.SetPos(1);
        // 		pDlg->m_nPos = 1;
        // 		pDlg->SetTimer(IDT_COPYFILE_TIMER, 300, 0);
        // 		CreateDirectoryW(L"/UsbDisk/mediaphone", 0);
        // 		CreateDirectoryW(L"/UsbDisk/mediaphone/backup", 0);
        // 		CopyFile(L"/flashdrv/res_dat/mmp.db", L"/UsbDisk/mediaphone/backup/mmp.db",  FALSE);
        // 		
        // 		pDlg->KillTimer(IDT_COPYFILE_TIMER);
        // 		pDlg->m_procbarSound.SetPos(4);
        // 		
        // 		pDlg->progress_status_ = finish_status;			
        // 		strTemp = Data::LanguageResource::Get(Data::RI_SETTING_COPYFILE_FINISH);
        // 		str = strTemp.c_str();
        // 		pDlg->m_sticTitle.SetWindowText(str);
        // 	}
        // 	else if(pDlg->type_ == restore_file)
        // 	{
        // 		BOOL DetectFile(TCHAR *sDir);
        // 		if(!DetectFile(L"/UsbDisk/mediaphone/backup/mmp.db"))
        // 		{
        // 			pDlg->progress_status_ = finish_status;
        // 			
        // 			strTemp = Data::LanguageResource::Get(Data::RI_SETTING_COPYFILE_ERROR3);
        // 			str = strTemp.c_str();
        // 			pDlg->m_sticTitle.SetWindowText(str);
        // 		}
        // 		else			//copy
        // 		{
        // 			pDlg->progress_status_ = copying_status;			
        // 			strTemp = Data::LanguageResource::Get(Data::RI_SETTING_COPYFILE_DOWITH);
        // 			str = strTemp.c_str();
        // 			pDlg->m_sticTitle.SetWindowText(str);
        // 			
        // 			pDlg->m_procbarSound.ShowWindow(SW_SHOW);
        // 			pDlg->m_procbarSound.SetPos(1);
        // 			pDlg->m_nPos = 1;
        // 			main->m_pSettingDlg->m_copyfileDlg->SetTimer(IDT_COPYFILE_TIMER, 300, 0);
        // 			
        // 			CopyFile(L"/UsbDisk/mediaphone/backup/mmp.db", L"/flashdrv/res_dat/mmp.db", FALSE);
        // 			
        // 			pDlg->KillTimer(IDT_COPYFILE_TIMER);
        // 			pDlg->m_procbarSound.SetPos(4);
        // 			
        // 			pDlg->progress_status_ = finish_status;			
        // 			strTemp = Data::LanguageResource::Get(Data::RI_SETTING_COPYFILE_FINISH);
        // 			str = strTemp.c_str();
        // 			pDlg->m_sticTitle.SetWindowText(str);
        // 		}
        // 	}
        // 	else if(pDlg->type_ == copy_file)
        // 	{
        // 		//录音备份
        // 		pDlg->progress_status_ = copying_status;			
        // 		strTemp = Data::LanguageResource::Get(Data::RI_SETTING_COPYFILE_DOWITH);
        // 		str = strTemp.c_str();
        // 		pDlg->m_sticTitle.SetWindowText(str);
        // 		pDlg->m_procbarSound.ShowWindow(SW_SHOW);
        // 		pDlg->m_procbarSound.SetPos(1);
        // 		pDlg->m_nPos = 1;
        // 		pDlg->SetTimer(IDT_COPYFILE_TIMER, 300, 0);
        // 		
        // 		//在USB上建立目录
        // 		CreateDirectoryW(L"/UsbDisk/mediaphone", 0);
        // 		CreateDirectoryW(L"/UsbDisk/mediaphone/backup", 0);
        // 		CreateDirectoryW(L"/UsbDisk/mediaphone/backup/flashdrv", 0);
        // 		CreateDirectoryW(L"/UsbDisk/mediaphone/backup/SDcard", 0);
        // 		CreateDirectoryW(L"/UsbDisk/mediaphone/backup/flashdrv/my_record", 0);
        // 		CreateDirectoryW(L"/UsbDisk/mediaphone/backup/SDcard/my_record", 0);
        // 		
        // 		extern void CopyDirFiles(TCHAR *src, TCHAR *des);
        // 		CopyDirFiles(L"/flashdrv/my_record", L"/UsbDisk/mediaphone/backup/flashdrv/my_record");
        // 		CopyDirFiles(L"/storagecard/my_record", L"/UsbDisk/mediaphone/backup/SDcard/my_record");
        // 		
        // 		pDlg->KillTimer(IDT_COPYFILE_TIMER);
        // 		pDlg->m_procbarSound.SetPos(4);
        // 		
        // 		pDlg->progress_status_ = finish_status;			
        // 		strTemp = Data::LanguageResource::Get(Data::RI_SETTING_COPYFILE_FINISH);
        // 		str = strTemp.c_str();
        // 		pDlg->m_sticTitle.SetWindowText(str);
        // 	}

    }

    void copyfileDlg::StartCopyFile()
    {
        hCopyFileThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)CopyFileProc, NULL, 0, NULL);
    }
    /////////////////////////////////////////////////////////////////////////////
    // copyfileDlg message handlers
}
