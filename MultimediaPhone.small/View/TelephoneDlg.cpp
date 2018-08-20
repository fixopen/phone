// telephonedlg.cpp : implementation file
//

#include "stdafx.h"
#include "TelephoneDlg.h"
#include "../MultimediaPhone.h"
#include "../MultimediaPhoneDlg.h"
#include "../Data/LanguageResource.h"
#include "../Data/SkinStyle.h"
#include "../Logical/Telephone.h"
#include "../inc/shine.h"
#include <queue>
//#include "sip.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace View {
    extern BOOL _test_call;
}

namespace View {
#define IDC_TELDLGSHOW		1000
#define IDC_TELDLGHIDE		1004
#define IDT_STOPAUTORECORDE_TIME	1001
    typedef enum __WAVEDEV_MSG__
    {
        WAV_SPEAKER_ONLY = 0x10000001,
        WAV_LINEOUT_ONLY = 0x10000002,
        WAV_SPEAKER_LINEOUT = 0x10000003,
        WAV_SNDEFFECT_POP	= 0x20000000,
        WAV_SNDEFFECT_3D	= 0x20000001,
        WAV_SNDEFFECT_CLASSIC	= 0x20000002,
        WAV_SNDEFFECT_JAZZ		= 0x20000003,
        WAV_LINEIN_MIC			= 0x30000001,
        WAV_LOCAL_MIC			= 0x30000002
    } WAVDEV_MESSAGE;

#define RIFF_FILE       mmioFOURCC('R','I','F','F')
#define RIFF_WAVE       mmioFOURCC('W','A','V','E')
#define RIFF_FORMAT     mmioFOURCC('f','m','t',' ')
#define RIFF_DATA       mmioFOURCC('d','a','t','a')

#define  IGNORERINGCOUNT 1

    int initResult = -1;

    HINSTANCE m_hSABTD;
    typedef int (__stdcall *SabtdInit)(void **ppSabtdStatus, int sampleRate, char *license);
    typedef int (__stdcall *SabtdUnInit)(void *pSabtdStatus);
    typedef int (__stdcall *SabtdSetParam)(void *pSabtdStatus, ParamType paramType, float paramValue);
    typedef int (__stdcall *SabtdGetParam)(void *pSabtdStatus, ParamType paramType, float *pParamValue);
    typedef int (__stdcall *SabtdReset)(void *pSabtdStatus);
    typedef int (__stdcall *SabtdProcess)(void *pSabtdStatus, int *result, short *data, int length);

    SabtdInit SabtdInit_;
    SabtdUnInit SabtdUnInit_;
    SabtdSetParam SabtdSetParam_;
    SabtdGetParam SabtdGetParam_;
    SabtdReset SabtdReset_;
    SabtdProcess SabtdProcess_;

    static void* m_sabtd_struct;
    static std::queue<OggCodec::PcmData*> CheckQueue;
    static HANDLE hCheckThread;
    static volatile bool bVoiceStop = true;


    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    DWORD WINAPI VoiceCheckThreadPrc(LPVOID lpParameter)
    {
        while (true)
        {
            ::Sleep(0);
            if (!CheckQueue.empty())
            {
                OggCodec::PcmData* m = CheckQueue.front();
                int toneDetected = 0;
                if (SabtdProcess_ != NULL)  
                {
                    if (SabtdProcess_(m_sabtd_struct, &toneDetected, m->data, m->dataLength) < 0)
                    {
                        //Dprintf("SABTD_Process Process Error!\n");
                        break;
                    }
                }

                if ((toneDetected == SABTD_TONE_DETECTED) || (toneDetected == SABTD_SILENCE_DETECTED))
                {
                    //Dprintf("SABTD_Process Process OK! %d\n", toneDetected);
                    if (SabtdReset_ != NULL)
                    {
                        SabtdReset_(m_sabtd_struct);
                    }
                    ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->HungOff();	
                }
                free(m->data);
                delete m;
                CheckQueue.pop();
                continue;
            }
            else if (bVoiceStop)
            {
                break;
            }
            Sleep(10);
        }

        if (SabtdReset_ != NULL)
        {
            SabtdReset_(m_sabtd_struct);
        }
        CloseHandle(hCheckThread);
        hCheckThread = NULL;
        //Dprintf("\nVoice Check over!\n");
        return 0;
    }

#define  SIMADDRLENGTH  29
    std::string StartSearch(FILE* file, const std::string& mobile, int low, int high)
    {
        if(low>high)
        {
            return "";
        }
        else
        {
            char buff[32];
            std::string result;
            int mid=(low+high)/2;
            fseek(file, 0, SEEK_SET);
            fseek(file, mid * SIMADDRLENGTH, SEEK_SET);
            fread(buff, 1, SIMADDRLENGTH, file);
            result = std::string(buff, SIMADDRLENGTH - 2);
            if(result.substr(0, 7) == mobile)
            {
                return result;
            }
            else if(result.substr(0, 7) > mobile)
            {
                return StartSearch(file,mobile,low,mid-1);
            }
            else
            {
                return StartSearch(file,mobile,mid+1,high);
            }
        }
    }

    /////////////////////////////////////////////////////////////////////////////
    // CTelephoneDlg dialog


    CTelephoneDlg::CTelephoneDlg(CWnd* pParent /*=NULL*/)
        : CCEDialog(CTelephoneDlg::IDD, pParent)
    {
        //{{AFX_DATA_INIT(CTelephoneDlg)
        // NOTE: the ClassWizard will add member initialization here
        //}}AFX_DATA_INIT
        m_bRing = FALSE;
    }


    void CTelephoneDlg::DoDataExchange(CDataExchange* pDX)
    {
        CDialog::DoDataExchange(pDX);
        //{{AFX_DATA_MAP(CTelephoneDlg)
        // NOTE: the ClassWizard will add DDX and DDV calls here
        //}}AFX_DATA_MAP
    }


    BEGIN_MESSAGE_MAP(CTelephoneDlg, CCEDialog)
        //{{AFX_MSG_MAP(CTelephoneDlg)
        ON_WM_TIMER()
        ON_MESSAGE(MM_WIM_OPEN,OnMM_WIM_OPEN)
        ON_MESSAGE(MM_WIM_DATA,OnMM_WIM_DATA)
        ON_MESSAGE(MM_WIM_CLOSE,OnMM_WIM_CLOSE)
        ON_MESSAGE(MM_WOM_OPEN,OnMM_WOM_OPEN)
        ON_MESSAGE(MM_WOM_DONE,OnMM_WOM_DONE)
        ON_MESSAGE(MM_WOM_CLOSE,OnMM_WOM_CLOSE)
        //}}AFX_MSG_MAP
        ON_BN_CLICKED(IDC_BUTTON_TELEPHONE_HIDE, OnButtonTelephoneHide)
        ON_BN_CLICKED(IDC_BUTTON_TELEPHONE_NOTE, OnButtonTelephoneNote)
        ON_BN_CLICKED(IDC_BUTTON_TELEPHONE_RECORD, OnButtonTelephoneRecord)
        ON_BN_CLICKED(IDC_BUTTON_TELEPHONE_HANDLE, OnButtonTelephoneHandle)
        ON_MESSAGE(WM_CLICKMJPG_TOAPP, OnClickMJPG)
    END_MESSAGE_MAP()

    /////////////////////////////////////////////////////////////////////////////
    // CTelephoneDlg message handlers
    LRESULT CTelephoneDlg::OnClickMJPG(WPARAM w, LPARAM l)
    {
        LRESULT result = S_OK;
        switch (w)
        {
        case 1:
            OnButtonTelephoneHide();
            break;
        case 2:
            OnButtonTelephoneNote();
            break;
        case 3:
            OnButtonTelephoneRecord();
            break;
        case 4:
            HandleOff();
            break;
        case 5:
            OnButtonTelephoneRecord();
            break;
        case 6:
            HandleOn();
            break;
        }
        return result;
    }

