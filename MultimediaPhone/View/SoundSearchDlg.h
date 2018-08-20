#if !defined(AFX_SOUNDSEARCHDLG_H__E2E6D87E_F7C4_4BB5_B083_DB5A8EE16084__INCLUDED_)
#define AFX_SOUNDSEARCHDLG_H__E2E6D87E_F7C4_4BB5_B083_DB5A8EE16084__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// soundsearchdlg.h : header file
#include "../Resource.h"
#include "../Control/CEStatic.h"
#include "../Control/CeBtnST.h"
#include "../Control/CELineEdit.h"
#include "../Control/CEDialog.h"
#include "../Control/MJPGStatic.h"
#include "../Control/DateCtrl.h"

namespace View {
    using namespace Control;
    /////////////////////////////////////////////////////////////////////////////
    // CSoundSearchDlg dialog

    class CSoundSearchDlg : public CCEDialog
    {
        // Construction
    public:
        CSoundSearchDlg(CWnd* pParent = NULL);   // standard constructor

        // Dialog Data
        //{{AFX_DATA(CSoundSearchDlg)
        enum { IDD = IDD_DIALOG_SOUNDSEARCH };
        // NOTE: the ClassWizard will add data members here
        //}}AFX_DATA


        // Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(CSoundSearchDlg)
    protected:
        virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
        //}}AFX_VIRTUAL

        // Implementation
    protected:

        // Generated message map functions
        //{{AFX_MSG(CSoundSearchDlg)
        virtual BOOL OnInitDialog();
        //}}AFX_MSG
        afx_msg void OnButtonSearchSoundCancel();
        afx_msg void OnButtonSearchSoundSearch();
        afx_msg LRESULT OnClickMJPG(WPARAM w, LPARAM l);
        DECLARE_MESSAGE_MAP()
    private:
        CMJPGStatic		m_MJPGList;
        CFont m_Font;

        CCEStatic m_sticTitle;
        CCEStatic m_sticTip;
        CCEStatic m_sticTimeRange;
        CCEStatic m_sticFrom;
        CCEStatic m_sticTo;
        CCEStatic m_sticName;
        CCEStatic m_sticNumber;
        CCERectEdit m_edtName;
        CCERectEdit m_edtNumber;
        CDateCtrl m_dtFrom;
        CDateCtrl m_dtTo;

        CCEBmpButton m_btnSearch;
        CCEBmpButton m_btnCancel;
        CCEFramePartStatic m_sticBackground;
    };

    //{{AFX_INSERT_LOCATION}}
    // Microsoft Visual C++ will insert additional declarations immediately before the previous line.
}

#endif // !defined(AFX_SOUNDSEARCHDLG_H__E2E6D87E_F7C4_4BB5_B083_DB5A8EE16084__INCLUDED_)
