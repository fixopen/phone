#if !defined(AFX_ALARMSHOWDLG_H__12A8AE91_FA94_4D46_862D_22225F439F9968__INCLUDED_)
#define AFX_ALARMSHOWDLG_H__12A8AE91_FA94_4D46_862D_22225F439F9968__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AlarmShowDlg.h : header file
#include "../Resource.h"
#include "../Control/CEDialog.h"
#include "../Control/MJPGStatic.h"

namespace View {
    using namespace Control;
    /////////////////////////////////////////////////////////////////////////////
    // CWarningNoFlashDlg dialog

    class CWarningNoFlashDlg : public CCEDialog
    {
        // Construction
    public:
        CMJPGStatic		m_MJPGList;
        CWarningNoFlashDlg(CWnd* pParent = NULL);   // standard constructor
        void SetType(int type);

        // Dialog Data
        //{{AFX_DATA(CWarningNoFlashDlg)
        enum { IDD = IDD_DIALOG_NOFLASH };
        // NOTE: the ClassWizard will add data members here
        //}}AFX_DATA


        // Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(CWarningNoFlashDlg)
    protected:
        virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
        //}}AFX_VIRTUAL

        // Implementation
    protected:

        // Generated message map functions
        //{{AFX_MSG(CWarningNoFlashDlg)
        virtual BOOL OnInitDialog();
        afx_msg LRESULT OnClickMJPG(WPARAM w, LPARAM l);
        //}}AFX_MSG
        DECLARE_MESSAGE_MAP()
    };

    //{{AFX_INSERT_LOCATION}}
    // Microsoft Visual C++ will insert additional declarations immediately before the previous line.
}

#endif // !defined(AFX_ALARMSHOWDLG_H__12A8AE91_FA94_4D46_862D_22225F439F99__INCLUDED_)
