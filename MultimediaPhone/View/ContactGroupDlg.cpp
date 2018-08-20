// contactgroupdlg.cpp : implementation file
//

#include "stdafx.h"
#include "ContactGroupDlg.h"
//#include "ContactDlg.h"
#include "../MultimediaPhone.h"
#include "../MultimediaPhoneDlg.h"
#include "../Data/LanguageResource.h"
#include "../Data/SkinStyle.h"
//#include "sip.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern BOOL DetectDIR(TCHAR *sDir);

namespace View {
    /////////////////////////////////////////////////////////////////////////////
    // CContactGroupDlg dialog


    CContactGroupDlg::CContactGroupDlg(CWnd* pParent /*=NULL*/)
        : CDialog(CContactGroupDlg::IDD, pParent)
    {
        //{{AFX_DATA_INIT(CContactGroupDlg)
        // NOTE: the ClassWizard will add member initialization here
        //}}AFX_DATA_INIT
    }


    void CContactGroupDlg::DoDataExchange(CDataExchange* pDX)
    {
        CDialog::DoDataExchange(pDX);
        //{{AFX_DATA_MAP(CContactGroupDlg)
        // NOTE: the ClassWizard will add DDX and DDV calls here
        //}}AFX_DATA_MAP
    }


    BEGIN_MESSAGE_MAP(CContactGroupDlg, CDialog)
        //{{AFX_MSG_MAP(CContactGroupDlg)
        //}}AFX_MSG_MAP
        ON_BN_CLICKED(IDC_BUTTON_CONTACTGROUP_OK, OnButtonContactGroupOk)
        ON_BN_CLICKED(IDC_BUTTON_CONTACTGROUP_CANCEL, OnButtonContactGroupCancel)
        ON_BN_CLICKED(IDC_BUTTON_CONTACTGROUP_DELETE, OnButtonContactGroupDelete)
        ON_BN_CLICKED(IDC_SETTING_RINGSELECT, OnRingSelect)
        ON_NOTIFY(NM_CLICK, IDC_SETTING_LSTRING, OnRingLst)
        ON_MESSAGE(WM_CLICKMJPG_TOAPP, OnClickMJPG)
        ON_MESSAGE(WM_LISTCTRL_CLICK, OnListCtrlClick)
        ON_MESSAGE(WM_STOPTRYRING, OnStopTryRing)
    END_MESSAGE_MAP()

    LRESULT CContactGroupDlg::OnListCtrlClick(WPARAM w, LPARAM l)
    {
        LRESULT ret;
        if(w == IDC_SETTING_LSTRING)
            OnRingLst(NULL, &ret);
        return ret;
    }

    /////////////////////////////////////////////////////////////////////////////
    // CContactGroupDlg message handlers
    LRESULT CContactGroupDlg::OnClickMJPG(WPARAM w, LPARAM l)
    {
        LRESULT result = 0;
        if (w != 10)
            OnStopTryRing(0, 0);

        switch (w)
        {
        case 1:
            OnButtonContactGroupOk();
            break;
        case 2:
            OnButtonContactGroupCancel();
            break;
        case 3:
            OnButtonContactGroupDelete();
            break;
        case 10:
            {
                if (!m_MJPGList.GetUnitIsDownStatus(5))
                {
                    CString   str;  
                    m_ringEdit.GetWindowText(str);
                    if (str.Compare(Util::StringOp::ToCString(Data::LanguageResource::Get(Data::RI_CARD_DEFAULT))) != 0)
                    {
                        ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pMainDlg->m_mainMp3Dlg_->OnTimer(1002); //SendMessage(WM_OUTEVENT, 0, 0);

                        CString s = m_ringDir;
                        s += str;
                        ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->SetMsgWnd(this);
                        ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->StartRing((LPTSTR)(LPCTSTR)s, 1);
                        m_MJPGList.SetUnitIsDownStatus(5, TRUE);
                        m_MJPGList.SetUnitIsShow(5, TRUE);
                    }
                }
                else
                {
                    OnStopTryRing(0, 0);
                }
            }
            break;
        }
        return result;
    }

