#if !defined(AFX_WEBDIALOG_H__84B1F0C3_C50A_4848_ABEA_3AB48C8C96B9__INCLUDED_)
#define AFX_WEBDIALOG_H__84B1F0C3_C50A_4848_ABEA_3AB48C8C96B9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WebDialog.h : header file
//
#include "../Resource.h"
#include "../Control/CeBtnST.h"
#include "../Control/CELineEdit.h"
#include "../Control/CEComboBox_.h"
#include "../Control/CEListCtrl.h"
#include "../Control/MJPGStatic.h"
#include "../Logical/explorer_browser.h"

//#include "Exdisp.h"

namespace View {
    /////////////////////////////////////////////////////////////////////////////
    // CWebDialog dialog
    using namespace Control;
    class CWebDialog : public CDialog
    {
        // Construction
    public:
        CWebDialog(CWnd* pParent = NULL);   // standard constructor

    public:
        Logical::CExplorer_browser browser_;
        BOOL m_bPlaying;
        HWND m_hIEWnd;
        CRect m_IERect;
        CString url_;
        char m_sUrlList[10][128];
        int m_nUrlCount;
        CMJPGStatic m_MJPGList;
        CCEComboBox_ m_cmbURL;
        CCERectEdit m_edit;

        void SendtoWebMessage(UINT message, WPARAM w, LPARAM l);
        void ExitWeb();
        void SetHistoryList();
        void AddHistory(CString s);
        void ShowWindow_(BOOL bShow);
        void SetURL(CString url);

        // Dialog Data
        //{{AFX_DATA(CWebDialog)
        enum { IDD = IDD_DIALOG_WEB };
        // NOTE: the ClassWizard will add data members here
        //}}AFX_DATA


        // Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(CWebDialog)
    protected:
        virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
        virtual void OnOk();
        //}}AFX_VIRTUAL

        // Implementation
    protected:

        // Generated message map functions
        //{{AFX_MSG(CWebDialog)
        virtual BOOL OnInitDialog();
        afx_msg LRESULT OnClickMJPG(WPARAM w, LPARAM l);
        afx_msg LRESULT OnEnter(WPARAM w, LPARAM l);
        afx_msg LRESULT OnKillWEBShow(WPARAM w, LPARAM l);
        afx_msg LRESULT OnSetLink(WPARAM w, LPARAM l);
        afx_msg void OnTimer(UINT nIDEvent);
        afx_msg LRESULT OnInputURL(WPARAM w, LPARAM l);
        afx_msg LRESULT OnEditURL(WPARAM w, LPARAM l);
        afx_msg LRESULT OnSelectURL(WPARAM w, LPARAM l);
        afx_msg LRESULT OnIEClick(WPARAM w, LPARAM l);
        //}}AFX_MSG
        DECLARE_MESSAGE_MAP()
    };

    //{{AFX_INSERT_LOCATION}}
    // Microsoft Visual C++ will insert additional declarations immediately before the previous line.
}

#endif // !defined(AFX_WEBDIALOG_H__84B1F0C3_C50A_4848_ABEA_3AB48C8C96B9__INCLUDED_)
