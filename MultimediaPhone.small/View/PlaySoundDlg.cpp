// playsounddlg.cpp : implementation file
//

#include "stdafx.h"
#include "PlaySoundDlg.h"
#include "ContactInfoDlg.h"
#include "SoundDlg.h"
#include "../MultimediaPhone.h"
#include "../MultimediaPhoneDlg.h"
#include "../Data/LanguageResource.h"
#include "../Data/SkinStyle.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern BOOL DetectFile(TCHAR *sDir);

namespace View {

    CPlaySoundDlg::CPlaySoundDlg(CWnd* pParent /*=NULL*/)
        : CCEDialog(CPlaySoundDlg::IDD, pParent)
    {
        //{{AFX_DATA_INIT(CPlaySoundDlg)
        // NOTE: the ClassWizard will add member initialization here
        //}}AFX_DATA_INIT
    }


    void CPlaySoundDlg::DoDataExchange(CDataExchange* pDX)
    {
        CDialog::DoDataExchange(pDX);
        //{{AFX_DATA_MAP(CPlaySoundDlg)
        // NOTE: the ClassWizard will add DDX and DDV calls here
        //}}AFX_DATA_MAP
    }


    BEGIN_MESSAGE_MAP(CPlaySoundDlg, CCEDialog)
        //{{AFX_MSG_MAP(CPlaySoundDlg)
        ON_WM_TIMER()
        //}}AFX_MSG_MAP
        ON_BN_CLICKED(IDC_BUTTON_SOUNDDLG_CLOSE, OnButtonClose)
        ON_BN_CLICKED(IDC_BUTTON_SOUNDDLG_NEXT, OnButtonNext)
        ON_BN_CLICKED(IDC_BUTTON_SOUNDDLG_PREV, OnButtonPrev)
        ON_BN_CLICKED(IDC_BUTTON_SOUNDDLG_PLAY, OnButtonPlay)
        ON_BN_CLICKED(IDC_CHECK_SOUNDDLG_PLAYCONTINUE, OnCheckPlaycontinue)
        ON_MESSAGE(WM_PROCESS_POS, OnPlaySeek)
        ON_MESSAGE(WM_CLICKMJPG_TOAPP, OnClickMJPG)
    END_MESSAGE_MAP()

