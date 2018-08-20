#if !defined(AFX_SCREENSAVEDLG_H__8F155D74_4CCD_45EF_ACBF_117CD830A649__INCLUDED_)
#define AFX_SCREENSAVEDLG_H__8F155D74_4CCD_45EF_ACBF_117CD830A649__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ScreenSaveDlg.h : header file
//
#include "PlayerDlg.h"
#include "../Control/CEStatic.h"
#include "../Control/CELineEdit.h"

namespace View {
    using namespace Control;
    /////////////////////////////////////////////////////////////////////////////
    // CScreenSaveDlg dialog

    class CScreenSaveDlg : public CDialog
    {
    public:
        CPlayerDlg	*playerDlg_; 
        CCEStatic	m_static, m_static1;
        //	CCEStatic	m_warningstatic;
        CCERectEdit	m_passwndEdit;
        CString 	m_password;

        int type_;		//0 Ê±¼ä 1 Í¼Æ¬
        void SetSaveScreenType(int type, BOOL Redraw = FALSE);
        void SetSaveScreenPhoto_();
        void OnExit();
        BOOL IsHaveScreenPwnd();
        void GetPassWord();

        // Construction
    public:
        CScreenSaveDlg(CWnd* pParent = NULL);   // standard constructor

        // Dialog Data
        //{{AFX_DATA(CScreenSaveDlg)
        enum { IDD = IDD_DIALOG_SCREENSAVE };
        // NOTE: the ClassWizard will add data members here
        //}}AFX_DATA


        // Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(CScreenSaveDlg)
    protected:
        virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
        //}}AFX_VIRTUAL

        // Implementation
    protected:

        // Generated message map functions
        //{{AFX_MSG(CScreenSaveDlg)
        virtual BOOL OnInitDialog();
        afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
        afx_msg void OnTimer(UINT nIDEvent);
        afx_msg void OnPaint();
        afx_msg BOOL OnEraseBkgnd(CDC* pDC);
        afx_msg void OnChangeEdit1();
        afx_msg LRESULT OnOutEvent(WPARAM w, LPARAM l);
        //}}AFX_MSG
        DECLARE_MESSAGE_MAP()
    };

    //{{AFX_INSERT_LOCATION}}
    // Microsoft Visual C++ will insert additional declarations immediately before the previous line.
}

#endif // !defined(AFX_SCREENSAVEDLG_H__8F155D74_4CCD_45EF_ACBF_117CD830A649__INCLUDED_)