    LRESULT CContactGroupDlg::OnStopTryRing(WPARAM w, LPARAM l)
    {
        LRESULT result = 0;
        if (m_MJPGList.GetUnitIsDownStatus(5))
        {
            ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->StartRing(L"");
            m_MJPGList.SetUnitIsDownStatus(5, FALSE);
            m_MJPGList.SetUnitIsShow(5, TRUE);

            ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pMainDlg->m_mainMp3Dlg_->SendMessage(WM_OUTEVENT, 0, 1);
        }
        return result;
    }

    BOOL CContactGroupDlg::OnInitDialog() 
    {
        CDialog::OnInitDialog();

        // TODO: Add extra initialization here
        std::string strTemp;
        CString str;
        /*	
        strTemp = Data::LanguageResource::Get(Data::RI_CARD_TYPENAME);
        str = strTemp.c_str();
        m_sticName.Create(str, WS_CHILD|WS_VISIBLE, CRect(112, 69, 170, 87), this);
        m_sticName.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
        m_sticName.SetAlign(SS_CENTER);

        strTemp = Data::LanguageResource::Get(Data::RI_CARD_RING);
        str = strTemp.c_str();
        m_sticRing.Create(str, WS_CHILD|WS_VISIBLE, CRect(112, 90, 170, 106), this);
        m_sticRing.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
        m_sticRing.SetAlign(SS_CENTER);

        strTemp = Data::LanguageResource::Get(Data::RI_CARD_SOUNDTIP);
        str = strTemp.c_str();
        m_sticSoudTip.Create(str, WS_CHILD|WS_VISIBLE, CRect(112, 110, 170, 126), this);
        m_sticSoudTip.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
        m_sticSoudTip.SetAlign(SS_CENTER);
        */
        m_edtName.Create(WS_CHILD|WS_VISIBLE, CRect(194, 134, 446, 172), this, IDC_EDIT_CONTACTGROUP_NAME);
        m_edtName.SetLimitText(15);

        //m_cmbRing.Create(WS_CHILD|WS_VISIBLE, CRect(174, 90, 288, 200), this, IDC_COMBOBOX_CONTACTGROUP_RING);
        m_ringEdit.Create(WS_CHILD|WS_VISIBLE, CRect(194,195,449,232), this, 0xFFFF);
        m_ringEdit.SetIsAutoInput(FALSE);
        m_ringButton.Create(L"", Data::g_comboxBMPID[0][Data::g_skinstyle], Data::g_comboxBMPID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(450,194,508,233), this, IDC_SETTING_RINGSELECT);
        m_ringList.Create(WS_CHILD|LVS_REPORT|LVS_NOCOLUMNHEADER|LVS_NOSORTHEADER, CRect(194,244,508,410), this, IDC_SETTING_LSTRING, TRUE, 1);
        m_ringList.SetListColor(Data::g_listctrlBackRGB1[Data::g_skinstyle], Data::g_listctrlBackRGB2[Data::g_skinstyle]);
        m_ringList.InsertColumn(0, _T("Filename"), LVCFMT_LEFT, 280);
        m_ringList.ShowWindow_(SW_HIDE);

        m_pImageList1 = new CImageList();
        m_pImageList1->Create(32, 32, ILC_COLOR32|ILC_MASK, 6, 6);   
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

        m_ringStatic.Create(L"", WS_CHILD, CRect(192,242,516,412), this);
        m_ringStatic.SetBorder(TRUE);

        m_cmbSoundTip.Create(WS_CHILD|WS_VISIBLE, CRect(194, 255, 509, 410), this, IDC_COMBOBOX_CONTACTGROUP_SOUNDTIP);
        /*
        strTemp = Data::LanguageResource::Get(Data::RI_COMN_OKBTN);
        str = strTemp.c_str();
        m_btnOk.Create(str, Data::g_buttonArcBMPALLDIALOGID[0][Data::g_skinstyle], Data::g_buttonArcBMPALLDIALOGID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(415, 41, 471, 61), this, IDC_BUTTON_CONTACTGROUP_OK);
        m_btnOk.SetBackRGB(Data::g_allFramAngleBackLineRGB[Data::g_skinstyle]);

        strTemp = Data::LanguageResource::Get(Data::RI_COMN_CANCELBTN);
        str = strTemp.c_str();
        m_btnCancel.Create(str, Data::g_buttonArcBMPALLDIALOGID[0][Data::g_skinstyle], Data::g_buttonArcBMPALLDIALOGID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(415, 66, 471, 86), this, IDC_BUTTON_CONTACTGROUP_CANCEL);
        m_btnCancel.SetBackRGB(Data::g_allFramAngleBackLineRGB[Data::g_skinstyle]);

        strTemp = Data::LanguageResource::Get(Data::RI_CARD_DELETETYPEBTN);
        str = strTemp.c_str();
        m_btnDelete.Create(str, Data::g_buttonArcBMPALLDIALOGID[0][Data::g_skinstyle], Data::g_buttonArcBMPALLDIALOGID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(415, 91, 471, 111), this, IDC_BUTTON_CONTACTGROUP_DELETE);
        m_btnDelete.SetBackRGB(Data::g_allFramAngleBackLineRGB[Data::g_skinstyle]);

        m_sticBackground.Create(CRect(0, 0, 480 * 125 / 100, 204), this, 1);
        */
        m_MJPGList.Create(L"", WS_VISIBLE|WS_CHILD, CRect(0, 0, 800 * 125 / 100, 420 * 125 / 100), this);
        m_MJPGList.SetCurrentLinkFile(_T(".\\adv\\mjpg\\k1\\中文\\新建类别.xml"));
        m_MJPGList.SetMJPGRect(CRect(0, 0, 800 * 125 / 100, 420 * 125 / 100));

        m_pContactGroup = Util::shared_ptr<Data::ContactGroup>();
        return TRUE;  // return TRUE unless you set the focus to a control
        // EXCEPTION: OCX Property Pages should return FALSE
    }