    /////////////////////////////////////////////////////////////////////////////
    // CPlaySoundDlg message handlers
    LRESULT CPlaySoundDlg::OnClickMJPG(WPARAM w, LPARAM l)
    {
        LRESULT result = 0;
        CMultimediaPhoneDlg *pWnd = (CMultimediaPhoneDlg *)theApp.m_pMainWnd;
        switch(w)
        {
        case 1:
            OnButtonClose();
            pWnd->m_pMainDlg->m_mainMp3Dlg_->SendMessage(WM_OUTEVENT, 0, 1);
            break;
        case 2:
            OnButtonPrev();
            break;
        case 3:
            OnButtonNext();
            break;
        case 4:
        case 5:
            pWnd->m_pMainDlg->m_mainMp3Dlg_->OnTimer(1002); //SendMessage(WM_OUTEVENT, 0, 0);
            OnButtonPlay();
            break;		
        }
        return result;
    }
    BOOL CPlaySoundDlg::OnInitDialog() 
    {
        CDialog::OnInitDialog();
        // TODO: Add extra initialization here
        //CenterWindow(GetDesktopWindow());	// center to the hpc screen

        std::string strTemp;
        CString str;

        m_sticNameNO.Create(str, WS_CHILD|WS_VISIBLE, CRect(48 + 54, 70 + 62, 526 + 54, 100 + 62), this);
        m_sticNameNO.SetColor(RGB(0, 0, 0), RGB(203, 203, 203));//Data::g_partFrameInRectBackRGB[Data::g_skinstyle]);

        m_sticSoundName.Create(str, WS_CHILD|WS_VISIBLE, CRect(48 + 54, 106 + 62, 280 + 54 -8, 136 + 62), this);
        m_sticSoundName.SetColor(RGB(0, 0, 0), RGB(203, 203, 203));//Data::g_partFrameInRectBackRGB[Data::g_skinstyle]);

        m_sticSoundDuration.Create(str, WS_CHILD|WS_VISIBLE, CRect(280 + 54 - 8, 106 + 62, 526 + 54, 136 + 62), this);
        m_sticSoundDuration.SetColor(RGB(0, 0, 0), RGB(203, 203, 203));//Data::g_partFrameInRectBackRGB[Data::g_skinstyle]);
        m_sticSoundDuration.SetAlign(SS_RIGHT);	

        m_procbarSound.Create(WS_CHILD|WS_VISIBLE, CRect(55 + 54, 153 + 62, 520 + 54, 180 + 62), this, IDC_PBAR_SOUNDDLG_PROCESS);
        m_procbarSound.SetParam(0, 0, 50, 1);
        m_procbarSound.SetPos(0);

        strTemp = Data::LanguageResource::Get(Data::RI_SOUND_PLAY_CONTINUE);
        str = strTemp.c_str();
        str = _T("");
        m_chkPlayContinue.Create(str, WS_CHILD|WS_VISIBLE|BS_CHECKBOX, CRect(48 + 54, 216 + 62, 88 + 54, 253 + 62), this, IDC_CHECK_SOUNDDLG_PLAYCONTINUE);
        m_chkPlayContinue.SetIcon(IDI_ICON_CHECK1, CSize(32, 32), IDI_ICON_CHECK0, CSize(32, 32));
        m_chkPlayContinue.SetColor(CCEButtonST::BTNST_COLOR_BK_IN, RGB(203, 203, 203));//Data::g_partFrameInRectBackRGB[Data::g_skinstyle]);
        m_chkPlayContinue.SetColor(CCEButtonST::BTNST_COLOR_BK_OUT, RGB(203, 203, 203));//Data::g_partFrameInRectBackRGB[Data::g_skinstyle]);
        m_chkPlayContinue.SetColor(CCEButtonST::BTNST_COLOR_BK_FOCUS, RGB(203, 203, 203));//Data::g_partFrameInRectBackRGB[Data::g_skinstyle]);
        m_chkPlayContinue.SetCheck(BST_CHECKED);
        /*
        strTemp = Data::LanguageResource::Get(Data::RI_SOUND_PLAY_CLOSE);
        str = strTemp.c_str();
        m_btnClose.Create(str, Data::g_buttonArcBMPPARTDILOAGID[0][Data::g_skinstyle], Data::g_buttonArcBMPPARTDILOAGID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(378, 78, 434, 98), this, IDC_BUTTON_SOUNDDLG_CLOSE);
        m_btnClose.SetBackRGB(Data::g_partFrameMainBackRGB[Data::g_skinstyle]);

        strTemp = Data::LanguageResource::Get(Data::RI_SOUND_PLAY_PREV);
        str = strTemp.c_str();
        m_btnPrev.Create(str, Data::g_buttonArcBMPPARTDILOAGID[0][Data::g_skinstyle], Data::g_buttonArcBMPPARTDILOAGID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(378, 103, 434, 123), this, IDC_BUTTON_SOUNDDLG_PREV);
        m_btnPrev.SetBackRGB(Data::g_partFrameMainBackRGB[Data::g_skinstyle]);

        strTemp = Data::LanguageResource::Get(Data::RI_SOUND_PLAY_TEXT);
        str = strTemp.c_str();
        m_btnNext.Create(str, Data::g_buttonArcBMPPARTDILOAGID[0][Data::g_skinstyle], Data::g_buttonArcBMPPARTDILOAGID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(378, 128, 434, 148), this, IDC_BUTTON_SOUNDDLG_NEXT);
        m_btnNext.SetBackRGB(Data::g_partFrameMainBackRGB[Data::g_skinstyle]);

        strTemp = Data::LanguageResource::Get(Data::RI_SOUND_PLAY_PLAY);
        str = strTemp.c_str();
        m_btnPlay.Create(str, Data::g_buttonArcBMPPARTDILOAGID[0][Data::g_skinstyle], Data::g_buttonArcBMPPARTDILOAGID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(378, 153, 434, 173), this, IDC_BUTTON_SOUNDDLG_PLAY);
        m_btnPlay.SetBackRGB(Data::g_partFrameMainBackRGB[Data::g_skinstyle]);

        m_sticBackground.Create(CRect(34, 40, 448, 184), this, 1);

        TextStruct ts[1];
        memset(ts, 0, sizeof(TextStruct) * 1);

        ts[0].txtRect = CRect(8, 0, 100, 20);
        ts[0].txtFontSize = 16;
        ts[0].sAlign = DT_LEFT | DT_BOTTOM;
        memcpy(ts[0].sTxt, Data::LanguageResource::Get(Data::RI_SOUND_PLAY_TITLE).c_str(), Data::LanguageResource::Get(Data::RI_SOUND_PLAY_TITLE).length());

        m_sticBackground.SetTextStruct(ts, 1);
        */
        m_MJPGList.Create(L"", WS_VISIBLE|WS_CHILD, CRect(54, 62, 746, 358), this);
        m_MJPGList.SetCurrentLinkFile(_T(".\\adv\\mjpg\\k1\\中文\\听录音.xml"));
        m_MJPGList.SetMJPGRect(CRect(54, 62, 746, 358));

        m_MJPGList.SetUnitIsShow(0, TRUE);
        m_MJPGList.SetUnitIsShow(1, TRUE);
        m_MJPGList.SetUnitIsShow(2, TRUE);
        m_MJPGList.SetUnitIsShow(3, TRUE);
        m_MJPGList.SetUnitIsShow(4, FALSE);

        m_iCurrentSound = 0;
        m_iModel = 0;

        m_iCurrentID = 0;
        m_iTotalSeconds = 0;
        m_iCurrentSecond = 0;
        m_bHasNext = false;
        m_bPlaying = false;
        m_bPlayPause = false;
        m_bOnlyNew  = false;
        m_pOggCodec = new OggCodec();

        return TRUE;  // return TRUE unless you set the focus to a control
        // EXCEPTION: OCX Property Pages should return FALSE
    }

