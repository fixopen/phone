#if !defined(AFX_MAINVIDEODLG_H__B24F63BA_C6CD_4AB1_842E_711E35A94A6F__INCLUDED_)
#define AFX_MAINVIDEODLG_H__B24F63BA_C6CD_4AB1_842E_711E35A94A6F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MainVideoDlg.h : header file
//
#include "../control/CeBtnST.h"
#include "../control/CEListCtrl.h"
#include "../control/CEStatic.h"
#include "../control/CEProcessBar.h"
#include "../Control/MJPGStatic.h"
#include "../resource.h"
#include "PlayerDlg.h"
/////////////////////////////////////////////////////////////////////////////
// CMainVideoDlg dialog

class CMainVideoDlg : public CDialog {
    //control
public:
    BOOL m_bIsPausebyEvent;
    TCHAR m_chDir[128];

    CCEProcessBar m_prgPlayTime;
    CMJPGStatic m_MJPGList;
    CCEMoveTxtStatic* m_MoveText;

    int m_ClickType;
    int m_IsPlay;
    BOOL m_IsSound;
    int m_Volume;
    unsigned int m_pageSize;
    unsigned int m_selectPageCount;   //被选中文件总页数
    unsigned int m_selectCurrentPage;   //当前显示的页数
    std::vector<CString> m_MovieList;
    std::vector<CString> m_ShowList;

    void SetVideo(CString filename);
    void OnExit_() {
        OnExit();
    }
    void SetVolume();
    void ChangeVolume(int w);
    void OnFirst();
    void OnLast();
    void ShowArrayInList(std::vector<CString> fileName);
    void PageUp();
    void PageDown();
    void ClearAll();
    void CalculatePage(int dataCount);
    void OnClickShowList(int unitNO);
    void OnDBClickShowList(int unitNO);

public:
    CPlayerDlg* playerDlg_; 
    // Construction
public:
    CMainVideoDlg(CWnd* pParent = NULL);   // standard constructor

    // Dialog Data
    //{{AFX_DATA(CMainVideoDlg)
    enum { IDD = IDD_DIALOG_CACULATER };
    // NOTE: the ClassWizard will add data members here
    //}}AFX_DATA

    afx_msg void OnTimer(UINT nIDEvent);
    afx_msg void OnOpenFile();


    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CMainVideoDlg)
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

    // Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CMainVideoDlg)
    virtual BOOL OnInitDialog();
    afx_msg void OnExit();
    afx_msg void OnPlayer();
    afx_msg void OnStop();
    afx_msg void OnPre();
    afx_msg void OnBack();
    afx_msg void OnPlayerAll();
    afx_msg void OnMute();
    afx_msg LRESULT OnPregress(WPARAM w, LPARAM l);
    afx_msg LRESULT OnOutEvent(WPARAM w, LPARAM l);
    afx_msg LRESULT OnClickMJPG(WPARAM w, LPARAM l);
    afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINVIDEODLG_H__B24F63BA_C6CD_4AB1_842E_711E35A94A6F__INCLUDED_)