    void CContactGroupDlg::SetButtonDefaultColor(CCEButtonST* button)
    {
        button->SetColor(CCEButtonST::BTNST_COLOR_BK_IN, RGB(248,214,147));
        button->SetColor(CCEButtonST::BTNST_COLOR_BK_OUT, RGB(248,214,147));
        button->SetColor(CCEButtonST::BTNST_COLOR_BK_FOCUS, RGB(248,214,147));
    }

    void CContactGroupDlg::SetButtonSelectedColor(CCEButtonST* button)
    {
        button->SetColor(CCEButtonST::BTNST_COLOR_BK_IN, RGB(248,214,147));
        button->SetColor(CCEButtonST::BTNST_COLOR_BK_OUT, RGB(248,214,147));
        button->SetColor(CCEButtonST::BTNST_COLOR_BK_FOCUS, RGB(248,214,147));
    }

    void CContactGroupDlg::AddRingTipItems(void)
    {

    }

    void CContactGroupDlg::OnButtonContactGroupOk() 
    {
        // TODO: Add your control notification handler code here
        CString s;
        m_edtName.GetWindowText(s);
        if (s.IsEmpty())
        {
            return;
        }

        m_ringList.ShowWindow_(SW_HIDE);
        m_ringStatic.ShowWindow(SW_HIDE);
        ShowWindow(FALSE);
        //SipShowIM(SIPF_OFF);

        if (!m_pContactGroup)
        {
            m_pContactGroup = Util::shared_ptr<Data::ContactGroup>(new Data::ContactGroup);
        }

        std::string name = Util::StringOp::FromCString(s);
        for (std::size_t i = 0; i < name.length(); ++i)
        {
            if (name[i] == '\'')
            {
                name[i] = '\"';
            }
        }

        m_pContactGroup->name(name);

        CString   str;  
        m_ringEdit.GetWindowText(str);

        /*
        if (str.GetLength() == 0)
        {
        str = Data::LanguageResource::Get(Data::RI_CARD_DEFAULT).c_str();
        }
        */
        if(str.Compare(Util::StringOp::ToCString(Data::LanguageResource::Get(Data::RI_CARD_DEFAULT))) != 0)
        {
            CString s = m_ringDir;
            s += str;
            str = s;
        }
        //str  --------->ringname

        m_pContactGroup->ringName(Util::StringOp::FromCString(str));


        m_cmbSoundTip.GetLBText(m_cmbSoundTip.GetCurSel(), str);

        int index = m_cmbSoundTip.GetItemData(m_cmbSoundTip.GetCurSel());
        if (index == 0)
        {
            str = csFlashRecordPath + str;
        }
        else if (index == 1)
        {
            str = csStorageCardRecordPath + str;
        }
        m_pContactGroup->tipName(Util::StringOp::FromCString(str));

        bool isInsert = false;
        if (m_pContactGroup->id() == 0)
        {
            m_pContactGroup->Insert();
            isInsert = true;
        }
        else
        {
            m_pContactGroup->Update();
        }
        int sel = ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactDlg->GetTypeListSelected();
        ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactDlg->ShowTypeItems();

        if ((sel > 0) && isInsert)
        {
            ++sel;
        }
        ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactDlg->SetTypeListSelected(sel);
        ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactDlg->ShowItemsInList();

        m_ringList.ShowWindow_(SW_HIDE);
        m_ringStatic.ShowWindow(SW_HIDE);
    }