    //void CPlaySoundDlg::SetSound(int listID, int offset)
    BOOL CPlaySoundDlg::SetSound(int type, int id, int dir, std::string searchFilter)
    {
        BOOL ret = FALSE;
        m_iType = type;
        m_sSearchFilter = searchFilter;
        if (m_iModel == 0)
        {
            std::string filter;
            //		if (type == 4)
            if (type == 3)
            {
                filter = searchFilter;
            }
            else
            {
                filter = "type = ";
                filter += Util::StringOp::FromInt(type);
                if (type == 0 && m_bOnlyNew)
                {
                    filter += " AND played = 0";
                }
            }
            std::vector<Util::shared_ptr<Data::SoundSegment> > result;
            if (dir == 0)
            {
                filter = "id = ";
                filter += Util::StringOp::FromInt(id);
                result = Data::SoundSegment::GetFromDatabase(filter);
                if (!result.empty())
                {			
                    m_iCurrentID = result[0]->id();
                    //m_sticTitle;
                    std::string nameNo;
                    if (result[0]->name() != "")
                    {
                        nameNo = result[0]->name();
                        nameNo = nameNo + "(" + result[0]->telephoneNumber().number() + ")";
                    }
                    else
                    {
                        nameNo = result[0]->telephoneNumber().number();
                    }
                    m_sticNameNO.SetWindowText(Util::StringOp::ToCString(nameNo));

                    m_sFilePath = result[0]->filename();
                    m_iTotalSeconds = result[0]->duration();
                    m_procbarSound.SetParam(0, 0, m_iTotalSeconds, 1);
                    std::string sFile = result[0]->filename();
                    CString sTemp = Util::StringOp::ToCString(sFile);				
                    if (::DetectFile(sTemp.GetBuffer(256)))
                    {
                        m_bHasNext = true;
                        ret = TRUE;
                        m_sticSoundName.SetWindowText(Util::StringOp::ToCString(result[0]->filename().substr(result[0]->filename().length() - 10)));
                    }
                    else
                    {
                        m_sticSoundName.SetWindowText(L"文件不存在");
                        m_sticSoundDuration.SetWindowText(_T(""));
                    }
                }
            }
            else
            {
                while (true)
                {
                    result = Data::SoundSegment::GetFromDatabase(filter, "id", Data::SeqenceRelation(dir), id, 1);
                    if (!result.empty())
                    {			
                        m_iCurrentID = result[0]->id();
                        id = m_iCurrentID;
                        //m_sticTitle;
                        std::string nameNo;
                        if (result[0]->name() != "")
                        {
                            nameNo = result[0]->name();
                            nameNo = nameNo + "(" + result[0]->telephoneNumber().number() + ")";
                        }
                        else
                        {
                            nameNo = result[0]->telephoneNumber().number();
                        }
                        m_sticNameNO.SetWindowText(Util::StringOp::ToCString(nameNo));

                        m_sFilePath = result[0]->filename();
                        m_iTotalSeconds = result[0]->duration();
                        m_procbarSound.SetParam(0, 0, m_iTotalSeconds, 1);
                        std::string sFile = result[0]->filename();
                        CString sTemp = Util::StringOp::ToCString(sFile);
                        if (::DetectFile(sTemp.GetBuffer(256)))
                        {
                            m_bHasNext = true;
                            ret = TRUE;
                            m_sticSoundName.SetWindowText(Util::StringOp::ToCString(result[0]->filename().substr(result[0]->filename().length() - 10)));
                            break;
                        }
                        else
                        {
                            m_sticSoundName.SetWindowText(L"文件不存在");
                            m_sticSoundDuration.SetWindowText(_T(""));
                        }
                    }
                    else
                    {
                        break;
                    }
                }

            }
        }
        else if (m_iModel == 1)
        {		
            std::string filter;
            if (type == 3)
            {
                filter = searchFilter;
            }
            else
            {
                filter = "type = ";
                filter += Util::StringOp::FromInt(type);
            }

            std::vector<Util::shared_ptr<Data::ContactInfo> > result;
            if (dir == 0)
            {
                filter = "id = ";
                filter += Util::StringOp::FromInt(id);
                result = Data::ContactInfo::GetFromDatabase(filter);
                if (!result.empty())
                {
                    int soundCount = result[0]->sounds().size();
                    if (soundCount > 0)
                    {
                        m_iCurrentID = result[0]->id();
                        filter = "contactInfoId = ";
                        filter += Util::StringOp::FromInt(result[0]->id());

                        m_vSoundResult = Data::SoundSegment::GetFromDatabase(filter);
                        if (!m_vSoundResult.empty())
                        {
                            m_iCurrentSound = m_vSoundResult.size() - 1;
                            std::string nameNo;
                            if (m_vSoundResult[0]->name() != "")
                            {
                                nameNo = m_vSoundResult[0]->name();
                                nameNo = nameNo + "(" + m_vSoundResult[0]->telephoneNumber().number() + ")";
                            }
                            else
                            {
                                nameNo = m_vSoundResult[0]->telephoneNumber().number();
                            }
                            m_sticNameNO.SetWindowText(Util::StringOp::ToCString(nameNo));
                            m_sFilePath = m_vSoundResult[m_iCurrentSound]->filename();
                            m_iTotalSeconds = m_vSoundResult[m_iCurrentSound]->duration();
                            m_procbarSound.SetParam(0, 0, m_iTotalSeconds, 1);						
                            std::string sFile = m_vSoundResult[m_iCurrentSound]->filename();
                            CString sTemp = Util::StringOp::ToCString(sFile);
                            if (::DetectFile(sTemp.GetBuffer(256)))
                            {
                                m_bHasNext = true;
                                ret = TRUE;
                                m_sticSoundName.SetWindowText(Util::StringOp::ToCString(m_vSoundResult[m_iCurrentSound]->filename().substr(m_vSoundResult[m_iCurrentSound]->filename().length() - 10)));
                            }
                            else
                            {
                                m_sticSoundName.SetWindowText(L"文件不存在");
                                m_sticSoundDuration.SetWindowText(_T(""));
                            }
                        }
                    }
                }
            }
            else
            {
                filter += " AND isSound = 1";

                while (true)
                {
                    result = Data::ContactInfo::GetFromDatabase(filter, "id", Data::SeqenceRelation(dir), id, 1);
                    if (!result.empty())
                    {
                        id = result[0]->id();
                        int soundCount = result[0]->sounds().size();
                        if (soundCount > 0)
                        {
                            m_iCurrentID = result[0]->id();
                            filter = "contactInfoId = ";
                            filter += Util::StringOp::FromInt(result[0]->id());

                            m_vSoundResult = Data::SoundSegment::GetFromDatabase(filter);
                            if (!m_vSoundResult.empty())
                            {
                                m_iCurrentSound = m_vSoundResult.size() - 1;
                                std::string nameNo;
                                if (m_vSoundResult[0]->name() != "")
                                {
                                    nameNo = m_vSoundResult[0]->name();
                                    nameNo = nameNo + "(" + m_vSoundResult[0]->telephoneNumber().number() + ")";
                                }
                                else
                                {
                                    nameNo = m_vSoundResult[0]->telephoneNumber().number();
                                }
                                m_sticNameNO.SetWindowText(Util::StringOp::ToCString(nameNo));
                                m_sFilePath = m_vSoundResult[m_iCurrentSound]->filename();
                                //m_sticSoundName.SetWindowText(Util::StringOp::ToCString(m_vSoundResult[m_iCurrentSound]->filename().substr(m_vSoundResult[m_iCurrentSound]->filename().length() - 10)));
                                m_iTotalSeconds = m_vSoundResult[m_iCurrentSound]->duration();
                                m_procbarSound.SetParam(0, 0, m_iTotalSeconds, 1);
                                ret = TRUE;
                                std::string sFile = m_vSoundResult[m_iCurrentSound]->filename();
                                CString sTemp = Util::StringOp::ToCString(sFile);
                                if (::DetectFile(sTemp.GetBuffer(256)))
                                {
                                    m_bHasNext = true;
                                    ret = TRUE;
                                    m_sticSoundName.SetWindowText(Util::StringOp::ToCString(m_vSoundResult[m_iCurrentSound]->filename().substr(m_vSoundResult[m_iCurrentSound]->filename().length() - 10)));
                                    break;
                                }
                                else
                                {
                                    m_sticSoundName.SetWindowText(L"文件不存在");
                                    m_sticSoundDuration.SetWindowText(_T(""));
                                }
                            }						
                        }
                    }
                    else
                    {
                        break;
                    }
                }
            }
        }
        if(ret)
            ShowSeconds();
        return ret;
    }

