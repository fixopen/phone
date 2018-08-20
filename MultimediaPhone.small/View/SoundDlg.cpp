// SoundDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SoundDlg.h"
#include "../MultimediaPhone.h"
#include "../MultimediaPhoneDlg.h"
#include "../Data/LanguageResource.h"
#include "../Data/Data.h"
#include "../Data/ContactInfo.h"
#include "../Data/Contact.h"
#include "../Data/SkinStyle.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace View {
    /////////////////////////////////////////////////////////////////////////////
    // CSoundDlg dialog

    CSoundDlg::CSoundDlg(CWnd* pParent /*=NULL*/)
        : CDialog(CSoundDlg::IDD, pParent)
        , SoundTotal(10000)
        , PageSize(10)
    {
        //{{AFX_DATA_INIT(CSoundDlg)
        // NOTE: the ClassWizard will add member initialization here
        //}}AFX_DATA_INIT
    }


    void CSoundDlg::DoDataExchange(CDataExchange* pDX)
    {
        CDialog::DoDataExchange(pDX);
        //{{AFX_DATA_MAP(CSoundDlg)
        // NOTE: the ClassWizard will add DDX and DDV calls here
        //}}AFX_DATA_MAP
    }


    BEGIN_MESSAGE_MAP(CSoundDlg, CDialog)
        //{{AFX_MSG_MAP(CSoundDlg)
        //}}AFX_MSG_MAP
        ON_NOTIFY(NM_CLICK, IDC_LIST_SOUND_TYPE, OnClickListType)
        ON_NOTIFY(NM_CLICK, IDC_LIST_SOUND_LIST, OnClickListList)
        ON_BN_CLICKED(IDC_BUTTON_SOUND_SOUND, OnButtonSound)
        ON_BN_CLICKED(IDC_BUTTON_SOUND_RECORD, OnButtonRecord)
        ON_BN_CLICKED(IDC_BUTTON_SOUND_SEARCH, OnButtonSearch)
        ON_BN_CLICKED(IDC_BUTTON_SOUND_SETTIP, OnButtonSetTip)
        ON_BN_CLICKED(IDC_BUTTON_SOUND_DELETE, OnButtonDelete)
        ON_BN_CLICKED(IDC_BUTTON_SOUND_CLOSE, OnButtonClose)
        ON_MESSAGE(WM_CLICKMJPG_TOAPP, OnClickMJPG)
        ON_MESSAGE(WM_LISTCTRL_CLICK, OnListCtrlClick)
        ON_MESSAGE(WM_RENAME, OnRename)
    END_MESSAGE_MAP()

    LRESULT CSoundDlg::OnListCtrlClick(WPARAM w, LPARAM l)
    {
        LRESULT ret;
        if(w == IDC_LIST_SOUND_TYPE)
            OnClickListType(NULL, &ret);
        else if(w == IDC_LIST_SOUND_LIST)
            OnClickListList(NULL, &ret);
        return ret;
    }

    LRESULT CSoundDlg::OnRename(WPARAM w, LPARAM l)
    {
        LRESULT result = 0;
        CString s;
        m_pRenameDlg->m_edtName.GetWindowText(s);
        std::string sName = Util::StringOp::FromCString(s);
        m_vCurrentResult[w]->name(sName);
        m_vCurrentResult[w]->Update();
        m_lsList.SetItemText(w, 1, s);
        return result;
    }

    /////////////////////////////////////////////////////////////////////////////
    // CSoundDlg message handlers
    LRESULT CSoundDlg::OnClickMJPG(WPARAM w, LPARAM l)
    {
        LRESULT result = 0;
        switch (w)
        {
        case 1:
        case 2:
        case 3:
        case 4:
            ShowItemsInList(w - 1);
            break;
        case 5:
            OnButtonSound();
            break;
        case 6:
            OnButtonRecord();
            break;
        case 7:
            OnButtonSearch();
            break;
        case 8:
            OnButtonSetTip();
            break;
        case 9:
            //lxz 20081118 rename
            {
                POSITION pos = m_lsList.GetFirstSelectedItemPosition();
                if (pos != NULL)
                {
                    int index = m_lsList.GetNextSelectedItem (pos);   		
                    std::string filename = m_vCurrentResult[index]->name();	
                    m_pRenameDlg->SetWndParam(this, index, Util::StringOp::ToCString(filename.c_str()));
                    m_pRenameDlg->ShowWindow_(SW_SHOW);
                }
            }
            break;
        case 11:
            OnButtonDelete();   
            break;
        case 10:
            OnButtonClose();
            break;
        }
        return result;
    }

    BOOL CSoundDlg::OnInitDialog() 
    {
        CDialog::OnInitDialog();

        // TODO: Add extra initialization here
        std::string strTemp;
        CString str;
        /*
        m_lsType.Create(WS_CHILD|WS_VISIBLE|LVS_REPORT|LVS_NOCOLUMNHEADER|LVS_NOSORTHEADER, CRect(8, 27, 80, 198), this, IDC_LIST_SOUND_TYPE, FALSE);
        m_lsType.SetExtendedStyle(m_lsType.GetExtendedStyle()|LVS_EX_FULLROWSELECT);
        m_lsType.InsertColumn(0, _T("Type"), LVCFMT_LEFT, 72);

        m_pImageList = new CImageList();
        m_pImageList->Create(16, 16, ILC_COLOR32|ILC_MASK, 3, 2);   
        CBitmap bm;
        bm.LoadBitmap(IDB_BITMAP1);
        m_pImageList->Add(&bm, RGB(255, 0, 255));
        bm.DeleteObject();
        bm.LoadBitmap(IDB_BITMAP24);
        m_pImageList->Add(&bm, RGB(255, 0, 255));
        bm.DeleteObject();
        bm.LoadBitmap(IDB_BITMAP25);
        m_pImageList->Add(&bm, RGB(255, 0, 255));
        bm.DeleteObject();
        bm.LoadBitmap(IDB_BITMAP32);
        m_pImageList->Add(&bm, RGB(255, 0, 255));
        bm.DeleteObject();
        // 	bm.LoadBitmap(IDB_BITMAP33);
        // 	m_pImageList->Add(&bm, RGB(255, 0, 255));
        // 	bm.DeleteObject();

        m_lsType.SetImageList(m_pImageList, LVSIL_SMALL);

        strTemp = Data::LanguageResource::Get(Data::RI_SOUND_SOUND);
        str = strTemp.c_str();
        m_lsType.InsertItem(0, str, 1);

        strTemp = Data::LanguageResource::Get(Data::RI_SOUND_RECORD);
        str = strTemp.c_str();
        m_lsType.InsertItem(1, str, 2);

        strTemp = Data::LanguageResource::Get(Data::RI_SOUND_LOCALRECORD);
        str = strTemp.c_str();
        m_lsType.InsertItem(2, str, 3);

        // 	strTemp = Data::LanguageResource::Get(Data::RI_SOUND_LEAVEWORDTIP);
        // 	str = strTemp.c_str();
        // 	m_lsType.InsertItem(3, str, 4);
        // 
        // 	strTemp = Data::LanguageResource::Get(Data::RI_RECORD_SEARCHRESULT);
        // 	str = strTemp.c_str();
        // 	m_lsType.InsertItem(4, str, 0);

        strTemp = Data::LanguageResource::Get(Data::RI_RECORD_SEARCHRESULT);
        str = strTemp.c_str();
        m_lsType.InsertItem(3, str, 0);

        m_lsType.SetScrollRagle(FALSE);
        */	
        m_lsList.Create(WS_CHILD|WS_VISIBLE|LVS_REPORT|LVS_NOCOLUMNHEADER|LVS_NOSORTHEADER, CRect(115, 46, 115+570, 45+360), this, IDC_LIST_SOUND_LIST, TRUE, 0, FALSE);
        m_lsList.SetExtendedStyle(m_lsList.GetExtendedStyle()|LVS_EX_FULLROWSELECT);
        m_lsList.InsertColumn(0, _T("Sound"), LVCFMT_LEFT, 120+10);
        m_lsList.InsertColumn(1, _T("NameNO"), LVCFMT_LEFT, 130+10-16);
        m_lsList.InsertColumn(2, _T("Time"), LVCFMT_LEFT, 180+10-24);
        m_lsList.InsertColumn(3, _T("Duration"), LVCFMT_LEFT, 100+10+40);


        m_pImageList = new CImageList();
        CBitmap bm;
        m_pImageList->Create(32, 32, ILC_COLOR32|ILC_MASK, 2, 2);   
        bm.LoadBitmap(IDB_BITMAP23);
        m_pImageList->Add(&bm, RGB(255, 0, 255));
        bm.DeleteObject();
        bm.LoadBitmap(IDB_BITMAP26);
        m_pImageList->Add(&bm, RGB(255, 0, 255));
        bm.DeleteObject();


        m_lsList.SetImageList(m_pImageList, LVSIL_SMALL);
        /*
        strTemp = Data::LanguageResource::Get(Data::RI_SOUND_PLAY);
        str = strTemp.c_str();
        m_btnSound.Create(str, Data::g_buttonRectBMPID[0][Data::g_skinstyle], Data::g_buttonRectBMPID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(417, 26, 471, 45), this, IDC_BUTTON_SOUND_SOUND);
        //	m_btnSound.SetBackRGB(Data::g_allFramBackRGB[Data::g_skinstyle]);

        //	strTemp = Data::LanguageResource::Get(Data::RI_TEL_RECORDEBTN);
        strTemp = Data::LanguageResource::Get(Data::RI_SOUND_LOCALRECORD);
        str = strTemp.c_str();
        m_btnRecord.Create(str, Data::g_buttonRectBMPID[0][Data::g_skinstyle], Data::g_buttonRectBMPID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(417, 46, 471, 65), this, IDC_BUTTON_SOUND_RECORD);
        //	m_btnRecord.SetBackRGB(Data::g_allFramBackRGB[Data::g_skinstyle]);

        strTemp = Data::LanguageResource::Get(Data::RI_COMN_FINDBTN);
        str = strTemp.c_str();
        m_btnSearch.Create(str, Data::g_buttonRectBMPID[0][Data::g_skinstyle], Data::g_buttonRectBMPID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(417, 66, 471, 85), this, IDC_BUTTON_SOUND_SEARCH);
        //	m_btnSearch.SetBackRGB(Data::g_allFramBackRGB[Data::g_skinstyle]);

        strTemp = Data::LanguageResource::Get(Data::RI_SOUND_LEAVEWORDTIP);
        str = strTemp.c_str();
        m_btnSetTip.Create(str, Data::g_buttonRectBMPID[0][Data::g_skinstyle], Data::g_buttonRectBMPID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(417, 86, 471, 105), this, IDC_BUTTON_SOUND_SETTIP);
        //	m_btnProtect.SetBackRGB(Data::g_allFramBackRGB[Data::g_skinstyle]);

        strTemp = Data::LanguageResource::Get(Data::RI_RECORD_DELETE);
        str = strTemp.c_str();
        m_btnDelete.Create(str, Data::g_buttonRectBMPID[0][Data::g_skinstyle], Data::g_buttonRectBMPID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(417, 106, 471, 125), this, IDC_BUTTON_SOUND_DELETE);
        //	m_btnDelete.SetBackRGB(Data::g_allFramBackRGB[Data::g_skinstyle]);

        strTemp = Data::LanguageResource::Get(Data::RI_SOUND_PLAY_CLOSE);
        str = strTemp.c_str();
        m_btnClose.Create(str, Data::g_buttonRectBMPID[0][Data::g_skinstyle], Data::g_buttonRectBMPID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(417, 178, 471, 198), this, IDC_BUTTON_SOUND_CLOSE);
        m_btnClose.SetBackRGB(Data::g_allFramBackRGB[Data::g_skinstyle]);

        m_sticPanel.Create(_T(""), WS_CHILD|WS_VISIBLE, CRect(416, 26, 472, 198), this);
        m_sticPanel.SetColor(RGB(0, 0, 0), Data::g_allFrameInFrameLine1RGB[0][Data::g_skinstyle]);

        m_sticBackground.Create(CRect(0, 0, 480, 204), this, 2);

        TextStruct ts[6];
        memset(ts, 0, sizeof(TextStruct) * 6);

        ts[0].txtRect = CRect(0, 0, 80, 20);
        ts[0].txtFontSize = 16;
        ts[0].sAlign = DT_CENTER | DT_BOTTOM;
        memcpy(ts[0].sTxt, Data::LanguageResource::Get(Data::RI_RECORD_TYPE).c_str(), Data::LanguageResource::Get(Data::RI_RECORD_TYPE).length());

        ts[1].txtRect = CRect(81, 0, 160, 20);
        ts[1].txtFontSize = 16;
        ts[1].sAlign = DT_CENTER | DT_BOTTOM;
        memcpy(ts[1].sTxt, Data::LanguageResource::Get(Data::RI_RECORD_SOUND).c_str(), Data::LanguageResource::Get(Data::RI_RECORD_SOUND).length());

        ts[2].txtRect = CRect(160, 0, 260, 20);
        ts[2].txtFontSize = 16;
        ts[2].sAlign = DT_CENTER | DT_BOTTOM;
        memcpy(ts[2].sTxt, Data::LanguageResource::Get(Data::RI_RECORD_NAMENO).c_str(), Data::LanguageResource::Get(Data::RI_RECORD_NAMENO).length());

        ts[3].txtRect = CRect(260, 0, 340, 20);
        ts[3].txtFontSize = 16;
        ts[3].sAlign = DT_CENTER | DT_BOTTOM;
        memcpy(ts[3].sTxt, Data::LanguageResource::Get(Data::RI_RECORD_TIME).c_str(), Data::LanguageResource::Get(Data::RI_RECORD_TIME).length());

        ts[4].txtRect = CRect(340, 0, 396, 20);
        ts[4].txtFontSize = 16;
        ts[4].sAlign = DT_CENTER | DT_BOTTOM;
        memcpy(ts[4].sTxt, Data::LanguageResource::Get(Data::RI_RECORD_DURATION).c_str(), Data::LanguageResource::Get(Data::RI_RECORD_DURATION).length());

        ts[5].txtRect = CRect(417, 0, 470, 20);
        ts[5].txtFontSize = 16;
        ts[5].sAlign = DT_CENTER | DT_BOTTOM;
        memcpy(ts[5].sTxt, Data::LanguageResource::Get(Data::RI_RECORD_OPERATION).c_str(), Data::LanguageResource::Get(Data::RI_RECORD_OPERATION).length());

        m_sticBackground.SetTextStruct(ts, 6);
        */
        m_MJPGList.Create(L"", WS_VISIBLE|WS_CHILD, CRect(0, 0, 800, 420), this);
        m_MJPGList.SetCurrentLinkFile(_T(".\\adv\\mjpg\\k1\\中文\\录音.xml"));
        m_MJPGList.SetMJPGRect(CRect(0, 0, 800, 420));

        m_pPlaySoundDlg = new CPlaySoundDlg(this);
        m_pPlaySoundDlg->Create(CPlaySoundDlg::IDD);
        m_pPlaySoundDlg->SetModel(0);

        m_pSoundSearchDlg = new CSoundSearchDlg(this);
        m_pSoundSearchDlg->Create(CSoundSearchDlg::IDD);

        m_pPasswordDlg = new CPasswordDlg(this);
        m_pPasswordDlg->Create(CPasswordDlg::IDD);

        m_pRecordSoundDlg = new CRecordSoundDlg(this);
        m_pRecordSoundDlg->Create(CRecordSoundDlg::IDD);

        m_pRenameDlg = new CRenameDlg(this);
        m_pRenameDlg->Create(CRenameDlg::IDD);

        m_bTelephoneInUse = false;
        m_uiSelectIndex = 0;
        // 	m_lsType.SetItemState(0, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
        m_uiType = 0;
        ShowItemsInList(m_uiType);

        return TRUE;  // return TRUE unless you set the focus to a control
        // EXCEPTION: OCX Property Pages should return FALSE
    }

    void CSoundDlg::SetButtonDefaultColor(CCEButtonST* button)
    {
        button->SetColor(CCEButtonST::BTNST_COLOR_BK_IN, RGB(248,214,147));
        button->SetColor(CCEButtonST::BTNST_COLOR_BK_OUT, RGB(248,214,147));
        button->SetColor(CCEButtonST::BTNST_COLOR_BK_FOCUS, RGB(248,214,147));
    }

    void CSoundDlg::SetButtonSelectedColor(CCEButtonST* button)
    {
        button->SetColor(CCEButtonST::BTNST_COLOR_BK_IN, RGB(248,214,147));
        button->SetColor(CCEButtonST::BTNST_COLOR_BK_OUT, RGB(248,214,147));
        button->SetColor(CCEButtonST::BTNST_COLOR_BK_FOCUS, RGB(248,214,147));
    }

    void CSoundDlg::OnClickListType(NMHDR* pNMHDR, LRESULT* pResult) 
    {
        // TODO: Add your control notification handler code here
        //	ShowItemsInList();
        *pResult = 0;
    }

    void CSoundDlg::OnClickListList(NMHDR* pNMHDR, LRESULT* pResult) 
    {
        // TODO: Add your control notification handler code here
        POSITION pos = m_lsList.GetFirstSelectedItemPosition();
        if (pos != NULL)
        {
            int index = m_lsList.GetNextSelectedItem (pos);
            m_uiSelectIndex = index;
        }

        *pResult = 0;
    }

    void CSoundDlg::OnButtonSound() 
    {
        // TODO: Add your control notification handler code here
        if ((((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pFSM->getCurrentState() != CMultimediaPhoneDlg::tsHangOff))
            return;

        POSITION pos = m_lsList.GetFirstSelectedItemPosition();
        if (pos != NULL)
        {
            int index = m_lsList.GetNextSelectedItem (pos);   		
            // 		pos = m_lsType.GetFirstSelectedItemPosition();
            // 		m_pPlaySoundDlg->SetSound(m_lsType.GetNextSelectedItem(pos), m_lsList.GetItemData(index), 0, m_sListSearchFilter);

            if(m_pPlaySoundDlg->SetSound(m_uiType, m_lsList.GetItemData(index), 0, m_sListSearchFilter))
            {
                if (((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pSettingDlg->m_pSetting->isPlayProtect() && !((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pSettingDlg->m_bLogin)
                {
                    m_pPasswordDlg->SetType(CHECK_PASSWORD);
                    std::string strTemp = ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pSettingDlg->m_pSetting->playRecordPassword();
                    m_pPasswordDlg->SetOldPassWord((char *)strTemp.c_str());
                    m_pPasswordDlg->SetHWnd(this->m_hWnd);
                    m_pPasswordDlg->ShowWindow(SW_SHOW);	
                }
                else
                {
                    m_pPlaySoundDlg->StartPlay(false);
                    m_pPlaySoundDlg->ShowWindow(TRUE);
                }
            }
        }
    }

    void CSoundDlg::OnButtonRecord()
    {
        if ((((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pFSM->getCurrentState() != CMultimediaPhoneDlg::tsHangOff))
            return;

        // 	POSITION pos = m_lsType.GetFirstSelectedItemPosition();
        // 	if (pos != NULL)
        // 	{
        // 		int index = m_lsType.GetNextSelectedItem (pos);   		
        // // 		if (index == 3)
        // // 		{
        // // 			m_pRecordSoundDlg->SetSoundSegmentType(Data::sstTips);
        // // 		}
        // // 		else
        // // 		{
        // // 			m_pRecordSoundDlg->SetSoundSegmentType(Data::sstLocalRecord);
        // // 		}
        m_pRecordSoundDlg->ShowWindow_(TRUE);
        // 	}
    }

    void CSoundDlg::OnButtonSearch() 
    {
        // TODO: Add your control notification handler code here
        m_pSoundSearchDlg->ShowWindow(TRUE);
    }

    void CSoundDlg::OnButtonSetTip() 
    {
        // TODO: Add your control notification handler code here
        POSITION pos = m_lsList.GetFirstSelectedItemPosition();
        if (pos != NULL)
        {	
            int index = m_lsList.GetNextSelectedItem(pos);
            if ((m_vCurrentResult[index]->type() == Data::sstLocalRecord) && (m_vCurrentResult[index]->duration() <= 10))
            {
                m_vCurrentResult[index]->isTip(!m_vCurrentResult[index]->isTip());
                m_vCurrentResult[index]->Update();
                ShowArrayInList(m_vCurrentResult);
                ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pSettingDlg->UpdateTips();
            }
        }
    }

    void CSoundDlg::OnButtonDelete() 
    {
        // TODO: Add your control notification handler code here
        POSITION pos = m_lsList.GetFirstSelectedItemPosition();
        if (pos != NULL)
        {
            ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetHWnd(this->GetSafeHwnd());
            if (!((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pSettingDlg->m_bLogin)
            {
                ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetPasswordModel(true);
            }
            std::string pw = ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pSettingDlg->m_pSetting->adminPassword();
            ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetPassword(Util::StringOp::ToCString(pw));
            std::string strTemp = Data::LanguageResource::Get(Data::RI_DELETETIP_TIP);
            ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetDelTip(Util::StringOp::ToCString(strTemp.c_str()));
            ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->ShowWindow(TRUE);
        }
    }

    void CSoundDlg::OnButtonClose()
    {
        //	ShowItemsInList();
        ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->SwitchPanel_(IDC_BUTTON_MAIN);
    }

    void CSoundDlg::ShowItemsInList(int type)
    {
        // 	POSITION pos = m_lsType.GetFirstSelectedItemPosition();
        // 	if (pos == NULL)
        // 	{
        // 		return;
        // 	}
        // 	int type = m_lsType.GetNextSelectedItem(pos);

        if (type == -1)
        {
            type = m_uiType;
        }
        else
        {
            m_uiType = type;
        }

        for (int i = 0; i < 4; ++i)
        {
            if (i == m_uiType)
            {
                m_MJPGList.SetUnitIsDownStatus(i, TRUE);
            }
            else
            {
                m_MJPGList.SetUnitIsDownStatus(i, FALSE);
            }
        }
        m_MJPGList.Invalidate();

        m_lsList.DeleteAllItems();
        std::string filter;

        if (type == 0)
        {
            m_sListFilter = "type = " + Util::StringOp::FromInt(Data::sstLeaveWord);
            filter = m_sListFilter;
        }
        else if (type == 1)
        {
            m_sListFilter = "type = " + Util::StringOp::FromInt(Data::sstTeleRecord);
            filter = m_sListFilter;
        }
        else if (type == 2)
        {
            m_sListFilter = "type = " + Util::StringOp::FromInt(Data::sstLocalRecord);
            filter = m_sListFilter;
        }
        else if (type == 3)
        {
            // 		m_sListFilter = "type = " + Util::StringOp::FromInt(Data::sstTips);
            // 		filter = m_sListFilter;
            // 	}
            // 	else if (type == 4)
            // 	{
            if (m_sListSearchFilter == "")
            {
                return;
            }
            filter = m_sListSearchFilter;
        }

        if (Data::SoundSegment::GetDataCount("") > 0)
        {
            m_vCurrentResult = Data::SoundSegment::GetFromDatabase(filter, "id", Data::srDesc, Data::SoundSegment::GetCurrentId() + 1, PageSize);
            int count = Data::SoundSegment::GetDataCount(filter);
            m_lsList.m_scollbar_.SetRangle(0, count, PageSize);
            ShowArrayInList(m_vCurrentResult, type);	
            m_lsList.SetScrollRagle(TRUE);
        }
    }

    void CSoundDlg::ScrollItemsInList(int step, int nPos)
    {
        if (m_vCurrentResult.empty())
        {
            return;
        }
        std::string filter = m_sListFilter;

        if (m_uiType == 3)
        {
            filter = m_sListSearchFilter;
        }

        // 	std::string s = "";
        // 	if(m_uiType < 3)
        // 	{
        // 		char txt[4];
        // 		sprintf(txt, "%d", m_uiType);
        // 		s = txt;
        // 	}

        m_vCurrentResult.clear();
        m_vCurrentResult = Data::SoundSegment::GetFromDatabase(filter, "id", Data::srDesc, nPos, PageSize);
        // 
        // 	std::vector<Util::shared_ptr<Data::SoundSegment> > temp;
        // 	int currentID = 0;

        if (step == SB_LINEUP)
        {
            // 		currentID = m_vCurrentResult[0]->id();
            // 		temp = Data::SoundSegment::GetFromDatabase(filter, Data::dUp, currentID, 1);
            // 		if (!temp.empty())
            // 		{
            // 			m_vCurrentResult.insert(m_vCurrentResult.begin(),temp[0]);
            // 			if (m_vCurrentResult.size() > PageSize)
            // 			{
            // 				m_vCurrentResult.pop_back();
            // 			}

            if (m_uiSelectIndex < m_vCurrentResult.size() - 1)
            {
                ++m_uiSelectIndex;
            }
            //		}
        }
        else if (step == SB_LINEDOWN)
        {
            // 		currentID = m_vCurrentResult[m_vCurrentResult.size() - 1]->id();
            // 		temp = Data::SoundSegment::GetFromDatabase(filter, Data::dDown, currentID, 1);
            // 		if (!temp.empty())
            // 		{
            // 			m_vCurrentResult.push_back(temp[0]);
            // 			if (m_vCurrentResult.size() > PageSize)
            // 			{
            // 				m_vCurrentResult.erase(m_vCurrentResult.begin());
            // 			}

            if (m_uiSelectIndex > 0)
            {
                --m_uiSelectIndex;
            }		
            //		}
        }

        else if (step == SB_PAGEUP)
        {
            // 		currentID = m_vCurrentResult[0]->id();
            // 		temp = Data::SoundSegment::GetFromDatabase(filter, Data::dUp, currentID, PageSize);
            // 		if (temp.size() < PageSize)
            // 		{
            // 			temp.insert(temp.end(), m_vCurrentResult.begin(), m_vCurrentResult.begin() + PageSize - temp.size());
            // 		}
            // 		m_vCurrentResult = temp;
            m_uiSelectIndex = 0;
        }
        else if(step == SB_PAGEDOWN)
        {
            // 		currentID = m_vCurrentResult[m_vCurrentResult.size() - 1]->id();
            // 		temp = Data::SoundSegment::GetFromDatabase(filter, Data::dDown, currentID, PageSize);
            // 		if (temp.size() < PageSize)
            // 		{
            // 			m_vCurrentResult.insert(m_vCurrentResult.end(), temp.begin(), temp.end());
            // 			if (m_vCurrentResult.size() > PageSize)
            // 			{
            // 				m_vCurrentResult.erase(m_vCurrentResult.begin(), m_vCurrentResult.begin() + m_vCurrentResult.size() - PageSize);
            // 			}
            // 		}
            // 		else
            // 		{
            // 			m_vCurrentResult = temp;
            // 		}
            m_uiSelectIndex = 0;
        }
        else if(step == SB_THUMBPOSITION)
        {
            m_uiSelectIndex = 0;

            // 		if(m_SoundFilter.size() <= 0)
            // 		{
            // 			m_SoundFilter = Data::SoundSegment::GetFromDatabase(filter);
            // 		}
            // 		m_vCurrentResult.clear();
            // 		if (m_SoundFilter.size() > nPos)
            // 		{
            // 			int nOff = m_SoundFilter.size() - nPos;
            // 			if(nOff >= PageSize)
            // 			{
            // 				m_vCurrentResult.insert(m_vCurrentResult.begin(), m_SoundFilter.begin() + nPos, m_SoundFilter.begin() + nPos + PageSize);
            // 			}
            // 			else
            // 				m_vCurrentResult.insert(m_vCurrentResult.begin(), m_SoundFilter.begin() + m_SoundFilter.size() - PageSize, m_SoundFilter.end());
            // 		}
            // 		else
            // 		{
            // 			m_vCurrentResult.insert(m_vCurrentResult.begin(), m_SoundFilter.begin() + m_SoundFilter.size() - PageSize, m_SoundFilter.end());
            // 		}

            // 		temp = Data::SoundSegment::GetFromDatabase(filter);
            // 		m_vCurrentResult.clear();
            // 		if (temp.size() > nPos)
            // 		{
            // 			int nOff = temp.size() - nPos;
            // 			if(nOff >= PageSize)
            // 			{
            // 				m_vCurrentResult.insert(m_vCurrentResult.begin(), temp.begin() + nPos, temp.begin() + nPos + PageSize);
            // 			}
            // 			else
            // 				m_vCurrentResult.insert(m_vCurrentResult.begin(), temp.begin() + temp.size() - PageSize, temp.end());
            // 		}
            // 		else
            // 		{
            // 			m_vCurrentResult.insert(m_vCurrentResult.begin(), temp.begin() + temp.size() - PageSize, temp.end());
            // 		}
            // 		temp.clear();
        }

        ShowArrayInList(m_vCurrentResult);		
    }

    void CSoundDlg::ShowArrayInList(std::vector<Util::shared_ptr<Data::SoundSegment> > array, int type)
    {
        m_lsList.DeleteAllItems();
        // 	UINT recordCount = 0;
        // 	if (type == 0)
        // 	{
        // 		recordCount = ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pTelephoneDlg->GetRecordCount();
        // 	}
        CString temp;
        for (std::size_t i = 0; i < (array).size(); ++i)
        {	
            std::string s = array[i]->filename();
            s = s.substr(s.length() - 10, 6);
            //temp = array[i]->filename().c_str();
            temp = Util::StringOp::ToCString(s);

            //		if (type == 0 && i < recordCount)
            //		if (type <= 0 && !array[i]->played())
            if (!array[i]->played())
            {
                m_lsList.InsertItem(i, temp, 0);
            }
            else
            {
                //  已读不标记
                m_lsList.InsertItem(i, temp, 1);
            }

            // 		if (array[i]->GetContact())
            // 		{
            // 			temp = (array[i]->GetContact()->name() + "(" + array[i]->telephoneNumber().number() + ")").c_str();
            // 		}
            // 		else
            // 		{
            // 			temp = array[i]->telephoneNumber().number().c_str();
            // 		}


            // 		if (array[i]->name() != "")
            // 		{
            // 			temp = (array[i]->name() + "(" + array[i]->telephoneNumber().number() + ")").c_str();
            // 		}
            // 		else
            // 		{
            // 			temp = array[i]->telephoneNumber().number().c_str();
            // 		}
            // 		m_lsList.SetItemText(i, 1, temp);

            if (array[i]->type() == 2)
            {
                if (array[i]->isTip())
                {
                    temp = Util::StringOp::ToCString(Data::LanguageResource::Get(Data::RI_SOUND_LEAVEWORDTIP));
                    m_lsList.SetItemText(i, 1, temp);
                }
                else if (array[i]->name() != "")
                {
                    temp = array[i]->name().c_str();
                    m_lsList.SetItemText(i, 1, temp);
                }

            }
            else
            {
                if (array[i]->name() != "")
                {
                    temp = (array[i]->name() + "(" + array[i]->telephoneNumber().number() + ")").c_str();
                }
                else
                {
                    temp = array[i]->telephoneNumber().number().c_str();
                }
                m_lsList.SetItemText(i, 1, temp);
            }

            temp.Format(_T("%02d-%02d %02d:%02d"),
                array[i]->startTime().GetMonth(),
                array[i]->startTime().GetDay(),
                array[i]->startTime().GetHour(),
                array[i]->startTime().GetMinute());
            m_lsList.SetItemText(i, 2, temp);

            if (array[i]->duration() >= 3600)
            {
                temp.Format(_T("%d:%02d:%02d"), array[i]->duration() / 3600 ,array[i]->duration() % 3600 / 60, array[i]->duration() % 60);
            }
            else
            {
                temp.Format(_T("%02d:%02d"), array[i]->duration() / 60, array[i]->duration() % 60);
            }

            std::string sFile = array[i]->filename();
            CString sTemp = Util::StringOp::ToCString(sFile);
            extern BOOL DetectFile(TCHAR *sDir);
            if(!DetectFile(sTemp.GetBuffer(256)))
                temp = "不存在";
            m_lsList.SetItemText(i, 3, temp);

            m_lsList.SetItemData(i, array[i]->id());
        }

        if (!array.empty())
        {
            POSITION pos = m_lsList.GetFirstSelectedItemPosition();   
            while (pos != NULL)
            {   
                int iSel = m_lsList.GetNextSelectedItem(pos);   
                m_lsList.SetItemState(iSel, 0, LVIS_SELECTED);   
            }
            m_lsList.SetItemState(m_uiSelectIndex, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
        }
    }

    void CSoundDlg::DeleteSelectedItem(void)
    {
        POSITION pos = m_lsList.GetFirstSelectedItemPosition();
        if (pos != NULL)
        {
            int index = m_lsList.GetNextSelectedItem (pos);   		
            //		int soundsegmentID = m_lsList.GetItemData(index);

            std::string filename = m_vCurrentResult[index]->filename();	
            bool isTip = m_vCurrentResult[index]->isTip();
            m_vCurrentResult[index]->Remove();

            CFileStatus   status;
            if (CFile::GetStatus(LPCTSTR(Util::StringOp::ToCString(filename)),status))
            {
                try
                {
                    CFile::Remove(LPCTSTR(Util::StringOp::ToCString(filename)));
                }
                catch (CFileException* pEx)
                {
                    pEx->Delete();
                }
            }
            ShowItemsInList(m_uiType);		
            ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactInfoDlg->ShowItemsInList(-1);

            if (isTip)
            {
                ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pSettingDlg->UpdateTips();
            }
            ::SendMessage(((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pMainDlg->GetSafeHwnd(), WM_TELNOTIFY, 0, 0);
        }
    }

    void CSoundDlg::Search(std::string filter)
    {
        m_sListSearchFilter = filter;
        // 	POSITION pos = m_lsType.GetFirstSelectedItemPosition();   
        // 	while (pos != NULL)
        // 	{   
        // 		int iSel = m_lsType.GetNextSelectedItem(pos);   
        // 		m_lsType.SetItemState(iSel, 0, LVIS_SELECTED);   
        // 	}
        //     m_lsType.SetItemState(m_lsType.GetItemCount() - 1, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);

        ShowItemsInList(3);
    }

    void CSoundDlg::SaveSoundSegment(Util::shared_ptr<Data::SoundSegment> ss)
    {
        ss->Insert();
        //	m_vContactInfo.insert(m_vContactInfo.begin(), ci);
        m_uiSelectIndex = 0;
        ShowItemsInList(0);
        int cout = Data::SoundSegment::GetDataCount("");
        if (cout > SoundTotal)
        {
            std::vector<Util::shared_ptr<Data::SoundSegment> > temp = Data::SoundSegment::GetFromDatabase("", "id", Data::srAsc, 0, cout - SoundTotal);
            if (!temp.empty())
            {
                for (std::size_t i = 0; i < temp.size(); ++i)
                {
                    temp[i]->Remove();
                }
            }
        }
    }

    LRESULT CSoundDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
    {
        // TODO: Add your specialized code here and/or call the base class
        switch (message)
        {
        case WM_DELETESELITEM:
            DeleteSelectedItem();
            break;
        case WM_SETTINGPASSWORD:
            ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pSettingDlg->m_pSetting->soundProtectPassword(std::string((char*)wParam, lParam));
            ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pSettingDlg->m_pSetting->Update();
            break;
        case WM_VSCROLL:
            ScrollItemsInList(wParam, lParam);
            break;
        case WM_CHECKPASSWORD:
            m_pPlaySoundDlg->StartPlay(false);
            m_pPlaySoundDlg->ShowWindow(TRUE);
            break;
        case WM_TEL_INUSE:
            if (!m_bTelephoneInUse)
            {
                m_bTelephoneInUse = true;
                m_pPlaySoundDlg->CloseSound();
                m_pRecordSoundDlg->CloseSound();
            }
            break;
        case WM_TEL_NOUSE:
            m_bTelephoneInUse = false;
            break;
        case WM_LOCALRECORD:
            if ((((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pFSM->getCurrentState() == CMultimediaPhoneDlg::tsHangOff))
            {
                OnButtonRecord();
                m_pRecordSoundDlg->StartRecord();
            }
            break;
        case WM_PLAYLEAVEWORD:
            if ((((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pFSM->getCurrentState() == CMultimediaPhoneDlg::tsHangOff))
            {
                std::vector<Util::shared_ptr<Data::SoundSegment> > result = Data::SoundSegment::GetFromDatabase("type = 0 AND played = 0");	

                if (result.size() > 0)
                {
                    if(m_pPlaySoundDlg->SetSound(0, result[result.size() - 1]->id(), 0, m_sListSearchFilter))
                    {
                        if (((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pSettingDlg->m_pSetting->isPlayProtect() && !((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pSettingDlg->m_bLogin)
                        {
                            m_pPasswordDlg->SetType(CHECK_PASSWORD);
                            std::string strTemp = ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pSettingDlg->m_pSetting->playRecordPassword();
                            m_pPasswordDlg->SetOldPassWord((char *)strTemp.c_str());
                            m_pPasswordDlg->SetHWnd(this->m_hWnd);
                            m_pPasswordDlg->ShowWindow(SW_SHOW);	
                        }
                        else
                        {
                            m_pPlaySoundDlg->StartPlay();
                            m_pPlaySoundDlg->ShowWindow(TRUE);
                        }
                    }
                }
            }
            break;
        };
        return CDialog::WindowProc(message, wParam, lParam);
    }
}
