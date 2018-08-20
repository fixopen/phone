#if !defined(AFX_ALARMSHOWDLG_H__12A8AE91_FA94_4D46_862D_22225F439F99__INCLUDED_)
#define AFX_ALARMSHOWDLG_H__12A8AE91_FA94_4D46_862D_22225F439F99__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AlarmShowDlg.h : header file
//
#include "../Control/CEDialog.h"
#include "../Control/MJPGStatic.h"

namespace View {
    /////////////////////////////////////////////////////////////////////////////
    // CAlarmShowDlg dialog

    class CAlarmShowDlg : public CCEDialog
    {
        // Construction
    public:
        int				m_nAlarmID;

        CMJPGStatic		m_MJPGList;
        void            SetTxt(int nAlarmID, CString sTime, CString sTitle, CString sContent, std::string sRing, BOOL isADDList = TRUE);

        CAlarmShowDlg(CWnd* pParent = NULL);   // standard constructor
        void ShowWindow_(int cmdshow);

        // Dialog Data
        //{{AFX_DATA(CAlarmShowDlg)
        enum { IDD = IDD_DIALOG_ALARM };
        // NOTE: the ClassWizard will add data members here
        //}}AFX_DATA


        // Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(CAlarmShowDlg)
    protected:
        virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
        //}}AFX_VIRTUAL

        // Implementation
    protected:

        // Generated message map functions
        //{{AFX_MSG(CAlarmShowDlg)
        virtual BOOL OnInitDialog();
        afx_msg LRESULT OnClickMJPG(WPARAM w, LPARAM l);
        afx_msg void OnTimer(UINT nIDEvent);
        //}}AFX_MSG
        DECLARE_MESSAGE_MAP()
    };

    //{{AFX_INSERT_LOCATION}}
    // Microsoft Visual C++ will insert additional declarations immediately before the previous line.
}

#endif // !defined(AFX_ALARMSHOWDLG_H__12A8AE91_FA94_4D46_862D_22225F439F99__INCLUDED_)