    void CPlaySoundDlg::SetModel(int model)
    {
        m_iModel = model;
    }

    void CPlaySoundDlg::ShowSeconds(void)
    {
        CString strTime;
        if (m_iTotalSeconds < 3600)
        {
            strTime.Format(_T("%02d:%02d/%02d:%02d"), m_iCurrentSecond / 60, m_iCurrentSecond % 60, m_iTotalSeconds / 60, m_iTotalSeconds % 60);
        }
        else
        {
            strTime.Format(_T("%02d:%02d:%02d/%02d:%02d:%02d"), m_iCurrentSecond / 3600, m_iCurrentSecond % 3600 / 60, m_iCurrentSecond % 60, m_iTotalSeconds / 3600, m_iTotalSeconds % 3600 / 60, m_iTotalSeconds % 60);
        }
        m_sticSoundDuration.SetWindowText(strTime);

        //关闭屏保，背光
        CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
        main->CancelBalckLightSaveTimer();
    }

    void CPlaySoundDlg::OnButtonClose() 
    {
        // TODO: Add your control notification handler code here
        ShowWindow_(FALSE);
        StopPlay();

        m_iTotalSeconds = 0;
        m_sticNameNO.SetWindowText(_T(""));
        m_sticSoundName.SetWindowText(_T(""));
        m_sticSoundDuration.SetWindowText(_T(""));
        m_vSoundResult.clear();
        m_iCurrentSound = 0;
        m_bOnlyNew = false;
        m_sFilePath = "";

        ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pSoundDlg->ShowItemsInList(-1);
        ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactInfoDlg->ShowItemsInList(-1);
    }