    void CContactGroupDlg::OnButtonContactGroupCancel() 
    {
        // TODO: Add your control notification handler code here
        m_ringList.ShowWindow_(SW_HIDE);
        m_ringStatic.ShowWindow(SW_HIDE);
        ShowWindow(FALSE);
        //SipShowIM(SIPF_OFF);
    }

    void CContactGroupDlg::OnButtonContactGroupDelete() 
    {
        // TODO: Add your control notification handler code here
        if (m_pContactGroup->id() > 0)
        {
            std::string strTemp = Data::LanguageResource::Get(Data::RI_DELETETIP_CONTACTGROUP);
            ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetDelTip(Util::StringOp::ToCString(strTemp.c_str()));
            ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetHWnd(m_hWnd);
            ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->ShowWindow(TRUE);
        }

        m_ringList.ShowWindow_(SW_HIDE);
        m_ringStatic.ShowWindow(SW_HIDE);
    }

    void CContactGroupDlg::SetContactGroup(Util::shared_ptr<Data::ContactGroup> contactgroup)
    {
        m_pContactGroup = contactgroup;
        //	AddRingTipItems();
        std::string strTemp;
        if (contactgroup->id() == 0)
        {
            //		strTemp = Data::LanguageResource::Get(Data::RI_CARD_NEWTYPEBTN);
            // 		CString str = strTemp.c_str();
            // 		m_sticTitle.SetWindowText(str);
            m_edtName.SetWindowText(_T(""));
            //		m_btnDelete.ShowWindow(FALSE);
            m_MJPGList.SetUnitIsShow(3, TRUE);
            m_MJPGList.SetUnitIsShow(4, FALSE);
        }
        else
        {
            //		strTemp = Data::LanguageResource::Get(Data::RI_CARD_EDITTYPEBTN);
            // 		CString str = strTemp.c_str();
            // 		m_sticTitle.SetWindowText(str);
            m_edtName.SetWindowText(Util::StringOp::ToCString(contactgroup->name()));
            //		m_btnDelete.ShowWindow(TRUE);
            m_MJPGList.SetUnitIsShow(4, TRUE);
            m_MJPGList.SetUnitIsShow(3, FALSE);
        }

        /*
        TextStruct ts[1];
        memset(ts, 0, sizeof(TextStruct) * 1);

        ts[0].txtRect = CRect(8, 0, 100, 20);
        ts[0].txtFontSize = 16;
        ts[0].sAlign = DT_LEFT | DT_BOTTOM;
        memcpy(ts[0].sTxt, strTemp.c_str(), strTemp.length());

        m_sticBackground.SetTextStruct(ts, 1);
        */
        CString ringName = Util::StringOp::ToCString(contactgroup->ringName()) ;

        memset(m_ringDir, 0, 128*2);
        if (ringName.GetLength() > 0)
        {
            if(ringName.Compare(Util::StringOp::ToCString(Data::LanguageResource::Get(Data::RI_CARD_DEFAULT))) == 0)
                m_ringEdit.SetWindowText(ringName);
            else
            {
                CString s = ringName;
                int nStart = 0;
                int n = s.Find(_T("/"), nStart);
                while(n != -1)
                {	nStart = n+1;
                n = s.Find(_T("/"), nStart);
                }
                CString filename = s.Mid(nStart);
                CString path = s.Mid(0, nStart);
                memcpy(m_ringDir, path.GetBuffer(256), path.GetLength()*2);
                m_ringEdit.SetWindowText(filename);
            }
        }
        else
        {
            m_ringEdit.SetWindowText(Util::StringOp::ToCString(Data::LanguageResource::Get(Data::RI_CARD_DEFAULT)));
        }

        m_cmbSoundTip.ResetContent();
        m_cmbSoundTip.AddString(Util::StringOp::ToCString(Data::LanguageResource::Get(Data::RI_CARD_DEFAULT)));	
        m_cmbSoundTip.SetItemData(0, -1);
        std::vector<Util::shared_ptr<Data::SoundSegment> > sounds = Data::SoundSegment::GetFromDatabase("type = 2 AND isTip = 1");
        int pos = 0;
        for (std::size_t i = 0; i < sounds.size(); ++i)
        {
            pos = sounds[i]->filename().rfind('\\');
            m_cmbSoundTip.AddString(Util::StringOp::ToCString(sounds[i]->filename().substr(pos + 1)));
            if (sounds[i]->filename().substr(0, pos + 1) == ssFlashRecordPath)
            {
                m_cmbSoundTip.SetItemData(i + 1, 0);
            }
            else if(sounds[i]->filename().substr(0, pos + 1) == "\\flashdrv\\MY_RECORD\\")
            {
                m_cmbSoundTip.SetItemData(i + 1, 0);
            }
            else if(sounds[i]->filename().substr(0, pos + 1) == "\\flashdrv\\my_record\\")
            {
                m_cmbSoundTip.SetItemData(i + 1, 0);
            }
            else
            {
                m_cmbSoundTip.SetItemData(i + 1, 1);
            }
        }

        pos = contactgroup->tipName().rfind('\\');
        strTemp = contactgroup->tipName().substr(pos + 1);

        int index = m_cmbSoundTip.SelectString(0, Util::StringOp::ToCString(strTemp));
        if (index == CB_ERR)
        {
            //		m_cmbSoundTip.SelectString(0, Util::StringOp::ToCString(Data::LanguageResource::Get(Data::RI_CARD_DEFAULT)));
            m_cmbSoundTip.SetCurSel(0);
        }
    }

