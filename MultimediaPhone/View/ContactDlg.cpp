// ContactDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ContactDlg.h"
#include "../MultimediaPhone.h"
#include "../Data/LanguageResource.h"
#include "../MultimediaPhoneDlg.h"
#include "../Data/ContactGroup.h"
#include "../Data/SkinStyle.h"

#include "SIMImportDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace View {
    /////////////////////////////////////////////////////////////////////////////
    // CContactDlg dialog

    CContactDlg::CContactDlg(CWnd* pParent /*=NULL*/)
        : CDialog(CContactDlg::IDD, pParent)
        , ContactTotal(10000)
        , ContactGroupTotal(8)
        , PageSize(8)
    {
        //{{AFX_DATA_INIT(CContactDlg)
        // NOTE: the ClassWizard will add member initialization here
        //}}AFX_DATA_INIT
        m_pSimImportDlg = NULL;
    }


    void CContactDlg::DoDataExchange(CDataExchange* pDX)
    {
        CDialog::DoDataExchange(pDX);
        //{{AFX_DATA_MAP(CContactDlg)
        // NOTE: the ClassWizard will add DDX and DDV calls here
        //}}AFX_DATA_MAP
    }


    BEGIN_MESSAGE_MAP(CContactDlg, CDialog)
        //{{AFX_MSG_MAP(CContactDlg)
        //}}AFX_MSG_MAP
        ON_NOTIFY(NM_CLICK, IDC_LIST_CONTACT_TYPE, OnClickListType)
        ON_NOTIFY(NM_CLICK, IDC_LIST_CONTACT_LIST, OnClickListList)
        ON_BN_CLICKED(IDC_BUTTON_CONTACT_NEW, OnButtonContactNew)
        ON_BN_CLICKED(IDC_BUTTON_CONTACT_NEWTYPE, OnButtonContactNewtype)
        ON_BN_CLICKED(IDC_BUTTON_CONTACT_EDITTYPE, OnButtonContactEdittype)
        ON_BN_CLICKED(IDC_BUTTON_CONTACT_SEARCH, OnButtonContactSearch)
        ON_BN_CLICKED(IDC_BUTTON_CONTACT_CLOSE, OnButtonContactClose)
        ON_MESSAGE(WM_SOFTKEY, OnSoftKeyMessage)
        ON_MESSAGE(WM_CLICKMJPG_TOAPP, OnClickMJPG)
        ON_MESSAGE(WM_LISTCTRL_CLICK, OnListCtrlClick)
        ON_WM_TIMER()
    END_MESSAGE_MAP()

    LRESULT CContactDlg::OnListCtrlClick(WPARAM w, LPARAM l)
    {
        LRESULT result = 0;
        if(w == IDC_LIST_CONTACT_TYPE)
            OnClickListType(NULL, &result);
        else if(w == IDC_LIST_CONTACT_LIST)
            OnClickListList(NULL, &result);
        return result;
    }

    /////////////////////////////////////////////////////////////////////////////
    // CContactDlg message handlers
    LRESULT CContactDlg::OnClickMJPG(WPARAM w, LPARAM l)
    {
        LRESULT result = 0;
        switch (w)
        {
        case 1:
            OnButtonContactNew();
            break;
        case 2:
            OnButtonContactNewtype();
            break;
        case 3:
            OnButtonContactEdittype();
            break;
        case 4:
            OnButtonContactSearch();
            break;
        case 5:						//sim卡导入
            //m_pSimImportDlg->ShowWindow(SW_SHOW);
            break;
        case 6:						//关闭
            OnButtonContactClose();
            break;
        }
        return result;
    }

    BOOL CContactDlg::OnInitDialog() 
    {
        CDialog::OnInitDialog();

        // TODO: Add extra initialization here
        std::string strTemp;
        CString str;

		m_MJPGList.Create(L"", WS_VISIBLE|WS_CHILD, CRect(0, 0, 800*X_XISHU, 420*Y_XISHU), this);
        m_MJPGList.SetCurrentLinkFile(_T(".\\adv\\mjpg\\k1\\中文\\通讯录.xml"));
        m_MJPGList.SetMJPGRect(CRect(0, 0, 800*X_XISHU, 420*Y_XISHU));

        m_lsType.m_nLineHeight = 30;
        m_lsType.Create(WS_CHILD|WS_VISIBLE|LVS_REPORT|LVS_NOCOLUMNHEADER|LVS_NOSORTHEADER, CRect(1*X_XISHU, 46*Y_XISHU, 112*X_XISHU, 348*Y_XISHU), &m_MJPGList, IDC_LIST_CONTACT_TYPE, FALSE);
        m_lsType.SetExtendedStyle(m_lsType.GetExtendedStyle()|LVS_EX_FULLROWSELECT);
        m_lsType.InsertColumn(0, _T("Type"), LVCFMT_LEFT, 110*X_XISHU);

        m_pImageList = new CImageList();
        m_pImageList->Create(32, 32, ILC_COLOR32|ILC_MASK, 12, 2);   

        CBitmap bm;
        bm.LoadBitmap(IDB_BITMAP5);
        m_pImageList->Add(&bm, RGB(255, 0, 255));
        bm.DeleteObject();
        bm.LoadBitmap(IDB_BITMAP1);
        m_pImageList->Add(&bm, RGB(255, 0, 255));
        bm.DeleteObject();
        bm.LoadBitmap(IDB_BITMAP9);
        m_pImageList->Add(&bm, RGB(255, 0, 255));
        bm.DeleteObject();
        bm.LoadBitmap(IDB_BITMAP10);
        m_pImageList->Add(&bm, RGB(255, 0, 255));
        bm.DeleteObject();
        bm.LoadBitmap(IDB_BITMAP11);
        m_pImageList->Add(&bm, RGB(255, 0, 255));
        bm.DeleteObject();
        bm.LoadBitmap(IDB_BITMAP12);
        m_pImageList->Add(&bm, RGB(255, 0, 255));
        bm.DeleteObject();
        bm.LoadBitmap(IDB_BITMAP13);
        m_pImageList->Add(&bm, RGB(255, 0, 255));
        bm.DeleteObject();
        bm.LoadBitmap(IDB_BITMAP14);
        m_pImageList->Add(&bm, RGB(255, 0, 255));
        bm.DeleteObject();
        bm.LoadBitmap(IDB_BITMAP15);
        m_pImageList->Add(&bm, RGB(255, 0, 255));
        bm.DeleteObject();
        bm.LoadBitmap(IDB_BITMAP16);
        m_pImageList->Add(&bm, RGB(255, 0, 255));
        bm.DeleteObject();
        bm.LoadBitmap(IDB_BITMAP17);
        m_pImageList->Add(&bm, RGB(255, 0, 255));
        bm.DeleteObject();
        bm.LoadBitmap(IDB_BITMAP18);
        m_pImageList->Add(&bm, RGB(255, 0, 255));
        bm.DeleteObject();

        //	m_lsType.SetImageList(m_pImageList, LVSIL_SMALL);

        m_lsList.Create(WS_CHILD|WS_VISIBLE|LVS_REPORT|LVS_NOCOLUMNHEADER|LVS_NOSORTHEADER, CRect(115*X_XISHU, 46*Y_XISHU, (115+570)*X_XISHU, (46+300)*Y_XISHU), &m_MJPGList, IDC_LIST_CONTACT_LIST, TRUE, 0, FALSE);
        m_lsList.SetExtendedStyle(m_lsList.GetExtendedStyle()|LVS_EX_FULLROWSELECT);
        m_lsList.InsertColumn(0, _T("Name"), LVCFMT_LEFT, (120+22+34)*X_XISHU);
        m_lsList.InsertColumn(1, _T("Company"), LVCFMT_LEFT, (180+60)*X_XISHU);
        //	m_lsList.InsertColumn(2, _T("Department"), LVCFMT_LEFT, 120);
        m_lsList.InsertColumn(2, _T("Duty"), LVCFMT_LEFT, (150-22/*+26*/)*X_XISHU);

        m_pImageList = new CImageList();
        m_pImageList->Create(32, 32, ILC_COLOR32|ILC_MASK, 3, 2);   
        bm.LoadBitmap(IDB_BITMAP8);
        m_pImageList->Add(&bm, RGB(255, 0, 255));
        bm.DeleteObject();
        bm.LoadBitmap(IDB_BITMAP7);
        m_pImageList->Add(&bm, RGB(255, 0, 255));
        bm.DeleteObject();
        bm.LoadBitmap(IDB_BITMAP6);
        m_pImageList->Add(&bm, RGB(255, 0, 255));
        bm.DeleteObject();

        m_lsList.SetImageList(m_pImageList, LVSIL_SMALL);

        CRect rectKey[29];
        ::UINT16 uiKey[29];
        int offsetX = 0;
        int offsetY = 0;
        for (int j = 0; j < 26; ++j)
        {	
            if (j > 12)
            {
                offsetX = ((j - 13) * 50)*X_XISHU;
                offsetY = 37*Y_XISHU;
            }
            else
            {
                offsetX = (j * 50)*X_XISHU;
            }
            rectKey[j] = CRect(150 *X_XISHU+ offsetX, offsetY, 200*X_XISHU + offsetX, offsetY + 37*Y_XISHU);
            uiKey[j] = 'a' + j;
        };
        rectKey[26] = CRect(0*X_XISHU, 37*Y_XISHU, 50*X_XISHU, 74*Y_XISHU);
        uiKey[26] = 0x7F;
        rectKey[27] = CRect(50*X_XISHU, 37*Y_XISHU, 100*X_XISHU, 74*Y_XISHU);
        uiKey[27] = 0x00;
        rectKey[28] = CRect(100*X_XISHU, 37*Y_XISHU, 150*X_XISHU, 74*Y_XISHU);
        uiKey[28] = '\?';

        m_sticPinYin.Create(_T(""), WS_CHILD|WS_VISIBLE, CRect(70*X_XISHU, 348*Y_XISHU, 148*X_XISHU, 380*Y_XISHU), &m_MJPGList, IDC_STATIC_CONTACT_PINYIN);
        m_sticPinYin.SetColor(RGB(0, 0, 0), RGB(173, 227, 251));
        //m_sticPinYin.SetLeftMargin(10);
        m_sticPinYin.SetFontSize(26*X_XISHU);
        m_skSoftKey.Create(IDB_VOIPPYSEARCH, CPoint(0,0), 29, rectKey, uiKey, CRect(0*X_XISHU, 346*Y_XISHU, 800*X_XISHU, 420*Y_XISHU),&m_MJPGList,WS_CHILD|WS_VISIBLE, IDC_SOFTKEY_CONTACT_PYSEARCH);


        m_pContactSearchDlg = new CContactSearchDlg(this);
        m_pContactSearchDlg->Create(CContactSearchDlg::IDD);

        ShowTypeItems();
        m_lsType.SetItemState(0, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);

        m_sListFilter = "";
        m_sListSearchFilter = "";	
        ShowItemsInList();

        m_pSimImportDlg = new CSIMImportDlg(this);
        m_pSimImportDlg->Create(CSIMImportDlg::IDD);
        m_pSimImportDlg->ShowWindow(SW_HIDE);

        return TRUE;  // return TRUE unless you set the focus to a control
        // EXCEPTION: OCX Property Pages should return FALSE
    }

    void CContactDlg::SetButtonDefaultColor(CCEButtonST* button)
    {
        button->SetColor(CCEButtonST::BTNST_COLOR_BK_IN, RGB(248,214,147));
        button->SetColor(CCEButtonST::BTNST_COLOR_BK_OUT, RGB(248,214,147));
        button->SetColor(CCEButtonST::BTNST_COLOR_BK_FOCUS, RGB(248,214,147));
    }

    void CContactDlg::SetButtonSelectedColor(CCEButtonST* button)
    {
        button->SetColor(CCEButtonST::BTNST_COLOR_BK_IN, RGB(248,214,147));
        button->SetColor(CCEButtonST::BTNST_COLOR_BK_OUT, RGB(248,214,147));
        button->SetColor(CCEButtonST::BTNST_COLOR_BK_FOCUS, RGB(248,214,147));
    }

    void CContactDlg::OnClickListType(NMHDR* pNMHDR, LRESULT* pResult) 
    {
        // TODO: Add your control notification handler code here

        m_MJPGList.GetDlgItem(IDC_STATIC_CONTACT_PINYIN)->SetWindowText(_T(""));
        ShowItemsInList();

        *pResult = 0;
    }

    void CContactDlg::ShowTypeItems(void)
    {
        m_lsType.DeleteAllItems();
        int imageID = 0;
        std::string strTemp = Data::LanguageResource::Get(Data::RI_CARD_ALLCARDSTC);
        CString str = Util::StringOp::ToCString(strTemp);
        m_lsType.InsertItem(0, str, imageID);
        m_lsType.SetItemData(0, 0);
        imageID = 2;
        std::vector<Util::shared_ptr<Data::ContactGroup> > result;
        if (Data::ContactGroup::GetDataCount("") > 0)
        {
            result = Data::ContactGroup::GetFromDatabase("");
            for (std::size_t i = 0; i < result.size(); ++i)
            {
                m_lsType.InsertItem(i + 1, Util::StringOp::ToCString(result[i]->name())/*, imageID*/);
                m_lsType.SetItemData(i + 1, result[i]->id());
                ++imageID;
                if (imageID > 10)
                {
                    imageID = 2;
                }
            }
        }
        strTemp = Data::LanguageResource::Get(Data::RI_CARD_FINDREAULTSTC);
        str = Util::StringOp::ToCString(strTemp);
        m_lsType.InsertItem(result.size() + 1, str/*, 1*/);
        m_lsType.SetItemData(result.size() + 1, result.size() + 1);
        m_lsType.SetScrollRagle(TRUE);
    }

    void CContactDlg::ShowItemsInList()
    {
        POSITION pos = m_lsType.GetFirstSelectedItemPosition();
        if (pos != NULL)
        {
            m_lsList.DeleteAllItems();

            m_sListFilter = "";

            std::string filter;
            std::string aliasFilter;
            std::string alias = GetPYIndex();
            if (alias != "")
            {
                aliasFilter = "alias LIKE '";
                aliasFilter += alias;
                aliasFilter += "%'";
            }

            int type = m_lsType.GetNextSelectedItem(pos);
            if (type == 0)
            {
                filter = aliasFilter;
            }
            else if (type == m_lsType.GetItemCount() - 1)
            {
                if (m_sListSearchFilter == "")
                    return;

                filter = m_sListSearchFilter;
                if (aliasFilter != "")
                {
                    filter += " AND ";
                    filter += aliasFilter;
                }

            }
            else
            {
                m_sListFilter = "groupId = " + Util::StringOp::FromInt(m_lsType.GetItemData(type));
                filter = m_sListFilter;
                if (aliasFilter != "")
                {
                    filter += " AND ";
                    filter += aliasFilter;
                }
            }	

            if (Data::Contact::GetDataCount("") > 0)
            {
                //m_vCurrentResult = Data::Contact::GetFromDatabase(filter, "id", Data::srDesc, Data::Contact::GetCurrentId() + 1, PageSize);
                m_vCurrentResult = Data::Contact::GetFromDatabase(filter, "id", Data::srDesc, 0, PageSize);
                int count = Data::Contact::GetDataCount(filter);
                m_lsList.m_scollbar_.SetRangle(0, count, PageSize);
                ShowArrayInList(m_vCurrentResult);
                m_lsList.SetScrollRagle(TRUE);
            }
        }
    }

    void CContactDlg::ScrollItemsInList(int step, int nPos)
    {
        if (m_vCurrentResult.empty())
        {
            return;
        }

        std::string filter = m_sListFilter;
        POSITION pos = m_lsType.GetFirstSelectedItemPosition();
        if (pos != NULL)
        {
            int type = m_lsType.GetNextSelectedItem(pos);
            if (type == m_lsType.GetItemCount() - 1)
            {
                filter = m_sListSearchFilter;
            }
        }

        std::string alias = GetPYIndex();
        if (alias != "")
        {
            if (filter != "")
            {
                filter += " AND ";
            }
            filter = "alias LIKE '";
            filter += alias;
            filter += "%'";
        }

        m_vCurrentResult.clear();
        m_vCurrentResult = Data::Contact::GetFromDatabase(filter, "id", Data::srDesc, nPos, PageSize);

        ShowArrayInList(m_vCurrentResult);
    }

    void CContactDlg::ShowArrayInList(std::vector<Util::shared_ptr<Data::Contact> > array)
    {
        m_lsList.DeleteAllItems();
        //lxz 090224
#if 0
        for (std::size_t i = 0; i < 10; ++i)
        {	
            std::string s = "test";
            CString temp = Util::StringOp::ToCString(s);	
            m_lsList.InsertItem(i, temp, 0);
            temp = Util::StringOp::ToCString(s);
            m_lsList.SetItemText(i, 1, temp);
            temp = Util::StringOp::ToCString(s);
            m_lsList.SetItemText(i, 2, temp);

            m_lsList.SetItemData(i, 0);
        }
#else
        CString temp;
        for (std::size_t i = 0; i < (array).size(); ++i)
        {	
            temp = Util::StringOp::ToCString(array[i]->name());	
            m_lsList.InsertItem(i, temp, array[i]->type());
            temp = Util::StringOp::ToCString(array[i]->company());
            m_lsList.SetItemText(i, 1, temp);
            temp = Util::StringOp::ToCString(array[i]->duty());
            m_lsList.SetItemText(i, 2, temp);

            m_lsList.SetItemData(i, array[i]->id());
        }
#endif
    }

    void CContactDlg::OnClickListList(NMHDR* pNMHDR, LRESULT* pResult) 
    {
        // TODO: Add your control notification handler code here
        //由于第一次点击名片列表经常不能显示名片详细信息，启动一个定时器，在定时器中处理.
        SetTimer(1, 50, NULL);
        *pResult = 0;
    }

    void CContactDlg::OnTimer(UINT nIDEvent)
    {
        if(nIDEvent == 1)
        {
            KillTimer(1);
            POSITION pos = m_lsList.GetFirstSelectedItemPosition();
            if (pos != NULL)
            {
                int index = m_lsList.GetNextSelectedItem (pos);
                int id = m_lsList.GetItemData(index);
                std::vector<Util::shared_ptr<Data::Contact> > result = Data::Contact::GetFromDatabase("id = " + Util::StringOp::FromInt(id));
                if (!result.empty())
                {
                    ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactNewDlg->SetContact(result[0]);
                    ::SetFocus(((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactNewDlg->m_hWnd);
                    ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactNewDlg->ShowWindow(TRUE);
                }
            }
        }
        CDialog::OnTimer(nIDEvent);
    }

    void CContactDlg::OnButtonContactNew() 
    {
        // TODO: Add your control notification handler code here
        if (Data::Contact::GetDataCount("") < ContactTotal)
        {
            Util::shared_ptr<Data::Contact> contact(new Data::Contact);
            contact->id(0);
            ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactNewDlg->SetContact(contact);
            ::SetFocus(((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactNewDlg->m_hWnd);
            ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactNewDlg->ShowWindow(TRUE);
        }
        else
        {
            ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pWarningNoFlashDlg->ShowWindow_(SW_SHOW);
        }
    }

    void CContactDlg::OnButtonContactNewtype() 
    {
        // TODO: Add your control notification handler code here
        if (Data::ContactGroup::GetDataCount("") < ContactGroupTotal)
        {
            Util::shared_ptr<Data::ContactGroup> contactgroup(new Data::ContactGroup);
            contactgroup->id(0);
            ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactGroupDlg->SetContactGroup(contactgroup);
            ::SetFocus(((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactGroupDlg->m_hWnd);
            ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactGroupDlg->ShowWindow(TRUE);
        }
        else
        {
            ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pWarningNoFlashDlg->ShowWindow_(SW_SHOW);
        }
    }

    void CContactDlg::OnButtonContactEdittype() 
    {
        // TODO: Add your control notification handler code here
        POSITION pos = m_lsType.GetFirstSelectedItemPosition();
        if (pos != NULL)
        {
            int index = m_lsType.GetNextSelectedItem (pos);
            if ((index == 0) || (index == m_lsType.GetItemCount() - 1))
            {
                return;
            }

            int id = m_lsType.GetItemData(index);
            std::vector<Util::shared_ptr<Data::ContactGroup> > result = Data::ContactGroup::GetFromDatabase("id = " + Util::StringOp::FromInt(id));
            if (!result.empty())
            {
                ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactGroupDlg->SetContactGroup(result[0]);
                ::SetFocus(((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactGroupDlg->m_hWnd);
                ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactGroupDlg->ShowWindow(TRUE);
            }
        }
    }

    void CContactDlg::OnButtonContactSearch() 
    {
        // TODO: Add your control notification handler code here
        m_pContactSearchDlg->ShowWindow(TRUE);
    }

    void CContactDlg::OnButtonContactClose()
    {
        ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->SwitchPanel_(IDC_BUTTON_MAIN);
    }

    LRESULT CContactDlg::OnSoftKeyMessage(WPARAM wParam, LPARAM lParam)
    {
        //@@ error for multi char
        TCHAR key = wParam;
        CString py(key);
        //CString py;
        switch (key)
        {
        case 0x7F://del
            m_MJPGList.GetDlgItem(IDC_STATIC_CONTACT_PINYIN)->GetWindowText(py);
            m_MJPGList.GetDlgItem(IDC_STATIC_CONTACT_PINYIN)->SetWindowText(py.Left(py.GetLength() - 1));
            break;
        case 0x00://clear
            m_MJPGList.GetDlgItem(IDC_STATIC_CONTACT_PINYIN)->SetWindowText(_T(""));
            break;
        default:
            m_MJPGList.GetDlgItem(IDC_STATIC_CONTACT_PINYIN)->GetWindowText(py);
            if (py.GetLength() >= 6)
            {
                m_MJPGList.GetDlgItem(IDC_STATIC_CONTACT_PINYIN)->SetWindowText(L"");
                py = _T("");
                return 0;
            }
            //GetDlgItem(IDC_STATIC_CONTACT_PINYIN)->SetWindowText(py + key);
            CString s_;
            if (py != "")
                s_.Format(_T("%s%c"), py, key);
            else
                s_.Format(_T("%c"), key);
            m_MJPGList.GetDlgItem(IDC_STATIC_CONTACT_PINYIN)->SetWindowText(s_);
            break;
        }
        
        ShowItemsInList();
        m_sticPinYin.SendMessage(WM_PAINT, 0, 0);
        return 0;
    }

    std::string CContactDlg::GetPYIndex(void)
    {
        std::string alias;
        CString py;
        m_MJPGList.GetDlgItem(IDC_STATIC_CONTACT_PINYIN)->GetWindowText(py);
        if (!py.IsEmpty())
        {
            alias = Util::StringOp::FromCString(py);
            for (std::size_t i = 0; i < alias.size(); ++i)
            {
                if (alias[i] == '\?')
                {
                    alias[i] = '_';
                }
            }
        }
        return alias;
    }

    int CContactDlg::GetTypeListSelected(void)
    {
        POSITION pos = m_lsType.GetFirstSelectedItemPosition();
        if (pos != NULL)
        {
            int index = m_lsType.GetNextSelectedItem (pos);
            return index;
        }
        return -1;
    }

    void CContactDlg::SetTypeListSelected(int index)
    {
        POSITION pos = m_lsType.GetFirstSelectedItemPosition();   
        while (pos != NULL)
        {   
            int iSel = m_lsType.GetNextSelectedItem(pos);   
            m_lsType.SetItemState(iSel, 0, LVIS_SELECTED);   
        }
        m_lsType.SetItemState(index, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
    }

    void CContactDlg::Search(std::string filter)
    {
        m_sListSearchFilter = filter;
        m_MJPGList.GetDlgItem(IDC_STATIC_CONTACT_PINYIN)->SetWindowText(_T(""));
        POSITION pos = m_lsType.GetFirstSelectedItemPosition();   
        while (pos != NULL)
        {   
            int iSel = m_lsType.GetNextSelectedItem(pos);   
            m_lsType.SetItemState(iSel, 0, LVIS_SELECTED);   
        }
        m_lsType.SetItemState(m_lsType.GetItemCount() - 1, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
        ShowItemsInList();
    }

    LRESULT CContactDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
    {
        // TODO: Add your specialized code here and/or call the base class
        switch (message)
        {
        case WM_VSCROLL:
            ScrollItemsInList(wParam, lParam);
            break;
        };

        return CDialog::WindowProc(message, wParam, lParam);
    }
}
