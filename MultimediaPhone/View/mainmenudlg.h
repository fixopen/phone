#if !defined(AFX_MAINMENUDLG_H__E608C677_E0D6_4BBF_9ED5_52B5DEC2EB2E__INCLUDED_)
#define AFX_MAINMENUDLG_H__E608C677_E0D6_4BBF_9ED5_52B5DEC2EB2E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MainMenuDlg.h : header file
//
#include "../Control/CeBtnST.h"
#include "../Control/CEStatic.h"
#include "../Control/SoftKey.h"
#include "PlayerDlg.h"

namespace View {
    using namespace Control;
    /////////////////////////////////////////////////////////////////////////////
    // CMainMenuDlg dialog

    class CMainMenuDlg : public CDialog
    {
    public:
        //	CPlayerDlg	*playerDlg_; 
        void SetVideo();
        void SetSkinStyle();

        void OnVideoBtn();
        void OnPhotoBtn();
        void OnMp3Btn();
        void OnScreenSaveBtn();
        void OnCalculBtn();
        void OnLunarderBtn();
        void ShowWindow_(int nCmdShow, int type = 0);
        // Construction
    public:
        CMainMenuDlg(CWnd* pParent = NULL);   // standard constructor
    public:
        virtual ~CMainMenuDlg();
        // Dialog Data
        //{{AFX_DATA(CMainMenuDlg)
        enum { IDD = IDD_DIALOG_MAINMENU };

        /*	
        CCEButtonST	m_ScreenSaveBtn;
        CCEButtonST m_CalcuBtn;
        CCEButtonST	m_CalenBtn;
        CCEButtonST	m_MusicBtn;
        CCEButtonST	m_PhotoBtn;
        CCEButtonST	m_VideoBtn;
        */
        CCEIconSoftKey  m_AppSoftKey;
        CCEFrameStatic m_backGroudStatic;

        //}}AFX_DATA


        // Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(CMainMenuDlg)
    protected:
        virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
        //}}AFX_VIRTUAL

        // Implementation
    protected:

        // Generated message map functions
        //{{AFX_MSG(CMainMenuDlg)
        virtual BOOL OnInitDialog();
        afx_msg void OnPaint();
        afx_msg LRESULT OnAppSoftKey(WPARAM wParam, LPARAM lParam);
        afx_msg void OnTimer(UINT nIDTimer);
        //}}AFX_MSG
        DECLARE_MESSAGE_MAP()
    };

    //{{AFX_INSERT_LOCATION}}
    // Microsoft Visual C++ will insert additional declarations immediately before the previous line.
}

#endif // !defined(AFX_MAINMENUDLG_H__E608C677_E0D6_4BBF_9ED5_52B5DEC2EB2E__INCLUDED_)
