#if !defined(AFX_FIREWALLDLG_H__37E23B5A_3692_4738_ABD1_37AA4B0386B5__INCLUDED_)
#define AFX_FIREWALLDLG_H__37E23B5A_3692_4738_ABD1_37AA4B0386B5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// firewalldlg.h : header file
//
#include "../Control/CEStatic.h"
#include "../Control/CeBtnST.h"
#include "../Control/CEDialog.h"
#include "../Control/CECombobox_.h"
#include "../Control/CERadioButton.h"
#include "../control/MJPGStatic.h"

/////////////////////////////////////////////////////////////////////////////
// CFireWallDlg dialog

class CFireWallDlg : public CDialog
{
    // Construction
public:
    CCEComboBox_ m_cmbTime;

    CFireWallDlg(CWnd* pParent = NULL);   // standard constructor
    void OnStart(BOOL isStart = TRUE);

    // Dialog Data
    //{{AFX_DATA(CFireWallDlg)
    enum { IDD = IDD_DIALOG_CACULATER };
    // NOTE: the ClassWizard will add data members here
    //}}AFX_DATA


    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CFireWallDlg)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

    // Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CFireWallDlg)
    virtual BOOL OnInitDialog();
    afx_msg void OnTimer(UINT nIDEvent);
    //}}AFX_MSG
    afx_msg void OnButtonFireWallOk();
    afx_msg void OnButtonFireWallCancel();
    afx_msg LRESULT OnClickMJPG(WPARAM w, LPARAM l);
    DECLARE_MESSAGE_MAP()
private:

    /*
    CCEStatic m_sticTitle;
    CCEStatic m_sticTimePart1;
    CCEStatic m_sticTimePart2;
    CCEBmpButton m_btnOk;
    CCEBmpButton m_btnCancel;
    */
    CCEButtonST m_chbEnableFireWall;


    CCERadioButton m_chbRefuseBlacklist;
    CCERadioButton m_chbContactExceptBlacklist;
    CCERadioButton m_chbOnlyVIP;
    CCERadioButton m_chbRefuseAllByTime;

    CMJPGStatic		m_MJPGList;

    //	CCEFramePartStatic m_sticBackground;
public:
    void SetParameters(bool enable, int type, int duration);
    int GetFirewallType();
    void BrushWindow(void);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FIREWALLDLG_H__37E23B5A_3692_4738_ABD1_37AA4B0386B5__INCLUDED_)