    BOOL CTelephoneDlg::OnInitDialog() 
    {
        CDialog::OnInitDialog();

        // TODO: Add extra initialization here
        std::string strTemp;
        CString str;

        m_sticNameNo.Create(str, WS_CHILD|WS_VISIBLE, CRect(48 + 54, 90 + 62, 536 + 54, 130 + 62), this, IDC_STATIC_TELEPHONE_NAMENO);
        m_sticNameNo.SetColor(RGB(0, 0, 0), RGB(203, 203, 203));//Data::g_partFrameInRectBackRGB[Data::g_skinstyle]);
        m_sticNameNo.SetFontSize(36);   

        m_sticCompany.Create(str, WS_CHILD|WS_VISIBLE, CRect(48 + 54, 150 + 62, 300 + 54, 180 + 62), this, IDC_STATIC_TELEPHONE_COMPANY);
        m_sticCompany.SetColor(RGB(0, 0, 0), RGB(203, 203, 203));//Data::g_partFrameInRectBackRGB[Data::g_skinstyle]);

        m_sticDepartment.Create(str, WS_CHILD|WS_VISIBLE, CRect(48 + 54, 190 + 62, 300 + 54, 220 + 62), this, IDC_STATIC_TELEPHONE_DEPARTMENT);
        m_sticDepartment.SetColor(RGB(0, 0, 0), RGB(203, 203, 203));//Data::g_partFrameInRectBackRGB[Data::g_skinstyle]);

        m_sticDuty.Create(str, WS_CHILD|WS_VISIBLE, CRect(48 + 54, 230 + 62, 300 + 54, 260 + 62), this, IDC_STATIC_TELEPHONE_DUTY);
        m_sticDuty.SetColor(RGB(0, 0, 0), RGB(203, 203, 203));//Data::g_partFrameInRectBackRGB[Data::g_skinstyle]);


        strTemp = Data::LanguageResource::Get(Data::RI_TEL_RECORDING);
        str = strTemp.c_str();
        m_sticRecordCaption.Create(str, WS_CHILD/*|WS_VISIBLE*/, CRect(350 + 54, 200 + 62, 448 + 54, 230 + 62), this);
        m_sticRecordCaption.SetColor(RGB(0, 0, 0), RGB(203, 203, 203));//Data::g_partFrameInRectBackRGB[Data::g_skinstyle]);

        m_sticRecord.Create(_T("00:00"), WS_CHILD/*|WS_VISIBLE*/, CRect(450 + 54, 200 + 62, 536 + 54, 230 + 62), this);
        m_sticRecord.SetColor(RGB(0, 0, 0), RGB(203, 203, 203));//Data::g_partFrameInRectBackRGB[Data::g_skinstyle]);

        strTemp = Data::LanguageResource::Get(Data::RI_TEL_DURATION);
        str = strTemp.c_str();
        m_sticDurationCaption.Create(str, WS_CHILD/*|WS_VISIBLE*/, CRect(350 + 54, 236 + 62, 448 + 54, 266 + 62), this);
        m_sticDurationCaption.SetColor(RGB(0, 0, 0), RGB(203, 203, 203));//Data::g_partFrameInRectBackRGB[Data::g_skinstyle]);

        m_sticDuration.Create(_T("00:00"), WS_CHILD/*|WS_VISIBLE*/, CRect(450 + 54, 236 + 62, 536 + 54, 266 + 62), this);
        m_sticDuration.SetColor(RGB(0, 0, 0), RGB(203, 203, 203));//Data::g_partFrameInRectBackRGB[Data::g_skinstyle]);

        /*
        strTemp = Data::LanguageResource::Get(Data::RI_TEL_HIDEBTN);
        str = strTemp.c_str();
        m_btnHide.Create(str, Data::g_buttonArcBMPPARTDILOAGID[0][Data::g_skinstyle], Data::g_buttonArcBMPPARTDILOAGID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(378, 78, 434, 98), this, IDC_BUTTON_TELEPHONE_HIDE);
        m_btnHide.SetBackRGB(Data::g_partFrameMainBackRGB[Data::g_skinstyle]);

        strTemp = Data::LanguageResource::Get(Data::RI_TEL_NOTEBTN);
        str = strTemp.c_str();
        m_btnNote.Create(str, Data::g_buttonArcBMPPARTDILOAGID[0][Data::g_skinstyle], Data::g_buttonArcBMPPARTDILOAGID[1][Data::g_skinstyle], WS_CHILD, CRect(378, 103, 434, 123), this, IDC_BUTTON_TELEPHONE_NOTE);
        m_btnNote.SetBackRGB(Data::g_partFrameMainBackRGB[Data::g_skinstyle]);

        strTemp = Data::LanguageResource::Get(Data::RI_TEL_RECORDEBTN);
        str = strTemp.c_str();
        m_btnRecord.Create(str, Data::g_buttonArcBMPPARTDILOAGID[0][Data::g_skinstyle], Data::g_buttonArcBMPPARTDILOAGID[1][Data::g_skinstyle], WS_CHILD, CRect(378, 128, 434, 148), this, IDC_BUTTON_TELEPHONE_RECORD);
        m_btnRecord.SetBackRGB(Data::g_partFrameMainBackRGB[Data::g_skinstyle]);

        strTemp = Data::LanguageResource::Get(Data::RI_TEL_ACCEPTBTN);
        str = strTemp.c_str();
        m_btnHandle.Create(str, Data::g_buttonArcBMPPARTDILOAGID[0][Data::g_skinstyle], Data::g_buttonArcBMPPARTDILOAGID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(378, 153, 434, 173), this, IDC_BUTTON_TELEPHONE_HANDLE);
        m_btnHandle.SetBackRGB(Data::g_partFrameMainBackRGB[Data::g_skinstyle]);
        */
        //	m_btnRecordTip.Create(_T(""),IDB_BITMAP31, IDB_BITMAP31, WS_CHILD|WS_VISIBLE, CRect(320 + 54, 200 + 62, 350 + 54, 230 + 62), this, IDC_BUTTON_TELEPHONE_RECORDTIP);
        //	m_btnRecordTip.SetBackRGB(Data::g_partFrameMainBackRGB[Data::g_skinstyle]);

        /*	
        m_sticBackground.Create(CRect(34, 40, 448, 184), this, 1);

        TextStruct ts[1];
        memset(ts, 0, sizeof(TextStruct) * 1);

        ts[0].txtRect = CRect(8, 0, 100, 20);
        ts[0].txtFontSize = 16;
        ts[0].sAlign = DT_LEFT | DT_BOTTOM;
        memcpy(ts[0].sTxt, Data::LanguageResource::Get(Data::RI_TEL_NEWTELSTC).c_str(), Data::LanguageResource::Get(Data::RI_TEL_NEWTELSTC).length());

        m_sticBackground.SetTextStruct(ts, 1);
        */
        m_MJPGList.Create(L"", WS_VISIBLE|WS_CHILD, CRect(54, 62, 746, 358), this);
        m_MJPGList.SetCurrentLinkFile(_T(".\\adv\\mjpg\\k1\\中文\\拨打电话.xml"));
        m_MJPGList.SetMJPGRect(CRect(54, 62, 746, 358));

        m_MJPGList.SetUnitIsShow(4, TRUE);
        m_MJPGList.SetUnitIsShow(5, FALSE);
        m_MJPGList.SetUnitIsShow(6, FALSE);
        m_MJPGList.SetUnitIsShow(7, FALSE);
        m_MJPGList.SetUnitIsShow(0, TRUE);
        m_MJPGList.SetUnitIsShow(1, TRUE);
        m_MJPGList.SetUnitIsShow(2, FALSE);
        m_MJPGList.SetUnitIsShow(3, TRUE);
        m_MJPGList.SetUnitIsShow(8, FALSE);
        m_MJPGList.SetUnitIsShow(9, FALSE);

        if (!InitData())
        {
            MessageBox(_T("Telephone Initialize error!\n"), _T("Error"));
        }

        return TRUE;  // return TRUE unless you set the focus to a control
        // EXCEPTION: OCX Property Pages should return FALSE
    }

    bool CTelephoneDlg::InitData(void)
    {
        m_strRecord = CString(Data::LanguageResource::Get(Data::RI_TEL_RECORDEBTN).c_str());
        m_strStopRecord = CString(Data::LanguageResource::Get(Data::RI_TEL_STOPRECORDEBTN).c_str());
        m_strHangOn = CString(Data::LanguageResource::Get(Data::RI_TEL_ACCEPTBTN).c_str());
        m_strHangOff = CString(Data::LanguageResource::Get(Data::RI_TEL_HUNGOFFBTN).c_str());
        m_uiTelephoneTimer = 1;
        m_uiTelephoneSecond = 0;
        m_uiRecordTimer = 2;
        m_uiRecordSecond = 0;
        m_uiRingCount = 0;
        m_uiTipTimer = 3;
        m_uiInNoCount = 0;
        m_uiRecordCound = 0;
        //	m_bAutoRecord = FALSE;
        m_bHasCallID = FALSE;

        //	m_spSimAddr = Util::shared_ptr<Data::SimAddr>(new Data::SimAddr);

        m_spContactInfo = Util::shared_ptr<Data::ContactInfo>(new Data::ContactInfo);
        m_spContactInfo->id(1);
        m_spSoundSegment = Util::shared_ptr<Data::SoundSegment>();

        m_bEnding = FALSE;
        m_bPlaying = FALSE;
        m_bRecording = FALSE;
        m_dwDataLength = 0;

        m_bPlayingLeaveWord = FALSE;
        m_bFirwall = FALSE;
        m_waveform.wFormatTag=WAVE_FORMAT_PCM;
        m_waveform.nChannels=CHANNELS;
        m_waveform.nSamplesPerSec=SAMPLESPERSEC;
        m_waveform.wBitsPerSample=BITSPERSAMPLE;
        m_waveform.nBlockAlign=m_waveform.nChannels * m_waveform.wBitsPerSample / 8;
        m_waveform.nAvgBytesPerSec=m_waveform.nBlockAlign * m_waveform.nSamplesPerSec;
        m_waveform.cbSize=0;	

        // 	m_pWaveHdr1 = reinterpret_cast<PWAVEHDR>(malloc(sizeof(WAVEHDR)));
        // 	m_pWaveHdr2 = reinterpret_cast<PWAVEHDR>(malloc(sizeof(WAVEHDR)));
        // 	m_pBuffer1=(PBYTE)malloc(INP_BUFFER_SIZE);
        // 	m_pBuffer2=(PBYTE)malloc(INP_BUFFER_SIZE);
        // 	
        // 	if (!m_pBuffer1 || !m_pBuffer2)
        // 	{
        // 		if (m_pBuffer1) free(m_pBuffer1);
        // 		if (m_pBuffer2) free(m_pBuffer2);
        // 		Dprintf("buffer malloc error!\n");
        // 	}

        for (int i = 0; i < WAVE_BUFFER_COUNT; ++i)
        {
            m_pBuffer[i] = 	(PBYTE)malloc(INP_BUFFER_SIZE);
            m_pWaveHdr[i] = reinterpret_cast<PWAVEHDR>(malloc(sizeof(WAVEHDR)));
        }

        m_hSABTD = NULL;
        m_hSABTD = LoadLibrary(_T("\\FlashDrv\\RES_DAT\\SABTD_CEDLL.dll")); 
        if (m_hSABTD == NULL)
        {
            SabtdInit_ = NULL;
            SabtdUnInit_ = NULL;
            SabtdSetParam_ = NULL;
            SabtdGetParam_ = NULL;
            SabtdReset_ = NULL;
            SabtdProcess_ = NULL;
            //return false;
        }
        else
        {
            SabtdInit_ = (SabtdInit)GetProcAddress(m_hSABTD, "SABTD_Init");
            SabtdUnInit_ = (SabtdUnInit)GetProcAddress(m_hSABTD, "SABTD_UnInit");
            SabtdSetParam_ = (SabtdSetParam)GetProcAddress(m_hSABTD, "SABTD_SetParam");
            SabtdGetParam_ = (SabtdGetParam)GetProcAddress(m_hSABTD, "SABTD_GetParam");
            SabtdReset_ = (SabtdReset)GetProcAddress(m_hSABTD, "SABTD_Reset");
            SabtdProcess_ = (SabtdProcess)GetProcAddress(m_hSABTD, "SABTD_Process");

            if(SabtdInit_ && SabtdUnInit_ && SabtdSetParam_ && SabtdGetParam_ && SabtdReset_ && SabtdProcess_) 
            {
                int initResult = 0;
                initResult = SabtdInit_(&m_sabtd_struct, SAMPLESPERSEC, "/\\FlashDrv\\RES_DAT\\sa_license.dat");
                if ((initResult==-1000) || (initResult<0 || m_sabtd_struct==NULL))
                {
                    //Dprintf("SABTD_CEDLL Initialize error!\n");
                    SabtdInit_ = NULL;
                    SabtdUnInit_ = NULL;
                    SabtdSetParam_ = NULL;
                    SabtdGetParam_ = NULL;
                    SabtdReset_ = NULL;
                    SabtdProcess_ = NULL;
                    //return false;		
                }
                else
                {
                    if (initResult == SARTC_NORMAL)
                    {
                        SabtdSetParam_(m_sabtd_struct, SABTD_PARAMTYPE_BUSYTONE_NUM, 3);
                        //SabtdSetParam_(m_sabtd_struct, SABTD_PARAMTYPE_SILENCE_LENGTH, 20);
                    }
                }
            }
        }

        m_pOggCodec = new OggCodec();

        //((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->SetSoundRingVal(-500);
        //((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->OpenTelRing(FALSE);

        //m_spSimAddr->Import("\\flashdrv\\mobile.txt");

        return true;
    }

