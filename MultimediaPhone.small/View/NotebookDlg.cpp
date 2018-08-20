// NotebookDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NotebookDlg.h"
#include "../MultimediaPhone.h"
#include "../MultimediaPhoneDlg.h"
#include "../Data/LanguageResource.h"
#include "../Data/SkinStyle.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNotebookDlg dialog

namespace View {
    CNotebookDlg::CNotebookDlg(CWnd* pParent /*=NULL*/)
        : CDialog(CNotebookDlg::IDD, pParent)
    {
        //{{AFX_DATA_INIT(CNotebookDlg)
        // NOTE: the ClassWizard will add member initialization here
        //}}AFX_DATA_INIT
    }


    void CNotebookDlg::DoDataExchange(CDataExchange* pDX)
    {
        CDialog::DoDataExchange(pDX);
        //{{AFX_DATA_MAP(CNotebookDlg)
        // NOTE: the ClassWizard will add DDX and DDV calls here
        //}}AFX_DATA_MAP
    }


    BEGIN_MESSAGE_MAP(CNotebookDlg, CDialog)
        //{{AFX_MSG_MAP(CNotebookDlg)
        //}}AFX_MSG_MAP
        ON_BN_CLICKED(IDC_BUTTON_NOTE_OK, OnButtonNoteOk)
        ON_BN_CLICKED(IDC_BUTTON_NOTE_PREV, OnButtonNotePrev)
        ON_BN_CLICKED(IDC_BUTTON_NOTE_NEXT, OnButtonNoteNext)
        ON_BN_CLICKED(IDC_BUTTON_NOTE_CLEAR, OnButtonNoteClear)
        ON_BN_CLICKED(IDC_BUTTON_NOTE_CLOSE, OnButtonNoteClose)
        ON_MESSAGE(WM_CLICKMJPG_TOAPP, OnClickMJPG)
    END_MESSAGE_MAP()

    /////////////////////////////////////////////////////////////////////////////
    // CNotebookDlg message handlers

    BOOL CNotebookDlg::OnInitDialog() 
    {
        CDialog::OnInitDialog();
        m_WriteStatic.Create(L"", WS_VISIBLE|WS_CHILD, CRect(8, 55, 791, 409), this, 8950);
        m_WriteStatic.SetWrite((TCHAR*)(LPCTSTR)(::notePath + "/1.bmp"));

        m_MJPGList.Create(L"", WS_VISIBLE|WS_CHILD, CRect(0, 0, 800, 420), this);
        m_MJPGList.SetCurrentLinkFile(_T(".\\adv\\mjpg\\k1\\中文\\笔迹记事.xml"));
        m_MJPGList.SetMJPGRect(CRect(0, 0, 800, 420));

        m_nIndex = 0;

        CString s;
        s.Format(_T("%d/20 %s"), m_nIndex + 1, Util::StringOp::ToCString(Data::LanguageResource::Get(Data::RI_NOTE_PAGE)));
        m_MJPGList.SetUnitText(0, s, FALSE);

        return TRUE;  // return TRUE unless you set the focus to a control
        // EXCEPTION: OCX Property Pages should return FALSE
    }

    LRESULT CNotebookDlg::OnClickMJPG(WPARAM w, LPARAM l)
    {
        LRESULT result = 0;
        switch (w)
        {
        case 5:
            OnButtonNoteOk();
            break;
        case 1:
            OnButtonNotePrev();
            break;
        case 2:
            OnButtonNoteNext();
            break;
        case 3:
            OnButtonNoteClear();
            break;
        case 4:
            OnButtonNoteClose();
            break;
        default:
            break;
        }
        return result;
    }

    void CNotebookDlg::SetPageIndex()
    {
        CString s;
        s.Format(_T("%d/20 %s"), m_nIndex+1, Util::StringOp::ToCString(Data::LanguageResource::Get(Data::RI_NOTE_PAGE)));

        m_MJPGList.SetUnitText(0, s, TRUE);
    }

    void CNotebookDlg::OnButtonNoteOk()
    {
        std::string filename = Util::StringOp::FromCString(::notePath);
        filename += "/" + Util::StringOp::FromInt(m_nIndex + 1);
        filename += ".bmp";
        m_WriteStatic.GetWrite((LPTSTR)(LPCTSTR)Util::StringOp::ToCString(filename));
        ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->SwitchPanel_(IDC_BUTTON_MAIN);
    }

    void CNotebookDlg::OnButtonNotePrev()
    {
        std::string filename = Util::StringOp::FromCString(::notePath);
        filename += "/" + Util::StringOp::FromInt(m_nIndex + 1);
        filename += ".bmp";
        m_WriteStatic.GetWrite((LPTSTR)(LPCTSTR)Util::StringOp::ToCString(filename));
        //m_WriteStatic.GetWrite(filename[m_nIndex]);
        --m_nIndex;
        if (m_nIndex < 0)
            m_nIndex = 19;

        filename = Util::StringOp::FromCString(::notePath);
        filename += "/" + Util::StringOp::FromInt(m_nIndex + 1);
        filename += ".bmp";
        m_WriteStatic.SetWrite((LPTSTR)(LPCTSTR)Util::StringOp::ToCString(filename));
        // 	m_WriteStatic.SetWrite(filename[m_nIndex]);

        SetPageIndex();
    }

    void CNotebookDlg::OnButtonNoteNext()
    {
        std::string filename = Util::StringOp::FromCString(::notePath);
        filename += "/" + Util::StringOp::FromInt(m_nIndex + 1);
        filename += ".bmp";
        m_WriteStatic.GetWrite((LPTSTR)(LPCTSTR)Util::StringOp::ToCString(filename));
        //	m_WriteStatic.GetWrite(filename[m_nIndex]);
        ++m_nIndex;
        if (m_nIndex > 19)
            m_nIndex = 0;
        filename = Util::StringOp::FromCString(::notePath);
        filename += "/" + Util::StringOp::FromInt(m_nIndex + 1);
        filename += ".bmp";
        m_WriteStatic.SetWrite((LPTSTR)(LPCTSTR)Util::StringOp::ToCString(filename));
        //	m_WriteStatic.SetWrite(filename[m_nIndex]);

        SetPageIndex();
    }

    void CNotebookDlg::OnButtonNoteClear()
    {
        m_WriteStatic.ClearDC();
    }

    void CNotebookDlg::OnButtonNoteClose()
    {
        //((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->SwitchPanel_(IDC_BUTTON_MAIN);
        ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pNotebookDlg->ShowWindow(SW_HIDE);
        ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pMainDlg->SetStatusAll(FALSE);
    }
}