    //void CPlaySoundDlg::OnButtonNext() 
    void CPlaySoundDlg::OnButtonPrev() 
    {
        // TODO: Add your control notification handler code here
        StopPlay();
        m_bHasNext = false;

        if (m_iModel == 1)
        {
            --m_iCurrentSound;
            if (m_vSoundResult.size() > 1 && m_iCurrentSound >= 0)
            {
                m_sFilePath = m_vSoundResult[m_iCurrentSound]->filename();
                m_sticSoundName.SetWindowText(Util::StringOp::ToCString(m_vSoundResult[m_iCurrentSound]->filename().substr(m_vSoundResult[m_iCurrentSound]->filename().length() - 10)));
                m_iTotalSeconds = m_vSoundResult[m_iCurrentSound]->duration();
                m_procbarSound.SetParam(0, 0, m_iTotalSeconds, 1);
                ShowSeconds();
                m_bHasNext = true;
                return;
            }
            else
            {
                m_iCurrentSound = 0;
            }
        }
        SetSound(m_iType, m_iCurrentID, 1, m_sSearchFilter);
        // 	OnButtonPlay();
    }

    //void CPlaySoundDlg::OnButtonPrev() 
    void CPlaySoundDlg::OnButtonNext() 
    {
        // TODO: Add your control notification handler code here
        StopPlay();
        m_bHasNext = false;

        if (m_iModel == 1)
        {
            ++m_iCurrentSound;
            if (m_vSoundResult.size() > 1 && m_iCurrentSound < m_vSoundResult.size())
            {
                m_sFilePath = m_vSoundResult[m_iCurrentSound]->filename();
                m_sticSoundName.SetWindowText(Util::StringOp::ToCString(m_vSoundResult[m_iCurrentSound]->filename().substr(m_vSoundResult[m_iCurrentSound]->filename().length() - 10)));
                m_iTotalSeconds = m_vSoundResult[m_iCurrentSound]->duration();
                m_procbarSound.SetParam(0, 0, m_iTotalSeconds, 1);
                ShowSeconds();
                m_bHasNext = true;
                return;
            }
            else
            {
                m_iCurrentSound = m_vSoundResult.size() - 1;
            }
        }
        SetSound(m_iType, m_iCurrentID, 2, m_sSearchFilter);
        // 	OnButtonPlay();
    }