    void CTelephoneDlg::OnButtonTelephoneHide()
    {
        ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->StopRing(true);
        ShowWindow_(FALSE);
    }

    void CTelephoneDlg::OnButtonTelephoneNote()
    {
        //((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->SwitchPanel_(IDC_BUTTON_INFORMATION);
        ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pNotebookDlg->ShowWindow(SW_SHOW);
    }

    std::string CTelephoneDlg::GetSoundPath(void)
    {
        path = ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pSetting->soundPath();

        BOOL DetectDIR(TCHAR *sDir);
        if (path == ssStorageCardRecordPath)
        {
            BOOL sd = DetectDIR((LPTSTR)(LPCTSTR)csStarageCard);
            if (sd)
            {
                if (!DetectDIR((LPTSTR)(LPCTSTR)csStorageCardRecordPath))
                {
                    CreateDirectory((LPTSTR)(LPCTSTR)csStorageCardRecordPath, NULL);
                }
            }
            else
            {
                path = ssFlashRecordPath;
                if (!DetectDIR((LPTSTR)(LPCTSTR)csFlashRecordPath))
                {
                    CreateDirectory((LPTSTR)(LPCTSTR)csFlashRecordPath, NULL);
                }
            }
        }
        return path;
    }

    void CTelephoneDlg::OnButtonTelephoneRecord()
    {
        // 	CString caption;
        // 	m_btnRecord.GetWindowText(caption);
        // 	if (caption == m_strRecord)
        if (!m_bRecording)
        {
            GetSoundPath();

            ULARGE_INTEGER freeBytes;
            ULARGE_INTEGER totalBytes;
            GetDiskFreeSpaceEx(Util::StringOp::ToCString(path), &freeBytes, &totalBytes, NULL);

            if (path == ssFlashRecordPath)
            {		
                //if (totalBytes.QuadPart < MINFREEBYTES)
                if (freeBytes.QuadPart < totalBytes.QuadPart * 0.1)
                {
                    ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pWarningNoFlashDlg->ShowWindow_(SW_SHOW);
                    return;
                }

                //m_uiRemainRecordSecond = (totalBytes.QuadPart - MINFREEBYTES) / SECONDBYTES + 1;
                m_uiRemainRecordSecond = (freeBytes.QuadPart - (totalBytes.QuadPart * 0.1)) / SECONDBYTES + 1;
            }
            else
            {
                if (freeBytes.QuadPart < SECONDBYTES)
                {
                    ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pWarningNoFlashDlg->ShowWindow_(SW_SHOW);
                    return;
                }
                m_uiRemainRecordSecond = (UINT)freeBytes.QuadPart / SECONDBYTES + 1;
            }

            if (RecStart())
            {
                //Dprintf("RecStart Ok\r\n");

                m_spSoundSegment = Util::shared_ptr<Data::SoundSegment>(new Data::SoundSegment);
                m_vSoundSegment.push_back(m_spSoundSegment);
                CTime t;
                t = CTime::GetCurrentTime();
                CString filename;
                filename.Format(_T("%02d%02d%02d.spx"), t.GetHour(), t.GetMinute(), t.GetSecond());
                m_sRecordFileName = path + Util::StringOp::FromCString(filename);
                m_spSoundSegment->filename(m_sRecordFileName);
                m_spSoundSegment->startTime(CTime::GetCurrentTime());

                //Dprintf("m_pOggCodec->StartEncode 0\r\n");

                m_pOggCodec->EncodeFile(m_sRecordFileName);
                //Dprintf("m_pOggCodec->StartEncode 1\r\n");
                if (m_pOggCodec->StartEncode())
                {
                    //Dprintf("m_pOggCodec->StartEncode 2\r\n");
                    m_uiRecordSecond = 0;
                    SetTimer(m_uiRecordTimer, 1000, 0);				
                    //m_btnRecordTip.ShowWindow(TRUE);
                    m_sticRecordCaption.ShowWindow(TRUE);
                    m_sticRecord.ShowWindow(TRUE);
                    //m_btnRecord.SetWindowText(m_strStopRecord);			
                    m_MJPGList.SetUnitIsShow(2, FALSE);
                    m_MJPGList.SetUnitIsShow(8, TRUE);
                }
            }			
        }
        else
        {
            if (RecStop())
            {
                KillTimer(m_uiRecordTimer);
                // 			m_btnRecordTip.ShowWindow(FALSE);
                // 			m_sticRecordCaption.ShowWindow(FALSE);
                // 			m_sticRecord.ShowWindow(FALSE);
                // 			m_sticRecord.SetWindowText(_T(""));
                // 			m_btnRecord.SetWindowText(m_strRecord);

                // 			if (m_uiRecordSecond == 0)
                // 			{
                // 				m_uiRecordSecond = 1;
                // 			}
                //Dprintf("record second = %d\n", m_uiRecordSecond);
                if (m_uiRecordSecond > 1)
                {
                    m_spContactInfo->isSound(true);
                }
                m_spSoundSegment->duration(m_uiRecordSecond);
                // 			m_spSoundSegment = Util::shared_ptr<Data::SoundSegment>();
                // 			m_uiRecordSecond = 0;
            }
        }
    }

    void CTelephoneDlg::HandleOn(void)
    {
        ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->HungOn();
    }

    void CTelephoneDlg::HandleOff(void)
    {
        ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pFSM->fireEvent(CMultimediaPhoneDlg::teHangOff, NULL);
        ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->HungOff();
    }

    void CTelephoneDlg::OnButtonTelephoneHandle()
    {
        //Dprintf("\nHandle \n");
        CString caption;
        //	m_btnHandle.GetWindowText(caption);
        if (caption == m_strHangOn)
        {
            ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->HungOn();
        }
        else 
        {
            ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->HungOff();
        }	
    }

