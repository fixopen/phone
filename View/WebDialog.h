#if !defined(AFX_WEBDIALOG_H__84B1F0C3_C50A_4848_ABEA_3AB48C8C96B9__INCLUDED_)
#define AFX_WEBDIALOG_H__84B1F0C3_C50A_4848_ABEA_3AB48C8C96B9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WebDialog.h : header file
//

#include "../Control/CeBtnST.h"
#include "../Control/CELineEdit.h"
#include "../Control/CEComboBox_.h"
#include "../Control/CEListCtrl.h"
#include "../control/MJPGStatic.h"

//#include "Exdisp.h"

/////////////////////////////////////////////////////////////////////////////
// CWebDialog dialog

class CWebDialog : public CDialog {
    // Construction
public:
    CWebDialog(CWnd* pParent = NULL);   // standard constructor

public:

    //  CWnd browserContainer_;
    //  IWebBrowser2* browser_;
    BOOL m_bPlaying;
    HWND m_hIEWnd;
    CRect m_IERect;
    CString url_;
    void SendtoWebMessage(UINT message, WPARAM w, LPARAM l);
    void ExitWeb();
    char m_sUrlList[10][128];
    int m_nUrlCount;
    void SetHistoryList();
    void AddHistory(CString s);

    //BOOL    m_bIsExitThread;

    CMJPGStatic m_MJPGList;

    void ShowWindow_(BOOL bShow);

    CCEComboBox_ m_cmbURL;
    //  CCEBmpButton m_btn;
    CCERectEdit m_edit;
    //  CCEBmpButton m_btnClose;
    /*  
    CCEStatic m_sticOperation;
    CCEStatic m_sticPanel;
    CCEStatic m_sticBack;
    CCEFrameStatic m_sticBackground;
    CCEListCtrl m_lstPlayList;
    CImageList *m_pImageList;
    TCHAR       m_chDir[128];
    void SetPlayList(TCHAR *dir, int local);
    */
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
    afx_msg LRESULT OnSelectURL(WPARAM w, LPARAM l);
    afx_msg LRESULT OnIEClick(WPARAM w, LPARAM l);
    //  afx_msg LRESULT OnUrlSelect(WPARAM w, LPARAM l);
    //  afx_msg void OnClickPlayList(NMHDR* pNMHDR, LRESULT* pResult);
    //  afx_msg LRESULT OnListCltrlClick(WPARAM w, LPARAM l);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WEBDIALOG_H__84B1F0C3_C50A_4848_ABEA_3AB48C8C96B9__INCLUDED_)