    void CPlaySoundDlg::OnButtonPlay() 
    {
        // TODO: Add your control notification handler code here
        if (m_bPlaying)
        {
            std::string strTemp;
            CString str;
            if (m_bPlayPause)
            {
                strTemp = Data::LanguageResource::Get(Data::RI_SOUND_PLAY_PLAY);
                m_bPlayPause = false;			
                m_MJPGList.SetUnitIsShow(3, TRUE);
                m_MJPGList.SetUnitIsShow(4, FALSE);
            }
            else
            {
                strTemp = Data::LanguageResource::Get(Data::RI_SOUND_PLAY_PAUSE);
                m_bPlayPause = true;
                m_MJPGList.SetUnitIsShow(4, TRUE);
                m_MJPGList.SetUnitIsShow(3, FALSE);
            }
            str = strTemp.c_str();
            //m_btnPlay.SetWindowText(str);
            m_pOggCodec->PauseDecode(!m_bPlayPause);		
        }
        else
        {
            // 		KillTimer(1);
            // 		m_pOggCodec->StopDecode(true);		
            //		((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->StopRing(true);
            // 		m_procbarSound.SetPos(0);
            // 		m_iCurrentSecond = 0;
            // 		ShowSeconds();

            waveOutSetVolume(NULL, ((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->phone_->g_RingSound);
            CString filename;
            filename = Util::StringOp::ToCString(m_sFilePath);
            CFileStatus   status;
            if (CFile::GetStatus(LPCTSTR(filename),status))
            {
                // 			((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->StartRing((LPTSTR)LPCTSTR(filename), 1);
                m_pOggCodec->DecodeFile(Util::StringOp::FromCString(filename));
                if (m_pOggCodec->StartDecode())
                {
                    SetTimer(1, 1000, 0);
                    std::string strTemp;
                    // 					CString str;
                    // 					strTemp = Data::LanguageResource::Get(Data::RI_SOUND_PLAY_PAUSE);
                    // 					str = strTemp.c_str();
                    // 					m_btnPlay.SetWindowText(str);
                    m_MJPGList.SetUnitIsShow(4, TRUE);
                    m_MJPGList.SetUnitIsShow(3, FALSE);
                    m_bPlaying = true;
                    m_bPlayPause = true;
                }
                std::vector<Util::shared_ptr<Data::SoundSegment> > result = Data::SoundSegment::GetFromDatabase("played = 0 AND filename = '" + Util::StringOp::FromCString(filename) + "'");	
                if (!result.empty())
                {
                    for (std::size_t i = 0; i < result.size(); ++i)
                    {
                        result[i]->played(true);
                        result[i]->Update();
                    }
                    ::SendMessage(((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pMainDlg->GetSafeHwnd(), WM_TELNOTIFY, 0, 0);
                }
            }
            else
            {
                m_sticSoundName.SetWindowText(L"文件不存在");
                m_sticSoundDuration.SetWindowText(_T(""));
                if(m_chkPlayContinue.GetCheck() == BST_CHECKED)
                {
                    OnButtonNext();
                    if (m_bHasNext)
                    {
                        OnButtonPlay();
                    }
                }
            }
        }
    }

    void CPlaySoundDlg::OnCheckPlaycontinue() 
    {
        // TODO: Add your control notification handler code here
    }

    LRESULT CPlaySoundDlg::OnPlaySeek(WPARAM w, LPARAM l)
    {
        LRESULT result = 0;
        if ((int)l == IDC_PBAR_SOUNDDLG_PROCESS)
        {
            if (m_bPlaying)
            {
                if (m_pOggCodec->DecodeSeek((float)w/(float)m_iTotalSeconds))
                {
                    m_iCurrentSecond = (int)w;
                }
            }
        }
        return result;
    }

    void CPlaySoundDlg::OnTimer(UINT nIDEvent) 
    {
        // TODO: Add your message handler code here and/or call default
        switch(nIDEvent)
        {
        case 1:
            if (!m_bPlayPause)
            {
                return;
            }
            ++m_iCurrentSecond;
            if (m_iCurrentSecond <= m_iTotalSeconds)
            {
                m_procbarSound.SetPos(m_iCurrentSecond);
                ShowSeconds();
            }
            else
            {
                KillTimer(1);
                //			((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->StopRing(true);
                m_pOggCodec->StopDecode(true);			
                m_bPlaying = false;
                m_bPlayPause = false;			
                // 			std::string strTemp = Data::LanguageResource::Get(Data::RI_SOUND_PLAY_PLAY);
                // 			CString str = strTemp.c_str();
                // 			m_btnPlay.SetWindowText(str);
                m_MJPGList.SetUnitIsShow(3, TRUE);
                m_MJPGList.SetUnitIsShow(4, FALSE);

                m_procbarSound.SetPos(0);
                m_iCurrentSecond = 0;
                ShowSeconds();

                if(m_chkPlayContinue.GetCheck() == BST_CHECKED)
                {
                    OnButtonNext();
                    if (m_bHasNext)
                    {
                        OnButtonPlay();
                    }
                }
            }
            break;
        }

        CCEDialog::OnTimer(nIDEvent);
    }

    void CPlaySoundDlg::CloseSound(void)
    {
        //OnButtonClose();
        StopPlay();
    }

    void CPlaySoundDlg::StartPlay(bool onlyNew)
    {
        if (onlyNew)
        {
            m_bOnlyNew = true;
            m_chkPlayContinue.SetCheck(BST_CHECKED);
        }
        CMultimediaPhoneDlg *pWnd = (CMultimediaPhoneDlg *)theApp.m_pMainWnd;
        pWnd->m_pMainDlg->m_mainMp3Dlg_->OnTimer(1002); //SendMessage(WM_OUTEVENT, 0, 0);
        OnButtonPlay();
    }
    void CPlaySoundDlg::StopPlay(void)
    {
        m_pOggCodec->StopDecode(true);
        KillTimer(1);
        m_bPlaying = false;
        m_bPlayPause = false;
        m_MJPGList.SetUnitIsShow(3, TRUE);
        m_MJPGList.SetUnitIsShow(4, FALSE);
        m_procbarSound.SetPos(0);
        m_iCurrentSecond = 0;
        ShowSeconds();
    }
}