    void CTelephoneDlg::HangOff_(void* param)
    {
        //Dprintf("TelDlg hide1\n");
        ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->StopRing(true);

        KillTimer(m_uiTipTimer);
        KillTimer(m_uiTelephoneTimer);

        // 	CString caption;
        // 	m_btnRecord.GetWindowText(caption);
        // 	if (caption == m_strStopRecord)
        if (m_bRecording)
        {
            OnButtonTelephoneRecord();
        }

        if (m_spContactInfo->id() == 0)// != Util::shared_ptr<Data::ContactInfo>()
        {
            m_spContactInfo->duration(m_uiTelephoneSecond);
            if (m_spContactInfo->type() == Data::citInNo)
            {
                m_spContactInfo->played(false);
                //++m_uiInNoCount;
            }

            ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pContactInfoDlg->SaveContactInfo(m_spContactInfo);

            if (!m_vSoundSegment.empty())
            {
                for (std::vector<Util::shared_ptr<Data::SoundSegment> >::iterator iter = m_vSoundSegment.begin(); iter != m_vSoundSegment.end(); ++iter)
                {
                    if ((*iter)->duration() > 1)
                    {
                        (*iter)->telephoneNumber(m_spContactInfo->telephoneNumber());
                        (*iter)->name(m_spContactInfo->name());
                        (*iter)->contactInfoId(m_spContactInfo->id());
                        (*iter)->contactId(m_spContactInfo->contactId());

                        if (m_spContactInfo->type() == Data::citInNo)
                        {
                            (*iter)->type(Data::sstLeaveWord);
                            (*iter)->played(false);
                        }
                        else
                        {
                            (*iter)->type(Data::sstTeleRecord);
                        }

                        ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSoundDlg->SaveSoundSegment(*iter);
                    }
                    //else
                    // 				{
                    // 					CFile::Remove(Util::StringOp::ToCString((*iter)->filename()));
                    // 				}
                }
            }
            m_vSoundSegment.clear();

            ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pContactInfoDlg->ShowItemsInList(0);
            if (m_spContactInfo->type() == Data::citInNo)
            {
                ::SendMessage(((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pMainDlg->GetSafeHwnd(), WM_TELNOTIFY, 0, 0);
            }
        }

        m_spContactInfo->telephoneNumber(Data::TelephoneNumber(""));
        m_spContactInfo->played(true);
        m_spContactInfo->contactId(0);
        m_spContactInfo->name("");
        m_spContactInfo->isSound(false);
        //	m_spContactInfo = Util::shared_ptr<Data::ContactInfo>();
        m_uiTelephoneSecond = 0;
        //	m_spSoundSegment = Util::shared_ptr<Data::SoundSegment>();
        //	m_uiRecordSecond = 0;
        m_uiRingCount = 0;
        //m_bAutoRecord = FALSE;

        m_bHasCallID = FALSE;
        m_bPlayingLeaveWord = FALSE;
        m_bFirwall = FALSE;
        m_sTipFilename = "";

        ShowWindow_(FALSE);
        m_sticDurationCaption.ShowWindow(FALSE);
        m_sticDuration.SetWindowText(_T(""));
        m_sticDuration.ShowWindow(FALSE);

        // 	m_btnNote.ShowWindow(FALSE);
        // 	m_btnRecord.ShowWindow(FALSE);
        // 	m_btnHandle.SetWindowText(m_strHangOn);

        m_MJPGList.SetUnitIsShow(9, FALSE);
        m_MJPGList.SetUnitIsShow(3, TRUE);
        m_MJPGList.SetUnitIsShow(2, FALSE);
        m_MJPGList.SetUnitIsShow(8, FALSE);

        //	m_btnHandle.ShowWindow(FALSE);
        m_sticNameNo.SetWindowText(_T(""));
        m_sticCompany.SetWindowText(_T(""));
        m_sticDepartment.SetWindowText(_T(""));
        m_sticDuty.SetWindowText(_T(""));

        ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->OpenTelRing(FALSE);

        //Dprintf("TelDlg hide\n");
        ShowWindow_(FALSE);
        SetTimer(IDC_TELDLGHIDE, 10, NULL);

        //lxz 20090207
        ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pMainDlg->SendMessage(WM_PLAYVIDEO, 1, 0);			//恢复视频
        ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pMainDlg->m_mainMp3Dlg_->SendMessage(WM_OUTEVENT, 0, 1);
    }

    void CTelephoneDlg::HangOn_(void* param)
    {
        //Dprintf("TelDlg show1\n");
        // 	TextStruct ts[1];
        // 	memset(ts, 0, sizeof(TextStruct) * 1);
        // 	
        // 	ts[0].txtRect = CRect(8, 0, 100, 20);
        // 	ts[0].txtFontSize = 16;
        // 	ts[0].sAlign = DT_LEFT | DT_BOTTOM;
        // 	memcpy(ts[0].sTxt, Data::LanguageResource::Get(Data::RI_TEL_DIALTELSTC).c_str(), Data::LanguageResource::Get(Data::RI_TEL_DIALTELSTC).length());
        // 	
        // 	m_sticBackground.SetTextStruct(ts, 1);
        // 

        m_MJPGList.SetUnitIsShow(4, TRUE);
        m_MJPGList.SetUnitIsShow(5, FALSE);
        m_MJPGList.SetUnitIsShow(6, FALSE);
        m_MJPGList.SetUnitIsShow(7, FALSE);
        m_MJPGList.SetUnitIsShow(9, FALSE);
        m_MJPGList.SetUnitIsShow(3, TRUE);
        m_MJPGList.SetUnitIsShow(8, FALSE);
        m_MJPGList.SetUnitIsShow(2, FALSE);

        // 	m_btnHandle.SetWindowText(m_strHangOff);
        // 	m_btnHandle.ShowWindow(TRUE);

        //SipShowIM(SIPF_OFF);
        //ShowWindow_(TRUE);
        SetTimer(IDC_TELDLGSHOW, 10, NULL);

    }

    void CTelephoneDlg::Key_(void* param)
    {
        BOOL ret1 = m_MJPGList.GetUnitIsShow(8);
        BOOL ret2 = m_MJPGList.GetUnitIsShow(2);
        if(!ret1 && !ret2)
            m_MJPGList.SetUnitIsShow(2, TRUE);

        CString number;
        m_sticNameNo.GetWindowText(number);
        number += char(param);
        if (number.GetLength() > 20)
        {
            m_sticNameNo.SetWindowText(number.Right(20));
        }
        else
        {
            m_sticNameNo.SetWindowText(number);
        }

        // 	if (!m_spContactInfo)
        if (m_spContactInfo->id() != 0)
        {
            m_spContactInfo->id(0);
            // 		m_spContactInfo = Util::shared_ptr<Data::ContactInfo>(new Data::ContactInfo);
            m_spContactInfo->type(Data::citOut);
            m_spContactInfo->startTime(CTime::GetCurrentTime());
        }
        m_spContactInfo->telephoneNumber(Util::StringOp::FromCString(number));
        m_sTelephoneNumber = Util::StringOp::FromCString(number);
        if (!IsWindowVisible())
        {
            //ShowWindow_(TRUE);
            SetTimer(IDC_TELDLGSHOW, 10, NULL);
        }
    }

    void CTelephoneDlg::SubKey_(void* param)
    {
        BOOL ret1 = m_MJPGList.GetUnitIsShow(8);
        BOOL ret2 = m_MJPGList.GetUnitIsShow(2);
        if(!ret1 && !ret2)
            m_MJPGList.SetUnitIsShow(2, TRUE);

        CString number;
        m_sticNameNo.GetWindowText(number);
        number += char(param);

        if (number.GetLength() > 20)
        {
            m_sticNameNo.SetWindowText(number.Right(number.GetLength() - 20));
        }
        else
        {
            m_sticNameNo.SetWindowText(number);
        }

        if (!IsWindowVisible())
        {
            //ShowWindow_(TRUE);
            SetTimer(IDC_TELDLGSHOW, 10, NULL);
        }
    }

    void CTelephoneDlg::Connect_(void* param)
    {
        m_MJPGList.SetUnitIsShow(9, FALSE);
        m_MJPGList.SetUnitIsShow(3, TRUE);
        // 		m_btnHandle.SetWindowText(m_strHangOff);
        // 		m_btnHandle.ShowWindow(TRUE);


        if (m_bPlayingLeaveWord)
        {
            // 		m_btnNote.ShowWindow(TRUE);
            // 		if (!((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pSetting->isMustRecord())
            // 		{
            // 			m_btnRecord.ShowWindow(TRUE);
            // 		}
            ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pFSM->setStartState(CMultimediaPhoneDlg::tsRing);
            return;
        }

        ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->StopRing(true);

        if (((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pSetting->isMustRecord())
        {
            // 		CString caption;
            // 		m_btnRecord.GetWindowText(caption);
            // 		if (caption == m_strRecord)
            if (!m_bRecording)
            {
                OnButtonTelephoneRecord();
            }
            m_MJPGList.SetUnitIsShow(8, FALSE);
            m_MJPGList.SetUnitIsShow(2, FALSE);
        }
        else
        {
            m_MJPGList.SetUnitIsShow(8, FALSE);
            m_MJPGList.SetUnitIsShow(2, TRUE);
        }

        // 	TextStruct ts[1];
        // 	memset(ts, 0, sizeof(TextStruct) * 1);
        // 	
        // 	ts[0].txtRect = CRect(8, 0, 100, 20);
        // 	ts[0].txtFontSize = 16;
        // 	ts[0].sAlign = DT_LEFT | DT_BOTTOM;
        // 	memcpy(ts[0].sTxt, Data::LanguageResource::Get(Data::RI_TEL_TELINGSTC).c_str(), Data::LanguageResource::Get(Data::RI_TEL_TELINGSTC).length());
        // 	
        // 	m_sticBackground.SetTextStruct(ts, 1);

        m_MJPGList.SetUnitIsShow(7, TRUE);
        m_MJPGList.SetUnitIsShow(5, FALSE);
        m_MJPGList.SetUnitIsShow(6, FALSE);
        m_MJPGList.SetUnitIsShow(4, FALSE);

        m_sticDurationCaption.ShowWindow(TRUE);
        m_sticDuration.ShowWindow(TRUE);
        m_sticDuration.SetWindowText(_T(""));

        //m_btnNote.ShowWindow(TRUE);
        // 	if (!((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pSetting->isMustRecord())
        // 	{
        // 		//m_btnRecord.ShowWindow(TRUE);
        // 		m_MJPGList.SetUnitIsShow(8, FALSE);
        // 		m_MJPGList.SetUnitIsShow(2, TRUE);
        //	}

        if (!IsWindowVisible())
        {
            //ShowWindow_(TRUE);
            SetTimer(IDC_TELDLGSHOW, 10, NULL);
        }

        if (m_spContactInfo->type() == Data::citOut)
        {
            //CString number;
            //m_sticNameNo.GetWindowText(number);
            //m_spContactInfo->telephoneNumber(Util::StringOp::FromCString(number));
            m_spContactInfo->telephoneNumber(m_sTelephoneNumber);
            m_spContactInfo->startTime(CTime::GetCurrentTime());
            //std::vector<Util::shared_ptr<Data::Contact> > result = FindCallContact(Util::StringOp::FromCString(number));
            std::vector<Util::shared_ptr<Data::Contact> > result = FindCallContact(m_sTelephoneNumber);
            if (!result.empty())
            {
                //ShowContact(result[0], Util::StringOp::FromCString(number));
                ShowContact(result[0], m_sTelephoneNumber);
            }
        }
        else
        {
            m_spContactInfo->type(Data::citInOk);
        }

        SetTimer(m_uiTelephoneTimer, 1000, 0);
    }

    void CTelephoneDlg::Ring_(void* param)
    {
        ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSoundDlg->m_pRecordSoundDlg->CloseSound();
        ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSoundDlg->m_pPlaySoundDlg->CloseSound();

        if (View::_test_call)
        {
            HandleOn();
            Sleep(1000);
            OnButtonTelephoneRecord();
            SetTimer(10, 30000, NULL);
        }

        m_bRing = TRUE;
        m_MJPGList.SetUnitIsShow(8, FALSE);
        m_MJPGList.SetUnitIsShow(2, FALSE);

        //	if (!m_spContactInfo)
        if (m_spContactInfo->id() != 0)
        {
            m_spContactInfo->id(0);
            //		m_spContactInfo = Util::shared_ptr<Data::ContactInfo>(new Data::ContactInfo);
            m_spContactInfo->startTime(CTime::GetCurrentTime());
            m_spContactInfo->type(Data::citInNo);
        }	

        if (!IsWindowVisible())
        {
            // 			TextStruct ts[1];
            // 			memset(ts, 0, sizeof(TextStruct) * 1);
            // 			
            // 			ts[0].txtRect = CRect(8, 0, 100, 20);
            // 			ts[0].txtFontSize = 16;
            // 			ts[0].sAlign = DT_LEFT | DT_BOTTOM;
            // 			memcpy(ts[0].sTxt, Data::LanguageResource::Get(Data::RI_TEL_NEWTELSTC).c_str(), Data::LanguageResource::Get(Data::RI_TEL_NEWTELSTC).length());
            // 			
            // 			m_sticBackground.SetTextStruct(ts, 1);

            m_MJPGList.SetUnitIsShow(5, TRUE);
            m_MJPGList.SetUnitIsShow(4, FALSE);
            m_MJPGList.SetUnitIsShow(6, FALSE);
            m_MJPGList.SetUnitIsShow(7, FALSE);
            m_MJPGList.SetUnitIsShow(3, FALSE);
            m_MJPGList.SetUnitIsShow(9, TRUE);

            //m_btnHandle.SetWindowText(m_strHangOn);
            //m_btnHandle.ShowWindow(TRUE);


            //SipShowIM(SIPF_OFF);
            //ShowWindow_(TRUE);
            SetTimer(IDC_TELDLGSHOW, 10, NULL);
            //m_bAutoRecord = FALSE;
        }

        if (((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pSetting->isFirewall())
        {
            int firetype = ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pSetting->firewallType();
            if (firetype == 3)
            {
                m_bFirwall = TRUE;
                return;
            }
        }

        ++m_uiRingCount;
        if ((m_uiRingCount == (IGNORERINGCOUNT + 1)) && !m_bHasCallID)
        {
            if (((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pSetting->isUseSpecRing())
            {
                std::string ringname = ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pSetting->defaultRingFilename();
                CFileStatus   status;
                if (CFile::GetStatus(LPCTSTR(Util::StringOp::ToCString(ringname)),status) && (ringname.find('.') != -1))
                {
                    ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->StartRing((LPTSTR)(LPCTSTR)Util::StringOp::ToCString(ringname));
                }
                else
                {
                    ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->OpenTelRing();
                }
            }
            else
            {
                ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->OpenTelRing();
            }
        }

        if (!m_bFirwall && ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pSetting->isAutoReply())
        {
            if (m_uiRingCount - IGNORERINGCOUNT ==  ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pSetting->autoReplyRingCount())
            {
                if (m_sTipFilename == "")
                {
                    m_sTipFilename = ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pSetting->defaultTipFilename();
                }
                CFileStatus   status;
                if(m_sTipFilename == "" || m_sTipFilename == "默认" || (!CFile::GetStatus(LPCTSTR(Util::StringOp::ToCString(m_sTipFilename)),status)))
                    m_sTipFilename = "\\flashdrv\\tip1.wav";
                if (CFile::GetStatus(LPCTSTR(Util::StringOp::ToCString(m_sTipFilename)),status))
                {
                    CString s = Util::StringOp::ToCString(m_sTipFilename.c_str());
                    if(s.Find(L".wav") > 0)				
                    {
                        ::KillTimer((theApp.m_pMainWnd)->m_hWnd, ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_uiRingTimer);
                        ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->StopRing(true);
                        ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->OpenTelRing(FALSE);
                        m_bPlayingLeaveWord = TRUE;
                        HangOnToRecord();

                        ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->StartRing((LPTSTR)(LPCTSTR)Util::StringOp::ToCString(m_sTipFilename), 1);
                        SetTimer(m_uiTipTimer, 5 * 1000, 0);

                    }
                    else if(s.Find(L".spx") > 0)
                    {
                        ::KillTimer((theApp.m_pMainWnd)->m_hWnd, ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_uiRingTimer);
                        ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->StopRing(true);
                        ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->OpenTelRing(FALSE);
                        m_bPlayingLeaveWord = TRUE;
                        HangOnToRecord();

                        std::vector<Util::shared_ptr<Data::SoundSegment> > result = Data::SoundSegment::GetFromDatabase("type = 2 AND isTip = 1 AND filename = '" + m_sTipFilename + "'");	
                        if (result.size() > 0)
                        {
                            int seconds = result[0]->duration();
                            seconds += 2;

                            m_pOggCodec->DecodeFile(m_sTipFilename);
                            m_pOggCodec->StartDecode();
                            SetTimer(m_uiTipTimer, seconds * 1000, 0);
                        }
                        else
                        {
                            SetTimer(m_uiTipTimer, 2 * 1000, 0);
                        }
                    }
                }
            }		
        }
    }

    void CTelephoneDlg::CallID_(void* param)
    {
        ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSoundDlg->m_pRecordSoundDlg->CloseSound();
        ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSoundDlg->m_pPlaySoundDlg->CloseSound();

        m_bRing = TRUE;
        m_bHasCallID = TRUE;
        if (!IsWindowVisible())
        {
            // 		TextStruct ts[1];
            // 		memset(ts, 0, sizeof(TextStruct) * 1);
            // 		
            // 		ts[0].txtRect = CRect(8, 0, 100, 20);
            // 		ts[0].txtFontSize = 16;
            // 		ts[0].sAlign = DT_LEFT | DT_BOTTOM;
            // 		memcpy(ts[0].sTxt, Data::LanguageResource::Get(Data::RI_TEL_NEWTELSTC).c_str(), Data::LanguageResource::Get(Data::RI_TEL_NEWTELSTC).length());
            // 		
            // 		m_sticBackground.SetTextStruct(ts, 1);

            m_MJPGList.SetUnitIsShow(5, TRUE);
            m_MJPGList.SetUnitIsShow(4, FALSE);
            m_MJPGList.SetUnitIsShow(6, FALSE);
            m_MJPGList.SetUnitIsShow(7, FALSE);
            m_MJPGList.SetUnitIsShow(3, FALSE);
            m_MJPGList.SetUnitIsShow(9, TRUE);

            // 		m_btnHandle.SetWindowText(m_strHangOn);
            // 		m_btnHandle.ShowWindow(TRUE);

            // 		//ShowWindow_(TRUE);
            // 		if (((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pSetting->isFirewall())
            // 		{
            // 			int firetype = ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pSetting->firewallType();
            // 			if (firetype != 3)
            // 			{
            // 				SetTimer(IDC_TELDLGSHOW, 10, NULL);
            // 			}
            // 		}
            // 		else
            SetTimer(IDC_TELDLGSHOW, 10, NULL);
            //SipShowIM(SIPF_OFF);
        }

        //	if (!m_spContactInfo)
        if (m_spContactInfo->id() != 0)
        {
            m_spContactInfo->id(0);
            //		m_spContactInfo = Util::shared_ptr<Data::ContactInfo>(new Data::ContactInfo);
            m_spContactInfo->startTime(CTime::GetCurrentTime());
            m_spContactInfo->type(Data::citInNo);
        }

        CALLID_INFO* info = (CALLID_INFO*)param;
        std::string tel = info->number;
        //Dprintf("Call ID telephone number:\n");
        //Dprintf(tel.c_str());

        std::string city;

        if (((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pSetting->thisTelephoneNumber().number().length() >= 2)
        {
            std::string code = ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pSetting->thisTelephoneNumber().number();
            if (code[0] != '0')
            {
                code = "0" + code;
            }

            std::string mobile = tel;
            if (tel[0] == '0')
            {
                int codeLen = code.length();
                if (tel.substr(0, codeLen) == code)
                {
                    tel = tel.substr(codeLen);
                }
                else
                {
                    mobile = tel.substr(1);
                }
            }
            mobile = mobile.substr(0, 7);

            //std::vector<Util::shared_ptr<Data::SimAddr> > addr = Data::SimAddr::GetFromDatabase("mobile = '" + mobile + "'");
            //if (!addr.empty())
            //{
            //	if (addr[0]->no() == code)
            //	{
            //		tel = tel.substr(1);
            //	}
            //	city = addr[0]->city();
            //}
            if (!mobile.empty())
            {
                FILE* file = fopen("\\Flashdrv\\res_dat\\mobile.txt" , "rb");
                if (file != NULL)
                {
                    std::string addr = StartSearch(file, mobile, 0, 93935);
                    if (addr != "")
                    {
                        if (addr.substr(23, code.length()) == code)
                        {
                            if (tel[0] == '0')
                            {
                                tel = tel.substr(1);
                            }
                        }
                        city = addr.substr(8, 14);
                    }
                    fclose(file);
                }
            }
        }
        else
        {
            std::string mobile = tel;
            if (tel[0] == '0')
            {
                mobile = tel.substr(1);
            }
            mobile = mobile.substr(0, 7);
            FILE* file = fopen("\\Flashdrv\\res_dat\\mobile.txt" , "rb");
            if (file != NULL)
            {
                std::string addr = StartSearch(file, mobile, 0, 93935);
                if (addr != "")
                {
                    city = addr.substr(8, 14);
                }
                fclose(file);
            }
        }


        m_spContactInfo->telephoneNumber(Data::TelephoneNumber(tel));

        //	CString number = info->number;
        // 	CString number = Util::StringOp::ToCString(tel);
        // 	m_sticNameNo.SetWindowText(number);
        m_sticNameNo.SetWindowText(Util::StringOp::ToCString(tel + city));
        // 	if (!number.IsEmpty())
        if (tel != "")
        {	
            //		std::vector<Util::shared_ptr<Data::Contact> > result = FindCallContact(Util::StringOp::FromCString(number));
            std::vector<Util::shared_ptr<Data::Contact> > result = FindCallContact(tel);
            if (result.empty() && (tel[0] == '0'))
            {
                result = FindCallContact(tel.substr(1));
            }
            if (!result.empty())
            {
                ShowContact(result[0], tel + city);

                if (((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pSetting->isFirewall())
                {
                    int firetype = ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pSetting->firewallType();
                    Data::ContactType type = result[0]->type();

                    if (firetype == 0 && type == Data::ctBlacklist)
                    {
                        m_bFirwall = TRUE;
                        return;
                    }
                    if (firetype == 1 && (type == Data::ctBlacklist))//!(type == Data::ctNormal || type == Data::ctVip)
                    {
                        m_bFirwall = TRUE;
                        return;
                    }

                    if (firetype == 2 && type != Data::ctVip)
                    {
                        m_bFirwall = TRUE;
                        return;
                    }

                    if (firetype == 3)
                    {
                        m_bFirwall = TRUE;
                        return;
                    }
                }

                if (result[0]->group())// && result[0]->group()->ring()
                {
                    //Dprintf("group name:  ");
                    //Dprintf(result[0]->group()->name().c_str());
                    std::string tipname = result[0]->group()->tipName();
                    if (tipname == Data::LanguageResource::Get(Data::RI_CARD_DEFAULT))
                    {
                        tipname = ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pSetting->defaultTipFilename();
                    }

                    CFileStatus   status;
                    if (CFile::GetStatus(LPCTSTR(Util::StringOp::ToCString(tipname)),status))
                    {
                        m_sTipFilename = tipname;
                    }

                    std::string ringname = result[0]->group()->ringName();
                    if (ringname == Data::LanguageResource::Get(Data::RI_CARD_DEFAULT))
                    {
                        ringname = ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pSetting->defaultRingFilename();
                    }

                    if (CFile::GetStatus(LPCTSTR(Util::StringOp::ToCString(ringname)),status) && (ringname.find('.') != -1))
                    {
                        ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->StartRing((LPTSTR)(LPCTSTR)Util::StringOp::ToCString(ringname));
                        return;
                    }
                }
            }
            else
            {
                if (((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pSetting->isFirewall())
                {
                    int firetype = ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pSetting->firewallType();
                    if (firetype == 1 || firetype == 2 || firetype == 3)
                    {
                        m_bFirwall = TRUE;
                        return;
                    }
                }
            }
        }

        if (((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pSetting->isFirewall())
        {
            int firetype = ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pSetting->firewallType();
            if (firetype == 3)
            {
                m_bFirwall = TRUE;
                return;
            }
        }

        if (((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pSetting->isUseSpecRing())
        {
            std::string ringname = ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pSetting->defaultRingFilename();
            CFileStatus   status;
            if (CFile::GetStatus(LPCTSTR(Util::StringOp::ToCString(ringname)),status) && (ringname.find('.') != -1))
            {
                ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->StartRing((LPTSTR)(LPCTSTR)Util::StringOp::ToCString(ringname));
                return;
            }
        }

        ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->OpenTelRing();
    }

    void CTelephoneDlg::Mute(void)
    {
        //Dprintf("Mute\n");
        ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->Mute();
    }

    void CTelephoneDlg::Redial(void)
    {
        // 	if (m_sTelephoneNumber != "" && (((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pFSM->getCurrentState() == CMultimediaPhoneDlg::tsHangOn))
        // 	{
        // 		DialContact(m_sTelephoneNumber);
        // 	}
    }

    void CTelephoneDlg::HangOnInRecord(void)
    {
        //if (!m_bAutoRecord)
        //Dprintf("Hang On IN Record 1\n");
        //	if (m_spContactInfo && (m_spContactInfo->type() == Data::citOut))
        if ((m_spContactInfo->id() == 0) && (m_spContactInfo->type() == Data::citOut))
            return;
        //Dprintf("Hang On IN Record 2\n");

        if (m_bPlayingLeaveWord)
        {
            KillTimer(m_uiTipTimer);
            ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->StopRing(true);
            m_pOggCodec->StopDecode(true);
            m_bPlayingLeaveWord = FALSE;
        }
        else
        {
            bVoiceStop = true;
        }

        if (m_spSoundSegment)
        {
            m_spSoundSegment->type(Data::sstTeleRecord);
            m_spSoundSegment->played(true);
        }

        //	if (m_spContactInfo)
        //	{
        m_spContactInfo->type(Data::citInOk);
        //	}

        //m_bAutoRecord = FALSE;
        m_bPlayingLeaveWord = FALSE;
        ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pFSM->fireEvent(CMultimediaPhoneDlg::teHangOn, NULL);
    }

    void CTelephoneDlg::HangOnToRecord(void)
    {
        //Dprintf("Hang On To Record\n");
        ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->HungOnToRecord();	
        //Mute();

        if (waveInOpen(&m_hWaveIn,0,&m_waveform,0,NULL,NULL))
        {
            //Dprintf("waveInOpen error!\n");
            return ;
        }	
        waveInMessage(m_hWaveIn,WAV_LINEOUT_ONLY,0,0);//WAV_SPEAKER_LINEOUT//WAV_LINEOUT_ONLY
        waveInReset(m_hWaveIn);
        waveInClose(m_hWaveIn);
    }

    std::vector<Util::shared_ptr<Data::Contact> > CTelephoneDlg::FindCallContact(std::string number)
    {
        std::string filter;
        filter += "mobilesTelephone = '";
        filter += (number);
        filter += "'";
        filter += " OR mobilesTelephone2 = '";
        filter += (number);
        filter += "'";
        filter += " OR worksTelephone = '";
        filter += (number);
        filter += "'";
        filter += " OR worksTelephone2 = '";
        filter += (number);
        filter += "'";

        std::vector<Util::shared_ptr<Data::Contact> > result = Data::Contact::GetFromDatabase(filter);
        return result;
    }

    void CTelephoneDlg::ShowContact(Util::shared_ptr<Data::Contact> contact, std::string number)
    {
        m_spContactInfo->contactId(contact->id());
        m_spContactInfo->name(contact->name());
        CString str = CString(contact->name().c_str());
        str += _T("(");
        str += CString(number.c_str());
        str += _T(")");
        m_sticNameNo.SetWindowText(str);
        // 	m_sticNameNo.SetWindowText(CString(contact->name().c_str()));
        m_sticCompany.SetWindowText(CString(contact->company().c_str()));
        m_sticDepartment.SetWindowText(CString(contact->department().c_str()));
        m_sticDuty.SetWindowText(CString(contact->duty().c_str()));
    }

    void CTelephoneDlg::DialContact(Data::TelephoneNumber telephoneNumber, int contactId)
    {
        if ((((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pFSM->getCurrentState() != CMultimediaPhoneDlg::tsHangOff)
            && (((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pFSM->getCurrentState() != CMultimediaPhoneDlg::tsHangOn))
            return;
        if (!IsWindowVisible())
        {
            // 		TextStruct ts[1];
            // 		memset(ts, 0, sizeof(TextStruct) * 1);
            // 		
            // 		ts[0].txtRect = CRect(8, 0, 100, 20);
            // 		ts[0].txtFontSize = 16;
            // 		ts[0].sAlign = DT_LEFT | DT_BOTTOM;
            // 		memcpy(ts[0].sTxt, Data::LanguageResource::Get(Data::RI_TEL_NEWTELSTC).c_str(), Data::LanguageResource::Get(Data::RI_TEL_NEWTELSTC).length());
            // 		
            // 		m_sticBackground.SetTextStruct(ts, 1);

            // 		m_btnHandle.SetWindowText(m_strHangOn);
            // 		m_btnHandle.ShowWindow(TRUE);

            m_MJPGList.SetUnitIsShow(9, FALSE);
            m_MJPGList.SetUnitIsShow(3, TRUE);

            //ShowWindow_(TRUE);
            SetTimer(IDC_TELDLGSHOW, 10, NULL);
            ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->EnableLine(TRUE);
        }

        //Dprintf("DialContact:\n");
        //Dprintf(telephoneNumber.number().c_str());

        m_sTelephoneNumber = telephoneNumber.number();
        std::string number = m_sTelephoneNumber;

        if (((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pFSM->getCurrentState() == CMultimediaPhoneDlg::tsHangOff)
        {
            ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->phone_->HungOn();
            Sleep(1000);
        }


        if (((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pSetting->thisTelephoneNumber().number().length() >= 2)
        {
            std::string code = ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pSetting->thisTelephoneNumber().number();
            if (code[0] != '0')
            {
                code = "0" + code;
            }
            int codeLen = code.length();
            if (number.substr(0, codeLen) == code)
            {
                number = number.substr(codeLen);
            }
            else
            {
                if ((number.length() > 11) && (number.substr(0, 3) == "861"))
                {
                    number = number.substr(2);
                }
                std::string mobile = number;
                if (number[0] == '0')
                {
                    mobile = number.substr(1);
                }
                mobile = mobile.substr(0, 7);

                FILE* file = fopen("\\Flashdrv\\res_dat\\mobile.txt" , "rb");
                if (file != NULL)
                {
                    std::string addr = StartSearch(file, mobile, 0, 93935);
                    if (addr != "")
                    {
                        if (addr.substr(23, code.length()) == code)
                        {
                            if (number[0] == '0')
                            {
                                number = number.substr(1);
                            }
                        }
                        else
                        {
                            if (number[0] != '0')
                            {
                                number = "0" + number;
                            }
                        }
                        //city = addr.substr(8, 14);
                    }
                    fclose(file);
                }
            }
        }

        m_sDialNumber = number;

        if (((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pSetting->isAppendIpPrefix())
        {
            if (number[0] == '0')
            {
                m_sDialNumber = ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pSetting->ipPrefix() + number;
            }
        }

        if (((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pSetting->isAutoAppendOutlinePrefix())
        {
            m_sOutLine = ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pSetting->outlinePrefix();
            // 		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->phone_->DialNumber((char*)out.c_str());
            // 		Sleep(1500);
            if(m_sDialNumber.length() >= 5)
                m_sticNameNo.SetWindowText(CString(std::string(m_sOutLine + m_sDialNumber).c_str()));    //lxz 20081027
            else 
            {
                int nindex = m_sDialNumber.find('1');
                if(nindex == 0)
                    m_sticNameNo.SetWindowText(CString(std::string(m_sOutLine + m_sDialNumber).c_str()));    //lxz 20081027
            }
        }
        else
            m_sticNameNo.SetWindowText(CString(std::string(m_sDialNumber).c_str()));	//lxz 20081027
        SetTimer(4, 500, NULL);

        //((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->phone_->DialNumber((char*)number.c_str());

        //	if (!m_spContactInfo)
        if (m_spContactInfo->id() != 0)
        {
            m_spContactInfo->id(0);
            //		m_spContactInfo = Util::shared_ptr<Data::ContactInfo>(new Data::ContactInfo);
            m_spContactInfo->type(Data::citOut);
            m_spContactInfo->startTime(CTime::GetCurrentTime());
            m_spContactInfo->telephoneNumber(m_sTelephoneNumber);
        }

        //((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pFSM->setStartState(CMultimediaPhoneDlg::tsKey);
        //((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pFSM->fireEvent(CMultimediaPhoneDlg::teConnect, NULL);
    }

    UINT CTelephoneDlg::GetUnconnectCount(void)
    {
        std::vector<Util::shared_ptr<Data::ContactInfo> > result = Data::ContactInfo::GetFromDatabase("type = 0 AND played = 0", "id", Data::srNull, 0, 1);	
        m_uiInNoCount = result.size();
        return m_uiInNoCount;
    }
    // 
    // void CTelephoneDlg::ClearUnconnectCount(void)
    // {
    // 	m_uiInNoCount = 0;
    // 	::SendMessage(((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pMainDlg->GetSafeHwnd(), WM_TELNOTIFY, 0, 0);
    // }

    UINT CTelephoneDlg::GetRecordCount(void)
    {
        std::vector<Util::shared_ptr<Data::SoundSegment> > result = Data::SoundSegment::GetFromDatabase("type = 0 AND played = 0", "id", Data::srNull, 0, 1);	
        m_uiRecordCound = result.size();
        return m_uiRecordCound;
    }
    // 
    // void CTelephoneDlg::ClearRecordCount(void)
    // {
    // 	m_uiRecordCound = 0;
    // 	::SendMessage(((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pMainDlg->GetSafeHwnd(), WM_TELNOTIFY, 0, 0);
    // }

    void CTelephoneDlg::ShowWindow_(int cmdshow)
    {
        if(cmdshow > 0)
        {
            ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pMainDlg->SendMessage(WM_PLAYVIDEO, 0, 0);    //暂停视频

            ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pMainDlg->m_pWebDialog->SendMessage(WM_KILLWEBSHOW, 1, 0);
            ::SetWindowPos(m_hWnd, HWND_TOPMOST, 0, 0, 800, 480, 0);
            CCEDialog::ShowWindow_(cmdshow);

            ::PostMessage(((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pMainDlg->m_firewalDlg_->m_cmbTime.m_Combo.m_hWnd, CB_SHOWDROPDOWN,0,0);  //如果commbox打开，让commobox隐藏
            ::PostMessage(((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pContactGroupDlg->m_cmbSoundTip.m_Combo.m_hWnd, CB_SHOWDROPDOWN,0,0);

            ::PostMessage(((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pContactNewDlg->m_cmbType.m_Combo.m_hWnd, CB_SHOWDROPDOWN,0,0);
            ::PostMessage(((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pContactNewDlg->m_cmbGroup.m_Combo.m_hWnd, CB_SHOWDROPDOWN,0,0);
            ::PostMessage(((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pMainDlg->m_mainLunarderDlg_->m_cmbRing.m_Combo.m_hWnd, CB_SHOWDROPDOWN,0,0);
            ::PostMessage(((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pContactDlg->m_pSimImportDlg->m_cmbGroup.m_Combo.m_hWnd, CB_SHOWDROPDOWN,0,0);
            ::PostMessage(((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pMainDlg->m_pWebDialog->m_cmbURL.m_Combo.m_hWnd, CB_SHOWDROPDOWN,0,0);

            ::PostMessage(((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_cmbRing.m_Combo.m_hWnd, CB_SHOWDROPDOWN,0,0);
            ::PostMessage(((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_cmbRingTimes.m_Combo.m_hWnd, CB_SHOWDROPDOWN,0,0);
            ::PostMessage(((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_cmbAutoRecoedeTimes.m_Combo.m_hWnd, CB_SHOWDROPDOWN,0,0);
            ::PostMessage(((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_cmbWaitTime.m_Combo.m_hWnd, CB_SHOWDROPDOWN,0,0);
            ::PostMessage(((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_cmbSoundSavePath.m_Combo.m_hWnd, CB_SHOWDROPDOWN,0,0);

            ::PostMessage(((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_cmbRingVolume.m_Combo.m_hWnd, CB_SHOWDROPDOWN,0,0);
            ::PostMessage(((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_cmbSystemVolume.m_Combo.m_hWnd, CB_SHOWDROPDOWN,0,0);
            ::PostMessage(((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_cmbContrlBlackLightWaitTime.m_Combo.m_hWnd, CB_SHOWDROPDOWN,0,0);
        }
        else
        {
            CCEDialog::ShowWindow_(cmdshow);
            ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pMainDlg->m_pWebDialog->SendMessage(WM_KILLWEBSHOW, 0, 0);

            //		((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pMainDlg->SendMessage(WM_PLAYVIDEO, 1, 0);			//恢复视频
            //		((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pMainDlg->m_mainMp3Dlg_->SendMessage(WM_OUTEVENT, 0, 1);
        }
    }

    void CTelephoneDlg::OnTimer(UINT nIDEvent) 
    {
        // TODO: Add your message handler code here and/or call default
        if (nIDEvent == m_uiTelephoneTimer)
        {
            ++m_uiTelephoneSecond;
            CTimeSpan ts(m_uiTelephoneSecond);
            CString duration;
            if (m_uiTelephoneSecond > 3600)
            {
                duration.Format(_T("%d:%02d:%02d"), ts.GetHours(), ts.GetMinutes(),ts.GetSeconds());
            }
            else
            {
                duration.Format(_T("%02d:%02d"), ts.GetMinutes(),ts.GetSeconds());
            }
            m_sticDuration.SetWindowText(duration);
        }
        else if (nIDEvent == m_uiRecordTimer)
        {
            //	std::string path = ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pSetting->soundPath();

            extern BOOL DetectDIR(TCHAR *sDir);
            if (path == ssStorageCardRecordPath)
            {
                if( !DetectDIR(_T("/storagecard")))
                {
                    OnButtonTelephoneRecord();
                    m_sticRecord.SetWindowText(L"SD卡不存在");
                    m_sticRecord.ShowWindow(TRUE);
                    CCEDialog::OnTimer(nIDEvent);
                    return;
                }
            }

            ++m_uiRecordSecond;
            CTimeSpan ts(m_uiRecordSecond);
            CString duration;
            if (m_uiRecordSecond > 3600)
            {
                duration.Format(_T("%d:%02d:%02d"), ts.GetHours(), ts.GetMinutes(),ts.GetSeconds());
            }
            else
            {
                duration.Format(_T("%02d:%02d"), ts.GetMinutes(),ts.GetSeconds());
            }
            m_sticRecord.SetWindowText(duration);

            if (m_uiRecordSecond >= m_uiRemainRecordSecond)
            {
                OnButtonTelephoneRecord();
            }
        }
        else if (nIDEvent == m_uiTipTimer)
        {
            KillTimer(m_uiTipTimer);
            m_bPlayingLeaveWord = FALSE;
            ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->StopRing(true);
            //		m_pOggCodec->StopDecode(TRUE);
            //		MessageBeep(MB_OK);

            if (waveInOpen(&m_hWaveIn,0,&m_waveform,0,NULL,NULL))
            {
                //Dprintf("waveInOpen error!\n");
                return ;
            }	
            waveInMessage(m_hWaveIn,WAV_LINEOUT_ONLY,0,0);//WAV_SPEAKER_LINEOUT//WAV_LINEOUT_ONLY
            waveInReset(m_hWaveIn);
            waveInClose(m_hWaveIn);

            ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->StartRing(_T("\\FlashDrv\\tip2.wav"), 1);
            OnButtonTelephoneRecord();
            Sleep(1000);
            ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->StopRing();

            bVoiceStop = false;
            //Dprintf("Voice Check start\n");
            hCheckThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)VoiceCheckThreadPrc, NULL, 0, NULL);
            if (hCheckThread == INVALID_HANDLE_VALUE)
            {
                //Dprintf("Create Check Thread error!\n");
                bVoiceStop = true;
            }
            int nTimers = ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pSetting->maxSoundUseSize();
            int nTimers_[] = {15, 30, 60, 90};
            SetTimer(IDT_STOPAUTORECORDE_TIME, (nTimers_[nTimers] + 1)*1000, NULL);
        }
        else if(nIDEvent == IDT_STOPAUTORECORDE_TIME)		//停止自动录音留言
        {
            KillTimer(IDT_STOPAUTORECORDE_TIME);
            ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->HungOff();	
        }
        else if(nIDEvent == IDC_TELDLGSHOW)
        {
            KillTimer(IDC_TELDLGSHOW);
            //Dprintf("TelDlg show\n");
            if(m_bRing || ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->GetPhoneHungOn())
            {
                ShowWindow_(TRUE);
            }
            else
            {
                //Dprintf("hide1111\r\n");
                ShowWindow_(FALSE);    //Mp3被挂起,不能继续播放
            }
        }
        else if(nIDEvent == IDC_TELDLGHIDE)
        {
            //Dprintf("TelDlg hide3\n");
            m_bRing = FALSE;
            KillTimer(IDC_TELDLGHIDE);
            ShowWindow_(FALSE);
        }
        else if(nIDEvent == 4)
        {
            KillTimer(4);
            memset(m_chDialNumber, 0, 64);
            if (((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pSetting->isAutoAppendOutlinePrefix())
            {

                if((m_sDialNumber.length() >= 5))
                {
                    m_sOutLine = ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pSetting->outlinePrefix();
                    //((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->phone_->DialNumber((char*)m_sOutLine.c_str());
                    strcpy(m_chDialNumber, (char*)m_sOutLine.c_str());
                    strcat(m_chDialNumber, ",");
                    //Sleep(2000);
                }
                else 
                {
                    int nindex = m_sDialNumber.find('1');
                    if( nindex == 0)
                    {
                        m_sOutLine = ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pSetting->outlinePrefix();
                        strcpy(m_chDialNumber, (char*)m_sOutLine.c_str());
                        strcat(m_chDialNumber, ",");
                        //((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->phone_->DialNumber((char*)m_sOutLine.c_str());
                        //Sleep(2000);
                    }
                }

                // 			m_sOutLine = ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pSetting->outlinePrefix();
                // 			((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->phone_->DialNumber((char*)m_sOutLine.c_str());
                // 			Sleep(1500);
            }
            memcpy(m_chDialNumber+strlen(m_chDialNumber), (char*)m_sDialNumber.c_str(), strlen((char*)m_sDialNumber.c_str()));
            ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->phone_->DialNumber(m_chDialNumber);
            //((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->phone_->DialNumber((char*)m_sDialNumber.c_str());
            ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pFSM->setStartState(CMultimediaPhoneDlg::tsKey);
            //	Sleep(4000);   //lxz20090204
            ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pFSM->fireEvent(CMultimediaPhoneDlg::teConnect, NULL);
        }

        if (View::_test_call)
        {
            if (nIDEvent == 10)
            {
                KillTimer(10);
                HandleOff();
            }
        }

        CCEDialog::OnTimer(nIDEvent);
    }

    bool CTelephoneDlg::RecStart(void)
    {
        // TODO: Add your control notification handler code here

        // 	SaveWavHead();
        // 	
        // 	if (m_fWav.Open(LPCTSTR(Util::StringOp::ToCString(m_sRecordFileName)), CFile::modeWrite) == 0)
        // 		return false;
        // 	
        // 	m_fWav.SeekToEnd();
        // 	Dprintf("SaveWavHead\n");

        /////////////////////////////////////////////////////////////////////////////////////////////////////
        LRESULT l = waveInOpen(&m_hWaveIn,WAVE_MAPPER,&m_waveform,(DWORD)this->m_hWnd,NULL,CALLBACK_WINDOW);
        if (l)
        {
            //Dprintf("waveInOpen error\r\n");
            //		m_fWav.Close();
            return false;
        }
        waveInMessage(m_hWaveIn,WAV_LINEIN_MIC,0,0);


        // 	m_pWaveHdr1->lpData=(char*)m_pBuffer1;
        // 	m_pWaveHdr1->dwBufferLength=INP_BUFFER_SIZE;
        // 	m_pWaveHdr1->dwBytesRecorded=0;
        // 	m_pWaveHdr1->dwUser=0;
        // 	m_pWaveHdr1->dwFlags=0;
        // 	m_pWaveHdr1->dwLoops=1;
        // 	m_pWaveHdr1->lpNext=NULL;
        // 	m_pWaveHdr1->reserved=0;
        // 	
        // 	MMRESULT r = waveInPrepareHeader(m_hWaveIn,m_pWaveHdr1,sizeof(WAVEHDR));
        // 	
        // 	m_pWaveHdr2->lpData=(char*)m_pBuffer2;
        // 	m_pWaveHdr2->dwBufferLength=INP_BUFFER_SIZE;
        // 	m_pWaveHdr2->dwBytesRecorded=0;
        // 	m_pWaveHdr2->dwUser=0;
        // 	m_pWaveHdr2->dwFlags=0;
        // 	m_pWaveHdr2->dwLoops=1;
        // 	m_pWaveHdr2->lpNext=NULL;
        // 	m_pWaveHdr2->reserved=0;
        // 	
        // 	r = waveInPrepareHeader(m_hWaveIn,m_pWaveHdr2,sizeof(WAVEHDR));
        // 		
        // 	waveInAddBuffer (m_hWaveIn, m_pWaveHdr1, sizeof (WAVEHDR)) ;
        // 	waveInAddBuffer (m_hWaveIn, m_pWaveHdr2, sizeof (WAVEHDR)) ;

        for (int i = 0; i < WAVE_BUFFER_COUNT; ++i)
        {
            m_pWaveHdr[i]->lpData=(char*)m_pBuffer[i];
            m_pWaveHdr[i]->dwBufferLength=INP_BUFFER_SIZE;
            m_pWaveHdr[i]->dwBytesRecorded=0;
            m_pWaveHdr[i]->dwUser=0;
            m_pWaveHdr[i]->dwFlags=0;
            m_pWaveHdr[i]->dwLoops=1;
            m_pWaveHdr[i]->lpNext=NULL;
            m_pWaveHdr[i]->reserved=0;

            waveInPrepareHeader(m_hWaveIn,m_pWaveHdr[i],sizeof(WAVEHDR));
            waveInAddBuffer (m_hWaveIn, m_pWaveHdr[i], sizeof (WAVEHDR));
        }

        // Begin sampling

        m_bEnding = FALSE ;
        m_bRecording = TRUE;
        m_dwDataLength = 0 ;
        waveInStart (m_hWaveIn) ;

        //Dprintf("RecStart\n");
        return true;
    }

    bool CTelephoneDlg::RecStop(void)
    {
        m_bEnding=TRUE;		
        waveInReset(m_hWaveIn);
        ///waveInClose(m_hWaveIn);

        return true;
    }

    void CTelephoneDlg::SaveWavHead(void)
    {
        if (m_sRecordFileName == "")
        {
            return;
        }

#pragma pack(2)

        struct wave_header
        {
            char riff[4];             /* "RIFF" */
            unsigned long size;       /* length of rest of file = size of rest of header(36) + data length */
            char wave[4];             /* "WAVE" */
            char fmt[4];              /* "fmt " */
            unsigned long fmt_len;    /* length of rest of fmt chunk = 16 */
            unsigned short tag;       /* MS PCM = 1 */
            unsigned short channels;  /* channels, mono = 1, stereo = 2 */
            unsigned long samp_rate;  /* samples per second = 44100 */
            unsigned long byte_rate;  /* bytes per second = samp_rate * byte_samp = 176400 */
            unsigned short byte_samp; /* block align (bytes per sample) = channels * bits_per_sample / 8 = 4 */
            unsigned short bit_samp;  /* bits per sample = 16 for MS PCM (format specific) */
            unsigned short res0;
            char data[4];             /* "data" */
            unsigned long length;     /* data length (bytes) */
        } header;

        DWORD temp = RIFF_FILE;
        memcpy(&header.riff, &temp, 4);
        header.size = 36 + m_dwDataLength;
        temp = RIFF_WAVE;
        memcpy(&header.wave, &temp, 4);
        temp = RIFF_FORMAT;
        memcpy(&header.fmt, &temp, 4);
        header.fmt_len = sizeof(WAVEFORMATEX);
        memcpy(&header.tag, &m_waveform, sizeof(WAVEFORMATEX));
        temp = RIFF_DATA;
        memcpy(&header.data, &temp, 4);
        header.length = m_dwDataLength;

        CFileStatus   status;
        if (CFile::GetStatus(LPCTSTR(Util::StringOp::ToCString(m_sRecordFileName)),status))
        {
            m_fWav.Open(LPCTSTR(Util::StringOp::ToCString(m_sRecordFileName)), CFile::modeWrite);
        }
        else
        {
            m_fWav.Open(LPCTSTR(Util::StringOp::ToCString(m_sRecordFileName)), CFile::modeCreate | CFile::modeWrite);
        }

        m_fWav.SeekToBegin();
        m_fWav.Write(&header, sizeof(wave_header));
        m_fWav.Close();
    }

    LRESULT CTelephoneDlg::OnMM_WIM_OPEN(UINT wParam, LONG lParam) 
    {
        LRESULT result = 0;
        // TODO: Add your message handler code here and/or call default
        //Dprintf("OnMM_WIM_OPEN\n");
        //	PlayStart();
        return result;
    }

    LRESULT CTelephoneDlg::OnMM_WIM_DATA(UINT wParam, LONG lParam) 
    {
        LRESULT result = 0;
        //Dprintf("OnMM_WIM_DATA\n");
        //	Dprintf("OnMM_WIM_DATA = %d\n", ((PWAVEHDR) lParam)->dwBytesRecorded);

        //  m_fWav.Write(((PWAVEHDR) lParam)->lpData, ((PWAVEHDR) lParam)->dwBytesRecorded);
        // 	m_dwDataLength += ((PWAVEHDR) lParam)->dwBytesRecorded;

        OggCodec::PcmData * pd = new OggCodec::PcmData;
        pd->dataLength = ((PWAVEHDR) lParam)->dwBytesRecorded / sizeof(short);
        pd->data = (short*)malloc(((PWAVEHDR) lParam)->dwBytesRecorded);
        memcpy(pd->data , ((PWAVEHDR) lParam)->lpData, ((PWAVEHDR) lParam)->dwBytesRecorded);
        m_pOggCodec->PushPcmData(pd);

        // 	char * d = (char*)malloc(((PWAVEHDR) lParam)->dwBytesRecorded);
        // 	memcpy(d , ((PWAVEHDR) lParam)->lpData, ((PWAVEHDR) lParam)->dwBytesRecorded);
        // 	
        // 	PWAVEHDR wh = reinterpret_cast<PWAVEHDR>(malloc(sizeof(WAVEHDR)));
        // 	wh->lpData          = d;
        // 	wh->dwBufferLength  = ((PWAVEHDR) lParam)->dwBytesRecorded / sizeof(short); ;
        // 	wh->dwBytesRecorded = 0 ;
        // 	wh->dwUser          = 0 ;
        // 	wh->dwFlags         = WHDR_BEGINLOOP | WHDR_ENDLOOP ;
        // 	wh->dwLoops         = 1 ;
        // 	wh->lpNext          = NULL ;
        // 	wh->reserved        = 0 ;
        // 	
        // 	// Prepare and write
        // 	
        // 	waveOutPrepareHeader (m_hWaveOut, wh, sizeof (WAVEHDR)) ;
        // 	waveOutWrite (m_hWaveOut, wh, sizeof (WAVEHDR)) ;

        if (!bVoiceStop)
        {
            OggCodec::PcmData * pd = new OggCodec::PcmData;
            pd->dataLength = ((PWAVEHDR) lParam)->dwBytesRecorded / sizeof(short);
            pd->data = (short*)malloc(((PWAVEHDR) lParam)->dwBytesRecorded);
            memcpy(pd->data , ((PWAVEHDR) lParam)->lpData, ((PWAVEHDR) lParam)->dwBytesRecorded);
            CheckQueue.push(pd);
        }

        if (m_bEnding)
        {
            //Dprintf("waveInClose\n");
            //waveInReset(m_hWaveIn);
            waveInClose(m_hWaveIn);
            return result;
        }

        // Send out a new buffer	
        waveInAddBuffer (m_hWaveIn, (PWAVEHDR) lParam, sizeof (WAVEHDR));
        return result;
    }

    LRESULT CTelephoneDlg::OnMM_WIM_CLOSE(UINT wParam, LONG lParam) 
    {
        LRESULT result = 0;
        // TODO: Add your message handler code here and/or call default
        //Dprintf("OnMM_WIM_CLOSE\n");

        //	waveInUnprepareHeader (m_hWaveIn, m_pWaveHdr1, sizeof (WAVEHDR)) ;
        //	waveInUnprepareHeader (m_hWaveIn, m_pWaveHdr2, sizeof (WAVEHDR)) ;
        for (int i = 0; i < WAVE_BUFFER_COUNT; ++i)
        {
            waveInUnprepareHeader (m_hWaveIn, m_pWaveHdr[i], sizeof (WAVEHDR));
        }

        // 	m_fWav.Close();
        // 	SaveWavHead();

        bVoiceStop = true;
        m_pOggCodec->StopEncode(true);
        m_bRecording = FALSE;

        //	KillTimer(m_uiRecordTimer);
        //m_btnRecordTip.ShowWindow(FALSE);
        m_sticRecordCaption.ShowWindow(FALSE);
        m_sticRecord.ShowWindow(FALSE);
        m_sticRecord.SetWindowText(_T(""));

        m_MJPGList.SetUnitIsShow(8, FALSE);
        m_MJPGList.SetUnitIsShow(2, TRUE);

        //	m_btnRecord.SetWindowText(m_strRecord);

        // 			if (m_uiRecordSecond == 0)
        // 			{
        // 				m_uiRecordSecond = 1;
        // 			}
        // 	m_spSoundSegment->duration(Util::TimeSpan(m_uiRecordSecond));
        // 	m_spSoundSegment = Util::shared_ptr<Data::SoundSegment>();
        // 	m_uiRecordSecond = 0;

        if (m_uiRecordSecond <= 1)
        {
            CFile::Remove(Util::StringOp::ToCString(m_spSoundSegment->filename()));
        }
        Sleep(500);

        //	PlayStop();
        return result;
    }

    bool CTelephoneDlg::PlayStart(void)
    {
        if (m_bPlaying)
        {
            waveOutReset(m_hWaveOut);
        }
        m_waveform.wFormatTag=WAVE_FORMAT_PCM;
        m_waveform.nChannels=CHANNELS;
        m_waveform.nSamplesPerSec=SAMPLESPERSEC;
        m_waveform.wBitsPerSample=BITSPERSAMPLE;
        m_waveform.nBlockAlign=m_waveform.nChannels * m_waveform.wBitsPerSample / 8;
        m_waveform.nAvgBytesPerSec=m_waveform.nBlockAlign * m_waveform.nSamplesPerSec;
        m_waveform.cbSize=0;	

        if (waveOutOpen(&m_hWaveOut,WAVE_MAPPER,&m_waveform,(DWORD)this->m_hWnd,NULL,CALLBACK_WINDOW))
        {
            return false;
        }

        waveOutMessage(m_hWaveOut,WAV_SPEAKER_LINEOUT,0,0);
        return true;
    }

    bool CTelephoneDlg::PlayStop(void)
    {
        if (!m_bPlaying)
        {
            return true;
        }
        waveOutReset(m_hWaveOut);
        waveOutClose(m_hWaveOut);
        return true;
    }

    LRESULT CTelephoneDlg::OnMM_WOM_OPEN(UINT wParam, LONG lParam)
    {
        LRESULT result = 0;
        //Dprintf("OnMM_WOM_OPEN\n");	
        m_bPlaying = TRUE ;
        return result;
    }

    LRESULT CTelephoneDlg::OnMM_WOM_DONE(UINT wParam, LONG lParam)
    {
        LRESULT result = 0;
        //Dprintf("OnMM_WOM_DONE\n");

        waveOutUnprepareHeader (m_hWaveOut, (PWAVEHDR) lParam, sizeof (WAVEHDR));
        free(((PWAVEHDR) lParam)->lpData);
        free((PWAVEHDR) lParam);

        // 	waveOutUnprepareHeader (m_hWaveOut, m_pWaveHdr1, sizeof (WAVEHDR)) ;
        // 	waveOutClose (m_hWaveOut) ;
        // 	m_bPlaying = FALSE ;	
        // 	if (m_pOutBuffer != NULL)
        // 	{
        // 		free(m_pOutBuffer);
        // 		m_pOutBuffer = NULL;
        // 	}
        return result;
    }

    LRESULT CTelephoneDlg::OnMM_WOM_CLOSE(UINT wParam, LONG lParam)
    {
        LRESULT result = 0;
        //Dprintf("OnMM_WOM_CLOSE\n");
        m_bPlaying = FALSE ;	
        // 	if (m_pOutBuffer != NULL)
        // 	{
        // 		free(m_pOutBuffer);
        // 		m_pOutBuffer = NULL;
        // 	}
        return result;
    }
}