    void CContactGroupDlg::OnRingLst(NMHDR* pNMHDR, LRESULT* pResult)
    {
        POSITION pos = m_ringList.GetFirstSelectedItemPosition();
        if (pos != NULL)
        {
            int index = m_ringList.GetNextSelectedItem (pos);
            CString s;
            CString s1 = Util::StringOp::ToCString(Data::LanguageResource::Get(Data::RI_COMN_TOBOTTOM));
            s = m_ringList.GetItemText(index, 0);
            if(s.Compare(_T("storagecard")) == 0)
            {
                SetRingLst(_T("/StorageCard/"));
            }
            //默认
            else if(s == Util::StringOp::ToCString(Data::LanguageResource::Get(Data::RI_CARD_DEFAULT)))
            {
                m_ringEdit.SetWindowText(s);
                m_ringList.ShowWindow_(SW_HIDE);
                m_ringStatic.ShowWindow(SW_HIDE);
            }

            //上一级
            else if(s.Compare(s1) == 0)
            {
                //	m_chDir
                CString sDir = m_ringDir;
                int n = sDir.Find(_T("/"));
                ::UINT8 old[16];
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

        *pResult = 0;
    }

    void CContactGroupDlg::OnRingSelect()
    {
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
    }

    void CContactGroupDlg::SetRingLst(TCHAR *dir)
    {
        memset(m_ringDir, 0, 128*2);
        memcpy(m_ringDir, dir, wcslen(dir)*2);
        m_ringList.DeleteAllItems();
        m_ringList.SetScrollPos(0, TRUE);
        int ncount = 0;
        if(wcslen(dir) == 0)
        {
            if (::DetectDIR((LPTSTR)(LPCTSTR)csStarageCard))
                m_ringList.InsertItem(ncount++, _T("storagecard"), 3);
            m_ringList.InsertItem(ncount++, Util::StringOp::ToCString(Data::LanguageResource::Get(Data::RI_CARD_DEFAULT)), 5);
            memcpy(m_ringDir, (TCHAR*)(LPCTSTR)::ringPath,  ::ringPath.GetLength() * 2);
        }

        else
        {
            CString s = Util::StringOp::ToCString(Data::LanguageResource::Get(Data::RI_COMN_TOBOTTOM));
            m_ringList.InsertItem(ncount++, s, 4);
            memcpy(m_ringDir, dir, wcslen(dir)*2);
        }

        CString sDir = m_ringDir;
        WIN32_FIND_DATA FindFileData;			//查找文件时要使用的数据结构
        HANDLE hFind = INVALID_HANDLE_VALUE;	//定义查找句柄

        sDir += "*.*";
        hFind = FindFirstFile(sDir, &FindFileData);//使用FindFirstFile函数来开始文件查找

        if (hFind == INVALID_HANDLE_VALUE) 
        {
            return;
        } 
        else 
        {
            char filename[128];
            int i = wcstombs( filename, FindFileData.cFileName, 128);
            filename[i] = '\0';
            if(strstr(filename, ".mp3")||strstr(filename, ".MP3")||strstr(filename, ".wav")||strstr(filename, ".WAV"))
            {
                m_ringList.InsertItem(ncount, FindFileData.cFileName, 5);
                m_ringList.SetItemData(ncount++, 0);
            }
            else if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
                CString sS;
                CString s = Util::StringOp::ToCString(Data::LanguageResource::Get(Data::RI_COMN_TOBOTTOM).c_str());
                sS = m_ringList.GetItemText(0, 0);
                if(sS == s)
                    m_ringList.InsertItem(1, FindFileData.cFileName, 2);
                else 
                    m_ringList.InsertItem(0, FindFileData.cFileName, 2);
                ncount++;
            }

            //以下是循环使用FindNextFile函数来查找文件
            while (FindNextFile(hFind, &FindFileData) != 0) 
            {
                i = wcstombs( filename, FindFileData.cFileName, /*wcslen(FindFileData.cFileName)*/128);
                filename[i] = '\0';
                if(strstr(filename, ".mp3")||strstr(filename, ".MP3")||strstr(filename, ".wav")||strstr(filename, ".WAV"))
                {
                    m_ringList.InsertItem(ncount, FindFileData.cFileName, 5);
                    m_ringList.SetItemData(ncount++, 0);
                }
                else if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
                {
                    CString sS;
                    CString s = Util::StringOp::ToCString(Data::LanguageResource::Get(Data::RI_COMN_TOBOTTOM).c_str());
                    sS = m_ringList.GetItemText(0, 0);
                    if(sS == s)
                        m_ringList.InsertItem(1, FindFileData.cFileName, 2);
                    else 
                        m_ringList.InsertItem(0, FindFileData.cFileName, 2);
                    ncount++;
                }
            }

            DWORD dwError = GetLastError();
            if (dwError == ERROR_NO_MORE_FILES) 
            {
                FindClose(hFind);//关闭查找句柄
            } 
            else 
            {
                return;
            }
        }
        m_ringList.SetScrollRagle(TRUE);
        return;
    }

    LRESULT CContactGroupDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
    {
        // TODO: Add your specialized code here and/or call the base class
        switch (message)
        {
        case WM_DELETESELITEM:
            {
                std::string filter = "groupId = " + Util::StringOp::FromInt(m_pContactGroup->id());
                Data::Contact::Remove(filter);

                m_pContactGroup->Remove();
                int sel = ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactDlg->GetTypeListSelected();
                ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactDlg->ShowTypeItems();		
                ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactDlg->SetTypeListSelected(sel);
                ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactDlg->ShowItemsInList();
                ShowWindow(FALSE);
            }
            break;
        }
        return CDialog::WindowProc(message, wParam, lParam);
    }
}
